# sub_82240B08 -- Save Device Handle Validation

## CORRECTION: Not Async I/O

Agent 4 described this function as checking "async I/O completion via GetOverlappedResult."
**This is incorrect.** The function calls `sub_8284B3D8`, which calls `sub_82A12718`, which is
a direct jump to `__imp__XamContentGetDeviceData`. There is no OVERLAPPED, no GetOverlappedResult,
and no async I/O involved. The function validates that a save storage device handle exists and is
still connected.

---

## sub_82240B08 -- Full Analysis

**Address**: `0x82240B08`
**File**: `gta4_recomp.6.cpp:80288`
**Size**: 62 lines (0x82240B08 - 0x82240B68)

### Pseudocode

```c
bool sub_82240B08(void) {
    uint32_t saveSlot = *(uint32_t*)0x82BF3D90;
    bool valid = sub_8284B3D8(0x83192C50, saveSlot);  // validate device handle

    if (valid) {
        // Device handle is valid -- save system is ready
        *(uint8_t*)0x82BF3A77 = 1;   // g_sceneReady = true
        *(uint8_t*)0x82BF3CDA = 1;   // g_contentReady = true
        return 1;                     // READY
    } else {
        // Device handle invalid -- cleanup and report not ready
        sub_8223DB90(/*ctx*/);        // reset/cleanup save state
        return 0;                     // NOT READY
    }
}
```

### Arguments and Return Value

| Register | Usage |
|----------|-------|
| r3 (in) | Set internally to `0x83192C50` (save device struct base) |
| r4 (in) | Loaded from `0x82BF3D90` (current save slot index) |
| r3 (out) | 1 = device valid/ready, 0 = device invalid/not ready |

### Global Variables Accessed

| Address | Name | Access | Description |
|---------|------|--------|-------------|
| `0x82BF3D90` | g_saveSlotIndex | READ | Current save slot index, used to index into device array |
| `0x82BF3A77` | g_sceneReady | WRITE (=1) | Set to 1 when device handle is valid |
| `0x82BF3CDA` | g_contentReady | WRITE (=1) | Set to 1 when device handle is valid |

### Call Chain

```
sub_82240B08
  |-- sub_8284B3D8(0x83192C50, saveSlot)   // Validate device handle
  |     |-- Computes: array[0x83192C58 + slot * 160 + 56]  (file handle)
  |     |-- If handle != 0: calls sub_82A12718(handle, &localBuf)
  |     |     |-- sub_82A12718 = __imp__XamContentGetDeviceData
  |     |     |-- Returns 1 if XamContentGetDeviceData succeeds (non-zero)
  |     |-- If handle == 0: returns 0
  |-- [on success]: sets 0x82BF3A77=1, 0x82BF3CDA=1, returns 1
  |-- [on failure]: calls sub_8223DB90() to reset, returns 0
```

---

## The Notification Array at 0x83192C58

**Base**: `0x83192C58`
**Element stride**: 160 bytes (0xA0)
**Indexing formula**: `base + (slot * 5 * 32)` = `base + slot * 160`

This is a save device descriptor array. Each 160-byte element contains:

| Offset | Size | Description |
|--------|------|-------------|
| +0 | 4 | First field (zeroed by sub_8284B4B0) |
| +32 | 4 | Status/error code (checked by sub_8284B430) |
| +56 | 4 | File handle (checked by sub_8284B3D8, cleared by sub_8284B3B0) |

The `0x83192C50` value passed as r3 to sub_82240B08 is 8 bytes before the array base.
The struct likely has an 8-byte header at `0x83192C50` (possibly count + flags), followed
by the element array starting at `0x83192C58`.

### sub_8284B3D8 -- Validate Handle (inner function)

```c
bool sub_8284B3D8(void* structBase, uint32_t slot) {
    // Array base = 0x83192C58
    uint32_t handle = *(uint32_t*)(0x83192C58 + slot * 160 + 56);
    if (handle == 0) return 0;

    X_CONTENT_DEVICE_DATA deviceData;  // 0x50 bytes on stack
    uint32_t result = XamContentGetDeviceData(handle, &deviceData);
    if (result != 0) return 1;   // Success: device exists
    return 0;                     // Failure: device disconnected
}
```

**NOTE**: The return logic is inverted from typical Win32 convention:
- `XamContentGetDeviceData` returns 0 on **success** (ERROR_SUCCESS)
- `XamContentGetDeviceData` returns non-zero on **failure** (e.g., X_ERROR_DEVICE_NOT_CONNECTED)

Looking at the assembly more carefully:
- Line 12608: `cmplwi cr6,r3,0` -- compare result to 0
- Line 12610: `li r3,1` -- preload return value 1
- Line 12612: `beq cr6,0x8284b420` -- if result == 0, skip to loc_8284B420
- Line 12614-12616: `li r3,0` -- handle == 0 OR result != 0 both land here, return 0

So: returns 1 only if `XamContentGetDeviceData(handle, &buf) == 0` (success). Returns 0 if
handle is null or if `XamContentGetDeviceData` returns an error.

---

## sub_82240B78 -- Device Removal Notification Guard

**Address**: `0x82240B78`
**File**: `gta4_recomp.6.cpp:80354`
**Size**: ~90 lines (0x82240B78 - 0x82240C10)

### Pseudocode

```c
bool sub_82240B78(void) {
    uint32_t id = 0;
    uint32_t param;
    uint32_t notifyHandle = *(uint32_t*)0x82BF3930;

    // Check for device REMOVAL notification (type 11)
    bool gotNotify = XNotifyGetNext(notifyHandle, 11, &id, &param);

    if (!gotNotify)
        return 0;   // No removal notification pending

    // A removal notification was received -- check if we're signed in
    bool signedIn = sub_8223DAA0();
    if (!signedIn)
        return 0;   // Not signed in, ignore notification

    // Check if g_sceneReady is set
    if (*(uint8_t*)0x82BF3A77 == 0)
        return 0;   // Scene not ready, nothing to invalidate

    // Re-validate the device handle after removal notification
    bool stillValid = sub_82240B08();

    if (stillValid)
        return 0;   // Device survived removal check, no action needed

    // Device was actually removed! Flag error state
    *(uint8_t*)0x82BF3A76 = 1;    // g_deviceRemoved flag
    *(uint8_t*)0x82A95467 = 1;    // g_needsRecheck flag
    return 1;                      // Device removed -- caller should handle error
}
```

### What It Checks (Summary)

1. Polls `XNotifyGetNext` with `match_id = 11` (XN_SYS_STORAGEDEVICESCHANGED or similar)
2. If no notification: returns 0 (no change)
3. If notification received, verifies user is still signed in via `sub_8223DAA0`
4. Checks `g_sceneReady` (0x82BF3A77) -- only proceeds if save system was previously initialized
5. Calls `sub_82240B08` to re-validate the device handle
6. If device is now invalid: sets error flags and returns 1
7. If device still valid: returns 0 (false alarm)

### Global Variables

| Address | Name | Access | Description |
|---------|------|--------|-------------|
| `0x82BF3930` | g_notifyHandle | READ | XNotify listener handle |
| `0x82BF3A76` | g_deviceRemoved | WRITE (=1) | Set when device removal confirmed |
| `0x82BF3A77` | g_sceneReady | READ | Must be set for removal to matter |
| `0x82A95467` | g_needsRecheck | WRITE (=1) | Signals UI/system to re-prompt for device |

---

## sub_8223DB20 -- Device Change Notification Check

**Address**: `0x8223DB20`
**File**: `gta4_recomp.6.cpp:73432`
**Size**: 63 lines

### Pseudocode

```c
bool sub_8223DB20(void) {
    uint32_t id = 0;
    uint32_t param;
    uint32_t notifyHandle = *(uint32_t*)0x82BF3930;

    // Check for device CHANGE notification (type 10)
    bool gotNotify = XNotifyGetNext(notifyHandle, 10, &id, &param);

    if (!gotNotify)
        return 0;   // No change notification

    int32_t saveSlot = *(int32_t*)0x82BF3D90;
    if (saveSlot == -1)
        return 0;   // No save slot assigned

    bool signedIn = sub_8223DAA0();
    if (!signedIn)
        return 0;

    return 1;  // Device changed while we have an active save slot
}
```

Called at the **start** of state 4 in `sub_82242910` (line 85121). If it returns 1 (device change
detected), state 4 transitions to error 33 at `loc_82242B2C`. If it returns 0, execution continues
into the main state 4 logic.

---

## How State 4 Uses sub_82240B08

From `sub_82242910` at `loc_82242B1C` (case 4):

```
State 4 entry:
  1. Call sub_8223DB20()  -- check for device CHANGE notification (type 10)
     If returns 1 -> error 33, exit with return 2

  2. If no device change, zero out local result, load sub-state from 0x82BF9844

  3. Check sub-state (0-4 switch):
     - Sub-state 0,1: set result=0, fall through
     - Sub-state 2: set result=1, fall through
     - Sub-state 3: check flag at 0x82BF981F, if set AND 0x82A95466==0 -> set 0x82BF981E=1
     - Sub-state 4: check 0x82A95466, if 0 -> set 0x82BF981E=1

  4. If result == 1 (sub-state >= 2):
     -> error 34 at loc_82242C48, exit with return 2

  5. If result == 0:
     -> state = 5, exit with return 1 (continue to next state)

  EXCEPT: Before the sub-state check, if 0x82BF981E is already set:
     -> Skip directly to state 5

  AND: Right after checking 0x82BF981E, call sub_82240B08():
     -> If returns 1 (device valid): continue into sub-state switch
     -> If returns 0 (device invalid): state = 9, exit with return 1
```

### The Critical Path

```
State 3 -> calls sub_82240AB0() which:
  - Clears g_contentReady (0x82BF3CDA = 0)
  - Calls sub_8223DB90(0) to clear file handle
  - Calls sub_8284B4B0 to zero array element
  - Sets state = 4

State 4:
  - sub_8223DB20() polls for device change (type 10 notification)
  - sub_82240B08() tries to validate device handle
    - sub_8284B3D8 reads handle from array[slot*160 + 56]
    - BUT sub_82240AB0 just zeroed this handle via sub_8284B3B0!
    - So handle == 0, XamContentGetDeviceData is never called
    - sub_8284B3D8 returns 0
    - sub_82240B08 returns 0
    - State machine sets state = 9 (skip to later stage)
```

**IMPORTANT**: In the normal Xbox 360 flow, state 3 sets up async content creation, and state 4
polls for completion. `sub_82240AB0` is called **before** the async operation begins (clearing
old state), then the async content creation populates the handle. State 4's `sub_82240B08` then
finds the handle valid once the async operation completes.

In the recomp, the content creation stub likely never populates the file handle at
`array[slot*160 + 56]`, so `sub_82240B08` always returns 0, causing a perpetual transition
to state 9 instead of state 5.

---

## Does GetOverlappedResult Work?

**N/A** -- GetOverlappedResult is not involved in this function at all. The validation uses
`XamContentGetDeviceData`, which is fully implemented in the recomp kernel at
`glue/rexglue-sdk-main/src/kernel/xam/xam_content_device.cpp`.

The real question is: **Does the file handle at offset +56 ever get populated?**

The handle is populated by `sub_8284B4D0`, which is the content creation/mounting function
called from `sub_8284B560` (the async content create flow). If the content creation stubs
(XamContentCreateEx, XamContentCreate) never write a valid handle into the array, then
`sub_82240B08` will always return 0.

---

## Could We Hook sub_82240B08 to Return "Ready"?

**Yes, but with caveats.**

A simple hook that returns 1 (and sets the two flags) would:
- Set `0x82BF3A77 = 1` (g_sceneReady)
- Set `0x82BF3CDA = 1` (g_contentReady)
- Allow state 4 to proceed to state 5

```cpp
// Proposed hook:
PPC_FUNC(sub_82240B08) {
    PPC_STORE_U8(0x82BF3A77, 1);  // g_sceneReady
    PPC_STORE_U8(0x82BF3CDA, 1);  // g_contentReady
    ctx.r3.s64 = 1;               // return "ready"
}
```

**Risks**:
1. Later code may try to read/write using the handle stored at `array[slot*160+56]`.
   If that handle is 0/invalid, file I/O operations on the save device will crash or silently fail.
2. `sub_82240B78` (device removal guard) also calls `sub_82240B08` to re-validate after a
   removal notification. If we always return 1, we mask actual device-not-connected errors
   that the game might need to handle gracefully.
3. The safer approach is to ensure the content creation flow populates a valid handle, then
   let sub_82240B08 work naturally.

**Recommendation**: Hook sub_82240B08 as a short-term unblock, but also investigate why
the content creation path (sub_8284B4D0 / sub_8284B560) does not populate the handle.
The root cause is likely in XamContentCreateEx or XamContentCreate stubs not setting up
the save device correctly.

---

## Related Function Reference

| Function | Address | Purpose |
|----------|---------|---------|
| sub_82240B08 | 0x82240B08 | Validate save device handle, set ready flags |
| sub_82240B78 | 0x82240B78 | Poll XNotify type 11 (device removal), re-validate |
| sub_8223DB20 | 0x8223DB20 | Poll XNotify type 10 (device change), check save slot |
| sub_8223DAA0 | 0x8223DAA0 | Get signed-in user, acquire device/slot via sub_829DB9B8 |
| sub_8223DB90 | 0x8223DB90 | Reset save state: clear handle, clear flags |
| sub_82240AB0 | 0x82240AB0 | Pre-transition cleanup: clear content ready, reset handle |
| sub_8284B3D8 | 0x8284B3D8 | Read handle from array, call XamContentGetDeviceData |
| sub_8284B3B0 | 0x8284B3B0 | Clear file handle at array[slot*160 + 56] |
| sub_8284B4B0 | 0x8284B4B0 | Clear first field of array element |
| sub_8284B430 | 0x8284B430 | Read status from array[slot*160 + 32], map to result codes |
| sub_82A12718 | 0x82A12718 | Thunk to XamContentGetDeviceData |
| sub_82A12188 | 0x82A12188 | Thunk to XamUserGetSigninState |

---

## Notification Type Summary

| Type ID | XNotify Constant | Function | Usage |
|---------|-----------------|----------|-------|
| 10 | XN_SYS_STORAGEDEVICESCHANGED (?) | sub_8223DB20 | Device added/changed |
| 11 | XN_SYS_STORAGEDEVICESREMOVED (?) | sub_82240B78 | Device removed |
