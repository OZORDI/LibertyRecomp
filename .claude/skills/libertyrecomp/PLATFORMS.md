# LibertyRecomp — Platform Guards

## Compile-time defines cheat sheet

| Define | Set when |
|--------|----------|
| `__ORBIS__` | PS4 (OpenOrbis toolchain) |
| `LIBERTY_RECOMP_PS4` | PS4 (also `add_compile_definitions` in CMakeLists) |
| `__SWITCH__` | Nintendo Switch (devkitPro libnx) |
| `__ANDROID__` | Android NDK |
| `TARGET_OS_IOS` | iOS (check `&& TARGET_OS_IOS` too) |
| `LIBERTY_RECOMP_EMBEDDED_ASSETS` | Any embedded build (PS4/Switch/iOS/Android) |
| `LIBERTY_RECOMP_NO_GNS` | PS4/Switch — GameNetworkingSockets not available |
| `LIBERTY_RECOMP_GAMECENTER` | macOS/iOS Game Center achievement bridge |

## Embedded build flow (`LIBERTY_RECOMP_EMBEDDED_ASSETS`)

When defined, `main.cpp` skips installer wizard and main menu entirely:
```
EmbeddedAssets::GetGameRoot()  →  platform game root
     ├── PS4:     /app0/game/
     ├── Switch:  romfs:/game/     (romfsInit() in switch_main.cpp __appInit)
     ├── iOS:     <Bundle>/game/
     └── Android: <internalStorage>/LibertyRecomp/game/  (extracted from OBB)
```

`EmbeddedAssets::GetGameRoot().parent_path()` = package root (contains `manifest.json`).

Android needs OBB extraction on first boot:
```cpp
extern const char* g_androidObbPath;  // set by JNI before SDL starts
EmbeddedAssets::ExtractObbIfNeeded(obbPath, destPath);
```

## User/save paths per platform (`user/paths.cpp`)

| Platform | `GetUserPath()` returns |
|----------|------------------------|
| PS4 | `/user/data/LBTY00001` |
| Switch | `sdmc:/LibertyRecomp` |
| Android | `<JNI internal path>/LibertyRecomp` |
| iOS | `<Documents>/LibertyRecomp` |
| macOS | `~/Library/Application Support/LibertyRecomp` |
| Linux | `~/.config/LibertyRecomp` |
| Windows | `%APPDATA%\LibertyRecomp` |

## PS4 audio (`__ORBIS__` guard in `apu/driver/voice_chat.cpp`)

SDL2 audio is not available on PS4. Use:
```cpp
#if defined(__ORBIS__)
// Capture
int handle = sceAudioInOpen(SCE_AUDIO_IN_TYPE_GENERAL, 256, VOICE_SAMPLE_RATE,
                             SCE_AUDIO_IN_FORMAT_S16_MONO);
sceAudioInInput(handle, buffer);
sceAudioInClose(handle);

// Playback
int handle = sceAudioOutOpen(SCE_USER_SERVICE_USER_ID_EVERYONE,
                              SCE_AUDIO_OUT_PORT_TYPE_VOICE, 0, 256,
                              VOICE_SAMPLE_RATE, SCE_AUDIO_OUT_CHANNEL_8CH_8CH);
sceAudioOutOutput(handle, stereo_buffer);  // mono → stereo expanded before call
sceAudioOutClose(handle);
#else
// SDL2 path for all other platforms
SDL_OpenAudioDevice(...);
#endif
```

Required sysmodules in `ps4_main.cpp::LoadRequiredModules()`:
- `ORBIS_SYSMODULE_AUDIO_IN` — microphone capture
- `ORBIS_SYSMODULE_NET` — BSD socket layer for raw P2P

## Multiplayer on PS4/Switch (`LIBERTY_RECOMP_NO_GNS`)

GameNetworkingSockets has no PS4/Switch port. These platforms use the raw-socket LAN fallback:
- `session_tracker_lan.cpp` — UDP broadcast discovery on port 3074
- Switch: `socketInitializeDefault()` called in `switch_main.cpp::__appInit()`
- PS4: BSD socket layer via `ORBIS_SYSMODULE_NET`

Community and Firebase backends still work (CURL is available on both platforms).

## CMake preset names

| Platform | Preset | EMBEDDED_ASSETS |
|----------|--------|----------------|
| macOS | `macos-release` | OFF (installer) |
| Windows | `x64-Clang-Release` | OFF |
| iOS | `ios-release` | ON |
| Android | `android-release` | ON |
| PS4 | `ps4-release` | ON |
| Switch | `switch-release` | ON |

## Payload layout (embedded builds)

Must exist at `LIBERTY_RECOMP_EMBEDDED_GAME_PATH` before configure:
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
CMake fails fast with a clear error if any required file is missing.
