#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
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
  kRawButtonPressed,
};

// Stable identities are independent of the label, image and current screen slot.
enum class TouchAction : uint16_t {
  kNone, kMove, kCamera, kFire, kAim, kFreeAim, kRunSprint, kJumpClimb,
  kContext, kReload, kCover, kCrouch, kWeaponNext, kWeaponPrevious, kWeaponWheel,
  // Pause, More and Settings IDs remain reserved for saved layout compatibility.
  kPhone, kPause, kMore, kSettings, kAccelerate, kBrake, kHandbrake,
  kVehicleFire, kVehicleAltFire, kHorn, kHeadlights, kRadioPrevious, kRadioNext,
  kCameraCycle, kLookBehind, kHeliYawLeft, kHeliYawRight, kHeliAscend,
  kHeliDescend, kHeliAction, kPhoneUp, kPhoneDown, kPhoneLeft, kPhoneRight,
  kPhoneAccept, kPhoneBack, kDeploy, kParachuteBrakeLeft, kParachuteBrakeRight,
  kDetach, kSmoke, kNativeA, kNativeB, kNativeX, kNativeY, kNativeUp,
  kNativeDown, kNativeLeft, kNativeRight, kNativeStart, kNativeBack,
  kNativeLeftShoulder, kNativeRightShoulder, kNativeLeftThumb, kNativeRightThumb,
  kNativeLeftTrigger, kNativeRightTrigger, kNativeLeftStick, kNativeRightStick,
  kScript, kZoomIn, kZoomOut, kEditDone, kEditReset, kEditSmaller, kEditLarger,
  kEditOpacity, kEditHandedness, kEditFloating, kEditCameraSpeed, kEditAimSpeed,
  kWeaponSelect, kActivityRightStick, kEditVehicleSpeed, kEditFlightSpeed, kEditInvertY, kCount,
};

enum class ContextTouchControlKind : uint8_t {
  kButton,
  kMovementStick,
  kLookSurface,
  kScriptButton,
  kNativeButton,
  kNativeTrigger,
  kRightStick,
  kUtility,
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
  float logical_width = 0.0f;
  float logical_height = 0.0f;
  bool host_space = false;
};

struct TouchScriptControl {
  TouchScriptQueryKind kind = TouchScriptQueryKind::kControlHeld;
  uint32_t action = 0;
  uint32_t input_group = 0;
  uint32_t script_thread = 0;
  uint64_t generation = 0;
};

// Raw buttons and semantic actions are different ID namespaces. The held,
// pressed and analog queries of one semantic action share a touch owner.
constexpr TouchScriptControl CanonicalTouchScriptControl(TouchScriptControl control) noexcept {
  if (control.kind == TouchScriptQueryKind::kControlPressed ||
      control.kind == TouchScriptQueryKind::kControlAnalog) {
    control.kind = TouchScriptQueryKind::kControlHeld;
  }
  if (control.kind == TouchScriptQueryKind::kRawButtonPressed) {
    control.kind = TouchScriptQueryKind::kRawButton;
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
  TouchAction action = TouchAction::kNone;
  std::array<char, 48> icon_id{};
  std::array<char, 64> accessible_name{};
  uint16_t pad_buttons = 0;
  uint8_t trigger_side = 0;
  uint8_t trigger_value = 255;
  bool visible = true;
  uint8_t weapon_slot = 255;
};

struct ContextTouchHudBounds {
  float left = 0.0f;
  float top = 0.0f;
  float right = 0.0f;
  float bottom = 0.0f;
};

struct ContextTouchLayoutOptions {
  bool contextual = false;
  bool phone_visible = false;
  bool armed = false;
  bool aiming = false;
  bool free_aim_available = false;
  bool in_cover = false;
  bool melee = false;
  bool scoped_zoom = false;
  bool can_enter_vehicle = false;
  bool editing = false;
  bool weapon_wheel_open = false;
  std::array<uint32_t, 11> weapon_types{};
  std::array<bool, 11> weapon_selectable{};
  std::array<std::array<char, 24>, 11> weapon_names{};
  bool left_handed = false;
  float button_scale = 1.0f;
  float opacity = 0.65f;
  // Exact observed native HUD bounds, mapped into the layout's coordinates.
  std::optional<ContextTouchHudBounds> weapon_hud_bounds;
};

struct ContextTouchLayout {
  static constexpr size_t kMaximumControls = 96;

  ContextTouchMode mode = ContextTouchMode::kDisabled;
  ContextTouchViewport viewport{};
  std::array<ContextTouchControl, kMaximumControls> controls{};
  size_t control_count = 0;
  float opacity = 0.65f;
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
    std::span<const TouchScriptControl> script_controls = {},
    const ContextTouchLayoutOptions& options = {}) noexcept;

std::optional<ContextTouchHudBounds> MapContextTouchHudBounds(
    const ContextTouchHudBounds& normalized_bounds,
    const ContextTouchViewport& viewport) noexcept;

// Call again after applying saved placements. Unrelated controls retain their
// geometry. Returns false if a conflicting circle cannot fit and is hidden.
bool ApplyContextTouchHudReservation(
    ContextTouchLayout& layout,
    const std::optional<ContextTouchHudBounds>& bounds) noexcept;

bool ContextTouchControlContains(const ContextTouchControl& control,
                                const ContextTouchViewport& viewport,
                                float x, float y) noexcept;

bool ContextTouchLayoutEquivalent(const ContextTouchLayout& left,
                                  const ContextTouchLayout& right) noexcept;

int32_t ContextTouchAxis(float displacement, float radius) noexcept;

ContextTouchOverlayTransform BuildContextTouchOverlayTransform(const ContextTouchViewport& viewport,
                                                               float logical_width,
                                                               float logical_height) noexcept;

}  // namespace gta4::input
