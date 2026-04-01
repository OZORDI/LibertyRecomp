# Analysis: sub_828470E0 and sub_82847120

## Location
- File: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.55.cpp`
- sub_828470E0: line 2436
- sub_82847120: line 2475
- Calling site (sub_82852D18): lines 31031-31063

## Global State Flag

**Address computation**: `(-31970 << 16) + 21996 = 0x831E55EC`

The code performs:
```
ptr = *(uint32_t*)0x831E55EC;   // global singleton pointer
flags = *(uint32_t*)(ptr + 40); // flags word at offset +40
bit17 = (flags >> 17) & 1;     // rlwinm r31,r11,15,31,31
```

When `bit17 == 1`: both functions are called (context save before, restore after).
When `bit17 == 0`: both calls are skipped entirely.

This pattern appears **10 times** in gta4_recomp.55.cpp, always wrapping vtable dispatches and resource operations. Likely a "multi-threaded rendering context" flag on RAGE's global render device singleton.

## sub_828470E0 — TLS Context Save ("Enter")

Operates on TLS block accessed via `*(r13)` with four fields:

| Offset|Role|
|-|-|
| +1668 | Depth counter (recursive nesting) |
| +1672 | Saved previous value |
| +1676 | Current active value |
| +1680 | Reference/target value |

**Pseudocode:**
```c
void sub_828470E0() {
    uint32_t* tls = *(uint32_t**)(r13);
    if (tls[1676/4] == tls[1680/4]) {
        tls[1668/4] += 1;        // nested re-entry: bump depth
    } else {
        tls[1672/4] = tls[1676/4]; // save current
        tls[1676/4] = tls[1680/4]; // adopt reference
    }
}
```

## sub_82847120 — TLS Context Restore ("Leave")

**Pseudocode:**
```c
void sub_82847120() {
    uint32_t* tls = *(uint32_t**)(r13);
    if (tls[1668/4] != 0) {
        tls[1668/4] -= 1;        // nested: decrement depth
    } else {
        uint32_t saved = tls[1672/4];
        tls[1672/4] = 0;          // clear saved
        tls[1676/4] = saved;      // restore previous
    }
}
```

## Calling Pattern in sub_82852D18

```
bit17 = (*(*(0x831E55EC) + 40) >> 17) & 1;
if (bit17) sub_828470E0();        // save TLS context
sub_8284FA58(r27);                // resource release
sub_821B3560(r27);                // additional cleanup
if (bit17) sub_82847120();        // restore TLS context
return r30;                       // (set to 1 earlier)
```

## Blocking Risk: NONE

Both functions are **purely computational TLS manipulation**:
- No spinlocks, mutexes, or wait primitives
- No system calls or kernel transitions
- No atomic operations or memory barriers
- No loops that could spin
- Maximum path length: 4 memory operations + 1 branch

These cannot deadlock or block. They are O(1) with deterministic execution.

## Potential Crash Risks

1. **Null global pointer**: If `*(0x831E55EC) == NULL`, the dereference at `*(ptr + 40)` will segfault. This would happen if the render device singleton is not yet initialized when these draw functions run.

2. **Uninitialized TLS block**: If `*(r13)` returns a TLS block where offsets 1668-1680 contain garbage, the depth counter and save/restore logic will produce undefined behavior (though not blocking).

3. **Mismatched enter/leave**: If an exception or early return skips sub_82847120 after sub_828470E0 was called, the TLS context will remain in the "adopted" state permanently for that thread, causing the wrong rendering context to be active.

## Conclusion

These are a **recursive TLS context save/restore pair** — a lightweight mechanism for switching per-thread rendering state around draw calls. They are gated by bit 17 of the RAGE render device flags, which likely indicates whether multi-threaded rendering is active. They are safe, non-blocking, and do not need hooks or workarounds. The only risk is the global pointer at `0x831E55EC` being NULL during early startup before the render device is created.
