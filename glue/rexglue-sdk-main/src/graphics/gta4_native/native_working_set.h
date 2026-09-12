#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <span>
#include <vector>

#include "frame_constant_arena.h"

namespace rex::graphics::gta4_native {

// Deterministic work bounds. Admit one oversized allocation so cleanup always
// progresses, then stop; emergency pressure recovery uses an unlimited budget.
class NativeWorkBudget {
 public:
  NativeWorkBudget(size_t items, uint64_t bytes) : item_limit_(items), byte_limit_(bytes) {}
  bool Consume(uint64_t bytes) {
    if (items_ >= item_limit_ ||
        (items_ && (bytes_ >= byte_limit_ || bytes > byte_limit_ - bytes_)))
      return false;
    ++items_;
    bytes_ = bytes > UINT64_MAX - bytes_ ? UINT64_MAX : bytes_ + bytes;
    return true;
  }
  size_t items() const { return items_; }
  uint64_t bytes() const { return bytes_; }

 private:
  size_t item_limit_, items_ = 0;
  uint64_t byte_limit_, bytes_ = 0;
};

// One planner belongs to one fence-protected frame slot. Identity zero is the
// dimension-correct null descriptor. Pages cover this frame's working set, not
// all image generations ever streamed through the process. Shader indices stay
// unchanged: each draw binds a page and indexes that page's five existing sets.
template <size_t Stages>
class NativeDescriptorWorkingSet {
 public:
  struct Page {
    FrameGenerationMap<uint64_t, uint32_t> image_lookup, sampler_lookup;
    std::vector<uint64_t> images, samplers;
    bool Reset() {
      if (!image_lookup.ResetGeneration() || !sampler_lookup.ResetGeneration())
        return false;
      images.clear();
      samplers.clear();
      images.push_back(0);
      samplers.push_back(0);
      return bool(image_lookup.Insert(0, 0)) && bool(sampler_lookup.Insert(0, 0));
    }
  };
  struct Assignment {
    uint32_t page = 0;
    std::array<uint32_t, Stages> images{}, samplers{};
  };
  bool Begin(uint32_t image_capacity, uint32_t sampler_capacity, uint32_t maximum_pages,
             uint64_t completed_submission = UINT64_MAX) {
    if (image_capacity < 2 || sampler_capacity < 2 || !maximum_pages || epoch_ == UINT64_MAX ||
        in_flight_submission_ > completed_submission)
      return false;
    image_capacity_ = image_capacity;
    sampler_capacity_ = sampler_capacity;
    maximum_pages_ = maximum_pages;
    active_pages_ = 0;
    ++epoch_;
    in_flight_submission_ = 0;
    return true;
  }
  std::optional<Assignment> Assign(const std::array<uint64_t, Stages>& images,
                                   const std::array<uint64_t, Stages>& samplers) {
    if (!maximum_pages_)
      return std::nullopt;
    if (!active_pages_ && !StartPage())
      return std::nullopt;
    auto fits = [&](const Page& page) {
      return page.images.size() + Missing(page.image_lookup, images) <= image_capacity_ &&
             page.samplers.size() + Missing(page.sampler_lookup, samplers) <= sampler_capacity_;
    };
    if (!fits(pages_[active_pages_ - 1])) {
      if (!StartPage() || !fits(pages_[active_pages_ - 1]))
        return std::nullopt;
    }
    Assignment out;
    out.page = uint32_t(active_pages_ - 1);
    Page& page = pages_[out.page];
    for (size_t i = 0; i < Stages; ++i) {
      out.images[i] = Insert(page.image_lookup, page.images, images[i]);
      out.samplers[i] = Insert(page.sampler_lookup, page.samplers, samplers[i]);
      if (out.images[i] == UINT32_MAX || out.samplers[i] == UINT32_MAX)
        return std::nullopt;
    }
    return out;
  }
  bool MarkSubmitted(uint64_t submission) {
    if (!epoch_ || !submission || in_flight_submission_ || submission <= last_submission_)
      return false;
    in_flight_submission_ = last_submission_ = submission;
    return true;
  }
  size_t page_count() const { return active_pages_; }
  const Page& page(size_t index) const { return pages_.at(index); }
  uint64_t epoch() const { return epoch_; }

 private:
  static size_t Missing(const FrameGenerationMap<uint64_t, uint32_t>& lookup,
                        const std::array<uint64_t, Stages>& keys) {
    size_t count = 0;
    for (size_t i = 0; i < Stages; ++i)
      if (!lookup.Find(keys[i]) &&
          std::find(keys.begin(), keys.begin() + i, keys[i]) == keys.begin() + i)
        ++count;
    return count;
  }
  static uint32_t Insert(FrameGenerationMap<uint64_t, uint32_t>& lookup,
                         std::vector<uint64_t>& values, uint64_t key) {
    if (const auto* existing = lookup.Find(key))
      return *existing;
    if (values.size() >= UINT32_MAX)
      return UINT32_MAX;
    const auto index = uint32_t(values.size());
    if (!lookup.Insert(key, index))
      return UINT32_MAX;
    values.push_back(key);
    return index;
  }
  bool StartPage() {
    if (active_pages_ >= maximum_pages_)
      return false;
    if (active_pages_ == pages_.size())
      pages_.emplace_back();
    if (!pages_[active_pages_].Reset())
      return false;
    ++active_pages_;
    return true;
  }
  std::vector<Page> pages_;
  size_t active_pages_ = 0;
  uint32_t image_capacity_ = 0, sampler_capacity_ = 0, maximum_pages_ = 0;
  uint64_t epoch_ = 0, in_flight_submission_ = 0, last_submission_ = 0;
};

template <typename Surface>
constexpr bool NativeSurfaceStateEqual(const Surface& a, const Surface& b) {
  return a.handle == b.handle && a.flags == b.flags && a.base == b.base && a.address == b.address &&
         a.packed_dimensions == b.packed_dimensions && a.format == b.format && a.width == b.width &&
         a.height == b.height && a.sample_type == b.sample_type;
}

// Equality is verified after a hash hit. Hash collisions must never change
// shader constants. Reservations refer to immutable bytes owned by this frame.
struct NativeConstantContentKey {
  uint64_t hash = 0;
  uint32_t kind = 0, byte_size = 0;
  bool operator==(const NativeConstantContentKey&) const = default;
};
struct NativeConstantContentHash {
  size_t operator()(const NativeConstantContentKey& key) const {
    return size_t(key.hash ^ (uint64_t(key.kind) << 56) ^ key.byte_size);
  }
};

}  // namespace rex::graphics::gta4_native
