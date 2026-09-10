/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2013 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

#include <algorithm>
#include <cmath>

#include <rex/dbg.h>
#include <rex/input/flags.h>
#include <rex/input/absolute_pointer.h>
#include <rex/input/mnk/controller_compatibility.h>
#include <rex/input/input_driver.h>
#include <rex/input/input_system.h>
#include <rex/input/input_trace.h>
#include <rex/input/mnk/mnk_input_driver.h>
#include <rex/input/nop/nop_input_driver.h>
#include <rex/input/sdl/sdl_input_driver.h>
#include <rex/input/xinput/xinput_input_driver.h>
#include <rex/logging.h>

REXCVAR_DEFINE_STRING(input_backend, "sdl", "Input", "Input backend: sdl, xinput")
    .allowed({"sdl", "xinput"});

REXCVAR_DEFINE_BOOL(guide_button, false, "Input", "Enable guide button pass-through");
namespace rex::input {

namespace {

bool GamepadStatesEqual(const X_INPUT_GAMEPAD& lhs, const X_INPUT_GAMEPAD& rhs) {
  return lhs.buttons == rhs.buttons && lhs.left_trigger == rhs.left_trigger &&
         lhs.right_trigger == rhs.right_trigger && lhs.thumb_lx == rhs.thumb_lx &&
         lhs.thumb_ly == rhs.thumb_ly && lhs.thumb_rx == rhs.thumb_rx &&
         lhs.thumb_ry == rhs.thumb_ry;
}

}  // namespace

InputSystem::InputSystem(rex::ui::Window* window) : window_(window) {}

InputSystem::~InputSystem() = default;

X_STATUS InputSystem::Setup() {
  return X_STATUS_SUCCESS;
}

void InputSystem::Shutdown() {
  drivers_.clear();
}

void InputSystem::AddDriver(std::unique_ptr<InputDriver> driver) {
  drivers_.push_back(std::move(driver));
  driver_trace_trackers_.emplace_back();
}

void InputSystem::AttachWindow(rex::ui::Window* window) {
  window_ = window;
  for (auto& driver : drivers_) {
    driver->OnWindowAvailable(window);
  }
}

void InputSystem::SetActiveCallback(std::function<bool()> callback) {
  is_active_callback_ = callback;
  for (auto& driver : drivers_) {
    driver->set_is_active_callback(callback);
  }
}

X_RESULT InputSystem::GetCapabilities(uint32_t user_index, uint32_t flags,
                                      X_INPUT_CAPABILITIES* out_caps) {
  SCOPE_profile_cpu_f("hid");

  bool any_connected = false;
  for (auto& driver : drivers_) {
    X_RESULT result = driver->GetCapabilities(user_index, flags, out_caps);
    if (result != X_ERROR_DEVICE_NOT_CONNECTED) {
      any_connected = true;
    }
    if (result == X_ERROR_SUCCESS) {
      return result;
    }
  }
  X_INPUT_GAMEPAD virtual_pad{};
  if (TouchControlsActive() && mnk::ReadVirtualControllerCompatibilityGamepad(user_index, virtual_pad)) {
    if (out_caps) {
      *out_caps = {};
      out_caps->type = 1;
      out_caps->sub_type = 1;
      out_caps->gamepad.buttons = 0xFFFF;
      out_caps->gamepad.left_trigger = 255;
      out_caps->gamepad.right_trigger = 255;
    }
    return X_ERROR_SUCCESS;
  }
  return any_connected ? X_ERROR_EMPTY : X_ERROR_DEVICE_NOT_CONNECTED;
}

X_RESULT InputSystem::GetState(uint32_t user_index, X_INPUT_STATE* out_state) {
  SCOPE_profile_cpu_f("hid");

  if (!out_state) {
    // XamInputGetState permits a null output pointer as a connectivity query.
    // Do not manufacture a temporary state here: state polling may transfer
    // ownership of transient input such as relative mouse motion. Capabilities
    // provide the same connection answer without sampling controller state.
    X_INPUT_GAMEPAD virtual_pad{};
    if (TouchControlsActive() && mnk::ReadVirtualControllerCompatibilityGamepad(user_index, virtual_pad))
      return X_ERROR_SUCCESS;
    bool any_connected = false;
    for (auto& driver : drivers_) {
      X_INPUT_CAPABILITIES capabilities = {};
      const X_RESULT result = driver->GetCapabilities(user_index, 0, &capabilities);
      if (result != X_ERROR_DEVICE_NOT_CONNECTED) {
        any_connected = true;
      }
      if (result == X_ERROR_SUCCESS) {
        return result;
      }
    }
    return any_connected ? X_ERROR_EMPTY : X_ERROR_DEVICE_NOT_CONNECTED;
  }

  bool any_connected = false;
  bool first_result = true;
  X_INPUT_STATE merged = {};
  BeginInputTracePoll();

  for (size_t driver_index = 0; driver_index < drivers_.size(); ++driver_index) {
    auto& driver = drivers_[driver_index];
    X_INPUT_STATE state = {};
    X_RESULT result = driver->GetState(user_index, &state);
    if (IsInputTraceEnabled() && user_index < driver_trace_trackers_[driver_index].size()) {
      std::lock_guard trace_lock(trace_mutex_);
      DriverTraceTracker& trace = driver_trace_trackers_[driver_index][user_index];
      const bool changed = !trace.initialized || trace.result != result ||
                           (result == X_ERROR_SUCCESS &&
                            !GamepadStatesEqual(trace.gamepad, state.gamepad));
      if (changed) {
        const uint64_t causal_sequence = InputTraceCausalSequence();
        const uint64_t sequence =
            causal_sequence != 0 ? causal_sequence : NextInputTraceSequence();
        REXLOG_INFO(
            "input-e2e: seq={} stage=driver-state driver={} user={} result={:08X} packet={} "
            "buttons={:04X} triggers={}/{} sticks={}/{}/{}/{}",
            sequence, driver->trace_name(), user_index, static_cast<uint32_t>(result),
            static_cast<uint32_t>(state.packet_number),
            static_cast<uint16_t>(state.gamepad.buttons), state.gamepad.left_trigger,
            state.gamepad.right_trigger, static_cast<int16_t>(state.gamepad.thumb_lx),
            static_cast<int16_t>(state.gamepad.thumb_ly),
            static_cast<int16_t>(state.gamepad.thumb_rx),
            static_cast<int16_t>(state.gamepad.thumb_ry));
        trace.initialized = true;
        trace.result = result;
        trace.gamepad = state.gamepad;
      }
    }
    if (result != X_ERROR_DEVICE_NOT_CONNECTED) {
      any_connected = true;
    }
    if (result == X_ERROR_SUCCESS) {
      if (first_result) {
        merged = state;
        first_result = false;
      } else {
        // Merge: OR buttons, max triggers, max-magnitude sticks
        merged.gamepad.buttons = static_cast<uint16_t>(merged.gamepad.buttons) |
                                 static_cast<uint16_t>(state.gamepad.buttons);
        merged.gamepad.left_trigger =
            std::max(merged.gamepad.left_trigger, state.gamepad.left_trigger);
        merged.gamepad.right_trigger =
            std::max(merged.gamepad.right_trigger, state.gamepad.right_trigger);

        auto merge_axis = [](int16_t a, int16_t b) -> int16_t {
          return (std::abs(static_cast<int>(a)) >= std::abs(static_cast<int>(b))) ? a : b;
        };
        merged.gamepad.thumb_lx = merge_axis(merged.gamepad.thumb_lx, state.gamepad.thumb_lx);
        merged.gamepad.thumb_ly = merge_axis(merged.gamepad.thumb_ly, state.gamepad.thumb_ly);
        merged.gamepad.thumb_rx = merge_axis(merged.gamepad.thumb_rx, state.gamepad.thumb_rx);
        merged.gamepad.thumb_ry = merge_axis(merged.gamepad.thumb_ry, state.gamepad.thumb_ry);

        if (static_cast<uint32_t>(state.packet_number) >
            static_cast<uint32_t>(merged.packet_number)) {
          merged.packet_number = state.packet_number;
        }
      }
    }
  }

  X_INPUT_GAMEPAD virtual_pad{};
  if (TouchControlsActive() && mnk::ReadVirtualControllerCompatibilityGamepad(user_index, virtual_pad)) {
    // Same host-UI ownership gate as every physical driver. Keep connectivity
    // but publish a neutral virtual source when the title cannot accept input.
    if (is_active_callback_ && !is_active_callback_()) virtual_pad = {};
    merged.gamepad.buttons = static_cast<uint16_t>(merged.gamepad.buttons) |
                             static_cast<uint16_t>(virtual_pad.buttons);
    merged.gamepad.left_trigger = std::max(merged.gamepad.left_trigger, virtual_pad.left_trigger);
    merged.gamepad.right_trigger = std::max(merged.gamepad.right_trigger, virtual_pad.right_trigger);
    const auto axis = [](int16_t a, int16_t b) -> int16_t {
      return std::abs(int(a)) >= std::abs(int(b)) ? a : b;
    };
    merged.gamepad.thumb_lx = axis(merged.gamepad.thumb_lx, virtual_pad.thumb_lx);
    merged.gamepad.thumb_ly = axis(merged.gamepad.thumb_ly, virtual_pad.thumb_ly);
    first_result = false;
  }

  if (first_result) {
    if (user_index < merged_state_trackers_.size()) {
      std::lock_guard lock(merged_state_mutex_);
      MergedStateTracker& tracker = merged_state_trackers_[user_index];
      tracker.gamepad = {};
      tracker.available = false;
    }
    return any_connected ? X_ERROR_EMPTY : X_ERROR_DEVICE_NOT_CONNECTED;
  }

  if (user_index < merged_state_trackers_.size()) {
    std::lock_guard lock(merged_state_mutex_);
    MergedStateTracker& tracker = merged_state_trackers_[user_index];
    const bool merged_changed = !tracker.initialized || !GamepadStatesEqual(tracker.gamepad,
                                                                            merged.gamepad);
    if (!tracker.initialized) {
      tracker.packet_number = merged.packet_number;
      tracker.initialized = true;
    } else if (!GamepadStatesEqual(tracker.gamepad, merged.gamepad)) {
      ++tracker.packet_number;
    }
    tracker.gamepad = merged.gamepad;
    tracker.available = true;
    merged.packet_number = tracker.packet_number;
    if (IsInputTraceEnabled() && merged_changed) {
      const uint64_t causal_sequence = InputTraceCausalSequence();
      const uint64_t sequence =
          causal_sequence != 0 ? causal_sequence : NextInputTraceSequence();
      REXLOG_INFO(
          "input-e2e: seq={} stage=merged-state user={} packet={} buttons={:04X} "
          "triggers={}/{} sticks={}/{}/{}/{}",
          sequence, user_index, tracker.packet_number,
          static_cast<uint16_t>(merged.gamepad.buttons), merged.gamepad.left_trigger,
          merged.gamepad.right_trigger, static_cast<int16_t>(merged.gamepad.thumb_lx),
          static_cast<int16_t>(merged.gamepad.thumb_ly),
          static_cast<int16_t>(merged.gamepad.thumb_rx),
          static_cast<int16_t>(merged.gamepad.thumb_ry));
    }
  }

  *out_state = merged;
  return X_ERROR_SUCCESS;
}

bool InputSystem::TryGetLastState(uint32_t user_index, X_INPUT_STATE* out_state) {
  if (!out_state || user_index >= merged_state_trackers_.size()) {
    return false;
  }
  std::lock_guard lock(merged_state_mutex_);
  const MergedStateTracker& tracker = merged_state_trackers_[user_index];
  if (!tracker.initialized || !tracker.available) {
    return false;
  }
  *out_state = {};
  out_state->packet_number = tracker.packet_number;
  out_state->gamepad = tracker.gamepad;
  return true;
}

X_RESULT InputSystem::SetState(uint32_t user_index, X_INPUT_VIBRATION* vibration) {
  SCOPE_profile_cpu_f("hid");

  X_RESULT first_failure = X_ERROR_DEVICE_NOT_CONNECTED;
  for (auto& driver : drivers_) {
    X_RESULT result = driver->SetState(user_index, vibration);
    if (result == X_ERROR_SUCCESS) {
      return result;
    }
    if (result != X_ERROR_DEVICE_NOT_CONNECTED && first_failure == X_ERROR_DEVICE_NOT_CONNECTED) {
      first_failure = result;
    }
  }
  return first_failure;
}

X_RESULT InputSystem::GetKeystroke(uint32_t user_index, uint32_t flags,
                                   X_INPUT_KEYSTROKE* out_keystroke) {
  SCOPE_profile_cpu_f("hid");

  bool any_connected = false;
  for (auto& driver : drivers_) {
    X_RESULT result = driver->GetKeystroke(user_index, flags, out_keystroke);
    if (result != X_ERROR_DEVICE_NOT_CONNECTED) {
      any_connected = true;
    }
    if (result == X_ERROR_SUCCESS) {
      return result;
    }
  }
  return any_connected ? X_ERROR_EMPTY : X_ERROR_DEVICE_NOT_CONNECTED;
}

bool InputSystem::TryGetMotionState(uint32_t user_index, MotionState* out_state) {
  if (!out_state) {
    return false;
  }

  // Motion samples from different physical controllers are not composable.
  // Select the first sensor-capable driver instead of merging vectors as if
  // they were ordinary XInput axes.
  for (auto& driver : drivers_) {
    MotionState state = {};
    if (driver->TryGetMotionState(user_index, &state)) {
      *out_state = state;
      return true;
    }
  }
  *out_state = {};
  return false;
}

std::unique_ptr<InputSystem> CreateDefaultInputSystem(bool tool_mode) {
  auto input = std::make_unique<InputSystem>(nullptr);

  if (!tool_mode) {
    const bool expose_sdl_gamepad_state = REXCVAR_GET(input_backend) == "sdl";
    auto sdl_driver = std::make_unique<sdl::SDLInputDriver>(nullptr, 0, expose_sdl_gamepad_state);
    if (sdl_driver->Setup() == X_STATUS_SUCCESS) {
      // SDL remains present even when XInput supplies controller state: it is
      // the cross-platform UI-thread authority for touch, keyboard, and
      // controller device inventory.
      input->AddDriver(std::move(sdl_driver));
    }

#if REX_PLATFORM_WIN32
    if (REXCVAR_GET(input_backend) == "xinput") {
      auto xinput_driver = std::make_unique<xinput::XinputInputDriver>(nullptr, 0);
      if (xinput_driver->Setup() == X_STATUS_SUCCESS) {
        input->AddDriver(std::move(xinput_driver));
      }
    }
#endif

    // MnK driver (keyboard/mouse -> controller emulation)
    auto mnk_driver = std::make_unique<mnk::MnkInputDriver>(nullptr, 0);
    if (mnk_driver->Setup() == X_STATUS_SUCCESS) {
      input->AddDriver(std::move(mnk_driver));
    }
  }

  // NOP driver (primary in tool mode, fallback otherwise)
  uint8_t nop_index = tool_mode ? 0 : 1;
  input->AddDriver(std::make_unique<nop::NopInputDriver>(nullptr, nop_index));
  return input;
}

}  // namespace rex::input
