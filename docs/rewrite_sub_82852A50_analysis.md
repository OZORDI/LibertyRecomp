# Analysis of sub_82852A50 — "GetResourcePtr"

## Location
- **Implementation**: `gta4_recomp.55.cpp` lines 30531–30702
- **Address range**: 0x82852A50 – 0x82852B74
- **Diagnostic probe**: `INIT_PROBE(sub_82852A50, 30556, "82852D18 get-resource-ptr")` in imports.cpp:1772

## Signature

```
ptr sub_82852A50(context_obj *r3, uint32_t key_r4)
```

Returns a ref-counted resource wrapper pointer, or 0 (null) on failure.

## Caller: sub_82852D18

At line 30982, sub_82852D18 calls sub_82852A50 and checks `if (r3 == 0) → return` (early exit, lines 30986–30993). If non-null, it dereferences the returned wrapper, calls a vtable dispatch to process the resource, then sets success=1.

## Globals Read

| Address | What | How |
|-|-|-|
| 0x831E55EC | Pointer to GPU/shader device context | `lis r11,-31970; lwz r11,21996(r11)` |
| [ptr+40] bit 17 | Thread-safety flag (0x00020000 mask) | `lwz r11,40(r11); rlwinm r29,r11,15,31,31` |

When bit 17 is set (r29=1), the function acquires/releases a TLS-based recursive lock around the main body.

## Call Tree

```
sub_82852A50(context_obj, key)
  [if r29=1] sub_828470E0 — TLS lock acquire
  sub_82852300(context_obj, key, &status, &extra) — search for resource
    sub_8285AD08 — ring buffer read (calls sub_8285A8B0 = GPU flush, ALREADY STUBBED)
    sub_828D0608 — hash table iterator init
    sub_82851C40 — hash table iterator advance (linked-list walk)
    indirect call [obj+4+12] — filter/match callback per hash entry
  [if status == -1] → goto return_null
  sub_82851DF0(context_obj, &status) — hash table key lookup
    Walks linked list comparing key at [node+0]; returns node+4 or 0
  [if lookup == 0] → goto return_null
  indirect call [obj+16+12] — primary acquire/create resource
  [if resource != 0]
    indirect call [vtable+4](resource, extra) — bind/configure
    sub_8284FC98(resource) — allocate 20-byte ref-counted wrapper
      sub_821B3510(20) — allocate
      sub_8284D220 x3 — init descriptors with vtable ptrs
      indirect call [resource_vtable+8] — finalize
    indirect call [vtable+0](resource, 1) — addref
    → return wrapper
  [if resource == 0]
    indirect call [obj+32+12] — fallback create
    → return fallback result
  return_null: → return 0
  [if r29=1] sub_82847120 — TLS lock release
```

## Synchronization Analysis

### No blocking synchronization primitives

sub_82852A50 does NOT call any of these directly or transitively:
- `KeWaitForSingleObject` / `NtWaitForSingleObjectEx`
- `RtlEnterCriticalSection`
- `sub_82849790` / `sub_828497D8` / `sub_82849820` (event wait functions)
- `sub_82849918` (yield/spin)

### TLS Lock (sub_828470E0 / sub_82847120)

These are lightweight, non-blocking TLS operations:

**sub_828470E0 (acquire)**: Reads `TLS[r13] + 1676` and `+1680`. If equal (same thread owns it), increments recursion count at `+1668`. If different, stores new owner and saves old.

**sub_82847120 (release)**: Decrements recursion count at `+1668`. When count reaches 0, restores saved value at `+1672` into `+1676` and zeroes `+1672`.

This is a **reentrant per-thread lock** with zero OS interaction. No contention possible since it only tracks the current thread.

### sub_8285AD08 (ring buffer read, called from sub_82852300)

This function calls `sub_8285A8B0` (GPU buffer flush) which is **already stubbed in imports.cpp:1310** — the hook body is empty. So the GPU flush path is a no-op. The rest of sub_8285AD08 does pure memory arithmetic (buffer pointer management).

## Does sub_82852A50 call sub_8285B088?

**No.** sub_82852A50 does not call sub_8285B088 at all. However, sub_82852B78 ("ShaderBind") calls sub_82852A50 first, then calls sub_8285B088 afterwards (line 30754). The comment in imports.cpp:1278 confirms this relationship.

## Failure Modes (returns 0)

1. **sub_8285AD08 returns < 4**: Ring buffer doesn't have enough data → sub_82852300 writes status=-1 → return 0
2. **Hash iteration finds no match**: sub_82852300 exhausts all entries, writes status=1 (no match) → flow falls to sub_82851DF0
3. **sub_82851DF0 returns 0**: Key not found in hash table → return 0
4. **Indirect vtable call returns 0**: Primary acquire fails → falls to fallback path at obj+32

## Data Structures

The context object passed in r3 contains:
- **+0**: hash table bucket array pointer
- **+4**: hash bucket count (u16)
- **+6**: insertion counter (u16)
- **+16**: primary vtable with method at +12 (acquire)
- **+32**: secondary vtable with method at +12 (fallback acquire)

The ref-counted wrapper returned (20 bytes, from sub_8284FC98):
- **+0**: data ptr 0
- **+4**: data ptr 1
- **+8**: counter (u16)
- **+10**: counter (u16)
- **+12**: descriptor ptr 1
- **+16**: descriptor ptr 2

## Summary

sub_82852A50 is a **synchronous, non-blocking resource lookup function**. It searches a hash table for a resource by key, acquires/creates it via vtable dispatch, wraps it in a ref-counted container, and returns the wrapper. The only lock is a TLS-based reentrant counter (not a real mutex). The only potential GPU interaction (sub_8285A8B0) is already stubbed. There are no semaphore waits, critical sections, event waits, or spin loops anywhere in this function's call tree.
