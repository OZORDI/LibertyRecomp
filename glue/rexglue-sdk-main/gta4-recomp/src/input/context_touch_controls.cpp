#include "input/context_touch_controls.h"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdint>
#include <limits>
#include <mutex>
#include <unordered_map>
#include <utility>
#include <vector>

#include <rex/cvar.h>
#include <rex/input/absolute_pointer.h>
#include <rex/input/mnk/encoded_action.h>
#include <rex/input/mnk/controller_compatibility.h>
#include <rex/logging.h>

#include "gta4_touch_coordinator.h"

REXCVAR_DECLARE(bool, gta4_touch_trace);

namespace gta4::input {
namespace {

constexpr uint32_t kActionArrayOffset = 2328;
constexpr uint32_t kActionStride = 12;
constexpr uint32_t kActionCurrentOffset = 2;
constexpr uint32_t kControlUserIndexOffset = 3412;
constexpr uint32_t kLastInputTimeOffset = 4200;
constexpr uint32_t kPrimaryTouchUser = 0;
constexpr uint32_t kGameInputTimeAddress = 0x82C6C2A4;
constexpr uint32_t kCurrentPlayerIndexAddress = 0x82A98778;
constexpr uint32_t kPlayerInfoTableAddress = 0x82C01C70;
constexpr uint32_t kPlayerInfoPedOffset = 1400;
constexpr uint32_t kPedVehicleFlagsOffset = 572;
constexpr uint32_t kPedVehicleOffset = 2688;
constexpr uint32_t kVehicleDriverOffset = 3904;
constexpr uint32_t kPedInVehicleFlag = 0x20000000;
constexpr uint32_t kMaximumLocalPlayers = 4;
constexpr uint32_t kGuestPointerSize = 4;
constexpr uint32_t kPhoneWithMovementFlagAddress = 0x831D4DD5;
constexpr uint32_t kMinigameActiveAddress = 0x82BA1D40;
constexpr uint32_t kHelicopterVtable = 0x8200B8D4;
constexpr uint32_t kAutomobileVtable = 0x82041D8C;
constexpr uint32_t kBikeVtable = 0x8204205C;
constexpr uint32_t kBoatVtable = 0x8204356C;
constexpr uint64_t kScriptQueryExpiryEpochs = 6;
constexpr uint32_t kParachuteFreefallState = 3;
constexpr uint32_t kParachuteDeployedState = 5;
constexpr float kLookUnitsPerPixel = 12.0f;

enum class Action : uint32_t {
  kMoveLeft = 12,
  kMoveRight = 13,
  kMoveUp = 14,
  kMoveDown = 15,
  kLookLeft = 16,
  kLookRight = 17,
  kLookUp = 18,
  kLookDown = 19,
  kVehicleMoveLeft = 30,
  kVehicleMoveRight = 31,
  kVehicleGunLeft = 34,
  kVehicleGunRight = 35,
  kVehicleGunUp = 36,
  kVehicleGunDown = 37,
  kVehicleLookLeft = 48,
  kVehicleLookRight = 49,
  kVehicleFlyYawLeft = 57,
  kVehicleFlyYawRight = 58,
  kVehicleMoveUp = 32,
  kVehicleMoveDown = 33,
};

struct ScriptKey {
  TouchScriptQueryKind kind = TouchScriptQueryKind::kControlHeld;
  uint32_t action = 0;

  bool operator==(const ScriptKey& other) const noexcept {
    return kind == other.kind && action == other.action;
  }
};

struct ScriptKeyHash {
  size_t operator()(const ScriptKey& key) const noexcept {
    return (static_cast<size_t>(key.action) << 3) ^ static_cast<size_t>(key.kind);
  }
};

ScriptKey CanonicalScriptKey(TouchScriptQueryKind kind, uint32_t action) {
  const auto canonical = CanonicalTouchScriptControl({kind, action});
  return {canonical.kind, canonical.action};
}

struct ScriptAvailability {
  uint64_t last_seen_epoch = 0;
};

struct PointerOwner {
  size_t control_index = 0;
  uint64_t layout_generation = 0;
  float start_x = 0.0f;
  float start_y = 0.0f;
  float x = 0.0f;
  float y = 0.0f;
};

struct RuntimeState {
  std::mutex mutex;
  ContextTouchLayout layout{};
  uint64_t epoch = 0;
  uint64_t look_epoch = 0;
  uint64_t layout_generation = 1;
  std::unordered_map<uint64_t, PointerOwner> pointers;
  ContextTouchKeyLatch key_latch{};
  std::unordered_map<ScriptKey, uint16_t, ScriptKeyHash> script_refcounts;
  std::unordered_map<ScriptKey, uint64_t, ScriptKeyHash> script_pressed_epoch;
  std::unordered_map<ScriptKey, ScriptAvailability, ScriptKeyHash> script_availability;
  uint64_t parachute_last_seen_epoch = 0;
  uint32_t parachute_state = 0;
  int32_t movement_x = 0;
  int32_t movement_y = 0;
  int32_t look_x = 0;
  int32_t look_y = 0;
  bool initialized = false;
};

RuntimeState g_runtime;

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

struct VehicleContext {
  uint32_t ped = 0;
  uint32_t vehicle = 0;
  uint32_t vtable = 0;
  bool driver = false;
};

VehicleContext ReadVehicleContext(uint8_t* base) {
  const uint32_t player_index = LoadU32(base, kCurrentPlayerIndexAddress);
  if (player_index >= kMaximumLocalPlayers) {
    return {};
  }
  const uint32_t player_info =
      LoadU32(base, kPlayerInfoTableAddress + player_index * kGuestPointerSize);
  if (!player_info) {
    return {};
  }
  const uint32_t ped = LoadU32(base, player_info + kPlayerInfoPedOffset);
  if (!ped || !(LoadU32(base, ped + kPedVehicleFlagsOffset) & kPedInVehicleFlag)) {
    return {};
  }
  const uint32_t vehicle = LoadU32(base, ped + kPedVehicleOffset);
  if (!vehicle) {
    return {};
  }
  return {
      .ped = ped,
      .vehicle = vehicle,
      .vtable = LoadU32(base, vehicle),
      .driver = LoadU32(base, vehicle + kVehicleDriverOffset) == ped,
  };
}

bool EpochWithin(uint64_t current, uint64_t observed, uint64_t lifetime) {
  return observed && current >= observed && current - observed <= lifetime;
}

ContextTouchMode DetermineModeLocked(uint8_t* base, uint64_t epoch) {
  if (EpochWithin(epoch, g_runtime.parachute_last_seen_epoch, kScriptQueryExpiryEpochs)) {
    if (g_runtime.parachute_state == kParachuteFreefallState) {
      return ContextTouchMode::kParachuteFreefall;
    }
    if (g_runtime.parachute_state == kParachuteDeployedState) {
      return ContextTouchMode::kParachuteDeployed;
    }
  }
  if (LoadU8(base, kPhoneWithMovementFlagAddress)) {
    return ContextTouchMode::kPhone;
  }
  if (LoadU32(base, kMinigameActiveAddress) > 0) {
    return ContextTouchMode::kMinigame;
  }
  const VehicleContext vehicle = ReadVehicleContext(base);
  if (vehicle.vehicle) {
    if (!vehicle.driver) {
      return ContextTouchMode::kVehiclePassenger;
    }
    if (vehicle.vtable == kHelicopterVtable) {
      return ContextTouchMode::kVehicleHelicopter;
    }
    if (vehicle.vtable == kBikeVtable) {
      return ContextTouchMode::kVehicleBike;
    }
    if (vehicle.vtable == kBoatVtable) {
      return ContextTouchMode::kVehicleBoat;
    }
    if (vehicle.vtable == kAutomobileVtable) {
      return ContextTouchMode::kVehicleAutomobile;
    }
    // An unproven driver class may be a train, aircraft, or another specialized
    // vehicle. Do not synthesize automobile steering for it.
    return ContextTouchMode::kVehicleDriverUnknown;
  }
  return ContextTouchMode::kOnFoot;
}

void ReleaseControlLocked(const ContextTouchControl& control, bool cancelled) {
  if (control.kind == ContextTouchControlKind::kButton) {
    g_runtime.key_latch.Release(control.key, cancelled);
  } else if (control.kind == ContextTouchControlKind::kScriptButton) {
    const ScriptKey key = CanonicalScriptKey(control.script.kind, control.script.action);
    auto it = g_runtime.script_refcounts.find(key);
    if (it != g_runtime.script_refcounts.end() && it->second && --it->second == 0) {
      g_runtime.script_refcounts.erase(it);
      if (cancelled) {
        g_runtime.script_pressed_epoch.erase(key);
      }
    }
  }
}

void CancelAllLocked() {
  g_runtime.pointers.clear();
  g_runtime.key_latch.Cancel();
  g_runtime.script_refcounts.clear();
  g_runtime.script_pressed_epoch.clear();
  g_runtime.movement_x = 0;
  g_runtime.movement_y = 0;
  g_runtime.look_x = 0;
  g_runtime.look_y = 0;
}

bool ScriptControlOwnedLocked(const ScriptKey& key) {
  if (key.kind == TouchScriptQueryKind::kAnalogueSticks) {
    return std::any_of(g_runtime.pointers.begin(), g_runtime.pointers.end(), [](const auto& item) {
      const size_t index = item.second.control_index;
      return index < g_runtime.layout.control_count &&
             g_runtime.layout.controls[index].kind == ContextTouchControlKind::kMovementStick;
    });
  }
  const auto held = g_runtime.script_refcounts.find(key);
  return held != g_runtime.script_refcounts.end() && held->second != 0;
}

std::vector<TouchScriptControl> ActiveScriptControlsLocked(uint64_t epoch) {
  std::vector<TouchScriptControl> controls;
  for (auto it = g_runtime.script_availability.begin();
       it != g_runtime.script_availability.end();) {
    const bool owned = ScriptControlOwnedLocked(it->first);
    if (!owned && !EpochWithin(epoch, it->second.last_seen_epoch, kScriptQueryExpiryEpochs)) {
      g_runtime.script_pressed_epoch.erase(it->first);
      it = g_runtime.script_availability.erase(it);
      continue;
    }
    controls.push_back({it->first.kind, it->first.action});
    ++it;
  }
  std::sort(controls.begin(), controls.end(), [](const auto& left, const auto& right) {
    // The visible grid is bounded by the safe area. Preserve active owners
    // before truncating newly discovered controls to that grid's capacity.
    const bool left_owned = ScriptControlOwnedLocked({left.kind, left.action});
    const bool right_owned = ScriptControlOwnedLocked({right.kind, right.action});
    if (left_owned != right_owned) {
      return left_owned;
    }
    if (left.kind != right.kind) {
      return left.kind < right.kind;
    }
    return left.action < right.action;
  });
  return controls;
}

ContextTouchViewport ReadViewport() {
  rex::input::TouchPresentationState presentation;
  if (!rex::input::GetTouchPresentationState(&presentation)) {
    return {};
  }
  return {
      .output_width = presentation.output_width,
      .output_height = presentation.output_height,
      .physical_output_x = presentation.physical_output_x,
      .physical_output_y = presentation.physical_output_y,
      .physical_output_width = presentation.physical_output_width,
      .physical_output_height = presentation.physical_output_height,
      .physical_surface_width = presentation.physical_surface_width,
      .physical_surface_height = presentation.physical_surface_height,
      .safe_x = static_cast<float>(presentation.safe_area_x),
      .safe_y = static_cast<float>(presentation.safe_area_y),
      .safe_width = static_cast<float>(presentation.safe_area_width),
      .safe_height = static_cast<float>(presentation.safe_area_height),
      .generation = presentation.generation,
      .valid = presentation.valid,
      .focused = presentation.focused,
  };
}

bool SameControlIdentity(const ContextTouchControl& a, const ContextTouchControl& b) {
  return a.kind == b.kind && a.key == b.key && a.script.kind == b.script.kind &&
         a.script.action == b.script.action;
}

void RecalculateAxesLocked();

void RefreshLayoutLocked(uint8_t* base, uint64_t epoch, bool frontend = false, bool map = false) {
  if (g_runtime.look_epoch != epoch) {
    g_runtime.look_epoch = epoch;
    g_runtime.look_x = 0;
    g_runtime.look_y = 0;
  }
  g_runtime.epoch = epoch;
  const bool enabled = rex::input::TouchControlsActive();
  const ContextTouchViewport viewport = ReadViewport();
  const ContextTouchMode mode = !enabled ? ContextTouchMode::kDisabled :
      map ? ContextTouchMode::kMap : frontend ? ContextTouchMode::kFrontend :
      DetermineModeLocked(base, epoch);
  const std::vector<TouchScriptControl> script_controls = mode == ContextTouchMode::kMinigame
                                                              ? ActiveScriptControlsLocked(epoch)
                                                              : std::vector<TouchScriptControl>{};
  ContextTouchLayout next = BuildContextTouchLayout(mode, viewport, script_controls);
  if (!ContextTouchLayoutEquivalent(g_runtime.layout, next)) {
    const ContextTouchMode previous = g_runtime.layout.mode;
    ContextTouchLayout old_geometry{.mode = mode, .viewport = g_runtime.layout.viewport};
    ContextTouchLayout new_geometry{.mode = mode, .viewport = next.viewport};
    const bool retain = previous == ContextTouchMode::kMinigame && mode == previous &&
                         ContextTouchLayoutEquivalent(old_geometry, new_geometry);
    if (retain) {
      // Reserve locations of surviving controls first; assign new controls only
      // unused cells. A query changing from pressed to held has one identity.
      std::array<bool, ContextTouchLayout::kMaximumControls> retained{};
      for (size_t n = 0; n < next.control_count; ++n) {
        for (size_t o = 0; o < g_runtime.layout.control_count; ++o) {
          if (SameControlIdentity(next.controls[n], g_runtime.layout.controls[o])) {
            next.controls[n] = g_runtime.layout.controls[o]; retained[n] = true; break;
          }
        }
      }
      for (size_t n = 0; n < next.control_count; ++n) {
        auto& control = next.controls[n];
        if (retained[n] || control.kind != ContextTouchControlKind::kScriptButton) continue;
        const float edge = std::min(viewport.safe_width, viewport.safe_height);
        const float step = control.radius * 2.0f + edge * 0.025f;
        for (size_t cell = 0; cell < ContextTouchLayout::kMaximumControls; ++cell) {
          const float x = viewport.safe_x + viewport.safe_width - edge * 0.035f -
                          control.radius - float(cell % 2) * step;
          const float y = viewport.safe_y + viewport.safe_height - edge * 0.035f -
                          control.radius - float(cell / 2) * step;
          bool occupied = false;
          for (size_t j = 0; j < next.control_count; ++j) {
            if (j == n || (!retained[j] && j > n)) continue;
            const auto& other = next.controls[j];
            occupied |= other.kind == ContextTouchControlKind::kScriptButton &&
                        std::abs(other.center_x - x) < control.radius &&
                        std::abs(other.center_y - y) < control.radius;
          }
          if (!occupied) {
            control.center_x = x; control.center_y = y;
            control.minimum_x = x - control.radius; control.maximum_x = x + control.radius;
            control.minimum_y = y - control.radius; control.maximum_y = y + control.radius;
            break;
          }
        }
      }
      for (auto it = g_runtime.pointers.begin(); it != g_runtime.pointers.end();) {
        auto& owner = it->second;
        const auto old_control = g_runtime.layout.controls[owner.control_index];
        bool matched = false;
        for (size_t n = 0; n < next.control_count; ++n) {
          if (SameControlIdentity(old_control, next.controls[n])) {
            owner.control_index = n;
            owner.layout_generation = g_runtime.layout_generation + 1;
            matched = true;
            break;
          }
        }
        if (matched) {
          ++it;
        } else {
          ReleaseControlLocked(old_control, true);
          it = g_runtime.pointers.erase(it);
        }
      }
    } else {
      CancelAllLocked();
    }
    g_runtime.layout = std::move(next);
    ++g_runtime.layout_generation;
    RecalculateAxesLocked();
    if (REXCVAR_GET(gta4_touch_trace)) {
      REXLOG_INFO(
          "gta4-touch: layout epoch={} mode={}->{} generation={} host_generation={} "
          "controls={} enabled={} focused={}",
          epoch, static_cast<uint32_t>(previous), static_cast<uint32_t>(mode),
          g_runtime.layout_generation, viewport.generation, g_runtime.layout.control_count, enabled,
          viewport.focused);
    }
  }
}

bool PointInside(const ContextTouchControl& control, float x, float y) {
  if (control.kind == ContextTouchControlKind::kLookSurface) {
    return x >= control.minimum_x && x <= control.maximum_x && y >= control.minimum_y &&
           y <= control.maximum_y;
  }
  const float dx = x - control.center_x;
  const float dy = y - control.center_y;
  return dx * dx + dy * dy <= control.radius * control.radius;
}

size_t FindControlLocked(float x, float y) {
  for (size_t index = 0; index < g_runtime.layout.control_count; ++index) {
    const auto& control = g_runtime.layout.controls[index];
    if (control.kind != ContextTouchControlKind::kLookSurface && PointInside(control, x, y)) {
      return index;
    }
  }
  for (size_t index = 0; index < g_runtime.layout.control_count; ++index) {
    if (g_runtime.layout.controls[index].kind == ContextTouchControlKind::kLookSurface &&
        PointInside(g_runtime.layout.controls[index], x, y)) {
      return index;
    }
  }
  return g_runtime.layout.control_count;
}

bool ControlAlreadyOwnedLocked(size_t control_index) {
  return std::any_of(g_runtime.pointers.begin(), g_runtime.pointers.end(),
                     [control_index](const auto& entry) {
                       const auto kind = g_runtime.layout.controls[entry.second.control_index].kind;
                       return entry.second.control_index == control_index &&
                              (kind == ContextTouchControlKind::kMovementStick ||
                               kind == ContextTouchControlKind::kLookSurface);
                     });
}

void PressControlLocked(const ContextTouchControl& control, uint64_t epoch) {
  if (control.kind == ContextTouchControlKind::kButton) {
    g_runtime.key_latch.Press(control.key, epoch);
  } else if (control.kind == ContextTouchControlKind::kScriptButton) {
    const ScriptKey key = CanonicalScriptKey(control.script.kind, control.script.action);
    uint16_t& refcount = g_runtime.script_refcounts[key];
    if (!refcount) {
      g_runtime.script_pressed_epoch[key] = epoch;
    }
    if (refcount != std::numeric_limits<uint16_t>::max()) {
      ++refcount;
    }
  }
}

void RecalculateAxesLocked() {
  g_runtime.movement_x = 0;
  g_runtime.movement_y = 0;
  for (const auto& [pointer_id, owner] : g_runtime.pointers) {
    (void)pointer_id;
    if (owner.control_index >= g_runtime.layout.control_count) {
      continue;
    }
    const auto& control = g_runtime.layout.controls[owner.control_index];
    if (control.kind == ContextTouchControlKind::kMovementStick) {
      g_runtime.movement_x = ContextTouchAxis(owner.x - control.center_x, control.radius);
      g_runtime.movement_y = ContextTouchAxis(owner.y - control.center_y, control.radius);
      if (g_runtime.layout.mode == ContextTouchMode::kVehicleAutomobile ||
          g_runtime.layout.mode == ContextTouchMode::kVehicleBoat) {
        g_runtime.movement_y = 0;
      }
    }
  }
}

bool OnPointerEvent(const rex::input::AbsolutePointerEvent& event, PPCContext&, uint8_t* base,
                    uint64_t epoch) {
  std::lock_guard lock(g_runtime.mutex);
  if (g_runtime.epoch != epoch) {
    return false;
  }
  if (g_runtime.layout.mode == ContextTouchMode::kDisabled ||
      event.generation != g_runtime.layout.viewport.generation) {
    CancelAllLocked();
    return false;
  }

  if (event.phase == rex::input::AbsolutePointerPhase::kDown) {
    const size_t index = FindControlLocked(event.x, event.y);
    if (index >= g_runtime.layout.control_count || ControlAlreadyOwnedLocked(index)) {
      return false;
    }
    g_runtime.pointers[event.pointer_id] = {
        .control_index = index,
        .layout_generation = g_runtime.layout_generation,
        .start_x = event.x,
        .start_y = event.y,
        .x = event.x,
        .y = event.y,
    };
    PressControlLocked(g_runtime.layout.controls[index], epoch);
    RecalculateAxesLocked();
    if (REXCVAR_GET(gta4_touch_trace)) {
      REXLOG_INFO("gta4-touch: down epoch={} pointer={} control={} mode={}", epoch,
                  event.pointer_id, index, static_cast<uint32_t>(g_runtime.layout.mode));
    }
    return true;
  }

  auto pointer = g_runtime.pointers.find(event.pointer_id);
  if (pointer == g_runtime.pointers.end()) {
    return false;
  }
  if (pointer->second.layout_generation != g_runtime.layout_generation) {
    g_runtime.pointers.erase(pointer);
    RecalculateAxesLocked();
    return true;
  }
  if (event.phase == rex::input::AbsolutePointerPhase::kMove) {
    const auto& control = g_runtime.layout.controls[pointer->second.control_index];
    if (control.kind == ContextTouchControlKind::kLookSurface) {
      g_runtime.look_x = static_cast<int32_t>(std::lround(std::clamp(
          static_cast<float>(g_runtime.look_x) + (event.x - pointer->second.x) * kLookUnitsPerPixel,
          -255.0f, 255.0f)));
      g_runtime.look_y = static_cast<int32_t>(std::lround(std::clamp(
          static_cast<float>(g_runtime.look_y) + (event.y - pointer->second.y) * kLookUnitsPerPixel,
          -255.0f, 255.0f)));
    }
    pointer->second.x = event.x;
    pointer->second.y = event.y;
    RecalculateAxesLocked();
    return true;
  }

  const size_t control_index = pointer->second.control_index;
  if (control_index < g_runtime.layout.control_count) {
    ReleaseControlLocked(g_runtime.layout.controls[control_index],
                         event.phase == rex::input::AbsolutePointerPhase::kCancel);
  }
  g_runtime.pointers.erase(pointer);
  RecalculateAxesLocked();
  if (REXCVAR_GET(gta4_touch_trace)) {
    REXLOG_INFO("gta4-touch: terminal epoch={} pointer={} control={} cancel={}", epoch,
                event.pointer_id, control_index,
                event.phase == rex::input::AbsolutePointerPhase::kCancel);
  }
  return true;
}

void BeginPoll(PPCContext&, uint8_t* base, uint64_t epoch, bool frontend, bool map) {
  std::lock_guard lock(g_runtime.mutex);
  RefreshLayoutLocked(base, epoch, frontend, map);
}

void CollectVirtualKeys(uint64_t epoch, std::array<uint8_t, 256>& down,
                        std::array<uint8_t, 256>& pressed) {
  std::lock_guard lock(g_runtime.mutex);
  if (g_runtime.epoch == epoch) {
    g_runtime.key_latch.Collect(epoch, down, pressed);
  }
}

void OnControlsDisabled(PPCContext&, uint8_t*, uint64_t epoch) {
  std::lock_guard lock(g_runtime.mutex);
  g_runtime.epoch = epoch;
  CancelAllLocked();
  g_runtime.layout = {};
  ++g_runtime.layout_generation;
}

uint32_t ActionAddress(uint32_t control, Action action) {
  return control + kActionArrayOffset + static_cast<uint32_t>(action) * kActionStride;
}

bool MergeAxis(uint8_t* base, uint32_t control, Action negative, Action positive,
               int32_t requested) {
  if (!requested) {
    return false;
  }
  const uint32_t negative_address = ActionAddress(control, negative);
  const uint32_t positive_address = ActionAddress(control, positive);
  const uint8_t negative_polarity = LoadU8(base, negative_address);
  const uint8_t positive_polarity = LoadU8(base, positive_address);
  const uint8_t negative_current = LoadU8(base, negative_address + kActionCurrentOffset);
  const uint8_t positive_current = LoadU8(base, positive_address + kActionCurrentOffset);
  const auto merge = rex::input::mnk::MergeSignedActionPair(
      negative_polarity, negative_current, positive_polarity, positive_current, requested);
  if (merge.changed) {
    StoreU8(base, negative_address + kActionCurrentOffset, merge.negative_encoded);
    StoreU8(base, positive_address + kActionCurrentOffset, merge.positive_encoded);
  }
  return merge.changed;
}

void OnControlReplay(PPCContext&, uint8_t* base, uint32_t control, uint32_t, uint64_t epoch) {
  std::lock_guard lock(g_runtime.mutex);
  if (g_runtime.epoch != epoch || !control ||
      LoadU32(base, control + kControlUserIndexOffset) != kPrimaryTouchUser) {
    return;
  }
  const ContextTouchMode mode = g_runtime.layout.mode;
  bool changed = false;
  if (mode == ContextTouchMode::kOnFoot || mode == ContextTouchMode::kPhone) {
    changed |=
        MergeAxis(base, control, Action::kMoveLeft, Action::kMoveRight, g_runtime.movement_x);
    changed |= MergeAxis(base, control, Action::kMoveUp, Action::kMoveDown, g_runtime.movement_y);
  } else if (mode == ContextTouchMode::kVehicleAutomobile ||
             mode == ContextTouchMode::kVehicleBoat) {
    changed |= MergeAxis(base, control, Action::kVehicleMoveLeft, Action::kVehicleMoveRight,
                         g_runtime.movement_x);
  } else if (mode == ContextTouchMode::kVehicleBike) {
    changed |= MergeAxis(base, control, Action::kVehicleMoveLeft, Action::kVehicleMoveRight,
                         g_runtime.movement_x);
    changed |= MergeAxis(base, control, Action::kVehicleMoveUp, Action::kVehicleMoveDown,
                         g_runtime.movement_y);
  } else if (mode == ContextTouchMode::kVehicleHelicopter) {
    // Generated sub_822ABEE0: 30/31 is bank, 32/33 is pitch, 57/58 is yaw.
    // Yaw has separate Numpad4/Numpad6 touch buttons in the virtual-key path.
    changed |= MergeAxis(base, control, Action::kVehicleMoveLeft, Action::kVehicleMoveRight,
                         g_runtime.movement_x);
    changed |= MergeAxis(base, control, Action::kVehicleMoveUp, Action::kVehicleMoveDown,
                         g_runtime.movement_y);
  }

  if (mode == ContextTouchMode::kOnFoot) {
    changed |= MergeAxis(base, control, Action::kLookLeft, Action::kLookRight, g_runtime.look_x);
    changed |= MergeAxis(base, control, Action::kLookUp, Action::kLookDown, g_runtime.look_y);
  } else if (mode == ContextTouchMode::kVehicleAutomobile ||
             mode == ContextTouchMode::kVehicleBike || mode == ContextTouchMode::kVehicleBoat ||
             mode == ContextTouchMode::kVehicleHelicopter ||
             mode == ContextTouchMode::kVehicleDriverUnknown ||
             mode == ContextTouchMode::kVehiclePassenger) {
    changed |= MergeAxis(base, control, Action::kVehicleGunLeft, Action::kVehicleGunRight,
                         g_runtime.look_x);
    changed |=
        MergeAxis(base, control, Action::kVehicleGunUp, Action::kVehicleGunDown, g_runtime.look_y);
    changed |= MergeAxis(base, control, Action::kVehicleLookLeft, Action::kVehicleLookRight,
                         g_runtime.look_x);
  }
  if (changed) {
    StoreU32(base, control + kLastInputTimeOffset, LoadU32(base, kGameInputTimeAddress));
  }
}

}  // namespace

void InitializeContextTouchControls() noexcept {
  std::lock_guard lock(g_runtime.mutex);
  if (g_runtime.initialized) {
    return;
  }
  g_runtime.initialized = true;
  GTA4_RegisterTouchExtension({
      .begin_poll = &BeginPoll,
      .on_pointer_event = &OnPointerEvent,
      .collect_virtual_keys = &CollectVirtualKeys,
      .on_controls_disabled = &OnControlsDisabled,
      .on_control_replay = &OnControlReplay,
  });
}

void ShutdownContextTouchControls() noexcept {
  rex::input::mnk::PublishVirtualControllerCompatibilityKeys(0, {}, false);
  GTA4_RegisterTouchExtension({});
  std::lock_guard lock(g_runtime.mutex);
  CancelAllLocked();
  g_runtime.layout = {};
  g_runtime.script_availability.clear();
  g_runtime.parachute_last_seen_epoch = 0;
  g_runtime.parachute_state = 0;
  g_runtime.initialized = false;
}

ContextTouchOverlaySnapshot GetContextTouchOverlaySnapshot() noexcept {
  if (GTA4_TouchTitleInputOwned()) {
    return {};
  }
  std::lock_guard lock(g_runtime.mutex);
  ContextTouchOverlaySnapshot snapshot{
      .layout = g_runtime.layout,
      .movement_x = static_cast<float>(g_runtime.movement_x),
      .movement_y = static_cast<float>(g_runtime.movement_y),
      .visible = g_runtime.layout.mode != ContextTouchMode::kDisabled,
  };
  for (const auto& [pointer_id, owner] : g_runtime.pointers) {
    (void)pointer_id;
    if (owner.control_index < snapshot.active.size()) {
      snapshot.active[owner.control_index] = 1;
    }
  }
  return snapshot;
}

void ObserveTouchScriptQuery(TouchScriptQueryKind kind, uint32_t action, uint64_t epoch) noexcept {
  std::lock_guard lock(g_runtime.mutex);
  g_runtime.script_availability[CanonicalScriptKey(kind, action)].last_seen_epoch = epoch;
}

void ObserveTouchParachuteState(uint32_t state, uint64_t epoch) noexcept {
  if (state != kParachuteFreefallState && state != kParachuteDeployedState) {
    return;
  }
  std::lock_guard lock(g_runtime.mutex);
  g_runtime.parachute_state = state;
  g_runtime.parachute_last_seen_epoch = epoch;
}

uint32_t GetTouchScriptQueryValue(TouchScriptQueryKind kind, uint32_t action,
                                  uint64_t epoch) noexcept {
  if (GTA4_TouchTitleInputOwned()) {
    return 0;
  }
  std::lock_guard lock(g_runtime.mutex);
  if (g_runtime.layout.mode != ContextTouchMode::kMinigame || epoch != g_runtime.epoch) {
    return 0;
  }
  const ScriptKey key = CanonicalScriptKey(kind, action);
  const auto pressed = g_runtime.script_pressed_epoch.find(key);
  const bool edge = epoch != 0 && pressed != g_runtime.script_pressed_epoch.end() &&
                    pressed->second == epoch;
  if (kind == TouchScriptQueryKind::kControlPressed) {
    return edge ? 1 : 0;
  }
  const auto held = g_runtime.script_refcounts.find(key);
  if (!edge && (held == g_runtime.script_refcounts.end() || !held->second)) {
    return 0;
  }
  return kind == TouchScriptQueryKind::kControlAnalog ? 255 : 1;
}

bool GetTouchScriptAnalogueSticks(uint64_t epoch, int32_t* horizontal, int32_t* vertical) noexcept {
  if (GTA4_TouchTitleInputOwned()) {
    return false;
  }
  std::lock_guard lock(g_runtime.mutex);
  if (epoch != g_runtime.epoch || (g_runtime.layout.mode != ContextTouchMode::kMinigame &&
                                   g_runtime.layout.mode != ContextTouchMode::kParachuteFreefall &&
                                   g_runtime.layout.mode != ContextTouchMode::kParachuteDeployed)) {
    return false;
  }
  if (horizontal) {
    *horizontal = g_runtime.movement_x;
  }
  if (vertical) {
    *vertical = g_runtime.movement_y;
  }
  return g_runtime.movement_x || g_runtime.movement_y;
}

bool MergeTouchScriptQueryResult(uint8_t* base, uint32_t call_context, TouchScriptQueryKind kind,
                                 uint64_t epoch) noexcept {
  if (!base || !call_context || LoadU32(base, kMinigameActiveAddress) == 0) {
    return false;
  }
  const uint32_t result = LoadU32(base, call_context);
  const uint32_t arguments = LoadU32(base, call_context + 8);
  if (!result || !arguments) {
    return false;
  }
  const uint32_t action = LoadU32(base, arguments + 4);
  ObserveTouchScriptQuery(kind, action, epoch);
  const uint32_t requested = GetTouchScriptQueryValue(kind, action, epoch);
  const uint32_t current = LoadU32(base, result);
  if (requested <= current) {
    return false;
  }
  StoreU32(base, result, requested);
  return true;
}

bool MergeTouchScriptAnalogueStickResults(uint8_t* base, uint32_t call_context,
                                          uint64_t epoch) noexcept {
  if (!base || !call_context) {
    return false;
  }
  const bool minigame_active = LoadU32(base, kMinigameActiveAddress) != 0;
  const uint32_t arguments = LoadU32(base, call_context + 8);
  if (!arguments) {
    return false;
  }
  if (minigame_active) {
    ObserveTouchScriptQuery(TouchScriptQueryKind::kAnalogueSticks, 0, epoch);
  }
  int32_t horizontal = 0;
  int32_t vertical = 0;
  if (!GetTouchScriptAnalogueSticks(epoch, &horizontal, &vertical)) {
    return false;
  }

  bool changed = false;
  const std::array requested = {horizontal, vertical};
  for (size_t index = 0; index < requested.size(); ++index) {
    const uint32_t output =
        LoadU32(base, arguments + 4 + static_cast<uint32_t>(index * sizeof(uint32_t)));
    if (!output) {
      continue;
    }
    const int32_t current = static_cast<int32_t>(LoadU32(base, output));
    const int32_t current_magnitude = current < 0 ? -current : current;
    const int32_t requested_magnitude = requested[index] < 0 ? -requested[index] : requested[index];
    if (requested_magnitude > current_magnitude) {
      StoreU32(base, output, static_cast<uint32_t>(requested[index]));
      changed = true;
    }
  }
  return changed;
}

}  // namespace gta4::input
