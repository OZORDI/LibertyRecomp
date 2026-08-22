#include "../../glue/rexglue-sdk-main/gta4-recomp/generated/gta4_init.h"

#include <rex/input/mnk/encoded_action.h>
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
  const uint32_t action_address =
      control + kActionArrayOffset + index * kActionStride;
  const uint32_t encoded_value_address =
      action_address + kActionNewStateOffset;
  const uint8_t polarity = LoadGuestU8(base, action_address);
  const uint8_t encoded_value = LoadGuestU8(base, encoded_value_address);
  const uint8_t merged_value = rex::input::mnk::MergeActionMagnitude(
      polarity, encoded_value, value);
  if (merged_value != encoded_value) {
    StoreGuestU8(base, encoded_value_address, merged_value);
  }
}

struct KeyActionBinding {
  VirtualKey key;
  GTA4Action action;
};

// One declarative keyboard map is the sole keyboard-to-title conversion path.
// Multiple entries for a key are intentional: GTA IV consumes different
// action families for on-foot, vehicle, aircraft and frontend contexts.
constexpr KeyActionBinding kKeyActionBindings[] = {
    {VirtualKey::kW, GTA4Action::kMoveUp},
    {VirtualKey::kW, GTA4Action::kVehicleMoveUp},
    {VirtualKey::kW, GTA4Action::kVehicleAccelerate},
    {VirtualKey::kW, GTA4Action::kVehicleFlyThrottleUp},
    {VirtualKey::kW, GTA4Action::kFrontendUp},
    {VirtualKey::kS, GTA4Action::kMoveDown},
    {VirtualKey::kS, GTA4Action::kVehicleMoveDown},
    {VirtualKey::kS, GTA4Action::kVehicleBrake},
    {VirtualKey::kS, GTA4Action::kVehicleFlyThrottleDown},
    {VirtualKey::kS, GTA4Action::kFrontendDown},
    {VirtualKey::kA, GTA4Action::kMoveLeft},
    {VirtualKey::kA, GTA4Action::kVehicleMoveLeft},
    {VirtualKey::kA, GTA4Action::kVehicleFlyYawLeft},
    {VirtualKey::kA, GTA4Action::kFrontendLeft},
    {VirtualKey::kD, GTA4Action::kMoveRight},
    {VirtualKey::kD, GTA4Action::kVehicleMoveRight},
    {VirtualKey::kD, GTA4Action::kVehicleFlyYawRight},
    {VirtualKey::kD, GTA4Action::kFrontendRight},
    {VirtualKey::kShift, GTA4Action::kSprint},
    {VirtualKey::kSpace, GTA4Action::kJump},
    {VirtualKey::kSpace, GTA4Action::kVehicleHandbrake},
    {VirtualKey::kSpace, GTA4Action::kFrontendAccept},
    {VirtualKey::kF, GTA4Action::kEnter},
    {VirtualKey::kF, GTA4Action::kVehicleExit},
    {VirtualKey::kR, GTA4Action::kReload},
    {VirtualKey::kQ, GTA4Action::kCover},
    {VirtualKey::kE, GTA4Action::kPickup},
    {VirtualKey::kC, GTA4Action::kDuck},
    {VirtualKey::kC, GTA4Action::kLookBehind},
    {VirtualKey::kC, GTA4Action::kVehicleLookBehind},
    {VirtualKey::kV, GTA4Action::kNextCamera},
    {VirtualKey::kV, GTA4Action::kVehicleCinematicCamera},
    {VirtualKey::kH, GTA4Action::kVehicleHeadlight},
    {VirtualKey::kG, GTA4Action::kVehicleHorn},
    {VirtualKey::kTab, GTA4Action::kZoomRadar},
    {VirtualKey::kUp, GTA4Action::kPhoneTakeOut},
    {VirtualKey::kUp, GTA4Action::kFrontendUp},
    {VirtualKey::kDown, GTA4Action::kPhonePutAway},
    {VirtualKey::kDown, GTA4Action::kFrontendDown},
    {VirtualKey::kLeft, GTA4Action::kFrontendLeft},
    {VirtualKey::kRight, GTA4Action::kFrontendRight},
    {VirtualKey::kReturn, GTA4Action::kFrontendAccept},
    {VirtualKey::kBack, GTA4Action::kPhonePutAway},
    {VirtualKey::kBack, GTA4Action::kFrontendCancel},
    {VirtualKey::kEscape, GTA4Action::kFrontendPause},
    {VirtualKey::kEscape, GTA4Action::kFrontendCancel},
    {VirtualKey::kLButton, GTA4Action::kAttack},
    {VirtualKey::kLButton, GTA4Action::kVehicleAttack},
    {VirtualKey::kLButton, GTA4Action::kMeleeAttack1},
    {VirtualKey::kLButton, GTA4Action::kFrontendAccept},
    {VirtualKey::kRButton, GTA4Action::kAim},
    {VirtualKey::kRButton, GTA4Action::kVehicleAttack2},
    {VirtualKey::kRButton, GTA4Action::kMeleeBlock},
    {VirtualKey::kRButton, GTA4Action::kFrontendCancel},
};

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

  for (const KeyActionBinding& binding : kKeyActionBindings) {
    if (IsDown(state, binding.key)) {
      MergeAction(base, control, binding.action, kPressed);
    }
  }

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
