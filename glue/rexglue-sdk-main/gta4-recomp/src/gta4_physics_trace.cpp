#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <cmath>
#include <cstdint>
#include <limits>

#include <rex/cvar.h>
#include <rex/diagnostics/policy.h>
#include <rex/logging.h>

#include "gta4_init.h"

REXCVAR_DEFINE_BOOL(gta4_trace_physics, true, "GTA IV/Diagnostics",
                    "Trace CPU physics, static XBN collision publication, and contact generation");
REXCVAR_DEFINE_INT32(gta4_trace_physics_tick_interval, 30, "GTA IV/Diagnostics",
                     "Physics ticks between aggregate trace lines")
    .range(1, 600);

namespace {

// Verified against generated PPC by tools/verify_physics_trace_sites.py.
constexpr uint32_t kBoundsStoreGlobal = 0x830960C0;
constexpr uint32_t kStreamingEntriesGlobal = 0x83032744;
constexpr uint32_t kPhysicsSimulatorGlobal = 0x82FF5364;
constexpr uint32_t kStreamingEntryStride = 24;
constexpr uint32_t kStreamingStateWordOffset = 8;
constexpr uint32_t kStreamingStateShift = 30;
constexpr uint32_t kStreamingStateMask = 3;
constexpr uint32_t kBoundsStoreCapacity = 500;

constexpr uint32_t kStoreEntriesOffset = 0;
constexpr uint32_t kStoreFlagsOffset = 4;
constexpr uint32_t kStoreStrideOffset = 12;
constexpr uint32_t kBoundsAssetOffset = 0;
constexpr uint32_t kBoundsMinimumXOffset = 16;
constexpr uint32_t kBoundsMinimumYOffset = 20;
constexpr uint32_t kBoundsMinimumZOffset = 24;
constexpr uint32_t kBoundsMaximumXOffset = 32;
constexpr uint32_t kBoundsMaximumYOffset = 36;
constexpr uint32_t kBoundsMaximumZOffset = 40;
constexpr uint32_t kBoundsBodyOffset = 48;
constexpr uint32_t kBoundTypeOffset = 4;

constexpr uint32_t kSimulatorLevelOffset = 4;
constexpr uint32_t kQueuedAddBodiesOffset = 168;
constexpr uint32_t kQueuedAddBodyCountOffset = 172;
constexpr uint32_t kQueuedAddBodyCapacityOffset = 174;
constexpr uint32_t kQueuedAddIdsOffset = 176;
constexpr uint32_t kQueuedAddIdCountOffset = 180;
constexpr uint32_t kQueuedAddIdCapacityOffset = 182;
constexpr uint32_t kQueuedRemoveBodiesOffset = 184;
constexpr uint32_t kQueuedRemoveCountOffset = 188;
constexpr uint32_t kQueuedRemoveCapacityOffset = 190;
constexpr uint32_t kBroadphaseStateOffset = 4;
constexpr uint32_t kBroadphaseBodyCountOffset = 16;
constexpr uint32_t kBroadphaseRecordsOffset = 8;
constexpr uint32_t kBroadphaseIdMapOffset = 12;
constexpr uint32_t kBroadphaseCountOffset = 24;
constexpr uint32_t kBroadphaseRecordStride = 48;
constexpr uint32_t kBroadphaseRecordMaximumOffset = 16;
constexpr uint32_t kBroadphaseRecordIdOffset = 32;
constexpr uint32_t kBroadphaseIdMapStride = 2;
constexpr uint32_t kAabbStride = 16;
constexpr uint32_t kIdStride = 4;
constexpr uint32_t kLevelBroadphaseOffset = 156;
constexpr uint32_t kLevelBroadphaseActiveMapOffset = 12;
constexpr uint32_t kLevelBroadphaseMaximumIdOffset = 16;
constexpr uint32_t kLevelBroadphaseActiveCountOffset = 20;
constexpr uint32_t kAxisSweepVtable = 0x820A29C4;
constexpr uint32_t kSpatialHashVtable = 0x820A2A14;
constexpr uint32_t kNxNVtable = 0x820A2A64;
constexpr uint32_t kLevelBroadphaseVtable = 0x820A296C;
constexpr uint32_t kAxisSweep3Vtable = 0x820A6E94;
constexpr uint32_t kAxisSweep3WorldMinimumOffset = 16;
constexpr uint32_t kAxisSweep3QuantizationOffset = 48;
constexpr uint32_t kAxisSweep3HandleCountOffset = 64;
constexpr uint32_t kAxisSweep3MaximumHandleOffset = 66;
constexpr uint32_t kAxisSweep3HandlesOffset = 68;
constexpr uint32_t kAxisSweep3EndpointArraysOffset = 72;
constexpr uint32_t kAxisSweep3FilterOwnerOffset = 84;
constexpr uint32_t kAxisSweep3HandleStride = 12;
constexpr uint32_t kAxisSweep3EndpointStride = 4;
constexpr uint32_t kAxisSweep3HandleMaximumIndexOffset = 6;
constexpr uint32_t kAxisSweep3FilterFlagsOffset = 52;
constexpr uint32_t kAxisSweep3FilterTypesOffset = 112;
constexpr uint32_t kAxisSweep3FilterTypeStride = 8;
constexpr uint32_t kAxisSweep3FilterTypeWordOffset = 4;
constexpr uint32_t kInvalidLevelId = 0xFFFF;
constexpr uint32_t kNoXbnIndex = std::numeric_limits<uint32_t>::max();

std::atomic<uint64_t> g_event_sequence{0};
std::atomic<uint64_t> g_tick_count{0};
std::atomic<uint64_t> g_stream_state_calls{0};
std::atomic<uint64_t> g_stream_complete_calls{0};
std::atomic<uint64_t> g_xbn_publish_calls{0};
std::atomic<uint64_t> g_xbn_unpublish_calls{0};
std::atomic<uint64_t> g_xbn_expand_calls{0};
std::atomic<uint64_t> g_body_create_calls{0};
std::atomic<uint64_t> g_body_remove_calls{0};
std::atomic<uint64_t> g_level_assign_calls{0};
std::atomic<uint64_t> g_immediate_insert_calls{0};
std::atomic<uint64_t> g_deferred_insert_calls{0};
std::atomic<uint64_t> g_broadphase_scan_calls{0};
std::atomic<uint64_t> g_broadphase_update_calls{0};
std::atomic<uint64_t> g_contact_pair_calls{0};
std::atomic<uint64_t> g_geometry_test_calls{0};
std::atomic<uint64_t> g_geometry_test_hits{0};
std::atomic<uint64_t> g_axis_sweep_update_calls{0};
std::atomic<uint64_t> g_axis_sweep_filter_calls{0};
std::atomic<uint64_t> g_axis_sweep_pair_add_calls{0};

struct BodyTraceRecord {
  uint32_t body = 0;
  uint32_t level = 0;
  uint32_t xbn_index = kNoXbnIndex;
  uint32_t category = 0;
  uint32_t options = 0;
  std::array<float, 3> minimum{};
  std::array<float, 3> maximum{};
  std::array<float, 3> previous_minimum{};
  std::array<float, 3> previous_maximum{};
  uint64_t aabb_updates = 0;
  bool active = false;
  bool has_aabb = false;
};

std::array<BodyTraceRecord, 1u << 16> g_body_records{};
uint32_t g_last_broadphase = 0;

thread_local uint32_t g_current_xbn_index = kNoXbnIndex;

bool TraceEnabled() {
  return rex::diagnostics::IsEnabled(rex::diagnostics::Category::kPhysics) &&
         REXCVAR_GET(gta4_trace_physics);
}

uint64_t NextEvent() {
  return g_event_sequence.fetch_add(1, std::memory_order_relaxed) + 1;
}

bool ShouldSample(uint64_t call, uint64_t initial_calls, uint64_t interval) {
  return call <= initial_calls || (interval && call % interval == 0);
}

uint32_t LoadU32(uint8_t* base, uint32_t address) {
  return REX_LOAD_U32(address);
}

uint16_t LoadU16(uint8_t* base, uint32_t address) {
  return REX_LOAD_U16(address);
}

uint8_t LoadU8(uint8_t* base, uint32_t address) {
  return REX_LOAD_U8(address);
}

float LoadFloat(uint8_t* base, uint32_t address) {
  return std::bit_cast<float>(LoadU32(base, address));
}

bool TryGuestAddress(uint32_t address, uint32_t offset, uint32_t& result) {
  const uint64_t candidate = static_cast<uint64_t>(address) + offset;
  if (candidate > std::numeric_limits<uint32_t>::max()) {
    return false;
  }
  result = static_cast<uint32_t>(candidate);
  return true;
}

bool TryGuestArrayAddress(uint32_t address, uint32_t index, uint32_t stride,
                          uint32_t& result) {
  const uint64_t offset = static_cast<uint64_t>(index) * stride;
  if (offset > std::numeric_limits<uint32_t>::max()) {
    return false;
  }
  return TryGuestAddress(address, static_cast<uint32_t>(offset), result);
}

uint32_t LoadU32At(uint8_t* base, uint32_t address, uint32_t offset) {
  uint32_t field = 0;
  return TryGuestAddress(address, offset, field) ? LoadU32(base, field) : 0;
}

uint16_t LoadU16At(uint8_t* base, uint32_t address, uint32_t offset) {
  uint32_t field = 0;
  return TryGuestAddress(address, offset, field) ? LoadU16(base, field) : 0;
}

uint8_t LoadU8At(uint8_t* base, uint32_t address, uint32_t offset) {
  uint32_t field = 0;
  return TryGuestAddress(address, offset, field) ? LoadU8(base, field) : 0;
}

float LoadFloatAt(uint8_t* base, uint32_t address, uint32_t offset) {
  uint32_t field = 0;
  return TryGuestAddress(address, offset, field) ? LoadFloat(base, field)
                                                 : std::numeric_limits<float>::quiet_NaN();
}

std::array<float, 3> ReadVector3(uint8_t* base, uint32_t address) {
  return {LoadFloatAt(base, address, 0), LoadFloatAt(base, address, 4),
          LoadFloatAt(base, address, 8)};
}

bool AabbFinite(const std::array<float, 3>& minimum,
                const std::array<float, 3>& maximum) {
  return std::isfinite(minimum[0]) && std::isfinite(minimum[1]) &&
         std::isfinite(minimum[2]) && std::isfinite(maximum[0]) &&
         std::isfinite(maximum[1]) && std::isfinite(maximum[2]);
}

bool AabbOrdered(const std::array<float, 3>& minimum,
                 const std::array<float, 3>& maximum) {
  return AabbFinite(minimum, maximum) && minimum[0] <= maximum[0] &&
         minimum[1] <= maximum[1] && minimum[2] <= maximum[2];
}

bool AabbCollapsed(const std::array<float, 3>& minimum,
                   const std::array<float, 3>& maximum) {
  return AabbOrdered(minimum, maximum) && minimum[0] == maximum[0] &&
         minimum[1] == maximum[1] && minimum[2] == maximum[2];
}

bool AabbOverlaps(const std::array<float, 3>& first_minimum,
                  const std::array<float, 3>& first_maximum,
                  const std::array<float, 3>& second_minimum,
                  const std::array<float, 3>& second_maximum) {
  return AabbOrdered(first_minimum, first_maximum) &&
         AabbOrdered(second_minimum, second_maximum) &&
         first_minimum[0] <= second_maximum[0] &&
         first_maximum[0] >= second_minimum[0] &&
         first_minimum[1] <= second_maximum[1] &&
         first_maximum[1] >= second_minimum[1] &&
         first_minimum[2] <= second_maximum[2] &&
         first_maximum[2] >= second_minimum[2];
}

const char* BroadphaseClassName(uint32_t vtable) {
  switch (vtable) {
    case kLevelBroadphaseVtable:
      return "phLevelBroadPhase_rage";
    case kAxisSweepVtable:
      return "btAxisSweep1_rage";
    case kSpatialHashVtable:
      return "btSpatialHash_rage";
    case kNxNVtable:
      return "btNxN_rage";
    case kAxisSweep3Vtable:
      return "btAxisSweep3_rage";
    default:
      return "unknown";
  }
}

struct BroadphaseMembership {
  bool present = false;
  uint16_t slot = 0;
  uint16_t stored_id = kInvalidLevelId;
};

struct LevelBroadphaseMembership {
  bool present = false;
  uint16_t active_value = 0;
  uint16_t active_count = 0;
  uint32_t maximum_id = 0;
};

LevelBroadphaseMembership ReadLevelBroadphaseMembership(
    uint8_t* base, uint32_t broadphase, uint16_t id) {
  LevelBroadphaseMembership membership;
  if (!base || !broadphase) {
    return membership;
  }
  const uint32_t active_map =
      LoadU32At(base, broadphase, kLevelBroadphaseActiveMapOffset);
  membership.active_count =
      LoadU16At(base, broadphase, kLevelBroadphaseActiveCountOffset);
  membership.maximum_id =
      LoadU32At(base, broadphase, kLevelBroadphaseMaximumIdOffset);
  uint32_t entry = 0;
  if (!active_map ||
      !TryGuestArrayAddress(active_map, id, sizeof(uint16_t), entry)) {
    return membership;
  }
  membership.active_value = LoadU16(base, entry);
  membership.present = membership.active_value != 0;
  return membership;
}

BroadphaseMembership ReadBroadphaseMembership(uint8_t* base,
                                              uint32_t broadphase,
                                              uint16_t id) {
  BroadphaseMembership membership;
  if (!base || !broadphase) {
    return membership;
  }
  const uint32_t id_map = LoadU32At(base, broadphase, kBroadphaseIdMapOffset);
  const uint32_t records = LoadU32At(base, broadphase, kBroadphaseRecordsOffset);
  const uint16_t count = LoadU16At(base, broadphase, kBroadphaseCountOffset);
  uint32_t map_entry = 0;
  if (!id_map || !records ||
      !TryGuestArrayAddress(id_map, id, kBroadphaseIdMapStride, map_entry)) {
    return membership;
  }
  membership.slot = LoadU16(base, map_entry);
  uint32_t record = 0;
  if (membership.slot >= count ||
      !TryGuestArrayAddress(records, membership.slot,
                            kBroadphaseRecordStride, record)) {
    return membership;
  }
  membership.stored_id =
      LoadU16At(base, record, kBroadphaseRecordIdOffset);
  membership.present = membership.stored_id == id;
  return membership;
}

struct AxisSweepEndpointState {
  uint16_t minimum_index = 0;
  uint16_t maximum_index = 0;
  uint16_t minimum_position = 0;
  uint16_t maximum_position = 0;
  uint16_t minimum_owner = kInvalidLevelId;
  uint16_t maximum_owner = kInvalidLevelId;
  bool address_valid = false;
};

struct AxisSweepHandleState {
  uint16_t handle_count = 0;
  uint16_t maximum_handle = 0;
  std::array<AxisSweepEndpointState, 3> axes{};
};

AxisSweepHandleState ReadAxisSweepHandleState(uint8_t* base,
                                              uint32_t broadphase,
                                              uint16_t id) {
  constexpr std::array<uint32_t, 3> kMinimumIndexOffsets = {0, 2, 4};
  constexpr std::array<uint32_t, 3> kMaximumIndexOffsets = {6, 8, 10};
  constexpr std::array<uint32_t, 3> kEndpointPointerOffsets = {72, 76, 80};

  AxisSweepHandleState state;
  if (!base || !broadphase) {
    return state;
  }
  state.handle_count =
      LoadU16At(base, broadphase, kAxisSweep3HandleCountOffset);
  state.maximum_handle =
      LoadU16At(base, broadphase, kAxisSweep3MaximumHandleOffset);
  const uint32_t handles =
      LoadU32At(base, broadphase, kAxisSweep3HandlesOffset);
  uint32_t handle = 0;
  if (!handles ||
      !TryGuestArrayAddress(handles, id, kAxisSweep3HandleStride, handle)) {
    return state;
  }
  for (uint32_t axis = 0; axis < state.axes.size(); ++axis) {
    AxisSweepEndpointState& endpoint = state.axes[axis];
    endpoint.minimum_index =
        LoadU16At(base, handle, kMinimumIndexOffsets[axis]);
    endpoint.maximum_index =
        LoadU16At(base, handle, kMaximumIndexOffsets[axis]);
    const uint32_t endpoint_array =
        LoadU32At(base, broadphase, kEndpointPointerOffsets[axis]);
    uint32_t minimum_endpoint = 0;
    uint32_t maximum_endpoint = 0;
    endpoint.address_valid =
        endpoint_array &&
        TryGuestArrayAddress(endpoint_array, endpoint.minimum_index,
                             kAxisSweep3EndpointStride, minimum_endpoint) &&
        TryGuestArrayAddress(endpoint_array, endpoint.maximum_index,
                             kAxisSweep3EndpointStride, maximum_endpoint);
    if (!endpoint.address_valid) {
      continue;
    }
    endpoint.minimum_position = LoadU16At(base, minimum_endpoint, 0);
    endpoint.minimum_owner = LoadU16At(base, minimum_endpoint, 2);
    endpoint.maximum_position = LoadU16At(base, maximum_endpoint, 0);
    endpoint.maximum_owner = LoadU16At(base, maximum_endpoint, 2);
  }
  return state;
}

struct AxisSweepFilterState {
  uint32_t owner = 0;
  uint32_t types = 0;
  uint32_t flags = 0;
  uint32_t type_word = 0;
  uint8_t flag_byte = 0;
};

AxisSweepFilterState ReadAxisSweepFilterState(uint8_t* base,
                                              uint32_t broadphase,
                                              uint16_t id) {
  AxisSweepFilterState state;
  if (!base || !broadphase) {
    return state;
  }
  state.owner = LoadU32At(base, broadphase, kAxisSweep3FilterOwnerOffset);
  state.types =
      state.owner ? LoadU32At(base, state.owner, kAxisSweep3FilterTypesOffset)
                  : 0;
  state.flags =
      state.owner ? LoadU32At(base, state.owner, kAxisSweep3FilterFlagsOffset)
                  : 0;
  uint32_t type_entry = 0;
  if (state.types &&
      TryGuestArrayAddress(state.types, id, kAxisSweep3FilterTypeStride,
                           type_entry)) {
    state.type_word =
        LoadU32At(base, type_entry, kAxisSweep3FilterTypeWordOffset);
  }
  uint32_t flag_entry = 0;
  if (state.flags && TryGuestArrayAddress(state.flags, id, 1, flag_entry)) {
    state.flag_byte = LoadU8(base, flag_entry);
  }
  return state;
}

void StoreBodyAabb(uint16_t id, uint32_t body,
                   const std::array<float, 3>& minimum,
                   const std::array<float, 3>& maximum) {
  BodyTraceRecord& record = g_body_records[id];
  if (record.has_aabb) {
    record.previous_minimum = record.minimum;
    record.previous_maximum = record.maximum;
  } else {
    record.previous_minimum = minimum;
    record.previous_maximum = maximum;
  }
  if (body) {
    record.body = body;
  }
  record.minimum = minimum;
  record.maximum = maximum;
  record.has_aabb = true;
  ++record.aabb_updates;
}

void LogDynamicStaticOverlaps(uint16_t dynamic_id) {
  const BodyTraceRecord& dynamic = g_body_records[dynamic_id];
  if (!dynamic.active || dynamic.xbn_index != kNoXbnIndex ||
      !dynamic.has_aabb || dynamic.category != 1) {
    return;
  }
  std::array<float, 3> swept_minimum{};
  std::array<float, 3> swept_maximum{};
  for (uint32_t axis = 0; axis < swept_minimum.size(); ++axis) {
    swept_minimum[axis] =
        std::min(dynamic.previous_minimum[axis], dynamic.minimum[axis]);
    swept_maximum[axis] =
        std::max(dynamic.previous_maximum[axis], dynamic.maximum[axis]);
  }
  uint32_t overlap_count = 0;
  std::array<uint16_t, 8> first_overlaps{};
  for (uint32_t candidate = 0; candidate < g_body_records.size();
       ++candidate) {
    const BodyTraceRecord& static_record = g_body_records[candidate];
    if (!static_record.active ||
        static_record.xbn_index == kNoXbnIndex ||
        !static_record.has_aabb ||
        !AabbOverlaps(swept_minimum, swept_maximum, static_record.minimum,
                      static_record.maximum)) {
      continue;
    }
    if (overlap_count < first_overlaps.size()) {
      first_overlaps[overlap_count] = static_cast<uint16_t>(candidate);
    }
    ++overlap_count;
  }
  if (overlap_count || !AabbOrdered(swept_minimum, swept_maximum)) {
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=dynamic-static-overlap "
        "id={:04X} body={:08X} category={} options={:08X} updates={} "
        "swept=({:.6f},{:.6f},{:.6f})..({:.6f},{:.6f},{:.6f}) "
        "finite={} ordered={} overlaps={} first={:04X},{:04X},{:04X},"
        "{:04X},{:04X},{:04X},{:04X},{:04X}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), dynamic_id,
        dynamic.body, dynamic.category, dynamic.options, dynamic.aabb_updates,
        swept_minimum[0], swept_minimum[1], swept_minimum[2],
        swept_maximum[0], swept_maximum[1], swept_maximum[2],
        AabbFinite(swept_minimum, swept_maximum),
        AabbOrdered(swept_minimum, swept_maximum), overlap_count,
        first_overlaps[0], first_overlaps[1], first_overlaps[2],
        first_overlaps[3], first_overlaps[4], first_overlaps[5],
        first_overlaps[6], first_overlaps[7]);
  }
}

void LogAxisSweepStaticQuantization(uint8_t* base, uint32_t broadphase,
                                    uint32_t ids, uint16_t count,
                                    const char* source) {
  uint32_t world_minimum_address = 0;
  uint32_t quantization_address = 0;
  const bool header_valid =
      TryGuestAddress(broadphase, kAxisSweep3WorldMinimumOffset,
                      world_minimum_address) &&
      TryGuestAddress(broadphase, kAxisSweep3QuantizationOffset,
                      quantization_address);
  const auto world_minimum = header_valid
                                 ? ReadVector3(base, world_minimum_address)
                                 : std::array<float, 3>{};
  const auto quantization = header_valid
                                ? ReadVector3(base, quantization_address)
                                : std::array<float, 3>{};
  uint32_t static_count = 0;
  uint32_t invalid_count = 0;
  for (uint32_t index = 0; index < count; ++index) {
    uint32_t id_address = 0;
    if (!TryGuestArrayAddress(ids, index, kIdStride, id_address)) {
      ++invalid_count;
      continue;
    }
    const uint16_t id = LoadU32(base, id_address) & kInvalidLevelId;
    const BodyTraceRecord& record = g_body_records[id];
    if (!record.active || record.xbn_index == kNoXbnIndex) {
      continue;
    }
    ++static_count;
    const AxisSweepHandleState state =
        ReadAxisSweepHandleState(base, broadphase, id);
    bool addresses_valid = true;
    bool indices_ordered = true;
    bool positions_ordered = true;
    bool owners_match = true;
    for (const AxisSweepEndpointState& axis : state.axes) {
      addresses_valid = addresses_valid && axis.address_valid;
      indices_ordered =
          indices_ordered && axis.minimum_index < axis.maximum_index;
      positions_ordered =
          positions_ordered && axis.minimum_position <= axis.maximum_position;
      owners_match = owners_match && axis.minimum_owner == id &&
                     axis.maximum_owner == id;
    }
    const bool valid = addresses_valid && indices_ordered &&
                       positions_ordered && owners_match;
    invalid_count += valid ? 0 : 1;
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=axis-sweep3-static-quantized "
        "source={} object={:08X} id={:04X} body={:08X} xbn={} "
        "handleCount={} maximumHandle={} headerValid={} "
        "worldMinimum=({:.6f},{:.6f},{:.6f}) "
        "quantization=({:.6f},{:.6f},{:.6f}) "
        "x=({},{},{},{},{:04X},{:04X}) "
        "y=({},{},{},{},{:04X},{:04X}) "
        "z=({},{},{},{},{:04X},{:04X}) "
        "addressValid={} indexOrdered={} positionOrdered={} ownersMatch={} "
        "valid={}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), source,
        broadphase, id, record.body, record.xbn_index, state.handle_count,
        state.maximum_handle, header_valid, world_minimum[0], world_minimum[1],
        world_minimum[2], quantization[0], quantization[1], quantization[2],
        state.axes[0].minimum_index, state.axes[0].maximum_index,
        state.axes[0].minimum_position, state.axes[0].maximum_position,
        state.axes[0].minimum_owner, state.axes[0].maximum_owner,
        state.axes[1].minimum_index, state.axes[1].maximum_index,
        state.axes[1].minimum_position, state.axes[1].maximum_position,
        state.axes[1].minimum_owner, state.axes[1].maximum_owner,
        state.axes[2].minimum_index, state.axes[2].maximum_index,
        state.axes[2].minimum_position, state.axes[2].maximum_position,
        state.axes[2].minimum_owner, state.axes[2].maximum_owner,
        addresses_valid, indices_ordered, positions_ordered, owners_match,
        valid);
  }
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=axis-sweep3-static-quantized-batch "
      "source={} object={:08X} requested={} static={} invalid={}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed), source,
      broadphase, count, static_count, invalid_count);
}

uint32_t LogAxisSweepDynamicStaticDetails(uint8_t* base,
                                          uint32_t broadphase,
                                          uint16_t dynamic_id) {
  const BodyTraceRecord& dynamic = g_body_records[dynamic_id];
  if (!dynamic.active || dynamic.xbn_index != kNoXbnIndex ||
      !dynamic.has_aabb || dynamic.category != 1) {
    return 0;
  }
  std::array<float, 3> swept_minimum{};
  std::array<float, 3> swept_maximum{};
  for (uint32_t axis = 0; axis < swept_minimum.size(); ++axis) {
    swept_minimum[axis] =
        std::min(dynamic.previous_minimum[axis], dynamic.minimum[axis]);
    swept_maximum[axis] =
        std::max(dynamic.previous_maximum[axis], dynamic.maximum[axis]);
  }
  const AxisSweepHandleState dynamic_handle =
      ReadAxisSweepHandleState(base, broadphase, dynamic_id);
  const AxisSweepFilterState dynamic_filter =
      ReadAxisSweepFilterState(base, broadphase, dynamic_id);
  uint32_t overlap_count = 0;
  for (uint32_t candidate = 0; candidate < g_body_records.size();
       ++candidate) {
    const BodyTraceRecord& static_record = g_body_records[candidate];
    if (!static_record.active || static_record.xbn_index == kNoXbnIndex ||
        !static_record.has_aabb ||
        !AabbOverlaps(swept_minimum, swept_maximum, static_record.minimum,
                      static_record.maximum)) {
      continue;
    }
    ++overlap_count;
    const uint16_t static_id = static_cast<uint16_t>(candidate);
    const AxisSweepHandleState static_handle =
        ReadAxisSweepHandleState(base, broadphase, static_id);
    const AxisSweepFilterState static_filter =
        ReadAxisSweepFilterState(base, broadphase, static_id);
    const bool current_overlap =
        AabbOverlaps(dynamic.minimum, dynamic.maximum, static_record.minimum,
                     static_record.maximum);
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=axis-sweep3-dynamic-static-detail "
        "object={:08X} dynamic={:04X} dynamicBody={:08X} "
        "static={:04X} staticBody={:08X} xbn={} currentOverlap={} "
        "dynamicAabb=({:.6f},{:.6f},{:.6f})..({:.6f},{:.6f},{:.6f}) "
        "sweptAabb=({:.6f},{:.6f},{:.6f})..({:.6f},{:.6f},{:.6f}) "
        "staticAabb=({:.6f},{:.6f},{:.6f})..({:.6f},{:.6f},{:.6f}) "
        "dynamicX=({},{},{},{}) dynamicY=({},{},{},{}) "
        "dynamicZ=({},{},{},{}) staticX=({},{},{},{}) "
        "staticY=({},{},{},{}) staticZ=({},{},{},{}) "
        "dynamicTypeWord={:08X} dynamicFlag={:02X} "
        "staticTypeWord={:08X} staticFlag={:02X}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), broadphase,
        dynamic_id, dynamic.body, static_id, static_record.body,
        static_record.xbn_index, current_overlap, dynamic.minimum[0],
        dynamic.minimum[1], dynamic.minimum[2], dynamic.maximum[0],
        dynamic.maximum[1], dynamic.maximum[2], swept_minimum[0],
        swept_minimum[1], swept_minimum[2], swept_maximum[0],
        swept_maximum[1], swept_maximum[2], static_record.minimum[0],
        static_record.minimum[1], static_record.minimum[2],
        static_record.maximum[0], static_record.maximum[1],
        static_record.maximum[2], dynamic_handle.axes[0].minimum_index,
        dynamic_handle.axes[0].maximum_index,
        dynamic_handle.axes[0].minimum_position,
        dynamic_handle.axes[0].maximum_position,
        dynamic_handle.axes[1].minimum_index,
        dynamic_handle.axes[1].maximum_index,
        dynamic_handle.axes[1].minimum_position,
        dynamic_handle.axes[1].maximum_position,
        dynamic_handle.axes[2].minimum_index,
        dynamic_handle.axes[2].maximum_index,
        dynamic_handle.axes[2].minimum_position,
        dynamic_handle.axes[2].maximum_position,
        static_handle.axes[0].minimum_index,
        static_handle.axes[0].maximum_index,
        static_handle.axes[0].minimum_position,
        static_handle.axes[0].maximum_position,
        static_handle.axes[1].minimum_index,
        static_handle.axes[1].maximum_index,
        static_handle.axes[1].minimum_position,
        static_handle.axes[1].maximum_position,
        static_handle.axes[2].minimum_index,
        static_handle.axes[2].maximum_index,
        static_handle.axes[2].minimum_position,
        static_handle.axes[2].maximum_position, dynamic_filter.type_word,
        dynamic_filter.flag_byte, static_filter.type_word,
        static_filter.flag_byte);
  }
  return overlap_count;
}

const char* StreamingStateName(uint32_t state) {
  switch (state) {
    case 0:
      return "unloaded";
    case 1:
      return "loaded";
    case 2:
      return "requested";
    case 3:
      return "loading";
    default:
      return "invalid";
  }
}

struct BoundsStoreSnapshot {
  uint32_t store = 0;
  uint32_t entries = 0;
  uint32_t flags = 0;
  uint32_t stride = 0;
  uint32_t entry = 0;
  uint32_t asset = 0;
  uint32_t body = 0;
  uint8_t entry_flags = 0;
  std::array<float, 3> minimum{};
  std::array<float, 3> maximum{};
  bool finite = false;
  bool ordered = false;
};

BoundsStoreSnapshot ReadBoundsStore(uint8_t* base, uint32_t index) {
  BoundsStoreSnapshot snapshot;
  if (!base) {
    return snapshot;
  }
  snapshot.store = LoadU32(base, kBoundsStoreGlobal);
  if (!snapshot.store) {
    return snapshot;
  }
  snapshot.entries = LoadU32At(base, snapshot.store, kStoreEntriesOffset);
  snapshot.flags = LoadU32At(base, snapshot.store, kStoreFlagsOffset);
  snapshot.stride = LoadU32At(base, snapshot.store, kStoreStrideOffset);
  if (index >= kBoundsStoreCapacity || !snapshot.entries || !snapshot.flags || !snapshot.stride) {
    return snapshot;
  }

  uint32_t flag_address = 0;
  if (!TryGuestAddress(snapshot.flags, index, flag_address)) {
    return snapshot;
  }
  snapshot.entry_flags = LoadU8(base, flag_address);
  if (snapshot.entry_flags & 0x80) {
    return snapshot;
  }

  const uint64_t entry =
      static_cast<uint64_t>(snapshot.entries) + static_cast<uint64_t>(snapshot.stride) * index;
  if (entry > std::numeric_limits<uint32_t>::max()) {
    return snapshot;
  }
  snapshot.entry = static_cast<uint32_t>(entry);
  snapshot.asset = LoadU32At(base, snapshot.entry, kBoundsAssetOffset);
  snapshot.body = LoadU32At(base, snapshot.entry, kBoundsBodyOffset);
  snapshot.minimum = {
      LoadFloatAt(base, snapshot.entry, kBoundsMinimumXOffset),
      LoadFloatAt(base, snapshot.entry, kBoundsMinimumYOffset),
      LoadFloatAt(base, snapshot.entry, kBoundsMinimumZOffset),
  };
  snapshot.maximum = {
      LoadFloatAt(base, snapshot.entry, kBoundsMaximumXOffset),
      LoadFloatAt(base, snapshot.entry, kBoundsMaximumYOffset),
      LoadFloatAt(base, snapshot.entry, kBoundsMaximumZOffset),
  };
  snapshot.finite = std::isfinite(snapshot.minimum[0]) && std::isfinite(snapshot.minimum[1]) &&
                    std::isfinite(snapshot.minimum[2]) && std::isfinite(snapshot.maximum[0]) &&
                    std::isfinite(snapshot.maximum[1]) && std::isfinite(snapshot.maximum[2]);
  snapshot.ordered = snapshot.finite && snapshot.minimum[0] <= snapshot.maximum[0] &&
                     snapshot.minimum[1] <= snapshot.maximum[1] &&
                     snapshot.minimum[2] <= snapshot.maximum[2];
  return snapshot;
}

struct SimulatorSnapshot {
  uint32_t simulator = 0;
  uint32_t level = 0;
  uint32_t add_bodies = 0;
  uint32_t add_ids = 0;
  uint32_t remove_bodies = 0;
  uint16_t add_body_count = 0;
  uint16_t add_body_capacity = 0;
  uint16_t add_id_count = 0;
  uint16_t add_id_capacity = 0;
  uint16_t remove_count = 0;
  uint16_t remove_capacity = 0;
};

SimulatorSnapshot ReadSimulator(uint8_t* base, uint32_t simulator) {
  SimulatorSnapshot snapshot;
  snapshot.simulator = simulator;
  if (!base || !simulator) {
    return snapshot;
  }
  snapshot.level = LoadU32At(base, simulator, kSimulatorLevelOffset);
  snapshot.add_bodies = LoadU32At(base, simulator, kQueuedAddBodiesOffset);
  snapshot.add_body_count = LoadU16At(base, simulator, kQueuedAddBodyCountOffset);
  snapshot.add_body_capacity = LoadU16At(base, simulator, kQueuedAddBodyCapacityOffset);
  snapshot.add_ids = LoadU32At(base, simulator, kQueuedAddIdsOffset);
  snapshot.add_id_count = LoadU16At(base, simulator, kQueuedAddIdCountOffset);
  snapshot.add_id_capacity = LoadU16At(base, simulator, kQueuedAddIdCapacityOffset);
  snapshot.remove_bodies = LoadU32At(base, simulator, kQueuedRemoveBodiesOffset);
  snapshot.remove_count = LoadU16At(base, simulator, kQueuedRemoveCountOffset);
  snapshot.remove_capacity = LoadU16At(base, simulator, kQueuedRemoveCapacityOffset);
  return snapshot;
}

bool QueueStateValid(const SimulatorSnapshot& snapshot) {
  return snapshot.add_body_count == snapshot.add_id_count &&
         snapshot.add_body_count <= snapshot.add_body_capacity &&
         snapshot.add_id_count <= snapshot.add_id_capacity &&
         snapshot.remove_count <= snapshot.remove_capacity;
}

uint32_t CurrentSimulator(uint8_t* base) {
  return base ? LoadU32(base, kPhysicsSimulatorGlobal) : 0;
}

uint32_t StreamingIndex(uint8_t* base, uint32_t entry) {
  if (!base || !entry) {
    return kNoXbnIndex;
  }
  const uint32_t entries = LoadU32(base, kStreamingEntriesGlobal);
  if (!entries || entry < entries) {
    return kNoXbnIndex;
  }
  const uint32_t difference = entry - entries;
  return difference % kStreamingEntryStride == 0 ? difference / kStreamingEntryStride : kNoXbnIndex;
}

void LogQueueViolation(const char* phase, const SimulatorSnapshot& snapshot) {
  if (QueueStateValid(snapshot)) {
    return;
  }
  REXLOG_WARN(
      "gta4-physics: event={} tick={} phase={} INVALID_QUEUE sim={:08X} level={:08X} "
      "addBodies={}/{} addIds={}/{} removes={}/{}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed), phase, snapshot.simulator,
      snapshot.level, snapshot.add_body_count, snapshot.add_body_capacity, snapshot.add_id_count,
      snapshot.add_id_capacity, snapshot.remove_count, snapshot.remove_capacity);
}

}  // namespace

extern "C" void sub_82511890(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_82511890(ctx, base);
    return;
  }
  const uint32_t entry = ctx.r3.u32;
  const uint32_t requested_state = ctx.r4.u32 & kStreamingStateMask;
  const uint32_t old_word = entry ? LoadU32At(base, entry, kStreamingStateWordOffset) : 0;
  const uint32_t old_state = (old_word >> kStreamingStateShift) & kStreamingStateMask;
  const uint32_t index = StreamingIndex(base, entry);
  __imp__sub_82511890(ctx, base);
  const uint32_t new_word = entry ? LoadU32At(base, entry, kStreamingStateWordOffset) : 0;
  const uint32_t new_state = (new_word >> kStreamingStateShift) & kStreamingStateMask;
  const uint64_t call = g_stream_state_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  if (old_state != new_state && ShouldSample(call, 1024, 4096)) {
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=stream-state call={} entry={:08X} index={} "
        "{}({})->{}({}) requested={} word={:08X}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), call, entry, index,
        StreamingStateName(old_state), old_state, StreamingStateName(new_state), new_state,
        requested_state, new_word);
  }
}

extern "C" void sub_82512BF0(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_82512BF0(ctx, base);
    return;
  }
  const uint32_t manager = ctx.r3.u32;
  const uint32_t index = ctx.r4.u32;
  const uint32_t source = ctx.r5.u32;
  __imp__sub_82512BF0(ctx, base);
  const uint64_t call = g_stream_complete_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  if (ShouldSample(call, 512, 2048)) {
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=stream-complete call={} manager={:08X} "
        "index={} source={:08X} result={:08X}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), call, manager, index, source,
        ctx.r3.u32);
  }
}

extern "C" void sub_82513A10(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_82513A10(ctx, base);
    return;
  }
  const uint32_t manager = ctx.r3.u32;
  const uint32_t index = ctx.r4.u32;
  REXLOG_INFO("gta4-physics: event={} tick={} phase=stream-unload manager={:08X} index={}",
              NextEvent(), g_tick_count.load(std::memory_order_relaxed), manager, index);
  __imp__sub_82513A10(ctx, base);
  REXLOG_INFO("gta4-physics: event={} tick={} phase=stream-unload-done index={} result={:08X}",
              NextEvent(), g_tick_count.load(std::memory_order_relaxed), index, ctx.r3.u32);
}

extern "C" void sub_825FD6B8(PPCContext& ctx, uint8_t* base) {
  __imp__sub_825FD6B8(ctx, base);
  if (!TraceEnabled()) {
    return;
  }
  const auto snapshot = ReadBoundsStore(base, 0);
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=xbn-store-init store={:08X} entries={:08X} "
      "flags={:08X} stride={} capacity={}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed), snapshot.store, snapshot.entries,
      snapshot.flags, snapshot.stride, kBoundsStoreCapacity);
}

extern "C" void sub_825FCD68(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_825FCD68(ctx, base);
    return;
  }
  const uint32_t index = ctx.r3.u32;
  const uint32_t asset = ctx.r4.u32;
  const auto before = ReadBoundsStore(base, index);
  __imp__sub_825FCD68(ctx, base);
  const auto after = ReadBoundsStore(base, index);
  const uint64_t call = g_xbn_publish_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=xbn-publish call={} index={} flags={:02X} "
      "asset={:08X}->{:08X} entry={:08X} body={:08X} result={} "
      "aabb=({:.3f},{:.3f},{:.3f})..({:.3f},{:.3f},{:.3f}) finite={} ordered={}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed), call, index, after.entry_flags,
      before.asset, after.asset, after.entry, after.body, ctx.r3.u32, after.minimum[0],
      after.minimum[1], after.minimum[2], after.maximum[0], after.maximum[1], after.maximum[2],
      after.finite, after.ordered);
  if (asset && (!after.entry || after.asset != asset || !after.body || !after.ordered)) {
    REXLOG_WARN(
        "gta4-physics: event={} tick={} phase=xbn-publish INVALID index={} requestedAsset={:08X} "
        "entry={:08X} publishedAsset={:08X} body={:08X} ordered={}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), index, asset, after.entry,
        after.asset, after.body, after.ordered);
  }
}

extern "C" void sub_825FC660(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_825FC660(ctx, base);
    return;
  }
  const uint32_t index = ctx.r3.u32;
  const auto before = ReadBoundsStore(base, index);
  __imp__sub_825FC660(ctx, base);
  const auto after = ReadBoundsStore(base, index);
  const uint64_t call = g_xbn_expand_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=xbn-expand call={} index={} asset={:08X} "
      "body={:08X}->{:08X} ordered={}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed), call, index, after.asset,
      before.body, after.body, after.ordered);
}

extern "C" void sub_825FC4F0(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_825FC4F0(ctx, base);
    return;
  }
  const uint32_t bound = ctx.r3.u32;
  const uint32_t index = ctx.r4.u32;
  const uint8_t bound_type = bound ? LoadU8At(base, bound, kBoundTypeOffset) : 0;
  const uint32_t previous_index = g_current_xbn_index;
  g_current_xbn_index = index;
  __imp__sub_825FC4F0(ctx, base);
  g_current_xbn_index = previous_index;
  const uint64_t call = g_body_create_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=xbn-body-create call={} index={} bound={:08X} "
      "type={} body={:08X}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed), call, index, bound, bound_type,
      ctx.r3.u32);
  if (!ctx.r3.u32) {
    REXLOG_WARN("gta4-physics: event={} tick={} phase=xbn-body-create FAILED index={} bound={:08X}",
                NextEvent(), g_tick_count.load(std::memory_order_relaxed), index, bound);
  }
}

extern "C" void sub_825FC448(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_825FC448(ctx, base);
    return;
  }
  const uint32_t index = ctx.r3.u32;
  const auto before = ReadBoundsStore(base, index);
  const uint32_t previous_index = g_current_xbn_index;
  g_current_xbn_index = index;
  __imp__sub_825FC448(ctx, base);
  g_current_xbn_index = previous_index;
  const auto after = ReadBoundsStore(base, index);
  const uint64_t call = g_xbn_unpublish_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=xbn-unpublish call={} index={} "
      "asset={:08X}->{:08X} body={:08X}->{:08X} result={:08X}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed), call, index, before.asset,
      after.asset, before.body, after.body, ctx.r3.u32);
}

extern "C" void sub_82956478(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_82956478(ctx, base);
    return;
  }
  const uint32_t simulator = ctx.r3.u32;
  const uint32_t body = ctx.r4.u32;
  const uint32_t caller_flags = ctx.r5.u32;
  const auto before = ReadSimulator(base, simulator);
  __imp__sub_82956478(ctx, base);
  const auto after = ReadSimulator(base, simulator);
  const uint32_t level_id = ctx.r3.u32;
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=sim-add xbn={} sim={:08X} level={:08X} "
      "body={:08X} id={:04X} callerFlags={:08X} queued={}->{}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed), g_current_xbn_index, simulator,
      after.level, body, level_id, caller_flags, before.add_body_count, after.add_body_count);
  LogQueueViolation("sim-add", after);
  if (!body || (level_id & kInvalidLevelId) == kInvalidLevelId) {
    REXLOG_WARN("gta4-physics: event={} tick={} phase=sim-add INVALID xbn={} body={:08X} id={:08X}",
                NextEvent(), g_tick_count.load(std::memory_order_relaxed), g_current_xbn_index,
                body, level_id);
  }
}

extern "C" void sub_82951980(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_82951980(ctx, base);
    return;
  }
  const uint32_t level = ctx.r3.u32;
  const uint32_t body = ctx.r4.u32;
  const uint32_t category = ctx.r5.u32;
  const uint32_t options = ctx.r6.u32;
  const bool deferred = (ctx.r7.u32 & 0xFF) != 0;
  const uint32_t caller_flags = ctx.r8.u32;
  __imp__sub_82951980(ctx, base);
  const uint16_t level_id = ctx.r3.u16;
  if (level_id != kInvalidLevelId) {
    BodyTraceRecord& record = g_body_records[level_id];
    record.body = body;
    record.level = level;
    record.xbn_index = g_current_xbn_index;
    record.category = category;
    record.options = options;
    record.active = true;
  }
  const uint64_t call = g_level_assign_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=level-assign call={} xbn={} level={:08X} "
      "body={:08X} id={:04X} deferred={} category={} options={:08X} callerFlags={:08X}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed), call, g_current_xbn_index, level,
      body, ctx.r3.u32, deferred, category, options, caller_flags);
}

extern "C" void sub_82956510(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_82956510(ctx, base);
    return;
  }
  const uint32_t simulator = ctx.r3.u32;
  const uint32_t level_id = ctx.r4.u32;
  const uint32_t caller_flags = ctx.r5.u32;
  const auto before = ReadSimulator(base, simulator);
  __imp__sub_82956510(ctx, base);
  if ((level_id & kInvalidLevelId) != kInvalidLevelId) {
    g_body_records[level_id & kInvalidLevelId].active = false;
  }
  const auto after = ReadSimulator(base, simulator);
  const uint64_t call = g_body_remove_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=sim-remove call={} xbn={} sim={:08X} "
      "level={:08X} id={:04X} callerFlags={:08X} queuedAdds={}->{} queuedRemoves={}->{}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed), call, g_current_xbn_index,
      simulator, after.level, level_id, caller_flags, before.add_body_count, after.add_body_count,
      before.remove_count, after.remove_count);
  LogQueueViolation("sim-remove", after);
}

extern "C" void sub_8294BAE0(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_8294BAE0(ctx, base);
    return;
  }
  const uint32_t broadphase = ctx.r3.u32;
  const uint32_t minimum_address = ctx.r4.u32;
  const uint32_t maximum_address = ctx.r5.u32;
  const uint16_t id = ctx.r6.u16;
  const uint32_t vtable = broadphase ? LoadU32(base, broadphase) : 0;
  const auto minimum = ReadVector3(base, minimum_address);
  const auto maximum = ReadVector3(base, maximum_address);
  const auto before = ReadLevelBroadphaseMembership(base, broadphase, id);
  StoreBodyAabb(id, g_body_records[id].body, minimum, maximum);
  g_last_broadphase = broadphase;
  __imp__sub_8294BAE0(ctx, base);
  const auto after = ReadLevelBroadphaseMembership(base, broadphase, id);
  const BodyTraceRecord& record = g_body_records[id];
  if (record.xbn_index != kNoXbnIndex || !AabbOrdered(minimum, maximum) ||
      AabbCollapsed(minimum, maximum)) {
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=level-broadphase-activate "
        "class={} vtable={:08X} object={:08X} id={:04X} body={:08X} "
        "xbn={} category={} options={:08X} active={}->{} count={}->{} "
        "maximumId={}->{} aabb=({:.6f},{:.6f},{:.6f}).."
        "({:.6f},{:.6f},{:.6f}) finite={} ordered={} collapsed={}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed),
        BroadphaseClassName(vtable), vtable, broadphase, id, record.body,
        record.xbn_index, record.category, record.options, before.active_value,
        after.active_value, before.active_count, after.active_count,
        before.maximum_id, after.maximum_id, minimum[0], minimum[1],
        minimum[2], maximum[0], maximum[1], maximum[2],
        AabbFinite(minimum, maximum), AabbOrdered(minimum, maximum),
        AabbCollapsed(minimum, maximum));
  }
}

extern "C" void sub_8294B600(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_8294B600(ctx, base);
    return;
  }
  const uint32_t broadphase = ctx.r3.u32;
  const uint32_t minimum_address = ctx.r4.u32;
  const uint32_t maximum_address = ctx.r5.u32;
  const uint16_t id = ctx.r6.u16;
  const uint32_t vtable = broadphase ? LoadU32(base, broadphase) : 0;
  const uint16_t count_before =
      broadphase ? LoadU16At(base, broadphase, kBroadphaseCountOffset) : 0;
  const auto minimum = ReadVector3(base, minimum_address);
  const auto maximum = ReadVector3(base, maximum_address);
  StoreBodyAabb(id, g_body_records[id].body, minimum, maximum);
  __imp__sub_8294B600(ctx, base);
  const uint16_t count_after =
      broadphase ? LoadU16At(base, broadphase, kBroadphaseCountOffset) : 0;
  const BroadphaseMembership membership =
      ReadBroadphaseMembership(base, broadphase, id);
  const BodyTraceRecord& record = g_body_records[id];
  if (record.xbn_index != kNoXbnIndex || !AabbOrdered(minimum, maximum) ||
      AabbCollapsed(minimum, maximum)) {
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=broadphase-insert-one "
        "class={} vtable={:08X} object={:08X} id={:04X} body={:08X} "
        "xbn={} category={} options={:08X} count={}->{} slot={} "
        "storedId={:04X} present={} "
        "aabb=({:.6f},{:.6f},{:.6f})..({:.6f},{:.6f},{:.6f}) "
        "finite={} ordered={} collapsed={}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed),
        BroadphaseClassName(vtable), vtable, broadphase, id, record.body,
        record.xbn_index, record.category, record.options, count_before,
        count_after, membership.slot, membership.stored_id,
        membership.present, minimum[0], minimum[1], minimum[2], maximum[0],
        maximum[1], maximum[2], AabbFinite(minimum, maximum),
        AabbOrdered(minimum, maximum), AabbCollapsed(minimum, maximum));
  }
}

extern "C" void sub_8294B6E8(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_8294B6E8(ctx, base);
    return;
  }
  const uint32_t broadphase = ctx.r3.u32;
  const uint32_t minimums = ctx.r4.u32;
  const uint32_t maximums = ctx.r5.u32;
  const uint32_t ids = ctx.r6.u32;
  const uint16_t count = ctx.r7.u16;
  const uint32_t vtable = broadphase ? LoadU32(base, broadphase) : 0;
  const uint16_t count_before =
      broadphase ? LoadU16At(base, broadphase, kBroadphaseCountOffset) : 0;
  g_last_broadphase = broadphase;
  uint32_t static_count = 0;
  uint32_t invalid_count = 0;
  for (uint32_t index = 0; index < count; ++index) {
    uint32_t minimum_address = 0;
    uint32_t maximum_address = 0;
    uint32_t id_address = 0;
    if (!TryGuestArrayAddress(minimums, index, kAabbStride,
                              minimum_address) ||
        !TryGuestArrayAddress(maximums, index, kAabbStride,
                              maximum_address) ||
        !TryGuestArrayAddress(ids, index, kIdStride, id_address)) {
      ++invalid_count;
      continue;
    }
    const uint16_t id = LoadU32(base, id_address) & kInvalidLevelId;
    const auto minimum = ReadVector3(base, minimum_address);
    const auto maximum = ReadVector3(base, maximum_address);
    StoreBodyAabb(id, g_body_records[id].body, minimum, maximum);
    const BodyTraceRecord& record = g_body_records[id];
    const bool static_xbn = record.xbn_index != kNoXbnIndex;
    static_count += static_xbn ? 1 : 0;
    invalid_count +=
        !AabbOrdered(minimum, maximum) || AabbCollapsed(minimum, maximum);
    if (static_xbn) {
      REXLOG_INFO(
          "gta4-physics: event={} tick={} phase=broadphase-batch-aabb "
          "class={} vtable={:08X} object={:08X} index={}/{} id={:04X} "
          "body={:08X} xbn={} category={} options={:08X} "
          "aabb=({:.6f},{:.6f},{:.6f})..({:.6f},{:.6f},{:.6f}) "
          "finite={} ordered={} collapsed={}",
          NextEvent(), g_tick_count.load(std::memory_order_relaxed),
          BroadphaseClassName(vtable), vtable, broadphase, index, count, id,
          record.body, record.xbn_index, record.category, record.options,
          minimum[0], minimum[1], minimum[2], maximum[0], maximum[1],
          maximum[2], AabbFinite(minimum, maximum),
          AabbOrdered(minimum, maximum), AabbCollapsed(minimum, maximum));
    }
  }
  __imp__sub_8294B6E8(ctx, base);
  const uint16_t count_after =
      broadphase ? LoadU16At(base, broadphase, kBroadphaseCountOffset) : 0;
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=broadphase-batch "
      "class={} vtable={:08X} object={:08X} requested={} static={} "
      "invalid={} count={}->{}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed),
      BroadphaseClassName(vtable), vtable, broadphase, count, static_count,
      invalid_count, count_before, count_after);
}

extern "C" void sub_8294B7A8(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_8294B7A8(ctx, base);
    return;
  }
  const uint32_t broadphase = ctx.r3.u32;
  const uint16_t id = ctx.r4.u16;
  const uint32_t minimum_address = ctx.r5.u32;
  const uint32_t maximum_address = ctx.r6.u32;
  const auto minimum = ReadVector3(base, minimum_address);
  const auto maximum = ReadVector3(base, maximum_address);
  StoreBodyAabb(id, g_body_records[id].body, minimum, maximum);
  __imp__sub_8294B7A8(ctx, base);
  g_last_broadphase = broadphase;
  LogDynamicStaticOverlaps(id);
}

extern "C" void sub_8294E568(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_8294E568(ctx, base);
    return;
  }
  const uint32_t level = ctx.r3.u32;
  const uint32_t body = ctx.r4.u32;
  const uint32_t level_id = ctx.r5.u32;
  __imp__sub_8294E568(ctx, base);
  const uint64_t call = g_immediate_insert_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  if (g_current_xbn_index != kNoXbnIndex || ShouldSample(call, 128, 2048)) {
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=broadphase-insert-immediate call={} xbn={} "
        "level={:08X} body={:08X} id={:04X} result={:08X}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), call, g_current_xbn_index, level,
        body, level_id, ctx.r3.u32);
  }
}

extern "C" void sub_8294E6D0(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_8294E6D0(ctx, base);
    return;
  }
  const uint32_t level = ctx.r3.u32;
  const uint32_t bodies = ctx.r4.u32;
  const uint32_t count = ctx.r5.u32;
  const uint32_t ids = ctx.r6.u32;
  const uint32_t first_body = count && bodies ? LoadU32(base, bodies) : 0;
  const uint32_t first_id = count && ids ? LoadU32(base, ids) : kInvalidLevelId;
  const uint32_t broadphase =
      level ? LoadU32At(base, level, kLevelBroadphaseOffset) : 0;
  g_last_broadphase = broadphase;
  __imp__sub_8294E6D0(ctx, base);
  const uint64_t call = g_deferred_insert_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  const uint32_t vtable = broadphase ? LoadU32(base, broadphase) : 0;
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=broadphase-insert-deferred call={} "
      "level={:08X} broadphase={:08X} class={} vtable={:08X} count={} "
      "bodies={:08X} ids={:08X} firstBody={:08X} firstId={:04X}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed), call, level,
      broadphase, BroadphaseClassName(vtable), vtable, count, bodies, ids,
      first_body, first_id);
}

extern "C" void sub_829561D0(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_829561D0(ctx, base);
    return;
  }
  const uint32_t simulator = ctx.r3.u32;
  const auto before = ReadSimulator(base, simulator);
  __imp__sub_829561D0(ctx, base);
  const auto after = ReadSimulator(base, simulator);
  if (before.add_body_count || before.add_id_count || before.remove_count) {
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=queue-drain sim={:08X} level={:08X} "
        "adds={}/{}->{}/{} removes={}->{}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), simulator, before.level,
        before.add_body_count, before.add_id_count, after.add_body_count, after.add_id_count,
        before.remove_count, after.remove_count);

    const uint32_t broadphase = g_last_broadphase;
    const uint32_t vtable = broadphase ? LoadU32(base, broadphase) : 0;
    uint32_t expected_static = 0;
    uint32_t present_static = 0;
    uint32_t missing_static = 0;
    for (uint32_t candidate = 0; candidate < g_body_records.size();
         ++candidate) {
      const BodyTraceRecord& record = g_body_records[candidate];
      if (!record.active || record.xbn_index == kNoXbnIndex) {
        continue;
      }
      ++expected_static;
      if (vtable != kLevelBroadphaseVtable) {
        continue;
      }
      const auto membership = ReadLevelBroadphaseMembership(
          base, broadphase, static_cast<uint16_t>(candidate));
      if (membership.present) {
        ++present_static;
      } else {
        ++missing_static;
        REXLOG_WARN(
            "gta4-physics: event={} tick={} phase=post-drain-static-missing "
            "class={} vtable={:08X} object={:08X} id={:04X} body={:08X} "
            "xbn={} category={} options={:08X} activeValue={} maximumId={}",
            NextEvent(), g_tick_count.load(std::memory_order_relaxed),
            BroadphaseClassName(vtable), vtable, broadphase, candidate,
            record.body, record.xbn_index, record.category, record.options,
            membership.active_value, membership.maximum_id);
      }
    }
    if (vtable == kLevelBroadphaseVtable) {
      REXLOG_INFO(
          "gta4-physics: event={} tick={} phase=post-drain-static-membership "
          "class={} vtable={:08X} object={:08X} activeCount={} maximumId={} "
          "expected={} present={} missing={}",
          NextEvent(), g_tick_count.load(std::memory_order_relaxed),
          BroadphaseClassName(vtable), vtable, broadphase,
          LoadU16At(base, broadphase, kLevelBroadphaseActiveCountOffset),
          LoadU32At(base, broadphase, kLevelBroadphaseMaximumIdOffset),
          expected_static, present_static, missing_static);
    } else {
      REXLOG_INFO(
          "gta4-physics: event={} tick={} phase=post-drain-static-class "
          "class={} vtable={:08X} object={:08X} expectedStatic={}",
          NextEvent(), g_tick_count.load(std::memory_order_relaxed),
          BroadphaseClassName(vtable), vtable, broadphase, expected_static);
    }
  }
  LogQueueViolation("queue-drain-before", before);
  LogQueueViolation("queue-drain-after", after);
}

void TraceConcreteBroadphaseUpdateBefore(uint8_t* base, uint32_t broadphase,
                                         const char* implementation) {
  const uint32_t vtable = broadphase ? LoadU32(base, broadphase) : 0;
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=concrete-broadphase-update-begin "
      "impl={} class={} vtable={:08X} object={:08X} count={} pendingPairs={}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed), implementation,
      BroadphaseClassName(vtable), vtable, broadphase,
      broadphase ? LoadU16At(base, broadphase, kBroadphaseCountOffset) : 0,
      broadphase ? LoadU16At(base, broadphase, 20) : 0);
}

void TraceConcreteBroadphaseUpdateAfter(uint8_t* base, uint32_t broadphase,
                                        const char* implementation) {
  const uint32_t vtable = broadphase ? LoadU32(base, broadphase) : 0;
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=concrete-broadphase-update-end "
      "impl={} class={} vtable={:08X} object={:08X} count={} pendingPairs={}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed), implementation,
      BroadphaseClassName(vtable), vtable, broadphase,
      broadphase ? LoadU16At(base, broadphase, kBroadphaseCountOffset) : 0,
      broadphase ? LoadU16At(base, broadphase, 20) : 0);
}

extern "C" void sub_829C7070(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_829C7070(ctx, base);
    return;
  }
  const uint32_t broadphase = ctx.r3.u32;
  TraceConcreteBroadphaseUpdateBefore(base, broadphase, "sub_829C7070");
  __imp__sub_829C7070(ctx, base);
  TraceConcreteBroadphaseUpdateAfter(base, broadphase, "sub_829C7070");
}

extern "C" void sub_829C7D40(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_829C7D40(ctx, base);
    return;
  }
  const uint32_t broadphase = ctx.r3.u32;
  TraceConcreteBroadphaseUpdateBefore(base, broadphase, "sub_829C7D40");
  __imp__sub_829C7D40(ctx, base);
  TraceConcreteBroadphaseUpdateAfter(base, broadphase, "sub_829C7D40");
}

extern "C" void sub_829C7F10(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_829C7F10(ctx, base);
    return;
  }
  const uint32_t broadphase = ctx.r3.u32;
  TraceConcreteBroadphaseUpdateBefore(base, broadphase, "sub_829C7F10");
  __imp__sub_829C7F10(ctx, base);
  TraceConcreteBroadphaseUpdateAfter(base, broadphase, "sub_829C7F10");
}

extern "C" void sub_829C96E0(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_829C96E0(ctx, base);
    return;
  }
  const uint32_t broadphase = ctx.r3.u32;
  const uint16_t id = ctx.r4.u16;
  const uint32_t minimum_address = ctx.r5.u32;
  const uint32_t maximum_address = ctx.r6.u32;
  const auto minimum = ReadVector3(base, minimum_address);
  const auto maximum = ReadVector3(base, maximum_address);
  StoreBodyAabb(id, g_body_records[id].body, minimum, maximum);
  __imp__sub_829C96E0(ctx, base);
  const uint32_t overlap_count =
      LogAxisSweepDynamicStaticDetails(base, broadphase, id);
  const uint64_t call =
      g_axis_sweep_update_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  const BodyTraceRecord& record = g_body_records[id];
  if (overlap_count || ShouldSample(call, 64, 2048)) {
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=axis-sweep3-set-aabb "
        "call={} object={:08X} id={:04X} body={:08X} xbn={} category={} "
        "aabb=({:.6f},{:.6f},{:.6f})..({:.6f},{:.6f},{:.6f}) "
        "finite={} ordered={} staticOverlaps={}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), call,
        broadphase, id, record.body, record.xbn_index, record.category,
        minimum[0], minimum[1], minimum[2], maximum[0], maximum[1],
        maximum[2], AabbFinite(minimum, maximum),
        AabbOrdered(minimum, maximum), overlap_count);
  }
}

extern "C" void sub_829C8400(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_829C8400(ctx, base);
    return;
  }
  const uint32_t broadphase = ctx.r3.u32;
  const uint16_t first = ctx.r4.u16;
  const uint16_t second = ctx.r5.u16;
  const BodyTraceRecord& first_record = g_body_records[first];
  const BodyTraceRecord& second_record = g_body_records[second];
  const bool first_static =
      first_record.active && first_record.xbn_index != kNoXbnIndex;
  const bool second_static =
      second_record.active && second_record.xbn_index != kNoXbnIndex;
  const AxisSweepFilterState first_filter =
      ReadAxisSweepFilterState(base, broadphase, first);
  const AxisSweepFilterState second_filter =
      ReadAxisSweepFilterState(base, broadphase, second);
  __imp__sub_829C8400(ctx, base);
  const bool accepted = ctx.r3.u32 != 0;
  const uint64_t call =
      g_axis_sweep_filter_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  if (first_static || second_static) {
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=axis-sweep3-filter "
        "call={} object={:08X} first={:04X} firstBody={:08X} firstXbn={} "
        "firstCategory={} firstOptions={:08X} firstTypeWord={:08X} "
        "firstFlag={:02X} second={:04X} secondBody={:08X} secondXbn={} "
        "secondCategory={} secondOptions={:08X} secondTypeWord={:08X} "
        "secondFlag={:02X} accepted={}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), call,
        broadphase, first, first_record.body, first_record.xbn_index,
        first_record.category, first_record.options, first_filter.type_word,
        first_filter.flag_byte, second, second_record.body,
        second_record.xbn_index, second_record.category, second_record.options,
        second_filter.type_word, second_filter.flag_byte, accepted);
  }
}

extern "C" void sub_829C4AF8(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_829C4AF8(ctx, base);
    return;
  }
  const uint32_t pair_cache = ctx.r3.u32;
  const uint16_t first = ctx.r4.u16;
  const uint16_t second = ctx.r5.u16;
  const BodyTraceRecord& first_record = g_body_records[first];
  const BodyTraceRecord& second_record = g_body_records[second];
  const bool first_static =
      first_record.active && first_record.xbn_index != kNoXbnIndex;
  const bool second_static =
      second_record.active && second_record.xbn_index != kNoXbnIndex;
  const uint32_t count_before =
      pair_cache ? LoadU32At(base, pair_cache, 8) : 0;
  const uint32_t pending_before =
      pair_cache ? LoadU32At(base, pair_cache, 12) : 0;
  __imp__sub_829C4AF8(ctx, base);
  const uint32_t count_after =
      pair_cache ? LoadU32At(base, pair_cache, 8) : 0;
  const uint32_t pending_after =
      pair_cache ? LoadU32At(base, pair_cache, 12) : 0;
  const uint64_t call =
      g_axis_sweep_pair_add_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  if (first_static || second_static) {
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=axis-sweep3-pair-add "
        "call={} pairCache={:08X} first={:04X} firstBody={:08X} "
        "firstXbn={} firstCategory={} second={:04X} secondBody={:08X} "
        "secondXbn={} secondCategory={} count={}->{} pending={}->{}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), call,
        pair_cache, first, first_record.body, first_record.xbn_index,
        first_record.category, second, second_record.body,
        second_record.xbn_index, second_record.category, count_before,
        count_after, pending_before, pending_after);
  }
}

extern "C" void sub_829C9EB0(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_829C9EB0(ctx, base);
    return;
  }
  const uint32_t broadphase = ctx.r3.u32;
  const uint32_t minimums = ctx.r4.u32;
  const uint32_t maximums = ctx.r5.u32;
  const uint32_t ids = ctx.r6.u32;
  const uint16_t count = ctx.r7.u16;
  const uint32_t vtable = broadphase ? LoadU32(base, broadphase) : 0;
  uint32_t static_count = 0;
  uint32_t invalid_count = 0;
  for (uint32_t index = 0; index < count; ++index) {
    uint32_t minimum_address = 0;
    uint32_t maximum_address = 0;
    uint32_t id_address = 0;
    if (!TryGuestArrayAddress(minimums, index, kAabbStride,
                              minimum_address) ||
        !TryGuestArrayAddress(maximums, index, kAabbStride,
                              maximum_address) ||
        !TryGuestArrayAddress(ids, index, kIdStride, id_address)) {
      ++invalid_count;
      continue;
    }
    const uint16_t id = LoadU32(base, id_address) & kInvalidLevelId;
    const auto minimum = ReadVector3(base, minimum_address);
    const auto maximum = ReadVector3(base, maximum_address);
    StoreBodyAabb(id, g_body_records[id].body, minimum, maximum);
    const BodyTraceRecord& record = g_body_records[id];
    const bool static_xbn = record.xbn_index != kNoXbnIndex;
    static_count += static_xbn ? 1 : 0;
    const bool invalid =
        !AabbOrdered(minimum, maximum) || AabbCollapsed(minimum, maximum);
    invalid_count += invalid ? 1 : 0;
    if (static_xbn || invalid) {
      REXLOG_INFO(
          "gta4-physics: event={} tick={} phase=axis-sweep3-alt-batch-aabb "
          "class={} vtable={:08X} object={:08X} index={}/{} id={:04X} "
          "body={:08X} xbn={} category={} options={:08X} "
          "aabb=({:.6f},{:.6f},{:.6f})..({:.6f},{:.6f},{:.6f}) "
          "finite={} ordered={} collapsed={}",
          NextEvent(), g_tick_count.load(std::memory_order_relaxed),
          BroadphaseClassName(vtable), vtable, broadphase, index, count, id,
          record.body, record.xbn_index, record.category, record.options,
          minimum[0], minimum[1], minimum[2], maximum[0], maximum[1],
          maximum[2], AabbFinite(minimum, maximum),
          AabbOrdered(minimum, maximum), AabbCollapsed(minimum, maximum));
    }
  }
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=axis-sweep3-alt-batch "
      "class={} vtable={:08X} object={:08X} count={} static={} invalid={}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed),
      BroadphaseClassName(vtable), vtable, broadphase, count, static_count,
      invalid_count);
  __imp__sub_829C9EB0(ctx, base);
  LogAxisSweepStaticQuantization(base, broadphase, ids, count,
                                 "sub_829C9EB0");
}

extern "C" void sub_829CA148(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_829CA148(ctx, base);
    return;
  }
  const uint32_t broadphase = ctx.r3.u32;
  const uint32_t minimums = ctx.r4.u32;
  const uint32_t maximums = ctx.r5.u32;
  const uint32_t ids = ctx.r6.u32;
  const uint16_t count = ctx.r7.u16;
  const uint32_t vtable = broadphase ? LoadU32(base, broadphase) : 0;
  uint32_t static_count = 0;
  uint32_t invalid_count = 0;
  for (uint32_t index = 0; index < count; ++index) {
    uint32_t minimum_address = 0;
    uint32_t maximum_address = 0;
    uint32_t id_address = 0;
    if (!TryGuestArrayAddress(minimums, index, kAabbStride,
                              minimum_address) ||
        !TryGuestArrayAddress(maximums, index, kAabbStride,
                              maximum_address) ||
        !TryGuestArrayAddress(ids, index, kIdStride, id_address)) {
      ++invalid_count;
      continue;
    }
    const uint16_t id = LoadU32(base, id_address) & kInvalidLevelId;
    const auto minimum = ReadVector3(base, minimum_address);
    const auto maximum = ReadVector3(base, maximum_address);
    StoreBodyAabb(id, g_body_records[id].body, minimum, maximum);
    const BodyTraceRecord& record = g_body_records[id];
    const bool static_xbn = record.xbn_index != kNoXbnIndex;
    static_count += static_xbn ? 1 : 0;
    const bool invalid =
        !AabbOrdered(minimum, maximum) || AabbCollapsed(minimum, maximum);
    invalid_count += invalid ? 1 : 0;
    if (static_xbn || invalid) {
      REXLOG_INFO(
          "gta4-physics: event={} tick={} phase=axis-sweep3-batch-aabb "
          "class={} vtable={:08X} object={:08X} index={}/{} id={:04X} "
          "body={:08X} xbn={} category={} options={:08X} "
          "aabb=({:.6f},{:.6f},{:.6f})..({:.6f},{:.6f},{:.6f}) "
          "finite={} ordered={} collapsed={}",
          NextEvent(), g_tick_count.load(std::memory_order_relaxed),
          BroadphaseClassName(vtable), vtable, broadphase, index, count, id,
          record.body, record.xbn_index, record.category, record.options,
          minimum[0], minimum[1], minimum[2], maximum[0], maximum[1],
          maximum[2], AabbFinite(minimum, maximum),
          AabbOrdered(minimum, maximum), AabbCollapsed(minimum, maximum));
    }
  }
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=axis-sweep3-batch "
      "class={} vtable={:08X} object={:08X} count={} static={} invalid={}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed),
      BroadphaseClassName(vtable), vtable, broadphase, count, static_count,
      invalid_count);
  __imp__sub_829CA148(ctx, base);
  LogAxisSweepStaticQuantization(base, broadphase, ids, count,
                                 "sub_829CA148");
}

extern "C" void sub_829CA560(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_829CA560(ctx, base);
    return;
  }
  const uint32_t broadphase = ctx.r3.u32;
  const uint32_t vtable = broadphase ? LoadU32(base, broadphase) : 0;
  __imp__sub_829CA560(ctx, base);
  const uint32_t pair_cache =
      broadphase ? LoadU32At(base, broadphase, 4) : 0;
  const uint32_t pairs = pair_cache ? LoadU32At(base, pair_cache, 4) : 0;
  const uint32_t pair_count =
      pair_cache ? LoadU32At(base, pair_cache, 8) : 0;
  constexpr uint32_t kMaximumTracePairCount = 1u << 16;
  const uint32_t inspected_count =
      std::min(pair_count, kMaximumTracePairCount);
  uint32_t static_pairs = 0;
  uint32_t dynamic_static_pairs = 0;
  for (uint32_t index = 0; index < inspected_count; ++index) {
    uint32_t pair = 0;
    if (!TryGuestArrayAddress(pairs, index, 8, pair)) {
      continue;
    }
    const uint16_t first = LoadU16At(base, pair, 0);
    const uint16_t second = LoadU16At(base, pair, 2);
    const BodyTraceRecord& first_record = g_body_records[first];
    const BodyTraceRecord& second_record = g_body_records[second];
    const bool first_static =
        first_record.active && first_record.xbn_index != kNoXbnIndex;
    const bool second_static =
        second_record.active && second_record.xbn_index != kNoXbnIndex;
    if (!first_static && !second_static) {
      continue;
    }
    ++static_pairs;
    const bool first_dynamic =
        first_record.active && !first_static && first_record.category == 1;
    const bool second_dynamic =
        second_record.active && !second_static && second_record.category == 1;
    const bool dynamic_static =
        (first_dynamic && second_static) || (second_dynamic && first_static);
    dynamic_static_pairs += dynamic_static ? 1 : 0;
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=axis-sweep3-pair "
        "object={:08X} index={}/{} first={:04X} firstBody={:08X} "
        "firstXbn={} firstCategory={} second={:04X} secondBody={:08X} "
        "secondXbn={} secondCategory={} dynamicStatic={}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), broadphase,
        index, inspected_count, first, first_record.body,
        first_record.xbn_index, first_record.category, second,
        second_record.body, second_record.xbn_index, second_record.category,
        dynamic_static);
  }
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=axis-sweep3-update "
      "class={} vtable={:08X} object={:08X} pairCache={:08X} pairs={} "
      "inspected={} staticPairs={} dynamicStatic={}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed),
      BroadphaseClassName(vtable), vtable, broadphase, pair_cache, pair_count,
      inspected_count, static_pairs, dynamic_static_pairs);
}

extern "C" void sub_829C5530(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_829C5530(ctx, base);
    return;
  }
  const uint32_t pair_manager = ctx.r3.u32;
  const uint32_t pairs = ctx.r4.u32;
  const uint32_t requested_count = ctx.r5.u32;
  constexpr uint32_t kMaximumTracePairCount = 1u << 16;
  const uint32_t pair_count =
      std::min(requested_count, kMaximumTracePairCount);
  uint32_t static_pairs = 0;
  uint32_t dynamic_static_pairs = 0;
  uint32_t known_aabb_overlaps = 0;
  for (uint32_t index = 0; index < pair_count; ++index) {
    uint32_t pair = 0;
    if (!TryGuestArrayAddress(pairs, index, 8, pair)) {
      continue;
    }
    const uint16_t first = LoadU16At(base, pair, 0);
    const uint16_t second = LoadU16At(base, pair, 2);
    const BodyTraceRecord& first_record = g_body_records[first];
    const BodyTraceRecord& second_record = g_body_records[second];
    const bool first_static =
        first_record.active && first_record.xbn_index != kNoXbnIndex;
    const bool second_static =
        second_record.active && second_record.xbn_index != kNoXbnIndex;
    if (!first_static && !second_static) {
      continue;
    }
    ++static_pairs;
    const bool first_dynamic = first_record.active && !first_static &&
                               first_record.category == 1;
    const bool second_dynamic = second_record.active && !second_static &&
                                second_record.category == 1;
    const bool dynamic_static =
        (first_dynamic && second_static) || (second_dynamic && first_static);
    dynamic_static_pairs += dynamic_static ? 1 : 0;
    const bool current_aabb_overlap =
        first_record.has_aabb && second_record.has_aabb &&
        AabbOverlaps(first_record.minimum, first_record.maximum,
                     second_record.minimum, second_record.maximum);
    known_aabb_overlaps += current_aabb_overlap ? 1 : 0;
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=level-pair-output "
        "manager={:08X} index={}/{} first={:04X} firstBody={:08X} "
        "firstXbn={} firstCategory={} firstOptions={:08X} second={:04X} "
        "secondBody={:08X} secondXbn={} secondCategory={} "
        "secondOptions={:08X} dynamicStatic={} currentAabbOverlap={}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), pair_manager,
        index, pair_count, first, first_record.body, first_record.xbn_index,
        first_record.category, first_record.options, second, second_record.body,
        second_record.xbn_index, second_record.category, second_record.options,
        dynamic_static, current_aabb_overlap);
  }
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=level-pair-batch "
      "manager={:08X} pairs={:08X} requested={} inspected={} staticPairs={} "
      "dynamicStatic={} knownAabbOverlaps={}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed), pair_manager,
      pairs, requested_count, pair_count, static_pairs, dynamic_static_pairs,
      known_aabb_overlaps);
  __imp__sub_829C5530(ctx, base);
}

extern "C" void sub_829D9128(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_829D9128(ctx, base);
    return;
  }
  const uint32_t broadphase = ctx.r3.u32;
  const uint32_t destination = ctx.r4.u32;
  const uint32_t destination_offset = ctx.r5.u32;
  const uint32_t pairs =
      broadphase ? LoadU32At(base, broadphase, 16) : 0;
  const uint16_t pair_count =
      broadphase ? LoadU16At(base, broadphase, 20) : 0;
  uint32_t dynamic_static_pairs = 0;
  uint32_t overlapping_pairs = 0;
  for (uint32_t index = 0; index < pair_count; ++index) {
    uint32_t pair = 0;
    if (!TryGuestArrayAddress(pairs, index, 8, pair)) {
      continue;
    }
    const uint16_t first = LoadU16At(base, pair, 0);
    const uint16_t second = LoadU16At(base, pair, 2);
    const uint32_t flags = LoadU32At(base, pair, 4);
    const BodyTraceRecord& first_record = g_body_records[first];
    const BodyTraceRecord& second_record = g_body_records[second];
    const bool first_dynamic = first_record.active &&
                               first_record.xbn_index == kNoXbnIndex &&
                               first_record.category == 1;
    const bool second_dynamic = second_record.active &&
                                second_record.xbn_index == kNoXbnIndex &&
                                second_record.category == 1;
    const bool first_static = first_record.active &&
                              first_record.xbn_index != kNoXbnIndex;
    const bool second_static = second_record.active &&
                               second_record.xbn_index != kNoXbnIndex;
    if (!((first_dynamic && second_static) ||
          (second_dynamic && first_static))) {
      continue;
    }
    ++dynamic_static_pairs;
    const BodyTraceRecord& dynamic = first_dynamic ? first_record : second_record;
    const BodyTraceRecord& static_record = first_static ? first_record : second_record;
    const bool overlaps =
        dynamic.has_aabb && static_record.has_aabb &&
        AabbOverlaps(dynamic.minimum, dynamic.maximum, static_record.minimum,
                     static_record.maximum);
    overlapping_pairs += overlaps ? 1 : 0;
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=broadphase-pair-output "
        "object={:08X} index={}/{} first={:04X} second={:04X} "
        "dynamic={:04X} dynamicBody={:08X} dynamicCategory={} "
        "dynamicOptions={:08X} static={:04X} staticBody={:08X} xbn={} "
        "flags={:08X} currentAabbOverlap={}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), broadphase,
        index, pair_count, first, second, first_dynamic ? first : second,
        dynamic.body, dynamic.category, dynamic.options,
        first_static ? first : second, static_record.body,
        static_record.xbn_index, flags, overlaps);
  }
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=broadphase-pair-batch "
      "object={:08X} destination={:08X} destinationOffset={} pairs={} "
      "dynamicStatic={} overlapping={}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed), broadphase,
      destination, destination_offset, pair_count, dynamic_static_pairs,
      overlapping_pairs);
  __imp__sub_829D9128(ctx, base);
}

extern "C" void sub_82958128(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_82958128(ctx, base);
    return;
  }
  const uint32_t simulator = ctx.r3.u32;
  const double delta_seconds = ctx.f1.f64;
  const uint64_t tick = g_tick_count.fetch_add(1, std::memory_order_relaxed) + 1;
  const int32_t interval = REXCVAR_GET(gta4_trace_physics_tick_interval);
  const bool sample = interval > 0 && tick % static_cast<uint32_t>(interval) == 0;
  const auto before = ReadSimulator(base, simulator);
  if (!std::isfinite(delta_seconds) || delta_seconds <= 0.0 || delta_seconds > 0.25) {
    REXLOG_WARN("gta4-physics: event={} tick={} phase=tick INVALID_DT sim={:08X} dt={}",
                NextEvent(), tick, simulator, delta_seconds);
  }
  __imp__sub_82958128(ctx, base);
  const auto after = ReadSimulator(base, simulator);
  if (sample) {
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=tick-summary sim={:08X} globalSim={:08X} "
        "level={:08X} dt={:.9f} queues(add={}/{},remove={}) "
        "totals(publish={},unpublish={},expand={},bodyAdd={},bodyRemove={},assign={},immediate={},"
        "deferred={},"
        "bpScan={},bpUpdate={},pairs={},geom={},hits={})",
        NextEvent(), tick, simulator, CurrentSimulator(base), after.level, delta_seconds,
        after.add_body_count, after.add_id_count, after.remove_count,
        g_xbn_publish_calls.load(std::memory_order_relaxed),
        g_xbn_unpublish_calls.load(std::memory_order_relaxed),
        g_xbn_expand_calls.load(std::memory_order_relaxed),
        g_body_create_calls.load(std::memory_order_relaxed),
        g_body_remove_calls.load(std::memory_order_relaxed),
        g_level_assign_calls.load(std::memory_order_relaxed),
        g_immediate_insert_calls.load(std::memory_order_relaxed),
        g_deferred_insert_calls.load(std::memory_order_relaxed),
        g_broadphase_scan_calls.load(std::memory_order_relaxed),
        g_broadphase_update_calls.load(std::memory_order_relaxed),
        g_contact_pair_calls.load(std::memory_order_relaxed),
        g_geometry_test_calls.load(std::memory_order_relaxed),
        g_geometry_test_hits.load(std::memory_order_relaxed));
  }
  LogQueueViolation("tick-before", before);
  LogQueueViolation("tick-after", after);
}

extern "C" void sub_829C56F8(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_829C56F8(ctx, base);
    return;
  }
  const uint32_t broadphase = ctx.r3.u32;
  const uint32_t query = ctx.r4.u32;
  __imp__sub_829C56F8(ctx, base);
  const uint64_t call = g_broadphase_scan_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  if (ShouldSample(call, 32, 4096)) {
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=broadphase-scan call={} object={:08X} "
        "query={:08X} result={:08X}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), call, broadphase, query,
        ctx.r3.u32);
  }
}

extern "C" void sub_829C5888(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_829C5888(ctx, base);
    return;
  }
  const uint32_t broadphase = ctx.r3.u32;
  const uint32_t state = broadphase ? LoadU32At(base, broadphase, kBroadphaseStateOffset) : 0;
  const uint16_t body_count = state ? LoadU16At(base, state, kBroadphaseBodyCountOffset) : 0;
  __imp__sub_829C5888(ctx, base);
  const uint64_t call = g_broadphase_update_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  if (ShouldSample(call, 32, 120)) {
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=broadphase-update call={} object={:08X} "
        "state={:08X} bodies={} result={:08X}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), call, broadphase, state,
        body_count, ctx.r3.u32);
  }
}

extern "C" void sub_82982FE8(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_82982FE8(ctx, base);
    return;
  }
  const uint32_t first = ctx.r3.u32;
  const uint32_t second = ctx.r4.u32;
  __imp__sub_82982FE8(ctx, base);
  const uint64_t call = g_contact_pair_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  if (ShouldSample(call, 32, 2048)) {
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=contact-pair call={} first={:08X} "
        "second={:08X} result={:08X}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), call, first, second, ctx.r3.u32);
  }
}

extern "C" void sub_82869620(PPCContext& ctx, uint8_t* base) {
  if (!TraceEnabled()) {
    __imp__sub_82869620(ctx, base);
    return;
  }
  const uint32_t first_address = ctx.r3.u32;
  const uint32_t second_address = ctx.r4.u32;
  const uint32_t third_address = ctx.r5.u32;
  const uint32_t fourth_address = ctx.r6.u32;
  __imp__sub_82869620(ctx, base);
  const uint64_t call = g_geometry_test_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  if (ctx.r3.u32) {
    g_geometry_test_hits.fetch_add(1, std::memory_order_relaxed);
  }
  if (ShouldSample(call, 32, 4096)) {
    REXLOG_INFO(
        "gta4-physics: event={} tick={} phase=geometry-test call={} result={} "
        "a={:08X} b={:08X} c={:08X} d={:08X} totalHits={}",
        NextEvent(), g_tick_count.load(std::memory_order_relaxed), call, ctx.r3.u32, first_address,
        second_address, third_address, fourth_address,
        g_geometry_test_hits.load(std::memory_order_relaxed));
  }
}
