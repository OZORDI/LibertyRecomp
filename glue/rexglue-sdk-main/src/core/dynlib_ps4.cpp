#include <rex/platform.h>

#if REX_PLATFORM_PS4

#include <rex/logging/macros.h>
#include <rex/platform/dynlib.h>

#include <cstdint>

// OpenOrbis libkernel module + dlsym APIs. Declared here to avoid forcing the
// whole SDK's <orbis/libkernel.h> on every consumer of dynlib.h.
extern "C" {
std::uint32_t sceKernelLoadStartModule(const char*, std::size_t, const void*,
                                       std::uint32_t, void*, void*);
std::int32_t  sceKernelStopUnloadModule(std::int32_t, std::size_t, const void*,
                                        std::uint32_t, void*, std::int32_t*);
std::int32_t  sceKernelDlsym(std::int32_t handle, const char* symbol, void** address);
}

namespace rex::platform {

namespace {
// Encoding: handle_ == 0 means "not loaded". When loaded we store
// (static_cast<uint32_t>(module_id)) | (1ULL << 32) so a module id of 0 is
// still distinguishable from "not loaded". Module ids from Orbis are small
// positive ints in practice, but be paranoid.
constexpr std::uintptr_t kLoadedFlag = static_cast<std::uintptr_t>(1) << 32;

inline std::uintptr_t Encode(std::int32_t module_id) {
  return kLoadedFlag | static_cast<std::uint32_t>(module_id);
}
inline std::int32_t Decode(std::uintptr_t h) {
  return static_cast<std::int32_t>(static_cast<std::uint32_t>(h & 0xFFFFFFFFu));
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
    return false;
  }
  const std::string p = path.string();
  std::int32_t result = 0;
  const std::uint32_t module_id =
      sceKernelLoadStartModule(p.c_str(), 0, nullptr, 0, nullptr, &result);
  // sceKernelLoadStartModule returns a negative SCE error on failure; the
  // high bit (0x80000000) is set on error codes.
  if (static_cast<std::int32_t>(module_id) < 0) {
    REXLOG_WARN("sceKernelLoadStartModule('{}') failed: 0x{:08x}", p,
                module_id);
    return false;
  }
  handle_ = Encode(static_cast<std::int32_t>(module_id));
  return true;
}

void DynamicLibrary::Close() {
  if (handle_ == kInvalidDynamicLibraryHandle) {
    return;
  }
  const std::int32_t module_id = Decode(handle_);
  std::int32_t stop_result = 0;
  const std::int32_t rc = sceKernelStopUnloadModule(
      module_id, 0, nullptr, 0, nullptr, &stop_result);
  if (rc < 0) {
    REXLOG_WARN("sceKernelStopUnloadModule({}) failed: 0x{:08x}", module_id,
                static_cast<std::uint32_t>(rc));
  }
  handle_ = kInvalidDynamicLibraryHandle;
}

void* DynamicLibrary::GetRawSymbol(const char* name) const {
  if (handle_ == kInvalidDynamicLibraryHandle || !name) {
    return nullptr;
  }
  void* addr = nullptr;
  const std::int32_t rc = sceKernelDlsym(Decode(handle_), name, &addr);
  if (rc < 0) {
    return nullptr;
  }
  return addr;
}

}  // namespace rex::platform

#endif  // REX_PLATFORM_PS4
