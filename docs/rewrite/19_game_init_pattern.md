# 19. The game_init.cpp Pattern (Namespace Replacement)

## Overview

`game_init.cpp` / `game_init.h` is the reference implementation for replacing a
complex Xbox 360 initialization function (sub_82120000) with a structured,
phase-based C++ replacement.  Agent 9 called this "Pattern 4 (Namespace
Replacement)".  This document captures every detail so it can be replicated for
the sub_82242910 state-machine rewrite.

### Source files

| File | Purpose |
|------|---------|
| `LibertyRecomp/kernel/game_init.h` | Constants namespace + function declarations |
| `LibertyRecomp/kernel/game_init.cpp` | Phase implementations + orchestrator |

---

## 1. Architectural Anatomy

### 1.1 Header: Two Namespaces

The header defines **two** namespaces:

```
namespace GameInitGlobals {     // Address constants only
    constexpr uint32_t ...;
}

namespace GameInit {            // Function declarations only
    bool InitCoreEngine(...);
    void InitGameManager(...);
    uint32_t AllocateFromPool(...);
    void InitProfileSystem(...);
    void InitSubsystems(...);
    uint32_t Initialize(...);   // Main entry point
}
```

This separation is critical: `GameInitGlobals` is a pure data namespace with
zero code.  Every Xbox 360 guest address that the module reads or writes is
defined there as a `constexpr uint32_t`, with a comment showing the PPC
instruction that generated it (lis/addi, lwz offset, etc.).

### 1.2 Constants Naming Convention

Each constant is named by its **semantic role**, not its PPC subroutine:

```cpp
constexpr uint32_t INIT_CONTEXT_ADDR   = 0x82126194;  // r3 passed to sub_8218C600
constexpr uint32_t POOL_PTR_ADDR       = 0x8305C1B0;  // memory pool pointer
constexpr uint32_t STRING_TABLE_ADDR   = 0x82129140;  // string table input
constexpr uint32_t CORE_INIT_FLAG      = 0x8312579A;  // stb 1 during core init
constexpr uint32_t GPU_STATE_1         = 0x83084044;  // stw -1
constexpr uint32_t SUBSYS_STATE        = 0x83137654;  // stw 0 at subsys start
```

Every constant has a trailing comment documenting:
- The PPC instruction that produces it (lis r9, -31982 + 22426)
- The operation performed (stb 1, stw 0, stw -1, etc.)
- Which original sub_XXXXXXXX it belongs to (grouped by section headers)

### 1.3 Constant Groups (by original function)

The header organizes constants into labeled groups:

```
// =========================================================================
// sub_82120000 - Game Init Entry Point
// =========================================================================
INIT_CONTEXT_ADDR, POOL_PTR_ADDR, STRING_TABLE_ADDR

// =========================================================================
// sub_8218C600 - Core Engine Initialization
// =========================================================================
CORE_INIT_FLAG, GPU_STATE_1..3, GPU_BUFFER_SIZE, ENGINE_VTABLE_PTR_1..5

// =========================================================================
// sub_82120EE8 - Game Manager Initialization
// =========================================================================
GAME_MANAGER_PTR, GAME_MANAGER_SIZE, WORLD_CONTEXT_PTR, WORLD_CONTEXT_SIZE

// =========================================================================
// sub_82124080 - Profile/Save Subsystem
// =========================================================================
PROFILE_CONTEXT, PROFILE_INIT_FLAG, PROFILE_FLAG_1..7

// =========================================================================
// sub_82120FB8 - Subsystem Initialization
// =========================================================================
SUBSYS_STATE, SUBSYS_FLAG_1, SUBSYS_FLAG_2, SUBSYS_COUNT
```

---

## 2. Implementation (.cpp): Phase Decomposition

### 2.1 The Original Call Tree

The file header documents the **full call tree** being replaced:

```
sub_82120000 (Game Init)
├── sub_8218C600  -> InitCoreEngine()      [Phase 1]
├── sub_82120EE8  -> InitGameManager()     [Phase 2]
├── sub_821250B0  -> AllocateFromPool()    [Phase 3]
├── sub_82318F60  -> [string table lookup] [Phase 3]
├── sub_82124080  -> InitProfileSystem()   [Phase 4]
└── sub_82120FB8  -> InitSubsystems()      [Phase 5]
                     LODHooks::Initialize  [Phase 6]  (new, not in original)
                     PostFXHooks::Init     [Phase 7]  (new, not in original)
```

### 2.2 External PPC Function Declarations

Each original sub-function that the replacement calls is declared at the top:

```cpp
extern "C" void __imp__sub_8218C600(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82120EE8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_821250B0(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82318F60(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82124080(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82120FB8(PPCContext& ctx, uint8_t* base);
```

The `__imp__` prefix accesses the **original recompiled function**, bypassing
any hook.  This is how game_init.cpp calls sub-functions individually while
replacing the parent.

### 2.3 Helper Macros

Standard macros for guest memory access (big-endian byte-swapped):

```cpp
#define PPC_STORE_U8(addr, val)   *(uint8_t*)(base + (addr)) = (uint8_t)(val)
#define PPC_STORE_U16(addr, val)  *(uint16_t*)(base + (addr)) = ByteSwap((uint16_t)(val))
#define PPC_STORE_U32(addr, val)  *(uint32_t*)(base + (addr)) = ByteSwap((uint32_t)(val))
#define PPC_LOAD_U32(addr)        ByteSwap(*(uint32_t*)(base + (addr)))
```

These are defined locally in game_init.cpp (not shared across files).

### 2.4 Phase Function Signatures

Every phase function takes `(PPCContext& ctx, uint8_t* base)` — the standard
PPC context.  Return types vary by need:

| Phase | Signature | Return |
|-------|-----------|--------|
| 1 | `bool InitCoreEngine(PPCContext&, uint8_t*)` | true/false success |
| 2 | `void InitGameManager(PPCContext&, uint8_t*)` | void |
| 3 | `uint32_t AllocateFromPool(PPCContext&, uint8_t*, uint32_t)` | guest pointer |
| 4 | `void InitProfileSystem(PPCContext&, uint8_t*)` | void |
| 5 | `void InitSubsystems(PPCContext&, uint8_t*)` | void |
| Main | `uint32_t Initialize(PPCContext&, uint8_t*)` | 1=success, 0=fail |

### 2.5 Phase Implementation Patterns

Each phase follows a consistent structure:

```cpp
bool InitCoreEngine(PPCContext& ctx, uint8_t* base)
{
    LOG_WARNING("[GameInit] Phase 1: InitCoreEngine starting...");

    // 1. Set up register arguments for the original function
    ctx.r3.u64 = GameInitGlobals::INIT_CONTEXT_ADDR;

    // 2. Call original PPC function via __imp__ prefix
    __imp__sub_8218C600(ctx, base);

    // 3. Check return value from PPC registers
    bool success = (ctx.r3.u32 & 0xFF) != 0;

    // 4. Log result
    LOGF_WARNING("[GameInit] Phase 1: InitCoreEngine {} (r3={})",
                 success ? "SUCCEEDED" : "FAILED", ctx.r3.u32);

    return success;
}
```

Key patterns observed:
- **Delegate to original**: Most phases call `__imp__sub_XXXXXXXX` directly
- **Pre-write state**: Some phases write flags before calling the original
  (e.g., Phase 5 writes SUBSYS_STATE/FLAGS before calling sub_82120FB8)
- **Post-process results**: Phase 3 reads the return value and writes to the
  allocated structure fields
- **Mix PPC + host logic**: Phase 6/7 call pure C++ modules (LODHooks, PostFXHooks)

### 2.6 The Orchestrator (Initialize)

The main entry point chains phases sequentially with error checking:

```cpp
uint32_t Initialize(PPCContext& ctx, uint8_t* base)
{
    // Phase 1: only phase with error checking (returns bool)
    if (!InitCoreEngine(ctx, base)) {
        return 0;  // FATAL
    }

    // Phase 2: fire-and-forget
    InitGameManager(ctx, base);

    // Phase 3: inline orchestration (load pool ptr, allocate, write struct)
    uint32_t poolPtr = PPC_LOAD_U32(GameInitGlobals::POOL_PTR_ADDR);
    uint32_t allocResult = AllocateFromPool(ctx, base, poolPtr);
    PPC_STORE_U32(allocResult + 0, 0);
    PPC_STORE_U32(allocResult + 4, 0);
    // ... string table lookup, write to struct ...

    // Phase 4-5: fire-and-forget
    InitProfileSystem(ctx, base);
    InitSubsystems(ctx, base);

    // Phase 6-7: host-only modules
    LODHooks::Initialize(base);
    PostFXHooks::Init(base);

    return 1;  // Success
}
```

---

## 3. How It Connects to the Hook System

### 3.1 Current Status: NOT Actively Hooking

As of the current codebase, `game_init.cpp` does **NOT** define a
`PPC_FUNC_HOOK(sub_82120000)`.  The `GameInit::Initialize` function exists as a
callable library but is not registered as the override for sub_82120000.

The `game_init.h` header is included by `imports.cpp` (line 39) but
`GameInit::Initialize` is never called from there.

### 3.2 Hook Registration Methods

This project uses three hook registration mechanisms:

| Method | Defined in | Effect |
|--------|-----------|--------|
| `PPC_FUNC_HOOK(sub_XXXXXXXX)` | function.h | `extern "C"` override of weak symbol from codegen |
| `InsertFunction(addr, func)` | memory.cpp | Writes to PPC_LOOKUP_FUNC table at runtime |
| `PPC_FUNC(sub_XXXXXXXX)` | (codegen) | C++-mangled; does NOT override weak C-linkage symbols |

To activate game_init.cpp as a full replacement, you would add:

```cpp
// In game_init.cpp (at file scope, outside namespace):
extern "C" void __imp__sub_82120000(PPCContext& ctx, uint8_t* base);
PPC_FUNC_HOOK(sub_82120000)
{
    ctx.r3.u64 = GameInit::Initialize(ctx, base);
}
```

### 3.3 The `__imp__` Convention

- `sub_XXXXXXXX` = the current (possibly hooked) version
- `__imp__sub_XXXXXXXX` = the original recompiled PPC function (never hooked)

This allows a hook to call the original implementation (trampoline pattern),
or call child functions individually while replacing the parent orchestration.

---

## 4. Related Patterns in the Codebase

### 4.1 memory.cpp: Sign-In State Emulation

`memory.cpp`'s `PopulateFunctionTableAndVtables()` demonstrates the
**direct memory write** pattern for environment emulation:

```cpp
// Write 1 to the sign-in state byte for player 0
PPC_STORE_U8(0x831C501C, 1);
```

This is a simpler variant of game_init's approach: instead of replacing a
function, it pre-writes the memory state that the function would have
produced.

### 4.2 save_hooks.cpp: Wrap-and-Log Pattern

`save_hooks.cpp` uses `PPC_FUNC(sub_829A1C38)` (note: NOT `PPC_FUNC_HOOK`) to
wrap save functions with logging:

```cpp
extern "C" void __imp__sub_829A1C38(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829A1C38)
{
    printf("[SaveHook] sub_829A1C38 #%d\n", ++s_count);
    __imp__sub_829A1C38(ctx, base);  // call original
    printf("[SaveHook] returned r3=0x%08X\n", ctx.r3.u32);
}
```

### 4.3 imports.cpp: GPU Stub Pattern

`imports.cpp` uses `PPC_FUNC_HOOK` for lightweight GPU stubs:

```cpp
PPC_FUNC_HOOK(sub_82A4EDC8) {
    uint32_t write_ptr = PPC_LOAD_U32(ctx.r3.u32 + 56);
    PPC_STORE_U32(ctx.r3.u32 + 60, write_ptr);
}
```

### 4.4 imports.cpp: KernelPhase System

The kernel phase system (Boot -> Init -> Runtime) in `imports.cpp` is relevant
for state machine replacements:

```cpp
enum class KernelPhase { Boot, Init, Runtime };
std::atomic<KernelPhase> g_kernelPhase{KernelPhase::Boot};
```

This pattern could be extended for a front-end state machine that needs to
track its own phase progression.

---

## 5. Coding Conventions Summary

1. **Section headers**: `// ===...===` (77 chars) for major sections,
   `// ---...---` for sub-sections within functions
2. **Log tag**: `[ModuleName]` prefix in all log messages
3. **Phase logging**: "Phase N: PhaseName starting..." / "...completed"
4. **Constants**: ALL_CAPS with semantic names, not address-based names
5. **Comments on constants**: Include PPC instruction derivation
6. **Includes**: `<stdafx.h>` first, then system, then project headers
7. **Namespace**: `GameInit` for functions, `GameInitGlobals` for constants
8. **Error handling**: Only Phase 1 returns bool; others are fire-and-forget
9. **Function signatures**: Always `(PPCContext& ctx, uint8_t* base)` + optional args
10. **TODO markers**: Used for known future work items

---

## 6. Template for sub_82242910 Replacement

Based on the game_init.cpp pattern, here is the structural template for
replacing the front-end state machine.

### 6.1 Header: `frontend_init.h`

```cpp
#pragma once
#include <cstdint>

// =============================================================================
// Front-End State Machine Module
// =============================================================================
// Replaces sub_82242910 (0x82242910) — the front-end initialization and
// scene dispatch state machine.
//
// Original call tree:
//   sub_82242910 (Front-End State Machine)
//   ├── sub_822438B0  -> RunReadinessChecks()      [state 0-2]
//   ├── sub_8223DAA0  -> CheckSubsystemReadiness() [state 3]
//   ├── sub_8223F308  -> CreateScene()              [state 4]
//   ├── sub_822422E0  -> DispatchScene()            [state 5-14]
//   └── sub_8224FA48  -> CheckDialogComplete()      [state 3 gate]
// =============================================================================

namespace FrontEndGlobals {

    // =========================================================================
    // sub_82242910 - State Machine Control
    // =========================================================================
    constexpr uint32_t STATE_COUNTER       = 0xXXXXXXXX;  // current state var
    constexpr uint32_t READINESS_FLAG      = 0x82BF9B70;  // -1=not ready, 0=ready
    // ... (populate from 07_memory_address_map.md)

    // =========================================================================
    // sub_822438B0 - Readiness Checks (States 0-2)
    // =========================================================================
    constexpr uint32_t SIGNIN_CHECK_ADDR   = 0xXXXXXXXX;
    // ...

    // =========================================================================
    // sub_8223F308 - Scene Creation (State 4)
    // =========================================================================
    constexpr uint32_t SCENE_VTABLE_ADDR   = 0xXXXXXXXX;
    // ...

} // namespace FrontEndGlobals

struct PPCContext;

namespace FrontEnd {

    // Phase functions (one per state group)
    bool RunReadinessChecks(PPCContext& ctx, uint8_t* base);
    bool CheckSubsystemReadiness(PPCContext& ctx, uint8_t* base);
    bool CreateScene(PPCContext& ctx, uint8_t* base);
    void DispatchScene(PPCContext& ctx, uint8_t* base, uint32_t state);

    // Main entry point (replaces sub_82242910)
    uint32_t StateMachine(PPCContext& ctx, uint8_t* base);

} // namespace FrontEnd
```

### 6.2 Implementation: `frontend_init.cpp`

```cpp
// =============================================================================
// Front-End State Machine Module
// =============================================================================
// Replaces sub_82242910 — see frontend_init.h for call tree and strategy.
// =============================================================================

#include <cstdint>
#include <cstdio>
#include <stdafx.h>
#include <cpu/ppc_context.h>
#include <os/logger.h>
#include "frontend_init.h"
#include "function.h"

// =============================================================================
// External PPC Function Declarations
// =============================================================================
extern "C" void __imp__sub_822438B0(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_8223DAA0(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_8223F308(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_822422E0(PPCContext& ctx, uint8_t* base);
// ... add more as needed

// =============================================================================
// Helper Macros
// =============================================================================
#define PPC_STORE_U8(addr, val)  *(uint8_t*)(base + (addr)) = (uint8_t)(val)
#define PPC_STORE_U32(addr, val) *(uint32_t*)(base + (addr)) = ByteSwap((uint32_t)(val))
#define PPC_LOAD_U32(addr)       ByteSwap(*(uint32_t*)(base + (addr)))

namespace FrontEnd {

// =============================================================================
// Phase 1: Readiness Checks (replaces states 0-2 of sub_82242910)
// =============================================================================
bool RunReadinessChecks(PPCContext& ctx, uint8_t* base)
{
    LOG_WARNING("[FrontEnd] Phase 1: RunReadinessChecks starting...");

    // Pre-write sign-in state (already done in memory.cpp, but verify)
    // ...

    // Call original readiness check or write flags directly
    // ...

    LOG_WARNING("[FrontEnd] Phase 1: RunReadinessChecks completed");
    return true;
}

// =============================================================================
// Phase 2: Subsystem Readiness (replaces state 3 gate)
// =============================================================================
bool CheckSubsystemReadiness(PPCContext& ctx, uint8_t* base)
{
    LOG_WARNING("[FrontEnd] Phase 2: CheckSubsystemReadiness starting...");

    // Write readiness flag to bypass XAM dialog wait
    PPC_STORE_U32(FrontEndGlobals::READINESS_FLAG, 0xFFFFFFFF);  // -1 = no dialog

    LOG_WARNING("[FrontEnd] Phase 2: CheckSubsystemReadiness completed");
    return true;
}

// ... (Phase 3: CreateScene, Phase 4: DispatchScene) ...

// =============================================================================
// Main Entry Point: StateMachine (replaces sub_82242910)
// =============================================================================
uint32_t StateMachine(PPCContext& ctx, uint8_t* base)
{
    LOG_WARNING("[FrontEnd] ============================================");
    LOG_WARNING("[FrontEnd] Front-End State Machine Starting");
    LOG_WARNING("[FrontEnd] ============================================");

    if (!RunReadinessChecks(ctx, base)) {
        LOG_WARNING("[FrontEnd] FATAL: ReadinessChecks failed!");
        return 0;
    }

    if (!CheckSubsystemReadiness(ctx, base)) {
        LOG_WARNING("[FrontEnd] FATAL: SubsystemReadiness failed!");
        return 0;
    }

    if (!CreateScene(ctx, base)) {
        LOG_WARNING("[FrontEnd] FATAL: CreateScene failed!");
        return 0;
    }

    // States 5-14 are dispatched by the scene system
    // The state machine hands off to the game loop here

    LOG_WARNING("[FrontEnd] ============================================");
    LOG_WARNING("[FrontEnd] Front-End State Machine Complete");
    LOG_WARNING("[FrontEnd] ============================================");
    return 1;
}

} // namespace FrontEnd

// =============================================================================
// Hook Registration (activates the replacement)
// =============================================================================
extern "C" void __imp__sub_82242910(PPCContext& ctx, uint8_t* base);
PPC_FUNC_HOOK(sub_82242910)
{
    ctx.r3.u64 = FrontEnd::StateMachine(ctx, base);
}
```

### 6.3 Key Differences from game_init.cpp

The sub_82242910 replacement differs from game_init.cpp in several ways:

1. **Loop-based state machine**: Unlike game_init's linear sequence,
   sub_82242910 is a while-loop with a state variable.  The replacement
   should collapse the loop into sequential phases rather than re-
   implementing the loop.

2. **Active hook registration**: game_init.cpp currently does NOT register
   a `PPC_FUNC_HOOK`.  The sub_82242910 replacement SHOULD register one
   from the start (see section 6.2 bottom).

3. **More error-checking phases**: The front-end has multiple readiness
   gates that can fail.  Each should return bool.

4. **May need to call into existing hooks**: The front-end interacts with
   save_hooks.cpp, xam.cpp, and user_profile.cpp.  Those existing hooks
   should be left in place; the front-end replacement just needs to write
   the memory state they produce.

---

## 7. Checklist for Implementing a Replacement Module

- [ ] Create `modulename.h` with `ModuleNameGlobals` constants namespace
- [ ] Document every address with PPC instruction derivation
- [ ] Group constants by original sub_XXXXXXXX function
- [ ] Create `modulename.cpp` with `ModuleName` functions namespace
- [ ] Document full call tree in file header comment
- [ ] Declare all `__imp__sub_XXXXXXXX` functions at top
- [ ] Define PPC_STORE/PPC_LOAD macros locally
- [ ] Implement each phase as a separate function
- [ ] Log entry/exit of each phase with `[ModuleName]` tag
- [ ] Write orchestrator function that chains phases
- [ ] Add `PPC_FUNC_HOOK(sub_XXXXXXXX)` at file scope to activate
- [ ] Add `#include "modulename.h"` to imports.cpp
- [ ] Verify all constants with Python arithmetic (never do hex math by hand)
