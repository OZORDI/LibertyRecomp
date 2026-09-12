// Observation-only trace. GPU resources belong to their exact physical frame slot.
// Copies run outside rendering, restore layouts, and publish after the submission fence.
static bool FireDownstreamShader(std::string_view name) {
  return name.find("/deferred_lighting/") != std::string_view::npos ||
         name.find("/rage_postfx") != std::string_view::npos ||
         name.find("/gta_im/") != std::string_view::npos;
}
void Gta4NativeGraphicsSystem::BeginFireFrame(VkCommandBuffer cb, uint32_t frame) {
  fire_frame_ = fire_images_ = fire_event_active_ = false;
  fire_frame_bytes_ = 0; fire_checkpoints_.clear(); fire_input_images_.clear();
  if (!FireTraceConfig().enabled) return;
  static bool announced = false;
  if (!announced) {
    announced = true;
    FireTraceLog("armed", fmt::format("selection={} request-file={} crop={},{},{},{} expiry=none observation-only=true",
      FireTraceConfig().elevated_rails ? "elevated-rail-materials" : "fire-escape-materials",
      FireTraceConfig().request_file, FireTraceConfig().crop_x, FireTraceConfig().crop_y,
      FireTraceConfig().crop_width, FireTraceConfig().crop_height));
  }
  fire_first_draw_index_ = fire_last_ui_index_ = SIZE_MAX;
  size_t matches = 0;
  for (size_t index = 0; index < current_frame_.size(); ++index) {
    const auto& c = current_frame_[index];
    const bool draw = c.type == CommandType::kDrawIndexedPrimitive || c.type == CommandType::kDrawPrimitive || c.type == CommandType::kDrawPrimitiveUp;
    if (!draw) continue;
    if (c.fire_trace && FireStrongTextureName(c.fire_trace->texture_name.data())) {
      ++matches; if (fire_first_draw_index_ == SIZE_MAX) fire_first_draw_index_ = index;
    }
    const auto* ps = c.pipeline_state ? c.pipeline_state->pixel_shader_resource : nullptr;
    if (ps && ps->filename.find("/gta_im/") != std::string::npos) fire_last_ui_index_ = index;
  }
  const auto now = std::chrono::steady_clock::now();
  if (matches) fire_last_seen_ = now;
  const bool recent = fire_last_seen_.time_since_epoch().count() &&
      now - fire_last_seen_ < std::chrono::seconds(60);
  uint64_t requested = 0;
  if (!FireTraceConfig().request_file.empty()) {
    std::ifstream request(FireTraceConfig().request_file);
    if (!(request >> requested)) requested = 0;
  }
  const bool requested_capture = requested > fire_capture_request_;
  if (!recent && !requested_capture) return;
  fire_frame_ = frame % FireTraceConfig().interval == 0 || !fire_last_logged_frame_;
  if (!fire_frame_) return;
  fire_last_logged_frame_ = frame ? frame : 1;
  fire_images_ = recent && (fire_last_images_.time_since_epoch().count() == 0 ||
      now - fire_last_images_ >= std::chrono::seconds(FireTraceConfig().image_interval_seconds));
  if (!FireTraceConfig().request_file.empty()) fire_images_ = requested_capture;
  if (fire_images_) {
    fire_last_images_ = now;
    fire_capture_request_ = std::max(fire_capture_request_, requested);
    FireTraceLog("capture-request", fmt::format("frame={} request={} rail-draws={}", frame, requested, matches));
  }
  FireTraceLog("frame-plan",fmt::format("frame={} commands={} fire-draws={} images={} slot={} observation-only=true",
    frame,current_frame_.size(),matches,fire_images_,active_frame_slot_));
  auto* provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const auto* device = provider ? provider->vulkan_device() : nullptr;
  if (!device || active_frame_slot_ >= fire_queries_.size()) return;
  auto& q = fire_queries_[active_frame_slot_];
  q.ready = false;
  if (!q.rows.empty()) { FireTraceLog("incomplete","reason=query-slot-not-retired"); return; }
  if (!q.pool) {
    VkQueryPoolCreateInfo info{VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO};
    info.queryType = VK_QUERY_TYPE_OCCLUSION; info.queryCount = 128;
    if (device->functions().vkCreateQueryPool(device->device(),&info,nullptr,&q.pool) != VK_SUCCESS) {
      FireTraceLog("incomplete","reason=query-pool-allocation"); return;
    }
  }
  device->functions().vkCmdResetQueryPool(cb,q.pool,0,128);
  q.submission = 0; q.ready = true;
}
void Gta4NativeGraphicsSystem::TraceFireCommand(std::string_view point,const NativeCommand& c,std::string_view result) {
  if (!fire_frame_) return;
  const auto* p = c.pipeline_state.get();
  const auto* ps = p ? p->pixel_shader_resource : nullptr;
  const auto* vs = p ? p->vertex_shader_resource : nullptr;
  const bool draw = c.type == CommandType::kDrawIndexedPrimitive || c.type == CommandType::kDrawPrimitive || c.type == CommandType::kDrawPrimitiveUp;
  if (!c.fire_trace && draw && (!ps || !FireDownstreamShader(ps->filename))) return;
  const FireTraceContext empty; const auto& x = c.fire_trace ? *c.fire_trace : empty;
  const auto& f = c.fixed_function_state;
  DrawIndexedPrimitiveCommand d{};
  if (c.type == CommandType::kDrawIndexedPrimitive && c.bytes.size() == sizeof(d)) std::memcpy(&d,c.bytes.data(),sizeof(d));
  FireTraceLog(point,fmt::format("frame={} cmd={} seq={} epoch={} occurrence={} event={} type={} phase={} result={} model={:08X} geometry={:08X} material={:08X} ordinal={} bucket={} technique={:08X} pass={:08X} replay={} list={:08X} texture-name={} vs={:016X} ps={:016X} shader={} draw={} indices={} start={} base={} caller={:08X} rt={:08X} ds={:08X} depth={},{},{} stencil={},{},{},{},{} stencil-back={},{},{},{} alpha={},{},{} coverage={} cull={} blend={},{},{},{} alpha-blend={},{},{} write-mask={:08X} used={:08X} realized={:08X} failed={:08X} pbool={:08X} vbool={:08X}",
    diagnostic_submitted_frame_,diagnostic_command_index_,c.diagnostic_submit_sequence,c.diagnostic_producer_epoch,
    x.occurrence,x.event,CommandTypeName(c.type),uint32_t(c.render_phase),result,x.model,x.geometry,x.material,x.ordinal,x.bucket,x.technique,x.pass,x.replay,x.command_list,x.texture_name.data(),
    vs?vs->hash:0,ps?ps->hash:0,ps?ps->filename:"none",d.draw_id,d.index_count,d.start_index,d.base_vertex,d.caller,
    p?p->render_targets[0].handle:0,p?p->depth_stencil.handle:0,f.depth_enable,f.depth_function,f.depth_write_enable,
    f.stencil_enable,f.stencil_function,f.stencil_reference,f.stencil_mask,f.stencil_write_mask,
    f.two_sided_stencil,f.ccw_stencil_function,f.ccw_stencil_pass,f.ccw_stencil_depth_fail,
    f.alpha_test_enable,f.alpha_function,f.alpha_reference,f.alpha_to_mask,f.cull_mode,
    f.blend_enable,f.source_blend,f.destination_blend,f.blend_operation,f.source_blend_alpha,f.destination_blend_alpha,f.blend_operation_alpha,
    f.color_write_mask,c.used_texture_mask,c.realized_image_mask,c.failed_texture_mask,
    c.shader_state?c.shader_state->pixel_booleans:0,c.shader_state?c.shader_state->vertex_booleans:0));
  if (c.fire_trace && p && point == "record-begin") {
    FireTraceLog("raster-state",fmt::format("frame={} seq={} viewport={:08X},{:08X},{:08X},{:08X},{:08X},{:08X} scissor={},{},{},{},{} clip={:08X} depth-clamp={} sample-mask={:08X} blend-controls={:08X},{:08X},{:08X},{:08X}",
      diagnostic_submitted_frame_,c.diagnostic_submit_sequence,f.viewport_bits[0],f.viewport_bits[1],f.viewport_bits[2],f.viewport_bits[3],f.viewport_bits[4],f.viewport_bits[5],f.scissor_enable,f.scissor[0],f.scissor[1],f.scissor[2],f.scissor[3],f.clip_control,f.depth_clamp_enable,f.sample_mask,f.blend_controls[0],f.blend_controls[1],f.blend_controls[2],f.blend_controls[3]));
    for (uint32_t i=0;i<4;++i) {
      const auto& r=p->render_targets[i];
      FireTraceLog("attachment",fmt::format("seq={} index={} handle={:08X} address={:08X} dimensions={}x{} samples={}",c.diagnostic_submit_sequence,i,r.handle,r.address,r.width,r.height,r.sample_type));
    }
  }
  if (c.type==CommandType::kResolve && c.bytes.size()==sizeof(ResolveCommand)) {
    ResolveCommand r{};std::memcpy(&r,c.bytes.data(),sizeof(r));
    FireTraceLog("resolve",fmt::format("frame={} cmd={} seq={} source={:08X} destination={:08X} generation={} mip={} slice={} flags={:08X} rectangle={},{},{},{} point={},{} origin={} caller={:08X}",diagnostic_submitted_frame_,diagnostic_command_index_,c.diagnostic_submit_sequence,r.source.handle,r.destination_texture,c.resolve_destination?c.resolve_destination->generation:0,r.destination_level,r.destination_slice_or_face,r.flags,r.source_rectangle.left,r.source_rectangle.top,r.source_rectangle.right,r.source_rectangle.bottom,r.destination_point.x,r.destination_point.y,r.trace_origin,r.trace_caller));
  }
}
bool Gta4NativeGraphicsSystem::SelectFireCheckpoint(const NativeCommand& c, const NativeRenderingTarget& target) {
  if (!fire_images_ || !c.pipeline_state || !c.pipeline_state->pixel_shader_resource) return false;
  if (c.type != CommandType::kDrawIndexedPrimitive && c.type != CommandType::kDrawPrimitive && c.type != CommandType::kDrawPrimitiveUp) return false;
  std::string key;
  if (c.fire_trace) key = target.is_reflection ? "fire-reflection" : "fire-" + std::to_string(c.fire_trace->bucket);
  else {
    const std::string_view name=c.pipeline_state->pixel_shader_resource->filename;
    if (fire_first_draw_index_ != SIZE_MAX && diagnostic_command_index_ < fire_first_draw_index_) return false;
    if (name.find("/gta_im/")!=std::string_view::npos) key = diagnostic_command_index_ == fire_last_ui_index_ ? "ui-final" : "ui-first";
    else if (name.find("/rage_postfx")!=std::string_view::npos) key="postfx";
    else if (name.find("deferred_lighting_ps0.bin")!=std::string_view::npos ||
             name.find("deferred_lighting_ps1.bin")!=std::string_view::npos ||
             name.find("deferred_lighting_ps2.bin")!=std::string_view::npos ||
             name.find("deferred_lighting_ps3.bin")!=std::string_view::npos) key="deferred-composite";
  }
  return !key.empty() && fire_checkpoints_[key]++ == 0;
}
void Gta4NativeGraphicsSystem::FireImage(VkCommandBuffer cb,std::string_view role,const NativeCommand* command,
    NativeSurfaceImage* surface,NativeTextureImage* texture,VkImage direct,VkFormat direct_format,
    VkImageLayout direct_layout,uint32_t direct_width,uint32_t direct_height,VkImageAspectFlags aspect,uint32_t mip) {
  if (!fire_images_) return;
  const auto skip=[&](std::string_view reason) {FireTraceLog("probe-unavailable",fmt::format("frame={} cmd={} role={} mip={} reason={}",diagnostic_submitted_frame_,diagnostic_command_index_,role,mip,reason));};
  auto* provider=static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const auto* device=provider?provider->vulkan_device():nullptr;
  if (!device || !cb || active_frame_slot_>=fire_probes_.size()) {skip("device-or-slot");return;}
  VkImage image=surface?surface->resource.image:texture?texture->resource.image:direct;
  const auto format=surface?surface->format:texture?texture->format:direct_format;
  const auto layout=surface?surface->layout:texture?texture->layout:direct_layout;
  const auto original_width=CalculateNativeMipExtent(surface?surface->width:texture?texture->width:direct_width,mip);
  const auto original_height=CalculateNativeMipExtent(surface?surface->height:texture?texture->height:direct_height,mip);
  // Sampled input textures keep their complete mip data. Only attachment and
  // presentation copies use the same explicit screen-region crop.
  const bool crop = !texture && (FireTraceConfig().crop_width || FireTraceConfig().crop_height);
  const auto region = MakeNativeProbeRegion(original_width, original_height,
      crop ? FireTraceConfig().crop_x : 0, crop ? FireTraceConfig().crop_y : 0,
      crop ? FireTraceConfig().crop_width : 0, crop ? FireTraceConfig().crop_height : 0);
  if (!region) {skip("crop-outside-image");return;}
  const auto width=region->width, height=region->height;
  const auto samples=surface?surface->samples:texture?texture->samples:VK_SAMPLE_COUNT_1_BIT;
  if (!image || !width || !height || layout==VK_IMAGE_LAYOUT_UNDEFINED) {skip("image-undefined");return;}
  if (texture && (!(texture->usage&VK_IMAGE_USAGE_TRANSFER_SRC_BIT) || mip>=texture->mip_levels || texture->view_type!=VK_IMAGE_VIEW_TYPE_2D)) {skip("usage-view-or-mip");return;}
  if (samples!=VK_SAMPLE_COUNT_1_BIT && aspect!=VK_IMAGE_ASPECT_COLOR_BIT) {skip("multisample-depth-use-explicit-resolve-checkpoint");return;}
  uint32_t block=1,bpt=PhoneProbeTexelBytes(format,aspect);
  if (format==VK_FORMAT_BC1_RGBA_UNORM_BLOCK || format==VK_FORMAT_BC1_RGBA_SRGB_BLOCK) {block=4;bpt=8;}
  if (format==VK_FORMAT_BC2_UNORM_BLOCK || format==VK_FORMAT_BC3_UNORM_BLOCK || format==VK_FORMAT_BC3_SRGB_BLOCK) {block=4;bpt=16;}
  const uint64_t bytes=BulbFullReadbackBytes(width,height,bpt,block);
  const uint64_t budget=uint64_t(FireTraceConfig().image_frame_mib)*1024*1024;
  const uint64_t reserve=role.starts_with("present-")?0:std::min(budget/4,uint64_t{48}*1024*1024);
  auto& probes=fire_probes_[active_frame_slot_];
  const size_t probe_limit = role.starts_with("present-") ? 128 : 126;
  if (!bytes || bytes>budget-reserve || fire_frame_bytes_>budget-reserve-bytes || probes.size()>=probe_limit) {skip("format-or-frame-budget");return;}
  std::error_code disk_error;
  const auto space=std::filesystem::space(FireTraceConfig().directory,disk_error);
  if (disk_error || space.available<uint64_t{768}*1024*1024) {skip("disk-headroom");return;}
  FireFullProbe probe;auto& p=probe.data;
  p.frame=diagnostic_submitted_frame_;p.command_index=uint32_t(diagnostic_command_index_);p.role=role;
  p.sequence=command?command->diagnostic_submit_sequence:0;
  if (command && command->fire_trace) {probe.occurrence=command->fire_trace->occurrence;probe.event=command->fire_trace->event;}
  p.width=width;p.height=height;p.format=uint32_t(format);p.aspect=aspect;p.bytes=bytes;
  p.block_extent=block;p.bytes_per_block=bpt;p.layout=uint32_t(layout);p.image=NativeVulkanHandleIdentity(image);
  probe.mip=mip;probe.samples=uint32_t(samples);
  probe.crop_x=region->x;probe.crop_y=region->y;probe.original_width=original_width;probe.original_height=original_height;
  if (surface) {p.handle=surface->descriptor.handle;p.lifetime=surface->lifetime_id;MarkNativeSurfaceImageUsed(*surface);}
  if (texture) {p.handle=texture->source?texture->source->handle:0;p.generation=texture->source?texture->source->generation:0;p.lifetime=texture->descriptor_lifetime;p.view=NativeVulkanHandleIdentity(texture->resource.view);p.components={uint32_t(texture->view_components.r),uint32_t(texture->view_components.g),uint32_t(texture->view_components.b),uint32_t(texture->view_components.a)}; texture->last_used_frame=std::max(texture->last_used_frame,active_texture_frame_); texture->last_used_submission=std::max(texture->last_used_submission,submission_tracker_?submission_tracker_->GetCurrentSubmission():0);StageNativeTextureFlightResources(*texture);}
  probes.reserve(128);
  const auto& dfn=device->functions();const auto vk_device=device->device();
  if (!ui::vulkan::util::CreateDedicatedAllocationBuffer(device,bytes,VK_BUFFER_USAGE_TRANSFER_DST_BIT,ui::vulkan::util::MemoryPurpose::kReadback,p.buffer,p.memory,&p.memory_type,&p.allocation_size)) {skip("buffer-allocation");return;}
  auto release=[&] {if(p.mapping)dfn.vkUnmapMemory(vk_device,p.memory);if(p.buffer)dfn.vkDestroyBuffer(vk_device,p.buffer,nullptr);if(p.memory)dfn.vkFreeMemory(vk_device,p.memory,nullptr);};
  if (dfn.vkMapMemory(vk_device,p.memory,0,VK_WHOLE_SIZE,0,reinterpret_cast<void**>(&p.mapping))!=VK_SUCCESS) {release();skip("map");return;}
  if (samples!=VK_SAMPLE_COUNT_1_BIT) {
    VkImageCreateInfo info{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};info.imageType=VK_IMAGE_TYPE_2D;info.format=format;info.extent={width,height,1};info.mipLevels=1;info.arrayLayers=1;info.samples=VK_SAMPLE_COUNT_1_BIT;info.tiling=VK_IMAGE_TILING_OPTIMAL;info.usage=VK_IMAGE_USAGE_TRANSFER_SRC_BIT|VK_IMAGE_USAGE_TRANSFER_DST_BIT;info.sharingMode=VK_SHARING_MODE_EXCLUSIVE;
    if (!ui::vulkan::util::CreateDedicatedAllocationImage(device,info,ui::vulkan::util::MemoryPurpose::kDeviceLocal,probe.scratch,probe.scratch_memory)) {release();skip("msaa-readback-image");return;}
  }
  const auto sync=ImageLayoutSynchronization(layout,aspect);
  VkImageMemoryBarrier before{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};before.srcAccessMask=sync.access;before.dstAccessMask=VK_ACCESS_TRANSFER_READ_BIT;before.oldLayout=layout;before.newLayout=VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;before.srcQueueFamilyIndex=before.dstQueueFamilyIndex=VK_QUEUE_FAMILY_IGNORED;before.image=image;
  before.subresourceRange=ui::vulkan::util::InitializeSubresourceRange(surface?surface->aspect:texture?texture->aspect:aspect,mip,1,0,1);
  dfn.vkCmdPipelineBarrier(cb,sync.stages,VK_PIPELINE_STAGE_TRANSFER_BIT,0,0,nullptr,0,nullptr,1,&before);
  VkImage copy_image=image;uint32_t copy_mip=mip;
  if (probe.scratch) {
    VkImageMemoryBarrier scratch{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};scratch.dstAccessMask=VK_ACCESS_TRANSFER_WRITE_BIT;scratch.oldLayout=VK_IMAGE_LAYOUT_UNDEFINED;scratch.newLayout=VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;scratch.srcQueueFamilyIndex=scratch.dstQueueFamilyIndex=VK_QUEUE_FAMILY_IGNORED;scratch.image=probe.scratch;scratch.subresourceRange=ui::vulkan::util::InitializeSubresourceRange();
    dfn.vkCmdPipelineBarrier(cb,VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,VK_PIPELINE_STAGE_TRANSFER_BIT,0,0,nullptr,0,nullptr,1,&scratch);
    VkImageResolve r{};r.srcSubresource={aspect,mip,0,1};r.dstSubresource={aspect,0,0,1};r.srcOffset={int32_t(region->x),int32_t(region->y),0};r.extent={width,height,1};
    dfn.vkCmdResolveImage(cb,image,VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,probe.scratch,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,1,&r);
    scratch.srcAccessMask=VK_ACCESS_TRANSFER_WRITE_BIT;scratch.dstAccessMask=VK_ACCESS_TRANSFER_READ_BIT;scratch.oldLayout=VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;scratch.newLayout=VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    dfn.vkCmdPipelineBarrier(cb,VK_PIPELINE_STAGE_TRANSFER_BIT,VK_PIPELINE_STAGE_TRANSFER_BIT,0,0,nullptr,0,nullptr,1,&scratch);
    copy_image=probe.scratch;copy_mip=0;
  }
  VkBufferImageCopy copy{};copy.imageSubresource={aspect,copy_mip,0,1};copy.imageExtent={width,height,1};
  if (!probe.scratch) copy.imageOffset={int32_t(region->x),int32_t(region->y),0};
  dfn.vkCmdCopyImageToBuffer(cb,copy_image,VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,p.buffer,1,&copy);
  auto restore=before;restore.srcAccessMask=VK_ACCESS_TRANSFER_READ_BIT;restore.dstAccessMask=sync.access;restore.oldLayout=VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;restore.newLayout=layout;
  VkBufferMemoryBarrier host{VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER};host.srcAccessMask=VK_ACCESS_TRANSFER_WRITE_BIT;host.dstAccessMask=VK_ACCESS_HOST_READ_BIT;host.srcQueueFamilyIndex=host.dstQueueFamilyIndex=VK_QUEUE_FAMILY_IGNORED;host.buffer=p.buffer;host.size=bytes;
  dfn.vkCmdPipelineBarrier(cb,VK_PIPELINE_STAGE_TRANSFER_BIT,sync.stages|VK_PIPELINE_STAGE_HOST_BIT,0,0,nullptr,1,&host,1,&restore);
  fire_frame_bytes_+=bytes;probes.push_back(std::move(probe));
  FireTraceLog("probe-scheduled",fmt::format("frame={} cmd={} role={} mip={} image={} bytes={} samples={} slot={} completed=false",diagnostic_submitted_frame_,diagnostic_command_index_,role,mip,NativeVulkanHandleIdentity(image),bytes,uint32_t(samples),active_frame_slot_));
}
void Gta4NativeGraphicsSystem::FireTarget(VkCommandBuffer cb,const NativeCommand& c,const NativeRenderingTarget& target,bool after) {
  if (!fire_images_) return;
  const std::string role=after?"target-after":"target-before";
  for (uint32_t i=0;i<4;++i) if ((target.color_attachment_mask&(1u<<i)) && target.color_surfaces[i])
    FireImage(cb,fmt::format("{}-{}",role,i),&c,target.color_surfaces[i]);
  if (c.fire_trace && target.depth_surface) {
    FireImage(cb,role+"-depth",&c,target.depth_surface,nullptr,VK_NULL_HANDLE,VK_FORMAT_UNDEFINED,VK_IMAGE_LAYOUT_UNDEFINED,0,0,VK_IMAGE_ASPECT_DEPTH_BIT);
    if (target.depth_surface->aspect&VK_IMAGE_ASPECT_STENCIL_BIT) FireImage(cb,role+"-stencil",&c,target.depth_surface,nullptr,VK_NULL_HANDLE,VK_FORMAT_UNDEFINED,VK_IMAGE_LAYOUT_UNDEFINED,0,0,VK_IMAGE_ASPECT_STENCIL_BIT);
  }
  if (!after) FireInputs(cb,c);
}
void Gta4NativeGraphicsSystem::FireInputs(VkCommandBuffer cb,const NativeCommand& c) {
  if (!fire_frame_) return;
  for (uint32_t i=0;i<c.textures.size();++i) {
    if (!(c.used_texture_mask&(1u<<i))) continue;
    const auto& r=c.textures[i];auto found=r?native_texture_images_.find(r->generation):native_texture_images_.end();
    auto* image=found==native_texture_images_.end()?nullptr:found->second.get();
    const NativeRoomLightInputBinding* binding=nullptr;
    if(c.room_light_input_bindings) for(const auto& b:*c.room_light_input_bindings) if(b.texture_stage==i){binding=&b;break;}
    const bool exact=image && binding && binding->image==image->resource.image && binding->view==image->resource.view && binding->image_lifetime==image->descriptor_lifetime;
    FireTraceLog("texture-bind",fmt::format("frame={} cmd={} seq={} slot={} handle={:08X} generation={} payload-hash={:016X} image={} bound-image={} view={} lifetime={} exact={} logical={}x{} host={}x{} format={} mip-count={} reflected={} family={} resolved={} last-frame={} last-writer={} descriptor={} sampler={} filter={},{},{} lod-range={},{} sampler-bias={} guest-bias={} swizzle={} signs={},{},{},{} realization={}",diagnostic_submitted_frame_,diagnostic_command_index_,c.diagnostic_submit_sequence,i,r?r->handle:0,r?r->generation:0,r?r->content_hash:0,image?NativeVulkanHandleIdentity(image->resource.image):0,binding?NativeVulkanHandleIdentity(binding->image):0,binding?NativeVulkanHandleIdentity(binding->view):0,image?image->descriptor_lifetime:0,exact,image?image->logical_width:0,image?image->logical_height:0,image?image->width:0,image?image->height:0,image?uint32_t(image->format):0,image?image->mip_levels:0,image?image->is_reflection:false,image?uint32_t(image->reflection.family):0,image?image->content.resolved:false,image?image->content.last_resolved_frame:0,image?image->content.last_source_write_serial:0,c.texture_descriptor_indices[i],c.sampler_descriptor_indices[i],binding?uint32_t(binding->sampler_info.minFilter):0,binding?uint32_t(binding->sampler_info.magFilter):0,binding?uint32_t(binding->sampler_info.mipmapMode):0,binding?binding->sampler_info.minLod:0,binding?binding->sampler_info.maxLod:0,binding?binding->sampler_info.mipLodBias:0,int32_t(c.texture_fetches[i].lod_bias),uint32_t(c.texture_fetches[i].swizzle),uint32_t(c.texture_fetches[i].sign_x),uint32_t(c.texture_fetches[i].sign_y),uint32_t(c.texture_fetches[i].sign_z),uint32_t(c.texture_fetches[i].sign_w),uint32_t(c.binding_realization[i])));
    if (!cb || !fire_images_ || !image || !exact || !fire_input_images_.insert(image->descriptor_lifetime).second) continue;
    for(uint32_t mip=0;mip<image->mip_levels;++mip) {
      const auto aspect=(image->aspect&VK_IMAGE_ASPECT_COLOR_BIT)?VK_IMAGE_ASPECT_COLOR_BIT:VK_IMAGE_ASPECT_DEPTH_BIT;
      FireImage(cb,fmt::format("input-{}",i),&c,nullptr,image,VK_NULL_HANDLE,VK_FORMAT_UNDEFINED,VK_IMAGE_LAYOUT_UNDEFINED,0,0,aspect,mip);
    }
  }
}
uint32_t Gta4NativeGraphicsSystem::BeginFireQuery(VkCommandBuffer cb,const NativeCommand& c,uint32_t existing) {
  if(!fire_frame_ || !c.fire_trace || existing!=UINT32_MAX || (c.type!=CommandType::kDrawIndexedPrimitive && c.type!=CommandType::kDrawPrimitive && c.type!=CommandType::kDrawPrimitiveUp)) return UINT32_MAX;
  auto& q=fire_queries_[active_frame_slot_];if(!q.ready || !q.pool || q.rows.size()>=128) return UINT32_MAX;
  FireQueryRow row;row.frame=diagnostic_submitted_frame_;row.command=uint32_t(diagnostic_command_index_);row.sequence=c.diagnostic_submit_sequence;row.occurrence=c.fire_trace->occurrence;row.event=c.fire_trace->event;
  const auto index=uint32_t(q.rows.size());q.rows.push_back(row);
  auto* provider=static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  provider->vulkan_device()->functions().vkCmdBeginQuery(cb,q.pool,index,0);
  return index;
}
void Gta4NativeGraphicsSystem::EndFireQuery(VkCommandBuffer cb,uint32_t index) {
  if(index==UINT32_MAX) return;
  auto* provider=static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  provider->vulkan_device()->functions().vkCmdEndQuery(cb,fire_queries_[active_frame_slot_].pool,index);
}
void Gta4NativeGraphicsSystem::PublishFireProbes(uint32_t slot,uint64_t submission) {
  if(slot>=fire_probes_.size()) return;
  auto* provider=static_cast<ui::vulkan::VulkanProvider*>(provider_.get());const auto* device=provider?provider->vulkan_device():nullptr;
  if(!device) return;const auto& dfn=device->functions();const auto vk=device->device();
  auto& q=fire_queries_[slot];
  if(q.submission && q.submission<=submission && !q.rows.empty()) {
    std::vector<std::array<uint64_t,2>> values(q.rows.size());
    const auto status=dfn.vkGetQueryPoolResults(vk,q.pool,0,uint32_t(values.size()),values.size()*sizeof(values[0]),values.data(),sizeof(values[0]),VK_QUERY_RESULT_64_BIT|VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
    for(size_t i=0;i<q.rows.size();++i) {const auto& r=q.rows[i];FireTraceLog("gpu-coverage",fmt::format("frame={} cmd={} seq={} occurrence={} event={} submission={} completed-submission={} result={} available={} any-samples={} raw={} precise=false",r.frame,r.command,r.sequence,r.occurrence,r.event,q.submission,submission,int32_t(status),values[i][1],values[i][1]&&values[i][0],values[i][0]));}
    q.rows.clear();q.submission=0;
  }
  auto& probes=fire_probes_[slot];
  for(auto& probe:probes) {
    auto& p=probe.data;if(!p.submission || p.submission>submission) continue;
    bool visible=p.mapping!=nullptr;
    if(visible && !(device->memory_types().host_coherent&(1u<<p.memory_type))) {VkMappedMemoryRange r{VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE};r.memory=p.memory;r.size=VK_WHOLE_SIZE;visible=dfn.vkInvalidateMappedMemoryRanges(vk,1,&r)==VK_SUCCESS;}
    auto bytes=std::make_shared<std::vector<uint8_t>>();
    if(visible) bytes->assign(p.mapping,p.mapping+p.bytes);
    const auto meta=probe;
    if(p.mapping) dfn.vkUnmapMemory(vk,p.memory);
    dfn.vkDestroyBuffer(vk,p.buffer,nullptr);dfn.vkFreeMemory(vk,p.memory,nullptr);
    if(probe.scratch) dfn.vkDestroyImage(vk,probe.scratch,nullptr);
    if(probe.scratch_memory) dfn.vkFreeMemory(vk,probe.scratch_memory,nullptr);
    p.buffer=VK_NULL_HANDLE;p.memory=VK_NULL_HANDLE;p.mapping=nullptr;probe.scratch=VK_NULL_HANDLE;probe.scratch_memory=VK_NULL_HANDLE;
    auto publish=[meta,bytes,visible,submission,slot] {
      const auto& p=meta.data;
      if(!visible || bytes->empty()) {FireTraceLog("incomplete","reason=gpu-map-not-visible");return;}
      std::vector<uint8_t> compressed(ZSTD_compressBound(bytes->size()));
      const auto size=ZSTD_compress(compressed.data(),compressed.size(),bytes->data(),bytes->size(),1);
      if(ZSTD_isError(size)) {FireTraceLog("incomplete","reason=zstd");return;}
      static std::atomic<uint64_t> disk_bytes{0};const auto budget=uint64_t(FireTraceConfig().disk_mib)*1024*1024;
      auto used=disk_bytes.load();
      for(;;) {if(size>budget || used>budget-size){FireTraceLog("probe-unavailable",fmt::format("frame={} cmd={} role={} reason=compressed-disk-budget",p.frame,p.command_index,p.role));return;}if(disk_bytes.compare_exchange_weak(used,used+size))break;}
      const auto directory=std::filesystem::path(FireTraceConfig().directory);
      const auto stem=fmt::format("f{}-c{}-s{}-{}-m{}-a{}",p.frame,p.command_index,p.sequence,p.role,meta.mip,p.aspect);
      std::ofstream binary(directory/(stem+".bin.zst"),std::ios::binary);binary.write(reinterpret_cast<const char*>(compressed.data()),std::streamsize(size));binary.close();
      std::ofstream json(directory/(stem+".json"));
      json<<fmt::format("{{\"schema\":2,\"crop_x\":{},\"crop_y\":{},\"original_width\":{},\"original_height\":{},\"frame\":{},\"command\":{},\"sequence\":{},\"occurrence\":{},\"event\":{},\"role\":\"{}\",\"image\":{},\"view\":{},\"lifetime\":{},\"generation\":{},\"handle\":{},\"format\":{},\"aspect\":{},\"width\":{},\"height\":{},\"mip\":{},\"source_samples\":{},\"bytes\":{},\"compressed_bytes\":{},\"block_extent\":{},\"bytes_per_block\":{},\"components\":[{},{},{},{}],\"submission\":{},\"completed_submission\":{},\"slot\":{},\"gpu_completed\":true,\"xxh3\":\"{:016X}\",\"binary\":\"{}.bin.zst\"}}\n",meta.crop_x,meta.crop_y,meta.original_width,meta.original_height,p.frame,p.command_index,p.sequence,meta.occurrence,meta.event,p.role,p.image,p.view,p.lifetime,p.generation,p.handle,p.format,p.aspect,p.width,p.height,meta.mip,meta.samples,p.bytes,size,p.block_extent,p.bytes_per_block,p.components[0],p.components[1],p.components[2],p.components[3],p.submission,submission,slot,XXH3_64bits(bytes->data(),bytes->size()),stem);json.close();
      FireTraceLog(binary&&json?"gpu-complete":"incomplete",fmt::format("frame={} cmd={} seq={} occurrence={} event={} role={} mip={} submission={} compressed-bytes={} metadata={}",p.frame,p.command_index,p.sequence,meta.occurrence,meta.event,p.role,meta.mip,p.submission,size,(directory/(stem+".json")).string()));
    };
    if(!EnqueueDeferredDiagnosticTask(publish)) RunLightProbeAnalysisInline(publish);
  }
  std::erase_if(probes,[](const auto& p){return !p.data.buffer;});
}
void Gta4NativeGraphicsSystem::DestroyFireProbes() {
  auto* provider=static_cast<ui::vulkan::VulkanProvider*>(provider_.get());const auto* d=provider?provider->vulkan_device():nullptr;if(!d)return;
  for(auto& q:fire_queries_) {if(q.pool)d->functions().vkDestroyQueryPool(d->device(),q.pool,nullptr);q.pool=VK_NULL_HANDLE;q.rows.clear();}
  for(auto& slot:fire_probes_) {for(auto& p:slot) {FireTraceLog("incomplete",fmt::format("frame={} reason=shutdown-before-readback",p.data.frame));if(p.data.mapping)d->functions().vkUnmapMemory(d->device(),p.data.memory);if(p.data.buffer)d->functions().vkDestroyBuffer(d->device(),p.data.buffer,nullptr);if(p.data.memory)d->functions().vkFreeMemory(d->device(),p.data.memory,nullptr);if(p.scratch)d->functions().vkDestroyImage(d->device(),p.scratch,nullptr);if(p.scratch_memory)d->functions().vkFreeMemory(d->device(),p.scratch_memory,nullptr);}slot.clear();}
}
