# Rendering Plan: Black Screen → Visible Output

## Current State
- Game init completes successfully
- VdSwap fires 600 times / 20 seconds (30 fps steady state)
- Video::Present runs, clears purple (0.1, 0.0, 0.2) as visual heartbeat
- VBlank ticks at 60Hz
- **No game geometry draws** — render dispatch never enters

## Root Cause Chain (verified with Python arithmetic + generated code)

### Blocker #1: Render Gate Stuck at -1
- **Address**: `0x82B0B48C` (PPC: `lis r10,-32079; lwz r10,-19316(r10)`)
- **Generated code**: [gta4_recomp.58.cpp](../glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.58.cpp) line 115343
- **Gate check**: `sub_828C15C8` loads gate, branches `ble cr6, loc_828C1764` (skip) when ≤ 0
- **Only writer**: `sub_828C1228` writes -1 after rendering (line 115146)
- **Positive writer was**: Xbox 360 D3D runtime via `VdCallGraphicsNotificationRoutines` callback chain
- **Current state**: `VdCallGraphicsNotificationRoutines` is **stubbed** (`return 0`) in [imports.cpp](../LibertyRecomp/kernel/imports.cpp#L374)
- **Result**: Gate starts at -1, never becomes positive, render dispatch body never executes

### Blocker #2: Scene Pointer Null (0x831C2458)
- Even when gate > 0, `sub_828C15C8` loads `*(0x831C2458)` for vtable[16] dispatch
- If null, skips scene dispatch but **still calls** `sub_828C1228` (the render body)
- `sub_828C1228` uses fallback device at `0x831C2DA8` (confirmed live at runtime: `0xD900B620`)
- Scene pointer populated by 15-state scene creation state machine (`sub_82242910`)
- State machine likely stalls at state 12 (STFS container issue) — secondary concern

### Diagnostic Typo (video.cpp)
- Line 9401: reads `0x8290B48C` (wrong, off by 0x200000)
- Should be `0x82B0B48C`
- Makes the `[RENDER-GATE]` diagnostic print garbage

## Call Flow

```
Frame rendering:
  grcSetup_rage::Present (sub_828C5BA0, vtable[4], indirect call)
    → sub_828C15C8 (render dispatch)
        reads gate at 0x82B0B48C
        if gate > 0:
          loads scene at 0x831C2458 → vtable[16] dispatch (if non-null)
          → sub_828C1228 (render body)
              uses device at 0x831C2DA8 → vtable[4], vtable[16]
              writes -1 to gate at 0x82B0B48C
        if gate ≤ 0:
          skips entire render body

Buffer swap:
  sub_82856F08 → sub_828529B0 → sub_828507F8
    → sub_82A467D8 (VdSwap, PPC_FUNC_HOOK)
        increments frame counter device[16544]
        → Video::Present() (purple clear + present)
        syncs 0x83124CCC and device[16552]
```

## Fix Plan

### Fix 1: Set Render Gate Positive (CRITICAL — one line)
In the VdSwap hook (`PPC_FUNC_HOOK(sub_82A467D8)` in [video.cpp](../LibertyRecomp/gpu/video.cpp#L9392)):

**Add** after `Video::Present()` call (line 9430):
```cpp
// Gate: sub_828C1228 resets to -1 after each frame. On real hardware,
// VdCallGraphicsNotificationRoutines sets this positive for the next frame.
// Since that kernel function is stubbed, we set it here after each present.
PPC_STORE_U32(0x82B0B48C, 1);
```

**Timing**: VdSwap runs AFTER grcSetup_rage::Present. Writing 1 here sets the gate for the NEXT frame. sub_828C1228 resets to -1 at end of each render, so each frame cycle is: read 1 → render → write -1 → VdSwap writes 1 → next frame.

**First frame**: Gate may still be ≤ 0 for frame 1 (VdSwap hasn't run yet). If needed, add an initial write at startup or in KernelPhase_EnterRuntime.

### Fix 2: Diagnostic Typo (trivial)
In [video.cpp](../LibertyRecomp/gpu/video.cpp#L9401), line 9401:
```diff
-        uint32_t gateVal = PPC_LOAD_U32(0x8290B48C);
+        uint32_t gateVal = PPC_LOAD_U32(0x82B0B48C);
```

### Fix 3 (if needed): Scene Pointer Investigation
After Fix 1, rendering will enter `sub_828C1228` which uses the fallback device at `0x831C2DA8`. If that device's vtable[4] and vtable[16] produce valid render commands, we may see output without a scene pointer.

If output is still wrong, investigate why `0x831C2458` is null:
- Trace `sub_82242910` (scene state machine) — what state does it reach?
- Check STFS container/content size validation in `sub_822417B0`
- May need to hook/fix the content validation path

## Active D3D Hook Status
The following GTA IV hooks are **active** (below `#endif // End Sonic 06 hooks`):

| Category | Hooks |
|-|-|
| Draw | UnifiedDraw, DrawSurface, DrawPrimitive (GUEST_FUNCTION_HOOK sub_82A49CB0) |
| Shaders | SetVertexShader (x2), SetPixelShader, SetBothShaders, CreateVertexShader, CreatePixelShader, CreateVertexDeclaration |
| Textures | SetTexture, GTAIV_CreateTexture, Texture/RT Alloc (sub_82A55DC0) |
| Render Target | SetRenderTarget, SetDepthStencilSurface |
| Viewport | SetViewport, SetScissorRect |
| Vertex | SetVertexDeclaration, SetStreamSource, SetIndices |
| State | 26 SetRenderState hooks (Z, alpha, blend, stencil, etc.) |
| Buffers | GTAIV_CreateVertexBuffer, LockTextureRect/Unlock, LockVertexBuffer/Unlock |
| PM4 | PM4 Packet Builder stub, PM4 Buffer Flush |

## Expected Outcome After Fix 1
1. Gate reads 1 → enters render dispatch
2. Scene at 0x831C2458 is likely null → skip scene vtable dispatch
3. sub_828C1228 runs with device at 0x831C2DA8 (live)
4. Device vtable calls trigger rendering through the active D3D hooks
5. Draw calls hit UnifiedDraw/DrawPrimitive → Metal render commands
6. Gate reset to -1 → VdSwap writes 1 → next frame

**Risk**: If the device vtable calls crash or dispatch to unhooked functions, we'll need to add more hooks. But the existing hook coverage is extensive (draw, shader, texture, RT, state, viewport).

## Verification Steps
1. Apply Fix 1 + Fix 2
2. Build: `cmake --build ./out/build/macos-release --target LibertyRecomp`
3. Run and check:
   - `[RENDER-GATE]` should show `gate=1` (not -1 or garbage)
   - New log output from draw hooks (DrawPrimitive, SetRenderTarget, etc.)
   - Visual output changes from solid purple to scene geometry
4. If crashes: check which vtable function crashes and add appropriate hook
