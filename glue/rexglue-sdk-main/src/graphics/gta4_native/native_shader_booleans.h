#pragma once
#include <cstdint>

namespace rex::graphics::gta4_native {
// Existing SharedConstants ABI: vertex b0..b15 and pixel b0..b15. The latter
// use absolute microcode addresses b128..b143. Vertex b8/b11 are title inputs,
// not padding: discarding them changes the selected material shader branch.
constexpr uint32_t PackNativeShaderBooleans(uint32_t vertex, uint32_t pixel) {
  return (vertex & 0xFFFFu) | ((pixel & 0xFFFFu) << 16);
}
}  // namespace rex::graphics::gta4_native
