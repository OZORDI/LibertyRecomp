# sub_822438B0 -- Outer Save State Machine

**Source**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.6.cpp` (line 87081)

## Key Registers and Addresses

All addresses computed via Python (`(-32064 & 0xFFFF) << 16 = 0x82C00000`, etc.).

| Register | Base Value | Notes |
|----------|-----------|-------|
| r31 | 0x82C00000 | lis -32064; primary global base |
| r28 | 0x82C00000 | same as r31 |
| r30 | 0x82BF3934 | lis -32065 + addi 14644; "timer/request" struct |
| r27 | 0x82BF3A78 | lis -32065 + addi 14968; "completion" struct |
| r29 | 0 (init) | "done" flag; set to 1 to signal completion |

## Global Memory Map

| Address | Size | Name | Description |
|---------|------|------|-------------|
| 0x82BF9838 | u32 | STATE | Current state (0-7); the switch variable at r31-26568 |
| 0x82BF9834 | u32 | SUB_STATE | Secondary state at r28-26572 |
| 0x82BF9844 | u32 | field_9844 | Written to 2 in state 1 (at r31-26556) |
| 0x82BF981E | u8 | field_981E | Cleared to 0 in state 1 (at r31-26594) |
| 0x82BF9848 | u32 | field_9848 | Cleared to 0 in state 1 (at r31-26552) |
| 0x82BF9898 | u64 | save_result | 8-byte value stored in state 6 (at r31-26472) |
| 0x82A95466 | u8 | save_trigger | Set to 1 in state 1 (at r10+21606, base 0x82A90000) |
| 0x82A9547C | u8 | ready_flag | Polled in state 2 (at r10+21628); nonzero = ready |
| 0x82A9546C | u32 | error_code | Error code checked against 33 (at r10+21612) |

## r30 Struct (0x82BF3934) -- "Timer/Request"

| Offset | Size | Description |
|--------|------|-------------|
| +0 | u32 | counter/timer value (compared to 3000 in state 6) |
| +4 | u8 | active flag (set to 1 in state 2, checked in state 6) |
| +8 | u32 | mode (set to 2 in state 2, checked == 2 in state 6) |

## r27 Struct (0x82BF3A78) -- "Completion"

| Offset | Size | Description |
|--------|------|-------------|
| +0 | u32 | pending_flag (checked in epilogue; if nonzero, calls sub_8223CC10) |
| +8 | u8 | completion_written flag (set to 1 on error-33 or normal completion) |

## Data Buffer (0x82BF3940)

16 bytes zeroed in state 2 transition to state 3. Bytes at offset +12..+15 (0x82BF394C-394F) are checked for the ASCII magic "SAVE" (S=83, A=65, V=86, E=69) in state 4.

---

## Return Values

The function returns one of three values in r3:

| Value | Meaning |
|-------|---------|
| 0 | Idle / no-op (state 0 early return) |
| 1 | In progress (normal fallthrough at loc_82243C40) |
| 2 | Done / completed with reset (epilogue when r29==1) |

When returning 2 (done), the epilogue also:
- Calls sub_8223CC68(r30, 2) -- cancels/resets the timer struct
- Resets STATE to 0 and SUB_STATE to 0
- Calls sub_8223CAD8 (cleanup)
- If SUB_STATE was 1 or 2, and r27+0 (pending_flag) is nonzero, calls sub_8223CC10

---

## State-by-State Documentation

### State 0 -- IDLE

**Location**: loc_8224391C (line 87145)

**Behavior**: Immediately returns 0 (r3 = 0). No side effects.

**Transitions**: None from within this state. External code must set STATE to 1 to begin.

---

### State 1 -- INITIATE SAVE

**Location**: loc_82243928 (line 87153)

**Behavior**: Sets up all save state variables, then falls through to state 2.

**Writes**:
- 0x82A95466 (save_trigger) = 1 (u8)
- 0x82BF9844 (field_9844) = 2 (u32)
- 0x82BF981E (field_981E) = 0 (u8)
- 0x82BF9848 (field_9848) = 0 (u32)
- 0x82BF9838 (STATE) = 2

**Transitions**: STATE -> 2, then falls through to state 2 code immediately.

---

### State 2 -- WAIT FOR CONTENT ENUMERATION (sub_82242910)

**Location**: loc_8224395C (line 87180)

**Behavior**:
1. Reads ready_flag at 0x82A9547C
2. If ready_flag == 0 (not yet ready):
   - Sets r30+4 (active flag) = 1
   - Sets r30+0 (counter) = 0
   - Sets r30+8 (mode) = 2
3. Calls **sub_82242910** (content enumeration / inner state machine)
4. Checks return value:

**Return from sub_82242910**:
- **r3 == 2 (error)**: Checks error_code at 0x82A9546C:
  - If error_code == 33: sets r29 = 1 (done flag), falls to epilogue -> returns 2
  - If error_code != 33: STATE -> 7 (error/retry)
- **r3 == 0 (success)**:
  - Zeroes 16-byte buffer at 0x82BF3940
  - STATE -> 3
- **r3 == 1 (in progress)**: Falls through to loc_82243BE8 -> returns 1

**Functions called**: sub_82242910

---

### State 3 -- EXECUTE SAVE OPERATION (sub_82240F80)

**Location**: loc_822439E0 (line 87252)

**Behavior**: Calls **sub_82240F80(0)** (save operation with arg=0).

**Return from sub_82240F80**:
- **r3 == 0 (success)**:
  - STATE -> 4
  - Checks "SAVE" magic in buffer at 0x82BF394C-394F (bytes: S=83, A=65, V=86, E=69)
  - If magic matches: falls through to epilogue -> returns 1 (continue)
  - If magic does NOT match: calls sub_822BCA90(0x820067B0), sets error_code = 29, STATE -> 7
- **r3 == 2 (error)**: Checks error_code at 0x82A9546C:
  - If error_code == 33: sets r29 = 1, writes completion flag r27+8 = 1, falls to epilogue -> returns 2
  - If error_code != 33: STATE -> 7, writes completion flag r27+8 = 1
- **r3 == 1 (in progress)**: Falls through -> returns 1

**Functions called**: sub_82240F80, sub_822BCA90 (on bad magic)

**Note on "SAVE" magic check**: The check at offset +12 of the 16-byte buffer determines if the save data header is valid. If the 4 bytes are not "SAVE", error_code is set to 29 and the machine enters error state 7.

---

### State 4 -- VALIDATE & POST-SAVE SETUP

**Location**: loc_82243ABC (line 87371)

**Behavior**:
1. Immediately sets STATE -> 5
2. Checks SUB_STATE (0x82BF9834):
   - If SUB_STATE != 2: falls through -> returns 1
   - If SUB_STATE == 2:
     - Calls **sub_822446E8** (no args)
     - Calls **sub_8223F740(0)** (some validation, returns bool)
     - If sub_8223F740 returns nonzero (true): falls through -> returns 1
     - If sub_8223F740 returns 0 (false): sets error_code = 31, STATE -> 7

**Functions called**: sub_822446E8, sub_8223F740

**Transitions**:
- Normal: STATE -> 5
- Error (sub_8223F740 fails with SUB_STATE==2): STATE -> 7, error_code = 31

---

### State 5 -- WRITE SAVE DATA (sub_82240F80)

**Location**: loc_82243B00 (line 87408)

**Behavior**: Calls **sub_82240F80(1)** (save operation with arg=1, i.e., write phase).

**Return from sub_82240F80**:
- **r3 == 0 (success)**: STATE -> 6
- **r3 == 2 (error)**: Checks error_code at 0x82A9546C:
  - If error_code == 33: sets r29 = 1, writes r27+8 = 1, falls to epilogue -> returns 2
  - If error_code != 33: jumps to loc_82243A18 (STATE -> 7, writes r27+8 = 1)
- **r3 == 1 (in progress)**: Falls through -> returns 1

**Functions called**: sub_82240F80

**Note**: State 5's error path for error_code != 33 reuses loc_82243A18 from state 3 (the goto target crosses state boundaries in the generated code).

---

### State 6 -- FINALIZE SAVE

**Location**: loc_82243B44 (line 87445)

**Behavior**: Checks if the save timer has elapsed, then finalizes.

1. Reads r30+4 (active flag):
   - If active flag == 0: skip to step 3 (r11 = 1)
2. Reads r30+8 (mode):
   - If mode != 2: skip to step 3 (r11 = 1)
3. Reads r30+0 (counter), compares against 3000:
   - If counter >= 3000: r11 = 1 (timer elapsed)
   - If counter < 3000: r11 = 0 (timer not elapsed)
   - (Note: If steps 1-2 skipped, r11 = 1 unconditionally)
4. If r11 == 0 (timer not elapsed): falls through -> returns 1 (still waiting)
5. If r11 != 0 (timer elapsed or inactive):
   - Calls **sub_8223CAD8** (cleanup)
   - Calls **sub_826CD808** (returns a pointer)
   - If pointer != 0:
     - Calls sub_826CD808 again
     - Calls **sub_829DBAA8** (returns pointer to 8-byte result)
     - Stores 8-byte value from result to 0x82BF9898 (save_result)
     - Returns 0 (idle -- save complete, state NOT changed explicitly)
   - If pointer == 0:
     - Calls **sub_829DB688(0x82BF9898)** (passes save_result address)
     - Sets r29 = 1 -> epilogue -> returns 2 (done with reset)

**Functions called**: sub_8223CAD8, sub_826CD808, sub_829DBAA8, sub_829DB688

**Transitions**:
- Timer not elapsed: stays in state 6, returns 1
- Pointer != 0: returns 0 (state unchanged, re-enters state 6 next tick)
- Pointer == 0: r29 = 1, epilogue resets STATE -> 0, returns 2

---

### State 7 -- ERROR / RETRY

**Location**: loc_82243BC8 (line 87524)

**Behavior**:
1. Calls **sub_8223CC68(r30, 2)** -- cancel/reset timer struct with mode=2
2. Calls **sub_82242608** -- retry/error handling function
3. Checks return value (bool, lower 8 bits):
   - If return == 0: falls through -> returns 1 (retry still in progress)
   - If return != 0: sets r29 = 1 -> epilogue -> returns 2 (done with reset)

**Functions called**: sub_8223CC68, sub_82242608

**Transitions**:
- sub_82242608 returns 0: stays in state 7, returns 1
- sub_82242608 returns nonzero: epilogue resets STATE -> 0, returns 2

---

## Epilogue (shared exit path)

**Location**: loc_82243BE8 (line 87544) through loc_82243C40 (line 87595)

After the switch body executes, the epilogue checks r29 (done flag):

**If r29 == 0** (not done):
- Returns 1 (in progress)

**If r29 != 0** (done):
1. Calls sub_8223CC68(r30, 2) -- cancel/reset timer struct
2. Checks SUB_STATE (0x82BF9834):
   - If SUB_STATE == 1 or SUB_STATE == 2: checks r27+0 (pending_flag)
     - If pending_flag != 0: calls sub_8223CC10 (cleanup pending operation)
   - Otherwise: skips
3. Resets STATE (0x82BF9838) = 0
4. Resets SUB_STATE (0x82BF9834) = 0
5. Calls sub_8223CAD8 (cleanup)
6. Returns 2 (done)

---

## State Transition Diagram

```
                    [External trigger]
                          |
                          v
    +---> State 0 (IDLE) --[set STATE=1]--> State 1 (INITIATE)
    |                                           |
    |                                     (falls through)
    |                                           v
    |                                     State 2 (ENUMERATE)
    |                                      /        |        \
    |                              r3==0  /    r3==1 |    r3==2\
    |                                    v     (wait)|         v
    |                              State 3           |    err33? -> done(2)
    |                              (SAVE OP)         |    else  -> State 7
    |                              /    |    \       |
    |                       r3==0 / r3==1\  r3==2    |
    |                            v  (wait)v     \    |
    |                      State 4         |  err33? -> done(2)
    |                      (VALIDATE)      |  else  -> State 7
    |                       /      \       |
    |                 ok   /  fail  \      |
    |                     v    (err31)\    |
    |               State 5     State 7   |
    |               (WRITE)       ^       |
    |               /    |    \   |       |
    |        r3==0 / r3==1\ r3==2|       |
    |              v (wait) v    /        |
    |        State 6       |   /         |
    |        (FINALIZE)    |  /          |
    |         /    \       | /           |
    |   timer/  ptr \      |            |
    |   wait  result \     |            |
    |     |      |    done(2)           |
    |     v      v                      |
    |   (wait) ret 0                    |
    |                                   |
    +------- done(2) resets STATE=0 <---+
```

## Error Codes (stored at 0x82A9546C)

| Code | Meaning | Set By |
|------|---------|--------|
| 29 | Invalid save header (no "SAVE" magic) | State 3 (sub_822438B0 itself) |
| 31 | Post-save validation failed (sub_8223F740 returned false) | State 4 |
| 33 | Special/cancellation (triggers immediate done, not error state) | External (checked in states 2, 3, 5) |

## Summary of Called Functions

| Function | Called From | Args | Purpose |
|----------|-----------|------|---------|
| sub_82242910 | State 2 | none | Content enumeration / inner state machine |
| sub_82240F80 | State 3, State 5 | r3=0 (state 3), r3=1 (state 5) | Save operation (read phase / write phase) |
| sub_822446E8 | State 4 | none | Post-save setup (only when SUB_STATE==2) |
| sub_8223F740 | State 4 | r3=0 | Validation check (only when SUB_STATE==2) |
| sub_822BCA90 | State 3 | r3=0x820067B0 (string) | Error logging/display (bad save magic) |
| sub_8223CAD8 | State 6, Epilogue | none | Cleanup |
| sub_826CD808 | State 6 | none | Get pointer (called twice if first returns nonzero) |
| sub_829DBAA8 | State 6 | r3=ptr from sub_826CD808 | Get 8-byte save result |
| sub_829DB688 | State 6 | r3=0x82BF9898 (save_result addr) | Process save result (when ptr==0) |
| sub_8223CC68 | State 7, Epilogue | r3=r30, r4=2 | Cancel/reset timer struct |
| sub_82242608 | State 7 | none | Error retry handler (returns bool) |
| sub_8223CC10 | Epilogue | none | Cleanup pending operation |
