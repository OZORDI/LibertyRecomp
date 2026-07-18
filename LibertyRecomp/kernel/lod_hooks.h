// =============================================================================
// LOD SYSTEM HOOKS - GTA IV Recompiled
// =============================================================================
// This module provides hooks and patches to modify the Level of Detail (LOD)
// system and render distances in GTA IV.
//
// Key global variables controlled:
//   0x82CDDC3C - farClipMultiplier (from VISUALSETTINGS.DAT, default 2.0)
//   0x82CDDC34 - heightStart (LOD scaling height start, default 100.0)
//   0x82CDDC38 - nearFogMultiplier (fog distance multiplier, default 0.5)
//   0x82A2E908 - Dynamic far clip factor (calculated per-frame, default 1.0)
//   0x82A2E90C - Dynamic near fog factor (calculated per-frame, default 1.0)
//   0x82A22E7C - Entity fade distance (default 500.0)
//   0x82A22F7C - Max render distance (default 1000.0)
//   0x82A37A84 - IPL streaming base distance (default 200.0)
//   0x82A2FC10 - Pedestrian LOD distance (default 200.0)
//   0x82A31BE4 - Building detail distance (default 100.0)
//
// Key functions hooked:
//   sub_822F6B40 - Calculates height-based LOD multipliers each frame
//   sub_822F4EC0 - Applies multipliers to render settings
//   sub_824E5148 - IplStore streaming distance initialization
// =============================================================================

#pragma once

#include <cstdint>
#include <cpu/ppc_context.h>

namespace LODHooks {

// =============================================================================
// Initialization
// =============================================================================

/// Initialize LOD hooks and apply initial memory patches
/// Call this during game initialization (after memory regions are set up)
void Initialize(uint8_t* base);

/// Apply LOD patches based on current config values
/// Can be called to update patches at runtime (e.g., from options menu)
void ApplyConfigPatches(uint8_t* base);

// =============================================================================
// MidAsm Hook Functions
// =============================================================================

/// Hook for sub_822F6B40 - LOD multiplier calculation
/// Intercepts the dynamic far clip factor calculation
void LODMultiplierCalculationHook(PPCRegister& f12);

/// Hook for sub_822F4EC0 - Render settings application
/// Intercepts the application of LOD multipliers to render settings
void RenderSettingsApplyHook(PPCRegister& f0, PPCRegister& f13);

/// Hook for sub_824E5148 - Streaming distance initialization
/// Modifies the streaming distance when IplStore initializes
void StreamingDistanceInitHook(PPCRegister& f1);

/// Disable the game's native DoF when custom DoF is enabled
/// Sets DOF_BLUR_MULTIPLIER (0x82A2E900) to 0.0 to prevent double-blur
/// Should be called when custom DoF is enabled, or per-frame
void DisableNativeDoF(uint8_t* base);

/// Restore the game's native DoF multiplier to its original value
/// Call when custom DoF is disabled
void RestoreNativeDoF(uint8_t* base);

/// Disable the game's native Edge Anti-Aliasing (EAA) when custom AA is enabled
/// Zeros out EAA_PARAMS2 shader parameter to prevent double-AA
/// Should be called when TAA/SMAA/FSR is enabled
void DisableNativeAA(uint8_t* base);

/// Restore the game's native EAA (if it was saved)
/// Call when custom AA is disabled
void RestoreNativeAA(uint8_t* base);

// =============================================================================
// Memory Address Constants (Xbox 360 guest addresses)
// =============================================================================
namespace Addresses {

// VISUALSETTINGS.DAT loaded values
constexpr uint32_t FAR_CLIP_MULTIPLIER     = 0x82CDDC3C;  // float, default 2.0
constexpr uint32_t HEIGHT_START            = 0x82CDDC34;  // float, default 100.0
constexpr uint32_t HEIGHT_MULTIPLIER       = 0x82CDDC30;  // float, calculated
constexpr uint32_t NEAR_FOG_MULTIPLIER     = 0x82CDDC38;  // float, default 0.5

// Dynamic per-frame calculated values
constexpr uint32_t DYNAMIC_FAR_CLIP_FACTOR = 0x82A2E908;  // float, default 1.0
constexpr uint32_t DYNAMIC_NEAR_FOG_FACTOR = 0x82A2E90C;  // float, default 1.0
constexpr uint32_t DOF_BLUR_MULTIPLIER     = 0x82A2E900;  // float

// PostFX / Anti-Aliasing system
constexpr uint32_t POSTFX_MANAGER_PTR      = 0x82CC1118;  // pointer to PostFX manager
constexpr uint32_t EAA_PARAMS_OFFSET       = 728;         // offset in PostFX struct to EAA_PARAMS2 handle (a1[182])

// Entity distance thresholds
constexpr uint32_t ENTITY_FADE_DISTANCE    = 0x82A22E7C;  // float, default 500.0
constexpr uint32_t MAX_RENDER_DISTANCE     = 0x82A22F7C;  // float, default 1000.0
constexpr uint32_t NEAR_LOD_THRESHOLD      = 0x82A23238;  // float, default 200.0

// Entity-specific distances
constexpr uint32_t PED_LOD_DISTANCE        = 0x82A2FC10;  // float, default 200.0
constexpr uint32_t PED_DETAIL_DISTANCE     = 0x82A2FC14;  // float, default 70.0
constexpr uint32_t PED_SHADOW_DISTANCE     = 0x82A2FC18;  // float, default 65.0
constexpr uint32_t PED_CLOSE_DISTANCE      = 0x82A2FC1C;  // float, default 10.0

// Building/world distances
constexpr uint32_t BUILDING_LOD_THRESHOLD  = 0x82A31BE0;  // float, default 60.0
constexpr uint32_t BUILDING_DETAIL_DIST    = 0x82A31BE4;  // float, default 100.0

// Streaming distances
constexpr uint32_t IPL_STREAMING_BASE_DIST = 0x82A37A84;  // float, default 200.0
constexpr uint32_t STREAMING_OFFSET        = 0x82F4E83C;  // float, varies

// LOD system control
constexpr uint32_t LOD_BIAS                = 0x82A22F84;  // float, default 15.0
constexpr uint32_t MAX_LOD_LEVELS          = 0x82A22F80;  // int, default 3
constexpr uint32_t SCREEN_SIZE_THRESHOLD   = 0x82A2308C;  // float, default 360.0
constexpr uint32_t MIN_SCREEN_SIZE         = 0x82A23090;  // float, default 10.0

// Model pool limits
constexpr uint32_t MODEL_POOL_SIZE         = 0x82A230A0;  // int, default 4000
constexpr uint32_t ACTIVE_MODEL_LIMIT      = 0x82A23230;  // int, default 43
constexpr uint32_t STREAMING_SLOTS         = 0x82A231E8;  // int, default 57

// Interior distance
constexpr uint32_t INTERIOR_DISTANCE       = 0x82017ED4;  // float, default 2500.0

// General render distances
constexpr uint32_t GENERAL_RENDER_DIST     = 0x82A30FB8;  // float, default 300.0
constexpr uint32_t WORLD_RENDER_DIST       = 0x82A35080;  // float, default 1000.0

} // namespace Addresses

} // namespace LODHooks
