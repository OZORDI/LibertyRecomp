// audio_pool.cpp
// RAGE Audio Voice Pool System - Clean C++ reimplementation
//
// Decompiled from GTA IV Xbox 360 (TU5) using IDA Hex-Rays + PPC recomp scaffolds.
// Implements the audVoicePool system that manages sound entities in a fixed-size
// pool with per-frame iteration, vtable dispatch, and voice control block (VCB)
// state tracking.
//
// Global state:
//   0x831C8EC0 = g_audVoicePool (ptr to CPool struct)
//   0x831C8EC9 = g_poolLockFlag (byte, spinlock for pool access)
//   0x831C8A38 = g_audMixerDevice (audMixerDevice instance)
//   0x831C8B20 = g_audMixerDevice.voiceSlotBase (at mixerDevice+0xE8)
//   0x831C8E88 = g_staticSoundList (ptr to uint16 array)
//   0x831C8E8C = g_dynamicSoundList (ptr to uint16 array)
//   0x831C9440 = g_audSecondaryGlobal (ptr)
//   0x831C6700 = g_audioFrameCounter (uint32)
//
// CPool layout (56 bytes):
//   +0x00: data        (uint32 - guest ptr to entity array, stride 768)
//   +0x04: flags       (uint32 - guest ptr to flag bytes, 1 per slot)
//   +0x08: critsec     (RTL_CRITICAL_SECTION, 24 bytes)
//   +0x28: count       (uint32 - total slot count)
//   +0x2C: free        (uint32 - available slot count)
//   +0x30: cursor      (uint32 - allocation scan cursor)
//   +0x34: init        (uint8  - initialized flag)
//
// Pool entity (768 bytes per slot):
//   +0x00: vtable      (uint32 - guest ptr to vtable)
//   +0x04: soundParams (uint32 - guest ptr to sound parameter block)
//   +0x88: entityFlags (uint8  - bit3 = something)
//
// Sound parameter block (pointed by entity+4):
//   +0x18: flags24     (uint8 - bit0=loop, bit1=volDirty, bit2=forceUpdate, bit3=needsSeek)
//   +0x19: flags25     (uint8 - bit7=seekDirtyHigh)
//   +0x6D: poolSlotIdx (uint8 - index of this entity in the pool)
//   +0x6E: categoryId  (uint8 - mixer category for voice slot lookup)
//
// Voice Control Block (VCB, 32 bytes):
//   +0x00: capacity    (uint32)
//   +0x04: nextSlot    (int32, -1 = none)
//   +0x08: volAndFlags (uint32 - bits[31:3]=volume/position, bits[2:0]=flags)
//   +0x0C: priority    (uint8 at byte offset 12, bit7=hasVolume)
//   +0x10: stateFlags  (uint32 - bits[31:30]=state: 0=free,1=pending,2=active,3=stopping)
//
// Entity vtable layout:
//   [0]  +0x00: SetState(entity, stateArg)
//   [1]  +0x04: Init(entity, soundParams)
//   [2]  +0x08: Stop(entity)
//   [3]  +0x0C: UpdateVolume(entity, volume)
//   [4]  +0x10: Play(entity)
//   [5]  +0x14: IsFinished(entity) -> bool
//   [6]  +0x18: GetPriority(entity) -> byte
//   [7]  +0x1C: Seek(entity, seekPos)
//   [9]  +0x24: GetVolume(entity) -> int
//   [10] +0x28: Update(entity)

#include <cpu/ppc_context.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>

// ---- Address constants ----
static constexpr uint32_t ADDR_POOL_PTR         = 0x831C8EC0; // ptr to CPool
static constexpr uint32_t ADDR_POOL_LOCK_FLAG   = 0x831C8EC9; // byte spinlock
static constexpr uint32_t ADDR_MIXER_DEVICE     = 0x831C8A38; // audMixerDevice
static constexpr uint32_t ADDR_STATIC_LIST      = 0x831C8E88; // ptr to uint16[]
static constexpr uint32_t ADDR_DYNAMIC_LIST     = 0x831C8E8C; // ptr to uint16[]
static constexpr uint32_t ADDR_SECONDARY_GLOBAL = 0x831C9440; // secondary global
static constexpr uint32_t ADDR_FRAME_COUNTER    = 0x831C6700; // uint32

static constexpr uint32_t ENTITY_STRIDE     = 768;
static constexpr uint32_t VCB_SIZE          = 32;
static constexpr uint32_t VOICE_SLOT_SIZE   = 112;
static constexpr uint32_t SLOTS_PER_CAT     = 130;
static constexpr uint32_t CAT_BYTE_STRIDE   = 14560; // 130 * 112

// ---- CPool field offsets (from pool base) ----
static constexpr uint32_t POOL_OFF_DATA     = 0x00;
static constexpr uint32_t POOL_OFF_FLAGS    = 0x04;
static constexpr uint32_t POOL_OFF_CRITSEC  = 0x08;
static constexpr uint32_t POOL_OFF_COUNT    = 0x28;
static constexpr uint32_t POOL_OFF_FREE     = 0x2C;
static constexpr uint32_t POOL_OFF_CURSOR   = 0x30;
static constexpr uint32_t POOL_OFF_INIT     = 0x34;

// ---- VCB field offsets ----
static constexpr uint32_t VCB_OFF_CAPACITY    = 0x00;
static constexpr uint32_t VCB_OFF_NEXT_SLOT   = 0x04;
static constexpr uint32_t VCB_OFF_VOL_FLAGS   = 0x08;
static constexpr uint32_t VCB_OFF_PRIORITY    = 0x0C;
static constexpr uint32_t VCB_OFF_STATE_FLAGS = 0x10;

// ---- Sound params offsets ----
static constexpr uint32_t SP_OFF_FLAGS24    = 0x18; // (24)
static constexpr uint32_t SP_OFF_FLAGS25    = 0x19; // (25)
static constexpr uint32_t SP_OFF_SLOT_IDX   = 0x6D; // (109)
static constexpr uint32_t SP_OFF_CATEGORY   = 0x6E; // (110)

// ---- Entity offsets ----
static constexpr uint32_t ENT_OFF_VTABLE       = 0x00;
static constexpr uint32_t ENT_OFF_SOUND_PARAMS  = 0x04;
static constexpr uint32_t ENT_OFF_ENTITY_FLAGS  = 0x88; // (136)

// ---- Helper: read pool pointer ----
static inline uint32_t GetPoolBase(uint8_t* base) {
    return PPC_LOAD_U32(ADDR_POOL_PTR);
}

// ---- Helper: get mixer device voice slot base ----
static inline uint32_t GetVoiceSlotBase(uint8_t* base) {
    return PPC_LOAD_U32(ADDR_MIXER_DEVICE + 232); // mixerDevice+0xE8
}

// ---- Helper: compute voice slot index (sub_829222E0) ----
// Returns: (-14560 * categoryId - voiceSlotBase + soundParams - 96) / 112
static inline int32_t GetVoiceSlotIndex(uint8_t* base, uint32_t mixerDevice,
                                         uint8_t categoryId, uint32_t soundParams) {
    uint32_t vsBase = PPC_LOAD_U32(mixerDevice + 232);
    return (int32_t)(-CAT_BYTE_STRIDE * categoryId - vsBase + soundParams - 96) / (int32_t)VOICE_SLOT_SIZE;
}

// ---- Helper: compute VCB address ----
// VCB = (455 * categoryId + slotIndex + 339) * 32 + voiceSlotBase
static inline uint32_t GetVCBAddress(uint8_t* base, uint32_t mixerDevice,
                                      uint8_t categoryId, int32_t slotIndex) {
    uint32_t vsBase = PPC_LOAD_U32(mixerDevice + 232);
    uint32_t flatIndex = 455 * (uint32_t)categoryId + (uint32_t)slotIndex + 339;
    return flatIndex * VCB_SIZE + vsBase;
}

// ---- Helper: safe vtable dispatch ----
// Returns false if vtable is null/invalid. On success, calls the vfunc.
static inline bool SafeVtableCall(PPCContext& ctx, uint8_t* base,
                                   uint32_t entityAddr, uint32_t vtableOffset) {
    uint32_t vtable = PPC_LOAD_U32(entityAddr + ENT_OFF_VTABLE);
    if (vtable == 0) return false;
    uint32_t func = PPC_LOAD_U32(vtable + vtableOffset);
    if (func == 0) return false;
    ctx.r3.u32 = entityAddr;
    ctx.lr = 0; // return address not needed for indirect call
    PPC_CALL_INDIRECT_FUNC(func);
    return true;
}

static inline bool SafeVtableCall1(PPCContext& ctx, uint8_t* base,
                                    uint32_t entityAddr, uint32_t vtableOffset,
                                    uint32_t arg) {
    uint32_t vtable = PPC_LOAD_U32(entityAddr + ENT_OFF_VTABLE);
    if (vtable == 0) return false;
    uint32_t func = PPC_LOAD_U32(vtable + vtableOffset);
    if (func == 0) return false;
    ctx.r3.u32 = entityAddr;
    ctx.r4.u32 = arg;
    ctx.lr = 0;
    PPC_CALL_INDIRECT_FUNC(func);
    return true;
}

// ---- Helper: mark voice slot for release (sub_82921E60) ----
static inline void MarkVoiceSlotForRelease(uint8_t* base, uint32_t mixerDevice,
                                            uint8_t categoryId, uint8_t slotIndex) {
    uint32_t vsBase = PPC_LOAD_U32(mixerDevice + 232);
    uint32_t byteAddr = vsBase + CAT_BYTE_STRIDE * (uint32_t)categoryId + (uint32_t)slotIndex;
    uint8_t val = PPC_LOAD_U8(byteAddr);
    PPC_STORE_U8(byteAddr, val | 0x04);
}

// ---- Forward declarations for original PPC functions we call through ----
extern "C" PPC_FUNC(__imp__sub_82849918);  // SleepEx(0) thunk - yield
extern "C" PPC_FUNC(__imp__sub_829222E0);  // GetVoiceSlotIndex
extern "C" PPC_FUNC(__imp__sub_82921E60);  // MarkVoiceSlotForRelease
extern "C" PPC_FUNC(__imp__sub_82902328);  // PostProcess
extern "C" PPC_FUNC(__imp__sub_8285FE78);  // EnterCriticalSection
extern "C" PPC_FUNC(__imp__sub_8285FEE0);  // LeaveCriticalSection
extern "C" PPC_FUNC(__imp__sub_8285FE48);  // InitializeCriticalSection
extern "C" PPC_FUNC(__imp__sub_821B3510);  // rage::malloc
extern "C" PPC_FUNC(__imp__sub_822BCA90);  // nullsub_1 (debug print)
extern "C" PPC_FUNC(__imp__sub_82905E20);  // GetCategoryPtr
extern "C" PPC_FUNC(__imp__sub_82906A70);  // GetSoundDefinition
extern "C" PPC_FUNC(__imp__sub_829305A8);  // ConstructStaticSound
extern "C" PPC_FUNC(__imp__sub_8292F0C0);  // ConstructDynamicSound
extern "C" PPC_FUNC(__imp__sub_82931EB8);  // ComputeDeltaTime
extern "C" PPC_FUNC(__imp__sub_82931700);  // PrepareUpdateLists
extern "C" PPC_FUNC(__imp__sub_82931B98);  // BuildUpdateLists
extern "C" PPC_FUNC(__imp__sub_8291E488);  // AllocateAndConstruct
extern "C" PPC_FUNC(__imp__sub_8291E3C0);  // CPool::Allocate
extern "C" PPC_FUNC(__imp__sub_8291E538);  // ActivateStaticSound
extern "C" PPC_FUNC(__imp__sub_8291E260);  // UpdateDynamicSound
extern "C" PPC_FUNC(__imp__sub_8291F270);  // PostProcess inner
extern "C" PPC_FUNC(__imp__sub_8291DF00);  // CPool::Process
extern "C" PPC_FUNC(__imp__sub_8291DED8);  // SetPoolCapacity
extern "C" PPC_FUNC(__imp__sub_8291E858);  // CPool::Init
extern "C" PPC_FUNC(__imp__sub_8291E620);  // audSoundManager::Update

// =============================================================================
// sub_8291DED8 - SetPoolCapacity
// =============================================================================
// Sets the pool capacity to min(requested, total - reserved).
// a1 = audSoundManager* (this), a2 = requested capacity
// Fields: this+24 = total, this+28 = reserved, this+12 = capacity

PPC_FUNC_HOOK(sub_8291DED8) {
    uint32_t self = ctx.r3.u32;
    uint32_t requested = ctx.r4.u32;

    uint32_t total    = PPC_LOAD_U32(self + 24);
    uint32_t reserved = PPC_LOAD_U32(self + 28);
    uint32_t maxCap   = total - reserved;

    if (requested >= maxCap)
        PPC_STORE_U32(self + 12, maxCap);
    else
        PPC_STORE_U32(self + 12, requested);
}

// =============================================================================
// sub_8291E858 - CPool::Init
// =============================================================================
// Initializes the voice pool with 'count' slots.
// a1 = CPool*, a2 = count, a3 = name string (for debug print)

PPC_FUNC_HOOK(sub_8291E858) {
    uint32_t pool  = ctx.r3.u32;
    uint32_t count = ctx.r4.u32;
    uint32_t name  = ctx.r5.u32;

    // Initialize critical section at pool+8
    ctx.r3.u32 = pool + POOL_OFF_CRITSEC;
    __imp__sub_8285FE48(ctx, base); // RtlInitializeCriticalSection

    // Allocate entity data: count * 768 bytes
    uint32_t dataSize = count * ENTITY_STRIDE;
    ctx.r3.u32 = dataSize;
    __imp__sub_821B3510(ctx, base); // rage::malloc
    PPC_STORE_U32(pool + POOL_OFF_DATA, ctx.r3.u32);

    // Allocate flag bytes: count bytes
    ctx.r3.u32 = count;
    __imp__sub_821B3510(ctx, base); // rage::malloc
    PPC_STORE_U32(pool + POOL_OFF_FLAGS, ctx.r3.u32);

    // Initialize pool fields
    PPC_STORE_U32(pool + POOL_OFF_COUNT, count);
    PPC_STORE_U32(pool + POOL_OFF_FREE, count);
    PPC_STORE_U8(pool + POOL_OFF_INIT, 1);
    PPC_STORE_U32(pool + POOL_OFF_CURSOR, 0xFFFFFFFF);

    // Mark all slots as free (flag byte bit7 = 1 means available)
    uint32_t flagsPtr = PPC_LOAD_U32(pool + POOL_OFF_FLAGS);
    for (uint32_t i = 0; i < count; i++) {
        uint8_t val = PPC_LOAD_U8(flagsPtr + i);
        PPC_STORE_U8(flagsPtr + i, val | 0x80);
    }

    // Debug print: "Pool %s size : %d\n"
    // nullsub_1 in retail, but call original for completeness
    ctx.r3.u32 = 0x8209D9CC; // Address of "Pool %s size : %d\n" in .rdata
    ctx.r4.u32 = name;
    ctx.r5.u32 = dataSize;
    __imp__sub_822BCA90(ctx, base);

    ctx.r3.u32 = pool;
}

// =============================================================================
// sub_8291E3C0 - CPool::Allocate
// =============================================================================
// Finds a free slot in the pool. Returns guest pointer to entity, or 0 if full.
// Scans from cursor, wraps around once. Thread-safe via critical section.

PPC_FUNC_HOOK(sub_8291E3C0) {
    uint32_t pool = ctx.r3.u32;

    // EnterCriticalSection(pool+8)
    ctx.r3.u32 = pool + POOL_OFF_CRITSEC;
    __imp__sub_8285FE78(ctx, base);

    uint32_t count = PPC_LOAD_U32(pool + POOL_OFF_COUNT);
    bool wrapped = false;

    while (true) {
        uint32_t cursor = PPC_LOAD_U32(pool + POOL_OFF_CURSOR) + 1;
        PPC_STORE_U32(pool + POOL_OFF_CURSOR, cursor);

        if (cursor == count) {
            PPC_STORE_U32(pool + POOL_OFF_CURSOR, 0);
            if (wrapped) {
                // Pool full - leave critical section and return 0
                ctx.r3.u32 = pool + POOL_OFF_CRITSEC;
                __imp__sub_8285FEE0(ctx, base);
                ctx.r3.u32 = 0;
                return;
            }
            wrapped = true;
        }

        uint32_t cursorNow = PPC_LOAD_U32(pool + POOL_OFF_CURSOR);
        uint32_t flagsPtr  = PPC_LOAD_U32(pool + POOL_OFF_FLAGS);
        uint8_t flag = PPC_LOAD_U8(flagsPtr + cursorNow);

        if (flag & 0x80) {
            // Found a free slot - mark as allocated (clear bit7)
            PPC_STORE_U8(flagsPtr + cursorNow, flag & 0x7F);

            // Decrement free count
            uint32_t freeCount = PPC_LOAD_U32(pool + POOL_OFF_FREE);
            PPC_STORE_U32(pool + POOL_OFF_FREE, freeCount - 1);

            // LeaveCriticalSection
            ctx.r3.u32 = pool + POOL_OFF_CRITSEC;
            __imp__sub_8285FEE0(ctx, base);

            // Return pointer: pool.data + cursorNow * 768
            uint32_t dataPtr = PPC_LOAD_U32(pool + POOL_OFF_DATA);
            ctx.r3.u32 = dataPtr + cursorNow * ENTITY_STRIDE;
            return;
        }
    }
}

// =============================================================================
// sub_8291E488 - AllocateAndConstruct
// =============================================================================
// Allocates a pool slot and constructs a sound entity (static or dynamic).
// a1 = unused (caller context), a2 = soundParams guest ptr
// Returns guest ptr to constructed entity, or 0 if pool full.

PPC_FUNC_HOOK(sub_8291E488) {
    uint32_t soundParams = ctx.r4.u32;

    uint32_t unkPtr = PPC_LOAD_U32(soundParams + 8); // soundParams+8
    int16_t typeId  = (int16_t)PPC_LOAD_U16(soundParams + 20); // soundParams+20

    // Get category pointer
    ctx.r3.s64 = typeId;
    __imp__sub_82905E20(ctx, base);

    // Get sound definition
    ctx.r4.u32 = unkPtr;
    __imp__sub_82906A70(ctx, base);

    // Check bit0 of definition+28 to determine static vs dynamic
    uint32_t defResult = ctx.r3.u32;
    bool isStatic = (PPC_LOAD_U32(defResult + 28) & 1) != 0;

    // Allocate from pool
    uint32_t pool = GetPoolBase(base);
    ctx.r3.u32 = pool;
    __imp__sub_8291E3C0(ctx, base);
    uint32_t entityAddr = ctx.r3.u32;

    if (entityAddr == 0) {
        ctx.r3.u32 = 0;
        return;
    }

    // Construct the appropriate sound type
    if (isStatic) {
        // sub_829305A8 = ConstructStaticSound
        ctx.r3.u32 = entityAddr;
        __imp__sub_829305A8(ctx, base);
    } else {
        // sub_8292F0C0 = ConstructDynamicSound
        ctx.r3.u32 = entityAddr;
        __imp__sub_8292F0C0(ctx, base);
    }

    uint32_t constructed = ctx.r3.u32;
    if (constructed != 0) {
        // Call virtual Init: vtable[1](entity, soundParams)
        uint32_t vtable = PPC_LOAD_U32(constructed + ENT_OFF_VTABLE);
        if (vtable != 0) {
            uint32_t initFunc = PPC_LOAD_U32(vtable + 4);
            if (initFunc != 0) {
                ctx.r3.u32 = constructed;
                ctx.r4.u32 = soundParams;
                PPC_CALL_INDIRECT_FUNC(initFunc);
            }
        }
    }

    ctx.r3.u32 = constructed;
}

// =============================================================================
// sub_8291E260 - UpdateDynamicSound
// =============================================================================
// Updates a dynamic sound's voice control block: plays it and advances the
// VCB position pointer with wrap-around logic.
// a1 = audSoundManager*, a2 = packed sound handle (hi byte=cat, lo byte=slot)

PPC_FUNC_HOOK(sub_8291E260) {
    uint32_t self = ctx.r3.u32;
    uint16_t soundHandle = (uint16_t)ctx.r4.u32;

    uint8_t categoryId = (uint8_t)(soundHandle >> 8);
    uint8_t slotNum    = (uint8_t)(soundHandle & 0xFF);

    uint32_t mixerDevice = ADDR_MIXER_DEVICE;
    uint32_t vsBase = GetVoiceSlotBase(base);

    // Resolve voice slot pointer
    uint32_t allocByte = PPC_LOAD_U8(vsBase + CAT_BYTE_STRIDE * (uint32_t)categoryId + (uint32_t)slotNum);
    uint32_t soundParams;
    if (allocByte & 0x01) {
        soundParams = ((uint32_t)SLOTS_PER_CAT * categoryId + slotNum) * VOICE_SLOT_SIZE
                      + vsBase + 96;
    } else {
        soundParams = 0;
    }

    // Guard: original code reads from addr 110 if slot unallocated (UB).
    // In practice the caller always provides a valid handle, but guard anyway.
    if (soundParams == 0) return;

    // Get category and slot index
    uint8_t catId = PPC_LOAD_U8(soundParams + SP_OFF_CATEGORY);

    // Call GetVoiceSlotIndex
    ctx.r3.u32 = mixerDevice;
    ctx.r4.u32 = catId;
    ctx.r5.u32 = soundParams;
    __imp__sub_829222E0(ctx, base);
    int32_t voiceSlotIdx = (int32_t)ctx.r3.u32;

    // Read pool slot index from sound params
    uint8_t poolSlotIdx = PPC_LOAD_U8(soundParams + SP_OFF_SLOT_IDX);

    // Compute VCB address
    uint32_t vcbAddr = (455 * (uint32_t)catId + (uint32_t)voiceSlotIdx + 339) * VCB_SIZE + vsBase;

    // Check if pool slot is allocated (flag bit7 clear = allocated)
    uint32_t pool = GetPoolBase(base);
    uint32_t flagsPtr = PPC_LOAD_U32(pool + POOL_OFF_FLAGS);
    uint8_t poolFlag = PPC_LOAD_U8(flagsPtr + poolSlotIdx);

    uint32_t entityAddr;
    if (poolFlag & 0x80) {
        // Slot not allocated - entity is null
        entityAddr = 0;
    } else {
        // Entity = pool.data + poolSlotIdx * 768
        uint32_t dataPtr = PPC_LOAD_U32(pool + POOL_OFF_DATA);
        entityAddr = dataPtr + (uint32_t)poolSlotIdx * ENTITY_STRIDE;
    }

    // Call virtual Play: vtable[4](entity)
    uint32_t vtable = PPC_LOAD_U32(entityAddr + ENT_OFF_VTABLE);
    if (vtable != 0) {
        uint32_t playFunc = PPC_LOAD_U32(vtable + 16); // vtable[4]
        if (playFunc != 0) {
            ctx.r3.u32 = entityAddr;
            PPC_CALL_INDIRECT_FUNC(playFunc);
        }
    }

    // Update VCB state: set bits[31:30] to 0b10 (active)
    uint32_t stateFlags = PPC_LOAD_U32(vcbAddr + VCB_OFF_STATE_FLAGS);
    stateFlags = (stateFlags & 0x3FFFFFFF) | 0x80000000;
    PPC_STORE_U32(vcbAddr + VCB_OFF_STATE_FLAGS, stateFlags);

    // Advance VCB position: newPos = currentPos + 80
    uint32_t volAndFlags = PPC_LOAD_U32(vcbAddr + VCB_OFF_VOL_FLAGS);
    uint32_t lowFlags = volAndFlags & 0x7;
    int32_t currentPos = (int32_t)volAndFlags >> 3;
    uint32_t capacity = PPC_LOAD_U32(vcbAddr + VCB_OFF_CAPACITY);
    uint32_t newPos = (uint32_t)((uint32_t)currentPos + 80);

    if (newPos >= capacity) {
        int32_t nextSlot = (int32_t)PPC_LOAD_U32(vcbAddr + VCB_OFF_NEXT_SLOT);
        if (nextSlot == -1) {
            // No next slot - write -1 position (0xFFFFFFF8 | flags)
            PPC_STORE_U32(vcbAddr + VCB_OFF_VOL_FLAGS, 0xFFFFFFF8 | lowFlags);
            return;
        }
        uint32_t range = capacity - (uint32_t)nextSlot;
        if (range == 0) {
            newPos = (uint32_t)nextSlot;
        } else {
            uint32_t overflow = newPos - (uint32_t)nextSlot;
            newPos = (overflow % range) + (uint32_t)nextSlot;
        }
    }

    PPC_STORE_U32(vcbAddr + VCB_OFF_VOL_FLAGS, (newPos << 3) | lowFlags);
}

// =============================================================================
// sub_8291E538 - ActivateStaticSound
// =============================================================================
// Activates a static sound: resolves voice slot, allocates pool entity,
// sets VCB state to pending, computes pool index.
// a1 = audSoundManager*, a2 = packed sound handle

PPC_FUNC_HOOK(sub_8291E538) {
    uint32_t self = ctx.r3.u32;
    uint16_t soundHandle = (uint16_t)ctx.r4.u32;

    uint8_t categoryId = (uint8_t)(soundHandle >> 8);
    uint8_t slotNum    = (uint8_t)(soundHandle & 0xFF);

    uint32_t mixerDevice = ADDR_MIXER_DEVICE;
    uint32_t vsBase = GetVoiceSlotBase(base);

    // Resolve voice slot pointer
    uint32_t allocByte = PPC_LOAD_U8(vsBase + CAT_BYTE_STRIDE * (uint32_t)categoryId + (uint32_t)slotNum);
    uint32_t soundParams;
    if (allocByte & 0x01) {
        soundParams = ((uint32_t)SLOTS_PER_CAT * categoryId + slotNum) * VOICE_SLOT_SIZE
                      + vsBase + 96;
    } else {
        soundParams = 0;
    }

    // Guard: original code has UB if slot unallocated.
    if (soundParams == 0) {
        ctx.r3.u32 = 0;
        return;
    }

    // Get category from sound params
    uint8_t catId = PPC_LOAD_U8(soundParams + SP_OFF_CATEGORY);

    // GetVoiceSlotIndex
    ctx.r3.u32 = mixerDevice;
    ctx.r4.u32 = catId;
    ctx.r5.u32 = soundParams;
    __imp__sub_829222E0(ctx, base);
    int32_t voiceSlotIdx = (int32_t)ctx.r3.u32;

    // Compute VCB address
    uint32_t vcbAddr = (455 * (uint32_t)catId + (uint32_t)voiceSlotIdx + 339) * VCB_SIZE + vsBase;

    // AllocateAndConstruct
    ctx.r3.u32 = self;
    ctx.r4.u32 = soundParams;
    __imp__sub_8291E488(ctx, base);
    uint32_t entityAddr = ctx.r3.u32;

    // Set VCB state to pending (bits[31:30] = 0b01 = 0x40000000)
    uint32_t stateFlags = PPC_LOAD_U32(vcbAddr + VCB_OFF_STATE_FLAGS);
    stateFlags = (stateFlags & 0x3FFFFFFF) | 0x40000000;
    PPC_STORE_U32(vcbAddr + VCB_OFF_STATE_FLAGS, stateFlags);

    // Compute pool slot index: (entityAddr - pool.data) / 768
    uint32_t pool = GetPoolBase(base);
    uint32_t dataPtr = PPC_LOAD_U32(pool + POOL_OFF_DATA);
    uint32_t diff = entityAddr - dataPtr;
    // Use the same magic multiply as the original PPC code:
    // mulhw with 0x2AAAAAAB then >>7, with sign correction
    int32_t signedDiff = (int32_t)diff;
    int64_t product = (int64_t)signedDiff * (int64_t)0x2AAAAAAB;
    int32_t highWord = (int32_t)(product >> 32);
    int32_t shifted = highWord >> 7;
    int32_t signBit = (uint32_t)highWord >> 31;
    int32_t slotIndex = shifted + signBit;

    PPC_STORE_U8(soundParams + SP_OFF_SLOT_IDX, (uint8_t)slotIndex);

    // Set volDirty flag
    uint8_t f24 = PPC_LOAD_U8(soundParams + SP_OFF_FLAGS24);
    PPC_STORE_U8(soundParams + SP_OFF_FLAGS24, f24 | 0x02);

    ctx.r3.u32 = entityAddr;
}

// =============================================================================
// sub_8291DF00 - CPool::Process
// =============================================================================
// Main per-frame iteration over all pool entities. Handles:
// - Teardown of finished sounds (IsFinished -> Stop -> SetState(1) -> clear VCB)
// - Seek position updates
// - Volume dirty flag handling
// - Loop restart via Play
// - Per-frame Update dispatch
// - Volume/priority readback to VCB
//
// a1 = audSoundManager*
// Writes activeCount to a1+16.
//
// This is the most complex function with 12 vtable dispatch sites.
// Every dispatch is guarded by vtable null checks for race-condition safety.

PPC_FUNC_HOOK(sub_8291DF00) {
    uint32_t self = ctx.r3.u32;
    int32_t activeCount = 0;
    uint32_t pool = GetPoolBase(base);
    uint32_t totalSlots = PPC_LOAD_U32(pool + POOL_OFF_COUNT);

    // Spin-wait while pool lock flag is set
    while (PPC_LOAD_U8(ADDR_POOL_LOCK_FLAG) != 0) {
        ctx.r3.u32 = 0;
        __imp__sub_82849918(ctx, base); // SleepEx(0) - yield
    }

    uint32_t mixerDevice = ADDR_MIXER_DEVICE;

    for (uint32_t i = 0, entityOff = 0; i < totalSlots; i++, entityOff += ENTITY_STRIDE) {
        // Check if slot is allocated (bit7 of flag byte clear = in use)
        uint32_t flagsPtr = PPC_LOAD_U32(pool + POOL_OFF_FLAGS);
        uint8_t flag = PPC_LOAD_U8(flagsPtr + i);
        if (flag & 0x80)
            continue; // slot not in use

        // Get entity pointer
        uint32_t dataPtr = PPC_LOAD_U32(pool + POOL_OFF_DATA);
        uint32_t entityAddr = dataPtr + entityOff;
        if (entityAddr == 0)
            continue;

        // Read sound params pointer
        uint32_t soundParams = PPC_LOAD_U32(entityAddr + ENT_OFF_SOUND_PARAMS);
        uint8_t categoryId = PPC_LOAD_U8(soundParams + SP_OFF_CATEGORY);

        // GetVoiceSlotIndex
        ctx.r3.u32 = mixerDevice;
        ctx.r4.u32 = categoryId;
        ctx.r5.u32 = soundParams;
        __imp__sub_829222E0(ctx, base);
        int32_t voiceSlotIdx = (int32_t)ctx.r3.u32;

        // Compute VCB address
        uint32_t vcbAddr = (455 * (uint32_t)categoryId + (uint32_t)voiceSlotIdx + 339)
                           * VCB_SIZE + GetVoiceSlotBase(base);

        // Check VCB state: if bits[31:30] == 0b10 (0x80000000), sound is active
        uint32_t stateFlags = PPC_LOAD_U32(vcbAddr + VCB_OFF_STATE_FLAGS);
        bool isActiveState = (stateFlags & 0xC0000000) == 0x80000000;

        if (isActiveState) {
            // === DISPATCH SITE 1: IsFinished (vtable[5], +0x14) ===
            uint32_t vtable = PPC_LOAD_U32(entityAddr + ENT_OFF_VTABLE);
            if (vtable == 0) continue;
            uint32_t isFinishedFn = PPC_LOAD_U32(vtable + 20);
            if (isFinishedFn == 0) continue;
            ctx.r3.u32 = entityAddr;
            PPC_CALL_INDIRECT_FUNC(isFinishedFn);
            bool isFinished = (ctx.r3.u32 & 0xFF) == 0;

            if (isFinished) {
                // Sound finished playing - tear it down
                // Set slot index to 0xFF (invalid)
                uint32_t sp = PPC_LOAD_U32(entityAddr + ENT_OFF_SOUND_PARAMS);
                PPC_STORE_U8(sp + SP_OFF_SLOT_IDX, 0xFF);

                // === DISPATCH SITE 2: Stop (vtable[2], +0x08) ===
                vtable = PPC_LOAD_U32(entityAddr + ENT_OFF_VTABLE);
                if (vtable != 0) {
                    uint32_t stopFn = PPC_LOAD_U32(vtable + 8);
                    if (stopFn != 0) {
                        ctx.r3.u32 = entityAddr;
                        PPC_CALL_INDIRECT_FUNC(stopFn);
                    }
                }

                // === DISPATCH SITE 3: SetState(entity, 1) (vtable[0], +0x00) ===
                vtable = PPC_LOAD_U32(entityAddr + ENT_OFF_VTABLE);
                if (vtable != 0) {
                    uint32_t setStateFn = PPC_LOAD_U32(vtable + 0);
                    if (setStateFn != 0) {
                        ctx.r3.u32 = entityAddr;
                        ctx.r4.u32 = 1;
                        PPC_CALL_INDIRECT_FUNC(setStateFn);
                    }
                }

                // Clear VCB state bits
                stateFlags = PPC_LOAD_U32(vcbAddr + VCB_OFF_STATE_FLAGS);
                PPC_STORE_U32(vcbAddr + VCB_OFF_STATE_FLAGS, stateFlags & 0x3FFFFFFF);

                // Check loop flag (bit0 of flags24)
                uint8_t f24 = PPC_LOAD_U8(soundParams + SP_OFF_FLAGS24);
                if (f24 & 0x01) {
                    // Loop: release voice slot and continue to next entity
                    MarkVoiceSlotForRelease(base, mixerDevice, categoryId, (uint8_t)voiceSlotIdx);
                }
                continue;
            }
        }

        // === Entity is actively playing - process per-frame updates ===
        activeCount++;
        uint8_t f24 = PPC_LOAD_U8(soundParams + SP_OFF_FLAGS24);

        // --- Handle seek request (bit3 of flags24) ---
        if (f24 & 0x08) {
            int32_t seekPos;
            uint8_t f25 = PPC_LOAD_U8(soundParams + SP_OFF_FLAGS25);
            if (f25 & 0x80) {
                // Read seek position from VCB
                seekPos = (int32_t)PPC_LOAD_U32(vcbAddr + VCB_OFF_VOL_FLAGS) >> 3;
            } else {
                seekPos = 0;
            }

            // === DISPATCH SITE 4: Seek (vtable[7], +0x1C) ===
            uint32_t vtable = PPC_LOAD_U32(entityAddr + ENT_OFF_VTABLE);
            if (vtable != 0) {
                uint32_t seekFn = PPC_LOAD_U32(vtable + 28);
                if (seekFn != 0) {
                    ctx.r3.u32 = entityAddr;
                    ctx.r4.s32 = seekPos;
                    PPC_CALL_INDIRECT_FUNC(seekFn);
                }
            }

            // Clear seek flags
            f24 = PPC_LOAD_U8(soundParams + SP_OFF_FLAGS24);
            uint8_t f25_new = PPC_LOAD_U8(soundParams + SP_OFF_FLAGS25);
            PPC_STORE_U8(soundParams + SP_OFF_FLAGS25, f25_new & 0x7F);
            PPC_STORE_U8(soundParams + SP_OFF_FLAGS24, f24 & 0xF7);
        }

        // --- Handle volume dirty (bit1 of flags24) ---
        f24 = PPC_LOAD_U8(soundParams + SP_OFF_FLAGS24);
        if (f24 & 0x02) {
            // Check entity flags: if entity+0x88 bit3 is set, only update if flags24 bit2 is also set
            uint8_t entityFlags = PPC_LOAD_U8(entityAddr + ENT_OFF_ENTITY_FLAGS);
            bool shouldUpdate = true;
            if (entityFlags & 0x08) {
                // Entity has restriction flag - only proceed if force flag (bit2) is set
                if (!(f24 & 0x04)) {
                    shouldUpdate = false;
                }
            }

            if (shouldUpdate) {
                int32_t vol = (int32_t)PPC_LOAD_U32(vcbAddr + VCB_OFF_VOL_FLAGS) >> 3;
                if (vol != -1) {
                    // === DISPATCH SITE 5: UpdateVolume (vtable[3], +0x0C) ===
                    uint32_t vtable = PPC_LOAD_U32(entityAddr + ENT_OFF_VTABLE);
                    if (vtable != 0) {
                        uint32_t volFn = PPC_LOAD_U32(vtable + 12);
                        if (volFn != 0) {
                            ctx.r3.u32 = entityAddr;
                            ctx.r4.s32 = vol;
                            PPC_CALL_INDIRECT_FUNC(volFn);
                        }
                    }
                }

                // Clear volDirty flag
                f24 = PPC_LOAD_U8(soundParams + SP_OFF_FLAGS24);
                PPC_STORE_U8(soundParams + SP_OFF_FLAGS24, f24 & 0xFD);
            }
        }

        // --- Handle loop restart (bit0 of flags24) ---
        f24 = PPC_LOAD_U8(soundParams + SP_OFF_FLAGS24);
        if (f24 & 0x01) {
            // Clear volDirty before play
            PPC_STORE_U8(soundParams + SP_OFF_FLAGS24, f24 & 0xFD);

            // === DISPATCH SITE 6: Play (vtable[4], +0x10) ===
            uint32_t vtable = PPC_LOAD_U32(entityAddr + ENT_OFF_VTABLE);
            if (vtable != 0) {
                uint32_t playFn = PPC_LOAD_U32(vtable + 16);
                if (playFn != 0) {
                    ctx.r3.u32 = entityAddr;
                    PPC_CALL_INDIRECT_FUNC(playFn);
                }
            }
        }

        // === DISPATCH SITE 7: Update (vtable[10], +0x28) ===
        {
            uint32_t vtable = PPC_LOAD_U32(entityAddr + ENT_OFF_VTABLE);
            if (vtable != 0) {
                uint32_t updateFn = PPC_LOAD_U32(vtable + 40);
                if (updateFn != 0) {
                    ctx.r3.u32 = entityAddr;
                    PPC_CALL_INDIRECT_FUNC(updateFn);
                }
            }
        }

        // --- Post-update: read volume back to VCB if state is pending/stopping ---
        stateFlags = PPC_LOAD_U32(vcbAddr + VCB_OFF_STATE_FLAGS);
        uint32_t state = stateFlags >> 30;
        if ((state == 1 || state == 3)) {
            // Only update volume if entity doesn't have restriction, or force flag is set
            uint8_t entityFlags = PPC_LOAD_U8(entityAddr + ENT_OFF_ENTITY_FLAGS);
            bool canUpdate = true;
            if (entityFlags & 0x08) {
                f24 = PPC_LOAD_U8(soundParams + SP_OFF_FLAGS24);
                if (!(f24 & 0x04)) canUpdate = false;
            }

            if (canUpdate) {
                // === DISPATCH SITE 8: GetVolume (vtable[9], +0x24) ===
                uint32_t vtable = PPC_LOAD_U32(entityAddr + ENT_OFF_VTABLE);
                if (vtable != 0) {
                    uint32_t getVolFn = PPC_LOAD_U32(vtable + 36);
                    if (getVolFn != 0) {
                        ctx.r3.u32 = entityAddr;
                        PPC_CALL_INDIRECT_FUNC(getVolFn);
                        int32_t volume = ctx.r3.s32;
                        uint32_t volFlags = PPC_LOAD_U32(vcbAddr + VCB_OFF_VOL_FLAGS);
                        PPC_STORE_U32(vcbAddr + VCB_OFF_VOL_FLAGS,
                                      ((uint32_t)(volume << 3)) | (volFlags & 0x7));
                    }
                }
            }
        }

        // --- Update priority byte in VCB ---
        // === DISPATCH SITE 9: GetPriority (vtable[6], +0x18) ===
        {
            uint32_t vtable = PPC_LOAD_U32(entityAddr + ENT_OFF_VTABLE);
            if (vtable != 0) {
                uint32_t getPrioFn = PPC_LOAD_U32(vtable + 24);
                if (getPrioFn != 0) {
                    ctx.r3.u32 = entityAddr;
                    PPC_CALL_INDIRECT_FUNC(getPrioFn);
                    uint8_t priority = (uint8_t)(ctx.r3.u32 & 0xFF);
                    uint8_t prioByte = PPC_LOAD_U8(vcbAddr + VCB_OFF_PRIORITY);
                    PPC_STORE_U8(vcbAddr + VCB_OFF_PRIORITY, (priority << 7) | (prioByte & 0x7F));
                }
            }
        }

        // --- Final IsFinished check for loop sounds ---
        f24 = PPC_LOAD_U8(soundParams + SP_OFF_FLAGS24);
        if (f24 & 0x01) {
            // === DISPATCH SITE 10: IsFinished (vtable[5], +0x14) ===
            uint32_t vtable = PPC_LOAD_U32(entityAddr + ENT_OFF_VTABLE);
            if (vtable == 0) continue;
            uint32_t isFinFn = PPC_LOAD_U32(vtable + 20);
            if (isFinFn == 0) continue;
            ctx.r3.u32 = entityAddr;
            PPC_CALL_INDIRECT_FUNC(isFinFn);

            if (ctx.r3.u32 & 0xFF) {
                // Not finished - continue to next entity
                continue;
            }

            // Sound finished while looping - tear down
            uint32_t sp = PPC_LOAD_U32(entityAddr + ENT_OFF_SOUND_PARAMS);
            PPC_STORE_U8(sp + SP_OFF_SLOT_IDX, 0xFF);

            // === DISPATCH SITE 11: Stop (vtable[2], +0x08) ===
            vtable = PPC_LOAD_U32(entityAddr + ENT_OFF_VTABLE);
            if (vtable != 0) {
                uint32_t stopFn = PPC_LOAD_U32(vtable + 8);
                if (stopFn != 0) {
                    ctx.r3.u32 = entityAddr;
                    PPC_CALL_INDIRECT_FUNC(stopFn);
                }
            }

            // === DISPATCH SITE 12: SetState(entity, 1) (vtable[0], +0x00) ===
            vtable = PPC_LOAD_U32(entityAddr + ENT_OFF_VTABLE);
            if (vtable != 0) {
                uint32_t setStateFn = PPC_LOAD_U32(vtable + 0);
                if (setStateFn != 0) {
                    ctx.r3.u32 = entityAddr;
                    ctx.r4.u32 = 1;
                    PPC_CALL_INDIRECT_FUNC(setStateFn);
                }
            }

            // Clear VCB state
            stateFlags = PPC_LOAD_U32(vcbAddr + VCB_OFF_STATE_FLAGS);
            PPC_STORE_U32(vcbAddr + VCB_OFF_STATE_FLAGS, stateFlags & 0x3FFFFFFF);

            // Release voice slot
            MarkVoiceSlotForRelease(base, mixerDevice, categoryId, (uint8_t)voiceSlotIdx);
        }
    }

    // Store active count and call PostProcess
    PPC_STORE_U32(self + 16, (uint32_t)activeCount);

    // sub_82902328: PostProcess (calls sub_8291F270 + increments frameCounter)
    __imp__sub_82902328(ctx, base);
}

// =============================================================================
// sub_8291E620 - audSoundManager::Update
// =============================================================================
// Main update entry point. Computes delta time, builds dynamic/static sound
// lists, processes pending activations, then runs the pool iteration.
// a1 = audSoundManager*, a2 = currentTime

PPC_FUNC_HOOK(sub_8291E620) {
    uint32_t self = ctx.r3.u32;
    uint32_t currentTime = ctx.r4.u32;

    uint32_t field0C = PPC_LOAD_U32(self + 12);
    uint32_t lastTime = PPC_LOAD_U32(self + 32);
    uint32_t deltaTime = currentTime - lastTime;

    uint32_t pool = GetPoolBase(base);
    uint32_t poolFree = PPC_LOAD_U32(pool + POOL_OFF_FREE);

    uint32_t typePtr = PPC_LOAD_U32(self + 0);

    // Store current time
    PPC_STORE_U32(self + 32, currentTime);

    // Allocate PPC stack frame (160 bytes, matching original stwu r1,-160(r1))
    // The helper functions PrepareUpdateLists and BuildUpdateLists write results
    // into this stack frame, so we must set it up on the PPC stack.
    //
    // Stack layout:
    //   sp+80  (+0x50): typePtr (uint32, passed by ref to PrepareUpdateLists)
    //   sp+84  (+0x54): secondaryGlobal addr (uint32, passed by ref)
    //   sp+96  (+0x60): listData[8 bytes] (output from PrepareUpdateLists)
    //   sp+104 (+0x68): staticCount (uint16, output from BuildUpdateLists)
    //   sp+106 (+0x6A): dynamicCount (uint16, output from BuildUpdateLists)
    //   sp+108 (+0x6C): padding (uint16, initialized to 0)
    uint32_t oldSp = ctx.r1.u32;
    uint32_t sp = oldSp - 160;
    PPC_STORE_U32(sp, oldSp); // save backchain
    ctx.r1.u32 = sp;

    PPC_STORE_U16(sp + 108, 0); // clear padding

    // Original stores the ADDRESS 0x831C9440 into sp+84 (lis+addi, not lwz)
    PPC_STORE_U32(sp + 84, ADDR_SECONDARY_GLOBAL);
    PPC_STORE_U32(sp + 80, typePtr);

    // ComputeDeltaTime(deltaTime)
    ctx.r3.u32 = deltaTime;
    __imp__sub_82931EB8(ctx, base);

    // PrepareUpdateLists(&sp[80], &sp[84], &sp[96])
    // This function reads/writes through the pointers to resolve type/secondary ptrs
    ctx.r3.u32 = sp + 80;
    ctx.r4.u32 = sp + 84;
    ctx.r5.u32 = sp + 96;
    __imp__sub_82931700(ctx, base);

    // BuildUpdateLists(sp[84], sp[80], poolFree, field0C, &sp[96])
    // After PrepareUpdateLists, sp+84 and sp+80 may hold updated values
    ctx.r3.u32 = PPC_LOAD_U32(sp + 84);
    ctx.r4.u32 = PPC_LOAD_U32(sp + 80);
    ctx.r5.u32 = poolFree;
    ctx.r6.u32 = field0C;
    ctx.r7.u32 = sp + 96;
    __imp__sub_82931B98(ctx, base);

    // Process dynamic sound activations
    // dynamicCount is at sp+106 (written by BuildUpdateLists)
    uint16_t dynamicCount = PPC_LOAD_U16(sp + 106);
    if (dynamicCount > 0) {
        uint32_t dynamicListPtr = PPC_LOAD_U32(ADDR_DYNAMIC_LIST);
        for (uint32_t j = 0; j < dynamicCount; j++) {
            uint16_t handle = PPC_LOAD_U16(dynamicListPtr + j * 2);
            ctx.r3.u32 = self;
            ctx.r4.u32 = handle;
            __imp__sub_8291E260(ctx, base); // UpdateDynamicSound
        }
    }

    // Process static sound activations
    // staticCount is at sp+104 (written by BuildUpdateLists)
    uint16_t staticCount = PPC_LOAD_U16(sp + 104);
    if (staticCount > 0) {
        uint32_t staticListPtr = PPC_LOAD_U32(ADDR_STATIC_LIST);
        for (uint32_t j = 0; j < staticCount; j++) {
            uint16_t handle = PPC_LOAD_U16(staticListPtr + j * 2);
            ctx.r3.u32 = self;
            ctx.r4.u32 = handle;
            __imp__sub_8291E538(ctx, base); // ActivateStaticSound
        }
    }

    // Run main pool iteration (Process)
    ctx.r3.u32 = self;
    __imp__sub_8291DF00(ctx, base); // CPool::Process

    // Restore PPC stack
    ctx.r1.u32 = oldSp;
}
