// Diagnostic-only TV content lineage. Every GPU copy restores its source layout.
uint32_t Gta4NativeGraphicsSystem::TvCommandRole(const NativeCommand& command) const {
  if (!command.tv_trace) return 0;
  const auto& c = *command.tv_trace;
  uint32_t role = 0;
  if (command.pipeline_state) {
    const auto* ps = command.pipeline_state->pixel_shader_resource;
    if (ps && IsTvBinkShader(ps->hash)) role |= 1;
    for (const auto& rt : command.pipeline_state->render_targets)
      if (c.script_surface && rt.handle == c.script_surface) role |= 2;
    for (uint32_t slot = 0; slot < command.textures.size(); ++slot)
      if (c.script_texture && (command.used_texture_mask & (uint32_t{1} << slot)) &&
          (command.pipeline_state->textures[slot] == c.script_texture ||
           (command.textures[slot] && command.textures[slot]->handle == c.script_texture))) role |= 4;
  }
  if (command.resolve_destination && command.resolve_destination->handle == c.script_texture) role |= 8;
  return role;
}

void Gta4NativeGraphicsSystem::TraceTvNativeCommand(const NativeCommand& command) {
  const auto role = TvCommandRole(command);
  if (!role) return;
  const auto& c = *command.tv_trace;
  const auto& f = command.fixed_function_state;
  const auto* state = command.pipeline_state.get();
  const auto* ps = state ? state->pixel_shader_resource : nullptr;
  const auto* vs = state ? state->vertex_shader_resource : nullptr;
  TvTraceLog("command",fmt::format(
      "run={} event={} binding={} seq={} epoch={} frame={} guest-frame={} cmd={} role={} type={} "
      "vs={:016X} ps={:016X} producer={:08X} consumer={:08X} "
      "viewport={:08X},{:08X},{:08X},{:08X},{:08X},{:08X} scissor={},{},{},{},{} "
      "depth={},{},{} stencil={},{},{},{} alpha={},{},{} coverage={} blend={},{},{},{} "
      "color-mask={} textures={:08X}/{:08X}/{:08X} constants={:016X}/{:016X}",
      c.run,c.event,c.binding_event,command.diagnostic_submit_sequence,command.diagnostic_producer_epoch,
      diagnostic_submitted_frame_,c.guest_frame,diagnostic_command_index_,role,CommandTypeName(command.type),
      vs?vs->hash:0,ps?ps->hash:0,c.script_surface,c.script_texture,
      f.viewport_bits[0],f.viewport_bits[1],f.viewport_bits[2],f.viewport_bits[3],f.viewport_bits[4],f.viewport_bits[5],
      f.scissor_enable,f.scissor[0],f.scissor[1],f.scissor[2],f.scissor[3],
      f.depth_enable,f.depth_function,f.depth_write_enable,f.stencil_enable,f.stencil_function,
      f.stencil_reference,f.stencil_mask,f.alpha_test_enable,f.alpha_function,f.alpha_reference,
      f.alpha_to_mask,f.blend_enable,f.source_blend,f.destination_blend,f.blend_operation,
      f.color_write_mask,command.used_texture_mask,command.realized_image_mask,command.failed_texture_mask,
      command.vertex_constants_hash,command.pixel_constants_hash));
  if ((role & 1) && command.shader_state && command.shader_state->pixel_constants) {
    auto constants = AuthoritativeConstantState::Materialize(command.shader_state->pixel_constants);
    std::string words;
    for (uint32_t reg : {208u,209u,210u,211u,255u}) {
      if ((reg+1)*16 > constants->size()) continue;
      std::array<uint32_t,4> value{};
      std::memcpy(value.data(),constants->data()+reg*16,16);
      words += fmt::format(" c{}={:08X},{:08X},{:08X},{:08X}",reg,value[0],value[1],value[2],value[3]);
    }
    TvTraceLog("yuv-constants",fmt::format("run={} event={} storage=materialized{}",c.run,c.event,words));
  }
  if (command.type == CommandType::kClear) {
    ClearCommand clear{}; std::memcpy(&clear,command.bytes.data(),sizeof(clear));
    TvTraceLog("clear",fmt::format("run={} event={} flags={:08X} rect={},{},{},{} rgba={:08X},{:08X},{:08X},{:08X}",
        c.run,c.event,clear.flags,clear.left,clear.top,clear.right,clear.bottom,
        clear.color_bits[0],clear.color_bits[1],clear.color_bits[2],clear.color_bits[3]));
  }
  if (command.type == CommandType::kResolve) {
    ResolveCommand resolve{}; std::memcpy(&resolve,command.bytes.data(),sizeof(resolve));
    TvTraceLog("resolve",fmt::format("run={} event={} source={:08X}/{:08X} destination={:08X} generation={} level={} slice={} flags={:08X} rect={},{},{},{}",
        c.run,c.event,resolve.source.handle,resolve.source.address,resolve.destination_texture,
        command.resolve_destination?command.resolve_destination->generation:0,
        resolve.destination_level,resolve.destination_slice_or_face,resolve.flags,
        resolve.source_rectangle.left,resolve.source_rectangle.top,resolve.source_rectangle.right,resolve.source_rectangle.bottom));
  }
}

void Gta4NativeGraphicsSystem::RecordTvImage(VkCommandBuffer cb,const NativeCommand& command,
    uint32_t checkpoint,uint32_t attachment,NativeSurfaceImage* surface,NativeTextureImage* texture,
    std::string_view role) {
  if (!command.tv_trace || !TvTraceConfig().readbacks) return;
  const auto& c = *command.tv_trace;
  const auto unavailable = [&](std::string_view reason) {
    TvTraceLog("probe-unavailable",fmt::format("run={} event={} frame={} cmd={} checkpoint={} attachment={} role={} reason={}",
        c.run,c.event,diagnostic_submitted_frame_,diagnostic_command_index_,checkpoint,attachment,role,reason));
  };
  if ((!surface && !texture) || (surface && texture)) { unavailable("image-not-identified"); return; }
  const auto image = surface ? surface->resource.image : texture->resource.image;
  const auto format = surface ? surface->format : texture->format;
  const auto layout = surface ? surface->layout : texture->layout;
  const auto samples = surface ? surface->samples : texture->samples;
  const auto aspects = surface ? surface->aspect : texture->aspect;
  const auto width = surface ? surface->width : texture->width;
  const auto height = surface ? surface->height : texture->height;
  const auto& content = surface ? surface->aspect_content : texture->aspect_content;
  if (texture && !(texture->usage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)) { unavailable("no-transfer-src-usage"); return; }
  if (samples != VK_SAMPLE_COUNT_1_BIT) { unavailable("multisampled-image"); return; }
  if (!InitializeContentProbeBuffer(&tv_content_probe_buffer_)) { unavailable("readback-allocation"); return; }
  auto& buffer = tv_content_probe_buffer_;
  // A zero native frame can be a legitimate internal flush. Occupancy is separate.
  buffer.pending_frame = 1;
  for (const auto aspect : {VK_IMAGE_ASPECT_COLOR_BIT,VK_IMAGE_ASPECT_DEPTH_BIT,VK_IMAGE_ASPECT_STENCIL_BIT}) {
    if (!(aspects & aspect)) continue;
    const auto bpt = PhoneProbeTexelBytes(format,aspect);
    const auto& stamp = aspect == VK_IMAGE_ASPECT_COLOR_BIT ? content.color :
                        aspect == VK_IMAGE_ASPECT_DEPTH_BIT ? content.depth : content.stencil;
    if (!bpt || !stamp.initialized || layout == VK_IMAGE_LAYOUT_UNDEFINED) { unavailable("format-or-content-undefined"); continue; }
    auto free = std::find_if(buffer.stages.begin(),buffer.stages.end(),[](const auto& s){return !s.valid && !s.reserved;});
    if (free == buffer.stages.end()) { unavailable("probe-budget"); return; }
    const auto index = uint32_t(std::distance(buffer.stages.begin(),free));
    const auto handle = surface ? surface->descriptor.handle : texture->source ? texture->source->handle : 0;
    const auto address = surface ? surface->descriptor.address : texture->source ? texture->source->info.memory.base_address : 0;
    auto identity = std::make_shared<TvProbeIdentity>();
    identity->context=c; identity->seq=command.diagnostic_submit_sequence;
    identity->epoch=command.diagnostic_producer_epoch; identity->frame=diagnostic_submitted_frame_;
    identity->command_index=uint32_t(diagnostic_command_index_); identity->checkpoint=checkpoint;
    identity->attachment=attachment; identity->image=NativeVulkanHandleIdentity(image);
    identity->lifetime=surface?surface->lifetime_id:texture->descriptor_lifetime;
    identity->generation=texture && texture->source?texture->source->generation:0;
    identity->writer=stamp.writer_serial; identity->ancestor=stamp.ancestor_serial;
    identity->bytes_per_texel=bpt; identity->role=role;
    if (surface) MarkNativeSurfaceImageUsed(*surface);
    else {
      texture->last_used_frame=std::max(texture->last_used_frame,active_texture_frame_);
      texture->last_used_submission=std::max(texture->last_used_submission,
          submission_tracker_?submission_tracker_->GetCurrentSubmission():0);
      StageNativeTextureFlightResources(*texture);
    }
    if (!RecordContentProbeImage(cb,index,image,format,layout,width,height,handle,address,92,0,aspect,0,1,&buffer)) {
      unavailable("image-copy-recording-failed"); continue;
    }
    free->tv=std::move(identity);
    TvTraceLog("probe-scheduled",fmt::format("run={} event={} seq={} frame={} cmd={} checkpoint={} attachment={} aspect={} image={} lifetime={} generation={} writer={} role={}",
        c.run,c.event,free->tv->seq,free->tv->frame,free->tv->command_index,checkpoint,attachment,uint32_t(aspect),
        free->tv->image,free->tv->lifetime,free->tv->generation,free->tv->writer,role));
  }
}

void Gta4NativeGraphicsSystem::RecordTvTarget(VkCommandBuffer cb,const NativeCommand& command,
    const NativeRenderingTarget& target,uint32_t checkpoint) {
  if (!TvCommandRole(command)) return;
  for (uint32_t i=0;i<target.color_surfaces.size();++i)
    if (target.color_surfaces[i]) RecordTvImage(cb,command,checkpoint,i,target.color_surfaces[i],nullptr,"draw-color");
  if (target.depth_surface) RecordTvImage(cb,command,checkpoint,4,target.depth_surface,nullptr,"draw-depth-stencil");
}

void Gta4NativeGraphicsSystem::RecordTvInputs(VkCommandBuffer cb,const NativeCommand& command) {
  if (!TvCommandRole(command)) return;
  const auto& c=*command.tv_trace;
  for (uint32_t slot=0;slot<command.textures.size();++slot) {
    if (!(command.used_texture_mask & (uint32_t{1}<<slot))) continue;
    const auto& resource=command.textures[slot];
    const NativeRoomLightInputBinding* binding=nullptr;
    if (command.room_light_input_bindings)
      for (const auto& b:*command.room_light_input_bindings) if (b.texture_stage==slot) {binding=&b;break;}
    NativeTextureImage* image=nullptr;
    if (resource) {
      auto found=native_texture_images_.find(resource->generation);
      if (found!=native_texture_images_.end()) image=found->second.get();
    }
    const bool exact=binding && image && binding->image==image->resource.image &&
        binding->view==image->resource.view && binding->image_lifetime==image->descriptor_lifetime;
    TvTraceLog("consumed-texture",fmt::format(
        "run={} event={} slot={} handle={:08X} generation={} payload-hash={:016X} payload-bytes={} "
        "image={} view={} lifetime={} exact={} dimensions={}x{} descriptor={} sampler={} "
        "lod={},{},{} filter={},{},{} realization={} binding-snapshot={:08X}",
        c.run,c.event,slot,resource?resource->handle:0,resource?resource->generation:0,
        resource?resource->content_hash:0,resource?resource->payload.size():0,
        image?NativeVulkanHandleIdentity(image->resource.image):0,binding?NativeVulkanHandleIdentity(binding->view):0,
        image?image->descriptor_lifetime:0,exact,image?image->width:0,image?image->height:0,
        command.texture_descriptor_indices[slot],command.sampler_descriptor_indices[slot],
        binding?binding->sampler_info.minLod:0,binding?binding->sampler_info.maxLod:0,
        binding?binding->sampler_info.mipLodBias:0,
        binding?uint32_t(binding->sampler_info.minFilter):0,binding?uint32_t(binding->sampler_info.magFilter):0,
        binding?uint32_t(binding->sampler_info.mipmapMode):0,uint32_t(command.binding_realization[slot]),
        slot<c.plane_handles.size()?c.plane_handles[slot]:0));
    if (!exact || image->view_type!=VK_IMAGE_VIEW_TYPE_2D || image->view_range.baseMipLevel || image->view_range.baseArrayLayer) {
      TvTraceLog("probe-unavailable",fmt::format("run={} event={} slot={} reason=consumed-view-unproven",c.run,c.event,slot));
      continue;
    }
    if ((TvCommandRole(command)&1) && resource && !resource->payload.empty() &&
        tv_exported_cpu_generations_.size()<4096 && tv_exported_cpu_generations_.emplace(c.run,resource->generation).second) {
      const auto publish=[resource,c]() {
        auto directory=TvTraceDirectory(c.run);std::error_code error;
        std::filesystem::create_directories(directory,error);
        if(error){TvTraceLog("incomplete","reason=cpu-output-directory");return;}
        auto path=directory/fmt::format("cpu-{}.bin",resource->generation);
        std::ofstream output(path,std::ios::binary);output.write(reinterpret_cast<const char*>(resource->payload.data()),std::streamsize(resource->payload.size()));output.close();
        const auto* mip=resource->mip_levels.empty()?nullptr:&resource->mip_levels.front();
        TvTraceLog(output?"cpu-payload":"incomplete",fmt::format("run={} generation={} handle={:08X} width={} height={} format={} bytes={} row-length={} image-height={} offset={} xxh3={:016X} path={}",
            c.run,resource->generation,resource->handle,resource->info.width+1,resource->info.height+1,
            uint32_t(resource->info.format),resource->payload.size(),mip?mip->buffer_row_length:0,
            mip?mip->buffer_image_height:0,mip?mip->payload_offset:0,
            XXH3_64bits(resource->payload.data(),resource->payload.size()),path.string()));
      };
      if(!EnqueueDeferredDiagnosticTask(publish)) RunLightProbeAnalysisInline(publish);
    }
    RecordTvImage(cb,command,3,8+slot,nullptr,image,(TvCommandRole(command)&1)?"bink-input":"material-input");
  }
}

void Gta4NativeGraphicsSystem::RecordTvResolve(VkCommandBuffer cb,const NativeCommand& command,uint32_t checkpoint) {
  if (!(TvCommandRole(command)&8)) return;
  ResolveCommand resolve{};std::memcpy(&resolve,command.bytes.data(),sizeof(resolve));
  NativeSurfaceImage* source=nullptr;
  if (const auto* owner=FindPlacementOwner(resolve.source,false)) source=owner->image;
  RecordTvImage(cb,command,checkpoint,40,source,nullptr,"script-resolve-source");
  const auto found=native_texture_images_.find(command.resolve_destination->generation);
  auto* destination=found==native_texture_images_.end()?nullptr:found->second.get();
  RecordTvImage(cb,command,checkpoint,41,nullptr,destination,"script-resolve-destination");
}

void Gta4NativeGraphicsSystem::PublishTvProbe(const NativeContentProbeStage& stage,const uint8_t* bytes,bool visible) {
  if(!stage.tv) return;
  const auto& p=*stage.tv;const auto& c=p.context;
  if(!visible || !bytes || !p.submission) {
    TvTraceLog("incomplete",fmt::format("run={} event={} checkpoint={} attachment={} reason=gpu-completion-unproven",c.run,c.event,p.checkpoint,p.attachment));return;
  }
  auto directory=TvTraceDirectory(c.run);std::error_code error;std::filesystem::create_directories(directory,error);
  if(error){TvTraceLog("incomplete","reason=readback-output-directory");return;}
  const auto stem=fmt::format("e{}-c{}-a{}-p{}",c.event,p.checkpoint,p.attachment,uint32_t(stage.aspect));
  std::vector<uint8_t> compact(size_t(kContentProbeSampleCount)*p.bytes_per_texel);
  for(uint32_t i=0;i<kContentProbeSampleCount;++i) std::memcpy(compact.data()+size_t(i)*p.bytes_per_texel,bytes+size_t(i)*kContentProbeSampleStride,p.bytes_per_texel);
  std::ofstream output(directory/(stem+".bin"),std::ios::binary);
  output.write(reinterpret_cast<const char*>(compact.data()),std::streamsize(compact.size()));output.close();
  if(!output){TvTraceLog("incomplete","reason=readback-write");return;}
  const auto json=fmt::format(
      "{{\"schema\":1,\"run\":{},\"event\":{},\"seq\":{},\"epoch\":{},\"frame\":{},\"guest_frame\":{},\"command_index\":{},"
      "\"checkpoint\":{},\"attachment\":{},\"aspect\":{},\"format\":{},\"width\":{},\"height\":{},\"handle\":{},\"image\":{},"
      "\"lifetime\":{},\"generation\":{},\"writer\":{},\"ancestor\":{},\"submission\":{},\"completed\":true,\"bytes_per_texel\":{},"
      "\"samples\":{},\"bytes\":{},\"xxh3\":\"{:016X}\",\"role\":\"{}\",\"binary\":\"{}.bin\"}}\n",
      c.run,c.event,p.seq,p.epoch,p.frame,c.guest_frame,p.command_index,p.checkpoint,p.attachment,
      uint32_t(stage.aspect),uint32_t(stage.format),stage.width,stage.height,stage.handle,p.image,p.lifetime,
      p.generation,p.writer,p.ancestor,p.submission,p.bytes_per_texel,kContentProbeSampleCount,compact.size(),
      XXH3_64bits(compact.data(),compact.size()),p.role,stem);
  std::ofstream metadata(directory/(stem+".json"));metadata<<json;metadata.close();
  TvTraceLog(metadata?"probe-complete":"incomplete",fmt::format("run={} event={} frame={} cmd={} checkpoint={} attachment={} aspect={} submission={} metadata={}",
      c.run,c.event,p.frame,p.command_index,p.checkpoint,p.attachment,uint32_t(stage.aspect),p.submission,(directory/(stem+".json")).string()));
}

void Gta4NativeGraphicsSystem::AnalyzePendingTvProbe(uint32_t slot,uint64_t submission) {
  auto& buffer=slot==active_frame_slot_?tv_content_probe_buffer_:secondary_tv_content_probe_buffer_;
  if(!buffer.pending_frame) return;
  auto* provider=static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const auto* device=provider?provider->vulkan_device():nullptr;
  bool visible=device && buffer.mapping && submission;
  for(const auto& stage:buffer.stages) if(stage.tv && (!stage.tv->submission || stage.tv->submission>submission)) visible=false;
  if(visible && !(device->memory_types().host_coherent & (uint32_t{1}<<buffer.memory_type))) {
    VkMappedMemoryRange range{};range.sType=VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;range.memory=buffer.memory;
    range.size=std::min(rex::round_up(kContentProbeBufferSize,device->properties().nonCoherentAtomSize),buffer.allocation_size);
    visible=device->functions().vkInvalidateMappedMemoryRanges(device->device(),1,&range)==VK_SUCCESS;
  }
  auto stages=std::make_shared<decltype(buffer.stages)>(std::move(buffer.stages));
  auto payload=std::make_shared<std::vector<uint8_t>>(size_t(kContentProbeBufferSize),0);
  if(visible) std::memcpy(payload->data(),buffer.mapping,payload->size());
  buffer.pending_frame=0;buffer.stages={};
  auto publish=[stages,payload,visible]() {
    for(uint32_t i=0;i<stages->size();++i) if((*stages)[i].tv)
      PublishTvProbe((*stages)[i],visible?payload->data()+size_t(i)*kContentProbeStageStride:nullptr,visible);
  };
  if(!EnqueueDeferredDiagnosticTask(publish)) RunLightProbeAnalysisInline(publish);
}


bool Gta4NativeGraphicsSystem::IsTvLifecycleImage(const NativeTextureImage& image) const {
  if (!TvTraceConfig().enabled || !tv_lifecycle_trace_ || !image.source) return false;
  const uint32_t handle = image.source->handle;
  return (handle && handle == tv_lifecycle_trace_->script_texture) ||
         tv_lifecycle_plane_handles_.contains(handle);
}

void Gta4NativeGraphicsSystem::TraceTvImageLifecycle(std::string_view point,
    const NativeTextureImage& image, std::string_view reason, std::string_view details) {
  if (!IsTvLifecycleImage(image)) return;
  const auto& c = *tv_lifecycle_trace_;
  const auto& source = *image.source;
  TvTraceLog(point, fmt::format(
      "run={} lifecycle={} context-event={} boundary-seq={} guest-frame={} batch-frame={} "
      "active-texture-frame={} handle={:08X} kind={} generation={} image={} view={} lifetime={} "
      "created-frame={} last-used-frame={} last-use-serial={} last-submission={} completed-submission={} "
      "gpu-produced={} payload-bytes={} origin={} virtual-lifetime={} defined={} resolved={} "
      "resolved-frame={} resolved-source-writer={} color-writer={} color-ancestor={} "
      "format={} size={}x{} mips={} layout={} bytes={} descriptor-reclaimed={} retirement-queued={} "
      "reason={} {}",
      c.run, ++tv_lifecycle_id_, c.event, tv_lifecycle_command_sequence_, c.guest_frame,
      tv_lifecycle_batch_frame_, active_texture_frame_, source.handle,
      source.handle == c.script_texture ? "script-target" : "video-plane",
      source.generation, NativeVulkanHandleIdentity(image.resource.image),
      NativeVulkanHandleIdentity(image.resource.view), image.descriptor_lifetime,
      image.created_frame, image.last_used_frame, image.last_use_serial, image.last_used_submission,
      completed_command_buffer_submission_, source.gpu_produced, source.payload.size(),
      uint32_t(source.origin), source.virtual_lifetime, image.content.defined, image.content.resolved,
      image.content.last_resolved_frame, image.content.last_source_write_serial,
      image.aspect_content.color.writer_serial, image.aspect_content.color.ancestor_serial,
      uint32_t(image.format), image.width, image.height, image.mip_levels, uint32_t(image.layout),
      image.allocation_size, image.descriptor_reclaimed, image.descriptor_retirement_queued, reason, details));
}
