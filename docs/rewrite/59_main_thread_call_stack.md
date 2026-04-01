# 59: Main Thread Stack Overflow — Root Cause Call Chain Analysis

## Executive Summary

The main thread (t41614048) overflows its guest stack at 5.8MB during world initialization.
The root cause is **NOT unbounded recursion** but rather the cumulative effect of three
factors operating simultaneously:

1. **Recompilation stack amplification**: Each PPC function becomes a native C++ function
   call consuming ~3-8x more stack than the original PPC frame
2. **Deep but bounded call chains**: RAGE engine world init has 20-40 levels of nesting
   through vtable dispatchers, render state setup, and shader parameter binding
3. **Undersized guest stack allocation**: The XEX header's `DEFAULT_STACK_SIZE` (likely
   64KB-256KB) is far too small when combined with amplification factor 1

The stack does NOT overflow from a single function or recursive loop. It overflows because
the rendering subsystem's initialization path creates a call chain deep enough that the
~200 bytes/frame (PPC average) times ~30 levels times ~5x amplification exhausts the guest
stack over many nested calls.

---

## 1. Log Timeline (lines 1175 to 81823)

### Phase 1: Normal Operation (lines ~462-1179)
- YIELD messages from `sub_82849918` (spin-wait/sleep in the per-frame loop)
- Counts from #0 to #21000 (21K iterations of the main loop)
- VFS file resolution for audio configs and DLC content
- `[SetTexture] skip` messages for unregistered textures (expected without GPU)

### Phase 2: MISSING-FUNC Storm (lines 1180-81822)
- ~80,000 lines of `[MISSING-FUNC] indirect call to 00000000` messages
- **Dominant callers** (by frequency across entire 5.5M line log):
  - `0x828C99CC` (inside `sub_828C9980`): **2,338,745 calls** — shader variable setter calling NULL vtable[+52]
  - `0x821446F8` (inside `sub_82144188`): **13,136 calls** — main frame loop calling `sub_821B3990`
  - `0x8214434C` (inside `sub_82144188`): **13,136 calls** — main frame loop calling `sub_821B3970`
  - `0x8291E144` / `0x8291E1B0` (inside `sub_8291DF00`): **96 calls each** — scene graph iterator null vtable
  - `0x821911C4` (inside `sub_821910D0`): **64 calls** — waitable object polling `vtable[+68]` getting 0x000F4000
  - `0x828C1F94` (inside `sub_828C19C0`): **37 calls** — render state switch dispatch

### Phase 3: Stack Guard Page Avalanche (lines 81823-81899+)
- Guard pages hit at **58 distinct addresses** from `0x70000000` to `0x705C0000` (each 64KB page)
- Then stuck at `0x705D0000` with `BaseHeap::Protect failed due to uncommitted page`
- **1,569,259 total guard page hits** across the entire run (most are 0x705D0000 repeats)
- Total guest stack growth: `0x705D0000 - 0x70000000 = 0x5D0000 = 5,832,704 bytes (5.8MB)`

### Phase 4: Degenerate Loop (lines ~82000 to 5,507,626)
- Alternating `BaseHeap::Protect failed` / `Stack guard page hit at 0x705D0000` forever
- Game continues executing (YIELD messages appear at line ~5.5M with counts up to #2306000)
- The stack is stuck at its limit but the game still runs (guard faults become no-ops)

---

## 2. Guest Stack Allocation Layout

**Source**: `XThread::AllocateStack()` in `glue/rexglue-sdk-main/src/system/xthread.cpp`, lines 224-253

The main thread stack is allocated by `KernelState::LaunchModule()` using `module->stack_size()`
which reads from `XEX_HEADER_DEFAULT_STACK_SIZE` (`0x00020200` in the XEX header).

```
stack_alloc_base_  = 0x70000000          (lowest address, bottom-up allocation)
  [64KB bottom guard page - NoAccess]
stack_limit_       = 0x70010000          (bottom of usable stack)
  [usable stack: round_up(XEX_stack_size, 64KB)]
stack_base_        = 0x70010000 + size   (top of usable stack, where r1 starts)
  [64KB top guard page - NoAccess]
```

Host thread stack: **16 MiB** (hardcoded at `xthread.cpp`, line 371)

The guard page handler (`xmemory.cpp`, line 448-461) catches ANY write in the 0x70000000-0x7F000000
range and unprotects the faulting page. This allows stack growth far beyond the original
allocation, until it hits an uncommitted page at 0x705D0000.

---

## 3. What Kind of Call Chain Consumes 5.8MB

### Guest Stack Frame Statistics (24,782 recompiled functions)
- **Average PPC frame**: 204 bytes (from `stwu r1,-N(r1)` analysis)
- **Functions >= 1KB frame**: 400 functions
- **Functions >= 10KB frame**: 35 functions
- **Largest frame**: 19,360 bytes (`sub_82364230` — shader/rendering function with VMX saves)

### Top 10 Largest PPC Stack Frames
| Function | Frame (bytes) | File |
|----------|--------------|------|
| sub_82364230 | 19,360 | gta4_recomp.13.cpp |
| sub_82368F78 | 19,328 | gta4_recomp.13.cpp |
| sub_82369158 | 19,296 | gta4_recomp.13.cpp |
| sub_828BAFD0 | 19,264 | gta4_recomp.58.cpp |
| sub_82367768 | 19,200 | gta4_recomp.13.cpp |
| sub_829B6178 | 19,168 | gta4_recomp.63.cpp |
| sub_82367240 | 19,152 | gta4_recomp.13.cpp |
| sub_82367A98 | 19,152 | gta4_recomp.13.cpp |
| sub_82367988 | 19,120 | gta4_recomp.13.cpp |
| sub_824EB020 | 19,152 | gta4_recomp.24.cpp |

**All 19KB-frame functions are in the 0x82364000-0x82369000 range** — RAGE engine shader
compiler/setup routines with VMX (vector unit) register saves. Each one does stack-probing
loads at -4096, -8192, -12288, -16384 before the `stwu` to avoid skipping guard pages.

### Why 5.8MB is Consumed

The 5.8MB guest stack is NOT consumed by a single call. It accumulates across the entire
run. Each time the main loop calls into world init or rendering, the call chain is:

```
game_main_loop (sub_82144188, 240-byte frame)
  -> sub_82143C88 (scene update, ~128 bytes)
    -> sub_828BD648 (render dispatch, 128 bytes)
      -> sub_828BD038 (render scene, ~160 bytes)
        -> sub_828BCFA8 (render pass, ~128 bytes)
          -> sub_828C9980 (SetVariable, 128 bytes)
            -> sub_828C97E0 (SetVariable inner, 160 bytes)
              -> sub_828C8A50 (data copy, leaf)
          -> sub_828C19C0 (render state, ~128 bytes)
            -> sub_828C8588 (state writer, leaf)
      -> sub_828BFF18 (present, ~96 bytes)
  -> sub_821B3970 (timing/sync, ~96 bytes)
```

At typical rendering depth (~10-15 levels), each level uses ~200 bytes of PPC stack:
- 15 levels x 200 bytes = **3,000 bytes per rendering pass** (PPC only)

But on the HOST, each level also consumes ~400-800 bytes of C++ stack:
- Callee-save register spills
- PPCRegister temp variables (8 bytes each)
- uint32_t ea (4 bytes)
- Return address and frame pointer (16 bytes)
- 16-byte alignment padding on macOS

So each PPC-to-host call pair uses ~600-1000 bytes total. For 15 levels of nesting,
that is **9,000-15,000 bytes per rendering pass** on the guest stack alone.

The critical accumulation happens during **world init**, when the game runs hundreds
of initialization passes without unwinding the top-level frame. The init code path is:

```
sub_82121E80 (world init entry, entered from sub_821200D0)
  -> sub_821E4398 (state machine reset)
  -> [many sub-calls for streaming, audio, particles, shaders...]
    -> sub_825BF8A8 (particle emitter batch registration, ~4608 emitters)
      -> sub_825EDDA0 (register emitter)
        -> sub_8218BE28 (malloc, reads TLS)
    -> [shader compilation / effect variable setup]
      -> sub_828C9980 x 2.3M calls (SetVariable with null vtables)
```

The particle emitter registration loop alone calls `sub_8218BE28` (malloc) 4608 times,
each requiring ~4-5 levels of nesting through the allocator. If the TLS heap is not set
up, the fallback allocator path adds additional depth.

---

## 4. Key Functions in the Call Chain

### sub_828C9980 — grcEffect::SetVariable (128-byte PPC frame)
- **2,338,745 MISSING-FUNC calls** (biggest contributor to log noise)
- Calls `vtable[+52]` on graphics resource objects
- Objects have NULL vtable pointers because D3D device not initialized (by design)
- **NOT recursive** — each call is independent, returns normally after MISSING-FUNC
- Stack impact: minimal per-call (128 bytes, unwound immediately)

### sub_82144188 — Main Frame Function (240-byte PPC frame)
- The per-frame dispatch hub called from the game's main loop
- Calls `sub_821B3970` and `sub_821B3990` (timing/sync, both hit null vtables)
- Calls `sub_828BD648` (render scene dispatch)
- Calls `sub_82143C88` (scene update) and `sub_82143DC8` (scene post-update)
- **This function stays on the stack for the entire duration of each frame**

### sub_821910D0 — Waitable Object Poller (vtable[+68] -> 0x000F4000)
- Calls `KeWaitForMultipleObjects` then polls `vtable[+68]` on a D3D device object
- Target 0x000F4000 is a garbage read from address 0x44 (68) in low memory
- 42 occurrences, appearing right before the stack guard avalanche
- **This is the last function executing when the stack finally overflows**

### sub_8291DF00 — Scene Graph Iterator (208-byte PPC frame)
- **NOT recursive** (confirmed in doc 63) — flat loop with stride 768 bytes
- Makes 9 indirect vtable calls per object, mostly NULL
- Contributes to MISSING-FUNC noise but NOT to stack depth

---

## 5. The 0x705D0000 Boundary

The guard page expansion stops at 0x705D0000 because:

1. `XThread::AllocateStack()` reserves+commits only `actual_size` bytes via `AllocRange`
2. The guard page handler (`AccessViolationCallback`) calls `heap->Protect()` on faulting pages
3. `Protect()` requires the page to be COMMITTED — it cannot change protection on reserved-only pages
4. At 0x705D0000, the page is reserved (part of the v40 heap's VA range) but NOT committed
5. `BaseHeap::Protect` returns failure, the handler logs the error, but STILL returns `true`
6. The fault handler returning `true` tells the OS "handled" — execution continues
7. But the next `stwu` instruction writes to the SAME uncommitted page again
8. This creates an infinite fault loop: write -> fault -> handler says "handled" -> retry write -> fault...

**The log ends at 5,507,626 lines because the process is stuck in this infinite fault loop.**

---

## 6. Recursive / Deep-Nesting Functions

### Direct Recursion: None Found
No function in the generated code calls itself directly. The search across all 76 recomp
files found zero cases of `sub_XXXXXXXX` calling itself.

### Indirect Recursion: sub_828C9980 -> sub_828C97E0 chain
- `sub_828C9980` calls `sub_828C97E0` (the "SetVariable inner" function)
- `sub_828C97E0` calls `sub_828C8A50` (leaf data copy, does NOT call back)
- **This is NOT recursive** — the chain terminates at the leaf

### Deep Nesting: RAGE Rendering Pipeline
The rendering call chain can reach **15-20 levels** during shader setup:
```
main_loop -> frame_dispatch -> render_scene -> render_pass ->
  draw_object -> set_material -> set_variables (x many) ->
    SetVariable -> vtable dispatch -> (NULL/missing)
```

Each level is a separate C++ function with its own host stack frame. At 20 levels with
~600 bytes per level (host + guest combined), that is **12,000 bytes per rendering invocation**.

### Deep Nesting: World Init Path
The world initialization path is deeper:
```
main_entry -> world_init -> state_machine -> streaming_init ->
  resource_load -> rpf_parse -> file_io -> allocator ->
    heap_alloc -> fallback_allocator
```

This chain easily reaches **25-35 levels** during the loading phase, consuming
**15,000-25,000 bytes** of guest stack per invocation.

---

## 7. Stack Size Configuration

### Current Configuration
- **Guest stack**: From XEX header `DEFAULT_STACK_SIZE`, minimum 16KB, likely 64KB-256KB for GTA IV
- **Host stack**: 16 MiB (hardcoded, `xthread.cpp` line 371)
- **Guest stack range**: 0x70000000-0x7F000000 (240MB virtual address space)

### Where to Override
**File**: `glue/rexglue-sdk-main/src/system/kernel_state.cpp`, line 271

```cpp
// Current code:
auto thread = object_ref<XThread>(new XThread(
    kernel_state(), module->stack_size(), 0, ...));

// Fix: force 16MB minimum for recompiled code
auto stack = std::max(module->stack_size(), uint32_t(16 * 1024 * 1024));
auto thread = object_ref<XThread>(new XThread(
    kernel_state(), stack, 0, ...));
```

### Why 16MB is Sufficient
- Peak observed usage: ~5.8MB (during world init, one-time)
- After init, per-frame usage drops to ~12-20KB (normal rendering depth)
- 16MB provides ~2.7x headroom over peak
- The call chains are all bounded (no unbounded recursion confirmed)

---

## 8. Summary of Findings

| Finding | Detail |
|---------|--------|
| **Root cause** | Guest stack too small for recompiled code's amplified frame sizes |
| **Stack amplification** | ~3-8x per frame (200 bytes PPC -> 600-1000 bytes host+guest) |
| **Peak depth** | ~25-35 levels during world init |
| **Peak usage** | ~5.8MB observed (likely ~6-8MB true peak with alignment) |
| **Largest single frame** | 19,360 bytes (sub_82364230, shader setup with VMX) |
| **MISSING-FUNC noise** | 2.3M calls, all from NULL vtables (GPU not emulated), no stack impact |
| **Recursion** | None found — all call chains are bounded |
| **Fix** | Override guest stack to 16MB in KernelState::LaunchModule() |
| **Host stack** | 16 MiB, not the bottleneck |
| **Terminal symptom** | Infinite fault loop at 0x705D0000 (uncommitted page) |
