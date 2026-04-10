// =============================================================================
// game_fixes.cpp — Targeted hooks for runtime crash issues
// =============================================================================
//
// Fix 1: sub_8291DF00 — CPool entity iterator / streaming sector processor
//        FULL REPLACEMENT with null-vtable guards before every vtable dispatch.
//        The original recompiled code dereferences entity vtable pointers without
//        null checks.  During pool churn (entities being created/destroyed across
//        threads), a slot can have its vtable pointer zeroed between the flag
//        check and the vtable dispatch, causing reads from address 0.
//
//        The previous pre-scan approach (mark null-vtable slots as free before
//        calling the original) was vulnerable to TOCTOU races — another thread
//        could zero a vtable between the scan and the dispatch.  This full
//        replacement checks the vtable pointer immediately before each dispatch.
//
// Fix 2: sub_82849918, sub_82849910, sub_82A12B60, sub_82A7A070
//        Sleep thunk replacements — ALREADY IMPLEMENTED in imports.cpp.
//
// Fix 3: sub_821910D0, sub_82191228, sub_82212EC0 — Audio render thread subsystem
//        ALREADY IMPLEMENTED in imports.cpp (complete decompilation with endpoint guard).
// =============================================================================

#include "function.h"
#include "memory.h"

#include <atomic>
#include <chrono>
#include <cstdio>
#include <thread>

#include <cpu/ppc_context.h>

// =============================================================================
// FIX 1: sub_8291DF00 — Complete Replacement with Vtable Guards
// =============================================================================
//
// Globals (all in guest memory):
//   0x831C8EC0  pointer to CPool struct
//   0x831C8EC9  lock byte (spin-wait while nonzero)
//   0x831C8A38  streaming bounds manager object (static)
//   *(0x831C8A38 + 232) = sector array base pointer
//
// CPool layout:
//   +0x00  items base pointer (array of 768-byte entities)
//   +0x04  flags array pointer (one byte per slot; bit 7 = free)
//   +0x28  slot count (uint32)
//
// Entity layout (768-byte stride):
//   +0x00  vtable pointer (guest address to vtable array)
//   +0x04  entityInfo pointer (guest address)
//   +0x88  byte flags (bit 3 used for LOD gate)
//
// EntityInfo layout:
//   +0x18  flags0  bit0=needsRemove, bit1=needsLODUpdate,
//                  bit2=forceUpdate,  bit3=needsDistUpdate
//   +0x19  flags1  bit7=distanceValid
//   +0x6D  state byte (0xFF = inactive)
//   +0x6E  model type index
//
// Sector entry (32-byte stride):
//   +0x08  packed distance (>> 3 for value, & 7 for lower bits)
//   +0x0C  visibility byte (bit 7)
//   +0x10  status flags (top 2 bits: 10b = 0x80000000 = marked for delete)
//
// Vtable slots dispatched (offset / index):
//   +0   [0]  destructor(this, flag)
//   +8   [2]  cleanup(this)
//   +12  [3]  updateLOD(this, distance)
//   +16  [4]  remove(this)
//   +20  [5]  isActive(this) -> u8
//   +24  [6]  getVisibility(this) -> u8
//   +28  [7]  updateDistance(this, distance)
//   +36  [9]  getDistance(this) -> int
//   +40  [10] process(this)
// =============================================================================

static constexpr uint32_t ENTITY_POOL_GLOBAL = 0x831C8EC0;
static constexpr uint32_t POOL_LOCK_BYTE     = 0x831C8EC9;
static constexpr uint32_t STREAM_BOUNDS_ADDR = 0x831C8A38;
static constexpr uint32_t ENTITY_STRIDE      = 768;

// Forward-declare the generated implementations we call
extern "C" void __imp__sub_8291DF00(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_829222E0(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82921E60(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82902328(PPCContext& ctx, uint8_t* base);

// ---------------------------------------------------------------------------
// SafeVCall — Look up a guest function address in the recomp table and call it.
// Returns true if the function was found and called, false otherwise.
// ---------------------------------------------------------------------------
static bool SafeVCall(PPCContext& ctx, uint8_t* base, uint32_t funcAddr) {
    if (funcAddr == 0)
        return false;

    bool inRange = (funcAddr >= uint32_t(PPC_CODE_BASE) &&
                    funcAddr <  uint32_t(PPC_CODE_BASE) + uint32_t(PPC_CODE_SIZE));
    PPCFunc* fn = inRange ? PPC_LOOKUP_FUNC(base, funcAddr) : nullptr;
    if (fn) {
        fn(ctx, base);
        return true;
    }

    // Not in table — this is the MISSING-FUNC case, but we don't crash
    static std::atomic<int> s_missingCount{0};
    int n = s_missingCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 20 || (n % 500) == 0) {
        fprintf(stderr, "[POOL-VCALL] missing func 0x%08X (in_range=%d) #%d\n",
                funcAddr, (int)inRange, n);
        fflush(stderr);
    }
    return false;
}

static std::atomic<int> s_poolIterSkips{0};

PPC_FUNC_HOOK(sub_8291DF00) {
    // r3 = 'this' pointer (result struct; processCount stored at offset +16)
    uint32_t resultPtr = ctx.r3.u32;
    int processCount = 0;

    // --- Read the CPool pointer ---
    uint32_t poolPtr = PPC_LOAD_U32(ENTITY_POOL_GLOBAL);
    if (poolPtr == 0) {
        PPC_STORE_U32(resultPtr + 16, 0);
        ctx.r3.u32 = resultPtr;
        __imp__sub_82902328(ctx, base);
        return;
    }

    uint32_t poolSize = PPC_LOAD_U32(poolPtr + 40);

    // --- Spin-wait on lock byte ---
    while (PPC_LOAD_U8(POOL_LOCK_BYTE) != 0) {
        std::this_thread::yield();
    }

    if (poolSize == 0) {
        PPC_STORE_U32(resultPtr + 16, 0);
        ctx.r3.u32 = resultPtr;
        __imp__sub_82902328(ctx, base);
        return;
    }

    // Streaming bounds manager
    uint32_t sectorArrayBase = PPC_LOAD_U32(STREAM_BOUNDS_ADDR + 232);

    // --- Main pool iteration ---
    for (uint32_t slot = 0, itemOffset = 0; slot < poolSize; ++slot, itemOffset += ENTITY_STRIDE) {

        // Re-read pool each iteration (volatile global)
        poolPtr = PPC_LOAD_U32(ENTITY_POOL_GLOBAL);
        uint32_t flagsArray = PPC_LOAD_U32(poolPtr + 4);
        uint8_t slotFlags = PPC_LOAD_U8(flagsArray + slot);

        // Skip free slots (bit 7 = unused)
        if (slotFlags & 0x80)
            continue;

        uint32_t itemsBase = PPC_LOAD_U32(poolPtr + 0);
        uint32_t entity = itemsBase + itemOffset;
        if (entity == 0)
            continue;

        // ---- THE CRITICAL NULL-VTABLE CHECK ----
        uint32_t vtablePtr = PPC_LOAD_U32(entity + 0);
        uint32_t entityInfoPtr = PPC_LOAD_U32(entity + 4);

        if (vtablePtr == 0 || entityInfoPtr == 0) {
            int n = s_poolIterSkips.fetch_add(1, std::memory_order_relaxed);
            if (n < 50 || (n % 1000) == 0) {
                fprintf(stderr, "[POOL-ITER] skip slot %u: vtable=0x%08X entityInfo=0x%08X\n",
                        slot, vtablePtr, entityInfoPtr);
                fflush(stderr);
            }
            continue;
        }

        // Entity info fields
        uint8_t modelType = PPC_LOAD_U8(entityInfoPtr + 110);

        // Compute sector entry address:
        // colIndex = sub_829222E0(streamBounds, modelType, entityInfoPtr)
        ctx.r3.u32 = STREAM_BOUNDS_ADDR;
        ctx.r4.u32 = (uint32_t)modelType;
        ctx.r5.u32 = entityInfoPtr;
        __imp__sub_829222E0(ctx, base);
        int32_t colIndex = (int32_t)ctx.r3.u32;

        // sectorIndex = 455 * modelType + colIndex + 339
        int32_t sectorIndex = 455 * (int32_t)modelType + colIndex + 339;
        // sectorEntry = (sectorIndex << 5) + sectorArrayBase
        uint32_t sectorEntry = (uint32_t)(sectorIndex << 5) + sectorArrayBase;

        // ---- Sector status check ----
        uint32_t sectorStatus = PPC_LOAD_U32(sectorEntry + 16);
        uint32_t topBits = sectorStatus & 0xC0000000u;

        if (topBits == 0x80000000u) {
            // --- DELETE PATH: sector marked for removal ---
            // isActive (vtable[5], offset +20)
            vtablePtr = PPC_LOAD_U32(entity + 0);
            if (vtablePtr == 0) goto skip_entity;
            uint32_t vfIsActive = PPC_LOAD_U32(vtablePtr + 20);
            if (vfIsActive == 0) goto skip_entity;

            ctx.r3.u32 = entity;
            if (!SafeVCall(ctx, base, vfIsActive)) goto skip_entity;
            uint8_t isActive = ctx.r3.u8;

            if (isActive == 0) {
                // Entity is not active — destroy it
                PPC_STORE_U8(entityInfoPtr + 109, 0xFF);

                // cleanup (vtable[2], offset +8)
                vtablePtr = PPC_LOAD_U32(entity + 0);
                if (vtablePtr == 0) goto skip_entity;
                uint32_t vfCleanup = PPC_LOAD_U32(vtablePtr + 8);
                if (vfCleanup == 0) goto skip_entity;
                ctx.r3.u32 = entity;
                if (!SafeVCall(ctx, base, vfCleanup)) goto skip_entity;

                // destructor (vtable[0], offset +0) with flag=1
                vtablePtr = PPC_LOAD_U32(entity + 0);
                if (vtablePtr == 0) goto skip_entity;
                uint32_t vfDtor = PPC_LOAD_U32(vtablePtr + 0);
                if (vfDtor == 0) goto skip_entity;
                ctx.r3.u32 = entity;
                ctx.r4.s64 = 1;
                if (!SafeVCall(ctx, base, vfDtor)) goto skip_entity;

                // Clear top 2 status bits
                sectorStatus = PPC_LOAD_U32(sectorEntry + 16);
                PPC_STORE_U32(sectorEntry + 16, sectorStatus & 0x3FFFFFFFu);

                // If flags0 bit 0 → mark sector dirty and continue
                uint8_t flags0 = PPC_LOAD_U8(entityInfoPtr + 24);
                if (flags0 & 0x01) {
                    ctx.r3.u32 = STREAM_BOUNDS_ADDR;
                    ctx.r4.u32 = (uint32_t)modelType;
                    ctx.r5.u32 = (uint32_t)colIndex;
                    __imp__sub_82921E60(ctx, base);
                }
                continue;
            }
            // isActive returned nonzero — fall through to normal processing
        }

        // --- NORMAL PROCESSING PATH ---
        ++processCount;

        uint8_t flags0 = PPC_LOAD_U8(entityInfoPtr + 24);

        // ---- Distance update (flags0 bit 3) ----
        if (flags0 & 0x08) {
            uint8_t flags1 = PPC_LOAD_U8(entityInfoPtr + 25);
            int32_t distance;
            if (flags1 & 0x80) {
                distance = (int32_t)PPC_LOAD_U32(sectorEntry + 8) >> 3;
            } else {
                distance = 0;
            }

            // updateDistance (vtable[7], offset +28)
            vtablePtr = PPC_LOAD_U32(entity + 0);
            if (vtablePtr == 0) goto skip_entity;
            uint32_t vfUpdateDist = PPC_LOAD_U32(vtablePtr + 28);
            if (vfUpdateDist == 0) goto skip_entity;
            ctx.r3.u32 = entity;
            ctx.r4.s64 = distance;
            SafeVCall(ctx, base, vfUpdateDist);

            // Clear distance update flags
            flags0 = PPC_LOAD_U8(entityInfoPtr + 24);
            flags1 = PPC_LOAD_U8(entityInfoPtr + 25);
            PPC_STORE_U8(entityInfoPtr + 25, flags1 & 0x7F);
            PPC_STORE_U8(entityInfoPtr + 24, flags0 & 0xF7);
        }

        flags0 = PPC_LOAD_U8(entityInfoPtr + 24);

        // ---- LOD update (flags0 bit 1) ----
        if (flags0 & 0x02) {
            uint8_t entityByte136 = PPC_LOAD_U8(entity + 136);
            // Gate: entity[136] bit 3 clear, OR flags0 bit 2 set
            if (!(entityByte136 & 0x08) || (flags0 & 0x04)) {
                int32_t distance = (int32_t)PPC_LOAD_U32(sectorEntry + 8) >> 3;
                if (distance != -1) {
                    // updateLOD (vtable[3], offset +12)
                    vtablePtr = PPC_LOAD_U32(entity + 0);
                    if (vtablePtr != 0) {
                        uint32_t vfUpdateLOD = PPC_LOAD_U32(vtablePtr + 12);
                        if (vfUpdateLOD != 0) {
                            ctx.r3.u32 = entity;
                            ctx.r4.s64 = distance;
                            SafeVCall(ctx, base, vfUpdateLOD);
                        }
                    }
                }
                // Clear bit 1
                flags0 = PPC_LOAD_U8(entityInfoPtr + 24);
                PPC_STORE_U8(entityInfoPtr + 24, flags0 & 0xFD);
            }
        }

        flags0 = PPC_LOAD_U8(entityInfoPtr + 24);

        // ---- Removal request (flags0 bit 0) ----
        if (flags0 & 0x01) {
            // Clear bit 1 first
            PPC_STORE_U8(entityInfoPtr + 24, flags0 & 0xFD);

            // remove (vtable[4], offset +16)
            vtablePtr = PPC_LOAD_U32(entity + 0);
            if (vtablePtr != 0) {
                uint32_t vfRemove = PPC_LOAD_U32(vtablePtr + 16);
                if (vfRemove != 0) {
                    ctx.r3.u32 = entity;
                    SafeVCall(ctx, base, vfRemove);
                }
            }
        }

        // ---- Process / update (vtable[10], offset +40) ----
        vtablePtr = PPC_LOAD_U32(entity + 0);
        if (vtablePtr == 0) goto skip_entity;
        {
            uint32_t vfProcess = PPC_LOAD_U32(vtablePtr + 40);
            if (vfProcess == 0) goto skip_entity;
            ctx.r3.u32 = entity;
            if (!SafeVCall(ctx, base, vfProcess)) goto skip_entity;
        }

        // ---- Update sector cache: distance ----
        sectorStatus = PPC_LOAD_U32(sectorEntry + 16);
        {
            uint32_t statusTop2 = sectorStatus >> 30;
            if (statusTop2 == 1 || statusTop2 == 3) {
                uint8_t entityByte136 = PPC_LOAD_U8(entity + 136);
                flags0 = PPC_LOAD_U8(entityInfoPtr + 24);
                if (!(entityByte136 & 0x08) || (flags0 & 0x04)) {
                    // getDistance (vtable[9], offset +36)
                    vtablePtr = PPC_LOAD_U32(entity + 0);
                    if (vtablePtr != 0) {
                        uint32_t vfGetDist = PPC_LOAD_U32(vtablePtr + 36);
                        if (vfGetDist != 0) {
                            ctx.r3.u32 = entity;
                            SafeVCall(ctx, base, vfGetDist);
                            int32_t dist = (int32_t)ctx.r3.u32;
                            uint32_t oldDistField = PPC_LOAD_U32(sectorEntry + 8);
                            PPC_STORE_U32(sectorEntry + 8,
                                          (uint32_t)(dist << 3) | (oldDistField & 7));
                        }
                    }
                }
            }
        }

        // ---- Update sector cache: visibility ----
        vtablePtr = PPC_LOAD_U32(entity + 0);
        if (vtablePtr != 0) {
            uint32_t vfGetVis = PPC_LOAD_U32(vtablePtr + 24);
            if (vfGetVis != 0) {
                ctx.r3.u32 = entity;
                SafeVCall(ctx, base, vfGetVis);
                uint8_t visibility = ctx.r3.u8;
                uint8_t oldVisByte = PPC_LOAD_U8(sectorEntry + 12);
                PPC_STORE_U8(sectorEntry + 12,
                             (uint8_t)((visibility << 7) | (oldVisByte & 0x7F)));
            }
        }

        // ---- Final removal check: if bit 0 set AND !isActive → destroy ----
        flags0 = PPC_LOAD_U8(entityInfoPtr + 24);
        if (!(flags0 & 0x01))
            continue;  // no removal pending

        // isActive (vtable[5], offset +20)
        vtablePtr = PPC_LOAD_U32(entity + 0);
        if (vtablePtr == 0) goto skip_entity;
        {
            uint32_t vfIsActive = PPC_LOAD_U32(vtablePtr + 20);
            if (vfIsActive == 0) goto skip_entity;
            ctx.r3.u32 = entity;
            if (!SafeVCall(ctx, base, vfIsActive)) goto skip_entity;
            if (ctx.r3.u8 != 0)
                continue;  // still active, skip destruction
        }

        // Entity not active, removal requested — destroy
        entityInfoPtr = PPC_LOAD_U32(entity + 4);
        if (entityInfoPtr == 0) goto skip_entity;
        PPC_STORE_U8(entityInfoPtr + 109, 0xFF);

        // cleanup (vtable[2], offset +8)
        vtablePtr = PPC_LOAD_U32(entity + 0);
        if (vtablePtr == 0) goto skip_entity;
        {
            uint32_t vfCleanup = PPC_LOAD_U32(vtablePtr + 8);
            if (vfCleanup == 0) goto skip_entity;
            ctx.r3.u32 = entity;
            if (!SafeVCall(ctx, base, vfCleanup)) goto skip_entity;
        }

        // destructor (vtable[0], offset +0) with flag=1
        vtablePtr = PPC_LOAD_U32(entity + 0);
        if (vtablePtr == 0) goto skip_entity;
        {
            uint32_t vfDtor = PPC_LOAD_U32(vtablePtr + 0);
            if (vfDtor == 0) goto skip_entity;
            ctx.r3.u32 = entity;
            ctx.r4.s64 = 1;
            if (!SafeVCall(ctx, base, vfDtor)) goto skip_entity;
        }

        // Clear top 2 status bits
        sectorStatus = PPC_LOAD_U32(sectorEntry + 16);
        PPC_STORE_U32(sectorEntry + 16, sectorStatus & 0x3FFFFFFFu);

        // Mark sector dirty
        ctx.r3.u32 = STREAM_BOUNDS_ADDR;
        ctx.r4.u32 = (uint32_t)modelType;
        ctx.r5.u32 = (uint32_t)colIndex;
        __imp__sub_82921E60(ctx, base);
        continue;

    skip_entity:
        {
            int n = s_poolIterSkips.fetch_add(1, std::memory_order_relaxed);
            if (n < 50 || (n % 1000) == 0) {
                fprintf(stderr, "[POOL-ITER] skip slot %u (null vtable/func mid-dispatch)\n",
                        slot);
                fflush(stderr);
            }
        }
        continue;
    }

    // Store process count and call the finalize function
    PPC_STORE_U32(resultPtr + 16, (uint32_t)processCount);
    ctx.r3.u32 = resultPtr;
    __imp__sub_82902328(ctx, base);
}
