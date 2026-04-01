# Platform Mode Gate Analysis

## Why 0x831C2458 (scene list pointer) Is NULL

The scene list at `0x831C2458` is populated during D3D device initialization, which is fully stubbed. The scene *object* is created by `sub_827ADB48` and stored at `0x82FF5368`, but never **registered** into the render device scene list array at `0x831C2458`. This is a GPU registration problem, not a state machine problem.

Separately, `imports.cpp` reads `0x82BF3A88` and labels it "scene object pointer" -- this is **wrong**. Per `04_sub_8223F308_scene_creation.md`, `0x82BF3A88` is scene data slot 0 (first parsed value from scene file). The actual scene object pointer is at `0x82BF3A78`.

## Current Hook State (imports.cpp)

### Hooks That Are Working Correctly

| Hook | Lines | Action | Status |
|-|-|-|-|
| sub_82242910 (scene creation SM) | 2346-2396 | Forces platformMode=3 at 0x82BF9844 for states 0-4; intercepts fast path 0->4 forcing 0->1 | Correct |
| sub_822422E0 (state 5) | 2122-2141 | Resets 0x82BF9834 from 2->0 after state 5 completes, preventing error 34 in scene creation state 4 | Correct |
| sub_822438B0 (state 6 inner) | 2091-2106 | Diagnostic only -- logs state transitions, scene creation sub-state, scene obj | Correct |
| sub_822440F8 (state 4 inner) | 2065-2084 | BYPASS: returns 2 (no-save success), skipping Xbox storage device selection | Correct |
| sub_82240B08 (content readiness) | 2175-2179 | Forces g_sceneReady=1, g_contentReady=1, returns 1 (device ready) | Correct |
| sub_8223DB20 (sign-in guard) | 2155-2157 | Stub: returns 0 (no sign-in change) | Correct |
| sub_82240B78 (storage guard) | 2164-2166 | Stub: returns 0 (no storage change) | Correct |
| sub_8224FFC8 (XAM dialog) | 2188-2195 | Auto-accepts dialog type 8, cancels others | Correct |
| sub_822417B0 (content size) | 2234-2252 | Clamps negative storage delta to 0 | Correct |

### Missing Hooks (Not Hooked)

| Function | Role | Impact |
|-|-|-|
| sub_8223DAA0 | XAM readiness check (state 0 of sub_82242910) | Returns 1 too early in recomp, causing fast path 0->4. Mitigated by the 0->4 intercept, but a direct stub returning 0 would be cleaner |
| sub_8223F308 | Scene creation dispatch (state 4 target) | Runs as recompiled code -- no hook. This is the function that parses scene file data into SceneStruct at 0x82BF3A78 |
| sub_8284AAE0 | Scene load (state 6 of sub_82242910) | Runs as recompiled code |
| sub_8284AB10 / sub_8284B490 | Scene object creation (state 8) | Runs as recompiled code |

## State Machine Flow Assessment

The outer state machine (sub_82142230, states 0-6) advances correctly:
- States 0-2: Sign-in, storage, save -- all pass via hooks
- State 3: Player slot content fields populated by sub_821406C8 hook
- State 4: Bypassed entirely (sub_822440F8 returns 2)
- State 5: sub_822422E0 runs, resets stale 0x82BF9834 value
- State 6: sub_822438B0 runs, which calls sub_82242910

Within sub_82242910 (15-state scene creation):
- State 0: Fast path intercepted (0->4 forced to 0->1)
- States 1-3: Run normally, write error code 6 to 0x82A9546C
- State 4: platformMode forced to 3, sub_8223F308 called (scene file parse)
- States 5-14: Run as recompiled code

## Root Cause Summary

The platformMode gate is handled correctly -- forcing 0x82BF9844=3 ensures state 4 takes the scene creation branch. The NULL scene list at 0x831C2458 is NOT caused by platformMode failure. It is caused by the scene object never being registered into the GPU device scene list array, because D3D device init (which performs that registration) is fully stubbed.

## Fix Path

1. After scene creation completes (sub_82242910 reaches state 14), copy the scene object from `0x82FF5368` into `0x831C2458`
2. Fix the diagnostic: change `0x82BF3A88` references to `0x82BF3A78` in hook logging
3. Optionally hook sub_8223DAA0 to return 0 directly, eliminating the need for the fast-path intercept
