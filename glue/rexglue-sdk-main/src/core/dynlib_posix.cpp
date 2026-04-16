#include <rex/platform.h>
#include <rex/platform/dynlib.h>

// POSIX dlopen-based implementation. Used on Linux, macOS, and Android.
// iOS uses dynlib_ios.cpp (dlopen wrappers with App Store warnings).
// PS4 uses dynlib_ps4.cpp (sceKernelLoadStartModule / sceKernelDlsym).
// Switch uses dynlib_switch.cpp (stubbed — no runtime loading on libnx).
#if REX_PLATFORM_POSIX && !REX_PLATFORM_IOS && !REX_PLATFORM_PS4 && !REX_PLATFORM_NX

#include <dlfcn.h>

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
  handle_ = dlopen(path.c_str(), RTLD_LAZY);
  return handle_ != kInvalidDynamicLibraryHandle;
}

void DynamicLibrary::Close() {
  if (handle_) {
    dlclose(handle_);
    handle_ = kInvalidDynamicLibraryHandle;
  }
}

void* DynamicLibrary::GetRawSymbol(const char* name) const {
  if (!handle_)
    return nullptr;
  return dlsym(handle_, name);
}

}  // namespace rex::platform

#endif  // POSIX desktop
