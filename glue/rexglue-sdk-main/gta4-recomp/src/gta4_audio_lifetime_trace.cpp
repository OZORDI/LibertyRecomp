#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <mutex>
#include <optional>
#include <string_view>
#include <thread>

#include <rex/logging.h>
#include <rex/audio/handoff_trace.h>
#include <rex/system/kernel_state.h>
#include <rex/thread.h>

#include "gta4_init.h"

namespace {

// All guest layout constants below were derived by
// /tmp/derive_audio_trace_constants.py from the generated implementations.
constexpr uint32_t kSourceVtable = 0x820BE570;
constexpr uint32_t kAllocatorHeaderSize = 0x1C;
constexpr uint32_t kOuterSourceSlotOffset = 0x13C;
constexpr uint32_t kAudioEnginePointerGlobal = 0x831D53EC;
constexpr uint32_t kAudioEngineListSentinelOffset = 0x50;
constexpr uint32_t kAudioEngineListNodeOffsetField = 0x58;
constexpr uint32_t kPublishIndexTable = 0x831C8BB0;
constexpr uint32_t kWorkerPoolRootGlobal = 0x831C8EC0;
constexpr uint32_t kWorkerPoolCountOffset = 0x28;
constexpr uint32_t kTitlePrimaryAllocatorGlobal = 0x831C8F94;
constexpr uint32_t kTitleSecondaryAllocatorGlobal = 0x831C8F90;
constexpr uint32_t kScriptRegistrationOffset = 0x39C0;
constexpr uint32_t kScriptRegistrationStride = 0x8;
constexpr uint32_t kScriptRegistrationCount = 0x14;
constexpr uint32_t kScriptSoundEntryOffset = 0x3384;
constexpr uint32_t kScriptSoundEntryStride = 0x10;
constexpr uint32_t kScriptSoundEntryCount = 0x64;
constexpr uint32_t kSoundFacadeHandleOffset = 0x4;
constexpr uint32_t kSoundFacadePartitionOffset = 0x40;
constexpr uint32_t kSoundFacadeTargetOffset = 0x90;
constexpr uint32_t kCrFrameBeginOffset = 0x14;
constexpr uint32_t kCrFrameEndOffset = 0x18;
constexpr uint32_t kCrFrameCapacityOffset = 0x1C;
constexpr uint32_t kCrFrameSecondOffset = 0x24;
constexpr uint32_t kInternalSelectorOffset = 0xE7;
constexpr uint32_t kInternalSelectedRecordStride = 0x20;
constexpr uint32_t kInternalSelectedRecordFlagOffset = 0x5F;
constexpr uint32_t kPartyStopCallSite = 0x8262AF34;
constexpr uint64_t kGuestAddressSpaceSize = 0x100000000ULL;
constexpr size_t kMaximumTrackedAllocators = 1024;
constexpr size_t kMaximumPartyStopEntries = 16;

enum RecordFlags : uint32_t {
  kAllocatorCreated = 1U << 0,
  kSourceConstructed = 1U << 1,
  kSourceLinked = 1U << 2,
  kFinalReleaseEntered = 1U << 3,
  kSourceDestructorEntered = 1U << 4,
  kAllocatorDestructorEntered = 1U << 5,
  kHeapFreeAttempted = 1U << 6,
  kHeapFreeReturned = 1U << 7,
  kAggregateDestructorReturned = 1U << 8,
  kPhysicalReuseSeen = 1U << 9,
};

enum TopologyFlags : uint32_t {
  kEnginePointerValid = 1U << 0,
  kSentinelValid = 1U << 1,
  kNodeValid = 1U << 2,
  kNextBacklinkValid = 1U << 3,
  kPreviousForwardLinkValid = 1U << 4,
};

struct LifetimeRecord {
  uint64_t generation = 0;
  uint32_t allocator = 0;
  uint32_t requested_size = 0;
  uint32_t backing = 0;
  uint32_t source = 0;
  uint32_t flags = 0;
};

struct GuestSnapshot {
  uint32_t vtable = 0;
  uint32_t source_ref = 0;
  uint32_t source_allocator = 0;
  uint32_t node_next = 0;
  uint32_t node_prev = 0;
  uint32_t engine = 0;
  uint32_t sentinel = 0;
  uint32_t node_offset = 0;
  uint32_t node = 0;
  uint32_t node_next_prev = 0;
  uint32_t node_prev_next = 0;
  uint32_t topology_flags = 0;
  uint32_t allocator_ref = 0;
  uint32_t allocator_size = 0;
  uint32_t allocator_used = 0;
  uint32_t allocator_backing = 0;
};

std::array<LifetimeRecord, kMaximumTrackedAllocators> g_records{};
std::atomic_flag g_records_lock = ATOMIC_FLAG_INIT;
std::atomic<uint64_t> g_sequence{0};
std::atomic<uint64_t> g_generation{0};
std::atomic<size_t> g_replacement_cursor{0};
std::atomic<uint64_t> g_party_transition_epoch{0};
std::atomic<uint64_t> g_last_publish_epoch{0};
std::atomic<uint64_t> g_last_worker_epoch{0};

bool IsAudioLifetimeTraceLoggingEnabled() {
  // Audio lifetime diagnostics are intentionally compiled out for renderer
  // investigation builds. Keep the publication locks and retail audio calls,
  // but emit no audio-lifetime records even if an inherited environment variable
  // requests them.
  return false;
}

// The retail XAudio wrappers serialize source calls with one global critical
// section, but audVoiceXenon reads its published source slot before entering
// that section. Cleanup also clears the slot only after Release returns. Keep
// every source-slot consumer and cleanup in one recursive publication domain so
// no caller can retain an unowned pointer across the final Release. Recursive
// locking is required because several of these generated methods call another
// method in the same audited set.
std::recursive_mutex g_audvoice_source_publication_mutex;

class AudVoiceSourcePublicationLock {
 public:
  explicit AudVoiceSourcePublicationLock(uint32_t pc=0)
      : pc_(pc),start_(rex::audio::handoff::Enabled()?rex::audio::handoff::Clock():0),lock_(g_audvoice_source_publication_mutex),acquired_(start_?rex::audio::handoff::Clock():0) {}
  ~AudVoiceSourcePublicationLock() {
    if(start_) {
      const auto finish=rex::audio::handoff::Clock();
      if(acquired_-start_>=1000000 || finish-acquired_>=1000000)
        rex::audio::handoff::Record("voice-lock",pc_,{acquired_-start_,finish-acquired_});
    }
  }
 private:
  uint32_t pc_=0;
  uint64_t start_=0;
  std::lock_guard<std::recursive_mutex> lock_;
  uint64_t acquired_=0;
};

class RecordsLock {
 public:
  RecordsLock() {
    while (g_records_lock.test_and_set(std::memory_order_acquire)) {
      std::this_thread::yield();
    }
  }

  ~RecordsLock() { g_records_lock.clear(std::memory_order_release); }
};

std::optional<LifetimeRecord> FindByAllocator(uint32_t allocator) {
  if (!allocator)
    return std::nullopt;
  RecordsLock lock;
  for (const auto& record : g_records) {
    if (record.allocator == allocator)
      return record;
  }
  return std::nullopt;
}

std::optional<LifetimeRecord> FindBySource(uint32_t source) {
  if (!source)
    return std::nullopt;
  RecordsLock lock;
  for (const auto& record : g_records) {
    if (record.source == source)
      return record;
  }
  return std::nullopt;
}

template <typename Predicate, typename Mutator>
std::optional<LifetimeRecord> UpdateRecord(Predicate&& predicate, Mutator&& mutator) {
  RecordsLock lock;
  for (auto& record : g_records) {
    if (!predicate(record))
      continue;
    mutator(record);
    return record;
  }
  return std::nullopt;
}

LifetimeRecord RegisterAllocator(uint32_t allocator, uint32_t requested_size, uint32_t backing) {
  RecordsLock lock;
  for (auto& record : g_records) {
    if (record.allocator != allocator)
      continue;
    record = LifetimeRecord{
        .generation = g_generation.fetch_add(1, std::memory_order_relaxed) + 1,
        .allocator = allocator,
        .requested_size = requested_size,
        .backing = backing,
        .flags = kAllocatorCreated,
    };
    return record;
  }
  for (auto& record : g_records) {
    if (record.allocator)
      continue;
    record = LifetimeRecord{
        .generation = g_generation.fetch_add(1, std::memory_order_relaxed) + 1,
        .allocator = allocator,
        .requested_size = requested_size,
        .backing = backing,
        .flags = kAllocatorCreated,
    };
    return record;
  }
  const size_t replacement =
      g_replacement_cursor.fetch_add(1, std::memory_order_relaxed) % g_records.size();
  g_records[replacement] = LifetimeRecord{
      .generation = g_generation.fetch_add(1, std::memory_order_relaxed) + 1,
      .allocator = allocator,
      .requested_size = requested_size,
      .backing = backing,
      .flags = kAllocatorCreated,
  };
  return g_records[replacement];
}

bool RangesOverlap(uint32_t allocation, uint32_t allocation_size, const LifetimeRecord& record) {
  if (!allocation || !allocation_size || !record.allocator)
    return false;
  const uint64_t allocation_begin = allocation;
  const uint64_t allocation_end = allocation_begin + allocation_size;
  const uint64_t watched_begin = record.allocator;
  const uint64_t watched_end = watched_begin + kAllocatorHeaderSize + record.requested_size;
  return allocation_begin < watched_end && watched_begin < allocation_end;
}

std::optional<LifetimeRecord> FindOverlap(uint32_t allocation, uint32_t allocation_size) {
  RecordsLock lock;
  for (const auto& record : g_records) {
    if (RangesOverlap(allocation, allocation_size, record))
      return record;
  }
  return std::nullopt;
}

std::optional<LifetimeRecord> FindContaining(uint32_t allocation) {
  if (!allocation)
    return std::nullopt;
  RecordsLock lock;
  for (const auto& record : g_records) {
    if (!record.allocator)
      continue;
    const uint64_t begin = record.allocator;
    const uint64_t end = begin + kAllocatorHeaderSize + record.requested_size;
    if (allocation >= begin && allocation < end)
      return record;
  }
  return std::nullopt;
}

bool IsGuestRange(uint32_t address, uint64_t size) {
  if (!address || !size || size > kGuestAddressSpaceSize)
    return false;
  const uint64_t end = static_cast<uint64_t>(address) + size - 1;
  if (end > UINT32_MAX)
    return false;

  auto* kernel_state = REX_KERNEL_STATE();
  auto* memory = kernel_state ? kernel_state->memory() : nullptr;
  auto* heap = memory ? memory->LookupHeap(address) : nullptr;
  if (!heap || heap != memory->LookupHeap(static_cast<uint32_t>(end)))
    return false;

  return heap->QueryRangeAccess(address, static_cast<uint32_t>(end)) !=
         rex::memory::PageAccess::kNoAccess;
}

bool CheckedGuestAdd(uint32_t address, uint32_t offset, uint64_t size, uint32_t* result) {
  if (!address || !result)
    return false;
  const uint64_t candidate = static_cast<uint64_t>(address) + offset;
  if (candidate > UINT32_MAX || !IsGuestRange(static_cast<uint32_t>(candidate), size))
    return false;
  *result = static_cast<uint32_t>(candidate);
  return true;
}

void CaptureListTopology(uint8_t* base, const LifetimeRecord& record, GuestSnapshot* snapshot) {
  if (!record.source || !IsGuestRange(kAudioEnginePointerGlobal, sizeof(uint32_t)))
    return;
  snapshot->engine = REX_LOAD_U32(kAudioEnginePointerGlobal);
  if (!IsGuestRange(snapshot->engine, kAudioEngineListNodeOffsetField + sizeof(uint32_t)))
    return;
  snapshot->topology_flags |= kEnginePointerValid;
  if (!CheckedGuestAdd(snapshot->engine, kAudioEngineListSentinelOffset, sizeof(uint64_t),
                       &snapshot->sentinel))
    return;
  snapshot->topology_flags |= kSentinelValid;
  snapshot->node_offset = REX_LOAD_U32(snapshot->engine + kAudioEngineListNodeOffsetField);
  if (!CheckedGuestAdd(record.source, snapshot->node_offset, sizeof(uint64_t), &snapshot->node))
    return;
  snapshot->topology_flags |= kNodeValid;
  snapshot->node_next = REX_LOAD_U32(snapshot->node);
  snapshot->node_prev = REX_LOAD_U32(snapshot->node + sizeof(uint32_t));

  uint32_t backlink = 0;
  if (CheckedGuestAdd(snapshot->node_next, sizeof(uint32_t), sizeof(uint32_t), &backlink)) {
    snapshot->node_next_prev = REX_LOAD_U32(backlink);
    snapshot->topology_flags |= kNextBacklinkValid;
  }
  if (IsGuestRange(snapshot->node_prev, sizeof(uint32_t))) {
    snapshot->node_prev_next = REX_LOAD_U32(snapshot->node_prev);
    snapshot->topology_flags |= kPreviousForwardLinkValid;
  }
}

bool SourceLinkIsProven(const GuestSnapshot& snapshot) {
  const uint32_t required = kEnginePointerValid | kSentinelValid | kNodeValid | kNextBacklinkValid |
                            kPreviousForwardLinkValid;
  return (snapshot.topology_flags & required) == required &&
         snapshot.node_next_prev == snapshot.node && snapshot.node_prev_next == snapshot.node;
}

bool SourceUnlinkIsProven(uint8_t* base, const GuestSnapshot& before, const GuestSnapshot& after) {
  if (!(after.topology_flags & kNodeValid) || !after.node || after.node != before.node ||
      after.node_next != after.node || after.node_prev != after.node)
    return false;
  uint32_t old_next_backlink = 0;
  if (!CheckedGuestAdd(before.node_next, sizeof(uint32_t), sizeof(uint32_t), &old_next_backlink) ||
      !IsGuestRange(before.node_prev, sizeof(uint32_t)))
    return false;
  return REX_LOAD_U32(old_next_backlink) == before.node_prev &&
         REX_LOAD_U32(before.node_prev) == before.node_next;
}

GuestSnapshot CaptureSnapshot(uint8_t* base, const LifetimeRecord& record) {
  GuestSnapshot snapshot{};
  if (IsGuestRange(record.source, 24)) {
    snapshot.vtable = REX_LOAD_U32(record.source);
    snapshot.source_ref = REX_LOAD_U32(record.source + 4);
    snapshot.source_allocator = REX_LOAD_U32(record.source + 8);
  }
  CaptureListTopology(base, record, &snapshot);
  if (IsGuestRange(record.allocator, kAllocatorHeaderSize)) {
    snapshot.allocator_ref = REX_LOAD_U32(record.allocator + 8);
    snapshot.allocator_size = REX_LOAD_U32(record.allocator + 16);
    snapshot.allocator_used = REX_LOAD_U32(record.allocator + 20);
    snapshot.allocator_backing = REX_LOAD_U32(record.allocator + 24);
  }
  return snapshot;
}

void LogRecord(std::string_view point, uint32_t function, uint32_t caller,
               const LifetimeRecord& record, const GuestSnapshot& snapshot, uint32_t argument0 = 0,
               uint32_t argument1 = 0, uint32_t argument2 = 0, uint32_t argument3 = 0,
               uint32_t argument4 = 0, uint32_t argument5 = 0) {
  if (!IsAudioLifetimeTraceLoggingEnabled()) {
    return;
  }
  const uint64_t sequence = g_sequence.fetch_add(1, std::memory_order_relaxed) + 1;
  REXSYS_INFO(
      "audio-lifetime seq={} point={} thread={} function={:08X} caller={:08X} "
      "generation={} source={:08X} vtable={:08X} source_ref={} "
      "source_allocator={:08X} node_next={:08X} node_prev={:08X} "
      "engine={:08X} sentinel={:08X} node_offset={:08X} node={:08X} "
      "next_prev={:08X} prev_next={:08X} topology={:08X} "
      "allocator={:08X} allocator_ref={} allocator_size={} allocator_used={} "
      "allocator_backing={:08X} registered_backing={:08X} requested={} "
      "flags={:08X} arg0={:08X} arg1={:08X} arg2={:08X} arg3={:08X} "
      "arg4={:08X} arg5={:08X}",
      sequence, point, rex::thread::current_thread_system_id(), function, caller, record.generation,
      record.source, snapshot.vtable, snapshot.source_ref, snapshot.source_allocator,
      snapshot.node_next, snapshot.node_prev, snapshot.engine, snapshot.sentinel,
      snapshot.node_offset, snapshot.node, snapshot.node_next_prev, snapshot.node_prev_next,
      snapshot.topology_flags, record.allocator, snapshot.allocator_ref, snapshot.allocator_size,
      snapshot.allocator_used, snapshot.allocator_backing, record.backing, record.requested_size,
      record.flags, argument0, argument1, argument2, argument3, argument4, argument5);
}

void LogRecord(std::string_view point, uint32_t function, uint32_t caller, uint8_t* base,
               const LifetimeRecord& record, uint32_t argument0 = 0, uint32_t argument1 = 0,
               uint32_t argument2 = 0, uint32_t argument3 = 0, uint32_t argument4 = 0,
               uint32_t argument5 = 0) {
  if (!IsAudioLifetimeTraceLoggingEnabled()) {
    return;
  }
  LogRecord(point, function, caller, record, CaptureSnapshot(base, record), argument0, argument1,
            argument2, argument3, argument4, argument5);
}

void LogCheckpoint(std::string_view point, uint32_t function, uint32_t caller,
                   uint32_t argument0 = 0, uint32_t argument1 = 0, uint32_t argument2 = 0,
                   uint32_t argument3 = 0, uint32_t argument4 = 0, uint32_t argument5 = 0) {
  if (!IsAudioLifetimeTraceLoggingEnabled()) {
    return;
  }
  LogRecord(point, function, caller, LifetimeRecord{}, GuestSnapshot{}, argument0, argument1,
            argument2, argument3, argument4, argument5);
}

void LogMethodAnomaly(std::string_view point, uint32_t function, uint32_t caller, uint8_t* base,
                      uint32_t source) {
  auto record = FindBySource(source);
  if (!record)
    return;
  const GuestSnapshot snapshot = CaptureSnapshot(base, *record);
  const bool lifetime_ended =
      (record->flags & (kFinalReleaseEntered | kSourceDestructorEntered |
                        kAllocatorDestructorEntered | kHeapFreeAttempted | kHeapFreeReturned)) != 0;
  if (!lifetime_ended && snapshot.vtable == kSourceVtable)
    return;
  LogRecord(point, function, caller, *record, snapshot, source, snapshot.vtable);
}

void LogHeapReuse(std::string_view point, uint32_t function, uint32_t caller, uint8_t* base,
                  uint32_t allocation, uint32_t allocation_size) {
  auto record = FindOverlap(allocation, allocation_size);
  if (!record)
    return;
  record = UpdateRecord(
      [allocator = record->allocator](const LifetimeRecord& candidate) {
        return candidate.allocator == allocator;
      },
      [](LifetimeRecord& candidate) { candidate.flags |= kPhysicalReuseSeen; });
  if (record)
    LogRecord(point, function, caller, base, *record, allocation, allocation_size);
}

std::optional<LifetimeRecord> NoteHeapFreeAttempt(std::string_view point, uint32_t function,
                                                  uint32_t caller, uint8_t* base,
                                                  uint32_t allocation, uint32_t flags,
                                                  uint32_t owner0 = 0, uint32_t owner1 = 0) {
  auto containing = FindContaining(allocation);
  if (!containing)
    return std::nullopt;
  auto record = UpdateRecord(
      [allocator = containing->allocator](const LifetimeRecord& candidate) {
        return candidate.allocator == allocator;
      },
      [](LifetimeRecord& candidate) { candidate.flags |= kHeapFreeAttempted; });
  if (record)
    LogRecord(point, function, caller, base, *record, allocation, flags, owner0, owner1);
  return record;
}

void NoteHeapFreeReturn(std::string_view point, uint32_t function, uint32_t caller,
                        const LifetimeRecord& previous, uint32_t allocation, uint32_t flags) {
  auto record = UpdateRecord(
      [allocator = previous.allocator](const LifetimeRecord& candidate) {
        return candidate.allocator == allocator;
      },
      [](LifetimeRecord& candidate) { candidate.flags |= kHeapFreeReturned; });
  if (record)
    LogRecord(point, function, caller, *record, GuestSnapshot{}, allocation, flags);
}

struct PartyStopEntrySnapshot {
  uint32_t entry = 0;
  uint32_t token = 0;
  uint32_t sound = 0;
  uint32_t owner = 0;
  uint32_t flag = 0;
};

struct InternalPublishSnapshot {
  uint32_t internal = 0;
  uint32_t selector = 0;
  uint32_t selected_index = 0;
  uint32_t selected_record = 0;
  uint32_t selected_flag = 0;
  uint32_t valid = 0;
};

struct PublishWindowSnapshot {
  uint32_t manager = 0;
  uint32_t selector = 0;
  uint32_t producer_index = 0;
  uint32_t consumer_index = 0;
  uint32_t producer_record = 0;
  uint32_t consumer_record = 0;
};

struct WorkerPoolSnapshot {
  uint32_t output = 0;
  uint32_t root = 0;
  uint32_t table = 0;
  uint32_t count = 0;
};

uint64_t ClaimPartyEpoch(std::atomic<uint64_t>* last_epoch) {
  const uint64_t epoch = g_party_transition_epoch.load(std::memory_order_acquire);
  if (!epoch)
    return 0;
  uint64_t observed = last_epoch->load(std::memory_order_relaxed);
  while (observed != epoch) {
    if (last_epoch->compare_exchange_weak(observed, epoch, std::memory_order_acq_rel,
                                          std::memory_order_relaxed))
      return epoch;
  }
  return 0;
}

InternalPublishSnapshot CaptureInternalPublish(uint8_t* base, uint32_t internal) {
  InternalPublishSnapshot snapshot{.internal = internal};
  if (!IsGuestRange(internal, kInternalSelectorOffset + sizeof(uint8_t)))
    return snapshot;
  snapshot.selector = REX_LOAD_U8(internal + kInternalSelectorOffset);
  const uint32_t table_offset = (snapshot.selector >> 5) * sizeof(uint64_t);
  uint32_t table_entry = 0;
  if (!CheckedGuestAdd(kPublishIndexTable, table_offset, sizeof(uint32_t), &table_entry))
    return snapshot;
  snapshot.selected_index = REX_LOAD_U32(table_entry);
  const uint64_t selected_offset =
      static_cast<uint64_t>(snapshot.selected_index) * kInternalSelectedRecordStride;
  if (selected_offset > UINT32_MAX)
    return snapshot;
  if (!CheckedGuestAdd(internal, static_cast<uint32_t>(selected_offset),
                       kInternalSelectedRecordFlagOffset + sizeof(uint8_t),
                       &snapshot.selected_record))
    return snapshot;
  snapshot.selected_flag =
      REX_LOAD_U8(snapshot.selected_record + kInternalSelectedRecordFlagOffset);
  snapshot.valid = 1;
  return snapshot;
}

PublishWindowSnapshot CapturePublishWindow(uint8_t* base, uint32_t manager) {
  PublishWindowSnapshot snapshot{.manager = manager};
  if (!IsGuestRange(manager, kInternalSelectorOffset + sizeof(uint8_t)))
    return snapshot;
  snapshot.selector = REX_LOAD_U8(manager + kInternalSelectorOffset);
  const uint32_t table_offset = (snapshot.selector >> 5) * sizeof(uint64_t);
  uint32_t table_entry = 0;
  if (!CheckedGuestAdd(kPublishIndexTable, table_offset, sizeof(uint64_t), &table_entry))
    return snapshot;
  snapshot.producer_index = REX_LOAD_U32(table_entry);
  snapshot.consumer_index = REX_LOAD_U32(table_entry + sizeof(uint32_t));
  const uint64_t producer_offset =
      static_cast<uint64_t>(snapshot.producer_index) * kInternalSelectedRecordStride;
  const uint64_t consumer_offset =
      static_cast<uint64_t>(snapshot.consumer_index) * kInternalSelectedRecordStride;
  if (producer_offset <= UINT32_MAX) {
    CheckedGuestAdd(manager, static_cast<uint32_t>(producer_offset), sizeof(uint32_t),
                    &snapshot.producer_record);
  }
  if (consumer_offset <= UINT32_MAX) {
    CheckedGuestAdd(manager, static_cast<uint32_t>(consumer_offset), sizeof(uint32_t),
                    &snapshot.consumer_record);
  }
  return snapshot;
}

WorkerPoolSnapshot CaptureWorkerPool(uint8_t* base, uint32_t output) {
  WorkerPoolSnapshot snapshot{.output = output};
  if (!IsGuestRange(kWorkerPoolRootGlobal, sizeof(uint32_t)))
    return snapshot;
  snapshot.root = REX_LOAD_U32(kWorkerPoolRootGlobal);
  if (!IsGuestRange(snapshot.root, kWorkerPoolCountOffset + sizeof(uint32_t)))
    return snapshot;
  snapshot.table = REX_LOAD_U32(snapshot.root);
  snapshot.count = REX_LOAD_U32(snapshot.root + kWorkerPoolCountOffset);
  return snapshot;
}

void TraceCrFrameArray(uint8_t* base, uint32_t caller, uint32_t frame, uint32_t data_source,
                       uint32_t array_index) {
  if (!IsGuestRange(frame, kCrFrameCapacityOffset + sizeof(uint32_t)))
    return;
  const uint32_t begin = REX_LOAD_U32(frame + kCrFrameBeginOffset);
  const uint32_t end = REX_LOAD_U32(frame + kCrFrameEndOffset);
  const uint32_t capacity = REX_LOAD_U32(frame + kCrFrameCapacityOffset);
  if (end <= begin)
    return;
  auto record = FindOverlap(begin, end - begin);
  if (!record)
    return;
  record = UpdateRecord(
      [allocator = record->allocator](const LifetimeRecord& candidate) {
        return candidate.allocator == allocator;
      },
      [](LifetimeRecord& candidate) { candidate.flags |= kPhysicalReuseSeen; });
  if (record) {
    LogRecord("crframe-array-reuse", 0x82444A30, caller, base, *record, frame, begin, end, capacity,
              data_source, array_index);
  }
}

}  // namespace

extern "C" void sub_8219A350(PPCContext& ctx, uint8_t* base) {
  const uint32_t caller = ctx.lr;
  const uint32_t allocation_flags = ctx.r3.u32;
  const uint32_t requested_size = ctx.r4.u32;
  const uint32_t output_pointer = ctx.r5.u32;
  __imp__sub_8219A350(ctx, base);
  if (ctx.r3.s32 < 0 || !IsGuestRange(output_pointer, sizeof(uint32_t)))
    return;
  const uint32_t allocator = REX_LOAD_U32(output_pointer);
  if (!IsGuestRange(allocator, kAllocatorHeaderSize))
    return;
  const uint32_t backing = REX_LOAD_U32(allocator + 24);
  const auto record = RegisterAllocator(allocator, requested_size, backing);
  LogRecord("allocator-create", 0x8219A350, caller, base, record, allocation_flags, output_pointer);
}

extern "C" void sub_8219A5C8(PPCContext& ctx, uint8_t* base) {
  const uint32_t allocator = ctx.r3.u32;
  const uint32_t requested_size = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  const auto record = FindByAllocator(allocator);
  __imp__sub_8219A5C8(ctx, base);
  if (record) {
    LogRecord("allocator-linear-alloc", 0x8219A5C8, caller, base, *record, requested_size,
              ctx.r3.u32);
  }
}

extern "C" void sub_82192BC8(PPCContext& ctx, uint8_t* base) {
  const uint32_t source = ctx.r3.u32;
  const uint32_t allocator = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  __imp__sub_82192BC8(ctx, base);
  auto record = UpdateRecord(
      [allocator](const LifetimeRecord& candidate) { return candidate.allocator == allocator; },
      [source](LifetimeRecord& candidate) {
        candidate.source = source;
        candidate.flags |= kSourceConstructed;
      });
  if (record) {
    const GuestSnapshot after = CaptureSnapshot(base, *record);
    const bool link_proven = SourceLinkIsProven(after);
    if (link_proven) {
      record = UpdateRecord(
          [source](const LifetimeRecord& candidate) { return candidate.source == source; },
          [](LifetimeRecord& candidate) { candidate.flags |= kSourceLinked; });
    }
    if (record)
      LogRecord("source-ctor-exit", 0x82192BC8, caller, *record, after, source,
                link_proven ? 1 : 0);
  }
}

extern "C" void sub_82192E40(PPCContext& ctx, uint8_t* base) {
  const uint32_t source = ctx.r3.u32;
  const uint32_t configuration = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  const auto record = FindBySource(source);
  if (record) {
    LogRecord("source-initialize-enter", 0x82192E40, caller, base, *record, configuration);
  }
  __imp__sub_82192E40(ctx, base);
  if (record) {
    LogRecord("source-initialize-exit", 0x82192E40, caller, base, *record, configuration,
              ctx.r3.u32);
  }
}

extern "C" void sub_821929C8(PPCContext& ctx, uint8_t* base) {
  const uint32_t configuration = ctx.r3.u32;
  const uint32_t flags = ctx.r4.u32;
  const uint32_t output_slot = ctx.r5.u32;
  const uint32_t caller = ctx.lr;
  __imp__sub_821929C8(ctx, base);
  if (ctx.r3.s32 < 0 || !IsGuestRange(output_slot, sizeof(uint32_t)))
    return;
  const uint32_t source = REX_LOAD_U32(output_slot);
  const auto record = FindBySource(source);
  if (record) {
    LogRecord("source-acquire-published", 0x821929C8, caller, base, *record, configuration, flags,
              output_slot, source, ctx.r3.u32);
  }
}

extern "C" void sub_829335B0(PPCContext& ctx, uint8_t* base) {
  const uint32_t source = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  const auto record = FindBySource(source);
  const GuestSnapshot before = record ? CaptureSnapshot(base, *record) : GuestSnapshot{};
  __imp__sub_829335B0(ctx, base);
  if (record) {
    LogRecord("source-addref", 0x829335B0, caller, *record, before, before.source_ref, ctx.r3.u32);
  }
}

extern "C" void sub_82192460(PPCContext& ctx, uint8_t* base) {
  const uint32_t source = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  const auto record = FindBySource(source);
  const GuestSnapshot before = record ? CaptureSnapshot(base, *record) : GuestSnapshot{};
  if (record) {
    LogRecord("source-release-enter", 0x82192460, caller, *record, before, before.source_ref, 0);
  }
  __imp__sub_82192460(ctx, base);
  if (record && before.source_ref > 1) {
    LogRecord("source-release-exit", 0x82192460, caller, base, *record, before.source_ref,
              ctx.r3.u32);
  }
}

extern "C" void sub_8218FF28(PPCContext& ctx, uint8_t* base) {
  const uint32_t source = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  auto record =
      UpdateRecord([source](const LifetimeRecord& candidate) { return candidate.source == source; },
                   [](LifetimeRecord& candidate) { candidate.flags |= kFinalReleaseEntered; });
  if (record) {
    LogRecord("source-final-enter", 0x8218FF28, caller, base, *record);
  }
  __imp__sub_8218FF28(ctx, base);
  if (record) {
    LogRecord("source-final-exit", 0x8218FF28, caller, *record, GuestSnapshot{}, source,
              ctx.r3.u32);
  }
}

extern "C" void sub_82192CF0(PPCContext& ctx, uint8_t* base) {
  const uint32_t source = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  auto record =
      UpdateRecord([source](const LifetimeRecord& candidate) { return candidate.source == source; },
                   [](LifetimeRecord& candidate) { candidate.flags |= kSourceDestructorEntered; });
  const GuestSnapshot before = record ? CaptureSnapshot(base, *record) : GuestSnapshot{};
  if (record) {
    LogRecord("source-dtor-enter", 0x82192CF0, caller, *record, before);
  }
  __imp__sub_82192CF0(ctx, base);
  const GuestSnapshot after = record ? CaptureSnapshot(base, *record) : GuestSnapshot{};
  const bool unlink_proven = record && SourceUnlinkIsProven(base, before, after);
  if (unlink_proven) {
    record = UpdateRecord(
        [source](const LifetimeRecord& candidate) { return candidate.source == source; },
        [](LifetimeRecord& candidate) { candidate.flags &= ~kSourceLinked; });
  }
  if (record) {
    LogRecord("source-dtor-exit", 0x82192CF0, caller, *record, after, source,
              unlink_proven ? 1 : 0);
  }
}

extern "C" void sub_8219FE60(PPCContext& ctx, uint8_t* base) {
  const uint32_t allocator = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  const auto record = FindByAllocator(allocator);
  const GuestSnapshot before = record ? CaptureSnapshot(base, *record) : GuestSnapshot{};
  __imp__sub_8219FE60(ctx, base);
  if (record) {
    LogRecord("allocator-addref", 0x8219FE60, caller, base, *record, before.allocator_ref,
              ctx.r3.u32);
  }
}

extern "C" void sub_82194438(PPCContext& ctx, uint8_t* base) {
  const uint32_t allocator = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  const auto record = FindByAllocator(allocator);
  const GuestSnapshot before = record ? CaptureSnapshot(base, *record) : GuestSnapshot{};
  if (record) {
    LogRecord("allocator-release-enter", 0x82194438, caller, *record, before, before.allocator_ref,
              0);
  }
  __imp__sub_82194438(ctx, base);
  if (record && before.allocator_ref > 1) {
    LogRecord("allocator-release-exit", 0x82194438, caller, base, *record, before.allocator_ref,
              ctx.r3.u32);
  }
}

extern "C" void sub_8219A4B8(PPCContext& ctx, uint8_t* base) {
  const uint32_t allocator = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  auto record = UpdateRecord(
      [allocator](const LifetimeRecord& candidate) { return candidate.allocator == allocator; },
      [](LifetimeRecord& candidate) { candidate.flags |= kAllocatorDestructorEntered; });
  if (record) {
    LogRecord("allocator-dtor-enter", 0x8219A4B8, caller, base, *record, ctx.r4.u32, 0);
  }
  __imp__sub_8219A4B8(ctx, base);
  if (record) {
    LogRecord("allocator-dtor-exit", 0x8219A4B8, caller, *record, GuestSnapshot{}, allocator, 0);
  }
}

extern "C" void sub_8219A6F8(PPCContext& ctx, uint8_t* base) {
  const uint32_t aggregate_interface = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  const uint32_t allocator =
      aggregate_interface >= sizeof(uint32_t) ? aggregate_interface - sizeof(uint32_t) : 0;
  const auto record = FindByAllocator(allocator);
  uint32_t allocation_tag = 0;
  uint32_t tag_address = 0;
  if (CheckedGuestAdd(aggregate_interface, sizeof(uint64_t), sizeof(uint32_t), &tag_address))
    allocation_tag = REX_LOAD_U32(tag_address);
  if (record) {
    LogRecord("aggregate-dtor-enter", 0x8219A6F8, caller, base, *record, aggregate_interface,
              allocation_tag);
  }
  __imp__sub_8219A6F8(ctx, base);
  if (!record)
    return;
  auto returned = UpdateRecord(
      [allocator](const LifetimeRecord& candidate) { return candidate.allocator == allocator; },
      [](LifetimeRecord& candidate) { candidate.flags |= kAggregateDestructorReturned; });
  if (returned) {
    LogRecord("aggregate-dtor-return", 0x8219A6F8, caller, *returned, GuestSnapshot{},
              aggregate_interface, allocation_tag);
  }
}

extern "C" void sub_821B3608(PPCContext& ctx, uint8_t* base) {
  const uint32_t requested_size = ctx.r3.u32;
  const uint32_t flags = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  __imp__sub_821B3608(ctx, base);
  LogHeapReuse("aggregate-alloc-reuse", 0x821B3608, caller, base, ctx.r3.u32, requested_size);
  (void)flags;
}

extern "C" void sub_821B3510(PPCContext& ctx, uint8_t* base) {
  const uint32_t requested_size = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  __imp__sub_821B3510(ctx, base);
  LogHeapReuse("default-alloc-reuse", 0x821B3510, caller, base, ctx.r3.u32, requested_size);
}

extern "C" void sub_821B3538(PPCContext& ctx, uint8_t* base) {
  const uint32_t requested_size = ctx.r3.u32;
  const uint32_t alignment = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  __imp__sub_821B3538(ctx, base);
  LogHeapReuse("default-aligned-alloc-reuse", 0x821B3538, caller, base, ctx.r3.u32, requested_size);
  (void)alignment;
}

extern "C" void sub_8291F2C8(PPCContext& ctx, uint8_t* base) {
  const uint32_t requested_size = ctx.r3.u32;
  const uint32_t flags = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  __imp__sub_8291F2C8(ctx, base);
  LogHeapReuse("title-heap-alloc-reuse", 0x8291F2C8, caller, base, ctx.r3.u32, requested_size);
  (void)flags;
}

extern "C" void sub_821B3700(PPCContext& ctx, uint8_t* base) {
  const uint32_t allocation = ctx.r3.u32;
  const uint32_t flags = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  const auto record =
      NoteHeapFreeAttempt("aggregate-free-attempt", 0x821B3700, caller, base, allocation, flags);
  __imp__sub_821B3700(ctx, base);
  if (record)
    NoteHeapFreeReturn("aggregate-free-return", 0x821B3700, caller, *record, allocation, flags);
}

extern "C" void sub_821B3560(PPCContext& ctx, uint8_t* base) {
  const uint32_t allocation = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  const auto record =
      NoteHeapFreeAttempt("default-free-attempt", 0x821B3560, caller, base, allocation, 0);
  __imp__sub_821B3560(ctx, base);
  if (record)
    NoteHeapFreeReturn("default-free-return", 0x821B3560, caller, *record, allocation, 0);
}

extern "C" void sub_8291F330(PPCContext& ctx, uint8_t* base) {
  const uint32_t allocation = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  const uint32_t primary = IsGuestRange(kTitlePrimaryAllocatorGlobal, sizeof(uint32_t))
                               ? REX_LOAD_U32(kTitlePrimaryAllocatorGlobal)
                               : 0;
  const uint32_t secondary = IsGuestRange(kTitleSecondaryAllocatorGlobal, sizeof(uint32_t))
                                 ? REX_LOAD_U32(kTitleSecondaryAllocatorGlobal)
                                 : 0;
  NoteHeapFreeAttempt("title-free-attempt", 0x8291F330, caller, base, allocation, 0, primary,
                      secondary);
  __imp__sub_8291F330(ctx, base);
}

extern "C" void sub_8218F700(PPCContext& ctx, uint8_t* base) {
  const uint32_t output_slot = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  __imp__sub_8218F700(ctx, base);
  if (ctx.r3.s32 < 0 || !IsGuestRange(output_slot, sizeof(uint32_t)))
    return;
  const uint32_t source = REX_LOAD_U32(output_slot);
  auto record = FindBySource(source);
  if (record) {
    LogRecord("outer-source-published", 0x8218F700, caller, base, *record, output_slot, source);
  }
}

extern "C" void sub_8292FAB8(PPCContext& ctx, uint8_t* base) {
  // This initializer publishes outer+316 through sub_8218F700 and then
  // immediately reloads and uses the published source. It must participate in
  // the same domain as cleanup and every consumer; locking sub_8218F700 alone
  // would still leave the post-publication use exposed to concurrent cleanup.
  AudVoiceSourcePublicationLock publication_lock(0x8292FAB8);
  __imp__sub_8292FAB8(ctx, base);
}

extern "C" void sub_8292F0E8(PPCContext& ctx, uint8_t* base) {
  AudVoiceSourcePublicationLock publication_lock(0x8292F0E8);
  const uint32_t outer = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  uint32_t source_slot = 0;
  const bool source_slot_valid =
      CheckedGuestAdd(outer, kOuterSourceSlotOffset, sizeof(uint32_t), &source_slot);
  const uint32_t source = source_slot_valid ? REX_LOAD_U32(source_slot) : 0;
  const auto record = FindBySource(source);
  if (record) {
    LogRecord("outer-cleanup-enter", 0x8292F0E8, caller, base, *record, outer, source);
  }
  __imp__sub_8292F0E8(ctx, base);
  if (record) {
    const uint32_t current_source = source_slot_valid ? REX_LOAD_U32(source_slot) : 0;
    LogRecord("outer-cleanup-exit", 0x8292F0E8, caller, *record, GuestSnapshot{}, outer,
              current_source);
  }
}

extern "C" void sub_8218EDB8(PPCContext& ctx, uint8_t* base) {
  const uint32_t source = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  const auto record = FindBySource(source);
  if (record) {
    LogRecord("locked-release-enter", 0x8218EDB8, caller, base, *record, source, 0);
  }
  __imp__sub_8218EDB8(ctx, base);
  if (record) {
    LogRecord("locked-release-exit", 0x8218EDB8, caller, *record, GuestSnapshot{}, source,
              ctx.r3.u32);
  }
}

extern "C" void sub_829304A0(PPCContext& ctx, uint8_t* base) {
  AudVoiceSourcePublicationLock publication_lock(0x829304A0);
  const uint32_t outer = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  uint32_t source_slot = 0;
  const bool source_slot_valid =
      CheckedGuestAdd(outer, kOuterSourceSlotOffset, sizeof(uint32_t), &source_slot);
  const uint32_t source = source_slot_valid ? REX_LOAD_U32(source_slot) : 0;
  LogMethodAnomaly("outer-update-stale-slot", 0x829304A0, caller, base, source);
  __imp__sub_829304A0(ctx, base);
}

extern "C" void sub_8292F130(PPCContext& ctx, uint8_t* base) {
  AudVoiceSourcePublicationLock publication_lock(0x8292F130);
  __imp__sub_8292F130(ctx, base);
}

extern "C" void sub_8292F250(PPCContext& ctx, uint8_t* base) {
  AudVoiceSourcePublicationLock publication_lock(0x8292F250);
  __imp__sub_8292F250(ctx, base);
}

extern "C" void sub_8292F2C0(PPCContext& ctx, uint8_t* base) {
  AudVoiceSourcePublicationLock publication_lock(0x8292F2C0);
  __imp__sub_8292F2C0(ctx, base);
}

extern "C" void sub_8292FDE0(PPCContext& ctx, uint8_t* base) {
  AudVoiceSourcePublicationLock publication_lock(0x8292FDE0);
  __imp__sub_8292FDE0(ctx, base);
}

extern "C" void sub_8292FFD0(PPCContext& ctx, uint8_t* base) {
  AudVoiceSourcePublicationLock publication_lock(0x8292FFD0);
  __imp__sub_8292FFD0(ctx, base);
}

extern "C" void sub_82930078(PPCContext& ctx, uint8_t* base) {
  AudVoiceSourcePublicationLock publication_lock(0x82930078);
  __imp__sub_82930078(ctx, base);
}

extern "C" void sub_829302A8(PPCContext& ctx, uint8_t* base) {
  AudVoiceSourcePublicationLock publication_lock(0x829302A8);
  __imp__sub_829302A8(ctx, base);
}

extern "C" void sub_8218EE20(PPCContext& ctx, uint8_t* base) {
  const uint32_t source = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  LogMethodAnomaly("source-method32-stale", 0x8218EE20, caller, base, source);
  __imp__sub_8218EE20(ctx, base);
}

extern "C" void sub_8218F0B8(PPCContext& ctx, uint8_t* base) {
  const uint32_t source = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  LogMethodAnomaly("source-method60-stale", 0x8218F0B8, caller, base, source);
  __imp__sub_8218F0B8(ctx, base);
}

extern "C" void sub_8262AE58(PPCContext& ctx, uint8_t* base) {
  const uint32_t manager = ctx.r3.u32;
  const uint32_t script_id = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  const uint64_t epoch = g_party_transition_epoch.fetch_add(1, std::memory_order_acq_rel) + 1;
  std::array<PartyStopEntrySnapshot, kMaximumPartyStopEntries> entries{};
  size_t captured = 0;
  uint32_t total = 0;
  uint32_t script_index = UINT32_MAX;

  for (uint32_t index = 0; index < kScriptRegistrationCount; ++index) {
    const uint64_t offset = static_cast<uint64_t>(kScriptRegistrationOffset) +
                            static_cast<uint64_t>(index) * kScriptRegistrationStride;
    if (offset > UINT32_MAX)
      break;
    uint32_t registration = 0;
    if (!CheckedGuestAdd(manager, static_cast<uint32_t>(offset), sizeof(uint32_t), &registration))
      break;
    if (REX_LOAD_U32(registration) == script_id) {
      script_index = index;
      break;
    }
  }

  if (script_index != UINT32_MAX) {
    for (uint32_t index = 0; index < kScriptSoundEntryCount; ++index) {
      const uint64_t offset = static_cast<uint64_t>(kScriptSoundEntryOffset) +
                              static_cast<uint64_t>(index) * kScriptSoundEntryStride;
      if (offset > UINT32_MAX)
        break;
      uint32_t entry = 0;
      if (!CheckedGuestAdd(manager, static_cast<uint32_t>(offset), 12, &entry))
        break;
      if (REX_LOAD_U32(entry + sizeof(uint32_t)) != script_index)
        continue;
      ++total;
      if (captured >= entries.size())
        continue;
      PartyStopEntrySnapshot& snapshot = entries[captured++];
      snapshot.entry = entry;
      snapshot.sound = REX_LOAD_U32(entry);
      snapshot.owner = REX_LOAD_U32(entry + sizeof(uint32_t));
      snapshot.flag = REX_LOAD_U8(entry + sizeof(uint64_t));
      if (entry >= sizeof(uint32_t) && IsGuestRange(entry - sizeof(uint32_t), sizeof(uint32_t))) {
        snapshot.token = REX_LOAD_U32(entry - sizeof(uint32_t));
      }
    }
  }

  LogCheckpoint("party-stop-enter", 0x8262AE58, caller, manager, script_id, script_index, total,
                static_cast<uint32_t>(captured), static_cast<uint32_t>(epoch));
  for (size_t index = 0; index < captured; ++index) {
    const auto& snapshot = entries[index];
    LogCheckpoint("party-stop-entry-before", 0x8262AE58, caller, snapshot.entry, snapshot.token,
                  snapshot.sound, snapshot.owner, snapshot.flag, static_cast<uint32_t>(index));
  }

  __imp__sub_8262AE58(ctx, base);

  for (size_t index = 0; index < captured; ++index) {
    const auto& snapshot = entries[index];
    if (!IsGuestRange(snapshot.entry, 12))
      continue;
    const uint32_t current_token =
        snapshot.entry >= sizeof(uint32_t) &&
                IsGuestRange(snapshot.entry - sizeof(uint32_t), sizeof(uint32_t))
            ? REX_LOAD_U32(snapshot.entry - sizeof(uint32_t))
            : 0;
    LogCheckpoint("party-stop-entry-after", 0x8262AE58, caller, snapshot.entry, current_token,
                  REX_LOAD_U32(snapshot.entry), REX_LOAD_U32(snapshot.entry + sizeof(uint32_t)),
                  REX_LOAD_U8(snapshot.entry + sizeof(uint64_t)), static_cast<uint32_t>(index));
  }
  LogCheckpoint("party-stop-exit", 0x8262AE58, caller, manager, script_id, script_index, total,
                static_cast<uint32_t>(captured), static_cast<uint32_t>(epoch));
}

extern "C" void sub_829093B0(PPCContext& ctx, uint8_t* base) {
  const uint32_t sound = ctx.r3.u32;
  const uint32_t stop_mode = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  const bool trace_party_stop = caller == kPartyStopCallSite;
  if (trace_party_stop && IsGuestRange(sound, kSoundFacadeTargetOffset + sizeof(uint32_t))) {
    LogCheckpoint("party-sound-stop-enter", 0x829093B0, caller, sound, stop_mode,
                  REX_LOAD_U8(sound + kSoundFacadeHandleOffset),
                  REX_LOAD_U8(sound + kSoundFacadePartitionOffset),
                  REX_LOAD_U32(sound + kSoundFacadeTargetOffset),
                  static_cast<uint32_t>(g_party_transition_epoch.load(std::memory_order_acquire)));
  }
  __imp__sub_829093B0(ctx, base);
  if (trace_party_stop && IsGuestRange(sound, kSoundFacadeTargetOffset + sizeof(uint32_t))) {
    LogCheckpoint("party-sound-stop-exit", 0x829093B0, caller, sound, stop_mode,
                  REX_LOAD_U8(sound + kSoundFacadeHandleOffset),
                  REX_LOAD_U8(sound + kSoundFacadePartitionOffset),
                  REX_LOAD_U32(sound + kSoundFacadeTargetOffset),
                  static_cast<uint32_t>(g_party_transition_epoch.load(std::memory_order_acquire)));
  }
}

extern "C" void sub_82910118(PPCContext& ctx, uint8_t* base) {
  const uint32_t internal = ctx.r3.u32;
  const uint32_t bit = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  if (caller != kPartyStopCallSite) {
    __imp__sub_82910118(ctx, base);
    return;
  }
  const InternalPublishSnapshot before = CaptureInternalPublish(base, internal);
  LogCheckpoint("party-publish-before", 0x82910118, caller, before.internal, before.selector,
                before.selected_index, before.selected_record, before.selected_flag, bit);
  __imp__sub_82910118(ctx, base);
  const InternalPublishSnapshot after = CaptureInternalPublish(base, internal);
  LogCheckpoint("party-publish-after", 0x82910118, caller, after.internal, after.selector,
                after.selected_index, after.selected_record, after.selected_flag, bit);
}

extern "C" void sub_8290FD90(PPCContext& ctx, uint8_t* base) {
  const uint32_t manager = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  const uint64_t epoch = ClaimPartyEpoch(&g_last_publish_epoch);
  if (!epoch) {
    __imp__sub_8290FD90(ctx, base);
    return;
  }
  const PublishWindowSnapshot before = CapturePublishWindow(base, manager);
  LogCheckpoint("party-publish-window-before", 0x8290FD90, caller, before.manager, before.selector,
                before.producer_index, before.consumer_index, before.producer_record,
                before.consumer_record);
  __imp__sub_8290FD90(ctx, base);
  const PublishWindowSnapshot after = CapturePublishWindow(base, manager);
  LogCheckpoint("party-publish-window-after", 0x8290FD90, caller, after.manager, after.selector,
                after.producer_index, after.consumer_index, after.producer_record,
                after.consumer_record);
}

extern "C" void sub_8291DF00(PPCContext& ctx, uint8_t* base) {
  const uint32_t output = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  const uint64_t epoch = ClaimPartyEpoch(&g_last_worker_epoch);
  if (!epoch) {
    __imp__sub_8291DF00(ctx, base);
    return;
  }
  const WorkerPoolSnapshot before = CaptureWorkerPool(base, output);
  LogCheckpoint("party-worker-before", 0x8291DF00, caller, before.output, before.root, before.table,
                before.count, static_cast<uint32_t>(epoch), 0);
  __imp__sub_8291DF00(ctx, base);
  const WorkerPoolSnapshot after = CaptureWorkerPool(base, output);
  LogCheckpoint("party-worker-after", 0x8291DF00, caller, after.output, after.root, after.table,
                after.count, static_cast<uint32_t>(epoch), ctx.r3.u32);
}

extern "C" void sub_82444A30(PPCContext& ctx, uint8_t* base) {
  const uint32_t first_frame = ctx.r3.u32;
  const uint32_t data_source = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  __imp__sub_82444A30(ctx, base);
  TraceCrFrameArray(base, caller, first_frame, data_source, 0);
  uint32_t second_frame = 0;
  if (CheckedGuestAdd(first_frame, kCrFrameSecondOffset, kCrFrameCapacityOffset + sizeof(uint32_t),
                      &second_frame)) {
    TraceCrFrameArray(base, caller, second_frame, data_source, 1);
  }
}
