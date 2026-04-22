/**
 * @file        core/memory_switch.cpp
 * @brief       Nintendo Switch (libnx) memory backend
 *
 * Uses svcMapPhysicalMemory to back virtual address ranges with physical RAM,
 * svcSetMemoryPermission for page protection, svcQueryMemory for introspection,
 * and virtmem* APIs for address space reservation.
 *
 * ── HOS memory model constraints (horizon userland) ──
 *
 * 1. svcMapPhysicalMemory (syscall 0x2C) requires the address AND size to be
 *    aligned to 0x200000 (2 MiB). This is the hardware TLB block size on
 *    Aarch64 and is enforced by the kernel. See switchbrew.org/wiki/SVC and
 *    libnx runtime/init.c (comment: "Must be a multiple of 0x200000").
 *    Consequently, the allocation_granularity surfaced to xmemory.cpp is
 *    0x200000; page_size() stays at 0x1000 so the emulator's write-watch and
 *    per-4K-page protection machinery in BaseHeap continues to work
 *    (svcSetMemoryPermission itself accepts 4 KiB granularity).
 *
 * 2. Userland heap budget on Switch is strictly bounded by the configured
 *    application memory pool: 3.0 GiB (Erista, 4 GiB RAM) or 3.5 GiB
 *    (Mariko, 6 GiB RAM), further reduced by applet overhead, framebuffers,
 *    gpu heap, and libnx internal allocations. The emulator's canonical
 *    ~4.5 GiB file mapping (0x11FFFFFFF bytes) will NOT fit. We cap physical
 *    commit to ~2.25 GiB (the canonical virtual region 0x00000000–0x8FFFFFFF
 *    plus the 512 MiB physical page pool) and refuse larger requests early
 *    with a clear error.
 *
 * 3. TRUE virtual aliasing (multiple VAs backed by the same physical pages)
 *    is UNAVAILABLE to userland homebrew. The needed syscalls —
 *    svcCreateSharedMemory (0x50), svcMapProcessMemory (0x74) — are
 *    privileged; svcMirrorMemory does not exist on HOS. The emulator's 9
 *    overlapping views (see xmemory.cpp map_info[]) therefore cannot be
 *    satisfied with distinct VAs backed by shared physical pages. Attempting
 *    svcMapPhysicalMemory on every view would multiply physical commit by 9×
 *    and blow the budget (~20 GiB for a 2.25 GiB backing store).
 *
 *    Our MapFileView only commits the primary view (the one whose file offset
 *    starts at 0). Aliased views (file_offset != 0, or that overlap an
 *    already-committed region) are recorded as VA reservations with NO
 *    physical backing; accessing them will fault. The emulator's MMIO / VA
 *    translation layer (see runtime::MMIOHandler and Memory::TranslateVirtual)
 *    already routes guest physical accesses (0xA0000000+) through
 *    physical_membase_, so the aliased views are not needed once translation
 *    is active — however, any code that does raw pointer arithmetic on a
 *    view VA *will* fault. This is a deliberate trade-off. See TODO below.
 *
 * TODO(switch): implement a patched translation path in xmemory.cpp that
 *   routes aliased-range host pointer access through the primary backing
 *   store on NX. Alternatively, investigate svcMapPhysicalMemoryUnsafe (5.0+)
 *   + sysmodule hook to increase the safe heap cap — unlikely, but worth
 *   documenting the option.
 *
 * @license     BSD 3-Clause License
 */

#include <cstdlib>
#include <cstring>

#include <rex/assert.h>
#include <rex/logging.h>
#include <rex/memory/utils.h>
#include <rex/platform.h>

extern "C" {
#include <switch/result.h>
#include <switch/kernel/svc.h>
#include <switch/kernel/virtmem.h>
}

namespace rex {
namespace memory {

// ── Helpers ──────────────────────────────────────────────────────────────────

// Protection/watchpoint page size. svcSetMemoryPermission accepts 4 KiB
// granularity (switchbrew.org/wiki/SVC#svcSetMemoryPermission). Keep this at
// 4 KiB so xmemory.cpp's per-system-page protection / write-watch tracking
// doesn't coarsen to 2 MiB granularity.
static constexpr size_t kNxPageSize = 0x1000;

// Physical allocation granularity. svcMapPhysicalMemory REQUIRES addr and
// size to be multiples of 2 MiB. All svcMapPhysicalMemory / svcUnmap calls
// below must be aligned to this.
static constexpr size_t kNxAllocGranularity = 0x200000;

// Budget cap: on userland homebrew the total mapped heap across the process
// is bounded (typically ~3 GiB Erista, ~3.5 GiB Mariko, minus framebuffer +
// gpu + sysmodule overhead). We refuse mapping requests larger than this to
// fail early rather than let svcMapPhysicalMemory return 0xD401 (LimitReached)
// deep inside MapViews.
//
// Picked 2304 MiB as a safe margin: 2048 MiB for the canonical 4 GiB guest VA
// minus the aliased physical ranges (0xA0000000–0xFFFFFFFF = 1.5 GiB that
// map onto the 512 MiB physical pool), plus the 512 MiB physical pool itself
// = 2048 + 512 = 2560 MiB on paper. We leave ~256 MiB headroom for everything
// else the process needs and settle on 2304 MiB. xmemory.cpp requests
// 0x11FFFFFFF (~4.5 GiB) for its mapping; that request will be CAPPED to
// this value on Switch via the CreateFileMappingHandle branch below.
static constexpr size_t kNxMaxMappingBytes = size_t(2304) * 1024 * 1024;

static inline size_t NxAlignUp(size_t value, size_t alignment) {
  return (value + alignment - 1) & ~(alignment - 1);
}

// Round up to the 2 MiB allocation granularity required by svcMapPhysicalMemory.
static inline size_t NxAlignAlloc(size_t value) {
  return NxAlignUp(value, kNxAllocGranularity);
}

// Round up to the 4 KiB permission granularity used by svcSetMemoryPermission.
static inline size_t NxAlignPage(size_t value) {
  return NxAlignUp(value, kNxPageSize);
}

// Convert PageAccess to libnx Permission enum value.
// svcSetMemoryPermission only accepts Perm_None(0), Perm_R(1), Perm_Rw(3).
// Execute permissions fall back to the non-execute equivalent (best effort).
static uint32_t ToNxPermission(PageAccess access) {
  switch (access) {
    case PageAccess::kNoAccess:
      return Perm_None;
    case PageAccess::kReadOnly:
    case PageAccess::kExecuteReadOnly:
      return Perm_R;
    case PageAccess::kReadWrite:
    case PageAccess::kExecuteReadWrite:
      return Perm_Rw;
    default:
      return Perm_None;
  }
}

// Convert libnx Permission bits to PageAccess.
static PageAccess NxPermToPageAccess(uint32_t perm) {
  const bool r = (perm & Perm_R) != 0;
  const bool w = (perm & Perm_W) != 0;
  const bool x = (perm & Perm_X) != 0;
  if (!r && !w && !x) return PageAccess::kNoAccess;
  if (x) return w ? PageAccess::kExecuteReadWrite : PageAccess::kExecuteReadOnly;
  return w ? PageAccess::kReadWrite : PageAccess::kReadOnly;
}

// Check if a memory region is already backed by physical pages (MemType_Heap
// or any mapped type). Used to distinguish "reserve then commit" from fresh
// allocation.
static bool NxIsRegionMapped(void* addr, size_t length) {
  MemoryInfo mem_info;
  u32 page_info;
  Result rc = svcQueryMemory(&mem_info, &page_info, (u64)addr);
  if (R_FAILED(rc)) return false;
  // MemType 0 = Unmapped. Anything else has backing pages.
  return (mem_info.type & 0xFF) != MemType_Unmapped;
}

size_t page_size() { return kNxPageSize; }
size_t allocation_granularity() { return kNxAllocGranularity; }

bool IsWritableExecutableMemorySupported() { return false; }

// ── AllocFixed ───────────────────────────────────────────────────────────────
// Emulates Windows VirtualAlloc semantics:
//   Reserve:       find/hold VA range, back with svcMapPhysicalMemory (Perm_None)
//   Commit:        set page permissions on already-backed VA range
//   ReserveCommit: reserve + commit in one step
//
// Note on alignment: svcMapPhysicalMemory needs 2 MiB alignment for both addr
// and size. svcSetMemoryPermission accepts 4 KiB. We use NxAlignAlloc for any
// length that flows into svcMap/Unmap and NxAlignPage for length into
// svcSetMemoryPermission.

void* AllocFixed(void* base_address, size_t length, AllocationType allocation_type,
                 PageAccess access) {
  if (length == 0) return nullptr;
  const size_t alloc_len = NxAlignAlloc(length);
  const size_t perm_len = NxAlignPage(length);
  if (alloc_len == 0) return nullptr;

  const bool do_reserve =
      (allocation_type == AllocationType::kReserve ||
       allocation_type == AllocationType::kReserveCommit);
  const bool do_commit =
      (allocation_type == AllocationType::kCommit ||
       allocation_type == AllocationType::kReserveCommit);

  // ── Fixed-address path ─────────────────────────────────────────────────
  if (base_address) {
    // 2 MiB alignment is required for svcMapPhysicalMemory. If the caller
    // gives us a mis-aligned base, we can't back it — this is always a bug.
    if ((reinterpret_cast<uintptr_t>(base_address) & (kNxAllocGranularity - 1)) != 0) {
      REXSYS_ERROR(
          "memory_switch: AllocFixed base {} not 2MiB-aligned; svcMapPhysicalMemory will fail",
          base_address);
      return nullptr;
    }

    if (do_reserve) {
      // Back this VA range with physical memory, permission = Perm_None for
      // reserve-only, or the requested access for reserve+commit.
      // If the region is already mapped (e.g. from MapFileView), skip the
      // svcMapPhysicalMemory call — it would fail on MemType_Heap memory.
      if (!NxIsRegionMapped(base_address, alloc_len)) {
        Result rc = svcMapPhysicalMemory(base_address, alloc_len);
        if (R_FAILED(rc)) {
          REXSYS_ERROR(
              "memory_switch: svcMapPhysicalMemory({}, 0x{:x}) failed rc=0x{:x}",
              base_address, alloc_len, rc);
          return nullptr;
        }
      }
    }

    if (do_commit) {
      // Apply requested permissions. svcMapPhysicalMemory gives Rw by default.
      uint32_t perm = ToNxPermission(access);
      Result rc = svcSetMemoryPermission(base_address, perm_len, perm);
      if (R_FAILED(rc)) {
        // Commit on already-committed memory with matching perm is fine.
        // Some memory types (MemType_Heap with Perm_Rw) may reject a
        // redundant svcSetMemoryPermission. Accept this silently when the
        // requested perm is Rw and the region is already Rw.
        if (perm == Perm_Rw && NxIsRegionMapped(base_address, alloc_len)) {
          return base_address;
        }
        return nullptr;
      }
      // Zero-fill committed memory.
      if (access == PageAccess::kReadWrite || access == PageAccess::kExecuteReadWrite) {
        std::memset(base_address, 0, perm_len);
      }
    } else {
      // Reserve-only: set to Perm_None.
      svcSetMemoryPermission(base_address, perm_len, Perm_None);
    }

    return base_address;
  }

  // ── No fixed address: find a free VA via virtmem ───────────────────────
  // virtmemFindAslr already yields 2 MiB-aligned addresses on HOS.
  virtmemLock();
  void* addr = virtmemFindAslr(alloc_len, kNxAllocGranularity);
  VirtmemReservation* rv = nullptr;
  if (addr) {
    rv = virtmemAddReservation(addr, alloc_len);
  }
  virtmemUnlock();
  if (!addr || !rv) return nullptr;

  // Back with physical memory.
  Result rc = svcMapPhysicalMemory(addr, alloc_len);
  if (R_FAILED(rc)) {
    REXSYS_ERROR(
        "memory_switch: svcMapPhysicalMemory(aslr={}, 0x{:x}) failed rc=0x{:x}", addr,
        alloc_len, rc);
    virtmemLock();
    virtmemRemoveReservation(rv);
    virtmemUnlock();
    return nullptr;
  }

  // Zero-fill.
  std::memset(addr, 0, alloc_len);

  if (do_commit) {
    uint32_t perm = ToNxPermission(access);
    if (perm != Perm_Rw) {
      svcSetMemoryPermission(addr, perm_len, perm);
    }
  } else {
    // Reserve-only: mark inaccessible.
    svcSetMemoryPermission(addr, perm_len, Perm_None);
  }

  return addr;
}

// ── DeallocFixed ─────────────────────────────────────────────────────────────

bool DeallocFixed(void* base_address, size_t length, DeallocationType deallocation_type) {
  if (!base_address) return false;
  const size_t alloc_len = NxAlignAlloc(length);
  const size_t perm_len = NxAlignPage(length);

  switch (deallocation_type) {
    case DeallocationType::kDecommit: {
      // Zero-fill BEFORE removing access.
      // Restore Rw first in case it was Perm_None or Perm_R.
      svcSetMemoryPermission(base_address, perm_len, Perm_Rw);
      std::memset(base_address, 0, perm_len);
      // Remove access but keep physical backing (can be re-committed).
      svcSetMemoryPermission(base_address, perm_len, Perm_None);
      return true;
    }
    case DeallocationType::kRelease: {
      // Fully release: restore Rw so svcUnmapPhysicalMemory can operate,
      // then unmap the physical backing. Note svcUnmapPhysicalMemory also
      // requires 2 MiB alignment — hence alloc_len.
      svcSetMemoryPermission(base_address, perm_len, Perm_Rw);
      Result rc = svcUnmapPhysicalMemory(base_address, alloc_len);
      // Note: if this was heap-allocated memory (no virtmem reservation),
      // we cannot free() it after unmapping physical pages — the VA is now
      // invalid. The reservation (if any) leaks; acceptable for the
      // emulator's process-lifetime allocations.
      return R_SUCCEEDED(rc);
    }
    default:
      return false;
  }
}

// ── Protect ──────────────────────────────────────────────────────────────────

bool Protect(void* base_address, size_t length, PageAccess access, PageAccess* out_old_access) {
  // 4 KiB granularity is sufficient here — svcSetMemoryPermission accepts
  // page-aligned (not block-aligned) regions.
  const size_t perm_len = NxAlignPage(length);

  // Query old access before changing, if the caller needs it.
  if (out_old_access) {
    *out_old_access = PageAccess::kNoAccess;
    MemoryInfo mem_info;
    u32 page_info;
    Result qrc = svcQueryMemory(&mem_info, &page_info, (u64)base_address);
    if (R_SUCCEEDED(qrc)) {
      *out_old_access = NxPermToPageAccess(mem_info.perm);
    }
  }

  uint32_t perm = ToNxPermission(access);
  Result rc = svcSetMemoryPermission(base_address, perm_len, perm);
  return R_SUCCEEDED(rc);
}

// ── QueryProtect ─────────────────────────────────────────────────────────────

bool QueryProtect(void* base_address, size_t& length, PageAccess& access_out) {
  access_out = PageAccess::kNoAccess;
  length = 0;

  MemoryInfo mem_info;
  u32 page_info;
  Result rc = svcQueryMemory(&mem_info, &page_info, (u64)base_address);
  if (R_FAILED(rc)) {
    return false;
  }

  const uintptr_t addr = reinterpret_cast<uintptr_t>(base_address);
  const uintptr_t region_end = mem_info.addr + mem_info.size;
  if (addr < mem_info.addr || addr >= region_end) {
    return false;
  }
  length = static_cast<size_t>(region_end - addr);
  access_out = NxPermToPageAccess(mem_info.perm);
  return true;
}

// ── File mapping (guest address space) ───────────────────────────────────────
//
// See the file header for the aliasing rationale. Briefly:
//   * CreateFileMappingHandle reserves a single contiguous VA range and backs
//     it with svcMapPhysicalMemory — this is the "primary" backing store.
//   * MapFileView at file_offset==0 returns the primary backing VA (or the
//     caller's requested fixed address, provided it matches the backing).
//   * MapFileView at file_offset!=0 (the aliased 9-view case from xmemory.cpp)
//     returns a VA reservation WITHOUT physical backing — touching it will
//     fault. The emulator must route accesses in those guest ranges through
//     the MMIO handler / physical_membase_ translation (which it already does
//     for physical guest addresses; see xmemory.cpp).

struct NxFileMappingInfo {
  void* base;                       // Primary backing store VA (physical-backed).
  size_t size;                      // Aligned size of the backing store.
  VirtmemReservation* reservation;  // Virtmem reservation (for cleanup).
};

static constexpr int kMaxNxFileMappings = 4;
static NxFileMappingInfo s_nx_file_mappings[kMaxNxFileMappings] = {};

// Tracking for "logical" (unbacked) aliased views created by MapFileView.
// We need to remember the virtmem reservation so UnmapFileView can release it
// without trying to call svcUnmapPhysicalMemory (which would fail on unbacked
// VA).
struct NxAliasView {
  void* base;
  size_t size;
  VirtmemReservation* reservation;  // nullptr if the VA overlaps the primary
                                    // backing and was not separately reserved.
};
static constexpr int kMaxNxAliasViews = 16;
static NxAliasView s_nx_alias_views[kMaxNxAliasViews] = {};

static int NxAllocAliasSlot() {
  for (int i = 0; i < kMaxNxAliasViews; i++) {
    if (s_nx_alias_views[i].base == nullptr) return i;
  }
  return -1;
}

static NxAliasView* NxFindAliasView(void* base) {
  for (int i = 0; i < kMaxNxAliasViews; i++) {
    if (s_nx_alias_views[i].base == base) return &s_nx_alias_views[i];
  }
  return nullptr;
}

FileMappingHandle CreateFileMappingHandle(const std::filesystem::path& path, size_t length,
                                          PageAccess access, bool commit) {
  (void)path;
  (void)access;
  (void)commit;

  // Enforce the budget cap: the canonical emulator request of 0x11FFFFFFF
  // (~4.5 GiB) cannot be satisfied on Switch userland. Fail early with a
  // clear error — the caller (xmemory.cpp) already treats a failed mapping as
  // fatal.
  if (length > kNxMaxMappingBytes) {
    REXSYS_ERROR(
        "memory_switch: requested mapping 0x{:x} ({} MiB) exceeds Switch userland cap 0x{:x} "
        "({} MiB); guest heap budget on NX is ~3 GiB. xmemory.cpp must be patched with an "
        "#if REX_PLATFORM_NX branch to request a smaller mapping or use a different layout.",
        length, length / (1024 * 1024), kNxMaxMappingBytes,
        kNxMaxMappingBytes / (1024 * 1024));
    return kFileMappingHandleInvalid;
  }

  const size_t aligned_length = NxAlignAlloc(length);

  // Find a free slot first.
  int slot = -1;
  for (int i = 0; i < kMaxNxFileMappings; i++) {
    if (s_nx_file_mappings[i].base == nullptr) {
      slot = i;
      break;
    }
  }
  if (slot < 0) return kFileMappingHandleInvalid;

  // Find a free VA range large enough for the entire backing store.
  virtmemLock();
  void* base = virtmemFindAslr(aligned_length, kNxAllocGranularity);
  VirtmemReservation* rv = nullptr;
  if (base) {
    rv = virtmemAddReservation(base, aligned_length);
  }
  virtmemUnlock();

  if (!base || !rv) {
    REXSYS_ERROR("memory_switch: virtmemFindAslr failed for 0x{:x} bytes", aligned_length);
    return kFileMappingHandleInvalid;
  }

  // Back the entire range with physical memory. This gives us a contiguous
  // region of real pages that serves as the "file" for MapFileView.
  Result rc = svcMapPhysicalMemory(base, aligned_length);
  if (R_FAILED(rc)) {
    REXSYS_ERROR(
        "memory_switch: svcMapPhysicalMemory({}, 0x{:x}) failed rc=0x{:x} (heap budget "
        "exhausted?)",
        base, aligned_length, rc);
    virtmemLock();
    virtmemRemoveReservation(rv);
    virtmemUnlock();
    return kFileMappingHandleInvalid;
  }

  // Zero-fill the backing store.
  std::memset(base, 0, aligned_length);

  s_nx_file_mappings[slot].base = base;
  s_nx_file_mappings[slot].size = aligned_length;
  s_nx_file_mappings[slot].reservation = rv;

  return static_cast<FileMappingHandle>(slot);
}

void CloseFileMappingHandle(FileMappingHandle handle, const std::filesystem::path& path) {
  (void)path;
  int idx = static_cast<int>(handle);
  if (idx < 0 || idx >= kMaxNxFileMappings) return;

  auto& info = s_nx_file_mappings[idx];
  if (info.base) {
    // Restore Rw before unmapping.
    svcSetMemoryPermission(info.base, info.size, Perm_Rw);
    svcUnmapPhysicalMemory(info.base, info.size);
    if (info.reservation) {
      virtmemLock();
      virtmemRemoveReservation(info.reservation);
      virtmemUnlock();
    }
    info.base = nullptr;
    info.size = 0;
    info.reservation = nullptr;
  }

  // Also release any alias views still tracked for this mapping. This is a
  // best-effort cleanup — ordinarily UnmapViews() releases them first.
  for (int i = 0; i < kMaxNxAliasViews; i++) {
    auto& av = s_nx_alias_views[i];
    if (av.base && av.reservation) {
      virtmemLock();
      virtmemRemoveReservation(av.reservation);
      virtmemUnlock();
    }
    av = {};
  }
}

void* MapFileView(FileMappingHandle handle, void* base_address, size_t length, PageAccess access,
                  size_t file_offset) {
  int idx = static_cast<int>(handle);
  if (idx < 0 || idx >= kMaxNxFileMappings) return nullptr;

  const auto& info = s_nx_file_mappings[idx];
  if (!info.base) return nullptr;

  const size_t alloc_length = NxAlignAlloc(length);
  const size_t perm_length = NxAlignPage(length);

  // Verify the view fits within the logical backing store.
  if (file_offset + alloc_length > info.size) {
    REXSYS_ERROR(
        "memory_switch: MapFileView offset+len=0x{:x}+0x{:x} exceeds mapping 0x{:x}",
        file_offset, alloc_length, info.size);
    return nullptr;
  }

  // ── The primary view: file_offset == 0 ───────────────────────────────
  // This is the one view that gets real physical backing. If the caller
  // requested a specific base_address, we can't satisfy it — the primary
  // backing lives at info.base and cannot be moved. Return info.base and
  // trust the caller (xmemory.cpp tries multiple bases; the returned one
  // becomes mapping_base_).
  if (file_offset == 0) {
    void* addr = info.base;
    if (base_address && base_address != addr) {
      // xmemory.cpp passes mapping_base + 0 here — it will accept the value
      // we return (it stores it in mapping_base_). To stay compatible with
      // callers that assume the returned address equals the requested one,
      // log a warning and still return the backing address.
      REXSYS_WARN(
          "memory_switch: MapFileView requested base {} but primary backing is at {}; "
          "returning backing address (caller must use returned value)",
          base_address, addr);
    }
    uint32_t perm = ToNxPermission(access);
    if (perm != Perm_Rw) {
      svcSetMemoryPermission(addr, perm_length, perm);
    }
    return addr;
  }

  // ── Aliased view: file_offset != 0 ───────────────────────────────────
  // These are the 8 aliased views from xmemory.cpp map_info[]. We cannot
  // create real VA aliases in userland HOS. Instead, we reserve the VA
  // range and return it unbacked. Any access will fault — the emulator's
  // MMIO handler and TranslateVirtual path must redirect these reads/writes
  // to info.base + file_offset.
  //
  // TODO(switch): xmemory.cpp currently does NOT redirect host-pointer
  // access to aliased ranges through MMIOHandler; only guest-virtual
  // accesses go through TranslateVirtual. Any code that does
  //   memcpy(mapping_base + 0xA0000000 + guest_offset, ...)
  // will fault on Switch. A proper fix requires an xmemory.cpp NX branch
  // that remaps aliased-range host pointers back to the primary backing.
  // Until that lands, NX builds will segfault on the first aliased-range
  // host access. DO NOT ship without completing that work.
  if (!base_address) {
    REXSYS_ERROR(
        "memory_switch: MapFileView aliased view (offset=0x{:x}) requires a fixed base "
        "address from the caller",
        file_offset);
    return nullptr;
  }
  if ((reinterpret_cast<uintptr_t>(base_address) & (kNxAllocGranularity - 1)) != 0) {
    REXSYS_ERROR("memory_switch: MapFileView alias base {} not 2MiB-aligned", base_address);
    return nullptr;
  }

  int slot = NxAllocAliasSlot();
  if (slot < 0) {
    REXSYS_ERROR("memory_switch: alias view slot table exhausted");
    return nullptr;
  }

  // If the requested VA happens to fall inside the primary backing already
  // (shouldn't, given xmemory.cpp's layout, but guard anyway), just return it.
  const uintptr_t req = reinterpret_cast<uintptr_t>(base_address);
  const uintptr_t base = reinterpret_cast<uintptr_t>(info.base);
  if (req >= base && req + alloc_length <= base + info.size) {
    s_nx_alias_views[slot].base = base_address;
    s_nx_alias_views[slot].size = alloc_length;
    s_nx_alias_views[slot].reservation = nullptr;  // inside primary; no extra res
    return base_address;
  }

  // Reserve the VA range so other allocators don't step on it, but do NOT
  // back it with physical memory.
  virtmemLock();
  VirtmemReservation* rv = virtmemAddReservation(base_address, alloc_length);
  virtmemUnlock();
  if (!rv) {
    REXSYS_ERROR(
        "memory_switch: virtmemAddReservation({}, 0x{:x}) failed for alias view",
        base_address, alloc_length);
    return nullptr;
  }

  s_nx_alias_views[slot].base = base_address;
  s_nx_alias_views[slot].size = alloc_length;
  s_nx_alias_views[slot].reservation = rv;

  REXSYS_WARN(
      "memory_switch: MapFileView created UNBACKED alias {} len=0x{:x} (file_offset=0x{:x}); "
      "host-pointer access will fault — see TODO(switch) in memory_switch.cpp",
      base_address, alloc_length, file_offset);

  return base_address;
}

bool UnmapFileView(FileMappingHandle handle, void* base_address, size_t length) {
  (void)handle;
  if (!base_address) return false;

  // Is this the primary view? (base_address matches a mapping's backing)
  for (int i = 0; i < kMaxNxFileMappings; i++) {
    if (s_nx_file_mappings[i].base == base_address) {
      // Primary view lives as long as the mapping handle; nothing to do here.
      // CloseFileMappingHandle will release it.
      return true;
    }
  }

  // Is this a tracked alias view?
  NxAliasView* av = NxFindAliasView(base_address);
  if (av) {
    if (av->reservation) {
      virtmemLock();
      virtmemRemoveReservation(av->reservation);
      virtmemUnlock();
    }
    *av = {};
    return true;
  }

  // Unknown view — treat it as a legacy fully-backed mapping and attempt a
  // plain unmap. This preserves behaviour for callers that might still use
  // the old single-view path.
  const size_t alloc_length = NxAlignAlloc(length);
  const size_t perm_length = NxAlignPage(length);
  svcSetMemoryPermission(base_address, perm_length, Perm_Rw);
  Result rc = svcUnmapPhysicalMemory(base_address, alloc_length);
  return R_SUCCEEDED(rc);
}

}  // namespace memory
}  // namespace rex
