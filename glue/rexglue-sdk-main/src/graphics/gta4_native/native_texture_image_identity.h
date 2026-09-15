#pragma once

#include <array>
#include <cstdint>

#include <rex/graphics/xenos.h>

namespace rex::graphics::gta4_native {

// Sampling state belongs to the draw's fetch and native sampler, not to the
// immutable image payload. Keep storage, mip coverage and view interpretation
// exact: in particular, changing swizzle or the captured mip range still
// requires a different image generation.
inline std::array<uint32_t, 6> NativeTextureImageFetchKey(
    xenos::xe_gpu_texture_fetch_t fetch) {
  fetch.clamp_x = {};
  fetch.clamp_y = {};
  fetch.clamp_z = {};
  fetch.nearest_clamp_policy = 0;
  fetch.mag_filter = {};
  fetch.min_filter = {};
  fetch.mip_filter = {};
  fetch.aniso_filter = {};
  fetch.arbitrary_filter = {};
  fetch.vol_mag_filter = 0;
  fetch.vol_min_filter = 0;
  fetch.mag_aniso_walk = 0;
  fetch.min_aniso_walk = 0;
  fetch.lod_bias = 0;
  fetch.grad_exp_adjust_h = 0;
  fetch.grad_exp_adjust_v = 0;
  fetch.border_color = {};
  fetch.tri_clamp = 0;
  fetch.aniso_bias = 0;
  return {fetch.dword_0, fetch.dword_1, fetch.dword_2,
          fetch.dword_3, fetch.dword_4, fetch.dword_5};
}

inline bool NativeTextureImageFetchEqual(const xenos::xe_gpu_texture_fetch_t& left,
                                          const xenos::xe_gpu_texture_fetch_t& right) {
  return NativeTextureImageFetchKey(left) == NativeTextureImageFetchKey(right);
}

}  // namespace rex::graphics::gta4_native
