# Bug A: Audio Render Thread vtable[17] Crash — Design Document

## Summary

The audio render thread calls `sub_821910D0` which performs an indirect call through
`vtable[17]` (offset 68) on an XAudioRenderDevice endpoint object. On Xbox 360 this
dispatches to the hardware audio endpoint. In the recomp, the GPU is not emulated and
audio is handled natively via SDL2, so the vtable pointer is either NULL or garbage,
causing a crash on the `PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32)` at address 0x821911C4.

---

## 1. Analysis of sub_821910D0 (lines 1888-2091 in gta4_recomp.2.cpp)

### Full control flow

```
sub_821910D0(r3 = XAudioRenderDevice*):
  1. Save registers, allocate 192-byte frame
  2. r28 = global audio state base (0x82B2xxxx - computed from lis -32077 + addi -31944)
  3. r30 = g_audioEndpoint = *(0x82BE53EC)   [lis -31971 = 0x82BE0000, +21484 = 0x53EC]
  4. RtlEnterCriticalSection(r28 + 4)

  5. Check r30->field_304 (endpoint state):
     - If field_304 == 0: goto RENDER_PATH (loc_82191188)
     - If field_304 != 0: goto WAIT_PATH (KeSetEvent + KeWaitForMultipleObjects)

  WAIT_PATH (field_304 != 0):
     6. KeSetEvent(signal render-ready event)
     7. KeWaitForMultipleObjects(2 objects, timeout=none, WaitAll=1, alertable=3)
     8. If wait result == 1: set r31 = 1 (shutdown flag)
     9. Skip to loc_8219119C

  RENDER_PATH (field_304 == 0):
     10. sub_8218FFB0(r30)         — "prepare frame" (raises IRQL, acquires spinlock,
                                     iterates voice sub-buffers via sub_82190120)
     11. sub_82191228(r30, 1)      — "submit frame" (calls sub_821916D8 to manage
                                     mix locks, queries XAudioGetVoiceCategoryVolumeChangeMask,
                                     reads category volumes, calls sub_82191858 + sub_82191360
                                     to process each voice, releases semaphore if pending > 1)

  COMMON TAIL (loc_8219119C):
     12. If r31 (shutdown) == 0:
         *** THE CRASH POINT ***
         r3 = r30->field_64         (endpoint interface pointer)
         r11 = *(r3)                (vtable base)
         r11 = *(r11 + 68)          (vtable slot 17 = offset 68 / 4 = index 17)
         bctrl r11                  (indirect call)
         If return value >= 0: atomic increment frame counter at (r31 - 28)

     13. Store 0 to r30->field_300 (clear "frames pending" field)
     14. Buffer swap loop: swaps 7 dwords between two arrays (double-buffering)
     15. RtlLeaveCriticalSection(r28 + 4)
     16. Return 0
```

### What is vtable slot 17?

The object at `r30->field_64` is the **IXAudioRenderDriver endpoint interface** — the Xbox
360 hardware audio render driver. Slot 17 (offset 68) in the XAudio render driver vtable is
**`GetCurrentFrame`** or **`PresentFrame`** — it signals the hardware to consume the mixed
audio buffer and returns an HRESULT.

On Xbox 360 hardware, this would be `XAudioRenderDriverEndpoint::Present()` which:
- Tells the audio DMA controller to read from the current buffer
- Returns S_OK (0) on success, negative HRESULT on failure
- The return value is checked: `if (result >= 0)` → increment frame counter

This is the audio equivalent of GPU Present/SwapBuffers.

---

## 2. Audio Worker Thread Entry Points

### sub_821909D0 — Primary audio render thread (lines 887-1004)

```
sub_821909D0:
  1. Store TLS thread index into endpoint object (processor affinity tracking)
  2. Set up pointers to three kernel events/semaphores:
     - r28 = event at (0x82BE0000 + 21196)  — "frame complete" event
     - r27 = semaphore at (0x82BE0000 + 21212) — "frames pending" semaphore
     - r29 = event at (0x82BE0000 + 21232)  — "render request" event

  MAIN LOOP (loc_82190A10):
  3. KeWaitForSingleObject(render-request event, timeout=infinite)
  4. Load endpoint->field_300 (pending frame count)
     - If 0: call sub_8218FFB0 + sub_82191228 (prepare+submit)
     - If != 0: check count, KeReleaseSemaphore if count > 1
  5. KeSetEvent(frame-complete event)
  6. Check shutdown flag (r31): if 0, loop back to step 3
  7. Return 0
```

This is the **main audio worker thread**. It waits for a render request event, then either
renders a frame directly (via sub_8218FFB0 + sub_82191228) or signals the semaphore.
It does NOT call sub_821910D0 directly.

### sub_82190A98 — Secondary audio sync thread (lines 1006-1104)

```
sub_82190A98:
  1. Store TLS thread index into endpoint object
  2. KeWaitForSingleObject(semaphore at 0x82BE0000 + 21212)
  3. Loop: while endpoint->field_300 != 0:
     - sub_82191228(endpoint, r4=0)  — submit frame (without lock management)
     - KeWaitForSingleObject(semaphore) again
  4. Return 0
```

This is a **secondary sync thread** that drains queued frames. It calls sub_82191228 but
NOT sub_821910D0 directly.

**Key finding**: sub_821910D0 is NOT called from either worker thread directly.
It appears to be called from the **game's main render loop** via sub_8219A2B8.

---

## 3. Side Effects in sub_821910D0 That Must Be Preserved

If we hook sub_821910D0 to skip the vtable call, we must preserve:

| Side Effect | Location | Purpose |
|---|---|---|
| RtlEnterCriticalSection | Line 1918 | Protects audio state from concurrent access |
| Store TLS timestamp to field_300 | Line 1926 | Tracks frame timing |
| sub_8218FFB0 (prepare frame) | Line 1995 | Raises IRQL, acquires spinlock, iterates voice sub-buffers |
| sub_82191228 (submit frame) | Line 2002 | Manages mix locks, reads volume changes, processes voices, releases semaphore |
| Atomic frame counter increment | Lines 2033-2058 | Frame counter used for sync (lwarx/stwcx loop) |
| Clear field_300 | Line 2063 | Reset pending-frames state |
| Buffer swap loop | Lines 2064-2078 | Double-buffer swap (7 dwords) |
| RtlLeaveCriticalSection | Line 2083 | Release lock |

**The vtable call itself** (lines 2014-2024) does only one thing: tells hardware to consume
the buffer. Its return value gates the frame counter increment. All other side effects are
independent of the vtable call.

---

## 4. Alternative: No-op sub_8219A2B8 Entirely?

`sub_8219A2B8` (line 23785) is a thin wrapper:
```cpp
sub_8219A2B8:
  r3 = *(0x82BE53EC)    // load global endpoint pointer
  tail-call sub_821910D0(r3)
```

**Would no-oping it break audio?**

YES — it would break audio. sub_821910D0 does far more than just the vtable call:
- It runs sub_8218FFB0 (voice preparation with IRQL raise + spinlock)
- It runs sub_82191228 (voice submission, volume change mask processing, semaphore release)
- It performs the double-buffer swap
- It increments the frame counter (used for audio/video sync)

Without these, the audio worker threads (sub_821909D0, sub_82190A98) would stall waiting
on events that never get signaled, and XMA voice data would never be mixed.

**Verdict: Do NOT no-op sub_8219A2B8.**

---

## 5. Existing Audio Hooks in LibertyRecomp/apu/

### Kernel-level XAudio imports (audio.cpp)
- `XAudioRegisterRenderDriverClient` — registers SDL2 callback, initializes audio manager
- `XAudioUnregisterRenderDriverClient` — no-op stub
- `XAudioSubmitRenderDriverFrame` — forwards mixed samples to SDL2 via `XAudioSubmitFrame()`

### XMA decoder hooks (xma_decoder.cpp) — all at 0x8255xxxx
22 GUEST_FUNCTION_HOOK entries for XMAPlayback* functions (Create, Destroy, SubmitData,
ConsumeDecodedData, etc.). These handle XMA2 decoding via the native decoder.

### Audio state tracking (audio_state.cpp)
Manages playback state, volume categories, active context counting.

### What the existing system handles
The existing `XAudioSubmitRenderDriverFrame` at 0x82A757E4 already handles the actual
audio output. It is called from `sub_82194698` (line 9997) which is a DIFFERENT function
in the game's higher-level audio mixing path — this is the function that calls
`XAudioSubmitRenderDriverFrame` after mixing voices via a vtable[8] dispatch.

**Key insight**: The game has TWO audio submission paths:
1. **High-level**: sub_82194698 → vtable[8] (mixer) → XAudioSubmitRenderDriverFrame (HOOKED, works)
2. **Low-level**: sub_821910D0 → vtable[17] (endpoint Present) → hardware DMA (CRASHES)

The SDL2 system already handles path #1. Path #2 is the Xbox hardware endpoint call
that has no host equivalent.

---

## 6. Existing Hooks in the 0x8219xxxx Range

Only one:
- `0x821966D0` — `sub_821966D0_hook` in `kernel/imports.cpp` / `kernel/memory.cpp`
  (worker thread gate for init suspension; not audio-related)

**No existing hooks for sub_821910D0, sub_8219A2B8, sub_821909D0, or sub_82190A98.**

---

## 7. Recommended Minimal Hook

### Option A: Hook sub_821910D0 — skip only the vtable[17] call (RECOMMENDED)

Hook the function to execute everything EXCEPT the indirect call at the crash point.
Replace the vtable call with a synthetic "success" return (r3 = 0, i.e. S_OK), which
allows the frame counter increment to proceed.

```cpp
// In apu/ or kernel/ — hook sub_821910D0
GUEST_FUNCTION_HOOK(sub_821910D0, AudioEndpointPresent_Hook);

void AudioEndpointPresent_Hook(PPCContext& ctx, uint8_t* base) {
    // Let the original function run, but patch the endpoint vtable first.
    //
    // The crash is: *(*(r30->field_64) + 68) is a garbage/NULL function pointer.
    // We need to either:
    // (a) Install a stub function at vtable slot 17, or
    // (b) Rewrite sub_821910D0 to skip the indirect call.
    //
    // Approach (b) is fragile. Approach (a) is cleaner:
    // Before calling the original, ensure the endpoint's vtable[17] points to a
    // stub that returns 0 (S_OK).

    // ... see detailed implementation below
    __imp__sub_821910D0(ctx, base);
}
```

### Option B: Stub vtable slot 17 at registration time (CLEANEST)

In `XAudioRegisterRenderDriverClient` (audio.cpp), after the endpoint object is created,
patch the vtable so slot 17 points to a PPC stub function that returns 0.

This is the cleanest approach because:
- No need to hook sub_821910D0 at all
- The original code runs unmodified
- All side effects (frame counter, buffer swap, critical section) work naturally
- The stub just needs to return HRESULT S_OK (0) in r3

```cpp
// Stub: replaces XAudioRenderDriverEndpoint::Present()
// Returns S_OK (0) — audio output is handled by SDL2 via XAudioSubmitRenderDriverFrame
static void AudioEndpointPresent_Stub(PPCContext& ctx, uint8_t* base) {
    ctx.r3.s64 = 0; // S_OK
}
```

The endpoint object is at `*(0x82BE53EC)`. Its vtable pointer is at `field_64`.
The vtable slot 17 is at offset 68 from the vtable base.

Patch sequence (in XAudioRegisterRenderDriverClient or a post-init hook):
1. Read endpoint ptr: `endpoint = PPC_LOAD_U32(0x82BE53EC)`
2. Read interface ptr: `iface = PPC_LOAD_U32(endpoint + 64)`
3. Read vtable base: `vtable = PPC_LOAD_U32(iface)`
4. Register `AudioEndpointPresent_Stub` as a PPC function
5. Write stub address to `vtable + 68`

### Option C: Hook sub_821910D0 to skip vtable call inline (SIMPLEST)

Just hook sub_821910D0 and reimplement the function, replacing the vtable call with
`ctx.r3.s64 = 0`:

```cpp
// Simplest hook: replaces sub_821910D0 entirely
// Reproduces all side effects except the vtable[17] indirect call
void AudioRenderPresent_Hook(PPCContext& ctx, uint8_t* base) {
    uint32_t endpoint = PPC_LOAD_U32(0x82BE53EC);  // g_audioEndpoint

    // Enter critical section
    uint32_t critSec = /* r28 + 4 from original */;
    ctx.r3.u32 = critSec;
    __imp__RtlEnterCriticalSection(ctx, base);

    // Store TLS timestamp
    uint32_t tls_ts = PPC_LOAD_U32(ctx.r13.u32 + 256);
    PPC_STORE_U32(endpoint + 300, tls_ts);

    uint32_t field304 = PPC_LOAD_U32(endpoint + 304);
    if (field304 == 0) {
        // RENDER PATH: prepare + submit
        ctx.r3.u32 = endpoint;
        sub_8218FFB0(ctx, base);          // prepare frame
        ctx.r3.u32 = endpoint;
        ctx.r4.s64 = 1;
        sub_82191228(ctx, base);          // submit frame
    } else {
        // WAIT PATH: signal + wait
        // ... (KeSetEvent + KeWaitForMultipleObjects)
    }

    // Skip vtable[17] call — just pretend it returned S_OK
    // Atomic increment frame counter
    // ... (lwarx/stwcx loop on frame counter)

    // Clear field_300, do buffer swap, leave critical section
    PPC_STORE_U32(endpoint + 300, 0);
    // ... buffer swap loop ...
    ctx.r3.u32 = critSec;
    __imp__RtlLeaveCriticalSection(ctx, base);
    ctx.r3.s64 = 0;
}
```

This is fragile because it duplicates complex logic. Better to use Option B.

---

## 8. Final Recommendation

**Use Option B: Stub vtable slot 17 at endpoint registration time.**

Rationale:
1. Zero changes to the game's audio render path — all side effects preserved automatically
2. The stub is trivial: just return 0 (S_OK)
3. Can be installed in `XAudioRegisterRenderDriverClient` which already runs at the right time
4. The SDL2 audio path (`XAudioSubmitRenderDriverFrame`) continues to handle actual output
5. The frame counter increment proceeds normally, maintaining audio/video sync

### Implementation location
`/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/apu/audio.cpp`, inside or after
`XAudioRegisterRenderDriverClient()`.

### Fallback
If the endpoint vtable is not yet populated at registration time (race condition),
use Option A: a `GUEST_FUNCTION_HOOK` on `sub_821910D0` that patches the vtable on
first entry (lazy init), then tail-calls the original.

### Global addresses referenced
- `0x82BE53EC` — g_audioEndpoint pointer (lis -31971 = 0x82BE0000, + 21484 = 0x53EC)
- `endpoint + 64` — IXAudioRenderDriver interface pointer
- `endpoint + 300` — TLS timestamp / pending frames state
- `endpoint + 304` — frame state flag (0 = render, nonzero = wait)
- `*(iface) + 68` — vtable slot 17 (Present/GetCurrentFrame)
- Frame counter base: 0x82BE0000 + 21516 - 28 = 0x82BE53FC - 28 = 0x82BE53E0
