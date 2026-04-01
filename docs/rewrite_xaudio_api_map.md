# XAudio Kernel API Map — Init Chain Analysis

## 1. All XAudio `__imp__` Call Sites in Generated Code

| API | Call site (generated file:line) | Parent function |
|-|-|-|
| `__imp__XAudioGetSpeakerConfig` | gta4_recomp.62.cpp:10855 | sub_8291F508 |
| `__imp__XAudioGetSpeakerConfig` | gta4_recomp.63.cpp:4457 | sub_82935A70 |
| `__imp__XAudioGetVoiceCategoryVolumeChangeMask` | gta4_recomp.2.cpp:2134 | sub_82191228 |
| `__imp__XAudioGetVoiceCategoryVolume` | gta4_recomp.2.cpp:2160 | sub_82191228 |
| `__imp__XMACreateContext` | gta4_recomp.2.cpp:8226 | sub_82193AD8 |
| `__imp__XMAReleaseContext` | gta4_recomp.2.cpp:8333 | sub_82193BA8 |
| `__imp__XAudioUnregisterRenderDriverClient` | gta4_recomp.2.cpp:9893 | sub_821945C0 |
| `__imp__XAudioUnregisterRenderDriverClient` | gta4_recomp.2.cpp:10042 | sub_82194708 |
| `__imp__XAudioSubmitRenderDriverFrame` | gta4_recomp.2.cpp:9997 | sub_82194698 |
| `__imp__XAudioRegisterRenderDriverClient` | gta4_recomp.2.cpp:10068 | sub_82194708 |

## 2. Per-Call Parameter and Return Value Analysis

### `__imp__XAudioGetSpeakerConfig` (in sub_8291F508)
- **Parameters**: r3 = stack-local pointer (`addi r3, r1, 80` — output buffer on stack)
- **Return**: result stored at `r1+80`, then extracted with `rlwinm r11,r11,17,30,30 | ori r11,r11,1` to produce a speaker flags word
- **Usage**: the extracted flags are compared against a cached value at a global address (`r9 + -24116`). If different, the cached value is updated. This is a speaker config change detection — called during audio system initialization via call chain: sub_82902178 → sub_8291F508.

### `__imp__XAudioGetVoiceCategoryVolumeChangeMask` (in sub_82191228)
- **Parameters**: r3 = driver handle (loaded from struct+24, expected `0x41550000 | index`), r4 = pointer to output mask (`addi r27, r31, 140`)
- **Return**: r3 = error code; mask written to `*r4`. Mask bits are then tested one-by-one (shift `r29 << r30`, AND with mask) to decide whether to call `XAudioGetVoiceCategoryVolume` for each category (loop runs up to 2 iterations)
- **Usage**: volume polling — reads which voice categories changed volume this frame

### `__imp__XAudioGetVoiceCategoryVolume` (in sub_82191228)
- **Parameters**: r3 = category index (0 or 1), r4 = pointer to float output (`addi r28, r31, 132`)
- **Return**: float written to `*r4`; r3 = error code (ignored)

### `__imp__XMACreateContext` (in sub_82193AD8)
- **Parameters**: r3 = XMA context struct pointer (`addi r30, r31, 64`)
- **Return**: r3 = status code; on success calls `__imp__MmGetPhysicalAddress(r3=*r30)` to get the physical address, then encodes it into an XMA register-file index (shift/mask arithmetic `srawi r11,r11,6 / clrlwi r11,r11,16`)
- **Usage**: iterates over audio source array (stride 96 bytes), creates an XMA context for each entry that doesn't already have one

### `__imp__XMAReleaseContext` (in sub_82193BA8)
- **Parameters**: r3 = XMA context pointer loaded from struct+64
- **Return**: ignored; struct+64 is written back to 0 (null)
- **Usage**: teardown loop, mirror of XMACreateContext loop

### `__imp__XAudioUnregisterRenderDriverClient` (in sub_821945C0 and sub_82194708)
- **Parameters**: r3 = driver handle (loaded from struct+24; must be `0x41550000 | index`)
- **Return**: r3 = error code; checked `blt cr6 → skip`; on success, struct+24 is zeroed (or set to new value)
- **Usage** in sub_821945C0: called during a lock-protected driver swap sequence (lwarx/stwcx reservation on struct field)
- **Usage** in sub_82194708: called at start of register/unregister helper — unregisters existing client before registering new one

### `__imp__XAudioRegisterRenderDriverClient` (in sub_82194708)
- **Parameters**: r3 = stack-local two-word struct `{callback_ptr, callback_arg_ptr}` (`addi r3, r1, 80`); r4 = output driver handle pointer (`addi r30, r31, 24`)
- **Return**: r3 = error code (checked `blt → skip`); driver handle written to `*r4`
- **Setup flow**: only called if `r29 != 0` (callback non-null) and after conditional unregister. The two words at `r1+80` are: [0] = callback address (from struct+12), [1] = wrapper callback arg pointer.

### `__imp__XAudioSubmitRenderDriverFrame` (in sub_82194698)
- **Parameters**: r3 = driver handle (loaded from struct+24), r4 = sample buffer pointer (computed as `input_ptr - 8 + offsets[20,28]`)
- **Return**: r3 = error code (ignored)
- **Usage**: called once per render frame after an indirect vtable call through `*(struct[0]+32)` (likely the game's mixing callback). This is the render loop pump.

## 3. RexGlue Implementation Analysis

Source files: `src/kernel/xboxkrnl/xboxkrnl_audio.cpp`, `src/audio/audio_system.cpp`, `src/audio/sdl/sdl_audio_driver.cpp`

| API | Real work? | Notes |
|-|-|-|
| `XAudioGetSpeakerConfig` | Stub-like | Always writes `0x00010001` (stereo + surround bit). No hardware query. |
| `XAudioGetVoiceCategoryVolumeChangeMask` | Real | Calls `MaybeYield()`, writes `*out = 0`. Never reports any change. May cause game to skip volume updates perpetually if game polls this in a spin-wait expecting non-zero. |
| `XAudioGetVoiceCategoryVolume` | Stub | Always writes `1.0f` to output float. |
| `XAudioEnableDucker` | Stub | Returns success, no-op. |
| `XAudioRegisterRenderDriverClient` | Real | Calls `AudioSystem::RegisterClient` → `CreateDriver` (SDL path: opens SDL audio device, starts playback). Writes `0x41550000 | index` to driver handle. |
| `XAudioUnregisterRenderDriverClient` | Real | Calls `AudioSystem::UnregisterClient` → `DestroyDriver` (closes SDL device). Drains semaphore. |
| `XAudioSubmitRenderDriverFrame` | Real | Calls `AudioSystem::SubmitFrame` → `SDLAudioDriver::SubmitFrame` → `memcpy` into queued frame buffer. |
| `XAudioRenderDriverInitialize` | Stub (no-op) | XBOXKRNL_EXPORT_STUB — returns 0, does nothing |
| `XAudioRenderDriverLock` | Stub | No-op |
| `XAudioGetRenderDriverThread` | Stub | No-op — game may block waiting for a thread handle returned here |
| `XAudioSetProcessFrameCallback` | Stub | No-op — game's per-frame audio callback never installed |
| `XAudioSuspendRenderDriverClients` | Stub | No-op |

## 4. Critical API Deep Dives

### XAudioRegisterRenderDriverClient — callback setup
- Input buffer at `callback_ptr`: `[0]` = guest function pointer (the game's render callback), `[1]` = callback arg
- `RegisterClient` allocates a 4-byte system heap block, stores `callback_arg` there as `wrapped_callback_arg`
- Worker thread waits on `client_semaphores_[index]` (pre-filled with `audio_maxqframes=64` permits)
- When woken: calls `processor_->Execute(worker_thread_->thread_state(), client_callback, {wrapped_callback_arg})`
- **Verdict**: callback IS set up and will be called by the audio worker thread.

### XAudioSubmitRenderDriverFrame — frame processing
- `SDLAudioDriver::SubmitFrame`: copies `frame_samples_` floats from guest memory into a host-side queue
- SDL callback (`SDLCallback`) dequeues and converts: 6-channel big-endian sequential → interleaved LE (stereo or 5.1)
- After dequeue: `semaphore_->Release(1)` signals the worker thread that a frame slot is free
- **Verdict**: full real implementation, audio actually plays through SDL2.

### XAudioGetSpeakerConfig — config returned
- Returns `0x00010001` unconditionally
- Bit 17 of the result (extracted by `rlwinm r11,r11,17,30,30`) = 0 → mono/stereo flag
- `ori r11,r11,1` sets bit 0 → final value `0x1` (stereo)
- Game caches this at a global and uses it to select mixing mode

## 5. Audio Render Thread in RexGlue

Yes. `AudioSystem::Setup` spawns **one host thread** named `"Audio Worker"` (128KB stack, priority 0):

```
WorkerThreadMain():
  Initialize()   // empty in base class; SDLAudioSystem::Initialize() is also empty
  loop:
    WaitAny(client_semaphores_[0..7] + shutdown_event, timeout=500ms)
    if signaled by client semaphore[i]:
      Execute(client_callback, {wrapped_callback_arg})  // calls game's render callback
    else if timeout:
      Sleep(500ms)
```

The semaphore for each client is pre-loaded with `audio_maxqframes` (default 64) permits on `RegisterClient`. The SDL callback releases one permit per consumed frame. This means the worker fires immediately 64 times on startup, then throttles to SDL's consumption rate.

**Potential hang vector**: if SDL fails to open an audio device (or `NopAudioSystem` is active — returns `X_STATUS_NOT_IMPLEMENTED` from `CreateDriver`), `RegisterClient` returns failure. The game checks the driver handle (`0x41550000 | index`) for subsequent calls. If `XAudioSubmitRenderDriverFrame` is reached with a null/invalid driver, `assert_true(clients_[index].driver != NULL)` will abort.

## 6. SDL Audio Integration

- **Driver**: `SDLAudioDriver` (sdl_audio_driver.cpp)
- `Initialize()`: calls `SDL_InitSubSystem(SDL_INIT_AUDIO)`, opens device at 48kHz, `AUDIO_F32`, 6 channels (falls back to stereo), `channel_samples_` samples per callback
- Starts unpaused immediately (`SDL_PauseAudioDevice(id, 0)`)
- XMA decoder disabled on `__APPLE__` (`#ifndef __APPLE__`)
- On macOS: XMA decode path is **completely absent** — only XAudio render path (PCM frames via `SubmitRenderDriverFrame`) is active

## 7. Init Chain Summary

```
sub_82902178  (audio subsystem Init, called from main world init)
  → sub_8291F508  → XAudioGetSpeakerConfig    (detect speaker layout)
  → sub_82902178 loop → sub_82193AD8 → XMACreateContext  (allocate XMA contexts)
  → sub_82194708 → XAudioUnregisterRenderDriverClient (cleanup old)
                 → XAudioRegisterRenderDriverClient   (register new callback + open SDL device)

Per-frame render loop (sub_82194698):
  → vtable call (game's mix callback)
  → XAudioSubmitRenderDriverFrame

Teardown:
  sub_821945C0 → XAudioUnregisterRenderDriverClient
  sub_82193BA8 → XMAReleaseContext
```

## 8. Hang Risk Assessment

| Risk | Severity | Details |
|-|-|-|
| `XAudioGetRenderDriverThread` stub returns nothing | High | If game blocks waiting for a thread handle from this API, it will spin forever. Thread handle is never provided. |
| `XAudioSetProcessFrameCallback` stub | Medium | Game's per-frame audio pump callback never registered; game may expect it to be called at render rate. |
| `XAudioGetVoiceCategoryVolumeChangeMask` always returns mask=0 | Low | Game skips volume updates but does not block. |
| SDL device open failure on macOS | High | `RegisterClient` fails → subsequent `SubmitRenderDriverFrame` triggers assert abort. |
| Worker semaphore flood (64 permits on register) | Low | Worker calls game callback 64 times immediately; not a hang but causes audio burst on init. |
