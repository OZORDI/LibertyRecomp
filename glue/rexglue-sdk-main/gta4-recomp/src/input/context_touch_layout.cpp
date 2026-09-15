#include "input/context_touch_layout.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
#include <rex/input/input.h>

namespace gta4::input {
namespace {

ContextTouchLayout BuildSemanticLayout(ContextTouchMode mode, const ContextTouchViewport& viewport,
                                      std::span<const TouchScriptControl> scripts,
                                      const ContextTouchLayoutOptions& options) noexcept;

constexpr float kEdgePaddingRatio = 0.035f;
constexpr float kStickRadiusRatio = 0.105f;
constexpr float kButtonRadiusRatio = 0.0575f;
constexpr float kButtonGapRatio = 0.025f;
constexpr int32_t kActionExtent = 255;

void SetLabel(ContextTouchControl& control, const char* label) {
  std::strncpy(control.label.data(), label, control.label.size() - 1);
  control.label.back() = '\0';
}

const char* ActionIconId(TouchAction action) {
  constexpr std::array names = {
      "none", "move", "camera", "fire", "aim", "free_aim",
      "run_sprint", "jump_climb", "context", "reload", "cover", "crouch",
      "weapon_next", "weapon_previous", "weapon_wheel", "phone", "pause", "more",
      "settings", "accelerate", "brake", "handbrake", "vehicle_fire", "vehicle_alt_fire",
      "horn", "headlights", "radio_previous", "radio_next", "camera_cycle", "look_behind",
      "heli_yaw_left", "heli_yaw_right", "heli_ascend", "heli_descend", "heli_action", "phone_up",
      "phone_down", "phone_left", "phone_right", "phone_accept", "phone_back", "deploy",
      "parachute_brake_left", "parachute_brake_right", "detach", "smoke", "native_a", "native_b",
      "native_x", "native_y", "native_up", "native_down", "native_left", "native_right",
      "native_start", "native_back", "native_left_shoulder", "native_right_shoulder", "native_left_thumb", "native_right_thumb",
      "native_left_trigger", "native_right_trigger", "native_left_stick", "native_right_stick", "script", "zoom_in",
      "zoom_out", "edit_done", "edit_reset", "edit_smaller", "edit_larger", "edit_opacity",
      "edit_handedness", "edit_floating", "edit_camera_speed", "edit_aim_speed", "weapon_select", "activity_right_stick",
      "edit_vehicle_speed", "edit_flight_speed", "edit_invert_y",
  };
  static_assert(names.size() == static_cast<size_t>(TouchAction::kCount));
  const auto index = static_cast<size_t>(action);
  return index < names.size() ? names[index] : "none";
}

const char* ScriptLabel(TouchScriptControl control) {
  // Raw indices: sub_825D1308 and accepted PAD tokens in sub_821F2360.
  constexpr std::array raw = {"LT", "RT", "LB", "RB", "UP", "DOWN", "LEFT", "RIGHT",
                              "START", "BACK", "X", "Y", "A", "B", "L3", "R3"};
  if (control.kind == TouchScriptQueryKind::kRawButton) {
    return control.action >= 4 && control.action - 4 < raw.size() ? raw[control.action - 4] : "ACTION";
  }
  // Original input binding names, 0x82A935B0 (sub_821F2CB0); shortened for circles.
  constexpr std::array actions = {
      "CAMERA", "SPRINT", "JUMP", "ENTER", "ATTACK", "ATTACK", "AIM", "LOOK BACK",
      "NEXT WEAPON", "PREV WEAPON", "TARGET LEFT", "TARGET RIGHT", "MOVE LEFT", "MOVE RIGHT",
      "MOVE UP", "MOVE DOWN", "LOOK LEFT", "LOOK RIGHT", "LOOK UP", "LOOK DOWN", "DUCK", "PHONE",
      "PUT AWAY", "PICK UP", "ZOOM IN", "ZOOM OUT", "ZOOM IN", "ZOOM OUT", "COVER", "RELOAD",
      "STEER LEFT", "STEER RIGHT", "VEHICLE UP", "VEHICLE DOWN", "GUN LEFT", "GUN RIGHT", "GUN UP",
      "GUN DOWN", "ATTACK", "ATTACK", "ACCELERATE", "BRAKE", "LIGHTS", "EXIT", "HANDBRAKE",
      "HANDBRAKE", "HOTWIRE LEFT", "HOTWIRE RIGHT", "LOOK LEFT", "LOOK RIGHT", "LOOK BACK", "CAMERA",
      "NEXT RADIO", "PREV RADIO", "HORN", "THROTTLE UP", "THROTTLE DOWN", "YAW LEFT", "YAW RIGHT",
      "ATTACK", "ATTACK", "ATTACK", "KICK", "BLOCK", "DOWN", "UP", "LEFT", "RIGHT", "RIGHT DOWN",
      "RIGHT UP", "RIGHT LEFT", "RIGHT RIGHT", "HORIZONTAL", "VERTICAL", "RIGHT HORIZONTAL",
      "RIGHT VERTICAL", "PAUSE", "ACCEPT", "CANCEL", "X", "Y", "LB", "RB", "LT", "RT", "ATTACK"};
  static_assert(actions.size() == 86);
  return control.action < actions.size() ? actions[control.action] : "ACTION";
}

bool IsPauseScriptControl(TouchScriptControl control) {
  control = CanonicalTouchScriptControl(control);
  // The minimap owns Pause, including when native help names the Start button
  // or FRONTEND_PAUSE action. These IDs use the namespaces above.
  return (control.kind == TouchScriptQueryKind::kRawButton && control.action == 12) ||
         (control.kind == TouchScriptQueryKind::kControlHeld && control.action == 76);
}

bool ValidHudBounds(const ContextTouchHudBounds& bounds) noexcept {
  return std::isfinite(bounds.left) && std::isfinite(bounds.top) &&
         std::isfinite(bounds.right) && std::isfinite(bounds.bottom) &&
         bounds.right > bounds.left && bounds.bottom > bounds.top;
}

bool HudOverlapsCircle(const ContextTouchHudBounds& bounds, float x, float y,
                       float radius, const ContextTouchViewport& viewport) noexcept {
  double scale_x = 1.0, scale_y = 1.0;
  if (!viewport.host_space && viewport.output_width > 0.0f && viewport.output_height > 0.0f &&
      viewport.physical_output_width > 0.0f && viewport.physical_output_height > 0.0f) {
    scale_x = double(viewport.physical_output_width) / viewport.output_width;
    scale_y = double(viewport.physical_output_height) / viewport.output_height;
  }
  const double dx = (double(x) - std::clamp(double(x), double(bounds.left), double(bounds.right))) * scale_x;
  const double dy = (double(y) - std::clamp(double(y), double(bounds.top), double(bounds.bottom))) * scale_y;
  const double rendered_radius = radius * std::min(scale_x, scale_y);
  return dx * dx + dy * dy <= rendered_radius * rendered_radius;
}

void AddControl(ContextTouchLayout& layout, ContextTouchControl control) {
  if (layout.control_count < layout.controls.size()) {
    layout.controls[layout.control_count++] = control;
  }
}

void AddCircle(ContextTouchLayout& layout, ContextTouchControlKind kind, rex::ui::VirtualKey key,
               const char* label, float x, float y, float radius, TouchScriptControl script = {}) {
  ContextTouchControl control{
      .kind = kind,
      .key = key,
      .script = script,
      .center_x = x,
      .center_y = y,
      .radius = radius,
      .minimum_x = x - radius,
      .minimum_y = y - radius,
      .maximum_x = x + radius,
      .maximum_y = y + radius,
  };
  SetLabel(control, label);
  AddControl(layout, control);
}

void AddLookSurface(ContextTouchLayout& layout, const ContextTouchViewport& viewport) {
  ContextTouchControl control{
      .kind = ContextTouchControlKind::kLookSurface,
      .minimum_x = viewport.safe_x + viewport.safe_width * 0.30f,
      .minimum_y = viewport.safe_y,
      .maximum_x = viewport.safe_x + viewport.safe_width,
      .maximum_y = viewport.safe_y + viewport.safe_height,
  };
  AddControl(layout, control);
}

struct ButtonDescription {
  rex::ui::VirtualKey key;
  const char* label;
};

void AddButtonGrid(ContextTouchLayout& layout, std::span<const ButtonDescription> buttons,
                   float right, float bottom, float padding, float radius, float gap) {
  const float step = radius * 2.0f + gap;
  for (size_t index = 0; index < buttons.size(); ++index) {
    const float column = static_cast<float>(index % 2);
    const float row = static_cast<float>(index / 2);
    AddCircle(layout, ContextTouchControlKind::kButton, buttons[index].key, buttons[index].label,
              right - padding - radius - column * step, bottom - padding - radius - row * step,
              radius);
  }
}

}  // namespace

ContextTouchLayout BuildContextTouchLayout(
    ContextTouchMode mode, const ContextTouchViewport& viewport,
    std::span<const TouchScriptControl> script_controls,
    const ContextTouchLayoutOptions& options) noexcept {
  // Native frontend/map gestures own these screens. Gameplay controls and
  // editor tools must never cover the game's pause menu.
  if (mode == ContextTouchMode::kFrontend || mode == ContextTouchMode::kMap) {
    return {.mode = mode, .viewport = viewport};
  }
  if (options.contextual) return BuildSemanticLayout(mode, viewport, script_controls, options);
  ContextTouchLayout layout{.mode = mode, .viewport = viewport};
  if (mode == ContextTouchMode::kDisabled || !viewport.valid || !viewport.focused ||
      viewport.safe_width <= 0.0f || viewport.safe_height <= 0.0f) {
    return layout;
  }

  const float short_edge = std::min(viewport.safe_width, viewport.safe_height);
  const float padding = short_edge * kEdgePaddingRatio;
  const float stick_radius = short_edge * kStickRadiusRatio;
  const float button_radius = short_edge * kButtonRadiusRatio;
  const float button_gap = short_edge * kButtonGapRatio;
  const float right = viewport.safe_x + viewport.safe_width;
  const float bottom = viewport.safe_y + viewport.safe_height;

  const bool minigame_uses_sticks =
      mode == ContextTouchMode::kMinigame &&
      std::any_of(script_controls.begin(), script_controls.end(),
                  [](const TouchScriptControl& control) {
                    return control.kind == TouchScriptQueryKind::kAnalogueSticks;
                  });
  if ((mode != ContextTouchMode::kMinigame && mode != ContextTouchMode::kVehiclePassenger &&
       mode != ContextTouchMode::kVehicleDriverUnknown) ||
      minigame_uses_sticks) {
    AddCircle(layout, ContextTouchControlKind::kMovementStick, rex::ui::VirtualKey::kNone, "MOVE",
              viewport.safe_x + padding + stick_radius, bottom - padding - stick_radius,
              stick_radius);
  }

  if (mode == ContextTouchMode::kOnFoot) {
    const std::array buttons = {
        ButtonDescription{rex::ui::VirtualKey::kLButton, options.armed ? "FIRE" : "ATTACK"},
        ButtonDescription{rex::ui::VirtualKey::kSpace, "JUMP"},
        ButtonDescription{rex::ui::VirtualKey::kRButton, "AIM"},
        ButtonDescription{rex::ui::VirtualKey::kShift, "RUN"},
        ButtonDescription{rex::ui::VirtualKey::kF, "ENTER"},
        ButtonDescription{rex::ui::VirtualKey::kR, "RELOAD"},
        ButtonDescription{rex::ui::VirtualKey::kQ, "COVER"},
        ButtonDescription{rex::ui::VirtualKey::kUp, "PHONE"},
    };
    std::array<ButtonDescription, buttons.size()> available{};
    size_t available_count = 0;
    for (const auto& button : buttons) {
      if (button.key == rex::ui::VirtualKey::kF && !options.can_enter_vehicle && !options.editing) continue;
      available[available_count++] = button;
    }
    AddButtonGrid(layout, std::span(available).first(available_count), right, bottom, padding,
                  button_radius, button_gap);
    AddLookSurface(layout, viewport);
  } else if (mode == ContextTouchMode::kVehicleAutomobile ||
             mode == ContextTouchMode::kVehicleBike || mode == ContextTouchMode::kVehicleBoat) {
    constexpr std::array buttons = {
        ButtonDescription{rex::ui::VirtualKey::kW, "GAS"},
        ButtonDescription{rex::ui::VirtualKey::kS, "BRAKE"},
        ButtonDescription{rex::ui::VirtualKey::kSpace, "HANDBRK"},
        ButtonDescription{rex::ui::VirtualKey::kF, "EXIT"},
        ButtonDescription{rex::ui::VirtualKey::kLButton, "FIRE"},
        ButtonDescription{rex::ui::VirtualKey::kRButton, "ALT"},
        ButtonDescription{rex::ui::VirtualKey::kG, "HORN"},
        ButtonDescription{rex::ui::VirtualKey::kH, "LIGHT"},
    };
    AddButtonGrid(layout, buttons, right, bottom, padding, button_radius, button_gap);
    AddLookSurface(layout, viewport);
  } else if (mode == ContextTouchMode::kVehicleHelicopter) {
    constexpr std::array buttons = {
        ButtonDescription{rex::ui::VirtualKey::kNumpad4, "YAW L"},
        ButtonDescription{rex::ui::VirtualKey::kNumpad6, "YAW R"},
        ButtonDescription{rex::ui::VirtualKey::kW, "THROTTLE"},
        ButtonDescription{rex::ui::VirtualKey::kS, "DESCEND"},
        ButtonDescription{rex::ui::VirtualKey::kLButton, "FIRE"},
        ButtonDescription{rex::ui::VirtualKey::kRButton, "ALT"},
        ButtonDescription{rex::ui::VirtualKey::kShift, "ACTION"},
        ButtonDescription{rex::ui::VirtualKey::kF, "EXIT"},
    };
    AddButtonGrid(layout, buttons, right, bottom, padding, button_radius, button_gap);
    AddLookSurface(layout, viewport);
  } else if (mode == ContextTouchMode::kVehiclePassenger) {
    constexpr std::array buttons = {
        ButtonDescription{rex::ui::VirtualKey::kLButton, "FIRE"},
        ButtonDescription{rex::ui::VirtualKey::kRButton, "AIM"},
        ButtonDescription{rex::ui::VirtualKey::kF, "EXIT"},
        ButtonDescription{rex::ui::VirtualKey::kV, "CAMERA"},
    };
    AddButtonGrid(layout, buttons, right, bottom, padding, button_radius, button_gap);
    AddLookSurface(layout, viewport);
  } else if (mode == ContextTouchMode::kVehicleDriverUnknown) {
    // Only expose bindings already proven global in the active keyboard bridge.
    // Specialized steering/throttle remain absent until the class is known.
    constexpr std::array buttons = {
        ButtonDescription{rex::ui::VirtualKey::kF, "EXIT"},
        ButtonDescription{rex::ui::VirtualKey::kCapital, "CAMERA"},
    };
    AddButtonGrid(layout, buttons, right, bottom, padding, button_radius, button_gap);
    AddLookSurface(layout, viewport);
  } else if (mode == ContextTouchMode::kPhone) {
    constexpr std::array buttons = {
        ButtonDescription{rex::ui::VirtualKey::kUp, "UP"},
        ButtonDescription{rex::ui::VirtualKey::kDown, "DOWN"},
        ButtonDescription{rex::ui::VirtualKey::kLeft, "LEFT"},
        ButtonDescription{rex::ui::VirtualKey::kRight, "RIGHT"},
        ButtonDescription{rex::ui::VirtualKey::kReturn, "SELECT"},
        ButtonDescription{rex::ui::VirtualKey::kBack, "BACK"},
    };
    AddButtonGrid(layout, buttons, right, bottom, padding, button_radius, button_gap);
  } else if (mode == ContextTouchMode::kParachuteFreefall) {
    constexpr std::array buttons = {
        ButtonDescription{rex::ui::VirtualKey::kLButton, "DEPLOY"},
    };
    AddButtonGrid(layout, buttons, right, bottom, padding, button_radius, button_gap);
  } else if (mode == ContextTouchMode::kParachuteDeployed) {
    constexpr std::array buttons = {
        ButtonDescription{rex::ui::VirtualKey::kLButton, "L BRAKE"},
        ButtonDescription{rex::ui::VirtualKey::kRButton, "R BRAKE"},
        ButtonDescription{rex::ui::VirtualKey::kF, "DETACH"},
        ButtonDescription{rex::ui::VirtualKey::kControl, "SMOKE"},
    };
    AddButtonGrid(layout, buttons, right, bottom, padding, button_radius, button_gap);
  }

  if (mode == ContextTouchMode::kMinigame) {
    const float step = button_radius * 2.0f + button_gap;
    size_t visible_index = 0;
    for (size_t index = 0; index < script_controls.size(); ++index) {
      if (script_controls[index].kind == TouchScriptQueryKind::kAnalogueSticks) {
        continue;
      }
      if (IsPauseScriptControl(script_controls[index])) continue;
      const auto canonical = CanonicalTouchScriptControl(script_controls[index]);
      bool duplicate = false;
      for (size_t prior = 0; prior < index; ++prior) {
        const auto earlier = CanonicalTouchScriptControl(script_controls[prior]);
        duplicate |= earlier.kind == canonical.kind && earlier.action == canonical.action &&
                     earlier.input_group == canonical.input_group &&
                     earlier.script_thread == canonical.script_thread &&
                     earlier.generation == canonical.generation;
      }
      if (duplicate) {
        continue;
      }
      const float column = static_cast<float>(visible_index % 2);
      const float row = static_cast<float>(visible_index / 2);
      if (bottom - padding - button_radius * 2.0f - row * step < viewport.safe_y) break;
      AddCircle(layout, ContextTouchControlKind::kScriptButton, rex::ui::VirtualKey::kNone, ScriptLabel(canonical),
                right - padding - button_radius - column * step,
                bottom - padding - button_radius - row * step, button_radius,
                canonical);
      ++visible_index;
    }
  }
  ApplyContextTouchHudReservation(layout, options.weapon_hud_bounds);
  return layout;
}

namespace {

ContextTouchLayout BuildSemanticLayout(ContextTouchMode mode, const ContextTouchViewport& viewport,
                                      std::span<const TouchScriptControl> scripts,
                                      const ContextTouchLayoutOptions& options) noexcept {
  using A = TouchAction;
  using K = ContextTouchControlKind;
  using V = rex::ui::VirtualKey;
  using namespace rex::input;
  ContextTouchLayout layout{.mode = mode, .viewport = viewport};
  layout.opacity = std::isfinite(options.opacity) ? std::clamp(options.opacity, 0.2f, 1.0f) : 0.65f;
  if (mode == ContextTouchMode::kDisabled || !viewport.valid || !viewport.focused ||
      !std::isfinite(viewport.safe_width) || !std::isfinite(viewport.safe_height) ||
      viewport.safe_width <= 0.0f || viewport.safe_height <= 0.0f) return layout;
  const float edge = std::min(viewport.safe_width, viewport.safe_height);
  const float scale = std::isfinite(options.button_scale) ? std::clamp(options.button_scale, 0.65f, 1.8f) : 1.0f;
  const float padding = edge * kEdgePaddingRatio;
  const float radius = std::clamp(edge * kButtonRadiusRatio * scale,
                                  viewport.host_space ? std::min(24.0f, edge * 0.1f) : edge * 0.04f,
                                  edge * 0.105f);
  const float gap = edge * kButtonGapRatio;
  const float step = radius * 2.0f + gap;
  const float right = viewport.safe_x + viewport.safe_width;
  const float bottom = viewport.safe_y + viewport.safe_height;
  const float cx = viewport.safe_x + viewport.safe_width * 0.5f;
  const float cy = viewport.safe_y + viewport.safe_height * 0.5f;
  const float utility_radius = std::min(radius, std::max(24.0f, edge * 0.045f));
  const bool vehicle = mode >= ContextTouchMode::kVehicleAutomobile && mode <= ContextTouchMode::kVehiclePassenger;
  const auto analogue = std::find_if(scripts.begin(), scripts.end(), [](const auto& script) {
    return script.kind == TouchScriptQueryKind::kAnalogueSticks;
  });
  const bool activity_sticks = analogue != scripts.end();
  ContextTouchControl overflow_control;
  const auto overlaps_weapon_hud = [&](float x, float y, float r) {
    if (!options.weapon_hud_bounds || !ValidHudBounds(*options.weapon_hud_bounds)) return false;
    const float final_x = options.left_handed ? viewport.safe_x + right - x : x;
    return HudOverlapsCircle(*options.weapon_hud_bounds, final_x, y, r, viewport);
  };

  const auto add = [&](A action, const char* label, uint16_t buttons, uint8_t trigger,
                       float x, float y, float r, bool visible = true, K kind = K::kNativeButton,
                       V key = V::kNone) -> ContextTouchControl& {
    ContextTouchControl c;
    c.kind = trigger ? K::kNativeTrigger : kind;
    c.action = action;
    c.key = key;
    c.pad_buttons = buttons;
    c.trigger_side = trigger;
    c.visible = visible;
    c.center_x = x;
    c.center_y = y;
    c.radius = r;
    c.minimum_x = x - r; c.maximum_x = x + r;
    c.minimum_y = y - r; c.maximum_y = y + r;
    SetLabel(c, label);
    std::snprintf(c.accessible_name.data(), c.accessible_name.size(), "%s", label);
    // Stable filenames can be supplied later without changing input ownership.
    std::snprintf(c.icon_id.data(), c.icon_id.size(), "%s", ActionIconId(action));
    if (layout.control_count == layout.controls.size()) {
      overflow_control = c;
      overflow_control.visible = false;
      return overflow_control;
    }
    AddControl(layout, c);
    return layout.controls[layout.control_count - 1];
  };
  const auto primary = [&](A action, const char* label, uint16_t buttons, uint8_t trigger,
                           size_t slot, V key = V::kNone) {
    add(action, label, buttons, trigger, right - padding - radius - float(slot % 2) * step,
        bottom - padding - radius - float(slot / 2) * step -
            (activity_sticks ? edge * kStickRadiusRatio * 2.0f + gap : 0.0f),
        radius, true,
        K::kNativeButton, key);
  };
  const auto hidden = [&](A action, const char* label, uint16_t buttons, uint8_t trigger, V key = V::kNone) {
    for (size_t i = 0; i < layout.control_count; ++i) if (layout.controls[i].action == action) return;
    add(action, label, buttons, trigger, right - padding - radius, bottom - padding - radius,
        radius, false, K::kNativeButton, key);
  };

  {
    const float stick_radius = edge * kStickRadiusRatio;
    auto& move = add(A::kMove, vehicle ? "STEER" : "MOVE", 0, 0,
                     viewport.safe_x + padding + stick_radius, bottom - padding - stick_radius,
                     stick_radius, true, K::kMovementStick);
    if (activity_sticks) move.script = *analogue;
    if (mode == ContextTouchMode::kMinigame && !activity_sticks) move.visible = false;
    if (activity_sticks) {
      auto& right_stick = add(A::kActivityRightStick, "LOOK", 0, 0,
                              right - padding - stick_radius, bottom - padding - stick_radius,
                              stick_radius, move.visible, K::kRightStick);
      right_stick.script = move.script;
    }
    if (mode == ContextTouchMode::kOnFoot || mode == ContextTouchMode::kPhone) {
      if (options.melee) {
        primary(A::kFire, "PUNCH", X_INPUT_GAMEPAD_B, 0, 0);
        primary(A::kContext, "ALT", X_INPUT_GAMEPAD_Y, 0, 1);
        primary(A::kJumpClimb, "KICK", X_INPUT_GAMEPAD_X, 0, 2);
        primary(A::kRunSprint, "BLOCK", X_INPUT_GAMEPAD_A, 0, 3);
      } else if (options.armed || options.aiming || options.in_cover) {
        primary(A::kFire, options.armed ? "FIRE" : "ATTACK", 0, 2, 0, V::kLButton);
        primary(A::kAim, "AIM", 0, 1, 1, V::kRButton);
        primary(A::kCover, options.in_cover ? "EXIT COVER" : "COVER", X_INPUT_GAMEPAD_RIGHT_SHOULDER, 0, 2, V::kQ);
        primary(A::kReload, "RELOAD", X_INPUT_GAMEPAD_B, 0, 3, V::kR);
      } else {
        // Retail attack remains RT before a melee task is active. The melee
        // task uses B/Y/X/A for punch/alternate/kick/block above.
        primary(A::kFire, "ATTACK", 0, 2, 0, V::kLButton);
        primary(A::kRunSprint, "RUN", X_INPUT_GAMEPAD_A, 0, 1, V::kShift);
        primary(A::kJumpClimb, "JUMP", X_INPUT_GAMEPAD_X, 0, 2, V::kSpace);
        primary(A::kAim, "AIM", 0, 1, 3, V::kRButton);
      }
      hidden(A::kFire, "FIRE", 0, 2, V::kLButton);
      hidden(A::kAim, "AIM", 0, 1, V::kRButton);
      hidden(A::kRunSprint, "RUN", X_INPUT_GAMEPAD_A, 0, V::kShift);
      hidden(A::kJumpClimb, "JUMP", X_INPUT_GAMEPAD_X, 0, V::kSpace);
      hidden(A::kContext, "ENTER", X_INPUT_GAMEPAD_Y, 0, V::kF);
      hidden(A::kReload, "RELOAD", X_INPUT_GAMEPAD_B, 0, V::kR);
      hidden(A::kCover, "COVER", X_INPUT_GAMEPAD_RIGHT_SHOULDER, 0, V::kQ);
    } else if (mode == ContextTouchMode::kVehicleHelicopter) {
      primary(A::kHeliAscend, "ASCEND", 0, 2, 0, V::kW);
      primary(A::kHeliDescend, "DESCEND", 0, 1, 1, V::kS);
      primary(A::kHeliYawLeft, "YAW L", X_INPUT_GAMEPAD_LEFT_SHOULDER, 0, 2, V::kNumpad4);
      primary(A::kHeliYawRight, "YAW R", X_INPUT_GAMEPAD_RIGHT_SHOULDER, 0, 3, V::kNumpad6);
    } else if (mode == ContextTouchMode::kVehiclePassenger) {
      primary(A::kVehicleFire, "FIRE", X_INPUT_GAMEPAD_LEFT_SHOULDER, 0, 0, V::kLButton);
      primary(A::kAim, "AIM", 0, 1, 1, V::kRButton);
      primary(A::kContext, "EXIT", X_INPUT_GAMEPAD_Y, 0, 2, V::kF);
      primary(A::kCameraCycle, "CAMERA", X_INPUT_GAMEPAD_BACK, 0, 3, V::kV);
    } else if (vehicle) {
      primary(A::kAccelerate, "THROTTLE", 0, 2, 0, V::kW);
      primary(A::kVehicleFire, "FIRE", X_INPUT_GAMEPAD_LEFT_SHOULDER, 0, 1, V::kLButton);
      primary(A::kBrake, "BRAKE", 0, 1, 2, V::kS);
      primary(A::kHandbrake, "HANDBRAKE", X_INPUT_GAMEPAD_RIGHT_SHOULDER, 0, 3, V::kSpace);
    } else if (mode == ContextTouchMode::kParachuteFreefall) {
      primary(A::kDeploy, "DEPLOY", 0, 0, 0, V::kLButton);
    } else if (mode == ContextTouchMode::kParachuteDeployed) {
      primary(A::kParachuteBrakeLeft, "L BRAKE", 0, 0, 0, V::kLButton);
      primary(A::kParachuteBrakeRight, "R BRAKE", 0, 0, 1, V::kRButton);
      primary(A::kDetach, "DETACH", 0, 0, 2, V::kF);
      primary(A::kSmoke, "SMOKE", 0, 0, 3, V::kControl);
    }
    if (mode == ContextTouchMode::kParachuteFreefall || mode == ContextTouchMode::kParachuteDeployed) {
      for (size_t i = 0; i < layout.control_count; ++i) {
        if (layout.controls[i].key != V::kNone) layout.controls[i].kind = K::kButton;
      }
    }
    if (mode != ContextTouchMode::kMinigame) {
      AddLookSurface(layout, viewport);
      auto& camera = layout.controls[layout.control_count - 1];
      camera.action = A::kCamera;
      camera.minimum_x = viewport.safe_x;
    }
  }



  // Accepted native help tokens use available space in the active layout.
  size_t script_index = 0;
  const float script_radius = utility_radius;
  const float script_step = script_radius * 2.0f + gap;
  const size_t script_columns = static_cast<size_t>(std::clamp(
      std::floor((viewport.safe_width - padding * 2.0f) / script_step), 1.0f, 12.0f));
  for (const auto& script : scripts) {
    if (options.editing || script_index >= 4) break;
    if (script.kind == TouchScriptQueryKind::kAnalogueSticks) continue;
    if (IsPauseScriptControl(script)) continue;
    const auto canonical = CanonicalTouchScriptControl(script);
    bool duplicate = false;
    for (size_t i = 0; i < layout.control_count; ++i) {
      const auto& c = layout.controls[i];
      duplicate |= c.kind == K::kScriptButton && c.script.kind == canonical.kind &&
                   c.script.action == canonical.action && c.script.input_group == canonical.input_group &&
                   c.script.script_thread == canonical.script_thread && c.script.generation == canonical.generation;
    }
    if (duplicate) continue;
    bool placed = false;
    for (size_t row = 0; row < 16 && !placed; ++row) {
      const float y = viewport.safe_y + padding + utility_radius * 2.0f + gap + script_radius + float(row) * script_step;
      if (y + script_radius > bottom - edge * 0.30f) break;
      for (size_t column = 0; column < script_columns; ++column) {
        const float x = cx + (float(column) - float(script_columns - 1) * 0.5f) * script_step;
        bool occupied = overlaps_weapon_hud(x, y, script_radius);
        for (size_t i = 0; i < layout.control_count; ++i) {
          const auto& other = layout.controls[i];
          if (!other.visible || other.kind == K::kLookSurface) continue;
          const float dx = x - other.center_x, dy = y - other.center_y;
          const float separation = script_radius + other.radius + gap * 0.25f;
          if (dx * dx + dy * dy < separation * separation) { occupied = true; break; }
        }
        if (occupied) continue;
        auto& c = add(A::kScript, ScriptLabel(canonical), 0, 0, x, y, script_radius, true, K::kScriptButton);
        c.script = canonical;
        placed = true;
        ++script_index;
        break;
      }
    }
  }

  const auto compact = [&](A action, const char* label, uint16_t buttons, uint8_t trigger,
                           bool visible = true, K kind = K::kNativeButton, V key = V::kNone,
                           float requested_radius = 0.0f, bool editor_control = false) {
    const float r = requested_radius > 0.0f ? requested_radius : utility_radius;
    ContextTouchControl* existing = nullptr;
    for (size_t i = 0; i < layout.control_count; ++i) {
      if (layout.controls[i].action != action) continue;
      if (layout.controls[i].visible) return;
      existing = &layout.controls[i];
      break;
    }
    float x = right - padding - r;
    float y = viewport.safe_y + padding + r;
    bool placed = false;
    if (visible) {
      // Search between the regular slots too. Large primary controls and the
      // editor toolbar leave usable gaps that a diameter-spaced grid misses.
      const float distance = (r * 2.0f + gap) * 0.125f;
      const size_t rows = static_cast<size_t>(std::clamp(
          std::ceil(viewport.safe_height / distance), 1.0f, 128.0f));
      const size_t columns = static_cast<size_t>(std::clamp(
          std::ceil(viewport.safe_width / distance), 1.0f, 128.0f));
      for (size_t row = 0; row < rows && !placed; ++row) {
        y = viewport.safe_y + padding + r + float(row) * distance;
        if (y + r > bottom - padding) break;
        for (size_t column = 0; column < columns; ++column) {
          x = editor_control ? viewport.safe_x + padding + r + float(column) * distance
                             : right - padding - r - float(column) * distance;
          if (editor_control ? x + r > right - padding : x - r < viewport.safe_x + padding) break;
          // Keep the upper-left radar area clear in either handedness. The
          // minimap itself owns Pause, so no separate Pause control is needed.
          const float radar_right = viewport.safe_x + viewport.safe_width * 0.34f;
          const float radar_bottom = viewport.safe_y + viewport.safe_height * 0.40f;
          const float final_x = options.left_handed ? viewport.safe_x + right - x : x;
          if (!editor_control && final_x - r < radar_right &&
              y - r < radar_bottom) continue;
          if (overlaps_weapon_hud(x, y, r)) continue;
          bool occupied = false;
          for (size_t i = 0; i < layout.control_count; ++i) {
            const auto& other = layout.controls[i];
            if (!other.visible || other.kind == K::kLookSurface) continue;
            const float dx = x - other.center_x, dy = y - other.center_y;
            const float separation = r + other.radius + gap * 0.25f;
            if (dx * dx + dy * dy < separation * separation) { occupied = true; break; }
          }
          if (!occupied) { placed = true; break; }
        }
      }
    }
    auto& control = add(action, label, buttons, trigger, x, y, r,
                        visible && placed, kind, key);
    if (existing) {
      *existing = control;
      --layout.control_count;
    }
  };

  if (options.editing) {
    constexpr std::array<std::pair<A, const char*>, 12> edit = {{
      {A::kEditDone,"DONE"}, {A::kEditReset,"RESET"}, {A::kEditSmaller,"SIZE -"},
      {A::kEditLarger,"SIZE +"}, {A::kEditOpacity,"OPACITY"}, {A::kEditHandedness,"MIRROR"},
      {A::kEditFloating,"STICK"}, {A::kEditCameraSpeed,"CAM SPEED"}, {A::kEditAimSpeed,"AIM SPEED"},
      {A::kEditVehicleSpeed,"CAR CAM"}, {A::kEditFlightSpeed,"FLIGHT CAM"}, {A::kEditInvertY,"INVERT Y"}}};
    const float r = std::min(24.0f, edge * 0.075f);
    for (const auto& [action, label] : edit) {
      compact(action, label, 0, 0, true, K::kUtility, V::kNone, r, true);
    }
  }


  if (options.phone_visible || mode == ContextTouchMode::kPhone) {
    const float phone_radius = utility_radius;
    const float phone_step = phone_radius * 2.0f + gap;
    const float phone_y = viewport.safe_y + padding + utility_radius * 2.0f + gap + phone_radius;
    const float phone_x = std::max(viewport.safe_x + padding + phone_step + phone_radius,
        std::min(cx, right - padding - radius * 2.0f - step - gap - phone_step - phone_radius));
    struct PhoneButton {
      A action;
      const char* label;
      uint16_t buttons;
      float x;
      float y;
    };
    const std::array phone_buttons = {
      PhoneButton{A::kPhoneUp, "UP", X_INPUT_GAMEPAD_DPAD_UP, phone_x, phone_y},
      PhoneButton{A::kPhoneLeft, "LEFT", X_INPUT_GAMEPAD_DPAD_LEFT, phone_x - phone_step, phone_y + phone_step},
      PhoneButton{A::kPhoneDown, "DOWN", X_INPUT_GAMEPAD_DPAD_DOWN, phone_x, phone_y + phone_step},
      PhoneButton{A::kPhoneRight, "RIGHT", X_INPUT_GAMEPAD_DPAD_RIGHT, phone_x + phone_step, phone_y + phone_step},
      PhoneButton{A::kPhoneAccept, "SELECT", X_INPUT_GAMEPAD_A, phone_x - phone_step * 0.5f, phone_y + phone_step * 2.0f},
      PhoneButton{A::kPhoneBack, "BACK", X_INPUT_GAMEPAD_B, phone_x + phone_step * 0.5f, phone_y + phone_step * 2.0f},
    };
    bool cluster_fits = true;
    for (const auto& button : phone_buttons) {
      cluster_fits &= button.x - phone_radius >= viewport.safe_x + padding &&
                      button.y - phone_radius >= viewport.safe_y + padding &&
                      button.x + phone_radius <= right - padding &&
                      button.y + phone_radius <= bottom - padding &&
                      !overlaps_weapon_hud(button.x, button.y, phone_radius);
      for (size_t i = 0; i < layout.control_count; ++i) {
        const auto& other = layout.controls[i];
        if (!other.visible || other.kind == K::kLookSurface) continue;
        const float dx = button.x - other.center_x, dy = button.y - other.center_y;
        const float separation = phone_radius + other.radius + gap * 0.25f;
        if (dx * dx + dy * dy < separation * separation) cluster_fits = false;
      }
    }
    // Retain the directional cluster when it fits. Activity sticks and the
    // editor can occupy that region, so use free compact slots in that case.
    for (const auto& button : phone_buttons) {
      if (cluster_fits) {
        add(button.action, button.label, button.buttons, 0, button.x, button.y, phone_radius);
      } else {
        compact(button.action, button.label, button.buttons, 0);
      }
    }
  }

  const bool on_foot = mode == ContextTouchMode::kOnFoot || mode == ContextTouchMode::kPhone;
  const bool phone = options.phone_visible || mode == ContextTouchMode::kPhone;
  if (on_foot) {
    compact(A::kContext, "ENTER", X_INPUT_GAMEPAD_Y, 0,
            !options.melee && (options.can_enter_vehicle || options.editing) && !phone,
            K::kNativeButton, V::kF);
    compact(A::kRunSprint, "RUN", X_INPUT_GAMEPAD_A, 0, !phone, K::kNativeButton, V::kShift);
    compact(A::kJumpClimb, "JUMP", X_INPUT_GAMEPAD_X, 0, !phone, K::kNativeButton, V::kSpace);
    compact(A::kCover, options.in_cover ? "EXIT COVER" : "COVER", X_INPUT_GAMEPAD_RIGHT_SHOULDER,
            0, !options.melee && !phone, K::kNativeButton, V::kQ);
    compact(A::kReload, "RELOAD", X_INPUT_GAMEPAD_B, 0,
            options.armed && !options.melee && !phone, K::kNativeButton, V::kR);
    compact(A::kAim, "AIM", 0, 1, !phone, K::kNativeTrigger, V::kRButton);
    compact(A::kCrouch, "CROUCH", X_INPUT_GAMEPAD_LEFT_THUMB, 0, !options.melee && !phone);
    compact(A::kWeaponWheel, "WEAPON", 0, 0, !phone, K::kUtility);
    compact(A::kFreeAim, "FREE AIM", 0, 1,
            options.armed && options.free_aim_available && !options.melee && !phone, K::kNativeTrigger);
    compact(A::kZoomIn, "ZOOM +", 0, 0, options.scoped_zoom && !phone, K::kUtility);
    compact(A::kZoomOut, "ZOOM -", 0, 0, options.scoped_zoom && !phone, K::kUtility);
  }
  if (vehicle) {
    compact(A::kContext, "EXIT", X_INPUT_GAMEPAD_Y, 0, true, K::kNativeButton, V::kF);
    if (mode == ContextTouchMode::kVehicleHelicopter) {
      // KeyboardControllerBindings publishes these same native A/X buttons;
      // the native helicopter consumer owns weapon and vehicle-specific use.
      compact(A::kVehicleFire, "FIRE", X_INPUT_GAMEPAD_A, 0, !phone);
      compact(A::kHeliAction, "ACTION", X_INPUT_GAMEPAD_X, 0, !phone);
    } else {
      compact(A::kHorn, "HORN", X_INPUT_GAMEPAD_LEFT_THUMB, 0,
              mode != ContextTouchMode::kVehiclePassenger && !phone);
      compact(A::kHeadlights, mode == ContextTouchMode::kVehiclePassenger ? "WEAPON" : "WEAPON / LIGHTS",
              X_INPUT_GAMEPAD_X, 0, !phone);
      compact(A::kRadioPrevious, "RADIO -", X_INPUT_GAMEPAD_DPAD_LEFT, 0, !phone);
      compact(A::kRadioNext, "RADIO +", X_INPUT_GAMEPAD_DPAD_RIGHT, 0, !phone);
    }
  }
  if (on_foot || vehicle) {
    compact(A::kCameraCycle, "CAMERA", X_INPUT_GAMEPAD_BACK, 0, !phone);
    compact(A::kLookBehind, "LOOK BACK", X_INPUT_GAMEPAD_RIGHT_THUMB, 0, !phone);
    compact(A::kPhone, "PHONE", X_INPUT_GAMEPAD_DPAD_UP, 0, !phone);
  }

  if (options.weapon_wheel_open) {
    for (size_t i = 0; i < layout.control_count; ++i) {
      if (layout.controls[i].action != A::kMove && layout.controls[i].action != A::kWeaponWheel)
        layout.controls[i].visible = false;
    }
    constexpr float tau = 6.2831853071795864769f;
    const float ring = edge * 0.34f;
    const float r = std::min(radius, edge * 0.065f);
    for (size_t i = 0; i < options.weapon_types.size(); ++i) {
      if (!options.weapon_selectable[i]) continue;
      const float angle = tau * float(i) / float(options.weapon_types.size()) - tau * 0.25f;
      char fallback[24];
      std::snprintf(fallback, sizeof(fallback), "WEAPON %u", options.weapon_types[i]);
      auto& c = add(A::kWeaponSelect, options.weapon_names[i][0] ? options.weapon_names[i].data() : fallback,
                    0, 0, cx + std::cos(angle) * ring, cy + std::sin(angle) * ring, r, true, K::kUtility);
      c.weapon_slot = static_cast<uint8_t>(i);
    }
  }
  if (options.left_handed) {
    for (size_t i = 0; i < layout.control_count; ++i) {
      auto& c = layout.controls[i];
      if (c.kind == K::kLookSurface) continue;
      c.center_x = viewport.safe_x + right - c.center_x;
      c.minimum_x = c.center_x - c.radius; c.maximum_x = c.center_x + c.radius;
    }
  }
  ApplyContextTouchHudReservation(layout, options.weapon_hud_bounds);
  return layout;
}

}  // namespace

std::optional<ContextTouchHudBounds> MapContextTouchHudBounds(
    const ContextTouchHudBounds& normalized_bounds,
    const ContextTouchViewport& viewport) noexcept {
  if (!viewport.valid || !viewport.focused || !ValidHudBounds(normalized_bounds)) return std::nullopt;
  ContextTouchHudBounds clipped{
      std::clamp(normalized_bounds.left, 0.0f, 1.0f),
      std::clamp(normalized_bounds.top, 0.0f, 1.0f),
      std::clamp(normalized_bounds.right, 0.0f, 1.0f),
      std::clamp(normalized_bounds.bottom, 0.0f, 1.0f),
  };
  if (!ValidHudBounds(clipped)) return std::nullopt;
  double offset_x = 0.0, offset_y = 0.0;
  double scale_x = viewport.output_width, scale_y = viewport.output_height;
  if (viewport.host_space) {
    if (!std::isfinite(viewport.physical_output_x) || !std::isfinite(viewport.physical_output_y) ||
        !std::isfinite(viewport.physical_output_width) || viewport.physical_output_width <= 0.0f ||
        !std::isfinite(viewport.physical_output_height) || viewport.physical_output_height <= 0.0f ||
        !std::isfinite(viewport.physical_surface_width) || viewport.physical_surface_width <= 0.0f ||
        !std::isfinite(viewport.physical_surface_height) || viewport.physical_surface_height <= 0.0f ||
        !std::isfinite(viewport.logical_width) || viewport.logical_width <= 0.0f ||
        !std::isfinite(viewport.logical_height) || viewport.logical_height <= 0.0f) return std::nullopt;
    const double logical_scale_x = double(viewport.logical_width) / viewport.physical_surface_width;
    const double logical_scale_y = double(viewport.logical_height) / viewport.physical_surface_height;
    offset_x = viewport.physical_output_x * logical_scale_x;
    offset_y = viewport.physical_output_y * logical_scale_y;
    scale_x = viewport.physical_output_width * logical_scale_x;
    scale_y = viewport.physical_output_height * logical_scale_y;
  }
  if (!std::isfinite(scale_x) || !std::isfinite(scale_y) || scale_x <= 0.0 || scale_y <= 0.0)
    return std::nullopt;
  ContextTouchHudBounds mapped{
      float(offset_x + clipped.left * scale_x), float(offset_y + clipped.top * scale_y),
      float(offset_x + clipped.right * scale_x), float(offset_y + clipped.bottom * scale_y),
  };
  return ValidHudBounds(mapped) ? std::optional(mapped) : std::nullopt;
}

bool ApplyContextTouchHudReservation(
    ContextTouchLayout& layout,
    const std::optional<ContextTouchHudBounds>& bounds) noexcept {
  if (!bounds || !ValidHudBounds(*bounds)) return true;
  const size_t count = std::min(layout.control_count, layout.controls.size());
  std::array<bool, ContextTouchLayout::kMaximumControls> affected{};
  bool any_affected = false;
  for (size_t i = 0; i < count; ++i) {
    auto& control = layout.controls[i];
    if (!control.visible || control.kind == ContextTouchControlKind::kLookSurface ||
        !std::isfinite(control.radius) || control.radius <= 0.0f ||
        !HudOverlapsCircle(*bounds, control.center_x, control.center_y, control.radius, layout.viewport)) continue;
    affected[i] = any_affected = true;
    // Pending circles must not block one another at their old HUD positions.
    control.visible = false;
  }
  if (!any_affected) return true;
  const auto& viewport = layout.viewport;
  if (!std::isfinite(viewport.safe_x) || !std::isfinite(viewport.safe_y) ||
      !std::isfinite(viewport.safe_width) || viewport.safe_width <= 0.0f ||
      !std::isfinite(viewport.safe_height) || viewport.safe_height <= 0.0f) return false;
  const float edge = std::min(viewport.safe_width, viewport.safe_height);
  const float padding = edge * kEdgePaddingRatio;
  const float gap = edge * kButtonGapRatio;
  const float right = viewport.safe_x + viewport.safe_width;
  const float bottom = viewport.safe_y + viewport.safe_height;
  if (!std::isfinite(right) || !std::isfinite(bottom)) return false;
  bool all_placed = true;
  for (size_t index = 0; index < count; ++index) {
    if (!affected[index]) continue;
    auto& control = layout.controls[index];
    const float r = control.radius;
    const float minimum_x = viewport.safe_x + padding + r;
    const float minimum_y = viewport.safe_y + padding + r;
    const float maximum_x = right - padding - r;
    const float maximum_y = bottom - padding - r;
    const bool editor_control =
        (control.action >= TouchAction::kEditDone && control.action <= TouchAction::kEditAimSpeed) ||
        (control.action >= TouchAction::kEditVehicleSpeed && control.action <= TouchAction::kEditInvertY);
    if (minimum_x > maximum_x || minimum_y > maximum_y) { all_placed = false; continue; }
    const auto available = [&](float x, float y) {
      if (!std::isfinite(x) || !std::isfinite(y) || x < minimum_x || x > maximum_x ||
          y < minimum_y || y > maximum_y || HudOverlapsCircle(*bounds, x, y, r, viewport)) return false;
      // This pass runs after mirroring and saved placement, so these are the
      // final screen coordinates, matching compact placement's radar reserve.
      if (!editor_control && x - r < viewport.safe_x + viewport.safe_width * 0.34f &&
          y - r < viewport.safe_y + viewport.safe_height * 0.40f) return false;
      for (size_t other_index = 0; other_index < count; ++other_index) {
        const auto& other = layout.controls[other_index];
        if (!other.visible || other.kind == ContextTouchControlKind::kLookSurface) continue;
        const double dx = double(x) - other.center_x, dy = double(y) - other.center_y;
        const double separation = double(r) + other.radius + gap * 0.25f;
        if (dx * dx + dy * dy < separation * separation) return false;
      }
      return true;
    };
    float target_x = control.center_x, target_y = control.center_y;
    bool placed = false;
    double nearest = std::numeric_limits<double>::infinity();
    const std::array candidates = {
        std::pair{bounds->left - r - gap, control.center_y},
        std::pair{bounds->right + r + gap, control.center_y},
        std::pair{control.center_x, bounds->top - r - gap},
        std::pair{control.center_x, bounds->bottom + r + gap},
    };
    for (const auto& [candidate_x, candidate_y] : candidates) {
      const float x = std::clamp(candidate_x, minimum_x, maximum_x);
      const float y = std::clamp(candidate_y, minimum_y, maximum_y);
      if (!available(x, y)) continue;
      const double dx = double(x) - control.center_x, dy = double(y) - control.center_y;
      const double distance = dx * dx + dy * dy;
      if (distance >= nearest) continue;
      nearest = distance;
      target_x = x; target_y = y;
      placed = true;
    }
    if (!placed) {
      const float step = (r * 2.0f + gap) * 0.125f;
      const size_t rows = static_cast<size_t>(std::clamp(
          std::ceil(viewport.safe_height / step), 1.0f, 128.0f));
      const size_t columns = static_cast<size_t>(std::clamp(
          std::ceil(viewport.safe_width / step), 1.0f, 128.0f));
      const bool from_left = control.center_x < viewport.safe_x + viewport.safe_width * 0.5f;
      for (size_t row = 0; row < rows && !placed; ++row) {
        const float y = minimum_y + float(row) * step;
        if (y > maximum_y) break;
        for (size_t column = 0; column < columns; ++column) {
          const float x = from_left ? minimum_x + float(column) * step
                                    : maximum_x - float(column) * step;
          if (x < minimum_x || x > maximum_x) break;
          if (!available(x, y)) continue;
          target_x = x; target_y = y;
          placed = true;
          break;
        }
      }
    }
    if (!placed) { all_placed = false; continue; }
    control.center_x = target_x; control.center_y = target_y;
    control.minimum_x = target_x - r; control.maximum_x = target_x + r;
    control.minimum_y = target_y - r; control.maximum_y = target_y + r;
    control.visible = true;
  }
  return all_placed;
}

bool ContextTouchLayoutEquivalent(const ContextTouchLayout& left,
                                  const ContextTouchLayout& right) noexcept {
  if (left.mode != right.mode || left.control_count != right.control_count ||
      left.viewport.generation != right.viewport.generation ||
      left.viewport.output_width != right.viewport.output_width ||
      left.viewport.output_height != right.viewport.output_height ||
      left.viewport.physical_output_x != right.viewport.physical_output_x ||
      left.viewport.physical_output_y != right.viewport.physical_output_y ||
      left.viewport.physical_output_width != right.viewport.physical_output_width ||
      left.viewport.physical_output_height != right.viewport.physical_output_height ||
      left.viewport.physical_surface_width != right.viewport.physical_surface_width ||
      left.viewport.physical_surface_height != right.viewport.physical_surface_height ||
      left.viewport.safe_x != right.viewport.safe_x ||
      left.viewport.safe_y != right.viewport.safe_y ||
      left.viewport.safe_width != right.viewport.safe_width ||
      left.viewport.safe_height != right.viewport.safe_height ||
      left.viewport.valid != right.viewport.valid ||
      left.viewport.focused != right.viewport.focused) {
    return false;
  }
  if (left.viewport.host_space != right.viewport.host_space ||
      left.viewport.logical_width != right.viewport.logical_width ||
      left.viewport.logical_height != right.viewport.logical_height) return false;
  for (size_t index = 0; index < left.control_count; ++index) {
    const ContextTouchControl& a = left.controls[index];
    const ContextTouchControl& b = right.controls[index];
    if (a.kind != b.kind || a.action != b.action || a.key != b.key ||
        a.pad_buttons != b.pad_buttons || a.trigger_side != b.trigger_side ||
        a.trigger_value != b.trigger_value || a.weapon_slot != b.weapon_slot ||
        a.script.input_group != b.script.input_group || a.script.script_thread != b.script.script_thread ||
        a.script.generation != b.script.generation || a.script.kind != b.script.kind ||
        a.script.action != b.script.action || a.center_x != b.center_x ||
        a.center_y != b.center_y || a.radius != b.radius || a.minimum_x != b.minimum_x ||
        a.minimum_y != b.minimum_y || a.maximum_x != b.maximum_x || a.maximum_y != b.maximum_y) {
      return false;
    }
  }
  return true;
}

bool ContextTouchControlContains(const ContextTouchControl& control,
                                const ContextTouchViewport& viewport,
                                float x, float y) noexcept {
  if (!control.visible || !std::isfinite(x) || !std::isfinite(y)) return false;
  if (control.kind == ContextTouchControlKind::kLookSurface) {
    return x >= control.minimum_x && x <= control.maximum_x &&
           y >= control.minimum_y && y <= control.maximum_y;
  }
  float sx = 1.0f, sy = 1.0f;
  if (!viewport.host_space && viewport.output_width > 0.0f && viewport.output_height > 0.0f &&
      viewport.physical_output_width > 0.0f && viewport.physical_output_height > 0.0f) {
    sx = viewport.physical_output_width / viewport.output_width;
    sy = viewport.physical_output_height / viewport.output_height;
  }
  const float dx = (x - control.center_x) * sx;
  const float dy = (y - control.center_y) * sy;
  const float radius = control.radius * std::min(sx, sy);
  return std::isfinite(radius) && radius > 0.0f && dx * dx + dy * dy <= radius * radius;
}

int32_t ContextTouchAxis(float displacement, float radius) noexcept {
  if (!std::isfinite(displacement) || !std::isfinite(radius) || radius <= 0.0f) {
    return 0;
  }
  const float normalized = std::clamp(displacement / radius, -1.0f, 1.0f);
  return static_cast<int32_t>(std::lround(normalized * static_cast<float>(kActionExtent)));
}

ContextTouchOverlayTransform BuildContextTouchOverlayTransform(const ContextTouchViewport& viewport,
                                                               float logical_width,
                                                               float logical_height) noexcept {
  if (viewport.host_space) {
    if (!viewport.valid || !viewport.focused || !std::isfinite(viewport.logical_width) ||
        !std::isfinite(viewport.logical_height) || !std::isfinite(logical_width) ||
        !std::isfinite(logical_height) || viewport.logical_width <= 0.0f ||
        viewport.logical_height <= 0.0f || logical_width <= 0.0f || logical_height <= 0.0f) return {};
    return {.scale_x = logical_width / viewport.logical_width,
            .scale_y = logical_height / viewport.logical_height, .valid = true};
  }
  if (!viewport.valid || !viewport.focused || !std::isfinite(viewport.output_width) ||
      !std::isfinite(viewport.output_height) || !std::isfinite(viewport.physical_output_x) ||
      !std::isfinite(viewport.physical_output_y) ||
      !std::isfinite(viewport.physical_output_width) ||
      !std::isfinite(viewport.physical_output_height) ||
      !std::isfinite(viewport.physical_surface_width) ||
      !std::isfinite(viewport.physical_surface_height) || !std::isfinite(logical_width) ||
      !std::isfinite(logical_height) || viewport.output_width <= 0.0f ||
      viewport.output_height <= 0.0f || viewport.physical_output_width <= 0.0f ||
      viewport.physical_output_height <= 0.0f || viewport.physical_surface_width <= 0.0f ||
      viewport.physical_surface_height <= 0.0f || logical_width <= 0.0f || logical_height <= 0.0f) {
    return {};
  }
  const float physical_to_logical_x = logical_width / viewport.physical_surface_width;
  const float physical_to_logical_y = logical_height / viewport.physical_surface_height;
  return {
      .offset_x = viewport.physical_output_x * physical_to_logical_x,
      .offset_y = viewport.physical_output_y * physical_to_logical_y,
      .scale_x = viewport.physical_output_width / viewport.output_width * physical_to_logical_x,
      .scale_y = viewport.physical_output_height / viewport.output_height * physical_to_logical_y,
      .valid = true,
  };
}

void ContextTouchKeyLatch::Press(rex::ui::VirtualKey key, uint64_t epoch) noexcept {
  const size_t index = static_cast<uint16_t>(key);
  if (index < refcounts_.size()) {
    if (!refcounts_[index]) {
      pressed_epoch_[index] = epoch;
    }
    if (refcounts_[index] != std::numeric_limits<uint16_t>::max()) {
      ++refcounts_[index];
    }
  }
}

void ContextTouchKeyLatch::Release(rex::ui::VirtualKey key, bool cancelled) noexcept {
  const size_t index = static_cast<uint16_t>(key);
  if (index < refcounts_.size() && refcounts_[index]) {
    --refcounts_[index];
    if (cancelled && !refcounts_[index]) {
      pressed_epoch_[index] = 0;
    }
  }
}

void ContextTouchKeyLatch::Cancel() noexcept {
  refcounts_.fill(0);
  pressed_epoch_.fill(0);
}

void ContextTouchKeyLatch::Collect(uint64_t epoch, std::array<uint8_t, 256>& down,
                                   std::array<uint8_t, 256>& pressed) const noexcept {
  for (size_t index = 0; index < refcounts_.size(); ++index) {
    const bool edge = epoch != 0 && pressed_epoch_[index] == epoch;
    // GTA reads held action bytes: keep a sub-poll tap held for its one poll.
    if (refcounts_[index] || edge) {
      down[index] = 1;
    }
    if (edge) {
      pressed[index] = 1;
    }
  }
}

}  // namespace gta4::input
