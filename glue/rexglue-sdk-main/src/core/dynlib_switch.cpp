#include <rex/platform.h>

#if REX_PLATFORM_NX

// Nintendo Switch (libnx/Horizon OS): homebrew libnx does not expose the
// nn::ro dynamic loader used by retail titles. ldrRoLoadNro exists but
// requires the ldr:ro service, which is privileged and not generally
// available to application processes. For the MVP we stub the entire API
// out — renderer code must not rely on runtime shared library loading on
// this platform.
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
  REXLOG_WARN(
      "DynamicLibrary::Load('{}') called on Switch — dynamic loading is not "
      "supported; link this code statically.",
      path.string());
  handle_ = kInvalidDynamicLibraryHandle;
  return false;
}

void DynamicLibrary::Close() { handle_ = kInvalidDynamicLibraryHandle; }

void* DynamicLibrary::GetRawSymbol(const char* name) const {
  REXLOG_WARN(
      "DynamicLibrary::GetRawSymbol('{}') called on Switch — dynamic loading "
      "is not supported.",
      name ? name : "(null)");
  return nullptr;
}

}  // namespace rex::platform

#endif  // REX_PLATFORM_NX
