#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

#include <rex/ui/virtual_key.h>

namespace gta4::input {

enum class ContextTouchMode : uint8_t {
  kDisabled,
  kOnFoot,
  kVehicleAutomobile,
  kVehicleBike,
  kVehicleBoat,
  kVehicleHelicopter,
  kVehicleDriverUnknown,
  kVehiclePassenger,
  kPhone,
  kParachuteFreefall,
  kParachuteDeployed,
  kMinigame,
  kFrontend,
  kMap,
};

enum class TouchScriptQueryKind : uint8_t {
  kRawButton,
  kControlHeld,
  kControlPressed,
  kControlAnalog,
  kAnalogueSticks,
};

enum class ContextTouchControlKind : uint8_t {
  kButton,
  kMovementStick,
  kLookSurface,
  kScriptButton,
};

struct ContextTouchViewport {
  float output_width = 0.0f;
  float output_height = 0.0f;
  float physical_output_x = 0.0f;
  float physical_output_y = 0.0f;
  float physical_output_width = 0.0f;
  float physical_output_height = 0.0f;
  float physical_surface_width = 0.0f;
  float physical_surface_height = 0.0f;
  float safe_x = 0.0f;
  float safe_y = 0.0f;
  float safe_width = 0.0f;
  float safe_height = 0.0f;
  uint64_t generation = 0;
  bool valid = false;
  bool focused = false;
};

struct TouchScriptControl {
  TouchScriptQueryKind kind = TouchScriptQueryKind::kControlHeld;
  uint32_t action = 0;
};

// Raw buttons and semantic actions are different ID namespaces. The held,
// pressed and analog queries of one semantic action share a touch owner.
constexpr TouchScriptControl CanonicalTouchScriptControl(TouchScriptControl control) noexcept {
  if (control.kind == TouchScriptQueryKind::kControlPressed ||
      control.kind == TouchScriptQueryKind::kControlAnalog) {
    control.kind = TouchScriptQueryKind::kControlHeld;
  }
  return control;
}

struct ContextTouchControl {
  ContextTouchControlKind kind = ContextTouchControlKind::kButton;
  rex::ui::VirtualKey key = rex::ui::VirtualKey::kNone;
  TouchScriptControl script{};
  float center_x = 0.0f;
  float center_y = 0.0f;
  float radius = 0.0f;
  float minimum_x = 0.0f;
  float minimum_y = 0.0f;
  float maximum_x = 0.0f;
  float maximum_y = 0.0f;
  std::array<char, 16> label{};
};

struct ContextTouchLayout {
  static constexpr size_t kMaximumControls = 32;

  ContextTouchMode mode = ContextTouchMode::kDisabled;
  ContextTouchViewport viewport{};
  std::array<ContextTouchControl, kMaximumControls> controls{};
  size_t control_count = 0;
};

struct ContextTouchOverlayTransform {
  float offset_x = 0.0f;
  float offset_y = 0.0f;
  float scale_x = 0.0f;
  float scale_y = 0.0f;
  bool valid = false;
};

class ContextTouchKeyLatch {
 public:
  void Press(rex::ui::VirtualKey key, uint64_t epoch) noexcept;
  void Release(rex::ui::VirtualKey key, bool cancelled = false) noexcept;
  void Cancel() noexcept;
  void Collect(uint64_t epoch, std::array<uint8_t, 256>& down,
               std::array<uint8_t, 256>& pressed) const noexcept;

 private:
  std::array<uint16_t, 256> refcounts_{};
  std::array<uint64_t, 256> pressed_epoch_{};
};

ContextTouchLayout BuildContextTouchLayout(
    ContextTouchMode mode, const ContextTouchViewport& viewport,
    std::span<const TouchScriptControl> script_controls = {}) noexcept;

bool ContextTouchLayoutEquivalent(const ContextTouchLayout& left,
                                  const ContextTouchLayout& right) noexcept;

int32_t ContextTouchAxis(float displacement, float radius) noexcept;

ContextTouchOverlayTransform BuildContextTouchOverlayTransform(const ContextTouchViewport& viewport,
                                                               float logical_width,
                                                               float logical_height) noexcept;

}  // namespace gta4::input
