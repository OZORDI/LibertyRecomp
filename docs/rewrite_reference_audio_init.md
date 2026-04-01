# Reference: Audio/Streaming Init in UnleashedRecomp and MarathonRecomp

## Summary

Both projects use identical architecture (MarathonRecomp is a direct fork of UnleashedRecomp with minor additions). Neither stubs audio — they fully replace the Xbox 360 XAudio render driver with a custom SDL2-based backend.

---

## XAudio Render Driver — Hook Strategy (not stub)

All three XAudio render driver imports are hooked in `kernel/imports.cpp`:

| Import | Handler |
|-|-|
| `XAudioRegisterRenderDriverClient` | Registers guest callback + spins up SDL2 audio thread |
| `XAudioUnregisterRenderDriverClient` | Returns 0 (no-op) |
| `XAudioSubmitRenderDriverFrame` | Passes frame to SDL2 queue |

The implementations live in `apu/audio.cpp`. `XAudioRegisterRenderDriverClient` stores a magic token (`'DAUD'`) as the driver handle, then calls `XAudioRegisterClient()` to hand the game's render callback to the SDL2 layer.

Two additional XAudio imports are stubbed:
- `XAudioGetVoiceCategoryVolume` — logs `!!! STUB !!!`, returns nothing
- `XAudioGetVoiceCategoryVolumeChangeMask` — writes `Mask = 0`, returns 0

XMA context functions (`XMACreateContext`, `XMAReleaseContext`) are also stubbed with log-only bodies.

---

## SDL2 Audio Backend (`apu/driver/sdl2_driver.cpp`)

`XAudioInitializeSystem()` is called from `main.cpp` **before** the recompiled game code runs. It:
1. Forces WASAPI on Windows (`SDL_setenv`)
2. Calls `SDL_InitSubSystem(SDL_INIT_AUDIO)`
3. Opens an SDL2 audio device at 48kHz, 6ch (or 2ch stereo), 256-sample frames — matching Xbox 360 XAudio specs exactly

When the game calls `XAudioRegisterRenderDriverClient`, the host:
1. Allocates a guest heap buffer for the callback parameter
2. Stores the game's render callback pointer
3. Launches a dedicated `std::thread` (`AudioThread`) that polls `SDL_GetQueuedAudioSize` and calls the game's PPC render callback at 48000/256 = ~187.5 Hz

`XAudioSubmitFrame` interleaves the game's planar 6-channel float output into SDL2's interleaved format, applying `MasterVolume`. Stereo downmix uses fixed pan weights (center = 0.75, LFE = 0).

MarathonRecomp adds NaN guards on each sample and a `MuteOnFocusLost` path.

---

## No RexGlue Audio

Neither project uses RexGlue's audio backend at all. The SDL2 driver is entirely self-contained. There is no `SDL_OpenAudio` (old API) — they use `SDL_OpenAudioDevice`.

---

## Streaming Thread

Neither project stubs or hooks streaming thread creation. The game's `CreateThread` calls for its streaming/IO workers run normally through the kernel's `GuestThread` implementation. The audio thread is a **host-side** `std::thread`, not a guest PPC thread.

---

## Audio-Related Patches

**UnleashedRecomp** (`patches/audio_patches.cpp`):
- Stubs `sub_82E58728` (volume setter): `GUEST_FUNCTION_STUB`
- `AudioPatches::Update()` writes `Config::MusicVolume` / `Config::EffectsVolume` directly to game memory addresses each frame
- Hack to keep `se_system_worldmap.csb` permanently loaded

**MarathonRecomp** (`patches/audio_patches.cpp`):
- Hooks `sub_8260F168` (CRI cue update) to inject correct `deltaTime`
- `AudioPatches::Update()` writes volume through a typed `AudioEngineXenon` API struct
- Additional fixes: jingle fade timing, XMV voice language selection

---

## Config Keys (both projects)

| Key | Default | Notes |
|-|-|-|
| `Audio/MasterVolume` | 1.0 | Applied in `XAudioSubmitFrame` |
| `Audio/MusicVolume` | 1.0 / 0.6 | Written to game memory each frame |
| `Audio/EffectsVolume` | 1.0 / 0.6 | Written to game memory each frame |
| `Audio/ChannelConfiguration` | Stereo | Controls SDL2 channel count |
| `Audio/MusicAttenuation` | false | Fades music when OS media plays |

---

## Implications for LibertyRecomp

GTA IV's audio init chain (sub_82478AF8 → XAudio render driver registration) follows the same pattern these projects handle. The correct approach is:

1. Hook `XAudioRegisterRenderDriverClient` / `XAudioSubmitRenderDriverFrame` exactly as shown above — do **not** stub them
2. Call `XAudioInitializeSystem()` (SDL2 device open) from host main before `rex::Runtime::Setup()`
3. The game's XAudio render thread will call the registered PPC callback; the host SDL2 thread services those frames
4. GTA IV likely also calls `XAudioGetVoiceCategoryVolume` / `XAudioGetVoiceCategoryVolumeChangeMask` — stub both identically

The key difference: GTA IV uses XMA2 for compressed audio, so `XMACreateContext` / `XMAReleaseContext` need real implementations (MarathonRecomp has an XMA decoder in `apu/xma_decoder.cpp` ported from Xenia Canary) rather than log-stubs.
