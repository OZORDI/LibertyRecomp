#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdint>

namespace rex::graphics::gta4_native {

// Appended to NativeSharedConstants, leaving every existing shader ABI offset intact.
inline constexpr uint32_t kNativeColorOutputOffset = 0x360;
inline constexpr uint32_t kNativeColorOutputTargetCount = 4;

struct NativeColorOutputParameters {
  std::array<float, 4> scale{1, 1, 1, 1};
  // An inverted interval disables input clamping for a floating-point component.
  std::array<float, 4> minimum{1, 1, 1, 1};
  std::array<float, 4> maximum{0, 0, 0, 0};
  constexpr bool operator==(const NativeColorOutputParameters&) const = default;
};
static_assert(sizeof(NativeColorOutputParameters) == 48);

constexpr int32_t NativeSignedSixBit(uint32_t value) {
  return int32_t(value & 31u) - int32_t(value & 32u);
}
constexpr int32_t NativeColorExponent(uint32_t color_info) {
  return NativeSignedSixBit(color_info >> 20);
}
constexpr int32_t NativeResolveExponent(uint32_t flags) {
  // generated .80 sub_82A3CC68: rotate flags by 6, then retain the low 6 bits.
  return NativeSignedSixBit(flags >> 26);
}
constexpr float NativePowerOfTwo(int32_t exponent) {
  // Callers supply the signed six-bit guest exponent, not an arbitrary shift.
  return std::bit_cast<float>(uint32_t((127 + exponent) * (1 << 23)));
}

constexpr NativeColorOutputParameters NativeColorOutput(uint32_t color_info,
                                                        bool active = true) {
  NativeColorOutputParameters output;
  if (!active) return output;
  output.scale.fill(NativePowerOfTwo(NativeColorExponent(color_info)));
  const uint32_t format = (color_info >> 16) & 15u;
  // registers.h RB_COLOR_INFO and RenderTargetCache::AddPSIColorFormatFlags.
  // Input RGB remains HDR for 7e3/FP16/FP32 formats. Packed floating RGB has
  // normalized alpha, even when the native backing image is RGBA16_SFLOAT.
  if (format == 0 || format == 1 || format == 2 || format == 10) {
    output.minimum.fill(0);
    output.maximum.fill(1);
  } else if (format == 4 || format == 5) {
    output.minimum.fill(-32);
    output.maximum.fill(32);
  } else if (format == 3 || format == 12) {
    output.minimum[3] = 0;
    output.maximum[3] = 1;
  }
  return output;
}

// CPU oracle for the shader epilogue. Alpha test and sample-mask generation must
// have consumed the unmodified output before this operation executes.
inline std::array<float, 4> ApplyNativeColorOutput(
    std::array<float, 4> color, const NativeColorOutputParameters& parameters) {
  for (uint32_t i = 0; i < color.size(); ++i) {
    color[i] *= parameters.scale[i];
    if (parameters.minimum[i] <= parameters.maximum[i]) {
      color[i] = std::isnan(color[i]) ? 0.0f : color[i];
      color[i] = std::clamp(color[i], parameters.minimum[i], parameters.maximum[i]);
    }
  }
  return color;
}

}  // namespace rex::graphics::gta4_native
