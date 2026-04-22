#include <rex/platform.h>
#include <rex/platform/dynlib.h>

// POSIX dlopen-based implementation. Used on Linux, macOS, and Android.
// iOS uses dynlib_ios.cpp (dlopen wrappers with App Store warnings).
// PS4 uses dynlib_ps4.cpp (sceKernelLoadStartModule / sceKernelDlsym).
// Switch uses dynlib_switch.cpp (stubbed — no runtime loading on libnx).
#if REX_PLATFORM_POSIX && !REX_PLATFORM_IOS && !REX_PLATFORM_PS4 && !REX_PLATFORM_NX

#include <dlfcn.h>

#include <rex/logging/macros.h>

namespace rex::platform {

DynamicLibrary::~DynamicLibrary() {
  Close();
}

DynamicLibrary::DynamicLibrary(DynamicLibrary&& other) noexcept : handle_(other.handle_) {
  other.handle_ = kInvalidDynamicLibraryHandle;
}

DynamicLibrary& DynamicLibrary::operator=(DynamicLibrary&& other) noexcept {
  if (this != &other) {
    Close();
    handle_ = other.handle_;
    other.handle_ = kInvalidDynamicLibraryHandle;
  }
  return *this;
}

bool DynamicLibrary::Load(const std::filesystem::path& path) {
  Close();
  // RTLD_LOCAL keeps the opened library's symbols out of the global
  // namespace. Without it, Vulkan loaders and similar re-exporting
  // libraries can collide (e.g. a later dlopen() resolving a symbol
  // back into the first loaded library instead of its own copy).
  // Clear any prior dlerror state so the message we read below
  // belongs to this call.
  (void)dlerror();
  handle_ = dlopen(path.c_str(), RTLD_LAZY | RTLD_LOCAL);
  if (handle_ == kInvalidDynamicLibraryHandle) {
    const char* err = dlerror();
    REXLOG_WARN("dlopen('{}') failed: {}", path.string(),
                err ? err : "(null)");
    return false;
  }
  return true;
}

void DynamicLibrary::Close() {
  if (handle_) {
    dlclose(handle_);
    handle_ = kInvalidDynamicLibraryHandle;
  }
}

void* DynamicLibrary::GetRawSymbol(const char* name) const {
  if (handle_ == kInvalidDynamicLibraryHandle || !name)
    return nullptr;
  return dlsym(handle_, name);
}

}  // namespace rex::platform

#endif  // POSIX desktop
