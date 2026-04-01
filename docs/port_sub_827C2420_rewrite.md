# sub_827C2420 — Activate Streaming (Phase 23 Tail)

## Location
`gta4_recomp.50.cpp` line 57394

## High-Level Pseudocode

```cpp
// Called from sub_82478AF8 Phase 23 with this = stream manager (from 0x82B393A4)
void StreamManager::ActivateStreaming(/* this */) {
    StreamingRequestStack* stack = g_StreamingRequestStack;   // 0x82B07278
    StreamingCoordinator*  coord = g_StreamingCoordinator;    // *(0x831E55EC)

    // 1. Push the search path for this stream manager
    stack->PushSearchPath(this->searchPath);                  // sub_8284F310

    // 2. Get the resource name via vtable dispatch
    const char* resourceName = this->vtable[1](this);        // indirect call

    // 3. Load the resource through the coordinator
    coord->LoadResourceByName(                                 // sub_82852DD0
        /* stringA  */ (const char*)0x8208A57C,   // unknown .rdata string
        /* stringB  */ (const char*)0x820CBFE4,   // unknown .rdata string
        /* resource */ resourceName,
        /* owner    */ this,
        /* flag     */ 1
    );

    // 4. Pop the search path
    stack->PopSearchPath();                                    // sub_8284E830
}
```

## Argument Flow

| Callee | r3 | r4 | r5 | r6 | r7 | r8 |
|-|-|-|-|-|-|-|
| sub_8284F310 | stack (0x82B07278) | this->searchPath (offset 56) | - | - | - | - |
| vtable[1] | this | - | - | - | - | - |
| sub_82852DD0 | coordinator | 0x8208A57C | 0x820CBFE4 | vtable[1] result | this | 1 |
| sub_8284E830 | stack (0x82B07278) | - | - | - | - | - |

## Sub-Function Documentation

### sub_8284F310 — PushSearchPath
**File:** `gta4_recomp.55.cpp` line 22166
**Signature:** `void PushSearchPath(StreamingRequestStack* stack, const char* path)`

1. If `path[0] == '$'` (0x24): treat as a device-relative path, skip to direct copy.
2. Otherwise call `sub_8284E690(path)` to check if it's a relative path:
   - Returns 1 if NULL, or if first char is not `/`, `\`, and contains no `:`.
   - Returns 0 if path is absolute (starts with `/`, `\`, or contains `:`).
3. If relative: read current stack depth from `stack[3072]`. If depth > 0, copy the existing top-of-stack path prefix, then append the new relative path.
4. If absolute or `$`-prefixed: copy directly.
5. Path is stored at `stack + (depth + 4) * 256` using `sub_8284EAF0` (strncpy + backslash-to-slash normalization, max 256 chars).
6. Increment `stack[3072]` (depth counter).

### sub_8284E830 — PopSearchPath
**File:** `gta4_recomp.55.cpp` line 20574
**Signature:** `void PopSearchPath(StreamingRequestStack* stack)`

Simple: decrements `stack[3072]`.

### sub_82852DD0 — LoadResourceByName (Resource Loader Wrapper)
**File:** `gta4_recomp.55.cpp` line 31076
**Signature:** `bool LoadResourceByName(Coordinator* coord, const char* pathA, const char* pathB, const char* resourceName, StreamManager* owner, bool flag)`

1. Calls `sub_8284F468(g_StreamingRequestStack, pathA, pathB, 0, index, 1)` to search all registered search paths for the resource file. This iterates `stack[3076]` registered paths, building full paths via `sub_8284F0C0`, then calling `sub_8285AA68` to open each candidate.
2. If found (non-NULL result): calls `sub_82852D18(coord, findResult, resourceName, owner, flag)`.
3. Calls `sub_8285B088(findResult)` to release the find handle.
4. Returns 0 (not found) or the result of the inner load.

### sub_82852D18 — Inner Resource Loader
**File:** `gta4_recomp.55.cpp` line 30962
**Signature:** `bool InnerLoad(Coordinator* coord, uint32_t findHandle, const char* resourceName, StreamManager* owner, bool flag)`

1. Calls `sub_82852A50(coord, findHandle)` to resolve to a resource entry. This function:
   - Reads `coordinator[40]` and extracts bit 17 to decide on thread-locking.
   - Calls `sub_82852300(coord, findHandle, &slot, &extra)` to find the entry.
   - If found, calls `entry->vtable2[3](entry, findHandle)` to verify/bind.
   - Calls `sub_8284FC98(entry)` and `sub_821B3560(entry)` for ref counting.
2. If entry is NULL: return 0.
3. Reads `entry[0]` (the resource object pointer).
4. Calls `sub_82851A10(coord, entry[0])` to look up the resource in the coordinator's hash map (at `coord + 24`). This strips leading `__` from names before lookup.
5. Calls `entry->vtable[2](resourceName, entry, owner)` — the actual load/activate virtual method.
6. Checks `coordinator[40]` bit 17 for thread locking (sub_828470E0 lock / sub_82847120 unlock).
7. Calls `sub_8284FA58(entry)` for finalization and `sub_821B3560(entry)` to free/release.
8. Returns 1 on success.

### sub_8285B088 — Release Find Handle
**File:** `gta4_recomp.56.cpp` line 2343
**Signature:** `void ReleaseFindHandle(FindResult* result)`

1. If `result[20] == 0` and `result[16] != 0`: calls `sub_8285A8B0()` (some conditional cleanup).
2. Reads `result[0]` (parent object) and `result[4]` (handle ID).
3. Calls `parent->vtable[10](parent, handleID)` to release.
4. Clears `result[0] = NULL`, `result[4] = -1`.

## Global Objects

### g_StreamingRequestStack (0x82B07278)
A static instance of the path-search stack. Layout:

| Offset | Size | Field |
|-|-|-|
| 0x000 | 1024 | Reserved / base path area |
| 0x400 | 256 | Path slot [0] (depth=0) |
| 0x500 | 256 | Path slot [1] (depth=1) |
| ... | 256 each | Up to ~8 nested slots |
| 0xC00 | 4 | `depth` (current push depth, inc/dec by push/pop) |
| 0xC04 | 4 | `numSearchPaths` (number of registered search paths, loop bound in sub_8284F468) |
| 0xC08 | 4 | `defaultIndex` (used in sub_8284F4E0) |

**Total size:** ~3084+ bytes. Slots are at `base + (depth + 4) * 256`, max 256 chars per path.

### g_StreamingCoordinator (0x831E55EC)
A pointer to the global streaming coordinator object. Key fields:

| Offset | Type | Field |
|-|-|-|
| 0 | ptr | vtable |
| 24 | obj | Hash map / resource registry (passed to sub_82851A10 as `this + 24`) |
| 40 | u32 | Flags word (bit 17 = thread-safety locking enabled) |

The coordinator is loaded indirectly: `*(uint32_t*)0x831E55EC`. It manages the resource registry and dispatches load operations.

### g_StreamManagerInstance (0x82B393A4)
A pointer to the active stream manager instance. Created in `sub_82477670`:

1. Allocates 912 bytes via `sub_821B3510(912)` (the game's operator new).
2. Calls constructor `sub_827C2650` which sets initial vtable `0x8207A574`.
3. Overwrites vtable with `0x8201C004` (the derived class vtable).
4. Stores the pointer at `0x82B393A4`.
5. Stores the current path string at offset 56.

### Stream Manager Struct Layout (912 bytes)

| Offset | Type | Field |
|-|-|-|
| 0 | ptr | vtable (set to 0x8201C004 for this derived class) |
| 4 | float | field_04 (initialized from .rdata) |
| 8 | float | field_08 |
| 12 | float | field_0C |
| 16 | float | field_10 |
| 20 | float | field_14 |
| 24 | float | field_18 |
| 28 | float | field_1C |
| 32 | float | field_20 |
| 36 | u8 | field_24 (= 0) |
| 40 | float | field_28 |
| 44 | float | field_2C |
| 48 | float | field_30 |
| 52 | float | field_34 |
| 56 | ptr | searchPath (const char*, set by caller) |
| 60 | u32 | field_3C (= 0) |
| 64 | u32 | field_40 (= 0) |
| 68 | u32 | field_44 (= 0) |
| 72 | float | field_48 |
| 76 | u32 | field_4C (= 0) |
| 80 | u32 | field_50 (= 0) |
| 84 | u32 | field_54 (= 0) |
| 88 | u32 | maxEntries (= 2048) |
| 92 | u32 | field_5C (= 0) |
| 96 | ptr | entryBuffer (allocated as maxEntries * 4 bytes) |
| 100 | obj | Sub-object constructed via sub_828D2E08 (starts at +100) |
| 876 | 12 | StatusBlock A (5 bytes flags + padding + u32) |
| 888 | 12 | StatusBlock B (5 bytes flags + padding + u32) |
| 908 | u8 | field_38C (= 0) |

**Vtable at 0x8201C004:**

| Slot | Purpose |
|-|-|
| [0] | Base virtual method (unused here) |
| [1] | GetResourceName() -> returns const char* (called in sub_827C2420) |

## Call Graph

```
sub_82478AF8 (Phase 23)
  |
  +-- sub_8284F310(stack, pathStr)         // push search path
  +-- sub_82477670()                       // create stream manager -> 0x82B393A4
  |     +-- sub_821B3510(912)              // operator new
  |     +-- sub_827C2650()                 // constructor
  +-- store pathStr at streamMgr[56]
  +-- sub_827C2420(streamMgr)              // <== THIS FUNCTION
  |     +-- sub_8284F310(stack, this[56])  // push search path
  |     +-- vtable[1](this)               // get resource name
  |     +-- sub_82852DD0(coord, ...)       // find + load resource
  |     |     +-- sub_8284F468(stack, ...) // search all paths
  |     |     +-- sub_82852D18(coord, ...) // inner load
  |     |     |     +-- sub_82852A50()     // resolve entry
  |     |     |     +-- sub_82851A10()     // hash map lookup
  |     |     |     +-- vtable[2]()        // virtual load
  |     |     +-- sub_8285B088()           // release find handle
  |     +-- sub_8284E830(stack)            // pop search path
  +-- sub_8284E830(stack, pathStr)         // pop search path (outer)
```

## Native Rewrite Skeleton

```cpp
struct StreamManager {
    void** vtable;                    // +0
    float  params[9];                 // +4 through +36
    uint8_t flag_24;                  // +36
    float  params2[4];                // +40 through +52
    const char* searchPath;           // +56
    uint32_t fields_3C_44[3];         // +60..68
    float  field_48;                  // +72
    uint32_t fields_4C_5C[4];         // +76..92
    void*  entryBuffer;               // +96
    // ... sub-objects and status blocks through offset 912
};

void StreamManager_ActivateStreaming(StreamManager* self) {
    auto* stack = GetStreamingRequestStack();  // 0x82B07278
    auto* coord = GetStreamingCoordinator();   // *(0x831E55EC)

    stack->PushSearchPath(self->searchPath);

    const char* resourceName = self->GetResourceName();  // vtable[1]

    coord->LoadResourceByName(
        "stringA",       // 0x8208A57C - needs .rdata dump to identify
        "stringB",       // 0x820CBFE4 - needs .rdata dump to identify
        resourceName,
        self,
        true
    );

    stack->PopSearchPath();
}
```

## Key Observations for Porting

1. **Thread safety**: The coordinator checks bit 17 of its flags word (offset 40) to decide whether to acquire/release a lock (sub_828470E0/sub_82847120) around resource operations.
2. **Search path stack**: A simple push/pop depth counter with 256-byte path slots. Paths are normalized (backslash to forward slash). Relative paths get prepended with the current top-of-stack.
3. **The two string constants** at 0x8208A57C and 0x820CBFE4 are likely file extensions or path fragments used by the resource finder. They need to be identified from an .rdata dump.
4. **vtable[1]** on the stream manager returns the resource name/path that this manager is responsible for streaming. The vtable at 0x8201C004 is a derived class override.
5. **sub_8285B088** (release find handle) calls the parent object's vtable[10] to release, then NULLs out the handle. This is RAII-like cleanup for the find result.
