# Streaming Coordinator Object — Full Analysis

## Global Pointer

| Symbol | Address | Description |
|-|-|-|
| g_pgStreamerCoordinator | 0x831E55EC | Pointer to 48-byte coordinator singleton |

Two independent init paths write this pointer:

1. **sub_821B3C80** (simple path): Allocates 48 bytes via `sub_821B3510`, calls `sub_82852F48` (initializer), stores to `0x831E55EC`. Called by `sub_82916330` when coordinator is NULL. Source: `gta4_recomp.2.cpp` lines 84247-84307.

2. **sub_821B3CE8** (RAGE engine init): Checks if 0x831E55EC is already non-NULL. If NULL, allocates 48 bytes, constructs, stores. Then calls `sub_82852FB0` (AddModule) and `sub_8285D948` (streaming thread setup). Source: `gta4_recomp.2.cpp` lines 84610-84674.

The address is encoded as `lis rN,-31970` (= 0x831E0000) + `lwz/stw rN,21996(rN)` (offset 0x55EC).

## Constructor: sub_82852F48

Source: `gta4_recomp.55.cpp` lines 31309-31363.

Zeroes all fields then sets:
- offset 36 = 4 (max resource type count)
- offset 40 = 0x3 (flags: bits 0 and 1 set)

No vtable is installed. The coordinator is a plain struct, not a virtual class.

## Coordinator Struct Layout (48 bytes)

Each of the three 12-byte "slots" is a hash map header (see Hash Map section below).

| Offset | Size | Type | Name | Init | Description |
|-|-|-|-|-|-|
| +0 | 4 | u32 | hashmap0.buckets | 0 | Hash map 0: bucket array pointer |
| +4 | 2 | u16 | hashmap0.count | 0 | Hash map 0: bucket count |
| +6 | 2 | u16 | hashmap0.used | 0 | Hash map 0: entries inserted |
| +8 | 3 | pad | -- | 0 | Padding |
| +11 | 1 | u8 | hashmap0.flag | 0 | Hash map 0: active flag |
| +12 | 4 | u32 | hashmap1.buckets | 0 | Hash map 1: bucket array pointer |
| +16 | 2 | u16 | hashmap1.count | 0 | Hash map 1: entries inserted |
| +18 | 2 | u16 | hashmap1.used | 0 | Hash map 1: entries used |
| +20 | 3 | pad | -- | 0 | Padding |
| +23 | 1 | u8 | hashmap1.flag | 0 | Hash map 1: active flag |
| +24 | 4 | u32 | hashmap2.buckets | 0 | Hash map 2 (resource name map): bucket array ptr |
| +28 | 2 | u16 | hashmap2.count | 0 | Hash map 2: bucket count |
| +30 | 2 | u16 | hashmap2.used | 0 | Hash map 2: entries inserted |
| +32 | 3 | pad | -- | 0 | Padding |
| +35 | 1 | u8 | hashmap2.flag | 0 | Hash map 2: active flag |
| +36 | 4 | u32 | maxTypes | 4 | Max resource type handlers |
| +40 | 4 | u32 | flags | 0x3 | Configuration flags (see below) |
| +44 | 4 | u32 | callCounter | 0 | Incremented by AddModule (sub_82852FB0) |

## Hash Map at Coordinator+24 (Resource Name Map)

The hash map at offset +24 is the primary resource name lookup table, accessed by `sub_82851A10` and `sub_82852E48`.

### Hash Map Header (12 bytes per slot)

| Offset | Size | Type | Description |
|-|-|-|-|
| +0 | 4 | u32* | Pointer to bucket array (each bucket is a u32 pointer to first node) |
| +4 | 2 | u16 | Bucket count (hash modulus) |
| +6 | 2 | u16 | Entry count (total nodes inserted) |
| +8-10 | 3 | pad | Padding |
| +11 | 1 | u8 | Active/initialized flag |

### Hash Map Node Layout (chained)

Each node in a bucket chain:

| Offset | Size | Type | Description |
|-|-|-|-|
| +0 | 4 | char* | Key: null-terminated resource name string |
| +4 | 4 | void* | Value: pointer to resource entry |
| +8 | 4 | node* | Next pointer in chain (0 = end) |

### Lookup: sub_82912948

Source: `gta4_recomp.61.cpp` lines 42616-42724.

1. Hash the key string via `sub_8285A720`
2. Modulo bucket_count to get bucket index
3. Walk the chain comparing keys byte-by-byte (case-sensitive `strcmp`)
4. Return `&node->value` (node+4) on match, or NULL

### Registration: sub_82851A10

Source: `gta4_recomp.55.cpp` lines 28111-28181.

1. Strip leading `__` prefix from resource name (checks for two consecutive 0x5F bytes)
2. Call `sub_82912948(coordinator+24, &name)` to look up in hash map 2
3. If found, dereference twice to get the resource handler pointer

### Insertion: sub_828523C8

Source: `gta4_recomp.55.cpp` lines 29596-29676.

1. If bucket_count == 0, initialize with 10 buckets via `sub_82849FF0`
2. Increment entry count (+6)
3. If entry count exceeds bucket count, rehash via `sub_8285A610` + `sub_827AD700`
4. Hash the key, find bucket, insert node at head of chain

## Flag Bits (offset +40)

These are extracted by various functions using `rlwinm Ra,Rs,SH,31,31` which extracts bit `(32-SH)` from the LSB:

| Bit (LSB) | Mask | rlwinm SH | Checked In | Purpose |
|-|-|-|-|-|
| 0 | 0x00000001 | -- | -- | Unknown (set at init via `ori ,1`) |
| 1 | 0x00000002 | 31 | sub_82851858 | Unknown (set at init via `ori ,2`) |
| 11 | 0x00000800 | 21 | sub_828518A8 | Cleared during ProcessAndFlush |
| 12 | 0x00001000 | 20 | sub_828518A8 | Cleared during ProcessAndFlush |
| 13 | 0x00002000 | 19 | sub_828518A8 | Cleared during ProcessAndFlush |
| 17 | 0x00020000 | 15 | sub_82852D18, sub_82852FB0, sub_82852A50, sub_82852E80 | Thread-safety lock enable |

### Thread-Safety Lock (bit 17)

When bit 17 is set, streaming functions bracket their critical sections with:
- `sub_828470E0` (lock): reads TLS[r13+0] to get thread block, then uses offsets 1668/1672/1676/1680 for recursive lock state
- `sub_82847120` (unlock): decrements recursion counter at TLS offset 1668

Functions that check bit 17: `sub_82852D18` (ProcessOpenedResource), `sub_82852A50` (ReadAndParseStream), `sub_82852FB0` (AddModule), `sub_82852E80` (iterate resources), and `sub_828091B4` (resource lookup).

With initial value 0x3, only bits 0-1 are set. All operational bits (11-13, 17) are OFF by default. This means the TLS-based lock is never acquired during normal streaming — the system runs single-threaded by default and only enables locking when explicitly configured.

## sub_827C2420 Missing Null Check

Source: `gta4_recomp.50.cpp` lines 57392-57462.

`sub_827C2420` (activate-streaming) loads the coordinator pointer from 0x831E55EC at line 57425 and passes it as `r3` to `sub_82852DD0` at line 57452 **without any null check**. If the coordinator has not been initialized by `sub_821B3CE8` (RAGE engine init) before `sub_827C2420` runs, the null pointer propagates into `sub_8284F468` which dereferences `r3+3076` and crashes.

In practice, the init order ensures safety: `sub_821B3CE8` (RAGE engine init, called from `sub_82140000`) runs before `sub_82478AF8` phase 22 which calls `sub_827C2420`. The INIT_PROBE numbering confirms this — the `sub_82140000` gate must succeed before phase 22 streaming activation begins.

However, if the RAGE init gate (`sub_82140000`) is hooked to return early without running `sub_821B3CE8`, the coordinator will be NULL and `sub_827C2420` will crash. The existing `s_rageInitDone` guard in `imports.cpp` (line 1586) prevents re-entry but does not skip the first call.

## The Hanging Call Chain

```
sub_827C2420 (activate-streaming, called from phase 22 of sub_82478AF8)
  -> sub_82852DD0 (OpenAndProcess) [imports.cpp line 1790, traced]
    -> sub_8284F468 (FindAndOpenResource on g_streamingMgr at 0x82B07278)
    -> sub_82852D18 (ProcessOpenedResource) [imports.cpp line 1806, traced]
      -> sub_82852A50 (ReadAndParseStream) -- returns OK
      -> sub_82851A10 (RegisterTypeHandler) -- returns OK
      -> INDIRECT CALL: callback->vtable[2](callback, resData, flags) -- HANGS
    -> sub_8285B088 (FlushAndClose) -- never reached
```

## Runtime Trace Evidence

From `/tmp/liberty_run.log` line 792:
```
[DIAG-82852D18] call=1 r3=0xD90BA660 r4=0x831B5DF4 r5(cb_obj)=0xD909D2A0
  vtable=0x82087934 vtable[2]=0x8286C238 r6=0xDAB4ECB0
```

- sub_82852DD0 enters at line 791 and **never returns**
- sub_82852A50 and sub_82851A10 return normally (lines 793-796)
- The hang is in the vtable[2] call to `sub_8286C238`
- Render loop continues on main thread (frames 3-900+), confirming this is a background streaming thread

## The Callback Object

| Field | Value | Description |
|-|-|-|
| Address | 0xD909D2A0 | Heap-allocated callback object |
| vtable_ptr | 0x82087934 | Points past dtor slot |
| vtable[2] | 0x8286C238 | rage::datResourceInfo visitor (pgStreamable tree traversal) |

### Vtable at 0x82087930 (full layout)

| Slot | Address | Function | Description |
|-|-|-|-|
| [0] | 0x820FE780 | dtor | Destructor |
| [1] | 0x8286C828 | setup | Installed as vtable_ptr+0 in object |
| [2] | 0x8286C750 | unknown | Installed as vtable_ptr+4 in object |
| [3] | 0x8286C238 | processResource | Installed as vtable_ptr+8 in object = **THE HANG** |

The object stores `vtable_ptr = 0x82087934` (skipping the destructor at slot 0). So `object->vt[2]` = full vtable slot [3] = `sub_8286C238`.

This vtable belongs to the class constructed by `sub_8286C778` (source: `gta4_recomp.56.cpp` line 44558). The class has a 64-byte object layout and is a singleton at `0x831C62A8`.

## sub_8286C238 — Why It Hangs

Source: `gta4_recomp.56.cpp` lines 43805-44374. 416-byte stack frame.

The function is the `rage::datResourceInfo` tree visitor. According to `port_sub_8286C238_vtable_target.md`:

- **No GPU calls** — no D3D, no Xenos, no PM4 ring buffer access
- **No sync primitives** — no semaphores, events, mutexes, or blocking waits
- All 10 sub-calls are pure computation or memory traversal

### Possible Hang Causes

1. **Linked list corruption**: The inner loop at `loc_8286C3E4` walks a linked list via `sub_82863248`. If the list is circular (node->next at +24 never reaches the sentinel), the loop spins forever.

2. **Indirect vtable calls on resource entries** (4 dispatch sites):
   - `0x8286C414`: vtable[1] (getName) on resource entry
   - `0x8286C4A0`: vtable[2] (getSize) on resource entry
   - `0x8286C4DC`: vtable[8] (apply/visit) on resource entry
   - `0x8286C560`: vtable[1] (getName, debug path)

   If any resource entry has a corrupt vtable or points to a function that blocks, the traversal hangs.

3. **Array count overflow**: The outer loop count from `sub_8286BF20` could be huge if the resource linked list is corrupt.

## AddModule: sub_82852FB0

Source: `gta4_recomp.55.cpp` lines 31365-31457.

Called on the coordinator to finalize a module registration. Checks `offset 44 == 0` (first call only), then:
1. Stores module params at offset 36-39
2. Calls `sub_8285A1B0` (GPU ring buffer dispatch — **known blocker**)
3. Calls `sub_828526A0` (slot registration)
4. Calls `sub_82851918` (type handler registration)
5. Increments counter at offset 44

**Note**: `sub_8285A1B0` is a GPU dispatch that hangs without real Xenos hardware. This function is only called during module registration (init time), not during resource processing.

## RegisterModule: sub_82852E48

Source: `gta4_recomp.55.cpp` lines 31150-31186.

Called by ~30+ module classes to register with the coordinator. Signature:
```
void RegisterModule(coordinator, moduleTypeID, modulePtr);
// r3 = coordinator (from 0x831E55EC)
// r4 = moduleTypeID (from module->vtable[1])
// r5 = module object pointer
```

Stores the module into the coordinator's linked list at offset 24 via `sub_828523C8`.

## Port Strategy

### For the vtable[2] hang (sub_8286C238)

The function itself is stateless computation — no I/O, no GPU, no sync. The hang is most likely caused by:

1. **Uninitialized resource tree**: The resource data parsed by `sub_82852A50` may reference guest memory that was never properly initialized on the host. Pointer fixups in the pgStreamable system expect Xbox 360 memory layout. On the port, guest heap allocations may not produce valid linked list structures.

2. **Resource entry vtable corruption**: The 4 indirect vtable dispatches on resource entries will crash or hang if the entries point to invalid guest addresses.

### Recommended approach

**Hook sub_82852D18 to return success (1) without calling vtable[2]**. The resource processing visitor is an in-memory fixup/relocation step for pgStreamable resources. On the port:

- File I/O is already handled by rexcrt hooks (ReadFile, CreateFileA, etc.)
- Resource data is loaded into host memory by `sub_82852A50` which completes successfully
- The tree visitor relocates pointers within the resource — but the port's flat memory model means these relocations are either unnecessary or actively harmful
- The cleanup steps (sub_8284FA58, sub_821B3560) should still run to free the slot

```cpp
PPC_FUNC_HOOK(sub_82852D18) {
    // Skip vtable[2] tree visitor — not needed on host platform.
    // The resource data is already in host memory with correct pointers.
    // Set r3 = 1 (success) to let callers proceed.
    ctx.r3.u32 = 1;
}
```

**Risk**: If any game code depends on side effects of the visitor (dirty flags, observer callbacks, version checks), those won't fire. Monitor for missing resources or rendering artifacts.

### For sub_82852FB0 (AddModule)

Already handled indirectly: `sub_8285A1B0` (GPU dispatch) and `sub_8285A8B0` (GPU buffer flush) are stubbed. The AddModule path should work with existing hooks.

### For the coordinator global at 0x831E55EC

No changes needed. The coordinator is allocated and initialized by the recompiled code. Its flags at offset 40 are 0x3, meaning all debug features are off. The coordinator's role is bookkeeping — it tracks which module types are registered. This works correctly in the port.

## Related Files

| Path | Description |
|-|-|
| `gta4_recomp.2.cpp:84247` | sub_821B3C80 — coordinator allocation (simple path) |
| `gta4_recomp.2.cpp:84309` | sub_821B3CE8 — RAGE engine init (full path, checks NULL first) |
| `gta4_recomp.50.cpp:57392` | sub_827C2420 — activate-streaming (no null check on coordinator) |
| `gta4_recomp.55.cpp:2434` | sub_828470E0 — TLS-based recursive lock (thread-safety) |
| `gta4_recomp.55.cpp:2473` | sub_82847120 — TLS-based recursive unlock |
| `gta4_recomp.55.cpp:22164` | sub_8284F310 — start-streaming-mgr (path builder) |
| `gta4_recomp.55.cpp:22359` | sub_8284F468 — FindAndOpenResource (iterates modules) |
| `gta4_recomp.55.cpp:28111` | sub_82851A10 — resource name lookup (strips `__`, queries hashmap2) |
| `gta4_recomp.55.cpp:29596` | sub_828523C8 — hash map insert (chained, auto-rehash) |
| `gta4_recomp.55.cpp:30529` | sub_82852A50 — ReadAndParseStream (loads from coordinator) |
| `gta4_recomp.55.cpp:30960` | sub_82852D18 — ProcessOpenedResource (the hanging function) |
| `gta4_recomp.55.cpp:31074` | sub_82852DD0 — OpenAndProcess (top-level caller) |
| `gta4_recomp.55.cpp:31150` | sub_82852E48 — RegisterModule (adds to hashmap at +24) |
| `gta4_recomp.55.cpp:31188` | sub_82852E80 — iterate resources (accesses +24 hashmap, checks bit 17) |
| `gta4_recomp.55.cpp:31309` | sub_82852F48 — coordinator constructor (zeros + flags = 0x3) |
| `gta4_recomp.55.cpp:31365` | sub_82852FB0 — AddModule (increments counter, GPU dispatch) |
| `gta4_recomp.56.cpp:2341` | sub_8285B088 — FlushAndClose (reads fields +16, +20 of resource) |
| `gta4_recomp.56.cpp:43805` | sub_8286C238 — vtable[2] target: pgStreamable tree visitor |
| `gta4_recomp.56.cpp:44558` | sub_8286C778 — module class constructor (installs vtable 0x82087934) |
| `gta4_recomp.61.cpp:42616` | sub_82912948 — hash map lookup (hash + chain walk + strcmp) |
| `gta4_recomp.61.cpp:51823` | sub_82916330 — alternate init path (calls sub_821B3C80 if NULL) |
| `imports.cpp:1586` | s_rageInitDone guard for sub_82140000 |
| `imports.cpp:1650` | Existing trace hook for sub_821B3CE8 (RAGE engine init) |
| `imports.cpp:1790` | Existing trace hook for sub_82852DD0 |
| `imports.cpp:1806` | Existing trace hook for sub_82852D18 |
