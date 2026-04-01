# 13: Ready-Signal Infinite Loop Analysis

## Summary

The ready-signal at `0x82BF9B70` oscillates `1->2->1->2` indefinitely because
`sub_82254FE0` (XAM dialog setup) unconditionally overwrites the counter back
to `1` every frame, defeating the increment performed by `sub_8214C8C8`.

---

## Key Memory Addresses

| Address        | Size | Description                                      |
|----------------|------|--------------------------------------------------|
| `0x82BF9B70`   | u32  | Ready flag (dialog counter: -1=idle, 1-3=active, >=4=done) |
| `0x82BF9B74`   | u32  | Dialog type parameter (often 72)                 |
| `0x82BF9B6A`   | u8   | Dialog flag byte (r6 param)                      |
| `0x82BF9B6C`   | u32  | Dialog timeout/param (r7, often -1)              |
| `0x82BF9B69`   | u8   | Dialog flag byte (r8 param)                      |
| `0x82BFA13C`   | u32  | Step counter for sub_8214B640 state machine      |
| `0x82A97CF4`   | u32  | Mode flag: 1 = use second switch in sub_8214B640 |

---

## Call Chain

```
sub_82142230 (frontend state machine, runs in loop)
  -> sub_821428C8 (per-frame update, case 3 handler)
       -> sub_8214C8C8 (ready counter: increments 0x82BF9B70)
            -> sub_8214B640 (giant state machine: switch on 0x82BFA13C)
                 -> sub_82254FE0 (XAM dialog setup: writes 1 to 0x82BF9B70)
```

Also called from `sub_82142F90` -> `sub_8214C8C8` (same chain, different entry).

---

## Function Analysis

### sub_8224FA48 -- Readiness Check

**Location**: `gta4_recomp.7.cpp:16178`
**Semantics**: Returns `1` (TRUE) if `0x82BF9B70 != -1`, returns `0` (FALSE) if `== -1`.

```
r11 = load(0x82BF9B70)
r11 = r11 + 1
r11 = clz(r11)          // clz(0) = 32, clz(anything_else) < 32
r11 = (r11 >> 5) & 1    // 1 only if clz==32, i.e., original value was -1
return r11 ^ 1           // invert: -1 -> FALSE, anything else -> TRUE
```

### sub_8224FA38 -- Ready Reset

**Location**: `gta4_recomp.7.cpp:16164`
**Semantics**: Writes `-1` to `0x82BF9B70`. Called when counter reaches `>= 4` to signal completion.

### sub_8214C8C8 -- Ready Counter (Incrementer)

**Location**: `gta4_recomp.0.cpp:30455`

Pseudocode:
```c
void sub_8214C8C8() {
    if (sub_8224FA48()) {           // if 0x82BF9B70 != -1
        int val = load(0x82BF9B70);
        if (val >= 4) {
            sub_8224FA38();         // reset to -1 (completion)
        } else {
            store(0x82BF9B70, val + 1);  // increment
        }
    }
    sub_8214B168();                 // post-ready processing
    sub_8214B640();                 // main state machine
    // ... continues with more logic after sub_8214B640 returns
}
```

### sub_82254FE0 -- XAM Dialog Setup

**Location**: `gta4_recomp.7.cpp:29074`

Pseudocode:
```c
void sub_82254FE0(r3=player, r4=title_str, r5=type, r6=flag, r7=timeout,
                   r8=flag2, r9=desc_str, r10=extra_str) {
    store_u32(0x82BF9B70, 1);      // <-- THE PROBLEMATIC WRITE (always 1)
    sub_8224CD48(r3);               // copy r3 string to buffer at 0x82BF9D58
    sub_8224CD88(r4);               // copy r4 string to buffer at 0x82BF9CF8
    store_u32(0x82BF9B74, r5);      // dialog type
    store_u8(0x82BF9B6A, r6);       // flag
    store_u32(0x82BF9B6C, r7);      // timeout
    store_u8(0x82BF9B69, r8);       // flag2
    sub_8224CDC8(r9);               // copy desc string to buffer at 0x82BF9C98
    sub_8224CE08(r10);              // copy extra string to buffer at 0x82BF9C38
    if (byte_at(0x82BFA164) != 0) {
        sub_82252D08(0x82BFA148, 0, 255, 0);  // clear some overlay buffer
    }
}
```

The four string-copy helpers (`sub_8224CD48/88/C8/CE08`) each copy a NUL-terminated
string into fixed buffers within the dialog struct at `0x82BF9B58`-`0x82BFA168`.

### sub_8214B640 -- Main State Machine

**Location**: `gta4_recomp.0.cpp:27784`
**Size**: ~2667 lines of generated code (27784-30451)

Structure:
1. Calls `sub_822BCA90` (lock/sync)
2. Reads mode flag at `0x82A97CF4`
3. **If mode != 1**: First switch on `(0x82BFA13C - 1)`, handles steps 0-14 + 32
   - Case 0 (step=1): Initial setup, calls `sub_826C4CA8`, `sub_82252D08`, etc.
   - Case 7 (step=8): Checks byte at `0x82BFA148`, calls `sub_8214A690`
   - Case 10 (step=11): Sets up overlay with string, calls `sub_8223CFC0`
   - Case 12 (step=13): Calls `sub_82168370` (network?), sets up display
   - Case 14 (step=15): Stores a byte flag
   - Most cases (1-6, 8-9, 11, 15-31): Fall through to `loc_8214C8AC` (exit)
4. **If mode == 1**: Second switch on `(0x82BFA13C - 2)`, handles steps 2-32
   - **Steps 17-20 (cases 15-18)**: The problematic ones -- call `sub_82254FE0`
   - Steps 9, 10, 12, 29: Various processing
   - Steps 21-30: Additional dialog/UI setup

The four `sub_82254FE0` calls within sub_8214B640 (all in mode=1 second switch):

| Call | Return addr  | Approx step | Context                              |
|------|-------------|-------------|--------------------------------------|
| 1    | 0x8214BEA4  | 18          | After `sub_8221A300` (get player obj)|
| 2    | 0x8214BFBC  | 17          | After clearing a flag byte           |
| 3    | 0x8214C06C  | 19          | After clearing a flag byte           |
| 4    | 0x8214C144  | 20          | After `sub_8221A300` (get player obj)|

All four calls pass nearly identical arguments: `r3=0, r5=72, r6=0, r7=-1, r8=0`
with varying string pointers in `r4` (title), `r9` (desc), `r10` (extra).

---

## The Infinite Loop Mechanism

### Intended flow on Xbox 360

```
Frame 0:  0x82BF9B70 = -1 (idle)
          sub_8214C8C8: sub_8224FA48() returns FALSE, no increment
          sub_8214B640: step reaches case 18
          sub_82254FE0: writes 1, launches XAM modal dialog
          XAM dialog blocks / processes asynchronously

Frame 1:  0x82BF9B70 = 1 (dialog active)
          sub_8214C8C8: increments 1 -> 2
          XAM dialog still active, step 18 case checks dialog status
          Dialog NOT complete: step stays at 18 but does NOT call sub_82254FE0 again
          (The case code checks dialog completion before re-invoking)

Frame N:  User dismisses dialog -> completion callback advances step to 19
          0x82BF9B70 increments 2 -> 3 -> 4
          sub_8224FA38 resets to -1 (completion)
          Next call: step=19, different case, flow continues
```

### Broken flow in recompilation

```
Frame 0:  0x82BF9B70 = -1 (idle)
          sub_8214C8C8: sub_8224FA48() returns FALSE, no increment
          sub_8214B640: step reaches case 18
          sub_82254FE0: writes 1 (no actual XAM dialog -- stubbed)

Frame 1:  0x82BF9B70 = 1
          sub_8214C8C8: increments 1 -> 2
          sub_8214B640: step STILL 18 (nothing advanced it)
          sub_82254FE0: writes 1 AGAIN (overwrites the 2!)

Frame 2:  0x82BF9B70 = 1 (not 2!)
          sub_8214C8C8: increments 1 -> 2
          sub_8214B640: sub_82254FE0 writes 1 again
          INFINITE LOOP: 1 -> 2 -> 1 -> 2 -> ...
```

The counter NEVER reaches 4 because `sub_82254FE0` resets it to 1 every frame.

---

## Root Cause

Two interacting problems:

1. **XAM dialogs are not implemented**: `sub_82254FE0` sets up dialog parameters in
   memory but no actual UI is shown. Without a real dialog, the completion callback
   never fires and the step counter at `0x82BFA13C` never advances.

2. **`sub_82254FE0` unconditionally writes 1**: Even though `sub_8214C8C8` incremented
   the counter to 2, `sub_82254FE0` overwrites it back to 1. On Xbox 360, this is fine
   because `sub_82254FE0` is only called ONCE per dialog invocation (the step advances
   on completion, preventing re-entry into the same case).

---

## Other Callers of sub_82254FE0

`sub_82254FE0` has 42 call sites across the codebase:

| File             | Call count | Notable callers                    |
|------------------|-----------|-------------------------------------|
| gta4_recomp.0.cpp| 8         | sub_8214B640 (4), sub_82144C30, sub_82149D08, sub_8214D380 |
| gta4_recomp.6.cpp| 30        | sub_8223F9F0 (XAM dialog flow - many calls) |
| gta4_recomp.14.cpp| 1        | Unknown function                    |
| gta4_recomp.39.cpp| 3        | Unknown function                    |

The heaviest caller is `sub_8223F9F0` in `gta4_recomp.6.cpp` with ~30 call sites,
which is the main XAM dialog dispatch function.

---

## Correct Hook Strategy for sub_82254FE0

### Option A: One-shot per dialog (skip if already active)

```cpp
PPC_FUNC_HOOK(sub_82254FE0) {
    uint32_t current = PPC_LOAD_U32(0x82BF9B70);
    if (current != -1u) {
        // Already active -- do NOT overwrite the counter back to 1
        // Just return without calling the original
        return;
    }
    // First call: set up dialog and write 1
    __imp__sub_82254FE0(ctx, base);
}
```

This prevents the re-write of 1 when the counter has already been incremented past 1.
The flow would then be: `sub_82254FE0` writes 1 (once), `sub_8214C8C8` increments
`1->2->3->4`, `sub_8224FA38` resets to -1, step advances.

**Problem**: The step at `0x82BFA13C` still never advances because there is no dialog
completion callback. The counter would reach 4, reset to -1, but the same step case
would trigger `sub_82254FE0` again, creating a slower loop: `-1->1->2->3->4->-1->1->...`

### Option B: Instant completion (skip entirely, advance step)

```cpp
PPC_FUNC_HOOK(sub_82254FE0) {
    // Set up dialog parameters (for any code that reads them)
    __imp__sub_82254FE0(ctx, base);
    // Immediately mark as completed by writing 4
    PPC_STORE_U32(0x82BF9B70, 4);
}
```

This makes `sub_8214C8C8` see `>= 4` and call `sub_8224FA38` (reset to -1) on the
next frame. But the STEP at `0x82BFA13C` still needs to advance.

### Option C: Skip dialog + advance step (full bypass)

```cpp
PPC_FUNC_HOOK(sub_82254FE0) {
    // Do NOT call original -- avoid writing 1
    // Read current step and advance it past the dialog case
    uint32_t step = PPC_LOAD_U32(0x82BFA13C);
    PPC_STORE_U32(0x82BFA13C, step + 1);
    // Mark dialog as already completed
    PPC_STORE_U32(0x82BF9B70, 4);
}
```

**Risk**: Blindly incrementing the step may skip important initialization done in
intermediate cases. The step counter is shared across all dialog invocations in the
state machine, so advancing it incorrectly could desync the flow.

### Recommended: Option A + step advance in sub_8214C8C8

The safest approach combines:
1. Make `sub_82254FE0` one-shot (Option A) to stop the counter reset
2. In `sub_8214C8C8`, when `0x82BF9B70` transitions from `>= 4` to `-1` (completion),
   also advance `0x82BFA13C` by 1 to simulate dialog dismissal

This preserves the dialog parameter setup for any code that reads those buffers while
correctly advancing the state machine.

---

## Existing Hooks (in imports.cpp)

The current hooks are **diagnostic only** -- they log but do not modify behavior:

- `sub_82254FE0`: Logs entry/exit, prints arguments and final counter value
- `sub_8224FA38`: Logs first 10 resets to -1
- `sub_8214C8C8`: Logs counter transitions, sampled every 500th call
- `sub_8214B168`: Logs first 5 post-ready results, sampled every 500th

No hook currently prevents the re-entry loop.

---

## File Locations

| File | Content |
|------|---------|
| `gta4_recomp.0.cpp:27784-30451` | sub_8214B640 (state machine) |
| `gta4_recomp.0.cpp:30455-30558+` | sub_8214C8C8 (incrementer) |
| `gta4_recomp.7.cpp:29074-29167` | sub_82254FE0 (dialog setup, writes 1) |
| `gta4_recomp.7.cpp:16164-16174` | sub_8224FA38 (reset to -1) |
| `gta4_recomp.7.cpp:16178-16194` | sub_8224FA48 (readiness check) |
| `gta4_recomp.0.cpp:5305-6277` | sub_82142230 (frontend SM, top-level) |
| `gta4_recomp.0.cpp:6281-?` | sub_821428C8 (calls sub_8214C8C8) |
| `imports.cpp:1988-2002` | Existing diagnostic hook for sub_82254FE0 |
| `imports.cpp:2004-2015` | Existing diagnostic hook for sub_8224FA38 |
| `imports.cpp:2017-2030` | Existing diagnostic hook for sub_8214C8C8 |

All paths are relative to `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/` for recomp files and `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/` for hooks.
