// streaming_diagnostics.cpp
// Lightweight streaming diagnostics for measuring I/O throughput, latency,
// handle usage, queue depth, and scene state transitions.
//
// Hooked functions:
//   sub_821D00F0 = RequestLoad         (176 callers, leaf, 40 bytes)
//   sub_8284CB80 = pgStreamer::Open     (6 callers, 696 bytes)
//   sub_8284BF50 = pgStreamer::Read     (2 callers, 832 bytes, core I/O)
//   sub_825132D0 = ProcessQueue         (3 callers, 1008 bytes)
//   sub_821EC8C8 = SceneStateMachine    (1 caller, 2600 bytes)
//
// All counters are std::atomic for thread safety. Summaries log every 5 seconds.
// Toggle via g_streamingDiagEnabled (default: true in debug, false in release).

#include <kernel/function.h>
#include <kernel/memory.h>
#include <kernel/kernel_sync.h>
#include <os/logger.h>
#include <fmt/format.h>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <mutex>

// =============================================================================
// Global toggle
// =============================================================================

#ifdef _DEBUG
static std::atomic<bool> g_streamingDiagEnabled{true};
#else
static std::atomic<bool> g_streamingDiagEnabled{false};
#endif

// =============================================================================
// Diagnostic counters (all atomic for cross-thread safety)
// =============================================================================

namespace StreamDiag {

// --- RequestLoad (sub_821D00F0) ---
static std::atomic<uint64_t> s_requestCount{0};       // total requests
static std::atomic<uint64_t> s_requestWindowCount{0};  // requests in current window

// --- pgStreamer::Open (sub_8284CB80) ---
static std::atomic<int32_t>  s_openHandles{0};         // currently open handles
static std::atomic<int32_t>  s_peakHandles{0};         // peak open handles
static std::atomic<uint64_t> s_openCount{0};           // total opens
static std::atomic<uint64_t> s_openFailCount{0};       // total open failures

// --- pgStreamer::Read (sub_8284BF50) ---
static std::atomic<uint64_t> s_readCount{0};           // total reads
static std::atomic<uint64_t> s_readBytes{0};           // total bytes read
static std::atomic<uint64_t> s_readTimeNs{0};          // total read time (nanoseconds)
static std::atomic<uint32_t> s_peakReadSize{0};        // largest single read

// --- ProcessQueue (sub_825132D0) ---
static std::atomic<uint64_t> s_queueProcessCount{0};   // total queue process calls
static std::atomic<uint64_t> s_queueTimeNs{0};         // total processing time (ns)
static std::atomic<uint32_t> s_peakQueueDepth{0};      // peak items seen in queue

// --- Scene state (sub_821EC8C8) ---
static std::atomic<int32_t>  s_sceneState{-1};         // current state
static std::atomic<uint64_t> s_stateTransitions{0};    // total transitions

// --- Timing ---
using Clock = std::chrono::steady_clock;
static Clock::time_point s_lastReport = Clock::now();
static Clock::time_point s_windowStart = Clock::now();
static std::mutex s_reportMtx;

// Helper: update an atomic max
template<typename T>
static void atomicMax(std::atomic<T>& target, T value) {
    T prev = target.load(std::memory_order_relaxed);
    while (prev < value && !target.compare_exchange_weak(prev, value,
           std::memory_order_relaxed, std::memory_order_relaxed)) {}
}

// =============================================================================
// Periodic summary reporter
// =============================================================================

static void MaybeReport() {
    auto now = Clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - s_lastReport).count();

    if (elapsed < 5000) return;

    // Avoid multiple threads racing to report
    std::unique_lock<std::mutex> lock(s_reportMtx, std::try_to_lock);
    if (!lock.owns_lock()) return;

    // Re-check after lock
    now = Clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - s_lastReport).count();
    if (elapsed < 5000) return;

    double elapsedSec = elapsed / 1000.0;

    // Requests per second
    uint64_t windowReqs = s_requestWindowCount.exchange(0, std::memory_order_relaxed);
    double reqsPerSec = (elapsedSec > 0.0) ? windowReqs / elapsedSec : 0.0;

    // I/O throughput
    uint64_t readBytes = s_readBytes.load(std::memory_order_relaxed);
    uint64_t readCount = s_readCount.load(std::memory_order_relaxed);
    uint64_t readTimeNs = s_readTimeNs.load(std::memory_order_relaxed);
    double avgReadUs = (readCount > 0) ? (readTimeNs / 1000.0) / readCount : 0.0;
    double throughputMBs = (readTimeNs > 0)
        ? (readBytes / (1024.0 * 1024.0)) / (readTimeNs / 1e9) : 0.0;

    // Queue stats
    uint64_t queueCalls = s_queueProcessCount.load(std::memory_order_relaxed);
    uint64_t queueTimeNs = s_queueTimeNs.load(std::memory_order_relaxed);
    double avgQueueMs = (queueCalls > 0) ? (queueTimeNs / 1e6) / queueCalls : 0.0;

    LOGF_INFO("[StreamDiag] reqs/s={:.1f} total={} | "
              "I/O: reads={} bytes={} avg={:.1f}us tput={:.1f}MB/s peak={}B | "
              "handles: cur={} peak={} opens={} fails={} | "
              "queue: calls={} avg={:.2f}ms peakDepth={} | "
              "scene: state={} transitions={}",
        reqsPerSec, s_requestCount.load(std::memory_order_relaxed),
        readCount, readBytes, avgReadUs, throughputMBs,
        s_peakReadSize.load(std::memory_order_relaxed),
        s_openHandles.load(std::memory_order_relaxed),
        s_peakHandles.load(std::memory_order_relaxed),
        s_openCount.load(std::memory_order_relaxed),
        s_openFailCount.load(std::memory_order_relaxed),
        queueCalls, avgQueueMs,
        s_peakQueueDepth.load(std::memory_order_relaxed),
        s_sceneState.load(std::memory_order_relaxed),
        s_stateTransitions.load(std::memory_order_relaxed));

    s_lastReport = now;
}

} // namespace StreamDiag

// =============================================================================
// Hook: sub_821D00F0 — RequestLoad (leaf function, 40 bytes, 176 callers)
// =============================================================================
// Original: indexes into streaming request table and tail-calls sub_825120E8
// Args: r3 = queue ptr, r4 = slot index, r5 = flags

PPC_FUNC_IMPL(__imp__sub_821D00F0);
PPC_FUNC_HOOK(sub_821D00F0)
{
    if (g_streamingDiagEnabled.load(std::memory_order_relaxed)) {
        StreamDiag::s_requestCount.fetch_add(1, std::memory_order_relaxed);
        StreamDiag::s_requestWindowCount.fetch_add(1, std::memory_order_relaxed);
    }

    // Call original
    __imp__sub_821D00F0(ctx, base);

    if (g_streamingDiagEnabled.load(std::memory_order_relaxed)) {
        StreamDiag::MaybeReport();
    }
}

// =============================================================================
// Hook: sub_8284CB80 — pgStreamer::Open (696 bytes, 6 callers)
// =============================================================================
// Opens a streaming handle. Returns handle index in r3, or -1 on failure.
// Args: r3 = filename ptr, r4 = mode, r5 = async flag

PPC_FUNC_IMPL(__imp__sub_8284CB80);
PPC_FUNC_HOOK(sub_8284CB80)
{
    // Call original
    __imp__sub_8284CB80(ctx, base);

    if (g_streamingDiagEnabled.load(std::memory_order_relaxed)) {
        int32_t result = static_cast<int32_t>(ctx.r3.s32);
        if (result >= 0) {
            int32_t cur = StreamDiag::s_openHandles.fetch_add(1, std::memory_order_relaxed) + 1;
            StreamDiag::atomicMax(StreamDiag::s_peakHandles, cur);
            StreamDiag::s_openCount.fetch_add(1, std::memory_order_relaxed);
        } else {
            StreamDiag::s_openFailCount.fetch_add(1, std::memory_order_relaxed);
        }
    }
}

// =============================================================================
// Hook: sub_8284BF50 — pgStreamer::Read / core I/O (832 bytes, 2 callers)
// =============================================================================
// Performs the actual disk read through the device vtable.
// Args: r3 = streamer context, r4 = buffer index
// We measure wall-clock time around the original call and track bytes from
// the read size field in the request entry.

// Streaming I/O event address — signaled after each read completes to wake
// any thread polling for streaming completion (e.g., the barrier at sub_827DE648).
// This replaces the Xbox 360 hardware completion interrupt.
static constexpr uint32_t STREAMING_IO_EVENT = 0x83131E10;

PPC_FUNC_IMPL(__imp__sub_8284BF50);
PPC_FUNC_HOOK(sub_8284BF50)
{
    if (!g_streamingDiagEnabled.load(std::memory_order_relaxed)) {
        __imp__sub_8284BF50(ctx, base);
        // Signal streaming completion — wakes polling code immediately
        // rather than waiting for the next VdSwap frame signal.
        SignalEventByGuestAddr(STREAMING_IO_EVENT);
        return;
    }

    // Read the request size from the streamer context before the call.
    // The context at r3 has field at offset +1540 (request count) and
    // the read size is derived from the entry's flags field.
    // We approximate by measuring pre/post and using the buffer index field.
    uint32_t streamerCtx = ctx.r3.u32;
    uint32_t requestCount = PPC_LOAD_U32(streamerCtx + 1540);

    auto start = StreamDiag::Clock::now();

    __imp__sub_8284BF50(ctx, base);

    auto end = StreamDiag::Clock::now();
    uint64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    StreamDiag::s_readCount.fetch_add(1, std::memory_order_relaxed);
    StreamDiag::s_readTimeNs.fetch_add(ns, std::memory_order_relaxed);

    // Estimate bytes read from the request entries (each entry is 12 bytes at offset +8)
    // The actual byte count is tracked by summing individual scatter reads inside the
    // original function. We use requestCount as a proxy for work done.
    if (requestCount > 0) {
        // Each scatter-gather entry reads up to 32KB (0x8000)
        uint32_t estimatedBytes = requestCount * 32768u;
        StreamDiag::s_readBytes.fetch_add(estimatedBytes, std::memory_order_relaxed);
        StreamDiag::atomicMax(StreamDiag::s_peakReadSize, estimatedBytes);
    }

    // Signal streaming I/O completion event — this replaces the Xbox 360
    // hardware interrupt that would wake the streaming coordinator after
    // each file read. Without this, the coordinator relies on the per-frame
    // VdSwap signal which adds up to 16ms latency per streaming operation.
    SignalEventByGuestAddr(STREAMING_IO_EVENT);
}

// =============================================================================
// Hook: sub_825132D0 — ProcessQueue (1008 bytes, 3 callers, recursive)
// =============================================================================
// Processes the streaming request queue. We track call count, timing, and
// approximate queue depth from the linked-list traversal.
// Args: r3 = queue object, r4 = process mode, r5 = filter flag

PPC_FUNC_IMPL(__imp__sub_825132D0);
PPC_FUNC_HOOK(sub_825132D0)
{
    if (!g_streamingDiagEnabled.load(std::memory_order_relaxed)) {
        __imp__sub_825132D0(ctx, base);
        return;
    }

    // Read queue depth: queue object at r3, linked list head at offset +16,
    // end sentinel at offset +20. If head != end, items are queued.
    uint32_t queueObj = ctx.r3.u32;
    uint32_t head = PPC_LOAD_U32(queueObj + 16);
    uint32_t end  = PPC_LOAD_U32(queueObj + 20);

    // Estimate depth by walking the linked list (capped at 256 to avoid infinite loops)
    uint32_t depth = 0;
    if (head != 0 && head != end) {
        uint32_t node = head;
        while (node != 0 && node != end && depth < 256) {
            // Each node has a "next" pointer at offset +16 (from the recomp scaffold)
            uint32_t nextField = PPC_LOAD_U16(node + 16);
            if (nextField == 0xFFFF) break;
            depth++;
            // Advance: next node = base + index * 24
            uint32_t baseArr = PPC_LOAD_U32(queueObj + 0);
            node = baseArr + nextField * 24;
        }
    }
    StreamDiag::atomicMax(StreamDiag::s_peakQueueDepth, depth);

    auto start = StreamDiag::Clock::now();

    __imp__sub_825132D0(ctx, base);

    auto end_time = StreamDiag::Clock::now();
    uint64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start).count();

    StreamDiag::s_queueProcessCount.fetch_add(1, std::memory_order_relaxed);
    StreamDiag::s_queueTimeNs.fetch_add(ns, std::memory_order_relaxed);
}

// =============================================================================
// Hook: sub_821EC8C8 — Scene State Machine (2600 bytes, 1 caller)
// =============================================================================
// The scene LOD/loading state machine. State is at 0x820977F0.
// States observed in recomp: 0-6, 9, 10, with transitions stored back to
// the same address. We track transitions and current state.

PPC_FUNC_IMPL(__imp__sub_821EC8C8);
PPC_FUNC_HOOK(sub_821EC8C8)
{
    static constexpr uint32_t ADDR_SCENE_STATE = 0x820977F0;

    int32_t prevState = -1;
    if (g_streamingDiagEnabled.load(std::memory_order_relaxed)) {
        prevState = static_cast<int32_t>(PPC_LOAD_U32(ADDR_SCENE_STATE));
    }

    __imp__sub_821EC8C8(ctx, base);

    if (g_streamingDiagEnabled.load(std::memory_order_relaxed)) {
        int32_t newState = static_cast<int32_t>(PPC_LOAD_U32(ADDR_SCENE_STATE));
        StreamDiag::s_sceneState.store(newState, std::memory_order_relaxed);

        if (newState != prevState) {
            StreamDiag::s_stateTransitions.fetch_add(1, std::memory_order_relaxed);
            LOGF_INFO("[StreamDiag] Scene state: {} -> {}", prevState, newState);
        }
    }
}
