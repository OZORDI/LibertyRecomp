# Resource Loader: sub_82852DD0 (OpenAndProcess) and sub_82852D18 (Process)

## Overview

`sub_82852DD0` is the top-level streaming resource loader. It locates a resource file across registered devices, opens it, looks up or creates an internal resource record, dispatches a processing callback, then cleans up. `sub_82852D18` handles the second half (lookup/create + callback + cleanup).

---

## Function Signatures (Deduced)

```
// sub_82852DD0 — OpenAndProcess
// r3 = this (streaming context object)
// r6 = callback_obj (interface with vtable)
// r7 = callback_arg1
// r8 = callback_arg2
// Returns: bool (1=success, 0=not found or error)

// sub_82852D18 — Process
// r3 = this
// r4 = device_result (from Find, 24-byte struct at minimum)
// r5 = callback_obj
// r6 = callback_arg1
// r7 = callback_arg2
// Returns: bool (1=processed, 0=skipped/already loaded)
```

---

## Full Pipeline (Step by Step)

### Step 1: Find Device — sub_8284F468

Called from `sub_82852DD0` at `0x82852E00`.

**Parameters:**
- r3 = global streaming manager at `0x82907278` (lis -32080, addi +29304)
- r4 = unused (0)
- r5 = resource name string
- r6 = 0
- r7 = match_flag (1)

**Logic:**
1. Reads device count from `streaming_mgr+3076` (offset `0xC04`)
2. Loops `i = 0..device_count`:
   a. Calls `sub_8284F0C0(streaming_mgr, stack_buf, 256, name, ext_filter, i)` to build the full path
   b. Calls `sub_8285AA68(stack_buf, match_flag)` to try opening on the resolved device
   c. If `sub_8285AA68` returns non-zero: breaks and returns the device result
3. Returns device result pointer (r3) or 0 if not found

### Step 1a: Build Path — sub_8284F0C0

**Parameters:** r3=streaming_mgr, r4=out_buf, r5=buf_size(256), r6=name, r7=ext_filter, r8=device_idx

**Logic:**
1. If name is NULL: sets `needs_device_prefix = 1`
2. Else checks if name starts with `/` (0x2F) or `\` (0x5C) or contains `:` (via `rexcrt_strchr`)
   - If absolute path: `needs_device_prefix = 0`
   - If relative: `needs_device_prefix = 1`
3. If needs prefix:
   - Computes device path offset: `(device_idx << 8) + streaming_mgr_base`
   - Copies device prefix bytes into out_buf (byte-by-byte loop, max `buf_size-1`)
   - Null-terminates
   - If name[0] == `$` (0x24): skips 2 chars (variable expansion marker)
   - Else checks `streaming_mgr+3072` (`0xC00`) for base path:
     - Calls `sub_8284E690(streaming_mgr, base_path_ptr)` to validate
     - Copies base path prefix into buf (or calls `sub_82211840` for concatenation)
4. Then appends the resource name to buf
5. If ext_filter provided: appends extension filter

### Step 1b: Match/Open on Device — sub_8285AA68

**Parameters:** r3=path_buf, r4=match_flag

**Logic:**
1. Calls `sub_82855460(path_buf, match_flag)` to resolve the device object
2. If device found (non-NULL):
   - Calls `device->vtable[4](device, path_buf, match_flag)` — **Open** method
   - If Open returns valid handle (not -1):
     - Logs success via `sub_822BCA90` (debug output)
     - Checks global callback at `0x831AB948`: if set, calls it
     - If callback rejects: calls `device->vtable[40]` (Close) to undo
     - Else calls `sub_8285A968(path_buf, handle, device)` to build result
3. Returns result struct pointer or 0

### Step 1c: Resolve Device by Prefix — sub_82855460

**Parameters:** r3=path_string, r4=match_flag

Compares the path against known device prefixes using `rexcrt_strncmp`:

| Order | Prefix Length | Address | Returns |
|-|-|-|-|
| 1 | 7 chars | `0x82085480` | `0x82908004` (update device) |
| 2 | 10 chars | `0x8208599C` | `0x82908004` (same) |
| 3 | 6 chars | `0x82085A18` | result of `sub_828708C8` (game device) |
| 4 | 6 chars | `0x82085A10` | `0x82907EF0` (local device) |
| 5 | 7 chars | `0x82085A08` | `0x82907EF0` (same) |
| 6 | 5 chars | `0x82085A00` | `0x82907EF0` (same) |
| 7 | 4 chars | `0x820859F8` | `0x82907EF0` (same) |
| 8 | 3 chars (+`:` check) | `0x820859F4` | `0x82907EF0` |

If no fixed prefix matches, falls through to the **registered device table** at `0x831AB940`:
- Table at `[0x831AB940]+0` = device array base, `[0x831AB940]+4` = count (u16)
- Each device entry = 276 bytes, with prefix length at entry+264 (u16)
- Loops all registered devices, calls `sub_82854A20(device_entry, path, prefix_len)` to match
- Picks the device with the longest matching prefix
- If device's type field at entry+272 == 1: returns `entry->ptr_268->value[0]` directly
- Otherwise walks a priority list of sub-devices

---

### Step 2: Process — sub_82852D18

Called from `sub_82852DD0` at `0x82852E24`.

**Parameters:** r3=this, r4=device_result, r5=callback_obj, r6=callback_arg1, r7=callback_arg2

#### Step 2a: Lookup/Create Resource — sub_82852A50

1. **Checks streaming-locked flag:**
   - Loads global ptr from `0x82A255EC` -> reads `+40` from that ptr
   - Extracts bit 17: `(value >> 17) & 1` (via `rotlw 15 & 1`)
   - If set: calls `sub_828470E0` (AcquireAllocatorLock)

2. **Looks up name in tree:** `sub_82852300(this, device_result, &idx, &extra_idx)`
   - Reads 4-byte identifier from name descriptor via `sub_8285AD08`
   - If < 4 bytes: returns idx=-1 (not found)
   - Creates tree iterator (`sub_828D0608`) rooted at `this+24`
   - Walks nodes: for each, checks `node[0] != 1`, then calls `node+4->vtable[12]` to compare
   - Returns matching entry index or `idx=1` (sentinel for "not found")

3. **Hash table lookup:** `sub_82851DF0(this, &idx)`
   - Structure: `this+0` = bucket_array_ptr, `this+4` = capacity (u16)
   - Bucket = `idx % capacity`, then `bucket_array[bucket * 4]`
   - Walks collision chain via `node+56` link
   - Returns `&node[4]` on match, or NULL

4. **If resource node found (`node+16` has vtable):**
   - `vtable[+12](node+16, device_name)` — **Create**: allocates/opens resource, returns resource obj ptr
   - If obj ptr non-NULL:
     - `device_result->vtable[4](obj, extra_idx)` — **SetHandle**: assigns streaming handle
     - `sub_8284FC98(obj)` — **AllocPathDescriptors**: allocates 20 bytes, builds two path descriptors at obj+12 and obj+28 via `sub_8284D220`
     - `obj->vtable[0](obj, 1)` — **Init**: initializes resource
   - Else if no obj ptr found: falls to secondary vtable path
     - `node+32->vtable[12](node+32, device_name, extra_idx)` — alternate create path

5. **Releases allocator lock** if it was acquired

6. **Returns resource obj pointer or 0**

#### Step 2b: Strip Prefix and Register — sub_82851A10

If `sub_82852A50` returned non-NULL and `result[+0]` is non-NULL:

1. Checks if resource name starts with `__` (two underscores, 0x5F 0x5F)
2. If so: advances past the prefix
3. Calls `sub_82912948(this+24, &name_ptr)` — dictionary lookup to register the resource name
4. Returns the registered resource object or NULL

#### Step 2c: Process Callback (Indirect Call)

```
callback_obj->vtable[+8](callback_obj, resource_node, callback_arg2)
```

This is the actual resource processing — reading file data, decompressing, etc. The specific behavior depends on the vtable implementation of the callback object.

#### Step 2d: Cleanup — sub_8284FA58 (FreeResources)

Frees all temporary allocations made during resource loading:

1. Iterates `node+8` (u16 count) times:
   - For each: loads ptr from `node+4[i*4]`, calls `sub_821B3560` (TLS free)
2. If `node+0` != NULL: calls `sub_82863628` (cleanup), then `sub_821B3560` (free)
3. If `node+10` (u16) != 0: calls `sub_821B3560` on `node+4` (free bucket array)

#### Step 2e: Free Resource Object

Calls `sub_821B3560(resource_obj)` — TLS-based free

### Step 3: Close Device — sub_8285B088

Called from `sub_82852DD0` at `0x82852E30` after `sub_82852D18` returns.

1. Checks `device_result+20` and `device_result+16`:
   - If `+20 == 0` and `+16 != 0`: calls `sub_8285A8B0` (internal close helper)
2. Loads `device_result+0` (device object ptr) and `device_result+4` (handle)
3. Calls `device->vtable[+40](device, handle)` — **Close** method
4. Sets `device_result+4 = -1` (invalid handle)
5. Sets `device_result+0 = 0` (NULL device)

### Step 4: Return

`sub_82852DD0` returns the result of `sub_82852D18` (bool success).
If the initial Find (`sub_8284F468`) returned NULL, returns 0 immediately.

---

## Memory Allocations

| Function | Size | Alignment | Via | Purpose |
|-|-|-|-|-|
| sub_8284FC98 | 20 bytes | 16 | sub_821B3510 (TLS alloc) | Resource descriptor (zeroed fields: 0,4,8h,10h,12,16) |
| sub_82852A50 indirect | varies | varies | vtable[+12] Create | Actual resource data allocation |
| sub_82851DF0 (rehash path) | capacity*4 | 16 | sub_821B3510 | Hash table bucket resize |

All allocations use `TLS[1676]->vtable[+8]` (the current thread's allocator).

---

## TLS Allocator Manipulation

### TLS Layout (offsets from thread data block at `[r13+0]`)

| Offset | Type | Name | Purpose |
|-|-|-|-|
| 1668 | u32 | lock_depth | Recursive allocator lock count |
| 1672 | ptr | saved_allocator | Previous allocator (saved during lock swap) |
| 1676 | ptr | current_allocator | Active allocator for sub_821B3510/sub_821B3560 |
| 1680 | ptr | allocator_owner_id | Owner identity for lock comparison |
| 1700 | u8 | flag | Used by sub_8284D320 (path validation) |

### sub_828470E0 — AcquireAllocatorLock

```
tls = [r13+0]
if tls[1676] == tls[1680]:   // same owner
    tls[1668] += 1           // increment recursive depth
else:
    tls[1672] = tls[1676]    // save current allocator
    tls[1676] = tls[1680]    // install owner's allocator
```

### sub_82847120 — ReleaseAllocatorLock

```
tls = [r13+0]
if tls[1668] > 0:
    tls[1668] -= 1           // decrement recursive depth
else:
    old = tls[1672]
    tls[1672] = 0            // clear saved
    tls[1676] = old          // restore previous allocator
```

### When Locks Are Acquired

The streaming-locked flag is checked in three places:
1. `sub_82852A50` — before LookupEntry (so allocations during lookup use streaming allocator)
2. `sub_82852D18` — before FreeResources (so frees go to correct allocator)
3. `sub_82852E80` (sibling function) — similar pattern

The flag is at: `*(*0x82A255EC + 40)`, bit 17.

---

## Lock Acquisitions

| Lock Type | Acquired In | Released In | Mechanism |
|-|-|-|-|
| TLS allocator swap | sub_82852A50 (conditional) | sub_82852A50 | sub_828470E0 / sub_82847120 |
| TLS allocator swap | sub_82852D18 (conditional) | sub_82852D18 | sub_828470E0 / sub_82847120 |

Note: There are NO mutex/critical section locks in this chain. The "lock" is a TLS-local allocator swap, which means the streaming system's allocator temporarily replaces the thread's default allocator for the duration of resource lookup and cleanup. This is thread-safe because TLS is per-thread.

---

## File I/O Calls

No direct file I/O (NtOpenFile, NtReadFile) occurs in this chain. All file operations are dispatched through **device vtables**:

| Vtable Offset | Method | Called From |
|-|-|-|
| +4 | Open | sub_8285AA68 (via GetDevice result) |
| +8 | Process/Read | sub_82852D18 (callback_obj) |
| +40 | Close | sub_8285B088 |
| +84 | FindFirst | sub_8284F568 (directory enumeration) |
| +88 | FindNext | sub_8284F568 |

The actual NtOpenFile/NtReadFile calls happen inside the device implementations (RPF device, raw file device, etc.), not in this loader chain.

### sub_82849860 / sub_8284C290 — Semaphore Signal

`sub_8284C290` tail-calls `sub_82849860`, which:
1. If r3 (handle) == 0: returns immediately
2. Calls `sub_82A12F50(handle, 1, 0)` which calls `NtReleaseSemaphore`
3. Returns bool (success if NTSTATUS >= 0)

This is used to signal completion of async I/O, not directly in the open-and-process chain but as part of the broader streaming system notification.

---

## Error Handling

Error handling is minimal:

1. **Find fails (no device matched):** `sub_82852DD0` returns 0 immediately. No error is logged.
2. **Open fails (vtable[4] returns -1):** `sub_8285AA68` logs via `sub_822BCA90`, returns 0.
3. **Lookup fails (name not in tree/hash):** `sub_82852A50` returns 0, `sub_82852D18` returns immediately.
4. **Alloc fails (sub_821B3510 returns NULL):** `sub_8284FC98` sets local to 0, continues (may crash later).
5. **Hash table empty:** `sub_82851DF0` returns NULL cleanly.

There are no exception handlers, no try/catch equivalents, and no NTSTATUS propagation.

---

## Resource Registration in Streaming Table

Resources are tracked via two complementary structures:

### 1. Tree (Sorted Container) — accessed via sub_82852300

- Root at `this+24` (the streaming context's resource dictionary)
- Iterator created via `sub_828D0608`
- Nodes have: `[0]` = type/flag, `[4+]` = embedded vtable interface
- Comparison via `vtable[+12]` of the embedded interface
- Used for ordered name lookups

### 2. Hash Table — accessed via sub_82851DF0

- At `this+0` (base of streaming context)
- Structure: `+0` = bucket_array_ptr, `+4` = capacity (u16)
- Index computed as: `key % capacity`
- Each bucket slot = 4-byte pointer to first node
- Nodes: `[0]` = key (hash), `[4..55]` = resource data, `[56]` = next_ptr (collision chain)
- On match: returns `&node[4]` (start of resource data, 52 bytes)

### Resource Node Layout (from sub_8284FC98 allocations)

```
Offset  Size  Field
  +0     4    ptr to internal data (or NULL if not loaded)
  +4     4    bucket array ptr
  +8     2    child count
  +10    2    owns-bucket flag
  +12    16   path descriptor 1 (built by sub_8284D220)
  +28    16   path descriptor 2 (built by sub_8284D220)
  +44    ...  additional fields
```

### Path Descriptor Layout (sub_8284D220)

```
Offset  Size  Field
  +0     4    hash / inline value (when path_ptr is NULL)
  +4     4    secondary data
  +8     4    path string pointer (when non-NULL)
  +12    4    metadata
```

If `path_ptr` is NULL: stores inline hash at `+0`, NULL at `+8`.
If `path_ptr` is non-NULL: copies up to `len` bytes from data source via memcpy, zero-fills if len < 8.

---

## Global Addresses Summary

| Address | Content |
|-|-|
| `0x82907278` | Global streaming manager (passed to sub_8284F468) |
| `0x82A255EC` | Ptr to streaming state object (read for lock flag at +40, bit 17) |
| `0x82908004` | Update/platform device object |
| `0x82907EF0` | Local file device object |
| `0x831AB940` | Registered device table (array base + count) |
| `0x831AB948` | Global resource-open callback function pointer |
| `0x831AB950` | Debug log output handle |
| `0x83192F90` | Streaming resource table base (+0=bucket_count, +28=entries) |
| `0x83192FA0` | Device lookup table |
| `0x83192FC4` | Device array (for sub_8284C298) |
| `0x83192FD0` | Lock name for device registration |
| `0x83192FF0` | Device check/validation structure |

---

## Call Graph

```
sub_82852DD0 (OpenAndProcess)
  |
  +-- sub_8284F468 (Find)
  |     |
  |     +-- [loop over devices]
  |           |
  |           +-- sub_8284F0C0 (BuildPath)
  |           |     +-- rexcrt_strchr (check for ':')
  |           |     +-- sub_8284E690 (validate base path)
  |           |     +-- sub_82211840 (string concat)
  |           |
  |           +-- sub_8285AA68 (MatchAndOpen)
  |                 +-- sub_82855460 (GetDevice)
  |                 |     +-- rexcrt_strncmp (prefix match x8)
  |                 |     +-- sub_82854A20 (registered device match)
  |                 |
  |                 +-- device->vtable[4] (Open)
  |                 +-- global_callback (optional filter)
  |                 +-- device->vtable[40] (Close, on reject)
  |                 +-- sub_8285A968 (build result struct)
  |
  +-- sub_82852D18 (Process)
  |     |
  |     +-- sub_82852A50 (Lookup/Create)
  |     |     +-- sub_828470E0 (AcquireAllocatorLock, conditional)
  |     |     +-- sub_82852300 (LookupEntry in tree)
  |     |     |     +-- sub_8285AD08 (read name bytes)
  |     |     |     +-- sub_828D0608 (create iterator)
  |     |     |     +-- sub_82851C40 (advance iterator)
  |     |     |     +-- node->vtable[12] (compare)
  |     |     |
  |     |     +-- sub_82851DF0 (HashTableLookup)
  |     |     +-- node->vtable[12] (Create resource)
  |     |     +-- device->vtable[4] (SetHandle)
  |     |     +-- sub_8284FC98 (AllocPathDescriptors)
  |     |     |     +-- sub_821B3510 (TLS alloc, 20 bytes)
  |     |     |     +-- sub_8284D220 (PathDescriptor ctor, x2)
  |     |     |           +-- sub_82A00DC0 (memcpy)
  |     |     |           +-- sub_829FF840 (memset, zero-fill)
  |     |     |
  |     |     +-- obj->vtable[0] (Init resource)
  |     |     +-- sub_82847120 (ReleaseAllocatorLock, conditional)
  |     |
  |     +-- sub_82851A10 (StripPrefix + Register)
  |     |     +-- sub_82912948 (dictionary lookup on this+24)
  |     |
  |     +-- callback_obj->vtable[8] (Process/Load data)
  |     |
  |     +-- sub_828470E0 (AcquireAllocatorLock, conditional)
  |     +-- sub_8284FA58 (FreeResources)
  |     |     +-- sub_821B3560 (TLS free, per child)
  |     |     +-- sub_82863628 (cleanup internal)
  |     |     +-- sub_821B3560 (TLS free, node data)
  |     |
  |     +-- sub_821B3560 (TLS free, resource obj)
  |     +-- sub_82847120 (ReleaseAllocatorLock, conditional)
  |
  +-- sub_8285B088 (CloseDevice)
        +-- sub_8285A8B0 (internal close helper, conditional)
        +-- device->vtable[40] (Close)
```

---

## Key Findings for Native Rewrite

1. **No direct file I/O** in this chain. All I/O is vtable-dispatched through device objects. A native rewrite needs to implement the device interface, not hook individual NT calls.

2. **TLS allocator swap is the only synchronization.** No mutexes, no critical sections. The allocator lock is really just a TLS-local pointer swap, making this inherently thread-safe per-thread.

3. **Device resolution is prefix-based.** Eight hardcoded prefix checks (strncmp) followed by a registered-device table walk. The longest-matching prefix wins.

4. **The callback vtable[+8] is where actual loading happens.** The entire find/open/lookup pipeline is just setup; the real work (decompression, parsing, upload) happens in the indirect call at `0x82852D84`.

5. **Error handling is sparse.** NULL returns propagate silently. The only logged error is a failed Open in `sub_8285AA68`.

6. **20-byte resource descriptors** are allocated per resource load via TLS alloc, with two 16-byte path descriptors embedded. These are freed in `sub_8284FA58` after processing completes.

7. **The streaming-locked flag** at `*(*0x82A255EC + 40) bit 17` controls whether the allocator swap happens. When set, the streaming system temporarily installs its own allocator into the thread's TLS[1676] for the duration of resource lookup and cleanup.
