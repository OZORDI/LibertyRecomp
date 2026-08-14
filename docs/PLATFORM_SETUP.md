# Platform Setup Guide

This document covers environment setup for all supported build targets.
Desktop platforms (Windows, Linux, macOS) are covered in [BUILDING.md](BUILDING.md).

---

## Table of Contents

1. [iOS (Metal)](#ios-metal)
2. [Android (Vulkan)](#android-vulkan)
3. [PS4 / OpenOrbis (Vulkan)](#ps4--openorbis-vulkan)
4. [Nintendo Switch / libnx (Vulkan)](#nintendo-switch--libnx-vulkan)
5. [Build-time Asset Payload](#build-time-asset-payload)
6. [CI / Caching Notes](#ci--caching-notes)

---

## iOS (Metal)

### Requirements

| Tool | Version | Notes |
|------|---------|-------|
| macOS host | ≥ 13.0 | Required by Xcode |
| Xcode | ≥ 15 | Includes iOS 17+ SDK |
| iOS Deployment Target | ≥ 16.0 | Set in toolchains/ios.cmake |
| SDL2 | ships in thirdparty/ | Built via CMake with `SDL2_IOS=ON` |

### Quick Start

```bash
# 1. Make sure Xcode command-line tools are installed
xcode-select --install

# 2. Configure + open Xcode in one step (recommended)
./scripts/ios-setup.sh <YOUR_TEAM_ID>
# e.g. ./scripts/ios-setup.sh ABCDE12345

# 3. Build & run on device or simulator from within Xcode
```

Find your Team ID at <https://developer.apple.com/account> (Membership tab).
The script wraps `cmake --preset ios-debug` with the signing cache variables
pre-filled and then opens the generated `.xcodeproj`.

### Manual Configure

If you prefer to drive CMake directly:

```bash
cmake --preset ios-debug \
      -DLIBERTY_IOS_DEVELOPMENT_TEAM=ABCDE12345 \
      -DLIBERTY_IOS_BUNDLE_IDENTIFIER=com.libertyrecomp
open out/build/ios-debug/LibertyRecomp.xcodeproj
```

### Signing Cache Variables

|Variable|Default|Purpose|
|-|-|-|
|`LIBERTY_IOS_DEVELOPMENT_TEAM`|(empty)|10-char Apple Team ID. Required for on-device builds. Simulator works without it.|
|`LIBERTY_IOS_BUNDLE_IDENTIFIER`|`com.libertyrecomp`|Bundle ID. Must match your provisioning profile.|
|`LIBERTY_IOS_PROVISIONING_PROFILE`|(empty)|Optional profile UUID. Leave blank for automatic signing.|
|`LIBERTY_MACOS_BUNDLE_IDENTIFIER`|`OZORDI.LibertyRecomp`|Bundle ID for macOS builds. Must match the Game Center app record when shipping macOS achievements.|
|`LIBERTY_MACOS_CODE_SIGN_IDENTITY`|(empty)|Optional macOS signing identity. Set this for Ninja-built macOS bundles that need Game Center entitlement signing.|

The iOS values feed the Xcode target attributes `DEVELOPMENT_TEAM`,
`PRODUCT_BUNDLE_IDENTIFIER`, `CODE_SIGN_IDENTITY` (`iPhone Developer`), and
`CODE_SIGN_STYLE` (`Automatic`), so the generated project signs itself
without manual clicks in the Xcode UI.

### Game Center

iOS and macOS builds include the Game Center entitlement. Enable Game Center for
the app's bundle ID in App Store Connect and create achievements named
`ach_001` through `ach_065`, plus `ach_platinum`. IDs `ach_051` through
`ach_055` are TLAD achievements, and `ach_056` through `ach_065` are TBoGT
achievements.

### Key Details

- Graphics backend: **Metal** (same codepath as macOS, `LIBERTY_RECOMP_METAL=ON`)
- No installer wizard — game assets are read from `<Bundle>/game/` via `EmbeddedAssets`
- DLC/TU toggles remain accessible in the in-game settings menu
- iOS sandbox: saves go to `<Documents>/LibertyRecomp/saves/`
- Logging: unified logging (`os_log`), visible in Console.app or the Xcode debug console

---

## Android (Vulkan)

### Requirements

| Tool | Version | Notes |
|-|-|-|
| Android SDK | ≥ 34 | Install via Android Studio or `sdkmanager` |
| Android NDK | r27c (27.2.12479018) | `sdkmanager --install 'ndk;27.2.12479018'` |
| CMake (SDK bundled) | ≥ 3.31 (3.31.4) | `sdkmanager --install 'cmake;3.31.4'` |
| Java | ≥ 17 | Required by Gradle 8.x |
| Gradle | 8.11.1 | Wrapper committed in `os/android/` |

### Environment Variables

```bash
export ANDROID_HOME=/path/to/android-sdk        # or ANDROID_SDK_ROOT
export ANDROID_NDK=$ANDROID_HOME/ndk/27.2.12479018
export JAVA_HOME=/path/to/jdk-17
```

### Quick Start

```bash
# Option A — Gradle (recommended, produces a full APK)
cd os/android/
./gradlew assembleDebug
# APK: os/android/app/build/outputs/apk/debug/app-debug.apk
adb install -r app/build/outputs/apk/debug/app-debug.apk

# Option B — CMake directly (native library only, no APK)
cmake --preset android-release
cmake --build out/build/android-release
```

### Game Assets

Game files are **not** embedded in the APK. On first launch the app shows a
folder-picker that asks the user to select the directory containing
`default.xex` and the RPF archives. The chosen path is persisted in
`SharedPreferences` so subsequent launches skip the picker.

Place your extracted game files on the device (e.g. via `adb push`) before
launching:

```bash
adb push /path/to/game/ /sdcard/LibertyRecomp/
```

### Release Signing

Debug builds are signed automatically with the shared Android debug key.
For release APKs, supply a keystore via Gradle properties:

```bash
./gradlew assembleRelease \
  -PlibertyRecompKeystore=/path/to/keystore.jks \
  -PlibertyRecompKeystorePassword=changeit \
  -PlibertyRecompKeyAlias=liberty \
  -PlibertyRecompKeyPassword=changeit
```

### Key Details

- Graphics backend: **Vulkan** (`LIBERTY_RECOMP_VULKAN=ON`, forced by toolchain)
- ABI: `arm64-v8a` (default); add `-PlibertyRecompEmulator=true` for x86_64 emulator builds
- JNI glue (`os/android/jni_glue.cpp`) receives paths from `LibertySDLActivity.java`
  before SDL initialises
- Vibration: `VibratorManager` (API 31+), single-`Vibrator` fallback (API 26+)
- Achievements: Play Games v2, graceful no-op when SDK is absent
- Logging: **logcat** — `adb logcat -s LibertyRecomp`

### Verify NDK

```bash
$ANDROID_NDK/build/cmake/android.toolchain.cmake  # should exist
$ANDROID_NDK/toolchains/llvm/prebuilt/*/bin/clang --version
```

---

## PS4 / OpenOrbis (Vulkan)

> **Important:** This targets **jailbroken** PS4 consoles running a supported firmware via the
> [Mira](https://github.com/OpenOrbis/mira-project) homebrew enabler.
> Production/retail PS4 distribution requires an official Sony partner SDK (NDA).

### Requirements

| Tool | Version | Notes |
|------|---------|-------|
| LLVM/Clang | ≥ 13 | `brew install llvm` / `apt install clang lld` |
| lld | ≥ 13 | Ships with LLVM |
| OpenOrbis Toolchain | latest | Already cloned → `tools/OpenOrbis-PS4-Toolchain/` |
| LibOrbisPkg | bundled with OpenOrbis | For `.pkg` creation |

### Environment Variables

```bash
# Point at the already-cloned toolchain in this repo
export OO_PS4_TOOLCHAIN="$(pwd)/tools/OpenOrbis-PS4-Toolchain"
```

### Quick Start

```bash
# 1. Install LLVM (macOS)
brew install llvm
export PATH="$(brew --prefix llvm)/bin:$PATH"

# 2. Set toolchain env var
export OO_PS4_TOOLCHAIN="$(pwd)/tools/OpenOrbis-PS4-Toolchain"

# 3. Configure and build
cmake --preset ps4-release
cmake --build out/build/ps4-release

# 4. Package as PS4 PKG (TODO: LibOrbisPkg script in build/pkg/)
```

### Key Details

- Graphics backend: Vulkan via OpenOrbis SDL-PS4 layer
- Binary target: `eboot.bin` inside a `.pkg` (title ID `LBTY00001`)
- Filesystem: `/app0/` (read-only game data) + `/user/data/LBTY00001/` (saves)
- Logging: `sceKernelDebugOutText` → visible in GDB/LLDB over network or Mira's `dbg` module

### Verify Toolchain

```bash
ls $OO_PS4_TOOLCHAIN/include/orbis/   # should show libkernel.h, etc.
clang --version                        # should be LLVM ≥ 13
lld --version
```

---

## Nintendo Switch / libnx (Vulkan)

> **Important:** This targets the **homebrew** environment via the Atmosphere custom firmware.
> Retail eShop distribution requires an official Nintendo developer account and NDA SDK.

### Requirements

| Tool | Version | Notes |
|------|---------|-------|
| devkitPro | latest | https://devkitpro.org/wiki/Getting_Started |
| devkitA64 | ships with devkitPro | `dkp-pacman -S devkitA64` |
| libnx | latest | `dkp-pacman -S switch-dev` |
| SDL2 (libnx port) | ships with switch-dev | Included in `switch-dev` group |

### Installation

**macOS / Linux:**
```bash
# 1. Install devkitPro pacman bootstrap
# Follow: https://devkitpro.org/wiki/Getting_Started

# 2. Install the Switch dev group (includes devkitA64 + libnx + SDL2)
dkp-pacman -S switch-dev

# 3. Source the environment
source /opt/devkitpro/devkita64/environment-setup-aarch64-none-elf
# Or add to your shell profile:
echo 'export DEVKITPRO=/opt/devkitpro' >> ~/.bashrc
echo 'export DEVKITA64=/opt/devkitpro/devkitA64' >> ~/.bashrc
```

**Windows:**
```powershell
# Use the MSYS2-based devkitPro installer from https://github.com/devkitPro/installer/releases
# Then open the devkitPro MSYS2 shell and run dkp-pacman -S switch-dev
```

### Quick Start

```bash
# 1. Verify installation
dkp-pacman -Q switch-dev    # should list installed version

# 2. Configure
cmake --preset switch-release

# 3. Build — produces LibertyRecomp.nro
cmake --build out/build/switch-release

# 4. Deploy to SD card
cp out/build/switch-release/LibertyRecomp.nro /Volumes/sd/switch/LibertyRecomp/
# Or use nxlink: nxlink -s LibertyRecomp.nro
```

### Key Details

- Graphics backend: Vulkan via SDL2/libnx (`LIBERTY_RECOMP_VULKAN=ON`)
- Output: `.nro` homebrew executable
- Assets: packed into **RomFS** at build time, mounted as `romfs:/` at runtime via `romfsInit()`
- Saves: `sdmc:/LibertyRecomp/saves/`
- Logging: `svcOutputDebugString` → visible via `nxlink -s` or Atmosphere sysmodule log

### Verify devkitPro

```bash
dkp-pacman -Q switch-dev       # e.g. switch-dev 1.0.0-1
$DEVKITA64/bin/aarch64-none-elf-gcc --version
ls $DEVKITPRO/libnx/include/switch.h
```

---

## Build-time Asset Payload

For all non-desktop platforms, game files must be staged before configure time.

### Layout

```
tools/local_game_payload/
├── default.xex    ← REQUIRED
├── common.rpf     ← REQUIRED
├── xbox360.rpf    ← REQUIRED
├── audio.rpf      ← REQUIRED
└── dlc/
    ├── TLAD/      ← optional
    └── TBOGT/     ← optional
```

### Overriding the Path

```bash
cmake --preset ios-release \
      -DLIBERTY_RECOMP_EMBEDDED_GAME_PATH=/path/to/your/gta_iv_files
```

CMake will fail fast at configure time with a clear error if any required file is missing.

---

## CI / Caching Notes

- **Never commit** game files or toolchain SDKs to the repository.
  The `.gitignore` already excludes `tools/local_game_payload/` (except `README.md`).
- For CI, mount the game payload as a secret/artifact volume into `tools/local_game_payload/`.
- Toolchain caches:
  - **Android NDK**: cache `$ANDROID_HOME/ndk/` by NDK version string
  - **devkitPro**: cache `/opt/devkitpro/` — invalidate on `dkp-pacman -Syu`
  - **OpenOrbis**: already in the repo under `tools/OpenOrbis-PS4-Toolchain/` (submodule)
  - **Xcode/iOS SDK**: available on GitHub-hosted macOS runners; no extra caching needed
