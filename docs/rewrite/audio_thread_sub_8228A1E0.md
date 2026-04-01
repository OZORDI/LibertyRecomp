# Audio Thread Initialization: sub_8228A1E0

## Overview

sub_8228A1E0 is GTA IV's RAGE audio streaming thread initialization function. Called at engine init phase 21 (from sub_82478AF8 at 0x82478E34), it sets up the RAGE audWaveSlot/audWaveFile streaming system -- NOT the XAudio render driver, which is initialized earlier via XAudioRegisterRenderDriverClient.

sub_8228A1E0 does NOT create a thread itself. It configures data structures and registers channel callbacks that are invoked by the audio render thread (created separately via XAudioRegisterRenderDriverClient).

## Call Sequence

```
sub_82478AF8 (engine init)
  |
  +-- Phase 7: sub_826BDC18 (audio device enumeration)
  +-- Phase 7: vtable call via *(0x831CC904) vtable[1] (device init)
  +-- Phase 8: sub_82953AD0(f1=volume_float) (master volume)
  +-- Phase 8: sub_8228A1E0() <--- THIS FUNCTION
  +-- Phase 9: sub_825FD6B8 (RPF streaming start)
  +-- Phase 9: sub_82955BE0 (XAudio streaming buffer alloc -- HANG POINT)
```

## sub_8228A1E0 Internal Flow

### Step 1: Set Audio Init Flag
```
PPC_STORE_U8(0x82A98C24, 1)   // g_bAudioStreamingInit = true
```

### Step 2: sub_82289A38(r3=330, r4=0x82009C60, r5=3, f1=float_from_0x8201966C)

Audio wave config initialization:
1. Calls `sub_821B3510(28)` -- operator new(28 bytes) -- allocates audWaveSlot
2. If non-NULL, calls `sub_82507368(obj, 330, 0x82009C60, 64)` -- audWaveSlot constructor
   - 330 = max wave file count
   - 0x82009C60 = name string pointer (e.g. "waveSlot" or config name)
   - 64 = pool size
   - Result stored at **0x82C6C3F4** (g_pWaveSlot)
3. Calls `sub_821B3510(330)` -- operator new for wave file array
   - Result stored at **0x82C6C408**
4. Calls `sub_829FF840(r3, r4=0, r5=330)` -- memset(array, 0, 330)
5. Calls `sub_8244C388()` -- queries audio device (XAudioGetSpeakerConfig-like)
   - If returns non-NULL, calls `sub_821FBB68(result, &doppler_params, 3)` -- 3D audio init
   - Result stored at **0x82C6C3F8** (g_pAudioEnv)
6. Stores float f31 at **0x82A98C20** (master volume level)
7. Clears byte at **0x82C6C3FC** (streaming error flag)

### Step 3: sub_821D0488() -- returns 0x82D515B8

Simple getter: returns pointer to RAGE profiling/stats struct. No side effects.

### Step 4: sub_8250D080(r3=r25, many args) -- audStreamConfig registration

Registers an audio stream configuration entry in a 100-byte-stride array at the profiling object (base from sub_821D0488 return). Structure:

| Offset | Size | Content |
|-|-|-|
| +0 | 4 | flags/type |
| +4 | 4 | renderer ptr (0x826041E8) |
| +8-12 | 8 | aux ptrs |
| +16-36 | 20 | callback table (from stack) |
| +40 | 4 | extra ptr |
| +44 | 4 | secondary ptr |
| +48 | 4 | tertiary ptr |
| +52-83 | 32 | name string copy |
| +84-91 | 8 | secondary name copy |
| +92 | 1 | bool A (0) |
| +93 | 1 | bool B (1) |
| +96 | 4 | buffer size (32) |

Count stored at base+2500, incremented after each add.
Result (index) stored at **0x82A98C1C** (g_streamConfigIndex).

### Step 5: sub_82662900 (called twice) -- Channel Registration

Object at **0x830B89F0** is the RAGE audChannelDispatcher. Entry structure:

| Offset | Size | Field |
|-|-|-|
| +0 | 4 | callback function ptr |
| +4 | 4 | interrupt handler ptr |
| +8 | 4 | renderer ptr |
| +12 | 48 | name string (null-terminated copy) |
| +56 | 4 | step size |

Count at obj+300. Each call appends one 60-byte entry.

**Channel 1 (wave file streaming):**

| Field | Value |
|-|-|
| name | from 0x82009C60 |
| callback | 0x82289030 (audWaveSlot::Update) |
| interrupt | 0x82288960 |
| renderer | 0x826041E8 |
| step | 8 |

**Channel 2 (secondary stream):**

| Field | Value |
|-|-|
| name | from 0x82009C40 |
| callback | 0x82288AD8 (audWaveFile::Update) |
| interrupt | 0x822889F0 |
| renderer | 0x826041E8 |
| step | 48 |

## Audio Thread Lifecycle

sub_8228A1E0 does NOT create the audio thread. The full lifecycle:

### Thread Creation (earlier in init)

The audio render thread is created by **XAudioRegisterRenderDriverClient** (kernel import at 0x82A75804). Two implementations exist:

1. **LibertyRecomp native** (`apu/driver/sdl2_driver.cpp`): Creates `std::thread(AudioThread)` which polls SDL_GetQueuedAudioSize and invokes the guest callback.

2. **RexGlue** (`audio_system.cpp`): Creates `XHostThread("Audio Worker")` via `AudioSystem::Setup()`. Worker runs `WorkerThreadMain()`.

### Thread Main Loop

**LibertyRecomp path** (sdl2_driver.cpp):
```
while (!g_audioThreadShouldExit) {
    if (SDL_GetQueuedAudioSize() / frameSize <= MAX_LATENCY) {
        ctx.r3.u32 = g_clientCallbackParam;
        g_clientCallback(ctx.ppcContext, g_memory.base);  // invoke guest
        g_audioEventCallback(ctx.ppcContext, g_memory.base); // signal event
    }
    sleep(frame_interval);
}
```

**RexGlue path** (audio_system.cpp):
```
while (worker_running_) {
    result = WaitAny(wait_handles_, count, 500ms);
    if (shutdown) continue;
    if (success) {
        Execute(worker_thread_state, client_callback, {callback_arg});
    }
    if (!pumped) Sleep(500ms);
}
```

### What the Guest Callback Does

The registered guest callback (from XAudioRegisterRenderDriverClient's callback_ptr[0]) calls:
- sub_821910D0 (XAudio render thread pump): enters critical section at 0x82B2834C, processes audio buffers, calls vtable[17] on XAudioRenderDriverEndpoint (DMA trigger -- stubbed as 0x000F4000 in recomp, silently skipped by MISSING-FUNC handler)
- XAudioSubmitRenderDriverFrame: submits 6-channel * 256-sample float buffer to SDL

## Audio Kernel APIs

### Fully Implemented (RexGlue XBOXKRNL layer)

| Import | Address | Implementation |
|-|-|-|
| XAudioGetSpeakerConfig | 0x82A74DB4 | Returns 0x00010001 (stereo) |
| XAudioGetVoiceCategoryVolumeChangeMask | 0x82A75774 | Returns 0 (no changes) |
| XAudioGetVoiceCategoryVolume | 0x82A75794 | Returns 1.0f |
| XAudioRegisterRenderDriverClient | 0x82A75804 | Registers callback, creates SDL audio driver |
| XAudioUnregisterRenderDriverClient | 0x82A757F4 | Unregisters client, destroys driver |
| XAudioSubmitRenderDriverFrame | 0x82A757E4 | Submits 6ch*256 float frame to SDL |

### macOS Stubs (XMA decoder not available)

| Import | Address | Implementation |
|-|-|-|
| XMACreateContext | 0x82A757C4 | Returns STATUS_NOT_IMPLEMENTED (0x8007000E) |
| XMAReleaseContext | 0x82A757B4 | No-op, returns 0 |

### Stubbed (RexGlue XBOXKRNL)

All remaining XAudio* functions are XBOXKRNL_EXPORT_STUB (return 0):
XAudioRenderDriverInitialize, XAudioRenderDriverLock, XAudioSetVoiceCategoryVolume, XAudioBeginDigitalBypassMode, XAudioEndDigitalBypassMode, XAudioSubmitDigitalPacket, XAudioQueryDriverPerformance, XAudioGetRenderDriverThread, XAudioSetSpeakerConfig, XAudioOverrideSpeakerConfig, XAudioSuspendRenderDriverClients, XAudioRegisterRenderDriverMECClient, XAudioUnregisterRenderDriverMECClient, XAudioCaptureRenderDriverFrame, XAudioGetRenderDriverTic, XAudioSetDuckerLevel, XAudioIsDuckerEnabled, XAudioGetDuckerLevel, XAudioGetDuckerThreshold, XAudioSetDuckerThreshold, XAudioGetDuckerAttackTime, XAudioSetDuckerAttackTime, XAudioGetDuckerReleaseTime, XAudioSetDuckerReleaseTime, XAudioGetDuckerHoldTime, XAudioSetDuckerHoldTime, XAudioGetUnderrunCount, XAudioSetProcessFrameCallback.

## Audio Device Manager (0x831CC904)

Global pointer `g_pDeviceManager`. Object is set up by sub_826BDC18 (audio device enumeration, Phase 7). Three vtable calls are made during engine init:

| PC | VTable Offset | Arg | Purpose |
|-|-|-|-|
| 0x82478DFC | vtable[1] (+4) | r4=0 | Device initialization |
| 0x824793FC | vtable[4] (+16) | r4=0x8201C018 (string) | Register audio device name 1 |
| 0x82479420 | vtable[4] (+16) | r4=0x8201C00C (string) | Register audio device name 2 |

The vtable pointer is read-only from the perspective of sub_8228A1E0. The device manager is already initialized before the streaming system is configured.

## Thread Synchronization

| Mechanism | Address | Purpose |
|-|-|-|
| Event | 0x83137B80 | Audio worker event -- signaled each frame by main thread hook AND by audio thread |
| Semaphore | 0x83130008 | Audio work queue -- signaled each frame (count 6) |
| Event | 0x83130044 | Signaled after sub_82168C08 (audio manager init) |
| Critical Section | 0x82B2834C | Protects audio buffer swap in sub_821910D0 |
| Frame Counter | 0x831D53F0 | Atomic increment per audio frame |
| Double Buffer | 0x831D53F0-0x831D540C | 7 u32 double-buffer for audio device state |

## Why Audio Init Can Block the Main Thread

### The HANG Point: sub_82955BE0

Called immediately AFTER sub_8228A1E0, sub_82955BE0 allocates XAudio streaming buffers:
- Loops allocating 1488-byte + 16384-byte structures (audStreamBuffer objects)
- Calls sub_829764D8 first (streaming resource init)
- Allocates 24-byte descriptor objects with 65536-byte data buffers
- Each iteration calls sub_821B3510 (operator new) multiple times

This function is labeled `"82478AF8 phase21 xaudio-stream-HANG"` in INIT_PROBE because the allocations can trigger the ALLOC FALLBACK storm (TLS[1676] heap not set up, falling through to host page allocator).

### Blocking Conditions

1. **sub_82955BE0 allocation storm**: ~6 iterations allocating 17872 bytes each through fallback allocator
2. **Audio worker thread sync**: sub_82169B00 (audio thread sync) is hooked to return 0 immediately, preventing deadlock on Xbox worker model
3. **sub_82169400** (audio worker thread) is also hooked to return 0 -- SDL handles audio natively
4. **sub_82168C08** (audio manager init) is hooked to signal events at 0x83130044, 0x83137B80, and semaphore at 0x83130008 to unblock waiting workers

## Registered Callbacks

sub_8228A1E0 registers two channel callbacks in the audChannelDispatcher at 0x830B89F0:

### Channel 1: sub_82289030 (audWaveSlot::Update)
- Reads wave file index from obj[0], looks up in wave table
- Calls sub_82288C58 to get current slot state
- Checks against profiling config at 0x82A98C1C
- Looks up entry in array at 0x82D515B8 + 30272 + (idx*5*32)
- Checks bytes at +158 and +159 for validity
- Writes result to array at 0x82C6C410 + (obj[4] * 4)

### Channel 2: sub_82288AD8 (audWaveFile::Update)
- More complex -- handles actual wave file streaming/decode
- Processes audio data buffers through the XMA decode pipeline
- Updates playback position and buffer state

## Globals Written by sub_8228A1E0

| Address | Type | Value | Name |
|-|-|-|-|
| 0x82A98C24 | u8 | 1 | g_bAudioStreamingInit |
| 0x82A98C20 | f32 | float from 0x8201966C | g_fMasterVolume |
| 0x82A98C1C | u32 | stream config index | g_streamConfigIndex |
| 0x82C6C3F4 | u32 | ptr | g_pWaveSlot (28-byte object) |
| 0x82C6C408 | u32 | ptr | g_pWaveFileArray (330 entries) |
| 0x82C6C3F8 | u32 | ptr or NULL | g_pAudioEnv (3D audio) |
| 0x82C6C3FC | u8 | 0 | g_bStreamingError |
| 0x830B89F0 +300 | u32 | incremented by 2 | audChannelDispatcher count |

## Architecture Summary

```
+-------------------+     +-------------------+     +------------------+
| Main Thread       |     | SDL Audio Thread  |     | Audio Worker     |
| (sub_82478AF8)    |     | (sdl2_driver.cpp) |     | (sub_82169400)   |
|                   |     |                   |     |  [HOOKED: nop]   |
| 1. Device enum    |     | Polls SDL queue   |     |                  |
| 2. Device init    |     | Invokes callback  |---->| Event 0x83137B80 |
| 3. Master volume  |     | via PPC context   |     | Sem   0x83130008 |
| 4. sub_8228A1E0   |     |                   |     |                  |
|    - wave config  |     | Calls:            |     |                  |
|    - channel reg  |     |  XAudioSubmit...  |     |                  |
| 5. Stream buffers |     |  sub_82172BE8     |     |                  |
+-------------------+     +-------------------+     +------------------+
         |                         |
         v                         v
+-------------------+     +-------------------+
| audWaveSlot       |     | SDL2 Audio Device |
| (0x82C6C3F4)      |     | 48kHz, 6ch, f32   |
| 330 wave files    |     | 256 samples/frame |
+-------------------+     +-------------------+
```

## Key Findings

1. **sub_8228A1E0 is NOT a thread creator** -- it's a configuration function that sets up wave streaming data structures and registers callbacks in the audChannelDispatcher.

2. **The audio thread is created earlier** by XAudioRegisterRenderDriverClient (Phase 7), not by this function.

3. **Two parallel audio systems exist**: LibertyRecomp's native SDL2 path (apu/driver/sdl2_driver.cpp) and RexGlue's AudioSystem (conditionally compiled). The native path is active.

4. **XMA decoding is disabled on macOS** -- XMACreateContext returns STATUS_NOT_IMPLEMENTED. Audio works through the XAudio render driver path (PCM float mixing) only.

5. **The HANG point** is sub_82955BE0 (called AFTER sub_8228A1E0), not sub_8228A1E0 itself. The hang is caused by allocation pressure through the fallback allocator.

6. **Three hooks prevent audio deadlocks**: sub_82169B00 (sync nop), sub_82169400 (worker nop), sub_82168C08 (event signaling).

7. **The audio device manager vtable at 0x831CC904** is initialized before sub_8228A1E0 runs. Its vtable calls are in Phase 7, not Phase 8.
