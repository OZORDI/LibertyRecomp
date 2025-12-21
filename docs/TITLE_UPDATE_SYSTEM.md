# Xbox 360 Title Update System

## Overview

This document describes how Xbox 360 Title Updates work and how to integrate them into the LibertyRecomp installation pipeline.

**Critical Flow:** Title Updates are applied to the XEX **before** static recompilation. The patched XEX is then fed to XenonRecomp, which generates C++ code that gets compiled to the user's target platform.

```
Installation Flow:
┌─────────────┐     ┌─────────────┐     ┌──────────────┐     ┌─────────────┐
│ User selects│     │ Apply XEXP  │     │ XenonRecomp  │     │ Compile C++ │
│ Title Update│ ──▶ │ to XEX      │ ──▶ │ (PPC → C++)  │ ──▶ │ to platform │
└─────────────┘     └─────────────┘     └──────────────┘     └─────────────┘
       │                   │                    │                    │
   TU0008.stfs        patched.xex         ppc_recomp.cpp      LibertyRecomp.app
```

---

## 1. Title Update File Structure

### STFS Container

Xbox 360 Title Updates are distributed as **STFS** (Secure Transacted File System) packages. These are the same format used for save games, DLC, and other Xbox 360 content.

**File signatures:**
- `CON ` (0x434F4E20) - Console-signed (user content)
- `LIVE` (0x4C495645) - Xbox Live signed
- `PIRS` (0x50495253) - Publisher-signed (read-only)

**Structure:**
```
STFS Package
├── Header (0x971A or 0xB000 bytes depending on type)
│   ├── Magic (4 bytes)
│   ├── Signatures
│   ├── License entries
│   ├── Content metadata
│   │   ├── Title ID
│   │   ├── Content Type (0x000B0000 = Title Update)
│   │   ├── Version numbers
│   │   └── Display name/description
│   └── Volume descriptor
├── Hash tables
└── File data blocks
    └── Embedded files (XEXP patches, etc.)
```

### XEXP Patch Files

Inside the STFS container are `.xexp` files - binary patches for XEX executables.

**Key characteristics:**
- Filename matches the target XEX (e.g., `default.xexp` patches `default.xex`)
- Contains delta or full replacement data
- XEX header flags indicate patch type:
  - Bit 4: Module Patch
  - Bit 5: Patch Full (complete replacement)
  - Bit 6: Patch Delta (differential)

---

## 2. Implementation (Native C++)

LibertyRecomp includes a native C++ implementation for Title Update handling, adapted from Xenia (BSD-3-Clause).

### Files

```
LibertyRecomp/install/xbox360/
├── stfs_parser.h/cpp          # STFS container parsing
├── xex_patcher.h/cpp          # XEX/XEXP patch application
└── title_update_manager.h/cpp # High-level TU management
```

### StfsParser Class

Parses Xbox 360 STFS packages (Title Updates, DLC, saves):

```cpp
#include "install/xbox360/stfs_parser.h"

liberty::install::StfsParser parser;
if (parser.Open("path/to/update.stfs")) {
    if (parser.IsTitleUpdate()) {
        auto info = parser.GetTitleUpdateInfo();
        printf("TU v%u for Title ID %08X\n", info.version, info.titleId);
        
        // Extract the XEXP patch file
        std::vector<uint8_t> xexpData;
        parser.ExtractFile("default.xexp", xexpData);
    }
}
```

### XexPatcher Class

Applies XEXP patches to XEX executables:

```cpp
#include "install/xbox360/xex_patcher.h"

liberty::install::XexPatcher patcher;
patcher.LoadBaseXex("default.xex");
patcher.LoadPatch("default.xexp");

if (patcher.IsPatchCompatible()) {
    auto result = patcher.ApplyPatch();
    if (result.success) {
        patcher.SavePatchedXex("patched.xex");
    }
}
```

### TitleUpdateManager Class

High-level API for the installer wizard:

```cpp
#include "install/xbox360/title_update_manager.h"

liberty::install::TitleUpdateManager manager;

// Scan for TU files
int count = manager.ScanDirectory("path/to/updates/");

// Display available updates
for (const auto& update : manager.GetDetectedUpdates()) {
    printf("%s\n", TitleUpdateManager::GetUpdateDisplayName(update.info).c_str());
}

// Select an update
manager.SelectUpdate(0);  // Select first (highest version)

// Apply during installation
auto result = manager.ApplySelectedUpdate(baseXexPath, outputDir);
if (result.success) {
    // Use result.patchedXexPath for XenonRecomp
}
```

### Fallback: XexTool Binary

For complex delta patches, the implementation falls back to `xextool`:

```bash
xextool -p patch.xexp -o output.xex input.xex
```

On macOS/Linux, this requires Wine.

---

## 3. Installer Integration

The update selection happens **during installation**, before recompilation.

**Installer Flow:**
1. User provides game files (ISO/folder)
2. User provides Title Update files (optional)
3. **NEW SCREEN:** User selects which update to apply (or none)
4. Installer extracts XEX from game files
5. If update selected: Apply XEXP patch to XEX
6. Run XenonRecomp on (patched) XEX → generates `ppc_recomp.*.cpp`
7. Compile generated C++ to target platform
8. Installation complete

**UI Screen:**
```
┌─────────────────────────────────────────────────────────────┐
│                    SELECT TITLE UPDATE                       │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  ○ No Update (Original v1.0)                                │
│                                                              │
│  ● Title Update v8 (TU0008)                    [SELECTED]   │
│    Final update - stability improvements                     │
│                                                              │
│  ○ Title Update v7 (TU0007)                                 │
│    Multiplayer fixes                                         │
│                                                              │
├─────────────────────────────────────────────────────────────┤
│  [BACK]                                        [CONTINUE]   │
└─────────────────────────────────────────────────────────────┘
```

**Key Points:**
- Selection happens once during install
- Changing updates requires reinstallation
- Selected version is baked into the compiled binary
- Store selected version in install metadata for display

---

## 4. GTA IV Title Update Details

### Known Title IDs
- `545407F2` - GTA IV (USA/EU)
- `545407EE` - GTA IV (Japan)

### Update Locations on Console
- `Hdd1:/Content/0000000000000000/<TITLE_ID>/000B0000/`
- `Hdd1:/Cache/` (uppercase format)

### GTA IV Update History
| Version | Notable Fixes |
|---------|---------------|
| TU8 | Final update, stability fixes |
| TU7 | Multiplayer fixes |
| TU6 | Various bug fixes |
| TU5 | Performance improvements |

---

## 5. File Hashing Strategy

To identify and validate update files:

```cpp
std::string ComputeTUHash(const std::string& path) {
    // SHA1 of entire file for identification
    // Compare against known-good hashes
}

std::map<std::string, int> g_knownTUHashes = {
    {"abc123...", 8},  // TU8 hash -> version 8
    {"def456...", 7},  // TU7 hash -> version 7
    // etc.
};
```

---

## 6. Implementation Status

### Completed ✅

- [x] **StfsParser** - Native STFS container parsing (magic, metadata, file extraction)
- [x] **XexPatcher** - XEX/XEXP patch framework (full patches work, delta falls back to xextool)
- [x] **TitleUpdateManager** - High-level API for scanning, selecting, and applying updates
- [x] CMake integration for new source files
- [x] Build verification

### TODO

- [ ] Add "Select Title Update" UI page to installer wizard
- [ ] Wire TitleUpdateManager into installer pipeline
- [ ] Store selected version in install metadata
- [ ] Display installed version in app UI
- [ ] Full native LZX delta decompression (currently falls back to xextool)

---

## 7. References

- **STFS Format:** https://free60.org/System-Software/Formats/STFS/
- **XEX Format:** https://free60.org/System-Software/Formats/XEX/
- **Xenia Source:** `tools/xenia/` (BSD-3-Clause, adapted for our implementation)
- **MS-PATCH (LZX Delta):** https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-patch/
- **ConsoleMods Wiki:** https://consolemods.org/wiki/Xbox_360:Files_and_Directories
