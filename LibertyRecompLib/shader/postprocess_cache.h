#pragma once

#include <cstdint>
#include <cstddef>

// Post-process shader cache entry (mirrors ShaderCacheEntry layout for postprocess shaders)
struct PostProcessShaderEntry
{
    uint64_t hash;
    const uint8_t* spirvData;
    size_t spirvSize;
    const uint8_t* dxilData;
    size_t dxilSize;
    const uint8_t* airData;
    size_t airSize;
};

extern PostProcessShaderEntry g_postprocessShaderEntries[];
extern const size_t g_postprocessShaderCount;
