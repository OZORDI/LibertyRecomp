# Virtual File System & Archive Loading

## Overview

LibertyRecomp uses a Virtual File System (VFS) to serve game files from extracted directories instead of reading from RPF archives at runtime. This design simplifies file access and enables mod support.

## Architecture

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

### File Structure

```
game/
├── default.xex
├── common/           # Extracted from common.rpf
│   ├── data/
│   │   ├── handling.dat
│   │   └── ...
│   ├── shaders/
│   │   └── fxl_final/
│   └── text/
├── xbox360/          # Extracted from xbox360.rpf
│   ├── textures/
│   └── models/
└── audio/            # Extracted from audio.rpf
```

## Archive Support

### RPF Archives (Install-time)

RPF2 archives are extracted during installation:
- **Magic:** `0x52504632` ("RPF2")
- **Encryption:** AES-256-ECB (16 rounds) for file data
- **Compression:** Raw deflate (zlib)
- **Implementation:** `install/rpf_extractor.cpp`

### RPF Loader (Runtime)

Mods can provide `.rpf` files that are extracted on-demand:
- Detected in mod overlay directories
- Lazy extraction on first file access
- Cached in memory or temp directory
- **Implementation:** `kernel/io/rpf_loader.cpp`

### IMG Archives

GTA IV uses IMG v3 archives for game assets:
- **Magic:** `0xA94E2A52`
- **Block size:** 2048 bytes
- Mods can override IMG contents via folder overlays
- **Implementation:** `kernel/io/img_loader.cpp`

## Mod Overlay System

The VFS includes FusionFix-compatible mod loading:

```
File Request Flow:
┌─────────────────────────────────────────────────────────────────┐
│  Game requests file (e.g., "common/data/handling.dat")          │
│                              │                                   │
│                              ▼                                   │
│  1. ModOverlay::Resolve() - Check mod overlays FIRST            │
│     Priority: mods/update/ > update/ > base files               │
│                              │                                   │
│              ┌───────────────┴───────────────┐                   │
│         [Override Found]              [No Override]              │
│              │                               │                   │
│              ▼                               ▼                   │
│    Return mod file path        2. VFS::Resolve() - Base files   │
└─────────────────────────────────────────────────────────────────┘
```

### Supported Features

| Feature | Status | Description |
|---------|--------|-------------|
| Loose file overrides | ✅ | Direct file replacement |
| IMG folder merging | ✅ | `update/path/archive.img/` folders |
| RPF runtime loading | ✅ | On-demand extraction |
| PC→Xbox texture conversion | ✅ | Auto-convert .wtd → .xtd |

### Overlay Locations

| Priority | Path | Description |
|----------|------|-------------|
| 100 | `mods/update/` | Highest priority |
| 50 | `update/` | Standard FusionFix location |
| 40 | `GTAIV.EFLC.FusionFix/update/` | Alternative location |
| 30 | `plugins/update/` | Plugins folder |
| 20 | `mods/` | Generic mods folder |

### Texture Auto-Conversion

When a mod provides PC textures (.wtd), they are automatically converted:
1. Endian swap (little → big endian)
2. Morton/Z-order swizzle for Xbox 360 GPU
3. Resource type update (0x08 → 0x07)
4. Cached for subsequent access

## Implementation Files

| File | Purpose |
|------|---------|
| `kernel/vfs.h/cpp` | Virtual file system core |
| `kernel/mod_overlay.h/cpp` | FusionFix-compatible mod loading |
| `kernel/io/rpf_loader.h/cpp` | Runtime RPF extraction |
| `kernel/io/img_loader.h/cpp` | IMG archive merging |
| `kernel/io/texture_convert.h/cpp` | PC↔Xbox texture conversion |
| `install/rpf_extractor.h/cpp` | Install-time RPF extraction |

## Usage

### Adding a Mod

1. Create `update/` folder next to game directory
2. Place mod files mirroring game structure
3. Launch game - mods automatically loaded

### Example

```
LibertyRecomp/
├── game/                           # Base files
│   └── common/data/handling.dat
└── update/                         # Mod overlay
    └── common/data/handling.dat    # Overrides base
```

For complete mod documentation, see [MOD_SUPPORT.md](MOD_SUPPORT.md).
