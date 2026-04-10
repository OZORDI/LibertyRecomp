// streaming_lod_patches.cpp
// LOD draw distance overrides for PC — removes Xbox 360 DVD-era streaming
// constraints that limited draw distances to 800/2500/250 world units.
//
// On Xbox 360 (512 MB RAM, 12 MB/s DVD), the scene streaming state machine
// kept draw distances low to avoid exceeding memory and I/O budgets.
// On PC with ample RAM and SSD storage, we can push draw distances much
// higher so all LODs load at maximum quality without visible pop-in.
//
// Hooked functions:
//   sub_821E4B30 = DrawDistanceAdvance (2 callers, 352 bytes)
//     - Sets per-scene draw distance and fade threshold
//     - Original values: 800 (normal), 2500 (highway), 250 (interior)
//     - We override all to PC_DRAW_DISTANCE (10000)
//
//   sub_821EBC40 = SceneInstantCommit (4 callers, 1624 bytes)
//     - Commits a new scene LOD level and calls sub_821ED738 with draw_dist
//     - Original values: 800 (match) or 0 (mismatch)
//     - We boost the draw distance written for matching scenes
//
// Guest memory map:
//   0x82B977F0  dword  Scene state machine state (set by sub_821E4B30)
//   0x82B96F14  dword  Current scene index
//   0x82B96F0C  dword  Target scene index
//   0x82B977B0  dword[4] Scene struct pointers (66 KB each)
//   0x82B96F08  float  Draw distance accumulator
//   0x82B96E9C  dword  Scene flags (bit 3 = highway, bit 2 = interior)
//   0x82B95EE6  byte   Advance-in-progress flag
//   0x82B95EE4  byte   LOD continuation flag
//   0x82B978B0  struct Scene data base (passed to fade setter)
//
// The state machine itself (sub_821EC8C8) is NOT hooked here — it still
// runs all 12 states. We only boost the distance thresholds so transitions
// happen at much greater range, effectively loading max-quality LODs everywhere.

#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>
#include <cstdint>

// =============================================================================
// PC draw distance tuning
// =============================================================================

// Maximum draw distance for all scene types on PC.
// Xbox 360 used 800 (normal), 2500 (highway), 250 (interior).
// 10000 units covers the entire visible city in most camera positions.
static constexpr float PC_DRAW_DISTANCE = 10000.0f;

// Fade offset subtracted from draw distance for LOD transition blending.
// Original game uses -60. We keep it proportional but the absolute value
// is small relative to 10000, so pop-in is invisible.
static constexpr int32_t FADE_OFFSET = 60;

// =============================================================================
// Guest memory addresses
// =============================================================================

static constexpr uint32_t ADDR_SCENE_PTR_ARRAY = 0x82B977B0; // dword[4]
static constexpr uint32_t ADDR_SCENE_INDEX     = 0x82B96F14; // current scene idx
static constexpr uint32_t ADDR_DRAW_ACCUM      = 0x82B96F08; // float accumulator
static constexpr uint32_t ADDR_SCENE_FLAGS     = 0x82B96E9C; // scene flag bits
static constexpr uint32_t ADDR_ADVANCE_FLAG    = 0x82B95EE6; // byte
static constexpr uint32_t ADDR_LOD_CONT_FLAG   = 0x82B95EE4; // byte
static constexpr uint32_t ADDR_SCENE_COUNT     = 0x82B95EFC; // dword
static constexpr uint32_t ADDR_SCENE_STATE     = 0x82B977F0; // state machine state
static constexpr uint32_t ADDR_TARGET_INDEX    = 0x82B96F0C; // target scene idx

// =============================================================================
// Hook: sub_821E4B30 — DrawDistanceAdvance
// =============================================================================
// Called during scene state machine states 9/10 to advance the LOD draw
// distance for the current scene. The original code picks 800/2500/250
// based on highway/interior flags, subtracts 60 for fade, and writes
// the result. We override to PC_DRAW_DISTANCE for all scene types.
//
// Original logic (pseudocode from IDA):
//   if (advance_flag) {
//       scene[idx].draw_distance = accum + 800.0;
//       unlock_streaming();
//       signal_ready(1);
//   } else {
//       lock_streaming();
//   }
//   if (scene_count == 1 || idx == scene_count - 1) a1 = 0;
//   if (can_set_fade) {
//       dist = 800;
//       if (!advance_flag) {
//           if (flags & 8) dist = 2500;  // highway
//           if (flags & 4) dist = 250;   // interior
//       }
//       sub_821ED6D8(scene_data, dist - 60, 0, 1);
//   }
//   state = (flag2 && a1) ? 9 : 10;

PPC_FUNC_IMPL(__imp__sub_821E4B30);
PPC_FUNC_HOOK(sub_821E4B30)
{
    // Before calling original: patch the draw distance accumulator so
    // scene[idx].draw_distance = accum + 800.0 becomes much larger.
    // The original adds 800.0 to the accumulator. We set the accumulator
    // to (PC_DRAW_DISTANCE - 800.0) so the sum equals PC_DRAW_DISTANCE.
    //
    // accum + 800.0 = PC_DRAW_DISTANCE
    // accum = PC_DRAW_DISTANCE - 800.0 = 9200.0
    static constexpr float BOOSTED_ACCUM = PC_DRAW_DISTANCE - 800.0f;

    uint8_t advanceFlag = PPC_LOAD_U8(ADDR_ADVANCE_FLAG);
    if (advanceFlag) {
        // Patch accumulator so the scene struct gets PC_DRAW_DISTANCE
        uint32_t accumBits;
        float accumVal = BOOSTED_ACCUM;
        std::memcpy(&accumBits, &accumVal, sizeof(accumBits));
        PPC_STORE_U32(ADDR_DRAW_ACCUM, accumBits);
    }

    // Call original — it will compute scene draw distance using our patched
    // accumulator, then call sub_821ED6D8 with (dist - 60) for fade.
    // The dist values (800/2500/250) are hardcoded in the original, but
    // the fade setter (sub_821ED6D8) just sets a screen fade distance,
    // which is less critical. The important thing is the scene struct
    // draw_distance at +16 which controls LOD loading.
    __imp__sub_821E4B30(ctx, base);

    // After original returns: override the scene struct draw_distance
    // to PC_DRAW_DISTANCE regardless of what was computed.
    uint32_t sceneIdx = PPC_LOAD_U32(ADDR_SCENE_INDEX);
    if (sceneIdx < 4) {
        uint32_t scenePtr = PPC_LOAD_U32(ADDR_SCENE_PTR_ARRAY + sceneIdx * 4);
        if (scenePtr) {
            float dist = PC_DRAW_DISTANCE;
            uint32_t distBits;
            std::memcpy(&distBits, &dist, sizeof(distBits));
            PPC_STORE_U32(scenePtr + 16, distBits);
        }
    }
}

// =============================================================================
// Hook: sub_821EBC40 — SceneInstantCommit
// =============================================================================
// Called when the scene state machine commits a new LOD level. This is the
// big commit function that initializes subsystems, flushes streaming, and
// sets up the new scene. On Xbox 360, it would wait for DVD loads.
//
// Key behavior we modify:
// 1. After the original commit completes, we override all active scene
//    draw distances to PC_DRAW_DISTANCE so new scenes start at max quality.
// 2. The original calls sub_821ED738(scene_data, 800, 1, 0) for matching
//    scenes — the fade distance. We post-patch scene structs to ensure
//    they all have PC_DRAW_DISTANCE.

PPC_FUNC_IMPL(__imp__sub_821EBC40);
PPC_FUNC_HOOK(sub_821EBC40)
{
    // Reset accumulator to boosted value before commit so any draw distance
    // calculations during commit use the higher value.
    static constexpr float BOOSTED_ACCUM = PC_DRAW_DISTANCE - 800.0f;
    {
        float accumVal = BOOSTED_ACCUM;
        uint32_t accumBits;
        std::memcpy(&accumBits, &accumVal, sizeof(accumBits));
        PPC_STORE_U32(ADDR_DRAW_ACCUM, accumBits);
    }

    // Call original — full commit with all subsystem init
    __imp__sub_821EBC40(ctx, base);

    // Post-commit: override draw distances on all active scene structs
    // to PC_DRAW_DISTANCE. The commit may have written lower values.
    float dist = PC_DRAW_DISTANCE;
    uint32_t distBits;
    std::memcpy(&distBits, &dist, sizeof(distBits));

    for (uint32_t i = 0; i < 4; ++i) {
        uint32_t scenePtr = PPC_LOAD_U32(ADDR_SCENE_PTR_ARRAY + i * 4);
        if (scenePtr) {
            PPC_STORE_U32(scenePtr + 16, distBits);
        }
    }
}
