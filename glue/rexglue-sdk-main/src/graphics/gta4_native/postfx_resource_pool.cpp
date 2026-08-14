#include "postfx_resource_pool.h"

#include <array>

#include <rex/ui/vulkan/device.h>
#include <rex/ui/vulkan/instance.h>
#include <rex/ui/vulkan/util.h>

namespace rex::graphics::gta4_native {

void PostFxResourcePool::DestroyImage(const ui::vulkan::VulkanDevice* device, Image& image) {
  if (!device) {
    return;
  }
  const auto& dfn = device->functions();
  const VkDevice vk_device = device->device();
  if (image.view) {
    dfn.vkDestroyImageView(vk_device, image.view, nullptr);
  }
  if (image.image) {
    dfn.vkDestroyImage(vk_device, image.image, nullptr);
  }
  if (image.memory) {
    dfn.vkFreeMemory(vk_device, image.memory, nullptr);
  }
  image = {};
}

bool PostFxResourcePool::EnsureImage(const ui::vulkan::VulkanDevice* device, VkFormat format,
                                     PostFxExtent extent, Image& image) {
  if (!device || format == VK_FORMAT_UNDEFINED || !extent.width || !extent.height) {
    return false;
  }
  if (image.image && image.format == format && image.extent == extent) {
    return true;
  }

  VkFormatProperties format_properties{};
  device->vulkan_instance()->functions().vkGetPhysicalDeviceFormatProperties(
      device->physical_device(), format, &format_properties);
  constexpr VkFormatFeatureFlags kRequiredFeatures =
      VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT |
      VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
  if ((format_properties.optimalTilingFeatures & kRequiredFeatures) != kRequiredFeatures) {
    return false;
  }
  DestroyImage(device, image);

  VkImageCreateInfo image_info{};
  image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  image_info.imageType = VK_IMAGE_TYPE_2D;
  image_info.format = format;
  image_info.extent = {extent.width, extent.height, 1};
  image_info.mipLevels = 1;
  image_info.arrayLayers = 1;
  image_info.samples = VK_SAMPLE_COUNT_1_BIT;
  image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
  image_info.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                     VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  if (!ui::vulkan::util::CreateDedicatedAllocationImage(
          device, image_info, ui::vulkan::util::MemoryPurpose::kDeviceLocal, image.image,
          image.memory)) {
    image = {};
    return false;
  }

  VkImageViewCreateInfo view_info{};
  view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  view_info.image = image.image;
  view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  view_info.format = format;
  view_info.subresourceRange =
      ui::vulkan::util::InitializeSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
  if (device->functions().vkCreateImageView(device->device(), &view_info, nullptr, &image.view) !=
      VK_SUCCESS) {
    DestroyImage(device, image);
    return false;
  }
  image.format = format;
  image.extent = extent;
  return true;
}

bool PostFxResourcePool::EnsureSceneSnapshot(const ui::vulkan::VulkanDevice* device,
                                             VkFormat format, PostFxExtent extent) {
  return EnsureImage(device, format, extent, scene_snapshot_);
}

bool PostFxResourcePool::EnsureSplitPostFxImages(const ui::vulkan::VulkanDevice* device,
                                                 VkFormat format, PostFxExtent extent) {
  const PostFxExtent half_extent = CalculatePostFxExtent(extent.width, extent.height, 2);
  return EnsureImage(device, format, extent, split_full_ping_) &&
         EnsureImage(device, format, half_extent, split_half_ping_) &&
         EnsureImage(device, format, half_extent, split_half_pong_) &&
         EnsureImage(device, format, extent, split_full_output_);
}

bool PostFxResourcePool::EnsureSunShaftImages(const ui::vulkan::VulkanDevice* device,
                                              VkFormat format, PostFxExtent extent) {
  const PostFxExtent half_extent = CalculatePostFxExtent(extent.width, extent.height, 2);
  return EnsureImage(device, format, half_extent, sun_half_prepass_) &&
         EnsureImage(device, format, half_extent, sun_half_ping_) &&
         EnsureImage(device, format, half_extent, sun_half_pong_) &&
         EnsureImage(device, format, extent, sun_full_output_);
}

bool PostFxResourcePool::RecordSceneSnapshot(VkCommandBuffer command_buffer,
                                             const ui::vulkan::VulkanDevice* device, VkImage source,
                                             VkFormat format, VkImageLayout source_layout,
                                             PostFxExtent extent) {
  if (!command_buffer || !source || source_layout != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ||
      !EnsureSceneSnapshot(device, format, extent)) {
    return false;
  }

  const auto& dfn = device->functions();
  std::array<VkImageMemoryBarrier, 2> barriers{};
  barriers[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barriers[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
  barriers[0].dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  barriers[0].oldLayout = source_layout;
  barriers[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  barriers[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barriers[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barriers[0].image = source;
  barriers[0].subresourceRange =
      ui::vulkan::util::InitializeSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);

  barriers[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barriers[1].srcAccessMask =
      scene_snapshot_.layout == VK_IMAGE_LAYOUT_UNDEFINED ? 0 : VK_ACCESS_SHADER_READ_BIT;
  barriers[1].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barriers[1].oldLayout = scene_snapshot_.layout;
  barriers[1].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  barriers[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barriers[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barriers[1].image = scene_snapshot_.image;
  barriers[1].subresourceRange = barriers[0].subresourceRange;
  constexpr VkPipelineStageFlags kShaderStages =
      VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  dfn.vkCmdPipelineBarrier(command_buffer, kShaderStages, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0,
                           nullptr, 0, nullptr, uint32_t(barriers.size()), barriers.data());

  VkImageCopy copy{};
  copy.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  copy.srcSubresource.layerCount = 1;
  copy.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  copy.dstSubresource.layerCount = 1;
  copy.extent = {extent.width, extent.height, 1};
  dfn.vkCmdCopyImage(command_buffer, source, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                     scene_snapshot_.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);

  barriers[0].srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  barriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  barriers[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  barriers[0].newLayout = source_layout;
  barriers[1].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barriers[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  barriers[1].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  barriers[1].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  dfn.vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, kShaderStages, 0, 0,
                           nullptr, 0, nullptr, uint32_t(barriers.size()), barriers.data());
  scene_snapshot_.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  return true;
}

void PostFxResourcePool::Destroy(const ui::vulkan::VulkanDevice* device) {
  DestroyImage(device, scene_snapshot_);
  DestroyImage(device, split_full_ping_);
  DestroyImage(device, split_half_ping_);
  DestroyImage(device, split_half_pong_);
  DestroyImage(device, split_full_output_);
  DestroyImage(device, sun_half_prepass_);
  DestroyImage(device, sun_half_ping_);
  DestroyImage(device, sun_half_pong_);
  DestroyImage(device, sun_full_output_);
}

}  // namespace rex::graphics::gta4_native
