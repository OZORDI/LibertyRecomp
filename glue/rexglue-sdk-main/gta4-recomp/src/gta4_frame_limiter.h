#pragma once

#include <cstdint>
#include <limits>

namespace gta4::frame_limiter {

inline constexpr int64_t kNanosecondsPerSecond = 1'000'000'000;

constexpr bool IsSupportedLimit(uint32_t frames_per_second) noexcept {
  return frames_per_second == 0 || frames_per_second == 30 || frames_per_second == 40 ||
         frames_per_second == 60 ||
         frames_per_second == 120;
}

struct State {
  uint32_t frames_per_second = 0;
  int64_t next_deadline_ns = 0;
  uint32_t fractional_numerator = 0;
};

struct Decision {
  State next_state{};
  int64_t wait_until_ns = 0;
  bool mode_changed = false;
  bool late_reset = false;

  constexpr bool should_wait(int64_t now_ns) const noexcept {
    return wait_until_ns > now_ns;
  }
};

constexpr void AdvanceDeadline(State& state, int64_t origin_ns) noexcept {
  if (!IsSupportedLimit(state.frames_per_second) || state.frames_per_second == 0) {
    state = {};
    return;
  }
  const int64_t whole_nanoseconds = kNanosecondsPerSecond / state.frames_per_second;
  const uint32_t fractional_numerator =
      static_cast<uint32_t>(kNanosecondsPerSecond % state.frames_per_second);
  state.next_deadline_ns = origin_ns;
  const uint64_t accumulated_fraction =
      static_cast<uint64_t>(state.fractional_numerator) + fractional_numerator;
  state.fractional_numerator =
      static_cast<uint32_t>(accumulated_fraction % state.frames_per_second);
  if (state.next_deadline_ns >
      std::numeric_limits<int64_t>::max() - whole_nanoseconds) {
    state.next_deadline_ns = std::numeric_limits<int64_t>::max();
    state.fractional_numerator = 0;
    return;
  }
  state.next_deadline_ns += whole_nanoseconds;
  if (accumulated_fraction >= state.frames_per_second) {
    if (state.next_deadline_ns != std::numeric_limits<int64_t>::max()) {
      ++state.next_deadline_ns;
    }
  }
}

constexpr Decision Plan(State previous, uint32_t frames_per_second, int64_t now_ns) noexcept {
  Decision decision{};
  if (!IsSupportedLimit(frames_per_second)) {
    frames_per_second = 0;
  }
  decision.mode_changed = previous.frames_per_second != frames_per_second;
  if (frames_per_second == 0) {
    return decision;
  }

  State next = previous;
  if (decision.mode_changed || next.next_deadline_ns <= 0) {
    next = {.frames_per_second = frames_per_second};
    AdvanceDeadline(next, now_ns);
    decision.next_state = next;
    return decision;
  }

  next.frames_per_second = frames_per_second;
  if (now_ns < next.next_deadline_ns) {
    decision.wait_until_ns = next.next_deadline_ns;
    const int64_t completed_deadline = next.next_deadline_ns;
    AdvanceDeadline(next, completed_deadline);
    decision.next_state = next;
    return decision;
  }

  const int64_t missed_deadline = next.next_deadline_ns;
  AdvanceDeadline(next, missed_deadline);
  if (now_ns < next.next_deadline_ns) {
    decision.next_state = next;
    return decision;
  }

  decision.late_reset = true;
  next = {.frames_per_second = frames_per_second};
  AdvanceDeadline(next, now_ns);
  decision.next_state = next;
  return decision;
}

}  // namespace gta4::frame_limiter
