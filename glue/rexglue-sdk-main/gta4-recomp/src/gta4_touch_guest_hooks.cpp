#include "gta4_init.h"
#include "gta4_touch_coordinator.h"
#include "input/context_touch_context.h"
#include "input/context_touch_controls.h"
#include "input/context_touch_radar.h"
#if !defined(GTA4_TOUCH_LEGACY_HOST)
#include "gta4_aspect_hooks.h"
#endif

#include <bit>
#include <cmath>
#include <cstring>
#include <deque>
#include <limits>
#include <mutex>
#include <unordered_map>
#include <rex/input/absolute_pointer.h>
#include <rex/runtime.h>

namespace gta4::input {
namespace {
constexpr uint32_t kRadarRenderPhaseVtable = 0x82013C9C;
constexpr uint32_t kViewportCopyVtable = 0x82001150;
constexpr uint32_t kCurrentRenderPhase = 0x82FEFDE4;
constexpr uint32_t kCurrentViewport = 0x831C2200;
constexpr uint32_t kRadarMapMode = 0x82BFA144;
constexpr uint32_t kTouchLoadingActive = 0x831D5335;
constexpr uint32_t kTouchCutsceneState = 0x82B977F0;
constexpr uint32_t kTouchCutscenePreparing = 0x82B977FC;
constexpr uint32_t kPhaseViewportOffset = 176;
constexpr uint32_t kCopiedViewportOffset = 16;
constexpr uint32_t kViewportCopySize = 1040;
constexpr size_t kMaximumRadarViewportCopies = 32768;

bool RadarSpan(uint8_t* base, uint32_t address, size_t size, bool write = false) {
  if (!base || !address || !size || uint64_t(address) + size > uint64_t(UINT32_MAX) + 1)
    return false;
  auto* kernel = REX_KERNEL_STATE();
  auto* memory = kernel ? kernel->memory() : nullptr;
  auto* heap = memory ? memory->LookupHeap(address) : nullptr;
  const uint32_t last = uint32_t(uint64_t(address) + size - 1);
  if (!heap || heap != memory->LookupHeap(last)) return false;
  const auto access = heap->QueryRangeAccess(address, last);
  using rex::memory::PageAccess;
  return access == PageAccess::kReadWrite || access == PageAccess::kExecuteReadWrite ||
      (!write && (access == PageAccess::kReadOnly || access == PageAccess::kExecuteReadOnly));
}
uint32_t RadarRead(uint8_t* base, uint32_t address) {
  uint32_t value;
  std::memcpy(&value, rex::memory::GuestPtr(base, address), sizeof(value));
  return __builtin_bswap32(value);
}

void ObserveTouchPresentationBlock(uint8_t* base) {
  // sub_821F4150 runs before normal frames, loading slides, and the loading
  // callback draw path. Publish host state here even if input polling stops.
  // State/preparation match sub_821E3788 / sub_821EC8C8; loading matches
  // sub_82144188. A false sample cannot revive the previous gameplay poll.
  if (!RadarSpan(base, kTouchLoadingActive, sizeof(uint8_t)) ||
      !RadarSpan(base, kTouchCutsceneState, sizeof(uint32_t)) ||
      !RadarSpan(base, kTouchCutscenePreparing, sizeof(uint32_t)) ||
      *rex::memory::GuestPtr(base, kTouchLoadingActive) != 0 ||
      RadarRead(base, kTouchCutsceneState) != 0 ||
      RadarRead(base, kTouchCutscenePreparing) != 0) {
    SuspendContextTouchGameplay();
  }
}
double RadarFloat(uint8_t* base, uint32_t address) {
  return std::bit_cast<float>(RadarRead(base, address));
}

// sub_82163270 stores the runtime HUD_WEAPON_ICON index at config+1008.
// sub_821C62C0 admits widgets by their layer byte before sub_821C5148 draws.
constexpr uint32_t kWeaponHudIndex = 0x82B39E88;
constexpr uint32_t kWeaponHudTable = 0x82B39990;
constexpr uint32_t kWeaponHudCount = 0x82B39A90;
constexpr uint32_t kActiveHudCamera = 0x82BEFA54;
constexpr uint32_t kMaximumHudWidgets = 64;

struct WeaponHudPass {
  TouchWeaponHudSnapshot snapshot;
  uint32_t index = UINT32_MAX;
  bool publish = true;
  bool capture = false;
  bool sprite = false;
};
thread_local WeaponHudPass* weapon_hud_pass = nullptr;

class WeaponHudPassScope {
 public:
  WeaponHudPassScope(const PPCContext& context, uint8_t* base)
      : previous_(weapon_hud_pass) {
    weapon_hud_pass = &pass_;
    const auto facts = GetTouchContextSnapshot();
    pass_.snapshot.epoch = facts.epoch;
    pass_.snapshot.generation = facts.generation;
    pass_.snapshot.presentation_revision = facts.presentation_revision;
    pass_.snapshot.weapon_type = facts.weapon_type;
    pass_.snapshot.weapon_slot = facts.weapon_slot;
    rex::input::TouchPresentationState presentation;
    if (rex::input::GetTouchPresentationState(&presentation) && presentation.valid &&
        presentation.focused) {
      pass_.snapshot.presentation_generation = presentation.generation;
      pass_.capture = facts.valid && facts.playing && facts.gameplay_allowed &&
          facts.native_input_allowed && !facts.frontend && !facts.map && !facts.loading &&
          !facts.cutscene && facts.presentation_revision == ContextTouchPresentationRevision();
    }
    if (!RadarSpan(base, kWeaponHudIndex, 4) || !RadarSpan(base, kWeaponHudCount, 4)) return;
    const uint32_t index = RadarRead(base, kWeaponHudIndex);
    const uint32_t count = RadarRead(base, kWeaponHudCount);
    if (!count || count > kMaximumHudWidgets || index >= count ||
        !RadarSpan(base, kWeaponHudTable + index * 4, 4)) return;
    const uint32_t widget = RadarRead(base, kWeaponHudTable + index * 4);
    if (!RadarSpan(base, widget, 91) || RadarRead(base, widget + 8) != 2) return;
    if (*rex::memory::GuestPtr(base, widget + 90) != context.r4.u8) {
      // The separate loading/foreground layer must not erase a gameplay draw.
      pass_.publish = false;
      return;
    }
    if (context.r3.u32 != UINT32_MAX) {
      if (!RadarSpan(base, kActiveHudCamera, 4)) return;
      const uint32_t camera = RadarRead(base, kActiveHudCamera);
      if (!RadarSpan(base, camera, 1348)) return;
      if (RadarRead(base, camera + 1344) != context.r3.u32) {
        // The native dispatcher returns for other cameras as well; those
        // calls do not constitute a suppressed pass for the active HUD.
        pass_.publish = false;
        return;
      }
    }
    pass_.index = index;
  }
  ~WeaponHudPassScope() {
    weapon_hud_pass = previous_;
    if (!pass_.publish) return;
    rex::input::TouchPresentationState presentation;
    if (pass_.snapshot.presentation_revision != ContextTouchPresentationRevision() ||
        !rex::input::GetTouchPresentationState(&presentation) || !presentation.valid ||
        !presentation.focused || presentation.generation != pass_.snapshot.presentation_generation)
      pass_.snapshot.bounds.reset();
    // Empty is deliberate: hidden widgets, zero alpha, missing textures and
    // failed command allocations must invalidate the previous hit region.
    PublishTouchWeaponHudSnapshot(pass_.snapshot);
  }

 private:
  WeaponHudPass pass_;
  WeaponHudPass* previous_;
};

void ObserveWeaponHudRectangle(uint8_t* base, uint32_t position, uint32_t size,
                               uint32_t texture, uint32_t color) {
  auto* pass = weapon_hud_pass;
  if (!pass || !pass->capture || !pass->sprite || !texture ||
      !RadarSpan(base, position, 8) || !RadarSpan(base, size, 8)) return;
  const double x = RadarFloat(base, position), y = RadarFloat(base, position + 4);
  const double width = RadarFloat(base, size), height = RadarFloat(base, size + 4);
  if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(width) ||
      !std::isfinite(height) || width <= 0.0 || height <= 0.0) return;
  gta4::aspect::Rect rect{x, y, x + width, y + height};
#if !defined(GTA4_TOUCH_LEGACY_HOST)
  // The queued command retains this same UiContext in FinalizeDc; the
  // immediate path applies it in DrawQuad. Native alignment is already done.
  const auto layout = gta4::aspect::CurrentUi(base);
  if (layout.active) rect = layout.transform.Map(rect);
#endif
  pass->snapshot.bounds = ClipTouchWeaponHudBounds(
      rect.left, rect.top, rect.right, rect.bottom, color);
}

gta4::aspect::Rect RadarViewportRect(uint8_t* base, uint32_t viewport, uint32_t offset) {
  const double x = RadarFloat(base, viewport + offset);
  const double y = RadarFloat(base, viewport + offset + 4);
  const double width = RadarFloat(base, viewport + offset + 8);
  const double height = RadarFloat(base, viewport + offset + 12);
  if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(width) || !std::isfinite(height) ||
      x < 0.0 || y < 0.0 || width <= 0.0 || height <= 0.0 ||
      x + width > 1.0 || y + height > 1.0) return {};
  return {x, y, x + width, y + height};
}
bool RadarRectValid(gta4::aspect::Rect rect) {
  return rect.right > rect.left && rect.bottom > rect.top;
}
struct RadarViewportCopy {
  uint32_t token = 0;
  uint64_t serial = 0;
  TouchRadarPass pass;
};
std::mutex radar_viewport_mutex;
std::unordered_map<uint32_t, RadarViewportCopy> radar_viewport_copies;
std::deque<std::pair<uint32_t, uint64_t>> radar_viewport_order;
uint64_t radar_viewport_serial = 0;
struct PendingRadarPass {
  uint32_t phase = 0;
  uint32_t command = 0;
  uint32_t token = 0;
};
thread_local PendingRadarPass pending_radar_pass;

void CopyTouchRadarViewport(PPCContext& context, uint8_t* base) {
  const uint32_t command = context.r3.u32;
  const uint32_t source = context.r4.u32;
  const bool radar = RadarSpan(base, kCurrentRenderPhase, sizeof(uint32_t));
  const uint32_t phase = radar ? RadarRead(base, kCurrentRenderPhase) : 0;
  const bool radar_phase = RadarSpan(base, phase, kPhaseViewportOffset) &&
      RadarRead(base, phase) == kRadarRenderPhaseVtable &&
      uint64_t(phase) + kPhaseViewportOffset == source;
  pending_radar_pass = {};
  {
    std::lock_guard lock(radar_viewport_mutex);
    radar_viewport_copies.erase(command);
  }
  __imp__sub_821BC948(context, base);
  if (!radar_phase || !RadarSpan(base, command, kViewportCopySize, true) ||
      RadarRead(base, command) != kViewportCopyVtable) return;

  const auto facts = GetTouchContextSnapshot();
  TouchRadarPass pass;
  pass.gameplay = RadarSpan(base, kRadarMapMode, 1) &&
      *rex::memory::GuestPtr(base, kRadarMapMode) == 0 && !facts.frontend && !facts.map;
  const uint32_t viewport = command + kCopiedViewportOffset;
  if (pass.gameplay) {
    // Generated sub_82155680 assigns the HUD_RADAR rectangle to its camera.
    // sub_8239C9B8 retains that rectangle in phase+176; sub_821BC948 copies
    // it into this command. Layer vertices are local 0..1, including masks.
    const auto authored = RadarViewportRect(base, viewport, 664);
    rex::input::TouchPresentationState presentation;
    if (RadarRectValid(authored) && RadarRead(base, viewport + 688) > 0 &&
        RadarRead(base, viewport + 692) > 0 && rex::input::GetTouchPresentationState(&presentation)) {
      const double output_width = std::round(double(presentation.physical_output_width));
      const double output_height = std::round(double(presentation.physical_output_height));
      if (output_width > 0.0 && output_height > 0.0 && output_width <= UINT32_MAX &&
          output_height <= UINT32_MAX && presentation.output_height > 0.0f) {
        const gta4::aspect::Extent output{uint32_t(output_width), uint32_t(output_height)};
#if defined(GTA4_TOUCH_LEGACY_HOST)
        // The legacy host owns its final output scaling. Touch only changes Y.
        gta4::aspect::Transform transform;
#else
        auto transform = gta4::aspect::RadarLayout(output);
#endif
        // The drawable snapshot retains visibility through the one-second
        // fade, while native frontend/map state always keeps its own viewport.
        if (GetContextTouchDrawableOverlaySnapshot().visible ||
            GetContextTouchOverlaySnapshot().visible || IsContextTouchEditorActive()) {
          const double top = double(presentation.safe_area_y) / presentation.output_height;
          const double bottom = (double(presentation.safe_area_y) + presentation.safe_area_height) /
                                presentation.output_height;
          transform = gta4::aspect::TopLeftRadarViewport(authored, top, bottom, transform);
        }
        const auto displayed = transform.Map(authored);
        if (RadarRectValid(displayed) && !transform.identity()) {
          // Calling the retail setter updates both clipped/unclipped rectangles,
          // viewport matrices and projection offsets. Source camera and phase
          // remain untouched; the queued copy owns all resulting state.
          PPCContext call = context;
          call.r3.u32 = viewport;
          call.f1.f64 = displayed.left;
          call.f2.f64 = displayed.top;
          call.f3.f64 = displayed.right - displayed.left;
          call.f4.f64 = displayed.bottom - displayed.top;
          call.f5.f64 = RadarFloat(base, viewport + 680);
          call.f6.f64 = RadarFloat(base, viewport + 684);
          __imp__sub_828BE5A0(call, base);
        }
        pass.bounds = RadarViewportRect(base, viewport, 640);
      }
    }
  }
  const uint32_t token = gta4::aspect::StableDcToken(RadarRead(base, command + 4));
  {
    std::lock_guard lock(radar_viewport_mutex);
    const uint64_t serial = ++radar_viewport_serial;
    radar_viewport_copies[command] = {token, serial, pass};
    radar_viewport_order.emplace_back(command, serial);
    for (; radar_viewport_order.size() > kMaximumRadarViewportCopies; radar_viewport_order.pop_front()) {
      const auto [old_command, old_serial] = radar_viewport_order.front();
      const auto it = radar_viewport_copies.find(old_command);
      if (it != radar_viewport_copies.end() && it->second.serial == old_serial)
        radar_viewport_copies.erase(it);
    }
  }
  pending_radar_pass = {phase, command, token};
}
}  // namespace

void DrawTouchWeaponHudSprite(PPCContext& context, uint8_t* base,
                              void (*original)(PPCContext&, uint8_t*)) {
  auto* pass = weapon_hud_pass;
  struct SpriteScope {
    WeaponHudPass* pass;
    bool previous;
    ~SpriteScope() { if (pass) pass->sprite = previous; }
  } scope{pass, pass && pass->sprite};
  if (pass) pass->sprite = pass->publish && context.r3.u32 == pass->index;
  original(context, base);
}

TouchRadarPass ConsumeTouchRadarPass(uint8_t* base, uint32_t phase) noexcept {
  const auto pending = pending_radar_pass;
  pending_radar_pass = {};
  if (pending.phase != phase || !RadarSpan(base, pending.command, 8) ||
      RadarRead(base, pending.command) != kViewportCopyVtable ||
      gta4::aspect::StableDcToken(RadarRead(base, pending.command + 4)) != pending.token) return {};
  std::lock_guard lock(radar_viewport_mutex);
  const auto it = radar_viewport_copies.find(pending.command);
  return it != radar_viewport_copies.end() && it->second.token == pending.token ?
      it->second.pass : TouchRadarPass{};
}

bool TouchRadarLocalViewport(uint8_t* base) noexcept {
  if (!RadarSpan(base, kCurrentViewport, sizeof(uint32_t))) return false;
  const uint32_t viewport = RadarRead(base, kCurrentViewport);
  if (viewport < kCopiedViewportOffset) return false;
  const uint32_t command = viewport - kCopiedViewportOffset;
  if (!RadarSpan(base, command, 8) || RadarRead(base, command) != kViewportCopyVtable) return false;
  const uint32_t token = gta4::aspect::StableDcToken(RadarRead(base, command + 4));
  std::lock_guard lock(radar_viewport_mutex);
  const auto it = radar_viewport_copies.find(command);
  return it != radar_viewport_copies.end() && it->second.token == token && it->second.pass.gameplay;
}
}  // namespace gta4::input

extern "C" void sub_821C62C0(PPCContext& context, uint8_t* base) {
  const gta4::input::WeaponHudPassScope pass(context, base);
  __imp__sub_821C62C0(context, base);
}

extern "C" void sub_821BEF30(PPCContext& context, uint8_t* base) {
  const uint32_t position = context.r4.u32, size = context.r5.u32;
  const uint32_t texture = context.r6.u32, color = context.r7.u32;
  const bool weapon_sprite = context.r9.u32 == 2;
  __imp__sub_821BEF30(context, base);
  // The admitted sprite path submits this command immediately after its
  // constructor. Allocation failure never reaches this boundary.
  if (weapon_sprite)
    gta4::input::ObserveWeaponHudRectangle(base, position, size, texture, color);
}

extern "C" void sub_821C3930(PPCContext& context, uint8_t* base) {
  const uint32_t position = context.r3.u32, size = context.r4.u32;
  const uint32_t texture = context.r5.u32, color = context.r6.u32;
  __imp__sub_821C3930(context, base);
  gta4::input::ObserveWeaponHudRectangle(base, position, size, texture, color);
}

#if defined(GTA4_TOUCH_LEGACY_HOST)
extern "C" void sub_821C5148(PPCContext& context, uint8_t* base) {
  gta4::input::DrawTouchWeaponHudSprite(context, base, __imp__sub_821C5148);
}
#endif

extern "C" void sub_821F4150(PPCContext& context, uint8_t* base) {
  gta4::input::ObserveTouchPresentationBlock(base);
  __imp__sub_821F4150(context, base);
}

extern "C" void sub_821EC3F0(PPCContext& context, uint8_t* base) {
  // START_CUTSCENE_NOW / INIT_CUTSCENE enter here before either native
  // cutscene state word becomes nonzero. Keep reentrant polls suppressed.
  const gta4::input::ContextTouchGameplayTransition transition;
  __imp__sub_821EC3F0(context, base);
}

#if defined(GTA4_TOUCH_LEGACY_HOST)
extern "C" void sub_82145420(PPCContext& context, uint8_t* base) {
  // The modern host composes this guard with its existing intro hook.
  const gta4::input::ContextTouchGameplayTransition transition;
  __imp__sub_82145420(context, base);
}
#endif

extern "C" void sub_821BC948(PPCContext& context, uint8_t* base) {
  gta4::input::CopyTouchRadarViewport(context, base);
}

// These parser boundaries are shared by both applications. Only tokens
// resolved inside a submitted help draw are retained by the observer.
extern "C" void sub_821F2360(PPCContext& context, uint8_t* base) {
  gta4::input::ResolveTouchHelpToken(context, base, __imp__sub_821F2360);
}

extern "C" void sub_822B7C00(PPCContext& context, uint8_t* base) {
  gta4::input::ResolveTouchHelpBinding(context, base, __imp__sub_822B7C00);
}

// The desktop legacy app has a single Sony poll/replay owner. Embedded
// legacy apps use the identical touch boundaries without that owner.
#if defined(GTA4_TOUCH_STANDALONE_POLL_OWNER)
extern "C" void sub_828CCD60(PPCContext& context, uint8_t* base) {
  const uint64_t epoch = GTA4_TouchCurrentEpoch() + 1;
  GTA4_TouchConsumePoll(context, base, epoch);
  __imp__sub_828CCD60(context, base);
}

extern "C" void sub_822B7DD0(PPCContext& context, uint8_t* base) {
  const uint32_t control = context.r3.u32;
  const uint32_t caller = static_cast<uint32_t>(context.lr);
  __imp__sub_822B7DD0(context, base);
  GTA4_TouchObserveControlReplay(context, base, control, caller, GTA4_TouchCurrentEpoch());
}
#endif

// Modern wrappers compose with their existing keyboard, Sony and diagnostic
// hooks. Legacy has no such translation units and owns these wrappers here.
#if defined(GTA4_TOUCH_LEGACY_HOST)
extern "C" void sub_8229D8A8(PPCContext& context, uint8_t* base) {
  GTA4_TouchCaptureFrontendDraw(context, base, __imp__sub_8229D8A8);
}

extern "C" void sub_82223CF8(PPCContext& context, uint8_t* base) {
  gta4::input::BeginTouchHelpDraw(base, context.r3.u32);
  struct Scope { ~Scope() { gta4::input::EndTouchHelpDraw(); } } scope;
  __imp__sub_82223CF8(context, base);
}

extern "C" void sub_821F6E38(PPCContext& context, uint8_t* base) {
  GTA4_TouchObserveHudSubmit(context, base);
  gta4::input::ObserveTouchHelpText(base, context.r5.u32);
  struct Scope { ~Scope() { gta4::input::EndTouchHelpText(); } } scope;
  __imp__sub_821F6E38(context, base);
}

namespace {
void ScriptQuery(PPCContext& context, uint8_t* base,
                  gta4::input::TouchScriptQueryKind kind,
                  void (*original)(PPCContext&, uint8_t*)) {
  const uint32_t call_context = context.r3.u32;
  const uint32_t thread = gta4::input::ReadTouchScriptThread(base);
  const auto parachute = gta4::input::ReadTouchParachuteState(base);
  const auto facts = gta4::input::GetTouchContextSnapshot();
  original(context, base);
  if (parachute) gta4::input::ObserveTouchParachuteState(*parachute, facts.epoch, thread);
  gta4::input::MergeTouchScriptQueryResult(base, call_context, kind, facts.epoch,
                                           thread, facts.generation);
}
}

extern "C" void sub_825D1AF8(PPCContext& context, uint8_t* base) {
  ScriptQuery(context, base, gta4::input::TouchScriptQueryKind::kRawButton,
               __imp__sub_825D1AF8);
}
extern "C" void sub_825D1B40(PPCContext& context, uint8_t* base) {
  ScriptQuery(context, base, gta4::input::TouchScriptQueryKind::kRawButtonPressed,
               __imp__sub_825D1B40);
}
extern "C" void sub_825D1B88(PPCContext& context, uint8_t* base) {
  ScriptQuery(context, base, gta4::input::TouchScriptQueryKind::kControlHeld,
               __imp__sub_825D1B88);
}
extern "C" void sub_825D1BD0(PPCContext& context, uint8_t* base) {
  ScriptQuery(context, base, gta4::input::TouchScriptQueryKind::kControlPressed,
               __imp__sub_825D1BD0);
}
extern "C" void sub_825D1C18(PPCContext& context, uint8_t* base) {
  ScriptQuery(context, base, gta4::input::TouchScriptQueryKind::kControlAnalog,
               __imp__sub_825D1C18);
}
extern "C" void sub_825D20A0(PPCContext& context, uint8_t* base) {
  const uint32_t call_context = context.r3.u32;
  const uint32_t thread = gta4::input::ReadTouchScriptThread(base);
  const auto parachute = gta4::input::ReadTouchParachuteState(base);
  const auto facts = gta4::input::GetTouchContextSnapshot();
  __imp__sub_825D20A0(context, base);
  if (parachute) gta4::input::ObserveTouchParachuteState(*parachute, facts.epoch, thread);
  gta4::input::MergeTouchScriptAnalogueStickResults(base, call_context, facts.epoch,
                                                    thread, facts.generation);
}
#endif
