#pragma once
/**
 * Android input driver for ReXGlue — NDK AInput / ASensor based gamepad backend.
 *
 * Event flow:
 *   * AInputEvents are push-driven. The host Android activity's onInputEvent()
 *     callback must forward events to this driver via HandleInputEvent().
 *   * Sensor data is polled via an ALooper/ASensorEventQueue driven by the
 *     driver's own internal polling (fetched lazily from GetState()).
 *
 * Rumble:
 *   * Routed through the host JNI layer (see LibertyRecomp/os/android/vibration_android.cpp).
 *     We do not touch JNI directly here — only call the extern "C" bridge.
 */

#include <array>
#include <atomic>
#include <cstdint>
#include <mutex>

#include <rex/input/input_driver.h>

#if REX_PLATFORM_ANDROID
struct AInputEvent;
struct ASensorManager;
struct ASensor;
struct ASensorEventQueue;
struct ALooper;
#endif

namespace rex::input::android {

#define HID_ANDROID_USER_COUNT 4
#define HID_ANDROID_STICK_DEADZONE 0.15f
#define HID_ANDROID_TRIGGER_THRES 0.12f

class AndroidInputDriver final : public InputDriver {
 public:
  explicit AndroidInputDriver(rex::ui::Window* window, size_t window_z_order);
  ~AndroidInputDriver() override;

  X_STATUS Setup() override;

  X_RESULT GetCapabilities(uint32_t user_index, uint32_t flags,
                           X_INPUT_CAPABILITIES* out_caps) override;
  X_RESULT GetState(uint32_t user_index, X_INPUT_STATE* out_state) override;
  X_RESULT SetState(uint32_t user_index, X_INPUT_VIBRATION* vibration) override;
  X_RESULT GetKeystroke(uint32_t user_index, uint32_t flags,
                        X_INPUT_KEYSTROKE* out_keystroke) override;

#if REX_PLATFORM_ANDROID
  // Called from the host activity's AInputQueue / onInputEvent callback on the
  // Android UI thread. Returns true if the event was consumed by a mapped pad.
  bool HandleInputEvent(AInputEvent* event);
#endif

 private:
  struct PadState {
    int32_t device_id = -1;   // AInputDevice id, -1 = unused slot
    bool connected = false;
    uint32_t packet_number = 0;
    X_INPUT_STATE state{};
    X_INPUT_STATE last_keystroke_state{};
  };

#if REX_PLATFORM_ANDROID
  bool HandleKeyEventLocked(AInputEvent* event);
  bool HandleMotionEventLocked(AInputEvent* event);
  size_t AssignSlotLocked(int32_t device_id);
  PadState* FindSlotByDeviceLocked(int32_t device_id);
  void PumpSensorQueueLocked();
  void InitSensorsLocked();
#endif

  std::mutex mutex_;
  std::array<PadState, HID_ANDROID_USER_COUNT> pads_{};

#if REX_PLATFORM_ANDROID
  ASensorManager* sensor_manager_ = nullptr;
  const ASensor* accel_sensor_ = nullptr;
  ASensorEventQueue* sensor_queue_ = nullptr;
  ALooper* sensor_looper_ = nullptr;
  std::atomic<float> accel_x_{0.0f};
  std::atomic<float> accel_y_{0.0f};
  std::atomic<float> accel_z_{0.0f};
#endif
};

}  // namespace rex::input::android
