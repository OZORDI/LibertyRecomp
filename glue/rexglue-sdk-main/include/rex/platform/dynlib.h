/**
 * @file        platform/dynlib.h
 * @brief       Platform-agnostic dynamic library loading
 */
#pragma once

#include <cstdint>
#include <filesystem>

#include <rex/platform.h>

namespace rex::platform {

// Handle type varies per platform:
//   * POSIX (Linux/macOS/Android/iOS) + Win32: opaque void* from dlopen/LoadLibrary.
//   * PS4: SceKernelModule (int32_t) — zero-extended into handle_.
//   * Switch: stub (no dynamic loading) — handle_ unused, always null.
#if REX_PLATFORM_PS4 || REX_PLATFORM_NX
using DynamicLibraryHandle = std::uintptr_t;
inline constexpr DynamicLibraryHandle kInvalidDynamicLibraryHandle = 0;
#else
using DynamicLibraryHandle = void*;
inline constexpr DynamicLibraryHandle kInvalidDynamicLibraryHandle = nullptr;
#endif

class DynamicLibrary {
 public:
  DynamicLibrary() = default;
  ~DynamicLibrary();

  DynamicLibrary(const DynamicLibrary&) = delete;
  DynamicLibrary& operator=(const DynamicLibrary&) = delete;
  DynamicLibrary(DynamicLibrary&& other) noexcept;
  DynamicLibrary& operator=(DynamicLibrary&& other) noexcept;

  bool Load(const std::filesystem::path& path);
  void Close();
  explicit operator bool() const { return handle_ != kInvalidDynamicLibraryHandle; }

  void* GetRawSymbol(const char* name) const;

  template <typename T>
  T GetSymbol(const char* name) const {
    return reinterpret_cast<T>(GetRawSymbol(name));
  }

 private:
  DynamicLibraryHandle handle_ = kInvalidDynamicLibraryHandle;
};

namespace lib_names {

#if REX_PLATFORM_WIN32

inline constexpr const char* kVulkanLoader = "vulkan-1.dll";
inline constexpr const char* kRenderDoc = "renderdoc.dll";
inline constexpr const char* kSpirvToolsSdkPath = "Bin/SPIRV-Tools-shared.dll";

#elif REX_PLATFORM_ANDROID

inline constexpr const char* kVulkanLoader = "libvulkan.so";
inline constexpr const char* kRenderDoc = "libVkLayer_GLES_RenderDoc.so";
inline constexpr const char* kSpirvToolsSdkPath = "bin/libSPIRV-Tools-shared.so";

#elif REX_PLATFORM_IOS

// iOS: MoltenVK is statically linked into the app; no external Vulkan loader.
// RenderDoc is unavailable on iOS. dlopen is restricted to system dylibs, so
// these remain nullptr — DynamicLibrary::Load() will return false for them.
inline constexpr const char* kVulkanLoader = nullptr;
inline constexpr const char* kRenderDoc = nullptr;
inline constexpr const char* kSpirvToolsSdkPath = nullptr;

#elif REX_PLATFORM_MAC

inline constexpr const char* kVulkanLoader = "libMoltenVK.dylib";
inline constexpr const char* kRenderDoc = "librenderdoc.dylib";
inline constexpr const char* kSpirvToolsSdkPath = "bin/libSPIRV-Tools-shared.dylib";

#elif REX_PLATFORM_LINUX

inline constexpr const char* kVulkanLoader = "libvulkan.so.1";
inline constexpr const char* kRenderDoc = "librenderdoc.so";
inline constexpr const char* kSpirvToolsSdkPath = "bin/libSPIRV-Tools-shared.so";

#elif REX_PLATFORM_PS4

// PS4 ships no Vulkan loader and no RenderDoc. The renderer should select the
// GNM/AGC backend instead; these nullptr values cause Load() to fail cleanly.
inline constexpr const char* kVulkanLoader = nullptr;
inline constexpr const char* kRenderDoc = nullptr;
inline constexpr const char* kSpirvToolsSdkPath = nullptr;

#elif REX_PLATFORM_NX

// Switch: vendor GPU driver is statically linked; no Vulkan loader dylib
// exists and dynamic loading is generally unavailable via libnx.
inline constexpr const char* kVulkanLoader = nullptr;
inline constexpr const char* kRenderDoc = nullptr;
inline constexpr const char* kSpirvToolsSdkPath = nullptr;

#else
#error No library names provided for the target platform.
#endif

}  // namespace lib_names

}  // namespace rex::platform
