#ifndef GTA4_DRAW_DISTANCE_POLICY_H_
#define GTA4_DRAW_DISTANCE_POLICY_H_

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>

namespace gta4::draw_distance {

inline constexpr double kMinimumScale = 1.0;
inline constexpr double kMaximumScale = 4.0;
inline constexpr int32_t kSliderIntervals = 30;
inline constexpr uint8_t kSliderPositions = 31;
inline constexpr uint8_t kSliderDisplayType = 101;

// GTA IV's flt_82A931BC is the engine's input multiplier for the published
// world-distance scalar. Keep invalid host configuration out of guest state
// and preserve the retail multiplier as the deterministic fallback.
inline float ResolveEngineScale(double configured_scale) noexcept {
  if (!std::isfinite(configured_scale) || configured_scale < 1.0 ||
      configured_scale > static_cast<double>(std::numeric_limits<float>::max())) {
    return 1.0f;
  }
  return static_cast<float>(configured_scale);
}

// Only the local 300-unit operands in sub_821D6260 and sub_821D8CD8 use
// this limit. The scale is the input already published for the current view,
// so a live menu change cannot mix an old view threshold with a new limit.
inline float ResolveRemapLimit(double published_input_scale) noexcept {
  const float scale = ResolveEngineScale(published_input_scale);
  return 300.0f * std::min(scale, static_cast<float>(kMaximumScale));
}

inline int32_t SliderPosition(double configured_scale) noexcept {
  const double scale = std::clamp(static_cast<double>(ResolveEngineScale(configured_scale)),
                                  kMinimumScale, kMaximumScale);
  return static_cast<int32_t>(std::lround((scale - kMinimumScale) * 10.0));
}

inline double ScaleAtSliderPosition(int32_t position) noexcept {
  return kMinimumScale + static_cast<double>(std::clamp(position, 0, kSliderIntervals)) / 10.0;
}

inline double AdjustSlider(double configured_scale, int32_t delta) noexcept {
  if (delta == 0) {
    return configured_scale;
  }
  const int32_t position = SliderPosition(configured_scale);
  // The retail input can also report a non-adjustment sentinel. Its magnitude
  // is not a step count; the frontend passes an actual left/right delta here.
  return ScaleAtSliderPosition(position + (delta > 0 ? 1 : delta < 0 ? -1 : 0));
}

}  // namespace gta4::draw_distance

#endif  // GTA4_DRAW_DISTANCE_POLICY_H_
