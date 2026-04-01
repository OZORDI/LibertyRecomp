# Error Codes and Ready Signal

## Part 1: Error Code System (0x82A9546C)

### Address

The global error code variable lives at `0x82A9546C`.
In the generated PPC code this is accessed via `lis rX, -32087` (loads `0x82A90000`)
followed by `stw/lwz rY, 21612(rX)` (offset `0x546C`).

### Overview

43 stores to this address were found across 10 functions.  Error codes range from 6 to 50.
There are **no stores of 0** to this address -- the error code is never explicitly cleared.
Once set, it persists until overwritten by another error code.

Most error-setting paths also set `r3` to a return value (typically 2 = "error/retry",
0 = "fatal/abort", or 1 = "continue with error noted").

### Error Codes by Category

#### Initialization / XAM Notification Errors (set by sub_8223DB20 / sub_82240B78)

These two functions call `XNotifyGetNext` with notification IDs 10 and 11 respectively.
When they return nonzero (notification received), the caller sets error code 33 or 34.

| Code | Setter Function(s) | Condition | Meaning |
|------|-------------------|-----------|---------|
| 33 | sub_82240C18, sub_82240F80, sub_822417B0, sub_82242678, sub_82242910 | `sub_8223DB20()` returns nonzero | XAM notification 10 (sign-in change) detected |
| 34 | sub_82240C18, sub_82240F80, sub_822417B0, sub_82242678, sub_82242910 | `sub_82240B78()` returns nonzero | XAM notification 11 (storage device change) detected |

Error 33 appears in 5 different functions (5 stores total).
Error 34 appears in 5 different functions (5 stores total).
These are the most common error codes -- every content-management function checks for them.

#### Boot/Startup Errors (sub_8223F568)

| Code | Setter Function | Condition | Meaning |
|------|----------------|-----------|---------|
| 6 | sub_8223F568 | Unconditional during initialization | Initial error state / content system reset; also clears several state variables at 0x82BF97xx and sets a "dirty" byte at 0x82A95467 |

#### Content Validation Pipeline (sub_82240C18)

This function validates content packages (DLC/title updates) in sequence.

| Code | Setter Function | Condition | Meaning |
|------|----------------|-----------|---------|
| 17 | sub_82240C18 | Content validation step fails (after logging string at offset 25348) | Content package header invalid |
| 18 | sub_82240C18 | Next validation step fails (string at offset 25280) | Content package signature invalid |
| 19 | sub_82240C18 | Validation fails (string at offset 25208) | Content package integrity check failed |
| 20 | sub_82240C18 | Validation fails (string at offset 25156) | Content metadata mismatch |
| 21 | sub_82240C18 | Validation fails (string at offset 25080) | Content file listing error |
| 22 | sub_82240C18 | Validation fails (string at offset 25000) | Content mount/registration error |

All return `r3 = 2` (retry/error state).

#### Content Installation Pipeline (sub_82240F80)

This function handles content installation and mounting.

| Code | Setter Function | Condition | Meaning |
|------|----------------|-----------|---------|
| 23 | sub_82240F80 | Installation step fails (string at offset 25604) | Content installation init failed |
| 24 | sub_82240F80 | Installation step fails (string at offset 25528) | Content file extraction failed |
| 25 | sub_82240F80 | Installation fails (string at offset 25448) | Content registration failed |
| 26 | sub_82240F80 | `lbz r11, 80(r1)` is zero (local flag) | Content completion flag not set |
| 27 | sub_82240F80 | Installation step fails (string at offset 25396) | Content finalization failed |
| 28 | sub_82240F80 | Pointer comparison fails: `r10 != 0` AND `r10 != r11` | Content handle mismatch (stale pointer) |
| 29 | sub_82240F80 | `sub_8284B490()` returns 1 | Content device reports error |
| 30 | sub_82240F80 | `sub_8284B430()` returns nonzero | Content device state invalid |

All return `r3 = 2`.

#### Save/Load State Machine (sub_822417B0)

| Code | Setter Function | Condition | Meaning |
|------|----------------|-----------|---------|
| 10 | sub_822417B0 | `sub_8284B010()` returns zero (masked to byte) | Save device not ready |

Returns `r3 = 2`.

#### Content Enumeration / Mounting (sub_82242910)

This is the main content enumeration state machine.

| Code | Setter Function | Condition | Meaning |
|------|----------------|-----------|---------|
| 6 | sub_82242910 | `lbz r11, 80(r1)` is zero AND `r31` is zero | No content found and no device available |
| 7 | sub_82242910 | `sub_8284AAE0()` returns zero (byte-masked) | Content enumeration start failed |
| 8 | sub_82242910 | Reached via `loc_82242E84` fallthrough | Content enumeration timed out or failed |
| 9 | sub_82242910 | `sub_8284B430()` returns nonzero | Content device error during enumeration |
| 14 | sub_82242910 | `sub_8284ABA0()` returns zero (byte-masked) | Content item validation failed |
| 15 | sub_82242910 | `sub_8284B490()` returns 1 | Content device reports enumeration error |
| 16 | sub_82242910 | `sub_8284B430()` does NOT return 5 | Content device not in expected state (state 5) |

All return `r3 = 2`.

#### Content Query Dispatch (sub_82242678)

| Code | Setter Function | Condition | Meaning |
|------|----------------|-----------|---------|
| 33 | sub_82242678 | `sub_8223DB20()` nonzero | XAM sign-in change (same as above) |
| 34 | sub_82242678 | `sub_82240B78()` nonzero | Storage device change (same as above) |

#### Content State Reset (sub_82243260)

| Code | Setter Function | Condition | Meaning |
|------|----------------|-----------|---------|
| 6 | sub_82243260 | Content validation failure (string at offset 26448) | Content system fatal reset |
| 32 | sub_82243260 | `sub_8223E980()` returns zero AND string logged at offset 26368 | Content state machine desync (unexpected state) |
| 6 | sub_82243260 | After `sub_8223CC68(r29, 1)` call | Post-cleanup reset to error state 6 |

#### Network/Live Errors (sub_822438B0)

| Code | Setter Function | Condition | Meaning |
|------|----------------|-----------|---------|
| 29 | sub_822438B0 | XAM check fails (string at offset 26544) | Live service unavailable |
| 31 | sub_822438B0 | `sub_8223F740(0)` returns zero (byte-masked) | Live content verification failed |

Both also set a secondary state variable to 7 at offset -26568 from r31/r28.

#### Session/Multiplayer (sub_822440F8)

| Code | Setter Function | Condition | Meaning |
|------|----------------|-----------|---------|
| 50 | sub_822440F8 | `lbz r11, 80(r1)` is nonzero | Session/multiplayer join error |

Returns `r3 = 0` (fatal) and sets a state variable to 5 at offset -26156.

#### Content Load Dispatch (sub_82242608)

| Code | Setter Function | Condition | Meaning |
|------|----------------|-----------|---------|
| 6 | sub_82242608 | `sub_8223F9F0(error_code)` returns nonzero | Content load dispatch found existing error |

This function READS the current error code from `0x82A9546C`, passes it to `sub_8223F9F0`,
and if that returns nonzero, overwrites the error code with 6 and returns `r3 = 1`.

#### RPF/Content Package Count (sub_8223D400)

| Code | Setter Function | Condition | Meaning |
|------|----------------|-----------|---------|
| 42 | sub_8223D400 | Content list count > 75 (via `cmpwi cr6, r11, 75; ble`) | Too many content packages registered |
| 44 | sub_8223D400 | Reached `loc_8223D650` during content iteration | Content package parse error during iteration |

Both return `r3 = 0` (fatal).  The function iterates content entries in a loop with stride 308 bytes,
up to 15 entries (4620 / 308 = 15).

#### XAM Readiness (sub_8223ECA8)

| Code | Setter Function | Condition | Meaning |
|------|----------------|-----------|---------|
| 46 | sub_8223ECA8 | `sub_8221AD28()` returns zero (byte-masked) AND string logged at offset 23416 | XAM subsystem not responding |

### Error Code Readers

The error code at `0x82A9546C` is read (via `lwz r11, 21612(r11)`) in `sub_82243260`
and `sub_822438B0`.  Both check specifically for error code 33 (`cmpwi cr6, r11, 33`).
If the error code equals 33, these functions take an alternate path (likely skipping
content operations when a sign-in change has been detected).

---

## Part 2: Ready Signal (0x82BF9B70)

### Address

The XAM readiness flag lives at `0x82BF9B70`.
Accessed via `lis rX, -32064` (loads `0x82C00000`) + offset `-25744` (= `0xFFFF9B70`).

```
0x82C00000 + 0xFFFF9B70 = 0x82BF9B70
```

### Key Functions

#### sub_82254FE0 -- "SetReady" (writes 1)

**Location**: `gta4_recomp.7.cpp:29074`

Stores the value `1` to `0x82BF9B70`, then calls several setup functions:
- `sub_8224CD48` -- initializes subsystem A
- `sub_8224CD88` -- initializes subsystem B (with parameter from `r4`)
- Stores additional parameters to nearby globals (`-25740`, `-25750`, `-25748`, `-25751`)
- `sub_8224CDC8` -- initializes subsystem C
- `sub_8224CE08` -- initializes subsystem D (with parameter from `r10`)
- Conditionally calls `sub_82252D08` (screen/UI clear) if byte at `-24252` is nonzero

**Callers** (8 call sites):
- `sub_82144B98` (1 call) -- initial game setup
- `sub_8214B640` (4 calls) -- **the key caller: render/update loop with multiple code paths**
- `sub_8214DBD0` (1 call) -- secondary game loop path
- `sub_82153290` (2 calls) -- another game loop path

#### sub_8214C8C8 -- "TickReady" (increments toward 4)

**Location**: `gta4_recomp.0.cpp:30455`

This function is called each frame from the main game loop.  Its logic:

```
flag = load(0x82BF9B70)

if sub_8224FA48() != 0:         // flag != -1 (system not disabled)
    if flag < 4:
        flag++
        store(0x82BF9B70, flag)  // increment
    else:
        sub_8224FA38()           // set flag = -1 (mark complete)

sub_8214B168()                   // frame processing
sub_8214B640()                   // *** CALLS sub_82254FE0 which RESETS flag to 1 ***
// ... rest of frame logic (content state machine, etc.)
```

**Callers** (2 call sites):
- `sub_821428C8` (at `0x821428DC`) -- main game loop variant A
- `sub_82142F90` (at `0x82142FBC`) -- main game loop variant B

#### sub_8224FA48 -- "IsReady" (returns 0 if flag == -1, else 1)

**Location**: `gta4_recomp.7.cpp:16178`

```c
uint32_t IsReady() {
    int32_t flag = load(0x82BF9B70);
    // PPC bit trick: add 1, count leading zeros, extract bit, xor
    // Equivalent to: return (flag != -1) ? 1 : 0;
    return (flag + 1) != 0 ? 1 : 0;
}
```

The PPC implementation uses `cntlzw` + `rlwinm` + `xori`:
- If `flag == -1` (0xFFFFFFFF): `flag+1 = 0`, `cntlzw = 32`, bit 5 extracted = 1, `xori 1` = **0**
- If `flag == anything else`: `flag+1 != 0`, `cntlzw < 32`, bit 5 = 0, `xori 1` = **1**

**Callers**: 14 call sites across `gta4_recomp.0.cpp`, `gta4_recomp.2.cpp`, `gta4_recomp.3.cpp`, `gta4_recomp.6.cpp`.  Every content management function checks this before proceeding.

#### sub_8224FA38 -- "MarkComplete" (writes -1)

**Location**: `gta4_recomp.7.cpp:16164`

```c
void MarkComplete() {
    store(0x82BF9B70, -1);  // 0xFFFFFFFF
}
```

Called when flag reaches >= 4 (from sub_8214C8C8) or during shutdown/reset.

#### Other functions that write -1 to 0x82BF9B70

- `sub_8224CCC8` (gta4_recomp.7.cpp:9228) -- system shutdown/cleanup
- `sub_8224DBE0` (gta4_recomp.7.cpp:11584) -- error recovery path
- `sub_8224DE10` (gta4_recomp.7.cpp:11929) -- error recovery path
- `sub_82255AF0` (gta4_recomp.7.cpp:30914) -- late cleanup

All of these store -1 to disable the ready system.

### The Oscillation Problem

#### Why it oscillates 1 -> 2 -> 1 -> 2 forever

The call chain creates a self-defeating cycle:

```
Frame N:
  sub_8214C8C8 enters
    sub_8224FA48() returns 1 (flag=1, not disabled)
    flag=1, flag < 4, so increment: flag = 2, store to 0x82BF9B70
    ...
    sub_8214B640()          <-- called later in the same function
      sub_82254FE0()        <-- RESETS flag back to 1!
        store(0x82BF9B70, 1)

Frame N+1:
  sub_8214C8C8 enters
    sub_8224FA48() returns 1 (flag=1 again)
    flag=1, flag < 4, increment: flag = 2, store to 0x82BF9B70
    ...
    sub_8214B640()
      sub_82254FE0()        <-- RESETS flag back to 1 again!
```

The root cause: **sub_8214C8C8 increments the flag early in its execution, but then calls sub_8214B640 later, which calls sub_82254FE0, which unconditionally writes 1 back to the flag.**  The increment from 1->2 is immediately undone by the reset to 1.

The flag value observed oscillates between 1 (after sub_82254FE0 resets it) and 2 (after sub_8214C8C8 increments it), depending on the exact point of observation within the frame.

#### What value does it need to reach?

The flag needs to reach **4** (or higher).  At `flag >= 4`, sub_8214C8C8 calls `sub_8224FA38()` which stores -1 (marking the ready system as complete/disabled).  Once -1, `sub_8224FA48()` returns 0, and all content management functions skip their initialization checks and proceed with normal operation.

#### How to fix

The fix requires one of:
1. **Hook sub_82254FE0** to only write 1 on the first call (not every call), allowing the counter to accumulate past 4
2. **Hook sub_8214C8C8** to skip the increment logic and directly call `sub_8224FA38()` (write -1), immediately marking the system as ready
3. **Pre-write -1 to 0x82BF9B70** before the game loop starts, bypassing the entire readiness ramp-up

Option 2 or 3 is simplest.  Writing -1 directly to `0x82BF9B70` causes `sub_8224FA48` to return 0 everywhere, which tells all content management functions that the XAM readiness handshake is complete.

### Related Globals

| Address | Offset from 0x82C00000 | Purpose |
|---------|----------------------|---------|
| 0x82BF9B70 | -25744 | XAM ready flag (this document) |
| 0x82BF9B74 | -25740 | Stored by sub_82254FE0 (parameter r5/r30) |
| 0x82BF9B66 | -25750 | Byte stored by sub_82254FE0 (parameter r6/r29) |
| 0x82BF9B68 | -25748 | Stored by sub_82254FE0 (parameter r7/r28) |
| 0x82BF9B65 | -25751 | Byte stored by sub_82254FE0 (parameter r8/r27) |
| 0x82BFA104 | -24252 | Byte flag checked by sub_82254FE0 (UI clear condition) |
| 0x82BFA0CC | -24308 | Used by sub_8224FA78 (cleanup: stores -90 to 5 consecutive words) |
| 0x82BFA0E4 | -24284 | Game phase counter (compared against 3, 5, 9 in sub_8214C8C8) |
