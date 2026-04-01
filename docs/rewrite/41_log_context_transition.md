# Log Analysis: SM Completion to Stack Guard Spam Transition

## Summary

The log at `/tmp/liberty_run.log` (5,507,626 lines) captures the full lifecycle from startup through an infinite loop of stack guard page faults. The transition from productive game initialization to the fatal spam loop occurs across a narrow 43-line window (lines 81780-81822) with zero intervening events.

---

## 1. Timeline

### Phase 1: Scene Creation State Machine (lines 1100-1153)

The scene creation SM runs from YIELD #8000 through YIELD #13000:

| Line | Event |
|------|-------|
| 1100 | `SCENE-CREATE #1` state=1->3 |
| 1131 | `SCENE-CREATE #9` state=10->11 |
| 1134 | `SCENE-CREATE #10` state=11->12 |
| 1136 | `CONTENT-CREATE #0 ENTER` slotIdx=0 |
| 1137 | `CONTENT-CREATE #0 RETURN` ret=1, slotState=16 (sync/BAD) |
| 1138 | `TWO-PHASE #0` phase=1 ret=1 delta=0 |
| 1139 | `TWO-PHASE #1` phase=0 ret=0, CLAMPED delta -560->0 |
| 1142 | `SCENE-CREATE #11` state=12->14 (COMPLETE) |
| 1143 | `STATE-6-INNER #11` state=2->3 |
| 1145 | `STATE-6-INNER #12` state=3->7 |
| 1147 | `READY-SIGNAL` writes 1 to 0x82BF9B70 |
| 1153 | `STATE-6-INNER #13` state=7->0 (SM exits, ret=2) |

### Phase 2: Post-SM Init (lines 1154-1179)

After SM completes, the game enters world loading:

- Lines 1155-1158: Four VFS lookups for `update:\text`
- Lines 1159-1162: YIELD #14000 through #17000 (rapid yields, loading)
- Lines 1163-1166: `SetTexture` skip warnings (unregistered tex 0xffc8f000)
- Line 1167: YIELD #18000
- Lines 1168-1175: DLC content loading (e1: loadingscreens.xtd, e2: audio config)
- Lines 1177-1179: YIELD #19000, #20000, #21000

### Phase 3: Null-Vtable MISSING-FUNC Flood (lines 1180-81779)

Starting at YIELD #21000, three distinct null-vtable call-site groups fire in sequence:

**Group A: Audio/physics vtable pair (lines 1180-1372)**
- `0x8291E144` and `0x8291E1B0` -- alternating calls to address 0x00000000
- 96 calls from each site (192 total)
- Likely an update loop iterating over uninitialized objects

**Group B: Main game loop vtable + subsidiary (lines 1373-81779)**
- `0x828C99CC` -- **dominant**: 2,337,254 calls to 0x00000000 across entire log
- `0x821446F8` and `0x8214434C` -- paired, 13,136 calls each (always appear together per-frame)
- `0x8291593C` -- 72 calls to address 0x00000246 (different target!)
- These represent the main game tick loop calling virtual methods on uninitialized object arrays

**Group C: Renderer vtable calls (lines 81738-81779)**
- `0x828C1F94` -- 37 calls to 0x00000000
- `0x828C1B58`, `0x828C1B64`, `0x828C1ADC` -- 1 call each (one-shot init)
- These are renderer-side vtable dispatches, also hitting null

### Phase 4: VBlank-FIX (line 81780)

```
[VBlank-FIX] Allocated stub at guest 0x12BD0000 for device[+10900]
```

This allocates a GPU VBlank callback stub. It occurs exactly once, between the renderer null-vtable calls and the 0x000F4000 calls. This is significant: it means the GPU device's VBlank pointer (device[+10900]) was null/stale until this point, and the fix allocated a replacement.

### Phase 5: 0x000F4000 MISSING-FUNC Burst (lines 81781-81822)

Exactly 42 calls from `0x821911C4` to target address `0x000F4000`. This is NOT a null vtable -- it is a specific bogus address. The value 0x000F4000 (=0xF4000) likely comes from reading an uninitialized or partially-written vtable entry. The caller 0x821911C4 is in the game's main code range.

### Phase 6: Stack Guard Page Spam (lines 81823-5507626)

Immediately after the last 0x000F4000 call (line 81822), the stack guard faults begin at line 81823. The progression:

- Lines 81823-81878: Walk through 67 unique pages from 0x70000000 to 0x705C0000 (growing stack downward in 0x10000/0x30000 steps)
- Line 81879: First `BaseHeap::Protect failed due to uncommitted page`
- Lines 81880-EOF: Infinite loop at 0x705D0000 with paired error/warning (1,569,138 hits at this address + 1,569,173 BaseHeap::Protect errors)

---

## 2. Are MISSING-FUNC Calls Causing the Stack Guard Issue?

**YES -- highly likely, via a specific mechanism:**

The 42 calls to 0x000F4000 from 0x821911C4 are the direct trigger. When the MISSING-FUNC handler is called for an out-of-range address, it returns without performing the intended operation. If 0x821911C4 is inside a recursive or deeply nested call chain (e.g., a physics/scene graph traversal), each "no-op" return from the missing function handler causes the calling code to:

1. Fail its expected initialization/update
2. Loop back and retry (or continue iterating with corrupt state)
3. Each iteration pushes more stack frames

The sequence is:
1. `0x828C1F94` makes 37 null-vtable calls (renderer dispatch) -- these are no-ops but survive
2. VBlank-FIX allocation happens (device pointer patched)
3. `0x821911C4` makes 42 calls to 0x000F4000 -- these are vtable entries pointing to bogus address 0xF4000 (likely a vtable in guest memory whose function pointers were not populated by the recompiler)
4. **Immediately** after these 42 calls, thread t41614048 starts hitting stack guard pages, walking from 0x70000000 downward to 0x705D0000 in ~56 pages
5. At 0x705D0000, BaseHeap::Protect fails (the page is beyond the committed stack region), and the thread enters an infinite fault-handle-fault loop

The 0x000F4000 calls are not coincidental. The 42-call burst represents iterating over a vtable or object array where each entry's virtual method pointer was populated with a stale/bogus address. The no-op returns cause the game logic to enter unbounded recursion or a deep loop that exhausts the stack.

---

## 3. Crash/Error Between SM Completion and Guard Spam

**No crash or error between SM completion and the guard spam.**

The entire span from line 1153 (SM exit) to line 81879 (first BaseHeap::Protect error) contains:
- VFS lookups (normal)
- SetTexture skip warnings (non-fatal, known issue)
- MISSING-FUNC warnings (non-fatal, logged but execution continues)
- One VBlank-FIX allocation (fixup, not error)

The first actual error is `BaseHeap::Protect failed due to uncommitted page` at line 81879, which is a consequence of the stack exhaustion, not a cause.

---

## 4. Active Threads

Five threads are visible in the log:

| Thread ID | Role | First Seen |
|-----------|------|------------|
| t41613634 | Main/setup thread | Line 11 (processor init) |
| t41613653 | Audio Worker (handle F8000004) | Line 13 |
| t41614047 | Kernel Dispatch (handle F800000C) | Line 107 |
| t41614048 | **Main game thread** (handle F8000010) | Line 155+ (all VFS, game logic) |
| t41614055 | GPU VSync (handle F8000C78) | Line 197 |

**All stack guard faults occur on t41614048** (the main game thread). No other thread produces errors. The Audio Worker and GPU VSync threads appear to continue running independently (yields continue up to #2,306,000 across the log).

---

## 5. Frames Rendered by Spam Start

**10 frames** were rendered before the MISSING-FUNC spam begins.

- RENDER-GATE frame#1 through frame#10 are logged at lines 468-811 (early startup, before SM begins)
- After frame#10 (line 811), no further RENDER-GATE entries appear in the entire 5.5M-line log
- The rendering system stopped producing frames after early init
- The game's yield counter reaches #21000 at line 1179 (just before the MISSING-FUNC flood), suggesting ~21,000 game ticks (not rendered frames) had occurred

The yield counter continues during the spam: #22000 appears at line 1,525,191, interspersed within the guard page spam. By log end, it reaches #2,306,000. This means the game's main loop is still ticking (yielding) but every tick triggers the guard page fault cycle.

---

## 6. Key Addresses Summary

### MISSING-FUNC Call Sites (by volume)

| Caller | Target | Count | Role |
|--------|--------|-------|------|
| 0x828C99CC | 0x00000000 | 2,337,254 | Main game loop null vtable (dominant) |
| 0x821446F8 | 0x00000000 | 13,136 | Per-frame paired dispatch |
| 0x8214434C | 0x00000000 | 13,136 | Per-frame paired dispatch |
| 0x8291E144 | 0x00000000 | 96 | Audio/physics init pair |
| 0x8291E1B0 | 0x00000000 | 96 | Audio/physics init pair |
| 0x8291593C | 0x00000246 | 72 | Non-null bogus target |
| 0x821911C4 | 0x000F4000 | 42 | **Stack-exhaustion trigger** |
| 0x828C1F94 | 0x00000000 | 37 | Renderer vtable dispatch |
| 0x82A4DB1C | 0x00000000 | 1 | GPU setup (early, line 226) |
| 0x82A4DB8C | 0x00000000 | 1 | GPU setup (early) |
| 0x82A4DBC0 | 0x00000000 | 1 | GPU setup (early) |
| 0x828C1B58 | 0x00000000 | 1 | Renderer one-shot |
| 0x828C1B64 | 0x00000000 | 1 | Renderer one-shot |
| 0x828C1ADC | 0x00000000 | 1 | Renderer one-shot |
| 0x82190B98 | 0x00000000 | 1 | Early init (line 505) |

### Stack Guard Page Progression

- 0x70000000 to 0x705C0000: one hit each (67 unique pages, walking down)
- 0x705D0000: 1,569,138 hits (infinite loop -- this is the stack limit)
- BaseHeap::Protect failures: 1,569,173 (paired with 0x705D0000 hits)

---

## 7. Conclusions

1. **The 42 calls to 0x000F4000 from 0x821911C4 are the proximate cause of stack exhaustion.** They occur on the last line before guard page faults begin, with zero intervening events. The target 0x000F4000 is a bogus vtable function pointer that the MISSING-FUNC handler silently no-ops, causing the caller to loop/recurse until the stack is consumed.

2. **The earlier null-vtable calls (828C99CC, 8291E144/E1B0, etc.) are symptoms, not causes.** They represent the game ticking with uninitialized virtual method tables. They are high-volume but non-fatal because the MISSING-FUNC handler returns cleanly.

3. **The VBlank-FIX at line 81780 is a timing marker, not a cause.** It patches the GPU device's VBlank pointer. It happens to occur between the renderer null-vtable calls and the 0x000F4000 burst, but does not itself trigger the stack overflow.

4. **The game was never rendering past frame 10.** The renderer stopped producing frames before the SM even began. All visual output would have been the initial loading/splash screens.

5. **Priority fix: Identify what object array 0x821911C4 is iterating over and why its vtable entries contain 0x000F4000.** This is likely a vtable in guest memory (around 0x000F4000) that the recompiler did not populate with host function pointers. Alternatively, a constructor for these objects was skipped (null-vtable dispatch returned without initializing the object), and 0x000F4000 is whatever garbage was in that memory region.
