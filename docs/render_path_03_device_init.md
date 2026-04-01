# Render Path 03: D3D Device Creation & GPU Initialization

## 1. Boot Sequence Overview

```
main.cpp:829  Video::CreateHostDevice()     ← host Metal/Vulkan/D3D12 device + swapchain
main.cpp:847  KiSystemStartup()             ← BSS zero, XEX header copy
main.cpp:853  LdrLoadModule()               ← decompresses PE into guest memory
main.cpp:~870 runtime->LaunchModule()        ← spawns XThread → xstart
              └─ sub_82A507A8               ← guest-side GPU init (calls kernel Vd* funcs)
                 ├─ VdInitializeEngines()
                 ├─ VdSetGraphicsInterruptCallback()
                 └─ sub_82A49D08 (ring buffer / device init)
```

**Key insight:** The host device (`g_device`, `g_swapChain`) is created **before** guest code runs. Guest GPU init only populates the ~19KB guest device context structure and registers MMIO callbacks. There are two independent devices: a real host GPU device and a fake guest D3D device.

---

## 2. VdInitializeEngines — GPU MMIO Registration

**File:** [imports.cpp](../LibertyRecomp/kernel/imports.cpp#L356-L369)

```cpp
uint32_t VdInitializeEngines() {
    auto* ks = rex::system::kernel_state();
    if (ks && ks->memory()) {
        ks->memory()->AddVirtualMappedRange(
            0x7FC80000, 0xFFFF0000, 0x10000,
            nullptr, GpuMmioRead, GpuMmioWrite);
    }
    return 1;
}
```

Registers MMIO range `0x7FC80000–0x7FC8FFFF` with callbacks:

| Register (offset/4) | Value | Purpose |
|-|-|-|
| `0x1951` | `1` | Interrupt status — vblank active |
| `0x0F00` | `0x08100748` | RB_EDRAM_TIMING |
| `0x0F01` | `0x0000200E` | RB_BC_CONTROL |
| `0x194C` | `0x000002D0` | D1MODE_V_COUNTER |
| `0x1961` | `0x050002D0` | AVIVO_D1MODE_VIEWPORT_SIZE (1280×720) |

Without this, `PPC_MM_LOAD_U32(0x7FC86544)` returns 0 and VBlank dispatch in `sub_829D7368` never fires, starving all worker threads.

---

## 3. Guest D3D Device Creation Functions

### 3.1 sub_82A50890 — Top-Level GPU CreateDevice
**File:** [imports.cpp](../LibertyRecomp/kernel/imports.cpp#L1087-L1093)

Diagnostic passthrough hook — logs entry/exit, calls original PPC code. This is the outermost guest function that creates the Xbox 360 D3D device context (~19KB `GuestDevice` structure).

### 3.2 sub_82A416B8 — D3D Device Setup (caller of sub_82A50890)
**File:** [imports.cpp](../LibertyRecomp/kernel/imports.cpp#L1098-L1105)

Another diagnostic passthrough. Calls `sub_82A50890` internally.

### 3.3 sub_82A49D08 — Ring Buffer / Device Init
**File:** [imports.cpp](../LibertyRecomp/kernel/imports.cpp#L1112-L1127)

Diagnostic passthrough. Sets up PM4 command buffer pointers in the guest device context:
- `device[+14888]` (`GpuContextPtr`) — immutable allocation base for PM4 buffer
- `device[+10900]` (`IndexBufferBase`) — base address for indexed draws
- `device[+48]`/`[+52]`/`[+56]` — command buffer write ptr, end, limit

### 3.4 sub_82A49C38 — GPU Sync (NOT CreateDevice)
**File:** [imports.cpp](../LibertyRecomp/kernel/imports.cpp#L552-L555)

```cpp
PPC_FUNC_HOOK(sub_82A49C38) {
    uint32_t deviceCtx = ctx.r3.u32;
    if (deviceCtx != 0) { PPC_STORE_U32(deviceCtx + 11000, 0); }
}
```

Initially misidentified as CreateDevice. Actually a per-frame GPU command sync function. Clears `device[+11000]` to prevent spinlocks.

### 3.5 sub_82A507A8 — GTA IV Device Init Orchestrator
**File:** [video.cpp](../LibertyRecomp/gpu/video.cpp#L8686-L8689)

Not hooked — relies on kernel hooks for VdInitializeEngines and VdSetGraphicsInterruptCallback. The guest PPC code runs as-is.

---

## 4. Guest Device Context Globals

Defined in [gtaiv_device.h](../LibertyRecomp/gpu/gtaiv_device.h#L15-L73):

| Offset | Name | Set During Init | Purpose |
|-|-|-|-|
| +48 | `CommandBufferPtr` | sub_82A49D08 | Current PM4 write position |
| +52 | `CommandBufferEnd` | sub_82A49D08 | Buffer segment end |
| +56 | `CommandBufferLimit` | sub_82A49D08 | Soft flush trigger |
| +10900 | `IndexBufferBase` | sub_82A49D08 | Indexed draw base addr |
| +14888 | `GpuContextPtr` | sub_82A49D08 | Immutable PM4 buffer base |
| +16544 | `FrameCounter` | Present hook | Frames presented |
| +16552 | `FrameSubmitted` | Present hook | GPU ring completion counter |
| +19480 | `FrameBufferIndex` | — | Current backbuffer index |

Guest memory global `0x83124CCC` — frame pacing gate used by `sub_828507F8`.

---

## 5. VBlank & Interrupt System

**File:** [imports.cpp](../LibertyRecomp/kernel/imports.cpp#L266-L330)

`VdSetGraphicsInterruptCallback()` stores the callback/userData and starts an `XHostThread` ("GPU VSync") at 60 Hz:
- Sets `0x83128A80` (GUEST_FRAME_READY_FLAG) = 1 each tick
- Calls `ks->processor()->ExecuteInterrupt()` with args `{0, userData}` → dispatches to `sub_82A46098`
- `sub_82A46098` increments FrameSubmitted/FrameCompleted counters

### VBlank Callback Fixup (sub_82A487B8)
**File:** [imports.cpp](../LibertyRecomp/kernel/imports.cpp#L607-L639)

When `device[+10900]` == 0 (before ring buffer init), allocates a 64-byte zero-filled guest stub to prevent null dereference in the recomp'd VBlank handler.

---

## 6. Host Device Creation (Metal/Vulkan/D3D12)

**File:** [video.cpp](../LibertyRecomp/gpu/video.cpp#L2040-L2282)

`Video::CreateHostDevice()` runs **before** any guest code. It:

1. Creates `RenderInterface` (tries Metal on macOS, Vulkan fallback)
2. Creates `RenderDevice` via `g_interface->createDevice()`
3. Creates `RenderCommandQueue` (DIRECT + COPY)
4. Creates `RenderSwapChain` connected to `GameWindow::s_renderWindow`
5. Creates `g_backBuffer` (`GuestSurface`) pointing at swapchain texture
6. Creates descriptor sets, shader pipelines, upload buffers

**Yes, the host device IS created and connected to a swapchain.** The render interface (Metal on macOS) creates a real GPU device. The swapchain is valid at:

```cpp
g_swapChain = g_queue->createSwapChain(
    RenderSwapChainDesc(GameWindow::s_renderWindow, g_backbufferFormat, bufferCount, false, Config::MaxFrameLatency));
```

Host-side globals created during init:

| Global | Type | Purpose |
|-|-|-|
| `g_device` | `RenderDevice` | Host GPU device (Metal/Vulkan) |
| `g_swapChain` | `RenderSwapChain` | Connected to SDL window |
| `g_backBuffer` | `GuestSurface*` | Points at swapchain texture |
| `g_queue` | `RenderCommandQueue` | Main render queue |
| `g_commandLists[2]` | `RenderCommandList` | Double-buffered cmd lists |
| `g_textureDescriptorSet` | `RenderDescriptorSet` | Bindless texture table |

---

## 7. GPU Function Hook/Stub Summary (video.cpp + imports.cpp)

### Kernel-Level Stubs (imports.cpp)

| Function | Guest Address | Treatment |
|-|-|-|
| `VdInitializeEngines` | kernel import | MMIO registration |
| `VdInitializeRingBuffer` | kernel import | STUB |
| `VdEnableRingBufferRPtrWriteBack` | kernel import | STUB |
| `VdSetGraphicsInterruptCallback` | kernel import | Starts VBlank thread |
| `VdShutdownEngines` | kernel import | STUB |
| `VdQueryVideoMode` | kernel import | Returns 1280×720 |
| `VdInitializeScalerCommandBuffer` | kernel import | STUB |
| `VdRetrainEDRAM` | kernel import | Returns 0 |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | kernel import | STUB |
| `sub_82A4EDC8` | 0x82A4EDC8 | Cmd buffer drain — syncs write/read ptr |
| `sub_82A486F0` | 0x82A486F0 | GPU atomic sync — no-op |
| `sub_82A49C38` | 0x82A49C38 | GPU sync — clears device[+11000] |
| `sub_82A41320` | 0x82A41320 | GPU fence — force done after 50 calls |
| `sub_82A46098` | 0x82A46098 | VBlank frame completion — passthrough |
| `sub_82A487B8` | 0x82A487B8 | VBlank callback — null-deref fixup |
| `sub_82A50890` | 0x82A50890 | GPU CreateDevice — diagnostic passthrough |
| `sub_82A416B8` | 0x82A416B8 | D3D device setup — diagnostic passthrough |
| `sub_82A49D08` | 0x82A49D08 | Ring buffer init — diagnostic passthrough |
| `sub_82871180` | 0x82871180 | Render state submission — no-op (SIGBUS) |

### GPU Hooks (video.cpp)

| Function | Guest Address | Treatment | Purpose |
|-|-|-|-|
| `sub_82A467D8` | 0x82A467D8 | PPC_FUNC_HOOK | Present — increments FrameCounter, calls Video::Present() |
| `sub_82A42BA8` | 0x82A42BA8 | PPC_FUNC_HOOK | CreateShader — parses Xbox shader bytecode |
| `sub_82A50F28` | 0x82A50F28 | GUEST_FUNCTION_HOOK | GpuMemAlloc — returns fake offsets |
| `sub_82A44970` | 0x82A44970 | GUEST_FUNCTION_HOOK | CreateVertexBuffer |
| `sub_82A44850` | 0x82A44850 | GUEST_FUNCTION_HOOK | CreateTexture |
| `sub_828BEC78` | 0x828BEC78 | PPC_FUNC_HOOK | RAGE CreateRenderTarget |
| `sub_82A55DC0` | 0x82A55DC0 | PPC_FUNC_HOOK | Texture/RT surface allocation (PM4 bypass) |
| `sub_82A492A8` | 0x82A492A8 | PPC_FUNC_HOOK | PM4 packet builder — returns cmdPtr unchanged |
| `sub_82A499B8` | 0x82A499B8 | PPC_FUNC_HOOK | PM4 buffer flush — resets write pointers |
| `sub_82A49CB0` | 0x82A49CB0 | GUEST_FUNCTION_HOOK | DrawPrimitive |
| `sub_82A46330` | 0x82A46330 | PPC_FUNC_HOOK | UnifiedDraw — no-op |
| `sub_82A46578` | 0x82A46578 | PPC_FUNC_HOOK | DrawSurface — no-op |
| `sub_82A3E7A0` | 0x82A3E7A0 | PPC_FUNC_HOOK | SetVertexShader (validated via registry) |
| `sub_82A47AE0` | 0x82A47AE0 | PPC_FUNC_HOOK | SetBothShaders (VS+PS) |
| `sub_82A44B78` | 0x82A44B78 | PPC_FUNC_HOOK | SetTexture (validated via registry) |
| `sub_82A3B690` | 0x82A3B690 | PPC_FUNC_HOOK | SetRenderTarget |
| `sub_82A3B7B0` | 0x82A3B7B0 | PPC_FUNC_HOOK | SetDepthStencilSurface |
| `sub_82A42760` | 0x82A42760 | GUEST_FUNCTION_HOOK | SetViewport |
| `sub_82A424A8` | 0x82A424A8 | GUEST_FUNCTION_HOOK | SetScissorRect |
| `sub_82A3A890` | 0x82A3A890 | PPC_FUNC_HOOK | SetVertexDeclaration |
| `sub_828574A0` | 0x828574A0 | PPC_FUNC_HOOK | Shader system — no-op |
| `sub_8285BDC8` | 0x8285BDC8 | PPC_FUNC_HOOK | Returns 1 |
| `sub_8285DF10` | 0x8285DF10 | PPC_FUNC_HOOK | No-op |
| `sub_82858758` | 0x82858758 | PPC_FUNC_HOOK | Returns 0 |
| `sub_82869F30` | 0x82869F30 | PPC_FUNC_HOOK | (GPU-related) |
| `sub_82869620` | 0x82869620 | PPC_FUNC_HOOK | (GPU-related) |
| `sub_828507F8` | 0x828507F8 | PPC_FUNC_HOOK | Frame presentation (fix throttle) |

### Shared Graphics Layer Hooks (video.cpp, 0x825xxxxx range)

| Function | Guest Address | Treatment | Purpose |
|-|-|-|-|
| `sub_82547118` | 0x82547118 | GUEST_FUNCTION_HOOK | CreateVertexDeclaration |
| `sub_82548700` | 0x82548700 | GUEST_FUNCTION_HOOK | CreateVertexShader |
| `sub_82548608` | 0x82548608 | GUEST_FUNCTION_HOOK | CreatePixelShader |
| `sub_82546EE0` | 0x82546EE0 | PPC_FUNC_HOOK | SetVertexShader (shared layer) |
| `sub_82546BD8` | 0x82546BD8 | PPC_FUNC_HOOK | SetPixelShader (shared layer) |
| `sub_82543918` | 0x82543918 | PPC_FUNC_HOOK | SetStreamSource |
| `sub_82543AC8` | 0x82543AC8 | PPC_FUNC_HOOK | SetIndices |
| `sub_82636BF8` | 0x82636BF8 | GUEST_FUNCTION_HOOK | BeginConditionalSurvey |
| `sub_82636C08` | 0x82636C08 | GUEST_FUNCTION_HOOK | EndConditionalSurvey |
| `sub_82636C10` | 0x82636C10 | GUEST_FUNCTION_HOOK | BeginConditionalRendering |
| `sub_82636C18` | 0x82636C18 | GUEST_FUNCTION_HOOK | EndConditionalRendering |
| Render states | 30+ hooks | GUEST_FUNCTION_HOOK | SetRenderState<D3DRS_*> |

### XeInitializeRenderTarget / Xe* Functions
**No `XeInitializeRenderTarget` or similar `Xe*` GPU functions are hooked or stubbed.** The codebase has no references to `XeInitializeRenderTarget`. The Xenos GPU emulation is handled entirely through:
- MMIO callbacks (`GpuMmioRead`/`GpuMmioWrite`)
- PM4 command buffer stubbing (`sub_82A492A8`, `sub_82A499B8`)
- VBlank interrupt simulation

---

## 8. Why Rendering Doesn't Work (Analysis)

The host device **is** created and connected to a valid swapchain. The gap is in the **render command dispatch**:

1. **Guest draw calls (`sub_82A46330`, `sub_82A46578`) are no-op'd** — they don't dispatch to the host render queue
2. **`sub_82A49CB0` (DrawPrimitive) is hooked to host `DrawPrimitive`** via GUEST_FUNCTION_HOOK, but the calling convention may not match GTA IV's wrapper
3. **Present path works:** `sub_82A467D8` → `Video::Present()` is correctly wired and calls `g_swapChain->present()`
4. **Shader binding hooks validate via registry** but may reject all shaders if the shader cache isn't populated with GTA IV shaders (only pre-compiled shaders from the installer are registered)

The core issue is likely that no draw calls reach the host command list between `CheckSwapChain()` and `Video::Present()`.
