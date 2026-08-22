#ifndef REX_GRAPHICS_GTA4_NATIVE_NATIVE_FRAME_SCHEDULING_H_
#define REX_GRAPHICS_GTA4_NATIVE_NATIVE_FRAME_SCHEDULING_H_

#include <cstdint>

namespace rex::graphics::gta4_native {

// Native texture images are destroyed only after going unreferenced for a
// full grace window to avoid repeatedly destroying and recreating resources
// that the title cycles across frames.
inline constexpr uint32_t kNativeTextureEvictionGraceFrames = 240;

constexpr bool ShouldEvictNativeTexture(uint32_t current_frame,
                                        uint32_t last_used_frame,
                                        uint32_t grace_frames) {
  const uint32_t age =
      current_frame >= last_used_frame ? current_frame - last_used_frame : 0u;
  return age > grace_frames;
}

}  // namespace rex::graphics::gta4_native

#endif  // REX_GRAPHICS_GTA4_NATIVE_NATIVE_FRAME_SCHEDULING_H_
