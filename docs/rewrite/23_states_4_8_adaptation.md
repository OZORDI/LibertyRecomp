# States 4-8 Platform Adaptation Analysis

**Source**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.6.cpp` lines 85118-85662
**Verified against**: Generated recomp code ONLY (not pseudocode)

All arithmetic verified with Python.

---

## Address Reference (Python-verified)

| Symbol | Address | Derivation |
|--------|---------|------------|
| STATE_VAR | `0x82BF9848` | r26(0x82C00000) + (-26552) |
| SCENE_MODE | `0x82BF9844` | r26 + (-26556) |
| RESTART_FLAG | `0x82BF981E` | r26 + (-26594) |
| FLAG_26593 | `0x82BF981F` | r26 + (-26593) |
| PENDING_VAL | `0x82BF99CC` | r26 + (-26164) |
| SCENE_NAME | `0x83192C50` | lis(-31975)+11344 |
| STRUCT_14968 | `0x82BF3A78` | lis(-32065)+14968 |
| SCENE_INFO_PTR | `0x82BF3D90` | lis(-32065)+15760 |
| BYTE_14966 | `0x82BF3A76` | lis(-32065)+14966 |
| ERROR_CODE | `0x82A9546C` | lis(-32087)+21612 |
| ONLINE_FLAG | `0x82A95466` | lis(-32087)+21606 |

---

## State 4 -- Scene Load Setup (CRITICAL)

**Label**: `loc_82242B1C` (line 85118)
**Lines**: 85118-85322

### Exact Flow from Generated Code

```
1. sub_8223DB20()                              // XNotify device-change check (type 10)
   -> nonzero: ERROR_CODE=33, return 2         // abort: sign-in changed

2. PENDING_VAL = 0                             // zero out player count

3. r11 = SCENE_MODE; r11 = r11 - 3
   if (r11 <= 1):                              // i.e., SCENE_MODE == 3 or 4
       r10 = 1
   else:
       r10 = 0                                 // modes 0,1,2

4. if (r10 & 0xFF != 0):                       // modes 3 or 4 only
       sub_8223F308(r3=1, r4=STRUCT_14968)     // load scene file WITH save validation
       PENDING_VAL = [STRUCT_14968 + 4]        // copy content ID from scene struct

5. if RESTART_FLAG != 0:                       // check 0x82BF981E
       STATE_VAR = 5, return 1                 // skip device check, go to content enum

6. sub_82240B08()                              // validate save device handle
   -> nonzero: STATE_VAR = 9, return 1         // handle VALID -> skip to state 9
   -> zero: fall through to inner switch       // handle INVALID -> check platformMode

7. Inner switch on SCENE_MODE (0-4):
   case 0, 1: r11 = 0 (no error)
   case 2:    r11 = 1 (error flag)
   case 3:    if FLAG_26593 != 0:
                  if ONLINE_FLAG == 0: RESTART_FLAG = 1
                  r11 = 0
              else:
                  r11 = 1 (error)
   case 4:    if ONLINE_FLAG == 0:
                  RESTART_FLAG = 1
                  r11 = 0
              else:
                  r11 = 1 (error)
   default:   r11 = 1 (error)

8. if (r11 & 0xFF != 0):
       ERROR_CODE = 34, return 2               // invalid mode for current state

9. STATE_VAR = 5, return 1                     // proceed to content enumeration
```

### Function Classification

| Function | Classification | Reason |
|----------|---------------|--------|
| `sub_8223DB20` | XBOX NOTIFICATION | Polls XNotifyGetNext(type=10) for device-change events |
| `sub_8223F308` | PURE GAME LOGIC | Reads scene binary stream, parses 32 argument slots into SceneStruct |
| `sub_82240B08` | XBOX SAVE DEVICE | Calls XamContentGetDeviceData via sub_8284B3D8 to validate device handle |

### Memory Writes (Downstream Dependencies)

| Address | What | Used By |
|---------|------|---------|
| `0x82BF99CC` (PENDING_VAL) | Content ID from scene file | States 6, 12, 14 (passed as r6 to sub_8284AAE0, sub_822417B0) |
| `0x82BF9848` (STATE_VAR) | Next state (5 or 9) | State machine dispatch |
| `0x82BF981E` (RESTART_FLAG) | Set to 1 in mode 3/4 when ONLINE_FLAG==0 | State 5 reads it to skip content enumeration |

### Critical Path for PC Recomp

**sub_8223DB20** polls `XNotifyGetNext(handle, type=10, ...)`. On Xbox 360 this checks
for storage device insertion/removal notifications. On PC there are no removable storage
devices. This function should return 0 (no notification) always.

**sub_8223F308** is pure game logic -- reads scene data from a binary stream and fills
the SceneStruct at `0x82BF3A78`. Must be preserved as-is. The scene data drives
everything downstream (content ID, argument slots, etc.).

**sub_82240B08** validates the save device handle at `array[slot*160 + 56]`. On Xbox,
the content creation async flow (state 6) populates this handle. On PC:
- If the handle was never populated (no XContent container system), this always returns 0
- Returning 0 causes state 4 to fall through to the platformMode switch
- For modes 0 and 1 (single-player): r11=0, no error, STATE_VAR=5 (correct path)
- For mode 2: r11=1, ERROR_CODE=34, return 2 (error -- but mode 2 is "load from content")
- For modes 3/4 (multiplayer): depends on FLAG_26593 and ONLINE_FLAG

**Proposed hook**: Hook `sub_82240B08` to always return 1 (device valid), set
`0x82BF3A77=1` and `0x82BF3CDA=1`. This sends state 4 directly to state 9, skipping
the content enumeration/creation flow (states 5-8). This is correct because on PC there
is no Xbox content container to create.

**Alternative**: Let `sub_82240B08` return 0 naturally. For single-player (modes 0,1),
the platformMode switch produces r11=0 and transitions to state 5 anyway. The risk is
states 5-8 then try Xbox Guide UI and XContent operations that need their own stubs.

---

## State 5 -- Content Enumeration / Load Decision

**Label**: `loc_82242C88` (line 85323)
**Lines**: 85323-85401

### Exact Flow from Generated Code

```
1. sub_8223DB20()                              // XNotify device-change check
   -> nonzero: ERROR_CODE=33, return 2

2. r31 = 0                                     // default: not ready
   if RESTART_FLAG != 0:                       // 0x82BF981E
       local_80 = 1                            // write 1 to stack byte
       RESTART_FLAG = 0                        // clear flag
       goto step 4

3. sub_8223F9F0(r3=2, r4=0, r5=&local_80)     // show storage device selector UI (mode 2)
   -> returns 0: goto loc_82243250 (wait)      // UI still showing
   -> returns nonzero: r31 = 1

4. if r31 == 0: goto loc_82243250 (wait, return 1)

5. BYTE_14966 (0x82BF3A76) = 0                // clear g_saveInitDone

6. Compute next state:
   cntlzw(local_80), rlwinm(,27,31,31), +6
   -> local_80 != 0: next = 6 (content found)
   -> local_80 == 0: next = 7 (no content)
   STATE_VAR = next, return 1
```

### Function Classification

| Function | Classification | Reason |
|----------|---------------|--------|
| `sub_8223DB20` | XBOX NOTIFICATION | Same XNotify poll as state 4 |
| `sub_8223F9F0` | XBOX UI/DIALOG | Giant 54-case switch showing Xbox Guide UI dialogs. Case 2: checks online status, shows storage device selector |

### Memory Writes (Downstream Dependencies)

| Address | What | Used By |
|---------|------|---------|
| `0x82BF981E` (RESTART_FLAG) | Cleared to 0 | State 4 and 5 check it |
| `0x82BF3A76` (BYTE_14966) | Cleared to 0 (g_saveInitDone) | sub_82240B78 checks it before re-validating device |
| `0x82BF9848` (STATE_VAR) | 6 or 7 | State machine dispatch |

### Platform Adaptation

**sub_8223F9F0(mode=2)** shows the Xbox Guide storage device selector UI. On PC,
this should immediately return 1 with `*r5 = 1` (user selected storage). The existing
RexGlue `XamShowDeviceSelectorUI` returns `X_ERROR_SUCCESS` synchronously but
sub_8223F9F0 is a higher-level wrapper with additional game logic.

**Proposed hook for sub_8223F9F0**: Write 1 to `[r5]`, return `r3 = 1`. This bypasses
all Guide UI dialogs and pretends the user always accepted the default storage device.
This hook works for ALL call sites (states 1, 2, 5, 7, 13) since the function is a
universal "show dialog and get result" dispatcher.

**RESTART_FLAG shortcut**: When RESTART_FLAG is already set (from state 4 modes 3/4),
state 5 skips sub_8223F9F0 entirely and goes directly to state 6. This path is pure
game logic and needs no adaptation.

---

## State 6 -- Scene Creation with Content (Network Session)

**Label**: `loc_82242D18` (line 85402)
**Lines**: 85402-85474

### Exact Flow from Generated Code

```
1. sub_8223DB20()                              // XNotify device-change check
   -> nonzero: ERROR_CODE=33, return 2

2. sub_826CBA70()                              // check streaming/loading complete
   -> nonzero: goto loc_82243250 (wait)        // still loading, return 1

3. Load parameters:
   r3 = SCENE_NAME (0x83192C50)
   r4 = [SCENE_INFO_PTR] (load from 0x82BF3D90)
   r5 = 1
   r6 = PENDING_VAL (from 0x82BF99CC, set by state 4)
   r7 = 1
   r8 = ONLINE_FLAG (from 0x82A95466)

4. sub_8284AAE0(r3, r4, r5, r6, r7, r8)       // begin XContent container create
   -> returns 0: ERROR_CODE=7, return 2        // create failed
   -> returns nonzero: STATE_VAR=8, return 1   // async create started
```

### Function Classification

| Function | Classification | Reason |
|----------|---------------|--------|
| `sub_8223DB20` | XBOX NOTIFICATION | XNotify poll |
| `sub_826CBA70` | GAME LOGIC (network) | Checks if streaming/network session is busy. Returns bool. |
| `sub_8284AAE0` | XBOX ASYNC PATTERN | Wrapper that computes async-op struct slot, calls sub_8284A0F8 which invokes XamShowDeviceSelectorUI to start XContent container creation |

### Memory Writes (Downstream Dependencies)

| Address | What | Used By |
|---------|------|---------|
| `0x82BF9848` (STATE_VAR) | 8 | State 8 monitors the async create |
| async-op struct state | Set to 2 by sub_8284A0F8 | State 8 polls via sub_8284AB10 |

### Platform Adaptation

**sub_826CBA70** is a network/streaming readiness check. It is game logic but may
involve Xbox LIVE session management internally. If it always returns 0 (not busy),
state 6 proceeds immediately.

**sub_8284AAE0** is the critical Xbox function. It is a wrapper around `sub_8284A0F8`
which calls `XamShowDeviceSelectorUI` (via `sub_82A116E8`). The current RexGlue impl
returns `X_ERROR_SUCCESS` instead of `X_ERROR_IO_PENDING`, causing sub_8284A0F8 to
return 0 (failure), which makes state 6 set ERROR_CODE=7.

**Proposed hook for sub_8284AAE0**: Have it set the async-op struct state to 2
(IO_PENDING_CREATE) and return 1. This mimics a successful async create initiation.
State 8 will then poll for completion.

**Or hook the inner sub_8284A0F8** (as documented in doc 16): set state=2, return 1.

---

## State 7 -- Scene Creation without Content (Retry Path)

**Label**: `loc_82242D9C` (line 85475)
**Lines**: 85475-85534

### Exact Flow from Generated Code

```
1. sub_8223DB20()                              // XNotify device-change check
   -> nonzero: ERROR_CODE=33, return 2

2. sub_8223CB60()                              // check if platformMode requires storage
   -> result saved as r31 (bool, u8 masked)

3. local_80 = 0                                // init stack byte

4. if r31 != 0:                                // storage needed
       sub_8223F9F0(r3=3, r4=0, r5=&local_80)  // show content check UI (mode 3)
       -> returns 0: goto loc_82243250 (wait)
       // returns nonzero: fall through

5. if local_80 == 0:
       ERROR_CODE=6, return 2                  // content not available

6. if r31 == 0:
       ERROR_CODE=6, return 2                  // storage not needed but no content??

7. STATE_VAR = 6, return 1                     // both conditions met, proceed to create
```

### Function Classification

| Function | Classification | Reason |
|----------|---------------|--------|
| `sub_8223DB20` | XBOX NOTIFICATION | XNotify poll |
| `sub_8223CB60` | PURE GAME LOGIC | Reads SCENE_MODE and returns 0 for modes 0,2, returns 1 for modes 1,3,4 |
| `sub_8223F9F0` | XBOX UI/DIALOG | Mode 3: content check/confirmation dialog |

### Memory Writes (Downstream Dependencies)

| Address | What | Used By |
|---------|------|---------|
| `0x82BF9848` (STATE_VAR) | 6 | Loops back to state 6 for creation |

### Platform Adaptation

State 7 is a **retry path** entered when state 5 found no content. It re-checks
whether content is available via `sub_8223F9F0(mode=3)`.

**sub_8223CB60** is pure game logic -- reads platformMode and returns whether storage
is needed. Must be preserved.

**sub_8223F9F0(mode=3)** shows a content check/confirmation dialog. Same hook as
state 5: return 1 with `*r5 = 1`.

If `sub_8223F9F0` is hooked to always succeed AND `sub_8223CB60` returns 1 (modes
1,3,4), state 7 transitions to state 6. This is the correct behavior -- on PC we
always have storage available.

If `sub_8223CB60` returns 0 (modes 0,2), the code skips the `sub_8223F9F0` call
entirely, local_80 stays 0, and we hit ERROR_CODE=6. This is a logic error only
reachable if state 5 set next_state=7 (no content) while in mode 0 or 2, which
would be a genuine content-not-found error even on Xbox.

---

## State 8 -- Monitor Scene Creation Progress

**Label**: `loc_82242E08` (line 85535)
**Lines**: 85535-85662

### Exact Flow from Generated Code

```
1. r31 = SCENE_NAME (0x83192C50)
   r30 = 0x82BF0000
   r4 = [SCENE_INFO_PTR] (from r30+15760 = 0x82BF3D90)

2. sub_8284AB10(r3=SCENE_NAME, r4=[SCENE_INFO_PTR])   // check if async op ready
   -> returns 0: goto loc_82243250 (wait, return 1)

3. sub_8284AB70(r3=SCENE_NAME, r4=[SCENE_INFO_PTR])   // advance/finalize async op
   (resets async-op struct state from 3 -> 0 if done)

4. sub_8223DB20()                              // XNotify device-change check
   -> nonzero: ERROR_CODE=33, return 2

5. sub_8284B490(r3=SCENE_NAME, r4=[SCENE_INFO_PTR])   // get operation state
   r4 = [SCENE_INFO_PTR] (reload)
   r3 = SCENE_NAME (reload)

   if result == 1:                             // state == 1 (error state)
       sub_8284B430(SCENE_NAME, [SCENE_INFO_PTR])  // get error code
       if result == 2: STATE_VAR=7, return 1   // retry via state 7
       else: ERROR_CODE=8, return 2            // unrecoverable creation error

   if result != 1:                             // state != 1
       sub_8284B430(SCENE_NAME, [SCENE_INFO_PTR])  // get result code
       if result == 0: goto step 6             // success!
       else: ERROR_CODE=9, return 2            // creation failed

6. sub_82240B08()                              // validate save device handle
   Computed via subfic/subfe/andi:
   -> returns nonzero (valid): STATE_VAR = 9   // device valid, proceed to load
   -> returns 0 (invalid): STATE_VAR = 0       // restart from beginning
   return 1
```

### Function Classification

| Function | Classification | Reason |
|----------|---------------|--------|
| `sub_8284AB10` | XBOX ASYNC PATTERN | Wrapper for sub_82849C18: polls XOverlappedGetResult on the async-op struct |
| `sub_8284AB70` | XBOX ASYNC PATTERN | Wrapper: if async-op state==3, resets to 0 (clear completed marker) |
| `sub_8223DB20` | XBOX NOTIFICATION | XNotify poll |
| `sub_8284B490` | GAME LOGIC (struct read) | Reads async-op struct state field at [base + slot*160 + 0] |
| `sub_8284B430` | GAME LOGIC (struct read) | Reads async-op struct result at [base + slot*160 + 32] |
| `sub_82240B08` | XBOX SAVE DEVICE | Validates device handle via XamContentGetDeviceData |

### Memory Writes (Downstream Dependencies)

| Address | What | Used By |
|---------|------|---------|
| `0x82BF9848` (STATE_VAR) | 0, 7, or 9 | State machine dispatch |
| async-op struct state | Reset from 3->0 by sub_8284AB70 | Future async operations |
| `0x82BF3A77`, `0x82BF3CDA` | Set by sub_82240B08 if valid | sub_82240B78, state 4, state 13 |

### State 8 End: subfic/subfe/andi Computation (Python-verified)

The end of state 8 computes the next state from sub_82240B08's return value:
- **returns 0** (device invalid): `subfic(0,0) = 0, CA=1`; `subfe = ~0+0+1 = 0`; `andi 9 = 0` => **STATE_VAR = 0** (restart)
- **returns nonzero** (device valid): `subfic = -val, CA=0`; `subfe = 0xFFFFFFFF`; `andi 9 = 9` => **STATE_VAR = 9** (proceed to load)

### Platform Adaptation

State 8 is the async completion monitor for the XContent create operation started
in state 6. It polls `sub_8284AB10` (wraps `sub_82849C18` which calls
`XOverlappedGetResult`), then reads the operation state and result from the
async-op struct.

**Four-function hook set** (from doc 16):

1. **sub_8284A0F8** (inner of sub_8284AAE0, called by state 6): Set struct state=2, return 1
2. **sub_82849C18** (inner of sub_8284AB10, called by state 8): If state==2, set state=3, return 1
3. **sub_8284A1E8** (inner of sub_8284ABA0, called by state 9): Set state=4, return 1
4. **sub_82849C98** (inner of sub_8284ABD0, called by state 10): If state==4, set state=5, write 0 to result, return 1

With hooks 1 and 2 in place:
- State 6 calls sub_8284AAE0 -> sub_8284A0F8 sets state=2, returns 1 -> STATE_VAR=8
- State 8 calls sub_8284AB10 -> sub_82849C18 sees state==2, sets state=3, returns 1
- State 8 calls sub_8284AB70 -> resets state from 3 to 0
- State 8 calls sub_8284B490 -> reads state (now 0) -> result != 1
- State 8 calls sub_8284B430 -> reads result at offset +32 (should be 0 after zeroing)
- If result == 0: calls sub_82240B08
- If sub_82240B08 hooked to return 1: STATE_VAR = 9

**The async-op struct result field at offset +32 MUST be 0** for state 8 to succeed.
The hooks for sub_8284A0F8 should zero the OVERLAPPED and extended_error fields, or
sub_82849C18 should explicitly write 0 to offset +32 on completion.

---

## Summary: All Functions Across States 4-8

### PURE GAME LOGIC (keep as recompiled code)

| Function | Where Called | Purpose |
|----------|-------------|---------|
| `sub_8223F308` | State 4 | Scene file parser -- reads binary stream, fills SceneStruct |
| `sub_8223CB60` | State 7 | PlatformMode check (returns 0 for modes 0,2; 1 for modes 1,3,4) |
| `sub_826CBA70` | State 6 | Network/streaming busy check |
| `sub_8284B490` | State 8 | Read async-op struct state field |
| `sub_8284B430` | State 8 | Read async-op struct result/error field |
| PlatformMode switch | State 4 | Mode validation with FLAG_26593 and ONLINE_FLAG checks |

### XBOX NOTIFICATION (needs hook -- return 0 always on PC)

| Function | Where Called | Xbox API | Proposed Hook |
|----------|-------------|----------|---------------|
| `sub_8223DB20` | States 4,5,6,7,8 | XNotifyGetNext(type=10) | Return 0 (no notification) |

### XBOX SAVE DEVICE (needs hook)

| Function | Where Called | Xbox API | Proposed Hook |
|----------|-------------|----------|---------------|
| `sub_82240B08` | States 4, 8 | XamContentGetDeviceData | Return 1, set g_sceneReady=1, g_contentReady=1 |

### XBOX UI/DIALOG (needs hook)

| Function | Where Called | Xbox API | Proposed Hook |
|----------|-------------|----------|---------------|
| `sub_8223F9F0` | States 5, 7 | XamShowDeviceSelectorUI (Guide UI) | Write 1 to [r5], return r3=1 |

### XBOX ASYNC PATTERN (needs hook set)

| Function | Where Called | Xbox API | Proposed Hook |
|----------|-------------|----------|---------------|
| `sub_8284A0F8` | State 6 (via sub_8284AAE0) | XamShowDeviceSelectorUI (async create) | Set struct state=2, zero offset+32, return 1 |
| `sub_82849C18` | State 8 (via sub_8284AB10) | XOverlappedGetResult (create poll) | If state==2: state=3, return 1 |
| `sub_8284AB70` | State 8 | State cleanup (if state==3: state=0) | Keep as-is (reads/writes struct only) |

---

## Recommended Hook Priority

### Minimum viable set (skip states 5-8 entirely)

Hook `sub_82240B08` to return 1. This makes state 4 jump directly to state 9,
bypassing all content enumeration (state 5), creation (state 6), retry (state 7),
and monitoring (state 8). This is safe for single-player because:

- State 4 step 6: sub_82240B08 returns 1 -> STATE_VAR=9
- States 5-8 never execute
- State 9 starts scene loading directly

**Risk**: States 5-8 set `g_saveInitDone` (0x82BF3A76) to 0 and may initialize
other save-related globals. If downstream code depends on these side effects, they
must be replicated in the sub_82240B08 hook.

### Full hook set (states 5-8 execute normally with Xbox stubs)

If any game logic in states 5-8 is needed for correct behavior:

1. `sub_8223DB20` -> return 0 (5 call sites in states 4-8)
2. `sub_82240B08` -> return 1, set flags (2 call sites: states 4, 8)
3. `sub_8223F9F0` -> write 1 to [r5], return 1 (2 call sites: states 5, 7)
4. `sub_8284A0F8` -> set state=2, zero result, return 1 (state 6)
5. `sub_82849C18` -> if state==2: state=3, return 1 (state 8)

Total: 5 hooks to make states 4-8 fully functional with Xbox patterns stubbed.

---

## State Transition Diagram (States 4-8 only)

```
State 4 (Scene Load Setup)
  |
  +-- sub_8223DB20 nonzero ---------> ERROR 33
  |
  +-- RESTART_FLAG set -------------> State 5
  |
  +-- sub_82240B08 returns 1 -------> State 9 (SKIP 5-8)
  |
  +-- sub_82240B08 returns 0:
  |     |
  |     +-- platformMode switch:
  |           mode 0,1: r11=0 ------> State 5
  |           mode 2:   r11=1 ------> ERROR 34
  |           mode 3:   (depends) --> State 5 or ERROR 34
  |           mode 4:   (depends) --> State 5 or ERROR 34
  |           mode >4:  r11=1 ------> ERROR 34
  v
State 5 (Content Enumeration)
  |
  +-- sub_8223DB20 nonzero ---------> ERROR 33
  |
  +-- RESTART_FLAG set: local_80=1, skip UI
  |     or
  +-- sub_8223F9F0(2) returns 0 ----> wait (return 1)
  |
  +-- local_80 != 0 ----------------> State 6 (has content)
  +-- local_80 == 0 ----------------> State 7 (no content)
  v
State 6 (Scene Create w/ Content)
  |
  +-- sub_8223DB20 nonzero ---------> ERROR 33
  +-- sub_826CBA70 nonzero ---------> wait (return 1)
  |
  +-- sub_8284AAE0 returns 0 -------> ERROR 7
  +-- sub_8284AAE0 returns nonzero -> State 8
  v
State 7 (Retry w/o Content)
  |
  +-- sub_8223DB20 nonzero ---------> ERROR 33
  |
  +-- sub_8223CB60 returns 1:
  |     sub_8223F9F0(3) returns 0 --> wait (return 1)
  |     local_80 != 0 AND r31 != 0 -> State 6
  |     local_80 == 0 --------------> ERROR 6
  |
  +-- sub_8223CB60 returns 0 -------> ERROR 6
  v
State 8 (Monitor Create Progress)
  |
  +-- sub_8284AB10 returns 0 -------> wait (return 1)
  +-- sub_8223DB20 nonzero ---------> ERROR 33
  |
  +-- sub_8284B490 returns 1:
  |     sub_8284B430 returns 2 -----> State 7 (retry)
  |     sub_8284B430 returns other -> ERROR 8
  |
  +-- sub_8284B490 returns other:
  |     sub_8284B430 returns 0 -----> check sub_82240B08
  |     sub_8284B430 returns other -> ERROR 9
  |
  +-- sub_82240B08 returns nonzero -> State 9
  +-- sub_82240B08 returns 0 -------> State 0 (restart)
```
