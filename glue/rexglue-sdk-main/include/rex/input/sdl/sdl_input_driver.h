/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2020 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

#pragma once

#include <array>
#include <atomic>
#include <mutex>
#include <memory>
#include <optional>
#include <vector>
#include <condition_variable>
#include <thread>

#include <rex/input/input_driver.h>
#include <rex/input/motion_sample_cache.h>
#include <rex/input/sony_feedback.h>
#include <rex/input/sdl/physical_device_inventory.h>

#include <SDL3/SDL.h>

#define HID_SDL_USER_COUNT 4
#define HID_SDL_THUMB_THRES 0x4E00
#define HID_SDL_TRIGG_THRES 0x1F
#define HID_SDL_REPEAT_DELAY 400
#define HID_SDL_REPEAT_RATE 100

namespace rex::input::sdl {

class SDLInputDriver final : public InputDriver,
                             public rex::ui::WindowListener,
                             public rex::ui::WindowInputListener {
 public:
  explicit SDLInputDriver(rex::ui::Window* window, size_t window_z_order,
                          bool expose_gamepad_state = true);
  ~SDLInputDriver() override;

  X_STATUS Setup() override;
  const char* trace_name() const override { return "sdl-gamepad"; }
  const char* input_trace_name() const override { return "sdl-input"; }

  X_RESULT GetCapabilities(uint32_t user_index, uint32_t flags,
                           X_INPUT_CAPABILITIES* out_caps) override;
  X_RESULT GetState(uint32_t user_index, X_INPUT_STATE* out_state) override;
  X_RESULT SetState(uint32_t user_index, X_INPUT_VIBRATION* vibration) override;
  X_RESULT GetKeystroke(uint32_t user_index, uint32_t flags,
                        X_INPUT_KEYSTROKE* out_keystroke) override;
  bool TryGetMotionState(uint32_t user_index, MotionState* out_state) override;
  void OnWindowAvailable(rex::ui::Window* window) override;

 private:
  struct ControllerState {
    SDL_Gamepad* sdl = nullptr;
    X_INPUT_CAPABILITIES caps{};
    X_INPUT_STATE state{};
    MotionState motion{};
    uint16_t left_motor_speed = 0;
    uint16_t right_motor_speed = 0;
    uint64_t next_rumble_refresh_ms = 0;
    bool rumble_supported = false;
    bool state_changed = false;
    bool is_active = false;
    bool trace_initialized = false;
    uint32_t user_index = 0;
    sony::Model sony_model = sony::Model::kNone;
    uint16_t applied_low_motor = 0;
    uint16_t applied_high_motor = 0;
    bool sony_light_set = false;
    bool sony_triggers_set = false;
    std::array<uint8_t, 3> sony_color{};
    sony::EffectPacket sony_packet{};
    uint8_t sony_light_failures = 0;
    uint8_t sony_trigger_failures = 0;
    uint8_t sony_rumble_failures = 0;
    uint64_t sony_rumble_retry_ms = 0;
    uint64_t sony_retry_ms = 0;
  };

  enum class RepeatState {
    Idle,       // no buttons pressed or repeating has ended
    Waiting,    // a button is held and the delay is awaited
    Repeating,  // actively repeating at a rate
  };
  struct KeystrokeState {
    uint64_t buttons;
    RepeatState repeat_state;
    // the button number that was pressed last:
    uint8_t repeat_butt_idx;
    // the last time (ms) a down (and/or repeat) event for that button was send:
    uint32_t repeat_time;
  };

  // WindowListener
  void OnClosing(rex::ui::UIEvent& e) override;
  void OnLostFocus(rex::ui::UISetupEvent& e) override;
  void OnGotFocus(rex::ui::UISetupEvent& e) override;
  void OnDpiChanged(rex::ui::UISetupEvent& e) override;
  void OnResize(rex::ui::UISetupEvent& e) override;

  // WindowInputListener
  void OnTouchEvent(rex::ui::TouchEvent& e) override;

  static bool SDLCALL EventWatch(void* userdata, SDL_Event* event);
  void HandleEvent(const SDL_Event& event);
  void RefreshDeviceInventoryFromUIThread(uint64_t timestamp_ns, bool force = false);
  void RefreshPointerPresentation(uint64_t timestamp_ns);
  std::optional<std::vector<SDL_JoystickID>> QueryControllerInventory();
  std::unique_lock<std::mutex> DrainAndLock(bool refresh_rumble = true);
  void ReconcileControllerInventoryLocked(const std::vector<SDL_JoystickID>& connected_ids);
  void CloseControllerLocked(size_t index, const char* reason);
  void OpenControllerLocked(SDL_JoystickID instance_id);
  void PollControllerStateLocked(ControllerState& state);
  void PollConnectedControllerStatesLocked();

  inline uint64_t AnalogToKeyfield(const X_INPUT_GAMEPAD& gamepad) const;
  std::optional<size_t> GetControllerIndexFromInstanceID(SDL_JoystickID instance_id);
  ControllerState* GetControllerState(uint32_t user_index);
  bool TestSDLVersion() const;
  void UpdateXCapabilities(ControllerState& state);
  X_RESULT ApplyRumbleLocked(uint32_t user_index, ControllerState& state, uint16_t left_motor,
                             uint16_t right_motor, bool is_refresh);
  void RefreshRumbleLocked();
  void QueueControllerUpdate();
  void StartSonyWorker();
  void StopSonyWorker();
  void RefreshSonyFeedbackLocked();

  rex::ui::Window* attached_window_ = nullptr;
  rex::ui::WindowedAppContext* attached_app_context_ = nullptr;
  std::shared_ptr<std::atomic<bool>> ui_callback_alive_ =
      std::make_shared<std::atomic<bool>>(true);
  const bool expose_gamepad_state_;
  bool sdl_events_initialized_;
  bool SDL_Gamepad_initialized_;
  bool event_watch_installed_ = false;
  PhysicalDeviceInventory physical_device_inventory_;
  std::atomic<bool> pointer_foreground_refresh_pending_{false};
  std::atomic<bool> accepting_input_requests_{false};
  std::atomic<bool> sdl_pumpevents_queued_;
  std::atomic<uint64_t> next_motion_device_generation_{1};
  std::array<ControllerState, HID_SDL_USER_COUNT> controllers_;
  std::mutex controllers_mutex_;
  MotionSampleCache motion_samples_;
  std::array<KeystrokeState, HID_SDL_USER_COUNT> keystroke_states_;
  std::thread sony_worker_;
  std::atomic<bool> sony_stopping_{true};
  std::mutex sony_wait_mutex_;
  std::condition_variable sony_wait_;
};

}  // namespace rex::input::sdl
