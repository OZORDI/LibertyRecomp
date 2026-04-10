// =============================================================================
// GTA IV Save System PPC Function Hooks
// =============================================================================
// These hooks intercept GTA IV's save wrapper functions to provide visibility
// into save operations and ensure proper integration with host filesystem.
//
// Based on REWRITE_PLAYBOOK.md section 14.1:
// - sub_829A1C38: Content creation wrapper (calls XamContentCreateEx)
// - sub_829A1CA0: Content close wrapper (calls XamContentClose)
// - sub_829A1CB8: Enumeration wrapper (calls XamContentCreateEnumerator)
// - sub_8297A930: Save manager (orchestrates save operations)
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

// Debug logging control
#define SAVE_HOOKS_DEBUG_LOGGING 1

// =============================================================================
// sub_829A1C38 - Content Creation Wrapper
// Location: ppc_recomp.79.cpp:37654
// Calls XamContentCreateEx to create/open save containers
// =============================================================================
extern "C" void __imp__sub_829A1C38(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829A1C38)
{
    static int s_count = 0;
    ++s_count;
    
#if SAVE_HOOKS_DEBUG_LOGGING
    printf("[SaveHook] ========================================\n");
    printf("[SaveHook] sub_829A1C38 (Content Creation) #%d\n", s_count);
    printf("[SaveHook] r3=0x%08X r4=0x%08X r5=0x%08X\n", ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
#endif
    
    // Call original implementation
    __imp__sub_829A1C38(ctx, base);
    
#if SAVE_HOOKS_DEBUG_LOGGING
    printf("[SaveHook] sub_829A1C38 returned r3=0x%08X\n", ctx.r3.u32);
    printf("[SaveHook] ========================================\n");
#endif
}

// =============================================================================
// sub_829A1CA0 - Content Close Wrapper
// Location: ppc_recomp.79.cpp:37723
// Direct jump to XamContentClose
// =============================================================================
extern "C" void __imp__sub_829A1CA0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829A1CA0)
{
    static int s_count = 0;
    ++s_count;
    
#if SAVE_HOOKS_DEBUG_LOGGING
    printf("[SaveHook] sub_829A1CA0 (Content Close) #%d r3=0x%08X\n", s_count, ctx.r3.u32);
#endif
    
    // Call original implementation
    __imp__sub_829A1CA0(ctx, base);
    
#if SAVE_HOOKS_DEBUG_LOGGING
    printf("[SaveHook] sub_829A1CA0 returned r3=0x%08X\n", ctx.r3.u32);
#endif
}

// =============================================================================
// sub_829A1CB8 - Enumeration Wrapper
// Location: ppc_recomp.79.cpp:37756
// Calls XamContentCreateEnumerator to list save files
// =============================================================================
extern "C" void __imp__sub_829A1CB8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829A1CB8)
{
    static int s_count = 0;
    ++s_count;
    
#if SAVE_HOOKS_DEBUG_LOGGING
    printf("[SaveHook] ========================================\n");
    printf("[SaveHook] sub_829A1CB8 (Content Enumeration) #%d\n", s_count);
    printf("[SaveHook] r3=0x%08X r4=0x%08X r5=0x%08X r6=0x%08X\n", 
           ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
#endif
    
    // Call original implementation
    __imp__sub_829A1CB8(ctx, base);
    
#if SAVE_HOOKS_DEBUG_LOGGING
    printf("[SaveHook] sub_829A1CB8 returned r3=0x%08X\n", ctx.r3.u32);
    printf("[SaveHook] ========================================\n");
#endif
}

// =============================================================================
// sub_8297A930 - Save Manager
// Location: ppc_recomp.77.cpp:7308
// Orchestrates save operations, calls sub_829A1878
// =============================================================================
extern "C" void __imp__sub_8297A930(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8297A930)
{
    static int s_count = 0;
    ++s_count;
    
#if SAVE_HOOKS_DEBUG_LOGGING
    if (s_count <= 10) {
        printf("[SaveHook] ========================================\n");
        printf("[SaveHook] sub_8297A930 (Save Manager) #%d\n", s_count);
        printf("[SaveHook] r3=0x%08X (context+208)\n", ctx.r3.u32);
        printf("[SaveHook] ========================================\n");
    }
#endif
    
    // Call original implementation
    __imp__sub_8297A930(ctx, base);
    
#if SAVE_HOOKS_DEBUG_LOGGING
    if (s_count <= 10) {
        printf("[SaveHook] sub_8297A930 returned\n");
    }
#endif
}

// =============================================================================
// sub_82122CA0 - Save System Init
// Location: ppc_recomp.0.cpp:7117
// Initializes save system with 3 save slot contexts (Profile, Game, Autosave)
// =============================================================================
extern "C" void __imp__sub_82122CA0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82122CA0)
{
    static int s_count = 0;
    ++s_count;
    
    printf("[SaveHook] ========================================\n");
    printf("[SaveHook] sub_82122CA0 (Save System Init) #%d\n", s_count);
    printf("[SaveHook] Initializing 3 save slot contexts:\n");
    printf("[SaveHook]   - Profile Save (vtable: 0x81209104)\n");
    printf("[SaveHook]   - Game Save (vtable: 0x81209064)\n");
    printf("[SaveHook]   - Autosave (vtable: 0x81209028)\n");
    printf("[SaveHook] ========================================\n");
    
    // Call original implementation
    __imp__sub_82122CA0(ctx, base);
    
    printf("[SaveHook] sub_82122CA0 completed\n");
}

// =============================================================================
// sub_821200D0 - Post-Init (Profiles/Saves)
// Location: ppc_recomp.0.cpp:134
// Loads profile and enumerates/opens save files
// =============================================================================
extern "C" void __imp__sub_821200D0(PPCContext& ctx, uint8_t* base);
// sub_821238D0 is declared in ppc_recomp_shared.h via PPC_EXTERN_FUNC
extern void ShutdownAllWorkers();  // From imports.cpp - sets exit flags and signals workers

// Memory addresses for loading screen state (Python-verified from PPC source)
// Base: lis r11,-31981 = 0x83130000
// byte_83137BB7 = base + 31671 = 0x83137BB7 (loading in progress flag)
// byte_83137BC9 = base + 31689 = 0x83137BC9 (loading step - exit when 0)
constexpr uint32_t LOADING_FLAG_ADDR = 0x83137BB7;  // Loading in progress flag
constexpr uint32_t LOADING_STEP_ADDR = 0x83137BC9;  // Loading step (non-zero = still loading)

// Save state machine addresses (inside sub_821E6508, called by sub_82121E80)
// sub_82121E80's yield loop calls sub_821E6508 which returns dword_82B946C8.
// The loop exits when the return value is 1 (load save), 2 (start loading), or 3 (new game).
// dword_82B94554 tracks the state machine step (0-17).
// Setting state=17 (final) and retval=3 (new game) makes the loop exit immediately.
// Python-verified: state 17 > 6 skips sub_8219F728 sign-in check; switch(17) → default → return 3.
constexpr uint32_t SAVE_STATE_ADDR  = 0x82B94554;  // dword_82B94554 - state machine step
constexpr uint32_t SAVE_RETVAL_ADDR = 0x82B946C8;  // dword_82B946C8 - return value
constexpr uint32_t SAVE_FINAL_STATE = 17;           // final/default state
constexpr uint32_t SAVE_NEW_GAME    = 3;            // return 3 → outer for loop breaks

// sub_821200D0 hook — bypass the loading screen busy-wait gate.
//
// Original PPC code (from Hex-Rays pseudocode):
//   while (sub_82124490()) sub_827DAE18(1);   // Loop 1: yield until BC9 clears
//   sub_82121E80();
//   if (byte_83137BB7) { load media; byte_83137BB7 = 0; }
//   for (i = sub_8218C2C0(); !i; ...) { render loop }  // Loop 2: render
//   ... shutdown ...
//
// On Xbox 360, Loop 1 is a passive wait — sub_821238D0 runs on a dedicated
// rendering thread and clears byte_83137BC9 (BC9) when its state machine
// reaches stage 3.  In the recomp there is no rendering thread, so BC9
// never clears and Loop 1 spins forever.
//
// Fix: force BC9 = 0 before calling the original function so Loop 1 exits
// immediately.  The original code handles everything else (media loading,
// clearing BB7, render loop 2, shutdown).
//
// CRITICAL: Named `sub_821200D0` (not `sub_821200D0_hook`) to OVERRIDE the
// weak symbol in ppc_recomp.0.cpp.  By defining a strong symbol we intercept
// ALL calls including direct bl instructions.
PPC_FUNC(sub_821200D0)
{
    static int s_count = 0;
    static bool s_runtimeForced = false;
    ++s_count;

    printf("[821200D0] #%d ENTER\n", s_count); fflush(stdout);

    // Force Runtime phase on first entry to break circular dependency
    if (!s_runtimeForced) {
        s_runtimeForced = true;
        if (g_kernelPhase.load(std::memory_order_acquire) != KernelPhase::Runtime) {
            KernelPhase_EnterRuntime();
            LOG_WARNING("[KERNEL_PHASE] Forced Init -> Runtime at sub_821200D0");
        }
    }

    // === FIX 1: Loop 1 bypass ===
    // Clear BC9 so the original function's Loop 1 (busy-wait on sub_82124490)
    // exits immediately.  sub_82124490 returns BC9 when BB7 != 0, else 0.
    // With BC9 = 0, sub_82124490 returns 0 regardless of BB7.
    PPC_STORE_U8(LOADING_STEP_ADDR, 0);

    // === FIX 2: sub_82121E80 save-load state machine bypass ===
    // sub_82121E80 has an infinite yield loop: sub_827DAE18(1) → sub_821E6508()
    // sub_821E6508 is a state machine that progresses through save loading states.
    // On Xbox 360, it waits for user profile sign-in and save file operations.
    // In the recomp, these Xbox-specific operations never complete, so the loop
    // never exits.
    //
    // Fix: pre-set the state machine to its final state so sub_821E6508 returns 3
    // ("new game") immediately.  The calling loop in sub_82121E80 checks:
    //   v8 = sub_821E6508(); if (v8 == 3) break;
    // This lets sub_82121E80 proceed to its post-loop initialization code
    // (clearing BC9/BC8, calling init functions, setting up the game world).
    //
    // NOTE: This approach is LTO-proof — it writes directly to guest memory
    // rather than trying to override weak symbols that ThinLTO may inline.
    PPC_STORE_U32(SAVE_STATE_ADDR,  SAVE_FINAL_STATE);  // dword_82B94554 = 17
    PPC_STORE_U32(SAVE_RETVAL_ADDR, SAVE_NEW_GAME);     // dword_82B946C8 = 3

    // === FIX 3: Loop 2 already eliminated by LTO ===
    // ThinLTO inlined sub_8218C2C0 inside __imp__sub_821200D0 and determined
    // the result is always 1, so Loop 2 was completely eliminated at link time.
    // No fix needed — the post-loop shutdown runs automatically.

    __imp__sub_821200D0(ctx, base);

    printf("[821200D0] #%d EXIT r3=%d\n", s_count, ctx.r3.s32); fflush(stdout);
}

// =============================================================================
// sub_8219F728 - Active Player Slot Counter
// Location: gta4_recomp.3.cpp:35234
//
// Original PPC behaviour:
//   Iterates 4 player slot structs at 0x82ACBD60 (stride 188).
//   For each slot: inner_ptr = [slot+0]; result = byte[inner_ptr+36]
//   Counts non-zero results and returns the count.
//
// On Xbox 360, player slot objects are created by the Xbox OS XNotify sign-in
// event system.  Their inner_ptr fields point to live Xbox OS objects that have
// an "active" byte set at offset 36.
//
// On macOS, no XNotify sign-in events ever fire, so all inner_ptrs remain null
// (PPC guest memory is zero-initialised on startup).  sub_8230D720 therefore
// reads byte[0+36] = 0 for every slot, returning 0.
//
// sub_821E6508 (the save state-machine gate) calls this function and returns 0
// immediately when it returns 0 — causing sub_82121E80's yield loop to spin
// forever without ever reaching the 17-state save/profile state machine.
//
// Fix: report 1 active player (user 0), which is factually correct on PC —
// RexGlue's UserProfile::signin_state() already hardcodes return 1 for player 0.
// This lets sub_821E6508 proceed past the guard and enter the state machine.
// =============================================================================
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
// Original PPC code:
//   int sub_8218C2C0() {
//     int result = sub_82856FE0();          // checks VBlank via sub_82878998
//     if (!(_BYTE)result)
//       return dword_82A2C54C == 16;        // fallback: check state variable
//     return result;
//   }
//
// In the recomp, sub_82856FE0 → sub_82878998 depends on Xbox 360 VBlank hardware
// that doesn't exist.  This function always returns 0, trapping Loop 2 in
// sub_821200D0 in an infinite render loop.
//
// Fix: return 1 ("loading complete") so Loop 2 exits and the post-loop
// initialization sequence (sub_821219B0, sub_821B8FB0, etc.) runs.
extern "C" void __imp__sub_8218C2C0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8218C2C0)
{
    static int s_count = 0;
    if (++s_count <= 5)
        printf("[sub_8218C2C0] hook called #%d — returning 1\n", s_count);
    // Signal loading is complete — the recomp doesn't need a loading screen.
    ctx.r3.s64 = 1;
}

// =============================================================================
// sub_82192E00 - Streaming Init (runs immediately before streaming barrier)
// Location: gta4_recomp.0.cpp:4100 (inside __imp__sub_821219B0)
//
// Call order in __imp__sub_821219B0:
//   sub_82192E00(ctx, base);   ← submits streaming I/O, sets 0x830F5820 != 0
//   sub_827DE648(ctx, base);   ← waits for 0x830F5820 == 0 (completion)
//
// sub_82192E00 marks a streaming job as "in-flight" by writing to 0x830F5820.
// On Xbox 360, hardware streaming completion workers clear this flag.
//
// With the async model (streaming_async_patches.cpp), host worker threads
// process the I/O and the completion path clears 0x830F5820 naturally.
// We let the original function run and trust the async completion path.
// A safety fallback clears the flag after a timeout in sub_827DE648.
//
// Python-verified: 0x830F0000 + 0x5820 (22560) = 0x830F5820
// =============================================================================
extern "C" void __imp__sub_82192E00(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82192E00)
{
    static int s_count = 0;
    ++s_count;

    __imp__sub_82192E00(ctx, base);

    // With async workers: I/O is dispatched to host thread pool.
    // The completion path (sub_8284BF50 -> refcount decrement -> callback)
    // will clear 0x830F5820 when the work finishes. No immediate clear needed.
    // The downstream barrier (sub_827DE648) handles polling with timeout.

    if (s_count <= 3) {
        uint32_t flagVal = PPC_LOAD_U32(0x830F5820);
        printf("[sub_82192E00] hook #%d: streaming init dispatched, "
               "0x830F5820=%u (async workers will clear)\n", s_count, flagVal);
        fflush(stdout);
    }
}

// =============================================================================
// sub_827DE648 - Streaming Completion Barrier
// Location: gta4_recomp.52.cpp:18640
//
// Original Xbox behavior: Spin-waits while 0x830F5820 != 0, expecting
// a hardware completion thread to clear the flag.
//
// Async model: Host worker threads (streaming_async_patches.cpp) process
// the I/O and the completion path clears the flag. This hook polls with
// yields, and applies a safety timeout to prevent infinite hangs if the
// async path fails to complete (e.g., during early boot when workers
// may not yet be running).
// =============================================================================
PPC_FUNC(sub_827DE648)
{
    static int s_count = 0;
    ++s_count;

    constexpr uint32_t STREAMING_PENDING = 0x830F5820;
    constexpr int MAX_POLLS = 50000;   // ~50ms at 1us per poll

    int polls = 0;
    while (PPC_LOAD_U32(STREAMING_PENDING) != 0) {
        std::this_thread::yield();
        ++polls;

        if (polls >= MAX_POLLS) {
            // Safety timeout: force-clear the flag to prevent a hang.
            // This covers the case where async workers are not yet running
            // (e.g., during early boot before pgStreamer::Init host pool start).
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
