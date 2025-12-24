# Liberty Recompiled - Enhanced Installation Architecture

## Overview

This document describes the comprehensive installation flow that handles:
1. ISO to folder extraction (using extract-xiso)
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
│   ├── xbox360.rpf
│   └── extracted\           # RPF contents
│       └── common\shaders\*.fxc
├── shader_cache\            # Compiled DXIL shaders
├── saves\                   # Save data
└── temp\                    # Temporary extraction files
```

**Path:** `C:\Users\<username>\AppData\Local\LibertyRecomp\`

### Linux
```
~/.local/share/LibertyRecomp/
├── game/                    # Extracted game files
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
├── shader_cache/            # Compiled AIR shaders
├── saves/
└── temp/
```

**Path:** `/Users/<username>/Library/Application Support/LibertyRecomp/`

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
│  2. SELECT TITLE UPDATE (NEW)                                           │
│     ├── Scans 'GAME UPDATES/' folder for STFS packages                 │
│     ├── Scans install path 'updates/' folder                           │
│     ├── User selects: No Update (v1.0) or detected TU version          │
│     └── Selected version stored in config for reference                │
│                                                                         │
│  3. SELECT DLC (Optional)                                               │
│     ├── The Lost and Damned (TLAD)                                     │
│     └── The Ballad of Gay Tony (TBOGT)                                 │
│                                                                         │
│  4. DETECT SOURCE TYPE                                                  │
│     ┌─────────────────┐                                                │
│     │ Is it an ISO?   │──YES──► Run extract-xiso                       │
│     └────────┬────────┘         └── Extract to temp directory          │
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
│     ├── Apply patch to default.xex                                     │
│     └── Fallback to xextool if native patching fails                   │
│                                                                         │
│  7. SCAN FOR SHADERS                                                    │
│     Look for .fxc files in:                                            │
│     ├── Extracted game folders directly                                │
│     └── Inside RPF archives (extract if needed)                        │
│                                                                         │
│  8. EXTRACT & CONVERT SHADERS                                           │
│     For each .fxc file found:                                          │
│     ├── Parse RAGE FXC container (magic: 0x61786772 "rgxa")            │
│     ├── Extract Xbox 360 shader binaries (magic: 0x102A11XX)           │
│     └── Store in shader_cache/extracted/                               │
│                                                                         │
│  9. COMPILE SHADER CACHE                                                │
│     Detect platform and compile:                                       │
│     ├── Windows: HLSL → DXIL (DXC)                                     │
│     ├── Linux: HLSL → SPIR-V (DXC)                                     │
│     └── macOS: HLSL → AIR (Metal compiler)                             │
│                                                                         │
│  10. MULTIPLAYER SETUP (Optional)                                       │
│      User selects multiplayer backend:                                 │
│      ├── Community Server (Recommended)                                │
│      │   └── Works out of box, no config needed                        │
│      ├── Firebase (Self-Hosted)                                        │
│      │   └── User provides project ID and API key                      │
│      └── LAN Only                                                      │
│          └── No internet matchmaking, local network only               │
│                                                                         │
│  11. FINALIZE                                                           │
│      ├── Write shader_cache.marker                                     │
│      ├── Save installed TU version to config                           │
│      ├── Save multiplayer backend choice to config                     │
│      ├── Clean up temp files                                           │
│      └── Signal completion                                             │
│                                                                         │
└─────────────────────────────────────────────────────────────────────────┘
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

## Optional: Install-Time PPC Recompilation (XenonRecomp)

Because GTA IV title updates are distributed as **full `default.xex` replacements**, supporting multiple update versions cleanly can require recompiling against the specific XEX the user provides.

Recent testing indicates XenonRecomp can run fast enough on modern machines that it is reasonable to support an **install-time (or first-run) PPC recompilation** mode. This can allow consumer distributions to avoid shipping the generated `ppc_recomp.*.cpp` sources.

### High-Level Concept

1. User selects base game source and optionally a title update source
2. Installer extracts/copies the chosen XEX into the install directory
3. Installer runs `XenonAnalyse` (if switch tables are not provided) and then `XenonRecomp` against the chosen XEX
4. The generated PPC C++ sources are compiled into the host binary (or into a loadable module, depending on the final architecture)

### Important Constraint

XenonRecomp outputs **C++ source**; it does not produce native machine code by itself. Therefore, install-time PPC recompilation is only viable if the installer can:

1. Bundle a compatible C++ toolchain for the platform, or
2. Require a system toolchain (macOS: Xcode Command Line Tools; Windows: MSVC Build Tools; Linux: clang/gcc + build essentials)

If neither is acceptable for the target audience, the alternative is to ship pre-generated PPC sources (or separate builds) for the supported XEX versions.

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

Xbox 360 ISO extraction using extract-xiso:

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

**Requirements:**
- `extract-xiso` must be installed and in PATH
- Repository: https://github.com/XboxDev/extract-xiso

### RPF Extractor (`install/rpf_extractor.h/cpp`)

RAGE Package File extraction:

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
- **Algorithm**: AES-256-ECB (no IV, no padding)
- **Compression**: zlib at block level

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

## External Tools

### extract-xiso

Cross-platform Xbox 360 ISO extraction tool.

**Installation:**
```bash
# Build from source
git clone https://github.com/XboxDev/extract-xiso.git
cd extract-xiso
mkdir build && cd build
cmake ..
make
sudo make install

# macOS (Homebrew)
brew install extract-xiso
```

**Usage:**
```bash
extract-xiso -x game.iso -d /output/directory/
```

### SparkCLI (Fallback)

C# tool for RPF extraction, used as fallback for complex encrypted archives.

**Location:** `/SparkIV-master/SRC/SparkCLI/`

**Cross-platform usage:**
```bash
# Windows
SparkCLI.exe extract common.rpf ./output --key ./default.xex

# Linux/macOS (requires Mono)
mono SparkCLI.exe extract common.rpf ./output --key ./default.xex
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

See [SHADER_PIPELINE.md](SHADER_PIPELINE.md) for detailed shader conversion documentation.

---

## Error Handling

### ISO Extraction Failures
- Check if `extract-xiso` is installed
- Verify ISO file is a valid Xbox 360 image
- Ensure sufficient disk space for extraction

### RPF Extraction Failures
- For encrypted RPFs, ensure AES key is available
- Try SparkCLI fallback for complex archives
- Pre-extract RPFs using OpenIV/SparkIV if automatic extraction fails

### Shader Conversion Failures
- Shader conversion is non-fatal
- Game can fall back to runtime compilation
- Check logs for specific shader errors

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

### GTA IV Game Modes Supported

| Mode | Description |
|------|-------------|
| Free Mode | Open world sandbox |
| Deathmatch | Free-for-all combat |
| Team Deathmatch | Team-based combat |
| Mafiya Work | Complete jobs for Petrovic |
| Car Jack City | Steal and deliver vehicles |
| Race / GTA Race | Vehicle racing |
| Cops 'n' Crooks | Asymmetric cops vs criminals |
| Turf War | Territory control |
| Hangman's NOOSE | Cooperative extraction |
| Deal Breaker | Cooperative mission |
| Bomb Da Base II | Cooperative mission |

---

## References

- [extract-xiso GitHub](https://github.com/XboxDev/extract-xiso)
- [SparkIV/RageLib](https://github.com/ahmed605/SparkIV)
- [GTAMods RPF Wiki](https://gtamods.com/wiki/RPF_archive)
- [XenosRecomp README](../tools/XenosRecomp/README.md)
- [Shader Pipeline Documentation](SHADER_PIPELINE.md)
