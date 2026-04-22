#include <rex/platform.h>

#if REX_PLATFORM_IOS

// iOS: dlopen() exists but App Store policy forbids loading third-party
// dylibs. Only system dylibs under /usr/lib/system/ and
// /System/Library/Frameworks/ may be opened, and even those are only
// accepted by App Review for narrow, well-justified use cases. All the
// "well-known libraries" (MoltenVK, RenderDoc, SPIRV-Tools) are either
// statically linked into the app or simply unavailable on iOS, so
// lib_names::* are nullptr and Load() refuses them up front.
//
// We expose Load()/Close()/GetRawSymbol() as thin dlopen() wrappers
// gated by a strict path whitelist. Anything outside the whitelist is
// rejected without even attempting dlopen(), so review-time static
// analysis won't flag a dynamic lookup against third-party content.
#include <dlfcn.h>

#include <array>
#include <string_view>

#include <rex/logging/macros.h>
#include <rex/platform/dynlib.h>

namespace rex::platform {

namespace {

// App Store-accepted prefixes. dlopen() against anything else is refused
// up front — no dlopen() attempt is made, so static review tooling won't
// even see a call against a third-party path.
//
//   /usr/lib/system/   — low-level system dylibs (libsystem_*.dylib etc.)
//   /usr/lib/          — public system dylibs (libSystem.B.dylib, libc++.1.dylib)
//   /System/Library/Frameworks/
//                      — framework bundles (Metal.framework/Metal,
//                        MoltenVK.framework/MoltenVK if bundled by Apple)
//   /System/Library/PrivateFrameworks/
//                      — private frameworks; lookups here will likely fail
//                        App Review, but are on-device-valid system paths.
constexpr std::array<std::string_view, 4> kAllowedPathPrefixes = {
    std::string_view{"/usr/lib/system/"},
    std::string_view{"/usr/lib/"},
    std::string_view{"/System/Library/Frameworks/"},
    std::string_view{"/System/Library/PrivateFrameworks/"},
};

bool IsSystemDylibPath(std::string_view path) {
  for (const std::string_view prefix : kAllowedPathPrefixes) {
    if (path.size() >= prefix.size() &&
        path.compare(0, prefix.size(), prefix) == 0) {
      return true;
    }
  }
  return false;
}

}  // namespace

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
  const std::string path_str = path.string();
  if (!IsSystemDylibPath(path_str)) {
    REXLOG_WARN(
        "DynamicLibrary::Load('{}') refused on iOS: only system dylibs "
        "under /usr/lib/, /usr/lib/system/, or /System/Library/(Private)"
        "Frameworks/ may be loaded (App Store policy).",
        path_str);
    return false;
  }
  // Clear any stale dlerror state before our dlopen() so the message we
  // log on failure is the one for this call.
  (void)dlerror();
  handle_ = dlopen(path_str.c_str(), RTLD_LAZY | RTLD_LOCAL);
  if (!handle_) {
    const char* err = dlerror();
    REXLOG_WARN("dlopen('{}') failed on iOS: {}", path_str,
                err ? err : "(null)");
    return false;
  }
  REXLOG_WARN(
      "DynamicLibrary::Load('{}') succeeded on iOS — verify with App "
      "Review before shipping; dynamic lookups against system paths are "
      "scrutinized.",
      path_str);
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
