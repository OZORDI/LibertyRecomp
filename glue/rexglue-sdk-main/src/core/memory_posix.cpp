/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2020 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
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

#ifdef __APPLE__
// macOS uses 64-bit file offsets natively; no *64 API variants needed.
#  define ftruncate64 ftruncate
#  define mmap64      mmap
#  define fstat64     fstat
typedef off_t off64_t;
#endif  // __APPLE__

#include <rex/string.h>

#if REX_PLATFORM_ANDROID
#include <string.h>

#include <dlfcn.h>
#include <sys/ioctl.h>

#include <linux/ashmem.h>

// TODO(tomc): Android or maybe na. idk
// #include "xenia/base/main_android.h"
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

#if REX_PLATFORM_ANDROID
// May be null if no dynamically loaded functions are required.
static void* libandroid_;
// API 26+.
static int (*android_ASharedMemory_create_)(const char* name, size_t size);

void AndroidInitialize() {
  if (rex::GetAndroidApiLevel() >= 26) {
    libandroid_ = dlopen("libandroid.so", RTLD_NOW);
    assert_not_null(libandroid_);
    if (libandroid_) {
      android_ASharedMemory_create_ = reinterpret_cast<decltype(android_ASharedMemory_create_)>(
          dlsym(libandroid_, "ASharedMemory_create"));
      assert_not_null(android_ASharedMemory_create_);
    }
  }
}

void AndroidShutdown() {
  android_ASharedMemory_create_ = nullptr;
  if (libandroid_) {
    dlclose(libandroid_);
    libandroid_ = nullptr;
  }
}
#endif

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

// TODO(tomc): this needs to go somewhere else. we should utilize the platform namespace more.
#if REX_PLATFORM_LINUX
namespace {

struct LinuxMapEntry {
  uintptr_t start = 0;
  uintptr_t end = 0;
  char perms[5] = {};
};

// Parse a line from /proc/self/maps into a LinuxMapEntry
static bool ParseProcMapsLine(const std::string& line, LinuxMapEntry& out) {
  out = LinuxMapEntry{};
  unsigned long long start = 0, end = 0;
  char perms[5] = {};
  const int matched = std::sscanf(line.c_str(), "%llx-%llx %4s", &start, &end, perms);
  if (matched < 3)
    return false;
  out.start = static_cast<uintptr_t>(start);
  out.end = static_cast<uintptr_t>(end);
  std::memcpy(out.perms, perms, sizeof(out.perms));
  return out.start < out.end;
}

// Find the mapping entry in /proc/self/maps that contains the given address
static bool FindEntryForAddress(void* address, LinuxMapEntry& out_entry) {
  const uintptr_t addr = reinterpret_cast<uintptr_t>(address);
  std::ifstream maps("/proc/self/maps");
  if (!maps.is_open())
    return false;
  std::string line;
  while (std::getline(maps, line)) {
    LinuxMapEntry e;
    if (!ParseProcMapsLine(line, e))
      continue;
    if (addr >= e.start && addr < e.end) {
      out_entry = e;
      return true;
    }
  }
  return false;
}

// Check if [base, base+length) is fully covered by existing mappings (no gaps)
static bool IsRangeFullyMapped(void* base_address, size_t length) {
  if (!base_address || length == 0)
    return false;

  const uintptr_t begin = reinterpret_cast<uintptr_t>(base_address);
  const uintptr_t end = begin + length;
  if (end < begin) {  // overflow check
    return false;
  }

  std::ifstream maps("/proc/self/maps");
  if (!maps.is_open())
    return false;

  uintptr_t cursor = begin;
  std::string line;
  while (std::getline(maps, line)) {
    LinuxMapEntry e;
    if (!ParseProcMapsLine(line, e))
      continue;
    if (e.end <= cursor)
      continue;
    if (e.start > cursor)
      return false;  // gap found
    cursor = e.end;
    if (cursor >= end)
      return true;
  }
  return cursor >= end;
}

// Convert /proc/self/maps permission chars to PageAccess
static PageAccess PermsToPageAccess(const char perms[5]) {
  const bool r = perms[0] == 'r';
  const bool w = perms[1] == 'w';
  const bool x = perms[2] == 'x';

  if (!r && !w && !x)
    return PageAccess::kNoAccess;
  if (x)
    return w ? PageAccess::kExecuteReadWrite : PageAccess::kExecuteReadOnly;
  return w ? PageAccess::kReadWrite : PageAccess::kReadOnly;
}

}  // namespace
#endif  // REX_PLATFORM_LINUX

#ifdef __APPLE__
#include <mach/mach.h>
// Returns true if [base, base+length) is already mapped in this process
static bool IsMacOSRangeAlreadyMapped(void* base_address, size_t length) {
  if (!base_address || length == 0) return false;
  const vm_address_t addr = reinterpret_cast<vm_address_t>(base_address);
  const vm_address_t end  = addr + length;
  vm_address_t cur = addr;
  while (cur < end) {
    vm_size_t region_size = 0;
    struct vm_region_basic_info_64 info;
    mach_msg_type_number_t info_count = VM_REGION_BASIC_INFO_COUNT_64;
    memory_object_name_t object_name;
    kern_return_t kr = vm_region_64(mach_task_self(), &cur, &region_size,
                                    VM_REGION_BASIC_INFO_64,
                                    reinterpret_cast<vm_region_info_t>(&info),
                                    &info_count, &object_name);
    if (kr != KERN_SUCCESS) return false;  // gap — not mapped
    if (cur > addr) return false;           // gap before region
    cur += region_size;
  }
  return true;
}
#endif  // __APPLE__

void* AllocFixed(void* base_address, size_t length, AllocationType allocation_type,
                 PageAccess access) {
  // Emulates Windows VirtualAlloc behavior:
  // - Reserve: create PROT_NONE mapping to hold address space
  // - Commit on existing reservation: mprotect to enable access
  // - New allocation: mmap with MAP_FIXED_NOREPLACE (never silently replace)
  const uint32_t prot_requested = ToPosixProtectFlags(access);

  // ─── macOS path ────────────────────────────────────────────────────────────
  // On macOS, MAP_FIXED_NOREPLACE is not available. When committing into the
  // pre-mapped shared-memory region (set up by MapViews via shm_open/MAP_FIXED)
  // we must use mprotect rather than a new anonymous mmap.
#ifdef __APPLE__
  if (base_address &&
      (allocation_type == AllocationType::kCommit ||
       allocation_type == AllocationType::kReserveCommit)) {
    // Try mprotect first — succeeds if the range is already file-mapped
    if (IsMacOSRangeAlreadyMapped(base_address, length)) {
      if (mprotect(base_address, length, static_cast<int>(prot_requested)) == 0) {
        return base_address;
      }
    }
  }
  if (allocation_type == AllocationType::kReserve && base_address) {
    // Reserve: map PROT_NONE over any existing mapping (MAP_FIXED replaces)
    void* result = mmap(base_address, length, PROT_NONE,
                        MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
    return (result != MAP_FAILED) ? result : nullptr;
  }
  // Fallback: non-fixed allocation (macOS only, should rarely hit this)
  {
    void* r = mmap(nullptr, length, static_cast<int>(prot_requested),
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    return (r != MAP_FAILED) ? r : nullptr;
  }
#endif  // __APPLE__

  // ─── Linux path ────────────────────────────────────────────────────────────
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
  if (result != MAP_FAILED) {
    return result;
  }
#if defined(MAP_FIXED_NOREPLACE) && REX_PLATFORM_LINUX
  if (errno == EEXIST && base_address &&
      (allocation_type == AllocationType::kCommit ||
       allocation_type == AllocationType::kReserveCommit)) {
    if (IsRangeFullyMapped(base_address, length)) {
      if (mprotect(base_address, length, static_cast<int>(prot_requested)) == 0) {
        return base_address;
      }
    }
  }
#endif

  return nullptr;
}

bool DeallocFixed(void* base_address, size_t length, DeallocationType deallocation_type) {
  switch (deallocation_type) {
    case DeallocationType::kDecommit: {
      // Decommit: remove access first, then release physical pages
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
      // how we get here? :(
      assert_always();
      return false;
  }
}

bool Protect(void* base_address, size_t length, PageAccess access, PageAccess* out_old_access) {
  if (out_old_access) {
    *out_old_access = PageAccess::kNoAccess;
  }

#if REX_PLATFORM_LINUX
  // NOTE(tomc): we may want to look at doing this differently. it should work for now
  //             but there is a TOCTOU window between reading and changing.
  //             This really shouldn't be an issue since VirtualProtect on Windows isn't truly
  //             atomic in a mutli-threaded process either, but it's something to be aware of.
  // Query old access before changing, if the caller needs it
  if (out_old_access) {
    LinuxMapEntry e;
    if (FindEntryForAddress(base_address, e)) {
      *out_old_access = PermsToPageAccess(e.perms);
    }
  }
#endif

  uint32_t prot = ToPosixProtectFlags(access);
  return mprotect(base_address, length, prot) == 0;
}

bool QueryProtect(void* base_address, size_t& length, PageAccess& access_out) {
#if !REX_PLATFORM_LINUX
  access_out = PageAccess::kNoAccess;
  length = 0;
  return false;
#else
  access_out = PageAccess::kNoAccess;
  length = 0;

  LinuxMapEntry e;
  if (!FindEntryForAddress(base_address, e)) {
    return false;
  }

  const uintptr_t addr = reinterpret_cast<uintptr_t>(base_address);
  length = static_cast<size_t>(e.end - addr);
  access_out = PermsToPageAccess(e.perms);

  return true;
#endif
}

FileMappingHandle CreateFileMappingHandle(const std::filesystem::path& path, size_t length,
                                          PageAccess access, bool commit) {
#if REX_PLATFORM_ANDROID
  // TODO(Triang3l): Check if memfd can be used instead on API 30+.
  if (android_ASharedMemory_create_) {
    int sharedmem_fd = android_ASharedMemory_create_(path.c_str(), length);
    return sharedmem_fd >= 0 ? static_cast<FileMappingHandle>(sharedmem_fd)
                             : kFileMappingHandleInvalid;
  }

  // Use /dev/ashmem on API versions below 26, which added ASharedMemory.
  // /dev/ashmem was disabled on API 29 for apps targeting it.
  // https://chromium.googlesource.com/chromium/src/+/master/third_party/ashmem/ashmem-dev.c
  int ashmem_fd = open("/" ASHMEM_NAME_DEF, O_RDWR);
  if (ashmem_fd < 0) {
    return kFileMappingHandleInvalid;
  }
  char ashmem_name[ASHMEM_NAME_LEN];
  strlcpy(ashmem_name, path.c_str(), rex::countof(ashmem_name));
  if (ioctl(ashmem_fd, ASHMEM_SET_NAME, ashmem_name) < 0 ||
      ioctl(ashmem_fd, ASHMEM_SET_SIZE, length) < 0) {
    close(ashmem_fd);
    return kFileMappingHandleInvalid;
  }
  return static_cast<FileMappingHandle>(ashmem_fd);
#else
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
  oflag |= O_CREAT;
  auto full_path = MakeShmName(path);
  int ret = shm_open(full_path.c_str(), oflag, 0777);
  if (ret < 0) {
    return kFileMappingHandleInvalid;
  }
  if (ftruncate64(ret, static_cast<off_t>(length)) != 0) {
    close(ret);
    shm_unlink(full_path.c_str());
    return kFileMappingHandleInvalid;
  }
  return static_cast<FileMappingHandle>(ret);
#endif
}

void CloseFileMappingHandle(FileMappingHandle handle, const std::filesystem::path& path) {
  close(static_cast<int>(handle));
#if !REX_PLATFORM_ANDROID
  auto full_path = MakeShmName(path);
  shm_unlink(full_path.c_str());
#endif
}

void* MapFileView(FileMappingHandle handle, void* base_address, size_t length, PageAccess access,
                  size_t file_offset) {
  // file_offset must be page-aligned
  const size_t page = page_size();
  if (file_offset % page != 0) {
    return nullptr;
  }

  int flags = MAP_SHARED;

  // For file views, we need MAP_FIXED to replace existing reservations.
  // The emulator reserves address space first, then maps file views into it.
  // MAP_FIXED_NOREPLACE would fail with EEXIST in this case.
  if (base_address) {
#ifdef __APPLE__
    // On macOS, MAP_FIXED silently replaces *any* existing mapping, including
    // the binary's own code pages.  The scan loop in Memory::Initialize()
    // probes 1<<32, 1<<33, ... looking for a completely free window.  The
    // binary is loaded near 0x100000000 but offset by an ASLR slide, so the
    // start of that 1GB window appears unmapped even though the binary sits
    // inside it.  Use vm_region_64 to detect ANY overlap with the requested
    // range and bail out so the loop can try the next candidate.
    {
      vm_address_t check_addr = reinterpret_cast<vm_address_t>(base_address);
      vm_size_t    region_sz  = 0;
      struct vm_region_basic_info_64 vr_info;
      mach_msg_type_number_t         vr_cnt = VM_REGION_BASIC_INFO_COUNT_64;
      memory_object_name_t           vr_obj;
      kern_return_t kr = vm_region_64(mach_task_self(), &check_addr, &region_sz,
                                      VM_REGION_BASIC_INFO_64,
                                      reinterpret_cast<vm_region_info_t>(&vr_info),
                                      &vr_cnt, &vr_obj);
      if (kr == KERN_SUCCESS) {
        // A mapping was found at or after base_address.  If it starts before
        // the end of our requested range it overlaps → refuse to MAP_FIXED.
        vm_address_t range_end =
            reinterpret_cast<vm_address_t>(base_address) + length;
        if (check_addr < range_end) {
          return nullptr;
        }
      }
    }
#endif
    flags |= MAP_FIXED;
  }

  uint32_t prot = ToPosixProtectFlags(access);
  void* result = mmap64(base_address, length, prot, flags, static_cast<int>(handle),
                        static_cast<off_t>(file_offset));
  if (result == MAP_FAILED) {
    return nullptr;
  }

  // Verify we got the address we asked for
  if (base_address && result != base_address) {
    munmap(result, length);
    return nullptr;
  }

  return result;
}

bool UnmapFileView(FileMappingHandle handle, void* base_address, size_t length) {
  return munmap(base_address, length) == 0;
}

}  // namespace memory
}  // namespace rex
