# XNotify System — Sign-In / Storage Device Change Guards

## Overview

The Xbox 360 XNotify system is a publish-subscribe mechanism for system events.
GTA IV's scene creation state machine (`sub_82242910`) uses two guard functions
that poll for notifications every frame. When a notification arrives, the guards
trigger error codes 33 or 34, aborting the current state and forcing a restart.

In the recomp, these guards are a source of spurious errors because RexGlue
enqueues `XN_SYS_SIGNINCHANGED` (0x0A) at startup, which the game immediately
picks up.

---

## Notification Infrastructure

### Listener Creation

GTA IV creates its XNotify listener in `sub_82A27170` (inside the game's
system-init object constructor):

```
r3 = 5          (mask)
r4 = 3          (max_version)
XamNotifyCreateListener(mask=5, max_version=3)
handle stored at obj+0x520
```

**Import address**: `0x82A74B74` -> `__imp__XamNotifyCreateListener`

**Mask = 5 (binary 101)**: bits 0 and 2 are set.
- Bit 0 = `XN_SYS` area (system notifications: sign-in, storage, UI, input)
- Bit 2 = `XN_FRIENDS` area

The `XNotificationKey` union (32-bit):
- bits 0-15: `local_id`
- bits 16-24: `version` (9 bits)
- bits 25-30: `mask_index` (6 bits)

For IDs 0x0A and 0x0B, mask_index = 0 (bits 25-30 are zero), so both pass
through the mask filter when bit 0 is set.

### Listener Handle Storage

The listener handle is stored at a global address derived from the game's
system-init object. The two guard functions load it from:

**`0x82BF3930`** (lis r11, -32065 => 0x82BF0000; lwz r3, 14640(r11))

### XNotifyGetNext

**Import address**: `0x82A74AA4` -> `__imp__XNotifyGetNext`

RexGlue implementation (`src/kernel/xam/xam_notify.cpp`):
- Looks up the listener handle in the object table
- If `match_id != 0`, dequeues only that specific notification ID
- Returns 1 if a notification was dequeued, 0 otherwise
- Both guard functions pass a specific `match_id` (10 or 11)

---

## Guard Functions

### sub_8223DB20 — Sign-In Change Guard (Error 33)

**File**: `gta4_recomp.6.cpp` line 73430
**Called ~30 times** across the scene creation state machine (states 4-14+)

**Logic** (decompiled pseudocode):
```c
bool sub_8223DB20() {
    uint32_t handle = *(uint32_t*)0x82BF3930;  // listener handle
    uint32_t id_out = 0;
    uint32_t param_out;

    // Poll for XN_SYS_SIGNINCHANGED (id=10)
    bool got = XNotifyGetNext(handle, 10, &id_out, &param_out);

    if (!got)
        return 0;  // no notification, all clear

    // Check if state at 0x82BF3D90 is -1
    int32_t state = *(int32_t*)0x82BF3D90;
    if (state == -1)
        return 0;  // readiness invalidated, ignore

    // Re-validate readiness
    bool ready = sub_8223DAA0();
    if (ready)
        return 1;  // notification consumed AND still ready -> trigger error
    else
        return 0;  // no longer ready, suppress
}
```

**Callers**: When this returns 1, callers branch to `loc_82240C84` which writes:
```
error_code = 33
*(uint32_t*)0x82A9546C = 33;
return 2;  // error return code
```

### sub_82240B78 — Storage Device Change Guard (Error 34)

**File**: `gta4_recomp.6.cpp` line 80352
**Called ~17 times** across the scene creation state machine

**Logic** (decompiled pseudocode):
```c
bool sub_82240B78() {
    uint32_t handle = *(uint32_t*)0x82BF3930;  // listener handle
    uint32_t id_out = 0;
    uint32_t param_out;

    // Poll for XN_SYS_STORAGEDEVICESCHANGED (id=11)
    bool got = XNotifyGetNext(handle, 11, &id_out, &param_out);

    if (!got)
        return 0;  // no notification

    // Re-validate readiness
    bool ready = sub_8223DAA0();
    if (!ready)
        return 0;  // not ready, suppress

    // Check storage device flag at 0x82BF3A77
    uint8_t devFlag = *(uint8_t*)0x82BF3A77;
    if (!devFlag)
        return 0;  // no storage device configured

    // Verify storage device still valid
    bool valid = sub_82240B08();
    if (valid)
        return 0;  // device still valid, suppress

    // Device changed and invalid -> trigger error
    *(uint8_t*)0x82BF3A76 = 1;   // error trigger flag
    *(uint8_t*)0x82A95467 = 1;   // secondary error flag
    return 1;
}
```

**Callers**: When this returns 1, callers branch to `loc_82240CAC` which writes:
```
error_code = 34
*(uint32_t*)0x82A9546C = 34;
return 2;  // error return code
```

---

## sub_8223DAA0 — Readiness Re-Validation

**File**: `gta4_recomp.6.cpp` line 73351
**Called by both guard functions** after a notification is dequeued.

**Logic** (decompiled pseudocode):
```c
bool sub_8223DAA0() {
    // Reset state to -1 (invalidate)
    *(int32_t*)0x82BF3D90 = -1;

    // Check some condition via sub_826CD808
    if (!sub_826CD808())
        return 0;  // not ready

    // Get device/user index
    uint32_t result = sub_829DB9B8(sub_826CD808());
    *(int32_t*)0x82BF3D90 = result;

    if (result < 0)
        return 1;  // negative = error condition -> trigger error path

    // Validate via sub_82A12188
    if (sub_82A12188() != 0)
        return 1;  // validation failed -> trigger error path

    // All good, reset to -1
    *(int32_t*)0x82BF3D90 = -1;
    return 0;  // suppress notification
}
```

---

## sub_82240B08 — Storage Device Validation

**File**: `gta4_recomp.6.cpp` line 80286
**Called by sub_82240B78** after notification + readiness checks pass.

Validates whether the storage device is still accessible. If validation passes
(`sub_8284B3D8` returns nonzero), it sets storage flags:
- `*(uint8_t*)0x82BF3A77 = 1` (storage device flag)
- `*(uint8_t*)0x82BF3CDA = 1` (secondary flag at 0x82BF0000 + 15578)

Then returns 1 (device valid, suppress error). If validation fails, calls
`sub_8223DB90` and returns 0 (device invalid, allow error).

---

## Key Memory Addresses

| Address      | Size | Description |
|-------------|------|-------------|
| `0x82BF3930` | u32  | XNotify listener handle |
| `0x82BF3D90` | s32  | Readiness state (-1 = invalidated) |
| `0x82BF3A76` | u8   | Error trigger flag (set by sub_82240B78) |
| `0x82BF3A77` | u8   | Storage device configured flag |
| `0x82BF3CDA` | u8   | Secondary storage flag (offset 15578) |
| `0x82A9546C` | u32  | Error code output (33 or 34) |
| `0x82A95467` | u8   | Secondary error flag |

---

## RexGlue Notification Behavior

### What fires at startup

`KernelState::RegisterNotifyListener()` (`kernel_state.cpp:595`) enqueues
these notifications for the FIRST listener registered:

| ID   | Name                          | Count |
|------|-------------------------------|-------|
| 0x09 | XN_SYS_UI                     | 2 (on/off) |
| 0x0A | XN_SYS_SIGNINCHANGED          | 2 |
| 0x12 | XN_SYS_INPUTDEVICESCHANGED    | 2 |
| 0x13 | XN_SYS_INPUTDEVICECONFIGCHANGED | 2 |

### What fires at runtime

`XamShowSigninUI_entry()` (`xam_user.cpp:499`) broadcasts:
- `0x0A` (XN_SYS_SIGNINCHANGED) with data=1
- `0x09` (XN_SYS_UI off) with data=0

### What NEVER fires

**`0x0B` (XN_SYS_STORAGEDEVICESCHANGED) is NEVER broadcast anywhere in RexGlue.**

Grep of the entire `src/` tree confirms no code calls
`BroadcastNotification(0xB, ...)` or enqueues notification ID 0x0B.

---

## Analysis: Do Notifications Cause Errors in the Recomp?

### XN_SYS_SIGNINCHANGED (0x0A) -> Error 33

**YES, this fires.** At startup, RexGlue enqueues two 0x0A notifications.
When `sub_8223DB20` first polls with `match_id=10`, it dequeues one. Then:

1. It checks `0x82BF3D90 != -1` (readiness state)
2. If not -1, it calls `sub_8223DAA0` to re-validate

Whether this triggers error 33 depends on timing:
- If readiness state is still -1 (startup default), the guard returns 0 (safe)
- If readiness was already set (e.g., by states 1-3), the guard may return 1 (error)

The existing `sub_82242910` hook in `imports.cpp` already works around this
by forcing the normal path (state 0 -> 1 instead of 0 -> 4), and the
`sub_822422E0` hook resets `0x82BF9834` as a safety net.

### XN_SYS_STORAGEDEVICESCHANGED (0x0B) -> Error 34

**NO, this never fires.** RexGlue never broadcasts 0x0B. The `sub_82240B78`
guard will always find an empty queue when polling for `match_id=11`, so it
always returns 0 and never triggers error 34 via this path.

Error 34 is triggered through OTHER paths in the state machine (stale platform
mode values), not through storage device notifications.

---

## Stub Feasibility

### Can sub_8223DB20 be stubbed to always return 0?

**Yes, safely.** Stubbing it to return 0 means "no sign-in change detected."
This is correct for the recomp because:
- There is no real Xbox Live sign-in system
- The startup 0x0A notifications are artifacts of RexGlue's compatibility layer
- The function is purely a guard; returning 0 just means "keep going"
- All ~30 call sites simply check `r3 != 0` and branch to error 33 if true

### Can sub_82240B78 be stubbed to always return 0?

**Yes, safely.** Stubbing it to return 0 means "no storage device change."
This is correct because:
- 0x0B is never broadcast by RexGlue, so it already always returns 0
- Stubbing just saves the overhead of the XNotifyGetNext lookup
- All ~17 call sites simply check `r3 != 0` and branch to error 34 if true

### Recommended stub implementation

```cpp
// Suppress XNotify sign-in change guard (prevents spurious error 33)
PPC_FUNC_HOOK(sub_8223DB20) {
    ctx.r3.s64 = 0;  // no notification
}

// Suppress XNotify storage device change guard (prevents spurious error 34)
PPC_FUNC_HOOK(sub_82240B78) {
    ctx.r3.s64 = 0;  // no notification
}
```

These are safe because:
1. No real sign-in or storage device changes will ever occur in the recomp
2. Both functions only serve as guards that abort the state machine
3. The return value is a simple boolean (0 = continue, 1 = error)

---

## Existing Hooks (imports.cpp)

There are **no existing hooks** for `sub_8223DB20` or `sub_82240B78`.

The existing workarounds in `imports.cpp` address the SAME error codes (33, 34)
but through different mechanisms:

| Hook | What it does | Error prevented |
|------|-------------|-----------------|
| `sub_822422E0` | Resets `0x82BF9834` from 2 -> 0 after state 5 | Error 34 (stale platform mode) |
| `sub_82242910` | Forces state 0 -> 1 (not 0 -> 4); pre-sets platformMode=3 | Error 34 (fast path skip) |

These hooks address the **platform mode path** to error 34, while `sub_82240B78`
addresses the **storage device notification path** to error 34. Both paths write
to the same error code address `0x82A9546C`.

---

## Call Graph

```
sub_82242910 (scene creation state machine, 15 states)
  |
  +-- State 4+: calls sub_8223DB20() [sign-in guard]
  |     |
  |     +-- XNotifyGetNext(handle, 10, &id, &param)
  |     +-- if got: sub_8223DAA0() [re-validate readiness]
  |     +-- returns 1 -> caller writes error 33 to 0x82A9546C
  |
  +-- State 4+: calls sub_82240B78() [storage device guard]
        |
        +-- XNotifyGetNext(handle, 11, &id, &param)
        +-- if got: sub_8223DAA0() [re-validate readiness]
        +--         check 0x82BF3A77 [device flag]
        +--         sub_82240B08() [validate device]
        +-- returns 1 -> caller writes error 34 to 0x82A9546C
```
