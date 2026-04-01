# GTA IV Rendering Architecture — Static Recomp D3D Hook Map

> Research doc: RTTI, vtables, call graph, pseudocode, and generated code cross-referenced
> No GPU emulation — all rendering goes through high-level D3D function hooks

## Architecture Overview

The GTA IV rendering pipeline is built on RAGE engine abstractions. The game never calls D3D directly — it goes through `rage::grcSetup` / `rage::grmSetup` vtable dispatch, which bottleneck into ~30 D3D-equivalent functions. LibertyRecomp hooks those functions and redirects them to the host GPU (Metal/Vulkan/D3D12 via plume abstraction).

```
Game Logic (PPC recompiled)
  └─ RAGE Engine (grcSetup / grmSetup vtable dispatch)
       └─ rage::grcDevice methods (SetRenderState, SetTexture, DrawPrimitive, etc.)
            └─ LibertyRecomp GUEST_FUNCTION_HOOKs
                 └─ RenderCommand queue → Host GPU (Metal/Vulkan/D3D12)
```

## Class Hierarchy (from RTTI + vtables)

### Setup Classes (render pipeline control)

| Class | Vtable | Role |
|-|-|-|
| grmSetup_rage | 0x82000588 | Base. vf[0]=DeviceCreate(828C5ED8), vf[4]=Present(828C5BA0) |
| grcSetup_rage | 0x82093B24 | Derived. vf[0]=Init(828C5840), vf[1]=FrameTime(828C59E0), vf[4]=Present(828C5BA0) |

grcSetup vtable (6 vfuncs):
| vf | Address | Role |
|-|-|-|
| [0] | 0x828C5840 | Init — calls device ctor (828DC890 or 828E7630), then 828C2A48 + 828E09A0 |
| [1] | 0x828C59E0 | FrameTime — timebase (mftb) delta calculation, smoothing filter |
| [2] | 0x828C5A98 | Unknown (loc, not function) |
| [3] | 0x828C5B08 | PreRender — calls 828BF898, then 828BF120 for tile setup |
| [4] | 0x828C5BA0 | **Present** — THE frame callback. Calls sub_828C15C8 (frame setup) |
| [5] | 0x828C5E58 | Cleanup — destroys render target pool, calls device dtor via vtable |

### Device Classes

| Class | Vtable | Role |
|-|-|-|
| grcRenderTarget_rage | 0x82093715 | Base render target (36 vfuncs) |
| grcRenderTargetXenon_rage | 0x82096170 | Xbox360-specific RT (36 vfuncs) |
| grcTexture_rage | 0x82093694 | Base texture |
| grcTextureXenon_rage | 0x82096105 | Xbox360-specific texture |
| grcIndexBuffer_rage | — | Index buffer base |
| grcIndexBufferD3D_rage | — | D3D index buffer |
| grcVertexBuffer_rage | — | Vertex buffer base |
| grcVertexBufferD3D_rage | — | D3D vertex buffer |

### Shader Classes

| Class | Vtable | Key Methods |
|-|-|-|
| grmShaderFx_rage | 0x82093DB4 | vf[2]=828C7A30 (param binding), 21 vfuncs |
| grmShaderFactoryStandard_rage | 0x82094B75 | Factory for shader creation |
| grmShaderGroup_rage | 0x82094D8B | Shader group collection |

### Viewport Classes (base CViewport at 0x8200E002, 12 vfuncs)

| Class | Vtable | Init |
|-|-|-|
| CViewportGame | 0x820B8F54 | 0x82155600 — main gameplay viewport |
| CViewportScripted | 0x820212EC | Cutscene/scripted viewports |
| CViewportRadar | 0x820B8F18 | Minimap |
| CViewportPrimaryOrtho | 0x820B8F94 | 2D overlay |
| CViewportMobilePhone | 0x820B935C | In-game phone |
| CViewportFrontend3DScene |0x820B9961 | Menu 3D backgrounds |
| CViewportHtml | 0x820BE289 | HTML UI elements |

### Render Phase Classes (base CRenderPhase at 0x8201A9CC, 14 vfuncs)

All share vf[4]=0x82174900 (common dispatch). Differ in vf[6] (actual render):

| Class | Vtable | vf[6] Render | Purpose |
|-|-|-|-|
| CRenderPhaseDrawScene | 0x82013140 | 0x8244E688 | Main 3D scene |
| CRenderPhaseBlit | 0x82013000 | 0x8244E688 | Blit/copy |
| CRenderPhaseFrontEnd | 0x820B9AF8 | 0x8244E688 | UI overlay |
| CRenderPhaseScript2d | 0x82013100 | 0x8235E468 | 2D scripted |
| CRenderPhaseHeight | 0x820434C8 | 0x826637C8 | Terrain height |
| CRenderPhaseDeferredLighting_GBuffer | 0x82044D48 | 0x8267D340 | G-buffer pass |
| CRenderPhaseDeferredLighting_Screen | 0x82044D88 | 0x8244E688 | Deferred lighting |
| CRenderPhaseWaterReflection | 0x820234A8 | 0x8244E688 | Water reflect |
| CRenderPhaseWaterSurface | 0x82023534 | 0x82521990 | Water surface |
| CRenderPhaseCloudGeneration | 0x82023578 | 0x82521D10 | Clouds |
| CRenderPhaseRainUpdate | 0x820235BC | 0x82521BD8 | Rain effects |
| CRenderPhaseReflection | 0x8202364E | 0x82522DB8 | Generic reflect |
| CRenderPhaseInteriorReflection | 0x82023694 | 0x82522FE0 | Indoor reflect |
| CRenderPhaseWarpShadow | 0x82023801 | 0x82524320 | Shadow maps |
| CRenderPhaseMirrorReflection | 0x820234EC | 0x8244E688 | Mirror |
| CRenderPhaseHtml | 0x820BE335 | 0x8244E688 | HTML render |
| CRenderPhasePlayerSettings | 0x820B9B54 | — | Per-player settings |
| CRenderPhasePhoneModel | 0x820B9AF8 | — | Phone 3D model |
| CRenderPhasePreRenderViewport | 0x82013080 | — | Pre-viewport setup |
| CRenderPhasePostRenderViewport | 0x820130C0 | — | Post-viewport cleanup |
| CRenderPhaseSetDefaultRenderState | 0x82013040 | — | State reset |

## Frame Pipeline (call graph + pseudocode)

```
grcSetup::vfunc[4] (sub_828C5BA0) — called every frame by engine
  └─ sub_828C15C8 — frame setup
       ├─ sub_828C9980 — scene list slot manager (0x831C25A8)
       ├─ if gate (0x82B0B48C) > 0:
       │    ├─ sub_828C1228 — main viewport render loop (iterates gate-count times)
       │    │    ├─ sub_82A3E7A0 — set render target state per viewport
       │    │    ├─ scene = 0x831C2458 ?: vtable[4](device@831C2DA8, 0)
       │    │    ├─ vtable[16](scene) — render into scene
       │    │    └─ sub_82A3CC68 — draw command dispatch
       │    └─ sub_82A3EDA8 — finalize viewport state
       └─ sub_828BF420 — present/swap
            ├─ sub_82A46D70 — wait for GPU ready
            └─ sub_82A467D8 — VdSwap (frame presentation)
```

### Draw Command Dispatch (sub_82A3CC68)

The central rendering dispatcher. 12 callees:
```
sub_82A3CC68
  ├─ sub_82A38F28 — primitive setup
  ├─ sub_82A3C598 — render state setup
  ├─ sub_82A3C6F8 — render state cleanup
  ├─ sub_82A43BE0 — shader binding
  ├─ sub_82A46FB0 — draw call submission
  ├─ sub_82A48B90 — buffer management
  ├─ sub_82A499B8 — device state query (20+ callers)
  ├─ sub_82A4ACF0 — vertex buffer operations
  └─ sub_82A4B088 — index buffer operations
```

## D3D Function Hook Map (Complete)

### Resource Creation

| Guest Function | Hook | Host Implementation |
|-|-|-|
| sub_8253A8D8 | CreateTexture | CreateTexture2D via plume |
| sub_82A44850 | CreateTexture (rage wrapper) | Same, through rage::grcTextureFactory |
| sub_8253B508 | CreateVertexBuffer | createBuffer (vertex) |
| sub_82A44970 | CreateVertexBuffer (rage wrapper) | Same |
| sub_8253B640 | CreateIndexBuffer | createBuffer (index) |
| sub_8253A9F8 | CreateSurface | Surface creation |
| sub_825471F8 | GetVertexDeclaration | Input layout query |

### State Setting

| Guest Function | Hook | D3D9 Equivalent |
|-|-|-|
| sub_82543EE0 | SetRenderTarget | IDirect3DDevice9::SetRenderTarget |
| sub_825444F0 | SetRenderTarget (alt) | Same (different call site) |
| sub_82544210 | SetDepthStencilSurface | IDirect3DDevice9::SetDepthStencilSurface |
| sub_825436F0 | SetViewport | IDirect3DDevice9::SetViewport |
| sub_82543628 | SetScissorRect | IDirect3DDevice9::SetScissorRect |
| sub_8253AC40 | SetTexture | IDirect3DDevice9::SetTexture (sampler slot 0-15) |
| sub_826FEC28 | DrawPrimitive | IDirect3DDevice9::DrawPrimitive |
| sub_826FF030 | DrawIndexedPrimitive | IDirect3DDevice9::DrawIndexedPrimitive |
| sub_826FE5C0 | DrawPrimitiveUP | IDirect3DDevice9::DrawPrimitiveUP |
| sub_82555B30 | Clear | IDirect3DDevice9::Clear |
| sub_825575B8 | StretchRect | IDirect3DDevice9::StretchRect |

### Shader/Constants

| Guest Function | Hook | D3D9 Equivalent |
|-|-|-|
| (SetVertexShader) | SetVertexShader | IDirect3DDevice9::SetVertexShader |
| (SetPixelShader) | SetPixelShader | IDirect3DDevice9::SetPixelShader |
| (SetStreamSource) | SetStreamSource | IDirect3DDevice9::SetStreamSource |
| (SetIndices) | SetIndices | IDirect3DDevice9::SetIndices |
| (SetVertexDecl) | SetVertexDeclaration | IDirect3DDevice9::SetVertexDeclaration |
| (SetVSConstants) | SetVertexShaderConstants | IDirect3DDevice9::SetVertexShaderConstantF |
| (SetPSConstants) | SetPixelShaderConstants | IDirect3DDevice9::SetPixelShaderConstantF |
| (SetSamplerState) | SetSamplerState | IDirect3DDevice9::SetSamplerState |

### Render State (per-state hooks)

| Guest Function | Hook | D3D9 State |
|-|-|-|
| sub_82541700 | SetRenderState | D3DRS_ZENABLE |
| sub_82541780 | SetRenderState | D3DRS_ZWRITEENABLE |
| sub_825417B0 | SetRenderState | D3DRS_ZFUNC |
| sub_82541810 | SetRenderState | D3DRS_ALPHABLENDENABLE |
| sub_825418D0 | SetRenderState | D3DRS_SRCBLEND |
| sub_82541960 | SetRenderState | D3DRS_DESTBLEND |
| sub_825419C0 | SetRenderState | D3DRS_BLENDOP |
| sub_82541A20 | SetRenderState | D3DRS_ALPHATESTENABLE |
| sub_82541AA0 | SetRenderState | D3DRS_ALPHAREF |
| sub_82541AE0 | SetRenderState | D3DRS_ALPHAFUNC |
| sub_82542090 | SetRenderState | D3DRS_CULLMODE |
| sub_82542050 | SetRenderState | D3DRS_COLORWRITEENABLE |
| sub_82541B30+ | SetRenderState | D3DRS_STENCIL* (12 states) |
| sub_82541E38 | SetRenderState | D3DRS_CLIPPLANEENABLE |

### Resource Lock/Unlock

| Guest Function | Hook | D3D9 Equivalent |
|-|-|-|
| sub_82A479B0 | LockTextureRect | IDirect3DTexture9::LockRect |
| sub_82A47AE0 | UnlockTextureRect | IDirect3DTexture9::UnlockRect |
| sub_82A47C80 | LockVertexBuffer | IDirect3DVertexBuffer9::Lock |
| sub_82A47E28 | UnlockVertexBuffer | IDirect3DVertexBuffer9::Unlock |
| sub_8253A740 | LockTextureRect (rage) | Same (rage path) |
| sub_82538D30 | UnlockTextureRect (rage) | Same |
| sub_8253B5D0 | LockVertexBuffer (rage) | Same |
| sub_8253B630 | UnlockVertexBuffer (rage) | Same |
| sub_8253B6F0 | LockIndexBuffer | IDirect3DIndexBuffer9::Lock |
| sub_8253B750 | UnlockIndexBuffer | IDirect3DIndexBuffer9::Unlock |

### Presentation

| Guest Function | Hook | D3D9 Equivalent |
|-|-|-|
| sub_825586B0 | Video::Present | IDXGISwapChain::Present |
| sub_82543B58 | GetBackBuffer | IDirect3DDevice9::GetBackBuffer |
| sub_82543BA0 | GetDepthStencil | IDirect3DDevice9::GetDepthStencilSurface |
| sub_82A44A98 | GetSurfaceDesc | IDirect3DSurface9::GetDesc |
| sub_82A467D8 | VdSwap (PPC_FUNC_HOOK) | Kernel frame swap |
| sub_8253AE98 | DestructResource | Release() |

### GPU Pipeline Control (no D3D equivalent — Xbox 360 specific)

| Guest Function | Hook | Purpose |
|-|-|-|
| sub_82A4EDC8 | GPU Ring Buffer drain | No-op (no Xenos hardware) |
| sub_82A486F0 | GPU Atomic Sync | No-op |
| sub_82A49C38 | GPU Sync bypass | Zeroes device slot |
| sub_82A41320 | GPU Fence completion | Forces complete after tight loop |
| sub_828507F8 | Frame sync throttle | Present check |
| sub_829A0678 | HDCP bypass | No-op (no HDCP on PC) |
| sub_82A46098 | VBlank interrupt | Frame counter dispatch |
| sub_82A487B8 | VBlank callback | Frame-done event |
| sub_82871180 | Render state submit | No-op (no Xenos cmd buffer) |
| sub_828574A0 | Shader upload | No-op (immutable shaders) |
| sub_8285BDC8 | Shader check | Returns 1 |
| sub_8285DF10 | Shader flush | No-op |
| sub_82858758 | Shader query | Returns 0 |
| sub_82A50F28 | GPU memory alloc | Stub |
| sub_82A55DC0 | render target op | Custom |
| sub_82A492A8 | device method | Custom |
| sub_82A499B8 | device state query | Custom (20+ callers) |
| sub_82A46330 | display method | Custom |
| sub_82A46578 | display method | Custom |

### Stubs

| Guest Function | Purpose |
|-|-|
| sub_82543BE0 | SetGammaRamp (no-op) |
| sub_82543C68 | SetGammaRamp (no-op) |
| sub_82547278 | Set shader allocation (no-op) |
| sub_8253EB38 | Unknown (no-op) |
| sub_8253EB78 | Unknown (no-op) |
| sub_82700C18 | D3DXFilterTexture (no-op) |
| sub_82558E00 | Unknown (no-op) |
| sub_82559928 | Unknown (no-op) |
| sub_82559C18 | Unknown (no-op) |
| sub_8254D598 | BeginConditional (no-op) |
| sub_8254D7B0 | BeginConditional (no-op) |
| sub_8254D9D0 | BeginConditional (no-op) |
| sub_8254DB90 | BeginConditional (no-op) |
| sub_8254DD40 | SetScreenExtentQueryMode (no-op) |

### Render Target Creation (special hook)

sub_828BEC78 (CreateRAGERT) — PPC_FUNC_HOOK that intercepts rage::grcRenderTarget creation. Called ~20 times during init for shadow maps, water, reflections, post-fx.

## Kernel Video Exports (Vd* functions)

All Vd* kernel calls the game makes and their implementation status:

| Import | Status | Implementation |
|-|-|-|
| VdSwap | Implemented | Writes GPU swap packet to ringbuffer (xboxkrnl_video.cpp:374) |
| VdQueryVideoMode | Implemented | Returns 1280x720, 60Hz, widescreen |
| VdQueryVideoFlags | Implemented | Derives from VideoMode |
| VdSetDisplayMode | Implemented | Adjusts mode |
| VdInitializeEngines | Implemented | Registers GPU MMIO range 0x7FC80000 |
| VdSetGraphicsInterruptCallback | Implemented | Sets VBlank callback + timer |
| VdInitializeRingBuffer | Implemented | Ringbuffer init |
| VdEnableRingBufferRPtrWriteBack | Implemented | Write-back setup |
| VdGetSystemCommandBuffer | Implemented | Returns system cmd buffer |
| VdSetSystemCommandBufferGpuIdentifierAddress | Implemented | Sets GPU ID addr |
| VdInitializeScalerCommandBuffer | Implemented | Scaler init |
| VdCallGraphicsNotificationRoutines | Implemented | **Stub — returns 0** |
| VdIsHSIOTrainingSucceeded | Implemented | Returns 1 (always succeed) |
| VdPersistDisplay | Implemented | Returns 0 |
| VdRetrainEDRAM | Implemented | Returns 0 |
| VdRetrainEDRAMWorker | Implemented | Returns 0 |
| VdGetCurrentDisplayGamma | Implemented | Returns type=2, power=2.22222 |
| VdGetCurrentDisplayInformation | Implemented | Returns display info struct |
| VdShutdownEngines | Implemented | Shutdown sequence |
| VdEnableDisableClockGating | Implemented | Clock gating toggle |
| VdGetGraphicsAsicID | Implemented | Returns 0x10 |
| 40+ more | **Stubbed** | No-op (HDCP, movie capture, display discovery, etc.) |

## Generated Code File Map

| File | Functions | Role |
|-|-|-|
| gta4_recomp.58.cpp | 828BEC78, 828C1228, 828C15C8, 828C5BA0, 828C5ED8 | Frame pipeline, render loop, device create |
| gta4_recomp.59.cpp | 828DC4D8, 828DC890 | Device constructors |
| gta4_recomp.71.cpp | 82A3CC68, 82A416B8, 82A44850, 82A467D8, 82A492A8, 82A499B8, 82A49C38, 82A50890 | GPU ops, D3D setup, draw dispatch |

## Key Global State Addresses

| Address | Name | Purpose | Runtime State |
|-|-|-|-|
| 0x82B0B48C | Render gate / viewport count | Loop iteration count. -1 = init sentinel, 1+ = render | Stuck at -1 |
| 0x831C2458 | Scene pointer | Active scene object. Set by D3D kernel on Xbox | NULL |
| 0x831C2DA8 | Fallback device (grcDevice) | Created by 828DC890/828E7630 | **Live (0xD900B620)** |
| 0x831C245C | Secondary scene | Referenced in dword_831C245C check | — |
| 0x831C22A4 | D3D device context | Passed to draw commands | Populated |
| 0x831C22B0 | Viewport list | Per-viewport render target pointers | Populated |
| 0x831C23C8 | Triple-buffer index | Cycles 0→1→2→0 | Active |
| 0x831C23E8 | Triple-buffer array | 3 frontbuffer pointers | Populated |
| 0x831C2460 | Resolve flag | Triggers surface resolve before present | — |
| 0x831C23D4 | Render flags | Bit 2 controls post-loop fallback path | — |
| 0x831C2540 | Viewport slot ID | Selects between primary/secondary rendering params | — |
| 0x82B0B48C-B51C | Viewport config block | Clear color, viewport dimensions, etc. | — |
| 0x831C2F88 | Render target pool | Created during init, destroyed during cleanup | — |
| 0x831C2FE0 | Alternate device flag | If set, uses 828E7630 instead of 828DC890 | — |
| 0x831C3094 | Device branch flag | Checked during grcSetup::vfunc[0] init | — |

## Draw Command Classes (CDrawXxxDC)

18 draw command types, all with 5 vfuncs, dispatched through the render list:

| Class | Vtable | Purpose |
|-|-|-|
| CDrawEntityDC | 0x8200106C | World entities |
| CDrawPedDC | 0x82001588 | Pedestrians |
| CDrawPlayerDC | 0x8200156C | Player model |
| CDrawPedPropDC | 0x82001088 | Ped accessories |
| CDrawPedPropsDC | 0x8200154C | Multiple ped props |
| CDrawFragDC | 0x820015A4 | Fragment objects |
| CDrawFragTypeDC | 0x820010A4 | Fragment type objects |
| CDrawDefLight | 0x820010C0 | Deferred lights |
| CDrawWaterSurfaceDC | 0x820010F8 | Water surfaces |
| CDrawMobilePhoneCameraDC | 0x820012B8 | Phone camera view |
| CDrawPtxEffectInst | 0x82001328 | Particle effects |
| CDrawRadarMapSectionDC | 0x820013F8 | Radar map sections |
| CDrawRadarCircleDC | 0x82001430 | Radar circle overlay |
| CDrawRadioHudTextDC | 0x82001414 | Radio station text |
| CDrawSpriteDC | 0x82001468 | 2D sprites |
| CDrawRectDC | 0x820014BC | Rectangles |
| CDrawPolyLoadingClockDC | 0x820014F4 | Loading spinner |
| CDrawSkinnedEntityDC | 0x820015C0 | Skinned meshes |
