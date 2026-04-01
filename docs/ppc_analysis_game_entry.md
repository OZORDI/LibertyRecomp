# GTA IV PPC Game Entry and Main Loop Analysis

> Source: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.0.cpp`
> Addresses are Xbox 360 virtual addresses (XEX load base = 0x82000000).

---

## 1. sub_82140000 — RAGE Init Gate

**Location**: `gta4_recomp.0.cpp` lines 5–62.

### Flow

```
r3 = sub_821B3CE8(0x82D73D44)   // Check some init flag (byte test)
r11 = r3 & 0xFF
if (r11 == 0) return 0           // NOT ready — bail out immediately

// Ready path:
sub_821411D8()                   // Core RAGE system init
sub_82145420(1, 0)               // Secondary init (args r3=1, r4=0)
sub_821412B8()                   // Final init step
return 1                          // Signal: proceed to main loop
```

### Purpose

Guards entry to the main boot sequence. Calls `sub_821B3CE8` to test a readiness byte at `0x82D73D44`. Returns 0 (not ready) or 1 (proceed). The caller must check the return value before calling `sub_82140088`.

---

## 2. sub_82140088 — Main Boot Orchestrator / Main Loop

**Location**: `gta4_recomp.0.cpp` lines 94–307.

### Pre-Loop: Wait for Frame Scheduler

```
do {
    sub_821458B8()               // Check frame scheduler ready
    if (r3 & 0xFF != 0) break   // Ready — enter main loop
    sub_82849918(1)              // Yield one frame (cooperative fiber switch)
} while (r3 & 0xFF == 0)
```

Spins yielding frames until the scheduler signals ready.

### Front-End Phase

```
sub_82142230()                   // FRONT-END STATE MACHINE (see section 3)
sub_82145770(0)                  // Post front-end setup
sub_821B4E38()                   // Resource finalization
```

### Readiness Gate

```
sub_821B39A8()                   // Returns: 0=proceed, nonzero=skip to shutdown
if (r3 != 0) goto teardown
```

If this returns non-zero, the orchestrator skips directly to the shutdown/teardown sequence. Otherwise it falls into the **main game loop**.

### Main Game Loop (loc_821400FC)

Each iteration:

| Step | Function | Purpose |
|-|-|-|
| 1 | `sub_822BCA90(r31)` | Begin frame |
| 2 | `sub_821B38D8()` | Input/controller update |
| 3 | `sub_82142F90()` | Per-frame update (render tick, diagnostics) |
| 4 | `sub_821B3958()` | Frame timing |
| 5 | `sub_822BCA90(r31)` | Second begin-frame marker |
| 6 | `sub_821C0B38(5)` | Render step (arg=5) |
| 7 | `sub_821B5A08(r31)` | Present / flip |
| — | Check `*(r28 - 27388)` | If == 2: check `sub_8225CF80`, call `sub_82359DF8` |
| 8 | `sub_8222E338(r30)` | Scene/world update |
| — | Check flag at r29+21285 | If set: `sub_821DFE40()` then `sub_821EE948(r27)` |
| — | else | `sub_82206D88()` |
| 9 | `sub_821B3B80()` | End frame |
| 10 | `sub_822BCA90(r31)` | Frame boundary |
| 11 | `sub_821B5890(r31)` | Post-present |
| 12 | `sub_821B39A8()` | Exit condition check |
| — | if returns 0: loop back to loc_821400FC |

**r31** = `0x82B30FE0` (main loop context object, lis -32077 + -2080)
**r28** = `0x82B40000` (lis -32076, episode/state base)
**r30** = `0x82BF0000 - 1472` = `0x82BEFA40` (scene struct pointer)

The loop exits when `sub_821B39A8()` returns non-zero (quit/restart signal).

### Shutdown / Teardown Sequence (loc_82140194)

```
sub_82141C80()                   // Shutdown request handler
*(r10 + 21288) = 0               // Clear 64-bit at 0x831D5328 (front-end flag)
sub_8229FA70()                   // Audio shutdown
sub_821FC780()                   // Streaming shutdown
sub_82333878()                   // Physics/world shutdown
sub_821CFEA0()                   // Script VM shutdown
sub_821CF798()                   // Mission shutdown
sub_82297A10()                   // Multiplayer shutdown
sub_822ED668()                   // Network/session shutdown
sub_822BE648()                   // Content manager shutdown
sub_8223CA88()                   // Final cleanup
```

---

## 3. sub_82142230 — Front-End State Machine

**Location**: `gta4_recomp.0.cpp` lines 5303–5999.

### Overview

Controls the GTA IV startup flow from sign-in through world/scene creation. Uses register **r29** as state variable. Runs as a cooperative loop — each iteration yields one frame via `sub_82849918(1)`.

Calls `sub_82241370()` once before the loop (pre-state-machine init: resets state variables, clears slot arrays).

### Loop Structure

```
sub_82241370()                   // Pre-init (once)
r29 = 0                          // Initial state
r25 = 0, r24 = 1                 // Constants: false, true

loc_821422A0:                    // LOOP TOP
    sub_82849918(1)              // Yield frame
    sub_821428C8()               // Per-iteration update (timers, input, render tick)
    if (r29 > 6) goto post_loop  // EXIT when state > 6
    switch(r29) { ... }          // Dispatch
    goto loc_821422A0
```

### State Dispatch Table

| State | Label | Delegated Function | Advance Condition | Next State |
|-|-|-|-|-|
| 0 | `loc_821422E8` | `sub_822414E8` | ret=1 → state 1; ret=2 → skip to 3/4 | 1 |
| 1 | `loc_8214232C` | `sub_8223DDA8` | ret in {1,2} → state 2 | 2 |
| 2 | `loc_82142344` | `sub_8223DEE8` | ret=1 → state 3+; ret=2 → state 8 | 3 or 4 |
| 3 | `loc_82142360` | (inline checks) | All content/slot readiness checks pass | 4 |
| 4 | `loc_821425B4` | `sub_822440F8` | ret=1 → state 7; ret=2 → state 5 | 5 or 7 |
| 5 | `loc_821425D0` | `sub_822422E0` | Unconditional | 6 |
| 6 | `loc_821425DC` | `sub_822438B0` | ret=0 → state 9; ret=2 → state 7 | 7 or 9 |

### Exit Values

| r29 | Meaning | Set By |
|-|-|-|
| 7 | Error exit | State 4 (ret=1), State 6 (ret=2), state 6 inner transitions |
| 8 | Storage cancel | State 2 (ret=2) |
| 9 | Success — scene loaded | State 6 (ret=0) |

### State Detail

**State 0 — Sign-In Check (`sub_822414E8`)**: 7-case internal switch. Scans controller slots 3→0 via `sub_829DB7F8(slot)`. Requires at least one signed-in user. Sign-in check bypassed in recomp via `memory.cpp` line 175: `PPC_STORE_U8(0x831C501C, 1)`.

**State 1 — Storage Device (`sub_8223DDA8`)**: Two-phase async content enumeration. Phase 0: calls `sub_8284ABA0` to begin async enum of up to 75 content slots. Phase 1: polls `sub_8284ABD0` for completion.

**State 2 — Save/Load Check (`sub_8223DEE8`)**: Verifies content system initialization (`0x8317F66C == 2`). Checks `manager+361` byte: if nonzero (save exists) → state 3; if zero (fresh boot) → state 4.

**State 3 — Content Readiness (inline)**: Gate between boot and world init. Loads player struct pointer from `*(0x831E4DD4)`, calls `sub_82219AC0`, checks `struct+361` (signed-in byte). Three paths to advance:
- Fast path: `struct+361 == 0` → immediately advance to state 4.
- No-player path: `sub_821406C8()` returns NULL → compute boot-mode, store at `0x82A95474`, advance to state 4.
- Full check: `sub_8224FA48` returns 0 (resources ready, `0x82BF9B70 == -1`), content-pending `0x82BF9D81 == 0`, `sub_821B6FD0` returns 0, and four slot field "newly-set" transition checks all pass.

**State 4 — Inner Machine (`sub_822440F8`)**: Save device selection and content enumeration. 7-state inner machine (state var at `0x82BF99D4`). Key failure conditions: no controller with storage, `playerIdx (0x82A95478) == -1`, content enum pointer null. Bypassed by hook returning 2 directly.

**State 5 — Game Start (`sub_822422E0`)**: One-shot init. Reads episode index from `0x82B39504` (12=GTA IV, 13=TLAD, 14=TBoGT). Calls `sub_82240480(levelIndex)` to build level name string. Calls `sub_822110E0()` to dispatch "load level" command (type 15, sub-command 52). Writes state=2 at `0x82BF9834` when done. Outer code unconditionally advances to state 6.

**State 6 — Scene/World Loading (`sub_822438B0`)**: 8-state inner machine (state var at `0x82BF9838`). Inner state 2 calls `sub_82242910` (15-state scene creation sub-machine). Inner state 6 calls `sub_826CD808()` to get scene object, stores handle at `0x82B91898`. Returns 0 (success), 1 (in-progress), 2 (error).

### sub_82242910 — Scene Creation Sub-Machine (15 States)

State var at `0x82BF9848`. The actual world construction pipeline:

| State | Purpose |
|-|-|
| 0 | Readiness check |
| 1 | Load prerequisites |
| 2 | Create scene object (`sub_8223CB60`) |
| 3 | Validate scene |
| 4 | Init scene data / player slots |
| 5 | Validate data structures |
| 6 | Load world assets (`sub_8284AAE0`, 7 params) |
| 7 | Init world objects / entity hierarchies |
| 8 | Finalize world, start physics and streaming |
| 9 | Load scripts (`sub_8284ABA0`, 7 params) |
| 10 | Init script VM |
| 11 | Finalize scripts |
| 12 | Load mission/cutscene data (`sub_822417B0`) |
| 13 | Finalize mission, clear flags, reset state |
| 14 | Second validation pass, return 0 (done) |

### Post-Loop Sequence (loc_82142608, all exits r29 > 6)

Shared preamble for all exit values:
1. Clear `0x831D5348` (front-end active flag)
2. Set `0x831D5337` (exit-in-progress flag)
3. `sub_821B5A68` — resource finalization
4. `sub_82220118(levelObj, 0)` — screen/overlay teardown
5. Load scene struct at `0x82BEFA40`, call cleanup
6. Branch to r29-specific handler

**r29=9 (success)**: Calls `sub_82141F00(scenePtr, 1, 1, 0)` (scene activate), then `sub_826CDEB8`, then `sub_8223F800`. On result==2: full scene transition with save data. Otherwise: `sub_82141BE0` (partial scene init). Ends with `sub_8223E028`.

**r29=7 (error)**: Calls `sub_82141F00(scenePtr, 0, 1, 0)` (scene teardown), fade via `sub_8233C230`, then `sub_8223E028`.

**r29=8 (cancel)**: Sign-out check, `sub_826CB600`, `sub_82142B10(1)`, `sub_8223E028`.

`sub_8223E028` (state exit): Writes `0x82BF3A80=1` (machine done), clears dialog pending `0x82BF3CDA`, resets profile index `0x82A95474=-1`, zeros front-end key array `0x82BF3940..394F`.

---

## 4. Scene Pointer at 0x831C2458

**PPC construction**: `lis -31972` → base `0x831C0000`, offset `+9304 (0x2458)`.

**What it is**: The primary scene object pointer in the global scene list array. The scene list array starts at `0x831C23E8`. Scene index 28, entry stride 4 bytes: `0x831C23E8 + (28 × 4) = 0x831C2458`.

**How it gets written** (indirect write chain):

1. `sub_82142230` state 6 inner state 2 runs `sub_82242910` (15-state scene creation machine)
2. `sub_82242910` state 2 calls `sub_8223CB60` — allocates scene object, writes pointer to `0x82BF3A88`
3. Scene initialization calls `sub_829DB8D0` — the scene registration function
4. Registration stores the scene object pointer into `0x831C23E8` at index 28 offset → writes `0x831C2458`

**Read by**: `sub_828C15C8` (render dispatch), `sub_82142F90` hook (diagnostic).

**Render gate**: `sub_828C15C8` checks `*(0x8290B48C) > 0` before dereferencing the scene pointer. If gate value ≤ 0 or pointer is NULL, rendering is skipped. Render path: `r3 = *(0x831C2458)`, `vtable = *(r3)`, `fn = *(vtable + 64)` (vtable slot 16), `fn(scene)`.

---

## 5. Expected Flow After Init Completes

```
sub_82140000() returns 1         // RAGE system ready
  ↓
sub_82140088() waits for scheduler
  ↓
sub_82142230() runs front-end state machine:
  State 0: sign-in verified       (bypassed via 0x831C501C=1)
  State 1: content enumerated
  State 2: save system ready
  State 3: all content ready
  State 4: save device selected   (bypassed via hook → returns 2)
  State 5: level name set, "load level" command dispatched
  State 6: sub_82242910 runs 15 states → scene object created → 0x831C2458 written
           inner state 6: sub_826CD808 returns scene ptr, stored at 0x82B91898
           returns 0 (success) → r29 = 9
  ↓
loc_82142608 exit prologue runs
  ↓
r29=9 path: sub_82141F00(scene, 1, 1, 0) activates scene
  ↓
sub_8223E028 resets front-end state
  ↓
sub_82142230 returns to sub_82140088
  ↓
sub_82145770(0), sub_821B4E38() run
  ↓
sub_821B39A8() returns 0 (proceed)
  ↓
Main loop (loc_821400FC) begins:
  sub_822BCA90 / sub_821B38D8 / sub_82142F90 / sub_821C0B38(5) / sub_821B5A08 per frame
  0x831C2458 != 0 → render dispatch active → game renders
```

---

## 6. Key Global Addresses

| Address | Type | Purpose |
|-|-|-|
| `0x82D73D44` | u8 | RAGE readiness byte (checked by sub_82140000) |
| `0x82A9172C` | u32 | Active player index (-1=none) |
| `0x82A95474` | u32 | Profile index |
| `0x82A95478` | u32 | Player/episode index (0=GTA IV, 1=TLAD, 2=TBoGT) |
| `0x82A9546C` | u32 | Save state result code |
| `0x82BF9B70` | u32 | Readiness flag (-1=ready, other=XAM dialog pending) |
| `0x82BF9834` | u32 | sub_822422E0 state (0=init, 2=done) |
| `0x82BF9838` | u32 | sub_822438B0 inner state (0-7) |
| `0x82BF99D4` | u32 | sub_822440F8 inner state (0-6) |
| `0x82BF9848` | u32 | sub_82242910 scene creation state (0-14) |
| `0x82BF3A88` | u32 | Scene object pointer (written by sub_82242910 states 2-4) |
| `0x82B91898` | u64 | Scene handle (written by sub_822438B0 inner state 6) |
| `0x831C2458` | u32 | Global scene pointer (list index 28, written via scene registration) |
| `0x831D5348` | u8 | Front-end active flag (cleared on exit) |
| `0x831D5337` | u8 | Exit-in-progress flag |
| `0x831E4DD4` | u32 | Frontend manager / player struct pointer |
| `0x8290B48C` | u32 | Render gate value (must be > 0 to dispatch render) |
