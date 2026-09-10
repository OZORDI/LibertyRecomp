#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include <rex/input/input.h>
#include <rex/ui/virtual_key.h>

namespace rex::input::mnk {

// Native keyboard/mouse mode writes GTA's PC-style action records directly.
// Some retail paths (pause, cutscenes and phone/frontend navigation) still
// observe controller buttons outside those records, however. This describes
// the configured controller meanings that must remain visible to those retail
// consumers. Stick directions are opt-in; mouse motion is never translated.
struct NativeControllerCompatibilityBindings {
  rex::ui::VirtualKey a = rex::ui::VirtualKey::kNone;
  // Optional second physical source for the platform's primary/Cross action.
  // LibertyRecomp uses this for Space while retaining Shift as GTA's A button.
  rex::ui::VirtualKey a_alias = rex::ui::VirtualKey::kNone;
  rex::ui::VirtualKey b = rex::ui::VirtualKey::kNone;
  // macOS Delete is Backspace; forward Delete is a separate physical source.
  rex::ui::VirtualKey b_alias = rex::ui::VirtualKey::kNone;
  rex::ui::VirtualKey x = rex::ui::VirtualKey::kNone;
  rex::ui::VirtualKey y = rex::ui::VirtualKey::kNone;
  rex::ui::VirtualKey dpad_up = rex::ui::VirtualKey::kNone;
  rex::ui::VirtualKey dpad_down = rex::ui::VirtualKey::kNone;
  rex::ui::VirtualKey dpad_left = rex::ui::VirtualKey::kNone;
  rex::ui::VirtualKey dpad_right = rex::ui::VirtualKey::kNone;
  rex::ui::VirtualKey start = rex::ui::VirtualKey::kNone;
  rex::ui::VirtualKey back = rex::ui::VirtualKey::kNone;
  rex::ui::VirtualKey left_shoulder = rex::ui::VirtualKey::kNone;
  rex::ui::VirtualKey right_shoulder = rex::ui::VirtualKey::kNone;
  rex::ui::VirtualKey left_trigger = rex::ui::VirtualKey::kNone;
  rex::ui::VirtualKey right_trigger = rex::ui::VirtualKey::kNone;
  rex::ui::VirtualKey left_stick_up = rex::ui::VirtualKey::kNone;
  rex::ui::VirtualKey left_stick_down = rex::ui::VirtualKey::kNone;
  rex::ui::VirtualKey left_stick_left = rex::ui::VirtualKey::kNone;
  rex::ui::VirtualKey left_stick_right = rex::ui::VirtualKey::kNone;

  bool operator==(const NativeControllerCompatibilityBindings&) const = default;
};

void SetNativeControllerCompatibilityBindings(
    const NativeControllerCompatibilityBindings& bindings);
NativeControllerCompatibilityBindings GetNativeControllerCompatibilityBindings();

// Immutable title-poll snapshot. An inactive source is not a connected device.
void PublishVirtualControllerCompatibilityKeys(uint32_t user_index,
    const std::array<uint8_t, 256>& keys, bool active);
bool ReadVirtualControllerCompatibilityGamepad(uint32_t user_index, X_INPUT_GAMEPAD& gamepad);

// Produces digital buttons, key-bound triggers and optional left-stick axes.
// Opposing directions cancel; unbound axes and the right stick stay neutral.
X_INPUT_GAMEPAD BuildNativeControllerCompatibilityGamepad(
    const NativeControllerCompatibilityBindings& bindings,
    const bool* key_down, size_t key_count);

}  // namespace rex::input::mnk
