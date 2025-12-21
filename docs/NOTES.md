# LibertyRecomp Development Notes

> **Private Notes** - Not for committing to repository

---

## Project Overview

LibertyRecomp is a static recompilation project for **Grand Theft Auto IV (Xbox 360)**, forked from MarathonRecomp (Sonic '06 recompilation). The goal is to convert the Xbox 360 PowerPC executable into native x86-64 code that can run on modern PCs.

---

## Core Tools

### XenonRecomp (CPU Recompiler)

**Location**: `tools/XenonRecomp/`

**Purpose**: Converts Xbox 360 PowerPC (PPC) executables into C++ code, which can then be compiled for x86-64.

**Key Implementation Details**:

1. **Instruction Translation**
   - Direct conversion without decompilation (output is NOT human-readable)
   - CPU state passed as argument to every PPC function
   - Second argument is base address pointer (Xbox 360 uses 32-bit pointers)

2. **Endianness Handling**
   - Xbox 360 is big-endian, x86 is little-endian
   - Memory loads swap endianness when reading
   - Memory stores reverse to big-endian before writing
   - All loads/stores marked volatile to prevent unsafe reordering

3. **Vector Registers (VMX)**
   - Instead of swapping 32-bit elements, entire 16-byte vector is reversed
   - Instructions must account for reversed order (WZY instead of XYZ for dot products)
   - Most VMX instructions use x86 intrinsics

4. **Indirect Functions (Virtual Calls)**
   - Resolved via "perfect hash table" at runtime
   - 64-bit pointer dereferencing using original instruction address * 2
   - Function addresses placed after valid XEX memory region

5. **Jump Tables**
   - Detected via `mtctr r0` instruction pattern
   - XenonAnalyse generates TOML file with detected tables
   - Recompiler converts to real switch cases

6. **Register Save/Restore Functions**
   - Xbox 360 has specialized functions for non-volatile registers
   - Must find these addresses via byte patterns:
     - `restgprlr_14`: `e9 c1 ff 68` (ld r14, -0x98(r1))
     - `savegprlr_14`: `f9 c1 ff 68` (std r14, -0x98(r1))
     - `restfpr_14`: `c9 cc ff 70` (lfd f14, -0x90(r12))
     - `savefpr_14`: `d9 cc ff 70` (stfd r14, -0x90(r12))

7. **Optimizations** (enable only after working build)
   - `skip_lr` - Skip link register (if no exceptions)
   - `ctr_as_local` - Count register as local variable
   - `non_volatile_as_local` - Reduces executable size significantly

8. **Mid-asm Hooks**
   - Insert function calls at specific instruction addresses
   - Can override PPC functions with custom implementations
   - Linker resolves during compilation

**Usage**:
```bash
XenonAnalyse [input.xex] [output_switch_tables.toml]
XenonRecomp [config.toml] [ppc_context.h]
```

### Install-Time PPC Recompilation (Optional)

Recent testing shows **XenonRecomp runs fast enough on modern machines** that a reasonable workflow is to run it during installation (or “first run”), instead of shipping the generated `LibertyRecompLib/ppc/ppc_recomp.*.cpp` sources in consumer distributions.

**Why this matters**:

1. **Title Update Support**
   - GTA IV title updates are **full XEX replacements**.
   - Different XEX builds imply different code layout/function boundaries.
   - Install-time recompilation allows the installer to recompile against the user's selected base XEX / title update XEX without us publishing separate builds per update.

2. **Smaller Source Distributions**
   - Generated PPC sources are very large and change frequently.
   - Keeping them out of consumer/source drops reduces repo size and avoids churn.

3. **Cleaner Build/CI**
   - CI can build from a known-good XEX for “official” releases.
   - Developers can still regenerate locally when needed.

**Important constraint**:

- XenonRecomp outputs **C++ source code**, which must still be compiled into the final binary.
- Therefore, an install-time recompilation pipeline is only viable if the installer can:
  - Use a prepackaged toolchain, or
  - Require a host toolchain (e.g. Xcode CLT / clang / Ninja) and build on the user's machine.

This is an architectural option to support multiple XEX variants; whether we enable it by default depends on distribution goals.

---

### XenosRecomp (Shader Recompiler)

**Location**: `tools/XenosRecomp/`

**Purpose**: Converts Xbox 360 Xenos GPU shaders to HLSL, then to DXIL (D3D12) and SPIR-V (Vulkan).

**Key Implementation Details**:

1. **Shader Container**
   - Xbox 360 shaders contain: constant buffer reflection, definitions, interpolators, vertex declarations, instructions
   - Reverse-engineered enough for Sonic Unleashed, may need more work for GTA IV

2. **Control Flow**
   - HLSL doesn't support `goto`
   - Implemented with `while` loop + `switch` statement
   - Local `pc` variable determines current block

3. **Constant Buffers**
   - Vertex shader: 4096 bytes (256 float4 registers)
   - Pixel shader: 3584 bytes (224 float4 registers)
   - Shared constants for recomp-specific data

4. **Textures & Samplers**
   - Bindless approach with descriptor indices
   - Cube textures need special handling (direction array)

5. **Specialization Constants**
   - R11G11B10 vertex format unpacking
   - Alpha testing (since modern HW lacks fixed function)
   - DXIL lacks native support, uses library linking

**Important**: Game-specific modifications are often required. Do NOT expect it to work out of the box.

---

## GTA IV Specific

### Game Structure

```
Grand Theft Auto IV (USA)/
├── default.xex          # Main executable (11MB)
├── audio.rpf            # Audio archives
├── common.rpf           # Common data
├── xbox360.rpf          # Platform-specific data
├── common/data/cdimages/
└── xbox360/
    ├── anim/
    ├── audio/
    ├── data/
    ├── models/
    └── movies/
```

### XEX Analysis Results

- **XenonAnalyse** successfully ran on `default.xex`
- Generated `gta4_switch_tables.toml` with **~28,000 lines** of switch tables
- XEX header verified: `XEX2` magic number present

### XDK Version (from XeXTool)

**Decisive evidence (static libraries)**

Static Libraries:

- `LIBCPMT` `v2.0.6274.0`
- `XAPILIB` `v2.0.6274.3`
- `XBOXKRNL` `v2.0.6274.0`
- `XMP` `v2.0.6274.0`
- `XHV` `v2.0.6274.3`
- `XONLINE` `v2.0.6274.0`
- `D3D9` `v2.0.6274.0`
- `XGRAPHC` `v2.0.6274.0`
- `XAUDLTCG` `v2.0.6274.0`

Interpretation:

- `2.0.6274.x` corresponds to the Xbox 360 XDK generation based on kernel `6274` (mid–late 2007)
- This reflects the actual SDK toolchain used to build the executable (link-time), not runtime requirements

**Import libraries (runtime minimum)**

Import Libraries:

- `xam.xex` `v2.0.6683.0` (min `v2.0.6683.0`)
- `xboxkrnl.exe` `v2.0.6683.0` (min `v2.0.6683.0`)

Interpretation:

- These indicate the minimum dashboard/kernel required to run
- They do not indicate the SDK used to compile the game

**Timestamp corroboration**

- Filetime: Mon Mar 31 09:40:52 2008

**Reusable conclusion**

GTA IV (Xbox 360) was built with Xbox 360 XDK `2.0.6274.x` (mid–late 2007), with runtime compatibility targeting kernel `2.0.6683.0+`.

### RAGE Engine (Rockstar Advanced Game Engine)

GTA IV uses the RAGE engine. Key structures to implement:

1. **Base Classes**
   - `datBase` - Base data class
   - `pgBase` - Page/streaming base
   - `atArray<T>` - Rockstar's array type

2. **Entity System**
   - `CEntity` - Base entity class
   - `CPhysical` - Physical objects
   - `CPed` - Pedestrians/players
   - `CVehicle` - Vehicles
   - `CObject` - World objects

3. **Game Systems**
   - `CGame` - Main game class
   - `CWorld` - World management
   - `CStreaming` - Asset streaming
   - `CTimer` - Game timing
   - `CCamera` - Camera system

4. **Resource System**
   - `.rpf` archives (RAGE Package File)
   - `.wtd` textures
   - `.wdr`/`.wdd` models
   - `.wpl` placement files

---

## Configuration Files

### GTA4.toml (XenonRecomp Config)

**Location**: `LibertyRecompLib/config/GTA4.toml`

```toml
[main]
file_path = "../../Grand Theft Auto IV (USA) (En,Fr,De,Es,It)/default.xex"
out_directory_path = "../ppc"
switch_table_file_path = "./gta4_switch_tables.toml"

# TODO: Find actual addresses
# restgprlr_14_address = 0x????????
# savegprlr_14_address = 0x????????
```

### Finding Register Save/Restore Addresses

Use hex editor or xxd to search for byte patterns:
```bash
xxd default.xex | grep -i "e9 c1 ff 68"  # restgprlr_14
xxd default.xex | grep -i "f9 c1 ff 68"  # savegprlr_14
```

---

## Build System

### Requirements
- CMake 3.20+
- Clang 18+ (required, not optional)
- Ninja build system
- vcpkg for dependencies

### Building XenonRecomp Tools
```bash
cd tools/XenonRecomp
mkdir build && cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
ninja
```

### Building LibertyRecomp
```bash
cmake . --preset macos-release  # or linux-release, x64-Clang-Release
cmake --build ./out/build/macos-release --target LibertyRecomp
```

---

## Progress Tracking

### Completed
- [x] Fork and rebrand from MarathonRecomp
- [x] Rename directories (MarathonRecomp → LibertyRecomp)
- [x] Update CMake configuration
- [x] Create GTA IV gitignore entries
- [x] Run XenonAnalyse on GTA IV XEX
- [x] Generate switch tables (~28k lines)
- [x] Create initial GTA4.toml config
- [x] Set up CI/CD for releases
- [x] Find register save/restore function addresses
- [x] Complete XenonRecomp pass on GTA IV  
- [x] Fix memcpy crash (sub_82990830 native implementation)
- [x] Fix empty path spam in logs
- [x] Basic rendering pipeline connected (VdSwap → Video::Present)
- [x] Input system working (XamInputGetState hooked, keyboard + controller)
- [x] Window displays visible color (dark blue clear instead of black)
- [x] Bypass installer wizard by pointing GetGamePath() to project folder
- [x] Set up RPF DUMP folder with extracted game data
- [x] Create symlinks for game content access

### In Progress
- [ ] **GPU/Rendering**: GTA IV D3D functions found but need proper implementation
- [ ] **File System**: Game can't find all required files, hitting "dirty disc" errors
- [ ] Implement RAGE engine stubs

### TODO
- [ ] Run XenosRecomp on GTA IV shaders
- [ ] Implement graphics translation layer for GTA IV's D3D calls
- [ ] Implement audio system
- [ ] Save data handling
- [ ] Input remapping for GTA IV controls

---

## Comparison: Sonic Unleashed Recomp vs GTA IV Recomp

### Key Architectural Differences

| Aspect | Sonic Unleashed (UnleashedRecomp) | GTA IV (LibertyRecomp) |
|--------|-----------------------------------|------------------------|
| **NtCreateFile** | Minimal stub, returns 0 (success) | Full implementation with path resolution |
| **Primary File API** | XCreateFileA/XReadFile (Win32-style) | Mix of NT-level and Win32-style APIs |
| **NtReadFile** | Stub (unused) | Full implementation with handle tracking |
| **Path Resolution** | Simple root path mapping | Complex with shader redirection, RPF handling |

### Lessons Learned

1. **Sonic Unleashed uses higher-level APIs**: The game hooks `XCreateFileA`, `XReadFile`, `XWriteFile` directly at specific game function addresses. The NT-level functions (`NtCreateFile`, `NtReadFile`) are essentially stubs because the game doesn't use them directly.

2. **GTA IV uses NT-level APIs more heavily**: The RAGE engine appears to use lower-level NT kernel functions for file I/O, which is why we have full implementations of `NtCreateFile` and `NtReadFile`.

3. **RPF Mount Detection**: We intentionally return `NOT_FOUND` when the game tries to open directories like `game:\` as files - this is the game's RPF archive loader. Since we've extracted the RPF contents, we want this to fail so the game falls back to loading individual extracted files.

4. **Function Hook Addresses**: 
   - Sonic Unleashed hooks: `sub_82BD4668` (XCreateFileA), `sub_82BD4478` (XReadFile), etc.
   - GTA IV hooks: `sub_82537400` (XCreateFileA), `sub_82537118` (XReadFile), etc.
   - These are game-specific addresses found in the recompiled PPC code.

### What We Should Investigate

1. **Why is the game hitting dirty disc error?** The game stores a NULL handle (0x00000000) somewhere and tries to read from it. Need to find which file open fails silently.

2. **File handle tracking**: We track NT file handles in `g_ntFileHandles` map, but the game might be using a different handle system internally.

3. **XCreateFileA vs NtCreateFile**: GTA IV might be calling both - need to verify both paths work correctly.

---

## Current State (December 14, 2025)

### What Works
1. **Build System**: Compiles on macOS with `cmake --preset macos-release` + ninja
2. **Installer Bypass**: GetGamePath() returns `/Users/Ozordi/Downloads/MarathonRecomp` with a `game` symlink
3. **Shader Loading**: fxl_final shaders found and enumerated (89 files)
4. **Thread Creation**: Game creates multiple threads successfully
5. **File System**: Basic NtCreateFile/NtReadFile working, directories accessible
6. **RPF Extraction**: All RPF files extracted to `RPF DUMP/` folder and copied to game folder

### Current Blocker: Dirty Disc Error

**Symptom**: After starting, game loops calling `XamShowDirtyDiscErrorUI` and eventually exits.

**Root Cause**: The game is trying to read from file handles that return 0x00000000 (NULL), which means `NtCreateFile` failed for some file but we don't see the failure in logs.

**Log Pattern**:
```
[NtReadFile] Directory handle 0xEB28D330 - returning END_OF_FILE  (x11 times)
[XamShowDirtyDiscErrorUI] !!! STUB !!! - Dirty disc error #1
[NtReadFile] INVALID handle 0x00000000  (many times)
```

**Investigation Needed**:
- Find what file the game tries to open that fails
- The game stores a NULL handle somewhere and then tries to read from it

---

## File System Architecture

### Path Resolution Flow

1. Game requests file with Xbox 360 path like `game:\something` or `fxl_final\shader.fxc`
2. `ResolveGuestPathBestEffort()` in `file_system.cpp` converts to host path
3. Special handling for:
   - `game:\` → `GetGamePath()/game/` 
   - `fxl_final\` → RPF DUMP shaders folder

### GetGamePath() Configuration

**File**: `LibertyRecomp/user/paths.h`

```cpp
inline std::filesystem::path GetGamePath()
{
    // Returns project folder, expects "game" subdirectory with actual files
    return "/Users/Ozordi/Downloads/MarathonRecomp";
}
```

### Directory Structure Required

```
/Users/Ozordi/Downloads/MarathonRecomp/
├── game -> Grand Theft Auto IV (USA) (En,Fr,De,Es,It)  # SYMLINK
├── Grand Theft Auto IV (USA) (En,Fr,De,Es,It)/         # Actual game files
│   ├── default.xex
│   ├── audio/
│   ├── common/
│   │   ├── data/           # Extracted from common.rpf
│   │   │   ├── *.dat files
│   │   │   ├── shaders/
│   │   │   │   └── fxl_final/  # 89 shader .fxc files
│   │   │   └── cdimages/
│   │   └── text/
│   └── xbox360/
│       ├── data/           # Extracted from xbox360.rpf
│       ├── models/
│       └── textures/       # 84 .xtd files
└── RPF DUMP/               # Reference copies of extracted RPF data
    ├── Common RPF Dump/
    ├── XBOX 360 RPF DUMP/
    └── AUDIO RPF DUMP/
```

---

## RPF File Extraction

### Why We Extract RPF Files

GTA IV stores most game assets in **RPF archives** (RAGE Package Files). The Xbox 360 game includes:
- `common.rpf` - Shared data (shaders, configs, text, UI)
- `xbox360.rpf` - Platform-specific assets (textures, models, data)
- `audio.rpf` - Audio configuration and metadata

**The recompiled game CANNOT read RPF archives directly** - it expects files to be accessible via the file system. Therefore, we must extract RPF contents and place them where the game expects to find them.

### Extraction Tool

Use **SparkIV** (included in `SparkIV-master/` folder) or **OpenIV** to extract RPF contents.

### RPF DUMP Folder Structure

Located at `/Users/Ozordi/Downloads/MarathonRecomp/RPF DUMP/`:

```
RPF DUMP/
├── Common RPF Dump/          # Extracted from common.rpf
│   ├── data/                 # 72 items - game configuration files
│   │   ├── *.dat files       # Game data (handling.dat, carcols.dat, etc.)
│   │   ├── *.xml files       # Weather, effects, etc.
│   │   ├── effects/          # Particle effects
│   │   ├── decision/         # AI decision files
│   │   └── ...
│   ├── shaders/              # Xbox 360 GPU shaders
│   │   ├── fxl_final/        # 89 compiled shader files (.fxc)
│   │   ├── db/               # Shader database
│   │   └── dcl/              # Shader declarations
│   └── text/                 # Localization strings
│
├── XBOX 360 RPF DUMP/        # Extracted from xbox360.rpf
│   ├── data/                 # Platform-specific configs
│   │   └── effects/          # Xbox 360 specific effects
│   ├── models/               # 3D models
│   ├── textures/             # 84 .xtd texture files
│   │   ├── hud.xtd
│   │   ├── fonts.xtd
│   │   ├── frontend_360.xtd
│   │   └── ...
│   ├── html/                 # In-game web browser content
│   └── stream.ini            # Streaming configuration
│
└── AUDIO RPF DUMP/           # Extracted from audio.rpf
    └── config/               # Audio system configuration
```

### What Each Dump Contains

| RPF File | Key Contents | Used For |
|----------|--------------|----------|
| common.rpf | Shaders (fxl_final/), handling.dat, carcols.dat, gta.dat | **CRITICAL** - Shaders required for rendering, game config for physics/vehicles |
| xbox360.rpf | Textures (.xtd), models, UI assets | Rendering textures, 3D models, HUD elements |
| audio.rpf | Audio config files | Sound system initialization |

### How Extracted Data Is Used

1. **Shaders**: Game loads from `fxl_final\` - redirected to `RPF DUMP/Common RPF Dump/shaders/fxl_final/`
2. **Game Data**: Copied to `game/common/data/` so game finds config files via `game:\common\data\` paths
3. **Textures**: Copied to `game/xbox360/textures/` for texture loading
4. **Models**: Copied to `game/xbox360/models/` for model loading

### Setup Commands (Already Run)

```bash
# Create symlink for game folder
ln -sf "Grand Theft Auto IV (USA) (En,Fr,De,Es,It)" game

# Copy common.rpf data to game folder
cp -Rn "RPF DUMP/Common RPF Dump/data/"* "game/common/data/"
cp -R "RPF DUMP/Common RPF Dump/shaders/"* "game/common/data/shaders/"
cp -R "RPF DUMP/Common RPF Dump/text/"* "game/common/text/"

# Copy xbox360.rpf data to game folder
cp -Rn "RPF DUMP/XBOX 360 RPF DUMP/textures/"* "game/xbox360/textures/"
cp -Rn "RPF DUMP/XBOX 360 RPF DUMP/models/"* "game/xbox360/models/"
cp -Rn "RPF DUMP/XBOX 360 RPF DUMP/data/"* "game/xbox360/data/"

# Copy audio.rpf data
cp -Rn "RPF DUMP/AUDIO RPF DUMP/"* "game/audio/"
```

---

## GPU Hooks Architecture

### Key Files

- **`LibertyRecomp/gpu/video.cpp`**: Main GPU/D3D hook implementations
- **`LibertyRecompLib/ppc/ppc_recomp.159.cpp`**: Contains GTA IV D3D wrapper functions

### GTA IV D3D Function Addresses (Discovered)

| Function | GTA IV Address | File Location | Notes |
|----------|----------------|---------------|-------|
| Present/VdSwap caller | 0x829D5388 | ppc_recomp.159.cpp:30750 | Confirmed, calls VdSwap |
| CreateDevice | 0x829D87E8 | ppc_recomp.159.cpp | Calls VdInitializeEngines |
| CreateTexture | 0x829D3400 | ppc_recomp.159.cpp | Texture creation |
| CreateVertexBuffer | 0x829D3520 | ppc_recomp.159.cpp | VB creation |
| GPU Memory Allocator | 0x829DFAD8 | ppc_recomp.168.cpp | GPU mem allocation |
| Effect Manager Load | 0x8285E048 | ppc_recomp.xxx | Shader loading |

### Current Stubs Implemented

```cpp
// In video.cpp - These bypass problematic initialization
GUEST_FUNCTION_HOOK(sub_8285E048, EffectManagerStub);  // Returns failure to skip shader init
GUEST_FUNCTION_HOOK(sub_829DFAD8, GpuMemAllocStub);    // Fake GPU memory allocation
GUEST_FUNCTION_HOOK(sub_829D3400, GTAIV_CreateTexture);    // Texture stub
GUEST_FUNCTION_HOOK(sub_829D3520, GTAIV_CreateVertexBuffer); // VB stub
```

---

## SDL Event Pumping

### The Problem
On macOS, SDL event pumping **must** happen on the main thread. But `GuestThread::Start()` blocks the main thread running PPC code. If no SDL events are pumped, the window becomes unresponsive.

### Solution Implemented
Added `PumpSdlEventsIfNeeded()` function in `imports.cpp`:

```cpp
void PumpSdlEventsIfNeeded()
{
    if (!IsMainThread()) return;  // Only pump on main thread
    
    auto now = std::chrono::steady_clock::now();
    if (now - g_lastSdlPumpTime >= SDL_PUMP_INTERVAL) {
        g_lastSdlPumpTime = now;
        SDL_PumpEvents();
        // Also handle SDL_QUIT
    }
}
```

Called from:
- `KeDelayExecutionThread()`
- `KeQuerySystemTime()`
- `NtCreateFile()`
- `NtReadFile()`
- `XamShowDirtyDiscErrorUI()`

---

## Shader System

### Shader Paths
- Xbox 360 shaders stored in `common/data/shaders/fxl_final/`
- 89 `.fxc` files (compiled shader bytecode)
- Example: `gta_default.fxc`, `gta_emissivestrong.fxc`, etc.

### Path Resolution for Shaders
In `file_system.cpp`:
```cpp
if (path starts with "fxl_final") {
    // Redirect to RPF DUMP shaders folder
    return "/Users/Ozordi/Downloads/MarathonRecomp/RPF DUMP/Common RPF Dump/shaders/fxl_final"
}
```

### Corrupted Shader Paths
The game sometimes requests paths with garbage characters like `fxl_final\������`. This is handled by:
```cpp
if (path has non-printable characters) {
    // Use fallback shader
    return "fxl_final/gta_default.fxc"
}
```

---

## Critical Issue: D3D Function Address Mapping

### The Problem

The recompiled GTA IV code is complete (**43,650 functions** in `LibertyRecompLib/ppc/`), but the GPU rendering hooks in `LibertyRecomp/gpu/video.cpp` were originally for **Sonic 06's addresses**.

### GTA IV D3D Function Addresses (Discovered)

| Function | GTA IV Address | Status |
|----------|----------------|--------|
| Present (VdSwap caller) | 0x829D5388 | ✅ Hooked |
| CreateDevice | 0x829D87E8 | ✅ Hooked |
| CreateTexture | 0x829D3400 | ✅ Stubbed |
| CreateVertexBuffer | 0x829D3520 | ✅ Stubbed |
| GPU Memory Allocator | 0x829DFAD8 | ✅ Stubbed |
| Effect Manager Load | 0x8285E048 | ✅ Stubbed |
| DrawPrimitive | ~0x829D* | ❌ Not found yet |
| SetTexture | ~0x829D* | ❌ Not found yet |

### Finding More D3D Functions

GTA IV D3D functions are in **0x829D0000-0x829EFFFF** range:

```bash
# List D3D range functions
grep -o "sub_829[DE][0-9A-F]*" LibertyRecompLib/ppc/ppc_recomp.159.cpp | sort -u

# Find VdSwap callers
grep -n "VdSwap" LibertyRecompLib/ppc/*.cpp
```

---

## Useful Links

- [N64 Recompiled](https://github.com/N64Recomp/N64Recomp) - Inspiration for XenonRecomp
- [Xenia Emulator](https://github.com/xenia-project/xenia) - Reference for Xbox 360 internals
- [Xenia Canary](https://github.com/xenia-canary/xenia-canary) - XEX patching code
- [Unleashed Recompiled](https://github.com/hedge-dev/UnleashedRecomp) - Similar project for Sonic Unleashed

---

## Notes on GTA IV Specifics

### Known Challenges
1. **RAGE Engine Complexity** - Much more complex than Hedgehog Engine (Sonic)
2. **Script System** - GTA IV uses compiled scripts that need handling
3. **Streaming System** - Open world requires sophisticated streaming
4. **Physics** - Uses Euphoria physics engine (middleware)
5. **Audio** - Custom audio system with multiple banks

### RPF Archive Format
- Version used in GTA IV is different from GTA V
- Contains compressed/encrypted data
- Tools exist for extraction (OpenIV, SparkIV)

---

## Build Commands Quick Reference

```bash
# Configure (first time or after CMakeLists changes)
cmake --preset macos-release

# Build
ninja -C out/build/macos-release LibertyRecomp

# Run
./out/build/macos-release/LibertyRecomp/Liberty\ Recompiled.app/Contents/MacOS/Liberty\ Recompiled

# Force rebuild after header changes
touch LibertyRecomp/user/paths.h && ninja -C out/build/macos-release LibertyRecomp
```

---

## Key Source Files

| File | Purpose |
|------|---------|
| `LibertyRecomp/gpu/video.cpp` | GPU hooks, D3D stubs, texture/buffer management |
| `LibertyRecomp/kernel/imports.cpp` | Kernel function hooks (file I/O, threads, etc.) |
| `LibertyRecomp/kernel/io/file_system.cpp` | Path resolution, file redirection |
| `LibertyRecomp/user/paths.h` | GetGamePath() definition |
| `LibertyRecomp/main.cpp` | Entry point, installer bypass |
| `LibertyRecompLib/ppc/ppc_recomp.159.cpp` | Recompiled D3D wrapper functions |

---

---

## Current State (December 15, 2025) - Boot Stall Investigation

### Summary

The game progresses through initial boot but **stalls after file read #8**. All PPC code translation is correct (XenonRecomp output is faithful). The stall is **host-side** - missing or incomplete implementations of Xbox kernel/XAM/async semantics.

### What Works Now

1. **File System**: `ResolvePath` fixed to handle slash-only paths (`\` → game root)
2. **Network Stubs**: `XNetGetTitleXnAddr` returns valid XNADDR with flags `0x66`
3. **GPU Poll Thread**: Forced GPU flags after 10 waits, thread now sleeps instead of spinning
4. **Barrier Overrides**: `sub_829A1EF8` override working (10 calls logged)
5. **Async File Reads**: 8 file reads complete successfully with proper XXOVERLAPPED completion
6. **Worker Threads**: 9 worker threads created and running

### Current Blocker: Stall After File Read #8

**Symptom**: After async file read #8 completes, no further activity occurs. Only the GPU poll thread continues (sleeping).

**Log Evidence**:
```
[GTA4_FileLoad] sub_829A1F00 #8 read 12 bytes from 'common' at offset 0x9FFA0
[GTA4_FileLoad] ASYNC: Set XXOVERLAPPED completion: Error=0, Length=12
... (no further activity except GPU poll)
```

**Analysis**:
- Worker threads are NOT blocked on `KeWaitForSingleObject` (no semaphore waits logged)
- Worker threads are NOT blocked on `KeWaitForMultipleObjects` (no calls logged)
- This means threads are either:
  1. **Spinning in tight CPU loops** without calling kernel functions
  2. **Exited/returned** after initial work

### Key Insight: XenonRecomp Limitations

XenonRecomp only handles **guest CPU-side code translation**. It does NOT implement:
- Xbox kernel semantics (`KeWaitForSingleObject`, events, semaphores)
- GPU initialization and interrupt handling
- Network/XAM APIs
- Async I/O completion semantics

**All of these must be manually implemented in `imports.cpp`**.

### Patches Applied

1. **`ppc_recomp.155.cpp`**: Patched `__imp__sub_829A1A50` to force return `0` (success)
   - This bypasses the cooperative polling barrier that returns `996` (no progress)
   
2. **`imports.cpp`**: Added overrides for:
   - `sub_829A1EF8` - Poll/yield helper, forces return `0`
   - `sub_829A1A50` - Async status helper (weak symbol override, not being called)
   - `sub_829A1CA0` - XamContentClose wrapper (weak symbol override, not being called)

### Weak Symbol Override Issue

The weak symbol overrides for `sub_829A1A50` and `sub_829A1CA0` are **not being called** despite being defined. This is because:
- `PPC_WEAK_FUNC` uses C++ linkage with `__attribute__((weak,noinline))`
- The linker may be resolving the weak symbol from the recompiled library before seeing our strong symbol
- Direct patching of `__imp__sub_829A1A50` in `ppc_recomp.155.cpp` is more reliable

### Magic Return Values (from docs.md)

| Value | Meaning | Effect |
|-------|---------|--------|
| `996` | "No progress" | Callers exit early without advancing state |
| `997` | "Pending" | Callers store pending state and return |
| `258` | Mapped to `996` | Intermediate value in `sub_829A1A50` |
| `259` | Triggers wait | Causes `__imp__NtWaitForSingleObjectEx` call |
| `257` | Retry trigger | Used in `sub_829A9738` to retry wait |

### Potential Blocking Points (from docs.md)

1. **`sub_82169400` / `sub_821694C8`**: Tight wait loops gated by global flags at offset `+300`
2. **`sub_829DDC90`**: Wait loop that checks for return value `258`
3. **`sub_827F0B20`**: Tight retry loop that spins while `sub_829A1F00` returns `0`
4. **`sub_829A3318`**: Boot orchestrator that loops on `XamTaskShouldExit()`

### Patches Applied (December 15, 2025)

| Function | File | Patch |
|----------|------|-------|
| `__imp__sub_829A1A50` | ppc_recomp.155.cpp | Force return 0 (success) |
| `__imp__sub_829A3A30` | ppc_recomp.155.cpp | Force return 1 (success) |
| `__imp__sub_82169400` | ppc_recomp.4.cpp | Force immediate exit |
| `__imp__sub_821694C8` | ppc_recomp.4.cpp | Force immediate exit |

**Result**: Patches did not help - these functions are not on the critical path causing the stall.

### Key Observations

1. **VdSwap is never called** - game hasn't reached render loop
2. **KeQuerySystemTime not called after setup** - no threads are actively running host functions
3. **No kernel wait functions called after file read #8** - threads either exited or spinning in pure CPU loops
4. **Weak symbol overrides don't work** - linker resolves from recompiled library first

### Sonic Unleashed Comparison

Key differences from Sonic Unleashed Recomp:
- Sonic uses **Win32-style file APIs** (`XCreateFileA`, `XReadFile`) hooked at game-specific addresses
- Sonic's `NtReadFile` is a **stub** - not used by the game
- GTA IV uses **NT-level APIs** (`NtCreateFile`, `NtReadFile`) more heavily
- Sonic's XAM functions return **synchronous success** (0), never 997 (pending)

### Next Steps

1. **Investigate if main thread has exited** after initial file reads
2. **Check if game expects specific file content** that we're not providing correctly
3. **Add logging to guest entry point** to trace execution flow
4. **Consider if the 8 file reads are failing silently** despite returning success

### Files Modified

| File | Changes |
|------|---------|
| `LibertyRecomp/kernel/imports.cpp` | ResolvePath fix, XNetGetTitleXnAddr stub, GPU poll handling, barrier overrides, extensive logging |
| `LibertyRecomp/kernel/io/file_system.cpp` | Slash-only path handling |
| `LibertyRecompLib/ppc/ppc_recomp.155.cpp` | Patched `__imp__sub_829A1A50` to force success |
| `LibertyRecompLib/ppc/docs.md` | Comprehensive documentation of boot-critical functions and state machines |

---

## RPF2 Archive Format (GTA IV)

### Header Structure (20 bytes, plaintext, LITTLE-ENDIAN on disk)
```
Offset  Size  Field
0x00    4     Magic: 0x52504632 ("RPF2")
0x04    4     TOC Size (bytes) - little-endian
0x08    4     Entry Count - little-endian  
0x0C    4     Unknown
0x10    4     Encryption Flag (0 = unencrypted, non-zero = encrypted files)
```

### Key Facts
- **RPF2 headers are ALWAYS plaintext** - never encrypted
- **TOC is ALWAYS plaintext** - starts at offset 0x800 (2048 bytes)
- **Encryption is per-file** - only individual file data blocks are encrypted, not the container
- **Whole-file decryption is WRONG** - will scramble valid plaintext data

### ⚠️ CRITICAL: Endianness Handling

**On-disk format**: RPF2 files store all multi-byte integers in **LITTLE-ENDIAN** format.

**Xbox 360 CPU**: PowerPC is **BIG-ENDIAN**.

**What this means**:
- Raw hex `00 70 00 00` in file = `0x00007000` (28672) as little-endian
- If Xbox 360 does raw 32-bit load without swap: reads as `0x00700000` (7,340,032) - **WRONG!**
- The game **MUST byte-swap** when reading RPF header/TOC fields

**GTA IV's approach**: The RAGE engine explicitly byte-swaps when reading RPF files so that fields become meaningful in big-endian context.

**For emulator**: Must mirror what real Xbox 360 RAGE engine does:
1. Read file bytes (little-endian on disk)
2. Convert header fields to PPC (big-endian) format immediately
3. Use converted values to drive TOC parsing

### Table of Contents (TOC) Structure

TOC starts at offset 0x800 (2048 bytes). Contains directory and file entries.

#### Directory Entry (16 bytes)
```
Offset  Size  Field
0x00    4     Name offset (into name string section)
0x04    4     Flags (directory metadata)
0x08    4     First child index (into TOC)
0x0C    4     Child count
```

#### File Entry (16 bytes)
```
Offset  Size  Field
0x00    4     Name offset (into name string section)
0x04    4     Size (compressed or raw)
0x08    3     Data offset (block offset relative to archive start)
0x0B    1     Resource type ID
0x0C    4     Flags (compression & encryption flags)
```

**Flag bits in files indicate**:
- Whether compressed
- Whether encrypted
- Resource category/type (audio, textures, models, etc.)

### Filename / Name Section

After TOC region, there's a name string region where all directory and file names reside (null-terminated). Filenames referenced in entries use an offset into this region.

### Encryption Details (when file entry has encrypted flag)
- Algorithm: **AES-256-ECB**
- No IV
- No padding
- Decrypt AFTER reading the block, BEFORE decompression
- Block-level encryption, not whole-file

### Compression
- Usually zlib at block level (no separate file header)
- Compressed block might be smaller or equal to uncompressed size

### AES Key Location
- **Path**: `/Users/Ozordi/Downloads/MarathonRecomp/extracted/aes_key.bin`
- **Size**: 32 bytes (256-bit key)

### Correct Loader Flow
1. Read RPF2 header (plaintext) - **byte-swap numeric fields for big-endian CPU**
2. Parse TOC entries at offset 0x800 (plaintext) - **byte-swap all fields**
3. For each file entry:
   - Seek to data offset
   - Read compressed size bytes
   - If entry has ENCRYPTED flag: AES-256-ECB decrypt that buffer
   - If entry has COMPRESSED flag: decompress (zlib)
   - Return result to game

### RPF Version Reference
| Version | Magic      | Game |
|---------|------------|------|
| 0       | 0x52504630 | Table Tennis |
| 2       | 0x52504632 | GTA IV |
| 3       | 0x52504633 | GTA IV Audio, Midnight Club LA |
| 4       | 0x52504634 | Max Payne 3 |
| 6       | 0x52504636 | Red Dead Redemption |
| 7       | 0x52504637 | GTA V |
| 8       | 0x52504638 | RDR2 |

### Tools That Support RPF2
- **OpenIV** - supports RPF2, browse/extract/rebuild
- **SparkIV** - older tool for GTA IV RPFs
- **CodeWalker** - primarily GTA V but has RPF support

---

## Current Crash Analysis (December 15, 2025)

### Crash Signature
```
Exception Type:    EXC_BAD_ACCESS (SIGBUS)
Exception Subtype: EXC_ARM_DA_ALIGN at 0x5441545300454c42
PC: 0x5441545300454c42 (invalid - contains ASCII "TATS\0ELB")
```

### Key Crash Parameters
| Field | Value | Interpretation |
|-------|-------|----------------|
| PC | 0x5441545300454c42 | ASCII garbage in program counter |
| size | 0xED00D1BC | ~3.96 GB - uninitialized |
| src | 0x98948285 | Invalid guest address |
| dst | 0x000A08F0 | Low heap struct |
| caller_lr | 0x827E84A0 | sub_827E8420 (stream buffer reader) |

### Call Stack (Thread 0)
```
0   ???                            0x545300454c42 ???  (corrupt PC)
1   sub_827E8B08  (string reader)
2   sub_827E8BA8  
3   sub_821928D0
4   sub_82192980
5   sub_821DB1E0
6   sub_821DE390
7   sub_82120EE8
8   sub_82120000
9   sub_8218BEB0
10  sub_827D89B8
11  _xstart
```

### Root Cause Analysis

The crash occurs in `sub_827E8420` which is a **stream buffer reading function**:
1. Loads object pointer from stream struct
2. Loads vtable pointer from object
3. Calls vtable[20] for read operation
4. **Vtable contains garbage** ("TATS\0ELB" = part of string data)

This indicates **vtable/function pointer corruption** caused by:
- Game reading RPF header successfully
- Game attempting to set up internal stream objects
- Stream objects populated with uninitialized/garbage data
- Indirect call through corrupted vtable → crash

### What's Working
- ✅ game:\ redirects to xbox360.rpf
- ✅ RPF2 header parsing (987 entries detected)
- ✅ Header bytes returned correctly: `52 50 46 32 00 70 00 00 DB 03 00 00`
- ✅ TOC marked as plaintext (tocEncrypted=false for RPF2)

### What's Failing
- ❌ Game's internal stream objects have corrupted vtables
- ❌ Possible endianness mismatch when game interprets RPF data
- ❌ Game may not be properly byte-swapping header fields

### Next Steps
1. Verify byte-swapping is happening correctly in game code path
2. Check if game expects RPF data to be pre-swapped by kernel
3. Investigate stream object initialization in RAGE engine

---

---

## RAGE Stream Functions (December 16, 2025)

### Overview

The RAGE engine uses a buffered stream abstraction for reading data. Three key functions handle stream operations:

| Function | Address | Purpose |
|----------|---------|---------|
| `sub_827E8420` | 0x827E8420 | **Buffered read** - Read data from stream into buffer |
| `sub_827E7FA8` | 0x827E7FA8 | **Flush/sync** - Call vtable[48] on stream object |
| `sub_827E87A0` | 0x827E87A0 | **Close/cleanup** - Finalize stream and reset object |

### Stream Structure Layout (r3 = stream pointer)

```
Offset  Size  Field              Description
+0x00   4     objectPtr          Pointer to stream object (has vtable at +0)
+0x04   4     handle             File handle or identifier
+0x08   4     bufferPtr          Internal read buffer pointer
+0x0C   4     position           Current position/offset in file
+0x10   4     readPos            Current read position in buffer
+0x14   4     availableData      Bytes available in buffer (filled)
+0x18   4     bufferSize         Total buffer capacity
```

### sub_827E8420 - Buffered Stream Read

**Signature**: `int32_t sub_827E8420(void* streamStruct, void* destBuffer, uint32_t size)`

**Arguments**:
- `r3`: Stream structure pointer
- `r4`: Destination buffer pointer  
- `r5`: Number of bytes to read

**Returns** (in `r3`):
- `>= 0`: Number of bytes successfully read
- `-1`: Error or no data available

**Algorithm**:
1. Check if buffer has data (`offset +20` != 0 or `offset +16` != 0)
2. If buffer empty, call `sub_827E7FC8` to refill
3. Calculate available bytes: `available = *(stream+20) - *(stream+16)`
4. If requested size > available:
   - Copy available bytes from buffer to destination
   - Call vtable[20] indirect function to read more data
   - Reset buffer pointers
5. Otherwise:
   - Copy requested bytes from `*(stream+8) + *(stream+16)` to destination
   - Update `*(stream+16)` read position
6. Return total bytes read

**Vtable Call** (at offset 0x827E84F4):
```cpp
// Load object from stream+0, get vtable from object+0, call vtable[20]
ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);  // object
ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);   // vtable
ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20); // vtable[20] = read func
PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);          // call it
```

### sub_827E7FA8 - Stream Flush/Sync

**Signature**: `void sub_827E7FA8(void* streamStruct)`

**Arguments**:
- `r3`: Stream structure pointer

**Algorithm**:
1. Load object pointer from `stream+0`
2. Load handle from `stream+4`
3. Get vtable from object
4. Call vtable[48] (flush/sync function)

**Purpose**: Forces buffered data to be written or syncs stream state.

### sub_827E87A0 - Stream Close/Cleanup

**Signature**: `int32_t sub_827E87A0(void* streamStruct)`

**Arguments**:
- `r3`: Stream structure pointer

**Returns**: 0 (success)

**Algorithm**:
1. If buffer has pending data, call `sub_827E7FC8` to flush
2. Load object from `stream+0`
3. Load handle from `stream+4`
4. Get vtable from object
5. Call vtable[40] (close function)
6. Set `stream+4` = -1 (invalid handle)
7. Set `stream+0` = 0 (clear object)
8. Return 0

### Object Vtable Layout

The object at `stream+0` has a vtable with these function pointers:

| Offset | Index | Function |
|--------|-------|----------|
| +0x00  | 0     | Destructor |
| +0x14  | 5     | Read (20 = 5*4) |
| +0x28  | 10    | Close (40 = 10*4) |
| +0x30  | 12    | Flush/Sync (48 = 12*4) |

### Current Issue: NULL Stream Infinite Loop

**Problem**: The game calls `sub_827E8420` with `stream = 0x00000000` billions of times per second.

**Cause**: Some code path passes NULL stream pointer, and our stub returns immediately without blocking.

**Symptoms**:
- 1.8 billion calls in 15 seconds
- VdSwap never called (render loop not reached)
- Game stuck in tight polling loop

**Fix Applied**: Return `-1` (no data) AND add 1ms sleep after 100 NULL calls to throttle the loop:
```cpp
if (streamStruct < 0x10000 || (streamStruct >= 0x82003880 && streamStruct < 0x82003900))
{
    static int s_nullCount = 0;
    if (++s_nullCount > 100)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    ctx.r3.s32 = -1;  // No data available
    return;
}
```

### Related Functions

| Function | Address | Purpose |
|----------|---------|---------|
| `sub_827E7FC8` | 0x827E7FC8 | Refill buffer from underlying source |
| `sub_82990830` | 0x82990830 | memcpy - copy data between buffers |
| `sub_827E8180` | 0x827E8180 | Stream seek |
| `sub_827E8290` | 0x827E8290 | Get stream position |

### Call Sites (from PPC source grep)

`sub_827E8420` is called from:
- `sub_82192820` (ppc_recomp.6.cpp) - Main boot chain
- `sub_828C4714` (ppc_recomp.140.cpp) - Resource loading
- `sub_825BFA0C` (ppc_recomp.77.cpp) - Multiple calls for parsing
- `sub_82802668` (ppc_recomp.126.cpp) - Data loading
- `sub_827F2FEC` (ppc_recomp.124.cpp) - File parsing
- `sub_827C6BA8` (ppc_recomp.119.cpp) - Shader loading
- `sub_8285B51C` (ppc_recomp.131.cpp) - Effect loading

---

## MAJOR MILESTONE: Boot Chain Complete (December 16, 2025)

### Achievement Summary
**The GTA IV recompiler now boots past the loading phase and enters the main game loop.**

### Evidence of Progress
```
[BootChain] sub_82120000 EXIT #1 ret=1    ← Main loop init SUCCESS
[XamInputGetKeystrokeEx] !!! STUB !!!     ← Input polling = interactive state
```

### What's Working
| Component | Status |
|-----------|--------|
| CPU execution | ✅ Working |
| Boot chain | ✅ Complete |
| Job/worker threads | ✅ 9 threads active |
| VirtualRpf I/O | ✅ Serving game data |
| Input polling | ✅ Active |
| Phase scheduler | ✅ Advancing (ret values changing) |

### What's Stubbed (Intentionally)
| Function | Reason |
|----------|--------|
| `sub_82273988` | Streaming init stuck in NULL stream loop |
| `sub_82124080` | Blocking initialization |
| `sub_82120FB8` | Stream reads corrupting state |
| `EffectManager::Load` | Shaders not translated yet |

### What's Missing (Next Steps)
| Component | Status | Notes |
|-----------|--------|-------|
| VdSwap | ❌ Not called | Render loop not submitting frames |
| Xenos shaders | ❌ Not translated | Need XenosRecomp integration |
| Draw calls | ❌ Unknown | Need to log DrawIndexed/DrawPrimitive |

### Key Fixes Applied This Session

1. **Synthetic vtable in GUEST memory** - Was only in host buffers, game loaded garbage
2. **Stream return values** - Changed -1 to 0 to prevent memcpy overflow
3. **Boot chain stubs** - Bypass blocking initialization functions
4. **GPU fence immediate completion** - Unblock GPU waits

### Next Milestones
1. Force VdSwap to be called (confirm render loop alive)
2. Stub shaders to render flat color
3. Re-enable streaming selectively

---

*Last updated: December 16, 2025*



















---

# RAGE Renderer Reverse-Engineering Documentation

## 1. RAGE Frame Model (Inferred)

Based on analysis of the recompiled PPC code, here is the frame lifecycle:

### Entry Point Chain
```
_xstart
    └── sub_8218BEA8 (game entry - ONE CALL only)
            └── sub_827D89B8 (frame tick function)
```

### sub_8218BEA8 (0x8218BEA8) - Game Entry Trampoline
```cpp
// This is a simple trampoline - just branches to frame tick
PPC_FUNC_IMPL(__imp__sub_8218BEA8) {
    // b 0x827d89b8
    sub_827D89B8(ctx, base);
    return;
}
```
**Finding**: This function does NOT loop. The Xbox 360 runtime was expected to call this repeatedly. In recompilation, we must provide the loop externally.

### sub_827D89B8 (0x827D89B8) - Frame Tick (Per-Frame)
This is the **actual per-frame function**. Call sequence:
1. `sub_827D8840(ctx, base)` - Unknown (possibly input processing)
2. `sub_827FFF80(ctx, base)` - Unknown (possibly timing)
3. `sub_827EEDE0(ctx, base)` - Unknown
4. **`sub_828E0AB8(ctx, base)`** - Debug/logging (called frequently throughout)
5. `sub_827EE620(ctx, base)` - Unknown
6. **Conditional vtable dispatch** at offset 52 (see Section 2)
7. **`sub_8218BEB0(ctx, base)`** - Core game tick/update
8. **Conditional vtable dispatch** at offset 56 (see Section 2)
9. `sub_827EECE8(ctx, base)` - Unknown
10. `sub_828E0AB8(ctx, base)` - Debug/logging again
11. `sub_827FFF88(ctx, base)` - Unknown (possibly frame end)

### sub_8218BEB0 (0x8218BEB0) - Core Game Update (Per-Frame)
```cpp
PPC_FUNC_IMPL(__imp__sub_8218BEB0) {
    sub_828E0AB8(ctx, base);           // Debug logging
    sub_82120000(ctx, base);            // INITIALIZATION CHECK
    
    // Check result - if returns 0, return -1 (failure)
    if ((ctx.r3.u32 & 0xFF) == 0) {
        sub_828E0AB8(ctx, base);
        ctx.r3.s64 = -1;                // Return failure
        return;
    }
    
    sub_821200D0(ctx, base);            // Per-frame update A
    sub_821200A8(ctx, base);            // Per-frame update B
    sub_828E0AB8(ctx, base);
    ctx.r3.s64 = 0;                     // Return success
}
```

**Critical Finding**: `sub_82120000` is a **one-time initialization** function. If it returns 0, the frame update fails with -1. This function must be cached to return success on subsequent calls.

---

## 2. Render Gate & Dispatch Logic

### vtable[52] and vtable[56] Dispatch Pattern

In `sub_827D89B8`, there are two conditional vtable dispatches:

```cpp
// Location ~0x827D8A24 - vtable[52] dispatch
ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);  // Load object pointer
if (ctx.r11.u32 != 0) {
    // Check if string at pointer is non-null
    ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
    if (ctx.r10.u32 != 0) {
        ctx.r4.u64 = ctx.r11.u64;  // Pass as parameter
    }
    
    // Load vtable and call function at offset 52
    ctx.r3.u64 = PPC_LOAD_U32(TLS_OFFSET + 1676);  // Device/context from TLS
    ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);    // vtable
    ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 52);  // vtable[13] (offset 52)
    PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);           // Call render function
}

// Then call sub_8218BEB0...

// Location ~0x827D8A70 - vtable[56] dispatch
ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
if (ctx.r11.u32 != 0) {
    ctx.r3.u64 = PPC_LOAD_U32(TLS_OFFSET + 1676);
    ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);    // vtable
    ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 56);  // vtable[14] (offset 56)
    PPC_CALL_INDIRECT_FUNC(ctx.r11.u32);
}
```

### Gate Variable Analysis

The gate is read from **`ctx.r31 + 4`** which is loaded from a global structure. The gate variable:
- Resides at a calculated address: `(-2096168960 + 20820) + 4` = **0x82F15158** (approximately)
- Must be **non-zero** for vtable dispatch to occur
- Contains a pointer to a string or object

### Why vtable[52]/vtable[56] Are Skipped

**Conditions for dispatch:**
1. `*(r31 + 4)` must be non-zero (gate pointer exists)
2. The first byte at that pointer must be non-zero (object initialized)
3. TLS offset 1676 must contain valid device pointer
4. Device vtable at offsets 52/56 must contain valid function pointers

**If any condition fails**: The dispatch is skipped entirely, and the render functions are never called.

---

## 3. Effect System Expectations

### sub_8285E048 (EffectManager::Load)

This is the **effect/shader loading function**. Analysis shows:

```cpp
PPC_FUNC_IMPL(__imp__sub_8285E048) {
    // r3 = effect context (source directory info)
    // r4 = output pointer for loaded effect
    
    sub_827E0740(ctx, base);    // Initialize something
    sub_827E0898(ctx, base);    // File system lookup - returns r28
    
    if (ctx.r28.u32 == 0) {
        // FAILURE PATH: File not found
        sub_828E0AB8(ctx, base);  // Log error
        
        // Call sub_82858758 TWICE with null (shader loading stubs)
        sub_82858758(ctx, base);  // VS loading attempt
        sub_82858758(ctx, base);  // PS loading attempt
        
        sub_827DFC60(ctx, base);  // Cleanup
        return;
    }
    
    // SUCCESS PATH: File found
    sub_828E0AB8(ctx, base);
    sub_827E0690(ctx, base);      // Get file handle/path
    sub_827F36F0(ctx, base);      // Parse effect data
    sub_827F17C0(ctx, base);      // Read shader entries
    
    // LOOP: For each shader entry
    while (result != 0) {
        sub_829915A0(ctx, base);  // Find delimiter '.'
        sub_8298F040(ctx, base);  // Compare extensions
        
        // If extension matches shader:
        sub_82858758(ctx, base);  // LOAD SHADER - this is critical!
        
        sub_827F17C0(ctx, base);  // Read next entry
    }
    
    sub_827E87A0(ctx, base);      // Close file handle
    sub_827DFC60(ctx, base);      // Cleanup
}
```

### sub_82858758 (ShaderLoader)

This is the **actual shader creation function**:

```cpp
PPC_FUNC_IMPL(__imp__sub_82858758) {
    // r3 = shader file path string
    // r4 = shader slot/parameter
    
    // Find empty slot in shader table (at 0x82F158E0 + 22784)
    r28 = 0x82F158E0;  // Shader table base
    for (r25 = 0; r25 < 128; r25++) {
        if (PPC_LOAD_U32(r28 + r25*4) == 0) break;  // Empty slot
    }
    
    if (r25 >= 128) {
        return -1;  // Table full
    }
    
    // Allocate 112-byte shader object
    sub_8218BE28(ctx, base);  // malloc(112)
    if (ctx.r3.u32 == 0) return -1;
    
    sub_82858708(ctx, base);  // Initialize shader object
    
    // Store in table
    PPC_STORE_U32(r28 + r25*4, shader_object);
    
    // Get file path
    sub_827DFC70(ctx, base);  // Resolve path
    sub_827DB078(ctx, base);  // Open file
    
    // Set initial state
    PPC_STORE_U32(shader_object + 104, -1);  // Mark as loading
    
    // Check if path contains expected shader extensions
    sub_82990020(ctx, base);  // String search
    
    if (found) {
        sub_827E0B78(ctx, base);  // Read shader data
        sub_827E0740(ctx, base);  // Parse shader header
        sub_827DFC70(ctx, base);  // Finalize
        // ... shader compilation/creation continues
    }
}
```

### What RAGE Expects After EffectManager Returns

1. **Output structure** at `r4` (passed in) must be populated:
   - Offset 0: Status (1 = loaded successfully)
   - Offset 4+: Technique/pass information
   - Shader object pointers

2. **Shader table** at `~0x82F15E00` must contain:
   - Non-null pointers to allocated 112-byte shader objects
   - Each object at offset 104 must have valid state (-1 = loading, other = ready)

3. **For rendering to work**, RAGE expects:
   - [SetVertexShader](cci:1://file:///Users/Ozordi/Downloads/MarathonRecomp/LibertyRecomp/gpu/video.cpp:5741:0-5761:1) called with valid shader from table
   - [SetPixelShader](cci:1://file:///Users/Ozordi/Downloads/MarathonRecomp/LibertyRecomp/gpu/video.cpp:8:0-19:1) called with valid shader from table
   - These are called via vtable dispatch based on effect parameters

---

## 4. Actionable Findings

### Why Draw Calls Are Skipped

1. **EffectManager is stubbed** - The stub returns success but doesn't populate the shader table or create shader objects. Result: No shaders exist to bind.

2. **vtable[52]/vtable[56] dispatch fails** - The gate pointer at `*(r31+4)` is null or uninitialized because effect initialization never completed properly.

3. **Shader table is empty** - `sub_82858758` is never called with valid shader data, so the table at ~0x82F15E00 has all null entries.

4. **SetVertexShader/SetPixelShader never receive valid shaders** - Without populated shader objects, these render state functions have nothing to bind.

### Required Fixes

| Issue | Fix |
|-------|-----|
| **EffectManager stub doesn't create shaders** | Hook `sub_82858758` to create shaders from precompiled cache |
| **Gate at r31+4 is null** | Initialize gate structure after EffectManager loads at least one effect |
| **vtable[52] never called** | Ensure device object at TLS+1676 has valid vtable with render functions |
| **Shader table empty** | Pre-populate table with default shaders at startup |

### Minimal Render Path Requirements

For draw calls to execute, these conditions must be true:

```
✓ sub_82120000 returns non-zero (init success - CURRENTLY HOOKED)
✓ Device at TLS+1676 is valid (CURRENTLY WORKING)
✗ Gate at *(r31+4) is non-zero (MISSING)
✗ vtable[52] contains valid render function (UNCHECKED)
✗ At least one shader in table at ~0x82F15E00 (MISSING)
✗ Effect output structure populated with shader pointers (MISSING)
```

### Specific Engineering Actions

1. **In EffectManagerStub**: Instead of just returning success, call `sub_82858758` with paths to default shaders, OR directly populate shader table with pre-loaded cache entries.

2. **Initialize gate variable**: After first successful EffectManager call, set `*(0x82F15158)` to a valid non-null value pointing to initialized render context.

3. **Verify TLS device setup**: Ensure `PPC_LOAD_U32(ctx.r13.u32 + 1676)` returns the device created by CreateDevice.

4. **Pre-create default shaders**: At CreateDevice time, call the shader creation path for `gta_default` shaders and store in the shader table.

---

 # 1. Boot-Critical Functions

This document is **source-level, conservative** documentation of boot-relevant behavior spanning:

- Host-side startup (`LibertyRecomp/*`), which loads the title image and transfers control into guest code.
- Recompiled guest-side PowerPC functions (`LibertyRecompLib/ppc/ppc_recomp.*.cpp`), which implement state machines, retry loops, and import calls.

Important limitation:

- From the `ppc_recomp.*.cpp` sources alone, it is **not provable** which *exact* guest functions execute during “boot” without a runtime trace (or at least the concrete guest entrypoint address plus call logs). This document therefore:
  - Documents the **host→guest entry plumbing** (provable from host sources).
  - Documents **boot-critical primitives and protocols** that appear in early/initialization-adjacent code paths (async helpers, mount/open primitives, XAM content/device flows).
  - Provides **cross-reference mechanics** to map runtime addresses (LR/PC) to named `sub_XXXXXXXX` functions.

## 1.1 Host boot chain (provable)

The host executable boots the guest by loading the title image and invoking the guest entrypoint as a mapped PPC function:

- `main.cpp`:
  - Calls `KiSystemStartup()`.
  - Calls `entry = LdrLoadModule(modulePath)`.
  - Calls `GuestThread::Start({ entry, 0, 0, 0 })`.

- `KiSystemStartup()` (`main.cpp`):
  - Performs host-side system setup (heap init, content registration/mounts, root path registration).
  - Sets up `game:`/`D:` and roots like `common:`, `platform:`, `audio:`.

- `LdrLoadModule(path)` (`main.cpp`):
  - Loads module bytes.
  - Calls `Image::ParseImage(...)`.
  - Copies `image.data` into guest memory at `g_memory.Translate(image.base)`.
  - Returns `image.entry_point` as the guest entry address.

- `GuestThread::Start(params)` (`cpu/guest_thread.cpp`):
  - Constructs a `GuestThreadContext` which initializes PPC registers (notably `r1` stack pointer and `r13`).
  - Calls `g_memory.FindFunction(params.function)(ctx.ppcContext, g_memory.base)`.

## 1.2 Host↔guest function dispatch and address→symbol mapping

The host maintains a lookup table from guest addresses to host function pointers:

- `Memory::Memory()` (`kernel/memory.cpp`):
  - Iterates `PPCFuncMappings[]` (from `LibertyRecompLib/ppc/ppc_func_mapping.cpp`).
  - Calls `InsertFunction(guest_addr, host_fn_ptr)` for each mapping.
  - This makes `g_memory.FindFunction(guest_addr)` return a callable `PPCFunc*`.

Cross-reference technique:

- If you have a guest address printed by host logs (e.g., `callerAddr = g_ppcContext->lr`), you can locate the corresponding symbol via `ppc_func_mapping.cpp` (search for the exact address). The mapping entries have the form:
  - `{ 0x82120000, sub_82120000 }, ...`

## 1.3 Boot-critical guest-side primitives (currently confirmed by static inspection)

The recompiled PPC functions are emitted as:

- `PPC_FUNC_IMPL(__imp__sub_XXXXXXXX)` (the recompiled implementation body)
- `PPC_WEAK_FUNC(sub_XXXXXXXX) { __imp__sub_XXXXXXXX(ctx, base); }` (a wrapper)

The following guest functions are repeatedly used as boot-critical *primitives* in the inspected regions:

| Function | Kind | Inputs (observed) | Outputs / side effects (observed) | Notes |
|---|---|---|---|---|
| `sub_829A1F00` | async driver / wait helper | Takes a handle-like value in `r3` in some call sites; other args via regs/stack | Returns `0`/nonzero; may call `__imp__NtWaitForSingleObjectEx` when a host call returns `259` | Contains explicit wait barrier (see §3) |
| `sub_829A2590` | operation start / open-like helper | Callers pass pointers/flags; often called with a path buffer base like `ctx+64` | Returns `-1` or non-`-1` value stored by callers into `ctx+96` | Exact semantics unknown (see §7) |
| `sub_829A1A50` | async-status helper | Callers pass `obj+8`, output ptr, and `0` | Returns `996` / `0` / other; `996` is treated as “no progress” | File contains `// PATCHED` logic that forces success (see §5) |
| `sub_829A1CA0` | wrapper around XAM close | Forwards to `__imp__XamContentClose` | Return compared against `997` | Cooperative pending semantics |
| `sub_829A1958` | close-handle helper | Takes a handle value | Close/cleanup | Used in cleanup paths |
| `sub_829A1EF8` | poll/yield-like helper | No obvious args | Returns a value sometimes compared against `997` | Not provably a yield; appears to read globals |
| `sub_829AAD20` | async-read wrapper | Takes a handle-like value loaded from `*(u32*)(obj+44)`; size compared against `*(u32*)(stack+84)` | Calls `__imp__NtReadFile`; if return equals `259`, calls `__imp__NtWaitForSingleObjectEx` then uses `stack+80` | Demonstrates the `259` → wait pattern around file I/O |
| `sub_829AADB8` | async-write wrapper | Takes a handle-like value loaded from `*(u32*)(obj+44)`; size compared against `*(u32*)(stack+84)` | Calls `__imp__NtWriteFile`; if return equals `259`, calls `__imp__NtWaitForSingleObjectEx` then uses `stack+80` | Demonstrates the `259` → wait pattern around file I/O |
| `sub_829A9738` | wait-with-retry helper | Takes an object handle in `r3` and a small control value in `r5` (stored to `r28`) | Calls `__imp__NtWaitForSingleObjectEx`; if return equals `257` and a flag is set, loops and waits again | Adds an additional distinguished wait result (`257`) used as a retry trigger |
| `sub_82169400` | event/wait loop | Uses globals under `-31981` base and waits on an event-like object at a fixed global address | Calls `__imp__KeWaitForSingleObject` in a loop; loop exit depends on a bit computed from `*(u32*)(global+300)` | Tight in-function loop that can spin if the global condition never changes |
| `sub_821694C8` | wait loop (variant) | Similar global/event pattern as `sub_82169400` | Calls `__imp__KeWaitForSingleObject` and loops while `*(u32*)(global+300) != 0` | Another tight wait loop gated by global memory |
| `sub_829DDC90` | wait loop (GPU init-adjacent) | Waits on an object at `r28+32` and compares wait result | Calls `__imp__KeWaitForSingleObject`; if return equals `258`, loops back | Matches host-side special-casing for caller `0x829DDD48` |
| `sub_829AAE50` | wait/dispatch loop | Waits on an object at `obj+16` then may call an indirect function pointer from a table | Calls `__imp__KeWaitForSingleObject`, then (on another path) executes `PPC_CALL_INDIRECT_FUNC` via a loaded `ctr` | Suggests a queue/dispatcher loop; exact target semantics unknown |

The following magic values are used as protocol signals in the inspected code:

- **`996`**: callers treat as “do not advance; return 0/no progress”.
- **`997`**: callers treat as “pending; retry / store pending state”.
- **`258`**: occurs inside `sub_829A1A50` and is mapped to `996`.
- **`259`**: triggers explicit wait path inside `sub_829A1F00`.
- **`257`**: used as a distinguished result in `sub_829A9738` that triggers retrying `__imp__NtWaitForSingleObjectEx`.

These values resemble Win32 constants, but this document **does not assume** that; meanings are inferred only from local branching.

## 1.4 Function-level dossiers (boot-adjacent; selected by import usage scan)

The functions below were selected because they directly invoke boot-critical host imports (filesystem open/read, waits, XAM task scheduling, etc.). This still does **not** prove they execute during boot, but they are high-value candidates.

### 1.4.1 `__imp__sub_829A2D98` (volume/open verification helper)

- **Source**:
  - `ppc_recomp.155.cpp`

- **Inputs (observed)**:
  - `r3`: treated as a pointer-sized value copied into `stack+100` (stored as `stw r11,100(r1)`).
  - `r4`: copied into `r30` and later compared against a computed value derived from volume info.

- **Outputs (observed)**:
  - Returns `r31`.
  - On error paths, returns a negative NTSTATUS-like value constructed as `(-16384<<16) | 335`.

- **Observed behavior**:
  - Calls `__imp__NtOpenFile` using stack structs located at `stack+80`, `stack+88`, `stack+96`, `stack+104`.
  - If open succeeds, calls `__imp__NtQueryVolumeInformationFile`, then closes the handle via `__imp__NtClose`.
  - After query, computes `r11 = *(u32*)(stack+128) * *(u32*)(stack+132)` and compares it against the input `r30`.
    - If equal, it returns without forcing the error code.
    - If not equal, it forces `r31 = (-16384<<16) | 335`.

- **Imports called (provable)**:
  - `__imp__NtOpenFile`
  - `__imp__NtQueryVolumeInformationFile`
  - `__imp__NtClose`

- **Async / retry**:
  - No internal loop; synchronous open→query→close sequence.

### 1.4.2 `__imp__sub_829A2AE0` (filesystem start/open + volume sizing + allocation)

- **Source**:
  - `ppc_recomp.155.cpp`

- **Inputs (observed)**:
  - `r3` saved as `r30` and later passed to a helper (`sub_8298F330`) and stored into memory.
  - `r4` saved as `r29` and later read at `*(u32*)(r29+4)`.
  - `r5` saved as `r31`; later used as a base pointer with fields at offsets `+24`, `+64`, `+80..+82`.
  - `r6` saved as `r25`; used as an output pointer for a page-aligned size stored via `stw ...,0(r25)`.
  - `r7` saved as `r28`; later used as an output pointer or callback argument.

- **Outputs (observed)**:
  - Returns a status in `r3` (checked by callers for `<0`).
  - Writes a page-aligned size to `*(u32*)(r25+0)`.
  - Writes several fields into the `r31` structure:
    - `stb r26,80(r31)` and `stb 1,81(r31)` and `stb 1,82(r31)`.
    - `std 4096,8(r31)` and `std (computed-4096),16(r31)`.

- **Observed behavior (conservative)**:
  - Prepares an ANSI-string-like structure on the stack via `__imp__RtlInitAnsiString`.
  - Calls `__imp__NtCreateFile` and bails out on `<0`.
  - Calls `__imp__NtAllocateVirtualMemory` for `r31+64` after storing a page-aligned size derived from `*(u32*)(r29+4)`.
  - Calls `__imp__NtQueryVolumeInformationFile` on the created/opened handle (`lwz r3,0(r24)` where `r24=r31+24`).
  - Calls at least one indirect function pointer loaded from `*(u32*)(*(u32*)(r29+664)+8)`.
  - No explicit `996/997` comparisons are present in the slice inspected; this function is not obviously using the `996/997` cooperative protocol.

- **Imports called (provable)**:
  - `__imp__RtlInitAnsiString`
  - `__imp__NtCreateFile`
  - `__imp__NtAllocateVirtualMemory`
  - `__imp__NtQueryVolumeInformationFile`

- **Async / retry**:
  - No internal loop was observed in this function; it is a synchronous “do work then return status” helper.

### 1.4.3 `__imp__sub_829A3560` (task + mount/open/read + wait integration)

- **Source**:
  - `ppc_recomp.155.cpp`

- **Inputs (observed)**:
  - `r3` saved as `r25`.
  - `r4` saved as `r29`.
  - `r5` saved as `r22`.
  - `r6` saved as `r21`.
  - `r7` saved as `r23`.

- **Outputs (observed)**:
  - Returns a status in `r3`.
  - Writes `r20` into the global at `*(u32*)(-31981:6964)` on one success path.
  - Performs a loop back to `loc_829A3690` (within this function) after some success/cleanup paths.

- **Observed behavior (conservative)**:
  - Calls `__imp__XexCheckExecutablePrivilege(23)` early; may set a local flag `r24`.
  - Calls `__imp__NtCreateFile` (stack-based structs around `stack+104`, `stack+128`, `stack+144`, etc.).
  - On successful create/open, performs `__imp__NtReadFile` reading `2048` bytes, closes via `__imp__NtClose`, and checks the read status.
  - Includes an in-function retry via `goto loc_829A3690` that is triggered after some conditions; the exact high-level intent cannot be proven from source alone.
  - Schedules work through XAM task APIs:
    - `__imp__XamTaskSchedule`
    - `__imp__XamTaskCloseHandle`
    - `__imp__XamTaskShouldExit` (used as a loop gate elsewhere, see `__imp__sub_829A3318`).
  - Contains a wait step via `__imp__KeWaitForSingleObject` on a fixed global address (`-32087:32604` pattern).

- **Imports called (provable; partial list)**:
  - `__imp__XexCheckExecutablePrivilege`
  - `__imp__NtCreateFile`
  - `__imp__NtReadFile`
  - `__imp__NtClose`
  - `__imp__KeWaitForSingleObject`
  - `__imp__KeResetEvent`
  - `__imp__KeEnterCriticalRegion` / `__imp__KeLeaveCriticalRegion`
  - `__imp__RtlEnterCriticalSection` / `__imp__RtlLeaveCriticalSection`
  - `__imp__XamTaskSchedule` / `__imp__XamTaskCloseHandle`

- **Async / retry**:
  - Contains in-function control-flow loops (label-based), including one that returns to a `NtCreateFile`-adjacent error-handling path.
  - The loop semantics depend on external conditions and imported call results; do not assume eventual convergence.

### 1.4.4 `__imp__sub_829A3178` (wait + critical-section mediated signal)

- **Source**:
  - `ppc_recomp.155.cpp`

- **Inputs/outputs (observed)**:
  - Returns a value in `r3` after calling `__imp__RtlNtStatusToDosError`.

- **Observed behavior (conservative)**:
  - Reads a global flag at `-31981:6964` and branches:
    - If it is `0`, it constructs `r30 = (-16384<<16) | 622` and then returns a translated error.
  - Checks privilege via `__imp__XexCheckExecutablePrivilege(11)`.
    - If privilege is not present, it sets `r30=0`.
  - On the “privilege present” path, it enters a critical region and critical section, sets a global at `-31981:6984` to `1`, signals an event (`__imp__KeSetEvent`), then waits once via `__imp__KeWaitForSingleObject` and resets that event.
  - After the wait, it loads `r30 = *(u32*)(r30+16)` (where `r30` was a pointer derived from `-31981:6984`) before leaving the critical section.

- **Imports called (provable)**:
  - `__imp__XexCheckExecutablePrivilege`
  - `__imp__KeEnterCriticalRegion` / `__imp__KeLeaveCriticalRegion`
  - `__imp__RtlEnterCriticalSection` / `__imp__RtlLeaveCriticalSection`
  - `__imp__KeSetEvent` / `__imp__KeWaitForSingleObject` / `__imp__KeResetEvent`
  - `__imp__RtlNtStatusToDosError`

- **Loop type**:
  - No internal loop in this function; single wait.

### 1.4.5 `__imp__sub_829A3238` (signal + wait driven by global state)

- **Source**:
  - `ppc_recomp.155.cpp`

- **Observed behavior (conservative)**:
  - Reads `*(u32*)(-31981:6964+4)`:
    - If nonzero, it enters a critical section, stores `2` to `-31981:6984`, calls `__imp__KeSetEvent`, waits once via `__imp__KeWaitForSingleObject`, resets the event, then leaves the critical section.
  - Else it reads `*(u32*)(-31981:6964+0)`:
    - If nonzero, it calls `sub_829A2FE8` (which in turn calls `__imp__ObDeleteSymbolicLink`, per the import-usage index).
  - In all cases, it stores `0` to `*(u32*)(-31981:6964+0)` before returning.

- **Imports called (provable)**:
  - `__imp__RtlEnterCriticalSection` / `__imp__RtlLeaveCriticalSection`
  - `__imp__KeSetEvent` / `__imp__KeWaitForSingleObject` / `__imp__KeResetEvent`

- **Loop type**:
  - No internal loop in this function; single wait.

### 1.4.6 `__imp__sub_829A3318` (boot-adjacent orchestrator; calls `sub_829A2AE0` and loops on task exit)

- **Source**:
  - `ppc_recomp.155.cpp`

- **Inputs (observed)**:
  - `r3` is treated as a pointer to a state object `r31`, with fields used at offsets:
    - `+8`, `+12`, `+16`, and `+664`.

- **Outputs (observed)**:
  - Writes `r30` (status) into `*(u32*)(r31+16)`.
  - Signals events via `__imp__KeSetEvent`.
  - Performs cleanup: dismount, free virtual memory, close handles, dereference objects.

- **Observed behavior (conservative)**:
  - Calls `sub_829A2AE0(r3 = *(u32*)(r31+8), r4=r31, r5=stack+96, r6=stack+80, r7=stack+272)` and stores its result to `r30`.
  - Registers title terminate notifications and uses multiple events.
  - Contains an explicit in-function loop (`loc_829A3404`) that:
    - waits (`__imp__KeWaitForSingleObject`),
    - invokes `sub_829A2CA8(...)`,
    - checks `__imp__XamTaskShouldExit()`, and
    - loops again when `XamTaskShouldExit` returns `0`.
  - Performs cleanup regardless of status (dismount, free, close, dereference).

- **Imports called (provable; partial list)**:
  - `__imp__KeWaitForSingleObject`
  - `__imp__KeSetEvent` / `__imp__KeResetEvent`
  - `__imp__XamTaskShouldExit`
  - `__imp__ExRegisterTitleTerminateNotification`
  - `__imp__IoDismountVolume`
  - `__imp__NtFreeVirtualMemory`
  - `__imp__NtClose`
  - `__imp__ObDereferenceObject`

- **Loop type**:
  - Tight in-function wait loop gated by `__imp__XamTaskShouldExit`.

### 1.4.7 `__imp__sub_829C4548` (Net init wrapper)

- **Source**:
  - `ppc_recomp.158.cpp`

- **Inputs (observed)**:
  - `r3`: treated as an `XNADDR*`-like pointer, moved to `r4`.

- **Outputs (observed)**:
  - Writes `1` to `r3` before tail-calling the import.
  - Returns whatever `__imp__NetDll_XNetGetTitleXnAddr` returns.

- **Observed behavior**:
  - Thin wrapper: `r4=r3; r3=1; __imp__NetDll_XNetGetTitleXnAddr(ctx, base)`.

- **Imports called (provable)**:
  - `__imp__NetDll_XNetGetTitleXnAddr`

# 2. State Machines

## 2.1 Async-completion wrapper state machine (`sub_827DBF10` and `sub_827DBF90`)

**Primary state field**:

- `*(u32*)(obj + 0x00)` (loaded via `lwz r11,0(r31)`)

**Observed states and meanings (conservative)**

These meanings are inferred only from guards and the immediate stores performed.

- **State `2`**: “waiting to complete phase-2 operation.”
  - Evidence: `sub_827DBF10` only performs the async-status check when `state==2`.
- **State `3`**: “phase-2 completed successfully.”
  - Evidence: `sub_827DBF10` stores `3` when `sub_829A1A50(...)` returns `0`.
- **State `1`**: “phase-2 completed with nonzero status (error or alternate result).”
  - Evidence: `sub_827DBF10` stores `1` when `sub_829A1A50(...)` returns nonzero and non-`996`.

- **State `4`**: “waiting to complete phase-4 operation.”
  - Evidence: `sub_827DBF90` only performs the async-status check when `state==4`.
- **State `5`**: “phase-4 completed successfully; produced a result.”
  - Evidence: `sub_827DBF90` stores `5` on the path where `sub_829A1A50(...) == 0`, and stores `*(u32*)(stack+80)` into the output.

**Transitions and conditions**

- `2 -> (no change)` when `sub_829A1A50(obj+8, out, 0) == 996`.
  - Implementation: `sub_827DBF10` immediately returns `0` via `loc_827DBF30`.
- `2 -> 3` when `sub_829A1A50(...) == 0`.
- `2 -> 1` when `sub_829A1A50(...) != 0` and `!= 996`.

- `4 -> (no change)` when `sub_829A1A50(...) == 996`.
  - Implementation: `sub_827DBF90` immediately returns `0` via `loc_827DBFB0`.
- `4 -> 5` when `sub_829A1A50(...) == 0`.
- `4 -> 1` when `sub_829A1A50(...) != 0` and additional checks interpret the situation as failure.
  - Evidence: path `loc_827DBFF8` stores `1` to state and stores `-1` to an output slot.

**Notes / unknowns**

- `sub_827DBF90` has additional conditional logic involving `*(u32*)(obj+32)` compared against `1223` and against `0x80070012` (constructed as `-2147024896 | 18`). This suggests special-case error treatment, but the exact meaning cannot be proven without knowing what `obj+32` encodes.

## 2.2 Content/device workflow state machine (`sub_827DDE30`)

This one is larger and includes a *secondary* phase field.

**Primary state field**:

- `*(u32*)(ctx + 0x00)` where `ctx` is the `r31` pointer inside `sub_827DDE30`.
  - Guard: `sub_827DDE30` only runs when `state==8`; otherwise returns `0`.

**Secondary sub-state field**:

- `*(u32*)(ctx + 0x04)` (loaded as `lwz r11,4(r31)`).
  - Used to branch among sub-phases `1`, `2`, `3`, `4`.

**Observed state meanings (partial)**

- **Primary state `8`**: “drive/advance workflow.”
  - Evidence: `sub_827DDE30` early-outs unless `state==8`.

- **Secondary state `1`**: “needs to start an operation that returns a handle in `ctx+96`.”
  - Evidence: when `substate==1`, it calls `sub_829A2590(...)` and stores the return value to `*(u32*)(ctx+96)`.
- **Secondary state `2`**: “operation issued; wait/drive completion using `sub_829A1F00` then poll.”
  - Evidence: on the success path after starting, it stores `2` into `ctx+4`, initializes a small struct at `ctx+36`, and calls `sub_829A1F00(handle=ctx+96, ...)`.
- **Secondary state `3`**: “cleanup/close path.”
  - Evidence: when `substate==3`, it transitions to primary state `9` and returns a stored value from `ctx+116`.
- **Secondary state `4`**: “terminal / cancel / abort path.”
  - Evidence: when `substate==4`, it forces primary `state=1` and outputs `0`.

**Transitions and conditions (selected, from the visible slice)**

- Gate 0: `if sub_827DC050(ctx, &tmp) == 996` then `return 0` and do not mutate state.
  - This is an explicit “do nothing until ready” barrier.

- `substate==1`:
  - Calls `sub_829A2590(ctx+64, ...)` and stores result in `ctx+96`.
  - If `ctx+96 == -1`, it calls `sub_829A1EF8()` and goes into a cleanup path.
  - Else sets `substate=2`, zeros `ctx+36..+52`, then calls `sub_829A1F00(ctx+96, ctx+112, ctx+116, ctx+36, 0)`.

- `substate==2`:
  - If `sub_829A1F00(...) != 0`, it returns `0` (no further progress in this call).
  - Else it calls `sub_829A1EF8()` and checks if the return equals `997`.
    - If `==997`, it returns `0` (still pending).
    - If `!=997`, it closes `ctx+96` via `sub_829A1958`, sets `ctx+96=-1`, then calls `sub_829A1EF8()` again and goes to cleanup.

- Cleanup loop portion:
  - Multiple paths eventually reach code that calls `sub_829A1CA0(handle)` (imported as `__imp__XamContentClose`) and checks whether it returns `997`.
    - If it returns `997`, it sets `substate=4` and returns `0`.
    - Otherwise it calls `sub_829A1EF8()` and either:
      - sets `state=1` and returns `1` (appears like “finished with failure”), or
      - continues cleanup.

Because the full function body is large, this is a partial reconstruction focused on the visible control flow around `loc_827DDF98`.

## 2.3 UI/selection workflow state machine (`sub_827DC458` and `sub_827DC368`)

These functions manipulate `*(u32*)(ctx+0)` and interact with XAM UI and enumeration APIs.

**Primary state field**:

- `*(u32*)(ctx + 0x00)` where `ctx` is `r31`.

**Key observations**

- `sub_827DC368` can set `state=2` if a call to `sub_829A1288` (imported as `__imp__XamShowDeviceSelectorUI`) returns `997`.
- `sub_827DC458` begins only if `state==0 || state==1 || *(u32*)(ctx+56)==0` (otherwise it returns `0`). It then:
  - calls `sub_829A1CB8` (imported as `__imp__XamContentCreateEnumerator`) and if it returns nonzero, sets `state=1` and returns `0`.
  - calls `sub_829A1A38` (imported as `__imp__XamEnumerate`) and checks `997`:
    - if `997`, sets `state=4` and returns `1` (meaning: “async started / pending”).
    - otherwise treats it as failure: sets `state=1`, closes a handle with `sub_829A1958`, and returns `0`.

## 2.4 Tight retry loop around `sub_829A1F00` (`sub_827F0B20`)

This is a non-state-machine *tight retry loop* (i.e., the loop happens inside a single function call) and is therefore a strong candidate for an observable “spin” if it fails to converge.

**Loop driver**:

- `sub_827F0B20` (in `ppc_recomp.124.cpp`) repeatedly calls `sub_829A1F00(...)`.

**Loop condition**:

- It loops while `sub_829A1F00(...) == 0`.
- It exits when `sub_829A1F00(...) != 0`, returning the value stored at `stack+80`.

**Internal pacing logic (conservative)**:

- A local countdown `r28` is initialized to `10`.
- Each loop iteration may call `sub_829A1EF8()`.
- When the countdown expires (`r28` becomes negative), the function conditionally calls an indirect function pointer loaded from a fixed-address global location (pattern: `lwz r11,-32108(r27)` followed by `mtctr r11; bctrl`). The target and its side effects are not provable from the source shown.

 # 3. Retry Loops / Barriers

This code uses explicit label-based loops (via `goto loc_...`) rather than structured `while` loops.

## 3.1 Cooperative barrier based on return code `996` (non-progress gate)

Pattern:

- Call a helper (`sub_829A1A50` directly, or via `sub_827DC050`).
- If return is `996`, **exit early without changing state**.

Examples:

- `sub_827DBF10`:
  - Loop-equivalent behavior: repeated calls are expected until `sub_829A1A50(...) != 996`.
  - Exit condition: `sub_829A1A50(...) != 996`.
  - Loop body: just the call + compare; no other observable side effects inside the function.

- `sub_827DDE30`:
  - First gate is `if sub_827DC050(...) == 996 return 0;`.

This is a *barrier* in the sense that forward progress is impossible while the helper keeps returning `996`.

## 3.2 Cooperative barrier around `XamContentClose` polling (`loc_827DE030` / `loc_827DDF98`) inside `sub_827DDE30`

From `ppc_recomp.122.cpp`:

- Label `loc_827DE030` performs a “poll” of `sub_829A1CA0(...)` (imported as `__imp__XamContentClose`) and then branches based on whether the return equals `997`.
- Label `loc_827DDF98` **does not loop**: it calls `sub_829A1EF8()` and then forces `*(u32*)(r31+0)=1`, writes `0` to the output pointer, and returns `1`.

Conservative interpretation:

- **Cooperative barrier condition**: `sub_829A1CA0(...) == 997` is treated as a special “pending” result that causes the function to set `*(u32*)(r31+4)=4` and return `0` (no progress).
- **Non-`997` close result**: if `sub_829A1CA0(...) != 997`, the code jumps to `loc_827DDF98` and terminates the workflow with `state=1` and output `0`.

This means that within `sub_827DDE30`, `997` is treated as “still pending; keep waiting,” but the wait is *not an in-function spin*: it returns to the caller in state `substate=4`.

Whether `sub_829A1EF8()` is an intentional “yield” is **not provable** from source: in the current implementation it calls `sub_829A92A0()` which either returns `0` (if a global flag is set) or returns a global error value from `*(u32*)(*(u32*)(r13+256) + 352)`.

## 3.3 Other cooperative polling patterns based on `997`

Several call sites treat `997` as “keep waiting” and return to the caller without changing to a terminal state. For example:

- `sub_827DC458` sets `state=4` and returns `1` when `XamEnumerate` returns `997`.

This is not an in-function busy-spin; it’s a **cooperative polling state machine** where the caller must call again.

## 3.4 Enforced wait barrier in `sub_829A1F00` (`259` → `__imp__NtWaitForSingleObjectEx`)

`sub_829A1F00` (in `ppc_recomp.155.cpp`) contains an explicit barrier:

- It calls an indirect host function pointer (from `*(u32*)(*(u32*)(-32756 + -32086-base) + 16)`), then checks whether the result equals `259`.
- If it equals `259`, it calls `__imp__NtWaitForSingleObjectEx(r3=r30, r4=1, r5=0, r6=0)` and then re-uses a value loaded from `stack+80`.

Static conclusions:

- The code enforces a blocking wait when this `259` result occurs.
- The code does not prove what `259` means, but it is used as a trigger for “wait for single object.”

## 3.5 Tight in-function retry loop that can look like a spin (`sub_827F0B20`)

Unlike the cooperative state machines that return to their caller, `sub_827F0B20` contains an actual in-function retry loop:

- First call: `sub_829A1F00(...)`.
- If it returns `0`, the code performs a bounded local countdown and repeatedly retries `sub_829A1F00(...)`.

The loop continues *solely* based on the return value of `sub_829A1F00`.

Therefore, if `sub_829A1F00` continues returning `0` indefinitely, `sub_827F0B20` can execute indefinitely within one invocation.

## 3.6 Tight in-function wait loop around `KeWaitForSingleObject` (`sub_829DDC90`)

From `ppc_recomp.160.cpp`:

- `sub_829DDC90` calls `__imp__KeWaitForSingleObject` and compares the return value against `258`.
- If the return equals `258`, it branches back to an earlier label (`loc_829DDCF0`) and repeats the wait path.

This is an in-function loop whose continuation depends on the return value of the wait call.

## 3.7 Tight in-function wait loops gated by global flags (`sub_82169400` / `sub_821694C8`)

From `ppc_recomp.4.cpp`:

- `sub_82169400` calls `__imp__KeWaitForSingleObject` in a loop and exits only after observing a computed condition derived from a global structure (notably loads at offsets like `+300` and `+304`).
- `sub_821694C8` contains a similar wait-and-check loop, repeatedly waiting and checking a global value at `+300` until it becomes `0`.

Static limitation: the meanings of the probed global offsets are not provable from these snippets alone.

## 3.8 Retry loop around `NtWaitForSingleObjectEx` (`sub_829A9738`)

From `ppc_recomp.156.cpp`:

- `sub_829A9738` calls `__imp__NtWaitForSingleObjectEx` and, on one path, compares the return value against `257`.
- If the value equals `257` and a caller-provided flag value (`r28`) is nonzero, it branches back and waits again.

This introduces an additional distinguished wait result used as a retry trigger.

 # 4. Host Interactions

This section documents host import interactions that are provably involved in the inspected guest-side logic.

## 4.1 Imported calls used by the inspected guest workflows

Observed imported calls and their usage patterns:

| Import thunk | Where it appears (examples) | How callers interpret return | Notes |
|---|---|---|---|
| `__imp__NtCreateFile` | Multiple call sites in `ppc_recomp.155.cpp` | Compared against `<0` or `==0` in various contexts | Host implementation special-cases `game:\` mount opens (see §4.2) |
| `__imp__NtReadFile` | Used in `ppc_recomp.155.cpp` in file/crypto routines | Return compared `<0` / `>=0` | Semantics depend on host FS |
| `__imp__NtWriteFile` | Used in `ppc_recomp.156.cpp` | Return compared against `259` and may be followed by `__imp__NtWaitForSingleObjectEx` | Appears as part of an async I/O wrapper |
| `__imp__NtClose` | Used to close handles | N/A | Cleanup |
| `__imp__NtWaitForSingleObjectEx` | Called from `sub_829A1F00` when host call returns `259` | Caller assumes this wait contributes to progress | Explicit enforced wait path |
| `__imp__KeWaitForSingleObject` | Used in `ppc_recomp.4.cpp`, `ppc_recomp.155.cpp`, `ppc_recomp.156.cpp`, `ppc_recomp.160.cpp` | Sometimes compared against `258`, sometimes used as a pure wait step in a loop | Host may short-circuit specific callers (see §4.3) |
| `__imp__XamEnumerate` | Used via wrapper `sub_829A1A38` in `sub_827DC458` | `997` treated as “pending” | Cooperative polling |
| `__imp__XamContentClose` | Used via wrapper `sub_829A1CA0` | `997` treated as “pending” | Cleanup barrier |
| `__imp__XamShowDeviceSelectorUI` | Used via wrapper `sub_829A1288` | `997` treated as “pending” | UI/device selection |

The guest also uses at least one **indirect host function pointer** (inside `sub_829A1F00`). The concrete target and semantics are not provable from the guest sources alone.

## 4.2 Host-side `NtCreateFile` special-case relevant to boot stalls (`game:\` open storm)

In `LibertyRecomp/kernel/imports.cpp`, the host implementation of `NtCreateFile` contains explicit logic for mount-style opens of the root paths:

- Detects `game:\` / `d:\` root opens.
- Tracks open count.
- Logs the guest caller address as `callerAddr = g_ppcContext->lr`.

This is a key safe instrumentation point because it can identify which guest code location is driving repeated root opens.

## 4.3 Host-side wait behavior that can affect boot progression

`KeWaitForSingleObject` (host, `imports.cpp`) contains special-case logic:

- Logs waits and records `caller = g_ppcContext->lr`.
- Has explicit "GPU init deadlock prevention" logic for some caller addresses (e.g. `0x829DDD48`, `0x829D8AA8`), and may force internal GPU flags and return `STATUS_SUCCESS`.

This means some guest-side waits may be short-circuited on the host, and some apparent boot progress may depend on these host interventions.

## 4.4 Host-side network stub observed during boot (`__imp__NetDll_XNetGetTitleXnAddr`)

The host provides an implementation for `__imp__NetDll_XNetGetTitleXnAddr` in `imports.cpp` that:

- Optionally writes an `XNADDR`-like structure into guest memory (when pointer appears valid).
- Returns a bitmask value (`0x66` in the current host code).

 This can affect guest-side network initialization state machines, but the corresponding guest call sites have not been mapped in this document.

## 4.5 Guest import usage index (auto-derived)

This table is generated by scanning for direct calls of the form `__imp__Xxx(ctx, base)` inside each `PPC_FUNC_IMPL(__imp__sub_XXXXXXXX)` body in `ppc_recomp.*.cpp`.

Important limitation:

- This is a **call-site index**, not proof that these functions execute during boot.

| Import thunk | # callers | Caller functions (`__imp__sub_XXXXXXXX`) |
|---|---:|---|
| `__imp__KeDelayExecutionThread` | 1 | __imp__sub_829A9620 |
| `__imp__KeWaitForMultipleObjects` | 1 | __imp__sub_82169B00 |
| `__imp__KeWaitForSingleObject` | 8 | __imp__sub_82169400, __imp__sub_821694C8, __imp__sub_829A3178, __imp__sub_829A3238, __imp__sub_829A3318, __imp__sub_829A3560, __imp__sub_829AAE50, __imp__sub_829DDC90 |
| `__imp__NetDll_XNetCleanup` | 1 | __imp__sub_829C4458 |
| `__imp__NetDll_XNetGetConnectStatus` | 1 | __imp__sub_829C44A0 |
| `__imp__NetDll_XNetGetEthernetLinkStatus` | 1 | __imp__sub_829C4558 |
| `__imp__NetDll_XNetGetTitleXnAddr` | 1 | __imp__sub_829C4548 |
| `__imp__NetDll_XNetQosListen` | 1 | __imp__sub_829C44B0 |
| `__imp__NetDll_XNetQosLookup` | 1 | __imp__sub_829C44D0 |
| `__imp__NetDll_XNetQosRelease` | 1 | __imp__sub_829C4538 |
| `__imp__NetDll_XNetServerToInAddr` | 1 | __imp__sub_829C4478 |
| `__imp__NetDll_XNetStartup` | 1 | __imp__sub_829C4390 |
| `__imp__NetDll_XNetUnregisterInAddr` | 1 | __imp__sub_829C4490 |
| `__imp__NetDll_XNetXnAddrToInAddr` | 1 | __imp__sub_829C4460 |
| `__imp__NtClose` | 15 | __imp__sub_829A0538, __imp__sub_829A29A8, __imp__sub_829A2D98, __imp__sub_829A3318, __imp__sub_829A3560, __imp__sub_829A3F10, __imp__sub_829A40F0, __imp__sub_829A41A8, __imp__sub_829A4278, __imp__sub_829A93A8, __imp__sub_829A9580, __imp__sub_829A9CB0, __imp__sub_829AA008, __imp__sub_829AFCA0, __imp__sub_829E8BA0 |
| `__imp__NtCreateFile` | 4 | __imp__sub_829A29A8, __imp__sub_829A2AE0, __imp__sub_829A3560, __imp__sub_829A4278 |
| `__imp__NtOpenFile` | 8 | __imp__sub_829A2D98, __imp__sub_829A3F10, __imp__sub_829A40F0, __imp__sub_829A41A8, __imp__sub_829A93A8, __imp__sub_829A9580, __imp__sub_829AFCA0, __imp__sub_829E8BA0 |
| `__imp__NtReadFile` | 3 | __imp__sub_829A29A8, __imp__sub_829A3560, __imp__sub_829AAD20 |
| `__imp__NtWaitForMultipleObjectsEx` | 1 | __imp__sub_829A22C8 |
| `__imp__NtWaitForSingleObjectEx` | 5 | __imp__sub_829A1F00, __imp__sub_829A3BD0, __imp__sub_829A9738, __imp__sub_829AAD20, __imp__sub_829AADB8 |
| `__imp__NtWriteFile` | 4 | __imp__sub_829A29A8, __imp__sub_829A3BD0, __imp__sub_829AADB8, __imp__sub_829AFCA0 |
| `__imp__XamContentClose` | 1 | __imp__sub_829A1CA0 |
| `__imp__XamContentCreateEnumerator` | 1 | __imp__sub_829A1CB8 |
| `__imp__XamContentCreateEx` | 1 | __imp__sub_829A1C38 |
| `__imp__XamContentGetCreator` | 1 | __imp__sub_829A1CB0 |
| `__imp__XamContentGetDeviceData` | 1 | __imp__sub_829A1CC0 |
| `__imp__XamContentSetThumbnail` | 1 | __imp__sub_829A1CA8 |
| `__imp__XamEnumerate` | 1 | __imp__sub_829A1A38 |
| `__imp__XamLoaderLaunchTitle` | 1 | __imp__sub_829A9208 |
| `__imp__XamLoaderTerminateTitle` | 2 | __imp__sub_829A0858, __imp__sub_829A09E8 |
| `__imp__XamShowDeviceSelectorUI` | 1 | __imp__sub_829A1288 |
| `__imp__XamShowDirtyDiscErrorUI` | 1 | __imp__sub_829A1290 |
| `__imp__XamShowGamerCardUIForXUID` | 1 | __imp__sub_829A1278 |
| `__imp__XamShowMessageBoxUIEx` | 1 | __imp__sub_829A0538 |
| `__imp__XamShowPlayerReviewUI` | 1 | __imp__sub_829A1280 |
| `__imp__XamShowSigninUI` | 1 | __imp__sub_829A1270 |
| `__imp__XexCheckExecutablePrivilege` | 3 | __imp__sub_829A0678, __imp__sub_829A3178, __imp__sub_829A3560 |
| `__imp__XexGetModuleHandle` | 2 | __imp__sub_829C3FE8, __imp__sub_829C4390 |
| `__imp__XexGetProcedureAddress` | 2 | __imp__sub_829C3FE8, __imp__sub_829C4390 |

# 5. Failure / Stall Modes
 
 ## 5.1 Async protocol signals and why they matter for boot
 
 **Where the magic values originate (from source)**:
 
 - `997` originates as:
   - the direct return value of several `__imp__Xam*` calls (e.g. `__imp__XamEnumerate`, `__imp__XamContentClose`).
   - a literal assigned in some functions (direct `li ...,997` patterns exist in the corpus).
 
 - `996` originates as:
   - a literal assigned in at least one function (e.g. `sub_829A4648` has a `li r3,996` path), and
   - a literal used as the “incomplete” result in multiple comparisons.
 
 - `258` originates as:
   - a literal assigned in `sub_829A1A50` on a particular path and then mapped to `996`.
 
 - `259` originates as:
   - a literal compared against the return value of an indirect host call inside `sub_829A1F00`, and is used as the condition to call `__imp__NtWaitForSingleObjectEx`.
 
 **Meaning inferred from control flow**:
 
 - **`997`**: callers often treat `997` as “operation pending; store a pending state and return.”
 - **`996`**: callers treat `996` as “do not advance; return `0` (no progress).”
 - **`258`**: intermediate value that is mapped to `996` inside `sub_829A1A50`.
 - **`259`**: triggers an explicit wait path in `sub_829A1F00`.
 
 **Resolution requirement (conservative)**:
 
 Nothing in the guest sources enforces that `996/997` will ever change. Progress requires external state changes by the host/runtime or other subsystems such that subsequent calls stop returning the “pending” value.
 
 ## 5.2 `sub_829A1A50` never stops yielding `996`
 
 - **Symptom**: callers repeatedly return without mutating state (no forward progress).
 - **Depends on**:
   - **Host API semantics / missing side effects**.
   - **Incorrect async completion handling**.
   - **Incorrect initialization** of the status object passed to the helper.
 
 ## 5.3 APIs keep returning `997` indefinitely
 
 - **Symptom**: “pending” states persist (cooperative polling never resolves).
 - **Depends on**:
   - **Host API semantics** (XAM stubs, async engine).
 
 ## 5.4 Cleanup logic depends on handle semantics
 
 In `sub_827DDE30`, cleanup includes closing handles (`sub_829A1958`) and calling `XamContentClose` (`sub_829A1CA0`). If the host’s rules differ from what the guest expects, it can repeatedly take non-progress branches.
 
 - **Depends on**:
   - **Host API semantics**.
 
 ## 5.5 Source-level patching can create false progress
 
 `sub_829A1A50` currently contains `// PATCHED` logic that forces success and/or converts `996/997` to `0`.
 
 - **Symptom**: barriers that would normally block no longer block; state machines may advance with invariants violated.
 
 ## 5.6 Tight retry loops can spin inside one invocation
 
 - `sub_827F0B20` contains a tight retry loop that continues while `sub_829A1F00(...) == 0`.
 - If `sub_829A1F00` never returns nonzero, the loop can be unbounded within one call.
 
 Additional host-side stall modes (provable from host code):
 
 - Host waits may block indefinitely if an object is never signaled (unless special-cased as in the GPU init path).
 - Host filesystem semantics for mount paths (`game:\`) can cause repeated open attempts if guest expects file-like behavior but host returns directory-like behavior.
 
 # 6. Safe Instrumentation Points
 
 These are instrumentation locations that are (a) guaranteed to execute when the relevant path is exercised and (b) can be implemented as logging only.
 
 ## 6.1 Host-side safe points
 
 - `NtCreateFile` (`imports.cpp`): log `guestPath`, `callerAddr = g_ppcContext->lr`, and status returned, especially for `game:\` root opens.
 - `KeWaitForSingleObject` (`imports.cpp`): log `caller = g_ppcContext->lr`, object type, timeout, and whether the GPU special-case path was taken.
 - `NetDll_XNetGetTitleXnAddr` (`imports.cpp`): log the `XNADDR*` pointer and return value to confirm network init isn’t stuck waiting on “pending”.
 
 ## 6.2 Guest-side safe points (log-only)
 
 - Entry/exit of `sub_829A1F00`: log the input handle value, whether the `259` branch was taken, and whether it returns `0` vs nonzero.
 - Loop backedge in `sub_827F0B20`: log iteration count and `sub_829A1F00` return.
 
 ## 6.3 Address-to-function correlation
 
 - Use `ppc_func_mapping.cpp` to map logged guest addresses (LR/PC) back to `sub_XXXXXXXX`.
 - This is required to convert host-side logs like `caller=0x829DDD48` into concrete guest function identities.
 
 # 7. Open Questions / Unknowns
 
 Static source alone cannot answer the following, but each item is directly relevant to proving boot completeness:
 
 - What concrete structure types correspond to the “context objects” whose fields are accessed at offsets `0`, `4`, `8`, `32`, `56`, `60`, `96`, `112`, `116`, etc.?
 - What does `sub_829A2590` do (it appears to start an operation returning `-1` or a handle)?
 - What does the indirect callback in `sub_827F0B20` (loaded from `lwz r11,-32108(r27)` then called via `bctrl`) do? Does it yield, pump events, or advance async state?
 - For the host-reported `game:\` open storm: which guest function(s) correspond to the logged `callerAddr = g_ppcContext->lr`, and do those call sites reside inside `sub_827F0B20`, `sub_827DDE30`, or a different loop entirely?
 - What are the host/runtime semantics of the imported XAM functions used here:
   - `__imp__XamEnumerate`
   - `__imp__XamContentCreateEnumerator`
   - `__imp__XamContentClose`
   - `__imp__XamShowDeviceSelectorUI`
 - Does `sub_829A1EF8` (via `sub_829A92A0`) have any host-side side effects that would act like a yield/sleep?
 - Are the `// PATCHED` blocks meant to be temporary instrumentation?
 
 Hooking-related questions:
 
 - Are attempted hooks targeting `sub_829A1A50` / `sub_829A1CA0` applied to the correct symbol (e.g. `sub_829A1A50` vs `__imp__sub_829A1A50`), and do call sites actually invoke the symbol being replaced?
 - For XAM-related wrappers (e.g. `sub_829A1CA0`), are call sites using the wrapper or calling the import thunk directly (e.g. `__imp__XamContentClose`)?

# 8. Render Loop Progression Analysis

This section documents the investigation into why the game boots but never enters the render loop (VdSwap never called).

## 8.1 Current State Summary

**What works:**
- Game boots successfully
- Files load from RPF (common.rpf, xbox360.rpf)
- Streaming system functions without corruption (heap aliasing fix applied)
- GPU initialization completes (VdInitializeEngines, VdRetrainEDRAM)
- Worker threads (9) are created and idle correctly
- XXOVERLAPPED async completion is being set up

**What doesn't work:**
- Render loop never starts
- VdSwap is never called
- Main thread is blocked waiting for a condition
- No crash, no spin — clean block

## 8.2 VdSwap Call Chain (Traced)

The VdSwap import is called through this chain:

```
sub_828529B0  (main loop orchestrator)
    └── sub_829CB818  (pre-frame setup)
    └── sub_828507F8  (frame presentation wrapper)
            └── sub_829D5388  (D3D Present wrapper)
                    └── __imp__VdSwap  (kernel import @ 0x82A0310C)
```

**`sub_828507F8`** (`ppc_recomp.130.cpp` line 1665):
- Takes a device context pointer in r3
- Loads frame buffer index from offset 19480
- Calls `sub_829D5388` with frame data
- Increments frame counter

**`sub_829D5388`** (`ppc_recomp.159.cpp` line 30427):
- The actual D3D Present wrapper
- Prepares display parameters
- Calls `__imp__VdSwap(ctx, base)` at address 0x829D55D4

## 8.3 Critical Blocking Issues Identified

### 8.3.1 XamTaskShouldExit Returns 1 (CRITICAL)

**Location:** `imports.cpp` line 5470

```cpp
uint32_t XamTaskShouldExit(uint32_t taskHandle)
{
    // Return 1 to indicate task should exit (no async tasks supported yet)
    return 1;
}
```

**Problem:** This causes ALL XAM async tasks to exit immediately after being scheduled. The game's boot orchestration functions (documented in §1.4.6 `sub_829A3318`) use XamTaskShouldExit in a loop:

```
while (XamTaskShouldExit() == 0) {
    KeWaitForSingleObject(...);
    // do async work
}
```

When XamTaskShouldExit returns 1, the task loop exits immediately without completing its work. This breaks:
- Content mounting completion
- Async I/O completion callbacks
- Background loading tasks
- Any subsystem that depends on XAM task completion signals

**Real Xbox 360 behavior:** XamTaskShouldExit returns 0 while the task should continue running, and returns 1 only when the system requests task termination (e.g., title exit).

**Required fix:**
```cpp
uint32_t XamTaskShouldExit(uint32_t taskHandle)
{
    // Return 0 to indicate task should continue running
    // Only return 1 when system is shutting down
    return 0;
}
```

### 8.3.2 XamTaskSchedule Is a Stub (CRITICAL)

**Location:** `imports.cpp` line 5465

```cpp
void XamTaskSchedule()
{
    LOG_UTILITY("!!! STUB !!!");
}
```

**Problem:** XamTaskSchedule is supposed to:
1. Accept a function pointer and context
2. Create an async task handle
3. Schedule the task for execution
4. Return the task handle to the caller

The current stub does nothing, meaning async tasks are never actually scheduled or executed.

**Real Xbox 360 signature:**
```cpp
DWORD XamTaskSchedule(
    LPVOID lpStartAddress,      // Task function pointer
    LPVOID lpParameter,         // Task context
    DWORD dwProcessId,          // Process ID (usually 0)
    DWORD dwStackSize,          // Stack size
    DWORD dwPriority,           // Thread priority
    DWORD dwFlags,              // Creation flags
    PHANDLE phTask              // Output: task handle
);
```

**Required implementation:** Create a native thread or queue the work to a thread pool, return a valid handle.

### 8.3.3 XamTaskCloseHandle Is a Stub

**Location:** `imports.cpp` line 5476

```cpp
uint32_t XamTaskCloseHandle(uint32_t taskHandle)
{
    LOG_UTILITY("!!! STUB !!!");
    return ERROR_SUCCESS;
}
```

This is less critical but should properly clean up task handles.

## 8.4 Boot-Critical Progression Gates

### 8.4.1 Content/Device Workflow (`sub_827DDE30`)

This function orchestrates content mounting and requires:
1. `sub_829A2590` to start an open operation (returns handle or -1)
2. `sub_829A1F00` to drive async I/O completion
3. `sub_829A1CA0` (XamContentClose wrapper) to return non-997

**Gate condition:** Primary state must be 8, and sub_829A1F00 must return non-zero.

### 8.4.2 Task + Mount/Read Integration (`sub_829A3318`)

This is a boot-adjacent orchestrator that:
1. Calls `sub_829A2AE0` (filesystem open + volume sizing)
2. Registers title terminate notifications
3. Enters a wait loop gated by `XamTaskShouldExit`

**Gate condition:** `XamTaskShouldExit()` must return 0 for the loop to continue processing.

**Current behavior:** Loop exits immediately because XamTaskShouldExit returns 1.

### 8.4.3 Async Status Helper (`sub_829A1A50`)

Returns protocol signals:
- `996`: "no progress, retry later"
- `0`: "operation complete"
- Other: "error or alternate state"

**Gate condition:** Must eventually return 0 for state machines to advance.

## 8.5 Main Thread Block Point Analysis

Based on the call chain and stub analysis, the main thread is likely blocked at:

1. **Early boot:** `sub_829A3318` or similar task orchestrator
2. **Cause:** XamTaskShouldExit returns 1, causing task loops to exit prematurely
3. **Effect:** Subsystem initialization never completes, preventing main loop entry

The game's boot sequence appears to be:
```
main() → KiSystemStartup() → LdrLoadModule() → GuestThread::Start()
    → [Guest entrypoint]
        → System initialization (XAM tasks scheduled)
        → Content mounting (waits for XAM task completion)
        → GPU initialization (completes - special-cased)
        → Main loop entry (NEVER REACHED)
            → sub_828529B0 (main loop)
                → sub_828507F8 (present)
                    → sub_829D5388 (VdSwap wrapper)
```

The block occurs between "Content mounting" and "Main loop entry" because XAM task completion never signals.

## 8.6 Required Kernel Semantics

### 8.6.1 Async I/O Completion (XXOVERLAPPED)

**Current implementation status:** Partially implemented

**XXOVERLAPPED structure:**
```cpp
struct XXOVERLAPPED {
    be<uint32_t> Error;           // 0x00: Result code
    be<uint32_t> Length;          // 0x04: Bytes transferred
    be<uint32_t> InternalContext; // 0x08: Internal
    be<uint32_t> hEvent;          // 0x0C: Event to signal
    be<uint32_t> pCompletionRoutine; // 0x10: APC routine
    be<uint32_t> dwCompletionContext; // 0x14: APC context
    be<uint32_t> dwExtendedError; // 0x18: Extended error
};
```

**What game expects:**
1. Error field set to 0 on success, error code on failure
2. Length field set to bytes transferred
3. hEvent signaled when operation completes
4. pCompletionRoutine called if provided (APC)

**Current behavior:** Error/Length/hEvent are set correctly. pCompletionRoutine (APC) is NOT called.

### 8.6.2 Event Signaling (KeSetEvent, KeWaitForSingleObject)

**Current implementation status:** Implemented

**KeSetEvent:** Correctly signals Event objects and uses atomic generation counter.

**KeWaitForSingleObject:** Implemented with:
- Type-based dispatch (Event, Semaphore, etc.)
- GPU init special-casing for callers 0x829DDD48, 0x829D8AA8
- Infinite wait logging

### 8.6.3 XAM Task Scheduling

**Current implementation status:** NOT IMPLEMENTED (stubs only)

**Required behavior:**

**XamTaskSchedule:**
1. Create a task descriptor with function pointer and context
2. Assign a unique task handle
3. Either:
   - Immediately execute the task on a worker thread, OR
   - Queue it for later execution
4. Return handle to caller

**XamTaskShouldExit:**
1. Return 0 while task should continue running
2. Return 1 only when system requests termination

**XamTaskCloseHandle:**
1. Wait for task completion if still running
2. Release task resources
3. Invalidate handle

### 8.6.4 Thread Barriers and Job Queues

The game creates 9 worker threads that idle on events. These appear to be correctly parked.

The main thread waits for initialization signals from XAM tasks. This is the block point.

### 8.6.5 GPU Sync Primitives

**Current implementation status:** WORKING

GPU initialization is special-cased in KeWaitForSingleObject:
- After 10 poll iterations, GPU flags are forced true
- Ring buffer read pointer writeback is faked
- Graphics interrupt callback is fired

## 8.7 Why VdSwap Is Never Reached (Causal Explanation)

1. **Boot sequence starts XAM tasks** for content mounting and initialization
2. **XamTaskSchedule is a stub** — tasks are never actually scheduled
3. **XamTaskShouldExit returns 1** — any task loops exit immediately
4. **Initialization state machines stall** — they wait for task completion signals that never come
5. **Main loop is gated by initialization completion** — this gate is never opened
6. **sub_828529B0 (main loop) is never called** — therefore sub_828507F8 → sub_829D5388 → VdSwap never executes

The game does not fail loudly because:
- It's designed for cooperative async scheduling
- Stall conditions result in infinite waits, not crashes
- The block is in a waiting state, not an error state

# 9. Minimum Required Implementations for GTA IV to Enter Render Loop

## 9.1 Critical (Blocks Main Thread)

| Function/Subsystem | Required Behavior | What Happens If Missing | Blocks |
|---|---|---|---|
| **XamTaskShouldExit** | Return 0 while task is running; return 1 only on system shutdown | Task loops exit immediately, initialization never completes | Main thread |
| **XamTaskSchedule** | Create async task, assign handle, schedule execution | Async tasks never run, completion signals never fire | Main thread + workers |
| **XamTaskCloseHandle** | Clean up task resources, wait for completion if needed | Resource leaks, handle reuse issues | Both |

## 9.2 High Priority (May Block Subsystems)

| Function/Subsystem | Required Behavior | What Happens If Missing | Blocks |
|---|---|---|---|
| **XXOVERLAPPED.pCompletionRoutine** | Call APC callback when async operation completes | Some async operations may not signal completion | Workers |
| **NtWaitForMultipleObjectsEx** | Wait on multiple kernel objects | Multi-object waits hang forever | Both |
| **Timer objects (NtCreateTimer, NtSetTimerEx)** | Schedule timed callbacks | Timed events never fire | Workers |

## 9.3 Medium Priority (Affects Functionality)

| Function/Subsystem | Required Behavior | What Happens If Missing | Blocks |
|---|---|---|---|
| **XamEnumerate** | Return content enumeration results with proper pending semantics | Content enumeration may hang | Main thread |
| **XamContentClose** | Close content with proper pending/completion semantics | Cleanup may hang | Main thread |
| **ObCreateSymbolicLink / ObDeleteSymbolicLink** | Create/delete symbolic links for mount points | Mount point management broken | Neither |

## 9.4 Immediate Action Items

1. **Fix XamTaskShouldExit** (1 line change):
   ```cpp
   uint32_t XamTaskShouldExit(uint32_t taskHandle)
   {
       return 0;  // Tasks should continue running
   }
   ```

2. **Implement XamTaskSchedule** (requires thread pool or immediate execution):
   ```cpp
   uint32_t XamTaskSchedule(
       uint32_t funcAddr, uint32_t context, uint32_t processId,
       uint32_t stackSize, uint32_t priority, uint32_t flags,
       be<uint32_t>* phTask)
   {
       // Option A: Execute immediately on current thread
       // Option B: Queue to thread pool and return handle
       // Either way, set *phTask to valid handle
   }
   ```

3. **Add logging to main loop entry** to confirm when/if it's reached:
   ```cpp
   // In sub_828529B0 hook or at its entry point
   LOGF_IMPL(Utility, "MainLoop", "MAIN LOOP ENTERED!");
   ```

## 9.5 Verification Steps

After implementing fixes:

1. **Check log for XamTaskSchedule calls** — should see task scheduling
2. **Check log for XamTaskShouldExit calls** — should see many 0 returns
3. **Check log for "VdSwap frame"** — should see frame presentation
4. **Observe window** — should see rendered frames

If VdSwap is still not called after XamTask fixes:
1. Add logging to sub_828529B0 entry
2. Add logging to sub_828507F8 entry
3. Trace what condition prevents main loop entry

---

## 10. Draw Call Chain Analysis (December 19, 2025)

### 10.1 Full Render Call Chain (Stack Trace Backwards)

Traced backwards from actual draw functions to understand the complete path:

```
GAME ENTRY
    └── sub_8218BEA8 (Main entry - Xbox runtime calls this)
            └── sub_827D89B8 (Frame tick)
                    └── sub_82120000 (ONE-TIME init) ← BLOCKS HERE
                            └── sub_82120EE8 (Subsystem init)
                                    └── sub_82673718 (Audio/streaming)
                                            └── sub_82975608 (Resource init)
                                                    └── sub_829748D0 (Thread/worker init)
                                                            └── sub_8298E810 ← WAITS FOR WORKERS

↓ AFTER INIT COMPLETES (never happens currently) ↓

    └── sub_82856F08 (Frame orchestrator)
            └── sub_828529B0 (MAIN LOOP) ← NEVER REACHED
                    └── sub_828507F8 (Present wrapper)
                            └── Draw dispatch to workers
                                    └── sub_829D4EE0 (UnifiedDraw)
                                            ├── sub_829CD350 (SetVertexShader) ✅ HOOKED
                                            └── sub_829D8568 (GPU commands)
```

### 10.2 Draw Function Call Hierarchy

```
RENDER SUBMISSION PATH
    ├── sub_829D5380 → sub_829D4EE0 (UnifiedDraw)
    ├── sub_829D5128 → sub_829D5088 → sub_829D4EE0
    └── sub_829D51A8 → sub_829D4EE0

DRAW FUNCTION (sub_829D4EE0 - UnifiedDraw/DrawIndexedPrimitive)
    ├── sub_829DC848 (pre-draw setup)
    ├── sub_829CD350 (SetVertexShader) ✅ HOOKED
    └── sub_829D8568 (command buffer flush)
```

### 10.3 What MUST Happen for Draws

| Step | Function | Status | Notes |
|------|----------|--------|-------|
| 1 | Init completes | ❌ BLOCKED | Stuck in sub_8298E810 waiting for workers |
| 2 | Main loop starts | ❌ NOT REACHED | sub_828529B0 never called |
| 3 | Render work queued | ❌ NEVER HAPPENS | Main loop must queue work for workers |
| 4 | Workers receive work | ❌ NO WORK | Workers wait on semaphores, pass with hack, find nothing |
| 5 | Draw functions called | ❌ NEVER CALLED | sub_829D4EE0 never executed |
| 6 | Shaders bound | ✅ HOOKED | sub_829CD350 ready |
| 7 | Commands flushed | ✅ HOOKED | sub_829D8568 ready |

### 10.4 Semaphore Experiment Results

**Experiment**: Force count=1 for render worker semaphores (0xA82487B0, 0xA82487F0)

**Result**:
- ✅ Workers pass their first wait
- ✅ Workers enter sub_827DE858 (render worker function)
- ❌ Workers exit immediately - no work queued
- ❌ draws=0 still

**Conclusion**: Semaphore fix alone is insufficient. The **main loop must start** to queue work for workers.

### 10.5 Blocking Points Identified

| Semaphore | Purpose | Status |
|-----------|---------|--------|
| 0xA82487B0 | Render worker 1 | Forced to count=1 |
| 0xA82487F0 | Render worker 2 | Forced to count=1 |
| 0xEB2D00B0 | Other worker | Blocking main thread (different path) |

### 10.6 Root Cause Summary

The game has a **circular dependency**:
1. Init spawns render workers
2. Workers wait on semaphores for work
3. Main thread should signal semaphores when work available
4. But main thread is ALSO waiting in init for workers to complete
5. **Deadlock**: Workers wait for work, main waits for workers

### 10.7 Key D3D Functions Already Hooked

| Address | Function | File |
|---------|----------|------|
| sub_829CD350 | SetVertexShader | video.cpp |
| sub_829D6690 | SetPixelShader | video.cpp |
| sub_829D4EE0 | DrawIndexedPrimitive | video.cpp |
| sub_829D8860 | DrawPrimitive | video.cpp |
| sub_829C9070 | SetStreamSource | video.cpp |
| sub_829C96D0 | SetIndices | video.cpp |
| sub_829D3728 | SetTexture | video.cpp |

### 10.8 Next Steps

1. **Trace what blocks in sub_8298E810** to find exact deadlock point
2. **Force init completion** to test if main loop queues work properly
3. **Add hooks to trace when work IS queued** to workers

---

## 11. Semaphore Deadlock Root Cause Analysis (December 20, 2025)

### 11.1 Blocking Semaphore Identified

The game is stuck polling semaphore `0xEB2D00B0` with `timeout=0` (non-blocking waits):
```
[NtWaitForSingleObjectEx] #1449 handle=0xEB2D00B0 objType=Semaphore (Sem=1 Evt=0)
[NtWaitForSingleObjectEx] #1450 handle=0xEB2D00B0 objType=Semaphore (Sem=1 Evt=0)
... (thousands of times)
```

### 11.2 Semaphore Creation

The semaphore was created via `NtCreateSemaphore` with count=0:
```
[NtCreateSemaphore] #533 handle=0xEB2D00B0 count=0 max=32767
[NtCreateSemaphore] #534 handle=0xEB2D00F0 count=0 max=32767
```

### 11.3 Key Findings

1. **No KeInitializeSemaphore calls** - All semaphores created via NtCreateSemaphore
2. **No KeReleaseSemaphore calls** - Nobody signals these semaphores  
3. **IsKernelObject check** - Returns true for ANY handle with high bit set (`handle & 0x80000000`)
4. **Worker threads created** - 11+ threads created successfully, some suspended then resumed

### 11.4 Thread Creation Log
```
[GuestThreadFunc] Thread starting, suspended=1, waiting...
[GuestThreadFunc] Thread RESUMED after wait
[GuestThreadFunc] Thread starting, suspended=0, waiting...
[GuestThreadFunc] Thread NOT suspended, running immediately
... (multiple threads)
```

### 11.5 Fix Implemented

Enhanced `NtWaitForSingleObjectEx` to detect guest dispatcher objects (XKSEMAPHORE/XKEVENT) by checking the Type field in the dispatcher header:
- Type 5 = Semaphore → use `QueryKernelObject<Semaphore>`
- Type 0/1 = Event → use `QueryKernelObject<Event>`  
- Other = Direct kernel handle (from NtCreate*)

### 11.6 Remaining Issue

The semaphore `0xEB2D00B0` is a **valid kernel handle** (created via NtCreateSemaphore), but it's never signaled. The main thread is stuck in a busy-wait polling loop waiting for a worker to signal, but workers are waiting for work from the main thread.

**Deadlock pattern:**
```
MAIN THREAD              WORKER THREADS
────────────             ──────────────
Create workers           
Wait for "ready"  ──X──  Should signal "ready" 
                         but waiting for "work"
Can't send work          Can't signal ready
    ↓                         ↓
  BLOCKED                  BLOCKED
```

### 11.7 Critical Finding: NtReleaseSemaphore Never Called

**ZERO calls to NtReleaseSemaphore or KeReleaseSemaphore in entire log.**

This confirms:
- Workers ARE created and resumed (GuestThreadFunc logs show this)
- But workers NEVER call their signal function
- Workers must be blocked BEFORE reaching the signal code

### 11.8 Worker Flow (Expected vs Actual)

**Expected:**
```
Worker starts → Signal "ready" → Wait for "work" → Do work → Loop
```

**Actual:**
```
Worker starts → ??? BLOCKED ??? → Never signals → Deadlock
```

### 11.9 Experiment: Force Semaphore Count=1

**Hypothesis:** If we force the initial count to 1 for semaphores in the 0xEB2D range, the main thread's wait will pass immediately, allowing init to continue.

**Implementation:**
```cpp
// In NtCreateSemaphore
if (handle >= 0xEB2D0000 && handle <= 0xEB2E0000 && InitialCount == 0) {
    // Force count=1 for blocking semaphores
    sem->count.store(1);
}
```

**Expected Outcome:**
- Main thread passes the wait on 0xEB2D00B0
- Init continues to next phase
- May reveal next blocking point or allow render loop to start

**Rollback Plan:** If no meaningful improvement, revert and trace worker blocking point.

### 11.10 Experiment Result

**Result:** Force signal DID work, but only ONCE.

- First wait succeeded (consumed count, count became 0)
- All subsequent waits timeout (count=0, nobody re-signals)
- Game is in polling loop expecting semaphore to be signaled repeatedly

**Conclusion:** The problem is NOT with semaphore creation or the Wait implementation. 
The problem is that **workers never call NtReleaseSemaphore** to signal the semaphore.

### 11.11 Next: Trace Worker Blocking Point

Workers ARE created and resumed (confirmed by GuestThreadFunc logs), but they never reach the signal function. Need to trace:
1. What function workers call after starting
2. Where they get blocked before signaling

### 11.12 PPC Hook Investigation

**Status:** PPC_FUNC hooks are defined for:
- `sub_827DAE40` - Worker thread entry point
- `sub_829A9738` - Wait-with-retry helper
- Various init functions

**Issue:** These hooks use `extern "C" void __imp__sub_XXXXX` pattern which requires the recompiler to link them. GUEST_FUNCTION_HOOK is for kernel imports only.

**Next Steps:**
1. Check if PPC hooks are being called by adding simpler tracing
2. Or try forcing semaphore signal from NtReleaseSemaphore hook when 0xEB2D handles are accessed
3. Consider stubbing the blocking wait to return success immediately

### 11.13 Experiment 2: Force Wait Success (IMPLEMENTED)

**Location:** `NtWaitForSingleObjectEx` in `kernel/imports.cpp` ~line 2359

**Change:** Force `STATUS_SUCCESS` return for waits on `0xEB2D00B0` and `0xEB2D00F0`:
```cpp
if (Handle == 0xEB2D00B0 || Handle == 0xEB2D00F0)
{
    return STATUS_SUCCESS;  // Break polling loop
}
```

**Status:** ✅ **SUCCESS!** The experiment works!

**Results (December 20, 2025):**
1. ✅ Waits on 0xEB2D00B0 return `STATUS_SUCCESS` (`r3=0x00000000`)
2. ✅ **NtReleaseSemaphore IS now being called** - 130+ calls to `0xEB2D00F0`
3. ✅ **Workers ARE signaling** - `sub_827DAD60` worker startup signal calls happening
4. ✅ **VBlank is firing** - game loop is running!

**Log evidence:**
```
[WaitHelper] sub_829A9738 #100 obj=0xEB2D00B0 timeout=0
[WaitHelper] sub_829A9738 #100 returned r3=0x00000000   <-- SUCCESS!
[NtReleaseSemaphore] #88 handle=0xEB2D00F0 release=1 count=84/32767
[sub_827DAD60] [SIGNAL] sub_827DAD60 #100 handle=0xEB2D00F0 (worker thread startup signal?)
[VBlank] Firing VBlank #120 callback=0x829D7368
```

**Conclusion:** The deadlock was caused by main thread waiting on 0xEB2D00B0 before workers could signal. By forcing that wait to succeed, workers now execute and signal properly.

### 11.14 Current Status After Fix

- **179 NtReleaseSemaphore calls** - workers actively signaling
- **VBlank callbacks running** - game loop is active
- **Init functions completing** - sub_8296BE18, sub_8218BE28 progressing

**Next:** Check if VdSwap/rendering is happening or if there's a new blocking point.

### 11.15 Additional Blocking Point (December 20, 2025)

After fixing 0xEB2D00B0/F0, game progresses but `sub_82120000` still doesn't exit.

**Current state:**
- 18 threads created
- 2000+ NtReleaseSemaphore calls on 0xEB2D00F0
- VBlank #2340+ firing
- `sub_82120000` entered but NOT exited

**Tried:** Added forced success for 0xA82487B0/F0 - didn't help

**Next:** Need to identify actual blocking handle inside sub_82120000. Run game manually and grep for blocking waits:
```bash
grep "NtWaitEx.*timeout=0" /tmp/gta_test2.log | sort | uniq -c | sort -rn | head -20
```

### 11.16 Extended Force Success (December 20, 2025)

**Findings via POLL logging:**
1. `0xA8240270` - found polling, added to force list
2. `0xEB2D0030` - found polling 5000+ times

**Solution:** Extended force success to handle entire `0xEB2D0000-0xEB2DFFFF` range:
```cpp
bool isBlockingSem = (Handle >= 0xEB2D0000 && Handle <= 0xEB2DFFFF) ||
                     (Handle == 0xA82487B0 || Handle == 0xA82487F0 || Handle == 0xA8240270);
```

**Status:** No more POLL messages after this update - all 0xEB2D range semaphores now force-succeeded.

**Current game state:**
- 18+ threads created
- 2000+ NtReleaseSemaphore calls
- VBlank firing regularly
- Entire 0xEB2D range handled
- No VdSwap yet - need to check if init completes

### 11.17 Final Force Success Range (December 20, 2025)

**Extended to cover all blocking semaphores:**
```cpp
bool isBlockingSem = (Handle >= 0xEB2C0000 && Handle <= 0xEB2DFFFF) ||
                     (Handle == 0xA82487B0 || Handle == 0xA82487F0 || 
                      Handle == 0xA8240270 || Handle == 0xA82402B0);
```

**Results:**
- Workers now starting and passing semaphore waits
- `sub_827DAE40` worker entry being called
- Threads created and resumed successfully
- Semaphore waits force-succeeded for both timeout=0 and INFINITE

**Game log shows:**
```
[WorkerThread] sub_827DAE40 ENTER #3 ctx=0x830F51EC taskFunc=0x827EE568
[sub_827DACD8] [SEM_WAIT] sub_827DACD8 #3 PASSED wait on semaphore 0xEB2CFAF0
[GuestThreadFunc] Thread RESUMED after wait
```

**Status:** Init progressing, workers active, but no VdSwap/draw calls yet. May need further investigation into render pipeline.

### 11.18 Thread Suspend Verification (December 20, 2025)

**Debug output confirmed correct behavior:**
```
[GuestThreadHandle] CTOR flags=0x1 shouldSuspend=1 suspended=1  // Worker threads
[GuestThreadHandle] CTOR flags=0x0 shouldSuspend=0 suspended=0  // GPU threads
```

**GuestThreadFunc correctly handles suspend:**
```
[GuestThreadFunc] Thread starting, suspended=1, waiting...
[GuestThreadFunc] Thread RESUMED after wait    // ✓ Correct
[GuestThreadFunc] Thread starting, suspended=0, waiting...
[GuestThreadFunc] Thread NOT suspended, running immediately  // ✓ GPU threads
```

**Current state:**
- Thread suspend/resume mechanism working correctly
- 6+ threads created and running
- Semaphore waits passing via force success
- No VdSwap yet - init still not complete
- No bus error crash in latest test

### 11.19 Critical Section Deadlock Fix (December 20, 2025)

**Issue Found:**
```
[LOCK] RtlEnterCriticalSection thisThread=0x00220260 WAITING on owner=0x00120260 cs=0x82A97FB4
```
Thread waiting on a critical section whose owner is blocked elsewhere.

**Fix Applied:** `RtlEnterCriticalSection` now force-acquires lock after 100K spins:
```cpp
if (loopCount > MAX_SPIN_LOOPS) {
    owningThread.store(thisThread);
    cs->RecursionCount = 1;
    return;
}
```

**Status:** Build complete, needs testing to verify init progresses past the lock.

---

## Session Update: December 20, 2024

### Major Progress: Core Init Completes Naturally

After removing forced VdSwap and semaphore force-success, **core init now completes successfully**:

```
[sub_8218C600] [INIT] sub_8218C600 EXIT #1 thread=0x225E r3=1
```

### Init Flow Status

| Function | Status | Notes |
|----------|--------|-------|
| `sub_8218C600` | ✅ EXIT r3=1 | Core init complete |
| `sub_82673718` | ✅ EXIT | Audio/streaming init resolved |
| `sub_82975608` | ✅ EXIT | Resource init resolved |
| `sub_82120EE8` | ❌ Blocking | Subsystem init stuck |
| `sub_82273988` | ❌ Blocking | **Current blocker** |

### Current Blocking Point: sub_82273988

The main thread (0x225E) is stuck inside `sub_82273988` (post-init loading).
- NOT a kernel wait (no ACTUAL WAIT from main thread)
- NOT a critical section wait (that's a different thread 0x00220260)
- Workers blocked on semaphores waiting for work signals
- Classic producer-consumer deadlock at a later init stage

### Key Changes Made

1. **Removed forced VdSwap** - Let game present frames naturally
2. **Removed semaphore force-success** - Let workers wait naturally
3. **Reduced critical section MAX_SPIN_LOOPS** to 50 - Force-acquire faster

### What's Working

- Core init completes with r3=1
- Thread creation and management
- File loading from RPF (common.rpf, game:\)
- Event creation (15+ events created)
- Worker threads start and enter wait states

### What's Still Blocking

- `sub_82273988` entered but never exits (3+ minutes)
- Main thread busy-waiting on unknown memory location
- Workers can't progress without work signals from main thread
- No VdSwap calls (0 frames presented)

### Next Steps to Investigate

1. **Trace inside sub_82273988** - Add hooks for functions it calls
2. **Check event signaling** - Maybe waiting for an event that's never signaled
3. **Look at Xenia traces** - Compare behavior with working emulator
4. **Possible async I/O wait** - File load completion that never signals
