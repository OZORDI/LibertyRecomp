# sub_82142230 -- Top-Level Front-End State Machine

**File**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.0.cpp`, lines 5303-6277

## 1. Overview

`sub_82142230` is the master front-end orchestrator. It is an infinite loop (never returns
during normal gameplay) that drives the entire boot-to-gameplay pipeline through a 10-state
switch (states 0-9) on register `r29`.

It calls:
- `sub_822414E8` -- user sign-in detection (state 0)
- `sub_8223DDA8` -- storage device selection (state 1)
- `sub_8223DEE8` -- content enumeration / DLC (state 2)
- `sub_82219AC0` -- save data tick (states 0-path, 3)
- `sub_821406C8` -- game config / settings init (state 3)
- `sub_8224FA48` -- ready-signal check (state 3)
- `sub_822440F8` -- save device selection UI (state 4)
- `sub_822422E0` -- scene dispatch / level load (state 5)
- `sub_822438B0` -- scene creation state machine (states 5-fall-through, 6)

## 2. Call Chain (Who Calls This)

```
sub_821B3598 (main game thread entry)
  -> sub_82140088 (front-end frame loop)
       -> sub_82142230 (THIS FUNCTION -- infinite loop, never returns until exit states 7/8/9)
```

`sub_82140088` has its own pre-loop: it calls `sub_821458B8()` in a spin until it returns 0
(ready), then enters `sub_82142230`.

## 3. State Variable

**r29** -- held in a callee-saved register across the loop. Initialized to 0 at entry
(line 5329: `mr r29, r25` where r25=0).

This is NOT backed by a memory location. The state is entirely register-held, persisting
across iterations of the `loc_821422A0` loop.

## 4. Register Preloads (Prologue Constants)

All computed via Python (`lis` + `addi`):

| Register | Value | Purpose |
|----------|-------|---------|
| r23 | 0x820B9138 | String/name ptr (arg to sub_8224DC48 in state 3 error-restart path) |
| r20 | 0x820B9120 | String/name ptr (arg to sub_82215530, "loaded" callback label) |
| r15 | 0x820B9108 | String/name ptr (arg to sub_82215530, "loading" callback label) |
| r19 | 0x820B90F4 | String/name ptr (arg to sub_822BCA90, alternate message) |
| r18 | 0x820B90E0 | String/name ptr (arg to sub_822BCA90, primary message) |
| r22 | 0x82BCC1F8 | Object ptr (CLoadingScreens / loading screen manager) |
| r14 | 0x82C00000 | Base for global state (lis -32064) |
| r27 | 0x831E0000 | Base for global objects (lis -31970) |
| r16 | 0x82B2F7E0 | String ptr (arg to sub_821B6FD0, sub_821B5A68, sub_822BCA90) |
| r21 | 0x82C00000 | Same as r14 |
| r26 | 0x831D0000 | Base for flags (lis -31971) |
| r30 | 0x831D0000 | Same as r26 (reused/overwritten in later states) |
| r28 | 0x82A90000 | Base for save/profile data (lis -32087) |
| r17 | 0x82A90000 | Same as r28 |
| r24 | 1 | Constant 1 |
| r25 | 0 | Constant 0 |

## 5. Key Memory Addresses

All computed via Python from register+offset:

| Address | Type | Description |
|---------|------|-------------|
| 0x831E4DD4 | u32 ptr | `r27+19924` -- global object pointer (save manager / game state object) |
| 0x831D5330 | u32 | `r30+21296` -- loading screen transition counter (0/1/2) |
| 0x82A95474 | u32 | `r28+21620` -- save slot result |
| 0x82A95478 | u32 | `r17+21624` -- arg passed to sub_822422E0 in state 5 |
| 0x831D5327 | u8 | `r26+21287` -- "restart needed" flag (state 3 error-restart path) |
| 0x831D5348 | u8 | `r10+21320` -- cleared on exit path (states 7/8/9) |
| 0x831D5337 | u8 | `r30+21303` -- set=1 on exit, cleared after sub_82145820 |
| 0x831D5324 | u8 | `r10+21284` -- cleared after sub_82145820 |
| 0x82BF9828 | u32 | `r21+(-26584)` -- cleared to 0 on state 0->1 transition (= sub_8223DDA8 state var) |
| 0x82BF9D81 | u8 | `r14+(-25215)` -- byte flag checked in state 3 |
| 0x82B394F8 | u32 | Store: save count (from sub_8221B198 result) |
| 0x82B394FC | u32 | Store: save count when profile is valid + count > 0 |
| 0x82B39518 | u32 | Load: value used as arg in state 9 exit path |
| 0x82BF9858 | u32 | `r11+(-26536)` where r11=0x82C00000 -- ptr loaded in states 7 and 9 |
| 0x82BF9B70 | u32 | Ready-signal (not directly in this function, but read by sub_8224FA48 in state 3) |

## 6. State-by-State Analysis

### Loop Top: loc_821422A0
Every iteration:
1. `sub_82849918(ctx, base)` with r3=1 -- yield / sleep 1ms
2. `sub_821428C8(ctx, base)` -- per-frame tick (input, network, etc.)
3. Switch on r29 (states 0-6 in the jump table; >6 falls to loc_821425F0)

---

### STATE 0: User Sign-In Detection
**Handler**: loc_821422E8

Calls `sub_822414E8()` which scans controller ports 0-3 for signed-in users
(via `sub_829DB7F8`), checking `0x82A9172C` for cached user index.

**Return values**:
- `r3 == 1` -> transition to **state 1** (r29=1), clears `0x82BF9828` to 0
- `r3 == 2` -> jump to loc_821422FC: load object from `0x831E4DD4`, call `sub_82219AC0`
  (save tick), check byte at `obj+361`:
  - If `obj+361 == 0` -> r29 = 3 (skip to state 3: no save data, fast-path)
  - If `obj+361 != 0` -> r29 = 4 (skip to state 4: have saves, go to save device)
- Other -> loop back (stay in state 0)

---

### STATE 1: Storage Device Selection
**Handler**: loc_8214232C

Calls `sub_8223DDA8()` -- storage device/profile enumeration state machine.
State var at `0x82BF9828`.

**Return values**:
- `r3 == 1 or 2` -> transition to **state 2** (r29=2)
- Other -> loop back (stay in state 1)

---

### STATE 2: Content Enumeration / DLC Check
**Handler**: loc_82142344

Calls `sub_8223DEE8()` -- content enumeration (DLC, title updates).
State var at `0x82BF98F4`.

**Return values**:
- `r3 == 1` -> jump to loc_821422FC (same as state 0 path 2): load save object,
  call `sub_82219AC0`, check `obj+361`:
  - `obj+361 == 0` -> r29 = 3
  - `obj+361 != 0` -> r29 = 4
- `r3 == 2` -> r29 = 8, jump to exit path (loc_82142608)
- Other -> loop back (stay in state 2)

---

### STATE 3: Game World Init / Resource Loading (THE BIG STATE)
**Handler**: loc_82142360

This is the most complex state. It performs:

1. **Save object tick**: loads object from `0x831E4DD4`, calls `sub_82219AC0`, checks `obj+361`
   - If `obj+361 == 0` -> jump to loc_821425AC (r29=4, skip to save device)

2. **Loading screen counter**: reads `0x831D5330`:
   - If 0: set to 1
   - Calls `sub_8223CAD8()` (front-end scene setup)

3. **Game config init**: calls `sub_821406C8()` -> returns ptr in r31
   - If ptr == NULL -> jump to loc_82142598 (store 0 to 0x831D5330, call sub_8221B198,
     store result to 0x82A95474, r29=4)

4. **Ready-signal check**: calls `sub_8224FA48()` -- reads `0x82BF9B70`
   - If returns nonzero (dialog pending) -> skip to loc_82142504
   - Also checks `0x82BF9D81` byte flag
   - Also calls `sub_821B6FD0(r16=0x82B2F7E0)` -- checks if level stream is ready

5. **Resource pair checks** (three pairs of offset checks on r31 object):
   - Pair 1: `obj+68` and `obj+148` -- both nonzero AND obj+148==0 means "ready"
   - Pair 2: `obj+72` and `obj+152` -- same pattern
   - Pair 3: `obj+4` and `obj+84` -- same pattern
   - If ANY pair is ready -> loc_82142454 (loading screen transition logic)

6. **Loading screen transition** (loc_82142454):
   - If `0x831D5330 == 1`: display message r18 (0x820B90E0), set counter to 2
   - If `0x831D5330 != 1`: display message r19 (0x820B90F4), set counter to 1
   - Calls `sub_82215530(r22=0x82BCC1F8, r15=0x820B9108)` -- register loading callback

7. **"Loaded" callback** (loc_8214248C): checks `obj+56` and `obj+136`:
   - If ready: calls `sub_82215530(r22=0x82BCC1F8, r20=0x820B9120)` -- register "loaded" cb
   - Reads loading screen counter, stores save result:
     - Counter==1 -> store 1 to 0x82A95474
     - Counter==2 -> store 2 to 0x82A95474
     - Otherwise -> call sub_8221B198, store result to 0x82A95474
   - Clear `0x831D5330` to 0
   - **r29 = 4** (transition to state 4)

8. **Signoff checks** (loc_82142504):
   - `sub_826CBD20()` -- check if user signed out
   - `sub_826CBD08()` -- check if storage device removed
   - If either true -> r29=2 (back to state 2), clear `0x831D5330`

9. **Error-restart path**: calls `sub_8223DAA0()` twice:
   - If returns nonzero AND `0x831D5327` flag set:
     clear flag, clear `0x831D5330`, r29=0 (restart from state 0),
     call `sub_8224DC48(r23=0x820B9138, 0, -1)` -- error notification
   - If returns zero AND `0x831D5327 == 0`: set flag to 1

---

### STATE 4: Save Device Selection UI
**Handler**: loc_821425B4

Calls `sub_822440F8()` -- save device selection state machine.
State var at `0x82BF99D4`.

**Return values**:
- `r3 == 1` -> r29 = 7 (exit: transition to state 7 via loc_821425FC)
- `r3 == 2` -> r29 = 5 (transition to state 5)
- Other -> loop back (stay in state 4)

---

### STATE 5: Scene Dispatch (Level Load)
**Handler**: loc_821425D0

1. Loads arg from `0x82A95478` (r17+21624)
2. Calls `sub_822422E0(arg)` -- scene dispatch / level selection
3. Sets r29 = 6

**Falls through** to state 6 handler.

---

### STATE 6: Scene Creation State Machine
**Handler**: loc_821425DC (also fall-through from state 5)

Calls `sub_822438B0()` -- the scene creation / outer state machine.
State var at `0x82BF9838`.

**Return values**:
- `r3 == 0` -> r29 = 9 (game is loaded, transition to exit-state 9)
- `r3 == 2` -> r29 = 7 (error, jump to state 7)
- `r3 == 1` (implied other) -> check if r29 <= 6: if so, loop back
  (sub_822438B0 is still working); if r29 > 6, fall to exit

---

### STATES 7, 8, 9: Exit / Finalization

All three reach loc_82142608 (the common exit prologue):

**Common exit setup** (loc_82142608):
1. Clear `0x831D5348` to 0
2. Set `0x831D5337` to 1
3. Call `sub_821B5A68(r16=0x82B2F7E0)` -- finalize level stream
4. Call `sub_82220118(0x82BCF998, 0)` -- clear loading screen state
5. Load r31 = object at `0x82BEFA40`
6. Call `sub_8222DB48(r31)` -- object finalization
7. Call `sub_8214AD88()` -- frontend cleanup
8. Call `sub_821ED6D8(0x82B978B0, 0, 0, 1)` -- scene param setup
9. If `obj+20` ptr is valid, set `*(obj+20+1376) = 1`
10. Call `sub_82145820()` -- finalize frame
11. Clear `0x831D5337` to 0, clear `0x831D5324` to 0
12. Branch by r29:

**STATE 8** (r29 == 8):
- Calls `sub_826CBD08()` -- check storage device
- If device gone: jump to loc_821426F8 (same as state 7 path)
- Otherwise:
  - Call `sub_821C12D0()` -- cleanup
  - Call `sub_826CB600()` -> r30 = save count
  - Load save object from `0x831E4DD4`, call `sub_82219AC0`
  - If `obj+361 != 0` AND save count > 0: store count to `0x82B394FC`
  - Store count to `0x82B394F8`
  - Call `sub_82142B10(1)` -- mark game ready
  - Call `sub_8223E028()` -- state machine exit callback
  - **RETURN** (function exits, game is running)

**STATE 7** (r29 == 7):
- loc_821426F8:
  - Load ptr from `0x82BF9858`
  - Call `sub_82141F00(ptr, 0, 1, 0)` -- scene teardown
  - Setup fade-out screen: load floats, call `sub_8233C230(obj+20+1052, ...)`
  - Call `sub_8223E028()` -- exit
  - **RETURN**

**STATE 9** (r29 == 9):
- loc_82142758:
  - Load ptr from `0x82BF9858`
  - Call `sub_82141F00(ptr, 1, 1, 0)` -- scene finalization (r4=1 = "success")
  - r30 = result
  - Call `sub_826CDEB8()` -- post-load validation
  - If r30 == 0 (sub_82141F00 failed): jump to loc_82142870 (error path)
  - Call `sub_8223F800()`:
    - If returns 2: call `sub_8223DC10(1)`, load save object, tick it,
      check profile, setup scene via `sub_82141F00`, fade-in screen, **RETURN**
  - Otherwise (returns != 2): loc_82142818:
    - Call `sub_82141BE0()` -- world init
    - Call `sub_822423E0(1)` -- start gameplay
    - Setup loading screen with ARGB=0xFF000000 overlay
    - Call `sub_8233C230`, `sub_8223E028`
    - **RETURN** (game is now in gameplay)

  **Error sub-path** (loc_82142870, if sub_82141F00 returned 0):
    - Call `sub_8223CF68()` -- error handling
    - Call `sub_8223DC10(1)` -- reset
    - Fade-in screen, **RETURN**

## 7. Interaction with 0x82BF9B70 (Ready-Signal)

`sub_82142230` does NOT read or write `0x82BF9B70` directly. Instead, it interacts
with it indirectly through **state 3** via:

- **sub_8224FA48()** (line 5519): reads `0x82BF9B70`. Returns 0 when value is -1
  ("no dialog pending"), returns 1 when value >= 0 ("dialog result available").
  In state 3, if this returns nonzero, the resource-pair checks are skipped.

- **sub_8224FA38()**: resets `0x82BF9B70` to -1 ("not ready"). Called by sub-functions.

- **sub_82254FE0()**: writes 1 to `0x82BF9B70` ("ready"). Called when XAM dialog completes.

- **sub_8214C8C8()**: increments `0x82BF9B70` toward 4. Called as a readiness counter.

The ready-signal flow:
1. `0x82BF9B70` starts at -1 (memory default)
2. When XAM dialog/sign-in completes, sub_82254FE0 writes value to it
3. sub_8224FA48 (polled in state 3) reports whether it's "not -1"
4. In the recomp, XAM dialogs don't exist, so this value is managed by hooks

## 8. How sub_822422E0 and sub_822438B0 Are Called

### sub_822422E0 (Scene Dispatch)
Called in **state 5** (loc_821425D0, line 5839):
```
r3 = PPC_LOAD_U32(0x82A95478)   // load level/scene ID
sub_822422E0(ctx, base)           // dispatch scene load
r29 = 6                           // advance to state 6
```
sub_822422E0's own state var is at `0x82BF9834`. It reads `0x82BF9838` (the sub_822438B0
state var) and if already >= 1, returns 2. Otherwise it copies the save game filename
and sets its state to 1, returning 2.

### sub_822438B0 (Scene Creation SM)
Called in **state 6** (loc_821425DC, line 5845), and also as fall-through from state 5:
```
sub_822438B0(ctx, base)           // tick scene creation
if (r3 == 0) r29 = 9             // done -> state 9 (success)
if (r3 == 2) r29 = 7             // error -> state 7 (teardown)
// r3 == 1 means "still working", stay in state 6
```
sub_822438B0's own state var is at `0x82BF9838`. It has 8 internal states (0-7)
managing the full scene creation pipeline (see doc 02).

## 9. Expected Boot-to-Gameplay Flow

```
STATE 0: sub_822414E8 scans for signed-in user
  -> user found (or emulated) -> returns 1
STATE 1: sub_8223DDA8 enumerates storage devices
  -> storage ready -> returns 1 or 2
STATE 2: sub_8223DEE8 enumerates DLC/content
  -> content enumerated -> returns 1
  -> loads save object, checks obj+361 (has saves?)
  -> has saves: STATE 4
  -> no saves: STATE 3
STATE 3: Heavy init -- front-end scene setup, game config, resource loading
  -> sub_821406C8 returns config ptr
  -> sub_8224FA48 checks ready-signal (0x82BF9B70)
  -> resource pairs loaded -> loading screen transitions
  -> all loaded -> STATE 4
STATE 4: sub_822440F8 -- save device UI
  -> returns 2 -> STATE 5
STATE 5: sub_822422E0 -- dispatch level load
  -> immediately sets r29=6, falls through
STATE 6: sub_822438B0 -- scene creation SM (8 internal states)
  -> returns 0 (done) -> STATE 9
  -> returns 2 (error) -> STATE 7
STATE 9: sub_82141F00(ptr, 1, 1, 0) -- finalize scene
  -> sub_8223F800() -> sub_822423E0(1) -- start gameplay
  -> RETURN (game is playing)
```

## 10. Error / Restart Paths

| Trigger | From State | Goes To | Mechanism |
|---------|------------|---------|-----------|
| User signs out (sub_826CBD20) | 3 | 2 | Clear loading counter, restart enumeration |
| Storage removed (sub_826CBD08) | 3 | 2 | Same as above |
| sub_8223DAA0 error + restart flag | 3 | 0 | Full restart from sign-in |
| sub_822438B0 returns 2 (error) | 6 | 7 | Scene teardown + fade out |
| sub_822440F8 returns 1 | 4 | 7 | Save device error |
| sub_8223DEE8 returns 2 | 2 | 8 | DLC error -> exit path |
| sub_82141F00 returns 0 | 9 | error sub-path | Scene load failure |

## 11. Key Sub-Function Summary

| Function | Role | State Var |
|----------|------|-----------|
| sub_822414E8 | User sign-in scan (ports 0-3) | 0x82A9172C |
| sub_8223DDA8 | Storage device enumeration | 0x82BF9828 |
| sub_8223DEE8 | Content/DLC enumeration | 0x82BF98F4 |
| sub_822440F8 | Save device selection UI | 0x82BF99D4 |
| sub_822422E0 | Scene dispatch / level selection | 0x82BF9834 |
| sub_822438B0 | Scene creation state machine | 0x82BF9838 |
| sub_8224FA48 | Ready-signal reader | reads 0x82BF9B70 |
| sub_821406C8 | Game config / settings init | returns ptr |
| sub_82219AC0 | Save data tick | takes object ptr |
| sub_8221B198 | Save slot count query | returns count |
| sub_82145820 | Frame finalization | -- |
| sub_8223E028 | State machine exit callback | -- |
| sub_82142B10 | Mark game ready | -- |
| sub_822423E0 | Start gameplay (with arg=1) | -- |
