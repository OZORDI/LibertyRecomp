/**
 * @file        core/memory_switch.cpp
 * @brief       Nintendo Switch (libnx) memory backend
 *
 * Uses svcMapPhysicalMemory to back virtual address ranges with physical RAM,
 * svcSetMemoryPermission for page protection, svcQueryMemory for introspection,
 * and virtmem* APIs for address space reservation.
 *
 * File mapping (CreateFileMappingHandle / MapFileView):
 * The Xbox 360 emulator creates a ~4.5GB file mapping and maps 9 overlapping
 * views at different offsets to alias physical and virtual guest memory regions.
 * On Switch, svcCreateSharedMemory (syscall 0x50) is privileged and unavailable
 * to userland homebrew, so true virtual aliasing (multiple VAs backed by the
 * same physical pages) is not possible.
 *
 * Strategy: CreateFileMappingHandle reserves a contiguous VA range via
 * virtmemFindAslr and backs it with svcMapPhysicalMemory — this is the
 * "backing store". MapFileView then reserves a separate VA range at the
 * caller's requested fixed address and backs it with its own physical pages
 * via svcMapPhysicalMemory. The emulator's MMIO handler and physical heap
 * translation layer handle the coherence between the virtual views and
 * physical heap at a higher level — byte-level aliasing is maintained by the
 * emulator's write-watch / address translation system, not by the MMU.
 *
 * AllocFixed with a fixed base address uses svcMapPhysicalMemory to back the
 * requested VA with physical RAM (commit), or just applies page permissions
 * on already-backed memory (commit-on-reservation pattern from the heap).
 *
 * @license     BSD 3-Clause License
 */

#include <cstdlib>
#include <cstring>

#include <rex/assert.h>
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

static constexpr size_t kNxPageSize = 0x1000;

static inline size_t NxAlignUp(size_t value, size_t alignment) {
  return (value + alignment - 1) & ~(alignment - 1);
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
size_t allocation_granularity() { return kNxPageSize; }

bool IsWritableExecutableMemorySupported() { return false; }

// ── AllocFixed ───────────────────────────────────────────────────────────────
// Emulates Windows VirtualAlloc semantics:
//   Reserve:       find/hold VA range, back with svcMapPhysicalMemory (Perm_None)
//   Commit:        set page permissions on already-backed VA range
//   ReserveCommit: reserve + commit in one step

void* AllocFixed(void* base_address, size_t length, AllocationType allocation_type,
                 PageAccess access) {
  const size_t aligned_len = NxAlignUp(length, kNxPageSize);
  if (aligned_len == 0) return nullptr;

  const bool do_reserve =
      (allocation_type == AllocationType::kReserve ||
       allocation_type == AllocationType::kReserveCommit);
  const bool do_commit =
      (allocation_type == AllocationType::kCommit ||
       allocation_type == AllocationType::kReserveCommit);

  // ── Fixed-address path ─────────────────────────────────────────────────
  if (base_address) {
    if (do_reserve) {
      // Back this VA range with physical memory, permission = Perm_None for
      // reserve-only, or the requested access for reserve+commit.
      // If the region is already mapped (e.g. from MapFileView), skip the
      // svcMapPhysicalMemory call — it would fail on MemType_Heap memory.
      if (!NxIsRegionMapped(base_address, aligned_len)) {
        Result rc = svcMapPhysicalMemory(base_address, aligned_len);
        if (R_FAILED(rc)) return nullptr;
      }
    }

    if (do_commit) {
      // Apply requested permissions. svcMapPhysicalMemory gives Rw by default.
      uint32_t perm = ToNxPermission(access);
      Result rc = svcSetMemoryPermission(base_address, aligned_len, perm);
      if (R_FAILED(rc)) {
        // Commit on already-committed memory with matching perm is fine.
        // Some memory types (MemType_Heap with Perm_Rw) may reject a
        // redundant svcSetMemoryPermission. Accept this silently when the
        // requested perm is Rw and the region is already Rw.
        if (perm == Perm_Rw && NxIsRegionMapped(base_address, aligned_len)) {
          return base_address;
        }
        return nullptr;
      }
      // Zero-fill committed memory.
      if (access == PageAccess::kReadWrite || access == PageAccess::kExecuteReadWrite) {
        std::memset(base_address, 0, aligned_len);
      }
    } else {
      // Reserve-only: set to Perm_None.
      svcSetMemoryPermission(base_address, aligned_len, Perm_None);
    }

    return base_address;
  }

  // ── No fixed address: find a free VA via virtmem ───────────────────────
  virtmemLock();
  void* addr = virtmemFindAslr(aligned_len, kNxPageSize);
  VirtmemReservation* rv = nullptr;
  if (addr) {
    rv = virtmemAddReservation(addr, aligned_len);
  }
  virtmemUnlock();
  if (!addr || !rv) return nullptr;

  // Back with physical memory.
  Result rc = svcMapPhysicalMemory(addr, aligned_len);
  if (R_FAILED(rc)) {
    virtmemLock();
    virtmemRemoveReservation(rv);
    virtmemUnlock();
    return nullptr;
  }

  // Zero-fill.
  std::memset(addr, 0, aligned_len);

  if (do_commit) {
    uint32_t perm = ToNxPermission(access);
    if (perm != Perm_Rw) {
      svcSetMemoryPermission(addr, aligned_len, perm);
    }
  } else {
    // Reserve-only: mark inaccessible.
    svcSetMemoryPermission(addr, aligned_len, Perm_None);
  }

  return addr;
}

// ── DeallocFixed ─────────────────────────────────────────────────────────────

bool DeallocFixed(void* base_address, size_t length, DeallocationType deallocation_type) {
  if (!base_address) return false;
  const size_t aligned_len = NxAlignUp(length, kNxPageSize);

  switch (deallocation_type) {
    case DeallocationType::kDecommit: {
      // Zero-fill BEFORE removing access.
      // Restore Rw first in case it was Perm_None or Perm_R.
      svcSetMemoryPermission(base_address, aligned_len, Perm_Rw);
      std::memset(base_address, 0, aligned_len);
      // Remove access but keep physical backing (can be re-committed).
      svcSetMemoryPermission(base_address, aligned_len, Perm_None);
      return true;
    }
    case DeallocationType::kRelease: {
      // Fully release: restore Rw so svcUnmapPhysicalMemory can operate,
      // then unmap the physical backing.
      svcSetMemoryPermission(base_address, aligned_len, Perm_Rw);
      Result rc = svcUnmapPhysicalMemory(base_address, aligned_len);
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
  const size_t aligned_len = NxAlignUp(length, kNxPageSize);

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
  Result rc = svcSetMemoryPermission(base_address, aligned_len, perm);
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

// Tracking structure for file mapping backing stores.
// The emulator creates one ~4.5GB mapping; we support up to 4 for safety.
struct NxFileMappingInfo {
  void* base;                       // Backing store VA (physical-backed).
  size_t size;                      // Aligned size of the backing store.
  VirtmemReservation* reservation;  // Virtmem reservation (for cleanup).
};

static constexpr int kMaxNxFileMappings = 4;
static NxFileMappingInfo s_nx_file_mappings[kMaxNxFileMappings] = {};

FileMappingHandle CreateFileMappingHandle(const std::filesystem::path& path, size_t length,
                                          PageAccess access, bool commit) {
  (void)path;
  (void)access;
  (void)commit;
  const size_t aligned_length = NxAlignUp(length, kNxPageSize);

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
  void* base = virtmemFindAslr(aligned_length, kNxPageSize);
  VirtmemReservation* rv = nullptr;
  if (base) {
    rv = virtmemAddReservation(base, aligned_length);
  }
  virtmemUnlock();

  if (!base || !rv) {
    return kFileMappingHandleInvalid;
  }

  // Back the entire range with physical memory. This gives us a contiguous
  // region of real pages that serves as the "file" for MapFileView.
  Result rc = svcMapPhysicalMemory(base, aligned_length);
  if (R_FAILED(rc)) {
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
}

void* MapFileView(FileMappingHandle handle, void* base_address, size_t length, PageAccess access,
                  size_t file_offset) {
  int idx = static_cast<int>(handle);
  if (idx < 0 || idx >= kMaxNxFileMappings) return nullptr;

  const auto& info = s_nx_file_mappings[idx];
  if (!info.base) return nullptr;

  const size_t aligned_length = NxAlignUp(length, kNxPageSize);

  // Verify the view fits within the backing store.
  if (file_offset + aligned_length > info.size) {
    return nullptr;
  }

  // On Switch we cannot create true virtual aliases (multiple VAs pointing to
  // the same physical page). Each view gets its own physical backing. The
  // emulator's MMIO handler maintains coherence at a higher level.
  void* addr = base_address;

  if (addr) {
    // Back the requested fixed address with physical memory.
    Result rc = svcMapPhysicalMemory(addr, aligned_length);
    if (R_FAILED(rc)) {
      return nullptr;
    }

    // Zero-fill (physical pages may contain stale data).
    std::memset(addr, 0, aligned_length);

    // Apply requested permissions.
    uint32_t perm = ToNxPermission(access);
    if (perm != Perm_Rw) {
      svcSetMemoryPermission(addr, aligned_length, perm);
    }

    return addr;
  }

  // No fixed address requested: find a free VA range.
  virtmemLock();
  addr = virtmemFindAslr(aligned_length, 0);
  VirtmemReservation* rv = nullptr;
  if (addr) {
    rv = virtmemAddReservation(addr, aligned_length);
  }
  virtmemUnlock();

  if (!addr || !rv) return nullptr;

  Result rc = svcMapPhysicalMemory(addr, aligned_length);
  if (R_FAILED(rc)) {
    virtmemLock();
    virtmemRemoveReservation(rv);
    virtmemUnlock();
    return nullptr;
  }

  std::memset(addr, 0, aligned_length);

  uint32_t perm = ToNxPermission(access);
  if (perm != Perm_Rw) {
    svcSetMemoryPermission(addr, aligned_length, perm);
  }

  return addr;
}

bool UnmapFileView(FileMappingHandle handle, void* base_address, size_t length) {
  (void)handle;
  if (!base_address) return false;
  const size_t aligned_length = NxAlignUp(length, kNxPageSize);

  // Restore Rw before unmapping (svcUnmapPhysicalMemory requires writable).
  svcSetMemoryPermission(base_address, aligned_length, Perm_Rw);
  Result rc = svcUnmapPhysicalMemory(base_address, aligned_length);
  return R_SUCCEEDED(rc);
}

}  // namespace memory
}  // namespace rex
