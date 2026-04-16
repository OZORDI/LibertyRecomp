/**
 * PS4 input driver — maps DualShock 4 / DualSense input via scePad into the
 * Xbox 360-shaped X_INPUT_* structures the rest of the runtime expects.
 */

#include <rex/platform.h>

#if REX_PLATFORM_PS4

#include "ps4_input_driver.h"

#include <algorithm>
#include <cstring>

#include <orbis/Pad.h>
#include <orbis/UserService.h>

#include <rex/input/flags.h>
#include <rex/input/input.h>
#include <rex/logging.h>

namespace rex::input::ps4 {

namespace {

// Expand an unsigned [0..255] stick axis to signed [-32768..32767] Xbox range.
// 0x80 (128) is the neutral midpoint on DS4; output 0 there.
inline int16_t ExpandStickAxis(uint8_t v) {
  int32_t centered = static_cast<int32_t>(v) - 128;
  // centered is in [-128, 127]; scale by 258 to reach ~[-32768, 32767].
  int32_t scaled = centered * 258;
  if (scaled > 32767) scaled = 32767;
  if (scaled < -32768) scaled = -32768;
  return static_cast<int16_t>(scaled);
}

}  // namespace

PS4InputDriver::PS4InputDriver(rex::ui::Window* window, size_t window_z_order)
    : InputDriver(window, window_z_order) {}

PS4InputDriver::~PS4InputDriver() {
  std::lock_guard<std::mutex> lock(mutex_);
  for (auto& slot : slots_) {
    if (slot.handle >= 0) {
      scePadClose(slot.handle);
      slot.handle = -1;
    }
  }
}

X_STATUS PS4InputDriver::Setup() {
  OrbisUserServiceInitializeParams params{};
  params.priority = ORBIS_KERNEL_PRIO_FIFO_LOWEST;
  int32_t rc = sceUserServiceInitialize(&params);
  // 0 = success; non-zero may mean already-initialized — treat as non-fatal.
  if (rc == 0) {
    user_service_initialized_ = true;
  } else {
    REXLOG_WARN("PS4: sceUserServiceInitialize rc=0x{:08X}", rc);
  }

  rc = scePadInit();
  if (rc != 0) {
    REXLOG_ERROR("PS4: scePadInit failed rc=0x{:08X}", rc);
    return X_STATUS_UNSUCCESSFUL;
  }
  pad_initialized_ = true;

  RefreshUsers();
  REXLOG_INFO("PS4 input driver initialized");
  return X_STATUS_SUCCESS;
}

void PS4InputDriver::RefreshUsers() {
  if (!pad_initialized_) return;

  OrbisUserServiceLoginUserIdList list{};
  for (auto& id : list.userId) id = ORBIS_USER_SERVICE_USER_ID_INVALID;
  sceUserServiceGetLoginUserIdList(&list);

  std::lock_guard<std::mutex> lock(mutex_);

  // Close pads whose user is no longer logged in.
  for (auto& slot : slots_) {
    if (slot.user_id == ORBIS_USER_SERVICE_USER_ID_INVALID) continue;
    bool still_logged_in = false;
    for (auto uid : list.userId) {
      if (uid == slot.user_id) {
        still_logged_in = true;
        break;
      }
    }
    if (!still_logged_in) {
      if (slot.handle >= 0) scePadClose(slot.handle);
      slot = {};
    }
  }

  // Open pads for newly-seen users.
  for (size_t li = 0; li < ORBIS_USER_SERVICE_MAX_LOGIN_USERS && li < kPS4UserCount; ++li) {
    int32_t uid = list.userId[li];
    if (uid == ORBIS_USER_SERVICE_USER_ID_INVALID) continue;

    bool already_have = false;
    for (auto& slot : slots_) {
      if (slot.user_id == uid) {
        already_have = true;
        break;
      }
    }
    if (already_have) continue;

    // Find a free slot.
    for (size_t si = 0; si < slots_.size(); ++si) {
      auto& slot = slots_[si];
      if (slot.user_id != ORBIS_USER_SERVICE_USER_ID_INVALID) continue;
      int32_t handle =
          scePadOpen(uid, ORBIS_PAD_PORT_TYPE_STANDARD, 0, nullptr);
      if (handle < 0) {
        REXLOG_WARN("PS4: scePadOpen(uid={}) failed rc=0x{:08X}", uid, handle);
        break;
      }
      slot.user_id = uid;
      slot.handle = handle;
      slot.connected = true;
      slot.packet_number = 1;
      REXLOG_INFO("PS4: opened pad for user {} at slot {} (handle={})", uid,
                  si, handle);
      break;
    }
  }
}

void PS4InputDriver::TranslatePadState(const OrbisPadData& pad,
                                       X_INPUT_GAMEPAD& out) {
  uint16_t xb = 0;
  uint32_t b = pad.buttons;
  if (b & ORBIS_PAD_BUTTON_UP) xb |= X_INPUT_GAMEPAD_DPAD_UP;
  if (b & ORBIS_PAD_BUTTON_DOWN) xb |= X_INPUT_GAMEPAD_DPAD_DOWN;
  if (b & ORBIS_PAD_BUTTON_LEFT) xb |= X_INPUT_GAMEPAD_DPAD_LEFT;
  if (b & ORBIS_PAD_BUTTON_RIGHT) xb |= X_INPUT_GAMEPAD_DPAD_RIGHT;
  if (b & ORBIS_PAD_BUTTON_OPTIONS) xb |= X_INPUT_GAMEPAD_START;
  if (b & ORBIS_PAD_BUTTON_TOUCH_PAD) xb |= X_INPUT_GAMEPAD_BACK;
  if (b & ORBIS_PAD_BUTTON_L3) xb |= X_INPUT_GAMEPAD_LEFT_THUMB;
  if (b & ORBIS_PAD_BUTTON_R3) xb |= X_INPUT_GAMEPAD_RIGHT_THUMB;
  if (b & ORBIS_PAD_BUTTON_L1) xb |= X_INPUT_GAMEPAD_LEFT_SHOULDER;
  if (b & ORBIS_PAD_BUTTON_R1) xb |= X_INPUT_GAMEPAD_RIGHT_SHOULDER;
  if (b & ORBIS_PAD_BUTTON_CROSS) xb |= X_INPUT_GAMEPAD_A;
  if (b & ORBIS_PAD_BUTTON_CIRCLE) xb |= X_INPUT_GAMEPAD_B;
  if (b & ORBIS_PAD_BUTTON_SQUARE) xb |= X_INPUT_GAMEPAD_X;
  if (b & ORBIS_PAD_BUTTON_TRIANGLE) xb |= X_INPUT_GAMEPAD_Y;
  out.buttons = xb;

  // L2/R2 analog triggers come through analogButtons, 0..255.
  out.left_trigger = pad.analogButtons.l2;
  out.right_trigger = pad.analogButtons.r2;

  out.thumb_lx = ExpandStickAxis(pad.leftStick.x);
  // DS4 Y axis grows downward; invert so up is positive like Xbox.
  out.thumb_ly = static_cast<int16_t>(-1 - ExpandStickAxis(pad.leftStick.y));
  out.thumb_rx = ExpandStickAxis(pad.rightStick.x);
  out.thumb_ry = static_cast<int16_t>(-1 - ExpandStickAxis(pad.rightStick.y));
}

X_RESULT PS4InputDriver::GetCapabilities(uint32_t user_index, uint32_t /*flags*/,
                                         X_INPUT_CAPABILITIES* out_caps) {
  if (user_index >= kPS4UserCount || !out_caps) {
    return X_ERROR_BAD_ARGUMENTS;
  }

  // Lazy re-scan to pick up logins that happened after Setup().
  RefreshUsers();

  std::lock_guard<std::mutex> lock(mutex_);
  auto& slot = slots_[user_index];
  if (slot.handle < 0 || !slot.connected) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }

  std::memset(out_caps, 0, sizeof(*out_caps));
  out_caps->type = XINPUT_DEVTYPE_GAMEPAD;
  out_caps->sub_type = 0x01;  // standard gamepad
  out_caps->flags =
      static_cast<uint16_t>(X_INPUT_CAPS_FFB_SUPPORTED | X_INPUT_CAPS_WIRELESS);
  out_caps->gamepad.buttons = 0xF3FF;
  out_caps->gamepad.left_trigger = 0xFF;
  out_caps->gamepad.right_trigger = 0xFF;
  out_caps->gamepad.thumb_lx = static_cast<int16_t>(0xFFFFu);
  out_caps->gamepad.thumb_ly = static_cast<int16_t>(0xFFFFu);
  out_caps->gamepad.thumb_rx = static_cast<int16_t>(0xFFFFu);
  out_caps->gamepad.thumb_ry = static_cast<int16_t>(0xFFFFu);
  out_caps->vibration.left_motor_speed = 0xFFFFu;
  out_caps->vibration.right_motor_speed = 0xFFFFu;
  return X_ERROR_SUCCESS;
}

X_RESULT PS4InputDriver::GetState(uint32_t user_index, X_INPUT_STATE* out_state) {
  if (user_index >= kPS4UserCount || !out_state) {
    return X_ERROR_BAD_ARGUMENTS;
  }

  int32_t handle;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    auto& slot = slots_[user_index];
    if (slot.handle < 0) {
      return X_ERROR_DEVICE_NOT_CONNECTED;
    }
    handle = slot.handle;
  }

  OrbisPadData pad{};
  int32_t rc = scePadReadState(handle, &pad);
  if (rc != 0) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  auto& slot = slots_[user_index];
  if (slot.handle < 0) {
    // Pad was closed between our release and re-lock.
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }
  if (!pad.connected) {
    slot.connected = false;
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }
  slot.connected = true;

  // Bump packet_number whenever buttons/timestamp change.
  if (pad.buttons != slot.last_buttons ||
      pad.timestamp != slot.last_timestamp) {
    slot.packet_number++;
    slot.last_buttons = pad.buttons;
    slot.last_timestamp = pad.timestamp;
  }

  std::memset(out_state, 0, sizeof(*out_state));
  out_state->packet_number = slot.packet_number;
  TranslatePadState(pad, out_state->gamepad);
  return X_ERROR_SUCCESS;
}

X_RESULT PS4InputDriver::SetState(uint32_t user_index,
                                  X_INPUT_VIBRATION* vibration) {
  if (user_index >= kPS4UserCount || !vibration) {
    return X_ERROR_BAD_ARGUMENTS;
  }

  int32_t handle;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    auto& slot = slots_[user_index];
    if (slot.handle < 0) return X_ERROR_DEVICE_NOT_CONNECTED;
    handle = slot.handle;
  }

  OrbisPadVibeParam vibe{};
  // X_INPUT_VIBRATION motor speeds are 0..65535; scePad wants 0..255.
  uint16_t lo = vibration->left_motor_speed;
  uint16_t hi = vibration->right_motor_speed;
  vibe.lgMotor = static_cast<uint8_t>(lo >> 8);
  vibe.smMotor = static_cast<uint8_t>(hi >> 8);
  int32_t rc = scePadSetVibration(handle, &vibe);
  if (rc != 0) {
    return X_ERROR_FUNCTION_FAILED;
  }
  return X_ERROR_SUCCESS;
}

X_RESULT PS4InputDriver::GetKeystroke(uint32_t /*user_index*/, uint32_t /*flags*/,
                                      X_INPUT_KEYSTROKE* /*out_keystroke*/) {
  // Keystroke emulation not implemented on PS4 backend — games that need
  // discrete keystrokes are expected to fall through to SDL / MnK drivers.
  return X_ERROR_EMPTY;
}

}  // namespace rex::input::ps4

#endif  // REX_PLATFORM_PS4
