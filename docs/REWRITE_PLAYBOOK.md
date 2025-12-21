# LibertyRecomp Rewrite Playbook

## Purpose
This single reference replaces the previous module inventory and handoff documents. It maps every rewrite-critical module, explains the Xbox 360 assumptions, and presents a prioritized rewrite path for modern host implementations.

**Target Audience:** New developers joining the project to perform targeted rewrites of Xbox 360-specific modules that cannot simply be translated—they must be rewritten from scratch for modern systems.

### Key Details
- **Total estimated effort:** 12–15 days for the core stack (timing, boot, GPU)
- **Priority order:** Timing/Sync > Boot/XAM Tasks > GPU Rendering > Runtime services (threads, async I/O)
- **Dependencies:** Timing foundations enable event/fence handling; GPU depends on timing+shader cache; runtime behavior references the same synchronization primitives.

### Critical Problem Summary
The game is **time-coupled like PS1/N64-era software**. It expects:
- VBlank at exactly 60Hz (16.67ms)
- Blocking waits that yield to the scheduler
- Semaphores and events signaled in precise order
- Audio buffers consumed at fixed cadence

### Magic Return Values Protocol
| Value | Meaning | Effect |
|-------|---------|--------|
| `996` | No progress | Callers exit early without advancing state |
| `997` | Pending | Callers store pending state and return |
| `258` | Mapped to 996 | Intermediate value in `sub_829A1A50` |
| `259` | Explicit wait | Triggers `__imp__NtWaitForSingleObjectEx` call |
| `257` | Retry trigger | Used in `sub_829A9738` to retry wait |

---

## 1. Module Inventory & Priority

| Module | Scope | Complexity | Effort | Status | Blocking |
|--------|-------|------------|--------|--------|----------|
| Timing & Synchronization | Full | Critical | 2–3 days | P0 | Yes - deadlocks |
| XAM Task System | Full | Critical | 1–2 days | P0 | Yes - init fails |
| GPU Rendering | Full | High | 5–7 days | P1 | Yes - no frames |
| Boot State Machine | Partial | High | 2–3 days | P1 | Yes - hangs |
| Shader System | Full | Medium | 3–4 days | P2 | Yes - no draws |
| File System/Async I/O | Partial | Medium | 2–3 days | P2 | Partial |
| Worker Threads | Partial | High | 2–3 days | P1 | Yes - semaphores |
| Audio System | Full | Medium | 3–4 days | P3 | No |
| Input System | Partial | Low | 1 day | P3 | No |

**Main dependencies:** timing → boot/xam → GPU, with runtime behaviors underpinning everything.

### Dependency Graph
```
                    ┌─────────────────┐
                    │ Timing & Sync   │ (P0)
                    │ VBlank/Events   │
                    └────────┬────────┘
                             │
              ┌──────────────┼──────────────┐
              ▼              ▼              ▼
    ┌─────────────────┐ ┌─────────┐ ┌──────────────┐
    │ XAM Task System │ │ Workers │ │ Async I/O    │
    │ (P0)            │ │ (P1)    │ │ XXOVERLAPPED │
    └────────┬────────┘ └────┬────┘ └──────┬───────┘
             │               │             │
             └───────────────┼─────────────┘
                             ▼
                    ┌─────────────────┐
                    │ Boot State      │ (P1)
                    │ Machine         │
                    └────────┬────────┘
                             │
              ┌──────────────┼──────────────┐
              ▼              ▼              ▼
    ┌─────────────────┐ ┌─────────┐ ┌──────────────┐
    │ GPU Rendering   │ │ Shaders │ │ Streaming    │
    │ (P1)            │ │ (P2)    │ │ (P2)         │
    └─────────────────┘ └─────────┘ └──────────────┘
```

---

## 2. Module: Timing & Synchronization

**Complexity**: Critical  
**Estimated Effort**: 2–3 days  
**Dependencies**: None (foundation module)

### Original Xbox 360 Behavior
The Xbox 360 provides hardware-timed VBlank interrupts at exactly 60Hz (16.67ms). The game registers a callback via `VdRegisterInterruptCallback` and expects:
- Precise 60Hz VBlank signals
- `KeWaitForSingleObject` blocking until events are signaled
- Semaphores released in specific order by hardware/kernel
- GPU fences completing within frame budget

### Why Rewrite is Required
- No hardware VBlank on PC - must be software emulated
- `KeWaitForSingleObject` can deadlock if events are never signaled
- GPU fence waits (`sub_829DDC90`) spin forever without host intervention
- Busy-wait loops (`sub_82169400`, `sub_821694C8`) poll global flags that are never set

### Functions to Rewrite

| Address | Name | Role | Current Status |
|---------|------|------|----------------|
| `0x829DDC90` | GPU fence wait | Waits on fence at `r28+32`, loops if returns 258 | Hooked - force success after timeout |
| `0x82169400` | Global flag wait | Waits on event + checks `*(global+300)` | Hooked - force immediate exit |
| `0x821694C8` | Global flag wait variant | Similar to above, loops while `*(global+300) != 0` | Hooked - force immediate exit |
| `0x829D7368` | VBlank registration | Registers interrupt callback | Hooked - store callback address |
| `0x829D4C48` | Frame timing | Frame boundary handling | Needs implementation |
| `0x82673718` | Audio/streaming init | Busy-wait on job counters | Needs bypass |

### Expected Inputs / Outputs

**`KeWaitForSingleObject`**:
- Input: XDISPATCHER_HEADER*, WaitReason, WaitMode, Alertable, Timeout
- Output: STATUS_SUCCESS (0), STATUS_TIMEOUT (0x102), STATUS_WAIT_0 (0)
- Special: Returns 258 for GPU fence retry

**`VdRegisterInterruptCallback`**:
- Input: Callback function address, user data pointer
- Output: Stores callback for VBlank firing
- Must fire callback at 60Hz with interrupt type 0 (VBlank)

### Test Cases
1. **VBlank Frequency**: Log VBlank callbacks - expect 60±2 per second
2. **Event Signaling**: Track `KeSetEvent` calls - expect 256+ events/frame
3. **No Deadlocks**: Boot completes within 30 seconds
4. **Frame Timing**: Frame times 14-20ms range (16.67ms target)

### Implementation Notes
```cpp
// Current implementation in imports.cpp fires VBlank from NtWaitForSingleObjectEx
// every 3 calls to ensure steady heartbeat through blocking waits
if (vblankCallback != 0 && s_callsSinceVBlank >= 3) {
    PPCContext tempCtx = *g_ppcContext;
    tempCtx.r3.u32 = 0;  // Interrupt Type 0 = VBlank
    tempCtx.r4.u32 = g_gpuRingBuffer.interruptUserData;
    func(tempCtx, g_memory.base);
}
```

**Key insight**: Force GPU flags (`enginesInitialized`, `edramTrainingComplete`, `interruptSeen`) to true after initial waits to prevent infinite polling.

---

## 3. Module: XAM Task System

**Complexity**: Critical  
**Estimated Effort**: 1–2 days  
**Dependencies**: Timing & Synchronization

### Original Xbox 360 Behavior
XAM (Xbox Application Manager) provides async task scheduling:
- `XamTaskSchedule`: Creates and schedules a task to run on a worker thread
- `XamTaskShouldExit`: Returns 0 while task should continue, 1 when task should exit
- `XamTaskCloseHandle`: Blocks until task completes, then closes handle

Tasks run asynchronously and signal completion events when done.

### Why Rewrite is Required
- Original stub: `XamTaskSchedule` was no-op, `XamTaskShouldExit` returned 1 immediately
- This caused tasks to exit before completing initialization
- Boot sequence deadlocked because async init tasks never finished
- Semaphores never signaled because task workers exited early

### Functions to Rewrite

| Address | Name | Role | Current Status |
|---------|------|------|----------------|
| `XamTaskSchedule` | Task creation | Create + schedule async task | Implemented - executes synchronously |
| `XamTaskShouldExit` | Exit check | Return 0 while running | Implemented - returns 0 |
| `XamTaskCloseHandle` | Cleanup | Block until complete | Stub - returns success |
| `0x829A3318` | Boot orchestrator | Loops on `XamTaskShouldExit` | Hooked - logs execution |

### Expected Inputs / Outputs

**`XamTaskSchedule`**:
- Input: funcAddr, context, processId, stackSize, priority, flags, phTask
- Output: ERROR_SUCCESS (0), task handle in phTask
- Must execute the task function with context in r3

**`XamTaskShouldExit`**:
- Input: taskHandle
- Output: 0 (keep running) or 1 (should exit)
- Current fix: Always return 0 so init tasks complete

**`XamTaskCloseHandle`**:
- Input: taskHandle
- Output: ERROR_SUCCESS after task completes

### Test Cases
1. **Task Execution**: Log `XamTaskSchedule` - expect 5-10 tasks scheduled during boot
2. **Task Completion**: Tasks should execute their function and return
3. **Boot Progress**: `sub_82120000` returns success (1) after tasks complete
4. **No Early Exit**: `XamTaskShouldExit` logs show return value 0

### Implementation Notes
```cpp
// Current implementation executes task synchronously
auto func = g_memory.FindFunction(funcAddr);
if (func) {
    PPCContext tempCtx = *g_ppcContext;
    tempCtx.r3.u32 = context;
    func(tempCtx, g_memory.base);
}
```

**Key insight**: The game expects tasks to run asynchronously, but synchronous execution works because it ensures completion before the caller continues.

---

## 4. Module: GPU Rendering

**Complexity**: High  
**Estimated Effort**: 5–7 days  
**Dependencies**: Timing, XAM Tasks, Shader System

### Original Xbox 360 Behavior
Xbox 360 uses Xenos GPU with:
- 10MB EDRAM for render targets
- PM4 command buffer format
- Ring-buffer submission model
- Hardware tile resolves

D3D-like wrapper functions build PM4 packets and submit to ring buffer.

### Why Rewrite is Required
- PM4 command buffers are non-portable (Xenos-specific)
- Must use pre-compiled shaders from XenosRecomp
- Shader table must be populated for draw calls to work
- Current stub returns success without creating shaders

### Functions to Rewrite

| Address | Name | Role | Current Status |
|---------|------|------|----------------|
| `0x829D87E8` | CreateDevice | D3D device creation | Hooked |
| `0x829D5388` | Present/VdSwap | Frame presentation | Hooked - calls Video::Present |
| `0x829D8860` | DrawPrimitive | Issue draw calls | Hooked - needs native draws |
| `0x829D4EE0` | UnifiedDraw | Indexed/non-indexed draws | Hooked |
| `0x829D3400` | CreateTexture | Texture creation | Stubbed |
| `0x829D3520` | CreateVertexBuffer | VB creation | Stubbed |
| `0x829DFAD8` | GpuMemAlloc | GPU memory allocation | Stubbed - returns fake offsets |
| `0x829CD350` | SetVertexShader | Bind VS | Hook needed |
| `0x829D6690` | SetPixelShader | Bind PS | Hook needed |
| `0x829C9070` | SetVertexDeclaration | Vertex format | Hook needed |
| `0x829C96D0` | SetIndexBuffer | Index buffer | Hook needed |
| `0x829D3728` | SetTexture | Texture binding | Hook needed |

### Expected Inputs / Outputs

**DrawPrimitive (0x829D8860)**:
- Input: Device context (14KB struct), primitive type, start vertex, count
- Output: Draw call issued to host GPU
- Device context offsets: +10932 VS, +10936 PS, +12020 stream sources

**Present (0x829D5388)**:
- Input: Device context
- Output: Frame swapped, VBlank signaled
- Calls `VdSwap` kernel function

### Test Cases
1. **Draw Call Count**: Log draws - expect ~961 per frame
2. **Shader Binding**: VS/PS handles non-null before draws
3. **Frame Rate**: Stable 60 FPS (16.67ms frames)
4. **Visual Output**: Any visible rendering indicates success

### Implementation Notes
**Strategy**: Hook layer 1 D3D wrappers, bypass PM4 builders entirely.

```cpp
// PM4 builders to stub/bypass (return early, no processing):
// sub_829D7E58 - PM4 packet builder
// sub_829D8568 - PM4 draw packet
// sub_829D7740 - PM4 state packet
```

**Device Context Structure** (14KB at TLS+1676):
- +48: Command buffer pointer
- +10456: Vertex declaration
- +10932: Vertex shader
- +10936: Pixel shader
- +12020-12032: Stream sources (vertex buffers)
- +13580: Index buffer
- +19480: Frame buffer index

---

## 5. Module: Boot State Machine

**Complexity**: High  
**Estimated Effort**: 2–3 days  
**Dependencies**: Timing, XAM Tasks

### Original Xbox 360 Behavior
Boot sequence uses cooperative polling state machines:
- `sub_82120000`: One-time initialization
- `sub_8218C600`: Creates 9 worker threads, initializes subsystems
- `sub_82673718`: Audio/streaming init with job polling
- `sub_829748D0`: Worker initialization
- `sub_8298E810`: Resource initialization
- `sub_829A3318`: Task orchestration with `XamTaskShouldExit` loop

### Why Rewrite is Required
State machines deadlock when signals never arrive because:
- XAM tasks exit immediately (fixed)
- Semaphores never signaled
- Global flags never set
- Async completions never fire

### Functions to Rewrite

| Address | Name | Role | Current Status |
|---------|------|------|----------------|
| `0x82120000` | One-time init | Main initialization | Hooked - cached result |
| `0x8218C600` | Worker setup | Creates 9 threads | Running |
| `0x82673718` | Audio init | Polls job counters | Needs bypass |
| `0x829748D0` | Worker init | Worker thread setup | Running |
| `0x8298E810` | Resource init | Resource loading | Partial |
| `0x829A3318` | Boot orchestrator | XamTask loop | Hooked |
| `0x8218BEA8` | Game entry | Trampoline to frame tick | Hooked |
| `0x827D89B8` | Frame tick | Per-frame processing | Running |

### Expected Inputs / Outputs

**`sub_82120000`** (One-time init):
- Input: None
- Output: 0 = not ready, 1 = ready
- Must return 1 after initialization completes

**`sub_8218BEB0`** (Core update):
- Calls `sub_82120000`, if returns 0, returns -1 (failure)
- If returns non-zero, calls per-frame updates

### Test Cases
1. **Init Completion**: `sub_82120000` returns 1 after boot
2. **Worker Creation**: 9 threads created via `ExCreateThread`
3. **Input Polling**: `XamInputGetKeystrokeEx` called = interactive state
4. **No Deadlock**: Boot completes without infinite loops

### Implementation Notes

**Boot Call Graph**:
```
_xstart
  └── sub_8218BEA8 (game entry - ONE CALL only)
        └── sub_827D89B8 (frame tick)
              ├── sub_827D8840 (input processing)
              ├── sub_827FFF80 (timing)
              ├── sub_8218BEB0 (core update)
              │     └── sub_82120000 (init check)
              └── Frame presentation
```

**Key insight**: `sub_8218BEA8` does NOT loop - Xbox 360 runtime called it repeatedly. Recompilation must provide external loop.

---

## 6. Module: Shader System

**Complexity**: Medium  
**Estimated Effort**: 3–4 days  
**Dependencies**: GPU Rendering

### Original Xbox 360 Behavior
- Shaders stored in `.fxc` files (RAGE FXC format)
- Xbox 360 Xenos microcode embedded in containers
- `EffectManager::Load` (`0x8285E048`) loads and parses FXC files
- `sub_82858758` creates shader objects and stores in table at ~0x830E5900

### Why Rewrite is Required
- Xbox 360 shader microcode is not portable
- Must use pre-compiled shaders from XenosRecomp
- Shader table must be populated for draw calls to work
- Current stub returns success without creating shaders

### Functions to Rewrite

| Address | Name | Role | Current Status |
|---------|------|------|----------------|
| `0x8285E048` | EffectManager::Load | Entry point for shader loading | Stubbed |
| `0x82858758` | ShaderLoader | Creates shader objects | Not hooked |
| `0x829CD350` | SetVertexShader | Binds VS from table | Not hooked |
| `0x829D6690` | SetPixelShader | Binds PS from table | Not hooked |

### Expected Inputs / Outputs

**EffectManager::Load (0x8285E048)**:
- Input: r3 = effect context, r4 = output pointer
- Output: Populate output with shader handles
- Must call `sub_82858758` for each shader in effect

**Shader Table** (0x830E5900):
- 128 slots, 4 bytes each (shader object pointer)
- Each shader object is 112 bytes
- Offset +104: State flag (-1 = loading, other = ready)

### Test Cases
1. **Shader Loading**: Log `EffectManager::Load` calls
2. **Table Population**: Check 0x830E5900 has non-null entries
3. **Binding Calls**: `SetVertexShader`/`SetPixelShader` receive valid pointers
4. **Draw Execution**: Draws issue after shaders bound

### Implementation Notes

**Pre-compiled Shader Cache**:
```cpp
// Embedded in LibertyRecompLib/shader/shader_cache.cpp
g_shaderCacheEntries[1132]     // Shader metadata (hash, offsets, sizes)
g_compressedDxilCache          // DXIL bytecode (Windows)
g_compressedSpirvCache         // SPIR-V bytecode (Linux)
g_compressedAirCache           // AIR bytecode (macOS)
```

**Lookup by hash**:
```cpp
auto findResult = FindShaderCacheEntry(hash);
if (findResult == nullptr) {
    std::_Exit(1);  // Fatal - shader must exist
}
```

---

## 7. Module: File System / Async I/O

**Complexity**: Medium  
**Estimated Effort**: 2–3 days  
**Dependencies**: Timing

### Original Xbox 360 Behavior
- NT-level APIs: `NtCreateFile`, `NtReadFile`, `NtWriteFile`
- Async I/O via `XXOVERLAPPED` structure
- Completion polling and event signaling
- RPF archive mounting via `game:\` paths

### Why Rewrite is Required
- Async completion events must be signaled
- `XXOVERLAPPED.Result` must be updated (not left at 0xFFFFFFFF)
- Big-endian byte swapping required for all fields
- RPF archives extracted, not directly read

### Functions to Rewrite

| Address | Name | Role | Current Status |
|---------|------|------|----------------|
| `NtCreateFile` | File open | Create/open files | Implemented |
| `NtReadFile` | File read | Read with optional async | Implemented |
| `NtWriteFile` | File write | Write with optional async | Implemented |
| `NtClose` | Handle close | Close file handles | Implemented |
| `0x829AAD20` | Async read wrapper | Calls NtReadFile, waits if 259 | Guest code |
| `0x829AADB8` | Async write wrapper | Calls NtWriteFile, waits if 259 | Guest code |

### Expected Inputs / Outputs

**XXOVERLAPPED Structure**:
```cpp
struct XXOVERLAPPED {
    uint32_t Result;         // +0x00 (0xFFFFFFFF while pending)
    uint32_t Length;         // +0x04
    uint32_t Context;        // +0x08
    uint32_t EventHandle;    // +0x0C
    uint32_t CompletionRoutine; // +0x10
    uint32_t ExtendedError;  // +0x14
};
```

### Test Cases
1. **Sync Read**: Result == 0 immediately for non-async
2. **Async Read**: Result transitions from 0xFFFFFFFF to 0
3. **Event Signaling**: EventHandle signaled on completion
4. **Path Resolution**: `game:\` → extracted game files

### Implementation Notes
```cpp
// Always update XXOVERLAPPED on completion
if (pOverlapped) {
    pOverlapped->Result = STATUS_SUCCESS;
    pOverlapped->Length = bytesRead;
    if (pOverlapped->EventHandle)
        KeSetEvent(pOverlapped->EventHandle, 0, FALSE);
}
```

---

## 8. Module: Worker Threads

**Complexity**: High  
**Estimated Effort**: 2–3 days  
**Dependencies**: Timing, XAM Tasks

### Original Xbox 360 Behavior
- 9 worker threads created via `ExCreateThread`
- 2 render workers wait on semaphores 0xA82487F0/0xA82487B0
- 3 streaming workers handle request queue
- 3 resource workers manage asset loading
- 2 GPU threads handle command processing

### Why Rewrite is Required
- Render workers blocked because semaphores never signaled
- Main thread blocked in init, can't signal workers
- Circular deadlock: workers wait for main, main waits for workers

### Functions to Rewrite

| Address | Name | Role | Current Status |
|---------|------|------|----------------|
| `0x827DE858` | Render worker entry | Waits on work semaphore | Blocked |
| `0x827DACD8` | Semaphore wait | Waits via KeWaitForSingleObject | Implemented |
| `0x827DAD60` | Semaphore signal | Signals via KeReleaseSemaphore | Implemented |
| `0x82193B80` | Streaming worker | Handles streaming requests | Running |

### Semaphore Map

| Address | Purpose | Waiters | Status |
|---------|---------|---------|--------|
| 0xA82487F0 | Render Worker #1 | 1 | Blocked - never signaled |
| 0xA82487B0 | Render Worker #2 | 1 | Blocked - never signaled |
| 0xA82402B0 | Streaming queue | Multiple | Active |
| 0xEB2D00B0 | Resource queue | Multiple | Active |

### Test Cases
1. **Thread Creation**: 9 threads created during boot
2. **Semaphore Signaling**: Log `KeReleaseSemaphore` calls
3. **Worker Activity**: Streaming workers processing requests
4. **Render Unblock**: Render workers eventually get signaled

### Implementation Notes

**Deadlock Cycle**:
```
Main thread (sub_8218C600) ───┬──► Creates workers
                             │
                             └──► Waits for XAM tasks (blocks)
Render workers ───────┬────► Wait on semaphores (0xA82487F0/B0)
                      │
                      └────► Never run (semaphores at 0)
```

**Current workaround**: Force-succeed worker semaphore waits in `NtWaitForSingleObjectEx` to allow workers to proceed.

---

## 9. Implementation Roadmap

### Phase 1: Timing Foundation (P0) - Days 1-3
1. Implement 60Hz VBlank timer thread
2. Fix `KeWaitForSingleObject` to properly handle events/semaphores
3. Force GPU flags after initial waits
4. Verify VBlank callbacks fire consistently

### Phase 2: XAM Task / Boot (P0/P1) - Days 3-5
1. Ensure `XamTaskSchedule` executes task functions
2. Keep `XamTaskShouldExit` returning 0
3. Signal render worker semaphores after init
4. Cache `sub_82120000` result to prevent re-init

### Phase 3: GPU Renderer (P1) - Days 5-10
1. Hook D3D wrapper functions (DrawPrimitive, SetShader, etc.)
2. Track dirty state from device context
3. Connect to pre-compiled shader cache
4. Issue native draw calls
5. Implement Present/VdSwap frame flip

### Phase 4: Runtime Services (P2/P3) - Days 10-15
1. Complete XXOVERLAPPED async completion
2. Verify file system paths resolve correctly
3. Implement audio system hooks (if needed)
4. Polish input handling

---

## 10. Testing Strategy

### Boot Validation
```bash
# Expected log sequence for successful boot:
[Boot] _xstart entry
[Boot] sub_8218BEA8 called
[XamTaskSchedule] #1 func=0x829A33XX
[XamTaskSchedule] Task completed
[Boot] sub_82120000 returned 1
[Input] XamInputGetKeystrokeEx called  # Interactive state reached
```

### Frame Validation
```bash
# Expected per-frame:
[VBlank] Firing #N from NtWaitEx
[Draw] DrawPrimitive count: 961
[Present] VdSwap called
# Repeat at 60Hz
```

### Regression Tests
1. Boot to interactive state < 30 seconds
2. Frame rate stable at 60 FPS ± 5%
3. No deadlocks or infinite loops
4. Memory usage stable (no leaks)

---

## 11. PPC Recompiled Code Reference

### 11.1 Code Structure Overview

The recompiled PowerPC code lives in `/LibertyRecompLib/ppc/` with:
- **54 source files**: `ppc_recomp.0.cpp` through `ppc_recomp.53.cpp` (~300,000 lines total)
- **43,650 functions** mapped in `ppc_func_mapping.cpp`
- **Comprehensive docs** in `docs.md` (1,078 lines of analysis)

### 11.2 PPC Function Format

Every recompiled function follows this pattern:

```cpp
PPC_FUNC_IMPL(__imp__sub_XXXXXXXX) {
    PPC_FUNC_PROLOGUE();
    PPCRegister temp{};
    uint32_t ea{};
    
    // Register operations
    ctx.r3.u64 = ctx.r4.u64;           // mr r3,r4
    ctx.r5.s64 = 0;                     // li r5,0
    
    // Memory operations
    ea = ctx.r1.u32 + 128;
    PPC_STORE_U32(ea, ctx.r3.u32);     // stw r3,128(r1)
    ctx.r11.u64 = PPC_LOAD_U32(ea);    // lwz r11,128(r1)
    
    // Comparisons
    ctx.cr0.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);  // cmpwi r3,0
    if (ctx.cr0.lt) goto loc_XXXXXXXX;                  // blt loc
    
    // Function calls
    ctx.lr = 0xXXXXXXXX;
    sub_YYYYYYYY(ctx, base);           // bl sub_YYYYYYYY
    __imp__KernelAPI(ctx, base);       // Import call
}

PPC_WEAK_FUNC(sub_XXXXXXXX) {
    __imp__sub_XXXXXXXX(ctx, base);    // Wrapper for hooking
}
```

### 11.3 Address-to-File Mapping

| Address Range | PPC File | Primary Content |
|---------------|----------|-----------------|
| 0x82120000-0x821FFFFF | ppc_recomp.0-4.cpp | Boot, init, game entry |
| 0x82180000-0x8218FFFF | ppc_recomp.3.cpp | Thread creation (`sub_8218C600`) |
| 0x826F0000-0x826FFFFF | ppc_recomp.40-50.cpp | Audio/streaming |
| 0x827D0000-0x827FFFFF | ppc_recomp.60-66.cpp | Workers, render submission |
| 0x82850000-0x8285FFFF | ppc_recomp.68-72.cpp | Shader/effect loading |
| 0x829A0000-0x829AFFFF | ppc_recomp.75-78.cpp | Async I/O, XAM wrappers |
| 0x829C0000-0x829CFFFF | ppc_recomp.80-81.cpp | Network, content |
| 0x829D0000-0x829DFFFF | ppc_recomp.81-82.cpp | **GPU/D3D wrappers** |

### 11.4 GPU Renderer Functions (0x829Dxxxx)

These are the D3D-like wrapper functions that must be hooked to implement rendering:

| Address | Function | Role | PPC File |
|---------|----------|------|----------|
| `0x829D87E8` | `sub_829D87E8` | CreateDevice | ppc_recomp.81.cpp |
| `0x829D8860` | `sub_829D8860` | DrawPrimitive | ppc_recomp.81.cpp |
| `0x829D4EE0` | `sub_829D4EE0` | UnifiedDraw (indexed/non-indexed) | ppc_recomp.82.cpp |
| `0x829D5388` | `sub_829D5388` | Present/VdSwap | ppc_recomp.82.cpp |
| `0x829D3400` | `sub_829D3400` | CreateTexture | ppc_recomp.82.cpp |
| `0x829D3520` | `sub_829D3520` | CreateVertexBuffer | ppc_recomp.82.cpp |
| `0x829D3728` | `sub_829D3728` | SetTexture | ppc_recomp.82.cpp |
| `0x829CD350` | `sub_829CD350` | SetVertexShader | ppc_recomp.81.cpp |
| `0x829D6690` | `sub_829D6690` | SetPixelShader | ppc_recomp.82.cpp |
| `0x829C9070` | `sub_829C9070` | SetVertexDeclaration | ppc_recomp.81.cpp |
| `0x829C96D0` | `sub_829C96D0` | SetIndexBuffer | ppc_recomp.81.cpp |
| `0x829DFAD8` | `sub_829DFAD8` | GpuMemAlloc | ppc_recomp.82.cpp |
| `0x829D7368` | `sub_829D7368` | VdRegisterInterruptCallback | ppc_recomp.82.cpp |
| `0x829DDC90` | `sub_829DDC90` | GPU fence wait loop | ppc_recomp.82.cpp |

**Rewrite Strategy**: Hook `PPC_WEAK_FUNC(sub_829DXXXX)` to intercept calls and implement native rendering instead of letting the PPC code build PM4 packets.

### 11.5 Thread/Worker Functions (0x8218xxxx, 0x827Dxxxx)

| Address | Function | Role | PPC File |
|---------|----------|------|----------|
| `0x8218C600` | `sub_8218C600` | **Worker thread creator** (creates 9 threads) | ppc_recomp.3.cpp |
| `0x8218BEA8` | `sub_8218BEA8` | Game entry point | ppc_recomp.3.cpp |
| `0x8218BEB0` | `sub_8218BEB0` | Core update (calls `sub_82120000`) | ppc_recomp.3.cpp |
| `0x827DE858` | `sub_827DE858` | Render worker entry | ppc_recomp.62.cpp |
| `0x827DACD8` | `sub_827DACD8` | Semaphore wait wrapper | ppc_recomp.62.cpp |
| `0x827DAD60` | `sub_827DAD60` | Semaphore signal wrapper | ppc_recomp.62.cpp |
| `0x82192578` | `sub_82192578` | Thread initialization | ppc_recomp.3.cpp |

**`sub_8218C600` Analysis** (Thread Creator):
```
Entry → Allocates 472 bytes per worker
     → Calls sub_827DF248 (thread pool setup)
     → Calls sub_82192578 (thread init)
     → Creates 9 worker threads via ExCreateThread
     → Workers wait on semaphores 0xA82487F0/B0
```

### 11.6 Async I/O Functions (0x829Axxxx)

| Address | Function | Role | Imports Used |
|---------|----------|------|--------------|
| `0x829A1F00` | `sub_829A1F00` | Async driver/wait helper | `NtWaitForSingleObjectEx` |
| `0x829A1A50` | `sub_829A1A50` | Async status helper | Returns 996/997/0 |
| `0x829AAD20` | `sub_829AAD20` | Async read wrapper | `NtReadFile`, `NtWaitForSingleObjectEx` |
| `0x829AADB8` | `sub_829AADB8` | Async write wrapper | `NtWriteFile`, `NtWaitForSingleObjectEx` |
| `0x829A9738` | `sub_829A9738` | Wait-with-retry helper | Retries on return 257 |
| `0x829A3318` | `sub_829A3318` | Boot orchestrator | `XamTaskShouldExit` loop |
| `0x829A2AE0` | `sub_829A2AE0` | Filesystem open+alloc | `NtCreateFile`, `NtAllocateVirtualMemory` |

### 11.7 Boot-Critical Functions

| Address | Function | Role | Blocking Condition |
|---------|----------|------|-------------------|
| `0x82120000` | `sub_82120000` | One-time init | Returns 0 until ready |
| `0x8218BEA8` | `sub_8218BEA8` | Game entry | Single call (no loop) |
| `0x827D89B8` | `sub_827D89B8` | Frame tick | Per-frame processing |
| `0x828529B0` | `sub_828529B0` | Main loop orchestrator | Calls VdSwap chain |
| `0x828507F8` | `sub_828507F8` | Frame presentation | Calls `sub_829D5388` |
| `0x829A3560` | `sub_829A3560` | Task+mount integration | `XamTaskSchedule` |

### 11.8 State Machine Patterns in PPC Code

**Pattern 1: Cooperative Polling (996 = No Progress)**
```cpp
// sub_827DBF10 pattern
status = sub_829A1A50(obj+8, &out, 0);
if (status == 996) {
    return 0;  // Caller must retry
}
// State advances only when status != 996
```

**Pattern 2: Pending State (997 = Async Pending)**
```cpp
// XamContentClose pattern in sub_827DDE30
status = sub_829A1CA0(handle);  // XamContentClose wrapper
if (status == 997) {
    *(obj+4) = 4;  // Set sub-state to pending
    return 0;      // Caller must retry
}
```

**Pattern 3: Enforced Wait (259 = Wait Required)**
```cpp
// sub_829A1F00 pattern
status = indirect_call(...);
if (status == 259) {
    NtWaitForSingleObjectEx(handle, 1, 0, 0);
    result = *(stack+80);
}
```

**Pattern 4: GPU Fence Loop (258 = Retry)**
```cpp
// sub_829DDC90 pattern
loc_829DDCF0:
    status = KeWaitForSingleObject(r28+32, ...);
    if (status == 258) goto loc_829DDCF0;  // Retry wait
```

### 11.9 Import Call Index (Critical APIs)

| Import | # Callers | Key Caller Functions |
|--------|-----------|---------------------|
| `KeWaitForSingleObject` | 8 | sub_82169400, sub_829DDC90, sub_829A3318 |
| `NtCreateFile` | 4 | sub_829A2AE0, sub_829A3560, sub_829A4278 |
| `NtReadFile` | 3 | sub_829A3560, sub_829AAD20 |
| `NtWaitForSingleObjectEx` | 5 | sub_829A1F00, sub_829AAD20, sub_829AADB8 |
| `XamTaskShouldExit` | 1+ | sub_829A3318 (boot orchestrator) |
| `XamTaskSchedule` | 1+ | sub_829A3560 (task integration) |
| `VdSwap` | 1 | sub_829D5388 (Present) |

### 11.10 How to Hook PPC Functions

To rewrite a PPC function, hook the weak wrapper in `imports.cpp`:

```cpp
// In imports.cpp - Hook sub_829D8860 (DrawPrimitive)
extern "C" void __imp__sub_829D8860(PPCContext& ctx, uint8_t* base);

PPC_FUNC(sub_829D8860) {
    // Extract parameters from PPC registers
    uint32_t deviceCtx = ctx.r3.u32;  // Device context pointer
    uint32_t primType = ctx.r4.u32;   // Primitive type
    uint32_t startVert = ctx.r5.u32;  // Start vertex
    uint32_t vertCount = ctx.r6.u32;  // Vertex count
    
    // Implement native draw call instead of PPC code
    Video::DrawPrimitive(primType, startVert, vertCount);
    
    // Return success in r3
    ctx.r3.u32 = 0;
}
```

### 11.11 Key Data Structure Offsets

**Device Context** (14KB structure at TLS+1676):
- +48: Command buffer pointer
- +10456: Vertex declaration
- +10932: Current vertex shader
- +10936: Current pixel shader
- +12020-12032: Stream sources (vertex buffers)
- +13580: Index buffer
- +19480: Frame buffer index

**XXOVERLAPPED** (Async I/O):
- +0x00: Result (0xFFFFFFFF while pending)
- +0x04: Length (bytes transferred)
- +0x0C: Event handle
- +0x10: Completion routine

**Worker Thread Context**:
- +0: vtable pointer
- +4-8: Thread state
- +36-52: Work queue data

---

## 12. Rewriting the Renderer

### 12.1 Minimum Viable Renderer

To get frames rendering, hook these functions in order:

1. **`sub_829D87E8`** (CreateDevice) - Initialize host GPU context
2. **`sub_829CD350`** (SetVertexShader) - Bind VS from shader cache
3. **`sub_829D6690`** (SetPixelShader) - Bind PS from shader cache
4. **`sub_829D8860`** (DrawPrimitive) - Issue native draw call
5. **`sub_829D5388`** (Present) - Swap buffers, fire VBlank

### 12.2 Renderer Data Flow

```
Game Code
    │
    ▼
Device Context (14KB)     ◄── Track state here
    │
    ├── VS/PS handles ──────► Shader Cache Lookup
    ├── Vertex buffers ─────► Native VB binding
    ├── Index buffer ───────► Native IB binding
    ├── Textures ───────────► Native texture binding
    │
    ▼
sub_829D8860 (DrawPrimitive)
    │
    ▼
Native GPU Draw Call
```

### 12.3 PM4 Bypass Strategy

The PPC code builds PM4 command packets for Xenos GPU. **Do not process PM4**. Instead:

1. Hook D3D wrapper entry points (layer 1)
2. Extract state from device context structure
3. Issue native GPU commands
4. Skip all PM4 packet builders:
   - `sub_829D7E58` (PM4 packet builder)
   - `sub_829D8568` (PM4 draw packet)
   - `sub_829D7740` (PM4 state packet)

---

## 13. Rewriting the Thread System

### 13.1 Thread Creation Flow

```
sub_8218C600 (Worker Creator)
    │
    ├── Allocates 472 bytes per worker
    ├── Calls sub_827DF248 (thread pool setup)
    ├── Calls sub_82192578 (thread initialization)
    │
    └── For each of 9 workers:
            │
            ├── ExCreateThread(entry, stack, priority)
            └── Worker waits on semaphore
```

### 13.2 Worker Semaphore Map

| Semaphore | Worker Type | Count | Initial |
|-----------|------------|-------|---------|
| 0xA82487F0 | Render #1 | 1 | 0 (blocked) |
| 0xA82487B0 | Render #2 | 1 | 0 (blocked) |
| 0xA82402B0 | Streaming | 3 | Active |
| 0xEB2D00B0 | Resource | 3 | Active |

### 13.3 Breaking the Deadlock

The deadlock cycle:
```
Main Thread ─────────────────────────┐
    │                                │
    ├── Creates workers              │
    ├── Waits for XAM tasks          │
    │       │                        │
    │       └── XamTaskShouldExit    │
    │           returns 1 (EXIT)     │
    │           ▼                    │
    │       Tasks exit immediately   │
    │           ▼                    │
    └── Never signals workers ◄──────┘
            │
Render Workers ────────────────────────
    │
    └── Wait on semaphores forever
```

**Fix**: `XamTaskShouldExit` must return 0 so tasks complete and signal workers.

---

## 14. Port Features Roadmap

This section documents features planned for LibertyRecomp to achieve parity with premium recompilation ports like UnleashedRecomp/MarathonRecomp.

### 14.1 Save Data System

**Status**: Basic stubs exist, full implementation pending

#### Xbox 360 Original Behavior
GTA IV uses the Xbox 360 content system for save data:
- `XamContentCreateEx` - Create/open content containers
- `XamContentClose` - Close content container
- `XamContentCreateEnumerator` - List available saves
- `XamUserReadProfileSettings` / `XamUserWriteProfileSettings` - Player profile data

#### PPC Import Addresses
| Address | Function | Purpose |
|---------|----------|---------|
| `0x82A0257C` | `XamContentCreateEx` | Create save container |
| `0x82A0258C` | `XamContentClose` | Close container |
| `0x82A025BC` | `XamContentCreateEnumerator` | Enumerate saves |
| `0x82A02D4C` | `XamUserReadProfileSettings` | Read profile |
| `0x82A02F8C` | `XamUserWriteProfileSettings` | Write profile |

#### PPC Game Functions (Hookable)
These are the actual game functions that wrap the XAM imports - hook these for save logic:

| Address | Function | Role | PPC File |
|---------|----------|------|----------|
| `0x829A1C38` | `sub_829A1C38` | **Content creation wrapper** - calls `XamContentCreateEx` | ppc_recomp.79.cpp:37654 |
| `0x829A1CA0` | `sub_829A1CA0` | **Content close wrapper** - direct jump to `XamContentClose` | ppc_recomp.79.cpp:37723 |
| `0x829A1CB8` | `sub_829A1CB8` | **Enumeration wrapper** - calls `XamContentCreateEnumerator` | ppc_recomp.79.cpp:37756 |
| `0x8297A930` | `sub_8297A930` | **Save manager** - orchestrates save operations, calls `sub_829A1878` | ppc_recomp.77.cpp:7308 |
| `0x829B8580` | Context around `XamUserReadProfileSettings` | Profile read logic | ppc_recomp.80.cpp:36958 |

**Hook Strategy**: Override `PPC_WEAK_FUNC(sub_829A1C38)` to intercept save creation, implement custom save format.

#### Current Implementation (`kernel/xam.cpp`)
```cpp
// Save data path: GetSavePath(true) → ~/.local/share/LibertyRecomp/save/
// Content types: XCONTENTTYPE_SAVEDATA (1), XCONTENTTYPE_DLC (2)
// Enumeration via XamEnumerator template class
```

#### Implementation Requirements
1. **Save Directory Structure**: `<user>/save/<content_name>/` 
2. **Profile Settings**: Map Xbox 360 profile settings to local config
3. **Cross-Platform Paths**: Use `user/paths.h` for platform abstraction
4. **Atomic Writes**: Prevent corruption on crash/power loss
5. **Multiple Slots**: Support GTA IV's 16 save slot system

---

### 14.2 Achievements System

**Status**: Stub returns `ERROR_NO_MORE_FILES`, needs full implementation

#### Xbox 360 Original Behavior
- `XamUserCreateAchievementEnumerator` - List achievements
- Achievements unlocked via internal game logic
- Notifications displayed via XAM notification system

#### PPC Import Addresses
| Address | Function | Purpose |
|---------|----------|---------|
| `0x82A0250C` | `XamUserCreateAchievementEnumerator` | List achievements |
| `0x82A0252C` | `XamNotifyCreateListener` | Create notification listener |

#### PPC Game Functions (Hookable)
| Address | Function | Role | PPC File |
|---------|----------|------|----------|
| `0x829A1878` | `sub_829A1878` | **Achievement enumeration wrapper** - sets up params, calls `XamUserCreateAchievementEnumerator` | ppc_recomp.79.cpp:37028 |
| `0x8297A930` | `sub_8297A930` | **Achievement/stats manager** - calls `sub_829A1878` with game context at offset 208 | ppc_recomp.77.cpp:7308 |
| `0x829A1950` | `sub_829A1950` | **Notification listener creator** - calls `XamNotifyCreateListener` with area=3 | ppc_recomp.79.cpp:37168 |

**Achievement Enumeration Call Chain** (from PPC analysis):
```
sub_8297A930 (stats manager)
    └── Loads context from r3+208
    └── Calls sub_824B8758 (data setup)
    └── Calls sub_829A1878 (achievement enum)
            └── __imp__XamUserCreateAchievementEnumerator
```

**Hook Strategy**: 
- Hook `sub_8297A930` to intercept achievement queries
- Implement custom achievement database that returns our tracked unlocks
- Use `XamNotifyEnqueueEvent()` from host side to display unlock toasts

#### Implementation Plan
1. **Achievement Database**: Define GTA IV's ~50 achievements in config
2. **Progress Tracking**: Hook game events to track unlock conditions
3. **Notification UI**: Toast-style popups matching game aesthetic
4. **Achievement Menu**: New menu screen showing progress/unlocks
5. **Persistence**: Save unlock state to user profile

#### Achievement Categories (GTA IV)
- **Story**: Complete missions (Liberty City Minute, etc.)
- **Side Content**: Pigeons, stunt jumps, random characters
- **Multiplayer**: Various MP modes (stubbed/disabled)
- **Misc**: Reach max wanted level, etc.

#### Design Notes
> *"Achievements are recreated with integrated notifications and a new menu faithful to the game's design language. Get all of them and you will be rewarded with a gold trophy!"*

---

### 14.3 Network/Multiplayer Features

**Status**: Stub-only, requires significant work for online functionality

#### Xbox 360 Original Behavior
GTA IV uses Xbox Live for multiplayer:
- `XNetStartup` / `XNetCleanup` - Network initialization
- `XSessionCreate` / `XSessionJoin` - Session management  
- `XLiveInitialize` - Xbox Live services
- BSD sockets API via `NetDll_*` wrappers

#### PPC Import Addresses (Network Layer)
| Address | Function | Purpose |
|---------|----------|---------|
| `0x82A02D9C` | `NetDll_WSAStartup` | Winsock init |
| `0x82A02DAC` | `NetDll_WSACleanup` | Winsock cleanup |
| `0x82A02DBC` | `NetDll_socket` | Create socket |
| `0x82A02DCC` | `NetDll_closesocket` | Close socket |
| `0x82A02DDC` | `NetDll_shutdown` | Shutdown socket |
| `0x82A02DEC` | `NetDll_ioctlsocket` | Socket ioctl |
| `0x82A02DFC` | `NetDll_setsockopt` | Set socket options |
| `0x82A02E1C` | `NetDll_bind` | Bind socket |
| `0x82A02E2C` | `NetDll_connect` | Connect socket |
| `0x82A02E3C` | `NetDll_listen` | Listen on socket |
| `0x82A02E4C` | `NetDll_accept` | Accept connection |
| `0x82A02E5C` | `NetDll_select` | Select on sockets |
| `0x82A02E6C` | `NetDll_recv` | Receive data |
| `0x82A02E7C` | `NetDll_recvfrom` | Receive from |
| `0x82A02E8C` | `NetDll_send` | Send data |
| `0x82A02E9C` | `NetDll_sendto` | Send to |

#### PPC Import Addresses (Xbox Live Layer)
| Address | Function | Purpose |
|---------|----------|---------|
| `0x82A02EDC` | `NetDll_XNetStartup` | XNet initialization |
| `0x82A02EEC` | `NetDll_XNetCleanup` | XNet cleanup |
| `0x82A02EFC` | `NetDll_XNetXnAddrToInAddr` | Xbox addr to IP |
| `0x82A02F0C` | `NetDll_XNetServerToInAddr` | Server addr to IP |
| `0x82A02F1C` | `NetDll_XNetUnregisterInAddr` | Unregister addr |
| `0x82A02F2C` | `NetDll_XNetGetConnectStatus` | Connection status |
| `0x82A02F3C` | `NetDll_XNetQosListen` | QoS listen |
| `0x82A02F4C` | `NetDll_XNetQosLookup` | QoS lookup |
| `0x82A02F5C` | `NetDll_XNetQosRelease` | QoS release |
| `0x82A02F6C` | `NetDll_XNetGetTitleXnAddr` | Get network address |
| `0x82A02F7C` | `NetDll_XNetGetEthernetLinkStatus` | Link status |
| `0x82A02FAC` | `XamSessionRefObjByHandle` | Session object ref |
| `0x82A02FBC` | `XamSessionCreateHandle` | Create session handle |

#### PPC Game Functions (Hookable for Online)
| Address | Function | Role | PPC File |
|---------|----------|------|----------|
| `0x829C4390` | `sub_829C4390` | **XNet startup wrapper** - calls `XexGetModuleHandle`, `XexGetProcedureAddress`, then `XNetStartup` | ppc_recomp.80.cpp:67081 |
| `0x829C4458` | `sub_829C4458` | **XNet cleanup wrapper** - calls `XNetCleanup` | ppc_recomp.80.cpp:67208 |
| `0x829C4548` | `sub_829C4548` | **Get title address wrapper** - calls `XNetGetTitleXnAddr` | ppc_recomp.80.cpp:67391 |
| `0x829C44A0` | `sub_829C44A0` | **Connection status wrapper** - calls `XNetGetConnectStatus` | ppc_recomp.80.cpp:67274 |
| `0x829C4460` | `sub_829C4460` | **Xbox addr to IP wrapper** | ppc_recomp.80.cpp:67221 |
| `0x829C44B0` | `sub_829C44B0` | **QoS listen wrapper** | ppc_recomp.80.cpp |
| `0x829C44D0` | `sub_829C44D0` | **QoS lookup wrapper** | ppc_recomp.80.cpp |

#### PPC Multiplayer Manager Functions
| Address | Function | Role | PPC File |
|---------|----------|------|----------|
| `0x82973EE0` | `sub_82973EE0` | **Network session init** - called extensively during MP setup | ppc_recomp.76.cpp:54957 |
| `0x82973FA8` | `sub_82973FA8` | **Network state checker** - checks online status | ppc_recomp.76.cpp:55125 |
| `0x82973F50` | `sub_82973F50` | **Session validation** - validates player sessions | ppc_recomp.76.cpp:55059 |
| `0x829733B0` | `sub_829733B0` | **MP mode handler** - called during multiplayer mode setup | ppc_recomp.58.cpp |
| `0x82973460` | `sub_82973460` | **MP state handler** | ppc_recomp.58.cpp |
| `0x82808718` | `sub_82808718` | **Network shutdown** (via `sub_827FFF80`) | ppc_recomp.64.cpp |
| `0x828087B0` | `sub_828087B0` | **Network cleanup** (via `sub_827FFF88`) | ppc_recomp.64.cpp |
| `0x82808CC8` | `sub_82808CC8` | **Session management** | ppc_recomp.64.cpp |

**Network Startup Call Chain** (from PPC analysis):
```
sub_829C4390 (XNet startup)
    └── XamGetSystemVersion (check system version)
    └── XexGetModuleHandle (get xnet module)
    └── XexGetProcedureAddress (get function 80)
    └── Either: indirect call OR __imp__NetDll_XNetStartup
```

**Multiplayer Session Flow** (from `ppc_recomp.76.cpp`):
```
sub_82973FA8 (check online status)
    └── sub_829C44A0 (XNetGetConnectStatus)
    └── Returns online/offline state

sub_82973EE0 (session init) - called 50+ times
    └── Initializes session structures
    └── Sets up player slots
```

#### Implementation Strategy for Online

**Phase 1: Basic Connectivity (LAN/Direct IP)**
1. Implement real `NetDll_socket`, `bind`, `connect`, `send`, `recv` using host sockets
2. Hook `sub_829C4390` to initialize host networking
3. Hook `sub_829C4548` to return valid XNADDR structure
4. Hook `sub_829C44A0` to return "connected" status

**Phase 2: Session Management**
1. Implement `XamSessionCreateHandle` / `XamSessionRefObjByHandle`
2. Hook `sub_82973EE0` to manage session state
3. Implement player join/leave logic

**Phase 3: Matchmaking (Optional)**
1. Create custom matchmaking server
2. Replace Xbox Live calls with custom server calls
3. Implement NAT traversal if needed

#### Current Host Stubs (`kernel/imports.cpp`)
All network functions are currently stubs that log and return:
```cpp
void NetDll_socket() { LOG_UTILITY("!!! STUB !!!"); }
void NetDll_connect() { LOG_UTILITY("!!! STUB !!!"); }
// etc...
```

**To enable online**, these must be replaced with real socket implementations that:
1. Translate Xbox 360 socket calls to host BSD sockets
2. Handle XNADDR ↔ IP address translation
3. Manage session encryption/authentication (or bypass it)

---

### 14.4 Localization Support

**Status**: Not yet implemented

#### Original Languages
GTA IV supports: English, French, German, Spanish, Italian

#### Implementation Plan
1. **Menu Strings**: Extract and load from game assets
2. **New UI Strings**: Create translation files for port-specific menus
3. **Font Support**: Ensure fonts handle all character sets
4. **Dynamic Switching**: Allow language change without restart

#### File Locations
- Game text: `common.rpf/text/`
- Fonts: `common.rpf/fonts/`

---

### 14.5 High Fidelity Renderer

**Status**: Foundation exists in `gpu/video.cpp`, needs enhancement

#### Goals
- **Color Accuracy**: Match PS3 version (no Xbox 360 color correction filter)
- **Optional Filter**: Recreate Xbox 360 warm filter as toggle
- **Resolution Independence**: Clean scaling at any resolution

#### Xbox 360 Quirks to Address
1. **10-bit framebuffer** → 8-bit output conversion
2. **Tile-based rendering** artifacts at edges
3. **EDRAM** bandwidth limitations (not applicable to PC)

#### Renderer Hooks (from Section 11.4)
| Address | Function | Enhancement |
|---------|----------|-------------|
| `0x829D5388` | Present | Apply post-processing filters |
| `0x829D8860` | DrawPrimitive | Track state for enhanced effects |

---

### 14.6 High Resolution Enhancements

#### MSAA (Multisample Anti-Aliasing)
- **Original**: 2x MSAA on Xbox 360
- **Enhancement**: Support 2x, 4x, 8x, 16x MSAA
- **Implementation**: Configure via render target creation

#### Enhanced Depth of Field
- **Original**: Low-tap DoF, breaks at high resolutions in emulators
- **Enhancement**: 5x5, 7x7, 9x9 tap kernels based on resolution
- **Implementation**: Custom shader replacement

#### Enhanced Motion Blur
- **Original**: Limited samples
- **Enhancement**: More samples for smoother blur
- **Implementation**: Shader modification with sample count option

#### Alpha to Coverage
- **Purpose**: Better AA on transparent textures (foliage, fences)
- **Implementation**: Enable via render state

#### Bicubic Texture Filtering
- **Purpose**: Enhance Global Illumination texture quality
- **Implementation**: Custom sampler or shader modification

#### Reverse-Z Precision
- **Original**: Standard Z-buffer with fighting issues
- **Enhancement**: Reverse-Z for better precision at distance
- **Fixes**: Jittery motion blur, Z-fighting on distant geometry

---

### 14.7 High Frame Rate Support

**Status**: Requires timing system rewrite (see Section 2)

#### Targets
- **Default**: 60 FPS (up from Xbox 360's 30 FPS lock)
- **Options**: 120 FPS, 144 FPS, unlocked

#### Known HFR Issues to Fix
1. **Physics timestep**: Decouple from frame rate
2. **Animation speed**: May run 2x at 60 FPS
3. **Camera interpolation**: Jitter at high frame rates
4. **Timer-based events**: Mission timers, etc.

#### PPC Timing Functions (Imports)
| Address | Function | HFR Impact |
|---------|----------|------------|
| `0x82A02ACC` | `KeQueryPerformanceFrequency` | Timer frequency base |
| VBlank callback | registered via `0x829D7368` | Frame pacing |

#### PPC Game Functions (Hookable for HFR)
| Address | Function | Role | PPC File |
|---------|----------|------|----------|
| `0x8216C770` | Context in `ppc_recomp.2.cpp` | **Delta time calculation** - calls `KeQueryPerformanceFrequency`, computes frame delta | ppc_recomp.2.cpp:12218 |
| `0x829D4DB0` | Context in `ppc_recomp.81.cpp` | **Frame timing** - uses perf frequency for timing | ppc_recomp.81.cpp:40269 |
| `0x829E30E8` | Context in `ppc_recomp.82.cpp` | **GPU timing** - reads perf frequency for GPU sync | ppc_recomp.82.cpp:24070 |
| `0x829A4710` | Context in `ppc_recomp.79.cpp` | **Async timing** - perf counter for async ops | ppc_recomp.79.cpp:44489 |

**Main Loop & Frame Presentation Chain** (critical for HFR):
```
sub_828529B0 (0x828529B0) - Main loop orchestrator
    └── sub_828E0AB8 - Frame setup
    └── sub_829CA360 - Render state setup  
    └── sub_829CA240 - Render target setup (called 4x)
    └── sub_829D3728 - Texture binding loop (19 iterations)
    └── [Frame work...]
    └── sub_828507F8 (0x828507F8) - Frame presentation
            └── sub_829D5388 (0x829D5388) - D3D Present
                    └── __imp__VdSwap - Actual buffer swap
```

**Hook Strategy for HFR**:
1. Hook `sub_828529B0` entry to inject frame timing
2. Hook `sub_829D5388` to control presentation rate
3. Override delta time reads at `0x8216C770` to return fixed timestep
4. Use `app.cpp`'s `GTA4FrameHooks::OnFrameEnd(deltaTime)` for host-side pacing

#### Implementation Notes
- Xbox 360 runs at 30 FPS with VBlank at 60Hz (skip every other)
- PC: Lock delta-time, interpolate visuals
- Reference: UnleashedRecomp HFR fixes in `SWA.toml` midasm hooks
- Host already tracks delta time in `App::s_deltaTime`

---

### 14.8 Ultrawide Support

**Status**: Requires UI and camera modifications

#### Aspect Ratios
- **16:9**: Original (1920x1080, etc.)
- **21:9**: Ultrawide (2560x1080, 3440x1440)
- **32:9**: Super ultrawide (5120x1440)

#### PPC Game Functions (Hookable for Ultrawide)
| Address | Function | Role | PPC File |
|---------|----------|------|----------|
| `0x829CA360` | `sub_829CA360` | **Render state setup** - called from main loop, sets up render targets | ppc_recomp.66.cpp:99708 |
| `0x829CA240` | `sub_829CA240` | **Render target config** - called 4x with different params (0-3) for different buffers | ppc_recomp.66.cpp:99724 |
| `0x829D3728` | `sub_829D3728` | **Texture binding** - loops 19 times binding textures | ppc_recomp.66.cpp:99806 |
| `0x829D1310` | `sub_829D1310` | **Viewport/scissor setup** - render state configuration | ppc_recomp.66.cpp:99776 |
| `0x829D1058` | `sub_829D1058` | **Additional render setup** | ppc_recomp.66.cpp:99782 |

**Resolution Constants Found** (from PPC analysis):
- `1280x720` hardcoded in `VdQueryVideoMode` (kernel/imports.cpp:4064)
- Float `1.0f` (0x3F800000 / 1065353216) used extensively for aspect calculations

**Hook Strategy for Ultrawide**:
1. Hook `VdQueryVideoMode` to return actual display resolution
2. Hook `sub_829CA240` to adjust render target dimensions
3. Find camera FOV calculation (search for aspect ratio math with 1.777... constant)
4. Hook UI positioning functions (need further PPC analysis to identify)

#### Implementation Requirements
1. **FOV Adjustment**: Horizontal+ scaling
2. **UI Alignment Options**:
   - Edges: HUD at screen edges
   - Safe Area: HUD within 16:9 center
3. **Cutscene Handling**: 
   - Default: Pillarbox to original aspect
   - Option: Full width (may have presentation issues)

---

### 14.9 Extended Controller Features

**Status**: Basic input via SDL in `kernel/xam.cpp`

#### Current Implementation
```cpp
// XamInputGetState - Read controller state via SDL
// XamInputSetState - Vibration feedback
// XamInputGetCapabilities - Report controller type
```

#### Enhancements Planned

**D-Pad Navigation**
- Full game playable with D-Pad over analog stick
- Menu navigation already D-Pad compatible

**DualShock 4 / DualSense Features**
- **LED Color**: Dynamic based on game context (wanted level, health)
- **Touchpad**: World map planet rotation
- **Adaptive Triggers**: Weapon feedback (DualSense only)

#### Implementation
```cpp
// LED control via SDL_GameControllerSetLED()
// Touchpad via SDL_CONTROLLERTOUCHPADMOTION event
// Note: May be limited with DS4Windows/Steam Input
```

---

### 14.10 Low Input Latency

**Status**: Requires frame pacing optimization

#### Techniques
1. **Waitable Swap Chain**: D3D12/Vulkan wait for optimal present
2. **Flip Model**: Direct-to-screen presentation (D3D12)
3. **Input Polling Timing**: Poll immediately before frame update
4. **Frame Queue**: Minimize pre-rendered frames (1-2 max)

#### Implementation Points
- Poll input in main loop before game update
- Configure swap chain with `DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT`
- Vulkan: `VK_PRESENT_MODE_MAILBOX_KHR` or `FIFO`

---

## 15. XAM Subsystem Reference

The XAM (Xbox Application Manager) subsystem handles user-facing Xbox 360 services. Critical for save data, profiles, and UI.

### 15.1 Content Types
```cpp
#define XCONTENTTYPE_SAVEDATA  1  // Save games
#define XCONTENTTYPE_DLC       2  // Downloadable content
#define XCONTENTTYPE_RESERVED  3  // System use
```

### 15.2 XAM Function Map
| Address | Function | Implementation Status |
|---------|----------|----------------------|
| `0x82A024AC` | `XamUserGetName` | Stub (returns "Player") |
| `0x82A024BC` | `XamUserGetSigninState` | Stub (signed in) |
| `0x82A024CC` | `XamUserAreUsersFriends` | Stub (false) |
| `0x82A024DC` | `XamUserCheckPrivilege` | Stub (allowed) |
| `0x82A024FC` | `XamUserCreateStatsEnumerator` | Stub |
| `0x82A0250C` | `XamUserCreateAchievementEnumerator` | Stub (no items) |
| `0x82A0251C` | `XamUserGetXUID` | Stub |
| `0x82A0257C` | `XamContentCreateEx` | **Implemented** |
| `0x82A0258C` | `XamContentClose` | **Implemented** |
| `0x82A025BC` | `XamContentCreateEnumerator` | **Implemented** |
| `0x82A025FC` | `XamTaskSchedule` | **Implemented** (sync) |
| `0x82A0260C` | `XamInputGetState` | **Implemented** |
| `0x82A0261C` | `XamInputGetCapabilities` | **Implemented** |
| `0x82A0262C` | `XamInputSetState` | **Implemented** |

### 15.3 Notification System
```cpp
// Create listener for specific event areas
uint32_t XamNotifyCreateListener(uint64_t qwAreas);

// Queue notification for listeners
void XamNotifyEnqueueEvent(uint32_t dwId, uint32_t dwParam);

// Check for pending notifications
bool XNotifyGetNext(uint32_t hNotification, uint32_t dwMsgFilter, 
                    be<uint32_t>* pdwId, be<uint32_t>* pParam);
```

Use for: Achievement popups, controller connect/disconnect, profile changes.

---

## 16. Execution Trace: Entry Point to First Draw Call

This section documents the complete execution path from the game's entry point (`_xstart`) to the first draw call (`VdSwap`). Traced manually from PPC recompiled code.

### 16.1 Host Boot Chain

```
main.cpp
    └── KiSystemStartup()          // Host system setup, mounts game:\, D:\
    └── LdrLoadModule(modulePath)  // Loads XEX, returns entry_point = 0x829A0860
    └── GuestThread::Start({ entry, 0, 0, 0 })
            └── g_memory.FindFunction(0x829A0860) → _xstart
```

### 16.2 _xstart (0x829A0860) - CRT Entry Point

**Location:** `ppc_recomp.79.cpp:34255`

```
_xstart (0x829A0860)
    ├── sub_829A7FF8              // Early system init
    ├── sub_829A7960              // System setup (r3=1)
    ├── sub_829A0678              // Privilege check
    │   └── If fails → XamLoaderTerminateTitle
    ├── sub_82994700              // Runtime init
    ├── sub_829A7EA8              // More system setup
    ├── sub_829A7DC8              // Additional init
    ├── sub_829A27D8              // Command line parsing setup
    └── sub_8218BEA8              // ★ GAME MAIN ENTRY
            └── sub_827D89B8      // Game initialization wrapper
```

### 16.3 sub_827D89B8 (0x827D89B8) - Game Init Wrapper

**Location:** `ppc_recomp.62.cpp:20371`

```
sub_827D89B8 (Game Init Wrapper)
    ├── sub_827D8840              // Pre-init setup
    ├── sub_827FFF80              // Network init (→ sub_82808718)
    ├── sub_827EEDE0              // Store argc/argv to globals
    ├── sub_828E0AB8              // Frame tick (called many times)
    ├── sub_827EE620              // Additional setup
    ├── [Indirect vtable call]    // Via offset 52 - engine init
    ├── sub_8218BEB0              // ★ ACTUAL GAME MAIN
    │       └── sub_82120000      // Game initialization
    │       └── sub_821200D0      // Post-init
    │       └── sub_821200A8      // Finalize init
    ├── [Indirect vtable call]    // Via offset 56 - cleanup
    ├── sub_827EECE8              // Cleanup
    └── sub_827FFF88              // Network cleanup
```

### 16.4 sub_82120000 (0x82120000) - Game Initialization

**Location:** `ppc_recomp.0.cpp:3`

```
sub_82120000 (Game Init)
    ├── sub_8218C600              // Check initialization flag
    │   └── If fails → return 0
    ├── sub_82120EE8              // Core engine init
    ├── sub_821250B0              // Memory/context allocation
    ├── sub_82318F60              // RAGE engine setup
    ├── sub_82124080              // Subsystem init
    └── sub_82120FB8              // ★ MAIN GAME SETUP (large init function)
```

### 16.5 sub_82120FB8 (0x82120FB8) - Main Game Setup

**Location:** `ppc_recomp.0.cpp:2531`

This is a massive initialization function (~450 lines) that sets up all game systems:

```
sub_82120FB8 (Main Setup)
    ├── XNotifyPositionUI          // UI notification setup
    ├── sub_822C1A30               // Streaming init
    ├── sub_82679950               // Graphics init
    ├── sub_8221D880               // World init
    ├── sub_827DB118               // Device context setup (×2)
    ├── sub_8219FD88               // Camera system init
    ├── sub_822F8980               // Resource manager init
    ├── sub_822EEDB8               // Audio system init
    ├── sub_82270170               // Vehicle system init
    ├── sub_822FD328               // Object pool init (2000 objects)
    ├── sub_822EFF40               // Physics init
    ├── sub_82120C48               // Player init
    ├── sub_82221410               // Script init
    ├── sub_8226CB50               // Weapon system init
    ├── sub_821A8868               // HUD init
    ├── sub_821BC9E0               // Menu system init
    ├── sub_822DB4B0               // Cutscene system init
    ├── sub_821B7218               // Mission system init
    ├── sub_822498F8               // Checkpoint system init
    ├── sub_8225DC40               // Weather system init
    ├── sub_821E24E0               // Population system init
    ├── sub_821DFD18               // Traffic system init
    ├── sub_8220E108               // Wanted system init
    ├── sub_821AB5F8               // Radio system init
    ├── sub_821D8358               // Map/GPS init
    ├── sub_821EA0B8               // Blip system init
    ├── sub_82122CA0               // Save system init
    ├── sub_82200EB8               // Stats system init
    ├── sub_8212FB78               // Friend system init
    ├── sub_8219ADF0               // Online system init
    ├── sub_8212F578               // Leaderboard init
    ├── sub_8212EDC8               // Achievement tracking init
    ├── sub_82138710               // Replay system init
    ├── sub_821B2ED8               // Camera recording init
    ├── sub_822467B8               // Cinematic camera init
    ├── sub_82208460               // Photo mode init
    ├── sub_821B9DA8               // TV system init
    ├── sub_82258100               // Internet cafe init
    ├── sub_821A03A0               // Phone system init
    ├── sub_8232A2C0               // Dating system init
    ├── sub_821B5DE8               // Bowling/darts init
    ├── sub_821D8058               // Pool/drinking init
    ├── sub_822868C8               // Cabaret init
    ├── sub_82289698               // Strip club init
    ├── sub_82125478               // Final setup
    ├── sub_8298ED98               // Thread setup
    ├── sub_827E0C30               // Register update callback
    ├── sub_827E0CF8               // Register render callback
    ├── sub_8227AC28               // World finalization
    ├── sub_82272290               // Entity system finalize
    ├── sub_82212450               // Script system finalize
    ├── sub_822C5768               // Streaming finalize
    └── sub_822D4C68               // Content finalize
```

### 16.6 Main Loop Entry - sub_82856F08 (0x82856F08)

**Location:** `ppc_recomp.66.cpp:110400`

After initialization completes, the main loop is entered through a registered callback:

```
sub_82856F08 (Main Loop Entry)
    ├── Setup frame parameters
    ├── Load game state from context
    └── sub_828529B0               // ★ MAIN LOOP ORCHESTRATOR
```

### 16.7 sub_828529B0 (0x828529B0) - Main Loop Orchestrator

**Location:** `ppc_recomp.66.cpp:99661`

```
sub_828529B0 (Main Loop)
    ├── sub_828E0AB8               // Frame tick
    ├── sub_8285ACE8               // Input processing
    ├── sub_829CA360               // Render state reset
    ├── sub_829CA240               // Render target setup (×4 for buffers 0-3)
    ├── sub_829D1310               // Viewport setup
    ├── sub_829D1058               // Scissor setup
    ├── sub_829D3728               // Texture binding (×19 loop)
    ├── sub_829D14E0               // Sampler setup
    ├── [World update logic...]
    ├── sub_829CB818               // Pre-frame render setup
    └── sub_828507F8               // ★ FRAME PRESENTATION
```

### 16.8 sub_828507F8 (0x828507F8) - Frame Presentation

**Location:** `ppc_recomp.66.cpp:94392`

```
sub_828507F8 (Frame Present)
    ├── sub_829D5920               // Pre-present setup
    ├── sub_829D5950               // Display mode check
    └── sub_829D5388               // ★ D3D PRESENT WRAPPER
```

### 16.9 sub_829D5388 (0x829D5388) - D3D Present Wrapper (VdSwap)

**Location:** `ppc_recomp.81.cpp:41226`

```
sub_829D5388 (D3D Present)
    ├── sub_82990830               // Prepare display params
    ├── [Setup frontbuffer/backbuffer pointers]
    ├── [Calculate display dimensions]
    └── __imp__VdSwap              // ★ FIRST DRAW CALL (0x82A0310C)
            └── Actual buffer swap to screen
```

### 16.10 Complete Call Chain Summary

```
ENTRY: _xstart (0x829A0860)
    └── sub_8218BEA8 (game main)
        └── sub_827D89B8 (init wrapper)
            └── sub_8218BEB0 (actual main)
                └── sub_82120000 (game init)
                    └── sub_82120FB8 (full system init)
                        └── [~50 subsystem inits]

MAIN LOOP: sub_82856F08 (via callback)
    └── sub_828529B0 (main loop)
        └── sub_828507F8 (frame present)
            └── sub_829D5388 (D3D present)
                └── __imp__VdSwap (FIRST DRAW @ 0x829D55D4)
```

### 16.11 Key Addresses Quick Reference

| Address | Function | Role |
|---------|----------|------|
| `0x829A0860` | `_xstart` | CRT entry point |
| `0x8218BEA8` | `sub_8218BEA8` | Game main entry |
| `0x827D89B8` | `sub_827D89B8` | Init wrapper |
| `0x82120000` | `sub_82120000` | Game initialization |
| `0x82120FB8` | `sub_82120FB8` | Full system setup |
| `0x82856F08` | `sub_82856F08` | Main loop entry |
| `0x828529B0` | `sub_828529B0` | Main loop orchestrator |
| `0x828507F8` | `sub_828507F8` | Frame presentation |
| `0x829D5388` | `sub_829D5388` | D3D Present wrapper |
| `0x82A0310C` | `__imp__VdSwap` | Actual buffer swap |

### 16.12 Hooking Points for Debugging

To debug boot issues, add logging at these key points:

```cpp
// In kernel/imports.cpp or via PPC_WEAK_FUNC overrides:

// 1. Entry point confirmation
PPC_WEAK_FUNC(_xstart) {
    LOGF_IMPL(Utility, "Boot", "★ _xstart ENTERED");
    __imp___xstart(ctx, base);
}

// 2. Game init start
PPC_WEAK_FUNC(sub_82120000) {
    LOGF_IMPL(Utility, "Boot", "★ Game init starting");
    __imp__sub_82120000(ctx, base);
}

// 3. Main loop entry
PPC_WEAK_FUNC(sub_828529B0) {
    LOGF_IMPL(Utility, "Boot", "★ MAIN LOOP ENTERED!");
    __imp__sub_828529B0(ctx, base);
}

// 4. First draw call
PPC_WEAK_FUNC(sub_829D5388) {
    LOGF_IMPL(Utility, "Render", "★ D3D Present called");
    __imp__sub_829D5388(ctx, base);
}
```

---

## 17. Deep Boot Function Traces

### 17.1 sub_829A7FF8 (0x829A7FF8) - Early System Init

**Location:** `ppc_recomp.79.cpp:56985`

This function performs early system initialization, checking XEX header validity and allocating initial memory.

```
sub_829A7FF8 (Early System Init)
    ├── sub_829A7F20              // XEX header validation
    │   ├── RtlImageXexHeaderField    // Get XEX header field (0x20001025)
    │   ├── Checks XEX header validity
    │   ├── If invalid:
    │   │   └── sub_829A5F10          // Allocate 1MB memory block
    │   │       └── Args: type=2, addr=0, size=0x100000, flags=4096
    │   └── Returns 1 if valid, 0 if allocation failed
    │
    ├── If sub_829A7F20 returns 0:
    │   ├── Load vtable from global [0x81200000+1712]
    │   ├── Call vtable[24] with args (2, 0)  // Shutdown callback
    │   └── HalReturnToFirmware(1)            // Fatal error - halt system
    │
    └── Returns (does not return if validation fails)
```

**Key Details:**
- XEX header field `0x20001025` checked for validity
- If invalid, attempts 1MB allocation via `sub_829A5F10`
- Fatal path calls `HalReturnToFirmware` - never returns

---

### 17.2 sub_82994700 (0x82994700) - Runtime Init

**Location:** `ppc_recomp.79.cpp:1233`

This function initializes the C++ runtime, thread-local storage, and exception handling.

```
sub_82994700 (Runtime Init)
    ├── Store vtable pointers to globals:
    │   ├── Global[6116] = 0x81A043E8  // CRT vtable 1
    │   ├── Global[6120] = 0x81A02704  // CRT vtable 2  
    │   ├── Global[6124] = 0x8180270C  // CRT vtable 3
    │   └── Global[6128] = 0x8180271C  // CRT vtable 4
    │
    ├── KeTlsAlloc()                   // Allocate TLS slot
    │   └── If returns -1 → fail
    │
    ├── KeTlsSetValue(slot, vtable2)   // Set TLS value
    │   └── If returns 0 → fail
    │
    ├── sub_82992680                   // CRT subsystem init
    │   ├── sub_82998ED0(0)            // Heap init
    │   ├── sub_82998DE0(0)            // Memory manager init
    │   ├── sub_82994830(0)            // Exception handler init
    │   ├── sub_82998DD0(0)            // I/O system init
    │   ├── sub_828E0AB8(0)            // Frame tick
    │   └── sub_82998DB8(0)            // Finalize CRT init
    │
    ├── sub_82998A48                   // Thread pool init
    │   ├── Iterates 36 thread slots (288 bytes / 8)
    │   ├── For each slot with state==1:
    │   │   └── sub_82998E20(ptr, 4000)  // Init thread with 4s timeout
    │   └── Returns 1 on success, 0 on failure
    │
    ├── [Indirect call via vtable1]    // Create main thread object
    │   └── Returns thread handle or -1
    │
    ├── sub_829937E0(1, 196)           // Allocate 196-byte context
    │   └── If returns NULL → fail
    │
    ├── [Indirect call via vtable3]    // Register thread context
    │   └── Args: (handle, context)
    │
    ├── sub_829A2810                   // Get current thread info
    │   └── Returns thread ID
    │
    ├── sub_829A79C0(globalPtr, 1)     // Enable runtime flag
    │
    └── Returns 1 on success, 0 on failure
```

**Critical Dependencies:**
- `KeTlsAlloc` / `KeTlsSetValue` - Thread-local storage
- Thread pool with 36 slots
- 196-byte per-thread context allocation

---

### 17.3 sub_829A0678 (0x829A0678) - Privilege Check

**Location:** `ppc_recomp.79.cpp:33838`

This function checks HDCP/AV output privileges and displays an error message if the game cannot run.

```
sub_829A0678 (Privilege Check)
    ├── XexCheckExecutablePrivilege(10)    // Check HDCP privilege
    │   └── If returns 0 → pass (privilege OK)
    │
    ├── XGetAVPack()                       // Get current AV output type
    │   ├── Returns 3 → pass (HDMI)
    │   ├── Returns 6 → pass (VGA)
    │   ├── Returns 8 → pass (Component HD)
    │   └── Returns 4 → pass (Component)
    │
    ├── ExGetXConfigSetting(2, 2, ...)     // Get video config
    │   └── Check for 768 (HD mode flag)
    │
    ├── ExGetXConfigSetting(3, 10, ...)    // Get display settings
    │   ├── Check bit 23 (0x800000) - HDCP required
    │   └── Check bit 22 (0x400000) - HDCP bypass
    │
    ├── If HDCP check fails:
    │   ├── XGetLanguage()                 // Get system language (1-17)
    │   │
    │   ├── Build language-specific error strings:
    │   │   ├── Languages 1-10 → Use index directly
    │   │   └── Languages 11+ → Map to index 1
    │   │
    │   ├── sub_8298EB90(buffer, 0, 510)   // Clear title buffer
    │   ├── sub_8298EB90(buffer, 0, 62)    // Clear message buffer
    │   │
    │   ├── Language ID mapping table (offset 96-140):
    │   │   └── [9,9,13,11,10,16,12,14,17,15] for EN,DE,FR,ES,IT,JP,KR,ZH,PT,PL
    │   │
    │   ├── sub_829A05F0(langId, titleBuf, 256)  // Load title string
    │   ├── sub_829A05F0(langId, msgBuf, 32)     // Load message string
    │   │
    │   ├── sub_829A0538(titleBuf, msgBuf, 1)    // Show error dialog
    │   │   └── Calls XamShowMessageBoxUIEx
    │   │
    │   └── Return 1 (error state)
    │
    └── Return 0 on success, 1 if user needs to change settings
```

**Privilege Values:**
| AV Pack | Value | Description |
|---------|-------|-------------|
| HDMI | 3 | High-definition HDMI output |
| VGA | 6 | VGA monitor output |
| Component HD | 8 | Component cables (HD) |
| Component | 4 | Component cables (SD) |

**HDCP Flags (ExGetXConfigSetting result):**
- Bit 23 (`0x800000`): HDCP required by content
- Bit 22 (`0x400000`): HDCP bypass allowed

---

### 17.4 sub_829A7EA8 (0x829A7EA8) - More System Setup

**Location:** `ppc_recomp.79.cpp:56788`

This function iterates through a small initialization table (3 entries) and calls each registered init function.

```
sub_829A7EA8 (System Init Table Executor)
    │
    ├── r31 = 0x81820000 (init table start)
    ├── r30 = 0x8182000C (init table end = start + 12)
    │
    ├── Loop while r31 < r30:
    │   ├── If r3 != 0 → exit loop (error state)
    │   │
    │   ├── r11 = [r31] (load function pointer)
    │   │
    │   ├── If r11 != NULL:
    │   │   └── Call r11() via indirect bctrl
    │   │
    │   └── r31 += 4 (next entry)
    │
    └── Return
```

**Init Table Structure (0x81820000):**
| Offset | Function Pointer | Purpose |
|--------|------------------|---------|
| +0 | init_func_1 | First system init |
| +4 | init_func_2 | Second system init |
| +8 | init_func_3 | Third system init |

---

### 17.5 sub_829A7DC8 (0x829A7DC8) - Additional Init (Constructor/Destructor Tables)

**Location:** `ppc_recomp.79.cpp:56654`

This function processes C++ static constructor and destructor tables.

```
sub_829A7DC8 (C++ Static Init)
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 1: Optional Pre-Init Callback
    ├── ═══════════════════════════════════════════
    │
    ├── r10 = Global[0x81269F70] (pre-init callback)
    │
    ├── If r10 != NULL:
    │   └── Call r10() via indirect bctrl
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 2: Constructor Table (.ctors)
    ├── ═══════════════════════════════════════════
    │
    ├── r31 = 0x8182150C (ctors start)
    ├── r30 = 0x81821518 (ctors end = 3 entries)
    │
    ├── Loop while r31 < r30 && r3 == 0:
    │   ├── r11 = [r31] (constructor pointer)
    │   │
    │   ├── If r11 != NULL:
    │   │   └── Call r11() - static constructor
    │   │
    │   └── r31 += 4
    │
    ├── If r3 != 0 → Return (error)
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 3: Initialization Table
    ├── ═══════════════════════════════════════════
    │
    ├── r31 = 0x81820010 (init table start)
    ├── r30 = 0x81821508 (init table end = 1338 entries!)
    │
    ├── Loop while r31 < r30:
    │   ├── r11 = [r31] (init function pointer)
    │   │
    │   ├── If r11 != NULL && r11 != -1:
    │   │   └── Call r11() - initialization function
    │   │
    │   └── r31 += 4
    │
    └── Return 0
```

**Key Tables:**
| Address | Size | Purpose |
|---------|------|---------|
| `0x8182150C` | 12 bytes (3 ptrs) | C++ static constructors |
| `0x81820010` | 5352 bytes (1338 ptrs) | Module init functions |

**Note:** The init table contains **1338 function pointers** - this is where most RAGE engine subsystems are initialized!

---

### 17.6 sub_829A27D8 (0x829A27D8) - Command Line Access

**Location:** `ppc_recomp.79.cpp:39598`

This is a simple accessor function that returns the command line string pointer.

```
sub_829A27D8 (Get Command Line)
    │
    ├── r11 = 0x81200000 (base address)
    │
    ├── r3 = [r11 + 1624]  // Load command line pointer
    │   └── Global @ 0x81200658 = command line string
    │
    └── Return r3 (command line pointer or NULL)
```

**Usage:** Called by `sub_827D8840` to parse command line arguments before game initialization.

---

### 17.7 sub_8218BEA8 (0x8218BEA8) - Game Main Entry

**Location:** `ppc_recomp.3.cpp:1038`

This is a thin wrapper that immediately jumps to the game initialization wrapper.

```
sub_8218BEA8 (Game Main Entry)
    │
    └── Tail call → sub_827D89B8 (Game Init Wrapper)
```

**Note:** This is the first game-specific function called from `_xstart`. It's a direct branch (not call) to `sub_827D89B8`.

---

### 17.8 sub_827D89B8 (0x827D89B8) - Game Initialization Wrapper (Detailed)

**Location:** `ppc_recomp.62.cpp:20371`

This is the main game initialization orchestrator that sets up all game systems.

```
sub_827D89B8 (Game Init Wrapper - Full Trace)
    │
    ├── Store argc/argv on stack:
    │   ├── stack[132] = r3 (argc)
    │   └── stack[140] = r4 (argv)
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 1: Command Line Parsing
    ├── ═══════════════════════════════════════════
    │
    ├── sub_827D8840(&argc, &argv)        // Parse command line
    │   ├── sub_829A27D8()                // Get raw command line
    │   ├── If NULL → return early
    │   │
    │   ├── Store argv array base to global:
    │   │   └── Global[0x81325008] = 0x812649A8
    │   │
    │   ├── Parse loop (while not end of string):
    │   │   ├── Skip whitespace (space=32, tab=9)
    │   │   ├── Handle quoted strings (char 34 = ")
    │   │   ├── Extract argument token
    │   │   ├── Store pointer in argv array
    │   │   └── Increment argc
    │   │
    │   └── NULL-terminate argv array
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 2: Network Initialization
    ├── ═══════════════════════════════════════════
    │
    ├── sub_827FFF80()                    // Network init wrapper
    │   └── sub_82808718()                // XNet startup
    │       ├── Increment init counter: Global[18256]++
    │       │
    │       ├── If first call (counter was 0):
    │       │   ├── Clear XNetStartupParams (13 bytes)
    │       │   ├── params.cfgSizeOfStruct = 13
    │       │   ├── params.cfgFlags = 1
    │       │   │
    │       │   ├── sub_829C4448(&params)     // XNetStartup
    │       │   │
    │       │   ├── sub_829C4090(2, &wsadata) // WSAStartup(2.0)
    │       │   │
    │       │   ├── Check WSA version:
    │       │   │   └── If major != 2 || minor == 0:
    │       │   │       └── sub_829C40A0()    // WSACleanup
    │       │   │
    │       │   └── Return
    │       │
    │       └── Return (already initialized)
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 3: Store argc/argv to Globals
    ├── ═══════════════════════════════════════════
    │
    ├── Reload argc/argv from stack
    │
    ├── sub_827EEDE0(argc, argv)          // Store to globals
    │   ├── Global[0x81328124] = 1        // Init flag
    │   │
    │   ├── If argc == 0 → call sub_827EEE1C
    │   ├── If argv == NULL → call sub_827EEE1C
    │   │
    │   ├── Global[0x81328120] = argv[0]  // First arg (exe path)
    │   ├── Global[0x81328128] = argc     // Argument count
    │   └── Global[0x8132812C] = argv     // Argument array
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 4: Frame Tick & Setup
    ├── ═══════════════════════════════════════════
    │
    ├── sub_828E0AB8()                    // Frame tick
    │
    ├── sub_827EE620()                    // Additional setup
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 5: Engine Init Callback (vtable)
    ├── ═══════════════════════════════════════════
    │
    ├── r31 = Global[0x81325154]          // Engine object
    ├── r11 = r31[4]                      // Check if init needed
    │
    ├── If r11 != NULL:
    │   ├── If r11[0] != 0:
    │   │   └── r4 = r11 (use as param)
    │   │
    │   ├── r3 = CurrentThread[1676]      // Get thread context
    │   ├── r5 = (r31[4] == NULL) ? 0 : 1 // Init flag
    │   │
    │   ├── r11 = r3[0]                   // vtable
    │   ├── r11 = r11[52]                 // vtable[13] = Init method
    │   └── Call r11(r3, r4, r5)          // Engine->Init(ctx, str, flag)
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 6: Main Game Initialization
    ├── ═══════════════════════════════════════════
    │
    ├── sub_8218BEB0()                    // ★ ACTUAL GAME INIT
    │   ├── sub_828E0AB8()                // Frame tick
    │   │
    │   ├── sub_82120000(&stack[80])      // ★ Full game init
    │   │   └── [~50 subsystem initializations]
    │   │
    │   ├── If init failed (r3 == 0):
    │   │   ├── sub_828E0AB8()            // Frame tick
    │   │   └── Return -1 (error)
    │   │
    │   ├── sub_821200D0()                // Post-init phase 1
    │   ├── sub_821200A8()                // Post-init phase 2
    │   ├── sub_828E0AB8()                // Frame tick
    │   └── Return 0 (success)
    │
    ├── r30 = return value (0 = success)
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 7: Engine Cleanup Callback (vtable)
    ├── ═══════════════════════════════════════════
    │
    ├── If r31[4] != NULL:
    │   ├── r3 = CurrentThread[1676]      // Get thread context
    │   ├── r11 = r3[0][56]               // vtable[14] = Cleanup method
    │   └── Call r11(r3)                  // Engine->Cleanup(ctx)
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 8: Finalization
    ├── ═══════════════════════════════════════════
    │
    ├── sub_827EECE8()                    // State cleanup
    │
    ├── sub_828E0AB8()                    // Frame tick
    │
    ├── sub_827FFF88()                    // Network cleanup
    │   └── [Decrement XNet init counter, cleanup if 0]
    │
    └── Return r30 (init result)
```

**Key Globals:**
| Address | Purpose |
|---------|---------|
| `0x81325008` | argv array base pointer |
| `0x81325154` | Engine object pointer |
| `0x81328120` | First command line arg |
| `0x81328124` | Init complete flag |
| `0x81328128` | argc count |
| `0x8132812C` | argv array |

---

## 18. Renderer Execution Trace (Detailed)

### 18.1 Render Pipeline Overview

The renderer operates through a command buffer system that builds GPU commands in guest memory, then submits them via `VdSwap`.

```
Main Loop (sub_828529B0)
    │
    ├── sub_828E0AB8           // Frame tick (timing)
    ├── sub_8285ACE8           // Input processing
    │
    ├── ═══════════════════════════════════════════
    │   RENDER STATE SETUP
    ├── ═══════════════════════════════════════════
    │
    ├── sub_829CA360(device, depthStencil)     // Set depth/stencil target
    │
    ├── sub_829CA240(device, rtIdx, rt, offset, pitch, flags)  // Set render target
    │   └── Called 4 times for buffers 0-3
    │
    ├── sub_829D1310(device, viewport)         // Set viewport
    │
    ├── sub_829D1058(device, scissor)          // Set scissor rect
    │
    ├── ═══════════════════════════════════════════
    │   TEXTURE BINDING (19 slots)
    ├── ═══════════════════════════════════════════
    │
    ├── Loop 19 times (slots 0-18):
    │   └── sub_829D3728(device, slot, texture, samplerMask)
    │
    ├── sub_829D14E0(device, samplerState)     // Configure samplers
    │
    ├── ═══════════════════════════════════════════
    │   WORLD UPDATE & RENDER
    ├── ═══════════════════════════════════════════
    │
    ├── [World update logic - indirect calls]
    │
    ├── sub_829CB818(device, ...)              // Pre-frame render setup
    │
    └── sub_828507F8                           // Frame presentation
            └── sub_829D5388                   // D3D Present
                    └── VdSwap                 // Buffer swap
```

### 18.2 sub_829CA360 (0x829CA360) - Render State Reset

**Location:** `ppc_recomp.81.cpp:14073`

Sets or clears the depth/stencil target for the current render pass.

```
sub_829CA360(device, depthStencil)
    │
    ├── r31 = device
    ├── r29 = depthStencil (new target)
    ├── r30 = device[12428] (current depth target)
    │
    ├── If current target (r30) != 0:
    │   ├── Check device[10908] (deferred flag)
    │   │   └── If set: Store to r30[8]
    │   │
    │   ├── Check device[10912] (dirty mask) & r30[0]
    │   │   └── If dirty:
    │   │       ├── Get command buffer ptr: device[13508]
    │   │       ├── Check against limit: device[13512]
    │   │       ├── If full: sub_829D5B60(device) // Flush command buffer
    │   │       └── Write command to buffer
    │   │
    │   └── Store new target: device[12428] = depthStencil
    │
    └── Return
```

### 18.3 sub_829CA240 (0x829CA240) - Render Target Setup

**Location:** `ppc_recomp.81.cpp:13909`

Configures one of 4 render target slots with surface parameters.

```
sub_829CA240(device, rtIndex, surface, offset, pitch, flags)
    │
    ├── r31 = device
    ├── r29 = rtIndex (0-3)
    ├── r30 = surface
    ├── r6 = offset into surface
    ├── r8 = flags (dirty mask)
    │
    ├── If surface != NULL:
    │   ├── Load surface dimensions:
    │   │   ├── r10 = surface[24] + offset (base address)
    │   │   └── r9 = surface[28] - offset (remaining size)
    │   │
    │   ├── Calculate packed address:
    │   │   ├── Extract 12-bit page offset
    │   │   ├── Add 512 alignment
    │   │   └── Combine with 29-bit base
    │   │
    │   ├── Store to device render target array:
    │   │   └── device[1780 + rtIndex*8] = packed_params
    │   │
    │   └── Update dirty flags: device[24] |= flags
    │
    ├── Calculate RT slot offset: (rtIndex + 3113) * 4
    │
    ├── If previous RT in slot != NULL:
    │   ├── Check deferred flag
    │   └── If dirty: Flush command to buffer
    │
    └── Store new RT: device[slot_offset] = surface
```

### 18.4 sub_829D1310 (0x829D1310) - Viewport Setup

**Location:** `ppc_recomp.81.cpp:31427`

Sets the rendering viewport (screen region to render to).

```
sub_829D1310(device, viewport)
    │
    ├── r30 = device
    ├── r29 = viewport (structure pointer)
    │
    ├── If viewport != NULL:
    │   └── Set dirty flag: device[16] |= 0x80000 (bit 19)
    │
    ├── r31 = device[12688] (current viewport)
    │
    ├── If current viewport != NULL:
    │   ├── Check deferred/dirty flags
    │   └── Flush viewport command if needed
    │
    ├── Store new viewport: device[12688] = viewport
    │
    ├── If viewport != NULL:
    │   ├── Load viewport bounds from (viewport + 872)
    │   │   ├── viewport[20] = X offset
    │   │   └── viewport[32] = Y offset
    │   │
    │   ├── Clear device[0] bits based on viewport flags
    │   │
    │   ├── Process viewport array (device + 1152)
    │   │   └── Copy viewport parameters
    │   │
    │   └── device[10942] &= 0x7F (clear high bit)
    │
    └── Return
```

### 18.5 sub_829D3728 (0x829D3728) - Texture Binding

**Location:** `ppc_recomp.81.cpp:36878`

Binds a texture to one of 19 sampler slots.

```
sub_829D3728(device, slot, texture, samplerMask)
    │
    ├── r31 = device
    ├── r4 = slot (0-18)
    ├── r5 = texture (or NULL to unbind)
    ├── r6 = samplerMask
    │
    ├── Calculate slot offset: (slot + 3134) * 4
    ├── r27 = device[slot_offset] (previous texture)
    │
    ├── If texture != NULL:
    │   ├── Extract texture parameters:
    │   │   ├── texture[28] = base address
    │   │   ├── texture[32] = format info
    │   │   ├── texture[36] = width
    │   │   ├── texture[40] = height
    │   │   ├── texture[44] = mip/filter info
    │   │   └── texture[48] = depth/array info
    │   │
    │   ├── Calculate sampler descriptor slot:
    │   │   └── desc_offset = (slot + 48) * 24
    │   │
    │   ├── Pack texture descriptor (24 bytes):
    │   │   ├── desc[0] = base_addr | format
    │   │   ├── desc[4] = pitch | flags
    │   │   ├── desc[8] = width
    │   │   ├── desc[12] = height | mip_count
    │   │   ├── desc[16] = filter_mode
    │   │   └── desc[20] = depth_info
    │   │
    │   ├── Compare min LOD with current:
    │   │   └── Use smaller value
    │   │
    │   └── Update dirty flags: device[24] |= samplerMask
    │
    ├── If previous texture != NULL:
    │   └── Flush unbind command if dirty
    │
    └── Store: device[slot_offset] = texture
```

### 18.6 sub_829D5388 (0x829D5388) - D3D Present (VdSwap)

**Location:** `ppc_recomp.81.cpp:41226`

Submits the frame to the display via VdSwap.

```
sub_829D5388(device, surface, flags)
    │
    ├── r31 = device
    ├── r28 = surface
    ├── r26 = flags
    │
    ├── Increment frame counter: device[16544]++
    │
    ├── sub_82990830(stack+128, surface+28, 24)  // Copy surface params
    │
    ├── Extract surface format from stack[132]:
    │   ├── If format == 50 → Convert to format 3
    │   └── If format == 7 → Convert to format 27
    │
    ├── Calculate display parameters:
    │   ├── r30 = device[13592] >> 17 (display width)
    │   ├── Load device[13596] for height
    │   └── Pack into submit structure
    │
    ├── Build swap descriptor:
    │   ├── Front buffer address
    │   ├── Back buffer address
    │   ├── Display dimensions
    │   └── Sync interval
    │
    ├── ══════════════════════════════
    │   VdSwap(swapDesc, ...)
    │   ══════════════════════════════
    │   └── @ address 0x829D55D4
    │
    ├── Update write pointer: device[48] = r30 + 256
    │
    ├── If flags != 0:
    │   └── sub_829DC778(device, buffer)  // Post-present cleanup
    │
    └── Return
```

---

## 19. Save System Execution Trace

### 19.1 sub_82122CA0 (0x82122CA0) - Save System Init

**Location:** `ppc_recomp.0.cpp:7117`

Initializes the save system with 3 save slot contexts.

```
sub_82122CA0 (Save System Init)
    │
    ├── r30 = Global save manager (0x81A32360)
    │
    ├── sub_8222D490(r30)              // Initialize save manager
    │
    ├── ═══════════════════════════════════════════
    │   SAVE SLOT 1 (Profile Save)
    ├── ═══════════════════════════════════════════
    │
    ├── sub_8218BE28(1392)             // Allocate 1392-byte context
    │   └── r31 = allocated pointer
    │
    ├── If allocation succeeded:
    │   ├── sub_822579B0()             // Init context structure
    │   │
    │   ├── Store vtable: r31[0] = 0x81209104 (ProfileSave vtable)
    │   │
    │   ├── Load float constants:
    │   │   ├── f6 = Global[-7984] (bounds max)
    │   │   └── f5 = Global[-7996] (bounds min)
    │   │
    │   ├── sub_8284EE18(r31+16, f5, f5, f5, f6, f6, f6)
    │   │   └── Set save bounds (AABB for save area)
    │   │
    │   └── r31[1376] = 1 (enabled flag)
    │
    ├── sub_823E8EC0(r30, 16)          // Get slot pointer
    │   └── Store r31 to slot
    │
    ├── sub_8222D300(r30)              // Finalize slot 1
    │
    ├── ═══════════════════════════════════════════
    │   SAVE SLOT 2 (Game Save)
    ├── ═══════════════════════════════════════════
    │
    ├── sub_8218BE28(1392)             // Allocate context
    │
    ├── If allocation succeeded:
    │   ├── sub_822579B0()             // Init structure
    │   │
    │   ├── Store vtable: r31[0] = 0x81209064 (GameSave vtable)
    │   │
    │   ├── Clear save data: VSPLTISW v0, 0
    │   │   └── Store 16 zero bytes at r31[1376]
    │   │
    │   └── No bounds init (zeroed)
    │
    ├── sub_823E8EC0(r30, 16) → Store r31
    ├── sub_8222D300(r30)
    │
    ├── ═══════════════════════════════════════════
    │   SAVE SLOT 3 (Autosave)
    ├── ═══════════════════════════════════════════
    │
    ├── sub_8218BE28(1392)             // Allocate context
    │
    ├── If allocation succeeded:
    │   ├── sub_822579B0()
    │   └── Store vtable: r31[0] = 0x81209028 (Autosave vtable)
    │
    ├── sub_823E8EC0(r30, 16) → Store r31
    └── sub_8222D300(r30)
```

### 19.2 Save Context Structure (1392 bytes)

```
Offset  Size    Field
------  ----    -----
0x000   4       vtable pointer
0x010   96      Bounds (sub_8284EE18 result - 6 floats min/max)
0x560   4       Save state flags
0x564   ?       Save data buffer
...
0x560   1       Enabled flag (slot 1 only)
```

### 19.3 Save Vtables

| Address | Type | Description |
|---------|------|-------------|
| `0x81209104` | Profile Save | Player profile, settings, stats |
| `0x81209064` | Game Save | Mission progress, checkpoints |
| `0x81209028` | Autosave | Automatic checkpoint saves |

### 19.4 Host-Side Save Implementation

**Current stubs in `kernel/xam.cpp`:**

```cpp
// Create save container - maps root name to filesystem path
uint32_t XamContentCreateEx(uint32_t dwUserIndex, const char* szRootName, 
    const XCONTENT_DATA* pContentData, uint32_t dwContentFlags, ...)
{
    // Maps szRootName (e.g., "GTA4Save") to host path
    // Creates directory structure for save files
}

// Enumerate existing saves
uint32_t XamContentCreateEnumerator(uint32_t dwUserIndex, uint32_t DeviceID,
    uint32_t dwContentType, ...)
{
    // Lists save files matching content type
    // Returns file names and metadata
}

// Close save container
uint32_t XamContentClose(const char* szRootName, XXOVERLAPPED* pOverlapped)
{
    gRootMap.erase(StringHash(szRootName));
    return 0;
}
```

### 19.5 Save Implementation Roadmap

**Phase 1: Basic Save/Load**
```cpp
// Hook sub_82122CA0 to inject host save paths
PPC_WEAK_FUNC(sub_82122CA0) {
    // Set up host-side save directory
    std::filesystem::path savePath = GetUserDataPath() / "saves";
    std::filesystem::create_directories(savePath);
    
    __imp__sub_82122CA0(ctx, base);
}

// Implement XamContentCreateEx to create save files
// Implement NtCreateFile/NtWriteFile for actual I/O
```

**Phase 2: Save Metadata**
- Parse XCONTENT_DATA structure
- Store thumbnail images
- Track save timestamps

**Phase 3: Cloud Sync (Optional)**
- Abstract save storage interface
- Support Steam Cloud / platform saves

---

## 20. Online/Multiplayer Detailed Trace

### 20.1 Network Initialization Chain

```
sub_827FFF80 (Network Init - called from sub_827D89B8)
    │
    ├── sub_82808718              // XNet startup
    │   ├── NetDll_WSAStartup     // Initialize Winsock
    │   └── sub_829C4390          // XNet init wrapper
    │       ├── XexGetModuleHandle("xnet.xex")
    │       ├── XexGetProcedureAddress(...)
    │       └── XNetStartup(params)
    │
    ├── sub_829C44A0              // Check connection status
    │   └── XNetGetTitleXnAddr(&xnaddr)
    │
    └── sub_829C4548              // Get title network address
        └── XNetXnAddrToInAddr(xnaddr, &inaddr)
```

### 20.2 Session Management Functions

| Address | Function | Role |
|---------|----------|------|
| `0x82973EE0` | `sub_82973EE0` | Session create/join |
| `0x82973FA8` | `sub_82973FA8` | Session state checker |
| `0x82973F50` | `sub_82973F50` | Session validation |
| `0x829733B0` | `sub_829733B0` | MP mode handler |
| `0x82973460` | `sub_82973460` | MP state handler |

### 20.3 Socket API Mapping

**Guest-to-Host Socket Wrappers:**

```
Guest Address    Host Function         Notes
-------------    -------------         -----
0x829C3510       NetDll_socket         Create socket
0x829C3588       NetDll_closesocket    Close socket
0x829C35E8       NetDll_shutdown       Shutdown socket
0x829C3660       NetDll_setsockopt     Set socket options
0x829C3750       NetDll_bind           Bind to address
0x829C37C0       NetDll_connect        Connect to peer
0x829C3830       NetDll_listen         Listen for connections
0x829C38A0       NetDll_accept         Accept connection
0x829C3950       NetDll_select         I/O multiplexing
0x829C39F8       NetDll_recv           Receive data
0x829C3A88       NetDll_recvfrom       Receive with address
0x829C3B48       NetDll_send           Send data
0x829C3BD8       NetDll_sendto         Send to address
```

### 20.4 XNet Functions for Xbox Live

```
Guest Address    Function                    Purpose
-------------    --------                    -------
0x829C4390       XNetStartup wrapper         Initialize Xbox Live networking
0x829C4458       XNetCleanup wrapper         Shutdown Xbox Live networking
0x829C44A0       XNetGetConnectStatus        Check Xbox Live connection
0x829C4548       XNetGetTitleXnAddr          Get secure title address
0x829C45A0       XNetXnAddrToInAddr          Convert XNet to IP address
0x829C4610       XNetInAddrToXnAddr          Convert IP to XNet address
0x829C4680       XNetUnregisterInAddr        Unregister IP mapping
0x829C46E8       XNetQosListen               Start QoS listener
0x829C4760       XNetQosLookup               Lookup QoS data
0x829C47D8       XNetQosRelease              Release QoS handle
```

### 20.5 Multiplayer Implementation Phases

**Phase 1: LAN Play**
```cpp
// Stub XNetStartup to succeed
uint32_t XNetStartup(void* params) {
    return 0; // Success
}

// Return local IP for XNetGetTitleXnAddr
uint32_t XNetGetTitleXnAddr(XNADDR* pxna) {
    // Fill with local network address
    pxna->ina.s_addr = GetLocalIPv4();
    return XNET_CONNECT_STATUS_CONNECTED;
}

// Implement UDP broadcast for LAN discovery
// Hook sub_82973EE0 to use host networking
```

**Phase 2: Online Infrastructure**
```cpp
// Implement matchmaking server client
// Replace XNet with custom relay protocol
// Handle NAT traversal via STUN/TURN
```

**Phase 3: Full Xbox Live Replacement**
```cpp
// Session management
// Voice chat (via Opus codec)
// Leaderboards
// Achievements sync
```

---

## 21. Online/Achievement/Leaderboard Deep Execution Traces

### 21.1 sub_8219ADF0 (0x8219ADF0) - Online System Init

**Location:** `ppc_recomp.3.cpp:38076`

This function initializes the online system infrastructure, including matchmaking state and network callbacks.

```
sub_8219ADF0 (Online System Init)
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 1: Network Session Manager Init
    ├── ═══════════════════════════════════════════
    │
    ├── sub_823D81D8()                    // Session config parser
    │   ├── Clear stack buffer (80, 84, 88 = 0)
    │   │
    │   ├── sub_82192648("sessionConfig")     // Get config path
    │   ├── sub_82192840(configPtr, path)     // Load config file
    │   │
    │   ├── sub_82192648("networkSettings")   // Get network settings path
    │   ├── sub_82192980(configPtr, 1)        // Parse config
    │   │
    │   ├── If parse succeeded:
    │   │   ├── Loop 228 config entries:
    │   │   │   ├── Check char 0 for '#' (comment) or NULL
    │   │   │   ├── If valid:
    │   │   │   │   ├── sub_8298EFE0(...)      // Format string
    │   │   │   │   ├── sub_82850B28()         // Validate entry
    │   │   │   │   ├── sub_8298F040(...)      // String compare
    │   │   │   │   └── Store parsed values
    │   │   │   └── Continue
    │   │   └── Return
    │   │
    │   └── Return (no config)
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 2: Online State Init
    ├── ═══════════════════════════════════════════
    │
    ├── Global[0x8134D0A8] = -1           // Session ID (invalid)
    ├── Global[0x8134D1B8] = 0            // Connection state (disconnected)
    │
    ├── sub_82197C78()                    // Reset matchmaking state
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 3: Callback Registration
    ├── ═══════════════════════════════════════════
    │
    ├── r31 = 0x81201638 (callback table)
    ├── r30 = 0x81331CF0 (handler list)
    │
    ├── Global[0x8134D0A0].enabled = 1    // Enable online system
    ├── Global[0x8134D0A0].state = 0      // Initial state
    │
    ├── sub_82319040(r30, r31, 0, 0, 512, 512)  // Create callback queue
    │   └── Args: handler, callbackTable, 0, 0, queueSize, queueSize
    │
    ├── Global[0x8134D0A4] = result       // Store callback handle
    │
    ├── r30 = Global[0x81325B70]          // Event dispatcher
    │
    ├── sub_82318F68(r30)                 // Get dispatcher interface
    │
    ├── sub_828552C0(dispatcher, callbackTable, interface)
    │   └── Register network event handlers
    │
    ├── sub_8219A428()                    // Final online init
    │
    └── Return
```

**Key Globals:**
| Address | Purpose |
|---------|---------|
| `0x8134D0A0` | Online system state struct |
| `0x8134D0A4` | Callback queue handle |
| `0x8134D0A8` | Current session ID |
| `0x8134D1B8` | Connection state |
| `0x81201638` | Callback vtable |
| `0x81331CF0` | Network handler list |

---

### 21.2 sub_8212EDC8 (0x8212EDC8) - Achievement Tracking Init

**Location:** `ppc_recomp.0.cpp:35882`

This is a **massive** function that initializes the entire achievement tracking system, creating 50+ achievement trackers.

```
sub_8212EDC8 (Achievement Tracking Init)
    │
    ├── r31 = 0x8134D0E0 (achievement manager base)
    ├── r30 = 0x8134D018 (tracker table base)
    ├── r27 = 3 (default achievement type)
    ├── r28 = 1 (enabled flag)
    ├── r29 = 0 (disabled flag)
    │
    ├── ═══════════════════════════════════════════
    │   ACHIEVEMENT TRACKER ALLOCATION (50+ trackers)
    │   Each tracker = 508 bytes via sub_82199658
    ├── ═══════════════════════════════════════════
    │
    ├── ──────────────────────────────────────
    │   Achievement 1: "ACHV_001" (Story Progress)
    ├── ──────────────────────────────────────
    ├── sub_82199658("ACHV_001", 5, &callback1, &data1, 1, cfg1, cfg2)
    │   ├── sub_8218BE28(508)             // Allocate 508-byte tracker
    │   ├── sub_821994D8(...)             // Initialize tracker struct
    │   └── Store in tracker table: table[count++] = tracker
    │
    ├── Store to r31[812]: achievement slot ID
    ├── Set type: tracker[52] = 3 (TYPE_PROGRESS)
    ├── Set flags: tracker[56] = 0, tracker[90] = 0, tracker[4] = 1
    │
    ├── ──────────────────────────────────────
    │   Achievement 2: "ACHV_002" (Missions Complete)
    ├── ──────────────────────────────────────
    ├── sub_82199658("ACHV_002", 5, &callback2, &data2, 1, cfg1, cfg2)
    ├── Store to r31[840], configure tracker...
    │
    ├── ──────────────────────────────────────
    │   Achievement 3: "ACHV_003" (Side Missions)
    ├── ──────────────────────────────────────
    ├── sub_82199658("ACHV_003", 5, ...)
    │
    ├── ──────────────────────────────────────
    │   Achievement 4: "ACHV_004" (Collectibles)
    ├── ──────────────────────────────────────
    ├── sub_82199658("ACHV_004", 5, ...)
    │
    ├── ──────────────────────────────────────
    │   Achievement 5: "ACHV_005" (Player Stats)
    ├── ──────────────────────────────────────
    ├── sub_82199658("ACHV_005", 5, ...)
    │
    ├── ──────────────────────────────────────
    │   Achievement 6: "ACHV_006" (Multiplayer)
    ├── ──────────────────────────────────────
    ├── sub_82199658("ACHV_006", 5, ...)
    │   └── Stores callback for thread context @ offset 576+24
    │
    ├── sub_8298F240(buffer, formatStr, ...)  // Format achievement name
    ├── sub_821DA6E8(threadCtx, buffer)       // Register with thread
    ├── sub_82197C10(tracker, threadOffset)   // Link callback
    │
    ├── ──────────────────────────────────────
    │   Achievements 7-15: Various Progress Types
    ├── ──────────────────────────────────────
    ├── [9 more trackers with type=3 or type=5]
    │
    ├── ──────────────────────────────────────
    │   Achievements 16-17: Zero-Progress Type
    ├── ──────────────────────────────────────
    ├── sub_82199658(..., 12, ...)  // type=12 (cumulative)
    ├── tracker[52] = 0 (TYPE_NONE - auto-unlock)
    │
    ├── ──────────────────────────────────────
    │   Achievement 18: Linked Stats
    ├── ──────────────────────────────────────
    ├── sub_82199658(..., 2, ...)   // type=2 (count-based)
    ├── r27 = 0x81328B28 (stats base)
    ├── sub_82197C18(tracker, stats+28)  // Link to stat counter
    │
    ├── ──────────────────────────────────────
    │   Achievements 19-30: Stat-Linked
    ├── ──────────────────────────────────────
    ├── [Multiple trackers linked to various stat offsets]
    ├── tracker[20] = 7 (secondary type flag)
    │
    ├── ──────────────────────────────────────
    │   Achievements 31-48: Loop-Generated (MP Awards)
    ├── ──────────────────────────────────────
    │
    ├── Loop (r24 = 50 to 56, step 2):  // 4 trackers
    │   ├── sub_8298F240(buffer, "ACHV_%d", r24-49)
    │   ├── sub_82199658(buffer, 5, ...)
    │   ├── tracker[52] = 0
    │   └── tracker[4] = 1 (enabled)
    │
    ├── Loop (r26 = 51 to 57, step 2):  // 4 more trackers
    │   ├── sub_8298F240(buffer, "ACHV_%d", r26-50)
    │   ├── sub_82199658(buffer, 9, ...)  // type=9 (MP specific)
    │   └── tracker[4] = 1
    │
    ├── ──────────────────────────────────────
    │   Final Achievements: 49-50+ (Special)
    ├── ──────────────────────────────────────
    ├── sub_82199658("ACHV_FINAL", 3, ...)
    │   └── tracker[20] = 6, tracker[96] = 4
    │
    ├── sub_82199658("ACHV_SECRET", 2, ...)
    │   └── Link to stats+20, tracker[20] = 1
    │
    ├── sub_82199658("ACHV_HIDDEN", 2, ...)
    │   └── Link to stats+24, tracker[20] = 1
    │
    └── Return
```

**Tracker Structure (508 bytes):**
```
Offset  Size    Field
------  ----    -----
0x000   4       State flags
0x004   1       Enabled (1=yes)
0x014   4       Secondary type
0x034   4       Type (0=auto, 2=count, 3=progress, 5=stat, 9=MP, 12=cumulative)
0x038   1       Lock state
0x05A   1       Completion flag
0x060   4       Progress value
0x0C0   ?       Callback data
```

**Achievement Types:**
| Type | ID | Description |
|------|-----|-------------|
| 0 | TYPE_NONE | Auto-unlock (no tracking) |
| 2 | TYPE_COUNT | Count-based (X kills, etc.) |
| 3 | TYPE_PROGRESS | Story/mission progress |
| 5 | TYPE_STAT | Linked to stat counter |
| 9 | TYPE_MULTIPLAYER | MP-specific achievement |
| 12 | TYPE_CUMULATIVE | Cumulative across sessions |

---

### 21.3 sub_8212F578 (0x8212F578) - Leaderboard Init

**Location:** `ppc_recomp.0.cpp:36919`

Initializes the leaderboard system with 27 leaderboard categories.

```
sub_8212F578 (Leaderboard Init)
    │
    ├── r30 = 0x812696B8 (leaderboard string table)
    │
    ├── sub_82318F60(r30)                 // Init string table
    │
    ├── ═══════════════════════════════════════════
    │   GET PRIMARY USER ID
    ├── ═══════════════════════════════════════════
    │
    ├── sub_82125040()                    // Get signed-in user ID
    ├── r31 = result (user ID or -1)
    │
    ├── If r31 == -1:
    │   ├── sub_82124EF0(r30)             // Get default user
    │   └── r31 = result
    │
    ├── ═══════════════════════════════════════════
    │   REGISTER USER FOR LEADERBOARDS
    ├── ═══════════════════════════════════════════
    │
    ├── sub_82205438(userId, "LeaderboardUser")
    │   └── Register user with Xbox Live services
    │
    ├── Check user state:
    │   ├── r11 = Global[0x81338830]      // Leaderboard config
    │   ├── r10 = r11[4] (user state table)
    │   ├── Load state for userId
    │   └── If signed in: use live data; else: use local
    │
    ├── ═══════════════════════════════════════════
    │   INCREMENT LEADERBOARD ACCESS COUNT
    ├── ═══════════════════════════════════════════
    │
    ├── If user valid:
    │   ├── r11 = Global[0x81338830]
    │   ├── r9 = r11[12] (entry size)
    │   ├── r10 = r11[0] (data base)
    │   ├── entry = r10 + r9 * userId
    │   └── entry[4]++ (access count)
    │
    ├── ═══════════════════════════════════════════
    │   SYNC LEADERBOARD STATE
    ├── ═══════════════════════════════════════════
    │
    ├── sub_82204770()                    // Begin sync transaction
    │
    ├── sub_82204E58(userId)              // Sync user leaderboards
    │
    ├── ═══════════════════════════════════════════
    │   INIT 27 LEADERBOARD CATEGORIES
    ├── ═══════════════════════════════════════════
    │
    ├── r30 = 0x81336618 (category vtable array)
    ├── r29 = 0x81328B28 (stats base)
    │
    ├── Loop r31 = 0 to 108 (step 4, 27 iterations):
    │   │
    │   ├── r3 = r29 + r31 (stats offset)
    │   ├── r4 = r30[r31] (category vtable)
    │   │
    │   ├── sub_821EC0C8(statsPtr, categoryVtable)
    │   │   └── Link stat to leaderboard category
    │   │
    │   └── Continue
    │
    ├── sub_82204EE0()                    // End sync transaction
    │
    └── Return
```

**Leaderboard Categories (27 total):**
| Index | Offset | Category |
|-------|--------|----------|
| 0 | +0 | Total Score |
| 1 | +4 | Missions Complete |
| 2 | +8 | Play Time |
| 3 | +12 | Distance Traveled |
| 4 | +16 | Vehicles Destroyed |
| 5 | +20 | Headshots |
| 6 | +24 | Accuracy |
| ... | ... | ... |
| 26 | +104 | Multiplayer Wins |

**Key Globals:**
| Address | Purpose |
|---------|---------|
| `0x812696B8` | Leaderboard string table |
| `0x81328B28` | Stats base pointer |
| `0x81336618` | Category vtable array |
| `0x81338830` | Leaderboard config struct |

---

### 21.4 sub_82199658 (0x82199658) - Achievement Tracker Factory

**Location:** `ppc_recomp.3.cpp:34522`

This helper function allocates and initializes individual achievement trackers.

```
sub_82199658 (Achievement Tracker Factory)
    │
    ├── Args:
    │   ├── r4 = achievement name string
    │   ├── r5 = callback function 1
    │   ├── r6 = callback function 2
    │   ├── r7 = enabled flag
    │   ├── r8 = config param 1
    │   └── r9 = config param 2
    │
    ├── sub_8218BE28(508)                 // Allocate 508 bytes
    │
    ├── If allocation failed:
    │   └── Return 0
    │
    ├── sub_821994D8(buffer, name, cb1, cb2, enabled, cfg1, cfg2)
    │   └── Initialize tracker structure
    │
    ├── r9 = result (tracker pointer)
    │
    ├── ═══════════════════════════════════════════
    │   REGISTER IN GLOBAL TRACKER TABLE
    ├── ═══════════════════════════════════════════
    │
    ├── r8 = 0x8134D018 (tracker table base)
    ├── r11 = Global[0x8134D1B8] (tracker count)
    ├── index = r11 * 4
    │
    ├── table[index] = r9 (store tracker)
    ├── Global[0x8134D1B8] = r11 + 1 (increment count)
    │
    └── Return (r11 - 1) (slot index)
```

**Tracker Table:**
- Base address: `0x8134D018`
- Count stored at: `0x8134D1B8`
- Each entry: 4 bytes (pointer to 508-byte tracker)

---

### 21.5 Implementation Hooks for Achievements/Leaderboards

**Achievement Unlock Hook:**
```cpp
// Hook sub_82199658 to track achievement registrations
PPC_WEAK_FUNC(sub_82199658) {
    const char* name = (const char*)(base + ctx.r4.u32);
    LOGF_IMPL(Achievements, "Tracking", "Registered: %s", name);
    __imp__sub_82199658(ctx, base);
}

// Hook achievement state changes
// Monitor tracker[0x38] (lock state) and tracker[0x5A] (completion)
```

**Leaderboard Sync Hook:**
```cpp
// Hook sub_82204E58 to capture leaderboard updates
PPC_WEAK_FUNC(sub_82204E58) {
    uint32_t userId = ctx.r3.u32;
    LOGF_IMPL(Leaderboard, "Sync", "User %d syncing leaderboards", userId);
    __imp__sub_82204E58(ctx, base);
}
```

**Stats Integration:**
```cpp
// Stats base at 0x81328B28
// 27 leaderboard stats at 4-byte intervals
// Hook sub_821EC0C8 to track stat updates
```

---

## 22. Xbox 360 Hardware-Tied Functions (REWRITE REQUIRED)

These functions are **critical** for the recompilation project as they contain Xbox 360 hardware-specific code that must be rewritten for PC/cross-platform support.

### 22.1 Game Init Wrapper Overview (sub_827D89B8)

This is the central orchestrator for game initialization and shutdown. All sub-functions listed here interact with Xbox 360 hardware/OS APIs.

```
sub_827D89B8 (Game Init Wrapper) - FULL CALL TREE
    │
    ├── sub_827D8840              // ★ Pre-init setup (cmdline parsing)
    ├── sub_827FFF80              // ★ Network init (XNet)
    │       └── sub_82808718      //   XNetStartup + WSAStartup
    ├── sub_827EEDE0              // Store argc/argv to globals
    ├── sub_828E0AB8              // Frame tick (timing)
    ├── sub_827EE620              // ★ Thread event creation
    ├── [vtable+52 call]          // Engine->Init()
    ├── sub_8218BEB0              // ★ ACTUAL GAME MAIN
    │       └── sub_82120000      // ★ Game subsystem init
    │       └── sub_821200D0      // ★ Post-init (profiles, saves)
    │       └── sub_821200A8      // Finalize init
    ├── [vtable+56 call]          // Engine->Cleanup()
    ├── sub_827EECE8              // ★ Event/thread cleanup
    └── sub_827FFF88              // ★ Network cleanup (XNet)
            └── sub_828087B0      //   WSACleanup + XNetCleanup
```

---

### 22.2 sub_827D8840 (0x827D8840) - Command Line Parser

**Location:** `ppc_recomp.62.cpp:20159`
**Xbox 360 Dependency:** Low (mostly portable)

Parses the command line string into argc/argv format.

```
sub_827D8840 (Command Line Parser)
    │
    ├── sub_829A27D8()                    // Get raw command line
    │   └── Returns: Global[0x81200658]
    │
    ├── If NULL → return early
    │
    ├── Store argv base:
    │   └── Global[0x81325008] = 0x812649A8
    │
    ├── Parse loop:
    │   ├── Skip whitespace (space=32, tab=9)
    │   │
    │   ├── Handle quoted strings (char 34 = ")
    │   │   ├── Advance past opening quote
    │   │   ├── Find closing quote or NULL
    │   │   └── Extract token
    │   │
    │   ├── Handle unquoted args:
    │   │   └── Find next whitespace or NULL
    │   │
    │   ├── Store pointer: argv[argc] = token
    │   ├── argc++
    │   ├── NULL-terminate token
    │   └── Continue
    │
    └── Store NULL terminator: argv[argc] = NULL
```

**Rewrite Notes:**
- ✅ Mostly portable, uses standard string parsing
- ⚠️ Replace `sub_829A27D8` with native command line getter
- Implementation: Use `GetCommandLineW()` on Windows, `main(argc, argv)` elsewhere

---

### 22.3 sub_827FFF80/sub_82808718 (0x82808718) - Network Initialization

**Location:** `ppc_recomp.64.cpp:51139`
**Xbox 360 Dependency:** **HIGH** - XNet APIs

```
sub_82808718 (Network Init - XNet)
    │
    ├── Increment init counter: Global[0x81327470]++
    │
    ├── If first call (counter was 0):
    │   │
    │   ├── Clear XNetStartupParams struct (13 bytes):
    │   │   ├── stack[80-92] = 0
    │   │   └── stack[80] = 13 (cfgSizeOfStruct)
    │   │   └── stack[81] = 1  (cfgFlags)
    │   │
    │   ├── ★ sub_829C4448(&params)       // XNetStartup()
    │   │   └── Xbox 360 XNet initialization
    │   │
    │   ├── ★ sub_829C4090(2, &wsadata)   // WSAStartup(MAKEWORD(2,0))
    │   │   └── Windows Sockets init
    │   │
    │   ├── Check WSA version:
    │   │   ├── major = wsadata[96] & 0xFF
    │   │   ├── minor = (wsadata[96] >> 8) & 0xFF
    │   │   │
    │   │   ├── If major != 2 || minor == 0:
    │   │   │   └── ★ sub_829C40A0()      // WSACleanup()
    │   │   │
    │   │   └── Continue
    │   │
    │   └── Return
    │
    └── Return (already initialized)
```

**Xbox 360 API Calls:**
| PPC Address | Xbox API | Purpose |
|-------------|----------|---------|
| `sub_829C4448` | `XNetStartup()` | Initialize Xbox networking stack |
| `sub_829C4090` | `WSAStartup()` | Windows Sockets (also on Xbox) |
| `sub_829C40A0` | `WSACleanup()` | Cleanup sockets |

**Rewrite Implementation:**
```cpp
// Replace XNetStartup with native networking
void NetworkInit() {
    #ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    #else
    // Unix: no init needed, sockets work directly
    #endif
    
    // Initialize ENet or custom networking layer for multiplayer
    enet_initialize();
}
```

---

### 22.4 sub_827EE620 (0x827EE620) - Thread Event Setup

**Location:** `ppc_recomp.63.cpp:31374`
**Xbox 360 Dependency:** **HIGH** - Kernel Events

Creates thread synchronization events in a loop.

```
sub_827EE620 (Thread Event Setup)
    │
    ├── r30 = 0x81302170 (event array base)
    ├── r31 = r30 + 156 (first event slot)
    ├── end = r30 + 65536 - 24420 = 0x81311F9C
    │
    ├── Loop (256 iterations, step 160):
    │   │
    │   ├── ★ sub_829A23C8(0, 0, 0, 0)    // KeInitializeEvent/NtCreateEvent
    │   │   └── Creates kernel event object
    │   │
    │   ├── Store handle: [r31] = result
    │   │
    │   ├── r31 += 160 (next slot)
    │   │
    │   └── Continue while r31 < end
    │
    └── Return
```

**Event Array Structure:**
```
Base: 0x81302170
Slot size: 160 bytes
Count: ~256 events
Event handle offset: +0
```

**Xbox 360 API Calls:**
| PPC Address | Xbox API | Purpose |
|-------------|----------|---------|
| `sub_829A23C8` | `KeInitializeEvent()` / `NtCreateEvent()` | Create sync event |

**Rewrite Implementation:**
```cpp
// Replace Xbox events with cross-platform primitives
#ifdef _WIN32
    HANDLE events[256];
    for (int i = 0; i < 256; i++) {
        events[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
    }
#else
    pthread_cond_t events[256];
    pthread_mutex_t mutexes[256];
    for (int i = 0; i < 256; i++) {
        pthread_cond_init(&events[i], NULL);
        pthread_mutex_init(&mutexes[i], NULL);
    }
#endif
```

---

### 22.5 sub_8218C600 (0x8218C600) - Core Engine Init

**Location:** `ppc_recomp.3.cpp:2250`
**Xbox 360 Dependency:** **CRITICAL** - Multiple Xbox APIs

This is the core engine initialization function with many Xbox 360 hardware dependencies.

```
sub_8218C600 (Core Engine Init)
    │
    ├── ★ sub_829A0A48(-2, 2)             // XSetThreadProcessor
    │   └── Set thread CPU affinity
    │
    ├── Global[0x813257AA] = 1            // Init flag
    ├── Global[0x81334044] = -1           // State
    ├── Global[0x813357A4] = -1           // State
    ├── Global[0x81337764] = -1           // State
    │
    ├── Global[0x8132504C] = 0x81835054   // Vtable setup
    │
    ├── ★ sub_827DF248(383, 2, 0)         // D3D device creation
    │   └── Initialize graphics device
    │
    ├── Thread context setup:
    │   ├── r11 = [r13+0] (TLS base)
    │   ├── Global[0x81330A20] = 64       // Buffer size
    │   ├── [r11+8] |= 6                  // Thread flags
    │   └── [r11+12] = 1                  // State
    │
    ├── sub_82192578()                    // Additional init
    │
    ├── String table setup:
    │   ├── Global[0x81326CC4] = "DefaultString"
    │   └── Global[0x81326CD4] = "DefaultString"
    │
    ├── Path table init (if NULL):
    │   ├── Global[0x81326CC4+4] = 0x812009E0
    │   └── Global[0x81326D24+4] = 0x812009DC
    │
    ├── ★ sub_82850AF0()                  // GPU init check
    │   └── Verify GPU state
    │
    ├── sub_82850B60(0)                   // GPU mode
    │
    ├── Check extended settings:
    │   └── Global[0x813257F8+4]
    │
    ├── Allocate engine context (472 bytes):
    │   ├── sub_8218BE28(472)
    │   │
    │   ├── If success:
    │   │   ├── ★ sub_82857028()          // Initialize D3D context
    │   │   ├── Store vtable: [r29] = 0x81200970
    │   │   └── Global[0x8133826C] = context
    │   │
    │   └── If fail: Global[0x8133826C] = 0
    │
    └── [Continues with more subsystem init...]
```

**Xbox 360 API Calls:**
| PPC Address | Xbox API | Purpose |
|-------------|----------|---------|
| `sub_829A0A48` | `XSetThreadProcessor()` | CPU core affinity |
| `sub_827DF248` | `D3DDevice::Create()` | Direct3D 9 device |
| `sub_82850AF0` | GPU validation | Xenos GPU check |
| `sub_82857028` | `D3DContext::Init()` | D3D context |

**Rewrite Priority:** 🔴 **CRITICAL**

---

### 22.6 sub_82120000 (0x82120000) - Game Subsystem Init

**Location:** `ppc_recomp.0.cpp:3`
**Xbox 360 Dependency:** **HIGH**

Main game initialization - creates game manager and subsystems.

```
sub_82120000 (Game Subsystem Init)
    │
    ├── ★ sub_8218C600("game:\\default.xex")  // Engine init
    │   └── Pass XEX path for config
    │
    ├── If init failed → return 0
    │
    ├── sub_82120EE8()                    // Subsystem allocation
    │   ├── Allocate game manager (944 bytes)
    │   │   ├── sub_8218BE28(944)
    │   │   ├── sub_821207B0()            // Init manager
    │   │   └── Global[0x81328B34] = manager
    │   │
    │   ├── Check physics engine:
    │   │   └── If NULL: sub_82673718()   // Init physics
    │   │
    │   ├── Allocate world context (352 bytes):
    │   │   ├── sub_8218BE28(352)
    │   │   ├── Init fields: [324]=0, [328]=0, [330]=0
    │   │   ├── sub_8296BE18(+336)        // Physics world
    │   │   └── Global[0x81328B38] = world
    │   │
    │   ├── sub_82269098()                // Resource init
    │   ├── sub_822054F8()                // Asset init
    │   ├── sub_821DE390()                // Script init
    │   ├── sub_8221F8A8()                // Audio init
    │   │
    │   └── sub_82273988(0x81323100, 1)   // Final setup
    │
    ├── r31 = Global[0x81338830]          // Game config
    │
    ├── Init config fields:
    │   ├── [r31+0] = 0
    │   ├── [r31+4] = 0
    │   ├── [r31+8] = string table result
    │   └── [r31+12] = -1
    │
    ├── ★ sub_82124080(1, 0)              // Profile/save init
    │
    ├── sub_82120FB8()                    // Finalize
    │
    └── Return 1 (success)
```

**Key Allocations:**
| Size | Purpose | Global Storage |
|------|---------|----------------|
| 944 bytes | Game Manager | `0x81328B34` |
| 352 bytes | World Context | `0x81328B38` |

---

### 22.7 sub_821200D0 (0x821200D0) - Post-Init (Profiles/Saves)

**Location:** `ppc_recomp.0.cpp:134`
**Xbox 360 Dependency:** **HIGH** - XContent APIs

```
sub_821200D0 (Post-Init / Profile Loading)
    │
    ├── ★ sub_82124490()                  // Check profile state
    │   └── Returns: profile ready flag
    │
    ├── While profile not ready:
    │   ├── ★ sub_827DAE18(1)             // Frame update + wait
    │   │   └── Process pending I/O, wait for profile
    │   │
    │   └── sub_82124490() again
    │
    ├── sub_82121E80()                    // Load user data
    │
    ├── r30 = 0x81327658 (profile base)
    │
    ├── If profile[1375] != 0 (profile loaded):
    │   │
    │   ├── r31 = 0x81333DE8 (save context)
    │   │
    │   ├── ★ sub_82193CB0(r31)           // XContentCreate enumerate
    │   │   └── Enumerate save files
    │   │
    │   ├── ★ sub_82192E00(r31)           // XContentOpen
    │   │   └── Open save content
    │   │
    │   ├── sub_82318F60("SaveData")      // Get save path
    │   │
    │   ├── ★ sub_82125040()              // Get signed-in user
    │   │
    │   ├── ★ sub_82124FB0()              // Validate save signature
    │   │
    │   ├── Clear profile state:
    │   │   ├── [r30+0] = 0
    │   │   └── [r30+1375] = 0
    │   │
    │   └── Continue
    │
    ├── sub_821924D8()                    // Config apply
    │
    ├── sub_8218C2C0()                    // Settings apply
    │
    └── Return
```

**Xbox 360 API Calls:**
| PPC Address | Xbox API | Purpose |
|-------------|----------|---------|
| `sub_82193CB0` | `XContentCreateEnumerator()` | Enumerate saves |
| `sub_82192E00` | `XContentCreate()` | Open save content |
| `sub_82125040` | `XUserGetSigninState()` | Get user signin |
| `sub_82124FB0` | Save signature validation | Xbox Live save protection |

---

### 22.8 sub_827EECE8 (0x827EECE8) - Thread/Event Cleanup

**Location:** `ppc_recomp.63.cpp:32385`
**Xbox 360 Dependency:** **HIGH** - Kernel APIs

```
sub_827EECE8 (Thread/Event Cleanup)
    │
    ├── r30 = 0x81302170 (event array base)
    ├── r31 = r30 + 156 (first event slot)
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 1: Close all events (256 iterations)
    ├── ═══════════════════════════════════════════
    │
    ├── Loop while r31 < end:
    │   │
    │   ├── r3 = [r31] (event handle)
    │   │
    │   ├── ★ sub_829A1958(r3)            // NtClose / KeCloseEvent
    │   │   └── Close kernel event
    │   │
    │   ├── r31 += 160
    │   │
    │   └── Continue
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 2: Cleanup thread pool
    ├── ═══════════════════════════════════════════
    │
    ├── r30 = 0x81302150 (thread pool base)
    │
    ├── While pool[32] > 0 (thread count):
    │   │
    │   ├── index = pool[32] - 1
    │   ├── r31 = pool[index * 4] (thread handle)
    │   │
    │   ├── If r31 != NULL:
    │   │   ├── sub_827EEA08(r31)         // Signal thread exit
    │   │   └── ★ sub_8218BE78(r31)       // Free thread memory
    │   │
    │   ├── pool[32]--
    │   │
    │   └── Continue
    │
    └── Return
```

**Xbox 360 API Calls:**
| PPC Address | Xbox API | Purpose |
|-------------|----------|---------|
| `sub_829A1958` | `NtClose()` | Close kernel handle |
| `sub_8218BE78` | Memory free | Deallocate thread |

---

### 22.9 sub_828087B0 (0x828087B0) - Network Cleanup

**Location:** `ppc_recomp.64.cpp:51229`
**Xbox 360 Dependency:** **HIGH** - XNet APIs

```
sub_828087B0 (Network Cleanup)
    │
    ├── Decrement init counter: Global[0x81327470]--
    │
    ├── If counter == 0 (last user):
    │   │
    │   ├── ★ sub_829C40A0()              // WSACleanup()
    │   │   └── Cleanup Windows Sockets
    │   │
    │   └── ★ sub_829C4458()              // XNetCleanup()
    │       └── Cleanup Xbox networking
    │
    └── Return
```

**Xbox 360 API Calls:**
| PPC Address | Xbox API | Purpose |
|-------------|----------|---------|
| `sub_829C40A0` | `WSACleanup()` | Socket cleanup |
| `sub_829C4458` | `XNetCleanup()` | XNet cleanup |

---

### 22.10 Rewrite Priority Matrix

| Function | Xbox Dependency | Rewrite Priority | Complexity |
|----------|-----------------|------------------|------------|
| `sub_827D8840` | Low | 🟢 Easy | Simple string parsing |
| `sub_82808718` | HIGH | 🟡 Medium | Replace XNet with ENet/native |
| `sub_827EE620` | HIGH | 🟡 Medium | Replace with pthreads/Win32 |
| `sub_8218C600` | CRITICAL | 🔴 Hard | D3D9→Vulkan/D3D12, threading |
| `sub_82120000` | HIGH | 🟡 Medium | Allocations mostly portable |
| `sub_821200D0` | HIGH | 🔴 Hard | XContent→native file I/O |
| `sub_827EECE8` | HIGH | 🟡 Medium | Replace kernel events |
| `sub_828087B0` | HIGH | 🟢 Easy | Standard socket cleanup |

---

### 22.11 Rewrite Strategy

**Phase 1: Threading & Events**
```cpp
// Create cross-platform event abstraction
class GameEvent {
    #ifdef _WIN32
    HANDLE m_handle;
    #else
    pthread_cond_t m_cond;
    pthread_mutex_t m_mutex;
    bool m_signaled;
    #endif
public:
    void Signal();
    void Wait();
    void Reset();
};
```

**Phase 2: Networking**
```cpp
// Replace XNet with ENet for multiplayer
#include <enet/enet.h>

void NetworkInit() {
    enet_initialize();
    // Create host for LAN/online play
}

void NetworkCleanup() {
    enet_deinitialize();
}
```

**Phase 3: Save Data**
```cpp
// Replace XContent with native file I/O
class SaveManager {
    std::filesystem::path GetSavePath(int slot);
    bool LoadSave(int slot, SaveData& data);
    bool WriteSave(int slot, const SaveData& data);
    // Use JSON or binary format, no Xbox signature
};
```

**Phase 4: Graphics**
```cpp
// Already handled by video.cpp via RT64
// D3D9 calls are translated to Vulkan/D3D12
```

---

## 23. Game Init (sub_82120000) Deep Execution Traces

These traces document the complete game initialization sequence - **critical for understanding Xbox 360 dependencies**.

### 23.1 sub_8218C600 (0x8218C600) - Core Engine Initialization

**Location:** `ppc_recomp.3.cpp:2250`
**Stack Frame:** 128 bytes
**Xbox 360 Dependency:** **CRITICAL**

This is the master engine initialization function - sets up D3D, GPU, threading, and all core systems.

```
sub_8218C600 (Core Engine Init) - COMPLETE TRACE
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 1: THREAD PROCESSOR AFFINITY
    ├── ═══════════════════════════════════════════
    │
    ├── Args: r3 = XEX path string ("game:\\default.xex")
    ├── r28 = r3 (save path)
    │
    ├── ★ sub_829A0A48(-2, 2)             // XSetThreadProcessor
    │   ├── r3 = -2 (current thread)
    │   └── r4 = 2 (processor mask)
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 2: GLOBAL STATE INITIALIZATION
    ├── ═══════════════════════════════════════════
    │
    ├── r30 = 1 (success flag)
    │
    ├── Global[0x813257AA] = 1            // Engine init started
    ├── Global[0x81334044] = -1           // Player state = invalid
    ├── Global[0x813357A4] = -1           // Session state = invalid
    ├── Global[0x81337764] = -1           // Network state = invalid
    ├── Global[0x8132504C] = 0x81835054   // Main vtable pointer
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 3: DIRECT3D DEVICE CREATION
    ├── ═══════════════════════════════════════════
    │
    ├── ★ sub_827DF248(383, 2, 0)         // D3DDevice::Create
    │   ├── r3 = 383 (adapter)
    │   ├── r4 = 2 (device type)
    │   └── r5 = 0 (behavior flags)
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 4: THREAD LOCAL STORAGE SETUP
    ├── ═══════════════════════════════════════════
    │
    ├── r11 = [r13+0] (TLS base pointer)
    ├── Global[0x81330A20] = 64           // TLS buffer size
    ├── [TLS+8] |= 6                      // Enable thread flags
    ├── [TLS+12] = 1                      // Thread state = active
    │
    ├── sub_82192578()                    // TLS init helper
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 5: STRING TABLE INITIALIZATION
    ├── ═══════════════════════════════════════════
    │
    ├── r11 = 0x81200A40 (-28864)         // Default string
    │
    ├── Store string tables:
    │   ├── Global[0x81326CC4] = "DefaultString"
    │   └── Global[0x81326CD4] = "DefaultString"
    │
    ├── Check path tables:
    │   ├── If Global[0x81326CC4+4] == 0:
    │   │   └── Store: 0x812009E0
    │   └── If Global[0x81326D24+4] == 0:
    │       └── Store: 0x812009DC
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 6: GPU INITIALIZATION
    ├── ═══════════════════════════════════════════
    │
    ├── ★ sub_82850AF0()                  // Check GPU ready
    │   └── Returns: bool (GPU available)
    │
    ├── If GPU not ready:
    │   └── ★ sub_82850B60(0)             // Init GPU mode
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 7: ENGINE CONTEXT ALLOCATION
    ├── ═══════════════════════════════════════════
    │
    ├── Check override: Global[0x813257F8+4]
    │   ├── If set: r30 = 0 (use existing)
    │   └── If not: use Global[0x813257D4+4]
    │
    ├── sub_8218BE28(472)                 // Allocate 472-byte context
    ├── r29 = result
    │
    ├── If allocation succeeded:
    │   │
    │   ├── ★ sub_82857028()              // Init D3D context
    │   │
    │   ├── [r29+0] = 0x81200970          // Store vtable
    │   │
    │   ├── r31 = 0x8133826C
    │   └── Global[0x8133826C] = r29      // Store context ptr
    │
    ├── Else:
    │   └── Global[0x8133826C] = 0
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 8: RENDER STATE SETUP
    ├── ═══════════════════════════════════════════
    │
    ├── ★ sub_82851548(2, 0)              // Set render mode
    │
    ├── r3 = [r31] (context)
    ├── r4 = 0x81301D38 (render params)
    ├── r5 = r28 (XEX path)
    │
    ├── Global[0x81333E80] = 0x812009C4   // Render callback
    │
    ├── ★ sub_82856C38(ctx, params, path) // Configure renderer
    │
    ├── sub_8285A0E0(0x812009B4)          // Set shader path
    │
    ├── ★ sub_82850748(32, 11264, 16, 1)  // Alloc render buffers
    │   ├── r3 = 32 (count)
    │   ├── r4 = 11264 (size each)
    │   ├── r5 = 16 (alignment)
    │   └── r6 = 1 (flags)
    │
    ├── ★ sub_82856C90(ctx, r30, 0)       // Final render init
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 9: VTABLE METHOD CALL
    ├── ═══════════════════════════════════════════
    │
    ├── r3 = [r31] (context)
    ├── r11 = [r3+0] (vtable)
    ├── r11 = [r11+4] (method @ offset 4)
    ├── CALL [r11]                        // context->Init()
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 10: SUBSYSTEM INITIALIZATION
    ├── ═══════════════════════════════════════════
    │
    ├── sub_82285F90()                    // Input init
    ├── sub_8219FC80(r30)                 // Timer init
    ├── sub_822214E0()                    // Audio init
    │
    ├── Check: Global[0x81333DD0+4]
    │
    ├── sub_823193A8(0x81386348)          // Resource paths
    ├── sub_821EC3E8()                    // Script system
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 11: CAMERA CONTEXT
    ├── ═══════════════════════════════════════════
    │
    ├── r31 = 0x81328CDC
    │
    ├── If Global[0x81328CDC] == 0:
    │   │
    │   ├── sub_8218BE28(48)              // Alloc 48 bytes
    │   ├── r30 = result
    │   │
    │   ├── If success:
    │   │   ├── sub_827EB6E0()            // Init camera
    │   │   └── Global[0x81328CDC] = r30
    │   │
    │   └── Else:
    │       └── Global[0x81328CDC] = 0
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 12: THREAD POOL
    ├── ═══════════════════════════════════════════
    │
    ├── sub_827EED88()                    // Get thread pool
    │
    ├── r3 = Global[0x81328CDC]
    ├── r4 = Global[0x81330C98] (64-bit)
    │
    ├── sub_827EB748(camera, params)      // Configure camera
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 13: WORKER THREADS
    ├── ═══════════════════════════════════════════
    │
    ├── sub_827EEB48(params, 65536, 32768, 1, 7, 0)
    │   ├── r3 = 0x812009A8 (thread params)
    │   ├── r4 = 65536 (stack size)
    │   ├── r5 = 32768 (heap size)
    │   ├── r6 = 1 (thread count)
    │   ├── r7 = 7 (priority)
    │   └── r8 = 0 (flags)
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 14: FINAL SETUP
    ├── ═══════════════════════════════════════════
    │
    ├── sub_82193BC0(0x81333DE8)          // Save context init
    ├── sub_82197338(0x8134B360)          // Config init
    │
    └── Return 1 (success)
```

**Key Allocations:**
| Size | Purpose | Global |
|------|---------|--------|
| 472 bytes | Engine context | `0x8133826C` |
| 48 bytes | Camera context | `0x81328CDC` |
| 32 × 11264 | Render buffers | Internal |

**Xbox 360 APIs Used:**
| Address | API | Purpose |
|---------|-----|---------|
| `sub_829A0A48` | `XSetThreadProcessor()` | CPU affinity |
| `sub_827DF248` | `IDirect3D9::CreateDevice()` | D3D device |
| `sub_82850AF0` | GPU check | Xenos validation |
| `sub_82857028` | D3D context init | Xenos setup |

---

### 23.2 sub_82120EE8 (0x82120EE8) - Core Engine Init

**Location:** `ppc_recomp.0.cpp:2398`
**Stack Frame:** 112 bytes

Allocates game manager and world context structures.

```
sub_82120EE8 (Core Engine Init)
    │
    ├── r31 = 0x81328B30 (globals base)
    ├── r29 = 0 (NULL constant)
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 1: GAME MANAGER ALLOCATION
    ├── ═══════════════════════════════════════════
    │
    ├── Check: Global[0x81328B34] (game manager)
    │
    ├── If NULL:
    │   │
    │   ├── sub_8218BE28(944)             // Allocate 944 bytes
    │   ├── r30 = result
    │   │
    │   ├── If success:
    │   │   ├── sub_821207B0()            // Initialize manager
    │   │   └── Global[0x81328B34] = r30
    │   │
    │   └── Else:
    │       └── Global[0x81328B34] = 0
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 2: PHYSICS ENGINE CHECK
    ├── ═══════════════════════════════════════════
    │
    ├── Check: Global[0x81320004+4] (physics engine)
    │
    ├── If NULL:
    │   └── sub_82673718()                // Init physics engine
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 3: WORLD CONTEXT ALLOCATION
    ├── ═══════════════════════════════════════════
    │
    ├── r30 = 0x81328B30
    │
    ├── Check: Global[0x81328B38] (world context)
    │
    ├── If NULL:
    │   │
    │   ├── sub_8218BE28(352)             // Allocate 352 bytes
    │   ├── r31 = result
    │   │
    │   ├── If success:
    │   │   ├── [r31+324] = 0             // Clear flags
    │   │   ├── [r31+328] = 0             // Clear count (16-bit)
    │   │   ├── [r31+330] = 0             // Clear index (16-bit)
    │   │   │
    │   │   ├── sub_8296BE18(r31+336)     // Init physics world
    │   │   │
    │   │   └── Global[0x81328B38] = r31
    │   │
    │   └── Else:
    │       └── Global[0x81328B38] = 0
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 4: RESOURCE SYSTEMS
    ├── ═══════════════════════════════════════════
    │
    ├── sub_82269098()                    // Resource manager
    │
    ├── sub_822054F8()                    // Asset manager
    │
    ├── sub_821DE390()                    // Script manager
    │
    ├── sub_8221F8A8()                    // Audio manager
    │
    ├── sub_82273988(0x81323100, 1)       // Final setup
    │   ├── r3 = 0x81323100 (context)
    │   └── r4 = 1 (enable flag)
    │
    └── Return
```

**Key Structures:**
| Size | Purpose | Global |
|------|---------|--------|
| 944 bytes | Game Manager | `0x81328B34` |
| 352 bytes | World Context | `0x81328B38` |

**World Context Layout:**
```
Offset  Size  Field
------  ----  -----
0x000   324   Main data
0x144   4     Flags
0x148   2     Count
0x14A   2     Index
0x150   ?     Physics world (via sub_8296BE18)
```

---

### 23.3 sub_821250B0 (0x821250B0) - Memory Pool Allocator

**Location:** `ppc_recomp.0.cpp:12583`
**No stack frame** (leaf function)

Circular buffer allocator for game objects.

```
sub_821250B0 (Memory Pool Allocator)
    │
    ├── Args:
    │   └── r3 = pool struct pointer
    │
    ├── Pool struct layout:
    │   ├── [+0]  = data base pointer
    │   ├── [+4]  = allocation bitmap
    │   ├── [+8]  = capacity
    │   ├── [+12] = element size
    │   ├── [+16] = current index
    │   └── [+20] = allocation count
    │
    ├── r11 = [r3+16] (current index)
    ├── r10 = 0 (wrapped flag)
    ├── r9 = [r3+8] (capacity)
    │
    ├── ═══════════════════════════════════════════
    │   SEARCH LOOP: Find free slot
    ├── ═══════════════════════════════════════════
    │
    │loc_821250BC:
    ├── r11++ (advance index)
    │
    ├── If r11 == r9 (wrapped):
    │   ├── r10 = r10 & 0xFF
    │   ├── r11 = 0 (reset to start)
    │   │
    │   ├── If r10 != 0 (already wrapped once):
    │   │   └── CALL sub_82125154() → return 0 (pool full)
    │   │
    │   └── r10 = 1 (mark wrapped)
    │
    ├── r8 = [r3+4] (bitmap base)
    ├── r8 = [r8 + r11] (byte at index)
    ├── r8 = r8 & 0xFFFFFF80 (check high bit = in use)
    │
    ├── If r8 == 0 (slot free):
    │   └── Continue loop → loc_821250BC
    │
    ├── ═══════════════════════════════════════════
    │   FOUND FREE SLOT
    ├── ═══════════════════════════════════════════
    │
    ├── r10 = [r3+4] (bitmap)
    ├── r9 = [bitmap + r11] & 0x7F (clear in-use bit)
    ├── [bitmap + r11] = r9
    │
    ├── r9 = ([bitmap + r11] & 0x7F) + 1
    ├── r9 = r9 & 0x7F (ref count, max 127)
    │
    ├── If r9 <= 1:
    │   └── r9 = 1 (minimum ref count)
    │
    ├── r8 = [bitmap + r11] & 0xFFFFFF80 (preserve in-use)
    ├── r9 = r8 | r9 (combine)
    ├── [bitmap + r11] = r9
    │
    ├── ═══════════════════════════════════════════
    │   CALCULATE RETURN ADDRESS
    ├── ═══════════════════════════════════════════
    │
    ├── r10 = [r3+20] (alloc count)
    ├── r8 = [r3+12] (element size)
    ├── r7 = r10 + 1
    ├── r9 = [r3+0] (data base)
    ├── r10 = r8 * r11 (offset)
    │
    ├── [r3+16] = r11 (update current index)
    ├── [r3+20] = r7 (increment alloc count)
    │
    ├── r3 = r10 + r9 (data base + offset)
    │
    └── Return r3 (pointer to allocated element)
```

**Pool Structure (24 bytes):**
```
Offset  Size  Field
------  ----  -----
0x00    4     Data base pointer
0x04    4     Allocation bitmap pointer
0x08    4     Capacity (max elements)
0x0C    4     Element size (bytes)
0x10    4     Current search index
0x14    4     Total allocation count
```

---

### 23.4 sub_82318F60 (0x82318F60) - RAGE String Table Init

**Location:** `ppc_recomp.14.cpp:71443`
**Leaf function** (tail call)

Simple wrapper that initializes a RAGE engine string table.

```
sub_82318F60 (RAGE String Table Init)
    │
    ├── Args:
    │   └── r3 = string table name (e.g. "SaveData")
    │
    ├── r4 = 0 (default flags)
    │
    └── TAIL CALL → sub_827DF490 (string parser)
```

**sub_827DF490 (String Parser):**
```
sub_827DF490 (String Table Parser)
    │
    ├── Args:
    │   ├── r3 = string pointer
    │   └── r4 = flags
    │
    ├── r8 = r4 (save flags)
    │
    ├── r11 = [r3+0] (first char)
    ├── r11 = sign_extend(r11)
    ├── r11 = r11 - 34 (check for '"')
    ├── r7 = count_leading_zeros(r11) >> 5 & 1
    │
    ├── If r7 != 0 (quoted string):
    │   └── r3++ (skip opening quote)
    │
    ├── r10 = [r3+0] (current char)
    ├── r9 = sign_extend(r10)
    │
    ├── If r9 == 0 (empty string):
    │   └── Return (error)
    │
    ├── If quoted AND r9 == 34 (closing quote):
    │   └── Return (error - empty quoted)
    │
    ├── Process character:
    │   ├── r3++ (advance)
    │   │
    │   ├── If char >= 'A' (65) AND char <= 'Z' (90):
    │   │   └── r11 = char + 32 (to lowercase)
    │   │
    │   └── Continue parsing...
    │
    └── Return string hash/index
```

---

### 23.5 sub_82124080 (0x82124080) - Profile/Save Subsystem Init

**Location:** `ppc_recomp.0.cpp:10159`
**Stack Frame:** 224 bytes
**Xbox 360 Dependency:** **HIGH** - Profile APIs

```
sub_82124080 (Profile/Save Subsystem Init)
    │
    ├── Args:
    │   ├── r3 = sign-in required flag
    │   └── r4 = auto-save flag
    │
    ├── r26 = 0x81327BA8 (profile base)
    │
    ├── Check: [r26-1] (already initialized)
    │
    ├── If initialized:
    │   └── Return early
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 1: PROFILE STATE INIT
    ├── ═══════════════════════════════════════════
    │
    ├── r29 = 0x81327BA0 (profile state)
    ├── r31 = 0x81327BAA (sign-in flag)
    ├── r28 = 0x81327BA9 (auto-save flag)
    ├── r27 = 0x8133369C (save context)
    │
    ├── Global[0x81327BC0] = 0            // Clear profile data
    ├── [r29+31692] = r4                  // Store auto-save flag
    ├── [r31+31690] = r3                  // Store sign-in flag
    ├── [r28+31689] = r3                  // Copy sign-in flag
    │
    ├── f0 = [0x8120E0E4] (float constant)
    ├── [r26+0] = f0                      // Init timer
    │
    ├── Global[0x81327BC4] = 0            // Clear state
    ├── Global[0x81327BA8] = 0            // Clear profile
    ├── Global[0x8133369C] = 1            // Enable save system
    ├── Global[0x81327BAB] = 1            // Ready flag
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 2: TIMESTAMP CAPTURE
    ├── ═══════════════════════════════════════════
    │
    │loc_821240F4:
    ├── ★ MFTB r9                         // Read time base register
    ├── r11 = rotate(r9, 0)
    │
    ├── If r11 == 0:
    │   └── Retry → loc_821240F4
    │
    ├── Global[0x81327CF0] = r9 (64-bit)  // Store timestamp
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 3: SAVE PATH SETUP
    ├── ═══════════════════════════════════════════
    │
    ├── sub_827DB118(&stack[80], 0, "SaveGames", 0, 0)
    │   └── Build save game path
    │
    ├── r4 = stack[80..96] (path struct)
    ├── stack[92] = 0x812031E8 (path suffix)
    │
    ├── r30 = 0x81333DE8 (save context)
    │
    ├── sub_82192EB8(r30, path64, path32) // Set save paths
    │
    ├── sub_82192E00(r30)                 // ★ XContentCreate
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 4: USER PROFILE QUERY
    ├── ═══════════════════════════════════════════
    │
    ├── r3 = ([r28+31689] == 0) ? 1 : 0
    │
    ├── sub_82124268(r3)                  // Query user state
    │
    ├── r31 = [0x81327BAA+31690] (sign-in)
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 5: PROFILE NAME SETUP
    ├── ═══════════════════════════════════════════
    │
    ├── If r31 != 0 (sign-in required):
    │   │
    │   ├── sub_82990830(&stack[96], "SIGNED_IN_USER", 36)
    │   │   └── Copy profile name template
    │   │
    │   └── Continue
    │
    ├── Else:
    │   │
    │   └── sub_82990830(&stack[96], "LOCAL_PROFILE_USER", 43)
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 6: WORLD PROFILE COPY
    ├── ═══════════════════════════════════════════
    │
    ├── If r31 == 0 AND [r29+31692] != 0:
    │   │
    │   ├── r7 = Global[0x81328B38] (world context)
    │   │
    │   ├── If r7 != 0:
    │   │   │
    │   │   ├── r11 = [r7+328] (profile count)
    │   │   │
    │   │   ├── If r11 != 0:
    │   │   │   │
    │   │   │   ├── Loop over profiles (636 bytes each):
    │   │   │   │   │
    │   │   │   │   ├── r11 = [r7+324] + r8 (profile base)
    │   │   │   │   ├── Check [r11+630] (valid flag)
    │   │   │   │   ├── Check [r11+496] (name set)
    │   │   │   │   │
    │   │   │   │   ├── If valid:
    │   │   │   │   │   ├── Copy name to stack[96]
    │   │   │   │   │   └── Character by character
    │   │   │   │   │
    │   │   │   │   └── r8 += 636
    │   │   │   │
    │   │   │   └── End loop
    │   │   │
    │   │   └── Continue
    │   │
    │   └── Continue
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 7: SAVE FILE CREATION
    ├── ═══════════════════════════════════════════
    │
    ├── sub_82124540(&stack[96])          // Create save file
    │
    ├── If [r28+31689] == 0:
    │   │
    │   ├── sub_82123E20(1, 1)            // ★ Enable autosave
    │   │
    │   └── Global[0x8133369C] = 0        // Mark complete
    │
    ├── sub_821244B8()                    // Finalize
    │
    ├── [r26-1] = 1                       // Mark initialized
    │
    └── Return
```

**Profile Structure:**
```
Global Base: 0x81327BA0
Offset  Size  Field
------  ----  -----
-1      1     Initialized flag
+0      4     Timer (float)
+8      1     Auto-save flag
+9      1     Sign-in required
+10     1     Sign-in state
+11     1     Ready flag
+32     4     Profile data ptr
+36     4     State flags
```

---

### 23.6 sub_82120FB8 (0x82120FB8) - Main Game Setup ★ CRITICAL

**Location:** `ppc_recomp.0.cpp:2531`
**Stack Frame:** 144 bytes

This is the **main game initialization function** - initializes ~50 subsystems!

```
sub_82120FB8 (Main Game Setup) - COMPLETE SUBSYSTEM LIST
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 1: INITIAL STATE
    ├── ═══════════════════════════════════════════
    │
    ├── Global[0x81327674] = 0            // Clear game state
    ├── Global[0x81327694] = 0            // Clear flags
    ├── Global[0x81327696] = 0            // Clear flags
    │
    ├── ★ XNotifyPositionUI(1)            // Set notification position
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 2: CORE SUBSYSTEMS
    ├── ═══════════════════════════════════════════
    │
    ├── sub_822C1A30()  // [1] Streaming init
    ├── sub_82679950()  // [2] Physics world
    ├── sub_8221D880()  // [3] Audio system
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 3: PATH CONFIGURATION
    ├── ═══════════════════════════════════════════
    │
    ├── sub_827DB118(&stack[80], 0, "Models", 0, 0)
    │   └── Build models path
    │
    ├── sub_827DB118(&stack[96], 0, "Textures", 0, 0)
    │   └── Build textures path
    │
    ├── Store paths to globals:
    │   ├── Global[0x81330328] = stack[80..96]
    │   └── Global[0x81330338] = stack[96..112]
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 4: GAME SYSTEMS INIT
    ├── ═══════════════════════════════════════════
    │
    ├── sub_8219FD88()  // [4] Timer system
    │
    ├── Check startup save:
    │   ├── r11 = Global[0x81302D04]
    │   ├── If r11 != -1:
    │   │   └── Configure startup from save
    │   └── Continue
    │
    ├── sub_822F8980()  // [5] Resource streamer
    ├── sub_828E0AB8()  // [6] ★ Frame tick
    ├── sub_822EEDB8()  // [7] World streamer
    ├── sub_82270170()  // [8] Entity system
    ├── sub_828E0AB8()  // [9] ★ Frame tick
    │
    ├── sub_822FD328(2000)  // [10] Pool init (size=2000)
    ├── sub_822EFF40()  // [11] Map loader
    ├── sub_82120C48()  // [12] Game config
    ├── sub_82221410()  // [13] UI system
    ├── sub_8226CB50()  // [14] Camera system
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 5: MISSION/SCRIPT SYSTEMS
    ├── ═══════════════════════════════════════════
    │
    ├── sub_821A8868()  // [15] Mission manager
    ├── sub_821A8278("MissionData", 50)  // [16] Mission loader
    │   ├── r3 = "MissionData" path
    │   └── r4 = 50 (max missions)
    │
    ├── sub_821BC9E0()  // [17] Script compiler
    ├── sub_822DB4B0()  // [18] Event system
    ├── sub_821B7218()  // [19] Trigger system
    ├── sub_822498F8()  // [20] Checkpoint system
    ├── sub_828E0AB8()  // [21] ★ Frame tick
    │
    ├── sub_8225DC40()  // [22] Cutscene system
    ├── sub_828E0AB8()  // [23] ★ Frame tick
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 6: WORLD SYSTEMS
    ├── ═══════════════════════════════════════════
    │
    ├── sub_821E24E0()  // [24] Weather system
    ├── sub_821DFD18()  // [25] Time of day
    ├── sub_8220E108()  // [26] Traffic system
    ├── sub_828E0AB8()  // [27] ★ Frame tick
    ├── sub_828E0AB8()  // [28] ★ Frame tick
    │
    ├── sub_821AB5F8()  // [29] Pedestrian system
    ├── sub_828E0AB8()  // [30] ★ Frame tick
    ├── sub_828E0AB8()  // [31] ★ Frame tick
    │
    ├── sub_821D8358()  // [32] Vehicle system
    ├── sub_821EA0B8()  // [33] Garage system
    ├── sub_82122CA0()  // [34] Player spawn
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 7: ONLINE/ACHIEVEMENT SYSTEMS
    ├── ═══════════════════════════════════════════
    │
    ├── sub_821AA660(0x81302E40)  // [35] Stats system
    ├── sub_82200EB8()  // [36] Profile system
    ├── sub_8212FB78()  // [37] Unlocks system
    │
    ├── ★ sub_8219ADF0()  // [38] Online system init
    ├── ★ sub_8212F578()  // [39] Leaderboard init
    ├── ★ sub_8212EDC8()  // [40] Achievement init
    │
    ├── sub_82138710()  // [41] Replay system
    ├── sub_821B2ED8()  // [42] Radio system
    ├── sub_828E0AB8()  // [43] ★ Frame tick
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 8: INPUT/CONTROLLER
    ├── ═══════════════════════════════════════════
    │
    ├── sub_822467B8()  // [44] Input mapper
    ├── sub_82208460()  // [45] Controller config
    ├── sub_821B9DA8()  // [46] Vibration system
    ├── sub_828E0AB8()  // [47] ★ Frame tick
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 9: MENUS/UI
    ├── ═══════════════════════════════════════════
    │
    ├── sub_82258100()  // [48] Menu system
    ├── sub_821A03A0()  // [49] HUD system
    ├── sub_8232A2C0()  // [50] Minimap
    ├── sub_828E0AB8()  // [51] ★ Frame tick
    ├── sub_828E0AB8()  // [52] ★ Frame tick
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 10: FINALIZATION
    ├── ═══════════════════════════════════════════
    │
    ├── sub_821B5DE8()  // [53] Weapon system
    ├── sub_821D8058()  // [54] Wanted system
    ├── sub_822868C8()  // [55] Phone system
    ├── sub_82289698(0x81309290)  // [56] Contact list
    ├── sub_82125478()  // [57] Startup handler
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 11: THREAD REGISTRATION
    ├── ═══════════════════════════════════════════
    │
    ├── r31 = 0x81328CD4 (game thread context)
    │
    ├── Check: Global[0x81328CF4] & 1
    │
    ├── If not registered:
    │   │
    │   ├── Global[0x81328CF4] |= 1
    │   │
    │   ├── [r31+0] = 0x812012F4 (vtable)
    │   ├── [r31+260] = 0
    │   │
    │   ├── sub_8298ED98(0x81300BD0)      // Thread name
    │   │
    │   └── Continue
    │
    ├── sub_827E0C30(r31, "GameThread", 1)
    │   └── Register main game thread
    │
    ├── sub_827E0CF8(r31, "GameLoop")
    │   └── Set thread function
    │
    ├── ═══════════════════════════════════════════
    │   PHASE 12: FINAL SYSTEMS
    ├── ═══════════════════════════════════════════
    │
    ├── sub_8227AC28()  // [58] Network manager
    ├── sub_828E0AB8()  // [59] ★ Frame tick
    ├── sub_82272290()  // [60] Session manager
    ├── sub_82212450()  // [61] Voice chat
    ├── sub_822C5768()  // [62] DLC manager
    ├── sub_822D4C68()  // [63] Content manager
    │
    └── Return
```

**Subsystem Init Order (63 systems):**
| # | Address | System |
|---|---------|--------|
| 1 | `sub_822C1A30` | Streaming |
| 2 | `sub_82679950` | Physics |
| 3 | `sub_8221D880` | Audio |
| 4 | `sub_8219FD88` | Timer |
| 5 | `sub_822F8980` | Resource Streamer |
| 6-9 | `sub_828E0AB8` | Frame Ticks |
| 10 | `sub_822FD328` | Memory Pools |
| 11 | `sub_822EFF40` | Map Loader |
| 12 | `sub_82120C48` | Game Config |
| 13 | `sub_82221410` | UI |
| 14 | `sub_8226CB50` | Camera |
| 15-16 | `sub_821A8xxx` | Missions |
| 17 | `sub_821BC9E0` | Scripts |
| 18-20 | Various | Events/Triggers/Checkpoints |
| 21-23 | Frame Ticks | --- |
| 24-26 | Weather/ToD/Traffic | World Systems |
| 27-31 | Frame Ticks | --- |
| 32-34 | Vehicles/Garage/Spawn | Player Systems |
| 35-37 | Stats/Profile/Unlocks | Progress |
| **38** | `sub_8219ADF0` | **Online System** |
| **39** | `sub_8212F578` | **Leaderboards** |
| **40** | `sub_8212EDC8` | **Achievements** |
| 41-43 | Replay/Radio | Media |
| 44-47 | Input/Controller | Controls |
| 48-52 | Menus/HUD/Minimap | UI |
| 53-56 | Weapons/Wanted/Phone | Gameplay |
| 57-63 | Network/Voice/DLC | Online |

---

## Document History
- 2025-12-20: Consolidated `MODULE_REWRITE_INDEX.md` + `REWRITE_HANDOFF.md` into this playbook.
- 2025-12-20: Added comprehensive per-module handoff documentation with function tables, test cases, and implementation notes.
- 2025-12-20: Added PPC recompiled code reference with function tables, address mappings, and rewrite strategies for renderer/thread systems.
- 2025-12-21: Added Port Features Roadmap (Sections 14-15) covering save data, achievements, network, localization, renderer enhancements, HFR, ultrawide, controller features, input latency, and XAM subsystem reference.
- 2025-12-21: **Added PPC hook discovery** - Analyzed dense PPC recompiled code to find hookable game functions for save data, achievements, HFR, and ultrawide. Documented call chains and hook strategies.
- 2025-12-21: **Added Online/Multiplayer hooks** - Documented 30+ network imports, 8 XNet wrappers, 8 multiplayer manager functions. Includes phased implementation strategy for LAN/online play.
- 2025-12-21: **Added Execution Trace** - Manually traced execution from `_xstart` (0x829A0860) through ~50 subsystem inits to first draw call (`VdSwap`). Documents complete boot chain with hookable addresses.
- 2025-12-21: **Added Deep Boot Traces (§17)** - Deep execution traces for `sub_829A7FF8` (early system init), `sub_82994700` (runtime init with TLS/CRT), `sub_829A0678` (HDCP privilege check with language tables).
- 2025-12-21: **Added Renderer Trace (§18)** - Complete render pipeline documentation: render targets, viewports, texture binding (19 slots), D3D Present/VdSwap with device structure offsets.
- 2025-12-21: **Added Save System Trace (§19)** - Save system init with 3 slot types (Profile/Game/Autosave), 1392-byte context structure, vtable addresses, implementation roadmap.
- 2025-12-21: **Added Online/Multiplayer Trace (§20)** - Network init chain, session management, socket API mapping (13 functions), XNet functions (10 functions), phased implementation plan.
- 2025-12-21: **Added Extended Boot Traces (§17.4-17.8)** - Deep traces for `sub_829A7EA8` (init table executor), `sub_829A7DC8` (C++ constructors with 1338 init functions!), `sub_829A27D8` (cmdline access), `sub_8218BEA8` (game entry), `sub_827D89B8` (8-phase game init wrapper with network, argc/argv, engine vtable calls).
- 2025-12-21: **Added Online/Achievement/Leaderboard Traces (§21)** - Deep traces for `sub_8219ADF0` (online system init with 228 config entries), `sub_8212EDC8` (achievement tracking with 50+ trackers, 508-byte structures, 6 achievement types), `sub_8212F578` (leaderboard init with 27 categories), `sub_82199658` (tracker factory). Includes tracker structure layout and implementation hooks.
- 2025-12-21: **Added Xbox 360 Hardware-Tied Functions (§22)** - Critical rewrite documentation for `sub_827D89B8` call tree: cmdline parser, XNet init/cleanup, thread events (256 kernel events), core engine init (D3D, GPU), game subsystem init (944B manager, 352B world), profile/save loading (XContent APIs). Includes rewrite priority matrix and cross-platform implementation strategies.
- 2025-12-21: **Added Game Init Deep Traces (§23)** - Complete execution traces for `sub_8218C600` (14-phase core engine init: D3D, GPU, TLS, render buffers), `sub_82120EE8` (game manager 944B, world context 352B), `sub_821250B0` (memory pool allocator with bitmap), `sub_82318F60` (RAGE string tables), `sub_82124080` (7-phase profile/save init with XContent), `sub_82120FB8` (**63 subsystem init** with complete order list).
