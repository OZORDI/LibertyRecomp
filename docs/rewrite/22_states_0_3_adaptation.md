# 22 - States 0-3 Adaptation Analysis

**Source of truth**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.6.cpp` lines 84816-85117

## Overview

States 0-3 of `sub_82242910` form the **initialization and readiness gate** for the
scene creation state machine. On Xbox 360, these states handle device enumeration
delays, Xbox LIVE network session detection, user sign-in validation, and content
(DLC/update) enumeration via XContent APIs. In the recomp, most of these Xbox-specific
concerns do not apply, but the game logic flow and memory writes they produce are
critical for states 4+ to function correctly.

---

## Shared Labels

Two shared exit labels are used by multiple states:

| Label | Line | Action |
|-------|------|--------|
| `loc_82243250` | 86152 | Return r3=1 (in progress, no state change) |
| `loc_82242B08` | 85106 | Set STATE_VAR=2, return r3=1 |
| `loc_82242A34` | 84982 | Call sub_826CB6B8 (network teardown), fall through to loc_82242A38 |
| `loc_82242A38` | 84986 | Set STATE_VAR=3, return r3=1 |
| `loc_82242AB8` | 85058 | Write ERROR_CODE=6, fall through to loc_82242AC4 |
| `loc_82242AC4` | 85065 | Return r3=2 (error) |

---

## State 0 -- Initialize / Check Ready

**Label**: `loc_822429A0` (line 84898)

### Code Flow

```
call sub_8223DAA0()     // readiness check
if (result & 0xFF) != 0:  // READY
    r3 = SCENE_NAME (0x83192C50)
    r4 = MEM[0x82BF3D90]  // SCENE_INFO_PTR (user index written by sub_8223DAA0)
    STATE_VAR = 4
    call sub_8284B4B0(SCENE_NAME, user_index)
    return 1
else:                     // NOT READY
    STATE_VAR = 1
    return 1
```

### Sub-function Classification

| Function | Classification | Rationale |
|----------|---------------|-----------|
| `sub_8223DAA0` | **XBOX SIGN-IN + DEVICE CHECK** | Resets MEM[0x82BF3D90]=-1, calls sub_826CD808 (device manager query), reads XamUserGetSigninState. On recomp, devices are immediately ready and user is signed in, so this returns 1 on first call -- skipping states 1-3. |
| `sub_8284B4B0` | **PURE GAME LOGIC** | Trivial: computes an array index from r4, zeroes a u32 at SCENE_NAME+offset. 7 instructions, no syscalls. |

### Memory Writes

| Address | Value | Downstream Dependency |
|---------|-------|----------------------|
| `0x82BF3D90` | user_index or -1 | Written by sub_8223DAA0. Read by state 4 (sub_8223DB20) and fast-path (sub_8284B4B0 arg). |
| `0x82BF9848` | 4 (fast) or 1 (normal) | STATE_VAR -- controls next state entry. |
| `SCENE_NAME+computed_offset` | 0 | Written by sub_8284B4B0 on fast path only. |

### Xbox-Specific Blocking

sub_8223DAA0 calls:
- `sub_826CD808` -- queries device manager object at 0x8318AAF8, calls `sub_822BF360` (are devices enumerated?), `sub_829DB9B8` (read user index from object+72)
- `sub_82A12188` -- `XamUserGetSigninState()` Xbox kernel call

**Problem**: In recomp, `sub_822BF360` returns true immediately (VFS is synchronous),
and `XamUserGetSigninState` returns `SignedInLocally` (1). This causes the fast path
(state 0->4), skipping states 1-3 which write ERROR_CODE=6 to 0x82A9546C and do
other initialization that state 4 expects.

### Existing Hook

The current hook in `imports.cpp:1816` uses **wrap + post-patch**: calls `__imp__`,
then if state went 0->4, forces it back to 1. This works but relies on the original
sub_8223DAA0 still running (which writes 0x82BF3D90).

### Proposed Approach

**KEEP EXISTING WRAP HOOK** (Pattern 2). The current post-patch (0->4 forced to 0->1)
is correct and sufficient. sub_8223DAA0 can run as recompiled code because its
Xbox calls (XamUserGetSigninState) are already hooked in the recomp. The side effect
write to 0x82BF3D90 is preserved.

No changes needed for state 0.

---

## State 1 -- Wait for Network + Content Check

**Label**: `loc_822429EC` (line 84942)

### Code Flow

```
call sub_826CBA70()           // is network session active?
if (result & 0xFF) != 0:     // network active
    call sub_826CB6B8()       // tear down network session
    STATE_VAR = 3
    return 1

call sub_8223DAA0()           // readiness check
if (result & 0xFF) != 0:     // ready
    STATE_VAR = 3
    return 1

call sub_8223F9F0(0, 0, &local_80)  // content enumeration mode 0
if (result & 0xFF) == 0:     // enumeration not done yet
    goto loc_82243250         // return 1, no state change (wait)

if local_80 != 0:            // content found
    STATE_VAR = 2             // go to alternate content check
    return 1

// local_80 == 0: no content, readiness not yet achieved
STATE_VAR = 3                 // skip to pre-load
return 1
```

### Sub-function Classification

| Function | Classification | Rationale |
|----------|---------------|-----------|
| `sub_826CBA70` | **XBOX NETWORK SESSION CHECK** | Thin wrapper: loads byte at 0x8317F870, returns bit 7 (network session active flag). Pure memory read -- no syscalls. In single-player recomp, this byte is always 0 (no LIVE session), so it always returns 0. **Safe to run as-is.** |
| `sub_826CB6B8` | **XBOX NETWORK TEARDOWN** | Thin wrapper: passes network object 0x8317F838 to sub_829EEE00, which reads flags, calls CloseHandle-like functions, clears state. In recomp with no network, this path is never taken (guarded by sub_826CBA70 returning nonzero). **Dead code in single-player.** |
| `sub_8223DAA0` | **XBOX SIGN-IN + DEVICE CHECK** | Same as state 0. On second call (state 1 re-entry), returns 1 since devices are ready. This causes immediate transition to state 3, skipping the content enumeration in sub_8223F9F0. |
| `sub_8223F9F0` | **XBOX CONTENT ENUMERATION** (54-state sub-machine) | Massive function with 54 switch cases. Calls `sub_8224EFE8` (XContentEnumerate wrapper), reads content enumeration results, checks DLC availability. Mode 0 = initial content scan. **This is the most Xbox-specific function in states 0-3.** |

### Memory Writes

| Address | Value | Downstream Dependency |
|---------|-------|----------------------|
| `0x82BF9848` | 2 or 3 | STATE_VAR transition |
| `local_80` (stack) | content flag | Determines state 2 vs 3 transition |

### Xbox-Specific Blocking

Three paths through state 1:
1. **Network active** (sub_826CBA70 returns 1): tears down network, goes to state 3. In recomp, byte at 0x8317F870 bit 7 is 0, so this path is never taken.
2. **Readiness achieved** (sub_8223DAA0 returns 1): skips content enum, goes to state 3. In recomp, this always happens on first call because devices are immediately ready.
3. **Content enumeration** (sub_8223F9F0): only reached if sub_8223DAA0 returns 0. In recomp, never reached due to path 2.

**In practice**, state 1 in the recomp immediately takes path 2 (sub_8223DAA0 returns 1)
and transitions to state 3. The content enumeration code is never executed.

### Proposed Approach

**LET RUN AS-IS** (no hook needed). The existing state 0 hook already forces the
machine into state 1. State 1's code path through sub_826CBA70 (returns 0 in recomp)
then sub_8223DAA0 (returns 1 in recomp) produces the correct transition to state 3.
The content enumeration path (sub_8223F9F0) is never reached, so its Xbox content
APIs are irrelevant.

If a hook on sub_8223DAA0 is added later to return 0 for the first N calls (to force
the content enumeration path), sub_8223F9F0 would need a hook too. But this is
unnecessary for single-player boot.

---

## State 2 -- Alternate Content Check Path

**Label**: `loc_82242A4C` (line 84998)

### Code Flow

```
call sub_8223CB60()           // does current mode need content check?
r31 = result & 0xFF          // save for later

local_80 = 0                  // initialize content flag

call sub_826CBA70()           // network session active?
if (result & 0xFF) != 0:     // network active
    call sub_826CB6B8()       // tear down
    STATE_VAR = 3
    return 1

call sub_8223DAA0()           // readiness check
if (result & 0xFF) != 0:     // ready
    STATE_VAR = 3
    return 1

if (r31 & 0xFF) != 0:        // mode needs content check
    call sub_8223F9F0(1, 0, &local_80)  // content enumeration mode 1
    if (result & 0xFF) == 0:  // not done yet
        goto loc_82243250     // return 1, wait

// Check results:
if local_80 != 0:            // content found
    if r31 != 0:             // mode needs content AND content found
        call sub_826CB6B8()  // tear down network (cleanup before proceeding)
        STATE_VAR = 3
        return 1

// Fall through: ERROR path
ERROR_CODE = 6               // content not available
return 2                     // error
```

### Sub-function Classification

| Function | Classification | Rationale |
|----------|---------------|-----------|
| `sub_8223CB60` | **PURE GAME LOGIC** | Switch on SCENE_MODE at 0x82BF9844: modes 0,2 return 0; modes 1,3,4 return 1; mode >4 returns 1. Determines whether DLC content enumeration is needed for the current game mode. No syscalls, pure memory read. |
| `sub_826CBA70` | **XBOX NETWORK CHECK** | Same as state 1. Returns 0 in recomp. |
| `sub_8223DAA0` | **XBOX SIGN-IN CHECK** | Same as states 0/1. Returns 1 in recomp. |
| `sub_8223F9F0` | **XBOX CONTENT ENUMERATION** | Mode 1 = DLC content scan. Only reached if sub_8223CB60 returns 1 AND sub_8223DAA0 returns 0 (never in recomp). |
| `sub_826CB6B8` | **XBOX NETWORK TEARDOWN** | Same as state 1. |

### Memory Writes

| Address | Value | Downstream Dependency |
|---------|-------|----------------------|
| `0x82BF9848` | 3 | STATE_VAR transition to state 3 |
| `0x82A9546C` | 6 | ERROR_CODE (only on error path) |
| `local_80` (stack) | content flag | Controls error vs success path |

### Xbox-Specific Blocking

State 2 is reached from state 1 only when `local_80 != 0` (content found during mode 0
enumeration). In the recomp, state 1 never performs content enumeration (sub_8223DAA0
returns 1 immediately), so state 2 is **never reached in normal single-player boot**.

Even if state 2 were reached:
- sub_826CBA70 returns 0 (no network)
- sub_8223DAA0 returns 1 (devices ready) -> immediate transition to state 3

The content enumeration and error paths are unreachable in the recomp.

### Proposed Approach

**LET RUN AS-IS** (no hook needed). State 2 is unreachable in normal single-player
boot. If it were reached, sub_8223DAA0 returning 1 would immediately transition to
state 3, bypassing all Xbox content enumeration code.

The ERROR_CODE=6 path requires both `local_80==0` AND `r31==0` (sub_8223CB60 returned
0, meaning mode 0 or 2). With the existing hook forcing platformMode=3, sub_8223CB60
would return 1, making the error path unreachable even if state 2 were entered.

---

## State 3 -- Network + Ready Check (Pre-Load)

**Label**: `loc_82242AD0` (line 85073)

### Code Flow

```
call sub_826CBA70()           // network session active?
if (result & 0xFF) != 0:     // network active
    goto loc_82243250         // return 1, wait for network to settle

call sub_8223DAA0()           // readiness check
if (result & 0xFF) == 0:     // NOT ready
    STATE_VAR = 2             // go back to state 2
    return 1

// Ready:
call sub_82240AB0()           // pre-load setup
STATE_VAR = 4
return 1
```

### Sub-function Classification

| Function | Classification | Rationale |
|----------|---------------|-----------|
| `sub_826CBA70` | **XBOX NETWORK CHECK** | Returns 0 in recomp. Gate passes immediately. |
| `sub_8223DAA0` | **XBOX SIGN-IN CHECK** | Returns 1 in recomp. Gate passes immediately. |
| `sub_82240AB0` | **GAME LOGIC + CLEANUP** | Calls sub_8223DAA0 (readiness), then if ready: clears ADDR_15578 (0x82BF3CDA)=0, calls sub_8223DB90(0) (scene state cleanup -- calls sub_8284B3B0 to stop/reset scene, then conditionally writes byte at 0x82BF3A76), calls sub_8284B4B0(SCENE_NAME, user_index) (zeroes scene array entry). This is the critical initialization that state 4 expects. |

### Memory Writes

| Address | Value | Written By | Downstream Dependency |
|---------|-------|-----------|----------------------|
| `0x82BF9848` | 4 or 2 | state 3 | STATE_VAR transition |
| `0x82BF3CDA` | 0 | sub_82240AB0 | ADDR_15578, read by states 13-14 on cleanup path |
| `0x82BF3D90` | user_index | sub_8223DAA0 (via sub_82240AB0) | SCENE_INFO_PTR, read by states 4+ for scene setup args |
| `0x82BF3A76` | conditional | sub_8223DB90 | BYTE_14966, read by state 5 |
| SCENE_NAME array | 0 | sub_8284B4B0 (via sub_82240AB0) | Scene tracking array, read by states 6+ |

### Xbox-Specific Blocking

In the recomp:
1. sub_826CBA70 returns 0 (no network) -> gate passes
2. sub_8223DAA0 returns 1 (ready) -> gate passes
3. sub_82240AB0 runs its initialization

State 3 transitions to state 4 on the first call. No blocking occurs.

### Proposed Approach

**LET RUN AS-IS** (no hook needed). All three sub-functions behave correctly in the
recomp:
- sub_826CBA70 returns 0 (correct: no Xbox LIVE network)
- sub_8223DAA0 returns 1 (correct: devices/user ready)
- sub_82240AB0 performs critical initialization that state 4 depends on

The initialization performed by sub_82240AB0 is the **entire reason** the existing
hook forces state 0->1 instead of 0->4. The fast path (0->4) skips sub_82240AB0,
leaving stale data that causes state 4 to hit error 34.

---

## Summary: Adaptation Requirements for States 0-3

### State-by-State Verdict

| State | Verdict | Reason |
|-------|---------|--------|
| **0** | **KEEP EXISTING HOOK** | Wrap + post-patch forces 0->1 instead of 0->4. Already works correctly. |
| **1** | **NO HOOK NEEDED** | sub_826CBA70 returns 0 (no network), sub_8223DAA0 returns 1 (ready), transitions to state 3 immediately. Content enumeration never reached. |
| **2** | **NO HOOK NEEDED** | Unreachable in normal single-player boot (state 1 never transitions to state 2). |
| **3** | **NO HOOK NEEDED** | sub_826CBA70 returns 0, sub_8223DAA0 returns 1, sub_82240AB0 performs critical init, transitions to state 4. All correct. |

### Critical Initialization Path

The correct path through states 0-3 in the recomp is:

```
State 0: sub_8223DAA0() returns 1 -> STATE=4 (fast path)
  Hook intercepts: STATE=4 forced to STATE=1

State 1: sub_826CBA70() returns 0, sub_8223DAA0() returns 1 -> STATE=3

State 3: sub_826CBA70() returns 0, sub_8223DAA0() returns 1
  -> sub_82240AB0() runs: clears 0x82BF3CDA, calls sub_8223DB90, calls sub_8284B4B0
  -> STATE=4
```

This takes 3 calls to sub_82242910 (one for each state: 0->1, 1->3, 3->4).

### Sub-function Summary

| Function | Address | Category | In Recomp |
|----------|---------|----------|-----------|
| `sub_8223DAA0` | 0x8223DAA0 | Xbox sign-in + device check | Returns 1 (ready) -- correct behavior |
| `sub_8223CB60` | 0x8223CB60 | Pure game logic (mode switch) | Returns 1 for mode 3 -- correct |
| `sub_826CBA70` | 0x826CBA70 | Xbox network session check | Returns 0 (no session) -- correct |
| `sub_826CB6B8` | 0x826CB6B8 | Xbox network teardown | Dead code (never called, guarded by sub_826CBA70) |
| `sub_8223F9F0` | 0x8223F9F0 | Xbox content enumeration (54 states) | Dead code in states 0-3 (never reached) |
| `sub_82240AB0` | 0x82240AB0 | Game logic + cleanup | Runs correctly, performs critical init |
| `sub_8284B4B0` | 0x8284B4B0 | Pure game logic (array clear) | Runs correctly |
| `sub_8223DB90` | 0x8223DB90 | Game logic (scene reset/cleanup) | Called by sub_82240AB0, runs correctly |

### Xbox APIs That Are Safely Avoided

The following Xbox-specific code paths exist in states 0-3 but are never executed
in the recomp's single-player boot:

1. **XContentEnumerate** (via sub_8223F9F0) -- content/DLC enumeration. Never reached
   because sub_8223DAA0 returns 1 before the enumeration call in states 1-2.

2. **Xbox LIVE session management** (sub_826CBA70/sub_826CB6B8) -- network session
   check and teardown. sub_826CBA70 always returns 0 (bit 7 of 0x8317F870 is clear),
   so teardown is never called.

3. **XamUserGetSigninState** (via sub_8223DAA0 -> sub_82A12188) -- already hooked in
   rexcrt, returns SignedInLocally (1).

### Risk Assessment

**Low risk**. States 0-3 are the most straightforward part of the state machine.
The only hook needed is the existing state 0 wrap hook that prevents the fast path.
All sub-functions either return correct values for the recomp environment or are
never called due to guard conditions.

The one area to monitor is if future changes (e.g., DLC support) require the
content enumeration path through sub_8223F9F0. That function's 54-state sub-machine
would need significant adaptation for non-Xbox content discovery. But for
single-player base game boot, it is completely bypassed.
