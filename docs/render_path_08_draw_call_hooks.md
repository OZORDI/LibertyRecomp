# Render Path 08 — D3D Draw Call Hooks Audit

## Summary

All D3D draw call hooks are **real, functional implementations** that submit work to the host GPU
via a render command queue. There are two layers of hooks: Sonic 06-era (disabled via `#if 0`) and
GTA IV-specific (active). The active hooks enqueue `RenderCommand` structs consumed by a dedicated
render thread that issues Metal/D3D12/Vulkan draw calls.

---

## Hook Registry

### Draw Calls

| Address | Name | Layer | Active | Submits GPU Work |
|-|-|-|-|-|
| `sub_826FEC28` | `DrawPrimitive` | Sonic 06 | **No** (`#if 0`) | N/A |
| `sub_826FF030` | `DrawIndexedPrimitive` | Sonic 06 | **No** (`#if 0`) | N/A |
| `sub_826FE5C0` | `DrawPrimitiveUP` | Sonic 06 | **No** (`#if 0`) | N/A |
| `sub_82A49CB0` | `DrawPrimitive` | GTA IV | **Yes** | **Yes** — enqueues `RenderCommandType::DrawPrimitive` |

### Present

| Address | Name | Layer | Active | Submits GPU Work |
|-|-|-|-|-|
| `sub_825586B0` | `Video::Present` | Sonic 06 | **No** (`#if 0`) | N/A |
| `sub_82A467D8` | GTA IV Present wrapper | GTA IV | **Yes** | **Yes** — calls `Video::Present()` |

### Render Target / Depth

| Address | Name | Layer | Active | Submits GPU Work |
|-|-|-|-|-|
| `sub_82543EE0` | `SetRenderTarget` | Sonic 06 | **No** (`#if 0`) | N/A |
| `sub_825444F0` | `SetRenderTarget` | Sonic 06 | **No** (`#if 0`) | N/A |
| `sub_82544210` | `SetDepthStencilSurface` | Sonic 06 | **No** (`#if 0`) | N/A |
| `sub_82A3B690` | `SetRenderTarget` | GTA IV | **Yes** | **Yes** — enqueues `SetRenderTarget` cmd |
| `sub_82A3B7B0` | `SetDepthStencilSurface` | GTA IV | **Yes** | **Yes** — enqueues `SetDepthStencilSurface` cmd |

### Clear

| Address | Name | Layer | Active | Submits GPU Work |
|-|-|-|-|-|
| `sub_82555B30` | `Clear` | Sonic 06 | **No** (`#if 0`) | N/A |
| (via `Video::Present`) | Purple heartbeat clear | GTA IV | **Yes** | **Yes** — `clearColor` / `clearDepthStencil` |

### PM4 Bypass (no-ops)

| Address | Name | Active | Purpose |
|-|-|-|-|
| `sub_82A492A8` | PM4 Packet Builder | **Yes** | No-op stub — returns `cmdPtr` unchanged, skips Xenos PM4 |
| `sub_82A499B8` | PM4 Buffer Flush | **Yes** | Resets write pointer only — no hardware submission |
| `sub_82A46330` | UnifiedDraw | **Yes** | No-op — prevents PM4 code path from executing |
| `sub_82A46578` | DrawSurface | **Yes** | No-op — prevents normal PM4 draw path |

---

## Detailed Analysis

### 1. `DrawPrimitive` (active: `sub_82A49CB0`)

**Real draw call.** Hooks to the same `DrawPrimitive()` function at line 5675.

- Calls `FlushRenderStateForMainThread()` — captures dirty shader constants, sampler states, booleans
- Enqueues `RenderCommandType::DrawPrimitive` with primitive type, start vertex, count
- **Render thread** (`ProcDrawPrimitive`, line 5699):
  - Sets primitive topology
  - `FlushRenderStateForRenderThread()` — binds pipeline, framebuffer, vertex/index buffers, constants
  - `commandList->drawInstanced(vertexCount, 1, startVertex, 0)` — **real host GPU draw**

### 2. `DrawIndexedPrimitive` (no active GTA IV hook)

The implementation exists (line 5721) and is fully functional:
- Enqueues `RenderCommandType::DrawIndexedPrimitive`
- Render thread calls `commandList->drawIndexedInstanced(primCount, 1, startIndex, baseVertex, 0)`
- **However**: only hooked via disabled Sonic 06 address `sub_826FF030`. No GTA IV address hooks it.
- GTA IV's indexed draw path goes through PM4 (`sub_82A46578` → `sub_82A492A8`) which is **stubbed to no-op**.

### 3. `DrawPrimitiveUP` (no active GTA IV hook)

Implementation exists (line 5757) and is fully functional:
- Copies vertex data via `g_intermediaryUploadAllocator`
- Handles quad lists, triangle fans, CSD filter state
- Render thread uploads to GPU buffer, calls `drawInstanced` or `drawIndexedInstanced`
- **However**: only hooked via disabled Sonic 06 address `sub_826FE5C0`. No GTA IV address hooks it.

### 4. `Video::Present` (active: `sub_82A467D8`)

**Real present.** The `PPC_FUNC_HOOK(sub_82A467D8)` at line 9392:

1. Increments `device[16544]` (frame counter for pacing)
2. Calls `Video::Present()` which:
   - Enqueues a purple screen clear (visual heartbeat)
   - Runs post-processing (SSAO, DoF, SSR, Bloom, Sun Shafts)
   - Enqueues `ExecuteCommandList` → render thread copies backbuffer to swapchain with gamma/HDR tonemap
   - **`g_swapChain->present()`** — real buffer swap
   - Advances frame index, waits on fence, resets allocators
3. Syncs `0x83124CCC` and `device[16552]` to prevent game frame-pacing stalls

### 5. `SetRenderTarget` (active: `sub_82A3B690`)

**Real bind.** Looks up `GuestSurface*` via `GTAIV::LookupSurface()`, then calls `SetRenderTarget()`:
- Enqueues `RenderCommandType::SetRenderTarget`
- Render thread updates `g_renderTarget`, pipeline format/sample count, alpha-to-coverage mode
- Only index 0 is handled; MRT (index > 0) asserts null

### 6. `SetDepthStencilSurface` (active: `sub_82A3B7B0`)

**Real bind.** Same pattern — lookup surface, enqueue command, render thread updates `g_depthStencil` and depth format.

### 7. `Clear` (no active GTA IV hook for game-issued clears)

The `Clear()` function (line 4569) is fully implemented:
- Enqueues `RenderCommandType::Clear`
- Render thread (`ProcClear`): validates textures, resolves pending stretch rects, sets framebuffer, calls `clearColor()` and/or `clearDepthStencil()`
- **Present** issues a hardcoded purple clear each frame — this is the only clear that fires

No GTA IV address hooks the `Clear` function (the `sub_82555B30` hook is Sonic 06, disabled).

---

## Rendering Gate Flags

| Flag | Location | Purpose |
|-|-|-|
| `g_readyForCommands` | line 874 | Set `true` after `BeginCommandList`; set `false` on `Present`. Not a gate for draw calls. |
| `g_swapChainValid` | line 426 | If `false`, Present drops frame (no swap). Draw calls still enqueue. |
| `g_backBuffer->texture` | checked in Present | If null (swapchain exhaustion), frame is dropped. |

There is **no global "rendering enabled/disabled" flag** that gates draw calls. All `DrawPrimitive`/`DrawIndexedPrimitive`/`DrawPrimitiveUP` calls always enqueue commands unconditionally.

---

## Key Finding: Draw Call Coverage Gap

The only **active** draw call hook for GTA IV is `sub_82A49CB0` → `DrawPrimitive`.
`DrawIndexedPrimitive` and `DrawPrimitiveUP` have no active GTA IV hooks.

GTA IV's draw path flows: game logic → `sub_82A46578` (DrawSurface) → PM4 packet builder → Xenos.
Since `sub_82A46578` and `sub_82A492A8` are stubbed to no-ops, the PM4 draw path is silently
discarded. The `sub_82A49CB0` DrawPrimitive hook catches calls at a higher abstraction layer.

**Result**: The engine produces a purple screen heartbeat clear + post-processing on each Present,
confirming the GPU pipeline works. Game geometry rendering depends on whether `sub_82A49CB0`
intercepts enough of the draw dispatch to produce visible output.

---

## Frame Lifecycle

```
Game thread                          Render thread
──────────                           ─────────────
SetRenderTarget (sub_82A3B690)   →   ProcSetRenderTarget (bind RT)
SetDepthStencil (sub_82A3B7B0)   →   ProcSetDepthStencilSurface
DrawPrimitive   (sub_82A49CB0)   →   ProcDrawPrimitive (drawInstanced)
  ...more draws...
Present         (sub_82A467D8)   →   ProcClear (purple)
                                     Post-processing (SSAO/DoF/SSR/Bloom)
                                     ProcExecuteCommandList (gamma → swapchain)
                                     swapChain->present()
```
