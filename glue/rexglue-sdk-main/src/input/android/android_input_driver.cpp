/**
 * Android input backend — NDK AInput + ASensor based.
 *
 * Event pipeline:
 *   Host activity (Java NativeActivity / GameActivity) receives AInputEvents
 *   on its UI thread and forwards them to AndroidInputDriver::HandleInputEvent.
 *   Axes (sticks, triggers, hat) are read via AMotionEvent_getAxisValue, and
 *   buttons via AKeyEvent_getKeyCode.
 *
 * Rumble:
 *   SetState() forwards to the host JNI bridge via the
 *     extern "C" void android_vibration_set_rumble(device_id, low, high, ms)
 *   already implemented in LibertyRecomp/os/android/vibration_android.cpp.
 *
 * Motion sensors:
 *   A background ALooper + ASensorEventQueue is created on Setup(). The
 *   accelerometer is polled lazily from GetState(). Readings are exposed
 *   only to host callers that care — XInput has no field for them.
 */

#include "android_input_driver.h"

#include <algorithm>
#include <cmath>
#include <cstring>

#include <rex/logging.h>
#include <rex/logging/macros.h>

#if REX_PLATFORM_ANDROID
#include <android/input.h>
#include <android/keycodes.h>
#include <android/looper.h>
#include <android/sensor.h>

// Rumble bridge implemented by the Android host app (JNI glue).
extern "C" void android_vibration_set_rumble(int32_t device_id,
                                             uint16_t low_freq,
                                             uint16_t high_freq,
                                             uint32_t duration_ms);
extern "C" void android_vibration_stop(int32_t device_id);
#endif

namespace rex::input::android {

namespace {

#if REX_PLATFORM_ANDROID

constexpr int kAndroidLooperSensorId = 3;

int16_t NormalizeStick(float v) {
  // AXIS_X/Y/Z/RZ are already normalised to [-1,1] for gamepads.
  if (std::fabs(v) < HID_ANDROID_STICK_DEADZONE) {
    return 0;
  }
  float clamped = std::clamp(v, -1.0f, 1.0f);
  int32_t scaled = static_cast<int32_t>(clamped * 32767.0f);
  return static_cast<int16_t>(std::clamp(scaled, -32768, 32767));
}

uint8_t NormalizeTrigger(float v) {
  if (v < HID_ANDROID_TRIGGER_THRES) return 0;
  float clamped = std::clamp(v, 0.0f, 1.0f);
  return static_cast<uint8_t>(clamped * 255.0f);
}

uint16_t KeyCodeToXButton(int32_t key_code) {
  switch (key_code) {
    case AKEYCODE_BUTTON_A:       return X_INPUT_GAMEPAD_A;
    case AKEYCODE_BUTTON_B:       return X_INPUT_GAMEPAD_B;
    case AKEYCODE_BUTTON_X:       return X_INPUT_GAMEPAD_X;
    case AKEYCODE_BUTTON_Y:       return X_INPUT_GAMEPAD_Y;
    case AKEYCODE_BUTTON_L1:      return X_INPUT_GAMEPAD_LEFT_SHOULDER;
    case AKEYCODE_BUTTON_R1:      return X_INPUT_GAMEPAD_RIGHT_SHOULDER;
    case AKEYCODE_BUTTON_THUMBL:  return X_INPUT_GAMEPAD_LEFT_THUMB;
    case AKEYCODE_BUTTON_THUMBR:  return X_INPUT_GAMEPAD_RIGHT_THUMB;
    case AKEYCODE_BUTTON_START:   return X_INPUT_GAMEPAD_START;
    case AKEYCODE_BUTTON_SELECT:  return X_INPUT_GAMEPAD_BACK;
    case AKEYCODE_BUTTON_MODE:    return X_INPUT_GAMEPAD_GUIDE;
    case AKEYCODE_DPAD_UP:        return X_INPUT_GAMEPAD_DPAD_UP;
    case AKEYCODE_DPAD_DOWN:      return X_INPUT_GAMEPAD_DPAD_DOWN;
    case AKEYCODE_DPAD_LEFT:      return X_INPUT_GAMEPAD_DPAD_LEFT;
    case AKEYCODE_DPAD_RIGHT:     return X_INPUT_GAMEPAD_DPAD_RIGHT;
    default:                      return 0;
  }
}

// Shoulder trigger keys carry value in AKEYCODE_BUTTON_L2/R2 on some pads.
uint8_t TriggerKeyToValue(int32_t key_code, int32_t action) {
  if (key_code == AKEYCODE_BUTTON_L2 || key_code == AKEYCODE_BUTTON_R2) {
    return (action == AKEY_EVENT_ACTION_DOWN) ? 0xFF : 0x00;
  }
  return 0xFFu;  // sentinel "not a trigger key"
}

#endif  // REX_PLATFORM_ANDROID

}  // namespace

AndroidInputDriver::AndroidInputDriver(rex::ui::Window* window,
                                      size_t window_z_order)
    : InputDriver(window, window_z_order) {}

AndroidInputDriver::~AndroidInputDriver() {
#if REX_PLATFORM_ANDROID
  if (sensor_queue_ && sensor_manager_) {
    if (accel_sensor_) {
      ASensorEventQueue_disableSensor(sensor_queue_, accel_sensor_);
    }
    ASensorManager_destroyEventQueue(sensor_manager_, sensor_queue_);
    sensor_queue_ = nullptr;
  }
#endif
}

X_STATUS AndroidInputDriver::Setup() {
#if REX_PLATFORM_ANDROID
  std::lock_guard<std::mutex> lk(mutex_);
  InitSensorsLocked();
#endif
  return X_STATUS_SUCCESS;
}

#if REX_PLATFORM_ANDROID
void AndroidInputDriver::InitSensorsLocked() {
  sensor_looper_ = ALooper_forThread();
  if (!sensor_looper_) {
    sensor_looper_ = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
  }
  if (!sensor_looper_) {
    REXLOG_WARN("no ALooper available; sensors disabled");
    return;
  }

  // Use package-free getInstance on modern Android (API 26+ deprecates the
  // signature-less variant, but it still functions).
  sensor_manager_ = ASensorManager_getInstance();
  if (!sensor_manager_) {
    REXLOG_WARN("ASensorManager_getInstance failed");
    return;
  }
  accel_sensor_ = ASensorManager_getDefaultSensor(sensor_manager_,
                                                  ASENSOR_TYPE_ACCELEROMETER);
  if (!accel_sensor_) {
    REXLOG_INFO("no accelerometer present");
    return;
  }
  sensor_queue_ = ASensorManager_createEventQueue(
      sensor_manager_, sensor_looper_, kAndroidLooperSensorId, nullptr, nullptr);
  if (!sensor_queue_) {
    REXLOG_WARN("failed to create sensor event queue");
    return;
  }
  ASensorEventQueue_enableSensor(sensor_queue_, accel_sensor_);
  // ~60Hz sampling. Arg is microseconds.
  ASensorEventQueue_setEventRate(sensor_queue_, accel_sensor_, 16'666);
}

void AndroidInputDriver::PumpSensorQueueLocked() {
  if (!sensor_queue_) return;
  ASensorEvent ev;
  while (ASensorEventQueue_getEvents(sensor_queue_, &ev, 1) > 0) {
    if (ev.type == ASENSOR_TYPE_ACCELEROMETER) {
      accel_x_.store(ev.acceleration.x, std::memory_order_relaxed);
      accel_y_.store(ev.acceleration.y, std::memory_order_relaxed);
      accel_z_.store(ev.acceleration.z, std::memory_order_relaxed);
    }
  }
}

AndroidInputDriver::PadState* AndroidInputDriver::FindSlotByDeviceLocked(
    int32_t device_id) {
  for (auto& p : pads_) {
    if (p.connected && p.device_id == device_id) return &p;
  }
  return nullptr;
}

size_t AndroidInputDriver::AssignSlotLocked(int32_t device_id) {
  for (size_t i = 0; i < pads_.size(); ++i) {
    if (pads_[i].connected && pads_[i].device_id == device_id) return i;
  }
  for (size_t i = 0; i < pads_.size(); ++i) {
    if (!pads_[i].connected) {
      pads_[i] = PadState{};
      pads_[i].device_id = device_id;
      pads_[i].connected = true;
      return i;
    }
  }
  return SIZE_MAX;
}

bool AndroidInputDriver::HandleKeyEventLocked(AInputEvent* event) {
  const int32_t device_id = AInputEvent_getDeviceId(event);
  const int32_t key_code = AKeyEvent_getKeyCode(event);
  const int32_t action = AKeyEvent_getAction(event);

  uint16_t mask = KeyCodeToXButton(key_code);
  uint8_t trig = TriggerKeyToValue(key_code, action);
  if (mask == 0 && trig == 0xFFu) {
    return false;  // not a pad button
  }

  size_t slot = AssignSlotLocked(device_id);
  if (slot == SIZE_MAX) return false;
  auto& pad = pads_[slot];

  uint16_t buttons = static_cast<uint16_t>(pad.state.gamepad.buttons);
  if (action == AKEY_EVENT_ACTION_DOWN) {
    buttons |= mask;
    if (key_code == AKEYCODE_BUTTON_L2) pad.state.gamepad.left_trigger = trig;
    if (key_code == AKEYCODE_BUTTON_R2) pad.state.gamepad.right_trigger = trig;
  } else if (action == AKEY_EVENT_ACTION_UP) {
    buttons &= ~mask;
    if (key_code == AKEYCODE_BUTTON_L2) pad.state.gamepad.left_trigger = 0;
    if (key_code == AKEYCODE_BUTTON_R2) pad.state.gamepad.right_trigger = 0;
  }
  pad.state.gamepad.buttons = buttons;
  pad.state.packet_number = ++pad.packet_number;
  return true;
}

bool AndroidInputDriver::HandleMotionEventLocked(AInputEvent* event) {
  const int32_t source = AInputEvent_getSource(event);
  const bool is_joystick = (source & AINPUT_SOURCE_JOYSTICK) == AINPUT_SOURCE_JOYSTICK;
  const bool is_gamepad  = (source & AINPUT_SOURCE_GAMEPAD)  == AINPUT_SOURCE_GAMEPAD;
  if (!is_joystick && !is_gamepad) return false;

  const int32_t device_id = AInputEvent_getDeviceId(event);
  size_t slot = AssignSlotLocked(device_id);
  if (slot == SIZE_MAX) return false;
  auto& pad = pads_[slot];

  const size_t idx = 0;  // historical pointer
  pad.state.gamepad.thumb_lx = NormalizeStick(
      AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_X, idx));
  // Android Y is down-positive; invert for Xbox convention.
  pad.state.gamepad.thumb_ly = NormalizeStick(
      -AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_Y, idx));
  pad.state.gamepad.thumb_rx = NormalizeStick(
      AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_Z, idx));
  pad.state.gamepad.thumb_ry = NormalizeStick(
      -AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_RZ, idx));

  float lt = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_LTRIGGER, idx);
  float rt = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_RTRIGGER, idx);
  // Fall back to BRAKE/GAS on some drivers.
  if (lt == 0.0f) {
    lt = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_BRAKE, idx);
  }
  if (rt == 0.0f) {
    rt = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_GAS, idx);
  }
  pad.state.gamepad.left_trigger = NormalizeTrigger(lt);
  pad.state.gamepad.right_trigger = NormalizeTrigger(rt);

  // Hat → D-pad (overrides key events if present).
  float hx = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_HAT_X, idx);
  float hy = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_HAT_Y, idx);
  uint16_t buttons = static_cast<uint16_t>(pad.state.gamepad.buttons);
  buttons &= ~(X_INPUT_GAMEPAD_DPAD_UP | X_INPUT_GAMEPAD_DPAD_DOWN |
               X_INPUT_GAMEPAD_DPAD_LEFT | X_INPUT_GAMEPAD_DPAD_RIGHT);
  if (hx < -0.5f) buttons |= X_INPUT_GAMEPAD_DPAD_LEFT;
  if (hx >  0.5f) buttons |= X_INPUT_GAMEPAD_DPAD_RIGHT;
  if (hy < -0.5f) buttons |= X_INPUT_GAMEPAD_DPAD_UP;
  if (hy >  0.5f) buttons |= X_INPUT_GAMEPAD_DPAD_DOWN;
  pad.state.gamepad.buttons = buttons;

  pad.state.packet_number = ++pad.packet_number;
  return true;
}

bool AndroidInputDriver::HandleInputEvent(AInputEvent* event) {
  if (!event) return false;
  std::lock_guard<std::mutex> lk(mutex_);
  const int32_t type = AInputEvent_getType(event);
  if (type == AINPUT_EVENT_TYPE_KEY) return HandleKeyEventLocked(event);
  if (type == AINPUT_EVENT_TYPE_MOTION) return HandleMotionEventLocked(event);
  return false;
}

#endif  // REX_PLATFORM_ANDROID

X_RESULT AndroidInputDriver::GetCapabilities(uint32_t user_index,
                                             uint32_t /*flags*/,
                                             X_INPUT_CAPABILITIES* out_caps) {
  if (user_index >= HID_ANDROID_USER_COUNT) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }
  std::lock_guard<std::mutex> lk(mutex_);
  auto& pad = pads_[user_index];
  if (!pad.connected) return X_ERROR_DEVICE_NOT_CONNECTED;
  if (out_caps) {
    std::memset(out_caps, 0, sizeof(*out_caps));
    out_caps->type = XINPUT_DEVTYPE_GAMEPAD;
    out_caps->sub_type = 0x01;
    out_caps->flags = 0;
    out_caps->gamepad.buttons = 0xFFFF;
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

X_RESULT AndroidInputDriver::GetState(uint32_t user_index,
                                      X_INPUT_STATE* out_state) {
  if (user_index >= HID_ANDROID_USER_COUNT) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }
  std::lock_guard<std::mutex> lk(mutex_);
#if REX_PLATFORM_ANDROID
  PumpSensorQueueLocked();
#endif
  auto& pad = pads_[user_index];
  if (!pad.connected) return X_ERROR_DEVICE_NOT_CONNECTED;
  if (out_state) *out_state = pad.state;
  return X_ERROR_SUCCESS;
}

X_RESULT AndroidInputDriver::SetState(uint32_t user_index,
                                      X_INPUT_VIBRATION* vibration) {
  if (user_index >= HID_ANDROID_USER_COUNT) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }
  std::lock_guard<std::mutex> lk(mutex_);
  auto& pad = pads_[user_index];
  if (!pad.connected) return X_ERROR_DEVICE_NOT_CONNECTED;
#if REX_PLATFORM_ANDROID
  if (vibration) {
    uint16_t lo = static_cast<uint16_t>(vibration->left_motor_speed);
    uint16_t hi = static_cast<uint16_t>(vibration->right_motor_speed);
    if (lo == 0 && hi == 0) {
      android_vibration_stop(pad.device_id);
    } else {
      // Xbox vibration has no duration — play a ~200ms pulse and let the
      // next SetState refresh/stop it.
      android_vibration_set_rumble(pad.device_id, lo, hi, 200u);
    }
  }
#else
  (void)vibration;
#endif
  return X_ERROR_SUCCESS;
}

X_RESULT AndroidInputDriver::GetKeystroke(uint32_t user_index,
                                          uint32_t /*flags*/,
                                          X_INPUT_KEYSTROKE* out_keystroke) {
  if (user_index >= HID_ANDROID_USER_COUNT) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }
  std::lock_guard<std::mutex> lk(mutex_);
  auto& pad = pads_[user_index];
  if (!pad.connected) return X_ERROR_DEVICE_NOT_CONNECTED;
  // MVP: no edge tracking yet. MnK driver covers keyboard keystrokes;
  // most games poll GetState for pads.
  (void)out_keystroke;
  return X_ERROR_EMPTY;
}

}  // namespace rex::input::android
