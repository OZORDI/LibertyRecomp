# RAGE v1 Virtual File System — Device System Deep Dive

## Overview

GTA IV's RAGE engine uses a custom VFS with three layers:

1. **VFS Handler Manager** (global at `0x82B07278`) — iterates registered path handlers to resolve resource paths
2. **Device Mount Table** (global at `0x831AB940`) — maps mount-point prefixes (e.g. `game:\`, `update:\`) to device objects
3. **Device Objects** (vtable-dispatched) — implement file I/O through a 25-slot vtable

The entry point is `sub_8284F468` (resource finder/opener), which calls `sub_8284F0C0` (path resolver), then `sub_8285AA68` (device-based file opener).

---

## 1. VFS Handler Manager Struct (0x82B07278)

Total size: >= 0xC08 bytes. Contains up to 12 handler entries.

| Offset | Size | Name | Description |
|-|-|-|-|
| +0x000 | 256 * 12 | `handlers[12]` | Path prefix strings, one per handler (e.g. "common/data/") |
| +0xC00 | 4 | `default_device_name_ptr` | Pointer to device name string used for relative path prefixing |
| +0xC04 | 4 | `handler_count` | Number of registered handlers (int32) |
| +0xC08 | 4 | `default_handler_index` | Default handler index for fallback resolution |

Each handler entry is 256 bytes of path prefix data. The handler index is used as `this + (index * 256)` to locate the prefix string.

---

## 2. Device Mount Table (0x831AB940)

Global registry of all mounted devices.

| Offset | Size | Name | Description |
|-|-|-|-|
| +0x00 | 4 | `entries_ptr` | Pointer to array of `DeviceMountEntry` |
| +0x04 | 2 | `count` | Number of registered entries (u16) |
| +0x06 | 2 | `capacity` | Max entries allocated (u16) |

### DeviceMountEntry (276 bytes, stride 0x114)

| Offset | Size | Name | Description |
|-|-|-|-|
| +0x000 | 264 | `prefix[264]` | Mount path string, e.g. `"game:\\"`, `"update:\\"` |
| +0x106 | 2 | `flags` | Mount flags (u16); checked by `sub_82855460` at offset +262 |
| +0x108 | 2 | `prefix_len` | String length of prefix (u16) |
| +0x10C | 4 | `devices_ptr` | Pointer to array of device object pointers (supports layered/stacked devices) |
| +0x110 | 2 | `device_count` | Number of devices for this mount point (u16) |
| +0x112 | 2 | `device_capacity` | Max devices in the pointer array (u16) |

Multiple devices can be registered under the same mount point. Resolution tries from highest index (most recently added) to lowest, enabling overlay/patch layering.

---

## 3. Device Vtable (25 slots)

Every device object has a vtable pointer at offset 0. The vtable has at minimum 25 function pointers (100 bytes):

| Slot | Offset | Signature (pseudocode) | Name |
|-|-|-|-|
| 0 | 0 | `void Destructor(device)` | Virtual destructor |
| 1 | 4 | `int32 Open(device, path, flags)` | Open file, returns handle or -1 |
| 2 | 8 | `uint32 Read(device, handle, buffer, size)` | Read from handle |
| 3 | 12 | `uint32 Write(device, handle, buffer, size)` | Write to handle |
| 4 | 16 | `uint32 CreateOrOpenSpecial(device, mode)` | Create/open with special mode |
| 5 | 20 | `void* GetBuffer(device, handle)` | Get memory-mapped buffer |
| 6 | 24 | `bool Rename(device, old_path, new_path)` | Rename file |
| 7 | 28 | `bool Delete(device, path)` | Delete file |
| 8 | 32 | `uint64 GetSize(device, handle)` | Get file size by handle |
| 9 | 36 | `int64 Seek(device, handle, offset, whence)` | Seek in file |
| 10 | 40 | `void Close(device, handle)` | Close handle |
| 11 | 44 | `bool SetAttributes(device, path, attrs)` | Set file attributes |
| 12 | 48 | `uint32 GetAttributes(device, path)` | Get file attributes |
| 13 | 52 | `bool Lock(device, handle, shared)` | Lock file |
| 14 | 56 | `void Unlock(device, handle)` | Unlock file |
| 15 | 60 | `bool Truncate(device, handle, size)` | Set file length |
| 16 | 64 | `bool Flush(device, handle)` | Flush buffers |
| 17 | 68 | `uint32 GetInfo(device, path, out_info)` | Get file info struct |
| 18 | 72 | `bool Exists(device, path)` | Check existence (most frequently called) |
| 19 | 76 | `bool IsDirectory(device, path)` | Check if directory |
| 20 | 80 | `uint32 GetMetadata(device, handle)` | Get metadata/type |
| 21 | 84 | `int32 FindFirst(device, pattern, out_info)` | Begin directory enumeration |
| 22 | 88 | `bool FindNext(device, handle, out_info)` | Continue enumeration |
| 23 | 92 | `void FindClose(device, handle)` | End enumeration |
| 24 | 96 | `bool MakeDirectory(device, path)` | Create directory |

---

## 4. Path Resolution (sub_8284F0C0)

**Signature**: `sub_8284F0C0(mgr, out_buf, buf_size=256, input_path, extension, handler_index)`

### Algorithm

```
path_resolve(mgr, out_buf, buf_size, input_path, extension, handler_idx):
    is_relative = true

    if input_path != NULL:
        ch = input_path[0]
        if ch == '/' or ch == '\\':
            is_relative = false
        elif strchr(input_path, ':') != NULL:
            is_relative = false   // has device prefix like "game:"

    if is_relative:
        // Copy handler path prefix to output buffer
        handler_data = mgr + handler_idx * 256
        strncpy(out_buf, handler_data, buf_size - 1)
        out_buf[copied] = '\0'

        // Check if input starts with '$' (variable substitution)
        if input_path[0] == '$':
            input_path += 2      // skip "$X" prefix

        // Check device name at mgr+0xC00 for additional prefix
        device_name = mgr->default_device_name_ptr
        if device_name != 0:
            has_device = path_has_device_prefix(device_name + 3)
            if not has_device:
                // Append device name prefix to buffer
                strncpy(out_buf, device_name_str, buf_size - 1)
            else:
                // Use snprintf to format full path
                snprintf(out_buf, buf_size, device_name_str, ...)
    else:
        out_buf[0] = '\0'       // absolute path, no prefix needed

    // Normalize '..' sequences in path
    while normalize_dotdot(out_buf):
        pass    // keep collapsing until no more '..'

    // Append file path and normalize
    build_final_path(out_buf, buf_size, input_path, extension)
```

### sub_8284E690 — Relative Path Detector

Returns `true` (1) if path is relative (no device prefix):
- Returns 1 if `path == NULL`
- Returns 0 if `path[0] == '/' or '\\'` (absolute)
- Returns 0 if `strchr(path, ':')` finds a colon (device prefix)
- Returns 1 otherwise (relative)

### sub_8284E700 — ".." Path Normalizer

Collapses parent directory references:
1. Find first `.` in path via `strchr`
2. Check if it's a `/../` or `\..\` pattern (dot preceded by separator, followed by `./` or `.\`)
3. Walk backwards from the dot to find the parent directory name
4. Use `memmove` to collapse `parent/../rest` to `rest`
5. Replace all `\` with `/` (forward-slash normalization)
6. Remove duplicate `//` sequences
7. Returns 0 when no more `..` found (loop until stable)

### sub_8284ED70 — Extension Appender

Appends a file extension if not already present:
1. Calls `strncat(out_buf, input_path, buf_size)` to combine prefix + path
2. If `extension` is non-null and non-empty:
   - `strrchr(path, '.')` to find existing extension
   - If no extension, or `_stricmp(existing_ext, desired_ext) != 0`:
     - Append `"."` then the extension string
3. Final normalization pass:
   - Replace all `\` with `/`
   - Remove duplicate `//`

---

## 5. Device Lookup (sub_82855460)

**Signature**: `sub_82855460(path, try_fallback)` — returns device object pointer or NULL.

### Algorithm

```
find_device(path, try_fallback):
    // Fast-path check for hardcoded device prefixes
    if strncmp(path, "device:", 7) == 0:      // 0x82085480
        return g_device_device            // at 0x82B08004

    if strncmp(path, "platformfs", 10) == 0:  // 0x8208599C
        return g_device_device

    if strncmp(path, "cdvfs:", 6) == 0:       // 0x82085A18
        return cdvfs_get_device()         // sub_828708C8

    if strncmp(path, "dvdfs:", 6) == 0:       // 0x82085A10
        return g_local_device             // at 0x82B07EF0

    if strncmp(path, "update:", 7) == 0:      // 0x82085A08
        return g_local_device

    if strncmp(path, "game:", 5) == 0:        // 0x82085A00
        return g_local_device

    if strncmp(path, "hdd:", 4) == 0:         // 0x820859F8
        return g_local_device

    if strncmp(path, "dvd", 3) == 0 and path[4] == ':':  // 0x820859F4
        return g_local_device

    // Generic lookup: scan device mount table
    mount_table = *(0x831AB940)           // device registry global
    count = *(uint16*)(0x831AB940 + 4)
    entries = mount_table->entries_ptr

    best_match_len = 0
    best_match_idx = -1
    entry = entries + 264                 // start at prefix_len field

    for i in 0..count:
        prefix_len = entry.prefix_len     // u16 at +264
        if path_compare_ci(entries + i*276, path, prefix_len) == 0:
            if prefix_len > best_match_len:
                best_match_len = prefix_len
                best_match_idx = i
        entry += 276

    if best_match_idx == -1:
        // No match found
        if g_local_device_ptr == NULL:
            log_warning("device not found: %s", path)
        if !try_fallback and g_use_fallback_device:
            return NULL
        return g_local_device_ptr

    // Found matching mount point
    entry = entries + best_match_idx * 276
    if entry.device_count == 1:
        return entry.devices_ptr[0]       // single device, fast path

    // Multiple layered devices — try from top (most recent) to bottom
    for j in (entry.device_count - 1) downto 0:
        device = entry.devices_ptr[j]
        handle = device->vtable[1](device, path, 1)    // Open with probe flag
        if handle != -1:
            device->vtable[10](device, handle)          // Close immediately
            return device                               // This device has the file

    // None of the layered devices had the file
    return entry.devices_ptr[0]           // fall back to base device
```

### sub_82854A20 — Case-Insensitive Path Compare

Compares up to N characters, converting uppercase to lowercase and treating `\` as `/`:

```
int path_compare_ci(const char* a, const char* b, int n):
    for i in 0..n:
        ca = tolower(a[i])
        cb = tolower(b[i])
        if ca == '\\': ca = '/'
        if cb == '\\': cb = '/'
        if n == 0 or ca == 0: break
        if ca != cb: return ca - cb
    return ca - cb    // 0 = match
```

---

## 6. File Open (sub_8285AA68)

**Signature**: `sub_8285AA68(path, flags)` — returns resource slot pointer or NULL.

### Algorithm

```
open_resource(path, flags):
    device = find_device(path, 0)       // sub_82855460
    if device == NULL:
        goto fail

    handle = device->vtable[1](device, path, flags)  // Open
    if handle == -1:
        log("open FAILED: %s (flags=%d)", path, flags)
        goto fail
    else:
        log("open OK: %s (flags=%d)", path, flags)

    // Check open callback filter
    if g_open_callback != NULL:         // at 0x831AB948
        if !g_open_callback(path, flags):
            device->vtable[10](device, handle)    // Close — callback rejected
            goto fail

    // Register in file handle table
    slot = register_handle(path, handle, device)  // sub_8285A968
    return slot

fail:
    // Check debug mode
    if g_debug_flag_struct[4] != 0:     // at 0x831AB960
        log("Could not open: %s", path)
    return NULL
```

---

## 7. File Handle Table (sub_8285A968)

**Signature**: `sub_8285A968(path, handle, device)` — registers an open file in the global slot table.

### Global State

| Address | Description |
|-|-|
| `0x831AB970` | Mutex/lock for handle table access |
| `0x82B093A8` | Max handle count |
| `0x831B5DD8` | Handle table base (array of 28-byte entries) |

### Handle Entry (28 bytes)

| Offset | Size | Name |
|-|-|-|
| +0 | 4 | `device_ptr` — owning device object |
| +4 | 4 | `handle` — device-specific file handle (from vtable[1]) |
| +8 | 4 | `buffer_ptr` — data buffer page (4096 byte page at `base + slot_index * 4096`) |
| +12 | 4 | `field_12` — zero-initialized |
| +16 | 4 | `field_16` — zero-initialized |
| +20 | 4 | `field_20` — zero-initialized |
| +24 | 4 | `page_size` — buffer page size (4096) |

### Algorithm

```
register_handle(path, handle, device):
    lock(g_handle_table_lock)

    max_count = g_max_handle_count
    table = g_handle_table_base
    slot_index = 0

    // Find first free slot (device_ptr == 0)
    for i in 0..max_count:
        if table[i].device_ptr == 0:
            slot_index = i
            break
    else:
        log_error("out of file handle slots")
        unlock()
        return NULL

    // Initialize slot
    buf_page = g_buffer_pool_base + slot_index * 4096
    slot = &table[slot_index]
    slot.device_ptr = device
    slot.handle = handle
    slot.buffer_ptr = buf_page
    slot.field_12 = 0
    slot.field_16 = 0
    slot.field_20 = 0
    slot.page_size = 4096

    // Update max if needed
    if slot_index > g_max_used_slot:
        g_max_used_slot = slot_index

    unlock()
    return slot
```

---

## 8. Device Registration (sub_82855A18)

**Signature**: `sub_82855A18(mount_path, device_ptr, flags)`

### Algorithm

```
register_device(mount_path, device_ptr, flags):
    // Verify mount path matches expected pattern (internal check)

    mount_table = *(0x831AB940)
    if mount_table.capacity == 0:
        init_mount_table(mount_table, DEFAULT_CAPACITY)

    path_len = strlen(mount_path)
    count = mount_table.count
    entries = mount_table.entries_ptr

    // Search for existing mount point
    for i in 0..count:
        if path_compare_ci(entries + i*276, mount_path, path_len) == 0:
            entry = entries + i*276
            goto found

    // No existing entry — create new
    if count >= capacity:
        return error   // table full

    entry = entries + count * 276
    mount_table.count = count + 1
    init_device_pointer_array(&entry.devices_ptr)

found:
    // Copy mount path into entry
    strcpy(entry.prefix, mount_path)
    entry.prefix_len = strlen(mount_path)
    entry.flags = flags

    // Add device to the mount point's device list
    if entry.device_count >= entry.device_capacity:
        grow_array(&entry.devices_ptr, entry.device_count + 1)
        entry.devices_ptr[0] = device_ptr
    else:
        entry.devices_ptr[entry.device_count] = device_ptr
        entry.device_count += 1
```

---

## 9. Mapping to RexGlue's Existing VFS

RexGlue (derived from Xenia) already has a parallel VFS implementation:

| RAGE Concept | RexGlue Equivalent | File |
|-|-|-|
| Device vtable | `rex::filesystem::Device` (abstract class) | `include/rex/filesystem/device.h` |
| Device mount table | `VirtualFileSystem::devices_` (vector) | `include/rex/filesystem/vfs.h` |
| Mount path prefix | `Device::mount_path_` (string) | `include/rex/filesystem/device.h` |
| `sub_82855A18` (register) | `VirtualFileSystem::RegisterDevice()` | `src/filesystem/virtual_file_system.cpp` |
| `sub_82855460` (lookup) | `VirtualFileSystem::ResolvePath()` | `src/filesystem/virtual_file_system.cpp` |
| Device vtable[1] (Open) | `Entry::Open()` + `HostPathEntry::Open()` | `include/rex/filesystem/entry.h` |
| Device vtable[10] (Close) | `File::~File()` / RAII | `include/rex/filesystem/file.h` |
| Symlinks (device: prefix) | `VirtualFileSystem::symlinks_` (map) | `src/filesystem/virtual_file_system.cpp` |
| HostPathDevice | `rex::filesystem::HostPathDevice` | `src/filesystem/devices/host_path_device.cpp` |
| File handle table | `rex::system::XFile` (kernel object) | `src/system/xam/` |

### Key Architectural Differences

1. **RAGE uses integer handles** (returned from vtable[1]). RexGlue uses C++ `File*` objects via RAII.
2. **RAGE supports device layering** — multiple devices on one mount point, tried top-down. RexGlue uses single device per mount.
3. **RAGE path resolution is string-based** with prefix matching. RexGlue canonicalizes paths, then does prefix matching against `Device::mount_path_`.
4. **RAGE normalizes `\` to `/`** internally. RexGlue preserves Xbox-style paths until device resolution.

---

## 10. Native C++ Pseudocode for Port

```cpp
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <cstdint>
#include <algorithm>
#include <cstring>

namespace rage::vfs {

// ---------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------
class Device;

// ---------------------------------------------------------------------------
// FileHandle — replaces RAGE's integer handles + 28-byte slot table
// ---------------------------------------------------------------------------
struct FileHandle {
    Device*  device      = nullptr;
    int      native_fd   = -1;   // or FILE*, or std::fstream index
    uint8_t* buffer      = nullptr;
    size_t   buffer_size = 4096;
    size_t   position    = 0;

    bool is_valid() const { return device != nullptr && native_fd >= 0; }
};

// ---------------------------------------------------------------------------
// Device — abstract base replacing RAGE's 25-slot vtable
// ---------------------------------------------------------------------------
class Device {
public:
    virtual ~Device() = default;

    // Core file I/O
    virtual int      Open(const std::string& path, uint32_t flags) = 0;
    virtual size_t   Read(int handle, void* buf, size_t size) = 0;
    virtual size_t   Write(int handle, const void* buf, size_t size) = 0;
    virtual int64_t  Seek(int handle, int64_t offset, int whence) = 0;
    virtual void     Close(int handle) = 0;

    // File info
    virtual uint64_t GetSize(int handle) = 0;
    virtual bool     Exists(const std::string& path) = 0;
    virtual bool     IsDirectory(const std::string& path) = 0;
    virtual uint32_t GetAttributes(const std::string& path) = 0;

    // Directory ops
    virtual bool     MakeDirectory(const std::string& path) = 0;
    virtual bool     DeleteFile(const std::string& path) = 0;
    virtual bool     Rename(const std::string& from, const std::string& to) = 0;

    // Enumeration
    virtual int      FindFirst(const std::string& pattern, std::string& out_name) = 0;
    virtual bool     FindNext(int handle, std::string& out_name) = 0;
    virtual void     FindClose(int handle) = 0;

    // Locking
    virtual bool     Lock(int handle, bool shared) { (void)handle; (void)shared; return true; }
    virtual void     Unlock(int handle) { (void)handle; }
    virtual bool     Flush(int handle) { (void)handle; return true; }
    virtual bool     Truncate(int handle, uint64_t size) { (void)handle; (void)size; return false; }
};

// ---------------------------------------------------------------------------
// HostDevice — maps a mount prefix to a host filesystem directory
// ---------------------------------------------------------------------------
class HostDevice : public Device {
public:
    HostDevice(std::string mount_prefix, std::filesystem::path host_root, bool read_only = true)
        : mount_prefix_(std::move(mount_prefix))
        , host_root_(std::move(host_root))
        , read_only_(read_only) {}

    int Open(const std::string& path, uint32_t flags) override {
        auto host_path = resolve_host_path(path);
        auto mode = std::ios::binary | std::ios::in;
        if (flags & 2) mode |= std::ios::out;

        auto stream = std::make_unique<std::fstream>(host_path, mode);
        if (!stream->is_open()) return -1;

        std::lock_guard lock(mutex_);
        int handle = next_handle_++;
        open_files_[handle] = std::move(stream);
        return handle;
    }

    size_t Read(int handle, void* buf, size_t size) override {
        std::lock_guard lock(mutex_);
        auto it = open_files_.find(handle);
        if (it == open_files_.end()) return 0;
        it->second->read(static_cast<char*>(buf), size);
        return it->second->gcount();
    }

    size_t Write(int handle, const void* buf, size_t size) override {
        if (read_only_) return 0;
        std::lock_guard lock(mutex_);
        auto it = open_files_.find(handle);
        if (it == open_files_.end()) return 0;
        it->second->write(static_cast<const char*>(buf), size);
        return it->second->good() ? size : 0;
    }

    int64_t Seek(int handle, int64_t offset, int whence) override {
        std::lock_guard lock(mutex_);
        auto it = open_files_.find(handle);
        if (it == open_files_.end()) return -1;
        auto dir = (whence == 0) ? std::ios::beg
                 : (whence == 1) ? std::ios::cur
                                 : std::ios::end;
        it->second->seekg(offset, dir);
        return it->second->tellg();
    }

    void Close(int handle) override {
        std::lock_guard lock(mutex_);
        open_files_.erase(handle);
    }

    uint64_t GetSize(int handle) override {
        std::lock_guard lock(mutex_);
        auto it = open_files_.find(handle);
        if (it == open_files_.end()) return 0;
        auto pos = it->second->tellg();
        it->second->seekg(0, std::ios::end);
        auto size = it->second->tellg();
        it->second->seekg(pos);
        return size;
    }

    bool Exists(const std::string& path) override {
        return std::filesystem::exists(resolve_host_path(path));
    }

    bool IsDirectory(const std::string& path) override {
        return std::filesystem::is_directory(resolve_host_path(path));
    }

    uint32_t GetAttributes(const std::string& path) override {
        auto hp = resolve_host_path(path);
        if (!std::filesystem::exists(hp)) return 0;
        uint32_t attrs = 0;
        if (std::filesystem::is_directory(hp)) attrs |= 0x10;
        if (read_only_) attrs |= 0x01;
        return attrs;
    }

    bool MakeDirectory(const std::string& path) override {
        if (read_only_) return false;
        return std::filesystem::create_directories(resolve_host_path(path));
    }

    bool DeleteFile(const std::string& path) override {
        if (read_only_) return false;
        return std::filesystem::remove(resolve_host_path(path));
    }

    bool Rename(const std::string& from, const std::string& to) override {
        if (read_only_) return false;
        std::filesystem::rename(resolve_host_path(from), resolve_host_path(to));
        return true;
    }

    int FindFirst(const std::string& pattern, std::string& out_name) override {
        auto dir_path = resolve_host_path(pattern);
        auto parent = dir_path.parent_path();
        if (!std::filesystem::is_directory(parent)) return -1;

        std::lock_guard lock(mutex_);
        int handle = next_handle_++;
        auto& ctx = find_contexts_[handle];
        for (auto& entry : std::filesystem::directory_iterator(parent)) {
            ctx.entries.push_back(entry.path().filename().string());
        }
        ctx.index = 0;
        if (ctx.entries.empty()) {
            find_contexts_.erase(handle);
            return -1;
        }
        out_name = ctx.entries[0];
        ctx.index = 1;
        return handle;
    }

    bool FindNext(int handle, std::string& out_name) override {
        std::lock_guard lock(mutex_);
        auto it = find_contexts_.find(handle);
        if (it == find_contexts_.end()) return false;
        if (it->second.index >= it->second.entries.size()) return false;
        out_name = it->second.entries[it->second.index++];
        return true;
    }

    void FindClose(int handle) override {
        std::lock_guard lock(mutex_);
        find_contexts_.erase(handle);
    }

    const std::string& mount_prefix() const { return mount_prefix_; }

private:
    std::filesystem::path resolve_host_path(const std::string& game_path) {
        // Strip mount prefix from path
        std::string_view sv(game_path);
        if (sv.substr(0, mount_prefix_.size()) == mount_prefix_) {
            sv.remove_prefix(mount_prefix_.size());
        }
        // Skip leading separators
        while (!sv.empty() && (sv[0] == '/' || sv[0] == '\\')) {
            sv.remove_prefix(1);
        }
        return host_root_ / std::string(sv);
    }

    std::string mount_prefix_;
    std::filesystem::path host_root_;
    bool read_only_;
    std::mutex mutex_;
    int next_handle_ = 1;
    std::unordered_map<int, std::unique_ptr<std::fstream>> open_files_;

    struct FindContext {
        std::vector<std::string> entries;
        size_t index = 0;
    };
    std::unordered_map<int, FindContext> find_contexts_;
};

// ---------------------------------------------------------------------------
// DeviceManager — replaces VFS Handler Manager + Device Mount Table
// ---------------------------------------------------------------------------
class DeviceManager {
public:
    // Register a device under a mount prefix
    void RegisterDevice(std::shared_ptr<Device> device, const std::string& mount_prefix) {
        std::lock_guard lock(mutex_);
        mount_points_[mount_prefix].push_back(std::move(device));
    }

    // Find the best-matching device for a path (longest prefix match)
    Device* FindDevice(const std::string& path) {
        std::lock_guard lock(mutex_);
        std::string best_prefix;

        for (auto& [prefix, devices] : mount_points_) {
            if (path_starts_with_ci(path, prefix) && prefix.size() > best_prefix.size()) {
                best_prefix = prefix;
            }
        }

        if (best_prefix.empty()) return nullptr;

        auto& devices = mount_points_[best_prefix];

        // Try layered devices top-down (most recently added first)
        for (int i = static_cast<int>(devices.size()) - 1; i >= 0; --i) {
            int h = devices[i]->Open(path, 1);  // probe
            if (h != -1) {
                devices[i]->Close(h);
                return devices[i].get();
            }
        }
        return devices.empty() ? nullptr : devices[0].get();
    }

    // Resolve a possibly-relative path to an absolute device path
    std::string ResolvePath(const std::string& input,
                            const std::string& default_prefix = "",
                            const std::string& extension = "") {
        std::string result;

        // Check if path is relative (no ':', '/', or '\' prefix)
        bool is_relative = true;
        if (!input.empty()) {
            char ch = input[0];
            if (ch == '/' || ch == '\\') is_relative = false;
            else if (input.find(':') != std::string::npos) is_relative = false;
        }

        if (is_relative && !default_prefix.empty()) {
            result = default_prefix;
            if (!result.empty() && result.back() != '/' && result.back() != '\\') {
                result += '/';
            }
            result += input;
        } else {
            result = input;
        }

        // Normalize path
        normalize_path(result);

        // Append extension if needed
        if (!extension.empty()) {
            auto dot = result.rfind('.');
            auto sep = result.rfind('/');
            if (dot == std::string::npos || (sep != std::string::npos && dot < sep)) {
                result += '.';
                result += extension;
            } else {
                std::string existing_ext = result.substr(dot + 1);
                if (!iequals(existing_ext, extension)) {
                    result += '.';
                    result += extension;
                }
            }
        }

        return result;
    }

    // Open a resource: resolve path, find device, open file
    FileHandle* OpenResource(const std::string& path,
                             const std::string& extension = "",
                             uint32_t flags = 0) {
        std::string resolved = ResolvePath(path, default_prefix_, extension);
        Device* dev = FindDevice(resolved);
        if (!dev) return nullptr;

        int handle = dev->Open(resolved, flags);
        if (handle < 0) return nullptr;

        // Register in handle table
        std::lock_guard lock(mutex_);
        auto fh = std::make_unique<FileHandle>();
        fh->device = dev;
        fh->native_fd = handle;
        auto* ptr = fh.get();
        handles_.push_back(std::move(fh));
        return ptr;
    }

    void SetDefaultPrefix(const std::string& prefix) { default_prefix_ = prefix; }

private:
    static bool path_starts_with_ci(const std::string& path, const std::string& prefix) {
        if (path.size() < prefix.size()) return false;
        for (size_t i = 0; i < prefix.size(); ++i) {
            char a = std::tolower(path[i]);
            char b = std::tolower(prefix[i]);
            if (a == '\\') a = '/';
            if (b == '\\') b = '/';
            if (a != b) return false;
        }
        return true;
    }

    static bool iequals(const std::string& a, const std::string& b) {
        if (a.size() != b.size()) return false;
        for (size_t i = 0; i < a.size(); ++i) {
            if (std::tolower(a[i]) != std::tolower(b[i])) return false;
        }
        return true;
    }

    static void normalize_path(std::string& path) {
        // Replace backslashes with forward slashes
        for (char& c : path) {
            if (c == '\\') c = '/';
        }

        // Collapse "/.." sequences
        while (true) {
            auto pos = path.find("/..");
            if (pos == std::string::npos || pos == 0) break;

            auto sep = path.rfind('/', pos - 1);
            if (sep == std::string::npos) {
                path.erase(0, pos + 3);
            } else {
                path.erase(sep, pos - sep + 3);
            }
        }

        // Remove duplicate slashes
        std::string clean;
        clean.reserve(path.size());
        for (size_t i = 0; i < path.size(); ++i) {
            if (i > 0 && path[i] == '/' && path[i - 1] == '/') continue;
            clean += path[i];
        }
        path = std::move(clean);
    }

    std::mutex mutex_;
    std::string default_prefix_;
    std::unordered_map<std::string, std::vector<std::shared_ptr<Device>>> mount_points_;
    std::vector<std::unique_ptr<FileHandle>> handles_;
};

// ---------------------------------------------------------------------------
// Example usage — how GTA IV's mount points would be set up
// ---------------------------------------------------------------------------
/*
    DeviceManager mgr;

    // Mount game data (read-only)
    mgr.RegisterDevice(
        std::make_shared<HostDevice>("game:", "/path/to/game/", true),
        "game:");

    // Mount update overlay (read-only, layered on top of game:)
    mgr.RegisterDevice(
        std::make_shared<HostDevice>("game:", "/path/to/update/", true),
        "game:");

    // Mount DLC
    mgr.RegisterDevice(
        std::make_shared<HostDevice>("dlc:", "/path/to/dlc/", true),
        "dlc:");

    // Mount save data (read-write)
    mgr.RegisterDevice(
        std::make_shared<HostDevice>("hdd:", "/path/to/saves/", false),
        "hdd:");

    // Set default device for relative paths
    mgr.SetDefaultPrefix("game:");

    // Open a resource — tries each handler prefix, finds device, opens file
    auto* handle = mgr.OpenResource("common/data/handling.dat");
    // This resolves to "game:common/data/handling.dat",
    // finds the update overlay device first (has the file),
    // opens and returns handle
*/

}  // namespace rage::vfs
```

---

## 11. Key Globals Summary

| Address | Type | Name | Description |
|-|-|-|-|
| `0x82B07278` | struct | VFS Handler Manager | 12 path prefix handlers + count + default index |
| `0x831AB940` | struct | Device Mount Table | Array of mounted device entries (276 bytes each) |
| `0x82B07EF0` | Device* | `g_local_device` | Default local filesystem device (game/update/hdd/dvd) |
| `0x82B08004` | Device* | `g_device_device` | Special "device:" prefix handler |
| `0x831AB948` | func* | `g_open_callback` | Optional callback invoked after successful open for filtering |
| `0x831AB950` | void* | `g_log_channel` | Logging channel for VFS debug output |
| `0x831AB93C` | bool | `g_use_fallback_device` | Whether to return fallback device when no mount matches |
| `0x82B093A8` | int32 | `g_max_handle_count` | Maximum entries in the file handle table |
| `0x831B5DD8` | HandleEntry* | `g_handle_table` | Base of 28-byte file handle entries |
| `0x831AB970` | mutex | `g_handle_table_lock` | Mutex protecting handle table access |

---

## 12. Hardcoded Device Prefix Strings

Computed from `lis`/`addi` pairs in `sub_82855460`:

| Address | Length | Likely String | Resolves To |
|-|-|-|-|
| `0x82085480` | 7 | `"device:"` | `g_device_device` (0x82B08004) |
| `0x8208599C` | 10 | `"platformfs"` | `g_device_device` (0x82B08004) |
| `0x82085A18` | 6 | `"cdvfs:"` | cdvfs subsystem (sub_828708C8) |
| `0x82085A10` | 6 | `"dvdfs:"` | `g_local_device` (0x82B07EF0) |
| `0x82085A08` | 7 | `"update:"` | `g_local_device` (0x82B07EF0) |
| `0x82085A00` | 5 | `"game:"` | `g_local_device` (0x82B07EF0) |
| `0x820859F8` | 4 | `"hdd:"` | `g_local_device` (0x82B07EF0) |
| `0x820859F4` | 3+colon | `"dvd"` + `:` | `g_local_device` (0x82B07EF0) |
