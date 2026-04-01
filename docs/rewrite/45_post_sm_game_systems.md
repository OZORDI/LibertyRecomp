# Post-Scene-SM Game Systems Analysis (Updated 2026-03-27)

## Log Timeline After Scene SM Completion

After `sub_822438B0` state 7 completes (ret=2, state=7->0), the following
systems activate before the game enters the MISSING-FUNC crash loop:

### Phase 1: SM Completion (yield #13000)
```
[STATE-6-INNER] sub_822438B0 #13 ret=2 state=7->0 sceneCreation=14 sceneObj=0x000000B9 err=6
```
- sub_822438B0 returns **2** to the parent sub_82142230
- In sub_82142230, `ret=2` at state 6 sets `r29=7` -> loc_821425FC -> loc_82142608

### Phase 2: XAM Flow + Ready Signal
```
[XAM-FLOW] sub_8223F9F0 #0 ENTER caller=0x82242634
[READY-SIGNAL] sub_82254FE0 ENTER — writing 1 to 0x82BF9B70! caller=0x8223FEEC
[READY-SIGNAL] sub_82254FE0 RETURN — 0x82BF9B70 now = 0x00000001
[XAM-FLOW] sub_8223F9F0 #0 RETURN r3=0x00000001
[STATE-3-INIT] sub_8223CAD8 #2 ENTER/RETURN
```
- XAM signaling occurs as part of the outer SM's state 5 (sub_82243260 case 5)
  which calls sub_8223F9F0(35, 0, &result) — an XAM notification dispatch
- Ready signal at 0x82BF9B70 is set to 1 — game is "ready to play"
- sub_8223CAD8 called as part of outer SM state 7 finalization

### Phase 3: Text/Locale Loading (yields #13000-#14000)
```
VFS: 'update:\text' -> ... (4 accesses)
```
- Localized text resources loaded from the update partition
- Triggered by post-SM code in sub_82142230 which calls sub_821B5A68
  (text system) and sub_82220118 (streaming setup)

### Phase 4: Render Pipeline Active (yields #14000-#21000)
```
[SetTexture] skip #14-#20 unregistered tex 0xffc8f000 (slot 0)
```
- Render thread running, attempting to bind textures
- Texture 0xffc8f000 is unregistered — loading screen being drawn with
  null/placeholder texture

### Phase 5: DLC Content Detection (yield #18000)
```
VFS: 'e1:\xbox360\textures\loadingscreens.xtd'  (TLAD)
VFS: 'e1:\common\data'
VFS: 'e2:\'                                       (TBOGT)
VFS: 'e2:\xbox360\audio\config\ep2_radio_game.dat16'
```
- DLC episode content detected and probed (e1=TLAD, e2=TBOGT)
- Likely triggered by sub_82145820 or a streaming system init call

### Phase 6: MISSING-FUNC Crash Loop (yield #21000+)
```
[MISSING-FUNC] indirect call to 00000000 from 8291E144   (x ~1.18M)
[MISSING-FUNC] indirect call to 00000000 from 8291E1B0   (x ~1.18M)
```
- 2.37 million total MISSING-FUNC entries, alternating between two addresses
- Eventually degrades into stack guard page faults at 0x705D0000

---

## Caller Chain: sub_82242910 -> sub_822438B0 -> sub_82142230

### sub_82142230 — Top-Level Game State Machine

**Address**: 0x82142230 | **File**: `gta4_recomp.0.cpp` line 5303

Main loop body with 7 states (r29 = 0..6), each iteration calls
`sub_82849918(1)` (yield) then `sub_821428C8` (frame tick):

| State | Purpose | Key Call |
|-------|---------|----------|
| 0 | Platform/system init | sub_822414E8 |
| 1 | Pre-game readiness | sub_8223DDA8 |
| 2 | Menu/title state | conditional |
| 3 | Save data / profile check | conditional |
| 4 | Outer scene creation SM | sub_822440F8 |
| 5 | Scene dispatch | sub_822422E0 |
| **6** | **Inner scene creation SM** | **sub_822438B0** |

**After sub_822438B0 returns 0 (success):**
- r29 = 9, falls through switch to loc_82142608 (post-scene finalization)
- Calls these systems in sequence:
  1. `sub_821B5A68` — text/locale system init
  2. `sub_82220118(0x82BCF998, 0)` — streaming world setup
  3. `sub_8222DB48(0x82BEFA40)` — world manager init
  4. `sub_8214AD88` — post-load setup
  5. `sub_821ED6D8(0x82B978B0, 0, 0, 1)` — game system activation
  6. `sub_82145820` — DLC detection / post-load work
- Then checks r29 for exit: r29=9 -> loc_82142758 (normal game start)

**After sub_822438B0 returns 2 (the actual observed case):**
- r29 = 7, jumps to loc_821426F0 -> loc_821426F8
- This is the "load new scene" / reload path
- Still calls the same finalization at loc_82142608 first

### sub_822438B0 — Outer Scene Creation Wrapper

**Address**: 0x822438B0 | **File**: `gta4_recomp.6.cpp` line 87079

Switch on 8 states (0..7):

| State | Purpose |
|-------|---------|
| 0 | Return 0 (not started) |
| 1 | Init flags, set state=2 |
| 2 | Call sub_82242910 (inner SM), loop until done |
| 3 | Load extra scene: sub_82240F80(0) |
| 4 | Check "SAVE" marker, set state 5 |
| 5 | Load extra scene: sub_82240F80(1) |
| 6 | Wait readiness flag + 3000-tick timer |
| **7** | **Terminal: sub_8223CAD8, sub_826CD808, return 2** |

State 7 is the terminal state observed in the log. It calls `sub_8223CAD8`
(STATE-3-INIT in log), then `sub_826CD808`, returns **2** to parent.

**Two callers of sub_822438B0:**
1. `sub_82142230` at line 5845 (state 6 of the main game SM)
2. A function near 0x8214C51C at line 29924 (appears to be a variant/reload path)

---

## The MISSING-FUNC Crash: sub_8291DF00

### Function Identity
**Address**: 0x8291DF00 | **File**: `gta4_recomp.62.cpp` line 7315

This is a **scene graph traversal / draw list processor**:

1. Reads object count from global pointer `[-28992](r21)` offset 40
2. Optionally calls `sub_82849918` (yield) if a flag at `[-28983](r31)` is set
3. Iterates objects in a loop (stride 768, counter r22, up to r20 objects)
4. For each object, performs vtable indirect calls through `r31` (object ptr):

```
// At 0x8291E130 — vtable[10] (offset 40):
lwz r11, 0(r31)     // load vtable ptr
lwz r11, 40(r11)    // vtable slot 10
mtctr r11; bctrl     // call -> returns to 0x8291E144
                     // PURPOSE: likely Render() or Update()

// At 0x8291E19C — vtable[6] (offset 24):
lwz r11, 0(r31)     // load vtable ptr
lwz r11, 24(r11)    // vtable slot 6
mtctr r11; bctrl     // call -> returns to 0x8291E1B0
                     // PURPOSE: likely GetBounds() or GetVisibility()
```

### Root Cause of Null Vtable Calls

The vtable pointer at `[r31+0]` loads a value that resolves to a vtable
where slot 10 (offset 40) and slot 6 (offset 24) are both **0x00000000**.

This means the scene objects exist in the object list but their vtables
were never populated with the correct function pointers. The likely cause:
- Scene objects were created during scene creation (state 4->9, when
  sceneObj changed from 0x0 to 0xB9)
- Their vtables point to a class that requires D3D/GPU backend initialization
- The recomp's rendering backend didn't create the concrete vtable entries
  for these scene node types

### Caller Chain
- `sub_8291E260` (line 7792) calls `sub_8291DF00` at 0x8291E71C
  - sub_8291E260 appears to be `CSceneRenderer::RenderScene()` or similar
  - It processes a sorted draw list before calling sub_8291DF00

---

## sub_821910D0 — Render Thread Dispatch

**Address**: 0x821910D0 | **File**: `gta4_recomp.2.cpp` line 1886

This is the **render thread's frame dispatch**:

1. Enters critical section (`RtlEnterCriticalSection`)
2. Stores TLS thread ID at render context + 300
3. If render context[304] != 0 (work pending):
   - Signals KeSetEvent (work ready notification)
   - Calls KeWaitForMultipleObjects(2 objects, timeout=3, wait=ALL)
   - Wait objects: work completion event + shutdown event
   - If shutdown signaled: sets r31=1 (exit flag)
4. If render context[304] == 0 (no work, direct processing):
   - Calls `sub_8218FFB0(renderCtx)` — process render commands
   - Calls `sub_82191228(renderCtx, 1)` — finalize frame
5. After processing, calls vtable[17] (offset 68) on object at renderCtx[64]:
   ```
   lwz r3, 64(r30)      // get render device
   lwz r11, 0(r3)       // load vtable
   lwz r11, 68(r11)     // vtable slot 17
   bctrl                 // call -> 0x821911C4
   ```
   This is likely `RenderDevice::Present()` or `EndFrame()`

**sub_8219A2B8** is a thin wrapper: loads the global render context pointer
from `[-31971*65536 + 21484]` and tail-calls sub_821910D0.

The 0x000F4000 vtable address previously identified as problematic would be
the vtable of the render device object at renderCtx[64].

---

## All Unique MISSING-FUNC Caller Addresses

| Address | Containing Function | Likely System |
|---------|-------------------|--------------|
| 0x8214434C | sub_82142230 area | Main game SM — vtable call |
| 0x821446F8 | sub_82142230 area | Main game SM — vtable call |
| 0x82190B98 | sub_82190B48 area | Render thread creation |
| 0x821911C4 | sub_821910D0 | Render dispatch vtable[68] (Present) |
| 0x828C1ADC | 0x828Cxxxx cluster | Physics/animation vtable call |
| 0x828C1B58 | 0x828Cxxxx cluster | Physics/animation vtable call |
| 0x828C1B64 | 0x828Cxxxx cluster | Physics/animation vtable call |
| 0x828C1F94 | 0x828Cxxxx cluster | Physics/animation vtable call |
| 0x828C99CC | 0x828Cxxxx cluster | Physics/animation vtable call |
| 0x8291593C | sub_8291xxxx area | Scene system vtable call |
| **0x8291E144** | **sub_8291DF00** | **Scene render vtable[10] (2.37M hits)** |
| **0x8291E1B0** | **sub_8291DF00** | **Scene render vtable[6] (2.37M hits)** |
| 0x82A4DB1C | 0x82A4xxxx area | CRT/kernel indirect call |
| 0x82A4DB8C | 0x82A4xxxx area | CRT/kernel indirect call |
| 0x82A4DBC0 | 0x82A4xxxx area | CRT/kernel indirect call |

The dominant crash is the 0x8291E144/0x8291E1B0 pair (sub_8291DF00), which
fires on every scene object, every frame, indefinitely.

---

## Key Findings

### 1. Game Systems That Activate Post-SM (in order)
1. XAM notification + ready signal (0x82BF9B70 = 1)
2. Text/locale loading (update:\text, 4 VFS accesses)
3. Render pipeline begins drawing frames (SetTexture attempts)
4. DLC content detection (TLAD + TBOGT probing via e1:\ and e2:\)
5. Scene graph render loop starts (sub_8291DF00) -- **crashes here**

### 2. The Blocker: Null Vtables on Scene Render Objects
The 2.37M MISSING-FUNC calls are from sub_8291DF00 iterating scene objects
whose vtable entries for Render (slot 10) and GetBounds (slot 6) are null.
These are indirect calls through `PPC_CALL_INDIRECT_FUNC(0)`.

### 3. The Transition Between SM and Crash is Clean
Between SM completion (line 1153) and first MISSING-FUNC from 8291E144
(line 1180), the game successfully:
- Runs XAM flow and sets the ready signal
- Loads text resources
- Renders ~7 frames with loading screen
- Detects DLC content
- Only ~27 log lines of useful activity before the crash loop

### 4. Stack Guard Exhaustion is Secondary
The stack guard faults at 0x705D0000 are a downstream effect of the
MISSING-FUNC handler being called millions of times, consuming stack
via the signal handler chain. The root cause is the null vtable entries.

### 5. sub_821910D0 is a Render Thread Dispatcher
It enters a critical section, checks for render work, processes the render
command queue, and calls Present() via a vtable. The vtable[68] call at
0x821911C4 is one of the 15 unique MISSING-FUNC callers, confirming the
render device vtable is also not populated.

### 6. sub_82142230 Returns After Scene Creation
The main game SM is NOT a per-frame loop. It runs once during boot,
driving the scene creation pipeline, then returns. The per-frame loop
is driven separately by the render loop hook around sub_82856F08.
