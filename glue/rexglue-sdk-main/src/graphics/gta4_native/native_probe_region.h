#pragma once
#include <algorithm>
#include <cstdint>
#include <optional>

namespace rex::graphics::gta4_native {
// Observation-only readback rectangle. A requested size of zero retains the
// complete dimension. An out-of-range origin is rejected, never wrapped.
struct NativeProbeRegion {
  uint32_t x = 0, y = 0, width = 0, height = 0;
  constexpr bool operator==(const NativeProbeRegion&) const = default;
};
constexpr std::optional<NativeProbeRegion> MakeNativeProbeRegion(
    uint32_t image_width, uint32_t image_height, uint32_t x = 0,
    uint32_t y = 0, uint32_t width = 0, uint32_t height = 0) {
  if (!image_width || !image_height || x >= image_width || y >= image_height)
    return std::nullopt;
  const auto available_width = image_width - x;
  const auto available_height = image_height - y;
  return NativeProbeRegion{x, y,
      width ? std::min(width, available_width) : available_width,
      height ? std::min(height, available_height) : available_height};
}
}  // namespace rex::graphics::gta4_native
