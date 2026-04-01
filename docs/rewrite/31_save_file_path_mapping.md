# 31 - Save File Path Mapping: Xbox API to Host Filesystem

## Summary

This document traces the full path from Xbox 360 save API calls through the
VFS / content system to actual files on the host macOS filesystem.

---

## 1. The Two Path Systems (CONFLICT)

There are **two separate path systems** that both deal with saves, and they
point to **different directories**:

### A. `paths.h` GetSavePath() (used by XamContentCreateEx)
```
GetSavePath(true)  ->  GetUserPath() / "save"
                   ->  ~/Library/Application Support/LibertyRecomp/save/
```

### B. `save_system.cpp` SaveSystem::Initialize() root registrations
```
XamRootCreate("SaveData", saveDirStr)   // saveDirStr = GetSavePath(true) = .../save/
XamRootCreate("save",     saveDirStr)
XamRootCreate("saves",    saveDirStr)
```

All three roots point to the same directory: `.../save/`.

**On disk, there are TWO directories:**
- `~/Library/Application Support/LibertyRecomp/save/`  (created by save_system.cpp)
- `~/Library/Application Support/LibertyRecomp/saves/` (created by unknown — NOT by code in this repo)

Both contain an identical `SGTA400` file (782,336 bytes = 0xBF000 bytes = 764.0 KB).

---

## 2. Full Path Trace: Xbox Content API to Disk

### Step 1: Game calls XamContentCreateEx

The game calls `sub_829A1C38` (content creation wrapper) which calls
`XamContentCreateEx` with:
- `szRootName` = a root name string (e.g., "SaveData")
- `pContentData->szFileName` = "SGTA400" (for slot 0)
- `pContentData->dwContentType` = 1 (XCONTENTTYPE_SAVEDATA)
- `dwContentFlags` = CREATE_ALWAYS or OPEN_EXISTING

### Step 2: XamContentCreateEx registers root mapping

In `xam.cpp` line 380-493:

**CREATE_ALWAYS path (new save):**
1. Checks `gContentRegistry[0]` (SAVEDATA = type 1, index 0) for "SGTA400"
2. If not found:
   - `rootPath = GetSavePath(true)` = `~/Library/Application Support/LibertyRecomp/save/`
   - Calls `XamRegisterContent(*pContentData, root)` — stores szRoot in registry
   - Calls `std::filesystem::create_directory(rootPath)` — creates save dir
   - Calls `XamRootCreate(szRootName, root)` — maps root name to host path
3. If found: reads szRoot from existing registry entry

**OPEN_EXISTING path:**
1. If content exists in registry: maps root name to stored szRoot
2. If not found: returns ERROR_PATH_NOT_FOUND

### Step 3: Root name stored in gRootMap

`gRootMap` (xxHashMap) maps root name strings to host directory paths:
```
"SaveData"  ->  "/Users/Ozordi/Library/Application Support/LibertyRecomp/save"
"save"      ->  "/Users/Ozordi/Library/Application Support/LibertyRecomp/save"
"saves"     ->  "/Users/Ozordi/Library/Application Support/LibertyRecomp/save"
```

### Step 4: Game opens file via CreateFileA

After `XamContentCreateEx` returns, the game opens files using Xbox paths like:
```
SaveData:\SGTA400
```

This enters `XCreateFileA` (file_system.cpp line 79), which calls
`FileSystem::ResolvePath("SaveData:\\SGTA400", true)`.

### Step 5: ResolvePath resolves the root

In `file_system.cpp` line 496:
1. Finds `:\\` at position 8, so `root = "SaveData"`, `pathSuffix = "SGTA400"`
2. Calls `XamGetRootPath("SaveData")` which looks up gRootMap
3. Returns `"/Users/Ozordi/Library/Application Support/LibertyRecomp/save"`
4. Builds: `builtPath = "/Users/Ozordi/Library/Application Support/LibertyRecomp/save/SGTA400"`

### Step 6: Final host path

```
~/Library/Application Support/LibertyRecomp/save/SGTA400
```

---

## 3. Save File on Disk

```
Path:   ~/Library/Application Support/LibertyRecomp/save/SGTA400
Size:   782,336 bytes (0xBF000 = 764.0 KB)
Date:   Jun 21 2008 (original Xbox 360 save file imported via copy)
```

This is a real GTA IV Xbox 360 save file. The date (2008) matches the game's
original release window.

---

## 4. Content Registry Structure

### XCONTENT_DATA (xbox.h line 332-338)
```c
struct XCONTENT_DATA {
    be<uint32_t> DeviceID;                               // +0x00
    be<uint32_t> dwContentType;                          // +0x04
    be<uint16_t> szDisplayName[128];                     // +0x08 (256 bytes, UTF-16BE)
    char         szFileName[42];                         // +0x108
};
// Xenia says size = 0x134 = 308 bytes (2 bytes padding at end)
```

### XHOSTCONTENT_DATA (xbox.h line 340-344)
```c
struct XHOSTCONTENT_DATA : XCONTENT_DATA {
    std::string szRoot;   // Host-only: filesystem path to content root
};
```

### gContentRegistry layout
```
gContentRegistry[0] = SAVEDATA (type 1)  — keyed by szFileName hash
gContentRegistry[1] = DLC      (type 2)
gContentRegistry[2] = RESERVED (type 3)
```

For saves, `gContentRegistry[0]["SGTA400"].szRoot` = host save directory path.

---

## 5. SaveSystem::Initialize() Sequence (main.cpp line 265)

Called during `KiSystemStartup()` before game code runs:

1. `GetSaveDirectory()` -> `GetSavePath(true)` -> `.../save/`
2. `create_directories(saveDir)` — ensures `.../save/` exists
3. `EnumerateSaveFiles()` — scans for files matching "SGTA4*"
4. For each existing slot (0-15): `RegisterSaveSlot(slot)`
   - Calls `XamRegisterContent(XamMakeContent(1, "SGTA4XX"), saveDirStr)`
   - This populates `gContentRegistry[0]` with szRoot = save directory
5. Creates root mappings: "SaveData", "save", "saves" -> save directory

---

## 6. Content Enumeration Flow

When the game enumerates saves:

1. `sub_829A1CB8` -> `XamContentCreateEnumerator(0, DeviceID, XCONTENTTYPE_SAVEDATA, ...)`
2. Returns enumerator handle over `gContentRegistry[0]` values
3. `XamEnumerate()` iterates, copying XCONTENT_DATA structs to guest memory
4. Game reads szFileName from each entry (e.g., "SGTA400")
5. Game calls `XamContentCreateEx(0, "SaveData", &contentData, OPEN_EXISTING, ...)`
6. This creates root mapping: "SaveData" -> save directory
7. Game opens: `SaveData:\SGTA400` -> ResolvePath -> host path

---

## 7. Critical Observations

### A. Dual save directories on disk
Both `save/` and `saves/` exist. The code only creates/uses `save/` (via
`GetSavePath`). The `saves/` directory may have been created by an older
version of the code or manual testing. The `XamRootCreate("saves", ...)` call
in `save_system.cpp` maps the root name "saves" to the `save/` directory
path, not to a `saves/` directory.

### B. The VFS (vfs.cpp) is NOT involved in save I/O
The VFS system in `vfs.cpp` is for game asset resolution (RPF contents,
shaders, textures). Save file I/O goes through:
```
XamContentCreateEx  ->  gRootMap registration
CreateFileA         ->  FileSystem::ResolvePath  ->  XamGetRootPath  ->  fstream
```
The VFS file index (g_fileIndex) only indexes the extracted game root, not the
save directory.

### C. File size: 782,336 bytes (0xBF000)
This is the size of the actual SGTA400 file on disk. If the game's save state
machine expects a different size, this mismatch could cause looping. GTA IV
Xbox 360 save files are typically in this size range, but the exact expected
size depends on the game version (pre-TU vs post-TU saves may differ).

### D. XamContentCreateEx CREATE_ALWAYS creates the DIRECTORY, not the FILE
When mode = CREATE_ALWAYS, `XamContentCreateEx` only:
- Creates the save directory (`.../save/`)
- Registers the content in gContentRegistry
- Maps the root name

It does NOT create or write the actual save file. The game writes save data
separately via `CreateFileA` + `WriteFile` after the content container is
"opened."

### E. No file size validation in content system
Neither `XamContentCreateEx` nor `XamContentCreateEnumerator` validates or
reports file sizes. The `uliContentSize` parameter in `XamContentCreateEx` is
accepted but never used. File size is only discoverable via `GetFileSize` /
`GetFileSizeEx` after opening the file handle.

---

## 8. Path Mapping Summary Table

| Xbox Path | Intermediate | Host Path |
|-----------|-------------|-----------|
| `XamContentCreateEx("SaveData", SGTA400, SAVEDATA)` | `gRootMap["SaveData"] = .../save/` | `~/Library/Application Support/LibertyRecomp/save/` |
| `SaveData:\SGTA400` | `ResolvePath -> XamGetRootPath("SaveData") + "/SGTA400"` | `~/Library/Application Support/LibertyRecomp/save/SGTA400` |
| `save:\SGTA400` | `ResolvePath -> XamGetRootPath("save") + "/SGTA400"` | `~/Library/Application Support/LibertyRecomp/save/SGTA400` |
| `saves:\SGTA400` | `ResolvePath -> XamGetRootPath("saves") + "/SGTA400"` | `~/Library/Application Support/LibertyRecomp/save/SGTA400` |

---

## 9. Files Referenced

- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/xam.cpp` — XamContentCreateEx, XamRootCreate, gRootMap, gContentRegistry
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/xam.h` — API declarations
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/io/file_system.cpp` — ResolvePath, XCreateFileA (line 384, 79)
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/save_system.cpp` — SaveSystem::Initialize, root registrations
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/save_system.h` — save vtable addresses
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/user/paths.h` — GetSavePath, GetSaveFilePath, GetGamePath
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/user/paths.cpp` — BuildUserPath (macOS: ~/Library/Application Support/LibertyRecomp)
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/save_hooks.cpp` — PPC hook wrappers for save functions
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/vfs.cpp` — VFS (NOT used for saves)
- `/Users/Ozordi/Downloads/LibertyRecomp/tools/XenonRecomp/XenonUtils/xbox.h` — XCONTENT_DATA, XHOSTCONTENT_DATA structs
