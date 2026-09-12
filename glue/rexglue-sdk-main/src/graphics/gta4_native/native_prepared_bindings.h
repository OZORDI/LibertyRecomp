#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace rex::graphics::gta4_native {
// This is used only inside one indexed preparation batch. Original commands
// retain their resources; pages are not repurposed until their slot fence.
// Compare guest-null meaning as well as resource identity and all fetch words.
template <typename Command>
bool NativePreparedTextureInputsEqual(const Command& previous, const Command& next) {
  if (!previous.bindings_prepared || previous.failed_texture_mask || !previous.pipeline_state ||
      !next.pipeline_state || previous.used_texture_mask != next.used_texture_mask) return false;
  for (size_t stage = 0; stage < previous.textures.size(); ++stage) {
    if (!(next.used_texture_mask & (uint32_t{1} << stage))) continue;
    if (previous.pipeline_state->textures[stage] != next.pipeline_state->textures[stage] ||
        previous.textures[stage] != next.textures[stage] ||
        std::memcmp(&previous.texture_fetches[stage], &next.texture_fetches[stage],
                    sizeof(previous.texture_fetches[stage])) != 0) return false;
  }
  return true;
}
template <typename Command>
void CopyNativePreparedTextureBindings(const Command& previous, Command& next) {
  next.descriptor_page=previous.descriptor_page; next.descriptor_copy=previous.descriptor_copy;
  next.image_descriptor_epoch=previous.image_descriptor_epoch;
  next.sampler_descriptor_epoch=previous.sampler_descriptor_epoch;
  next.cached_descriptor_epoch=previous.cached_descriptor_epoch;
  next.texture_descriptor_indices=previous.texture_descriptor_indices;
  next.sampler_descriptor_indices=previous.sampler_descriptor_indices;
  next.draw_descriptor_sets=previous.draw_descriptor_sets;
  next.binding_realization=previous.binding_realization;
  next.realized_image_mask=previous.realized_image_mask;
  next.realized_sampler_mask=previous.realized_sampler_mask;
  next.guest_null_texture_mask=previous.guest_null_texture_mask;
  next.failed_texture_mask=0; next.bindings_prepared=true;
  // Never copy diagnostic-owned objects from another draw.
  next.room_light_input_bindings.reset();
}
}  // namespace rex::graphics::gta4_native
