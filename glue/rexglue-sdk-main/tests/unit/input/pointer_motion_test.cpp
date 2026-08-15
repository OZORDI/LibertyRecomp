#include <catch2/catch_test_macros.hpp>

#include <limits>

#include <rex/input/mnk/pointer_motion.h>

namespace {

using rex::input::mnk::MouseAxisQuantizer;
using rex::input::mnk::PointerMotionAccumulator;
using MotionSource = rex::ui::MouseEvent::MotionSource;

// Generated and verified by tools/generate_mouse_input_constants.py.
constexpr double kReferenceFrameSeconds = 0x1.1111120000000p-5;
constexpr double kSixtyHzFrameSeconds = 0x1.1111120000000p-6;
constexpr double kOneTwentyHzFrameSeconds = 0x1.1111120000000p-7;
constexpr double kMouseUnitsPerCount = 0x1.8000000000000p+3;
constexpr double kHalfCount = 0x1.0000000000000p-1;
constexpr double kQuarterCount = 0x1.0000000000000p-2;

TEST_CASE("raw pointer motion wins over its accelerated duplicate", "[pointer_motion]") {
  PointerMotionAccumulator accumulator;
  accumulator.Add(MotionSource::kSystemAccelerated, 100.0, 200.0);
  accumulator.Add(MotionSource::kRawMouse, 2.5, -1.25);

  const auto sample = accumulator.Consume();
  REQUIRE(sample.has_motion);
  CHECK(sample.source == MotionSource::kRawMouse);
  CHECK(sample.delta_x == 2.5);
  CHECK(sample.delta_y == -1.25);
}

TEST_CASE("an idle raw mouse does not block accelerated trackpad motion", "[pointer_motion]") {
  PointerMotionAccumulator accumulator;
  accumulator.Add(MotionSource::kSystemAccelerated, -3.25, 4.5);

  const auto sample = accumulator.Consume();
  REQUIRE(sample.has_motion);
  CHECK(sample.source == MotionSource::kSystemAccelerated);
  CHECK(sample.delta_x == -3.25);
  CHECK(sample.delta_y == 4.5);
  CHECK_FALSE(accumulator.Consume().has_motion);
}

TEST_CASE("raw and accelerated sources alternate cleanly across samples", "[pointer_motion]") {
  PointerMotionAccumulator accumulator;

  accumulator.Add(MotionSource::kRawMouse, kHalfCount, -kQuarterCount);
  const auto raw = accumulator.Consume();
  REQUIRE(raw.has_motion);
  CHECK(raw.source == MotionSource::kRawMouse);

  accumulator.Add(MotionSource::kSystemAccelerated, -kQuarterCount, kHalfCount);
  const auto accelerated = accumulator.Consume();
  REQUIRE(accelerated.has_motion);
  CHECK(accelerated.source == MotionSource::kSystemAccelerated);
}

TEST_CASE("explicit zero and subpixel motion remains a valid sample", "[pointer_motion]") {
  PointerMotionAccumulator accumulator;
  accumulator.Add(MotionSource::kSystemAccelerated, 0.0, kQuarterCount);

  const auto sample = accumulator.Consume();
  REQUIRE(sample.has_motion);
  CHECK(sample.source == MotionSource::kSystemAccelerated);
  CHECK(sample.delta_x == 0.0);
  CHECK(sample.delta_y == kQuarterCount);
}

TEST_CASE("an all-zero raw event does not mask accelerated movement", "[pointer_motion]") {
  PointerMotionAccumulator accumulator;
  accumulator.Add(MotionSource::kRawMouse, 0.0, 0.0);
  accumulator.Add(MotionSource::kSystemAccelerated, kQuarterCount, 0.0);

  const auto sample = accumulator.Consume();
  REQUIRE(sample.has_motion);
  CHECK(sample.source == MotionSource::kSystemAccelerated);
  CHECK(sample.delta_x == kQuarterCount);
  CHECK(sample.delta_y == 0.0);
}

TEST_CASE("pointer reset drops every pending source", "[pointer_motion]") {
  PointerMotionAccumulator accumulator;
  accumulator.Add(MotionSource::kRawMouse, 1.0, 1.0);
  accumulator.Add(MotionSource::kSystemAccelerated, 1.0, 1.0);
  accumulator.Reset();
  CHECK_FALSE(accumulator.Consume().has_motion);
}

TEST_CASE("non-finite pointer motion is rejected", "[pointer_motion]") {
  PointerMotionAccumulator accumulator;
  accumulator.Add(MotionSource::kRawMouse, std::numeric_limits<double>::infinity(), 1.0);
  CHECK_FALSE(accumulator.Consume().has_motion);
}

TEST_CASE("mouse quantization preserves fractional residuals", "[pointer_motion]") {
  MouseAxisQuantizer quantizer;
  CHECK(quantizer.Quantize(kHalfCount, 1.0, 1.0, kReferenceFrameSeconds, kReferenceFrameSeconds) ==
        0);
  CHECK(quantizer.Quantize(kHalfCount, 1.0, 1.0, kReferenceFrameSeconds, kReferenceFrameSeconds) ==
        1);

  quantizer.Reset();
  CHECK(quantizer.Quantize(kHalfCount, 1.0, 1.0, kReferenceFrameSeconds, kReferenceFrameSeconds) ==
        0);
  CHECK(quantizer.Quantize(-kHalfCount, 1.0, 1.0, kReferenceFrameSeconds, kReferenceFrameSeconds) ==
        0);
}

TEST_CASE("mouse quantization is normalized to the reference frame rate", "[pointer_motion]") {
  MouseAxisQuantizer at_thirty;
  MouseAxisQuantizer at_sixty;
  MouseAxisQuantizer at_one_twenty;

  const int32_t thirty = at_thirty.Quantize(1.0, 1.0, kMouseUnitsPerCount, kReferenceFrameSeconds,
                                            kReferenceFrameSeconds);
  const int32_t sixty = at_sixty.Quantize(kHalfCount, 1.0, kMouseUnitsPerCount,
                                          kSixtyHzFrameSeconds, kReferenceFrameSeconds);
  const int32_t one_twenty = at_one_twenty.Quantize(
      kQuarterCount, 1.0, kMouseUnitsPerCount, kOneTwentyHzFrameSeconds, kReferenceFrameSeconds);
  CHECK(sixty == thirty);
  CHECK(one_twenty == thirty);
}

TEST_CASE("mouse quantization clamps without retaining an overflow backlog", "[pointer_motion]") {
  MouseAxisQuantizer quantizer;
  CHECK(quantizer.Quantize(0x1.0000000000000p+10, 1.0, 1.0, kReferenceFrameSeconds,
                           kReferenceFrameSeconds) == 255);
  CHECK(quantizer.Quantize(0.0, 1.0, 1.0, kReferenceFrameSeconds, kReferenceFrameSeconds) == 0);
}

TEST_CASE("invalid gameplay timestep uses the reference-rate fallback", "[pointer_motion]") {
  MouseAxisQuantizer fallback;
  MouseAxisQuantizer reference;
  CHECK(fallback.Quantize(1.0, 1.0, kMouseUnitsPerCount, 0.0, kReferenceFrameSeconds) ==
        reference.Quantize(1.0, 1.0, kMouseUnitsPerCount, kReferenceFrameSeconds,
                           kReferenceFrameSeconds));
}

}  // namespace
