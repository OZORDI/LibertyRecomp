# RexGlue VFS and Streaming Architecture

## Key Question Answered

**When RAGE's streaming system calls vtable methods on a VFS device to open/read files, does RexGlue intercept at the NT kernel level or at device-level vtable hooks?**

RexGlue intercepts at the **NT kernel level**. All file I/O from both the game's CRT layer and its NT syscall layer converges into the same VFS. No device-level vtable hooks are needed because the game's internal VFS device classes (fiDeviceLocal, fiPackfile) ultimately call NtCreateFile/NtReadFile, and RexGlue's kernel export table intercepts those calls before they reach any OS.

---

## Two-Layer Interception Architecture

### Layer 1: NT Kernel Exports (xboxkrnl imports)

**Source**: `glue/rexglue-sdk-main/src/kernel/xboxkrnl/xboxkrnl_io.cpp`

These intercept the Xbox 360 kernel syscalls that every file operation ultimately passes through:

| Export | What it does |
|-|-|
| `NtCreateFile` | Resolves Xbox path via VFS, creates XFile object in kernel object table, returns handle |
| `NtOpenFile` | Thin wrapper calling NtCreateFile with kOpen disposition |
| `NtReadFile` | Looks up XFile by handle, calls `XFile::Read()` (always sync -- `if (true \|\| file->is_synchronous())`) |
| `NtReadFileScatter` | Scatter-gather variant; same sync-only behavior |
| `NtWriteFile` | Looks up XFile by handle, calls `XFile::Write()` (sync-only) |
| `NtQueryInformationFile` | File metadata: position, size, timestamps, attributes |
| `NtSetInformationFile` | Set position, EOF, IO completion port association |
| `NtQueryVolumeInformationFile` | Volume info (size, attributes, name) |
| `NtQueryFullAttributesFile` | Stat a path without opening it |
| `NtQueryDirectoryFile` | Directory enumeration with wildcard matching |
| `NtFlushBuffersFile` | Stub (no-op, returns success) |
| `NtCreateIoCompletion` | Creates IO completion port object |
| `NtSetIoCompletion` / `NtRemoveIoCompletion` | Queue/dequeue IO completion notifications |

**Source**: `glue/rexglue-sdk-main/src/kernel/xboxkrnl/xboxkrnl_io_info.cpp`

Additional info-class handlers for NtQuery/NtSetInformationFile and NtQueryVolumeInformationFile.

### Layer 2: CRT / Win32 API Exports (rexcrt hooks)

**Source**: `glue/rexglue-sdk-main/src/kernel/crt/file.cpp`

These replace the game's statically-linked Win32 CRT functions. Each one calls back into the same VFS that the NT layer uses:

| rexcrt Export | XAM Export | Backed by |
|-|-|-|
| `rexcrt_CreateFileA` | `__imp__CreateFileA` | `VFS::OpenFile()` -> XFile handle |
| `rexcrt_ReadFile` | `__imp__ReadFile` | `XFile::Read()` |
| `rexcrt_WriteFile` | `__imp__WriteFile` | `XFile::Write()` |
| `rexcrt_SetFilePointer` | `__imp__SetFilePointer` | `XFile::set_position()` |
| `rexcrt_CloseHandle` | `__imp__CloseHandle` | Releases XFile from object table |
| `rexcrt_GetFileSize` | `__imp__GetFileSize` | `XFile::entry()->size()` |
| `rexcrt_GetFileSizeEx` | `__imp__GetFileSizeEx` | Same |
| `rexcrt_FindFirstFileA` | `__imp__FindFirstFileA` | VFS directory iteration |
| `rexcrt_FindNextFileA` | `__imp__FindNextFileA` | Same |
| `rexcrt_DeleteFileA` | `__imp__DeleteFileA` | VFS delete |
| `rexcrt_CreateDirectoryA` | `__imp__CreateDirectoryA` | VFS create entry |
| `rexcrt_MoveFileA` | `__imp__MoveFileA` | Stub (warns) |
| `rexcrt_GetFileAttributesA` | `__imp__GetFileAttributesA` | VFS entry attributes |
| `rexcrt_SetFileAttributesA` | `__imp__SetFileAttributesA` | Stub |
| `rexcrt_CopyFileA` | `__imp__CopyFileA` | VFS open source + dest, copy loop |
| `rexcrt_RemoveDirectoryA` | `__imp__RemoveDirectoryA` | VFS delete |
| `rexcrt_GetDiskFreeSpaceExA` | `__imp__GetDiskFreeSpaceExA` | Reports device capacity |

Each rexcrt function maps a Win32 creation disposition to a VFS FileDisposition, then calls `VirtualFileSystem::OpenFile()`.

---

## VFS Core

**Source**: `glue/rexglue-sdk-main/src/filesystem/virtual_file_system.cpp`

The VFS is a Xenia-derived device/symlink registry:

1. **Devices** are registered with mount paths (e.g. `\Device\Harddisk0\Partition1`).
2. **Symlinks** map Xbox drive letters to device paths (e.g. `game:` -> `\Device\Harddisk0\Partition1`).
3. `ResolvePath()` resolves symlinks iteratively, then finds the matching device by mount-path prefix, then calls `device->ResolvePath(relative_path)` to get an Entry.
4. `OpenFile()` resolves path -> Entry, checks disposition/access, calls `entry->Open()` to get a `File*`.

### Path Resolution Flow

```
Game calls NtCreateFile("game:\\common\\data\\default.rpf")
  -> symlink:  game: => \Device\Harddisk0\Partition1
  -> device:   \Device\Harddisk0\Partition1 (HostPathDevice @ /path/to/game/)
  -> relative: common\data\default.rpf
  -> host:     /path/to/game/common/data/default.rpf
```

---

## VFS Device Types

### HostPathDevice

**Source**: `glue/rexglue-sdk-main/src/filesystem/devices/host_path_device.cpp`

- Maps an Xbox mount path to a host filesystem directory.
- **Snapshots directory tree at mount time** via `PopulateEntry()` (recursive `ListFiles`).
- Files must exist on disk before `Initialize()` is called.
- Read/write via `rex::filesystem::FileHandle` (host `fopen`/`fread`/`fwrite`).
- Can be read-only or read-write.

### DiscImageDevice

**Source**: `glue/rexglue-sdk-main/src/filesystem/devices/disc_image_device.cpp`

- Mounts a GDFX (Xbox 360 disc) image file.
- Memory-maps the entire image via `MappedMemory`.
- Parses MICROSOFT*XBOX*MEDIA header, builds entry tree from GDFX directory structure.
- All reads are direct memcpy from the mmap (zero-copy, inherently sync).
- Read-only.

### StfsContainerDevice

**Source**: `glue/rexglue-sdk-main/src/filesystem/devices/stfs_container_device.cpp`

- Mounts STFS packages (CON/LIVE/PIRS -- Xbox 360 content packages).
- Parses the STFS block chain to build a virtual file tree.
- Used for DLC, title updates, etc.
- Read-only.

### NullDevice

**Source**: `glue/rexglue-sdk-main/src/filesystem/devices/null_device.cpp`

- Returns empty/zero responses for all operations.
- Used as a placeholder for unmounted paths.

---

## XFile Object and Async I/O

**Source**: `glue/rexglue-sdk-main/src/system/xfile.cpp`

XFile wraps a VFS `File*` and lives in the kernel object table (looked up by handle).

### Sync vs Async

**All I/O is currently synchronous.** In `NtReadFile_entry` (line 216):
```cpp
if (true || file->is_synchronous()) {
    // Synchronous.
```

The `true ||` forces the sync path for ALL files regardless of the synchronous flag. The async code path exists but is commented out with TODO markers. The same pattern appears in NtReadFileScatter and NtWriteFile.

### Fake-Async Completion

Even though reads are synchronous, RexGlue provides async signaling:

1. **Event signaling**: If NtReadFile receives an event handle, it signals the event after the sync read completes.
2. **IO Completion Ports**: `XFile::RegisterIOCompletionPort()` allows associating an `XIOCompletion` port. After each read/write, `NotifyIOCompletionPorts()` queues a completion notification. `NtRemoveIoCompletion` dequeues these with a timeout-based semaphore wait.
3. **APC callbacks**: If `apc_routine` is provided, the current thread's APC queue receives a callback entry after the sync read.
4. **async_event_**: Each XFile has an internal auto-reset event (`async_event_`) that is Set after every Read/Write/ReadScatter. This is the XFile's "waitable" signal.
5. **STATUS_PENDING**: For files opened without FILE_SYNCHRONOUS_IO_*, NtReadFile returns `X_STATUS_PENDING` even though the data is already available (the event/IOCP notification tells the caller it's done).

This "sync-read, fake-async-signal" pattern works for RAGE's streaming because the streamer thread posts an NtReadFile, then waits on the event or IOCP -- the wait returns immediately since the data is already read.

---

## LibertyRecomp-Side File System (Parallel Layer)

**Source**: `LibertyRecomp/kernel/io/file_system.cpp`

LibertyRecomp has a **second, independent file I/O layer** using `std::fstream`:

- `XCreateFileA()` resolves paths via `FileSystem::ResolvePath()` (game-specific path mapping with mod overlay, RPF DUMP fallback, shader redirection, button prompt swapping).
- Returns a `FileHandle` (wraps `std::fstream`, not an XFile).
- `XReadFile()`, `XWriteFile()`, `XSetFilePointer()` operate on `FileHandle::stream`.
- Supports OVERLAPPED-style offset reads but all sync.

This layer is hooked via `GUEST_FUNCTION_HOOK` macros (not shown inline -- registered elsewhere in the LibertyRecomp kernel hook tables). It handles the game-specific path remapping that the generic rexcrt/VFS layer doesn't know about.

**Source**: `LibertyRecomp/kernel/vfs.cpp`

A higher-level VFS overlay that builds a file index from the extracted game directory, handles GTA IV-specific path normalization, drive prefix stripping, button prompt asset swapping, and mod directory merging.

---

## How RAGE Streaming Reaches the VFS

RAGE's streaming system (fiDeviceLocal, fiPackfile) uses the Xbox 360 CRT's file functions:

1. `CreateFileA("game:\\common\\data\\default.rpf")` -- intercepted by rexcrt
2. rexcrt calls `VFS::OpenFile()` which resolves symlinks and finds the HostPathDevice
3. `ReadFile(handle, buffer, size, &overlapped)` -- intercepted by rexcrt
4. rexcrt calls `XFile::Read()` -> `HostPathFile::ReadSync()` -> host `FileHandle::Read()`

The game's own VFS device vtable methods (`fiDeviceLocal::Open`, `fiDeviceLocal::Read`, etc.) are **recompiled game code** that runs natively. They call through to CreateFileA/ReadFile, which are the interception points. No vtable hooks are needed.

RPF archives are handled by the game's own `fiPackfile` code (recompiled). It opens the .rpf file via CreateFileA, reads headers, then does offset-based ReadFile calls into the archive. The VFS does not parse RPF format -- that is entirely game-side.

---

## Device Registration at Startup

**Source**: `glue/rexglue-sdk-main/src/system/runtime.cpp` (SetupVfs)

| Mount path | Symlinks | Host path | Device type | R/W |
|-|-|-|-|-|
| `\Device\Harddisk0\Partition1` | `game:`, `d:` | game data root | HostPathDevice | RO |
| `\Device\Harddisk0\PartitionUpdate` | `update:` | update data root | HostPathDevice | RO |
| `\Device\Harddisk0\PartitionUser` | `hdd:`, `userdata:` | user data root | HostPathDevice | RW |

**Source**: `LibertyRecomp/main.cpp`

LibertyRecomp also registers additional devices at runtime:
- Cache partition device for utility drive
- DLC content devices

All mounted via `HostPathDevice` + `RegisterDevice()` + `RegisterSymbolicLink()`.

---

## Summary Table

| Interception layer | Scope | Path resolution | Handle type | Async support |
|-|-|-|-|-|
| NT kernel (xboxkrnl_io) | NtCreateFile, NtReadFile, etc. | VFS symlinks + devices | XFile (kernel object table) | Fake-async (sync read + event/IOCP signal) |
| rexcrt (crt/file.cpp) | CreateFileA, ReadFile, etc. | VFS symlinks + devices | XFile (kernel object table) | Sync only |
| LibertyRecomp (file_system.cpp) | XCreateFileA, XReadFile, etc. | Game-specific ResolvePath + mod overlay | FileHandle (std::fstream) | Sync only (OVERLAPPED offset reads) |
| XAM exports | __imp__CreateFileA, etc. | Same as rexcrt (shared impl) | XFile | Sync only |
