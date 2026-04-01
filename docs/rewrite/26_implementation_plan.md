# 26 - Implementation Plan: Scene State Machine Replacement

## Overview

This document specifies exactly what files to create, what files to modify, and
what each hook function does. It follows the Pattern 4 (Namespace Replacement)
established by `game_init.cpp` / `game_init.h`.

The goal is to replace the fragile wrap+patch hooks for the scene creation path
with a structured module that calls essential sub-functions directly, skipping
all Xbox 360 gate functions.

---

## 1. NEW FILES TO CREATE

### 1.1 `LibertyRecomp/kernel/scene_state_machine.h`

**Namespace: `SceneGlobals`** (constants) + **`SceneStateMachine`** (functions)

```
SceneGlobals (constexpr address constants):
    // =========================================================================
    // State Variables
    // =========================================================================
    SCENE_CREATE_STATE     = 0x82BF9848   // sub_82242910 state (0-14), lwz r26(-32064) + (-26552)
    OUTER_SM_STATE         = 0x82BF9838   // sub_822438B0 state (0-7), lwz r31(-32064) + (-26568)
    STATE5_DONE_FLAG       = 0x82BF9834   // sub_822422E0 done flag, lwz r30(-32064) + (-26572)
    PLATFORM_MODE          = 0x82BF9844   // platformMode, lwz r29(-32064) + (-26556)
    PENDING_VALUE          = 0x82BF99CC   // scene store/handle, lwz r31(-32064) + (-26164)

    // =========================================================================
    // Error & Index Variables
    // =========================================================================
    ERROR_CODE             = 0x82A9546C   // error code word, stw r10(-32087) + 21612
    CONTENT_BYTE_FLAG      = 0x82A95466   // content byte flag, u8, stb r11(-32087) + 21606
    PLAYER_EPISODE_IDX     = 0x82A95478   // active player/episode index, lwz r17(-32087) + 21624
    PROFILE_INDEX          = 0x82A95474   // active profile index, lwz r28(-32087) + 21620

    // =========================================================================
    // Flag Bytes
    // =========================================================================
    RESTART_FLAG           = 0x82BF981E   // done/restart flag byte, lbz r31(-32064) + (-26594)
    SCENE_LOADED_FLAG      = 0x82BF981F   // scene loaded flag byte, lbz r11(-32064) + (-26593)
    SCENE_BYTE_FLAG        = 0x82BF3A76   // cleared in scene state 5, stb r10(-32065) + 14966
    STATE13_FLAG_BYTE      = 0x82BF3CDA   // cleared in state 13, stb r10(-32065) + 15578

    // =========================================================================
    // Pointers & Structs
    // =========================================================================
    SCENE_OBJ_PTR          = 0x82BF3A88   // scene object pointer, r30(-32065) + 14984
    SCENE_NAME             = 0x83192C50   // scene manager "this", lis(-31975) + 11344
    SCENE_INFO_PTR         = 0x82BF3D90   // frequently loaded param, lwz r11(-32065) + 15760
    SCENE_STRUCT_BASE      = 0x82BF3A60   // r28 base for sub_822417B0, addi r11(-32065) + 14944
    SCENE_PARAM_BUF        = 0x82BF3A78   // scene creation struct, addi r10(-32065) + 14968
    STATE_SUBSTRUCT        = 0x82BF99C8   // r29 in sub_82242910, addi r11(-32064) + (-26168)
    LOAD_PROGRESS_PTR      = 0x82BF3D94   // state 10 param ptr, addi r11(-32065) + 15764
    RESOURCE_CB_PTR        = 0x82BF3D98   // state 9 string/param, addi r11(-32065) + 15768

    // =========================================================================
    // Outer State Machine / Global Scene
    // =========================================================================
    GLOBAL_SCENE_PTR       = 0x831C2458   // scene pointer (global), lis(-31972) + 9304
    ACTIVE_PLAYER_SLOT     = 0x82A9172C   // active player slot index, lis(-32087) + 5932
    XAM_READINESS          = 0x82BF9B70   // XAM dialog readiness dword, lis(-32064) + (-25744)

SceneStateMachine (function declarations):
    // Phase functions — one per logical group of original states
    int  RunPhase(PPCContext& ctx, uint8_t* base);  // Main entry, called each frame
    void Reset();                                     // Reset internal state for new scene
```

### 1.2 `LibertyRecomp/kernel/scene_state_machine.cpp`

**Structure**: Follows `game_init.cpp` pattern exactly.

```
File header comment:
    // =============================================================================
    // Scene State Machine Module
    // =============================================================================
    // Replaces sub_82242910 (0x82242910) — the 15-state scene creation sub-machine.
    //
    // Original call tree:
    //   sub_82242910 (Scene Creation SM — 15 states)
    //   ├── States 0-3: Xbox gate checks (sub_8223DAA0, sub_826CBA70, sub_8223F9F0)
    //   ├── State 4:    Platform mode switch + sub_8223F308 (scene params)
    //   ├── State 5:    Content enumeration (sub_8223F9F0)
    //   ├── State 6:    Scene creation (sub_8284AAE0)
    //   ├── State 7:    Content retry path (sub_8223CB60, sub_8223F9F0)
    //   ├── State 8:    Scene load monitor (sub_8284AB10, sub_8284AB70, sub_8284B490, sub_8284B430)
    //   ├── State 9:    Resource load start (sub_8223D2F0, sub_8284ABA0)
    //   ├── State 10:   Resource load poll (sub_8284ABD0, sub_8284ABF8, sub_8284B490, sub_8284B430)
    //   ├── State 11:   Post-load check (sub_8223D400, platformMode validation)
    //   ├── States 12,14: Save data (sub_822417B0, sub_8223F790, sub_8223CAD8)
    //   └── State 13:   Error recovery (sub_8223F9F0)
    //
    // Strategy:
    //   - SKIP states 0-3 entirely (all Xbox gate functions)
    //   - Call sub_8223F308 directly (state 4 core logic)
    //   - Call scene creation / resource loading functions in sequence (states 6-10)
    //   - Call post-load + save data functions (states 11-14)
    //   - Write all required side effects to guest memory
    //   - Return 1 (working) each frame, 0 (done) at completion, 2 (error) on failure
    // =============================================================================

External PPC declarations:
    extern "C" void __imp__sub_8284B4B0(PPCContext&, uint8_t*);  // set scene name (state 0)
    extern "C" void __imp__sub_8223F308(PPCContext&, uint8_t*);  // scene param setup (state 4)
    extern "C" void __imp__sub_8284AAE0(PPCContext&, uint8_t*);  // create scene (state 6)
    extern "C" void __imp__sub_8284AB10(PPCContext&, uint8_t*);  // check scene started (state 8)
    extern "C" void __imp__sub_8284AB70(PPCContext&, uint8_t*);  // advance scene creation (state 8)
    extern "C" void __imp__sub_8284B490(PPCContext&, uint8_t*);  // get scene status (state 8, 10)
    extern "C" void __imp__sub_8284B430(PPCContext&, uint8_t*);  // get scene result (state 8, 10)
    extern "C" void __imp__sub_8223D2F0(PPCContext&, uint8_t*);  // pre-load setup (state 9)
    extern "C" void __imp__sub_8284ABA0(PPCContext&, uint8_t*);  // begin resource load (state 9)
    extern "C" void __imp__sub_8284ABD0(PPCContext&, uint8_t*);  // poll resource load (state 10)
    extern "C" void __imp__sub_8284ABF8(PPCContext&, uint8_t*);  // advance resource load (state 10)
    extern "C" void __imp__sub_8223D400(PPCContext&, uint8_t*);  // post-load check (state 11)
    extern "C" void __imp__sub_822417B0(PPCContext&, uint8_t*);  // save data ops (state 12, 14)
    extern "C" void __imp__sub_8223F790(PPCContext&, uint8_t*);  // network session final (state 14)
    extern "C" void __imp__sub_8223CAD8(PPCContext&, uint8_t*);  // cleanup/teardown (state 14)
    extern "C" void __imp__sub_8223DB90(PPCContext&, uint8_t*);  // final cleanup (state 13/14)
    extern "C" void __imp__sub_82240AB0(PPCContext&, uint8_t*);  // pre-load setup (state 3)

Helper macros:
    PPC_STORE_U8, PPC_STORE_U32, PPC_LOAD_U32, PPC_LOAD_U8
    (same as game_init.cpp, defined locally)

Internal state:
    static int s_phase = 0;  // Simplified phase counter (0-8)
```

#### Phase Decomposition

The 15 original states collapse into 9 sequential phases:

| Phase | Replaces States | What It Does | Returns |
|-------|----------------|--------------|---------|
| 0 | 0-4 | Initialize: set platformMode=3, error=6, call sub_8223F308, call sub_8284B4B0, call sub_82240AB0 | 1 (working) |
| 1 | 5 | Content decision: clear SCENE_BYTE_FLAG, set PENDING_VALUE from SCENE_PARAM_BUF+4 | 1 (working) |
| 2 | 6 | Scene creation: call sub_8284AAE0 with all 6 args | 1 (working) or 2 (error 7) |
| 3 | 8 | Scene load monitor: call sub_8284AB10, sub_8284AB70, sub_8284B490, sub_8284B430 | 1 (working) or 2 (error) |
| 4 | 9 | Resource load start: call sub_8223D2F0, sub_8284ABA0 | 1 (working) or 2 (error 14) |
| 5 | 10 | Resource load poll: call sub_8284ABD0, sub_8284ABF8, sub_8284B490, sub_8284B430 | 1 (working) or 2 (error) |
| 6 | 11 | Post-load: call sub_8223D400, validate platformMode in {0,1,3,4} | 1 (working) or 2 (error) |
| 7 | 12 | Save data pass 1: call sub_822417B0(r3=0, r4=1, ...) | 1 (working) or advance |
| 8 | 14 | Save data pass 2 + finalization: call sub_822417B0(r3=0, r4=0, ...), sub_8223F790, sub_8223CAD8, sub_8223DB90 | 0 (done) |

#### Hook Registration (at file scope, outside namespace)

```cpp
extern "C" void __imp__sub_82242910(PPCContext& ctx, uint8_t* base);
PPC_FUNC_HOOK(sub_82242910)
{
    int result = SceneStateMachine::RunPhase(ctx, base);
    ctx.r3.s64 = result;
}
```

#### Detailed Phase 0 Implementation (most critical)

```cpp
// Phase 0 replaces original states 0-4
// States 0-3 are ALL Xbox gate functions (sub_8223DAA0, sub_826CBA70, sub_8223F9F0)
// State 4 is the core: platformMode switch + sub_8223F308 call
//
// Original state 4 logic:
//   1. sub_8223DB20() abort check -> skip (always 0 in recomp)
//   2. PENDING_VALUE = 0
//   3. Read PLATFORM_MODE: if (mode - 3) <= 1, call sub_8223F308(1, SCENE_PARAM_BUF)
//      and copy SCENE_PARAM_BUF[+4] to PENDING_VALUE
//   4. Check RESTART_FLAG: if nonzero, advance to state 5
//   5. sub_82240B08() check: if nonzero, advance to state 9
//   6. Inner switch on PLATFORM_MODE for error validation
//   7. If no error: advance to state 5

void Phase0_Initialize(PPCContext& ctx, uint8_t* base) {
    LOG_WARNING("[SceneSM] Phase 0: Initialize (replaces states 0-4)");

    // 1. Set platformMode = 3 (base game) — BEFORE any state checks
    PPC_STORE_U32(SceneGlobals::PLATFORM_MODE, 3);

    // 2. Clear error code to 6 (what states 1-3 naturally produce)
    PPC_STORE_U32(SceneGlobals::ERROR_CODE, 6);

    // 3. Clear state 5 done flag (prevent error 34 from stale value)
    PPC_STORE_U32(SceneGlobals::STATE5_DONE_FLAG, 0);

    // 4. PENDING_VALUE = 0 (original state 4 init)
    PPC_STORE_U32(SceneGlobals::PENDING_VALUE, 0);

    // 5. Call sub_8284B4B0(SCENE_NAME, [SCENE_INFO_PTR]) — set scene name
    //    This is what state 0 does on the fast path (sub_8223DAA0 returns ready)
    ctx.r3.u64 = SceneGlobals::SCENE_NAME;
    ctx.r4.u64 = PPC_LOAD_U32(SceneGlobals::SCENE_INFO_PTR);
    __imp__sub_8284B4B0(ctx, base);

    // 6. Call sub_82240AB0() — pre-load setup (what state 3 does when ready)
    __imp__sub_82240AB0(ctx, base);

    // 7. Call sub_8223F308(r3=1, r4=SCENE_PARAM_BUF) — scene parameter setup
    //    Only called when platformMode is 3 or 4 (we set it to 3)
    ctx.r3.s64 = 1;
    ctx.r4.u64 = SceneGlobals::SCENE_PARAM_BUF;
    __imp__sub_8223F308(ctx, base);

    // 8. Copy scene result to PENDING_VALUE
    uint32_t sceneResult = PPC_LOAD_U32(SceneGlobals::SCENE_PARAM_BUF + 4);
    PPC_STORE_U32(SceneGlobals::PENDING_VALUE, sceneResult);

    // 9. Clear RESTART_FLAG (we are not restarting)
    PPC_STORE_U8(SceneGlobals::RESTART_FLAG, 0);

    // 10. Update the original state counter for diagnostic/downstream readers
    PPC_STORE_U32(SceneGlobals::SCENE_CREATE_STATE, 5);
}
```

---

## 2. EXISTING FILES TO MODIFY

### 2.1 `LibertyRecomp/kernel/imports.cpp`

#### Hooks to REMOVE (replaced by scene_state_machine.cpp)

| Address | Function | Lines | Reason for Removal |
|---------|----------|-------|-------------------|
| 0x82242910 | sub_82242910 | ~1814-1866 | Fully replaced by `SceneStateMachine::RunPhase()` |
| 0x822422E0 | sub_822422E0 | ~1772-1791 | Defense-in-depth no longer needed; scene SM writes STATE5_DONE_FLAG=0 in Phase 0 |

**How to remove**: Delete the `PPC_FUNC_HOOK(sub_82242910)` block (lines ~1793-1866) and the `PPC_FUNC_HOOK(sub_822422E0)` block (lines ~1760-1791). Also remove the `extern "C" void __imp__sub_82242910` and `__imp__sub_822422E0` declarations, and the `s_sceneCreateCount` atomic counter.

#### Hooks to KEEP (still needed)

| Address | Function | Why Keep |
|---------|----------|----------|
| 0x821406C8 | sub_821406C8 | Player slot content-readiness population. State 3 of sub_82142230 needs this. Unrelated to scene creation. |
| 0x822440F8 | sub_822440F8 | State 4 inner bypass (save device selection). Clean replacement, works well. Unrelated to scene creation. |
| 0x822438B0 | sub_822438B0 | Diagnostic wrap only. Keep for logging state 6 inner transitions. No behavior change. Optional. |
| 0x82254FE0 | sub_82254FE0 | Diagnostic wrap of XAM dialog completion. Keep for debugging XAM flow. Optional. |
| 0x82142230 | sub_82142230 | Diagnostic wrap of front-end SM. Optional. |
| 0x82142F90 | sub_82142F90 | Diagnostic wrap of frame update. Optional. |
| 0x8223E028 | sub_8223E028 | Diagnostic wrap of state machine exit. Optional. |

#### Other Changes to imports.cpp

Add include at the top (near line 39 where `game_init.h` is included):
```cpp
#include "scene_state_machine.h"
```

### 2.2 `LibertyRecomp/kernel/save_hooks.cpp`

**No changes needed.** All hooks in save_hooks.cpp address different concerns:
- sub_821200D0 (Loop 1 bypass) -- essential, unrelated
- sub_8219F728 (player count) -- essential, unrelated
- sub_8218C2C0 (loading complete) -- essential, unrelated
- sub_82192E00 (streaming flag) -- essential, unrelated
- sub_827DE648 (streaming barrier) -- essential, unrelated

### 2.3 `LibertyRecomp/CMakeLists.txt`

Add to `LIBERTY_RECOMP_KERNEL_CXX_SOURCES` (after `"kernel/game_init.cpp"` on line 105):
```cmake
"kernel/scene_state_machine.cpp"
```

---

## 3. HOOK FUNCTION SPECIFICATION

### 3.1 PPC_FUNC_HOOK(sub_82242910) -- FULL REPLACEMENT

| Field | Value |
|-------|-------|
| **Address** | 0x82242910 |
| **File** | scene_state_machine.cpp |
| **Type** | Complete replacement (never calls `__imp__sub_82242910`) |
| **Return convention** | r3: 0=done, 1=working, 2=error |
| **Called by** | sub_822438B0 state 2 |

**Internal phases** (each runs once per frame call):

#### Phase 0: Initialize (replaces original states 0-4)
- Writes: PLATFORM_MODE=3, ERROR_CODE=6, STATE5_DONE_FLAG=0, PENDING_VALUE=0, RESTART_FLAG=0, SCENE_CREATE_STATE=5
- Calls: `__imp__sub_8284B4B0` (set scene name), `__imp__sub_82240AB0` (pre-load setup), `__imp__sub_8223F308` (scene params)
- Xbox patterns skipped: sub_8223DAA0 (device check), sub_826CBA70 (network check), sub_8223F9F0 (XAM dialog), sub_8223DB20 (sign-in check), sub_82240B08 (platform readiness)
- Returns: 1 (advance to Phase 1)

#### Phase 1: Content Decision (replaces original state 5)
- Writes: SCENE_BYTE_FLAG=0, SCENE_CREATE_STATE=6
- Reads: RESTART_FLAG -- if set, treats content as available
- Xbox patterns skipped: sub_8223DB20, sub_8223F9F0 (content enumeration mode 2)
- Logic: We always have content (no Xbox content enumeration needed)
- Returns: 1 (advance to Phase 2)

#### Phase 2: Scene Creation (replaces original state 6)
- Calls: `__imp__sub_8284AAE0(SCENE_NAME, [SCENE_INFO_PTR], 1, PENDING_VALUE, 1, CONTENT_BYTE_FLAG)`
- Writes: ERROR_CODE=7 on failure, SCENE_CREATE_STATE=8 on success
- Xbox patterns skipped: sub_8223DB20, sub_826CBA70
- Game logic preserved: The actual scene instantiation call with all 6 parameters
- Returns: 1 (working) or 2 (error 7)

#### Phase 3: Scene Load Monitor (replaces original state 8)
- Calls: `__imp__sub_8284AB10`, `__imp__sub_8284AB70`, `__imp__sub_8284B490`, `__imp__sub_8284B430`
- Writes: ERROR_CODE=8 or 9 on failure, SCENE_CREATE_STATE=9 on success
- Xbox patterns skipped: sub_8223DB20 (abort check)
- Game logic preserved: Scene load polling + status check loop
- Returns: 1 (still loading) or 1 (advance to Phase 4) or 2 (error)

Note: sub_82240B08 in state 8 can return nonzero meaning "restart from state 0".
We call it; if nonzero, reset s_phase=0. This is pure game logic.

#### Phase 4: Resource Load Start (replaces original state 9)
- Calls: `__imp__sub_8223D2F0` (pre-load), `__imp__sub_8284ABA0(SCENE_NAME, [SCENE_INFO_PTR], 1, RESOURCE_CB_PTR, 75)`
- Writes: ERROR_CODE=14 on failure, SCENE_CREATE_STATE=10 on success
- Xbox patterns skipped: sub_8223DB20, sub_82240B78
- Returns: 1 (working) or 2 (error 14)

#### Phase 5: Resource Load Poll (replaces original state 10)
- Calls: `__imp__sub_8284ABD0`, `__imp__sub_8284ABF8`, `__imp__sub_8284B490`, `__imp__sub_8284B430`
- Writes: ERROR_CODE=15 or 16 on failure, SCENE_CREATE_STATE=11 on success
- Xbox patterns skipped: sub_8223DB20, sub_82240B78
- Game logic preserved: Resource load polling loop
- PLATFORM_MODE check: modes {0,1,3,4} advance to Phase 6; mode 2 -> call sub_8223CAD8 + return 0 (done)
- Returns: 1 (still loading) or 1 (advance) or 2 (error)

#### Phase 6: Post-Load Processing (replaces original state 11)
- Calls: `__imp__sub_8223D400` (post-load check)
- Writes: SCENE_CREATE_STATE=12 on success
- Xbox patterns skipped: sub_8223DB20, sub_82240B78
- PLATFORM_MODE check: modes {3,4} advance to Phase 7; modes {0,1} -> call sub_8223CAD8 + return 0 (done)
- Returns: 1 (working) or 0 (done for non-MP modes)

#### Phase 7: Save Data Pass 1 (replaces original state 12)
- Calls: `__imp__sub_822417B0(r3=0, r4=1, r5=SCENE_STRUCT_BASE, r6=PENDING_VALUE, r7=STATE_SUBSTRUCT, r8=&local1, r9=&local2)`
- Note: r8/r9 are stack-allocated output buffers. Implementation uses static uint32_t variables at known guest addresses, or allocates from the PPC stack (ctx.r1 - 16, ctx.r1 - 20).
- Writes: SCENE_CREATE_STATE=14 on success
- Returns: 1 (advance) or 2 (error from sub_822417B0 returning 2)

#### Phase 8: Save Data Pass 2 + Finalization (replaces original state 14)
- Calls: `__imp__sub_822417B0(r3=0, r4=0, ...)` (note: r8/r9 args swapped from Phase 7)
- Post-checks:
  - Read [STATE_SUBSTRUCT]: if negative, enter error recovery (Phase 0 restart after sub_8223DB90)
  - Check CONTENT_BYTE_FLAG and SCENE_LOADED_FLAG for sub_8223F790 call conditions
  - Call `__imp__sub_8223CAD8` (cleanup)
  - Clear CONTENT_BYTE_FLAG=0, STATE13_FLAG_BYTE=0
  - Call `__imp__sub_8223DB90` (final cleanup)
  - Reset SCENE_CREATE_STATE=0
- Returns: 0 (done)

### 3.2 PPC_FUNC_HOOK(sub_822422E0) -- TO BE REMOVED

Currently a wrap+patch that resets 0x82BF9834 after state 5. With the new
module writing STATE5_DONE_FLAG=0 in Phase 0, this defense-in-depth is no longer
needed. The original recompiled sub_822422E0 runs unhooked.

If desired for safety, the hook can be kept as a diagnostic-only wrapper (remove
the state reset logic, keep logging).

---

## 4. BUILD INTEGRATION

### 4.1 CMakeLists.txt Change

Single line addition to `LIBERTY_RECOMP_KERNEL_CXX_SOURCES`:

```cmake
set(LIBERTY_RECOMP_KERNEL_CXX_SOURCES
    "kernel/imports.cpp"
    ...
    "kernel/game_init.cpp"
    "kernel/scene_state_machine.cpp"    # <-- ADD THIS LINE
)
```

### 4.2 Include Dependencies

`scene_state_machine.cpp` includes:
- `<cstdint>`, `<cstdio>`, `<atomic>`
- `<stdafx.h>` (provides ByteSwap via XenonUtils)
- `<cpu/ppc_context.h>` (PPCContext struct)
- `<os/logger.h>` (LOG_WARNING, LOGF_WARNING)
- `"scene_state_machine.h"` (own header)
- `"function.h"` (PPC_FUNC_HOOK macro)

No new third-party dependencies. Uses the same headers as `game_init.cpp`.

### 4.3 Build Command (macOS)

Same as existing build:
```bash
export VCPKG_ROOT=$(pwd)/thirdparty/vcpkg
cmake --build ./out/build/macos-release --target LibertyRecompLib
cmake --build ./out/build/macos-release --target LibertyRecomp
```

---

## 5. TESTING STRATEGY

### 5.1 Log Tags

All new log messages use tag `[SceneSM]`:
```
[SceneSM] Phase 0: Initialize (replaces states 0-4)
[SceneSM] Phase 0: platformMode=3, error=6, pendingValue=0xXXXXXXXX
[SceneSM] Phase 2: Scene creation call result=N
[SceneSM] Phase 3: Scene load status=N, result=N
[SceneSM] Phase 4: Resource load initiated (75 items)
[SceneSM] Phase 5: Resource load poll — still loading
[SceneSM] Phase 6: Post-load check result=N
[SceneSM] Phase 7: sub_822417B0 pass 1 result=N
[SceneSM] Phase 8: sub_822417B0 pass 2 result=N, finalizing...
[SceneSM] COMPLETE: Scene creation done (total frames=N)
```

### 5.2 What to Look For in Output

**SUCCESS indicators** (in order of appearance):
1. `[SceneSM] Phase 0:` appears — module is active
2. `[STATE-6-INNER] sub_822438B0 ... state=1->2` — outer SM entered state 2 (calls us)
3. `[SceneSM] Phase 2: Scene creation call result=1` — sub_8284AAE0 succeeded
4. `[SceneSM] Phase 3:` appears multiple times — scene loading is polling
5. `[SceneSM] Phase 5:` appears — resource loading started
6. `[SceneSM] COMPLETE:` — scene creation finished
7. `[STATE-6-INNER] sub_822438B0 ... state=2->3` — outer SM advanced past scene creation
8. `[STATE-EXIT] sub_8223E028 ENTER` — state machine completed successfully
9. Game renders the world (visual confirmation)

**FAILURE indicators**:
1. `[SceneSM] ERROR:` followed by error code — scene creation failed
2. `[STATE-6-INNER] ... err=33` — fatal error propagated to outer SM
3. Phase counter stuck at same value for 1000+ frames — deadlock
4. No `[SceneSM]` output at all — hook not linking (check extern "C")
5. SIGBUS/SIGSEGV after Phase 2 — sub_8284AAE0 accessing invalid memory

**REGRESSION checks**:
1. Existing `[STATE-4-INNER] BYPASS` still appears (sub_822440F8 hook unchanged)
2. `[STATE-3] Populated player slot` still appears (sub_821406C8 hook unchanged)
3. `[GameInit]` phases still complete successfully (game_init.cpp unchanged)
4. Frame count continues increasing after scene creation

### 5.3 Fallback Plan

If the new module fails, revert by:
1. Remove `scene_state_machine.cpp` from CMakeLists.txt
2. Restore the deleted hooks in imports.cpp
3. Rebuild

The original hooks are simple wrap+patch and can be restored from git history.

---

## 6. RISK ASSESSMENT

### 6.1 sub_822417B0 Stack Parameters

**Risk: MEDIUM.** States 12 and 14 call sub_822417B0 with r8 and r9 pointing to
stack-allocated locals (sp+84 and sp+88 in the original). The replacement must
provide valid guest-addressable memory for these output parameters.

**Mitigation**: Use the PPC stack frame. Before calling sub_822417B0:
```cpp
uint32_t sp = ctx.r1.u32;
uint32_t local84 = sp - 16;  // allocate space on PPC stack
uint32_t local88 = sp - 20;
PPC_STORE_U32(local84, 0);   // zero-initialize
PPC_STORE_U32(local88, 0);
ctx.r8.u64 = local84;
ctx.r9.u64 = local88;
```

### 6.2 sub_82240B08 in State 8

**Risk: LOW.** The original state 8 calls sub_82240B08 after scene load
completes. If it returns nonzero, the state machine restarts from state 0.
This is pure game logic (async I/O completion check), not Xbox-specific. We
should call it and respect its return value.

### 6.3 Register Corruption

**Risk: LOW.** Each `__imp__` call corrupts PPCContext registers. The
implementation must reload all parameters from global constants (not from ctx
registers) before each call. The pattern in game_init.cpp handles this
correctly.

### 6.4 Static Phase Counter

**Risk: LOW.** Using `static int s_phase` means only one scene creation can be
active at a time. This matches the original game behavior (sub_82242910's state
counter at 0x82BF9848 is a global).

### 6.5 Removing sub_822422E0 Hook

**Risk: VERY LOW.** The root cause (stale value 2 in STATE5_DONE_FLAG) is
eliminated by Phase 0 writing 0 to that address before any scene creation
logic runs. The sub_822422E0 hook was defense-in-depth for the wrap+patch
approach. With a full replacement that pre-initializes all state, it is
redundant.

---

## 7. IMPLEMENTATION ORDER

1. **Create `scene_state_machine.h`** with all SceneGlobals constants
2. **Create `scene_state_machine.cpp`** with Phase 0 only (return 1 after init, let original handle rest)
   - This is a hybrid approach: Phase 0 initializes, then calls `__imp__sub_82242910` for states 5+
   - Validates that the init path works before replacing more states
3. **Add to CMakeLists.txt**
4. **Build and test** — verify Phase 0 log output and scene creation succeeds
5. **Expand phases 1-5** (scene creation + resource loading)
6. **Build and test** — verify resource loading works
7. **Expand phases 6-8** (post-load + save data + finalization)
8. **Build and test** — verify full scene creation
9. **Remove imports.cpp hooks** for sub_82242910 and sub_822422E0
10. **Final build and test** — full regression check

---

## 8. FILE SUMMARY

| Action | File | Type |
|--------|------|------|
| CREATE | `LibertyRecomp/kernel/scene_state_machine.h` | Header with SceneGlobals constants + SceneStateMachine declarations |
| CREATE | `LibertyRecomp/kernel/scene_state_machine.cpp` | Implementation with 9 phases + PPC_FUNC_HOOK registration |
| MODIFY | `LibertyRecomp/kernel/imports.cpp` | Remove sub_82242910 hook (~50 lines), remove sub_822422E0 hook (~20 lines), add include |
| MODIFY | `LibertyRecomp/CMakeLists.txt` | Add one line to LIBERTY_RECOMP_KERNEL_CXX_SOURCES |

Total estimated new code: ~400-500 lines (header ~80, implementation ~350-420).
Total estimated removed code: ~70 lines from imports.cpp.
