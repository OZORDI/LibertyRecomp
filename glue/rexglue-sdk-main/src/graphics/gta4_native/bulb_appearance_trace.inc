// Observation-only extension of the existing emission logger and readback utility.
// No physical-light queries, extra draws, shader substitution, or guest writes.
void Gta4NativeGraphicsSystem::CaptureBulbSource(NativeCommand& command,
                                               std::span<const uint8_t> device_state) {
  if (!EmissionTraceConfig().pipeline || bulb_source_capture_done_.load(std::memory_order_relaxed) ||
      command.type != CommandType::kDrawIndexedPrimitive || !command.vertex_buffers[0]) return;
  DrawIndexedPrimitiveCommand draw{};
  std::memcpy(&draw, command.bytes.data(), sizeof(draw));
  const auto& vb = *command.vertex_buffers[0];
  const int fixture = FindBulbFixture(vb.content_hash, vb.payload.size(), draw.index_count);
  if (fixture < 0 || device_state.size() < kPixelConstantsOffset + kPixelConstantsSize ||
      !EmissionTraceActive(uint32_t(diagnostic_producer_epoch_))) return;
  auto source = std::make_shared<BulbSourceSnapshot>();
  source->fixture = uint32_t(fixture); source->caller = draw.caller;
  source->command_list = draw.command_list; source->origin = draw.origin_flags;
  if (const auto state = producer_device_shader_states_.find(draw.device);
      state != producer_device_shader_states_.end()) {
    const auto shader_hash = [&](uint32_t handle) {
      const auto entry = producer_shader_metadata_.find(handle);
      return entry == producer_shader_metadata_.end() ? uint64_t{} : entry->second.hash;
    };
    source->vertex_shader = shader_hash(state->second.vertex_shader);
    source->pixel_shader = shader_hash(state->second.pixel_shader);
  }
  const auto vertex = device_state.subspan(kVertexConstantsOffset, kVertexConstantsSize);
  const auto pixel = device_state.subspan(kPixelConstantsOffset, kPixelConstantsSize);
  source->vertex_constants.assign(vertex.begin(), vertex.end());
  source->pixel_constants.assign(pixel.begin(), pixel.end());
  if (EmissionTraceConfig().pipeline_full_readback && device_state.size() >= 16696) {
    // Generated .80 sub_82A3B8C8 / sub_82A3B990 store the CPU display LUT here;
    // generated .81 sub_82A467D8 forwards it to the original display path.
    source->display_gamma.assign(device_state.begin()+14988,device_state.begin()+16524);
    source->display_format=BulbGuestWord(device_state,13592);
    source->display_dirty=BulbGuestWord(device_state,16692);
    // Generated sub_8233CA00: minute, minute override, hour, hour override.
    if (const auto* clock = memory_->TranslateVirtual<const uint8_t*>(0x82DF391C)) {
      for (size_t i=0;i<source->game_clock.size();++i) source->game_clock[i]=BulbGuestWord({clock,16},i*4);
      source->clock_captured=true;
    }
  }
  // Independent observer copies. Never replace the normal dirty-state deltas.
  command.bulb_trace = std::move(source);
}

void Gta4NativeGraphicsSystem::BeginBulbFrame(uint32_t frame) {
  bulb_routes_.clear(); bulb_prospective_route_.reset(); bulb_trace_frame_ = false; bulb_current_command_ = nullptr;
  bulb_selected_fixture_ = UINT32_MAX;
  if (!EmissionTraceConfig().pipeline || !EmissionTraceActive(frame)) return;
  if (bulb_capture_count_ >= EmissionTraceConfig().pipeline_frames) {
    if (!bulb_source_capture_done_.exchange(true, std::memory_order_relaxed))
      REXLOG_INFO("gta4-bulb: point=capture-complete run={} captures={} further-source-copies=false",
                  bulb_run_, bulb_capture_count_);
    return;
  }
  if (bulb_capture_count_ && uint64_t(frame) < uint64_t(bulb_last_frame_) + EmissionTraceConfig().pipeline_interval) return;
  std::array<uint32_t, kBulbFixtures.size()> observed{};
  for (const auto& command : current_frame_) if (command.bulb_trace) ++observed[command.bulb_trace->fixture];
  if (std::all_of(observed.begin(), observed.end(), [](auto count) { return count == 0; })) return;
  // One visible fixture per sampled frame leaves space for its entire sequence
  // of later writers. Rotate between eligible fixtures, rather than silently
  // exhausting the shared readback buffer on several simultaneous chains.
  for (const auto& command : current_frame_) {
    if (!command.bulb_trace || !command.pipeline_state || !command.shader_state || !command.vertex_buffers[0]) continue;
    const auto* vs=command.pipeline_state->vertex_shader_resource;
    const auto* ps=command.pipeline_state->pixel_shader_resource;
    if (!vs || !ps || ((vs->hash!=0x1746F0E888217EADull || ps->hash!=0xF634FCEBA607E8A5ull) &&
                      (vs->hash!=0x561E6FAA7F555B48ull || ps->hash!=0x07675BF8EF5E48DCull))) continue;
    if (EmissionTraceConfig().vertex_hash && command.vertex_buffers[0]->content_hash!=EmissionTraceConfig().vertex_hash) continue;
    const auto* constants=AuthoritativeConstantState::MaterializeView(command.shader_state->vertex_constants);
    const auto& stream=command.pipeline_state->vertex_streams[0];
    if (!constants || stream.stride!=24) continue;
    const auto region=InspectBulbScreenRegion(command.vertex_buffers[0]->payload,stream.offset,stream.stride,*constants);
    if (!region.known || !region.visible) continue;
    // Optional capture selection only: don't accept a sliver of a bulb clipped
    // by a screen edge as the diagnostic view of the whole fixture.
    if (EmissionTraceConfig().require_full_region &&
        (region.normalized[0] <= 0.01f || region.normalized[1] <= 0.01f ||
         region.normalized[2] >= 0.99f || region.normalized[3] >= 0.99f)) continue;
    const uint32_t fixture=command.bulb_trace->fixture;
    if (bulb_selected_fixture_==UINT32_MAX || bulb_fixture_visits_[fixture]<bulb_fixture_visits_[bulb_selected_fixture_])
    {
      bulb_selected_fixture_=fixture;
      if (EmissionTraceConfig().corona_trace) {
        BulbRoute route;
        route.source=command.bulb_trace;route.sequence=command.diagnostic_submit_sequence;
        route.command_index=uint32_t(&command-current_frame_.data());route.region=region;
        bulb_prospective_route_=std::move(route);
      }
    }
  }
  if (bulb_selected_fixture_ == UINT32_MAX) {
    static uint32_t last_wait_frame = 0;
    if (frame >= uint64_t(last_wait_frame) + 75) {
      last_wait_frame = frame;
      REXLOG_INFO("gta4-bulb: point=await-visible frame={} reason=selected-forward-mesh-outside-view capture-budget-consumed=false", frame);
    }
    return;
  }
  bulb_trace_frame_ = true; bulb_last_frame_ = frame; ++bulb_capture_count_;
  if (!bulb_run_) bulb_run_ = rex::chrono::Clock::QueryHostTickCount();
  REXLOG_INFO("gta4-bulb: point=frame run={} frame={} capture={}/{} physical-light-checks=reused observer-only=true",
              bulb_run_, frame, bulb_capture_count_, EmissionTraceConfig().pipeline_frames);
  for (size_t i = 0; i < observed.size(); ++i)
    REXLOG_INFO("gta4-bulb: point=inventory run={} frame={} fixture={} model={} preset={} submitted={} missing-is-off=false",
                bulb_run_, frame, kBulbFixtures[i].role, kBulbFixtures[i].model, kBulbFixtures[i].preset, observed[i]);
}

void Gta4NativeGraphicsSystem::TraceBulbCommand(std::string_view point, const NativeCommand& command,
                                              std::string_view reason) {
  if (!bulb_trace_frame_ || !command.bulb_trace) return;
  const auto& src = *command.bulb_trace;
  const auto* vertex = command.shader_state ? AuthoritativeConstantState::MaterializeView(command.shader_state->vertex_constants) : nullptr;
  const auto* pixel = command.shader_state ? AuthoritativeConstantState::MaterializeView(command.shader_state->pixel_constants) : nullptr;
  const auto* vs = command.pipeline_state ? command.pipeline_state->vertex_shader_resource : nullptr;
  const auto* ps = command.pipeline_state ? command.pipeline_state->pixel_shader_resource : nullptr;
  const auto producer = InspectBulbEmission(src.pixel_shader, src.pixel_constants);
  const auto renderer = InspectBulbEmission(ps ? ps->hash : 0, pixel ? std::span<const uint8_t>(*pixel) : std::span<const uint8_t>{});
  REXLOG_INFO("gta4-bulb: point={} run={} frame={} cmd={} seq={} epoch={} fixture={} reason={} "
              "caller={:08X} list={:08X} origin={} producer-shaders={:016X}/{:016X} renderer-shaders={:016X}/{:016X} "
              "producer-state={} renderer-state={} multiplier={} night={} gate={} alpha-scalar={} exp-bias={} "
              "tint={},{},{},{} vertex-transport-equal={} pixel-transport-equal={} required={:08X} realized={:08X} failed={:08X}",
              point, bulb_run_, diagnostic_submitted_frame_, diagnostic_command_index_, command.diagnostic_submit_sequence,
              command.diagnostic_producer_epoch, kBulbFixtures[src.fixture].role, reason,
              src.caller, src.command_list, src.origin, src.vertex_shader, src.pixel_shader, vs ? vs->hash : 0, ps ? ps->hash : 0,
              producer.state, renderer.state, renderer.multiplier, renderer.night, renderer.gate, renderer.scalar, renderer.exposure,
              renderer.tint[0], renderer.tint[1], renderer.tint[2], renderer.tint[3],
              vertex && *vertex == src.vertex_constants, pixel && *pixel == src.pixel_constants,
              command.used_texture_mask, command.realized_image_mask, command.failed_texture_mask);
}

bool Gta4NativeGraphicsSystem::PrepareBulbDraw(const NativeCommand& command, const NativeRenderingTarget& target) {
  if (!bulb_trace_frame_ || !command.bulb_trace || !command.pipeline_state ||
      !command.pipeline_state->vertex_shader_resource || !command.pipeline_state->pixel_shader_resource) return false;
  if (!bulb_routes_.empty() || command.bulb_trace->fixture!=bulb_selected_fixture_) return false;
  const auto vs = command.pipeline_state->vertex_shader_resource->hash;
  const auto ps = command.pipeline_state->pixel_shader_resource->hash;
  if ((vs != 0x1746F0E888217EADull || ps != 0xF634FCEBA607E8A5ull) &&
      (vs != 0x561E6FAA7F555B48ull || ps != 0x07675BF8EF5E48DCull)) return false;
  if (target.is_reflection || target.uses_presenter || !target.color_surfaces[0]) return false;
  const auto* constants = command.shader_state ? AuthoritativeConstantState::MaterializeView(command.shader_state->vertex_constants) : nullptr;
  if (!constants || !command.vertex_buffers[0]) return false;
  const auto& stream = command.pipeline_state->vertex_streams[0];
  DrawIndexedPrimitiveCommand draw{}; std::memcpy(&draw, command.bytes.data(), sizeof(draw));
  const auto& fixed = command.fixed_function_state;
  const bool standard_viewport = std::bit_cast<float>(fixed.viewport_bits[0]) == 0 &&
      std::bit_cast<float>(fixed.viewport_bits[1]) == 0 &&
      std::bit_cast<float>(fixed.viewport_bits[2]) == float(target.logical_width) &&
      std::bit_cast<float>(fixed.viewport_bits[3]) == float(target.logical_height);
  BulbScreenRegion region;
  if (!draw.base_vertex && !draw.start_index && stream.stride == 24 && standard_viewport &&
      !fixed.user_clip_plane_enable_mask && !fixed.negative_one_to_one_clip_space)
    region = InspectBulbScreenRegion(command.vertex_buffers[0]->payload, stream.offset, stream.stride, *constants);
  REXLOG_INFO("gta4-bulb: point=visibility run={} frame={} cmd={} seq={} fixture={} projection-known={} "
              "intersects-view={} region={},{},{},{} viewport-standard={} gpu-visibility=await-query",
              bulb_run_, diagnostic_submitted_frame_, diagnostic_command_index_, command.diagnostic_submit_sequence,
              kBulbFixtures[command.bulb_trace->fixture].role, region.known, region.visible,
              region.normalized[0], region.normalized[1], region.normalized[2], region.normalized[3], standard_viewport);
  if (!region.known || !region.visible) return false;
  BulbRoute route;
  route.source = command.bulb_trace; route.sequence = command.diagnostic_submit_sequence;
  route.command_index = uint32_t(diagnostic_command_index_); route.region = region;
  route.images.emplace(NativeVulkanHandleIdentity(target.color_surfaces[0]->resource.image), target.color_surfaces[0]->lifetime_id);
  ++bulb_fixture_visits_[route.source->fixture];
  bulb_routes_.push_back(std::move(route));
  return true;
}

bool Gta4NativeGraphicsSystem::BulbRouteTouches(const BulbRoute& route, const NativeCommand& command,
                                              const NativeRenderingTarget& target) const {
  if (diagnostic_command_index_ < route.command_index || !target.color_surfaces[0]) return false;
  const auto* surface = target.color_surfaces[0];
  if (route.images.contains({NativeVulkanHandleIdentity(surface->resource.image), surface->lifetime_id})) return true;
  for (uint32_t i = 0; i < command.textures.size(); ++i)
    if ((command.used_texture_mask & (uint32_t{1} << i)) && command.textures[i] &&
        route.generations.contains(command.textures[i]->generation)) return true;
  return false;
}

bool Gta4NativeGraphicsSystem::NeedsBulbCheckpoint(const NativeCommand& command, const NativeRenderingTarget& target) const {
  if (!bulb_trace_frame_) return false;
  return std::any_of(bulb_routes_.begin(), bulb_routes_.end(), [&](const auto& route) {
    return route.writers < EmissionTraceConfig().pipeline_writers && BulbRouteTouches(route, command, target);
  });
}

void Gta4NativeGraphicsSystem::RecordBulbImage(VkCommandBuffer cb, BulbRoute& route,
    std::string_view role, NativeSurfaceImage* surface, NativeTextureImage* texture,
    VkImage direct_image, VkFormat direct_format, VkImageLayout direct_layout,
    uint32_t direct_width, uint32_t direct_height, VkImageAspectFlags aspect, bool full_texture) {
  const auto skip = [&](std::string_view reason) {
    REXLOG_INFO("gta4-bulb: point=probe-unavailable run={} frame={} cmd={} source-seq={} fixture={} role={} reason={}",
        bulb_run_, diagnostic_submitted_frame_, diagnostic_command_index_, route.sequence,
        kBulbFixtures[route.source->fixture].role, role, reason);
  };
  const auto image = surface ? surface->resource.image : texture ? texture->resource.image : direct_image;
  const auto format = surface ? surface->format : texture ? texture->format : direct_format;
  const auto layout = surface ? surface->layout : texture ? texture->layout : direct_layout;
  const uint32_t width = surface ? surface->width : texture ? texture->width : direct_width;
  const uint32_t height = surface ? surface->height : texture ? texture->height : direct_height;
  const auto samples = surface ? surface->samples : texture ? texture->samples : VK_SAMPLE_COUNT_1_BIT;
  if (!image || !width || !height || layout == VK_IMAGE_LAYOUT_UNDEFINED) { skip("undefined-image"); return; }
  if (samples != VK_SAMPLE_COUNT_1_BIT) { skip("multisample-needs-existing-resolve"); return; }
  if (texture && !(texture->usage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)) { skip("image-not-transfer-readable"); return; }
  auto bpt = PhoneProbeTexelBytes(format, aspect);
  if (aspect == VK_IMAGE_ASPECT_COLOR_BIT) {
    if (format == VK_FORMAT_BC1_RGBA_UNORM_BLOCK) bpt = 8;
    if (format == VK_FORMAT_BC2_UNORM_BLOCK || format == VK_FORMAT_BC3_UNORM_BLOCK) bpt = 16;
  }
  if (!bpt) { skip("unsupported-readback-format-cpu-texture-dump-retained"); return; }
  if (!InitializeContentProbeBuffer(&bulb_content_probe_buffer_)) { skip("readback-allocation"); return; }
  auto& buffer = bulb_content_probe_buffer_;
  auto free = std::find_if(buffer.stages.begin(), buffer.stages.end(), [](const auto& s) { return !s.valid && !s.reserved; });
  const size_t used=size_t(std::distance(buffer.stages.begin(),free));
  const bool final_checkpoint=role=="final-composite-input" || role=="final-composite-output" ||
                              role=="present-source" || role=="presenter-image";
  if (free == buffer.stages.end() || (!final_checkpoint && used>=buffer.stages.size()-8)) {
    skip("readback-budget-final-slots-reserved"); return;
  }
  const auto n = full_texture ? std::array<float,4>{0,0,1,1} : route.region.normalized;
  const uint32_t left = std::min(uint32_t(std::floor(n[0] * width)), width - 1);
  const uint32_t top = std::min(uint32_t(std::floor(n[1] * height)), height - 1);
  const uint32_t right = std::clamp(uint32_t(std::ceil(n[2] * width)), left + 1, width);
  const uint32_t bottom = std::clamp(uint32_t(std::ceil(n[3] * height)), top + 1, height);
  const VkRect2D rect{{int32_t(left),int32_t(top)},{right-left,bottom-top}};
  const uint32_t index = uint32_t(std::distance(buffer.stages.begin(), free));
  auto identity = std::make_shared<BulbProbeIdentity>();
  identity->run=bulb_run_; identity->source_sequence=route.sequence; identity->frame=diagnostic_submitted_frame_;
  identity->command_index=uint32_t(diagnostic_command_index_); identity->fixture=route.source->fixture;
  identity->image=NativeVulkanHandleIdentity(image); identity->bytes_per_texel=bpt;
  identity->rectangle={left,top,right-left,bottom-top}; identity->role=role;
  uint32_t handle=0,address=0;
  if (surface) {
    identity->lifetime=surface->lifetime_id; handle=surface->descriptor.handle; address=surface->descriptor.address;
    identity->writer=(aspect == VK_IMAGE_ASPECT_DEPTH_BIT ? surface->aspect_content.depth :
                      aspect == VK_IMAGE_ASPECT_STENCIL_BIT ? surface->aspect_content.stencil :
                      surface->aspect_content.color).writer_serial; MarkNativeSurfaceImageUsed(*surface);
  } else if (texture) {
    identity->lifetime=texture->descriptor_lifetime;
    identity->generation=texture->source?texture->source->generation:0;
    identity->writer=texture->aspect_content.color.writer_serial;
    handle=texture->source?texture->source->handle:0;
    address=texture->source?texture->source->info.memory.base_address:0;
    texture->last_used_frame=std::max(texture->last_used_frame,active_texture_frame_);
    texture->last_used_submission=std::max(texture->last_used_submission,submission_tracker_?submission_tracker_->GetCurrentSubmission():0);
    StageNativeTextureFlightResources(*texture);
  }
  if (!RecordContentProbeImage(cb,index,image,format,layout,width,height,handle,address,93,0,aspect,0,1,&buffer,&rect)) {
    skip("image-copy-recording"); return;
  }
  free->bulb=std::move(identity); buffer.pending_frame=1;
  REXLOG_INFO("gta4-bulb: point=probe-scheduled run={} frame={} cmd={} source-seq={} fixture={} role={} "
              "image={} lifetime={} generation={} roi={},{},{},{} format={} aspect={} gpu-complete=false",
      bulb_run_,diagnostic_submitted_frame_,diagnostic_command_index_,route.sequence,kBulbFixtures[route.source->fixture].role,
      role,free->bulb->image,free->bulb->lifetime,free->bulb->generation,left,top,right-left,bottom-top,uint32_t(format),uint32_t(aspect));
}

void Gta4NativeGraphicsSystem::RecordBulbBefore(VkCommandBuffer cb,const NativeCommand& command,const NativeRenderingTarget& target) {
  for (auto& route : bulb_routes_) if (route.sequence == command.diagnostic_submit_sequence) {
    RecordBulbImage(cb,route,"bulb-before",target.color_surfaces[0]);
    if (target.depth_stencil_attachment_active && target.depth_surface) {
      RecordBulbImage(cb,route,"bulb-depth-before",target.depth_surface,nullptr,VK_NULL_HANDLE,VK_FORMAT_UNDEFINED,VK_IMAGE_LAYOUT_UNDEFINED,0,0,VK_IMAGE_ASPECT_DEPTH_BIT);
      if (target.depth_surface->aspect & VK_IMAGE_ASPECT_STENCIL_BIT)
        RecordBulbImage(cb,route,"bulb-stencil-before",target.depth_surface,nullptr,VK_NULL_HANDLE,VK_FORMAT_UNDEFINED,VK_IMAGE_LAYOUT_UNDEFINED,0,0,VK_IMAGE_ASPECT_STENCIL_BIT);
    }
    if (command.textures[0]) {
      const auto image=native_texture_images_.find(command.textures[0]->generation);
      if (image!=native_texture_images_.end()) RecordBulbImage(cb,route,"bulb-texture-input",nullptr,image->second.get(),VK_NULL_HANDLE,VK_FORMAT_UNDEFINED,VK_IMAGE_LAYOUT_UNDEFINED,0,0,VK_IMAGE_ASPECT_COLOR_BIT,true);
    }
  }
}

void Gta4NativeGraphicsSystem::RecordBulbAfter(VkCommandBuffer cb,const NativeCommand& command,
                                             const NativeRenderingTarget& target,bool recorded) {
  for (auto& route : bulb_routes_) {
    if (!BulbRouteTouches(route,command,target)) continue;
    const bool original=route.sequence==command.diagnostic_submit_sequence;
    if (!original && route.writers>=EmissionTraceConfig().pipeline_writers) {
      if (!route.budget_reported) {
        route.budget_reported=true;
        REXLOG_INFO("gta4-bulb: point=writer-budget run={} frame={} fixture={} intervening-writer-capture=incomplete final-checkpoints=retained",
                    bulb_run_,diagnostic_submitted_frame_,kBulbFixtures[route.source->fixture].role);
      }
      continue;
    }
    const auto* ps=command.pipeline_state?command.pipeline_state->pixel_shader_resource:nullptr;
    REXLOG_INFO("gta4-bulb: point={} run={} frame={} cmd={} seq={} source-seq={} fixture={} recorded={} "
                "writer-type={} writer-shader={} target={:08X} color-mask={:08X} mapping=projected-screen-region",
                original?"original-result":"later-writer",bulb_run_,diagnostic_submitted_frame_,diagnostic_command_index_,
                command.diagnostic_submit_sequence,route.sequence,kBulbFixtures[route.source->fixture].role,recorded,
                CommandTypeName(command.type),ps?ps->filename:"none",target.color_surfaces[0]->descriptor.handle,target.color_write_mask);
    if (!recorded) continue;
    ++route.writers;
    if (route.writers == EmissionTraceConfig().pipeline_writers) {
      route.budget_reported = true;
      REXLOG_INFO("gta4-bulb: point=writer-budget run={} frame={} source-seq={} fixture={} intervening-writer-capture=incomplete final-checkpoints=retained",
                  bulb_run_,diagnostic_submitted_frame_,route.sequence,kBulbFixtures[route.source->fixture].role);
    }
    if (original) route.recorded=true;
    route.images.emplace(NativeVulkanHandleIdentity(target.color_surfaces[0]->resource.image),target.color_surfaces[0]->lifetime_id);
    RecordBulbImage(cb,route,original?"bulb-after":"after-writer",target.color_surfaces[0]);
  }
}

void Gta4NativeGraphicsSystem::TraceBulbResolve(VkCommandBuffer cb,const NativeCommand& command,
    NativeSurfaceImage* source,NativeTextureImage* destination,std::string_view result,std::string_view reason) {
  if (!bulb_trace_frame_ || !source) return;
  ResolveCommand resolve{};std::memcpy(&resolve,command.bytes.data(),sizeof(resolve));
  for(auto& route:bulb_routes_) {
    if(!route.images.contains({NativeVulkanHandleIdentity(source->resource.image),source->lifetime_id}))continue;
    REXLOG_INFO("gta4-bulb: point=resolve run={} frame={} cmd={} source-seq={} fixture={} result={} reason={} "
                "source={:08X} source-image={} source-lifetime={} destination={:08X} generation={} flags={:08X} level={} slice={} "
                "source-rect={}:{},{},{},{} destination-point={}:{},{}",
                bulb_run_,diagnostic_submitted_frame_,diagnostic_command_index_,route.sequence,kBulbFixtures[route.source->fixture].role,
                result,reason,source->descriptor.handle,NativeVulkanHandleIdentity(source->resource.image),source->lifetime_id,
                resolve.destination_texture,destination&&destination->source?destination->source->generation:0,resolve.flags,resolve.destination_level,
                resolve.destination_slice_or_face,resolve.source_rectangle_valid,resolve.source_rectangle.left,resolve.source_rectangle.top,
                resolve.source_rectangle.right,resolve.source_rectangle.bottom,resolve.destination_point_valid,resolve.destination_point.x,resolve.destination_point.y);
    if (result == "before") { RecordBulbImage(cb,route,"resolve-source-before",source); continue; }
    if(result!="ok" || !destination || !destination->source)continue;
    if(resolve.destination_level || resolve.destination_slice_or_face || resolve.source_rectangle_valid || resolve.destination_point_valid) {
      REXLOG_INFO("gta4-bulb: point=resolve-mapping run={} frame={} fixture={} mapping=requires-rectangle-review",bulb_run_,diagnostic_submitted_frame_,kBulbFixtures[route.source->fixture].role);
    }
    route.generations.insert(destination->source->generation);
    RecordBulbImage(cb,route,"resolved-texture",nullptr,destination);
  }
}

void Gta4NativeGraphicsSystem::RecordBulbPresentation(VkCommandBuffer cb,
    NativeSurfaceImage* final_surface,NativeTextureImage* composite_input,
    const std::shared_ptr<const NativeTextureResource>& present_source,VkImage image,VkImageLayout layout,uint32_t width,uint32_t height) {
  for(auto& route:bulb_routes_) {
    if(!route.recorded)continue;
    if(composite_input)RecordBulbImage(cb,route,"final-composite-input",nullptr,composite_input);
    if(final_surface)RecordBulbImage(cb,route,"final-composite-output",final_surface);
    if(present_source) {
      const auto found=native_texture_images_.find(present_source->generation);
      if(found!=native_texture_images_.end())RecordBulbImage(cb,route,"present-source",nullptr,found->second.get());
    }
    RecordBulbImage(cb,route,"presenter-image",nullptr,nullptr,image,ui::vulkan::VulkanPresenter::kGuestOutputFormat,layout,width,height);
  }
  if(bulb_content_probe_buffer_.pending_frame) {
    auto* provider=static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
    VkBufferMemoryBarrier b{};b.sType=VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    b.srcAccessMask=VK_ACCESS_TRANSFER_WRITE_BIT;b.dstAccessMask=VK_ACCESS_HOST_READ_BIT;
    b.srcQueueFamilyIndex=b.dstQueueFamilyIndex=VK_QUEUE_FAMILY_IGNORED;
    b.buffer=bulb_content_probe_buffer_.buffer;b.size=VK_WHOLE_SIZE;
    provider->vulkan_device()->functions().vkCmdPipelineBarrier(cb,VK_PIPELINE_STAGE_TRANSFER_BIT,VK_PIPELINE_STAGE_HOST_BIT,0,0,nullptr,1,&b,0,nullptr);
  }
}

void Gta4NativeGraphicsSystem::PublishBulbProbe(const NativeContentProbeStage& stage,const uint8_t* bytes,bool visible) {
  if(!stage.bulb)return;
  const auto& p=*stage.bulb;
  if(!visible || !bytes || !p.submission) {
    REXLOG_WARN("gta4-bulb: point=incomplete run={} frame={} source-seq={} fixture={} role={} reason=gpu-completion-unproven",
                p.run,p.frame,p.source_sequence,kBulbFixtures[p.fixture].role,p.role);return;
  }
  auto directory=EmissionTraceConfig().directory;
  if(directory.empty())directory=std::filesystem::temp_directory_path()/fmt::format("liberty-bulb-{}",p.run);
  std::error_code error;std::filesystem::create_directories(directory,error);
  if(error){REXLOG_WARN("gta4-bulb: point=incomplete reason=readback-directory");return;}
  const auto stem=fmt::format("bulb-r{}-f{}-s{}-c{}-{}",p.run,p.frame,p.source_sequence,p.command_index,p.role);
  std::vector<uint8_t> compact(size_t(kContentProbeSampleCount)*p.bytes_per_texel);
  for(uint32_t i=0;i<kContentProbeSampleCount;++i)
    std::memcpy(compact.data()+size_t(i)*p.bytes_per_texel,bytes+size_t(i)*kContentProbeSampleStride,p.bytes_per_texel);
  std::ofstream output(directory/(stem+".bin"),std::ios::binary);
  output.write(reinterpret_cast<const char*>(compact.data()),std::streamsize(compact.size()));output.close();
  if(!output){REXLOG_WARN("gta4-bulb: point=incomplete reason=readback-write");return;}
  const auto json=fmt::format("{{\"schema\":1,\"run\":{},\"frame\":{},\"source_sequence\":{},\"command_index\":{},"
      "\"fixture\":\"{}\",\"role\":\"{}\",\"image\":{},\"lifetime\":{},\"generation\":{},\"writer\":{},"
      "\"format\":{},\"aspect\":{},\"width\":{},\"height\":{},\"rectangle\":[{},{},{},{}],\"grid_axis\":{},"
      "\"bytes_per_texel\":{},\"submission\":{},\"gpu_completed\":true,\"mapping\":\"projected-screen-region\","
      "\"checksum\":\"{:016X}\",\"binary\":\"{}.bin\"}}\n",
      p.run,p.frame,p.source_sequence,p.command_index,kBulbFixtures[p.fixture].role,p.role,p.image,p.lifetime,p.generation,p.writer,
      uint32_t(stage.format),uint32_t(stage.aspect),stage.width,stage.height,p.rectangle[0],p.rectangle[1],p.rectangle[2],p.rectangle[3],
      kContentProbeAxis,p.bytes_per_texel,p.submission,XXH3_64bits(compact.data(),compact.size()),stem);
  std::ofstream metadata(directory/(stem+".json"));metadata<<json;metadata.close();
  REXLOG_INFO("gta4-bulb: point={} run={} frame={} cmd={} source-seq={} fixture={} role={} submission={} metadata={}",
      metadata?"gpu-complete":"incomplete",p.run,p.frame,p.command_index,p.source_sequence,kBulbFixtures[p.fixture].role,p.role,p.submission,(directory/(stem+".json")).string());
}

void Gta4NativeGraphicsSystem::AnalyzePendingBulbProbe(uint32_t slot,uint64_t submission) {
  auto& buffer=slot==active_frame_slot_?bulb_content_probe_buffer_:secondary_bulb_content_probe_buffer_;
  if(!buffer.pending_frame)return;
  auto* provider=static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const auto* device=provider?provider->vulkan_device():nullptr;
  bool visible=device&&buffer.mapping&&submission;
  for(const auto& stage:buffer.stages)if(stage.bulb&&(!stage.bulb->submission||stage.bulb->submission>submission))visible=false;
  if(visible&&!(device->memory_types().host_coherent&(uint32_t{1}<<buffer.memory_type))) {
    VkMappedMemoryRange range{};range.sType=VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;range.memory=buffer.memory;
    range.size=std::min(rex::round_up(kContentProbeBufferSize,device->properties().nonCoherentAtomSize),buffer.allocation_size);
    visible=device->functions().vkInvalidateMappedMemoryRanges(device->device(),1,&range)==VK_SUCCESS;
  }
  auto stages=std::make_shared<decltype(buffer.stages)>(std::move(buffer.stages));
  auto payload=std::make_shared<std::vector<uint8_t>>(size_t(kContentProbeBufferSize),0);
  if(visible)std::memcpy(payload->data(),buffer.mapping,payload->size());
  buffer.pending_frame=0;buffer.stages={};
  auto publish=[stages,payload,visible]() {
    for(uint32_t i=0;i<stages->size();++i)if((*stages)[i].bulb)
      PublishBulbProbe((*stages)[i],visible?payload->data()+size_t(i)*kContentProbeStageStride:nullptr,visible);
  };
  if(!EnqueueDeferredDiagnosticTask(publish))RunLightProbeAnalysisInline(publish);
}

void Gta4NativeGraphicsSystem::RecordBulbPostFx(VkCommandBuffer cb, const NativeCommand& command,
    NativeTextureImage* image, std::string_view role, bool recorded) {
  if (!bulb_trace_frame_ || !image || !image->source) return;
  for (auto& route : bulb_routes_) {
    if (!route.generations.contains(image->source->generation)) continue;
    REXLOG_INFO("gta4-bulb: point=postfx run={} frame={} cmd={} seq={} source-seq={} fixture={} role={} recorded={} image={} generation={} shader={}",
                bulb_run_,diagnostic_submitted_frame_,diagnostic_command_index_,command.diagnostic_submit_sequence,
                route.sequence,kBulbFixtures[route.source->fixture].role,role,recorded,
                NativeVulkanHandleIdentity(image->resource.image),image->source->generation,
                command.pipeline_state && command.pipeline_state->pixel_shader_resource ? command.pipeline_state->pixel_shader_resource->filename : "none");
    RecordBulbImage(cb,route,role,nullptr,image);
  }
}
