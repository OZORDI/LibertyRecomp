/**
 * @file        core/memory_ps4.cpp
 * @brief       PlayStation 4 memory backend (Orbis direct-memory)
 *
 * Unlike Linux/macOS (which use anonymous mmap), PS4 must back guest memory
 * with Orbis direct-memory. Anonymous mmap on Orbis consumes flexible memory
 * (~512 MB process budget) — the guest wants ~4.5 GB, so mmap will OOM.
 *
 * Guest reservations therefore go through:
 *   sceKernelAllocateDirectMemory(0, SCE_KERNEL_MAIN_DMEM_SIZE, len, 2 MB,
 *                                  SCE_KERNEL_WB_ONION, &dmem_off)
 *   sceKernelMapNamedDirectMemory(&vaddr, len, prot, MAP_FIXED, dmem_off, 0, "LR_Guest")
 *   sceKernelReleaseDirectMemory(dmem_off, len)  // on release
 *
 * A side-table tracks vaddr -> (dmem_offset, length) so DeallocFixed can
 * release the backing physical memory. Non-fixed allocations (file views,
 * shm) still use POSIX mmap and are unaffected.
 *
 * Differences from Linux:
 *   - 64-bit file APIs are the default (no *64 suffix variants).
 *   - MAP_ANONYMOUS is spelled MAP_ANON.
 *   - No /proc/self/maps — QueryProtect returns kNoAccess.
 *
 * @license     BSD 3-Clause License
 */

#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <mutex>
#include <string>
#include <unordered_map>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <rex/math.h>
#include <rex/memory/utils.h>
#include <rex/diagnostics/policy.h>
#include <rex/platform.h>
#include <rex/string.h>

#if REX_PLATFORM_PS4
#  include <orbis/libkernel.h>
#  include <orbis/libc/stdlib.h>  // SCE_KERNEL_WB_ONION, SCE_KERNEL_PROT_CPU_*
#endif

// POSIX PROT_EXEC is 0x04; OpenOrbis does not define SCE_KERNEL_PROT_CPU_EXEC
// but the kernel accepts the same bit value. Keep a local alias for clarity.
#if REX_PLATFORM_PS4
#  ifndef SCE_KERNEL_PROT_CPU_EXEC
#    define SCE_KERNEL_PROT_CPU_EXEC 0x04
#  endif
#endif

// PS4 (FreeBSD-based) uses 64-bit file offsets natively; no *64 API variants needed.
#ifndef ftruncate64
#  define ftruncate64 ftruncate
#endif
#ifndef mmap64
#  define mmap64 mmap
#endif
#ifndef fstat64
#  define fstat64 fstat
#endif
#ifndef off64_t
typedef off_t off64_t;
#endif

// Ensure MAP_ANONYMOUS is defined (FreeBSD / PS4 use MAP_ANON).
#ifndef MAP_ANONYMOUS
#  ifdef MAP_ANON
#    define MAP_ANONYMOUS MAP_ANON
#  else
#    define MAP_ANONYMOUS 0x1000
#  endif
#endif

namespace rex {
namespace memory {

// Convert filesystem path to valid shm_open name (must start with /, no other slashes)
static std::string MakeShmName(const std::filesystem::path& path) {
  std::string name = path.string();
  for (char& c : name) {
    if (c == '/')
      c = '_';
  }
  if (name.empty() || name[0] != '/') {
    name.insert(name.begin(), '/');
  }
  return name;
}

size_t page_size() {
  return getpagesize();
}
size_t allocation_granularity() {
  return page_size();
}

uint32_t ToPosixProtectFlags(PageAccess access) {
  switch (access) {
    case PageAccess::kNoAccess:
      return PROT_NONE;
    case PageAccess::kReadOnly:
      return PROT_READ;
    case PageAccess::kReadWrite:
      return PROT_READ | PROT_WRITE;
    case PageAccess::kExecuteReadOnly:
      return PROT_READ | PROT_EXEC;
    case PageAccess::kExecuteReadWrite:
      return PROT_READ | PROT_WRITE | PROT_EXEC;
    default:
      assert_unhandled_case(access);
      return PROT_NONE;
  }
}

#if REX_PLATFORM_PS4
// Convert the rex PageAccess enum to SCE_KERNEL_PROT_CPU_* bits for
// sceKernelMapNamedDirectMemory. POSIX PROT_READ / PROT_WRITE happen to
// match SCE_KERNEL_PROT_CPU_READ / WRITE bit-for-bit on Orbis, so we can
// build the value from ToPosixProtectFlags() and translate PROT_EXEC.
static int32_t ToOrbisCpuProt(PageAccess access) {
  switch (access) {
    case PageAccess::kNoAccess:
      return 0;
    case PageAccess::kReadOnly:
      return SCE_KERNEL_PROT_CPU_READ;
    case PageAccess::kReadWrite:
      return SCE_KERNEL_PROT_CPU_READ | SCE_KERNEL_PROT_CPU_WRITE;
    case PageAccess::kExecuteReadOnly:
      return SCE_KERNEL_PROT_CPU_READ | SCE_KERNEL_PROT_CPU_EXEC;
    case PageAccess::kExecuteReadWrite:
      return SCE_KERNEL_PROT_CPU_READ | SCE_KERNEL_PROT_CPU_WRITE |
             SCE_KERNEL_PROT_CPU_EXEC;
    default:
      return 0;
  }
}

// Orbis direct-memory granularity.
static constexpr size_t kOrbisDirectAlign = 2ULL * 1024ULL * 1024ULL;  // 2 MB

// Side-table of live direct-memory reservations so DeallocFixed can call
// sceKernelReleaseDirectMemory with the matching (dmem_off, len).
struct DirectMemEntry {
  off_t  dmem_offset;
  size_t length;
};
static std::mutex                               g_dmem_mutex;
static std::unordered_map<void*, DirectMemEntry> g_dmem_table;

static inline size_t RoundUpTo(size_t v, size_t align) {
  return (v + align - 1) & ~(align - 1);
}
#endif  // REX_PLATFORM_PS4

bool IsWritableExecutableMemorySupported() {
  // PS4 retail / userland homebrew cannot map PROT_EXEC pages without a kernel
  // exploit (JIT path). Keep false so the recomp generator routes through its
  // non-RWX code-patch path on PS4.
  return false;
}

void* AllocFixed(void* base_address, size_t length, AllocationType allocation_type,
                 PageAccess access) {
  // Emulates Windows VirtualAlloc behavior:
  // - Reserve: create inaccessible mapping to hold address space
  // - Commit on existing reservation: mprotect to enable access
  // - New allocation: map backing storage at base_address

#if REX_PLATFORM_PS4
  // On Orbis we must use direct-memory (physical pool) rather than anonymous
  // mmap, which draws from flexible memory and maxes out at ~512 MB.
  // Direct-memory has 2 MB allocation granularity — round up.
  const size_t aligned_length = RoundUpTo(length, kOrbisDirectAlign);

  // Reserve mapping: still created with PROT_NONE, but *backed* by direct mem
  // so the reservation sits in the same pool as future commits.
  const int32_t orbis_prot_initial =
      (allocation_type == AllocationType::kReserve) ? 0 : ToOrbisCpuProt(access);

  // Use sceKernelAllocateDirectMemory (6-arg variant) because the OpenOrbis
  // header declares sceKernelAllocateMainDirectMemory's last arg as off_t
  // (by value) rather than off_t* — passing an address via that signature
  // is not portable. The 6-arg form matches user_malloc_ps4.cpp's pattern.
  off_t        dmem_offset  = 0;
  size_t       effective_len = aligned_length;
  int32_t      res          = sceKernelAllocateDirectMemory(
      /*searchStart*/ 0,
      /*searchEnd*/   SCE_KERNEL_MAIN_DMEM_SIZE,
      /*len*/         effective_len,
      /*alignment*/   kOrbisDirectAlign,
      /*memoryType*/  SCE_KERNEL_WB_ONION,
      /*physAddrOut*/ &dmem_offset);
  if (res < 0) {
    // Fallback ladder: try progressively smaller reservations so a slightly
    // over-budget request (e.g. guest wants 4.5 GB, only 4 GB free) still
    // succeeds with a reduced footprint. We log the reduction once.
    static bool s_warned = false;
    if (diagnostics::IsEnabled(diagnostics::Category::kLogging) &&
        !s_warned) {
      s_warned = true;
      std::fprintf(stderr,
                   "[memory_ps4] AllocateDirectMemory(%zu) failed "
                   "(res=0x%08x); falling back to smaller sizes.\n",
                   aligned_length, static_cast<unsigned>(res));
    }
    const size_t kFallbacks[] = {
        4ULL * 1024ULL * 1024ULL * 1024ULL,  // 4 GB
        2ULL * 1024ULL * 1024ULL * 1024ULL,  // 2 GB
        1ULL * 1024ULL * 1024ULL * 1024ULL,  // 1 GB
    };
    for (size_t fb : kFallbacks) {
      if (fb >= aligned_length) continue;  // only try *smaller* than the ask
      res = sceKernelAllocateDirectMemory(0, SCE_KERNEL_MAIN_DMEM_SIZE, fb,
                                          kOrbisDirectAlign,
                                          SCE_KERNEL_WB_ONION, &dmem_offset);
      if (res >= 0) {
        if (diagnostics::IsEnabled(diagnostics::Category::kLogging)) {
          std::fprintf(stderr,
                       "[memory_ps4] Reduced reservation: requested=%zu got=%zu\n",
                       aligned_length, fb);
        }
        effective_len = fb;
        break;
      }
    }
    if (res < 0) {
      std::fprintf(stderr,
                   "[memory_ps4] All direct-memory fallbacks exhausted\n");
      return nullptr;
    }
  }

  const size_t final_length = effective_len;
  void*        mapped       = base_address;
  // MAP_FIXED is honored by sceKernelMapNamedDirectMemory when base_address
  // is non-null. If null, the kernel picks an address.
  const int32_t map_flags = base_address ? MAP_FIXED : 0;
  res = sceKernelMapNamedDirectMemory(&mapped, final_length, orbis_prot_initial,
                                      map_flags, dmem_offset, 0, "LR_Guest");
  if (res < 0) {
    sceKernelReleaseDirectMemory(dmem_offset, final_length);
    std::fprintf(stderr,
                 "[memory_ps4] MapNamedDirectMemory(len=%zu, prot=0x%x) "
                 "failed (res=0x%08x)\n",
                 final_length, static_cast<unsigned>(orbis_prot_initial),
                 static_cast<unsigned>(res));
    return nullptr;
  }

  {
    std::lock_guard<std::mutex> lock(g_dmem_mutex);
    g_dmem_table[mapped] = DirectMemEntry{dmem_offset, final_length};
  }
  return mapped;

#else  // Non-PS4 fallback — retained so the file remains compilable if the
       // CMake gate ever slips. macOS/Linux builds use memory_mac.cpp /
       // memory_posix.cpp instead and never reach this path.
  const uint32_t prot_requested = ToPosixProtectFlags(access);

  int prot_initial = 0;
  switch (allocation_type) {
    case AllocationType::kReserve:
      prot_initial = PROT_NONE;
      break;
    case AllocationType::kCommit:
    case AllocationType::kReserveCommit:
    default:
      prot_initial = static_cast<int>(prot_requested);
      break;
  }

  int flags = MAP_PRIVATE | MAP_ANONYMOUS;
#  if defined(MAP_FIXED_NOREPLACE)
  if (base_address) {
    flags |= MAP_FIXED_NOREPLACE;
  }
#  else
  if (base_address) {
    flags |= MAP_FIXED;
  }
#  endif

  void* result = mmap(base_address, length, prot_initial, flags, -1, 0);
  return result == MAP_FAILED ? nullptr : result;
#endif
}

bool DeallocFixed(void* base_address, size_t length, DeallocationType deallocation_type) {
  switch (deallocation_type) {
    case DeallocationType::kDecommit: {
      // Decommit: drop permissions but keep the address-space reservation.
      // The direct-memory backing stays tied to the virtual range; we just
      // disable access. madvise doesn't apply to direct-memory mappings.
#if REX_PLATFORM_PS4
      return sceKernelMprotect(base_address, length, 0) == 0;
#else
      if (mprotect(base_address, length, PROT_NONE) != 0) {
        return false;
      }
#  if defined(MADV_DONTNEED)
      (void)madvise(base_address, length, MADV_DONTNEED);
#  endif
      return true;
#endif
    }
    case DeallocationType::kRelease: {
#if REX_PLATFORM_PS4
      // Look up the dmem backing, unmap the virtual range, release physical.
      DirectMemEntry entry{};
      bool           found = false;
      {
        std::lock_guard<std::mutex> lock(g_dmem_mutex);
        auto                        it = g_dmem_table.find(base_address);
        if (it != g_dmem_table.end()) {
          entry = it->second;
          g_dmem_table.erase(it);
          found = true;
        }
      }
      if (!found) {
        // Fall back to munmap — safest for pointers we didn't allocate via
        // AllocFixed (shouldn't happen in practice but avoids leaks).
        return munmap(base_address, length) == 0;
      }
      bool ok = (sceKernelMunmap(base_address, entry.length) == 0);
      ok     &= (sceKernelReleaseDirectMemory(entry.dmem_offset, entry.length) == 0);
      return ok;
#else
      return munmap(base_address, length) == 0;
#endif
    }
    default:
      assert_always();
      return false;
  }
}

bool Protect(void* base_address, size_t length, PageAccess access, PageAccess* out_old_access) {
  if (out_old_access) {
    // No /proc/self/maps on PS4 — best effort only.
    *out_old_access = PageAccess::kNoAccess;
  }
#if REX_PLATFORM_PS4
  // sceKernelMprotect on a direct-memory mapping takes CPU_* prot bits.
  return sceKernelMprotect(base_address, length, ToOrbisCpuProt(access)) == 0;
#else
  uint32_t prot = ToPosixProtectFlags(access);
  return mprotect(base_address, length, prot) == 0;
#endif
}

bool QueryProtect(void* base_address, size_t& length, PageAccess& access_out) {
  // No /proc/self/maps on PS4 (FreeBSD kernel). sceKernelQueryMemoryProtection
  // exists but returns raw CPU/GPU bits we'd have to round-trip back into the
  // rex enum; leaving it as a stub matches memory_mac.cpp semantics.
  // TODO(ps4): wire up sceKernelQueryMemoryProtection if a caller starts
  // relying on out_old_access beyond "non-null / null".
  static bool s_warned = false;
  if (diagnostics::IsEnabled(diagnostics::Category::kLogging) && !s_warned) {
    s_warned = true;
    std::fprintf(stderr,
                 "[memory_ps4] QueryProtect is a stub on PS4; returning "
                 "kNoAccess. Wire sceKernelQueryMemoryProtection if needed.\n");
  }
  (void)base_address;
  access_out = PageAccess::kNoAccess;
  length     = 0;
  return false;
}

FileMappingHandle CreateFileMappingHandle(const std::filesystem::path& path, size_t length,
                                          PageAccess access, bool commit) {
  int oflag;
  switch (access) {
    case PageAccess::kNoAccess:
      oflag = 0;
      break;
    case PageAccess::kReadOnly:
    case PageAccess::kExecuteReadOnly:
      oflag = O_RDONLY;
      break;
    case PageAccess::kReadWrite:
    case PageAccess::kExecuteReadWrite:
      oflag = O_RDWR;
      break;
    default:
      assert_always();
      return kFileMappingHandleInvalid;
  }
  (void)commit;

  oflag |= O_CREAT;
  auto full_path = MakeShmName(path);
  int ret = shm_open(full_path.c_str(), oflag, 0777);
  if (ret < 0) {
    return kFileMappingHandleInvalid;
  }
  if (ftruncate(ret, static_cast<off_t>(length)) != 0) {
    close(ret);
    shm_unlink(full_path.c_str());
    return kFileMappingHandleInvalid;
  }
  return static_cast<FileMappingHandle>(ret);
}

void CloseFileMappingHandle(FileMappingHandle handle, const std::filesystem::path& path) {
  close(static_cast<int>(handle));
  auto full_path = MakeShmName(path);
  shm_unlink(full_path.c_str());
}

void* MapFileView(FileMappingHandle handle, void* base_address, size_t length, PageAccess access,
                  size_t file_offset) {
  const size_t page = page_size();
  if (file_offset % page != 0) {
    return nullptr;
  }

  int flags = MAP_SHARED;
  // File views replace prior anonymous reservations, so we need MAP_FIXED.
  if (base_address) {
    flags |= MAP_FIXED;
  }

  uint32_t prot = ToPosixProtectFlags(access);
  void* result = mmap(base_address, length, prot, flags, static_cast<int>(handle),
                      static_cast<off_t>(file_offset));
  if (result == MAP_FAILED) {
    return nullptr;
  }

  if (base_address && result != base_address) {
    munmap(result, length);
    return nullptr;
  }

  return result;
}

bool UnmapFileView(FileMappingHandle handle, void* base_address, size_t length) {
  (void)handle;
  return munmap(base_address, length) == 0;
}

}  // namespace memory
}  // namespace rex
