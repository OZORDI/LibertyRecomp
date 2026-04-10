// streaming_async_patches.cpp
// Replaces the Xbox 360 pgStreamer worker model with host-native async I/O.
//
// PROBLEM:
//   pgStreamer has 2 Xbox worker threads (sub_8284C5E8) that die immediately
//   due to a race condition on worker struct initialization. The old workaround
//   forced synchronous mode (dword_830F589C=1), blocking the main thread on
//   every resource load while 32KB chunk reads complete sequentially.
//
// SOLUTION:
//   1. Force sync flag so InitInternal (sub_8284CFD8) skips Xbox thread creation
//   2. Call original init to set up handle table, ring buffers, semaphores, etc.
//   3. Launch host std::threads that run the recompiled worker function directly
//   4. Clear the runtime sync flag so the submission path uses async enqueue
//
//   The recompiled worker function (sub_8284C5E8) handles all synchronization
//   internally via guest semaphores and critical sections, which the kernel
//   import layer maps to host OS primitives. The submission function
//   (sub_8284C888) enqueues work items to ring buffers and signals the worker
//   semaphore. Our host threads wait on those same semaphores and process
//   reads through the full recompiled fiDevice chain.
//
// COMPLETION SIGNALING:
//   After each ProcessRead (sub_8284BF50) completes, we signal the streaming
//   I/O event (0x83131E10) to immediately wake any thread polling for streaming
//   completion. This replaces the Xbox 360 hardware interrupt and eliminates
//   the up-to-16ms latency of waiting for the next VdSwap frame signal.
//
// ARCHITECTURE:
//   Game thread                    Host worker threads
//   ----------                     -------------------
//   sub_8284C888 (Read)            sub_8284C5E8 (Worker loop)
//     -> validate handle              -> Wait on semaphore
//     -> inc refcounts                 -> Lock ring buffer CS
//     -> sub_8284C540 (Enqueue)        -> Dequeue work item
//       -> copy to ring buffer         -> Unlock CS
//       -> ReleaseSemaphore ---------> sub_8284BF50 (ProcessRead)
//     -> return handle to caller         -> scatter-gather via fiDevice
//                                       -> 32KB chunk reads -> rexcrt -> host OS
//                                       -> dec refcounts (completion signal)
//                                       -> SignalEventByGuestAddr(0x83131E10)

#include <cpu/ppc_context.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <kernel/kernel_sync.h>
#include <cstring>
#include <cstdio>
#include <thread>
#include <vector>
#include <atomic>

// ============================================================================
// pgStreamer global addresses
// ============================================================================
static constexpr uint32_t SYNC_FLAG_INPUT   = 0x830F589C; // checked by sub_827DF248
static constexpr uint32_t SYNC_FLAG_RUNTIME = 0x83192FF4; // checked by Read/Init

// Streaming I/O completion event — signaled after each read to wake polling code
static constexpr uint32_t STREAMING_IO_EVENT = 0x83131E10;

// Number of host worker threads — matches the 2 Xbox workers that InitInternal
// would have created (loop iterates 2 times: dword_83192FBC to dword_83192FC4).
static constexpr int NUM_HOST_WORKERS = 2;

// ============================================================================
// Host worker state
// ============================================================================
static std::vector<std::thread> g_hostWorkers;
static std::atomic<bool> g_hostWorkersStarted{false};

// Forward-declare the recompiled worker function and ProcessRead
extern "C" void __imp__sub_8284C5E8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_8284BF50(PPCContext& ctx, uint8_t* base);

// ============================================================================
// HOOK: sub_8284BF50 — pgStreamer::ProcessRead with completion signaling
//
// Wraps the original ProcessRead to signal the streaming I/O event after
// each read completes. This replaces the Xbox 360 hardware completion
// interrupt and ensures any thread waiting on streaming completion is
// woken immediately rather than waiting for the next VdSwap frame.
// ============================================================================
PPC_FUNC_IMPL(__imp__sub_8284BF50);
PPC_FUNC_HOOK(sub_8284BF50)
{
    __imp__sub_8284BF50(ctx, base);

    // Signal streaming completion — wakes polling code immediately
    SignalEventByGuestAddr(STREAMING_IO_EVENT);
}

// ============================================================================
// Host worker thread entry point
//
// Creates a PPC context and calls the recompiled pgStreamer::WorkerThread
// (sub_8284C5E8) directly. The worker loops forever:
//   1. Wait on semaphore (sub_828497D8 -> NtWaitForSingleObjectEx -> host OS)
//   2. Lock ring buffer critical section (RtlEnterCriticalSection -> host OS)
//   3. Dequeue work item, advance read index
//   4. Unlock critical section
//   5. Call ProcessRead (sub_8284BF50) -> our hook -> scatter-gather + signal
//   6. If more items queued, continue; else wait on secondary event
//   7. Exit when shutdown sentinel encountered (work item field +1548 == 0)
//
// Thread safety:
//   - Semaphores: kernel import layer maps guest handles to host OS semaphores
//   - Critical sections: RtlEnterCriticalSection/Leave -> host pthread mutexes
//   - Atomic refcounts: sub_8284E618/E640 use lwarx/stwcx (recompiled as
//     host atomic CAS via PPC_STORE_U32 with memory ordering)
//   - r13 (TLS): The priority tracking functions (sub_8284D3B0/D320) guard
//     r13 access behind dword_831AB5D8 which is 0 at boot, so r13=0 is safe
// ============================================================================
static void HostWorkerEntry(int workerIndex) {
    printf("[STREAMING] Host pgStreamer worker %d starting\n", workerIndex);
    fflush(stdout);

    PPCContext ctx{};
    uint8_t* base = g_memory.base;

    // Argument: r3 = workerIndex (0 or 1)
    ctx.r3.u32 = static_cast<uint32_t>(workerIndex);

    // Stack pointer: sub_8284C5E8 uses a 160-byte frame, and calls
    // sub_8284BF50 which allocates ~32KB (stwux with r12=0xFFFF7F10).
    // Provide 128KB per worker in low guest address space.
    static constexpr uint32_t WORKER_STACK_BASE = 0x7F000000;
    static constexpr uint32_t WORKER_STACK_SIZE = 0x20000; // 128KB
    ctx.r1.u32 = WORKER_STACK_BASE + WORKER_STACK_SIZE * (workerIndex + 1) - 16;

    // r13 = TLS base. Set to 0 — safe because sub_8284D3B0/D320 check
    // dword_831AB5D8 (BSS, zero-initialized) before accessing r13.
    ctx.r13.u64 = 0;

    // Run the recompiled worker loop — blocks on semaphore between work items
    __imp__sub_8284C5E8(ctx, base);

    printf("[STREAMING] Host pgStreamer worker %d exited\n", workerIndex);
    fflush(stdout);
}

static void StartHostWorkers() {
    if (g_hostWorkersStarted.exchange(true))
        return;

    g_hostWorkers.reserve(NUM_HOST_WORKERS);
    for (int i = 0; i < NUM_HOST_WORKERS; i++) {
        g_hostWorkers.emplace_back(HostWorkerEntry, i);
    }
    printf("[STREAMING] Started %d host pgStreamer workers\n", NUM_HOST_WORKERS);
    fflush(stdout);
}

// Cleanup on process exit — workers exit via shutdown sentinel or we detach
__attribute__((destructor))
static void StopHostWorkers() {
    if (!g_hostWorkersStarted.load())
        return;

    for (auto& t : g_hostWorkers) {
        if (t.joinable())
            t.detach();
    }
    g_hostWorkers.clear();
}

// ============================================================================
// HOOK: sub_827DF248 — pgStreamer init entry point
//
// Replaces the old sync-mode workaround in imports.cpp.
// Sequence:
//   1. Set input sync flag (0x830F589C = 1) so InitInternal skips Xbox threads
//   2. Call original init — allocates handle table, ring buffers, semaphores
//   3. Launch host worker threads that run recompiled sub_8284C5E8
//   4. Clear runtime sync flag (0x83192FF4 = 0) so Read takes async path
// ============================================================================
extern "C" void __imp__sub_827DF248(PPCContext& ctx, uint8_t* base);

PPC_FUNC_HOOK(sub_827DF248) {
    // Step 1: Force sync so InitInternal skips Xbox worker thread creation
    *reinterpret_cast<be<uint32_t>*>(g_memory.Translate(SYNC_FLAG_INPUT)) = 1;

    printf("[STREAMING] pgStreamer::Init — sync flag set, calling original init\n");
    fflush(stdout);

    // Step 2: Run original init (handle table, CritSecs, semaphores — no Xbox threads)
    __imp__sub_827DF248(ctx, base);

    // Step 3: Launch host workers that consume from the ring buffers
    StartHostWorkers();

    // Step 4: Clear runtime sync flag -> submission path (sub_8284C888) will
    // enqueue to ring buffers + signal semaphores instead of calling inline
    *reinterpret_cast<be<uint32_t>*>(g_memory.Translate(SYNC_FLAG_RUNTIME)) = 0;

    printf("[STREAMING] pgStreamer::Init complete — host async mode active\n");
    fflush(stdout);
}
