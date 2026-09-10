#include "gta4_init.h"
#include "gta4_frontend_hooks.h"
#include "gta4_input_action_routing.h"
#include "gta4_map_pan_policy.h"
#include "gta4_pc_input_bridge.h"
#include "gta4_vehicle_weapon_policy.h"
#include "gta4_vehicle_mouse_policy.h"
#include "gta4_touch_coordinator.h"
#include "gta4_motion_bridge.h"
#include "gta4_gyro_aim_policy.h"
#include "input/context_touch_controls.h"
#include "input/user_music_player.h"

#include <rex/cvar.h>
#include <rex/input/input.h>
#include <rex/input/absolute_pointer.h>
#include <rex/input/input_system.h>
#include <rex/input/mnk/encoded_action.h>
#include <rex/input/mnk/mnk_input_driver.h>
#include <rex/input/mnk/pointer_motion.h>
#include <rex/input/input_trace.h>
#include <rex/logging.h>
#include <rex/runtime.h>
#include <rex/ui/virtual_key.h>

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <unordered_map>

REXCVAR_DEFINE_BOOL(gta4_native_input_trace, false, "GTA IV/Input",
                    "Trace native keyboard/mouse poll epochs and action injection");
REXCVAR_DEFINE_BOOL(gta4_motion_aim, false, "GTA IV/Motion Sensor",
                    "Enable gyroscope fine aiming when aiming on foot");
REXCVAR_DEFINE_DOUBLE(gta4_motion_aim_full_scale, 2.0, "GTA IV/Motion Sensor/Tuning",
                      "Gyroscope radians per second for full aim input").range(0.1, 20.0);
REXCVAR_DEFINE_BOOL(gta4_motion_aim_invert_x, false, "GTA IV/Motion Sensor/Tuning", "Invert gyro aim X");
REXCVAR_DEFINE_BOOL(gta4_motion_aim_invert_y, false, "GTA IV/Motion Sensor/Tuning", "Invert gyro aim Y");

namespace gta4::input {
namespace {

using rex::input::mnk::MouseAxisQuantizer;
using rex::input::mnk::NativeInputState;
using rex::ui::VirtualKey;

enum class Action : uint32_t {
  kNextCamera = 0,
  kSprint = 1,
  kJump = 2,
  kEnter = 3,
  kAttack = 4,
  kAttack2 = 5,
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
  kSniperZoomInAlternate = 26,
  kSniperZoomOutAlternate = 27,
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
  // The retail game reuses this record in context-sensitive vehicle paths,
  // including secondary weapon fire. It must not share Space/handbrake.
  kVehicleContextAction45 = 45,
  kVehicleHotwireLeft = 46,
  kVehicleHotwireRight = 47,
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
  kMeleeAttack2 = 60,
  kMeleeAttack3 = 61,
  kMeleeKick = 62,
  kMeleeBlock = 63,
  kFrontendDown = 64,
  kFrontendUp = 65,
  kFrontendLeft = 66,
  kFrontendRight = 67,
  kFrontendScrollY = 75,
  kFrontendPause = 76,
  kFrontendAccept = 77,
  kFrontendCancel = 78,
  kFrontendX = 79,
  kFrontendY = 80,
  kFrontendLeftShoulder = 81,
  kFrontendRightShoulder = 82,
  kFrontendLeftTrigger = 83,
  kFrontendRightTrigger = 84,
  kMeleeAttack4 = 85,
  kZoomRadar = 86,
  kMapX = 72,
  kMapY = 73,
};

constexpr Action kFrontendButtonActions[] = {
    Action::kFrontendDown, Action::kFrontendUp,
    Action::kFrontendLeft, Action::kFrontendRight,
    Action::kFrontendPause, Action::kFrontendAccept, Action::kFrontendCancel,
    Action::kFrontendX, Action::kFrontendY,
    Action::kFrontendLeftShoulder, Action::kFrontendRightShoulder,
    Action::kFrontendLeftTrigger, Action::kFrontendRightTrigger,
};
constexpr Action kPhoneButtonActions[] = {
    Action::kPhoneTakeOut, Action::kPhonePutAway,
};

// These values are recovered from generated sub_822B7DD0/sub_822B7CD8 and
// sub_821B4768. tools/verify_gta4_input_layout.py verifies all derived offsets.
constexpr uint32_t kActionArrayOffset = 2328;
constexpr uint32_t kActionStride = 12;
constexpr uint32_t kActionCurrentOffset = 2;
constexpr uint32_t kActionPreviousOffset = 3;
// sub_822B7DD0 caches the previous signed action-75 value here, and
// sub_8224FFC8 consumes it for right-stick vertical scroll edge events.
constexpr uint32_t kFrontendScrollPreviousOffset = 4212;
constexpr uint32_t kControlUserIndexOffset = 3412;
constexpr uint32_t kLastInputTimeOffset = 4200;
// Generated sub_828CC9D0 converts the XInput state into one of four retail
// controller records before native PC actions are merged. The record geometry
// and addresses are independently checked by
// /tmp/verify_controller_record_layout.py.
constexpr uint32_t kRetailControllerRecordsAddress = 0x831C4FF8;
constexpr uint32_t kRetailControllerRecordStride = 56;
constexpr uint32_t kRetailControllerRecordCount = 4;
constexpr uint32_t kRetailControllerFlagsOffset = 4;
constexpr uint32_t kRetailControllerAFlag = 0x40;
constexpr uint32_t kRetailControllerStartFlag = 0x800;
constexpr uint32_t kRetailControllerDpadUpFlag = 0x1000;
constexpr uint32_t kRetailControllerDpadDownFlag = 0x4000;
constexpr uint32_t kRetailControllerDpadLeftFlag = 0x8000;
constexpr uint32_t kRetailControllerDpadRightFlag = 0x2000;
constexpr uint32_t kGameInputTimeAddress = 0x82C6C2A4;
constexpr uint32_t kGameplayTimeStepAddress = 0x82C6C2AC;
constexpr uint32_t kCurrentScreenAddress = 0x82BFA124;
constexpr uint32_t kMapScreen = 3;
constexpr uint32_t kMapZoomLevelAddress = 0x82BF9D88;
constexpr uint32_t kMapZoomSettledAddress = 0x82BF9D8C;
constexpr uint32_t kMapZoomMinimum = 0;
constexpr uint32_t kMapZoomMaximum = 5;
// Generated sub_8224FFC8 reads and clears this one-shot frontend refresh flag.
// The address is independently derived by tools/verify_gta4_input_layout.py.
constexpr uint32_t kFrontendOneShotFlagAddress = 0x82BFA129;
// Registered IS_PAUSE_MENU_ACTIVE native sub_825DAA40, via sub_825DD3E8.
constexpr uint32_t kPauseMenuTransitionAddress = 0x82BFA13C;
constexpr uint32_t kPauseMenuVisibleAddress = 0x82BFA144;
// The registered CAN_PHONE_BE_SEEN_ON_SCREEN native (sub_8217D3E0) loads the
// active phone render object through this index/table pair, then evaluates
// sub_821C2FA8. That retail leaf returns the object's byte-17 hidden state;
// CAN_PHONE_BE_SEEN_ON_SCREEN returns its inverse. The absolute addresses are
// checked against generated instructions by verify_gta4_keyboard_consumers.py.
constexpr uint32_t kPhoneRenderIndexAddress = 0x82B3A0F0;
constexpr uint32_t kPhoneRenderObjectTableAddress = 0x82B39990;
// The retail data definition reserves 0x100 bytes at dword_82B39990 before
// dword_82B39A90. /tmp/phone_table_geometry.py derives 64 guest pointers.
// Retail callers index the initialized value directly; this poll hook runs
// more broadly, so reject an out-of-range startup/corruption value first.
constexpr uint32_t kPhoneRenderObjectCount = 64;
constexpr uint32_t kPhoneHiddenStateOffset = 17;
// CREATE_MOBILE_PHONE (sub_8217C958) sets this byte and
// DESTROY_MOBILE_PHONE (sub_82146BE8) clears it. The persistent HUD widget
// queried by CAN_PHONE_BE_SEEN_ON_SCREEN is not proof that a phone exists.
// Generated sub_823CA860/sub_823CE3A8 additionally reject the offscreen state
// written by SCRIPT_IS_MOVING_MOBILE_PHONE_OFFSCREEN (sub_8217D3C0).
constexpr uint32_t kPhoneCreatedAddress = 0x831D4DD4;
constexpr uint32_t kPhoneMovingOffscreenAddress = 0x831D534C;
constexpr uint32_t kPhoneConsumerDisableFlagAOffset = 528;
constexpr uint32_t kPhoneConsumerDisableFlagBOffset = 529;
constexpr uint32_t kPhoneConsumerStateOffset = 640;
constexpr uint32_t kCurrentPlayerIndexAddress = 0x82A98778;
constexpr uint32_t kPlayerInfoTableAddress = 0x82C01C70;
constexpr uint32_t kPlayerInfoPedOffset = 1400;
constexpr uint32_t kPedVehicleFlagsOffset = 572;
constexpr uint32_t kPedVehicleOffset = 2688;
constexpr uint32_t kVehicleDriverOffset = 3904;
// IS_CHAR_IN_ANY_HELI (sub_825C4400) tests the retail class kind, including
// derived helicopter models, rather than one particular vtable address.
constexpr uint32_t kVehicleClassOffset = 4836;
constexpr uint32_t kHelicopterVehicleClass = 4;
// SET_PLAYER_CAN_DROP_WEAPONS_IN_CAR writes this byte. Generated
// sub_8237FC98/sub_82384518 read it after the action-42 trigger and before
// executing the GTA Race drop-weapon operation.
constexpr uint32_t kPlayerCanDropWeaponsInCarAddress = 0x82BD42E8;
constexpr uint32_t kRadioEntityVehicleOffset = 40;
constexpr uint32_t kRadioEntityStateOffset = 108;
constexpr uint32_t kRadioEntityStationOffset = 110;
constexpr uint32_t kPedInVehicleFlag = 0x20000000;
constexpr uint32_t kMaximumLocalPlayers = 4;
constexpr uint32_t kGuestPointerSize = 4;
constexpr uint8_t kPressed = 255;
constexpr int32_t kFullNegative = -255;
constexpr int32_t kFullPositive = 255;
constexpr double kReferenceFrameSeconds = 0x1.1111120000000p-5;
constexpr double kMouseUnitsPerCount = 0x1.8000000000000p+3;
constexpr uint32_t kDirectWeaponPredicateCaller = 0x823CFD54;
constexpr uint32_t kVehicleWeaponPressPredicateCaller = 0x823CFB28;
constexpr uint32_t kVehicleWeaponReleasePredicateCaller = 0x823CFAF0;
constexpr uint32_t kDirectWeaponSelectionCaller = 0x823CFD94;
constexpr uint32_t kRadioOffPredicateCaller = 0x822D4A18;
constexpr uint32_t kPedWeaponManagerOffset = 640;
// rage::scrThread* is installed here only while the interpreter is executing
// a native call. The script key and shared-global layout are recovered from
// generated sub_82844200 and the executable parachute_player SCO.
constexpr uint32_t kExecutingScriptThreadAddress = 0x8319277C;
constexpr uint32_t kScriptGlobalsAddress = 0x831927B4;
constexpr uint32_t kScriptProgramKeyOffset = 8;
constexpr uint32_t kParachutePlayerProgramKey = 0x98751695;
constexpr uint32_t kParachuteStateOffset = 0x2A18;
constexpr uint32_t kParachuteFreefallState = 3;
constexpr uint32_t kParachuteDeployedState = 5;
constexpr uint32_t kParachuteDeployAction = 1;
constexpr uint32_t kParachuteDetachAction = 3;
constexpr uint32_t kParachuteLeftBrakeAction = 4;
constexpr uint32_t kParachuteRightBrakeAction = 6;
constexpr uint32_t kParachutePcRightBrakeAction = 137;
constexpr uint32_t kParachutePcLeftBrakeAction = 138;
constexpr uint32_t kParachuteSmokeAction = 51;
constexpr uint32_t kParachuteSmokeButton = 17;
constexpr int32_t kAnalogueNegativeExtent = -128;
constexpr int32_t kAnaloguePositiveExtent = 127;

struct DirectWeaponRequest {
  uint64_t epoch = 0;
  uint32_t user = 0;
  uint32_t ped = 0;
  uint32_t slot = 0;
  KeyboardWeaponRequest request = KeyboardWeaponRequest::kDirect;
  bool in_vehicle = false;
  bool armed = false;
  bool predicate_forced = false;
};

struct RadioOffRequest {
  uint64_t epoch = 0;
  uint32_t user = 0;
  uint32_t ped = 0;
  uint32_t vehicle = 0;
  bool armed = false;
};

struct InputEpoch {
  NativeInputState state{};
  std::array<uint8_t, 256> pressed_keys{};
  std::array<uint8_t, 256> changed_keys{};
  uint16_t gamepad_buttons = 0;
  uint32_t gamepad_packet = 0;
  int32_t mouse_x = 0;
  int32_t mouse_y = 0;
  int32_t gyro_x = 0;
  int32_t gyro_y = 0;
  int32_t map_mouse_x = 0;
  int32_t map_mouse_y = 0;
  uint64_t sequence = 0;
  uint32_t poll_caller = 0;
  uint32_t phone_render_index = 0;
  uint32_t phone_render_object = 0;
  bool phone_created = false;
  bool phone_moving_offscreen = false;
  bool phone_render_visible = false;
  uint32_t pause_menu_transition = 0;
  bool pause_menu_visible = false;
  bool frontend_active = false;
  bool phone_visible = false;
  bool map_active = false;
  bool helicopter_controls = false;
  KeyboardEscapeRoute escape_route = KeyboardEscapeRoute::kPause;
  bool gamepad_valid = false;
  bool valid = false;
  bool trace_sample = false;
};

std::mutex g_epoch_mutex;
InputEpoch g_epoch;
MouseAxisQuantizer g_mouse_x_quantizer;
MouseAxisQuantizer g_mouse_y_quantizer;
MouseAxisQuantizer g_map_mouse_x_quantizer;
MouseAxisQuantizer g_map_mouse_y_quantizer;
rex::ui::MouseEvent::MotionSource g_last_mouse_source = rex::ui::MouseEvent::MotionSource::kGeneric;
uint64_t g_last_mouse_reset_generation = 0;
bool g_mouse_conversion_initialized = false;
bool g_logged_first_epoch = false;
bool g_trace_state_initialized = false;
bool g_trace_last_valid = false;
bool g_trace_last_frontend_active = false;
bool g_trace_last_phone_visible = false;
bool g_trace_last_map_active = false;
bool g_trace_last_gamepad_valid = false;
uint16_t g_trace_last_gamepad_buttons = 0;
std::array<uint8_t, 256> g_trace_last_keys{};
uint64_t g_trace_last_reset_generation = 0;
rex::ui::MouseEvent::MotionSource g_trace_last_source = rex::ui::MouseEvent::MotionSource::kGeneric;
std::array<uint8_t, 256> g_last_functional_keys{};
KeyboardEscapeState g_escape_state;
using FrontendControlHistory =
    std::array<KeyboardActionHistory, std::size(kFrontendButtonActions)>;
std::unordered_map<uint32_t, FrontendControlHistory> g_frontend_history;
using PhoneControlHistory =
    std::array<KeyboardActionHistory, std::size(kPhoneButtonActions)>;
std::unordered_map<uint32_t, PhoneControlHistory> g_phone_history;
std::unordered_map<uint32_t, KeyboardActionHistory> g_frontend_scroll_history;
DirectWeaponRequest g_direct_weapon_request;
thread_local VehicleWeaponCandidatePolicy g_vehicle_weapon_candidates;
RadioOffRequest g_radio_off_request;
uint64_t g_last_map_epoch_sequence = 0;
bool g_user_music_vehicle_active = false;

struct PauseTabInputState {
  uint64_t epoch = 0;
  uint32_t screen = 0;
  bool initialized = false;
  bool left_down = false;
  bool right_down = false;
};

struct GtaActionTraceSnapshot {
  uint32_t screen = 0;
  uint32_t control = 0;
  uint8_t sprint = 0;
  uint8_t jump = 0;
  uint8_t enter = 0;
  uint8_t duck = 0;
  uint8_t pickup = 0;
  uint8_t accelerate = 0;
  uint8_t brake = 0;
  uint8_t steer_left = 0;
  uint8_t steer_right = 0;
  uint8_t pitch_up = 0;
  uint8_t pitch_down = 0;
  uint8_t exit_vehicle = 0;
  uint8_t phone_take_out = 0;
  uint8_t phone_put_away = 0;
  uint8_t frontend_down = 0;
  uint8_t frontend_up = 0;
  uint8_t frontend_left = 0;
  uint8_t frontend_right = 0;
  uint8_t frontend_left_shoulder = 0;
  uint8_t frontend_right_shoulder = 0;
  uint8_t frontend_accept = 0;
  uint8_t frontend_cancel = 0;
  uint8_t frontend_pause = 0;
  uint8_t frontend_x = 0;
  uint8_t frontend_y = 0;

  bool operator==(const GtaActionTraceSnapshot&) const = default;
};

struct GtaActionTraceTracker {
  GtaActionTraceSnapshot snapshot{};
  bool initialized = false;
};

PauseTabInputState g_pause_tab_input;
GtaActionTraceTracker g_controller_action_trace;
GtaActionTraceTracker g_keyboard_action_trace;
GtaActionTraceTracker g_final_action_trace;
GtaActionTraceTracker g_interface_before_trace;
GtaActionTraceTracker g_interface_after_trace;
GtaActionTraceTracker g_interface_consumed_trace;

InputEpoch ReadEpoch();
bool IsDown(const NativeInputState& state, VirtualKey key);
bool IsPressed(const InputEpoch& epoch, VirtualKey key);

struct ScriptNativeCall {
  uint32_t result = 0;
  uint32_t arguments = 0;
};

uint8_t LoadU8(uint8_t* base, uint32_t address) {
  return *reinterpret_cast<volatile uint8_t*>(base + address);
}

uint32_t LoadU32(uint8_t* base, uint32_t address) {
  return __builtin_bswap32(*reinterpret_cast<volatile uint32_t*>(base + address));
}

float LoadFloat(uint8_t* base, uint32_t address) {
  return std::bit_cast<float>(LoadU32(base, address));
}

void StoreU8(uint8_t* base, uint32_t address, uint8_t value) {
  *reinterpret_cast<volatile uint8_t*>(base + address) = value;
}

void StoreU32(uint8_t* base, uint32_t address, uint32_t value) {
  *reinterpret_cast<volatile uint32_t*>(base + address) = __builtin_bswap32(value);
}

ScriptNativeCall ReadScriptNativeCall(uint8_t* base, uint32_t call_context) {
  if (!call_context) {
    return {};
  }
  return {
      .result = LoadU32(base, call_context),
      .arguments = LoadU32(base, call_context + 8),
  };
}

uint32_t ReadScriptNativeArgument(uint8_t* base, const ScriptNativeCall& call,
                                  uint32_t byte_offset) {
  return call.arguments ? LoadU32(base, call.arguments + byte_offset) : 0;
}

struct ParachuteScriptContext {
  uint32_t state = 0;
  bool active = false;
};

ParachuteScriptContext ReadParachuteScriptContext(uint8_t* base) {
  const uint32_t thread = LoadU32(base, kExecutingScriptThreadAddress);
  if (!thread ||
      LoadU32(base, thread + kScriptProgramKeyOffset) !=
          kParachutePlayerProgramKey) {
    return {};
  }
  const uint32_t globals = LoadU32(base, kScriptGlobalsAddress);
  if (!globals) {
    return {};
  }
  return {
      .state = LoadU32(base, globals + kParachuteStateOffset),
      .active = true,
  };
}

bool ForceParachuteControlResult(uint8_t* base, uint32_t call_context,
                                 bool just_pressed, uint32_t forced_value) {
  const ParachuteScriptContext parachute = ReadParachuteScriptContext(base);
  const InputEpoch epoch = ReadEpoch();
  const ScriptNativeCall call = ReadScriptNativeCall(base, call_context);
  if (!parachute.active || !epoch.valid || !call.result || !call.arguments) {
    return false;
  }

  const uint32_t action = ReadScriptNativeArgument(base, call, 4);
  bool force = false;
  if (just_pressed) {
    force = (parachute.state == kParachuteFreefallState &&
             (action == kParachuteDeployAction ||
              action == kParachutePcRightBrakeAction) &&
             IsPressed(epoch, VirtualKey::kLButton)) ||
            (parachute.state == kParachuteDeployedState &&
             action == kParachuteDetachAction &&
             GTA4_TouchVirtualKeyPressed(epoch.sequence,
                                         static_cast<uint16_t>(VirtualKey::kF)));
  } else if (parachute.state == kParachuteDeployedState) {
    force = ((action == kParachuteLeftBrakeAction ||
              action == kParachutePcLeftBrakeAction) &&
             IsDown(epoch.state, VirtualKey::kLButton)) ||
            ((action == kParachuteRightBrakeAction ||
              action == kParachutePcRightBrakeAction) &&
             IsDown(epoch.state, VirtualKey::kRButton)) ||
            (action == kParachuteSmokeAction &&
             IsDown(epoch.state, VirtualKey::kControl));
  }

  if (force) {
    StoreU32(base, call.result, forced_value);
  }
  return force;
}

bool ForceParachuteRawButtonResult(uint8_t* base, uint32_t call_context) {
  const ParachuteScriptContext parachute = ReadParachuteScriptContext(base);
  const InputEpoch epoch = ReadEpoch();
  const ScriptNativeCall call = ReadScriptNativeCall(base, call_context);
  if (!parachute.active || parachute.state != kParachuteDeployedState ||
      !epoch.valid || !call.result || !call.arguments ||
      ReadScriptNativeArgument(base, call, 4) != kParachuteSmokeButton ||
      !IsDown(epoch.state, VirtualKey::kControl)) {
    return false;
  }
  StoreU32(base, call.result, 1);
  return true;
}

bool ApplyParachuteAnalogueSticks(uint8_t* base, uint32_t call_context) {
  const ParachuteScriptContext parachute = ReadParachuteScriptContext(base);
  const InputEpoch epoch = ReadEpoch();
  const ScriptNativeCall call = ReadScriptNativeCall(base, call_context);
  if (!parachute.active ||
      (parachute.state != kParachuteFreefallState &&
       parachute.state != kParachuteDeployedState) ||
      !epoch.valid || !call.arguments) {
    return false;
  }

  const bool left = IsDown(epoch.state, VirtualKey::kA);
  const bool right = IsDown(epoch.state, VirtualKey::kD);
  const bool up = IsDown(epoch.state, VirtualKey::kW);
  const bool down = IsDown(epoch.state, VirtualKey::kS);
  const bool horizontal = left != right;
  const bool vertical = up != down;
  if (horizontal) {
    const uint32_t output = ReadScriptNativeArgument(base, call, 4);
    if (output) {
      StoreU32(base, output, static_cast<uint32_t>(
                                 left ? kAnalogueNegativeExtent
                                      : kAnaloguePositiveExtent));
    }
  }
  if (vertical) {
    const uint32_t output = ReadScriptNativeArgument(base, call, 8);
    if (output) {
      StoreU32(base, output, static_cast<uint32_t>(
                                 up ? kAnalogueNegativeExtent
                                    : kAnaloguePositiveExtent));
    }
  }
  return horizontal || vertical;
}

struct VehicleInputContext {
  uint32_t ped = 0;
  uint32_t vehicle = 0;
  uint32_t vehicle_vtable = 0;
  bool is_driver = false;
  bool is_heli = false;
  bool can_drop_weapon = false;
};

VehicleInputContext ReadVehicleInputContext(uint8_t* base) {
  VehicleInputContext context;
  // This is the leaf lookup performed by generated sub_82238C28(0). The
  // in-vehicle bit and pointer are updated by the retail set-in/set-out tasks,
  // so recomputing them here also covers vehicle transitions without a stale
  // host-side tracker.
  const uint32_t player_index = LoadU32(base, kCurrentPlayerIndexAddress);
  if (player_index >= kMaximumLocalPlayers) {
    return context;
  }

  const uint32_t player_info =
      LoadU32(base, kPlayerInfoTableAddress + player_index * kGuestPointerSize);
  if (!player_info) {
    return context;
  }

  context.ped = LoadU32(base, player_info + kPlayerInfoPedOffset);
  if (!context.ped || !(LoadU32(base, context.ped + kPedVehicleFlagsOffset) & kPedInVehicleFlag)) {
    return context;
  }

  context.vehicle = LoadU32(base, context.ped + kPedVehicleOffset);
  if (!context.vehicle) {
    return context;
  }

  context.vehicle_vtable = LoadU32(base, context.vehicle);
  context.is_driver = LoadU32(base, context.vehicle + kVehicleDriverOffset) == context.ped;
  context.is_heli =
      LoadU32(base, context.vehicle + kVehicleClassOffset) == kHelicopterVehicleClass;
  context.can_drop_weapon =
      LoadU8(base, kPlayerCanDropWeaponsInCarAddress) != 0;
  return context;
}

uint32_t ActionAddress(uint32_t control, Action action) {
  return control + kActionArrayOffset + static_cast<uint32_t>(action) * kActionStride;
}

bool IsDown(const NativeInputState& state, VirtualKey key) {
  const auto index = static_cast<uint16_t>(key);
  return (index < state.keys.size() && state.keys[index] != 0) ||
         GTA4_TouchVirtualKeyDown(index);
}

bool IsNativeActionDown(const InputEpoch& epoch, VirtualKey key) {
  // Both physical and touch compatibility keys now reach retail XInput.
  // Only non-controller PC actions are merged at this later boundary.
  const auto index = static_cast<uint16_t>(key);
  return !IsKeyboardControllerKey(key, epoch.helicopter_controls) &&
         (GTA4_TouchVirtualKeyDown(index) ||
          (index < epoch.state.keys.size() && epoch.state.keys[index] != 0));
}

bool IsPressed(const InputEpoch& epoch, VirtualKey key) {
  const auto index = static_cast<uint16_t>(key);
  return (index < epoch.pressed_keys.size() && epoch.pressed_keys[index] != 0) ||
         GTA4_TouchVirtualKeyPressed(epoch.sequence, index);
}

bool IsChanged(const InputEpoch& epoch, VirtualKey key) {
  const auto index = static_cast<uint16_t>(key);
  return index < epoch.changed_keys.size() && epoch.changed_keys[index] != 0;
}

uint64_t StoredKeyEventSequence(const InputEpoch& epoch, VirtualKey key) {
  const auto index = static_cast<uint16_t>(key);
  if (index < epoch.state.key_event_sequences.size() &&
      epoch.state.key_event_sequences[index] != 0) {
    return epoch.state.key_event_sequences[index];
  }
  return 0;
}

uint64_t KeyEventSequence(const InputEpoch& epoch, VirtualKey key) {
  const uint64_t stored = StoredKeyEventSequence(epoch, key);
  if (stored != 0) {
    return stored;
  }
  return epoch.state.last_key_event_sequence != 0
             ? epoch.state.last_key_event_sequence
             : rex::input::NextInputTraceSequence();
}

// Every physical key consumed by the native GTA input bridge, plus the
// configured PC control surface (letter/number rows and navigation keys).
// Trace only edges below so held movement keys don't flood persistent logs.
constexpr VirtualKey kControlTraceKeys[] = {
    VirtualKey::kLButton, VirtualKey::kRButton, VirtualKey::kXButton1,
    VirtualKey::kBack, VirtualKey::kTab, VirtualKey::kReturn,
    VirtualKey::kShift, VirtualKey::kControl, VirtualKey::kCapital,
    VirtualKey::kEscape, VirtualKey::kSpace,
    VirtualKey::kLeft, VirtualKey::kUp, VirtualKey::kRight,
    VirtualKey::kDown, VirtualKey::kDelete,
    VirtualKey::k0, VirtualKey::k1, VirtualKey::k2, VirtualKey::k3,
    VirtualKey::k4, VirtualKey::k5, VirtualKey::k6, VirtualKey::k7,
    VirtualKey::k8, VirtualKey::k9,
    VirtualKey::kA, VirtualKey::kB, VirtualKey::kC, VirtualKey::kD,
    VirtualKey::kE, VirtualKey::kF, VirtualKey::kG, VirtualKey::kH,
    VirtualKey::kI, VirtualKey::kJ, VirtualKey::kK, VirtualKey::kL,
    VirtualKey::kM, VirtualKey::kN, VirtualKey::kO, VirtualKey::kP,
    VirtualKey::kQ, VirtualKey::kR, VirtualKey::kS, VirtualKey::kT,
    VirtualKey::kU, VirtualKey::kV, VirtualKey::kW, VirtualKey::kX,
    VirtualKey::kY, VirtualKey::kZ,
    VirtualKey::kNumpad0, VirtualKey::kNumpad2, VirtualKey::kNumpad4,
    VirtualKey::kNumpad6, VirtualKey::kNumpad8, VirtualKey::kOem3,
};

bool EpochHasFocusedTraceInput(const InputEpoch& epoch) {
  for (VirtualKey key : kControlTraceKeys) {
    if (IsChanged(epoch, key)) {
      return true;
    }
  }
  return false;
}

uint64_t FocusedTraceSequence(const InputEpoch& epoch) {
  for (VirtualKey key : kControlTraceKeys) {
    if (IsChanged(epoch, key)) {
      return KeyEventSequence(epoch, key);
    }
  }
  return epoch.state.last_key_event_sequence != 0
             ? epoch.state.last_key_event_sequence
             : rex::input::NextInputTraceSequence();
}

bool FrontendActive(const PPCContext& parent, uint8_t* base) {
  const bool pause_visible = LoadU8(base, kPauseMenuVisibleAddress) != 0;
  if (pause_visible) {
    return RetailPauseMenuActive(
        pause_visible, LoadU32(base, kPauseMenuTransitionAddress));
  }
  // Non-pause frontend widgets still use their own activation state. A pause
  // menu may own keyboard navigation while this particular widget is inactive.
  PPCContext nested = parent;
  nested.r3.u32 = 0;
  __imp__sub_8224EEF8(nested, base);
  return nested.r3.u8 != 0;
}

bool ConfigureKeyboardControllerForPoll(const PPCContext& parent, uint8_t* base) {
  const VehicleInputContext vehicle = ReadVehicleInputContext(base);
  const bool helicopter_controls = UseHelicopterControllerBindings(
      vehicle.is_driver, vehicle.is_heli, FrontendActive(parent, base));
  rex::input::mnk::SetNativeControllerCompatibilityBindings(
      KeyboardControllerBindings(helicopter_controls));
  return helicopter_controls;
}

uint32_t SelectInterfaceControl(const PPCContext& parent, uint8_t* base) {
  // Every action read in generated sub_8224FFC8 obtains its control through
  // sub_821B42B8(1). Use the same selector at the consumer boundary rather
  // than guessing that the normal gameplay replay object is also current.
  PPCContext nested = parent;
  nested.r3.u32 = 1;
  __imp__sub_821B42B8(nested, base);
  return nested.r3.u32;
}

struct PhoneVisibilityContext {
  uint32_t render_index = 0;
  uint32_t render_object = 0;
  bool created = false;
  bool moving_offscreen = false;
  bool render_visible = false;
  bool visible = false;
};

PhoneVisibilityContext ReadPhoneVisibilityContext(uint8_t* base) {
  PhoneVisibilityContext context;
  context.created = LoadU8(base, kPhoneCreatedAddress) != 0;
  context.moving_offscreen = LoadU8(base, kPhoneMovingOffscreenAddress) != 0;
  context.render_index = LoadU32(base, kPhoneRenderIndexAddress);
  if (context.render_index >= kPhoneRenderObjectCount) {
    return context;
  }
  context.render_object =
      LoadU32(base, kPhoneRenderObjectTableAddress +
                        context.render_index * kGuestPointerSize);
  // This is the exact leaf state tested by the retail
  // CAN_PHONE_BE_SEEN_ON_SCREEN native. Treat a missing object as not visible
  // instead of dereferencing it; the retail path assumes initialization has
  // already installed the object.
  context.render_visible =
      context.render_object &&
      LoadU8(base, context.render_object + kPhoneHiddenStateOffset) == 0;
  context.visible = PhoneOwnsKeyboard(context.created, context.moving_offscreen,
                                     context.render_visible);
  return context;
}

bool IsGameplayAction(Action action) {
  const uint32_t index = static_cast<uint32_t>(action);
  return index < 64 || index > 84;
}

bool IsFrontendAction(Action action) {
  return ClassifyKeyboardActionRoute(static_cast<uint32_t>(action)) ==
         KeyboardActionRoute::kFrontendReplayWithConsumerFallback;
}

bool IsPhoneAction(Action action) {
  return ClassifyKeyboardActionRoute(static_cast<uint32_t>(action)) ==
         KeyboardActionRoute::kPhoneActiveGameplayControl;
}

bool IsInterfaceKeyboardAction(Action action) {
  return IsFrontendAction(action) || IsPhoneAction(action);
}

bool IsGlobalKeyboardAction(Action action) {
  return action == Action::kZoomRadar;
}

bool MergeButton(uint8_t* base, uint32_t control, Action action, uint8_t requested) {
  if (!requested) {
    return false;
  }
  const uint32_t address = ActionAddress(control, action);
  const uint8_t polarity = LoadU8(base, address);
  const uint8_t current = LoadU8(base, address + kActionCurrentOffset);
  const uint8_t merged = rex::input::mnk::MergeActionMagnitude(polarity, current, requested);
  if (merged != current) {
    StoreU8(base, address + kActionCurrentOffset, merged);
  }
  return IsGameplayAction(action);
}

uint8_t ReadActionRaw(uint8_t* base, uint32_t control, Action action) {
  const uint32_t address = ActionAddress(control, action);
  return rex::input::mnk::DecodeActionMagnitude(LoadU8(base, address),
                                                LoadU8(base, address + kActionCurrentOffset));
}

GtaActionTraceSnapshot CaptureActionTrace(uint8_t* base, uint32_t control) {
  if (!control) {
    return {};
  }
  return {
      .screen = LoadU32(base, kCurrentScreenAddress),
      .control = control,
      .sprint = ReadActionRaw(base, control, Action::kSprint),
      .jump = ReadActionRaw(base, control, Action::kJump),
      .enter = ReadActionRaw(base, control, Action::kEnter),
      .duck = ReadActionRaw(base, control, Action::kDuck),
      .pickup = ReadActionRaw(base, control, Action::kPickup),
      .accelerate = ReadActionRaw(base, control, Action::kVehicleAccelerate),
      .brake = ReadActionRaw(base, control, Action::kVehicleBrake),
      .steer_left = ReadActionRaw(base, control, Action::kVehicleMoveLeft),
      .steer_right = ReadActionRaw(base, control, Action::kVehicleMoveRight),
      .pitch_up = ReadActionRaw(base, control, Action::kVehicleMoveUp),
      .pitch_down = ReadActionRaw(base, control, Action::kVehicleMoveDown),
      .exit_vehicle = ReadActionRaw(base, control, Action::kVehicleExit),
      .phone_take_out = ReadActionRaw(base, control, Action::kPhoneTakeOut),
      .phone_put_away = ReadActionRaw(base, control, Action::kPhonePutAway),
      .frontend_down = ReadActionRaw(base, control, Action::kFrontendDown),
      .frontend_up = ReadActionRaw(base, control, Action::kFrontendUp),
      .frontend_left = ReadActionRaw(base, control, Action::kFrontendLeft),
      .frontend_right = ReadActionRaw(base, control, Action::kFrontendRight),
      .frontend_left_shoulder =
          ReadActionRaw(base, control, Action::kFrontendLeftShoulder),
      .frontend_right_shoulder =
          ReadActionRaw(base, control, Action::kFrontendRightShoulder),
      .frontend_accept = ReadActionRaw(base, control, Action::kFrontendAccept),
      .frontend_cancel = ReadActionRaw(base, control, Action::kFrontendCancel),
      .frontend_pause = ReadActionRaw(base, control, Action::kFrontendPause),
      .frontend_x = ReadActionRaw(base, control, Action::kFrontendX),
      .frontend_y = ReadActionRaw(base, control, Action::kFrontendY),
  };
}

void TraceFocusedKeyRoutes(uint8_t* base, const InputEpoch& epoch,
                           uint32_t replay_control,
                           uint32_t active_gameplay_control,
                           const char* stage) {
  if (!rex::input::IsInputTraceEnabled() || !epoch.valid) {
    return;
  }
  const GtaActionTraceSnapshot replay =
      CaptureActionTrace(base, replay_control);
  const GtaActionTraceSnapshot active =
      CaptureActionTrace(base, active_gameplay_control);
  for (VirtualKey key : kControlTraceKeys) {
    if (!IsChanged(epoch, key)) {
      continue;
    }
    const auto index = static_cast<uint16_t>(key);
    REXLOG_INFO(
        "input-e2e: seq={} stage={} epoch={} key={} vk={} down={} changed={} "
        "pressed={} frontend-active={} phone-visible={} map-active={} "
        "gamepad-valid={} packet={} buttons={:04X} "
        "replay-control={:08X} active-control={:08X} "
        "replay-face={}/{}/{}/{}/{} active-face={}/{}/{}/{}/{} "
        "replay-phone={}/{} replay-frontend={}/{}/{}/{}:{}/{}/{} "
        "replay-frontend-xy={}/{} active-phone={}/{} "
        "active-frontend={}/{}/{}/{}:{}/{}/{} active-frontend-xy={}/{}",
        KeyEventSequence(epoch, key), stage, epoch.sequence,
        rex::input::InputTraceVirtualKeyName(index), index,
        IsDown(epoch.state, key), IsChanged(epoch, key), IsPressed(epoch, key),
        epoch.frontend_active, epoch.phone_visible, epoch.map_active,
        epoch.gamepad_valid, epoch.gamepad_packet, epoch.gamepad_buttons,
        replay_control, active_gameplay_control,
        replay.sprint, replay.jump, replay.enter, replay.duck, replay.pickup,
        active.sprint, active.jump, active.enter, active.duck, active.pickup,
        replay.phone_take_out,
        replay.phone_put_away, replay.frontend_down, replay.frontend_up,
        replay.frontend_left, replay.frontend_right, replay.frontend_pause,
        replay.frontend_accept, replay.frontend_cancel, replay.frontend_x,
        replay.frontend_y, active.phone_take_out,
        active.phone_put_away, active.frontend_down, active.frontend_up,
        active.frontend_left, active.frontend_right, active.frontend_pause,
        active.frontend_accept, active.frontend_cancel, active.frontend_x,
        active.frontend_y);
  }
}

void TraceActionState(const char* stage, const InputEpoch& epoch,
                      const GtaActionTraceSnapshot& snapshot,
                      GtaActionTraceTracker& tracker) {
  if (!rex::input::IsInputTraceEnabled() ||
      (tracker.initialized && tracker.snapshot == snapshot)) {
    return;
  }
  tracker.initialized = true;
  tracker.snapshot = snapshot;
  const uint64_t host_sequence = epoch.state.last_key_event_sequence != 0
                                     ? epoch.state.last_key_event_sequence
                                     : rex::input::NextInputTraceSequence();
  REXLOG_INFO(
      "input-e2e: seq={} stage={} epoch={} screen={} frontend-active={} "
      "phone-visible={} map-active={} control={:08X} "
      "face=sprint:{}/jump:{}/enter:{}/duck:{}/pickup:{} "
      "vehicle=accelerate:{}/brake:{}/steer:{}/{}:pitch:{}/{}:exit:{} "
      "phone=take-out:{}/put-away:{} "
      "frontend=direction:{}/{}/{}/{}:shoulder:{}/{}:accept:{}:cancel:{}:pause:{}:x:{}:y:{}",
      host_sequence, stage, epoch.sequence, snapshot.screen, epoch.frontend_active,
      epoch.phone_visible, epoch.map_active, snapshot.control,
      snapshot.sprint, snapshot.jump, snapshot.enter, snapshot.duck,
      snapshot.pickup,
      snapshot.accelerate, snapshot.brake, snapshot.steer_left, snapshot.steer_right,
      snapshot.pitch_up, snapshot.pitch_down, snapshot.exit_vehicle,
      snapshot.phone_take_out, snapshot.phone_put_away, snapshot.frontend_down,
      snapshot.frontend_up, snapshot.frontend_left, snapshot.frontend_right,
      snapshot.frontend_left_shoulder, snapshot.frontend_right_shoulder,
      snapshot.frontend_accept, snapshot.frontend_cancel, snapshot.frontend_pause,
      snapshot.frontend_x, snapshot.frontend_y);
}

void ProcessPauseTabShoulders(PPCContext& parent, uint8_t* base,
                              const InputEpoch& epoch) {
  if ((!epoch.valid && !epoch.gamepad_valid) ||
      g_pause_tab_input.epoch == epoch.sequence) {
    return;
  }
  g_pause_tab_input.epoch = epoch.sequence;
  const uint32_t screen = LoadU32(base, kCurrentScreenAddress);
  const bool keyboard_left_down = epoch.valid && IsDown(epoch.state, VirtualKey::kQ);
  const bool gamepad_left_down =
      epoch.gamepad_valid &&
      (epoch.gamepad_buttons & rex::input::X_INPUT_GAMEPAD_LEFT_SHOULDER) != 0;
  const bool gamepad_right_down =
      epoch.gamepad_valid &&
      (epoch.gamepad_buttons & rex::input::X_INPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
  const bool left_down = keyboard_left_down || gamepad_left_down;
  const bool right_down = gamepad_right_down;

  if (!epoch.frontend_active ||
      !gta4::frontend_menu::policy::IsPauseTabScreen(screen)) {
    g_pause_tab_input = {
        .epoch = epoch.sequence,
        .screen = screen,
        .initialized = false,
        .left_down = left_down,
        .right_down = right_down,
    };
    return;
  }

  if (!g_pause_tab_input.initialized) {
    g_pause_tab_input.screen = screen;
    g_pause_tab_input.initialized = true;
    g_pause_tab_input.left_down = left_down;
    g_pause_tab_input.right_down = right_down;
    if (rex::input::IsInputTraceEnabled()) {
      REXLOG_INFO(
          "input-e2e: seq={} stage=pause-tab result=armed epoch={} screen={} "
          "shoulders={}/{} keyboard-left={} gamepad={}/{} buttons={:04X}",
          rex::input::NextInputTraceSequence(), epoch.sequence, screen, left_down, right_down,
          keyboard_left_down, gamepad_left_down, gamepad_right_down,
          epoch.gamepad_buttons);
    }
    return;
  }

  const auto edge = gta4::frontend_menu::policy::ClassifyPauseTabShoulderEdge(
      g_pause_tab_input.left_down, g_pause_tab_input.right_down, left_down,
      right_down);
  g_pause_tab_input.screen = screen;
  g_pause_tab_input.left_down = left_down;
  g_pause_tab_input.right_down = right_down;

  if (edge == gta4::frontend_menu::policy::PauseTabShoulderEdge::kNone ||
      edge == gta4::frontend_menu::policy::PauseTabShoulderEdge::kSimultaneous) {
    if (edge == gta4::frontend_menu::policy::PauseTabShoulderEdge::kSimultaneous &&
        rex::input::IsInputTraceEnabled()) {
      REXLOG_INFO(
          "input-e2e: seq={} stage=pause-tab result=ignored reason=simultaneous epoch={} "
          "screen={}",
          rex::input::NextInputTraceSequence(), epoch.sequence, screen);
    }
    return;
  }

  uint32_t previous_screen = 0;
  uint32_t target_screen = 0;
  const auto direction = edge == gta4::frontend_menu::policy::PauseTabShoulderEdge::kPrevious
                             ? gta4::frontend_menu::policy::PauseTabDirection::kPrevious
                             : gta4::frontend_menu::policy::PauseTabDirection::kNext;
  if (!gta4::frontend_menu::SwitchPauseTab(parent, base, direction, &previous_screen,
                                            &target_screen) &&
      rex::input::IsInputTraceEnabled()) {
    REXLOG_INFO(
        "input-e2e: seq={} stage=pause-tab result=rejected epoch={} screen={} shoulders={}/{}",
        rex::input::NextInputTraceSequence(), epoch.sequence, screen, left_down, right_down);
  }
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
  return IsGameplayAction(negative) || IsGameplayAction(positive);
}

bool MergeSignedAction(uint8_t* base, uint32_t control, Action action,
                       int32_t requested) {
  if (!requested) {
    return false;
  }
  const uint32_t address = ActionAddress(control, action);
  const uint8_t polarity = LoadU8(base, address);
  const uint8_t current = LoadU8(base, address + kActionCurrentOffset);
  const auto merge =
      rex::input::mnk::MergeSignedAction(polarity, current, requested);
  if (merge.changed) {
    StoreU8(base, address + kActionCurrentOffset, merge.encoded);
  }
  return merge.changed;
}

struct DirectWeaponBinding {
  VirtualKey key;
  uint32_t slot;
};

constexpr DirectWeaponBinding kDirectWeaponBindings[] = {
    {VirtualKey::k1, 0}, {VirtualKey::k2, 1}, {VirtualKey::k3, 2},
    {VirtualKey::k4, 3}, {VirtualKey::k5, 4}, {VirtualKey::k6, 5},
    {VirtualKey::k7, 6}, {VirtualKey::k8, 7}, {VirtualKey::k9, 8},
    {VirtualKey::k0, 9},
};

bool IsVehicleUsableWeaponSlot(uint32_t slot) {
  return slot == 2 || slot == 4 || slot == 8;
}

void ArmContextRequests(uint8_t* base) {
  g_direct_weapon_request = {};
  g_radio_off_request = {};

  if (!g_epoch.valid) {
    return;
  }

  if (g_epoch.frontend_active || g_epoch.phone_visible) {
    bool direct_weapon_key_pressed = false;
    for (const DirectWeaponBinding& binding : kDirectWeaponBindings) {
      direct_weapon_key_pressed |= IsPressed(g_epoch, binding.key);
    }
    if (direct_weapon_key_pressed && REXCVAR_GET(gta4_native_input_trace)) {
      REXLOG_INFO(
          "gta4-input: direct-weapon-suppressed epoch={} frontend-active={} "
          "phone-visible={} reason=interface-owns-number-row",
          g_epoch.sequence, g_epoch.frontend_active, g_epoch.phone_visible);
    }
    return;
  }

  const VehicleInputContext context = ReadVehicleInputContext(base);
  const bool music_context = context.vehicle && context.is_driver;
  const bool next_song_pressed = IsPressed(g_epoch, VirtualKey::kN);
  const bool previous_song_pressed = IsPressed(g_epoch, VirtualKey::kB);
  const bool radio_off_pressed = IsPressed(g_epoch, VirtualKey::kX);
  const bool pc_radio_key_pressed =
      next_song_pressed || previous_song_pressed || radio_off_pressed;
  const bool user_music_available =
      pc_radio_key_pressed && IsUserMusicAvailable();
  bool user_music_accepted = false;
  if (!music_context && g_user_music_vehicle_active) {
    RequestUserMusicStop();
    g_user_music_vehicle_active = false;
  }
  // N/B are the PC Independence FM skip bindings. Only replace the guest
  // vehicle radio after the pre-scanned host playlist has accepted exactly
  // one request. In particular, an empty User Music directory must leave the
  // title's radio untouched.
  if (music_context && !radio_off_pressed &&
      next_song_pressed != previous_song_pressed) {
    user_music_accepted = next_song_pressed ? RequestUserMusicNext()
                                            : RequestUserMusicPrevious();
    if (user_music_accepted) {
      g_user_music_vehicle_active = true;
    }
  }
  // X is also a native GTA PC binding: unlike N/B, it always requests the
  // title's own radio-off transition while driving. It additionally cancels
  // host user music if that path was active.
  if (radio_off_pressed) {
    RequestUserMusicStop();
    g_user_music_vehicle_active = false;
  }
  uint32_t requested_slot = 0;
  uint32_t pressed_slot_count = 0;
  for (const DirectWeaponBinding& binding : kDirectWeaponBindings) {
    if (IsPressed(g_epoch, binding.key)) {
      requested_slot = binding.slot;
      ++pressed_slot_count;
    }
  }

  if (pressed_slot_count == 1 && context.ped &&
      (!context.vehicle || IsVehicleUsableWeaponSlot(requested_slot))) {
    g_direct_weapon_request = {
        .epoch = g_epoch.sequence,
        .user = g_epoch.state.user_index,
        .ped = context.ped,
        .slot = requested_slot,
        .in_vehicle = context.vehicle != 0,
        .armed = true,
    };
  } else if (!pressed_slot_count && context.ped && context.vehicle &&
             IsPressed(g_epoch, VirtualKey::kQ) !=
                 IsPressed(g_epoch, VirtualKey::kZ)) {
    g_direct_weapon_request = {
        .epoch = g_epoch.sequence,
        .user = g_epoch.state.user_index,
        .ped = context.ped,
        .request = IsPressed(g_epoch, VirtualKey::kQ)
                       ? KeyboardWeaponRequest::kNext
                       : KeyboardWeaponRequest::kPrevious,
        .in_vehicle = true,
        .armed = true,
    };
  }

  if ((radio_off_pressed || user_music_accepted) && music_context &&
      context.ped) {
    g_radio_off_request = {
        .epoch = g_epoch.sequence,
        .user = g_epoch.state.user_index,
        .ped = context.ped,
        .vehicle = context.vehicle,
        .armed = true,
    };
  }

  if (REXCVAR_GET(gta4_native_input_trace) && pc_radio_key_pressed) {
    REXLOG_INFO(
        "gta4-input: pc-radio-key epoch={} user={} ped={:08X} "
        "vehicle={:08X} driver={} next={} previous={} off={} "
        "host-available={} host-accepted={} host-active={} "
        "guest-radio-off-armed={}",
        g_epoch.sequence, g_epoch.state.user_index, context.ped,
        context.vehicle, context.is_driver, next_song_pressed,
        previous_song_pressed, radio_off_pressed, user_music_available,
        user_music_accepted, g_user_music_vehicle_active,
        g_radio_off_request.armed);
  }

  if (REXCVAR_GET(gta4_native_input_trace) &&
      (g_direct_weapon_request.armed || g_radio_off_request.armed ||
       pressed_slot_count > 1)) {
    REXLOG_INFO(
        "gta4-input: context-request epoch={} user={} ped={:08X} "
        "vehicle={:08X} weapon_armed={} weapon_slot={} weapon_key_count={} "
        "radio_off_armed={}",
        g_epoch.sequence, g_epoch.state.user_index, context.ped, context.vehicle,
        g_direct_weapon_request.armed, requested_slot, pressed_slot_count,
        g_radio_off_request.armed);
  }
}

struct ButtonBinding {
  VirtualKey key;
  Action action;
};

// GTA itself selects the active family (on-foot, vehicle, phone or frontend).
// Broadcasting the corresponding semantic actions preserves that engine
// context switching while avoiding heuristic mode detection in host code.
constexpr ButtonBinding kButtonBindings[] = {
    {VirtualKey::kShift, Action::kSprint},
    {VirtualKey::kSpace, Action::kJump},
    {VirtualKey::kLButton, Action::kAttack},
    {VirtualKey::kRButton, Action::kAim},
    {VirtualKey::kC, Action::kLookBehind},
    {VirtualKey::kR, Action::kReload},
    {VirtualKey::kQ, Action::kCover},
    {VirtualKey::kE, Action::kPickup},
    {VirtualKey::kControl, Action::kDuck},
    {VirtualKey::kV, Action::kNextCamera},
    {VirtualKey::kEscape, Action::kPhonePutAway},
    {VirtualKey::kLButton, Action::kVehicleAttack},
    {VirtualKey::kRButton, Action::kVehicleAttack2},
    {VirtualKey::kW, Action::kVehicleAccelerate},
    {VirtualKey::kS, Action::kVehicleBrake},
    {VirtualKey::kH, Action::kVehicleHeadlight},
    {VirtualKey::kSpace, Action::kVehicleHandbrake},
    {VirtualKey::kW, Action::kVehicleHotwireLeft},
    {VirtualKey::kS, Action::kVehicleHotwireRight},
    {VirtualKey::kC, Action::kVehicleLookBehind},
    {VirtualKey::kCapital, Action::kVehicleCinematicCamera},
    {VirtualKey::kG, Action::kVehicleHorn},
    {VirtualKey::kW, Action::kVehicleFlyThrottleUp},
    {VirtualKey::kS, Action::kVehicleFlyThrottleDown},
    {VirtualKey::kNumpad4, Action::kVehicleFlyYawLeft},
    {VirtualKey::kNumpad6, Action::kVehicleFlyYawRight},
    {VirtualKey::kLButton, Action::kMeleeAttack1},
    {VirtualKey::kR, Action::kMeleeAttack2},
    {VirtualKey::kQ, Action::kMeleeKick},
    {VirtualKey::kSpace, Action::kMeleeBlock},
    {VirtualKey::kS, Action::kFrontendDown},
    {VirtualKey::kW, Action::kFrontendUp},
    {VirtualKey::kA, Action::kFrontendLeft},
    {VirtualKey::kD, Action::kFrontendRight},
    {VirtualKey::kEscape, Action::kFrontendPause},
    {VirtualKey::kLButton, Action::kFrontendAccept},
    {VirtualKey::kEscape, Action::kFrontendCancel},
    {VirtualKey::kXButton1, Action::kFrontendCancel},
    {VirtualKey::kR, Action::kFrontendX},
    {VirtualKey::kSpace, Action::kFrontendY},
    {VirtualKey::kE, Action::kFrontendY},
    {VirtualKey::kQ, Action::kFrontendLeftShoulder},
    {VirtualKey::kRButton, Action::kFrontendLeftTrigger},
    {VirtualKey::kLButton, Action::kFrontendRightTrigger},
    {VirtualKey::kT, Action::kZoomRadar},
    {VirtualKey::kTab, Action::kZoomRadar},
};

bool ShouldInjectInterfaceBinding(const ButtonBinding& binding,
                                  const InputEpoch& epoch) {
  return ShouldInjectKeyboardInterfaceAction(
      static_cast<uint32_t>(binding.action), binding.key,
      {.frontend_active = epoch.frontend_active,
       .phone_visible = epoch.phone_visible,
       .map_active = epoch.map_active,
       .helicopter_controls = epoch.helicopter_controls,
       .escape_route = epoch.escape_route});
}

template <size_t Count>
void PrepareKeyboardHistory(uint8_t* base, uint32_t control,
                             uint64_t epoch, bool replayed,
                             const Action (&actions)[Count],
                             std::array<KeyboardActionHistory, Count>& history) {
  for (size_t index = 0; index < history.size(); ++index) {
    const uint32_t address = ActionAddress(control, actions[index]);
    const KeyboardActionBytes observed{
        LoadU8(base, address + kActionCurrentOffset),
        LoadU8(base, address + kActionPreviousOffset)};
    const KeyboardActionBytes prepared =
        history[index].Prepare(epoch, replayed, observed);
    if (prepared.current != observed.current) {
      StoreU8(base, address + kActionCurrentOffset, prepared.current);
    }
    if (prepared.previous != observed.previous) {
      StoreU8(base, address + kActionPreviousOffset, prepared.previous);
    }
  }
}

template <size_t Count>
void CommitKeyboardHistory(uint8_t* base, uint32_t control,
                            const Action (&actions)[Count],
                            std::array<KeyboardActionHistory, Count>& history) {
  for (size_t index = 0; index < history.size(); ++index) {
    const uint32_t address = ActionAddress(control, actions[index]);
    history[index].Commit({LoadU8(base, address + kActionCurrentOffset),
                           LoadU8(base, address + kActionPreviousOffset)});
  }
}

void PrepareFrontendHistory(uint8_t* base, uint32_t control,
                             uint64_t epoch, bool replayed) {
  PrepareKeyboardHistory(base, control, epoch, replayed,
                         kFrontendButtonActions, g_frontend_history[control]);
}

void CommitFrontendHistory(uint8_t* base, uint32_t control) {
  CommitKeyboardHistory(base, control, kFrontendButtonActions,
                        g_frontend_history[control]);
}

void PreparePhoneHistory(uint8_t* base, uint32_t control,
                          uint64_t epoch, bool replayed) {
  PrepareKeyboardHistory(base, control, epoch, replayed,
                         kPhoneButtonActions, g_phone_history[control]);
}

void CommitPhoneHistory(uint8_t* base, uint32_t control) {
  CommitKeyboardHistory(base, control, kPhoneButtonActions,
                        g_phone_history[control]);
}

void InjectFrontendScroll(const PPCContext& parent, uint8_t* base,
                           uint32_t control, const InputEpoch& epoch,
                           bool replayed) {
  if (!epoch.valid || !control ||
      LoadU32(base, control + kControlUserIndexOffset) != epoch.state.user_index) {
    return;
  }
  const uint32_t address = ActionAddress(control, Action::kFrontendScrollY);
  auto& history = g_frontend_scroll_history[control];
  const KeyboardActionBytes observed{
      LoadU8(base, address + kActionCurrentOffset),
      LoadU8(base, address + kActionPreviousOffset)};
  if (history.initialized && !replayed && history.epoch != epoch.sequence &&
      observed == history.written) {
    // Match the generated replay's call exactly, including GTA's signed
    // deadzone/response curve. Do not approximate it from the encoded byte.
    PPCContext nested = parent;
    nested.r3.u64 = (static_cast<uint64_t>(LoadU32(base, address)) << 32) |
                    LoadU32(base, address + 4);
    nested.r4.u64 = static_cast<uint64_t>(LoadU32(base, address + 8)) << 32;
    __imp__sub_822B7958(nested, base);
    StoreU32(base, control + kFrontendScrollPreviousOffset, nested.r3.u32);
  }
  const KeyboardActionBytes prepared =
      history.Prepare(epoch.sequence, replayed, observed);
  if (prepared.current != observed.current) {
    StoreU8(base, address + kActionCurrentOffset, prepared.current);
  }
  if (prepared.previous != observed.previous) {
    StoreU8(base, address + kActionPreviousOffset, prepared.previous);
  }
  const int32_t requested = KeyboardFrontendScroll(
      epoch.state.mouse_wheel, epoch.frontend_active, epoch.map_active);
  if (requested) {
    MergeSignedAction(base, control, Action::kFrontendScrollY, requested);
    StoreU32(base, control + kLastInputTimeOffset,
             LoadU32(base, kGameInputTimeAddress));
  }
  history.Commit({LoadU8(base, address + kActionCurrentOffset),
                   LoadU8(base, address + kActionPreviousOffset)});
}

bool InjectFrontendFallbackActions(uint8_t* base, uint32_t control,
                                   const InputEpoch& epoch) {
  if (!epoch.valid || !control ||
      LoadU32(base, control + kControlUserIndexOffset) != epoch.state.user_index) {
    return false;
  }

  PrepareFrontendHistory(base, control, epoch.sequence, false);

  bool requested = false;
  for (const ButtonBinding& binding : kButtonBindings) {
    const KeyboardActionRoute route =
        ClassifyKeyboardActionRoute(static_cast<uint32_t>(binding.action));
    if (!NeedsFrontendConsumerFallback(route) ||
        !ShouldInjectInterfaceBinding(binding, epoch) ||
        !IsDown(epoch.state, binding.key)) {
      continue;
    }
    MergeButton(base, control, binding.action, kPressed);
    requested = true;
  }
  if (requested) {
    StoreU32(base, control + kLastInputTimeOffset,
             LoadU32(base, kGameInputTimeAddress));
  }
  CommitFrontendHistory(base, control);
  return requested;
}

bool InjectPhoneActions(uint8_t* base, uint32_t control,
                        const InputEpoch& epoch) {
  if (!epoch.valid || !control ||
      LoadU32(base, control + kControlUserIndexOffset) != epoch.state.user_index) {
    return false;
  }

  PreparePhoneHistory(base, control, epoch.sequence, false);

  bool requested = false;
  for (const ButtonBinding& binding : kButtonBindings) {
    const KeyboardActionRoute route =
        ClassifyKeyboardActionRoute(static_cast<uint32_t>(binding.action));
    if (!UsesActiveGameplayControl(route) ||
        !ShouldInjectInterfaceBinding(binding, epoch) ||
        !IsDown(epoch.state, binding.key)) {
      continue;
    }
    MergeButton(base, control, binding.action, kPressed);
    requested = true;
  }
  if (requested) {
    StoreU32(base, control + kLastInputTimeOffset,
             LoadU32(base, kGameInputTimeAddress));
  }
  CommitPhoneHistory(base, control);
  return requested;
}

int32_t NativeDigitalAxis(const InputEpoch& epoch, VirtualKey negative, VirtualKey positive) {
  const bool negative_down = IsNativeActionDown(epoch, negative);
  const bool positive_down = IsNativeActionDown(epoch, positive);
  if (negative_down == positive_down) {
    return 0;
  }
  return negative_down ? kFullNegative : kFullPositive;
}

void ResetMouseConversion() {
  g_mouse_x_quantizer.Reset();
  g_mouse_y_quantizer.Reset();
  g_map_mouse_x_quantizer.Reset();
  g_map_mouse_y_quantizer.Reset();
}

InputEpoch CaptureEpoch(const PPCContext& entry_context, uint8_t* base,
                        uint32_t caller, bool helicopter_controls) {
  NativeInputState state{};
  const bool native_valid = rex::input::mnk::ConsumeNativeInputState(&state);
  const bool touch_active = rex::input::TouchControlsActive() && !GTA4_TouchTitleInputOwned();
  if (!native_valid) state = {};
  const uint32_t input_user = native_valid ? state.user_index : 0;
  rex::input::X_INPUT_STATE gamepad_state{};
  auto* runtime = rex::Runtime::instance();
  auto* input_system =
      runtime ? static_cast<rex::input::InputSystem*>(runtime->input_system()) : nullptr;
  const bool gamepad_valid =
      input_system && input_system->TryGetLastState(input_user, &gamepad_state);
  const bool valid = NeedsNativeActionReplay(native_valid, touch_active,
      REXCVAR_GET(gta4_motion_aim), gamepad_valid);
  const uint16_t gamepad_buttons =
      gamepad_valid ? static_cast<uint16_t>(gamepad_state.gamepad.buttons) : 0;
  const bool frontend_active = FrontendActive(entry_context, base);
  const PhoneVisibilityContext phone = ReadPhoneVisibilityContext(base);
  const uint32_t screen = LoadU32(base, kCurrentScreenAddress);

  std::lock_guard lock(g_epoch_mutex);
  const bool controller_context_changed =
      g_epoch.helicopter_controls != helicopter_controls;
  ++g_epoch.sequence;
  g_epoch.helicopter_controls = helicopter_controls;
  g_epoch.valid = valid;
  g_epoch.gamepad_valid = gamepad_valid;
  g_epoch.gamepad_buttons = gamepad_buttons;
  g_epoch.gamepad_packet =
      gamepad_valid ? static_cast<uint32_t>(gamepad_state.packet_number) : 0;
  g_epoch.poll_caller = caller;
  g_epoch.phone_render_index = phone.render_index;
  g_epoch.phone_render_object = phone.render_object;
  g_epoch.phone_created = phone.created;
  g_epoch.phone_moving_offscreen = phone.moving_offscreen;
  g_epoch.phone_render_visible = phone.render_visible;
  g_epoch.pause_menu_transition = LoadU32(base, kPauseMenuTransitionAddress);
  g_epoch.pause_menu_visible = LoadU8(base, kPauseMenuVisibleAddress) != 0;
  g_epoch.frontend_active = frontend_active;
  g_epoch.phone_visible = phone.visible;
  g_epoch.map_active = frontend_active && screen == kMapScreen;
  g_epoch.mouse_x = 0;
  g_epoch.mouse_y = 0;
  g_epoch.map_mouse_x = 0;
  g_epoch.map_mouse_y = 0;
  g_epoch.pressed_keys = {};
  g_epoch.changed_keys = {};
  g_epoch.trace_sample = controller_context_changed ||
                         !g_trace_state_initialized || valid != g_trace_last_valid ||
                         frontend_active != g_trace_last_frontend_active ||
                         phone.visible != g_trace_last_phone_visible ||
                         g_epoch.map_active != g_trace_last_map_active ||
                         gamepad_valid != g_trace_last_gamepad_valid ||
                         gamepad_buttons != g_trace_last_gamepad_buttons;
  if (!valid) {
    g_epoch.state = {};
    g_last_functional_keys = {};
    g_escape_state = {};
    g_direct_weapon_request = {};
    g_radio_off_request = {};
    g_trace_state_initialized = true;
    g_trace_last_valid = false;
    g_trace_last_frontend_active = frontend_active;
    g_trace_last_phone_visible = phone.visible;
    g_trace_last_map_active = g_epoch.map_active;
    g_trace_last_gamepad_valid = gamepad_valid;
    g_trace_last_gamepad_buttons = gamepad_buttons;
    g_trace_last_keys = {};
    return g_epoch;
  }

  g_epoch.state = state;
  for (size_t index = 0; index < state.keys.size(); ++index) {
    g_epoch.pressed_keys[index] =
        state.pressed_keys[index] != 0 ||
        (state.keys[index] != 0 && g_last_functional_keys[index] == 0);
    g_epoch.changed_keys[index] =
        state.pressed_keys[index] != 0 ||
        state.keys[index] != g_last_functional_keys[index];
  }
  g_epoch.escape_route = g_escape_state.Update(
      IsDown(state, VirtualKey::kEscape),
      IsPressed(g_epoch, VirtualKey::kEscape), frontend_active, phone.visible);
  g_last_functional_keys = state.keys;
  g_epoch.trace_sample |= state.keys != g_trace_last_keys || state.mouse_has_motion ||
                          state.mouse_wheel != 0 ||
                          state.mouse_reset_generation != g_trace_last_reset_generation ||
                          state.mouse_source != g_trace_last_source;
  g_trace_state_initialized = true;
  g_trace_last_valid = true;
  g_trace_last_frontend_active = frontend_active;
  g_trace_last_phone_visible = phone.visible;
  g_trace_last_map_active = g_epoch.map_active;
  g_trace_last_gamepad_valid = gamepad_valid;
  g_trace_last_gamepad_buttons = gamepad_buttons;
  g_trace_last_keys = state.keys;
  g_trace_last_reset_generation = state.mouse_reset_generation;
  g_trace_last_source = state.mouse_source;
  if (!g_mouse_conversion_initialized ||
      state.mouse_reset_generation != g_last_mouse_reset_generation) {
    ResetMouseConversion();
    g_last_mouse_reset_generation = state.mouse_reset_generation;
    g_mouse_conversion_initialized = true;
  }
  if (state.mouse_has_motion && state.mouse_source != g_last_mouse_source) {
    ResetMouseConversion();
    g_last_mouse_source = state.mouse_source;
  }
  if (state.mouse_has_motion) {
    const double frame_seconds = static_cast<double>(LoadFloat(base, kGameplayTimeStepAddress));
    g_epoch.mouse_x =
        g_mouse_x_quantizer.Quantize(state.mouse_dx, state.mouse_sensitivity, kMouseUnitsPerCount,
                                     frame_seconds, kReferenceFrameSeconds);
    const double y_delta = state.invert_mouse_y ? -state.mouse_dy : state.mouse_dy;
    g_epoch.mouse_y =
        g_mouse_y_quantizer.Quantize(y_delta, state.mouse_sensitivity, kMouseUnitsPerCount,
                                     frame_seconds, kReferenceFrameSeconds);
    g_epoch.map_mouse_x = g_map_mouse_x_quantizer.Quantize(
        state.mouse_dx, state.mouse_sensitivity, kMouseUnitsPerCount, frame_seconds,
        kReferenceFrameSeconds);
    g_epoch.map_mouse_y = g_map_mouse_y_quantizer.Quantize(
        state.mouse_dy, state.mouse_sensitivity, kMouseUnitsPerCount, frame_seconds,
        kReferenceFrameSeconds);
  }
  g_epoch.gyro_x = g_epoch.gyro_y = 0;
  const auto vehicle = ReadVehicleInputContext(base);
  const bool aiming = IsDown(g_epoch.state, VirtualKey::kRButton) ||
      (gamepad_valid && gamepad_state.gamepad.left_trigger > 30);
  const bool gyro_allowed = REXCVAR_GET(gta4_motion_aim) && aiming && !frontend_active &&
      !phone.visible && !vehicle.vehicle && !GTA4_TouchTitleInputOwned() &&
      LoadU32(base, 0x82BA1D40) == 0;
  if (gyro_allowed) {
    const auto motion = gta4::GTA4MotionBridge::Get().Read(input_user);
    const auto gyro = BuildGyroAimActions(motion.controls_enabled && motion.fresh,
        motion.angular_velocity_rad_s, float(REXCVAR_GET(gta4_motion_aim_full_scale)),
        REXCVAR_GET(gta4_motion_aim_invert_x), REXCVAR_GET(gta4_motion_aim_invert_y));
    g_epoch.gyro_x = gyro.horizontal;
    g_epoch.gyro_y = gyro.vertical;
  }
  ArmContextRequests(base);
  return g_epoch;
}

InputEpoch ReadEpoch() {
  std::lock_guard lock(g_epoch_mutex);
  return g_epoch;
}

void InjectEpoch(uint8_t* base, uint32_t control, uint32_t active_gameplay_control,
                 const InputEpoch& epoch, uint32_t caller) {
  if (!epoch.valid || !control) {
    return;
  }
  const uint32_t control_user = LoadU32(base, control + kControlUserIndexOffset);
  const bool user_matches = control_user == epoch.state.user_index;
  // sub_822B7DD0 is GTA's per-device replay boundary. The object passed here
  // is the object the retail input update has just reset and populated. GTA
  // may subsequently transfer that state into the object returned by
  // sub_821B41E8; that downstream consumer selector is not an injection-owner
  // test. Requiring pointer equality with it drops keyboard gameplay actions
  // whenever the replay and consumer objects differ (the live vehicle trace
  // shows exactly that split).
  //
  // CaptureEpoch runs once at the outer GTA poll, before these replays, so its
  // persistent keys and relative pointer sample can safely be merged into each
  // replay object for the native user. The pointer sample is consumed only
  // once by CaptureEpoch, not once per replay object. This preserves GTA's
  // retail object selection without losing camera motion or vehicle input.
  const bool owns_gameplay = user_matches;
  const bool map_context = epoch.map_active;
  const bool phone_activity =
      owns_gameplay &&
      InjectPhoneActions(base, active_gameplay_control, epoch);
  if (!owns_gameplay) {
    if (REXCVAR_GET(gta4_native_input_trace) && epoch.trace_sample) {
      REXLOG_INFO(
          "gta4-input: interface-only epoch={} caller={:08X} control={:08X} "
          "active_control={:08X} control_user={} native_user={} user_match={}",
          epoch.sequence, caller, control, active_gameplay_control, control_user,
          epoch.state.user_index, user_matches);
    }
  }

  bool gameplay_activity = false;
  bool context_activity = false;
  const VehicleInputContext vehicle_context = ReadVehicleInputContext(base);
  const bool helicopter_driver =
      vehicle_context.vehicle && vehicle_context.is_driver && vehicle_context.is_heli;
  for (const ButtonBinding& binding : kButtonBindings) {
    const KeyboardActionRoute route =
        ClassifyKeyboardActionRoute(static_cast<uint32_t>(binding.action));
    // sub_822B7DD0 has just reset and populated this object. Merge frontend
    // records here so a nested replay in sub_8224FFC8 cannot erase them before
    // the generated consumer reads them. Phone records are handled above on
    // the exact active gameplay control selected by sub_821B41E8.
    if (IsContextAction(route)) {
      if (UsesActiveGameplayControl(route)) {
        continue;
      }
      if (owns_gameplay && ShouldInjectInterfaceBinding(binding, epoch) &&
          IsNativeActionDown(epoch, binding.key)) {
        MergeButton(base, control, binding.action, kPressed);
        context_activity = true;
      }
      continue;
    }
    if (map_context &&
        (IsGameplayAction(binding.action) || binding.key == VirtualKey::kLButton ||
         binding.key == VirtualKey::kRButton)) {
      continue;
    }
    if (helicopter_driver &&
        (binding.action == Action::kVehicleAttack ||
         binding.action == Action::kVehicleAttack2)) {
      continue;
    }
    if (IsNativeActionDown(epoch, binding.key) &&
        (owns_gameplay || IsGlobalKeyboardAction(binding.action))) {
      gameplay_activity |= MergeButton(base, control, binding.action, kPressed);
    }
  }
  if (!owns_gameplay) {
    if (gameplay_activity || context_activity) {
      StoreU32(base, control + kLastInputTimeOffset, LoadU32(base, kGameInputTimeAddress));
    }
    if (REXCVAR_GET(gta4_native_input_trace) && epoch.trace_sample) {
      REXLOG_INFO(
          "gta4-input: replay-not-owned epoch={} caller={:08X} "
          "control={:08X} active_control={:08X} phone-visible={} "
          "context-owner=matching-replay",
          epoch.sequence, caller, control, active_gameplay_control,
          epoch.phone_visible);
    }
    return;
  }

  // The map has its own signed pan records and edge-triggered waypoint action.
  // Do not fan pointer or wheel input into gameplay/camera/radio actions while
  // that screen owns the pointer.
  if (map_context) {
    return;
  }

  if (vehicle_context.vehicle) {
    // These are PC-only aliases whose Xbox action records are also consumed
    // outside their named context. Gate them on GTA's authoritative current
    // vehicle state instead of broadcasting them into on-foot gameplay.
    // Q/Z use the vehicle-only action-42 predicate in sub_823CF9C0.
    // ArmContextRequests routes them at that consumer so they cannot also
    // toggle headlights or drop a GTA Race weapon through the shared record.
    if (vehicle_context.is_driver && vehicle_context.is_heli) {
      // Physical LMB/Shift now publish A/X through the controller. Preserve
      // only the separate touch fire source and existing Numpad0 alias here.
      if (IsDown(epoch.state, VirtualKey::kNumpad0)) {
        gameplay_activity |=
            MergeButton(base, control, Action::kVehicleAttack2, kPressed);
      }
    }

    // GTA Race's generated drop paths pair action 42 with the global byte
    // written by the SET_PLAYER_CAN_DROP_WEAPONS_IN_CAR script native.
    // Gate the R alias on that same authoritative state so ordinary driving
    // keeps H as headlights and R cannot trigger an unrelated action-42 path.
    if (vehicle_context.is_driver && vehicle_context.can_drop_weapon &&
        IsDown(epoch.state, VirtualKey::kR)) {
      gameplay_activity |=
          MergeButton(base, control, Action::kVehicleHeadlight, kPressed);
    }
  }

  const int32_t horizontal = NativeDigitalAxis(epoch, VirtualKey::kA, VirtualKey::kD);
  const int32_t vertical = NativeDigitalAxis(epoch, VirtualKey::kW, VirtualKey::kS);
  gameplay_activity |= MergeAxis(base, control, Action::kMoveLeft, Action::kMoveRight, horizontal);
  gameplay_activity |= MergeAxis(base, control, Action::kMoveUp, Action::kMoveDown, vertical);
  gameplay_activity |=
      MergeAxis(base, control, Action::kVehicleMoveLeft, Action::kVehicleMoveRight, horizontal);

  // GTA's 32/33 pair is vehicle pitch/weight-shift, and its follow-camera also
  // observes that axis. W/S must never feed it: W/S already use 40/41 for
  // accelerate/brake, and doing both caused the camera to pitch continuously.
  // The same guest pair represents ground-vehicle weight shift and aircraft
  // pitch. Route the aliases by the authoritative vehicle class: Shift/Ctrl
  // lean in non-helicopters, while Numpad 8/2 pitch helicopters. In
  // particular, helicopter secondary-fire Shift must not also pitch forward.
  int32_t vehicle_pitch = 0;
  if (vehicle_context.vehicle && vehicle_context.is_driver) {
    vehicle_pitch = vehicle_context.is_heli
                        ? NativeDigitalAxis(epoch, VirtualKey::kNumpad8,
                                            VirtualKey::kNumpad2)
                        : NativeDigitalAxis(epoch, VirtualKey::kShift,
                                            VirtualKey::kControl);
  }
  gameplay_activity |=
      MergeAxis(base, control, Action::kVehicleMoveUp, Action::kVehicleMoveDown, vehicle_pitch);

  // sub_825ED3C8 reads signed action 24 (26 for alternate pad controls).
  // Negative input reduces the scope FOV; positive input increases it. The
  // paired records are centered axes, so treating zoom-out as button 25
  // leaves the actual consumer neutral and reverses zoom-in's direction.
  const char* wheel_route = "none";
  if (!epoch.frontend_active && epoch.state.mouse_wheel > 0) {
    if (!vehicle_context.vehicle) {
      wheel_route = "on-foot";
      gameplay_activity |=
          MergeButton(base, control, Action::kNextWeapon, kPressed);
      gameplay_activity |=
          MergeAxis(base, control, Action::kSniperZoomIn, Action::kSniperZoomOut,
                    -kPressed);
      gameplay_activity |=
          MergeAxis(base, control, Action::kSniperZoomInAlternate,
                    Action::kSniperZoomOutAlternate, -kPressed);
    } else if (vehicle_context.is_driver) {
      wheel_route = "driver-radio";
      gameplay_activity |=
          MergeButton(base, control, Action::kVehicleNextRadio, kPressed);
    }
  } else if (!epoch.frontend_active && epoch.state.mouse_wheel < 0) {
    if (!vehicle_context.vehicle) {
      wheel_route = "on-foot";
      gameplay_activity |=
          MergeButton(base, control, Action::kPrevWeapon, kPressed);
      gameplay_activity |=
          MergeAxis(base, control, Action::kSniperZoomIn, Action::kSniperZoomOut,
                    kPressed);
      gameplay_activity |=
          MergeAxis(base, control, Action::kSniperZoomInAlternate,
                    Action::kSniperZoomOutAlternate, kPressed);
    } else if (vehicle_context.is_driver) {
      wheel_route = "driver-radio";
      gameplay_activity |=
          MergeButton(base, control, Action::kVehiclePrevRadio, kPressed);
    }
  }

  if (!vehicle_context.vehicle) {
    gameplay_activity |= MergeAxis(base, control, Action::kLookLeft, Action::kLookRight, epoch.gyro_x);
    gameplay_activity |= MergeAxis(base, control, Action::kLookUp, Action::kLookDown, epoch.gyro_y);
  }

  const VehicleMouseActions mouse = RouteVehicleMouse(
      helicopter_driver, IsDown(epoch.state, VirtualKey::kRButton),
      epoch.mouse_x, epoch.mouse_y);
  // sub_822ABEE0 consumes 32/33 through sub_822B8178 for pitch, and
  // directly decodes 57/58 for yaw. Use the same actions as the numpad so
  // the retail aircraft simulation and controller magnitudes remain intact.
  gameplay_activity |= MergeAxis(base, control, Action::kVehicleMoveUp,
                                Action::kVehicleMoveDown, mouse.helicopter_pitch);
  if (mouse.helicopter_yaw) {
    const uint32_t left = ActionAddress(control, Action::kVehicleFlyYawLeft);
    const uint32_t right = ActionAddress(control, Action::kVehicleFlyYawRight);
    const VehicleYawButtons yaw = MergeVehicleYawButtons(
        ReadActionRaw(base, control, Action::kVehicleFlyYawLeft),
        ReadActionRaw(base, control, Action::kVehicleFlyYawRight),
        mouse.helicopter_yaw);
    StoreU8(base, left + kActionCurrentOffset,
            rex::input::mnk::EncodeActionMagnitude(LoadU8(base, left), yaw.left));
    StoreU8(base, right + kActionCurrentOffset,
            rex::input::mnk::EncodeActionMagnitude(LoadU8(base, right), yaw.right));
    gameplay_activity = true;
  }
  gameplay_activity |=
      MergeAxis(base, control, Action::kLookLeft, Action::kLookRight, mouse.camera_x);
  gameplay_activity |= MergeAxis(base, control, Action::kLookUp, Action::kLookDown, mouse.camera_y);
  gameplay_activity |=
      MergeAxis(base, control, Action::kVehicleGunLeft, Action::kVehicleGunRight, mouse.camera_x);
  gameplay_activity |=
      MergeAxis(base, control, Action::kVehicleGunUp, Action::kVehicleGunDown, mouse.camera_y);
  gameplay_activity |=
      MergeAxis(base, control, Action::kVehicleLookLeft, Action::kVehicleLookRight, mouse.camera_x);

  if (gameplay_activity || context_activity) {
    StoreU32(base, control + kLastInputTimeOffset, LoadU32(base, kGameInputTimeAddress));
  }

  if (REXCVAR_GET(gta4_native_input_trace) && epoch.trace_sample) {
    if (active_gameplay_control &&
        (phone_activity || IsDown(epoch.state, VirtualKey::kUp) ||
         IsDown(epoch.state, VirtualKey::kEscape) ||
         IsDown(epoch.state, VirtualKey::kBack) ||
         IsDown(epoch.state, VirtualKey::kDelete))) {
      REXLOG_INFO(
          "gta4-input: phone-route epoch={} replay_control={:08X} "
          "active_control={:08X} requested={} take_out={} put_away={}",
          epoch.sequence, control, active_gameplay_control, phone_activity,
          ReadActionRaw(base, active_gameplay_control, Action::kPhoneTakeOut),
          ReadActionRaw(base, active_gameplay_control, Action::kPhonePutAway));
    }
    REXLOG_INFO(
        "gta4-input: inject epoch={} caller={:08X} control={:08X} "
        "active_control={:08X} user={} "
        "wasd={}/{}/{}/{} f={} return={} space={} back={} delete={} "
        "escape={} shift={} control={} mouse={}/{} wheel={} "
        "actions=enter:{}/phone_out:{}/phone_put_away:{}/steer:{}/{}:"
        "vertical:{}/{}:accelerate:{}/brake:{}/exit:{}/handbrake:{}:"
        "frontend:{}/{}/{}",
        epoch.sequence, caller, control, active_gameplay_control, control_user,
        IsDown(epoch.state, VirtualKey::kW),
        IsDown(epoch.state, VirtualKey::kA), IsDown(epoch.state, VirtualKey::kS),
        IsDown(epoch.state, VirtualKey::kD), IsDown(epoch.state, VirtualKey::kF),
        IsDown(epoch.state, VirtualKey::kReturn), IsDown(epoch.state, VirtualKey::kSpace),
        IsDown(epoch.state, VirtualKey::kBack), IsDown(epoch.state, VirtualKey::kDelete),
        IsDown(epoch.state, VirtualKey::kEscape), IsDown(epoch.state, VirtualKey::kShift),
        IsDown(epoch.state, VirtualKey::kControl), epoch.mouse_x, epoch.mouse_y,
        epoch.state.mouse_wheel, ReadActionRaw(base, control, Action::kEnter),
        ReadActionRaw(base, control, Action::kPhoneTakeOut),
        ReadActionRaw(base, control, Action::kPhonePutAway),
        ReadActionRaw(base, control, Action::kVehicleMoveLeft),
        ReadActionRaw(base, control, Action::kVehicleMoveRight),
        ReadActionRaw(base, control, Action::kVehicleMoveUp),
        ReadActionRaw(base, control, Action::kVehicleMoveDown),
        ReadActionRaw(base, control, Action::kVehicleAccelerate),
        ReadActionRaw(base, control, Action::kVehicleBrake),
        ReadActionRaw(base, control, Action::kVehicleExit),
        ReadActionRaw(base, control, Action::kVehicleHandbrake),
        ReadActionRaw(base, control, Action::kFrontendAccept),
        ReadActionRaw(base, control, Action::kFrontendCancel),
        ReadActionRaw(base, control, Action::kFrontendPause));
    REXLOG_INFO(
        "gta4-input: context epoch={} control={:08X} ped={:08X} "
        "vehicle={:08X} vtable={:08X} driver={} heli={} race_drop={} "
        "keys=r:{}:shift:{}:num0:{}:num8:{}:num2:{} wheel={}:{} "
        "actions=primary:{}:secondary:{}:pitch:{}/{}:drop_headlight:{}:"
        "weapon:{}/{}:sniper:{}/{}:radio:{}/{}:hotwire:{}/{}",
        epoch.sequence, control, vehicle_context.ped, vehicle_context.vehicle,
        vehicle_context.vehicle_vtable, vehicle_context.is_driver,
        vehicle_context.is_heli, vehicle_context.can_drop_weapon,
        IsDown(epoch.state, VirtualKey::kR),
        IsDown(epoch.state, VirtualKey::kShift),
        IsDown(epoch.state, VirtualKey::kNumpad0),
        IsDown(epoch.state, VirtualKey::kNumpad8),
        IsDown(epoch.state, VirtualKey::kNumpad2), epoch.state.mouse_wheel,
        wheel_route, ReadActionRaw(base, control, helicopter_driver
                                                   ? Action::kVehicleAttack2
                                                   : Action::kVehicleAttack),
        ReadActionRaw(base, control, Action::kVehicleContextAction45),
        ReadActionRaw(base, control, Action::kVehicleMoveUp),
        ReadActionRaw(base, control, Action::kVehicleMoveDown),
        ReadActionRaw(base, control, Action::kVehicleHeadlight),
        ReadActionRaw(base, control, Action::kNextWeapon),
        ReadActionRaw(base, control, Action::kPrevWeapon),
        ReadActionRaw(base, control, Action::kSniperZoomIn),
        ReadActionRaw(base, control, Action::kSniperZoomOut),
        ReadActionRaw(base, control, Action::kVehicleNextRadio),
        ReadActionRaw(base, control, Action::kVehiclePrevRadio),
        ReadActionRaw(base, control, Action::kVehicleHotwireLeft),
        ReadActionRaw(base, control, Action::kVehicleHotwireRight));
  }
}

bool ClaimMapEpoch(uint64_t sequence) {
  std::lock_guard lock(g_epoch_mutex);
  if (g_last_map_epoch_sequence == sequence) {
    return false;
  }
  g_last_map_epoch_sequence = sequence;
  return true;
}

void ApplyMapEpoch(const PPCContext& entry_context, uint8_t* base) {
  const InputEpoch epoch = ReadEpoch();
  if (!epoch.valid || !epoch.map_active) {
    return;
  }

  PPCContext nested = entry_context;
  nested.r3.u32 = 1;
  __imp__sub_821B42B8(nested, base);
  const uint32_t control = nested.r3.u32;
  if (!control ||
      LoadU32(base, control + kControlUserIndexOffset) != epoch.state.user_index ||
      !ClaimMapEpoch(epoch.sequence)) {
    return;
  }

  const bool waypoint = IsPressed(epoch, VirtualKey::kRButton);
  const bool dragging = IsDown(epoch.state, VirtualKey::kLButton);
  bool changed = false;
  if (waypoint) {
    MergeButton(base, control, Action::kFrontendAccept, kPressed);
    changed = true;
  }
  const MapPanAction map_pan = DirectManipulationMapPan(epoch.map_mouse_x, epoch.map_mouse_y);
  if (dragging) {
    changed |= MergeSignedAction(base, control, Action::kMapX, map_pan.horizontal);
    changed |= MergeSignedAction(base, control, Action::kMapY, map_pan.vertical);
  }

  uint32_t zoom_before = LoadU32(base, kMapZoomLevelAddress);
  uint32_t zoom_after = zoom_before;
  if (epoch.state.mouse_wheel > 0 && zoom_after < kMapZoomMaximum) {
    ++zoom_after;
  } else if (epoch.state.mouse_wheel < 0 && zoom_after > kMapZoomMinimum) {
    --zoom_after;
  }
  if (zoom_after != zoom_before) {
    StoreU32(base, kMapZoomLevelAddress, zoom_after);
    changed = true;
  }

  if (changed) {
    StoreU32(base, control + kLastInputTimeOffset, LoadU32(base, kGameInputTimeAddress));
  }
  if (REXCVAR_GET(gta4_native_input_trace) &&
      (epoch.trace_sample || waypoint || dragging || zoom_before != zoom_after)) {
    REXLOG_INFO(
        "gta4-input: map epoch={} control={:08X} waypoint={} dragging={} "
        "pointer-motion={}/{} action={}/{} zoom={}->{} settled={}",
        epoch.sequence, control, waypoint, dragging, epoch.map_mouse_x, epoch.map_mouse_y,
        map_pan.horizontal, map_pan.vertical, zoom_before, zoom_after,
        LoadU8(base, kMapZoomSettledAddress));
  }
}

bool ConsumeDirectWeaponSelection(PPCContext& ctx,
                                   DirectWeaponRequest& request) {
  std::lock_guard lock(g_epoch_mutex);
  if (!g_direct_weapon_request.armed ||
      !g_direct_weapon_request.predicate_forced ||
      g_direct_weapon_request.epoch != g_epoch.sequence ||
      ctx.lr != kDirectWeaponSelectionCaller ||
      ctx.r3.u32 != g_direct_weapon_request.ped + kPedWeaponManagerOffset) {
    return false;
  }

  request = g_direct_weapon_request;
  g_direct_weapon_request.armed = false;
  return true;
}

uint32_t AvailableDirectWeaponSlot(const PPCContext& parent, uint8_t* base,
                                  uint32_t manager, uint32_t requested_slot,
                                  uint32_t original_slot) {
  if (!requested_slot) {
    return requested_slot;
  }
  // The generated forward search tests ownership, ammo and weapon fire type
  // without changing the weapon manager. An unavailable number-row slot must
  // leave the current weapon selected rather than selecting unarmed.
  PPCContext nested = parent;
  nested.r3.u32 = manager;
  nested.r4.u32 = requested_slot - 1;
  __imp__sub_823D52D0(nested, base);
  return nested.r3.u32 == requested_slot ? requested_slot : original_slot;
}

void MaybeForceRadioOffPredicate(PPCContext& ctx, uint8_t* base,
                                 uint32_t action_record, uint32_t caller,
                                 uint32_t entity) {
  std::lock_guard lock(g_epoch_mutex);
  constexpr uint32_t kActionOffset =
      kActionArrayOffset +
      static_cast<uint32_t>(Action::kVehiclePrevRadio) * kActionStride;
  const uint32_t control =
      action_record >= kActionOffset ? action_record - kActionOffset : 0;
  if (!g_radio_off_request.armed ||
      g_radio_off_request.epoch != g_epoch.sequence ||
      caller != kRadioOffPredicateCaller || !control ||
      LoadU32(base, control + kControlUserIndexOffset) !=
          g_radio_off_request.user ||
      !entity || LoadU32(base, entity + kRadioEntityVehicleOffset) !=
                     g_radio_off_request.vehicle) {
    return;
  }

  const bool radio_active =
      LoadU8(base, entity + kRadioEntityStationOffset) != 255 &&
      LoadU8(base, entity + kRadioEntityStateOffset) != 0;
  g_radio_off_request.armed = false;
  if (radio_active) {
    ctx.r3.u64 = 1;
  }
  if (REXCVAR_GET(gta4_native_input_trace)) {
    REXLOG_INFO(
        "gta4-input: radio-off epoch={} entity={:08X} vehicle={:08X} "
        "active={} forced={}",
        g_epoch.sequence, entity, g_radio_off_request.vehicle, radio_active,
        radio_active);
  }
}

}  // namespace

void MaybeForceDirectWeaponAction(PPCContext& ctx, uint8_t* base,
                                  uint32_t action_record, uint32_t caller) {
  std::lock_guard lock(g_epoch_mutex);
  const bool vehicle_caller =
      caller == kVehicleWeaponPressPredicateCaller ||
      caller == kVehicleWeaponReleasePredicateCaller;
  const uint32_t action_offset =
      kActionArrayOffset +
      static_cast<uint32_t>(vehicle_caller ? Action::kVehicleHeadlight
                                          : Action::kNextWeapon) * kActionStride;
  const uint32_t control =
      action_record >= action_offset ? action_record - action_offset : 0;
  if (!g_direct_weapon_request.armed ||
      g_direct_weapon_request.predicate_forced ||
      g_direct_weapon_request.epoch != g_epoch.sequence ||
      (!vehicle_caller && caller != kDirectWeaponPredicateCaller) ||
      vehicle_caller != g_direct_weapon_request.in_vehicle ||
      ctx.r29.u32 != g_direct_weapon_request.ped ||
      !control || LoadU32(base, control + kControlUserIndexOffset) !=
                      g_direct_weapon_request.user) {
    return;
  }

  ctx.r3.u64 = 1;
  g_direct_weapon_request.predicate_forced = true;
  if (REXCVAR_GET(gta4_native_input_trace)) {
    REXLOG_INFO(
        "gta4-input: direct-weapon-predicate epoch={} ped={:08X} "
        "control={:08X} slot={} caller={:08X}",
        g_epoch.sequence, g_direct_weapon_request.ped,
        control, g_direct_weapon_request.slot, caller);
  }
}

}  // namespace gta4::input

extern "C" void sub_828CCD60(PPCContext& ctx, uint8_t* base) {
  const uint32_t caller = ctx.lr;
  // Select bindings before retail polls XInput, and use that same selection
  // for this epoch's native overlay. No first-frame delay or duplicate keys.
  const bool helicopter_controls =
      gta4::input::ConfigureKeyboardControllerForPoll(ctx, base);
  const uint64_t touch_epoch = gta4::input::ReadEpoch().sequence + 1;
  GTA4_TouchConsumePoll(ctx, base, touch_epoch);
  __imp__sub_828CCD60(ctx, base);
  const gta4::input::InputEpoch epoch =
      gta4::input::CaptureEpoch(ctx, base, caller, helicopter_controls);
  gta4::input::ProcessPauseTabShoulders(ctx, base, epoch);
  gta4::GTA4MotionBridge::Get().SetReloadContextActive(
      !epoch.frontend_active && !epoch.phone_visible && !GTA4_TouchTitleInputOwned() &&
      gta4::input::LoadU32(base, 0x82BA1D40) == 0 &&
      !gta4::input::ReadVehicleInputContext(base).vehicle);
  if (rex::input::IsInputTraceEnabled() && epoch.valid) {
    uint32_t retail_controller_flags = 0;
    if (epoch.state.user_index < gta4::input::kRetailControllerRecordCount) {
      const uint32_t retail_controller =
          gta4::input::kRetailControllerRecordsAddress +
          epoch.state.user_index * gta4::input::kRetailControllerRecordStride;
      retail_controller_flags = gta4::input::LoadU32(
          base, retail_controller + gta4::input::kRetailControllerFlagsOffset);
    }
    for (rex::ui::VirtualKey key : gta4::input::kControlTraceKeys) {
      if (!gta4::input::IsChanged(epoch, key)) {
        continue;
      }
      const auto index = static_cast<uint16_t>(key);
      REXLOG_INFO(
          "input-e2e: seq={} stage=gta-key-epoch epoch={} caller={:08X} "
          "key={} vk={} down={} pressed={} user={} frontend-active={} "
          "phone-visible={} map-active={} gamepad-valid={} packet={} "
          "buttons={:04X} controller=a:{} x:{} start:{} dpad:{}/{}/{}/{} "
          "retail-flags={:08X} mapped=a:{} start:{} dpad:{}/{}/{}/{}",
          gta4::input::KeyEventSequence(epoch, key), epoch.sequence, caller,
          rex::input::InputTraceVirtualKeyName(index), index,
          gta4::input::IsDown(epoch.state, key),
          gta4::input::IsPressed(epoch, key), epoch.state.user_index,
          epoch.frontend_active, epoch.phone_visible, epoch.map_active,
          epoch.gamepad_valid, epoch.gamepad_packet, epoch.gamepad_buttons,
          (epoch.gamepad_buttons & rex::input::X_INPUT_GAMEPAD_A) != 0,
          (epoch.gamepad_buttons & rex::input::X_INPUT_GAMEPAD_X) != 0,
          (epoch.gamepad_buttons & rex::input::X_INPUT_GAMEPAD_START) != 0,
          (epoch.gamepad_buttons & rex::input::X_INPUT_GAMEPAD_DPAD_UP) != 0,
          (epoch.gamepad_buttons & rex::input::X_INPUT_GAMEPAD_DPAD_DOWN) != 0,
          (epoch.gamepad_buttons & rex::input::X_INPUT_GAMEPAD_DPAD_LEFT) != 0,
          (epoch.gamepad_buttons & rex::input::X_INPUT_GAMEPAD_DPAD_RIGHT) != 0,
          retail_controller_flags,
          (retail_controller_flags & gta4::input::kRetailControllerAFlag) != 0,
          (retail_controller_flags & gta4::input::kRetailControllerStartFlag) != 0,
          (retail_controller_flags & gta4::input::kRetailControllerDpadUpFlag) != 0,
          (retail_controller_flags & gta4::input::kRetailControllerDpadDownFlag) != 0,
          (retail_controller_flags & gta4::input::kRetailControllerDpadLeftFlag) != 0,
          (retail_controller_flags & gta4::input::kRetailControllerDpadRightFlag) != 0);
    }
  }
  if (rex::input::IsInputTraceEnabled() && epoch.trace_sample) {
    const uint64_t host_sequence = epoch.state.last_key_event_sequence != 0
                                       ? epoch.state.last_key_event_sequence
                                       : rex::input::NextInputTraceSequence();
    REXLOG_INFO(
        "input-e2e: seq={} stage=gta-poll epoch={} caller={:08X} valid={} user={} "
        "frontend-active={} phone-visible={} phone-index={} phone-object={:08X} "
        "phone-created={} phone-moving-offscreen={} phone-render-visible={} "
        "pause-menu-visible={} pause-menu-transition={} "
        "map-active={} helicopter-controls={} screen={} key-generation={} "
        "wasd={}/{}/{}/{} interface-keys={}/{}/{}/{}:{}/{}/{}/{} "
        "interface-changed={}/{}/{}/{}:{}/{}/{}/{} "
        "key-sequences=escape:{}:up:{}:down:{} "
        "mouse={}/{} wheel={} gamepad-valid={} packet={} buttons={:04X}",
        host_sequence, epoch.sequence, caller, epoch.valid, epoch.state.user_index,
        epoch.frontend_active, epoch.phone_visible, epoch.phone_render_index,
        epoch.phone_render_object, epoch.phone_created,
        epoch.phone_moving_offscreen, epoch.phone_render_visible,
        epoch.pause_menu_visible, epoch.pause_menu_transition, epoch.map_active,
        epoch.helicopter_controls,
        gta4::input::LoadU32(base, gta4::input::kCurrentScreenAddress),
        epoch.state.key_state_generation,
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kW),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kA),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kS),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kD),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kDown),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kUp),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kLeft),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kRight),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kReturn),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kBack),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kDelete),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kEscape),
        gta4::input::IsChanged(epoch, rex::ui::VirtualKey::kDown),
        gta4::input::IsChanged(epoch, rex::ui::VirtualKey::kUp),
        gta4::input::IsChanged(epoch, rex::ui::VirtualKey::kLeft),
        gta4::input::IsChanged(epoch, rex::ui::VirtualKey::kRight),
        gta4::input::IsChanged(epoch, rex::ui::VirtualKey::kReturn),
        gta4::input::IsChanged(epoch, rex::ui::VirtualKey::kBack),
        gta4::input::IsChanged(epoch, rex::ui::VirtualKey::kDelete),
        gta4::input::IsChanged(epoch, rex::ui::VirtualKey::kEscape),
        gta4::input::StoredKeyEventSequence(epoch, rex::ui::VirtualKey::kEscape),
        gta4::input::StoredKeyEventSequence(epoch, rex::ui::VirtualKey::kUp),
        gta4::input::StoredKeyEventSequence(epoch, rex::ui::VirtualKey::kDown),
        epoch.mouse_x,
        epoch.mouse_y, epoch.state.mouse_wheel, epoch.gamepad_valid,
        epoch.gamepad_packet, epoch.gamepad_buttons);
  }
  if (REXCVAR_GET(gta4_native_input_trace) &&
      (epoch.trace_sample || !gta4::input::g_logged_first_epoch)) {
    gta4::input::g_logged_first_epoch = true;
    REXLOG_INFO(
        "gta4-input: poll epoch={} caller={:08X} valid={} user={} "
        "motion={}/{} quantized={}/{} wheel={} reset={} source={}",
        epoch.sequence, caller, epoch.valid, epoch.state.user_index, epoch.state.mouse_dx,
        epoch.state.mouse_dy, epoch.mouse_x, epoch.mouse_y, epoch.state.mouse_wheel,
        epoch.state.mouse_reset_generation, static_cast<uint32_t>(epoch.state.mouse_source));
  }
}

extern "C" void sub_822B7DD0(PPCContext& ctx, uint8_t* base) {
  const uint32_t control = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  __imp__sub_822B7DD0(ctx, base);
  // Ask GTA which control object its current player/camera/vehicle consumers
  // use. Calling the generated implementation directly avoids recursively
  // entering a hook and preserves the authoritative retail selection logic.
  PPCContext active_control_context{};
  __imp__sub_821B41E8(active_control_context, base);
  const uint32_t active_gameplay_control = active_control_context.r3.u32;
  const gta4::input::InputEpoch epoch = gta4::input::ReadEpoch();
  const bool native_user_control =
      control && epoch.valid &&
      gta4::input::LoadU32(
          base, control + gta4::input::kControlUserIndexOffset) ==
          epoch.state.user_index;
  if (rex::input::IsInputTraceEnabled() &&
      gta4::input::EpochHasFocusedTraceInput(epoch)) {
    REXLOG_INFO(
        "input-e2e: seq={} stage=gta-replay-boundary epoch={} caller={:08X} "
        "control={:08X} active-control={:08X} control-user={} native-user={} "
        "user-match={}",
        gta4::input::FocusedTraceSequence(epoch), epoch.sequence, caller,
        control, active_gameplay_control,
        control
            ? gta4::input::LoadU32(
                  base,
                  control + gta4::input::kControlUserIndexOffset)
            : 0,
        epoch.state.user_index, native_user_control);
  }
  if (native_user_control) {
    gta4::input::PrepareFrontendHistory(base, control, epoch.sequence, true);
    gta4::input::PreparePhoneHistory(base, control, epoch.sequence, true);
    gta4::input::TraceActionState(
        "gta-actions-controller", epoch,
        gta4::input::CaptureActionTrace(base, control),
        gta4::input::g_controller_action_trace);
  }
  gta4::input::InjectEpoch(base, control, active_gameplay_control, epoch, caller);
  if (native_user_control) {
    gta4::input::InjectFrontendScroll(ctx, base, control, epoch, true);
    gta4::input::CommitFrontendHistory(base, control);
    gta4::input::CommitPhoneHistory(base, control);
    gta4::input::TraceActionState(
        "gta-actions-keyboard", epoch,
        gta4::input::CaptureActionTrace(base, control),
        gta4::input::g_keyboard_action_trace);
  }
  GTA4_TouchObserveControlReplay(ctx, base, control, caller, epoch.sequence);
  if (native_user_control) {
    gta4::input::TraceActionState(
        "gta-actions-final", epoch,
        gta4::input::CaptureActionTrace(base, control),
        gta4::input::g_final_action_trace);
  }
  gta4::input::TraceFocusedKeyRoutes(
      base, epoch, control, active_gameplay_control,
      native_user_control ? "gta-route-final" : "gta-route-non-owner");
}

extern "C" void sub_8224FFC8(PPCContext& ctx, uint8_t* base) {
  const uint32_t event = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  const bool one_shot_before =
      gta4::input::LoadU8(base, gta4::input::kFrontendOneShotFlagAddress) != 0;
  const gta4::input::InputEpoch epoch = gta4::input::ReadEpoch();
  // The generated frontend consumer obtains its action records through
  // sub_821B42B8(1). Normally the post-sub_822B7DD0 replay hook has already
  // merged keyboard state into this object. Keep this idempotent fallback for
  // event paths that consume an object without replaying it in the current
  // epoch. Phone actions are intentionally excluded: generated phone gameplay
  // code reads the sub_821B41E8 control family instead.
  const uint32_t interface_control =
      epoch.valid ? gta4::input::SelectInterfaceControl(ctx, base) : 0;
  const bool interface_user_matches =
      interface_control &&
      gta4::input::LoadU32(
          base, interface_control + gta4::input::kControlUserIndexOffset) ==
          epoch.state.user_index;
  bool injected_frontend = false;
  gta4::input::GtaActionTraceSnapshot frontend_before{};
  if (interface_user_matches) {
    gta4::input::TraceActionState(
        "gta-interface-before", epoch,
        gta4::input::CaptureActionTrace(base, interface_control),
        gta4::input::g_interface_before_trace);
    injected_frontend = gta4::input::InjectFrontendFallbackActions(
        base, interface_control, epoch);
    gta4::input::InjectFrontendScroll(ctx, base, interface_control, epoch, false);
    frontend_before =
        gta4::input::CaptureActionTrace(base, interface_control);
    gta4::input::TraceActionState(
        "gta-interface-injected", epoch,
        frontend_before,
        gta4::input::g_interface_after_trace);
  }

  __imp__sub_8224FFC8(ctx, base);

  gta4::input::GtaActionTraceSnapshot frontend_after{};
  if (interface_user_matches) {
    frontend_after =
        gta4::input::CaptureActionTrace(base, interface_control);
    gta4::input::TraceActionState(
        "gta-interface-consumed", epoch, frontend_after,
        gta4::input::g_interface_consumed_trace);
  }

  if (rex::input::IsInputTraceEnabled() && (epoch.trace_sample || one_shot_before)) {
    const uint64_t host_sequence = epoch.state.last_key_event_sequence != 0
                                       ? epoch.state.last_key_event_sequence
                                       : rex::input::NextInputTraceSequence();
    REXLOG_INFO(
        "input-e2e: seq={} stage=gta-frontend epoch={} caller={:08X} event={} result={} "
        "screen={} control={:08X} control-user-match={} injected={} frontend-active={} "
        "phone-visible={} phone-index={} phone-object={:08X} one-shot={}->{} "
        "return={} space={} back={} delete={} escape={} up={} down={} "
        "actions-before=down:{}/up:{}/left:{}/right:{}/pause:{}/accept:{}/cancel:{} "
        "actions-after=down:{}/up:{}/left:{}/right:{}/pause:{}/accept:{}/cancel:{}",
        gta4::input::EpochHasFocusedTraceInput(epoch)
            ? gta4::input::FocusedTraceSequence(epoch)
            : host_sequence,
        epoch.sequence, caller, event, ctx.r3.u32,
        gta4::input::LoadU32(base, gta4::input::kCurrentScreenAddress),
        interface_control, interface_user_matches, injected_frontend,
        epoch.frontend_active,
        epoch.phone_visible, epoch.phone_render_index, epoch.phone_render_object,
        one_shot_before,
        gta4::input::LoadU8(base, gta4::input::kFrontendOneShotFlagAddress) != 0,
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kReturn),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kSpace),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kBack),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kDelete),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kEscape),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kUp),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kDown),
        frontend_before.frontend_down, frontend_before.frontend_up,
        frontend_before.frontend_left, frontend_before.frontend_right,
        frontend_before.frontend_pause, frontend_before.frontend_accept,
        frontend_before.frontend_cancel, frontend_after.frontend_down,
        frontend_after.frontend_up, frontend_after.frontend_left,
        frontend_after.frontend_right, frontend_after.frontend_pause,
        frontend_after.frontend_accept, frontend_after.frontend_cancel);
  }

  if (REXCVAR_GET(gta4_native_input_trace) && (epoch.trace_sample || one_shot_before)) {
    REXLOG_INFO(
        "gta4-input: frontend epoch={} caller={:08X} event={} result={} "
        "control={:08X} control_user_match={} injected={} frontend_active={} "
        "phone_visible={} phone_index={} phone_object={:08X} "
        "one_shot={}->{} return={} space={} back={} delete={} escape={} f={}",
        epoch.sequence, caller, event, ctx.r3.u32, interface_control,
        interface_user_matches, injected_frontend, epoch.frontend_active,
        epoch.phone_visible,
        epoch.phone_render_index, epoch.phone_render_object, one_shot_before,
        gta4::input::LoadU8(base, gta4::input::kFrontendOneShotFlagAddress) != 0,
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kReturn),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kSpace),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kBack),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kDelete),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kEscape),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kF));
  }
}

extern "C" void sub_823CF9C0(PPCContext& ctx, uint8_t* base) {
  const uint32_t phone_object = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  const gta4::input::InputEpoch epoch = gta4::input::ReadEpoch();

  PPCContext active_control_context{};
  __imp__sub_821B41E8(active_control_context, base);
  const uint32_t active_control = active_control_context.r3.u32;
  const gta4::input::GtaActionTraceSnapshot before =
      gta4::input::CaptureActionTrace(base, active_control);
  const bool trace = rex::input::IsInputTraceEnabled() &&
                     gta4::input::EpochHasFocusedTraceInput(epoch);
  const uint64_t sequence =
      trace ? gta4::input::FocusedTraceSequence(epoch) : 0;
  const uint8_t disabled_a_before =
      phone_object
          ? gta4::input::LoadU8(
                base, phone_object +
                          gta4::input::kPhoneConsumerDisableFlagAOffset)
          : 0;
  const uint8_t disabled_b_before =
      phone_object
          ? gta4::input::LoadU8(
                base, phone_object +
                          gta4::input::kPhoneConsumerDisableFlagBOffset)
          : 0;
  const uint32_t phone_state_before =
      phone_object
          ? gta4::input::LoadU32(
                base,
                phone_object + gta4::input::kPhoneConsumerStateOffset)
          : 0;
  if (trace) {
    REXLOG_INFO(
        "input-e2e: seq={} stage=gta-phone-consumer phase=enter epoch={} "
        "caller={:08X} object={:08X} active-control={:08X} control-user={} "
        "phone-visible={} object-gates={}/{} object-state={} "
        "keys=escape:{}:up:{}:down:{} key-sequences=escape:{}:up:{}:down:{} "
        "actions=take-out:{}/put-away:{}",
        sequence, epoch.sequence, caller, phone_object, active_control,
        active_control
            ? gta4::input::LoadU32(
                  base,
                  active_control + gta4::input::kControlUserIndexOffset)
            : 0,
        epoch.phone_visible, disabled_a_before, disabled_b_before,
        phone_state_before,
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kEscape),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kUp),
        gta4::input::IsDown(epoch.state, rex::ui::VirtualKey::kDown),
        gta4::input::StoredKeyEventSequence(epoch, rex::ui::VirtualKey::kEscape),
        gta4::input::StoredKeyEventSequence(epoch, rex::ui::VirtualKey::kUp),
        gta4::input::StoredKeyEventSequence(epoch, rex::ui::VirtualKey::kDown),
        before.phone_take_out, before.phone_put_away);
  }

  __imp__sub_823CF9C0(ctx, base);

  if (trace) {
    const gta4::input::PhoneVisibilityContext phone_after =
        gta4::input::ReadPhoneVisibilityContext(base);
    const gta4::input::GtaActionTraceSnapshot after =
        gta4::input::CaptureActionTrace(base, active_control);
    REXLOG_INFO(
        "input-e2e: seq={} stage=gta-phone-consumer phase=exit epoch={} "
        "caller={:08X} object={:08X} active-control={:08X} "
        "phone-visible={}->{} object-gates={}/{}->{}/{} object-state={}->{} "
        "actions=take-out:{}/{} put-away:{}/{}",
        sequence, epoch.sequence, caller, phone_object, active_control,
        epoch.phone_visible, phone_after.visible, disabled_a_before,
        disabled_b_before,
        phone_object
            ? gta4::input::LoadU8(
                  base, phone_object +
                            gta4::input::kPhoneConsumerDisableFlagAOffset)
            : 0,
        phone_object
            ? gta4::input::LoadU8(
                  base, phone_object +
                            gta4::input::kPhoneConsumerDisableFlagBOffset)
            : 0,
        phone_state_before,
        phone_object
            ? gta4::input::LoadU32(
                  base,
                  phone_object + gta4::input::kPhoneConsumerStateOffset)
            : 0,
        before.phone_take_out, after.phone_take_out,
        before.phone_put_away, after.phone_put_away);
  }
}

extern "C" void sub_82252488(PPCContext& ctx, uint8_t* base) {
  const PPCContext entry_context = ctx;
  if (ctx.r3.u32) {
    gta4::input::ApplyMapEpoch(entry_context, base);
  }
  __imp__sub_82252488(ctx, base);
}

extern "C" void sub_823D5800(PPCContext& ctx, uint8_t* base) {
  const uint32_t ped_weapon_manager = ctx.r3.u32;
  gta4::input::DirectWeaponRequest request;
  const bool direct = gta4::input::ConsumeDirectWeaponSelection(ctx, request);
  const auto previous_candidates = gta4::input::g_vehicle_weapon_candidates;
  if (direct) {
    const uint32_t original_slot =
        gta4::input::LoadU32(base, ped_weapon_manager);
    if (request.in_vehicle) {
      gta4::input::g_vehicle_weapon_candidates = {
          .manager = ped_weapon_manager,
          .original_slot = original_slot,
          .requested_slot = request.slot,
          .request = request.request,
      };
    } else {
      ctx.r4.u32 = 0;
      ctx.r5.u32 = gta4::input::AvailableDirectWeaponSlot(
          ctx, base, ped_weapon_manager, request.slot, original_slot);
    }
  }
  const uint32_t operation = ctx.r4.u32;
  const uint32_t slot = ctx.r5.u32;
  __imp__sub_823D5800(ctx, base);
  gta4::input::g_vehicle_weapon_candidates = previous_candidates;
  if (direct && REXCVAR_GET(gta4_native_input_trace)) {
    REXLOG_INFO(
        "gta4-input: direct-weapon-select manager={:08X} operation={} "
        "slot={} result={}",
        ped_weapon_manager, operation, slot, ctx.r3.u32);
  }
}

extern "C" void sub_823D5358(PPCContext& ctx, uint8_t* base) {
  auto& policy = gta4::input::g_vehicle_weapon_candidates;
  switch (policy.Route(ctx.r3.u32, ctx.lr)) {
    case gta4::input::VehicleWeaponCandidateRoute::kAscending:
      // Reverse the vehicle's descending search; its generated caller still
      // checks the returned candidate against the current vehicle's rules.
      __imp__sub_823D52D0(ctx, base);
      return;
    case gta4::input::VehicleWeaponCandidateRoute::kRequestedSlot:
      ctx.r3.u64 = gta4::input::AvailableDirectWeaponSlot(
          ctx, base, policy.manager, policy.requested_slot,
          policy.original_slot);
      return;
    case gta4::input::VehicleWeaponCandidateRoute::kOriginalSlot:
      ctx.r3.u64 = policy.original_slot;
      return;
    case gta4::input::VehicleWeaponCandidateRoute::kRetail:
      __imp__sub_823D5358(ctx, base);
      return;
  }
}

extern "C" void sub_822D3230(PPCContext& ctx, uint8_t* base) {
  const uint32_t action_record = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  __imp__sub_822D3230(ctx, base);
  gta4::input::MaybeForceDirectWeaponAction(ctx, base, action_record, caller);
}

extern "C" void sub_822D4158(PPCContext& ctx, uint8_t* base) {
  const uint32_t action_record = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  const uint32_t radio_entity = ctx.r31.u32;
  __imp__sub_822D4158(ctx, base);
  gta4::input::MaybeForceRadioOffPredicate(ctx, base, action_record, caller,
                                           radio_entity);
}

extern "C" void sub_825D1AF8(PPCContext& ctx, uint8_t* base) {
  const uint32_t call_context = ctx.r3.u32;
  __imp__sub_825D1AF8(ctx, base);
  const uint64_t epoch = gta4::input::ReadEpoch().sequence;
  const gta4::input::ParachuteScriptContext parachute =
      gta4::input::ReadParachuteScriptContext(base);
  if (parachute.active) {
    gta4::input::ObserveTouchParachuteState(parachute.state, epoch);
  }
  const bool forced =
      gta4::input::ForceParachuteRawButtonResult(base, call_context) |
      gta4::input::MergeTouchScriptQueryResult(
          base, call_context, gta4::input::TouchScriptQueryKind::kRawButton,
          epoch);
  if (forced && REXCVAR_GET(gta4_native_input_trace)) {
    REXLOG_INFO("gta4-input: parachute smoke button forced");
  }
}

extern "C" void sub_825D1B88(PPCContext& ctx, uint8_t* base) {
  const uint32_t call_context = ctx.r3.u32;
  __imp__sub_825D1B88(ctx, base);
  const uint64_t epoch = gta4::input::ReadEpoch().sequence;
  const gta4::input::ParachuteScriptContext parachute =
      gta4::input::ReadParachuteScriptContext(base);
  if (parachute.active) {
    gta4::input::ObserveTouchParachuteState(parachute.state, epoch);
  }
  const bool forced =
      gta4::input::ForceParachuteControlResult(base, call_context, false, 1) |
      gta4::input::MergeTouchScriptQueryResult(
          base, call_context, gta4::input::TouchScriptQueryKind::kControlHeld,
          epoch);
  if (forced && REXCVAR_GET(gta4_native_input_trace)) {
    REXLOG_INFO("gta4-input: parachute held control forced");
  }
}

extern "C" void sub_825D1BD0(PPCContext& ctx, uint8_t* base) {
  const uint32_t call_context = ctx.r3.u32;
  __imp__sub_825D1BD0(ctx, base);
  const uint64_t epoch = gta4::input::ReadEpoch().sequence;
  const gta4::input::ParachuteScriptContext parachute =
      gta4::input::ReadParachuteScriptContext(base);
  if (parachute.active) {
    gta4::input::ObserveTouchParachuteState(parachute.state, epoch);
  }
  const bool forced =
      gta4::input::ForceParachuteControlResult(base, call_context, true, 1) |
      gta4::input::MergeTouchScriptQueryResult(
          base, call_context,
          gta4::input::TouchScriptQueryKind::kControlPressed, epoch);
  if (forced && REXCVAR_GET(gta4_native_input_trace)) {
    REXLOG_INFO("gta4-input: parachute edge control forced");
  }
}

extern "C" void sub_825D1C18(PPCContext& ctx, uint8_t* base) {
  const uint32_t call_context = ctx.r3.u32;
  __imp__sub_825D1C18(ctx, base);
  const uint64_t epoch = gta4::input::ReadEpoch().sequence;
  const gta4::input::ParachuteScriptContext parachute =
      gta4::input::ReadParachuteScriptContext(base);
  if (parachute.active) {
    gta4::input::ObserveTouchParachuteState(parachute.state, epoch);
  }
  const bool forced =
      gta4::input::ForceParachuteControlResult(base, call_context, false, 255) |
      gta4::input::MergeTouchScriptQueryResult(
          base, call_context,
          gta4::input::TouchScriptQueryKind::kControlAnalog, epoch);
  if (forced && REXCVAR_GET(gta4_native_input_trace)) {
    REXLOG_INFO("gta4-input: parachute analogue brake forced");
  }
}

extern "C" void sub_825D20A0(PPCContext& ctx, uint8_t* base) {
  const uint32_t call_context = ctx.r3.u32;
  __imp__sub_825D20A0(ctx, base);
  const uint64_t epoch = gta4::input::ReadEpoch().sequence;
  const gta4::input::ParachuteScriptContext parachute =
      gta4::input::ReadParachuteScriptContext(base);
  if (parachute.active) {
    gta4::input::ObserveTouchParachuteState(parachute.state, epoch);
  }
  const bool forced =
      gta4::input::ApplyParachuteAnalogueSticks(base, call_context) |
      gta4::input::MergeTouchScriptAnalogueStickResults(base, call_context,
                                                        epoch);
  if (forced && REXCVAR_GET(gta4_native_input_trace)) {
    REXLOG_INFO("gta4-input: parachute keyboard stick forced");
  }
}

namespace gta4::input {
namespace {

enum class ScriptInputQueryTraceKind : uint8_t {
  kRawHeld,
  kRawPressed,
  kControlHeld,
  kControlPressed,
};

void TraceUpScriptInputQuery(uint8_t* base, ScriptInputQueryTraceKind kind,
                             uint32_t group, uint32_t index, uint32_t caller,
                             uint32_t result) {
  if (!rex::input::IsInputTraceEnabled()) {
    return;
  }
  const InputEpoch epoch = ReadEpoch();
  if (!epoch.valid ||
      (!IsDown(epoch.state, VirtualKey::kUp) &&
       !IsChanged(epoch, VirtualKey::kUp))) {
    return;
  }
  struct QueryKey {
    ScriptInputQueryTraceKind kind;
    uint32_t group;
    uint32_t index;
  };
  static thread_local uint64_t traced_epoch = 0;
  static thread_local std::array<QueryKey, 64> traced_queries{};
  static thread_local size_t traced_count = 0;
  if (traced_epoch != epoch.sequence) {
    traced_epoch = epoch.sequence;
    traced_count = 0;
  }
  for (size_t entry = 0; entry < traced_count; ++entry) {
    const QueryKey& key = traced_queries[entry];
    if (key.kind == kind && key.group == group && key.index == index) {
      return;
    }
  }
  if (traced_count == traced_queries.size()) {
    return;
  }
  traced_queries[traced_count++] = {kind, group, index};

  // sub_825D1308/sub_825D1468 select these raw pad objects; button 8
  // reads Up at +60/+140. sub_828CC9D0 maps XInput D-pad Up to internal
  // device bit 0x1000 before sub_82208F50 builds these raw button records.
  // sub_822094F8 maintains their current/history
  // arrays independently of the semantic actions used by control queries.
  const uint32_t selected_pad = LoadU32(base, 0x82A9172C);
  const uint32_t raw_pad = group >= 4 ? 0x82B2A208
                           : selected_pad < 4 ? 0x82B29F18 + selected_pad * 188
                                              : 0;
  const char* query = "control-pressed";
  switch (kind) {
    case ScriptInputQueryTraceKind::kRawHeld:
      query = "raw-held";
      break;
    case ScriptInputQueryTraceKind::kRawPressed:
      query = "raw-pressed";
      break;
    case ScriptInputQueryTraceKind::kControlHeld:
      query = "control-held";
      break;
    case ScriptInputQueryTraceKind::kControlPressed:
      break;
  }
  REXLOG_INFO(
      "input-e2e: seq={} stage=gta-script-input epoch={} query={} group={} "
      "index={} caller={:08X} result={} up={} up-pressed={} up-changed={} "
      "frontend-active={} phone-visible={} phone-created={} selected-pad={} "
      "raw-pad={:08X} raw-up-current={} raw-up-previous={}",
      FocusedTraceSequence(epoch), epoch.sequence, query, group, index, caller,
      result, IsDown(epoch.state, VirtualKey::kUp),
      IsPressed(epoch, VirtualKey::kUp), IsChanged(epoch, VirtualKey::kUp),
      epoch.frontend_active, epoch.phone_visible, epoch.phone_created,
      selected_pad, raw_pad, raw_pad ? LoadU32(base, raw_pad + 60) : 0,
      raw_pad ? LoadU32(base, raw_pad + 140) : 0);
}

}  // namespace
}  // namespace gta4::input

extern "C" void sub_825D1308(PPCContext& ctx, uint8_t* base) {
  const uint32_t group = ctx.r3.u32;
  const uint32_t button = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  __imp__sub_825D1308(ctx, base);
  gta4::input::TraceUpScriptInputQuery(
      base, gta4::input::ScriptInputQueryTraceKind::kRawHeld, group, button,
      caller, ctx.r3.u32);
}

extern "C" void sub_825D1468(PPCContext& ctx, uint8_t* base) {
  const uint32_t group = ctx.r3.u32;
  const uint32_t button = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  __imp__sub_825D1468(ctx, base);
  gta4::input::TraceUpScriptInputQuery(
      base, gta4::input::ScriptInputQueryTraceKind::kRawPressed, group, button,
      caller, ctx.r3.u32);
}

extern "C" void sub_825D1908(PPCContext& ctx, uint8_t* base) {
  const uint32_t group = ctx.r3.u32;
  const uint32_t action = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  __imp__sub_825D1908(ctx, base);
  gta4::input::TraceUpScriptInputQuery(
      base, gta4::input::ScriptInputQueryTraceKind::kControlHeld, group, action,
      caller, ctx.r3.u32);
}

extern "C" void sub_825D1980(PPCContext& ctx, uint8_t* base) {
  const uint32_t group = ctx.r3.u32;
  const uint32_t action = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  __imp__sub_825D1980(ctx, base);
  gta4::input::TraceUpScriptInputQuery(
      base, gta4::input::ScriptInputQueryTraceKind::kControlPressed, group, action,
      caller, ctx.r3.u32);
}
