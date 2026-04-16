/**
 * Android filesystem backend for ReXGlue.
 *
 * Mirrors filesystem_posix.cpp for the common POSIX I/O surface (OpenFile,
 * Seek, Tell, PosixFileHandle, GetInfo, ListFiles, etc.) but overrides the
 * user/caches folder queries to go through the Java Context, since Android
 * apps do not have access to $HOME / $XDG_DATA_HOME.
 */

#if defined(REX_PLATFORM_ANDROID) || defined(__ANDROID__)

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <dirent.h>
#include <libgen.h>
#include <mutex>

#include <android/log.h>
#include <jni.h>

#include <rex/assert.h>
#include <rex/filesystem.h>
#include <rex/logging.h>
#include <rex/platform/android.h>
#include <rex/string.h>

#define REX_ANDROID_LOG_TAG "rex.fs"
#define REX_ANDROID_LOGE(...) \
  __android_log_print(ANDROID_LOG_ERROR, REX_ANDROID_LOG_TAG, __VA_ARGS__)
#define REX_ANDROID_LOGI(...) \
  __android_log_print(ANDROID_LOG_INFO, REX_ANDROID_LOG_TAG, __VA_ARGS__)

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

namespace {

std::mutex g_android_paths_mutex;
std::filesystem::path g_filesDir;
std::filesystem::path g_cacheDir;
std::filesystem::path g_externalFilesDir;

// Calls Context.<methodName>().getAbsolutePath() and writes to out.
// Returns true on success.
bool QueryContextDirMethod(JNIEnv* env, jobject context, const char* methodName,
                           std::filesystem::path& out) {
  if (!env || !context || !methodName) return false;

  jclass contextCls = env->GetObjectClass(context);
  if (!contextCls) return false;

  jmethodID mid = env->GetMethodID(contextCls, methodName, "()Ljava/io/File;");
  if (!mid) {
    env->ExceptionClear();
    env->DeleteLocalRef(contextCls);
    return false;
  }

  jobject fileObj = env->CallObjectMethod(context, mid);
  if (env->ExceptionCheck()) {
    env->ExceptionDescribe();
    env->ExceptionClear();
    env->DeleteLocalRef(contextCls);
    return false;
  }
  if (!fileObj) {
    env->DeleteLocalRef(contextCls);
    return false;
  }

  jclass fileCls = env->GetObjectClass(fileObj);
  jmethodID getAbs =
      env->GetMethodID(fileCls, "getAbsolutePath", "()Ljava/lang/String;");
  jstring jpath =
      static_cast<jstring>(env->CallObjectMethod(fileObj, getAbs));
  if (env->ExceptionCheck()) {
    env->ExceptionDescribe();
    env->ExceptionClear();
    env->DeleteLocalRef(fileCls);
    env->DeleteLocalRef(fileObj);
    env->DeleteLocalRef(contextCls);
    return false;
  }

  const char* cpath = env->GetStringUTFChars(jpath, nullptr);
  if (cpath) {
    out = std::filesystem::path(cpath);
    env->ReleaseStringUTFChars(jpath, cpath);
  }

  env->DeleteLocalRef(jpath);
  env->DeleteLocalRef(fileCls);
  env->DeleteLocalRef(fileObj);
  env->DeleteLocalRef(contextCls);
  return !out.empty();
}

// Same but for getExternalFilesDir(String type) which takes a nullable arg.
bool QueryExternalFilesDir(JNIEnv* env, jobject context,
                           std::filesystem::path& out) {
  if (!env || !context) return false;

  jclass contextCls = env->GetObjectClass(context);
  jmethodID mid = env->GetMethodID(contextCls, "getExternalFilesDir",
                                   "(Ljava/lang/String;)Ljava/io/File;");
  if (!mid) {
    env->ExceptionClear();
    env->DeleteLocalRef(contextCls);
    return false;
  }

  jobject fileObj = env->CallObjectMethod(context, mid, nullptr);
  if (env->ExceptionCheck()) {
    env->ExceptionClear();
    env->DeleteLocalRef(contextCls);
    return false;
  }
  if (!fileObj) {
    env->DeleteLocalRef(contextCls);
    return false;
  }

  jclass fileCls = env->GetObjectClass(fileObj);
  jmethodID getAbs =
      env->GetMethodID(fileCls, "getAbsolutePath", "()Ljava/lang/String;");
  jstring jpath =
      static_cast<jstring>(env->CallObjectMethod(fileObj, getAbs));
  if (env->ExceptionCheck()) {
    env->ExceptionClear();
    env->DeleteLocalRef(fileCls);
    env->DeleteLocalRef(fileObj);
    env->DeleteLocalRef(contextCls);
    return false;
  }

  const char* cpath = env->GetStringUTFChars(jpath, nullptr);
  if (cpath) {
    out = std::filesystem::path(cpath);
    env->ReleaseStringUTFChars(jpath, cpath);
  }
  env->DeleteLocalRef(jpath);
  env->DeleteLocalRef(fileCls);
  env->DeleteLocalRef(fileObj);
  env->DeleteLocalRef(contextCls);
  return !out.empty();
}

// Last-resort guess at /data/data/<pkg>/files using getprogname() as package
// hint. Only used when the Java Context was never provided.
std::filesystem::path FallbackFilesDir() {
  const char* pkg = nullptr;
#if defined(__BIONIC__) || defined(__ANDROID__)
  pkg = getprogname();
#endif
  if (!pkg || !*pkg) {
    pkg = std::getenv("ANDROID_PACKAGE_NAME");
  }
  if (!pkg || !*pkg) {
    pkg = "com.libertyrecomp.app";
  }
  return std::filesystem::path("/data/data") / pkg / "files";
}

std::filesystem::path FallbackCacheDir() {
  return FallbackFilesDir().parent_path() / "cache";
}

}  // namespace

namespace platform {
namespace android {

void SetAndroidJavaContext(JNIEnv* env, jobject context) {
  std::lock_guard<std::mutex> lock(g_android_paths_mutex);

  if (!env || !context) {
    g_filesDir.clear();
    g_cacheDir.clear();
    g_externalFilesDir.clear();
    return;
  }

  std::filesystem::path filesDir, cacheDir, extDir;
  if (QueryContextDirMethod(env, context, "getFilesDir", filesDir)) {
    g_filesDir = std::move(filesDir);
    REX_ANDROID_LOGI("filesDir=%s", g_filesDir.c_str());
  } else {
    REX_ANDROID_LOGE("getFilesDir() failed");
  }
  if (QueryContextDirMethod(env, context, "getCacheDir", cacheDir)) {
    g_cacheDir = std::move(cacheDir);
    REX_ANDROID_LOGI("cacheDir=%s", g_cacheDir.c_str());
  } else {
    REX_ANDROID_LOGE("getCacheDir() failed");
  }
  if (QueryExternalFilesDir(env, context, extDir)) {
    g_externalFilesDir = std::move(extDir);
    REX_ANDROID_LOGI("externalFilesDir=%s", g_externalFilesDir.c_str());
  }
}

}  // namespace android
}  // namespace platform

namespace filesystem {

std::filesystem::path GetExecutablePath() {
  char buff[FILENAME_MAX] = "";
  ssize_t n = readlink("/proc/self/exe", buff, FILENAME_MAX - 1);
  if (n > 0) buff[n] = '\0';
  return std::string(buff);
}

std::filesystem::path GetExecutableFolder() {
  return GetExecutablePath().parent_path();
}

std::filesystem::path GetUserFolder() {
  std::lock_guard<std::mutex> lock(g_android_paths_mutex);
  if (!g_filesDir.empty()) return g_filesDir;
  return FallbackFilesDir();
}

std::filesystem::path GetCachesFolder() {
  std::lock_guard<std::mutex> lock(g_android_paths_mutex);
  if (!g_cacheDir.empty()) return g_cacheDir;
  return FallbackCacheDir();
}

std::filesystem::path GetExternalFilesFolder() {
  std::lock_guard<std::mutex> lock(g_android_paths_mutex);
  if (!g_externalFilesDir.empty()) return g_externalFilesDir;
  return {};
}

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
  return (static_cast<uint64_t>(unixtime) * 10000000ULL) +
         116444736000000000ULL;
}

bool CreateEmptyFile(const std::filesystem::path& path) {
  int file = creat(path.c_str(), 0664);
  if (file >= 0) {
    close(file);
    return true;
  }
  return false;
}

class AndroidFileHandle : public FileHandle {
 public:
  AndroidFileHandle(std::filesystem::path path, int handle)
      : FileHandle(std::move(path)), handle_(handle) {}
  ~AndroidFileHandle() override {
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
    return ftruncate(handle_, length) >= 0;
  }
  void Flush() override { fsync(handle_); }

 private:
  int handle_ = -1;
};

std::unique_ptr<FileHandle> FileHandle::OpenExisting(
    const std::filesystem::path& path, uint32_t desired_access) {
  int open_access = 0;
  if (desired_access & FileAccess::kGenericRead) open_access |= O_RDONLY;
  if (desired_access & FileAccess::kGenericWrite) open_access |= O_WRONLY;
  if (desired_access & FileAccess::kGenericExecute) open_access |= O_RDONLY;
  if (desired_access & FileAccess::kGenericAll) open_access |= O_RDWR;
  if (desired_access & FileAccess::kFileReadData) open_access |= O_RDONLY;
  if (desired_access & FileAccess::kFileWriteData) open_access |= O_WRONLY;
  if (desired_access & FileAccess::kFileAppendData) open_access |= O_APPEND;

  int handle = open(path.c_str(), open_access);
  if (handle == -1) return nullptr;
  return std::make_unique<AndroidFileHandle>(path, handle);
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
  if (!dir) return result;

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

#endif  // REX_PLATFORM_ANDROID
