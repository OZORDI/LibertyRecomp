#include "input/context_touch_controls.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <limits>
#include <mutex>
#include <numeric>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include <rex/cvar.h>
#include <rex/input/absolute_pointer.h>
#include <rex/input/input.h>
#include <rex/input/mnk/controller_compatibility.h>
#include <rex/input/mnk/encoded_action.h>
#include <rex/input/mnk/pointer_motion.h>
#include <rex/logging.h>

#include "gta4_touch_coordinator.h"
#include "input/context_touch_context.h"
#include "input/context_touch_fade.h"
#include "input/context_touch_settings.h"

REXCVAR_DECLARE(bool, gta4_touch_trace);

namespace gta4::input {
namespace {

// Generated sub_822B7DD0 and the action decoders use this record layout.
constexpr uint32_t kActionArrayOffset = 2328;
constexpr uint32_t kActionStride = 12;
constexpr uint32_t kActionCurrentOffset = 2;
constexpr uint32_t kControlUserIndexOffset = 3412;
constexpr uint32_t kLastInputTimeOffset = 4200;
constexpr uint32_t kGameInputTimeAddress = 0x82C6C2A4;
constexpr uint32_t kGameplayTimeStepAddress = 0x82C6C2AC;
constexpr uint64_t kScriptQueryExpiryEpochs = 6;
constexpr uint64_t kScopedObservationExpiryEpochs = 2;
constexpr uint32_t kParachuteFreefallState = 3;
constexpr uint32_t kParachuteDeployedState = 5;
constexpr double kLookUnitsPerPixel = 12.0;
constexpr double kReferenceFrameSeconds = 1.0 / 60.0;
constexpr double kPinchUnitsPerPixel = 12.0;
constexpr double kWeaponWheelHoldSeconds = 0.35;
constexpr uint64_t kWeaponWheelHoldNanoseconds = 350000000;
constexpr uint64_t kWeaponCycleRetryEpochs = 6;
constexpr uint32_t kNoWeaponSelection = std::numeric_limits<uint32_t>::max();

enum class Action : uint32_t {
  kMoveLeft = 12, kMoveRight = 13, kMoveUp = 14, kMoveDown = 15,
  kLookLeft = 16, kLookRight = 17, kLookUp = 18, kLookDown = 19,
  kVehicleMoveLeft = 30, kVehicleMoveRight = 31,
  kVehicleMoveUp = 32, kVehicleMoveDown = 33,
  kVehicleGunLeft = 34, kVehicleGunRight = 35,
  kVehicleGunUp = 36, kVehicleGunDown = 37,
  kVehicleLookLeft = 48, kVehicleLookRight = 49,
};

struct ScriptKey {
  TouchScriptQueryKind kind = TouchScriptQueryKind::kControlHeld;
  uint32_t action = 0;
  uint32_t input_group = 0;
  uint32_t script_thread = 0;
  uint64_t generation = 0;
  bool operator==(const ScriptKey&) const noexcept = default;
};

struct ScriptKeyHash {
  size_t operator()(const ScriptKey& key) const noexcept {
    size_t result = std::hash<uint32_t>{}(key.action);
    for (const uint64_t value : {uint64_t(key.kind), uint64_t(key.input_group),
                                 uint64_t(key.script_thread), key.generation}) {
      result ^= std::hash<uint64_t>{}(value) + 0x9e3779b9U + (result << 6) + (result >> 2);
    }
    return result;
  }
};

ScriptKey CanonicalScriptKey(TouchScriptControl control) {
  control = CanonicalTouchScriptControl(control);
  return {control.kind, control.action, control.input_group, control.script_thread,
          control.generation};
}

TouchScriptControl ScriptControl(const ScriptKey& key) {
  return {key.kind, key.action, key.input_group, key.script_thread, key.generation};
}

struct ScriptAvailability { uint64_t last_seen_epoch = 0; };

struct PointerOwner {
  size_t control_index = 0;
  ContextTouchControl control{};
  uint64_t down_epoch = 0;
  uint64_t down_timestamp_ns = 0;
  double held_seconds = 0.0;
  float start_x = 0.0f;
  float start_y = 0.0f;
  float x = 0.0f;
  float y = 0.0f;
  double look_dx = 0.0;
  double look_dy = 0.0;
  bool composite_fire = false;
  ContextTouchControl fire_control{};
  bool pinch_member = false;
  bool editing = false;
  bool wheel_opened = false;
  bool wheel_consumed = false;
};

struct NativePulse {
  uint64_t pointer_id = 0;
  uint64_t epoch = 0;
  ContextTouchControl control{};
};

struct RuntimeState {
  std::mutex mutex;
  ContextTouchLayout layout{};
  TouchContextSnapshot context{};
  ContextTouchPreferences preferences{};
  uint64_t epoch = 0;
  uint64_t frozen_epoch = 0;
  uint64_t layout_generation = 1;
  std::unordered_map<uint64_t, PointerOwner> pointers;
  ContextTouchKeyLatch key_latch{};
  std::unordered_map<ScriptKey, uint16_t, ScriptKeyHash> script_refcounts;
  std::unordered_map<ScriptKey, uint64_t, ScriptKeyHash> script_pressed_epoch;
  std::unordered_map<ScriptKey, ScriptAvailability, ScriptKeyHash> script_availability;
  std::vector<NativePulse> native_pulses;
  uint64_t parachute_last_seen_epoch = 0;
  uint64_t parachute_generation = 0;
  uint32_t parachute_state = 0;
  uint32_t parachute_thread = 0;
  uint64_t scoped_seen_epoch = 0;
  uint64_t scoped_generation = 0;
  uint32_t scoped_action = 24;
  bool scoped_zoom = false;
  bool awaiting_gameplay_poll = false;
  uint32_t presentation_transition_depth = 0;
  bool editing = false;
  bool editor_pending = false;
  bool editor_session = false;
  bool editor_closing = false;
  uint64_t editor_close_started_ns = 0;
  uint64_t last_drawable_ns = 0;
  ContextTouchFade fade{};
  ContextTouchOverlaySnapshot last_drawable{};
  bool weapon_wheel_open = false;
  uint32_t weapon_target = kNoWeaponSelection;
  uint32_t weapon_last_slot = kNoWeaponSelection;
  uint64_t weapon_cycle_epoch = 0;
  uint32_t weapon_cycle_attempts = 0;
  bool pinch_active = false;
  uint64_t pinch_first = 0;
  uint64_t pinch_second = 0;
  double pinch_distance = 0.0;
  float editor_pinch_radius = 0.0f;
  float editor_pinch_center_x = 0.0f;
  float editor_pinch_center_y = 0.0f;
  double zoom_delta = 0.0;
  double completed_zoom = 0.0;
  int32_t zoom = 0;
  int32_t movement_x = 0;
  int32_t movement_y = 0;
  int32_t right_x = 0;
  int32_t right_y = 0;
  double completed_look_x = 0.0;
  double completed_look_y = 0.0;
  int32_t look_x = 0;
  int32_t look_y = 0;
  double frame_seconds = kReferenceFrameSeconds;
  rex::input::mnk::MouseAxisQuantizer look_quantizer_x;
  rex::input::mnk::MouseAxisQuantizer look_quantizer_y;
  rex::input::mnk::MouseAxisQuantizer zoom_quantizer;
  TouchNativePadState native_pad{};
  bool initialized = false;
};

RuntimeState g_runtime;
std::atomic<uint64_t> g_presentation_revision{0};

ContextTouchOverlaySnapshot BuildOverlaySnapshotLocked();

uint64_t MonotonicNanoseconds() {
  return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::steady_clock::now().time_since_epoch()).count());
}

void FinishEditorFadeLocked(uint64_t monotonic_ns) {
  monotonic_ns = std::max(monotonic_ns, g_runtime.last_drawable_ns);
  if (!g_runtime.editor_closing || monotonic_ns < g_runtime.editor_close_started_ns ||
      monotonic_ns - g_runtime.editor_close_started_ns < ContextTouchFade::kDurationNanoseconds) return;
  g_runtime.editor_session = false;
  g_runtime.editor_closing = false;
  g_runtime.fade.Reset();
  g_runtime.last_drawable = {};
}

void BeginEditorFadeOutLocked() {
  if (!g_runtime.editor_session || g_runtime.editor_closing) return;
  g_runtime.last_drawable = BuildOverlaySnapshotLocked();
  g_runtime.editor_close_started_ns = std::max(MonotonicNanoseconds(), g_runtime.last_drawable_ns);
  g_runtime.fade.Advance(false, g_runtime.editor_close_started_ns);
  g_runtime.editor_closing = true;
}

uint8_t LoadU8(uint8_t* base, uint32_t address) {
  return *reinterpret_cast<volatile uint8_t*>(base + address);
}

uint32_t LoadU32(uint8_t* base, uint32_t address) {
  return __builtin_bswap32(*reinterpret_cast<volatile uint32_t*>(base + address));
}

void StoreU8(uint8_t* base, uint32_t address, uint8_t value) {
  *reinterpret_cast<volatile uint8_t*>(base + address) = value;
}

void StoreU32(uint8_t* base, uint32_t address, uint32_t value) {
  *reinterpret_cast<volatile uint32_t*>(base + address) = __builtin_bswap32(value);
}

bool EpochWithin(uint64_t current, uint64_t observed, uint64_t lifetime) {
  return observed && current >= observed && current - observed <= lifetime;
}

bool IsVehicle(ContextTouchMode mode) {
  return mode >= ContextTouchMode::kVehicleAutomobile &&
         mode <= ContextTouchMode::kVehiclePassenger;
}

bool IsParachute(ContextTouchMode mode) {
  return mode == ContextTouchMode::kParachuteFreefall ||
         mode == ContextTouchMode::kParachuteDeployed;
}

uint16_t WeaponCycleButton(ContextTouchMode mode) {
  if (mode == ContextTouchMode::kOnFoot) return rex::input::X_INPUT_GAMEPAD_DPAD_RIGHT;
  if (IsVehicle(mode) && mode != ContextTouchMode::kVehicleHelicopter)
    return rex::input::X_INPUT_GAMEPAD_X;
  return 0;
}

bool IsNative(const ContextTouchControl& control) {
  return control.kind == ContextTouchControlKind::kNativeButton ||
         control.kind == ContextTouchControlKind::kNativeTrigger;
}

bool IsStick(const ContextTouchControl& control) {
  return control.kind == ContextTouchControlKind::kMovementStick ||
         control.kind == ContextTouchControlKind::kRightStick;
}

bool IsCamera(const ContextTouchControl& control) {
  return control.kind == ContextTouchControlKind::kLookSurface ||
         control.action == TouchAction::kFire || control.action == TouchAction::kVehicleFire;
}

bool SameControlIdentity(const ContextTouchControl& a, const ContextTouchControl& b) {
  if (a.action != b.action || a.kind != b.kind) return false;
  if (a.action == TouchAction::kWeaponSelect) return a.weapon_slot == b.weapon_slot;
  if (a.kind == ContextTouchControlKind::kScriptButton ||
      a.script.kind == TouchScriptQueryKind::kAnalogueSticks ||
      b.script.kind == TouchScriptQueryKind::kAnalogueSticks) {
    return CanonicalScriptKey(a.script) == CanonicalScriptKey(b.script);
  }
  return a.action != TouchAction::kNone || a.key == b.key;
}

void ReleaseControlLocked(const ContextTouchControl& control, bool cancelled) {
  if (control.kind == ContextTouchControlKind::kButton) {
    g_runtime.key_latch.Release(control.key, cancelled);
  } else if (control.kind == ContextTouchControlKind::kScriptButton) {
    const auto key = CanonicalScriptKey(control.script);
    auto it = g_runtime.script_refcounts.find(key);
    if (it != g_runtime.script_refcounts.end() && it->second && --it->second == 0) {
      g_runtime.script_refcounts.erase(it);
      if (cancelled) g_runtime.script_pressed_epoch.erase(key);
    }
  }
}

void ResetPinchLocked() {
  g_runtime.pinch_active = false;
  g_runtime.pinch_distance = 0.0;
  g_runtime.zoom_delta = 0.0;
}

void CancelAllLocked() {
  g_runtime.pointers.clear();
  g_runtime.key_latch.Cancel();
  g_runtime.script_refcounts.clear();
  g_runtime.script_pressed_epoch.clear();
  g_runtime.native_pulses.clear();
  g_runtime.native_pad = {};
  g_runtime.movement_x = g_runtime.movement_y = 0;
  g_runtime.right_x = g_runtime.right_y = 0;
  g_runtime.look_x = g_runtime.look_y = 0;
  g_runtime.completed_look_x = g_runtime.completed_look_y = 0.0;
  g_runtime.completed_zoom = 0.0;
  g_runtime.zoom = 0;
  g_runtime.weapon_wheel_open = false;
  g_runtime.weapon_target = kNoWeaponSelection;
  g_runtime.weapon_last_slot = kNoWeaponSelection;
  g_runtime.weapon_cycle_epoch = 0;
  g_runtime.weapon_cycle_attempts = 0;
  g_runtime.look_quantizer_x.Reset();
  g_runtime.look_quantizer_y.Reset();
  g_runtime.zoom_quantizer.Reset();
  ResetPinchLocked();
}

void HideGameplayLocked() {
  g_runtime.awaiting_gameplay_poll = true;
  CancelAllLocked();
  g_runtime.fade.Reset();
  g_runtime.last_drawable = {};
}

void SuspendGameplayLocked() {
  g_presentation_revision.fetch_add(1, std::memory_order_release);
  g_runtime.awaiting_gameplay_poll = true;
  // An explicitly opened native-menu editor owns its own preview/input.
  if (g_runtime.editor_session) return;
  HideGameplayLocked();
  g_runtime.layout = {};
  g_runtime.script_availability.clear();
  g_runtime.parachute_last_seen_epoch = 0;
  g_runtime.scoped_seen_epoch = 0;
}

void CancelOwnerLocked(uint64_t pointer_id, const PointerOwner& owner) {
  if (!owner.editing) {
    ReleaseControlLocked(owner.control, true);
    if (owner.composite_fire) ReleaseControlLocked(owner.fire_control, true);
  } else {
    SetContextTouchPlacement(g_runtime.layout, owner.control.action, owner.control.center_x,
                             owner.control.center_y, owner.control.radius);
    ApplyContextTouchSavedLayout(g_runtime.layout);
  }
  std::erase_if(g_runtime.native_pulses, [pointer_id](const NativePulse& pulse) {
    return pulse.pointer_id == pointer_id;
  });
  if (owner.pinch_member) {
    ResetPinchLocked();
    g_runtime.completed_zoom = 0.0;
    g_runtime.zoom_quantizer.Reset();
  }
}

bool ScriptControlOwnedLocked(const ScriptKey& key) {
  if (key.kind == TouchScriptQueryKind::kAnalogueSticks) {
    return std::any_of(g_runtime.pointers.begin(), g_runtime.pointers.end(),
                       [&](const auto& item) {
      return !item.second.editing && IsStick(item.second.control) &&
             CanonicalScriptKey(item.second.control.script) == key;
    });
  }
  const auto it = g_runtime.script_refcounts.find(key);
  return it != g_runtime.script_refcounts.end() && it->second;
}

std::vector<TouchScriptControl> ActiveScriptControlsLocked(uint64_t epoch) {
  std::vector<TouchScriptControl> controls;
  const auto prompts = GetTouchVisiblePromptSnapshot(epoch, g_runtime.context.generation);
  for (auto it = g_runtime.script_availability.begin();
       it != g_runtime.script_availability.end();) {
    const auto& key = it->first;
    const bool owned = ScriptControlOwnedLocked(key);
    if (key.generation != g_runtime.context.generation ||
        (!owned && !EpochWithin(epoch, it->second.last_seen_epoch, kScriptQueryExpiryEpochs))) {
      g_runtime.script_pressed_epoch.erase(key);
      it = g_runtime.script_availability.erase(it);
      continue;
    }
    bool prompted = false;
    for (size_t i = 0; prompts.visible && i < prompts.control_count; ++i) {
      const auto prompt = CanonicalTouchScriptControl(prompts.controls[i]);
      prompted |= prompt.kind == key.kind && prompt.action == key.action;
    }
    if (owned || prompted) controls.push_back(ScriptControl(key));
    ++it;
  }
  std::sort(controls.begin(), controls.end(), [](const auto& left, const auto& right) {
    const bool left_owned = ScriptControlOwnedLocked(CanonicalScriptKey(left));
    const bool right_owned = ScriptControlOwnedLocked(CanonicalScriptKey(right));
    if (left_owned != right_owned) return left_owned;
    return std::tie(left.kind, left.action, left.input_group, left.script_thread, left.generation) <
           std::tie(right.kind, right.action, right.input_group, right.script_thread, right.generation);
  });
  return controls;
}

bool GameplayPresentationAdmittedLocked(const TouchContextSnapshot& context) {
  if (g_runtime.awaiting_gameplay_poll || g_runtime.presentation_transition_depth ||
      !context.valid || !context.native_input_allowed || context.loading || context.cutscene ||
      context.frontend || context.map || context.generation != g_runtime.context.generation ||
      context.presentation_revision != ContextTouchPresentationRevision() ||
      context.player_identity != g_runtime.context.player_identity ||
      context.input_user != g_runtime.context.input_user) return false;
  if (context.gameplay_allowed || context.minigame_active) return true;
  const bool parachute = g_runtime.parachute_generation == context.generation &&
      EpochWithin(g_runtime.epoch, g_runtime.parachute_last_seen_epoch, kScriptQueryExpiryEpochs) &&
      (g_runtime.parachute_state == kParachuteFreefallState ||
       g_runtime.parachute_state == kParachuteDeployedState);
  // Disabled player control alone is not an interactive minigame. Retain
  // only verified parachute or currently prompted script controls here.
  if (parachute) return true;
  const auto prompts = GetTouchVisiblePromptSnapshot(g_runtime.epoch, context.generation);
  if (!prompts.visible || !prompts.control_count) return false;
  for (const auto& control : ActiveScriptControlsLocked(g_runtime.epoch)) {
    const auto key = CanonicalTouchScriptControl(control);
    for (size_t index = 0; index < prompts.control_count; ++index) {
      const auto prompt = CanonicalTouchScriptControl(prompts.controls[index]);
      if (key.kind == prompt.kind && key.action == prompt.action) return true;
    }
  }
  return false;
}

ContextTouchViewport ReadViewport() {
  rex::input::TouchPresentationState p;
  if (!rex::input::GetTouchPresentationState(&p)) return {};
  const bool host = std::isfinite(p.logical_width) && std::isfinite(p.logical_height) &&
                    p.logical_width > 0.0f && p.logical_height > 0.0f;
  return {
      .output_width = host ? p.logical_width : p.output_width,
      .output_height = host ? p.logical_height : p.output_height,
      .physical_output_x = p.physical_output_x,
      .physical_output_y = p.physical_output_y,
      .physical_output_width = p.physical_output_width,
      .physical_output_height = p.physical_output_height,
      .physical_surface_width = p.physical_surface_width,
      .physical_surface_height = p.physical_surface_height,
      .safe_x = host ? p.logical_safe_x : static_cast<float>(p.safe_area_x),
      .safe_y = host ? p.logical_safe_y : static_cast<float>(p.safe_area_y),
      .safe_width = host ? p.logical_safe_width : static_cast<float>(p.safe_area_width),
      .safe_height = host ? p.logical_safe_height : static_cast<float>(p.safe_area_height),
      .generation = p.generation,
      .valid = p.valid,
      .focused = p.focused,
      .logical_width = p.logical_width,
      .logical_height = p.logical_height,
      .host_space = host,
  };
}

void RecalculateAxesLocked() {
  g_runtime.movement_x = g_runtime.movement_y = 0;
  g_runtime.right_x = g_runtime.right_y = 0;
  for (const auto& [id, owner] : g_runtime.pointers) {
    (void)id;
    if (owner.editing || !IsStick(owner.control)) continue;
    const bool left = owner.control.kind == ContextTouchControlKind::kMovementStick;
    const float center_x = left && g_runtime.preferences.floating_stick ? owner.start_x
                                                                       : owner.control.center_x;
    const float center_y = left && g_runtime.preferences.floating_stick ? owner.start_y
                                                                       : owner.control.center_y;
    float dx = owner.x - center_x;
    float dy = owner.y - center_y;
    const float distance = std::hypot(dx, dy);
    if (distance > owner.control.radius && owner.control.radius > 0.0f) {
      const float scale = owner.control.radius / distance;
      dx *= scale;
      dy *= scale;
    }
    const int32_t x = ContextTouchAxis(dx, owner.control.radius);
    const int32_t y = ContextTouchAxis(dy, owner.control.radius);
    if (left) {
      g_runtime.movement_x = x;
      g_runtime.movement_y = y;
    } else {
      g_runtime.right_x = x;
      g_runtime.right_y = y;
    }
  }
}

void RefreshLayoutLocked(uint64_t epoch, bool frontend, bool map) {
  const auto context = GetTouchContextSnapshot();
  const auto preferences = GetContextTouchPreferences();
  const auto viewport = ReadViewport();
  FinishEditorFadeLocked(MonotonicNanoseconds());
  if (g_runtime.editor_pending && viewport.valid && viewport.focused) {
    SetContextTouchEditorOpen(true);
    g_runtime.editor_pending = false;
  }
  if (g_runtime.editing && !ContextTouchEditorOpen()) BeginEditorFadeOutLocked();
  const bool editing = g_runtime.editor_session && ContextTouchEditorOpen() && !g_runtime.editor_closing;
  const bool context_changed = context.generation != g_runtime.context.generation ||
                               context.player_identity != g_runtime.context.player_identity ||
                               context.input_user != g_runtime.context.input_user;
  const bool native_ui_changed = frontend != g_runtime.context.frontend ||
                                 map != g_runtime.context.map;
  const bool geometry_changed = !ContextTouchLayoutEquivalent(
      {.viewport = g_runtime.layout.viewport}, {.viewport = viewport});
  if (context_changed || native_ui_changed || geometry_changed || editing != g_runtime.editing ||
      preferences.left_handed != g_runtime.preferences.left_handed ||
      preferences.floating_stick != g_runtime.preferences.floating_stick) {
    CancelAllLocked();
  }
  if (context_changed) {
    g_runtime.script_availability.clear();
    g_runtime.scoped_seen_epoch = 0;
    g_runtime.parachute_last_seen_epoch = 0;
  }
  g_runtime.context = context;
  g_runtime.preferences = preferences;
  g_runtime.editing = editing;
  g_runtime.epoch = epoch;
  const bool enabled = (editing || (!g_runtime.editor_session && !frontend && !map &&
                                   GameplayPresentationAdmittedLocked(context) &&
                                   rex::input::TouchControlsActive())) &&
                       viewport.valid && viewport.focused;
  ContextTouchMode mode = ContextTouchMode::kDisabled;
  if (enabled) {
    if (editing) {
      mode = context.valid ? context.base_mode : ContextTouchMode::kOnFoot;
      if (mode == ContextTouchMode::kDisabled || mode == ContextTouchMode::kFrontend ||
          mode == ContextTouchMode::kMap) mode = ContextTouchMode::kOnFoot;
    } else if (context.valid && context.native_input_allowed) {
      mode = context.base_mode;
      if (g_runtime.parachute_generation == context.generation &&
               EpochWithin(epoch, g_runtime.parachute_last_seen_epoch, kScriptQueryExpiryEpochs)) {
        if (g_runtime.parachute_state == kParachuteFreefallState)
          mode = ContextTouchMode::kParachuteFreefall;
        else if (g_runtime.parachute_state == kParachuteDeployedState)
          mode = ContextTouchMode::kParachuteDeployed;
      } else if (context.minigame_active || !context.gameplay_allowed) mode = ContextTouchMode::kMinigame;
    }
  }
  const bool scoped = !g_runtime.editor_session && context.gameplay_allowed && mode == ContextTouchMode::kOnFoot &&
      g_runtime.scoped_generation == context.generation &&
      EpochWithin(epoch, g_runtime.scoped_seen_epoch, kScopedObservationExpiryEpochs);
  if (!scoped && g_runtime.scoped_zoom) {
    ResetPinchLocked();
    g_runtime.completed_zoom = 0.0;
    g_runtime.zoom = 0;
    g_runtime.zoom_quantizer.Reset();
  }
  g_runtime.scoped_zoom = scoped;
  if (!context.inventory_known || !context.gameplay_allowed || mode != ContextTouchMode::kOnFoot) {
    g_runtime.weapon_wheel_open = false;
    g_runtime.weapon_target = kNoWeaponSelection;
  } else if (!g_runtime.editing) {
    for (auto& [id, owner] : g_runtime.pointers) {
      (void)id;
      if (owner.control.action == TouchAction::kWeaponWheel &&
          !owner.wheel_consumed && owner.held_seconds >= kWeaponWheelHoldSeconds) {
        g_runtime.weapon_wheel_open = true;
        owner.wheel_opened = true;
      }
    }
  }
  const auto scripts = context.native_input_allowed && !frontend && !map
                           ? ActiveScriptControlsLocked(epoch) : std::vector<TouchScriptControl>{};
  ContextTouchLayoutOptions options{
      .contextual = true,
      .phone_visible = context.phone_visible,
      .armed = editing || (context.weapon_known && context.armed),
      .aiming = context.aiming,
      .free_aim_available = editing || (context.aim_settings_known &&
          (context.alternate_aim_setting ? context.aim_threshold != 0
                                        : context.aim_threshold == 0 || context.aim_threshold > 11)),
      .in_cover = context.cover_known && context.in_cover,
      .melee = context.melee_known && context.melee,
      .scoped_zoom = scoped,
      .can_enter_vehicle = editing || context.can_enter_vehicle,
      .editing = editing,
      .weapon_wheel_open = g_runtime.weapon_wheel_open,
      .left_handed = preferences.left_handed,
      .button_scale = preferences.button_scale,
      .opacity = preferences.opacity,
  };
  if (context.weapon_hud_bounds) {
    options.weapon_hud_bounds = MapContextTouchHudBounds(*context.weapon_hud_bounds, viewport);
  }
  for (size_t i = 0; i < context.weapons.size(); ++i) {
    options.weapon_types[i] = context.weapons[i].type;
    options.weapon_selectable[i] = context.inventory_known && context.weapons[i].selectable;
    const auto& identifier = context.weapons[i].native_identifier;
    auto& name = options.weapon_names[i];
    for (size_t letter = 0; letter + 1 < name.size() && identifier[letter]; ++letter)
      name[letter] = identifier[letter] == '_' ? ' ' : identifier[letter];
  }
  auto next = BuildContextTouchLayout(mode, viewport, scripts, options);
  if (!g_runtime.editor_session && !GameplayPresentationAdmittedLocked(context)) HideGameplayLocked();
  ApplyContextTouchSavedLayout(next);
  ApplyContextTouchHudReservation(next, options.weapon_hud_bounds);
  if (mode != g_runtime.layout.mode) CancelAllLocked();
  for (auto it = g_runtime.pointers.begin(); it != g_runtime.pointers.end();) {
    auto& owner = it->second;
    size_t match = next.control_count;
    for (size_t index = 0; index < next.control_count; ++index) {
      if (SameControlIdentity(owner.control, next.controls[index])) {
        match = index;
        break;
      }
    }
    if (match == next.control_count) {
      CancelOwnerLocked(it->first, owner);
      it = g_runtime.pointers.erase(it);
      continue;
    }
    owner.control_index = match;
    // The origin and output mapping are copied at Down. Reordering or hiding
    // a still-legal semantic control cannot transfer its held finger.
    if (owner.composite_fire) {
      const bool legal = std::any_of(next.controls.begin(), next.controls.begin() + next.control_count,
                                    [&](const auto& c) { return SameControlIdentity(c, owner.fire_control); });
      if (!legal) {
        ReleaseControlLocked(owner.fire_control, true);
        owner.composite_fire = false;
      }
    }
    ++it;
  }
  if (!ContextTouchLayoutEquivalent(g_runtime.layout, next)) {
    ++g_runtime.layout_generation;
    if (REXCVAR_GET(gta4_touch_trace)) {
      REXLOG_INFO("gta4-touch: layout epoch={} mode={} generation={} controls={} host-space={}",
                  epoch, static_cast<uint32_t>(mode), g_runtime.layout_generation,
                  next.control_count, viewport.host_space);
    }
  }
  g_runtime.layout = std::move(next);
  RecalculateAxesLocked();
}

bool PointInside(const ContextTouchControl& control, float x, float y) {
  return ContextTouchControlContains(control, g_runtime.layout.viewport, x, y);
}

size_t FindControlLocked(float x, float y) {
  const auto& layout = g_runtime.layout;
  for (size_t i = 0; i < layout.control_count; ++i) {
    const auto& c = layout.controls[i];
    if (c.visible && c.kind != ContextTouchControlKind::kLookSurface && PointInside(c, x, y)) return i;
  }
  const auto& v = layout.viewport;
  if (!g_runtime.editing && g_runtime.preferences.floating_stick &&
      x >= v.safe_x && x <= v.safe_x + v.safe_width &&
      y >= v.safe_y + v.safe_height * 0.35f && y <= v.safe_y + v.safe_height) {
    const float middle = v.safe_x + v.safe_width * 0.5f;
    const bool movement_side = g_runtime.preferences.left_handed ? x >= middle : x <= middle;
    for (size_t i = 0; movement_side && i < layout.control_count; ++i) {
      if (layout.controls[i].visible &&
          layout.controls[i].kind == ContextTouchControlKind::kMovementStick) return i;
    }
  }
  for (size_t i = 0; i < layout.control_count; ++i) {
    if (layout.controls[i].visible &&
        layout.controls[i].kind == ContextTouchControlKind::kLookSurface &&
        PointInside(layout.controls[i], x, y)) return i;
  }
  return layout.control_count;
}

bool ControlAlreadyOwnedLocked(const ContextTouchControl& control) {
  if (g_runtime.editing) return false;
  if (control.kind == ContextTouchControlKind::kLookSurface && g_runtime.scoped_zoom) {
    size_t cameras = 0;
    bool tail = false;
    for (const auto& [id, owner] : g_runtime.pointers) {
      (void)id;
      cameras += owner.control.kind == ContextTouchControlKind::kLookSurface;
      tail |= owner.pinch_member;
    }
    return cameras >= 2 || tail;
  }
  if (!IsStick(control) && control.kind != ContextTouchControlKind::kLookSurface) return false;
  return std::any_of(g_runtime.pointers.begin(), g_runtime.pointers.end(), [&](const auto& item) {
    return item.second.control.kind == control.kind;
  });
}

void PressControlLocked(const ContextTouchControl& control, uint64_t epoch) {
  if (control.kind == ContextTouchControlKind::kButton) {
    g_runtime.key_latch.Press(control.key, epoch);
  } else if (control.kind == ContextTouchControlKind::kScriptButton) {
    const auto key = CanonicalScriptKey(control.script);
    uint16_t& count = g_runtime.script_refcounts[key];
    if (!count) g_runtime.script_pressed_epoch[key] = epoch;
    if (count != std::numeric_limits<uint16_t>::max()) ++count;
  }
}

void TryStartPinchLocked(uint64_t second_id) {
  auto& second = g_runtime.pointers.at(second_id);
  if ((!g_runtime.scoped_zoom && !g_runtime.editing) || g_runtime.pinch_active) return;
  for (auto& [id, first] : g_runtime.pointers) {
    if (id == second_id || first.pinch_member) continue;
    const bool camera_pair = !g_runtime.editing &&
        first.control.kind == ContextTouchControlKind::kLookSurface &&
        second.control.kind == ContextTouchControlKind::kLookSurface;
    const bool editor_pair = g_runtime.editing && first.editing && second.editing &&
        SameControlIdentity(first.control, second.control) && IsStick(first.control) == IsStick(second.control);
    if (!camera_pair && !editor_pair) continue;
    const double distance = std::hypot(double(second.x) - first.x, double(second.y) - first.y);
    if (distance <= 0.0) return;
    first.pinch_member = second.pinch_member = true;
    first.look_dx = first.look_dy = second.look_dx = second.look_dy = 0.0;
    g_runtime.pinch_active = true;
    g_runtime.pinch_first = id;
    g_runtime.pinch_second = second_id;
    g_runtime.pinch_distance = distance;
    g_runtime.zoom_delta = 0.0;
    g_runtime.editor_pinch_radius = first.control.radius;
    g_runtime.editor_pinch_center_x = std::midpoint(first.x, second.x);
    g_runtime.editor_pinch_center_y = std::midpoint(first.y, second.y);
    return;
  }
}

void UpdateEditorPlacementLocked(PointerOwner& owner, float x, float y, float radius) {
  if (SetContextTouchPlacement(g_runtime.layout, owner.control.action, x, y, radius)) {
    ApplyContextTouchSavedLayout(g_runtime.layout);
  }
}

void UpdatePinchLocked() {
  if (!g_runtime.pinch_active) return;
  auto first = g_runtime.pointers.find(g_runtime.pinch_first);
  auto second = g_runtime.pointers.find(g_runtime.pinch_second);
  if (first == g_runtime.pointers.end() || second == g_runtime.pointers.end()) {
    ResetPinchLocked();
    return;
  }
  const double distance = std::hypot(double(second->second.x) - first->second.x,
                                     double(second->second.y) - first->second.y);
  if (g_runtime.editing) {
    auto& owner = first->second;
    const float radius = g_runtime.editor_pinch_radius * static_cast<float>(distance / g_runtime.pinch_distance);
    const float x = owner.control.center_x + std::midpoint(first->second.x, second->second.x) -
                    g_runtime.editor_pinch_center_x;
    const float y = owner.control.center_y + std::midpoint(first->second.y, second->second.y) -
                    g_runtime.editor_pinch_center_y;
    UpdateEditorPlacementLocked(owner, x, y, radius);
  } else {
    // The native signed decoder's negative direction decreases scope FOV.
    g_runtime.zoom_delta += g_runtime.pinch_distance - distance;
    g_runtime.pinch_distance = distance;
  }
}

void UpdateCompositeLocked(PointerOwner& owner, uint64_t epoch) {
  if (owner.control.action != TouchAction::kAccelerate) return;
  const ContextTouchControl* fire = nullptr;
  for (size_t i = 0; i < g_runtime.layout.control_count; ++i) {
    const auto& c = g_runtime.layout.controls[i];
    if (c.visible && c.action == TouchAction::kVehicleFire && PointInside(c, owner.x, owner.y)) {
      fire = &c;
      break;
    }
  }
  if (fire && !owner.composite_fire) {
    owner.composite_fire = true;
    owner.fire_control = *fire;
    PressControlLocked(*fire, epoch);
  } else if (!fire && owner.composite_fire) {
    // Departure ends only the Fire part of this gesture, including an
    // unconsumed key edge. The Throttle origin continues to be held.
    ReleaseControlLocked(owner.fire_control, true);
    owner.composite_fire = false;
  }
}

void UpdateOwnerPositionLocked(PointerOwner& owner, float x, float y, uint64_t epoch) {
  if (!owner.editing && IsCamera(owner.control) && !owner.pinch_member) {
    owner.look_dx += double(x) - owner.x;
    owner.look_dy += double(y) - owner.y;
  }
  owner.x = x;
  owner.y = y;
  if (owner.editing) {
    if (!owner.pinch_member) {
      UpdateEditorPlacementLocked(owner, owner.control.center_x + x - owner.start_x,
                                   owner.control.center_y + y - owner.start_y, owner.control.radius);
    }
  } else {
    UpdateCompositeLocked(owner, epoch);
  }
  if (owner.pinch_member) UpdatePinchLocked();
}

bool IsHeldUtility(TouchAction action) {
  return action == TouchAction::kZoomIn || action == TouchAction::kZoomOut ||
         action == TouchAction::kWeaponWheel || action == TouchAction::kWeaponSelect;
}

void SelectWeaponLocked(uint32_t slot) {
  if (g_runtime.context.inventory_known && slot < g_runtime.context.weapons.size() &&
      g_runtime.context.weapons[slot].selectable) {
    g_runtime.weapon_target = slot;
    g_runtime.weapon_last_slot = kNoWeaponSelection;
    g_runtime.weapon_cycle_epoch = 0;
    g_runtime.weapon_cycle_attempts = 0;
  }
  for (auto& [id, owner] : g_runtime.pointers) {
    (void)id;
    if (owner.control.action == TouchAction::kWeaponWheel) owner.wheel_consumed = true;
  }
  g_runtime.weapon_wheel_open = false;
}

void ReleaseWeaponWheelLocked(const PointerOwner& owner, uint64_t pointer_id, uint64_t epoch) {
  if (owner.control.action == TouchAction::kWeaponSelect) {
    SelectWeaponLocked(owner.control.weapon_slot);
    return;
  }
  if (owner.wheel_consumed) return;
  if (!g_runtime.weapon_wheel_open) {
    if (owner.wheel_opened) return;
    ContextTouchControl cycle;
    cycle.action = TouchAction::kWeaponNext;
    cycle.kind = ContextTouchControlKind::kNativeButton;
    cycle.pad_buttons = rex::input::X_INPUT_GAMEPAD_DPAD_RIGHT;
    g_runtime.native_pulses.push_back({pointer_id, epoch, cycle});
    return;
  }
  for (size_t i = 0; i < g_runtime.layout.control_count; ++i) {
    const auto& control = g_runtime.layout.controls[i];
    if (control.visible && control.action == TouchAction::kWeaponSelect &&
        PointInside(control, owner.x, owner.y)) {
      SelectWeaponLocked(control.weapon_slot);
      return;
    }
  }
  g_runtime.weapon_wheel_open = false;
}

bool OnPointerEvent(const rex::input::AbsolutePointerEvent& event, PPCContext&, uint8_t*,
                    uint64_t epoch) {
  if (!rex::input::TouchPointerInputActive() ||
      (!rex::input::TouchControlsActive() && !ContextTouchEditorOpen())) return false;
  std::lock_guard lock(g_runtime.mutex);
  if (g_runtime.epoch != epoch || g_runtime.frozen_epoch == epoch) return false;
  if (!g_runtime.editing && !GameplayPresentationAdmittedLocked(GetTouchContextSnapshot())) {
    if (!g_runtime.editor_session) HideGameplayLocked();
    return false;
  }
  if (g_runtime.layout.mode == ContextTouchMode::kDisabled) {
    CancelAllLocked();
    return false;
  }
  if (event.generation != g_runtime.layout.viewport.generation) return false;
  const float x = g_runtime.layout.viewport.host_space ? event.logical_x : event.x;
  const float y = g_runtime.layout.viewport.host_space ? event.logical_y : event.y;
  const bool finite = std::isfinite(x) && std::isfinite(y);
  auto pointer = g_runtime.pointers.find(event.pointer_id);
  if (pointer != g_runtime.pointers.end() && !g_runtime.weapon_wheel_open &&
      pointer->second.control.action == TouchAction::kWeaponWheel &&
      !pointer->second.wheel_consumed &&
      g_runtime.context.inventory_known &&
      event.timestamp_ns >= pointer->second.down_timestamp_ns &&
      event.timestamp_ns - pointer->second.down_timestamp_ns >= kWeaponWheelHoldNanoseconds) {
    g_runtime.weapon_wheel_open = true;
    pointer->second.wheel_opened = true;
    RefreshLayoutLocked(epoch, g_runtime.context.frontend, g_runtime.context.map);
    pointer = g_runtime.pointers.find(event.pointer_id);
  }
  if (event.phase == rex::input::AbsolutePointerPhase::kDown) {
    if (pointer != g_runtime.pointers.end()) {
      CancelOwnerLocked(pointer->first, pointer->second);
      g_runtime.pointers.erase(pointer);
      RecalculateAxesLocked();
    }
    if (!finite) return false;
    const size_t index = FindControlLocked(x, y);
    if (index >= g_runtime.layout.control_count) return false;
    const auto control = g_runtime.layout.controls[index];
    if (control.kind == ContextTouchControlKind::kUtility && !IsHeldUtility(control.action)) {
      CancelAllLocked();
      HandleContextTouchEditorAction(control.action, g_runtime.layout);
      RefreshLayoutLocked(epoch, g_runtime.context.frontend, g_runtime.context.map);
      return true;
    }
    if (ControlAlreadyOwnedLocked(control) ||
        (g_runtime.editing && control.kind == ContextTouchControlKind::kLookSurface)) return false;
    PointerOwner owner{.control_index = index, .control = control, .down_epoch = epoch,
                       .down_timestamp_ns = event.timestamp_ns,
                       .start_x = x, .start_y = y, .x = x, .y = y,
                       .editing = g_runtime.editing};
    g_runtime.pointers.emplace(event.pointer_id, owner);
    if (!owner.editing) {
      PressControlLocked(control, epoch);
      if (IsNative(control) || control.action == TouchAction::kZoomIn ||
          control.action == TouchAction::kZoomOut)
        g_runtime.native_pulses.push_back({event.pointer_id, epoch, control});
    }
    TryStartPinchLocked(event.pointer_id);
    RecalculateAxesLocked();
    return true;
  }
  if (pointer == g_runtime.pointers.end()) return false;
  auto& owner = pointer->second;
  const bool cancelled = event.phase == rex::input::AbsolutePointerPhase::kCancel || !finite;
  if (!cancelled) UpdateOwnerPositionLocked(owner, x, y, epoch);
  if (event.phase == rex::input::AbsolutePointerPhase::kMove && !cancelled) {
    RecalculateAxesLocked();
    return true;
  }
  if (cancelled) {
    CancelOwnerLocked(pointer->first, owner);
    if (owner.control.action == TouchAction::kWeaponWheel ||
        owner.control.action == TouchAction::kWeaponSelect) g_runtime.weapon_wheel_open = false;
  } else {
    if (!owner.editing) {
      ReleaseControlLocked(owner.control, false);
      if (owner.composite_fire) ReleaseControlLocked(owner.fire_control, true);
      if (IsCamera(owner.control) && !owner.pinch_member) {
        g_runtime.completed_look_x += owner.look_dx;
        g_runtime.completed_look_y += owner.look_dy;
      }
      if (owner.control.action == TouchAction::kWeaponWheel ||
          owner.control.action == TouchAction::kWeaponSelect) {
        ReleaseWeaponWheelLocked(owner, pointer->first, epoch);
      }
    }
    if (owner.pinch_member) {
      g_runtime.completed_zoom += g_runtime.zoom_delta;
      ResetPinchLocked();
    }
  }
  g_runtime.pointers.erase(pointer);
  RecalculateAxesLocked();
  if (!g_runtime.weapon_wheel_open && (g_runtime.weapon_target != kNoWeaponSelection ||
      std::any_of(g_runtime.layout.controls.begin(),
                  g_runtime.layout.controls.begin() + g_runtime.layout.control_count,
                  [](const auto& c) { return c.action == TouchAction::kWeaponSelect; }))) {
    RefreshLayoutLocked(epoch, g_runtime.context.frontend, g_runtime.context.map);
  }
  return true;
}

void BeginPoll(PPCContext&, uint8_t* base, uint64_t epoch, bool frontend, bool map) {
  std::lock_guard lock(g_runtime.mutex);
  const bool new_poll = g_runtime.epoch != epoch;
  if (new_poll) {
    if (!g_runtime.presentation_transition_depth &&
        GetTouchContextSnapshot().presentation_revision == ContextTouchPresentationRevision())
      g_runtime.awaiting_gameplay_poll = false;
    g_runtime.frozen_epoch = 0;
    g_runtime.look_x = g_runtime.look_y = g_runtime.zoom = 0;
    std::erase_if(g_runtime.native_pulses, [epoch](const NativePulse& p) { return p.epoch != epoch; });
  }
  g_runtime.frame_seconds = base ? std::bit_cast<float>(LoadU32(base, kGameplayTimeStepAddress))
                                : kReferenceFrameSeconds;
  if (!std::isfinite(g_runtime.frame_seconds) || g_runtime.frame_seconds <= 0.0)
    g_runtime.frame_seconds = kReferenceFrameSeconds;
  if (new_poll) {
    for (auto& [id, owner] : g_runtime.pointers) {
      (void)id;
      owner.held_seconds += g_runtime.frame_seconds;
    }
  }
  RefreshLayoutLocked(epoch, frontend, map);
}

int16_t NativeAxis(int32_t value) {
  value = std::clamp(value, -255, 255);
  const double limit = value < 0 ? 32768.0 : 32767.0;
  return static_cast<int16_t>(std::lround(static_cast<double>(value) * limit / 255.0));
}

uint8_t NativeTriggerValue(const ContextTouchControl& control) {
  if (control.action == TouchAction::kFreeAim && !g_runtime.context.aim_settings_known) return 0;
  if ((control.action == TouchAction::kAim || control.action == TouchAction::kFreeAim) &&
      g_runtime.context.aim_settings_known) {
    // sub_823C4728 admits lock-on before sub_823C5EE0 acquires/sets a target.
    // sub_823C4510 admits the complementary free-aim range. The alternate
    // setting reverses these ranges; an empty native range stays inactive.
    const int threshold = g_runtime.context.aim_threshold;
    const bool alternate = g_runtime.context.alternate_aim_setting;
    const bool lock_on = control.action == TouchAction::kAim;
    if (lock_on) return alternate ? static_cast<uint8_t>(threshold) : uint8_t{255};
    const int upper = (threshold + 255) & 255;
    if (alternate) return upper < 255 ? uint8_t{255} : uint8_t{0};
    return upper > 10 ? static_cast<uint8_t>(upper) : uint8_t{0};
  }
  return control.trigger_value;
}

void MergeNativeControl(TouchNativePadState& pad, const ContextTouchControl& control) {
  if (control.kind == ContextTouchControlKind::kNativeButton) pad.buttons |= control.pad_buttons;
  if (control.kind == ContextTouchControlKind::kNativeTrigger) {
    const uint8_t value = NativeTriggerValue(control);
    if (control.trigger_side == 1) pad.left_trigger = std::max(pad.left_trigger, value);
    else if (control.trigger_side == 2) pad.right_trigger = std::max(pad.right_trigger, value);
  }
}

double CameraSensitivityLocked() {
  for (const auto& [id, owner] : g_runtime.pointers) {
    (void)id;
    if (owner.control.action == TouchAction::kAim || owner.control.action == TouchAction::kFreeAim)
      return g_runtime.preferences.aim_sensitivity;
  }
  if (g_runtime.context.aiming || g_runtime.scoped_zoom) return g_runtime.preferences.aim_sensitivity;
  if (g_runtime.layout.mode == ContextTouchMode::kVehicleHelicopter || IsParachute(g_runtime.layout.mode))
    return g_runtime.preferences.flight_sensitivity;
  if (IsVehicle(g_runtime.layout.mode)) return g_runtime.preferences.vehicle_sensitivity;
  return g_runtime.preferences.camera_sensitivity;
}

void MergeWeaponSelectionLocked(TouchNativePadState& pad, uint64_t epoch) {
  const auto target = g_runtime.weapon_target;
  const auto& context = g_runtime.context;
  if (target == kNoWeaponSelection) return;
  if (!context.gameplay_allowed || !context.inventory_known ||
      target >= context.weapons.size() || !context.weapons[target].selectable ||
      context.weapon_slot >= context.weapons.size() || context.weapon_slot == target) {
    g_runtime.weapon_target = kNoWeaponSelection;
    return;
  }
  if (g_runtime.weapon_cycle_epoch) {
    if (epoch <= g_runtime.weapon_cycle_epoch || epoch - g_runtime.weapon_cycle_epoch <= 1) return;
    if (context.weapon_slot == g_runtime.weapon_last_slot) {
      if (epoch - g_runtime.weapon_cycle_epoch < kWeaponCycleRetryEpochs) return;
      if (g_runtime.weapon_cycle_attempts >= 3) {
        g_runtime.weapon_target = kNoWeaponSelection;
        return;
      }
    } else {
      g_runtime.weapon_cycle_attempts = 0;
    }
  }
  size_t forward = 0, backward = 0;
  const size_t count = context.weapons.size();
  for (size_t distance = 1; distance < count; ++distance) {
    const size_t slot = (context.weapon_slot + distance) % count;
    forward += context.weapons[slot].selectable;
    if (slot == target) break;
  }
  for (size_t distance = 1; distance < count; ++distance) {
    const size_t slot = (context.weapon_slot + count - distance) % count;
    backward += context.weapons[slot].selectable;
    if (slot == target) break;
  }
  pad.buttons |= forward <= backward ? rex::input::X_INPUT_GAMEPAD_DPAD_RIGHT
                                    : rex::input::X_INPUT_GAMEPAD_DPAD_LEFT;
  g_runtime.weapon_last_slot = context.weapon_slot;
  g_runtime.weapon_cycle_epoch = epoch;
  ++g_runtime.weapon_cycle_attempts;
}

void FreezeLocked(uint64_t epoch) {
  if (g_runtime.frozen_epoch == epoch) return;
  g_runtime.frozen_epoch = epoch;
  double dx = std::exchange(g_runtime.completed_look_x, 0.0);
  double dy = std::exchange(g_runtime.completed_look_y, 0.0);
  for (auto& [id, owner] : g_runtime.pointers) {
    (void)id;
    dx += std::exchange(owner.look_dx, 0.0);
    dy += std::exchange(owner.look_dy, 0.0);
  }
  const double sensitivity = CameraSensitivityLocked();
  if (g_runtime.preferences.invert_camera_y) dy = -dy;
  g_runtime.look_x = g_runtime.look_quantizer_x.Quantize(dx, sensitivity, kLookUnitsPerPixel,
      g_runtime.frame_seconds, kReferenceFrameSeconds);
  g_runtime.look_y = g_runtime.look_quantizer_y.Quantize(dy, sensitivity, kLookUnitsPerPixel,
      g_runtime.frame_seconds, kReferenceFrameSeconds);
  if (IsParachute(g_runtime.layout.mode) &&
      std::none_of(g_runtime.pointers.begin(), g_runtime.pointers.end(), [](const auto& item) {
        return item.second.control.kind == ContextTouchControlKind::kRightStick;
      })) {
    g_runtime.right_x = g_runtime.look_x;
    g_runtime.right_y = g_runtime.look_y;
  }
  const double zoom = std::exchange(g_runtime.completed_zoom, 0.0) +
                       std::exchange(g_runtime.zoom_delta, 0.0);
  g_runtime.zoom = g_runtime.scoped_zoom ? g_runtime.zoom_quantizer.Quantize(
      zoom, 1.0, kPinchUnitsPerPixel, g_runtime.frame_seconds, kReferenceFrameSeconds) : 0;
  TouchNativePadState pad{.epoch = epoch};
  pad.active = !g_runtime.editor_session && g_runtime.context.input_user_known &&
      g_runtime.layout.mode != ContextTouchMode::kDisabled &&
      g_runtime.context.native_input_allowed &&
      !g_runtime.context.frontend && !g_runtime.context.map;
  if (pad.active) {
    for (const auto& [id, owner] : g_runtime.pointers) {
      (void)id;
      if (owner.editing) continue;
      MergeNativeControl(pad, owner.control);
      if (owner.composite_fire) MergeNativeControl(pad, owner.fire_control);
      if (g_runtime.scoped_zoom && owner.control.action == TouchAction::kZoomIn) g_runtime.zoom = -255;
      if (g_runtime.scoped_zoom && owner.control.action == TouchAction::kZoomOut) g_runtime.zoom = 255;
    }
    for (const auto& pulse : g_runtime.native_pulses) {
      if (pulse.epoch == epoch) {
        MergeNativeControl(pad, pulse.control);
        if (g_runtime.scoped_zoom && pulse.control.action == TouchAction::kZoomIn) g_runtime.zoom = -255;
        if (g_runtime.scoped_zoom && pulse.control.action == TouchAction::kZoomOut) g_runtime.zoom = 255;
      }
    }
    pad.left_x = NativeAxis(g_runtime.movement_x);
    pad.left_y = NativeAxis(-g_runtime.movement_y);
    pad.right_x = NativeAxis(g_runtime.right_x);
    pad.right_y = NativeAxis(-g_runtime.right_y);
    MergeWeaponSelectionLocked(pad, epoch);
  }
  g_runtime.native_pad = pad;
}

void CollectVirtualKeys(uint64_t epoch, std::array<uint8_t, 256>& down,
                        std::array<uint8_t, 256>& pressed) {
  std::lock_guard lock(g_runtime.mutex);
  if (g_runtime.epoch != epoch) return;
  if (!g_runtime.editor_session && !GameplayPresentationAdmittedLocked(GetTouchContextSnapshot())) {
    HideGameplayLocked();
    return;
  }
  FreezeLocked(epoch);
  // Parachute keys are private aliases for the verified SCO query merge.
  // Publishing them as keyboard keys would additionally translate LButton,
  // RButton, F and Control into unrelated native pad/action bindings.
  if (!g_runtime.editor_session && !IsParachute(g_runtime.layout.mode))
    g_runtime.key_latch.Collect(epoch, down, pressed);
}

void OnControlsDisabled(PPCContext&, uint8_t*, uint64_t epoch) {
  std::lock_guard lock(g_runtime.mutex);
  g_runtime.epoch = epoch;
  g_runtime.frozen_epoch = epoch;
  CancelAllLocked();
  g_runtime.layout = {};
  g_runtime.script_availability.clear();
  g_runtime.scoped_seen_epoch = 0;
  g_runtime.parachute_last_seen_epoch = 0;
  ++g_runtime.layout_generation;
}

uint32_t ActionAddress(uint32_t control, Action action) {
  return control + kActionArrayOffset + static_cast<uint32_t>(action) * kActionStride;
}

bool MergeAxis(uint8_t* base, uint32_t control, Action negative, Action positive, int32_t requested) {
  if (!requested) return false;
  const uint32_t a = ActionAddress(control, negative);
  const uint32_t b = ActionAddress(control, positive);
  const auto merge = rex::input::mnk::MergeSignedActionPair(
      LoadU8(base, a), LoadU8(base, a + kActionCurrentOffset),
      LoadU8(base, b), LoadU8(base, b + kActionCurrentOffset), requested);
  if (merge.changed) {
    StoreU8(base, a + kActionCurrentOffset, merge.negative_encoded);
    StoreU8(base, b + kActionCurrentOffset, merge.positive_encoded);
  }
  return merge.changed;
}

void OnControlReplay(PPCContext&, uint8_t* base, uint32_t control, uint32_t, uint64_t epoch) {
  if (!rex::input::TouchControlsActive()) return;
  std::lock_guard lock(g_runtime.mutex);
  if (g_runtime.epoch != epoch || g_runtime.frozen_epoch != epoch || !base || !control ||
      !g_runtime.context.gameplay_allowed || !g_runtime.context.input_user_known || g_runtime.editor_session ||
      !GameplayPresentationAdmittedLocked(GetTouchContextSnapshot()) ||
      control > std::numeric_limits<uint32_t>::max() - kLastInputTimeOffset - sizeof(uint32_t) ||
      LoadU32(base, control + kControlUserIndexOffset) != g_runtime.context.input_user) return;
  const auto mode = g_runtime.layout.mode;
  bool changed = false;
  if (mode == ContextTouchMode::kOnFoot || mode == ContextTouchMode::kPhone) {
    changed |= MergeAxis(base, control, Action::kMoveLeft, Action::kMoveRight, g_runtime.movement_x);
    changed |= MergeAxis(base, control, Action::kMoveUp, Action::kMoveDown, g_runtime.movement_y);
    changed |= MergeAxis(base, control, Action::kLookLeft, Action::kLookRight, g_runtime.look_x);
    changed |= MergeAxis(base, control, Action::kLookUp, Action::kLookDown, g_runtime.look_y);
  } else if (IsVehicle(mode)) {
    if (mode == ContextTouchMode::kVehicleAutomobile || mode == ContextTouchMode::kVehicleBoat ||
        mode == ContextTouchMode::kVehicleBike || mode == ContextTouchMode::kVehicleHelicopter) {
      changed |= MergeAxis(base, control, Action::kVehicleMoveLeft, Action::kVehicleMoveRight,
                           g_runtime.movement_x);
    }
    if (mode == ContextTouchMode::kVehicleBike || mode == ContextTouchMode::kVehicleHelicopter) {
      changed |= MergeAxis(base, control, Action::kVehicleMoveUp, Action::kVehicleMoveDown,
                           g_runtime.movement_y);
    }
    changed |= MergeAxis(base, control, Action::kVehicleGunLeft, Action::kVehicleGunRight, g_runtime.look_x);
    changed |= MergeAxis(base, control, Action::kVehicleGunUp, Action::kVehicleGunDown, g_runtime.look_y);
    changed |= MergeAxis(base, control, Action::kVehicleLookLeft, Action::kVehicleLookRight, g_runtime.look_x);
  }
  if (changed) StoreU32(base, control + kLastInputTimeOffset, LoadU32(base, kGameInputTimeAddress));
}

bool NativePadProvider(uint32_t user, rex::input::X_INPUT_GAMEPAD* output) noexcept {
  if (!output) return false;
  const bool owned = GTA4_TouchTitleInputOwned();
  const bool active = rex::input::TouchControlsActive();
  std::lock_guard lock(g_runtime.mutex);
  const auto& pad = g_runtime.native_pad;
  if (!g_runtime.initialized || !g_runtime.context.valid || !g_runtime.context.input_user_known ||
      user != g_runtime.context.input_user) return false;
  *output = {};
  if (owned || !active || !pad.active || pad.epoch != g_runtime.epoch ||
      !GameplayPresentationAdmittedLocked(GetTouchContextSnapshot())) return true;
  output->buttons = pad.buttons;
  output->left_trigger = pad.left_trigger;
  output->right_trigger = pad.right_trigger;
  output->thumb_lx = pad.left_x;
  output->thumb_ly = pad.left_y;
  output->thumb_rx = pad.right_x;
  output->thumb_ry = pad.right_y;
  return true;
}

bool ScriptQueryAllowedLocked(uint64_t epoch, uint64_t generation) {
  return epoch == g_runtime.epoch && epoch == g_runtime.frozen_epoch &&
      generation == g_runtime.context.generation && !g_runtime.editor_session &&
      g_runtime.context.native_input_allowed &&
      GameplayPresentationAdmittedLocked(GetTouchContextSnapshot()) &&
      g_runtime.layout.mode != ContextTouchMode::kDisabled &&
      !g_runtime.context.frontend && !g_runtime.context.map;
}

uint32_t ParachuteQueryValueLocked(TouchScriptQueryKind kind, uint32_t action,
                                   uint64_t epoch, uint32_t script_thread) {
  if (!script_thread || script_thread != g_runtime.parachute_thread ||
      g_runtime.parachute_generation != g_runtime.context.generation ||
      !EpochWithin(epoch, g_runtime.parachute_last_seen_epoch, kScriptQueryExpiryEpochs) ||
      !IsParachute(g_runtime.layout.mode)) return 0;
  std::array<uint8_t, 256> down{}, pressed{};
  g_runtime.key_latch.Collect(epoch, down, pressed);
  using V = rex::ui::VirtualKey;
  const auto held = [&](V key) { return down[static_cast<uint16_t>(key)] != 0; };
  const auto edge = [&](V key) { return pressed[static_cast<uint16_t>(key)] != 0; };
  const bool deployed = g_runtime.parachute_state == kParachuteDeployedState;
  bool requested = false;
  // These are the executable parachute_player SCO aliases already used by
  // ForceParachuteControlResult/RawButtonResult in gta4_input_hooks.cpp.
  if (kind == TouchScriptQueryKind::kControlPressed) {
    requested = (!deployed && (action == 1 || action == 137) && edge(V::kLButton)) ||
                (deployed && action == 3 && edge(V::kF));
  } else if (deployed && (kind == TouchScriptQueryKind::kControlHeld ||
                          kind == TouchScriptQueryKind::kControlAnalog)) {
    requested = ((action == 4 || action == 138) && held(V::kLButton)) ||
                ((action == 6 || action == 137) && held(V::kRButton)) ||
                (action == 51 && held(V::kControl));
  } else if (deployed && action == 17) {
    if (kind == TouchScriptQueryKind::kRawButton) requested = held(V::kControl);
    if (kind == TouchScriptQueryKind::kRawButtonPressed) requested = edge(V::kControl);
  }
  return requested ? (kind == TouchScriptQueryKind::kControlAnalog ? 255 : 1) : 0;
}

}  // namespace

void InitializeContextTouchControls() noexcept {
  {
    std::lock_guard lock(g_runtime.mutex);
    if (g_runtime.initialized) return;
    g_runtime.initialized = true;
  }
  rex::input::SetTouchGamepadProvider(&NativePadProvider);
  GTA4_RegisterTouchExtension({.begin_poll = &BeginPoll, .on_pointer_event = &OnPointerEvent,
      .collect_virtual_keys = &CollectVirtualKeys, .on_controls_disabled = &OnControlsDisabled,
      .on_control_replay = &OnControlReplay});
}

void ShutdownContextTouchControls() noexcept {
  rex::input::SetTouchGamepadProvider(nullptr);
  rex::input::mnk::PublishVirtualControllerCompatibilityKeys(0, {}, false);
  GTA4_RegisterTouchExtension({});
  std::lock_guard lock(g_runtime.mutex);
  CancelAllLocked();
  g_runtime.layout = {};
  g_runtime.script_availability.clear();
  g_runtime.parachute_last_seen_epoch = 0;
  g_runtime.scoped_seen_epoch = 0;
  g_runtime.editor_pending = false;
  g_runtime.editing = false;
  g_runtime.editor_session = false;
  g_runtime.editor_closing = false;
  g_runtime.editor_close_started_ns = 0;
  g_runtime.last_drawable_ns = 0;
  SetContextTouchEditorOpen(false);
  g_runtime.fade.Reset();
  g_runtime.last_drawable = {};
  g_runtime.initialized = false;
}

void RequestContextTouchEditor() noexcept {
  std::lock_guard lock(g_runtime.mutex);
  CancelAllLocked();
  if (!g_runtime.editor_session) {
    g_runtime.fade.Reset();
    g_runtime.last_drawable = {};
    g_runtime.last_drawable_ns = 0;
  }
  g_runtime.editor_session = true;
  g_runtime.editor_closing = false;
  g_runtime.editor_pending = true;
}

void SuspendContextTouchGameplay() noexcept {
  {
    std::lock_guard lock(g_runtime.mutex);
    SuspendGameplayLocked();
  }
  GTA4_CancelTouchGameplayReplay();
}

uint64_t ContextTouchPresentationRevision() noexcept {
  return g_presentation_revision.load(std::memory_order_acquire);
}

bool ContextTouchGameplayInputAdmitted() noexcept {
  const auto context = GetTouchContextSnapshot();
  std::lock_guard lock(g_runtime.mutex);
  return g_runtime.initialized && !g_runtime.editor_session &&
      GameplayPresentationAdmittedLocked(context);
}

bool ContextTouchWeaponCycleAdmitted() noexcept {
  if (GTA4_TouchTitleInputOwned() || !rex::input::TouchPointerInputActive() ||
      !rex::input::TouchControlsActive()) return false;
  const auto context = GetTouchContextSnapshot();
  std::lock_guard lock(g_runtime.mutex);
  return g_runtime.initialized && !g_runtime.editor_session &&
      !g_runtime.weapon_wheel_open && !context.phone_visible && context.gameplay_allowed &&
      GameplayPresentationAdmittedLocked(context) && WeaponCycleButton(g_runtime.layout.mode) != 0;
}

bool QueueContextTouchWeaponCycle(uint64_t epoch, uint64_t pointer_id) noexcept {
  if (GTA4_TouchTitleInputOwned() || !rex::input::TouchPointerInputActive() ||
      !rex::input::TouchControlsActive()) return false;
  const auto context = GetTouchContextSnapshot();
  std::lock_guard lock(g_runtime.mutex);
  const uint16_t button = WeaponCycleButton(g_runtime.layout.mode);
  if (!g_runtime.initialized || g_runtime.editor_session || g_runtime.weapon_wheel_open ||
      context.phone_visible || !context.gameplay_allowed || !button ||
      !GameplayPresentationAdmittedLocked(context) || g_runtime.epoch != epoch ||
      g_runtime.frozen_epoch == epoch) return false;
  ContextTouchControl cycle;
  cycle.action = TouchAction::kWeaponNext;
  cycle.kind = ContextTouchControlKind::kNativeButton;
  cycle.pad_buttons = button;
  g_runtime.native_pulses.push_back({pointer_id, epoch, cycle});
  if (REXCVAR_GET(gta4_touch_trace)) {
    REXLOG_INFO("gta4-touch: epoch={} pointer={} owner=weapon-hud op=cycle buttons={:04X}",
                epoch, pointer_id, button);
  }
  return true;
}

ContextTouchGameplayTransition::ContextTouchGameplayTransition() noexcept {
  {
    std::lock_guard lock(g_runtime.mutex);
    ++g_runtime.presentation_transition_depth;
    SuspendGameplayLocked();
  }
  GTA4_CancelTouchGameplayReplay();
}

ContextTouchGameplayTransition::~ContextTouchGameplayTransition() {
  {
    std::lock_guard lock(g_runtime.mutex);
    --g_runtime.presentation_transition_depth;
    g_presentation_revision.fetch_add(1, std::memory_order_release);
    g_runtime.awaiting_gameplay_poll = true;
  }
  GTA4_CancelTouchGameplayReplay();
}

bool ContextTouchEditorRequested() noexcept {
  std::lock_guard lock(g_runtime.mutex);
  return g_runtime.editor_pending || ContextTouchEditorOpen();
}

bool IsContextTouchEditorActive() noexcept {
  if (GTA4_TouchTitleInputOwned() || !rex::input::TouchPointerInputActive()) return false;
  std::lock_guard lock(g_runtime.mutex);
  return g_runtime.editor_session && g_runtime.editing && ContextTouchEditorOpen() &&
         g_runtime.layout.mode != ContextTouchMode::kDisabled;
}

bool ContextTouchEditorCapturesInput(uint64_t monotonic_ns) noexcept {
  if (!monotonic_ns) monotonic_ns = MonotonicNanoseconds();
  std::lock_guard lock(g_runtime.mutex);
  FinishEditorFadeLocked(monotonic_ns);
  return g_runtime.editor_session;
}

namespace {

ContextTouchOverlaySnapshot BuildOverlaySnapshotLocked() {
  if ((!g_runtime.editing && (g_runtime.context.frontend || g_runtime.context.map)) ||
      g_runtime.layout.mode == ContextTouchMode::kFrontend ||
      g_runtime.layout.mode == ContextTouchMode::kMap) return {};
  ContextTouchOverlaySnapshot snapshot{
      .layout = g_runtime.layout,
      .movement_x = static_cast<float>(g_runtime.movement_x),
      .movement_y = static_cast<float>(g_runtime.movement_y),
      .right_x = static_cast<float>(g_runtime.right_x),
      .right_y = static_cast<float>(g_runtime.right_y),
      .editor = g_runtime.editing,
      .visible = g_runtime.layout.mode != ContextTouchMode::kDisabled,
  };
  for (const auto& [id, owner] : g_runtime.pointers) {
    (void)id;
    if (owner.control_index < snapshot.active.size()) snapshot.active[owner.control_index] = 1;
    if (owner.control.kind == ContextTouchControlKind::kMovementStick && !owner.editing) {
      snapshot.movement_owned = true;
      snapshot.movement_origin_x = g_runtime.preferences.floating_stick ? owner.start_x : owner.control.center_x;
      snapshot.movement_origin_y = g_runtime.preferences.floating_stick ? owner.start_y : owner.control.center_y;
    }
    if (owner.composite_fire) {
      for (size_t i = 0; i < snapshot.layout.control_count; ++i) {
        if (SameControlIdentity(snapshot.layout.controls[i], owner.fire_control)) snapshot.active[i] = 1;
      }
    }
    if (owner.control.action == TouchAction::kWeaponWheel && g_runtime.weapon_wheel_open) {
      for (size_t i = 0; i < snapshot.layout.control_count; ++i) {
        const auto& c = snapshot.layout.controls[i];
        if (c.visible && c.action == TouchAction::kWeaponSelect && PointInside(c, owner.x, owner.y))
          snapshot.active[i] = 1;
      }
    }
  }
  return snapshot;
}

bool ViewportGeometryMatches(ContextTouchViewport left, ContextTouchViewport right) {
  left.generation = right.generation;
  return ContextTouchLayoutEquivalent({.viewport = left}, {.viewport = right});
}

}  // namespace

ContextTouchOverlaySnapshot GetContextTouchOverlaySnapshot() noexcept {
  if (GTA4_TouchTitleInputOwned() || !rex::input::TouchPointerInputActive()) return {};
  const auto context = GetTouchContextSnapshot();
  const bool policy_visible = rex::input::TouchControlsVisible();
  std::lock_guard lock(g_runtime.mutex);
  if (g_runtime.editor_closing || ((context.frontend || context.map) && !g_runtime.editing)) return {};
  if (!g_runtime.editing && !GameplayPresentationAdmittedLocked(context)) {
    HideGameplayLocked();
    return {};
  }
  if (!policy_visible && !g_runtime.editing) return {};
  return BuildOverlaySnapshotLocked();
}

ContextTouchOverlaySnapshot GetContextTouchDrawableOverlaySnapshot(uint64_t monotonic_ns) noexcept {
  const bool pointer_allowed = rex::input::TouchPointerInputActive() && !GTA4_TouchTitleInputOwned();
  const bool policy_visible = rex::input::TouchControlsVisible();
  const auto context = GetTouchContextSnapshot();
  const auto viewport = ReadViewport();
  if (!monotonic_ns) monotonic_ns = MonotonicNanoseconds();
  std::lock_guard lock(g_runtime.mutex);
  g_runtime.last_drawable_ns = std::max(monotonic_ns, g_runtime.last_drawable_ns);
  FinishEditorFadeLocked(monotonic_ns);
  const bool native_ui = context.frontend || context.map ||
                         g_runtime.context.frontend || g_runtime.context.map;
  const bool editor_drawable = g_runtime.editor_session &&
      (g_runtime.editing || (g_runtime.editor_closing && g_runtime.last_drawable.editor));
  if (!editor_drawable && !GameplayPresentationAdmittedLocked(context)) {
    HideGameplayLocked();
    return {};
  }
  if (!pointer_allowed || !viewport.valid || !viewport.focused || !g_runtime.initialized ||
      (native_ui && !editor_drawable)) {
    // Only an explicit editor session may draw over native menus or the map.
    g_runtime.fade.Reset();
    g_runtime.last_drawable = {};
    if (g_runtime.editor_closing) {
      g_runtime.editor_closing = false;
      g_runtime.editor_session = false;
    }
    return {};
  }
  const bool visible = (g_runtime.editing || (!g_runtime.editor_session && policy_visible && !native_ui)) &&
      g_runtime.layout.mode != ContextTouchMode::kDisabled &&
      g_runtime.layout.viewport.generation == viewport.generation;
  if (visible) g_runtime.last_drawable = BuildOverlaySnapshotLocked();
  if (g_runtime.last_drawable.visible &&
      !ViewportGeometryMatches(g_runtime.last_drawable.layout.viewport, viewport)) {
    // Rotation or a changed safe area invalidates the old screen positions.
    g_runtime.fade.Reset();
    g_runtime.last_drawable = {};
    return {};
  }
  const float alpha = g_runtime.fade.Advance(visible, monotonic_ns);
  auto snapshot = g_runtime.last_drawable;
  snapshot.fade_alpha = alpha;
  snapshot.visible = snapshot.visible && alpha > 0.0f;
  if (snapshot.visible) snapshot.layout.viewport = viewport;
  if (!visible && alpha == 0.0f) g_runtime.last_drawable = {};
  return snapshot;
}

bool GetTouchNativePadState(uint64_t epoch, TouchNativePadState* output) noexcept {
  if (output) *output = {};
  if (!output || GTA4_TouchTitleInputOwned() || !rex::input::TouchControlsActive()) return false;
  std::lock_guard lock(g_runtime.mutex);
  if (!GameplayPresentationAdmittedLocked(GetTouchContextSnapshot()) ||
      !g_runtime.native_pad.active || (epoch && g_runtime.native_pad.epoch != epoch) ||
      g_runtime.native_pad.epoch != g_runtime.epoch) return false;
  *output = g_runtime.native_pad;
  return true;
}

int32_t MergeTouchScopedZoom(uint64_t epoch, uint32_t input_user, uint32_t action,
                            int32_t original) noexcept {
  if (GTA4_TouchTitleInputOwned() || !rex::input::TouchControlsActive()) return original;
  std::lock_guard lock(g_runtime.mutex);
  if (epoch != g_runtime.epoch || input_user != g_runtime.context.input_user ||
      !g_runtime.context.gameplay_allowed || !g_runtime.context.input_user_known ||
      !GameplayPresentationAdmittedLocked(GetTouchContextSnapshot()) ||
      g_runtime.context.base_mode != ContextTouchMode::kOnFoot ||
      g_runtime.editor_session || (action != 24 && action != 26)) return original;
  // This API is called only after the original scoped-camera decoder. Its
  // admission is the evidence for showing zoom controls at the next poll.
  g_runtime.scoped_seen_epoch = epoch;
  g_runtime.scoped_generation = g_runtime.context.generation;
  g_runtime.scoped_action = action;
  if (!g_runtime.scoped_zoom || g_runtime.frozen_epoch != epoch) return original;
  return std::abs(int64_t(g_runtime.zoom)) > std::abs(int64_t(original)) ? g_runtime.zoom : original;
}

void ObserveTouchScriptQuery(TouchScriptQueryKind kind, uint32_t action, uint64_t epoch,
                             uint32_t input_group, uint32_t script_thread,
                             uint64_t generation) noexcept {
  std::lock_guard lock(g_runtime.mutex);
  if (!script_thread || generation != g_runtime.context.generation || epoch != g_runtime.epoch ||
      !g_runtime.context.native_input_allowed) return;
  const auto key = CanonicalScriptKey({kind, action, input_group, script_thread, generation});
  g_runtime.script_availability[key].last_seen_epoch = epoch;
}

void ObserveTouchParachuteState(uint32_t state, uint64_t epoch, uint32_t script_thread) noexcept {
  std::lock_guard lock(g_runtime.mutex);
  if (epoch != g_runtime.epoch) return;
  if (state != kParachuteFreefallState && state != kParachuteDeployedState) {
    g_runtime.parachute_last_seen_epoch = 0;
    g_runtime.parachute_state = 0;
    g_runtime.parachute_thread = 0;
    return;
  }
  g_runtime.parachute_state = state;
  g_runtime.parachute_thread = script_thread;
  g_runtime.parachute_generation = g_runtime.context.generation;
  g_runtime.parachute_last_seen_epoch = epoch;
}

uint32_t GetTouchScriptQueryValue(TouchScriptQueryKind kind, uint32_t action,
                                  uint64_t epoch, uint32_t input_group,
                                  uint32_t script_thread, uint64_t generation) noexcept {
  if (GTA4_TouchTitleInputOwned() || !rex::input::TouchControlsActive()) return 0;
  std::lock_guard lock(g_runtime.mutex);
  if (!script_thread || !ScriptQueryAllowedLocked(epoch, generation)) return 0;
  const uint32_t parachute = ParachuteQueryValueLocked(kind, action, epoch, script_thread);
  if (parachute) return parachute;
  const auto key = CanonicalScriptKey({kind, action, input_group, script_thread, generation});
  const auto pressed = g_runtime.script_pressed_epoch.find(key);
  const bool edge = epoch && pressed != g_runtime.script_pressed_epoch.end() && pressed->second == epoch;
  if (kind == TouchScriptQueryKind::kControlPressed || kind == TouchScriptQueryKind::kRawButtonPressed)
    return edge ? 1 : 0;
  const auto held = g_runtime.script_refcounts.find(key);
  if (!edge && (held == g_runtime.script_refcounts.end() || !held->second)) return 0;
  return kind == TouchScriptQueryKind::kControlAnalog ? 255 : 1;
}

bool GetTouchScriptAnalogueSticks(uint64_t epoch, std::array<int32_t, 4>* axes,
                                 uint32_t input_group, uint32_t script_thread,
                                 uint64_t generation) noexcept {
  if (!axes || GTA4_TouchTitleInputOwned() || !rex::input::TouchControlsActive()) return false;
  *axes = {};
  std::lock_guard lock(g_runtime.mutex);
  if (!script_thread || !ScriptQueryAllowedLocked(epoch, generation)) return false;
  const auto key = CanonicalScriptKey({TouchScriptQueryKind::kAnalogueSticks, 0,
                                       input_group, script_thread, generation});
  for (const auto& [id, owner] : g_runtime.pointers) {
    (void)id;
    if (owner.editing || !IsStick(owner.control)) continue;
    if (CanonicalScriptKey(owner.control.script) != key) continue;
    if (owner.control.kind == ContextTouchControlKind::kMovementStick) {
      (*axes)[0] = g_runtime.movement_x;
      (*axes)[1] = g_runtime.movement_y;
    } else {
      (*axes)[2] = g_runtime.right_x;
      (*axes)[3] = g_runtime.right_y;
    }
  }
  if (IsParachute(g_runtime.layout.mode) && script_thread == g_runtime.parachute_thread &&
      g_runtime.parachute_generation == g_runtime.context.generation) {
    const auto observed = g_runtime.script_availability.find(key);
    if (observed != g_runtime.script_availability.end() &&
        EpochWithin(epoch, observed->second.last_seen_epoch, kScriptQueryExpiryEpochs)) {
      *axes = {g_runtime.movement_x, g_runtime.movement_y, g_runtime.right_x, g_runtime.right_y};
    }
  }
  return std::any_of(axes->begin(), axes->end(), [](int32_t value) { return value != 0; });
}

bool MergeTouchScriptQueryResult(uint8_t* base, uint32_t call_context, TouchScriptQueryKind kind,
                                 uint64_t epoch, uint32_t script_thread, uint64_t generation) noexcept {
  if (!base || !call_context || call_context > std::numeric_limits<uint32_t>::max() - 12) return false;
  const uint32_t result = LoadU32(base, call_context);
  const uint32_t arguments = LoadU32(base, call_context + 8);
  if (!result || result > std::numeric_limits<uint32_t>::max() - 4 ||
      !arguments || arguments > std::numeric_limits<uint32_t>::max() - 8) return false;
  const uint32_t input_group = LoadU32(base, arguments);
  const uint32_t action = LoadU32(base, arguments + 4);
  ObserveTouchScriptQuery(kind, action, epoch, input_group, script_thread, generation);
  const uint32_t requested = GetTouchScriptQueryValue(kind, action, epoch, input_group, script_thread, generation);
  const uint32_t current = LoadU32(base, result);
  if (requested <= current) return false;
  StoreU32(base, result, requested);
  return true;
}

bool MergeTouchScriptAnalogueStickResults(uint8_t* base, uint32_t call_context,
                                          uint64_t epoch, uint32_t script_thread,
                                          uint64_t generation) noexcept {
  if (!base || !call_context || call_context > std::numeric_limits<uint32_t>::max() - 12) return false;
  const uint32_t arguments = LoadU32(base, call_context + 8);
  if (!arguments || arguments > std::numeric_limits<uint32_t>::max() - 20) return false;
  const uint32_t input_group = LoadU32(base, arguments);
  ObserveTouchScriptQuery(TouchScriptQueryKind::kAnalogueSticks, 0, epoch,
                          input_group, script_thread, generation);
  std::array<int32_t, 4> requested{};
  if (!GetTouchScriptAnalogueSticks(epoch, &requested, input_group, script_thread, generation)) return false;
  bool changed = false;
  for (size_t i = 0; i < requested.size(); ++i) {
    const uint32_t output = LoadU32(base, arguments + 4 + static_cast<uint32_t>(i * sizeof(uint32_t)));
    if (!output || output > std::numeric_limits<uint32_t>::max() - 4) continue;
    const int32_t current = static_cast<int32_t>(LoadU32(base, output));
    if (std::abs(int64_t(requested[i])) > std::abs(int64_t(current))) {
      StoreU32(base, output, static_cast<uint32_t>(requested[i]));
      changed = true;
    }
  }
  return changed;
}

}  // namespace gta4::input
