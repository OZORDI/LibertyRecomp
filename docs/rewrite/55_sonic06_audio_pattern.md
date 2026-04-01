# Sonic '06 Audio Pattern Research for GTA IV

## 1. Sonic '06 / sonicnext-dev Heritage

LibertyRecomp is forked from the sonicnext-dev Sonic '06 recompilation project. Key
upstream references:

- `.gitmodules` points to `sonicnext-dev/XenonRecomp`, `sonicnext-dev/XenosRecomp`,
  `sonicnext-dev/rexglue-sdk`, `sonicnext-dev/ffmpeg-core`
- `LibertyRecompLib/config/Liberty.toml` retains Sonic '06 config entries:
  `SonicCamera_InvertAzDriveK`, `SonicGauge_FixFlags`,
  `Sonicteam::MyPE::CManageParticle`, etc.
- `LibertyRecomp/patches/` still contains Sonic '06-era files:
  `TitleTask_patches.cpp`, `MainMenuTask_patches.cpp`, `player_patches.cpp`,
  `camera_patches.cpp`, `aspect_ratio_patches.cpp` (with Sonic '06 Audio Room
  sprite table entries)
- `LibertyRecomp/hid/driver/sdl_hid.cpp` references `EPlayerCharacter::Sonic`

The project was adapted for GTA IV by adding GTA IV-specific patches
(`gta4_patches.cpp`, `gta4_aspect_ratio_patches.cpp`, `gta4_input_patches.cpp`,
`gta4_ds4_patches.cpp`, `gta4_motion_patches.cpp`), audio hooks
(`audio_patches.cpp`), and a complete save/IO/kernel stack.

## 2. Audio Architecture Overview

LibertyRecomp has a **two-layer** audio architecture:

### Layer 1: RexGlue SDK (Xenia-derived kernel exports)

Location: `glue/rexglue-sdk-main/src/kernel/xboxkrnl/xboxkrnl_audio.cpp`

The RexGlue SDK provides Xbox 360 kernel-level audio exports. These are the
functions that appear in the XEX import table:

| Export | Implementation |
|--------|---------------|
| `XAudioRegisterRenderDriverClient` | `AudioSystem::RegisterClient()` -- creates an AudioDriver, spawns worker thread |
| `XAudioSubmitRenderDriverFrame` | `AudioSystem::SubmitFrame()` -- forwards to driver |
| `XAudioUnregisterRenderDriverClient` | `AudioSystem::UnregisterClient()` |
| `XAudioGetVoiceCategoryVolumeChangeMask` | Returns 0 (no changes) |
| `XAudioGetVoiceCategoryVolume` | Returns 1.0f |
| `XAudioGetSpeakerConfig` | Returns `0x00010001` |
| `XAudioEnableDucker` | Stub (SUCCESS) |
| 20+ others | `XBOXKRNL_EXPORT_STUB` (no-op) |

The `AudioSystem` class (`glue/rexglue-sdk-main/src/audio/audio_system.cpp`) is
Xenia-derived. It:
- Manages up to `kMaximumClientCount` audio clients
- Runs a worker thread that calls back into guest PPC code via `Processor::Execute`
- Creates per-client `AudioDriver` instances (SDL backend)
- Supports XMA decoding via `XmaDecoder` (disabled on macOS with `__APPLE__` guard)

### Layer 2: LibertyRecomp Overrides (Liberty-owned audio)

Location: `LibertyRecomp/apu/`

LibertyRecomp **overrides** the RexGlue audio path with its own implementation:

**`audio.cpp`** -- Overrides `XAudioRegisterRenderDriverClient` and
`XAudioSubmitRenderDriverFrame`:
- Before starting the SDL2 audio thread, calls `InitializeAudioManager()` which
  invokes GTA IV's `sub_82168C08` (audio manager init) via `GuestThreadContext`
- This ensures the audio manager pointer at `0x83137CA8` is valid before
  `sub_82172BE8` -> `sub_82169B00` executes
- The driver key is `'DAUD'` (simple uint32 sentinel, NOT a COM vtable object)

**`driver/sdl2_driver.cpp`** -- Runs its own audio thread:
- Creates SDL2 audio device (48kHz, float32, 6 channels or stereo downmix)
- Audio thread invokes the guest callback (from `XAudioRegisterRenderDriverClient`)
  periodically at the XAudio frame rate (256 samples / 48000 Hz)
- Also invokes `sub_82172BE8` (audio event callback) to wake the game's audio
  worker thread (`sub_82169400`)
- Handles stereo downmix and surround sound passthrough

**`xma_decoder.cpp`** -- Full XMA frame decoder (Xenia-derived):
- 21 `GUEST_FUNCTION_HOOK` registrations for XMAPlayback* functions
- Addresses are in the `0x8255Cxxx` range (XMA playback library in GTA IV v8)
- Uses FFmpeg's `AV_CODEC_ID_XMAFRAMES` for decoding
- Integrates with `AudioStateManager` for playback state tracking

**`audio_state.cpp`** -- Tracks XMA playback states:
- Replaces Xbox hardware state flags that `sub_822FA4E8` would check
- Maps `XmaPlayback*` objects to guest addresses
- Tracks PLAYING/LOADING/STOPPED/INACTIVE states

**`embedded_player.cpp`** -- UI sound effects (SDL_mixer):
- Plays embedded OGG sounds for menu navigation
- Separate from the game's XMA audio pipeline

## 3. Vtable Stub Pattern Analysis

### Does the Sonic '06 project create fake COM objects with stub vtables?

**No.** The codebase does NOT use fake COM objects or IUnknown-style stub vtables
for audio. The patterns found are:

1. **Simple sentinel values for driver handles**: The `XAudioRegisterRenderDriverClient`
   override returns `AUDIO_DRIVER_KEY = 'DAUD'` as the driver handle. The RexGlue
   version returns `0x41550000 | index`. Neither creates a COM-style vtable object.

2. **Guest-memory vtable pre-population**: `memory.cpp` + `vtable_prepopulate.h`
   write function pointers into guest memory at vtable addresses extracted from the
   XEX. This is for recompiled game code to dispatch virtual calls correctly --
   NOT for creating fake host-side COM objects. There are 146 vtables with 3809
   total entries, plus ~25 manually written vtables in `PopulateFunctionTableAndVtables()`.

3. **GPU vftable references**: `video.cpp` reads guest-memory vftable pointers
   (`MODEL_DATA_VFTABLE = 0x82073A44`, `TERRAIN_MODEL_DATA_VFTABLE = 0x8211D25C`,
   `PARTICLE_MATERIAL_VFTABLE = 0x8211F198`) to dispatch rendering by object type.
   These are guest-side vtable reads, not host-side fake COM objects.

4. **Havok physics vftable**: API headers in `api/hk330/` define `Vftable` structs
   with `xpointer<>` fields for guest-memory vtable dispatch.

5. **Boost shared_ptr**: `api/boost/smart_ptr/shared_ptr.h` defines a guest-side
   `vftable_t` with `dispose` and `destroy` entries, dispatched via
   `GuestToHostFunction<>`.

### Pattern summary

The established pattern is: **guest-memory vtables with PPC function pointers,
dispatched by recompiled code or by `GuestToHostFunction<>` / `g_memory.FindFunction()`**.
There are NO host-side fake COM objects with C++ vtables passed to guest code.

## 4. Existing Hooks for Audio Render Functions

### sub_821910D0

**Not hooked.** No `GUEST_FUNCTION_HOOK`, `PPC_FUNC_HOOK`, or `InsertFunction` for
this address exists in the codebase. The address falls in the `0x82191xxx` range
which has 23 `PPC_EXTERN_IMPORT` declarations in `ppc_func_decls.h` (from
`sub_82191080` to `sub_82191FE0`), meaning these functions were recompiled and exist
as generated code.

### sub_8219A2B8

**Not hooked.** The nearby address `sub_8219A2E8` appears as `PPC_EXTERN_IMPORT` in
`ppc_func_decls.h` but `sub_8219A2B8` itself does not appear anywhere. This suggests
it may be an interior label or was not identified as a function entry point during
codegen.

### Audio-related hooks that DO exist

| Hook Target | Type | File | Purpose |
|-------------|------|------|---------|
| `sub_82168C08` | Called via `g_memory.FindFunction()` | `apu/audio.cpp` | Audio manager init (called before SDL thread start) |
| `sub_82172BE8` | Called via `g_memory.FindFunction()` | `apu/driver/sdl2_driver.cpp` | Audio event callback (wakes audio worker) |
| `sub_821966D0` | `InsertFunction` | `kernel/memory.cpp` | RMPTFX worker thread gate (suspend during init) |
| `sub_822EEDB8` | `PPC_FUNC_HOOK` | `patches/audio_patches.cpp` | RAGE audEngine init tracking |
| `sub_821AB5F8` | `PPC_FUNC_HOOK` | `patches/audio_patches.cpp` | Radio system init tracking |
| `sub_82910858` | `PPC_FUNC_HOOK` | `patches/audio_patches.cpp` | Audio node tree traversal depth limiter (max 64) |
| `sub_822FA4E8` | Referenced in `audio_state.h` | -- | Audio object ready check (state == 2) |
| 21 XMAPlayback* funcs | `GUEST_FUNCTION_HOOK` | `apu/xma_decoder.cpp` | Full XMA decode pipeline |

## 5. Audio Patches (patches/audio_patches.cpp)

The GTA IV audio patches provide:

1. **Volume management**: `GTA4Audio` namespace tracks music/effects/master volumes
   with config sync via `AudioConfigChanged()`

2. **Radio system stubs**: `GTA4Radio` namespace for station selection and volume

3. **PPC hooks**:
   - `sub_822EEDB8` (audEngine init) -- marks audio initialized after original runs
   - `sub_821AB5F8` (radio init) -- logs radio system initialization
   - `sub_82910858` (audio node traversal) -- depth-limited to 64 to prevent stack
     overflow from circular node references

4. **Attenuation**: Platform-specific support (macOS/Linux/Windows) for external
   media ducking

## 6. How the Sonic '06 Project Handled Audio (Pattern for GTA IV)

The Sonic '06 project's audio approach, which LibertyRecomp inherits and extends:

### Approach: Intercept at kernel export level, let game code run

1. **Kernel exports are fully implemented** (not stubbed): `XAudioRegisterRenderDriverClient`,
   `XAudioSubmitRenderDriverFrame`, `XAudioGetVoiceCategoryVolume` all have real
   implementations that create SDL audio devices and pump frames.

2. **Game-side audio code runs as recompiled PPC**: The audio manager init
   (`sub_82168C08`), audio worker thread (`sub_82169400`), and render callbacks are
   all recompiled code that executes on the host. They are NOT replaced.

3. **Only blocking/hardware paths are intercepted**: GPU shader compilation
   (`sub_8285B088`), GPU fence waits (`sub_8285C648`), and ring buffer submissions
   (`sub_8285D018`) are stubbed because they talk to non-existent Xenos hardware.
   Audio follows the same pattern: XMA hardware decode is replaced by FFmpeg, but the
   game's audio mixing/routing code runs natively.

4. **Orphaned init functions are called explicitly**: `sub_82168C08` is described as
   "ORPHANED in the static call graph - never called by game code" -- it must be
   called by the host before the audio thread starts. This is done in
   `InitializeAudioManager()` using `GuestThreadContext`.

5. **No fake COM objects**: Driver handles are simple integer sentinels (`'DAUD'` or
   `0x4155xxxx`). The game code never QI's or AddRef/Release's these handles. They
   are opaque tokens passed back to kernel exports.

### What this means for GTA IV vtable stubs

If `sub_821910D0` or `sub_8219A2B8` need vtable-based audio objects, the pattern
would be:

1. **Write function pointers into guest memory** at the vtable address (like
   `PrePopulateVtables` does for 146 existing vtables)
2. **Hook the vtable dispatch targets** with `PPC_FUNC_HOOK` or `GUEST_FUNCTION_HOOK`
   if they touch hardware
3. **Do NOT create host-side C++ objects with fake vtables** -- the game reads vtable
   pointers from guest memory and dispatches via `PPC_CALL_INDIRECT_FUNC`

## 7. Key Findings Summary

| Question | Answer |
|----------|--------|
| Does the project create fake COM objects? | **No.** All vtables are guest-memory PPC function pointer tables. |
| Are sub_821910D0 / sub_8219A2B8 hooked? | **No.** Neither has any hook in the codebase. |
| How does Sonic '06 handle XAudio? | Intercepts kernel exports, lets recompiled game code handle mixing/routing. |
| What pattern for new vtable stubs? | Write PPC function pointers to guest memory vtable addresses; hook dispatch targets if they touch hardware. |
| Is XMA decoding working? | Yes -- 21 XMAPlayback hooks with FFmpeg backend. Disabled on macOS (no FFmpeg XMA support). |
| Audio manager initialized? | Yes -- `sub_82168C08` is explicitly called from `InitializeAudioManager()` before SDL thread starts. |
| Audio worker thread? | Yes -- SDL2 thread pumps guest callback at ~5.33ms intervals. Also calls `sub_82172BE8` to wake game's audio worker. |
