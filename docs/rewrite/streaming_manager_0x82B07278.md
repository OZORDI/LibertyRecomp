# Streaming Manager Object (0x82B07278) — Full Struct Map

## Class Hierarchy

```
CFileManager          vtable: 0x82078D28   ctor: sub_827ADAC8 / sub_827AD200
  └ CFileManagerBase  vtable: 0x82078D38   ctor: sub_827AD9C8
      └ CFileManagerHashed  vtable: 0x82078D48   ctor: sub_827ADB48
```

Inner interface object at `this+16`: vtable `0x82078CB4`, ctor `sub_827ACCE0`.

The instance at `0x82B07278` is a `CFileManagerHashed`.

## Global Pointers

| Address | Name | Purpose |
|-|-|-|
| `0x82FF5368` | `g_streamingModule` | Ptr to CFileManagerHashed instance (0x82B07278) |
| `0x82FF536C` | `g_streamingArray` | Ptr to allocated streaming entry array (176 B/entry) |
| `0x82FF541C` | `g_streamingFlags` | Init flags (bit 0 = capacity set, bit 1 = maxReq set) |
| `0x82FF5418` | `g_streamingCapacity` | Memory capacity for streaming |
| `0x82FF5414` | `g_streamingMaxReq` | Max concurrent requests (default 168) |
| `0x8318F1A0` | `g_resourceMgr` | Resource manager instance ptr |
| `0x831AB810` | `g_debugOutput` | Debug/log output ptr (used by sub_8284F4E0) |

## CFileManagerHashed Struct Layout (~3084 bytes / 0xC0C)

Base fields occupy indices 0-3 (offsets 0x000-0x3FF = 1024 bytes).
Search path strings occupy indices 4-11 (offsets 0x400-0xBFF, 256 bytes each, max 8 paths).
Metadata at 0xC00-0xC0C.

| Offset | Type | Name | Purpose |
|-|-|-|-|
| 0x000 | u32 | vtable | VTable ptr (0x82078D48 for CFileManagerHashed) |
| 0x004 | u32 | field_4 | Init = 0 |
| 0x008 | u16 | field_8 | Init = 0 |
| 0x00A | u16 | field_A | Init = 0 |
| 0x00F | u8 | field_F | Init = 0 |
| 0x010 | obj | inner_object | Inner interface (vtable 0x82078CB4, ctor sub_827ACCE0) |
| 0x010 | f32 | inner_f16 | Float at inner+0 (setter: sub_827ACC38) |
| 0x014 | f32 | inner_f20 | Float at inner+4 (setter: sub_827ACC40) |
| 0x018 | f32 | inner_f24 | Float at inner+8 (setter: sub_827ACC48) |
| 0x0B0 | u32 | path_ptr | String ptr to file path (sub_827ADC88 calls strrchr('/')) |
| 0x0B4 | u32 | param_ptr | Param struct ptr; sub_827ADD80 reads param+112 as float |
| 0x0D4 | u32 | entry_array_ptr | Array of sub-entry ptrs (sub_827ADCD8 iterates) |
| 0x0FC | u32* | hash_keys | Hash key array (sub_827ADDA8 reallocs) |
| 0x100 | u32* | hash_values | Hash value array (sub_827ADDA8 reallocs) |
| 0x1F0 | u8 | hash_count | Current hash entry count |
| 0x1F1 | u8 | hash_capacity | Hash array capacity |
| 0x1F3 | u8 | num_sub_items | Sub-item count (compared to 4 in sub_827ADCD8) |
| 0x1F5 | u8 | loaded_flag | Set from vtable[20] result (sub_827ADE50) |
| 0x2E0 | f32 | stream_rate_0 | Float param (init from constant 0.0) |
| 0x2E4 | f32 | stream_rate_1 | Float param |
| 0x2E8 | f32 | stream_rate_2 | Float param |
| 0x2EC | f32 | stream_rate_3 | Float param |
| 0x2F0 | u32 | thread_pool_ptr | Thread pool (sub_827ACC98 dispatches to sub_827BC040) |
| 0x2F4 | u32 | io_queue_ptr | I/O queue (sub_827ACCA0 dispatches to sub_827F5860) |
| 0x2F8 | u32 | io_thread_count | Init = 2 |
| 0x2FC | u32 | pending_op_id | Init = -1 |
| 0x300 | u32 | max_requests | Init = 1, set to 128 during streaming init |
| 0x304 | u32 | current_op_id | Init = -1 |
| 0x308 | u8 | is_loading | Init = 0 |
| 0x309 | u8 | is_suspended | Init = 0 |
| 0x30A | u8 | needs_flush | Init = 0 |
| 0x30B | u8 | is_shutdown | Init = 0 |
| 0x30C | 16B[8] | hash_data[0..7] | 8 blocks of 16-byte hash/config data |
| 0x38C | 32B | callback_list | Zero-init callback registration block |
| 0x3AC | 16B | mount_data_0 | Mount point config data |
| 0x3BC | 16B | mount_data_1 | Mount point config data |
| 0x3CC | 16B | mount_data_2 | Mount point config data |
| 0x3E0 | obj | hash_map | Hash map object (sub_8279A6D0 ctor, capacity=20) |
| 0x400-0xBFF | char[256][8] | search_paths | Up to 8 search paths, 256 chars each |
| 0xC00 | u32 | search_path_count | Number of registered search paths |
| 0xC04 | u32 | active_search_paths | Active search path count (find loop bound) |
| 0xC08 | u32 | default_path_index | Default search path index |

### Search Path Storage

Paths are stored at `this + (index+4) * 256`:
- Path 0 at `this+0x400` (1024)
- Path 1 at `this+0x500` (1280)
- ...up to 8 paths
- `search_path_count` at `this+0xC00` = 12*256, max index 11, 8 usable slots

`addSearchPath` (sub_8284F310): reads `search_path_count`, uses slot `count+4` for the prefix string, copies the full resolved path to slot `count+4+1`, increments `search_path_count`.

`findFile` (sub_8284F468): iterates `i=0..active_search_paths`, calls `buildSearchPath` with search index `i`, then calls callback.

## Streaming Entry Struct (176 bytes)

Constructor: `sub_8261FBA0`. Base class init: `sub_827BA840`.

| Offset | Type | Name | Purpose |
|-|-|-|-|
| 0x00 | u32 | vtable_main | Main vtable (0x8203FC0C) |
| 0x04 | ... | base_fields | Inherited from sub_827BA840 |
| 0x0C | u32 | resource_ptr | Ptr to resource object (0 = unloaded) |
| 0x50 | u32 | vtable_secondary | Secondary interface vtable (0x8203FBF8) |
| 0xA0 | u32 | state | State enum: 13=UNLOADED, 18=PENDING_INIT |
| 0xA4 | u32 | flags | Flags (init=0) |

## Resource Object Struct (pointed to by entry+0x0C)

| Offset | Type | Name | Purpose |
|-|-|-|-|
| 0x000 | u32 | vtable | Resource vtable ptr |
| 0x024 | u32 | flags | Bit 31 = active/valid flag |
| 0x028 | u32 | state_bits | Bits [22:25] = streaming state (2-4 = loading) |
| 0x038 | u32 | owner_ptr | Ptr to owning CFileManager |
| 0x1BC | u32 | parent_ptr | Parent resource ptr |
| 0x1E2 | u16 | status_flags | Bits [0:3] = status (3 or 7 = READY), bit 8 = pending |
| 0x1F4 | u32 | callback_ptr | Load/unload notification callback ptr |

## Streaming State Machine

### Entry States (at entry+0xA0)

| Value | State | Description |
|-|-|-|
| 13 | UNLOADED | Default state, set by sub_8261FBA0 constructor |
| 18 | PENDING_INIT | Set during streaming system initialization |

### Resource States (at resource+0x28, bits [22:25])

| Range | State | Description |
|-|-|-|
| 0-1 | IDLE/FREE | Not in use |
| 2-4 | LOADING | I/O in progress |
| 5+ | LOADED | Resource fully loaded |

### Resource Status (at resource+0x1E2, bits [0:3])

| Value | Meaning |
|-|-|
| 3 | READY |
| 7 | READY (alternate) |

### Transitions

1. **Load Request**: caller invokes `findFile()` on the manager
2. **Path Resolution**: `buildSearchPath()` iterates search prefixes, concatenates with filename
3. **File Open**: `sub_82855460` opens file handle, vtable[84] reads initial data
4. **I/O Submit**: data sent through `io_queue_ptr` (this+0x2F4) via `sub_827F5860`
5. **Completion**: `onLoadComplete` (sub_827ADE50) fires, sets `loaded_flag` via vtable[20]
6. **Callback Dispatch**: vtable slots 20, 24, 84, 88 handle load/unload events

## All Methods

### CFileManager Base Methods (sub_827AC*)

| Function | Kind | Signature / Purpose |
|-|-|-|
| sub_827ACC38 | setter | `setInnerFloat0(float)` stores at this+0x10 |
| sub_827ACC40 | setter | `setInnerFloat4(float)` stores at this+0x14 |
| sub_827ACC48 | setter | `setInnerFloat8(float)` stores at this+0x18 |
| sub_827ACC50 | dtor | Base interface destructor, sets vtable 0x82078CB4 |
| sub_827ACC98 | getter | `getThreadPool()` returns this+0x2F0, dispatches to sub_827BC040 |
| sub_827ACCA0 | method | `configureIO(size, priority)` reads this+0x2F4, calls sub_827F5860 |
| sub_827ACCA8 | vfunc | Dispatches vtable[236] |
| sub_827ACCB8 | getter | `getInnerObj()` returns this+0x10, calls sub_827F5278 |
| sub_827ACCC0 | vfunc | Dispatches via vtable[0] of argument |
| sub_827ACCE0 | ctor | Inner object constructor at this+0x10 |

### CFileManagerBase Methods (sub_827AD*)

| Function | Kind | Signature / Purpose |
|-|-|-|
| sub_827AD200 | ctor | Full base constructor; inits all fields, allocates thread pool + I/O queue |
| sub_827AD580 | dtor | Destructor; sets vtable to 0x82078D38, frees thread pool |
| sub_827AD9C8 | ctor | CFileManagerBase ctor; calls sub_827AD200, inits hash_map at +0x3E0 |
| sub_827ADAC8 | ctor | CFileManager ctor; wraps sub_827AD200 + thread/resource setup |
| sub_827ADB48 | ctor | CFileManagerHashed ctor; calls sub_827AD9C8, sets vtable 0x82078D48 |
| sub_827ADC38 | method | `setup(this, bool)` — teardown then optional realloc |
| sub_827ADC88 | getter | `getFilename()` — strrchr(this+0xB0, '/') + 1 |
| sub_827ADCD8 | method | `getLoadProgress()` — iterates this+0xD4 array, sums floats from entries |
| sub_827ADD80 | getter | `hasProgress()` — checks this+0xB4 → offset+112 > 0.0 |
| sub_827ADDA8 | method | `resizeHashTable(newCap)` — reallocs arrays at +0xFC/+0x100 |
| sub_827ADE50 | handler | `onLoadComplete(event)` — sets this+0x1F5 from vtable[20] |

### Search Path Methods (sub_8284E*/F*)

| Function | Kind | Signature / Purpose |
|-|-|-|
| sub_8284E690 | method | `isValidPath(this, str)` returns bool |
| sub_8284EAF0 | method | `copyPath(dst, maxLen, src)` bounded string copy |
| sub_8284EEC0 | method | `buildPath(this, buf, 256, name, ext)` no search prefix |
| sub_8284F0C0 | method | `buildSearchPath(this, buf, 256, name, ext, searchIdx)` with prefix |
| sub_8284F310 | method | `addSearchPath(this, pathStr)` adds prefix, increments count at +0xC00 |
| sub_8284F468 | method | `findFile(this, name, ext, ?, callback)` iterates all search paths |
| sub_8284F4E0 | method | `logDefaultPath(this, name, ext)` uses default search idx at +0xC08 |
| sub_8284F538 | method | `resolvePathDirect(this, name, ext)` no search prefix |
| sub_8284F568 | method | `findAllFiles(this, name, ext, callback, userData)` directory enumeration |

## Initialization Sequence (sub_82478AF8)

1. `sub_826225E0/648/6B0` — init streaming subsystems
2. Allocate 176 bytes, construct via `sub_8294BD68`
3. Store module wrapper at `g_streamingModule` (0x82FF5368)
4. Read system memory info, compute streaming budget
5. Set `g_streamingFlags`, `g_streamingMaxReq=168`, `g_streamingCapacity`
6. Allocate CFileManagerHashed via `sub_82478A80`, construct via `sub_827ADB48`
7. Allocate entry array: `count * 176 + 16` bytes
8. Init each entry via `sub_8261FBA0` in loop (stride 176)
9. Set entry state = 18 (`PENDING_INIT`), build sorted pointer array
10. Store array at `g_streamingArray` (0x82FF536C)
11. `sub_827ACC98` — set up thread pool (this+0x2F0)
12. Set `max_requests = 128` at module+0x300
13. `sub_827ACCA0` x3 — configure I/O queues (8192/200, 4096/300, 2048/500)
14. Register 16+ resource types (stride-36 type registration entries at 0x82AF85xx)
15. `sub_8261C7C8` — streaming event system init
16. Configure streaming rates and budget limits

## Callers of g_streamingModule

The streaming module global is loaded via `lis r11,-32001; lwz r11,21352(r11)` and used as a null-check guard before dispatching vtable[164] on resource objects. Key callers:
- `sub_823B25xx` — resource unload path (vtable[164] = unload handler)
- `sub_8263F1xx` — scene/world resource dispatch
- `sub_82300Exx` — generic resource dispatch
- `sub_82476Dxx` — streaming update tick (calls sub_827ACCB8 for inner obj)

## Native Rewrite Requirements

1. **Streaming manager struct**: ~3KB with 8 search paths (256 chars each)
2. **Search path system**: `addPath` / `findFile` with prefix concatenation
3. **Hash map** at +0x3E0 for name-to-resource lookup (sub_8279A6D0)
4. **Thread pool** at +0x2F0 and **I/O queue** at +0x2F4
5. **Entry array**: N entries x 176 bytes with state machine
6. **Resource state tracking**: loading/loaded/unloaded transitions via bitfield states
7. **Callback system**: vtable dispatch for load/unload events (slots 20, 24, 84, 88, 164, 236)
8. **16+ resource type registrations** with priority and size configuration
9. **Memory budget**: respects system memory limits set during init
10. **Globals**: `g_streamingModule` and `g_streamingArray` must remain accessible for legacy code paths that null-check before dispatching
