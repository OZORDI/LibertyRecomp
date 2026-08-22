#pragma once

#include <cstdint>

namespace rex::input::mnk {

// GTA IV stores an action's logical magnitude as the XOR of a per-action
// polarity byte and its current encoded byte. Negative and positive halves of
// an axis consequently use different encoded bytes for the same magnitude.
constexpr uint8_t DecodeActionMagnitude(uint8_t polarity,
                                        uint8_t encoded_value) noexcept {
  return static_cast<uint8_t>(polarity ^ encoded_value);
}

constexpr uint8_t EncodeActionMagnitude(uint8_t polarity,
                                        uint8_t magnitude) noexcept {
  return static_cast<uint8_t>(polarity ^ magnitude);
}

constexpr uint8_t MergeActionMagnitude(uint8_t polarity,
                                       uint8_t encoded_value,
                                       uint8_t requested_magnitude) noexcept {
  const uint8_t current_magnitude =
      DecodeActionMagnitude(polarity, encoded_value);
  return requested_magnitude > current_magnitude
             ? EncodeActionMagnitude(polarity, requested_magnitude)
             : encoded_value;
}

}  // namespace rex::input::mnk
