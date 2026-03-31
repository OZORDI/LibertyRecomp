# GTA IV Rendering Pipeline Research

## Architecture Overview

GTA IV (RAGE v4) on Xbox 360 has NO separate D3D wrapper layer like Unleashed's `0x82BExxxx`. RAGE calls PM4 command buffer builders directly. The PM4 functions DO receive clean D3D-style parameters in PPC registers — they just also write Xenos GPU packets internally.

LibertyRecomp already has the FULL Unleashed-style render infrastructure: `RenderCommandType` enum (32 types), `g_renderQueue` (moodycamel concurrent queue), `PipelineState`, `FlushRenderStateForMainThread`, `FlushRenderStateForRenderThread`, `ProcDrawPrimitive`, render thread, pipeline caching — everything. It was never connected because hooks were at wrong addresses.

## Rendering Layers

```
Layer 1: Game Logic (0x821x-0x824x)
  CViewportGame::Render → CRenderPhaseDrawScene → CDrawEntityDC::Execute

Layer 2: RAGE Model Draw (0x8235x)
  sub_8235CF60 (model dispatch)

Layer 3: RAGE Shader/Geometry (0x828B-0x828E)
  sub_828C19C0  SetTechnique (VS+PS bind, 125K calls/30s)
  sub_828C7A30  grmShaderFx::Draw (main draw dispatch, 17 callees)
  sub_828E6A98  grmGeometryQB::Draw (primary indexed, full setup)
  sub_828E5CB0  grmGeometryQB::DrawSkinned (batched)
  sub_828E6770  grmGeometryQB::DrawUnskinned
  sub_828E7410  grmGeometryQB::DrawInstanced
  sub_828D0A40  SetVertexStream
  sub_828D0C58  SetIndexStream

Layer 4: PM4 Command Buffer (0x82A3-0x82A4) — HOOK LAYER
  sub_82A3CC68  DrawPrimitivesInternal (27 callers, mega-draw)
  sub_82A3DAB0  DrawPrimitiveUP_Begin (two-phase vertex alloc)
  sub_82A3DF50  DrawPrimitiveUP_Commit
  sub_82A3E348  DrawIndexedVertices (3 callers)
  sub_82A3E7A0  PM4 SetVertexShader register writer
  sub_82A44B78  PM4 SetTexture fetch constant writer
  sub_82A3B690  PM4 SetRenderTarget register writer
  sub_82A3B7B0  PM4 SetDepthStencil register writer
  sub_82A3BF50  SetShader (device, type, shaderObj) — ACTUAL shader bind

Layer 5: PM4 Infrastructure (no-op'd)
  sub_82A492A8  PM4 packet builder (returns cmdPtr unchanged)
  sub_82A499B8  PM4 flush (recycles ring buffer pointers)
  sub_82A49CB0  PM4 resolve draw (no-op)
```

## Hook Strategy

PPC_FUNC_HOOK uses `extern "C"` strong symbols that completely REPLACE the weak recompiled functions at link time. There is NO way to call the original from inside a hook (except via `__imp__sub_XXXXXXXX` forward declaration).

The correct approach:
1. Let PM4 state setters RUN as recompiled code (they update GuestDevice state AND emit PM4 — but PM4 is caught by the no-op'd sub_82A492A8/sub_82A499B8)
2. Hook ONLY the draw functions (sub_82A3CC68, sub_82A3DAB0, sub_82A3DF50, sub_82A3E348)
3. In draw hooks, read all current state from GuestDevice context, enqueue RenderCommands
4. The existing render thread processes everything → host GPU draws

## PM4 Function Parameter Layouts (verified via generated code + runtime register dumps)

### Draw Functions

**sub_82A3CC68 — DrawPrimitivesInternal (27 callers, main draw)**

| Reg | Type | Meaning |
|-|-|-|
| r3 | GuestDevice* | Device context |
| r4 | uint32_t | Flags: bits 0-2 = primType, bits 3-6 = tessellation, bit 9 = predicated, bit 13 = instancing |
| r5 | uint32_t* | Scissor rect pointer (NULL = default) |
| r6 | uint32_t* | Draw parameter struct (physical addr): +28=format, +32=tileParams, +36=vertStart/count packed |
| r7 | uint32_t* | Index buffer info pointer (NULL = default) |
| r8 | uint32_t | Vertex shader handle/resource |
| r9 | uint32_t | Instance count (0 = non-instanced) |
| r10 | uint32_t* | Viewport rect pointer (NULL = default) |
| f1 | float | Depth bias |

**sub_82A3DAB0 — DrawPrimitiveUP_Begin (7 callers, two-phase)**

| Reg | Type | Meaning |
|-|-|-|
| r3 | GuestDevice* | Device context |
| r4 | uint32_t | Primitive type (4=TRIFAN, 6=TRILIST/RECTLIST, 8=TRISTRIP) |
| r5 | uint32_t | Vertex count |
| r6 | uint32_t | Stride in bytes (28, 36, 52) |
| r7-r10 | - | NOT parameters (stale registers) |
| Returns r3 | uint32_t* | Ring buffer write pointer for vertex data |

Two-phase: caller fills vertex data into returned buffer, then calls sub_82A3DF50.

**sub_82A3DF50 — Commit (3-line function)**

```
device[48] = device[13428]
```

Copies saved write position to current write position. No parameters beyond r3=device.

**sub_82A3E348 — DrawIndexedVertices (3 callers)**

| Reg | Type | Meaning |
|-|-|-|
| r3 | GuestDevice* | Device context |
| r4 | uint32_t | Primitive type (low 6 bits) |
| r5 | uint32_t | Start index |
| r6 | uint32_t | Index count |
| r7 | uint32_t | Total indices (for 16/32-bit batching if >65535) |

### State Setting Functions

**sub_82A3BF50 — SetShader (the ACTUAL shader bind)**

| Reg | Type | Meaning |
|-|-|-|
| r3 | GuestDevice* | Device context |
| r4 | uint32_t | Shader type: 0=VS slot 0, 1-3=VS slots 1-3, 4=PS |
| r5 | uint32_t* | Shader object pointer |

Stores at `device[(shaderType + 3108) * 4]`.

**sub_82A44B78 — SetTexture (CONFIRMED)**

| Reg | Type | Meaning |
|-|-|-|
| r3 | GuestDevice* | Device context |
| r4 | uint32_t | Texture slot index (0-19) |
| r5 | uint32_t* | Texture object pointer (0 = unbind) |

Stores at `device[(slotIndex + 3134) * 4]`. Sets dirty bit in device+24.

**sub_82A3B690 — SetRenderTarget (CONFIRMED)**

| Reg | Type | Meaning |
|-|-|-|
| r3 | GuestDevice* | Device context |
| r4 | uint32_t | RT index (0-3) |
| r5 | uint32_t* | Surface pointer |
| r6 | uint32_t | Mip offset |
| r7 | uint32_t | Flags (MSAA resolve mode) |

**sub_82A3B7B0 — SetDepthStencil (CONFIRMED)**

| Reg | Type | Meaning |
|-|-|-|
| r3 | GuestDevice* | Device context |
| r4 | uint32_t* | Surface pointer |

**sub_82A3E7A0 — PM4 VS register writer (NOT SetVertexShader)**

| Reg | Type | Meaning |
|-|-|-|
| r3 | GuestDevice* | Device context |
| r4 | uint32_t | Pass bitmask / shader resource token (NOT a shader pointer) |
| r5 | - | UNUSED |

Writes PM4 packet `{0xC0006000, device[12700]}`. Shader handle read from device, NOT from r4.

**sub_82A42760 — SetViewport**

| Reg | Type | Meaning |
|-|-|-|
| r3 | GuestDevice* | Device context |
| r4 | uint32_t* | Pointer to viewport state struct (NOT scalar x/y/w/h) |

**sub_82A424A8 — SetScissorRect**

| Reg | Type | Meaning |
|-|-|-|
| r3 | GuestDevice* | Device context |
| r4 | uint32_t* | Pointer to scissor rect state struct |

**sub_82A3A890 — SetVertexDeclaration**

| Reg | Type | Meaning |
|-|-|-|
| r3 | GuestDevice* | Device context |
| r4 | uint32_t | Vertex decl handle (0xFFFF = reset/none) |

## GuestDevice Context Layout (~19KB at 0x831C22A4)

| Offset | Hex | Content |
|-|-|-|
| +0/+8/+16/+24/+32 | 0x00-0x20 | 5 x uint64 dirty flag bitmasks |
| +48 | 0x30 | Command buffer write pointer |
| +52 | 0x34 | Command buffer end |
| +56 | 0x38 | Command buffer soft limit |
| +1152 | 0x480 | Texture fetch constants (24B x 32 slots) |
| +1792 | 0x700 | VS float constants (256 x vec4) |
| +5888 | 0x1700 | PS float constants (256 x vec4) |
| +10272 | 0x2820 | Sampler states (16B each) |
| +10456 | 0x28D8 | Vertex declaration handle |
| +10932 | 0x2AB4 | Current vertex shader handle |
| +10936 | 0x2AB8 | Current pixel shader handle |
| +12020 | 0x2EF4 | Stream source 0 (vertex buffer) |
| +12376 | 0x3058 | Viewport (6 floats) |
| +12428 | 0x308C | Depth stencil surface ptr |
| +12432 | 0x3090 | Shader params slot 0 (VS) |
| +12448 | 0x30A0 | Shader params slot 4 (PS) |
| +12452 | 0x30A4 | Render target slots (4 x 4B ptrs) |
| +12536 | 0x30F8 | Texture slots (19 x 4B ptrs) |
| +12700 | 0x319C | VS shader cache |
| +12704 | 0x31A0 | PS shader cache |
| +12708 | 0x31A4 | Shader valid flag |
| +13428 | 0x3474 | Saved command buffer write position |
| +13500 | 0x34BC | GPU state ptr |
| +14888 | 0x3A28 | GPU context ptr |
| +14912 | 0x3A40 | Buffer segment base |

## Rendering Globals (0x831C0000 region)

| Address | Offset | Name | Status |
|-|-|-|-|
| 0x831C22A0 | +0x22A0 | Alternate device ptr | Set |
| 0x831C22A4 | +0x22A4 | D3D device context (GuestDevice*) | Set |
| 0x831C23D0 | +0x23D0 | Command buffer swap lock (secondary gate) | Stuck non-zero |
| 0x831C2408 | +0x2408 | render_initialized flag | Unknown |
| 0x831C2458 | +0x2458 | Scene override ptr (sm_OverrideScene) | Always NULL by design |
| 0x831C2910 | +0x2910 | Current viewport ptr | Corrupt vtable (0x87000000) |
| 0x831C2D48 | +0x2D48 | Default viewport ptr | Unknown |
| 0x831C2DA8 | +0x2DA8 | grcDevice singleton | Created, vtable OK |
| 0x82B0B48C | - | Render gate (viewport count) | Working (1/-1 cycle) |

## Render Gate Mechanism

Address 0x82B0B48C, loaded via `lis r, -32079` (0x82B10000) + offset -19316.

- VdSwap hook writes 1 after Present
- sub_828C15C8 reads gate: if > 0, calls sub_828C1228 (render body)
- sub_828C1228 loops 'gate' times, then resets gate to -1
- Cycle is stable: `before=-1, after=1` every frame

## Secondary Gate (0x831C23D0)

Command buffer swap lock. Set by sub_828C01E0 (BeginCommandBuffer), cleared by sub_828C0338 (EndCommandBuffer). sub_828C0338 hangs because its D3D callees (sub_82A479B0, sub_82A47C80, sub_82A4A130) dereference Xenos device objects with garbage vtables. None are hooked.

Fix: `PPC_STORE_U32(0x831C23D0, 0)` in VdSwap after Present.

## Viewport Object Corruption (0x831C2910)

The MISSING-FUNC crashes (0x87000000, 0x00000000, 0x0000043F) come from object at 0x831C2910 (NOT 0x831C2DA8). sub_828C2178 reads from 0x831C2910, set by sub_828C2140. The vtable pointer is 0x87000000 — Xbox 360 GPU register space leaking into a C++ vtable field.

The object at 0x831C2DA8 is fine (constructor sub_828DC4D8 properly writes vtable 0x82096084).

Fix: Null-guard on sub_828C1228 checking 0x831C2DA8 vtable range:
```cpp
PPC_FUNC_IMPL(__imp__sub_828C1228);
PPC_FUNC_HOOK(sub_828C1228) {
    uint32_t obj = PPC_LOAD_U32(0x831C2DA8);
    if (obj == 0) { PPC_STORE_U32(0x82B0B48C, (uint32_t)-1); return; }
    uint32_t vt = PPC_LOAD_U32(obj);
    if (vt < 0x82000000 || vt > 0x82FFFFFF) { PPC_STORE_U32(0x82B0B48C, (uint32_t)-1); return; }
    __imp__sub_828C1228(ctx, base);
}
```

## RT Creation Register Bug

sub_828BEC78 hook reads wrong registers:

| Register | Hook reads as | Actually is |
|-|-|-|
| r3 | device (ignored) | **width** |
| r4 | width | **height** |
| r5 | (unread) | depth |
| r6 | height | **mip levels** |
| r7 | (unread) | array size |
| r8 | format | format (correct) |
| r9 | MSAA | MSAA (correct) |
| r10 | type | type (correct) |

All 20 render targets created as Nx1 pixels. Fix: r3=width, r4=height.

## Shader Pipeline

### Shader Cache

1137 entries across 79 FXC groups in `LibertyRecompLib/shader/shader_cache.cpp` (32MB). Pre-compiled SPIR-V + AIR (Metal). DXIL all zeros (Windows backend has no shaders). Created offline by XenosRecomp from extracted Xenos microcode.

Each `ShaderCacheEntry` has: XXH3 hash, SPIR-V/DXIL/AIR offsets+sizes, filename (e.g. `shaders/gta_default/gta_default_ps7.bin`), guestShader back-pointer.

### Shader Creation Flow

Normal flow: FXC loader parses .fxc files → calls sub_82A42BA8 (CreateShaderFromBytecode) → hook hashes Xenos bytecode via XXH3 → binary search in cache → creates GuestShader with linked ShaderCacheEntry → returns guest-mapped pointer.

### Current Blocker: FXC Loader Functions Stubbed

Four shader loading functions are overridden by PPC_FUNC_HOOK stubs in video.cpp:

| Address | Name | Purpose | Stub behavior |
|-|-|-|-|
| 0x82858758 | FXC preload | Opens .fxc, parses, calls sub_82A42BA8 per VS/PS | Returns 0 |
| 0x8285BDC8 | FXC file parser | Core .fxc binary container parser | Returns 1 |
| 0x828574A0 | Shader reload | Hot-swap shaders at runtime | No-op |
| 0x8285DF10 | Shader fixup | Post-load fixup processor | No-op |

RexGlue recompiles ALL functions from the XEX — these functions have recompiled implementations. The PPC_FUNC_HOOK stubs override them with strong symbols. LibertyRecomp uses rexglue in partial mode (runtime only, no GPU emulation). Full rexglue (runtime + GPU emulation) runs GTA IV completely.

**Fix**: Remove the PPC_FUNC_HOOK stubs. The recompiled FXC loading code will run freely, call sub_82A42BA8 (which IS hooked to use the shader cache), and create GuestShader objects naturally. Keep sub_828574A0 (reload) stubbed as it passes NULL causing `(null).sps` paths.

### Alternative: Manual Shader Creation (Option C)

`CreateShadersForFxc(name)` (video.cpp:8729) iterates cache entries by filename prefix and creates GuestShader objects. Exists but is never called. Could be used to pre-create all shaders at init.

`ScreenShaderInit` (video.cpp:7072) is a working example of manual shader creation: allocates GuestShader, gets guest handle via `g_memory.MapVirtual()`, writes handle into game structure.

### Missing: Shader Enqueue in Draw Hooks

FlushRenderStateForMainThread only flushes constants, booleans, and samplers — NOT shader pointers. SetVertexShader/SetPixelShader are never called because no hook binds them. Draw hooks must read device+10932 (VS) and device+10936 (PS), look up GuestShader via GTAIV::LookupShader(), and call SetVertexShader/SetPixelShader before enqueuing draws.

## Sonic '06 Address Contamination

All hooks at 0x8253xxxx-0x826Fxxxx were Sonic '06 (MarathonRecomp) addresses copy-pasted into LibertyRecomp. 0/24 exist in GTA IV's binary. All 31 SetRenderState hooks, all resource binding hooks, all conditional rendering hooks were dead code that never fired. Removed 2026-03-31.

## grcSetup_rage Vtable (0x82093B24, 6 vfuncs)

| Slot | Address | Name |
|-|-|-|
| 0 | 0x828C5840 | Init / CreateDevice |
| 1 | 0x828C59E0 | UpdateTimer / BeginFrame |
| 2 | 0x828C5A98 | UpdateRenderTimer |
| 3 | 0x828C5B08 | BeginScene / Clear |
| 4 | 0x828C5BA0 | Present / EndScene + Swap |
| 5 | 0x828C5E58 | Shutdown / DestroyDevice |

## grcDeviceXenon Vtable (0x82096084, 32 slots)

Purely a texture/render-target factory. NO draw/state functions. Draw calls go through the Xenos GPU context directly.

Key slots: [4]=GetBackBuffer, [14]=CreateRenderTarget, [15]=SetRenderTarget, [16]=ClearRenderTarget, [23]=LookupTexture, [24]=LoadTextureDict.

## grmGeometryQB Vtable (0x82097764, 18 vfuncs)

| Slot | Address | Name |
|-|-|-|
| 0 | 0x828E6A98 | Draw (primary indexed) |
| 1 | 0x828E7000 | GetVertexBuffer/Lock |
| 13 | 0x828E5CB0 | DrawSkinned (batched) |
| 14 | 0x828E6770 | DrawUnskinned |
| 15 | 0x828E7410 | DrawInstanced |
| 16 | 0x828E5A40 | DrawWithMaterial |

## Complete Draw Call Chain (verified)

```
CViewportGame::Render (0x82155640)
  → CRenderPhaseDrawScene::ProcessDrawList (0x8244E688)
    → CDrawEntityDC::Execute (0x821BB6D8)
      → sub_8235CF60 (model draw)
        → sub_828C19C0 (SetTechnique / shader bind)
          → sub_828C8588 (bind constants/textures)
        → sub_828C60C0 (geometry submit)
          → sub_828C9090 (DrawPrimitive wrapper)
            → sub_828DFF50 (DrawIndexedPrimitive)
              → sub_82A42168/338/3F8 (PM4 packets)
            → sub_828E01E0 (DrawPrimitive)
              → sub_82A42250/398/450 (PM4 packets)
```

## PM4 Safety Audit

All PM4 paths covered. sub_82A492A8 and sub_82A499B8 intercept ALL packet building. Only 3 functions directly call sub_82A492A8: sub_82A46330 (hooked), sub_82A49CB0 (hooked), sub_82A4F3C8 (calls through to hooked sub_82A492A8). No ring buffer corruption risk.

## SPS Database (Shader Preset System)

131 entries in `sps_preset_table.h` covering all GTA IV shader categories. sub_82869F30 hook populates guest shader DB from embedded table (no file I/O). sub_82869620 hook provides lazy FXC effect creation via shader manager vtable dispatch.

Warning: One agent found sub_82869620 may be hooking the WRONG function (a VMX geometry/collision function). Needs verification.

## Remaining Fixes Needed (Priority Order)

1. **Add missing shader functions to codegen** — Add 0x82858758, 0x8285BDC8, 0x828574A0, 0x8285DF10 to `[functions]` in `gta4_config.toml`, re-run codegen, then remove PPC_FUNC_HOOK stubs so recompiled FXC loading runs
2. **Add shader enqueue to draw hooks** — Read device+10932/+10936, LookupShader, call SetVertexShader/SetPixelShader before DrawPrimitive
3. **Fix RT register mapping** — r3=width, r4=height in sub_828BEC78
4. **Null-guard sub_828C1228** — Check 0x831C2DA8 vtable validity before calling __imp__
5. **Clear secondary gate** — PPC_STORE_U32(0x831C23D0, 0) in VdSwap
6. **Verify sub_82869620 hook address** — May be on wrong function (collision vs SPS lookup)
