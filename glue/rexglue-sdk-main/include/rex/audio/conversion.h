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

// NEON intrinsics MUST be included at global scope so simde and any other
// consumer can find them; otherwise namespace-scoped includes shadow the
// builtin/intrinsic declarations.
#if REX_ARCH_ARM64
#include <arm_neon.h>
#endif

namespace rex::audio::conversion {

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
  const __m128 half = _mm_set1_ps(0.5f);
  const __m128 two_fifths = _mm_set1_ps(1.0f / 2.5f);

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

    __m128 center_halved = _mm_mul_ps(fc, half);
    __m128 left = _mm_add_ps(_mm_add_ps(fl, bl), center_halved);
    __m128 right = _mm_add_ps(_mm_add_ps(fr, br), center_halved);
    left = _mm_mul_ps(left, two_fifths);
    right = _mm_mul_ps(right, two_fifths);
    _mm_storeu_ps(&output[sample * 2], _mm_unpacklo_ps(left, right));
    _mm_storeu_ps(&output[(sample + 2) * 2], _mm_unpackhi_ps(left, right));
  }
}
#elif REX_ARCH_ARM64
// arm_neon.h is included at global scope above (outside this namespace) so
// simde and other consumers can find the NEON intrinsics.

// NEON byte-swap: reverse bytes within each 32-bit lane (BE float -> LE float)
static inline float32x4_t neon_byte_swap_f32x4(float32x4_t v) {
  uint8x16_t bytes = vreinterpretq_u8_f32(v);
  return vreinterpretq_f32_u8(vrev32q_u8(bytes));
}

inline void sequential_6_BE_to_interleaved_6_LE(float* output, const float* input,
                                                size_t ch_sample_count) {
  const uint32_t* in = reinterpret_cast<const uint32_t*>(input);
  uint32_t* out = reinterpret_cast<uint32_t*>(output);
  for (size_t sample = 0; sample < ch_sample_count; sample++) {
    // Gather one sample from each of the first 4 channels
    uint32x4_t sample0 = {
        in[0 * ch_sample_count + sample],
        in[1 * ch_sample_count + sample],
        in[2 * ch_sample_count + sample],
        in[3 * ch_sample_count + sample]};
    uint32_t sample1 = in[4 * ch_sample_count + sample];
    uint32_t sample2 = in[5 * ch_sample_count + sample];
    // Byte-swap all 4 lanes at once
    sample0 = vreinterpretq_u32_u8(vrev32q_u8(vreinterpretq_u8_u32(sample0)));
    vst1q_u32(&out[sample * 6], sample0);
    // Byte-swap remaining 2 channels scalar
    out[sample * 6 + 4] = rex::byte_swap(sample1);
    out[sample * 6 + 5] = rex::byte_swap(sample2);
  }
}

inline void sequential_6_BE_to_interleaved_2_LE(float* output, const float* input,
                                                size_t ch_sample_count) {
  assert_true(ch_sample_count % 4 == 0);

  const float32x4_t half = vdupq_n_f32(0.5f);
  const float32x4_t two_fifths = vdupq_n_f32(1.0f / 2.5f);

  // 5.1 downmix to stereo: center on left and right, discard low frequency
  for (size_t sample = 0; sample < ch_sample_count; sample += 4) {
    // Load 4 samples from each of the 5 relevant channels
    float32x4_t fl = vld1q_f32(&input[0 * ch_sample_count + sample]);
    float32x4_t fr = vld1q_f32(&input[1 * ch_sample_count + sample]);
    float32x4_t fc = vld1q_f32(&input[2 * ch_sample_count + sample]);
    float32x4_t bl = vld1q_f32(&input[4 * ch_sample_count + sample]);
    float32x4_t br = vld1q_f32(&input[5 * ch_sample_count + sample]);
    // Byte-swap BE -> LE
    fl = neon_byte_swap_f32x4(fl);
    fr = neon_byte_swap_f32x4(fr);
    fc = neon_byte_swap_f32x4(fc);
    bl = neon_byte_swap_f32x4(bl);
    br = neon_byte_swap_f32x4(br);

    // Downmix: L = (fl + bl + fc*0.5) * (1/2.5), R = (fr + br + fc*0.5) * (1/2.5)
    float32x4_t center_halved = vmulq_f32(fc, half);
    float32x4_t left = vmulq_f32(vaddq_f32(vaddq_f32(fl, bl), center_halved), two_fifths);
    float32x4_t right = vmulq_f32(vaddq_f32(vaddq_f32(fr, br), center_halved), two_fifths);

    // Interleave stereo pairs: [L0,R0,L1,R1] and [L2,R2,L3,R3]
    float32x4x2_t interleaved = vzipq_f32(left, right);
    vst1q_f32(&output[sample * 2], interleaved.val[0]);
    vst1q_f32(&output[(sample + 2) * 2], interleaved.val[1]);
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
    float br = rex::byte_swap(input[4 * ch_sample_count + sample]);
    float bl = rex::byte_swap(input[5 * ch_sample_count + sample]);
    float center_halved = fc * 0.5f;
    output[sample * 2] = (fl + bl + center_halved) * (1.0f / 2.5f);
    output[sample * 2 + 1] = (fr + br + center_halved) * (1.0f / 2.5f);
  }
}
#endif

}  // namespace rex::audio::conversion
