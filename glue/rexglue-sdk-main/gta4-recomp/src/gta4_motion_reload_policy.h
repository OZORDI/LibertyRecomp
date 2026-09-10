#pragma once

#include <array>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace gta4 {

struct MotionReloadConfig {
  double up_radians = 0.0;
  double down_radians = 0.0;
  double neutral_radians = 0.0;
  std::chrono::milliseconds window{800};
  std::chrono::milliseconds cooldown{600};
  std::chrono::milliseconds completion_lifetime{250};
};

// Each completion has a monotonically increasing identity and one bounded
// delivery per consumer. Invalidating input never carries a pending gesture
// into another context; a fresh neutral sample is needed before rearming.
template <size_t ConsumerCount>
class MotionReloadGesture {
 public:
  using Clock = std::chrono::steady_clock;
  using TimePoint = Clock::time_point;

  void Reset() noexcept {
    phase_ = Phase::kNeedNeutral;
    pending_.fill(false);
    deadline_ = {};
    expires_ = {};
  }

  void Expire(TimePoint now) noexcept {
    if (now >= expires_) {
      pending_.fill(false);
    }
  }

  void Observe(double pitch, TimePoint now, const MotionReloadConfig& config) noexcept {
    Expire(now);
    if (!std::isfinite(pitch)) {
      Reset();
      return;
    }
    switch (phase_) {
      case Phase::kNeedNeutral:
        if (std::abs(pitch) <= config.neutral_radians) {
          phase_ = Phase::kIdle;
        }
        break;
      case Phase::kIdle:
        if (pitch >= config.up_radians) {
          phase_ = Phase::kAwaitingDown;
          deadline_ = now + config.window;
        }
        break;
      case Phase::kAwaitingDown:
        if (now > deadline_) {
          Reset();
        } else if (pitch <= config.down_radians) {
          ++sequence_;
          pending_.fill(true);
          expires_ = now + config.completion_lifetime;
          phase_ = Phase::kCooldown;
          deadline_ = now + config.cooldown;
        }
        break;
      case Phase::kCooldown:
        if (now >= deadline_ && std::abs(pitch) <= config.neutral_radians) {
          phase_ = Phase::kIdle;
        }
        break;
    }
  }

  bool Consume(size_t consumer, TimePoint now) noexcept {
    Expire(now);
    if (consumer >= pending_.size() || !pending_[consumer]) {
      return false;
    }
    pending_[consumer] = false;
    return true;
  }

  uint64_t sequence() const noexcept { return sequence_; }

 private:
  enum class Phase { kNeedNeutral, kIdle, kAwaitingDown, kCooldown };
  Phase phase_ = Phase::kNeedNeutral;
  std::array<bool, ConsumerCount> pending_{};
  TimePoint deadline_{};
  TimePoint expires_{};
  uint64_t sequence_ = 0;
};

}  // namespace gta4
