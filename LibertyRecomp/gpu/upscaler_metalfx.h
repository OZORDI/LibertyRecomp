// =============================================================================
// MetalFX Temporal Upscaler - C++ Interface Header
// =============================================================================
#pragma once

#include <cstdint>

namespace Upscaler {

// C++ interface for MetalFX (implementation in upscaler_metalfx.mm)
bool MetalFX_Initialize(void* mtlDevice, uint32_t inputWidth, uint32_t inputHeight,
                        uint32_t outputWidth, uint32_t outputHeight);

void MetalFX_Shutdown();

bool MetalFX_UpscaleTemporal(void* colorTexture, void* depthTexture,
                              void* motionTexture, void* outputTexture,
                              float jitterX, float jitterY, bool reset);

bool MetalFX_UpscaleSpatial(void* colorTexture, void* outputTexture);

void MetalFX_ResetHistory();

bool MetalFX_IsTemporalAvailable();
bool MetalFX_IsSpatialAvailable();

} // namespace Upscaler
