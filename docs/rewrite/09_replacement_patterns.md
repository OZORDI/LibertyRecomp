# 09 - Replacement Patterns in LibertyRecomp

## Overview

This document catalogs every pattern used in the LibertyRecomp codebase for replacing
Xbox 360 functionality, classified by replacement strategy. The goal is to identify
which pattern is appropriate for replacing sub_82242910 (the 15-state scene creation
sub-machine).

---

## Pattern 1: COMPLETE REPLACEMENT (no `__imp__` call)

These hooks entirely replace the original PPC function with native C++ logic.
The original code never executes.

### Examples

| Function | File | What it replaces | Strategy |
|----------|------|-----------------|----------|
| `sub_8219F728` | save_hooks.cpp | Active player slot counter | Returns hardcoded `1` (user 0 active). Original reads Xbox OS player slot objects that never exist in recomp. |
| `sub_8218C2C0` | save_hooks.cpp | Loading complete check | Returns hardcoded `1`. Original depends on Xbox VBlank hardware (sub_82878998). |
| `sub_827DE648` | save_hooks.cpp | Streaming completion barrier | Returns immediately (no-op). Original spin-waits on 0x830F5820 which is pre-cleared by the sub_82192E00 hook. |
| `sub_822440F8` | imports.cpp | STATE 4 inner state machine (7 states) | Returns hardcoded `2` (no-save success). Entire purpose is Xbox 360 save device/controller selection which cannot work in recomp. Sets playerIdx side effect. |
| `sub_82169B00` | imports.cpp | Audio thread sync | Returns `0`. Xbox worker model not needed on PC. |
| `sub_82169400` | imports.cpp | Audio worker thread | Returns `0`. SDL handles audio. |
| `sub_8285D018` | imports.cpp | GPU ring buffer submit + fence wait | Returns `0` (fence already complete). No Xenos GPU hardware. |
| `sub_8285C648` | imports.cpp | GPU fence wait | Returns `1` (signaled). |
| `sub_8285CF98` | imports.cpp | GPU fence create+wait | Returns `1` (success). |
| `sub_8285A8B0` | imports.cpp | GPU shader bytecode flush | No-op. Skips Xenos ring buffer submission. |
| `sub_82A4EDC8` | imports.cpp | GPU command buffer drain | Manually advances write/read pointers. No GPU hardware. |
| `sub_82A486F0` | imports.cpp | GPU atomic sync | No-op (r3 already set). |
| `sub_82A49C38` | imports.cpp | GPU sync bypass | Clears device field, skips spin loop. |
| `sub_82A46098` | imports.cpp | Frame swap stub | No-op return. Would SIGBUS without GPU context. |
| `sub_82871180` | imports.cpp | GPU render state submission | Returns `0`. Pure Xbox 360 D3D code that accesses unmapped addresses. |
| `sub_829A0678` | imports.cpp | HDCP bypass | Returns `0`. PC has no HDCP. |
| `sub_829FBE38` | memory.cpp (InsertFunction) | Unknown callback | Returns `0` (success stub). |
| `sub_830F2CB8` | memory.cpp (InsertFunction) | BSS-range pointer | Returns `0` (success stub). |

### Key Characteristics
- Used when the ENTIRE function depends on Xbox hardware that does not exist
- Must reproduce all **side effects** that downstream code depends on (memory writes, return values)
- Typical return values: 0 (success), 1 (done/ready), or specific state-machine codes

---

## Pattern 2: WRAP + PATCH (call `__imp__`, fix before/after)

The original PPC function runs, but the hook writes memory or adjusts state
before or after the call to fix Xbox-specific issues.

### Examples

| Function | File | What it patches | Strategy |
|----------|------|----------------|----------|
| `sub_821200D0` | save_hooks.cpp | Post-init (profiles/saves) | **Pre-patch**: clears LOADING_STEP_ADDR (0x83137BC9) to 0 so Loop 1 exits. Forces Runtime phase. Calls `__imp__` for all remaining logic. |
| `sub_82192E00` | save_hooks.cpp | Streaming init | **Post-patch**: calls `__imp__`, then zeroes 0x830F5820 (streaming-pending flag). Recomp VFS is synchronous. |
| `sub_82242910` | imports.cpp | Scene creation sub-machine (15 states) | **Pre-patch**: forces platformMode at 0x82BF9844 to 3 (base game). **Post-patch**: intercepts fast-path 0->4 and resets state to 1. |
| `sub_822422E0` | imports.cpp | STATE 5 game start | **Post-patch**: resets 0x82BF9834 from 2->0 to prevent scene creation error 34. |
| `sub_8284CFD8` | imports.cpp | Streaming ring-buffer worker init | **Post-patch**: seeds semaphore handles into worker structs via rex::system::XSemaphore after original init runs. |
| `sub_827DF248` | imports.cpp | pgStreamer::Init | **Pre-patch**: sets dword_830F589C = 1 to force synchronous streaming mode (no worker threads). Then calls `__imp__`. |
| `sub_827D85E0` | imports.cpp | RAGE allocator PUSH | **Conditional guard**: if target allocator is invalid but current is valid, skip the push to prevent zeroing TLS[1676]. |
| `sub_827D8620` | imports.cpp | RAGE allocator POP | **Conditional guard**: if refcount=0 and saved=0 and current!=0, skip the pop. |
| `sub_8218BE28` | imports.cpp | RAGE malloc | **Conditional fallback**: if TLS[1676] is invalid, route to RexGlue SystemHeapAlloc instead of calling `__imp__`. |
| `sub_82A487B8` | imports.cpp | VBlank callback | **Pre-patch**: allocates a zero-filled stub for device[+10900] if null, preventing null deref. Then calls `__imp__`. |
| `sub_829A2540` | imports.cpp | NtSetEvent wrapper | **Guard**: skips `__imp__` call when handle is 0 or 0xCDCDCDCD. |
| `sub_827FC7F0` | imports.cpp | RAGE AES decrypt | **Complete replacement on macOS**: uses CommonCrypto AES-256-ECB. Falls through to `__imp__` on non-Apple. |
| `sub_82140000` | imports.cpp | RAGE init gate | **Idempotency guard**: calls `__imp__` only once, returns 1 on subsequent calls. |
| `sub_821406C8` | imports.cpp | Player accessor (state 3) | **Post-patch on first call**: populates player slot content-readiness fields (offsets 56, 68, 72, 4) that XAM notifications would have set. |

### Key Characteristics
- Used when the function contains BOTH Xbox-specific and game-logic code
- The fix is surgical: patch a specific memory location, guard a specific condition
- Side effects are preserved by letting original code run

---

## Pattern 3: DIAGNOSTIC WRAP (call `__imp__`, log only)

The original function runs unmodified. The hook only adds logging/tracing.
No behavior change.

### Examples

| Function | File | Purpose |
|----------|------|---------|
| `sub_829A1C38` | save_hooks.cpp | Content creation wrapper - log args and return value |
| `sub_829A1CA0` | save_hooks.cpp | Content close wrapper - log handle |
| `sub_829A1CB8` | save_hooks.cpp | Content enumeration - log args |
| `sub_8297A930` | save_hooks.cpp | Save manager - log invocation count |
| `sub_82122CA0` | save_hooks.cpp | Save system init - log 3 save slot contexts |
| `sub_82142230` | imports.cpp | Front-end state machine - log entry/exit |
| `sub_822414E8` | imports.cpp | STATE 0 sign-in check - log return value |
| `sub_8223DDA8` | imports.cpp | STATE 1 storage device - log return value |
| `sub_8223DEE8` | imports.cpp | STATE 2 save/load - log return value |
| `sub_822438B0` | imports.cpp | STATE 6 inner machine - log state transitions |
| `sub_82A50890` | imports.cpp | GPU CreateDevice - log args |
| `sub_821B3CE8` | imports.cpp | RAGE engine init - log success/failure |
| Many others | imports.cpp | Various lifecycle tracing hooks |

### Key Characteristics
- Used during development/debugging to understand state machine flow
- Some have been left in as permanent diagnostics
- Zero behavior impact (important: these can be safely removed later)

---

## Pattern 4: NAMESPACE REPLACEMENT (game_init.cpp)

The most structured replacement pattern. A C++ namespace (`GameInit`) provides
a complete replacement for `sub_82120000` by orchestrating calls to the original
sub-functions while adding custom logic between them.

### Architecture
```
GameInit::Initialize() replaces sub_82120000
  |-- GameInit::InitCoreEngine()    -> calls __imp__sub_8218C600
  |-- GameInit::InitGameManager()   -> calls __imp__sub_82120EE8
  |-- GameInit::AllocateFromPool()  -> calls __imp__sub_821250B0
  |-- (inline struct setup)         -> manual PPC_STORE_U32 writes
  |-- GameInit::InitProfileSystem() -> calls __imp__sub_82124080
  |-- GameInit::InitSubsystems()    -> calls __imp__sub_82120FB8
  |-- LODHooks::Initialize()        -> custom hook registration
  |-- PostFXHooks::Init()           -> custom hook registration
```

### Key Characteristics
- Header file (`game_init.h`) documents ALL known memory addresses
- Each phase wraps a single `__imp__` call with pre/post setup
- Custom initialization (LOD, PostFX) is added AFTER original phases
- Sub-functions are called individually, not the parent function
- Memory layout constants are centralized in `GameInitGlobals` namespace

---

## Pattern 5: KERNEL OBJECT REPLACEMENT (xam.cpp, user_profile.cpp)

Complete replacement of Xbox 360 kernel services with native C++ implementations.

### Examples
- `XamContentCreateEx` - Maps content names to host filesystem paths
- `XamContentCreateEnumerator` - Returns C++ iterators over content registry
- `XamEnumerate` - Drives C++ iterator-based enumeration
- `XamNotifyCreateListener` - Queues startup notifications directly to listeners
- `XamInputGetState` - Translates SDL keyboard/mouse/gamepad to Xbox controller state
- `UserProfile` - Singleton with hardcoded "Niko" profile and settings storage
- `XamUserGetSigninState` - Returns profile singleton's signin state

### Key Characteristics
- No `__imp__` calls at all - pure native C++ implementations
- Registered via `GUEST_FUNCTION_HOOK` macro in the registration block
- Must match Xbox ABI exactly (parameter order, struct layout, endianness)

---

## Pattern 6: InsertFunction (runtime function table patching)

Used for functions that are called via indirect dispatch (PPC_CALL_INDIRECT_FUNC)
rather than direct `bl` instructions.

### Examples
```cpp
InsertFunction(0x821966D0, sub_821966D0_hook);  // Worker thread gate
InsertFunction(0x829FBE38, [](PPCContext& ctx, uint8_t* base) { ctx.r3.u32 = 0; });
InsertFunction(0x830F2CB8, [](PPCContext& ctx, uint8_t* base) { ctx.r3.u32 = 0; });
```

### Key Characteristics
- Patches the PPC_LOOKUP_FUNC table (used by indirect calls and thread trampolines)
- PatchFuncMapping only patches PPCFuncMappings[] which is NOT consulted by indirect calls
- Used when the target is called via vtable dispatch or computed address

---

## Analysis: Design Pattern for Replacing sub_82242910

### Current Implementation (Pattern 2: Wrap + Patch)

The existing hook at `imports.cpp:1816` uses Pattern 2:
1. **Pre-patch**: Forces platformMode at 0x82BF9844 to 3 (base game)
2. **Call `__imp__`**: Lets the original 15-state machine run
3. **Post-patch**: Intercepts fast-path 0->4 and resets state to 1

### Should we call `__imp__sub_82242910` at all?

**Recommendation: NO - use Pattern 4 (Namespace Replacement)**

Rationale:
- sub_82242910 is a 15-state machine where ~5 states depend on Xbox hardware
  (XAM dialogs, storage device enumeration, controller sign-in)
- The current wrap+patch approach requires increasingly fragile state corrections
  (platformMode fix, fast-path interception, stale value reset)
- Each new bug found requires another surgical patch to the pre/post hooks
- The game_init.cpp pattern proves that calling sub-functions individually with
  orchestration logic between them produces more stable results

### Recommended Architecture

```
SceneCreation::Create() replaces sub_82242910
  |
  |-- State 0-3: SKIP entirely (platform mode / XAM prerequisites)
  |     - Write platformMode = 3 to 0x82BF9844
  |     - Write error code = 6 to 0x82A9546C (what states 1-3 produce)
  |     - Advance state counter at 0x82BF9848 to 4
  |
  |-- State 4: CALL sub_8223F308 (scene gate check)
  |     - This is the core "can we create a scene?" check
  |     - Pure game logic, no Xbox dependencies
  |     - Advances to state 5 on success
  |
  |-- States 5-10: CALL __imp__sub_82242910 with state pre-set
  |     - These are world/scene loading states
  |     - Mostly pure game logic (streaming, resource loading)
  |     - May need individual sub-function hooks for streaming barriers
  |
  |-- State 11: CHECK result and handle
  |     - Validates platformMode is in {3,4} (we set it to 3)
  |     - Pure logic check
  |
  |-- States 12-14: CALL __imp__ (cleanup/finalization)
  |     - Write scene object pointer to 0x82BF3A88
  |     - Pure finalization logic
```

### Sub-functions to still call

| Address | Name/Purpose | Call? | Reason |
|---------|-------------|-------|--------|
| `sub_8223F308` | Scene gate check | YES | Pure game logic, validates prerequisites for scene creation |
| `sub_8223DAA0` | Device enumeration status | NO (stub return 0) | Xbox storage device check, always returns wrong value in recomp |
| `sub_8223DB20` | Profile/device validation | MAYBE | Both return paths are valid; stubbing to return 0 is safe |
| `sub_822428E0` | Scene resource allocation | YES | Core game logic, allocates scene structures |
| `sub_82243090` | World loading dispatch | YES | Triggers resource streaming |

### State variable writes the replacement MUST produce

| Address | Value | Purpose |
|---------|-------|---------|
| `0x82BF9848` | 0->14 (final) | Scene creation state counter - other functions read this |
| `0x82BF9844` | 3 | Platform mode - must be 3 (base game) or 4 (DLC) |
| `0x82A9546C` | 6 | Error code field - states 1-3 write this, state 11 validates it |
| `0x82BF3A88` | scene_ptr | Scene object pointer - must be non-null on success |
| `0x82BF9834` | 0 | Legacy state variable - must be reset to prevent error 34 |

### Minimal side effects checklist

1. platformMode = 3 at 0x82BF9844 (BEFORE any state runs)
2. error code = 6 at 0x82A9546C (what natural states 1-3 would write)
3. state counter = final value at 0x82BF9848 (14 on success)
4. scene object pointer at 0x82BF3A88 (from sub_822428E0)
5. legacy state = 0 at 0x82BF9834 (prevent error 34 in state 4)
6. Return value: 0 = working, 1 = error, 2 = done (matches sub_822438B0 expectations)

### Implementation Strategy

Follow the `game_init.cpp` pattern:
1. Create `scene_creation.h` with address constants (like `GameInitGlobals`)
2. Create `scene_creation.cpp` with `SceneCreation::Create()` function
3. Use `PPC_FUNC_HOOK(sub_82242910)` to intercept and call the replacement
4. Call individual sub-functions via `__imp__` where they contain useful game logic
5. Skip Xbox-dependent sub-functions entirely, writing their expected outputs directly
6. Log state transitions for debugging (Pattern 3 diagnostics)

---

## Summary of All Hook Mechanisms

| Mechanism | Count | Usage |
|-----------|-------|-------|
| `PPC_FUNC_HOOK` (wrap+patch) | ~35 | Most common - surgical fixes to original PPC code |
| `PPC_FUNC` (complete replacement) | ~10 | Used when function is entirely Xbox-dependent |
| `GUEST_FUNCTION_HOOK` (kernel export) | ~60 | Xbox kernel API replacement (video, input, network, XAM) |
| `GUEST_FUNCTION_WEAK_STUB` | ~15 | Linker stubs for unimplemented kernel APIs |
| `InsertFunction` (indirect dispatch) | 3 | Runtime patching for vtable/indirect calls |
| Namespace replacement (game_init) | 1 | Full decomposition + orchestration of init sequence |
