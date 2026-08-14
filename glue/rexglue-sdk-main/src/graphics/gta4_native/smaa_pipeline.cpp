#include "smaa_pipeline.h"

#include <algorithm>
#include <array>
#include <cstring>

#include <rex/ui/vulkan/device.h>
#include <rex/ui/vulkan/instance.h>
#include <rex/ui/vulkan/util.h>

#include <postfx/smaa/AreaTex.h>
#include <postfx/smaa/SearchTex.h>

#include "../shaders/vulkan_spirv/fullscreen_cw_vs.h"

namespace rex::graphics::gta4_native {

namespace {

#include "smaa/smaa_shaders.inc"

constexpr VkFormat kEdgesFormat = VK_FORMAT_R8G8_UNORM;
constexpr VkFormat kWeightsFormat = VK_FORMAT_R8G8B8A8_UNORM;
constexpr VkFormat kOutputFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
constexpr VkFormat kAreaFormat = VK_FORMAT_R8G8_UNORM;
constexpr VkFormat kSearchFormat = VK_FORMAT_R8_UNORM;

struct SmaaConstants {
  float inverse_width;
  float inverse_height;
  float width;
  float height;
};

static_assert(sizeof(SmaaConstants) == 16);
static_assert(sizeof(areaTexBytes) == AREATEX_SIZE);
static_assert(sizeof(searchTexBytes) == SEARCHTEX_SIZE);

void DestroyImage(const ui::vulkan::VulkanDevice* device, SmaaPipeline::Image& image) {
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

bool CreateImage(const ui::vulkan::VulkanDevice* device, VkFormat format, PostFxExtent extent,
                 VkImageUsageFlags usage, SmaaPipeline::Image& image) {
  VkImageCreateInfo image_info{};
  image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  image_info.imageType = VK_IMAGE_TYPE_2D;
  image_info.format = format;
  image_info.extent = {extent.width, extent.height, 1};
  image_info.mipLevels = 1;
  image_info.arrayLayers = 1;
  image_info.samples = VK_SAMPLE_COUNT_1_BIT;
  image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
  image_info.usage = usage;
  image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  if (!ui::vulkan::util::CreateDedicatedAllocationImage(
          device, image_info, ui::vulkan::util::MemoryPurpose::kDeviceLocal, image.image,
          image.memory)) {
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

VkPipeline CreateFullscreenPipeline(const ui::vulkan::VulkanDevice* device,
                                    VkPipelineCache pipeline_cache, VkPipelineLayout layout,
                                    VkFormat format, const uint32_t* fragment_code,
                                    size_t fragment_size) {
  const auto& dfn = device->functions();
  const VkDevice vk_device = device->device();
  VkShaderModule vertex = ui::vulkan::util::CreateShaderModule(
      device, fullscreen_cw_vs, sizeof(fullscreen_cw_vs));
  VkShaderModule fragment =
      ui::vulkan::util::CreateShaderModule(device, fragment_code, fragment_size);
  if (!vertex || !fragment) {
    if (vertex) {
      dfn.vkDestroyShaderModule(vk_device, vertex, nullptr);
    }
    if (fragment) {
      dfn.vkDestroyShaderModule(vk_device, fragment, nullptr);
    }
    return VK_NULL_HANDLE;
  }

  std::array<VkPipelineShaderStageCreateInfo, 2> stages{};
  stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
  stages[0].module = vertex;
  stages[0].pName = "main";
  stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  stages[1].module = fragment;
  stages[1].pName = "main";
  VkPipelineVertexInputStateCreateInfo vertex_input{};
  vertex_input.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  VkPipelineInputAssemblyStateCreateInfo input_assembly{};
  input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  VkPipelineViewportStateCreateInfo viewport_state{};
  viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state.viewportCount = 1;
  viewport_state.scissorCount = 1;
  VkPipelineRasterizationStateCreateInfo rasterization{};
  rasterization.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterization.polygonMode = VK_POLYGON_MODE_FILL;
  rasterization.cullMode = VK_CULL_MODE_NONE;
  rasterization.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterization.lineWidth = 1.0f;
  VkPipelineMultisampleStateCreateInfo multisample{};
  multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  VkPipelineColorBlendAttachmentState color_attachment{};
  color_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                    VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  VkPipelineColorBlendStateCreateInfo color_blend{};
  color_blend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blend.attachmentCount = 1;
  color_blend.pAttachments = &color_attachment;
  const std::array<VkDynamicState, 2> dynamic_states = {VK_DYNAMIC_STATE_VIEWPORT,
                                                        VK_DYNAMIC_STATE_SCISSOR};
  VkPipelineDynamicStateCreateInfo dynamic_state{};
  dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamic_state.dynamicStateCount = uint32_t(dynamic_states.size());
  dynamic_state.pDynamicStates = dynamic_states.data();
  VkPipelineRenderingCreateInfo rendering_info{};
  rendering_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
  rendering_info.colorAttachmentCount = 1;
  rendering_info.pColorAttachmentFormats = &format;
  VkGraphicsPipelineCreateInfo pipeline_info{};
  pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_info.pNext = &rendering_info;
  pipeline_info.stageCount = uint32_t(stages.size());
  pipeline_info.pStages = stages.data();
  pipeline_info.pVertexInputState = &vertex_input;
  pipeline_info.pInputAssemblyState = &input_assembly;
  pipeline_info.pViewportState = &viewport_state;
  pipeline_info.pRasterizationState = &rasterization;
  pipeline_info.pMultisampleState = &multisample;
  pipeline_info.pColorBlendState = &color_blend;
  pipeline_info.pDynamicState = &dynamic_state;
  pipeline_info.layout = layout;
  VkPipeline pipeline = VK_NULL_HANDLE;
  const VkResult result = dfn.vkCreateGraphicsPipelines(
      vk_device, pipeline_cache, 1, &pipeline_info, nullptr, &pipeline);
  dfn.vkDestroyShaderModule(vk_device, fragment, nullptr);
  dfn.vkDestroyShaderModule(vk_device, vertex, nullptr);
  return result == VK_SUCCESS ? pipeline : VK_NULL_HANDLE;
}

}  // namespace

SmaaQuality ParseSmaaQuality(std::string_view quality) {
  if (quality == "low") {
    return SmaaQuality::kLow;
  }
  if (quality == "medium") {
    return SmaaQuality::kMedium;
  }
  if (quality == "ultra") {
    return SmaaQuality::kUltra;
  }
  return SmaaQuality::kHigh;
}

bool SmaaPipeline::EnsureStaticResources(const ui::vulkan::VulkanDevice* device,
                                         VkPipelineCache pipeline_cache) {
  if (neighborhood_pipeline_) {
    return true;
  }
  if (!device) {
    return false;
  }
  const auto& dfn = device->functions();
  const VkDevice vk_device = device->device();

  const auto supports = [&](VkFormat format, VkFormatFeatureFlags required) {
    VkFormatProperties properties{};
    device->vulkan_instance()->functions().vkGetPhysicalDeviceFormatProperties(
        device->physical_device(), format, &properties);
    return (properties.optimalTilingFeatures & required) == required;
  };
  constexpr VkFormatFeatureFlags kIntermediateFeatures =
      VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
  constexpr VkFormatFeatureFlags kLookupFeatures =
      VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
  if (!supports(kEdgesFormat, kIntermediateFeatures) ||
      !supports(kWeightsFormat, kIntermediateFeatures) ||
      !supports(kOutputFormat, kIntermediateFeatures) ||
      !supports(kAreaFormat, kLookupFeatures) || !supports(kSearchFormat, kLookupFeatures)) {
    return false;
  }

  VkSamplerCreateInfo sampler_info{};
  sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  sampler_info.minLod = 0.0f;
  sampler_info.maxLod = 0.0f;
  sampler_info.minFilter = VK_FILTER_LINEAR;
  sampler_info.magFilter = VK_FILTER_LINEAR;
  sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
  if (dfn.vkCreateSampler(vk_device, &sampler_info, nullptr, &linear_sampler_) != VK_SUCCESS) {
    return false;
  }
  sampler_info.minFilter = VK_FILTER_NEAREST;
  sampler_info.magFilter = VK_FILTER_NEAREST;
  if (dfn.vkCreateSampler(vk_device, &sampler_info, nullptr, &point_sampler_) != VK_SUCCESS) {
    Destroy(device);
    return false;
  }

  const std::array<std::array<VkSampler, 3>, kDescriptorSetCount> immutable_samplers = {{
      {point_sampler_, VK_NULL_HANDLE, VK_NULL_HANDLE},
      {linear_sampler_, linear_sampler_, point_sampler_},
      {linear_sampler_, linear_sampler_, VK_NULL_HANDLE},
  }};
  const std::array<uint32_t, kDescriptorSetCount> binding_counts = {1, 3, 2};
  for (uint32_t pass = 0; pass < kDescriptorSetCount; ++pass) {
    std::array<VkDescriptorSetLayoutBinding, 3> bindings{};
    for (uint32_t binding = 0; binding < binding_counts[pass]; ++binding) {
      bindings[binding].binding = binding;
      bindings[binding].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      bindings[binding].descriptorCount = 1;
      bindings[binding].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
      bindings[binding].pImmutableSamplers = &immutable_samplers[pass][binding];
    }
    VkDescriptorSetLayoutCreateInfo layout_info{};
    layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = binding_counts[pass];
    layout_info.pBindings = bindings.data();
    if (dfn.vkCreateDescriptorSetLayout(vk_device, &layout_info, nullptr,
                                        &descriptor_set_layouts_[pass]) != VK_SUCCESS) {
      Destroy(device);
      return false;
    }
    VkPushConstantRange push_range{};
    push_range.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    push_range.size = sizeof(SmaaConstants);
    VkPipelineLayoutCreateInfo pipeline_layout_info{};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_info.setLayoutCount = 1;
    pipeline_layout_info.pSetLayouts = &descriptor_set_layouts_[pass];
    pipeline_layout_info.pushConstantRangeCount = 1;
    pipeline_layout_info.pPushConstantRanges = &push_range;
    if (dfn.vkCreatePipelineLayout(vk_device, &pipeline_layout_info, nullptr,
                                   &pipeline_layouts_[pass]) != VK_SUCCESS) {
      Destroy(device);
      return false;
    }
  }

  const std::array<const uint32_t*, size_t(SmaaQuality::kCount)> edge_code = {
      smaa_edge_low_ps, smaa_edge_medium_ps, smaa_edge_high_ps, smaa_edge_ultra_ps};
  const std::array<size_t, size_t(SmaaQuality::kCount)> edge_size = {
      sizeof(smaa_edge_low_ps), sizeof(smaa_edge_medium_ps), sizeof(smaa_edge_high_ps),
      sizeof(smaa_edge_ultra_ps)};
  const std::array<const uint32_t*, size_t(SmaaQuality::kCount)> weight_code = {
      smaa_weight_low_ps, smaa_weight_medium_ps, smaa_weight_high_ps, smaa_weight_ultra_ps};
  const std::array<size_t, size_t(SmaaQuality::kCount)> weight_size = {
      sizeof(smaa_weight_low_ps), sizeof(smaa_weight_medium_ps), sizeof(smaa_weight_high_ps),
      sizeof(smaa_weight_ultra_ps)};
  for (size_t quality = 0; quality < size_t(SmaaQuality::kCount); ++quality) {
    edge_pipelines_[quality] = CreateFullscreenPipeline(
        device, pipeline_cache, pipeline_layouts_[0], kEdgesFormat, edge_code[quality],
        edge_size[quality]);
    weight_pipelines_[quality] = CreateFullscreenPipeline(
        device, pipeline_cache, pipeline_layouts_[1], kWeightsFormat, weight_code[quality],
        weight_size[quality]);
    if (!edge_pipelines_[quality] || !weight_pipelines_[quality]) {
      Destroy(device);
      return false;
    }
  }
  neighborhood_pipeline_ = CreateFullscreenPipeline(
      device, pipeline_cache, pipeline_layouts_[2], kOutputFormat, smaa_neighborhood_ps,
      sizeof(smaa_neighborhood_ps));
  if (!neighborhood_pipeline_) {
    Destroy(device);
    return false;
  }

  if (!CreateImage(device, kAreaFormat, {AREATEX_WIDTH, AREATEX_HEIGHT},
                   VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, area_) ||
      !CreateImage(device, kSearchFormat, {SEARCHTEX_WIDTH, SEARCHTEX_HEIGHT},
                   VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, search_)) {
    Destroy(device);
    return false;
  }
  const auto create_staging = [&](const void* bytes, VkDeviceSize size,
                                  StagingBuffer& staging) {
    uint32_t memory_type = 0;
    if (!ui::vulkan::util::CreateDedicatedAllocationBuffer(
            device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            ui::vulkan::util::MemoryPurpose::kUpload, staging.buffer, staging.memory,
            &memory_type)) {
      return false;
    }
    void* mapping = nullptr;
    if (dfn.vkMapMemory(vk_device, staging.memory, 0, VK_WHOLE_SIZE, 0, &mapping) != VK_SUCCESS) {
      return false;
    }
    std::memcpy(mapping, bytes, size_t(size));
    ui::vulkan::util::FlushMappedMemoryRange(device, staging.memory, memory_type);
    dfn.vkUnmapMemory(vk_device, staging.memory);
    return true;
  };
  if (!create_staging(areaTexBytes, sizeof(areaTexBytes), area_staging_) ||
      !create_staging(searchTexBytes, sizeof(searchTexBytes), search_staging_)) {
    Destroy(device);
    return false;
  }
  return true;
}

bool SmaaPipeline::EnsureExtentResources(const ui::vulkan::VulkanDevice* device,
                                         PostFxExtent extent) {
  if (edges_.image && edges_.extent == extent) {
    return true;
  }
  DestroyExtentResources(device);
  constexpr VkImageUsageFlags kIntermediateUsage =
      VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  if (!CreateImage(device, kEdgesFormat, extent, kIntermediateUsage, edges_) ||
      !CreateImage(device, kWeightsFormat, extent, kIntermediateUsage, weights_) ||
      !CreateImage(device, kOutputFormat, extent, kIntermediateUsage, output_)) {
    DestroyExtentResources(device);
    return false;
  }
  return true;
}

bool SmaaPipeline::RecordLookupUpload(VkCommandBuffer command_buffer,
                                      const ui::vulkan::VulkanDevice* device) {
  if (lookup_upload_recorded_) {
    return true;
  }
  const auto& dfn = device->functions();
  std::array<VkImageMemoryBarrier, 2> barriers{};
  const std::array<Image*, 2> images = {&area_, &search_};
  for (size_t index = 0; index < images.size(); ++index) {
    barriers[index].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barriers[index].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barriers[index].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barriers[index].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barriers[index].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barriers[index].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barriers[index].image = images[index]->image;
    barriers[index].subresourceRange =
        ui::vulkan::util::InitializeSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
  }
  dfn.vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                           VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr,
                           uint32_t(barriers.size()), barriers.data());

  const std::array<StagingBuffer*, 2> staging = {&area_staging_, &search_staging_};
  for (size_t index = 0; index < images.size(); ++index) {
    VkBufferImageCopy copy{};
    copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copy.imageSubresource.layerCount = 1;
    copy.imageExtent = {images[index]->extent.width, images[index]->extent.height, 1};
    dfn.vkCmdCopyBufferToImage(command_buffer, staging[index]->buffer, images[index]->image,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);
    barriers[index].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barriers[index].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barriers[index].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barriers[index].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    images[index]->layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  }
  dfn.vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                           VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr,
                           uint32_t(barriers.size()), barriers.data());
  lookup_upload_recorded_ = true;
  return true;
}

bool SmaaPipeline::Record(VkCommandBuffer command_buffer, const ui::vulkan::VulkanDevice* device,
                          VkDescriptorPool frame_descriptor_pool, VkPipelineCache pipeline_cache,
                          VkImage source_image, VkImageView source_view,
                          VkImageLayout& source_layout, PostFxExtent extent, SmaaQuality quality,
                          Output& result) {
  result = {};
  if (!command_buffer || !device || !frame_descriptor_pool || !source_image || !source_view ||
      !extent.width || !extent.height || quality >= SmaaQuality::kCount ||
      !EnsureStaticResources(device, pipeline_cache) || !EnsureExtentResources(device, extent)) {
    return false;
  }
  const auto& dfn = device->functions();
  const VkDevice vk_device = device->device();

  VkImageMemoryBarrier source_barrier{};
  source_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  source_barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
  source_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  source_barrier.oldLayout = source_layout;
  source_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  source_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  source_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  source_barrier.image = source_image;
  source_barrier.subresourceRange =
      ui::vulkan::util::InitializeSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
  dfn.vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                           VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1,
                           &source_barrier);
  source_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

  std::array<VkDescriptorSet, kDescriptorSetCount> sets{};
  VkDescriptorSetAllocateInfo allocation{};
  allocation.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocation.descriptorPool = frame_descriptor_pool;
  allocation.descriptorSetCount = kDescriptorSetCount;
  allocation.pSetLayouts = descriptor_set_layouts_.data();
  if (dfn.vkAllocateDescriptorSets(vk_device, &allocation, sets.data()) != VK_SUCCESS) {
    return false;
  }
  if (!RecordLookupUpload(command_buffer, device)) {
    return false;
  }

  const std::array<VkImageView, kCombinedImageSamplerDescriptorCount> views = {
      source_view, edges_.view, area_.view, search_.view, source_view, weights_.view};
  const std::array<VkImageLayout, kCombinedImageSamplerDescriptorCount> layouts = {
      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
  const std::array<uint32_t, kCombinedImageSamplerDescriptorCount> set_indices = {0, 1, 1, 1, 2,
                                                                                  2};
  const std::array<uint32_t, kCombinedImageSamplerDescriptorCount> bindings = {0, 0, 1, 2, 0, 1};
  std::array<VkDescriptorImageInfo, kCombinedImageSamplerDescriptorCount> images{};
  std::array<VkWriteDescriptorSet, kCombinedImageSamplerDescriptorCount> writes{};
  for (size_t index = 0; index < writes.size(); ++index) {
    images[index].imageView = views[index];
    images[index].imageLayout = layouts[index];
    writes[index].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[index].dstSet = sets[set_indices[index]];
    writes[index].dstBinding = bindings[index];
    writes[index].descriptorCount = 1;
    writes[index].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writes[index].pImageInfo = &images[index];
  }
  dfn.vkUpdateDescriptorSets(vk_device, uint32_t(writes.size()), writes.data(), 0, nullptr);

  const SmaaConstants constants = {1.0f / float(extent.width), 1.0f / float(extent.height),
                                   float(extent.width), float(extent.height)};
  const auto record_pass = [&](Image& destination, VkPipeline pipeline, VkPipelineLayout layout,
                               VkDescriptorSet descriptor_set) {
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.srcAccessMask = destination.layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                                ? VK_ACCESS_SHADER_READ_BIT
                                : 0;
    barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    barrier.oldLayout = destination.layout;
    barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = destination.image;
    barrier.subresourceRange =
        ui::vulkan::util::InitializeSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
    dfn.vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                             VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0,
                             nullptr, 1, &barrier);
    VkRenderingAttachmentInfo attachment{};
    attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    attachment.imageView = destination.view;
    attachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    VkRenderingInfo rendering{};
    rendering.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    rendering.renderArea.extent = {extent.width, extent.height};
    rendering.layerCount = 1;
    rendering.colorAttachmentCount = 1;
    rendering.pColorAttachments = &attachment;
    dfn.vkCmdBeginRendering(command_buffer, &rendering);
    VkViewport viewport{};
    viewport.width = float(extent.width);
    viewport.height = float(extent.height);
    viewport.maxDepth = 1.0f;
    dfn.vkCmdSetViewport(command_buffer, 0, 1, &viewport);
    VkRect2D scissor{};
    scissor.extent = {extent.width, extent.height};
    dfn.vkCmdSetScissor(command_buffer, 0, 1, &scissor);
    dfn.vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    dfn.vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1,
                                &descriptor_set, 0, nullptr);
    dfn.vkCmdPushConstants(command_buffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                           sizeof(constants), &constants);
    dfn.vkCmdDraw(command_buffer, 3, 1, 0, 0);
    dfn.vkCmdEndRendering(command_buffer);
    barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    dfn.vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                             VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1,
                             &barrier);
    destination.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  };

  const size_t quality_index = size_t(quality);
  record_pass(edges_, edge_pipelines_[quality_index], pipeline_layouts_[0], sets[0]);
  record_pass(weights_, weight_pipelines_[quality_index], pipeline_layouts_[1], sets[1]);
  record_pass(output_, neighborhood_pipeline_, pipeline_layouts_[2], sets[2]);
  result = {output_.image, output_.view, output_.layout, output_.format, output_.extent};
  return true;
}

void SmaaPipeline::DestroyExtentResources(const ui::vulkan::VulkanDevice* device) {
  if (!device) {
    return;
  }
  DestroyImage(device, edges_);
  DestroyImage(device, weights_);
  DestroyImage(device, output_);
}

void SmaaPipeline::Destroy(const ui::vulkan::VulkanDevice* device) {
  if (!device) {
    return;
  }
  const auto& dfn = device->functions();
  const VkDevice vk_device = device->device();
  DestroyExtentResources(device);
  DestroyImage(device, area_);
  DestroyImage(device, search_);
  for (VkPipeline& pipeline : edge_pipelines_) {
    if (pipeline) {
      dfn.vkDestroyPipeline(vk_device, pipeline, nullptr);
    }
    pipeline = VK_NULL_HANDLE;
  }
  for (VkPipeline& pipeline : weight_pipelines_) {
    if (pipeline) {
      dfn.vkDestroyPipeline(vk_device, pipeline, nullptr);
    }
    pipeline = VK_NULL_HANDLE;
  }
  if (neighborhood_pipeline_) {
    dfn.vkDestroyPipeline(vk_device, neighborhood_pipeline_, nullptr);
    neighborhood_pipeline_ = VK_NULL_HANDLE;
  }
  for (VkPipelineLayout& layout : pipeline_layouts_) {
    if (layout) {
      dfn.vkDestroyPipelineLayout(vk_device, layout, nullptr);
    }
    layout = VK_NULL_HANDLE;
  }
  for (VkDescriptorSetLayout& layout : descriptor_set_layouts_) {
    if (layout) {
      dfn.vkDestroyDescriptorSetLayout(vk_device, layout, nullptr);
    }
    layout = VK_NULL_HANDLE;
  }
  if (linear_sampler_) {
    dfn.vkDestroySampler(vk_device, linear_sampler_, nullptr);
    linear_sampler_ = VK_NULL_HANDLE;
  }
  if (point_sampler_) {
    dfn.vkDestroySampler(vk_device, point_sampler_, nullptr);
    point_sampler_ = VK_NULL_HANDLE;
  }
  for (StagingBuffer* staging : {&area_staging_, &search_staging_}) {
    if (staging->buffer) {
      dfn.vkDestroyBuffer(vk_device, staging->buffer, nullptr);
    }
    if (staging->memory) {
      dfn.vkFreeMemory(vk_device, staging->memory, nullptr);
    }
    *staging = {};
  }
  lookup_upload_recorded_ = false;
}

}  // namespace rex::graphics::gta4_native
