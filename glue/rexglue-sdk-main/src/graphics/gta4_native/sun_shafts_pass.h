#pragma once

#include <array>
#include <vector>

#include <rex/graphics/gta4_native/environmental_data.h>
#include <rex/ui/vulkan/api.h>

#include "postfx_resource_pool.h"

namespace rex::ui::vulkan {
class VulkanDevice;
}

namespace rex::graphics::gta4_native {

struct SunShaftParameters {
  std::array<float, 2> screen_position{};
  std::array<float, 3> sun_color{};
  float intensity = 0.0f;
  float density = 0.0f;
  float decay = 0.0f;
  float horizon_fade = 0.0f;
  bool valid = false;
};

SunShaftParameters BuildSunShaftParameters(const EnvironmentalDataV1* environmental_data);

class SunShaftsPass {
 public:
  bool Record(VkCommandBuffer command_buffer, const ui::vulkan::VulkanDevice* device,
              VkDescriptorPool descriptor_pool, VkPipelineCache pipeline_cache,
              VkImage destination_image, VkImageView destination_view, VkImageView depth_view,
              VkFormat color_format, PostFxExtent extent, const SunShaftParameters& parameters,
              PostFxResourcePool& resources);
  void Destroy(const ui::vulkan::VulkanDevice* device);

 private:
  struct Pipeline {
    VkFormat format = VK_FORMAT_UNDEFINED;
    VkPipeline pipeline = VK_NULL_HANDLE;
  };

  bool EnsureObjects(const ui::vulkan::VulkanDevice* device);
  VkPipeline GetOrCreatePipeline(const ui::vulkan::VulkanDevice* device,
                                 VkPipelineCache pipeline_cache, VkFormat format);
  bool RecordPass(VkCommandBuffer command_buffer, const ui::vulkan::VulkanDevice* device,
                  VkDescriptorPool descriptor_pool, VkPipeline pipeline,
                  const std::array<VkImageView, 3>& inputs,
                  PostFxResourcePool::Image& destination, uint32_t pass_index,
                  PostFxExtent source_extent, const SunShaftParameters& parameters);

  VkSampler sampler_ = VK_NULL_HANDLE;
  VkDescriptorSetLayout descriptor_set_layout_ = VK_NULL_HANDLE;
  VkPipelineLayout pipeline_layout_ = VK_NULL_HANDLE;
  std::vector<Pipeline> pipelines_;
};

}  // namespace rex::graphics::gta4_native
