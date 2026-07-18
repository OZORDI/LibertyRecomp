/**
 * @file        system/gpu_plugin.h
 * @brief       GPU emulation plugin ABI and host-side loader
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 *
 * @remarks     One plugin exists today (rexgpu-xenos). The factory carries an
 *              ABI version so this can grow into a general plugin API later.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <string_view>

#include <rex/system/interfaces/graphics.h>

#if defined(_WIN32)
#define REX_GPU_PLUGIN_EXPORT __declspec(dllexport)
#else
#define REX_GPU_PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

namespace rex::system {

// Bump on any change to GpuCreateInfo or to the IGraphicsSystem interface.
inline constexpr uint32_t kGpuPluginAbiVersion = 1;

inline constexpr const char* kGpuCreateSymbol = "rex_gpu_create";
inline constexpr const char* kGpuAbiVersionSymbol = "rex_gpu_abi_version";

struct GpuCreateInfo {
  uint32_t struct_size = 0;       // sizeof(GpuCreateInfo), set by the host
  const char* backend = nullptr;  // "d3d12", "vulkan", or "any"
};

// extern "C" exports every GPU plugin must provide:
//   uint32_t rex_gpu_abi_version(void);
//   rex::system::IGraphicsSystem* rex_gpu_create(uint32_t abi_version,
//                                                const GpuCreateInfo* info);
using GpuAbiVersionFn = uint32_t (*)();
using GpuCreateFn = IGraphicsSystem* (*)(uint32_t abi_version, const GpuCreateInfo* info);

// Loads rexgpu-<name> from the executable's directory and constructs its
// graphics system. Returns nullptr after logging a detailed error (missing
// file, missing exports, ABI mismatch, or factory failure). The library
// handle is retained for process lifetime; plugins are never unloaded.
std::unique_ptr<IGraphicsSystem> LoadGpuPlugin(std::string_view name,
                                               std::string_view backend = "any");

}  // namespace rex::system
