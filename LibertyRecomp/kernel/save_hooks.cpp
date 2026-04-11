// =============================================================================
// GTA IV Save System PPC Function Hooks
// =============================================================================
// Essential platform fixes for save/loading system integration.
// =============================================================================

#include <stdafx.h>
#include <cpu/ppc_context.h>
#include "function.h"
#include "memory.h"
#include <cstdio>
#include <thread>
#include <chrono>
#include <atomic>
#include <os/logger.h>

// =============================================================================
// KERNEL PHASE - extern declarations from imports.cpp
// =============================================================================
enum class KernelPhase { Boot, Init, Runtime };
extern std::atomic<KernelPhase> g_kernelPhase;
extern void KernelPhase_EnterRuntime();

// =============================================================================
// sub_821200D0 - Post-Init (Profiles/Saves)
// =============================================================================
extern "C" void __imp__sub_821200D0(PPCContext& ctx, uint8_t* base);
extern void ShutdownAllWorkers();

constexpr uint32_t LOADING_FLAG_ADDR = 0x83137BB7;
constexpr uint32_t LOADING_STEP_ADDR = 0x83137BC9;
constexpr uint32_t SAVE_STATE_ADDR  = 0x82B94554;
constexpr uint32_t SAVE_RETVAL_ADDR = 0x82B946C8;
constexpr uint32_t SAVE_FINAL_STATE = 17;
constexpr uint32_t SAVE_NEW_GAME    = 3;

// sub_821200D0 hook -- bypass the loading screen busy-wait gate.
//
// Fix 1: force BC9 = 0 so Loop 1 exits immediately.
// Fix 2: pre-set save state machine to final state so sub_821E6508 returns 3.
// Fix 3: Loop 2 already eliminated by LTO.
PPC_FUNC(sub_821200D0)
{
    static int s_count = 0;
    static bool s_runtimeForced = false;
    ++s_count;

    printf("[821200D0] #%d ENTER\n", s_count); fflush(stdout);

    if (!s_runtimeForced) {
        s_runtimeForced = true;
        if (g_kernelPhase.load(std::memory_order_acquire) != KernelPhase::Runtime) {
            KernelPhase_EnterRuntime();
            LOG_WARNING("[KERNEL_PHASE] Forced Init -> Runtime at sub_821200D0");
        }
    }

    PPC_STORE_U8(LOADING_STEP_ADDR, 0);
    PPC_STORE_U32(SAVE_STATE_ADDR,  SAVE_FINAL_STATE);
    PPC_STORE_U32(SAVE_RETVAL_ADDR, SAVE_NEW_GAME);

    __imp__sub_821200D0(ctx, base);

    printf("[821200D0] #%d EXIT r3=%d\n", s_count, ctx.r3.s32); fflush(stdout);
}

// =============================================================================
// sub_8219F728 - Active Player Slot Counter
// =============================================================================
// On Xbox 360, iterates player slots populated by XNotify sign-in events.
// In the recomp, no sign-in events fire so all slots are null -> returns 0.
// Fix: report 1 active player (user 0).
extern "C" void __imp__sub_8219F728(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8219F728)
{
    static int s_count = 0;
    if (++s_count <= 3)
        printf("[sub_8219F728] active-player count hook #%d — returning 1 (user 0 active)\n", s_count);
    ctx.r3.s64 = 1;
}

// =============================================================================
// sub_8218C2C0 - Loading Complete Check
// =============================================================================
// Depends on Xbox 360 VBlank hardware that doesn't exist in the recomp.
// Fix: return 1 ("loading complete") so Loop 2 exits.
extern "C" void __imp__sub_8218C2C0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8218C2C0)
{
    static int s_count = 0;
    if (++s_count <= 5)
        printf("[sub_8218C2C0] hook called #%d — returning 1\n", s_count);
    ctx.r3.s64 = 1;
}

// =============================================================================
// sub_82192E00 - Streaming Init
// =============================================================================
extern "C" void __imp__sub_82192E00(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82192E00)
{
    static int s_count = 0;
    ++s_count;

    __imp__sub_82192E00(ctx, base);

    if (s_count <= 3) {
        uint32_t flagVal = PPC_LOAD_U32(0x830F5820);
        printf("[sub_82192E00] hook #%d: streaming init dispatched, "
               "0x830F5820=%u (async workers will clear)\n", s_count, flagVal);
        fflush(stdout);
    }
}

// =============================================================================
// sub_827DE648 - Streaming Completion Barrier
// =============================================================================
PPC_FUNC(sub_827DE648)
{
    static int s_count = 0;
    ++s_count;

    constexpr uint32_t STREAMING_PENDING = 0x830F5820;
    constexpr int MAX_POLLS = 50000;

    int polls = 0;
    while (PPC_LOAD_U32(STREAMING_PENDING) != 0) {
        std::this_thread::yield();
        ++polls;

        if (polls >= MAX_POLLS) {
            PPC_STORE_U32(STREAMING_PENDING, 0);
            if (s_count <= 5)
                printf("[sub_827DE648] barrier #%d: TIMEOUT after %d polls — "
                       "force-cleared 0x830F5820\n", s_count, polls);
            fflush(stdout);
            return;
        }
    }

    if (s_count <= 5) {
        printf("[sub_827DE648] barrier #%d: completed after %d polls%s\n",
               s_count, polls, polls == 0 ? " (immediate)" : "");
        fflush(stdout);
    }
}
