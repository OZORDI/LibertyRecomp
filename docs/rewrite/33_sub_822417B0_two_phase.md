# sub_822417B0 -- Two-Phase Content Size Check

**Source**: `gta4_recomp.6.cpp` lines 82158-82373

## Function Signature

```
sub_822417B0(r3=playerIndex, r4=phase, r5=contentName, r6=sizeNeeded, r7=?, r8=outDeltaKB, r9=outResult)
```

- **r4=1**: Initiate phase (start the operation)
- **r4=0**: Poll phase (check if complete)

## Return Values

| Return (r3) | Meaning |
|-------------|---------|
| 0 | Success -- operation completed, check out-params |
| 1 | Completed (initiate path finished synchronously) |
| 2 | Error -- error code stored at `0x82A9546C` |

## Error Codes at 0x82A9546C

| Value | Stored When |
|-------|-------------|
| 33 | sub_8223DB20 returns nonzero (storage device changed notification) |
| 34 | sub_82240B78 returns nonzero (profile changed notification) |
| 10 | Initiate-path content operation (sub_8284AD78/sub_8284B010) fails |
| 6 | Set by caller in state machine before jumping to error exit |

---

## Phase 1: Initiate (r4=1)

**Entry**: Line 82192, when `r4 & 0xFF != 0`

### Step-by-step:

1. **Check storage device removal** -- calls `sub_8223DB20`
   - Calls `XNotifyGetNext(handle, notifID=10, ...)` -- notification 10 = storage device changed
   - If notification fired AND user index != -1: returns 1 (device changed)
   - If device changed detected: stores error code **33** at `0x82A9546C`, returns **2** (error)

2. **Check profile change** -- calls `sub_82240B78`
   - Calls `XNotifyGetNext(handle, notifID=11, ...)` -- notification 11 = profile changed
   - If profile changed: sets flags at `0x82BF3A76` and `0x82A95467`, returns 1
   - If profile changed detected: stores error code **34** at `0x82A9546C`, returns **2** (error)

3. **Initiate content operation** -- branches based on `r29` (original r3 = playerIndex):
   - If `r29 == 0`: calls **sub_8284AD78** (single-player content init, passes `r4=0` to sub_8284A7E8)
   - If `r29 != 0`: calls **sub_8284B010** (multi-player content init, passes `r4=1` to sub_8284A7E8)

4. **Check initiate result**:
   - If content init returns **nonzero** (success): jumps to `loc_8224190C`, returns **1**
   - If content init returns **0** (fail): stores error code **10** at `0x82A9546C`, returns **2** (error)

### sub_8284AD78 / sub_8284B010 -> sub_8284A7E8

These are thin wrappers that compute a slot pointer into the content array at `0x83192C58` and tail-call `sub_8284A7E8`.

**sub_8284A7E8** is the content slot initializer:
- Checks slot state == 0 (idle); if not, returns 0 (fail)
- Zeroes out 304 bytes of local XCONTENT_DATA structure
- Copies content name (up to 41 chars) into the structure
- Calls `sub_82A12720` (likely `NtQueryDirectoryFile` or `GetFileSize`) to get content directory size, stores as 64-bit at `slot+136`
- Initializes slot async I/O handle area (`slot+8` through `slot+32`) to zero
- Calls `sub_82A127A0` (async content open/create) with parameters:
  - r3 = user index (254 if slot >= 4)
  - r4 = content device ID
  - r5 = pointer to XCONTENT_DATA (on stack)
  - r6 = open disposition (3 = OPEN_EXISTING)
  - r7 = 0
  - r8 = 0
- `sub_82A127A0` calls `sub_82A12690` -- this is the underlying **NtCreateFile/XamContentCreateEx** wrapper
- If returns **997** (STATUS_PENDING): sets slot state to **17** (async pending, single) or **20** (async pending, multi)
- If returns immediately (not pending): sets slot state to **16** (ready, single) or **19** (ready, multi)
- Returns 1 (success)

**Does NOT call XamContentCreateEx directly** -- uses internal CRT wrappers (sub_82A127A0 -> sub_82A12690).

---

## Phase 2: Poll (r4=0)

**Entry**: Line 82285, `loc_82241884`, when `r4 & 0xFF == 0`

### Step-by-step:

1. **Compute slot pointer** -- same formula as initiate: `r31 = 0x83192C50`, `r30 = 0x82BF3D90`

2. **Call sub_8284ADA0** (poll the content slot) with `r3=0x83192C50`, `r4=userIndex`, `r5=outDeltaKB`:
   - Checks slot state at `[slot+0]`:
     - **State 16** (ready, no async): jumps to size comparison at `loc_8284AF48`
     - **State 17** (async pending): calls `sub_82A11EB8` (WaitForSingleObject / GetOverlappedResult)
       - Returns **996**: still pending -> return **0** (still working)
       - Returns **0**: I/O complete -> proceeds to open file, get size, compare
       - Returns other: error -> proceed to error handling
     - **Other state**: returns **0** (not ready)

3. **If sub_8284ADA0 returns nonzero** (poll complete):
   - Re-reads user index, re-computes slot
   - Calls **sub_8284AFE0** (if `r29==0`, single-player cleanup) or **sub_8284B2F8** (if `r29!=0`, multi-player cleanup)
   - Rechecks storage device notification (sub_8223DB20): if changed, stores error 33, returns 2
   - Rechecks profile notification (sub_82240B78): if changed, stores error 34, returns 2
   - If neither changed: returns **0** (success, done)

4. **If sub_8284ADA0 returns 0** (still working or initiate fail):
   - Jumps to `loc_8224190C`, returns **1** (done/completed -- but this is the "still working" path from the caller's perspective since state 14 will be re-entered)

### sub_8284ADA0 Size Comparison Logic

When the async I/O completes (state 17 -> done, or state 16 already ready):

1. Sets slot state to **21** (processing)
2. Calls `sub_8284A078` (finalize content -- reads actual size)
3. Loads `[slot+136]` = **expected size** (set during initiate)
4. Loads `[slot+144]` = **actual size** (set by CreateFileA/GetFileSize in poll)
5. Compares:
   - If `expected == actual`: writes 0 to `outDeltaKB`, returns 1
   - If `expected > actual`: calculates delta in KB = `(expected - actual + 1023) >> 10`, stores to `outDeltaKB`
   - Sets slot state to **18** (size comparison done)

---

## How States 12 and 14 Interpret the Return Value

### State 12 (loc_822430B4) -- Initiate

```
r3=0, r4=1  // playerIndex=0, phase=INITIATE
call sub_822417B0(...)
if (r3 == 2) goto loc_82242AC4  // ERROR -> return 2 from state machine
// else: set state = 14
```

**State 12 ALWAYS transitions to state 14** unless sub_822417B0 returns 2 (error).
It does NOT transition to state 13. State 12 cannot produce a state-13 transition.

### State 14 (loc_822430F0) -- Poll

```
r3=0, r4=0  // playerIndex=0, phase=POLL
call sub_822417B0(...)
if (r3 == 2) goto loc_82242AC4      // ERROR -> return 2
if (r3 != 0) goto loc_82243250      // STILL WORKING -> return 1 (re-enter state 14 next tick)
// r3 == 0: poll COMPLETE
r11 = PPC_LOAD_U32(r29 + 0)         // r29 = 0x82BF99C8 (delta KB result)
if (r11 < 0) goto loc_822431C8      // NEGATIVE delta -> set state=13 (error)
// else: r11 >= 0 -> check save flags, possibly done
```

**The transition to state 13 happens in state 14**, NOT state 12:
- sub_822417B0 returns 0 (poll complete)
- The value at `0x82BF99C8` is checked
- If `[0x82BF99C8] < 0`: **state -> 13** (insufficient storage / size mismatch error)
- If `[0x82BF99C8] >= 0`: proceeds with save completion logic

### State 13 (loc_822431DC) -- Error Recovery

State 13 is a storage-error handler:
1. Re-checks notifications (sub_8223DB20, sub_82240B78)
2. Calls `sub_8223F9F0` with `r3=4, r4=-[0x82BF99C8]` (negated delta = KB needed)
   - This likely shows a "not enough storage space" UI prompt
3. If user confirms: clears flags, calls sub_8223DB90, resets state to 0 -> return 1
4. If user cancels: returns 1 (done, go to loc_82243250)

---

## Root Cause of State 12 -> 13 Failure

**State 12 does NOT directly transition to state 13.** The flow is:

```
State 11 -> State 12 (initiate) -> State 14 (poll) -> State 13 (if size delta < 0)
```

The failure path is:
1. State 12 calls sub_822417B0 with r4=1 (initiate)
2. sub_822417B0 calls sub_8284AD78 -> sub_8284A7E8 -> sub_82A127A0 (async content open)
3. If async succeeds: returns 1, state transitions to 14
4. State 14 calls sub_822417B0 with r4=0 (poll)
5. sub_8284ADA0 polls the slot, gets the actual content size
6. If `[slot+136] (expected) != [slot+144] (actual)` and `expected > actual`:
   - Delta stored at `[0x82BF99C8]` could be negative if the arithmetic overflows or the comparison logic inverts
7. State 14 checks `[0x82BF99C8] < 0` -> transitions to state 13

**Most likely failure cause**: The content directory doesn't exist or has size 0, making `actual_size = 0` while `expected_size > 0`, producing a positive delta (needing more KB). However, if `sub_82A127A0` or `sub_82A12720` fails entirely (returns -1), the stored size at `[slot+144]` could be 0 or garbage, and the delta computation at `[0x82BF99C8]` could go negative.

Alternatively: If `sub_82A127A0` returns an error (not 997 and not 0), the slot enters error cleanup and the delta stored via `outDeltaKB` (r5 param = the stack address) could be negative.

---

## Key Functions Called

| Address | Name | Purpose |
|---------|------|---------|
| sub_8223DB20 | CheckStorageDeviceRemoval | XNotifyGetNext(id=10), returns 1 if device changed |
| sub_82240B78 | CheckProfileChanged | XNotifyGetNext(id=11), returns 1 if profile changed |
| sub_8284AD78 | ContentInitSingle | Wrapper: computes slot, calls sub_8284A7E8 with r4=0 |
| sub_8284B010 | ContentInitMulti | Wrapper: computes slot, calls sub_8284A7E8 with r4=1 |
| sub_8284A7E8 | ContentSlotInit | Sets up content slot, calls sub_82A127A0 (async open), stores expected size |
| sub_82A127A0 | AsyncContentOpen | Calls sub_82A12690 (NtCreateFile wrapper), returns 997=pending |
| sub_8284ADA0 | ContentSlotPoll | Polls slot state (16=ready, 17=pending), compares sizes, returns delta |
| sub_8284AFE0 | ContentCleanupSingle | Cleanup after poll completes (single-player) |
| sub_8284B2F8 | ContentCleanupMulti | Cleanup after poll completes (multi-player) |
| sub_8284A078 | ContentFinalize | Finalizes content, reads actual size |

## Does sub_822417B0 call sub_8284ADA0?

**YES** -- in the poll path (r4=0), line 82304: `sub_8284ADA0(ctx, base)`.

## Does sub_822417B0 call XamContentCreateEx?

**NO** -- it does not call XamContentCreateEx (0x82A74C34) directly. It uses internal CRT wrappers:
- sub_82A127A0 -> sub_82A12690 (which wraps NtCreateFile-style operations)
- sub_82A131B0 = rexcrt_CreateFileA (called within sub_8284ADA0 to open the save file for size check)

## Key Memory Addresses

| Address | Description |
|---------|-------------|
| 0x82A9546C | Error code store (33=device changed, 34=profile changed, 10=content fail) |
| 0x83192C50 | Content data structure base |
| 0x83192C58 | Content slot array base |
| 0x82BF3D90 | Active user index |
| 0x82BF3930 | XNotify listener handle |
| 0x82BF9848 | State machine state variable |
| 0x82BF99C8 | Out-result: storage delta in KB (passed as r29 in state machine, loaded after poll) |
| 0x82BF99CC | Out-param: r6 to sub_822417B0 (size needed) |
