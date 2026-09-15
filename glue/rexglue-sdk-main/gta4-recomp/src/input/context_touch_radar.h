#pragma once

#include <cstdint>
#include "gta4_aspect_policy.h"

namespace gta4::input {
struct TouchRadarPass {
  gta4::aspect::Rect bounds{};
  bool gameplay = false;
};

// The render phase queues a private viewport before its radar draw. Consume
// that exact copy so hit testing shares the draw's rounded viewport bounds.
TouchRadarPass ConsumeTouchRadarPass(uint8_t* base, uint32_t phase) noexcept;
// Radar vertices are local to this viewport; screen layout must not be applied
// a second time while the copied viewport is bound on the render thread.
bool TouchRadarLocalViewport(uint8_t* base) noexcept;
}  // namespace gta4::input
