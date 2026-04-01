# platformMode Dual-Usage Problem: Complete Analysis

## Problem Statement

`platformMode` at `0x82BF9844` serves two different purposes in `sub_82242910`:

1. **State 4**: Gate for `sub_8223F308` (scene creation) -- requires value 3 or 4
2. **State 11**: Routes to state 12 (multiplayer save overwrite) -- values 3 or 4 trigger it

We currently set `platformMode=3` for state 4, but this causes state 11 to enter the
multiplayer save overwrite path (state 12), which fails. The game loops 12->14->13->0.

## Key Addresses

| Address      | Name             | Description                              |
|-------------|------------------|------------------------------------------|
| `0x82BF9844` | platformMode     | Platform/multiplayer mode selector       |
| `0x82BF9848` | stateVar         | sub_82242910 state machine counter       |
| `0x82BF99CC` | sceneVar         | Scene creation result                    |
| `0x82BF99C8` | content_size_ptr | Content size for save operations         |
| `0x82BF981E` | flagByte         | Async completion flag                    |
| `0x82BF981F` | flagByte2        | Secondary flag (checked in platformMode switch) |
| `0x82A9546C` | errorCode        | Error code written on failure            |

## platformMode Values (Xbox 360)

| Value | Meaning                      | Set by         |
|-------|------------------------------|----------------|
| 0     | Single-player (DEFAULT)      | Never written (init=0) |
| 1     | Conditional variant          | sub_822444F0   |
| 2     | System Link multiplayer      | sub_822438B0   |
| 3     | Xbox LIVE multiplayer        | sub_82243260   |
| 4     | Multiplayer variant (party?) | sub_82243C50   |

**For GTA IV single-player on Xbox 360, platformMode is NEVER written. It stays 0.**

Only multiplayer menu functions write to this address.

## State 4 Exact Flow (loc_82242B44, lines 85142-85322)

```
r11 = PPC_LOAD_U32(0x82BF9844)  // read platformMode
r11 = r11 - 3                    // subtract 3
PPC_STORE_U32(0x82BF99CC, 0)     // clear sceneVar
if (r11 unsigned > 1):           // platformMode NOT in {3,4}
    r10 = 0  // skip scene creation
else:                             // platformMode in {3,4}
    r10 = 1  // enable scene creation

if (r10 != 0):
    sub_8223F308(1, saveStruct)   // CREATE SCENE
    store result to 0x82BF99CC

// After scene gate (regardless of r10):
if (flagByte@0x82BF981E != 0):
    goto STATE 5
call sub_82240B08()               // OUR HOOK returns 1
if (nonzero):
    goto STATE 9                  // *** SKIP platformMode switch ***
// platformMode switch only reached if sub_82240B08 returns 0
```

**Key finding**: The platformMode switch (case 0-4 at lines 85218-85278) NEVER runs
when sub_82240B08 returns nonzero. Our hook returns 1, so state 4 always exits to state 9.

## State 10 Analysis (loc_82242F50, lines 85725-85871)

State 10 checks platformMode at `loc_82243010` (after content enumeration completes):

```
platformMode = PPC_LOAD_U32(0x82BF9844)
if platformMode in {0, 1, 3, 4}:
    state = 11                    // advance
else:  // platformMode == 2 or >4
    goto error cleanup (loc_82243178)
```

**Values accepted**: 0, 1, 3, 4 -> state 11. Value 2 -> ERROR.

## State 11 Analysis (loc_82243058, lines 85872-85923)

```
call sub_8223DB20()  // check
call sub_82240B78()  // check
call sub_8223D400()  // save slot finder/matcher
if (sub_8223D400 returns 0):
    goto loc_82242AC4  // COMPLETE (return 2)
if (sub_8223D400 returns 1):  // normal path
    platformMode = PPC_LOAD_U32(0x82BF9844)
    if platformMode in {3, 4}:
        state = 12              // multiplayer save overwrite
    else:
        goto error cleanup      // ERROR!
```

**Critical**: `sub_8223D400` normally returns 1 (even on first launch with no saves,
because it allocates a new save slot). Return 0 only happens on error (field > 75
or duplicate detection, error codes 42/44).

Therefore:
- platformMode=0 at state 11 -> ERROR (not in {3,4})
- platformMode=3 at state 11 -> state 12 (multiplayer save overwrite)

## States 12/14/13 Flow

**State 12** (loc_822430B4): Call `sub_822417B0(r3=0, r4=1, ...)` (begin overwrite)
- Returns 2 -> complete (but with error)
- Returns other -> state = 14, fall through

**State 14** (loc_822430F0): Call `sub_822417B0(r3=0, r4=0, ...)` (continue write)
- Returns 2 -> complete
- Returns 0 -> check `*0x82BF99C8` (content_size); if < 0 -> **state 13**
- Returns other -> stay in state 14

**State 13** (loc_822431DC): Delete save + cleanup -> state 0 (restart)

This creates the observed loop: **12 -> 14 -> 13 -> 0 -> restart**

## Option Analysis

### Option A: Set platformMode=0 After State 4

**VERDICT: DOES NOT WORK**

With platformMode=0 at state 4, the scene creation gate `(val-3) unsigned <= 1` fails:
`(0-3) = 0xFFFFFFFD unsigned > 1` -> `sub_8223F308` is never called. No scene is created.

Even though sub_82240B08 (hooked) returns 1 and advances to state 9, the scene gate
runs BEFORE sub_82240B08. The scene must be created first.

### Option B: Keep platformMode=3, Fix State 12 Content

**VERDICT: POSSIBLE BUT COMPLEX**

If state 12's `sub_822417B0` succeeds, the flow continues:
- State 12 -> state 14 (set state=14, fall through)
- State 14: `sub_822417B0(r4=0)` completes -> check content_size
- If content_size >= 0 and flag byte checks pass:
  - platformMode=3 + flagByte2@0x82BF981F != 0: call `sub_8223F790`, cleanup
  - This is the multiplayer save completion path

After state 14 succeeds, the flow goes to cleanup at `loc_82243188` or `loc_82243178`,
both of which call `sub_8223CAD8` and return 0.

**Problem**: Requires the Xbox XCONTENT write infrastructure (sub_8284AD78 etc.) to work.
These functions interact with the save device container system, which may not be fully
functional in the recomp.

### Option C: Set platformMode=1

**VERDICT: DOES NOT WORK**

`(1-3) unsigned = 0xFFFFFFFE > 1` -> scene creation gate fails. Same problem as Option A.

### Option D: Set platformMode=3 Before State 4, Reset to 0 After State 4

**VERDICT: THIS IS THE CURRENT IMPLEMENTATION (AND IT FAILS)**

The existing hook in `imports.cpp` already does exactly this:
- PRE: if `stateBefore <= 4`: set platformMode=3
- POST: if `stateBefore==4 && stateAfter>=9`: reset platformMode to 0

The problem: platformMode=0 at state 11 causes ERROR because `sub_8223D400` normally
returns 1, and the platformMode check requires {3,4} for the save overwrite path.

### Option E: Hook sub_8223D400 to Return 0

**VERDICT: WORKS FOR STATE MACHINE COMPLETION, BUT RISKS SAVE FUNCTIONALITY**

If `sub_8223D400` returns 0 at state 11, the state machine goes to `loc_82242AC4`
(complete, return 2). The save overwrite is skipped entirely.

**Risk**: `sub_8223D400` performs save slot allocation/matching. Returning 0 means
"error occurred" (codes 42 or 44). This might cause the game to believe saves are
broken, potentially preventing all save operations later.

### Option F: Keep platformMode=3 Through ALL States (No Reset)

**VERDICT: CORRECT APPROACH IF STATE 12/14 CAN SUCCEED**

Flow with platformMode=3 throughout:
1. State 4: scene created (OK)
2. State 9: no platformMode check (OK)
3. State 10: 3 in {0,1,3,4} -> state 11 (OK)
4. State 11: sub_8223D400=1, platformMode=3 -> state 12 (OK)
5. State 12: sub_822417B0 must succeed
6. State 14: sub_822417B0 must succeed

Requires fixing the content write path (sub_822417B0 / sub_8284AD78 / sub_8284ADA0).
This is the multiplayer save write infrastructure but it's the same code used for
single-player saves when platformMode >= 3.

### Option G: Hook sub_8223F308 Directly, Keep platformMode=0

**VERDICT: DOES NOT WORK**

Even if we call sub_8223F308 manually, state 11 still checks platformMode.
With platformMode=0, sub_8223D400 returns 1, and the platformMode check fails.

### Option H: Keep platformMode=3 at State 4 + Hook sub_8223D400 to Return 0

**VERDICT: BEST PRAGMATIC OPTION**

Combines:
- platformMode=3 at state 4 for scene creation (existing)
- Reset to 0 after state 4 exits to state 9 (existing)
- Hook sub_8223D400 to return 0 at state 11 (NEW)

With sub_8223D400 returning 0, state 11 completes directly at loc_82242AC4.
The save overwrite path (states 12-14) is never entered.

**Risk assessment**: sub_8223D400's return-0 path stores error code 42 or 44 to
`0x82A9546C`. The game uses this error code for UI display. Error 42 = "too many saves"
which might trigger a user-facing dialog. However, this may be acceptable for initial
game boot where no save writing is needed yet.

### Option I: Keep platformMode=3 + Remove Reset + Fix sub_822417B0

**VERDICT: MOST CORRECT LONG-TERM SOLUTION**

Remove the `platformMode=3->0` reset after state 4. Keep platformMode=3 throughout.
Ensure sub_822417B0 can complete the save write operation:
- Fix sub_8284AD78 (begin content write) - needs working XCONTENT infrastructure
- Fix sub_8284ADA0 (complete content write)
- Ensure content_size at `*0x82BF99C8` is >= 0 after write

This follows the actual multiplayer save path and produces a valid save file.
Most complex but most correct for full save support.

## Recommendation

**Short-term**: Option H -- hook sub_8223D400 to return 0. This unblocks the state
machine immediately. Save writing can be handled separately through other code paths.

**Long-term**: Option I -- fix the content write infrastructure so the full state 12/14
path works. This provides proper save file support.

## Source File References

- State machine: `gta4_recomp.6.cpp` lines 84816-86160 (sub_82242910)
- sub_822417B0: `gta4_recomp.6.cpp` lines 82160-82373
- sub_8223D400: `gta4_recomp.6.cpp` lines 72404-72746
- Current hook: `LibertyRecomp/kernel/imports.cpp` lines 1872-1932
- platformMode writes: sub_82243260 (=3), sub_822438B0 (=2), sub_82243C50 (=4), sub_822444F0 (=0|1)
