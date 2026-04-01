# VFS Device System Map

## Overview

GTA IV (RAGE engine) implements a virtual file system with device objects.
All file I/O flows through `fiDevice` vtable dispatches. The recomp has
**two parallel VFS layers**: the guest RAGE VFS (recompiled PPC) and the
host RexGlue VFS (Xenia-derived C++). They do NOT interact -- the rexcrt
file hooks (CreateFileA etc.) route through the RexGlue VFS, while
unhooked guest code uses the original RAGE VFS with its vtable dispatches.

## RAGE VFS Architecture (Guest / PPC)

### sub_82855460 -- fiDevice::GetDevice (Device Resolution)

Address: `0x82855460`
Params: `r3 = path_string`, `r4 = open_mode_flag (0=normal, 1=read)`
Returns: `r3 = fiDevice* (0 if not found)`

**Phase 1: Hardcoded prefix checks (8 checks, in order)**

|#|strncmp addr|len|prefix (inferred)|returns|
|-|-|-|-|-|
|1|0x82085480|7|`update:`|0x82B18004|
|2|0x8208599C|10|`platform:/`|0x82B18004|
|3|0x82085A18|6|`cache:`|0x82B09318 (via sub_828708C8)|
|4|0x82085A10|6|`game:/`|0x82B07EF0|
|5|0x82085A08|7|`device:`|0x82B07EF0|
|6|0x82085A00|5|`game:`|0x82B07EF0|
|7|0x820859F8|4|`dvd:`|0x82B07EF0|
|8|0x820859F4|3|`cd:` + path[4]==':'|0x82B07EF0|

**Phase 2: Mount table scan**

If no hardcoded prefix matches, scans the dynamic mount table:

- Mount table pointer struct: `0x831BB940`
  - `+0`: `uint32_t` pointer to entry array
  - `+4`: `uint16_t` entry count
- Calls `sub_82854A20(entry, path, name_length)` for prefix matching
- Selects the entry with the **longest matching prefix** (best match)
- Entry has device array at `+268`; walks the array in reverse order
  trying `vtable[1]` (Open) on each device until one succeeds

**Phase 3: Fallback**

If mount table also fails, returns the fallback device at `0x82B07EE8`
(set during startup by `sub_82855A18`). If fallback is null, returns 0.

### Static Device Pointers

|Address|Device|Prefixes|
|-|-|-|
|0x82B07EF0|fiDeviceLocal (main game)|game:/, game:, device:, dvd:, cd:|
|0x82B18004|fiDeviceLocal (update/platform)|update:, platform:/|
|0x82B09318|Cache device|cache:|
|0x82B07EE8|Fallback device|any unmatched path|

### Mount Table Entry Layout (276 bytes)

|Offset|Size|Field|
|-|-|-|
|+0|262|mount path string (null-terminated, e.g. "common:/")|
|+262|2|name_length (u16)|
|+264|2|flags/type (u16)|
|+266|2|reserved (u16)|
|+268|4|device_ptr_array (u32 ptr to array of fiDevice*)|
|+272|2|device_count (u16)|
|+274|2|max_devices_capacity (u16)|

When `device_count == 1`, the single device pointer is read directly:
`*(*(entry + 268))`. When `device_count > 1`, devices are tried in
reverse order (highest index first, like a stack of overlay devices).

### sub_82855A18 -- fiDevice::MountAs (Device Registration)

Address: `0x82855A18`
Params: `r3 = mount_name`, `r4 = fiDevice*`, `r5 = relative_flag`

- Compares mount_name byte-by-byte against hardcoded string at `0x82018E00`
- If match (root device): stores device to `0x82B07EE8` (fallback) and
  sets suppress flag at `0x831BB93C`
- If no match: scans file handle open-slot table, allocates a 4096-byte
  I/O buffer per slot, registers the device in the mount table

### sub_82855878 -- Mount Table Allocator

Address: `0x82855878`
Allocates `count * 276` bytes for the mount table array, initializes
each entry with zeroed fields and a 4-byte device pointer array
(capacity = 1 device per mount point initially).

## fiDevice Vtable Map

All device objects have a vtable pointer at `+0`. The vtable is an array
of function pointers. Based on analysis of dispatchers sub_8285AA68,
sub_8285AB78, sub_8285AC00, sub_8285A890, and sub_8285A8B0:

|Slot|Offset|Name|Signature|
|-|-|-|-|
|0|+0|Destructor|`(this, bool_free)`|
|1|+4|Open|`(this, path, mode) -> handle (-1 = fail)`|
|2|+8|Read|`(this, handle, buf, size) -> bytes_read`|
|3|+12|ReadBulk|`(this, handle, offset, buf, size) -> bytes_read`|
|4|+16|GetFileSize|`(this, path) -> uint32_t size`|
|5|+20|Seek|`(this, handle, offset, origin) -> position`|
|6|+24|SeekLong|`(this, handle, offset_lo, origin)`|
|7|+28|Close|`(this, handle)`|
|8|+32|Delete|`(this, path) -> bool`|
|9|+36|WriteBulk|`(this, handle, buf, size, flags) -> written`|
|10|+40|CloseHandle|`(this, handle)` -- cleanup on Open failure|
|11|+44|Exists|`(this, path) -> bool`|
|12|+48|FindFirst|`(this, pattern, finddata) -> findhandle`|
|13|+52|SetEndOfFile|`(this, handle)`|

### Identified Vtable Addresses

|Address|Section|Class (inferred)|
|-|-|-|
|0x82085C04|.rdata|fiDevice (pure virtual base)|
|0x82086034|.rdata|fiDeviceImplemented (base with default impls)|
|0x82000A4C|.data|fiDeviceLocal (local filesystem device)|
|0x82087704|.rdata|fiDeviceRelative (path-relative wrapper)|
|0x8208610C|.rdata|fiPackfile (RPF archive device)|

## VFS Open Flow (sub_8285AA68)

Address: `0x8285AA68`
Params: `r3 = path_string`, `r4 = open_mode (0=write, 1=read)`

```
1. sub_82855460(path, mode) -> device_ptr
2. if (!device_ptr) return 0;
3. handle = device->vtable[1](device, path, mode)  // Open
4. if (handle == -1) {
     device->vtable[10](device, handle)            // CloseHandle cleanup
     return 0;
   }
5. if (open_hook @ 0x831BB948) {
     if (!open_hook(path, mode)) {
       device->vtable[10](device, handle)
       return 0;
     }
   }
6. file_entry = sub_8285A968(handle, device_ptr)   // Register in handle table
7. return file_entry;
```

### What Can Block

- `sub_82855460` itself does NOT block (pure prefix matching + table scan)
- `vtable[1]` (Open) CAN block if the device implementation does synchronous
  disk I/O or waits for a lock (e.g., RPF device acquiring a read lock on
  the archive file)
- `sub_8285A968` acquires a mutex at `0x831BB970` to allocate a handle slot,
  which could contend with other threads

## File Handle Table

|Field|Address|
|-|-|
|Handle array base|0x831B5DD8|
|Data buffer array|0x831B5F28|
|Mutex/lock|0x831BB970|
|Max slot counter|0x82B181AC|
|Max open files limit|0x82B193A8|

Each handle entry is 28 bytes:

|Offset|Size|Field|
|-|-|-|
|+0|4|device_ptr (0 = free slot)|
|+4|4|file_descriptor (from device Open)|
|+8|4|data_buffer_ptr (4096-byte I/O buffer)|
|+12|4|write_cursor|
|+16|4|read_cursor|
|+20|4|dirty_flag|
|+24|4|buffer_size (4096)|

## Global VFS Variables

|Address|Type|Name|
|-|-|-|
|0x831BB93C|u8|suppress_no_device_error|
|0x831BB940|struct|mount_table_ptr {array_ptr, count}|
|0x831BB948|u32|open_hook_callback (function ptr)|
|0x831BB94C|u32|getsize_hook_callback|
|0x831BB950|u32|debug_log_instance|
|0x831BB95C|struct|close_hook {unknown, callback_ptr}|
|0x831BB970|lock|file_handle_table_mutex|

## RPF Device Mounting

RPF archives (Rockstar Package Format) are mounted as fiPackfile devices.
The game calls MountAs with paths like `"common:/"` mapping to a fiPackfile
that reads from `common.rpf`. This creates mount table entries where the
device is an RPF reader rather than a local filesystem device.

The RPF device's `Open` (vtable[1]) translates the virtual path to an
offset within the archive and returns a "handle" that encodes the offset.
`Read` (vtable[2]/[3]) then reads from the archive file at that offset.

### RPF in the Recomp

LibertyRecomp does NOT use RPF archives at runtime. Instead:

1. Game data is pre-extracted during installation (RPF contents unpacked)
2. `VFS::Initialize()` builds a file index of the extracted directory tree
3. The LibertyRecomp VFS (`kernel/vfs.cpp`) resolves guest paths like
   `"game:\common\data\version.txt"` to host paths like
   `<extracted_root>/common/data/version.txt`
4. Path mappings handle the RPF-to-directory translation:
   - `common.rpf` -> `common/`
   - `xbox360.rpf` -> `xbox360/`
   - `audio.rpf` -> `audio/`
   - `platform:` -> `xbox360/`
   - `common:` -> `common/`

## RexGlue VFS (Host / C++)

The RexGlue layer provides a parallel VFS based on Xenia's filesystem code.
It handles kernel-level NtCreateFile/NtReadFile/NtWriteFile calls.

### Mount Points (from runtime.cpp SetupVfs)

|Symlink|Device Path|Host Path|RO|
|-|-|-|-|
|game:|\\Device\\Harddisk0\\Partition1|game_data_root|yes|
|d:|\\Device\\Harddisk0\\Partition1|game_data_root|yes|
|update:|\\Device\\Harddisk0\\PartitionUpdate|update_data_root|yes|
|hdd:|\\Device\\Harddisk0\\PartitionUser|user_data_root|no|
|userdata:|\\Device\\Harddisk0\\PartitionUser|user_data_root|no|

NullDevice registered for `\\Device\\Harddisk0\\{Partition0, Cache0, Cache1}`.

### I/O Flow Through RexGlue

```
Game PPC code
  -> rexcrt CreateFileA hook (at 0x82A131B0)
    -> NtCreateFile kernel import
      -> VirtualFileSystem::OpenFile()
        -> ResolveSymbolicLink("game:" -> "\\Device\\Harddisk0\\Partition1")
        -> HostPathDevice::ResolvePath()
        -> HostPathEntry::Open() -> HostPathFile
      -> XFile wraps HostPathFile
    -> returns X_HANDLE to guest
```

## What a Native Rewrite Needs

### Current State

There are TWO VFS layers:
1. **RAGE VFS** (guest PPC): fiDevice vtable dispatches, mount table at
   0x831BB940, hardcoded prefix resolution in sub_82855460
2. **RexGlue VFS** (host C++): Xenia-derived, symlink resolution,
   HostPathDevice serving extracted files
3. **LibertyRecomp VFS** (host C++): `kernel/vfs.cpp`, path mapping and
   file index for extracted game data

The rexcrt hooks (CreateFileA, ReadFile, etc.) intercept the CRT-level
file calls, routing them through the RexGlue VFS. But any game code that
calls the RAGE VFS directly (fiDevice::Open via vtable dispatch) bypasses
the rexcrt hooks entirely and goes through the recompiled PPC device code.

### Requirements for Native Rewrite

1. **Hook sub_82855460** (GetDevice): Replace device resolution to return
   a native device object that serves extracted files via host filesystem

2. **Hook sub_8285AA68** (VFS Open): Or alternatively hook the individual
   device vtable entries. The Open function is the critical path -- it
   must not block on the game thread

3. **Hook sub_8285A968** (Handle Registration): May need native handle
   table to avoid guest memory contention

4. **RPF bypass is already working**: Extracted files are served via
   VFS path mappings. No need to implement RPF reading.

5. **Mount table population**: The game calls MountAs during init to
   register devices. A native rewrite could hook MountAs and record
   which mount points the game expects, then serve them via native I/O.

6. **Thread safety**: The file handle mutex at 0x831BB970 protects the
   handle table. A native rewrite must maintain equivalent thread safety.

### Blocking Analysis

The VFS Open path (`sub_8285AA68`) can hang if:
- The device `Open` (vtable[1]) does synchronous I/O on a slow device
- The file handle mutex (0x831BB970) is contended
- The mount table scan loops over many entries with failed Opens

In practice, the main risk is when the streaming system opens RPF files
on the main thread, and the RPF device does a synchronous read to locate
the file entry within the archive. With extracted files, this reduces to
a host filesystem `stat()` call which should not block.
