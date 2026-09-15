#pragma once

namespace rex::graphics::gta4_native {

// MoltenVK implements Apple stage-counter timestamps with additional fenced
// blit encoders. Keep routine captures to the frame envelope; fine attribution
// is still available as an explicit, perturbing diagnostic experiment.
constexpr bool NativeDetailedGpuTimingEnabled(bool requested, bool moltenvk,
                                              bool allow_moltenvk_detail) {
  return requested && (!moltenvk || allow_moltenvk_detail);
}

}  // namespace rex::graphics::gta4_native
