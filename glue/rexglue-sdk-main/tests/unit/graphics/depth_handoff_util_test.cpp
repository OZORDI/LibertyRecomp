#include <array>
#include <string_view>
#include <utility>

#include <catch2/catch_test_macros.hpp>

#include "graphics/gta4_native/depth_handoff_util.h"

namespace rex::graphics::gta4_native {
namespace {

DepthHandoffAttachmentCapabilities FullAttachmentCapabilities() {
  return {
      .source_sampled = true,
      .descriptor_objects_available = true,
      .destination_view_available = true,
      .source_single_sampled = true,
      .destination_single_sampled = true,
      .matching_depth_stencil_aspects = true,
      .matching_format = true,
      .matching_extent = true,
      .pipeline_available = true,
  };
}

TEST_CASE("GTA IV depth handoff transport defaults to proven buffer path") {
  const DepthHandoffTransportSelection selection =
      SelectDepthHandoffTransport(false, FullAttachmentCapabilities());
  CHECK(selection.transport == DepthHandoffTransport::kBuffer);
  CHECK(selection.reject_reason == DepthHandoffAttachmentRejectReason::kNotRequested);
}

TEST_CASE("GTA IV depth handoff attachment requires every capability") {
  const DepthHandoffTransportSelection accepted =
      SelectDepthHandoffTransport(true, FullAttachmentCapabilities());
  REQUIRE(accepted.transport == DepthHandoffTransport::kAttachment);
  CHECK(accepted.reject_reason == DepthHandoffAttachmentRejectReason::kNone);

  using RejectReason = DepthHandoffAttachmentRejectReason;
  using CapabilityMember = bool DepthHandoffAttachmentCapabilities::*;
  const auto predicates = std::to_array<std::pair<CapabilityMember, RejectReason>>({
      {&DepthHandoffAttachmentCapabilities::source_sampled, RejectReason::kSourceNotSampled},
      {&DepthHandoffAttachmentCapabilities::descriptor_objects_available,
       RejectReason::kDescriptorObjectsUnavailable},
      {&DepthHandoffAttachmentCapabilities::destination_view_available,
       RejectReason::kDestinationViewUnavailable},
      {&DepthHandoffAttachmentCapabilities::source_single_sampled,
       RejectReason::kSourceNotSingleSampled},
      {&DepthHandoffAttachmentCapabilities::destination_single_sampled,
       RejectReason::kDestinationNotSingleSampled},
      {&DepthHandoffAttachmentCapabilities::matching_depth_stencil_aspects,
       RejectReason::kAspectMismatch},
      {&DepthHandoffAttachmentCapabilities::matching_format, RejectReason::kFormatMismatch},
      {&DepthHandoffAttachmentCapabilities::matching_extent, RejectReason::kExtentMismatch},
      {&DepthHandoffAttachmentCapabilities::pipeline_available, RejectReason::kPipelineUnavailable},
  });

  for (const auto& [member, expected_reason] : predicates) {
    DepthHandoffAttachmentCapabilities capabilities = FullAttachmentCapabilities();
    capabilities.*member = false;
    const DepthHandoffTransportSelection rejected = SelectDepthHandoffTransport(true, capabilities);
    CHECK(rejected.transport == DepthHandoffTransport::kBuffer);
    CHECK(rejected.reject_reason == expected_reason);
  }
}

TEST_CASE("GTA IV depth handoff rejection names are stable") {
  CHECK(std::string_view(DepthHandoffAttachmentRejectReasonName(
            DepthHandoffAttachmentRejectReason::kNone)) == "none");
  CHECK(std::string_view(DepthHandoffAttachmentRejectReasonName(
            DepthHandoffAttachmentRejectReason::kPipelineUnavailable)) == "pipeline-unavailable");
}

TEST_CASE("GTA IV depth image layouts map to precise synchronization scopes") {
  const DepthImageAccessScope undefined = GetDepthImageAccessScope(VK_IMAGE_LAYOUT_UNDEFINED);
  CHECK(undefined.stages == VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
  CHECK(undefined.access == 0);

  const DepthImageAccessScope attachment =
      GetDepthImageAccessScope(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
  CHECK(attachment.stages ==
        (VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT));
  CHECK(attachment.access == (VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                              VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT));

  const DepthImageAccessScope sampled =
      GetDepthImageAccessScope(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
  CHECK(sampled.stages ==
        (VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT));
  CHECK(sampled.access == VK_ACCESS_SHADER_READ_BIT);

  const DepthImageAccessScope transfer_source =
      GetDepthImageAccessScope(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
  CHECK(transfer_source.stages == VK_PIPELINE_STAGE_TRANSFER_BIT);
  CHECK(transfer_source.access == VK_ACCESS_TRANSFER_READ_BIT);

  const DepthImageAccessScope transfer_destination =
      GetDepthImageAccessScope(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  CHECK(transfer_destination.stages == VK_PIPELINE_STAGE_TRANSFER_BIT);
  CHECK(transfer_destination.access == VK_ACCESS_TRANSFER_WRITE_BIT);

  const DepthImageAccessScope fallback = GetDepthImageAccessScope(VK_IMAGE_LAYOUT_GENERAL);
  CHECK(fallback.stages == VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);
  CHECK(fallback.access == (VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT));
}

}  // namespace
}  // namespace rex::graphics::gta4_native
