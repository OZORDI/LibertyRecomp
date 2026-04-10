// =============================================================================
// RAGE Render Phase System - Decompiled from GTA IV Xbox 360
// =============================================================================
//
// The render phase system is RAGE's core rendering dispatch pipeline.
// It manages 37 named render phases (0..36) that control how the GPU
// processes each frame. Game systems (shadows, water, particles, coronas,
// post-fx, etc.) call sub_828C19C0 to configure phases before the frame
// is presented.
//
// Architecture:
//   gRenderPhaseParams[37]  - per-phase parameter storage (0x831C2740)
//   gRenderState[43+]       - GPU render state array (0x82B0D280)
//   gRenderStateDispatch[]  - maps state ID -> grcDevice method offset
//
//   sub_828C19C0 (SetRenderPhase):
//     Stores param at gRenderPhaseParams[phaseId], then maps
//     phaseId -> render state slot(s) and applies via sub_828C8588.
//     571 callers across the engine.
//
//   sub_828C8588 (SetRenderState):
//     Writes value to gRenderState[stateId], dispatches to grcDevice.
//     47 callers, hot function.
//
//   sub_828C15C8 (RenderDispatch):
//     Called from grmSetup::EndFrame. Sets up render targets, clears
//     device, runs multi-viewport loop (if sceneCount>0), or falls
//     through to single-viewport present path.
//
//   sub_828C1228 (RenderScenes):
//     Multi-viewport scene loop. Iterates sceneCount viewports,
//     setting up camera transforms and clear colors per viewport.
//     Only active when g_SceneCount > 0 (unused in SP).
//
//   sub_828C1080 (SetupViewport):
//     Computes viewport rect from pixel coords using render target
//     dimensions, subtracts 0.5 offset, then calls sub_828C0688.
//
//   sub_828C0688 (EmitViewportQuad):
//     Allocates 3-vertex triangle strip via grcDevice::AllocVerts,
//     fills with viewport coordinates and UV mapping, submits.
//
//   sub_821D6210 (InitRenderPhaseDefaults):
//     Called from grmSetup construction. Reads 37 default values
//     from .rdata table at 0x82002430 and sets all phases.
//
// Scene count (0x82B0B48C):
//   Initialized to -1 in .data section. Value -1 means single-viewport
//   mode (standard GTA IV). Values >0 would enable split-screen multi-
//   viewport rendering (unused in single player). Only written back to
//   -1 at end of sub_828C1228.
//
// Call chain:
//   grmSetup::EndFrame (sub_828C5BA0)
//     -> sub_828C15C8 (RenderDispatch)
//        -> sub_828C9980 (SetRenderTarget)
//        -> grcDevice::Clear, SetBlendState, SetSamplerState (x20)
//        -> sub_828C1228 (RenderScenes, if sceneCount>0)
//           -> sub_828C19C0 (SetRenderPhase, phases 7/8/10)
//           -> sub_828C1080 (SetupViewport)
//              -> sub_828C0688 (EmitViewportQuad)
//        -> sub_82A3CC68 (grcDevice::ClearViewport)
//        -> sub_828BF420 (Present/Flip)
// =============================================================================

#include <kernel/function.h>
#include <kernel/memory.h>
#include <cstring>
#include <cstdint>
#include <cmath>

// =============================================================================
// Address Constants (Python-verified from PPC lis/addi patterns)
// =============================================================================

// --- Base: 0x831C0000 (lis -31972) — RAGE graphics system data section ---

// Render phase parameter table: 37 x uint32_t
// lis r11,-31972 (0x831C0000) + addi r11,r11,10048
static constexpr uint32_t ADDR_RENDER_PHASE_PARAMS = 0x831C2740;

// grcDevice pointer
// lis r31,-31972 + lwz r3,8868(r31)
static constexpr uint32_t ADDR_GRC_DEVICE = 0x831C22A4;

// Render dirty/scene flags
static constexpr uint32_t ADDR_RENDER_DIRTY_FLAG = 0x831C3DE4;
static constexpr uint32_t ADDR_RENDER_SCENE_FLAG = 0x831C3DDC;

// Render phase mode storage for phases 1 and 2
static constexpr uint32_t ADDR_PHASE1_MODE = 0x831C27E0;
static constexpr uint32_t ADDR_PHASE2_MODE = 0x831C27E4;

// Viewport/camera globals
// lis r28,-31972 + lwz r11,9160(r28)
static constexpr uint32_t ADDR_VIEWPORT_INDEX = 0x831C23C8;
// addi r29,r10,9192
static constexpr uint32_t ADDR_RENDER_TARGETS = 0x831C23E8;
// addi r11,r11,9304
static constexpr uint32_t ADDR_VIEWPORT_DATA = 0x831C2458;
// Scene camera object
static constexpr uint32_t ADDR_CAMERA_MANAGER = 0x831C2DA8;

// Viewport data base for multi-scene rendering
static constexpr uint32_t ADDR_SCENE_VIEWPORT_BASE = 0x831C22B0;

// render config/state
static constexpr uint32_t ADDR_RENDER_CONFIG = 0x831C22A0;
// render count
static constexpr uint32_t ADDR_RENDER_COUNT = 0x831C2410;
// flip pending flag at 0x831C24A8
static constexpr uint32_t ADDR_FLIP_PENDING = 0x831C24A8;

// Current and default render targets
static constexpr uint32_t ADDR_CURRENT_RT = 0x831C2910;
static constexpr uint32_t ADDR_DEFAULT_RT = 0x831C2D48;

// Render target setup struct
// addi r3,r11,9640 where r11 = 0x831C0000
static constexpr uint32_t ADDR_RT_STRUCT = 0x831C25A8;

// --- Base: 0x82B10000 (lis -32079) — scene/render state data ---

// Render state array: gRenderState[stateId] = value
// lis r11,-32079 (0x82B10000) + addi r11,r11,-11648
static constexpr uint32_t ADDR_RENDER_STATE = 0x82B0D280;

// Render state dispatch offset table: maps stateId -> grcDevice offset
static constexpr uint32_t ADDR_RENDER_STATE_DISPATCH = 0x82B0D8F8;

// Frame counter (incremented each RenderDispatch call)
// lis r11,-32079 + lwz/stw r10,-25324(r11)
static constexpr uint32_t ADDR_FRAME_COUNTER = 0x82B09D14;

// Scene count: -1 = single viewport, >0 = multi-viewport count
// lis r22,-32079 (0x82B10000) + lwz/stw r11,-19316(r22) => 0x82B0B48C
static constexpr uint32_t ADDR_SCENE_COUNT = 0x82B0B48C;

// Back buffer format identifier
static constexpr uint32_t ADDR_BACKBUFFER_FORMAT = 0x82B0B494;
static constexpr uint32_t FMT_A8R8G8B8_CONST    = 438436287; // 0x1A2201BF
static constexpr uint32_t FMT_X8R8G8B8_CONST    = 438436247; // 0x1A220197

// byte flags for depth inversion
static constexpr uint32_t ADDR_DEPTH_INVERT_FLAG = 0x82B0B469;

// Scene info struct (for multi-viewport)
// addi r31,r11,-19180 where r11 = 0x82B10000
static constexpr uint32_t ADDR_SCENE_INFO = 0x82B0B514;

// Render phase saved state (written by sub_821D6210)
static constexpr uint32_t ADDR_RENDER_PHASE_SAVED = 0x82B0B578;

// --- .rdata constants ---

// Render phase defaults (.rdata, 37 x uint32_t)
static constexpr uint32_t ADDR_RENDER_PHASE_DEFAULTS = 0x82002430;

// Render state lookup tables (.rdata) used by SetRenderPhase
static constexpr uint32_t ADDR_LUT_BLEND_SRC     = 0x82093248; // phase 0 -> slot 6
static constexpr uint32_t ADDR_LUT_CULL_MODE     = 0x82093254; // phases 3,23,33
static constexpr uint32_t ADDR_LUT_STENCIL_OP    = 0x82093200; // phase 14 -> slot 33
static constexpr uint32_t ADDR_LUT_FILTER_MODE   = 0x82093208; // phases 15-18
static constexpr uint32_t ADDR_LUT_WRAP_MODE     = 0x82093274; // phases 20-22,30-32
static constexpr uint32_t ADDR_LUT_DEPTH_FUNC    = 0x82093294; // phase 9 -> slot 1
static constexpr uint32_t ADDR_LUT_BLEND_SRC_ALT = 0x820932A0; // phase 5 alt table

// Number of render phases
static constexpr uint32_t NUM_RENDER_PHASES = 37;

// 1.0/255.0 constant for color normalization
static constexpr float INV_255 = 0.0039215689f;

// =============================================================================
// External function declarations
// =============================================================================

// Functions hooked in this file (forward declare __imp__ for call-through):
PPC_FUNC_IMPL(__imp__sub_828C8588);  // SetRenderState(stateId, value)
PPC_FUNC_IMPL(__imp__sub_828E02E8);  // DispatchRenderState(stateId)
PPC_FUNC_IMPL(__imp__sub_828BE978);  // AdjustDepthValue
PPC_FUNC_IMPL(__imp__sub_828C2178);  // GetActiveRenderTarget
PPC_FUNC_IMPL(__imp__sub_828C2140);  // SetActiveRenderTarget
PPC_FUNC_IMPL(__imp__sub_821D6210);  // InitRenderPhaseDefaults
PPC_FUNC_IMPL(__imp__sub_828C19C0);  // SetRenderPhase (37 phases)
PPC_FUNC_IMPL(__imp__sub_828C15C8);  // RenderDispatch (main frame entry)
PPC_FUNC_IMPL(__imp__sub_828C1228);  // RenderScenes (multi-viewport)
PPC_FUNC_IMPL(__imp__sub_828C1080);  // SetupViewport
PPC_FUNC_IMPL(__imp__sub_828C0688);  // EmitViewportQuad

// External functions (called but not hooked here):
PPC_FUNC_IMPL(__imp__sub_828C9980);  // SetRenderTarget(rtStruct, rtEnd, config, target)
PPC_FUNC_IMPL(__imp__sub_822BCA90);  // nullsub_1 (debug marker)
PPC_FUNC_IMPL(__imp__sub_82A3B7B0);  // grcDevice::SetDepthStencilSurface
PPC_FUNC_IMPL(__imp__sub_82A3B690);  // grcDevice::SetRenderTarget
PPC_FUNC_IMPL(__imp__sub_82A42760);  // grcDevice::SetScissorRect(disable)
PPC_FUNC_IMPL(__imp__sub_82A424A8);  // grcDevice::SetViewport(reset)
PPC_FUNC_IMPL(__imp__sub_82A44B78);  // grcDevice::SetSamplerState
PPC_FUNC_IMPL(__imp__sub_82A42930);  // grcDevice::SetStreamSource
PPC_FUNC_IMPL(__imp__sub_82A3CC68);  // grcDevice::ClearViewport
PPC_FUNC_IMPL(__imp__sub_828BF420);  // Present/Flip
PPC_FUNC_IMPL(__imp__sub_82A49C38);  // grcDevice::Resolve (back-buffer)
PPC_FUNC_IMPL(__imp__sub_82A3E7A0);  // grcDevice::SetScissorEnable(mask)
PPC_FUNC_IMPL(__imp__sub_82A3EDA8);  // grcDevice::ClearDepthStencil
PPC_FUNC_IMPL(__imp__sub_82849920);  // GetNextRenderTarget in chain

// =============================================================================
// sub_828C8588 - SetRenderState
// =============================================================================
// Writes value to gRenderState[stateId], then dispatches via grcDevice.
// 24 bytes, leaf function, 47 callers (hot).
//
// void SetRenderState(int stateId, int value)
// {
//     gRenderState[stateId] = value;
//     DispatchRenderState(stateId, value);
// }

PPC_FUNC_HOOK(sub_828C8588)
{
    uint32_t stateId = ctx.r3.u32;
    uint32_t value   = ctx.r4.u32;

    // gRenderState[stateId] = value
    PPC_STORE_U32(ADDR_RENDER_STATE + stateId * 4, value);

    // Dispatch to grcDevice method
    ctx.r3.u32 = stateId;
    ctx.r4.u32 = value;
    __imp__sub_828E02E8(ctx, base);
}


// =============================================================================
// sub_828C19C0 - SetRenderPhase (37 phases, 571 callers)
// =============================================================================
// The core render phase registration function. Takes a phase index (0..36)
// and a parameter value. Stores the parameter, then maps the phase to one
// or more render state slots and applies them.
//
// Phase 1: special (stores mode, no state call)
// Phase 2: special (stores mode + inner switch with 14 cases for blend modes)
// Phase 8: sets TWO states (slot 28 = param, then slot 10 = param)
// All others: map phase -> single state slot, optionally lookup param in table
//
// The function runs as recompiled PPC code because the switch table
// is complex (37 outer cases + 14 inner cases for phase 2) and
// performance-critical (571 callers). Hooking would require reproducing
// the entire switch exactly.

// Decompiled C++ hook for sub_828C19C0.
// Replaces the recompiled 37-case + 14-inner-case switch with clean C++.
//
// Phase -> State mapping:
//   Phase 0  -> state  6 (via LUT_BLEND_SRC[param])
//   Phase 1  -> special: stores param to PHASE1_MODE only
//   Phase 2  -> special: stores param to PHASE2_MODE, then inner 14-case
//               switch sets states 4 (blendSrc), 5 (blendDst), 23 (alphaBlend)
//   Phase 3  -> state  9 (via LUT_CULL_MODE[param])
//   Phase 4  -> state  8 (passthrough)
//   Phase 5  -> state  7 (via LUT_CULL_MODE[param] or LUT_BLEND_SRC_ALT[param])
//   Phase 6  -> state  2 (passthrough)
//   Phase 7  -> state  3 (passthrough)
//   Phase 8  -> state 28 (param) + state 10 (param)
//   Phase 9  -> state  1 (via LUT_DEPTH_FUNC[param])
//   Phase 10 -> state  0 (passthrough)
//   Phase 11 -> state 30 (passthrough)
//   Phase 12 -> state 29 (passthrough)
//   Phase 13 -> state 32 (passthrough)
//   Phase 14 -> state 33 (via LUT_STENCIL_OP[param])
//   Phase 15 -> state 19 (via LUT_FILTER_MODE[param])
//   Phase 16 -> state 20 (via LUT_FILTER_MODE[param])
//   Phase 17 -> state 21 (via LUT_FILTER_MODE[param])
//   Phase 18 -> state 22 (via LUT_FILTER_MODE[param])
//   Phase 19 -> state 11 (passthrough)
//   Phase 20 -> state 12 (via LUT_WRAP_MODE[param])
//   Phase 21 -> state 13 (via LUT_WRAP_MODE[param])
//   Phase 22 -> state 14 (via LUT_WRAP_MODE[param])
//   Phase 23 -> state 15 (via LUT_CULL_MODE[param])
//   Phase 24 -> state 16 (passthrough)
//   Phase 25 -> state 17 (passthrough)
//   Phase 26 -> state 18 (passthrough)
//   Phase 27 -> state 34 (passthrough)
//   Phase 28 -> state 25 (passthrough)
//   Phase 29 -> state 35 (passthrough)
//   Phase 30 -> state 36 (via LUT_WRAP_MODE[param])
//   Phase 31 -> state 37 (via LUT_WRAP_MODE[param])
//   Phase 32 -> state 38 (via LUT_WRAP_MODE[param])
//   Phase 33 -> state 39 (via LUT_CULL_MODE[param])
//   Phase 34 -> state 40 (passthrough)
//   Phase 35 -> state 41 (passthrough)
//   Phase 36 -> state 42 (passthrough)



// =============================================================================
// sub_821D6210 - InitRenderPhaseDefaults
// =============================================================================
// Called once from grmSetup construction (sub_821B3CE8).
// Reads 37 default values from .rdata and initializes all phases.
//
// for (int i = 0; i < 37; i++) {
//     int defaultVal = gRenderPhaseDefaults[i];
//     gRenderPhaseSaved[i] = defaultVal;
//     SetRenderPhase(i, defaultVal);
// }

PPC_FUNC_HOOK(sub_821D6210)
{
    for (uint32_t i = 0; i < NUM_RENDER_PHASES; i++) {
        uint32_t defaultVal = PPC_LOAD_U32(ADDR_RENDER_PHASE_DEFAULTS + i * 4);

        // Save to backup array
        PPC_STORE_U32(ADDR_RENDER_PHASE_SAVED + i * 4, defaultVal);

        // Apply via SetRenderPhase
        ctx.r3.u32 = i;
        ctx.r4.u32 = defaultVal;
        __imp__sub_828C19C0(ctx, base);
    }
}

// =============================================================================
// sub_828C15C8 - RenderDispatch (main frame render entry point)
// =============================================================================
// Called from grmSetup::EndFrame with present params (or nullptr).
// This is the heart of RAGE's per-frame rendering.
//
// Flow:
//   1. Debug marker (nullsub)
//   2. Increment frame counter
//   3. Set render target via sub_828C9980
//   4. Clear depth/stencil surface
//   5. Set 4 render targets (0-3) with blend modes
//   6. Disable scissor, reset viewport
//   7. Set 20 sampler states (0-19) with default filter
//   8. Reset stream source
//   9. Clear render flags
//  10. If sceneCount > 0: multi-viewport path (sub_828C1228)
//  11. Single-viewport clear with color/depth
//  12. Present/Flip via sub_828BF420
//  13. Optional back-buffer resolve
//
// Already guarded at dispatch level in grm_setup_patches.cpp (grmSetup thunks).
// The function itself has a clean C++ hook below.

PPC_FUNC_HOOK(sub_828C15C8)
{
    uint32_t presentParams = ctx.r3.u32; // r27 in recomp

    // 1. Debug marker (nullsub)
    ctx.r3.u32 = presentParams;
    __imp__sub_822BCA90(ctx, base);

    // 2. Increment frame counter
    uint32_t frameCount = PPC_LOAD_U32(ADDR_FRAME_COUNTER);
    PPC_STORE_U32(ADDR_FRAME_COUNTER, frameCount + 1);

    // 3. Set render target
    uint32_t device = PPC_LOAD_U32(ADDR_GRC_DEVICE);
    ctx.r3.u32 = ADDR_RT_STRUCT;
    ctx.r4.u32 = ADDR_RT_STRUCT + 48;
    ctx.r5.u32 = PPC_LOAD_U32(0x831C2290);
    ctx.r6.u32 = 0;
    __imp__sub_828C9980(ctx, base);

    // 4. Clear depth/stencil surface
    ctx.r3.u32 = device;
    ctx.r4.u32 = 0;
    __imp__sub_82A3B7B0(ctx, base);

    // 5. Set 4 render targets (0-3) with blend modes: RT0-2 = mode 1, RT3 = mode 2
    for (uint32_t i = 0; i < 4; i++) {
        ctx.r3.u32 = device;
        ctx.r4.u32 = i;
        ctx.r5.u32 = 0;
        ctx.r6.u32 = 0;
        ctx.r7.u32 = 0;
        ctx.r8.u32 = (i < 3) ? 1 : 2;
        __imp__sub_82A3B690(ctx, base);
    }

    // 6. Disable scissor rect, reset viewport
    ctx.r3.u32 = device;
    ctx.r4.u32 = 0;
    __imp__sub_82A42760(ctx, base);

    ctx.r3.u32 = device;
    ctx.r4.u32 = 0;
    __imp__sub_82A424A8(ctx, base);

    // 7. Set 20 sampler states (0-19) with default filter
    // Each sampler gets a mask = (1 << 63) >> (i + 32)
    uint64_t baseMask = uint64_t(1) << 63;
    for (uint32_t i = 0; i <= 19; i++) {
        uint32_t shift = i + 32;
        uint64_t mask = (shift >= 64) ? 0 : (baseMask >> shift);
        ctx.r3.u32 = device;
        ctx.r4.u32 = i;
        ctx.r5.u32 = 0;
        ctx.r6.u64 = mask;
        __imp__sub_82A44B78(ctx, base);
    }

    // 8. Reset stream source
    ctx.r3.u32 = device;
    ctx.r4.u32 = 0;
    __imp__sub_82A42930(ctx, base);

    // 9. Clear render flags
    PPC_STORE_U32(ADDR_RENDER_DIRTY_FLAG, 0);
    PPC_STORE_U32(ADDR_CURRENT_RT + 4, 0); // 0x831C22A8 = viewport scene counter
    PPC_STORE_U32(ADDR_RENDER_SCENE_FLAG, 0);

    // 10. Multi-viewport path if sceneCount > 0
    int32_t sceneCount = static_cast<int32_t>(PPC_LOAD_U32(ADDR_SCENE_COUNT));
    bool sceneRendered = false;

    if (sceneCount > 0) {
        // Check if active viewport exists and matches current scene RT
        uint32_t viewportData = PPC_LOAD_U32(ADDR_VIEWPORT_DATA);
        if (viewportData != 0) {
            // Call vtable[16] (GetRenderTarget) on the viewport
            uint32_t vtbl = PPC_LOAD_U32(viewportData);
            uint32_t getFn = PPC_LOAD_U32(vtbl + 64);
            ctx.r3.u32 = viewportData;
            PPC_CALL_INDIRECT_FUNC(getFn);
            uint32_t vpRT = ctx.r3.u32;

            uint32_t vpIndex = PPC_LOAD_U32(ADDR_VIEWPORT_INDEX);
            uint32_t sceneRT = PPC_LOAD_U32(ADDR_RENDER_TARGETS + vpIndex * 4);
            if (vpRT == sceneRT) {
                sceneRendered = true;
            }
        }

        // Render all scenes
        __imp__sub_828C1228(ctx, base);
    }

    // 11. If scene was not rendered, do single-viewport clear
    if (!sceneRendered) {
        if (presentParams != 0) {
            // Extract ARGB clear color from presentParams+8 and convert to float[4]
            uint32_t argb = PPC_LOAD_U32(presentParams + 8);
            uint8_t a = (argb >> 24) & 0xFF;
            uint8_t r = (argb >> 16) & 0xFF;
            uint8_t g = (argb >>  8) & 0xFF;
            uint8_t b = (argb >>  0) & 0xFF;

            // Pack float color into stack (f13=a, f10=r, f12=g, f11=b)
            float clearColor[4];
            clearColor[0] = static_cast<float>(a) * INV_255;
            clearColor[1] = static_cast<float>(r) * INV_255;
            clearColor[2] = static_cast<float>(g) * INV_255;
            clearColor[3] = static_cast<float>(b) * INV_255;

            // Determine clear flags: 768 (D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER)
            // unless back-buffer format matches magic, then also set depth bits
            uint32_t clearFlags = 768;
            uint32_t renderConfig = PPC_LOAD_U32(ADDR_RENDER_CONFIG);
            if (PPC_LOAD_U32(renderConfig + 40) == FMT_A8R8G8B8_CONST) {
                uint32_t renderCount = PPC_LOAD_U32(ADDR_RENDER_COUNT);
                clearFlags = (static_cast<uint32_t>(-renderCount) << 26) | 0x300;
            }

            uint32_t vpIndex = PPC_LOAD_U32(ADDR_VIEWPORT_INDEX);
            uint32_t sceneRT = PPC_LOAD_U32(ADDR_RENDER_TARGETS + vpIndex * 4);

            // ClearViewport with color
            ctx.r3.u32 = device;
            ctx.r4.u32 = clearFlags;
            ctx.r5.u32 = 0;
            ctx.r6.u32 = sceneRT;
            ctx.r7.u32 = 0;
            ctx.r8.u32 = 0;
            ctx.r9.u32 = 0;
            // r10 = pointer to float[4] color — stored on PPC stack
            // We need to write the color data to guest memory for the callee
            uint32_t colorAddr = ctx.r1.u32 + 160;
            uint32_t fa, fr, fg, fb;
            std::memcpy(&fa, &clearColor[0], 4);
            std::memcpy(&fr, &clearColor[1], 4);
            std::memcpy(&fg, &clearColor[2], 4);
            std::memcpy(&fb, &clearColor[3], 4);
            PPC_STORE_U32(colorAddr + 0, fa);
            PPC_STORE_U32(colorAddr + 4, fr);
            PPC_STORE_U32(colorAddr + 8, fg);
            PPC_STORE_U32(colorAddr + 12, fb);
            ctx.r10.u32 = colorAddr;
            __imp__sub_82A3CC68(ctx, base);
        } else {
            // No present params: clear with default flags
            uint32_t clearFlags = 0;
            uint32_t renderConfig = PPC_LOAD_U32(ADDR_RENDER_CONFIG);
            if (PPC_LOAD_U32(renderConfig + 40) == FMT_A8R8G8B8_CONST) {
                uint32_t renderCount = PPC_LOAD_U32(ADDR_RENDER_COUNT);
                clearFlags = static_cast<uint32_t>(-renderCount) << 26;
            }

            uint32_t vpIndex = PPC_LOAD_U32(ADDR_VIEWPORT_INDEX);
            uint32_t sceneRT = PPC_LOAD_U32(ADDR_RENDER_TARGETS + vpIndex * 4);

            ctx.r3.u32 = device;
            ctx.r4.u32 = clearFlags;
            ctx.r5.u32 = 0;
            ctx.r6.u32 = sceneRT;
            ctx.r7.u32 = 0;
            ctx.r8.u32 = 0;
            ctx.r9.u32 = 0;
            ctx.r10.u32 = 0; // no color
            __imp__sub_82A3CC68(ctx, base);
        }
    }

    // 12. Present/Flip
    __imp__sub_828BF420(ctx, base);

    // 13. Optional back-buffer resolve
    uint32_t flipPending = PPC_LOAD_U32(ADDR_FLIP_PENDING);
    if (flipPending) {
        ctx.r3.u32 = device;
        __imp__sub_82A49C38(ctx, base);
    }
}


// =============================================================================
// sub_828C0688 - EmitViewportQuad
// =============================================================================
// Allocates a 3-vertex triangle strip (28 bytes/vertex) via grcDevice,
// fills vertex positions and UV coordinates, then submits.
//
// Handles depth inversion for back-buffer format.
// The quad is two triangles covering the viewport rect.
//
// Vertex layout (28 bytes):
//   +0:  float x
//   +4:  float y
//   +8:  float depth (possibly inverted)
//   +12: float w (always 1.0)
//   +16: uint32 clearColor
//   +20: float u
//   +24: float v

// sub_828C0688 runs as recompiled code.

// =============================================================================
// sub_828BF420 - Present/Flip
// =============================================================================
// Manages the frame present queue:
//   1. If flip pending (dword_831C2460==1): flush via sub_82A46DA0
//   2. Wait for GPU idle via sub_82A46D70 (loop while depth >= 2)
//   3. Present via sub_82A467D8 with triple-buffered render target
//   4. Advance triple-buffer index: (index + 1) % 3

// sub_828BF420 runs as recompiled code.
