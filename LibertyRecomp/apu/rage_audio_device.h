#pragma once
// =============================================================================
// RAGE Audio Device / Endpoint — Decompiled structures
// =============================================================================
//
// Source functions:
//   sub_821910D0 — render pump (main thread)
//   sub_82191228 — voice volume update + queue drain
//   sub_8218FFB0 — buffer drain (ready/free queue swap)
//   sub_82191858 — voice queue dequeue + atomic frame counter
//   sub_82194340 — XAudio render driver callback (DPC-level)
//   sub_821909D0 — primary render thread entry
//   sub_82190A98 — secondary render thread entry
//   sub_82190B48 — device Init (constructor body)
//   sub_82194698 — endpoint Present (XAudioSubmitRenderDriverFrame)
//   sub_82194708 — endpoint connect (XAudioRegisterRenderDriverClient)
//   sub_821945C0 — endpoint disconnect (destructor)
//   sub_82212EC0 — device connection state machine step
//   sub_82190120 — renderState buffer swap (ready<->free)
//   sub_82191360 — submit info ready-flag writer
//   sub_821916D8 — per-bank callback dispatcher
//   sub_82190FB8 — thread affinity mask parser
//
// Vtables (all in .rdata, no RTTI):
//   0x820BE4C0 — audDevice (25 slots)
//   0x820BE768 — audEndpoint
//   0x820BE5CC — audRenderStateA (render state variant)
//   0x820BE5F8 — audRenderStateB (render state variant)
//   0x820BE5EC — audRenderDriverEndpointA (set in destructor)
//   0x820BE618 — audRenderDriverEndpointB (set in destructor)
//   0x820BE494 — audRenderDriverEndpoint_Shutdown
//
// BSS globals:
//   0x831D53EC — g_pAudioDevice (pointer to audDevice singleton)
//   0x82B2833C — g_audioDeviceCritSec (RTL_CRITICAL_SECTION, offset +4 from base)
//   0x831D53F0 — g_frontBuffer[7]  (atomic frame counters)
//   0x831D540C — g_backBuffer[7]   (snapshot for XAudio callback)
//   0x831D5428 — g_channelEnabled[7]
//   0x831D52CC — g_shutdownDoneEvent (KEVENT, manual-reset)
//   0x831D52DC — g_workerSemaphore (KSEMAPHORE, max 6)
//   0x831D52F0 — g_renderWakeEvent (KEVENT)
//   0x831D5310 — g_shutdownCompleteEvent (KEVENT)
//   0x831E4DB8 — g_audioSpinLock (KSPIN_LOCK + recursion state)
//   0x831EA404 — g_xaudioFrameSize (6144 = 0x1800)
//
// =============================================================================

#include <cstdint>
#include <atomic>
#include <mutex>

// Address constants for guest memory
namespace rage::aud {

// BSS addresses
constexpr uint32_t ADDR_DEVICE_PTR       = 0x831D53EC;
constexpr uint32_t ADDR_CRITSEC_BASE     = 0x82B28338; // critsec at +4 = 0x82B2833C
constexpr uint32_t ADDR_CRITSEC          = 0x82B2833C;
constexpr uint32_t ADDR_FRONT_BUFFER     = 0x831D53F0;
constexpr uint32_t ADDR_BACK_BUFFER      = 0x831D540C;
constexpr uint32_t ADDR_CHANNEL_ENABLED  = 0x831D5428;
constexpr uint32_t ADDR_SHUTDOWN_DONE    = 0x831D52CC; // KEVENT
constexpr uint32_t ADDR_WORKER_SEMA      = 0x831D52DC; // KSEMAPHORE
constexpr uint32_t ADDR_RENDER_WAKE      = 0x831D52F0; // KEVENT
constexpr uint32_t ADDR_SHUTDOWN_COMPLETE= 0x831D5310; // KEVENT
constexpr uint32_t ADDR_SPINLOCK         = 0x831E4DB8;
constexpr uint32_t ADDR_SPINLOCK_COUNT   = 0x831E4DBC;
constexpr uint32_t ADDR_SPINLOCK_OWNER   = 0x831E4DC0;
constexpr uint32_t ADDR_SPINLOCK_IRQL    = 0x831E4DC4;
constexpr uint32_t ADDR_XAUDIO_FRAME_SZ  = 0x831EA404;

// Vtable addresses
constexpr uint32_t VTBL_DEVICE           = 0x820BE4C0;
constexpr uint32_t VTBL_ENDPOINT         = 0x820BE768;
constexpr uint32_t VTBL_RENDER_STATE_A   = 0x820BE5CC;
constexpr uint32_t VTBL_RENDER_STATE_B   = 0x820BE5F8;
constexpr uint32_t VTBL_RDRV_ENDPOINT_A  = 0x820BE5EC;
constexpr uint32_t VTBL_RDRV_ENDPOINT_B  = 0x820BE618;
constexpr uint32_t VTBL_RDRV_SHUTDOWN    = 0x820BE494;

// XAudio constants
constexpr uint32_t XAUDIO_FRAME_SIZE     = 6144;  // bytes per hardware frame
constexpr uint32_t XAUDIO_SAMPLE_RATE    = 48000;
constexpr uint32_t XAUDIO_FRAME_SAMPLES  = 1536;  // 6144 / 4 (stereo S16)
constexpr uint32_t NUM_DOUBLE_BUFS       = 7;
constexpr uint32_t NUM_WORKER_THREADS_MAX= 6;
constexpr uint32_t NUM_CALLBACK_BANKS    = 2;
constexpr uint32_t CALLBACKS_PER_BANK    = 8;
constexpr uint32_t NUM_VOICE_CATEGORIES  = 2;

// =============================================================================
// Struct: audRenderState (44 bytes)
// =============================================================================
// Double-buffered linked-list node for audio voice render state.
// sub_82190120 swaps readyQueue <-> freeQueue (buffer flip).
// sub_82191858 dequeues from readyQueue, links into freeQueue.
//
// Layout derived from sub_82190B48 constructor (44-byte stride, 11 DWORDs):
//   +0x00: list next (flink for intrusive list)
//   +0x04: list prev (blink)
//   +0x08: base offset (for relocation within voice buffer)
//   +0x0C: render data word 0 (size field, e.g. 16 or 24)
//   +0x10: render data word 1 (self-pointer for sentinel)
//   +0x14: render data word 2 (sentinel flink)
//   +0x18: render data word 3 (stride/format)
//   +0x1C: render data word 4 (sentinel flink for second pair)
//   +0x20: render data word 5 (sentinel blink for second pair)
//   +0x24: readyQueue pointer
//   +0x28: freeQueue pointer
// =============================================================================
struct audRenderState {
    uint32_t flink;          // +0x00
    uint32_t blink;          // +0x04
    uint32_t baseOffset;     // +0x08
    uint32_t data[6];        // +0x0C..+0x20 (24 bytes)
    uint32_t readyQueue;     // +0x24
    uint32_t freeQueue;      // +0x28
};
static_assert(sizeof(audRenderState) == 44, "audRenderState must be 44 bytes");

// =============================================================================
// Struct: audCallbackSlot (8 bytes)
// =============================================================================
// Used by sub_821916D8 — iterates 8 slots per bank, calling funcPtr(context).
// Bank index (0 or 1) selects which 64-byte region at device+168.
// =============================================================================
struct audCallbackSlot {
    uint32_t funcPtr;        // +0x00: PPC function address
    uint32_t context;        // +0x04: opaque context pointer
};
static_assert(sizeof(audCallbackSlot) == 8, "audCallbackSlot must be 8 bytes");

// =============================================================================
// Struct: audSubmitInfo (8 bytes)
// =============================================================================
// Written by sub_82191360 — sets a per-core ready flag within the struct.
// Two instances at device+356 and device+364, selected by alternating index.
// =============================================================================
struct audSubmitInfo {
    uint8_t  readyFlags[4];  // +0x00: per-core ready flag (indexed by TLS coreId)
    uint32_t reserved;       // +0x04
};
static_assert(sizeof(audSubmitInfo) == 8, "audSubmitInfo must be 8 bytes");

// =============================================================================
// Struct: audDevice (>= 372 bytes)
// =============================================================================
// The main RAGE audio mixer device. Singleton stored at g_pAudioDevice.
//
// RENDER FLOW (Xbox 360):
//   1. sub_821909D0 (primary render thread):
//      Waits on g_renderWakeEvent, calls sub_8218FFB0 (drain) +
//      sub_82191228 (voice update), signals g_shutdownDoneEvent.
//
//   2. sub_82190A98 (secondary render threads):
//      Wait on g_workerSemaphore, call sub_82191228, loop.
//
//   3. sub_821910D0 (render pump, called from endpoint Present):
//      Lock g_audioDeviceCritSec.
//      If numWorkerThreads > 0: signal g_renderWakeEvent, wait for
//      g_shutdownDoneEvent + g_shutdownCompleteEvent (2-object wait).
//      Else: inline drain + voice update.
//      Atomic-increment g_frontBuffer[0].
//      Copy g_frontBuffer -> g_backBuffer, zero g_frontBuffer.
//      Unlock critsec.
//
//   4. sub_82194340 (XAudio DPC callback):
//      For each enabled channel, reads g_backBuffer[ch] and calls
//      XAudioSubmitRenderDriverFrame.
//
// SDL REPLACEMENT:
//   Steps 1-3 are handled by SDL's audio callback thread which invokes
//   the guest callback (sub_82194340 path) via XAudioRegisterClient.
//   The SDL2 driver in sdl2_driver.cpp handles frame timing, downmix,
//   and volume. sub_82191858 is no-op'd to prevent UAF on freed voices.
//   sub_82212EC0 is hooked to force connection state = connected.
// =============================================================================
struct audDevice {
    uint32_t vtable;                                    // +0x000
    uint32_t unk_04;                                    // +0x004
    uint32_t pAllocator;                                // +0x008: heap allocator interface ptr
    uint32_t voicePtrs[12];                             // +0x00C: voice object pointers (2 per HW thread)
    uint32_t pEndpointBase;                             // +0x03C: endpoint obj (before adjustment)
    uint32_t pEndpoint;                                 // +0x040: endpoint obj ptr (used for vtable calls)
    uint32_t voiceQueueHead;                            // +0x044
    uint32_t voiceQueuePad[2];                          // +0x048
    audRenderState masterVoice;                         // +0x050: primary voice render state (44 bytes)
    uint32_t pExtraVoices;                              // +0x07C: allocated array of extra voices
    uint8_t  numExtraVoices;                            // +0x080
    uint8_t  pad_081[3];                                // +0x081
    float    categoryVolumes[NUM_VOICE_CATEGORIES];     // +0x084: indexed by category (0=default, 1=music)
    uint32_t volumeChangeMask;                          // +0x08C
    uint32_t perCoreThreadIds[NUM_WORKER_THREADS_MAX];  // +0x090: TLS thread-ID per core
    audCallbackSlot callbacks[NUM_CALLBACK_BANKS][CALLBACKS_PER_BANK]; // +0x0A8: 2 banks x 8 slots
    uint32_t currentCallbackPtr;                        // +0x128: active callback during dispatch
    uint32_t currentThreadId;                           // +0x12C: render thread's TLS ID
    uint32_t numWorkerThreads;                          // +0x130: count of spawned worker threads
    uint32_t threadHandles[NUM_WORKER_THREADS_MAX];     // +0x134: thread object handles
    uint32_t pad_14C[6];                                // +0x14C: alignment / extra handles
    audSubmitInfo submitInfo[2];                         // +0x164: alternating submit structs
};
// Minimum size verified: offset 356 + 16 = 372 (0x174)

// =============================================================================
// Struct: audRenderDriverEndpoint (28+ bytes)
// =============================================================================
// Wraps XAudioRegisterRenderDriverClient / XAudioSubmitRenderDriverFrame.
// Vtable set to 0x820BE5EC/0x820BE618 in constructor, 0x820BE494 on shutdown.
//
// sub_82194698 (Present):
//   Reads format from (a2-8)+20..28 (7 DWORDs of timing/format info).
//   Calls vtable[8] (endpoint.SubmitMix) on the endpoint.
//   Calls XAudioSubmitRenderDriverFrame(this->driverHandle, timestamp).
//
// sub_82194708 (Connect):
//   If this->driverHandle: XAudioUnregisterRenderDriverClient(handle).
//   If a2 (callback): XAudioRegisterRenderDriverClient({a2, this->maxFrames}, &handle).
//
// sub_821945C0 (Disconnect / destructor):
//   Sets vtable[0] = 0x820BE5EC, vtable[1] = 0x820BE618.
//   Atomic-add 6144 to g_xaudioFrameSize counter.
//   If driverHandle: XAudioUnregisterRenderDriverClient.
//   Sets vtable[1] = 0x820BE494 (shutdown vtable).
// =============================================================================
struct audRenderDriverEndpoint {
    uint32_t vtableA;         // +0x00: primary vtable (0x820BE5EC)
    uint32_t vtableB;         // +0x04: secondary vtable (0x820BE618)
    uint32_t refCount;        // +0x08: reference count (sub_82194438 decrements)
    uint32_t maxFrames;       // +0x0C: max frame size for XAudioRegisterRenderDriverClient
    uint32_t unk_10;          // +0x10
    uint32_t unk_14;          // +0x14
    uint32_t driverHandle;    // +0x18: handle from XAudioRegisterRenderDriverClient
};

// =============================================================================
// Struct: audDeviceConnectionState (>= 2028 bytes)
// =============================================================================
// Used by sub_82212EC0 (device connection state machine).
// Large structure — only key fields mapped.
//
// sub_82212EC0 checks field+2016 == 1 (connecting state).
// Calls sub_8220FDB8 to poll async device enumeration.
// On PC/macOS, hooked to force state=0 (connected) immediately.
// =============================================================================
struct audDeviceConnectionState {
    uint8_t  pad_000[288];       // +0x000
    uint32_t endpointCount;      // +0x120 (288): number of discovered endpoints
    uint8_t  pad_124[1724];      // +0x124
    uint32_t connectionState;    // +0x7E0 (2016): 0=connected, 1=connecting
    uint8_t  pad_7E4[4];         // +0x7E4
    uint8_t  flags;              // +0x7E8 (2024): bit7 = endpoint ready
};

} // namespace rage::aud

// =============================================================================
// Host-side audio render pump state
// =============================================================================
// Mirrors the Xbox 360 render pump (sub_821910D0) but uses SDL2 for output.
// The SDL2 audio thread calls the game's registered XAudio callback which
// drives the entire audio mix pipeline.
//
// Thread model:
//   Xbox 360: 1 primary + up to 5 secondary render threads, woken by events.
//   SDL2 recomp: 1 audio thread polls SDL queue depth, invokes guest callback
//   at ~5.33ms intervals (48kHz / 256 samples). The guest callback chain:
//     XAudioRegisterRenderDriverClient callback
//     -> sub_82194340 (reads g_backBuffer, calls XAudioSubmitRenderDriverFrame)
//     -> XAudioSubmitFrame (SDL_QueueAudio with volume + downmix)
//
// Critical section usage:
//   sub_821910D0 holds g_audioDeviceCritSec for the entire render pump:
//     enter critsec -> drain/update voices -> atomic inc frontBuffer ->
//     copy front->back, zero front -> leave critsec.
//   In the recomp, the critsec calls go to RtlEnterCriticalSection/
//   RtlLeaveCriticalSection which are rexcrt-hooked to std::mutex.
//
// Double buffer:
//   g_frontBuffer[7]: Accumulated by render threads (atomic increment).
//   g_backBuffer[7]:  Snapshot consumed by XAudio callback.
//   Swap happens under critsec in sub_821910D0.
// =============================================================================
