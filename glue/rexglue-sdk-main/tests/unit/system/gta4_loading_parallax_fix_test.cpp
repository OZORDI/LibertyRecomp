#include <bit>
#include <cmath>
#include <cstdint>
#include <limits>

#include <catch2/catch_test_macros.hpp>

#include "../../../gta4-recomp/src/gta4_loading_parallax_fix.h"

namespace loading_parallax = gta4::loading_parallax;

namespace {

float FloatFromBits(uint32_t bits) {
  return std::bit_cast<float>(bits);
}

}  // namespace

TEST_CASE("GTA IV loading parallax uses a 30 Hz logical accumulator step",
          "[gta4][loading][parallax]") {
  const float corrected = loading_parallax::CorrectAccumulator(
      FloatFromBits(0x3E800000), FloatFromBits(0x3FE00000), FloatFromBits(0x3FC00000));
  CHECK(std::bit_cast<uint32_t>(corrected) == 0x3E911111);
}

TEST_CASE("GTA IV loading parallax preserves non-accumulation branches",
          "[gta4][loading][parallax]") {
  const float before = FloatFromBits(0x3E800000);
  const float cumulative = FloatFromBits(0x3FC00000);

  CHECK(std::bit_cast<uint32_t>(loading_parallax::CorrectAccumulator(before, before, cumulative)) ==
        0x3E800000);
  CHECK(std::bit_cast<uint32_t>(loading_parallax::CorrectAccumulator(before, 0.0f, cumulative)) ==
        0x00000000);
  CHECK(std::isnan(loading_parallax::CorrectAccumulator(
      before, std::numeric_limits<float>::quiet_NaN(), cumulative)));
  CHECK(std::bit_cast<uint32_t>(loading_parallax::CorrectAccumulator(
            -1.0f, FloatFromBits(0x3FE00000), cumulative)) == 0x3FE00000);
}

TEST_CASE("GTA IV loading parallax advances 30 logical frames per second",
          "[gta4][loading][parallax]") {
  const auto CountAdvances = [](uint32_t render_rate) {
    loading_parallax::LogicalAnimationClock clock;
    uint32_t advances = 0;
    float elapsed = 0.0f;
    const float delta = 1.0f / static_cast<float>(render_rate);
    for (uint32_t frame = 0; frame < render_rate; ++frame) {
      elapsed += delta;
      advances += clock.Observe(elapsed, delta) ? 1 : 0;
    }
    return advances;
  };

  CHECK(CountAdvances(30) == 30);
  CHECK(CountAdvances(60) == 30);
  CHECK(CountAdvances(64) == 30);
  CHECK(CountAdvances(90) == 30);
  CHECK(CountAdvances(120) == 30);
  CHECK(CountAdvances(144) == 30);
  CHECK(CountAdvances(165) == 30);
  CHECK(CountAdvances(240) == 30);
}

TEST_CASE("GTA IV loading parallax reuses cadence within one rendered frame",
          "[gta4][loading][parallax]") {
  loading_parallax::LogicalAnimationClock clock;
  const float delta = FloatFromBits(0x3C888889);
  const float elapsed = delta;

  CHECK_FALSE(clock.Observe(elapsed, delta));
  CHECK_FALSE(clock.Observe(elapsed, delta));
  CHECK(clock.Observe(elapsed + delta, delta));
  CHECK(clock.Observe(elapsed + delta, delta));
}

TEST_CASE("GTA IV loading parallax drops excess stall catch-up steps",
          "[gta4][loading][parallax]") {
  loading_parallax::LogicalAnimationClock clock;

  CHECK(clock.Observe(FloatFromBits(0x3F000000), FloatFromBits(0x3F000000)));
  CHECK_FALSE(clock.Observe(FloatFromBits(0x3F044444), FloatFromBits(0x3C888889)));
}

TEST_CASE("GTA IV loading parallax preserves the 30 FPS two-pass animation rate",
          "[gta4][loading][parallax]") {
  const auto CountAnimationTriggers = [](uint32_t render_rate) {
    loading_parallax::LogicalAnimationClock clock;
    uint32_t triggers = 0;
    float elapsed = 0.0f;
    float timer = 0.0f;
    const float delta = 1.0f / static_cast<float>(render_rate);
    for (uint32_t frame = 0; frame < render_rate; ++frame) {
      elapsed += delta;
      for (uint32_t pass = 0; pass < 2; ++pass) {
        if (!clock.Observe(elapsed, delta)) {
          continue;
        }
        if (timer * 1000.0f >= 20.0f) {
          timer = 0.0f;
          ++triggers;
        } else {
          timer = loading_parallax::CorrectAccumulator(timer, timer + delta, delta);
        }
      }
    }
    return triggers;
  };

  CHECK(CountAnimationTriggers(30) == 30);
  CHECK(CountAnimationTriggers(60) == 30);
  CHECK(CountAnimationTriggers(64) == 30);
  CHECK(CountAnimationTriggers(90) == 30);
  CHECK(CountAnimationTriggers(120) == 30);
  CHECK(CountAnimationTriggers(144) == 30);
  CHECK(CountAnimationTriggers(165) == 30);
  CHECK(CountAnimationTriggers(240) == 30);
}
