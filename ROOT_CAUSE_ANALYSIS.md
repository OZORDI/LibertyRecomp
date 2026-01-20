# LibertyRecomp Root Cause Analysis

## Executive Summary

This document identifies the root causes of cascading failures in the LibertyRecomp project and the order in which hacks/workarounds should be unraveled. The failures form a dependency chain where upstream issues cause downstream symptoms that were individually patched rather than fixing the root cause.

---

## Cascade Chain Overview

```
┌─────────────────────────────────────────────────────────────────┐
│  LAYER 1: KERNEL/SYNC PRIMITIVES (Root Causes)                  │
│  - Fail-open waits during Boot/Init                             │
│  - Xbox kernel object emulation gaps                            │
└───────────────────────────┬─────────────────────────────────────┘
                            │
┌───────────────────────────▼─────────────────────────────────────┐
│  LAYER 2: HEAP/MEMORY CORRUPTION                                │
│  - Heap critical section corruption (sub_829A64C0)              │
│  - Memory allocation failures (sub_8218BE28)                    │
└───────────────────────────┬─────────────────────────────────────┘
                            │
┌───────────────────────────▼─────────────────────────────────────┐
│  LAYER 3: DATA STRUCTURE INITIALIZATION                         │
│  - Shader effect params not initialized (sub_8285A538)          │
│  - Static data arrays zeroed (dword_82AA5DB0[1798])             │
└───────────────────────────┬─────────────────────────────────────┘
                            │
┌───────────────────────────▼─────────────────────────────────────┐
│  LAYER 4: RUNTIME FAILURES (Symptoms)                           │
│  - Dictionary lookup failures (sub_82859E98)                    │
│  - GPU sync loops (sub_829CFED0)                                │
│  - Texture tiling garbage (sub_829E4970)                        │
└─────────────────────────────────────────────────────────────────┘
```

---

## Root Cause #1: Fail-Open Wait Policy During Boot/Init

### Location
`imports.cpp` lines 36-68, 2732-2748, 4830-4861

### Symptom
Infinite waits converted to immediate success during boot, causing race conditions.

### Root Cause
The `ShouldFailOpenWait()` mechanism returns STATUS_SUCCESS for infinite waits during Boot/Init phases. This was added to prevent hangs but causes:
- Worker threads to skip initialization
- Semaphores to be in wrong states
- Race conditions between threads

### Upstream Fix Required
Instead of fail-open waits, implement proper Xbox kernel object emulation:
1. Implement `KeInitializeEvent`, `KeInitializeSemaphore` fully
2. Track all dispatcher objects properly
3. Let waits block properly once kernel objects are correct

### Current Hack
```cpp
if (timeout == INFINITE && ShouldFailOpenWait()) {
    return STATUS_SUCCESS;  // HACK: Skip wait
}
```

---

## Root Cause #2: Heap Critical Section Corruption

### Location
`imports.cpp` lines 6480-6552 (sub_829A64C0 wrapper)

### Symptom
Heap structure at offset 1408 (critical section pointer) becomes NULL.

### Root Cause Analysis
1. `sub_829A7F20` initializes heap at `dword_831483DC`
2. `RtlImageXexHeaderField` stub returns 0, which is correct
3. Heap structure created via `sub_829A5F10`
4. **UNKNOWN CODE** zeroes critical section at offset 1408

### Likely Upstream Cause
The heap corruption is likely caused by:
- A buffer overflow from texture operations
- Memory protection disabled at 0x831F0000+
- Some initialization code writing to wrong address

### Current Hack
```cpp
// Cache valid state and restore when corrupted
if (critSecPtr == 0 && s_cachedCritSecPtr != 0) {
    PPC_STORE_U32(heapArg + 1408, s_cachedCritSecPtr);
}
```

### Fix Order
1. First fix memory protection properly
2. Add memory watchpoint at heap+1408 to find corruptor
3. Remove corruption recovery hack

---

## Root Cause #3: Static Data Array Not Initialized (dword_82AA5DB0)

### Location
`default (1).xex.c` line 66417

### Symptom
`sub_82859E98` finds NULL at v5+20 for addresses 0x82AA5FED, 0x82AA601D, etc.

### Root Cause Analysis
```c
// In decompiled code:
int dword_82AA5DB0[1798] = { 0, 0, 0, ... };  // All zeros!

// Addresses that fail:
// 0x82AA5FED = 0x82AA5DB0 + 0x23D (within array)
// 0x82AA601D = 0x82AA5DB0 + 0x26D
// etc.
```

The array `dword_82AA5DB0` is in the game's .data section (0x82AA****) and is declared as zeros in decompilation. On Xbox 360, this would contain shader effect parameter metadata populated by:
1. XEX loader (from .data section)
2. Game initialization code (runtime)

### Why It's Zero
The static recompiler copies code but the .data section initialization may be incomplete. The game's constructor chain should populate this array via:
- `sub_821940F0(dword_82AA5DB0)` - clears and links entries
- `sub_821948F0(dword_82AA5DB0)` - shader effect init
- `sub_8285A538` - individual entry init (sets offset +20)

### Current Hack
```cpp
// Validate v5+20 before access
if (ptrAtV5_20 < 0x80000000 || ptrAtV5_20 > 0xE0000000) {
    ctx.r3.u32 = 0;  // Return 0 instead of crashing
    return;
}
```

### Fix Required
Trace why `sub_8285A538` doesn't set offset +20:
1. Is it called at all? (Check logs for sub_8285BDC8 -> sub_8285B680 -> sub_8285A538)
2. Does `sub_8218BE28` (memory allocation) return NULL?
3. Is there a race condition with shader loading?

---

## Root Cause #4: GPU Sync Loop (sub_829CFED0)

### Location
`imports.cpp` lines 6647-6683

### Symptom
Infinite loop waiting for GPU completion that never comes.

### Root Cause
The function checks:
1. GPU timestamp at r13+256+88 vs saved value
2. GPU completion counter at *(v3+10896) vs saved value

Since we're not running real Xenos GPU, these counters never advance.

### Current Hack
```cpp
if (s_consecutiveCalls > 50) {
    ctx.r3.u32 = 0;  // Force "done" status
}
```

### Proper Fix
Implement GPU timestamp/counter updates in the rendering path:
- When VdSwap or equivalent is called, advance GPU counters
- When command buffer is submitted, update completion counter

---

## Root Cause #5: GPU Command Buffer Flush (sub_829D8568)

### Location
`imports.cpp` lines 6620-6631 (hook)

### Symptom
Function spins forever waiting for read pointer to catch up to write pointer.

### Root Cause
```c
// Original logic:
v12 = ((read_ptr - write_ptr) >> 2) - 1;
// If read_ptr < write_ptr, v12 is negative -> infinite loop
```

Since there's no real GPU consuming commands, read_ptr never advances.

### Current Hack
Stub the function entirely, return write pointer.

### Proper Fix
Implement ring buffer consumption simulation:
- After each frame, set read_ptr = write_ptr
- Or implement proper command buffer draining in Present

---

## Root Cause #6: Texture Tiling Garbage Parameters (sub_829E4970)

### Location
`imports.cpp` lines 6554-6608

### Symptom
Function receives garbage dimensions (>16384) and crashes in memmove.

### Root Cause
The texture structure passed to this function has garbage data because:
1. Memory at 0x831F0000+ was incorrectly protected (now fixed)
2. Texture initialization code didn't run properly
3. Heap corruption affected texture allocator

### Current Hack
```cpp
if (width > 16384 || height > 16384 || depth > 16384) {
    return;  // Skip call
}
```

### Fix Required
1. Trace texture creation to find where garbage data comes from
2. Ensure texture memory is properly initialized
3. Remove dimension check hack

---

## Unravel Order (Priority)

### Phase 1: Kernel Primitives (Remove Fail-Open)
1. **Fix KeInitializeSemaphore/KeInitializeEvent** - ensure proper initialization
2. **Fix NtWaitForSingleObjectEx** - proper blocking without fail-open
3. **Remove ShouldFailOpenWait()** - let waits work correctly

### Phase 2: Heap/Memory Integrity
4. **Find heap corruptor** - add watchpoint at heap+1408
5. **Fix sub_829A64C0** - remove corruption recovery hack
6. **Verify memory allocations** - ensure sub_8218BE28 doesn't fail

### Phase 3: Data Structure Initialization
7. **Trace dword_82AA5DB0 initialization** - verify sub_821940F0, sub_821948F0 run
8. **Fix sub_8285A538** - ensure offset +20 is set
9. **Remove sub_82859E98 validation hack**

### Phase 4: GPU Emulation
10. **Implement GPU counter advancement** - in VdSwap/Present
11. **Remove sub_829CFED0 force-completion hack**
12. **Implement ring buffer draining** - fix sub_829D8568 properly

### Phase 5: Texture System
13. **Trace texture allocation chain** - find garbage source
14. **Remove sub_829E4970 dimension check**

---

## LLDB Debug Commands

```bash
# Break on heap corruption
watchpoint set expression -w write -- *(uint32_t*)(heap_addr + 1408)

# Break on sub_82859E98 to trace dictionary failures
breakpoint set -n "sub_82859E98"

# Break on memory allocation failures
breakpoint set -n "sub_8218BE28" -c "ctx.r3.u32 == 0"

# Trace shader effect initialization
breakpoint set -n "sub_8285A538"
breakpoint set -n "sub_8285B680"
breakpoint set -n "sub_8285BDC8"
```

---

## Key Addresses Reference

| Address | Purpose |
|---------|---------|
| 0x831483DC | Game heap pointer (dword_831483DC) |
| 0x82AA5DB0 | Shader effect parameter array (1798 entries) |
| 0x82AA5FED-0x82AA619D | Entries in dword_82AA5DB0 with NULL at +20 |
| heap+1408 | Critical section pointer (corrupted) |
| heap+384 | Free list head |
| heap+388 | Free list next |
| 0x82A97FB4 | Callback critical section |
| 0x82A97FD0 | Callback linked list head |

---

## Summary

The cascade of failures originates from **Layer 1** (kernel/sync primitives) and propagates down. The fail-open wait policy causes race conditions that lead to incomplete initialization, which manifests as heap corruption and NULL pointers in data structures.

**The correct fix order is bottom-up**: fix kernel primitives first, then remove downstream hacks one by one, verifying each layer works before proceeding to the next.
