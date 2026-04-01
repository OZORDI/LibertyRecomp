# sub_8223DAA0 — Device/Sign-in Readiness Check

## Location
- **File**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.6.cpp`, line 73351
- **Address**: `0x8223DAA0`
- **Related**: `sub_8223DB20` at line 73430 (sign-in change notification handler)

## Purpose

`sub_8223DAA0` is the "readiness check" function called by state 0 of the frontend state machine (`sub_82242910`). It determines whether the game's storage device manager and user sign-in are ready. Its return value controls whether the state machine takes the **fast path** (skip to state 4) or the **normal path** (proceed to state 1).

## Return Values

| Return | Meaning | State Transition |
|--------|---------|-----------------|
| 0      | Not ready (device not enumerated or user not signed in) | State 0 -> State 1 (normal path) |
| 1      | Ready (device enumerated AND user signed in) | State 0 -> State 4 (fast path) |

## Memory Addresses

All addresses computed with Python (`lis rN, imm` = `imm * 65536`, then `addi`/offset).

| Address | Type | Description |
|---------|------|-------------|
| `0x82BF3D90` | `int32_t` | **Readiness field** — stores the active user index (>=0) or -1 (no user). Written by sub_8223DAA0, read by sub_8223DB20 and state 0's fast path. |
| `0x8318AAF8` | object ptr | Device/storage manager object (passed to sub_829DB9B8 and sub_822BF360) |
| `0x82A9172C` | `int32_t` | Cached sign-in user index (read by sub_826CD808 at startup) |
| `0x82BF3930` | `uint32_t` | XNotify listener handle (read by sub_8223DB20) |
| `0x82BF9848` | `uint32_t` | State counter for sub_82242910 (the outer state machine) |
| `0x82A9546C` | `uint32_t` | Secondary state/error code (written by state 4 on sign-in change) |
| `0x83192C50` | object ptr | Object passed to sub_8284B4B0 in the fast path |

## Control Flow

### sub_8223DAA0 (readiness check)

```
sub_8223DAA0():
    // Step 1: Reset readiness field to "no user"
    MEM[0x82BF3D90] = -1  (0xFFFFFFFF)

    // Step 2: Check if device manager is ready
    result = sub_826CD808()
    if (result == 0):
        return 0           // NOT READY — devices not enumerated

    // Step 3: Get current user index from device manager
    sub_826CD808()         // second call (refreshes internal state)
    user_index = sub_829DB9B8(result)   // reads field at +72 of object
    MEM[0x82BF3D90] = user_index

    // Step 4: Validate user_index
    if (user_index < 0):
        return 1           // READY — negative means "use default"

    // Step 5: Verify user is actually signed in
    signin_state = XamUserGetSigninState(user_index)   // via sub_82A12188
    if (signin_state == 0):
        MEM[0x82BF3D90] = -1   // reset — user not signed in
        return 0               // NOT READY

    return 1                   // READY — user is signed in
```

### sub_826CD808 (device manager query)

Called by sub_8223DAA0. Checks whether the storage device manager has finished enumerating devices and the user's sign-in state is consistent.

```
sub_826CD808():
    obj = 0x8318AAF8                          // device manager object
    cached_user = MEM[0x82A9172C]             // previously cached sign-in user index
    current_user = sub_829DB9B8(obj)          // read obj->field_72

    if (current_user == cached_user):
        goto check_ready                      // no change

    if (cached_user < 0):
        sub_826CD5A8()                        // reset/clear (no valid cached user)
        goto check_ready

    if (sub_829DB7F8(cached_user) == false):  // validate cached user is still valid
        sub_826CD5A8()                        // invalid, reset
        goto check_ready

    // Cached user changed — update profile
    sub_829DBBA8(&local_buf)                  // init buffer
    sub_829DBC30(cached_user, &local_buf)     // populate with user data
    sub_826CD660(&local_buf)                  // apply profile update

check_ready:
    ready = sub_822BF360(obj)                 // query: are devices enumerated?
    if (ready):
        return obj   // 0x8318AAF8 (nonzero = truthy)
    else:
        return 0     // devices not ready
```

**Key sub-functions of sub_826CD808:**
- `sub_829DB9B8(obj)`: Returns `MEM[obj + 72]` — the signed-in user index from the device manager object
- `sub_822BF360(obj)`: Returns bool — whether devices are fully enumerated and ready
- `sub_829DB7F8(user)`: Returns bool — whether a given user index is still valid
- `sub_826CD5A8()`: Resets/clears device state (called when no valid user)

### sub_8223DB20 (sign-in change notification handler)

Called by **state 4** of `sub_82242910`. Polls for XN_SYS_SIGNINCHANGED notifications and re-checks readiness if one is received.

```
sub_8223DB20():
    listener_handle = MEM[0x82BF3930]

    // Poll for sign-in change notification (id = 10 = XN_SYS_SIGNINCHANGED)
    has_notif = XNotifyGetNext(listener_handle, match_id=10, &id_out, &param_out)

    if (!has_notif):
        return 0                  // no notification

    // Sign-in changed — check if we had a valid readiness state
    readiness = MEM[0x82BF3D90]
    if (readiness == -1):
        return 0                  // was already "no user", ignore

    // Re-check readiness after sign-in change
    result = sub_8223DAA0()
    if (result == 0):
        return 0                  // not ready after re-check

    return 1                      // sign-in changed AND still ready => trigger re-init
```

## State Machine Context (sub_82242910)

The outer state machine at `0x82242910` uses a state variable at `0x82BF9848`. States 0-14 are valid.

### State 0 — Initial readiness gate

```
state_0:
    result = sub_8223DAA0()
    if (result != 0):
        // FAST PATH: device ready + user signed in
        user_index = MEM[0x82BF3D90]
        state = 4                           // skip states 1-3
        sub_8284B4B0(0x83192C50, user_index)  // init with user
        return 1
    else:
        // NORMAL PATH: not ready yet
        state = 1
        return 1
```

### State 4 — Monitoring for sign-in changes

```
state_4:
    result = sub_8223DB20()
    if (result != 0):
        // Sign-in changed — write error/restart code
        MEM[0x82A9546C] = 33
        return 2
    else:
        // No change — continue normal processing
        ... (further state 4 logic with save system init)
```

## Why sub_8223DAA0 Returns 1 (Fast Path) in Our Recomp

On Xbox 360, devices are enumerated asynchronously. When the game first boots:

1. `sub_826CD808` calls `sub_822BF360` to check if device enumeration is complete
2. On real hardware, this returns **false** for several frames while the HDD/memory unit is being discovered
3. So `sub_8223DAA0` returns **0**, sending the state machine to state 1
4. State 1 begins the normal initialization sequence (UI prompts, storage device selection, etc.)

In RexGlue's recomp:

1. **Devices are immediately available** — the VFS is mounted synchronously at startup
2. `sub_822BF360` returns **true** on the very first call
3. `sub_826CD808` returns the object pointer (nonzero)
4. `sub_829DB9B8` reads the user index from the object (+72), which is likely 0
5. `XamUserGetSigninState(0)` returns **1** (`SignedInLocally`) — our `UserProfile` initializes `signinState_` to `SignedInLocally` in `user_profile.cpp:31`
6. Since sign-in state != 0, `sub_8223DAA0` returns **1**

This causes the state machine to **skip states 1-3** entirely (the normal device selection and sign-in flow) and jump directly to state 4, which expects all initialization from states 1-3 to have already completed. This breaks the state progression.

## Recommended Fix Strategy

To force the **normal path** (state 0 -> state 1), `sub_8223DAA0` should be hooked to return **0** on the first call, simulating the Xbox 360 behavior where devices are not immediately ready. This gives the state machine time to run through states 1-3 and complete proper initialization.

Alternatively, the hook could track a frame counter or initialization flag:
- First N calls: return 0 (force normal path)
- After initialization completes: return the real result (so state 4's re-check via sub_8223DB20 -> sub_8223DAA0 works correctly)

The readiness field at `0x82BF3D90` must also be managed correctly since sub_8223DB20 reads it to decide whether a sign-in change notification is relevant.
