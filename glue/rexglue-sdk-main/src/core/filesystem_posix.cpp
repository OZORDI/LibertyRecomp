/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2022 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <iostream>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <rex/assert.h>
#include <rex/filesystem.h>
#include <rex/logging.h>
#include <rex/platform.h>
#include <rex/string.h>

#include <dirent.h>

// <ftw.h> (nftw) and <pwd.h> (getpwuid_r) are not available in Switch newlib.
// PS4's FreeBSD libc has them, but we don't use them there either since
// /proc doesn't exist and there is no $HOME environment variable.
#if !REX_PLATFORM_SWITCH && !REX_PLATFORM_PS4
#  include <ftw.h>
#  include <libgen.h>
#  include <pwd.h>
#endif

// Platform-specific executable resolution
#if REX_PLATFORM_PS4
// PS4 homebrew (OpenOrbis) — application always unpacked at /app0/
// No /proc filesystem exists on FreeBSD-derived PS4 kernel.
#elif REX_PLATFORM_SWITCH
// Switch (libnx) — NRO loaded from SD; envGetNextLoadPath() gives NRO path.
#  include <switch.h>
#elif defined(__APPLE__)
// macOS uses 64-bit off_t natively — no *64 variants needed.
#  include <mach-o/dyld.h>      // _NSGetExecutablePath
#endif

#if defined(__APPLE__) || REX_PLATFORM_PS4 || REX_PLATFORM_SWITCH
#  define fseeko64    fseeko
#  define ftello64    ftello
#  define ftruncate64 ftruncate
typedef off_t off64_t;
#endif


namespace rex {

std::string path_to_utf8(const std::filesystem::path& path) {
  return path.string();
}

std::u16string path_to_utf16(const std::filesystem::path& path) {
  return rex::string::to_utf16(path.string());
}

std::filesystem::path to_path(const std::string_view source) {
  return source;
}

std::filesystem::path to_path(const std::u16string_view source) {
  return rex::string::to_utf8(source);
}

namespace filesystem {

std::filesystem::path GetExecutablePath() {
#if REX_PLATFORM_PS4
  // PS4: the application package is always extracted to /app0/ by the kernel.
  // There is no /proc filesystem on PS4's FreeBSD-derived OS.
  return std::filesystem::path("/app0/eboot.bin");
#elif REX_PLATFORM_SWITCH
  // Switch: the NRO is loaded from the SD card; envGetNextLoadPath() gives the
  // loader-assigned path (e.g. "sdmc:/switch/LibertyRecomp/LibertyRecomp.nro").
  // Fall back to romfs:/ root if the loader did not set a path.
  const char* nro_path = envGetNextLoadPath();
  if (nro_path && nro_path[0] != '\0')
    return std::filesystem::path(nro_path);
  return std::filesystem::path("romfs:/");
#elif defined(__APPLE__)
  char buff[FILENAME_MAX] = "";
  uint32_t buffsize = FILENAME_MAX;
  _NSGetExecutablePath(buff, &buffsize);
  return std::filesystem::canonical(buff);
#else
  // Linux: read the /proc/self/exe symlink.
  char buff[FILENAME_MAX] = "";
  readlink("/proc/self/exe", buff, FILENAME_MAX);
  return std::string(buff);
#endif
}

std::filesystem::path GetExecutableFolder() {
  return GetExecutablePath().parent_path();
}

std::filesystem::path GetUserFolder() {
#if REX_PLATFORM_PS4
  // PS4: save data is mounted at /savedata0/ by os::savedata::MountSaveData()
  // before this is called. Return that mount point as the user data root.
  // If it hasn't been mounted yet we return /app0/ as a safe read-only fallback
  // so the path is always valid; SetupVfs will refuse to write to a read-only device.
  return std::filesystem::path("/savedata0");
#elif REX_PLATFORM_SWITCH
  // Switch: account save data is mounted as "save:/" by switch_main's __appInit
  // via fsdevMountSaveData(). For config/cache that doesn't need the save-data
  // journal (screenshots, settings) we use the SD card.
  // SetupVfs mounts save:/ as the writable user-data VFS device.
  return std::filesystem::path("save:/");
#else
  // Desktop: honour XDG_DATA_HOME, then $HOME/.local/share, then passwd entry.
  char* home = std::getenv("XDG_DATA_HOME");
  if (home) {
    return std::string(home);
  }
  home = std::getenv("HOME");
  if (home == nullptr) {
    struct passwd pw1;
    struct passwd* pw;
    char buf[4096];
    getpwuid_r(getuid(), &pw1, buf, sizeof(buf), &pw);
    assert(&pw1 == pw);
    home = pw->pw_dir;
  }
  return std::filesystem::path(home) / ".local" / "share";
#endif
}

FILE* OpenFile(const std::filesystem::path& path, const std::string_view mode) {
  return fopen(path.c_str(), std::string(mode).c_str());
}

bool Seek(FILE* file, int64_t offset, int origin) {
  return fseeko64(file, off64_t(offset), origin) == 0;
}

int64_t Tell(FILE* file) {
  return int64_t(ftello64(file));
}

bool TruncateStdioFile(FILE* file, uint64_t length) {
  if (fflush(file)) {
    return false;
  }
  int64_t position = Tell(file);
  if (position < 0) {
    return false;
  }
  if (ftruncate64(fileno(file), off64_t(length))) {
    return false;
  }
  if (uint64_t(position) > length) {
    if (!Seek(file, 0, SEEK_END)) {
      return false;
    }
  }
  return true;
}

// nftw-based recursive removal: only available on platforms that have <ftw.h>.
// PS4 and Switch use std::filesystem::remove_all() as the portable fallback.
#if !REX_PLATFORM_SWITCH && !REX_PLATFORM_PS4
static int removeCallback(const char* fpath, const struct stat* sb, int typeflag,
                          struct FTW* ftwbuf) {
  int rv = remove(fpath);
  return rv;
}
#endif

static uint64_t convertUnixtimeToWinFiletime(time_t unixtime) {
  // Linux uses number of seconds since 1/1/1970, and Windows uses
  // number of nanoseconds since 1/1/1601
  // so we convert linux time to nanoseconds and then add the number of
  // nanoseconds from 1601 to 1970
  // see https://msdn.microsoft.com/en-us/library/ms724228
  uint64_t filetime = filetime = (unixtime * 10000000) + 116444736000000000;
  return filetime;
}

bool CreateEmptyFile(const std::filesystem::path& path) {
  int file = creat(path.c_str(), 0774);
  if (file >= 0) {
    close(file);
    return true;
  }
  return false;
}

class PosixFileHandle : public FileHandle {
 public:
  PosixFileHandle(std::filesystem::path path, int handle)
      : FileHandle(std::move(path)), handle_(handle) {}
  ~PosixFileHandle() override {
    close(handle_);
    handle_ = -1;
  }
  bool Read(size_t file_offset, void* buffer, size_t buffer_length,
            size_t* out_bytes_read) override {
    ssize_t out = pread(handle_, buffer, buffer_length, file_offset);
    *out_bytes_read = out;
    return out >= 0 ? true : false;
  }
  bool Write(size_t file_offset, const void* buffer, size_t buffer_length,
             size_t* out_bytes_written) override {
    ssize_t out = pwrite(handle_, buffer, buffer_length, file_offset);
    *out_bytes_written = out;
    return out >= 0 ? true : false;
  }
  bool SetLength(size_t length) override { return ftruncate(handle_, length) >= 0 ? true : false; }
  void Flush() override { fsync(handle_); }

 private:
  int handle_ = -1;
};

std::unique_ptr<FileHandle> FileHandle::OpenExisting(const std::filesystem::path& path,
                                                     uint32_t desired_access) {
  int open_access = 0;
  if (desired_access & FileAccess::kGenericRead) {
    open_access |= O_RDONLY;
  }
  if (desired_access & FileAccess::kGenericWrite) {
    open_access |= O_WRONLY;
  }
  if (desired_access & FileAccess::kGenericExecute) {
    open_access |= O_RDONLY;
  }
  if (desired_access & FileAccess::kGenericAll) {
    open_access |= O_RDWR;
  }
  if (desired_access & FileAccess::kFileReadData) {
    open_access |= O_RDONLY;
  }
  if (desired_access & FileAccess::kFileWriteData) {
    open_access |= O_WRONLY;
  }
  if (desired_access & FileAccess::kFileAppendData) {
    open_access |= O_APPEND;
  }
  int handle = open(path.c_str(), open_access);
  if (handle == -1) {
    // TODO(benvanik): pick correct response.
    return nullptr;
  }
  return std::make_unique<PosixFileHandle>(path, handle);
}

bool GetInfo(const std::filesystem::path& path, FileInfo* out_info) {
  struct stat st;
  if (stat(path.c_str(), &st) == 0) {
    if (S_ISDIR(st.st_mode)) {
      out_info->type = FileInfo::Type::kDirectory;
      out_info->total_size = 0;
    } else {
      out_info->type = FileInfo::Type::kFile;
      out_info->total_size = st.st_size;
    }
    out_info->path = path.parent_path();
    out_info->name = path.filename();
    out_info->create_timestamp = convertUnixtimeToWinFiletime(st.st_ctime);
    out_info->access_timestamp = convertUnixtimeToWinFiletime(st.st_atime);
    out_info->write_timestamp = convertUnixtimeToWinFiletime(st.st_mtime);
    return true;
  }
  return false;
}

std::vector<FileInfo> ListFiles(const std::filesystem::path& path) {
  std::vector<FileInfo> result;

  DIR* dir = opendir(path.c_str());
  if (!dir) {
    return result;
  }

  while (auto ent = readdir(dir)) {
    if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
      continue;
    }

    FileInfo info;

    info.name = ent->d_name;
    struct stat st;
    stat((path / info.name).c_str(), &st);
    info.create_timestamp = convertUnixtimeToWinFiletime(st.st_ctime);
    info.access_timestamp = convertUnixtimeToWinFiletime(st.st_atime);
    info.write_timestamp = convertUnixtimeToWinFiletime(st.st_mtime);
    info.path = path;
    if (S_ISDIR(st.st_mode)) {
      info.type = FileInfo::Type::kDirectory;
      info.total_size = 0;
    } else {
      info.type = FileInfo::Type::kFile;
      info.total_size = st.st_size;
    }
    result.push_back(info);
  }
  closedir(dir);
  return result;
}

}  // namespace filesystem
}  // namespace rex
