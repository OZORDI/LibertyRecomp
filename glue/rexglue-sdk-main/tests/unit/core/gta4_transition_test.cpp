#include <type_traits>

#include <catch2/catch_test_macros.hpp>

#include <rex/diagnostics/gta4_transition.h>

namespace transition = rex::diagnostics::gta4_transition;

TEST_CASE("GTA4 transition event ABI is fixed and realtime-copyable",
          "[diagnostics][gta4-transition]") {
  STATIC_REQUIRE(sizeof(transition::TransitionEvent) == 64);
  STATIC_REQUIRE(alignof(transition::TransitionEvent) == 8);
  STATIC_REQUIRE(std::is_trivially_copyable_v<transition::TransitionEvent>);

  transition::TransitionEvent event{};
  REQUIRE(event.host_tick == 0);
  REQUIRE(event.sequence == 0);
  REQUIRE(event.transition_id == 0);
  REQUIRE(event.flags == transition::kFlagNone);
}

TEST_CASE("GTA4 transition diagnostics are inert by default",
          "[diagnostics][gta4-transition]") {
  REQUIRE(transition::GetMode() == transition::Mode::kOff);
  REQUIRE_FALSE(transition::IsEnabled());
  REQUIRE(transition::ActiveTransitionId() == 0);
  REQUIRE_FALSE(transition::Record(transition::EventSource::kAudio,
                                   transition::EventType::kAudioSubmit));
}
