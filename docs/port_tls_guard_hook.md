# TLS Allocator Guard Hook Analysis (sub_8284C290)

## What sub_8284C290 Does

sub_8284C290 is a **1-instruction tail call** to sub_82849860. It has no logic of its own.

sub_82849860 is RAGE's **streaming resource handle opener**. Its key behavior (lines 8654-8667 of gta4_recomp.55.cpp):

```
r10 = [sp+88]       // allocator pointer from caller's stack frame
r11 = [r13+0]       // TLS base
stwx r10, r11, 1676 // TLS[1676] = r10  (active allocator)
stwx r10, r11, 1680 // TLS[1680] = r10  (target allocator)
bctrl [sp+80]       // call the actual resource callback
```

It writes `[sp+88]` directly into BOTH TLS[1676] and TLS[1680], bypassing the push/pop protocol (sub_827D85E0/sub_827D8620). The value comes from a 44-byte struct prepared by the caller and stored on the stack.

## The Hook

**Location**: `LibertyRecomp/kernel/imports.cpp`, line 984

The hook runs the original function, then validates TLS[1676] afterward:
- If non-zero but the vtable pointer at `[TLS[1676]+0]` is outside 0x82000000-0x84000000, it's corrupt
- Clears both TLS[1676] and TLS[1680] to 0, so Phase 3 (fallback allocator at sub_8218BE28) handles subsequent allocations
- The comment says "sub_827DAE40" but this is **misleading** -- sub_827DAE40 does not exist in generated code. The hook is on sub_8284C290.

## Could This Hook Cause the sub_821B3510 Hang?

**Yes, this is a plausible corruption vector.** Here's why:

1. sub_82849860 writes a **stack-sourced** allocator pointer into TLS[1676/1680]
2. If the caller's stack frame at `[sp+88]` contains garbage (e.g. 0xBEBEBEBE from RexGlue stack fill, or a stale pointer from a previous frame), the original function writes it to TLS
3. The guard hook then **clears** TLS[1676] to 0
4. The Phase 3 fallback (sub_8218BE28/sub_8218BE50) routes to SystemHeapAlloc -- but sub_821B3510 is `operator new`, which calls sub_8218BE28 internally
5. **If the fallback path itself triggers operator new recursively**, or if sub_821B3510 doesn't go through the hooked sub_8218BE28 but reads TLS[1676] directly before calling into the allocator vtable, the cleared-to-0 value causes a null dereference loop

**Critical gap**: The guard catches corrupt non-zero values, but if a valid-looking but **wrong** allocator pointer is written (vtable in range but object is freed/reused), IsValidAllocator returns true and the corruption passes through undetected.

## Relationship to sub_827C2420 and sub_82852DD0

- **sub_827C2420** ("activate-streaming") is the top-level streaming activation call during init (INIT_PROBE at phase 3055). It calls sub_8284F310 which manages the streaming manager. Neither calls sub_8284C290 directly.
- **sub_82852DD0** ("OpenAndProcess") is the streaming resource loader. It calls sub_8284F468 (find) and sub_82852D18 (process). Neither calls sub_8284C290 directly.
- sub_8284C290/sub_82849860 would be reached via **indirect vtable dispatch** from the streaming subsystem, not from direct calls in those functions.

## TLS[1676] Allocator Lifecycle

| Phase | Function | Action |
|-|-|
| **Init** | sub_82849860 (called via sub_8284C290 and others) | Direct-writes TLS[1676] and TLS[1680] from caller's struct. Bypasses push/pop. |
| **Push** | sub_827D85E0 | If cur != target, sets TLS[1676] = TLS[1680]. Refcount++ at TLS[1668]. |
| **Pop** | sub_827D8620 | If refcount==0, restores TLS[1676] from TLS[1672] (saved). Zeros TLS[1672]. |
| **Read** | sub_8218BE28 / sub_8218BE50 | Reads TLS[1676] to get the active allocator, calls vtable[method]. |
| **Consume** | sub_821B3510 (operator new) | Calls sub_8218BE28 internally. Hangs if allocator is null/corrupt. |

### Who Corrupts TLS[1676]

1. **sub_82849860 (this function)**: Writes arbitrary stack value. If stack is dirty, writes garbage.
2. **sub_827D85E0 (push)**: Writes TLS[1680] into TLS[1676]. If TLS[1680] is 0 (stubbed GPU init), zeros the allocator. *Guarded by Phase 2 push hook.*
3. **sub_827D8620 (pop)**: Writes TLS[1672] into TLS[1676]. If push was skipped, TLS[1672] is 0. *Guarded by Phase 2 pop hook.*
4. **RexGlue stack fill**: Fresh thread stacks filled with 0xBEBEBEBE. If sub_82849860 reads from an uninitialized stack slot, it writes 0xBEBEBEBE into TLS. *Guarded by Phase 4 (this hook).*

### What's NOT Guarded

- A freed-but-in-range allocator object (passes IsValidAllocator, vtable looks valid, but object is dead)
- A race where Thread A's sub_82849860 writes TLS[1676] while Thread B is mid-allocation using the old value (no mutex on TLS writes, but TLS is per-thread so this shouldn't apply unless r13 is shared)
- sub_821B3510 reading TLS[1676] directly without going through the hooked sub_8218BE28 path

## Files

- Hook: `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/imports.cpp` (lines 973-998)
- Original: `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.55.cpp` (line 14748-14755, tail-calls sub_82849860 at line 8424-8470)
- TLS write site in sub_82849860: lines 8654-8667 of gta4_recomp.55.cpp
