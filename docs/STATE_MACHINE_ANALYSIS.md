# GTA IV Front-End State Machine — Complete Analysis

> Auto-generated research document. Each section researched from the v8 XEX recompiled code.

## Overview

The front-end state machine (`sub_82142230`) controls the game's startup flow from sign-in
through to scene creation. It uses register r29 as a state variable (0-9) and loops until
r29 > 6 (exit).

**Current status**: States 0-3 pass. State 4 inner machine (`sub_822440F8`) exits with
error because `0x82A95478` (player/episode index) = -1.

**CRITICAL BUG FOUND**: The diagnostic hooks in `imports.cpp` read inner state variables
from wrong addresses. **Four** addresses are wrong (see Section 11 errata). The garbage value
`941686896` (`0x3828C470`) was from reading the wrong memory location.

---

## Table of Contents

1. [Outer State Machine (sub_82142230)](#1-outer-state-machine-sub_82142230)
2. [State 0: Sign-In Check (sub_822414E8)](#2-state-0-sign-in-check-sub_822414e8)
3. [State 1: Storage Device (sub_8223DDA8)](#3-state-1-storage-device)
4. [State 2: Save/Load (sub_8223DEE8)](#4-state-2-saveload-check)
5. [State 3: Content Readiness](#5-state-3-content-readiness)
6. [State 4: Inner State Machine (sub_822440F8)](#6-state-4-inner-state-machine-sub_822440f8)
7. [State 5: Game Start (sub_822422E0)](#7-state-5-game-start-sub_822422e0)
8. [State 6: Scene/World Loading (sub_822438B0)](#8-state-6-sceneworld-loading-sub_822438b0)
9. [Exit Path (r29 > 6)](#9-exit-path-r29--6)
10. [Scene Pointer (0x831C2458)](#10-scene-pointer-0x831c2458)
11. [Key Global Addresses](#11-key-global-addresses)
12. [Helper Functions](#12-helper-functions)
13. [CPool::ProcessAll Null Vtable Issue](#13-cpoolprocessall-null-vtable-issue)
14. [Render Dispatch Chain](#14-render-dispatch-chain)
15. [XAM Sign-In Emulation Flow](#15-xam-sign-in-emulation-flow)
16. [Per-Frame Update (sub_82142F90)](#16-per-frame-update-sub_82142f90)
17. [Scene Creation Sub-Machine Deep Dive (sub_82242910)](#17-scene-creation-sub-machine-sub_82242910--deep-dive)
18. [Platform/Hardware Init (sub_82240B08)](#18-sub_82240b08--platformhardware-initialization)
19. [Scene Ready Query (sub_8223CB60)](#19-sub_8223cb60--scene-ready-query)
20. [Level/Scene Setup (sub_82240480)](#20-sub_82240480--levelscene-setup)
21. [Pre-State-Machine Init (sub_82241370)](#21-sub_82241370--pre-state-machine-initialization)
22. [System Readiness Check (sub_8223DAA0)](#22-sub_8223daa0--system-readiness-check)
23. [Error Recovery Loop (State 7)](#23-error-recovery-loop-state-7)
24. [0x82BF9834 Write Chain Summary](#24-summary-of-0x82bf9834-write-chain)

**Part II: RexGlue Integration Analysis**

25. [RexGlue Architecture Overview](#25-rexglue-architecture-overview)
26. [Root Cause: Immediate Device Readiness](#26-root-cause-immediate-device-readiness)
27. [Content Manager: Synchronous By Design](#27-rexglue-content-manager-synchronous-by-design)
28. [XAM Dialog System: Complete But Never Invoked](#28-xam-dialog-system-complete-but-never-invoked)
29. [Threading: No Frame Synchronization](#29-threading-no-frame-synchronization)
30. [sub_8223DB20: Notification Processing Gate](#30-sub_8223db20-notification-processing-gate)
31. [sub_8284xxxx: Graphics Device Management](#31-sub_8284xxxx-graphics-device-management)
32. [Save System Integration](#32-save-system-integration)
33. [Synthesis: Why RexGlue Doesn't Prevent Error 34](#33-synthesis-why-rexglue-doesnt-prevent-error-34)

**Part III: Fix Research — Detailed Feasibility Analysis**

34. [Fix #1: Hook sub_8223DAA0 (RECOMMENDED)](#34-fix-1-hook-sub_8223daa0-recommended--lowest-risk)
35. [Fix #2: Pre-Write 0x82BF9834=6](#35-fix-2-pre-write-0x82bf98346-already-partially-applied)
36. [Fix #3: RexGlue signin_state() Delay](#36-fix-3-rexglue-signin_state-delay-upstream-fix)
37. [Fix #4: Recompile sub_821200D0 (NOT PRACTICAL)](#37-fix-4-recompile-sub_821200d0-not-practical)
38. [Fix #5: Fix APC Delivery (CRITICAL BUG)](#38-fix-5-fix-apc-delivery-critical-bug-found)
39. [Fix #6: Hook sub_82242910 State Transition](#39-fix-6-hook-sub_82242910-state-transition-alternative)
40. [Fix #7: Notification Queue (NOT AN ISSUE)](#40-fix-7-notification-queue-not-an-issue)
41. [Fix #8: sub_82849918 Yield (NOT ROOT CAUSE)](#41-fix-8-sub_82849918-yield-not-the-root-cause)
42. [Fix #9: Hook sub_826CD808 Selectively](#42-fix-9-hook-sub_826cd808-selectively-viable-alternative)
43. [Fix Priority Ranking](#43-fix-priority-ranking)

---

## 1. Outer State Machine (sub_82142230)

### Overview

`sub_82142230` is the front-end state machine that controls the GTA IV boot sequence from sign-in through to scene creation. It runs as a synchronous loop, spinning through states 0-6. Each iteration yields to the game's frame scheduler via `sub_82849918(ctx, 1)` (a co-routine yield / fiber switch), making the loop execute one state-step per frame.

It is called once from `sub_82140088` (the boot orchestrator). The per-frame callback `sub_82142F90` runs independently and handles render/update ticking while the state machine loop is active.

### Loop Structure

```
sub_82241370(ctx)             // pre-state-machine init (runs once)
r29 = 0                       // initial state

loc_821422A0:                 // <-- LOOP TOP
    sub_82849918(ctx, 1)      // yield frame (cooperative scheduling)
    sub_821428C8(ctx)         // per-iteration update (timers, input, render tick)
    if (r29 > 6) goto post_loop   // EXIT: state exceeded valid range
    switch (r29) { ... }      // dispatch current state
    goto loc_821422A0         // loop back
```

The loop runs indefinitely until `r29` is set to a value greater than 6 (specifically 7, 8, or 9).

### State Dispatch Table

| State | Entry Label | Function Called | Advance Condition | Next State |
|-------|-------------|-----------------|-------------------|------------|
| 0 | `loc_821422E8` | `sub_822414E8` | ret=1 → advance; ret=2 → skip to 3/4 | 1 (or 3/4) |
| 1 | `loc_8214232C` | `sub_8223DDA8` | ret ∈ {1,2} → advance | 2 |
| 2 | `loc_82142344` | `sub_8223DEE8` | ret=1 → advance; ret=2 → state 8 (exit) | 3 or 4 |
| 3 | `loc_82142360` | (complex checks) | All slot field checks pass | 4 |
| 4 | `loc_821425B4` | `sub_822440F8` | ret=1 → state 7; ret=2 → state 5 | 5 or 7 |
| 5 | `loc_821425D0` | `sub_822422E0` | Unconditional advance | 6 |
| 6 | `loc_821425DC` | `sub_822438B0` | ret=0 → state 9; ret=2 → state 7 | 7 or 9 |

### Exit Values

| r29 | Meaning | Set By |
|-----|---------|--------|
| 7 | Error exit | State 4 (ret=1), State 6 (ret=2) |
| 8 | Storage cancel | State 2 (ret=2) |
| 9 | Success — scene loaded | State 6 (ret=0) |

### Post-Loop Sequence (`loc_82142608`)

1. Clears `[0x831D5348]` to 0 (front-end active flag)
2. Sets `[0x831D5337]` to 1 (exit-in-progress flag)
3. Calls `sub_821B5A68` — resource finalization
4. Calls `sub_82220118` — screen/overlay teardown
5. Loads scene struct at `0x82BEFA40`, calls cleanup functions
6. Branches to r29-specific exit handler (7, 8, or 9)
7. All paths eventually call `sub_8223E028` — state exit function (resets profile index to -1, clears flags)

---

## 2. State 0: Sign-In Check (sub_822414E8)

### Overview

`sub_822414E8` is the sign-in gatekeeper. It verifies that at least one Xbox 360 user profile is signed in before allowing the game to proceed. The function is a 7-case internal switch (cases 0-6).

### Preamble

1. Initialize count=0, last_slot=-1
2. Read `[0x82A9172C]` (active player index). If not -1, skip scan (already have a user)
3. Otherwise, loop slots 3→0 calling `sub_829DB7F8(slot)` for each controller. Count signed-in controllers.
4. Call `sub_821B4108()` — active player count. If returns 0: show message dialog #51, return 0 (loop).

### Return Values

- `r3 = 0`: Stay in state 0
- `r3 = 1`: Advance to state 1
- `r3 = 2`: Skip to state 2 (dialog #48 path)

### Recomp Handling

Bypassed via `memory.cpp` line 175: `PPC_STORE_U8(0x831C501C, 1)` — sets player 0 sign-in byte to non-zero, making `sub_821B4108()` return 1.

---

## 3. State 1: Storage Device

### Overview

State 1 handles storage device selection and content enumeration. Calls `sub_8223DDA8`, which is a two-phase asynchronous enumeration.

### Key Globals

| Address | Type | Meaning |
|---------|------|---------|
| `0x82BF9828` | u32 | Internal sub-state (0=start enum, 1=poll completion) |
| `0x82BF3D90` | u32 | Selected storage device ID (-1=none) |
| `0x82BF3D94` | u32 | Overlapped result pointer |
| `0x82BF3D98` | buf | Content enumeration buffer (75 slots × 308 bytes) |

### Phase 0 — Start Enumeration
1. Call `sub_8223DB20()` — notification pump (checks `XN_SYS_STORAGEDEVICESCHANGED`)
2. If device ID is -1, return 1
3. Call `sub_8223D2F0()` — reset content arrays
4. Call `sub_8284ABA0(contentMgr, deviceId, 1, buf, 75)` — begin async enumeration
5. Set phase=1

### Phase 1 — Poll Completion
1. Call `sub_8284ABD0()` — check if enumeration complete. If not, return 0 (keep polling)
2. Call `sub_8284ABF8()` — finalize enumeration
3. Check result status. Return 2 on success (advance to state 2)

### Return Values
- `0`: Still working (keep polling)
- `1`: Device changed/error (back to state 0)
- `2`: Enumeration complete (advance to state 2)

---

## 4. State 2: Save/Load Check

### Overview

State 2 handles save game detection. Calls `sub_8223DEE8`, which verifies content system initialization.

### Key Globals

| Address | Type | Meaning |
|---------|------|---------|
| `0x82BF98F4` | u32 | Internal sub-state (0=check system, 1=check content init) |
| `0x82A9547D` | u8 | Save-in-progress flag |
| `0x8317F66C` | u32 | Content init state (2=fully initialized) |

### Success Path (`loc_821422FC`)

After state 2 succeeds, the code loads the frontend manager from `[0x831E4DD4]`, calls `sub_82219AC0`, checks byte at `manager+361`:
- If byte == 0 (no prior save): state → 4
- If byte != 0 (save data exists): state → 3

### Return Values
- `0`: Still working
- `1`: Save system ready (advance)
- `2`: Error (jump to state 8)

---

## 5. State 3: Content Readiness

State 3 is the core gate between "game boot" and "world init." It determines whether all content — player profile, resource packs, multiplayer session state, Xbox LIVE credentials, and system-level storage — is ready before advancing to state 4.

### Three Paths to State 4

**Path A: Fast Path** (`byte 361 == 0`): World object has no content loaded → unconditionally advance to state 4.

**Path B: No-Player Path** (`sub_821406C8` returns NULL): No player signed in → reset sub_state, compute boot-mode via `sub_8221B198`, store at `0x82A95474`, advance to state 4.

**Path C: Full Check Path** (player exists): Must pass through:
1. `sub_8224FA48` returns 0 (resources ready — `0x82BF9B70` == -1)
2. Content-enum-pending flag `0x82BF9D81` == 0
3. `sub_821B6FD0` returns 0 (no multiplayer notifications)
4. All four slot field "newly set" transition checks pass

### The "Newly Set" Transition Detector Pattern

Four field pairs on the player slot structure are checked:

| Pair | Current Offset | Shadow Offset | Field Name |
|------|---------------|---------------|------------|
| 1 | +68 | +148 | Content package handle |
| 2 | +72 | +152 | DLC content handle |
| 3 | +4 | +84 | Profile data pointer |
| 4 | +56 | +136 | Storage device selection |

Logic: **"newly set" = (current != 0 AND shadow == 0)**. The fourth pair (storage device) triggers the actual state 4 transition.

### sub_8224FA48 — Resource Readiness Check

```
value = *(uint32_t*)0x82BF9B70
result = (value == -1) ? 0 : 1
```
Returns 0 when value is -1 (ready — "no XAM dialog pending"). Returns 1 when any other value (not ready).

**Why -1 is correct**: The natural default of -1 means "no XAM dialog pending, advance." Writing 1 was previously tried and was WRONG — it made the state machine loop forever in dialog-handling mode.

### Xbox LIVE Async Checks

If any content check fails, execution reaches `loc_82142504`:
- `sub_826CBD20()` — checks two async structures for completion (at `0x833BF778` and `0x833CAB48`)
- `sub_826CBD08()` — checks `*(0x833BF66C) == 2`
- If either returns nonzero → **regress to state 2**

### System Readiness Check (`loc_82142530`)

- `sub_8223DAA0()` returning 1 with the system-ready flag set → **FULL RESET to state 0**
- `sub_8223DAA0()` returning 0 → set flag for next iteration, stay in state 3

---

## 6. State 4: Inner State Machine (sub_822440F8)

### Overview

`sub_822440F8` implements the save device selection and content enumeration inner state machine. It is the **current blocker** — returns 1 (error) even after fixing playerIdx at `0x82A95478` to 0.

### Inner State Variable

**CRITICAL**: The inner state is at `0x82BF99D4` (NOT `0x829F99D4` as in imports.cpp).

Computed from: `lis r28, -32064` → `r28 = 0x82C00000`, offset `-26156` → `0x82BF99D4`.

### Return Value Convention

- `r3 = 0`: Still running
- `r3 = 1`: Error/abort → outer state sets r29=7
- `r3 = 2`: Success → outer state sets r29=5

### The 7 Inner States

#### State 0: Profile Detection and Save Enumeration Init

1. Load save manager from `*(0x831E4DD4)`
2. Call `sub_82219AC0` (reset save lock)
3. Check `manager+361` (isSignedIn byte)
4. If signed in: resolve profile index from `0x82A95474` or via `sub_8221B198`
5. Call `sub_8223DAA0()` — has storage device been selected?
6. If no device: call `sub_82241428()` — find active controller with storage
   - **If returns ≤ 0: RETURNS 1 (ERROR)** — this is the most likely failure path
7. If device available: check content listener, call `sub_82241428` again

#### State 1: Wait for Storage Device Validation

- Read `*(0x82BF3D94)` (contentEnumReady)
- **If == 0: RETURNS 1 (ERROR)**
- If valid: set innerState=2, call `sub_822409F0()` (starts content enumeration)

#### State 2: Wait for Content Enumeration

- Call `sub_82243F00(1)` — content enumeration poll
- If returns 0 (found a slot): check `*(0x82A95478)` (playerIdx)
  - **If playerIdx == -1: RETURNS 1 (ERROR)** ← critical check
- If playerIdx valid: set innerState=4

#### State 3: Check XNotify and Compute Save Path

- Check `*(0x82BF9864)` (contentDeviceId) — if ≤ 0, return 1
- Call `sub_8223F9F0(52, 0, &outByte)` — content enumeration with ID 52
- On completion: set `hasEnumeratedSaves = 1`

#### State 4: Save Slot Read/Write

- Call `sub_8223DB20()` — check for device removal notification
- Compute save slot offset: `playerIdx * 308`
- Call `sub_82242678(playerIdx, &outByte)` — save read/write
- If save loaded successfully: errorCode=50, innerState=5
- If no save data: innerState=6

#### State 5: Save Write Completion

- Call `sub_82242608()` — poll async save
- Returns 1 when complete

#### State 6: No-Save Completion Path

- Check for device removal
- Call `sub_8223DC10(0)` — cleanup/reset
- **Returns 2** (completed successfully → outer state advances to r29=5)

### Every Path That Returns 1 (Error)

| State | Condition |
|-------|-----------|
| 0 | `sub_82241428() <= 0` (no controller with storage) |
| 0 | `contentSelected (0x82BF984E) != 0` |
| 0 | `byte[gameStateObj+361] != 0` after checks |
| 1 | `contentEnumReady (0x82BF3D94) == 0` |
| 2 | `sub_82243F00(1)` returns 2 (error) |
| 2 | `playerIdx (0x82A95478) == -1` after slot found |
| 3 | `contentDeviceId (0x82BF9864) <= 0` |
| 4 | `sub_8223DB20()` returns nonzero (device removal) |
| 5 | `sub_82242608()` returns nonzero (save complete) |
| 6 | `sub_8223DB20()` returns nonzero (device removal) |

---

## 7. State 5: Game Start (sub_822422E0)

### Overview

State 5 is a one-shot fire-and-forget initialization step. The caller at `loc_821425D0` **unconditionally** sets r29=6 after calling `sub_822422E0`, without checking the return value.

### Internal 3-Phase State Machine

State variable at **`0x82BF9834`** (NOT `0x829F9834` — same wrong-address bug as the others).
Computed from: `lis r30, -32064` → `0x82C00000`, offset `-26572` → `0x82BF9834`.

Second state counter at **`0x82BF9838`** (offset `-26568`).

### Addresses Accessed

| Address | Type | Purpose |
|---------|------|---------|
| `0x82BF9834` | u32 | Primary state counter (0=init, ≥1=done) |
| `0x82BF9838` | u32 | Secondary state counter (written to 1 on init) |
| `0x82A9547C` | u8 | Episode/DLC flag byte (0=single-player, ≠0=has DLC) |
| `0x82B39504` | u32 | Episode index (1=TLAD, 2=TBoGT, else=main GTA IV) |
| `0x82BF981F` | u8 | Flag byte (cleared to 0 during init) |

### Entry Check

```
state = *(0x82BF9834)
if (state >= 1) return 2    // already initialized, skip
```

### Level Selection Logic

**Phase 0 (Initial entry)**:
1. Check `*(0x82BF9838)` — if non-zero, skip to end (already started)
2. Load DLC flag from `0x82A9547C`:
   - If `== 0` (single-player mode): read episode index from `0x82B39504`:
     - Episode `== 1`: level index **13** (The Lost and Damned)
     - Episode `== 2`: level index **14** (The Ballad of Gay Tony)
     - Else: level index **12** (Grand Theft Auto IV main game)
   - If `!= 0` (has DLC): complex string copying for DLC-specific level name setup

3. Call `sub_82240480(levelIndex)` to build the level name string
4. Clear flag at `0x82BF981F` to 0
5. Write 1 to `0x82BF9838`, write 2 to `0x82BF9834`
6. Call `sub_822110E0()` to dispatch scene load

**Phase 1+**: Returns 2 immediately (already initialized).

### sub_82240480 — Level/Scene Setup Function

Encodes episode-specific resource loading:
- Episode 1 (TLAD): Writes "TLAD" as 16-bit characters for DLC resource paths
- Episode 2 (TBOGT): Writes "TBoGT" as 16-bit characters
- Calls `sub_821D1128(670)` to queue mission dispatcher
- Calls `sub_8221FD88()` for file system handlers, `sub_82221768()` for file lengths

### sub_822110E0 — Scene Command Dispatcher

Issues a scene loading command:
1. `sub_825EC320(buf, 15)` — Initialize command (type 15 = "load level")
2. `sub_82210C70(buf, 52)` — Dispatch (sub-command 52 = "begin streaming")
3. `sub_825EC370(buf)` — Finalize command

---

## 8. State 6: Scene/World Loading (sub_822438B0)

### Overview

State 6 polls the scene/world loading progress. Returns 0 (success), 1 (in progress), or 2 (error).

### State Variable

At `0x82B91838` (computed from `r31 + (-26568)` where `r31 = 0x82B98000`).

### Inner State Machine (8 states, 0-7)

| State | Purpose | Key Action |
|-------|---------|------------|
| 0 | Idle | Returns 0 immediately |
| 1 | Initialize Loading | Sets flags, advances to state 2 |
| 2 | Scene Creation | Calls `sub_82242910` (15-state sub-machine) |
| 3 | World Load Pass 1 | Calls `sub_82240F80(0)`, checks for "SAVE" marker |
| 4 | Post-Load Setup | Calls `sub_822446E8`, `sub_8223F740` |
| 5 | World Load Pass 2 | Calls `sub_82240F80(1)` |
| 6 | **Scene Finalization** | Captures scene handle, stores at `0x82B91898` |
| 7 | Error Cleanup | Calls `sub_8223CC68`, `sub_82242608` |

### State 6 (Finalize) — Scene Handle Capture

1. Timeout check: counter at `0x82B83934` must reach 3000 ticks
2. Call `sub_8223CAD8()` — scene ready notification
3. Call `sub_826CD808()` — get scene/world object pointer
4. If valid scene: load 8-byte handle via `sub_829DBAA8`, store at `0x82B91898`
5. **Return 0 (SUCCESS)** — only path that returns success

### Return Value Protocol

| r3 | Meaning | Outer State Transition |
|----|---------|----------------------|
| 0 | Success | r29 = 9 (proceed to gameplay) |
| 1 | In progress | r29 unchanged, loop back |
| 2 | Error | r29 = 7 (error/cleanup) |

### sub_82242910 — Scene Creation Sub-Machine (15 States)

Called from State 6 inner state 2. This is the function that actually creates the game world.

**State counter**: `0x82BD9AF8` (lis -32064 - 26552)

| State | Purpose | Key Function |
|-------|---------|-------------|
| 0 | Init & Ready Check | Verify scene subsystem ready |
| 1 | Load Prerequisites | Load resource packages |
| 2 | **Create Scene Object** | `sub_8223CB60` — actual scene allocation |
| 3 | Validate Scene | Sanity check on scene object |
| 4 | Init Scene Data | Allocate metadata, player slots (nested 5-case machine) |
| 5 | Validate Data | Verify data structures initialized |
| 6 | **Load World Assets** | `sub_8284AAE0` — main world model/collision load (7 params) |
| 7 | Init World Objects | Create entity instances, object hierarchies |
| 8 | Finalize World | Complete world init, start physics/streaming |
| 9 | **Load Scripts** | `sub_8284ABA0` — load mission script bytecode (7 params) |
| 10 | Init Script VM | Initialize script virtual machine (complex nested machine) |
| 11 | Finalize Scripts | Final script validation |
| 12 | Load Mission Data | `sub_822417B0` — load mission/cutscene content |
| 13 | Finalize Mission | Clear flags at `0x82A9546E`, `0x82BF+15578`, reset state |
| 14 | Complete | Validation pass, return 0 (success) |

**Key addresses**:
| Address | Purpose |
|---------|---------|
| `0x82BF3A88` | Scene object pointer (written during states 2-4) |
| `0x82BFF760` | Context handle |
| `0x82BFF768` | Scene metadata |
| `0x82BDA3BC` | Player slot |
| `0x82A9546C` | Error code (values: 6, 7, 8, 9, 14, 15, 16, 33, 34) |

**Return values**: 0 = complete (scene ready), 1 = continue (call again next frame), 2 = error (check error code at `0x82A9546C`)

---

## 9. Exit Path (r29 > 6)

### Common Exit Prologue (loc_82142608)

All exit values (7, 8, 9) share this preamble:
- Clear front-end active flag at `0x831D5348`
- Set exit-in-progress flag at `0x831D5337`
- Call `sub_821B5A68`, `sub_82220118`, `sub_8222DB48`, `sub_8214AD88`
- If inner scene pointer exists, mark scene complete at offset+1376

### r29 = 7 (Error Exit)

Calls `sub_82141F00(scenePtr, 0, 1, 0)` — scene teardown. This is a straight-line function (no loops/blocking). Then renders a fade transition via `sub_8233C230` and calls `sub_8223E028` (state exit). Returns.

### r29 = 8 (Storage Cancel)

Checks sign-out via `sub_826CBD08`. Calls `sub_826CB600` (get save count), `sub_82142B10(1)`, `sub_8223E028`. Returns.

### r29 = 9 (Success)

Calls `sub_82141F00(scenePtr, 1, 1, 0)` — scene activate. Then `sub_826CDEB8` (post-transition check). If activation succeeded, calls `sub_8223F800` to check scene state. If result == 2: full scene transition with save data. If result != 2: calls `sub_82141BE0` (partial scene init). All paths call `sub_8223E028` and return.

### sub_8223E028 — State Exit Function

Writes performed:
| Address | Value | Meaning |
|---------|-------|---------|
| `0x82BF3A80` | 1 | State machine completed |
| `0x82BF3CDA` | 0 | Dialog pending cleared |
| `0x82A95474` | -1 | Profile index reset |
| `0x82BF3940..394F` | 0 | Front-end key array zeroed |

If `sub_8223DAA0` indicates multiplayer dialog active, dismisses it via `sub_8223DB90(0)`.

---

## 10. Scene Pointer (0x831C2458)

### Address and Reader

The scene pointer at `0x831C2458` is computed as `lis(-31972) + 9304`. It is read by `sub_828C15C8` (the core render dispatch function) via vtable dispatch:

```
r3 = *(0x831C2458)          // scene pointer
vtable = *(r3)               // vtable pointer
fn = *(vtable + 64)          // vtable[16] = Render method
fn(scene)                    // indirect call
```

### How It Gets Written

The scene pointer is NOT written by a direct `stw` to this address. It is written **indirectly through the scene registration system**:

1. State 6 inner state 6 calls `sub_826CD808()` → `sub_829DBAA8(r3)` → loads 8-byte handle → stores at `0x82B91898`
2. `sub_826CD5A8` (scene initialization) calls `sub_829DB8D0` — the scene registration function
3. Registration writes the scene object pointer into the scene list array at `0x831C23E8` through an indexed store
4. For the primary scene (index 28, offset 112): `0x831C23E8 + 112 = 0x831C2458`

### Render Gate

`sub_828C15C8` checks `*(0x82B0B48C) > 0` before dereferencing the scene pointer. If the gate is ≤ 0 or the scene pointer is NULL, rendering is skipped.

---

## 11. Key Global Addresses

### Address Verification Errata

**CRITICAL**: Four addresses in `imports.cpp` are WRONG:

| imports.cpp Address | Actual Address | Discrepancy |
|---------------------|----------------|-------------|
| `0x829F99D4` (sub_822440F8 inner state) | **`0x82BF99D4`** | Wrong lis base |
| `0x829F9858` (sub_822438B0 inner state) | **`0x82BF9838`** | Wrong lis base + offset |
| `0x82A03D94` (content enum result) | **`0x82BF3D94`** | Wrong lis base |
| `0x829F9834` (sub_822422E0 state var) | **`0x82BF9834`** | Wrong lis base |

### Reference Table

| Address | Type | Default | Purpose |
|---------|------|---------|---------|
| `0x82A9172C` | u32 | 0 | Active player index (-1=none, 0-3=controller slot) |
| `0x82A95474` | u32 | 0 | Profile index (set by game during state 4) |
| `0x82A95478` | u32 | 0 | Player/episode index (0=GTA IV, 1=TLAD, 2=TBoGT) |
| `0x82A9546C` | u32 | 0 | Save state result code (29, 31, 33, 50) |
| `0x82BF9B70` | u32 | -1 | Readiness flag (-1=no dialog pending=advance) |
| `0x82BF9D81` | u8 | 0 | Warning dialog active flag |
| `0x82BF9834` | u32 | 0 | sub_822422E0 state counter (0=init, ≥1=done) |
| `0x82BF99D4` | u32 | 0 | sub_822440F8 inner state (0-6) |
| `0x82BF9838` | u32 | 0 | sub_822438B0 inner state (0-7) |
| `0x82BF99D0` | u32 | 0 | Content state flag (0=idle, 1=loading, ≥2=DLC) |
| `0x82BF982C` | u32 | 0 | Content slot index (incremented per poll) |
| `0x82BF3D98` | buf | 0 | Content slot array (75 × 308 bytes) |
| `0x82BF3B08` | buf | 0 | DLC content slot data (15 slots) |
| `0x82DF3910` | u32 | 0 | XNotify handle (XNotifyGetNext) |
| `0x82DF3D70` | u32 | -1 | System readiness flag (-1=skip check) |
| `0x82BD9AF8` | u32 | 0 | sub_82242910 scene creation state (0-14) |
| `0x82BF3A88` | ptr | 0 | Scene object pointer (written by sub_82242910) |
| `0x82B39504` | u32 | 0 | Episode index (1=TLAD, 2=TBoGT, else=main) |
| `0x82A9547C` | u8 | 0 | Episode/DLC flag byte |
| `0x82BF3D90` | u32 | -1 | Storage device ID |
| `0x82BF3D94` | u32 | 0 | Content enumeration result pointer |
| `0x82BF984E` | u8 | 0 | Content selected flag |
| `0x82BF9858` | u32 | 0 | Saved device ID |
| `0x82BF9864` | u32 | 0 | Content device ID from sub_82241428 |
| `0x831C2458` | ptr | 0 | Scene list pointer (head of active scene) |
| `0x831C501C` | u8 | 0 | Player 0 sign-in state byte (written 1 by memory.cpp) |
| `0x831E4DD4` | ptr | 0 | Front-end manager / save manager object pointer |
| `0x831D5337` | u8 | 0 | Exit-in-progress flag |
| `0x831D5348` | u8 | 0 | Front-end active flag |
| `0x82BF3A80` | u8 | 0 | State machine completed flag |
| `0x82BAFA44` | ptr | 0 | Content manager pointer |
| `0x82B29F18` | struct | 0 | Player slot array (4×188 bytes) |

---

## 12. Helper Functions

### sub_8223DAA0 — System Readiness Check

- **Returns**: 1 = system ready, 0 = not ready
- **Address**: Handle at `0x82DF3D70` (lis -32065 + 15760)

**Algorithm**:
1. Pre-initialize `*(0x82DF3D70)` to -1
2. Call `sub_826CD808()` twice — if first call returns 0 → return 0
3. Call `sub_829DB9B8()` — store result to `0x82DF3D70`
4. If result < 0: return 1 (READY — negative = valid device index)
5. If result ≥ 0: call `sub_82A12188` (XamUserGetSigninState)
   - If returns ≠ 0: return 1 (READY — user signed in)
   - If returns == 0: set `0x82DF3D70` to -1, return 0 (NOT READY)

### sub_8223DB20 — XNotify Pump / Device Removal Check

- **Returns**: 1 = device state changed (caller should abort/re-check), 0 = stable (continue)
- **XNotify handle**: `0x82DF3910` (lis -32065 + 14640)

**Algorithm**:
1. Call `XNotifyGetNext(handle, 10, &id, &param)` — polls for `XN_SYS_SIGNINCHANGED` (0x0A)
2. **If XNotifyGetNext returns 0** (no notifications): return 0 immediately
3. **If notification received**: check `*(0x82DF3D70)` — if -1, return 0 (skip check)
4. Otherwise: call `sub_8223DAA0()` — if returns ≠ 0, return 1 (device changed)

**Corrected understanding**: When notifications are exhausted (XNotifyGetNext returns 0), sub_8223DB20 returns 0, and state 4 treats this as **success** (stable, continue). The finite notification supply is **NOT a blocker** — returning 0 is the correct "all clear" path.

### sub_82241428 — Controller Storage Device Detector

- **Returns**: Controller index (1-15), or -1 if no controller needs storage assignment
- **Caller check**: `cmpwi r3, 0; ble -> error` — returns ≤ 0 means FAILURE in state 4

**Algorithm**:
1. Load controller data struct pointer from `0x82BAFA44`
2. Validate byte `controller[+16]` ≠ 0 (controller exists). If 0 → return -1
3. Call `sub_822051E0(controller, 500)` — pre-check if storage interrogation should proceed. If returns 0 → return -1
4. Call `sub_82205180(controller, 500)` → bitmask where bit N = controller N **already has** storage
5. Load active controller mask from `0x831E4DD4`, process via `sub_82219F98(mask)`
6. Loop 15→0: for each controller slot, check `(1 << index) & active_mask != 0` AND `(1 << index) & storage_mask == 0`
   - First match (active controller WITHOUT storage) → save index
7. Call `sub_82205330(controller, 500, combined_mask)` — finalize storage configuration
8. Return saved controller index, or -1 if none found

**Key insight**: This finds a controller that is active but does NOT yet have storage mapped. The save state machine needs this to assign a storage device. In the recomp, if no controller slot meets this criteria, state 4 fails immediately.

**Globals**: `0x82BAFA44` (controller struct ptr), `0x831E4DD4` (active controller bitmask)

### sub_821406C8 — Player Slot Accessor

- Returns pointer to active player's 188-byte slot at `0x82B29F18 + (index * 188)`, or NULL if index is -1
- Active player index read from `0x82A9172C`

### sub_8224FA48 — Resource Readiness Check

- Reads `0x82BF9B70`, returns 0 when value is -1 (ready), 1 otherwise
- Uses `cntlzw` + `rlwinm` + `xori` idiom for `(value == -1) ? 0 : 1`

### sub_821B6FD0 — Multiplayer Notification Check

- Returns 1 if multiplayer notification pending, 0 if none
- Checks session manager at offset +4064 via `sub_8284D2A0`

### sub_82243F00 — Content Loading/Streaming Poll

- **Returns**: 0 = all slots exhausted (done), 1 = still loading, 2 = error
- **Slot stride**: 308 bytes per content slot

**State variables**:
| Address | Purpose |
|---------|---------|
| `0x82BF982C` | Current slot index (incremented each call) |
| `0x82BF99D0` | Content state flag (0=idle, 1=loading, ≥2=DLC phase) |
| `0x82BF3D98` | Main content slot array base (75 slots max) |
| `0x82BF3EA0` | Slot status bytes (slot[i]+264, 0=inactive) |
| `0x82BF3B08` | DLC content slot data (15 slots max) |
| `0x82BF93C4` | Max content slot index (loaded when DLC mode) |

**Algorithm**:
1. Validate current slot index from `0x82BF982C` against bounds
2. Check slot status byte at `slot_array + 264 + (index × 308)` — skip if 0
3. Read content state flag from `0x82BF99D0`:
   - **State 1**: Call `sub_82240798(slot_index)` — poll individual slot (0=done, 1=loading, 2=error)
   - **State ≥ 2**: DLC detection — `strncmp(slot+264, "DLC", 5)`. If match: call `sub_82242678` for DLC mounting
4. Advance to next active slot, return 0 when all slots exhausted

**Stall risk**: If `sub_82240798` or `sub_82242678` persistently return 1 (loading), this polls forever

### sub_82242678 — Save Game Enumeration

- **Returns**: 0 = done, 1 = still enumerating, 2 = error
- 2-state sub-machine: state 0 = start enumeration, state 1 = poll completion

### sub_82242608 — Save Completion Check

- Calls `sub_8223F9F0(timer, 0, &outBool)` to poll async save
- Returns 1 when complete, 0 when in progress

### sub_82141F00 — Scene Transition Handler

- Straight-line function (no loops/blocking)
- Calls ~15 shutdown/init functions
- Returns boolean (1=success, 0=profile validation failed)

### sub_82141BE0 — Success Handler (r29=9)

- Snapshots current state, finalizes game session
- Clears error display flag at `0x828E547C`

---

## 13. CPool::ProcessAll Null Vtable Issue

### Root Cause

`sub_8291DF00` iterates a `CPool` of streaming objects (768 bytes/element). For each active slot, it reads `*(uint32_t*)object` (vtable pointer) and makes indirect calls to virtual methods at offsets +0, +8, +12, +16, +20, +24, +28, +36, +40.

Two vtable addresses are written by constructors but **NOT populated** in `vtable_prepopulate.h`:

| Constructor | Vtable Address | Object Type |
|-------------|---------------|-------------|
| `sub_8292F0C0` | **0x820A0E34** | CBaseModelInfo |
| `sub_829305A8` | **0x820A1014** | CStreamedSceneObject |

Since these vtable regions contain zeros, all indirect calls go to address 0 → `[MISSING-FUNC]` spam.

### Required Vtable Offsets (from sub_8291DF00)

ProcessAll calls the following virtual methods on each active pool object:

| Offset | Slot | Code Location | Likely Purpose |
|--------|------|---------------|----------------|
| +0x00 | [0] | line 7470 | Destructor / primary method |
| +0x08 | [2] | line 7457, 7734 | GetType / UpdateState |
| +0x0C | [3] | line 7584 | GetBounds |
| +0x10 | [4] | line 7617, 7713 | SetFlags |
| +0x14 | [5] | line 7629 | GetFlags |
| +0x18 | [6] | line 7688 | GetLodInfo |
| +0x1C | [7] | line 7527 | GetStreamingState |
| +0x24 | [9] | line 7688 | GetModelData |
| +0x28 | [10] | line 7629 | UpdateFlags / GetModelFlags |

**Note**: The actual function addresses for these vtable slots are in the XEX `.rdata` section, NOT in the constructors (which only write zeros). Need to extract from `default.bin` or the pseudo code at the vtable addresses.

### Spam Call Sites

- **0x8291E144**: `vtable[10]` (offset +40) — `UpdateFlags()` / `GetModelFlags()`
- **0x8291E1B0**: `vtable[6]` (offset +24) — `GetLodInfo()`

### Fix Options

1. **Extract vtable entries from XEX `.rdata`** — add 9 entries each for 0x820A0E34 and 0x820A1014 to `vtable_prepopulate.h`
2. **Hook `sub_8291DF00`** — skip null-vtable objects (check `*(uint32_t*)object == 0` before dispatch)
3. **Hook constructors** — write vtable addresses pointing to registered stubs

### Pool Structure

Global pool pointer at `0x831C8EC0`. Element size: 768 bytes. Flag array at `pool+4` (bit 7 = free slot).

---

## 14. Render Dispatch Chain

### Core Dispatch: sub_828C15C8

The central render dispatch function. Executes per frame:

1. Increments render counter at `0x82B0B48C`
2. Initializes GPU state, sets render targets 0-3, depth stencil, viewport
3. **Scene vtable dispatch**: reads `*(0x831C2458)`, loads vtable, calls `vtable[16]` (offset 64)
4. Validates render result against scene list array at `0x831C23E8`
5. Submits clear + draw calls via `sub_82A3CC68`
6. GPU fence/sync via `sub_828BF420`

### Complete Call Chain

```
sub_82142F90 (per-frame update)
  → sub_82142230 (state machine, creates scene when ready)
  → sub_828C5ED8 (render frame orchestrator)
      → sub_828C5840 (begin frame)
      → sub_828C6AD0 (scene graph setup)
      → sub_828CD438 (render pass 1)
      → sub_828CFE30 (render pass 2)

sub_828C5BA0 (render timer)
  → sub_828C15C8 (core dispatch)
      → scene->vtable[16](scene)  ← reads 0x831C2458
      → sub_82A3CC68 (draw submit)
      → sub_828BF420 (GPU fence)

sub_82A467D8 (Present, hooked in video.cpp)
  → Video::Present()
```

---

## 15. XAM Sign-In Emulation Flow

### Architecture

Three layers:
1. **LibertyRecomp kernel** (`xam.cpp`, `memory.cpp`) — game-specific hooks
2. **Liberty UserProfile** (`user_profile.h/.cpp`) — "Niko" profile singleton
3. **RexGlue XAM** (`xam_user.cpp`) — Xenia-derived implementation

### XamUserGetSigninState

LibertyRecomp overrides this. Returns `SignedInLocally` (1) for user 0, `NotSignedIn` (0) for others.

5 call sites in generated code:
- `sub_82A12188` — pure tail-call wrapper
- `sub_82A12198` — scans users 0-3 looking for signin state == 1
- `sub_82A29028` — notification listener / LIVE state check
- `sub_82A36558` — Xbox LIVE login check (gates LIVE features, returns 1245 if state != 2)

### Notification System

`XamNotifyCreateListener` enqueues startup notifications on first call:
- `XN_SYS_SIGNINCHANGED` ×2 (0x0A)
- `XN_SYS_INPUTDEVICESCHANGED` ×2 (0x12)

**Corrected**: Only 2 sign-in notifications are ever enqueued. `sub_8223DB20` consumes them via `XNotifyGetNext`. After both are consumed, XNotifyGetNext returns 0 → sub_8223DB20 returns 0 → caller treats as "stable, continue." This is the **correct path**, not a blocker.

### Key Address Relationships

```
0x831C501C (sign-in byte=1) → sub_821B4108 counts 1 active player
    → Unlocks state 0 → state 3+

0x82A9172C (active player=0) → sub_821406C8 returns valid slot
    → State 3 checks slot fields for "newly set"

0x82A95478 (player/episode=0, patched from -1) → sub_822440F8 state 2 proceeds
    → State 4 inner machine runs

0x82A95474 (profile index=2) → set BY THE GAME during state 4
    → Identifies base GTA IV episode
```

---

## 16. Per-Frame Update (sub_82142F90)

### Call Context

Called from `sub_82140088` (game loop) every frame. `sub_82142230` (state machine) is called ONCE before the loop starts, then `sub_82142F90` runs every frame inside the loop.

### Rendering Mode Decision

Evaluates `bShouldRenderScene` at `0x831D5325` based on:
- Loading state counter at `0x831D535C`
- Game-running byte at `0x82BFA144`
- Game state at `0x82BFA124` (compared to 3 and 8)

### Full Scene Update (57+ subsystems)

When `bShouldRenderScene == true`, updates include:
- Input (`sub_82205850`), Timer (`sub_82142B88`), Network (`sub_826CDEB8`)
- Camera (`sub_821EC8C8`), Weather (`sub_8229F078`), Clock (`sub_822021E8`)
- Physics (`sub_821D0558`), Collision (`sub_8227E130`), Weapons (`sub_823ABAB8`)
- Particles (`sub_824414E8`), Cutscenes (`sub_822A86A0`), Shadows (`sub_823FD3C0`)
- Animation (`sub_822136A0`), AI/Peds (`sub_822236F0`), Player (`sub_822387B8`)
- Police (`sub_821C1C78`), Scripts (`sub_821FB770`), Interiors (`sub_82159150`)
- Audio (ambience, radio, script audio, finalize)
- Render dispatch (`sub_82149930`), Post-FX (`sub_821E6660`), Texture streaming (`sub_822D1C20`)

### Loading Screen Path

When `bShouldRenderScene == false`: minimal update with clock, collision, physics, streaming progress, loading screen render, render dispatch.

### Scene Pointer System

- Scene index at `0x82A98778` (int32, -1 = no scene)
- Scene array at `0x82C01C70` (array of pointers)
- `sub_8225CF80` (GetCurrentScene): indexes into array, returns scene+1400 (viewport subobject)

---

## 17. Scene Creation Sub-Machine (sub_82242910) — Deep Dive

### Overview

`sub_82242910` is a 15-state sub-machine called from the outer state 6 inner state 2
(`sub_822438B0`). It handles the full scene creation pipeline: readiness checks, resource
loading, platform/hardware initialization, device validation, and scene finalization.

**State variable**: stored at offset -26556 from base 0x82C00000 → `0x82BF9844`
**Error code**: written to `0x82A9546C` (offset 21612)
**Return**: r3 = 0 (in progress), 1 (done), 2 (error)

### State Transitions

| State | Handler | Action | Next State (success) |
|-------|---------|--------|---------------------|
| 0 | loc_822429A0 | sub_8223DAA0 readiness check | → 4 (fast) or → 1 (normal) |
| 1 | (normal path) | writes value to 0x82BF9834 | → 3 |
| 2 | loc_82242A4C | sub_8223CB60 scene ready query, sub_826CBA70, sub_8223DAA0 | → 3 |
| 3 | (normal path) | writes 6 to 0x82BF9834 | → 4 |
| 4 | loc_82242B1C | sub_82240B08 hardware init, inner sub-state machine | varies |
| 6 | loc_82242D18 | sub_8284AAE0 platform/resource creation | → 7 or 8 |
| 8 | loc_82242E08 | sub_8284B430 + sub_8284B490 device checks | → 9 |
| 9 | loc_82242EE4 | sub_8284ABA0 rendering/scene resource op | → 10 |
| 10 | loc_82242F50 | sub_8284B490 + sub_8284B430 final validation | → done |

### Error 34 Root Cause (Current Blocker)

**The fast path problem:**

1. State 0 calls `sub_8223DAA0` → returns 1 (ready) because:
   - `sub_826CD808` returns non-zero (device appears active due to sign-in emulation)
   - `sub_829DB9B8` returns < 0 (trivial accessor reads *(r3+72))
2. Fast path: state 0 → state 4 directly, skipping states 1-3
3. State 4 reads `0x82BF9834` which still holds value 2 (written by `sub_822438B0` state 1)
4. Switch on value 2 → error code 34

**The normal path (what Xbox 360 does):**

1. State 0 → state 1 (sub_8223DAA0 returns 0, not ready yet)
2. State 1 → state 3
3. State 2 → state 3 (writes 6 to 0x82BF9834)
4. State 3 → state 4
5. State 4 reads `0x82BF9834` = 6 → different switch case, no error 34

### All Error Codes

| Error | State(s) | Trigger | Failed Function |
|-------|----------|---------|-----------------|
| 6 | 2, 7, 11, 14 | Resource/property validation failure | sub_8223CB60 / sub_8223F9F0 chain |
| 7 | 5, 6, 8 | Platform/resource creation failed | sub_8284AAE0 |
| 8 | 8 | Device check returns status ≠ 2 | sub_8284B430 |
| 9 | 8 | Device state assertion failed | sub_8284B490 → sub_8284B430 |
| 14 | 9 | Scene model load/setup failed | sub_8284ABA0 |
| 15 | 10 | Device validation returns 1 | sub_8284B490 |
| 16 | 10 | Device state bad (not 0 or 5) | sub_8284B430 |
| 33 | 4 | Game/user state validation failed | sub_8223DB20 |
| 34 | 4, 9, 10, 11, 14 | Scene asset loading/validation failed | sub_82240B08 / sub_82240B78 |

### Full 15-State Flow

| State | Functions Called | Success → | Error Codes |
|-------|----------------|-----------|-------------|
| 0 | sub_8223DAA0 | → 1 (normal) or → 4 (fast) | — |
| 1 | sub_826CBA70, sub_8223DAA0, sub_8223F9F0 | → 2 | — |
| 2 | sub_8223CB60, sub_826CBA70, sub_8223DAA0 | → 3 | 6 |
| 3 | sub_826CBA70, sub_8223DAA0 | → 4 | — |
| 4 | sub_8223DB20 validation | → 5 | 33, 34 |
| 5 | sub_8223DB20, sub_826CBA70, sub_8284AAE0 | → 6 | 7 |
| 6 | sub_8223DB20, sub_826CBA70, sub_8284AAE0 | → 7 or 8 | 7 |
| 7 | sub_8223DB20, sub_8223CB60 | → 8 | 6 |
| 8 | sub_8284AB10, sub_8284AB70, sub_8223DB20, sub_8284B490, sub_8284B430 | → 9 | 7, 8, 9 |
| 9 | sub_8223DB20, sub_82240B78, sub_8223D2F0, sub_8284ABA0 | → 10 | 14, 34 |
| 10 | sub_8284ABD0, sub_8284ABF8, sub_8223DB20, sub_82240B78, sub_8284B490, sub_8284B430 | → 11 | 15, 16, 34 |
| 11 | sub_8223DB20, sub_82240B78, sub_8223D400 | → 12 or 13 | 6, 34 |
| 12 | sub_822417B0 (save op) | → 13 or 14 | — |
| 13 | sub_822417B0 (save op) | → 0 (complete) | — |
| 14 | sub_8223DB20, sub_82240B78, sub_8223F9F0 | → 0 | 6, 34 |

**Key pattern**: `sub_8223DB20` is called at nearly every state as a validation gate.
Error 34 can occur at 5 different states (4, 9, 10, 11, 14) via sub_82240B08/sub_82240B78.

---

## 18. sub_82240B08 — Platform/Hardware Initialization

Called during scene creation state 4 to initialize hardware resources.

### Behavior

1. Calls `sub_8284B3D8(0x83192C50, value_from_0x82BF3D90)`
2. Tests result (byte mask, compare to 0)
3. **Success** (result != 0):
   - Stores 1 to `0x82BF3A77` (byte)
   - Stores 1 to `0x82BF3CDA` (byte)
   - Returns 1
4. **Failure** (result == 0):
   - Calls `sub_8223DB90` (error handler/fallback)
   - Returns 0

### Key Addresses

| Address | Direction | Description |
|---------|-----------|-------------|
| 0x82BF3D90 | Read | Parameter for hardware init |
| 0x82BF3A77 | Write | Hardware init flag (set to 1) |
| 0x82BF3CDA | Write | Hardware init flag (set to 1) |

Does NOT write to 0x82BF9834.

### Related: sub_8284B4B0 (Sparse Array Clear)

Called in state 0 fast path before transitioning to state 4:
- Takes base address `0x83192C50` and an index
- Computes offset via `(r4 * 4 + r4 << 5) << 5`
- Clears one 32-bit location at computed offset
- Purpose: marks sparse array element as "ready" for state 4

---

## 19. sub_8223CB60 — Scene Ready Query

Returns a boolean (0 or 1) indicating if the scene is ready for loading.

### Switch Logic

Based on status value at offset -26556 (0x82BF9844):

| Value | Return | Meaning |
|-------|--------|---------|
| 0 | 0 | Scene not ready |
| 2 | 0 | Scene not ready |
| 1 | 1 | Scene ready |
| 3 | 1 | Scene ready |
| 4 | 1 | Scene ready |

Called from sub_82242910 state 2; return value determines whether to proceed.

### Related: sub_8223CAD8 (Scene Ready Notification)

- Checks byte flag at offset -26548
- Clears flag: `PPC_STORE_U8(offset -26548, 0)`
- Conditionally calls sub_8224EFE8 or sub_8224E620 based on values at -24252/-24260

### Related: sub_826CD5A8 (Scene Initialization/Registration)

- Uses **static pre-allocated memory** (not heap allocation)
- Memsets 320 bytes at offset -2840 to 0xFF (clearing previous scene data)
- Reinitializes multiple flag and state fields
- Calls sub_829DB8D0, sub_82A11D08 (memset), sub_826CAF78, sub_826CD550
- Uses function dispatch based on state flags rather than vtables

---

## 20. sub_82240480 — Level/Scene Setup

Sets up level-specific paths and resource identifiers based on content type.

### Content Type Dispatch

Reads content type from `0x82B39504` (r30):

| r30 | Content | String Written | Destination |
|-----|---------|----------------|-------------|
| 1 | GTA IV base | "TLAD" (UTF-16) | 0x82BF3960 |
| 2 | DLC | "TBOGT" (UTF-16) | 0x82BF3960 |
| ≤0 | Default | Path from 0x82BCF998 | 0x82BF3960 |

### String/Path References

- Loads paths from: 0x82006170, 0x82006178, 0x82006184, 0x8200618C
- Calls sub_8221F160 for path copying
- Calls sub_821D1128 for resource ID 670 lookup
- Uses sprintf (sub_82A00108) for path formatting

### Callers (5 locations)

| Caller | Context |
|--------|---------|
| sub_82242218 | Level setup flow (writes 0x82BF9834 after return) |
| sub_822422E0 | State 5 game start |
| sub_82243340 | Error code selection |
| sub_82243338 | Error code selection |
| sub_82243E6C | Complex game state update |

### Connection to 0x82BF9834

The **caller** function `sub_82242218` writes to `0x82BF9834` immediately after
sub_82240480 returns:

```
lis r29, -32064       // r29 = 0x82C00000
stw r11, -26572(r29)  // stores to 0x82BF9834
```

This confirms `0x82BF9834` is initialized as part of the level setup flow, NOT inside
sub_82240480 itself but in its caller sub_82242218.

---

## 21. sub_82241370 — Pre-State-Machine Initialization

Runs exactly once before the state machine loop starts in sub_82142230.

### Addresses Initialized

| Address | Value | Description |
|---------|-------|-------------|
| 0x82BF98A0-0x82BF98B4 | 0 (6 dwords, 24 bytes) | Game state variables zeroed |
| 0x82A95474 | 0xFFFFFFFF (-1) | Player slot 0 sentinel |
| 0x82A95478 | 0xFFFFFFFF (-1) | Player slot 1 sentinel (episode index) |
| 0x82BF9858 | user count from 0x831E4DD4 | Cached active user count |
| 0x82BF985C | 0 | State variable |
| 0x82BF9860 | 0 | State variable |

### Control Flow

```
sub_82241370():
  1. Load user count from 0x831E4DD4
  2. Reset 24 bytes of state at 0x82BF98A0 to zero
  3. Initialize player slot markers to -1 at 0x82A95474/0x82A95478
  4. Call sub_8221B198() (init helper)
  5. Store user count at 0x82BF9858
  6. Clear 0x82BF985C and 0x82BF9860 to 0
  7. Check console mode flag at 0x82BCFBFA:
     - If 0: exit (single console mode)
     - If ≠ 0: Call sub_8221FD88() + sub_822217B0() for multi-console setup
  8. Return
```

### Sub-functions Called

1. **sub_8221B198** — init helper, return value stored at 0x82BF9858
2. **sub_8221FD88** — console-specific config (only if byte at 0x82BCFBFA != 0)
3. **sub_822217B0** — copies 64 bytes of console config to 0x82BF8C50 (conditional)

### Does NOT Initialize 0x82BF9834

The function does NOT write to 0x82BF9834 directly. That address is written by
sub_82242218 (the level setup caller) during scene creation.

---

## 22. sub_8223DAA0 — System Readiness Check

Controls whether state 0 takes the fast path (→ state 4) or normal path (→ state 1).

### Logic

```
result = 0
if sub_826CD808() != 0:        // device/controller active?
    if sub_829DB9B8(ptr) < 0:  // status field negative?
        result = 1             // system ready
return result
```

### Why It Returns 1 Prematurely in Recomp

On Xbox 360, `sub_826CD808` returns 0 during early boot because no controller is signed in
yet. The state machine loops through states 0→1→3→2→3→…→4 over multiple frames as
devices come online.

In the recomp, `PPC_STORE_U8(0x831C501C, 1)` (sign-in emulation in memory.cpp) makes the
device appear active immediately. `sub_826CD808` returns non-zero on the very first call,
and `sub_829DB9B8` reads a negative status value, so `sub_8223DAA0` returns 1 → fast path
→ state 4 → error 34.

### sub_826CD808 (Device/Controller Status)

Returns non-zero when a device is active. Reads controller slot data to determine status.

### sub_829DB9B8 (Trivial Accessor)

Just reads `*(r3 + 72)` — a status field from a structure pointer. Returns the value
directly.

---

## 23. Error Recovery Loop (State 7)

When scene creation fails with any error, `sub_822438B0` enters state 7 which calls:
- `sub_8223CC68` — cleanup/reset
- `sub_82242608` — error recovery

This loops repeatedly but never completes in the recomp because the error condition
persists (0x82BF9834 never gets the correct value).

---

## 24. Summary of 0x82BF9834 Write Chain

The address `0x82BF9834` is the critical shared variable between the outer state machine
and the scene creation sub-machine:

| Writer | When | Value | Effect |
|--------|------|-------|--------|
| sub_822438B0 state 1 | Outer state 6, inner state 1 | 2 | "Pending" marker |
| sub_82242218 | Level setup (calls sub_82240480) | varies | Level-specific init |
| sub_82242910 state 2 | Normal path, state 2→3 | 6 | "Level loaded" |
| sub_82242910 state 4 | Reads it | — | Switch dispatch on value |

**In recomp**: value stays at 2 because fast path skips states 1-3 where value 6 is written.
State 4 switch on value 2 → error 34.

**Fix strategy**: Either make `sub_8223DAA0` return 0 on first call (forcing normal path),
or pre-write 6 to `0x82BF9834` before state 4 executes.

---

# Part II: RexGlue Integration Analysis

> Research from 10 agents investigating why RexGlue's runtime doesn't prevent the
> scene creation error 34 blocker.

---

## 25. RexGlue Architecture Overview

**Division of responsibility:**
- **RexGlue**: Handles ALL Xbox 360 kernel exports (xboxkrnl.exe + xam.xex)
- **Liberty**: Only hooks frontend systems — Video/GPU (31), Audio, Input (4),
  Networking (27), Voice (4), Sessions (2), Profile (1)

RexGlue is a port of Xenia's kernel/XAM subsystem adapted for recompilation rather
than emulation. It provides: memory management, threading, VFS, content manager,
user profiles, notifications, overlapped I/O, and UI dialogs.

---

## 26. Root Cause: Immediate Device Readiness

The fundamental problem is that RexGlue reports all devices as immediately ready,
while Xbox 360 hardware brings devices online gradually over multiple frames.

### Sign-in State: Hardcoded to 1

**File**: `glue/rexglue-sdk-main/include/rex/system/xam/user_profile.h:217`
```cpp
uint32_t signin_state() const { return 1; }  // ALWAYS returns 1
```

`XamUserGetSigninState` (in `xam_user.cpp`) calls this for user 0 and returns 1
on the very first call. No frame-based progression exists.

### Input: NOP Driver Always Returns SUCCESS

**File**: `glue/rexglue-sdk-main/src/input/nop/nop_input_driver.cpp`

RexGlue's input system uses a dual-driver architecture:
1. **SDL driver**: Polls real controllers via SDL2
2. **NOP driver (fallback)**: Always reports device connected for user 0

When no physical controller is connected, SDL returns `X_ERROR_DEVICE_NOT_CONNECTED`,
but the NOP fallback immediately returns `X_ERROR_SUCCESS` with zeroed state.
Comment in code: *"Spoof a connected controller for user 0 so games don't pause
waiting for input"*

### Device State: Always Ready

**File**: `LibertyRecomp/kernel/xam.cpp:528-535`
```cpp
uint32_t XamContentGetDeviceState(uint32_t DeviceID, be<uint32_t>* pState) {
    if (pState) *pState = 1;  // ALWAYS ready
    return ERROR_SUCCESS;
}
```

### Notifications: Sent Once, Immediately

**File**: `LibertyRecomp/kernel/xam.cpp:147-187`

When the first `XamNotifyCreateListener` is called, ALL startup notifications are
enqueued at once:
- `XN_SYS_UI` (0x09) = 1
- `XN_SYS_SIGNINCHANGED` (0x0A) = 1 (×2)
- `XN_SYS_INPUTDEVICESCHANGED` (0x12) = 0 (×2)
- `XN_SYS_INPUTDEVICECONFIGCHANGED` (0x13) = 0 (×2)
- `XN_LIVE_CONNECTIONCHANGED` (0x0B) = 0

On Xbox 360, these would fire over multiple frames as hardware initializes.

### Impact on State Machine

`sub_8223DAA0` checks device readiness:
```
if sub_826CD808() != 0 AND sub_829DB9B8(ptr) < 0:
    return 1  // System ready → FAST PATH
```

Because RexGlue makes everything ready immediately, this returns 1 on the first
call, causing state 0 → state 4 (skipping states 1-3 that write value 6 to
0x82BF9834).

---

## 27. RexGlue Content Manager: Synchronous By Design

**Files**: `glue/rexglue-sdk-main/src/system/xam/content_manager.cpp`,
`glue/rexglue-sdk-main/src/kernel/xam/xam_content.cpp`

All content operations complete **synchronously** within the function call:

| Operation | RexGlue Behavior | Xbox 360 Behavior |
|-----------|-----------------|-------------------|
| XamContentCreateEnumerator | Scans filesystem, returns immediately | Async enumeration |
| XamContentCreate/Open | Creates dirs + mounts device immediately | Async with overlapped |
| XamContentClose | Unregisters device immediately | Sync |
| Overlapped I/O | Returns IO_PENDING then completes immediately | True async completion |

When `overlapped_ptr` is provided, RexGlue returns `X_ERROR_IO_PENDING` but
calls `CompleteOverlappedDeferredEx()` which executes the lambda immediately on
a dispatch thread (with only a 10ms delay).

**0x82BF9834 is NOT written by RexGlue** — it's a game-internal state variable
managed by the scene creation sub-machine (sub_82242910 states 1-3).

---

## 28. XAM Dialog System: Complete But Never Invoked

**File**: `glue/rexglue-sdk-main/src/kernel/xam/xam_ui.cpp` (684 lines)

RexGlue implements a full XAM dialog system with ImGui rendering:

**Implemented dialogs:**
- `XamShowMessageBoxUI` — Modal with buttons, ImGui popup
- `XamShowKeyboardUI` — Text input with OK/Cancel
- `XamShowDeviceSelectorUI` — Returns dummy device 0x00000001
- `XamShowDirtyDiscErrorUI` — Disc error dialog
- `XamIsUIActive` — Active dialog counter

**Dialog lifecycle** (3-phase):
```
pre()  → Broadcasts XN_SYS_UI = true (0x9, 1)
run()  → Executes dialog on UI thread via ImGui
post() → Broadcasts XN_SYS_UI = false (0x9, 0) + 100ms delay
```

**Why it's never invoked**: The save state machine caller `sub_821200D0` was never
recompiled (address 0x821200D0 is below the generated code range 0x82140000+).
This function calls `sub_821E6508` → `sub_8223F9F0` (XAM dialog flow) →
`sub_82254FE0` which would write 1 to `0x82BF9B70` (readiness flag).

Without sub_821200D0 running, the XAM dialog flow never fires, and the readiness
signal is never sent.

---

## 29. Threading: No Frame Synchronization

**File**: `glue/rexglue-sdk-main/src/system/xthread.cpp`

### 1:1 Thread Model (Not Fibers)

RexGlue uses native OS threads, NOT coroutines or fibers:
```
XThread::Create() → rex::thread::Thread::Create() [native OS thread]
```

Each Xbox 360 thread maps directly to a host thread with 16KB minimum stack.

### Yield Function (sub_82849918)

The state machine's per-frame yield is just a passthrough:
```cpp
PPC_FUNC_HOOK(sub_82849918) {
    __imp__sub_82849918(ctx, base);  // Calls recompiled PPC code
    // No actual sleep/delay
}
```

If the recompiled `sub_82849918` doesn't call a Wait/Sleep function internally,
the state machine runs at CPU speed with no frame pacing.

### No VBlank Simulator

RexGlue has NO frame counter, NO VBlank interrupt, and NO frame limiter.
The time system exists (50MHz guest ticks via `chrono::Clock`) but there's no
mechanism to pace state machine iterations to real frames.

### APCs May Be Broken

**File**: `glue/rexglue-sdk-main/src/system/xthread.cpp:654-657`
```cpp
// TODO(tomc): uh I think this needs to be fixed. without the processor
// object managing things, I don't think any apcs are being delivered...
// !critical
```

If APCs don't fire, interrupt-driven yield mechanisms are disabled. This could
prevent the state machine from properly yielding between iterations.

---

## 30. sub_8223DB20: Notification Processing Gate

**File**: `gta4-recomp/generated/gta4_recomp.6.cpp` (line 73432)

This validation function is called at nearly every state in sub_82242910:

1. Loads notification queue handle from `0x82A65FD0`
2. Calls `XNotifyGetNext()` (at 0x82A74AA4) to check for pending notifications
3. If no notification → returns 0 (success)
4. If notification found → validates user context at `0x82A7C2F0` via sub_8223DAA0
5. Returns 0 (success) or 1 (error)

**Error 33** at state 4 means either:
- A stale notification was found in the queue (from RexGlue's immediate notification
  dump at listener creation)
- User context at 0x82A7C2F0 wasn't properly initialized

Since RexGlue dumps all startup notifications at once when the first listener
registers, the notification queue may contain unexpected items when sub_8223DB20
runs during scene creation.

---

## 31. sub_8284xxxx: Graphics Device Management

**File**: `gta4-recomp/generated/gta4_recomp.55.cpp`

These functions manage a **graphics device state table** at base address ~0x82847D88
with 32-byte stride entries:

| Function | Purpose | Return Values |
|----------|---------|---------------|
| sub_8284B3D8 | Device init validation | 1=ready, 0=not ready |
| sub_8284B430 | Device state classifier | 0=unknown, 2/3/5=specific states |
| sub_8284B490 | Raw device state reader | Raw 32-bit status code |
| sub_8284B4B0 | Sparse array clear | void |

**sub_8284B430 return value semantics:**
- Status code 0x4C7 (1223) → returns 2
- Status code related to 0x80000012 → returns 3 or 5
- Anything else → returns 0

These appear to be **D3D/XGDevice state codes**. Error codes 8, 9, 15, 16 in the
scene creation machine all come from these device state checks failing.

**Missing functions**: sub_8284AAE0, sub_8284ABA0, sub_8284AB10, sub_8284ABD0,
sub_8284ABF8 are called but not found in the generated code — they may be
unresolved external stubs or in a different compilation unit.

---

## 32. Save System Integration

**Files**: `LibertyRecomp/kernel/save_hooks.cpp`, `save_system.cpp`, `xam.cpp`

Liberty's save system is complete and well-integrated:

### Initialization Flow
```
main.cpp:265 → SaveSystem::Initialize()
  ├─ Creates save directories (~/.../LibertyRecomp/saves/)
  ├─ Enumerates existing saves (SGTA400-SGTA415)
  ├─ Registers each save slot via XamRegisterContent()
  └─ Maps "SaveData" VFS root

Runtime → sub_821200D0 hook
  ├─ Clears BC9 (exits loading busy-wait)
  ├─ Calls __imp__sub_821200D0 (lets RexGlue handle naturally)
  └─ Save state machine runs through XAM dialogs
```

### 6 Critical PPC Hooks
1. `sub_829A1C38` — Content creation wrapper
2. `sub_829A1CA0` — Content close wrapper
3. `sub_829A1CB8` — Content enumeration wrapper
4. `sub_8297A930` — Save manager orchestrator
5. `sub_82122CA0` — Save system init (3 slot contexts)
6. `sub_821200D0` — Post-init entry point (THE critical hook)

### 0x82BF9B70 Readiness Flag

This flag is expected to be written by `sub_82254FE0` via the XAM dialog flow
(sub_8223F9F0). The save state machine must run to completion for this to happen.

Previous approach (WRONG): Pre-wrote STAGE_COUNTER=17 to bypass state machine.
Current approach: Let RexGlue's XAM subsystem handle naturally.

---

## 33. Synthesis: Why RexGlue Doesn't Prevent Error 34

The error 34 blocker is caused by a **chain of architectural mismatches** between
RexGlue's immediate-readiness model and Xbox 360's gradual-initialization model:

### The Chain

```
1. RexGlue hardcodes signin_state() = 1 (always signed in)
   + NOP input driver spoofs connected controller
   + XamContentGetDeviceState always returns ready
   + All notifications dumped at once on first listener
   ↓
2. sub_8223DAA0 returns 1 on FIRST call (system appears ready)
   ↓
3. Scene creation takes FAST PATH: state 0 → state 4
   (skips states 1-3 that would write 6 to 0x82BF9834)
   ↓
4. State 4 reads 0x82BF9834 = 2 (stale "pending" value)
   Switch case 2 → ERROR 34
   ↓
5. Error recovery loop (state 7) never resolves
   → Game stuck in error loop forever
```

### Why RexGlue Can't Fix This Alone

1. **0x82BF9834 is game-internal** — RexGlue doesn't know about this address
2. **Content manager is correct** — synchronous ops are fine for a recomp
3. **XAM dialogs are implemented** — but never invoked (sub_821200D0 not recompiled)
4. **No frame pacing** — state machine may run too fast even on normal path
5. **APCs possibly broken** — interrupt-driven yields may not work

### Potential Fix Approaches (Initial)

| Approach | Where | Risk |
|----------|-------|------|
| Hook sub_8223DAA0 to return 1 for N calls | Liberty imports.cpp | Low — forces normal path |
| Pre-write 6 to 0x82BF9834 before state 4 | Liberty imports.cpp | Medium — bypasses normal init |
| Add frame counter to UserProfile::signin_state() | RexGlue user_profile.h | Medium — affects all games |
| Defer notification delivery over multiple frames | RexGlue xam.cpp | High — complex timing |
| Fix APC delivery (TODO in xthread.cpp) | RexGlue xthread.cpp | High — fundamental threading change |
| Recompile sub_821200D0 | Codegen config | Unknown — may have dependencies |

---

# Part III: Fix Research — Detailed Feasibility Analysis

> Research from 10 agents exploring each potential fix approach.

---

## 34. Fix #1: Hook sub_8223DAA0 (RECOMMENDED — Lowest Risk)

**Feasibility: 9/10** — Not currently hooked, simple return value override.

### Current Status
- sub_8223DAA0 is NOT hooked in imports.cpp
- IS in PPCFuncMappings (address 0x8223DAA0, gta4_init.cpp line 3376)
- Has 16 call sites across the codebase

### Return Value Semantics (CORRECTED)
- Return **0** = System READY → triggers fast path (state 0→4) — **causes error 34**
- Return **1** = System NOT READY → triggers normal path (state 0→1) — **correct behavior**

### Implementation
```cpp
extern "C" void __imp__sub_8223DAA0(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_readyCheckCount{0};

PPC_FUNC_HOOK(sub_8223DAA0) {
    __imp__sub_8223DAA0(ctx, base);
    int n = s_readyCheckCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 10) {  // Force "not ready" for first 10 calls
        ctx.r3.s64 = 1;
    }
}
```

### Why It Works
- Forces normal path: 0→1→3→2→3→4 instead of 0→4
- States 1-3 write value 6 to 0x82BF9834 before state 4 reads it
- After N calls, natural behavior resumes
- No side effects on other systems

### Risk: LOW
- Only affects return register r3
- Doesn't modify heap, memory, or critical state
- Can be tuned by adjusting N (recommended: 10)

---

## 35. Fix #2: Pre-Write 0x82BF9834=6 (Already Partially Applied)

**Feasibility: 7/10** — Current hook resets to 0, could change to 6.

### Current Status
- sub_822422E0 hook already resets 0x82BF9834 from 2→0 after state 5
- Value 6 is valid (written at state 2 completion, leads to state 6 success path)

### State 4 Switch Values
| Value | Destination | Result |
|-------|-------------|--------|
| 0 | Default/safe | Proceeds normally |
| 2 | Error path | **ERROR 34** (the bug) |
| 6 | loc_82242D18 | Scene creation continuation (success) |

### Risk: MEDIUM
- Timing issue: reset must happen BEFORE state 4 reads the value
- May mask underlying initialization problems
- Current reset to 0 is safer than 6 (0 = "no constraint" default)

---

## 36. Fix #3: RexGlue signin_state() Delay (Upstream Fix)

**Feasibility: 8/10** — 4-5 lines using existing Clock API.

### Implementation
```cpp
// In user_profile.h
uint32_t signin_state() const {
    uint64_t elapsed = chrono::Clock::QueryGuestUptimeMillis();
    return (elapsed >= 150) ? 1 : 0;  // 150ms ≈ 9 frames @ 60fps
}
```

### Timing References Available
- `Clock::QueryGuestTickCount()` — 50MHz guest ticks (thread-safe)
- `Clock::QueryGuestUptimeMillis()` — milliseconds since guest start
- Graphics system already uses these for VSync timing

### Risk: MEDIUM
- Affects ALL RexGlue titles (could add per-title config)
- Liberty has its own UserProfile class that could override independently
- 150ms delay should be safe for most games

---

## 37. Fix #4: Recompile sub_821200D0 (NOT PRACTICAL)

**Feasibility: 2/10** — Cascade of dependencies makes this impractical.

### Why It Fails
- Code range (0x82140000+) is auto-calculated from XEX binary sections
- sub_821200D0 is at 0x821200D0 — below the range
- Callees also outside range: sub_821E6508, sub_82121E80, sub_8223F9F0
- Would cascade into recompiling ~39KB of code in 0x82100000-0x82140000
- Many functions in that range depend on Xbox 360 hardware (VBlank, threading)

### Current Approach (Correct)
- Hook wraps unrecompiled original via `__imp__sub_821200D0`
- Clears BC9 to exit busy-wait, lets RexGlue handle XAM naturally

---

## 38. Fix #5: Fix APC Delivery (CRITICAL BUG FOUND)

**Feasibility: 6/10** — Simple patch but affects fundamental threading.

### The Bug
In `threading_posix.cpp` signal handler (line 1278-1283):
```cpp
case SignalType::kThreadUserCallback: {
    auto p_thread = static_cast<PosixCondition<Thread>*>(info->si_value.sival_ptr);
    if (alertable_state_) {  // ← THIS GUARD DROPS ALL APCs
        p_thread->CallUserCallback();
    }
}
```

`alertable_state_` is a thread-local that's **always false** during recompiled PPC
execution because game threads never enter alertable waits. Result: **ALL overlapped
I/O completion callbacks are silently dropped**.

### Impact
- File I/O completion callbacks never fire
- Content operations that queue APCs for notification never complete
- Dialog operations that depend on APC-based signaling stall

### Fix Options
**Option A** — Remove guard (simple but broad):
```cpp
case SignalType::kThreadUserCallback: {
    auto p_thread = ...;
    p_thread->CallUserCallback();  // Always deliver
}
```

**Option B** — Add cooperative yield points in codegen (correct but complex)

### Risk: HIGH
- Fundamental threading change affects all games
- Could introduce new race conditions
- Needs extensive testing

---

## 39. Fix #6: Hook sub_82242910 State Transition (Alternative)

**Feasibility: 8/10** — Already hooked (logging only), easy to add state override.

### Implementation
```cpp
PPC_FUNC_HOOK(sub_82242910) {
    uint32_t stateBefore = PPC_LOAD_U32(0x82BF9838);
    __imp__sub_82242910(ctx, base);
    uint32_t stateAfter = PPC_LOAD_U32(0x82BF9838);

    // Intercept fast path: if jumped 0→4, force back to 1
    if (stateBefore == 0 && stateAfter == 4) {
        PPC_STORE_U32(0x82BF9838, 1);  // Force normal path
    }
}
```

### Risk: LOW-MEDIUM
- State 1 has no hardware dependencies — safe to force
- May need multiple interceptions (state 0 loops)
- More surgical than hooking sub_8223DAA0

---

## 40. Fix #7: Notification Queue (NOT AN ISSUE)

**Feasibility: N/A** — Research confirmed stale notifications are not the problem.

XNotifyGetNext **dequeues** (removes) notifications. The `has_notified_startup_` flag
prevents re-queuing. By the time sub_82242910 runs, all startup notifications should
be consumed. Error 33 is from a different validation gate.

---

## 41. Fix #8: sub_82849918 Yield (NOT THE ROOT CAUSE)

**Feasibility: N/A** — The yield function does actually sleep.

sub_82849918 calls `KeDelayExecutionThread` via sub_82A12B60 → sub_82A1A200, which
maps to `XThread::Delay()` in RexGlue. The state machine IS sleeping between
iterations. The fast path problem is caused by sub_8223DAA0 returning "ready" too
early, not by missing delays.

---

## 42. Fix #9: Hook sub_826CD808 Selectively (Viable Alternative)

**Feasibility: 7/10** — 38 callers, but return address distinguishes context.

### Selective Hook by Return Address
sub_8223DAA0 calls sub_826CD808 with predictable return addresses:
- First call: lr = 0x8223DAC0
- Second call: lr = 0x8223DACC

```cpp
PPC_FUNC_HOOK(sub_826CD808) {
    if ((ctx.lr == 0x8223DAC0 || ctx.lr == 0x8223DACC)) {
        static int s_count = 0;
        if (s_count++ < 5) { ctx.r3.u64 = 0; return; }
    }
    __imp__sub_826CD808(ctx, base);
}
```

### Risk: LOW (with return address check)
- Only affects calls from sub_8223DAA0
- Other 36 callers continue normally
- More granular than hooking sub_8223DAA0 directly

---

## 43. Fix Priority Ranking

| Priority | Fix | Effort | Risk | Confidence |
|----------|-----|--------|------|------------|
| **1** | Hook sub_8223DAA0 → return 1 for N calls | 5 min | Low | High |
| **2** | Hook sub_82242910 → intercept 0→4 transition | 5 min | Low-Med | High |
| **3** | Hook sub_826CD808 selectively by lr | 10 min | Low | Medium |
| **4** | RexGlue signin_state() delay | 15 min | Medium | Medium |
| **5** | Fix APC delivery in threading_posix.cpp | 30 min | High | Medium |
| **6** | Change sub_822422E0 reset from 0 to 6 | 2 min | Medium | Low |
| — | Recompile sub_821200D0 | Days | Very High | N/A |
| — | Notification queue clearing | N/A | N/A | Not needed |
| — | Add sleep to sub_82849918 | N/A | N/A | Already sleeps |

### Recommended Strategy
1. Apply Fix #1 (hook sub_8223DAA0) as the primary fix — forces normal path
2. Keep existing sub_822422E0 reset as backup safety net
3. Consider Fix #5 (APC delivery) as a separate PR — fixes a real RexGlue bug

---

# Part IV: Deep-Dive Implementation Plan — Hook sub_8223DAA0

> 10 research agents investigated the function body, all 16 call sites, internal
> dependencies, state flow requirements, hook placement, existing hook interactions,
> counter vs state-aware strategies, multi-caller risk, and build/test verification.

---

## 44. sub_8223DAA0 Function Body — What It Actually Does

**Location**: `gta4_recomp.6.cpp` lines 73351-73428

### Internal Control Flow
```
1. Write -1 to state variable at 0x82BF3D90
2. Call sub_826CD808 → compare result to 0
   ├─ If == 0: IMMEDIATELY return 0 (READY / fast path)
   └─ If != 0: Continue...
3. Call sub_829DB9B8(object) → read field at offset +72
4. Write result to 0x82BF3D90
5. Check if result < 0 (negative?)
   ├─ If YES: return 1 (NOT READY)
   └─ If NO: Continue...
6. Call sub_82A12188 (= XamUserGetSigninState wrapper)
7. Check if signin result == 0 (NOT signed in?)
   ├─ If YES: Write -1 to 0x82BF3D90, return 0 (READY)
   └─ If NO: return 1 (NOT READY)
```

### Sub-Function Details

| Function | Location | Purpose |
|----------|----------|---------|
| sub_826CD808 | gta4_recomp.39.cpp:71134 | Device/connection state check. Reads cached value from 0x82A9172C, calls sub_829DB9B8, compares. Returns 0 if states match (stable). |
| sub_829DB9B8 | gta4_recomp.66.cpp:82729 | Trivial accessor: `return *(uint32_t*)(r3 + 72)`. Reads field at offset +72 from passed object. |
| sub_82A12188 | gta4_recomp.69.cpp:35681 | Direct tail-call to `XamUserGetSigninState`. Returns signin state (0=not signed in, 1+=signed in). |

### Return Value Semantics
- **Return 0** = "READY" — prerequisites met, proceed to next state
- **Return 1** = "NOT READY" — prerequisites not yet met, caller should wait/retry

### RexGlue Behavior Analysis

Two scenarios determine what sub_8223DAA0 returns in the recomp:

**Scenario A** — sub_826CD808 returns 0 (device state already stable):
→ sub_8223DAA0 returns 0 (READY) immediately, before game infrastructure is set up.
→ This is the "immediate readiness" problem from RexGlue's instant device enumeration.

**Scenario B** — sub_826CD808 returns non-zero, sub_829DB9B8 returns negative:
→ sub_8223DAA0 returns 1 (NOT READY) because the object field is uninitialized.

**Which scenario actually occurs depends on runtime memory state**. The hook must
handle BOTH cases by logging the actual return value first, then deciding.

---

## 45. All 16 Call Sites of sub_8223DAA0

### Call Site Distribution

| Parent Function | Call Sites | Context |
|-----------------|------------|---------|
| sub_82142230 (outer state machine) | 2 | gta4_recomp.0.cpp lines 5747, 5782 |
| sub_8223DB20 (validation gate) | 1 | gta4_recomp.6.cpp line 73474 |
| sub_8223DEE8 (outer state 2) | 1 | gta4_recomp.6.cpp line 74085 |
| sub_8223E028 (state exit) | 1 | gta4_recomp.6.cpp line 74238 |
| sub_8223F568 | 1 | gta4_recomp.6.cpp line 77392 |
| sub_82240AB0 | 1 | gta4_recomp.6.cpp line 80246 |
| sub_82240B78 | 1 | gta4_recomp.6.cpp line 80388 |
| sub_822414E8 (outer state 0) | 4 | gta4_recomp.6.cpp lines 81869, 81932, 81984, 82049 |
| **sub_82242910 (scene creation)** | **3** | gta4_recomp.6.cpp lines **84901, 84954, 85019** |
| sub_822440F8 (outer state 4 inner) | 1 | gta4_recomp.6.cpp line 88374 |

### Critical Finding
All 16 callers follow the same pattern: call → extract low byte → compare to 0 → branch.
**A global hook affecting ALL calls would alter 14+ code paths** across the entire game
lifecycle — from boot through runtime. This rules out any global override approach.

### sub_8223DB20 Amplification
sub_8223DB20 (the "validation gate") wraps sub_8223DAA0 and is itself called from
**35+ locations** across the codebase. Any change to sub_8223DAA0's return value
propagates through sub_8223DB20 to all those callers.

---

## 46. sub_82242910 State Machine — Detailed Flow Trace

### State 0 (loc_822429A0, line 84898)
```
call sub_8223DAA0()
if (r3 & 0xFF) != 0:     → write state=4, return 1  [FAST PATH — skips 1-3]
if (r3 & 0xFF) == 0:     → write state=1, return 1  [NORMAL PATH]
```
**Normal path requires**: sub_8223DAA0 returns **0** (ready)

### State 1 (loc_822429EC, line 84942)
```
call sub_826CBA70()       → if != 0: goto error (state=3)
call sub_8223DAA0()       → if != 0: goto error (state=3)
call sub_8223F9F0(0, 0)   → if == 0: goto fatal error
→ write state=2, return 1
```
**Normal path requires**: sub_8223DAA0 returns **0** (ready)

### State 2 (loc_82242A4C, line 84998)
```
call sub_8223CB60()       → save result to r31
call sub_826CBA70()       → if != 0: goto error (state=3)
call sub_8223DAA0()       → if != 0: goto error (state=3)
if r31 != 0: call sub_8223F9F0(1)
*** WRITES VALUE 6 TO 0x82A9546C ***    ← THE CRITICAL WRITE
→ return 2
```
**Normal path requires**: sub_8223DAA0 returns **0** (ready)
**Side effect**: Writes value **6** to error code location (0x82A9546C)

### State 3 (loc_82242AD0, line 85073)
```
call sub_826CBA70()       → if != 0: goto fatal error
call sub_8223DAA0()       → if == 0: goto state=2 (retry loop)
                          → if != 0: continue...
call sub_82240AB0()
→ write state=4, return 1
```
**Normal path requires**: sub_8223DAA0 returns **non-zero** (1) to advance to state 4

### State 4+ (loc_82242B1C, line 85118)
```
call sub_8223DB20()       → if != 0: write error 33, return 2
                          → if == 0: complex initialization...
```
Uses sub_8223DB20 (not sub_8223DAA0 directly). Needs return 0 to proceed.

### Normal Path Summary Table

| Step | State | sub_8223DAA0 return needed | Next State | Side Effects |
|------|-------|---------------------------|------------|--------------|
| 1 | 0 | **0** (ready) | 1 | — |
| 2 | 1 | **0** (ready) | 2 | calls sub_8223F9F0 |
| 3 | 2 | **0** (ready) | returns 2 | **writes 6 to 0x82A9546C** |
| 4 | 3 | **1** (not ready) | 4 | calls sub_82240AB0 |
| 5 | 4+ | 0 (via sub_8223DB20) | 5+ | complex init |

**Total sub_8223DAA0 calls in normal path**: 4 (states 0, 1, 2, 3)

---

## 47. Why a Global Counter Hook FAILS

### Problem 1: 16 Call Sites Across Entire Lifecycle
sub_8223DAA0 is called from boot initialization (sub_822414E8), storage setup
(sub_8223DEE8), scene creation (sub_82242910), and runtime validation (sub_8223DB20
with 35+ callers). A counter that "burns" during boot exhausts before scene creation.

### Problem 2: Alternating Return Values Required
The normal path needs return 0 at states 0, 1, 2 then return 1 at state 3. A simple
"return 1 for first N calls" would prevent states 0→1 (which needs return 0).

### Problem 3: Thread Safety Logic Errors
Multiple threads may call sub_8223DAA0 concurrently. An atomic counter prevents data
races but causes logic errors — different threads see different counter values, leading
to inconsistent behavior.

### Problem 4: sub_8223DB20 Amplification
sub_8223DB20 calls sub_8223DAA0 but also calls XNotifyGetNext first. If XNotifyGetNext
returns 0, it skips sub_8223DAA0 entirely. A counter-based hook doesn't account for
these skipped calls.

---

## 48. Revised Fix Strategy — Hook sub_82242910 State Transition

### Why sub_82242910 is Better Than sub_8223DAA0

Given the findings:
1. sub_8223DAA0 has 16 call sites — too many to safely override globally
2. The actual problem is specifically at **sub_82242910 state 0** transitioning to state 4
3. sub_82242910 is ALREADY HOOKED (logging only) — just needs logic added
4. The state variable at 0x82BF9838 can be read to detect the 0→4 fast path

### Implementation: Intercept State 0→4 Transition

```cpp
// sub_82242910 — SCENE CREATION SUB-MACHINE (15 states, 0-14)
// ENHANCED: Intercept fast path (state 0→4) and force normal path (state 0→1)
extern "C" void __imp__sub_82242910(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_sceneCreateCount{0};
PPC_FUNC_HOOK(sub_82242910) {
    uint32_t stateBefore = PPC_LOAD_U32(0x82BF9838);
    uint32_t modeVal = PPC_LOAD_U32(0x82BF9834);
    __imp__sub_82242910(ctx, base);
    uint32_t stateAfter = PPC_LOAD_U32(0x82BF9838);

    // ── Fix: Intercept fast path 0→4 ──────────────────────────────
    // On Xbox 360, sub_8223DAA0 returns 0 at state 0 (prerequisites loaded
    // after device enumeration delay), taking the normal path 0→1→2→3→4.
    // In the recomp, sub_8223DAA0 may return 1 (not ready) OR 0 (ready
    // too early), both causing state 0→4 which skips states 1-3.
    // State 2 writes value 6 to 0x82A9546C — without it, state 4 reads
    // stale data and triggers error 34.
    //
    // Fix: If state jumped 0→4, force it back to 1 (normal path entry).
    // This is safe because:
    //   - State 1 has no hardware dependencies
    //   - States 1-3 set up the prerequisites that state 4 expects
    //   - sub_8223DB20 (used by state 4+) cannot fail (both paths valid)
    if (stateBefore == 0 && stateAfter == 4) {
        PPC_STORE_U32(0x82BF9838, 1);  // Force normal path
        stateAfter = 1;
        printf("[SCENE-CREATE] INTERCEPTED fast path 0→4, forced to 0→1\n");
        fflush(stdout);
    }
    // ──────────────────────────────────────────────────────────────

    int n = s_sceneCreateCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 50 || stateBefore != stateAfter || (n % 200) == 0) {
        uint32_t sceneObj = PPC_LOAD_U32(0x82BF3A88);
        uint32_t errorCode = PPC_LOAD_U32(0x82A9546C);
        printf("[SCENE-CREATE] sub_82242910 #%d ret=%d state=%d→%d "
               "mode=0x%08X sceneObj=0x%08X err=%d\n",
               n, ctx.r3.s32, stateBefore, stateAfter, modeVal,
               sceneObj, errorCode);
        fflush(stdout);
    }
}
```

### Why This Works

1. **Targeted**: Only affects sub_82242910 state 0→4, nothing else
2. **No new hooks needed**: Modifies the existing sub_82242910 hook
3. **Safe for all callers**: sub_8223DAA0 behavior is unchanged everywhere
4. **State 1 has no hardware deps**: Game can enter state 1 at any time
5. **Self-correcting**: If state 1-3 logic works naturally, the fix just
   provides the entry point — the game's own code handles the rest

### Risk Assessment

| Factor | Risk Level | Notes |
|--------|-----------|-------|
| State 1 entry safety | **LOW** | No Xbox hardware dependencies in state 1 |
| State 2 value-6 write | **LOW** | Pure memory write, no I/O |
| State 3→4 transition | **MEDIUM** | Requires sub_8223DAA0 to return 1 — may need additional handling if it keeps returning 0 |
| Interaction with existing hooks | **NONE** | sub_822440F8 (outer state 4) and sub_822422E0 (outer state 5) run in different contexts |
| Multi-threading | **LOW** | sub_82242910 is called from a single thread context (outer state 6) |

---

## 49. Alternative: Direct sub_8223DAA0 Hook (Context-Aware)

If the sub_82242910 state interception doesn't work (e.g., states 1-3 also need
special handling), the fallback is a context-aware sub_8223DAA0 hook that only
activates when called from sub_82242910.

### Implementation: Return Address Check

```cpp
extern "C" void __imp__sub_8223DAA0(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_prereqCount{0};
PPC_FUNC_HOOK(sub_8223DAA0) {
    __imp__sub_8223DAA0(ctx, base);  // Always call original first (side effects!)

    uint32_t ret = ctx.r3.u32 & 0xFF;
    uint32_t sceneState = PPC_LOAD_U32(0x82BF9838);  // sub_82242910 state

    int n = s_prereqCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 30 || (n % 500) == 0) {
        printf("[PREREQ] sub_8223DAA0 #%d ret=%d sceneState=%d lr=0x%08X\n",
               n, ret, sceneState, (uint32_t)ctx.lr);
        fflush(stdout);
    }

    // Only override when called from sub_82242910 state 0
    // sub_82242910 calls sub_8223DAA0 at PPC address 0x822429B4 (state 0)
    // The lr register contains the return address
    if (ctx.lr == 0x822429B8 && sceneState == 0) {
        // Force return 0 (ready) to take normal path 0→1
        ctx.r3.u64 = 0;
        printf("[PREREQ] OVERRIDE: forced ret=0 at scene state 0 (normal path)\n");
        fflush(stdout);
    }
}
```

### Caveats
- Return address (lr) must be verified from the generated code
- More fragile than the sub_82242910 state interception approach
- Still safe because it only affects one specific call site

---

## 50. Existing Hooks — Interaction Matrix

### Hook Execution Order (Chronological)
```
1. sub_822440F8 (outer state 4 inner) — bypasses save device, returns 2
2. sub_822422E0 (outer state 5)       — resets 0x82BF9834 from 2→0
3. sub_822438B0 (outer state 6 inner) — logging only
4. sub_82242910 (scene creation)      — logging + FIX HERE
```

### Interaction Analysis

| Existing Hook | Conflict with Fix? | Keep/Remove? |
|---------------|-------------------|--------------|
| sub_822440F8 (bypass state 4) | **NO** — different state machine (outer vs scene creation) | **KEEP** — still needed for save device bypass |
| sub_822422E0 (reset 0x82BF9834) | **NO** — runs before scene creation | **KEEP** — safety net if value 6 write is delayed |
| sub_822438B0 (logging) | **NO** — just wraps scene creation calls | **KEEP** — useful diagnostics |
| sub_82242910 (logging) | **MODIFY** — add state 0→4 interception | **MODIFY** — becomes the fix location |

### Key Insight: Complementary Fixes
The sub_822422E0 reset (0x82BF9834: 2→0) and the sub_82242910 state interception
work on **different failure modes**:
- Reset prevents error 34 when fast path IS taken (defensive)
- State interception prevents fast path from being taken (proactive)
Both should be kept for defense-in-depth.

---

## 51. Hook Placement — Exact Location in imports.cpp

### File Structure (imports.cpp)
```
Lines 1-100:    Headers, includes, forward declarations
Lines 500-1500: GPU/memory/threading hooks
Lines 1587-1736: Save/storage hooks (sub_822440F8)
Lines 1738-1758: sub_822438B0 (state 6 inner, logging)
Lines 1760-1791: sub_822422E0 (state 5, reset fix)
Lines 1793-1813: sub_82242910 (scene creation, MODIFY HERE)
Lines 1815-1850: Other state machine hooks
Lines 2037-2172: Linker stubs
```

### Modification Point
**Line 1799-1813**: Existing `PPC_FUNC_HOOK(sub_82242910)` — add the state 0→4
interception logic between the `__imp__` call and the logging block.

### No New Registration Needed
PPC_FUNC_HOOK uses `extern "C"` weak symbol override — the linker automatically
picks up the hook. No PatchFuncMapping or InsertFunction call required.

---

## 52. Build & Test Verification Plan

### Build Command (macOS)
```bash
export VCPKG_ROOT=$(pwd)/thirdparty/vcpkg
cmake --build ./out/build/macos-release --target LibertyRecompLib
cmake --build ./out/build/macos-release --target LibertyRecomp
```
**Expected time**: ~2s for imports.cpp compile + ~60s for linking

### Run Command
```bash
"./out/build/macos-release/LibertyRecomp/Liberty Recompiled.app/Contents/MacOS/Liberty Recompiled" > /tmp/liberty_run.log 2>&1
```

### Success Indicators (grep from log)
```
[SCENE-CREATE] INTERCEPTED fast path 0→4, forced to 0→1   ← Fix activated
[SCENE-CREATE] sub_82242910 #N ret=R state=0→1             ← Normal path entered
[SCENE-CREATE] sub_82242910 #N ret=R state=1→2             ← State 1 progressed
[SCENE-CREATE] sub_82242910 #N ret=2 state=2→2 ... err=6   ← Value 6 written!
[SCENE-CREATE] sub_82242910 #N ret=R state=3→4             ← Reached state 4 naturally
[SCENE-CREATE] sub_82242910 #N ret=R state=4→5             ← Past error 34!
```

### Failure Indicators
```
[SCENE-CREATE] sub_82242910 #N ... state=1→3               ← State 1 check failed
[SCENE-CREATE] sub_82242910 #N ... state=2→3               ← State 2 check failed
[SCENE-CREATE] sub_82242910 #N ... err=33                   ← Error 33 (sub_8223DB20)
[SCENE-CREATE] sub_82242910 #N ... err=34                   ← Error 34 (stale value)
```
If state 1→3 loops forever: sub_8223DAA0 keeps returning non-zero at state 1.
If state 2→3 loops forever: sub_8223DAA0 keeps returning non-zero at state 2.
These indicate the **fallback** (Section 49) is needed — context-aware sub_8223DAA0 hook.

### Deadlock Check
If log shows the same state repeating 100+ times with no transition:
```bash
grep -c "state=1→3" /tmp/liberty_run.log  # Should be < 10
grep -c "state=3→2" /tmp/liberty_run.log  # Retry loop count
```
A 3→2→3 cycle is NORMAL (retry pattern). It should eventually break out to 3→4.

---

## 53. Implementation Priority — Revised

Based on deep-dive findings, the priority ranking is updated:

| Priority | Fix | Effort | Risk | Confidence |
|----------|-----|--------|------|------------|
| **1** | **Hook sub_82242910 → intercept 0→4** | 5 min | **Low** | **High** |
| **2** | Context-aware sub_8223DAA0 hook (lr check) | 10 min | Low-Med | Medium |
| **3** | Hook sub_826CD808 selectively by lr | 15 min | Low | Medium |
| ~~1~~ | ~~Global sub_8223DAA0 counter~~ | — | ~~High~~ | ~~REJECTED~~ |

### Rationale for Promoting sub_82242910 Over sub_8223DAA0

1. **sub_82242910 is already hooked** — zero new symbols to add
2. **Only 1 state transition to intercept** vs 16 call sites to manage
3. **No risk to other callers** — sub_8223DAA0 behavior unchanged everywhere
4. **Defense-in-depth** with existing sub_822422E0 reset
5. **Global counter proven unsafe** by multi-caller and multi-thread analysis

### Escalation Path
If sub_82242910 interception alone doesn't work (states 1-3 also fail):
1. Add sub_8223DAA0 context-aware hook (Section 49) as secondary fix
2. If needed, also force sub_82242910 state 1→2 and 2→(value-6 write) directly
3. Last resort: bypass sub_82242910 states 0-3 entirely, pre-write value 6
   that likely affects other async operations beyond scene creation

---

# Part V: Error 24 Deep-Dive — Post-Scene-Creation Failure

> 10 research agents investigated states 8-11, error 24 origin, the 0x82BF9844
> platformMode dual-use, sceneObj=0 root cause, outer state 7 loop, and the
> READY-SIGNAL handshake. Key finding: setting 0x82BF9844=0 fixed error 34
> but broke scene object creation, causing error 24 downstream.

---

## 54. Error 34 Fix Side Effect — 0x82BF9844 Dual Usage

### The Address 0x82BF9844 (offset -26556 from r26/r29 = 0x82C00000)

This address is used at **multiple states** with **different expected values**:

| State | Check | Expected Values | Result if Wrong |
|-------|-------|----------------|-----------------|
| **4** | Switch on value | 0 or 1 → success | 2 → error 34; >4 → error 34 |
| **4** | Conditional `(value-3) <= 1` | 3 or 4 → call sub_8223F308 (scene creation) | 0,1,2 → skip scene creation |
| **10** | Check `r11 ∈ {0,1,3,4}` | 0,1,3,4 → advance to state 11 | Other → cleanup, return 0 |
| **11** | Check `r11 ∈ {3,4}` | 3 or 4 → advance to state 12 | Other → cleanup, return 0 |

### The Problem

Setting 0x82BF9844 = 0 (our error 34 fix) satisfies state 4's switch (case 0 → success)
but **skips the scene creation conditional** (0-3 = -3, unsigned > 1). This means:
- `sub_8223F308` is never called
- The scene object pointer at 0x82BF3A88 stays NULL
- State 11 sees value 0, which is NOT 3 or 4 → cleanup → return 0

### The Correct Value

**0x82BF9844 must be 3 or 4** to satisfy ALL state requirements:
- State 4 switch: case 3 → conditional check (reads byte, sets flag); case 4 → different check
- State 4 scene creation: `(3-3) <= 1` = true → calls sub_8223F308 ✓
- State 10: value 3 or 4 → advance ✓
- State 11: value 3 or 4 → advance to state 12 ✓

### What Writes 3 or 4 on Xbox 360?

On Xbox 360, the outer state machine's **sub_822422E0 (state 5)** runs the save/content
state machine which sets this value based on the game mode:
- **3** = Standard game (single player base game)
- **4** = Episode content (TLAD/TBOGT DLC)

The sub_822422E0 hook currently resets 0x82BF9834 (wrong address!) from 2→0. The ACTUAL
platform mode that matters is at **0x82BF9844**.

---

## 55. Scene Object Creation Chain

### State 4 Scene Creation Path (when 0x82BF9844 = 3 or 4)

```
State 4 (loc_82242B44):
  1. Read 0x82BF9844, subtract 3, compare to 1
     ├─ If (value-3) <= 1 (value is 3 or 4): r10=1
     └─ If (value-3) > 1 (value NOT 3/4): r10=0 → SKIP scene creation
  2. If r10=1:
     - Call sub_8223F308(1, r30) → creates scene object
     - Load scene pointer from r30+4
     - Write to 0x82BF3A88 (scene object global)
  3. Continue to byte checks...
```

### Why sceneObj = 0x00000000

With 0x82BF9844 = 0:
- `(0-3)` unsigned = 0xFFFFFFFD > 1 → r10 = 0
- sub_8223F308 never called
- 0x82BF3A88 stays at the value state 4 zeroed it to (line 85154)

With 0x82BF9844 = 3:
- `(3-3)` = 0 ≤ 1 → r10 = 1
- sub_8223F308 called → scene object created
- 0x82BF3A88 gets valid pointer

---

## 56. Error 24 — Origin and Flow

### Where Error 24 Is Written

**Function**: `sub_82240F80` (scene render state machine)
**Location**: gta4_recomp.6.cpp line 81335
**Trigger**: `sub_8284B490()` returns 1 (scene validation failure)

### Error 24 Flow Chain

```
1. sub_82242910 reaches state 11, returns 0 (cleanup path — value not 3/4)
2. sub_822438B0 state 2 sees return 0 → transitions to state 3
3. sub_822438B0 state 3 calls sub_82240F80(0)
4. sub_82240F80 calls sub_8284B490() → returns 1 (no valid scene)
5. sub_82240F80 writes 24 to 0x82A9546C → returns 2
6. sub_822438B0 state 3 reads error code: 24 ≠ 33 → transitions to state 7
7. State 7 = error recovery dialog loop (never exits in recomp)
```

### Error Code Semantics (GTA IV Internal)

| Range | Category |
|-------|----------|
| 6-10 | Resource initialization |
| 14-22 | Scene dependency setup |
| **23-27** | **Scene object creation** (error 24 = specific object type failed) |
| 28-34 | Advanced scene setup |
| 33 | Special: handled gracefully by outer state machine |
| 34 | Platform mode switch error |

---

## 57. Outer State Machine State 7 — Error Recovery

### What State 7 Does

sub_822438B0 state 7 (loc_82243BC8) is an **error recovery dialog handler**:
1. Calls `sub_8223CC68(r30, 2)` — re-initialize dialog with error context
2. Calls `sub_82242608()` — check if error is cleared
3. If error cleared (return 0) → exit state 7
4. If error persists (return 1) → loop

### Why It Loops Forever

`sub_82242608` reads the error code at the error storage address. Error 24 is never
cleared because no XAM dialog system is running in the recomp to dismiss it. On Xbox
360, this would show a "Load failed" dialog and wait for the user to press A.

### The READY-SIGNAL Loop (0x82BF9B70)

This is a **separate system** — the XAM readiness handshake:
- sub_82254FE0 writes 1 ("setup complete")
- sub_8214C8C8 increments to 2
- Counter never reaches 4 → loop repeats

This runs on the same thread as part of the error recovery path's dialog flow
(sub_8223F9F0), not as a separate thread.

---

## 58. States 8-11 — Successful Progression Details

The state machine successfully traversed states 8-11 before the error:

| State | Purpose | Loop Count | Key Functions |
|-------|---------|------------|---------------|
| **8** | Stream/resource loading | ~10 | sub_8284AB10, sub_8284B490, sub_8284B430 |
| **9** | Event fire trigger | 1 (immediate) | sub_8284ABA0(r7=75), sub_8223D2F0 |
| **10** | Fire completion poll | ~12 | sub_8284ABD0 (polls), sub_8284ABF8 |
| **11** | Final validation | 1 | sub_8223D400, checks 0x82BF9844 |

State 8 waits for resource streaming. State 10 polls for fire event completion.
These work correctly — the issue is only at state 11's final gate.

---

## 59. State 6→8 Skip — Not a Problem

State 6 calls `sub_8284AAE0()`:
- Returns non-zero (success) → state 6→8 (fast path, normal)
- Returns zero (failure) → state 6→7 (fallback initialization)

State 7 (scene creation internal, NOT outer state 7) is a **fallback path** that
calls `sub_8223CB60` and `sub_8223F9F0(3)`. Skipping it is the SUCCESS path.

---

## 60. Revised Fix — Set 0x82BF9844 to 3 Instead of 0

### The Fix

```cpp
// In sub_82242910 hook:
if (stateAfter == 4) {
    uint32_t platMode = PPC_LOAD_U32(0x82BF9844);
    if (platMode == 2 || platMode > 4) {
        PPC_STORE_U32(0x82BF9844, 3);  // Base game = 3 (NOT 0!)
        // Value 3: satisfies state 4 switch (case 3)
        //          enables scene creation (3-3=0 ≤ 1)
        //          satisfies state 10 check (3 ∈ {0,1,3,4})
        //          satisfies state 11 check (3 ∈ {3,4})
    }
}
```

### Why 3 and Not 4

- **3** = Standard single-player game mode (base GTA IV)
- **4** = Episode content (TLAD/TBOGT DLC)
- For base game startup, 3 is the correct value
- State 4 case 3 checks a byte at 0x82BF984F — if zero, proceeds normally

### Risk Assessment

| Factor | Risk |
|--------|------|
| State 4 case 3 logic | LOW — checks a byte then proceeds |
| State 10 value check | NONE — 3 is in allowed set {0,1,3,4} |
| State 11 value check | NONE — 3 is in allowed set {3,4} |
| Scene creation | FIXED — sub_8223F308 will be called |
| Error 24 | SHOULD BE FIXED — scene object will exist |

### Interaction with Existing Hooks

- sub_822422E0 reset (0x82BF9834: 2→0) — **different address**, no conflict
- sub_82242910 state 0→4 interception — **complementary**, still needed
- sub_822440F8 bypass — **orthogonal**, different state machine
