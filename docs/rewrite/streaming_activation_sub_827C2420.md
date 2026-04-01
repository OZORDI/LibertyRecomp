# Streaming Activation: sub_827C2420

## Function Purpose

`sub_827C2420` is the streaming resource activation function called during engine init from `sub_82478AF8` (the "tail" init function). It registers a streaming resource by:

1. Adding a search path to the streaming module
2. Calling a virtual method on the resource object to get its handle
3. Opening and processing the resource via the streaming manager
4. Removing the search path

Called at address `0x82478EAC`. Immediately after this function returns, the caller invokes `sub_8284E830` (end search paths) and then attempts the 1024-byte allocation via `sub_821B3510` -- connecting the streaming system directly to the allocation hang point.

## Signature

```
void sub_827C2420(PPCContext& ctx, uint8_t* base)
// r3 = this (resource object pointer)
//   this->vtable at offset 0
//   this->field_56 at offset 56 (path string pointer)
```

## Key Globals

|Address|Description|
|-|-|
|0x82B07278|Static streaming module (ragePath manager)|
|0x831E55EC|Resource manager pointer|
|0x831AB828|Streaming callback linked list head|
|0x82B08040|File handle array (VFS mount table)|
|0x82B08150|File state tracking struct|
|0x831AB948|File open filter callback pointer|
|0x831AB810|Logging/debug output ptr|
|0x831AB814|Alternate search paths ptr|

## Streaming Module Layout (at 0x82B07278)

```
+0x000 to +0xBFF  Path buffer storage (12 slots x 256 bytes each)
+0xC00 (3072)     search_path_count (uint32_t) - incremented by AddSearchPath
+0xC04 (3076)     active_search_paths count (uint32_t) - used by FindFile loop
+0xC08 (3080)     current_index (uint32_t)
```

Path slot addressing: `slot_base = streaming_module + (index << 8)` with additional +4 offset for the data portion.

## Complete Call Tree

```
sub_827C2420(this)
  |
  +-- sub_8284F310(0x82B07278, this->field_56)    [AddSearchPath]
  |     +-- sub_8284E690(this->field_56)           [isRelativePath]
  |     |     +-- rexcrt_strchr(path, ':')
  |     +-- sub_82211840(buf, path, 256)           [stringAppend]
  |     +-- sub_8284EAF0(dst, 256, src)            [copyPathNormalize]
  |     +-- increments streaming_module+0xC00
  |
  +-- vtable[1](this)                              [getResourceHandle - indirect call]
  |     reads vtable at this+0, calls slot 4 (offset +4)
  |
  +-- sub_82852DD0(resMgr, str1, str2, handle, this, 1)  [OpenAndProcess]
  |     |
  |     +-- sub_8284F468(0x82B07278, str1, str2, 0, this, 1, 0)  [FindFile]
  |     |     |
  |     |     +-- loop: for i in 0..search_path_count:
  |     |           +-- sub_8284F0C0(mod, buf, 256, name, prefix, empty, i)  [BuildFullPath]
  |     |           |     +-- sub_8284E690(prefix)     [isRelativePath]
  |     |           |     +-- copies search path + name into buffer
  |     |           |
  |     |           +-- sub_8285AA68(buf, this)        [TryOpenFile] *** BLOCKING ***
  |     |                 +-- sub_82855460(buf)         [GetFSHandler]
  |     |                 |     +-- rexcrt_strncmp x 7  [protocol matching]
  |     |                 |     returns handler for device:/game:/update:/platform:/cd:
  |     |                 |
  |     |                 +-- handler->vtable[4](handler, buf, this)  [Open] *** FILE I/O ***
  |     |                 +-- sub_822BCA90(...)         [log printf]
  |     |                 +-- filter callback check via 0x831AB948
  |     |                 +-- sub_8285A968(buf, idx, handler)  [AddToRegistry]
  |     |
  |     +-- if found:
  |           +-- sub_82852D18(resMgr, handle, resObj, this, 1)  [ProcessResource]
  |           |     +-- sub_82852A50(resMgr, handle)   [FindExistingResource]
  |           |     |     +-- sub_828470E0()            [TLS lock enter]
  |           |     |     +-- sub_82852300(resMgr, handle, &idx, &slot)  [HashLookup]
  |           |     |     |     +-- sub_8285AD08()      [hash key compute]
  |           |     |     |     +-- sub_828D0608()      [hash table begin-iter]
  |           |     |     |     +-- sub_82851C40()      [hash table next-iter]
  |           |     |     +-- sub_82851DF0()            [GetResourceHandler]
  |           |     |     +-- handler->vtable[12](handler, handle)  [Lookup indirect]
  |           |     |     +-- obj->vtable[4](obj, slot)  [SetSlot indirect]
  |           |     |     +-- sub_8284FC98(obj)         [GetFileInfo]
  |           |     |     +-- obj->vtable[0](obj, 1)   [AddRef indirect]
  |           |     |     +-- sub_82847120()            [TLS lock leave]
  |           |     |
  |           |     +-- sub_828470E0()                  [TLS lock enter]
  |           |     +-- resObj->vtable[8](resObj, handle, this)  [Activate indirect] *** BLOCKING ***
  |           |     +-- sub_8284FA58(result)            [FreeOldResources]
  |           |     |     +-- sub_821B3560() x N        [free memory]
  |           |     |     +-- sub_82863628()            [release object]
  |           |     +-- sub_821B3560(result)            [free allocation]
  |           |     +-- sub_82847120()                  [TLS lock leave]
  |           |
  |           +-- sub_8285B088(fileRef)                [FlushFileRef]
  |                 +-- sub_8285A8B0()                  [sync if pending]
  |                 +-- fileRef->vtable[40](obj, idx)  [Release indirect]
  |                 +-- clears fileRef fields to -1/0
  |
  +-- sub_8284E830(0x82B07278)                        [EndSearchPaths]
        decrements streaming_module+0xC00
```

## TLS Lock System (sub_828470E0 / sub_82847120)

These are NOT mutexes. They are a TLS-based lock ordering validator (debug assertion system):

```
Thread block accessed via: [r13+0] -> thread_data
  +1668  recursion_count (uint32_t)
  +1672  saved_lock_id (uint32_t)
  +1676  current_lock_id (uint32_t)
  +1680  target_lock_id (uint32_t)
```

**Enter (sub_828470E0):**
- If `TLS[1676] == TLS[1680]`: increment recursion_count (nested re-entry)
- Else: save current to TLS[1672], write target to TLS[1676]

**Leave (sub_82847120):**
- If recursion_count > 0: decrement
- Else: restore TLS[1672] -> TLS[1676], clear TLS[1672]

These cannot deadlock independently. They only validate lock ordering.

## File System Protocol Handlers

`sub_82855460` dispatches by path prefix using `strncmp`:

|Prefix|Handler Address|
|-|-|
|`device:` (7)|0x82B08004|
|`platform:/` (10)|0x82B08004 (same)|
|`game:` (6)|0x82B07EF0 or via sub_828708C8|
|`data:` (6)|0x82B07EF0|
|`update:` (7)|0x82B07EF0|
|`cd:` (5)|0x82B07EF0|

String constants at 0x82085480, 0x8208599C, 0x82085A18, 0x82085A10, 0x82085A08, 0x82085A00, 0x820859F8.

## File I/O Path

The actual file I/O occurs in `sub_8285AA68` when it calls `handler->vtable[4]` (the Open method):

1. `sub_82855460` resolves the protocol handler from the path prefix
2. The handler's vtable slot 4 is the `Open(handler, path, context)` method
3. If Open returns != -1, the file is considered found
4. The file handle is logged via `sub_822BCA90`
5. A filter callback at `0x831AB948` can reject the file
6. If accepted, `sub_8285A968` adds the file to the active registry

## Thread and Synchronization Analysis

**No real mutexes in this call path.** The TLS lock enter/leave (sub_828470E0/sub_82847120) is a debug-only lock ordering checker, not a blocking primitive.

The function does NOT:
- Create or signal threads
- Acquire CRITICAL_SECTION or mutex objects
- Call WaitForSingleObject or similar
- Use semaphores

The function DOES:
- Perform file I/O through VFS handler vtable calls
- Allocate memory (indirectly through resource handler activation)
- Free memory via sub_821B3560 (operator delete)
- Iterate callback linked lists (sub_82851918 path)

## Why This Function Can Hang

### Primary Hang Path: VFS Open Failure

```
sub_827C2420
  -> sub_82852DD0
    -> sub_8284F468 (FindFile loop)
      -> sub_8285AA68
        -> sub_82855460 returns handler
        -> handler->vtable[4] = OPEN
```

If the VFS handler's Open method:
- References an unmounted device (device path not in HostPathDevice)
- Triggers a blocking I/O completion wait (OVERLAPPED with no signal)
- Causes a fallback allocation via sub_8218BE28 (TLS allocator context not set)

Then the streaming activation will stall.

### Secondary Hang Path: Resource Activation Vtable

```
sub_827C2420
  -> sub_82852DD0
    -> sub_82852D18
      -> resObj->vtable[8] (Activate)
```

The vtable[8] call on the resource object is an indirect call that could invoke any registered resource handler. If this handler:
- Needs GPU resources that aren't initialized
- Requires another streaming resource that creates circular dependency
- Calls back into the allocator

Then this is another potential hang point.

### Connection to 1024-byte Allocation

The caller sequence is:
```
0x82478EAC: bl sub_827C2420    // streaming activation
0x82478EB4: bl sub_8284E830    // end search paths
0x82478EC0: li r3, 1024        // allocate 1024 bytes
0x82478EC4: bl sub_821B3510    // operator new(1024)
```

If streaming activation causes heap corruption or TLS allocator confusion, the immediately-following 1024-byte allocation will encounter a corrupted heap state. Both paths go through `sub_8218BE28` (game malloc) which reads `TLS[r13+1676]` for allocator context.

### Allocator Interaction Detail

Inside the streaming path, `sub_82852D18` calls:
- `sub_821B3560` (operator delete) -- frees memory through the game allocator
- `sub_8284FA58` -> `sub_821B3560` multiple times (cleanup old resources)
- `sub_8285A1B0` -> `sub_821B8240` (allocate for file handle list)

All these allocations go through the game's TLS-based allocator system. If TLS[r13+1676] is not properly set up for the calling thread, every allocation falls through to the host page allocator, creating the "ALLOC FALLBACK storm" pattern previously observed with particle emitters.

## Global State Modifications Summary

1. `0x82B07278+0xC00` (search_path_count): incremented then decremented (balanced)
2. `0x82B07278+0xC04` (active_search_paths): may be modified during FindFile
3. Resource manager's internal hash tables: modified during ProcessResource
4. `0x831AB828` (callback list): iterated and cleared during finalize
5. File handle registry: new entry added via sub_8285A968
6. TLS lock state: modified and restored (balanced)
7. Resource object fields: handle stored, vtable calls modify internal state
