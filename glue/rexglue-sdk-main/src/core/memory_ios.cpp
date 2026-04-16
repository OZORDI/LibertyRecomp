/**
 * @file        core/memory_ios.cpp
 * @brief       iOS virtual memory backend
 *
 * iOS is derived from the macOS/Darwin backend but adds two critical
 * restrictions:
 *
 *   1. PROT_EXEC cannot be set on mmap'd anonymous pages unless the process
 *      holds the dynamic-codesigning entitlement (MAP_JIT + signed JIT
 *      entitlement). LibertyRecomp does NOT JIT on iOS — rexglue emits
 *      static C++ recomp code that is linked into the app binary and therefore
 *      already lives in an executable, code-signed Mach-O segment. The guest
 *      4 GiB reservation only ever needs PROT_READ | PROT_WRITE for guest
 *      data, so we strip PROT_EXEC from every allocation and protect() call.
 *
 *   2. arm64e (A12+, iPhone XS and later) enables pointer authentication on
 *      all C function pointers. When a PAC'd host pointer is stored into a
 *      32-bit guest field (e.g. xam callback thunk pointers), the high bits
 *      that hold the auth signature collide with the guest address space
 *      layout. The PACStripGuestPointer helper strips the signature with
 *      ptrauth_strip so the guest sees a canonical host pointer.
 *
 * @license     BSD 3-Clause License
 */

#include <rex/platform.h>

#if REX_PLATFORM_IOS

#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#if __has_include(<ptrauth.h>)
#include <ptrauth.h>
#endif

#include <rex/math.h>
#include <rex/memory/utils.h>
#include <rex/string.h>

namespace rex {
namespace memory {

namespace {

struct HostPageRange {
  void* base = nullptr;
  size_t length = 0;
};

// iOS devices use 16 KiB host pages across the board (A7+). The guest still
// allocates in 4 KiB units, so expand every protection window to the enclosing
// 16 KiB host page range — same strategy used by the macOS backend.
HostPageRange AlignToHostPageRange(void* base_address, size_t length) {
  const size_t host_page_size = page_size();
  const uintptr_t base = reinterpret_cast<uintptr_t>(base_address);
  const uintptr_t aligned_base = base & ~(uintptr_t(host_page_size - 1));
  const uintptr_t end = rex::round_up(base + length, host_page_size);
  return {reinterpret_cast<void*>(aligned_base), end - aligned_base};
}

// iOS forbids PROT_EXEC on unsigned anonymous pages. All guest data lives in
// non-executable mappings; the generated recomp code lives in the app's
// signed __TEXT segment. Strip PROT_EXEC from every protection request.
inline int StripExec(int prot) {
  return prot & ~PROT_EXEC;
}

}  // namespace

// Convert filesystem path to a valid shm_open name.
static std::string MakeShmName(const std::filesystem::path& path) {
  std::string name = path.string();
  for (char& c : name) {
    if (c == '/') c = '_';
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
  // NB: Execute variants degrade to non-executable on iOS. Code pages are
  // part of the signed binary; guest data never runs.
  switch (access) {
    case PageAccess::kNoAccess:
      return PROT_NONE;
    case PageAccess::kReadOnly:
      return PROT_READ;
    case PageAccess::kReadWrite:
      return PROT_READ | PROT_WRITE;
    case PageAccess::kExecuteReadOnly:
      return PROT_READ;  // PROT_EXEC stripped for iOS
    case PageAccess::kExecuteReadWrite:
      return PROT_READ | PROT_WRITE;  // PROT_EXEC stripped for iOS
    default:
      assert_unhandled_case(access);
      return PROT_NONE;
  }
}

bool IsWritableExecutableMemorySupported() {
  // No W^X JIT on iOS without the dynamic-codesigning entitlement.
  return false;
}

void* AllocFixed(void* base_address, size_t length, AllocationType allocation_type,
                 PageAccess access) {
  // Adjust protections on an existing mapping (the 4 GiB guest reservation
  // was established at startup via a single mmap).
  if (base_address && length) {
    const auto host_range = AlignToHostPageRange(base_address, length);
    const int prot = StripExec(allocation_type == AllocationType::kReserve
                                   ? PROT_NONE
                                   : ToPosixProtectFlags(access));
    if (mprotect(host_range.base, host_range.length, prot) == 0) {
      return base_address;
    }
    return nullptr;
  }

  const int prot_requested = StripExec(ToPosixProtectFlags(access));

  int prot_initial = 0;
  switch (allocation_type) {
    case AllocationType::kReserve:
      prot_initial = PROT_NONE;
      break;
    case AllocationType::kCommit:
    case AllocationType::kReserveCommit:
    default:
      prot_initial = prot_requested;
      break;
  }

  // NOTE: No MAP_JIT. No PROT_EXEC. Pure RW anonymous reservation.
  int flags = MAP_PRIVATE | MAP_ANONYMOUS;
#if defined(MAP_FIXED_NOREPLACE)
  if (base_address) {
    flags |= MAP_FIXED_NOREPLACE;
  }
#else
  if (base_address) {
    flags |= MAP_FIXED;
  }
#endif

  void* result = mmap(base_address, length, prot_initial, flags, -1, 0);
  return result == MAP_FAILED ? nullptr : result;
}

bool DeallocFixed(void* base_address, size_t length, DeallocationType deallocation_type) {
  switch (deallocation_type) {
    case DeallocationType::kDecommit: {
      if (mprotect(base_address, length, PROT_NONE) != 0) {
        return false;
      }
#if defined(MADV_DONTNEED)
      (void)madvise(base_address, length, MADV_DONTNEED);
#endif
#if defined(MADV_FREE)
      (void)madvise(base_address, length, MADV_FREE);
#endif
      return true;
    }
    case DeallocationType::kRelease: {
      return munmap(base_address, length) == 0;
    }
    default:
      assert_always();
      return false;
  }
}

bool Protect(void* base_address, size_t length, PageAccess access, PageAccess* out_old_access) {
  if (out_old_access) {
    *out_old_access = PageAccess::kNoAccess;
  }
  if (!base_address || !length) {
    return false;
  }
  const auto host_range = AlignToHostPageRange(base_address, length);
  const int prot = StripExec(ToPosixProtectFlags(access));
  return mprotect(host_range.base, host_range.length, prot) == 0;
}

bool QueryProtect(void* base_address, size_t& length, PageAccess& access_out) {
  (void)base_address;
  access_out = PageAccess::kNoAccess;
  length = 0;
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
  // iOS sandboxed shm_open: the name is scoped to the app's container,
  // which is exactly what we want — no cross-app sharing.
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
  // File views need MAP_FIXED so they stamp over the anonymous reservation.
  if (base_address) {
    flags |= MAP_FIXED;
  }

  const int prot = StripExec(ToPosixProtectFlags(access));
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

// ── arm64e pointer authentication helper ────────────────────────────────────
//
// On arm64e (iPhone XS / A12 and later, when the app is compiled with
// -arch arm64e or opts in to ptrauth ABI), every C function pointer carries
// a PAC signature in the high bits. If we store such a pointer into a
// guest-visible field (e.g. a 64-bit host callback slot inside a guest xam
// notification record), the signature must be stripped because:
//
//   * the guest has no way to validate or re-sign it,
//   * bit layout clashes with guest addressing assumptions,
//   * a later `blraa` through that pointer would trap.
//
// Callers that hand host function pointers to the guest MUST route them
// through PACStripGuestPointer first. Callers that only store data pointers
// can call this unconditionally — it is a no-op on non-arm64e builds.

void* PACStripGuestPointer(void* raw) {
#if defined(__ptrauth_function_pointer_type_discriminator) || defined(__arm64e__)
  return ptrauth_strip(raw, ptrauth_key_function_pointer);
#else
  return raw;
#endif
}

}  // namespace memory
}  // namespace rex

#endif  // REX_PLATFORM_IOS
