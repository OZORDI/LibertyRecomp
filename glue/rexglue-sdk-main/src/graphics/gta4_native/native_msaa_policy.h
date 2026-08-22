#pragma once

#include <cstdint>

namespace rex::graphics::gta4_native {

// The title deliberately switches from multisampled deferred attachments to
// single-sampled persistent/forward attachments. A host MSAA quality override
// may change the sample count of the former, but must never promote the latter:
// doing so changes the depth/stencil handoff topology and invalidates the
// title-authored stencil contents used by the scene composite.
constexpr bool ShouldApplyNativeSceneSampleOverride(
    bool reflection_target, bool has_bound_surface,
    bool all_bound_surfaces_are_guest_multisampled) {
  return !reflection_target && has_bound_surface &&
         all_bound_surfaces_are_guest_multisampled;
}

// VkSampleCountFlagBits values are the corresponding power-of-two sample
// counts. Select the requested count when supported, otherwise the greatest
// coherent lower count. Returning zero means even the mandatory 1x fallback
// was not advertised and surface creation must fail explicitly.
constexpr uint32_t SelectSupportedNativeSceneSampleCount(
    uint32_t requested, uint32_t supported) {
  if (requested >= 4u && (supported & 4u)) {
    return 4u;
  }
  if (requested >= 2u && (supported & 2u)) {
    return 2u;
  }
  return supported & 1u;
}

}  // namespace rex::graphics::gta4_native
