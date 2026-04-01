# sub_82142230: Front-End State Machine Transition Map

Generated: 2026-03-28
Source: `gta4_recomp.0.cpp` lines 5305-6060, `imports.cpp` lines 1926-2396

---

## Register Constants (set once in prologue)

| Register | Value | Usage |
|-|-|-|
| r29 | 0 (init) | **Current state** (0-9) |
| r24 | 1 | Constant `1` |
| r25 | 0 | Constant `0` |
| r27 | 0x831E0000 | Player slot base |
| r28 | 0x82A90000 | Profile/episode base |
| r30 | 0x831D0000 | Flags/transition base |
| r26 | 0x831D0000 | XAM ready flags |
| r14 | 0x82C00000 | State variable base |
| r21 | 0x82C00000 | State variable base (alias) |
| r17 | 0x82A90000 | Scene dispatch base |
| r16 | 0x82B2F7E0 | Thread manager ptr |
| r22 | 0x82BCC1F8 | Streaming world base |

## Key Memory Addresses

| Address | Formula | Type | Purpose |
|-|-|-|-|
| 0x831E4DD4 | r27+19924 | u32 ptr | Player slot struct pointer (188 bytes) |
| 0x831D5330 | r30+21296 | u32 | Content transition counter (0/1/2) |
| 0x82A95474 | r28+21620 | u32 | Profile index |
| 0x82A95478 | r17+21624 | u32 | Episode/player index |
| 0x831D5327 | r26+21287 | u8 | XAM ready flag |
| 0x831D5348 | r30+21320 | u8 | Post-exit flag 1 (cleared) |
| 0x831D5337 | r30+21303 | u8 | Post-exit flag 2 (set/cleared) |
| 0x831D5324 | r30+21284 | u8 | Post-exit flag 3 (cleared) |
| 0x82BF9828 | r21-26584 | u32 | State 0 flag (cleared on sign-in) |
| 0x82BF9D81 | r14-25215 | u8 | DLC episode flag |

## State 0: Sign-In Check

**Function**: `sub_822414E8`
**Transition logic**:
- Returns 1 -> set r29=1 (advance), clear `0x82BF9828` to 0
- Returns 2 -> jump to `loc_821422FC` (fast path to state 3/4)
- Other -> loop (stay in state 0)

**Fast path (ret=2)**: Loads player slot from `[0x831E4DD4]`, calls `sub_82219AC0`, checks byte at slot+361. If nonzero -> r29=3, else r29=4.

## State 1: Storage Device Selection

**Function**: `sub_8223DDA8`
**Transition logic**:
- Returns 1 or 2 (i.e., `(r3-1) <= 1` unsigned) -> set r29=2
- Other -> loop

## State 2: Save/Load Check

**Function**: `sub_8223DEE8`
**Transition logic**:
- Returns 1 -> goto `loc_821422FC` (same fast path as state 0 ret=2: checks slot+361, sets r29=3 or r29=4)
- Returns 2 -> set r29=8, exit state machine
- Other -> loop

## State 3: Content Readiness Gate (COMPLEX)

**Functions**: `sub_8223CAD8`, `sub_821406C8` (player accessor), `sub_8224FA48`, `sub_821B6FD0`, `sub_826CBD20`, `sub_826CBD08`, `sub_8223DAA0`, `sub_822BCA90`, `sub_82215530`, `sub_8221B198`

**Structure**: This state has multiple sequential checks, not a single function call. Flow:

1. Load player slot from `[0x831E4DD4]`, call `sub_82219AC0`, check slot+361
   - If slot+361 == 0 -> r29=4 (skip to state 4, no player signed in)

2. If `[r30+21296]` (0x831D5330) == 0 -> set it to 1

3. Call `sub_8223CAD8` (cleanup), then `sub_821406C8` (get player slot ptr r31)
   - If r31 == NULL -> clear `0x831D5330` to 0, call `sub_8221B198` -> store to `0x82A95474`, set r29=4

4. Call `sub_8224FA48` -> if nonzero -> goto step 9

5. Check byte at `0x82BF9D81` (DLC flag) -> if nonzero -> goto step 9

6. Call `sub_821B6FD0(r16)` -> if nonzero -> goto step 9

7. **Newly-set transition detectors** (3 checks in sequence):
   - slot[68] != 0 AND slot[148] == 0 -> newly signed in -> goto step 8
   - slot[72] != 0 AND slot[152] == 0 -> storage newly selected -> goto step 8
   - slot[4] != 0 AND slot[84] == 0 -> profile newly loaded -> goto step 8
   - If none triggered -> goto step 8b

8. **Content transition routing** based on `[0x831D5330]`:
   - == 1: call `sub_822BCA90(r18)`, set `[0x831D5330]` = 2
   - == 2: (already showing)
   - other: call `sub_8221B198([0x831E4DD4])` -> store to `0x82A95474`
   - Then call `sub_82215530(r22, r15)` (streaming world setup)

8b. **Content readiness check**: slot[56] != 0 AND slot[136] == 0:
   - If newly set: call `sub_82215530(r22, r20)`, then route by `[0x831D5330]`:
     - == 1: keep 1
     - == 2: set to 2
     - other: call `sub_8221B198` -> store to `0x82A95474`
   - Clear `[0x831D5330]` to 0, set **r29=4**

9. **Network/XAM checks** (always runs after step 4-8):
   - `sub_826CBD20` nonzero OR `sub_826CBD08` nonzero -> clear `[0x831D5330]`, set r29=2
   - `sub_8223DAA0` returns nonzero AND `[0x831D5327]` nonzero -> clear `[0x831D5330]`, clear `[0x831D5327]`, reset r29=0
   - `sub_8223DAA0` returns zero AND `[0x831D5327]` == 0 -> set `[0x831D5327]` = 1 -> exit loop (r29 > 6)

**Recomp hook**: `sub_821406C8` pre-populates slot fields (56, 68, 72, 4) = 1 on first call so the "newly set" detectors fire immediately.

## State 4: Save/Content Inner State Machine

**Function**: `sub_822440F8` (7 inner states)
**Transition logic**:
- Returns 1 -> r29=7 (error, exit)
- Returns 2 -> r29=5 (advance to game start)
- Other -> loop

**Recomp hook**: BYPASSED entirely. Returns 2 immediately (no-save success). Sets `0x82A95478` = 0 if uninitialized.

## State 5: Game Start / Level Selection

**Function**: `sub_822422E0`
**Input**: `[0x82A95478]` (episode index, loaded from r17+21624)
**Transition**: Always falls through to state 6 (r29=6). No conditional.

**Then immediately enters state 6** (no loop iteration between 5 and 6).

**Recomp hook**: Resets `0x82BF9834` from 2 to 0 after completion (prevents error 34 in scene creation).

## State 6: Scene/World Loading Inner State Machine

**Function**: `sub_822438B0` (8 inner states at `0x82BF9838`)
**Calls**: `sub_82242910` (15-state scene creation sub-machine at `0x82BF9848`)
**Transition logic**:
- Returns 0 -> r29=9 (done, exit state machine to post-init)
- Returns 2 -> r29=7 (error, exit)
- Other -> loop

## Exit Paths

### Normal exit (r29 == 8, from state 2 ret=2):
Jumps to `loc_82142608`. Writes:
- `[0x831D5348]` = 0 (post-exit flag 1, cleared)
- `[0x831D5337]` = 1 (post-exit flag 2, set)
Then runs post-init sequence:
1. `sub_821B5A68` (text/locale)
2. `sub_82220118(0x82BCF998, 0)` (streaming world)
3. `sub_8222DB48(0x82BEFA40)` (world manager)
4. `sub_8214AD88` (post-load setup)
5. `sub_821ED6D8(0x82B978B0, 0, 0, 1)` (game activation)
6. DLC check, then `sub_8223E028` (state exit completion)

### Error exit (r29 == 7):
Same as r29=8 but calls `sub_82141F00([0x82BF9858], 1, 0, 0)` first, then `sub_8223F800`, then potentially restart logic. Reaches `sub_8223E028` after error handling.

### Scene complete exit (r29 == 9, from state 6 ret=0):
Same post-init sequence as r29=8.

### Loop guard:
At `loc_821425F0`: if `r29 <= 6`, loop back to `loc_821422A0`. If `r29 > 6`, fall through to exit.

## Scene Pointer 0x831C2458

This address is **NOT directly written by sub_82142230**. It is written by `sub_82242910` (the 15-state scene creation SM called from state 6's inner SM `sub_822438B0`).

The scene object pointer tracked by the state 6 hook is at **0x82BF3A88**, not 0x831C2458. The address 0x831C2458 is likely a secondary reference populated during post-init systems (`sub_82220118` or `sub_8222DB48`), or by the scene creation SM at states 8-12 via `sub_8284AB10`/`sub_8284B490`.

## Why 0x831C2458 Stays NULL

For the scene pointer to be written, the state machine must reach:
1. State 3 -> needs player slot populated (hooked)
2. State 4 -> needs save/content SM to return 2 (bypassed/hooked)
3. State 5 -> needs episode index set (hooked)
4. State 6 -> needs `sub_822438B0` inner SM to complete
5. Inner SM state 2 -> needs `sub_82242910` (15-state scene creation) to complete all 15 states
6. Scene creation states 6-14 -> actual resource loading, scene object construction

If any inner state stalls (e.g., streaming hang at `sub_8286C238`, content size mismatch at state 12/14's `sub_822417B0`, or platformMode gate at state 4/11), the scene pointer is never written.

The current blocker is in `sub_82242910`'s states 12-14 (content size comparison via `sub_822417B0`), as documented in `docs/rewrite/28-38*.md`.
