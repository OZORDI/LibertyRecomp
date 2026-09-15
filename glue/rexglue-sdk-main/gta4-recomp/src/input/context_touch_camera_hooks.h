#pragma once

#include <cstdint>
#include <limits>

namespace gta4::input {

// These two calls in generated sub_825ED3C8 occur after its FPS weapon
// camera, zoom range and camera-state checks. Other signed action decoders
// must retain their original behavior.
constexpr uint32_t TouchScopedZoomActionForCaller(uint32_t caller) noexcept {
  if (caller == 0x825ED638) return 26;
  if (caller == 0x825ED65C) return 24;
  return std::numeric_limits<uint32_t>::max();
}

}  // namespace gta4::input
