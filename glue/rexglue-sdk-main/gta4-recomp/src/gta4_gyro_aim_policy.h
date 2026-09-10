#pragma once
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
namespace gta4::input {
struct GyroAimActions { int32_t horizontal = 0; int32_t vertical = 0; };
// A controller-only setup still needs the native camera-action merge when
// optional gyro aiming is enabled; no physical keyboard is required.
constexpr bool NeedsNativeActionReplay(bool native, bool touch, bool gyro_enabled, bool controller) {
  return native || touch || (gyro_enabled && controller);
}
// Angular velocity is an input rate, not an accumulated angle. GTA applies its
// own frame timestep when consuming these camera actions, so do not integrate
// the same sensor reading again for each action consumer.
inline GyroAimActions BuildGyroAimActions(bool enabled, const std::array<float, 3>& rate,
                                         float full_scale, bool invert_x, bool invert_y) {
  if (!enabled || !std::isfinite(full_scale) || full_scale <= 0) return {};
  for (float value : rate) if (!std::isfinite(value)) return {};
  const auto axis = [full_scale](float value) -> int32_t {
    if (std::abs(value) <= 0.015f) return 0;
    return static_cast<int32_t>(std::lround(std::clamp(value / full_scale, -1.0f, 1.0f) * 255.0f));
  };
  return {axis(invert_x ? -rate[1] : rate[1]), axis(invert_y ? rate[0] : -rate[0])};
}
}  // namespace gta4::input
