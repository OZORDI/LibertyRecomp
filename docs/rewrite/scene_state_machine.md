# Scene State Machine Analysis: Why 0x831C2458 is NULL

## Overview

The scene pointer at `0x831C2458` is NULL because the scene creation state machine
(`sub_82242910`) never completes its full 15-state sequence. Two nested state machines
must complete in order: the outer front-end SM (`sub_82142230`, states 0-6) and the
inner scene creation SM (`sub_82242910`, states 0-14). Scene objects are created but
never registered into the device scene list array.

---

## Outer State Machine: sub_82142230 (states 0-6)

**File**: `gta4_recomp.0.cpp` line 5305
**State variable**: r29 (register, not memory -- persists across loop iterations)
**Loop**: Each iteration calls `sub_82849918` (yield) + `sub_821428C8` (frame tick), then dispatches on r29.

| State | Function | Gate condition | Next state |
|-|-|-|-|
| 0 | sub_822414E8 | Returns 1 -> state 1; returns 2 -> state 3 | 1 or 3 |
| 1 | sub_8223DDA8 | Returns 1 or 2 -> state 2 | 2 |
| 2 | sub_8223DEE8 | Returns 1 -> loc_821422FC (content check); returns 2 -> state 8 (exit) | 3 or 4 or 8 |
| 3 | (complex) | sub_821406C8 returns player slot; checks slot[68/148], slot[72/152], slot[4/84], slot[56/136] "newly set" transitions | 4 |
| 4 | sub_822440F8 | Returns 2 -> state 5; returns 1 -> state 7 (error) | 5 or 7 |
| 5 | sub_822422E0 | Always advances to state 6 (unconditional `li r29,6`) | 6 |
| 6 | sub_822438B0 | Returns 0 -> state 9 (done); returns 2 -> state 7 (error) | 9 or 7 |
| >6 | sub_8223E028 | Exit -- activates post-init systems | (done) |

### State 3 Gate (the "newly set" detector)

State 3 is the most complex gate. It reads a player slot struct returned by
`sub_821406C8`. For each pair of fields (current, shadow), it fires when
`current != 0 AND shadow == 0` (first-time detection). The pairs checked:

| Field pair | Meaning | Fires when |
|-|-|-|
| slot[68], slot[148] | Sign-in completion | First sign-in detected |
| slot[72], slot[152] | Storage device selected | First storage device detected |
| slot[4], slot[84] | Profile data loaded | First profile load detected |
| slot[56], slot[136] | Content ready (DLC/update) | First content detection |

When content ready fires (slot[56] check), it calls `sub_822BCA90` (network tick)
and `sub_82215530` (content transition). When profile fires, it calls `sub_8221B198`.
After all checks pass, it sets r29=4.

**Hook fix (imports.cpp line 1999)**: On first call, writes 1 to slot[56/68/72/4],
causing all "newly set" detectors to fire immediately.

### State 4 Bypass

`sub_822440F8` handles Xbox 360 save device selection (controller scan, storage
enumeration). None of this works in recomp.

**Hook fix (imports.cpp line 2065)**: Returns 2 (success) directly, bypassing
the entire save device flow. Sets playerIdx to 0 if uninitialized.

### State 5 to 6 Transition

State 5 calls `sub_822422E0` (level selection / scene load dispatch) then
unconditionally sets r29=6. State 5 writes 2 to `0x82BF9834` ("done").

**Hook fix (imports.cpp line 2122)**: Resets `0x82BF9834` from 2->0 after
state 5 completes, preventing sub_82242910 state 4 from seeing stale value 2
and triggering error 34.

---

## Inner Scene Creation State Machine: sub_82242910 (states 0-14)

**File**: `gta4_recomp.6.cpp` line 84816
**State variable**: `0x82BF9848` (lis r26=0x82C00000, offset -26552 = -0x67B8)
**platformMode**: `0x82BF9844` (offset -26556)
**Called from**: sub_822438B0 (outer state 6's inner SM) at its state 2

### State Transition Map

| State | Key call(s) | Success transition | Error transition |
|-|-|-|-|
| 0 | sub_8223DAA0 (XAM readiness) | 0->1 (normal), 0->4 (fast, intercepted) | -- |
| 1 | sub_826CBA70, sub_8223DAA0, sub_8223F9F0 | 1->3 | 1->2 (if notification) |
| 2 | sub_8223CB60, sub_826CBA70, sub_8223DAA0 | 2->return(2) (writes error 6 to 0x82A9546C) | 2->3 (if notification) |
| 3 | sub_826CBA70, sub_8223DAA0, sub_82240AB0 | 3->4 | 3->2 (fallback) |
| 4 | sub_8223DB20, sub_82240B08, platformMode switch | 4->5 (if mode 3 or 4), 4->9 (if sub_82240B08 ready) | 4->error 33/34 |
| 5 | sub_8223DB20, sub_8223F9F0 (XAM notification) | 5->6 or 5->7 | 5->error 33 |
| 6 | sub_8223DB20, sub_826CBA70, sub_8284AAE0 (scene load) | 6->8 | 6->error 7 |
| 7 | sub_8223DB20, sub_8223CB60, sub_8223F9F0 | 7->return(2) (writes error 6) | 7->6 (retry) |
| 8 | sub_8284AB10, sub_8284AB70, sub_8284B490, sub_8284B430 | 8->9 (if sub_82240B08 ready) | 8->7 (error), 8->error 8/9 |
| 9 | sub_8223DB20, sub_82240B78, sub_8223D2F0, sub_8284ABA0 | 9->10 | 9->error 14/34 |
| 10 | sub_8284ABD0, sub_8284ABF8, sub_8284B490, sub_8284B430, sub_82240B08 | 10->11 (if platformMode in {0,1,3,4}) | 10->error 15/16 |
| 11 | sub_8223DB20, sub_82240B78, sub_8223D400 | 11->12 (if platformMode 3 or 4) | 11->return(2), 11->error |
| 12 | sub_822417B0 (phase 1, r4=1) | 12->14 | 12->return(2) |
| 13 | (error recovery) | 13->0 (reset) | -- |
| 14 | sub_822417B0 (phase 2, r4=0), completion checks | 14->return(0) (SUCCESS) | 14->13 (if delta<0), 14->error |

### Critical Gates Identified

**Gate 1: State 0 fast path (FIXED)**
sub_8223DAA0 returns 1 in recomp (devices immediately ready), causing 0->4 jump.
State 4 reads stale platformMode and fails.
**Fix**: Hook intercepts 0->4 transition, forces to 0->1 (imports.cpp line 2374).

**Gate 2: State 4 platformMode check (FIXED)**
platformMode at `0x82BF9844` must be 3 or 4. Computed as `(val-3) unsigned <= 1`.
**Fix**: Hook forces platformMode=3 before state 4 runs (imports.cpp line 2361).

**Gate 3: State 4 sub_82240B08 (FIXED)**
Content device readiness check. Returns 0 in recomp (no async content op completed).
**Fix**: Hook sets readiness flags and returns 1 (imports.cpp line 2175).

**Gate 4: States 4+, sub_8223DB20 sign-in guard (FIXED)**
Polls for XN_SYS_SIGNINCHANGED notifications. RexGlue broadcasts 0x0A at startup,
causing spurious error 33.
**Fix**: Stubbed to return 0 (imports.cpp line 2155).

**Gate 5: States 9+, sub_82240B78 storage guard (FIXED)**
Polls for XN_SYS_STORAGEDEVICESCHANGED notifications. Already returns 0 in practice.
**Fix**: Explicitly stubbed (imports.cpp line 2164).

**Gate 6: State 12 and 14, sub_822417B0 content size (FIXED)**
Two-phase content size checker. Phase 2 writes size delta to `0x82BF99C8`.
If delta < 0, state 14 loops to state 13 (error restart).
**Fix**: Clamped delta to >= 0 (imports.cpp line 2241).

**Gate 7: State 14 completion check (CURRENT BLOCKER)**
After sub_822417B0 phase 2, the code checks `*(r29)` (scene creation result pointer).
If `*(r29) < 0`, transitions to state 13 (restart). The code then checks:
1. `0x82A95497` (byte) -- if non-zero AND platformMode==3 AND `0x82BF9A2F` (byte) != 0: calls sub_8223F790
2. If all checks pass: returns 0 (SUCCESS, scene creation complete)

On success return, sub_82242910 writes 0 to `0x82BF9848` (reset state) and clears
`0x82A95496` and `0x82BF3CDA`.

### Scene Registration Gap

Even if sub_82242910 completes all 14 states successfully:
- Scene objects are created via sub_827ADB48 and stored at `0x82FF5368`
- But they are never written to the device scene list array at `0x831C2458`
- That registration happens during D3D device initialization, which is fully stubbed
- Sub_82242910 creates the game world (streaming, resources, save) but does NOT
  perform the GPU-level scene list registration

---

## Current Status and Blocking Points

### What Works (hooks in imports.cpp)

| Hook | Purpose | Status |
|-|-|-|
| sub_822414E8 | State 0 sign-in | Diagnostic tracing |
| sub_8223DDA8 | State 1 storage | Diagnostic tracing |
| sub_8223DEE8 | State 2 save check | Diagnostic tracing |
| sub_821406C8 | State 3 player slot | Populates slot fields |
| sub_822440F8 | State 4 (outer) save device | Bypass, returns 2 |
| sub_822422E0 | State 5 level dispatch | Resets 0x82BF9834 |
| sub_822438B0 | State 6 inner SM | Diagnostic tracing |
| sub_8223DB20 | Sign-in guard | Stubbed (return 0) |
| sub_82240B78 | Storage guard | Stubbed (return 0) |
| sub_82240B08 | Content readiness | Force ready (return 1) |
| sub_8224FFC8 | XAM dialog result | Auto-accept |
| sub_822417B0 | Content size check | Delta clamped >= 0 |
| sub_82242910 | Scene creation SM | Fast-path intercept + platformMode fix |

### What Still Blocks

1. **sub_82242910 state 6**: Calls `sub_8284AAE0` (scene load via streaming system).
   This triggers the resource loading pipeline (sub_82852DD0 chain) which may hang
   on `sub_8286C238` (pgStreamable tree visitor) or `sub_82955BE0` (XAudio hang).

2. **sub_82242910 states 8/10**: Call `sub_8284B490` (resource activation polling)
   and `sub_8284B430` (status check). These poll streaming completion, which depends
   on the forced-synchronous I/O pipeline completing without hanging.

3. **Scene registration gap**: Even if sub_82242910 completes, the scene object at
   `0x82FF5368` must be manually copied to `0x831C2458` for rendering to work.
   The registration function (part of D3D device init) is fully stubbed.

### Required Fixes for Scene Pointer

**Minimum fix** (bridge the gap):
```cpp
// After sub_82242910 returns 0 (success), copy scene ptr to device array:
uint32_t sceneObj = PPC_LOAD_U32(0x82FF5368);
if (sceneObj != 0) {
    PPC_STORE_U32(0x831C2458, sceneObj);
}
```

**But first**: The streaming/resource loading pipeline (states 6-10) must complete
without hanging. The known hang points are:
- `sub_82955BE0` (XAudio streaming init, already identified as needing a stub)
- `sub_8286C238` (pgStreamable tree visitor, recommended to skip via sub_82852D18 hook)

---

## Address Reference

| Address | Name | Role |
|-|-|-|
| 0x82BF9848 | sceneState | sub_82242910 state counter (0-14) |
| 0x82BF9844 | platformMode | Must be 3 for base game scene creation |
| 0x82BF9838 | innerState6 | sub_822438B0 state counter |
| 0x82BF9834 | legacyState | sub_822422E0 writes 2; must be reset to 0 |
| 0x82BF3A88 | sceneObj | Scene object pointer (created by sub_82242910) |
| 0x82BF3A77 | sceneReady | Scene readiness flag |
| 0x82BF3CDA | contentReady | Content readiness flag |
| 0x82BF9A2F | episodeFlag | Episode/DLC context flag |
| 0x82BF99C8 | sizeDelta | Content size comparison result |
| 0x82A9546C | errorCode | Error code from state machine |
| 0x82A95478 | playerIdx | Active player/episode index |
| 0x82A95497 | byte flag | Checked at state 14 completion |
| 0x82FF5368 | g_scene | Created grcSceneList pointer |
| 0x831C2458 | deviceSceneList | Render dispatch reads this -- currently NULL |
| 0x82B053E4 | initComplete | Gates conditional subsystems after scene creation |
