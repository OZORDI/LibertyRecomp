# XamContentCreateEx Implementation Analysis

## Overview

There are **two competing implementations** of XamContentCreateEx in the codebase:

1. **RexGlue (Xenia-derived)** -- `glue/rexglue-sdk-main/src/kernel/xam/xam_content.cpp`
   - Registered via `XAM_EXPORT(__imp__XamContentCreateEx, ...)` at line 382.
   - This is the **active PPC import handler** because it is linked via
     `PPC_EXTERN_IMPORT(__imp__XamContentCreateEx)` in `ppc_func_decls.h`.

2. **Liberty's own** -- `LibertyRecomp/kernel/xam.cpp:380`
   - A plain C++ function, NOT a PPC_FUNC export.
   - Only callable if invoked directly from other C++ code (e.g., the old
     `save_system.cpp` initialization path).
   - **Not reached** when the game's recompiled PPC code calls the import.

**Conclusion: The RexGlue implementation is what runs when the game calls
XamContentCreateEx at runtime.**

---

## RexGlue Implementation (Active)

### Entry Point

```
XamContentCreateEx_entry(user_index, root_name, content_data_ptr, flags,
                         disposition_ptr, license_mask_ptr, cache_size,
                         content_size, overlapped_ptr)
```

Delegates to `xeXamContentCreate()` with `content_data_size = sizeof(XCONTENT_DATA)` (0x134 bytes).

### Mode Dispatch (flags & 0xF)

| Value | Mode              | Behavior                                              |
|-------|-------------------|-------------------------------------------------------|
| 1     | CREATE_NEW        | Fail with `X_ERROR_ALREADY_EXISTS` if exists           |
| 2     | CREATE_ALWAYS     | Delete existing content if present, then create        |
| 3     | OPEN_EXISTING     | Fail with `X_ERROR_PATH_NOT_FOUND` if not found        |
| 4     | OPEN_ALWAYS       | Create if missing, open if exists                      |
| 5     | TRUNCATE_EXISTING | Fail if missing; delete + recreate if exists           |

**GTA IV uses mode 3 (OPEN_EXISTING) in state 12** per doc 28.

### What "Create" Actually Does (ContentManager::CreateContent)

File: `glue/rexglue-sdk-main/src/system/xam/content_manager.cpp:128`

1. Checks if a package with the given `root_name` is already open -- returns
   `X_ERROR_ALREADY_EXISTS` if so.
2. Resolves the package path:
   ```
   content_root / title_id_hex / content_type_hex / file_name
   ```
   Example for save data (type 1):
   ```
   <content_root>/5451000E/00000001/SGTA400/
   ```
3. Checks if the directory already exists -- returns `X_ERROR_ALREADY_EXISTS`
   if so.
4. **Creates the directory** via `std::filesystem::create_directories()`.
   - **No file is created.** Only a directory.
   - **No pre-allocation of space.** The directory is empty.
5. Creates a `ContentPackage` which:
   - Registers a `HostPathDevice` at `\Device\Content\N\` pointing to the
     package directory on the host filesystem.
   - Creates a VFS symbolic link: `<root_name>:` -> `\Device\Content\N\`.
6. Stores the package in `open_packages_` keyed by root_name.

### What "Open" Does (ContentManager::OpenContent)

File: `glue/rexglue-sdk-main/src/system/xam/content_manager.cpp:155`

1. Checks if already open -- `X_ERROR_ALREADY_EXISTS`.
2. Checks if directory exists -- `X_ERROR_FILE_NOT_FOUND` if not.
3. Creates a `ContentPackage` (same VFS mount as Create).

### Key Insight: NO FILES ARE CREATED

XamContentCreateEx creates a **directory container**, not a file. The actual
save file (e.g., `SGTA400`) is written later by the game itself using
`CreateFileA`/`WriteFile` calls routed through the VFS symbolic link.

---

## Async / Overlapped Pattern

### When overlapped_ptr is provided:

1. `xeXamContentCreate()` returns `X_ERROR_IO_PENDING` (997) immediately.
2. The actual work is queued to a **dispatch thread** via
   `CompleteOverlappedDeferredEx()`.
3. The dispatch thread:
   - Sleeps 100ms (`kDeferredOverlappedDelayMillis`).
   - Runs the `run` lambda (which does the Create/Open/Delete).
   - Calls `CompleteOverlappedEx()` which:
     - Writes result to the OVERLAPPED structure in guest memory.
     - Signals the `hEvent` handle (if set) via `XEvent::Set()`.
     - Queues an APC to the requesting thread (if completion routine set).

### When overlapped_ptr is NULL:

- Runs synchronously and returns the result directly.

### OVERLAPPED Structure Layout (XXOVERLAPPED, 0x1C bytes)

| Offset | Size | Field                |
|--------|------|----------------------|
| 0x00   | 4    | Error (result code)  |
| 0x04   | 4    | Length               |
| 0x08   | 4    | InternalLow          |
| 0x0C   | 4    | InternalHigh         |
| 0x10   | 4    | InternalContext       |
| 0x14   | 4    | hEvent               |
| 0x18   | 4    | pCompletionRoutine   |
| 0x1C   | 4    | dwCompletionContext  |
| 0x20   | 4    | dwExtendedError      |

When the operation completes:
- `Error` = X_RESULT code
- `Length` = disposition value (1=Create, 2=Open)
- `hEvent` is signaled if non-zero

---

## XamContentGetDeviceData

### RexGlue Implementation (Active)

File: `glue/rexglue-sdk-main/src/kernel/xam/xam_content_device.cpp:111`

Returns a hardcoded dummy HDD device:
- **total_bytes**: 20 GB (20 * 1024 * 1024 * 1024)
- **free_bytes**: 3 GB (3 * 1024 * 1024 * 1024)
- **device_type**: HDD
- **name**: "Dummy HDD"

### Liberty Implementation (likely inactive)

File: `LibertyRecomp/kernel/xam.cpp:513`

Returns:
- **ulDeviceBytes**: 0x40000000 (1 GB)
- **ulDeviceFreeBytes**: 0x40000000 (1 GB)
- **DeviceType**: XCONTENTDEVICETYPE_HDD (1)
- **name**: "GTA4"

**Both report non-zero free space.** The game reads `free_bytes` to determine
expected content container capacity. If `free_bytes > 0` but the actual content
file on disk has size 0 (or does not exist), the size delta goes negative and
state 12 fails.

---

## XamContentClose

### RexGlue Implementation

File: `glue/rexglue-sdk-main/src/kernel/xam/xam_content.cpp:255`

1. Calls `ContentManager::CloseContent(root_name)`.
2. CloseContent:
   - Closes all open file handles inside the content package.
   - Unregisters the VFS symbolic link (`root_name:`).
   - Unregisters the HostPathDevice.
   - Deletes the ContentPackage object.
3. If overlapped: completes with result and returns `X_ERROR_IO_PENDING`.
4. If synchronous: returns result directly.

---

## Content Path Resolution

### RexGlue ContentManager paths

```
root_path / title_id_hex / content_type_hex / file_name /
```

Where:
- `root_path` = the content root passed to `ContentManager` constructor
- `title_id_hex` = 8-char hex of the running title ID (e.g., `5451000E` for GTA IV)
- `content_type_hex`:
  - `00000001` = XCONTENTTYPE_SAVEDATA (kSavedGame)
  - `00000002` = XCONTENTTYPE_DLC (kMarketplaceContent)
- `file_name` = the `szFileName` from XCONTENT_DATA (e.g., `SGTA400`)

### VFS Mount Created by ContentPackage

```
<root_name>:  ->  \Device\Content\N\  ->  <host_package_path>/
```

After XamContentCreateEx, the game accesses files as:
```
<root_name>:\filename.ext
```
Which resolves through VFS to:
```
<host_package_path>/filename.ext
```

---

## State 12 Failure: Root Cause Chain

Based on doc 28, the failure sequence is:

1. **sub_8284A7E8** calls `XamContentCreateEx(mode=3=OPEN_EXISTING)`.
   - RexGlue checks if the package directory exists.
   - If it does NOT exist: returns `X_ERROR_PATH_NOT_FOUND`.
   - If it DOES exist: opens it, creates VFS mount.

2. **sub_8284ADA0** after content creation completes:
   - Opens the content file via `CreateFileA` (path derived from slot+64).
   - Gets file size via `GetFileSizeEx`.
   - Compares `slot[136]` (expected size from XamContentGetDeviceData) vs
     `slot[144]` (actual disk file size).

3. **The mismatch**:
   - `slot[136]` = non-zero value from `XamContentGetDeviceData.free_bytes`
     (3 GB in RexGlue, or 1 GB in Liberty's version).
   - `slot[144]` = 0 because either:
     - The content directory was just created (empty, no save file yet).
     - `CreateFileA` failed because the VFS path was not properly mounted.
     - The file path derivation at slot+64 points to a non-existent file.

4. **Result**: `expected > actual` -> negative delta -> state 13 (restart).

### Critical Observation

The comparison is fundamentally wrong for the "create new save" case:
- The game creates a content container via XamContentCreateEx.
- Then checks if the **save file** inside it is as large as the **device free space**.
- On Xbox 360, the STFS container pre-allocates space so this check passes.
- In the recomp, `create_directories()` creates an empty directory -- no file
  exists, so the size check always fails.

**Possible fix strategies:**

1. **Hook sub_8284ADA0** to force `*(0x82BF99C8) = 0` (success).
2. **Hook XamContentGetDeviceData** to report `free_bytes = 0`, but this might
   cause the game to think there is no storage available.
3. **Pre-create a dummy file** of the expected size inside the content directory
   when XamContentCreateEx creates it.
4. **Investigate what slot[136] actually stores** -- it may not be `free_bytes`
   directly but a derived value. The pseudocode says "expected content size"
   which may come from the `uliContentSize` parameter of XamContentCreateEx.

---

## Liberty's XamContentCreateEx (Inactive, for Reference)

File: `LibertyRecomp/kernel/xam.cpp:380`

This implementation is simpler and does NOT use the RexGlue content manager:

- **CREATE_ALWAYS (mode 2)**: Registers content in `gContentRegistry`, creates
  the save directory via `std::filesystem::create_directory()`, maps root name
  via `XamRootCreate()`. Returns `ERROR_SUCCESS` synchronously.
- **OPEN_EXISTING (mode 3)**: If content is registered, maps the root name.
  Returns `ERROR_SUCCESS`. If not registered, returns `ERROR_PATH_NOT_FOUND`.
- **No overlapped support**: Always returns synchronously (no ERROR_IO_PENDING).
- **No VFS mount**: Uses `XamRootCreate()` which populates `gRootMap` -- this
  is the Liberty-specific root resolution, not the RexGlue VFS.

---

## File Inventory

| File | Role |
|------|------|
| `glue/rexglue-sdk-main/src/kernel/xam/xam_content.cpp` | Active PPC import handlers for XamContentCreateEx, XamContentClose, etc. |
| `glue/rexglue-sdk-main/src/kernel/xam/xam_content_device.cpp` | XamContentGetDeviceData (dummy 20GB HDD, 3GB free) |
| `glue/rexglue-sdk-main/src/system/xam/content_manager.cpp` | ContentManager: CreateContent, OpenContent, CloseContent, ListContent |
| `glue/rexglue-sdk-main/include/rex/system/xam/content_manager.h` | ContentManager class + XCONTENT_DATA structs |
| `glue/rexglue-sdk-main/src/system/kernel_state.cpp` | CompleteOverlappedDeferredEx dispatch queue (100ms delay) |
| `LibertyRecomp/kernel/xam.cpp` | Liberty's own XamContentCreateEx (inactive for PPC calls) |
| `LibertyRecomp/kernel/xam.h` | Liberty's function declarations |
| `LibertyRecomp/kernel/save_hooks.cpp` | PPC hooks for GTA IV save wrappers |
| `LibertyRecomp/kernel/save_system.cpp` | SaveSystem::Initialize, RegisterSaveSlot |
| `LibertyRecomp/user/paths.h` | GetSavePath, GetGamePath |
| `tools/XenonRecomp/XenonUtils/xbox.h` | XCONTENT_DATA, XDEVICE_DATA, XXOVERLAPPED struct defs |
| `LibertyRecomp/kernel/imports.cpp:1818` | sub_82240B08 hook (content device readiness) |
