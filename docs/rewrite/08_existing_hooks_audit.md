# 08 - Existing Hooks Audit: State Machine & Scene Creation

## Overview

This document audits every hook in `imports.cpp` and `save_hooks.cpp` that
relates to the front-end state machine, scene creation, and the lifecycle path
from `xstart` through game-loop entry. It also documents the hooking
infrastructure itself.

**Source files audited:**
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/imports.cpp` (2225 lines)
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/save_hooks.cpp` (363 lines)
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/function.h` (macro definitions)

---

## 1. Hooking Infrastructure

### 1.1 PPC_FUNC_HOOK Macro

Defined in `function.h` line 354:

```cpp
#define PPC_FUNC_HOOK(x) extern "C" PPC_FUNC(x)
```

`PPC_FUNC` expands to a function with signature
`void <name>(PPCContext& ctx, uint8_t* base)`. The `extern "C"` linkage is
critical: the generated recomp code emits each function as a `__attribute__((weak))`
C-linkage symbol. Without `extern "C"`, C++ name mangling produces a different
symbol that does NOT override the weak one.

### 1.2 The `__imp__` Prefix Convention

Every recompiled function `sub_XXXXXXXX` has a companion `__imp__sub_XXXXXXXX`
symbol that points to the original recompiled code. When a hook wants to
**wrap** (not replace) a function:

1. Forward-declare: `extern "C" void __imp__sub_XXXXXXXX(PPCContext& ctx, uint8_t* base);`
2. Define the hook with `PPC_FUNC_HOOK(sub_XXXXXXXX) { ... }`
3. Call `__imp__sub_XXXXXXXX(ctx, base);` inside the hook to run the original.

The hook's strong `extern "C"` symbol overrides the weak symbol from codegen.
All callers (both direct `bl` and indirect via function table) are redirected.

### 1.3 How to Write a Complete Replacement Hook

Use `PPC_FUNC` (from `save_hooks.cpp` pattern) or `PPC_FUNC_HOOK` and simply
do NOT call `__imp__`. Example:

```cpp
PPC_FUNC_HOOK(sub_XXXXXXXX) {
    // Complete replacement -- never calls __imp__sub_XXXXXXXX
    ctx.r3.u32 = 1; // return value
}
```

Or for `save_hooks.cpp` style (identical effect):

```cpp
PPC_FUNC(sub_XXXXXXXX) {
    // Complete replacement
    ctx.r3.s64 = 1;
}
```

Both produce the same `extern "C"` symbol. `PPC_FUNC_HOOK` is preferred in
`imports.cpp` for consistency.

### 1.4 Atomic Counters and Diagnostic Variables

The file uses `std::atomic<int>` counters for rate-limited logging. Pattern:

```cpp
static std::atomic<int> s_fooCount{0};
PPC_FUNC_HOOK(sub_XXXXXXXX) {
    int n = s_fooCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 20 || (n % 200) == 0) { printf(...); }
    ...
}
```

Key counters relevant to state machine:
- `s_state0Count` -- sign-in check calls
- `s_state1Count` -- storage device calls
- `s_state2Count` -- save/load check calls
- `s_state6InnerCount` -- outer state 6 inner machine iterations
- `s_sceneCreateCount` -- scene creation sub-machine iterations
- `s_frameUpdateCount` -- main frame update calls
- `s_resCheckCount` -- readiness reader calls
- `s_counterCount` -- ready-counter increment calls
- `s_resetCount` -- ready-reset calls
- `s_playerAccessCount` -- player accessor calls
- `s_yieldCount` -- yield/sleep calls

---

## 2. Lifecycle Path Hooks (xstart through game loop)

### 2.1 xstart (0x82000000 region -- CRT entry)

| Field | Value |
|-------|-------|
| **Address** | xstart (CRT entry) |
| **Hook type** | `extern "C" PPC_FUNC(xstart)` -- complete override |
| **What it does** | Logs entry, calls `__imp__xstart(ctx, base)` (original MSVC `__tmainCRTStartup`), logs completion |
| **Original function** | C++ CRT startup: global constructors, KeTlsAlloc, then calls `main()` |
| **Status** | Working. Must call original to properly init TLS slot 1676 (RAGE allocator). |
| **Still needed?** | Yes for diagnostics. Could be removed if boot is stable. |

### 2.2 sub_82A18BE0 (firmware check)

| Field | Value |
|-------|-------|
| **Address** | 0x82A18BE0 |
| **Hook type** | Wrap (calls `__imp__`) |
| **What it does** | Diagnostic logging only |
| **Original function** | Firmware/init check at boot |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic, safe to remove |

### 2.3 sub_82A18B08 (firmware init check)

| Field | Value |
|-------|-------|
| **Address** | 0x82A18B08 |
| **Hook type** | Wrap |
| **What it does** | Logs result; warns if return=0 (would trigger HalReturnToFirmware) |
| **Original function** | Determines if firmware is initialized |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 2.4 sub_82A18620 (notification callbacks)

| Field | Value |
|-------|-------|
| **Address** | 0x82A18620 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic logging only |
| **Original function** | Registers notification callbacks with critical section |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 2.5 sub_82A110A8 (XEX privilege check)

| Field | Value |
|-------|-------|
| **Address** | 0x82A110A8 |
| **Hook type** | Wrap |
| **What it does** | Logs result; warns if non-zero (would call XamLoaderTerminateTitle) |
| **Original function** | XEX privilege / AV pack check |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 2.6 sub_82140000 (RAGE init gate)

| Field | Value |
|-------|-------|
| **Address** | 0x82140000 |
| **Hook type** | Wrap with guard (`s_rageInitDone`) |
| **What it does** | PRE: If already initialized, returns 1 immediately (skip re-init). Otherwise calls original. POST: Sets `s_rageInitDone=true` on success. |
| **Original function** | Calls sub_821B3CE8 (full RAGE engine init) and returns 1/0 |
| **Known issue** | sub_821B3CE8 is NOT idempotent -- called twice (1st from init path, 2nd from per-frame tick sub_821B3598). Without guard, 2nd call hangs in streaming event wait. |
| **Status** | WORKING -- critical fix |
| **Still needed?** | YES -- prevents re-init hang. Essential in any rewrite. |

### 2.7 sub_82140088 (main game loop)

| Field | Value |
|-------|-------|
| **Address** | 0x82140088 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic logging (entry/exit) |
| **Original function** | Main game loop entry |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 2.8 sub_821B3CE8 (RAGE engine init)

| Field | Value |
|-------|-------|
| **Address** | 0x821B3CE8 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic logging (entry/exit with success/fail) |
| **Original function** | Full RAGE engine initialization |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 2.9 sub_821411D8 (game systems init)

| Field | Value |
|-------|-------|
| **Address** | 0x821411D8 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic logging only |
| **Original function** | Initializes game systems (only called if RAGE engine init succeeds) |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 2.10 sub_821458B8 (init gate)

| Field | Value |
|-------|-------|
| **Address** | 0x821458B8 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs return value (0=not ready, non-zero=ready) |
| **Original function** | Returns whether initialization is complete |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 2.11 sub_821B39A8 (quit flag)

| Field | Value |
|-------|-------|
| **Address** | 0x821B39A8 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs quit flag (0=keep running, non-zero=exit) |
| **Original function** | Returns whether game should exit |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 2.12 sub_8218BEA8 (main entry point / render loop)

| Field | Value |
|-------|-------|
| **Address** | 0x8218BEA8 |
| **Hook type** | Complete override with init guard + infinite loop |
| **What it does** | First call: runs `__imp__sub_8218BEA8` (full game init). Then enters infinite loop calling `__imp__sub_82856F08` (frame tick) every 16ms. |
| **Original function** | Game main entry -- initializes then enters native render loop |
| **Known issues** | Infinite loop with `std::this_thread::sleep_for(16ms)` -- crude frame pacing. Never returns. |
| **Status** | WORKING -- drives the game |
| **Still needed?** | YES -- but the frame loop could be improved (proper VSync, exit handling) |

---

## 3. Front-End State Machine Hooks

### 3.1 sub_82142230 (front-end state machine -- states 0-6+)

| Field | Value |
|-------|-------|
| **Address** | 0x82142230 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs entry/exit |
| **Original function** | Main state machine controlling sign-in -> storage -> save -> scene creation. Uses r29 to track current state. States: 0=sign-in, 1=storage device, 2=save/load, 3=resource check, 4=save/content inner machine, 5=game start/level select, 6=scene creation inner machine, 7+=exit |
| **Status** | Working (diagnostic only) |
| **Still needed?** | Diagnostic wrapper can be removed. The function itself runs unmodified. |

### 3.2 sub_822414E8 (STATE 0: sign-in check)

| Field | Value |
|-------|-------|
| **Address** | 0x822414E8 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs return value. Returns 0=not signed in, 1=signed in, 2=skip to state 3+ |
| **Original function** | Checks XAM sign-in state |
| **Status** | Working (diagnostic only). RexGlue XAM handles sign-in. |
| **Still needed?** | No -- pure diagnostic |

### 3.3 sub_8223DDA8 (STATE 1: storage device selection)

| Field | Value |
|-------|-------|
| **Address** | 0x8223DDA8 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs return value (1 or 2 = advance to state 2) |
| **Original function** | Storage device selection via XAM |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 3.4 sub_8223DEE8 (STATE 2: save/load check)

| Field | Value |
|-------|-------|
| **Address** | 0x8223DEE8 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs return value (1=advance, 2=jump to state 8) |
| **Original function** | Save/load readiness check |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 3.5 sub_821406C8 (STATE 3 gate: player accessor)

| Field | Value |
|-------|-------|
| **Address** | 0x821406C8 |
| **Hook type** | Wrap with SIDE EFFECTS |
| **What it does** | POST-CALL: On first call, populates player slot content-readiness fields (slot offsets 56, 68, 72, 4) with value 1. This simulates XAM notification callbacks that would fire on Xbox 360. The "newly set" transition detector pattern (current != 0 AND shadow == 0) triggers state 3 progression. Also logs active player index and slot field values. |
| **Original function** | Reads active player index from 0x82A9172C, returns pointer to 188-byte player slot struct (NULL if index is -1) |
| **Status** | WORKING -- critical for state 3 advancement |
| **Still needed?** | YES -- without this, state 3 never sees content-readiness transitions and loops forever. A holistic rewrite must either: (a) keep this slot population, or (b) implement proper XAM notification callbacks that write these fields. |

### 3.6 sub_82142F90 (main frame update)

| Field | Value |
|-------|-------|
| **Address** | 0x82142F90 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: periodically logs scene pointer (0x831C2458), inner state variables (0x82BF99D4, 0x82BF9838), player/profile indices (0x82A95478, 0x82A95474) |
| **Original function** | Per-frame update that drives sub_82142230 and scene render |
| **Status** | Working (diagnostic only, slight overhead from periodic reads) |
| **Still needed?** | No -- pure diagnostic |

### 3.7 sub_82241370 (pre-state machine setup)

| Field | Value |
|-------|-------|
| **Address** | 0x82241370 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs first 5 calls |
| **Original function** | Called at top of sub_82142230 before the state switch |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 3.8 sub_821428C8 (per-iteration update)

| Field | Value |
|-------|-------|
| **Address** | 0x821428C8 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs periodically |
| **Original function** | Called each iteration of the state machine loop |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

---

## 4. Scene Creation Path Hooks

### 4.1 sub_822440F8 (STATE 4 inner state machine -- BYPASSED)

| Field | Value |
|-------|-------|
| **Address** | 0x822440F8 |
| **Hook type** | COMPLETE REPLACEMENT (never calls `__imp__`) |
| **What it does** | Returns 2 (success/no-save path) directly. Sets playerIdx (0x82A95478) to 0 if currently -1. Logs once. |
| **Original function** | 7-state inner machine for Xbox 360 save device selection: state 0 scans controllers, state 1 validates storage device, states 2-6 handle save slot enumeration. All depend on Xbox hardware. |
| **Known issues** | Complete bypass means NO save device selection occurs. This is intentional -- none of the Xbox 360 controller/storage logic works in recomp. |
| **Status** | WORKING -- successfully advances outer state to 5 |
| **Still needed?** | YES as bypass. A holistic rewrite should replace this with PC save path logic (VFS-based save enumeration). |

### 4.2 sub_822422E0 (STATE 5: game start / scene dispatch)

| Field | Value |
|-------|-------|
| **Address** | 0x822422E0 |
| **Hook type** | Wrap with POST-CALL FIX |
| **What it does** | PRE: Logs state and episode index. POST: If state at 0x82BF9834 becomes 2 after the call, resets it to 0. This prevents sub_82242910 state 4 from reading stale value 2, which triggers error 34 in a platform mode switch. |
| **Original function** | Reads episode index from 0x82B39504 to pick level 12/13/14, writes 2 to 0x82BF9834 on completion |
| **Bug being fixed** | State variable 0x82BF9834 is shared between state 5 and sub_82242910's state 4. On Xbox 360, sub_8223DAA0 returns 0 during scene creation (slow device enumeration), so the fast path reading 0x82BF9834 is never taken. In recomp, RexGlue reports devices as immediately ready, so sub_8223DAA0 returns 1 too early, hitting the fast path and reading stale value 2 -> error 34. |
| **Status** | WORKING -- prevents error 34 |
| **Still needed?** | YES -- defense-in-depth against stale state. Also fixes the root timing mismatch between RexGlue device readiness and the game's expected delay. |

### 4.3 sub_82242910 (SCENE CREATION sub-machine -- 15 states, 0-14)

| Field | Value |
|-------|-------|
| **Address** | 0x82242910 |
| **Hook type** | Wrap with PRE-CALL and POST-CALL fixes |
| **What it does** | **PRE-CALL**: Ensures platformMode at 0x82BF9844 is 3 (base game) or 4. If not, writes 3. This is required because state 4 switch rejects values 2 or >4 with error 34, and states 4+11 require values 3 or 4. **POST-CALL**: If state jumped from 0 directly to 4 (fast path), forces it back to 1 (normal path entry) by writing 1 to 0x82BF9848. Logs state transitions and diagnostics. |
| **Original function** | 15-state scene creation sub-machine called from sub_822438B0 state 2. Creates the game world. State counter at 0x82BF9848. Scene object written to 0x82BF3A88. |
| **Key state variables** | 0x82BF9848 = scene creation state (0-14), 0x82BF9844 = platform mode, 0x82BF3A88 = scene object pointer, 0x82A9546C = error code |
| **Bug being fixed** | RexGlue reports devices immediately, so sub_8223DAA0 returns 1 at state 0, jumping directly to state 4. States 1-3 (which set up prerequisites including writing 6 to 0x82A9546C) are skipped. State 4 reads stale data and fails with error 34. |
| **Status** | WORKING -- successfully forces normal path 0->1->2->3->4 |
| **Still needed?** | YES -- critical fix. Without it, scene creation takes the fast path and crashes. A holistic rewrite must either: (a) keep this intercept, (b) fix sub_8223DAA0 to return correctly, or (c) make device readiness timing match Xbox 360. |

### 4.4 sub_822438B0 (STATE 6 inner state machine -- 8 states)

| Field | Value |
|-------|-------|
| **Address** | 0x822438B0 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs state transitions (0x82BF9838), scene creation sub-state (0x82BF9848), scene object pointer (0x82BF3A88), error code (0x82A9546C) |
| **Original function** | 8-state inner machine for scene/world loading. Calls sub_82242910 (15-state scene creation) in state 2. State variable at 0x82BF9838. |
| **Status** | Working (diagnostic only -- no behavior changes) |
| **Still needed?** | No -- pure diagnostic |

### 4.5 sub_8223E028 (state machine exit)

| Field | Value |
|-------|-------|
| **Address** | 0x8223E028 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs when state machine completes (r29 > 6). Writes completion bytes to 0x831D5348 and 0x831D5337. |
| **Original function** | State machine completion handler |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

---

## 5. Readiness & Signal Hooks

### 5.1 sub_8224FA48 (readiness reader)

| Field | Value |
|-------|-------|
| **Address** | 0x8224FA48 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs return value and dword at 0x82BF9B70. Returns 0 when value is -1 (no dialog pending = advance), 1 when >= 0 (dialog pending = loop). |
| **Original function** | Reads 0x82BF9B70 to determine XAM dialog readiness. Used by state 3 and every iteration. |
| **Status** | Working (diagnostic only). Natural default of -1 is CORRECT. |
| **Still needed?** | No -- pure diagnostic |

### 5.2 sub_82254FE0 (ready-signal writer)

| Field | Value |
|-------|-------|
| **Address** | 0x82254FE0 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs entry with full register dump (r3-r10), logs 0x82BF9B70 value after call |
| **Original function** | The ONLY function that sets 0x82BF9B70 to a non-negative value (writes 1 = "ready"). Called by XAM dialog completion flow. If never called, state 3 loops forever. |
| **Status** | Working (diagnostic only). The function fires naturally via RexGlue XAM. |
| **Still needed?** | No -- pure diagnostic, but useful to keep for debugging if XAM flow breaks |

### 5.3 sub_8214C8C8 (ready-counter incrementer)

| Field | Value |
|-------|-------|
| **Address** | 0x8214C8C8 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs 0x82BF9B70 before/after (tracks increment toward 4) |
| **Original function** | Increments the readiness counter at 0x82BF9B70 |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 5.4 sub_8224FA38 (ready reset -- writes -1)

| Field | Value |
|-------|-------|
| **Address** | 0x8224FA38 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs reset of 0x82BF9B70 to -1 |
| **Original function** | Resets readiness dword to -1 ("not ready") |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 5.5 sub_8223F9F0 (XAM dialog flow)

| Field | Value |
|-------|-------|
| **Address** | 0x8223F9F0 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs first 5 calls |
| **Original function** | Big function that calls sub_82254FE0 many times (XAM dialog orchestration) |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 5.6 sub_8214B168 (post-readiness check)

| Field | Value |
|-------|-------|
| **Address** | 0x8214B168 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs return value periodically |
| **Original function** | Called in sub_8214C8C8 after readiness check |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

---

## 6. State Machine Support Hooks

### 6.1 sub_82849918 (yield/sleep)

| Field | Value |
|-------|-------|
| **Address** | 0x82849918 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs call count periodically |
| **Original function** | Yield/sleep called each state machine iteration |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 6.2 sub_821B4108 (active player count)

| Field | Value |
|-------|-------|
| **Address** | 0x821B4108 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs return value (count of active players from pool at 0x82B29F18) |
| **Original function** | Iterates player pool, counts active players |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 6.3 sub_8223CAD8 (state 3 init)

| Field | Value |
|-------|-------|
| **Address** | 0x8223CAD8 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs first 5 calls |
| **Original function** | Called at start of state 3 |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 6.4 sub_82219AC0 (player check -- byte 361)

| Field | Value |
|-------|-------|
| **Address** | 0x82219AC0 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs r3 value periodically |
| **Original function** | Called with player struct in state 3, checks byte 361 |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

### 6.5 sub_821B6FD0 (multiplayer notification check)

| Field | Value |
|-------|-------|
| **Address** | 0x821B6FD0 |
| **Hook type** | Wrap |
| **What it does** | Diagnostic: logs return value (must be 0 for state 3 to advance) |
| **Original function** | Multiplayer notification check (state 3 gate 3) |
| **Status** | Working (diagnostic only) |
| **Still needed?** | No -- pure diagnostic |

---

## 7. save_hooks.cpp -- State Machine Hooks

### 7.1 sub_821200D0 (post-init: profiles/saves)

| Field | Value |
|-------|-------|
| **Address** | 0x821200D0 |
| **Hook type** | `PPC_FUNC` -- COMPLETE OVERRIDE with pre-call fixes |
| **What it does** | PRE: Forces KernelPhase to Runtime (breaks circular dependency). Clears LOADING_STEP_ADDR (0x83137BC9) to 0 so Loop 1 (busy-wait on sub_82124490) exits immediately. Then calls `__imp__sub_821200D0`. POST: Logs exit. |
| **Original function** | Post-init entry: Loop 1 waits for rendering thread to clear BC9 (loading screen gate). Then runs save state machine via sub_82121E80. Then does media loading + render loop 2 + shutdown. |
| **Bug being fixed** | Loop 1: On Xbox 360, sub_821238D0 runs on a dedicated rendering thread and clears BC9 at stage 3. In recomp, no rendering thread exists, so BC9 never clears -> infinite spin. |
| **Historical note** | Previously forced save state=17 + retval=3 ("new game") to bypass save state machine entirely. This broke XAM dialog flow (sub_8223F9F0 / sub_82254FE0 never fired), leaving readiness dword at -1 and trapping state 3 forever. Now lets RexGlue XAM run naturally. |
| **Status** | WORKING -- critical fix |
| **Still needed?** | YES -- Loop 1 bypass is essential. Without it, game hangs at loading screen. |

### 7.2 sub_8219F728 (active player slot counter)

| Field | Value |
|-------|-------|
| **Address** | 0x8219F728 |
| **Hook type** | `PPC_FUNC` -- COMPLETE REPLACEMENT |
| **What it does** | Returns 1 (user 0 active) unconditionally. Never calls `__imp__`. |
| **Original function** | Iterates 4 player slot structs at 0x82ACBD60 (stride 188). For each slot: reads inner_ptr = [slot+0], checks byte[inner_ptr+36]. Counts non-zero results. |
| **Bug being fixed** | On Xbox 360, player slot inner_ptrs point to live OS objects. In recomp, all inner_ptrs are null (no XNotify sign-in events). Reading byte[0+36] = 0 for every slot -> returns 0. sub_821E6508 gates on this and returns 0 immediately when count=0, trapping sub_82121E80's yield loop forever. |
| **Status** | WORKING -- critical fix |
| **Still needed?** | YES -- without this, save state machine never enters. A holistic rewrite must either keep this or properly populate player slot inner_ptrs. |

### 7.3 sub_8218C2C0 (loading complete check)

| Field | Value |
|-------|-------|
| **Address** | 0x8218C2C0 |
| **Hook type** | `PPC_FUNC` -- COMPLETE REPLACEMENT |
| **What it does** | Returns 1 ("loading complete") unconditionally |
| **Original function** | Calls sub_82856FE0 -> sub_82878998 (VBlank hardware check). If that returns 0, falls back to checking dword_82A2C54C == 16. |
| **Bug being fixed** | sub_82856FE0 depends on Xbox 360 VBlank hardware. Always returns 0 in recomp, trapping Loop 2 in sub_821200D0 in an infinite render loop. |
| **Status** | WORKING -- critical fix |
| **Still needed?** | YES -- VBlank hardware doesn't exist. Must signal loading complete. |

### 7.4 sub_82192E00 (streaming init)

| Field | Value |
|-------|-------|
| **Address** | 0x82192E00 |
| **Hook type** | `PPC_FUNC` -- wrap with post-call fix |
| **What it does** | Calls `__imp__sub_82192E00`, then zeros 0x830F5820 (streaming-pending flag) |
| **Original function** | Marks streaming job as in-flight by writing to 0x830F5820. On Xbox 360, hardware streaming completion thread clears this. |
| **Bug being fixed** | RexGlue VFS is synchronous -- no thread clears the flag. Downstream sub_827DE648 spins waiting for 0x830F5820 == 0. |
| **Status** | WORKING -- critical fix |
| **Still needed?** | YES -- streaming flag must be cleared for synchronous VFS |

### 7.5 sub_827DE648 (streaming completion barrier)

| Field | Value |
|-------|-------|
| **Address** | 0x827DE648 |
| **Hook type** | `PPC_FUNC` -- COMPLETE REPLACEMENT |
| **What it does** | Returns immediately (no-op). Belt-and-suspenders fallback. |
| **Original function** | Spins while 0x830F5820 != 0 |
| **Status** | WORKING -- fallback safety |
| **Still needed?** | YES -- prevents hang if streaming flag is somehow not cleared |

### 7.6 sub_829A1C38, sub_829A1CA0, sub_829A1CB8, sub_8297A930, sub_82122CA0

These are save system hooks (content creation, close, enumeration, save manager, save system init). All are diagnostic wrappers that call `__imp__` and log. Not directly related to the state machine progression but are in the call path.

| Address | Role | Hook type |
|---------|------|-----------|
| 0x829A1C38 | Content creation wrapper | Diagnostic wrap |
| 0x829A1CA0 | Content close wrapper | Diagnostic wrap |
| 0x829A1CB8 | Content enumeration wrapper | Diagnostic wrap |
| 0x8297A930 | Save manager orchestrator | Diagnostic wrap |
| 0x82122CA0 | Save system init (3 save slots) | Diagnostic wrap |

All are pure diagnostics. None are needed for a holistic rewrite unless save system debugging is required.

---

## 8. Functions Referenced But NOT Hooked

### 8.1 sub_8223DAA0 (readiness check)

Referenced in comments on sub_822422E0 and sub_82242910 but NOT directly hooked.
This is the function whose behavior differs between Xbox 360 and recomp:
- Xbox 360: Returns 0 during scene creation (device enumeration takes time)
- Recomp: Returns 1 immediately (RexGlue reports devices as ready)
- This timing mismatch causes the fast path (state 0->4) in sub_82242910

**Impact**: The sub_82242910 hook works around this by intercepting the 0->4 jump.
A holistic fix could either hook sub_8223DAA0 directly (force return 0 during
scene creation) or fix the root cause in RexGlue device timing.

### 8.2 sub_8223DB20 (used by state 4+)

Referenced in sub_82242910 comments. Both return paths are valid. Not hooked.

### 8.3 sub_8223F308 (scene gate)

Referenced in sub_82242910 comments. Called when platformMode is 3 or 4. Not hooked.

---

## 9. Summary: Essential vs. Diagnostic Hooks

### ESSENTIAL (must keep or replace in holistic rewrite)

| Hook | Address | Type | Why essential |
|------|---------|------|---------------|
| sub_82140000 | 0x82140000 | Re-init guard | Prevents RAGE engine double-init hang |
| sub_8218BEA8 | 0x8218BEA8 | Main loop driver | Drives the entire game frame loop |
| sub_821200D0 | 0x821200D0 | Loop 1 bypass | Clears BC9 loading flag (no rendering thread) |
| sub_8219F728 | 0x8219F728 | Player count | Returns 1 (no Xbox sign-in events in recomp) |
| sub_8218C2C0 | 0x8218C2C0 | Loading complete | Returns 1 (no VBlank hardware) |
| sub_82192E00 | 0x82192E00 | Streaming flag | Clears sync flag (VFS is synchronous) |
| sub_827DE648 | 0x827DE648 | Streaming barrier | Safety no-op |
| sub_822440F8 | 0x822440F8 | State 4 bypass | Returns 2 (no Xbox save device selection) |
| sub_822422E0 | 0x822422E0 | State 5 fix | Resets stale 0x82BF9834 value |
| sub_82242910 | 0x82242910 | Scene creation | Forces normal path, sets platformMode=3 |
| sub_821406C8 | 0x821406C8 | Player slot populate | Seeds content-readiness fields for state 3 |

### DIAGNOSTIC ONLY (safe to remove)

| Hook | Address | Purpose |
|------|---------|---------|
| xstart | CRT entry | Log entry/exit |
| sub_82A18BE0 | 0x82A18BE0 | Log firmware check |
| sub_82A18B08 | 0x82A18B08 | Log firmware init |
| sub_82A18620 | 0x82A18620 | Log notification callbacks |
| sub_82A110A8 | 0x82A110A8 | Log XEX privilege check |
| sub_82140088 | 0x82140088 | Log game loop entry |
| sub_821B3CE8 | 0x821B3CE8 | Log RAGE engine init |
| sub_821411D8 | 0x821411D8 | Log game systems init |
| sub_821458B8 | 0x821458B8 | Log init gate |
| sub_821B39A8 | 0x821B39A8 | Log quit flag |
| sub_82142230 | 0x82142230 | Log state machine entry |
| sub_822414E8 | 0x822414E8 | Log state 0 |
| sub_8223DDA8 | 0x8223DDA8 | Log state 1 |
| sub_8223DEE8 | 0x8223DEE8 | Log state 2 |
| sub_82142F90 | 0x82142F90 | Log frame update |
| sub_822438B0 | 0x822438B0 | Log state 6 inner |
| sub_8223E028 | 0x8223E028 | Log state machine exit |
| sub_82241370 | 0x82241370 | Log pre-state setup |
| sub_821428C8 | 0x821428C8 | Log per-iteration |
| sub_82849918 | 0x82849918 | Log yield/sleep |
| sub_821B4108 | 0x821B4108 | Log player count |
| sub_8223CAD8 | 0x8223CAD8 | Log state 3 init |
| sub_82219AC0 | 0x82219AC0 | Log player check |
| sub_821B6FD0 | 0x821B6FD0 | Log MP notification |
| sub_8224FA48 | 0x8224FA48 | Log readiness reader |
| sub_82254FE0 | 0x82254FE0 | Log ready-signal writer |
| sub_8214C8C8 | 0x8214C8C8 | Log ready-counter |
| sub_8224FA38 | 0x8224FA38 | Log ready reset |
| sub_8223F9F0 | 0x8223F9F0 | Log XAM dialog flow |
| sub_8214B168 | 0x8214B168 | Log post-readiness |
| sub_829A1C38 | 0x829A1C38 | Log content creation |
| sub_829A1CA0 | 0x829A1CA0 | Log content close |
| sub_829A1CB8 | 0x829A1CB8 | Log content enumeration |
| sub_8297A930 | 0x8297A930 | Log save manager |
| sub_82122CA0 | 0x82122CA0 | Log save system init |

---

## 10. Key State Variables (Guest Memory Addresses)

| Address | Name | Usage |
|---------|------|-------|
| 0x82BF9834 | State 5 completion | Written 2 by sub_822422E0; read by sub_82242910 state 4 |
| 0x82BF9838 | State 6 inner state | sub_822438B0's state counter (8 states) |
| 0x82BF9844 | Platform mode | Must be 3 (base game) or 4 for scene creation |
| 0x82BF9848 | Scene creation state | sub_82242910's state counter (15 states) |
| 0x82BF3A88 | Scene object pointer | Written during scene creation |
| 0x82BF9B70 | Readiness dword | -1=not ready, >=0=ready. Written by sub_82254FE0 |
| 0x82A9546C | Error code | Set during scene creation states |
| 0x82A95478 | Player/episode index | 0=base game, others=DLC episodes |
| 0x82A95474 | Profile index | Active profile |
| 0x82A9172C | Active player index | -1=none, 0+=player slot index |
| 0x831C2458 | Global scene pointer | NULL until scene creation completes |
| 0x82B94554 | Save state machine step | 0-17 |
| 0x82B946C8 | Save return value | 1=load save, 2=start loading, 3=new game |
| 0x83137BC9 | Loading step (BC9) | Non-zero=still loading, 0=done |
| 0x83137BB7 | Loading flag (BB7) | Loading in progress |

---

## 11. Observations for Holistic Rewrite

1. **Root cause**: Most state machine hooks exist because RexGlue XAM completes
   device operations instantly (no physical hardware delay), while the game
   expects multi-frame delays. A single fix to sub_8223DAA0 (delay device
   readiness for N frames) could eliminate the sub_82242910 and sub_822422E0
   hooks.

2. **Player slot population** (sub_821406C8) is a separate concern. The game
   expects XAM notification callbacks to populate these fields over multiple
   frames. A proper fix would implement these notification callbacks in
   RexGlue or a Liberty notification module.

3. **Save device bypass** (sub_822440F8) is fundamentally correct for PC --
   there are no Xbox controllers or storage devices. This should be kept but
   possibly enhanced to support VFS-based save enumeration.

4. **30+ diagnostic hooks** add overhead and log noise. They should be
   stripped or gated behind a debug flag in a production build.

5. **sub_8218BEA8's infinite loop** is crude. The holistic rewrite should
   implement proper frame pacing tied to VSync/SDL events rather than
   `sleep_for(16ms)`.
