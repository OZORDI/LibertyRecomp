// Included in graphics_system.cpp: Vulkan integration for the independently
// tested working-set planner. Only the render worker accesses these pages.
bool Gta4NativeGraphicsSystem::BeginIndexedWorkingSet() {
#ifdef GTA4_PROFILE_DESCRIPTOR_SCOPE
  GTA4_PROFILE_DESCRIPTOR_SCOPE(DescriptorWorkingSet);
#endif
  if (active_frame_slot_ >= native_descriptor_working_sets_.size() || command_buffer_submission_)
    return false;
  auto& planner = native_descriptor_working_sets_[active_frame_slot_];
  // ClearGuestOutput completed this slot's fence before preparation. Internal
  // readback batches use the same fence path, never a still-pending page.
  return planner.Begin(native_descriptor_capacity_, native_sampler_descriptor_capacity_,
                       native_descriptor_maximum_page_count_ / NativeFrameContextRing::kSlotCount,
                       completed_command_buffer_submission_);
}

template <typename DescriptorKey>
bool Gta4NativeGraphicsSystem::AssignIndexedWorkingSet(NativeCommand& command,
                                                       const DescriptorKey& key) {
  auto& planner = native_descriptor_working_sets_[active_frame_slot_];
  std::array<uint64_t, kShaderTextureCount> sampler_ids{};
  for (size_t i = 0; i < sampler_ids.size(); ++i)
    sampler_ids[i] =
        key.samplers[i] == null_sampler_ ? 0 : NativeVulkanHandleIdentity(key.samplers[i]);
  const auto assignment = planner.Assign(key.image_lifetimes, sampler_ids);
  if (!assignment) {
    // This is a real per-frame device-budget overflow, not history exhaustion.
    // PrepareFrameTextures will replay its preparation once with exact cached
    // tuples, retaining every draw rather than publishing partial bindings.
    return ActivateCachedDescriptorFallback(NativeDescriptorStatus::kCapacityExhausted,
                                            "per-frame-working-set-overflow");
  }
  const uint32_t page_index =
      assignment->page * NativeFrameContextRing::kSlotCount + active_frame_slot_;
  if (!EnsureIndexedDescriptorPages(page_index + 1))
    return ActivateCachedDescriptorFallback(NativeDescriptorStatus::kCapacityExhausted,
                                            "working-set-page-allocation");
  auto& page = native_descriptor_pages_[page_index];
  const NativeDescriptorImageSlot null_image{1,
                                             {null_texture_2d_.view, null_texture_2d_array_.view,
                                              null_texture_3d_.view, null_texture_cube_.view}};
  if (!(page.image_slots[0] == null_image)) {
    page.image_slots[0] = null_image;
    page.images_dirty = true;
  }
  const NativeDescriptorSamplerSlot null_sampler{1, null_sampler_};
  if (!(page.sampler_slots[0] == null_sampler)) {
    page.sampler_slots[0] = null_sampler;
    page.samplers_dirty = true;
  }
  for (size_t i = 0; i < kShaderTextureCount; ++i) {
    const uint32_t image_index = assignment->images[i];
    const uint32_t sampler_index = assignment->samplers[i];
    if (image_index) {
      const NativeDescriptorImageSlot wanted{
          key.image_lifetimes[i],
          {key.images_2d[i], key.images_2d_array[i], key.images_3d[i], key.images_cube[i]}};
      if (!(page.image_slots[image_index] == wanted)) {
        page.image_slots[image_index] = wanted;
        page.images_dirty = true;
      }
    }
    if (sampler_index) {
      const NativeDescriptorSamplerSlot wanted{sampler_ids[i], key.samplers[i]};
      if (!(page.sampler_slots[sampler_index] == wanted)) {
        page.sampler_slots[sampler_index] = wanted;
        page.samplers_dirty = true;
      }
    }
  }
  page.frame_epoch = planner.epoch();
  command.descriptor_page = page_index;
  command.descriptor_copy = active_descriptor_copy_;
  command.texture_descriptor_indices = assignment->images;
  command.sampler_descriptor_indices = assignment->samplers;
  command.image_descriptor_epoch = planner.epoch();
  command.sampler_descriptor_epoch = planner.epoch();
  command.bindings_prepared = true;
  return true;
}

bool Gta4NativeGraphicsSystem::PublishIndexedWorkingSet() {
#ifdef GTA4_PROFILE_DESCRIPTOR_SCOPE
  GTA4_PROFILE_DESCRIPTOR_SCOPE(DescriptorPublish);
#endif
  const auto* provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const auto* device = provider ? provider->vulkan_device() : nullptr;
  if (!device || active_frame_slot_ >= native_descriptor_working_sets_.size() ||
      command_buffer_submission_)
    return false;
  const auto& planner = native_descriptor_working_sets_[active_frame_slot_];
  const NativeDescriptorImageSlot null_image{1, {null_texture_2d_.view, null_texture_2d_array_.view,
                                                null_texture_3d_.view, null_texture_cube_.view}};
  const NativeDescriptorSamplerSlot null_sampler{1, null_sampler_};
  for (size_t page_index = active_frame_slot_; page_index < native_descriptor_pages_.size();
       page_index += NativeFrameContextRing::kSlotCount) {
    const size_t local = page_index / NativeFrameContextRing::kSlotCount;
    auto& page = native_descriptor_pages_[page_index];
    const bool used = local < planner.page_count();
    const uint32_t live_images = used ? uint32_t(planner.page(local).images.size()) : 1;
    const uint32_t live_samplers = used ? uint32_t(planner.page(local).samplers.size()) : 1;
    const uint32_t images = std::max(live_images, page.published_image_count);
    const uint32_t samplers = std::max(live_samplers, page.published_sampler_count);
    if (images > page.image_slots.size() || samplers > page.sampler_slots.size()) return false;
    // A shorter working set must overwrite stale descriptors, including pages
    // not used at all this frame. Leaving old views in Metal argument buffers
    // can retain their allocations beyond the resource's logical lifetime.
    for (uint32_t i = used ? live_images : 0; i < images; ++i) {
      if (!(page.image_slots[i] == null_image)) {
        page.image_slots[i] = null_image;
        page.images_dirty = true;
      }
    }
    for (uint32_t i = used ? live_samplers : 0; i < samplers; ++i) {
      if (!(page.sampler_slots[i] == null_sampler)) {
        page.sampler_slots[i] = null_sampler;
        page.samplers_dirty = true;
      }
    }
    page.published_image_count = live_images;
    page.published_sampler_count = live_samplers;
    const size_t image_entries = page.images_dirty ? size_t(images) * 4 : 0;
    const size_t sampler_entries = page.samplers_dirty ? samplers : 0;
    if (!image_entries && !sampler_entries)
      continue;
    page.update_scratch.resize(image_entries + sampler_entries);
    std::array<VkWriteDescriptorSet, 5> writes{};
    uint32_t count = 0;
    if (page.images_dirty) {
      for (uint32_t dimension = 0; dimension < 4; ++dimension) {
        auto* infos = page.update_scratch.data() + size_t(dimension) * images;
        for (uint32_t i = 0; i < images; ++i)
          infos[i] = {VK_NULL_HANDLE, page.image_slots[i].views[dimension],
                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
        auto& write = writes[count++];
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = page.descriptor_sets[dimension];
        write.dstBinding = 0;
        write.descriptorCount = images;
        write.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        write.pImageInfo = infos;
      }
    }
    if (page.samplers_dirty) {
      auto* infos = page.update_scratch.data() + image_entries;
      for (uint32_t i = 0; i < samplers; ++i)
        infos[i] = {page.sampler_slots[i].sampler, VK_NULL_HANDLE, VK_IMAGE_LAYOUT_UNDEFINED};
      auto& write = writes[count++];
      write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      write.dstSet = page.descriptor_sets[4];
      write.dstBinding = 0;
      write.descriptorCount = samplers;
      write.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
      write.pImageInfo = infos;
    }
    device->functions().vkUpdateDescriptorSets(device->device(), count, writes.data(), 0, nullptr);
    const uint64_t entries = image_entries + sampler_entries;
    AddNativeGpuProfileCounter(performance::Counter::kDescriptorEntriesWritten, entries);
    frame_descriptor_entries_written_ += uint32_t(entries);
    page.images_dirty = page.samplers_dirty = false;
  }
  frame_descriptor_unique_draws_ = uint32_t(planner.page_count());
  return true;
}
