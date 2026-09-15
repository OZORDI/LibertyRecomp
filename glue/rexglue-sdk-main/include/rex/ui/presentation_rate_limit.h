#pragma once

#include <algorithm>
#include <cstdint>
#include <limits>

namespace rex::ui {

// Nonblocking host-presentation admission. The title keeps its existing producer
// limiter; this also limits UI-only redraws of the same title image. All accesses
// belong to the presenter's existing exclusive paint owner. No driver clock or
// display timestamp is involved, and failed paint attempts spend no frame slot.
class PresentationRateLimit {
 public:
  uint64_t Delay(uint32_t fps, uint64_t now_ns) noexcept {
    if (fps > 1000) fps = 0;
    if (fps != fps_ || now_ns < last_observed_ns_) {
      fps_ = fps;
      deadline_ns_ = 0;
      fraction_ = 0;
    }
    last_observed_ns_ = now_ns;
    return fps_ && deadline_ns_ > now_ns ? deadline_ns_ - now_ns : 0;
  }

  void Presented(uint64_t frame_begin_ns) noexcept {
    if (!fps_) return;
    const uint64_t whole = kSecond / fps_;
    fraction_ += uint32_t(kSecond % fps_);
    const uint64_t period = whole + (fraction_ >= fps_ ? 1 : 0);
    fraction_ %= fps_;
    // Small scheduling delays keep the existing phase. Large stalls rebase
    // rather than releasing a burst of overdue presentations.
    const bool keep_phase = deadline_ns_ && frame_begin_ns >= deadline_ns_ &&
                            frame_begin_ns - deadline_ns_ < whole;
    const uint64_t origin = keep_phase ? deadline_ns_ : frame_begin_ns;
    deadline_ns_ = origin > std::numeric_limits<uint64_t>::max() - period
                       ? std::numeric_limits<uint64_t>::max() : origin + period;
  }

  static uint32_t DelayMilliseconds(uint64_t delay_ns) noexcept {
    const uint64_t milliseconds = delay_ns / 1'000'000 + (delay_ns % 1'000'000 != 0);
    return uint32_t(std::clamp<uint64_t>(milliseconds, 1, 1000));
  }

 private:
  static constexpr uint64_t kSecond = 1'000'000'000;
  uint32_t fps_ = 0;
  uint32_t fraction_ = 0;
  uint64_t deadline_ns_ = 0;
  uint64_t last_observed_ns_ = 0;
};

}  // namespace rex::ui
