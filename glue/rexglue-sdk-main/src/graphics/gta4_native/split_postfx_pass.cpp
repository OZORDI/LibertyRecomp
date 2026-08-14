#include "split_postfx_pass.h"

#include <cstring>

#include <rex/ui/vulkan/device.h>
#include <rex/ui/vulkan/util.h>

#include "../shaders/vulkan_spirv/fullscreen_cw_vs.h"
#include "split_postfx_ps.h"

namespace rex::graphics::gta4_native {
namespace {

struct SplitPostFxPushConstants {
  int32_t source_extent[2];
  int32_t destination_extent[2];
  uint32_t pass_index;
  uint32_t depth_source;
  uint32_t reserved[2];
  float dof_projection[4];
  float dof_distance[4];
  float dof_blur[4];
};

static_assert(sizeof(SplitPostFxPushConstants) == 80);

}  // namespace

bool SplitPostFxPass::EnsureObjects(const ui::vulkan::VulkanDevice* device) {
  if (!device) {
    return false;
  }
  if (pipeline_layout_) {
    return true;
  }
  const auto& dfn = device->functions();
  const VkDevice vk_device = device->device();

  VkSamplerCreateInfo sampler_info{};
  sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  sampler_info.magFilter = VK_FILTER_LINEAR;
  sampler_info.minFilter = VK_FILTER_LINEAR;
  sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
  sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  sampler_info.maxLod = 0.0f;
  if (dfn.vkCreateSampler(vk_device, &sampler_info, nullptr, &sampler_) != VK_SUCCESS) {
    return false;
  }

  std::array<VkDescriptorSetLayoutBinding, 4> bindings{};
  for (uint32_t index = 0; index < bindings.size(); ++index) {
    bindings[index].binding = index;
    bindings[index].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[index].descriptorCount = 1;
    bindings[index].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    bindings[index].pImmutableSamplers = &sampler_;
  }
  VkDescriptorSetLayoutCreateInfo layout_info{};
  layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layout_info.bindingCount = uint32_t(bindings.size());
  layout_info.pBindings = bindings.data();
  if (dfn.vkCreateDescriptorSetLayout(vk_device, &layout_info, nullptr, &descriptor_set_layout_) !=
      VK_SUCCESS) {
    Destroy(device);
    return false;
  }

  VkPushConstantRange push_range{};
  push_range.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  push_range.size = sizeof(SplitPostFxPushConstants);
  VkPipelineLayoutCreateInfo pipeline_layout_info{};
  pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipeline_layout_info.setLayoutCount = 1;
  pipeline_layout_info.pSetLayouts = &descriptor_set_layout_;
  pipeline_layout_info.pushConstantRangeCount = 1;
  pipeline_layout_info.pPushConstantRanges = &push_range;
  if (dfn.vkCreatePipelineLayout(vk_device, &pipeline_layout_info, nullptr, &pipeline_layout_) !=
      VK_SUCCESS) {
    Destroy(device);
    return false;
  }
  return true;
}

VkPipeline SplitPostFxPass::GetOrCreatePipeline(const ui::vulkan::VulkanDevice* device,
                                                VkPipelineCache pipeline_cache, VkFormat format) {
  for (const Pipeline& existing : pipelines_) {
    if (existing.format == format) {
      return existing.pipeline;
    }
  }
  if (!EnsureObjects(device)) {
    return VK_NULL_HANDLE;
  }
  const auto& dfn = device->functions();
  const VkDevice vk_device = device->device();
  VkShaderModule vertex_shader =
      ui::vulkan::util::CreateShaderModule(device, fullscreen_cw_vs, sizeof(fullscreen_cw_vs));
  VkShaderModule pixel_shader = ui::vulkan::util::CreateShaderModule(
      device, gta4_native_split_postfx_ps, sizeof(gta4_native_split_postfx_ps));
  if (!vertex_shader || !pixel_shader) {
    if (vertex_shader) {
      dfn.vkDestroyShaderModule(vk_device, vertex_shader, nullptr);
    }
    if (pixel_shader) {
      dfn.vkDestroyShaderModule(vk_device, pixel_shader, nullptr);
    }
    return VK_NULL_HANDLE;
  }

  std::array<VkPipelineShaderStageCreateInfo, 2> stages{};
  stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
  stages[0].module = vertex_shader;
  stages[0].pName = "main";
  stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  stages[1].module = pixel_shader;
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
  pipeline_info.layout = pipeline_layout_;
  VkPipeline pipeline = VK_NULL_HANDLE;
  const VkResult result = dfn.vkCreateGraphicsPipelines(vk_device, pipeline_cache, 1,
                                                        &pipeline_info, nullptr, &pipeline);
  dfn.vkDestroyShaderModule(vk_device, pixel_shader, nullptr);
  dfn.vkDestroyShaderModule(vk_device, vertex_shader, nullptr);
  if (result != VK_SUCCESS) {
    return VK_NULL_HANDLE;
  }
  pipelines_.push_back({format, pipeline});
  return pipeline;
}

bool SplitPostFxPass::RecordPass(VkCommandBuffer command_buffer,
                                 const ui::vulkan::VulkanDevice* device,
                                 VkDescriptorPool descriptor_pool, VkPipeline pipeline,
                                 const std::array<VkImageView, 4>& inputs,
                                 PostFxResourcePool::Image& destination, uint32_t pass_index,
                                 PostFxExtent source_extent,
                                 const SplitPostFxParameters& parameters) {
  if (!command_buffer || !descriptor_pool || !pipeline || !destination.view) {
    return false;
  }
  const auto& dfn = device->functions();
  const VkDevice vk_device = device->device();
  VkDescriptorSet descriptor_set = VK_NULL_HANDLE;
  VkDescriptorSetAllocateInfo allocate_info{};
  allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocate_info.descriptorPool = descriptor_pool;
  allocate_info.descriptorSetCount = 1;
  allocate_info.pSetLayouts = &descriptor_set_layout_;
  if (dfn.vkAllocateDescriptorSets(vk_device, &allocate_info, &descriptor_set) != VK_SUCCESS) {
    return false;
  }
  std::array<VkDescriptorImageInfo, 4> image_infos{};
  std::array<VkWriteDescriptorSet, 4> writes{};
  for (uint32_t index = 0; index < inputs.size(); ++index) {
    if (!inputs[index]) {
      return false;
    }
    image_infos[index].imageView = inputs[index];
    image_infos[index].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    writes[index].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[index].dstSet = descriptor_set;
    writes[index].dstBinding = index;
    writes[index].descriptorCount = 1;
    writes[index].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writes[index].pImageInfo = &image_infos[index];
  }
  dfn.vkUpdateDescriptorSets(vk_device, uint32_t(writes.size()), writes.data(), 0, nullptr);

  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.srcAccessMask =
      destination.layout == VK_IMAGE_LAYOUT_UNDEFINED ? 0 : VK_ACCESS_SHADER_READ_BIT;
  barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  barrier.oldLayout = destination.layout;
  barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = destination.image;
  barrier.subresourceRange =
      ui::vulkan::util::InitializeSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
  const VkPipelineStageFlags source_stage = destination.layout == VK_IMAGE_LAYOUT_UNDEFINED
                                                ? VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
                                                : VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  dfn.vkCmdPipelineBarrier(command_buffer, source_stage,
                           VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr,
                           1, &barrier);

  VkRenderingAttachmentInfo attachment{};
  attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
  attachment.imageView = destination.view;
  attachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  VkRenderingInfo rendering_info{};
  rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
  rendering_info.renderArea.extent = {destination.extent.width, destination.extent.height};
  rendering_info.layerCount = 1;
  rendering_info.colorAttachmentCount = 1;
  rendering_info.pColorAttachments = &attachment;
  dfn.vkCmdBeginRendering(command_buffer, &rendering_info);
  dfn.vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  dfn.vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout_, 0,
                              1, &descriptor_set, 0, nullptr);
  VkViewport viewport{};
  viewport.width = float(destination.extent.width);
  viewport.height = float(destination.extent.height);
  viewport.maxDepth = 1.0f;
  dfn.vkCmdSetViewport(command_buffer, 0, 1, &viewport);
  VkRect2D scissor{};
  scissor.extent = {destination.extent.width, destination.extent.height};
  dfn.vkCmdSetScissor(command_buffer, 0, 1, &scissor);
  SplitPostFxPushConstants push{};
  push.source_extent[0] = int32_t(source_extent.width);
  push.source_extent[1] = int32_t(source_extent.height);
  push.destination_extent[0] = int32_t(destination.extent.width);
  push.destination_extent[1] = int32_t(destination.extent.height);
  push.pass_index = pass_index;
  push.depth_source = uint32_t(parameters.depth_source);
  std::memcpy(push.dof_projection, parameters.dof_projection.data(), sizeof(push.dof_projection));
  std::memcpy(push.dof_distance, parameters.dof_distance.data(), sizeof(push.dof_distance));
  std::memcpy(push.dof_blur, parameters.dof_blur.data(), sizeof(push.dof_blur));
  dfn.vkCmdPushConstants(command_buffer, pipeline_layout_, VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                         sizeof(push), &push);
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
  return true;
}

bool SplitPostFxPass::Record(VkCommandBuffer command_buffer, const ui::vulkan::VulkanDevice* device,
                             VkDescriptorPool descriptor_pool, VkPipelineCache pipeline_cache,
                             VkImage destination_image, VkImageView destination_view,
                             VkImageView depth_view, VkImageView stipple_mask_view,
                             VkFormat color_format, PostFxExtent extent,
                             const SplitPostFxParameters& parameters,
                             PostFxResourcePool& resources) {
  if (!destination_image || !destination_view || !depth_view || !stipple_mask_view ||
      !resources.scene_snapshot().view ||
      !resources.EnsureSplitPostFxImages(device, color_format, extent)) {
    return false;
  }
  VkPipeline pipeline = GetOrCreatePipeline(device, pipeline_cache, color_format);
  if (!pipeline) {
    return false;
  }
  auto& full_ping = resources.split_full_ping();
  auto& half_ping = resources.split_half_ping();
  auto& half_pong = resources.split_half_pong();
  auto& full_output = resources.split_full_output();
  const VkImageView snapshot = resources.scene_snapshot().view;
  if (!RecordPass(command_buffer, device, descriptor_pool, pipeline,
                  {snapshot, snapshot, depth_view, stipple_mask_view}, full_ping, 0, extent,
                  parameters) ||
      !RecordPass(command_buffer, device, descriptor_pool, pipeline,
                  {full_ping.view, full_ping.view, depth_view, stipple_mask_view}, half_ping, 1,
                  extent, parameters) ||
      !RecordPass(command_buffer, device, descriptor_pool, pipeline,
                  {half_ping.view, half_ping.view, depth_view, stipple_mask_view}, half_pong, 2,
                  half_ping.extent, parameters) ||
      !RecordPass(command_buffer, device, descriptor_pool, pipeline,
                  {full_ping.view, half_pong.view, depth_view, stipple_mask_view}, full_output, 3,
                  extent, parameters)) {
    return false;
  }

  const auto& dfn = device->functions();
  std::array<VkImageMemoryBarrier, 2> barriers{};
  for (VkImageMemoryBarrier& barrier : barriers) {
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange =
        ui::vulkan::util::InitializeSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
  }
  barriers[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
  barriers[0].dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  barriers[0].oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  barriers[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  barriers[0].image = full_output.image;
  barriers[1].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
  barriers[1].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barriers[1].oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  barriers[1].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  barriers[1].image = destination_image;
  dfn.vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                           VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr,
                           uint32_t(barriers.size()), barriers.data());
  VkImageCopy copy{};
  copy.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  copy.srcSubresource.layerCount = 1;
  copy.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  copy.dstSubresource.layerCount = 1;
  copy.extent = {extent.width, extent.height, 1};
  dfn.vkCmdCopyImage(command_buffer, full_output.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                     destination_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);
  barriers[0].srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  barriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  barriers[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  barriers[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  barriers[1].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barriers[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  barriers[1].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  barriers[1].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  dfn.vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                           VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr,
                           uint32_t(barriers.size()), barriers.data());
  full_output.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  return true;
}

void SplitPostFxPass::Destroy(const ui::vulkan::VulkanDevice* device) {
  if (!device) {
    return;
  }
  const auto& dfn = device->functions();
  const VkDevice vk_device = device->device();
  for (const Pipeline& pipeline : pipelines_) {
    if (pipeline.pipeline) {
      dfn.vkDestroyPipeline(vk_device, pipeline.pipeline, nullptr);
    }
  }
  pipelines_.clear();
  if (pipeline_layout_) {
    dfn.vkDestroyPipelineLayout(vk_device, pipeline_layout_, nullptr);
  }
  if (descriptor_set_layout_) {
    dfn.vkDestroyDescriptorSetLayout(vk_device, descriptor_set_layout_, nullptr);
  }
  if (sampler_) {
    dfn.vkDestroySampler(vk_device, sampler_, nullptr);
  }
  pipeline_layout_ = VK_NULL_HANDLE;
  descriptor_set_layout_ = VK_NULL_HANDLE;
  sampler_ = VK_NULL_HANDLE;
}

}  // namespace rex::graphics::gta4_native
