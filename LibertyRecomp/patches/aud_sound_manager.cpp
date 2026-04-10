// =============================================================================
// audSoundManager - RAGE Audio Sound Budget Manager
// =============================================================================
// Decompiled from GTA IV Xbox 360 (TU5 v0.0.8.5)
//
// The sound manager implements a priority-based voice budget system.
// Each audio frame, it:
//   1. Advances playback cursors for all active sound slots
//   2. Collects active sounds, computes distance-based priority, radix sorts
//   3. Distributes voice budget to highest-priority sounds
//   4. Re-activates dynamic sounds that regained budget
//   5. Allocates voices for newly budgeted static sounds
//   6. Drives all voice objects (play/stop/seek/update)
//   7. Services pending sound requests from the queue
//
// Global addresses (guest .data segment 0x831Cxxxx):
//   0x831C8A38  audMixer  (+204=numCategories, +232=categoryDataPtr)
//   0x831C8E04  enable flag
//   0x831C8E88  static activation index array (u16[])
//   0x831C8E8C  dynamic re-activation index array (u16[])
//   0x831C8EC0  voice pool descriptor ptr
//   0x831C8EC9  voice pool lock byte
//   0x831C9440  sort workspace (8192 bytes)
//   0x831CB440  radix sort ping-pong buffer (2 x 2048 bytes)
//   0x831C6710  audSoundManager instance ptr

#include <stdafx.h>
#include <api/Liberty.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>

// =============================================================================
// Guest address constants
// =============================================================================

static constexpr uint32_t ADDR_AUDMIXER            = 0x831C8A38;
static constexpr uint32_t ADDR_NUM_CATEGORIES      = 0x831C8B04;  // audMixer+204
static constexpr uint32_t ADDR_CATEGORY_DATA_PTR   = 0x831C8B20;  // audMixer+232
static constexpr uint32_t ADDR_SORT_WORKSPACE      = 0x831C9440;
static constexpr uint32_t ADDR_RADIX_BUFFER        = 0x831CB440;
static constexpr uint32_t ADDR_STATIC_INDICES      = 0x831C8E88;
static constexpr uint32_t ADDR_DYNAMIC_INDICES     = 0x831C8E8C;
static constexpr uint32_t ADDR_VOICE_POOL_PTR      = 0x831C8EC0;
static constexpr uint32_t ADDR_VOICE_LOCK          = 0x831C8EC9;
static constexpr uint32_t ADDR_SM_INSTANCE         = 0x831C6710;
static constexpr uint32_t ADDR_ENABLE_FLAG         = 0x831C8E04;
static constexpr uint32_t ADDR_REQ_ARRAY           = 0x831C8618;
static constexpr uint32_t ADDR_REQ_COUNT           = 0x831C8624;
static constexpr uint32_t ADDR_REQ_LOCK            = 0x831C865C;
static constexpr uint32_t ADDR_DEFERRED_FLAG       = 0x831C8634;

// =============================================================================
// Structure constants
// =============================================================================

static constexpr uint32_t CATEGORY_STRIDE       = 14560;
static constexpr uint32_t SOUNDS_PER_CAT        = 96;
static constexpr uint32_t SOUND_DESC_SIZE       = 112;
static constexpr uint32_t SOUND_DESC_BASE       = 96;      // offset in category
static constexpr uint32_t CONTROL_BASE          = 10848;   // offset in category
static constexpr uint32_t CONTROL_STRIDE        = 32;
static constexpr uint32_t SORT_ENTRY_SIZE       = 16;
static constexpr uint32_t GROUP_HEADER_SIZE     = 32;      // 8 dwords
static constexpr uint32_t SORT_MAX_ENTRIES      = 512;
static constexpr uint32_t RADIX_BUCKETS         = 256;
static constexpr uint32_t VOICE_SIZE            = 768;
static constexpr uint32_t INDEX_SLOTS_PER_CAT   = 130;

// SoundDesc offsets
static constexpr uint32_t SD_CURVE_SCALE   = 0;
static constexpr uint32_t SD_GROUP_ID      = 4;
static constexpr uint32_t SD_WAVE_REF      = 20;
static constexpr uint32_t SD_FLAGS         = 24;
static constexpr uint32_t SD_FLAGS2        = 25;
static constexpr uint32_t SD_HALF_RADIUS   = 26;
static constexpr uint32_t SD_MIN_DIST      = 28;
static constexpr uint32_t SD_MAX_DIST      = 32;
static constexpr uint32_t SD_VOICE_SLOT    = 109;
static constexpr uint32_t SD_CAT_IDX       = 110;

// SoundDesc flag bits
static constexpr uint8_t SDF_DONE             = 0x01;
static constexpr uint8_t SDF_NEEDS_ACTIVATE   = 0x02;
static constexpr uint8_t SDF_NEEDS_SEEK       = 0x04;
static constexpr uint8_t SDF_NEWLY_ACTIVATED  = 0x08;
static constexpr uint8_t SDF_STREAMING        = 0x10;
static constexpr uint8_t SDF_ALWAYS_ACTIVE    = 0x20;

// ControlEntry field offsets (relative to base of entry)
static constexpr int32_t CE_MAX_POS     = -4;   // u32, at entry - 4
static constexpr int32_t CE_LOOP_END    = 0;    // u32, -1 = no loop
static constexpr int32_t CE_PACKED_POS  = 4;    // u32, bits[31:3]=pos, [2:0]=flags
static constexpr int32_t CE_SEEK_FLAGS  = 12;   // u8, bit7=seekPending
static constexpr int32_t CE_STATE       = 16;   // u32, bits[31:30]=activation

// Activation states (top 2 bits of CE_STATE)
static constexpr uint32_t CE_STATE_MASK  = 0xC0000000;
static constexpr uint32_t CE_INACTIVE    = 0x00000000;
static constexpr uint32_t CE_ACTIVE      = 0x40000000;
static constexpr uint32_t CE_PENDING     = 0x80000000;

// Slot flags (per-slot byte in category)
static constexpr uint8_t SLOT_ACTIVE        = 0x01;
static constexpr uint8_t SLOT_CURSOR_DIRTY  = 0x08;

// Voice status
static constexpr uint8_t VOICE_DISABLED     = 0x80;

// =============================================================================
// Sort/budget result struct (stack allocated in Update)
// =============================================================================
struct SortResult {
    uint16_t totalEntries;
    uint16_t numGroups;
    uint16_t numAlwaysActive;
    uint16_t numUngrouped;
};

// =============================================================================
// Forward declarations - original PPC functions called through
// =============================================================================
extern "C" {
    void __imp__sub_82931D98(PPCContext& ctx, uint8_t* base);  // UpdateCategorySlotCursors
    void __imp__sub_82931EB8(PPCContext& ctx, uint8_t* base);  // UpdateCategoryData
    void __imp__sub_829222E0(PPCContext& ctx, uint8_t* base);  // ComputeSoundIndex
    void __imp__sub_8291E488(PPCContext& ctx, uint8_t* base);  // AllocateVoice
    void __imp__sub_8291DF00(PPCContext& ctx, uint8_t* base);  // ProcessVoices
    void __imp__sub_82849918(PPCContext& ctx, uint8_t* base);  // YieldThread
    void __imp__sub_82902328(PPCContext& ctx, uint8_t* base);  // FlushVoiceUpdates
    void __imp__sub_82921E60(PPCContext& ctx, uint8_t* base);  // FreeSoundIndex
    void __imp__sub_82908068(PPCContext& ctx, uint8_t* base);  // PostUpdate
    void __imp__sub_829022B0(PPCContext& ctx, uint8_t* base);  // UpdateAndPostProcess

    // Sort/distribute inner implementations (called by Update)
    void __imp__sub_82931700(PPCContext& ctx, uint8_t* base);  // BucketSortSounds
    void __imp__sub_82931B98(PPCContext& ctx, uint8_t* base);  // DistributeSounds
    void __imp__sub_8291E260(PPCContext& ctx, uint8_t* base);  // UpdateDynamicSound
    void __imp__sub_8291E538(PPCContext& ctx, uint8_t* base);  // ActivateStaticSound
    void __imp__sub_8291E620(PPCContext& ctx, uint8_t* base);  // Update

    // PostUpdate helpers
    void __imp__sub_8285FF50(PPCContext& ctx, uint8_t* base);  // EnterCriticalSection
    void __imp__sub_8285FFA0(PPCContext& ctx, uint8_t* base);  // LeaveCriticalSection
    void __imp__sub_82920F60(PPCContext& ctx, uint8_t* base);  // StreamingUpdate
    void __imp__sub_829219F0(PPCContext& ctx, uint8_t* base);  // StreamService
    void __imp__sub_829072C0(PPCContext& ctx, uint8_t* base);  // GeneralService
    void __imp__sub_82907CE8(PPCContext& ctx, uint8_t* base);  // FlushPending
    void __imp__sub_82906450(PPCContext& ctx, uint8_t* base);  // QueueIterator
    void __imp__sub_82849790(PPCContext& ctx, uint8_t* base);  // IsResourceReady
    void __imp__sub_82906FA8(PPCContext& ctx, uint8_t* base);  // ActivateQueued
    void __imp__sub_82906D20(PPCContext& ctx, uint8_t* base);  // DeactivateQueued

    void rexcrt_memset(PPCContext& ctx, uint8_t* base);
}

// =============================================================================
// audSoundManager class
// =============================================================================
// This is the decompiled C++ class representing the RAGE audio sound budget
// manager. All methods operate on guest memory via PPC_LOAD/STORE macros.
//
// The class is not instantiated on the host - it provides static methods
// that operate on the guest-memory instance at ADDR_SM_INSTANCE.

namespace rage {

class audSoundManager {
public:
    // =========================================================================
    // UpdateCategoryData (sub_82931EB8)
    // =========================================================================
    // Iterates all active categories and advances each sound slot's playback
    // cursor by deltaTime * curveScale.
    //
    // For each category (stride 14560):
    //   For each of 96 slots:
    //     - If slot not active -> skip
    //     - If newly active (active but not cursor-dirty): mark dirty, skip
    //     - If always-active flag: skip cursor advance
    //     - If activation state is active(1) or inactive(0): advance cursor
    //     - Cursor = (int)(deltaTime * curveScale) + currentPos
    //     - If cursor >= maxPos: wrap via loop points
    static void UpdateCategoryData(PPCContext& ctx, uint8_t* base, int32_t deltaTime) {
        uint32_t numCats = PPC_LOAD_U32(ADDR_NUM_CATEGORIES);
        if (numCats == 0) return;

        uint32_t catDataPtr = PPC_LOAD_U32(ADDR_CATEGORY_DATA_PTR);
        for (uint32_t i = 0; i < numCats; i++) {
            uint32_t catBase = catDataPtr + i * CATEGORY_STRIDE;
            // Delegate to original PPC for precision-sensitive cursor math
            ctx.r3.s32 = deltaTime;
            ctx.r4.u32 = catBase;
            __imp__sub_82931D98(ctx, base);
        }
    }

    // =========================================================================
    // BucketSortSounds (sub_82931700)
    // =========================================================================
    // Collects all active sounds into a flat sort array, computes a
    // distance-based priority key, groups sounds with matching groupIds,
    // and performs an 8-pass LSD radix sort on ungrouped sounds.
    //
    // Priority key = ~((distSqSum * timeSq) >> 32)
    //   distSqSum = (minDist*65535)^2 + (maxDist*65535)^2
    //   timeSq    = (halfToFloat(radius) * 1000)^2
    //   Lower key = higher priority (closer sounds win)
    //   Always-active / streaming+active sounds get key 0
    //
    // The radix sort uses 256 buckets, 8 passes of 8 bits each (LSD first),
    // with a ping-pong buffer at ADDR_RADIX_BUFFER.
    //
    // Output:
    //   *soundListPtr = head of sorted ungrouped linked list
    //   *groupListPtr = head of sorted group linked list
    //   result->totalEntries, numGroups, numAlwaysActive, numUngrouped
    static void BucketSortSounds(PPCContext& ctx, uint8_t* base,
                                 uint32_t soundListAddr, uint32_t groupListAddr,
                                 uint32_t resultAddr) {
        // The sort involves complex bit manipulation, half-float conversion,
        // 64-bit multiply for priority, and an 8-pass radix sort with linked
        // list threading. We delegate to the original implementation which has
        // been verified to produce correct results.
        ctx.r3.u32 = soundListAddr;
        ctx.r4.u32 = groupListAddr;
        ctx.r5.u32 = resultAddr;
        __imp__sub_82931700(ctx, base);
    }

    // =========================================================================
    // DistributeSounds (sub_82931B98)
    // =========================================================================
    // Merges the sorted ungrouped and grouped lists and distributes
    // the voice budget. Sound groups are treated atomically - all members
    // of a group are admitted or none.
    //
    // Algorithm:
    //   while (ungroupedList || groupedList):
    //     if budget > 0:
    //       Pick whichever has better (lower) sort key
    //       For ungrouped: if state==0 (inactive) and voices left: admit
    //       For groups: if budget >= group.memberCount: admit all members
    //       Admitted sounds written to staticIndices[staticCount++]
    //     else (budget exhausted):
    //       Any sound with state==1 (currently active) -> dynamicIndices[dynCount++]
    //       These will be re-activated via UpdateDynamicSound
    //
    // Output:
    //   result+8  = staticCount  (sounds to newly activate)
    //   result+10 = dynamicCount (sounds to re-activate)
    static void DistributeSounds(PPCContext& ctx, uint8_t* base,
                                 uint32_t groupList, uint32_t ungroupedList,
                                 uint32_t maxVoices, uint32_t maxBudget,
                                 uint32_t resultAddr) {
        ctx.r3.u32 = groupList;
        ctx.r4.u32 = ungroupedList;
        ctx.r5.u32 = maxVoices;
        ctx.r6.u32 = maxBudget;
        ctx.r7.u32 = resultAddr;
        __imp__sub_82931B98(ctx, base);
    }

    // =========================================================================
    // UpdateDynamicSound (sub_8291E260)
    // =========================================================================
    // Re-activates a sound that regained voice budget. Resolves the sound
    // descriptor from a packed category|slot ID, calls the voice's Play
    // vfunc through the voice pool, and advances the control entry's
    // cursor by 80 samples to compensate for re-activation latency.
    //
    // packedId format: high byte = categoryIndex, low byte = slotIndex
    //
    // Steps:
    //   1. Decode catIdx, slotIdx from packedId
    //   2. Resolve SoundDesc: catData + catIdx*14560 + slotIdx*112 + 96
    //   3. Get voice slot from SoundDesc+109
    //   4. Look up category control entry via ComputeSoundIndex
    //   5. Check voice status flags - skip if disabled
    //   6. Call voice->Play() (vtable slot 4)
    //   7. Set control state to CE_PENDING (bits 31:30 = 0b10)
    //   8. Advance cursor position by 80, wrapping at loop points
    static void UpdateDynamicSound(PPCContext& ctx, uint8_t* base,
                                   uint32_t managerAddr, uint16_t packedId) {
        ctx.r3.u32 = managerAddr;
        ctx.r4.u32 = packedId;
        __imp__sub_8291E260(ctx, base);
    }

    // =========================================================================
    // ActivateStaticSound (sub_8291E538)
    // =========================================================================
    // Allocates a new voice for a sound that was just granted budget.
    //
    // Steps:
    //   1. Decode catIdx, slotIdx from packedId
    //   2. Resolve SoundDesc
    //   3. Look up control entry via ComputeSoundIndex
    //   4. Call AllocateVoice (sub_8291E488):
    //      - Checks waveRef to determine if streaming or one-shot
    //      - Attempts to claim a free voice from the pool
    //      - Calls voice->Init() (vtable slot 1)
    //      - Returns voice pointer or 0
    //   5. Set control state to CE_ACTIVE (bits 31:30 = 0b01)
    //   6. Store voiceSlotIndex = (voicePtr - poolBase) / 768
    //   7. Set SDF_NEEDS_ACTIVATE flag in SoundDesc
    static void ActivateStaticSound(PPCContext& ctx, uint8_t* base,
                                    uint32_t managerAddr, uint16_t packedId) {
        ctx.r3.u32 = managerAddr;
        ctx.r4.u32 = packedId;
        __imp__sub_8291E538(ctx, base);
    }

    // =========================================================================
    // ProcessVoices (sub_8291DF00)
    // =========================================================================
    // Drives all voice objects in the voice pool. For each active voice:
    //
    //   - If activation state is CE_PENDING and voice->IsPlaying() returns
    //     false: release the voice (call voice->Stop, voice->Destroy)
    //   - If SDF_NEWLY_ACTIVATED: call voice->SetPosition with seek offset,
    //     clear the flag
    //   - If SDF_NEEDS_ACTIVATE and (not paused or has seek flag):
    //     call voice->Play() if position != -1, clear the flag
    //   - If SDF_DONE and voice->IsPlaying() returns false:
    //     release voice, free sound index, clear state
    //   - Otherwise: call voice->Update(), poll position from voice,
    //     update cursor in control entry
    //
    // Waits for voice pool lock (ADDR_VOICE_LOCK) before iterating.
    // Writes active voice count to manager+16.
    static void ProcessVoices(PPCContext& ctx, uint8_t* base, uint32_t managerAddr) {
        ctx.r3.u32 = managerAddr;
        __imp__sub_8291DF00(ctx, base);
    }

    // =========================================================================
    // Update (sub_8291E620) - Main entry point
    // =========================================================================
    // Called once per audio frame. Orchestrates the full pipeline:
    //
    //   deltaTime = currentTime - lastUpdateTime
    //   lastUpdateTime = currentTime
    //
    //   1. UpdateCategoryData(deltaTime)
    //   2. BucketSortSounds(&soundList, &groupList, stats)
    //   3. DistributeSounds(groupList, soundList, maxVoices, budget, stats)
    //   4. for each dynamicIndices[i]: UpdateDynamicSound(id)
    //   5. for each staticIndices[i]:  ActivateStaticSound(id)
    //   6. ProcessVoices()
    //
    // Parameters:
    //   r3 = audSoundManager* (guest ptr at 0x831C6710)
    //   r4 = currentTime (milliseconds)
    static void Update(PPCContext& ctx, uint8_t* base) {
        // Forward to original - this is the orchestrator that calls all
        // the sub-steps above in sequence. The individual functions can
        // be hooked independently for debugging or modification.
        __imp__sub_8291E620(ctx, base);
    }

    // =========================================================================
    // PostUpdate (sub_82908068)
    // =========================================================================
    // Services pending sound requests under critical section.
    //
    // Phase 1: Active request slots (array at 0x831C8618, count at 0x831C8624)
    //   Each slot is 144 bytes:
    //     +36: type (1 = streaming)
    //     +52: resource handle
    //     +70: hasData flag
    //     +71: needsService flag
    //   For type==1: call StreamingUpdate
    //   If needsService: dispatch StreamService or GeneralService
    //
    // Phase 2: If no pending work, flush (sub_82907CE8)
    //   Check deferred flag at 0x831C8634
    //
    // Phase 3: Walk sound queue via iterator (sub_82906450)
    //   type==1: StreamService (finalize)
    //   needsService + resourceReady: ActivateQueued
    //   !needsService + hasData: DeactivateQueued
    //   Break on first request that becomes pending
    static void PostUpdate(PPCContext& ctx, uint8_t* base) {
        __imp__sub_82908068(ctx, base);
    }

    // =========================================================================
    // UpdateAndPostProcess (sub_829022B0) - Top-level caller
    // =========================================================================
    // Called by the audio thread. Chains Update + PostUpdate:
    //   audSoundManager::Update(g_instance, currentTime)
    //   audSoundManager::PostUpdate()
    static void UpdateAndPostProcess(PPCContext& ctx, uint8_t* base) {
        __imp__sub_829022B0(ctx, base);
    }
};

} // namespace rage

// =============================================================================
// PPC Hook implementations
// =============================================================================
// These hooks wrap the original PPC functions. They can be extended with
// logging, profiling, or behavioral overrides while preserving the
// original game logic.

// sub_82931EB8 - UpdateCategoryData
// Advances playback cursors for all active sounds across all categories.
// Inner loop (sub_82931D98) handles per-slot cursor math with float->int
// conversion and loop-point wrapping.
PPC_FUNC_HOOK(sub_82931EB8) {
    __imp__sub_82931EB8(ctx, base);
}

// sub_82931700 - BucketSortSounds
// Collects active sounds, computes distance-based priority keys, groups
// sounds by groupId, and performs 8-pass LSD radix sort.
// Sort workspace: 512 entries x 16 bytes at 0x831C9440
// Radix buffer: 2 x 2048 bytes at 0x831CB440
PPC_FUNC_HOOK(sub_82931700) {
    __imp__sub_82931700(ctx, base);
}

// sub_82931B98 - DistributeSounds
// Walks merged priority list, admits sounds within voice budget to
// static array (0x831C8E88), overflows active sounds to dynamic
// array (0x831C8E8C) for re-activation.
PPC_FUNC_HOOK(sub_82931B98) {
    __imp__sub_82931B98(ctx, base);
}

// NOTE: sub_8291E260, sub_8291E538, sub_8291E620 moved to audio_pool.cpp (full decompilation)
// NOTE: sub_82908068, sub_829022B0 moved to scene_tick_patches.cpp (full decompilation)
