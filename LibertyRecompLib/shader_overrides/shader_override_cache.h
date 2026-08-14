#pragma once

#include <cstddef>
#include <cstdint>

inline constexpr uint32_t kShaderOverrideStagePixel = 0;
inline constexpr uint32_t kShaderOverrideStageVertex = 1;

struct ShaderOverrideCacheEntry {
  uint64_t hash;
  uint32_t stage;
  uint32_t specialization_constants_mask;
  const uint32_t* spirv;
  size_t spirv_size;
  const uint32_t* late_spirv;
  size_t late_spirv_size;
  const char* filename;
};

extern const ShaderOverrideCacheEntry g_shaderOverrideEntries[];
extern const size_t g_shaderOverrideEntryCount;
