#pragma once

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <limits>
#include <mutex>

namespace rex::ui {

inline uint64_t FramePacerNowNs() noexcept {
  return uint64_t(std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::steady_clock::now().time_since_epoch()).count());
}

// One timeline, owned by the existing exclusive presenter paint owner. A slot
// is committed only by a successful host present, never by a retry or a game
// command being enqueued. No GPU lifetime or display-clock assumption lives here.
class FramePacer {
 public:
  static constexpr uint64_t kSecond = 1'000'000'000;
  struct Attempt {
    uint64_t slot_ns = 0;
    uint64_t delay_ns = 0;
    uint64_t display_target_ns = 0;  // Host clock; translate only after calibration.
    uint32_t fps = 0;
    uint64_t generation = 0;
  };

  static constexpr uint64_t Add(uint64_t a, uint64_t b) noexcept {
    return a > UINT64_MAX - b ? UINT64_MAX : a + b;
  }
  void Configure(uint32_t fps, uint64_t now_ns) noexcept {
    if (fps > 1000) fps = 0;
    if (fps != fps_ || now_ns < last_observed_ns_) {
      Reset();
      fps_ = fps;
    }
    last_observed_ns_ = now_ns;
  }
  void Reset() noexcept {
    next_ns_ = last_observed_ns_ = 0;
    fraction_ = 0;
    phase_observed_ = false;
    attempt_ = {};
    ++generation_;
  }
  Attempt Plan(uint64_t now_ns) noexcept {
    Configure(fps_, now_ns);
    if (!fps_) return attempt_ = {0, 0, 0, 0, generation_};
    const uint64_t period = kSecond / fps_;
    if (!next_ns_ || (now_ns >= next_ns_ && now_ns - next_ns_ >= period)) {
      if (next_ns_) ++missed_slots_;
      next_ns_ = now_ns;
      fraction_ = 0;
    }
    // At most one interval of display lead. CPU admission and timed display
    // use this SAME slot, not two independently advancing clocks.
    attempt_ = {next_ns_, next_ns_ > now_ns ? next_ns_ - now_ns : 0,
                Add(next_ns_, period + (fraction_ + kSecond % fps_ >= fps_ ? 1 : 0)),
                fps_, generation_};
    return attempt_;
  }
  void Queued(uint64_t queue_end_ns) noexcept {
    if (!fps_ || attempt_.generation != generation_) return;
    const uint64_t whole = kSecond / fps_;
    fraction_ += uint32_t(kSecond % fps_);
    const uint64_t period = whole + (fraction_ >= fps_ ? 1 : 0);
    fraction_ %= fps_;
    next_ns_ = Add(attempt_.slot_ns, period);
    // Long stalls do not grant a burst of overdue frame slots.
    if (queue_end_ns >= next_ns_ && queue_end_ns - next_ns_ >= whole) {
      next_ns_ = Add(queue_end_ns, whole);
      fraction_ = 0;
      ++missed_slots_;
    }
  }
  // Called only with a validated, matched driver observation translated to the
  // host clock. Use it once per rate/surface epoch to establish the display phase.
  bool ObservePhase(uint64_t actual_host_ns, uint64_t now_ns) noexcept {
    if (!fps_ || phase_observed_ || !actual_host_ns || actual_host_ns > now_ns ||
        now_ns - actual_host_ns > 2 * kSecond) return false;
    const uint64_t floor = std::max(next_ns_, now_ns);
    const uint64_t elapsed = floor - actual_host_ns;
    if (elapsed > 3 * kSecond) return false;
    const uint64_t periods = (elapsed / kSecond) * fps_ +
                            ((elapsed % kSecond) * fps_) / kSecond + 1;
    const uint64_t remainder = periods * (kSecond % fps_);
    next_ns_ = Add(actual_host_ns, periods * (kSecond / fps_) + remainder / fps_);
    fraction_ = uint32_t(remainder % fps_);
    phase_observed_ = true;
    return true;
  }
  uint32_t fps() const noexcept { return fps_; }
  uint64_t generation() const noexcept { return generation_; }
  uint64_t missed_slots() const noexcept { return missed_slots_; }
  bool phase_observed() const noexcept { return phase_observed_; }

 private:
  uint32_t fps_ = 0;
  uint32_t fraction_ = 0;
  uint64_t next_ns_ = 0;
  uint64_t last_observed_ns_ = 0;
  uint64_t generation_ = 1;
  uint64_t missed_slots_ = 0;
  bool phase_observed_ = false;
  Attempt attempt_{};
};

// Back-pressure, NOT another timer/cadence. Only the native render worker opts
// into waiting. It has already submitted its GPU work and holds no presenter,
// queue, resource or UI mutex. The UI only acknowledges; it never waits here.
class FramePublicationGate {
 public:
  uint64_t Publish(uint32_t fps) {
    std::lock_guard lock(mutex_);
    limited_ = fps != 0;
    const auto result = ++published_;
    condition_.notify_all();
    return result;
  }
  void Accept(uint64_t serial) {
    std::lock_guard lock(mutex_);
    if (serial <= published_) accepted_ = std::max(accepted_, serial);
    condition_.notify_all();
  }
  bool Wait(uint64_t serial) {
    std::unique_lock lock(mutex_);
    const uint64_t epoch = epoch_;
    // A minimized/occluded window or missing callback must not hang the title.
    // This is a watchdog, never a frame-rate target.
    return condition_.wait_for(lock, std::chrono::milliseconds(250), [&] {
      return stopped_ || !available_ || !limited_ || epoch_ != epoch || accepted_ >= serial;
    });
  }
  void SetAvailable(bool value) {
    std::lock_guard lock(mutex_);
    available_ = value;
    if (!value) ++epoch_;
    condition_.notify_all();
  }
  void Stop() {
    std::lock_guard lock(mutex_);
    stopped_ = true;
    condition_.notify_all();
  }
 private:
  std::mutex mutex_;
  std::condition_variable condition_;
  uint64_t published_ = 0, accepted_ = 0, epoch_ = 0;
  bool limited_ = false, available_ = false, stopped_ = false;
};

}  // namespace rex::ui
