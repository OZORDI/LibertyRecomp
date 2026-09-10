#include "gta4_motion_bridge.h"
#include "gta4_motion_action_policy.h"

#include <algorithm>
#include <atomic>
#include <bit>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <limits>
#include <mutex>

#include <rex/cvar.h>
#include <rex/logging.h>

#include "gta4_init.h"
#include "gta4_pc_input_bridge.h"

REXCVAR_DEFINE_INT32(gta4_motion_vehicle_reentry_ms, 750, "GTA IV/Motion Sensor/Tuning",
                     "Gap after which the same vehicle is treated as a new entry")
    .range(100, 5000);

namespace gta4 {
namespace {

// All guest offsets and masks in this file are checked against generated PPC by
// verify_motion_vehicle_sites.py. The bNotInAir mask is additionally
// cross-checked against CVehicleWheel::m_nFlags in the public IV SDK.
constexpr uint32_t kPrimaryReloadActionRecord = 0x82B2AD64;

constexpr uint32_t kAutomobileWheelsOffset = 3952;
constexpr uint32_t kAutomobileWheelCountOffset = 3956;
constexpr uint32_t kWheelFlagsOffset = 356;
constexpr uint32_t kWheelStride = 368;
constexpr uint32_t kWheelNotInAirMask = 0x00000002;
constexpr uint32_t kPrimaryUserIndex = 0;

struct PlayerVehicleTracker {
  std::mutex mutex;
  uint32_t vehicle = 0;
  VehicleMotionKind kind = VehicleMotionKind::kAutomobile;
  std::chrono::steady_clock::time_point last_seen = {};
};

PlayerVehicleTracker g_player_vehicle_tracker;
std::atomic<uint64_t> g_motion_application_count = 0;

uint32_t ResolvePlayerPad(const PPCContext& parent, uint8_t* base, uint32_t controller) {
  PPCContext nested = parent;
  nested.r3.u32 = controller;
  __imp__sub_823CF688(nested, base);
  return nested.r3.u32;
}

void ObservePlayerVehicle(uint32_t vehicle, VehicleMotionKind kind) {
  const auto now = std::chrono::steady_clock::now();
  const auto reentry_gap = std::chrono::milliseconds(REXCVAR_GET(gta4_motion_vehicle_reentry_ms));
  bool entered = false;
  {
    std::lock_guard lock(g_player_vehicle_tracker.mutex);
    entered = !g_player_vehicle_tracker.vehicle || g_player_vehicle_tracker.vehicle != vehicle ||
              g_player_vehicle_tracker.kind != kind ||
              now - g_player_vehicle_tracker.last_seen > reentry_gap;
    g_player_vehicle_tracker.vehicle = vehicle;
    g_player_vehicle_tracker.kind = kind;
    g_player_vehicle_tracker.last_seen = now;
  }
  if (entered) {
    GTA4MotionBridge::Get().NotifyVehicleEntry(kPrimaryUserIndex);
    REXLOG_INFO("gta4-motion: player entered vehicle {:08X} category={}", vehicle,
                static_cast<uint32_t>(kind));
  }
}

bool IsAutomobileAirborne(uint8_t* base, uint32_t automobile) {
  const uint32_t wheels = REX_LOAD_U32(automobile + kAutomobileWheelsOffset);
  const uint32_t wheel_count = REX_LOAD_U32(automobile + kAutomobileWheelCountOffset);
  if (!wheels || !wheel_count || wheel_count > 32) {
    return false;
  }

  for (uint32_t index = 0; index < wheel_count; ++index) {
    const uint64_t wheel_address =
        static_cast<uint64_t>(wheels) + static_cast<uint64_t>(index) * kWheelStride;
    if (wheel_address + kWheelFlagsOffset + sizeof(uint32_t) >
        static_cast<uint64_t>(std::numeric_limits<uint32_t>::max()) + 1) {
      return false;
    }
    const uint32_t flags = REX_LOAD_U32(static_cast<uint32_t>(wheel_address) + kWheelFlagsOffset);
    if (flags & kWheelNotInAirMask) {
      return false;
    }
  }
  return true;
}

void LogAppliedMotion(VehicleMotionKind kind, uint32_t vehicle) {
  const uint64_t count = ++g_motion_application_count;
  if (count <= 16 || !(count % 2048)) {
    REXLOG_DEBUG("gta4-motion: applied vehicle motion #{} category={} vehicle={:08X}", count,
                 static_cast<uint32_t>(kind), vehicle);
  }
}

bool MotionKindEnabled(const GTA4MotionBridge& bridge, VehicleMotionKind kind) {
  switch (kind) {
    case VehicleMotionKind::kHelicopter:
      return bridge.IsPreferenceEnabled(MotionPreference::kHelicopter);
    case VehicleMotionKind::kBike:
      return bridge.IsPreferenceEnabled(MotionPreference::kBike);
    case VehicleMotionKind::kBoat:
      return bridge.IsPreferenceEnabled(MotionPreference::kBoat);
    case VehicleMotionKind::kAutomobile:
      return bridge.IsPreferenceEnabled(MotionPreference::kAftertouch);
  }
  return false;
}

void InvokeVehicleControl(PPCContext& ctx, uint8_t* base, PPCFunc* original,
                           VehicleMotionKind kind) {
  const uint32_t vehicle = ctx.r3.u32, controller = ctx.r4.u32;
  const uint32_t pad = vehicle && controller ? ResolvePlayerPad(ctx, base, controller) : 0;
  VehicleMotionActions actions{};
  if (pad && REX_LOAD_U32(pad + 3412) == kPrimaryUserIndex) {
    ObservePlayerVehicle(vehicle, kind);
    auto& bridge = GTA4MotionBridge::Get();
    const MotionSnapshot motion = bridge.Read(kPrimaryUserIndex);
    if (motion.controls_enabled && motion.fresh && MotionKindEnabled(bridge, kind)) {
      const bool airborne = kind != VehicleMotionKind::kAutomobile ||
                            IsAutomobileAirborne(base, vehicle);
      actions = BuildVehicleMotionActions(kind, motion.roll_axis, motion.pitch_axis, airborne);
    }
  }
  ScopedMotionActions scope(pad, actions,
      [base](uint32_t address) { return REX_LOAD_U8(address); },
      [base](uint32_t address, uint8_t value) { REX_STORE_U8(address, value); });
  // All field derivation (including boat/bike steering +4216) stays in the
  // compiled retail routine. Its register results are retained unchanged.
  original(ctx, base);
  if (scope.changed()) {
    LogAppliedMotion(kind, vehicle);
  }
}

}  // namespace
}  // namespace gta4

extern "C" void sub_82163CE0(PPCContext& ctx, uint8_t* base) {
  const uint32_t action_record = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  __imp__sub_82163CE0(ctx, base);
  gta4::input::MaybeForceDirectWeaponAction(ctx, base, action_record, caller);
  if (action_record != gta4::kPrimaryReloadActionRecord) {
    return;
  }

  auto& bridge = gta4::GTA4MotionBridge::Get();
  const gta4::MotionSnapshot motion = bridge.Read(gta4::kPrimaryUserIndex);
  if (motion.controls_enabled && motion.fresh &&
      bridge.IsPreferenceEnabled(gta4::MotionPreference::kReload) &&
      bridge.ConsumeReloadGesture(gta4::MotionReloadConsumer::kGameplay, gta4::kPrimaryUserIndex)) {
    ctx.r3.u64 = 1;
  }
}

extern "C" void sub_822ABEE0(PPCContext& ctx, uint8_t* base) {
  gta4::InvokeVehicleControl(ctx, base, __imp__sub_822ABEE0, gta4::VehicleMotionKind::kHelicopter);
}

extern "C" void sub_82643870(PPCContext& ctx, uint8_t* base) {
  gta4::InvokeVehicleControl(ctx, base, __imp__sub_82643870, gta4::VehicleMotionKind::kAutomobile);
}

extern "C" void sub_82647DD0(PPCContext& ctx, uint8_t* base) {
  gta4::InvokeVehicleControl(ctx, base, __imp__sub_82647DD0, gta4::VehicleMotionKind::kBike);
}

extern "C" void sub_82664450(PPCContext& ctx, uint8_t* base) {
  gta4::InvokeVehicleControl(ctx, base, __imp__sub_82664450, gta4::VehicleMotionKind::kBoat);
}
