# Building Liberty Recompiled

Liberty Recompiled is an unofficial PC port of Grand Theft Auto IV for Xbox 360, created through static recompilation. This guide covers building the project from source.

## Supported Platforms

| Platform | Architecture | Status | CMake Preset |
|-|-|-|-|
| Windows | x64 | Supported | `x64-Clang-Release` |
| Windows | ARM64 | Supported | `arm64-Clang-Release` |
| Linux | x64 | Supported | `linux-release` |
| Linux | ARM64 | Supported | `linux-arm64-release` |
| macOS | ARM64 (Apple Silicon) | Supported | `macos-release` |
| macOS | x64 (Intel) | Supported | `macos-release` |
| iOS | ARM64 | Experimental | `ios-release` |
| Android | ARM64 | Experimental | `android-release` |
| PS4 | x64 | Experimental | `ps4-release` |
| Switch | ARM64 | Experimental | `switch-release` |

## 1. Clone the Repository

Clone **LibertyRecomp** with submodules using [Git](https://git-scm.com/).
```
git clone --recurse-submodules https://github.com/OZORDI/LibertyRecomp.git
```

### Windows
If you skipped the `--recurse-submodules` argument during cloning, you can run `update_submodules.bat` to ensure the submodules are pulled.

## 2. Add the Required Game Files

Copy the following files from your GTA IV Xbox 360 game and place them inside `./LibertyRecompLib/private/`:
- `default.xex` - Main executable (from game root)
- `xbox360.rpf` - Main game archive (from game root)

> [!TIP]
> It is recommended that you install the game using [an existing Liberty Recompiled release](https://github.com/OZORDI/LibertyRecomp/releases/latest) to acquire these files, otherwise you'll need to rely on third-party tools to extract them from your Xbox 360 disc or ISO.
>
> When sourcing these files from a Liberty Recompiled installation, they will be stored under the `game` subdirectory.

### Shader Files (Optional)

For shader development, you can also copy the shader files from `common/shaders/` to enable the shader pipeline:
- All `.fxc` files from the game's shader directories

These will be automatically processed during installation to generate platform-native shader caches.

## 3. Install Dependencies

### Windows (x64 and ARM64)
You will need to install [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/).

In the installer, you must select the following **Workloads** and **Individual components** for installation:
- Desktop development with C++
- C++ Clang Compiler for Windows
- C++ CMake tools for Windows

For **ARM64 builds**, also install:
- C++ ARM64/ARM64EC build tools (Latest)
- MSVC v143 - VS 2022 C++ ARM64/ARM64EC build tools

### Linux
The following command will install the required dependencies on a distro that uses `apt` (such as Debian-based distros).
```bash
sudo apt install autoconf automake libtool pkg-config curl cmake ninja-build clang clang-tools libgtk-3-dev lld libsdl2-dev libx11-xcb-dev
```
The following command will install the required dependencies on a distro that uses `pacman` (such as Arch-based distros).
```bash
sudo pacman -S base-devel ninja lld clang gtk3
```
You can also find the equivalent packages for your preferred distro.

> [!NOTE]
> This list may not be comprehensive for your particular distro and you may be required to install additional packages, should an error occur during configuration.

### macOS
You will need to install the latest Xcode from Apple.

The following commands will install additional required dependencies, depending on which package manager you use.

If you use Homebrew:
```bash
brew install cmake ninja pkg-config
```

If you use MacPorts:
```bash
sudo port install cmake ninja pkg-config
```

## 4. Build the Project

### Windows (x64)
1. Open the repository directory in Visual Studio and wait for CMake generation to complete. If you don't plan to debug, switch to the `Release` configuration.

> [!TIP]
> If you need a Release-performant build and want to iterate on development without debugging, **it is highly recommended** that you use the `RelWithDebInfo` configuration for faster compile times.

2. Under **Solution Explorer**, right-click and choose **Switch to CMake Targets View**.
3. Right-click the **LibertyRecomp** project and choose **Set as Startup Item**, then choose **Add Debug Configuration**.
4. Add a `currentDir` property to the first element under `configurations` in the generated JSON and set its value to the path to your game directory (where root is the directory containing `dlc`, `game`, etc).
5. Start **LibertyRecomp**. The initial compilation may take a while to complete due to code and shader recompilation.

#### Command Line Build (x64)
```powershell
# Open Developer Command Prompt for VS 2022, then:
cmake . --preset x64-Clang-Release
cmake --build .\out\build\x64-Clang-Release --target LibertyRecomp
```

### Windows (ARM64)
For ARM64 builds, use the command line:

```powershell
# Open ARM64 Developer Command Prompt for VS 2022, then:
cmake . --preset arm64-Clang-Release
cmake --build .\out\build\arm64-Clang-Release --target LibertyRecomp
```

> [!NOTE]
> The available Windows presets are:
> - **x64**: `x64-Clang-Debug`, `x64-Clang-RelWithDebInfo`, `x64-Clang-Release`
> - **ARM64**: `arm64-Clang-Debug`, `arm64-Clang-RelWithDebInfo`, `arm64-Clang-Release`

### Linux (x64 and ARM64)
The build process is the same for both x64 and ARM64 - the architecture is auto-detected.

1. Configure the project using CMake by navigating to the repository and running the following command.
```bash
cmake . --preset linux-release
```

> [!NOTE]
> The available presets are `linux-debug`, `linux-relwithdebinfo` and `linux-release`. Presets for ARM64 are `linux-arm64-debug`, `linux-arm64-relwithdebinfo`, `linux-arm64-release`.

2. Build the recompiled game library, then the main application:
```bash
cmake --build ./out/build/linux-release --target LibertyRecompLib
cmake --build ./out/build/linux-release --target LibertyRecomp
```

3. Navigate to the directory that was specified as the output in the previous step and run the game.
```bash
./LibertyRecomp
```

### macOS (ARM64 and x64)
The build process works for both Apple Silicon (ARM64) and Intel (x64) Macs.

1. Set the VCPKG_ROOT environment variable (required for dependency management):
```bash
export VCPKG_ROOT=$(pwd)/thirdparty/vcpkg
```

2. Configure the project using CMake by navigating to the repository and running the following command.
```bash
# For Apple Silicon (ARM64) - default on M1/M2/M3 Macs
cmake . --preset macos-release

# For Intel (x64) Macs  
cmake . --preset macos-release -DCMAKE_OSX_ARCHITECTURES=x86_64
```

> [!NOTE]
> The available presets are `macos-debug`, `macos-relwithdebinfo` and `macos-release`.

3. Build the recompiled game library, then the main application:
```bash
cmake --build ./out/build/macos-release --target LibertyRecompLib
cmake --build ./out/build/macos-release --target LibertyRecomp
```

4. Navigate to the directory that was specified as the output in the previous step and run the game.
```bash
open "./out/build/macos-release/LibertyRecomp/Liberty Recompiled.app"
```

## 5. Shader Pipeline (Development)

Liberty Recompiled includes an automated shader pipeline that converts Xbox 360 RAGE engine shaders to platform-native formats during installation.

### How It Works

1. **During Installation**: The installer automatically extracts and converts shaders from `.fxc` files
2. **Platform Detection**: Automatically selects the correct format:
   - **Windows**: DXIL (Direct3D 12)
   - **Linux**: SPIR-V (Vulkan)
   - **macOS**: AIR (Metal)
3. **Caching**: Converted shaders are cached to avoid re-conversion on subsequent runs

### Building the Shader Tools

#### RAGE FXC Extractor (Standalone Tool)
```bash
cd tools/rage_fxc_extractor
mkdir build && cd build
cmake ..
make
```

#### XenosRecomp (Shader Compiler)
```bash
cd build_xenosrecomp
cmake ../tools/XenosRecomp
make
```

### Manual Shader Conversion

For development purposes, you can manually convert shaders:

```bash
# Extract shaders from RAGE FXC files
./tools/rage_fxc_extractor/build/rage_fxc_extractor --batch shader_batch/ LibertyRecompLib/shader/rage_shaders/

# Compile to shader cache
./build_xenosrecomp/XenosRecomp/XenosRecomp LibertyRecompLib/shader/rage_shaders/ LibertyRecompLib/shader/shader_cache.cpp tools/XenosRecomp/XenosRecomp/shader_common.h
```

For more details, see [SHADER_PIPELINE.md](SHADER_PIPELINE.md).

## 6. Project Structure

```
LibertyRecomp/
├── LibertyRecomp/          # Main application code
│   ├── api/                # Public API headers (RAGE, Havok, stdx)
│   ├── apu/                # Audio processing (XMA decoder, embedded player)
│   ├── cpu/                # CPU emulation / guest thread / PPC context
│   ├── gpu/                # Graphics / video rendering / shaders / upscaling
│   ├── hid/                # Human input devices (SDL, DualSense)
│   ├── install/            # Installer, shader converter, RPF/ISO extraction
│   ├── kernel/             # Kernel imports, memory, VFS, XAM, file I/O
│   ├── locale/             # Localization / language support
│   ├── mod/                # Mod loader and INI file parsing
│   ├── os/                 # Platform-specific code (win32, linux, macos, ios, android, ps4, switch)
│   ├── patches/            # GTA IV specific patches (input, camera, FPS, audio)
│   ├── runtime/            # Rex runtime adapters (sync, threads, xobject)
│   ├── ui/                 # ImGui-based UI (menus, overlays, installer wizard)
│   ├── user/               # User config, saves, achievements, registry
│   └── utils/              # Utility classes (bit_stream, ring_buffer)
├── LibertyRecompLib/       # Recompiled game code
│   ├── config/             # Recompiler configuration (TOML, switch tables)
│   ├── shader/             # Shader cache (generated)
│   └── private/            # Game files (not in repo)
├── tools/                  # Development tools
│   # Xbox 360 PPC recompilation is provided by rexglue (graine SDK) under glue/
│   ├── XenosRecomp/        # Xbox 360 shader recompiler
│   ├── rage_fxc_extractor/ # RAGE FXC shader extractor
│   └── bc_diff/            # Binary comparison tool
└── docs/                   # Documentation
```

## 7. Decompiled Subsystems

The `patches/` directory contains both high-level game hooks and fully decompiled RAGE engine subsystems reimplemented in clean C++. Decompiled patches were produced from IDA Hex-Rays pseudocode cross-referenced against the PPC recomp scaffolds generated by rexglue (graine SDK).

### Engine Core

| File | Subsystem | Description |
|-|-|-|
| `grm_setup_patches.cpp` | `rage::grmSetup` | Graphics setup singleton: device init/teardown, frame timing EMA, v-sync state machine, vtable dispatch thunks |
| `scene_tick_patches.cpp` | Scene tick | Entity update chain: streaming subsystem update, 24-slot entity tick loop, world update, entity finalize, sector dirty marking |
| `frontend_state_hooks.cpp` | Front-end FSM | Episode selection / title screen state machine with PC keyboard input injection for controller-less operation |
| `postfx_hooks.cpp` | PostFX | Native post-processing intercept: bloom/motion-blur/DOF disable when custom effects active, sun direction extraction |

### Audio

| File | Subsystem | Description |
|-|-|-|
| `audio_pool.cpp` | `audVoicePool` | Fixed-size voice pool with per-frame iteration, vtable dispatch, and voice control block (VCB) state tracking |
| `aud_sound_manager.cpp` | `audSoundManager` | Priority-based voice budget: distance sorting, radix sort, voice allocation, playback cursor advancement, request queue servicing |
| `audio_patches.cpp` | Audio hooks | XMA decoder integration, audio thread management |

### Game Systems

| File | Subsystem | Description |
|-|-|-|
| `fps_patches.cpp` | Frame rate | Unlocked frame rate, delta time fixes |
| `camera_patches.cpp` | Camera | Camera system patches for widescreen and input |
| `aspect_ratio_patches.cpp` | Aspect ratio | Widescreen / ultrawide support |
| `gta4_input_patches.cpp` | Input | Keyboard/mouse input remapping from Xbox controller layout |
| `gta4_ds4_patches.cpp` | DualSense | PS5 DualSense controller support with haptics |
| `gta4_motion_patches.cpp` | Motion | Gyro/accelerometer input for supported controllers |
| `memcpy_patches.cpp` | Memory | Optimized memory copy paths for host architecture |
| `loading_patches.cpp` | Loading | Loading screen event system |
| `misc_patches.cpp` | Miscellaneous | Various small fixes and workarounds |

### UI / Frontend

| File | Subsystem | Description |
|-|-|-|
| `MainMenuTask_patches.cpp` | Main menu | Options menu integration, menu state hooks |
| `SaveDataTask_patches.cpp` | Save data | Storage device alert redirection for PC |
| `TitleTask_patches.cpp` | Title screen | Title screen outro timing, language selection |
| `text_patches.cpp` | Text | Text rendering and localization patches |
| `video_patches.cpp` | Video | Video playback hooks |
| `frontend_listener.cpp` | Frontend events | Frontend event listener bridge |

## 8. Development Tools

### liberty-decomp MCP Server

The project includes an MCP (Model Context Protocol) server that provides AI-assisted decompilation tooling. It exposes the full GTA IV binary analysis database (31,782 functions, 3,332 RTTI classes, 51,494 symbols) through structured tool calls.

**Available tools:**

| Tool | Purpose |
|-|-|
| `get_function_info` | Address, size, class, vtable slot, hook status for any function |
| `get_function_pseudocode` | IDA Hex-Rays decompiled C for ~32K functions |
| `get_function_recomp` | PPC recomp scaffold with annotated addresses |
| `get_class_context` | RTTI inheritance, vtable layout, field clusters, debug strings |
| `search_symbols` | Substring search across functions, symbols, and RTTI classes |
| `get_string_refs` | Find strings referenced by a function, or find functions that reference a string |
| `resolve_address` | Identify any address: symbol, string content, vtable class, nearest symbol |
| `find_callees` / `find_callers` | Call graph traversal (96K forward edges, 139K reverse edges) |
| `suggest_unimplemented_func` | Pick a random un-hooked function with full context card |
| `suggest_file_placement` | Determine which patch file a new hook belongs in |
| `write_source_file` | Surgical editor for patch files |

**Workflow for decompiling a new function:**

1. Use `suggest_unimplemented_func` or `search_symbols` to find a target
2. Call `get_function_info` to check metadata and hook status
3. Read `get_function_pseudocode` and `get_function_recomp` side by side
4. Use `resolve_address` on every unknown `lbl_` address in the scaffold
5. Use `get_class_context` for any classes the function touches
6. Call `suggest_file_placement` to determine the correct patch file
7. Write the clean C++ reimplementation with `write_source_file`

### Codegen (Recompiler)

The codegen step converts the Xbox 360 PPC binary into x86/ARM C++ source. It requires Homebrew LLVM (not Apple Clang) on macOS:

```bash
cmake -B build-codegen -S glue/rexglue-sdk-main -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DREXGLUE_USE_VULKAN=ON \
  -DCMAKE_C_COMPILER=/opt/homebrew/opt/llvm/bin/clang \
  -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++ \
  -DCMAKE_CXX_FLAGS="-I$(pwd)/thirdparty/o1heap -nostdlib++" \
  -DCMAKE_EXE_LINKER_FLAGS="-L/opt/homebrew/opt/llvm/lib/c++ -Wl,-rpath,/opt/homebrew/opt/llvm/lib/c++ -lc++"
```

**Codegen statistics:** 38,546 functions discovered, 38,268 recompiled, 278 excluded (246 imports + 32 rexcrt native replacements).

## 9. Known Issues

### Runtime State

- The game boots through the title screen and reaches the main menu
- Audio subsystem initializes and loads all RPF archives
- Save state machine runs (content enumeration, save manager)
- Particle emitter registration storm (~4,608 allocators via `sub_825BF8A8`) uses fallback allocator when heap TLS is not yet configured; this is one-time init and not a deadlock
- The main world init function (`sub_821200D0`) enters but takes extended time to complete during first boot

### Build Notes

- macOS builds require `VCPKG_ROOT` to be set before CMake configuration
- Codegen requires Homebrew LLVM on macOS; Apple Clang does not support the required flags
- The `RelWithDebInfo` configuration is recommended for iterative development
- CRT functions hooked via rexcrt (32 functions) must appear in both `[hooks]` and `[functions]` sections of the recompiler config to force proper code splitting
