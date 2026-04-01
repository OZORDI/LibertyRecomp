# Deep Analysis: sub_82852A50 (Resource Get-Pointer)

## Location
- File: `gta4_recomp.55.cpp` lines 30531–30703
- Hook: `INIT_PROBE(sub_82852A50, 30556, "82852D18 get-resource-ptr")` in `imports.cpp:1772`

## Pseudocode

```
void* sub_82852A50(ResourceMgr* this, uint32_t key) {
    bool lock = bit17_of( [[0x831E55EC]+40] );
    if (lock) sub_828470E0();           // TLS interrupt disable

    uint32_t slot, idx;
    sub_82852300(this, key, &slot, &idx); // stream-read + hash iterate

    if (slot == -1) goto fail;

    Entry* entry = sub_82851DF0(this, &slot); // hash table lookup
    if (!entry) goto fail;

    void* result = entry->vtable_at_16.method3(entry+16, key); // INDIRECT #1
    if (!result) {
        result = entry->vtable_at_32.method3(entry+32, key, idx); // INDIRECT (alt path)
        if (lock) sub_82847120();       // TLS interrupt restore
        return result;
    }

    result->vtable[1](result, idx);     // INDIRECT #2
    void* handle = sub_8284FC98(result); // alloc+init handle wrapper
    result->vtable[0](result, 1);       // INDIRECT #3 (AddRef/Release)

    if (lock) sub_82847120();           // TLS interrupt restore
    return handle;

fail:
    if (lock) sub_82847120();
    return NULL;
}
```

## Direct Sub-calls (static)

| Function | Role | Blocking? |
|-|-|
| sub_828470E0 | TLS interrupt disable (leaf) | No |
| sub_82852300 | Stream read + hash iterate | No (see below) |
| sub_82851DF0 | Hash table lookup (leaf) | No |
| sub_8284FC98 | Alloc + init resource handle | No (see below) |
| sub_82847120 | TLS interrupt restore (leaf) | No |

## sub_82852300 call tree

| Function | Role | Blocking? |
|-|-|
| sub_8285AD08 | Buffered stream reader | No static blocking |
| sub_828D0608 | Hash bucket iterator (leaf) | No |
| sub_82851C40 | Iterator advance (leaf) | No |

### sub_8285AD08 deeper calls

| Function | Role | Blocking? |
|-|-|
| sub_8285A8B0 | Stream flush/refill | No static blocking |
| sub_82A00DC0 | memcpy (CRT) | No |
| INDIRECT vtable[20] | Stream write/read op | **Unknown at compile time** |

### sub_8285A8B0 deeper calls

| Function | Role | Blocking? |
|-|-|
| sub_82854C80 | Read loop via vtable[32] | **Unknown — indirect** |
| INDIRECT vtable[36] | Flush/seek | **Unknown** |
| INDIRECT vtable[52] | Refill/load | **Unknown** |

## sub_8284FC98 call tree

| Function | Role | Blocking? |
|-|-|
| sub_821B3510 | Memory allocator | No |
| sub_8284D220 | Key-value compare/copy | No |
| INDIRECT vtable[8] | Object setup | **Unknown** |

## Indirect (vtable) calls in sub_82852A50 itself

1. **Line 30603**: `PPC_CALL_INDIRECT_FUNC` — entry->vtable_at_16 method 3 (offset 12). Lookup by key.
2. **Line 30619–30620**: `PPC_CALL_INDIRECT_FUNC` — result->vtable[1] (offset 4). Likely SetIndex or Bind.
3. **Line 30638–30640**: `PPC_CALL_INDIRECT_FUNC` — result->vtable[0] (offset 0). Likely AddRef/Release.

## Global State

- Resource manager pointer loaded from `0x831E55EC`
- Lock flag: bit 17 of `[[0x831E55EC]+40]`
- If bit 17 set, TLS interrupt disable/restore wraps entire function

## Blocking Verdict

**No known blocking functions** (sub_82849790, sub_828497D8, sub_82849820, sub_828498C0, sub_8285FE78, sub_8285FEF8, sub_8285D110, sub_82A13040) appear anywhere in the static call tree.

The blocking risk is **entirely from indirect vtable calls**:
- 3 in sub_82852A50 itself
- 2 in sub_8285AD08 (stream I/O)
- 3 in sub_8285A8B0 (stream flush/refill)
- 1 in sub_82854C80 (read loop)
- 1 in sub_8284FC98 (object setup)

Total: **10 indirect calls** that could resolve to blocking implementations at runtime.

The most likely blocking path is the **stream I/O chain**: sub_82852300 → sub_8285AD08 → sub_8285A8B0 → sub_82854C80, where vtable methods perform file reads that may internally wait on I/O completion events (NtWaitForSingleObjectEx). This is the RAGE streaming subsystem's buffered reader — if the underlying file device's read method blocks on an async I/O completion event, the entire call chain hangs.

## Callers (10 total in generated code)

- sub_82852D18 (line 30982) — **first call in function**, probed as INIT_PROBE 30555
- sub_82852B78 (line 30747) — "ShaderBind" path
- sub_82852DD0 via sub_82852D18
- Multiple callers in gta4_recomp.50/51/52/59/60.cpp

## Hook Status in imports.cpp

- `INIT_PROBE` (pass-through trace) at line 1772 — no override, just logs entry/return
- No blocking bypass hook exists
- **No occurrence of "82852A50" outside the INIT_PROBE and a comment at line 1278**
