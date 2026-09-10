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

#include <algorithm>
#include <array>
#include <filesystem>

#include <rex/assert.h>
#include <rex/chrono/clock.h>
#include <rex/cvar.h>
#include <rex/input/absolute_pointer.h>
#include <rex/input/flags.h>
#include <rex/input/input_trace.h>
#include <rex/input/sdl/sdl_input_driver.h>
#include <rex/logging.h>
#include <rex/ui/virtual_key.h>

#include "sdl_axis_policy.h"
#include "sdl_hotplug_policy.h"
#include "sdl_rumble_policy.h"

REXCVAR_DEFINE_STRING(hid_mappings_file, "", "Input", "Path to SDL gamecontroller mappings file");

namespace rex::input::sdl {

namespace {

constexpr std::array<uint16_t, SDL_GAMEPAD_BUTTON_COUNT> kXInputButtonFromSDL = {
    X_INPUT_GAMEPAD_A,
    X_INPUT_GAMEPAD_B,
    X_INPUT_GAMEPAD_X,
    X_INPUT_GAMEPAD_Y,
    X_INPUT_GAMEPAD_BACK,
    X_INPUT_GAMEPAD_GUIDE,
    X_INPUT_GAMEPAD_START,
    X_INPUT_GAMEPAD_LEFT_THUMB,
    X_INPUT_GAMEPAD_RIGHT_THUMB,
    X_INPUT_GAMEPAD_LEFT_SHOULDER,
    X_INPUT_GAMEPAD_RIGHT_SHOULDER,
    X_INPUT_GAMEPAD_DPAD_UP,
    X_INPUT_GAMEPAD_DPAD_DOWN,
    X_INPUT_GAMEPAD_DPAD_LEFT,
    X_INPUT_GAMEPAD_DPAD_RIGHT,
    X_INPUT_GAMEPAD_GUIDE,
    X_INPUT_GAMEPAD_Y,
    X_INPUT_GAMEPAD_B,
    X_INPUT_GAMEPAD_X,
    X_INPUT_GAMEPAD_A,
    X_INPUT_GAMEPAD_BACK,
    0,
    0,
    0,
    0,
    0,
};

static_assert(SDL_GAMEPAD_BUTTON_SOUTH == 0);
static_assert(SDL_GAMEPAD_BUTTON_DPAD_RIGHT == 14);
static_assert(SDL_GAMEPAD_BUTTON_TOUCHPAD == 20);
static_assert(kXInputButtonFromSDL.size() == SDL_GAMEPAD_BUTTON_COUNT);

}  // namespace

#if REX_PLATFORM_ANDROID
namespace {

constexpr uint64_t kAndroidKeyboardRefreshIntervalMs = 1000;

}  // namespace
#endif

SDLInputDriver::SDLInputDriver(rex::ui::Window* window, size_t window_z_order,
                               bool expose_gamepad_state)
    : InputDriver(window, window_z_order),
      expose_gamepad_state_(expose_gamepad_state),
      sdl_events_initialized_(false),
      SDL_Gamepad_initialized_(false),
      sdl_pumpevents_queued_(false),
      controllers_(),
      controllers_mutex_(),
      keystroke_states_() {}

SDLInputDriver::~SDLInputDriver() {}

X_STATUS SDLInputDriver::Setup() {
  if (!TestSDLVersion()) {
    return X_STATUS_UNSUCCESSFUL;
  }

  return X_STATUS_SUCCESS;
}

void SDLInputDriver::OnWindowAvailable(rex::ui::Window* window) {
  if (window && !attached_window_) {
    attached_window_ = window;
    window->AddListener(this);
    window->AddInputListener(this, window_z_order());
    window->app_context().CallInUIThreadSynchronous([this]() {
      accepting_input_requests_.store(false, std::memory_order_release);
      // Initialize SDL events subsystem
      if (!SDL_InitSubSystem(SDL_INIT_EVENTS)) {
        REXLOG_ERROR("SDL: Failed to init events subsystem: {}", SDL_GetError());
        return;
      }
      sdl_events_initialized_ = true;

      // With an event watch we will always get notified, even if the event queue
      // is full, which can happen if another subsystem does not clear its events.
      event_watch_installed_ = SDL_AddEventWatch(EventWatch, this);
      if (!event_watch_installed_) {
        REXLOG_ERROR("SDL: Failed to install input device event watch: {}", SDL_GetError());
      }

      // Initialize game controller subsystem
      if (!SDL_InitSubSystem(SDL_INIT_GAMEPAD)) {
        REXLOG_ERROR("SDL: Failed to init gamecontroller subsystem: {}", SDL_GetError());
        return;
      }
      SDL_Gamepad_initialized_ = true;

      const uint64_t inventory_timestamp_ns = SDL_GetTicksNS();
      RefreshDeviceInventoryFromUIThread(inventory_timestamp_ns);
      RefreshAndroidKeyboardFromUIThread(inventory_timestamp_ns, true);
      GetAbsolutePointerService().SetFocused(attached_window_->HasFocus(), inventory_timestamp_ns);
      RefreshPointerPresentation(inventory_timestamp_ns);

      // Load custom controller mappings if available
      if (!REXCVAR_GET(hid_mappings_file).empty()) {
        std::filesystem::path mappings_path(REXCVAR_GET(hid_mappings_file));
        if (!std::filesystem::exists(mappings_path)) {
          REXLOG_WARN("SDL GameControllerDB: file '{}' does not exist.",
                      REXCVAR_GET(hid_mappings_file));
        } else {
          auto mappings_result =
              SDL_AddGamepadMappingsFromFile(REXCVAR_GET(hid_mappings_file).c_str());
          if (mappings_result < 0) {
            REXLOG_ERROR("SDL GameControllerDB: error loading file '{}': {}.",
                         REXCVAR_GET(hid_mappings_file), mappings_result);
          } else {
            REXLOG_INFO("SDL GameControllerDB: loaded {} mappings.", mappings_result);
          }
        }
      }
      REXLOG_INFO("SDL input driver initialized successfully");
      accepting_input_requests_.store(true, std::memory_order_release);
    });
  }
}

void SDLInputDriver::OnClosing(rex::ui::UIEvent&) {
  if (attached_window_) {
    // Stop new guest-thread polls before waiting for any poll already holding
    // controllers_mutex_. DrainAndLock rechecks this flag after taking the
    // mutex, so no SDL gamepad call can race subsystem teardown.
    accepting_input_requests_.store(false, std::memory_order_release);
    GetAbsolutePointerService().SetFocused(false, SDL_GetTicksNS());
    attached_window_->RemoveInputListener(this);
    attached_window_->RemoveListener(this);
    if (event_watch_installed_) {
      SDL_RemoveEventWatch(EventWatch, this);
      event_watch_installed_ = false;
    }

    std::unique_lock controllers_guard(controllers_mutex_);
    if (sdl_pumpevents_queued_) {
      attached_window_->app_context().CallInUIThreadSynchronous(
          [this]() { attached_window_->app_context().ExecutePendingFunctionsFromUIThread(); });
    }
    for (size_t i = 0; i < controllers_.size(); i++) {
      CloseControllerLocked(i, "window-closing");
    }
    if (SDL_Gamepad_initialized_) {
      SDL_QuitSubSystem(SDL_INIT_GAMEPAD);
      SDL_Gamepad_initialized_ = false;
    }
    if (sdl_events_initialized_) {
      SDL_QuitSubSystem(SDL_INIT_EVENTS);
      sdl_events_initialized_ = false;
    }
    attached_window_ = nullptr;
  }
}

void SDLInputDriver::OnLostFocus(rex::ui::UISetupEvent&) {
  GetAbsolutePointerService().SetFocused(false, SDL_GetTicksNS());
}

void SDLInputDriver::OnGotFocus(rex::ui::UISetupEvent&) {
  const uint64_t timestamp_ns = SDL_GetTicksNS();
  GetAbsolutePointerService().SetFocused(true, timestamp_ns);
  RefreshDeviceInventoryFromUIThread(timestamp_ns);
  RefreshAndroidKeyboardFromUIThread(timestamp_ns, true);
  RefreshPointerPresentation(timestamp_ns);
}

void SDLInputDriver::OnDpiChanged(rex::ui::UISetupEvent&) {
  RefreshPointerPresentation(SDL_GetTicksNS());
}

void SDLInputDriver::OnResize(rex::ui::UISetupEvent&) {
  RefreshPointerPresentation(SDL_GetTicksNS());
}

void SDLInputDriver::OnTouchEvent(rex::ui::TouchEvent& event) {
  AbsolutePointerPhase phase;
  switch (event.action()) {
    case rex::ui::TouchEvent::Action::kDown:
      phase = AbsolutePointerPhase::kDown;
      break;
    case rex::ui::TouchEvent::Action::kMove:
      phase = AbsolutePointerPhase::kMove;
      break;
    case rex::ui::TouchEvent::Action::kUp:
      phase = AbsolutePointerPhase::kUp;
      break;
    case rex::ui::TouchEvent::Action::kCancel:
      phase = AbsolutePointerPhase::kCancel;
      break;
    default:
      return;
  }

  RefreshPointerPresentation(event.timestamp_ns());
  GetAbsolutePointerService().SubmitPointer(event.device_id(), event.pointer_id(), phase, event.x(),
                                            event.y(), event.pressure(), event.timestamp_ns());
}

bool SDLCALL SDLInputDriver::EventWatch(void* userdata, SDL_Event* event) {
  if (!userdata || !event) {
    assert_always();
    return false;
  }

  const auto type = event->type;
  const bool controller_event =
      type == SDL_EVENT_GAMEPAD_ADDED || type == SDL_EVENT_GAMEPAD_REMOVED;
  const bool keyboard_inventory_event =
      type == SDL_EVENT_KEYBOARD_ADDED || type == SDL_EVENT_KEYBOARD_REMOVED;
  const bool sensor_event = type == SDL_EVENT_GAMEPAD_SENSOR_UPDATE;
  if (!controller_event && !keyboard_inventory_event && !sensor_event) {
    return false;
  }

  // Inventory is reconciled at each guest poll. Actual sensor delivery is
  // observed here; repeated reads of SDL's cached values are not new samples.
  static_cast<SDLInputDriver*>(userdata)->HandleEvent(*event);
  return false;
}

void SDLInputDriver::RefreshDeviceInventoryFromUIThread(uint64_t timestamp_ns) {
  int keyboard_count = 0;
  SDL_KeyboardID* keyboard_ids = SDL_GetKeyboards(&keyboard_count);
  std::vector<uint64_t> keyboards;
  if (keyboard_ids) {
    keyboards.reserve(size_t(std::max(keyboard_count, 0)));
    for (int i = 0; i < keyboard_count; ++i) {
      if (keyboard_ids[i]) {
        keyboards.push_back(uint64_t(keyboard_ids[i]));
      }
    }
    SDL_free(keyboard_ids);
  } else if (keyboard_count) {
    REXLOG_WARN("SDL keyboard inventory failed: {}", SDL_GetError());
  }
  GetAbsolutePointerService().ReplacePhysicalKeyboards(keyboards, timestamp_ns);

  int gamepad_count = 0;
  SDL_ClearError();
  SDL_JoystickID* gamepad_ids = SDL_GetGamepads(&gamepad_count);
  std::vector<uint64_t> gamepads;
  if (gamepad_ids) {
    gamepads.reserve(size_t(std::max(gamepad_count, 0)));
    for (int i = 0; i < gamepad_count; ++i) {
      if (gamepad_ids[i]) {
        gamepads.push_back(uint64_t(gamepad_ids[i]));
      }
    }
    SDL_free(gamepad_ids);
  } else {
    REXLOG_WARN("SDL gamepad inventory failed: {}", SDL_GetError());
    return;
  }
  GetAbsolutePointerService().ReplaceGameControllers(gamepads, timestamp_ns);
}

std::optional<std::vector<SDL_JoystickID>> SDLInputDriver::QueryControllerInventory() {
  int gamepad_count = 0;
  SDL_ClearError();
  SDL_JoystickID* gamepad_ids = SDL_GetGamepads(&gamepad_count);
  if (!gamepad_ids) {
    REXLOG_WARN("SDL gamepad inventory query failed: {}", SDL_GetError());
    return std::nullopt;
  }

  std::vector<SDL_JoystickID> inventory;
  inventory.reserve(size_t(std::max(gamepad_count, 0)));
  for (int i = 0; i < gamepad_count; ++i) {
    if (gamepad_ids[i]) {
      inventory.push_back(gamepad_ids[i]);
    }
  }
  SDL_free(gamepad_ids);
  return inventory;
}

void SDLInputDriver::RefreshAndroidKeyboardFromUIThread(uint64_t timestamp_ns, bool force) {
#if REX_PLATFORM_ANDROID
  const uint64_t now_ms = SDL_GetTicks();
  if (!force && now_ms < next_android_keyboard_refresh_ms_) {
    return;
  }
  next_android_keyboard_refresh_ms_ = now_ms + kAndroidKeyboardRefreshIntervalMs;
  RefreshAndroidPhysicalKeyboardPresence(timestamp_ns);
#else
  (void)timestamp_ns;
  (void)force;
#endif
}

void SDLInputDriver::RefreshPointerPresentation(uint64_t timestamp_ns) {
  if (!attached_window_) {
    return;
  }
  int32_t safe_x = 0;
  int32_t safe_y = 0;
  int32_t safe_width = 0;
  int32_t safe_height = 0;
  attached_window_->GetPhysicalSafeArea(safe_x, safe_y, safe_width, safe_height);
  GetAbsolutePointerService().UpdatePresentation(attached_window_->GetGuestOutputTransform(),
                                                 safe_x, safe_y, safe_width, safe_height,
                                                 timestamp_ns);
}

X_RESULT SDLInputDriver::GetCapabilities(uint32_t user_index, uint32_t flags,
                                         X_INPUT_CAPABILITIES* out_caps) {
  if (!accepting_input_requests_.load(std::memory_order_acquire)) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }
  if (user_index >= HID_SDL_USER_COUNT || !out_caps) {
    return X_ERROR_BAD_ARGUMENTS;
  }

  auto guard = DrainAndLock();
  if (!expose_gamepad_state_) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }

  auto controller = GetControllerState(user_index);
  if (!controller) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }

  // Unfortunately drivers can't present all information immediately (e.g.
  // battery information) so this needs to be refreshed every time.
  UpdateXCapabilities(*controller);

  std::memcpy(out_caps, &controller->caps, sizeof(*out_caps));

  return X_ERROR_SUCCESS;
}

X_RESULT SDLInputDriver::GetState(uint32_t user_index, X_INPUT_STATE* out_state) {
  if (!accepting_input_requests_.load(std::memory_order_acquire)) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }
  if (user_index >= HID_SDL_USER_COUNT) {
    return X_ERROR_BAD_ARGUMENTS;
  }

  auto is_active = this->is_active();

  auto guard = DrainAndLock();
  if (!expose_gamepad_state_) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }

  auto controller = GetControllerState(user_index);
  if (!controller) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }

  // Make sure packet_number is only incremented by 1, even if there have been
  // multiple updates between GetState calls. Also track `is_active` to
  // increment the packet number if it changed.
  if ((is_active != controller->is_active) || (is_active && controller->state_changed)) {
    controller->state.packet_number++;
    controller->is_active = is_active;
    controller->state_changed = false;
  }
  std::memcpy(out_state, &controller->state, sizeof(*out_state));
  if (!is_active) {
    // Simulate an "untouched" controller. When we become active again the
    // pressed buttons aren't lost and will be visible again.
    std::memset(&out_state->gamepad, 0, sizeof(out_state->gamepad));
  }
  return X_ERROR_SUCCESS;
}

bool SDLInputDriver::TryGetMotionState(uint32_t user_index, MotionState* out_state) {
  if (!accepting_input_requests_.load(std::memory_order_acquire)) {
    return false;
  }
  if (!out_state || user_index >= HID_SDL_USER_COUNT || !is_active()) {
    return false;
  }
  auto guard = DrainAndLock();
  if (!expose_gamepad_state_) {
    return false;
  }
  auto controller = GetControllerState(user_index);
  if (!controller || controller->motion.available_sensors == kMotionSensorNone) {
    return false;
  }

  MotionState sampled{};
  if (!motion_samples_.Read(SDL_GetGamepadID(controller->sdl), SDL_GetTicksNS(), sampled)) {
    return false;
  }
  // Sample the poll clock after the cache copy, so a concurrent delivery
  // cannot appear to come from the future and spuriously reset calibration.
  sampled.poll_host_timestamp_ns = SDL_GetTicksNS();
  sampled.available_sensors = controller->motion.available_sensors;
  sampled.valid_samples &= sampled.available_sensors;
  sampled.accelerometer_rate_hz = controller->motion.accelerometer_rate_hz;
  sampled.gyroscope_rate_hz = controller->motion.gyroscope_rate_hz;
  // Do not call a current-state getter and restamp its cached data as new.
  *out_state = sampled;
  return true;
}

X_RESULT SDLInputDriver::SetState(uint32_t user_index, X_INPUT_VIBRATION* vibration) {
  if (!accepting_input_requests_.load(std::memory_order_acquire)) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }
  if (user_index >= HID_SDL_USER_COUNT || !vibration) {
    return X_ERROR_BAD_ARGUMENTS;
  }

  auto guard = DrainAndLock(false);
  if (!expose_gamepad_state_) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }

  auto controller = GetControllerState(user_index);
  if (!controller) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }

  const uint16_t left_motor = vibration->left_motor_speed;
  const uint16_t right_motor = vibration->right_motor_speed;
  if (!controller->rumble_supported) {
    if (!left_motor && !right_motor) {
      return X_ERROR_SUCCESS;
    }
    REXLOG_WARN(
        "SDL HID: Vibration requested for player index {}, but '{}' does not "
        "report rumble support.",
        user_index, SDL_GetGamepadName(controller->sdl));
    return X_ERROR_FUNCTION_FAILED;
  }

  return ApplyRumbleLocked(user_index, *controller, left_motor, right_motor, false);
}

X_RESULT SDLInputDriver::GetKeystroke(uint32_t users, uint32_t flags,
                                      X_INPUT_KEYSTROKE* out_keystroke) {
  // TODO(JoelLinn): Figure out the flags
  // https://github.com/evilC/UCR/blob/0489929e2a8e39caa3484c67f3993d3fba39e46f/Libraries/XInput.ahk#L85-L98
  if (!accepting_input_requests_.load(std::memory_order_acquire)) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }
  bool user_any = users == 0xFF;
  if (users >= HID_SDL_USER_COUNT && !user_any) {
    return X_ERROR_BAD_ARGUMENTS;
  }
  if (!out_keystroke) {
    return X_ERROR_BAD_ARGUMENTS;
  }
  // The order of this list is also the order in which events are send if
  // multiple buttons change at once.
  static_assert(sizeof(X_INPUT_GAMEPAD::buttons) == 2);
  static constexpr std::array<rex::ui::VirtualKey, 34> kVkLookup = {
      // 00 - True buttons from xinput button field
      rex::ui::VirtualKey::kXInputPadDpadUp,
      rex::ui::VirtualKey::kXInputPadDpadDown,
      rex::ui::VirtualKey::kXInputPadDpadLeft,
      rex::ui::VirtualKey::kXInputPadDpadRight,
      rex::ui::VirtualKey::kXInputPadStart,
      rex::ui::VirtualKey::kXInputPadBack,
      rex::ui::VirtualKey::kXInputPadLThumbPress,
      rex::ui::VirtualKey::kXInputPadRThumbPress,
      rex::ui::VirtualKey::kXInputPadLShoulder,
      rex::ui::VirtualKey::kXInputPadRShoulder,
      rex::ui::VirtualKey::kNone, /* Guide has no VK */
      rex::ui::VirtualKey::kNone, /* Unknown */
      rex::ui::VirtualKey::kXInputPadA,
      rex::ui::VirtualKey::kXInputPadB,
      rex::ui::VirtualKey::kXInputPadX,
      rex::ui::VirtualKey::kXInputPadY,
      // 16 - Fake buttons generated from analog inputs
      rex::ui::VirtualKey::kXInputPadLTrigger,
      rex::ui::VirtualKey::kXInputPadRTrigger,
      // 18
      rex::ui::VirtualKey::kXInputPadLThumbUp,
      rex::ui::VirtualKey::kXInputPadLThumbDown,
      rex::ui::VirtualKey::kXInputPadLThumbRight,
      rex::ui::VirtualKey::kXInputPadLThumbLeft,
      rex::ui::VirtualKey::kXInputPadLThumbUpLeft,
      rex::ui::VirtualKey::kXInputPadLThumbUpRight,
      rex::ui::VirtualKey::kXInputPadLThumbDownRight,
      rex::ui::VirtualKey::kXInputPadLThumbDownLeft,
      // 26
      rex::ui::VirtualKey::kXInputPadRThumbUp,
      rex::ui::VirtualKey::kXInputPadRThumbDown,
      rex::ui::VirtualKey::kXInputPadRThumbRight,
      rex::ui::VirtualKey::kXInputPadRThumbLeft,
      rex::ui::VirtualKey::kXInputPadRThumbUpLeft,
      rex::ui::VirtualKey::kXInputPadRThumbUpRight,
      rex::ui::VirtualKey::kXInputPadRThumbDownRight,
      rex::ui::VirtualKey::kXInputPadRThumbDownLeft,
  };

  auto is_active = this->is_active();

  auto guard = DrainAndLock();
  if (!expose_gamepad_state_) {
    return X_ERROR_DEVICE_NOT_CONNECTED;
  }

  for (uint32_t user_index = (user_any ? 0 : users);
       user_index < (user_any ? HID_SDL_USER_COUNT : users + 1); user_index++) {
    auto controller = GetControllerState(user_index);
    if (!controller) {
      if (user_any) {
        continue;
      } else {
        return X_ERROR_DEVICE_NOT_CONNECTED;
      }
    }

    // If input is not active (e.g. due to a dialog overlay), force buttons to
    // "unpressed". The algorithm will automatically send UP events when
    // `is_active()` goes low and DOWN events when it goes high again.
    const uint64_t curr_butts =
        is_active
            ? (controller->state.gamepad.buttons | AnalogToKeyfield(controller->state.gamepad))
            : uint64_t(0);
    KeystrokeState& last = keystroke_states_.at(user_index);

    // Handle repeating
    auto guest_now = rex::chrono::Clock::QueryGuestUptimeMillis();
    static_assert(HID_SDL_REPEAT_DELAY >= HID_SDL_REPEAT_RATE);
    if (last.repeat_state == RepeatState::Waiting &&
        (last.repeat_time + HID_SDL_REPEAT_DELAY < guest_now)) {
      last.repeat_state = RepeatState::Repeating;
    }
    if (last.repeat_state == RepeatState::Repeating &&
        (last.repeat_time + HID_SDL_REPEAT_RATE < guest_now)) {
      last.repeat_time = guest_now;
      rex::ui::VirtualKey vk = kVkLookup.at(last.repeat_butt_idx);
      assert_true(vk != rex::ui::VirtualKey::kNone);
      out_keystroke->virtual_key = uint16_t(vk);
      out_keystroke->unicode = 0;
      out_keystroke->user_index = user_index;
      out_keystroke->hid_code = 0;
      out_keystroke->flags = X_INPUT_KEYSTROKE_KEYDOWN | X_INPUT_KEYSTROKE_REPEAT;
      return X_ERROR_SUCCESS;
    }

    auto butts_changed = curr_butts ^ last.buttons;
    if (!butts_changed) {
      continue;
    }

    // First try to clear buttons with up events. This is to match xinput
    // behaviour when transitioning thumb sticks, e.g. so that THUMB_UPLEFT is
    // up before THUMB_LEFT is down.
    for (auto [clear_pass, i] = std::tuple{true, 0}; i < 2; clear_pass = false, i++) {
      for (uint8_t i = 0; i < uint8_t(std::size(kVkLookup)); i++) {
        auto fbutton = uint64_t(1) << i;
        if (!(butts_changed & fbutton)) {
          continue;
        }
        rex::ui::VirtualKey vk = kVkLookup.at(i);
        if (vk == rex::ui::VirtualKey::kNone) {
          continue;
        }

        out_keystroke->virtual_key = uint16_t(vk);
        out_keystroke->unicode = 0;
        out_keystroke->user_index = user_index;
        out_keystroke->hid_code = 0;

        bool is_pressed = curr_butts & fbutton;
        if (clear_pass && !is_pressed) {
          // up
          out_keystroke->flags = X_INPUT_KEYSTROKE_KEYUP;
          last.buttons &= ~fbutton;
          last.repeat_state = RepeatState::Idle;
          return X_ERROR_SUCCESS;
        }
        if (!clear_pass && is_pressed) {
          // down
          out_keystroke->flags = X_INPUT_KEYSTROKE_KEYDOWN;
          last.buttons |= fbutton;
          last.repeat_state = RepeatState::Waiting;
          last.repeat_butt_idx = i;
          last.repeat_time = guest_now;
          return X_ERROR_SUCCESS;
        }
      }
    }
  }
  return X_ERROR_EMPTY;
}

void SDLInputDriver::HandleEvent(const SDL_Event& event) {
  // This callback will likely run on the thread that posts the event, which
  // may be a dedicated thread SDL has created for the joystick subsystem.

  switch (event.type) {
    case SDL_EVENT_GAMEPAD_SENSOR_UPDATE: {
      const uint32_t sensor = event.gsensor.sensor == SDL_SENSOR_ACCEL
                                  ? kMotionSensorAccelerometer
                                  : event.gsensor.sensor == SDL_SENSOR_GYRO
                                        ? kMotionSensorGyroscope : kMotionSensorNone;
      motion_samples_.Observe(event.gsensor.which, sensor,
                              {event.gsensor.data[0], event.gsensor.data[1], event.gsensor.data[2]},
                              event.gsensor.timestamp, event.gsensor.sensor_timestamp);
      return;
    }
    case SDL_EVENT_KEYBOARD_ADDED:
      GetAbsolutePointerService().AddPhysicalKeyboard(uint64_t(event.kdevice.which),
                                                      event.kdevice.timestamp);
      return;
    case SDL_EVENT_KEYBOARD_REMOVED:
      GetAbsolutePointerService().RemovePhysicalKeyboard(uint64_t(event.kdevice.which),
                                                         event.kdevice.timestamp);
      return;
    case SDL_EVENT_GAMEPAD_ADDED:
      GetAbsolutePointerService().AddGameController(uint64_t(event.gdevice.which),
                                                    event.gdevice.timestamp);
      break;
    case SDL_EVENT_GAMEPAD_REMOVED:
      GetAbsolutePointerService().RemoveGameController(uint64_t(event.gdevice.which),
                                                       event.gdevice.timestamp);
      break;
    default:
      break;
  }
}

std::unique_lock<std::mutex> SDLInputDriver::DrainAndLock(bool refresh_rumble) {
  std::unique_lock<std::mutex> guard(controllers_mutex_);
  if (!accepting_input_requests_.load(std::memory_order_acquire)) {
    return guard;
  }

  // Queue the main-thread event pump for platform window and keyboard state,
  // but don't wait for it: SDL documents the gamepad update, inventory and
  // state getters below as thread-safe current-state APIs.
  QueueControllerUpdate();
  if (!expose_gamepad_state_) {
    return guard;
  }

  SDL_UpdateGamepads();
  const auto gamepad_inventory = QueryControllerInventory();
  if (gamepad_inventory) {
    ReconcileControllerInventoryLocked(*gamepad_inventory);
  }
  PollConnectedControllerStatesLocked();
  if (refresh_rumble) {
    RefreshRumbleLocked();
  }
  return guard;
}

void SDLInputDriver::CloseControllerLocked(size_t index, const char* reason) {
  auto& state = controllers_.at(index);
  if (!state.sdl) {
    return;
  }
  const SDL_JoystickID instance_id = SDL_GetGamepadID(state.sdl);
  if (SDL_GamepadConnected(state.sdl)) {
    if (state.rumble_supported && (state.left_motor_speed || state.right_motor_speed)) {
      SDL_RumbleGamepad(state.sdl, 0, 0, 0);
    }
    if (state.motion.available_sensors & kMotionSensorAccelerometer) {
      SDL_SetGamepadSensorEnabled(state.sdl, SDL_SENSOR_ACCEL, false);
    }
    if (state.motion.available_sensors & kMotionSensorGyroscope) {
      SDL_SetGamepadSensorEnabled(state.sdl, SDL_SENSOR_GYRO, false);
    }
  }
  motion_samples_.EndDevice(instance_id);
  SDL_CloseGamepad(state.sdl);
  state = {};
  keystroke_states_.at(index) = {};
  REXLOG_INFO("SDL controller closed: player-index={} instance-id={} reason={}", index, instance_id,
              reason ? reason : "unknown");
}

void SDLInputDriver::ReconcileControllerInventoryLocked(
    const std::vector<SDL_JoystickID>& connected_ids) {
  for (size_t index = 0; index < controllers_.size(); ++index) {
    ControllerState& state = controllers_.at(index);
    if (state.sdl && (!SDL_GamepadConnected(state.sdl) || !SDL_GetGamepadID(state.sdl))) {
      CloseControllerLocked(index, "disconnected-before-inventory");
    }
  }

  std::vector<uint64_t> open_ids;
  open_ids.reserve(controllers_.size());
  for (const ControllerState& state : controllers_) {
    if (state.sdl) {
      const SDL_JoystickID id = SDL_GetGamepadID(state.sdl);
      if (id) {
        open_ids.push_back(uint64_t(id));
      }
    }
  }
  std::vector<uint64_t> inventory_ids;
  inventory_ids.reserve(connected_ids.size());
  for (const SDL_JoystickID id : connected_ids) {
    if (id) {
      inventory_ids.push_back(uint64_t(id));
    }
  }

  const GamepadInventoryPlan plan = PlanGamepadInventory(open_ids, inventory_ids);
  for (const uint64_t id : plan.remove) {
    const auto index = GetControllerIndexFromInstanceID(static_cast<SDL_JoystickID>(id));
    if (index) {
      CloseControllerLocked(*index, "absent-from-inventory");
    }
  }
  for (const uint64_t id : plan.add) {
    OpenControllerLocked(static_cast<SDL_JoystickID>(id));
  }
}

void SDLInputDriver::OpenControllerLocked(SDL_JoystickID instance_id) {
  if (!instance_id || GetControllerIndexFromInstanceID(instance_id)) {
    return;
  }
  // Open the controller.
  const auto controller = SDL_OpenGamepad(instance_id);
  if (!controller) {
    REXLOG_WARN("SDL OpenController: Failed to open instance {}: {}", instance_id, SDL_GetError());
    return;
  }
  const char* path = SDL_GetGamepadPath(controller);
  const char* name = SDL_GetGamepadName(controller);
  const char* serial = SDL_GetGamepadSerial(controller);
  REXLOG_INFO(
      "SDL OpenController: instance-id={} path='{}' serial='{}' \"{}\", "
      "JoystickType({}), "
      "GameControllerType({}), "
      "VendorID(0x{:04X}), "
      "ProductID(0x{:04X})",
      SDL_GetGamepadID(controller), path ? path : "", serial ? serial : "", name ? name : "",
      static_cast<int>(SDL_GetJoystickType(SDL_GetGamepadJoystick(controller))),
      static_cast<int>(SDL_GetGamepadType(controller)), SDL_GetGamepadVendor(controller),
      SDL_GetGamepadProduct(controller));
  int user_id = -1;
  // Check if the controller has a player index LED.
  user_id = SDL_GetGamepadPlayerIndex(controller);
  // Is that id already taken?
  if (user_id < 0 || user_id >= static_cast<int>(controllers_.size()) ||
      controllers_.at(user_id).sdl) {
    user_id = -1;
  }
  // No player index or already taken, just take the first free slot.
  if (user_id < 0) {
    for (size_t i = 0; i < controllers_.size(); i++) {
      if (!controllers_.at(i).sdl) {
        user_id = static_cast<int>(i);
#if SDL_VERSION_ATLEAST(2, 0, 12)
        SDL_SetGamepadPlayerIndex(controller, user_id);
#endif
        break;
      }
    }
  }
  if (user_id >= 0) {
    auto& state = controllers_.at(user_id);
    state = {};
    state.sdl = controller;
    state.motion.device_generation =
        next_motion_device_generation_.fetch_add(1, std::memory_order_relaxed);
    motion_samples_.BeginDevice(instance_id, state.motion.device_generation, SDL_GetTicksNS());
    SDL_SetEventEnabled(SDL_EVENT_GAMEPAD_SENSOR_UPDATE, true);

    auto enable_sensor = [&](SDL_SensorType sensor, uint32_t flag, float& out_rate) {
      if (!SDL_GamepadHasSensor(controller, sensor)) {
        return;
      }
      if (!SDL_SetGamepadSensorEnabled(controller, sensor, true)) {
        REXLOG_WARN("SDL OpenController: Failed to enable sensor {}: {}", static_cast<int>(sensor),
                    SDL_GetError());
        return;
      }
      state.motion.available_sensors |= flag;
      out_rate = SDL_GetGamepadSensorDataRate(controller, sensor);
      REXLOG_INFO("SDL OpenController: Enabled sensor {} at {} Hz.", static_cast<int>(sensor),
                  out_rate);
    };
    enable_sensor(SDL_SENSOR_ACCEL, kMotionSensorAccelerometer, state.motion.accelerometer_rate_hz);
    enable_sensor(SDL_SENSOR_GYRO, kMotionSensorGyroscope, state.motion.gyroscope_rate_hz);
    // XInput seems to start with packet_number = 1 .
    state.state_changed = true;
    UpdateXCapabilities(state);

    REXLOG_INFO(
        "SDL OpenController: instance-id={} added at player-index={} "
        "(rumble_supported={}).",
        SDL_GetGamepadID(controller), user_id, state.rumble_supported);
  } else {
    // No more controllers needed, close it.
    SDL_CloseGamepad(controller);
    REXLOG_WARN("SDL OpenController: Ignored. No free slots.");
  }
}

void SDLInputDriver::PollControllerStateLocked(ControllerState& state) {
  if (!state.sdl || !SDL_GamepadConnected(state.sdl)) {
    return;
  }

  X_INPUT_GAMEPAD polled{};
  polled.thumb_lx = SDL_GetGamepadAxis(state.sdl, SDL_GAMEPAD_AXIS_LEFTX);
  polled.thumb_ly = ToXInputThumbY(SDL_GetGamepadAxis(state.sdl, SDL_GAMEPAD_AXIS_LEFTY));
  polled.thumb_rx = SDL_GetGamepadAxis(state.sdl, SDL_GAMEPAD_AXIS_RIGHTX);
  polled.thumb_ry = ToXInputThumbY(SDL_GetGamepadAxis(state.sdl, SDL_GAMEPAD_AXIS_RIGHTY));
  polled.left_trigger =
      static_cast<uint8_t>(SDL_GetGamepadAxis(state.sdl, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) >> 7);
  polled.right_trigger =
      static_cast<uint8_t>(SDL_GetGamepadAxis(state.sdl, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) >> 7);

  for (size_t button_index = 0; button_index < kXInputButtonFromSDL.size(); ++button_index) {
    if (!SDL_GetGamepadButton(state.sdl, static_cast<SDL_GamepadButton>(button_index))) {
      continue;
    }
    const auto xbutton = kXInputButtonFromSDL[button_index];
    if (xbutton && (xbutton != X_INPUT_GAMEPAD_GUIDE || REXCVAR_GET(guide_button))) {
      polled.buttons = static_cast<uint16_t>(polled.buttons) | xbutton;
    }
  }

  const bool changed = std::memcmp(&polled, &state.state.gamepad, sizeof(polled)) != 0;
  if (rex::input::IsInputTraceEnabled() && (!state.trace_initialized || changed)) {
    const uint64_t sequence = rex::input::NextInputTraceSequence();
    REXLOG_INFO(
        "input-e2e: seq={} stage=sdl-gamepad instance={} buttons={:04X} triggers={}/{} "
        "sticks={}/{}/{}/{}",
        sequence, SDL_GetGamepadID(state.sdl), static_cast<uint16_t>(polled.buttons),
        polled.left_trigger, polled.right_trigger, static_cast<int16_t>(polled.thumb_lx),
        static_cast<int16_t>(polled.thumb_ly), static_cast<int16_t>(polled.thumb_rx),
        static_cast<int16_t>(polled.thumb_ry));
    state.trace_initialized = true;
  }
  if (changed) {
    state.state.gamepad = polled;
    state.state_changed = true;
  }
}

void SDLInputDriver::PollConnectedControllerStatesLocked() {
  for (ControllerState& state : controllers_) {
    PollControllerStateLocked(state);
  }
}

std::optional<size_t> SDLInputDriver::GetControllerIndexFromInstanceID(SDL_JoystickID instance_id) {
  // Loop through our controllers and try to match the given ID.
  for (size_t i = 0; i < controllers_.size(); i++) {
    auto controller = controllers_.at(i).sdl;
    if (!controller) {
      continue;
    }
    if (SDL_GetGamepadID(controller) == instance_id) {
      return i;
    }
  }
  return std::nullopt;
}

SDLInputDriver::ControllerState* SDLInputDriver::GetControllerState(uint32_t user_index) {
  if (user_index >= controllers_.size()) {
    return nullptr;
  }
  auto controller = &controllers_.at(user_index);
  if (!controller->sdl || !SDL_GamepadConnected(controller->sdl)) {
    return nullptr;
  }
  return controller;
}

bool SDLInputDriver::TestSDLVersion() const {
  REXLOG_INFO("SDL: Using version {}.{}.{}", SDL_MAJOR_VERSION, SDL_MINOR_VERSION,
              SDL_MICRO_VERSION);
  return true;
}

void SDLInputDriver::UpdateXCapabilities(ControllerState& state) {
  assert(state.sdl);
  uint16_t cap_flags = 0x0;

  // The RAWINPUT driver combines and enhances input from different APIs. For
  // details, see `SDL_rawinputjoystick.c`. This correlation however has latency
  // which might confuse games calling `GetCapabilities()` (The power level is
  // only available after the controller has been "touched"). Generally that
  // should not be a problem, when in doubt disable the RAWINPUT driver via hint
  // (env var).

  if (SDL_GetJoystickConnectionState(SDL_GetGamepadJoystick(state.sdl)) ==
      SDL_JOYSTICK_CONNECTION_WIRELESS) {
    cap_flags |= X_INPUT_CAPS_WIRELESS;
  }

  const SDL_PropertiesID properties = SDL_GetGamepadProperties(state.sdl);
  const bool rumble_supported =
      properties != 0 &&
      SDL_GetBooleanProperty(properties, SDL_PROP_GAMEPAD_CAP_RUMBLE_BOOLEAN, false);
  if (!rumble_supported && state.rumble_supported &&
      (state.left_motor_speed || state.right_motor_speed)) {
    SDL_RumbleGamepad(state.sdl, 0, 0, 0);
    state.left_motor_speed = 0;
    state.right_motor_speed = 0;
    state.next_rumble_refresh_ms = 0;
  }
  state.rumble_supported = rumble_supported;
  if (rumble_supported) {
    cap_flags |= X_INPUT_CAPS_FFB_SUPPORTED;
  }

  // Check if all navigational buttons are present
  static constexpr std::array<SDL_GamepadButton, 6> nav_buttons = {
      SDL_GAMEPAD_BUTTON_START,     SDL_GAMEPAD_BUTTON_BACK,      SDL_GAMEPAD_BUTTON_DPAD_UP,
      SDL_GAMEPAD_BUTTON_DPAD_DOWN, SDL_GAMEPAD_BUTTON_DPAD_LEFT, SDL_GAMEPAD_BUTTON_DPAD_RIGHT,
  };
  for (auto it = nav_buttons.begin(); it < nav_buttons.end(); it++) {
    if (!SDL_GamepadHasButton(state.sdl, *it)) {
      cap_flags |= X_INPUT_CAPS_NO_NAVIGATION;
      break;
    }
  }

  auto& c = state.caps;
  c.type = 0x01;      // XINPUT_DEVTYPE_GAMEPAD
  c.sub_type = 0x01;  // XINPUT_DEVSUBTYPE_GAMEPAD
  c.flags = cap_flags;
  c.gamepad.buttons = 0xF3FF | (REXCVAR_GET(guide_button) ? X_INPUT_GAMEPAD_GUIDE : 0x0);
  c.gamepad.left_trigger = 0xFF;
  c.gamepad.right_trigger = 0xFF;
  c.gamepad.thumb_lx = static_cast<int16_t>(0xFFFFu);
  c.gamepad.thumb_ly = static_cast<int16_t>(0xFFFFu);
  c.gamepad.thumb_rx = static_cast<int16_t>(0xFFFFu);
  c.gamepad.thumb_ry = static_cast<int16_t>(0xFFFFu);
  c.vibration.left_motor_speed = rumble_supported ? 0xFFFFu : 0;
  c.vibration.right_motor_speed = rumble_supported ? 0xFFFFu : 0;
}

X_RESULT SDLInputDriver::ApplyRumbleLocked(uint32_t user_index, ControllerState& state,
                                           uint16_t left_motor, uint16_t right_motor,
                                           bool is_refresh) {
  const uint32_t duration_ms = GetRumbleDurationMs(left_motor, right_motor);
  const bool succeeded = SDL_RumbleGamepad(state.sdl, left_motor, right_motor, duration_ms);
  if (!succeeded) {
    state.left_motor_speed = 0;
    state.right_motor_speed = 0;
    state.next_rumble_refresh_ms = 0;
    REXLOG_WARN("SDL HID: {} vibration failed for player index {} on '{}': {}",
                is_refresh ? "Refreshing" : "Setting", user_index, SDL_GetGamepadName(state.sdl),
                SDL_GetError());
    return TranslateSdlRumbleResult(false);
  }

  state.left_motor_speed = left_motor;
  state.right_motor_speed = right_motor;
  state.next_rumble_refresh_ms = duration_ms ? SDL_GetTicks() + kRumbleRefreshIntervalMs : 0;
  return TranslateSdlRumbleResult(true);
}

void SDLInputDriver::RefreshRumbleLocked() {
  const uint64_t now_ms = SDL_GetTicks();
  for (uint32_t user_index = 0; user_index < controllers_.size(); ++user_index) {
    ControllerState& state = controllers_[user_index];
    if (!state.sdl || !state.rumble_supported ||
        (!state.left_motor_speed && !state.right_motor_speed) ||
        !ShouldRefreshRumble(now_ms, state.next_rumble_refresh_ms)) {
      continue;
    }
    ApplyRumbleLocked(user_index, state, state.left_motor_speed, state.right_motor_speed, true);
  }
}

void SDLInputDriver::QueueControllerUpdate() {
  if (!accepting_input_requests_.load(std::memory_order_acquire) || !attached_window_) {
    return;
  }
  // Keep platform events and host keyboard/touch inventory moving on SDL's
  // required UI thread. Gamepad state itself is sampled synchronously by
  // DrainAndLock through SDL's thread-safe polling API.
  bool is_queued = false;
  sdl_pumpevents_queued_.compare_exchange_strong(is_queued, true);
  if (!is_queued) {
    if (!attached_window_->app_context().CallInUIThread([this]() {
          SDL_PumpEvents();
          const uint64_t timestamp_ns = SDL_GetTicksNS();
          RefreshDeviceInventoryFromUIThread(timestamp_ns);
          RefreshAndroidKeyboardFromUIThread(timestamp_ns, false);
          RefreshPointerPresentation(timestamp_ns);
          sdl_pumpevents_queued_ = false;
        })) {
      sdl_pumpevents_queued_ = false;
    }
  }
}

// Check if the analog inputs exceed their thresholds to become a button press
// and build the bitfield.
inline uint64_t SDLInputDriver::AnalogToKeyfield(const X_INPUT_GAMEPAD& gamepad) const {
  uint64_t f = 0;

  f |= static_cast<uint64_t>(gamepad.left_trigger > HID_SDL_TRIGG_THRES) << 16;
  f |= static_cast<uint64_t>(gamepad.right_trigger > HID_SDL_TRIGG_THRES) << 17;

  auto thumb_x = static_cast<int16_t>(gamepad.thumb_lx);
  auto thumb_y = static_cast<int16_t>(gamepad.thumb_ly);
  for (size_t i = 0; i <= 8; i = i + 8) {
    uint64_t u = thumb_y > HID_SDL_THUMB_THRES;
    uint64_t d = thumb_y < ~HID_SDL_THUMB_THRES;
    uint64_t r = thumb_x > HID_SDL_THUMB_THRES;
    uint64_t l = thumb_x < ~HID_SDL_THUMB_THRES;
    if (u && l) {
      u = l = 0;
      f |= uint64_t(1) << (22 + i);
    }
    if (u && r) {
      u = r = 0;
      f |= uint64_t(1) << (23 + i);
    }
    if (d && r) {
      d = r = 0;
      f |= uint64_t(1) << (24 + i);
    }
    if (d && l) {
      d = l = 0;
      f |= uint64_t(1) << (25 + i);
    }
    f |= u << (18 + i);
    f |= d << (19 + i);
    f |= r << (20 + i);
    f |= l << (21 + i);

    thumb_x = static_cast<int16_t>(gamepad.thumb_rx);
    thumb_y = static_cast<int16_t>(gamepad.thumb_ry);
  }
  return f;
}

}  // namespace rex::input::sdl
