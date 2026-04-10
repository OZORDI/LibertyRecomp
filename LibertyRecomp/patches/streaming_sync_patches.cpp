// =============================================================================
// Audio Render Thread Sync Optimization
// =============================================================================
//
// Replaces heavyweight Xbox 360 kernel sync primitives in the audio render
// pipeline with lightweight host-native equivalents:
//
//   sub_821910D0 — Render pump: replaces lwarx/stwcx global-lock-based atomic
//                  increment with a plain increment (already under critsec).
//                  Keeps KeSetEvent/KeWaitForMultipleObjects via rexglue since
//                  sub_82191228 internally signals the same kernel objects.
//
//   sub_8218FFB0 — Buffer drain: replaces KeRaiseIrqlToDpcLevel +
//                  KeAcquireSpinLockAtRaisedIrql + recursive lock tracking
//                  with a simple std::mutex. The IRQL machinery is meaningless
//                  on host (no interrupt priority levels) and the spinlock +
//                  recursion tracking adds ~6 kernel calls per invocation.
//
// Functions NOT hooked (and why):
//   sub_821909D0 — Primary worker: calls sub_82191228 which internally calls
//                  KeReleaseSemaphore on the BSS KSEMAPHORE. If we replaced
//                  the worker's wait with a host semaphore, the semaphore
//                  release inside sub_82191228 would signal the kernel object
//                  instead of the host one, deadlocking secondaries.
//   sub_82190A98 — Secondary workers: same coupling issue via sub_82191228.
//   sub_82190B48 — Init: creates kernel objects that sub_82191228 uses.
//
// The two hooked functions account for the main optimization wins:
//   - sub_821910D0: eliminates PPC_ENTER_GLOBAL_LOCK (process-wide recursive
//     mutex) for what is just a single increment already protected by critsec
//   - sub_8218FFB0: eliminates 6 kernel calls (RaiseIrql, AcquireSpinLock,
//     recursion tracking x2, ReleaseSpinLock, LowerIrql) per audio frame
//
// =============================================================================

#include <stdafx.h>
#include <api/Liberty.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <apu/rage_audio_device.h>
#include <os/logger.h>
#include <mutex>
#include <cstring>

using namespace rage::aud;

// Forward declarations for generated recomp functions we call.
// __imp__ prefix = original implementation (bypasses our hook).
// Plain prefix = weak symbol (resolves to our hook if defined, else original).
PPC_FUNC_IMPL(__imp__sub_821910D0);  // original render pump (not called, but declared for completeness)
PPC_FUNC_IMPL(sub_82190120);         // buffer swap (not hooked, called from our sub_8218FFB0 hook)
PPC_FUNC_IMPL(sub_82191228);         // voice update (not hooked, called from sub_821910D0 inline path)
// Note: sub_8218FFB0 is defined in this file (PPC_FUNC_HOOK) so no forward decl needed.
// The sub_821910D0 inline path calls it and sees the definition above.

// =============================================================================
// Host-side sync primitive replacing IRQL spinlock
// =============================================================================
// The original code at 0x831E4DB8 uses:
//   KeRaiseIrqlToDpcLevel() -> save old IRQL
//   if (!recursion || owner != currentThread)
//     KeAcquireSpinLockAtRaisedIrql(&g_audioSpinLock)
//     owner = currentThread
//   recursion++
//   ... work ...
//   recursion--
//   if (recursion == 0)
//     KeReleaseSpinLockFromRaisedIrql(&g_audioSpinLock)
//     KfLowerIrql(savedIrql)
//
// On host there is no IRQL concept. The recursive spinlock with ownership
// tracking is replaced by std::recursive_mutex which provides the same
// semantics (recursive acquisition by the same thread) without IRQL overhead.
// =============================================================================
namespace {
    std::recursive_mutex g_audioSpinMtx;
} // anonymous namespace

// =============================================================================
// sub_8218FFB0 — Buffer drain (ready/free queue swap)
// =============================================================================
// Original flow (per-frame, called from primary worker and render pump):
//   1. KeRaiseIrqlToDpcLevel()           — kernel call, sets IRQL state
//   2. Check recursion counter at 0x831E4DBC
//   3. If not recursive: KeAcquireSpinLockAtRaisedIrql(0x831E4DB8)
//   4. Set owner = currentThread at 0x831E4DC0
//   5. Save old IRQL at 0x831E4DC4
//   6. Increment recursion counter
//   7. sub_82190120(device+80)           — swap master voice buffers
//   8. For each extra voice: sub_82190120(extraBase + i*44)
//   9. Decrement recursion counter
//  10. If recursion == 0: KeReleaseSpinLockFromRaisedIrql, KfLowerIrql
//
// Replacement: recursive_mutex lock -> buffer swaps -> unlock
// Saves 6 kernel import calls per audio frame.
// =============================================================================
PPC_FUNC_HOOK(sub_8218FFB0) {
    uint32_t a1 = ctx.r3.u32;  // audDevice*

    // Recursive mutex replaces IRQL raise + spinlock + ownership tracking
    g_audioSpinMtx.lock();

    // Call sub_82190120 (buffer swap) on master voice at device+80
    ctx.r3.u32 = a1 + 80;
    ctx.lr = 0;
    sub_82190120(ctx, base);

    // Swap extra voices (each audRenderState is 44 bytes)
    uint8_t numExtra = PPC_LOAD_U8(a1 + 128);
    if (numExtra > 0) {
        uint32_t extraBase = PPC_LOAD_U32(a1 + 124);
        for (uint32_t i = 0; i < numExtra; ++i) {
            ctx.r3.u32 = extraBase + i * 44;
            ctx.lr = 0;
            sub_82190120(ctx, base);
        }
    }

    // Unlock replaces spinlock release + IRQL lower
    g_audioSpinMtx.unlock();
}

// =============================================================================
// sub_821910D0 — Render pump (main thread, called each audio frame)
// =============================================================================
// Original flow:
//   1. RtlEnterCriticalSection(0x82B2833C)
//   2. Store callerThread at device+300
//   3. If numWorkers > 0:
//      a. KeSetEvent(g_renderWakeEvent)
//      b. KeWaitForMultipleObjects({g_shutdownDoneEvent, g_shutdownCompleteEvent})
//   4. Else: inline drain + voice update
//   5. If not shutdown AND endpoint.Present() >= 0:
//      a. mfmsr r7                        — read MSR
//      b. mtmsrd r13,1                    — disable interrupts
//      c. lwarx r9,0,r11                  — load-reserved frontBuffer[0]
//      d. add r8 = r9 + 1
//      e. stwcx. r8,0,r11                — store-conditional
//      f. mtmsrd r7,1                     — restore MSR
//      g. retry if stwcx failed
//      This translates to PPC_ENTER_GLOBAL_LOCK / PPC_LEAVE_GLOBAL_LOCK
//      which is a PROCESS-WIDE recursive mutex just for fetch_add(1).
//   6. Clear device+300
//   7. Copy frontBuffer[0..6] -> backBuffer[0..6], zero frontBuffer
//   8. RtlLeaveCriticalSection
//
// Replacement:
//   - Steps 1-4: keep as-is (critsec via rexcrt, events via rexglue)
//   - Step 5: replace global lock atomic with plain increment. This is safe
//     because we're already inside the critical section (step 1) which
//     provides exclusive access to frontBuffer.
//   - Steps 6-8: keep as-is
//
// Savings: eliminates PPC_ENTER_GLOBAL_LOCK + PPC_LEAVE_GLOBAL_LOCK per frame
// (each is mutex lock/unlock + atomic counter + fence). The critsec already
// ensures no concurrent access.
// =============================================================================
PPC_FUNC_HOOK(sub_821910D0) {
    bool shutdownOccurred = false;
    uint32_t devicePtr = PPC_LOAD_U32(ADDR_DEVICE_PTR);

    // Enter critical section (rexcrt-hooked to std::mutex)
    ctx.r3.u32 = ADDR_CRITSEC;
    ctx.lr = 0;
    __imp__RtlEnterCriticalSection(ctx, base);

    uint32_t numWorkers = PPC_LOAD_U32(devicePtr + 304);
    uint32_t callerThread = PPC_LOAD_U32(ctx.r13.u32 + 256);

    // Store caller thread ID at device+300 (nonzero = work, 0 = shutdown)
    PPC_STORE_U32(devicePtr + 300, callerThread);

    if (numWorkers > 0) {
        // Signal primary worker via g_renderWakeEvent (rexglue KEVENT)
        ctx.r3.u32 = ADDR_RENDER_WAKE;
        ctx.r4.u32 = 1;   // increment
        ctx.r5.u32 = 0;   // wait = false
        ctx.lr = 0;
        __imp__KeSetEvent(ctx, base);

        // Wait for completion: {g_shutdownDoneEvent, g_shutdownCompleteEvent}
        // WaitType=1 (WaitAny), count=2
        // Build the object pointer array on the PPC stack
        uint32_t sp = ctx.r1.u32;
        // Allocate stack frame for wait block + object array
        ctx.r1.u32 = sp - 192;
        PPC_STORE_U32(ctx.r1.u32, sp);  // back-chain

        // Object array at sp-192+96 = sp-96
        uint32_t objArrayAddr = ctx.r1.u32 + 80;
        PPC_STORE_U32(objArrayAddr + 0, ADDR_SHUTDOWN_DONE);     // object[0]
        PPC_STORE_U32(objArrayAddr + 4, ADDR_SHUTDOWN_COMPLETE); // object[1]

        // Zero the wait block area (44 bytes, same as original sub_829FF840 call)
        uint32_t waitBlockAddr = ctx.r1.u32 + 100;
        for (uint32_t i = 0; i < 44; i += 4)
            PPC_STORE_U32(waitBlockAddr + i, 0);

        // KeWaitForMultipleObjects(2, objects, WaitAny=1, UserRequest=3,
        //                          UserMode=1, alertable=0, timeout=0, waitBlocks)
        ctx.r3.u32 = 2;                        // count
        ctx.r4.u32 = objArrayAddr;              // objects
        ctx.r5.u32 = 1;                         // WaitAny
        ctx.r6.u32 = 3;                         // UserRequest
        ctx.r7.u32 = 1;                         // UserMode
        ctx.r8.u32 = 0;                         // alertable
        ctx.r9.u32 = 0;                         // timeout (infinite)
        ctx.r10.u32 = ctx.r1.u32 + 96;         // wait block storage
        // Store remaining args where the ABI expects them
        PPC_STORE_U32(ctx.r1.u32 + 96, 0);     // wait status init
        ctx.lr = 0;
        __imp__KeWaitForMultipleObjects(ctx, base);

        // Result: 0 = shutdownDone signaled, 1 = shutdownComplete signaled
        shutdownOccurred = (ctx.r3.s32 == 1);

        // Restore stack
        ctx.r1.u32 = sp;
    } else {
        // No worker threads — do work inline (same as original fallback)
        ctx.r3.u32 = devicePtr;
        ctx.lr = 0;
        sub_8218FFB0(ctx, base);

        ctx.r3.u32 = devicePtr;
        ctx.r4.u32 = 1;
        ctx.lr = 0;
        sub_82191228(ctx, base);
    }

    // Atomic increment of g_frontBuffer[0] — THE KEY OPTIMIZATION
    // Original: PPC_ENTER_GLOBAL_LOCK -> lwarx/stwcx -> PPC_LEAVE_GLOBAL_LOCK
    // Replacement: plain increment (we hold the critsec, no contention possible)
    if (!shutdownOccurred) {
        // Call endpoint vtable[17] (Present) to check readiness
        uint32_t endpointObj = PPC_LOAD_U32(devicePtr + 64);
        if (endpointObj != 0) {
            uint32_t endpointVtable = PPC_LOAD_U32(endpointObj);
            uint32_t presentFunc = PPC_LOAD_U32(endpointVtable + 68);

            ctx.r3.u32 = endpointObj;
            ctx.lr = 0;
            PPC_CALL_INDIRECT_FUNC(presentFunc);

            if (ctx.r3.s32 >= 0) {
                // Plain increment — safe under critsec, no global lock needed
                uint32_t val = PPC_LOAD_U32(ADDR_FRONT_BUFFER);
                PPC_STORE_U32(ADDR_FRONT_BUFFER, val + 1);
            }
        }
    }

    // Clear caller thread ID (signals shutdown to workers)
    PPC_STORE_U32(devicePtr + 300, 0);

    // Copy frontBuffer -> backBuffer, zero frontBuffer (7-element double buffer)
    for (uint32_t i = 0; i < NUM_DOUBLE_BUFS; ++i) {
        uint32_t val = PPC_LOAD_U32(ADDR_FRONT_BUFFER + i * 4);
        PPC_STORE_U32(ADDR_FRONT_BUFFER + i * 4, 0);
        PPC_STORE_U32(ADDR_BACK_BUFFER + i * 4, val);
    }

    // Leave critical section
    ctx.r3.u32 = ADDR_CRITSEC;
    ctx.lr = 0;
    __imp__RtlLeaveCriticalSection(ctx, base);

    ctx.r3.u32 = 0;
}
