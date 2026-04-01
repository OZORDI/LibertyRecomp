# Render / Frame Loop — Full Trace

## 1. Main Loop Structure (Original Game Flow)

The game's main loop is **not** sub_82856F08 or sub_8218BEA8. Both are dead code in the
recompiled binary (see doc 68). The actual per-frame tick is:

```
sub_82142230  (main game state machine, gta4_recomp.0.cpp)
  -> sub_82849918  (yield / sleep)
  -> sub_821428C8  (per-frame tick, 17 subsystem calls)
       -> sub_8214C8C8  (game update dispatcher, 58 callees)
            -> sub_829025A8  (scene graph manager)
                 -> [indirect vtable dispatch] -> sub_8291E620 (scene renderer)
```

The imports.cpp hook at line 2643 (`PPC_FUNC_HOOK(sub_8218BEA8)`) calls
`__imp__sub_82856F08` in a while(true) loop, but both symbols are dead-stripped
by the linker. sub_82856F08 is an offset inside sub_82856D48, a **base64/encoding
function** (not a frame tick). sub_8218BEA8 has no generated entry point.

### The real per-frame tick: sub_821428C8

Called every yield iteration from sub_82142230. Executes 17 subsystems per frame:

| # | Function | System |
|-|-|-|
| 1 | sub_821B38D8 | Timer/clock update |
| 2 | sub_8214C8C8 | Main game update (58 callees, deepest path) |
| 3 | sub_82205850 | World/scene update |
| 4 | sub_821B71B0 | Streaming tick (flag-gated by 0x82B053E4) |
| 5 | sub_821B5A68 | Text/locale (flag-gated) |
| 6 | sub_8222DAA0 | World manager (flag-gated) |
| 7 | sub_82145820 | DLC/post-load (flag-gated) |
| 8 | sub_826CDEB8 | Save/profile system |
| 9 | sub_8222E760 | World streaming finalize (flag-gated) |
| 10 | sub_8222DE88 | Streaming flush |
| 11 | sub_821B3958 | Resource manager tick |
| 12 | sub_821B3A70 | Memory/heap maintenance |
| 13 | sub_8222E338 | Streaming post-tick |
| 14 | sub_821B5A08 | Timer/profiling |
| 15 | sub_821B3B80 | Allocator maintenance |
| 16 | sub_822BCA90 | Network/multiplayer tick |
| 17 | sub_821B5890 | End-of-frame cleanup |

Flag at 0x82B053E4 gates subsystems 4-7, 9. Set to 1 after scene creation completes
(around yield #13000). Before that, only 10 of 17 subsystems run.

---

## 2. Frame Submission Call Chain

The render path from game code to host Present:

```
sub_828C5BA0  (frame timing / vsync wrapper)
  -> sub_828C15C8  (RAGE render dispatch — THE central render function)
       -> sub_828C9980  (grcEffect::SetVariable, shader param setter, 93 call sites)
       -> sub_82A3B7B0  (device state reset)
       -> sub_82A3B690  (render target setup, called 4x for RT 0-3)
       -> sub_82A42760  (viewport setup)
       -> sub_82A424A8  (scissor rect setup)
       -> sub_82A44B78  (texture sampler setup, loop 0-19)
       -> sub_82A42930  (depth stencil setup)
       -> sub_82A3CC68  (DrawPrimitive — actual draw call submission)
       -> sub_828BF420  (GPU ring buffer flush / frame pacing)
       -> sub_82A49C38  (GPU sync bypass)
  -> returns to caller
```

### sub_828C15C8 internals (render dispatch)

**File**: `gta4_recomp.58.cpp` line 115160, spans ~500 lines.

1. Calls sub_822BCA90 (network tick)
2. Loads device ptr from `*(0x831C22A4)` (grcDevice, offset 8868 from 0x831C0000)
3. Resets device state via sub_82A3B7B0
4. Sets up 4 render targets (sub_82A3B690 x4)
5. Configures viewport and scissor
6. Loops samplers 0-19 via sub_82A44B78
7. Sets depth stencil state
8. Increments frame counter at `0x82B09D14`
9. **Gate check**: loads `*(0x82B0B48C)` — if > 0, enters the scene render path:
   - Loads scene list ptr from `*(0x831C2458)` (offset 9304 from 0x831C0000)
   - If non-null: `vtable = *sceneList; func = vtable[64]; call func(sceneList)`
   - This dispatches to grcSceneList::Render()
10. If no scene, falls through to clear-only path via sub_82A3CC68
11. Calls sub_828BF420 (flush) and sub_82A49C38 (sync)

### sub_828BF420 (GPU frame pacing)

**File**: `gta4_recomp.58.cpp` line 110109

- Reads frame ready flag from `*(0x831C2460)` (offset 9312)
- If flag == 1: calls sub_82A46DA0 (begin frame swap), clears flag
- Calls sub_82A46D70 to read frame delta from device
- **Spin-waits** while `device[FrameCounter] - device[FrameSubmitted] >= 2`
  - Fixed by hook in video.cpp: keeps FrameSubmitted = FrameCounter - 1
- Proceeds to build render command for current frame

### sub_82A467D8 (D3D Present wrapper — hooked)

**File**: `gta4_recomp.71.cpp` line 30184 (original), hooked in `video.cpp` line 9392

Original PPC code:
- Increments `device[16544]` (FrameCounter)
- Builds PM4 swap command (resolve EDRAM, set display source, swap)
- Writes to GPU ring buffer

Hook replacement:
- Increments FrameCounter manually
- Calls `Video::Present()` (host-side Metal/Vulkan/D3D12 present)
- Syncs `0x83124CCC = FrameCounter` (prevents sub_828507F8 from blocking)
- Sets `device[16552] = FrameCounter - 1` (prevents sub_828BF420 spin-wait)

---

## 3. Scene Graph Initialization

### scene@0x831C2458 — What It Is

Address `0x831C2458` = offset 9304 from the RAGE global data base at `0x831C0000`.
It is a slot in an **array of grcSceneList pointers** within the render device globals.

The render dispatch (sub_828C15C8) reads this slot:
```
r11 = 0x831C2458           // scene list array
r3  = *(r11)               // first scene list ptr
r11 = *(r3)                // vtable
r11 = *(r11 + 64)          // vtable[16] = Render() virtual
call r11                   // grcSceneList::Render()
```

**Current state**: `*(0x831C2458) = 0x00000000` (NULL). The render dispatch gate at
0x82B0B48C is > 0, so the code enters the scene render path, but immediately skips
because the scene list pointer is null. Result: blank purple frames (clear color only).

### Who Creates the Scene Object

**sub_827ADB48** (grcSceneList constructor):
- **File**: `gta4_recomp.50.cpp` line 8045
- Takes `this` pointer + 10 parameters (display params)
- Calls sub_82A00DC0 (memcpy) 6 times to copy display configuration
- Calls sub_827AD9C8 (grcSceneNode base constructor) which calls sub_827AD200
- Sets vtable to `0x82078D48` (grcSceneList vtable)
- Returns constructed object in r3

**Caller**: sub_82478AF8 (audio engine init, line 84737 in gta4_recomp.20.cpp):
```
r3 = sub_827ADB48(...)          // construct scene list
*(0x82FF5368) = r3              // store in global 'g_scene'
```

The scene object is created and stored at global `0x82FF5368`, but it is **never
registered** into the device scene list array at `0x831C2458`. That registration
happens during D3D device initialization, which is fully stubbed in the recomp.

### grcSceneList vtable hierarchy

| Address | Class | Constructor |
|-|-|-|
| 0x82078D28 | grcSceneList variant | sub_827ADAC8 |
| 0x82078D38 | grcSceneNode (base) | sub_827AD9C8 |
| 0x82078D48 | grcSceneList (full) | sub_827ADB48 |

---

## 4. All Globals That Must Be Non-Null for Rendering

### RAGE Global Data Area (base 0x831C0000)

| Address | Offset | Name | Purpose | Current State |
|-|-|-|-|-|
| 0x831C22A0 | 8864 | grcDevice secondary | Secondary device reference | May be set |
| 0x831C22A4 | 8868 | grcDevice primary | THE D3D device pointer | Set by stubs |
| 0x831C2290 | 8848 | Effect constant | Shader effect config | Unknown |
| 0x831C23C8 | 9160 | RT index | Current render target index | Written by sub_828C15C8 |
| 0x831C23E8 | 9192 | RT array base | Render target pointer array | Null (no real RTs) |
| 0x831C2458 | 9304 | Scene list array | **CRITICAL** — scene ptrs | **NULL** |
| 0x831C2460 | 9312 | Frame ready flag | Signals frame begin | Set by sub_828BF420 |
| 0x831C24A4 | 9380 | Post-frame callback | Called after flush | Unknown |
| 0x831C25A8 | 9640 | Scene info struct | Scene metadata | Null |
| 0x831C2728 | 10024 | Shader cache | grcEffect cache | Created lazily |
| 0x831C2740 | 10048 | Render state table | 37-entry dispatch | Populated by sub_828C19C0 |
| 0x831C3DDC | 15836 | Frame timing A | Profiling timestamp | Written |
| 0x831C3DE4 | 15844 | Frame timing B | Dirty flag / timing | Written |

### Frame Pacing Globals

| Address | Name | Purpose |
|-|-|-|
| 0x82B0B48C | Render gate | >0 enables scene render dispatch (set during init) |
| 0x82B09D14 | Frame counter | Incremented per frame by sub_828C15C8 |
| 0x83124CCC | Submitted counter | Must equal device FrameCounter for pacing |
| device+16544 | FrameCounter | Presented frame count |
| device+16552 | FrameSubmitted | GPU completion count (synced by hook) |
| 0x83128A80 | VBlank ready flag | Set by VBlank timer thread |

### Scene Object Globals

| Address | Name | Purpose |
|-|-|-|
| 0x82FF5368 | g_scene | Pointer to created grcSceneList object |
| 0x82B053E4 | Init complete flag | Gates conditional subsystems (streaming, world mgr) |

---

## 5. Host-Side Present Pipeline

`Video::Present()` in `video.cpp` line 3377:

1. Calls `KernelPhase_EnterRuntime()` on first invocation
2. Updates post-process AA (TAA jitter)
3. Validates swapchain (drops frame if invalid or starved)
4. Clears backbuffer to purple (0.1, 0.0, 0.2) — visual heartbeat
5. Executes pending stretch rect commands
6. Extracts camera from vertex shader constants
7. Applies post-processing (SSAO, DoF, SSR — if enabled and depth available)
8. Draws ImGui overlay
9. Presents to swapchain

The purple clear is intentional — it confirms the host rendering pipeline works.
Game content is absent because no draw calls are submitted by the guest code
(scene list is null, no shader/vertex/index buffers are bound).

---

## 6. What a Native Rewrite Needs

### Root Cause of Blank Frames

The scene list pointer at `0x831C2458` is NULL because:

1. Scene objects ARE created (sub_827ADB48 runs, stores result at 0x82FF5368)
2. But they are never **registered** into the device scene list array at 0x831C2458
3. Registration happens during D3D device initialization (grcDevice::Create or equivalent)
4. D3D device init is fully stubbed (VdInitializeEngines just registers MMIO range)
5. Without registration, sub_828C15C8 skips the scene render dispatch entirely

### The Registration Gap

Between:
- `sub_827ADB48` creating grcSceneList objects (called from sub_82478AF8)
- The render dispatch reading `*(0x831C2458)` in sub_828C15C8

There must be a function that writes the scene object pointer into the device's
scene list array. This function runs as part of the original D3D device startup
and was never executed because the GPU subsystem is stubbed.

### What Needs to Happen

To get non-blank frames, one of two approaches:

**Approach A: Bridge the registration gap**
- Find the scene registration function (writes scene ptr into 0x831C2458)
- Hook it or call it manually after sub_827ADB48 returns
- Ensure `*(0x831C2458) = *(0x82FF5368)` (copy scene ptr into device array)
- This alone is insufficient — the scene objects also need valid render targets,
  shaders, vertex buffers, etc. to produce actual geometry

**Approach B: Bypass the guest render entirely**
- The host-side rendering (Metal/Vulkan/D3D12) already has:
  - Swapchain management (works)
  - Shader translation (XenosRecomp)
  - Texture/buffer management (GuestTexture, GuestBuffer)
  - Draw call infrastructure (RenderCommand queue)
- Instead of fixing the guest scene list, intercept draw calls at the
  GPU command buffer level (sub_82A3CC68 = DrawPrimitive) and translate
  them to host draw calls
- This is the approach RexGlue already takes for other recomp projects

### Critical Missing Pieces

1. **Scene registration**: Write `*(0x82FF5368)` into `*(0x831C2458)`
2. **Render target creation**: The RT array at 0x831C23E8 needs valid entries
3. **Shader binding**: grcEffect instances need vtable pointers (currently null,
   causing 2.3M MISSING-FUNC calls per frame via sub_828C9980)
4. **Vertex/Index buffers**: Need host-side counterparts via GuestBuffer
5. **Draw call translation**: sub_82A3CC68 needs to produce host RenderCommands
6. **Depth buffer**: Required for post-processing (SSAO, DoF)

### Frame Loop Summary

```
[Guest Thread]                              [Host Thread]
sub_82142230 (state machine)
  -> sub_821428C8 (frame tick)
       -> sub_8214C8C8 (game update)
            -> scene graph + physics + AI
       -> sub_828C15C8 (render dispatch)
            -> *(0x831C2458) == NULL          Video::Present()
            -> skip scene render                -> clear purple
            -> sub_82A3CC68 (clear only)        -> present to swapchain
            -> sub_828BF420 (flush)
            -> sub_82A467D8 (present hook)
                 -> Video::Present() --------->
```

The game loop runs. Frames are submitted. The host presents. But frames are blank
because the scene list is null and no draw calls reach the host renderer.
