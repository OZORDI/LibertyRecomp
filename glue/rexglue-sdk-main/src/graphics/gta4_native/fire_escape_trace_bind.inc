// Executed after the original draw's immutable constants and dynamic state are prepared.
if (fire_event_active_ && command.pipeline_state) {
  const auto* ps = command.pipeline_state->pixel_shader_resource;
  const auto* vs = command.pipeline_state->vertex_shader_resource;
  auto upload_equal = [](const std::vector<uint8_t>* original, const NativeUploadAllocation& uploaded) {
    if (!original || !uploaded.mapping) return false;
    for (size_t offset = 0; offset < original->size(); offset += sizeof(uint32_t)) {
      uint32_t actual = 0; std::memcpy(&actual, uploaded.mapping + offset, sizeof(actual));
      if (actual != LoadGuestWord(*original, offset)) return false;
    }
    return true;
  };
  const auto overrides = ResolvePipelineShaderOverrides(vs, ps, samples);
  const bool coverage = IsNativeFragmentCoverageRequested(
      IsAlphaTestRequested(fixed.alpha_test_enable, fixed.alpha_function), fixed.alpha_to_mask);
  const uint32_t variant = (overrides.pixel_override ? 2u : 0u) + (coverage ? 1u : 0u);
  FireTraceLog("gpu-bind", fmt::format(
      "frame={} cmd={} seq={} pipeline={} vs={:016X} ps={:016X} ps-variant={} module-hash={:016X} override={} late={} "
      "logical={}x{} physical={}x{} samples={} viewport={},{},{},{},{},{} scissor={},{},{},{} "
      "vs-upload-equal={} ps-upload-equal={} vs-gpu-buffer={} vs-offset={} ps-gpu-buffer={} ps-offset={} shared-buffer={} shared-offset={} "
      "pbool={:08X} vbool={:08X} descriptor-copy={} image-epoch={} sampler-epoch={}",
      diagnostic_submitted_frame_, diagnostic_command_index_, command.diagnostic_submit_sequence,
      NativeVulkanHandleIdentity(pipeline), vs ? vs->hash : 0, ps ? ps->hash : 0, variant,
      ps ? ps->module_code_hashes[variant] : 0, overrides.pixel_override, coverage,
      logical_width, logical_height, width, height, uint32_t(samples),
      viewport.x, viewport.y, viewport.width, viewport.height, viewport.minDepth, viewport.maxDepth,
      scissor.offset.x, scissor.offset.y, scissor.extent.width, scissor.extent.height,
      upload_equal(vertex_constants, vertex_constants_allocation), upload_equal(pixel_constants, pixel_constants_allocation),
      NativeVulkanHandleIdentity(vertex_constants_allocation.buffer), vertex_constants_allocation.offset,
      NativeVulkanHandleIdentity(pixel_constants_allocation.buffer), pixel_constants_allocation.offset,
      NativeVulkanHandleIdentity(shared_constants_allocation.buffer), shared_constants_allocation.offset,
      command.shader_state->pixel_booleans, command.shader_state->vertex_booleans,
      command.descriptor_copy, command.image_descriptor_epoch, command.sampler_descriptor_epoch));
  FireInputs(VK_NULL_HANDLE, command);
  const auto dump = [&](std::string_view role, const uint8_t* bytes, size_t size) {
    if (!bytes || !size || FireTraceConfig().directory.empty()) return;
    constexpr uint64_t limit = uint64_t{64} * 1024 * 1024;
    if (size > limit || fire_cpu_bytes_ > limit - size) {
      FireTraceLog("artifact-unavailable", "reason=cpu-artifact-budget"); return;
    }
    auto path = std::filesystem::path(FireTraceConfig().directory) /
        fmt::format("cpu-f{}-s{}-{}.bin", diagnostic_submitted_frame_, command.diagnostic_submit_sequence, role);
    std::ofstream output(path, std::ios::binary);
    output.write(reinterpret_cast<const char*>(bytes), std::streamsize(size)); output.close();
    if (output) fire_cpu_bytes_ += size;
    FireTraceLog("cpu-artifact", fmt::format("frame={} seq={} role={} bytes={} checksum={:016X} written={} path={}",
        diagnostic_submitted_frame_, command.diagnostic_submit_sequence, role, size,
        XXH3_64bits(bytes, size), bool(output), path.string()));
  };
  if (command.fire_trace || (fire_images_ && ps && fire_checkpoints_["constants-" + std::to_string(ps->hash)]++ == 0)) {
    dump("vs-guest", vertex_constants->data(), vertex_constants->size());
    dump("ps-guest", pixel_constants->data(), pixel_constants->size());
    dump("vs-upload", vertex_constants_allocation.mapping, vertex_constants->size());
    dump("ps-upload", pixel_constants_allocation.mapping, pixel_constants->size());
    dump("shared-upload", shared_constants_allocation.mapping, sizeof(NativeSharedConstants));
    if (command.fire_trace) {
      for (uint32_t stream = 0; stream < command.vertex_buffers.size(); ++stream) {
        const auto& r = command.vertex_buffers[stream]; if (!r) continue;
        const auto& b = command.pipeline_state->vertex_streams[stream];
        FireTraceLog("vertex-buffer", fmt::format("seq={} stream={} handle={:08X} generation={} hash={:016X} offset={} stride={} bytes={}",
            command.diagnostic_submit_sequence, stream, r->handle, r->generation, r->content_hash, b.offset, b.stride, r->payload.size()));
        if (fire_cpu_resources_.insert(r->content_hash).second)
          dump(fmt::format("vb{}", stream), r->payload.data(), r->payload.size());
      }
      if (const auto& r = command.index_buffer) {
        FireTraceLog("index-buffer", fmt::format("seq={} handle={:08X} generation={} hash={:016X} flags={:08X} bytes={}",
            command.diagnostic_submit_sequence, r->handle, r->generation, r->content_hash, r->flags, r->payload.size()));
        if (fire_cpu_resources_.insert(r->content_hash).second) dump("ib", r->payload.data(), r->payload.size());
      }
      if (const auto& decl = command.pipeline_state->vertex_declaration_resource)
        for (const auto& e : decl->elements)
          FireTraceLog("vertex-element", fmt::format("seq={} stream={} offset={} type={} usage={} usage-index={}",
              command.diagnostic_submit_sequence, e.stream, e.offset, e.type, e.usage, e.usage_index));
    }
  }
}
