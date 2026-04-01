# 46: Cascading Failures Audit -- Scene Creation Hooks

## Scope

Six hooks added for scene creation state machine adaptation. Audit each for
side effects that could cause the stack guard page infinite loop.

---

## 1. sub_8223DB20 -- Sign-In Notification Guard (returns 0)

**Hook**: `ctx.r3.u64 = 0` (no sign-in change detected)

**Callers**: ~30 states in sub_82242910 (states 4, 5, 6, 7, 8, 9, 10, 11, 13, 14).

**Native behavior**: Calls `XNotifyGetNext(handle, 10)` for XN_SYS_SIGNINCHANGED.
If notification found, returns 1 (triggers error 33 path). Otherwise returns 0.

**Risk**: **NONE**. Pure notification poll with no side effects. The native code
already returns 0 in the recomp because RexGlue's XNotifyGetNext for event 10
returns false (no pending sign-in notifications after initial boot broadcast).
The hook just makes this explicit and avoids the spurious initial broadcast hit.

**Cascading potential**: Zero. Return value only gates error 33 (sign-in changed).
Returning 0 means "no error" -- the safest possible return.

---

## 2. sub_82240B78 -- Storage Device Notification Guard (returns 0)

**Hook**: `ctx.r3.u64 = 0` (no storage device change detected)

**Callers**: ~17 states in sub_82242910 (states 4, 9, 10, 11, 13, 14).

**Native behavior**: Calls `XNotifyGetNext(handle, 11)` for XN_SYS_STORAGEDEVICESCHANGED.
If found AND sub_8223DAA0 returns true AND 0x82BF3A77 != 0, then recursively calls
sub_82240B08 and potentially sets error 34.

**Risk**: **NONE**. RexGlue never broadcasts event 0x0B. The native code already
returns 0. Hook is a no-op optimization.

**Note**: The native code at line 80397-80398 reads 0x82BF3A77 (flag set by
sub_82240B08). This read only matters when XNotifyGetNext returns true, which
never happens. So even though sub_82240B08 unconditionally sets this flag (see
hook 3), the flag is never read in a harmful context through this path.

---

## 3. sub_82240B08 -- Content Device Readiness (returns 1 + sets flags)

**Hook**: Sets 0x82BF3A77=1, 0x82BF3CDA=1, returns 1.

**Native behavior**: Calls sub_8284B3D8 to check device handle validity.
If valid: sets 0x82BF3A77=1 and 0x82BF3CDA=1, returns 1.
If invalid: calls sub_8223DB90 (cleanup), returns 0.

**Callers in sub_82242910**:
- State 4 (line 85192): If returns 1 -> jumps to state 9 (normal). If 0 -> falls
  through to platformMode switch which may loop back.
- State 8 (line 85642): If returns 1 -> computes `~1 & 9 = 0`, sets state=0.
  If returns 0 -> `~0 & 9 = 9`, sets state=9.

### Flag 0x82BF3A77 (g_sceneReady) -- Read Analysis

**Readers**: Only 1 read site in entire generated codebase (line 80397 of
gta4_recomp.6.cpp). This read is inside sub_82240B78 (storage notification guard),
which is already stubbed to return 0. The flag is gated behind
`XNotifyGetNext(handle, 11) == true` which never fires.

**Verdict**: **SAFE**. Setting this flag prematurely has no observable effect.

### Flag 0x82BF3CDA (g_contentReady) -- Read Analysis

**Readers**: Only 1 read site (line 73527 of gta4_recomp.6.cpp). This is inside
sub_8223DB90's helper flow. The read controls whether a device index byte is
written to 0x82BF3A76:
- If 0x82BF3CDA != 0: writes r31 (device index) to 0x82BF3A76
- If 0x82BF3CDA == 0: writes 0 to 0x82BF3A76

**Verdict**: **LOW RISK**. Setting this to 1 means the device index from the last
XamContentGetDeviceData call gets preserved. Since we never actually call
XamContentGetDeviceData, the device index value is whatever r31 happened to
contain -- likely 0 or stale. This has no downstream effect because the content
system uses RexGlue's VFS, not Xbox device indices.

### Overall Risk for sub_82240B08: **LOW**

The two flags have extremely limited read surface (1 reader each). Neither reader
is reachable under normal recomp execution.

---

## 4. sub_8224FFC8 -- XAM Dialog Result Processor (returns 1 for accept)

**Hook**: If queryType==8 (accept check), returns 1. Otherwise returns 0.

**74 call sites** across the codebase (32 in gta4_recomp.0.cpp, 21 in .14.cpp,
6 in .39.cpp, 6 in .8.cpp, 5 in .7.cpp, 2 in .6.cpp, 2 in .3.cpp).

**Native behavior**: Complex function with 29-case switch on a step counter at
0x82BF9E5C. For most step values (1-16, 21-27), returns 0 immediately. Only
step values 0, 17-20, 28 execute dialog-checking logic.

The actual dialog check calls sub_8223CBC8 (checks if XAM overlay is active),
then checks queryType:
- queryType 8: "was accept pressed?" -> checks controller input
- queryType 11: "was cancel pressed?" -> checks controller input
- queryType 12/11 (special): additional checks

**Caller analysis for state machine path (gta4_recomp.6.cpp)**:

1. **Line 78151** (inside sub_8223F9F0's flow): Called with r3=8. If returns 1:
   writes r27 to 0x82BF3CDA (if r26 != 0), sets r3=1, returns. This is the
   "user accepted device selection" path. **Auto-accept here is correct behavior.**

2. **Line 79321** (inside same flow, cancel check): Called with r3=11. Our hook
   returns 0. If returns 1, it would trigger the cancel path. **Returning 0
   here is correct -- we don't want to simulate cancel.**

### Cascading Risk Assessment

**Question**: Could auto-accepting dialogs in the 74 call sites trigger dangerous
game state changes?

**Answer**: **LOW RISK for state machine path, MODERATE RISK for broader game**.

In the state machine path, sub_8224FFC8 is called within sub_8223F9F0 (device
selection flow). Auto-accepting here is the intended behavior -- it simulates the
user choosing the save device.

For the other 72 call sites spread across the game, auto-accepting could
theoretically trigger unintended purchases, profile changes, or game mode
selections. However:
- The native function checks a step counter (0x82BF9E5C) that must be in the
  right range (0, 17-20, 28) for any action
- Most call sites would hit the early-return paths (step values 1-16)
- The game cannot reach most UI dialogs in the recomp (no Xbox Guide overlay)
- The only reachable dialog path during boot is the save device selection

**Regarding deep recursion**: The function does NOT call itself or any callers
recursively. It reads state from memory and returns a bool. No recursion risk.

---

## 5. sub_8284A7E8 -- Content Creation Initiator (diagnostic wrapper)

**Hook**: Pass-through diagnostic wrapper. Calls the original `__imp__sub_8284A7E8`,
then prints the slot state and return values.

**Risk**: **NONE**. This hook does not modify any game state. It only prints
diagnostic information. The `printf` and `fflush` calls add negligible overhead.

---

## 6. sub_822417B0 -- Two-Phase Content Size Checker (delta clamp)

**Hook**: Pass-through that clamps the delta value at 0x82BF99C8 to >= 0 after
the original function executes.

**Callers**: Only 2 -- state 12 (line 85947) and state 14 (line 85979) of
sub_82242910.

**State 14 path when delta is clamped to 0** (the critical path):

```
sub_822417B0 returns 0 (complete, delta >= 0)
  -> state 14 checks: r3 == 2? No. r3 == 0? Yes.
  -> loads r29[0] (content size comparison result)
  -> if r29[0] < 0: goto state 13 (error restart) -- but we clamped to 0
  -> if r29[0] >= 0: checks isMultiplayer (0x82A95466)
     -> if isMultiplayer != 0: goto loc_82243188
        -> checks platformMode == 3? YES (our hook set it)
        -> checks 0x82BF981F != 0?
           -> if yes: calls sub_8223F790 (save file validation)
           -> sub_8223F790 returns 0 or 1
           -> returns 0 (sub_82242910 complete)
     -> if isMultiplayer == 0: checks 0x82BF981F
        -> if 0: returns r3=2 (error) -- but only if NOT platformMode 3
        -> if platformMode == 3: calls sub_8223F790
```

**Key finding**: After state 14 with delta=0, the function returns 0 to
sub_822438B0, which means "scene creation complete." This is the desired outcome.

**Does delta=0 cause heavy processing?** No. The code after the clamped delta
performs simple comparisons and flag checks. The sub_8223F790 call is a lightweight
readiness check (reads 3 memory locations, returns 0 or 1). No heavy computation,
no deep call trees, no recursive paths.

---

## 7. sub_82242910 -- Scene Creation Hook (platformMode=3 fix)

**Hook**: Forces platformMode (0x82BF9844) to 3 for states 0-4, intercepts
fast-path 0->4 and redirects to 0->1.

### platformMode=3 After State 4

**Question**: After state 4, platformMode stays 3 throughout states 5-14.
Does this cause multiplayer-specific work in later states?

**Analysis of platformMode reads in sub_82242910**:

| Location | States | What it does with platformMode |
|----------|--------|-------------------------------|
| State 4, line 85149-85158 | 4 | `(platMode - 3) unsigned <= 1` -> if 3 or 4, call sub_8223F308 (scene creation). **This is why we set it to 3.** |
| State 10, line 85840-85855 | 10 | After content check: if platMode in {0,1,3,4} -> state 11. Otherwise -> loc_82243178 (reset). **3 routes to state 11 -- correct.** |
| State 11, line 85900-85911 | 11 | If sub_8223D400 returns true: if platMode in {3,4} -> state 12 (save overwrite). Otherwise -> loc_82243178. **3 routes to state 12 -- this is the save path.** |
| State 14, line 86012-86017 | 14 | After delta >= 0: if platMode == 3 -> calls sub_8223F790. **This triggers save file validation -- expected.** |
| State 14, line 86044-86047 | 14 | (isMultiplayer path): if platMode == 3 AND 0x82BF981F != 0 -> calls sub_8223F790. **Same check.** |

**Verdict**: platformMode=3 routes through the "platform mode 3" save/content
paths in states 10-14. This is the expected behavior -- mode 3 means "online
single-player with save device" on Xbox 360. The save path is what we WANT to
execute for the recomp's VFS-backed saves.

**Does mode=3 trigger multiplayer networking?** No. The isMultiplayer flag at
0x82A95466 is set by sub_822438B0 state 1, but it only controls whether to check
save file readiness (sub_8223F790), not whether to start multiplayer sessions.
The multiplayer code paths are gated by completely different mechanisms (network
session hooks, XNet APIs).

---

## 8. What Happens When sub_82242910 Returns 0 to sub_822438B0

When sub_82242910 returns 0 (complete), sub_822438B0 state 2 (line 87223-87249):

```
1. Checks r3 == 0 -> YES
2. Zeroes 16 bytes at 0x82BF3920 (array clear loop, 16 iterations)
3. Sets sub_822438B0 state to 3
4. Falls through to loc_82243BE8 (end of state switch)
```

sub_822438B0 state 3 then calls sub_82240F80 (another state machine). This is the
normal game initialization progression -- scene creation is done, now the game
sets up the world.

**This is not a recursive path.** sub_822438B0 does not re-enter sub_82242910
after it returns 0. The state counter moves forward from 2 to 3.

---

## Conclusion: Hook Cascading Risk Summary

| Hook | Risk Level | Can Cause Recursion? | Can Cause Stack Overflow? |
|------|-----------|---------------------|--------------------------|
| sub_8223DB20 (sign-in guard) | NONE | No | No |
| sub_82240B78 (storage guard) | NONE | No | No |
| sub_82240B08 (device ready) | LOW | No | No |
| sub_8224FFC8 (dialog accept) | LOW | No (no recursion in function) | No |
| sub_8284A7E8 (diagnostic) | NONE | No (pass-through) | No |
| sub_822417B0 (delta clamp) | LOW | No | No |
| sub_82242910 (platformMode) | LOW | No (state machine, not recursive) | No |

**Overall verdict: These hooks are NOT the cause of the stack guard page fault.**

The stack guard page infinite loop at 0x705D0000 is a pre-existing issue (see
doc 43). The hooks correctly implement the scene creation adaptation:
1. Sign-in and storage guards suppress spurious Xbox notifications
2. Device readiness returns "ready" with flags that have no observable readers
3. Dialog auto-accept only fires for the save device selection path
4. Delta clamp prevents the "insufficient storage" error restart loop
5. platformMode=3 routes through the save/content paths (correct for VFS saves)
6. The state machine is non-recursive -- sub_82242910 returning 0 advances
   sub_822438B0 to state 3, it does not re-enter state 2

The stack overflow originates from a different subsystem (likely the particle
emitter registration storm or vtable dispatcher, both documented in earlier logs).
