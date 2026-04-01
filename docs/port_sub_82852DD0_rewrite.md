# Port Documentation: sub_82852DD0 — OpenAndProcess Streaming Resource Loader

## Overview

`sub_82852DD0` is the top-level "open file, parse resource, invoke callback" entry point for
GTA IV's RAGE streaming resource system. It finds a resource across all mounted devices,
reads/parses it, dispatches a callback, then cleans up. The flow is:

```
sub_8284F468 (find & open resource across devices)
  -> sub_82852D18 (read, parse, dispatch callback)
    -> sub_82852A50 (read stream into parsed resource struct)
    -> sub_82851A10 (register resource type handler)
    -> callback->vtable[2](callback, resource, arg) (process)
    -> sub_8285B088 (flush dirty buffers + close file handle)
    -> sub_8284FA58 (free resource slot arrays & memory)
```

---

## Global Addresses

| Name | Address | Description |
|-|-|-|
| g_streamingMgr | 0x82B07278 | Singleton fiPackfile/streaming manager |
| g_debugFlags | 0x831E55EC | Ptr to debug state; word at +40, bit 17 = streaming lock flag |
| g_streamingLock | 0x831AB970 | CriticalSection for stream slot allocation |
| g_slotArray | 0x831B5DD8 | Array of 28-byte stream slot structs |
| g_slotBufferBase | 0x831B5F28 | 4KB-aligned buffer pool base |
| g_maxSlots | 0x82B093A8 | int32: max number of stream slots |
| g_highWaterSlot | 0x82B081AC | int32: highest slot index ever allocated |

---

## Stream Slot Struct (28 bytes)

Located at `g_slotArray + slotIndex * 28`:

| Offset | Size | Type | Name | Description |
|-|-|-|-|-|
| +0 | 4 | fiDevice* | device | Filesystem device that owns this handle |
| +4 | 4 | int32 | fileHandle | Handle returned by device->Open(); -1 = closed |
| +8 | 4 | void* | buffer | 4KB-aligned read buffer (at g_slotBufferBase + slotIndex * 4096) |
| +12 | 4 | uint32 | fileOffset | Current absolute file position |
| +16 | 4 | uint32 | bytesBuffered | Bytes currently in buffer (valid read data) |
| +20 | 4 | uint32 | dirtyBytes | Bytes written to buffer not yet flushed |
| +24 | 4 | uint32 | bufferCapacity | Buffer size in bytes (default 4096) |

---

## Stream Buffer Struct (used by sub_8285A8B0 / sub_8285B088)

This is a higher-level stream handle (the 24-byte struct at the head of the slot):

| Offset | Size | Type | Name | Description |
|-|-|-|-|-|
| +0 | 4 | fiDevice* | device | Vtable-bearing device object |
| +4 | 4 | int32 | fileHandle | Device file handle; set to -1 on close |
| +8 | 4 | void* | buffer | Read/write buffer |
| +12 | 4 | uint32 | fileOffset | Current absolute offset |
| +16 | 4 | uint32 | dirtyStart | Start of dirty region (unflushed writes) |
| +20 | 4 | uint32 | dirtyEnd | End of dirty region; 0 if clean |

---

## Function: sub_82852DD0 — OpenAndProcess

### Signature
```
bool OpenAndProcess(
    void*  this,           // r3 — resource handler / pgStreamer object
    char*  devicePath,     // r4 — device-relative path (e.g. "update:/common/data/...")
    char*  filename,       // r5 — filename or pattern to search
    void*  callbackObj,    // r6 — object with vtable for Process callback
    uint32 callbackArg1,   // r7 — passed through to callback
    uint32 callbackArg2    // r8 — passed through to callback
);
// Returns: r3 = result from sub_82852D18 (1 = success, 0 = fail)
```

### Pseudocode
```cpp
bool fiStreamer::OpenAndProcess(void* handler, const char* devPath, const char* filename,
                                IResourceCallback* callback, uint32_t arg1, uint32_t arg2)
{
    // Step 1: Find & open the resource file across all mounted devices
    StreamSlot* slot = g_streamingMgr->FindAndOpenResource(devPath, filename,
                                                            /*isRelative=*/false,
                                                            /*syncOpen=*/true);
    if (!slot)
        return false;  // file not found on any device

    // Step 2: Read, parse, and dispatch callback
    bool result = ProcessOpenedResource(handler, slot, callback, arg1, arg2);

    // Step 3: Flush & close the stream slot
    slot->FlushAndClose();

    return result;
}
```

---

## Function: sub_8284F468 — FindAndOpenResource

### Signature
```
StreamSlot* FindAndOpenResource(
    void*    streamMgr,    // r3 — g_streamingMgr (0x82B07278)
    char*    devPath,      // r4 — device path
    char*    filename,     // r5 — filename
    bool     isRelative,   // r6 — 0 = absolute, 1 = relative
    bool     syncOpen      // r7 — 1 = synchronous open
);
// Returns: r3 = pointer to stream handle path buffer, or 0 if not found
```

### Pseudocode
```cpp
StreamSlot* fiStreamingMgr::FindAndOpenResource(const char* devPath, const char* filename,
                                                 bool isRelative, bool syncOpen)
{
    int deviceCount = this->deviceCount;  // at this+3076
    StreamSlot* result = nullptr;

    for (int i = 0; i < deviceCount && result == nullptr; i++) {
        char pathBuf[256];
        // sub_8284F0C0: Build full path = devicePrefix[i] + filename
        BuildFullPath(this, pathBuf, 256, filename, isRelative, /*deviceIndex=*/i);

        // sub_8285AA68: Try to open the path on the filesystem
        result = TryOpenFile(pathBuf, syncOpen);
    }
    return result;
}
```

### sub_8284F0C0 — BuildFullPath

Resolves a filename into a full device path:

1. If `filename` is NULL, treat as relative path
2. If `filename[0]` is '/' or '\\', or contains ':', treat as absolute (skip device prefix)
3. For relative paths: copy `devicePrefixTable[deviceIndex]` into pathBuf, then append filename
4. If `filename[0] == '$'`, skip 2 chars (strip `$:` or `$/` prefix)
5. If `streamMgr+3072` (altPrefix) is set, try that path first via `sub_8284E690` (path exists check)
6. Normalize separators via `sub_8284E700` (replaces forward/backslash)
7. Call `sub_8284ED70` to finalize the path into the output buffer

### sub_8285AA68 — TryOpenFile

1. Call `sub_82855460(pathBuf)` to resolve which `fiDevice*` handles this path
2. If no device found, return NULL
3. Call `device->vtable[1](device, pathBuf, syncOpen)` to open file (vtable slot 1 = Open)
4. If open returns -1 (fail), log error and return NULL
5. If a filter callback exists at `0x831AB958`, call it; if it rejects, close via `device->vtable[10]`
6. Otherwise call `sub_8285A968(pathBuf, fileHandle, device)` to allocate a stream slot
7. Return the allocated `StreamSlot*`

### sub_82855460 — GetDeviceForPath

Resolves a file path to the appropriate `fiDevice*` by prefix matching:

| Prefix | Length | Device Address | Likely Name |
|-|-|-|-|
| "update:" | 7 | 0x82B08004 | fiDeviceUpdate |
| "platform:" | 10 | 0x82B08004 | fiDeviceUpdate (same) |
| "cd:" | 6 | sub_828708C8() | fiDeviceCdrom |
| "game:" | 6 | 0x82B07EF0 | fiDeviceLocal |
| "local:" | 7 | 0x82B07EF0 | fiDeviceLocal |
| "data:" | 5 | 0x82B07EF0 | fiDeviceLocal |
| "x:" | 4 | 0x82B07EF0 | fiDeviceLocal |
| "d:" (+colon check) | 3 | 0x82B07EF0 | fiDeviceLocal |

If none match, iterates a registered device table at `0x831AB940` to find a matching mount point.

### sub_8285A968 — AllocateStreamSlot

1. Lock `g_streamingLock`
2. Scan `g_slotArray` (up to `g_maxSlots`) for first slot with `device == NULL`
3. If all slots occupied, log error "fiStream: ran out of stream handles", return NULL
4. Initialize slot:
   - `slot[0] = device`
   - `slot[4] = fileHandle`
   - `slot[8] = g_slotBufferBase + slotIndex * 4096` (4KB buffer)
   - `slot[12] = 0` (fileOffset)
   - `slot[16] = 0` (bytesBuffered)
   - `slot[20] = 0` (dirtyBytes)
   - `slot[24] = 4096` (bufferCapacity)
5. Update `g_highWaterSlot` if this index exceeds it
6. Unlock and return slot pointer

---

## Function: sub_82852D18 — ProcessOpenedResource

### Signature
```
bool ProcessOpenedResource(
    void*       handler,       // r3 — resource handler (this)
    StreamSlot* streamHandle,  // r4 — from FindAndOpenResource
    void*       callbackObj,   // r5 — IResourceCallback*
    uint32_t    callbackArg,   // r6 — user argument
    uint32_t    unused         // r7 — (not visibly used beyond this level)
);
// Returns: 1 if resource loaded & callback dispatched, 0 on failure
```

### Pseudocode
```cpp
bool pgStreamer::ProcessOpenedResource(StreamSlot* stream, IResourceCallback* callback,
                                       uint32_t callbackArg)
{
    // Step 1: Read stream data and parse into resource structure
    // sub_82852A50 reads the file contents, parses headers, builds resource graph
    ResourceData* resData = ReadAndParseStream(this, stream);

    if (!resData)
        return 0;

    void* resPtr = resData->ptr;  // [resData+0]
    bool result;

    if (!resPtr) {
        result = false;
    } else {
        // Step 2: Register type handler for this resource
        // sub_82851A10 looks up resource name hash in handler's type registry (this+24)
        uint32_t nameHash = *(uint32_t*)(resPtr);
        RegisterTypeHandler(this, nameHash);

        // Step 3: Invoke the process callback
        // Virtual call: callback->vtable[2](callback, resPtr, callbackArg)
        callback->Process(resPtr, callbackArg);
        result = true;
    }

    // Step 4: Check debug streaming lock flag
    bool debugLocked = (*(uint32_t*)((*g_debugFlags) + 40) >> 17) & 1;
    if (debugLocked)
        AcquireStreamingDebugLock();  // sub_828470E0

    // Step 5: Free resource slot data
    FreeResourceSlotArrays(resData);  // sub_8284FA58
    FreeMemory(resData);              // sub_821B3560

    if (debugLocked)
        ReleaseStreamingDebugLock();  // sub_82847120

    return result;
}
```

### sub_82852A50 — ReadAndParseStream

1. Check debug streaming lock flag; acquire if set (`sub_828470E0`)
2. Call `sub_82852300(this, stream, &outIndex, &outSecondary)` to read 4-byte header from stream
   - Uses `sub_8285AD08` (stream read) to read 4 bytes into `outIndex`
   - If read < 4 bytes, set `outIndex = -1` (fail)
   - Otherwise iterates an internal collection via `sub_828D0608` (begin iterator), `sub_82851C40` (next)
   - For each entry: if `entry[0] != 1`, call `entry->vtable[3](entry, outIndex)` to check if this
     entry type handles this resource format
   - If handler found, set `outIndex = entry[0]`; if no handler, set `outIndex = 1` (default)
3. If `outIndex == -1`, return NULL (unrecognized format)
4. Call `sub_82851DF0(this, &outIndex)` to look up resource entry in hash table:
   - Hash table at `this+0`: bucket array pointer
   - Hash table at `this+4`: bucket count (uint16)
   - `bucket = outIndex % bucketCount`
   - Walk chain at `buckets[bucket]` comparing `entry[0]` to target; chain link at `entry+56`
   - Returns `entry+4` if found, NULL if not
5. If entry found, get the `pgBase` handler at `entry+16`:
   - Call `handler->vtable[3](handler, stream)` to get resource object
   - If resource obtained:
     - Call `resource->vtable[1](resource, outSecondary)` to set secondary index
     - Call `sub_8284FC98(resource)` to build the resource dependency graph (3 passes using `sub_8284D220`)
     - Call `resource->vtable[0](resource, 1)` to finalize/lock
   - If resource is NULL, try alternate handler at `entry+32`:
     - Call `altHandler->vtable[3](altHandler, stream, outSecondary)` to load
6. Release debug lock if held
7. Return resource object

### sub_8284FC98 — BuildResourceDependencyGraph

Allocates a 20-byte `ResourceInfo` struct, then makes 3 passes with `sub_8284D220`:
- Pass 1: Build primary resource chunk list (vtable from 0x82847C00)
- Pass 2: Build secondary chunk list (vtable from 0x82847800)
- Pass 3: Build tertiary chunk list (vtable from 0x82847AD8)

Copies results into `resource+12`, `resource+28`, `resource+44`, and sets `resource+64 = resInfo+4`.

### ResourceInfo struct (20 bytes, allocated in sub_8284FC98)
| Offset | Size | Type | Description |
|-|-|-|-|
| +0 | 4 | void* | Primary resource ptr |
| +4 | 4 | void* | Sub-resource array |
| +8 | 2 | uint16 | Sub-resource count |
| +10 | 2 | uint16 | Owns-array flag |
| +12 | 4 | void* | Extra data ptr |
| +16 | 4 | void* | Extra data 2 |

---

## Function: sub_8285B088 — FlushAndClose

### Signature
```
void StreamSlot::FlushAndClose();  // r3 = this (StreamSlot*)
```

### Pseudocode
```cpp
void StreamSlot::FlushAndClose()
{
    // If there are unflushed dirty bytes, write them out first
    if (this->dirtyEnd == 0 && this->dirtyStart != 0) {
        // sub_8285A8B0: Flush dirty buffer to device
        this->FlushDirtyBuffer();
    }

    // Close the file handle via device vtable
    // device->vtable[10](device, fileHandle) — vtable slot 10 = Close
    this->device->Close(this->fileHandle);

    // Reset slot
    this->fileHandle = -1;  // mark as closed
    this->device = NULL;    // mark slot as free
}
```

### sub_8285A8B0 — FlushDirtyBuffer

1. If `dirtyEnd != 0`:
   - If `dirtyEnd != dirtyStart`: partial flush via `device->vtable[9](device, handle, buffer + dirtyStart, 0)` (Write with seek)
   - If `dirtyEnd == dirtyStart`: full flush via `sub_82854C80(device, handle, buffer, dirtyStart)`
2. After flush: update `fileOffset += dirtyStart`, reset `dirtyEnd = 0`, `dirtyStart = 0`
3. Call `device->vtable[13](device, handle)` to sync/commit

---

## Function: sub_8284FA58 — FreeResourceSlotArrays

### Signature
```
void FreeResourceSlotArrays(ResourceData* resData);  // r3 = resData
```

### Pseudocode
```cpp
void FreeResourceSlotArrays(ResourceData* resData)
{
    // Free all sub-resource pointers
    uint16_t subCount = resData->subResourceCount;  // at +8 (uint16)
    uint32_t* subArray = resData->subResourceArray;  // at +4

    for (int i = 0; i < subCount; i++) {
        FreeMemory(subArray[i]);  // sub_821B3560 — allocator->Free(ptr)
    }

    // Free the main resource and unlink from parent chain
    void* mainRes = resData->mainResource;  // at +0
    if (mainRes) {
        UnlinkResource(mainRes);   // sub_82863628 — remove from linked list
        FreeMemory(mainRes);       // sub_821B3560
    }

    // Free the sub-resource array itself if it was separately allocated
    uint16_t ownsArray = resData->ownsArrayFlag;  // at +10 (uint16)
    if (ownsArray) {
        FreeMemory(resData->subResourceArray);  // at +4
    }
}
```

### sub_82863628 — UnlinkResource

Removes a resource node from a doubly-linked sibling chain:
1. If `node->parent` (+20) is set, walk the parent's child list (+28) to find this node
2. Replace this node with `node->next` (+24) in the parent's child list
3. Clear `node->parent` and `node->next`
4. Recursively free all children: while `node->firstChild` (+28) is non-null, call `UnlinkResource(child)` then `FreeMemory(child)`
5. If `node->ownedBuffer` (+40) is non-null, free `node->data` (+32)
6. Call `sub_82850678(node)` to finalize cleanup

---

## Function: sub_821B3560 — FreeMemory (Allocator Dispatch)

### Pseudocode
```cpp
void FreeMemory(void* ptr)
{
    if (!ptr) return;

    // Get per-thread allocator from TLS slot 1676
    // r13 = thread-local storage base
    void* tls = *(void**)(r13);
    Allocator* alloc = *(Allocator**)((char*)tls + 1676);

    // Virtual call: alloc->vtable[3](alloc, ptr) — slot 3 = Free
    alloc->Free(ptr);
}
```

---

## Function: sub_82851A10 — RegisterTypeHandler

### Pseudocode
```cpp
void pgStreamer::RegisterTypeHandler(uint32_t nameHash)
{
    const char* name = (const char*)nameHash;

    // Strip leading "__" prefix if present
    if (name[0] == '_' && name[1] == '_')
        name += 2;

    // Look up in handler's type registry (this+24 is a collection/map)
    // sub_82912948 does the actual string-keyed lookup
    void* entry = TypeRegistry_Find(this + 24, &name);

    if (entry && *entry)
        return *(uint32_t*)(*entry);  // return registered type ID
    return 0;
}
```

---

## Native C++ Port

```cpp
#include <cstdint>
#include <cstring>

// Forward declarations matching RAGE engine types
struct fiDevice;         // Virtual filesystem device (packfile, local, etc.)
struct StreamSlot;       // 28-byte buffered stream handle
struct ResourceData;     // Parsed resource with dependency graph
struct IResourceCallback {
    virtual ~IResourceCallback() = default;
    virtual void OnLoad() = 0;
    virtual void Process(void* resource, uint32_t arg) = 0;
};

// --- Stream Slot ---
struct StreamSlot {
    fiDevice* device;
    int32_t   fileHandle;
    uint8_t*  buffer;
    uint32_t  fileOffset;
    uint32_t  bytesBuffered;
    uint32_t  dirtyBytes;
    uint32_t  bufferCapacity;
};
static_assert(sizeof(StreamSlot) == 28);

// --- Resource Data (from sub_8284FC98) ---
struct ResourceData {
    void*    mainResource;      // +0
    void**   subResourceArray;  // +4
    uint16_t subResourceCount;  // +8
    uint16_t ownsArray;         // +10
    void*    extraData;         // +12
    void*    extraData2;        // +16
};
static_assert(sizeof(ResourceData) == 20);

// --- Port of sub_82852DD0 ---
class fiStreamer {
public:
    // Opens a resource file, reads it, invokes the callback, and cleans up.
    // Returns true if the resource was successfully loaded and processed.
    bool OpenAndProcess(const char* devicePath, const char* filename,
                        IResourceCallback* callback, uint32_t callbackArg1,
                        uint32_t callbackArg2)
    {
        // Step 1: Search all mounted devices for this file
        StreamSlot* slot = m_streamingMgr->FindAndOpenResource(
            devicePath, filename, /*isRelative=*/false, /*syncOpen=*/true);

        if (!slot)
            return false;

        // Step 2: Read, parse, and dispatch
        bool result = ProcessOpenedResource(slot, callback, callbackArg1, callbackArg2);

        // Step 3: Flush and close
        slot->FlushAndClose();

        return result;
    }

private:
    bool ProcessOpenedResource(StreamSlot* stream, IResourceCallback* callback,
                                uint32_t arg1, uint32_t /*arg2*/)
    {
        ResourceData* resData = ReadAndParseStream(stream);
        if (!resData) return false;

        bool result = false;
        void* resPtr = resData->mainResource;

        if (resPtr) {
            // Register the resource type in our handler map
            RegisterTypeHandler(resPtr);

            // Dispatch the callback
            callback->Process(resPtr, arg1);
            result = true;
        }

        // Cleanup
        FreeResourceData(resData);
        return result;
    }

    ResourceData* ReadAndParseStream(StreamSlot* stream);
    void RegisterTypeHandler(void* resource);
    void FreeResourceData(ResourceData* data);

    struct StreamingManager* m_streamingMgr;  // points to global at 0x82B07278
};
```

---

## Key Observations for Port

1. **Device Resolution**: The `sub_82855460` prefix-matching is a simple dispatch table. In a native port, replace with `std::unordered_map<std::string, fiDevice*>` keyed by mount prefix.

2. **Stream Slots**: Fixed-size pool of 28-byte slots with 4KB buffers. In a native port, replace with `std::vector<std::unique_ptr<StreamSlot>>` or a proper pool allocator.

3. **Virtual Dispatch**: All device operations (Open, Close, Read, Write, Seek, Sync) go through vtable calls. The vtable layout for `fiDevice` is:
   - vtable[0] = ??? (init/ref)
   - vtable[1] = Open(path, mode) -> handle
   - vtable[4] = Read(handle, buf, size) -> bytesRead
   - vtable[9] = Write(handle, buf, offset, size)
   - vtable[10] = Close(handle)
   - vtable[13] = Sync(handle)

4. **Debug Locking**: Bit 17 of `(*(g_debugFlags))+40` controls whether streaming operations acquire/release a debug lock (`sub_828470E0` / `sub_82847120`). This is a debugging feature that can be omitted in a release port.

5. **Resource Dependency Graph**: `sub_8284FC98` builds a 3-level dependency graph for RAGE resources. This is the `pgBase` serialization system - each resource has primary chunks, secondary chunks, and tertiary chunks. The native port should implement this as a proper `ResourceGraph` class.

6. **Memory Allocation**: All allocations go through TLS-based allocator at `r13->TLS[1676]`. This is the game's custom heap allocator (`sub_8218BE28` / `sub_821B3560`). In a native port, use `std::allocator` or a custom pool.

7. **Thread Safety**: The slot allocation in `sub_8285A968` is protected by `g_streamingLock` (a critical section). The native port should use `std::mutex`.
