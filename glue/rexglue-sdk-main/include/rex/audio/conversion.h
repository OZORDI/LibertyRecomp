/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2021 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

#pragma once

#include <cstdint>

#include <rex/platform.h>
#include <rex/types.h>

#if REX_ARCH_ARM64
#include <arm_neon.h>
#endif

namespace rex::audio::conversion {

// ITU-R BS.775 Lo/Ro downmix, normalized so coincident full-scale front,
// center, and surround samples cannot overflow a stereo output channel.
// Keeping surround below the direct front signal is also important for game
// mixes, where the surround bus may contain a delayed room/reflection copy.
constexpr float kStereoFrontGain = 0.45308185f;
constexpr float kStereoCenterGain = 0.32037723f;
constexpr float kStereoSurroundGain = 0.22654092f;

#if REX_ARCH_AMD64
inline void sequential_6_BE_to_interleaved_6_LE(float* output, const float* input,
                                                size_t ch_sample_count) {
  const uint32_t* in = reinterpret_cast<const uint32_t*>(input);
  uint32_t* out = reinterpret_cast<uint32_t*>(output);
  const __m128i byte_swap_shuffle =
      _mm_set_epi8(12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3);
  for (size_t sample = 0; sample < ch_sample_count; sample++) {
    __m128i sample0 =
        _mm_set_epi32(in[3 * ch_sample_count + sample], in[2 * ch_sample_count + sample],
                      in[1 * ch_sample_count + sample], in[0 * ch_sample_count + sample]);
    uint32_t sample1 = in[4 * ch_sample_count + sample];
    uint32_t sample2 = in[5 * ch_sample_count + sample];
    sample0 = _mm_shuffle_epi8(sample0, byte_swap_shuffle);
    _mm_storeu_si128(reinterpret_cast<__m128i*>(&out[sample * 6]), sample0);
    sample1 = rex::byte_swap(sample1);
    out[sample * 6 + 4] = sample1;
    sample2 = rex::byte_swap(sample2);
    out[sample * 6 + 5] = sample2;
  }
}

inline void sequential_6_BE_to_interleaved_2_LE(float* output, const float* input,
                                                size_t ch_sample_count) {
  assert_true(ch_sample_count % 4 == 0);

  const __m128i byte_swap_shuffle =
      _mm_set_epi8(12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3);
  const __m128 front_gain = _mm_set1_ps(kStereoFrontGain);
  const __m128 center_gain = _mm_set1_ps(kStereoCenterGain);
  const __m128 surround_gain = _mm_set1_ps(kStereoSurroundGain);

  // put center on left and right, discard low frequency
  for (size_t sample = 0; sample < ch_sample_count; sample += 4) {
    // load 4 samples from 6 channels each
    __m128 fl = _mm_loadu_ps(&input[0 * ch_sample_count + sample]);
    __m128 fr = _mm_loadu_ps(&input[1 * ch_sample_count + sample]);
    __m128 fc = _mm_loadu_ps(&input[2 * ch_sample_count + sample]);
    __m128 bl = _mm_loadu_ps(&input[4 * ch_sample_count + sample]);
    __m128 br = _mm_loadu_ps(&input[5 * ch_sample_count + sample]);
    // byte swap
    fl = _mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(fl), byte_swap_shuffle));
    fr = _mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(fr), byte_swap_shuffle));
    fc = _mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(fc), byte_swap_shuffle));
    bl = _mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(bl), byte_swap_shuffle));
    br = _mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(br), byte_swap_shuffle));

    __m128 left = _mm_add_ps(
        _mm_add_ps(_mm_mul_ps(fl, front_gain), _mm_mul_ps(fc, center_gain)),
        _mm_mul_ps(bl, surround_gain));
    __m128 right = _mm_add_ps(
        _mm_add_ps(_mm_mul_ps(fr, front_gain), _mm_mul_ps(fc, center_gain)),
        _mm_mul_ps(br, surround_gain));
    _mm_storeu_ps(&output[sample * 2], _mm_unpacklo_ps(left, right));
    _mm_storeu_ps(&output[(sample + 2) * 2], _mm_unpackhi_ps(left, right));
  }
}
#elif REX_ARCH_ARM64
inline float32x4_t load_4_BE_float(const float* input) {
  uint8x16_t bytes = vld1q_u8(reinterpret_cast<const uint8_t*>(input));
  return vreinterpretq_f32_u8(vrev32q_u8(bytes));
}

inline void sequential_6_BE_to_interleaved_6_LE(float* output, const float* input,
                                                size_t ch_sample_count) {
  // Six-way interleaved stores have no single NEON instruction. Keep this
  // uncommon surround path scalar while still using the corrected channel
  // order; the latency-critical stereo path below is fully vectorized.
  for (size_t sample = 0; sample < ch_sample_count; sample++) {
    for (size_t channel = 0; channel < 6; channel++) {
      output[sample * 6 + channel] = rex::byte_swap(input[channel * ch_sample_count + sample]);
    }
  }
}

inline void sequential_6_BE_to_interleaved_2_LE(float* output, const float* input,
                                                size_t ch_sample_count) {
  assert_true(ch_sample_count % 4 == 0);
  const float32x4_t front_gain = vdupq_n_f32(kStereoFrontGain);
  const float32x4_t center_gain = vdupq_n_f32(kStereoCenterGain);
  const float32x4_t surround_gain = vdupq_n_f32(kStereoSurroundGain);
  for (size_t sample = 0; sample < ch_sample_count; sample += 4) {
    const float32x4_t fl = load_4_BE_float(&input[0 * ch_sample_count + sample]);
    const float32x4_t fr = load_4_BE_float(&input[1 * ch_sample_count + sample]);
    const float32x4_t fc = load_4_BE_float(&input[2 * ch_sample_count + sample]);
    const float32x4_t bl = load_4_BE_float(&input[4 * ch_sample_count + sample]);
    const float32x4_t br = load_4_BE_float(&input[5 * ch_sample_count + sample]);
    const float32x4_t left = vaddq_f32(
        vaddq_f32(vmulq_f32(fl, front_gain), vmulq_f32(fc, center_gain)),
        vmulq_f32(bl, surround_gain));
    const float32x4_t right = vaddq_f32(
        vaddq_f32(vmulq_f32(fr, front_gain), vmulq_f32(fc, center_gain)),
        vmulq_f32(br, surround_gain));
    const float32x4x2_t stereo{{left, right}};
    vst2q_f32(&output[sample * 2], stereo);
  }
}
#else
inline void sequential_6_BE_to_interleaved_6_LE(float* output, const float* input,
                                                size_t ch_sample_count) {
  for (size_t sample = 0; sample < ch_sample_count; sample++) {
    for (size_t channel = 0; channel < 6; channel++) {
      output[sample * 6 + channel] = rex::byte_swap(input[channel * ch_sample_count + sample]);
    }
  }
}
inline void sequential_6_BE_to_interleaved_2_LE(float* output, const float* input,
                                                size_t ch_sample_count) {
  // Default 5.1 channel mapping is fl, fr, fc, lf, bl, br
  // https://docs.microsoft.com/en-us/windows/win32/xaudio2/xaudio2-default-channel-mapping
  for (size_t sample = 0; sample < ch_sample_count; sample++) {
    // put center on left and right, discard low frequency
    float fl = rex::byte_swap(input[0 * ch_sample_count + sample]);
    float fr = rex::byte_swap(input[1 * ch_sample_count + sample]);
    float fc = rex::byte_swap(input[2 * ch_sample_count + sample]);
    float bl = rex::byte_swap(input[4 * ch_sample_count + sample]);
    float br = rex::byte_swap(input[5 * ch_sample_count + sample]);
    output[sample * 2] =
        fl * kStereoFrontGain + fc * kStereoCenterGain + bl * kStereoSurroundGain;
    output[sample * 2 + 1] =
        fr * kStereoFrontGain + fc * kStereoCenterGain + br * kStereoSurroundGain;
  }
}
#endif

}  // namespace rex::audio::conversion
