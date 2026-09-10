#include <rex/input/mnk/controller_compatibility.h>

#include <limits>
#include <mutex>

namespace rex::input::mnk {
namespace {

std::mutex g_bindings_mutex;
NativeControllerCompatibilityBindings g_bindings;
std::array<uint8_t, 256> g_virtual_keys{};
uint32_t g_virtual_user = 0;
bool g_virtual_active = false;

bool IsBindingPressed(rex::ui::VirtualKey key, const bool* key_down,
                      size_t key_count) {
  const auto index = static_cast<size_t>(key);
  return key != rex::ui::VirtualKey::kNone && key_down && index < key_count &&
         key_down[index];
}

}  // namespace

void SetNativeControllerCompatibilityBindings(
    const NativeControllerCompatibilityBindings& bindings) {
  std::lock_guard lock(g_bindings_mutex);
  g_bindings = bindings;
}

NativeControllerCompatibilityBindings GetNativeControllerCompatibilityBindings() {
  std::lock_guard lock(g_bindings_mutex);
  return g_bindings;
}

void PublishVirtualControllerCompatibilityKeys(uint32_t user_index,
    const std::array<uint8_t, 256>& keys, bool active) {
  std::lock_guard lock(g_bindings_mutex);
  g_virtual_user = user_index;
  g_virtual_active = active;
  g_virtual_keys = active ? keys : std::array<uint8_t, 256>{};
}

bool ReadVirtualControllerCompatibilityGamepad(uint32_t user_index, X_INPUT_GAMEPAD& gamepad) {
  std::lock_guard lock(g_bindings_mutex);
  gamepad = {};
  if (!g_virtual_active || user_index != g_virtual_user) return false;
  bool keys[256]{};
  for (size_t i = 0; i < g_virtual_keys.size(); ++i) keys[i] = g_virtual_keys[i] != 0;
  gamepad = BuildNativeControllerCompatibilityGamepad(g_bindings, keys, std::size(keys));
  return true;
}

X_INPUT_GAMEPAD BuildNativeControllerCompatibilityGamepad(
    const NativeControllerCompatibilityBindings& bindings,
    const bool* key_down, size_t key_count) {
  X_INPUT_GAMEPAD gamepad = {};
  auto add_button = [&](rex::ui::VirtualKey key, uint16_t button) {
    if (IsBindingPressed(key, key_down, key_count)) {
      gamepad.buttons = static_cast<uint16_t>(gamepad.buttons) | button;
    }
  };

  add_button(bindings.a, X_INPUT_GAMEPAD_A);
  add_button(bindings.a_alias, X_INPUT_GAMEPAD_A);
  add_button(bindings.b, X_INPUT_GAMEPAD_B);
  add_button(bindings.b_alias, X_INPUT_GAMEPAD_B);
  add_button(bindings.x, X_INPUT_GAMEPAD_X);
  add_button(bindings.y, X_INPUT_GAMEPAD_Y);
  add_button(bindings.dpad_up, X_INPUT_GAMEPAD_DPAD_UP);
  add_button(bindings.dpad_down, X_INPUT_GAMEPAD_DPAD_DOWN);
  add_button(bindings.dpad_left, X_INPUT_GAMEPAD_DPAD_LEFT);
  add_button(bindings.dpad_right, X_INPUT_GAMEPAD_DPAD_RIGHT);
  add_button(bindings.start, X_INPUT_GAMEPAD_START);
  add_button(bindings.back, X_INPUT_GAMEPAD_BACK);
  add_button(bindings.left_shoulder, X_INPUT_GAMEPAD_LEFT_SHOULDER);
  add_button(bindings.right_shoulder, X_INPUT_GAMEPAD_RIGHT_SHOULDER);

  gamepad.left_trigger =
      IsBindingPressed(bindings.left_trigger, key_down, key_count) ? 0xFF : 0;
  gamepad.right_trigger =
      IsBindingPressed(bindings.right_trigger, key_down, key_count) ? 0xFF : 0;
  auto axis = [&](rex::ui::VirtualKey negative, rex::ui::VirtualKey positive) -> int16_t {
    const bool negative_down = IsBindingPressed(negative, key_down, key_count);
    const bool positive_down = IsBindingPressed(positive, key_down, key_count);
    if (negative_down == positive_down) {
      return 0;
    }
    constexpr int16_t full_scale = std::numeric_limits<int16_t>::max();
    return positive_down ? full_scale : static_cast<int16_t>(-full_scale);
  };
  gamepad.thumb_lx = axis(bindings.left_stick_left, bindings.left_stick_right);
  gamepad.thumb_ly = axis(bindings.left_stick_down, bindings.left_stick_up);
  return gamepad;
}

}  // namespace rex::input::mnk
