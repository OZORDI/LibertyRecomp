#pragma once

#include <bit>
#include <cmath>
#include <cstdint>

namespace gta4::loading_parallax {

constexpr double kLogicalStepSeconds = 0.03333333333333333;
constexpr double kClockToleranceSeconds = 0.00000005960464477539063;
constexpr uint32_t kLogicalStepFloatBits = 0x3D088889;

class LogicalAnimationClock {
 public:
  bool Observe(float cumulative_elapsed, float frame_delta) noexcept {
    if (!std::isfinite(cumulative_elapsed) || !std::isfinite(frame_delta) ||
        cumulative_elapsed < 0.0f || frame_delta <= 0.0f) {
      Reset();
      return false;
    }

    const uint32_t cumulative_bits = std::bit_cast<uint32_t>(cumulative_elapsed);
    if (initialized_ && cumulative_bits == last_cumulative_bits_) {
      return advance_this_frame_;
    }

    if (!initialized_ || cumulative_elapsed < last_cumulative_elapsed_) {
      pending_seconds_ = 0.0;
    }

    initialized_ = true;
    last_cumulative_bits_ = cumulative_bits;
    last_cumulative_elapsed_ = cumulative_elapsed;
    pending_seconds_ += static_cast<double>(frame_delta);
    advance_this_frame_ = pending_seconds_ + kClockToleranceSeconds >= kLogicalStepSeconds;
    if (!advance_this_frame_) {
      return false;
    }

    // Emit at most one logical update per rendered frame. Dropping excess
    // whole steps prevents a long render stall from causing an animation
    // catch-up burst on subsequent frames.
    if (pending_seconds_ < kLogicalStepSeconds) {
      pending_seconds_ = 0.0;
    } else {
      pending_seconds_ = std::fmod(pending_seconds_, kLogicalStepSeconds);
      if (pending_seconds_ + kClockToleranceSeconds >= kLogicalStepSeconds) {
        pending_seconds_ = 0.0;
      }
    }
    return true;
  }

  void Reset() noexcept {
    initialized_ = false;
    advance_this_frame_ = false;
    last_cumulative_bits_ = 0;
    last_cumulative_elapsed_ = 0.0f;
    pending_seconds_ = 0.0;
  }

 private:
  bool initialized_ = false;
  bool advance_this_frame_ = false;
  uint32_t last_cumulative_bits_ = 0;
  float last_cumulative_elapsed_ = 0.0f;
  double pending_seconds_ = 0.0;
};

inline float CorrectAccumulator(float before, float after, float original_increment) noexcept {
  if (!std::isfinite(before) || !std::isfinite(after) || !std::isfinite(original_increment) ||
      before < 0.0f || original_increment < 0.0f) {
    return after;
  }

  const float original_result = before + original_increment;
  if (std::bit_cast<uint32_t>(after) != std::bit_cast<uint32_t>(original_result) ||
      std::bit_cast<uint32_t>(after) == std::bit_cast<uint32_t>(before)) {
    return after;
  }

  return before + std::bit_cast<float>(kLogicalStepFloatBits);
}

}  // namespace gta4::loading_parallax
