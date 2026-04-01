# States 9-14: Game Logic vs Xbox-Specific Adaptation Analysis

**Source**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.6.cpp` lines 85664-86159
**Cross-references**: docs 01, 10, 17

---

## Address Reference (Python-verified)

| Symbol | Address | Source |
|--------|---------|--------|
| STATE_VAR | `0x82BF9848` | r26(0x82C00000) + (-26552) |
| platformMode | `0x82BF9844` | r26(0x82C00000) + (-26556) |
| errorCode | `0x82A9546C` | 0x82A90000 + 21612 |
| g_needDeviceReselect | `0x82A95466` | 0x82A90000 + 21606 |
| g_hasValidSave | `0x82BF981F` | r26(0x82C00000) + (-26593) |
| g_saveInProgress | `0x82BF3CDA` | r30(0x82BF0000) + 15578 |
| g_playerCount | `0x82BF99CC` | r27(0x82C00000) + (-26164) |
| g_stateSubstruct | `0x82BF99C8` | r26(0x82C00000) + (-26168) |
| saveSlot | `0x82BF3D90` | r30(0x82BF0000) + 15760 |
| g_saveProgressCurrent | `0x82BF3D94` | r30(0x82BF0000) + 15764 |
| g_saveProgressData | `0x82BF3D98` | r30(0x82BF0000) + 15768 |
| saveDeviceStruct | `0x83192C50` | r31 (lis(-31975)+11344) |
| SCENE_STRUCT (r28) | `0x82BF3A60` | r30(0x82BF0000) + 14944 |

---

## State 9 -- Initialize Progress Data + Start Save Write

**Label**: `loc_82242EE4` (line 85664)

### Game Logic (PRESERVE)
1. Guard: call `sub_8223DB20()` -- if sign-in changed, error 33.
2. Guard: call `sub_82240B78()` -- if device removed, error 34.
3. Call `sub_8223D2F0()` -- zero-fill save progress data structure (75 entries x 308 bytes at `0x82BF3D98`, plus 450 status bytes at `0x82BE3B28`, plus 15 priority bytes at `0x82BE3B18`). **Pure memory zeroing, no Xbox dependency.**
4. Call `sub_8284ABA0(saveDeviceStruct, saveSlot, 1, g_saveProgressData, 75)` -- start save write operation.
5. On success: STATE_VAR = 10, return r3=1.
6. On failure: errorCode = 14, return r3=2.

### Sub-function Classification

| Function | Type | Notes |
|----------|------|-------|
| `sub_8223DB20` | Xbox (no-op in recomp) | XNotifyGetNext for sign-in. Hook returns 0, always passes. |
| `sub_82240B78` | Xbox (no-op in recomp) | XNotifyGetNext for device removal. Hook returns 0, always passes. |
| `sub_8223D2F0` | Game logic | Pure memory zeroing. Works as recompiled. |
| `sub_8284ABA0` | **Xbox-specific** | Wrapper around `sub_8284A1E8` which initiates async XContent write. Must be stubbed to set save device element state=5 (write-complete) and result=0. |

### Memory Writes
- Zero-fills 23,100+ bytes of save progress data via `sub_8223D2F0`.
- STATE_VAR = 10 (on success) at `0x82BF9848`.
- errorCode = 14 (on failure) at `0x82A9546C`.

### Proposed Hook Approach
- `sub_8223D2F0`: Let run as recompiled code (pure zeroing).
- `sub_8284ABA0` (or inner `sub_8284A1E8`): Hook to immediately set device element state to 5 (write-complete) with result code 0 (success). This makes state 10 see an already-completed write.

---

## State 10 -- Wait for Save Write Progress

**Label**: `loc_82242F50` (line 85725)

### Game Logic (PRESERVE)
1. Set r29=0 (progressDone flag).
2. Call `sub_8284ABD0(saveDeviceStruct, saveSlot, &g_saveProgressCurrent)` -- poll write progress.
   - Returns 0: jump to done-flag check (step 8).
   - Returns nonzero: continue to step 3.
3. Call `sub_8284ABF8(saveDeviceStruct, saveSlot)` -- if element state==5, reset to 0.
4. Guard: `sub_8223DB20()` -- error 33.
5. Guard: `sub_82240B78()` -- error 34.
6. Call `sub_8284B490(saveDeviceStruct, saveSlot)` -- get operation state.
   - If state == 1: errorCode = 15, return r3=2.
7. Call `sub_8284B430(saveDeviceStruct, saveSlot)` -- get result code.
   - If result != 0 AND result != 5: errorCode = 16, return r3=2.
   - If result == 0 or result == 5: set r29=1 (done).
   - If first call returns != 0, calls `sub_8284B430` a second time. If second call returns 5: r29=1.
8. Check r29:
   - If 0: return r3=1 (wait).
   - If 1: read platformMode.

### platformMode Check (EXACT values accepted)

| platformMode | Action |
|-------------|--------|
| **0** | STATE_VAR = 11, return r3=1 |
| **1** | STATE_VAR = 11, return r3=1 |
| **2** | Call `sub_8223CAD8()` (cleanup), return r3=0 (FINISHED) |
| **3** | STATE_VAR = 11, return r3=1 |
| **4** | STATE_VAR = 11, return r3=1 |
| **5+** | Call `sub_8223CAD8()` (cleanup), return r3=0 (FINISHED) |

The check is implemented as four sequential `cmpwi` instructions testing 0, 1, 3, 4 -- any match branches to `loc_82243044` which sets STATE_VAR=11. The final `cmpwi cr6,r11,4; bne cr6,loc_82243178` sends all other values to cleanup.

### Sub-function Classification

| Function | Type | Notes |
|----------|------|-------|
| `sub_8284ABD0` | **Xbox-specific** | Wrapper around `sub_82849C98`, polls async I/O completion. Must be stubbed. |
| `sub_8284ABF8` | Game logic | Pure memory: if element[0]==5, set to 0. Works as recompiled. |
| `sub_8223DB20` | Xbox (no-op) | Sign-in check. Hook returns 0. |
| `sub_82240B78` | Xbox (no-op) | Device-removal check. Hook returns 0. |
| `sub_8284B490` | Game logic | Pure memory read: returns element[0] (raw status). |
| `sub_8284B430` | Game logic | Pure logic: maps HRESULT values to result codes (0/2/3/5). |
| `sub_8223CAD8` | Game logic (partial) | Cleanup/finalize. Calls dialog functions that may need stubbing. |

### Memory Writes
- STATE_VAR = 11 (for modes 0,1,3,4) at `0x82BF9848`.
- errorCode = 15 or 16 (on failure) at `0x82A9546C`.

### Proposed Hook Approach
- `sub_8284ABD0` (or inner `sub_82849C98`): Hook to return 1 (ready) immediately. If the state 9 stub already set element state to 5 with result 0, sub_8284ABF8 will reset state to 0, sub_8284B490 returns 0 (idle, not 1), sub_8284B430 returns 0 (success) -- all game logic passes cleanly.

---

## State 11 -- Verify Save + Check Platform-Specific Paths

**Label**: `loc_82243058` (line 85872)

### Game Logic (PRESERVE)
1. Guard: `sub_8223DB20()` -- error 33.
2. Guard: `sub_82240B78()` -- error 34.
3. Call `sub_8223D400()` -- verify saved data by comparing formatted save names with progress entry name fields (offset +264 in 308-byte entries). Uses sprintf via `sub_82A00108`.
   - Returns 0: goto `loc_82242AC4`, return r3=2 (generic error).
   - Returns nonzero: continue.
4. Read platformMode.

### platformMode Check (EXACT values accepted)

| platformMode | Action |
|-------------|--------|
| **0** | Call `sub_8223CAD8()` (cleanup), return r3=0 (FINISHED) |
| **1** | Call `sub_8223CAD8()` (cleanup), return r3=0 (FINISHED) |
| **2** | Call `sub_8223CAD8()` (cleanup), return r3=0 (FINISHED) |
| **3** | STATE_VAR = 12, return r3=1 |
| **4** | STATE_VAR = 12, return r3=1 |
| **5+** | Call `sub_8223CAD8()` (cleanup), return r3=0 (FINISHED) |

Only modes 3 and 4 (multiplayer) continue to state 12. All other modes complete here.

### Sub-function Classification

| Function | Type | Notes |
|----------|------|-------|
| `sub_8223DB20` | Xbox (no-op) | Sign-in check. Always passes. |
| `sub_82240B78` | Xbox (no-op) | Device-removal check. Always passes. |
| `sub_8223D400` | Game logic | Pure string comparison / data verification. Works as recompiled. |
| `sub_8223CAD8` | Game logic (partial) | Cleanup, may call dialog functions. |

### Memory Writes
- STATE_VAR = 12 (for modes 3,4) at `0x82BF9848`.

### Proposed Hook Approach
- No hooks needed in state 11 itself. All functions either work as recompiled or are already hooked (XNotifyGetNext).
- For single-player (modes 0,1): this is the terminal state -- save flow completes here with r3=0.

---

## State 12 -- Start Save Overwrite/Delete (Multiplayer Phase 1)

**Label**: `loc_822430B4` (line 85924)

### Game Logic (PRESERVE)
1. Initialize two local u32s on stack: local_84=0, local_88=0.
2. Call `sub_822417B0(r3=0, r4=1, r5=SCENE_STRUCT, r6=g_playerCount, r7=r29, r8=&local_84, r9=&local_88)`.
   - **r4=1 = initiate mode** (first phase of two-phase pattern).
   - r3=0 determines which inner functions are called (r29==0 branch in sub_822417B0).
3. If returns 2: goto `loc_82242AC4` (error, return r3=2).
4. Otherwise: STATE_VAR = 14, fall through to state 14.

### sub_822417B0 Two-Phase Pattern (CRITICAL)

**Phase 1 (state 12, r4=1 "initiate"):**
1. `sub_8223DB20()` sign-in check -> error 33 on fail.
2. `sub_82240B78()` device-removal check -> error 34 on fail.
3. Since r29(=r3 arg)=0:
   - Calls `sub_8284AD78(saveDeviceStruct, saveSlot, SCENE_STRUCT, g_playerCount)` -- initiates save overwrite/delete operation.
   - If returns 0: errorCode=10, return 2.
   - If returns nonzero: return 1 (in progress).

**Note**: State 12 does NOT check the return value for 1 vs 0 distinction -- it only checks for 2 (error). If sub_822417B0 returns 1 (in progress, the normal success path), state 12 still sets STATE_VAR=14 and falls through. This is correct because the intent is "initiate then immediately start polling."

### Sub-function Classification

| Function | Type | Notes |
|----------|------|-------|
| `sub_822417B0` | Mixed | Orchestrator with Xbox guards + save device operations. |
| `sub_8284AD78` (called via sub_822417B0) | **Xbox-specific** | Save overwrite/delete initiation via XContent. Must be stubbed. |

### Memory Writes
- local_84 = 0, local_88 = 0 (stack, via sub_822417B0 zeroing r8/r9 targets).
- STATE_VAR = 14 at `0x82BF9848`.
- Possible: errorCode = 10, 33, or 34 (via sub_822417B0 error paths).

### Proposed Hook Approach
- `sub_8284AD78`: Hook to immediately set device element state to completed (state=3) with result=0. This allows sub_822417B0's Phase 2 (state 14) to find a completed operation immediately.

---

## State 13 -- Error Recovery / Retry After Negative Slot

**Label**: `loc_822431DC` (line 86089)

### How State 13 Is Entered
State 14 transitions here when `[g_stateSubstruct]` (value at `0x82BF99C8`) is **negative** (< 0). A negative value indicates the save operation returned an error slot index, signaling that a save deletion or re-selection is needed.

### Game Logic (PRESERVE)
1. Guard: `sub_8223DB20()` -- error 33.
2. Guard: `sub_82240B78()` -- error 34.
3. Load value from `[g_stateSubstruct]` (`0x82BF99C8`), negate it.
4. Call `sub_8223F9F0(r3=4, r4=-[g_stateSubstruct], r5=&outBool)` -- shows delete/overwrite confirmation UI.
   - If returns 0: return r3=1 (wait for user response).
5. Check outBool (stack offset 80):
   - If 0 (user cancelled): errorCode=6, return r3=2.
   - If nonzero (user confirmed):
     - Clear `g_needDeviceReselect` (0x82A95466) = 0.
     - Clear `g_saveInProgress` (0x82BF3CDA) = 0.
     - Call `sub_8223DB90(r3=0)` -- finalize/close save device.
     - STATE_VAR = 0 (full restart from beginning).
     - Return r3=1.

### How It Restarts
State 13 performs a **full restart**: it clears the online/device flags, closes the save device via `sub_8223DB90`, and resets STATE_VAR to 0. The next call to sub_82242910 will re-enter at state 0, re-running the entire initialization and device selection flow from scratch.

### Sub-function Classification

| Function | Type | Notes |
|----------|------|-------|
| `sub_8223DB20` | Xbox (no-op) | Sign-in check. Always passes. |
| `sub_82240B78` | Xbox (no-op) | Device-removal check. Always passes. |
| `sub_8223F9F0` | **Xbox-specific** | Xbox guide UI for delete confirmation. Must be stubbed. |
| `sub_8223DB90` | **Xbox-specific** | Calls `sub_8284B3B0` (XContent close). Must be stubbed. |

### Memory Writes
- `g_needDeviceReselect` (0x82A95466) = 0 (on confirm).
- `g_saveInProgress` (0x82BF3CDA) = 0 (on confirm).
- STATE_VAR = 0 (on confirm) at `0x82BF9848`.
- errorCode = 6 (on cancel) at `0x82A9546C`.

### Proposed Hook Approach
- `sub_8223F9F0` (mode 4): Hook to immediately return 1 with outBool=1 (auto-confirm delete). On PC there is no physical storage device to select; the save system should transparently handle overwrites.
- `sub_8223DB90`: Hook to perform platform-agnostic save file close (or no-op if save stubs don't create real files yet).

---

## State 14 -- Poll Save Write Completion (Multiplayer Phase 2)

**Label**: `loc_822430F0` (line 85956)

### Game Logic (PRESERVE)
1. Initialize local_88=0, local_84=0 on stack.
2. Call `sub_822417B0(r3=0, r4=0, r5=SCENE_STRUCT, r6=g_playerCount, r7=r29, r8=&local_88, r9=&local_84)`.
   - **r4=0 = poll mode** (second phase of two-phase pattern).
   - **Note**: r8 and r9 are SWAPPED vs state 12 (state 12: r8=&local_84, r9=&local_88; state 14: r8=&local_88, r9=&local_84).
3. If returns 2: goto `loc_82242AC4` (error, return r3=2).
4. If returns != 0 (and != 2): goto `loc_82243250` (return r3=1, wait).
5. If returns 0 (complete):
   - Load `[g_stateSubstruct]` (value at `0x82BF99C8`).
   - If < 0: STATE_VAR = 13, return r3=1 (error recovery).
   - If >= 0: check `g_needDeviceReselect` (0x82A95466).

### Completion Paths (when [g_stateSubstruct] >= 0)

**Path A: g_needDeviceReselect == 0 (offline mode):**
1. Check `g_hasValidSave` (0x82BF981F).
   - If 0: goto `loc_82242AC4` (error, return r3=2).
2. Check platformMode:
   - If platformMode == 3 AND g_hasValidSave != 0:
     - Call `sub_8223F790()`. If returns 0: call `sub_8223CAD8()`.
     - If returns nonzero: skip `sub_8223CAD8()`.
   - If platformMode != 3: call `sub_8223CAD8()`.
3. Return r3=0 (FINISHED).

**Path B: g_needDeviceReselect != 0 (online mode):**
1. Check platformMode:
   - If platformMode == 3 AND `g_hasValidSave` != 0:
     - Call `sub_8223F790()`. If returns 0: call `sub_8223CAD8()`.
     - If returns nonzero: skip `sub_8223CAD8()`.
   - If platformMode != 3 OR g_hasValidSave == 0: call `sub_8223CAD8()`.
2. Return r3=0 (FINISHED).

Both paths converge on returning r3=0 (finished). The key difference is that offline mode requires g_hasValidSave to be set, while online mode always succeeds.

### sub_822417B0 Two-Phase Pattern (Phase 2)

**Phase 2 (state 14, r4=0 "poll"):**
1. Skips sign-in and device-removal checks (jumps to `loc_82241884`).
2. Sets r31 = saveDeviceStruct (0x83192C50), r30 = &saveSlot.
3. Since r29(=r3 arg)=0:
   - Calls `sub_8284ADA0(saveDeviceStruct, saveSlot)` -- poll async completion.
   - If returns 0: return 1 (still busy).
   - If returns nonzero (ready):
     - Calls `sub_8284AFE0(saveDeviceStruct, saveSlot)` -- advance/finalize.
     - Then calls `sub_8223DB20()` sign-in check -> error 33.
     - Then calls `sub_82240B78()` device-removal check -> error 34.
     - Returns 0 (complete).

### Sub-function Classification

| Function | Type | Notes |
|----------|------|-------|
| `sub_822417B0` | Mixed | Orchestrator. Phase 2 polls and advances. |
| `sub_8284ADA0` (via sub_822417B0) | **Xbox-specific** | Polls async save completion. Must be stubbed. |
| `sub_8284AFE0` (via sub_822417B0) | **Xbox-specific** | Advances/finalizes save write. Must be stubbed. |
| `sub_8223F790` | Game logic | Pure data inspection: checks g_needDeviceCheck + progress entry status byte. |
| `sub_8223CAD8` | Game logic (partial) | Cleanup/finalize. May call dialog functions. |

### Memory Writes
- STATE_VAR = 13 (if [g_stateSubstruct] < 0) at `0x82BF9848`.
- No explicit writes to g_needDeviceReselect or g_saveInProgress in state 14 itself (those are done in state 13's confirm path).

### Proposed Hook Approach
- `sub_8284ADA0`: Hook to return 1 (ready) immediately, since the state 12 stub should have already marked the operation as complete.
- `sub_8284AFE0`: Hook to no-op or perform final write. Since the save operation was stubbed, this just needs to not crash.

---

## Summary: Game Logic vs Xbox-Specific by State

### State 9 (Init Progress + Start Write)
- **Game logic**: sub_8223D2F0 (zero-fill progress data) -- PRESERVE.
- **Xbox-specific**: sub_8284ABA0/sub_8284A1E8 (XContent write start) -- STUB.
- **Xbox guards (no-op)**: sub_8223DB20, sub_82240B78.

### State 10 (Wait for Write Progress)
- **Game logic**: sub_8284ABF8 (state reset), sub_8284B490 (read status), sub_8284B430 (map result), platformMode branching -- all PRESERVE.
- **Xbox-specific**: sub_8284ABD0/sub_82849C98 (poll async I/O) -- STUB.
- **Xbox guards (no-op)**: sub_8223DB20, sub_82240B78.

### State 11 (Verify Save)
- **Game logic**: sub_8223D400 (verify save names), platformMode branching -- PRESERVE.
- **Xbox guards (no-op)**: sub_8223DB20, sub_82240B78.
- **No Xbox-specific functions** in state 11 itself.

### State 12 (Multiplayer Phase 1: Initiate)
- **Game logic**: sub_822417B0 orchestration flow -- PRESERVE structure.
- **Xbox-specific**: sub_8284AD78 (save overwrite/delete initiation) -- STUB.
- **Xbox guards**: sub_8223DB20, sub_82240B78 (called via sub_822417B0).

### State 13 (Error Recovery)
- **Game logic**: memory clears (g_needDeviceReselect=0, g_saveInProgress=0), STATE_VAR=0 restart -- PRESERVE.
- **Xbox-specific**: sub_8223F9F0 mode 4 (delete confirmation UI) -- STUB to auto-confirm.
- **Xbox-specific**: sub_8223DB90/sub_8284B3B0 (XContent close) -- STUB.
- **Xbox guards (no-op)**: sub_8223DB20, sub_82240B78.

### State 14 (Multiplayer Phase 2: Poll + Finalize)
- **Game logic**: [g_stateSubstruct] check, g_needDeviceReselect branching, sub_8223F790 (check progress entry), sub_8223CAD8 (cleanup) -- PRESERVE.
- **Xbox-specific**: sub_8284ADA0 (poll async), sub_8284AFE0 (finalize) -- STUB (via sub_822417B0).
- **Xbox guards**: sub_8223DB20, sub_82240B78 (called via sub_822417B0).

---

## sub_822417B0 Two-Phase Call Pattern (Complete)

```
State 12 calls: sub_822417B0(r3=0, r4=1, ...)   <-- INITIATE
  |
  +-- r4=1 branch:
  |     sub_8223DB20()     -> error 33
  |     sub_82240B78()     -> error 34
  |     sub_8284AD78(...)  -> initiate save op
  |     Returns: 2=error, 1=in-progress (normal)
  |
  v
State 14 calls: sub_822417B0(r3=0, r4=0, ...)   <-- POLL
  |
  +-- r4=0 branch:
        sub_8284ADA0(...)  -> poll completion
        if ready:
          sub_8284AFE0(...)  -> advance/finalize
          sub_8223DB20()     -> error 33
          sub_82240B78()     -> error 34
          Returns: 0=complete
        if not ready:
          Returns: 1=in-progress
```

The r8/r9 swap between state 12 and state 14:
- State 12: r8=&local_84, r9=&local_88 (sub_822417B0 writes 0 to both via its prologue)
- State 14: r8=&local_88, r9=&local_84 (SWAPPED)

This swap means the two output pointers track different things depending on the phase. In phase 1, local_84 gets the first output and local_88 the second. In phase 2, the ordering reverses so the caller can distinguish which output came from which phase.

---

## Minimum Stubs Required for States 9-14

| Stub Target | What to Do | Used By |
|-------------|-----------|---------|
| `sub_8284A1E8` (via sub_8284ABA0) | Set element state=5, result=0 | State 9 |
| `sub_82849C98` (via sub_8284ABD0) | Return 1 (ready) | State 10 |
| `sub_8284AD78` (via sub_822417B0) | Set element state=3, result=0, return 1 | State 12 |
| `sub_8284ADA0` (via sub_822417B0) | Return 1 (ready) | State 14 |
| `sub_8284AFE0` (via sub_822417B0) | No-op or minimal finalize, return nonzero | State 14 |
| `sub_8223F9F0` (mode 4) | Return 1 with outBool=1 (auto-confirm) | State 13 |
| `sub_8223DB90` / `sub_8284B3B0` | No-op close | State 13 |

### Already-Working Functions (no hooks needed)
- `sub_8223D2F0` -- pure memory zeroing
- `sub_8284ABF8` -- pure state reset (element[0]: 5->0)
- `sub_8284B490` -- pure status read
- `sub_8284B430` -- pure result mapping
- `sub_8223D400` -- pure string comparison
- `sub_8223F790` -- pure data inspection
- `sub_8223DB20` -- XNotifyGetNext hook already returns 0
- `sub_82240B78` -- XNotifyGetNext hook already returns 0

---

## Single-Player vs Multiplayer Flow

For **single-player** (platformMode 0 or 1):
- States 9 -> 10 -> 11 -> FINISHED (r3=0)
- States 12, 13, 14 are **never reached**

For **multiplayer** (platformMode 3 or 4):
- States 9 -> 10 -> 11 -> 12 -> 14 -> (possibly 13) -> FINISHED or restart
- State 13 only entered if [g_stateSubstruct] < 0 (error slot)

For platformMode 2:
- State 10 completes directly (never reaches 11+)

This means for an initial single-player implementation, only states 9, 10, and 11 need working stubs. States 12-14 can be deferred until multiplayer support is needed.
