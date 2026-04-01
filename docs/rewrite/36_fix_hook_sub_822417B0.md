# 36. Fix: Hook sub_822417B0 to Clamp Storage Delta

## Problem

State 14 of `sub_82242910` calls `sub_822417B0(r4=0)` to poll content size
verification. After the poll completes (return 0), state 14 reads the int32 at
`0x82BF99C8`. If negative, it transitions to state 13 (error recovery / "not
enough storage" dialog). In the recomp, this negative value causes an unwanted
error loop.

## Root Cause: How 0x82BF99C8 Becomes Negative

`sub_822417B0` (poll path) calls `sub_8284ADA0`, which:

1. Opens the save content file via `rexcrt_CreateFileA`
2. Gets its on-disk size -> `slot[144]` (actual_size)
3. Compares against `slot[136]` (expected_size, set during initiate)
4. If `expected > actual`:
   - Computes `needed = expected - actual` (bytes the save needs to grow)
   - Calls `sub_8284A078` to get available free space (bytes)
   - If `available >= needed`: writes 0 to `[r26]` (no deficit)
   - If `available < needed`: writes `(available_KB - needed_KB)` to `[r26]`
   - **This subtraction produces a NEGATIVE value** (deficit in KB)

The pointer `r26` inside `sub_8284ADA0` comes from the `r5` parameter, which
traces back through `sub_822417B0` to the `r7` parameter, which is `r29` in
`sub_82242910` = `0x82BF99C8`.

### Parameter Trace (Python-verified)

```
sub_82242910 state 14:
  r7 = r29 = 0x82C00000 + (-26168) = 0x82BF99C8

sub_822417B0 prologue (before poll branch):
  mr r5,r7   -> r5 = 0x82BF99C8

sub_822417B0 poll path (r4=0, loc_82241884):
  r5 unchanged, passed to sub_8284ADA0 as 3rd arg

sub_8284ADA0:
  r26 = r5 = 0x82BF99C8   (line 11668: mr r26,r5)
  stw r11,0(r26)           (line 11938/11959/11971: writes delta)
```

## State 14 Post-Poll Check (lines 85988-85993)

```c
// r3 == 0: poll complete
ctx.r11.u64 = PPC_LOAD_U32(0x82BF99C8);     // lwz r11,-26168(r11)
ctx.cr6.compare<int32_t>(ctx.r11.s32, 0);    // cmpwi cr6,r11,0
if (ctx.cr6.lt) goto loc_822431C8;           // blt -> state 13 (error)
```

If `[0x82BF99C8]` is negative (signed), state machine goes to state 13.
State 13 negates the value and shows a "need X KB free" dialog via
`sub_8223F9F0(r3=4, r4=-delta)`.

## All Readers/Writers of 0x82BF99C8

| Location | Operation | Context |
|----------|-----------|---------|
| sub_82242910 line 84852 | `addi r29,r11,-26168` | Address setup in prologue |
| sub_82242910 state 14 (line 85988) | `lwz r11,-26168(r11)` | Read delta after poll |
| sub_82242910 state 13 (line 86114) | `lwz r11,-26168(r11)` | Read delta for error dialog |
| sub_8284ADA0 line 11866 | `stw r29(=0),0(r26)` | Write 0 (sizes match) |
| sub_8284ADA0 line 11938 | `stw r11,0(r26)` | Write `available_KB - needed_KB` (can be negative) |
| sub_8284ADA0 line 11959 | `stw r11,0(r26)` | Write `(expected-actual) >> 10` (always positive) |
| sub_8284ADA0 line 11971 | `stw r11,0(r26)` | Write `(delta-1023) >> 10` (dead code from this path) |

**Only sub_82242910 (states 14 and 13) reads this address.** No other function
in the generated codebase references offset -26168 from 0x82C00000.

## Callers of sub_822417B0

Only TWO callers exist, both in `sub_82242910`:

| State | Line | r4 | Purpose |
|-------|------|----|---------|
| 12 (loc_822430B4) | 85947 | 1 | Initiate content size check |
| 14 (loc_822430F0) | 85979 | 0 | Poll content size check |

No other function calls sub_822417B0.

## State 12 Call Parameters

```
r3  = 0         (player index)
r4  = 1         (phase = INITIATE)
r5  = r28 = 0x82BF3A60  (content name buffer)
r6  = PPC_LOAD_U32(0x82BF99CC)  (size needed)
r7  = r29 = 0x82BF99C8  (output: delta KB pointer)
r8  = &sp[84]   (output: local var 1)
r9  = &sp[88]   (output: local var 2)
```

After return: if r3 == 2, error exit. Otherwise, set state = 14.

## State 14 Call Parameters

```
r3  = 0         (player index)
r4  = 0         (phase = POLL)
r5  = r28 = 0x82BF3A60  (content name buffer)
r6  = PPC_LOAD_U32(0x82BF99CC)  (size needed)
r7  = r29 = 0x82BF99C8  (output: delta KB pointer)
r8  = &sp[88]   (output: local var 2)  *** NOTE: r8/r9 SWAPPED vs state 12 ***
r9  = &sp[84]   (output: local var 1)
```

After return:
- r3 == 2: error exit (loc_82242AC4)
- r3 != 0 and r3 != 2: still working, re-enter state 14 (loc_82243250)
- r3 == 0: poll complete, check `[0x82BF99C8]` sign

## Proposed Hook

```cpp
extern "C" void __imp__sub_822417B0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_822417B0)
{
    // Call original two-phase content size check
    __imp__sub_822417B0(ctx, base);

    // After return, clamp the storage delta at 0x82BF99C8 to zero.
    // This tells the caller "storage is fine" regardless of the
    // actual size comparison result.
    //
    // In the recomp, save files are stored on the host filesystem
    // which has effectively unlimited space compared to Xbox 360's
    // STFS containers. The negative delta occurs because the expected
    // STFS allocation size doesn't match the actual file size on the
    // host VFS -- this is a false alarm, not a real space shortage.
    int32_t delta = static_cast<int32_t>(PPC_LOAD_U32(0x82BF99C8));
    if (delta < 0) {
        PPC_STORE_U32(0x82BF99C8, 0);
    }
}
```

### Why Clamp Instead of Unconditionally Zero?

We only clamp negative values. If the delta is 0 (sizes match) or positive
(extra space available), we leave it alone. This preserves the game's own
bookkeeping while only suppressing the false "not enough space" error.

### Why This Is Safe

1. **0x82BF99C8 has exactly TWO readers** -- state 14 (sign check) and state 13
   (error dialog KB display). Both are in the same function, `sub_82242910`.

2. **State 13 becomes unreachable.** With the delta clamped to >= 0, the
   `blt` branch in state 14 (line 85992) is never taken. State 13's error
   recovery path simply never executes.

3. **No downstream logic uses the delta magnitude.** After the sign check in
   state 14, the code proceeds to flag checks (`0x82A95466`, `0x82BF981F`)
   and save completion. The actual delta value is not used further.

4. **The hook is idempotent.** If `sub_8284ADA0` writes 0 (sizes match),
   the hook's clamp is a no-op. If it writes a positive value, the hook
   leaves it. Only negative (false deficit) values are corrected.

5. **sub_822417B0 has only 2 callers** (states 12 and 14). State 12 calls
   with r4=1 (initiate), which exits before `sub_8284ADA0` runs, so the
   address is not written on that path. The hook is effectively state-14-only.

## Comparison of Approaches

### Approach A: Hook sub_822417B0 (THIS DOCUMENT)

- **Target**: The two-phase content size check wrapper
- **Mechanism**: Clamp `[0x82BF99C8]` to >= 0 after original returns
- **Scope**: Only 2 callers, both in sub_82242910
- **Side effects**: State 13 error dialog never triggers (benign -- no real storage issues on host)
- **Risk**: LOW. Preserves all game logic; original function runs unmodified.
  Only the output interpretation changes.

### Approach B: Hook sub_8284ADA0

- **Target**: The content slot poll/size comparison function
- **Mechanism**: Force sizes to match (write `slot[144] = slot[136]` before comparison)
- **Scope**: Called by sub_822417B0 poll path AND potentially by sub_8284B010/others
- **Side effects**: Masks actual file size discrepancies
- **Risk**: MEDIUM. Modifying slot data could confuse downstream content operations
  (sub_8284AFE0 cleanup, sub_8284B2F8 multi-player cleanup). Those functions read
  slot[136] and slot[144] for their own logic.

### Approach C: Change XamContentCreateEx return value

- **Target**: The kernel-level content creation shim
- **Mechanism**: Force content operations to report specific sizes
- **Scope**: GLOBAL -- affects ALL XamContentCreateEx callers across the game
- **Side effects**: Could break DLC content, title update verification, multiplayer
  content enumeration, etc.
- **Risk**: HIGH. XamContentCreateEx is called by many subsystems. Changing its
  behavior has unpredictable cascading effects.

### Recommendation

**Approach A (hook sub_822417B0) is the safest.** It operates at the narrowest
scope (2 callers), preserves all internal game logic, and only corrects the
final output value. The original size comparison still runs (for any debugging
or logging purposes), but the false "insufficient storage" signal is suppressed.

## Implementation Checklist

1. Add hook to `LibertyRecomp/kernel/save_hooks.cpp`
2. Add `extern "C" void __imp__sub_822417B0(PPCContext& ctx, uint8_t* base);`
3. Define `PPC_FUNC(sub_822417B0)` with the clamp logic above
4. Rebuild: `cmake --build ./out/build/macos-release --target LibertyRecompLib && cmake --build ./out/build/macos-release --target LibertyRecomp`
5. Test: verify state 14 transitions past the sign check to save completion

## Key Source Locations

| File | Lines | Content |
|------|-------|---------|
| `gta4_recomp.6.cpp` | 82158-82373 | sub_822417B0 full implementation |
| `gta4_recomp.6.cpp` | 84814-86160 | sub_82242910 (state machine with states 12/13/14) |
| `gta4_recomp.6.cpp` | 85924-85955 | State 12 (initiate, lines up to state=14 transition) |
| `gta4_recomp.6.cpp` | 85956-86088 | State 14 (poll + delta sign check + state 13 transition) |
| `gta4_recomp.6.cpp` | 86089-86160 | State 13 (error recovery, reads delta for dialog) |
| `gta4_recomp.55.cpp` | 11651-11979 | sub_8284ADA0 (content slot poll, writes delta) |
| `save_hooks.cpp` | 1-363 | Existing save system hooks |
