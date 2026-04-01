# sub_82242910 States 5-14 — Helper Functions Reference

**Source**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.6.cpp` lines 85299-86159

This document covers states 5 through 14 of the save/scene state machine in sub_82242910.
States 0-4 are documented in `01_sub_82242910_states.md`.

## Key Globals (refresher)

| Symbol | Address | Type | Description |
|--------|---------|------|-------------|
| STATE_VAR | `0x82BF9848` | u32 | Current state (0-14) |
| platformMode | `0x82BF9844` | u32 | Platform/scene mode selector |
| saveSlot | `0x82BE3D90` | u32 | Current save slot index |
| errorCode | `0x82A9546C` | u32 | Error code output |
| g_needDeviceReselect | `0x82A95466` | u8 | Online/network flag |
| g_saveDirty | `0x82BF981E` | u8 | Save dirty / restart-pending flag |
| g_hasValidSave | `0x82BF981F` | u8 | Whether a valid save exists |
| g_saveInitDone | `0x82BE3A76` | u8 | Save system initialization complete |
| g_saveEnabled | `0x82BE3A77` | u8 | Save system enabled |
| g_saveInProgress | `0x82BE3CDA` | u8 | Save operation in progress |
| g_needDeviceCheck | `0x82A95467` | u8 | Need device recheck |
| g_playerCount | `0x82BF99CC` | u32 | Player count value |
| g_saveProgressData | `0x82BE3D98` | struct | Save progress tracking data |
| g_saveProgressCurrent | `0x82BE3D94` | u32 | Current save progress value |
| saveDeviceStruct | `0x82C92C50` | struct | Save device structure (r31 alias) |
| saveDeviceArray | `0x82C92C58` | array | Save device element array (160B stride) |
| g_notifyHandle | `0x82BE3930` | handle | XNotify listener handle |

## Return Convention

- r3=0: Finished (terminal)
- r3=1: In progress (keep calling)
- r3=2: Error (errorCode written)

## Shared Exit Labels

| Label | Address | Action |
|-------|---------|--------|
| `loc_82242B2C` | `0x82242B2C` | Set errorCode=33, return r3=2 (sign-in notification error) |
| `loc_82242C48` | `0x82242C48` | Set errorCode=34, return r3=2 (device-removal notification error) |
| `loc_82242AB8` | `0x82242AB8` | Set errorCode=6, then fall to loc_82242AC4 |
| `loc_82242AC4` | `0x82242AC4` | Return r3=2 (error) |
| `loc_82243250` | `0x82243250` | Return r3=1 (in progress, no-op) |

---

## State 5 — Check Save Dirty + Show Storage UI

**Label**: `loc_82242C88` (line 85323)

**Calls**:
1. `sub_8223DB20()` — XNotify sign-in check
2. `sub_8223F9F0(r3=2, r4=0, r5=&outBool)` — show storage device selector UI

**Logic**:
1. Call `sub_8223DB20()`. If nonzero (sign-in changed): goto error 33.
2. Check `g_saveDirty` (0x82BF981E). If set:
   - Write 1 to stack bool, clear g_saveDirty.
3. Else: Call `sub_8223F9F0(2, 0, &outBool)` to show device picker.
   - If returns 0: goto loc_82243250 (stay in progress).
4. If either path set outBool=1:
   - Clear `g_saveInitDone` (0x82BE3A76) to 0.
   - Compute next state: if outBool was from g_saveDirty, next=6; else next=7.
   - Set STATE_VAR = 6 or 7, return r3=1.

**Transitions**: 5 -> 6 (device was re-selected) or 5 -> 7 (device newly selected)

---

## State 6 — Begin Save Create Operation

**Label**: `loc_82242D18` (line 85402)

**Calls**:
1. `sub_8223DB20()` — sign-in check
2. `sub_826CBA70()` — check streaming/loading complete
3. `sub_8284AAE0(saveDeviceStruct, saveSlot, ?, playerCount, ?, g_needDeviceReselect)` — begin save create

**Logic**:
1. Call `sub_8223DB20()`. If sign-in changed: error 33.
2. Call `sub_826CBA70()`. If nonzero (still loading): goto loc_82243250 (wait).
3. Set up params and call `sub_8284AAE0(...)`:
   - r3 = saveDeviceStruct (0x82C92C50)
   - r4 = saveSlot, r5 = 1, r6 = playerCount, r7 = 1, r8 = g_needDeviceReselect
4. If returns 0 (failed):
   - Set errorCode=7, return r3=2.
5. If returns nonzero (success):
   - Set STATE_VAR = 8, return r3=1.

**Transitions**: 6 -> 8 (success), 6 -> error 7

---

## State 7 — Check Save Status After Device Selection

**Label**: `loc_82242D9C` (line 85475)

**Calls**:
1. `sub_8223DB20()` — sign-in check
2. `sub_8223CB60()` — check platformMode validity
3. `sub_8223F9F0(r3=3, r4=0, r5=&outBool)` — show content check UI

**Logic**:
1. Call `sub_8223DB20()`. If sign-in changed: error 33.
2. Call `sub_8223CB60()`. This reads platformMode (0x82BF9844) and returns:
   - 0 for modes 0,2 (no storage needed)
   - 1 for modes 1,3,4 (storage needed)
3. If `sub_8223CB60()` returned 1: Call `sub_8223F9F0(3, 0, &outBool)`.
   - If returns 0: goto loc_82243250 (wait).
4. Check outBool AND sub_8223CB60 result. If both true:
   - Set STATE_VAR = 6, return r3=1 (proceed to create).
5. Else: set errorCode=6, goto error.

**Transitions**: 7 -> 6 (proceed to create save) or 7 -> error

---

## State 8 — Wait for Save Create + Check Async Result

**Label**: `loc_82242E08` (line 85535)

**Calls**:
1. `sub_8284AB10(saveDeviceStruct, saveSlot)` — check if async op ready
2. `sub_8284AB70(saveDeviceStruct, saveSlot)` — reset state if completed (state==3 -> 0)
3. `sub_8223DB20()` — sign-in check
4. `sub_8284B490(saveDeviceStruct, saveSlot)` — get async operation state
5. `sub_8284B430(saveDeviceStruct, saveSlot)` — get async operation result/error code
6. `sub_82240B08()` — check save device file handle valid

**Logic**:
1. Call `sub_8284AB10()`. If returns 0: goto loc_82243250 (not ready, wait).
2. Call `sub_8284AB70()` — if device element state==3, reset to 0 (clear completed marker).
3. Call `sub_8223DB20()`. If sign-in changed: error 33.
4. Call `sub_8284B490()` — get operation state. If state==1 (success):
   - Call `sub_8284B430()` to get result code.
   - If result==2: STATE_VAR=7, return r3=1 (retry device selection).
   - Else: set errorCode=8, return r3=2.
5. If state!=1: Call `sub_8284B430()` again.
   - If result==0: fall through to sub_82240B08 check.
   - Else: set errorCode=9, return r3=2.
6. Call `sub_82240B08()` — validates save device file handle.
   - Returns 1 if handle valid (saves enabled flag set), 0 otherwise.
   - If returns 0: STATE_VAR=0 (restart from beginning); if returns 1: STATE_VAR=9.

**Transitions**: 8 -> 7 (retry), 8 -> 9 (handle valid), 8 -> 0 (restart), error 8 or 9

---

## State 9 — Initialize Progress Data + Start Save Write

**Label**: `loc_82242EE4` (line 85664)

**Calls**:
1. `sub_8223DB20()` — sign-in check
2. `sub_82240B78()` — device-removal notification check
3. `sub_8223D2F0()` — initialize save progress data arrays (zero-fill)
4. `sub_8284ABA0(saveDeviceStruct, saveSlot, saveProgressData, maxEntries=75, flag=1)` — start save write

**Logic**:
1. Call `sub_8223DB20()`. If sign-in changed: error 33.
2. Call `sub_82240B78()`. If device removed: error 34.
3. Call `sub_8223D2F0()` — zeros out the save progress data structure at 0x82BE3D98:
   - Clears 75 progress entry slots (308 bytes each = 23,100 bytes)
   - Clears 450 status bytes at 0x82BE3B28
   - Clears 15 priority bytes at 0x82BE3B18
4. Call `sub_8284ABA0(saveDeviceStruct, saveSlot, progressData, progressCurrent, maxEntries=75, flag=1)`.
   - If returns 0 (failed): set errorCode=14, return r3=2.
   - If returns nonzero: STATE_VAR=10, return r3=1.

**Transitions**: 9 -> 10 (write started), error 14 or 33 or 34

---

## State 10 — Wait for Save Write Progress

**Label**: `loc_82242F50` (line 85725)

**Calls**:
1. `sub_8284ABD0(saveDeviceStruct, saveSlot, &progressCurrent)` — check write progress
2. `sub_8284ABF8(saveDeviceStruct, saveSlot)` — reset state if completed (state==5 -> 0)
3. `sub_8223DB20()` — sign-in check
4. `sub_82240B78()` — device-removal check
5. `sub_8284B490(saveDeviceStruct, saveSlot)` — get operation state
6. `sub_8284B430(saveDeviceStruct, saveSlot)` — get result code

**Logic**:
1. Set r29=0 (progressDone flag).
2. Call `sub_8284ABD0()`. If returns 0: goto loc_82243010 (not ready, check done flag).
3. Call `sub_8284ABF8()` — if state==5, reset to 0.
4. Call `sub_8223DB20()`. If sign-in changed: error 33.
5. Call `sub_82240B78()`. If device removed: error 34.
6. Call `sub_8284B490()` — get state.
   - If state==1 (error): set errorCode=15, return r3=2.
7. Call `sub_8284B430()` — get result.
   - If result!=0 AND result!=5: set errorCode=16, return r3=2.
   - If result==0 or result==5: set r29=1 (done).
8. Check r29 (progressDone):
   - If 0: goto loc_82243250 (wait).
   - If 1: Read platformMode (0x82BF9844). Based on value:
     - 0, 1, 3, 4: STATE_VAR=11, return r3=1.
     - Other: goto loc_82243178 (call sub_8223CAD8 cleanup, return r3=0).

**Transitions**: 10 -> 11 (for modes 0,1,3,4), 10 -> finished (other modes), error 15 or 16

---

## State 11 — Verify Save + Check Platform-Specific Paths

**Label**: `loc_82243058` (line 85872)

**Calls**:
1. `sub_8223DB20()` — sign-in check
2. `sub_82240B78()` — device-removal check
3. `sub_8223D400()` — verify saved data (compare save names with progress entries)

**Logic**:
1. Call `sub_8223DB20()`. If sign-in changed: error 33.
2. Call `sub_82240B78()`. If device removed: error 34.
3. Call `sub_8223D400()`:
   - This function formats a save name using `sub_82A00108` (sprintf) with a counter.
   - Iterates through 75 progress entries (308B stride) comparing entry name (at offset +264) with formatted name.
   - Returns nonzero (1) if verification passes.
4. If returns 0: goto loc_82242AC4 (return r3=2, generic error).
5. If returns nonzero:
   - Read platformMode (0x82BF9844).
   - If mode==3 or mode==4: STATE_VAR=12, return r3=1.
   - Else: goto loc_82243178 (cleanup, return r3=0).

**Transitions**: 11 -> 12 (for modes 3,4), 11 -> finished (for modes 0,1)

---

## State 12 — Start Save Overwrite/Delete Operation

**Label**: `loc_822430B4` (line 85924)

**Calls**:
1. `sub_822417B0(r3=0, r4=1, r5=saveProgressData, r6=playerCount, r7=r29, r8=&out1, r9=&out2)` — save write helper (with sign-in + device checks)

**Logic**:
1. Zero out two local u32s (out1, out2 on stack).
2. Call `sub_822417B0(0, 1, saveProgressData, playerCount, r29, &out1, &out2)`:
   - This function internally calls `sub_8223DB20()` and `sub_82240B78()` for error checks.
   - Returns 0 for success/continue, 2 for error.
3. If returns 2: goto loc_82242AC4 (error).
4. Else: STATE_VAR=14, fall through to state 14.

**Transitions**: 12 -> 14 (continue), 12 -> error

---

## State 13 — Handle Negative Save Slot

**Label**: `loc_822431DC` (line 86089)

**Calls**:
1. `sub_8223DB20()` — sign-in check
2. `sub_82240B78()` — device-removal check
3. `sub_8223F9F0(r3=4, r4=-playerCount, r5=&outBool)` — show overwrite/delete confirmation UI

**Logic**:
1. Call `sub_8223DB20()`. If sign-in changed: error 33.
2. Call `sub_82240B78()`. If device removed: error 34.
3. Load playerCount from 0x82BF99C8, negate it.
4. Call `sub_8223F9F0(4, -playerCount, &outBool)` — likely a "delete save?" confirmation dialog.
   - If returns 0: goto loc_82243250 (wait for user response).
5. If outBool nonzero (user confirmed):
   - Clear g_needDeviceReselect (0x82A95466) to 0.
   - Clear g_saveInProgress (0x82BE3CDA) to 0.
   - Call `sub_8223DB90(0)` — finalize/close save device.
   - Set STATE_VAR=0 (restart from beginning).
6. Else (user cancelled): goto loc_82242AB8 (errorCode=6).

**Transitions**: 13 -> 0 (restart after delete confirmation), 13 -> error 6

---

## State 14 — Poll Save Write Completion

**Label**: `loc_822430F0` (line 85956)

**Calls**:
1. `sub_822417B0(r3=0, r4=0, r5=saveProgressData, r6=playerCount, r7=r29, r8=&out2, r9=&out1)` — poll save completion
2. `sub_8223F790()` — check if device-specific save path needs handling
3. `sub_8223CAD8()` — cleanup / finalize save session

**Logic**:
1. Zero out two local u32s.
2. Call `sub_822417B0(0, 0, saveProgressData, playerCount, r29, &out2, &out1)`:
   - r4=0 (vs r4=1 in state 12) means "poll" instead of "initiate".
   - Returns 2 for error, 0 for complete, other for in-progress.
3. If returns 2: goto loc_82242AC4 (error).
4. If returns != 0: goto loc_82243250 (wait).
5. If returns 0 (complete):
   - Read `out1` (r29). If < 0: STATE_VAR=13, return r3=1 (negative slot means deletion needed).
   - Check g_needDeviceReselect (0x82A95466):
     - If nonzero (online mode):
       - Check platformMode (0x82BF9844).
       - If mode==3 AND g_hasValidSave (0x82BF981F) is set:
         - Call `sub_8223F790()` — checks g_needDeviceCheck and returns based on save progress entry status.
         - If returns 0: call `sub_8223CAD8()` (cleanup).
       - Else: call `sub_8223CAD8()` (cleanup).
       - Return r3=0 (finished).
     - If zero (offline mode):
       - Check g_hasValidSave (0x82BF981F). If not set: goto loc_82242AC4 (error).
       - Check platformMode. If mode==3 AND g_hasValidSave:
         - Call `sub_8223F790()`. If returns 0: call `sub_8223CAD8()`.
       - Else: call `sub_8223CAD8()`.
       - Return r3=0 (finished).

**Transitions**: 14 -> 13 (negative slot, deletion), 14 -> 0 (finished, r3=0), 14 -> error

---

## Helper Function Details

### sub_8223DB20 — XNotify Sign-In Check
**Address**: `0x8223DB20` | **File**: gta4_recomp.6.cpp:73432 | **Size**: ~60 lines

**What it does**: Calls `XNotifyGetNext(g_notifyHandle, 10, &id, &param)` to check for Xbox LIVE sign-in state change notifications. If a notification is received and saveSlot != -1, calls `sub_8223DAA0()` to re-validate save device. Returns 1 if sign-in changed (error condition), 0 otherwise.

**Xbox dependency**: Calls `__imp__XNotifyGetNext` (Xbox notification system). In recomp, this import is hooked and should return "no notification" (0). **Should work as-is** since the hook returns 0, making the function always return 0.

**Essential?**: Called as a guard in every state 4+. Must exist but effectively a no-op in recomp.

---

### sub_8223F9F0 — Show Storage Device Selector / Confirmation UI
**Address**: `0x8223F9F0` | **File**: gta4_recomp.6.cpp:77883 | **Size**: ~200+ lines

**What it does**: Complex UI state machine that shows different Xbox guide overlays:
- r3=0: Initial storage device selector
- r3=1: Storage device for save data
- r3=2: Re-select storage device (state 5)
- r3=3: Content check (state 7)
- r3=4: Delete save confirmation (state 13)

Reads various flags including `g_needDeviceReselect` (0x82A95466), checks an "interrupted" flag at 0x82A9547C. Loads XNKID/XUID from 0x82A90000+21628. Writes result bool to the pointer in r5.

**Xbox dependency**: **Heavy** - uses Xbox guide UI calls, XNKID/XUID structures. This is the primary blocker for save states 5, 7, 13. Needs a replacement that immediately returns success.

**Essential?**: Yes, but must be stubbed/replaced to always indicate "device selected" on PC.

---

### sub_826CBA70 — Check Streaming/Loading Complete
**Address**: `0x826CBA70` | **File**: gta4_recomp.39.cpp:66615 | **Size**: 10 lines (tiny)

**What it does**: Tail-calls `sub_829EEA58()` with a pointer to a streaming structure at 0x82C80000-1992 = 0x82C7F838. The inner function `sub_829EEA58` reads byte at offset +56 of the struct and extracts bit 7 (returns 0 or 1). Returns nonzero if streaming is still in progress, 0 if complete.

**Xbox dependency**: None. Pure memory read.

**Essential?**: Yes. Used as a guard in state 6 to wait for streaming to finish.

---

### sub_8284AAE0 — Begin Save Create (Indexed)
**Address**: `0x8284AAE0` | **File**: gta4_recomp.55.cpp:11216 | **Size**: 28 lines (wrapper)

**What it does**: Index-wrapper around `sub_8284A0F8`. Computes element address in save device array: `base + (idx*5) << 5` (idx*160 byte stride). Passes through r5,r6,r7,r8 as r4,r5,r6,r7 to the inner function.

The inner `sub_8284A0F8` sets up an async save container creation:
- Checks device state (must be 0 or 1)
- Zeros out the async operation struct (offsets 8-31)
- Calls XCONTENT functions to create save container

**Xbox dependency**: Inner function calls XContent APIs (Xbox save container system). **Will need stubbing** - save container creation must be faked.

**Essential?**: Yes for save flow, but needs replacement/stub.

---

### sub_8284AB10 — Check Async Op Ready (Indexed)
**Address**: `0x8284AB10` | **File**: gta4_recomp.55.cpp:11247 | **Size**: 18 lines (wrapper)

**What it does**: Index-wrapper around `sub_82849C18`. Computes element address, then calls inner function which checks if async IO operation has completed. Reads device state at offset +0 of element; if state==2, calls `sub_82A11EB8` (likely GetOverlappedResult). Returns 1 if operation complete, 0 if still pending.

**Xbox dependency**: Uses async IO / OVERLAPPED checking. Should work in recomp if the save container creation is properly stubbed to set completion immediately.

**Essential?**: Yes for async flow.

---

### sub_8284AB70 — Reset Completed State (Indexed)
**Address**: `0x8284AB70` | **File**: gta4_recomp.55.cpp:11312 | **Size**: 25 lines (wrapper)

**What it does**: Reads the first u32 of the indexed save device element. If value==3 (completed), sets it to 0 (idle). Otherwise does nothing.

**Xbox dependency**: None. Pure memory read/write.

**Essential?**: Yes, simple state reset.

---

### sub_8284B490 — Get Async Operation State (Indexed)
**Address**: `0x8284B490` | **File**: gta4_recomp.55.cpp:12686 | **Size**: 16 lines (wrapper)

**What it does**: Reads the first u32 of the indexed save device element and returns it. This is the async operation state: 0=idle, 1=error, 2=pending, 3=create-complete, 5=write-complete.

**Xbox dependency**: None. Pure memory read.

**Essential?**: Yes. Used to poll async result.

---

### sub_8284B430 — Get Async Operation Result Code (Indexed)
**Address**: `0x8284B430` | **File**: gta4_recomp.55.cpp:12630 | **Size**: 52 lines

**What it does**: Reads the u32 at offset +32 of the indexed save device element (the HRESULT / error code from the async operation). Returns:
- 2 if code == 1223 (ERROR_CANCELLED)
- 5 if code == 0x80070012 (ERROR_NO_MORE_ITEMS)
- 3 if code == 0x80070012 + 1374
- 0 otherwise (unknown/unhandled)

**Xbox dependency**: None. Interprets Xbox error codes but is pure logic.

**Essential?**: Yes. Determines how to handle async results.

---

### sub_82240B08 — Validate Save Device File Handle
**Address**: `0x82240B08` | **File**: gta4_recomp.6.cpp:80288 | **Size**: 64 lines

**What it does**: Calls `sub_8284B3D8(saveDeviceStruct, saveSlot)` which reads the file handle at offset +56 of the indexed save device element and calls `sub_82A12718` (likely GetFileInformationByHandle). If handle is valid:
- Sets g_saveEnabled (0x82BE3A77) = 1
- Sets g_saveInProgress (0x82BE3CDA) = 1
- Returns 1
If handle invalid:
- Calls `sub_8223DB90(0)` to close/cleanup
- Returns 0

**Xbox dependency**: Uses file handle validation. Should work if save creation stub produces a valid handle.

**Essential?**: Yes. Gate between state 8 and 9.

---

### sub_82240B78 — Device-Removal Notification Check
**Address**: `0x82240B78` | **File**: gta4_recomp.6.cpp:80354 | **Size**: ~60 lines

**What it does**: Calls `XNotifyGetNext(g_notifyHandle, 11, &id, &param)` to check for storage device removal notifications. If a device was removed, calls `sub_8223DAA0()` and `sub_82240B08()` to re-validate. Returns 1 if device was removed (error), 0 otherwise.

**Xbox dependency**: Uses `__imp__XNotifyGetNext` with notification type 11. Same as sub_8223DB20 but for device removal. **Should work** since XNotifyGetNext hook returns 0.

**Essential?**: Called as guard in states 9, 10, 11, 13. No-op in recomp.

---

### sub_8223CB60 — Check Platform Mode Validity
**Address**: `0x8223CB60` | **File**: gta4_recomp.6.cpp:71077 | **Size**: 50 lines

**What it does**: Reads platformMode (0x82BF9844). Returns based on mode value:
- Mode 0, 2: returns 0 (no storage device needed)
- Mode 1, 3, 4: returns 1 (storage device needed)
- Mode > 4: returns 1 (default)

**Xbox dependency**: None. Pure logic.

**Essential?**: Yes. Determines save flow branching.

---

### sub_8223D2F0 — Initialize Save Progress Data
**Address**: `0x8223D2F0` | **File**: gta4_recomp.6.cpp:72245 | **Size**: ~80 lines

**What it does**: Zero-fills the save progress data structure:
1. Loop: 75 iterations (stride 308 bytes) at 0x82BE3D98+4, clearing 10 bytes per entry (u32+u32+u16+u8 at offset +264)
2. Loop: 450 bytes at 0x82BE3B28, set to 0
3. Loop: 15 bytes at 0x82BE3B18, set to 0
4. Loop: 15 bytes at another offset, set to 0

**Xbox dependency**: None. Pure memory zeroing.

**Essential?**: Yes. Initializes progress tracking before save write.

---

### sub_8284ABA0 — Start Save Write (Indexed)
**Address**: `0x8284ABA0` | **File**: gta4_recomp.55.cpp:11340 | **Size**: 26 lines (wrapper)

**What it does**: Index-wrapper around `sub_8284A1E8`. Computes element address in save device array and tail-calls inner function with args: (element, progressData, buffer, maxEntries).

The inner function likely initiates the actual save data write operation to the save container.

**Xbox dependency**: Inner function uses XContent write APIs. **Needs stubbing/replacement**.

**Essential?**: Yes for save functionality.

---

### sub_8284ABD0 — Check Save Write Progress (Indexed)
**Address**: `0x8284ABD0` | **File**: gta4_recomp.55.cpp:11369 | **Size**: 22 lines (wrapper)

**What it does**: Index-wrapper around `sub_82849C98`. Computes element address and tail-calls inner function with (element, progressCurrent). Checks if save write has made progress.

**Xbox dependency**: Inner function checks async IO. Similar to sub_8284AB10 but for write operations.

**Essential?**: Yes. Polls write progress in state 10.

---

### sub_8284ABF8 — Reset Write-Complete State (Indexed)
**Address**: `0x8284ABF8` | **File**: gta4_recomp.55.cpp:11394 | **Size**: ~22 lines

**What it does**: Same pattern as sub_8284AB70 but checks for state==5 (write-complete) instead of state==3 (create-complete). If state==5, resets to 0.

**Xbox dependency**: None. Pure memory read/write.

**Essential?**: Yes, simple state cleanup.

---

### sub_822417B0 — Save Write Helper (with Guards)
**Address**: `0x822417B0` | **File**: gta4_recomp.6.cpp:82160 | **Size**: ~150 lines

**What it does**: Orchestrates a save write with built-in error checking:
1. If r4!=0 (initiate mode): calls sub_8223DB20 sign-in check. If fails: errorCode=33, return 2.
2. Calls sub_82240B78 device-removal check. If fails: errorCode=34, return 2.
3. Performs the actual save write/read operation through save device element functions.
4. Returns: 0 = complete, 2 = error.

Used in state 12 (initiate, r4=1) and state 14 (poll, r4=0).

**Xbox dependency**: Uses the same async save infrastructure. Needs save system stub.

**Essential?**: Yes. Core save write orchestration.

---

### sub_8223F790 — Check Device-Specific Save Path
**Address**: `0x8223F790` | **File**: gta4_recomp.6.cpp:77529 | **Size**: 62 lines

**What it does**:
1. Checks g_needDeviceCheck (0x82A95467). If 0: return 0.
2. Reads a mode value from 0x82B40000-27388 (= 0x82B394F4):
   - mode 1: index = 13
   - mode 2: index = 14
   - default: index = 12
3. Computes offset: index * 308 (save progress entry stride).
4. Reads a status byte at (progressData + offset + 264) - signed extend.
5. Returns 1 if the byte is nonzero (save path exists), 0 otherwise.

**Xbox dependency**: None. Pure data inspection.

**Essential?**: Yes. Determines if cleanup is needed in state 14.

---

### sub_8223CAD8 — Cleanup / Finalize Save Session
**Address**: `0x8223CAD8` | **File**: gta4_recomp.6.cpp:70996 | **Size**: ~80 lines

**What it does**:
1. Checks a flag at 0x82BF9854 (interruption flag). If not set: return.
2. Clears the flag.
3. Checks another flag at 0x82BFA164. If nonzero:
   - Calls `sub_8224EFE8(2)` — likely "show interrupted save dialog"
4. If zero:
   - Calls `sub_8224E620(0)` — likely "dismiss save dialog"
   - Checks value at 0x82BFA13C: if == 33 or == 14, calls `sub_8224EFE8(r3)`.

**Xbox dependency**: Calls UI/dialog functions. May need stubbing if they reference Xbox guide.

**Essential?**: Called at terminal states. Could be stubbed to no-op.

---

### sub_8223DB90 — Finalize / Close Save Device
**Address**: `0x8223DB90` | **File**: gta4_recomp.6.cpp:73499 | **Size**: ~50 lines

**What it does**:
1. Calls `sub_8284B3B0(saveDeviceStruct, saveSlot)` — likely closes save container.
2. Checks g_saveInProgress (0x82BE3CDA):
   - If set: stores r31 (argument) to g_saveInitDone (0x82BE3A76).
   - If not set: clears g_saveInitDone to 0.
3. Clears g_saveEnabled (0x82BE3A77) to 0.
4. Sets g_needDeviceCheck (0x82A95467) to 1.

Used when save operation finishes or needs to be aborted.

**Xbox dependency**: Inner close function may use XContent. Needs stub.

**Essential?**: Yes. Cleanup function used in multiple paths.

---

### sub_8223D400 — Verify Saved Data Names
**Address**: `0x8223D400` | **File**: gta4_recomp.6.cpp:72404 | **Size**: ~200 lines

**What it does**:
1. Checks g_saveProgressCurrent (0x82BE3D94) > 75: if so, set errorCode=42, return 0.
2. Loops through progress entries (308B stride):
   - Formats a name string via `sub_82A00108` (sprintf).
   - Compares formatted name with entry's name field at offset +264 (byte-by-byte).
   - If name matches: marks entry as valid.
3. Returns 1 if all entries verified, 0 on mismatch.

**Xbox dependency**: None. Pure string comparison / data verification.

**Essential?**: Yes. Validates save integrity in state 11.

---

## State Transition Diagram

```
State 4 (from doc 01)
  |
  v
State 5 (check dirty / show storage UI)
  |-- (device re-selected) --> State 6
  |-- (device newly selected) --> State 7

State 6 (begin save create)
  |-- (success) --> State 8
  |-- (error) --> errorCode 7

State 7 (check save status after device)
  |-- (proceed) --> State 6
  |-- (error) --> errorCode 6

State 8 (wait for async create)
  |-- (create complete, handle valid) --> State 9
  |-- (create complete, handle invalid) --> State 0 (restart)
  |-- (cancelled by user) --> State 7 (retry)
  |-- (error) --> errorCode 8 or 9

State 9 (init progress + start write)
  |-- (write started) --> State 10
  |-- (error) --> errorCode 14

State 10 (wait for write progress)
  |-- (write done, modes 0/1/3/4) --> State 11
  |-- (write done, other modes) --> Finished (r3=0)
  |-- (error) --> errorCode 15 or 16

State 11 (verify saved data)
  |-- (verified, modes 3/4) --> State 12
  |-- (verified, modes 0/1) --> Finished (r3=0)
  |-- (error) --> r3=2

State 12 (start save overwrite)
  |-- (success) --> State 14
  |-- (error) --> r3=2

State 14 (poll save write completion)
  |-- (complete, slot >= 0, online) --> Finished (r3=0)
  |-- (complete, slot >= 0, offline) --> Finished (r3=0)
  |-- (complete, slot < 0) --> State 13
  |-- (error) --> r3=2

State 13 (delete save confirmation)
  |-- (user confirms) --> State 0 (restart)
  |-- (user cancels) --> errorCode 6
```

## Recomp Feasibility Summary

| Function | Works in Recomp? | Action Needed |
|----------|-----------------|---------------|
| sub_8223DB20 | Yes (no-op) | XNotifyGetNext hook returns 0 |
| sub_82240B78 | Yes (no-op) | XNotifyGetNext hook returns 0 |
| sub_8223CB60 | Yes | Pure logic |
| sub_826CBA70 | Yes | Pure memory read |
| sub_8223D2F0 | Yes | Pure memory zeroing |
| sub_8223D400 | Yes | Pure string comparison |
| sub_8284B490 | Yes | Pure memory read |
| sub_8284B430 | Yes | Pure logic |
| sub_8284AB70 | Yes | Pure memory write |
| sub_8284ABF8 | Yes | Pure memory write |
| sub_8223F790 | Yes | Pure data inspection |
| sub_8223F9F0 | **NO** | Xbox guide UI - must be stubbed to return "selected" |
| sub_8284AAE0 | **NO** | XContent create - must be stubbed |
| sub_8284AB10 | Partial | Depends on async IO being properly faked |
| sub_8284ABA0 | **NO** | XContent write - must be stubbed |
| sub_8284ABD0 | Partial | Depends on async IO being properly faked |
| sub_82240B08 | Partial | Depends on file handle from stubbed create |
| sub_822417B0 | Partial | Depends on save infrastructure stubs |
| sub_8223CAD8 | Partial | Calls UI functions that may need stubbing |
| sub_8223DB90 | Partial | Calls XContent close - needs stub |

### Critical Path for Save System

The minimum stubs needed to make the save state machine work:
1. **sub_8223F9F0**: Return immediately with outBool=1 (device selected)
2. **sub_8284A0F8** (via sub_8284AAE0): Set device element state to 3 (complete) and result to 0 (success), set file handle to a valid host handle
3. **sub_8284A1E8** (via sub_8284ABA0): Set device element state to 5 (write-complete) and result to 0 (success)
4. **sub_8284B3B0** (via sub_8223DB90): No-op close
5. **sub_82849C18** (via sub_8284AB10): Return 1 (ready) immediately
6. **sub_82849C98** (via sub_8284ABD0): Return 1 (ready) immediately
