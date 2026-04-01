# 18 - Unified Call Graph: Boot to Gameplay

This document synthesizes all research from docs 01-11 into a single call graph
showing the entire chain from game boot to scene creation and gameplay entry.

---

## 1. Top-Level Architecture

The GTA IV recomp boot-to-gameplay pipeline is a **four-level nested state machine**:

```
LEVEL 0: CRT Entry + Engine Init
  xstart (0x82000000)
    -> sub_82120000 (RAGE init, replaced by GameInit::Initialize)
      -> sub_821200D0 (post-init: profiles/saves)
        -> sub_82142230 (front-end state machine, LEVEL 1)

LEVEL 1: sub_82142230 — Front-End State Machine (register r29, states 0-9)
  state 0 -> sub_822414E8         — sign-in gate
  state 1 -> sub_8223DDA8         — storage device gate
  state 2 -> sub_8223DEE8         — save/load check
  state 3 -> player slot checks   — content transition detection
  state 4 -> sub_822440F8         — Xbox save device selection (REPLACED: returns 2)
  state 5 -> sub_822422E0         — game start / level selection (LEVEL 2a)
  state 6 -> sub_822438B0         — scene loading (LEVEL 2b)
  state 7 -> exit sequence
  state 8 -> exit sequence
  state 9 -> exit sequence

LEVEL 2a: sub_822422E0 — Level Selection (0x82BF9834, states 0-3)
  state 0 -> select level name by game_mode (12=GTA4, 13=TLAD, 14=TBOGT)
  state 1 -> in progress
  state 2 -> done (sets state_phase = 2)

LEVEL 2b: sub_822438B0 — Outer Save/Load Machine (0x82BF9838, states 0-7)
  state 0 -> IDLE (return 0)
  state 1 -> INITIATE: set platformMode=2, clear flags, reset scene state
  state 2 -> sub_82242910 (LEVEL 3)
  state 3 -> sub_82240F80(0) — resource readiness polling
  state 4 -> auto-advance + validate (sub_822446E8, sub_8223F740)
  state 5 -> sub_82240F80(1) — second resource poll
  state 6 -> ready-signal gate (timer + scene finalization)
  state 7 -> teardown (sub_8223CC68 + sub_82242608)

LEVEL 3: sub_82242910 — Scene Creation Machine (0x82BF9848, states 0-14)
  state 0  -> sub_8223DAA0 readiness gate
  state 1  -> network + content check
  state 2  -> alt content check
  state 3  -> pre-load setup
  state 4  -> scene load setup (sub_8223F308)
  state 5  -> content enumeration
  state 6  -> SCENE CREATION (sub_8284AAE0)
  state 7  -> retry path
  state 8  -> monitor scene creation (sub_8284AB10, sub_8284AB70)
  state 9  -> begin resource load (sub_8284ABA0)
  state 10 -> monitor resource load (sub_8284ABD0, sub_8284ABF8)
  state 11 -> post-load validation (sub_8223D400)
  state 12 -> save data pass 1 (sub_822417B0)
  state 13 -> error recovery
  state 14 -> save data pass 2 (sub_822417B0) + final commit
```

---

## 2. Complete Call Graph

### 2.1 CRT to Front-End Entry

```
xstart (0x82000000)
 |
 +-> GameInit::Initialize() [replaces sub_82120000]
 |    |-- sub_8218C600  (core engine init)
 |    |-- sub_82120EE8  (game manager init)
 |    |-- sub_821250B0  (pool allocation)
 |    |-- sub_82124080  (profile system)
 |    |-- sub_82120FB8  (subsystems)
 |    |-- LODHooks::Initialize()
 |    +-- PostFXHooks::Init()
 |
 +-> sub_821200D0 [HOOKED: pre-clears 0x83137BC9, forces runtime phase]
 |    |-- (profiles, saves, streaming init)
 |    +-- sub_82192E00 [HOOKED: zeroes 0x830F5820 streaming-pending flag]
 |
 +-> Main Loop Entry
      |-- sub_821428C8 / sub_82142F90 (frame tick variants)
           |-- sub_8214C8C8 [HOOKED: XAM ready counter]
           |    |-- sub_8224FA48() -- IsReady check
           |    |-- sub_8214B168() -- frame processing
           |    +-- sub_8214B640() -- secondary game state
           |         |-- sub_82254FE0 [HOOKED: writes 1 to 0x82BF9B70]
           |         +-- sub_822422E0 (level selection, 4 call sites)
           |
           +-> sub_82142230 (FRONT-END STATE MACHINE)
```

### 2.2 sub_82142230 — Front-End State Machine

```
sub_82142230 [HOOKED: diagnostic logging]
 |
 STATE 0: sub_822414E8 [HOOKED: diagnostic]
 |  Returns: 1 = ready (-> state 1), 2 = needs restart
 |  GATE: sign-in check
 |
 STATE 1: sub_8223DDA8 [HOOKED: diagnostic]
 |  Returns: 1 or 2 = ready (-> state 2)
 |  GATE: storage device enumeration
 |
 STATE 2: sub_8223DEE8 [HOOKED: diagnostic]
 |  Returns: 1 = needs save work (-> restart), 2 = ok (-> state 8 -> 3)
 |  GATE: save/load system check
 |
 STATE 3: Player Slot Checks
 |  Reads: 0x82BF9D81 (gate byte), 0x831D5327 (XAM ready flag)
 |  Calls: sub_8223DAA0 (readiness), sub_821406C8 [HOOKED: populates player slots]
 |         sub_8223CAD8 (cleanup), sub_8221B198 (profile)
 |  Reads: 0x831D5330 (content transition counter)
 |  GATE: sub_8224FA48 returning 0 (XAM ready = -1 at 0x82BF9B70)
 |  Writes: 0x82A95474 (profile index), 0x831D5330 (transition counter)
 |  -> state 4
 |
 STATE 4: sub_822440F8 [FULLY REPLACED: returns 2]
 |  Original: 7-state Xbox save device/controller selection
 |  Side effect: sets episode index at 0x82A95478
 |  Returns: 1 = error (-> state 7), 2 = success (-> state 5)
 |
 STATE 5: sub_822422E0 [HOOKED: resets 0x82BF9834]
 |  Selects level name by game_mode:
 |    game_mode 0 -> level index 12 (GTA IV)
 |    game_mode 1 -> level index 13 (TLAD)
 |    game_mode 2 -> level index 14 (TBOGT)
 |  Calls: sub_82240480 (format level name)
 |  Writes: 0x82BF3A60 (level name), 0x82BF9834 (state_phase = 2)
 |  -> state 6 (falls through)
 |
 STATE 6: sub_822438B0 [HOOKED: diagnostic logging]
 |  Returns: 0 = scene load complete (-> state 9),
 |           2 = error (-> state 7)
 |  (See LEVEL 2b below)
 |
 STATE 7/8/9: Exit sequence
 |  Clears: 0x831D5327, 0x831D5348, sets 0x831D5337
 |  Calls: sub_8222DB48, sub_82220118, sub_821ED6D8
```

### 2.3 sub_822438B0 — Outer Save/Load Machine (LEVEL 2b)

```
sub_822438B0 [HOOKED: diagnostic logging]
 |
 STATE 0: IDLE -- return 0 (re-entered each frame until STATE set to 1 externally)
 |
 STATE 1: INITIATE
 |  Writes: 0x82A95466 = 1 (content byte), 0x82BF9844 = 2 (platformMode)
 |          0x82BF981E = 0 (clear done flag), 0x82BF9848 = 0 (reset scene state)
 |  -> STATE 2 (falls through immediately)
 |
 STATE 2: ENUMERATE (calls sub_82242910)
 |  sub_82242910 returns:
 |    r3=2 (error): check error_code at 0x82A9546C
 |      if 33 -> set done flag, return 2 (fatal)
 |      else  -> STATE 7 (error/retry)
 |    r3=0 (success): zero 16 bytes at 0x82BF3940, -> STATE 3
 |    r3=1 (working): return 1 (stay in state 2)
 |
 STATE 3: SAVE OPERATION (calls sub_82240F80(0))
 |  Returns: 0 = success -> STATE 4, 2 = error, 1 = working
 |  Checks "SAVE" magic at 0x82BF394C-394F
 |  Errors: 29 (bad magic), 33 (fatal)
 |
 STATE 4: VALIDATE
 |  Immediately -> STATE 5
 |  If SUB_STATE==2: calls sub_822446E8, sub_8223F740
 |  Error: 31 (validation failed)
 |
 STATE 5: WRITE SAVE DATA (calls sub_82240F80(1))
 |  Returns: 0 = success -> STATE 6, 2 = error, 1 = working
 |
 STATE 6: FINALIZE
 |  Checks timer struct at 0x82BF3934 (counter >= 3000ms)
 |  Calls: sub_8223CAD8, sub_826CD808, sub_829DBAA8
 |  Returns: 0 when scene object stored, 2 when done with reset
 |
 STATE 7: ERROR / RETRY
 |  Calls: sub_8223CC68(r30, 2), sub_82242608
 |  Returns: 1 = retry in progress, 2 = done with reset
```

### 2.4 sub_82242910 — Scene Creation Machine (LEVEL 3)

```
sub_82242910 [HOOKED: pre-sets platformMode=3, intercepts 0->4 fast path]
 |
 STATE 0: READINESS GATE
 |  sub_8223DAA0() -- device/sign-in check
 |    ready (nonzero) -> STATE 4 (FAST PATH -- problematic in recomp!)
 |                       + sub_8284B4B0(sceneName, sceneInfoPtr)
 |    not ready (0)   -> STATE 1 (NORMAL PATH)
 |
 STATE 1: NETWORK + CONTENT CHECK
 |  sub_826CBA70() -- network session active?
 |    active -> sub_826CB6B8() teardown, -> STATE 3
 |  sub_8223DAA0() -- ready now?
 |    ready -> STATE 3
 |  sub_8223F9F0(0,0,&out) -- content enumeration
 |    pending (ret=0) -> wait
 |    content found   -> STATE 2
 |    no content      -> STATE 3
 |
 STATE 2: ALT CONTENT CHECK
 |  sub_8223CB60() + sub_826CBA70() + sub_8223DAA0() + sub_8223F9F0(1,0,&out)
 |  -> STATE 3 or ERROR 6
 |
 STATE 3: PRE-LOAD SETUP
 |  sub_826CBA70() -- wait if network busy
 |  sub_8223DAA0() -- not ready -> STATE 2
 |  sub_82240AB0() -- pre-load setup
 |  ready -> STATE 4
 |
 STATE 4: SCENE LOAD SETUP (CRITICAL)
 |  sub_8223DB20() -- abort check (nonzero -> ERROR 33)
 |  platformMode switch:
 |    {3,4} -> sub_8223F308(1, sceneStruct) -- load scene file
 |    {0,1} -> no scene file load
 |    {2}   -> ERROR 34
 |    {>4}  -> ERROR 34
 |  Flag checks: 0x82BF981E (done flag), 0x82BF981F (scene loaded)
 |  sub_82240B08() -- content notification -> STATE 9
 |  -> STATE 5 (normal) or STATE 9 (content ready)
 |
 STATE 5: CONTENT ENUMERATION
 |  sub_8223DB20() -- abort check
 |  sub_8223F9F0(2,0,&out) -- storage device selector UI
 |  content available -> STATE 6
 |  no content        -> STATE 7
 |
 STATE 6: *** SCENE CREATION *** (the key action)
 |  sub_8223DB20() -- abort check
 |  sub_826CBA70() -- wait if network busy
 |  sub_8284AAE0(sceneName, sceneInfoPtr, 1, pendingVal, 1, onlineFlag)
 |    success -> STATE 8
 |    failure -> ERROR 7
 |
 STATE 7: RETRY WITHOUT CONTENT
 |  sub_8223DB20() + sub_8223CB60() + sub_8223F9F0(3,0,&out)
 |  content found -> STATE 6
 |  no content    -> ERROR 6
 |
 STATE 8: MONITOR SCENE CREATION
 |  sub_8284AB10(sceneName, sceneInfoPtr) -- check started?
 |    not started -> wait
 |  sub_8284AB70(sceneName, sceneInfoPtr) -- advance
 |  sub_8223DB20() -- abort check
 |  sub_8284B490() -- get status
 |  sub_8284B430() -- get result
 |  sub_82240B08() -- secondary validation
 |  Complete: -> STATE 9 (or STATE 0 restart, or STATE 7 retry)
 |
 STATE 9: BEGIN RESOURCE LOAD
 |  sub_8223DB20() + sub_82240B78() -- gate checks
 |  sub_8223D2F0() -- pre-load setup (zero-fill progress)
 |  sub_8284ABA0(sceneName, sceneInfoPtr, 1, resCallback, 75)
 |    success -> STATE 10
 |    failure -> ERROR 14
 |
 STATE 10: MONITOR RESOURCE LOAD
 |  sub_8284ABD0() -- poll progress
 |  sub_8284ABF8() -- advance
 |  sub_8284B490() + sub_8284B430() -- verify
 |  Complete, mode {0,1,3,4} -> STATE 11
 |  Complete, other mode     -> DONE (r3=0)
 |
 STATE 11: POST-LOAD VALIDATION
 |  sub_8223DB20() + sub_82240B78() -- gate checks
 |  sub_8223D400() -- verify saved data names
 |  mode {3,4} -> STATE 12
 |  mode {0,1} -> DONE (r3=0) via sub_8223CAD8
 |
 STATE 12: SAVE DATA PASS 1
 |  sub_822417B0(0, 1, ...) -- initiate save write
 |  error -> r3=2
 |  success -> STATE 14
 |
 STATE 13: ERROR RECOVERY
 |  sub_8223DB20() + sub_82240B78()
 |  sub_8223F9F0(4, -value, &out) -- confirmation dialog
 |  confirmed -> STATE 0 (restart)
 |  error     -> ERROR 6
 |
 STATE 14: SAVE DATA PASS 2 + FINAL COMMIT
 |  sub_822417B0(0, 0, ...) -- poll save completion
 |  [STATE_SUBSTRUCT] < 0 -> STATE 13
 |  Complete:
 |    sub_8223F790() -- device-specific save path check
 |    sub_8223CAD8() -- cleanup
 |    sub_8223DB90() -- finalize
 |    -> DONE (r3=0) or STATE 0 (reset)
```

---

## 3. Function Classification

### 3.1 GATE Functions (return ready/not-ready, block progress)

| Function | Address | What It Gates | Returns | Xbox-Dependent? |
|----------|---------|---------------|---------|-----------------|
| sub_8223DAA0 | 0x8223DAA0 | Device enumeration + sign-in | 0=not ready, 1=ready | YES: XamUserGetSigninState |
| sub_8223DB20 | 0x8223DB20 | Sign-in change notification | 0=ok, 1=changed (abort) | YES: XNotifyGetNext(10) -- but hooked to no-op |
| sub_82240B78 | 0x82240B78 | Storage device removal | 0=ok, 1=removed (abort) | YES: XNotifyGetNext(11) -- but hooked to no-op |
| sub_826CBA70 | 0x826CBA70 | Network/streaming complete | 0=done, 1=busy | No (pure memory read) |
| sub_8223CB60 | 0x8223CB60 | Platform mode validity | 0=no storage needed, 1=needed | No (pure logic) |
| sub_82240B08 | 0x82240B08 | Content file handle valid | 0=invalid, 1=valid | Partial (file handle validation) |
| sub_8224FA48 | 0x8224FA48 | XAM dialog readiness | 0=ready (-1), 1=pending | No (pure logic on 0x82BF9B70) |
| sub_8223F9F0 | 0x8223F9F0 | Xbox guide UI dialogs | 0=pending, 1=done; out byte | YES: Xbox guide UI -- HOOKED |
| sub_822414E8 | 0x822414E8 | Sign-in check (state 0) | 1=ready, 2=restart | YES: XAM sign-in |
| sub_8223DDA8 | 0x8223DDA8 | Storage device (state 1) | 1/2=ready | YES: device enumeration |
| sub_8223DEE8 | 0x8223DEE8 | Save/load check (state 2) | 1=need work, 2=ok | Partial |

### 3.2 ACTION Functions (do actual work)

| Function | Address | What It Does | Called From |
|----------|---------|--------------|-------------|
| sub_8223F308 | 0x8223F308 | Load scene file data from binary stream | State 4 of sub_82242910 |
| sub_8284AAE0 | 0x8284AAE0 | **Create scene** (main instantiation) | State 6 of sub_82242910 |
| sub_8284AB10 | 0x8284AB10 | Check scene creation started | State 8 |
| sub_8284AB70 | 0x8284AB70 | Advance scene creation | State 8 |
| sub_8284ABA0 | 0x8284ABA0 | Begin resource load (75-item batch) | State 9 |
| sub_8284ABD0 | 0x8284ABD0 | Poll resource load progress | State 10 |
| sub_8284ABF8 | 0x8284ABF8 | Advance resource load | State 10 |
| sub_8223D2F0 | 0x8223D2F0 | Pre-load setup (zero-fill progress) | State 9 |
| sub_8223D400 | 0x8223D400 | Post-load verification | State 11 |
| sub_822417B0 | 0x822417B0 | Save data write orchestration | States 12, 14 |
| sub_8223F790 | 0x8223F790 | Device-specific save path check | State 14 |
| sub_8223CAD8 | 0x8223CAD8 | Cleanup/teardown | States 11, 14, exits |
| sub_8223DB90 | 0x8223DB90 | Finalize/close save device | States 13, 14 |
| sub_82240F80 | 0x82240F80 | Save operation (read/write phases) | sub_822438B0 states 3, 5 |
| sub_82240AB0 | 0x82240AB0 | Pre-load setup | State 3 of sub_82242910 |
| sub_822422E0 | 0x822422E0 | Level name selection | sub_82142230 state 5 |

---

## 4. Critical Path: Boot to Gameplay

The minimum sequence of function calls needed to go from boot to first frame of gameplay,
skipping all Xbox-specific checks:

```
STEP 1: CRT + Engine Init
  xstart -> GameInit::Initialize() -> sub_821200D0
  STATUS: WORKS (already hooked and functional)

STEP 2: Front-End State 0 (sign-in)
  sub_822414E8 -> returns 1 (ready)
  STATUS: WORKS (hooked, returns appropriate value)

STEP 3: Front-End State 1 (storage device)
  sub_8223DDA8 -> returns 1 or 2 (ready)
  STATUS: WORKS (hooked)

STEP 4: Front-End State 2 (save/load check)
  sub_8223DEE8 -> returns 2 (ok, skip to state 3)
  STATUS: WORKS (hooked)

STEP 5: Front-End State 3 (player slots + content)
  sub_8224FA48 returns 0 (XAM ready = -1, advance)
  sub_821406C8 populates player slot fields
  Content transition counter advances: 0 -> 1 -> 2
  STATUS: WORKS (sub_821406C8 hooked to populate slots)

STEP 6: Front-End State 4 (save device selection)
  sub_822440F8 -> returns 2 (FULLY REPLACED, bypasses Xbox save device UI)
  Side effect: sets 0x82A95478 (episode index)
  STATUS: WORKS (fully replaced)

STEP 7: Front-End State 5 (level selection)
  sub_822422E0 selects level by game_mode
  Writes level name to 0x82BF3A60
  STATUS: WORKS (hooked to reset 0x82BF9834)

STEP 8: Front-End State 6 (scene loading via sub_822438B0)
  sub_822438B0 STATE 1 -> initiates, sets platformMode=2
  sub_822438B0 STATE 2 -> calls sub_82242910
  STATUS: PARTIALLY WORKING — see blocking points below

STEP 9: sub_82242910 (scene creation)
  STATE 0 -> sub_8223DAA0 readiness
  STATE 1-3 -> network/content gates
  STATE 4 -> sub_8223F308 scene file load
  STATE 5-7 -> content enumeration / creation gates
  STATE 6 -> sub_8284AAE0 SCENE CREATION
  STATE 8-10 -> scene/resource loading
  STATE 11-14 -> post-load + save data
  STATUS: PARTIALLY WORKING — hooked but fragile

STEP 10: sub_822438B0 states 3-6 (save operation + finalization)
  sub_82240F80(0) -> resource readiness
  sub_82240F80(1) -> save write
  Timer gate at 0x82BF3934 (3000ms)
  STATUS: DEPENDS ON STEP 9 completing successfully

STEP 11: Front-End State 6 returns 0 (done) -> State 9 (exit)
  Game transitions to gameplay loop
```

### 4.1 Critical Path Function Status

| Step | Function | Works As-Is? | Has Hook? | Needs New Hook? |
|------|----------|-------------|-----------|-----------------|
| 1 | GameInit::Initialize | YES | YES (full replace) | No |
| 1 | sub_821200D0 | YES | YES (wrap+patch) | No |
| 2 | sub_822414E8 | YES | YES (diagnostic) | No |
| 3 | sub_8223DDA8 | YES | YES (diagnostic) | No |
| 4 | sub_8223DEE8 | YES | YES (diagnostic) | No |
| 5 | sub_821406C8 | YES | YES (post-patch) | No |
| 6 | sub_822440F8 | YES | YES (full replace) | No |
| 7 | sub_822422E0 | YES | YES (wrap+patch) | No |
| 8 | sub_822438B0 | PARTIAL | YES (diagnostic) | Maybe (state 6 timer) |
| 9 | sub_82242910 | **PARTIAL** | YES (wrap+patch) | **YES (fragile)** |
| 9 | sub_8223DAA0 | **PROBLEM** | No | **YES** |
| 9 | sub_8223F9F0 | **PROBLEM** | YES (stub) | Verify stub works |
| 9 | sub_8284AAE0 | **UNKNOWN** | No | Needs investigation |
| 10 | sub_82240F80 | UNKNOWN | No | Depends on state 9 |

---

## 5. Blocking Points

These are the specific places where the recomp gets stuck because Xbox-specific
checks never pass or produce wrong values.

### BLOCKER 1: sub_8223DAA0 Fast Path (sub_82242910 State 0)

**Problem**: On Xbox 360, device enumeration is async, so sub_8223DAA0 returns 0 for
several frames. In the recomp, devices are immediately ready, so it returns 1 on
the first call, causing STATE 0 -> STATE 4 (fast path). This skips states 1-3
which perform critical initialization.

**Current Hook**: sub_82242910 hook intercepts the 0->4 transition and resets state to 1.
This forces the normal path but is fragile (timing-dependent).

**Root Cause**: sub_8223DAA0 is unhooked and returns 1 too quickly.

**Fix Options**:
1. Hook sub_8223DAA0 to return 0 on first call
2. Keep current sub_82242910 wrap hook (works but fragile)
3. Full replacement of sub_82242910 (eliminates the problem entirely)

### BLOCKER 2: platformMode Stale Value (sub_82242910 State 4)

**Problem**: sub_822438B0 state 1 writes platformMode = 2 at 0x82BF9844. When
sub_82242910 reaches state 4, it checks platformMode via a switch. Mode 2 triggers
ERROR 34 (invalid mode). The original code expected modes 1-3 would be set during
states 1-3, which were skipped.

**Current Hook**: sub_82242910 hook forces platformMode = 3 before calling __imp__.

**Status**: Working but fragile. If any code between the hook and state 4 overwrites
platformMode, the fix breaks.

### BLOCKER 3: XAM Ready Signal Oscillation (0x82BF9B70)

**Problem**: sub_8214C8C8 increments the ready flag (0x82BF9B70) from 1 toward 4.
But sub_82254FE0 (called later in the same frame via sub_8214B640) resets it back
to 1. The flag oscillates 1->2->1->2 and never reaches 4.

**Current Hook**: sub_82254FE0 is hooked but the oscillation pattern persists.
sub_8214C8C8 is also hooked.

**Impact**: The ready flag at 0x82BF9B70 controls sub_8224FA48. When -1, it returns 0
(advance allowed). The default value is -1 (not initialized), which is correct for
the recomp. This blocker affects state 3 of sub_82142230, which currently works
because the default -1 value causes sub_8224FA48 to return 0.

**Status**: Not currently blocking but could cause issues if the flag gets set to a
non-(-1) value.

### BLOCKER 4: sub_8223F9F0 Xbox Guide UI (sub_82242910 States 1,2,5,7,13)

**Problem**: sub_8223F9F0 implements Xbox guide overlay dialogs (storage device
selection, content check, delete confirmation). These cannot work in the recomp.

**Current Hook**: Hooked at imports.cpp:2035. Needs to return success with outBool=1.

**Status**: Hooked. Needs verification that the stub correctly handles all 5 calling
modes (r3=0,1,2,3,4).

### BLOCKER 5: sub_82240B08 Content Notification (sub_82242910 States 4,8)

**Problem**: sub_82240B08 checks if a content file's async I/O has completed by
calling GetOverlappedResult on the file handle at saveDeviceArray[idx]+56. If no
save container was created, the handle is null, causing sub_82240B08 to return 0.

**Impact**: In state 4, returning 0 calls sub_8223DB90(1) (cleanup) and prevents
transition to state 9. In state 8, returning 0 causes STATE -> 0 (full restart).

**Current Hook**: None.

**Fix**: Hook sub_82240B08 to return 1 (content ready), or ensure the save container
stub sets a valid file handle.

### BLOCKER 6: sub_822438B0 State 6 Timer Gate

**Problem**: State 6 of sub_822438B0 checks a timer at 0x82BF3934. The timer must
reach 3000 (milliseconds) before the state advances. The timer struct's flag byte
(+4) must be 1 and mode (+8) must be 2 for the timer comparison to activate.

**Impact**: If the timer is not incrementing (no tick function advancing it), state 6
loops forever returning 1.

**Current Hook**: None specific to the timer. sub_822438B0 has diagnostic logging only.

**Status**: Unknown -- depends on whether the timer increment function runs correctly
in the recomp.

### BLOCKER 7: sub_8284AAE0 Scene Creation (sub_82242910 State 6)

**Problem**: sub_8284AAE0 is an index wrapper around sub_8284A0F8, which creates an
async save container via XContent APIs. If these APIs are not properly stubbed,
the function returns 0 (failure), triggering ERROR 7.

**Current Hook**: None.

**Fix**: Needs either:
1. XContent API stubs that set device element state = 3, result = 0
2. Direct hook on sub_8284AAE0 to return 1 (success)

---

## 6. Dependency Matrix

### 6.1 State Machine State Dependencies

Each state machine state depends on specific memory addresses being set correctly.

#### sub_82142230 (Front-End)

| State | Depends On | Address | Required Value | Set By |
|-------|-----------|---------|---------------|--------|
| 0->1 | sub_822414E8 return | (register) | 1 | sub_822414E8 |
| 1->2 | sub_8223DDA8 return | (register) | 1 or 2 | sub_8223DDA8 |
| 2->3 | sub_8223DEE8 return | (register) | 2 | sub_8223DEE8 |
| 3->4 | XAM ready flag | 0x82BF9B70 | -1 (0xFFFFFFFF) | default / sub_8224FA38 |
| 3->4 | gate byte | 0x82BF9D81 | 0 (skip check) | unknown |
| 3->4 | XAM flag | 0x831D5327 | transitions 0->1 | sub_82142230 state 3 |
| 3->4 | content counter | 0x831D5330 | transitions 0->1->2 | sub_82142230 state 3 |
| 3->4 | profile index | 0x82A95474 | valid (from sub_8221B198) | sub_82142230 state 3 |
| 4->5 | sub_822440F8 return | (register) | 2 | hook (returns 2) |
| 4->5 | episode index | 0x82A95478 | 0 (GTA4 base) | sub_822440F8 hook |
| 5->6 | sub_822422E0 return | (register) | 0 | sub_822422E0 |
| 5->6 | state_phase | 0x82BF9834 | 0 (reset by hook) | sub_822422E0 hook |
| 6->9 | sub_822438B0 return | (register) | 0 (done) | sub_822438B0 |

#### sub_822438B0 (Outer Save/Load)

| State | Depends On | Address | Required Value | Set By |
|-------|-----------|---------|---------------|--------|
| 0->1 | (external trigger) | 0x82BF9838 | set to 1 | sub_82142230 state 6 entry |
| 1->2 | (immediate) | 0x82BF9844 | 2 (written) | state 1 itself |
| 2->3 | sub_82242910 return | (register) | 0 (done) | sub_82242910 |
| 2->3 | error code | 0x82A9546C | not 33 | sub_82242910 |
| 3->4 | sub_82240F80(0) return | (register) | 0 | sub_82240F80 |
| 3->4 | SAVE magic | 0x82BF394C-F | "SAVE" (0x53415645) | sub_82240F80 |
| 4->5 | (immediate) | N/A | N/A | N/A |
| 5->6 | sub_82240F80(1) return | (register) | 0 | sub_82240F80 |
| 6->done | timer counter | 0x82BF3934 | >= 3000 | timer tick |
| 6->done | timer flag | 0x82BF3938 | 1 | state 2 |
| 6->done | timer mode | 0x82BF393C | 2 | state 2 |

#### sub_82242910 (Scene Creation)

| State | Depends On | Address | Required Value | Set By |
|-------|-----------|---------|---------------|--------|
| 0->1 | sub_8223DAA0 return | (register) | 0 (not ready) | sub_8223DAA0 |
| 0->4 | sub_8223DAA0 return | (register) | 1 (ready) | sub_8223DAA0 -- FAST PATH |
| 1->3 | sub_826CBA70 return | (register) | nonzero, or sub_8223DAA0=1 | various |
| 3->4 | sub_826CBA70 return | (register) | 0 (network idle) | sub_826CBA70 |
| 3->4 | sub_8223DAA0 return | (register) | 1 (ready) | sub_8223DAA0 |
| 4->5 | sub_8223DB20 return | (register) | 0 (no sign-in change) | sub_8223DB20 |
| 4->5 | platformMode | 0x82BF9844 | 3 or 4 | hook (forces 3) |
| 4->5 | done flag | 0x82BF981E | 0 or 1 | various |
| 4->9 | sub_82240B08 return | (register) | 1 (content ready) | sub_82240B08 |
| 5->6 | sub_8223F9F0 out byte | (stack) | 1 (content found) | sub_8223F9F0 stub |
| 6->8 | sub_8284AAE0 return | (register) | nonzero (success) | sub_8284AAE0 |
| 8->9 | sub_8284AB10 return | (register) | nonzero (started) | sub_8284AB10 |
| 8->9 | sub_8284B490 return | (register) | not 1 | sub_8284B490 |
| 8->9 | sub_8284B430 return | (register) | 0 or 5 | sub_8284B430 |
| 9->10 | sub_8284ABA0 return | (register) | nonzero (started) | sub_8284ABA0 |
| 10->11 | sub_8284ABD0 return | (register) | nonzero (done) | sub_8284ABD0 |
| 10->11 | sub_8284B490 return | (register) | not 1 | sub_8284B490 |
| 10->11 | sub_8284B430 return | (register) | 0 or 5 | sub_8284B430 |
| 10->11 | platformMode | 0x82BF9844 | 0,1,3,4 | hook |
| 11->12 | sub_8223D400 return | (register) | nonzero (valid) | sub_8223D400 |
| 11->12 | platformMode | 0x82BF9844 | 3 or 4 | hook |
| 12->14 | sub_822417B0 return | (register) | not 2 | sub_822417B0 |
| 14->done | sub_822417B0 return | (register) | 0 (complete) | sub_822417B0 |

---

## 7. Error Code Quick Reference

| Code | Meaning | Set Where | Fatal? |
|------|---------|-----------|--------|
| 6 | Content not available / enumeration failed | States 2, 7, 13 | No (retry) |
| 7 | Scene creation call failed | State 6 | Yes |
| 8 | Scene creation error during progress | State 8 | Yes |
| 9 | Scene creation unexpected result | State 8 | Yes |
| 10 | Save device not ready | sub_822417B0 | Yes |
| 14 | Resource load initiation failed | State 9 | Yes |
| 15 | Unexpected load status during monitoring | State 10 | Yes |
| 16 | Unexpected load result on completion | State 10 | Yes |
| 17-22 | Content validation pipeline errors | sub_82240C18 | Yes |
| 23-30 | Content installation errors | sub_82240F80 | Yes |
| 29 | "SAVE" magic not found / Live unavailable | sub_822438B0 state 3 | Retry (state 7) |
| 31 | Post-save validation failed | sub_822438B0 state 4 | Retry (state 7) |
| 32 | Content state machine desync | sub_82243260 | Yes |
| 33 | **Sign-in change detected** (special) | sub_8223DB20 callers | Fatal in sub_822438B0 |
| 34 | **Storage device / platform mode error** | sub_82240B78 callers | Fatal |
| 42 | Too many content packages (> 75) | sub_8223D400 | Fatal |
| 44 | Content package parse error | sub_8223D400 | Fatal |
| 46 | XAM subsystem not responding | sub_8223ECA8 | Fatal |
| 50 | Session/multiplayer join error | sub_822440F8 | Fatal |

---

## 8. Summary: What Is Working, What Is Not

### Working (Steps 1-7)

The boot pipeline from CRT entry through front-end states 0-5 is **functional**.
All gate functions in this path are hooked or return acceptable values. The game
successfully reaches the point where sub_822438B0 is called (step 8).

### Partially Working (Steps 8-9)

sub_822438B0 enters and triggers sub_82242910. The scene creation machine runs
but faces multiple fragile patches:

1. The sub_82242910 hook forces platformMode=3 and intercepts the fast path
2. sub_8223F9F0 is stubbed for Xbox guide UI
3. sub_822422E0 hook resets state_phase

### Remaining Unknowns (Steps 9-11)

- Whether sub_8284AAE0 (scene creation) succeeds with current XContent stubs
- Whether sub_82240B08 (content notification) returns the right value
- Whether sub_822438B0 state 6 timer advances correctly
- Whether sub_82240F80 (save operation) succeeds

### Recommended Priority for Fixes

1. **sub_8223DAA0**: Hook to return 0 on first call (prevent fast path)
2. **sub_82240B08**: Hook to return 1 (content ready) unconditionally
3. **sub_8284AAE0 / XContent stubs**: Ensure scene creation returns success
4. **sub_82242910**: Consider full replacement (section 5 of doc 11) if patch-on-patch continues to fail
5. **sub_822438B0 state 6 timer**: Investigate whether timer increments; if not, hook to bypass

---

*Generated 2026-03-27 from synthesis of docs 01-11. All addresses verified via
Python in the original research documents.*
