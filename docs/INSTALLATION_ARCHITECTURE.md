# Liberty Recompiled - Installation Architecture

## Overview

This document describes the comprehensive installation flow that handles:
1. ISO to folder extraction (native implementation)
2. RPF archive parsing and shader extraction
3. Platform-native shader cache generation
4. Cross-platform support (Windows, Linux, macOS)

---

## Platform-Specific Install Directories

### Windows
```
%LOCALAPPDATA%\LibertyRecomp\
├── game\                    # Extracted game files
│   ├── default.xex
│   ├── common\              # Extracted from common.rpf
│   ├── xbox360\             # Extracted from xbox360.rpf
│   └── audio\               # Extracted from audio.rpf
├── dlc\                     # DLC content
│   ├── TLAD\                # The Lost and Damned
│   └── TBOGT\               # The Ballad of Gay Tony
├── shader_cache\            # Compiled DXIL shaders
├── saves\                   # Save data
└── temp\                    # Temporary extraction files
```

**Path:** `C:\Users\<username>\AppData\Local\LibertyRecomp\`

### Linux
```
~/.local/share/LibertyRecomp/
├── game/                    # Extracted game files
├── dlc/                     # DLC content
│   ├── TLAD/
│   └── TBOGT/
├── shader_cache/            # Compiled SPIR-V shaders
├── saves/
└── temp/
```

**Path:** `/home/<username>/.local/share/LibertyRecomp/`
**XDG compliant:** `$XDG_DATA_HOME/LibertyRecomp/`

### macOS
```
~/Library/Application Support/LibertyRecomp/
├── game/                    # Extracted game files
├── dlc/                     # DLC content
│   ├── TLAD/
│   └── TBOGT/
├── shader_cache/            # Compiled AIR shaders
├── saves/
└── temp/
```

**Path:** `/Users/<username>/Library/Application Support/LibertyRecomp/`

---

## Launch Arguments

Liberty Recompiled supports several command-line arguments for troubleshooting and advanced use:

| Argument | Description |
|----------|-------------|
| `--install` | Force the installation wizard to run, even if game is already installed. Useful if game files were modified or corrupted. |
| `--install-dlc` | Force the DLC installation wizard to run. Allows adding DLC without reinstalling the base game. |
| `--install-check` | Run file integrity verification. Checks all installed files against known hashes. |
| `--skip-logos` | Skip intro logos on game startup. |
| `--use-cwd` | Use current working directory instead of executable path. |
| `--sdl-video-driver <driver>` | Force specific SDL video driver (e.g., `x11`, `wayland`, `metal`). |

### Examples

```bash
# Force reinstallation
./LibertyRecomp --install

# Add DLC to existing installation
./LibertyRecomp --install-dlc

# Verify file integrity
./LibertyRecomp --install-check
```

---

## Installation Flow

```
┌─────────────────────────────────────────────────────────────────────────┐
│                          INSTALLATION WIZARD                            │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  1. SELECT GAME SOURCE                                                  │
│     User selects:                                                       │
│     ├── ISO file (.iso)                                                │
│     ├── Extracted folder                                               │
│     └── XContent package                                               │
│                                                                         │
│  2. SELECT TITLE UPDATE (Optional)                                      │
│     ├── Scans 'GAME UPDATES/' folder for STFS packages                 │
│     ├── Scans install path 'updates/' folder                           │
│     ├── User selects: No Update (v1.0) or detected TU version          │
│     └── Selected version stored in config for reference                │
│                                                                         │
│  3. SELECT DLC (Optional)                                               │
│     User provides DLC sources:                                         │
│     ├── STFS packages (CON/LIVE/PIRS)                                  │
│     ├── ZIP files containing STFS packages                             │
│     └── Extracted DLC folders                                          │
│     Supported DLC:                                                      │
│     ├── The Lost and Damned (TLAD)                                     │
│     └── The Ballad of Gay Tony (TBOGT)                                 │
│                                                                         │
│  4. DETECT SOURCE TYPE                                                  │
│     ┌─────────────────┐                                                │
│     │ Is it an ISO?   │──YES──► Use native ISOFileSystem               │
│     └────────┬────────┘         └── Parse and extract directly         │
│              │NO                                                        │
│              ▼                                                          │
│     ┌─────────────────┐                                                │
│     │ Is it XContent? │──YES──► Use XContentFileSystem                 │
│     └────────┬────────┘                                                │
│              │NO                                                        │
│              ▼                                                          │
│     Use DirectoryFileSystem (already extracted)                        │
│                                                                         │
│  5. COPY GAME FILES                                                     │
│     Copy essential files to platform install directory:                │
│     ├── default.xex                                                    │
│     ├── xbox360.rpf                                                    │
│     ├── common.rpf                                                     │
│     └── other required files                                           │
│                                                                         │
│  6. APPLY TITLE UPDATE (if selected)                                    │
│     ├── Extract .xexp from STFS container                              │
│     └── Apply patch to default.xex                                     │
│                                                                         │
│  7. INSTALL DLC (if selected)                                           │
│     For each DLC source:                                               │
│     ├── Detect DLC type (TLAD or TBOGT)                                │
│     ├── Extract from STFS or copy from folder                          │
│     └── Install to <install_path>/dlc/<DLC_NAME>/                      │
│                                                                         │
│  8. EXTRACT RPF ARCHIVES                                                │
│     Extract RPF contents using native RPF2 parser:                     │
│     ├── common.rpf → game/common/                                      │
│     ├── xbox360.rpf → game/xbox360/                                    │
│     ├── audio.rpf → game/audio/                                        │
│     └── Nested RPFs extracted recursively                              │
│                                                                         │
│  9. SCAN FOR SHADERS                                                    │
│     Look for .fxc files in:                                            │
│     ├── Extracted game folders directly                                │
│     └── Inside RPF archives (extract if needed)                        │
│                                                                         │
│  10. COMPILE SHADER CACHE                                               │
│      Detect platform and compile:                                      │
│      ├── Windows: HLSL → DXIL (DXC)                                    │
│      ├── Linux: HLSL → SPIR-V (DXC)                                    │
│      └── macOS: HLSL → AIR (Metal compiler)                            │
│                                                                         │
│  11. MULTIPLAYER SETUP (Optional)                                       │
│      User selects multiplayer backend:                                 │
│      ├── Community Server (Recommended)                                │
│      ├── Firebase (Self-Hosted)                                        │
│      └── LAN Only                                                      │
│                                                                         │
│  12. FINALIZE                                                           │
│      ├── Write shader_cache.marker                                     │
│      ├── Save installed TU version to config                           │
│      ├── Save multiplayer backend choice to config                     │
│      ├── Clean up temp files                                           │
│      └── Signal completion                                             │
│                                                                         │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## DLC System

### Supported DLC

| DLC | Install Directory | Detection Method |
|-----|-------------------|------------------|
| The Lost and Damned (TLAD) | `<install_path>/dlc/TLAD/` | Contains `tlad.rpf` or folder named TLAD |
| The Ballad of Gay Tony (TBOGT) | `<install_path>/dlc/TBOGT/` | Contains `tbogt.rpf` or folder named TBOGT |

### DLC Source Formats

The installer accepts DLC in several formats:

| Format | Description |
|--------|-------------|
| STFS (CON/LIVE/PIRS) | Xbox 360 content package (direct from console) |
| ZIP containing STFS | Compressed archive with STFS package inside |
| Extracted folder | Pre-extracted DLC directory |

### DLC Detection

The installer automatically detects which DLC is being installed by:
1. Checking for `tlad.rpf` or `tbogt.rpf` inside the package
2. Checking folder names for keywords like "lost", "tlad", "ballad", "tbogt"
3. Checking subfolder structure (TLAD/ or TBOGT/)

### Runtime DLC Loading

At runtime, the game scans `<install_path>/dlc/` for DLC directories:
```cpp
for (auto& file : std::filesystem::directory_iterator(GetGamePath() / "dlc", ec))
{
    if (file.is_directory())
    {
        // Register DLC content
        XamRegisterContent(XamMakeContent(XCONTENTTYPE_DLC, fileName), filePath);
    }
}
```

---

## Title Update System

GTA IV Xbox 360 Title Updates are distributed as STFS packages containing `.xexp` patch files.

### Title Update Detection

The installer automatically scans for Title Updates in:
- `GAME UPDATES/` folder (relative to application)
- `<install_path>/updates/` folder

### Supported Formats

| Format | Description |
|--------|-------------|
| STFS (CON/LIVE/PIRS) | Xbox 360 content package containing .xexp |
| Raw .xexp | Direct patch file (less common) |

### Components

```
LibertyRecomp/install/xbox360/
├── stfs_parser.h/cpp          # STFS container parsing
├── xex_patcher.h/cpp          # XEX/XEXP patch application  
└── title_update_manager.h/cpp # High-level TU management
```

### Usage

```cpp
#include "install/xbox360/title_update_manager.h"

liberty::install::TitleUpdateManager manager;

// Scan for updates
manager.ScanDirectory("GAME UPDATES/");

// Display available updates
for (const auto& update : manager.GetDetectedUpdates()) {
    printf("%s\n", TitleUpdateManager::GetUpdateDisplayName(update.info).c_str());
}

// Select and apply
manager.SelectUpdate(0);
auto result = manager.ApplySelectedUpdate(baseXexPath, outputDir);
```

See [TITLE_UPDATE_SYSTEM.md](TITLE_UPDATE_SYSTEM.md) for detailed documentation.

---

## Key Components

### Platform Paths (`install/platform_paths.h/cpp`)

Cross-platform directory resolution:

```cpp
#include "install/platform_paths.h"

// Get platform-specific install directory
auto installDir = PlatformPaths::GetInstallDirectory();
// Windows: %LOCALAPPDATA%\LibertyRecomp\
// Linux:   ~/.local/share/LibertyRecomp/
// macOS:   ~/Library/Application Support/LibertyRecomp/

auto gameDir = PlatformPaths::GetGameDirectory();        // <install>/game/
auto shaderDir = PlatformPaths::GetShaderCacheDirectory(); // <install>/shader_cache/
auto savesDir = PlatformPaths::GetSavesDirectory();       // <install>/saves/
```

### ISO Extractor (`install/iso_extractor.h/cpp`)

**Native Xbox 360 ISO extraction** - no external tools required:

```cpp
#include "install/iso_extractor.h"

if (IsoExtractor::IsIsoFile(sourcePath))
{
    auto result = IsoExtractor::Extract(
        sourcePath,
        PlatformPaths::GetTempDirectory(),
        [](float progress) { /* update UI */ }
    );
    
    if (result.success)
    {
        // Use result.extractedPath as game source
    }
}
```

The ISO extractor uses `ISOFileSystem` internally to parse Xbox 360 disc images directly, supporting multiple ISO formats (XGD1, XGD2, XGD3).

### RPF Extractor (`install/rpf_extractor.h/cpp`)

**Native RAGE Package File extraction** with AES-256 decryption:

```cpp
#include "install/rpf_extractor.h"

// Check if file has encrypted content
if (RpfExtractor::HasEncryptedFiles(rpfPath))
{
    // Load AES key (32 bytes for AES-256)
    std::vector<uint8_t> key;
    RpfExtractor::LoadAesKey("aes_key.bin", key);
}

// Scan game directory and extract all shaders
auto result = RpfExtractor::ScanAndExtractShaders(
    gameDir,
    outputDir,
    aesKey,
    [](float progress) { /* update UI */ }
);

printf("Found %zu shader files\n", result.fxcFiles.size());
```

### XContent File System (`install/xcontent_file_system.h/cpp`)

**Native STFS/SVOD parsing** for Xbox 360 content packages:

```cpp
#include "install/xcontent_file_system.h"

// Check if file is an Xbox content package
if (XContentFileSystem::check(packagePath))
{
    auto vfs = XContentFileSystem::create(packagePath);
    
    // List all files
    for (const auto& file : vfs->getFileList())
    {
        printf("%s\n", file.c_str());
    }
    
    // Load a specific file
    std::vector<uint8_t> data;
    vfs->load("default.xex", data);
}
```

Supports CON, LIVE, and PIRS package types.

---

## RPF2 Archive Format (GTA IV)

### Header (20 bytes, ALWAYS plaintext, LITTLE-ENDIAN)
```
Offset  Size  Field
0x00    4     Magic: 0x52504632 ("RPF2")
0x04    4     TOC Size (bytes)
0x08    4     Entry Count
0x0C    4     Unknown
0x10    4     Encryption Flag (0 = unencrypted, non-zero = encrypted files)
```

### Key Facts
- **Header is ALWAYS plaintext** - never encrypted
- **TOC is ALWAYS plaintext** - starts at offset 0x800 (2048 bytes)
- **Encryption is per-file** - only individual file data blocks are encrypted
- **Algorithm**: AES-256-ECB (16 rounds, no IV, no padding)
- **Compression**: Raw deflate (zlib without header)

### Shader Location
```
common.rpf/
└── shaders/
    └── fxl_final/     ← 89 .fxc files
        ├── gta_default.fxc
        ├── gta_vehicle_paint1.fxc
        ├── gta_ped.fxc
        └── ...
```

---

## Shader Pipeline Integration

After extraction, shaders go through the conversion pipeline:

1. **RAGE FXC Parsing** - Extract Xbox 360 shader containers from `.fxc` files
2. **XenosRecomp** - Convert Xbox 360 Xenos shaders to HLSL
3. **Platform Compilation**:
   - Windows: DXC → DXIL
   - Linux: DXC → SPIR-V
   - macOS: Metal compiler → AIR

---

## Error Handling

### ISO Extraction Failures
- Verify ISO file is a valid Xbox 360 image (magic: `MICROSOFT*XBOX*MEDIA`)
- Ensure sufficient disk space for extraction
- Check file permissions on output directory

### RPF Extraction Failures
- For encrypted RPFs, ensure AES key is available in `aes_key.bin`
- Check that key is exactly 32 bytes (256 bits)
- Verify RPF header magic is `RPF2`

### Shader Conversion Failures
- Shader conversion is non-fatal
- Game can fall back to runtime compilation
- Check logs for specific shader errors

### DLC Installation Failures
- Verify STFS package is valid (magic: `CON `, `LIVE`, or `PIRS`)
- Ensure package contains expected DLC files
- Check that DLC type can be detected

---

## Multiplayer Backend System

Liberty Recomp supports three multiplayer backends, selectable during installation:

### Community Server (Default)

The recommended option for most users. Uses a community-hosted REST API for Xbox Live-style matchmaking.

**Features:**
- Works out of the box, no configuration needed
- Full Quick Match / Custom Match support
- Session browsing with filters (game mode, map area)
- Lobby codes for private sessions

**Config:**
```toml
[Multiplayer]
MultiplayerBackend = "Community"
# CommunityServerURL = "https://liberty-sessions.libertyrecomp.com"  # Hidden, uses default
```

### Firebase (Self-Hosted)

For private communities who want to run their own matchmaking server.

**Setup:**
1. Create a Firebase project at [console.firebase.google.com](https://console.firebase.google.com)
2. Enable Realtime Database
3. Copy Project ID and Web API Key
4. Enter credentials during installation or in config

**Config:**
```toml
[Multiplayer]
MultiplayerBackend = "Firebase"
FirebaseProjectId = "your-project-id"
FirebaseApiKey = "AIzaSy..."
```

### LAN Only

For offline/local network play without internet matchmaking.

**Features:**
- UDP broadcast discovery on local network
- No internet connection required
- Automatic session discovery
- Works behind any firewall

**Config:**
```toml
[Multiplayer]
MultiplayerBackend = "LAN"
# LANBroadcastPort = 3074  # Hidden, uses default
```

### Session Tracker Components

```
LibertyRecomp/kernel/io/
├── session_tracker.h              # ISessionTracker interface
├── session_tracker.cpp            # Factory and utilities
├── session_tracker_community.cpp  # Community server backend
├── session_tracker_firebase.cpp   # Firebase backend
└── session_tracker_lan.cpp        # LAN broadcast backend
```

---

## References

- [ISOFileSystem](../LibertyRecomp/install/iso_file_system.h) - Native Xbox 360 ISO parsing (based on Xenia)
- [XContentFileSystem](../LibertyRecomp/install/xcontent_file_system.h) - Native STFS/SVOD parsing (based on Xenia)
- [RpfExtractor](../LibertyRecomp/install/rpf_extractor.h) - Native RPF2 extraction
- [GTAMods RPF Wiki](https://gtamods.com/wiki/RPF_archive)
- [XenosRecomp README](../tools/XenosRecomp/README.md)
