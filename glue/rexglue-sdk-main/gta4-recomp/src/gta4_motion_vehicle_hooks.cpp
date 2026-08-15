#include "gta4_motion_bridge.h"

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

REXCVAR_DEFINE_INT32(gta4_motion_vehicle_reentry_ms, 750, "GTA IV/Motion Sensor/Tuning",
                     "Gap after which the same vehicle is treated as a new entry")
    .range(100, 5000);

namespace gta4 {
namespace {

// All guest offsets and masks in this file are checked against generated PPC by
// verify_motion_vehicle_sites.py. The bNotInAir mask is additionally
// cross-checked against CVehicleWheel::m_nFlags in the public IV SDK.
constexpr uint32_t kPrimaryReloadActionRecord = 0x82B2AD64;

constexpr uint32_t kVehicleGasPedalOffset = 4200;
constexpr uint32_t kVehicleBrakePedalOffset = 4204;
constexpr uint32_t kVehicleSteeringOffset = 4208;
constexpr uint32_t kBikePitchOffset = 5328;
constexpr uint32_t kBoatTrimOffset = 6376;
constexpr uint32_t kHelicopterBankOffset = 8100;
constexpr uint32_t kHelicopterPitchOffset = 8104;

constexpr uint32_t kAutomobileWheelsOffset = 3952;
constexpr uint32_t kAutomobileWheelCountOffset = 3956;
constexpr uint32_t kWheelFlagsOffset = 356;
constexpr uint32_t kWheelStride = 368;
constexpr uint32_t kWheelNotInAirMask = 0x00000002;
constexpr uint32_t kPrimaryUserIndex = 0;

enum class VehicleMotionKind : uint32_t {
  kHelicopter,
  kBike,
  kBoat,
  kAutomobile,
};

struct PlayerVehicleTracker {
  std::mutex mutex;
  uint32_t vehicle = 0;
  VehicleMotionKind kind = VehicleMotionKind::kAutomobile;
  std::chrono::steady_clock::time_point last_seen = {};
};

PlayerVehicleTracker g_player_vehicle_tracker;
std::atomic<uint64_t> g_motion_application_count = 0;

float LoadFloat(uint8_t* base, uint32_t address) {
  return std::bit_cast<float>(REX_LOAD_U32(address));
}

void StoreFloat(uint8_t* base, uint32_t address, float value) {
  REX_STORE_U32(address, std::bit_cast<uint32_t>(value));
}

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

bool ApplySignedAxis(uint8_t* base, uint32_t address, float motion) {
  if (!std::isfinite(motion) || motion == 0.0f) {
    return false;
  }
  const float original = LoadFloat(base, address);
  if (!std::isfinite(original) || std::abs(motion) <= std::abs(original)) {
    return false;
  }
  StoreFloat(base, address, motion);
  return true;
}

bool ApplySplitAxis(uint8_t* base, uint32_t positive_address, uint32_t negative_address,
                    float motion) {
  if (!std::isfinite(motion) || motion == 0.0f) {
    return false;
  }
  const float positive = LoadFloat(base, positive_address);
  const float negative = LoadFloat(base, negative_address);
  if (!std::isfinite(positive) || !std::isfinite(negative) ||
      std::abs(motion) <= std::max(std::abs(positive), std::abs(negative))) {
    return false;
  }
  StoreFloat(base, positive_address, motion > 0.0f ? motion : 0.0f);
  StoreFloat(base, negative_address, motion < 0.0f ? -motion : 0.0f);
  return true;
}

bool IsAutomobileAirborne(uint8_t* base, uint32_t automobile) {
  const uint32_t wheels = REX_LOAD_U32(automobile + kAutomobileWheelsOffset);
  const uint32_t wheel_count = REX_LOAD_U32(automobile + kAutomobileWheelCountOffset);
  if (!wheels || !wheel_count) {
    return false;
  }

  for (uint32_t index = 0; index < wheel_count; ++index) {
    const uint64_t wheel_address =
        static_cast<uint64_t>(wheels) + static_cast<uint64_t>(index) * kWheelStride;
    if (wheel_address > std::numeric_limits<uint32_t>::max()) {
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

void ApplyVehicleMotion(const PPCContext& entry_context, uint8_t* base, uint32_t vehicle,
                        uint32_t controller, VehicleMotionKind kind) {
  if (!vehicle || !controller || !ResolvePlayerPad(entry_context, base, controller)) {
    return;
  }

  ObservePlayerVehicle(vehicle, kind);
  auto& bridge = GTA4MotionBridge::Get();
  const MotionSnapshot motion = bridge.Read(kPrimaryUserIndex);
  if (!motion.controls_enabled || !motion.fresh) {
    return;
  }

  bool applied = false;
  switch (kind) {
    case VehicleMotionKind::kHelicopter:
      if (!bridge.IsPreferenceEnabled(MotionPreference::kHelicopter)) {
        return;
      }
      applied |= ApplySignedAxis(base, vehicle + kHelicopterBankOffset, motion.roll_axis);
      applied |= ApplySignedAxis(base, vehicle + kHelicopterPitchOffset, motion.pitch_axis);
      break;
    case VehicleMotionKind::kBike:
      if (!bridge.IsPreferenceEnabled(MotionPreference::kBike)) {
        return;
      }
      applied |= ApplySignedAxis(base, vehicle + kVehicleSteeringOffset, motion.roll_axis);
      applied |= ApplySignedAxis(base, vehicle + kBikePitchOffset, motion.pitch_axis);
      break;
    case VehicleMotionKind::kBoat:
      if (!bridge.IsPreferenceEnabled(MotionPreference::kBoat)) {
        return;
      }
      applied |= ApplySignedAxis(base, vehicle + kVehicleSteeringOffset, motion.roll_axis);
      applied |= ApplySignedAxis(base, vehicle + kBoatTrimOffset, motion.pitch_axis);
      break;
    case VehicleMotionKind::kAutomobile:
      if (!bridge.IsPreferenceEnabled(MotionPreference::kAftertouch) ||
          !IsAutomobileAirborne(base, vehicle)) {
        return;
      }
      applied |= ApplySignedAxis(base, vehicle + kVehicleSteeringOffset, motion.roll_axis);
      applied |= ApplySplitAxis(base, vehicle + kVehicleGasPedalOffset,
                                vehicle + kVehicleBrakePedalOffset, motion.pitch_axis);
      break;
  }

  if (applied) {
    LogAppliedMotion(kind, vehicle);
  }
}

void InvokeVehicleControl(PPCContext& ctx, uint8_t* base, PPCFunc* original,
                          VehicleMotionKind kind) {
  const PPCContext entry_context = ctx;
  const uint32_t vehicle = ctx.r3.u32;
  const uint32_t controller = ctx.r4.u32;
  original(ctx, base);
  ApplyVehicleMotion(entry_context, base, vehicle, controller, kind);
}

}  // namespace
}  // namespace gta4

extern "C" void sub_82163CE0(PPCContext& ctx, uint8_t* base) {
  const uint32_t action_record = ctx.r3.u32;
  __imp__sub_82163CE0(ctx, base);
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
