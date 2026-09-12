#ifndef REX_GRAPHICS_GTA4_NATIVE_STATEFUL_CONSTANT_STATE_H_
#define REX_GRAPHICS_GTA4_NATIVE_STATEFUL_CONSTANT_STATE_H_

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <span>
#include <utility>
#include <vector>

#include "dirty_state_delta.h"

namespace rex::graphics::gta4_native {

// Payload offsets refer to bytes owned by ConstantPayloadDelta. Destination
// offsets refer to the canonical guest-endian constant block. Commands own
// these bytes, so guest memory may change immediately after queue submission.
struct ConstantDeltaRange {
  uint32_t destination_offset = 0;
  uint32_t payload_offset = 0;
  uint32_t byte_count = 0;

  constexpr bool operator==(const ConstantDeltaRange&) const = default;
};

struct ConstantPayloadDelta {
  std::vector<ConstantDeltaRange> ranges;
  std::vector<uint8_t> payload;
  bool complete_snapshot = false;

  bool empty() const { return ranges.empty(); }
};

inline bool ValidateConstantPayloadDelta(const ConstantPayloadDelta& delta, size_t block_size,
                                         bool require_complete_snapshot = false) {
  if (require_complete_snapshot && !delta.complete_snapshot) {
    return false;
  }
  if (delta.complete_snapshot &&
      (delta.ranges.size() != 1 || delta.ranges[0].destination_offset != 0 ||
       delta.ranges[0].payload_offset != 0 || delta.ranges[0].byte_count != block_size ||
       delta.payload.size() != block_size)) {
    return false;
  }
  size_t previous_end = 0;
  bool first = true;
  for (const ConstantDeltaRange& range : delta.ranges) {
    if (!range.byte_count || range.destination_offset > block_size ||
        range.byte_count > block_size - range.destination_offset ||
        range.payload_offset > delta.payload.size() ||
        range.byte_count > delta.payload.size() - range.payload_offset) {
      return false;
    }
    if (!first && range.destination_offset < previous_end) {
      return false;
    }
    previous_end = size_t(range.destination_offset) + range.byte_count;
    first = false;
  }
  return true;
}

inline bool CaptureConstantPayloadDelta(std::span<const uint8_t> source,
                                        const DirtyRangeSet& dirty_ranges,
                                        uint32_t bytes_per_element,
                                        ConstantPayloadDelta& result) {
  result = {};
  if (!bytes_per_element) {
    return false;
  }
  for (const DirtyElementRange& dirty : dirty_ranges.ranges) {
    if (!dirty.count || dirty.first > std::numeric_limits<uint32_t>::max() / bytes_per_element ||
        dirty.count > std::numeric_limits<uint32_t>::max() / bytes_per_element) {
      result = {};
      return false;
    }
    const uint32_t destination_offset = dirty.first * bytes_per_element;
    const uint32_t byte_count = dirty.count * bytes_per_element;
    if (destination_offset > source.size() || byte_count > source.size() - destination_offset ||
        result.payload.size() > std::numeric_limits<uint32_t>::max() - byte_count) {
      result = {};
      return false;
    }
    const uint32_t payload_offset = static_cast<uint32_t>(result.payload.size());
    const bool coalesce =
        !result.ranges.empty() &&
        result.ranges.back().destination_offset <=
            std::numeric_limits<uint32_t>::max() - result.ranges.back().byte_count &&
        result.ranges.back().destination_offset + result.ranges.back().byte_count ==
            destination_offset &&
        result.ranges.back().payload_offset <=
            std::numeric_limits<uint32_t>::max() - result.ranges.back().byte_count &&
        result.ranges.back().payload_offset + result.ranges.back().byte_count == payload_offset &&
        result.ranges.back().byte_count <= std::numeric_limits<uint32_t>::max() - byte_count;
    if (coalesce) {
      result.ranges.back().byte_count += byte_count;
    } else {
      result.ranges.push_back({destination_offset, payload_offset, byte_count});
    }
    result.payload.insert(result.payload.end(), source.begin() + destination_offset,
                          source.begin() + destination_offset + byte_count);
  }
  return ValidateConstantPayloadDelta(result, source.size());
}

inline bool CaptureCompleteConstantSnapshot(std::span<const uint8_t> source,
                                            ConstantPayloadDelta& result) {
  if (source.size() > std::numeric_limits<uint32_t>::max()) {
    result = {};
    return false;
  }
  result.ranges = {{0, 0, static_cast<uint32_t>(source.size())}};
  result.payload.assign(source.begin(), source.end());
  result.complete_snapshot = true;
  return ValidateConstantPayloadDelta(result, source.size(), true);
}

struct ConstantStateVersion {
  StateVersionStamp token{};
  uint64_t content_hash = 0;
  size_t byte_size = 0;
  ConstantPayloadDelta delta;
  uint32_t deferred_ancestor_count = 0;
  // The parent and materialized bytes are render-worker-owned memoization.
  // State bytes and token are immutable once the version is published.
  mutable std::shared_ptr<const ConstantStateVersion> parent;
  mutable std::shared_ptr<const std::vector<uint8_t>> materialized;
};

enum class ConstantApplyStatus : uint8_t {
  kApplied,
  kInvalidDelta,
  kNeedsCompleteSnapshot,
  kVersionSpaceExhausted,
};

struct ConstantApplyResult {
  ConstantApplyStatus status = ConstantApplyStatus::kApplied;
  bool changed = false;
  std::shared_ptr<const ConstantStateVersion> version;

  explicit operator bool() const { return status == ConstantApplyStatus::kApplied; }
};

// One instance is owned by the render worker for each device and shader
// stage. Published versions retain only copied deltas. A contiguous host view
// is reconstructed lazily on first consumer use and then memoized.
class AuthoritativeConstantState {
 public:
  static constexpr uint32_t kMaximumDeferredAncestors = 64;
  explicit AuthoritativeConstantState(size_t byte_size = 0,
                                      StateVersionStamp initial_version = {})
      : canonical_(byte_size), version_(initial_version) {}

  size_t byte_size() const { return canonical_.size(); }
  bool initialized() const { return initialized_; }
  const std::vector<uint8_t>& canonical() const { return canonical_; }
  const std::shared_ptr<const ConstantStateVersion>& current_version() const { return current_; }

  template <typename HashCallback>
  ConstantApplyResult Apply(const ConstantPayloadDelta& delta, HashCallback&& hash_callback) {
    if (!ValidateConstantPayloadDelta(delta, canonical_.size())) {
      return {ConstantApplyStatus::kInvalidDelta, false, current_};
    }
    if (!initialized_ && !delta.complete_snapshot) {
      return {ConstantApplyStatus::kNeedsCompleteSnapshot, false, current_};
    }

    ConstantPayloadDelta changed_delta;
    changed_delta.complete_snapshot = !initialized_;
    for (const ConstantDeltaRange& range : delta.ranges) {
      const uint8_t* source = delta.payload.data() + range.payload_offset;
      uint8_t* destination = canonical_.data() + range.destination_offset;
      if (initialized_ && std::memcmp(destination, source, range.byte_count) == 0) {
        continue;
      }
      const uint32_t payload_offset = static_cast<uint32_t>(changed_delta.payload.size());
      changed_delta.ranges.push_back(
          {range.destination_offset, payload_offset, range.byte_count});
      changed_delta.payload.insert(changed_delta.payload.end(), source,
                                   source + range.byte_count);
    }

    if (initialized_ && changed_delta.empty()) {
      return {ConstantApplyStatus::kApplied, false, current_};
    }
    if (!CanIncrementStateVersion(version_)) {
      return {ConstantApplyStatus::kVersionSpaceExhausted, false, current_};
    }
    for (const ConstantDeltaRange& range : changed_delta.ranges) {
      std::memcpy(canonical_.data() + range.destination_offset,
                  changed_delta.payload.data() + range.payload_offset, range.byte_count);
    }
    IncrementStateVersion(version_);
    initialized_ = true;

    auto next = std::make_shared<ConstantStateVersion>();
    next->token = version_;
    next->content_hash = hash_callback(std::span<const uint8_t>(canonical_));
    next->byte_size = canonical_.size();
    // Updates without a consuming draw must not retain an unlimited chain.
    // A complete copied base is equivalent to replaying all prior deltas.
    const bool complete_bytes = changed_delta.ranges.size() == 1 &&
        changed_delta.ranges[0].destination_offset == 0 &&
        changed_delta.ranges[0].payload_offset == 0 &&
        changed_delta.ranges[0].byte_count == canonical_.size() &&
        changed_delta.payload.size() == canonical_.size();
    if (complete_bytes) {
      changed_delta.complete_snapshot = true;
    } else if (current_ && current_->deferred_ancestor_count >= kMaximumDeferredAncestors) {
      if (!CaptureCompleteConstantSnapshot(canonical_, changed_delta)) {
        return {ConstantApplyStatus::kInvalidDelta, false, current_};
      }
    }
    next->delta = std::move(changed_delta);
    if (!next->delta.complete_snapshot) {
      next->parent = current_;
      next->deferred_ancestor_count = current_ ? (current_->materialized ? 1 : current_->deferred_ancestor_count + 1) : 0;
    }
    current_ = std::move(next);
    return {ConstantApplyStatus::kApplied, true, current_};
  }

  // The caller must retain the version for the whole use of this view. Draw
  // commands already do so, and need not retain/release its immutable byte
  // allocation again on every binding. Owning/asynchronous consumers continue
  // to use Materialize below.
  static const std::vector<uint8_t>* MaterializeView(
      const std::shared_ptr<const ConstantStateVersion>& version) {
    if (!version) {
      return nullptr;
    }
    if (!version->materialized) {
      Materialize(version);
    }
    return version->materialized.get();
  }

  static std::shared_ptr<const std::vector<uint8_t>> Materialize(
      const std::shared_ptr<const ConstantStateVersion>& version) {
    if (!version) {
      return {};
    }
    if (version->materialized) {
      return version->materialized;
    }

    // The common ordered-draw case has a memoized direct parent. Avoid a
    // temporary owning chain and its allocator traffic for a single delta.
    if (version->parent && version->parent->materialized &&
        version->parent->materialized->size() == version->byte_size &&
        ValidateConstantPayloadDelta(version->delta, version->byte_size)) {
      auto bytes = std::make_shared<std::vector<uint8_t>>(*version->parent->materialized);
      for (const auto& range : version->delta.ranges)
        std::memcpy(bytes->data() + range.destination_offset,
                    version->delta.payload.data() + range.payload_offset, range.byte_count);
      version->materialized = std::move(bytes);
      version->parent.reset();
      return version->materialized;
    }

    std::vector<std::shared_ptr<const ConstantStateVersion>> chain;
    std::shared_ptr<const ConstantStateVersion> cursor = version;
    while (cursor && !cursor->materialized) {
      chain.push_back(cursor);
      cursor = cursor->parent;
    }
    auto bytes = std::make_shared<std::vector<uint8_t>>(
        cursor && cursor->materialized ? *cursor->materialized
                                       : std::vector<uint8_t>(version->byte_size));
    for (auto node = chain.rbegin(); node != chain.rend(); ++node) {
      if ((*node)->byte_size != bytes->size() ||
          !ValidateConstantPayloadDelta((*node)->delta, bytes->size())) {
        return {};
      }
      for (const ConstantDeltaRange& range : (*node)->delta.ranges) {
        std::memcpy(bytes->data() + range.destination_offset,
                    (*node)->delta.payload.data() + range.payload_offset, range.byte_count);
      }
    }
    version->materialized = bytes;
    // The memoized contiguous state is now a complete immutable base. Cutting
    // its ancestry prevents a long-running device from retaining every prior
    // delta version across frames.
    version->parent.reset();
    return version->materialized;
  }

 private:
  std::vector<uint8_t> canonical_;
  StateVersionStamp version_{};
  bool initialized_ = false;
  std::shared_ptr<const ConstantStateVersion> current_;
};

template <typename T>
class AuthoritativeScalarState {
 public:
  struct ApplyResult {
    ConstantApplyStatus status = ConstantApplyStatus::kApplied;
    bool changed = false;
  };

  ApplyResult Apply(const T& value) {
    if (initialized_ && value_ == value) {
      return {};
    }
    if (!CanIncrementStateVersion(version_)) {
      return {ConstantApplyStatus::kVersionSpaceExhausted, false};
    }
    value_ = value;
    initialized_ = true;
    IncrementStateVersion(version_);
    return {ConstantApplyStatus::kApplied, true};
  }

  bool initialized() const { return initialized_; }
  const T& value() const { return value_; }
  StateVersionStamp version() const { return version_; }

 private:
  T value_{};
  StateVersionStamp version_{};
  bool initialized_ = false;
};

// This is the complete semantic input identity for the host-built shared
// constant block. It intentionally includes descriptor meaning epochs and
// page/copy ownership in addition to the indices written into the block.
// Keeping it independently testable makes omissions visible when the shared
// block gains another producer input.
template <size_t TextureStageCount>
struct SharedConstantSemanticKey {
  std::array<uint32_t, TextureStageCount> texture_descriptor_indices{};
  std::array<uint32_t, TextureStageCount> sampler_descriptor_indices{};
  // Sampler objects deliberately exclude bias. Reusing the same descriptor
  // with different guest bias must still produce a different shared block.
  std::array<uint32_t, TextureStageCount> sampler_lod_bias_bits{};
  StateVersionStamp boolean_version{};
  uint64_t image_descriptor_epoch = 0;
  uint64_t sampler_descriptor_epoch = 0;
  uint64_t cached_descriptor_epoch = 0;
  uint64_t environmental_data_hash = 0;
  uint64_t environmental_sequence = 0;
  uint32_t device = 0;
  uint32_t descriptor_copy = 0;
  uint32_t descriptor_page = std::numeric_limits<uint32_t>::max();
  uint32_t width = 0;
  uint32_t height = 0;
  uint32_t logical_width = 0;
  uint32_t logical_height = 0;
  uint32_t sample_count = 0;
  uint32_t alpha_reference_bits = 0;
  uint32_t alpha_to_mask = 0;
  std::array<uint32_t, 4> color_output_info{};
  uint32_t color_output_mask = 0;
  std::array<uint32_t, 4> clip_plane_bits{};
  uint32_t clip_plane_enable_mask = 0;
  uint32_t vertex_booleans = 0;
  uint32_t pixel_booleans = 0;
  uint8_t descriptor_backend = 0;
  uint8_t environment_present = 0;

  constexpr bool operator==(const SharedConstantSemanticKey&) const = default;
};

inline size_t FindFirstConstantMismatch(std::span<const uint8_t> left,
                                        std::span<const uint8_t> right) {
  const size_t common_size = std::min(left.size(), right.size());
  size_t offset = 0;
  while (offset < common_size && left[offset] == right[offset]) {
    ++offset;
  }
  return offset != common_size || left.size() != right.size() ? offset : kNoSemanticMismatch;
}

}  // namespace rex::graphics::gta4_native

#endif  // REX_GRAPHICS_GTA4_NATIVE_STATEFUL_CONSTANT_STATE_H_
