// Inside BindCommonDrawState, after the actual immutable constants are uploaded.
// Byte comparisons use mapped upload memory, not a substitute calculated buffer.
if (!EmissionTraceConfig().pipeline && EmissionTraceActive(diagnostic_submitted_frame_) && command.pipeline_state &&
    command.pipeline_state->pixel_shader_resource &&
    IsEmissionShader(command.pipeline_state->pixel_shader_resource->filename)) {
  const auto& ps = *command.pipeline_state->pixel_shader_resource;
  DrawIndexedPrimitiveCommand indexed{};
  if (command.type == CommandType::kDrawIndexedPrimitive)
    std::memcpy(&indexed, command.bytes.data(), sizeof(indexed));
  const auto vb_hash = command.vertex_buffers[0] ? command.vertex_buffers[0]->content_hash : 0;
  const auto texture_hash = command.textures[0] ? command.textures[0]->content_hash : 0;
  const std::array<uint64_t, 6> identity = {ps.hash, vb_hash, texture_hash, indexed.index_count,
                                         target.width, target.height};
  const auto signature = XXH3_64bits(identity.data(), sizeof(identity));
  struct Observation { uint32_t frame = 0; uint32_t count = 0; };
  static std::unordered_map<uint64_t, Observation> observed; // One render worker.
  static uint32_t total = 0;
  if (total < 512 && (observed.contains(signature) || observed.size() < 128)) {
    auto& entry = observed[signature];
    if (!entry.count || (entry.count < 4 && diagnostic_submitted_frame_ >= entry.frame + 120)) {
      ++total;
      const bool first = entry.count++ == 0;
      entry.frame = diagnostic_submitted_frame_;
      std::string constants;
      bool upload_equal = pixel_constants_allocation.mapping != nullptr;
      if (upload_equal) {
        for (size_t offset = 0; offset < pixel_constants->size(); offset += sizeof(uint32_t)) {
          uint32_t actual = 0;
          std::memcpy(&actual, pixel_constants_allocation.mapping + offset, sizeof(actual));
          if (actual != LoadGuestWord(*pixel_constants, offset)) { upload_equal = false; break; }
        }
      }
      for (uint32_t reg : {39u,45u,46u,51u,208u,209u,210u}) {
        constants += fmt::format(" c{}=[", reg);
        for (uint32_t component = 0; component < 4; ++component) {
          const auto word = LoadGuestWord(*pixel_constants, size_t(reg) * 16 + component * 4);
          uint32_t uploaded = 0;
          if (pixel_constants_allocation.mapping)
            std::memcpy(&uploaded, pixel_constants_allocation.mapping + size_t(reg) * 16 + component * 4, 4);
          constants += fmt::format("{}{:08X}:{:.9g}:{:08X}", component ? "," : "", word,
                                   std::bit_cast<float>(word), uploaded);
        }
        constants += "]";
      }
      const auto output = NativeColorOutput(command.pipeline_state->render_targets[0].address);
      REXLOG_INFO("gta4-emission: point=bind seq={} epoch={} frame={} cmd={} draw={:016X} signature={:016X} "
                  "ps={:016X} vs={:016X} shader={} indices={} start={} base={} "
                  "rt={:08X}/{:08X} format={} extent={}x{} samples={} "
                  "texture={:016X} vb={:016X} upload-mapped={} upload-equal={} "
                  "pc-version={} gpu-buffer={} gpu-offset={} "
                  "z={},{},{} stencil={},{},{} alpha={},{},{} coverage={} cull={} "
                  "blend={},{},{},{} alpha-blend={},{},{} color-mask={:08X} "
                  "output-scale={},{},{},{} pbool={:08X} vbool={:08X} constants:{}",
                  command.diagnostic_submit_sequence, command.diagnostic_producer_epoch,
                  diagnostic_submitted_frame_, diagnostic_command_index_, diagnostic_draw_id_, signature,
                  ps.hash, bound_vertex_shader_hash, ps.filename, indexed.index_count,
                  indexed.start_index, indexed.base_vertex,
                  command.pipeline_state->render_targets[0].handle, command.pipeline_state->render_targets[0].address,
                  target.color_surfaces[0] ? uint32_t(target.color_surfaces[0]->format) : 0,
                  width, height, uint32_t(samples), texture_hash, vb_hash,
                  pixel_constants_allocation.mapping != nullptr, upload_equal,
                  command.pixel_constants_hash, fmt::ptr(pixel_constants_allocation.buffer),
                  pixel_constants_allocation.offset, fixed.depth_enable, fixed.depth_function,
                  fixed.depth_write_enable, fixed.stencil_enable, fixed.stencil_function, fixed.stencil_reference,
                  fixed.alpha_test_enable, fixed.alpha_function, fixed.alpha_reference, fixed.alpha_to_mask,
                  fixed.cull_mode, fixed.blend_enable, fixed.source_blend, fixed.destination_blend, fixed.blend_operation,
                  fixed.source_blend_alpha, fixed.destination_blend_alpha, fixed.blend_operation_alpha,
                  fixed.color_write_mask, output.scale[0], output.scale[1], output.scale[2], output.scale[3],
                  command.shader_state->pixel_booleans, command.shader_state->vertex_booleans, constants);
      const auto& directory = EmissionTraceConfig().directory;
      if (first && !directory.empty()) {
        std::error_code error;
        std::filesystem::create_directories(directory, error);
        const std::string prefix = fmt::format("emission-{:016X}", signature);
        auto dump = [&](std::string_view suffix, const auto& bytes) {
          const auto path = directory / (prefix + std::string(suffix));
          std::ofstream file(path, std::ios::binary | std::ios::trunc);
          if (file) file.write(reinterpret_cast<const char*>(bytes.data()), std::streamsize(bytes.size()));
          REXLOG_INFO("gta4-emission: point=artifact signature={:016X} suffix={} bytes={} result={}",
                      signature, suffix, bytes.size(), file.good() ? "written" : "failed");
        };
        dump("-vs-guest.bin", *vertex_constants);
        dump("-ps-guest.bin", *pixel_constants);
        if (pixel_constants_allocation.mapping)
          dump("-ps-upload.bin", std::span<const uint8_t>(pixel_constants_allocation.mapping, pixel_constants->size()));
        for (uint32_t stream = 0; stream < command.vertex_buffers.size(); ++stream) {
          const auto& resource = command.vertex_buffers[stream];
          if (!resource) continue;
          const auto& binding = command.pipeline_state->vertex_streams[stream];
          dump(fmt::format("-vb{}.bin", stream), resource->payload);
          REXLOG_INFO("gta4-emission: point=stream signature={:016X} stream={} handle={:08X} "
                      "generation={} hash={:016X} offset={} stride={} bytes={}", signature, stream,
                      resource->handle, resource->generation, resource->content_hash, binding.offset,
                      binding.stride, resource->payload.size());
        }
        if (command.index_buffer) {
          dump("-ib.bin", command.index_buffer->payload);
          REXLOG_INFO("gta4-emission: point=index signature={:016X} flags={:08X} bytes={}",
                      signature, command.index_buffer->flags, command.index_buffer->payload.size());
        }
        if (const auto& declaration = command.pipeline_state->vertex_declaration_resource) {
          for (const auto& e : declaration->elements)
            REXLOG_INFO("gta4-emission: point=element signature={:016X} stream={} offset={} type={} usage={} index={}",
                        signature, e.stream, e.offset, uint32_t(e.type), uint32_t(e.usage), e.usage_index);
        }
        for (uint32_t slot = 0; slot < command.textures.size(); ++slot) {
          if (!(command.used_texture_mask & (1u << slot)) || !command.textures[slot]) continue;
          const auto& texture = *command.textures[slot];
          dump(fmt::format("-texture{}.bin", slot), texture.payload);
          REXLOG_INFO("gta4-emission: point=texture signature={:016X} slot={} handle={:08X} generation={} "
                      "hash={:016X} format={} width={} height={} sign={},{},{},{} swizzle={} endian={} "
                      "required={:08X} realized={:08X} failed={:08X}",
                      signature, slot, texture.handle, texture.generation, texture.content_hash,
                      uint32_t(texture.info.format), texture.info.width + 1, texture.info.height + 1,
                      uint32_t(texture.fetch.sign_x), uint32_t(texture.fetch.sign_y),
                      uint32_t(texture.fetch.sign_z), uint32_t(texture.fetch.sign_w),
                      uint32_t(texture.fetch.swizzle), uint32_t(texture.fetch.endianness),
                      command.used_texture_mask, command.realized_image_mask, command.failed_texture_mask);
          for (const auto& mip : texture.mip_levels)
            REXLOG_INFO("gta4-emission: point=mip signature={:016X} slot={} level={} width={} height={} "
                        "offset={} size={} row-length={} image-height={}", signature, slot, mip.level,
                        mip.width, mip.height, mip.payload_offset, mip.payload_size, mip.buffer_row_length,
                        mip.buffer_image_height);
        }
      }
    }
  }
}
