#include <catch2/catch_test_macros.hpp>

#include "gta4_frame_limiter.h"

namespace frame_limiter = gta4::frame_limiter;

TEST_CASE("GTA IV frame limiter accepts only menu-supported rates") {
  REQUIRE(frame_limiter::IsSupportedLimit(0));
  REQUIRE(frame_limiter::IsSupportedLimit(30));
  REQUIRE(frame_limiter::IsSupportedLimit(40));
  REQUIRE(frame_limiter::IsSupportedLimit(60));
  REQUIRE(frame_limiter::IsSupportedLimit(120));
  REQUIRE_FALSE(frame_limiter::IsSupportedLimit(1));
  REQUIRE_FALSE(frame_limiter::IsSupportedLimit(144));
}

TEST_CASE("GTA IV frame limiter distributes fractional nanoseconds without drift") {
  frame_limiter::State state{};
  auto decision = frame_limiter::Plan(state, 60, 1'000);
  REQUIRE(decision.mode_changed);
  REQUIRE(decision.next_state.next_deadline_ns == 16'667'666);

  decision =
      frame_limiter::Plan(decision.next_state, 60, decision.next_state.next_deadline_ns);
  decision =
      frame_limiter::Plan(decision.next_state, 60, decision.next_state.next_deadline_ns);
  REQUIRE(decision.next_state.next_deadline_ns == 50'001'000);
}

TEST_CASE("GTA IV frame limiter waits against the persistent deadline") {
  auto first = frame_limiter::Plan({}, 30, 5'000);
  auto second = frame_limiter::Plan(first.next_state, 30, 6'000);
  REQUIRE(second.should_wait(6'000));
  REQUIRE(second.wait_until_ns == first.next_state.next_deadline_ns);
  REQUIRE_FALSE(second.late_reset);
}

TEST_CASE("GTA IV frame limiter changes rates live and disables immediately") {
  auto limited = frame_limiter::Plan({}, 30, 5'000);
  auto changed = frame_limiter::Plan(limited.next_state, 120, 6'000);
  REQUIRE(changed.mode_changed);
  REQUIRE(changed.next_state.frames_per_second == 120);
  REQUIRE(changed.wait_until_ns == 0);

  auto unlocked = frame_limiter::Plan(changed.next_state, 0, 7'000);
  REQUIRE(unlocked.mode_changed);
  REQUIRE(unlocked.next_state.frames_per_second == 0);
  REQUIRE(unlocked.next_state.next_deadline_ns == 0);
  REQUIRE_FALSE(unlocked.should_wait(7'000));
}

TEST_CASE("GTA IV frame limiter resets after a complete missed interval") {
  auto first = frame_limiter::Plan({}, 60, 1'000);
  const int64_t severely_late = first.next_state.next_deadline_ns + 20'000'000;
  auto recovered = frame_limiter::Plan(first.next_state, 60, severely_late);
  REQUIRE(recovered.late_reset);
  REQUIRE(recovered.wait_until_ns == 0);
  REQUIRE(recovered.next_state.next_deadline_ns > severely_late);
}

TEST_CASE("Every selectable GTA IV cap constrains producer cadence") {
  for (uint32_t fps : {30u, 40u, 60u, 120u}) {
    frame_limiter::State state{};
    const int64_t origin = 1000;
    int64_t now = origin;
    const uint32_t frames = fps * 10;
    for (uint32_t frame = 0; frame < frames; ++frame) {
      const auto decision = frame_limiter::Plan(state, fps, now);
      REQUIRE(decision.next_state.frames_per_second == fps);
      REQUIRE_FALSE(decision.late_reset);
      now = decision.wait_until_ns > now ? decision.wait_until_ns : now;
      state = decision.next_state;
      now += 700'000;
    }
    REQUIRE(state.next_deadline_ns == origin + 10'000'000'000);
  }
}
