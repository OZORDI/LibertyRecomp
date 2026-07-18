// =============================================================================
// LOD SYSTEM HOOKS - GTA IV Recompiled
// =============================================================================
// Implementation of LOD and render distance modification hooks.
// See lod_hooks.h for detailed documentation.
// =============================================================================

#include "lod_hooks.h"
#include <stdafx.h>
#include <cpu/ppc_context.h>
#include <user/config.h>
#include <os/logger.h>
#include <cstring>

// Big-endian helpers for writing to guest memory
namespace {

inline void WriteFloatBE(uint8_t* base, uint32_t addr, float value) {
    uint32_t* ptr = reinterpret_cast<uint32_t*>(base + addr);
    uint32_t bits;
    memcpy(&bits, &value, sizeof(bits));
    *ptr = __builtin_bswap32(bits);
}

inline float ReadFloatBE(uint8_t* base, uint32_t addr) {
    uint32_t* ptr = reinterpret_cast<uint32_t*>(base + addr);
    uint32_t bits = __builtin_bswap32(*ptr);
    float value;
    memcpy(&value, &bits, sizeof(value));
    return value;
}

inline void WriteU32BE(uint8_t* base, uint32_t addr, uint32_t value) {
    uint32_t* ptr = reinterpret_cast<uint32_t*>(base + addr);
    *ptr = __builtin_bswap32(value);
}

inline uint32_t ReadU32BE(uint8_t* base, uint32_t addr) {
    uint32_t* ptr = reinterpret_cast<uint32_t*>(base + addr);
    return __builtin_bswap32(*ptr);
}

// Store base pointer for use in hooks
static uint8_t* g_guestBase = nullptr;

// Original values (for restoration if needed)
static float g_originalFarClipMultiplier = 2.0f;
static float g_originalEntityFadeDistance = 500.0f;
static float g_originalMaxRenderDistance = 1000.0f;
static float g_originalPedLODDistance = 200.0f;
static float g_originalBuildingLODDistance = 100.0f;
static float g_originalStreamingDistance = 200.0f;
static float g_originalDofBlurMultiplier = 1.0f;  // Default for HD mode
static bool g_nativeDofDisabled = false;

// AA disable state
static bool g_nativeAADisabled = false;

// Whether patches have been applied
static bool g_patchesApplied = false;

} // anonymous namespace

namespace LODHooks {

void Initialize(uint8_t* base) {
    g_guestBase = base;
    
    LOG_INFO("[LOD] Initializing LOD hooks system");
    LOGF_INFO("[LOD] Guest base address: {:p}", (void*)base);
    
    // Store original values for potential restoration
    // Note: These may not be initialized yet at this point
    // We'll read them after game initialization
    
    LOG_INFO("[LOD] LOD hooks initialized successfully");
}

void ApplyConfigPatches(uint8_t* base) {
    if (!base) {
        base = g_guestBase;
    }
    
    if (!base) {
        LOG_ERROR("[LOD] Cannot apply patches - base address not set");
        return;
    }
    
    LOG_INFO("[LOD] Applying LOD configuration patches");
    
    // Get config values
    float renderDistMult = Config::RenderDistanceMultiplier;
    float lodDistMult = Config::LODDistanceMultiplier;
    float streamingDistMult = Config::StreamingDistanceMultiplier;
    float farClipMult = Config::FarClipMultiplier;
    bool disableLOD = Config::DisableLOD;
    
    // Get override values (use config if set, otherwise calculate from multipliers)
    float entityFadeDist = Config::EntityFadeDistance;
    float maxRenderDist = Config::MaxRenderDistance;
    float pedLODDist = Config::PedestrianLODDistance;
    float buildingLODDist = Config::BuildingLODDistance;
    
    LOG_INFO("[LOD] Config values:");
    LOGF_INFO("[LOD]   RenderDistanceMultiplier: {:.2f}", renderDistMult);
    LOGF_INFO("[LOD]   LODDistanceMultiplier: {:.2f}", lodDistMult);
    LOGF_INFO("[LOD]   StreamingDistanceMultiplier: {:.2f}", streamingDistMult);
    LOGF_INFO("[LOD]   FarClipMultiplier: {:.2f}", farClipMult);
    LOGF_INFO("[LOD]   DisableLOD: {}", disableLOD ? "true" : "false");
    
    // ==========================================================================
    // Apply render distance patches
    // ==========================================================================
    
    // Entity fade distance (controls when entities start to fade out)
    float newEntityFade = entityFadeDist * renderDistMult;
    WriteFloatBE(base, Addresses::ENTITY_FADE_DISTANCE, newEntityFade);
    LOGF_INFO("[LOD] Entity fade distance: {:.1f} -> {:.1f}", entityFadeDist, newEntityFade);
    
    // Max render distance (controls maximum entity visibility)
    float newMaxRender = maxRenderDist * renderDistMult;
    WriteFloatBE(base, Addresses::MAX_RENDER_DISTANCE, newMaxRender);
    LOGF_INFO("[LOD] Max render distance: {:.1f} -> {:.1f}", maxRenderDist, newMaxRender);
    
    // Near LOD threshold
    float nearLOD = 200.0f * renderDistMult;
    WriteFloatBE(base, Addresses::NEAR_LOD_THRESHOLD, nearLOD);
    
    // General render distances
    WriteFloatBE(base, Addresses::GENERAL_RENDER_DIST, 300.0f * renderDistMult);
    WriteFloatBE(base, Addresses::WORLD_RENDER_DIST, 1000.0f * renderDistMult);
    WriteFloatBE(base, Addresses::INTERIOR_DISTANCE, 2500.0f * renderDistMult);
    
    // ==========================================================================
    // Apply LOD distance patches
    // ==========================================================================
    
    // Pedestrian LOD distances
    float newPedLOD = pedLODDist * lodDistMult;
    WriteFloatBE(base, Addresses::PED_LOD_DISTANCE, newPedLOD);
    WriteFloatBE(base, Addresses::PED_DETAIL_DISTANCE, 70.0f * lodDistMult);
    WriteFloatBE(base, Addresses::PED_SHADOW_DISTANCE, 65.0f * lodDistMult);
    LOGF_INFO("[LOD] Ped LOD distance: {:.1f} -> {:.1f}", pedLODDist, newPedLOD);
    
    // Building LOD distances
    float newBuildingLOD = buildingLODDist * lodDistMult;
    WriteFloatBE(base, Addresses::BUILDING_LOD_THRESHOLD, 60.0f * lodDistMult);
    WriteFloatBE(base, Addresses::BUILDING_DETAIL_DIST, newBuildingLOD);
    LOGF_INFO("[LOD] Building LOD distance: {:.1f} -> {:.1f}", buildingLODDist, newBuildingLOD);
    
    // Screen size thresholds (affect when LOD switches occur)
    if (lodDistMult > 1.0f) {
        // Reduce screen size thresholds to delay LOD switching
        WriteFloatBE(base, Addresses::SCREEN_SIZE_THRESHOLD, 360.0f / lodDistMult);
        WriteFloatBE(base, Addresses::MIN_SCREEN_SIZE, 10.0f / lodDistMult);
    }
    
    // LOD bias (higher = more aggressive LOD reduction, 0 = disabled)
    if (disableLOD) {
        WriteFloatBE(base, Addresses::LOD_BIAS, 0.0f);
        LOG_INFO("[LOD] LOD system disabled (bias set to 0)");
    } else {
        // Reduce bias proportionally to LOD multiplier
        float newBias = 15.0f / lodDistMult;
        WriteFloatBE(base, Addresses::LOD_BIAS, newBias);
    }
    
    // ==========================================================================
    // Apply streaming distance patches
    // ==========================================================================
    
    float newStreamingDist = 200.0f * streamingDistMult;
    WriteFloatBE(base, Addresses::IPL_STREAMING_BASE_DIST, newStreamingDist);
    LOGF_INFO("[LOD] Streaming distance: 200.0 -> {:.1f}", newStreamingDist);
    
    // ==========================================================================
    // Apply far clip multiplier patches
    // ==========================================================================
    
    // The far clip multiplier affects the overall draw distance
    // Original value from VISUALSETTINGS.DAT is 2.0
    float newFarClip = 2.0f * farClipMult;
    WriteFloatBE(base, Addresses::FAR_CLIP_MULTIPLIER, newFarClip);
    LOGF_INFO("[LOD] Far clip multiplier: 2.0 -> {:.2f}", newFarClip);
    
    // Also boost the dynamic factors for immediate effect
    // These are recalculated per-frame by sub_822F6B40, but we can set them higher
    float dynamicFarClip = 1.0f * farClipMult * renderDistMult;
    WriteFloatBE(base, Addresses::DYNAMIC_FAR_CLIP_FACTOR, dynamicFarClip);
    WriteFloatBE(base, Addresses::DYNAMIC_NEAR_FOG_FACTOR, 1.0f * farClipMult);
    
    // ==========================================================================
    // Increase model pool limits if distances are significantly increased
    // ==========================================================================
    
    if (renderDistMult > 1.5f || lodDistMult > 1.5f) {
        // Increase model pool to accommodate more visible models
        uint32_t newPoolSize = static_cast<uint32_t>(4000 * renderDistMult);
        uint32_t newActiveLimit = static_cast<uint32_t>(43 * renderDistMult);
        uint32_t newStreamingSlots = static_cast<uint32_t>(57 * renderDistMult);
        
        // Cap at reasonable values to avoid memory issues
        if (newPoolSize > 16000) newPoolSize = 16000;
        if (newActiveLimit > 200) newActiveLimit = 200;
        if (newStreamingSlots > 200) newStreamingSlots = 200;
        
        WriteU32BE(base, Addresses::MODEL_POOL_SIZE, newPoolSize);
        WriteU32BE(base, Addresses::ACTIVE_MODEL_LIMIT, newActiveLimit);
        WriteU32BE(base, Addresses::STREAMING_SLOTS, newStreamingSlots);
        
        LOGF_INFO("[LOD] Increased model pool: {} models, {} active, {} streaming slots",
                 newPoolSize, newActiveLimit, newStreamingSlots);
    }
    
    g_patchesApplied = true;
    LOG_INFO("[LOD] All LOD patches applied successfully");
}

// =============================================================================
// MidAsm Hook Implementations
// =============================================================================

void LODMultiplierCalculationHook(PPCRegister& f12) {
    // This hook intercepts the LOD multiplier calculation in sub_822F6B40
    // f12 contains the calculated heightFactor (0.0 to 1.0 based on camera height)
    //
    // The original calculation is:
    //   flt_82A2E908 = ((flt_82CDDC3C - 1.0) * heightFactor) + 1.0
    //
    // We can modify f12 to affect the final result, or we can let it proceed
    // and patch the result addresses directly in ApplyConfigPatches()
    
    // For now, we just log and let the patches handle it
    // Future enhancement: Could modify f12 here for per-frame control
}

void RenderSettingsApplyHook(PPCRegister& f0, PPCRegister& f13) {
    // This hook intercepts sub_822F4EC0 when it applies multipliers
    // f0/f13 contain intermediate calculation values
    //
    // The function modifies:
    //   offset +68: far clip distance (multiplied by flt_82A2E908)
    //   offset +44: packed fog distance byte
    //   offset +80/84: DOF blur multipliers
    
    // Apply our multipliers
    float renderMult = Config::RenderDistanceMultiplier;
    if (renderMult != 1.0f) {
        // Boost the value being applied
        f0.f64 *= renderMult;
        f13.f64 *= renderMult;
    }
}

void StreamingDistanceInitHook(PPCRegister& f1) {
    // This hook intercepts sub_824E5148 when IplStore initializes
    // f1 contains the streaming distance (default 200.0)
    
    float streamingMult = Config::StreamingDistanceMultiplier;
    if (streamingMult != 1.0f) {
        f1.f64 *= streamingMult;
        LOGF_INFO("[LOD] Streaming distance initialized: {:.1f}", f1.f64);
    }
}

void DisableNativeDoF(uint8_t* base) {
    if (!base) {
        base = g_guestBase;
    }
    
    if (!base) {
        LOG_ERROR("[DOF] Cannot disable native DoF - base address not set");
        return;
    }
    
    if (g_nativeDofDisabled) {
        return;  // Already disabled
    }
    
    // Save the original value before disabling
    g_originalDofBlurMultiplier = ReadFloatBE(base, Addresses::DOF_BLUR_MULTIPLIER);
    
    // Set DOF_BLUR_MULTIPLIER to 0.0 to disable game's native DoF
    // This makes all DoF blur calculations result in 0:
    //   *(float *)(&unk_82CF3BE0 + v6) *= flt_82A2E900;  // Near DoF
    //   *(float *)(&unk_82CF3BE4 + v6) *= flt_82A2E900;  // Far DoF
    WriteFloatBE(base, Addresses::DOF_BLUR_MULTIPLIER, 0.0f);
    
    g_nativeDofDisabled = true;
    LOGF_INFO("[DOF] Native DoF disabled (original multiplier: {:.2f})", g_originalDofBlurMultiplier);
}

void RestoreNativeDoF(uint8_t* base) {
    if (!base) {
        base = g_guestBase;
    }
    
    if (!base) {
        LOG_ERROR("[DOF] Cannot restore native DoF - base address not set");
        return;
    }
    
    if (!g_nativeDofDisabled) {
        return;  // Not disabled, nothing to restore
    }
    
    // Restore the original DOF_BLUR_MULTIPLIER value
    WriteFloatBE(base, Addresses::DOF_BLUR_MULTIPLIER, g_originalDofBlurMultiplier);
    
    g_nativeDofDisabled = false;
    LOGF_INFO("[DOF] Native DoF restored (multiplier: {:.2f})", g_originalDofBlurMultiplier);
}

void DisableNativeAA(uint8_t* base) {
    if (!base) {
        base = g_guestBase;
    }
    
    if (!base) {
        LOG_ERROR("[AA] Cannot disable native AA - base address not set");
        return;
    }
    
    if (g_nativeAADisabled) {
        return;  // Already disabled
    }
    
    // Read PostFX manager pointer from guest memory
    // GTA IV uses Edge Anti-Aliasing (EAA) through the rage_postfx shader system
    // EAA_PARAMS2 is at offset 728 (a1[182]) in the PostFX manager structure
    uint32_t postfxMgrPtr = ReadU32BE(base, Addresses::POSTFX_MANAGER_PTR);
    
    if (postfxMgrPtr == 0) {
        // PostFX manager not initialized yet - this is normal during startup
        return;
    }
    
    // Get the EAA_PARAMS2 shader parameter handle
    // This handle is used to set the edge anti-aliasing parameters
    uint32_t eaaHandle = ReadU32BE(base, postfxMgrPtr + Addresses::EAA_PARAMS_OFFSET);
    
    if (eaaHandle != 0) {
        // Zero out the shader parameter handle to disable EAA
        // When the handle is 0, the shader won't apply edge anti-aliasing
        WriteU32BE(base, postfxMgrPtr + Addresses::EAA_PARAMS_OFFSET, 0);
        
        g_nativeAADisabled = true;
        LOGF_INFO("[AA] Native Edge Anti-Aliasing disabled (PostFX mgr: 0x{:08X}, EAA handle was: 0x{:08X})", 
                 postfxMgrPtr, eaaHandle);
    }
}

void RestoreNativeAA(uint8_t* base) {
    if (!base) {
        base = g_guestBase;
    }
    
    if (!base) {
        LOG_ERROR("[AA] Cannot restore native AA - base address not set");
        return;
    }
    
    if (!g_nativeAADisabled) {
        return;  // Not disabled, nothing to restore
    }
    
    // Note: We don't store the original handle, so we just mark it as restored
    // The game will reinitialize EAA_PARAMS2 on the next PostFX update if needed
    // For a more robust implementation, we could cache the original handle
    
    g_nativeAADisabled = false;
    LOG_INFO("[AA] Native AA restore flag cleared - game will reinitialize EAA on next update");
}

} // namespace LODHooks
