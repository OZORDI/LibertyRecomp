# Analysis: sub_82851A10 and Vtable Dispatch in sub_82852D18

## sub_82851A10 — String Hash-Map Lookup (Non-Blocking)

**Location**: `gta4_recomp.55.cpp` line 28113
**Signature**: `sub_82851A10(r3=container_ptr, r4=string_ptr)`
**Returns**: r3 = pointer to value, or 0 if not found

### Behavior

1. Checks if string starts with `"__"` (two underscores, ASCII 95). If so, advances past them.
2. Stores (possibly adjusted) string pointer at stack+80.
3. Calls `sub_82912948(r3+24, &stack_string_ptr)` — a **hash table lookup**.
4. If result is non-null: dereferences twice (`*(*result)`) and returns the inner pointer.
5. If result is null: returns 0.

### sub_82912948 — Hash Table String Lookup (Non-Blocking)

**Location**: `gta4_recomp.61.cpp` line 42618

This is a standard open-addressing hash map lookup:
- Calls `sub_8285A720(string)` to compute a **string hash** (iterative hash, non-blocking).
- Uses `hash % table_size` as bucket index into a pointer array at `table+0`.
- Walks the collision chain (linked via offset +8 in each node).
- Compares strings byte-by-byte (inline strcmp).
- Returns `node+4` on match, or NULL.

**No blocking operations anywhere in sub_82851A10 or its callees.**

## Vtable Dispatch After sub_82851A10 (sub_82852D18 line 31013-31027)

```
r11 = PPC_LOAD_U32(r29 + 0);   // vtable ptr from object at r29
r11 = PPC_LOAD_U32(r11 + 8);   // vtable[2]
call vtable[2](r29, r31, r28)  // r29=handler, r31=node, r28=context
```

### Tracing r29 Back to Origin

| Function | Register | Source |
|-|-|
| sub_82852D18 | r29 | = r5 param |
| sub_82852DD0 | r5 to sub_82852D18 | = r29 of sub_82852DD0 = r6 param |
| sub_827C2420 | r6 to sub_82852DD0 | = r3 return from `this->vtable[1](this)` |

**The vtable object is the return value of a virtual call** — it cannot be statically resolved. The dispatched method is `vtable[2]` of whatever object `this->vtable[1](this)` returns.

### Static Parameters from sub_827C2420

| Value | Address | Role |
|-|-|-|
| `(-32244 << 16) + (-16412)` | **0x820BBFE4** | r5 = event/command name string (`.rdata`) |
| `(-32248 << 16) + (-23172)` | **0x8207A57C** | r4 = type name string (`.rdata`) |
| `(-32080 << 16) + 29304` | **0x82B07278** | Container object (`.data`) |
| `*((-31970 << 16) + 21996)` | load from **0x831E55EC** | Global event manager pointer |

### sub_827C2420 Call to sub_82852DD0 — Full Parameter Map

```
r3 = PPC_LOAD_U32(0x831E55EC)          // global event manager
r4 = 0x8207A57C                         // type name string
r5 = 0x820BBFE4                         // event name string
r6 = this->vtable[1](this)              // runtime: handler object
r7 = this                               // original object
r8 = 1                                  // flag (immediate dispatch)
```

### sub_82852DD0 Internal Flow

1. Stores r6 (handler) in r29, r7 (this) in r28.
2. Calls `sub_8284F468(0x82B07278, 0x8207A57C, 0x820BBFE4, 0, 1)` — pool/resource lookup with the two string addresses. Iterates registered entries, builds a path string, compares with `sub_8285AA68`.
3. If pool lookup returns non-null (r31): calls `sub_82852D18(global, r31, r29_handler, r28_this)`.
4. Cleans up with `sub_8285B088`.

### sub_82852D18 Internal Flow

1. Calls `sub_82852A50(global, result)` — acquires an event node. Internally:
   - Conditionally calls `sub_828470E0` (TLS recursion guard — NOT a mutex/lock, just a counter check).
   - Calls `sub_82852300` for iterator-based node matching via hash lookup.
2. If node found: loads `node->ptr` (r31), calls `sub_82851A10` for string hash lookup.
3. **Vtable dispatch**: `r29->vtable[2](r29, node->ptr, context)` — the actual event handler.
4. Calls `sub_8284FA58` + `sub_821B3560` for cleanup/deref.
5. Conditionally calls `sub_82847120` (TLS guard restore).

### sub_828470E0 / sub_82847120 — TLS Recursion Guard (Non-Blocking)

These are **not** mutexes. They read/write TLS fields at offsets 1668-1680:
- `sub_828470E0`: If TLS[1676] == TLS[1680], increments TLS[1668] (re-entrancy count). Otherwise saves/swaps state.
- `sub_82847120`: Decrements TLS[1668] or restores prior state.

No syscalls, no waits, no blocking.

## Blocking Assessment

**sub_82851A10**: Pure computational — string prefix check + hash table lookup. Zero blocking risk.

**sub_82852D18 overall**: The only potential blocking is in the **indirect vtable[2] call** itself, which dispatches to a runtime-determined handler. The surrounding infrastructure (hash lookup, TLS guard, node acquisition) is entirely non-blocking.

**sub_82852A50**: Contains indirect vtable calls for node matching (`offset+12` dispatch), plus `sub_828D0608` for iterator init. These are computational, not synchronization primitives.

The critical question is: **what does vtable[2] of the handler object do?** Since the handler comes from `this->vtable[1](this)` in sub_827C2420, it depends on the concrete class of the `this` parameter. The vtable is determined at runtime.
