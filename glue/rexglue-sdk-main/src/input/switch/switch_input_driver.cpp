/**
 * Switch (libnx) input driver implementation.
 */

#include "switch_input_driver.h"

#if REX_PLATFORM_NX

#include <algorithm>
#include <cmath>
#include <cstring>

#include <rex/input/flags.h>
#include <rex/logging.h>

namespace rex::input::nx {

namespace {

constexpr uint32_t kStickMax = 0x7FFF;

// Clamp libnx stick [-0x7FFF..0x7FFF] -> X360 int16 [-32768..32767].
int16_t ConvertStick(int32_t v) {
  if (v >  static_cast<int32_t>(kStickMax)) v =  static_cast<int32_t>(kStickMax);
  if (v < -static_cast<int32_t>(kStickMax)) v = -static_cast<int32_t>(kStickMax);
  // Scale slightly so max maps to INT16_MAX.
  int32_t scaled = (v * 32767) / static_cast<int32_t>(kStickMax);
  return static_cast<int16_t>(scaled);
}

// Button mapping. Per task spec, position-swap Switch <-> Xbox face buttons:
//   Switch A (east)  -> X360 B
//   Switch B (south) -> X360 A
//   Switch X (north) -> X360 Y
//   Switch Y (west)  -> X360 X
uint16_t MapButtons(uint64_t nx) {
  uint16_t r = 0;
  if (nx & HidNpadButton_A)          r |= X_INPUT_GAMEPAD_B;
  if (nx & HidNpadButton_B)          r |= X_INPUT_GAMEPAD_A;
  if (nx & HidNpadButton_X)          r |= X_INPUT_GAMEPAD_Y;
  if (nx & HidNpadButton_Y)          r |= X_INPUT_GAMEPAD_X;
  if (nx & HidNpadButton_Plus)       r |= X_INPUT_GAMEPAD_START;
  if (nx & HidNpadButton_Minus)      r |= X_INPUT_GAMEPAD_BACK;
  if (nx & HidNpadButton_StickL)     r |= X_INPUT_GAMEPAD_LEFT_THUMB;
  if (nx & HidNpadButton_StickR)     r |= X_INPUT_GAMEPAD_RIGHT_THUMB;
  if (nx & HidNpadButton_L)          r |= X_INPUT_GAMEPAD_LEFT_SHOULDER;
  if (nx & HidNpadButton_R)          r |= X_INPUT_GAMEPAD_RIGHT_SHOULDER;
  if (nx & HidNpadButton_Up)         r |= X_INPUT_GAMEPAD_DPAD_UP;
  if (nx & HidNpadButton_Down)       r |= X_INPUT_GAMEPAD_DPAD_DOWN;
  if (nx & HidNpadButton_Left)       r |= X_INPUT_GAMEPAD_DPAD_LEFT;
  if (nx & HidNpadButton_Right)      r |= X_INPUT_GAMEPAD_DPAD_RIGHT;
  return r;
}

uint8_t MapTrigger(uint64_t nx, uint64_t mask) {
  return (nx & mask) ? 0xFF : 0x00;
}

}  // namespace

SwitchInputDriver::SwitchInputDriver(rex::ui::Window* window, size_t window_z_order)
    : InputDriver(window, window_z_order) {
  for (size_t i = 0; i < slots_.size(); ++i) {
    slots_[i].id = IndexToId(static_cast<uint32_t>(i));
    slots_[i].packet_number = 0;
    slots_[i].last_buttons = 0;
    slots_[i].vibration_ready = false;
    slots_[i].sixaxis_ready = false;
  }
}

SwitchInputDriver::~SwitchInputDriver() = default;

HidNpadIdType SwitchInputDriver::IndexToId(uint32_t user_index) {
  switch (user_index) {
    case 0: return HidNpadIdType_No1;
    case 1: return HidNpadIdType_No2;
    case 2: return HidNpadIdType_No3;
    case 3: return HidNpadIdType_No4;
    case 4: return HidNpadIdType_No5;
    case 5: return HidNpadIdType_No6;
    case 6: return HidNpadIdType_No7;
    case 7: return HidNpadIdType_No8;
    default: return HidNpadIdType_No1;
  }
}

X_STATUS SwitchInputDriver::Setup() {
  hidInitializeNpad();
  hidSetSupportedNpadStyleSet(HidNpadStyleSet_NpadStandard);

  const HidNpadIdType ids[9] = {
      HidNpadIdType_No1, HidNpadIdType_No2, HidNpadIdType_No3, HidNpadIdType_No4,
      HidNpadIdType_No5, HidNpadIdType_No6, HidNpadIdType_No7, HidNpadIdType_No8,
      HidNpadIdType_Handheld,
  };
  hidSetSupportedNpadIdType(ids, 9);
  hidSetNpadJoyHoldType(HidNpadJoyHoldType_Horizontal);

  initialized_ = true;
  REX_LOG_INFO("[input.nx] Switch HID driver online");
  return X_STATUS_SUCCESS;
}

void SwitchInputDriver::EnsureHandles(UserSlot& slot, uint32_t style_set) {
  HidNpadStyleTag style = HidNpadStyleTag_NpadFullKey;
  HidNpadIdType id_for_handles = slot.id;

  if (slot.id == HidNpadIdType_No1 && (style_set & HidNpadStyleTag_NpadHandheld)) {
    style = HidNpadStyleTag_NpadHandheld;
    id_for_handles = HidNpadIdType_Handheld;
  } else if (style_set & HidNpadStyleTag_NpadFullKey) {
    style = HidNpadStyleTag_NpadFullKey;
  } else if (style_set & HidNpadStyleTag_NpadJoyDual) {
    style = HidNpadStyleTag_NpadJoyDual;
  } else if (style_set & HidNpadStyleTag_NpadJoyLeft) {
    style = HidNpadStyleTag_NpadJoyLeft;
  } else if (style_set & HidNpadStyleTag_NpadJoyRight) {
    style = HidNpadStyleTag_NpadJoyRight;
  } else {
    return;
  }

  if (!slot.vibration_ready) {
    if (R_SUCCEEDED(hidInitializeVibrationDevices(slot.vibration, 2,
                                                  id_for_handles, style))) {
      slot.vibration_ready = true;
    }
  }
  if (!slot.sixaxis_ready) {
    if (R_SUCCEEDED(hidGetSixAxisSensorHandles(slot.sixaxis, 2,
                                               id_for_handles, style))) {
      hidStartSixAxisSensor(slot.sixaxis[0]);
      hidStartSixAxisSensor(slot.sixaxis[1]);
      slot.sixaxis_ready = true;
    }
  }
}

bool SwitchInputDriver::ReadPad(UserSlot& slot, uint32_t* buttons_out,
                                HidAnalogStickState* l_out,
                                HidAnalogStickState* r_out) {
  uint32_t style_set = hidGetNpadStyleSet(slot.id);
  // Handheld mode for slot 0 only.
  if (slot.id == HidNpadIdType_No1) {
    uint32_t hh_style = hidGetNpadStyleSet(HidNpadIdType_Handheld);
    if (hh_style & HidNpadStyleTag_NpadHandheld) {
      style_set = hh_style;
      EnsureHandles(slot, style_set);
      HidNpadHandheldState s{};
      if (hidGetNpadStatesHandheld(HidNpadIdType_Handheld, &s, 1) >= 1) {
        *buttons_out = static_cast<uint32_t>(s.buttons);
        *l_out = s.analog_stick_l;
        *r_out = s.analog_stick_r;
        return true;
      }
    }
  }

  if (!style_set) return false;
  EnsureHandles(slot, style_set);

  if (style_set & HidNpadStyleTag_NpadFullKey) {
    HidNpadFullKeyState s{};
    if (hidGetNpadStatesFullKey(slot.id, &s, 1) >= 1) {
      *buttons_out = static_cast<uint32_t>(s.buttons);
      *l_out = s.analog_stick_l;
      *r_out = s.analog_stick_r;
      return true;
    }
  }
  if (style_set & HidNpadStyleTag_NpadJoyDual) {
    HidNpadJoyDualState s{};
    if (hidGetNpadStatesJoyDual(slot.id, &s, 1) >= 1) {
      *buttons_out = static_cast<uint32_t>(s.buttons);
      *l_out = s.analog_stick_l;
      *r_out = s.analog_stick_r;
      return true;
    }
  }
  if (style_set & HidNpadStyleTag_NpadJoyLeft) {
    HidNpadJoyLeftState s{};
    if (hidGetNpadStatesJoyLeft(slot.id, &s, 1) >= 1) {
      *buttons_out = static_cast<uint32_t>(s.buttons);
      *l_out = s.analog_stick_l;
      *r_out = s.analog_stick_l;  // single joycon -> duplicate
      return true;
    }
  }
  if (style_set & HidNpadStyleTag_NpadJoyRight) {
    HidNpadJoyRightState s{};
    if (hidGetNpadStatesJoyRight(slot.id, &s, 1) >= 1) {
      *buttons_out = static_cast<uint32_t>(s.buttons);
      *l_out = s.analog_stick_r;
      *r_out = s.analog_stick_r;
      return true;
    }
  }
  return false;
}

X_RESULT SwitchInputDriver::GetCapabilities(uint32_t user_index, uint32_t /*flags*/,
                                            X_INPUT_CAPABILITIES* out_caps) {
  if (user_index >= slots_.size()) return X_ERROR_DEVICE_NOT_CONNECTED;
  UserSlot& slot = slots_[user_index];

  uint32_t style_set = hidGetNpadStyleSet(slot.id);
  if (slot.id == HidNpadIdType_No1) {
    style_set |= hidGetNpadStyleSet(HidNpadIdType_Handheld);
  }
  if (!style_set) return X_ERROR_DEVICE_NOT_CONNECTED;

  if (out_caps) {
    std::memset(out_caps, 0, sizeof(*out_caps));
    out_caps->type = XINPUT_DEVTYPE_GAMEPAD;
    out_caps->sub_type = 0x01;
    out_caps->flags = 0;
    out_caps->gamepad.buttons = 0xF3FF;
    out_caps->gamepad.left_trigger = 0xFF;
    out_caps->gamepad.right_trigger = 0xFF;
    out_caps->gamepad.thumb_lx = static_cast<int16_t>(0x7FFF);
    out_caps->gamepad.thumb_ly = static_cast<int16_t>(0x7FFF);
    out_caps->gamepad.thumb_rx = static_cast<int16_t>(0x7FFF);
    out_caps->gamepad.thumb_ry = static_cast<int16_t>(0x7FFF);
    out_caps->vibration.left_motor_speed = 0xFFFF;
    out_caps->vibration.right_motor_speed = 0xFFFF;
  }
  return X_ERROR_SUCCESS;
}

X_RESULT SwitchInputDriver::GetState(uint32_t user_index, X_INPUT_STATE* out_state) {
  if (user_index >= slots_.size()) return X_ERROR_DEVICE_NOT_CONNECTED;
  UserSlot& slot = slots_[user_index];

  uint32_t raw_buttons = 0;
  HidAnalogStickState l{}, r{};
  if (!ReadPad(slot, &raw_buttons, &l, &r)) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }

  uint16_t mapped = MapButtons(raw_buttons);
  uint8_t lt = MapTrigger(raw_buttons, HidNpadButton_ZL);
  uint8_t rt = MapTrigger(raw_buttons, HidNpadButton_ZR);

  if (mapped != slot.last_buttons) {
    slot.packet_number++;
    slot.last_buttons = mapped;
  }

  if (out_state) {
    std::memset(out_state, 0, sizeof(*out_state));
    out_state->packet_number = slot.packet_number;
    out_state->gamepad.buttons = mapped;
    out_state->gamepad.left_trigger = lt;
    out_state->gamepad.right_trigger = rt;
    out_state->gamepad.thumb_lx = ConvertStick(l.x);
    out_state->gamepad.thumb_ly = ConvertStick(l.y);
    out_state->gamepad.thumb_rx = ConvertStick(r.x);
    out_state->gamepad.thumb_ry = ConvertStick(r.y);
  }
  return X_ERROR_SUCCESS;
}

X_RESULT SwitchInputDriver::SetState(uint32_t user_index, X_INPUT_VIBRATION* vibration) {
  if (user_index >= slots_.size()) return X_ERROR_DEVICE_NOT_CONNECTED;
  UserSlot& slot = slots_[user_index];

  uint32_t style_set = hidGetNpadStyleSet(slot.id);
  if (slot.id == HidNpadIdType_No1) style_set |= hidGetNpadStyleSet(HidNpadIdType_Handheld);
  if (!style_set) return X_ERROR_DEVICE_NOT_CONNECTED;
  EnsureHandles(slot, style_set);
  if (!slot.vibration_ready || !vibration) return X_ERROR_SUCCESS;

  float l_amp = static_cast<float>(static_cast<uint16_t>(vibration->left_motor_speed)) / 65535.0f;
  float r_amp = static_cast<float>(static_cast<uint16_t>(vibration->right_motor_speed)) / 65535.0f;

  HidVibrationValue values[2]{};
  values[0].amp_low = l_amp;        // left motor -> low band
  values[0].freq_low = 160.0f;
  values[0].amp_high = 0.0f;
  values[0].freq_high = 320.0f;
  values[1].amp_low = 0.0f;
  values[1].freq_low = 160.0f;
  values[1].amp_high = r_amp;       // right motor -> high band
  values[1].freq_high = 320.0f;

  hidSendVibrationValues(slot.vibration, values, 2);
  return X_ERROR_SUCCESS;
}

X_RESULT SwitchInputDriver::GetKeystroke(uint32_t user_index, uint32_t /*flags*/,
                                         X_INPUT_KEYSTROKE* /*out_keystroke*/) {
  if (user_index >= slots_.size()) return X_ERROR_DEVICE_NOT_CONNECTED;
  return X_ERROR_EMPTY;
}

}  // namespace rex::input::nx

#endif  // REX_PLATFORM_NX
