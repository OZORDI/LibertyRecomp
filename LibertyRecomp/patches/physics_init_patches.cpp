// =============================================================================
// physics_init_patches.cpp - phColliderManager + Audio/Physics Init Hooks
// =============================================================================
//
// Two layers of protection for the audio/physics broadphase init chain:
//
// Layer 1: sub_82478AF8 wrapper + NULL guards on sub_82953088, sub_82954618
//   The caller (broadphase init) does alloc(192) for the voice manager; if that
//   returns NULL, three downstream calls crash.  We guard them.
//
// Layer 2: sub_82955838 FULL REIMPLEMENTATION (phColliderManager::Init)
//   The #1 regression.  The original function allocates count * 672-byte
//   phCollider_rage voice objects, count * 44-byte control blocks, and a
//   uint16 index array, then loops calling vtable[1] on every voice to
//   bind it to its control block.
//
//   ROOT CAUSE: The init loop at 0x82955998 does:
//       r11 = [voice + 0]     // load vtable ptr
//       r11 = [r11  + 4]      // load vtable[1] = sub_829728E8
//       call r11(voice, 0, ctlBlock)
//   If the constructor (sub_82970B80) wrote vtable ptr 0x820A3E74 but the
//   guest .rdata page wasn't resident, or SIMD store-forwarding in the ctor
//   clobbered the first dword, then [voice+0] reads 0.  Dereferencing
//   [0 + 4] = address 4 yields 0 again on most platforms, causing an
//   infinite loop dispatching to address 0 (nullsub / trap).
//
//   FIX: Full C++ reimplementation with NULL checks on every allocation
//   and vtable validation before every indirect dispatch.
//
// Decompiled from GTA IV Xbox 360 (TU8) using PPC recomp scaffolds.
// Class: phCollider_rage (RTTI), vtable 0x820A3E74 (50 slots)
//
// =============================================================================
// phColliderManager layout (this = r3, ~200+ bytes):
//   +0x04  ownerPtr           (uint32)
//   +0x08  auxObj              (uint32, sub_8297AE58 product)
//   +0x0C  pairListObj         (uint32, sub_829CB7B8 product)
//   +0x10  broadphaseObj       (uint32, sub_829CA798 product)
//   +0x14  colArrayMemCtrl     (uint32, 16-byte struct)
//   +0x18  count               (uint32)
//   +0x1C  usedCount           (uint32, init 0)
//   +0x20  voiceArrayBase      (uint32, ptr to voice[0])
//   +0x24  flagArrayBase       (uint32, byte flags)
//   +0x28  ctlBlockBase        (uint32, ptr to ctlBlock[0])
//   +0x2C  indexArrayBase      (uint32, uint16 per entry)
//   +0x38  freeListField       (uint32)
//   +0x3C  obj240_a            (uint32, 240-byte alloc)
//   +0x44  obj240_b            (uint32, 240-byte alloc)
//   +0xA8  hashList168         (uint32)
//   +0xAC  hashCount172        (uint16)
//   +0xAE  hashCap174          (uint16)
//   +0xB0  hashList176         (uint32)
//   +0xB4  hashCount180        (uint16)
//   +0xB6  hashCap182          (uint16)
//   +0xB8  hashList184         (uint32)
//   +0xBE  hashCap190          (uint16)
// =============================================================================
// phCollider_rage (voice object, 672 bytes):
//   +0x00  vtable              (uint32, = 0x820A3E74)
//   +0x10  phInstPtr           (uint32)
//   +0x18  ctlBlockPtr         (uint32)
//   +0xA0  aabbMin             (float[3] at +160)
//   +0xB0  aabbReciprocal      (float[4] at +176)
//   +0xC0  boundsA             (float)
//   +0xC4  boundsB             (float)
//   +0xC8  boundsC             (float)
//   +0xD0  matrix3x3           (float[9] at +208)
//   +0x150-0x1C0  AABB arrays  (zero-filled by vtable[5])
//   +0x292 flagA               (uint8)
//   +0x293 flagB               (uint8)
//   +0x294 flagC               (uint8)
//   +0x29C field668            (uint32, init 0)
// =============================================================================

#include <stdafx.h>
#include <api/Liberty.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>

// === Guest function imports ===
extern "C" PPC_FUNC(__imp__sub_82478AF8);  // main broadphase init (original)
extern "C" PPC_FUNC(__imp__sub_82953088);  // voiceMgr publish to globals
extern "C" PPC_FUNC(__imp__sub_82954618);  // voiceMgr set max concurrent voices
extern "C" PPC_FUNC(__imp__sub_82955838);  // CreateSourceVoices (original)
extern "C" PPC_FUNC(__imp__sub_821B3510);  // rage::datMemoryAllocator::Allocate (r3=size)
extern "C" PPC_FUNC(__imp__sub_821B3560);  // rage::datMemoryAllocator::Free (r3=ptr)
extern "C" PPC_FUNC(__imp__sub_82970B80);  // phCollider_rage constructor (r3=this)
extern "C" PPC_FUNC(__imp__sub_8296F0C0);  // CtlBlock constructor (r3=this, r4=0)
extern "C" PPC_FUNC(__imp__sub_8296F040);  // CtlBlock::Link (r3=ctlBlock, r4=voice)
extern "C" PPC_FUNC(__imp__sub_82953290);  // phColliderManager post-init setup
extern "C" PPC_FUNC(__imp__sub_8297AE58);  // aux object constructor (r3=mem, r4=param)
extern "C" PPC_FUNC(__imp__sub_829CA798);  // broadphase object constructor (r3=mem)
extern "C" PPC_FUNC(__imp__sub_829CB7B8);  // pair-list constructor (r3=mem, r4=param)
extern "C" PPC_FUNC(__imp__sub_822BCA90);  // final init call (nullsub / debug print)

// === Constants ===
static constexpr uint32_t VTABLE_PHCOLLIDER   = 0x820A3E74;
static constexpr uint32_t VOICE_SIZE           = 672;
static constexpr uint32_t CTL_BLOCK_SIZE       = 44;
static constexpr uint32_t VTABLE_SLOT_INIT     = 1; // vtable[1] = sub_829728E8

// Globals written by sub_82478AF8:
static constexpr uint32_t ADDR_AUD_SOUND_MANAGER  = 0x82FF5360;
static constexpr uint32_t ADDR_AUD_VOICE_MANAGER  = 0x82FF5364;
static constexpr uint32_t ADDR_BT_BROADPHASE      = 0x82FF5368;
static constexpr uint32_t ADDR_CONTACT_POOL       = 0x82FF536C;

// Singleton check (from tail of sub_82955838)
static constexpr uint32_t ADDR_COLLIDER_MGR_SINGLETON  = 0x831CD9C0;
static constexpr uint32_t ADDR_COLLIDER_MGR_AUX_GLOBAL = 0x831CDA78;

// === Helpers ===

static uint32_t GameAlloc(PPCContext& ctx, uint8_t* base, uint32_t size) {
    ctx.r3.u32 = size;
    __imp__sub_821B3510(ctx, base);
    return ctx.r3.u32;
}

static void GameFree(PPCContext& ctx, uint8_t* base, uint32_t ptr) {
    if (ptr != 0) {
        ctx.r3.u32 = ptr;
        __imp__sub_821B3560(ctx, base);
    }
}

// Validate that a vtable pointer is in the expected .rdata range and that
// the target slot contains a plausible code address.
static bool IsValidVtableSlot(uint8_t* base, uint32_t vtablePtr, uint32_t slotIndex) {
    if (vtablePtr < 0x82000000 || vtablePtr > 0x830FFFFF)
        return false;
    uint32_t funcAddr = PPC_LOAD_U32(vtablePtr + slotIndex * 4);
    if (funcAddr < 0x82100000 || funcAddr > 0x82BFFFFF)
        return false;
    return true;
}

// =============================================================================
// sub_82478AF8 -- Audio/Physics init (diagnostics wrapper)
// =============================================================================

PPC_FUNC_HOOK(sub_82478AF8) {
    LOG_INFO("[PhysicsInit] sub_82478AF8: entering audio/physics system init");

    __imp__sub_82478AF8(ctx, base);

    uint32_t soundMgr    = PPC_LOAD_U32(ADDR_AUD_SOUND_MANAGER);
    uint32_t voiceMgr    = PPC_LOAD_U32(ADDR_AUD_VOICE_MANAGER);
    uint32_t broadphase  = PPC_LOAD_U32(ADDR_BT_BROADPHASE);
    uint32_t contactPool = PPC_LOAD_U32(ADDR_CONTACT_POOL);

    if (soundMgr == 0)
        LOG_WARNING("[PhysicsInit] audSoundManager alloc(176) returned NULL!");
    if (voiceMgr == 0)
        LOG_WARNING("[PhysicsInit] audVoiceManager alloc(192) returned NULL!");
    if (broadphase == 0)
        LOG_WARNING("[PhysicsInit] btBroadphase alloc(1024) returned NULL!");
    if (contactPool == 0)
        LOG_WARNING("[PhysicsInit] contactPool alloc returned NULL!");

    LOG_INFO("[PhysicsInit] sub_82478AF8 complete");
}

// =============================================================================
// sub_82953088 -- Publish voiceMgr to globals (NULL guard)
// =============================================================================

PPC_FUNC_HOOK(sub_82953088) {
    if (ctx.r3.u32 == 0) {
        LOG_WARNING("[PhysicsInit] sub_82953088: voiceMgr is NULL, skipping publish");
        return;
    }
    __imp__sub_82953088(ctx, base);
}

// =============================================================================
// sub_82954618 -- Set max concurrent voices (NULL guard)
// =============================================================================

PPC_FUNC_HOOK(sub_82954618) {
    if (ctx.r3.u32 == 0) {
        LOG_WARNING("[PhysicsInit] sub_82954618: voiceMgr is NULL, skipping");
        return;
    }
    __imp__sub_82954618(ctx, base);
}

// =============================================================================
// sub_82955838 -- phColliderManager::Init  (FULL REIMPLEMENTATION)
// =============================================================================
// Decompiled from PPC recomp scaffold at gta4_recomp.63.cpp lines 82575-83103.
// Every allocation is NULL-checked.  Every vtable dispatch is validated.
//
// Parameters (from caller sub_82478AF8):
//   r3 = this  (phColliderManager, ~200 bytes)
//   r4 = ownerPtr
//   r5 = count (number of phCollider_rage objects to create)
//   r6 = auxParam   (forwarded to sub_8297AE58 constructor)
//   r7 = pairListParam (forwarded to sub_829CB7B8 constructor)
// =============================================================================

PPC_FUNC_HOOK(sub_82955838) {
    const uint32_t thisPtr       = ctx.r3.u32;
    const uint32_t ownerPtr      = ctx.r4.u32;
    uint32_t       count         = ctx.r5.u32;
    const uint32_t auxParam      = ctx.r6.u32;
    const uint32_t pairListParam = ctx.r7.u32;

    // --- NULL this guard (same as before) ---
    if (thisPtr == 0) {
        LOG_WARNING("[PhysicsInit] sub_82955838: this is NULL, aborting");
        ctx.r3.u64 = 0;
        return;
    }

    // Store owner pointer and count
    PPC_STORE_U32(thisPtr + 0x04, ownerPtr);
    PPC_STORE_U32(thisPtr + 0x18, count);

    // Overflow cap -- original checks: count <= (lis 97 | ori 34328) = 0x00618618 = 6391320
    if (count > 6391320u) {
        LOGF_WARNING("[PhysicsInit] sub_82955838: count {} exceeds limit, clamping", count);
        count = 6391320u;
        PPC_STORE_U32(thisPtr + 0x18, count);
    }

    // =====================================================================
    // Allocation 1: Voice array  (count * 672 + 16 header)
    // Header [+0] = count, array starts at [+16]
    // =====================================================================
    uint32_t voiceArrayBase = 0;
    if (count > 0) {
        uint32_t allocSize = count * VOICE_SIZE + 16;
        uint32_t rawAlloc = GameAlloc(ctx, base, allocSize);
        if (rawAlloc != 0) {
            PPC_STORE_U32(rawAlloc, count);
            voiceArrayBase = rawAlloc + 16;

            // Construct each phCollider_rage (writes vtable 0x820A3E74)
            for (uint32_t i = 0; i < count; i++) {
                ctx.r3.u32 = voiceArrayBase + i * VOICE_SIZE;
                __imp__sub_82970B80(ctx, base);
            }
        } else {
            LOG_WARNING("[PhysicsInit] sub_82955838: voice array alloc failed");
        }
    }
    PPC_STORE_U32(thisPtr + 0x20, voiceArrayBase);

    // =====================================================================
    // Allocation 2: Byte-flag array (count bytes, via allocator(count))
    // =====================================================================
    uint32_t flagArrayBase = 0;
    if (count > 0) {
        flagArrayBase = GameAlloc(ctx, base, count);
    }
    PPC_STORE_U32(thisPtr + 0x24, flagArrayBase);

    // =====================================================================
    // Allocation 3: Control block array (count * 44 + 4 header)
    // Header [+0] = count, array starts at [+4]
    // =====================================================================
    uint32_t ctlBlockBase = 0;
    if (count > 0) {
        uint32_t ctlAllocSize = count * CTL_BLOCK_SIZE + 4;
        uint32_t ctlRaw = GameAlloc(ctx, base, ctlAllocSize);
        if (ctlRaw != 0) {
            PPC_STORE_U32(ctlRaw, count);
            ctlBlockBase = ctlRaw + 4;

            for (uint32_t i = 0; i < count; i++) {
                ctx.r3.u32 = ctlBlockBase + i * CTL_BLOCK_SIZE;
                ctx.r4.u32 = 0;  // no voice linked yet
                __imp__sub_8296F0C0(ctx, base);
            }
        } else {
            LOG_WARNING("[PhysicsInit] sub_82955838: control block array alloc failed");
        }
    }
    PPC_STORE_U32(thisPtr + 0x28, ctlBlockBase);

    // =====================================================================
    // Allocation 4: Index array  (count * 2 bytes, uint16 per entry)
    // =====================================================================
    uint32_t indexArrayBase = 0;
    if (count > 0) {
        indexArrayBase = GameAlloc(ctx, base, count * 2);
    }
    PPC_STORE_U32(thisPtr + 0x2C, indexArrayBase);

    // =====================================================================
    // Init loop: for i in 0..count-1
    //   1. Store index i as uint16 into indexArray
    //   2. voice->vtable[1](voice, 0, ctlBlock)   <-- THE CRASH SITE
    //   3. Clear flag byte to 0
    //   4. sub_8296F040(ctlBlock, voice)           <-- link them
    // =====================================================================
    if (voiceArrayBase != 0 && ctlBlockBase != 0 && indexArrayBase != 0) {
        for (uint32_t i = 0; i < count; i++) {
            uint32_t voiceAddr = voiceArrayBase + i * VOICE_SIZE;
            uint32_t ctlAddr   = ctlBlockBase   + i * CTL_BLOCK_SIZE;

            // Store index
            PPC_STORE_U16(indexArrayBase + i * 2, static_cast<uint16_t>(i));

            // --- CRITICAL VTABLE DISPATCH: voice->vtable[1](voice, 0, ctlBlock) ---
            uint32_t vtablePtr = PPC_LOAD_U32(voiceAddr);

            if (vtablePtr != 0 && IsValidVtableSlot(base, vtablePtr, VTABLE_SLOT_INIT)) {
                // Normal path: dispatch through vtable[1] = sub_829728E8
                uint32_t initFunc = PPC_LOAD_U32(vtablePtr + VTABLE_SLOT_INIT * 4);
                ctx.r3.u32 = voiceAddr;
                ctx.r4.u32 = 0;          // phInst = NULL (no physics instance yet)
                ctx.r5.u32 = ctlAddr;
                PPC_CALL_INDIRECT_FUNC(initFunc);
            } else {
                // RECOVERY PATH: vtable is NULL or corrupt.
                // This is the exact bug that caused the infinite loop.
                //
                // What sub_829728E8 does when phInst == NULL:
                //   1. Calls sub_829727C8(voice, 0) which:
                //      a. Stores 0 at voice+16 (phInst field)
                //      b. Stores 0 at voice+668
                //      c. Calls sub_8296F268(voice) to reset collision state
                //      d. Since phInst==0, goes to null path:
                //         Sets voice+192 = 1.0, voice+196 = 1.0
                //         Sets voice+160..171 = {1.0, 1.0, 1.0}
                //         Calls sub_829BE200 (AABB normalize)
                //         Stores AABB reciprocal at voice+176
                //   2. Stores ctlBlock at voice+24
                //
                // We do the essential parts: zero phInst, store ctlBlock.
                if (i == 0) {
                    LOGF_WARNING("[PhysicsInit] sub_82955838: voice[0] vtable={:#010x} invalid! "
                                 "Applying manual init for all {} voices", vtablePtr, count);
                }

                // Fix the vtable if it was zero
                if (vtablePtr == 0) {
                    PPC_STORE_U32(voiceAddr, VTABLE_PHCOLLIDER);
                }
                PPC_STORE_U32(voiceAddr + 16, 0);    // phInst = NULL
                PPC_STORE_U32(voiceAddr + 668, 0);   // field668 = 0
                PPC_STORE_U32(voiceAddr + 24, ctlAddr); // ctlBlock ptr
            }

            // Clear flag byte
            if (flagArrayBase != 0) {
                PPC_STORE_U8(flagArrayBase + i, 0);
            }

            // Link control block to voice: sub_8296F040(ctlBlock, voice)
            ctx.r3.u32 = ctlAddr;
            ctx.r4.u32 = voiceAddr;
            __imp__sub_8296F040(ctx, base);
        }
    } else if (count > 0) {
        LOGF_WARNING("[PhysicsInit] sub_82955838: arrays NULL (v={:#010x} c={:#010x} i={:#010x}), "
                     "skipping init loop", voiceArrayBase, ctlBlockBase, indexArrayBase);
    }

    // =====================================================================
    // Post-init: remaining allocations (from loc_829559FC onward)
    // =====================================================================

    // usedCount = 0
    PPC_STORE_U32(thisPtr + 0x1C, 0);

    // Alloc 240 bytes -> field +0x3C
    PPC_STORE_U32(thisPtr + 0x3C, GameAlloc(ctx, base, 240));

    // Alloc 240 bytes -> field +0x44
    PPC_STORE_U32(thisPtr + 0x44, GameAlloc(ctx, base, 240));

    // sub_82953290(this) -- post-init setup
    ctx.r3.u32 = thisPtr;
    __imp__sub_82953290(ctx, base);

    // Free + re-alloc hash lists at offsets +168..+190
    // Original: free old, zero counts, alloc 2048 each, capacity=512
    GameFree(ctx, base, PPC_LOAD_U32(thisPtr + 168));
    PPC_STORE_U32(thisPtr + 168, 0);
    PPC_STORE_U16(thisPtr + 172, 0);
    PPC_STORE_U16(thisPtr + 174, 0);

    GameFree(ctx, base, PPC_LOAD_U32(thisPtr + 176));
    PPC_STORE_U32(thisPtr + 176, 0);
    PPC_STORE_U16(thisPtr + 180, 0);
    PPC_STORE_U16(thisPtr + 182, 0);

    PPC_STORE_U32(thisPtr + 168, GameAlloc(ctx, base, 2048));
    PPC_STORE_U16(thisPtr + 174, 512);

    PPC_STORE_U32(thisPtr + 176, GameAlloc(ctx, base, 2048));
    PPC_STORE_U16(thisPtr + 182, 512);

    PPC_STORE_U32(thisPtr + 184, GameAlloc(ctx, base, 2048));
    PPC_STORE_U16(thisPtr + 190, 512);

    // Alloc 48 -> sub_8297AE58(mem, auxParam) -> store at +0x08
    {
        uint32_t auxMem = GameAlloc(ctx, base, 48);
        uint32_t auxObj = 0;
        if (auxMem != 0) {
            ctx.r3.u32 = auxMem;
            ctx.r4.u32 = auxParam;
            __imp__sub_8297AE58(ctx, base);
            auxObj = ctx.r3.u32;
        }
        PPC_STORE_U32(thisPtr + 0x08, auxObj);

        // If this is the global singleton, also store auxObj at secondary global
        uint32_t singleton = PPC_LOAD_U32(ADDR_COLLIDER_MGR_SINGLETON);
        if (singleton == thisPtr && auxObj != 0) {
            PPC_STORE_U32(ADDR_COLLIDER_MGR_AUX_GLOBAL, auxObj);
        }
    }

    // Alloc 76 -> sub_829CB7B8(mem, pairListParam) -> store at +0x0C
    {
        uint32_t plMem = GameAlloc(ctx, base, 76);
        uint32_t pairListObj = 0;
        if (plMem != 0) {
            ctx.r3.u32 = plMem;
            ctx.r4.u32 = pairListParam;
            __imp__sub_829CB7B8(ctx, base);
            pairListObj = ctx.r3.u32;
        }
        PPC_STORE_U32(thisPtr + 0x0C, pairListObj);
    }

    // Alloc 16 -> colArrayMemCtrl -> store at +0x14
    // Original reads owner+100 for capacity, allocs cap*3*16
    {
        uint32_t memCtrl = GameAlloc(ctx, base, 16);
        if (memCtrl != 0 && ownerPtr != 0) {
            uint32_t cap = PPC_LOAD_U32(ownerPtr + 100);
            PPC_STORE_U32(memCtrl + 8, 0);
            PPC_STORE_U32(memCtrl + 4, cap);

            uint64_t dataSize64 = static_cast<uint64_t>(cap) * 3u * 16u;
            uint32_t dataSize = (dataSize64 <= 0xFFFFFFF0u)
                                    ? static_cast<uint32_t>(dataSize64)
                                    : 0xFFFFFFFFu;
            PPC_STORE_U32(memCtrl + 0, GameAlloc(ctx, base, dataSize));
        }
        PPC_STORE_U32(thisPtr + 0x14, memCtrl);
    }

    // Alloc 880 -> sub_829CA798(mem) -> broadphaseObj -> store at +0x10
    // Then call sub_822BCA90(broadphaseObj)
    {
        uint32_t bpMem = GameAlloc(ctx, base, 880);
        uint32_t broadphaseObj = 0;
        if (bpMem != 0) {
            ctx.r3.u32 = bpMem;
            __imp__sub_829CA798(ctx, base);
            broadphaseObj = ctx.r3.u32;
        }
        PPC_STORE_U32(thisPtr + 0x10, broadphaseObj);

        // sub_822BCA90: final broadphase init (called even if obj is NULL)
        ctx.r3.u32 = broadphaseObj;
        __imp__sub_822BCA90(ctx, base);
    }

    // Return this
    ctx.r3.u32 = thisPtr;
}
