#include "gta4_aspect_hooks.h"
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
gta4::touch::MapGesture g_map_gesture;
ReplaySnapshot g_replay;
bool g_context_controls_were_active = false;
std::atomic<bool> g_title_input_owned{false};

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

void ResetTransactions() noexcept {
  g_frontend_transaction.Reset();
  g_minimap_transaction.Reset();
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
  VirtualKeySnapshot snapshot;
  snapshot.epoch = epoch;
  if (controls_active && extension.collect_virtual_keys) {
    extension.collect_virtual_keys(epoch, snapshot.down, snapshot.pressed);
  }
  std::lock_guard lock(g_extension_mutex);
  if (GTA4_TouchTitleInputOwned()) snapshot = {};
  g_virtual_keys = snapshot;
  rex::input::mnk::PublishVirtualControllerCompatibilityKeys(0, snapshot.down,
      controls_active && !GTA4_TouchTitleInputOwned());
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
  const GTA4TouchExtension extension = ReadExtension();
  const bool controls_active = rex::input::TouchControlsActive();
  if (!controls_active) {
    AbsolutePointerEvent discarded;
    while (rex::input::TryDequeueAbsolutePointerEvent(&discarded)) {
    }
    ResetTransactions();
    DisableContextControls(extension, context, base, epoch);
    FreezeVirtualKeys(extension, epoch, false);
    return;
  }

  const uint32_t screen = LoadU32(base, kCurrentScreenAddress);
  const bool frontend_active = FrontendActive(context, base);
  const bool map_active = frontend_active && screen == kMapScreen;
  const bool frontend_navigation_active = frontend_active && !map_active;
  const bool gameplay_active = !frontend_active;
  const bool title_input_owned = GTA4_TouchTitleInputOwned();
  const bool context_controls_active = !title_input_owned;
  g_replay.frontend_active = frontend_navigation_active;
  g_replay.map_active = map_active;
  g_replay.context_controls_active = context_controls_active;
  const FrontendGeometry frontend =
      frontend_navigation_active ? ReadFrontendGeometry() : FrontendGeometry{};
  const RadarGeometry radar = gameplay_active ? ReadRadarGeometry() : RadarGeometry{};

  if (!frontend_navigation_active) {
    g_frontend_transaction.Reset();
  }
  if (!map_active) {
    g_map_gesture.Reset();
  }
  if (!gameplay_active) {
    g_minimap_transaction.Reset();
  }
  if (title_input_owned) {
    ResetTransactions();
  }
  if (!context_controls_active) {
    DisableContextControls(extension, context, base, epoch);
  }

  if (context_controls_active && extension.begin_poll) {
    extension.begin_poll(context, base, epoch, frontend_active, map_active);
  }
  AbsolutePointerEvent event;
  while (rex::input::TryDequeueAbsolutePointerEvent(&event)) {
    if (title_input_owned) {
      continue;
    }
    if (frontend_active && extension.on_pointer_event &&
        extension.on_pointer_event(event, context, base, epoch)) {
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
    if (gameplay_active && extension.on_pointer_event) {
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
  if (!control) {
    return;
  }

  const bool touch_replay_enabled =
      g_replay.epoch == epoch && !GTA4_TouchTitleInputOwned();
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
                           g_replay.pause ? kPressed : 0);
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

bool GTA4_TouchVirtualKeyDown(uint16_t key) noexcept {
  if (GTA4_TouchTitleInputOwned()) {
    return false;
  }
  std::lock_guard lock(g_extension_mutex);
  return key < g_virtual_keys.down.size() && g_virtual_keys.down[key] != 0;
}

bool GTA4_TouchVirtualKeyPressed(uint64_t epoch, uint16_t key) noexcept {
  if (GTA4_TouchTitleInputOwned()) {
    return false;
  }
  std::lock_guard lock(g_extension_mutex);
  return g_virtual_keys.epoch == epoch && key < g_virtual_keys.pressed.size() &&
         g_virtual_keys.pressed[key] != 0;
}

void GTA4_SetTouchTitleInputOwned(bool owned) noexcept {
  g_title_input_owned.store(owned, std::memory_order_release);
  if (owned) {
    std::lock_guard lock(g_extension_mutex);
    g_virtual_keys = {};
    rex::input::mnk::PublishVirtualControllerCompatibilityKeys(0, {}, false);
  }
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
  const auto layout = gta4::aspect::CurrentUi(base);
  const auto mapped = layout.transform.Map(gta4::aspect::Rect{cell.left, cell.top, cell.right, cell.bottom});
  cell = {.left = float(mapped.left), .top = float(mapped.top),
          .right = float(mapped.right), .bottom = float(mapped.bottom)};
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
  const gta4::aspect::Scope aspect_scope(gta4::aspect::MenuBodyUi(base));
  const gta4::aspect::FrontendLayoutScope adaptive_columns(context, base);
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
      const auto layout = gta4::aspect::CurrentUi(base);
      const auto mapped = layout.transform.Map(gta4::aspect::Rect{quad.left, quad.top, quad.right, quad.bottom});
      quad = {.left = float(mapped.left), .top = float(mapped.top),
              .right = float(mapped.right), .bottom = float(mapped.bottom)};
      IncludeRect(g_radar_capture.bounds, quad);
      g_radar_capture.has_quad = true;
    }
  }
  __imp__sub_821BF050(context, base);
}

extern "C" void sub_8233ABF0(PPCContext& context, uint8_t* base) {
  const gta4::aspect::Scope aspect_scope(gta4::aspect::UiRole::kRadar);
  RadarCapture previous = g_radar_capture;
  g_radar_capture = {.active = true};
  __imp__sub_8233ABF0(context, base);
  PublishRadarGeometry(g_radar_capture.has_quad ? g_radar_capture.bounds : Rect{});
  g_radar_capture = previous;
}
