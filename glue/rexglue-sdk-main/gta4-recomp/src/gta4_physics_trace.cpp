#include <array>
#include <atomic>
#include <bit>
#include <cmath>
#include <cstdint>
#include <limits>

#include <rex/cvar.h>
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

thread_local uint32_t g_current_xbn_index = kNoXbnIndex;

bool TraceEnabled() {
  return REXCVAR_GET(gta4_trace_physics);
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
  __imp__sub_8294E6D0(ctx, base);
  const uint64_t call = g_deferred_insert_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  REXLOG_INFO(
      "gta4-physics: event={} tick={} phase=broadphase-insert-deferred call={} "
      "level={:08X} count={} bodies={:08X} ids={:08X} firstBody={:08X} firstId={:04X}",
      NextEvent(), g_tick_count.load(std::memory_order_relaxed), call, level, count, bodies, ids,
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
  }
  LogQueueViolation("queue-drain-before", before);
  LogQueueViolation("queue-drain-after", after);
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
