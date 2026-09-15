#pragma once

#include <cstdint>
#include <vulkan/vulkan_core.h>

#include "native_fixed_function_policy.h"

namespace rex::graphics::gta4_native {

// Component write masks are pipeline state. This comparison is only valid
// inside the existing LOAD/STORE scope; resolves, clears and sampling barriers
// still end rendering through the normal command-ordering path.
template <typename Target>
constexpr bool NativeRenderingAttachmentsEqual(const Target& left, const Target& right) {
    return left.color_views == right.color_views && left.depth_view == right.depth_view &&
           left.color_formats == right.color_formats && left.depth_format == right.depth_format &&
           left.samples == right.samples && left.width == right.width &&
           left.height == right.height &&
           // Component write masks belong to the pipeline, not the rendering
           // attachment identity. Keep a compatible LOAD/STORE scope open.
           left.color_attachment_mask == right.color_attachment_mask &&
           left.depth_stencil_attachment_active == right.depth_stencil_attachment_active &&
           left.uses_presenter == right.uses_presenter;
}

// A stencil-only light setup can execute in the already-open lighting pass.
// Borrow only its attachment bindings; the setup retains its zero write mask,
// depth identity, and guest state. This must only be called after all operations
// that can end rendering, for a setup shader with no texture reads or outputs.
template <typename Target>
bool ReuseNativeLightingAttachments(const Target& active, Target& setup) {
  if (!active.color_attachment_mask || setup.color_attachment_mask || setup.color_write_mask ||
      !active.depth_stencil_attachment_active || !setup.depth_stencil_attachment_active ||
      !setup.depth_view || active.depth_view != setup.depth_view ||
      active.depth_surface != setup.depth_surface || active.depth_format != setup.depth_format ||
      active.samples != setup.samples || active.guest_samples != setup.guest_samples ||
      active.width != setup.width || active.height != setup.height ||
      active.logical_width != setup.logical_width || active.logical_height != setup.logical_height ||
      active.uses_presenter || setup.uses_presenter || active.is_reflection || setup.is_reflection) {
    return false;
  }
  setup.color_surfaces = active.color_surfaces;
  setup.color_views = active.color_views;
  setup.color_formats = active.color_formats;
  setup.color_attachment_mask = active.color_attachment_mask;
  return true;
}

// Calculated from guest state before images are allocated. Inactive bound
// descriptors cannot constrain dimensions, sample count, or target families.
struct NativeAttachmentUsage {
  uint32_t color_attachment_mask = 0;
  uint32_t color_write_mask = 0;
  VkImageAspectFlags depth_stencil_aspects = 0;
};

constexpr NativeAttachmentUsage SelectNativeDrawAttachmentUsage(
    uint32_t color_write_mask, uint32_t shader_output_mask,
    uint32_t format_component_masks, bool depth_enable, bool stencil_enable) {
  NativeAttachmentUsage usage{};
  usage.color_write_mask = NormalizeNativeColorWriteMask(
      color_write_mask, shader_output_mask, format_component_masks);
  usage.color_attachment_mask = NativeColorTargetMaskFromWriteMask(usage.color_write_mask);
  if (depth_enable) {
    usage.depth_stencil_aspects |= VK_IMAGE_ASPECT_DEPTH_BIT;
  }
  if (stencil_enable) {
    usage.depth_stencil_aspects |= VK_IMAGE_ASPECT_STENCIL_BIT;
  }
  return usage;
}

constexpr NativeAttachmentUsage SelectNativeClearAttachmentUsage(uint32_t flags) {
  NativeAttachmentUsage usage{};
  usage.color_attachment_mask = flags & 0xFu;
  for (uint32_t index = 0; index < kNativeFixedRenderTargetCount; ++index) {
    if (usage.color_attachment_mask & (1u << index)) {
      usage.color_write_mask |= 0xFu << (index * 4u);
    }
  }
  if (flags & 0x10u) {
    usage.depth_stencil_aspects |= VK_IMAGE_ASPECT_DEPTH_BIT;
  }
  if (flags & 0x20u) {
    usage.depth_stencil_aspects |= VK_IMAGE_ASPECT_STENCIL_BIT;
  }
  return usage;
}

// No tracing fields enter attachment selection. Depth and stencil use separate
// VkRenderingAttachmentInfo objects so their load operations stay independent.
inline void SetNativeRenderingAttachmentBindings(
    VkRenderingInfo& rendering, VkImageAspectFlags aspects,
    const VkRenderingAttachmentInfo& depth_or_color,
    const VkRenderingAttachmentInfo& stencil) {
  rendering.colorAttachmentCount = (aspects & VK_IMAGE_ASPECT_COLOR_BIT) ? 1u : 0u;
  rendering.pColorAttachments = rendering.colorAttachmentCount ? &depth_or_color : nullptr;
  rendering.pDepthAttachment = (aspects & VK_IMAGE_ASPECT_DEPTH_BIT) ? &depth_or_color : nullptr;
  rendering.pStencilAttachment = (aspects & VK_IMAGE_ASPECT_STENCIL_BIT) ? &stencil : nullptr;
}

}  // namespace rex::graphics::gta4_native
