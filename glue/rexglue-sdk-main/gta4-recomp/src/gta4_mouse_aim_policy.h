#pragma once

#include <array>
#include <cmath>
#include <cstdint>

namespace gta4::input {

// The retail fire-type name table at 0x82A99508 starts with MELEE (0),
// INSTANT_HIT (1), DELAYED_HIT (2), PROJECTILE (3), AREA_EFFECT (4).
constexpr bool MouseFreeAimWeapon(uint32_t fire_type) {
  return fire_type >= 1 && fire_type <= 4;
}

// Retail normal aim uses pressures above 10 but below byte_82AA1B0F;
// alternate aim reverses the lock/free-aim ranges. Stay outside lock-on.
constexpr uint8_t MouseAimPressure(bool free_aim, bool alternate, uint8_t threshold) {
  if (!free_aim) return 255;
  if (alternate) return threshold < 255 ? 255 : 0;
  return threshold > 11 ? static_cast<uint8_t>(threshold - 1) : 0;
}

class MouseAimLatch {
 public:
  bool Update(bool allowed, bool toggle, bool held, bool pressed,
              uint64_t reset_generation, uint32_t context) {
    const bool changed = initialized_ &&
        (reset_generation != generation_ || context != context_ || toggle != toggle_);
    if (!allowed || changed) latched_ = false;
    if (allowed && toggle && pressed && !changed) latched_ = !latched_;
    initialized_ = true;
    generation_ = reset_generation;
    context_ = context;
    toggle_ = toggle;
    return allowed && (toggle ? latched_ : held);
  }
 private:
  bool initialized_ = false;
  bool latched_ = false;
  bool toggle_ = false;
  uint64_t generation_ = 0;
  uint32_t context_ = 0;
};

struct MouseRotation {
  double yaw = 0;
  double pitch = 0;
  bool moving() const { return yaw != 0 || pitch != 0; }
};

inline MouseRotation MouseDisplacement(double dx, double dy, double sensitivity,
                                       bool invert_y, double fov) {
  if (!std::isfinite(dx) || !std::isfinite(dy) ||
      !std::isfinite(sensitivity) || sensitivity <= 0 ||
      !std::isfinite(fov) || fov <= 0 || fov >= 180) return {};
  // Tunable host sensitivity, with the retail weapon camera's linear FOV
  // scaling. This is a displacement, independent of the frame timestep.
  const double gain = 0.002 * sensitivity * (fov / 45.0);
  const double yaw = -dx * gain;
  const double pitch = (invert_y ? dy : -dy) * gain;
  if (!std::isfinite(yaw) || !std::isfinite(pitch)) return {};
  return {yaw, pitch};
}

inline double MouseRotationRate(double displacement, double seconds) {
  return std::isfinite(seconds) && seconds > 0
      ? displacement / (seconds * 30.0) : 0;
}

inline double ConstrainMousePitch(double angle, double displacement,
                                  double lower, double upper) {
  if (displacement == 0 || !std::isfinite(displacement) ||
      !std::isfinite(angle) || !std::isfinite(lower) ||
      !std::isfinite(upper) || lower > upper) return 0;
  return std::fmax(lower, std::fmin(upper, angle + displacement)) - angle;
}

inline double WrapMouseYaw(double displacement) {
  // sub_82369828 wraps only one revolution. Keep a large mouse flick within
  // that contract without discarding its final orientation.
  return std::isfinite(displacement)
      ? std::remainder(displacement, 6.283185307179586) : 0;
}

// Camera blending can update multiple cameras, or revisit one camera, in the
// same poll. Each camera gets the snapshot once. Fixed storage owns no heap.
class MouseCameraEpochs {
 public:
  bool Claim(uint64_t sequence, uint32_t camera) {
    if (sequence != sequence_) { cameras_ = {}; sequence_ = sequence; }
    if (!camera) return false;
    for (auto& entry : cameras_) {
      if (entry == camera) return false;
      if (!entry) { entry = camera; return true; }
    }
    return false;
  }
 private:
  uint64_t sequence_ = 0;
  std::array<uint32_t, 32> cameras_{};
};

}  // namespace gta4::input
