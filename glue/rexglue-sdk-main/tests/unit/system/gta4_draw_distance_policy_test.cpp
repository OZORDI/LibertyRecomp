#include <bit>
#include <cstdint>
#include <cmath>
#include <limits>

#include <catch2/catch_test_macros.hpp>

#include "../../../gta4-recomp/src/gta4_draw_distance_policy.h"

namespace draw_distance = gta4::draw_distance;

TEST_CASE("GTA IV draw-distance policy publishes configured engine scales",
          "[gta4][draw-distance]") {
  CHECK(std::bit_cast<uint32_t>(draw_distance::ResolveEngineScale(1.0)) == 0x3F800000);
  CHECK(std::bit_cast<uint32_t>(draw_distance::ResolveEngineScale(3.0)) == 0x40400000);
  CHECK(std::bit_cast<uint32_t>(draw_distance::ResolveEngineScale(4.0)) == 0x40800000);
}

TEST_CASE("GTA IV draw-distance policy rejects invalid host scales",
          "[gta4][draw-distance]") {
  CHECK(std::bit_cast<uint32_t>(draw_distance::ResolveEngineScale(0.0)) == 0x3F800000);
  CHECK(std::bit_cast<uint32_t>(draw_distance::ResolveEngineScale(-1.0)) == 0x3F800000);
  CHECK(std::bit_cast<uint32_t>(draw_distance::ResolveEngineScale(
            std::numeric_limits<double>::infinity())) == 0x3F800000);
  CHECK(std::bit_cast<uint32_t>(draw_distance::ResolveEngineScale(
            std::numeric_limits<double>::quiet_NaN())) == 0x3F800000);
  CHECK(std::bit_cast<uint32_t>(draw_distance::ResolveEngineScale(
            std::numeric_limits<double>::max())) == 0x3F800000);
}

namespace {
// The PPC fcmp/fmadds/fmsubs block shared by sub_821D6260 and sub_821D8CD8.
// derive_draw_distance_hooks.py proves only its limit operand is substituted.
float RetailTestDistance(float distance, float threshold, float limit) {
  if (distance > limit && threshold > limit && distance < threshold + 20.0f) {
    return (threshold - limit) + distance;
  }
  return distance;
}
}  // namespace

TEST_CASE("Extended draw distance survives both retail remapping paths",
          "[gta4][draw-distance]") {
  const float limit = draw_distance::ResolveRemapLimit(3.0);
  CHECK(limit == 900.0f);
  // Reported regression: 200-unit entity, 3x scale, camera 350 units away.
  CHECK(RetailTestDistance(350.0f, 600.0f, 300.0f) == 650.0f);
  CHECK(RetailTestDistance(350.0f, 600.0f, limit) == 350.0f);
  CHECK(RetailTestDistance(350.0f, 600.0f, limit) < 600.0f);
  CHECK(RetailTestDistance(599.0f, 600.0f, limit) < 600.0f);
  CHECK(RetailTestDistance(621.0f, 600.0f, limit) > 600.0f);
  // Preserve the game's cap for long-range entities, scaled with the setting.
  CHECK(RetailTestDistance(950.0f, 1200.0f, limit) == 1250.0f);
  CHECK(draw_distance::ResolveRemapLimit(4.0) == 1200.0f);
}

TEST_CASE("Retail scale and invalid configuration retain the stock remapping boundary",
          "[gta4][draw-distance]") {
  for (double scale : {1.0, 0.0, -1.0, std::numeric_limits<double>::infinity(),
                       std::numeric_limits<double>::quiet_NaN()}) {
    const float limit = draw_distance::ResolveRemapLimit(scale);
    CHECK(limit == 300.0f);
    CHECK(RetailTestDistance(350.0f, 400.0f, limit) == 450.0f);
    CHECK(RetailTestDistance(250.0f, 400.0f, limit) == 250.0f);
  }
}

TEST_CASE("Draw-distance slider has exact endpoints and never wraps",
          "[gta4][draw-distance][frontend]") {
  CHECK(draw_distance::SliderPosition(1.0) == 0);
  CHECK(draw_distance::SliderPosition(3.0) == 20);
  CHECK(draw_distance::SliderPosition(4.0) == 30);
  CHECK(draw_distance::SliderPosition(2.34) == 13);
  CHECK(draw_distance::ScaleAtSliderPosition(-1) == 1.0);
  CHECK(draw_distance::ScaleAtSliderPosition(31) == 4.0);
  CHECK(draw_distance::AdjustSlider(1.0, -1) == 1.0);
  CHECK(draw_distance::AdjustSlider(4.0, 1) == 4.0);
  CHECK(draw_distance::AdjustSlider(2.34, 0) == 2.34);
  CHECK(draw_distance::AdjustSlider(2.34, 1) == 2.4);

  double scale = 1.0;
  for (int32_t position = 0; position <= draw_distance::kSliderIntervals; ++position) {
    CHECK(draw_distance::SliderPosition(scale) == position);
    CHECK(draw_distance::SliderPosition(draw_distance::ScaleAtSliderPosition(position)) == position);
    scale = draw_distance::AdjustSlider(scale, 1);
  }
  CHECK(scale == 4.0);
  for (int32_t position = draw_distance::kSliderIntervals; position >= 0; --position) {
    CHECK(draw_distance::SliderPosition(scale) == position);
    scale = draw_distance::AdjustSlider(scale, -1);
  }
  CHECK(scale == 1.0);
}
