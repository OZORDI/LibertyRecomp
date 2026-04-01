# State 12 Failure Analysis — sub_82242910

## Summary

State 12 is the "content verification" state. It calls `sub_822417B0` twice:
once with r4=1 (initiate content creation) and once with r4=0 (poll for
completion and verify file sizes). The state transitions to 13 (error recovery /
restart) when the content file's on-disk size does not match the expected size,
causing `*(0x82BF99C8)` to go negative.

---

## Key Memory Addresses

All addresses computed with Python:

| Address        | Type   | Description                                           |
|----------------|--------|-------------------------------------------------------|
| `0x82BF9848`   | u32    | STATE_VAR (switch variable for the state machine)     |
| `0x82BF99C8`   | s32    | "size delta" output from sub_822417B0 / sub_8284ADA0  |
| `0x82BF99CC`   | u32    | Device data / content metadata index                  |
| `0x82BF3A60`   | struct | Content info structure (r28 in outer function)        |
| `0x82A95466`   | u8     | Flag byte (checked after successful verification)     |
| `0x82BF981F`   | u8     | Online/multiplayer related flag                       |
| `0x82BF9844`   | u32    | Another state variable (compared to 3)                |
| `0x82A9546C`   | u32    | Error code storage (set to 6, 33, 34, 10 on errors)  |
| `0x82BF3CDA`   | u8     | Byte cleared on restart                               |

---

## State 12 — Detailed Flow

### Step 1: First call to sub_822417B0 (r4=1 = "initiate")

```
sub_822417B0(r3=0, r4=1, r5=0x82BF3A60, r6=load(0x82BF99CC),
             r7=0x82BF99C8, r8=&stack84, r9=&stack88)
```

With r4=1, sub_822417B0 takes the "initiate" path (line 82192):
1. Calls `sub_8223DB20` — checks `XNotifyGetNext(notification=10)` for
   storage device changes. If nonzero, returns 2 (error code 33).
2. Calls `sub_82240B78` — checks `XNotifyGetNext(notification=11)` for
   profile/signin changes. If nonzero, returns 2 (error code 34).
3. If both pass, calls `sub_8284AD78` (since r3=0):
   - This is a thin wrapper that computes a slot in the content array at
     `0x83192C58` based on the device index, then tail-calls `sub_8284A7E8`.
   - `sub_8284A7E8` calls `XamContentCreateEx` (via sub_82A12690) with mode=3
     (CREATE_ALWAYS).
   - If XamContentCreateEx returns 997 (`ERROR_IO_PENDING`), sets slot
     state=17 (needs completion polling).
   - If it returns immediately, sets slot state=16 (ready) or 19/20.

**Return values from sub_822417B0:**
- `0` = success, operation complete
- `1` = still pending (slot not yet in final state)
- `2` = error (notification received or XamContentCreateEx failed)

**After the first call:**
- If returns 2 -> goto `loc_82242AC4` (returns 2 from outer function, no
  state transition — game exits this state machine).
- If returns != 2 -> **sets STATE_VAR = 14**, then falls through to state 14
  code (loc_822430F0).

### Step 2: State 14 code (falls through from state 12, or entered directly)

```
sub_822417B0(r3=0, r4=0, r5=0x82BF3A60, r6=load(0x82BF99CC),
             r7=0x82BF99C8, r8=&stack88, r9=&stack84)
```

With r4=0, sub_822417B0 takes the "poll" path (line 82285 / loc_82241884):
1. Calls `sub_8284ADA0` — the content completion/verification function:
   - Reads slot state from the content array.
   - If state == 16 (ready from XamContentCreateEx not pending): jumps to
     size comparison (loc_8284AF48).
   - If state == 17 (pending): calls `sub_82A11EB8` to poll the overlapped
     I/O result. `sub_82A11EB8` checks if the OVERLAPPED at slot+0 holds
     status 997 (pending); if still pending, calls `sub_82A13040`
     (WaitForSingleObject on the OVERLAPPED event handle, timeout=-1 =
     infinite). Returns 996 if still waiting, 0 if complete, or error code.
   - After I/O completes: opens the content file via `CreateFileA`, gets file
     size via `sub_82A135B0` (GetFileSizeEx wrapper), then calls
     `XamContentClose` (sub_82A126F8).
   - **Compares `slot[136]` (expected size) vs `slot[144]` (actual disk size).**
   - If `expected <= actual`: stores 0 to `*(0x82BF99C8)` -> success.
   - If `expected > actual`: computes `(expected - actual)` rounded in 1KB
     units, stores as a **negative value** to `*(0x82BF99C8)`.

2. Returns to sub_822417B0, which then re-checks sub_8223DB20 + sub_82240B78.
   Returns 0 (success) or 1 (still pending).

**After the second call (back in sub_82242910 state 14 logic):**
- If returns 2 -> goto loc_82242AC4 (return 2, exit)
- If returns nonzero but != 2 (i.e., 1 = pending) -> goto loc_82243250
  (return 1 from outer, try again next tick)
- If returns 0 (complete) -> **checks `*(0x82BF99C8)`**

### Step 3: The Critical Check

```c
int32_t size_delta = *(int32_t*)0x82BF99C8;
if (size_delta < 0) {
    // STATE = 13 (error recovery / restart)
    // This is the path we are hitting!
} else {
    // Success path: check various flags, return
}
```

**The negative value means: the content file on disk is SMALLER than what the
game expects.** The absolute value represents the shortfall in ~1KB units.

---

## Why State 12 Fails

The root cause is in `sub_8284ADA0`:

1. `sub_8284A7E8` called `XamContentCreateEx(mode=3=CREATE_ALWAYS)` which
   succeeded and set `slot[136]` = expected content size (from
   `XamContentGetDeviceData`).

2. `sub_8284ADA0` then opens the actual file via `CreateFileA` and reads its
   size into `slot[144]`.

3. The comparison `slot[136]` (expected) vs `slot[144]` (actual disk size)
   fails because either:
   - **The file does not exist** (size = 0 or CreateFileA returned -1,
     leading to slot[144] = 0).
   - **The file is smaller** than what the device reports as available/expected.

In the recomp, the most likely scenario is that `slot[136]` was set to a
non-zero expected size (from `XamContentGetDeviceData` / `sub_82A12720`),
but `slot[144]` is 0 because either:
- `CreateFileA` returns INVALID_HANDLE_VALUE (-1) for the content file path
  (the path is derived from the content root + file name constructed at
  slot+64).
- `GetFileSizeEx` returns 0 because the file does not exist on the virtual
  filesystem.

---

## State 13 — Error Recovery / Restart

State 13 (loc_822431DC) implements a "not enough storage space" dialog flow:

1. Checks `sub_8223DB20` (XNotifyGetNext notification 10) — if storage
   device changed, returns 2 with error code 33.
2. Checks `sub_82240B78` (XNotifyGetNext notification 11) — if signin
   changed, returns 2 with error code 34.
3. Calls `sub_8223F9F0(type=4, waitMs=neg(*(0x82BF99C8)), &result)`:
   - Type 4 = "insufficient storage" dialog (shows a message box telling
     the user to free space).
   - The wait time = negate(the negative delta) = positive number of ~1KB units.
   - `sub_8223F9F0` is the dialog system: calls `sub_82254FE0` to show UI
     (with a string from the string table at a computed address), then polls
     via `sub_8224FFC8`.
   - Returns 0 if the dialog is still showing / waiting.
   - Returns nonzero when the user responds.

4. If `sub_8223F9F0` returns 0: stays in state 13 (return 1, poll next tick).
5. If `sub_8223F9F0` returns nonzero:
   - If result byte == 0: user declined -> goto loc_82242AB8, sets error
     code 6, returns 2 (exit state machine permanently).
   - If result byte != 0: user accepted / retried ->
     - Clears `0x82A95466` = 0
     - Clears `0x82BF3CDA` = 0
     - Calls `sub_8223DB90` (notification cleanup)
     - **Sets STATE_VAR = 0** (full restart of state machine)
     - Returns 1

### Why State 13 Always Restarts (in the recomp)

In the recomp, `sub_8223F9F0(type=4)` likely returns immediately with a
nonzero result and result byte != 0 (since the dialog system probably has no
real UI and auto-accepts). This causes an immediate restart to state 0.

The cycle then repeats:
```
0 -> 1 -> 3 -> 4 -> 9 -> 10 -> 11 -> 12 -> (size check fails) -> 13 -> 0 -> ...
```

---

## Conditions for State 12 to Succeed (transition past state 14)

State 12 succeeds when `*(0x82BF99C8) >= 0` after the second sub_822417B0
call. This requires `sub_8284ADA0` to find that the actual file size >=
expected file size:

```
slot[136] <= slot[144]   =>   *(0x82BF99C8) = 0   =>   success
```

After success, the code checks:
1. `*(0x82A95466)` (flag byte): if nonzero, takes loc_82243188 path.
2. `*(0x82BF981F)` (byte): if zero, returns 2 (done, via loc_82242AC4).
3. `*(0x82BF9844)` (word): if == 3, calls sub_8223F790, then sub_8223CAD8.
4. Returns 0 (state machine complete for this frame).

Note: the STATE_VAR is NOT explicitly set on success — it remains at 14
(set at line 85954). The next tick enters case 14 directly (loc_822430F0)
and re-polls, eventually returning 0 (complete).

---

## Xbox-Specific Functions That May Fail in Recomp

| Function             | Called Via       | Risk in Recomp                                |
|----------------------|-----------------|-----------------------------------------------|
| `XamContentCreateEx` | sub_82A12690    | Returns ERROR_SUCCESS but may set wrong sizes  |
| `XamContentClose`    | sub_82A126F8    | Appears to work (returns 997 = IO_PENDING)     |
| `XamContentGetDeviceData` | sub_82A12720 | Sets slot[136] expected size — may be wrong   |
| `CreateFileA`        | rexcrt          | May fail if content root path not mapped       |
| `GetFileSizeEx`      | sub_82A135B0    | Returns 0 if file not found                   |
| `XNotifyGetNext`     | sub_8223DB20    | Notification system — may not fire correctly   |
| `sub_82A11EB8`       | overlapped poll | Waits on OVERLAPPED hEvent — may hang or fail |

---

## Root Cause Hypothesis

The most likely root cause is in `sub_82A12720` (`XamContentGetDeviceData`).
This function is called during `sub_8284ADA0` to compute the expected
content size that gets stored in `slot[136]`. If it returns a non-zero
expected size but the actual save file is empty or missing, the size
comparison fails.

Alternatively, the issue may be that `CreateFileA` in `sub_8284ADA0` is
trying to open a file within the content root (the path at slot+64), but
the content root was never properly mapped by `XamContentCreateEx` in the
first call, so the file open fails and the size defaults to 0.

### Recommended Investigation

1. Add logging to `XamContentCreateEx` to print the root path it creates.
2. Add logging to `sub_82A12720` (XamContentGetDeviceData) to see what
   device size it reports.
3. Check if `CreateFileA` in `sub_8284ADA0` is opening a valid path.
4. Check what `slot[136]` and `slot[144]` are set to after the calls.
5. Consider hooking sub_822417B0 or sub_8284ADA0 to force
   `*(0x82BF99C8) = 0` as a temporary bypass.
