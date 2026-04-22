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

// Provided by the host Android app (LibertyRecomp jni_glue.cpp). Returns
// the JavaVM captured in JNI_OnLoad, or null in non-JVM builds.
extern "C" JavaVM* rex_android_get_jvm();

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

// Global ref to the hosting Activity / application Context. Owned by
// SetAndroidJavaContext; cleared by passing a null context. Used by the
// content-URI resolver which must call Context.getContentResolver() off
// an arbitrary thread (so the local ref from nativeSetActivity cannot be
// reused).
jobject g_android_context = nullptr;

// Scoped thread attach helper. Uses GetEnv first so already-attached
// threads (the JNI caller itself) don't detach on destruction.
struct JniAttach {
  JavaVM* vm = nullptr;
  JNIEnv* env = nullptr;
  bool detach = false;
  explicit JniAttach(JavaVM* v) : vm(v) {
    if (!vm) return;
    int status = vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    if (status == JNI_EDETACHED) {
      if (vm->AttachCurrentThread(&env, nullptr) == JNI_OK) {
        detach = true;
      } else {
        env = nullptr;
      }
    } else if (status != JNI_OK) {
      env = nullptr;
    }
  }
  ~JniAttach() {
    if (detach && vm) vm->DetachCurrentThread();
  }
};

// Cached JNI identifiers for content-URI resolution. Guarded by
// g_android_content_mutex. Populated by AndroidInitialize(), torn down by
// AndroidShutdown(). All jclass refs are global; method IDs remain valid
// for the lifetime of the referenced class.
std::mutex g_android_content_mutex;
jobject g_content_resolver = nullptr;          // ContentResolver (global ref)
jclass g_content_resolver_class = nullptr;     // global ref
jmethodID g_open_file_descriptor_mid = nullptr;
jclass g_parcel_file_descriptor_class = nullptr;
jmethodID g_detach_fd_mid = nullptr;
jclass g_uri_class = nullptr;
jmethodID g_uri_parse_mid = nullptr;
bool g_content_resolver_initialized = false;

// Helper: UTF-8 -> Java String containing UTF-16 code units, matching
// Android's internal representation. Xenia uses the same pattern; avoids
// NewStringUTF's modified-UTF8 quirks (which mangle 4-byte sequences).
jstring NewJavaStringFromUtf8(JNIEnv* env, const std::string_view s) {
  std::u16string u16 = rex::string::to_utf16(s);
  return env->NewString(reinterpret_cast<const jchar*>(u16.data()),
                        static_cast<jsize>(u16.size()));
}

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

  // Drop any previous global ref before replacing. Safe even with a null
  // env because DeleteGlobalRef doesn't strictly require the same env
  // that created the ref, but we need *some* env to free it — on null
  // env we just leak the previous ref rather than crash. This branch is
  // only expected at shutdown.
  if (g_android_context && env) {
    env->DeleteGlobalRef(g_android_context);
  }
  g_android_context = nullptr;

  if (!env || !context) {
    g_filesDir.clear();
    g_cacheDir.clear();
    g_externalFilesDir.clear();
    return;
  }

  g_android_context = env->NewGlobalRef(context);

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

// -----------------------------------------------------------------------
// Android content-URI support
//
// Ported from Xenia (tools/xenia-master-1/src/xenia/base/filesystem_android.cc):
//   - AndroidInitializeContentResolver / AndroidShutdownContentResolver
//   - IsAndroidContentUri
//   - OpenAndroidContentFileDescriptor
// Adapted to:
//   - use rex_android_get_jvm() for off-thread JNIEnv acquisition (Xenia
//     has GetAndroidThreadJniEnv() built into main_android.h)
//   - read the Activity/Context global ref stashed by SetAndroidJavaContext
// Lifecycle: AndroidInitialize() caches JNI globals; AndroidShutdown()
// releases them. Re-entrancy: last call wins; safe to re-init after
// shutdown.

namespace {

void ShutdownContentResolverLocked(JNIEnv* env) {
  g_content_resolver_initialized = false;
  g_uri_parse_mid = nullptr;
  g_detach_fd_mid = nullptr;
  g_open_file_descriptor_mid = nullptr;
  if (env) {
    if (g_uri_class) env->DeleteGlobalRef(g_uri_class);
    if (g_parcel_file_descriptor_class)
      env->DeleteGlobalRef(g_parcel_file_descriptor_class);
    if (g_content_resolver_class)
      env->DeleteGlobalRef(g_content_resolver_class);
    if (g_content_resolver) env->DeleteGlobalRef(g_content_resolver);
  }
  g_uri_class = nullptr;
  g_parcel_file_descriptor_class = nullptr;
  g_content_resolver_class = nullptr;
  g_content_resolver = nullptr;
}

}  // namespace

void AndroidInitialize() {
  std::lock_guard<std::mutex> lock(g_android_content_mutex);
  if (g_content_resolver_initialized) return;

  JavaVM* vm = rex_android_get_jvm();
  JniAttach att(vm);
  JNIEnv* env = att.env;
  if (!env) {
    REX_ANDROID_LOGE("AndroidInitialize: no JNIEnv");
    return;
  }

  jobject context = nullptr;
  {
    std::lock_guard<std::mutex> paths_lock(g_android_paths_mutex);
    context = g_android_context;
  }
  if (!context) {
    REX_ANDROID_LOGE("AndroidInitialize: no Java Context cached");
    return;
  }

  // Context.getContentResolver()
  jclass context_class = env->GetObjectClass(context);
  if (!context_class) {
    REX_ANDROID_LOGE("AndroidInitialize: no Context class");
    ShutdownContentResolverLocked(env);
    return;
  }
  jmethodID get_cr_mid = env->GetMethodID(
      context_class, "getContentResolver", "()Landroid/content/ContentResolver;");
  if (!get_cr_mid) {
    if (env->ExceptionCheck()) env->ExceptionClear();
    REX_ANDROID_LOGE("AndroidInitialize: no getContentResolver method");
    env->DeleteLocalRef(context_class);
    ShutdownContentResolverLocked(env);
    return;
  }
  jobject cr_local = env->CallObjectMethod(context, get_cr_mid);
  env->DeleteLocalRef(context_class);
  if (env->ExceptionCheck()) {
    env->ExceptionClear();
    ShutdownContentResolverLocked(env);
    return;
  }
  if (!cr_local) {
    REX_ANDROID_LOGE("AndroidInitialize: ContentResolver null");
    ShutdownContentResolverLocked(env);
    return;
  }
  g_content_resolver = env->NewGlobalRef(cr_local);
  env->DeleteLocalRef(cr_local);
  if (!g_content_resolver) {
    ShutdownContentResolverLocked(env);
    return;
  }

  // ContentResolver.openFileDescriptor(Uri, String) : ParcelFileDescriptor
  jclass cr_class_local = env->GetObjectClass(g_content_resolver);
  if (!cr_class_local) {
    ShutdownContentResolverLocked(env);
    return;
  }
  g_content_resolver_class = reinterpret_cast<jclass>(
      env->NewGlobalRef(reinterpret_cast<jobject>(cr_class_local)));
  env->DeleteLocalRef(cr_class_local);
  if (!g_content_resolver_class) {
    ShutdownContentResolverLocked(env);
    return;
  }
  g_open_file_descriptor_mid = env->GetMethodID(
      g_content_resolver_class, "openFileDescriptor",
      "(Landroid/net/Uri;Ljava/lang/String;)Landroid/os/ParcelFileDescriptor;");
  if (!g_open_file_descriptor_mid) {
    if (env->ExceptionCheck()) env->ExceptionClear();
    REX_ANDROID_LOGE("AndroidInitialize: no openFileDescriptor method");
    ShutdownContentResolverLocked(env);
    return;
  }

  // ParcelFileDescriptor.detachFd() : int
  jclass pfd_local = env->FindClass("android/os/ParcelFileDescriptor");
  if (!pfd_local) {
    if (env->ExceptionCheck()) env->ExceptionClear();
    ShutdownContentResolverLocked(env);
    return;
  }
  g_parcel_file_descriptor_class = reinterpret_cast<jclass>(
      env->NewGlobalRef(reinterpret_cast<jobject>(pfd_local)));
  env->DeleteLocalRef(pfd_local);
  if (!g_parcel_file_descriptor_class) {
    ShutdownContentResolverLocked(env);
    return;
  }
  g_detach_fd_mid =
      env->GetMethodID(g_parcel_file_descriptor_class, "detachFd", "()I");
  if (!g_detach_fd_mid) {
    if (env->ExceptionCheck()) env->ExceptionClear();
    ShutdownContentResolverLocked(env);
    return;
  }

  // android.net.Uri.parse(String) : Uri
  jclass uri_local = env->FindClass("android/net/Uri");
  if (!uri_local) {
    if (env->ExceptionCheck()) env->ExceptionClear();
    ShutdownContentResolverLocked(env);
    return;
  }
  g_uri_class = reinterpret_cast<jclass>(
      env->NewGlobalRef(reinterpret_cast<jobject>(uri_local)));
  env->DeleteLocalRef(uri_local);
  if (!g_uri_class) {
    ShutdownContentResolverLocked(env);
    return;
  }
  g_uri_parse_mid = env->GetStaticMethodID(
      g_uri_class, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
  if (!g_uri_parse_mid) {
    if (env->ExceptionCheck()) env->ExceptionClear();
    ShutdownContentResolverLocked(env);
    return;
  }

  g_content_resolver_initialized = true;
  REX_ANDROID_LOGI("AndroidInitialize: content resolver ready");
}

void AndroidShutdown() {
  std::lock_guard<std::mutex> lock(g_android_content_mutex);
  JavaVM* vm = rex_android_get_jvm();
  JniAttach att(vm);
  ShutdownContentResolverLocked(att.env);
}

bool IsAndroidContentUri(const std::string_view source) {
  // URI schemes are case-insensitive. Matching "content://" (not just
  // "content:") avoids misclassifying a path whose filename begins with
  // "content:".
  static constexpr char kContentSchema[] = "content://";
  constexpr size_t kContentSchemaLength = sizeof(kContentSchema) - 1;
  if (source.size() < kContentSchemaLength) return false;
  return strncasecmp(source.data(), kContentSchema, kContentSchemaLength) == 0;
}

int OpenAndroidContentFileDescriptor(const std::string_view uri,
                                     const char* mode) {
  std::lock_guard<std::mutex> lock(g_android_content_mutex);
  if (!g_content_resolver_initialized) {
    REX_ANDROID_LOGE("OpenAndroidContentFileDescriptor: not initialized");
    return -1;
  }

  JavaVM* vm = rex_android_get_jvm();
  JniAttach att(vm);
  JNIEnv* env = att.env;
  if (!env) return -1;

  // Uri.parse(uri_string)
  jstring uri_string = NewJavaStringFromUtf8(env, uri);
  if (!uri_string) return -1;
  jobject uri_object =
      env->CallStaticObjectMethod(g_uri_class, g_uri_parse_mid, uri_string);
  env->DeleteLocalRef(uri_string);
  if (env->ExceptionCheck()) {
    env->ExceptionClear();
    return -1;
  }
  if (!uri_object) return -1;

  jstring mode_string = env->NewStringUTF(mode ? mode : "r");
  if (!mode_string) {
    env->DeleteLocalRef(uri_object);
    return -1;
  }

  jobject pfd = env->CallObjectMethod(g_content_resolver,
                                      g_open_file_descriptor_mid, uri_object,
                                      mode_string);
  env->DeleteLocalRef(mode_string);
  env->DeleteLocalRef(uri_object);
  if (env->ExceptionCheck()) {
    env->ExceptionClear();
    return -1;
  }
  if (!pfd) return -1;

  // detachFd() transfers ownership to the caller — the ParcelFileDescriptor
  // will NOT close the fd when GC'd.
  int fd = env->CallIntMethod(pfd, g_detach_fd_mid);
  env->DeleteLocalRef(pfd);
  if (env->ExceptionCheck()) {
    env->ExceptionClear();
    return -1;
  }
  return fd;
}

}  // namespace filesystem
}  // namespace rex

#endif  // REX_PLATFORM_ANDROID
