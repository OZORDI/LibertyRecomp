# Audio Render Thread Path — Full Trace

**Generated**: 2026-03-28
**Source**: `default (1).xex.c` (Hex-Rays pseudo-code), `gta4_recomp.55.cpp`, `gta4_recomp.70.cpp`, RexGlue audio system

---

## 1. sub_82849A50 — Thread Creation Wrapper (r5 passed to ExCreateThread)

sub_82849A50 is the **thread pool alloc + ExCreateThread** wrapper called by sub_8285D500 (the per-thread slot initializer, called from sub_8285D610). It is NOT the thread entry point — it is the **creator** of threads.

Recomp source: `gta4_recomp.55.cpp` lines 8749–8916.

```
Arguments (r3..r9):
  r3 = thread_name_ptr
  r4 = thread_slot ptr
  r5 = stack_size (clamped to min 16384 at line 8779)
  r6 = thread name index (affinity/name)
  r7 = thread_slot (context passed to entry point)
  r8 = resume_flag (1 = resume immediately after create)
  r9 = priority
```

What it does:
1. Clamps `r5` to 16384 if smaller.
2. Acquires a free thread slot from pool at `0x83192C44` via `sub_821D5F70` + `sub_828499E8`.
3. Returns -1 (failure) if pool is exhausted.
4. Reads TLS allocator context from `r13+1676`, stores at `thread_slot+8`.
5. Stores `thread_name_ptr` at `thread_slot+0`, `thread_slot_ptr` at `thread_slot+4`.
6. Calls `sub_82A13110` → `sub_82A1A4B8` → `__imp__ExCreateThread` with:
   - `stack_size = 16384`
   - `start_routine` = a generic streaming dispatch function (encoded in sub_82A1A4B8)
   - `context = thread_slot`
   - `flags = r7` (bit 0 = CREATE_SUSPENDED)
7. On success: calls `sub_82A11478` (set name), `sub_82A114F8` (set affinity=1), `sub_82A11580` (set priority).
8. If `resume_flag != 0`, calls `sub_82A13120` → `NtResumeThread`.

**sub_82849A50 is NOT the thread entry point.** The actual entry point is the generic streaming dispatcher embedded inside `sub_82A1A4B8` (the thin ExCreateThread wrapper at `gta4_recomp.69.cpp`).

---

## 2. sub_82849A50 in sub_82849A50 Context — What Threads Does sub_8285D610 Create?

sub_8285D610 (streaming channel initializer, `gta4_recomp.56.cpp` lines 7988–8267) creates **streaming worker threads**. The thread entry point passed as r5 to ExCreateThread through sub_82849A50 is the **generic streaming dispatcher** — a function that calls `KeWaitForSingleObject` on the channel semaphore (at `channel+1068`, created via `NtCreateSemaphore(max=32767)`) and then dispatches IO work items.

The ExCreateThread call in sub_82169578 (the XAudio init function) creates **different threads**: `sub_82169400` and `sub_821694C8`. These are the XAudio render driver client threads, not streaming threads.

---

## 3. Thread Entry Functions: sub_82169400 and sub_821694C8

These are the two XAudio render driver thread entry points created by **sub_82169578** (`//----- (82169578)`, pseudo-code line 1197327). sub_82169578 is the XAudio render driver client initialization function.

### sub_82169400 — XAudio Primary Render Thread

```c
// Pseudo-code (lines 1197278–1197301)
int sub_82169400()
{
  // Store this thread's handle in the driver's thread-handle array
  *ptr(4 * (TLS_cpu_id + 83) + dword_83137CA8) = TLS_thread_handle;
  do {
    KeWaitForSingleObject(&byte_83137B80, UserRequest, UserMode, 0, 0);
    if (*ptr(dword_83137CA8 + 300) == 0) {
      // No voices registered — release semaphore for worker and set event
      if (*ptr(dword_83137CA8 + 304) != 1)
        KeReleaseSemaphore(&unk_83137B6C, 1);
    } else {
      sub_821689E0(dword_83137CA8);  // voice graph update
      sub_82169C58(dword_83137CA8, 1u);  // render frame + submit
    }
    KeSetEvent(&byte_83137B5C, 1u, 0);
  } while (!done);
}
```

**Semaphore waited on**: `byte_83137B80` — a KEVENT (not semaphore) that is signaled once per audio frame by `sub_82169B00` (called via `sub_82172BE8` at the end of init, then every frame).

### sub_821694C8 — XAudio Secondary (Worker) Render Thread

```c
// Pseudo-code (lines 1197310–1197323)
int sub_821694C8()
{
  *ptr(4 * (TLS_cpu_id + 83) + dword_83137CA8) = TLS_thread_handle;
  KeWaitForSingleObject(&unk_83137B6C, UserRequest, UserMode, 0, 0);
  for (; *ptr(dword_83137CA8 + 300); ) {
    sub_82169C58(dword_83137CA8, 0);  // render frame (no category volume update)
    KeWaitForSingleObject(&unk_83137B6C, UserRequest, UserMode, 0, 0);
  }
}
```

**Semaphore waited on**: `unk_83137B6C` — a KSEMAPHORE (initialized with `KeInitializeSemaphore(..., 0, 6)` at pseudo-code line 1197442). Released by sub_82169400 when it has more than one voice thread to coordinate.

---

## 4. sub_82169C58 — The Work Function

Both threads call `sub_82169C58(a1=driver_state, a2=is_primary)` (pseudo-code lines 1197688–1197744).

What it does when `a2 != 0` (primary thread):
1. Calls `sub_8216A108(a1, 0)` — locks driver for processing.
2. Calls `XAudioGetVoiceCategoryVolumeChangeMask` + `XAudioGetVoiceCategoryVolume` — queries volume state.
3. If `thread_count > 1`, releases semaphore `unk_83137B6C` by `thread_count - 1` counts (wakes worker threads).
4. Calls `sub_8216A288(a1, a1+80)` — dequeues and processes voice commands from the ring buffer (spinlock-protected, raises IRQL to DPC level).
5. Calls `sub_82169D90(a1, a1+356)` — dispatches voice frame to hardware output slot.
6. Iterates over all registered voice streams (`a1+128` count, stride 44), calling `sub_8216A288` + `sub_82169D90` for each.
7. Calls `sub_8216A108(a1, 1)` — unlocks driver.

`sub_8216A288` dequeues work items from the ring buffer using spinlock (`KeAcquireSpinLockAtRaisedIrql`) and calls the item's vtable dispatch at `vtable+68`.

`sub_82169D90` waits for CPU affinity bit to be set (all assigned hardware threads to have processed), then clears the counter.

The render loop terminates in `sub_8216DEA0` which calls **`XAudioSubmitRenderDriverFrame(driver_handle, samples_ptr)`** — this is the only call site.

---

## 5. sub_82A34FD8 — What It Actually Is

`sub_82A34FD8` is **not** an audio thread start function. It is a one-instruction stub:

```cpp
// gta4_recomp.70.cpp line 46579
PPC_FUNC_IMPL(__imp__sub_82A34FD8) {
    // b 0x82a755b4
    __imp__NetDll_inet_addr(ctx, base);
    return;
}
```

It is a thin forwarder to `NetDll_inet_addr` (network library). It is in the XAM/NET DLL stub region (0x82A34xxx). If a prior analysis referenced this address as "audio thread start function," that was a misidentification.

---

## 6. XAudioSubmitRenderDriverFrame — Call Site

Single call site: **`sub_8216DEA0`** (pseudo-code line 1201327, address `0x8216DEA0`).

```c
int sub_8216DEA0(int* a1, int a2, int a3, int a4, int a5)
{
  // Mix/process voice buffers into output frame
  (vtable[a1][8])(a1, &output_frame, a3, a4, a5);
  // Submit the completed frame to the audio backend
  return XAudioSubmitRenderDriverFrame(a1[6], output_frame_ptr);
}
```

`a1[6]` is the driver handle (the `0x41550000 | index` token registered via `XAudioRegisterRenderDriverClient`). This function is called from inside the voice processing vtable chain that `sub_82169C58` drives.

---

## 7. RexGlue Audio Render Driver

### XAudioSubmitRenderDriverFrame → AudioSystem::SubmitFrame

`xboxkrnl_audio.cpp` line 81:
```cpp
ppc_u32_result_t XAudioSubmitRenderDriverFrame_entry(ppc_pvoid_t driver_ptr, ppc_pvoid_t samples_ptr) {
    audio_system->SubmitFrame(driver_ptr & 0x0000FFFF, samples_ptr.guest_address());
}
```

`AudioSystem::SubmitFrame` (`audio_system.cpp` line 229) calls `SDLAudioDriver::SubmitFrame`, which copies the PCM frame into `frames_queued_`.

### SDL Audio Callback Thread (OS-driven, not PPC)

`SDLAudioDriver::SDLCallback` is called by SDL's audio thread (a native OS thread, not a guest PPC thread) whenever the hardware buffer needs refill. It:
1. Dequeues one frame from `frames_queued_`.
2. Converts from Xbox 360 planar 6-channel big-endian float to interleaved LE float (stereo or 5.1).
3. Calls `semaphore_->Release(1, nullptr)` — releases `client_semaphores_[index]`.

### WorkerThreadMain — The Callback Loop

`AudioSystem::WorkerThreadMain` (`audio_system.cpp` line 95) runs on a host `XHostThread`:
1. Waits on `client_semaphores_[i]` (up to 64 queued frames, capped by `audio_maxqframes`).
2. When signaled (SDL consumed a frame), calls `processor_->Execute(worker_thread_->thread_state(), client_callback, args)` — this **re-enters the PPC guest** to call the registered XAudio render callback.

### XAudioRegisterRenderDriverClient → RegisterClient

`xboxkrnl_audio.cpp` line 55:
- Stores `callback` (guest function address) + `callback_arg` in `clients_[index]`.
- Calls `AudioSystem::RegisterClient` which calls `SDLAudioSystem::CreateDriver` → `SDLAudioDriver::Initialize` → `SDL_OpenAudioDevice` + `SDL_PauseAudioDevice(0)`.
- Pre-releases `client_semaphores_[index]` by `audio_maxqframes` (64) counts so the first 64 frames can be submitted before SDL has played any.

---

## 8. Full Audio Render Thread Path

```
[Guest PPC - sub_82169B00 called each frame]
  → KeSetEvent(&byte_83137B80)            # wake primary render thread

[Guest PPC - sub_82169400 (XAudio primary render thread)]
  → KeWaitForSingleObject(&byte_83137B80)  # wait for frame trigger
  → sub_82169C58(driver, 1)               # process voices
    → XAudioGetVoiceCategoryVolumeChangeMask
    → sub_8216A288()                        # dequeue voice commands (spinlock)
    → sub_82169D90()                        # dispatch to output slots
    → sub_8216DEA0()                        # mix + submit
      → XAudioSubmitRenderDriverFrame(handle, samples_ptr)
        [Host - xboxkrnl_audio.cpp]
        → AudioSystem::SubmitFrame()
          → SDLAudioDriver::SubmitFrame()    # memcpy into frames_queued_

[Host OS SDL audio thread - SDLAudioDriver::SDLCallback]
  → dequeue frame from frames_queued_
  → convert BE planar 6ch → LE interleaved
  → write to SDL hardware buffer
  → semaphore_->Release(1)                  # signal client_semaphores_[i]

[Host XHostThread - AudioSystem::WorkerThreadMain]
  → WaitAny(client_semaphores_[i])          # woken by SDL callback
  → processor_->Execute(client_callback)    # re-enter PPC guest render callback

[Guest PPC - registered render callback]
  → drives next voice processing cycle
```

---

## 9. Key Addresses and Globals

| Address | Description |
|-|-|
| `0x83137B80` (byte) | KEVENT: signals primary render thread per frame |
| `0x83137B5C` (byte) | KEVENT: set by primary thread when frame complete |
| `0x83137B6C` (unk) | KSEMAPHORE: coordinates primary → worker threads (init count=0, max=6) |
| `0x83137BA0` (byte) | KEVENT: used in pause/shutdown path |
| `0x83137CA8` (dword) | Driver state singleton pointer |
| `dword_83137CA8 + 300` | Pointer: current thread handle (written by each thread at startup) |
| `dword_83137CA8 + 304` | Count: number of active render threads |
| `0x8216DEA0` | sub_8216DEA0: only caller of XAudioSubmitRenderDriverFrame |
| `0x82169578` | sub_82169578: XAudio render driver client init (creates threads) |
| `0x82169400` | sub_82169400: primary render thread entry (waits on byte_83137B80) |
| `0x821694C8` | sub_821694C8: secondary render thread entry (waits on unk_83137B6C) |

---

## 10. File Locations

| File | Content |
|-|-|
| `gta4_recomp.55.cpp:8749` | sub_82849A50 (thread pool alloc + ExCreateThread) |
| `gta4_recomp.70.cpp:46577` | sub_82A34FD8 (stub → NetDll_inet_addr, unrelated to audio) |
| `default (1).xex.c:1197278` | sub_82169400 (primary render thread entry) |
| `default (1).xex.c:1197310` | sub_821694C8 (secondary render thread entry) |
| `default (1).xex.c:1197327` | sub_82169578 (XAudio render driver client init) |
| `default (1).xex.c:1197603` | sub_82169B00 (per-frame audio trigger, called from sub_82172BE8) |
| `default (1).xex.c:1197688` | sub_82169C58 (render work: voice dequeue + submit) |
| `default (1).xex.c:1201326` | sub_8216DEA0 (XAudioSubmitRenderDriverFrame caller) |
| `default (1).xex.c:1201349` | sub_8216DF10 (XAudioRegisterRenderDriverClient caller) |
| `src/kernel/xboxkrnl/xboxkrnl_audio.cpp` | Host XAudio import hooks |
| `src/audio/audio_system.cpp` | AudioSystem::WorkerThreadMain + RegisterClient + SubmitFrame |
| `src/audio/sdl/sdl_audio_driver.cpp` | SDLAudioDriver::SubmitFrame + SDLCallback |
| `src/audio/sdl/sdl_audio_system.cpp` | SDLAudioSystem::CreateDriver |

All `gta4_recomp.*` paths relative to `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/`.
All `src/` paths relative to `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/`.
