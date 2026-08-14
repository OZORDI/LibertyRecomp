#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

#include <rex/ui/vulkan/api.h>

#include "postfx_resource_pool.h"

namespace rex::ui::vulkan {
class VulkanDevice;
}

namespace rex::graphics::gta4_native {

enum class SmaaQuality : uint32_t {
  kLow,
  kMedium,
  kHigh,
  kUltra,
  kCount,
};

SmaaQuality ParseSmaaQuality(std::string_view quality);

class SmaaPipeline {
 public:
  static constexpr uint32_t kDescriptorSetCount = 3;
  static constexpr uint32_t kCombinedImageSamplerDescriptorCount = 6;

  struct Output {
    VkImage image = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
    VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkFormat format = VK_FORMAT_UNDEFINED;
    PostFxExtent extent{};
  };

  // Public only so the translation-unit helpers can create and destroy the
  // tightly owned Vulkan allocations; callers interact through Output.
  struct Image {
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
    VkFormat format = VK_FORMAT_UNDEFINED;
    VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
    PostFxExtent extent{};
  };

  bool Record(VkCommandBuffer command_buffer, const ui::vulkan::VulkanDevice* device,
              VkDescriptorPool frame_descriptor_pool, VkPipelineCache pipeline_cache,
              VkImage source_image, VkImageView source_view, VkImageLayout& source_layout,
              PostFxExtent extent, SmaaQuality quality, Output& output);
  void Destroy(const ui::vulkan::VulkanDevice* device);

 private:
  struct StagingBuffer {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
  };

  bool EnsureStaticResources(const ui::vulkan::VulkanDevice* device,
                             VkPipelineCache pipeline_cache);
  bool EnsureExtentResources(const ui::vulkan::VulkanDevice* device, PostFxExtent extent);
  bool RecordLookupUpload(VkCommandBuffer command_buffer,
                          const ui::vulkan::VulkanDevice* device);
  void DestroyExtentResources(const ui::vulkan::VulkanDevice* device);

  Image edges_;
  Image weights_;
  Image output_;
  Image area_;
  Image search_;
  StagingBuffer area_staging_;
  StagingBuffer search_staging_;
  bool lookup_upload_recorded_ = false;

  VkSampler linear_sampler_ = VK_NULL_HANDLE;
  VkSampler point_sampler_ = VK_NULL_HANDLE;
  std::array<VkDescriptorSetLayout, kDescriptorSetCount> descriptor_set_layouts_{};
  std::array<VkPipelineLayout, kDescriptorSetCount> pipeline_layouts_{};
  std::array<VkPipeline, size_t(SmaaQuality::kCount)> edge_pipelines_{};
  std::array<VkPipeline, size_t(SmaaQuality::kCount)> weight_pipelines_{};
  VkPipeline neighborhood_pipeline_ = VK_NULL_HANDLE;
};

}  // namespace rex::graphics::gta4_native
