#include <array>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <rex/audio/conversion.h>
#include <rex/types.h>

namespace {

constexpr size_t kChannels = 6;
constexpr size_t kFrames = 4;

std::array<float, kChannels * kFrames> MakePlanarImpulse(size_t active_channel) {
  std::array<float, kChannels * kFrames> input{};
  for (size_t frame = 0; frame < kFrames; ++frame) {
    input[active_channel * kFrames + frame] = rex::byte_swap(1.0f);
  }
  return input;
}

}  // namespace

TEST_CASE("six-channel big-endian conversion preserves canonical order", "[audio][conversion]") {
  for (size_t active_channel = 0; active_channel < kChannels; ++active_channel) {
    const auto input = MakePlanarImpulse(active_channel);
    std::array<float, kChannels * kFrames> output{};
    rex::audio::conversion::sequential_6_BE_to_interleaved_6_LE(output.data(), input.data(),
                                                                kFrames);
    for (size_t frame = 0; frame < kFrames; ++frame) {
      for (size_t channel = 0; channel < kChannels; ++channel) {
        CHECK(output[frame * kChannels + channel] == (channel == active_channel ? 1.0f : 0.0f));
      }
    }
  }
}

TEST_CASE("stereo downmix maps rear channels to the correct side", "[audio][conversion]") {
  constexpr std::array<std::array<float, 2>, kChannels> expected{{
      {{rex::audio::conversion::kStereoFrontGain, 0.0f}},  // front left
      {{0.0f, rex::audio::conversion::kStereoFrontGain}},  // front right
      {{rex::audio::conversion::kStereoCenterGain,
        rex::audio::conversion::kStereoCenterGain}},       // center
      {{0.0f, 0.0f}},  // LFE is intentionally discarded
      {{rex::audio::conversion::kStereoSurroundGain, 0.0f}},  // back left
      {{0.0f, rex::audio::conversion::kStereoSurroundGain}},  // back right
  }};

  for (size_t active_channel = 0; active_channel < kChannels; ++active_channel) {
    const auto input = MakePlanarImpulse(active_channel);
    std::array<float, 2 * kFrames> output{};
    rex::audio::conversion::sequential_6_BE_to_interleaved_2_LE(output.data(), input.data(),
                                                                kFrames);
    for (size_t frame = 0; frame < kFrames; ++frame) {
      CHECK(output[frame * 2] == Catch::Approx(expected[active_channel][0]));
      CHECK(output[frame * 2 + 1] == Catch::Approx(expected[active_channel][1]));
    }
  }
}

TEST_CASE("stereo downmix attenuates a delayed surround copy", "[audio][conversion]") {
  std::array<float, kChannels * kFrames> input{};
  input[0 * kFrames] = rex::byte_swap(1.0f);
  input[4 * kFrames + 1] = rex::byte_swap(1.0f);

  std::array<float, 2 * kFrames> output{};
  rex::audio::conversion::sequential_6_BE_to_interleaved_2_LE(output.data(), input.data(),
                                                              kFrames);

  CHECK(output[0] == Catch::Approx(rex::audio::conversion::kStereoFrontGain));
  CHECK(output[2] == Catch::Approx(rex::audio::conversion::kStereoSurroundGain));
  CHECK(output[2] < output[0]);
}
