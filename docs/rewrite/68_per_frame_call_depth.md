# 68: Per-Frame Call Depth Analysis (sub_82856F08 and sub_821428C8)

## Executive Summary

The per-frame loop hook in `imports.cpp` (line 2180) that calls `__imp__sub_82856F08`
is **dead code**. The address 0x82856F08 was never identified as a function boundary
by Ghidra or the codegen -- it was absorbed into `sub_82856D48` (a base64/encoding
utility function). Neither `sub_82856F08` nor `sub_8218BEA8` exist in the generated
mapping table or the final binary's symbol table. The hook compiles but links to
nothing.

The **actual per-frame tick** is `sub_821428C8`, called every yield iteration by the
main game state machine `sub_82142230`. It calls 15-17 subsystem update functions per
frame. After scene creation completes, several conditional subsystems activate, adding
new call chains that increase stack depth.

---

## 1. The Dead Hook: sub_82856F08

### imports.cpp (lines 2174-2200)
```cpp
extern "C" void __imp__sub_8218BEA8(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_82856F08(PPCContext &ctx, uint8_t *base);

PPC_FUNC_HOOK(sub_8218BEA8) {
  static bool s_initDone = false;
  if (!s_initDone) {
    __imp__sub_8218BEA8(ctx, base);  // full game init
    s_initDone = true;
  }
  while (true) {
    __imp__sub_82856F08(ctx, base);  // frame tick
    sleep(16ms);
  }
}
```

### Why it is dead code

| Check | Result |
|-------|--------|
| sub_82856F08 in `gta4_functions.txt` (Ghidra) | NOT PRESENT |
| sub_82856F08 in `gta4_init.cpp` (mapping table) | NOT PRESENT |
| sub_82856F08 in any `gta4_recomp.*.cpp` | NOT PRESENT (absorbed into sub_82856D48) |
| `__imp__sub_82856F08` in `libLibertyRecompLib.a` | NOT PRESENT |
| `_sub_82856F08` in final binary (`nm`) | NOT PRESENT |
| `_sub_8218BEA8` in final binary (`nm`) | NOT PRESENT |
| `__imp__sub_82856F08` in `imports.cpp.o` | Present as `U` (undefined) |
| `__imp__sub_8218BEA8` in `imports.cpp.o` | Present as `U` (undefined) |

The object file references these symbols but the linker dead-strips the hook because:
1. `sub_8218BEA8` doesn't exist in generated code (no weak symbol to override)
2. No other code calls `sub_8218BEA8`, so the strong definition has no callers
3. The linker discards the unreachable function and its undefined references

### What sub_82856D48 actually is

`sub_82856D48` spans 0x82856D48 to ~0x82857070 (440 lines of generated code). It is a
**base64/encoding function** that encodes data to a stream using a 64-entry lookup
table. It calls only `sub_8285AFD8` (stream flush). It has nothing to do with the
per-frame game loop.

The render path comment in `video.cpp` line 9386:
```
// Render path: sub_82856F08 -> sub_828529B0 -> sub_828507F8 -> sub_82A467D8 -> Video::Present
```
was likely based on runtime address tracing. The addresses 0x828529B0 and 0x828507F8 are
also mid-function offsets (absorbed into sub_828526A0 and sub_828507A8 respectively), NOT
separate function boundaries.

---

## 2. The Real Per-Frame Tick: sub_821428C8

**File**: `gta4_recomp.0.cpp` line 6281
**PPC stack frame**: 128 bytes (`stwu r1,-128(r1)`)

Called every yield iteration from sub_82142230 (the main game SM), this is the true
frame tick. It processes all per-frame game systems.

### Call sequence (in execution order)

| # | Function | Condition | Likely Subsystem |
|---|----------|-----------|------------------|
| 1 | `sub_821B38D8` | Always | Timer/clock update |
| 2 | `sub_8214C8C8` | Always | **Main game update** (58 callees, ~2000 lines) |
| 3 | `sub_82205850(ptr, 0)` | Always | World/scene update (calls 10 sub-functions) |
| 4 | `sub_821B71B0` | Flag-gated | Streaming tick (only when world initialized) |
| 5 | `sub_821B5A68` | Flag-gated | Text/locale update |
| 6 | `sub_8222DAA0` | Flag-gated | World manager update |
| 7 | `sub_82145820` | Flag-gated | DLC/post-load work |
| 8 | `sub_826CDEB8` | Always | Save/profile system update |
| 9 | `sub_8222E760` | Flag-gated | World streaming finalize |
| 10 | `sub_8222DE88` | Always | Streaming system flush |
| 11 | `sub_821B3958` | Always | Resource manager tick |
| 12 | `sub_821B3A70` | Always | Memory/heap maintenance |
| 13 | `sub_8222E338` | Always | Streaming system post-tick |
| 14 | `sub_821B5A08` | Always | Timer/profiling update |
| 15 | `sub_821B3B80` | Always | Allocator maintenance |
| 16 | `sub_822BCA90` | Always | Network/multiplayer tick |
| 17 | `sub_821B5890` | Always | End-of-frame cleanup |

### The gating flags

The conditional subsystems (#4-#7, #9) are gated by a flag at:
```
r29 = lis -31971 -> r29+21284  (address ~0x82B053E4)
```
- Byte at 0x82B053E4 = 0: conditional subsystems SKIPPED (during boot/init)
- Byte at 0x82B053E4 = 1: conditional subsystems ACTIVE (after scene creation)

A second flag at offset -24252 from `r28` (lis -32064) provides additional gating.

**This is the mechanism that changes the per-frame depth after scene creation.**
Before scene creation completes, only calls #1-3, #8, #10-17 execute (10 functions).
After scene creation completes, ALL 17 calls execute, adding world streaming, DLC
detection, and world manager updates.

---

## 3. sub_8214C8C8 -- The Deepest Per-Frame Path

**File**: `gta4_recomp.0.cpp` line 30455
**PPC stack frame**: estimated 128-256 bytes
**Callees**: 58 unique functions (the largest fan-out of any per-frame function)

Key subsystems called:

| Address Range | Count | Likely System |
|--------------|-------|---------------|
| 0x8214xxxx | 8 | Core game logic, initialization |
| 0x8215xxxx | 4 | Game objects, entities |
| 0x8216xxxx | 2 | Physics/collision |
| 0x821Bxxxx | 1 | Streaming |
| 0x821Cxxxx | 1 | Events/callbacks |
| 0x8220xxxx | 2 | World/scene management |
| 0x8221xxxx | 1 | AI/decision |
| 0x8223xxxx | 2 | UI/XAM state machine |
| 0x8224xxxx | 14 | **Player/vehicle systems** (largest group) |
| 0x8225xxxx | 8 | **Input/camera/gameplay** |
| 0x8229xxxx | 7 | **Streaming/resource loading** |
| 0x822Dxxxx | 1 | Animation |
| 0x8239xxxx | 2 | Audio |
| 0x826Cxxxx | 4 | Save/profile |
| 0x8290xxxx | 1 | Scene graph |

This function is the **game update dispatcher** -- it calls into nearly every major
game subsystem. Each of these 58 callees has its own call tree. The player/vehicle
systems (0x8224xxxx, 14 functions) are the largest group, suggesting character and
vehicle update is the deepest branch.

---

## 4. Call Depth Estimation

### Before scene creation (~21000 yield iterations OK)

```
sub_82142230 (main SM, frame ~128B)
  -> sub_82849918 (yield, pass-through)
  -> sub_821428C8 (frame tick, 128B)
       -> sub_821B38D8 (timer, ~96B)
       -> sub_8214C8C8 (update, ~256B)
            -> 58 subsystems, each ~128-256B
                 -> 2-5 levels deep on average
       -> sub_826CDEB8 (save, ~128B)
       -> sub_821B3958..sub_821B5890 (maintenance, ~96B each)
```

Estimated max stack depth: **~6-8 call levels**, consuming:
- PPC guest stack: ~128B x 8 = ~1KB per frame
- Host stack: ~256B x 8 = ~2KB per frame
- Both easily within limits (64KB guest, 16MB host)

### After scene creation (new systems activate)

The conditional subsystems add:

```
sub_821428C8 (frame tick)
  -> sub_821B71B0 (streaming tick)
       -> streaming subsystem (3-5 levels deep)
  -> sub_8222DAA0 (world manager)
       -> sub_8222E760 (world streaming finalize)
            -> possible vtable dispatch to scene objects
  -> sub_82145820 (DLC detection)
```

The world manager `sub_8222DAA0` and streaming tick `sub_821B71B0` can trigger:
1. **Resource loading**: file I/O chains (CreateFileA -> ReadFile -> NtReadFile)
2. **Object instantiation**: particle emitter registration (sub_825BF8A8 loop)
3. **Scene graph population**: vtable dispatch to scene render objects

### The crash path (scene render vtable dispatch)

The scene render system runs on the **main thread** via sub_8214C8C8:

```
sub_8214C8C8 (game update)
  -> sub_829025A8 (scene graph manager)
       -> [indirect] sub_829022B0 (render dispatch)
            -> sub_8291E620 (scene renderer)
                 -> sub_8291E260 (draw list processor)
                 -> sub_8291DF00 (scene object iterator)
                      -> vtable[10]: NULL -> MISSING-FUNC (x 1.18M)
                      -> vtable[6]:  NULL -> MISSING-FUNC (x 1.18M)
```

This path is 6 levels deep from sub_821428C8. The MISSING-FUNC calls themselves
do NOT grow the stack (they are inline fprintf), but the 6-level nesting contributes
~6 x 200B = ~1.2KB of PPC guest stack.

---

## 5. What Changes After Scene Creation

### Before scene creation (yield #0 to #13000)

The main SM is in states 0-6, driving the scene creation pipeline. sub_821428C8 runs
every iteration but the conditional subsystems are DISABLED (flag at 0x82B053E4 = 0).

Per-frame stack depth: **~4-6 levels** (timer + game update + save + maintenance)

### After scene creation (yield #13000 to #21000)

The SM reaches state 7/9 and activates the ready signal (0x82BF9B70 = 1). The flag
at 0x82B053E4 is set to 1 by the finalization code. Now ALL conditional subsystems
run:

1. **Streaming tick** (`sub_821B71B0`): begins loading world chunks
2. **World manager** (`sub_8222DAA0`): starts managing loaded objects
3. **DLC detection** (`sub_82145820`): probes TLAD/TBOGT content
4. **World streaming finalize** (`sub_8222E760`): processes loaded data

Per-frame stack depth: **~6-10 levels** (all subsystems active)

### The overflow point (yield #21000+)

At yield #21000, the scene graph has been populated with objects whose vtable entries
are null (render device not initialized). The render dispatch path activates:

```
sub_8214C8C8 -> sub_829025A8 -> [indirect] -> sub_829022B0 -> sub_8291E620
  -> sub_8291DF00 (iterates N objects, 2 vtable calls each)
```

The MISSING-FUNC handler fires 2.37M times (2 x 1.18M objects). While each call is
inline (no stack growth), the **continuous execution** without returning means the
call chain from sub_82142230 down to sub_8291DF00 holds all its stack frames for the
duration of the object iteration. If N objects is large and sub_8291DF00 calls yield
(`sub_82849918`) mid-loop, the stack includes BOTH the scene iteration state AND
whatever yield triggers.

The actual stack overflow is NOT from the MISSING-FUNC calls themselves but from:
1. **Accumulated stack frames** from the 6-10 level call chain being held open
2. **Particle emitter registration** (sub_825BF8A8) allocating via sub_8218BE28
   which triggers fallback allocator paths that recursively call host page allocators
3. **Concurrent access**: the render thread (sub_821910D0) also holds its own deep
   call chain simultaneously

---

## 6. sub_828507A8 and sub_82A467D8 -- Render Submission

### sub_828507A8 (NOT sub_828507F8)
**File**: `gta4_recomp.55.cpp` line 25263
**PPC stack frame**: 128 bytes
**Calls**: sub_82850458, sub_82856140, sub_82856250

This is a **command buffer submission function**. It allocates a GPU command slot
(`sub_82850458`), writes render state (`sub_82856140`), writes texture state
(`sub_82856250`), and returns. Call depth: 2 levels. Not deep.

### sub_82A467D8 (Present -- hooked in video.cpp)
**File**: `gta4_recomp.71.cpp` line 30184
**PPC stack frame**: 432 bytes (large -- does frame presentation work)
**Called by**: sub_828BF420 (GPU ring buffer flush) and sub_82A4F140 (direct Present)

This function is hooked by `video.cpp` to call `Video::Present()`. The hook
increments the frame counter and syncs the GPU completion counter. Call depth from
the main thread: typically 3-4 levels from sub_821428C8.

---

## 7. Is There a Specific Overflow Frame?

**Yes.** The transition occurs around yield #21000 when the scene render dispatch
activates. The evidence:

| Yield Range | Active Systems | Stack Depth |
|------------|---------------|-------------|
| #0-#13000 | Boot SM, basic update | 4-6 levels (~1KB guest) |
| #13000-#14000 | + text loading, ready signal | 5-7 levels (~1.5KB guest) |
| #14000-#21000 | + render pipeline (loading screen) | 6-8 levels (~2KB guest) |
| **#21000+** | **+ scene graph render dispatch** | **8-10+ levels (~3KB+ guest)** |

The scene graph dispatch adds 2-4 levels of depth (sub_829025A8 -> sub_829022B0 ->
sub_8291E620 -> sub_8291DF00). Combined with the particle emitter registration storm
(sub_825BF8A8, which can add 3-4 more levels through sub_825EDDA0 -> sub_8218BE28 ->
host allocator), the total peak depth can reach **12-14 levels**, pushing the guest
stack to ~3.5KB-4KB per chain.

The overflow is not from a single deep chain but from **concurrent chains**:
- Main thread: sub_82142230 -> sub_821428C8 -> sub_8214C8C8 -> ... -> sub_8291DF00
- Within the same frame: particle emitters registering via sub_825BF8A8 (different path)
- These stack frames accumulate because the game update function (sub_8214C8C8) doesn't
  return until ALL 58 subsystem calls complete

---

## 8. Subsystem Summary

After scene creation, the per-frame tick invokes these subsystem categories:

| Subsystem | Key Functions | Activated By |
|-----------|--------------|-------------|
| **Timer/Clock** | sub_821B38D8, sub_821B3958 | Always |
| **Game Update** | sub_8214C8C8 (58 callees) | Always |
| **Scene/World** | sub_82205850 (10 callees) | Always |
| **Streaming** | sub_821B71B0, sub_8222DAA0, sub_8222DE88, sub_8222E338, sub_8222E760 | After scene creation |
| **DLC** | sub_82145820 | After scene creation |
| **Save/Profile** | sub_826CDEB8 | Always |
| **Network** | sub_822BCA90 | Always |
| **Memory** | sub_821B3A70, sub_821B3B80 | Always |
| **Scene Render** | sub_829025A8 -> sub_8291DF00 (via indirect) | After scene graph populated |
| **Physics/Anim** | sub_828C9980 (via vtable) | After scene graph populated |
| **Audio** | sub_8239AB38, sub_8239AEC8 (from sub_8214C8C8) | After audio init |
| **Particles** | sub_825BF8A8 -> sub_825EDDA0 (one-time storm) | After scene creation |

---

## Source Files Referenced

- `LibertyRecomp/kernel/imports.cpp` -- dead hook at lines 2174-2200
- `LibertyRecomp/gpu/video.cpp` -- sub_82A467D8 Present hook (line 9392)
- `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.0.cpp` -- sub_82142230 (line 5305), sub_821428C8 (line 6281)
- `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.55.cpp` -- sub_82856D48 (line 40812, absorbs 0x82856F08), sub_828507A8 (line 25261), sub_828526A0 (line 30027)
- `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.62.cpp` -- sub_8291DF00 (line 7315), sub_8291E620 (line 8345)
- `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.71.cpp` -- sub_82A467D8 (line 30184)
- `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.58.cpp` -- sub_828BF420 (line 110109, calls Present)
- `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_init.cpp` -- PPCFuncMappings (function table)
- `glue/rexglue-sdk-main/gta4-recomp/gta4_config.toml` -- codegen configuration
- `gta_iv/gta4_functions.txt` -- Ghidra function list (sub_82856F08 NOT present)
- `LibertyRecomp/cpu/ppc_func_decls.h` -- PPC_EXTERN_IMPORT declarations (line 27994)
- `tools/ppc-mcp-server/src/tools/rendering-tools.ts` -- frame sync documentation (line 351)
