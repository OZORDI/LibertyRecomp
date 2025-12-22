# LibertyRecomp Architecture Refactoring Plan

Based on analysis of UnleashedRecomp's proven architecture.

## Core Insight

**UnleashedRecomp does NOT replace `_xstart`**. They:
1. Call `GuestThread::Start({ entry, 0, 0 })` with the XEX entry point
2. Let the game's CRT (_xstart) run naturally
3. Only hook the Xbox kernel/XAM APIs that _xstart calls

**We have been:** Replacing _xstart with 8000+ lines of inline hacks.

---

## File Size Comparison

| File | LibertyRecomp | UnleashedRecomp |
|------|---------------|-----------------|
| `kernel/imports.cpp` | **347KB** (~8000 lines) | **45KB** (~1800 lines) |
| `main.cpp` | 14KB | 12KB |

---

## Key Differences

### UnleashedRecomp main.cpp (line 364):
```cpp
GuestThread::Start({ entry, 0, 0 });  // Just calls entry point, done
```

### UnleashedRecomp imports.cpp (end of file):
```cpp
// Clean list of GUEST_FUNCTION_HOOK macros
GUEST_FUNCTION_HOOK(__imp__VdInitializeEngines, VdInitializeEngines);
GUEST_FUNCTION_HOOK(__imp__MmAllocatePhysicalMemoryEx, MmAllocatePhysicalMemoryEx);
// ... etc
```

### UnleashedRecomp guest_thread.cpp:
```cpp
uint32_t GuestThread::Start(const GuestThreadParams& params)
{
    GuestThreadContext ctx(cpuNumber);      // Sets up PCR, TLS, TEB, stack
    ctx.ppcContext.r3.u64 = params.value;
    g_memory.FindFunction(params.function)(ctx.ppcContext, g_memory.base);
    return ctx.ppcContext.r3.u32;
}
```

---

## What We Need To Do

### Phase 1: Remove _xstart Replacement

Delete from `kernel/imports.cpp`:
- `PPC_FUNC(_xstart)` - all 500+ lines of it
- `InitializeModernCRT()` function
- `BootGlobals` namespace
- Manual constructor table scanning
- All `[BOOT]` logging code in boot sequence

### Phase 2: Let Game CRT Run Naturally

The game's _xstart (at 0x829A0860) will call:
- `sub_829A7FF8` - Early system init → Hook Xbox-specific APIs it calls
- `sub_829A7960` - Runtime callbacks → Let it run, hook callback registration
- `sub_829A0678` - Privilege check → Hook `XexCheckExecutablePrivilege` to return success
- `sub_82994700` - CRT/TLS init → Let it run, hook TLS APIs
- `sub_829A7EA8` - Init table → Let it run
- `sub_829A7DC8` - Constructors → Let it run
- `sub_8218BEA8` - Game main → Let it run

### Phase 3: Hook Only What's Needed

Instead of replacing functions, hook the Xbox APIs they call:

```cpp
// Privilege check - always pass
uint32_t XexCheckExecutablePrivilege(uint32_t privilege) { return 1; }
GUEST_FUNCTION_HOOK(__imp__XexCheckExecutablePrivilege, XexCheckExecutablePrivilege);

// Video init - host handles
void VdInitializeEngines(...) { /* host GPU init */ }
GUEST_FUNCTION_HOOK(__imp__VdInitializeEngines, VdInitializeEngines);

// TLS - proper implementation
uint32_t KeTlsAlloc() { /* real TLS allocation */ }
GUEST_FUNCTION_HOOK(__imp__KeTlsAlloc, KeTlsAlloc);
```

---

## GTA4 vs Sonic Unleashed Address Differences

Both games use base address 0x82000000, but specific function addresses differ:

| Function | Sonic (SWA) | GTA4 |
|----------|-------------|------|
| Entry point | (unknown) | 0x829A0860 |
| restgprlr_14 | 0x831B0B40 | 0x8298EB30 |
| savegprlr_14 | 0x831B0AF0 | 0x8298EAE0 |

This means we cannot copy UnleashedRecomp's GUEST_FUNCTION_HOOK addresses directly.
We need to find the equivalent GTA4 function addresses for each hook.

---

## Implementation Order

1. **Create backup branch** (done - pushed to main as pre-refactor snapshot)
2. **Delete _xstart replacement** from imports.cpp
3. **Delete BootGlobals, InitializeModernCRT** from imports.cpp
4. **Verify GuestThread::Start** works with natural entry point
5. **Add minimal hooks** for Xbox APIs that fail
6. **Test incrementally** - add hooks as needed based on crashes/failures
7. **Split remaining imports.cpp** into proper modules

---

## Files to Modify

1. `LibertyRecomp/kernel/imports.cpp` - Remove boot hacks, clean up
2. `LibertyRecomp/main.cpp` - Ensure proper GuestThread::Start call
3. `LibertyRecomp/cpu/guest_thread.cpp` - Verify context setup

---

## Expected Result

- `imports.cpp` shrinks from 347KB to ~50KB
- Game's CRT runs naturally
- Only Xbox APIs are hooked, not game functions
- Clean, maintainable codebase matching UnleashedRecomp patterns

---

*Created: 2025-12-21*
*Reference: UnleashedRecomp TECHNICAL_DOCUMENTATION.md*
