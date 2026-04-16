/**
 * @file        core/memory_mac.cpp
 * @brief       macOS memory file derived from the POSIX backend
 *
 * @license     BSD 3-Clause License
 */

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

#include <rex/math.h>
#include <rex/memory/utils.h>
#include <rex/platform.h>
#include <rex/string.h>

namespace rex {
namespace memory {

namespace {

struct HostPageRange {
  void* base = nullptr;
  size_t length = 0;
};

// Apple Silicon uses 16 KB host pages while the guest heaps still allocate at
// 4 KB in multiple regions. Align host protection updates to the enclosing host
// page range so fixed guest reserve / commit calls don't trip over page-size
// mismatches.
HostPageRange AlignToHostPageRange(void* base_address, size_t length) {
  const size_t host_page_size = page_size();
  const uintptr_t base = reinterpret_cast<uintptr_t>(base_address);
  const uintptr_t aligned_base = base & ~(uintptr_t(host_page_size - 1));
  const uintptr_t end = rex::round_up(base + length, host_page_size);
  return {reinterpret_cast<void*>(aligned_base), end - aligned_base};
}

}  // namespace

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

bool IsWritableExecutableMemorySupported() {
  return true;
}

void* AllocFixed(void* base_address, size_t length, AllocationType allocation_type,
                 PageAccess access) {
  // The guest memory space is already mapped into the process by MapViews.
  // On macOS we only need to adjust protections for fixed guest allocations
  // rather than replacing the mapping with a new mmap.
  if (base_address && length) {
    const auto host_range = AlignToHostPageRange(base_address, length);
    const int prot =
        allocation_type == AllocationType::kReserve ? PROT_NONE : ToPosixProtectFlags(access);
    if (mprotect(host_range.base, host_range.length, prot) == 0) {
      return base_address;
    }
    return nullptr;
  }

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

  // Match the fixed-allocation behavior above: any guest-protection change must
  // be expanded to the host page that actually owns the mapping.
  const auto host_range = AlignToHostPageRange(base_address, length);
  return mprotect(host_range.base, host_range.length, ToPosixProtectFlags(access)) == 0;
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

  // These file-backed guest views replace the anonymous reservation created
  // during heap setup, so macOS needs MAP_FIXED here as well.
  if (base_address) {
    flags |= MAP_FIXED;
  }

  void* result = mmap(base_address, length, ToPosixProtectFlags(access), flags,
                      static_cast<int>(handle), static_cast<off_t>(file_offset));
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
