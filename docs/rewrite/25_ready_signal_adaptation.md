# 25: Ready-Signal Platform Adaptation

## Summary

The ready-signal system at `0x82BF9B70` oscillates `1->2->1` indefinitely because
Xbox Guide (XAM) dialog acknowledgment never fires. This document identifies the
true root cause, maps the complete dialog flow, and proposes a two-hook solution
that preserves all game logic while making dialogs complete naturally on PC.

---

## Root Cause (Revised)

Prior analysis (doc 13) identified `sub_82254FE0` resetting the counter as the problem.
This is a **symptom**, not the root cause. The actual root cause is:

**`sub_8224FFC8` (dialog result processor) always returns 0** because no XAM overlay
exists to be acknowledged. This prevents `sub_8224EFE8` (step setter) from advancing
the step counter at `0x82BFA13C`, which means `sub_8214B640` re-enters the same dialog
case every frame, calling `sub_82254FE0` again and resetting the ready signal to 1.

### The Real Call Chain

```
sub_8214C8C8 (per-frame)
  |
  +-- sub_8224FA48()          checks 0x82BF9B70 != -1
  +-- increment 0x82BF9B70   (1 -> 2 ... -> 4 -> reset to -1)
  +-- sub_8214B640()          state machine on step counter 0x82BFA13C
       |
       +-- [step 17-20, mode=1 second switch]
       |     |
       |     +-- sub_82254FE0()     writes 1 to 0x82BF9B70 (dialog setup)
       |     +-- sub_8224FFC8()     dialog result check: returns 0 (*)
       |     |     |
       |     |     +-- sub_8223CBC8()   checks 0x82BF9834 (scene state)
       |     |     +-- sub_8224EC60()   checks button press on pad
       |     |     +-- [long processing path for XAM overlay data]
       |     |
       |     +-- IF sub_8224FFC8 returned 1:
       |     |     sub_8224EFE8(next_step)  <-- ADVANCES step counter
       |     +-- IF returned 0:
       |           exit case, step unchanged, SAME CASE NEXT FRAME
       |
       +-- (*) because sub_8223CBC8 returns 0 (no XAM overlay)
             and sub_8224EC60 returns 0 (no button press)
```

---

## Function Map

### sub_8224FFC8 -- Dialog Result Processor (THE KEY FUNCTION)

**Address**: `0x8224FFC8`
**File**: `gta4_recomp.7.cpp:17007`
**Call sites**: 68 across the codebase
**Semantics**: Checks if an XAM dialog has been acknowledged by the user

**Arguments**:
- `r3` (saved as `r31`): Dialog type ID (8, 11, 12, 22, etc.)
- `r4` (saved as `r22`): Sub-type
- `r5` (saved as `r30`): Flags bitmask (bit 1 = skip ready check, bit 2 = ?, bit 3 = ?)
- `r6`: Condition flag
- `r7` (saved as `r25`): Extra param
- `r8` (saved as `r26`): Extra param

**Return**: `r3 = 1` (dialog acknowledged) or `r3 = 0` (still pending)

**Internal logic**:
1. Check `0x82BF9B70` (ready signal): if == -1, go to dialog check
2. Check bit 1 of r5: if set, also go to dialog check
3. At dialog check:
   - If r6 != 0 OR step counter `0x82BFA13C` matches steps 0, 17-20: call `sub_8223CBC8`
   - `sub_8223CBC8` checks `0x82BF9834` (scene dispatch state) and byte `0x82BF3B17`
   - If `sub_8223CBC8` returns 1 AND additional flag/button checks pass via `sub_8224EC60`
   - Then enters long processing path (reads XAM overlay string buffers)
   - Returns 1
4. Otherwise: returns 0

**Why it fails in recomp**: `sub_8223CBC8` returns 0 because `0x82BF9834` is 0 (no
scene dispatch has stored a value there yet) and byte `0x82BF3B17` is 0 (no XAM
overlay activation). Even if those passed, `sub_8224EC60` checks for button presses
on the controller pad to dismiss the dialog -- no button is ever pressed.

### sub_8224EFE8 -- Step Counter Setter

**Address**: `0x8224EFE8`
**File**: `gta4_recomp.7.cpp:14641`
**Semantics**: `store(0x82BFA13C, r3); return 1;`

Takes a step value in r3 and stores it to the step counter. Called by callers of
`sub_8224FFC8` when the dialog is acknowledged to advance the state machine.

### sub_82254FE0 -- Dialog Setup (writes 1)

**Address**: `0x82254FE0`
**File**: `gta4_recomp.7.cpp:29074`
**Call sites**: 42 (8 in gta4_recomp.0.cpp, 30+ in gta4_recomp.6.cpp, others)
**Semantics**: Sets up XAM dialog parameters and writes 1 to `0x82BF9B70`

### sub_8223CBC8 -- Dialog Active Check

**Address**: `0x8223CBC8`
**File**: `gta4_recomp.6.cpp:71144`
**Semantics**: Returns 1 if `0x82BF9834 != 0` OR `0x82BF3B17 != 0`, else 0

### sub_8224EC60 -- Button Press Check

**Address**: `0x8224EC60`
**File**: `gta4_recomp.7.cpp:14082`
**Semantics**: Reads pad button state at player_obj+3108 and +3110, returns 1 if XOR != 0

---

## Step Transitions in sub_8214B640 (mode=1)

When `0x82A97CF4 == 1` (mode flag), sub_8214B640 uses its second switch on
`(0x82BFA13C - 2)`. The dialog cases and their transitions:

| Current Step | Case | Dialog Action | sub_8224FFC8 first arg | On acknowledge (step=) | On cancel (step=) |
|-------------|------|---------------|------------------------|----------------------|-------------------|
| 18 | 16 | sub_82254FE0 (type 72, player dialog) | 8 then 11 | 21 | 9 |
| 17 | 15 | sub_82254FE0 (type 72, cleared flag) | 8 then 11 | 9* | 9 |
| 19 | 17 | sub_82254FE0 (type 72, cleared flag) | 8 then 11 | 23 | 9 |
| 20 | 18 | sub_82254FE0 (type 72, player dialog) | 8 then 11 | 22 | 9 |

*Step 17 goes to step 9 on either path (calls sub_8214D700 cleanup on the "ok" path).

### Dual sub_8224FFC8 Pattern

Each dialog case calls `sub_8224FFC8` TWICE:
1. First with `r3=8`: the "OK/Accept" check
2. Second with `r3=11`: the "Cancel/Back" check

If the first returns 1: advance to "ok" step, exit.
If the second returns 1: advance to "cancel" step (usually step 9), exit.
If neither returns 1: exit case with step unchanged (re-enter same case next frame).

---

## Step Counter Stores (sub_8223F9F0 and related)

The step counter `0x82BFA13C` is written by `sub_8224EFE8(value)` and by direct stores
in `gta4_recomp.7.cpp`. Key stores found:

| Value Written | Location | Context |
|--------------|----------|---------|
| r3 (variable) | sub_8224EFE8 (line 14648) | Generic step setter |
| 0 | line 16919 | System reset (also clears mode flag) |
| 2 | lines 14727, 37037 | Dialog chain start |
| 17 | line 37256 | Set by sub_8223F9F0 |
| 18 | line 37096 | Set by sub_8223F9F0 |
| 19 | line 37244 | Set by sub_8223F9F0 |
| 20 | lines 37073, 37117 | Set by sub_8223F9F0 |
| 26 | line 38139 | Set by sub_8223F9F0 |
| 29 | lines 36862, 36916 | Set by sub_8223F9F0 |
| 30 | line 37232 | Set by sub_8223F9F0 |

`sub_8223F9F0` is the XAM dialog dispatch function that determines which dialog
sequence to show. It sets the initial step value, then `sub_8214B640` handles
each step. The step counter is the thread that links them.

---

## Proposed Hook Strategy

### Approach: Hook sub_8224FFC8 to Auto-Acknowledge

Since `sub_8224FFC8` represents "has the user acknowledged this Xbox Guide dialog?",
and on PC there is no Xbox Guide, the correct platform adaptation is to make it
always return 1 (acknowledged). This is semantically correct: the user is never
shown an Xbox dialog, so the answer is "yes, proceed."

This single hook solves the entire problem:
1. `sub_82254FE0` writes 1 to `0x82BF9B70` (dialog setup runs normally)
2. `sub_8224FFC8` immediately returns 1 (dialog acknowledged)
3. Caller calls `sub_8224EFE8(next_step)` to advance the step counter
4. Next frame: `sub_8214C8C8` increments `0x82BF9B70` from 1 to 2
5. `sub_8214B640` enters the NEXT step case (not the same one)
6. No repeated `sub_82254FE0` call, no counter reset
7. After 3 more frames: counter reaches 4, `sub_8224FA38` resets to -1
8. Ready signal complete

### Why This Preserves Game Logic

- `sub_82254FE0` still runs (dialog parameter buffers are populated)
- `sub_8224EFE8` still sets the step counter via the normal code path
- The step counter advances through the exact sequence the game expects
- The ready signal reaches 4 and resets to -1 via the normal incrementer
- No state machine bypasses, no counter pre-writes, no skipped states

### Handling the Dual-Call Pattern

Each dialog case calls `sub_8224FFC8` twice (first=8/ok, second=11/cancel).
If we always return 1, the FIRST call (ok) will succeed and advance the step.
The caller then jumps past the second call. This is the "auto-accept" behavior.

For cases where we might want "auto-cancel" instead (e.g., marketplace prompts),
we could check the first arg (r3) and return 0 for `r3=8` (ok) and 1 for `r3=11`
(cancel). But for initial boot-up, auto-accept is correct.

---

## Concrete Hook Code

### Option 1: Simple Auto-Acknowledge (Recommended for Initial Fix)

```cpp
// sub_8224FFC8 -- XAM Dialog Result Processor
// On Xbox: checks if user has acknowledged an XAM overlay dialog
// On PC: no XAM overlays exist, so auto-acknowledge immediately
// This allows sub_8214B640's step counter to advance naturally.
extern "C" void __imp__sub_8224FFC8(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_dialogAckCount{0};
PPC_FUNC_HOOK(sub_8224FFC8) {
    int n = s_dialogAckCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 20 || (n % 1000) == 0) {
        printf("[DIALOG-ACK] sub_8224FFC8 #%d auto-acknowledge "
               "type=%d sub=%d flags=0x%X step=0x%X ready=0x%08X\n",
               n, ctx.r3.s32, ctx.r4.s32, ctx.r5.u32,
               PPC_LOAD_U32(0x82BFA13C), PPC_LOAD_U32(0x82BF9B70));
        fflush(stdout);
    }
    // Auto-acknowledge: return 1 (dialog accepted)
    ctx.r3.s64 = 1;
}
```

### Option 2: Auto-Acknowledge with OK/Cancel Discrimination

```cpp
// sub_8224FFC8 -- XAM Dialog Result Processor
// Auto-acknowledges with discrimination between OK (type 8) and Cancel (type 11).
// Some dialog sequences may need cancel instead of ok.
extern "C" void __imp__sub_8224FFC8(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_dialogAckCount{0};
PPC_FUNC_HOOK(sub_8224FFC8) {
    uint32_t dialog_type = ctx.r3.u32;
    uint32_t step = PPC_LOAD_U32(0x82BFA13C);

    int n = s_dialogAckCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 30 || (n % 1000) == 0) {
        printf("[DIALOG-ACK] sub_8224FFC8 #%d type=%d step=%d ready=0x%08X\n",
               n, dialog_type, step, PPC_LOAD_U32(0x82BF9B70));
        fflush(stdout);
    }

    // On PC, auto-accept OK dialogs (type 8) and auto-dismiss cancel (type 11).
    // This matches the behavior of a user pressing A on Xbox.
    if (dialog_type == 8) {
        ctx.r3.s64 = 1;  // Accept
    } else if (dialog_type == 11) {
        ctx.r3.s64 = 0;  // Don't trigger cancel path
    } else {
        ctx.r3.s64 = 1;  // Default: acknowledge
    }
}
```

### Option 3: Conservative with Fallback to Original

```cpp
// sub_8224FFC8 -- XAM Dialog Result Processor
// Tries original logic first; if it returns 0 for too many frames on the same
// step, forces acknowledgment to prevent infinite loops.
extern "C" void __imp__sub_8224FFC8(PPCContext &ctx, uint8_t *base);
static uint32_t s_lastStep = 0xFFFFFFFF;
static int s_sameStepFrames = 0;
static constexpr int MAX_DIALOG_WAIT_FRAMES = 3;
PPC_FUNC_HOOK(sub_8224FFC8) {
    uint32_t step = PPC_LOAD_U32(0x82BFA13C);

    // Let original logic run
    __imp__sub_8224FFC8(ctx, base);

    // If original returned 1, accept it
    if (ctx.r3.u32 != 0) {
        s_lastStep = 0xFFFFFFFF;
        s_sameStepFrames = 0;
        return;
    }

    // Track how many frames we've been stuck on the same step
    if (step == s_lastStep) {
        s_sameStepFrames++;
    } else {
        s_lastStep = step;
        s_sameStepFrames = 1;
    }

    // If stuck for too long, force acknowledgment
    if (s_sameStepFrames >= MAX_DIALOG_WAIT_FRAMES && step >= 17 && step <= 20) {
        printf("[DIALOG-ACK] Force-acknowledging dialog at step %d "
               "after %d frames\n", step, s_sameStepFrames);
        fflush(stdout);
        ctx.r3.s64 = 1;
        s_sameStepFrames = 0;
    }
}
```

---

## Hook Registration

Add to `imports.cpp` hook registration table:

```cpp
{ 0x8224FFC8, sub_8224FFC8 },  // XAM dialog result processor (auto-acknowledge)
```

### Hooks to REMOVE or KEEP

The existing diagnostic hooks should remain but can be simplified once the fix works:

| Hook | Action |
|------|--------|
| `sub_82254FE0` (0x82254FE0) | KEEP diagnostic. No longer loops, so logging is minimal. |
| `sub_8224FA38` (0x8224FA38) | KEEP diagnostic. Will now fire naturally when counter reaches 4. |
| `sub_8214C8C8` (0x8214C8C8) | KEEP diagnostic. Will now show normal progression: 1->2->3->4->-1. |
| `sub_8223F9F0` (0x8223F9F0) | KEEP diagnostic. Shows which dialog sequence was dispatched. |

No changes needed to `sub_82254FE0` hook -- once `sub_8224FFC8` returns 1, the
step counter advances and `sub_82254FE0` is only called once per step (not
every frame).

---

## Recommendation

**Use Option 2** (Auto-Acknowledge with OK/Cancel Discrimination).

Rationale:
- Option 1 is simplest but may trigger both OK and Cancel paths simultaneously
  (the second `sub_8224FFC8(11,...)` call would also return 1)
- Option 2 correctly simulates "user pressed Accept" on every dialog
- Option 3 is overly complex and adds frame-counting state

With Option 2, the flow becomes:

```
Frame 0: 0x82BF9B70 = -1 (idle)
  sub_8214C8C8: sub_8224FA48 returns 0, no increment
  sub_8214B640: step=18, calls sub_82254FE0 (writes 1)
  sub_8224FFC8(8, ...): hook returns 1 (auto-accept)
  sub_8224EFE8(21): step advances to 21
  sub_8224FFC8(11, ...): hook returns 0 (skip cancel path)

Frame 1: 0x82BF9B70 = 1
  sub_8214C8C8: increments 1 -> 2
  sub_8214B640: step=21, different case (no sub_82254FE0 call)

Frame 2: 0x82BF9B70 = 2
  sub_8214C8C8: increments 2 -> 3

Frame 3: 0x82BF9B70 = 3
  sub_8214C8C8: increments 3 -> 4
  sub_8224FA38: resets to -1 (COMPLETION)

All subsequent frames: 0x82BF9B70 = -1
  sub_8224FA48 returns 0 everywhere
  Content management functions proceed normally
```

---

## Impact on Other Call Sites

`sub_8224FFC8` has 68 call sites. All represent Xbox Guide overlay dialogs:

| File | Call Count | Context |
|------|-----------|---------|
| gta4_recomp.0.cpp | 28 | sub_8214B640 (state machine), sub_82142B10, sub_8214B168, sub_8214D380, sub_82153290, sub_8216BA00 |
| gta4_recomp.6.cpp | 2 | sub_8223F9F0 (XAM dialog dispatch), sub_82240448 |
| gta4_recomp.7.cpp | 4 | sub_82251F70, sub_82258E00 |
| gta4_recomp.8.cpp | 6 | sub_8229D3F0 (content system) |
| gta4_recomp.14.cpp | 21 | sub_8238C058, sub_8239AC00 (multiplayer UI) |
| gta4_recomp.39.cpp | 5 | sub_826C9200, sub_826CC568 (misc UI) |
| gta4_recomp.3.cpp | 2 | sub_821CD640 |

ALL of these are Xbox Guide overlay interactions. On PC, they should all
auto-acknowledge. There is no scenario where the recomp should wait for a
user to dismiss an Xbox Guide overlay that does not exist.

---

## Key Memory Addresses

| Address | Type | Purpose |
|---------|------|---------|
| `0x82BF9B70` | u32 | Ready signal counter (-1=idle, 1-3=active, >=4=done) |
| `0x82BFA13C` | u32 | Step counter for sub_8214B640 state machine |
| `0x82A97CF4` | u32 | Mode flag (1=use second switch in sub_8214B640) |
| `0x82BF9834` | u32 | Scene dispatch state (checked by sub_8223CBC8) |
| `0x82BF3B17` | u8 | XAM overlay activation flag (checked by sub_8223CBC8) |
| `0x82BFA119` | u8 | Dialog cleanup flag (byte at 0x82C00000-24279) |
| `0x82BFA0E4` | u32 | Game phase counter (compared against 3, 5, 9) |

---

## File Locations

| File | Content |
|------|---------|
| `gta4_recomp.7.cpp:17007-17300+` | sub_8224FFC8 (dialog result processor) |
| `gta4_recomp.7.cpp:14641-14653` | sub_8224EFE8 (step counter setter) |
| `gta4_recomp.7.cpp:29074-29167` | sub_82254FE0 (dialog setup, writes 1) |
| `gta4_recomp.7.cpp:14082-14121` | sub_8224EC60 (button press check) |
| `gta4_recomp.6.cpp:71144-71171` | sub_8223CBC8 (dialog active check) |
| `gta4_recomp.6.cpp:77881-78200+` | sub_8223F9F0 (XAM dialog dispatch) |
| `gta4_recomp.0.cpp:27782-30451` | sub_8214B640 (main state machine) |
| `gta4_recomp.0.cpp:30453-30558` | sub_8214C8C8 (ready counter incrementer) |
| `imports.cpp:1988-2030` | Existing diagnostic hooks |

All generated code paths relative to:
`/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/`

Hooks file:
`/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/imports.cpp`
