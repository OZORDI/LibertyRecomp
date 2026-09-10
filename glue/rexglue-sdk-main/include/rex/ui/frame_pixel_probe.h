#pragma once
// Optional per-image observation metadata. This never selects rendering state.
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <span>

namespace rex::ui {
struct FramePixelProbe {
  uint64_t run = 0;
  uint64_t source_sequence = 0;
  uint64_t native_submission = 0;
  uint64_t guest_image = 0;
  uint64_t guest_version = 0;
  uint32_t frame = 0;
  uint32_t fixture = 0;
  uint32_t width = 0;
  uint32_t height = 0;
  std::array<float, 4> normalized_region{}; // left, top, right, bottom

  bool valid() const {
    const auto& r = normalized_region;
    return run && source_sequence && native_submission && guest_image && frame &&
        width && height && width <= 65536 && height <= 65536 &&
        std::all_of(r.begin(), r.end(), [](float v) { return std::isfinite(v); }) &&
        r[0] >= 0 && r[1] >= 0 && r[2] <= 1 && r[3] <= 1 && r[0] < r[2] && r[1] < r[3];
  }
  bool matches(uint32_t submitted_frame, uint64_t actual_image,
               uint32_t actual_width, uint32_t actual_height, uint64_t actual_version) const {
    return valid() && frame == submitted_frame && guest_image == actual_image &&
           width == actual_width && height == actual_height && guest_version == actual_version;
  }
};

// Used by the real mailbox publisher: failure, inactivity, and a different
// source frame discard the observation instead of carrying it to another image.
inline FramePixelProbe PublishFramePixelProbe(const FramePixelProbe& requested,
    bool refreshed, uint32_t frame, uint32_t width, uint32_t height) {
  return refreshed && requested.matches(frame, requested.guest_image, width, height, requested.guest_version)
             ? requested : FramePixelProbe{};
}

struct FramePixelProbeRegion {
  std::array<uint32_t, 4> rectangle{}; // clipped swapchain x, y, width, height
  std::array<int64_t, 4> output_rectangle{}; // uncropped final guest-output rectangle
  bool valid() const { return rectangle[2] && rectangle[3]; }
  std::array<uint32_t, 2> sample(uint32_t x, uint32_t y, uint32_t axis = 16) const {
    if (!valid() || !axis || x >= axis || y >= axis) return {};
    return {rectangle[0] + uint32_t(uint64_t(rectangle[2]) * (uint64_t(x) * 2 + 1) / (uint64_t(axis) * 2)),
            rectangle[1] + uint32_t(uint64_t(rectangle[3]) * (uint64_t(y) * 2 + 1) / (uint64_t(axis) * 2))};
  }
};
inline FramePixelProbeRegion MapFramePixelProbe(const FramePixelProbe& probe,
    int32_t output_x, int32_t output_y, uint32_t output_width, uint32_t output_height,
    uint32_t swap_width, uint32_t swap_height) {
  FramePixelProbeRegion result;
  if (!probe.valid() || !output_width || !output_height || !swap_width || !swap_height ||
      output_width > 65536 || output_height > 65536 || swap_width > 65536 || swap_height > 65536) return result;
  const auto& r = probe.normalized_region;
  const double left = std::clamp(double(output_x) + double(r[0]) * output_width, 0.0, double(swap_width));
  const double top = std::clamp(double(output_y) + double(r[1]) * output_height, 0.0, double(swap_height));
  const double right = std::clamp(double(output_x) + double(r[2]) * output_width, 0.0, double(swap_width));
  const double bottom = std::clamp(double(output_y) + double(r[3]) * output_height, 0.0, double(swap_height));
  if (right <= left || bottom <= top) return result;
  const uint32_t x = uint32_t(std::floor(left)), y = uint32_t(std::floor(top));
  result.rectangle = {x, y, uint32_t(std::ceil(right)) - x, uint32_t(std::ceil(bottom)) - y};
  result.output_rectangle = {output_x, output_y, output_width, output_height};
  return result;
}

struct FramePixelProbeKey {
  uint64_t run = 0, sequence = 0;
  uint32_t frame = 0;
  uint64_t epoch = 0;
  bool operator==(const FramePixelProbeKey&) const = default;
};
class FramePixelProbeBudget {
 public:
  static constexpr size_t kLimit = 32;
  bool can_record(FramePixelProbeKey key) const {
    if (!key.run || !key.sequence || !key.frame || count_ == seen_.size()) return false;
    return std::find(seen_.begin(), seen_.begin() + count_, key) == seen_.begin() + count_;
  }
  bool commit(FramePixelProbeKey key) {
    if (!can_record(key)) return false;
    seen_[count_++] = key;
    return true;
  }
 private:
  std::array<FramePixelProbeKey, kLimit> seen_{};
  size_t count_ = 0;
};

inline uint64_t FramePixelProbeChecksum(std::span<const uint8_t> bytes) {
  uint64_t value = 14695981039346656037ull;
  for (uint8_t byte : bytes) { value ^= byte; value *= 1099511628211ull; }
  return value;
}
inline bool FramePixelProbeReadable(bool pending, uint64_t submitted, uint64_t completed,
                                     bool mapped, bool host_visible) {
  return pending && submitted && submitted <= completed && mapped && host_visible;
}
} // namespace rex::ui
