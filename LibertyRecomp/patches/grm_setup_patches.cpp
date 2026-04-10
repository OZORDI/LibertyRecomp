// =============================================================================
// rage::grmSetup Singleton System - Decompiled Dispatch Hooks
// =============================================================================
// Class hierarchy: rage::datBase -> rage::grcSetup -> rage::grmSetup
//
// The grmSetup singleton manages the RAGE graphics setup lifecycle:
// - Device initialization and teardown
// - Frame timing via exponential moving average of timebase deltas
// - V-sync state machine (uninit -> pending -> ready)
// - Screenshot path and user name storage
//
// Vtable at 0x82000990 (7 slots):
//   [0] sub_821B3A20  - datBase destructor (scalar deleting)
//   [1] sub_828C5ED8  - InitDevice
//   [2] sub_828C5F90  - BeginScene (thunk -> sub_828C59E0)
//   [3] sub_828C5A98  - UpdateTimingRaw  (compute raw delta EMA)
//   [4] sub_828C5B08  - BeginFrame       (v-sync + timebase snap)
//   [5] sub_828C5BA0  - EndFrame         (present delta EMA + present params)
//   [6] sub_828C5F10  - Shutdown         (release resources)
//
// Singleton pointer: 0x82B29EEC (sm_Instance)
//
// Dispatch thunks (called by game code, read vtable, dispatch indirectly):
//   sub_821B38D8  - dispatches slot[2] (BeginScene) with GPU marker bracket
//   sub_821B3958  - dispatches slot[3] (UpdateTimingRaw)
//   sub_821B3970  - dispatches slot[4] (BeginFrame) with arg=0
//   sub_821B3990  - dispatches slot[5] (EndFrame)
//   sub_821B39A8  - dispatches sub_828C5C78 (GPU device check) with fallback
//
// Construction: sub_821B3CE8 (called once during engine boot)
// Destruction:  sub_821B3FF0 (called during engine shutdown)
// =============================================================================

#include <api/Liberty.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>
#include <cstring>

// =============================================================================
// rage::grmSetup Struct Layout (472 bytes / 0x1D8)
// =============================================================================
// Derived from constructor sub_828C5CC0 + field access patterns in vtable fns.
//
// +0x000 (  0)  uint32_t       vtable
// +0x004 (  4)  uint32_t       clearColor          (ARGB, init 0xFF000000)
// +0x008 (  8)  uint64_t       timebaseInit        (snapshot at construction)
// +0x010 ( 16)  uint64_t       timebaseFrame       (per-frame snapshot)
// +0x018 ( 24)  uint64_t       timebasePresent     (present-loop snapshot)
// +0x020 ( 32)  float          smoothedDeltaRaw    (EMA of raw delta, seconds)
// +0x024 ( 36)  float          smoothedDeltaPresent(EMA of present delta, secs)
// +0x028 ( 40)  float          smoothedDeltaScene  (EMA of scene delta, seconds)
// +0x02C ( 44)  float          smoothedDeltaFence  (fourth timing float)
// +0x030 ( 48)  uint8_t        bUnknown30
// +0x031 ( 49)  uint8_t        pad_31
// +0x032 ( 50)  uint8_t        bUnknown32
// +0x033 ( 51)  uint8_t        bUnknown33
// +0x034 ( 52)  uint8_t        bScreenshotFmtValid
// +0x035 ( 53)  uint8_t        bNotWidescreen
// +0x036 ( 54)  uint8_t[2]     pad_36
// +0x038 ( 56)  int32_t        screenshotIndex     (init 1)
// +0x03C ( 60)  int32_t        field_3C            (init 0)
// +0x040 ( 64)  char[256]      screenshotPath      ("c:/Screenshot")
// +0x140 (320)  uint8_t        bVsyncEnabled
// +0x141 (321)  uint8_t[3]     pad_141
// +0x144 (324)  float          clearR
// +0x148 (328)  float          clearG
// +0x14C (332)  float          clearB
// +0x150 (336)  char[64]       userName            ("Unknown")
// +0x190 (400)  char[64]       displayName         ("Unknown")
// +0x1D0 (464)  uint8_t        initState           (0=uninit,1=pending,2=ready)
// +0x1D1 (465)  uint8_t[7]     pad_1D1
// Total: 472 bytes (0x1D8)
//
// Timing constants (Xbox 360 timebase):
//   ticksToSeconds = 0.000020050125 (1 / 49,875,000 Hz)
//   EMA weights: old=63/64 (0.984375), new=1/64 (0.015625)
//   Max acceptable frame delta: 1.0 second
// =============================================================================

// =============================================================================
// Address Constants
// =============================================================================

// Singleton pointer: grmSetup* sm_Instance
static constexpr uint32_t ADDR_GRM_SETUP_INSTANCE = 0x82B29EEC;

// Vtable address for rage::grmSetup
static constexpr uint32_t ADDR_GRM_SETUP_VTABLE   = 0x82000990;

// grcSetup vtable (parent class, used in destructor path)
static constexpr uint32_t ADDR_GRC_SETUP_VTABLE   = 0x82093B28;

// GPU marker context: dword_82BEFA40 base, dword_82BEFA50 = *(base+16)
static constexpr uint32_t ADDR_GPU_MARKER_BASE     = 0x82BEFA40;
static constexpr uint32_t ADDR_GPU_MARKER_HANDLE   = 0x82BEFA50;

// GPU device state global (sub_828E7808/sub_828E7818 access)
// Used by sub_828C5C78 to test if GPU device is available

// Fallback comparison value in sub_821B39A8
static constexpr uint32_t ADDR_GFX_BACKEND_STATE   = 0x82A97CF4;

// Object field offsets (see rage::grcSetup in RAGE.h for full layout)
static constexpr uint32_t OFF_VTABLE        = 0x000;
static constexpr uint32_t OFF_CLEAR_COLOR   = 0x004;
static constexpr uint32_t OFF_TB_INIT       = 0x008;
static constexpr uint32_t OFF_TB_FRAME      = 0x010;
static constexpr uint32_t OFF_TB_PRESENT    = 0x018;
static constexpr uint32_t OFF_DELTA_RAW     = 0x020;
static constexpr uint32_t OFF_DELTA_PRESENT = 0x024;
static constexpr uint32_t OFF_DELTA_SCENE   = 0x028;
static constexpr uint32_t OFF_DELTA_FENCE   = 0x02C;
static constexpr uint32_t OFF_INIT_STATE    = 0x1D0;

// Globals managed by InitDevice chain
static constexpr uint32_t ADDR_GRC_TEXTURE_FACTORY   = 0x831C2F88;
static constexpr uint32_t ADDR_GRC_TEXTURE_FACTORY2  = 0x831C5E08;
static constexpr uint32_t ADDR_GRC_EFFECT_FACTORY    = 0x831C513C;
static constexpr uint32_t ADDR_GRC_RENDERSTATE_FACTORY = 0x831C51CC;
static constexpr uint32_t ADDR_GRC_DEVICE            = 0x831C2DA8;

// =============================================================================
// Helper: Validate grmSetup pointer
// =============================================================================
// After RAGE heap reuse, the singleton memory may be recycled and the vtable
// pointer overwritten. We guard every indirect dispatch by checking that
// the vtable pointer falls within the RAGE .rdata section range.
//
// Must accept BOTH grmSetup vtable (0x82000990) AND parent grcSetup vtable
// (0x82093B28) which is temporarily set during construction before being
// overwritten. An exact-match check was too aggressive and blocked the
// shader factory vtable dispatch during init.

static inline bool grmSetup_IsValid(uint32_t instanceAddr, uint8_t* base)
{
    if (instanceAddr == 0)
        return false;

    uint32_t vtablePtr = PPC_LOAD_U32(instanceAddr + OFF_VTABLE);
    return (vtablePtr >= 0x82000400 && vtablePtr < 0x82107068);
}

// =============================================================================
// sub_821B38D8 - BeginScene Dispatch (vtable slot[2])
// =============================================================================
// Called 2x (by sub_82140088, sub_821428C8).
// Brackets the vtable[2] call with GPU performance markers if a marker handle
// is active. Reads the singleton, dispatches through vtable slot 2.
//
// Pseudocode:
//   markerHandle = *(ADDR_GPU_MARKER_BASE + 16);
//   if (markerHandle) GPU_MarkerBegin(&markerCtx, markerHandle);
//   sm_Instance->vtable[2](sm_Instance);  // BeginScene
//   if (markerHandle) GPU_MarkerEnd(&markerCtx);
//   sub_8227FE40();  // post-scene
//   sub_821B4768();  // flip/present
//
PPC_FUNC_IMPL(__imp__sub_821B38D8);
PPC_FUNC_HOOK(sub_821B38D8)
{
    uint32_t inst = PPC_LOAD_U32(ADDR_GRM_SETUP_INSTANCE);
    if (!grmSetup_IsValid(inst, base))
    {
        // Singleton destroyed or vtable corrupted - skip dispatch entirely.
        // We cannot dispatch into Xenon GPU objects (no GPU emulation).
        ctx.r3.u64 = 0;
        return;
    }

    __imp__sub_821B38D8(ctx, base);
}

// =============================================================================
// sub_821B3958 - UpdateTimingRaw Dispatch (vtable slot[3])
// =============================================================================
// Thin dispatch: sm_Instance->vtable[3](sm_Instance)
// This computes the EMA-smoothed raw frame delta from timebase differences.
//
PPC_FUNC_IMPL(__imp__sub_821B3958);
PPC_FUNC_HOOK(sub_821B3958)
{
    uint32_t inst = PPC_LOAD_U32(ADDR_GRM_SETUP_INSTANCE);
    if (!grmSetup_IsValid(inst, base))
    {
        // Safety: if singleton is dead, skip the dispatch entirely.
        // The original code would crash reading a corrupted vtable.
        return;
    }

    __imp__sub_821B3958(ctx, base);
}

// =============================================================================
// sub_821B3970 - BeginFrame Dispatch (vtable slot[4], arg=0)
// =============================================================================
// Thin dispatch: sm_Instance->vtable[4](sm_Instance, 0)
// Calls BeginFrame with shouldPresent=false. Manages the v-sync state machine
// transition (uninit->pending->ready) and snapshots the frame timebase.
//
PPC_FUNC_IMPL(__imp__sub_821B3970);
PPC_FUNC_HOOK(sub_821B3970)
{
    uint32_t inst = PPC_LOAD_U32(ADDR_GRM_SETUP_INSTANCE);
    if (!grmSetup_IsValid(inst, base))
        return;

    __imp__sub_821B3970(ctx, base);
}

// =============================================================================
// sub_821B3990 - EndFrame Dispatch (vtable slot[5])
// =============================================================================
// Thin dispatch: sm_Instance->vtable[5](sm_Instance)
// Computes the EMA-smoothed present delta and builds present parameters.
// If initState == 2 (ready), passes present params to sub_828C15C8.
//
PPC_FUNC_IMPL(__imp__sub_821B3990);
PPC_FUNC_HOOK(sub_821B3990)
{
    uint32_t inst = PPC_LOAD_U32(ADDR_GRM_SETUP_INSTANCE);
    if (!grmSetup_IsValid(inst, base))
        return;

    __imp__sub_821B3990(ctx, base);
}

// =============================================================================
// sub_821B39A8 - GPU Device Ready Check
// =============================================================================
// Calls sub_828C5C78 (which tests the GPU device via sub_828E7808/sub_828E7818).
// If the GPU reports not-ready, falls back to checking whether
// *(0x82A97CF4) == 16 (DirectX backend state == D3D_READY).
//
// Returns: bool (1 = device ready, 0 = not ready)
//
PPC_FUNC_IMPL(__imp__sub_821B39A8);
PPC_FUNC_HOOK(sub_821B39A8)
{
    uint32_t inst = PPC_LOAD_U32(ADDR_GRM_SETUP_INSTANCE);
    if (inst == 0)
    {
        // No singleton - report not ready
        ctx.r3.u64 = 0;
        return;
    }

    __imp__sub_821B39A8(ctx, base);
}

// =============================================================================
// grcSetup initState State Machine - Complete Decompilation
// =============================================================================
//
// The initState field at offset +0x1D0 (464) controls the graphics device
// lifecycle. On Xbox 360, the GPU device-ready callback drives 0->1. Since the
// recompilation has no GPU device, we force the transition in the construction
// hook so that BeginFrame can promote 1->2 on the first present-enabled call.
//
// STATE 0 (UNINIT):
//   Set by sub_828C58D0 (ConfigureDevice) during construction.
//
// STATE 1 (PENDING):
//   On Xbox 360: set by the D3D device-ready callback (not present in recomp).
//   On recomp: forced to 1 after construction in the sub_821B3CE8 hook.
//
// STATE 2 (READY):
//   Set by sub_828C5B08 (BeginFrame, vtable[4]) when shouldPresent is non-zero
//   AND initState was 1 after calling sub_828BF120. Once READY, EndFrame passes
//   present parameters to sub_828C15C8.
//
// Functions that READ initState:
//   sub_828C5B08  BeginFrame  - checks !=2 to gate first present, ==1 to promote
//   sub_828C5BA0  EndFrame    - checks ==2 to decide present params vs null
//
// Functions that WRITE initState:
//   sub_828C58D0  ConfigureDevice   - writes 0 (construction)
//   sub_828C5B08  BeginFrame        - writes 2 (promotion from 1)
//   [GPU callback]                  - writes 1 (device ready, Xbox 360 only)
//
// Decompiled vtable functions (run as recompiled code, passthrough hooks):
//
//   grcSetup::UpdateTimingRaw [vtable[3], sub_828C5A98]:
//     rawDelta = (mftb() - timebaseInit) * ticksToSeconds
//     EMA: smoothedDeltaRaw = old*(63/64) + rawDelta*(1/64), reset on spike>1.0
//
//   grcSetup::BeginFrame(shouldPresent) [vtable[4], sub_828C5B08]:
//     sub_828BF898(); if shouldPresent && initState!=2: sub_828BF120(...)
//     if initState==1: initState=2; timebaseFrame = mftb()
//
//   grcSetup::EndFrame [vtable[5], sub_828C5BA0]:
//     EMA: smoothedDeltaPresent from (mftb() - timebaseFrame) * ticksToSeconds
//     if initState==2: sub_828C15C8(&presentParams) else sub_828C15C8(nullptr)
//
//   grcSetup::BeginScene [vtable[2], sub_828C59E0 via sub_828C5F90]
//   grmSetup::InitDevice [vtable[1], sub_828C5ED8]
//   grmSetup::Shutdown   [vtable[6], sub_828C5F10]
// =============================================================================

// DISABLED: 6 vtable target hooks blocked shader factory creation during init.
// The hooks' vtable validation guards ran during construction when the object
// still had the grcSetup vtable (before overwrite to grmSetup). This prevented
// sub_828C5ED8 (InitDevice) from creating grcEffectFactory (0x831C513C) and
// grcRenderStateFactory (0x831C51CC). The 5 dispatch thunk hooks above provide
// all necessary validation. All vtable targets run as recompiled code.
#if 0
PPC_FUNC_IMPL(__imp__sub_828C5ED8);
PPC_FUNC_HOOK(sub_828C5ED8) { __imp__sub_828C5ED8(ctx, base); }

PPC_FUNC_IMPL(__imp__sub_828C5F90);
PPC_FUNC_HOOK(sub_828C5F90) { __imp__sub_828C5F90(ctx, base); }

PPC_FUNC_IMPL(__imp__sub_828C5A98);
PPC_FUNC_HOOK(sub_828C5A98) { __imp__sub_828C5A98(ctx, base); }

PPC_FUNC_IMPL(__imp__sub_828C5B08);
PPC_FUNC_HOOK(sub_828C5B08) { __imp__sub_828C5B08(ctx, base); }

PPC_FUNC_IMPL(__imp__sub_828C5BA0);
PPC_FUNC_HOOK(sub_828C5BA0) { __imp__sub_828C5BA0(ctx, base); }

PPC_FUNC_IMPL(__imp__sub_828C5F10);
PPC_FUNC_HOOK(sub_828C5F10) { __imp__sub_828C5F10(ctx, base); }
#endif

// =============================================================================
// sub_821B3CE8 - grmSetup System Construction (engine boot)
// =============================================================================
// Called once from sub_82140000 during engine startup. Allocates the 472-byte
// grmSetup object, constructs it, configures shaders, resolution, and the
// default scheduler.
//
// Key operations:
//   1. Set global debug flags
//   2. Call sub_8284CFD8(383, 2, 0) - allocate GPU resources
//   3. Set display resolution defaults (1280x720 if not already set)
//   4. Check widescreen support via sub_828BF708
//   5. Allocate 472 bytes, call sub_828C5CC0 (grcSetup constructor)
//   6. Write grmSetup vtable over grcSetup vtable
//   7. Store in sm_Instance (0x82B29EEC)
//   8. Configure shader path, call InitDevice (vtable[1])
//   9. Set up default scheduler ("gtaDefSched")
//
// The constructor sub_828C5CC0 initializes:
//   - vtable = grcSetup vtable (overwritten to grmSetup vtable after return)
//   - clearColor = -1 (0xFFFFFFFF)
//   - Three timebase snapshots via mftb
//   - screenshotPath = "c:/Screenshot"
//   - userName = "Unknown", displayName = "Unknown"
//   - Various bool flags from global config
//
// Then sub_828C58D0 overwrites clearColor to 0xFF000000 and initState to 0.
//
PPC_FUNC_IMPL(__imp__sub_821B3CE8);
PPC_FUNC_HOOK(sub_821B3CE8)
{
    __imp__sub_821B3CE8(ctx, base);

    uint32_t inst = PPC_LOAD_U32(ADDR_GRM_SETUP_INSTANCE);
    if (inst != 0)
    {
        uint8_t state = PPC_LOAD_U8(inst + OFF_INIT_STATE);

        // On Xbox 360, the GPU device-ready callback sets initState from 0 to 1
        // after D3D initialization completes. In the recompilation there is no GPU
        // device and no callback fires, so initState stays 0 forever. This blocks
        // BeginFrame from promoting to state 2 (READY), which in turn blocks
        // EndFrame from passing present parameters to sub_828C15C8.
        //
        // Force the 0 -> 1 (UNINIT -> PENDING) transition here so that the first
        // BeginFrame call with shouldPresent=true can promote to 2 (READY).
        if (state == 0)
        {
            PPC_STORE_U8(inst + OFF_INIT_STATE, 1);
            LOGF_INFO("grmSetup: forced initState 0->1 (PENDING) at guest 0x{:08X}", inst);
        }

        uint32_t vtbl = PPC_LOAD_U32(inst + OFF_VTABLE);
        state = PPC_LOAD_U8(inst + OFF_INIT_STATE);
        LOGF_INFO("grmSetup: constructed at guest 0x{:08X}, vtable=0x{:08X}, initState={}",
                  inst, vtbl, state);
    }
    else
    {
        LOG_WARNING("grmSetup: construction failed (sm_Instance is null)");
    }
}

// =============================================================================
// sub_821B3FF0 - grmSetup System Destruction (engine shutdown)
// =============================================================================
// Called once during engine teardown.
//
// 1. If dword_831E55EC is valid (scheduler?), destroy it
// 2. Call sub_8235CA28 + sub_821B4100 (cleanup helpers)
// 3. If sm_Instance is non-null:
//    a. Call vtable[6](this) = Shutdown
//    b. Call nullsub (debug breakpoint placeholder)
//    c. Call vtable[0](this, 1) = scalar deleting destructor
//    d. Set sm_Instance = 0
//
PPC_FUNC_IMPL(__imp__sub_821B3FF0);
PPC_FUNC_HOOK(sub_821B3FF0)
{
    uint32_t inst = PPC_LOAD_U32(ADDR_GRM_SETUP_INSTANCE);
    if (inst != 0)
    {
        LOGF_INFO("grmSetup: shutting down instance at guest 0x{:08X}", inst);
    }

    __imp__sub_821B3FF0(ctx, base);

    // Verify cleanup
    uint32_t postInst = PPC_LOAD_U32(ADDR_GRM_SETUP_INSTANCE);
    if (postInst != 0)
    {
        LOGF_WARNING("grmSetup: sm_Instance not cleared after destruction (0x{:08X})", postInst);
    }
}

// =============================================================================
// sub_821B3A20 - datBase Scalar Deleting Destructor (vtable slot[0])
// =============================================================================
// Standard MSVC scalar deleting destructor pattern:
//   1. Call sub_828C5830 (sets vtable back to grcSetup and calls datBase dtor)
//   2. If (flags & 1), free the memory via sub_821B3560
//   3. Return this
//
PPC_FUNC_IMPL(__imp__sub_821B3A20);
PPC_FUNC_HOOK(sub_821B3A20)
{
    __imp__sub_821B3A20(ctx, base);
}
