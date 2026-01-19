// =============================================================================
// Shadow Quality System - Implementation
// =============================================================================

#include "shadow_quality.h"
#include <os/logger.h>

namespace ShadowQuality {

ShadowState g_shadowState;

void Initialize()
{
    g_shadowState = ShadowState();
    UpdateFromConfig();
    g_shadowState.initialized = true;
    
    LOGF_INFO("[ShadowQuality] Initialized: resolution={}x{} filter={}",
              g_shadowState.shadowMapWidth, g_shadowState.shadowMapHeight,
              static_cast<int>(g_shadowState.filter));
}

void UpdateFromConfig()
{
    g_shadowState.resolution = Config::ShadowResolution;
    g_shadowState.filter = Config::ShadowFilter;
    
    // Calculate shadow map dimensions
    int32_t res = static_cast<int32_t>(g_shadowState.resolution);
    if (res > 0) {
        g_shadowState.shadowMapWidth = static_cast<uint32_t>(res);
        g_shadowState.shadowMapHeight = static_cast<uint32_t>(res);
    } else {
        // Original resolution (game default is typically 512 or 1024)
        g_shadowState.shadowMapWidth = 1024;
        g_shadowState.shadowMapHeight = 1024;
    }
    
    // Update PCF texel size
    g_shadowState.pcfTexelSize = 1.0f / static_cast<float>(g_shadowState.shadowMapWidth);
    
    // Adjust bias based on resolution (higher res needs less bias)
    float resScale = 1024.0f / static_cast<float>(g_shadowState.shadowMapWidth);
    g_shadowState.pcfBias = 0.001f * resScale;
    g_shadowState.pcfNormalBias = 0.01f * resScale;
}

void Shutdown()
{
    g_shadowState.initialized = false;
    LOG_INFO("[ShadowQuality] Shutdown");
}

void GetShadowMapDimensions(uint32_t& width, uint32_t& height)
{
    width = g_shadowState.shadowMapWidth;
    height = g_shadowState.shadowMapHeight;
}

int GetPCFKernelSize()
{
    switch (g_shadowState.filter) {
        case EShadowFilter::Off:    return 0;
        case EShadowFilter::PCF3x3: return 1;
        case EShadowFilter::PCF5x5: return 2;
        case EShadowFilter::PCF7x7: return 3;
        case EShadowFilter::PCSS:   return 4;
        default:                    return 0;
    }
}

float GetPCFTexelSize()
{
    return g_shadowState.pcfTexelSize;
}

void GetShadowBias(float& depthBias, float& normalBias)
{
    depthBias = g_shadowState.pcfBias;
    normalBias = g_shadowState.pcfNormalBias;
}

float GetPCSSLightSize()
{
    return g_shadowState.pcssLightSize;
}

bool IsFilteringEnabled()
{
    return g_shadowState.filter != EShadowFilter::Off;
}

void GetShaderConstants(ShadowConstants& constants)
{
    float w = static_cast<float>(g_shadowState.shadowMapWidth);
    float h = static_cast<float>(g_shadowState.shadowMapHeight);
    
    constants.shadowMapSize[0] = w;
    constants.shadowMapSize[1] = h;
    constants.shadowMapSize[2] = 1.0f / w;
    constants.shadowMapSize[3] = 1.0f / h;
    
    constants.shadowParams[0] = g_shadowState.pcfBias;
    constants.shadowParams[1] = g_shadowState.pcfNormalBias;
    constants.shadowParams[2] = static_cast<float>(GetPCFKernelSize());
    constants.shadowParams[3] = g_shadowState.pcssLightSize;
}

} // namespace ShadowQuality
