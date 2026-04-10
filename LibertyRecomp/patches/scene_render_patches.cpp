// =============================================================================
// Scene Rendering Pipeline - Decompiled from GTA IV Xbox 360
// =============================================================================
//
// This file decompiles the scene creation and world rendering pipeline.
// The scene is not a pre-allocated object at a fixed address. Instead,
// the renderer fetches a render target from the grcDevice backend each frame
// via a virtual dispatch.
//
// ARCHITECTURE:
//
//   grcDevice backend (dword_831C2DA8)
//     Created by sub_828C5840 (grcSetup::Init, vtable[1] of grmSetup)
//     Two implementations:
//       - grcDeviceXenon (sub_828E7630, vtable 0x82097A04) -- real GPU path
//       - grcDeviceNull  (sub_828DC890, vtable 0x82096084) -- fallback path
//     Both inherit from grcDeviceBase (sub_828C2350, vtable 0x82093614)
//
//   dword_831C2458: OPTIONAL scene override pointer (secondary viewport)
//     - NULL for primary viewport (this is BY DESIGN, not an error)
//     - When set, forces rendering to an alternate target (mirrors, etc.)
//     - Only ever read, never written by any function in the binary
//
//   dword_831C2DA8: grcDevice backend singleton
//     - Written by sub_828C5840 during grcSetup::Init
//     - vtable[4]: GetRenderTarget(index) - returns grcRenderTarget*
//     - vtable[16]: SetResolveTarget(target, flags, colorBuf, depthVal)
//     - The primary scene comes from: device->GetRenderTarget(0)
//
// RENDER FLOW (per frame):
//
//   EndFrame (sub_828C5BA0)
//     -> sub_828C15C8 (PresentFrame)
//        -> sub_828C1228 (DrawScene_Internal)
//           For each light source (dword_82B0B48C count):
//             1. scene = dword_831C2458 ?: device->GetRenderTarget(0)
//             2. texture = scene->GetTexture()  [vtable slot 16]
//             3. Compute RGBA from dword_82B0B51C, scale by 1/255
//             4. Submit textured quad via sub_82A3CC68
//           After loop (if !scene_ptr && !(flags & 2)):
//             5. Set render states (7=ZEnable, 8=ZWriteEnable, 10=AlphaBlendEnable)
//             6. Set active render target via sub_828C2140
//             7. Draw fullscreen clear quad via sub_828C1080
//             8. Restore render states
//
//   sub_828C1080 (DrawQuad):
//     1. target = sub_828C2178() -- get current render target
//     2. width = target->GetWidth()   [vtable slot 6]
//     3. height = target->GetHeight() [vtable slot 7]
//     4. uvScaleX = 1.0 / width, uvScaleY = 1.0 / height
//     5. sub_828C0688(coords, uvScale, depth, color)
//
//   sub_828C0688 (SubmitFullscreenQuad):
//     1. Set draw list state via sub_828C05A8
//     2. Allocate vertex buffer: sub_82A3DAB0(device, 8, 3, 28)
//     3. Write 3 vertices (triangle strip) with pos + UV + color
//     4. Finalize draw: sub_82A3DF50(device)
//
// GLOBALS:
//   0x831C22A4 - grcDevice context (low-level D3D device)
//   0x831C2290 - display mode / resolution descriptor
//   0x831C2DA8 - grcDevice backend (high-level render target manager)
//   0x831C2458 - scene override pointer (NULL for primary viewport)
//   0x831C245C - secondary scene override (used with reflections)
//   0x831C23C8 - current back buffer index
//   0x831C23E8 - back buffer array (indexed by 831C23C8)
//   0x831C2540 - comparison target for split-screen detection
//   0x831C2910 - active render target handle
//   0x831C2D48 - null render target sentinel
//   0x82B0B48C - light source count (set to -1 after draw)
//   0x82B0B494 - backend type ID (438436247 = Xenon GPU)
//   0x82B0B469 - orientation flip flag
//   0x82B0B514 - light intensity scale (float)
//   0x82B0B51C - packed ARGB light color (4 bytes)
//   0x82BF3A88 - scene creation state counter array (32 entries)
//   0x831C23D4 - render flags (bit 1 = skip fallback clear)
//
// =============================================================================

#include <api/Liberty.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>

// =============================================================================
// Address Constants
// =============================================================================

namespace SceneAddr {
    // grcDevice backend (high-level render target manager)
    constexpr uint32_t kDeviceBackend       = 0x831C2DA8;
    // Scene override pointer (NULL = use device backend)
    constexpr uint32_t kSceneOverride       = 0x831C2458;
    // Secondary scene override (reflections)
    constexpr uint32_t kSceneOverride2      = 0x831C245C;
    // Render flags at 0x831C23D4 (bit 1 = skip fallback clear)
    constexpr uint32_t kRenderFlags         = 0x831C23D4;
    // Low-level D3D device context
    constexpr uint32_t kDeviceContext       = 0x831C22A4;
    // Back buffer index + array
    constexpr uint32_t kBackBufferIndex     = 0x831C23C8;
    constexpr uint32_t kBackBufferArray     = 0x831C23E8;
    // Active render target
    constexpr uint32_t kActiveRenderTarget  = 0x831C2910;
    // Null render target sentinel
    constexpr uint32_t kNullTargetSentinel  = 0x831C2D48;
    // Split-screen comparison target
    constexpr uint32_t kSplitScreenTarget   = 0x831C2540;
    // Light source count (signed, set to -1 after draw)
    constexpr uint32_t kLightCount          = 0x82B0B48C;
    // Packed ARGB light color
    constexpr uint32_t kLightColor          = 0x82B0B51C;
    // Light intensity scale (float)
    constexpr uint32_t kLightIntensity      = 0x82B0B514;
    // Orientation flip flag
    constexpr uint32_t kOrientFlip          = 0x82B0B469;
    // Backend type ID
    constexpr uint32_t kBackendType         = 0x82B0B494;
    // Xenon backend type value
    constexpr uint32_t kXenonBackendID      = 438436247;
    // Light source data base (array of 16-byte structs)
    constexpr uint32_t kLightDataBase       = 0x82B0B51C;
    // Scene creation state counter array (32 entries)
    constexpr uint32_t kSceneStateArray     = 0x82BF3A88;

    // Display/resolution globals
    constexpr uint32_t kDisplayMode         = 0x831C2290;
    constexpr uint32_t kDisplayConfig       = 0x831C22A0;
    constexpr uint32_t kDisplayModeCurrent  = 0x831C22A8;

    // Present-frame state
    constexpr uint32_t kPresentCounter      = 0x82B09D14;
    constexpr uint32_t kPostProcessFlag     = 0x831C24A8;
    constexpr uint32_t kDirtyFlag3DE4       = 0x831C3DE4;
    constexpr uint32_t kDirtyFlag3DDC       = 0x831C3DDC;

    // Render state array
    constexpr uint32_t kRenderStateArray    = 0x831C2740;

    // grcDevice backend vtable offsets
    constexpr uint32_t kVT_GetRenderTarget  = 16;   // slot 4: GetRenderTarget(index)
    constexpr uint32_t kVT_SetResolveTarget = 64;   // slot 16: SetResolveTarget(...)

    // grcRenderTarget vtable offsets
    constexpr uint32_t kRT_VT_GetTexture    = 64;   // slot 16: GetTexture()
    constexpr uint32_t kRT_VT_GetWidth      = 24;   // slot 6: GetWidth()
    constexpr uint32_t kRT_VT_GetHeight     = 28;   // slot 7: GetHeight()

    // grcDeviceXenon vtable address
    constexpr uint32_t kVtableXenonDevice   = 0x82097A04;
    // grcDeviceNull vtable address
    constexpr uint32_t kVtableNullDevice    = 0x82096084;
    // grcDeviceBase vtable address
    constexpr uint32_t kVtableBaseDevice    = 0x82093614;

    // GPU check global
    constexpr uint32_t kHasXenonGPU         = 0x831C3094;
}

// =============================================================================
// sub_828C2178 - Get current render target
// =============================================================================
// Returns dword_831C2910 unless it equals the null sentinel (dword_831C2D48).
// Tiny 32-byte leaf function.
//
// Decompiled:
//   if (activeTarget == nullSentinel) return 0;
//   return activeTarget;

PPC_FUNC_IMPL(__imp__sub_828C2178);
PPC_FUNC_HOOK(sub_828C2178)
{
    uint32_t active   = PPC_LOAD_U32(SceneAddr::kActiveRenderTarget);
    uint32_t sentinel = PPC_LOAD_U32(SceneAddr::kNullTargetSentinel);

    if (active == sentinel)
        ctx.r3.u64 = 0;
    else
        ctx.r3.u64 = active;
}

// =============================================================================
// sub_828C2140 - Set active render target
// =============================================================================
// Sets dword_831C2910 to the given target (or null sentinel if 0).
// Then calls sub_828C9980 to sync the display descriptor.
// 89 callers - hot function.
//
// Decompiled:
//   void SetActiveRenderTarget(grcRenderTarget* target) {
//       if (!target) target = nullSentinel;
//       activeTarget = target;
//       sub_828C9980(&sceneCfg, displayMode, displayConfig, target);
//   }

PPC_FUNC_IMPL(__imp__sub_828C2140);
PPC_FUNC_HOOK(sub_828C2140)
{
    // Let recompiled code handle this - it's a hot path and the logic
    // is straightforward enough to run as PPC
    __imp__sub_828C2140(ctx, base);
}

// =============================================================================
// sub_828BE978 - Conditional UV flip for Xenon backend
// =============================================================================
// Returns (1.0 - val) when running on Xenon GPU without orientation override.
// On non-Xenon or with flip flag set, returns val unchanged.
//
// Decompiled:
//   float AdjustUV(float val) {
//       if (!orientFlip && backendType == XENON_ID)
//           return 1.0f - val;
//       return val;
//   }

PPC_FUNC_IMPL(__imp__sub_828BE978);
PPC_FUNC_HOOK(sub_828BE978)
{
    // Let recompiled code run - simple float math
    __imp__sub_828BE978(ctx, base);
}

// =============================================================================
// sub_828C19C0 - Set render state by category
// =============================================================================
// Large switch (37 cases) that translates game-level render state IDs
// to low-level D3D render state values via lookup tables, then calls
// sub_828C8588 to apply.
//
// 571 callers - extremely hot function.
// Let recompiled code run directly.

PPC_FUNC_IMPL(__imp__sub_828C19C0);
PPC_FUNC_HOOK(sub_828C19C0)
{
    __imp__sub_828C19C0(ctx, base);
}

// =============================================================================
// sub_828C0688 - Submit fullscreen quad to draw list
// =============================================================================
// Allocates a 3-vertex triangle strip with position + UV + color per vertex.
// Handles Xenon UV flip for Y coordinates.
//
// Parameters (via FP regs):
//   f1=left, f2=top, f3=right, f4=bottom (screen coords, 0-based)
//   f5=depth
//   f6=uvLeft*scaleX, f7=uvTop*scaleY, f8=uvRight*scaleX
//   f9=uvBottom*scaleY (from resolution 1/width, 1/height)
//
// Additional: r3 stack param = packed ARGB color

PPC_FUNC_IMPL(__imp__sub_828C0688);
PPC_FUNC_HOOK(sub_828C0688)
{
    // Complex vertex buffer allocation + fill.
    // Let recompiled code handle the D3D draw list interaction.
    __imp__sub_828C0688(ctx, base);
}

// =============================================================================
// sub_828C1080 - Draw fullscreen quad using current render target
// =============================================================================
// Gets the current render target via sub_828C2178, queries its resolution
// via vtable[6] (GetWidth) and vtable[7] (GetHeight), computes UV scaling,
// then calls sub_828C0688 to submit the quad.
//
// Parameters (via registers and FP regs):
//   r3=left, r4=right, r5=bottom, r6=top (pixel coords)
//   f1=depth
//   r8=uvTop, r9=uvBottom, r10=uvRight (pixel offsets)
//   stack: packed ARGB color
//
// Decompiled:
//   void DrawQuad(int left, int right, int bottom, int top, float depth,
//                 int uvTop, int uvBottom, int uvRight, int color) {
//       grcRenderTarget* target = GetCurrentRenderTarget();
//       float uvScaleX, uvScaleY;
//       if (target) {
//           uvScaleX = 1.0f / (float)target->GetWidth();
//           uvScaleY = 1.0f / (float)target->GetHeight();
//       } else {
//           uvScaleX = uvScaleY = 0.0f;
//       }
//       float fLeft   = (float)left   - 0.5f;
//       float fRight  = (float)right  - 0.5f;
//       float fTop    = (float)top    - 0.5f;
//       float fBottom = (float)bottom - 0.5f;
//       SubmitQuad(fLeft, fTop, fRight, fBottom, depth,
//                  (float)left * uvScaleX, (float)uvTop * uvScaleY,
//                  (float)uvRight * uvScaleX, (float)uvBottom * uvScaleY,
//                  color);
//   }

PPC_FUNC_IMPL(__imp__sub_828C1080);
PPC_FUNC_HOOK(sub_828C1080)
{
    __imp__sub_828C1080(ctx, base);
}

// =============================================================================
// sub_828C1228 - DrawScene_Internal (core scene rendering)
// =============================================================================
// The main scene rendering function. Iterates over light sources, fetches the
// scene render target from the device backend, submits textured quads, and
// handles the fallback clear-screen path when no scene override is set.
//
// Called by: sub_828C15C8 (PresentFrame)
//
// Key logic:
//   for (i = 0; i < lightCount; i++) {
//       SetBlendState(device, 3 << (i*2));
//       scene = sceneOverride ?: device->GetRenderTarget(0);
//       if (sceneOverride2)
//           SubmitLitQuad(sceneOverride2, lightData[i]);
//       texture = scene->GetTexture();
//       SubmitLitQuad(scene, lightData[i]);
//   }
//   SetBlendState(device, 0);
//   SetResolveTarget(device, 0, 0, 0, 0, color, 0, 0, 1.0);
//   device->SetResolveTarget(0, 0, -1);
//
//   if (!sceneOverride && !(renderFlags & 2)) {
//       // Fallback clear rendering (no scene available yet)
//       SetRenderState(7, 0);   // ZEnable = false
//       SetRenderState(8, 0);   // ZWriteEnable = false
//       SetRenderState(10, 0);  // AlphaBlendEnable = false
//       target = device->GetRenderTarget(0);
//       SetActiveRenderTarget(target);
//       DrawQuad(0, 0, bottom, right, 0.0, color);
//       SetRenderState(7, 1);   // restore
//       SetRenderState(8, 1);
//       SetRenderState(10, 1);
//   }
//   lightCount = -1;  // reset for next frame

PPC_FUNC_IMPL(__imp__sub_828C1228);
PPC_FUNC_HOOK(sub_828C1228)
{
    // Validate device backend exists before entering draw loop
    uint32_t device = PPC_LOAD_U32(SceneAddr::kDeviceBackend);
    if (device == 0)
    {
        // No device backend - cannot render. Set lightCount to -1 and return.
        PPC_STORE_U32(SceneAddr::kLightCount, 0xFFFFFFFF);
        LOG_DEBUG("DrawScene_Internal: device backend is NULL, skipping");
        return;
    }

    // Validate device vtable is sane
    uint32_t vtable = PPC_LOAD_U32(device);
    if (vtable == 0 || vtable < 0x82000000 || vtable > 0x82FFFFFF)
    {
        PPC_STORE_U32(SceneAddr::kLightCount, 0xFFFFFFFF);
        LOGF_WARNING("DrawScene_Internal: device vtable invalid (0x{:08X})", vtable);
        return;
    }

    __imp__sub_828C1228(ctx, base);
}

// =============================================================================
// sub_828C15C8 - PresentFrame (top-level frame present)
// =============================================================================
// Already hooked in grm_setup_patches.cpp.
// This function:
//   1. Increments frame counter
//   2. Syncs display descriptor
//   3. Sets up render state (samplers, blend modes)
//   4. Sets up rotation matrix from display config
//   5. Checks if scene is valid (sceneOverride->GetTexture() == current backbuf)
//   6. Calls sub_828C1228 (DrawScene_Internal)
//   7. If scene invalid: clear/blit with present params
//   8. Calls sub_828BF420 (flip/present)
//   9. Optionally calls sub_82A49C38 (post-process)
//
// NOTE: Already hooked, not re-hooked here.

// =============================================================================
// sub_828C5840 - grcSetup::Init (render target creation)
// =============================================================================
// Creates the grcDevice backend and stores it to dword_831C2DA8.
// Path selection:
//   if (dword_831C3094)  // has Xenon GPU
//       sub_828E7630(1)  // create grcDeviceXenon, store to 831C2DA8
//   else
//       sub_828DC890(1)  // create grcDeviceNull, store to 831C2DA8
//
// Then:
//   sub_828C2A48()       // initialize render target pool
//   dword_831C2F88 = dword_831C5E08 = sub_828E09A0(config, 8, 8, 1, 127)
//
// Already hooked in grm_setup_patches.cpp (called by InitDevice vtable[1]).
// NOTE: Not re-hooked here.

// =============================================================================
// sub_828E7630 - Create grcDeviceXenon (Xenon GPU backend)
// =============================================================================
// Allocates 4 bytes, constructs grcDeviceBase (vtable 0x82093614),
// overwrites vtable to grcDeviceXenon (0x82097A04).
// If a1 is true, stores result to dword_831C2DA8.
//
// Decompiled:
//   grcDeviceXenon* CreateXenonDevice(bool setGlobal) {
//       void* mem = Alloc(4);
//       if (!mem) return nullptr;
//       grcDeviceBase::ctor(mem);         // sets vtable = 0x82093614
//       mem->vtable = &grcDeviceXenon_vtable;  // 0x82097A04
//       if (setGlobal)
//           dword_831C2DA8 = mem;
//       return mem;
//   }

PPC_FUNC_IMPL(__imp__sub_828E7630);
PPC_FUNC_HOOK(sub_828E7630)
{
    __imp__sub_828E7630(ctx, base);

    uint32_t result = ctx.r3.u32;
    uint32_t device = PPC_LOAD_U32(SceneAddr::kDeviceBackend);
    LOGF_INFO("grcDeviceXenon: created at 0x{:08X}, global=0x{:08X}",
              result, device);
}

// =============================================================================
// sub_828DC890 - Create grcDeviceNull (fallback software backend)
// =============================================================================
// Allocates 80 bytes, constructs via sub_828DC4D8 which:
//   1. Calls grcDeviceBase::ctor (vtable 0x82093614)
//   2. Overwrites vtable to 0x82096084 (grcDeviceNull)
//   3. Initializes 5 ring-buffer index structures (stride 28)
//   4. Creates 5 grcRenderTargetXenon objects (76 bytes each)
//      at offsets +60, +64, +68, +72, +76
//   5. Configures them with sub_828DA9B0/sub_828DA8F8/sub_828DBCB0
//      using current display config globals
//
// Decompiled:
//   grcDeviceNull* CreateNullDevice(bool setGlobal) {
//       void* mem = Alloc(80);
//       if (!mem) return nullptr;
//       grcDeviceNull::ctor(mem);
//       if (setGlobal)
//           dword_831C2DA8 = mem;
//       return mem;
//   }

PPC_FUNC_IMPL(__imp__sub_828DC890);
PPC_FUNC_HOOK(sub_828DC890)
{
    __imp__sub_828DC890(ctx, base);

    uint32_t result = ctx.r3.u32;
    uint32_t device = PPC_LOAD_U32(SceneAddr::kDeviceBackend);
    LOGF_INFO("grcDeviceNull: created at 0x{:08X}, global=0x{:08X}",
              result, device);

    if (result != 0)
    {
        uint32_t vtbl = PPC_LOAD_U32(result);
        LOGF_INFO("grcDeviceNull: vtable=0x{:08X} (expect 0x{:08X})",
                  vtbl, SceneAddr::kVtableNullDevice);

        // Log render targets created at offsets +60..+76
        for (int i = 0; i < 5; i++)
        {
            uint32_t rt = PPC_LOAD_U32(result + 60 + i * 4);
            if (rt != 0)
            {
                uint32_t rtVtbl = PPC_LOAD_U32(rt);
                LOGF_DEBUG("  renderTarget[{}] = 0x{:08X}, vtable=0x{:08X}",
                           i, rt, rtVtbl);
            }
        }
    }
}

// =============================================================================
// sub_828C29E0 / sub_828C2360 - Device backend destructors
// =============================================================================
// Both check if dword_831C2DA8 == this, and if so set it to 0.
// Let recompiled code handle these.

// =============================================================================
// sub_8223F458 - Scene state array initialization
// =============================================================================
// Zeros the 32-entry state array at 0x82BF3A88 during world init.
// The state counter value 0xB9 (185) represents an advanced world-load state.
// This array tracks the scene creation/world initialization progress.
//
// Let recompiled code handle this.
