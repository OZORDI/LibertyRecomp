# 41: Log Context Before Stack Guard Crash

Analysis of `/tmp/liberty_run.log` (398MB, 5,507,626 lines) from 2026-03-27.

## 1. First Stack Guard Occurrence

| Metric | Value |
|--------|-------|
| First `Stack guard page` message | **Line 81823** |
| First `BaseHeap::Protect failed` | **Line 81879** (at address 0x705D0000) |
| Total `BaseHeap::Protect` messages | 1,569,173 |
| Total log lines | 5,507,626 |
| Stack guard address range | 0x70000000 through 0x705D0000 |
| Bytes consumed before stuck | 0x5D0000 = 6,094,848 bytes (~5.8MB) |

## 2. What Happens in the 50 Lines Before the First Stack Guard (lines 81770-81822)

The log is **pure MISSING-FUNC spam** from two call sites:

```
[MISSING-FUNC] indirect call to 00000000 (in_range=0) from 828C1F94   (many hundreds)
[MISSING-FUNC] indirect call to 00000000 (in_range=0) from 828C99CC   (interspersed)
[VBlank-FIX] Allocated stub at guest 0x12BD0000 for device[+10900]     (line 81780)
[MISSING-FUNC] indirect call to 000F4000 (in_range=0) from 821911C4   (42 occurrences, lines 81781-81822)
```

The critical transition:
- **Line 81780**: `[VBlank-FIX]` allocates a VBlank stub -- this is the last "useful" message.
- **Lines 81781-81822**: 42 calls to address `0x000F4000` from `0x821911C4` -- a NEW call target appearing for the first time. This function is trying to call `0x000F4000` which is NOT in the recompiled code range.
- **Line 81823**: First stack guard page hit at `0x70000000`.

## 3. Complete State Machine Timeline (Lines 1008-1157)

The scene creation state machine **completes successfully**:

```
Line 1008: [DIAG] sub_82140088 ENTER (main game loop!)
Line 1009: [DIAG] sub_821458B8 (init gate) #0 = 0 (not ready)
Line 1010: [STATE-MACHINE] sub_82142230 ENTER
Line 1011: [PRE-STATE] sub_82241370 #0 = -2101372968

  -- State 0: Sign-in check --
Line 1019: [STATE-0] sub_822414E8 (sign-in check) #0 = 1

  -- State 1: Storage device (loops 10 times) --
Lines 1021-1073: [STATE-1] sub_8223DDA8 (storage device) #0-#10
Line 1073: STATE-1 #10 = 2  (storage device ready!)

  -- State 2: Save/load --
Line 1074: [STATE-2] sub_8223DEE8 (save/load) #0 = 1

  -- State 3: Content enumeration --
Line 1076: [STATE-3-INIT] sub_8223CAD8 #0
Line 1079: [STATE-3] Populated player slot

  -- State 4: No-save bypass --
Line 1090: [STATE-4-INNER] BYPASS: returning 2 (no-save success)

  -- State 5: Scene creation entry --
Line 1092: [STATE-5-START] sub_822422E0 ENTER state=0 episode=0
Line 1093: [STATE-5-START] ret=0 state=0->2

  -- State 6 / Scene Creation (sub_82242910): 12 iterations --
Lines 1096-1142: Scene creation SM: 0->1->3->4->9->10->10->10->10->11->12->14

  ** COMPLETION **
Line 1142: [SCENE-CREATE] sub_82242910 #11 ret=0 state=12->14  <-- SM DONE
Line 1143: [STATE-6-INNER] sub_822438B0 #11 ret=1 state=2->3  <-- outer advances
Line 1145: [STATE-6-INNER] sub_822438B0 #12 ret=1 state=3->7 err=24
Line 1146: [XAM-FLOW] sub_8223F9F0 #0 ENTER
Line 1147: [READY-SIGNAL] sub_82254FE0 ENTER -- writing 1 to 0x82BF9B70!
Line 1149: [READY-SIGNAL] sub_82254FE0 RETURN -- 0x82BF9B70 now = 0x00000001
Line 1150: [XAM-FLOW] sub_8223F9F0 #0 RETURN r3=0x00000001
Line 1153: [STATE-6-INNER] sub_822438B0 #13 ret=2 state=7->0  <-- SM exits
```

## 4. Messages Between SM Completion and Stack Guard

After STATE-6-INNER #13 returns `ret=2 state=7->0` at line ~1157:

| Line Range | Content |
|-----------|---------|
| 1158-1163 | VFS reads: `update:\text` (4x) |
| 1164-1175 | YIELD #14000 through #21000 (game loop still running) |
| 1176-1187 | SetTexture skip #14-#20 (unregistered tex 0xffc8f000) |
| 1188-1195 | VFS: loading screens from DLC (e1:\, e2:\) |
| 1196-81779 | **Massive MISSING-FUNC spam** from 0x8291E144, 0x8291E1B0, 0x828C99CC, 0x828C1F94, 0x821446F8, 0x8214434C, 0x828C1ADC, 0x828C1B58, 0x828C1B64 |
| 81780 | `[VBlank-FIX]` stub allocation |
| 81781-81822 | `MISSING-FUNC` calls to `0x000F4000` from `0x821911C4` (42x) |
| 81823 | **FIRST Stack guard page hit at 0x70000000** |

## 5. Does sub_822438B0 (Outer SM) Advance After sub_82242910 Returns 0?

**YES.** The outer state machine advances correctly:

```
sub_82242910 #11 ret=0 state=12->14   (scene creation complete)
sub_822438B0 #11 ret=1 state=2->3     (outer: 2->3, continues)
sub_822438B0 #12 ret=1 state=3->7     (outer: 3->7, err changes 6->24)
  [XAM-FLOW fires, READY-SIGNAL written]
sub_822438B0 #13 ret=2 state=7->0     (outer: 7->0, EXITS with ret=2)
```

The outer SM runs through states 2->3->7->0 and exits with `ret=2` (success/complete).

## 6. Progress Indicators

### Frame Counts
- Renders stop at **frame #10** (line 810) and **Present #5** (line 518).
- No frames render after early initialization.
- RENDER-GATE consistently shows `scene@831C2458=0x00000000` (no scene object in render path).

### YIELD Counts
- YIELD counter runs from #0 to **#2,306,000** before the user killed the app.
- YIELDs continue throughout the stack guard spam (the game loop is still "running").
- The stack guard infinite loop does NOT freeze the YIELD counter.

### ALLOC FALLBACK
- **ZERO** ALLOC FALLBACK messages in this log. The particle emitter storm from previous runs is not occurring here.

### MISSING-FUNC Call Sites (Between SM and Stack Guard)

| Target Address | Caller | Count | Notes |
|---------------|--------|-------|-------|
| 0x00000000 | 0x8291E144 | many | Vtable dispatch, null ptr |
| 0x00000000 | 0x8291E1B0 | many | Vtable dispatch, null ptr |
| 0x00000000 | 0x828C99CC | many | Vtable dispatch, null ptr |
| 0x00000000 | 0x828C1F94 | many | Vtable dispatch, null ptr |
| 0x00000000 | 0x821446F8 | some | Vtable dispatch, null ptr |
| 0x00000000 | 0x8214434C | some | Vtable dispatch, null ptr |
| 0x00000001 | 0x8214434C | 1 | Vtable dispatch, near-null |
| 0x00000246 | 0x8291593C | rare | Vtable dispatch, garbage ptr |
| 0x000F4000 | 0x821911C4 | 42 | **NEW**: immediately precedes stack guard |

## 7. Ready Signal (0x82BF9B70)

| Phase | Value | Line |
|-------|-------|------|
| Initial (pre-SM) | 0xFFFFFFFF (-1) | Lines 1013-1065 (RES-CHECK shows repeated reads) |
| After XAM-FLOW | **0x00000001** | Line 1149 (READY-SIGNAL RETURN) |

**The ready signal does NOT reach -1 for completion.** It starts at 0xFFFFFFFF (uninitialized sentinel), then gets written to **0x00000001** by `sub_82254FE0` after the XAM flow. The value 1 means "scene created / ready to play" -- but it is not the expected -1 completion value.

Note: The 0xFFFFFFFF seen in RES-CHECK during the SM is the **initial** value (before the SM writes anything), not a completion signal.

## 8. Root Cause Analysis

The crash sequence is:

1. **State machine completes normally** (state 0->1->3->4->9->10->11->12->14, outer 2->3->7->0).
2. **Ready signal written to 1** (not -1).
3. **Game enters main loop** with YIELD continuing.
4. **Massive MISSING-FUNC vtable dispatch failures** from ~10 different callers (0x8291E1xx, 0x828C1xxx, 0x821446xx) -- these are null virtual function calls suggesting uninitialized objects.
5. **0x821911C4 starts calling 0x000F4000** (42 times) -- this address is in the low 1MB of guest memory, far below the code range (0x82000000+). This is reading a corrupted function pointer.
6. **Stack guard pages begin firing at 0x70000000** and walk upward by ~0x10000-0x30000 per step until reaching 0x705D0000 where `BaseHeap::Protect` fails (uncommitted page = stack limit reached).
7. **Infinite loop**: The stack has grown 5.8MB, hit the allocation ceiling at 0x705D0000, and the guard page handler keeps re-firing because it cannot commit more pages.

### Key Insight

The **MISSING-FUNC calls to 0x000F4000 from 0x821911C4** are the immediate trigger. This function is likely in a recursive call path that grows the stack. The null vtable dispatches before it suggest that the world initialization after scene creation is trying to use objects whose vtables were not properly populated.

The stack guard is NOT a memory corruption bug -- it is **stack overflow** caused by unbounded recursion or extremely deep call chains in the world loading code after the scene creation SM completes.
