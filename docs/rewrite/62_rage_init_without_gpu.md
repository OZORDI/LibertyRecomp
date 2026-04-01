# 62: RAGE Engine Initialization Without GPU -- Research

## 1. Existing GPU/D3D Stubs Inventory

LibertyRecomp has an extensive set of GPU-related hooks in `LibertyRecomp/kernel/imports.cpp`
and `LibertyRecomp/gpu/video.cpp`. These fall into several categories:

### 1.1 Kernel-Level Video Driver Stubs (imports.cpp lines 228-396)

The entire Xbox 360 video driver (Vd*) API is stubbed:

| Function | Hook | Behavior |
|----------|------|----------|
| VdQueryVideoMode | GUEST_FUNCTION_HOOK | Returns 1280x720, 60Hz, HD, widescreen |
| VdSetGraphicsInterruptCallback | GUEST_FUNCTION_HOOK | Stores callback + starts VBlank timer |
| VdInitializeEngines | GUEST_FUNCTION_HOOK | Registers GPU MMIO range (0x7FC80000) |
| VdPersistDisplay | GUEST_FUNCTION_HOOK | Stub |
| VdSwap | GUEST_FUNCTION_HOOK | Stub (actual swap in sub_82A467D8) |
| VdGetSystemCommandBuffer | GUEST_FUNCTION_HOOK | Stub |
| VdEnableRingBufferRPtrWriteBack | GUEST_FUNCTION_HOOK | Stub |
| VdInitializeRingBuffer | GUEST_FUNCTION_HOOK | Stub |
| VdShutdownEngines | GUEST_FUNCTION_HOOK | Stub |
| VdRetrainEDRAM | GUEST_FUNCTION_HOOK | Sets edramTrainingComplete = true |
| VdGetGpuMemoryUsage | GUEST_FUNCTION_HOOK | Returns 0 |
| VdSetDisplayMode/Configuration | GUEST_FUNCTION_HOOK | Stub |
| VdPerformHardwareTest | GUEST_FUNCTION_HOOK | Returns 1 (pass) |
| VdGetHardwareStatus | GUEST_FUNCTION_HOOK | Returns 1 (OK) |

### 1.2 GPU Command Buffer / Fence Stubs (imports.cpp lines 535-577, 1106-1175)

| Function | Address | Behavior |
|----------|---------|----------|
| sub_82A4EDC8 | GPU CB drain | Copies write_ptr to read_ptr (no-op drain) |
| sub_82A486F0 | GPU atomic sync | No-op (r3 pass-through) |
| sub_82A49C38 | GPU sync bypass | Writes 0 to device+11000 |
| sub_82A41320 | GPU fence completion | Forces done after 50 consecutive calls |
| sub_828507F8 | Frame presentation | Pass-through (fix for throttle check) |
| sub_82A46098 | Frame swap | Return immediately (SIGBUS without GPU) |
| sub_829A0678 | HDCP bypass | Returns 0 |
| sub_82871180 | GPU render state | Returns 0 (SIGBUS on D3D context 0x83124AF4) |
| sub_8285D018 | Ring buffer submit+wait | Stubbed entirely (returns 0) |
| sub_8285C648 | GPU fence wait | Returns 1 (signaled immediately) |
| sub_8285CF98 | Fence create+wait | Returns 1 (signaled) |
| sub_828497D8 | NtWait dispatcher | Pass-through (non-GPU waits still work) |

### 1.3 GPU Shader/Device Init Stubs (imports.cpp lines 1178-1233)

| Function | Address | Behavior |
|----------|---------|----------|
| sub_8285B088 | Shader registration consumer | Pass-through (sub_8285A8B0 stubbed) |
| sub_8285A8B0 | GPU buffer flush | Stubbed entirely (no Xenos to submit to) |
| sub_82852B78 | ShaderBind | Pass-through (safe since sub_8285B088 stubbed) |

### 1.4 GPU Memory / Heap Hooks (imports.cpp lines 1063-1104)

| Function | Address | Behavior |
|----------|---------|----------|
| sub_82A10EB0 | RAGE GPU heap allocator | Routes through SystemHeapAlloc |
| sub_82A50F28 | GPU memory pool alloc | Returns sequential offsets (video.cpp) |
| sub_821B3608 | RAGE alloc dispatch | Pass-through with logging |

### 1.5 Native Rendering Hooks (video.cpp)

The actual rendering is handled host-side via video.cpp. Key hooks:

| Function | Address | Purpose |
|----------|---------|---------|
| sub_82A467D8 | D3D Present wrapper | Increments frame counter + calls Video::Present() |
| sub_82A55DC0 | Texture/RT alloc | Allocates real host textures via render device |
| sub_82A44850 | CreateTexture | GUEST_FUNCTION_HOOK creating GuestTexture |
| sub_82A44970 | CreateVertexBuffer | GUEST_FUNCTION_HOOK creating GuestBuffer |
| sub_82A479B0 | LockTextureRect | Native texture lock |
| sub_82A47C80 | LockVertexBuffer | Native buffer lock |

### 1.6 Resource Dictionary Guard (imports.cpp line 1124-1143)

**sub_827A9A20** (binary search in resource dictionary): When the dict pointer from
global 0x831C2EF8 is null (because GPU init is stubbed), 15+ callers would SIGBUS.
The hook returns 0 (not found) when dict is null.

---

## 2. VBlank-FIX Hook Analysis (sub_82A487B8)

**File**: imports.cpp lines 587-632

### What It Tells Us

The VBlank callback (sub_82A487B8) reveals the primary symptom of GPU device absence:

1. The game passes a GPU device object as `userData` to `VdSetGraphicsInterruptCallback`
2. The VBlank interrupt handler reads `device[+10900]` (offset 0x2A94) to find a
   "frame_done callback descriptor"
3. It then dereferences `*(device[+10900]+16)` for the actual callback function pointer
4. **When GPU init fails** (XMemAlloc returns 0), device[+10900] stays NULL
5. Without the fix, this null dereference crashes on every VBlank (60 times/sec)

### The Fix Pattern

The hook allocates a 64-byte zero-filled guest stub and writes its address into
device[+10900]. The recomp code then reads `*(stub+16) = 0` (no callback), which
it handles gracefully by skipping the callback call. The spinlock-clear at
loc_82A48810 still executes, unblocking the main game thread.

### What This Reveals About GPU Device State

The GPU device object is large (10900+ bytes). When GPU init fails:
- device[+10896] (0x2A90) = NULL (GPU ring buffer control struct)
- device[+10900] (0x2A94) = NULL (frame-done callback descriptor)
- device[+11000] stays uninitialized (sub_82A49C38 clears it)
- device[+16544] = frame counter (maintained by sub_82A467D8 Present hook)
- device[+16552] = GPU completion counter (maintained by sub_82A467D8)
- Global 0x831C2EF8 = NULL (resource dictionary never populated)
- Global 0x83124AF4 = contains stale GPU command buffer pointers (0x70xxxxxx range)

The VBlank fix is a **reactive patch** -- it handles one specific crash caused by
device[+10900] being NULL. The broader pattern is that MANY device fields are NULL
or uninitialized, causing crashes throughout the rendering pipeline.

---

## 3. sub_82A10EB0 (RAGE GPU Heap Allocator) Cascade Analysis

**File**: imports.cpp lines 1063-1104

### The Chain

```
sub_821B3608 (RAGE alloc dispatch)
  flags & 0x80000000 (bit-31 set) ?
    YES -> sub_82A10EB0 (GPU/physical alloc)
           -> sub_82A18920 (XMemAlloc wrapper)
              -> XMemAlloc (kernel stub -> returns 0)
                 = allocation FAILS
```

### What Happens When sub_82A10EB0 Returns 0

Without the hook, the cascade is:
1. Device internal struct allocations fail (device[+10896], device[+10900] stay NULL)
2. VBlank callback crashes on null deref (fixed by sub_82A487B8 hook)
3. Frame-done callback never fires -> main thread deadlocks on spinlock
4. GPU resource dictionary (0x831C2EF8) never populated -> sub_827A9A20 SIGBUS
5. Shader registration (sub_8285B088) tries to flush to non-existent GPU -> hang

### The Fix

The hook intercepts sub_82A10EB0 and routes through `SystemHeapAlloc`. This provides
real guest memory for "GPU" allocations. However, the allocated memory is not backed
by actual GPU hardware, so:
- Structures get allocated (device fields become non-null)
- Internal pointers exist but point to zeroed memory
- Vtable pointers in allocated objects are 0x00000000
- Code that dereferences these vtables hits MISSING-FUNC

---

## 4. RAGE Rendering Objects Using GPU/D3D Device

### Primary Object Hierarchies

Based on the code and existing research docs:

**grcDevice** (`api/RAGE/RAGE.h` line 202):
- sm_Instance = singleton pointer (NULL in recomp -- host rendering bypasses this)
- m_d3dDevice = would point to ID3D11Device on PC / Xenos on 360
- Virtual methods: BeginFrame, EndFrame, Present

**grcEffect / grcTexture / grcResource**:
- Allocated by game code, but vtable pointers stay 0x00000000
- sub_828C9980 (SetVariable) reads vtable[+52] on effect variable objects -> NULL -> MISSING-FUNC
- 2.3 million MISSING-FUNC calls per run from this single path (doc 51)

**Entity objects** (sub_8291DF00):
- Iterates game entities calling vtable[+16], vtable[+40], vtable[+24]
- 192 null vtable calls (96 entity pairs) during world setup

**Render state objects** (sub_828C19C0 -> sub_828C8588 -> sub_828E02E8):
- 37 render states written to global table at 0x83169C00
- sub_828E02E8 (state flush handler) dispatches through null GPU device vtable

### Functions That Try to Use These Objects

| Caller | # Calls | Object Type | Vtable Offset | Purpose |
|--------|---------|-------------|---------------|---------|
| 0x828C99CC (sub_828C9980) | ~2.3M | grcResource | +52 | Release/RefCount |
| 0x828C97E0 | Many | entity/drawable | +24,+28,+40,+44 | Property accessors |
| 0x828C1F94 (sub_828C19C0) | 37 | GPU device | state flush | Render state write |
| 0x8291E144 (sub_8291DF00) | ~1.18M | entity | +40 | GetBounds |
| 0x8291E1B0 (sub_8291DF00) | ~1.18M | entity | +24 | GetType/Flags |
| 0x821911C4 (sub_821910D0) | 42 | audio endpoint | +17 (vtable[68]) | Render callback |

---

## 5. sub_8291DF00 (Entity Update / Scene Graph Traversal)

### Does It Stack Overflow?

**No.** Per the research in doc 51 (sub_828C9980_render_dispatch.md):

sub_8291DF00 iterates over entity objects and calls virtual methods. Each call is
a flat call-return pattern:

```
entity_loop:
  load entity from array
  call vtable[+16]  -> MISSING-FUNC (returns immediately)
  call vtable[+40]  -> MISSING-FUNC (returns immediately)
  call vtable[+24]  -> MISSING-FUNC (returns immediately)
  next entity
```

The MISSING-FUNC handler (`PPC_CALL_INDIRECT_FUNC` in `rex/ppc/context.h`) does NOT
make recursive calls. It prints a warning via fprintf and returns. Each entity
iteration pushes/pops its own stack frame independently.

The 2.37 million MISSING-FUNC calls from 0x8291E144/0x8291E1B0 are sequential
iterations across frames, not recursive depth. Stack depth never exceeds ~5-6 frames.

**However**: sub_8291DF00 is a symptom indicator. The volume of null vtable calls
(1.18M per call site) shows that entity objects allocated during world loading have
completely uninitialized vtables -- a direct consequence of GPU device absence
preventing proper construction.

---

## 6. Render/GPU Init Functions Called During World Loading

### The InitCoreEngine Chain (sub_8218C600)

From `game_init.cpp` lines 70-97, the original sub_8218C600 calls these GPU init
functions in order:

| # | Function | Address | Purpose |
|---|----------|---------|---------|
| 5 | sub_82850AF0 | GPU availability check | Queries hardware presence |
| 6 | sub_82850B60 | GPU init mode | Sets rendering pipeline mode |
| 7 | sub_8218BE28(472) | Allocate render context | 472-byte struct (malloc equivalent) |
| 8 | sub_82857028 | Render buffer init | Creates GPU command buffers |
| 9 | sub_82851548 | GPU mode setup | Configures rendering mode |
| 10 | sub_82856C38 | GPU context setup | Initializes D3D context object |
| 11 | sub_8285A0E0 | GPU resource setup | Creates resource management tables |
| 12 | sub_82850748 | GPU buffer creation | Allocates vertex/index buffers |
| 13 | sub_82856C90 | GPU state init | Initializes render state tables |

All of these run as original PPC code inside `__imp__sub_8218C600`. The game_init.cpp
`Initialize()` function calls `__imp__sub_8218C600` directly, letting these GPU
init functions execute. Since XMemAlloc returns 0, the GPU allocations inside these
functions fail silently, leaving device fields at 0.

### The Renderer Subsystem Init Chain (sub_82299500)

Documented in imports.cpp line 1193-1196:

```
sub_82299500 ("renderer subsystem init")
  -> sub_82902AF8
     -> sub_829029A0
        -> sub_82920060
           -> sub_8291DC80
              -> sub_82852FB0 (ShaderFinalise) -- stubbed via sub_8285A8B0
              -> sub_82852B78 (ShaderBind) -- pass-through, safe
```

This chain is responsible for setting up the shader pipeline. The inner functions
dispatch through GPU device vtable calls that would hang without the sub_8285A8B0
stub (GPU buffer flush = no-op).

### GPU Device Create Chain

```
sub_82A416B8 (D3D device setup)
  -> sub_82A50890 (GPU CreateDevice)
     -> sub_82A49D08 (ring buffer / device init)
        -> sub_82A10EB0 (RAGE GPU heap allocator) -- hooked to use SystemHeapAlloc
           -> allocates device internal structures
              -> device[+10896], device[+10900] populated (or 0 if unhook)
```

Both sub_82A416B8 and sub_82A50890 have diagnostic hooks that log entry/exit but
don't modify behavior. sub_82A49D08 also has a diagnostic hook logging device field
values after init.

---

## 7. Patterns for Short-Circuiting RAGE GPU Init

The codebase uses several distinct patterns to handle GPU absence:

### Pattern 1: Complete Stub (return constant)

Used for functions that are purely GPU hardware operations:
```cpp
PPC_FUNC_HOOK(sub_82871180) { ctx.r3.u32 = 0; }  // GPU render state submission
PPC_FUNC_HOOK(sub_82A46098) { return; }             // Frame swap
PPC_FUNC_HOOK(sub_8285D018) { ctx.r3.u32 = 0; }    // Ring buffer submit+wait
PPC_FUNC_HOOK(sub_8285C648) { ctx.r3.u32 = 1; }    // GPU fence wait
```

### Pattern 2: Pre-patch + Pass-through

Used when the function has mixed GPU and non-GPU logic:
```cpp
PPC_FUNC_HOOK(sub_82A487B8) {
    // Fix null device field BEFORE running original code
    if (slot_val == 0) { allocate_stub(); patch_device_field(); }
    __imp__sub_82A487B8(ctx, base);  // run original
}
```

### Pattern 3: Redirect Allocator

Used when GPU-path allocations fail:
```cpp
PPC_FUNC_HOOK(sub_82A10EB0) {
    // Instead of XMemAlloc (returns 0), use SystemHeapAlloc
    uint32_t guest = mem->SystemHeapAlloc(size);
    ctx.r3.u32 = guest;
}
```

### Pattern 4: Null Guard on Callers

Used when many callers share a null pointer:
```cpp
PPC_FUNC_HOOK(sub_827A9A20) {
    if (ctx.r3.u32 == 0) { ctx.r3.u32 = 0; return; }  // null dict
    __imp__sub_827A9A20(ctx, base);
}
```

### Pattern 5: Host-Native Replacement

Used for functions where LibertyRecomp provides its own implementation:
```cpp
PPC_FUNC_HOOK(sub_82A467D8) {
    // Increment frame counter (original PPC logic)
    // Then call Video::Present() (host-side Vulkan/Metal)
    Video::Present();
}
```

### Pattern 6: Force Synchronous Mode

Used when async GPU workers die without the hardware:
```cpp
PPC_FUNC_HOOK(sub_827DF248) {
    PPC_STORE_U32(0x830F589C, 1);  // sync mode flag
    __imp__sub_827DF248(ctx, base);
}
```

---

## 8. Headless Mode / No-Render Flag

### RexGlue's `headless` CVar

RexGlue declares a `headless` boolean CVar (`include/rex/system/flags.h` line 16):

```cpp
REXCVAR_DECLARE(bool, headless);
```

Defined in `src/kernel/xam/xam_ui.cpp` line 21 (default: false):
```cpp
REXCVAR_DEFINE_BOOL(headless, false, "Kernel", "Don't show XAM UI");
```

It is used in:
- `xam_ui.cpp`: Skips XAM dialog rendering (XamShowMessageBoxUI, etc.)
- `xam_nui.cpp`: Skips NUI (Natural User Interface) processing

**This is NOT a general "no-render" flag.** It only affects XAM overlay UI, not the
game's own RAGE rendering pipeline. Setting `headless=true` would prevent XAM dialogs
from displaying but would not affect the game's D3D/GPU init paths.

### LibertyRecomp's Kernel Phase System

imports.cpp lines 60-86 define a three-phase system:

```cpp
enum class KernelPhase { Boot, Init, Runtime };
```

- **Boot**: Initial state, open waits fail-fast
- **Init**: After kernel setup
- **Runtime**: After first Video::Present() -- GPU is "active"

The comment at line 83 says: `"Disabled headless wait cap (GPU active)"` -- this
was related to a removed SDK feature. The phase system gates whether NtWaitForSingleObject
calls should fail-open (during boot/init) or block normally (during runtime).

### No RAGE-Level Headless Flag

There is **no known "headless mode" or "no-render" flag within RAGE itself** that
could cleanly skip GPU initialization. The RAGE engine assumes a D3D device always
exists. The approach taken by LibertyRecomp is correct: stub the GPU hardware layer
and provide host-native rendering.

---

## 9. Root Cause Analysis: What Goes Deep When GPU Is Absent

### The Problematic Init Cascade

During world loading (after scene creation state machine completes), the following
systems activate and produce GPU-dependent calls:

1. **Shader system** (sub_828C9980 path): Every material on every drawable calls
   SetVariable to bind shader parameters. With null vtables, each call hits
   MISSING-FUNC. This is 2.3M calls but does NOT stack overflow -- each is flat.

2. **Entity system** (sub_8291DF00 path): World entities initialized during streaming
   have null vtables. Update/bounds/type virtual calls fail. Again flat, not deep.

3. **Render state init** (sub_828C19C0): One-time 37-state initialization hits null
   GPU device vtable for state flush.

4. **GPU command buffer** (sub_8285D018 chain): Without the stub, this chain goes:
   sub_8285D018 -> sub_8285CF98 -> sub_8285CEA8 -> sub_8285C648 -> sub_828497D8
   -> sub_82A13040 -> NtWaitForSingleObjectEx -> **BLOCKS FOREVER** (no GPU to
   complete the fence). This is the most dangerous path -- infinite blocking.

5. **Streaming ring buffer** (sub_8284CFD8): Workers die immediately without proper
   semaphore handles, causing the main thread's completion event to never fire.
   Fixed by semaphore seeding.

### Paths That Could Still Cause Deep Recursion

The existing hooks handle the known paths. Potential remaining risks:

- **Resource dictionary lookups** (0x831C2EF8 == NULL): sub_827A9A20 is guarded, but
  any NEW code path that reads this global without going through sub_827A9A20 would crash.

- **Vtable dispatch chains**: If a null-vtable call somehow resolves to a valid function
  that calls back into the same chain, recursion could occur. The MISSING-FUNC handler
  prevents this by returning immediately for unknown addresses.

- **RAGE allocator TLS corruption** (documented in imports.cpp lines 766-778): When GPU
  init stubs skip the push/pop allocator-swap protocol, TLS[1676] gets zeroed. The
  protective push/pop hooks and fallback allocator address this, but any new GPU init
  stub could reintroduce the problem.

---

## 10. Summary: Current GPU Stub Coverage

### Well-Covered Areas
- VBlank timer and interrupt dispatch (full implementation)
- GPU fence/wait chain (all 4 functions stubbed)
- GPU memory allocation (redirected to SystemHeapAlloc)
- Shader compilation/binding (sub_8285A8B0 flush stubbed)
- Frame presentation (host-native via Video::Present)
- GPU MMIO register reads (minimal emulation)
- Streaming worker semaphores (seeded via XSemaphore)
- Resource dictionary null guard (sub_827A9A20)
- RAGE allocator TLS protection (push/pop guards + fallback)

### Known Remaining Symptoms
- 2.3M MISSING-FUNC calls from null vtable objects (grcResource, entities)
- Resource dictionary (0x831C2EF8) stays NULL, all lookups return "not found"
- GPU render state flush (sub_828E02E8) dispatches to null
- Audio endpoint vtable corruption (0x000F4000 calls, 42 per run)

### Architectural Observation

LibertyRecomp correctly does NOT try to emulate the Xbox 360 GPU. Instead, it:
1. Stubs the hardware interface layer (Vd*, ring buffer, fences)
2. Redirects GPU memory to host heap
3. Provides host-native rendering via video.cpp hooks
4. Guards null pointers at high-traffic crash sites

The remaining 2.3M+ MISSING-FUNC calls are a consequence of RAGE engine objects
being allocated but never properly constructed (no D3D device to set vtables).
These are performance noise, not functional bugs -- the game continues running.
The real rendering happens entirely through the host GPU via video.cpp's
GUEST_FUNCTION_HOOK system, which intercepts D3D calls at the API boundary and
routes them to Vulkan/Metal.
