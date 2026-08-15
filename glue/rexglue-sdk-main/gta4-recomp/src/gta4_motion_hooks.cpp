#include "gta4_motion_bridge.h"

#include <array>
#include <bit>
#include <cstdint>
#include <mutex>
#include <string_view>

#include <rex/logging.h>
#include <rex/runtime.h>
#include <rex/system/function_dispatcher.h>

#include "gta4_init.h"

namespace gta4 {
namespace {

// GTA IV script-native context layout, verified against generated
// sub_825D2138/sub_825DE6E8 and checked by verify_motion_context_layout.py.
constexpr uint32_t kReturnStorageOffset = 0;
constexpr uint32_t kArgumentVectorOffset = 8;
constexpr std::array<uint32_t, 3> kArgumentOffsets = {0, 4, 8};
constexpr uint32_t kPrimaryUserIndex = 0;

uint32_t LoadArgument(PPCContext& ctx, uint8_t* base, size_t index) {
  const uint32_t arguments = REX_LOAD_U32(ctx.r3.u32 + kArgumentVectorOffset);
  return REX_LOAD_U32(arguments + kArgumentOffsets.at(index));
}

void StoreReturn(PPCContext& ctx, uint8_t* base, uint32_t value) {
  const uint32_t storage = REX_LOAD_U32(ctx.r3.u32 + kReturnStorageOffset);
  if (storage) {
    REX_STORE_U32(storage, value);
  }
}

void StoreBool(uint8_t* base, uint32_t storage, bool value) {
  if (storage) {
    REX_STORE_U8(storage, value ? 1 : 0);
  }
}

void StoreFloat(uint8_t* base, uint32_t storage, float value) {
  if (storage) {
    REX_STORE_U32(storage, std::bit_cast<uint32_t>(value));
  }
}

void NativeGetPadPitchRoll(PPCContext& ctx, uint8_t* base) {
  const uint32_t user_index = LoadArgument(ctx, base, 0);
  const uint32_t pitch_storage = LoadArgument(ctx, base, 1);
  const uint32_t roll_storage = LoadArgument(ctx, base, 2);

  MotionSnapshot motion = {};
  if (user_index == kPrimaryUserIndex) {
    motion = GTA4MotionBridge::Get().Read(user_index);
  }
  const bool enabled = motion.controls_enabled && motion.fresh;
  StoreFloat(base, pitch_storage, enabled ? motion.pitch_axis : 0.0f);
  StoreFloat(base, roll_storage, enabled ? motion.roll_axis : 0.0f);
  StoreReturn(ctx, base, enabled ? 1 : 0);
}

void NativeGetMotionControlsEnabled(PPCContext& ctx, uint8_t* base) {
  const MotionSnapshot motion = GTA4MotionBridge::Get().Read(kPrimaryUserIndex);
  const bool enabled = motion.controls_enabled && motion.fresh;

  // The generated 360 stub proves two BOOL output pointers but does not retain
  // their PS3 semantic names. Returning the same availability state in both is
  // order-independent and matches the native's enable/disable contract.
  StoreBool(base, LoadArgument(ctx, base, 0), enabled);
  StoreBool(base, LoadArgument(ctx, base, 1), enabled);
}

void NativeHasReloadedWithMotionControl(PPCContext& ctx, uint8_t* base) {
  const uint32_t user_index = LoadArgument(ctx, base, 0);
  const uint32_t reloaded_storage = LoadArgument(ctx, base, 1);

  bool enabled = false;
  bool reloaded = false;
  if (user_index == kPrimaryUserIndex) {
    const MotionSnapshot motion = GTA4MotionBridge::Get().Read(user_index);
    enabled = motion.controls_enabled && motion.fresh &&
              GTA4MotionBridge::Get().IsPreferenceEnabled(MotionPreference::kReload);
    if (enabled) {
      reloaded =
          GTA4MotionBridge::Get().ConsumeReloadGesture(MotionReloadConsumer::kScript, user_index);
    }
  }
  StoreBool(base, reloaded_storage, reloaded);
  StoreReturn(ctx, base, enabled ? 1 : 0);
}

void NativeSetAllMotionControlPreferences(PPCContext& ctx, uint8_t* base) {
  GTA4MotionBridge::Get().SetAllPreferences(LoadArgument(ctx, base, 0) != 0);
}

void NativeGetMotionControlPreference(PPCContext& ctx, uint8_t* base) {
  const uint32_t preference = LoadArgument(ctx, base, 0);
  const MotionSnapshot motion = GTA4MotionBridge::Get().Read(kPrimaryUserIndex);
  const bool enabled = motion.controls_enabled && motion.fresh &&
                       GTA4MotionBridge::Get().IsPreferenceEnabled(preference);
  StoreReturn(ctx, base, enabled ? 1 : 0);
}

struct MotionNativeRegistration {
  uint32_t return_address;
  uint32_t name_address;
  uint32_t original_handler;
  std::string_view name;
  PPCFunc* replacement;
  uint32_t thunk_address = 0;
};

// Addresses are derived from the generated sub_825D2158 body by
// verify_motion_native_sites.py. Every match also checks the live guest string,
// original handler, and unique registration return address before replacing it.
std::array<MotionNativeRegistration, 5> g_motion_native_registrations = {{
    {0x825D2344, 0x82037240, 0x825DE6E8, "GET_PAD_PITCH_ROLL", NativeGetPadPitchRoll},
    {0x825D2358, 0x82037224, 0x825D2138, "GET_MOTION_CONTROLS_ENABLED",
     NativeGetMotionControlsEnabled},
    {0x825D236C, 0x82037200, 0x825DE6E8, "HAS_RELOADED_WITH_MOTION_CONTROL",
     NativeHasReloadedWithMotionControl},
    {0x825D2380, 0x820371D4, 0x822BCA90, "SET_ALL_MOTION_CONTROL_PREFERENCES_ON_OFF",
     NativeSetAllMotionControlPreferences},
    {0x825D2394, 0x820371B4, 0x825DE6E8, "GET_MOTION_CONTROL_PREFERENCE",
     NativeGetMotionControlPreference},
}};

std::mutex g_motion_registration_mutex;

bool GuestNameMatches(uint8_t* base, uint32_t address, std::string_view expected) {
  return REX_LOAD_STRING(address, expected.size()) == expected &&
         REX_LOAD_U8(address + static_cast<uint32_t>(expected.size())) == 0;
}

MotionNativeRegistration* FindRegistration(uint32_t return_address) {
  for (auto& registration : g_motion_native_registrations) {
    if (registration.return_address == return_address) {
      return &registration;
    }
  }
  return nullptr;
}

}  // namespace
}  // namespace gta4

extern "C" void sub_82845600(PPCContext& ctx, uint8_t* base) {
  const uint32_t return_address = static_cast<uint32_t>(ctx.lr);
  auto* registration = gta4::FindRegistration(return_address);
  if (!registration) {
    __imp__sub_82845600(ctx, base);
    return;
  }

  const bool exact_match = ctx.r3.u32 == registration->name_address &&
                           ctx.r4.u32 == registration->original_handler &&
                           gta4::GuestNameMatches(base, ctx.r3.u32, registration->name);
  if (!exact_match) {
    REXLOG_ERROR(
        "gta4-motion: native registration invariant failed at {:08X}: "
        "name={:08X} handler={:08X} expected_name={:08X} expected_handler={:08X}",
        return_address, ctx.r3.u32, ctx.r4.u32, registration->name_address,
        registration->original_handler);
    __imp__sub_82845600(ctx, base);
    return;
  }

  {
    std::lock_guard lock(gta4::g_motion_registration_mutex);
    if (!registration->thunk_address) {
      auto* runtime = rex::Runtime::instance();
      auto* dispatcher = runtime ? runtime->function_dispatcher() : nullptr;
      registration->thunk_address =
          dispatcher ? dispatcher->AllocateThunk(registration->replacement, return_address) : 0;
      if (registration->thunk_address) {
        REXLOG_INFO("gta4-motion: restored native {} through thunk {:08X}", registration->name,
                    registration->thunk_address);
      } else {
        REXLOG_ERROR("gta4-motion: failed to allocate thunk for {}", registration->name);
      }
    }
    if (registration->thunk_address) {
      ctx.r4.u32 = registration->thunk_address;
    }
  }

  __imp__sub_82845600(ctx, base);
}
