#include <cassert>
#include <deque>
#include <iostream>
#include <string_view>
#include <sys/mman.h>

#include "gta4_init.h"
#include "gta4_aspect_hooks.h"
#include "input/context_touch_radar.h"
#include "input/context_touch_controls.cpp"
#include "touch_samples.h"

namespace review {
std::deque<rex::input::AbsolutePointerEvent> events;
rex::input::TouchPresentationState presentation;
gta4::input::TouchContextSnapshot facts;
gta4::input::TouchVisiblePromptSnapshot prompts;
gta4::input::TouchRadarPass radar_pass;
rex::input::TouchGamepadProvider provider = nullptr;
bool enabled = true, frontend = false;
bool pointer_enabled = true;
bool suspend_during_capture = false;
bool suspend_during_collect = false;
bool draw_radar_quad = false;
bool publish_test_key = false;
uint64_t captures = 0;
uint32_t row_selections = 0, selected_row = 0;
}
namespace rex::input {
bool TryDequeueAbsolutePointerEvent(AbsolutePointerEvent* event) noexcept {
  if (review::events.empty()) return false;
  *event = review::events.front();
  review::events.pop_front();
  return true;
}
bool TouchControlsActive() noexcept { return review::enabled; }
bool TouchControlsVisible() noexcept { return review::enabled; }
bool TouchPointerInputActive() noexcept {
  return review::pointer_enabled && review::presentation.valid && review::presentation.focused;
}
bool GetTouchPresentationState(TouchPresentationState* state) noexcept {
  *state = review::presentation;
  return state->valid;
}
void SetTouchGamepadProvider(TouchGamepadProvider provider) noexcept { review::provider = provider; }
bool ReadTouchGamepad(uint32_t user, X_INPUT_GAMEPAD* state) noexcept {
  return review::provider && review::provider(user, state);
}
}
namespace gta4::input {
void CaptureTouchContext(PPCContext&, uint8_t*, uint64_t epoch, bool frontend, bool map) noexcept {
  review::facts.presentation_revision = ContextTouchPresentationRevision();
  if (review::suspend_during_capture) {
    review::suspend_during_capture = false;
    SuspendContextTouchGameplay();
  }
  ++review::captures;
  review::facts.epoch = epoch;
  review::facts.frontend = frontend;
  review::facts.map = map;
}
TouchContextSnapshot GetTouchContextSnapshot() noexcept { return review::facts; }
TouchVisiblePromptSnapshot GetTouchVisiblePromptSnapshot(uint64_t, uint64_t generation) noexcept {
  return review::prompts.generation == generation ? review::prompts : TouchVisiblePromptSnapshot{};
}
TouchRadarPass ConsumeTouchRadarPass(uint8_t*, uint32_t) noexcept { return review::radar_pass; }
}
namespace gta4::aspect {
UiContext CurrentUi(uint8_t*) { return {.render = {1280, 720}, .active = true}; }
UiContext MenuBodyUi(uint8_t* base) { return CurrentUi(base); }
UiContext RadarUi(uint8_t* base) { return CurrentUi(base); }
UiContext RadarLocalUi() { return CurrentUi(nullptr); }
Scope::Scope(UiRole, Point) {}
Scope::Scope(UiContext) {}
Scope::~Scope() = default;
FrontendLayoutScope::FrontendLayoutScope(PPCContext&, uint8_t*) {}
FrontendLayoutScope::~FrontendLayoutScope() = default;
}
void __imp__sub_8224EEF8(PPCContext& context, uint8_t*) { context.r3.u32 = review::frontend; }
void __imp__sub_8224EE98(PPCContext& context, uint8_t*) {
  ++review::row_selections;
  review::selected_row = context.r4.u32;
}
void __imp__sub_821BF050(PPCContext&, uint8_t*) {}
void __imp__sub_8233ABF0(PPCContext&, uint8_t*) {}
extern "C" void sub_821BF050(PPCContext&, uint8_t*);
extern "C" void sub_8239C468(PPCContext&, uint8_t*);
void __imp__sub_8239C468(PPCContext& context, uint8_t* base) {
  if (!review::draw_radar_quad) return;
  PPCContext nested = context;
  nested.lr = 0x8233AB4C;
  nested.r4.u32 = 0x80000;
  sub_821BF050(nested, base);
}

namespace {
void Word(uint8_t* base, uint32_t address, uint32_t value) {
  *reinterpret_cast<uint32_t*>(base + address) = __builtin_bswap32(value);
}
void NeutralAxes(uint8_t* base, uint32_t control, uint32_t user) {
  Word(base, control + 3412, user);
  for (uint32_t action = 12; action < 20; ++action) {
    base[control + 2328 + action * 12] = 0;
    base[control + 2328 + action * 12 + 2] = 127;
  }
}
void DrawFrontendRow(PPCContext& context, uint8_t* base) {
  context.lr = 0x8229E7C0;
  context.r23.u32 = 0;
  context.r28.u32 = 0;
  context.f1.f64 = 0.2;
  context.f2.f64 = 0.3;
  GTA4_TouchObserveHudSubmit(context, base);
}
void CollectTestVirtualKeys(uint64_t epoch, std::array<uint8_t, 256>& down,
                            std::array<uint8_t, 256>& pressed) {
  gta4::input::CollectVirtualKeys(epoch, down, pressed);
  // Exercise the coordinator's extension contract even though ordinary
  // semantic controls now publish their native pad state directly.
  if (review::publish_test_key) {
    const auto key = static_cast<size_t>(rex::ui::VirtualKey::kSpace);
    down[key] = pressed[key] = 1;
  }
  if (review::suspend_during_collect) {
    review::suspend_during_collect = false;
    gta4::input::SuspendContextTouchGameplay();
  }
}

void CheckSemanticLayout(const gta4::input::ContextTouchLayout& layout) {
  using gta4::input::TouchAction;
  bool attack = false;
  for (size_t index = 0; index < layout.control_count; ++index) {
    const auto& control = layout.controls[index];
    assert(control.action != TouchAction::kMore && control.action != TouchAction::kPause &&
           control.action != TouchAction::kSettings);
    assert(control.action < TouchAction::kNativeA || control.action > TouchAction::kNativeRightStick);
    attack |= control.action == TouchAction::kFire && control.visible;
  }
  if (layout.mode == gta4::input::ContextTouchMode::kOnFoot && layout.control_count) assert(attack);
}

// Vertex addresses, IEEE float bits, viewport coordinates and action addresses
// were calculated with Python from the guest stride and normalized viewport.
constexpr std::array<std::array<uint32_t, 2>, 8> kRadarVertices = {{
    {0x80000, 0x00000000}, {0x80004, 0x00000000},
    {0x80010, 0x3F800000}, {0x80014, 0x00000000},
    {0x80020, 0x3F800000}, {0x80024, 0x3F800000},
    {0x80030, 0x00000000}, {0x80034, 0x3F800000},
}};
constexpr gta4::input::TouchRadarPass kGameplayRadar = {
    .bounds = {0.0625, 0.75, 0.25, 0.9375}, .gameplay = true};
constexpr gta4::input::ContextTouchControl kRadarCenter = {
    .center_x = 200.0f, .center_y = 607.5f};
constexpr gta4::input::ContextTouchControl kOutsideRadar = {
    .center_x = 640.0f, .center_y = 360.0f};
constexpr std::array<uint32_t, 3> kPauseCurrent = {0x10CAA, 0x20CAA, 0x30CAA};
// Python: normalized {13/16, 1/16, 15/16, 3/16} at 1280 by 720.
constexpr gta4::input::ContextTouchHudBounds kWeaponHudBounds = {
    0.8125f, 0.0625f, 0.9375f, 0.1875f};
constexpr gta4::input::ContextTouchControl kWeaponHudCenter = {
    .center_x = 1120.0f, .center_y = 90.0f};
constexpr gta4::input::ContextTouchControl kWeaponCameraOrigin = {
    .center_x = 1000.0f, .center_y = 300.0f};
// Python: output width times 0.03 exceeds normalized tap slop while remaining
// inside this HUD. Width times 0.15 releases beyond its right edge.
constexpr float kWeaponHudDrag = 38.4f;
constexpr float kWeaponHudOutside = 192.0f;
}

int main() {
  using namespace gta4::input;
  using namespace rex::input;
  constexpr size_t memory_size = 4294967296ULL;
  auto* base = static_cast<uint8_t*>(mmap(nullptr, memory_size, PROT_READ | PROT_WRITE,
                                         MAP_PRIVATE | MAP_ANON, -1, 0));
  assert(base != MAP_FAILED);
  auto& presentation = review::presentation;
  presentation.generation = 1;
  presentation.valid = presentation.focused = true;
  presentation.output_width = presentation.physical_output_width = presentation.physical_surface_width = 1280;
  presentation.output_height = presentation.physical_output_height = presentation.physical_surface_height = 720;
  presentation.safe_area_width = 1280;
  presentation.safe_area_height = 720;
  auto& facts = review::facts;
  facts.valid = facts.playing = facts.input_user_known = facts.gameplay_allowed = facts.native_input_allowed = true;
  facts.player_identity = 0x100000;
  facts.generation = 1;
  facts.base_mode = ContextTouchMode::kOnFoot;
  facts.weapon_known = true;
  SetContextTouchPreferences({.floating_stick = false});
  PPCContext context{};
  constexpr uint32_t first_control = 0x10000, second_control = 0x20000, other_user = 0x30000;
  NeutralAxes(base, first_control, 0);
  NeutralAxes(base, second_control, 0);
  NeutralAxes(base, other_user, 1);
  Word(base, 0x82C6C2A4, 123);
  InitializeContextTouchControls();
  GTA4_RegisterTouchExtension({.begin_poll = &BeginPoll, .on_pointer_event = &OnPointerEvent,
      .collect_virtual_keys = &CollectTestVirtualKeys, .on_controls_disabled = &OnControlsDisabled,
      .on_control_replay = &OnControlReplay});
  mnk::SetNativeControllerCompatibilityBindings({.x = rex::ui::VirtualKey::kSpace});
  const auto poll = [&](uint64_t epoch) {
    const auto before = review::captures;
    GTA4_TouchConsumePoll(context, base, epoch);
    assert(review::captures == before + 1 && facts.epoch == epoch && GTA4_TouchCurrentEpoch() == epoch);
    CheckSemanticLayout(GetContextTouchOverlaySnapshot().layout);
  };
  const auto find = [&](TouchAction action) {
    const auto layout = GetContextTouchOverlaySnapshot().layout;
    for (size_t index = 0; index < layout.control_count; ++index)
      if (layout.controls[index].action == action && layout.controls[index].visible) return layout.controls[index];
    std::cerr << "missing action " << static_cast<int>(action) << '\n';
    assert(false);
    return ContextTouchControl{};
  };
  const auto send = [&](const ContextTouchControl& control, AbsolutePointerPhase phase,
                         uint64_t id, float dx = 0, float dy = 0) {
    AbsolutePointerEvent event{};
    event.generation = presentation.generation;
    event.pointer_id = id;
    event.phase = phase;
    event.x = event.logical_x = control.center_x + dx;
    event.y = event.logical_y = control.center_y + dy;
    event.output_width = 1280;
    event.output_height = 720;
    review::events.push_back(event);
  };
  const auto pad = [&](uint64_t epoch) {
    TouchNativePadState result;
    assert(GetTouchNativePadState(epoch, &result));
    return result;
  };
  poll(100);
  const auto unarmed_attack = find(TouchAction::kFire);
  assert(std::string_view(unarmed_attack.label.data()) == "ATTACK" && !facts.armed && !facts.melee);
  auto jump = find(TouchAction::kJumpClimb);
  send(jump, AbsolutePointerPhase::kDown, 1);
  send(jump, AbsolutePointerPhase::kUp, 1);
  poll(101);
  assert(pad(101).buttons == X_INPUT_GAMEPAD_X);
  X_INPUT_GAMEPAD native{};
  const auto neutral_native = [&] {
    assert(ReadTouchGamepad(0, &native));
    assert(uint16_t(native.buttons) == 0 && uint8_t(native.left_trigger) == 0 &&
           uint8_t(native.right_trigger) == 0 && int16_t(native.thumb_lx) == 0 &&
           int16_t(native.thumb_ly) == 0 && int16_t(native.thumb_rx) == 0 &&
           int16_t(native.thumb_ry) == 0);
  };
  assert(ReadTouchGamepad(0, &native) && uint16_t(native.buttons) == X_INPUT_GAMEPAD_X);
  assert(!ReadTouchGamepad(1, &native));
  assert(!GTA4_TouchVirtualKeyDown(static_cast<uint16_t>(rex::ui::VirtualKey::kSpace)));
  poll(102);
  assert(pad(102).buttons == 0);
  std::cout << "PASS coordinator freezes complete tap into one native poll with isolated user\n";

  const auto publish_radar = [&](bool gameplay, bool draw_quad) {
    review::radar_pass = kGameplayRadar;
    review::radar_pass.gameplay = gameplay;
    review::draw_radar_quad = draw_quad;
    for (const auto& vertex : kRadarVertices) Word(base, vertex[0], vertex[1]);
    sub_8239C468(context, base);
  };
  const auto replay_pause = [&](uint64_t epoch, bool expected) {
    for (uint32_t address : kPauseCurrent) base[address] = 0;
    GTA4_TouchObserveControlReplay(context, base, first_control, 0, epoch);
    GTA4_TouchObserveControlReplay(context, base, second_control, 0, epoch);
    GTA4_TouchObserveControlReplay(context, base, other_user, 0, epoch);
    if (base[kPauseCurrent[0]] != (expected ? 255 : 0))
      std::cerr << "unexpected Pause replay at epoch " << epoch << '\n';
    assert(base[kPauseCurrent[0]] == (expected ? 255 : 0));
    assert(base[kPauseCurrent[1]] == base[kPauseCurrent[0]] && base[kPauseCurrent[2]] == 0);
    GTA4_TouchObserveControlReplay(context, base, first_control, 0, epoch);
    assert(base[kPauseCurrent[0]] == (expected ? 255 : 0));
  };
  publish_radar(true, true);
  send(kRadarCenter, AbsolutePointerPhase::kDown, 20);
  send(kRadarCenter, AbsolutePointerPhase::kUp, 20);
  poll(110);
  replay_pause(110, true);
  neutral_native();
  assert(__builtin_bswap32(*reinterpret_cast<uint32_t*>(base + 0x11068)) == 123);
  poll(111);
  replay_pause(111, false);
  std::cout << "PASS published radar viewport tap pauses for one poll across same-user control objects\n";

  send(kRadarCenter, AbsolutePointerPhase::kDown, 21);
  send(kRadarCenter, AbsolutePointerPhase::kMove, 21, touch_samples::kMapDragPixels);
  send(kRadarCenter, AbsolutePointerPhase::kUp, 21);
  poll(112);
  replay_pause(112, false);
  send(kRadarCenter, AbsolutePointerPhase::kDown, 22);
  send(kRadarCenter, AbsolutePointerPhase::kCancel, 22);
  send(kRadarCenter, AbsolutePointerPhase::kUp, 22);
  poll(113);
  replay_pause(113, false);
  send(kOutsideRadar, AbsolutePointerPhase::kDown, 23);
  send(kOutsideRadar, AbsolutePointerPhase::kUp, 23);
  poll(114);
  replay_pause(114, false);
  send(kRadarCenter, AbsolutePointerPhase::kDown, 24);
  send(kRadarCenter, AbsolutePointerPhase::kUp, 24, 256.0f);
  poll(115);
  replay_pause(115, false);
  publish_radar(true, false);
  send(kRadarCenter, AbsolutePointerPhase::kDown, 25);
  send(kRadarCenter, AbsolutePointerPhase::kUp, 25);
  poll(116);
  replay_pause(116, false);
  publish_radar(false, true);
  send(kRadarCenter, AbsolutePointerPhase::kDown, 26);
  send(kRadarCenter, AbsolutePointerPhase::kUp, 26);
  poll(117);
  replay_pause(117, false);
  std::cout << "PASS minimap drag, cancel, outside and unpublished radar gestures do not pause\n";

  send(unarmed_attack, AbsolutePointerPhase::kDown, 27);
  poll(118);
  assert(pad(118).right_trigger == 255);
  send(unarmed_attack, AbsolutePointerPhase::kUp, 27);
  poll(119);
  assert(pad(119).right_trigger == 0);
  std::cout << "PASS unarmed on-foot Attack remains directly available without More or a raw pad page\n";

  publish_radar(true, true);
  send(kRadarCenter, AbsolutePointerPhase::kDown, 28);
  send(kRadarCenter, AbsolutePointerPhase::kUp, 28, touch_samples::kMapDragPixels);
  poll(120);
  replay_pause(120, false);
  publish_radar(false, false);
  std::cout << "PASS coalesced minimap release beyond tap slop does not pause\n";

  auto movement = find(TouchAction::kMove);
  send(movement, AbsolutePointerPhase::kDown, 2);
  send(movement, AbsolutePointerPhase::kMove, 2, movement.radius * 0.75f);
  poll(200);
  assert(pad(200).left_x > 0);
  GTA4_TouchObserveControlReplay(context, base, first_control, 0, 200);
  GTA4_TouchObserveControlReplay(context, base, second_control, 0, 200);
  GTA4_TouchObserveControlReplay(context, base, other_user, 0, 200);
  const uint32_t right = 2328 + 13 * 12 + 2;
  const auto merged = base[first_control + right];
  assert(merged != 127 && base[second_control + right] == merged && base[other_user + right] == 127);
  GTA4_TouchObserveControlReplay(context, base, first_control, 0, 200);
  assert(base[first_control + right] == merged);
  std::cout << "PASS same frozen axis reaches multiple same-user control objects without consumption\n";

  ++facts.generation;
  facts.minigame_active = true;
  facts.gameplay_allowed = false;
  poll(201);
  assert(pad(201).left_x == 0);
  assert(GetContextTouchOverlaySnapshot().layout.mode == ContextTouchMode::kMinigame);
  review::prompts.generation = facts.generation;
  review::prompts.visible = true;
  review::prompts.controls[0] = {TouchScriptQueryKind::kControlHeld, 17};
  review::prompts.control_count = 1;
  ObserveTouchScriptQuery(TouchScriptQueryKind::kControlPressed, 17, 201, 2, 0x500,
                         facts.generation);
  poll(202);
  const auto activity = find(TouchAction::kScript);
  assert(activity.script.action == 17 && activity.script.input_group == 2 &&
         activity.script.script_thread == 0x500);
  send(activity, AbsolutePointerPhase::kDown, 3);
  send(activity, AbsolutePointerPhase::kUp, 3);
  poll(203);
  assert(pad(203).buttons == 0 && pad(203).left_x == 0);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlPressed, 17, 203,
                                  2, 0x500, facts.generation) == 1);
  poll(204);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld, 17, 204,
                                  2, 0x500, facts.generation) == 0);
  send(activity, AbsolutePointerPhase::kDown, 3);
  poll(205);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld, 17, 205,
                                  2, 0x500, facts.generation) == 1);
  send(activity, AbsolutePointerPhase::kCancel, 3);
  poll(206);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld, 17, 206,
                                  2, 0x500, facts.generation) == 0);
  review::prompts = {};
  std::cout << "PASS context changes cancel movement while a visible activity prompt preserves taps and cancellation\n";

  ++facts.generation;
  facts.minigame_active = false;
  facts.gameplay_allowed = true;
  facts.phone_visible = true;
  poll(300);
  const std::array actions = {TouchAction::kPhoneUp, TouchAction::kPhoneDown,
      TouchAction::kPhoneLeft, TouchAction::kPhoneRight, TouchAction::kPhoneAccept, TouchAction::kPhoneBack};
  const std::array<uint16_t, 6> buttons = {X_INPUT_GAMEPAD_DPAD_UP, X_INPUT_GAMEPAD_DPAD_DOWN,
      X_INPUT_GAMEPAD_DPAD_LEFT, X_INPUT_GAMEPAD_DPAD_RIGHT, X_INPUT_GAMEPAD_A, X_INPUT_GAMEPAD_B};
  uint64_t epoch = 301;
  for (size_t index = 0; index < actions.size(); ++index) {
    auto control = find(actions[index]);
    send(control, AbsolutePointerPhase::kDown, 4);
    send(control, AbsolutePointerPhase::kUp, 4);
    poll(epoch);
    assert(pad(epoch).buttons == buttons[index]);
    poll(++epoch);
    assert(pad(epoch).buttons == 0);
    ++epoch;
  }
  std::cout << "PASS real phone overlay directions, accept and back preserve native button identities\n";

  assert(!GetContextTouchDrawableOverlaySnapshot(touch_samples::kFadeStart).visible);
  assert(GetContextTouchDrawableOverlaySnapshot(touch_samples::kFadeQuarter).fade_alpha ==
         touch_samples::kFadeQuarterAlpha);
  ++facts.generation;
  facts.phone_visible = false;
  facts.gameplay_allowed = false;
  review::frontend = true;
  Word(base, 0x82BFA124, 3);
  poll(400);
  assert(!GetContextTouchOverlaySnapshot().visible);
  assert(!GetContextTouchDrawableOverlaySnapshot(touch_samples::kFadeQuarter).visible);
  assert(g_runtime.layout.control_count == 0 && g_runtime.pointers.empty());
  const ContextTouchControl map_point{.center_x = touch_samples::kMapCenterX,
                                      .center_y = touch_samples::kMapCenterY};
  send(map_point, AbsolutePointerPhase::kDown, 5);
  send(map_point, AbsolutePointerPhase::kMove, 5, touch_samples::kMapDragPixels);
  send(map_point, AbsolutePointerPhase::kUp, 5, touch_samples::kMapDragPixels);
  poll(401);
  neutral_native();
  base[first_control + touch_samples::kMapXCurrent] = 127;
  GTA4_TouchObserveControlReplay(context, base, first_control, 0, 401);
  assert(base[first_control + touch_samples::kMapXCurrent] != 127);
  GTA4_SetTouchTitleInputOwned(true);
  neutral_native();
  TouchNativePadState disabled_pad;
  assert(!GetTouchNativePadState(401, &disabled_pad));
  assert(!GetContextTouchOverlaySnapshot().visible);
  poll(402);
  GTA4_SetTouchTitleInputOwned(false);
  poll(403);
  neutral_native();
  assert(!GetContextTouchDrawableOverlaySnapshot(touch_samples::kFadeFull).visible);
  std::cout << "PASS pause map immediately hides every circle while native map drag and title ownership remain correct\n";

  ++facts.generation;
  facts.gameplay_allowed = true;
  review::frontend = false;
  poll(500);
  jump = find(TouchAction::kJumpClimb);
  send(jump, AbsolutePointerPhase::kDown, 6);
  send(jump, AbsolutePointerPhase::kCancel, 6);
  poll(501);
  assert(pad(501).buttons == 0);
  send(jump, AbsolutePointerPhase::kDown, 7);
  poll(502);
  assert(pad(502).buttons == X_INPUT_GAMEPAD_X);
  review::enabled = false;
  poll(503);
  neutral_native();
  assert(!GetTouchNativePadState(503, &disabled_pad));
  assert(!GetContextTouchOverlaySnapshot().visible);
  review::enabled = true;
  poll(504);
  assert(pad(504).buttons == 0);
  std::cout << "PASS cancel/disable release owners while context capture continues each poll\n";

  ++facts.generation;
  facts.armed = true;
  poll(600);
  const auto fire = find(TouchAction::kFire);
  const auto look = find(TouchAction::kCamera);
  send(fire, AbsolutePointerPhase::kDown, 8);
  send(look, AbsolutePointerPhase::kDown, 9);
  send(look, AbsolutePointerPhase::kMove, 9, touch_samples::kCameraStep);
  poll(601);
  assert(pad(601).right_trigger == 255);
  assert(g_runtime.look_x != 0);
  review::enabled = false;
  assert(!GetContextTouchOverlaySnapshot().visible);
  assert(!GetTouchNativePadState(601, &disabled_pad));
  neutral_native();
  NeutralAxes(base, first_control, 0);
  GTA4_TouchObserveControlReplay(context, base, first_control, 0, 601);
  assert(base[first_control + touch_samples::kLookRightCurrent] == 127);
  poll(602);
  assert(g_runtime.pointers.empty() && g_runtime.look_x == 0);
  std::cout << "PASS disabling policy immediately suppresses held trigger, camera and overlay\n";

  review::enabled = true;
  poll(610);
  publish_radar(true, true);
  constexpr auto test_key = static_cast<uint16_t>(rex::ui::VirtualKey::kSpace);
  const auto hold_gameplay = [&](uint64_t held_epoch) {
    send(fire, AbsolutePointerPhase::kDown, 30);
    send(look, AbsolutePointerPhase::kDown, 31);
    send(look, AbsolutePointerPhase::kMove, 31, touch_samples::kCameraStep);
    send(kRadarCenter, AbsolutePointerPhase::kDown, 32);
    send(kRadarCenter, AbsolutePointerPhase::kUp, 32);
    review::publish_test_key = true;
    poll(held_epoch);
    assert(pad(held_epoch).right_trigger == 255 && g_runtime.look_x != 0);
    assert(GTA4_TouchVirtualKeyDown(test_key) && GTA4_TouchVirtualKeyPressed(held_epoch, test_key));
    X_INPUT_GAMEPAD compatibility{};
    assert(mnk::ReadVirtualControllerCompatibilityGamepad(0, compatibility));
    assert(uint16_t(compatibility.buttons) == X_INPUT_GAMEPAD_X);
    replay_pause(held_epoch, true);
  };
  const auto suppressed_gameplay = [&](uint64_t stale_epoch) {
    assert(!GetTouchNativePadState(stale_epoch, &disabled_pad));
    neutral_native();
    assert(!GTA4_TouchVirtualKeyDown(test_key) && !GTA4_TouchVirtualKeyPressed(stale_epoch, test_key));
    X_INPUT_GAMEPAD compatibility{};
    assert(!mnk::ReadVirtualControllerCompatibilityGamepad(0, compatibility));
    assert(uint16_t(compatibility.buttons) == 0);
    NeutralAxes(base, first_control, 0);
    replay_pause(stale_epoch, false);
    assert(base[first_control + touch_samples::kLookRightCurrent] == 127);
    assert(!GetContextTouchOverlaySnapshot().visible);
    assert(!GetContextTouchDrawableOverlaySnapshot(MonotonicNanoseconds()).visible);
    assert(g_runtime.pointers.empty() && g_runtime.look_x == 0);
  };
  const auto resumed_gameplay = [&](uint64_t resumed_epoch) {
    review::publish_test_key = false;
    poll(resumed_epoch);
    assert(GetContextTouchOverlaySnapshot().visible && ContextTouchGameplayInputAdmitted());
    neutral_native();
    replay_pause(resumed_epoch, false);
    assert(!GTA4_TouchVirtualKeyDown(test_key) && !GTA4_TouchVirtualKeyPressed(resumed_epoch, test_key));
  };

  hold_gameplay(611);
  {
    ContextTouchGameplayTransition loading;
    // The presentation hook suspends before the guest publishes loading facts.
    assert(!facts.loading && facts.gameplay_allowed);
    suppressed_gameplay(611);
    facts.loading = true;
    poll(612);
    suppressed_gameplay(612);
    facts.loading = false;
  }
  suppressed_gameplay(612);
  resumed_gameplay(613);
  hold_gameplay(614);
  {
    ContextTouchGameplayTransition cutscene;
    suppressed_gameplay(614);
    facts.cutscene = true;
    poll(615);
    suppressed_gameplay(615);
    facts.cutscene = false;
  }
  suppressed_gameplay(615);
  resumed_gameplay(616);
  std::cout << "PASS loading and cutscene transitions immediately clear native replay, camera and compatibility keys\n";

  hold_gameplay(617);
  SuspendContextTouchGameplay();
  suppressed_gameplay(617);
  resumed_gameplay(618);
  hold_gameplay(619);
  review::suspend_during_capture = true;
  poll(620);
  assert(facts.presentation_revision != ContextTouchPresentationRevision());
  suppressed_gameplay(620);
  resumed_gameplay(621);
  hold_gameplay(622);
  review::suspend_during_collect = true;
  poll(623);
  suppressed_gameplay(623);
  resumed_gameplay(624);
  std::cout << "PASS suspension during context capture or virtual-key collection cannot restore stale gameplay\n";

  send(kRadarCenter, AbsolutePointerPhase::kDown, 33);
  poll(625);
  replay_pause(625, false);
  SuspendContextTouchGameplay();
  suppressed_gameplay(625);
  send(kRadarCenter, AbsolutePointerPhase::kUp, 33);
  poll(626);
  replay_pause(626, false);
  send(kRadarCenter, AbsolutePointerPhase::kDown, 34);
  send(kRadarCenter, AbsolutePointerPhase::kUp, 34);
  poll(627);
  replay_pause(627, true);
  poll(628);
  replay_pause(628, false);
  publish_radar(false, false);
  std::cout << "PASS suspension cancels an unfinished minimap gesture until a new tap begins\n";

  review::enabled = false;

  ++facts.generation;
  facts.gameplay_allowed = false;
  review::frontend = true;
  Word(base, 0x82BFA124, 0);
  constexpr auto frontend_object = touch_samples::kFrontendObject;
  Word(base, 0x82CC7BD0, frontend_object);
  base[frontend_object + 4] = 1;
  base[frontend_object + 2944] = 1;
  base[frontend_object + 2964] = 1;
  Word(base, frontend_object + 3112, touch_samples::kFrontendColumnBits);
  Word(base, frontend_object + 3120, touch_samples::kFrontendColumnBits);
  Word(base, frontend_object + 3168, touch_samples::kFrontendHeightBits);
  context.r3.u32 = 0;
  GTA4_TouchCaptureFrontendDraw(context, base, &DrawFrontendRow);
  const ContextTouchControl native_row{.center_x = touch_samples::kFrontendRowX,
                                        .center_y = touch_samples::kFrontendRowY};
  send(native_row, AbsolutePointerPhase::kDown, 10);
  send(native_row, AbsolutePointerPhase::kUp, 10);
  poll(700);
  assert(review::row_selections == 1 && review::selected_row == 0);
  assert(!GetContextTouchOverlaySnapshot().visible);
  GTA4_TouchObserveControlReplay(context, base, first_control, 0, 700);
  assert(base[first_control + touch_samples::kFrontendAcceptCurrent] == 255);
  // The native settings handler changes its existing cvar after receiving
  // this accept action. The following poll must start with fresh owners.
  review::enabled = true;
  poll(701);
  assert(!GetContextTouchOverlaySnapshot().visible);
  assert(!GetContextTouchDrawableOverlaySnapshot(touch_samples::kFadeFull).visible);
  assert(g_runtime.pointers.empty());
  neutral_native();
  std::cout << "PASS captured native menu rows remain touchable while Off and re-enabling keeps pause free of circles\n";

  review::enabled = false;
  for (uint64_t blocked_epoch : {800, 801, 802}) {
    review::pointer_enabled = blocked_epoch != 800;
    presentation.focused = blocked_epoch != 801;
    GTA4_SetTouchTitleInputOwned(blocked_epoch == 802);
    send(native_row, AbsolutePointerPhase::kDown, 11);
    send(native_row, AbsolutePointerPhase::kUp, 11);
    base[first_control + touch_samples::kFrontendAcceptCurrent] = 0;
    poll(blocked_epoch);
    GTA4_TouchObserveControlReplay(context, base, first_control, 0, blocked_epoch);
    assert(review::row_selections == 1);
    assert(base[first_control + touch_samples::kFrontendAcceptCurrent] == 0);
  }
  GTA4_SetTouchTitleInputOwned(false);
  presentation.focused = true;
  review::pointer_enabled = true;
  std::cout << "PASS host capture, focus loss and title input ownership block native touch even while Off\n";

  facts.valid = facts.gameplay_allowed = facts.native_input_allowed = false;
  facts.player_identity = 0;
  facts.base_mode = ContextTouchMode::kDisabled;
  RequestContextTouchEditor();
  assert(ContextTouchEditorCapturesInput());
  poll(900);
  assert(ContextTouchEditorOpen() && IsContextTouchEditorActive());
  const auto preview = GetContextTouchOverlaySnapshot();
  assert(preview.visible && preview.editor && preview.layout.mode == ContextTouchMode::kOnFoot);
  assert(!review::enabled && review::frontend && !facts.valid);
  const auto editor_start = MonotonicNanoseconds();
  assert(!GetContextTouchDrawableOverlaySnapshot(editor_start).visible);
  assert(GetContextTouchDrawableOverlaySnapshot(editor_start + touch_samples::kFadeSecond).fade_alpha == 1.0f);
  send(native_row, AbsolutePointerPhase::kDown, 12);
  send(native_row, AbsolutePointerPhase::kUp, 12);
  base[first_control + touch_samples::kFrontendAcceptCurrent] = 0;
  poll(901);
  GTA4_TouchObserveControlReplay(context, base, first_control, 0, 901);
  assert(review::row_selections == 1 && base[first_control + touch_samples::kFrontendAcceptCurrent] == 0);
  const auto editable = find(TouchAction::kFire);
  send(editable, AbsolutePointerPhase::kDown, 12);
  send(editable, AbsolutePointerPhase::kMove, 12, -touch_samples::kCameraStep);
  send(editable, AbsolutePointerPhase::kUp, 12, -touch_samples::kCameraStep);
  poll(902);
  assert(find(TouchAction::kFire).center_x != editable.center_x);
  assert(!GetTouchNativePadState(902, &disabled_pad));
  assert(!ReadTouchGamepad(0, &native));
  const auto done = find(TouchAction::kEditDone);
  send(done, AbsolutePointerPhase::kDown, 13);
  send(done, AbsolutePointerPhase::kUp, 13);
  poll(903);
  assert(!ContextTouchEditorRequested() && !IsContextTouchEditorActive());
  assert(!GetContextTouchOverlaySnapshot().visible && !review::enabled);
  assert(!ReadTouchGamepad(0, &native));
  const auto editor_closed = g_runtime.editor_close_started_ns;
  const auto fading_editor = GetContextTouchDrawableOverlaySnapshot(
      editor_closed + touch_samples::kFadeQuarterDuration);
  assert(fading_editor.visible && fading_editor.editor &&
         fading_editor.fade_alpha == touch_samples::kEditorFadeOutQuarterAlpha);
  assert(ContextTouchEditorCapturesInput(editor_closed + touch_samples::kFadeHalfDuration));
  send(native_row, AbsolutePointerPhase::kDown, 14);
  send(map_point, AbsolutePointerPhase::kDown, 15);
  send(map_point, AbsolutePointerPhase::kMove, 15, touch_samples::kMapDragPixels);
  send(map_point, AbsolutePointerPhase::kUp, 15, touch_samples::kMapDragPixels);
  Word(base, 0x82BFA124, 3);
  base[first_control + touch_samples::kMapXCurrent] = 127;
  poll(904);
  GTA4_TouchObserveControlReplay(context, base, first_control, 0, 904);
  assert(review::row_selections == 1 && base[first_control + touch_samples::kMapXCurrent] == 127);
  assert(!GetContextTouchDrawableOverlaySnapshot(editor_closed + touch_samples::kFadeSecond).visible);
  assert(!ContextTouchEditorCapturesInput(editor_closed + touch_samples::kFadeSecond));
  Word(base, 0x82BFA124, 0);
  send(native_row, AbsolutePointerPhase::kUp, 14);
  send(native_row, AbsolutePointerPhase::kDown, 16);
  send(native_row, AbsolutePointerPhase::kUp, 16);
  poll(905);
  GTA4_TouchObserveControlReplay(context, base, first_control, 0, 905);
  assert(review::row_selections == 2 && base[first_control + touch_samples::kFrontendAcceptCurrent] == 255);
  assert(!GetContextTouchDrawableOverlaySnapshot(editor_closed + touch_samples::kFadeSecond).visible);
  assert(!ContextTouchEditorRequested() && !review::enabled);
  std::cout << "PASS explicit main-menu editor works without a player and returns native touch only after Done fade without click-through\n";

  ++facts.generation;
  facts.valid = facts.playing = facts.input_user_known = facts.gameplay_allowed = facts.native_input_allowed = true;
  facts.player_identity = 0x100000;
  facts.base_mode = ContextTouchMode::kOnFoot;
  facts.weapon_hud_bounds = kWeaponHudBounds;
  review::enabled = true;
  review::frontend = false;
  const auto weapon_poll = [&](uint64_t weapon_epoch, uint16_t expected) {
    poll(weapon_epoch);
    const auto state = pad(weapon_epoch);
    if (state.buttons != expected)
      std::cerr << "unexpected weapon HUD buttons at epoch " << weapon_epoch << '\n';
    assert(state.buttons == expected);
    assert(ReadTouchGamepad(0, &native) && uint16_t(native.buttons) == expected);
  };
  weapon_poll(1000, 0);
  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 40);
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 40);
  weapon_poll(1001, X_INPUT_GAMEPAD_DPAD_RIGHT);
  assert(pad(1001).buttons == X_INPUT_GAMEPAD_DPAD_RIGHT);
  assert(!QueueContextTouchWeaponCycle(1001, 40));
  weapon_poll(1002, 0);
  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 41);
  weapon_poll(1003, 0);
  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 66);
  weapon_poll(1004, 0);
  assert(!g_runtime.pointers.contains(66));
  send(kWeaponHudCenter, AbsolutePointerPhase::kMove, 66, touch_samples::kCameraStep);
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 66, touch_samples::kCameraStep);
  send(kWeaponHudCenter, AbsolutePointerPhase::kMove, 41);
  review::events.back().timestamp_ns = touch_samples::kHoldTimestamp;
  weapon_poll(1005, 0);
  assert(!g_runtime.weapon_wheel_open && g_runtime.look_x == 0 && !g_runtime.pointers.contains(66));
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 41);
  weapon_poll(1006, X_INPUT_GAMEPAD_DPAD_RIGHT);
  weapon_poll(1007, 0);
  std::cout << "PASS native weapon HUD tap cycles once while holds and a second finger cannot repeat, pan or open a wheel\n";

  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 42);
  send(kWeaponHudCenter, AbsolutePointerPhase::kCancel, 42);
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 42);
  weapon_poll(1010, 0);
  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 43);
  send(kWeaponHudCenter, AbsolutePointerPhase::kMove, 43, kWeaponHudDrag);
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 43);
  weapon_poll(1011, 0);
  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 44);
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 44, kWeaponHudDrag);
  weapon_poll(1012, 0);
  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 45);
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 45, kWeaponHudOutside);
  weapon_poll(1013, 0);
  send(kWeaponCameraOrigin, AbsolutePointerPhase::kDown, 46);
  send(kWeaponCameraOrigin, AbsolutePointerPhase::kUp, 46);
  weapon_poll(1014, 0);
  facts.weapon_hud_bounds.reset();
  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 47);
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 47);
  weapon_poll(1015, 0);
  facts.weapon_hud_bounds = kWeaponHudBounds;
  weapon_poll(1016, 0);
  std::cout << "PASS cancelled, dragged, coalesced, outside and hidden weapon HUD taps cannot cycle\n";

  send(kWeaponCameraOrigin, AbsolutePointerPhase::kDown, 48);
  weapon_poll(1020, 0);
  assert(g_runtime.pointers.at(48).control.action == TouchAction::kCamera);
  send(kWeaponHudCenter, AbsolutePointerPhase::kMove, 48);
  weapon_poll(1021, 0);
  assert(g_runtime.look_x != 0 && g_runtime.pointers.at(48).control.action == TouchAction::kCamera);
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 48);
  weapon_poll(1022, 0);
  assert(!g_runtime.pointers.contains(48));
  weapon_poll(1023, 0);
  const auto weapon_attack = find(TouchAction::kFire);
  const auto weapon_movement = find(TouchAction::kMove);
  send(weapon_attack, AbsolutePointerPhase::kDown, 49);
  send(weapon_movement, AbsolutePointerPhase::kDown, 50);
  send(weapon_movement, AbsolutePointerPhase::kMove, 50, touch_samples::kMapDragPixels);
  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 51);
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 51);
  weapon_poll(1030, X_INPUT_GAMEPAD_DPAD_RIGHT);
  assert(pad(1030).right_trigger == 255 && pad(1030).left_x > 0);
  weapon_poll(1031, 0);
  assert(pad(1031).right_trigger == 255 && pad(1031).left_x > 0);
  send(weapon_attack, AbsolutePointerPhase::kUp, 49);
  send(weapon_movement, AbsolutePointerPhase::kUp, 50);
  weapon_poll(1032, 0);
  neutral_native();
  std::cout << "PASS camera drags crossing the weapon HUD and simultaneous attack/movement keep their owners\n";

  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 52);
  weapon_poll(1040, 0);
  presentation.focused = false;
  poll(1041);
  assert(!GetTouchNativePadState(1041, &disabled_pad));
  presentation.focused = true;
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 52);
  weapon_poll(1042, 0);
  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 53);
  weapon_poll(1043, 0);
  ++presentation.generation;
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 53);
  weapon_poll(1044, 0);
  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 54);
  weapon_poll(1050, 0);
  review::frontend = true;
  poll(1051);
  assert(!GetTouchNativePadState(1051, &disabled_pad));
  review::frontend = false;
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 54);
  weapon_poll(1052, 0);
  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 55);
  weapon_poll(1060, 0);
  RequestContextTouchEditor();
  poll(1061);
  assert(IsContextTouchEditorActive() && !GetTouchNativePadState(1061, &disabled_pad));
  const auto weapon_editor_done = find(TouchAction::kEditDone);
  send(weapon_editor_done, AbsolutePointerPhase::kDown, 56);
  send(weapon_editor_done, AbsolutePointerPhase::kUp, 56);
  poll(1062);
  assert(!GetTouchNativePadState(1062, &disabled_pad));
  const auto weapon_editor_closed = g_runtime.editor_close_started_ns;
  assert(!ContextTouchEditorCapturesInput(weapon_editor_closed + touch_samples::kFadeSecond));
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 55);
  weapon_poll(1063, 0);
  std::cout << "PASS focus, surface generation, menu and editor changes invalidate pending weapon HUD taps\n";

  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 57);
  weapon_poll(1070, 0);
  {
    ContextTouchGameplayTransition loading;
    assert(!GetTouchNativePadState(1070, &disabled_pad));
    facts.loading = true;
    poll(1071);
    assert(!GetTouchNativePadState(1071, &disabled_pad));
    facts.loading = false;
  }
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 57);
  weapon_poll(1072, 0);
  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 58);
  weapon_poll(1080, 0);
  {
    ContextTouchGameplayTransition cutscene;
    facts.cutscene = true;
    poll(1081);
    assert(!GetTouchNativePadState(1081, &disabled_pad));
    facts.cutscene = false;
  }
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 58);
  weapon_poll(1082, 0);
  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 59);
  weapon_poll(1090, 0);
  SuspendContextTouchGameplay();
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 59);
  weapon_poll(1091, 0);
  std::cout << "PASS loading, cutscene and between-poll suspension cancel pending weapon HUD cycles\n";

  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 60);
  weapon_poll(1100, 0);
  facts.weapon_hud_bounds.reset();
  weapon_poll(1101, 0);
  facts.weapon_hud_bounds = kWeaponHudBounds;
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 60);
  weapon_poll(1102, 0);
  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 61);
  weapon_poll(1110, 0);
  ++facts.generation;
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 61);
  weapon_poll(1111, 0);
  facts.weapon_hud_bounds.reset();
  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 62);
  weapon_poll(1120, 0);
  facts.weapon_hud_bounds = kWeaponHudBounds;
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 62);
  weapon_poll(1121, 0);
  std::cout << "PASS weapon HUD visibility and player context changes cannot finish an older gesture\n";

  struct VehicleCycleCase {
    ContextTouchMode mode;
    uint64_t ready_epoch, tap_epoch, released_epoch;
  };
  constexpr std::array vehicle_cycles = {
      VehicleCycleCase{ContextTouchMode::kVehicleAutomobile, 1140, 1141, 1142},
      VehicleCycleCase{ContextTouchMode::kVehicleBike, 1150, 1151, 1152},
      VehicleCycleCase{ContextTouchMode::kVehicleBoat, 1160, 1161, 1162},
      VehicleCycleCase{ContextTouchMode::kVehiclePassenger, 1170, 1171, 1172},
      VehicleCycleCase{ContextTouchMode::kVehicleDriverUnknown, 1180, 1181, 1182},
  };
  for (const auto& vehicle : vehicle_cycles) {
    ++facts.generation;
    facts.base_mode = vehicle.mode;
    weapon_poll(vehicle.ready_epoch, 0);
    send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 63);
    send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 63);
    weapon_poll(vehicle.tap_epoch, X_INPUT_GAMEPAD_X);
    weapon_poll(vehicle.released_epoch, 0);
  }
  ++facts.generation;
  facts.base_mode = ContextTouchMode::kVehicleHelicopter;
  weapon_poll(1190, 0);
  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 64);
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 64);
  weapon_poll(1191, 0);
  assert(!ContextTouchWeaponCycleAdmitted());
  ++facts.generation;
  facts.base_mode = ContextTouchMode::kOnFoot;
  facts.phone_visible = true;
  weapon_poll(1200, 0);
  send(kWeaponHudCenter, AbsolutePointerPhase::kDown, 65);
  send(kWeaponHudCenter, AbsolutePointerPhase::kUp, 65);
  weapon_poll(1201, 0);
  assert(!ContextTouchWeaponCycleAdmitted());
  std::cout << "PASS vehicle weapon HUD cycles use native X while helicopter and phone contexts omit cycling\n";
  ShutdownContextTouchControls();
  assert(!review::provider);
  munmap(base, memory_size);
}
