// =============================================================================
// GTA IV Loading Screen State Machine — Complete Decompilation
// =============================================================================
//
// Decompiled functions:
//   sub_82144188  — Frame pump (5 state types, timing, fade logic)
//   sub_822155F0  — Audio readiness check (XAudio bank state)
//   sub_82144708  — Advance to next entry in the loading screen table
//   sub_82144800  — Setup / legal text display
//   sub_8214C8C8  — Popup counter and menu state dispatcher
//
// PC audio fix: The Xbox 360 version spins on XAudio wave-bank state
// (sub_822155F0) until the bank reaches PLAYING. On PC we replace this
// with AudioStateManager::IsAudioReady() plus a 10-second timeout so the
// game never hangs if audio initialisation is slow or fails.
//
// All rendering is delegated to the original PPC implementations since
// the draw pipeline (sub_828BF120, sub_828C21D0, sub_828C2290, etc.)
// interacts heavily with the GPU command buffer and is best left as
// recompiled PPC.
// =============================================================================

#include <api/Liberty.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <apu/audio_state.h>
#include <os/logger.h>
#include <chrono>
#include <cstring>

// =============================================================================
// Guest Memory Addresses
// =============================================================================

namespace LoadingScreen {

// ---- State variable cluster (0x831D5330 .. 0x831D534A) ----
constexpr uint32_t OVERLAY_MODE     = 0x831D5330;  // int32  — overlay active flag
constexpr uint32_t ACTIVE           = 0x831D5335;  // uint8  — loading screens enabled
constexpr uint32_t WAITING_AUDIO    = 0x831D5336;  // uint8  — currently waiting on audio
constexpr uint32_t FROZEN           = 0x831D5337;  // uint8  — timer paused / skip state
constexpr uint32_t ACCUM_TIME       = 0x831D5338;  // float  — accumulated time (seconds)
constexpr uint32_t DELTA_TIME       = 0x831D533C;  // float  — per-frame delta (seconds)
constexpr uint32_t CURRENT_INDEX    = 0x831D5340;  // int32  — current entry index
constexpr uint32_t ENTRY_COUNT      = 0x831D5344;  // int32  — total entries in table
constexpr uint32_t ALLOW_SKIP       = 0x831D5348;  // uint8  — user-skip allowed
constexpr uint32_t FADE_OUT_FLAG    = 0x831D534A;  // uint8  — fade-out in progress

// ---- PPC timebase snapshot (for original delta-time path) ----
constexpr uint32_t LAST_TIMEBASE    = 0x831D5490;  // uint64

// ---- Entry table (stride 400 bytes) ----
constexpr uint32_t ENTRY_TABLE      = 0x831D5498;
constexpr uint32_t ENTRY_STRIDE     = 400;

// Entry header offsets (from entry base)
constexpr uint32_t OFF_DURATION_MS  = 0x00;  // int32
constexpr uint32_t OFF_SUB_COUNT    = 0x04;  // int32
constexpr uint32_t OFF_STATE_TYPE   = 0x08;  // int32
constexpr uint32_t OFF_FADE_TYPE    = 0x0C;  // int32

// ---- Fade alpha ratchets ----
constexpr uint32_t FADE_IN_ALPHA    = 0x831E4DD8;  // int32 [0..255]
constexpr uint32_t FADE_OUT_ALPHA   = 0x831E4DDC;  // int32 [0..255]

// ---- Audio resource handle ----
constexpr uint32_t AUDIO_RESOURCE   = 0x82BCC1F8;

// ---- Popup counter (sub_8214C8C8) ----
constexpr uint32_t POPUP_COUNTER    = 0x82BF9B70;

// ---- Input flag cleared by STATE_FADE_TEXT ----
constexpr uint32_t INPUT_FLAG       = 0x82B282D4;

} // namespace LoadingScreen

// =============================================================================
// State type / fade type enums
// =============================================================================

enum LoadingStateType : int32_t {
    LST_FADE_TEXT   = 1,  // Legal text with fade
    LST_STATIC      = 2,  // Static image
    LST_SKIPPABLE   = 3,  // User-skippable
    LST_SEPARATOR   = 4,  // Boundary marker (skipped during advance)
    LST_WAIT_AUDIO  = 5,  // Gate on audio readiness
};

enum LoadingFadeType : int32_t {
    LFT_NONE        = 0,
    LFT_IN_OUT      = 1,  // Fade in + fade out
    LFT_IN          = 2,  // Fade in only
    LFT_OUT         = 3,  // Fade out only
};

// =============================================================================
// Helpers — float <-> uint32 bit-cast for PPC memory macros
// =============================================================================
// PPC_LOAD_U32 returns a byte-swapped uint32 (host-endian bit pattern).
// PPC_STORE_U32 expects a host-endian uint32 and writes it byte-swapped.
// For floats: memcpy between float and uint32 preserves the bit pattern.

static inline float LoadGuestFloat(uint8_t* base, uint32_t addr) {
    uint32_t bits = PPC_LOAD_U32(addr);
    float val;
    std::memcpy(&val, &bits, sizeof(float));
    return val;
}

static inline void StoreGuestFloat(uint8_t* base, uint32_t addr, float val) {
    uint32_t bits;
    std::memcpy(&bits, &val, sizeof(uint32_t));
    PPC_STORE_U32(addr, bits);
}

// =============================================================================
// Audio readiness — PC replacement for sub_822155F0
// =============================================================================
// Xbox original (52 bytes, leaf):
//   bool sub_822155F0(int a1) {
//       if (*(uint16*)(a1+4) == 0xFFFF) return false;
//       int p = *(int*)(a1+16);
//       if (!p) return false;
//       return *(uint16*)(p+6) == 2;  // XAudio bank state == PLAYING
//   }
//
// PC: Query AudioStateManager. The wave bank handle structure does not
// exist on PC (we decode XMA via FFmpeg), so checking guest memory is
// meaningless.

static constexpr float AUDIO_WAIT_TIMEOUT_SEC = 10.0f;

static bool IsAudioReadyPC() {
    return AudioStateManager::Instance().IsAudioReady();
}

PPC_FUNC_IMPL(__imp__sub_822155F0);
PPC_FUNC_HOOK(sub_822155F0) {
    ctx.r3.u64 = IsAudioReadyPC() ? 1u : 0u;
}

// =============================================================================
// sub_82144708 — Advance loading screen entry
// =============================================================================
// Pseudocode (248 bytes):
//   void sub_82144708(bool wrap, bool randomize) {
//       if (!wrap && currentIdx < entryCount - 1) {
//           currentIdx++;
//       } else {
//           currentIdx = 0;
//           // Skip past initial non-SEPARATOR entries to find loop start
//           if (entry[0].stateType != SEPARATOR) {
//               for (i = 1; i <= entryCount; i++) {
//                   if (entry[i].stateType == SEPARATOR) { currentIdx = i; break; }
//                   if (i > entryCount) { currentIdx = i; i = 0; break; }
//               }
//               currentIdx = i;
//           }
//           if (randomize) {
//               currentIdx = rand(0, entryCount-1);
//               if (currentIdx >= entryCount) sub_82144708(1, 0); // recurse
//           }
//           sub_821458E0();  // reset sub-item animation positions
//       }
//       accumTime = 0.0f;
//   }

PPC_FUNC_IMPL(__imp__sub_82143C00);   // int rand_range(int min, int max)
PPC_FUNC_IMPL(__imp__sub_821458E0);   // void reset_subitem_animations()
PPC_FUNC_IMPL(__imp__sub_82144708);

PPC_FUNC_HOOK(sub_82144708) {
    using namespace LoadingScreen;

    bool wrap      = (ctx.r3.u32 & 0xFF) != 0;
    bool randomize = (ctx.r4.u32 & 0xFF) != 0;

    int32_t curIdx     = static_cast<int32_t>(PPC_LOAD_U32(CURRENT_INDEX));
    int32_t entryCount = static_cast<int32_t>(PPC_LOAD_U32(ENTRY_COUNT));

    if (!wrap && curIdx < entryCount - 1) {
        // Simple sequential advance
        PPC_STORE_U32(CURRENT_INDEX, static_cast<uint32_t>(curIdx + 1));
    } else {
        // Wrap: find first SEPARATOR or loop from entry 0
        PPC_STORE_U32(CURRENT_INDEX, 0u);

        int32_t firstType = static_cast<int32_t>(PPC_LOAD_U32(ENTRY_TABLE + OFF_STATE_TYPE));
        if (firstType != LST_SEPARATOR) {
            int32_t found = 0;
            for (int32_t i = 1; i <= entryCount; i++) {
                int32_t st = static_cast<int32_t>(
                    PPC_LOAD_U32(ENTRY_TABLE + i * ENTRY_STRIDE + OFF_STATE_TYPE));
                if (st == LST_SEPARATOR) {
                    found = i;
                    break;
                }
            }
            // If no SEPARATOR found, found stays 0 (loop from start)
            PPC_STORE_U32(CURRENT_INDEX, static_cast<uint32_t>(found));
        }

        if (randomize) {
            // Pick random entry via game's rand_range (sub_82143C00)
            ctx.r3.s64 = 0;
            ctx.r4.s64 = entryCount - 1;
            __imp__sub_82143C00(ctx, base);
            int32_t rndIdx = ctx.r3.s32;
            PPC_STORE_U32(CURRENT_INDEX, static_cast<uint32_t>(rndIdx));

            // Guard: if random index is out of range, recurse with wrap
            if (rndIdx >= entryCount) {
                ctx.r3.u64 = 1;  // wrap = true
                ctx.r4.u64 = 0;  // randomize = false
                sub_82144708(ctx, base);
                StoreGuestFloat(base, ACCUM_TIME, 0.0f);
                return;
            }
        }

        // Reset all sub-item animation positions for the new entry set
        __imp__sub_821458E0(ctx, base);
    }

    // Always reset accumulated time on entry change
    StoreGuestFloat(base, ACCUM_TIME, 0.0f);
}

// =============================================================================
// sub_82144188 — Loading screen frame pump (1408 bytes)
// =============================================================================
// The main per-frame tick. Responsibilities:
//   1. Early-out if loading screens are disabled (byte_831D5335 == 0)
//   2. Compute delta time from PPC timebase (replaced with std::chrono on PC)
//   3. Accumulate time; check if current entry's duration has elapsed
//   4. Handle state-type-specific logic:
//      - TYPE 5 (WAIT_AUDIO): poll sub_822155F0, don't advance until ready
//      - TYPE 3 (SKIPPABLE): freeze if skip flag is set
//      - TYPE 1 (FADE_TEXT): clear input flag on advance
//   5. Call sub_82144708 to advance entry when duration expires
//   6. Delegate rendering:
//      - sub_821B3970 / sub_821F4150: begin frame
//      - sub_828BF120: clear to black
//      - sub_828BD648 / sub_828C20C8: render pass setup
//      - sub_82144B98 (overlay mode) or sub_82143DC8 (normal mode)
//      - sub_82144800: legal text for TYPE 1
//      - Fade-in/out alpha overlays (500ms ramp, rate 0.002/ms)
//
// Our hook intercepts the advancement logic to fix the audio-wait path,
// then delegates all rendering to the original recompiled implementation.

PPC_FUNC_IMPL(__imp__sub_82144188);

// Audio-wait timeout tracking (persistent across frames)
static std::chrono::steady_clock::time_point s_audioWaitStart;
static bool s_audioWaitActive = false;

PPC_FUNC_HOOK(sub_82144188) {
    using namespace LoadingScreen;

    // ---- Guard: loading screens must be active ----
    if (!PPC_LOAD_U8(ACTIVE))
        return;

    // ---- Determine if timer is paused ----
    bool frozen = PPC_LOAD_U8(FROZEN) != 0;
    bool overlay = PPC_LOAD_U32(OVERLAY_MODE) != 0;
    bool timerPaused = frozen || overlay;

    // ---- Compute delta time ----
    // On Xbox 360 this reads the PPC timebase (~50 MHz) via mftb.
    // On PC we use std::chrono for a precise host-side clock.
    static auto s_lastFrame = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    float dt = std::chrono::duration<float>(now - s_lastFrame).count();
    s_lastFrame = now;

    // Clamp delta to prevent huge jumps (e.g. after breakpoint)
    if (dt > 0.5f) dt = 0.033f;

    float accumTime;
    if (timerPaused) {
        StoreGuestFloat(base, DELTA_TIME, 0.0f);
        accumTime = 0.0f;
    } else {
        StoreGuestFloat(base, DELTA_TIME, dt);
        float prev = LoadGuestFloat(base, ACCUM_TIME);
        accumTime = prev + dt;
    }
    StoreGuestFloat(base, ACCUM_TIME, accumTime);

    // ---- Entry advancement (only when timer is running) ----
    if (!timerPaused) {
        int32_t curIdx = static_cast<int32_t>(PPC_LOAD_U32(CURRENT_INDEX));
        uint32_t eBase = ENTRY_TABLE + static_cast<uint32_t>(curIdx) * ENTRY_STRIDE;

        // Duration is stored as milliseconds (int32). Convert to seconds.
        int32_t durMs = static_cast<int32_t>(PPC_LOAD_U32(eBase + OFF_DURATION_MS));
        float durSec = static_cast<float>(durMs) / 1000.0f;

        // Time exceeded -> process state type and advance
        if (accumTime * 1000.0f >= static_cast<float>(durMs)) {
            uint8_t allowSkip = PPC_LOAD_U8(ALLOW_SKIP);
            int32_t stateType = static_cast<int32_t>(PPC_LOAD_U32(eBase + OFF_STATE_TYPE));

            // --- TYPE 3 (SKIPPABLE): set frozen, clear skip flag ---
            if (allowSkip && stateType == LST_SKIPPABLE) {
                PPC_STORE_U8(ALLOW_SKIP, 0);
                PPC_STORE_U8(FROZEN, 1);
            }

            // --- TYPE 5 (WAIT_AUDIO): gate on AudioStateManager ---
            if (stateType == LST_WAIT_AUDIO) {
                if (!IsAudioReadyPC()) {
                    if (!s_audioWaitActive) {
                        s_audioWaitStart = std::chrono::steady_clock::now();
                        s_audioWaitActive = true;
                        LOG_WARNING("[LoadingScreen] Waiting on audio readiness...");
                    }

                    float elapsed = std::chrono::duration<float>(
                        std::chrono::steady_clock::now() - s_audioWaitStart).count();

                    if (elapsed < AUDIO_WAIT_TIMEOUT_SEC) {
                        // Still waiting: set flag so the original renderer
                        // shows the "waiting" state, then skip advancement.
                        PPC_STORE_U8(WAITING_AUDIO, 1);
                        goto render;
                    }
                    // Timeout expired: force advance
                    LOGF_WARNING("[LoadingScreen] Audio wait timed out after {:.1f}s, forcing advance",
                                 elapsed);
                    s_audioWaitActive = false;
                } else {
                    if (s_audioWaitActive) {
                        float elapsed = std::chrono::duration<float>(
                            std::chrono::steady_clock::now() - s_audioWaitStart).count();
                        LOGF_WARNING("[LoadingScreen] Audio became ready after {:.1f}s", elapsed);
                    }
                    s_audioWaitActive = false;
                }
            }

            // --- TYPE 1 (FADE_TEXT): clear input flag when advancing ---
            if (allowSkip && stateType == LST_FADE_TEXT) {
                PPC_STORE_U8(INPUT_FLAG, 0);
            }

            // Advance to next entry
            ctx.r3.u64 = 0;  // wrap = false
            ctx.r4.u64 = 0;  // randomize = false
            sub_82144708(ctx, base);
        }
    }

    // Clear audio-wait flag if we are not waiting
    PPC_STORE_U8(WAITING_AUDIO, 0);

render:
    // ---- Delegate all rendering to original PPC implementation ----
    // The original function reads ACTIVE, FROZEN, OVERLAY_MODE, ACCUM_TIME,
    // DELTA_TIME, CURRENT_INDEX, WAITING_AUDIO etc. from guest memory.
    // We have already written the correct values above, so the original
    // rendering path (fades, quad draws, legal text, sub-item animations)
    // works correctly.
    __imp__sub_82144188(ctx, base);
}

// =============================================================================
// sub_82144800 — Legal text setup (436 bytes)
// =============================================================================
// Draws "LEGAL_360" or "LEGAL_360_US" depending on XGetGameRegion().
// On PC, XGetGameRegion returns 0xFF, selecting LEGAL_360_US (US/generic).
// Also patches version string "2008" -> "2009" in the legal text buffer.
// Pure pass-through: no PC-specific changes needed.

PPC_FUNC_IMPL(__imp__sub_82144800);
PPC_FUNC_HOOK(sub_82144800) {
    __imp__sub_82144800(ctx, base);
}

// =============================================================================
// sub_8214C8C8 — Popup counter & menu state dispatcher (3264 bytes)
// =============================================================================
// Already hooked in kernel/imports.cpp as a pass-through.
// Manages popup_counter at 0x82BF9B70 (max 4, then dismiss via sub_8224FA38).
// Dispatches ~48 menu screen states through a large switch (frontend flow).
// Pure game logic — no PC-specific changes needed.
//
// Key behavior:
//   if (sub_8224FA48()) {           // popup detected?
//       if (popup_counter < 4)
//           popup_counter++;
//       else
//           sub_8224FA38();         // dismiss popup
//   }
//   if (sub_8214B168())             // some condition
//       sub_8224F020();             // handle
//   ... large switch on dword_82BFA124 (menu state) ...

// =============================================================================
// No explicit registration needed.
// PPC_FUNC_HOOK expands to: extern "C" PPC_FUNC(sub_XXXXXXXX)
// The extern "C" linkage overrides the weak __imp__sub_XXXXXXXX symbols
// emitted by the code generator at link time.
// =============================================================================
