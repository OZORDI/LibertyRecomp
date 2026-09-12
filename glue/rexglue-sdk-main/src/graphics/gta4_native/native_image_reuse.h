#pragma once
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace rex::graphics::gta4_native {
struct NativeImageAllocationKey {
  // flags, type, format, width/height/depth, mip levels, layers, samples,
  // tiling and usage. Only exclusive, unextended creation infos are pooled.
  std::array<uint32_t, 11> fields{};
  bool operator==(const NativeImageAllocationKey&) const = default;
};
template <typename Allocation>
class NativeImageReusePool {
 public:
  struct Entry {
    NativeImageAllocationKey key{};
    Allocation allocation{};
    uint64_t bytes = 0;
    uint32_t frame = 0;
  };
  NativeImageReusePool(uint64_t bytes, size_t entries)
      : byte_limit_(bytes), entry_limit_(entries) {}
  bool Retain(const NativeImageAllocationKey& key, Allocation allocation, uint64_t bytes,
              uint64_t last_submission, uint64_t completed_submission, uint32_t frame) {
    if (!bytes || last_submission > completed_submission || entries_.size() >= entry_limit_ ||
        bytes > byte_limit_ || bytes_ > byte_limit_ - bytes)
      return false;
    entries_.push_back({key, std::move(allocation), bytes, frame});
    bytes_ += bytes;
    return true;
  }
  std::optional<Entry> Take(const NativeImageAllocationKey& key) {
    auto it =
        std::find_if(entries_.begin(), entries_.end(), [&](const auto& e) { return e.key == key; });
    if (it == entries_.end())
      return std::nullopt;
    Entry entry = std::move(*it);
    bytes_ -= entry.bytes;
    entries_.erase(it);
    return entry;
  }
  template <typename Release>
  void Trim(uint32_t frame, uint32_t age, size_t maximum, Release release) {
    for (auto it = entries_.begin(); it != entries_.end() && maximum;) {
      if (age && (frame < it->frame || frame - it->frame < age)) {
        ++it;
        continue;
      }
      release(it->allocation);
      bytes_ -= it->bytes;
      it = entries_.erase(it);
      --maximum;
    }
  }
  uint64_t bytes() const { return bytes_; }
  size_t size() const { return entries_.size(); }
  const auto& entries() const { return entries_; }

 private:
  uint64_t byte_limit_ = 0, bytes_ = 0;
  size_t entry_limit_ = 0;
  std::vector<Entry> entries_;
};
}  // namespace rex::graphics::gta4_native
