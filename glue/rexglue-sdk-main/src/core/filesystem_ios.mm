/**
 ******************************************************************************
 * ReXGlue iOS filesystem backend                                             *
 ******************************************************************************
 * Objective-C++ implementation that resolves sandboxed iOS paths via Cocoa   *
 * (NSBundle / NSFileManager / NSTemporaryDirectory) while delegating generic *
 * file I/O to POSIX syscalls which are fully available on iOS.               *
 ******************************************************************************
 */

#include <rex/platform.h>

#if REX_PLATFORM_IOS

#import  <Foundation/Foundation.h>
#import  <UIKit/UIKit.h>

#include <mach-o/dyld.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <dirent.h>
#include <libgen.h>

#include <rex/assert.h>
#include <rex/filesystem.h>
#include <rex/logging.h>
#include <rex/string.h>

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

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
static std::filesystem::path NSStringToPath(NSString* s) {
  if (!s) return std::filesystem::path();
  const char* utf8 = [s UTF8String];
  return utf8 ? std::filesystem::path(std::string(utf8)) : std::filesystem::path();
}

// ---------------------------------------------------------------------------
// Path accessors (iOS sandbox aware)
// ---------------------------------------------------------------------------
std::filesystem::path GetExecutablePath() {
  @autoreleasepool {
    // Preferred: NSBundle reports the real executable inside the .app.
    NSString* exec = [[NSBundle mainBundle] executablePath];
    if (exec) {
      return NSStringToPath(exec);
    }
  }
  // Fallback to dyld API.
  char buf[PATH_MAX] = {};
  uint32_t size = sizeof(buf);
  if (_NSGetExecutablePath(buf, &size) == 0) {
    return std::filesystem::path(std::string(buf));
  }
  return std::filesystem::path();
}

std::filesystem::path GetExecutableFolder() {
  return GetExecutablePath().parent_path();
}

std::filesystem::path GetUserFolder() {
  @autoreleasepool {
    NSFileManager* fm = [NSFileManager defaultManager];
    NSArray<NSURL*>* urls = [fm URLsForDirectory:NSDocumentDirectory
                                       inDomains:NSUserDomainMask];
    NSURL* first = [urls firstObject];
    if (first) {
      return NSStringToPath([first path]);
    }
  }
  return std::filesystem::path();
}

// Extra iOS-specific helpers. Not declared in rex/filesystem.h; reachable via
// forward decl from iOS-only callers (platform_paths_ios.cpp).
std::filesystem::path GetBundleResourcePath() {
  @autoreleasepool {
    NSString* rp = [[NSBundle mainBundle] resourcePath];
    return NSStringToPath(rp);
  }
}

std::filesystem::path GetTemporaryFolder() {
  @autoreleasepool {
    NSString* tmp = NSTemporaryDirectory();
    return NSStringToPath(tmp);
  }
}

std::filesystem::path GetCachesFolder() {
  @autoreleasepool {
    NSFileManager* fm = [NSFileManager defaultManager];
    NSArray<NSURL*>* urls = [fm URLsForDirectory:NSCachesDirectory
                                       inDomains:NSUserDomainMask];
    NSURL* first = [urls firstObject];
    if (first) {
      return NSStringToPath([first path]);
    }
  }
  return std::filesystem::path();
}

// ---------------------------------------------------------------------------
// Generic POSIX-backed file operations (iOS supports all of these).
// ---------------------------------------------------------------------------
FILE* OpenFile(const std::filesystem::path& path, const std::string_view mode) {
  return fopen(path.c_str(), std::string(mode).c_str());
}

bool Seek(FILE* file, int64_t offset, int origin) {
  return fseeko(file, static_cast<off_t>(offset), origin) == 0;
}

int64_t Tell(FILE* file) {
  return static_cast<int64_t>(ftello(file));
}

bool TruncateStdioFile(FILE* file, uint64_t length) {
  if (fflush(file)) return false;
  int64_t position = Tell(file);
  if (position < 0) return false;
  if (ftruncate(fileno(file), static_cast<off_t>(length))) return false;
  if (uint64_t(position) > length) {
    if (!Seek(file, 0, SEEK_END)) return false;
  }
  return true;
}

static uint64_t convertUnixtimeToWinFiletime(time_t unixtime) {
  return (static_cast<uint64_t>(unixtime) * 10000000ULL) + 116444736000000000ULL;
}

bool CreateEmptyFile(const std::filesystem::path& path) {
  int file = creat(path.c_str(), 0774);
  if (file >= 0) {
    close(file);
    return true;
  }
  return false;
}

class IosFileHandle : public FileHandle {
 public:
  IosFileHandle(std::filesystem::path path, int handle)
      : FileHandle(std::move(path)), handle_(handle) {}
  ~IosFileHandle() override {
    if (handle_ >= 0) close(handle_);
    handle_ = -1;
  }
  bool Read(size_t file_offset, void* buffer, size_t buffer_length,
            size_t* out_bytes_read) override {
    ssize_t out = pread(handle_, buffer, buffer_length, file_offset);
    *out_bytes_read = out < 0 ? 0 : static_cast<size_t>(out);
    return out >= 0;
  }
  bool Write(size_t file_offset, const void* buffer, size_t buffer_length,
             size_t* out_bytes_written) override {
    ssize_t out = pwrite(handle_, buffer, buffer_length, file_offset);
    *out_bytes_written = out < 0 ? 0 : static_cast<size_t>(out);
    return out >= 0;
  }
  bool SetLength(size_t length) override {
    return ftruncate(handle_, static_cast<off_t>(length)) >= 0;
  }
  void Flush() override { fsync(handle_); }

 private:
  int handle_ = -1;
};

std::unique_ptr<FileHandle> FileHandle::OpenExisting(const std::filesystem::path& path,
                                                     uint32_t desired_access) {
  int open_access = 0;
  if (desired_access & FileAccess::kGenericRead)    open_access |= O_RDONLY;
  if (desired_access & FileAccess::kGenericWrite)   open_access |= O_WRONLY;
  if (desired_access & FileAccess::kGenericExecute) open_access |= O_RDONLY;
  if (desired_access & FileAccess::kGenericAll)     open_access |= O_RDWR;
  if (desired_access & FileAccess::kFileReadData)   open_access |= O_RDONLY;
  if (desired_access & FileAccess::kFileWriteData)  open_access |= O_WRONLY;
  if (desired_access & FileAccess::kFileAppendData) open_access |= O_APPEND;

  int handle = open(path.c_str(), open_access);
  if (handle == -1) {
    return nullptr;
  }
  return std::make_unique<IosFileHandle>(path, handle);
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
    out_info->write_timestamp  = convertUnixtimeToWinFiletime(st.st_mtime);
    return true;
  }
  return false;
}

std::vector<FileInfo> ListFiles(const std::filesystem::path& path) {
  std::vector<FileInfo> result;
  DIR* dir = opendir(path.c_str());
  if (!dir) return result;

  while (auto ent = readdir(dir)) {
    if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;

    FileInfo info;
    info.name = ent->d_name;
    struct stat st{};
    stat((path / info.name).c_str(), &st);
    info.create_timestamp = convertUnixtimeToWinFiletime(st.st_ctime);
    info.access_timestamp = convertUnixtimeToWinFiletime(st.st_atime);
    info.write_timestamp  = convertUnixtimeToWinFiletime(st.st_mtime);
    info.path = path;
    if (ent->d_type == DT_DIR) {
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

#endif  // REX_PLATFORM_IOS
