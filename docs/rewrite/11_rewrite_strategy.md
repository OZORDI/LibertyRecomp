# Scene Creation State Machine Rewrite Strategy

## 1. Architecture Overview

The front-end boot chain is a three-level nested state machine:

```
sub_82142230 (front-end SM, ~8 states)
  state 6 -> sub_822438B0 (outer SM, 8 states: 0-7)
               state 1 -> init: sets platformMode=2, clears flags, resets scene state to 0
               state 2 -> sub_82242910 (scene creation SM, 15 states: 0-14)
               state 3 -> sub_82240F80(0) — resource readiness polling
               state 4 -> auto-advances to state 5
               state 5 -> sub_82240F80(1) — second resource poll
               state 6 -> ready-signal gate (0x82BF9B70 timeout + scene finalization)
               state 7 -> sub_8223CC68 + sub_82242608 — teardown/completion
```

## 2. Current Hook Approach: Patch-On-Patch Problem

The existing hooks in `imports.cpp` use a "wrap + fixup" strategy:

| Hook | Strategy | Problem |
|------|----------|---------|
| sub_82242910 | Wrap: pre-set platformMode to 3, intercept 0->4 fast path | Fragile: depends on timing of sub_8223DAA0 return value |
| sub_822422E0 | Wrap: reset 0x82BF9834 after state 5 completes | Defense-in-depth, but band-aid for a root cause in state 4 |
| sub_822438B0 | Wrap: logging only, no fixups | Passthrough with diagnostics |
| sub_822440F8 | Full replace: return 2 (bypass save device selection) | Clean, works well |

The fundamental problem: the original code was designed for Xbox 360 async timing where:
- `sub_8223DAA0` returns 0 for several frames (device enumeration delay)
- Platform mode transitions through 2 -> 3 -> 4 with real hardware callbacks
- XAM dialogs gate progress via 0x82BF9B70

In the recomp, RexGlue reports devices as immediately ready, sign-in is emulated, and there are no XAM dialogs. This causes:
- Fast path 0->4 (skipping states 1-3 setup)
- Stale platformMode values triggering error 34
- Missing byte flags (0x82BF981F etc.) causing error 24 at state 11

Each fixup patches one symptom but creates assumptions that break when the next state encounters a different timing issue.

## 3. Recommended Rewrite Strategy

### 3.1 Functions to FULLY REPLACE

**sub_82242910 (scene creation SM)** -- FULL REPLACEMENT

Rationale: This is the most problematic function. 10 of its 15 states contain Xbox-specific checks (`sub_8223DAA0`, `sub_826CBA70`, `sub_82240B08`, `sub_82240B78`, platformMode switches). The current wrap+fixup approach requires ever-growing pre/post patches that still miss edge cases.

**sub_822440F8 (save device selection)** -- Already fully replaced, keep as-is.

### 3.2 Functions to WRAP (keep original, add pre/post logic)

**sub_822438B0 (outer SM)** -- WRAP with targeted fixups.

Rationale: This function's 8 states are mostly dispatch logic. States 0-2 are init/dispatch, state 3-5 are resource polling (sub_82240F80), state 6 is the ready-signal gate, state 7 is teardown. The problematic parts are:
- State 1: writes platformMode = 2 (correct for init)
- State 2: calls sub_82242910 (replaced, so this just works)
- State 3: calls sub_82240F80(0) which may need its own hook
- State 6: ready-signal timeout loop on 0x82BF9B70

If sub_82242910 is fully replaced to always succeed quickly, most of sub_822438B0's complexity becomes harmless. The wrap hook only needs to handle the ready-signal gate (state 6).

**sub_822422E0 (state 5 game start)** -- WRAP, keep current reset of 0x82BF9834.

This function is simpler (level selection + scene load dispatch). The current hook works. With sub_82242910 replaced, the stale-value problem it defends against is eliminated, but the hook is still good defense-in-depth.

### 3.3 Functions to leave UNHOOKED

- sub_8223E028 (state machine exit) -- keep diagnostic hook but let original run
- sub_82240F80 (resource readiness) -- let original run; if sub_82242910 succeeds, this will too
- sub_82240AB0 (called from state 3) -- no Xbox dependencies

## 4. sub_82242910 Replacement Design

### 4.1 State Analysis: What Each State Actually Does

```
State 0:  Gate — sub_8223DAA0() check. If 0: goto 1. If nonzero: jump to 4 + sub_8284B4B0.
State 1:  Gate — sub_826CBA70() + sub_8223DAA0() + sub_8223F9F0(0,0). Transition: 1->3 or 1->2.
State 2:  sub_8223CB60() + sub_826CBA70() + sub_8223DAA0() + sub_8223F9F0(1,0). Writes 6 to 0x82A9546C. Transition: 2->3.
State 3:  Gate — sub_826CBA70() + sub_8223DAA0() + sub_82240AB0(). Transition: 3->4.
State 4:  Gate — sub_8223DB20(). platformMode switch: {3,4}->sub_8223F308. Flag gate on byte@0x82BF981E.
          sub_82240B08() check. Second platformMode switch for error paths. Transition: 4->5 or 4->9.
State 5:  Gate — sub_8223DB20(). Flag gate on 0x82BF981E. sub_8223F9F0(2,0). Transition: 5->6 or 5->7.
State 6:  Gate — sub_8223DB20() + sub_826CBA70(). SCENE CREATION: sub_8284AAE0(...). Transition: 6->8 or error 7.
State 7:  Gate — sub_8223DB20(). sub_8223CB60() + sub_8223F9F0(3,0). Transition: 7->6 (recycle) or 7->write(6).
State 8:  SCENE LOADING: sub_8284AB10, sub_8284AB70, sub_8223DB20, sub_8284B490, sub_8284B430, sub_82240B08.
          Transition: 8->9 or 8->0 (via sub_82240B08 result).
State 9:  Gate — sub_8223DB20() + sub_82240B78(). sub_8223D2F0(). RESOURCE LOAD: sub_8284ABA0(...,75,...).
          Transition: 9->10.
State 10: RESOURCE POLL: sub_8284ABD0 + sub_8284ABF8 + sub_8223DB20 + sub_82240B78 + sub_8284B490 + sub_8284B430.
          Transition: 10->11 (when complete).
State 11: Gate — sub_8223DB20() + sub_82240B78() + sub_8223D400(). platformMode check: {3,4}->12, else error 24.
State 12: SAVE DATA: sub_822417B0(r3=0,r4=1,...). Transition: 12->14 or 12->done(ret=2).
State 13: Gate — sub_8223DB20() + sub_82240B78() + sub_8223F9F0(4,...). Clears byte@0x82A9546E. ret=0.
State 14: SAVE DATA continued: sub_822417B0(r3=0,r4=0,...). Complex conditional end with sub_8223F790.
```

### 4.2 Essential Sub-Function Calls (must be preserved)

These are the actual work functions that create the game world. Everything else is gating/validation:

| Function | Purpose | Called in State |
|----------|---------|----------------|
| sub_8223CB60 | Content readiness check | 2, 7 |
| sub_8223F308 | Scene parameter setup (called with platformMode 3 or 4) | 4 |
| sub_8284AAE0 | **Scene creation** (the main scene instantiation call) | 6 |
| sub_8284AB10 | Scene load initiation | 8 |
| sub_8284AB70 | Scene load step 2 | 8 |
| sub_8284B490 | Scene load polling (returns 1 when done) | 8, 10 |
| sub_8284B430 | Scene load status check (returns 0 or 5 = ok) | 8, 10 |
| sub_8284ABA0 | Resource loading start (75-item batch) | 9 |
| sub_8284ABD0 | Resource load polling | 10 |
| sub_8284ABF8 | Resource load step | 10 |
| sub_8223D2F0 | Pre-resource-load setup | 9 |
| sub_8223D400 | Post-resource-load check | 11 |
| sub_822417B0 | Save data operations | 12, 14 |
| sub_8223F790 | Final scene commit | 14 |
| sub_8223CAD8 | Cleanup/teardown | 11 (error), 14 (end) |

### 4.3 Gate Functions (can be bypassed or stubbed)

| Function | Purpose | Can bypass? |
|----------|---------|-------------|
| sub_8223DAA0 | Device enumeration check (Xbox storage) | Yes -- always return 1 (ready) |
| sub_8223DB20 | Sign-in / profile validity check | Yes -- always return 0 (ok) |
| sub_826CBA70 | Controller disconnect check | Yes -- always return 0 (ok) |
| sub_82240B08 | Platform-specific readiness flag | Context-dependent -- see state 4, 8 |
| sub_82240B78 | Secondary platform readiness | Context-dependent -- see state 9-13 |
| sub_8223F9F0 | XAM dialog flow (sign-in, content selection) | Yes -- set output byte to 1 (complete) |

### 4.4 Required Side Effects for Downstream Code

The replacement must produce these observable effects:

1. **0x82BF9848** (scene creation state) -- must reach final value (0 = done/reset, or advance past 14)
2. **0x82A9546C** (error code word) -- must be 6 (success) or at least not an error value
3. **0x82BF9844** (platformMode) -- must be 3 (base game) or 4 (DLC) at completion
4. **0x82BF981E** (flag byte) -- set to 1 if platformMode is 3 and the device flag is set
5. **0x82BF3A88** (scene object pointer) -- must be non-null after state 6
6. **0x82BF9834** (sub_822422E0 done flag) -- must be 0 when scene creation starts
7. **0x82BF9B68** (scene data) -- written by sub_8284B430 path
8. **Error code at 0x82A9546C** -- sub_822438B0 checks for value 33 specifically (error 33 = fatal)

### 4.5 sub_822438B0 Interaction

sub_822438B0 state 2 calls sub_82242910 and checks its return value:
- `r3 == 2` (error): checks error code for 33 (fatal) vs other (goto state 7)
- `r3 == 0` (done): zeros 16 bytes at 0x82BF3A40, advances to state 3
- `r3 == 1` (working): stays in state 2

The replacement should return `1` while working, `0` when scene creation is complete.

### 4.6 Ready-Signal Loop (0x82BF9B70)

This address is used by sub_822438B0 state 6 as a timeout gate:
- `sub_8224FA38` resets it to -1 (not ready)
- `sub_82254FE0` writes 1 (ready -- from XAM dialog completion)
- `sub_8214C8C8` increments it toward 4
- `sub_8224FA48` reads it: returns 0 if -1, returns 1 if >= 0

In the recomp, no XAM dialogs exist, so `sub_82254FE0` is never called naturally. The ready-signal stays at -1, meaning `sub_8224FA48` always returns 0. This is actually correct for state 3 of sub_82142230 (allows it to advance). But for sub_822438B0 state 6, which uses a separate readiness gate via `r30+4` (byte flag) and a 3000ms timeout, the ready-signal is not the gating factor -- the byte flag at `r30+4` and the timer at `r30+0` control state 6.

**Recommendation**: Leave the ready-signal hooks as diagnostic-only. The ready-signal is not part of the scene creation SM. It gates the outer front-end SM state 3, which works correctly already.

## 5. Proposed Replacement: Simplified State Machine

### 5.1 Design Principles

1. **Keep async polling pattern**: Return `1` (working) each frame, advance internal state, return `0` (done) at completion. This matches the caller's expectations in sub_822438B0.
2. **Call essential sub-functions directly**: No Xbox gates. Just call the scene/resource creation functions in sequence.
3. **Unconditionally set platformMode to 3**: Base game mode. DLC mode (4) can be added later with an episode check.
4. **Reproduce all required side effects**: Write all flag bytes and state values that downstream code reads.

### 5.2 Pseudocode

```cpp
// Simplified scene creation -- replaces the 15-state Xbox state machine
// with a linear sequence that calls essential sub-functions directly.

// State variable addresses (from original):
//   STATE     = r26 + (-26552) = 0x82BF9848  (scene creation internal state)
//   PLATMODE  = 0x82BF9844  (platform mode: 3=base, 4=DLC)
//   ERROR     = 0x82A9546C  (error code word)
//   FLAG_E    = 0x82BF981E  (device flag byte)
//   FLAG_F    = 0x82BF981F  (secondary flag byte -- 0x82BF981E + 1)
//   SCENE_OBJ = 0x82BF3A88  (scene object pointer, set by sub_8284AAE0)
//   SCENE_PTR = 0x82BF9998  (r27 + (-26164) = scene data ptr)

static int s_scenePhase = 0;  // our simplified phase counter

PPC_FUNC_HOOK(sub_82242910) {
    // r26 base = 0x82C00000 (lis r26, -32064)
    // r30 base = 0x82BF0000 (lis r30, -32065)
    // r31 = r30 + 11344 = 0x82BF2C50  (some global struct)
    // r28 = r30 + 14944 = 0x82BF3A60
    // r27 = 0x82C00000 (same as r26)
    // r29 = r27 + (-26168) = 0x82BF9998

    uint32_t localR31 = 0x82BF2C50;  // "this" pointer for scene functions
    uint32_t localR30_15760 = PPC_LOAD_U32(0x82BF0000 + 15760);  // r30+15760 = 0x82BF3D90

    switch (s_scenePhase) {
    case 0: {
        // --- Phase 0: Initialize ---
        // Set platformMode to 3 (base game)
        PPC_STORE_U32(0x82BF9844, 3);
        // Clear error code
        PPC_STORE_U32(0x82A9546C, 6);  // 6 = success value (written by original state 2)
        // Clear flags
        PPC_STORE_U8(0x82BF981E, 0);
        // Clear scene data pointer
        PPC_STORE_U32(0x82BF9998, 0);  // r27 + (-26164)

        // Call sub_8223F308(r3=1, r4=r30+14968) -- scene parameter setup
        // This is what state 4 does when platformMode is 3 or 4
        ctx.r3.s64 = 1;
        ctx.r4.u64 = 0x82BF3A98;  // r30 + 14968
        sub_8223F308(ctx, base);
        // Store scene data ptr
        uint32_t sceneData = PPC_LOAD_U32(0x82BF3A98 + 4);
        PPC_STORE_U32(0x82BF9998, sceneData);

        s_scenePhase = 1;
        ctx.r3.s64 = 1;  // working
        return;
    }

    case 1: {
        // --- Phase 1: Create scene ---
        // sub_8284AAE0(r3=localR31, r4=localR30_15760, r5=1, r6=sceneDataPtr, r7=1, r8=deviceFlag)
        ctx.r3.u64 = localR31;
        ctx.r4.u64 = localR30_15760;
        ctx.r5.s64 = 1;
        ctx.r6.u64 = PPC_LOAD_U32(0x82BF9998);
        ctx.r7.s64 = 1;
        ctx.r8.u64 = PPC_LOAD_U8(0x82A9546E);  // device flag byte
        sub_8284AAE0(ctx, base);
        if ((ctx.r3.u32 & 0xFF) == 0) {
            // Scene creation failed -- error 7
            PPC_STORE_U32(0x82A9546C, 7);
            s_scenePhase = 0;
            ctx.r3.s64 = 2;  // error
            return;
        }
        s_scenePhase = 2;
        ctx.r3.s64 = 1;  // working
        return;
    }

    case 2: {
        // --- Phase 2: Initiate scene loading ---
        // sub_8284AB10(r3=localR31, r4=localR30_15760)
        ctx.r3.u64 = localR31;
        ctx.r4.u64 = localR30_15760;
        sub_8284AB10(ctx, base);
        if ((ctx.r3.u32 & 0xFF) == 0) {
            // wait -- not ready yet
            ctx.r3.s64 = 1;
            return;
        }
        // sub_8284AB70(r3=localR31, r4=localR30_15760)
        ctx.r3.u64 = localR31;
        ctx.r4.u64 = localR30_15760;
        sub_8284AB70(ctx, base);

        s_scenePhase = 3;
        ctx.r3.s64 = 1;  // working
        return;
    }

    case 3: {
        // --- Phase 3: Poll scene load completion ---
        // sub_8284B490(r3=localR31, r4=localR30_15760)
        ctx.r3.u64 = localR31;
        ctx.r4.u64 = localR30_15760;
        sub_8284B490(ctx, base);
        if (ctx.r3.s32 != 1) {
            // Not done yet -- check for errors
            ctx.r3.u64 = localR31;
            ctx.r4.u64 = localR30_15760;
            sub_8284B430(ctx, base);
            if (ctx.r3.s32 != 0 && ctx.r3.s32 != 5) {
                // Error in scene loading
                s_scenePhase = 0;
                ctx.r3.s64 = 2;  // error
                return;
            }
            ctx.r3.s64 = 1;  // still working
            return;
        }
        // Scene load done
        s_scenePhase = 4;
        ctx.r3.s64 = 1;
        return;
    }

    case 4: {
        // --- Phase 4: Start resource loading ---
        sub_8223D2F0(ctx, base);  // pre-resource-load setup

        // sub_8284ABA0(r3=localR31, r4=localR30_15760, r5=1, r6=loadCallback, r7=75)
        ctx.r3.u64 = localR31;
        ctx.r4.u64 = localR30_15760;
        ctx.r5.s64 = 1;
        ctx.r6.u64 = 0x82BF3D98;  // r30 + 15768 (resource load callback struct)
        ctx.r7.s64 = 75;
        sub_8284ABA0(ctx, base);
        if ((ctx.r3.u32 & 0xFF) == 0) {
            PPC_STORE_U32(0x82A9546C, 14);  // error 14
            s_scenePhase = 0;
            ctx.r3.s64 = 2;
            return;
        }

        s_scenePhase = 5;
        ctx.r3.s64 = 1;
        return;
    }

    case 5: {
        // --- Phase 5: Poll resource loading ---
        ctx.r3.u64 = localR31;
        ctx.r4.u64 = localR30_15760;
        ctx.r5.u64 = 0x82BF3D94;  // r30 + 15764 (progress output)
        sub_8284ABD0(ctx, base);
        if ((ctx.r3.u32 & 0xFF) == 0) {
            ctx.r3.s64 = 1;  // still loading
            return;
        }
        // Loading complete -- verify
        ctx.r3.u64 = localR31;
        ctx.r4.u64 = localR30_15760;
        sub_8284ABF8(ctx, base);

        // Check scene load status
        ctx.r3.u64 = localR31;
        ctx.r4.u64 = localR30_15760;
        sub_8284B490(ctx, base);
        if (ctx.r3.s32 == 1) {
            // Error 15 -- scene load failed after resources
            PPC_STORE_U32(0x82A9546C, 15);
            s_scenePhase = 0;
            ctx.r3.s64 = 2;
            return;
        }

        s_scenePhase = 6;
        ctx.r3.s64 = 1;
        return;
    }

    case 6: {
        // --- Phase 6: Post-load validation ---
        sub_8223D400(ctx, base);  // post-resource-load check
        if ((ctx.r3.u32 & 0xFF) == 0) {
            ctx.r3.s64 = 1;  // not ready yet
            return;
        }

        // Write success: error code = 6 (set by original state 2 path)
        PPC_STORE_U32(0x82A9546C, 6);

        s_scenePhase = 7;
        ctx.r3.s64 = 1;
        return;
    }

    case 7: {
        // --- Phase 7: Save data + finalization ---
        // sub_822417B0 -- save data operation (pass 1: r4=1)
        uint32_t r29addr = 0x82BF9998;
        ctx.r3.s64 = 0;
        ctx.r4.s64 = 1;
        ctx.r5.u64 = 0x82BF3A60;   // r28
        ctx.r6.u64 = PPC_LOAD_U32(0x82BF9998);
        ctx.r7.u64 = r29addr;
        // stack locals for output
        // ... (needs stack frame setup, may need to call __imp__ for this)
        sub_822417B0(ctx, base);
        if (ctx.r3.s32 == 2) {
            // Done -- success path
            s_scenePhase = 0;
            ctx.r3.s64 = 2;  // return done+success to caller
            return;
        }

        s_scenePhase = 8;
        ctx.r3.s64 = 1;
        return;
    }

    case 8: {
        // --- Phase 8: Save data pass 2 + final commit ---
        ctx.r3.s64 = 0;
        ctx.r4.s64 = 0;
        // ... (same as above with r4=0)
        sub_822417B0(ctx, base);
        if (ctx.r3.s32 == 2) {
            s_scenePhase = 0;
            ctx.r3.s64 = 2;
            return;
        }
        if (ctx.r3.s32 != 0) {
            ctx.r3.s64 = 1;  // still working
            return;
        }

        // Final commit
        // Check conditions for sub_8223F790 call
        uint32_t platMode = PPC_LOAD_U32(0x82BF9844);
        if (platMode == 3) {
            uint8_t flag = PPC_LOAD_U8(0x82BF981F);
            if (flag != 0) {
                sub_8223F790(ctx, base);
            }
        }
        sub_8223CAD8(ctx, base);  // cleanup

        s_scenePhase = 0;
        ctx.r3.s64 = 0;  // done
        return;
    }

    default:
        s_scenePhase = 0;
        ctx.r3.s64 = 0;
        return;
    }
}
```

### 5.3 Important Notes on the Pseudocode

1. **Stack frame**: The original function uses a 160-byte stack frame with locals at offsets 80, 84, 88. The replacement needs to either allocate its own stack frame or use static storage for the output parameters of sub_822417B0 and sub_8223F9F0.

2. **Register context**: Each sub-function call corrupts the PPC register context. The replacement must reload `localR31`, `localR30_15760`, etc. from globals before each call rather than relying on register values.

3. **Phase 7-8 (save data)**: sub_822417B0 takes many parameters including stack-allocated output buffers (r8=sp+84, r9=sp+88). The replacement must provide these. Consider calling `__imp__sub_82242910` for just these states if the parameter passing is too complex.

4. **Error propagation**: The replacement uses the same error code format (stored to 0x82A9546C) as the original. sub_822438B0 state 2 checks for error code 33 specifically to decide between fatal error and retry.

## 6. Alternative Approach: Hybrid Replacement

Instead of a full replacement, a simpler approach:

```cpp
PPC_FUNC_HOOK(sub_82242910) {
    uint32_t state = PPC_LOAD_U32(0x82BF9848);

    // PRE-CALL: Ensure all gate functions will pass
    PPC_STORE_U32(0x82BF9844, 3);      // platformMode = base game
    PPC_STORE_U8(0x82BF981E, 0);        // clear device flag (prevents error 34 path)

    // Let original run
    __imp__sub_82242910(ctx, base);

    uint32_t newState = PPC_LOAD_U32(0x82BF9848);

    // POST-CALL: Fix any bad transitions
    if (state == 0 && newState == 4) {
        // Fast path -- force normal path
        PPC_STORE_U32(0x82BF9848, 1);
    }

    // Ensure platformMode stays valid for all states that check it
    uint32_t pm = PPC_LOAD_U32(0x82BF9844);
    if (pm != 3 && pm != 4) {
        PPC_STORE_U32(0x82BF9844, 3);
    }
}
```

This is essentially the current approach with one additional fix (clearing 0x82BF981E). It is simpler but may continue to need patches as new edge cases surface.

## 7. Recommendation

**Phase 1 (immediate)**: Enhance the existing hybrid hook to also handle sub_82240B08 and sub_82240B78 gate functions. These return platform readiness flags that can block states 4, 8, 9, 10, 11, 13. Hook them to return 0 (ok) unconditionally.

**Phase 2 (if Phase 1 still has issues)**: Full replacement of sub_82242910 as described in section 5. This eliminates all timing-dependent behavior but requires careful parameter passing for the scene/resource loading functions.

**Do NOT replace sub_822438B0**: Its 8 states are mostly dispatch. With sub_82242910 working correctly, sub_822438B0 will naturally advance through states 2->3->4->5->6->7. The only remaining concern is state 6's ready-signal timeout, which should resolve naturally once the scene is created.

## 8. Key Addresses Reference

| Address | Name | Type | Purpose |
|---------|------|------|---------|
| 0x82BF9848 | STATE | u32 | Scene creation SM state (0-14) |
| 0x82BF9838 | OUTER_STATE | u32 | sub_822438B0 state (0-7) |
| 0x82BF9844 | PLATMODE | u32 | Platform mode (2=init, 3=base, 4=DLC) |
| 0x82BF9834 | DONE_FLAG | u32 | sub_822422E0 completion (0=ready, 2=done) |
| 0x82A9546C | ERROR | u32 | Error code (6=ok, 7/8/9/14/15/16/24/33/34=errors) |
| 0x82BF981E | DEV_FLAG | u8 | Device enumeration flag |
| 0x82BF981F | DEV_FLAG2 | u8 | Secondary device flag |
| 0x82BF3A88 | SCENE_OBJ | u32 | Scene object pointer |
| 0x82BF9998 | SCENE_DATA | u32 | Scene data pointer (r27-26164) |
| 0x82BF9B70 | READY_SIG | u32 | XAM dialog ready signal (-1=not ready) |
| 0x82A9546E | BYTE_FLAG | u8 | Device flag byte (passed to sub_8284AAE0 as r8) |
| 0x82BF2C50 | THIS_PTR | u32 | Scene manager "this" pointer |
| 0x82BF3D90 | LOAD_CTX | u32 | Scene load context (r30+15760) |
| 0x82BF3A98 | PARAM_BUF | struct | Scene parameter buffer (r30+14968) |
| 0x82BF3D94 | PROGRESS | u32 | Resource load progress output |
| 0x82BF3D98 | RES_CB | struct | Resource load callback struct (r30+15768) |
| 0x82BF3A60 | R28_BASE | struct | Scene data base struct (r30+14944) |
