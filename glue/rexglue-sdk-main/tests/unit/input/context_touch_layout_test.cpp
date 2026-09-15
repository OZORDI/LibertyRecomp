#include "input/context_touch_layout.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <string_view>

#include <rex/input/input.h>

#include <catch2/catch_test_macros.hpp>

namespace gta4::input {
namespace {

ContextTouchViewport TestViewport() {
  return {
      .output_width = 1280.0f,
      .output_height = 720.0f,
      .physical_output_x = 64.0f,
      .physical_output_y = 36.0f,
      .physical_output_width = 1920.0f,
      .physical_output_height = 1080.0f,
      .physical_surface_width = 2048.0f,
      .physical_surface_height = 1152.0f,
      .safe_x = 32.0f,
      .safe_y = 18.0f,
      .safe_width = 1216.0f,
      .safe_height = 684.0f,
      .generation = 7,
      .valid = true,
      .focused = true,
  };
}

using A = TouchAction;
using K = ContextTouchControlKind;
using M = ContextTouchMode;
using namespace rex::input;

// Values captured from the original saved-layout enum before removing the
// retired controls. New controls must not reuse these stored identities.
static_assert(static_cast<uint16_t>(A::kPause) == 16);
static_assert(static_cast<uint16_t>(A::kMore) == 17);
static_assert(static_cast<uint16_t>(A::kSettings) == 18);
static_assert(static_cast<uint16_t>(A::kAccelerate) == 19);
static_assert(static_cast<uint16_t>(A::kNativeA) == 46);
static_assert(static_cast<uint16_t>(A::kNativeRightStick) == 63);
static_assert(static_cast<uint16_t>(A::kScript) == 64);

ContextTouchLayout SemanticLayout(M mode, ContextTouchLayoutOptions options = {},
                                 std::span<const TouchScriptControl> prompts = {}) {
  options.contextual = true;
  return BuildContextTouchLayout(mode, TestViewport(), prompts, options);
}

size_t CountVisibleKind(const ContextTouchLayout& layout, K kind) {
  size_t count = 0;
  for (size_t index = 0; index < layout.control_count; ++index) {
    const auto& control = layout.controls[index];
    if (control.visible && control.kind == kind) ++count;
  }
  return count;
}

const ContextTouchControl* FindAction(const ContextTouchLayout& layout, A action) {
  for (size_t index = 0; index < layout.control_count; ++index) {
    if (layout.controls[index].action == action) return &layout.controls[index];
  }
  return nullptr;
}

const ContextTouchControl& VisibleAction(const ContextTouchLayout& layout, A action) {
  const auto* control = FindAction(layout, action);
  REQUIRE(control);
  REQUIRE(control->visible);
  return *control;
}

void CheckButton(const ContextTouchLayout& layout, A action, uint16_t buttons) {
  const auto& control = VisibleAction(layout, action);
  CHECK(control.kind == K::kNativeButton);
  CHECK(control.pad_buttons == buttons);
  CHECK(control.trigger_side == 0);
}

void CheckTrigger(const ContextTouchLayout& layout, A action, uint8_t side) {
  const auto& control = VisibleAction(layout, action);
  CHECK(control.kind == K::kNativeTrigger);
  CHECK(control.pad_buttons == 0);
  CHECK(control.trigger_side == side);
  CHECK(control.trigger_value != 0);
}

bool OverlapsHud(const ContextTouchControl& control, const ContextTouchHudBounds& bounds) {
  const double nearest_x = std::clamp(double(control.center_x), double(bounds.left), double(bounds.right));
  const double nearest_y = std::clamp(double(control.center_y), double(bounds.top), double(bounds.bottom));
  return std::hypot(control.center_x - nearest_x, control.center_y - nearest_y) <= control.radius;
}

TEST_CASE("context touch layout is hidden for invalid presentation", "[input][touch][layout]") {
  auto viewport = TestViewport();
  SECTION("focus lost") { viewport.focused = false; }
  SECTION("invalid output") { viewport.valid = false; }
  SECTION("empty safe area") { viewport.safe_width = 0.0f; }
  SECTION("nonfinite safe area") { viewport.safe_height = std::numeric_limits<float>::quiet_NaN(); }
  CHECK(BuildContextTouchLayout(M::kOnFoot, viewport, {}, {.contextual = true}).control_count == 0);
}

TEST_CASE("native pause and map never receive an overlay layout", "[input][touch][layout]") {
  for (const auto mode : {M::kFrontend, M::kMap}) {
    for (const bool contextual : {false, true}) {
      for (const bool editing : {false, true}) {
        const auto layout = BuildContextTouchLayout(mode, TestViewport(), {},
            {.contextual = contextual, .phone_visible = true, .armed = true,
             .can_enter_vehicle = true, .editing = editing});
        CHECK(layout.control_count == 0);
      }
    }
  }
}

TEST_CASE("on foot controls use native actions for the observed combat state", "[input][touch][layout]") {
  SECTION("unarmed") {
    const auto layout = SemanticLayout(M::kOnFoot);
    CHECK(VisibleAction(layout, A::kMove).kind == K::kMovementStick);
    CHECK(VisibleAction(layout, A::kCamera).kind == K::kLookSurface);
    CheckButton(layout, A::kRunSprint, X_INPUT_GAMEPAD_A);
    CheckButton(layout, A::kJumpClimb, X_INPUT_GAMEPAD_X);
    CheckTrigger(layout, A::kFire, 2);
    CHECK(std::string_view(VisibleAction(layout, A::kFire).label.data()) == "ATTACK");
    const auto* enter = FindAction(layout, A::kContext);
    REQUIRE(enter);
    CHECK_FALSE(enter->visible);
    CheckButton(layout, A::kCrouch, X_INPUT_GAMEPAD_LEFT_THUMB);
    CheckButton(layout, A::kCover, X_INPUT_GAMEPAD_RIGHT_SHOULDER);
    CheckButton(layout, A::kCameraCycle, X_INPUT_GAMEPAD_BACK);
    CheckButton(layout, A::kLookBehind, X_INPUT_GAMEPAD_RIGHT_THUMB);
    CHECK(VisibleAction(layout, A::kWeaponWheel).kind == K::kUtility);
  }
  SECTION("native vehicle entry candidate") {
    CheckButton(SemanticLayout(M::kOnFoot, {.can_enter_vehicle = true}), A::kContext, X_INPUT_GAMEPAD_Y);
  }
  SECTION("editor previews vehicle entry") {
    CheckButton(SemanticLayout(M::kOnFoot, {.editing = true}), A::kContext, X_INPUT_GAMEPAD_Y);
  }
  SECTION("armed in cover") {
    const auto layout = SemanticLayout(M::kOnFoot, {.armed = true, .in_cover = true});
    CheckTrigger(layout, A::kFire, 2);
    CheckTrigger(layout, A::kAim, 1);
    CheckButton(layout, A::kCover, X_INPUT_GAMEPAD_RIGHT_SHOULDER);
    CheckButton(layout, A::kReload, X_INPUT_GAMEPAD_B);
    CheckButton(layout, A::kRunSprint, X_INPUT_GAMEPAD_A);
    CheckButton(layout, A::kJumpClimb, X_INPUT_GAMEPAD_X);
    CHECK(std::string_view(VisibleAction(layout, A::kCover).label.data()) == "EXIT COVER");
  }
  SECTION("melee") {
    const auto layout = SemanticLayout(M::kOnFoot, {.melee = true});
    CheckButton(layout, A::kFire, X_INPUT_GAMEPAD_B);
    CheckButton(layout, A::kContext, X_INPUT_GAMEPAD_Y);
    CheckButton(layout, A::kJumpClimb, X_INPUT_GAMEPAD_X);
    CheckButton(layout, A::kRunSprint, X_INPUT_GAMEPAD_A);
    CHECK(std::string_view(VisibleAction(layout, A::kFire).label.data()) == "PUNCH");
  }
}

TEST_CASE("phone is an overlay that preserves base movement and camera", "[input][touch][layout]") {
  for (const M mode : {M::kOnFoot, M::kVehicleAutomobile, M::kVehicleHelicopter}) {
    CAPTURE(mode);
    const auto base = SemanticLayout(mode);
    const auto phone = SemanticLayout(mode, {.phone_visible = true});
    const auto& base_move = VisibleAction(base, A::kMove);
    const auto& phone_move = VisibleAction(phone, A::kMove);
    CHECK(phone_move.center_x == base_move.center_x);
    CHECK(phone_move.center_y == base_move.center_y);
    CHECK(VisibleAction(phone, A::kCamera).kind == K::kLookSurface);
    CHECK(CountVisibleKind(phone, K::kLookSurface) == 1);
    CheckButton(phone, A::kPhoneAccept, X_INPUT_GAMEPAD_A);
    CheckButton(phone, A::kPhoneBack, X_INPUT_GAMEPAD_B);
    CheckButton(phone, A::kPhoneUp, X_INPUT_GAMEPAD_DPAD_UP);
    CheckButton(phone, A::kPhoneDown, X_INPUT_GAMEPAD_DPAD_DOWN);
    CheckButton(phone, A::kPhoneLeft, X_INPUT_GAMEPAD_DPAD_LEFT);
    CheckButton(phone, A::kPhoneRight, X_INPUT_GAMEPAD_DPAD_RIGHT);
    if (mode == M::kVehicleAutomobile) CheckTrigger(phone, A::kAccelerate, 2);
    if (mode == M::kVehicleHelicopter) CheckTrigger(phone, A::kHeliAscend, 2);
  }
  const auto phone_mode = SemanticLayout(M::kPhone);
  CheckButton(phone_mode, A::kPhoneAccept, X_INPUT_GAMEPAD_A);
  CheckButton(phone_mode, A::kPhoneBack, X_INPUT_GAMEPAD_B);
  CheckTrigger(phone_mode, A::kFire, 2);
}

TEST_CASE("vehicle and parachute families retain their camera and native controls", "[input][touch][layout]") {
  for (const M mode : {M::kVehicleAutomobile, M::kVehicleBike, M::kVehicleBoat}) {
    CAPTURE(mode);
    const auto layout = SemanticLayout(mode);
    CHECK(VisibleAction(layout, A::kMove).kind == K::kMovementStick);
    CHECK(VisibleAction(layout, A::kCamera).kind == K::kLookSurface);
    CheckTrigger(layout, A::kAccelerate, 2);
    CheckTrigger(layout, A::kBrake, 1);
    CheckButton(layout, A::kVehicleFire, X_INPUT_GAMEPAD_LEFT_SHOULDER);
    CheckButton(layout, A::kHandbrake, X_INPUT_GAMEPAD_RIGHT_SHOULDER);
    CheckButton(layout, A::kContext, X_INPUT_GAMEPAD_Y);
    CheckButton(layout, A::kHorn, X_INPUT_GAMEPAD_LEFT_THUMB);
    CheckButton(layout, A::kHeadlights, X_INPUT_GAMEPAD_X);
    CheckButton(layout, A::kRadioPrevious, X_INPUT_GAMEPAD_DPAD_LEFT);
    CheckButton(layout, A::kRadioNext, X_INPUT_GAMEPAD_DPAD_RIGHT);
    CheckButton(layout, A::kCameraCycle, X_INPUT_GAMEPAD_BACK);
    CheckButton(layout, A::kLookBehind, X_INPUT_GAMEPAD_RIGHT_THUMB);
  }
  const auto helicopter = SemanticLayout(M::kVehicleHelicopter);
  CheckTrigger(helicopter, A::kHeliAscend, 2);
  CheckTrigger(helicopter, A::kHeliDescend, 1);
  CheckButton(helicopter, A::kHeliYawLeft, X_INPUT_GAMEPAD_LEFT_SHOULDER);
  CheckButton(helicopter, A::kHeliYawRight, X_INPUT_GAMEPAD_RIGHT_SHOULDER);
  CheckButton(helicopter, A::kContext, X_INPUT_GAMEPAD_Y);
  CheckButton(helicopter, A::kVehicleFire, X_INPUT_GAMEPAD_A);
  CheckButton(helicopter, A::kHeliAction, X_INPUT_GAMEPAD_X);
  const auto passenger = SemanticLayout(M::kVehiclePassenger);
  CHECK(VisibleAction(passenger, A::kCamera).kind == K::kLookSurface);
  CheckButton(passenger, A::kContext, X_INPUT_GAMEPAD_Y);
  CheckButton(passenger, A::kVehicleFire, X_INPUT_GAMEPAD_LEFT_SHOULDER);
  for (const M mode : {M::kParachuteFreefall, M::kParachuteDeployed}) {
    const auto layout = SemanticLayout(mode);
    CHECK(VisibleAction(layout, A::kMove).kind == K::kMovementStick);
    CHECK(VisibleAction(layout, A::kCamera).kind == K::kLookSurface);
    // These actions go through the verified parachute script aliases, not an
    // unrelated native fire/exit button while the script owns the activity.
    const auto& action = VisibleAction(layout, mode == M::kParachuteFreefall ? A::kDeploy : A::kDetach);
    CHECK(action.pad_buttons == 0);
    CHECK(action.trigger_side == 0);
  }
}

TEST_CASE("contextual controls never generate More Pause or a raw controller grid", "[input][touch][layout]") {
  for (const M mode : {M::kOnFoot, M::kVehicleAutomobile, M::kVehicleBike, M::kVehicleBoat,
      M::kVehicleHelicopter, M::kVehicleDriverUnknown, M::kVehiclePassenger, M::kPhone,
      M::kParachuteFreefall, M::kParachuteDeployed, M::kMinigame}) {
    for (const bool editing : {false, true}) {
      const auto layout = SemanticLayout(mode, {.armed = true, .can_enter_vehicle = true, .editing = editing});
      for (size_t index = 0; index < layout.control_count; ++index) {
        const auto& control = layout.controls[index];
        CHECK(control.action != A::kMore);
        CHECK(control.action != A::kPause);
        CHECK((control.action < A::kNativeA || control.action > A::kNativeRightStick));
      }
    }
  }
  const auto unknown = SemanticLayout(M::kVehicleDriverUnknown);
  CheckButton(unknown, A::kContext, X_INPUT_GAMEPAD_Y);
  CheckTrigger(unknown, A::kAccelerate, 2);
  CheckTrigger(unknown, A::kBrake, 1);
  const auto activity = SemanticLayout(M::kMinigame);
  CHECK(CountVisibleKind(activity, K::kScriptButton) == 0);
  CHECK(CountVisibleKind(activity, K::kNativeButton) == 0);
  CHECK(CountVisibleKind(activity, K::kNativeTrigger) == 0);
}

TEST_CASE("accepted native prompt controls retain their complete script identity", "[input][touch][layout]") {
  // The runtime admits recent visible native help tokens before calling layout.
  // Layout receives that accepted list, never arbitrary background queries.
  constexpr TouchScriptControl held{TouchScriptQueryKind::kControlHeld, 17, 2, 0xA100, 9};
  auto pressed = held; pressed.kind = TouchScriptQueryKind::kControlPressed;
  auto analog = held; analog.kind = TouchScriptQueryKind::kControlAnalog;
  const std::array same_action = {held, pressed, analog};
  const auto canonical = SemanticLayout(M::kMinigame, {}, same_action);
  REQUIRE(CountVisibleKind(canonical, K::kScriptButton) == 1);
  const auto& control = VisibleAction(canonical, A::kScript);
  CHECK(control.script.kind == TouchScriptQueryKind::kControlHeld);
  CHECK(control.script.action == held.action);
  CHECK(control.script.input_group == held.input_group);
  CHECK(control.script.script_thread == held.script_thread);
  CHECK(control.script.generation == held.generation);
  CHECK(std::string_view(control.label.data()) == "LOOK RIGHT");

  auto other = held;
  SECTION("raw and semantic actions are separate namespaces") { other.kind = TouchScriptQueryKind::kRawButtonPressed; }
  SECTION("different input groups remain separate") { other.input_group = 3; }
  SECTION("different native script instances remain separate") { other.script_thread = 0xA101; }
  SECTION("different world generations remain separate") { other.generation = 10; }
  const std::array distinct = {held, other};
  const auto separate = SemanticLayout(M::kMinigame, {}, distinct);
  REQUIRE(CountVisibleKind(separate, K::kScriptButton) == 2);
  for (size_t i = 0; i < separate.control_count; ++i) {
    const auto& prompt = separate.controls[i];
    if (prompt.kind != K::kScriptButton || !prompt.visible) continue;
    CHECK(ContextTouchControlContains(prompt, separate.viewport, prompt.center_x, prompt.center_y));
    if (prompt.script.kind == TouchScriptQueryKind::kRawButton) {
      CHECK(std::string_view(prompt.label.data()) == "B");
    }
  }
  const std::array next = {other};
  CHECK_FALSE(ContextTouchLayoutEquivalent(canonical, SemanticLayout(M::kMinigame, {}, next)));
}

TEST_CASE("native Pause prompts leave the minimap as the only pause control", "[input][touch][layout]") {
  const std::array prompts = {
      TouchScriptControl{TouchScriptQueryKind::kControlHeld, 76},
      TouchScriptControl{TouchScriptQueryKind::kControlPressed, 76},
      TouchScriptControl{TouchScriptQueryKind::kControlAnalog, 76},
      TouchScriptControl{TouchScriptQueryKind::kRawButton, 12},
      TouchScriptControl{TouchScriptQueryKind::kRawButtonPressed, 12},
  };
  for (const bool contextual : {false, true}) {
    const auto layout = BuildContextTouchLayout(M::kMinigame, TestViewport(), prompts,
                                               {.contextual = contextual});
    CHECK(CountVisibleKind(layout, K::kScriptButton) == 0);
  }
}

TEST_CASE("an accepted analogue prompt exposes both script sticks", "[input][touch][layout]") {
  constexpr TouchScriptControl sticks{TouchScriptQueryKind::kAnalogueSticks, 0, 2, 0xA100, 9};
  constexpr std::array accepted = {sticks};
  const auto layout = SemanticLayout(M::kMinigame, {}, accepted);
  CHECK(CountVisibleKind(layout, K::kScriptButton) == 0);
  CHECK(CountVisibleKind(layout, K::kMovementStick) == 1);
  CHECK(CountVisibleKind(layout, K::kRightStick) == 1);
  for (const auto options : {ContextTouchLayoutOptions{},
                            ContextTouchLayoutOptions{.armed = true},
                            ContextTouchLayoutOptions{.phone_visible = true},
                            ContextTouchLayoutOptions{.melee = true}}) {
    const auto on_foot = SemanticLayout(M::kOnFoot, options, accepted);
    if (options.melee) CheckButton(on_foot, A::kFire, X_INPUT_GAMEPAD_B);
    else CheckTrigger(on_foot, A::kFire, 2);
    CHECK(VisibleAction(on_foot, A::kActivityRightStick).kind == K::kRightStick);
  }
  for (const A action : {A::kMove, A::kActivityRightStick}) {
    const auto& control = VisibleAction(layout, action);
    CHECK(control.script.kind == sticks.kind);
    CHECK(control.script.input_group == sticks.input_group);
    CHECK(control.script.script_thread == sticks.script_thread);
    CHECK(control.script.generation == sticks.generation);
  }
}

TEST_CASE("touch axes preserve sign and clamp to GTA action extent") {
  CHECK(ContextTouchAxis(0.0f, 100.0f) == 0);
  CHECK(ContextTouchAxis(-100.0f, 100.0f) == -255);
  CHECK(ContextTouchAxis(100.0f, 100.0f) == 255);
  CHECK(ContextTouchAxis(1000.0f, 100.0f) == 255);
  CHECK(ContextTouchAxis(1.0f, 0.0f) == 0);
}

TEST_CASE("touch key latch keeps same-action owners and sub-poll taps") {
  ContextTouchKeyLatch latch;
  constexpr uint64_t epoch = 41;
  latch.Press(rex::ui::VirtualKey::kSpace, epoch);
  latch.Press(rex::ui::VirtualKey::kSpace, epoch);
  latch.Release(rex::ui::VirtualKey::kSpace);

  std::array<uint8_t, 256> down{};
  std::array<uint8_t, 256> pressed{};
  latch.Collect(epoch, down, pressed);
  const size_t space = static_cast<uint16_t>(rex::ui::VirtualKey::kSpace);
  CHECK(down[space] == 1);
  CHECK(pressed[space] == 1);

  latch.Release(rex::ui::VirtualKey::kSpace);
  down.fill(0);
  pressed.fill(0);
  latch.Collect(epoch, down, pressed);
  CHECK(down[space] == 1);
  CHECK(pressed[space] == 1);
  down.fill(0);
  pressed.fill(0);
  latch.Collect(42, down, pressed);
  CHECK(down[space] == 0);
  CHECK(pressed[space] == 0);

  latch.Cancel();
  pressed.fill(0);
  latch.Collect(epoch, down, pressed);
  CHECK(pressed[space] == 0);
}

TEST_CASE("physical output transform changes invalidate a touch layout") {
  const ContextTouchViewport viewport = TestViewport();
  const auto first = BuildContextTouchLayout(ContextTouchMode::kOnFoot, viewport, {}, {.contextual = true});
  ContextTouchViewport moved = viewport;
  moved.physical_output_x = 65.0f;
  const auto second = BuildContextTouchLayout(ContextTouchMode::kOnFoot, moved, {}, {.contextual = true});
  CHECK_FALSE(ContextTouchLayoutEquivalent(first, second));
}

TEST_CASE("touch overlay transform preserves host output letterboxing") {
  const ContextTouchViewport viewport = TestViewport();
  const ContextTouchOverlayTransform transform =
      BuildContextTouchOverlayTransform(viewport, 1024.0f, 576.0f);
  REQUIRE(transform.valid);
  CHECK(transform.offset_x == 32.0f);
  CHECK(transform.offset_y == 18.0f);
  CHECK(transform.scale_x == 0.75f);
  CHECK(transform.scale_y == 0.75f);

  ContextTouchViewport invalid = viewport;
  invalid.physical_surface_width = 0.0f;
  CHECK_FALSE(BuildContextTouchOverlayTransform(invalid, 1024.0f, 576.0f).valid);
}

TEST_CASE("native HUD bounds map through the displayed output rectangle", "[input][touch][layout]") {
  auto viewport = TestViewport();
  constexpr ContextTouchHudBounds normalized{0.75f, 0.125f, 0.875f, 0.25f};
  // Independent Python fixtures for normalized * guest extent and for
  // (physical offset + normalized * physical extent) * logical / surface.
  auto mapped = MapContextTouchHudBounds(normalized, viewport);
  REQUIRE(mapped);
  CHECK(mapped->left == 960.0f);
  CHECK(mapped->top == 90.0f);
  CHECK(mapped->right == 1120.0f);
  CHECK(mapped->bottom == 180.0f);
  viewport.host_space = true;
  viewport.logical_width = 1024.0f;
  viewport.logical_height = 576.0f;
  mapped = MapContextTouchHudBounds(normalized, viewport);
  REQUIRE(mapped);
  CHECK(mapped->left == 752.0f);
  CHECK(mapped->top == 85.5f);
  CHECK(mapped->right == 872.0f);
  CHECK(mapped->bottom == 153.0f);
  mapped = MapContextTouchHudBounds({-0.25f, -0.5f, 1.5f, 1.75f}, viewport);
  REQUIRE(mapped);
  CHECK(mapped->left == 32.0f);
  CHECK(mapped->top == 18.0f);
  CHECK(mapped->right == 992.0f);
  CHECK(mapped->bottom == 558.0f);
  CHECK_FALSE(MapContextTouchHudBounds({2.0f, 0.0f, 3.0f, 1.0f}, viewport));
  CHECK_FALSE(MapContextTouchHudBounds({0.5f, 0.0f, 0.5f, 1.0f}, viewport));
  CHECK_FALSE(MapContextTouchHudBounds({0.0f, 0.5f, 1.0f, 0.25f}, viewport));
  CHECK_FALSE(MapContextTouchHudBounds({0.0f, 0.0f, std::numeric_limits<float>::infinity(), 1.0f}, viewport));
  for (const auto field : {&ContextTouchViewport::physical_output_width,
                          &ContextTouchViewport::physical_output_height,
                          &ContextTouchViewport::physical_surface_width,
                          &ContextTouchViewport::physical_surface_height,
                          &ContextTouchViewport::logical_width,
                          &ContextTouchViewport::logical_height}) {
    for (const float value : {0.0f, -1.0f, std::numeric_limits<float>::quiet_NaN()}) {
      auto invalid = viewport;
      invalid.*field = value;
      CHECK_FALSE(MapContextTouchHudBounds(normalized, invalid));
    }
  }
  viewport.host_space = false;
  viewport.output_width = 0.0f;
  CHECK_FALSE(MapContextTouchHudBounds(normalized, viewport));
}

TEST_CASE("observed weapon HUD bounds reserve the same screen area in both handednesses", "[input][touch][layout]") {
  // Deliberately synthetic bounds exercise layout reservation independently of
  // the guest HUD capture; production supplies the observed rectangle.
  const ContextTouchHudBounds bounds{1000.0f, 18.0f, 1248.0f, 190.0f};
  for (const bool left_handed : {false, true}) {
    for (const M mode : {M::kOnFoot, M::kVehicleAutomobile, M::kVehicleHelicopter}) {
      ContextTouchLayoutOptions options{.armed = true, .left_handed = left_handed};
      const auto before = SemanticLayout(mode, options);
      options.weapon_hud_bounds = bounds;
      const auto after = SemanticLayout(mode, options);
      auto after_saved = before;
      REQUIRE(ApplyContextTouchHudReservation(after_saved, bounds));
      REQUIRE(after.control_count == before.control_count);
      for (size_t i = 0; i < before.control_count; ++i) {
        const auto& original = before.controls[i];
        const auto& reserved = after.controls[i];
        CHECK(reserved.action == original.action);
        CHECK(reserved.visible == original.visible);
        CHECK(reserved.radius == original.radius);
        if (original.kind == K::kLookSurface || !original.visible || !OverlapsHud(original, bounds)) {
          CHECK(after_saved.controls[i].center_x == original.center_x);
          CHECK(after_saved.controls[i].center_y == original.center_y);
        }
        if (reserved.visible && reserved.kind != K::kLookSurface) CHECK_FALSE(OverlapsHud(reserved, bounds));
      }
      if (mode == M::kOnFoot) CHECK(VisibleAction(after, A::kWeaponWheel).kind == K::kUtility);
    }
  }
}

TEST_CASE("HUD reservation repairs only saved circles that cover the HUD", "[input][touch][layout]") {
  ContextTouchLayout layout;
  layout.viewport = {.output_width = 360, .output_height = 320,
                     .safe_width = 360, .safe_height = 320, .valid = true, .focused = true};
  layout.controls[0] = {.kind = K::kUtility, .center_x = 250, .center_y = 50, .radius = 20,
                        .minimum_x = 230, .minimum_y = 30, .maximum_x = 270, .maximum_y = 70,
                        .action = A::kWeaponWheel};
  layout.controls[1] = {.kind = K::kMovementStick, .center_x = 100, .center_y = 250, .radius = 20,
                        .minimum_x = 80, .minimum_y = 230, .maximum_x = 120, .maximum_y = 270,
                        .action = A::kMove};
  layout.controls[2] = {.kind = K::kLookSurface, .minimum_x = 0, .minimum_y = 0,
                        .maximum_x = 360, .maximum_y = 320, .action = A::kCamera};
  layout.control_count = 3;
  const auto before = layout;
  const ContextTouchHudBounds bounds{230, 30, 280, 90};
  REQUIRE(ApplyContextTouchHudReservation(layout, bounds));
  const auto& wheel = VisibleAction(layout, A::kWeaponWheel);
  CHECK(wheel.center_x == 202.0f);
  CHECK(wheel.center_y == 50.0f);
  CHECK(wheel.radius == before.controls[0].radius);
  CHECK_FALSE(OverlapsHud(wheel, bounds));
  auto unchanged = layout;
  unchanged.controls[0] = before.controls[0];
  CHECK(ContextTouchLayoutEquivalent(before, unchanged));
  const auto reserved = layout;
  REQUIRE(ApplyContextTouchHudReservation(layout, bounds));
  CHECK(ContextTouchLayoutEquivalent(reserved, layout));
  REQUIRE(ApplyContextTouchHudReservation(layout, std::nullopt));
  CHECK(ContextTouchLayoutEquivalent(reserved, layout));
}

TEST_CASE("HUD reservation uses circular corners and never occupies the radar", "[input][touch][layout]") {
  ContextTouchLayout layout;
  layout.viewport = {.output_width = 360, .output_height = 320,
                     .safe_width = 360, .safe_height = 320, .valid = true, .focused = true};
  layout.control_count = 1;
  auto& circle = layout.controls[0];
  circle.kind = K::kUtility;
  circle.action = A::kWeaponWheel;
  SECTION("bounding boxes touch but the circle clears the HUD corner") {
    circle.center_x = circle.center_y = 42.0f;
    circle.radius = 10.0f;
    const auto before = layout;
    REQUIRE(ApplyContextTouchHudReservation(layout, ContextTouchHudBounds{50, 50, 70, 70}));
    CHECK(ContextTouchLayoutEquivalent(before, layout));
  }
  SECTION("a circular corner overlap is relocated") {
    circle.center_x = circle.center_y = 45.0f;
    circle.radius = 8.0f;
    const ContextTouchHudBounds bounds{50, 50, 70, 70};
    REQUIRE(ApplyContextTouchHudReservation(layout, bounds));
    REQUIRE(circle.visible);
    CHECK_FALSE(OverlapsHud(circle, bounds));
  }
  SECTION("unequal output scaling preserves a rendered circle that already clears the HUD") {
    layout.viewport.output_width = layout.viewport.output_height = 100;
    layout.viewport.safe_width = layout.viewport.safe_height = 100;
    layout.viewport.physical_output_width = 200;
    layout.viewport.physical_output_height = 100;
    circle.center_x = circle.center_y = 50;
    circle.radius = 10;
    const auto before = layout;
    REQUIRE(ApplyContextTouchHudReservation(layout, ContextTouchHudBounds{56, 40, 70, 60}));
    CHECK(ContextTouchLayoutEquivalent(before, layout));
  }
  SECTION("the only space beside the HUD includes the radar") {
    circle.center_x = 300; circle.center_y = 50; circle.radius = 24;
    const ContextTouchHudBounds bounds{140, 0, 360, 320};
    REQUIRE(ApplyContextTouchHudReservation(layout, bounds));
    REQUIRE(circle.visible);
    CHECK_FALSE(OverlapsHud(circle, bounds));
    CHECK(circle.center_y - circle.radius >= 128.0f);
  }
  SECTION("a HUD covering the entire safe area has a bounded fallback") {
    circle.center_x = 300; circle.center_y = 50; circle.radius = 24;
    CHECK_FALSE(ApplyContextTouchHudReservation(layout, ContextTouchHudBounds{0, 0, 360, 320}));
    CHECK_FALSE(circle.visible);
  }
}

}  // namespace
}  // namespace gta4::input
