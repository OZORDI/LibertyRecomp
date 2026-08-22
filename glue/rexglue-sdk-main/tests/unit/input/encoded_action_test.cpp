#include <catch2/catch_test_macros.hpp>

#include <rex/input/mnk/encoded_action.h>

namespace {

using rex::input::mnk::DecodeActionMagnitude;
using rex::input::mnk::EncodeActionMagnitude;
using rex::input::mnk::MergeActionMagnitude;

TEST_CASE("action magnitude encoding preserves both polarities",
          "[input][encoded_action]") {
  for (const uint8_t polarity : {uint8_t{0}, uint8_t{255}}) {
    for (const uint8_t magnitude :
         {uint8_t{0}, uint8_t{1}, uint8_t{127}, uint8_t{255}}) {
      CHECK(DecodeActionMagnitude(
                polarity, EncodeActionMagnitude(polarity, magnitude)) ==
            magnitude);
    }
  }
}

TEST_CASE("action merge compares decoded magnitudes",
          "[input][encoded_action]") {
  CHECK(MergeActionMagnitude(0, 0, 255) == 255);
  CHECK(MergeActionMagnitude(255, 255, 255) == 0);

  const uint8_t positive_existing = EncodeActionMagnitude(0, 200);
  const uint8_t negative_existing = EncodeActionMagnitude(255, 200);
  CHECK(MergeActionMagnitude(0, positive_existing, 100) == positive_existing);
  CHECK(MergeActionMagnitude(255, negative_existing, 100) ==
        negative_existing);
  CHECK(DecodeActionMagnitude(
            255, MergeActionMagnitude(255, negative_existing, 250)) == 250);
}

}  // namespace
