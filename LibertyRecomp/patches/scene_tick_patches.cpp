// scene_tick_patches.cpp
// Scene tick and entity update chain - decompiled from GTA IV Xbox 360
//
// Call chain:
//   sub_82902420 (scene tick)
//     -> sub_8291FA80 (streaming subsystem update, at this+380)
//     -> sub_829158F0 (entity tick loop, 24 slots, at this+172)
//     -> sub_829022B0 (bridge: world update + entity finalize)
//        -> sub_8291E620 (world update on scene global)
//        -> sub_82908068 (entity finalize)
//   sub_82902328 (finalize frame: flush + increment frame counter)
//   sub_82915840 (cleanup: destroy entities + clear metadata + post-cleanup)
//     -> sub_829022E8 (pre-destroy thunk -> sub_8291EF18)
//     -> sub_82912B48 (post-cleanup thunk -> sub_8290F6B0)
//   sub_829222E0 (compute sector index from entity address)
//   sub_82921E60 (mark sector dirty)

#include <kernel/function.h>
#include <kernel/memory.h>
#include <cstring>
#include <cstdint>
#include <cmath>

// =============================================================================
// Globals (guest memory addresses)
// =============================================================================

// Scene global at 0x831C6710
static constexpr uint32_t ADDR_SCENE            = 0x831C6710;
// Streaming manager at 0x831C6750
static constexpr uint32_t ADDR_STREAMING_MGR     = 0x831C6750;
// Frame counter at 0x831C6700
static constexpr uint32_t ADDR_FRAME_COUNTER     = 0x831C6700;
// Streaming bounds base at 0x831C8A38
static constexpr uint32_t ADDR_STREAMING_BOUNDS  = 0x831C8A38;
// Triple-buffer index pair at 0x831C8BB0 / 0x831C8BB4
static constexpr uint32_t ADDR_BUFFER_INDEX      = 0x831C8BB0;
static constexpr uint32_t ADDR_BUFFER_INDEX_NEXT = 0x831C8BB4;
// Sector count at 0x831C8B04
static constexpr uint32_t ADDR_SECTOR_COUNT      = 0x831C8B04;
// Tick-enabled flag at 0x831C8E04
static constexpr uint32_t ADDR_TICK_ENABLED      = 0x831C8E04;
// Sector dirty swap flags at 0x831C9414 / 0x831C9415
static constexpr uint32_t ADDR_DIRTY_SWAP_A      = 0x831C9414;
static constexpr uint32_t ADDR_DIRTY_SWAP_B      = 0x831C9415;
// FPS cap flag at 0x82B1A360
static constexpr uint32_t ADDR_FPS_CAP_FLAG      = 0x82B1A360;
// Float constants pool at 0x831C6C80
static constexpr uint32_t ADDR_FLOAT_POOL        = 0x831C6C80;
// Tracked entity at 0x831C8F98
static constexpr uint32_t ADDR_TRACKED_ENTITY    = 0x831C8F98;

// =============================================================================
// Layout constants (Python-verified)
// =============================================================================

// Sector grid
static constexpr uint32_t SECTOR_STRIDE       = 14560;  // 0x38E0
static constexpr uint32_t ENTITY_SIZE          = 112;    // 0x70
static constexpr uint32_t SECTOR_HEADER_SIZE   = 96;     // 0x60
static constexpr uint32_t SECTOR_CELL_COUNT    = 64;     // cells per sector
static constexpr uint32_t SECTOR_CELL_SIZE     = 3;      // bytes per cell (flagA, flagB, active)
static constexpr uint32_t DIRTY_FLAG           = 4;      // bit 2

// Entity container layout (used by sub_82915840)
static constexpr uint32_t MAX_TICK_SLOTS       = 24;
static constexpr uint32_t CONTAINER_COUNT_IDX  = 24;     // a1[24] = entity count
static constexpr uint32_t CONTAINER_META_START = 25;     // a1[25..33] = 9 metadata dwords
static constexpr uint32_t CONTAINER_META_COUNT = 9;
static constexpr uint32_t CONTAINER_POST_IDX   = 34;     // a1[34] = post-cleanup data

// Scene tick object offsets
static constexpr uint32_t OFF_SUBSYSTEM_LIST   = 172;    // 0xAC  - subsystem pointer array
static constexpr uint32_t OFF_STREAMING_SUB    = 380;    // 0x17C - streaming subsystem
static constexpr uint32_t OFF_CURRENT_TIME     = 784;    // 0x310
static constexpr uint32_t OFF_AUDIO_FRAME_TIME = 792;    // 0x318 (qword)
static constexpr uint32_t OFF_SAVED_CAPACITY   = 800;    // 0x320
static constexpr uint32_t OFF_FLAG_A           = 804;    // 0x324
static constexpr uint32_t OFF_FLAG_B           = 805;    // 0x325

// Streaming subsystem offsets (relative to subsystem base)
static constexpr uint32_t SS_POS_X             = 192;    // 0xC0  (float)
static constexpr uint32_t SS_RATE_X            = 196;    // 0xC4  (float)
static constexpr uint32_t SS_FLAGS_X           = 200;    // 0xC8  (byte at high byte = paused)
static constexpr uint32_t SS_POS_Y             = 204;    // 0xCC  (float)
static constexpr uint32_t SS_RATE_Y            = 208;    // 0xD0  (float)
static constexpr uint32_t SS_FLAGS_Y           = 212;    // 0xD4
static constexpr uint32_t SS_POS_Z             = 216;    // 0xD8  (float)
static constexpr uint32_t SS_RATE_Z            = 220;    // 0xDC  (float)
static constexpr uint32_t SS_FLAGS_Z           = 224;    // 0xE0
static constexpr uint32_t SS_LAST_TIME         = 300;    // 0x12C
static constexpr uint32_t SS_STAT_A            = 304;    // 0x130
static constexpr uint32_t SS_STAT_B            = 308;    // 0x134
static constexpr uint32_t SS_STAT_C            = 312;    // 0x138
static constexpr uint32_t SS_STAT_D            = 316;    // 0x13C

// =============================================================================
// External function declarations (called but not hooked here)
// =============================================================================

PPC_FUNC_IMPL(__imp__sub_8286A370);  // Timer::Init(timer*, name)
PPC_FUNC_IMPL(__imp__sub_8286A2E8);  // Timer::Start(timer*)
PPC_FUNC_IMPL(__imp__sub_8286A308);  // Timer::Stop(timer*)
PPC_FUNC_IMPL(__imp__sub_8286A348);  // Timer::GetElapsed(timer*) -> float
PPC_FUNC_IMPL(__imp__sub_8290ED60);  // GetAudioFrameTime(timerVal) -> qword
PPC_FUNC_IMPL(__imp__sub_8290EDB8);  // GetCurrentTimeMs() -> int
PPC_FUNC_IMPL(__imp__sub_8291DED8);  // SetVisibleCapacity(scene, count)
PPC_FUNC_IMPL(__imp__sub_82A58008);  // sysMemExternalAllocator::GetAllocCount(alloc) -> int
PPC_FUNC_IMPL(__imp__sub_82849918);  // Sleep(ms)
PPC_FUNC_IMPL(__imp__sub_8291E620);  // WorldUpdate(scene, timeMs)
PPC_FUNC_IMPL(__imp__sub_82908068);  // EntityFinalize(result)
PPC_FUNC_IMPL(__imp__sub_8291F270);  // FlushStreaming() (thunk -> sub_8218F7C0)
PPC_FUNC_IMPL(__imp__sub_8291EF18);  // PreDestroyEntity(entity) (frees tracked entity)
PPC_FUNC_IMPL(__imp__sub_8290F6B0);  // PostCleanup(data) (flushes streaming mgr queues)
PPC_FUNC_IMPL(__imp__sub_8290C0F8);  // LockStreamingMutex(mgr)
PPC_FUNC_IMPL(__imp__sub_8285D0D8);  // InitScopedLock(lock)
PPC_FUNC_IMPL(__imp__sub_8290C128);  // UnlockStreamingMutex(mgr)
PPC_FUNC_IMPL(__imp__sub_82904680);  // UpdateFloatPool(pool)
PPC_FUNC_IMPL(__imp__sub_829225E0);  // SectorTraverse(bounds, sectorIdx, callback, userData)
PPC_FUNC_IMPL(__imp__sub_82921F30);  // SectorCommit(bounds, sectorIdx)
PPC_FUNC_IMPL(__imp__sub_82922998);  // SectorProcess(bounds, userData, sectorIdx)
PPC_FUNC_IMPL(__imp__sub_82922070);  // SectorFinalize(bounds, sectorIdx)
// Note: sub_829180A0 (0x829180A0) is a callback function pointer passed to
// SectorTraverse -- not called directly from here, just passed as an address.

// =============================================================================
// sub_829222E0 - Compute sector index from entity address
// =============================================================================
// Formula: (entity_addr - sector_base - sector_stride*sectorIdx - header) / entity_size
// Pseudocode: (-14560 * a2 - base_ptr + a3 - 96) / 112
//
// a1 = streaming bounds, a2 = sector index, a3 = entity address
// returns: entity index within the sector

PPC_FUNC_IMPL(__imp__sub_829222E0);
PPC_FUNC_HOOK(sub_829222E0)
{
    uint32_t boundsPtr = ctx.r3.u32;
    int32_t sectorIdx  = static_cast<int32_t>(ctx.r4.u32);
    int32_t entityAddr = static_cast<int32_t>(ctx.r5.u32);

    int32_t basePtr = static_cast<int32_t>(PPC_LOAD_U32(boundsPtr + 232));

    // (-SECTOR_STRIDE * sectorIdx - basePtr + entityAddr - SECTOR_HEADER_SIZE) / ENTITY_SIZE
    int32_t numerator = -static_cast<int32_t>(SECTOR_STRIDE) * sectorIdx
                        - basePtr + entityAddr
                        - static_cast<int32_t>(SECTOR_HEADER_SIZE);

    // Signed division by 112 (truncated toward zero, matching PPC srawi+sign-fixup)
    int32_t result = numerator / static_cast<int32_t>(ENTITY_SIZE);

    ctx.r3.s64 = result;
}

// =============================================================================
// sub_82921E60 - Mark sector cell dirty
// =============================================================================
// Sets bit 2 (DIRTY_FLAG) on the byte at:
//   base_ptr + SECTOR_STRIDE * sectorIdx + cellOffset
//
// a1 = streaming bounds, a2 = sector index, a3 = cell byte offset

PPC_FUNC_IMPL(__imp__sub_82921E60);
PPC_FUNC_HOOK(sub_82921E60)
{
    uint32_t boundsPtr  = ctx.r3.u32;
    uint32_t sectorIdx  = ctx.r4.u32;
    uint32_t cellOffset = ctx.r5.u32;

    uint32_t basePtr = PPC_LOAD_U32(boundsPtr + 232);
    uint32_t addr = basePtr + SECTOR_STRIDE * sectorIdx + cellOffset;

    uint8_t val = PPC_LOAD_U8(addr);
    val |= DIRTY_FLAG;
    PPC_STORE_U8(addr, val);

    // Return value is the bounds pointer (r3 unchanged in original)
    ctx.r3.u32 = boundsPtr;
}

// =============================================================================
// sub_829022E8 - Pre-destroy entity (thunk)
// =============================================================================
// Thunk to sub_8291EF18 which frees an entity's tracked allocation

PPC_FUNC_IMPL(__imp__sub_829022E8);
PPC_FUNC_HOOK(sub_829022E8)
{
    // Direct thunk - just forward to the real implementation
    __imp__sub_8291EF18(ctx, base);
}

// =============================================================================
// sub_82912B48 - Post-cleanup (thunk)
// =============================================================================
// Thunk to sub_8290F6B0 which flushes streaming manager queues

PPC_FUNC_IMPL(__imp__sub_82912B48);
PPC_FUNC_HOOK(sub_82912B48)
{
    // Direct thunk - forward to sub_8290F6B0
    __imp__sub_8290F6B0(ctx, base);
}

// =============================================================================
// sub_829158F0 - Entity tick loop (24 iterations)
// =============================================================================
// Iterates over 24 subsystem slots. For each non-null entry, dispatches
// vtable[2] (offset +8) as the tick function.
//
// a1 = pointer to array of 24 entity pointers (at scene+172)
// a2 = current time (unused in the loop body, passed via r4)

PPC_FUNC_IMPL(__imp__sub_829158F0);
PPC_FUNC_HOOK(sub_829158F0)
{
    uint32_t listBase = ctx.r3.u32;

    // Check the global tick-enabled flag
    uint8_t tickEnabled = PPC_LOAD_U8(ADDR_TICK_ENABLED);
    if (!tickEnabled) {
        return;
    }

    for (uint32_t i = 0; i < MAX_TICK_SLOTS; i++) {
        uint32_t entityPtr = PPC_LOAD_U32(listBase + i * 4);
        if (entityPtr == 0) {
            continue;
        }

        // Null-vtable safety: read vtable pointer, skip if null
        uint32_t vtable = PPC_LOAD_U32(entityPtr);
        if (vtable == 0) {
            continue;
        }

        // Dispatch vtable[2] (offset +8) = tick function
        uint32_t tickFn = PPC_LOAD_U32(vtable + 8);
        if (tickFn == 0) {
            continue;
        }

        // Call: entity->Tick(entity)
        ctx.r3.u32 = entityPtr;
        ctx.lr = 0;
        PPC_CALL_INDIRECT_FUNC(tickFn);
    }
}

// =============================================================================
// sub_82915840 - Cleanup: destroy entities + clear metadata + post-cleanup
// =============================================================================
// a1 = entity container (_DWORD* with 24 entity slots + count + metadata)
//
// For each active entity (up to a1[24] count):
//   1. Call sub_829022E8 (pre-destroy)
//   2. Dispatch vtable[0] (destructor) with arg2=1 (destroy+free)
//   3. Null out the slot
// Then zero 9 metadata dwords (a1[25..33])
// Finally call sub_82912B48 (post-cleanup) on a1[34]

PPC_FUNC_IMPL(__imp__sub_82915840);
PPC_FUNC_HOOK(sub_82915840)
{
    uint32_t containerBase = ctx.r3.u32;

    uint32_t entityCount = PPC_LOAD_U32(containerBase + CONTAINER_COUNT_IDX * 4);

    for (uint32_t i = 0; i < entityCount; i++) {
        uint32_t slotAddr = containerBase + i * 4;
        uint32_t entityPtr = PPC_LOAD_U32(slotAddr);

        if (entityPtr == 0) {
            continue;
        }

        // Pre-destroy: call sub_829022E8 (thunk -> sub_8291EF18)
        // sub_8291EF18 takes the entity in r3, checks field at +32
        ctx.r3.u32 = entityPtr;
        __imp__sub_8291EF18(ctx, base);

        // Re-read entity pointer (pre-destroy may have modified state)
        entityPtr = PPC_LOAD_U32(slotAddr);
        if (entityPtr != 0) {
            // Null-vtable safety
            uint32_t vtable = PPC_LOAD_U32(entityPtr);
            if (vtable != 0) {
                uint32_t dtorFn = PPC_LOAD_U32(vtable);
                if (dtorFn != 0) {
                    // Call: entity->Destroy(entity, 1)
                    ctx.r3.u32 = entityPtr;
                    ctx.r4.s64 = 1;
                    ctx.lr = 0;
                    PPC_CALL_INDIRECT_FUNC(dtorFn);
                }
            }

            // Null out the slot
            PPC_STORE_U32(slotAddr, 0);
        }
    }

    // Zero 9 metadata dwords at a1[25..33]
    for (uint32_t i = 0; i < CONTAINER_META_COUNT; i++) {
        PPC_STORE_U32(containerBase + (CONTAINER_META_START + i) * 4, 0);
    }

    // Post-cleanup: call sub_82912B48 on a1[34] (thunk -> sub_8290F6B0)
    ctx.r3.u32 = containerBase + CONTAINER_POST_IDX * 4;
    __imp__sub_8290F6B0(ctx, base);
}

// =============================================================================
// sub_829022B0 - Bridge: world update + entity finalize
// =============================================================================
// Calls sub_8291E620(scene_global, timeMs) then sub_82908068(result)

PPC_FUNC_IMPL(__imp__sub_829022B0);
PPC_FUNC_HOOK(sub_829022B0)
{
    uint32_t timeMs = ctx.r3.u32;

    // WorldUpdate(scene_global, timeMs)
    ctx.r3.u32 = ADDR_SCENE;
    ctx.r4.u32 = timeMs;
    __imp__sub_8291E620(ctx, base);

    // EntityFinalize(result from WorldUpdate)
    // r3 already contains the return value from WorldUpdate
    __imp__sub_82908068(ctx, base);
}

// =============================================================================
// sub_82902328 - Finalize frame: flush streaming + increment frame counter
// =============================================================================

PPC_FUNC_IMPL(__imp__sub_82902328);
PPC_FUNC_HOOK(sub_82902328)
{
    // Flush pending streaming operations
    __imp__sub_8291F270(ctx, base);

    // Increment global frame counter
    uint32_t frameCount = PPC_LOAD_U32(ADDR_FRAME_COUNTER);
    frameCount++;
    PPC_STORE_U32(ADDR_FRAME_COUNTER, frameCount);
}

// =============================================================================
// sub_8291FA80 - Streaming subsystem update (at scene+380)
// =============================================================================
// Updates streaming position interpolation for 3 axes (X, Y, Z),
// manages triple-buffered sector updates, and processes sector traversal.
//
// a1 = subsystem pointer (scene + 380)
// a2 = current time in ms

PPC_FUNC_IMPL(__imp__sub_8291FA80);
PPC_FUNC_HOOK(sub_8291FA80)
{
    uint32_t subsys = ctx.r3.u32;
    uint32_t currentTime = ctx.r4.u32;

    // --- Lock streaming mutex, init scoped lock, unlock ---
    ctx.r3.u32 = ADDR_STREAMING_MGR;
    __imp__sub_8290C0F8(ctx, base);

    // Scoped lock init (stack-based in original, we just call it)
    {
        // The original allocates a 128-byte lock on stack. We use a temporary
        // area in guest memory via the PPC stack.
        uint32_t savedSP = ctx.r1.u32;
        ctx.r1.u32 -= 256;  // allocate stack space
        PPC_STORE_U32(ctx.r1.u32, savedSP);  // save backchain

        ctx.r3.u32 = ctx.r1.u32 + 128;
        __imp__sub_8285D0D8(ctx, base);

        ctx.r1.u32 = savedSP;  // restore stack
    }

    ctx.r3.u32 = ADDR_STREAMING_MGR;
    __imp__sub_8290C128(ctx, base);

    // Update float pool
    ctx.r3.u32 = ADDR_FLOAT_POOL;
    __imp__sub_82904680(ctx, base);

    // --- Triple-buffer index rotation: (current + 2) % 3 ---
    uint32_t bufferIdx = PPC_LOAD_U32(ADDR_BUFFER_INDEX);
    uint32_t nextIdx = (bufferIdx + 2) % 3;
    PPC_STORE_U32(ADDR_BUFFER_INDEX_NEXT, nextIdx);

    // --- Compute delta time ---
    uint32_t lastTime = PPC_LOAD_U32(subsys + SS_LAST_TIME);
    uint32_t deltaMs = currentTime - lastTime;
    float deltaSec = static_cast<float>(deltaMs) * 0.001f;

    // --- Interpolate X axis ---
    PPCRegister tmpF;
    tmpF.u32 = PPC_LOAD_U32(subsys + SS_POS_X);
    float posX = tmpF.f32;
    tmpF.u32 = PPC_LOAD_U32(subsys + SS_RATE_X);
    float rateX = tmpF.f32;
    uint8_t pausedX = PPC_LOAD_U8(subsys + SS_FLAGS_X);
    if (!pausedX) {
        posX = rateX * deltaSec + posX;
    }

    // --- Interpolate Y axis ---
    tmpF.u32 = PPC_LOAD_U32(subsys + SS_POS_Y);
    float posY = tmpF.f32;
    tmpF.u32 = PPC_LOAD_U32(subsys + SS_RATE_Y);
    float rateY = tmpF.f32;
    uint8_t pausedY = PPC_LOAD_U8(subsys + SS_FLAGS_Y);
    if (!pausedY) {
        posY = rateY * deltaSec + posY;
    }

    // --- Interpolate Z axis ---
    tmpF.u32 = PPC_LOAD_U32(subsys + SS_POS_Z);
    float posZ = tmpF.f32;
    tmpF.u32 = PPC_LOAD_U32(subsys + SS_RATE_Z);
    float rateZ = tmpF.f32;
    uint8_t pausedZ = PPC_LOAD_U8(subsys + SS_FLAGS_Z);
    if (!pausedZ) {
        posZ = rateZ * deltaSec + posZ;
    }

    // --- Save timestamp and clear stats ---
    PPC_STORE_U32(subsys + SS_LAST_TIME, currentTime);
    PPC_STORE_U32(subsys + SS_STAT_A, 0);
    PPC_STORE_U32(subsys + SS_STAT_B, 0);
    PPC_STORE_U32(subsys + SS_STAT_C, 0);
    PPC_STORE_U32(subsys + SS_STAT_D, 0);

    // --- Build stack-based parameter block for sector callbacks ---
    // The original places {currentTime, posX, rateX, pausedX, posY, ...} on stack
    // We pass it via the PPC stack as the sector functions expect it there
    uint32_t savedSP = ctx.r1.u32;
    ctx.r1.u32 -= 336;
    PPC_STORE_U32(ctx.r1.u32, savedSP);

    // Store the parameter block at sp+80 (matching original stack layout)
    uint32_t paramBase = ctx.r1.u32 + 80;
    PPC_STORE_U32(paramBase + 0, currentTime);

    // Store interpolated position data in the param block
    // Layout matches the local vars v14-v23 in the pseudocode
    {
        PPCRegister conv;

        conv.f32 = posX;   PPC_STORE_U32(paramBase + 4, conv.u32);   // v15 = posX
        conv.f32 = rateX;  PPC_STORE_U32(paramBase + 8, conv.u32);   // v16 = rateX
        PPC_STORE_U32(paramBase + 12, PPC_LOAD_U32(subsys + SS_FLAGS_X));  // v17

        conv.f32 = posY;   PPC_STORE_U32(paramBase + 16, conv.u32);  // v18 = posY
        conv.f32 = rateY;  PPC_STORE_U32(paramBase + 20, conv.u32);  // v19 = rateY
        PPC_STORE_U32(paramBase + 24, PPC_LOAD_U32(subsys + SS_FLAGS_Y));  // v20

        conv.f32 = posZ;   PPC_STORE_U32(paramBase + 28, conv.u32);  // v21 = posZ
        conv.f32 = rateZ;  PPC_STORE_U32(paramBase + 32, conv.u32);  // v22 = rateZ
        PPC_STORE_U32(paramBase + 36, PPC_LOAD_U32(subsys + SS_FLAGS_Z));  // v23
    }

    // --- Process each sector ---
    uint32_t sectorCount = PPC_LOAD_U32(ADDR_SECTOR_COUNT);

    for (uint32_t sectorIdx = 0; sectorIdx < sectorCount; sectorIdx++) {
        // Reset 64 cells in sector: set flagA=0xFF, flagB=0xFF, active=0
        uint32_t boundsBase = ADDR_STREAMING_BOUNDS;
        for (uint32_t cell = 0; cell < SECTOR_CELL_COUNT; cell++) {
            uint32_t cellAddr = boundsBase + cell * SECTOR_CELL_SIZE;
            PPC_STORE_U8(cellAddr + 0, 0xFF);  // flagA
            PPC_STORE_U8(cellAddr + 1, 0xFF);  // flagB
            PPC_STORE_U8(cellAddr + 2, 0x00);  // active
        }
        // Clear byte at bounds+192 (byte_831C8AF8 equivalent relative to bounds)
        PPC_STORE_U8(boundsBase + 192, 0);

        // SectorTraverse(bounds, sectorIdx, callback, &paramBlock)
        ctx.r3.u32 = boundsBase;
        ctx.r4.u32 = sectorIdx;
        ctx.r5.u32 = 0x829180A0;  // callback function address
        ctx.r6.u32 = paramBase;
        __imp__sub_829225E0(ctx, base);

        // SectorCommit(bounds, sectorIdx)
        ctx.r3.u32 = boundsBase;
        ctx.r4.u32 = sectorIdx;
        __imp__sub_82921F30(ctx, base);

        // SectorProcess(bounds, &paramBlock, sectorIdx)
        ctx.r3.u32 = boundsBase;
        ctx.r4.u32 = paramBase;
        ctx.r5.u32 = sectorIdx;
        __imp__sub_82922998(ctx, base);

        // SectorFinalize(bounds, sectorIdx)
        ctx.r3.u32 = boundsBase;
        ctx.r4.u32 = sectorIdx;
        __imp__sub_82922070(ctx, base);
    }

    // Restore stack
    ctx.r1.u32 = savedSP;

    // --- Write back interpolated positions + pause flags ---
    {
        PPCRegister conv;

        conv.f32 = posX;
        PPC_STORE_U32(subsys + SS_POS_X, conv.u32);
        PPC_STORE_U8(subsys + SS_FLAGS_X + 1, pausedX);  // +201 relative to subsys

        conv.f32 = posY;
        PPC_STORE_U32(subsys + SS_POS_Y, conv.u32);
        PPC_STORE_U8(subsys + SS_FLAGS_Y + 1, PPC_LOAD_U8(subsys + SS_FLAGS_Y));

        conv.f32 = posZ;
        PPC_STORE_U32(subsys + SS_POS_Z, conv.u32);
        PPC_STORE_U8(subsys + SS_FLAGS_Z + 1, PPC_LOAD_U8(subsys + SS_FLAGS_Z));
    }

    // Swap dirty flags: byte_831C9414 = byte_831C9415
    uint8_t dirtyB = PPC_LOAD_U8(ADDR_DIRTY_SWAP_B);
    PPC_STORE_U8(ADDR_DIRTY_SWAP_A, dirtyB);
}

// =============================================================================
// sub_82902420 - Scene tick (top-level)
// =============================================================================
// Called once per frame. Manages audio frame timing, streaming capacity,
// streaming subsystem update, entity tick loop, and frame bridging.
//
// a1 = scene tick object ("this")

PPC_FUNC_IMPL(__imp__sub_82902420);
PPC_FUNC_HOOK(sub_82902420)
{
    uint32_t thisPtr = ctx.r3.u32;

    // --- Start audio frame timer ---
    // Timer is a 40-byte stack object
    uint32_t savedSP = ctx.r1.u32;
    ctx.r1.u32 -= 160;
    PPC_STORE_U32(ctx.r1.u32, savedSP);
    uint32_t timerAddr = ctx.r1.u32 + 96;

    // Timer::Init(timer, "audioFrameTimer")
    ctx.r3.u32 = timerAddr;
    ctx.r4.u32 = 0x8209B42C;  // "audioFrameTimer" string
    __imp__sub_8286A370(ctx, base);

    // Timer::Start(timer)
    ctx.r3.u32 = timerAddr;
    __imp__sub_8286A2E8(ctx, base);

    // GetAudioFrameTime(timerResult)
    __imp__sub_8290ED60(ctx, base);
    // Store qword result at this+792
    PPC_STORE_U64(thisPtr + OFF_AUDIO_FRAME_TIME, ctx.r3.u64);

    // GetCurrentTimeMs()
    __imp__sub_8290EDB8(ctx, base);
    uint32_t currentTimeMs = ctx.r3.u32;
    PPC_STORE_U32(thisPtr + OFF_CURRENT_TIME, currentTimeMs);

    // --- Streaming capacity management ---
    uint8_t flagA = PPC_LOAD_U8(thisPtr + OFF_FLAG_A);
    uint8_t flagB = PPC_LOAD_U8(thisPtr + OFF_FLAG_B);

    if (flagA) {
        // flagA is set: entering constrained mode
        if (!flagB) {
            // Save current allocator count, then set visible capacity to 0
            ctx.r3.u32 = ADDR_SCENE;
            __imp__sub_82A58008(ctx, base);
            PPC_STORE_U32(thisPtr + OFF_SAVED_CAPACITY, ctx.r3.u32);

            ctx.r3.u32 = ADDR_SCENE;
            ctx.r4.u32 = 0;
            __imp__sub_8291DED8(ctx, base);

            PPC_STORE_U8(thisPtr + OFF_FLAG_B, 1);
        }
    } else {
        // flagA is clear
        if (flagB) {
            // Restore saved capacity
            uint32_t savedCapacity = PPC_LOAD_U32(thisPtr + OFF_SAVED_CAPACITY);
            ctx.r3.u32 = ADDR_SCENE;
            ctx.r4.u32 = savedCapacity;
            __imp__sub_8291DED8(ctx, base);

            PPC_STORE_U8(thisPtr + OFF_FLAG_B, 0);
        }
    }

    // --- Streaming subsystem update (this+380) ---
    ctx.r3.u32 = thisPtr + OFF_STREAMING_SUB;
    ctx.r4.u32 = currentTimeMs;
    // Call our hook directly (it re-enters via __imp__)
    sub_8291FA80(ctx, base);

    // --- Entity tick loop (this+172, 24 subsystem slots) ---
    ctx.r3.u32 = thisPtr + OFF_SUBSYSTEM_LIST;
    ctx.r4.u32 = currentTimeMs;
    sub_829158F0(ctx, base);

    // --- Bridge: world update + entity finalize ---
    ctx.r3.u32 = currentTimeMs;
    sub_829022B0(ctx, base);

    // --- Stop audio frame timer ---
    ctx.r3.u32 = timerAddr;
    __imp__sub_8286A308(ctx, base);

    // --- FPS cap sleep (if flag is set) ---
    uint8_t fpsCapFlag = PPC_LOAD_U8(ADDR_FPS_CAP_FLAG);
    if (fpsCapFlag) {
        // Get elapsed time
        ctx.r3.u32 = timerAddr;
        __imp__sub_8286A348(ctx, base);
        float elapsed = static_cast<float>(ctx.f1.f64);

        // Compute sleep: clamp(15.0 - elapsed, 0.0, 15.0)
        float remaining = 15.0f - elapsed;
        if (remaining < 0.0f) {
            remaining = 0.0f;
        }
        if (remaining > 15.0f) {
            remaining = 15.0f;
        }

        int32_t sleepMs = static_cast<int32_t>(remaining);
        ctx.r3.s64 = sleepMs;
        __imp__sub_82849918(ctx, base);
    }

    // Restore stack
    ctx.r1.u32 = savedSP;
}
