#include "../../glue/rexglue-sdk-main/gta4-recomp/generated/gta4_init.h"

#include <rex/input/mnk/mnk_input_driver.h>
#include <rex/input/mnk/pointer_motion.h>
#include <rex/ui/virtual_key.h>

#include <algorithm>
#include <bit>
#include <cstdint>

namespace {

using rex::input::mnk::NativeInputState;
using rex::input::mnk::MouseAxisQuantizer;
using rex::ui::VirtualKey;

enum class GTA4Action : uint32_t {
  kNextCamera = 0,
  kSprint = 1,
  kJump = 2,
  kEnter = 3,
  kAttack = 4,
  kAim = 6,
  kLookBehind = 7,
  kNextWeapon = 8,
  kPrevWeapon = 9,
  kMoveLeft = 12,
  kMoveRight = 13,
  kMoveUp = 14,
  kMoveDown = 15,
  kLookLeft = 16,
  kLookRight = 17,
  kLookUp = 18,
  kLookDown = 19,
  kDuck = 20,
  kPhoneTakeOut = 21,
  kPhonePutAway = 22,
  kPickup = 23,
  kSniperZoomIn = 24,
  kSniperZoomOut = 25,
  kCover = 28,
  kReload = 29,
  kVehicleMoveLeft = 30,
  kVehicleMoveRight = 31,
  kVehicleMoveUp = 32,
  kVehicleMoveDown = 33,
  kVehicleGunLeft = 34,
  kVehicleGunRight = 35,
  kVehicleGunUp = 36,
  kVehicleGunDown = 37,
  kVehicleAttack = 38,
  kVehicleAttack2 = 39,
  kVehicleAccelerate = 40,
  kVehicleBrake = 41,
  kVehicleHeadlight = 42,
  kVehicleExit = 43,
  kVehicleHandbrake = 44,
  kVehicleLookLeft = 48,
  kVehicleLookRight = 49,
  kVehicleLookBehind = 50,
  kVehicleCinematicCamera = 51,
  kVehicleNextRadio = 52,
  kVehiclePrevRadio = 53,
  kVehicleHorn = 54,
  kVehicleFlyThrottleUp = 55,
  kVehicleFlyThrottleDown = 56,
  kVehicleFlyYawLeft = 57,
  kVehicleFlyYawRight = 58,
  kMeleeAttack1 = 59,
  kMeleeBlock = 63,
  kFrontendDown = 64,
  kFrontendUp = 65,
  kFrontendLeft = 66,
  kFrontendRight = 67,
  kFrontendPause = 76,
  kFrontendAccept = 77,
  kFrontendCancel = 78,
  kZoomRadar = 86,
};

constexpr uint32_t kActionArrayOffset = 2328;
constexpr uint32_t kActionStride = 12;
constexpr uint32_t kActionNewStateOffset = 2;
constexpr uint32_t kLastInputTimeOffset = 4200;
constexpr uint32_t kGameInputTimeAddress = 0x82C6C2A4;
constexpr uint32_t kGameplayTimeStepAddress = 0x82C6C2AC;
constexpr uint8_t kPressed = 255;
// Generated and verified by tools/generate_mouse_input_constants.py.
constexpr double kReferenceFrameSeconds = 0x1.1111120000000p-5;
constexpr double kMouseUnitsPerCount = 0x1.8000000000000p+3;

MouseAxisQuantizer gMouseXQuantizer;
MouseAxisQuantizer gMouseYQuantizer;
rex::ui::MouseEvent::MotionSource gLastMouseSource =
    rex::ui::MouseEvent::MotionSource::kGeneric;
uint64_t gLastMouseResetGeneration = 0;
bool gMouseConversionInitialized = false;

bool IsDown(const NativeInputState& state, VirtualKey key) {
  const auto index = static_cast<uint16_t>(key);
  return index < state.keys.size() && state.keys[index] != 0;
}

uint8_t LoadGuestU8(uint8_t* base, uint32_t address) {
  return *reinterpret_cast<volatile uint8_t*>(base + address);
}

uint32_t LoadGuestU32(uint8_t* base, uint32_t address) {
  const uint32_t value = *reinterpret_cast<volatile uint32_t*>(base + address);
  return __builtin_bswap32(value);
}

float LoadGuestFloat(uint8_t* base, uint32_t address) {
  return std::bit_cast<float>(LoadGuestU32(base, address));
}

void StoreGuestU8(uint8_t* base, uint32_t address, uint8_t value) {
  *reinterpret_cast<volatile uint8_t*>(base + address) = value;
}

void StoreGuestU32(uint8_t* base, uint32_t address, uint32_t value) {
  *reinterpret_cast<volatile uint32_t*>(base + address) = __builtin_bswap32(value);
}

void MergeAction(uint8_t* base, uint32_t control, GTA4Action action, uint8_t value) {
  if (value == 0) {
    return;
  }

  const uint32_t index = static_cast<uint32_t>(action);
  const uint32_t address =
      control + kActionArrayOffset + index * kActionStride + kActionNewStateOffset;
  if (value > LoadGuestU8(base, address)) {
    StoreGuestU8(base, address, value);
  }
}

void MergeKey(uint8_t* base, uint32_t control, const NativeInputState& state, VirtualKey key,
              GTA4Action action) {
  if (IsDown(state, key)) {
    MergeAction(base, control, action, kPressed);
  }
}

void MergeMouseAxis(uint8_t* base, uint32_t control, int32_t value, GTA4Action negative,
                    GTA4Action positive) {
  if (value < 0) {
    MergeAction(base, control, negative, static_cast<uint8_t>(-value));
  } else if (value > 0) {
    MergeAction(base, control, positive, static_cast<uint8_t>(value));
  }
}

void ResetMouseConversion() {
  gMouseXQuantizer.Reset();
  gMouseYQuantizer.Reset();
}

bool HasNativeActivity(const NativeInputState& state) {
  if (state.mouse_dx != 0 || state.mouse_dy != 0 || state.mouse_wheel != 0) {
    return true;
  }
  return std::any_of(state.keys.begin(), state.keys.end(), [](uint8_t value) {
    return value != 0;
  });
}

void InjectNativeInput(uint8_t* base, uint32_t control, const NativeInputState& state) {
  if (!gMouseConversionInitialized ||
      state.mouse_reset_generation != gLastMouseResetGeneration) {
    ResetMouseConversion();
    gLastMouseResetGeneration = state.mouse_reset_generation;
    gMouseConversionInitialized = true;
  }
  if (state.mouse_has_motion && state.mouse_source != gLastMouseSource) {
    ResetMouseConversion();
    gLastMouseSource = state.mouse_source;
  }

  // Movement is written to every applicable gameplay context. GTA IV's active
  // task decides whether the on-foot, vehicle, or aircraft action is consumed.
  MergeKey(base, control, state, VirtualKey::kW, GTA4Action::kMoveUp);
  MergeKey(base, control, state, VirtualKey::kW, GTA4Action::kVehicleMoveUp);
  MergeKey(base, control, state, VirtualKey::kW, GTA4Action::kVehicleAccelerate);
  MergeKey(base, control, state, VirtualKey::kW, GTA4Action::kVehicleFlyThrottleUp);
  MergeKey(base, control, state, VirtualKey::kW, GTA4Action::kFrontendUp);

  MergeKey(base, control, state, VirtualKey::kS, GTA4Action::kMoveDown);
  MergeKey(base, control, state, VirtualKey::kS, GTA4Action::kVehicleMoveDown);
  MergeKey(base, control, state, VirtualKey::kS, GTA4Action::kVehicleBrake);
  MergeKey(base, control, state, VirtualKey::kS, GTA4Action::kVehicleFlyThrottleDown);
  MergeKey(base, control, state, VirtualKey::kS, GTA4Action::kFrontendDown);

  MergeKey(base, control, state, VirtualKey::kA, GTA4Action::kMoveLeft);
  MergeKey(base, control, state, VirtualKey::kA, GTA4Action::kVehicleMoveLeft);
  MergeKey(base, control, state, VirtualKey::kA, GTA4Action::kVehicleFlyYawLeft);
  MergeKey(base, control, state, VirtualKey::kA, GTA4Action::kFrontendLeft);

  MergeKey(base, control, state, VirtualKey::kD, GTA4Action::kMoveRight);
  MergeKey(base, control, state, VirtualKey::kD, GTA4Action::kVehicleMoveRight);
  MergeKey(base, control, state, VirtualKey::kD, GTA4Action::kVehicleFlyYawRight);
  MergeKey(base, control, state, VirtualKey::kD, GTA4Action::kFrontendRight);

  MergeKey(base, control, state, VirtualKey::kShift, GTA4Action::kSprint);
  MergeKey(base, control, state, VirtualKey::kSpace, GTA4Action::kJump);
  MergeKey(base, control, state, VirtualKey::kSpace, GTA4Action::kVehicleHandbrake);
  MergeKey(base, control, state, VirtualKey::kSpace, GTA4Action::kFrontendAccept);
  MergeKey(base, control, state, VirtualKey::kF, GTA4Action::kEnter);
  MergeKey(base, control, state, VirtualKey::kF, GTA4Action::kVehicleExit);
  MergeKey(base, control, state, VirtualKey::kR, GTA4Action::kReload);
  MergeKey(base, control, state, VirtualKey::kQ, GTA4Action::kCover);
  MergeKey(base, control, state, VirtualKey::kE, GTA4Action::kPickup);
  MergeKey(base, control, state, VirtualKey::kC, GTA4Action::kDuck);
  MergeKey(base, control, state, VirtualKey::kC, GTA4Action::kLookBehind);
  MergeKey(base, control, state, VirtualKey::kC, GTA4Action::kVehicleLookBehind);
  MergeKey(base, control, state, VirtualKey::kV, GTA4Action::kNextCamera);
  MergeKey(base, control, state, VirtualKey::kV, GTA4Action::kVehicleCinematicCamera);
  MergeKey(base, control, state, VirtualKey::kH, GTA4Action::kVehicleHeadlight);
  MergeKey(base, control, state, VirtualKey::kG, GTA4Action::kVehicleHorn);
  MergeKey(base, control, state, VirtualKey::kTab, GTA4Action::kZoomRadar);

  MergeKey(base, control, state, VirtualKey::kUp, GTA4Action::kPhoneTakeOut);
  MergeKey(base, control, state, VirtualKey::kUp, GTA4Action::kFrontendUp);
  MergeKey(base, control, state, VirtualKey::kDown, GTA4Action::kPhonePutAway);
  MergeKey(base, control, state, VirtualKey::kDown, GTA4Action::kFrontendDown);
  MergeKey(base, control, state, VirtualKey::kLeft, GTA4Action::kFrontendLeft);
  MergeKey(base, control, state, VirtualKey::kRight, GTA4Action::kFrontendRight);
  MergeKey(base, control, state, VirtualKey::kReturn, GTA4Action::kFrontendAccept);
  MergeKey(base, control, state, VirtualKey::kBack, GTA4Action::kPhonePutAway);
  MergeKey(base, control, state, VirtualKey::kBack, GTA4Action::kFrontendCancel);
  MergeKey(base, control, state, VirtualKey::kEscape, GTA4Action::kFrontendPause);
  MergeKey(base, control, state, VirtualKey::kEscape, GTA4Action::kFrontendCancel);

  MergeKey(base, control, state, VirtualKey::kLButton, GTA4Action::kAttack);
  MergeKey(base, control, state, VirtualKey::kLButton, GTA4Action::kVehicleAttack);
  MergeKey(base, control, state, VirtualKey::kLButton, GTA4Action::kMeleeAttack1);
  MergeKey(base, control, state, VirtualKey::kLButton, GTA4Action::kFrontendAccept);
  MergeKey(base, control, state, VirtualKey::kRButton, GTA4Action::kAim);
  MergeKey(base, control, state, VirtualKey::kRButton, GTA4Action::kVehicleAttack2);
  MergeKey(base, control, state, VirtualKey::kRButton, GTA4Action::kMeleeBlock);
  MergeKey(base, control, state, VirtualKey::kRButton, GTA4Action::kFrontendCancel);

  if (state.mouse_wheel > 0) {
    MergeAction(base, control, GTA4Action::kNextWeapon, kPressed);
    MergeAction(base, control, GTA4Action::kSniperZoomIn, kPressed);
    MergeAction(base, control, GTA4Action::kVehicleNextRadio, kPressed);
  } else if (state.mouse_wheel < 0) {
    MergeAction(base, control, GTA4Action::kPrevWeapon, kPressed);
    MergeAction(base, control, GTA4Action::kSniperZoomOut, kPressed);
    MergeAction(base, control, GTA4Action::kVehiclePrevRadio, kPressed);
  }

  if (state.mouse_has_motion) {
    const double frame_seconds =
        static_cast<double>(LoadGuestFloat(base, kGameplayTimeStepAddress));
    const int32_t mouse_x = gMouseXQuantizer.Quantize(
        state.mouse_dx, state.mouse_sensitivity, kMouseUnitsPerCount, frame_seconds,
        kReferenceFrameSeconds);
    const double mouse_y_delta = state.invert_mouse_y ? -state.mouse_dy : state.mouse_dy;
    const int32_t mouse_y = gMouseYQuantizer.Quantize(
        mouse_y_delta, state.mouse_sensitivity, kMouseUnitsPerCount, frame_seconds,
        kReferenceFrameSeconds);

    MergeMouseAxis(base, control, mouse_x, GTA4Action::kLookLeft, GTA4Action::kLookRight);
    MergeMouseAxis(base, control, mouse_x, GTA4Action::kVehicleGunLeft,
                   GTA4Action::kVehicleGunRight);
    MergeMouseAxis(base, control, mouse_x, GTA4Action::kVehicleLookLeft,
                   GTA4Action::kVehicleLookRight);
    MergeMouseAxis(base, control, mouse_y, GTA4Action::kLookUp, GTA4Action::kLookDown);
    MergeMouseAxis(base, control, mouse_y, GTA4Action::kVehicleGunUp,
                   GTA4Action::kVehicleGunDown);
  }

  if (HasNativeActivity(state)) {
    StoreGuestU32(base, control + kLastInputTimeOffset,
                  LoadGuestU32(base, kGameInputTimeAddress));
  }
}

}  // namespace

extern "C" void sub_822B7DD0(PPCContext& ctx, uint8_t* base) {
  const uint32_t control = ctx.r3.u32;
  __imp__sub_822B7DD0(ctx, base);

  NativeInputState state;
  if (control != 0 && rex::input::mnk::ConsumeNativeInputState(&state)) {
    InjectNativeInput(base, control, state);
  }
}
