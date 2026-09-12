#ifndef REX_GRAPHICS_GTA4_NATIVE_FRAME_CONSTANT_ARENA_H_
#define REX_GRAPHICS_GTA4_NATIVE_FRAME_CONSTANT_ARENA_H_

#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

namespace rex::graphics::gta4_native {

// A frame-slot cache only retains non-owning immutable identities and POD
// allocation metadata. The command list owns the underlying version bytes
// while recording, and the slot fence protects the resulting GPU allocation.
enum class FrameConstantKind : uint8_t {
  kVertex,
  kPixel,
  kShared,
};

struct FrameConstantIdentity {
  FrameConstantKind kind = FrameConstantKind::kVertex;
  uint64_t immutable_identity = 0;

  constexpr bool operator==(const FrameConstantIdentity&) const = default;
};

struct FrameConstantIdentityHash {
  size_t operator()(const FrameConstantIdentity& identity) const noexcept {
    size_t hash = std::hash<uint64_t>{}(identity.immutable_identity);
    hash ^= std::hash<uint8_t>{}(uint8_t(identity.kind)) + size_t(0x9E3779B9u) + (hash << 6) +
            (hash >> 2);
    return hash;
  }
};

// Open addressing with generation-stamped buckets makes ResetGeneration O(1):
// stale buckets are ignored without walking or destroying them. Keys and
// values are deliberately restricted to POD so a stale generation cannot
// retain ownership of command or GPU resources.
template <typename Key, typename Value, typename Hash = std::hash<Key>,
          typename Equal = std::equal_to<Key>>
class FrameGenerationMap {
 public:
  static_assert(std::is_trivially_copyable_v<Key>);
  static_assert(std::is_trivially_copyable_v<Value>);

  struct InsertResult {
    Value* value = nullptr;
    bool inserted = false;

    explicit operator bool() const { return value != nullptr; }
  };

  Value* Find(const Key& key) {
    Bucket* bucket = FindBucket(key);
    return bucket ? &bucket->value : nullptr;
  }

  const Value* Find(const Key& key) const {
    const Bucket* bucket = FindBucket(key);
    return bucket ? &bucket->value : nullptr;
  }

  InsertResult Insert(const Key& key, const Value& value) {
    if (Value* existing = Find(key)) {
      return {existing, false};
    }
    if (!EnsureInsertionCapacity()) {
      return {};
    }
    Bucket* bucket = InsertWithoutGrowth(key, value);
    if (!bucket) {
      return {};
    }
    return {&bucket->value, true};
  }

  bool CanResetGeneration() const {
    return generation_ != std::numeric_limits<uint64_t>::max();
  }

  bool ResetGeneration() {
    if (!CanResetGeneration()) {
      return false;
    }
    ++generation_;
    size_ = 0;
    return true;
  }

  // Inspect only this generation; stale POD buckets retain no owners.
  template <typename Visitor> void ForEach(Visitor&& visitor) const {
    for (const auto& bucket : buckets_) if (bucket.generation == generation_) visitor(bucket.key, bucket.value);
  }

  size_t size() const { return size_; }
  size_t bucket_count() const { return buckets_.size(); }

 private:
  static constexpr size_t kInitialBucketCount = 32;
  static_assert((kInitialBucketCount & (kInitialBucketCount - 1)) == 0);

  struct Bucket {
    Key key{};
    Value value{};
    uint64_t generation = 0;
  };

  Bucket* FindBucket(const Key& key) {
    return const_cast<Bucket*>(std::as_const(*this).FindBucket(key));
  }

  const Bucket* FindBucket(const Key& key) const {
    if (buckets_.empty()) {
      return nullptr;
    }
    const size_t mask = buckets_.size() - 1;
    size_t index = hasher_(key) & mask;
    for (size_t probe = 0; probe < buckets_.size(); ++probe) {
      const Bucket& bucket = buckets_[index];
      if (bucket.generation != generation_) {
        return nullptr;
      }
      if (equal_(bucket.key, key)) {
        return &bucket;
      }
      index = (index + 1) & mask;
    }
    return nullptr;
  }

  bool EnsureInsertionCapacity() {
    if (buckets_.empty()) {
      buckets_.resize(kInitialBucketCount);
      return true;
    }
    if (size_ < buckets_.size() / 2) {
      return true;
    }
    if (buckets_.size() > std::numeric_limits<size_t>::max() / 2) {
      return false;
    }
    return Rehash(buckets_.size() * 2);
  }

  bool Rehash(size_t bucket_count) {
    std::vector<Bucket> previous = std::move(buckets_);
    const uint64_t previous_generation = generation_;
    buckets_.clear();
    buckets_.resize(bucket_count);
    generation_ = 1;
    const size_t previous_size = size_;
    size_ = 0;
    for (const Bucket& bucket : previous) {
      if (bucket.generation == previous_generation &&
          !InsertWithoutGrowth(bucket.key, bucket.value)) {
        return false;
      }
    }
    return size_ == previous_size;
  }

  Bucket* InsertWithoutGrowth(const Key& key, const Value& value) {
    if (buckets_.empty()) {
      return nullptr;
    }
    const size_t mask = buckets_.size() - 1;
    size_t index = hasher_(key) & mask;
    for (size_t probe = 0; probe < buckets_.size(); ++probe) {
      Bucket& bucket = buckets_[index];
      if (bucket.generation != generation_) {
        bucket.key = key;
        bucket.value = value;
        bucket.generation = generation_;
        ++size_;
        return &bucket;
      }
      index = (index + 1) & mask;
    }
    return nullptr;
  }

  std::vector<Bucket> buckets_;
  size_t size_ = 0;
  uint64_t generation_ = 1;
  Hash hasher_{};
  Equal equal_{};
};

struct FrameConstantReservation {
  size_t offset = 0;
  size_t byte_size = 0;
  bool reused = false;
};

// CPU-side index for a slot's host-visible BDA buffer. The byte cursor and
// lookup generation are reset only after that slot's submission fence, or
// after an unsubmitted command buffer has been rolled back.
class FrameConstantArenaIndex {
 public:
  bool SetByteCapacity(size_t byte_capacity) {
    if (in_flight_submission_ || reservation_count() || byte_cursor_) {
      return false;
    }
    byte_capacity_ = byte_capacity;
    return true;
  }

  std::optional<FrameConstantReservation> Find(const FrameConstantIdentity& identity) const {
    const Reservation* reservation = reservations_.Find(identity);
    if (!reservation) {
      return std::nullopt;
    }
    return FrameConstantReservation{reservation->offset, reservation->byte_size, true};
  }

  std::optional<FrameConstantReservation> FindOrReserve(const FrameConstantIdentity& identity,
                                                        size_t byte_size, size_t alignment) {
    if (!identity.immutable_identity || !byte_size || !alignment ||
        (alignment & (alignment - 1)) != 0) {
      return std::nullopt;
    }
    if (const Reservation* existing = reservations_.Find(identity)) {
      if (existing->byte_size != byte_size || (existing->offset & (alignment - 1)) != 0) {
        return std::nullopt;
      }
      return FrameConstantReservation{existing->offset, existing->byte_size, true};
    }
    const size_t alignment_mask = alignment - 1;
    if (byte_cursor_ > std::numeric_limits<size_t>::max() - alignment_mask) {
      return std::nullopt;
    }
    const size_t aligned_offset = (byte_cursor_ + alignment_mask) & ~alignment_mask;
    if (aligned_offset > byte_capacity_ || byte_size > byte_capacity_ - aligned_offset) {
      return std::nullopt;
    }
    const Reservation reservation{aligned_offset, byte_size};
    const auto insertion = reservations_.Insert(identity, reservation);
    if (!insertion || !insertion.inserted) {
      return std::nullopt;
    }
    byte_cursor_ = aligned_offset + byte_size;
    return FrameConstantReservation{aligned_offset, byte_size, false};
  }

  bool MarkSubmitted(uint64_t submission) {
    if (!submission || in_flight_submission_) {
      return false;
    }
    in_flight_submission_ = submission;
    return true;
  }

  bool CanResetAfterCompletion(uint64_t completed_submission) const {
    return (!in_flight_submission_ || in_flight_submission_ <= completed_submission) &&
           reservations_.CanResetGeneration();
  }

  bool ResetAfterCompletion(uint64_t completed_submission) {
    if (!CanResetAfterCompletion(completed_submission) || !reservations_.ResetGeneration()) {
      return false;
    }
    in_flight_submission_ = 0;
    byte_cursor_ = 0;
    return true;
  }

  bool ResetUnsubmitted() {
    if (in_flight_submission_ || !reservations_.ResetGeneration()) {
      return false;
    }
    byte_cursor_ = 0;
    return true;
  }

  size_t byte_capacity() const { return byte_capacity_; }
  size_t bytes_used() const { return byte_cursor_; }
  size_t reservation_count() const { return reservations_.size(); }
  size_t lookup_bucket_count() const { return reservations_.bucket_count(); }
  uint64_t in_flight_submission() const { return in_flight_submission_; }

 private:
  struct Reservation {
    size_t offset = 0;
    size_t byte_size = 0;
  };
  static_assert(std::is_trivially_copyable_v<Reservation>);

  FrameGenerationMap<FrameConstantIdentity, Reservation, FrameConstantIdentityHash> reservations_;
  size_t byte_capacity_ = 0;
  size_t byte_cursor_ = 0;
  uint64_t in_flight_submission_ = 0;
};

}  // namespace rex::graphics::gta4_native

#endif  // REX_GRAPHICS_GTA4_NATIVE_FRAME_CONSTANT_ARENA_H_
