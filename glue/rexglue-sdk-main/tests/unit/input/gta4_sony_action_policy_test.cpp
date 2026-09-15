#include <catch2/catch_test_macros.hpp>

#include "gta4_sony_action_policy.h"

namespace gta4::sony {
namespace {
using Gesture = rex::input::sony::Gesture;

void Establish(PulseQueue& queue, bool vehicle = false, bool driver = false) {
  queue.Advance(1, 10, 20, true, vehicle, driver, {}, false);
}
}  // namespace

TEST_CASE("Sony radio swipes generate press then release on separate polls", "[input][sony]") {
  PulseQueue queue;
  Establish(queue, true, true);
  const std::array gestures = {Gesture::kNextRadio};
  const auto press = queue.Advance(1, 10, 20, true, true, true, gestures, false);
  CHECK(press.current[4]);
  CHECK_FALSE(press.previous[4]);
  const auto release = queue.Advance(1, 10, 20, true, true, true, {}, false);
  CHECK_FALSE(release.current[4]);
  CHECK(release.previous[4]);
  const auto idle = queue.Advance(1, 10, 20, true, true, true, {}, false);
  CHECK_FALSE(idle.current[4]);
  CHECK_FALSE(idle.previous[4]);
}

TEST_CASE("Sony repeated swipes retain a release between presses", "[input][sony]") {
  PulseQueue queue;
  Establish(queue);
  const std::array gestures = {Gesture::kNextWeapon, Gesture::kNextWeapon};
  CHECK(queue.Advance(1, 10, 20, true, false, false, gestures, false).current[1]);
  CHECK_FALSE(queue.Advance(1, 10, 20, true, false, false, {}, false).current[1]);
  CHECK(queue.Advance(1, 10, 20, true, false, false, {}, false).current[1]);
  CHECK_FALSE(queue.Advance(1, 10, 20, true, false, false, {}, false).current[1]);
}

TEST_CASE("Sony gesture routes respect vehicle and driver context", "[input][sony]") {
  const std::array gestures = {Gesture::kCamera, Gesture::kNextWeapon,
      Gesture::kPreviousWeapon, Gesture::kNextRadio, Gesture::kPreviousRadio};
  PulseQueue foot;
  Establish(foot);
  const auto on_foot = foot.Advance(1, 10, 20, true, false, false, gestures, true);
  CHECK(on_foot.current[0]);
  CHECK(on_foot.current[1]);
  CHECK(on_foot.current[2]);
  CHECK_FALSE(on_foot.current[3]);
  CHECK_FALSE(on_foot.current[4]);
  CHECK_FALSE(on_foot.current[5]);
  PulseQueue passenger;
  Establish(passenger, true, false);
  const auto seated = passenger.Advance(1, 10, 20, true, true, false, gestures, true);
  CHECK(seated.current[0]);
  CHECK_FALSE(seated.current[1]);
  CHECK_FALSE(seated.current[2]);
  CHECK(seated.current[3]);
  CHECK_FALSE(seated.current[4]);
  CHECK_FALSE(seated.current[5]);
}

TEST_CASE("Sony context ownership and reconnect cancel pending and release edges", "[input][sony]") {
  const std::array gestures = {Gesture::kNextRadio, Gesture::kNextRadio};
  for (int transition = 0; transition < 4; ++transition) {
    PulseQueue queue;
    Establish(queue, true, true);
    queue.Advance(1, 10, 20, true, true, true, gestures, false);
    const auto cancelled = queue.Advance(transition == 0 ? 2 : 1,
        transition == 1 ? 11 : 10, transition == 2 ? 21 : 20,
        transition != 3, true, true, gestures, false);
    CHECK(cancelled.current == ActionEpoch{}.current);
    CHECK(cancelled.previous == ActionEpoch{}.previous);
  }
}

TEST_CASE("Sony cinematic contact remains held until the service releases it", "[input][sony]") {
  PulseQueue queue;
  Establish(queue, true, true);
  CHECK(queue.Advance(1, 10, 20, true, true, true, {}, true).current[3]);
  const auto held = queue.Advance(1, 10, 20, true, true, true, {}, true);
  CHECK(held.current[3]);
  CHECK(held.previous[3]);
  const auto released = queue.Advance(1, 10, 20, true, true, true, {}, false);
  CHECK_FALSE(released.current[3]);
  CHECK(released.previous[3]);
}

TEST_CASE("Sony losing driver ownership cancels radio even when the identity token is unchanged",
          "[input][sony]") {
  PulseQueue queue;
  Establish(queue, true, true);
  const std::array gestures = {Gesture::kNextRadio, Gesture::kNextRadio};
  queue.Advance(1, 10, 20, true, true, true, gestures, false);
  const auto passenger = queue.Advance(1, 10, 20, true, true, false, {}, false);
  CHECK_FALSE(passenger.current[4]);
  CHECK_FALSE(passenger.previous[4]);
  queue.Advance(1, 10, 20, true, true, true, {}, false);
  CHECK_FALSE(queue.Advance(1, 10, 20, true, true, true, {}, false).current[4]);
}

TEST_CASE("Sony action history survives repeated control replays and cancellation", "[input][sony]") {
  ActionHistory history;
  CHECK(history.Merge(1, 0, {0, 0}, true, false) == ActionBytes{255, 0});
  // Retail advanced our own synthetic current when replaying the same object.
  CHECK(history.Merge(1, 0, {0, 255}, true, false) == ActionBytes{255, 0});
  CHECK(history.Merge(2, 0, {0, 255}, false, true) == ActionBytes{0, 255});
  CHECK(history.Merge(2, 0, {0, 0}, false, true) == ActionBytes{0, 255});
  CHECK(history.Merge(3, 0, {0, 0}, false, false) == ActionBytes{0, 0});

  ActionHistory cancelled;
  cancelled.Merge(1, 0, {0, 0}, true, false);
  CHECK(cancelled.Merge(2, 0, {0, 255}, false, false) == ActionBytes{0, 0});
}

TEST_CASE("Sony overlays retain physical input and encoded action polarity", "[input][sony]") {
  ActionHistory physical;
  CHECK(physical.Merge(1, 0, {255, 255}, true, false) == ActionBytes{255, 255});
  CHECK(physical.Merge(2, 0, {255, 255}, false, false) == ActionBytes{255, 255});
  CHECK(physical.Merge(3, 0, {0, 255}, false, false) == ActionBytes{0, 255});

  ActionHistory inverted;
  CHECK(inverted.Merge(1, 255, {255, 255}, true, false) == ActionBytes{0, 255});
  CHECK(inverted.Merge(2, 255, {255, 0}, false, true) == ActionBytes{255, 0});
  CHECK(inverted.Merge(3, 255, {255, 255}, false, false) == ActionBytes{255, 255});
}

TEST_CASE("Sony gameplay ownership follows retail control mode and UI blockers", "[input][sony]") {
  CHECK(GameplayOwned(true, 0, false, 0, false, false, false, false));
  CHECK_FALSE(GameplayOwned(false, 0, false, 0, false, false, false, false));
  CHECK_FALSE(GameplayOwned(true, 1, false, 0, false, false, false, false));
  CHECK_FALSE(GameplayOwned(true, 1024, false, 0, false, false, false, false));
  CHECK(GameplayOwned(true, 1024, true, 0, false, false, false, false));
  CHECK_FALSE(GameplayOwned(true, 1025, true, 0, false, false, false, false));
  CHECK_FALSE(GameplayOwned(true, 0, false, 1, false, false, false, false));
  CHECK_FALSE(GameplayOwned(true, 0, false, 0, true, false, false, false));
  CHECK_FALSE(GameplayOwned(true, 0, false, 0, false, true, false, false));
  CHECK_FALSE(GameplayOwned(true, 0, false, 0, false, false, true, false));
  CHECK_FALSE(GameplayOwned(true, 0, false, 0, false, false, false, true));
}

}  // namespace gta4::sony
