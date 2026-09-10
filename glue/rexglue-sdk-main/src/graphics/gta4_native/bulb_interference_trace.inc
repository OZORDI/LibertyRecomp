// Optional one-frame, full-image evidence for bulb/shade/postfx/presentation.
// All copies execute outside rendering, restore layouts, and publish only after
// their own frame-slot submission completes. No extra game draws or state edits.
bool Gta4NativeGraphicsSystem::NeedsBulbInterference(const NativeCommand& command,
                                                   const NativeRenderingTarget& target) const {
  if (!EmissionTraceConfig().pipeline_full_readback || !bulb_trace_frame_ || (bulb_routes_.empty() && !bulb_prospective_route_) ||
      (bulb_full_frame_ && bulb_full_frame_ != diagnostic_submitted_frame_) ||
      !command.pipeline_state || !command.pipeline_state->pixel_shader_resource ||
      target.is_reflection || !target.color_surfaces[0]) return false;
  const auto hash = command.pipeline_state->pixel_shader_resource->hash;
  if (IsBulbCoronaShader(hash)) return EmissionTraceConfig().corona_trace;
  if (bulb_routes_.empty()) return false;
  const auto& route = bulb_routes_.front();
  if (diagnostic_command_index_ < route.command_index) return false;
  // This focused mode records halo inputs/results plus the ordinary bulb/shade
  // and presentation. Existing sparse postfx ancestry remains available.
  if (EmissionTraceConfig().corona_trace &&
      command.pipeline_state->pixel_shader_resource->filename == "shader/rage_shaders/rage_postfx_e2.fxc") return false;
  if (!IsBulbInterferenceFamily(hash, command.pipeline_state->pixel_shader_resource->filename)) return false;
  if (hash == 0xF634FCEBA607E8A5ull || hash == 0x07675BF8EF5E48DCull)
    return command.diagnostic_submit_sequence == route.sequence ||
           (command.bulb_trace && kBulbFixtures[command.bulb_trace->fixture].role == "ceiling-shade");
  return true;
}

void Gta4NativeGraphicsSystem::RecordBulbFullImage(VkCommandBuffer cb, std::string_view role,
    const NativeCommand* command, NativeSurfaceImage* surface, NativeTextureImage* texture,
    VkImage direct, VkFormat direct_format, VkImageLayout direct_layout,
    uint32_t direct_width, uint32_t direct_height, VkImageAspectFlags requested_aspect) {
  if (!EmissionTraceConfig().pipeline_full_readback || !bulb_trace_frame_ ||
      (bulb_routes_.empty() && !bulb_prospective_route_) ||
      (bulb_full_frame_ && bulb_full_frame_ != diagnostic_submitted_frame_)) return;
  const auto skip = [&](std::string_view reason) {
    REXLOG_WARN("gta4-bulb-full: point=unavailable frame={} cmd={} role={} reason={}",
                diagnostic_submitted_frame_, diagnostic_command_index_, role, reason);
  };
  if (active_frame_slot_ >= bulb_full_probes_.size()) { skip("frame-slot"); return; }
  auto* provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const auto* device = provider ? provider->vulkan_device() : nullptr;
  if (!cb || !device) { skip("device"); return; }
  const VkImage image = surface ? surface->resource.image : texture ? texture->resource.image : direct;
  const VkFormat format = surface ? surface->format : texture ? texture->format : direct_format;
  const VkImageLayout layout = surface ? surface->layout : texture ? texture->layout : direct_layout;
  const uint32_t width = surface ? surface->width : texture ? texture->width : direct_width;
  const uint32_t height = surface ? surface->height : texture ? texture->height : direct_height;
  const auto samples = surface ? surface->samples : texture ? texture->samples : VK_SAMPLE_COUNT_1_BIT;
  const VkImageAspectFlags aspect = requested_aspect;
  if (!image || layout == VK_IMAGE_LAYOUT_UNDEFINED || samples != VK_SAMPLE_COUNT_1_BIT) {
    skip("undefined-or-multisample-image"); return;
  }
  if (texture && (!(texture->usage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) ||
      texture->view_type != VK_IMAGE_VIEW_TYPE_2D || texture->view_range.baseMipLevel ||
      texture->view_range.baseArrayLayer || texture->view_range.layerCount != 1)) {
    skip("unsupported-view-or-transfer-usage"); return;
  }
  uint32_t block = 1;
  uint32_t bpt = PhoneProbeTexelBytes(format, aspect);
  if (aspect == VK_IMAGE_ASPECT_COLOR_BIT) {
    if (format == VK_FORMAT_BC1_RGBA_UNORM_BLOCK) { bpt = 8; block = 4; }
    if (format == VK_FORMAT_BC2_UNORM_BLOCK || format == VK_FORMAT_BC3_UNORM_BLOCK) { bpt = 16; block = 4; }
  }
  const uint64_t bytes = BulbFullReadbackBytes(width, height, bpt, block);
  auto& probes = bulb_full_probes_[active_frame_slot_];
  const bool presentation = role == "present-input" || role == "present-output";
  constexpr uint64_t presentation_reserve = uint64_t{64} * 1024 * 1024;
  if (!BulbFullBudgetAccepts(bulb_full_bytes_, bytes, probes.size()) ||
      (!presentation && (bytes > kBulbFullReadbackBudget - presentation_reserve ||
       bulb_full_bytes_ > kBulbFullReadbackBudget - presentation_reserve - bytes ||
       probes.size() >= kBulbFullReadbackCount - 2))) {
    skip(bytes ? "bounded-capture-budget" : "unsupported-format-or-extent"); return;
  }
  const auto& route = bulb_routes_.empty() ? *bulb_prospective_route_ : bulb_routes_.front();
  NativeBulbFullProbe p;
  p.frame = diagnostic_submitted_frame_; p.command_index = uint32_t(diagnostic_command_index_);
  p.source_sequence = route.sequence; p.sequence = command ? command->diagnostic_submit_sequence : 0;
  p.run = bulb_run_; p.role = role; p.fixture = uint32_t(route.source->fixture);
  p.width = width; p.height = height; p.format = uint32_t(format); p.aspect = aspect;
  p.bytes = bytes; p.block_extent = block; p.bytes_per_block = bpt;
  p.image = NativeVulkanHandleIdentity(image); p.layout = uint32_t(layout);
  if (command && command->pipeline_state) {
    if (auto* ps = command->pipeline_state->pixel_shader_resource) p.pixel_shader = ps->hash;
    if (auto* vs = command->pipeline_state->vertex_shader_resource) p.vertex_shader = vs->hash;
  }
  if (surface) {
    p.lifetime = surface->lifetime_id; p.handle = surface->descriptor.handle;
    MarkNativeSurfaceImageUsed(*surface);
  } else if (texture) {
    p.lifetime = texture->descriptor_lifetime; p.generation = texture->source ? texture->source->generation : 0;
    p.handle = texture->source ? texture->source->handle : 0;
    p.view = NativeVulkanHandleIdentity(texture->resource.view);
    p.components = {uint32_t(texture->view_components.r), uint32_t(texture->view_components.g),
                    uint32_t(texture->view_components.b), uint32_t(texture->view_components.a)};
    texture->last_used_frame = std::max(texture->last_used_frame, active_texture_frame_);
    texture->last_used_submission = std::max(texture->last_used_submission,
        submission_tracker_ ? submission_tracker_->GetCurrentSubmission() : 0);
    StageNativeTextureFlightResources(*texture);
  }
  // A vector slot is reserved before recording to avoid reallocations after a
  // Vulkan copy starts referring to its storage.
  probes.reserve(kBulbFullReadbackCount);
  if (!ui::vulkan::util::CreateDedicatedAllocationBuffer(device, bytes, VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      ui::vulkan::util::MemoryPurpose::kReadback, p.buffer, p.memory, &p.memory_type, &p.allocation_size)) {
    skip("readback-allocation"); return;
  }
  const auto& dfn = device->functions(); const auto vk_device = device->device();
  if (dfn.vkMapMemory(vk_device, p.memory, 0, VK_WHOLE_SIZE, 0,
                     reinterpret_cast<void**>(&p.mapping)) != VK_SUCCESS) {
    dfn.vkDestroyBuffer(vk_device, p.buffer, nullptr); dfn.vkFreeMemory(vk_device, p.memory, nullptr);
    skip("readback-map"); return;
  }
  const auto sync = ImageLayoutSynchronization(layout, aspect);
  VkImageMemoryBarrier to_copy{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
  to_copy.srcAccessMask = sync.access; to_copy.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  to_copy.oldLayout = layout; to_copy.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  to_copy.srcQueueFamilyIndex = to_copy.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  to_copy.image = image;
  const VkImageAspectFlags barrier_aspect = surface ? surface->aspect : texture ? texture->aspect : aspect;
  to_copy.subresourceRange = ui::vulkan::util::InitializeSubresourceRange(barrier_aspect, 0, 1, 0, 1);
  dfn.vkCmdPipelineBarrier(cb, sync.stages, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &to_copy);
  VkBufferImageCopy copy{};
  copy.imageSubresource = {aspect, 0, 0, 1}; copy.imageExtent = {width, height, 1};
  dfn.vkCmdCopyImageToBuffer(cb, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, p.buffer, 1, &copy);
  auto restore = to_copy;
  restore.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT; restore.dstAccessMask = sync.access;
  restore.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL; restore.newLayout = layout;
  VkBufferMemoryBarrier host{VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER};
  host.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; host.dstAccessMask = VK_ACCESS_HOST_READ_BIT;
  host.srcQueueFamilyIndex = host.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  host.buffer = p.buffer; host.size = bytes;
  dfn.vkCmdPipelineBarrier(cb, VK_PIPELINE_STAGE_TRANSFER_BIT, sync.stages | VK_PIPELINE_STAGE_HOST_BIT,
                         0, 0, nullptr, 1, &host, 1, &restore);
  bulb_full_frame_ = p.frame; bulb_full_bytes_ += bytes;
  probes.push_back(std::move(p));
  REXLOG_INFO("gta4-bulb-full: point=scheduled frame={} cmd={} role={} image={} bytes={} slot={} gpu-completed=false",
              diagnostic_submitted_frame_, diagnostic_command_index_, role, NativeVulkanHandleIdentity(image), bytes, active_frame_slot_);
}

void Gta4NativeGraphicsSystem::RecordBulbInterferenceBefore(VkCommandBuffer cb, const NativeCommand& command,
                                                           const NativeRenderingTarget& target) {
  if (!NeedsBulbInterference(command, target)) return;
  RecordBulbFullImage(cb, "target-before", &command, target.color_surfaces[0]);
  if (command.bulb_trace && target.depth_surface && target.depth_surface->samples == VK_SAMPLE_COUNT_1_BIT)
    RecordBulbFullImage(cb, "depth-before", &command, target.depth_surface, nullptr, VK_NULL_HANDLE,
                       VK_FORMAT_UNDEFINED, VK_IMAGE_LAYOUT_UNDEFINED, 0, 0, VK_IMAGE_ASPECT_DEPTH_BIT);
  for (uint32_t slot = 0; slot < command.textures.size(); ++slot) {
    if (!(command.used_texture_mask & (uint32_t{1} << slot)) || !command.textures[slot]) continue;
    const auto found = native_texture_images_.find(command.textures[slot]->generation);
    if (found == native_texture_images_.end()) continue;
    auto* image = found->second.get();
    RecordBulbFullImage(cb, fmt::format("input-{}", slot), &command, nullptr, image, VK_NULL_HANDLE,
                       VK_FORMAT_UNDEFINED, VK_IMAGE_LAYOUT_UNDEFINED, 0, 0, image->view_range.aspectMask);
  }
}

void Gta4NativeGraphicsSystem::PublishBulbFullProbe(uint32_t slot, uint64_t submission) {
  if (slot >= bulb_full_probes_.size() || bulb_full_probes_[slot].empty()) return;
  auto* provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const auto* device = provider ? provider->vulkan_device() : nullptr;
  if (!device) return;
  auto& probes = bulb_full_probes_[slot];
  for (auto& p : probes) {
    if (!p.submission || p.submission > submission) continue;
    const auto& dfn = device->functions(); const auto vk_device = device->device();
    bool visible = p.mapping != nullptr;
    if (visible && !(device->memory_types().host_coherent & (uint32_t{1} << p.memory_type))) {
      VkMappedMemoryRange range{VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE};
      range.memory = p.memory; range.size = VK_WHOLE_SIZE;
      visible = dfn.vkInvalidateMappedMemoryRanges(vk_device, 1, &range) == VK_SUCCESS;
    }
    auto directory = EmissionTraceConfig().directory;
    if (directory.empty()) directory = std::filesystem::temp_directory_path() / fmt::format("liberty-bulb-{}", p.run);
    std::error_code error; std::filesystem::create_directories(directory, error);
    const auto stem = fmt::format("full-r{}-f{}-c{}-{}", p.run, p.frame, p.command_index, p.role);
    bool written = false;
    if (visible && !error) {
      std::ofstream data(directory / (stem + ".bin"), std::ios::binary);
      data.write(reinterpret_cast<const char*>(p.mapping), std::streamsize(p.bytes)); data.close();
      written = bool(data);
      if (written) {
        std::ofstream meta(directory / (stem + ".json"));
        meta << fmt::format("{{\"schema\":1,\"run\":{},\"frame\":{},\"command_index\":{},\"source_sequence\":{},\"sequence\":{},"
          "\"fixture\":\"{}\",\"role\":\"{}\",\"pixel_shader\":\"{:016X}\",\"vertex_shader\":\"{:016X}\","
          "\"image\":{},\"view\":{},\"lifetime\":{},\"generation\":{},\"handle\":{},\"layout_restored\":{},"
          "\"format\":{},\"aspect\":{},\"width\":{},\"height\":{},\"bytes\":{},\"block_extent\":{},\"bytes_per_block\":{},"
          "\"components\":[{},{},{},{}],\"submission\":{},\"completed_submission\":{},\"slot\":{},\"gpu_completed\":true,"
          "\"checksum\":\"{:016X}\",\"binary\":\"{}.bin\"}}\n",
          p.run,p.frame,p.command_index,p.source_sequence,p.sequence,kBulbFixtures[p.fixture].role,p.role,
          p.pixel_shader,p.vertex_shader,p.image,p.view,p.lifetime,p.generation,p.handle,p.layout,
          p.format,p.aspect,p.width,p.height,p.bytes,p.block_extent,p.bytes_per_block,
          p.components[0],p.components[1],p.components[2],p.components[3],p.submission,submission,slot,
          XXH3_64bits(p.mapping,size_t(p.bytes)),stem);
        meta.close(); written = bool(meta);
      }
    }
    REXLOG_INFO("gta4-bulb-full: point={} frame={} cmd={} role={} submission={} bytes={} metadata={}",
                written ? "gpu-complete" : "unavailable", p.frame,p.command_index,p.role,p.submission,p.bytes,
                (directory / (stem + ".json")).string());
    if (p.mapping) dfn.vkUnmapMemory(vk_device, p.memory);
    dfn.vkDestroyBuffer(vk_device, p.buffer, nullptr); dfn.vkFreeMemory(vk_device, p.memory, nullptr);
    p.buffer = VK_NULL_HANDLE; p.memory = VK_NULL_HANDLE; p.mapping = nullptr;
  }
  std::erase_if(probes, [](const auto& p) { return !p.buffer; });
}

void Gta4NativeGraphicsSystem::DestroyBulbFullProbes() {
  auto* provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const auto* device = provider ? provider->vulkan_device() : nullptr;
  if (!device) return;
  for (auto& slot : bulb_full_probes_) for (auto& p : slot) {
    REXLOG_WARN("gta4-bulb-full: point=unavailable frame={} cmd={} role={} reason=shutdown-before-publication submission={}",
                p.frame,p.command_index,p.role,p.submission);
    if (p.mapping) device->functions().vkUnmapMemory(device->device(), p.memory);
    if (p.buffer) device->functions().vkDestroyBuffer(device->device(), p.buffer, nullptr);
    if (p.memory) device->functions().vkFreeMemory(device->device(), p.memory, nullptr);
  }
  for (auto& slot : bulb_full_probes_) slot.clear();
}
