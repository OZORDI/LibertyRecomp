// streaming_prefetch_patches.cpp
// Predictive streaming prefetch for PC — increases effective streaming radius
// based on camera velocity so resources ahead of camera movement are loaded
// before they're needed.
//
// On Xbox 360, DVD latency made prefetching impractical. On PC with SSDs,
// we can predict where the camera is heading and pre-request resources
// in that direction at extended distances.
//
// Hook target: sub_821D08A8 (StreamAtDistance, 94 callers)
//   r3 = resource_id (streaming module index, 0-648 range)
//   f1 = distance from camera
//   Computes LOD distance, calls sub_821D2080(id, combined_distance) to request
//
// Camera position is written per-frame by sub_821EC8C8 (scene streaming state
// machine) to 0x82AA89D0 as a vec3 float (x, y, z).
//
// Strategy: track camera velocity, scale the distance parameter fed to the
// original StreamAtDistance so the game naturally requests at a larger radius
// when the player is moving fast. No new streaming requests are injected —
// we just inflate the distance the existing callers pass in.

#include <kernel/function.h>
#include <kernel/memory.h>
#include <cmath>
#include <cstdint>

// =============================================================================
// Guest memory addresses
// =============================================================================

// Camera position vec3 (float x, y, z) — written per-frame by scene state machine
static constexpr uint32_t ADDR_CAMERA_POS = 0x82AA89D0;

// Scene state machine state dword — 0 = inactive, >2 = camera pos valid
static constexpr uint32_t ADDR_SCENE_STATE = 0x82B977F0;

// =============================================================================
// Prefetch tuning constants
// =============================================================================

// Minimum camera speed (world units/frame) before prefetch activates.
// Below this threshold the camera is essentially stationary — no prefetch needed.
static constexpr float VELOCITY_THRESHOLD = 0.5f;

// Maximum distance multiplier applied on top of the base distance.
// At max speed the effective streaming radius becomes (1 + MAX_DISTANCE_SCALE)
// times the original. 1.5 means up to 2.5x the base radius.
static constexpr float MAX_DISTANCE_SCALE = 1.5f;

// Camera speed at which MAX_DISTANCE_SCALE is fully applied (units/frame).
// Tuned for GTA IV vehicle speeds: ~60 units/frame at highway speed.
static constexpr float VELOCITY_SATURATION = 60.0f;

// Exponential smoothing factor for velocity (0..1). Lower = smoother but laggier.
// 0.15 gives roughly 6-frame smoothing window.
static constexpr float VELOCITY_SMOOTHING = 0.15f;

// =============================================================================
// Camera velocity tracker (host-side state, NOT in guest memory)
// =============================================================================

static float s_prevCamX = 0.0f;
static float s_prevCamY = 0.0f;
static float s_prevCamZ = 0.0f;
static float s_smoothedSpeed = 0.0f;
static bool  s_initialized = false;

// Reads current camera position from guest memory and updates smoothed speed.
// Called once per invocation of the hook; since StreamAtDistance is called many
// times per frame (94 callers), we gate the update so it only recalculates
// when the camera position has actually changed.
static float GetDistanceScale()
{
    // Don't prefetch until the scene state machine is past initialization
    uint32_t sceneState = PPC_LOAD_U32(ADDR_SCENE_STATE);
    if (sceneState < 3) {
        s_initialized = false;
        s_smoothedSpeed = 0.0f;
        return 1.0f;
    }

    // Read camera position (3 consecutive floats, big-endian in guest memory)
    union { uint32_t u; float f; } cx, cy, cz;
    cx.u = PPC_LOAD_U32(ADDR_CAMERA_POS + 0);
    cy.u = PPC_LOAD_U32(ADDR_CAMERA_POS + 4);
    cz.u = PPC_LOAD_U32(ADDR_CAMERA_POS + 8);

    float camX = cx.f;
    float camY = cy.f;
    float camZ = cz.f;

    if (!s_initialized) {
        s_prevCamX = camX;
        s_prevCamY = camY;
        s_prevCamZ = camZ;
        s_initialized = true;
        return 1.0f;
    }

    // Compute per-frame displacement
    float dx = camX - s_prevCamX;
    float dy = camY - s_prevCamY;
    float dz = camZ - s_prevCamZ;

    // Only update prev position when it actually changed (multiple calls per frame)
    float dispSq = dx * dx + dy * dy + dz * dz;
    if (dispSq > 0.0001f) {
        float speed = sqrtf(dispSq);

        // Exponential moving average for smooth velocity
        s_smoothedSpeed = s_smoothedSpeed * (1.0f - VELOCITY_SMOOTHING)
                        + speed * VELOCITY_SMOOTHING;

        s_prevCamX = camX;
        s_prevCamY = camY;
        s_prevCamZ = camZ;
    }

    // Below threshold: no scaling
    if (s_smoothedSpeed < VELOCITY_THRESHOLD) {
        return 1.0f;
    }

    // Linear ramp from 1.0 to (1.0 + MAX_DISTANCE_SCALE)
    float t = (s_smoothedSpeed - VELOCITY_THRESHOLD)
            / (VELOCITY_SATURATION - VELOCITY_THRESHOLD);
    if (t > 1.0f) t = 1.0f;

    return 1.0f + t * MAX_DISTANCE_SCALE;
}

// =============================================================================
// Hook: sub_821D08A8 — StreamAtDistance
// =============================================================================
// Original signature: void StreamAtDistance(uint32_t resource_id, double distance)
//   r3  = resource_id
//   f1  = distance from camera (float passed as double in PPC ABI)
//
// We scale f1 by the camera-velocity-based distance multiplier, then fall
// through to the original implementation. This makes the game naturally
// request resources at greater distances when moving fast.

PPC_FUNC_IMPL(__imp__sub_821D08A8);
PPC_FUNC_HOOK(sub_821D08A8)
{
    float scale = GetDistanceScale();

    if (scale > 1.0f) {
        // Scale the distance parameter — the original function reads f1
        ctx.f1.f64 = ctx.f1.f64 * static_cast<double>(scale);
    }

    // Call the original StreamAtDistance with (potentially inflated) distance
    __imp__sub_821D08A8(ctx, base);
}
