#include <cassert>
#include <iostream>
#include <sys/mman.h>

#include "gta4_init.h"
void __imp__sub_822B7958(PPCContext&, uint8_t*);
#include "input/context_touch_controls.cpp"
#include "input/context_touch_camera_hooks.cpp"
#include "touch_samples.h"

review::Setting<bool> gta4_touch_trace{false};
namespace {
gta4::input::TouchContextSnapshot facts;
gta4::input::TouchVisiblePromptSnapshot prompts;
rex::input::TouchPresentationState presentation;
rex::input::TouchGamepadProvider provider = nullptr;
bool title_owned = false;
bool controls_enabled = true;
bool pointer_allowed = true;
uint64_t next_epoch = 1;
uint64_t next_generation = 1;
uint32_t decoder_calls = 0;
int32_t decoder_result = 0;
uint8_t* guest = nullptr;
PPCContext ppc;
}

void __imp__sub_822B7958(PPCContext& context, uint8_t*) {
  ++decoder_calls;
  context.r3.u64 = static_cast<uint32_t>(decoder_result);
}
void GTA4_RegisterTouchExtension(GTA4TouchExtension) noexcept {}
void GTA4_CancelTouchGameplayReplay() noexcept {}
bool GTA4_TouchTitleInputOwned() noexcept { return title_owned; }
namespace rex::input {
bool TouchControlsActive() noexcept { return controls_enabled; }
bool TouchControlsVisible() noexcept { return controls_enabled; }
bool TouchPointerInputActive() noexcept {
  return pointer_allowed && presentation.valid && presentation.focused;
}
bool GetTouchPresentationState(TouchPresentationState* output) noexcept { *output = presentation; return true; }
void SetTouchGamepadProvider(TouchGamepadProvider value) noexcept { provider = value; }
}
namespace gta4::input {
TouchContextSnapshot GetTouchContextSnapshot() noexcept { return facts; }
TouchVisiblePromptSnapshot GetTouchVisiblePromptSnapshot(uint64_t, uint64_t generation) noexcept {
  return generation == prompts.generation ? prompts : TouchVisiblePromptSnapshot{};
}
}

namespace {
using namespace gta4::input;
using namespace touch_samples;
using Phase = rex::input::AbsolutePointerPhase;
using A = TouchAction;

uint64_t Poll() {
  facts.epoch = next_epoch++;
  facts.presentation_revision = ContextTouchPresentationRevision();
  BeginPoll(ppc, guest, facts.epoch, facts.frontend, facts.map);
  return facts.epoch;
}

void Reset(ContextTouchMode mode = ContextTouchMode::kOnFoot) {
  ShutdownContextTouchControls();
  OnControlsDisabled(ppc, guest, next_epoch++);
  SetContextTouchEditorOpen(false);
  SetContextTouchPreferences({});
  controls_enabled = true;
  pointer_allowed = true;
  title_owned = false;
  facts = {};
  facts.generation = next_generation++;
  facts.player_identity = 0x10000;
  facts.input_user_known = true;
  facts.valid = facts.playing = facts.gameplay_allowed = facts.native_input_allowed = true;
  facts.base_mode = mode;
  prompts = {};
  presentation = {};
  presentation.generation = facts.generation;
  presentation.valid = presentation.focused = true;
  presentation.output_width = 1280;
  presentation.output_height = 720;
  presentation.logical_width = presentation.logical_safe_width = 1280;
  presentation.logical_height = presentation.logical_safe_height = 720;
  presentation.physical_output_width = presentation.physical_surface_width = 2560;
  presentation.physical_output_height = presentation.physical_surface_height = 1440;
  StoreU32(guest, kGameplayTimeStepAddress, 0);
  Poll();
}

ContextTouchControl Control(A action, bool visible = true) {
  for (size_t i = 0; i < g_runtime.layout.control_count; ++i) {
    const auto& c = g_runtime.layout.controls[i];
    if (c.action == action && (!visible || c.visible)) return c;
  }
  std::cerr << "Missing touch action " << static_cast<unsigned>(action) << '\n';
  std::abort();
}

rex::input::AbsolutePointerEvent Event(uint64_t id, Phase phase, float x, float y, uint64_t timestamp = 0) {
  rex::input::AbsolutePointerEvent e{};
  e.generation = presentation.generation;
  e.pointer_id = id;
  e.phase = phase;
  e.timestamp_ns = timestamp;
  // Different guest/host positions ensure frontbuffer scaling cannot alter host hits.
  e.x = e.y = -100;
  e.logical_x = x;
  e.logical_y = y;
  return e;
}

void Send(uint64_t id, Phase phase, float x, float y, uint64_t timestamp = 0) {
  const auto e = Event(id, phase, x, y, timestamp);
  assert(OnPointerEvent(e, ppc, guest, facts.epoch));
}
void Send(uint64_t id, Phase phase, const ContextTouchControl& c, uint64_t timestamp = 0) {
  Send(id, phase, c.center_x, c.center_y, timestamp);
}

TouchNativePadState Freeze() {
  std::array<uint8_t, 256> down{}, pressed{};
  CollectVirtualKeys(facts.epoch, down, pressed);
  TouchNativePadState pad;
  GetTouchNativePadState(facts.epoch, &pad);
  return pad;
}

void Arm() {
  facts.armed = facts.weapon_known = true;
  facts.aim_settings_known = true;
  facts.aim_threshold = 128;
  facts.weapon_slot = 2;
  facts.weapon_type = 7;
  Poll();
}

void PrepareControl(uint32_t control, uint32_t user) {
  StoreU32(guest, control + kControlUserIndexOffset, user);
  StoreU32(guest, control + kLastInputTimeOffset, 0);
  for (uint32_t action = 0; action <= 86; ++action) {
    const uint32_t record = control + kActionArrayOffset + action * kActionStride;
    StoreU8(guest, record, 0);
    StoreU8(guest, record + kActionCurrentOffset, 127);
  }
}
uint8_t Current(uint32_t control, Action action) {
  return LoadU8(guest, ActionAddress(control, action) + kActionCurrentOffset);
}

void NativeTapAndOwnership() {
  Reset();
  const auto jump = Control(A::kJumpClimb);
  Send(1, Phase::kDown, jump);
  Send(1, Phase::kUp, jump);
  auto pad = Freeze();
  assert(pad.buttons & rex::input::X_INPUT_GAMEPAD_X);
  assert(Freeze().buttons == pad.buttons);
  Poll();
  assert(!(Freeze().buttons & rex::input::X_INPUT_GAMEPAD_X));
  Poll();
  Send(1, Phase::kDown, jump);
  Send(1, Phase::kCancel, jump);
  assert(!(Freeze().buttons & rex::input::X_INPUT_GAMEPAD_X));
  std::cout << "PASS native sub-poll tap freezes once; cancellation discards pending press\n";

  Reset();
  const auto run = Control(A::kRunSprint);
  Send(2, Phase::kDown, run);
  Freeze();
  Arm();
  assert(Control(A::kRunSprint).visible);
  assert(Freeze().buttons & rex::input::X_INPUT_GAMEPAD_A);
  Poll();
  Send(2, Phase::kUp, run);
  assert(!(Freeze().buttons & rex::input::X_INPUT_GAMEPAD_A));
  std::cout << "PASS held semantic action survives contextual layout reordering\n";

  Reset();
  Send(3, Phase::kDown, Control(A::kRunSprint));
  auto stale = Event(99, Phase::kDown, 800, 300);
  stale.generation = 0;
  assert(!OnPointerEvent(stale, ppc, guest, facts.epoch));
  assert(Freeze().buttons & rex::input::X_INPUT_GAMEPAD_A);
  ++presentation.generation;
  Poll();
  assert(Freeze().buttons == 0 && g_runtime.pointers.empty());
  std::cout << "PASS stale events preserve current owners; presentation changes cancel them\n";
}

void MovementCameraAndReplay() {
  Reset();
  const auto move = Control(A::kMove);
  Send(10, Phase::kDown, move);
  assert(g_runtime.movement_x == 0 && g_runtime.movement_y == 0);
  Send(10, Phase::kMove, move.center_x + move.radius * 0.5f, move.center_y);
  const auto overlay = GetContextTouchOverlaySnapshot();
  assert(overlay.movement_owned && overlay.movement_origin_x == move.center_x);
  assert(Freeze().left_x == kHalfNativeAxis);
  Poll();
  Send(10, Phase::kUp, move);
  assert(Freeze().left_x == 0);
  std::cout << "PASS floating movement keeps its down origin and emits analog native axes\n";

  Reset();
  Send(11, Phase::kDown, 850, 350);
  Send(11, Phase::kMove, 1150, 350);
  Send(11, Phase::kMove, 850 + kCameraStep, 350);
  Send(11, Phase::kUp, 850 + kCameraStep, 350);
  assert(Freeze().right_x == 0 && g_runtime.look_x == kCameraUnits);
  assert(Freeze().right_x == 0 && g_runtime.look_x == kCameraUnits);
  PrepareControl(kControlA, 0);
  PrepareControl(kControlB, 0);
  PrepareControl(kControlOther, 1);
  StoreU32(guest, kGameInputTimeAddress, 1234);
  OnControlReplay(ppc, guest, kControlA, 0, facts.epoch);
  OnControlReplay(ppc, guest, kControlB, 0, facts.epoch);
  OnControlReplay(ppc, guest, kControlOther, 0, facts.epoch);
  assert(Current(kControlA, Action::kLookRight) != 127);
  assert(Current(kControlA, Action::kLookRight) == Current(kControlB, Action::kLookRight));
  assert(Current(kControlOther, Action::kLookRight) == 127);
  assert(LoadU32(guest, kControlA + kLastInputTimeOffset) == 1234);
  Poll(); Freeze();
  assert(g_runtime.look_x == 0);
  std::cout << "PASS camera sums before quantization; Up survives once; replay covers same-user objects\n";

  Reset();
  Send(12, Phase::kDown, 850, 350);
  Send(12, Phase::kMove, 850 + kCameraStep, 350);
  Send(12, Phase::kCancel, 850 + kCameraStep, 350);
  Freeze();
  assert(g_runtime.look_x == 0);
  Poll(); Freeze();
  assert(g_runtime.look_x == 0);
  std::cout << "PASS camera Cancel discards pending relative delta without later replay\n";

  Reset();
  StoreU32(guest, kGameplayTimeStepAddress, std::bit_cast<uint32_t>(1.0f / 30.0f));
  Poll();
  Send(13, Phase::kDown, 850, 350);
  Send(13, Phase::kUp, 850 + kCameraStep, 350);
  Freeze();
  assert(g_runtime.look_x == kCameraHalfRateUnits);
  std::cout << "PASS camera gain uses native timestep at one frozen poll boundary\n";

  Reset(); Arm();
  Send(14, Phase::kDown, Control(A::kAim));
  Send(15, Phase::kDown, 850, 350);
  Send(15, Phase::kUp, 850 + kCameraStep, 350);
  Freeze();
  assert(g_runtime.look_x == kAimCameraUnits);
  facts.aim_settings_known = true;
  facts.aim_threshold = 128;
  Poll();
  assert(Freeze().left_trigger == 255);
  Poll();
  Send(14, Phase::kUp, Control(A::kAim));
  Send(16, Phase::kDown, Control(A::kFreeAim));
  assert(Freeze().left_trigger == 127);
  facts.alternate_aim_setting = true;
  Poll();
  assert(Freeze().left_trigger == 255);
  std::cout << "PASS Aim uses separate sensitivity and verified analog trigger ranges\n";
}

void VehiclePhoneAndComposite() {
  Reset(ContextTouchMode::kVehicleAutomobile);
  const auto throttle = Control(A::kAccelerate);
  const auto fire = Control(A::kVehicleFire);
  const auto steer = Control(A::kMove);
  Send(20, Phase::kDown, throttle);
  Send(21, Phase::kDown, steer);
  Send(21, Phase::kMove, steer.center_x + steer.radius * 0.5f, steer.center_y);
  Freeze();
  facts.phone_visible = facts.phone_visibility_known = true;
  Poll();
  auto pad = Freeze();
  assert(g_runtime.layout.mode == ContextTouchMode::kVehicleAutomobile);
  assert(pad.right_trigger == 255 && pad.left_x == kHalfNativeAxis);
  assert(Control(A::kPhoneAccept).visible);
  Poll(); Send(20, Phase::kMove, fire); pad = Freeze();
  assert(pad.right_trigger == 255 && (pad.buttons & rex::input::X_INPUT_GAMEPAD_LEFT_SHOULDER));
  Poll(); Send(20, Phase::kMove, throttle); pad = Freeze();
  assert(pad.right_trigger == 255 && !(pad.buttons & rex::input::X_INPUT_GAMEPAD_LEFT_SHOULDER));
  Poll(); Send(20, Phase::kCancel, throttle);
  assert(Freeze().right_trigger == 0);
  std::cout << "PASS phone preserves steering; Throttle-to-Fire enters and leaves only Fire\n";

  Reset(ContextTouchMode::kVehicleAutomobile);
  Send(22, Phase::kDown, 850, 350);
  Send(22, Phase::kMove, Control(A::kVehicleFire));
  assert(!(Freeze().buttons & rex::input::X_INPUT_GAMEPAD_LEFT_SHOULDER));
  std::cout << "PASS camera-origin drag never acquires vehicle Fire\n";

  Reset(); Arm();
  const auto onfoot_fire = Control(A::kFire);
  Send(23, Phase::kDown, onfoot_fire);
  Send(23, Phase::kMove, onfoot_fire.center_x + kCameraStep, onfoot_fire.center_y);
  assert(Freeze().right_trigger == 255 && g_runtime.look_x != 0);
  std::cout << "PASS Fire-origin drag retains Fire while contributing relative aim\n";
}

void ScopePinchAndNativeHook() {
  Reset(); Arm(); Freeze();
  assert(MergeTouchScopedZoom(facts.epoch, 0, 24, 0) == 0);
  Poll();
  assert(g_runtime.scoped_zoom && Control(A::kZoomIn).visible);
  Send(30, Phase::kDown, 720, 350);
  Send(31, Phase::kDown, 850, 350);
  Send(31, Phase::kMove, 850 + kPinchSpread, 350);
  Send(31, Phase::kUp, 850 + kPinchSpread, 350);
  Freeze();
  assert(g_runtime.zoom == kPinchZoom && g_runtime.look_x == 0);
  assert(MergeTouchScopedZoom(facts.epoch, 0, 24, 0) == kPinchZoom);
  assert(MergeTouchScopedZoom(facts.epoch, 1, 24, 7) == 7);
  assert(MergeTouchScopedZoom(facts.epoch, 0, 25, 7) == 7);
  assert(MergeTouchScopedZoom(facts.epoch, 0, 24, -255) == -255);
  PrepareControl(kControlA, 0);
  ppc.lr = 0x825ED65C; ppc.r28.u32 = kControlA;
  decoder_result = 7; decoder_calls = 0;
  sub_822B7958(ppc, guest);
  assert(decoder_calls == 1 && static_cast<int32_t>(ppc.r3.u32) == kPinchZoom);
  ppc.lr = 0x822B7A00;
  sub_822B7958(ppc, guest);
  assert(decoder_calls == 2 && ppc.r3.u32 == 7);
  Poll();
  Send(30, Phase::kMove, 740, 350); Send(30, Phase::kUp, 740, 350); Freeze();
  assert(g_runtime.zoom == 0 && g_runtime.look_x == 0);
  std::cout << "PASS pinch Up preserves zoom once; tail suppresses camera; original decoder always runs\n";

  MergeTouchScopedZoom(facts.epoch, 0, 24, 0); Poll();
  Send(32, Phase::kDown, 720, 350); Send(33, Phase::kDown, 850, 350);
  Send(33, Phase::kMove, 850 + kPinchSpread, 350);
  Send(33, Phase::kCancel, 850 + kPinchSpread, 350); Freeze();
  assert(g_runtime.zoom == 0 && g_runtime.look_x == 0);
  Poll(); Send(32, Phase::kUp, 720, 350);
  const auto zoom = Control(A::kZoomIn);
  Send(34, Phase::kDown, zoom); Send(34, Phase::kUp, zoom); Freeze();
  assert(g_runtime.zoom == -255);
  std::cout << "PASS pinch Cancel discards zoom; scoped zoom button preserves complete tap\n";

  Reset(); Send(35, Phase::kDown, 720, 350);
  const auto second = Event(36, Phase::kDown, 850, 350);
  assert(!OnPointerEvent(second, ppc, guest, facts.epoch) && !g_runtime.pinch_active);
  std::cout << "PASS ordinary chase camera never starts scoped pinch\n";
}

void ScriptIdentityAndSticks() {
  Reset(ContextTouchMode::kMinigame);
  facts.minigame_active = true; facts.gameplay_allowed = false; Poll();
  prompts.generation = facts.generation; prompts.visible = true;
  prompts.controls[0] = {TouchScriptQueryKind::kControlHeld, 17}; prompts.control_count = 1;
  ObserveTouchScriptQuery(TouchScriptQueryKind::kControlPressed, 17, facts.epoch, 2, 0x500, facts.generation);
  ObserveTouchScriptQuery(TouchScriptQueryKind::kControlHeld, 17, facts.epoch, 3, 0x600, facts.generation);
  ObserveTouchScriptQuery(TouchScriptQueryKind::kControlHeld, 42, facts.epoch, 2, 0x500, facts.generation);
  Freeze(); Poll();
  ContextTouchControl chosen;
  size_t scripts = 0;
  for (size_t i = 0; i < g_runtime.layout.control_count; ++i) {
    const auto& c = g_runtime.layout.controls[i];
    if (c.kind == ContextTouchControlKind::kScriptButton) {
      ++scripts; assert(c.script.action == 17);
      if (c.script.input_group == 2) chosen = c;
    }
  }
  assert(scripts == 2); Send(40, Phase::kDown, chosen); Freeze();
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld, 17, facts.epoch, 2, 0x500, facts.generation) == 1);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlPressed, 17, facts.epoch, 2, 0x500, facts.generation) == 1);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlAnalog, 17, facts.epoch, 2, 0x500, facts.generation) == 255);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld, 17, facts.epoch, 3, 0x600, facts.generation) == 0);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kRawButton, 17, facts.epoch, 2, 0x500, facts.generation) == 0);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld, 17, facts.epoch, 2, 0x500, 0) == 0);
  for (size_t gap = 0; gap < 8; ++gap) { Poll(); Freeze(); }
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld, 17, facts.epoch, 2, 0x500, facts.generation) == 1);
  Poll(); Send(40, Phase::kCancel, chosen); Freeze();
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld, 17, facts.epoch, 2, 0x500, facts.generation) == 0);
  std::cout << "PASS prompted scripts isolate group/thread/generation/namespace and retain owners through query gaps\n";

  Reset(ContextTouchMode::kMinigame);
  facts.minigame_active = true; facts.gameplay_allowed = false;
  auto preferences = GetContextTouchPreferences(); preferences.floating_stick = false;
  SetContextTouchPreferences(preferences); Poll();
  prompts.generation = facts.generation; prompts.visible = true;
  prompts.controls[0] = {TouchScriptQueryKind::kAnalogueSticks, 0}; prompts.control_count = 1;
  ObserveTouchScriptQuery(TouchScriptQueryKind::kAnalogueSticks, 0, facts.epoch, 2, 0x500, facts.generation);
  Freeze(); Poll();
  const auto left = Control(A::kMove), right = Control(A::kActivityRightStick);
  Send(41, Phase::kDown, left.center_x + left.radius * 0.5f, left.center_y);
  Send(42, Phase::kDown, right);
  Send(42, Phase::kMove, right.center_x, right.center_y + right.radius); Freeze();
  std::array<int32_t, 4> axes{};
  assert(GetTouchScriptAnalogueSticks(facts.epoch, &axes, 2, 0x500, facts.generation));
  assert((axes == std::array<int32_t, 4>{kHalfAxis, 0, 0, 255}));
  assert(!GetTouchScriptAnalogueSticks(facts.epoch, &axes, 2, 0x600, facts.generation));
  StoreU32(guest, kScriptContext + 8, kScriptArguments); StoreU32(guest, kScriptArguments, 2);
  for (size_t i = 0; i < 4; ++i) {
    StoreU32(guest, kScriptArguments + 4 + static_cast<uint32_t>(i * sizeof(uint32_t)), kStickOutputs[i]);
    StoreU32(guest, kStickOutputs[i], 0);
  }
  StoreU32(guest, kStickOutputs[2], 0x80000000);
  assert(MergeTouchScriptAnalogueStickResults(guest, kScriptContext, facts.epoch, 0x500, facts.generation));
  assert(LoadU32(guest, kStickOutputs[0]) == kHalfAxis && LoadU32(guest, kStickOutputs[1]) == 0);
  assert(LoadU32(guest, kStickOutputs[2]) == 0x80000000 && LoadU32(guest, kStickOutputs[3]) == 255);
  std::cout << "PASS four script stick outputs keep identity and preserve stronger original values\n";
}

void EditorWheelAndProvider() {
  Reset(); InitializeContextTouchControls(); assert(provider != nullptr); Freeze();
  rex::input::X_INPUT_GAMEPAD native{};
  assert(provider(0, &native) && !provider(1, &native));
  title_owned = true;
  assert(provider(0, &native) && static_cast<uint16_t>(native.buttons) == 0);
  title_owned = false;
  RequestContextTouchEditor(); Poll(); assert(ContextTouchEditorOpen());
  const auto editable = Control(A::kFire);
  Send(51, Phase::kDown, editable);
  Send(51, Phase::kMove, editable.center_x - editable.radius * 0.25f, editable.center_y);
  Send(51, Phase::kUp, editable.center_x - editable.radius * 0.25f, editable.center_y);
  assert(Control(A::kFire).center_x != editable.center_x);
  const auto placed = Control(A::kFire);
  Send(56, Phase::kDown, placed.center_x - placed.radius * 0.25f, placed.center_y);
  Send(57, Phase::kDown, placed.center_x + placed.radius * 0.25f, placed.center_y);
  assert(g_runtime.pinch_active);
  Send(57, Phase::kMove, placed.center_x + placed.radius * 0.125f, placed.center_y);
  const auto resized = Control(A::kFire);
  assert(resized.radius < placed.radius);
  Send(57, Phase::kCancel, placed.center_x + placed.radius * 0.125f, placed.center_y);
  assert(Control(A::kFire).radius > resized.radius);
  Send(56, Phase::kUp, placed.center_x - placed.radius * 0.25f, placed.center_y);
  assert(!Freeze().active);
  Poll(); Send(52, Phase::kDown, Control(A::kEditDone)); assert(!ContextTouchEditorOpen()); Freeze();
  std::cout << "PASS native provider is user-scoped; editor drag/resize/cancel suppress gameplay output\n";

  Reset(); Arm();
  const auto weapon = Control(A::kWeaponWheel);
  Send(53, Phase::kDown, weapon); Send(53, Phase::kUp, weapon);
  assert(Freeze().buttons & rex::input::X_INPUT_GAMEPAD_DPAD_RIGHT);
  Poll(); assert(!(Freeze().buttons & rex::input::X_INPUT_GAMEPAD_DPAD_RIGHT));
  facts.inventory_known = true;
  facts.weapons[2] = {.slot = 2, .type = 7, .ammo = 30, .owned = true, .selectable = true};
  facts.weapons[3] = {.slot = 3, .type = 8, .ammo = 30, .owned = true, .selectable = true};
  Poll(); Send(54, Phase::kDown, weapon); Send(54, Phase::kMove, weapon, kHoldTimestamp);
  assert(g_runtime.weapon_wheel_open);
  ContextTouchControl choice;
  for (size_t i = 0; i < g_runtime.layout.control_count; ++i) {
    const auto& c = g_runtime.layout.controls[i];
    if (c.action == A::kWeaponSelect && c.weapon_slot == 3) choice = c;
  }
  assert(choice.action == A::kWeaponSelect);
  Send(54, Phase::kUp, choice, kHoldTimestamp);
  assert(!g_runtime.weapon_wheel_open);
  assert(Freeze().buttons & rex::input::X_INPUT_GAMEPAD_DPAD_RIGHT);
  Poll(); assert(Freeze().buttons == 0);
  facts.weapon_slot = 3; Poll();
  assert(Freeze().buttons == 0 && g_runtime.weapon_target == kNoWeaponSelection);
  std::cout << "PASS weapon tap cycles once; inventory selection stops on observed native slot\n";
  ShutdownContextTouchControls(); assert(provider == nullptr);
}

void RestrictedNativeAndMissionPrompts() {
  Reset();
  facts.gameplay_allowed = false;
  facts.minigame_active = true;
  auto preferences = GetContextTouchPreferences(); preferences.floating_stick = false;
  SetContextTouchPreferences(preferences); Poll();
  assert(g_runtime.layout.mode == ContextTouchMode::kMinigame);
  prompts.generation = facts.generation; prompts.visible = true;
  prompts.controls[0] = {TouchScriptQueryKind::kControlHeld, 17}; prompts.control_count = 1;
  ObserveTouchScriptQuery(TouchScriptQueryKind::kControlHeld, 17, facts.epoch, 2, 0x500, facts.generation);
  Poll();
  Send(62, Phase::kDown, Control(A::kScript)); Freeze();
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld, 17, facts.epoch, 2, 0x500, facts.generation) == 1);
  for (size_t i = 0; i < g_runtime.layout.control_count; ++i)
    assert(g_runtime.layout.controls[i].action != A::kMore && g_runtime.layout.controls[i].action != A::kNativeA);
  std::cout << "PASS known minigame directly exposes its requested action without a generic controller page\n";

  Reset();
  ObserveTouchScriptQuery(TouchScriptQueryKind::kAnalogueSticks, 0, facts.epoch, 2, 0x500, facts.generation);
  Freeze(); Poll();
  for (size_t i = 0; i < g_runtime.layout.control_count; ++i)
    assert(g_runtime.layout.controls[i].action != A::kActivityRightStick || !g_runtime.layout.controls[i].visible);
  prompts.generation = facts.generation; prompts.visible = true;
  prompts.controls[0] = {TouchScriptQueryKind::kControlHeld, 17}; prompts.control_count = 1;
  ObserveTouchScriptQuery(TouchScriptQueryKind::kControlHeld, 17, facts.epoch, 2, 0x500, facts.generation);
  facts.gameplay_allowed = false;
  Freeze(); Poll();
  Send(63, Phase::kDown, Control(A::kScript)); Freeze();
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld, 17, facts.epoch, 2, 0x500, facts.generation) == 1);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld, 17, facts.epoch, 2, 0, facts.generation) == 0);
  facts.loading = true;
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld, 17, facts.epoch, 2, 0x500, facts.generation) == 0);
  Poll(); Freeze();
  assert(g_runtime.pointers.empty() && !GetContextTouchOverlaySnapshot().visible);
  facts.loading = false;
  prompts.visible = false;
  Poll(); Freeze();
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld, 17, facts.epoch, 2, 0x500, facts.generation) == 0);
  std::cout << "PASS visible mission prompts remain usable with disabled player control and loading cancels their held script input\n";

  Reset();
  prompts.generation = facts.generation; prompts.visible = true;
  prompts.controls[0] = {TouchScriptQueryKind::kControlHeld, 17}; prompts.control_count = 1;
  ObserveTouchScriptQuery(TouchScriptQueryKind::kControlHeld, 17, facts.epoch, 2, 0x500, facts.generation);
  Poll();
  Send(65, Phase::kDown, Control(A::kScript)); Freeze();
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld, 17, facts.epoch, 2, 0x500, facts.generation) == 1);
  prompts.visible = false;
  facts.gameplay_allowed = false;
  assert(!GetContextTouchOverlaySnapshot().visible);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld, 17, facts.epoch, 2, 0x500, facts.generation) == 0);
  Poll(); Freeze();
  assert(g_runtime.pointers.empty() && g_runtime.layout.control_count == 0);
  std::cout << "PASS a held script owner cannot replace fresh visible activity evidence after player control is lost\n";
}

void ParachuteAliases() {
  Reset();
  facts.gameplay_allowed = false; Poll();
  ObserveTouchParachuteState(3, facts.epoch, 0x700); Freeze(); Poll();
  assert(g_runtime.layout.mode == ContextTouchMode::kParachuteFreefall);
  const auto deploy = Control(A::kDeploy);
  assert(deploy.kind == ContextTouchControlKind::kButton);
  Send(70, Phase::kDown, deploy); Send(70, Phase::kUp, deploy); Freeze();
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlPressed, 1, facts.epoch, 0, 0x700, facts.generation) == 1);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlPressed, 137, facts.epoch, 0, 0x700, facts.generation) == 1);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlPressed, 1, facts.epoch, 0, 0x701, facts.generation) == 0);
  ObserveTouchParachuteState(5, facts.epoch, 0x700); Poll();
  const auto left = Control(A::kParachuteBrakeLeft), right = Control(A::kParachuteBrakeRight);
  Send(71, Phase::kDown, left); Send(72, Phase::kDown, right);
  const auto smoke = Control(A::kSmoke);
  Send(73, Phase::kDown, smoke); Send(73, Phase::kUp, smoke);
  Send(74, Phase::kDown, 850, 350); Send(74, Phase::kUp, 850 + kCameraStep, 350);
  const auto pad = Freeze();
  assert(pad.buttons == 0 && pad.left_trigger == 0 && pad.right_trigger == 0);
  std::array<uint8_t, 256> exported_down{}, exported_pressed{};
  exported_down[static_cast<uint16_t>(rex::ui::VirtualKey::kReturn)] = 1;
  CollectVirtualKeys(facts.epoch, exported_down, exported_pressed);
  for (const auto key : {rex::ui::VirtualKey::kLButton, rex::ui::VirtualKey::kRButton,
                         rex::ui::VirtualKey::kF, rex::ui::VirtualKey::kControl})
    assert(exported_down[static_cast<uint16_t>(key)] == 0 && exported_pressed[static_cast<uint16_t>(key)] == 0);
  assert(exported_down[static_cast<uint16_t>(rex::ui::VirtualKey::kReturn)] == 1);
  for (const uint32_t action : {4, 138, 6, 137})
    assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlAnalog, action, facts.epoch, 0, 0x700, facts.generation) == 255);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld, 51, facts.epoch, 0, 0x700, facts.generation) == 1);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kRawButton, 17, facts.epoch, 0, 0x700, facts.generation) == 1);
  ObserveTouchScriptQuery(TouchScriptQueryKind::kAnalogueSticks, 0, facts.epoch, 0, 0x700, facts.generation);
  std::array<int32_t, 4> axes;
  assert(GetTouchScriptAnalogueSticks(facts.epoch, &axes, 0, 0x700, facts.generation));
  assert(axes[2] == kCameraUnits);
  Poll(); Send(71, Phase::kCancel, left); Send(72, Phase::kCancel, right);
  const auto detach = Control(A::kDetach);
  Send(75, Phase::kDown, detach); Send(75, Phase::kUp, detach); Freeze();
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlPressed, 3, facts.epoch, 0, 0x700, facts.generation) == 1);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlAnalog, 4, facts.epoch, 0, 0x700, facts.generation) == 0);
  Poll(); Freeze();
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlPressed, 3, facts.epoch, 0, 0x700, facts.generation) == 0);
  std::cout << "PASS verified parachute deploy/brake/smoke/detach aliases and camera stick remain script-instance scoped\n";
}

void OverlayFade() {
  ShutdownContextTouchControls(); Reset(); InitializeContextTouchControls(); Arm();
  const auto fire = Control(A::kFire);
  Send(90, Phase::kDown, fire); Freeze();
  assert(!GetContextTouchDrawableOverlaySnapshot(kFadeStart).visible);
  assert(GetContextTouchDrawableOverlaySnapshot(kFadeQuarter).fade_alpha == kFadeQuarterAlpha);
  const auto shown = GetContextTouchDrawableOverlaySnapshot(kFadeFull);
  assert(shown.visible && shown.fade_alpha == 1.0f);
  controls_enabled = false;
  assert(!GetContextTouchOverlaySnapshot().visible);
  TouchNativePadState pad;
  assert(!GetTouchNativePadState(facts.epoch, &pad));
  assert(GetContextTouchDrawableOverlaySnapshot(kFadeFull).fade_alpha == 1.0f);
  OnControlsDisabled(ppc, guest, next_epoch++);
  ++presentation.generation;
  const auto fading = GetContextTouchDrawableOverlaySnapshot(kFadeHalfOut);
  assert(fading.visible && fading.fade_alpha == kFadeHalfOutAlpha);
  assert(fading.layout.control_count == shown.layout.control_count);
  assert(g_runtime.pointers.empty());
  controls_enabled = true; Poll(); Freeze();
  assert(GetContextTouchDrawableOverlaySnapshot(kFadeHalfOut).fade_alpha == kFadeHalfOutAlpha);
  assert(GetContextTouchDrawableOverlaySnapshot(kFadeReverseQuarter).fade_alpha == kFadeReversedAlpha);
  assert(GetContextTouchDrawableOverlaySnapshot(kFadeReverseFull).fade_alpha == 1.0f);
  facts.valid = false; Poll();
  assert(!GetContextTouchOverlaySnapshot().visible);
  assert(!GetContextTouchDrawableOverlaySnapshot(kFadeReverseFull).visible);
  assert(!GetContextTouchDrawableOverlaySnapshot(kFadeHidden).visible);
  std::cout << "PASS one-second visual fade retains disabled layout and reverses continuously while input cancels immediately\n";

  facts.valid = true; Poll();
  uint64_t now = kFadeHidden;
  assert(!GetContextTouchDrawableOverlaySnapshot(now).visible);
  now += kFadeSecond;
  assert(GetContextTouchDrawableOverlaySnapshot(now).fade_alpha == 1.0f);
  for (int gate = 0; gate < 3; ++gate) {
    pointer_allowed = gate != 0;
    presentation.focused = gate != 1;
    title_owned = gate == 2;
    assert(!GetContextTouchDrawableOverlaySnapshot(now).visible);
    pointer_allowed = presentation.focused = true;
    title_owned = false;
    assert(!GetContextTouchDrawableOverlaySnapshot(now).visible);
    now += kFadeSecond;
    assert(GetContextTouchDrawableOverlaySnapshot(now).fade_alpha == 1.0f);
  }
  ShutdownContextTouchControls();
  std::cout << "PASS focus, host capture and title modal discard fading visuals and restart cleanly\n";
}

void NativeScreensSuppressOverlay() {
  ShutdownContextTouchControls(); Reset(); InitializeContextTouchControls(); Arm();
  const auto fire = Control(A::kFire);
  Send(91, Phase::kDown, fire); Freeze();
  assert(!GetContextTouchDrawableOverlaySnapshot(kFadeStart).visible);
  assert(GetContextTouchDrawableOverlaySnapshot(kFadeQuarter).fade_alpha == kFadeQuarterAlpha);
  facts.frontend = true;
  // The newly captured native UI state hides a retained drawable even before
  // the extension is polled or disabled in this epoch.
  assert(!GetContextTouchOverlaySnapshot().visible);
  assert(!GetContextTouchDrawableOverlaySnapshot(kFadeQuarter).visible);
  assert(!g_runtime.last_drawable.visible);
  Poll(); Freeze();
  assert(g_runtime.pointers.empty() && g_runtime.layout.control_count == 0);
  assert(!g_runtime.native_pad.active && g_runtime.native_pad.right_trigger == 0);
  facts.frontend = false;
  Poll();
  assert(!GetContextTouchDrawableOverlaySnapshot(kFadeFull).visible);
  assert(GetContextTouchDrawableOverlaySnapshot(kFadeReverseFull).fade_alpha == 1.0f);
  controls_enabled = false;
  assert(GetContextTouchDrawableOverlaySnapshot(kFadeReverseFull).visible);
  facts.map = true;
  assert(!GetContextTouchDrawableOverlaySnapshot(kFadeReverseFull).visible);
  Poll();
  assert(g_runtime.layout.control_count == 0);
  std::cout << "PASS pause entry during fade-in and map entry during fade-out immediately discard every overlay control\n";

  ShutdownContextTouchControls();
}

void NoninteractiveTransitions() {
  enum class Transition { kLoading, kLoadingDuringFade, kCutscene, kControlDisabled, kNoPlayer };
  for (const auto transition : {Transition::kLoading, Transition::kLoadingDuringFade, Transition::kCutscene,
                                Transition::kControlDisabled, Transition::kNoPlayer}) {
    Reset(); InitializeContextTouchControls(); Arm();
    const auto fire = Control(A::kFire);
    const auto movement = Control(A::kMove);
    Send(101, Phase::kDown, fire);
    Send(102, Phase::kDown, movement);
    Send(102, Phase::kMove, movement.center_x + movement.radius, movement.center_y);
    Send(103, Phase::kDown, 850, 350);
    Send(103, Phase::kMove, 850 + kCameraStep, 350);
    const auto sprint = Control(A::kRunSprint);
    Send(105, Phase::kDown, sprint);
    const auto held = Freeze();
    assert(held.active && held.right_trigger == 255 && held.left_x > 0 &&
           (held.buttons & rex::input::X_INPUT_GAMEPAD_A) && g_runtime.look_x > 0);
    assert(!GetContextTouchDrawableOverlaySnapshot(kFadeStart).visible);
    const auto shown = GetContextTouchDrawableOverlaySnapshot(kFadeFull);
    assert(shown.visible && shown.fade_alpha == 1.0f);
    assert(Control(A::kRunSprint).visible);
    uint64_t transition_time = kFadeFull;
    if (transition == Transition::kLoadingDuringFade) {
      controls_enabled = false;
      assert(GetContextTouchDrawableOverlaySnapshot(kFadeFull).visible);
      assert(GetContextTouchDrawableOverlaySnapshot(kFadeHalfOut).fade_alpha == kFadeHalfOutAlpha);
      transition_time = kFadeHalfOut;
    }

    // Preserve the native permission and generation deliberately: the newest
    // loading/cutscene facts must override any previous frozen runtime state.
    const auto generation = facts.generation;
    if (transition == Transition::kLoading || transition == Transition::kLoadingDuringFade) facts.loading = true;
    if (transition == Transition::kCutscene) facts.cutscene = true;
    if (transition == Transition::kControlDisabled) facts.gameplay_allowed = false;
    if (transition == Transition::kNoPlayer) {
      facts.valid = false;
      facts.player_identity = 0;
    }
    assert(facts.generation == generation && facts.native_input_allowed);
    assert(!GetContextTouchOverlaySnapshot().visible);
    assert(!GetContextTouchDrawableOverlaySnapshot(transition_time).visible);
    assert(!g_runtime.last_drawable.visible);
    TouchNativePadState blocked;
    assert(!GetTouchNativePadState(facts.epoch, &blocked));
    rex::input::X_INPUT_GAMEPAD native{};
    if (provider(0, &native)) {
      assert(uint16_t(native.buttons) == 0 && uint8_t(native.left_trigger) == 0 &&
             uint8_t(native.right_trigger) == 0 && int16_t(native.thumb_lx) == 0 &&
             int16_t(native.thumb_ly) == 0 && int16_t(native.thumb_rx) == 0 && int16_t(native.thumb_ry) == 0);
    }
    PrepareControl(kControlA, 0);
    OnControlReplay(ppc, guest, kControlA, 0, facts.epoch);
    assert(Current(kControlA, Action::kMoveRight) == 127 && Current(kControlA, Action::kLookRight) == 127);
    assert(LoadU32(guest, kControlA + kLastInputTimeOffset) == 0);
    Poll(); Freeze();
    assert(g_runtime.layout.control_count == 0 && g_runtime.pointers.empty());
    assert(!g_runtime.native_pad.active && g_runtime.native_pad.buttons == 0 &&
           g_runtime.native_pad.right_trigger == 0 && g_runtime.look_x == 0 && g_runtime.movement_x == 0);
    assert(!OnPointerEvent(Event(101, Phase::kUp, fire.center_x, fire.center_y), ppc, guest, facts.epoch));
    assert(!OnPointerEvent(Event(105, Phase::kUp, sprint.center_x, sprint.center_y), ppc, guest, facts.epoch));

    facts.loading = facts.cutscene = false;
    facts.valid = facts.gameplay_allowed = true;
    facts.player_identity = 0x10000;
    controls_enabled = true;
    Poll();
    const auto resumed = Freeze();
    assert(resumed.buttons == 0 && resumed.left_trigger == 0 && resumed.right_trigger == 0 &&
           resumed.left_x == 0 && resumed.left_y == 0 && resumed.right_x == 0 && resumed.right_y == 0);
    assert(g_runtime.look_x == 0 && g_runtime.pointers.empty());
    assert(!GetContextTouchDrawableOverlaySnapshot(transition_time).visible);
    assert(GetContextTouchDrawableOverlaySnapshot(kFadeReverseFull).fade_alpha == 1.0f);
    PrepareControl(kControlA, 0);
    OnControlReplay(ppc, guest, kControlA, 0, facts.epoch);
    assert(Current(kControlA, Action::kMoveRight) == 127 && Current(kControlA, Action::kLookRight) == 127);
    ShutdownContextTouchControls();
  }
  std::cout << "PASS loading, cutscene, unknown control loss and missing player hard-hide cached controls and cancel frozen input before replay\n";

  Reset(); InitializeContextTouchControls();
  facts.gameplay_allowed = false;
  assert(!facts.loading && !facts.cutscene && !facts.minigame_active && facts.native_input_allowed);
  Poll(); Freeze();
  assert(g_runtime.layout.control_count == 0);
  assert(!GetContextTouchOverlaySnapshot().visible && !GetContextTouchDrawableOverlaySnapshot(kFadeStart).visible);
  assert(!g_runtime.native_pad.active);
  ShutdownContextTouchControls();
  std::cout << "PASS disabled player control alone never creates a minigame or opens an automatic raw-pad page\n";
}

void ExplicitFrontendEditor() {
  for (const bool policy : {false, true}) {
    Reset(); InitializeContextTouchControls();
    controls_enabled = policy;
    facts.frontend = true;
    facts.valid = facts.gameplay_allowed = facts.native_input_allowed = false;
    facts.player_identity = 0;
    facts.base_mode = ContextTouchMode::kDisabled;
    Poll();
    assert(!GetContextTouchOverlaySnapshot().visible);
    RequestContextTouchEditor();
    assert(ContextTouchEditorCapturesInput());
    Poll();
    assert(IsContextTouchEditorActive() && ContextTouchEditorRequested());
    RestoreContextTouchLayout(g_runtime.layout);
    Poll();
    SuspendContextTouchGameplay();
    {
      ContextTouchGameplayTransition transition;
      Poll();
      assert(IsContextTouchEditorActive() && GetContextTouchOverlaySnapshot().editor);
    }
    const auto preview = GetContextTouchOverlaySnapshot();
    assert(preview.visible && preview.editor && preview.layout.mode == ContextTouchMode::kOnFoot);
    const auto start = MonotonicNanoseconds();
    assert(!GetContextTouchDrawableOverlaySnapshot(start).visible);
    assert(GetContextTouchDrawableOverlaySnapshot(start + kFadeQuarterDuration).fade_alpha == kFadeQuarterAlpha);
    assert(GetContextTouchDrawableOverlaySnapshot(start + kFadeSecond).fade_alpha == 1.0f);
    const auto editable = Control(A::kFire);
    Send(93, Phase::kDown, editable);
    Send(93, Phase::kMove, editable.center_x - kCameraStep, editable.center_y);
    Send(93, Phase::kUp, editable.center_x - kCameraStep, editable.center_y);
    assert(Control(A::kFire).center_x != editable.center_x);
    assert(!Freeze().active);
    facts.map = true;
    Poll();
    assert(GetContextTouchOverlaySnapshot().editor);
    const auto done = Control(A::kEditDone);
    Send(94, Phase::kDown, done);
    assert(!ContextTouchEditorRequested() && !IsContextTouchEditorActive());
    assert(!GetContextTouchOverlaySnapshot().visible && g_runtime.pointers.empty());
    const auto closed = g_runtime.editor_close_started_ns;
    const auto closing = GetContextTouchDrawableOverlaySnapshot(closed + kFadeQuarterDuration);
    assert(closing.visible && closing.editor && closing.fade_alpha == kEditorFadeOutQuarterAlpha);
    assert(ContextTouchEditorCapturesInput(closed + kFadeHalfDuration));
    assert(!OnPointerEvent(Event(95, Phase::kDown, editable.center_x, editable.center_y), ppc, guest, facts.epoch));
    assert(!Freeze().active);
    assert(!GetContextTouchDrawableOverlaySnapshot(closed + kFadeSecond).visible);
    assert(!ContextTouchEditorCapturesInput(closed + kFadeSecond));
    assert(controls_enabled == policy && !ContextTouchEditorRequested());
    assert(!GetContextTouchOverlaySnapshot().visible);
    ShutdownContextTouchControls();
  }
  std::cout << "PASS explicit frontend editor previews without a player, fades over native UI, and retains input through Done without changing mode\n";
}

void OnFootAttackAlwaysAvailable() {
  for (const bool weapon_known : {false, true}) {
    Reset();
    facts.weapon_known = weapon_known;
    facts.armed = false;
    Poll();
    const auto attack = Control(A::kFire);
    assert(std::string_view(attack.label.data()) == "ATTACK");
    Send(110, Phase::kDown, attack);
    assert(Freeze().right_trigger == 255);
    Poll();
    Send(110, Phase::kUp, attack);
    Poll();
    assert(Freeze().right_trigger == 0);
    facts.melee_known = facts.melee = true;
    Poll();
    const auto punch = Control(A::kFire);
    assert(std::string_view(punch.label.data()) == "PUNCH");
    Send(110, Phase::kDown, punch);
    const auto pad = Freeze();
    assert(pad.buttons & rex::input::X_INPUT_GAMEPAD_B);
    assert(pad.right_trigger == 0);
    Poll();
    Send(110, Phase::kUp, punch);
    Poll();
    assert(!(Freeze().buttons & rex::input::X_INPUT_GAMEPAD_B));
  }
  std::cout << "PASS unarmed and unknown-weapon on-foot attack stays available and melee uses native Punch input\n";

  Reset();
  assert(!Control(A::kContext, false).visible);
  facts.can_enter_vehicle = true;
  Poll();
  const auto enter = Control(A::kContext);
  assert(std::string_view(enter.label.data()) == "ENTER");
  Send(110, Phase::kDown, enter);
  Send(110, Phase::kUp, enter);
  assert(Freeze().buttons & rex::input::X_INPUT_GAMEPAD_Y);
  facts.can_enter_vehicle = false;
  Poll();
  assert(!Control(A::kContext, false).visible);
  assert(!(Freeze().buttons & rex::input::X_INPUT_GAMEPAD_Y));
  std::cout << "PASS native vehicle eligibility shows Enter and emits one native Y tap\n";
}

void BetweenPollSuspension() {
  Reset(); InitializeContextTouchControls(); Arm();
  Send(111, Phase::kDown, Control(A::kFire));
  Send(112, Phase::kDown, 850, 350);
  Send(112, Phase::kMove, 850 + kCameraStep, 350);
  assert(Freeze().right_trigger == 255 && g_runtime.look_x > 0);
  assert(!GetContextTouchDrawableOverlaySnapshot(kFadeStart).visible);
  assert(GetContextTouchDrawableOverlaySnapshot(kFadeFull).visible);
  const auto generation = facts.generation;
  const auto assert_suspended = [&] {
    assert(facts.generation == generation && facts.valid && facts.gameplay_allowed &&
           facts.native_input_allowed && !facts.loading && !facts.cutscene);
    assert(!GetContextTouchOverlaySnapshot().visible);
    assert(!GetContextTouchDrawableOverlaySnapshot(kFadeFull).visible);
    TouchNativePadState pad;
    assert(!GetTouchNativePadState(facts.epoch, &pad));
    rex::input::X_INPUT_GAMEPAD native{};
    assert(provider(0, &native));
    assert(uint16_t(native.buttons) == 0 && uint8_t(native.left_trigger) == 0 &&
           uint8_t(native.right_trigger) == 0 && int16_t(native.thumb_lx) == 0 &&
           int16_t(native.thumb_ly) == 0 && int16_t(native.thumb_rx) == 0 && int16_t(native.thumb_ry) == 0);
    PrepareControl(kControlA, 0);
    OnControlReplay(ppc, guest, kControlA, 0, facts.epoch);
    assert(Current(kControlA, Action::kLookRight) == 127);
    assert(g_runtime.pointers.empty() && g_runtime.look_x == 0);
  };
  SuspendContextTouchGameplay();
  assert_suspended();
  BeginPoll(ppc, guest, facts.epoch, false, false);
  assert_suspended();
  // Capture began before the presentation transition but completed later.
  // Advancing the poll epoch must not admit facts carrying the old revision.
  facts.epoch = next_epoch++;
  BeginPoll(ppc, guest, facts.epoch, false, false);
  assert_suspended();
  Poll();
  assert(GetContextTouchOverlaySnapshot().visible && Freeze().right_trigger == 0);
  assert(!GetContextTouchDrawableOverlaySnapshot(kFadeFull).visible);
  assert(GetContextTouchDrawableOverlaySnapshot(kFadeReverseFull).visible);
  {
    ContextTouchGameplayTransition outer;
    assert_suspended();
    Poll(); Freeze();
    assert_suspended();
    {
      ContextTouchGameplayTransition inner;
      Poll(); Freeze();
      assert_suspended();
    }
    Poll(); Freeze();
    assert_suspended();
  }
  assert_suspended();
  BeginPoll(ppc, guest, facts.epoch, false, false);
  assert_suspended();
  facts.epoch = next_epoch++;
  BeginPoll(ppc, guest, facts.epoch, false, false);
  assert_suspended();
  Poll();
  assert(GetContextTouchOverlaySnapshot().visible && Freeze().right_trigger == 0);
  assert(g_runtime.pointers.empty() && g_runtime.look_x == 0);
  ShutdownContextTouchControls();
  std::cout << "PASS presentation suspension hides stale facts between polls and nested transition scopes cannot resume through reentrant polls\n";
}
}

int main() {
  guest = static_cast<uint8_t*>(mmap(nullptr, touch_samples::kGuestBytes, PROT_READ | PROT_WRITE,
      MAP_PRIVATE | MAP_ANON, -1, 0));
  assert(guest != MAP_FAILED);
  NativeTapAndOwnership(); MovementCameraAndReplay(); VehiclePhoneAndComposite();
  ScopePinchAndNativeHook(); ScriptIdentityAndSticks(); EditorWheelAndProvider();
  RestrictedNativeAndMissionPrompts(); ParachuteAliases(); OverlayFade(); NativeScreensSuppressOverlay();
  NoninteractiveTransitions();
  OnFootAttackAlwaysAvailable();
  BetweenPollSuspension();
  ExplicitFrontendEditor();
  munmap(guest, touch_samples::kGuestBytes);
}
