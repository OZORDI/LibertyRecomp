# Outer State Machine (sub_822438B0) and XNotify Guard Hooks

## Overview

This document designs concrete hooks for:
1. **sub_822438B0** -- The outer save state machine (8 states, 0-7)
2. **sub_8223DB20** -- Sign-in change guard (error 33)
3. **sub_82240B78** -- Storage device change guard (error 34)
4. **sub_82240B08** -- Content device readiness check

The goal is to preserve game logic while eliminating Xbox 360-specific notification
and device validation paths that cause spurious errors in the recomp.

---

## 1. sub_822438B0 -- Outer Save State Machine

### Current Hook (imports.cpp line 1743)

The existing hook is **passthrough with logging**: it calls `__imp__sub_822438B0`,
logs state transitions, and returns the original result. It does NOT modify behavior.

### State-by-State Analysis

| State | Name | Xbox Dependencies | Recomp Concerns |
|-------|------|-------------------|-----------------|
| 0 | IDLE | None | Safe. Returns 0 immediately. |
| 1 | INITIATE | None | Safe. Sets globals and falls through to state 2. |
| 2 | ENUMERATE | Calls **sub_82242910** | Already hooked (fast-path interception). |
| 3 | SAVE OP | Calls **sub_82240F80(0)** | Checks "SAVE" magic in buffer at 0x82BF394C. |
| 4 | VALIDATE | Calls sub_822446E8, sub_8223F740 | Only runs validation when SUB_STATE==2. |
| 5 | WRITE | Calls **sub_82240F80(1)** | Same function as state 3 but write phase. |
| 6 | FINALIZE | Timer check, sub_826CD808, sub_829DBAA8 | Timer struct at r30 (0x82BF3934). |
| 7 | ERROR/RETRY | Calls **sub_82242608** (shows Guide UI dialog) | Guide UI dependency. |

### Key Question: Hook sub_822438B0 or let it run?

**Recommendation: Let it run as-is.** The outer SM itself contains no Xbox-specific
code. All Xbox dependencies are in the *called* functions:

- **sub_82242910** (state 2): Already hooked with fast-path interception.
- **sub_82240F80** (states 3, 5): The save I/O function -- uses VFS, not Xbox APIs directly.
- **sub_82242608** (state 7): Error retry -- calls sub_8223F9F0 (Guide UI dialog).

The state machine logic (transitions, error codes, timer checks, "SAVE" magic
validation) is all pure game logic that should be preserved. Hooking the inner
functions is the correct approach.

### State 2 and sub_82242910

State 2 calls sub_82242910 and checks its return:
- r3 == 2 (error): checks error_code at 0x82A9546C for value 33 (sign-in cancel)
- r3 == 0 (success): zeroes buffer at 0x82BF3940, advances to state 3
- r3 == 1 (in progress): returns 1

The existing sub_82242910 hook (imports.cpp line 1816) handles the fast-path
interception (state 0->4 forced to 0->1) and pre-sets platformMode=3. With that
hook in place, the outer SM's state 2 works correctly.

### State 7 and Error Recovery

State 7 does:
1. Calls sub_8223CC68(r30, 2) -- cancel/reset timer struct
2. Calls sub_82242608 -- error retry handler

sub_82242608 (generated code line 84353) is simple:
1. Reads error_code from 0x82A9546C
2. Calls sub_8223F9F0(error_code, 0, &output_flag) -- the Guide UI dialog function
3. If sub_8223F9F0 returns nonzero: writes error_code=6, returns 1 (done)
4. If sub_8223F9F0 returns 0: returns 0 (still waiting)

**Critically**, sub_82242608 also calls sub_8223DB20 and sub_82240B78 at line 84465
and 84489 (via sub_82242678 which is the content enumeration state 0 handler).
These guard checks happen WITHIN the error retry path.

If sub_8223DB20 or sub_82240B78 return 1 during error recovery, they write
error codes 33 or 34 respectively to 0x82A9546C and return 2, which causes
the outer SM's epilogue to reset STATE=0. This creates a restart loop.

**Fix**: Stubbing the guard functions to return 0 prevents this restart loop
and lets sub_82242608 proceed with showing the error dialog (which sub_8223F9F0
handles headlessly via RexGlue).

---

## 2. sub_8223DB20 -- Sign-In Change Guard

### Generated Code Analysis (line 73432)

```
1. Load listener handle from 0x82BF3930
2. Call XNotifyGetNext(handle, match_id=10, &id_out, &param_out)
3. If no notification: return 0
4. Load readiness state from 0x82BF3D90
5. If readiness == -1: return 0 (invalidated, ignore)
6. Call sub_8223DAA0() (re-validate readiness)
7. If sub_8223DAA0 returns nonzero: return 1 (trigger error 33)
8. Else: return 0
```

### Does It Do Any Game Logic?

**No.** This function is purely a notification guard. It:
- Polls XNotifyGetNext for sign-in change notifications (ID 10 / 0x0A)
- Validates whether the notification is relevant via sub_8223DAA0
- Returns a boolean: 0 = continue, 1 = abort with error 33

It writes nothing to game state. All state writes happen in the callers.

### Can It Be Safely Stubbed to Return 0?

**Yes.** Returning 0 means "no sign-in change detected." This is correct because:
1. There is no real Xbox Live sign-in system in the recomp
2. RexGlue enqueues spurious XN_SYS_SIGNINCHANGED (0x0A) at startup
3. The function is called ~30 times across the scene creation state machine
4. Every caller simply checks `r3 != 0` and branches to error 33 if true
5. sub_8223DAA0 (the readiness re-validation) has side effects (writes -1 to
   0x82BF3D90, calls sub_826CD808/sub_829DB9B8) but these are ONLY reached
   after a notification is dequeued -- which we prevent by returning early

### Proposed Hook

```cpp
// Suppress XNotify sign-in change guard -- prevents spurious error 33
// from RexGlue's startup XN_SYS_SIGNINCHANGED broadcasts.
// Pure guard function with no game-state side effects.
PPC_FUNC_HOOK(sub_8223DB20) {
    ctx.r3.s64 = 0;  // no sign-in change detected
}
```

---

## 3. sub_82240B78 -- Storage Device Change Guard

### Generated Code Analysis (line 80354)

```
1. Load listener handle from 0x82BF3930
2. Call XNotifyGetNext(handle, match_id=11, &id_out, &param_out)
3. If no notification: return 0
4. Call sub_8223DAA0() (re-validate readiness)
5. If not ready: return 0
6. Load device flag from 0x82BF3A77
7. If device flag == 0: return 0 (no storage device configured)
8. Call sub_82240B08() (validate device still accessible)
9. If device valid (returns nonzero): return 0 (suppress)
10. Device invalid: set 0x82BF3A76=1, 0x82A95467=1, return 1
```

### Does It Do Any Game Logic?

**Mostly no.** The only state writes happen in the error path (step 10):
- 0x82BF3A76 = 1 (error trigger flag)
- 0x82A95467 = 1 (secondary error flag)

These are error flags that the callers also set. The function's purpose is to
detect Xbox storage device removal and trigger error 34.

### Can It Be Safely Stubbed to Return 0?

**Yes.** This is even safer than sub_8223DB20 because:
1. RexGlue NEVER broadcasts XN_SYS_STORAGEDEVICESCHANGED (0x0B)
2. The function already always returns 0 in the current recomp
3. Stubbing saves the overhead of the XNotifyGetNext lookup
4. No storage devices will ever be physically removed in the recomp

### Proposed Hook

```cpp
// Suppress XNotify storage device change guard -- prevents spurious error 34.
// RexGlue never broadcasts 0x0B, so this already returns 0 in practice.
// Stubbing eliminates the XNotifyGetNext call overhead.
PPC_FUNC_HOOK(sub_82240B78) {
    ctx.r3.s64 = 0;  // no storage device change detected
}
```

---

## 4. sub_82240B08 -- Content Device Readiness

### Generated Code Analysis (line 80288)

```
1. Compute slot base: lis r11,-31975 -> 0x83190000 + 11344 = 0x83192C50
2. Load readiness state from 0x82BF3D90 (offset 15760 from 0x82BF0000)
3. Call sub_8284B3D8(slot_base, readiness_state) -- device validation
4. If sub_8284B3D8 returns 0 (device invalid):
   a. Call sub_8223DB90 (cleanup/reset)
   b. Return 0 (device not ready)
5. If sub_8284B3D8 returns nonzero (device valid):
   a. Set 0x82BF3A77 = 1 (storage device flag)
   b. Set 0x82BF3CDA = 1 (secondary storage flag)
   c. Return 1 (device ready)
```

### What Does State 4 Need?

sub_82240B08 is called by sub_82240B78, NOT directly by the outer SM's state 4.
The outer SM's state 4 (VALIDATE) does NOT call sub_82240B08. State 4 calls:
- sub_822446E8 (post-save setup)
- sub_8223F740(0) (validation check)

Both are only called when SUB_STATE==2. The device readiness flags at 0x82BF3A77
and 0x82BF3CDA are used by the storage device guard (sub_82240B78), not by the
outer SM state 4 directly.

### What Should the Hook Do?

If hooked, sub_82240B08 should simulate a valid storage device:
- Set 0x82BF3A77 = 1 (storage device configured)
- Set 0x82BF3CDA = 1 (secondary storage flag)
- Return 1 (device ready)

However, since sub_82240B78 is already stubbed to return 0, sub_82240B08 will
never be called through the notification path. It IS called from other locations
(e.g., sub_82242678 at line 84489 in the content enumeration flow).

### Proposed Hook

```cpp
// Content device readiness -- always report device as valid.
// Sets the storage flags that downstream code expects and returns 1.
// This is defense-in-depth; sub_82240B78 is stubbed to never reach here
// via the notification path, but other callers may still invoke this.
PPC_FUNC_HOOK(sub_82240B08) {
    PPC_STORE_U8(0x82BF3A77, 1);  // storage device configured
    PPC_STORE_U8(0x82BF3CDA, 1);  // secondary storage flag
    ctx.r3.s64 = 1;               // device ready
}
```

---

## 5. Complete Hook Set

### Registration (in imports.cpp REGISTER block)

```cpp
// XNotify guard stubs
PPC_FUNC_HOOK(sub_8223DB20) {
    ctx.r3.s64 = 0;  // no sign-in change
}

PPC_FUNC_HOOK(sub_82240B78) {
    ctx.r3.s64 = 0;  // no storage device change
}

PPC_FUNC_HOOK(sub_82240B08) {
    PPC_STORE_U8(0x82BF3A77, 1);  // storage device configured
    PPC_STORE_U8(0x82BF3CDA, 1);  // secondary storage flag
    ctx.r3.s64 = 1;               // device ready
}
```

### What About sub_822438B0?

**Keep the existing passthrough hook.** The outer SM should run as-is with the
guard functions stubbed. The current hook at imports.cpp line 1743 provides
valuable diagnostic logging. No behavioral changes needed.

### What About sub_82242608 (Error/Retry)?

sub_82242608 calls sub_8223F9F0 (Guide UI dialog). With the guard stubs in place,
the error/retry path will no longer be triggered by spurious notifications.
If state 7 IS reached (due to genuine errors like bad "SAVE" magic), sub_82242608
should still work because sub_8223F9F0 runs headlessly through RexGlue.

No hook needed for sub_82242608.

---

## 6. Risk Assessment

| Hook | Risk | Mitigation |
|------|------|------------|
| sub_8223DB20 = 0 | Low | No side effects; pure guard |
| sub_82240B78 = 0 | Very Low | Already returns 0 in practice (0x0B never broadcast) |
| sub_82240B08 = 1 | Low | Sets same flags the original does on success path |
| sub_822438B0 passthrough | None | No behavioral change from current |

### Interaction with Existing Hooks

| Existing Hook | Interaction | Status |
|---------------|-------------|--------|
| sub_82242910 (fast-path fix) | Guard stubs prevent error 33/34 during scene creation | Complementary |
| sub_822422E0 (SUB_STATE reset) | Guard stubs prevent the stale-value-2 path from triggering | Complementary |
| sub_822438B0 (logging) | No change -- keeps diagnostic output | Compatible |

### Error Code 33 Path (Eliminated)

Before: RexGlue startup -> XN_SYS_SIGNINCHANGED enqueued -> sub_8223DB20 dequeues
-> sub_8223DAA0 returns 1 -> guard returns 1 -> error_code=33 -> state 7 -> retry loop

After: sub_8223DB20 returns 0 -> no error -> state machine proceeds normally

### Error Code 34 Path (Already Inactive, Now Explicitly Blocked)

Before: XN_SYS_STORAGEDEVICESCHANGED never broadcast -> sub_82240B78 always returns 0

After: sub_82240B78 stubbed to return 0 -> identical behavior, lower overhead

---

## 7. Memory Address Reference

All addresses verified via Python arithmetic against the generated PPC code.

| Address | Size | Field | Used By |
|---------|------|-------|---------|
| 0x82BF3930 | u32 | XNotify listener handle | sub_8223DB20, sub_82240B78 |
| 0x82BF3934 | struct | Timer/request struct (r30) | sub_822438B0 states 2, 6, 7 |
| 0x82BF3940 | 16B | Data buffer (zeroed in state 2->3) | sub_822438B0 state 3 SAVE check |
| 0x82BF3A76 | u8 | Error trigger flag | sub_82240B78 error path |
| 0x82BF3A77 | u8 | Storage device configured flag | sub_82240B08, sub_82240B78 |
| 0x82BF3A78 | struct | Completion struct (r27) | sub_822438B0 epilogue |
| 0x82BF3CDA | u8 | Secondary storage flag | sub_82240B08 |
| 0x82BF3D90 | s32 | Readiness state (-1=invalidated) | sub_8223DB20, sub_8223DAA0 |
| 0x82BF9834 | u32 | SUB_STATE | sub_822438B0 |
| 0x82BF9838 | u32 | STATE (outer SM, 0-7) | sub_822438B0 |
| 0x82A9546C | u32 | Error code | All error paths |
| 0x82A95467 | u8 | Secondary error flag | sub_82240B78 |
