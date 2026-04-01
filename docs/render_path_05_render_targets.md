# Render Path 05 — Render Targets & Back Buffer Resolve

## 1. Guest Render Targets → Host Textures

The project **does not emulate Xbox 360 EDRAM tiles**. Instead, GTA IV's D3D wrapper functions are hooked to create native GPU resources directly.

### CreateSurface ([video.cpp:4034](../LibertyRecomp/gpu/video.cpp#L4034))

```cpp
static GuestSurface* CreateSurface(uint32_t width, uint32_t height,
    uint32_t format, uint32_t multiSample, GuestSurfaceCreateParams* params)
```

- Creates a `GuestSurface` with a host `RenderTexture` (via `g_device->createTexture()`)
- Surface cache (`g_surfaceCache`) reuses surfaces matching width/height/format/base
- Registers via `GTAIV::RegisterSurface(guestAddr, surface)` for handle translation
- MSAA: `multiSample=1` → 2x, else → 4x

### RAGE CreateRenderTarget — `sub_828BEC78` ([video.cpp:9273](../LibertyRecomp/gpu/video.cpp#L9273))

```cpp
PPC_FUNC_HOOK(sub_828BEC78)
// r4=width, r6=height, r8=D3DFMT, r9=MSAA, r10=type (1=tex2D, 3=cube, else=surface)
// sp+84 = output handle pointer
```

- Bypasses the entire Xenos GPU chain (`sub_82A55538`, PM4 commands)
- Type 1/3 → creates `GuestTexture` with `RenderTextureFlag::NONE`
- Else → creates `GuestSurface` via `CreateSurface()` with `RENDER_TARGET` or `DEPTH_TARGET` flag

### Texture/RT Allocation — `sub_82A55DC0` ([video.cpp:9535](../LibertyRecomp/gpu/video.cpp#L9535))

- Allocates guest memory via `sub_821B3608`, then creates a host `GuestTexture`
- Clamps garbage dimensions (>16384) to 32×32
- Registers texture under both physical address and virtual address

## 2. XeInitializeRenderTarget / XeSetRenderTarget / XeResolve*

**None of these exist.** The project hooks GTA IV's D3D wrapper layer directly, not the Xenos GPU API. There is no Xenos register-level emulation.

## 3. Active GTA IV D3D Hooks

| Guest Address | Function | Location |
|-|-|
| `sub_82A3B690` | SetRenderTarget(device, index, surface) | [video.cpp:9836](../LibertyRecomp/gpu/video.cpp#L9836) |
| `sub_82A3B7B0` | SetDepthStencilSurface(device, surface) | [video.cpp:9851](../LibertyRecomp/gpu/video.cpp#L9851) |
| `sub_828BEC78` | RAGE CreateRenderTarget factory | [video.cpp:9273](../LibertyRecomp/gpu/video.cpp#L9273) |
| `sub_82A55DC0` | Texture/RT surface allocation | [video.cpp:9535](../LibertyRecomp/gpu/video.cpp#L9535) |
| `sub_82A467D8` | Present → `Video::Present()` | [video.cpp:9392](../LibertyRecomp/gpu/video.cpp#L9392) |
| `sub_82A46330` | UnifiedDraw — **STUBBED (no-op)** | [video.cpp:9716](../LibertyRecomp/gpu/video.cpp#L9716) |
| `sub_82A46578` | DrawSurface — **STUBBED (no-op)** | [video.cpp:9730](../LibertyRecomp/gpu/video.cpp#L9730) |

**Sonic 06 hooks** (sub_82543EE0 SetRenderTarget, sub_82543B58 GetBackBuffer, sub_825575B8 StretchRect, etc.) are **all inside `#if 0` and disabled** ([video.cpp:9460](../LibertyRecomp/gpu/video.cpp#L9460)).

## 4. gtaiv_render_state.cpp/h

[gtaiv_render_state.h](../LibertyRecomp/gpu/gtaiv_render_state.h) / [gtaiv_render_state.cpp](../LibertyRecomp/gpu/gtaiv_render_state.cpp)

Tracks guest device context state extraction:

- **EDRAM offsets**: `DeviceOffsetExt::EdramBase` (+1784), `EdramPitch` (+1788), `SurfaceFormat` (+1792)
- **`XenosSurfaceFormat` enum**: `R8G8B8A8` (0x06), `R10G10B10A2` (0x1A), `R16G16B16A16_F` (0x1F), `D24S8` (0x28), `D24FS8` (0x29)
- **`RenderState::Extract(device)`** — pulls dirty flags, shaders, render targets, texture slots, blend state, EDRAM, frame counter from device context
- **`DirtyBit` bitmask** at `device+16` (64-bit) — tracks RT changes, shader changes, etc.
- **HDR passthrough** — `IsCurrentRenderTargetHDR()` checks surface format for 10-bit or 16-bit float
- **State change detection** — `DetectStateChanges()` compares against `g_lastState`

**This module is informational/diagnostic — it does not drive actual rendering.**

## 5. gfx_state.cpp/h

[gfx_state.h](../LibertyRecomp/gpu/gfx_state.h) / [gfx_state.cpp](../LibertyRecomp/gpu/gfx_state.cpp)

Host-side resource handle translation maps:

- `RegisterShader/LookupShader/UnregisterShader` — `unordered_map<uint32_t, GuestShader*>`
- `RegisterBuffer/LookupBuffer/UnregisterBuffer` — `unordered_map<uint32_t, GuestBuffer*>`
- `RegisterTexture/LookupTexture/UnregisterTexture` — `unordered_map<uint32_t, GuestTexture*>`
- `RegisterSurface/LookupSurface/UnregisterSurface` — `unordered_map<uint32_t, GuestSurface*>`

Key function: `GetTextureLevelInfo(texturePtr, mipLevel)` — replaces `sub_829E5C38` for render target backup creation (`sub_8286BAE0`).

Draw call tracking: `BeginDrawCall/EndDrawCall`, render pass management (`BeginRenderPass/EndRenderPass`).

## 6. Back Buffer → Swap Chain Present Path

### Frame setup — `BeginCommandList()` ([video.cpp:1948](../LibertyRecomp/gpu/video.cpp#L1948))

```
g_renderTarget = g_backBuffer
g_backBuffer->texture = g_intermediaryBackBufferTexture   (when swapChainValid)
```

- Creates `g_intermediaryBackBufferTexture` at viewport resolution with `BACKBUFFER_FORMAT` (B8G8R8A8_UNORM or R16G16B16A16_FLOAT for HDR)
- All game rendering targets this intermediary texture

### Present — `Video::Present()` ([video.cpp:3377](../LibertyRecomp/gpu/video.cpp#L3377))

1. Enqueues purple clear (visual heartbeat: `{0.1, 0.0, 0.2, 1.0}`)
2. Enqueues `ExecutePendingStretchRectCommands` (MSAA resolve)
3. Applies post-processing (SSAO, DoF, SSR, Bloom, Sun Shafts)
4. Enqueues `ExecuteCommandList`

### Final blit — `ProcExecuteCommandList()` ([video.cpp:3700](../LibertyRecomp/gpu/video.cpp#L3700))

```
intermediaryBackBuffer → [gamma correction or HDR tonemap shader] → swapChainTexture
```

- Creates framebuffer wrapping `swapChainTexture`
- Barriers: intermediary → SHADER_READ, swap chain → COLOR_WRITE
- Full-screen triangle draw (6 verts) with gamma/HDR constants
- Final barrier: swap chain → PRESENT
- `g_swapChain->present(backBufferIndex, signalSemaphores)`

## 7. StretchRect — The "Resolve" Step

### Guest-side StretchRect ([video.cpp:4134](../LibertyRecomp/gpu/video.cpp#L4134))

The Xbox 360's `XeResolve` (EDRAM → texture copy) is implemented as `StretchRect`:

```cpp
static void StretchRect(GuestDevice* device, uint32_t flags, ..., GuestTexture* texture, ..., uint32_t destSliceOrFace)
```

- `flags & 0x4` → depth stencil, else → color render target
- Links `texture->sourceSurface = surface` and `surface->destinationTextures[texture] = slice`

### MSAA Resolve ([video.cpp:4299](../LibertyRecomp/gpu/video.cpp#L4299))

`ExecutePendingStretchRectCommands()` handles the actual copy:

- **Hardware resolve** (non-depth or depth with `resolveModes` capability): `commandList->resolveTexture()`
- **Shader resolve** (fallback): Uses `g_resolveMsaaColorShaders[n]` / `g_resolveMsaaDepthPipelines[n]` for 2x/4x/8x MSAA
- Resolve shaders exist for Vulkan (SPIRV), Metal (MSL), and D3D12 (DXIL)

### Pending resolve tracking

- `g_pendingResolves` — set of `GuestSurface*` needing MSAA resolve
- `g_pendingSurfaceCopies` — set of surfaces with pending copy commands
- Barriers managed by `PopulateBarriersForStretchRect()`

## 8. Critical Finding — Why No Visible Frames

The draw path is broken:

1. **`sub_82A46330` (UnifiedDraw) — STUBBED to no-op** ([video.cpp:9716](../LibertyRecomp/gpu/video.cpp#L9716))
2. **`sub_82A46578` (DrawSurface) — STUBBED to no-op** ([video.cpp:9730](../LibertyRecomp/gpu/video.cpp#L9730))
3. `GUEST_FUNCTION_HOOK(sub_82A49CB0, DrawPrimitive)` is active but requires valid shaders + vertex declarations

The game boots, `Video::Present()` runs, the purple heartbeat clear goes to the swap chain, but **no game geometry is drawn** because the two primary draw dispatch functions are no-op stubs. The render target pipeline is wired correctly — surfaces are created, SetRenderTarget works, StretchRect/resolve logic exists — but nothing draws into the render targets.

## 9. RexGlue Render Target Cache (Reference Only)

The Vulkan render target cache at [glue/rexglue-sdk-main/src/graphics/vulkan/render_target_cache.cpp](../glue/rexglue-sdk-main/src/graphics/vulkan/render_target_cache.cpp) and [header](../glue/rexglue-sdk-main/include/rex/graphics/vulkan/render_target_cache.h) is a Xenia-derived EDRAM emulation system with full resolve shaders (clear, fast, full variants for 8/16/32/64/128bpp at 1x/2x/4x MSAA). **This is NOT used by LibertyRecomp** — the project uses direct host GPU resources instead of EDRAM emulation.
