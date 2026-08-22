#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

namespace rex::graphics::gta4_native {

enum class DepthHandoffTransport : uint8_t {
  kBuffer,
  kAttachment,
};

enum class DepthHandoffAttachmentRejectReason : uint8_t {
  kNone,
  kNotRequested,
  kSourceNotSampled,
  kDescriptorObjectsUnavailable,
  kDestinationViewUnavailable,
  kSourceNotSingleSampled,
  kDestinationNotSingleSampled,
  kAspectMismatch,
  kFormatMismatch,
  kExtentMismatch,
  kPipelineUnavailable,
};

struct DepthHandoffAttachmentCapabilities {
  bool source_sampled = false;
  bool descriptor_objects_available = false;
  bool destination_view_available = false;
  bool source_single_sampled = false;
  bool destination_single_sampled = false;
  bool matching_depth_stencil_aspects = false;
  bool matching_format = false;
  bool matching_extent = false;
  bool pipeline_available = false;
};

struct DepthHandoffTransportSelection {
  DepthHandoffTransport transport = DepthHandoffTransport::kBuffer;
  DepthHandoffAttachmentRejectReason reject_reason =
      DepthHandoffAttachmentRejectReason::kNotRequested;
};

struct DepthImageAccessScope {
  VkPipelineStageFlags stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
  VkAccessFlags access = 0;
};

constexpr DepthImageAccessScope GetDepthImageAccessScope(VkImageLayout layout) {
  switch (layout) {
    case VK_IMAGE_LAYOUT_UNDEFINED:
    case VK_IMAGE_LAYOUT_PREINITIALIZED:
      return {};
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
      return {
          VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
          VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
              VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      };
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
      return {
          VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT |
              VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
          VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_SHADER_READ_BIT,
      };
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
      return {
          VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
          VK_ACCESS_SHADER_READ_BIT,
      };
    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
      return {VK_PIPELINE_STAGE_TRANSFER_BIT, VK_ACCESS_TRANSFER_READ_BIT};
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
      return {VK_PIPELINE_STAGE_TRANSFER_BIT, VK_ACCESS_TRANSFER_WRITE_BIT};
    default:
      return {
          VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
          VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT,
      };
  }
}

constexpr DepthHandoffTransportSelection SelectDepthHandoffTransport(
    bool request_attachment, const DepthHandoffAttachmentCapabilities& capabilities) {
  if (!request_attachment) {
    return {};
  }
  if (!capabilities.source_sampled) {
    return {DepthHandoffTransport::kBuffer, DepthHandoffAttachmentRejectReason::kSourceNotSampled};
  }
  if (!capabilities.descriptor_objects_available) {
    return {DepthHandoffTransport::kBuffer,
            DepthHandoffAttachmentRejectReason::kDescriptorObjectsUnavailable};
  }
  if (!capabilities.destination_view_available) {
    return {DepthHandoffTransport::kBuffer,
            DepthHandoffAttachmentRejectReason::kDestinationViewUnavailable};
  }
  if (!capabilities.source_single_sampled) {
    return {DepthHandoffTransport::kBuffer,
            DepthHandoffAttachmentRejectReason::kSourceNotSingleSampled};
  }
  if (!capabilities.destination_single_sampled) {
    return {DepthHandoffTransport::kBuffer,
            DepthHandoffAttachmentRejectReason::kDestinationNotSingleSampled};
  }
  if (!capabilities.matching_depth_stencil_aspects) {
    return {DepthHandoffTransport::kBuffer, DepthHandoffAttachmentRejectReason::kAspectMismatch};
  }
  if (!capabilities.matching_format) {
    return {DepthHandoffTransport::kBuffer, DepthHandoffAttachmentRejectReason::kFormatMismatch};
  }
  if (!capabilities.matching_extent) {
    return {DepthHandoffTransport::kBuffer, DepthHandoffAttachmentRejectReason::kExtentMismatch};
  }
  if (!capabilities.pipeline_available) {
    return {DepthHandoffTransport::kBuffer,
            DepthHandoffAttachmentRejectReason::kPipelineUnavailable};
  }
  return {DepthHandoffTransport::kAttachment, DepthHandoffAttachmentRejectReason::kNone};
}

constexpr const char* DepthHandoffAttachmentRejectReasonName(
    DepthHandoffAttachmentRejectReason reason) {
  switch (reason) {
    case DepthHandoffAttachmentRejectReason::kNone:
      return "none";
    case DepthHandoffAttachmentRejectReason::kNotRequested:
      return "not-requested";
    case DepthHandoffAttachmentRejectReason::kSourceNotSampled:
      return "source-not-sampled";
    case DepthHandoffAttachmentRejectReason::kDescriptorObjectsUnavailable:
      return "descriptor-objects-unavailable";
    case DepthHandoffAttachmentRejectReason::kDestinationViewUnavailable:
      return "destination-view-unavailable";
    case DepthHandoffAttachmentRejectReason::kSourceNotSingleSampled:
      return "source-not-single-sampled";
    case DepthHandoffAttachmentRejectReason::kDestinationNotSingleSampled:
      return "destination-not-single-sampled";
    case DepthHandoffAttachmentRejectReason::kAspectMismatch:
      return "aspect-mismatch";
    case DepthHandoffAttachmentRejectReason::kFormatMismatch:
      return "format-mismatch";
    case DepthHandoffAttachmentRejectReason::kExtentMismatch:
      return "extent-mismatch";
    case DepthHandoffAttachmentRejectReason::kPipelineUnavailable:
      return "pipeline-unavailable";
  }
  return "unknown";
}

}  // namespace rex::graphics::gta4_native
