#if !defined(GTA4_TOUCH_LEGACY_HOST)
#include "gta4_aspect_hooks.h"
#endif
#include "gta4_touch_coordinator.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <cmath>
#include <cstdint>
#include <limits>
#include <mutex>
#include <optional>
#include <utility>
#include <vector>

#include <rex/cvar.h>
#include <rex/input/absolute_pointer.h>
#include <rex/input/mnk/encoded_action.h>
#include <rex/input/mnk/controller_compatibility.h>
#include <rex/logging.h>

#include "gta4_init.h"
#include "gta4_map_pan_policy.h"
#include "gta4_touch_policy.h"
#include "input/context_touch_context.h"
#include "input/context_touch_controls.h"
#include "input/context_touch_radar.h"

REXCVAR_DEFINE_BOOL(gta4_touch_trace, false, "GTA IV/Input/Touch",
                    "Trace joined touch pointer, layout, and retail action transactions");

namespace {

using gta4::touch::FrontendRow;
using gta4::touch::MapGestureOutput;
using gta4::touch::Point;
using gta4::touch::Rect;
using rex::input::AbsolutePointerEvent;
using rex::input::AbsolutePointerPhase;

constexpr uint32_t kMapScreen = 3;
constexpr uint32_t kCurrentScreenAddress = 0x82BFA124;
constexpr uint32_t kMapZoomLevelAddress = 0x82BF9D88;
constexpr uint32_t kMapZoomMinimum = 0;
constexpr uint32_t kMapZoomMaximum = 5;

constexpr uint32_t kFrontendChannel = 0;
constexpr uint32_t kFrontendChannelTableAddress = 0x82CC7BD0;
constexpr uint32_t kFrontendHudCaller = 0x8229E7C0;
constexpr uint32_t kFrontendRowVisibleOffset = 2944;
constexpr uint32_t kFrontendRowSelectableOffset = 2964;
constexpr uint32_t kFrontendColumnOffsetBase = 3112;
constexpr uint32_t kFrontendHorizontalBaseOffset = 3120;
constexpr uint32_t kFrontendRowHeightOffset = 3168;
constexpr uint32_t kFrontendRowStride = 60;
constexpr uint32_t kFrontendListStride = 1200;
constexpr uint32_t kFrontendPayloadPresentOffset = 4;
constexpr uint32_t kMaximumFrontendRows = 64;
constexpr uint32_t kMaximumFrontendLists = 16;

constexpr uint32_t kRadarQuadCaller = 0x8233AB4C;
constexpr uint32_t kRadarVertexCount = 4;
constexpr uint32_t kRadarVertexStride = 16;
constexpr uint32_t kRadarVertexXOffset = 0;
constexpr uint32_t kRadarVertexYOffset = 4;

constexpr uint32_t kActionArrayOffset = 2328;
constexpr uint32_t kActionStride = 12;
constexpr uint32_t kActionCurrentOffset = 2;
constexpr uint32_t kLastInputTimeOffset = 4200;
constexpr uint32_t kGameInputTimeAddress = 0x82C6C2A4;
constexpr uint8_t kPressed = 255;

enum class Action : uint32_t {
  kFrontendDown = 64,
  kFrontendUp = 65,
  kFrontendLeft = 66,
  kFrontendRight = 67,
  kMapX = 72,
  kMapY = 73,
  kFrontendPause = 76,
  kFrontendAccept = 77,
};

struct FrontendGeometry {
  std::vector<FrontendRow> rows;
  uint64_t generation = 1;
};

struct RadarGeometry {
  Rect bounds{};
  uint64_t generation = 1;
};

struct ReplaySnapshot {
  uint64_t epoch = 0;
  uint32_t input_user = 0;
  bool input_user_known = false;
  int32_t map_x = 0;
  int32_t map_y = 0;
  int32_t zoom_steps = 0;
  bool accept = false;
  bool pause = false;
  bool frontend_active = false;
  bool map_active = false;
  bool context_controls_active = false;
};

struct VirtualKeySnapshot {
  std::array<uint8_t, 256> down{};
  std::array<uint8_t, 256> pressed{};
  uint64_t epoch = 0;
};

struct RowCapture {
  uint32_t channel_object = 0;
  std::vector<FrontendRow> rows;
  bool active = false;
};

struct RadarCapture {
  Rect bounds{};
  bool active = false;
  bool has_quad = false;
};

std::mutex g_geometry_mutex;
FrontendGeometry g_frontend_geometry;
RadarGeometry g_radar_geometry;
thread_local RowCapture g_row_capture;
thread_local RadarCapture g_radar_capture;

std::mutex g_extension_mutex;
GTA4TouchExtension g_extension;
VirtualKeySnapshot g_virtual_keys;

gta4::touch::FrontendTransaction g_frontend_transaction;
gta4::touch::TapTransaction g_minimap_transaction;
uint64_t g_hud_presentation_revision = 0;
gta4::touch::TapTransaction g_weapon_transaction;
RadarGeometry g_weapon_geometry;
uint64_t g_weapon_context_generation = 0;
gta4::touch::MapGesture g_map_gesture;
ReplaySnapshot g_replay;
bool g_context_controls_were_active = false;
std::atomic<bool> g_title_input_owned{false};
std::atomic<uint64_t> g_poll_epoch{0};

uint8_t LoadU8(uint8_t* base, uint32_t address) noexcept {
  return *reinterpret_cast<volatile uint8_t*>(base + address);
}

uint32_t LoadU32(uint8_t* base, uint32_t address) noexcept {
  return __builtin_bswap32(*reinterpret_cast<volatile uint32_t*>(base + address));
}

float LoadFloat(uint8_t* base, uint32_t address) noexcept {
  return std::bit_cast<float>(LoadU32(base, address));
}

void StoreU8(uint8_t* base, uint32_t address, uint8_t value) noexcept {
  *reinterpret_cast<volatile uint8_t*>(base + address) = value;
}

void StoreU32(uint8_t* base, uint32_t address, uint32_t value) noexcept {
  *reinterpret_cast<volatile uint32_t*>(base + address) = __builtin_bswap32(value);
}

uint32_t ActionAddress(uint32_t control, Action action) noexcept {
  return control + kActionArrayOffset + static_cast<uint32_t>(action) * kActionStride;
}

uint8_t ReadAction(uint8_t* base, uint32_t control, Action action) noexcept {
  const uint32_t address = ActionAddress(control, action);
  return rex::input::mnk::DecodeActionMagnitude(LoadU8(base, address),
                                                LoadU8(base, address + kActionCurrentOffset));
}

bool MergeButton(uint8_t* base, uint32_t control, Action action, uint8_t requested) noexcept {
  if (!control || !requested) {
    return false;
  }
  const uint32_t address = ActionAddress(control, action);
  const uint8_t polarity = LoadU8(base, address);
  const uint8_t current = LoadU8(base, address + kActionCurrentOffset);
  const uint8_t merged =
      rex::input::mnk::MergeActionMagnitude(polarity, current, requested);
  if (merged == current) {
    return false;
  }
  StoreU8(base, address + kActionCurrentOffset, merged);
  return true;
}

bool MergeSignedAction(uint8_t* base, uint32_t control, Action action,
                       int32_t requested) noexcept {
  if (!control || !requested) {
    return false;
  }
  const uint32_t address = ActionAddress(control, action);
  const auto merge = rex::input::mnk::MergeSignedAction(
      LoadU8(base, address), LoadU8(base, address + kActionCurrentOffset), requested);
  if (!merge.changed) {
    return false;
  }
  StoreU8(base, address + kActionCurrentOffset, merge.encoded);
  return true;
}

bool RectNearlyEqual(const Rect& left, const Rect& right) noexcept {
  if (left.valid() != right.valid()) {
    return false;
  }
  if (!left.valid()) {
    return true;
  }
  return gta4::touch::NearlyEqual(left.left, right.left) &&
         gta4::touch::NearlyEqual(left.top, right.top) &&
         gta4::touch::NearlyEqual(left.right, right.right) &&
         gta4::touch::NearlyEqual(left.bottom, right.bottom);
}

void IncludeRect(Rect& bounds, const Rect& addition) noexcept {
  if (!addition.valid()) {
    return;
  }
  if (!bounds.valid()) {
    bounds = addition;
    return;
  }
  bounds.left = std::min(bounds.left, addition.left);
  bounds.top = std::min(bounds.top, addition.top);
  bounds.right = std::max(bounds.right, addition.right);
  bounds.bottom = std::max(bounds.bottom, addition.bottom);
}

GTA4TouchExtension ReadExtension() noexcept {
  std::lock_guard lock(g_extension_mutex);
  return g_extension;
}

FrontendGeometry ReadFrontendGeometry() {
  std::lock_guard lock(g_geometry_mutex);
  return g_frontend_geometry;
}

RadarGeometry ReadRadarGeometry() noexcept {
  std::lock_guard lock(g_geometry_mutex);
  return g_radar_geometry;
}

void PublishFrontendGeometry(std::vector<FrontendRow> rows) {
  std::sort(rows.begin(), rows.end(), [](const FrontendRow& left, const FrontendRow& right) {
    if (left.channel != right.channel) {
      return left.channel < right.channel;
    }
    return left.row < right.row;
  });
  rows.erase(std::remove_if(rows.begin(), rows.end(), [](const FrontendRow& row) {
               return !row.selectable || !row.bounds.valid();
             }),
             rows.end());

  std::lock_guard lock(g_geometry_mutex);
  if (gta4::touch::SameFrontendRows(g_frontend_geometry.rows, rows)) {
    return;
  }
  g_frontend_geometry.rows = std::move(rows);
  ++g_frontend_geometry.generation;
  if (REXCVAR_GET(gta4_touch_trace)) {
    REXLOG_INFO("gta4-touch: layout kind=frontend generation={} rows={}",
                g_frontend_geometry.generation, g_frontend_geometry.rows.size());
  }
}

void PublishRadarGeometry(Rect bounds) noexcept {
  std::lock_guard lock(g_geometry_mutex);
  if (RectNearlyEqual(g_radar_geometry.bounds, bounds)) {
    return;
  }
  g_radar_geometry.bounds = bounds;
  ++g_radar_geometry.generation;
  if (REXCVAR_GET(gta4_touch_trace)) {
    REXLOG_INFO(
        "gta4-touch: layout kind=radar generation={} valid={} bounds={:.7g},{:.7g},{:.7g},{:.7g}",
        g_radar_geometry.generation, bounds.valid(), bounds.left, bounds.top, bounds.right,
        bounds.bottom);
  }
}

Point NormalizedPoint(const AbsolutePointerEvent& event) noexcept {
  if (!std::isfinite(event.output_width) || !std::isfinite(event.output_height) ||
      event.output_width <= 0.0f || event.output_height <= 0.0f) {
    return {.x = std::numeric_limits<float>::quiet_NaN(),
            .y = std::numeric_limits<float>::quiet_NaN()};
  }
  return {.x = event.x / event.output_width, .y = event.y / event.output_height};
}

bool FrontendActive(PPCContext& context, uint8_t* base) {
  PPCContext nested = context;
  nested.r3.u32 = kFrontendChannel;
  __imp__sub_8224EEF8(nested, base);
  return nested.r3.u8 != 0;
}

void SelectFrontendRow(PPCContext& context, uint8_t* base, uint32_t channel, uint32_t row,
                       uint64_t epoch, const AbsolutePointerEvent& event) {
  PPCContext nested = context;
  nested.r3.u32 = channel;
  nested.r4.u32 = row;
  __imp__sub_8224EE98(nested, base);
  if (REXCVAR_GET(gta4_touch_trace)) {
    REXLOG_INFO(
        "gta4-touch: epoch={} event={} pointer={} generation={} owner=frontend op=select "
        "channel={} row={}",
        epoch, event.sequence, event.pointer_id, event.generation, channel, row);
  }
}

gta4::input::MapPanAction AccumulateMapOutput(const MapGestureOutput& output,
                                              const AbsolutePointerEvent& event) {
  if (!output.consumed) {
    return {};
  }
  const gta4::input::MapPanAction action = gta4::input::DirectManipulationMapPan(
      gta4::touch::ScaleMapPan(output.pan_x, event.output_width),
      gta4::touch::ScaleMapPan(output.pan_y, event.output_height));
  g_replay.map_x = std::clamp(g_replay.map_x + action.horizontal, -255, 255);
  g_replay.map_y = std::clamp(g_replay.map_y + action.vertical, -255, 255);
  g_replay.zoom_steps += output.zoom_steps;
  g_replay.accept |= output.waypoint;
  return action;
}

void ProcessMapEvent(const AbsolutePointerEvent& event, uint64_t epoch) {
  MapGestureOutput output;
  const Point point{event.x, event.y};
  switch (event.phase) {
    case AbsolutePointerPhase::kDown:
      output = g_map_gesture.Down(event.pointer_id, event.generation, point, event.output_width,
                                  event.output_height);
      break;
    case AbsolutePointerPhase::kMove:
      output = g_map_gesture.Move(event.pointer_id, event.generation, point);
      break;
    case AbsolutePointerPhase::kUp:
      output = g_map_gesture.Up(event.pointer_id, event.generation, point);
      break;
    case AbsolutePointerPhase::kCancel:
      output = g_map_gesture.Cancel(event.pointer_id, event.generation);
      break;
  }
  const gta4::input::MapPanAction action = AccumulateMapOutput(output, event);
  if (REXCVAR_GET(gta4_touch_trace) &&
      (output.consumed || output.cancelled || output.waypoint || output.zoom_steps)) {
    REXLOG_INFO(
        "gta4-touch: epoch={} event={} pointer={} generation={} owner=map phase={} "
        "pointer-pan={:.7g},{:.7g} action={}/{} accumulated={}/{} zoom-steps={} "
        "waypoint={} cancelled={}",
        epoch, event.sequence, event.pointer_id, event.generation,
        static_cast<uint32_t>(event.phase), output.pan_x, output.pan_y, action.horizontal,
        action.vertical, g_replay.map_x, g_replay.map_y, output.zoom_steps, output.waypoint,
        output.cancelled);
  }
}

void ProcessFrontendEvent(PPCContext& context, uint8_t* base,
                          const AbsolutePointerEvent& event, uint64_t epoch,
                          const FrontendGeometry& geometry) {
  const Point point = NormalizedPoint(event);
  const std::optional<FrontendRow> hit =
      gta4::touch::HitTestFrontendRow(geometry.rows, point);
  switch (event.phase) {
    case AbsolutePointerPhase::kDown:
      if (hit && g_frontend_transaction.Begin(event.pointer_id, event.generation,
                                               geometry.generation, *hit)) {
        SelectFrontendRow(context, base, hit->channel, hit->row, epoch, event);
      }
      break;
    case AbsolutePointerPhase::kMove: {
      const auto move = g_frontend_transaction.Move(
          event.pointer_id, event.generation, geometry.generation, hit);
      if (move.selection_changed) {
        SelectFrontendRow(context, base, move.channel, move.row, epoch, event);
      }
      break;
    }
    case AbsolutePointerPhase::kUp:
      if (g_frontend_transaction.End(event.pointer_id, event.generation,
                                     geometry.generation, hit)) {
        g_replay.accept = true;
        if (REXCVAR_GET(gta4_touch_trace)) {
          REXLOG_INFO(
              "gta4-touch: epoch={} event={} pointer={} generation={} owner=frontend op=accept",
              epoch, event.sequence, event.pointer_id, event.generation);
        }
      }
      break;
    case AbsolutePointerPhase::kCancel:
      g_frontend_transaction.Cancel(event.pointer_id, event.generation);
      break;
  }
}

bool ProcessMinimapEvent(const AbsolutePointerEvent& event, uint64_t epoch,
                         const RadarGeometry& radar) {
  const Point point = NormalizedPoint(event);
  if (event.phase == AbsolutePointerPhase::kDown) {
    // Radar vertices and point are both normalized. Keep tap slop in that
    // coordinate space rather than mixing it with guest-pixel output extents.
    if (!g_minimap_transaction.Begin(event.pointer_id, event.generation, radar.generation, point,
                                     1.0f, 1.0f, radar.bounds)) {
      return false;
    }
  } else if (!g_minimap_transaction.active() ||
             g_minimap_transaction.pointer_id() != event.pointer_id) {
    return false;
  } else if (event.phase == AbsolutePointerPhase::kMove) {
    g_minimap_transaction.Move(event.pointer_id, event.generation, radar.generation, point);
  } else if (event.phase == AbsolutePointerPhase::kUp) {
    if (g_minimap_transaction.End(event.pointer_id, event.generation, radar.generation, point)) {
      // The recovered direct-map bootstrap has two caller-dependent setup
      // paths and is not safe to invoke synthetically. Retail Pause is the
      // proven path; it leaves all subsequent map setup in game code.
      g_replay.pause = true;
      if (REXCVAR_GET(gta4_touch_trace)) {
        REXLOG_INFO(
            "gta4-touch: epoch={} event={} pointer={} generation={} owner=minimap op=pause",
            epoch, event.sequence, event.pointer_id, event.generation);
      }
    }
  } else {
    g_minimap_transaction.Reset();
  }
  return true;
}

void UpdateWeaponGeometry(const gta4::input::TouchContextSnapshot& context, bool admitted) {
  Rect bounds;
  if (admitted && context.weapon_hud_bounds) {
    const auto& hud = *context.weapon_hud_bounds;
    bounds = {hud.left, hud.top, hud.right, hud.bottom};
    if (!bounds.valid() || bounds.left < 0.0f || bounds.top < 0.0f ||
        bounds.right > 1.0f || bounds.bottom > 1.0f) bounds = {};
  }
  if (context.generation != g_weapon_context_generation ||
      !RectNearlyEqual(bounds, g_weapon_geometry.bounds)) {
    g_weapon_geometry.bounds = bounds;
    g_weapon_context_generation = context.generation;
    ++g_weapon_geometry.generation;
  }
}

bool ProcessWeaponHudEvent(const AbsolutePointerEvent& event, uint64_t epoch) {
  const Point point = NormalizedPoint(event);
  const auto& geometry = g_weapon_geometry;
  if (event.phase == AbsolutePointerPhase::kDown) {
    if (!g_weapon_transaction.Begin(event.pointer_id, event.generation,
                                    geometry.generation, point, 1.0f, 1.0f,
                                    geometry.bounds)) {
      // A second finger on the occupied HUD region must not start a camera
      // gesture underneath the first finger's tap transaction.
      return geometry.bounds.contains(point);
    }
  } else if (!g_weapon_transaction.active() ||
             g_weapon_transaction.pointer_id() != event.pointer_id) {
    return false;
  } else if (event.phase == AbsolutePointerPhase::kMove) {
    g_weapon_transaction.Move(event.pointer_id, event.generation, geometry.generation, point);
  } else if (event.phase == AbsolutePointerPhase::kUp) {
    if (g_weapon_transaction.End(event.pointer_id, event.generation, geometry.generation, point)) {
      gta4::input::QueueContextTouchWeaponCycle(epoch, event.pointer_id);
    }
  } else {
    g_weapon_transaction.Reset();
  }
  return true;
}

void ResetTransactions() noexcept {
  g_frontend_transaction.Reset();
  g_minimap_transaction.Reset();
  g_weapon_transaction.Reset();
  g_map_gesture.Reset();
}

void ApplyMapZoom(uint8_t* base, uint64_t epoch) noexcept {
  if (!g_replay.zoom_steps || !g_replay.map_active) {
    return;
  }
  const uint32_t before = LoadU32(base, kMapZoomLevelAddress);
  const int32_t bounded_before = static_cast<int32_t>(std::min(before, kMapZoomMaximum));
  const int32_t after = std::clamp(bounded_before + g_replay.zoom_steps,
                                   static_cast<int32_t>(kMapZoomMinimum),
                                   static_cast<int32_t>(kMapZoomMaximum));
  if (after == bounded_before) {
    return;
  }
  StoreU32(base, kMapZoomLevelAddress, static_cast<uint32_t>(after));
  if (REXCVAR_GET(gta4_touch_trace)) {
    REXLOG_INFO("gta4-touch: epoch={} owner=map op=zoom level={}->{} steps={}", epoch, before,
                after, g_replay.zoom_steps);
  }
}

void FreezeVirtualKeys(const GTA4TouchExtension& extension, uint64_t epoch,
                       bool controls_active) noexcept {
  const uint64_t revision = gta4::input::ContextTouchPresentationRevision();
  VirtualKeySnapshot snapshot;
  snapshot.epoch = epoch;
  if (controls_active && extension.collect_virtual_keys) {
    extension.collect_virtual_keys(epoch, snapshot.down, snapshot.pressed);
  }
  const bool gameplay_admitted = gta4::input::ContextTouchGameplayInputAdmitted();
  if (!gameplay_admitted) snapshot = {};
  std::lock_guard lock(g_extension_mutex);
  // A presentation transition can cancel input while collection is in flight.
  // Recheck under the same lock used by immediate cancellation so an old poll
  // cannot republish held keys after the cancellation has completed.
  const bool current = revision == gta4::input::ContextTouchPresentationRevision();
  const bool active = controls_active && gameplay_admitted && current &&
      !GTA4_TouchTitleInputOwned();
  if (!active) snapshot = {};
  g_virtual_keys = snapshot;
  rex::input::mnk::PublishVirtualControllerCompatibilityKeys(0, snapshot.down, active);
}

void DisableContextControls(const GTA4TouchExtension& extension,
                            PPCContext& context, uint8_t* base,
                            uint64_t epoch) noexcept {
  if (g_context_controls_were_active && extension.on_controls_disabled) {
    extension.on_controls_disabled(context, base, epoch);
  }
  g_context_controls_were_active = false;
}

}  // namespace

void GTA4_RegisterTouchExtension(GTA4TouchExtension extension) noexcept {
  std::lock_guard lock(g_extension_mutex);
  g_extension = extension;
}

void GTA4_TouchConsumePoll(PPCContext& context, uint8_t* base, uint64_t epoch) {
  g_replay = {.epoch = epoch};
  g_poll_epoch.store(epoch, std::memory_order_release);
  const GTA4TouchExtension extension = ReadExtension();
  const uint32_t screen = LoadU32(base, kCurrentScreenAddress);
  const bool frontend_active = FrontendActive(context, base);
  const bool map_active = frontend_active && screen == kMapScreen;
  gta4::input::CaptureTouchContext(context, base, epoch, frontend_active, map_active);
  const uint64_t presentation_revision = gta4::input::ContextTouchPresentationRevision();
  if (g_hud_presentation_revision != presentation_revision) {
    // A complete loading/cutscene transition may occur between input polls.
    // An old HUD Down must never pair with a release in the resumed game.
    g_minimap_transaction.Reset();
    g_weapon_transaction.Reset();
    g_hud_presentation_revision = presentation_revision;
  }
  const auto touch_context = gta4::input::GetTouchContextSnapshot();
  g_replay.input_user = touch_context.input_user;
  g_replay.input_user_known = touch_context.input_user_known;
  const bool pointer_input_active = rex::input::TouchPointerInputActive();
  const bool controls_active = rex::input::TouchControlsActive();
  const bool title_input_owned = GTA4_TouchTitleInputOwned();
  const bool editor_captures = gta4::input::ContextTouchEditorCapturesInput();
  // Native menu rows stay directly touchable when the gameplay overlay is
  // Off, including the native setting needed to turn it back on. Focus and
  // host/title modal ownership still gate every guest pointer transaction.
  if (!pointer_input_active || title_input_owned ||
      (!controls_active && !frontend_active && !editor_captures)) {
    AbsolutePointerEvent discarded;
    while (rex::input::TryDequeueAbsolutePointerEvent(&discarded)) {
    }
    ResetTransactions();
    DisableContextControls(extension, context, base, epoch);
    FreezeVirtualKeys(extension, epoch, false);
    return;
  }

  const bool frontend_navigation_active = frontend_active && !map_active && !editor_captures;
  const bool context_controls_active = editor_captures || (!frontend_active && controls_active);
  if (context_controls_active && extension.begin_poll) {
    extension.begin_poll(context, base, epoch, frontend_active, map_active);
  }
  const bool gameplay_active = !frontend_active && controls_active && !editor_captures &&
      gta4::input::ContextTouchGameplayInputAdmitted();
  g_replay.frontend_active = frontend_navigation_active;
  g_replay.map_active = map_active && !editor_captures;
  g_replay.context_controls_active = context_controls_active;
  const FrontendGeometry frontend =
      frontend_navigation_active ? ReadFrontendGeometry() : FrontendGeometry{};
  const RadarGeometry radar = gameplay_active ? ReadRadarGeometry() : RadarGeometry{};
  UpdateWeaponGeometry(touch_context, gameplay_active &&
      gta4::input::ContextTouchWeaponCycleAdmitted());

  if (!frontend_navigation_active) {
    g_frontend_transaction.Reset();
  }
  if (!map_active || editor_captures) {
    g_map_gesture.Reset();
  }
  if (!gameplay_active) {
    g_minimap_transaction.Reset();
    g_weapon_transaction.Reset();
  }
  if (!context_controls_active) {
    DisableContextControls(extension, context, base, epoch);
  }

  AbsolutePointerEvent event;
  while (rex::input::TryDequeueAbsolutePointerEvent(&event)) {
    if (editor_captures) {
      // The whole gesture belongs to the editor, including unused screen
      // space and the noninteractive fade after Done.
      if (extension.on_pointer_event) extension.on_pointer_event(event, context, base, epoch);
      continue;
    }
    if (map_active) {
      ProcessMapEvent(event, epoch);
      continue;
    }
    if (frontend_navigation_active) {
      ProcessFrontendEvent(context, base, event, epoch, frontend);
      continue;
    }
    if (gameplay_active && ProcessMinimapEvent(event, epoch, radar)) {
      continue;
    }
    if (gameplay_active && ProcessWeaponHudEvent(event, epoch)) {
      continue;
    }
    if (!frontend_active && context_controls_active && extension.on_pointer_event) {
      extension.on_pointer_event(event, context, base, epoch);
    }
  }

  ApplyMapZoom(base, epoch);
  if (context_controls_active) {
    g_context_controls_were_active = true;
  }
  FreezeVirtualKeys(extension, epoch, context_controls_active);
}

void GTA4_TouchObserveControlReplay(PPCContext& context, uint8_t* base, uint32_t control,
                                    uint32_t caller, uint64_t epoch) {
  if (!base || !control || uint64_t{control} + 3412 + sizeof(uint32_t) >
                              uint64_t{UINT32_MAX} + 1) {
    return;
  }

  const bool touch_replay_enabled =
      g_replay.epoch == epoch && g_replay.input_user_known &&
      LoadU32(base, control + 3412) == g_replay.input_user &&
      rex::input::TouchPointerInputActive() && !GTA4_TouchTitleInputOwned() &&
      (g_replay.frontend_active || g_replay.map_active ||
       gta4::input::ContextTouchGameplayInputAdmitted()) &&
      !gta4::input::ContextTouchEditorCapturesInput();
  const bool controller_navigation =
      ReadAction(base, control, Action::kFrontendDown) ||
      ReadAction(base, control, Action::kFrontendUp) ||
      ReadAction(base, control, Action::kFrontendLeft) ||
      ReadAction(base, control, Action::kFrontendRight);
  if (controller_navigation && touch_replay_enabled && g_replay.frontend_active) {
    ResetTransactions();
    // Navigation observed during the same immutable replay wins over a touch
    // selection, including a complete Down+Up transaction drained this poll.
    g_replay.accept = false;
  }

  bool changed = false;
  if (touch_replay_enabled) {
    if (g_replay.map_active) {
      changed |= MergeSignedAction(base, control, Action::kMapX, g_replay.map_x);
      changed |= MergeSignedAction(base, control, Action::kMapY, g_replay.map_y);
    }
    changed |= MergeButton(base, control, Action::kFrontendAccept,
                           g_replay.accept ? kPressed : 0);
    changed |= MergeButton(base, control, Action::kFrontendPause,
                           g_replay.pause && rex::input::TouchControlsActive() ? kPressed : 0);
  }
  if (changed) {
    StoreU32(base, control + kLastInputTimeOffset, LoadU32(base, kGameInputTimeAddress));
  }

  const GTA4TouchExtension extension = ReadExtension();
  if (extension.on_control_replay && touch_replay_enabled &&
      g_replay.context_controls_active) {
    extension.on_control_replay(context, base, control, caller, epoch);
  }

  if (REXCVAR_GET(gta4_touch_trace) &&
      (changed || controller_navigation) && touch_replay_enabled) {
    REXLOG_INFO(
        "gta4-touch: epoch={} owner=replay caller={:08X} control={:08X} "
        "map={}/{} accept={} pause={} changed={} controller-nav={}",
        epoch, caller, control, g_replay.map_x, g_replay.map_y, g_replay.accept, g_replay.pause,
        changed, controller_navigation);
  }
}

uint64_t GTA4_TouchCurrentEpoch() noexcept {
  return g_poll_epoch.load(std::memory_order_acquire);
}

bool GTA4_TouchVirtualKeyDown(uint16_t key) noexcept {
  const uint64_t revision = gta4::input::ContextTouchPresentationRevision();
  if (GTA4_TouchTitleInputOwned() || !rex::input::TouchControlsActive() ||
      !gta4::input::ContextTouchGameplayInputAdmitted()) {
    return false;
  }
  std::lock_guard lock(g_extension_mutex);
  return revision == gta4::input::ContextTouchPresentationRevision() &&
      key < g_virtual_keys.down.size() && g_virtual_keys.down[key] != 0;
}

bool GTA4_TouchVirtualKeyPressed(uint64_t epoch, uint16_t key) noexcept {
  const uint64_t revision = gta4::input::ContextTouchPresentationRevision();
  if (GTA4_TouchTitleInputOwned() || !rex::input::TouchControlsActive() ||
      !gta4::input::ContextTouchGameplayInputAdmitted()) {
    return false;
  }
  std::lock_guard lock(g_extension_mutex);
  return revision == gta4::input::ContextTouchPresentationRevision() &&
         g_virtual_keys.epoch == epoch && key < g_virtual_keys.pressed.size() &&
         g_virtual_keys.pressed[key] != 0;
}

void GTA4_CancelTouchGameplayReplay() noexcept {
  std::lock_guard lock(g_extension_mutex);
  g_virtual_keys = {};
  rex::input::mnk::PublishVirtualControllerCompatibilityKeys(0, {}, false);
}

void GTA4_SetTouchTitleInputOwned(bool owned) noexcept {
  g_title_input_owned.store(owned, std::memory_order_release);
  if (owned) GTA4_CancelTouchGameplayReplay();
}

bool GTA4_TouchTitleInputOwned() noexcept {
  return g_title_input_owned.load(std::memory_order_acquire);
}

void GTA4_TouchObserveHudSubmit(const PPCContext& context, uint8_t* base) noexcept {
  if (!g_row_capture.active || context.lr != kFrontendHudCaller ||
      !g_row_capture.channel_object) {
    return;
  }
  const uint32_t row = context.r23.u32;
  const uint32_t list = context.r28.u32;
  if (row >= kMaximumFrontendRows || list >= kMaximumFrontendLists ||
      !LoadU8(base, g_row_capture.channel_object + kFrontendRowVisibleOffset + row) ||
      !LoadU8(base, g_row_capture.channel_object + kFrontendRowSelectableOffset + row)) {
    return;
  }
  const uint32_t payload_address =
      g_row_capture.channel_object + row * kFrontendRowStride + list * kFrontendListStride +
      kFrontendPayloadPresentOffset;
  if (!LoadU8(base, payload_address)) {
    return;
  }

  const float text_x = static_cast<float>(context.f1.f64);
  const float text_y = static_cast<float>(context.f2.f64);
  const float horizontal_base =
      LoadFloat(base, g_row_capture.channel_object + kFrontendHorizontalBaseOffset);
  float cell_left = horizontal_base;
  for (uint32_t prior = 0; prior < list; ++prior) {
    cell_left += LoadFloat(base, g_row_capture.channel_object + kFrontendColumnOffsetBase +
                                    prior * sizeof(float));
  }
  const float column_width = LoadFloat(
      base, g_row_capture.channel_object + kFrontendColumnOffsetBase + list * sizeof(float));
  const float row_height =
      std::abs(LoadFloat(base, g_row_capture.channel_object + kFrontendRowHeightOffset));
  if (!std::isfinite(text_x) || !std::isfinite(text_y) || !std::isfinite(horizontal_base) ||
      !std::isfinite(cell_left) || !std::isfinite(column_width) ||
      !std::isfinite(row_height) || row_height <= 0.0f) {
    return;
  }

  const float cell_right = cell_left + column_width;
  const float padding = row_height * 0.5f;
  Rect cell{
      .left = std::min({cell_left, cell_right, text_x}) - padding,
      .top = text_y - row_height * 0.5f,
      .right = std::max({cell_left, cell_right, text_x}) + padding,
      .bottom = text_y + row_height * 0.5f,
  };
#if !defined(GTA4_TOUCH_LEGACY_HOST)
  const auto layout = gta4::aspect::CurrentUi(base);
  const auto mapped = layout.transform.Map(gta4::aspect::Rect{cell.left, cell.top, cell.right, cell.bottom});
  cell = {.left = float(mapped.left), .top = float(mapped.top),
          .right = float(mapped.right), .bottom = float(mapped.bottom)};
#endif
  if (!cell.valid()) {
    return;
  }

  auto found = std::find_if(g_row_capture.rows.begin(), g_row_capture.rows.end(),
                            [row](const FrontendRow& candidate) {
                              return candidate.channel == kFrontendChannel &&
                                     candidate.row == row;
                            });
  if (found == g_row_capture.rows.end()) {
    g_row_capture.rows.push_back({.bounds = cell,
                                  .channel = kFrontendChannel,
                                  .row = row,
                                  .selectable = true});
  } else {
    IncludeRect(found->bounds, cell);
  }
}

void GTA4_TouchCaptureFrontendDraw(PPCContext& context, uint8_t* base,
                                   GTA4GuestFunction draw_function) {
#if !defined(GTA4_TOUCH_LEGACY_HOST)
  const gta4::aspect::Scope aspect_scope(gta4::aspect::MenuBodyUi(base));
  const gta4::aspect::FrontendLayoutScope adaptive_columns(context, base);
#endif
  RowCapture previous = std::move(g_row_capture);
  g_row_capture = {};
  if (context.r3.u32 == kFrontendChannel) {
    g_row_capture.active = true;
    g_row_capture.channel_object =
        LoadU32(base, kFrontendChannelTableAddress + context.r3.u32 * sizeof(uint32_t));
  }
  draw_function(context, base);
  if (g_row_capture.active) {
    PublishFrontendGeometry(std::move(g_row_capture.rows));
  }
  g_row_capture = std::move(previous);
}

extern "C" void sub_821BF050(PPCContext& context, uint8_t* base) {
  if (g_radar_capture.active && context.lr == kRadarQuadCaller && context.r4.u32) {
    Rect quad{};
    bool first = true;
    for (uint32_t vertex = 0; vertex < kRadarVertexCount; ++vertex) {
      const uint32_t address = context.r4.u32 + vertex * kRadarVertexStride;
      const float x = LoadFloat(base, address + kRadarVertexXOffset);
      const float y = LoadFloat(base, address + kRadarVertexYOffset);
      if (!std::isfinite(x) || !std::isfinite(y)) {
        first = true;
        break;
      }
      if (first) {
        quad = {.left = x, .top = y, .right = x, .bottom = y};
        first = false;
      } else {
        quad.left = std::min(quad.left, x);
        quad.top = std::min(quad.top, y);
        quad.right = std::max(quad.right, x);
        quad.bottom = std::max(quad.bottom, y);
      }
    }
    if (!first && quad.valid()) {
#if !defined(GTA4_TOUCH_LEGACY_HOST)
      const auto layout = gta4::aspect::CurrentUi(base);
      const auto mapped = layout.transform.Map(gta4::aspect::Rect{quad.left, quad.top, quad.right, quad.bottom});
      quad = {.left = float(mapped.left), .top = float(mapped.top),
              .right = float(mapped.right), .bottom = float(mapped.bottom)};
#endif
      IncludeRect(g_radar_capture.bounds, quad);
      g_radar_capture.has_quad = true;
    }
  }
  __imp__sub_821BF050(context, base);
}

extern "C" void sub_8239C468(PPCContext& context, uint8_t* base) {
  const auto pass = gta4::input::ConsumeTouchRadarPass(base, context.r3.u32);
  RadarCapture previous = g_radar_capture;
  g_radar_capture = {.active = true};
#if !defined(GTA4_TOUCH_LEGACY_HOST)
  // Apply display layout once, to the queued viewport. Its tiles, blips,
  // circles and callback-drawn mask/frame all retain their local coordinates.
  // A native pause-map phase keeps the pre-existing frontend drawing context.
  const gta4::aspect::Scope aspect_scope(pass.gameplay ? gta4::aspect::RadarLocalUi() :
                                         gta4::aspect::CurrentUi(base));
#endif
  __imp__sub_8239C468(context, base);
  const auto bounds = pass.bounds;
  const bool visible = pass.gameplay && g_radar_capture.has_quad &&
      bounds.right > bounds.left && bounds.bottom > bounds.top;
  PublishRadarGeometry(visible ? Rect{.left = float(bounds.left), .top = float(bounds.top),
                                     .right = float(bounds.right), .bottom = float(bounds.bottom)} :
                                Rect{});
  g_radar_capture = previous;
}
