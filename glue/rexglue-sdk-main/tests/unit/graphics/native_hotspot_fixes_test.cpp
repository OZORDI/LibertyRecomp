#include <catch2/catch_test_macros.hpp>

#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <vector>

#include "graphics/gta4_native/native_attachment_policy.h"
#include "graphics/gta4_native/native_gpu_profile_policy.h"
#include "graphics/gta4_native/native_texture_upload_batch.h"

namespace rex::graphics::gta4_native {
namespace {

template <typename T> T Handle(uintptr_t value) { return reinterpret_cast<T>(value); }

struct Target {
  std::array<void*, 4> color_surfaces{};
  std::array<VkImageView, 4> color_views{};
  std::array<VkFormat, 4> color_formats{};
  void* depth_surface = nullptr;
  VkImageView depth_view = VK_NULL_HANDLE;
  VkFormat depth_format = VK_FORMAT_D32_SFLOAT_S8_UINT;
  VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
  VkSampleCountFlagBits guest_samples = VK_SAMPLE_COUNT_1_BIT;
  uint32_t width = 1280, height = 720, logical_width = 1280, logical_height = 720;
  uint32_t color_attachment_mask = 0, color_write_mask = 0;
  bool depth_stencil_attachment_active = true, uses_presenter = false, is_reflection = false;
};

Target LightingTarget() {
  Target target;
  target.color_surfaces[0] = Handle<void*>(1);
  target.color_views[0] = Handle<VkImageView>(2);
  target.color_formats[0] = VK_FORMAT_R16G16B16A16_SFLOAT;
  target.depth_surface = Handle<void*>(3);
  target.depth_view = Handle<VkImageView>(4);
  target.color_attachment_mask = 1;
  target.color_write_mask = 15;
  return target;
}

Target StencilTarget() {
  Target target;
  target.depth_surface = Handle<void*>(3);
  target.depth_view = Handle<VkImageView>(4);
  return target;
}

TEST_CASE("Light setup retains color without acquiring color write ownership",
          "[gta4-native][hotspot][attachments]") {
  auto active = LightingTarget();
  const auto before = active;
  auto setup = StencilTarget();
  REQUIRE_FALSE(NativeRenderingAttachmentsEqual(active, setup));
  REQUIRE(ReuseNativeLightingAttachments(active, setup));
  CHECK(NativeRenderingAttachmentsEqual(active, setup));
  CHECK(setup.color_write_mask == 0);
  CHECK(setup.depth_surface == Handle<void*>(3));
  CHECK(setup.color_surfaces == active.color_surfaces);
  CHECK(active.color_write_mask == before.color_write_mask);
  // The next contribution can continue in this same attachment set.
  CHECK(NativeRenderingAttachmentsEqual(setup, LightingTarget()));
}

TEST_CASE("Light pass reuse rejects incompatible or newly active attachments",
          "[gta4-native][hotspot][attachments]") {
  const std::vector<std::function<void(Target&)>> changes = {
      [](auto& t) { t.depth_view = Handle<VkImageView>(5); },
      [](auto& t) { t.depth_surface = Handle<void*>(6); },
      [](auto& t) { t.depth_format = VK_FORMAT_D24_UNORM_S8_UINT; },
      [](auto& t) { t.depth_stencil_attachment_active = false; },
      [](auto& t) { t.samples = VK_SAMPLE_COUNT_2_BIT; },
      [](auto& t) { t.guest_samples = VK_SAMPLE_COUNT_2_BIT; },
      [](auto& t) { t.width = 1920; }, [](auto& t) { t.height = 1080; },
      [](auto& t) { t.logical_width = 1920; }, [](auto& t) { t.logical_height = 1080; },
      [](auto& t) { t.uses_presenter = true; },
      [](auto& t) { t.is_reflection = true; },
      [](auto& t) { t.color_attachment_mask = 2; },
      [](auto& t) { t.color_write_mask = 15; },
  };
  for (const auto& change : changes) {
    auto setup = StencilTarget();
    change(setup);
    const auto before = setup;
    CHECK_FALSE(ReuseNativeLightingAttachments(LightingTarget(), setup));
    CHECK(NativeRenderingAttachmentsEqual(before, setup));
    CHECK(setup.color_surfaces == before.color_surfaces);
    CHECK(setup.color_write_mask == before.color_write_mask);
  }
  auto active = StencilTarget();
  auto setup = StencilTarget();
  CHECK_FALSE(ReuseNativeLightingAttachments(active, setup));
}

NativeTextureUploadBatch::Upload Upload(uintptr_t id) {
  NativeTextureUploadBatch::Upload upload;
  upload.image = Handle<VkImage>(id);
  upload.buffer = Handle<VkBuffer>(7);
  upload.range = {VK_IMAGE_ASPECT_COLOR_BIT, 2, 2, 1, 6};
  VkBufferImageCopy mip{};
  mip.bufferOffset = 256;
  mip.bufferRowLength = 32;
  mip.bufferImageHeight = 32;
  mip.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 2, 1, 6};
  mip.imageExtent = {32, 32, 1};
  upload.copies.push_back(mip);
  mip.bufferOffset = 24832;
  mip.bufferRowLength = 16;
  mip.bufferImageHeight = 16;
  mip.imageSubresource.mipLevel = 3;
  mip.imageExtent = {16, 16, 1};
  upload.copies.push_back(mip);
  return upload;
}

TEST_CASE("Texture batch preserves image ranges, copy payloads and consumer visibility",
          "[gta4-native][hotspot][uploads]") {
  NativeTextureUploadBatch batch;
  std::map<VkImage, VkImageLayout> layouts;
  std::vector<char> events;
  const auto barriers = [&](VkPipelineStageFlags source, VkPipelineStageFlags destination,
                            std::span<const VkImageMemoryBarrier> items) {
    REQUIRE(items.size() == 3);
    const bool opening = source == VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    if (opening) CHECK(destination == VK_PIPELINE_STAGE_TRANSFER_BIT);
    else {
      CHECK(source == VK_PIPELINE_STAGE_TRANSFER_BIT);
      CHECK(destination == (VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT));
    }
    for (const auto& b : items) {
      CHECK(b.sType == VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER);
      CHECK(b.srcQueueFamilyIndex == VK_QUEUE_FAMILY_IGNORED);
      CHECK(b.dstQueueFamilyIndex == VK_QUEUE_FAMILY_IGNORED);
      CHECK(b.subresourceRange.aspectMask == VK_IMAGE_ASPECT_COLOR_BIT);
      CHECK(b.subresourceRange.baseMipLevel == 2);
      CHECK(b.subresourceRange.levelCount == 2);
      CHECK(b.subresourceRange.baseArrayLayer == 1);
      CHECK(b.subresourceRange.layerCount == 6);
      CHECK(layouts[b.image] == b.oldLayout);
      CHECK(b.srcAccessMask == (opening ? 0u : VK_ACCESS_TRANSFER_WRITE_BIT));
      CHECK(b.dstAccessMask == (opening ? VK_ACCESS_TRANSFER_WRITE_BIT : VK_ACCESS_SHADER_READ_BIT));
      layouts[b.image] = b.newLayout;
    }
    events.push_back(opening ? 'B' : 'E');
  };
  const auto copy = [&](const NativeTextureUploadBatch::Upload& u) {
    CHECK(layouts[u.image] == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CHECK(u.buffer == Handle<VkBuffer>(7));
    REQUIRE(u.copies.size() == 2);
    CHECK(u.copies[0].bufferOffset == 256);
    CHECK(u.copies[1].bufferOffset == 24832);
    CHECK(u.copies[0].imageSubresource.baseArrayLayer == 1);
    CHECK(u.copies[1].imageSubresource.layerCount == 6);
    CHECK(u.copies[1].imageSubresource.mipLevel == 3);
    CHECK(u.copies[1].imageExtent.width == 16);
    CHECK(u.copies[1].bufferRowLength == 16);
    events.push_back('C');
  };
  for (uintptr_t id : {10, 11, 12}) batch.Add(Upload(id));
  CHECK(events.empty());
  batch.Flush(barriers, copy);
  CHECK(events == std::vector<char>{'B', 'C', 'C', 'C', 'E'});
  CHECK(batch.empty());
  for (const auto& [image, layout] : layouts) CHECK(layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
  batch.Flush(barriers, copy);
  CHECK(events.size() == 5);
}

TEST_CASE("Repeated upload batches stay bounded and release owned copy metadata",
          "[gta4-native][hotspot][uploads][lifetime]") {
  size_t barriers = 0, copies = 0;
  for (unsigned frame = 0; frame < 100; ++frame) {
    NativeTextureUploadBatch batch;
    const auto barrier = [&](auto, auto, auto items) {
      REQUIRE(items.size() <= NativeTextureUploadBatch::kCapacity);
      ++barriers;
    };
    const auto copy = [&](const auto&) { ++copies; };
    for (uintptr_t image = 1; image <= 257; ++image) {
      if (batch.full()) batch.Flush(barrier, copy);
      batch.Add(Upload(image));
    }
    batch.Flush(barrier, copy);
    CHECK(batch.empty());
  }
  CHECK(copies == 25700);
  CHECK(barriers == 600);
}

TEST_CASE("MoltenVK detailed timing requires explicit overhead opt-in",
          "[gta4-native][hotspot][profiling]") {
  CHECK_FALSE(NativeDetailedGpuTimingEnabled(true, true, false));
  CHECK(NativeDetailedGpuTimingEnabled(true, true, true));
  CHECK(NativeDetailedGpuTimingEnabled(true, false, false));
  CHECK(NativeDetailedGpuTimingEnabled(true, false, true));
  CHECK_FALSE(NativeDetailedGpuTimingEnabled(false, true, true));
  CHECK_FALSE(NativeDetailedGpuTimingEnabled(false, false, true));
}

}  // namespace
}  // namespace rex::graphics::gta4_native
