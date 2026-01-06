# RPF Extraction and Virtual File System Design

## Problem Statement

The GTA IV recompilation crashes because:
1. The game's streaming system passes NULL stream pointers to `sub_827E8420`
2. The current approach serves files from RPF archives using offset-based reads
3. The streaming system's internal state isn't properly synchronized
4. Stream objects at address `0x82003890` get corrupted with "common.rpf" string data

## Root Cause Analysis

### Stream Object Corruption
- Address `0x82003890` aliases with "common.rpf" string in image `.rdata` section
- Xbox 360 kept these regions separate; native recompilation does not
- The game's streaming system creates stream objects but they get corrupted

### Current File Serving Issues
1. `NtCreateFile` returns directory handles for `game:\`
2. `sub_829A1F00` tries to read RPF data using offset-based reads
3. Stream objects aren't properly initialized before use
4. Results in NULL or corrupted stream pointers

## Solution Architecture

### Virtual File System (VFS)

The VFS (`kernel/vfs.h` and `kernel/vfs.cpp`) provides:

```cpp
namespace VFS {
    void Initialize(const std::filesystem::path& extractedRoot);
    std::filesystem::path Resolve(const std::string& guestPath);
    bool Exists(const std::string& guestPath);
    bool IsDirectory(const std::string& guestPath);
    uint64_t GetFileSize(const std::string& guestPath);
}
```

### Path Mapping

| Game Request | Extracted Path |
|--------------|----------------|
| `game:\` | `<extracted_root>/` |
| `game:\common.rpf` | `<extracted_root>/common/` |
| `game:\common\shaders\*` | `<extracted_root>/common/shaders/*` |
| `fxl_final\` | `<extracted_root>/common/shaders/fxl_final/` |
| `game:\xbox360.rpf` | `<extracted_root>/xbox360/` |

### File Structure After Extraction

```
game/
├── default.xex
├── common.rpf (original, can be removed after extraction)
├── xbox360.rpf (original, can be removed after extraction)
├── audio.rpf (original, can be removed after extraction)
└── extracted/
    ├── common/
    │   ├── data/
    │   │   ├── action_table.csv
    │   │   ├── handling.dat
    │   │   └── ...
    │   ├── shaders/
    │   │   ├── fxl_final/
    │   │   │   ├── gta_default.fxc (89 files)
    │   │   │   └── ...
    │   │   └── preload.list
    │   └── text/
    ├── xbox360/
    │   ├── textures/
    │   └── models/
    └── audio/
```

## Implementation

### Phase 1: VFS Class (Completed)

**Files:**
- `kernel/vfs.h` - Header with VFS interface
- `kernel/vfs.cpp` - Implementation

**Features:**
- Path normalization (case-insensitive, backslash conversion)
- Special path mappings for GTA IV
- File index for fast lookups
- Statistics tracking

### Phase 2: NtCreateFile Integration

Update `imports.cpp` to use VFS:

```cpp
// In NtCreateFile
if (VFS::IsInitialized()) {
    auto resolved = VFS::Resolve(guestPath);
    if (!resolved.empty() && std::filesystem::exists(resolved)) {
        // Serve file directly from extracted directory
        return OpenHostFile(resolved, FileHandle);
    }
}
// Fall back to legacy RPF-based logic
```

### Phase 3: Stream Safety Wrapper

The `sub_827E8420` hook validates pointers before use:

```cpp
PPC_FUNC(sub_827E8420) {
    const uint32_t streamPtr = ctx.r3.u32;
    
    // Validate stream pointer
    if (streamPtr < 0x80000000 || streamPtr >= 0x90000000) {
        ctx.r3.s32 = -1;  // Return error
        return;
    }
    
    // Validate object pointer at stream[0]
    uint32_t objectPtr = PPC_LOAD_U32(streamPtr + 0);
    if (objectPtr != 0 && (objectPtr < 0x80000000 || objectPtr >= 0x90000000)) {
        ctx.r3.s32 = -1;
        return;
    }
    
    // Call original
    __imp__sub_827E8420(ctx, base);
}
```

## Benefits

| Current Approach | VFS Approach |
|------------------|--------------|
| Offset-based RPF reads | Direct file access |
| Complex stream management | Simple file handles |
| NULL pointer crashes | No streaming system involved |
| Hard to debug | Can inspect files directly |
| Complex code paths | Simple path resolution |

## Installation Flow Integration

During installation:

1. **Copy game files** to install directory
2. **Extract RPFs** recursively using `rpf_extractor.cpp`
3. **Initialize VFS** with extracted root path
4. **Validate extraction** by checking for key files (e.g., shaders)

## RPF2 Format Notes

- Magic: `0x52504632` ("RPF2")
- Header/TOC: Always plaintext (never encrypted)
- File data: Per-file AES-256-ECB encryption
- Some files: zlib compressed
- Key: Extractable from game executable

## Implementation Status (December 2024)

### Completed ✅

| File | Action | Purpose |
|------|--------|---------|
| `kernel/vfs.h` | Created | VFS interface - path resolution, file lookup |
| `kernel/vfs.cpp` | Created | VFS implementation - 277 lines |
| `docs/RPF_EXTRACTION_DESIGN.md` | Created | Design documentation |
| `CMakeLists.txt` | Modified | Added vfs.cpp to build |
| `kernel/imports.cpp` | Modified | Added VFS include + NtCreateFile integration |
| `main.cpp` | Modified | Added VFS include + initialization |

### Key Features Implemented

1. **VFS Path Resolution** - Maps Xbox 360 paths to extracted files:
   - `game:\` → `extracted/`
   - `fxl_final\` → `extracted/common/shaders/fxl_final/`
   - `common.rpf` → `extracted/common/`

2. **Direct File Serving** - NtCreateFile now tries VFS first:
   - If file exists in extracted directory, serves directly
   - Bypasses complex RPF offset-based reading
   - Falls back to legacy logic if VFS doesn't have the file

3. **Automatic Initialization** - VFS initializes at startup with the extracted root

### What This Solves

The VFS approach eliminates the root cause of NULL stream pointers:

| Before | After |
|--------|-------|
| Game reads RPF offsets → streaming system creates streams → NULL pointers → crash | Game requests file → VFS serves directly from extracted directory → no streaming system involved |

### Build & Test

```bash
cd /Users/Ozordi/Downloads/MarathonRecomp
cmake --build out/build/macos-debug --target LibertyRecomp -j8
```

## Mod Overlay Integration

The VFS now includes a **FusionFix-compatible mod overlay system** that allows mods to override extracted game files.

### How It Works

```
File Request Flow:
┌─────────────────────────────────────────────────────────────────┐
│  Game requests file (e.g., "common/data/handling.dat")          │
│                              │                                   │
│                              ▼                                   │
│  ┌─────────────────────────────────────────────────────────────┐│
│  │  1. ModOverlay::Resolve() - Check mod overlays FIRST        ││
│  │     Priority order: mods/update/ > update/ > base files     ││
│  └─────────────────────────────────────────────────────────────┘│
│                              │                                   │
│              ┌───────────────┴───────────────┐                   │
│              │                               │                   │
│         [Override Found]              [No Override]              │
│              │                               │                   │
│              ▼                               ▼                   │
│    Return mod file path        2. VFS::Resolve() - Base files   │
│                                              │                   │
│                                              ▼                   │
│                                   Return extracted file path     │
└─────────────────────────────────────────────────────────────────┘
```

### Files Added

| File | Purpose |
|------|---------|
| `kernel/mod_overlay.h` | Mod overlay API - overlay management, file resolution |
| `kernel/mod_overlay.cpp` | Implementation - FusionFix folder scanning, priority resolution |

### VFS Integration

The `VFS::Resolve()` function now checks mod overlays before base files:

```cpp
std::filesystem::path Resolve(const std::string& guestPath) {
    // 1. Check mod overlays FIRST (FusionFix-compatible)
    if (ModOverlay::IsInitialized()) {
        auto overridePath = ModOverlay::Resolve(stripped);
        if (!overridePath.empty() && std::filesystem::exists(overridePath)) {
            return overridePath;  // Mod file takes priority
        }
    }
    
    // 2. Fall back to base extracted files
    // ... existing VFS resolution logic ...
}
```

### Supported Overlay Locations

| Priority | Path | Description |
|----------|------|-------------|
| 100 | `mods/update/` | Highest priority - custom mod folder |
| 50 | `update/` | Standard FusionFix location |
| 40 | `GTAIV.EFLC.FusionFix/update/` | Alternative FusionFix location |
| 30 | `plugins/update/` | Plugins folder |
| 20 | `mods/` | Generic mods folder |

### FusionFix Path Mapping

Mod overlay paths are automatically mapped to match game paths:

| Overlay Path | Maps To |
|--------------|---------|
| `update/common/` | `common/` |
| `update/TLAD/` | `dlc/TLAD/` |
| `update/TBoGT/` | `dlc/TBoGT/` |
| `update/GTAIV.EFLC.FusionFix/shaders/` | `common/shaders/` |

### Example: Mod File Override

```
LibertyRecomp/
├── game/                           # Base extracted files
│   └── common/
│       └── data/
│           └── handling.dat        # Original file
└── update/                         # Mod overlay
    └── common/
        └── data/
            └── handling.dat        # MOD FILE - takes priority!
```

When the game requests `common/data/handling.dat`, the VFS returns the mod version.

For complete mod documentation, see [MOD_SUPPORT.md](MOD_SUPPORT.md).

---

## Future Improvements

1. **Lazy extraction**: Extract files on first access instead of upfront
2. **Memory-mapped files**: Use mmap for large files ✅ (Implemented)
3. **Parallel extraction**: Multi-threaded RPF unpacking
4. **Compression support**: Keep files compressed, decompress on read
5. **IMG/RPF Loader**: Load modified archive files from mods (like FusionFix)
6. **Hot reload**: Detect mod file changes and reload without restart
