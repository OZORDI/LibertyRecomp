# 53 — RexGlue Native Audio: How Xbox 360 XAudio2 Is Replaced

## Summary

RexGlue does **not** intercept XAudio2 COM vtable calls. The Xbox 360 audio
model is fundamentally different from PC XAudio2 — it uses a kernel-level
render driver API (XAudio*) and a hardware XMA decoder. RexGlue replaces both
at the kernel export layer, routing all audio output through SDL2. There is
no COM object interception because Xbox 360 games never instantiate COM-based
XAudio2 objects; they call xboxkrnl exports directly.

---

## Architecture Overview

```
Guest game code
      |
      |  calls __imp__XAudioRegisterRenderDriverClient,
      |        __imp__XAudioSubmitRenderDriverFrame,
      |        __imp__XMACreateContext, etc.
      v
RexGlue XBOXKRNL export layer
  (glue/rexglue-sdk-main/src/kernel/xboxkrnl/xboxkrnl_audio.cpp)
  (glue/rexglue-sdk-main/src/kernel/xboxkrnl/xboxkrnl_audio_xma.cpp)
      |
      v
rex::audio::AudioSystem  (abstract base)
      |
      +---> rex::audio::sdl::SDLAudioSystem  (concrete, SDL2 backend)
      |         creates SDLAudioDriver per registered client
      |
      +---> rex::audio::nop::NopAudioSystem   (no-op fallback)
      |
      +---> XmaDecoder  (XMA compressed audio → PCM, via FFmpeg)
                         (compiled out on macOS: REX_NO_XMA_DECODER=1)
```

---

## 1. XAudio Kernel Exports (Render Driver)

**File**: `glue/rexglue-sdk-main/src/kernel/xboxkrnl/xboxkrnl_audio.cpp`

Xbox 360 games interact with audio through **xboxkrnl** exports, not through
COM interfaces. The key exports are:

### Fully implemented (7 functions)

| Export | Role |
|--------|------|
| `XAudioRegisterRenderDriverClient` | Registers a guest callback + creates an `SDLAudioDriver` instance. Returns a driver handle `0x4155xxxx` encoding the client index. |
| `XAudioUnregisterRenderDriverClient` | Tears down the SDL audio device for that client. |
| `XAudioSubmitRenderDriverFrame` | Copies 6-channel × 256-sample BE float frame from guest memory into the SDL driver's queue. |
| `XAudioGetSpeakerConfig` | Returns `0x00010001` (stereo). |
| `XAudioGetVoiceCategoryVolumeChangeMask` | Returns 0 (no changes). Calls `MaybeYield()`. |
| `XAudioGetVoiceCategoryVolume` | Returns 1.0f (full volume). |
| `XAudioEnableDucker` | No-op success. |

### Stubbed (22 functions)

All remaining XAudio exports are `XBOXKRNL_EXPORT_STUB` — they return success
without performing any work. This includes ducking, digital bypass, speaker
config overrides, MEC client, performance queries, and process-frame callbacks.

---

## 2. XMA Decoder Exports

**File**: `glue/rexglue-sdk-main/src/kernel/xboxkrnl/xboxkrnl_audio_xma.cpp`

The Xbox 360's hardware XMA decoder is emulated by RexGlue's `XmaDecoder`
class. 21 kernel exports manipulate XMA context data structures:

- `XMACreateContext` / `XMAReleaseContext` — allocate/free from a context pool
- `XMAInitializeContext` — set up input/output buffer pointers (virtual → physical address translation)
- `XMAEnableContext` / `XMADisableContext` — trigger decoding via register writes
- `XMABlockWhileInUse` — spin-wait until input buffers are consumed
- `XMASetInputBuffer0/1`, `XMAIsInputBuffer0/1Valid`, etc.
- `XMAGetOutputBufferReadOffset/WriteOffset`, `XMASetOutputBufferValid`
- `XMASetLoopData`, `XMAGetPacketMetadata`

**macOS limitation**: XMA decoding depends on FFmpeg's `libavcodec` with XMA
support. On macOS, `REX_NO_XMA_DECODER=1` is set and all XMA functionality is
compiled out. LibertyRecomp's `imports.cpp` provides stub fallbacks for
`XMACreateContext` (returns `STATUS_NOT_IMPLEMENTED`) and `XMAReleaseContext`.

---

## 3. LibertyRecomp's Own Audio Layer (Parallel to RexGlue)

LibertyRecomp has a **second**, independent audio implementation in
`LibertyRecomp/apu/` that handles GTA IV-specific audio needs:

### 3a. SDL2 Render Driver (`apu/audio.cpp` + `apu/driver/sdl2_driver.cpp`)

This is LibertyRecomp's custom audio thread, separate from RexGlue's. It:

1. Opens an SDL2 audio device (48 kHz, float32, 6-channel or stereo)
2. Spawns a dedicated `AudioThread` that:
   - Creates a `GuestThreadContext` for PPC callback execution
   - Calls the guest's registered audio callback at ~5.33ms intervals (256 samples / 48 kHz)
   - Also invokes `sub_82172BE8` (audio event callback) to signal `byte_83137B80`, waking the game's audio worker thread
3. `XAudioSubmitFrame` byte-swaps BE float samples → LE and queues via `SDL_QueueAudio`
4. Supports stereo downmix (5.1 → 2.0 with center channel at 0.75 weight)

A critical addition: `InitializeAudioManager` calls the game's `sub_82168C08`
to set up the audio manager pointer at `0x83137CA8` before the SDL thread
starts. Without this, `sub_82172BE8 → sub_82169B00` would crash on a NULL read.

### 3b. XMA Playback Engine (`apu/xma_decoder.cpp`)

LibertyRecomp has its own XMA decoder using FFmpeg's `AV_CODEC_ID_XMAFRAMES`
codec. This hooks specific GTA IV guest functions (not kernel exports) via
`GUEST_FUNCTION_HOOK`:

| Guest Address | Function |
|---------------|----------|
| `sub_8255C090` | `XMAPlaybackCreate` |
| `sub_8255C398` | `XMAPlaybackSubmitData` |
| `sub_8255C5F0` | `XMAPlaybackConsumeDecodedData` |
| `sub_8255C2C0` | `XMAPlaybackDestroy` |
| ... | (20 total hooks) |

Each `XmaPlayback` instance owns an FFmpeg codec context and runs a dedicated
decoder thread. The decoded PCM data is written to a ring buffer that the game
reads from via `XMAPlaybackConsumeDecodedData`.

### 3c. Audio State Manager (`apu/audio_state.h` + `audio_state.cpp`)

Tracks playback lifecycle (INACTIVE → LOADING → PLAYING → STOPPED) for proper
Xbox audio state emulation. The game's `sub_822FA4E8` checks whether audio
objects are in the PLAYING state — this manager provides that information since
there is no XMA hardware setting status bits.

### 3d. Embedded Player (`apu/embedded_player.cpp`)

Uses SDL_mixer (`Mix_*` API) for UI sounds and installer music. Completely
independent of game audio — loads embedded OGG resources for menu effects.

### 3e. Audio Volume Manager (`apu/audio_state.h`)

Manages per-category volume (Default, Music, SFX, Voice, Ambient, UI, Radio)
with a change bitmask, mirroring Xbox's `XAudioGetVoiceCategoryVolume` /
`XAudioGetVoiceCategoryVolumeChangeMask` semantics.

---

## 4. How RexGlue Intercepts Audio Calls

### No COM Vtable Interception

Xbox 360 games do **not** use COM-style `IXAudio2` interfaces. The PC XAudio2
COM API (IXAudio2, IXAudio2SourceVoice, IXAudio2MasteringVoice) exists only in
Xenia's Windows host-side driver (`tools/xenia-master-1/src/xenia/apu/xaudio2/`)
for playing decoded audio on Windows hosts. This code is **not used** by
RexGlue — it was replaced by the SDL2 backend.

### The Actual Interception Pattern

RexGlue uses **kernel export replacement**, not COM interception:

1. **Codegen** generates calls to `__imp__XAudioRegisterRenderDriverClient` etc.
   whenever the guest binary imports ordinal 0x1F3 from xboxkrnl.
2. **XBOXKRNL_EXPORT** macro links these `__imp__` symbols to C++ implementations
   in `xboxkrnl_audio.cpp`.
3. The implementations call into `rex::audio::AudioSystem` (abstract), which
   is instantiated as `SDLAudioSystem` via the `audio_factory` config:
   ```cpp
   rexConfig.audio_factory = REX_AUDIO_BACKEND(rex::audio::sdl::SDLAudioSystem);
   ```
4. `SDLAudioSystem::CreateDriver` creates an `SDLAudioDriver` that opens an
   SDL2 audio device and converts frames from Xbox format (6-channel
   sequential, big-endian float) to host format (interleaved, little-endian).

### Sample Format Conversion

**File**: `glue/rexglue-sdk-main/include/rex/audio/conversion.h`

Xbox 360 audio frames are 6 channels × 256 samples, stored as sequential
big-endian floats (all ch0 samples, then all ch1, etc.). Two conversion paths:

- **6-channel**: `sequential_6_BE_to_interleaved_6_LE` — byte-swap + interleave
  (SSE optimized on x86, scalar fallback on ARM)
- **Stereo downmix**: `sequential_6_BE_to_interleaved_2_LE` — standard 5.1→2.0
  fold-down: `L = (FL + BL + FC*0.5) / 2.5`, `R = (FR + BR + FC*0.5) / 2.5`

---

## 5. Data Flow Summary

### Render Driver Path (mixed audio → speakers)

```
Game's audio mixer (recompiled PPC)
  → XAudioSubmitRenderDriverFrame(__imp__)
    → AudioSystem::SubmitFrame(client_index, guest_ptr)
      → SDLAudioDriver::SubmitFrame(guest_ptr)
        → memcpy from guest memory to host queue
          → SDL callback: sequential_6_BE_to_interleaved_*_LE
            → SDL audio device → speakers
```

### XMA Decode Path (compressed audio → PCM buffers)

**RexGlue path** (non-macOS):
```
Game calls XMACreateContext → XMAInitializeContext → XMAEnableContext
  → XmaDecoder processes contexts via register writes
    → FFmpeg decodes XMA frames → output buffer in guest memory
      → Game reads decoded PCM → feeds to audio mixer
```

**LibertyRecomp path** (GTA IV-specific, hooks game functions directly):
```
Game calls sub_8255C090 (XMAPlaybackCreate)
  → Allocates XmaPlayback with FFmpeg AV_CODEC_ID_XMAFRAMES
  → Spawns decoder thread
Game calls sub_8255C398 (XMAPlaybackSubmitData)
  → Decoder thread: Decode() → Consume() → ring buffer
Game calls sub_8255C5F0 (XMAPlaybackConsumeDecodedData)
  → Returns PCM from ring buffer
```

### Audio Worker Thread Wakeup

```
SDL audio thread (5.33ms interval)
  → Calls guest audio callback (registered via XAudioRegisterRenderDriverClient)
    → Guest mixer runs, produces mixed frame
      → XAudioSubmitRenderDriverFrame
  → Calls sub_82172BE8 (audio event signal)
    → Sets byte_83137B80, wakes game's audio worker
```

---

## 6. Key Files Reference

| Path | Role |
|------|------|
| `glue/rexglue-sdk-main/src/kernel/xboxkrnl/xboxkrnl_audio.cpp` | XAudio render driver kernel exports |
| `glue/rexglue-sdk-main/src/kernel/xboxkrnl/xboxkrnl_audio_xma.cpp` | XMA decoder kernel exports |
| `glue/rexglue-sdk-main/src/audio/audio_system.cpp` | AudioSystem base class (worker thread, client management) |
| `glue/rexglue-sdk-main/src/audio/sdl/sdl_audio_driver.cpp` | SDL2 audio driver (device open, frame queue, callback) |
| `glue/rexglue-sdk-main/src/audio/sdl/sdl_audio_system.cpp` | SDL2 audio system factory |
| `glue/rexglue-sdk-main/include/rex/audio/conversion.h` | BE→LE sample format conversion (SSE + scalar) |
| `glue/rexglue-sdk-main/include/rex/audio/audio_system.h` | AudioSystem abstract class declaration |
| `glue/rexglue-sdk-main/include/rex/audio/audio_driver.h` | AudioDriver abstract class |
| `glue/rexglue-sdk-main/include/rex/audio/nop/nop_audio_system.h` | No-op fallback audio system |
| `glue/rexglue-sdk-main/include/rex/system/interfaces/audio.h` | IAudioSystem interface |
| `glue/rexglue-sdk-main/src/audio/CMakeLists.txt` | Build config (XMA compiled out on Apple) |
| `glue/rexglue-sdk-main/src/kernel/xboxkrnl/export_table.inc` | Kernel export ordinal table |
| `LibertyRecomp/apu/audio.cpp` | LibertyRecomp's own render driver client |
| `LibertyRecomp/apu/driver/sdl2_driver.cpp` | LibertyRecomp's SDL2 audio thread + frame submit |
| `LibertyRecomp/apu/audio.h` | Audio constants (48kHz, 6ch, 256 samples) |
| `LibertyRecomp/apu/xma_decoder.cpp` | GTA IV XMA playback hooks (FFmpeg) |
| `LibertyRecomp/apu/xma_decoder.h` | XmaPlayback struct with FFmpeg codec context |
| `LibertyRecomp/apu/audio_state.h` | AudioStateManager + AudioVolumeManager |
| `LibertyRecomp/apu/audio_state.cpp` | Playback state tracking implementation |
| `LibertyRecomp/apu/embedded_player.cpp` | UI sound effects via SDL_mixer |
| `LibertyRecomp/kernel/imports.cpp` | Stub XMACreateContext/XMAReleaseContext for macOS |
| `LibertyRecomp/main.cpp` | Configures `audio_factory = SDLAudioSystem` at line 536 |

---

## 7. Answers to Research Questions

**Q: How does RexGlue intercept XAudio2 COM vtable calls?**
A: It does not. Xbox 360 games use kernel-level XAudio exports, not COM
interfaces. RexGlue replaces these kernel exports with native implementations.

**Q: Is there an audio subsystem replacing Xbox 360 XAudio with host audio?**
A: Yes. `rex::audio::sdl::SDLAudioSystem` creates `SDLAudioDriver` instances
that open SDL2 audio devices. The game's audio mixer runs as recompiled PPC
code and submits mixed frames through the kernel export path.

**Q: Is there a pattern where RexGlue creates stub COM objects with
host-implemented vtables?**
A: No. The Xbox 360 audio API is not COM-based. The interception is purely at
the kernel export symbol level (`XBOXKRNL_EXPORT` → C++ function → AudioSystem).

**Q: What about XAudio2Create?**
A: `XAudio2Create` appears only in Xenia's Windows-host XAudio2 driver
(`tools/xenia-master-1/src/xenia/apu/xaudio2/xaudio2_audio_driver.cc`), which
is the **host-side** audio output path for Xenia on Windows. RexGlue replaced
this entirely with SDL2. The Xbox 360 guest never calls `XAudio2Create`.
