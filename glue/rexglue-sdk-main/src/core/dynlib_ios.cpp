#include <rex/platform.h>

#if REX_PLATFORM_IOS

// iOS: dlopen() exists but App Store policy forbids loading third-party
// dylibs — only system dylibs under /usr/lib/system/ may be opened, and
// even that is discouraged by App Review. All the "well-known libraries"
// (MoltenVK, RenderDoc, SPIRV-Tools) are either statically linked into the
// app or simply unavailable on iOS, so lib_names::* are nullptr and
// Load() will refuse them up front.
//
// We still implement Load/Close/GetRawSymbol as thin dlopen() wrappers so
// that callers who really do want to open a shipped system dylib (e.g.
// libSystem.B.dylib) can do so. A warning is logged on every successful
// load to flag App Store review risk.
#include <dlfcn.h>

#include <rex/logging/macros.h>
#include <rex/platform/dynlib.h>

namespace rex::platform {

DynamicLibrary::~DynamicLibrary() { Close(); }

DynamicLibrary::DynamicLibrary(DynamicLibrary&& other) noexcept
    : handle_(other.handle_) {
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
  if (path.empty()) {
    REXLOG_WARN(
        "DynamicLibrary::Load() called on iOS with an empty path — the "
        "requested library is not available on this platform (statically "
        "linked or disallowed).");
    return false;
  }
  handle_ = dlopen(path.c_str(), RTLD_LAZY | RTLD_LOCAL);
  if (!handle_) {
    const char* err = dlerror();
    REXLOG_WARN("dlopen('{}') failed on iOS: {}", path.string(),
                err ? err : "(null)");
    return false;
  }
  REXLOG_WARN(
      "DynamicLibrary::Load('{}') succeeded on iOS — only system dylibs "
      "under /usr/lib/system/ are permitted by App Store policy; verify "
      "before shipping.",
      path.string());
  return true;
}

void DynamicLibrary::Close() {
  if (handle_) {
    dlclose(handle_);
    handle_ = kInvalidDynamicLibraryHandle;
  }
}

void* DynamicLibrary::GetRawSymbol(const char* name) const {
  if (!handle_ || !name) {
    return nullptr;
  }
  return dlsym(handle_, name);
}

}  // namespace rex::platform

#endif  // REX_PLATFORM_IOS
