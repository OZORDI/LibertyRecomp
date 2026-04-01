# GTA IV Streaming Manager API Surface

## Source
All functions in `gta4_recomp.55.cpp` and `gta4_recomp.56.cpp` (generated/).

## Global State
- **Streaming device instance**: `[0x831E55EC]` (pointer loaded via `lis -31970 / lwz 21996`)
- **Device name string**: `0x82B07278` (used by sub_82852DD0 as r3 arg to sub_8284F468)

## Streaming Manager Object Layout (this = r3 in most calls)

| Offset | Size | Field | Notes |
|-|-|-|-|
| +0x000..0xBFF | | Path slot array | Each slot = 256 bytes (0x100), indexed by active_stream_count |
| +0xC00 (3072) | u32 | active_stream_count | Incremented by AddSearchPath, decremented by FinalizeStream |
| +0xC04 (3076) | u32 | num_search_paths | Upper bound for FindResource loop |
| +0xC08 (3080) | u32 | default_search_path_index | Used by OpenResourceDefault |

Slot calculation: `slot_addr = this + (index << 8)` where index is from `[this+0xC00]`.

## Core API Functions

### sub_8284E830 — `StreamingManager::FinalizeStream(this)`
- **Body**: `this->active_stream_count--` (3 instructions, leaf function)
- **No events signaled. No semaphores released. No locks touched.**
- Just a reference count decrement on the path slot counter
- Called after sub_82852DD0 returns in sub_827C2420

### sub_8284F310 — `StreamingManager::AddSearchPath(this, path)`
- Checks if path starts with `$` (ASCII 36) — if so, uses raw path; otherwise:
  - Calls sub_8284E690 to validate the device
  - Reads `active_stream_count` at +0xC00
  - If count > 0: computes slot address as `(count+3)<<8 + this`
  - If count == 0: uses default path at `0x820B92FC`
  - Copies path into 256-byte stack buffer, null-terminates
  - Calls sub_82211840 (some path resolution/registration)
  - Copies resolved path into slot via sub_8284EAF0
- **Increments** `this->active_stream_count` at +0xC00
- Inverse of sub_8284E830

### sub_8284F468 — `StreamingManager::FindResource(this, name, type, callback)`
- r3=this, r4=name, r5=type, r7=callback
- Loops `i = 0..num_search_paths` (+0xC04):
  - Calls sub_8284F0C0 to build full resource path (slot_buf, 256, this, name, type, i)
  - Calls sub_8285AA68(path, callback) to attempt open
  - Returns first non-null result; exits loop early on success
- Returns r3 = resource handle or 0

### sub_8284FA58 — `StreamingManager::ReleaseResourceBundle(bundle)`
- r3 = resource bundle pointer (struct below)
- Iterates `handle_array[0..handle_count]`, calling sub_821B3560 (free) on each
- If `main_buffer != 0`: calls sub_82863628 then sub_821B3560 (teardown + free)
- If `array_allocated != 0`: frees the handle array itself
- **Pure cleanup — no synchronization primitives**

### sub_8284FAD8 — `StreamingManager::GrowResourceBuffer(this, src, size)`
- r3=this, r4=src_data, r5=additional_size
- Reads current buffer state from `[this+12]+32..44`
- If current allocation < needed: doubles allocation, calls sub_8284F968 (alloc new buffer), copies old data via sub_82A00DC0, swaps via sub_8284F9C0
- Writes new data at `old_buffer + old_offset` via sub_82A00DC0
- Manages a growing buffer for accumulating streamed resource data

### sub_8284FC98 — `StreamingManager::CreateResourceHandles(this)`
- Allocates a 20-byte resource bundle (sub_821B3510)
- Initializes it to zeros (same layout as sub_8284FA38)
- Calls sub_8284D220 twice to create two packfile handles
- Stores handles at `[this+12]` and `[this+28]`
- Returns the resource bundle

## Resource Bundle Struct (20 bytes)

| Offset | Size | Field |
|-|-|-|
| +0 | u32 | main_buffer_ptr |
| +4 | u32 | handle_array_ptr |
| +8 | u16 | handle_count |
| +10 | u16 | array_allocated |
| +12 | u32 | extra_ptr_a |
| +16 | u32 | extra_ptr_b |

## Higher-Level Functions

### sub_82852DD0 — `StreamDevice::OpenAndProcess(device, name, offset, callback, userData)`
- r3=device, r4=name(unused), r5=path, r6=offset, r7=callback, r8=userData
- Calls `FindResource(device_name, path, 0, 1)` via sub_8284F468
- If found: calls **sub_82852D18** to process the resource, then sub_8285B088 to release the handle
- Returns result of sub_82852D18 or 0

### sub_82852D18 — `StreamDevice::ProcessResource(device, handle, offset_ptr, callback)`
- **This is where the hang occurs (never returns on 4th call)**
- Calls sub_82852A50 to **locate and load** the resource
- If resource found:
  - Calls sub_82851A10 to register it in the device's map (at +24)
  - Calls vtable[8] on the callback object (indirect call — the actual processing)
- Then: optionally takes allocator lock (sub_828470E0), calls sub_8284FA58 (release bundle), calls sub_821B3560 (free), releases lock
- Returns 1 on success, 0 if sub_82852A50 returned null

### sub_82852A50 — `StreamDevice::LocateResource(device, name)`
- Takes allocator lock (sub_828470E0)
- Calls sub_82852300 to search device index
- If index found: calls sub_82851DF0 to get resource descriptor
- Uses vtable dispatch to load/create the resource object
- Calls sub_8284FC98 to create resource handles
- Calls vtable[0] (destructor/release) on intermediate object
- Releases lock
- **Contains multiple vtable indirect calls — any could block if resource I/O hangs**

## Call Flow: sub_827C2420 (the observed sequence)

```
sub_827C2420(this):
  device = 0x82B07278
  path = this->filename  // [this+56]

  StreamingManager::AddSearchPath(device, path)     // sub_8284F310 — increments refcount
  globalDevice = [0x831E55EC]

  // vtable call on this->vtable[4] to get resource name
  name = this->vtable->GetName(this)

  StreamDevice::OpenAndProcess(globalDevice, ...)    // sub_82852DD0 — THIS HANGS
    └─ FindResource(...)                             // sub_8284F468
    └─ ProcessResource(...)                          // sub_82852D18
       └─ LocateResource(...)                        // sub_82852A50 ← likely hang point
          └─ vtable indirect calls for I/O

  StreamingManager::FinalizeStream(device)           // sub_8284E830 — decrements refcount
```

## Key Finding: sub_8284E830

**sub_8284E830 does NOT signal any events or release any semaphores.** It is a trivial 3-instruction leaf function that decrements `[r3+0xC00]`. It has no bearing on synchronization. The hang is upstream in sub_82852A50's vtable dispatch calls during resource I/O, not in any event/semaphore mechanism visible at this API level.

## Allocator Context Functions

| Function | Role |
|-|-|
| sub_828470E0 | Push allocator context: saves TLS[1676]→TLS[1672], loads TLS[1680]→TLS[1676], increments TLS[1668] |
| sub_82847120 | Pop allocator context: decrements TLS[1668], restores if zero |
| sub_821B3510 | Allocate: reads allocator from TLS, calls vtable[8] (malloc) |
| sub_821B3560 | Free: reads allocator from TLS, calls vtable[12] (free) |

## Helper Functions

| Function | Role |
|-|-|
| sub_8284E840 | Parse protocol prefix from path (checks "device:" prefix via strncmp 7, finds filename after last `/` or `\`) |
| sub_8284E910 | Copy string up to `.` or limit (basename extraction) |
| sub_8284E960 | Find file extension position in path |
| sub_8284E9E0 | Append suffix to path (strrchr `/`, insert before extension) |
| sub_8284EAF0 | Copy resolved path into slot buffer |
| sub_8284F0C0 | Build full resource path from slot + name + type + index |
| sub_8284F4E0 | OpenResourceDefault: uses default_search_path_index (+0xC08) |
| sub_8284F538 | OpenResourceSimple: calls sub_8284EEC0 |
| sub_8284F568 | OpenResourceMulti: iterates search paths like FindResource but with different callback |
| sub_8284F968 | Alloc buffer: if size>0 calls sub_821B3510, stores ptr and size |
| sub_8284F9C0 | Swap buffer: move-assigns one buffer struct to another, zeros source |
| sub_8284FA38 | Init resource bundle: zeros all 20 bytes |
| sub_8285AA68 | TryOpenFile: calls sub_82855460 to get device, vtable[4] dispatch to open |
| sub_8285AB78 | LogOpenResult: debug logging |
| sub_8285B088 | ReleaseHandle: checks state fields, vtable[40] dispatch to close, sets handle to {0, -1} |
| sub_82851A10 | RegisterInMap: strips `__` prefix, calls sub_82912948 on device's map (+24) |
