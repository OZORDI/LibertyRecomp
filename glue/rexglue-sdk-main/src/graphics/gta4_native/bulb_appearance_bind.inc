// Included at the end of BindCommonDrawState after all actual allocations exist.
if (bulb_trace_frame_ && command.bulb_trace) {
  const auto& source=*command.bulb_trace;
  const auto equal_upload=[](const std::vector<uint8_t>* expected,const NativeUploadAllocation& allocation) {
    if(!expected || !allocation.mapping)return false;
    for(size_t i=0;i<expected->size();i+=4) {
      uint32_t actual=0;std::memcpy(&actual,allocation.mapping+i,4);
      if(actual!=BulbGuestWord(*expected,i))return false;
    }
    return true;
  };
  const auto* pixel_shader=command.pipeline_state->pixel_shader_resource;
  const auto selection=ResolvePipelineShaderOverrides(command.pipeline_state->vertex_shader_resource,pixel_shader,target.samples);
  const auto color=NativeColorOutput(shared_key.color_output_info[0]);
  TraceBulbCommand("bound",command,"actual-gpu-bindings");
  REXLOG_INFO("gta4-bulb: point=upload run={} frame={} cmd={} seq={} fixture={} vertex-upload-equal={} pixel-upload-equal={} "
              "vertex-buffer={} offset={} address={} pixel-buffer={} offset={} address={} shared-buffer={} offset={} address={} "
              "pipeline={} vertex-override={} pixel-override={} output-scale={} alpha-bounds={},{} "
              "depth={},{},{} stencil={},{},{} cull={} clip={:08X} alpha={},{},{} sample-mask={} color-mask={:08X} "
              "blend-word={:08X} viewport={},{},{},{},{},{} scissor={},{},{},{}",
      bulb_run_,diagnostic_submitted_frame_,diagnostic_command_index_,command.diagnostic_submit_sequence,kBulbFixtures[source.fixture].role,
      equal_upload(vertex_constants,vertex_constants_allocation),equal_upload(pixel_constants,pixel_constants_allocation),
      NativeVulkanHandleIdentity(vertex_constants_allocation.buffer),vertex_constants_allocation.offset,vertex_constants_allocation.device_address,
      NativeVulkanHandleIdentity(pixel_constants_allocation.buffer),pixel_constants_allocation.offset,pixel_constants_allocation.device_address,
      NativeVulkanHandleIdentity(shared_constants_allocation.buffer),shared_constants_allocation.offset,shared_constants_allocation.device_address,
      NativeVulkanHandleIdentity(pipeline),selection.vertex_override,selection.pixel_override,color.scale[0],color.minimum[3],color.maximum[3],
      fixed.depth_enable,fixed.depth_function,fixed.depth_write_enable,fixed.stencil_enable,fixed.stencil_function,fixed.stencil_reference,
      fixed.cull_mode,fixed.clip_control,fixed.alpha_test_enable,fixed.alpha_function,fixed.alpha_reference,fixed.sample_mask,target.color_write_mask,
      fixed.blend_controls[0],viewport.x,viewport.y,viewport.width,viewport.height,viewport.minDepth,viewport.maxDepth,
      scissor.offset.x,scissor.offset.y,scissor.extent.width,scissor.extent.height);
  auto directory=EmissionTraceConfig().directory;
  if(directory.empty())directory=std::filesystem::temp_directory_path()/fmt::format("liberty-bulb-{}",bulb_run_);
  std::error_code error;std::filesystem::create_directories(directory,error);
  const auto prefix=fmt::format("bulb-r{}-f{}-s{}",bulb_run_,diagnostic_submitted_frame_,command.diagnostic_submit_sequence);
  const auto dump=[&](std::string_view suffix,std::span<const uint8_t> data) {
    if(error)return;
    const auto path=directory/(prefix+std::string(suffix));
    std::ofstream file(path,std::ios::binary);file.write(reinterpret_cast<const char*>(data.data()),std::streamsize(data.size()));file.close();
    REXLOG_INFO("gta4-bulb: point=input-artifact run={} frame={} seq={} fixture={} kind={} bytes={} written={} path={}",
                bulb_run_,diagnostic_submitted_frame_,command.diagnostic_submit_sequence,kBulbFixtures[source.fixture].role,suffix,data.size(),bool(file),path.string());
  };
  dump("-producer-vs.bin",source.vertex_constants);dump("-producer-ps.bin",source.pixel_constants);
  if (!source.display_gamma.empty()) {
    dump("-display-gamma.bin",source.display_gamma);
    if (command.device_snapshot.size()) dump("-device-snapshot.bin", {command.device_snapshot.data(), command.device_snapshot.size()});
    REXLOG_INFO("gta4-bulb: point=source-display-state run={} frame={} cmd={} seq={} fixture={} display-format={:08X} display-dirty={:08X} gamma-hash={:016X} "
                "clock-captured={} hour={} minute={} hour-override={} minute-override={} phase=producer-snapshot-not-gpu-result",
                bulb_run_,diagnostic_submitted_frame_,diagnostic_command_index_,command.diagnostic_submit_sequence,kBulbFixtures[source.fixture].role,
                source.display_format,source.display_dirty,XXH3_64bits(source.display_gamma.data(),source.display_gamma.size()),
                source.clock_captured,source.game_clock[2],source.game_clock[0],int32_t(source.game_clock[3]),int32_t(source.game_clock[1]));
  }

  dump("-renderer-vs.bin",*vertex_constants);dump("-renderer-ps.bin",*pixel_constants);
  if(vertex_constants_allocation.mapping)dump("-upload-vs.bin",{vertex_constants_allocation.mapping,vertex_constants->size()});
  if(pixel_constants_allocation.mapping)dump("-upload-ps.bin",{pixel_constants_allocation.mapping,pixel_constants->size()});
  if(shared_constants_allocation.mapping)dump("-upload-shared.bin",{shared_constants_allocation.mapping,sizeof(NativeSharedConstants)});
  if(command.vertex_buffers[0])dump("-vertices-guest.bin",command.vertex_buffers[0]->payload);
  if(command.index_buffer)dump("-indices-guest.bin",command.index_buffer->payload);
  for(uint32_t slot=0;slot<command.textures.size();++slot) {
    if(!(command.used_texture_mask&(uint32_t{1}<<slot)) || !command.textures[slot])continue;
    const auto& resource=*command.textures[slot];
    const auto entry=native_texture_images_.find(resource.generation);
    const auto* image=entry!=native_texture_images_.end()?entry->second.get():nullptr;
    REXLOG_INFO("gta4-bulb: point=texture run={} frame={} cmd={} seq={} fixture={} slot={} handle={:08X} generation={} "
                "hash={:016X} image={} view={} lifetime={} descriptor={} sampler={} required={} realized={} failed={} "
                "format={} dimensions={}x{} fetch={:08X},{:08X},{:08X},{:08X},{:08X},{:08X}",
        bulb_run_,diagnostic_submitted_frame_,diagnostic_command_index_,command.diagnostic_submit_sequence,kBulbFixtures[source.fixture].role,
        slot,resource.handle,resource.generation,resource.content_hash,image?NativeVulkanHandleIdentity(image->resource.image):0,
        image?NativeVulkanHandleIdentity(image->resource.view):0,image?image->descriptor_lifetime:0,
        command.texture_descriptor_indices[slot],command.sampler_descriptor_indices[slot],bool(command.used_texture_mask&(uint32_t{1}<<slot)),
        bool(command.realized_image_mask&(uint32_t{1}<<slot)),bool(command.failed_texture_mask&(uint32_t{1}<<slot)),
        image?uint32_t(image->format):0,image?image->width:0,image?image->height:0,
        command.texture_fetches[slot].dword_0,command.texture_fetches[slot].dword_1,command.texture_fetches[slot].dword_2,
        command.texture_fetches[slot].dword_3,command.texture_fetches[slot].dword_4,command.texture_fetches[slot].dword_5);
    dump(fmt::format("-texture{}.bin",slot),resource.payload);
    for (const auto& mip : resource.mip_levels)
      REXLOG_INFO("gta4-bulb: point=mip run={} frame={} seq={} slot={} level={} width={} height={} offset={} size={} row-length={} image-height={}",
                  bulb_run_,diagnostic_submitted_frame_,command.diagnostic_submit_sequence,slot,mip.level,mip.width,mip.height,
                  mip.payload_offset,mip.payload_size,mip.buffer_row_length,mip.buffer_image_height);
  }
}

// On an already-selected bulb frame, preserve the actual post-process constants.
// The readbacks follow these same draws. This only writes diagnostic artifacts;
// it never changes an exposure value, image, shader, or dirty-state delta.
if (bulb_trace_frame_ && !bulb_routes_.empty() && command.pipeline_state &&
    command.pipeline_state->pixel_shader_resource &&
    command.pipeline_state->pixel_shader_resource->filename == "shader/rage_shaders/rage_postfx_e2.fxc") {
  static uint64_t last_sequence = UINT64_MAX;
  static uint32_t captured_count = 0;
  if (captured_count < 512 && last_sequence != command.diagnostic_submit_sequence) {
    last_sequence = command.diagnostic_submit_sequence;
    ++captured_count;
    const auto& shader = *command.pipeline_state->pixel_shader_resource;
    auto directory = EmissionTraceConfig().directory;
    if (directory.empty()) directory = std::filesystem::temp_directory_path() / fmt::format("liberty-bulb-{}", bulb_run_);
    std::error_code error;
    std::filesystem::create_directories(directory, error);
    const auto prefix = fmt::format("postfx-r{}-f{}-c{}-p{:016X}", bulb_run_, diagnostic_submitted_frame_, diagnostic_command_index_, shader.hash);
    auto dump = [&](std::string_view suffix, std::span<const uint8_t> bytes) {
      if (error) return;
      const auto path = directory / (prefix + std::string(suffix));
      std::ofstream output(path, std::ios::binary);
      output.write(reinterpret_cast<const char*>(bytes.data()), std::streamsize(bytes.size()));
      output.close();
      REXLOG_INFO("gta4-bulb: point=postfx-constants frame={} cmd={} seq={} ps={:016X} kind={} bytes={} written={} path={}",
                  diagnostic_submitted_frame_, diagnostic_command_index_, command.diagnostic_submit_sequence,
                  shader.hash, suffix, bytes.size(), bool(output), path.string());
    };
    dump("-pixel-guest.bin", *pixel_constants);
    dump("-vertex-guest.bin", *vertex_constants);
    if (pixel_constants_allocation.mapping)
      dump("-pixel-upload.bin", {pixel_constants_allocation.mapping, pixel_constants->size()});
    if (shared_constants_allocation.mapping)
      dump("-shared-upload.bin", {shared_constants_allocation.mapping, sizeof(NativeSharedConstants)});
    if (command.device_snapshot.size() >= kGuestDeviceSize) {
      const std::span<const uint8_t> snapshot(command.device_snapshot.data(), command.device_snapshot.size());
      dump("-producer-pixel.bin", snapshot.subspan(kPixelConstantsOffset, kPixelConstantsSize));
    }
    const auto output = NativeColorOutput(shared_key.color_output_info[0]);
    REXLOG_INFO("gta4-bulb: point=postfx-bind frame={} cmd={} seq={} ps={:016X} target={:08X}/{:08X} extent={}x{} "
                "used={:08X} realized={:08X} failed={:08X} color-scale={} blend={:08X} source-snapshot={}",
                diagnostic_submitted_frame_, diagnostic_command_index_, command.diagnostic_submit_sequence,
                shader.hash, command.pipeline_state->render_targets[0].handle, shared_key.color_output_info[0],
                width, height, command.used_texture_mask, command.realized_image_mask, command.failed_texture_mask,
                output.scale[0], fixed.blend_controls[0], command.device_snapshot.size() >= kGuestDeviceSize);
    for (uint32_t slot = 0; slot < command.textures.size(); ++slot) {
      if (!(command.used_texture_mask & (uint32_t{1} << slot)) || !command.textures[slot]) continue;
      const auto& resource = *command.textures[slot];
      const auto image = native_texture_images_.find(resource.generation);
      REXLOG_INFO("gta4-bulb: point=postfx-texture frame={} cmd={} ps={:016X} slot={} handle={:08X} generation={} image={} descriptor={} sampler={}",
                  diagnostic_submitted_frame_, diagnostic_command_index_, shader.hash, slot, resource.handle,
                  resource.generation, image != native_texture_images_.end() ? NativeVulkanHandleIdentity(image->second->resource.image) : 0,
                  command.texture_descriptor_indices[slot], command.sampler_descriptor_indices[slot]);
    }
  }
}

if (NeedsBulbInterference(command, target)) {
  auto full_directory = EmissionTraceConfig().directory;
  if (full_directory.empty()) full_directory = std::filesystem::temp_directory_path() / fmt::format("liberty-bulb-{}", bulb_run_);
  const auto full_prefix = fmt::format("draw-r{}-f{}-c{}",bulb_run_,diagnostic_submitted_frame_,diagnostic_command_index_);
  const auto dump_draw = [&](std::string_view suffix, std::span<const uint8_t> bytes) {
    const auto path = full_directory / (full_prefix + std::string(suffix));
    std::ofstream artifact(path,std::ios::binary);
    artifact.write(reinterpret_cast<const char*>(bytes.data()),std::streamsize(bytes.size()));artifact.close();
    REXLOG_INFO("gta4-bulb-full: point=draw-artifact frame={} cmd={} kind={} written={} bytes={} path={}",
                diagnostic_submitted_frame_,diagnostic_command_index_,suffix,bool(artifact),bytes.size(),path.string());
  };
  dump_draw("-command.bin",command.bytes);dump_draw("-payload.bin",command.payload);
  dump_draw("-fixed-function.bin", {reinterpret_cast<const uint8_t*>(&fixed), sizeof(fixed)});
  dump_draw("-renderer-vs.bin", *vertex_constants);dump_draw("-renderer-ps.bin", *pixel_constants);
  if (EmissionTraceConfig().corona_trace && IsBulbCoronaShader(command.pipeline_state->pixel_shader_resource->hash)) {
    for (uint32_t stream=0;stream<command.vertex_buffers.size();++stream) if(command.vertex_buffers[stream]) {
      dump_draw(fmt::format("-vb{}.bin",stream),command.vertex_buffers[stream]->payload);
      REXLOG_INFO("gta4-bulb-corona: point=stream frame={} cmd={} stream={} offset={} stride={} bytes={}",
          diagnostic_submitted_frame_,diagnostic_command_index_,stream,command.pipeline_state->vertex_streams[stream].offset,
          command.pipeline_state->vertex_streams[stream].stride,command.vertex_buffers[stream]->payload.size());
    }
    if(command.index_buffer)dump_draw("-indices.bin",command.index_buffer->payload);
    if(command.pipeline_state->vertex_declaration_resource)for(const auto& e:command.pipeline_state->vertex_declaration_resource->elements)
      REXLOG_INFO("gta4-bulb-corona: point=element frame={} cmd={} stream={} offset={} type={} usage={} index={}",
          diagnostic_submitted_frame_,diagnostic_command_index_,e.stream,e.offset,e.type,e.usage,e.usage_index);
    REXLOG_INFO("gta4-bulb-corona: point=bound frame={} cmd={} seq={} vs={:016X} ps={:016X} required={:08X} realized={:08X} failed={:08X} "
                "vertex-booleans={:08X} pixel-booleans={:08X} prospective-fixture={} prospective-sequence={}",
          diagnostic_submitted_frame_,diagnostic_command_index_,command.diagnostic_submit_sequence,
          command.pipeline_state->vertex_shader_resource->hash,command.pipeline_state->pixel_shader_resource->hash,
          command.used_texture_mask,command.realized_image_mask,command.failed_texture_mask,
          command.shader_state->vertex_booleans,command.shader_state->pixel_booleans,
          bulb_prospective_route_ ? kBulbFixtures[bulb_prospective_route_->source->fixture].role : "none",
          bulb_prospective_route_ ? bulb_prospective_route_->sequence : 0);
  }
  if (shared_constants_allocation.mapping)
    dump_draw("-upload-shared.bin", {shared_constants_allocation.mapping,sizeof(NativeSharedConstants)});
  if (command.device_snapshot.size())
    dump_draw("-device-snapshot.bin",{command.device_snapshot.data(),command.device_snapshot.size()});

  const auto shader_hash = command.pipeline_state->pixel_shader_resource->hash;
  REXLOG_INFO("gta4-bulb-full: point=draw-bind frame={} cmd={} seq={} ps={:016X} blend-enabled={} blend-word={:08X} depth={},{},{} "
              "stencil={} cull={} viewport={},{},{},{},{},{} scissor={},{},{},{} color-mask={:08X} output-info={:08X} bindings={}",
              diagnostic_submitted_frame_, diagnostic_command_index_, command.diagnostic_submit_sequence, shader_hash,
              fixed.blend_enable,fixed.blend_controls[0],fixed.depth_enable,fixed.depth_function,fixed.depth_write_enable,
              fixed.stencil_enable,fixed.cull_mode,viewport.x,viewport.y,viewport.width,viewport.height,viewport.minDepth,viewport.maxDepth,
              scissor.offset.x,scissor.offset.y,scissor.extent.width,scissor.extent.height,target.color_write_mask,
              shared_key.color_output_info[0],command.bindings_prepared);
  for (uint32_t stage=0;stage<command.textures.size();++stage) {
    if (!(command.used_texture_mask & (uint32_t{1}<<stage))) continue;
    const auto& fetch=command.texture_fetches[stage];
    const auto& resource=command.textures[stage];
    const auto found=resource?native_texture_images_.find(resource->generation):native_texture_images_.end();
    const auto* image=found!=native_texture_images_.end()?found->second.get():nullptr;
    REXLOG_INFO("gta4-bulb-full: point=texture-contract frame={} cmd={} seq={} ps={:016X} slot={} "
                "exp-adjust={} signs={},{},{},{} number-format={} swizzle={} fetch-format={} "
                "fetch={:08X},{:08X},{:08X},{:08X},{:08X},{:08X} required=true realized={} failed={} "
                "resource-hash={:016X} generation={} vk-format={} dimensions={}x{} logical={}x{} "
                "created-frame={} resolved-frame={} initialized={} writer={} ancestor={} view-components={},{},{},{}",
                diagnostic_submitted_frame_,diagnostic_command_index_,command.diagnostic_submit_sequence,shader_hash,stage,
                int32_t(fetch.exp_adjust),uint32_t(fetch.sign_x),uint32_t(fetch.sign_y),uint32_t(fetch.sign_z),uint32_t(fetch.sign_w),
                uint32_t(fetch.num_format),uint32_t(fetch.swizzle),uint32_t(fetch.format),
                fetch.dword_0,fetch.dword_1,fetch.dword_2,fetch.dword_3,fetch.dword_4,fetch.dword_5,
                bool(command.realized_image_mask&(uint32_t{1}<<stage)),bool(command.failed_texture_mask&(uint32_t{1}<<stage)),
                resource?resource->content_hash:0,resource?resource->generation:0,image?uint32_t(image->format):0,
                image?image->width:0,image?image->height:0,image?image->logical_width:0,image?image->logical_height:0,
                image?image->created_frame:0,image?image->content.last_resolved_frame:0,
                image?image->aspect_content.color.initialized:false,image?image->aspect_content.color.writer_serial:0,
                image?image->aspect_content.color.ancestor_serial:0,
                image?uint32_t(image->view_components.r):0,image?uint32_t(image->view_components.g):0,
                image?uint32_t(image->view_components.b):0,image?uint32_t(image->view_components.a):0);
  }
  if (command.room_light_input_bindings) for (const auto& b : *command.room_light_input_bindings) {
    const auto& info=b.sampler_info;
    REXLOG_INFO("gta4-bulb-full: point=binding frame={} cmd={} ps={:016X} slot={} image={} view={} lifetime={} sampler={} "
                "filter={},{},{} address={},{},{} lod={},{},{} anisotropy={},{} compare={},{} unnormalized={}",
                diagnostic_submitted_frame_,diagnostic_command_index_,shader_hash,b.texture_stage,
                NativeVulkanHandleIdentity(b.image),NativeVulkanHandleIdentity(b.view),b.image_lifetime,NativeVulkanHandleIdentity(b.sampler),
                uint32_t(info.magFilter),uint32_t(info.minFilter),uint32_t(info.mipmapMode),uint32_t(info.addressModeU),uint32_t(info.addressModeV),uint32_t(info.addressModeW),
                info.mipLodBias,info.minLod,info.maxLod,bool(info.anisotropyEnable),info.maxAnisotropy,bool(info.compareEnable),uint32_t(info.compareOp),bool(info.unnormalizedCoordinates));
  }
}
