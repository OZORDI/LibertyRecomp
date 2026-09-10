#include "input/context_touch_layout.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>

namespace gta4::input {
namespace {

constexpr float kEdgePaddingRatio = 0.035f;
constexpr float kStickRadiusRatio = 0.105f;
constexpr float kButtonRadiusRatio = 0.0575f;
constexpr float kButtonGapRatio = 0.025f;
constexpr int32_t kActionExtent = 255;

void SetLabel(ContextTouchControl& control, const char* label) {
  std::strncpy(control.label.data(), label, control.label.size() - 1);
  control.label.back() = '\0';
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
    std::span<const TouchScriptControl> script_controls) noexcept {
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

  if (mode == ContextTouchMode::kFrontend || mode == ContextTouchMode::kMap) {
    AddCircle(layout, ContextTouchControlKind::kButton, rex::ui::VirtualKey::kBack, "BACK",
              right - padding - button_radius, bottom - padding - button_radius, button_radius);
    return layout;
  }

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
    constexpr std::array buttons = {
        ButtonDescription{rex::ui::VirtualKey::kLButton, "FIRE"},
        ButtonDescription{rex::ui::VirtualKey::kSpace, "JUMP"},
        ButtonDescription{rex::ui::VirtualKey::kRButton, "AIM"},
        ButtonDescription{rex::ui::VirtualKey::kShift, "RUN"},
        ButtonDescription{rex::ui::VirtualKey::kF, "ENTER"},
        ButtonDescription{rex::ui::VirtualKey::kR, "RELOAD"},
        ButtonDescription{rex::ui::VirtualKey::kQ, "COVER"},
        ButtonDescription{rex::ui::VirtualKey::kUp, "PHONE"},
    };
    AddButtonGrid(layout, buttons, right, bottom, padding, button_radius, button_gap);
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
      const auto canonical = CanonicalTouchScriptControl(script_controls[index]);
      bool duplicate = false;
      for (size_t prior = 0; prior < index; ++prior) {
        const auto earlier = CanonicalTouchScriptControl(script_controls[prior]);
        duplicate |= earlier.kind == canonical.kind && earlier.action == canonical.action;
      }
      if (duplicate) {
        continue;
      }
      const float column = static_cast<float>(visible_index % 2);
      const float row = static_cast<float>(visible_index / 2);
      if (bottom - padding - button_radius * 2.0f - row * step < viewport.safe_y) break;
      char label[16]{};
      std::snprintf(label, sizeof(label),
                    canonical.kind == TouchScriptQueryKind::kRawButton ? "BTN %u" : "ACT %u",
                    canonical.action);
      AddCircle(layout, ContextTouchControlKind::kScriptButton, rex::ui::VirtualKey::kNone, label,
                right - padding - button_radius - column * step,
                bottom - padding - button_radius - row * step, button_radius,
                canonical);
      ++visible_index;
    }
  }
  return layout;
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
  for (size_t index = 0; index < left.control_count; ++index) {
    const ContextTouchControl& a = left.controls[index];
    const ContextTouchControl& b = right.controls[index];
    if (a.kind != b.kind || a.key != b.key || a.script.kind != b.script.kind ||
        a.script.action != b.script.action || a.center_x != b.center_x ||
        a.center_y != b.center_y || a.radius != b.radius || a.minimum_x != b.minimum_x ||
        a.minimum_y != b.minimum_y || a.maximum_x != b.maximum_x || a.maximum_y != b.maximum_y) {
      return false;
    }
  }
  return true;
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
