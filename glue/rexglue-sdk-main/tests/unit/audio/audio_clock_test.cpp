#include <chrono>
#include <cstdint>

#include <catch2/catch_test_macros.hpp>

#include <rex/audio/rational_frame_clock.h>

TEST_CASE("audio rational clock carries fractional nanoseconds without drift", "[audio][clock]") {
  rex::audio::RationalFrameClock clock(48000, 256);
  int64_t total_nanoseconds = 0;
  for (uint32_t i = 0; i < 1000; ++i) {
    total_nanoseconds += clock.NextDuration().count();
  }
  CHECK(total_nanoseconds == 5333333333LL);
}

TEST_CASE("audio rational clock reset reproduces the first quantum", "[audio][clock]") {
  rex::audio::RationalFrameClock clock(48000, 256);
  const auto first = clock.NextDuration();
  clock.NextDuration();
  clock.Reset();
  CHECK(clock.NextDuration() == first);
}
