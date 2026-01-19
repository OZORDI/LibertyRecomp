// =============================================================================
// Shadow Quality System
// Provides configuration and hooks for shadow quality enhancements
// =============================================================================
#pragma once

#include <cstdint>
#include "../user/config.h"

namespace ShadowQuality {

// =============================================================================
// Shadow Quality State
// =============================================================================
struct ShadowState {
    bool initialized = false;
    EShadowResolution resolution = EShadowResolution::x1024;
    EShadowFilter filter = EShadowFilter::Off;
    
    // Shadow map dimensions based on config
    uint32_t shadowMapWidth = 1024;
    uint32_t shadowMapHeight = 1024;
    
    // PCF filter parameters
    float pcfTexelSize = 1.0f / 1024.0f;
    float pcfBias = 0.001f;
    float pcfNormalBias = 0.01f;
    float pcssLightSize = 4.0f;  // For PCSS penumbra calculation
};

extern ShadowState g_shadowState;

// =============================================================================
// API Functions
// =============================================================================

// Initialize shadow quality system
void Initialize();

// Update shadow state from config (call when config changes)
void UpdateFromConfig();

// Shutdown shadow quality system
void Shutdown();

// Get current shadow map resolution
void GetShadowMapDimensions(uint32_t& width, uint32_t& height);

// Get PCF filter kernel size based on config
// Returns: 0 = no filter, 1 = 3x3, 2 = 5x5, 3 = 7x7, 4 = PCSS
int GetPCFKernelSize();

// Get texel size for PCF sampling
float GetPCFTexelSize();

// Get shadow bias values
void GetShadowBias(float& depthBias, float& normalBias);

// Get PCSS light size for contact hardening
float GetPCSSLightSize();

// Check if shadow filtering is enabled
bool IsFilteringEnabled();

// =============================================================================
// Shadow Shader Constants (for GPU upload)
// =============================================================================
struct ShadowConstants {
    float shadowMapSize[4];     // xy = size, zw = 1/size
    float shadowParams[4];      // x = bias, y = normalBias, z = filterSize, w = lightSize
};

// Fill shadow constants structure for shader upload
void GetShaderConstants(ShadowConstants& constants);

} // namespace ShadowQuality
