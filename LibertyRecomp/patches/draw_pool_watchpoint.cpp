// =============================================================================
// Draw-Pool Watchpoint Instrumentation
// =============================================================================
// Pure observation, no fixes. Tracks the lifecycle of the singleton pointer
// at *0x831C22A4 (the "mgr" field read by sub_828BF270 and dereferenced by
// sub_82A3DF50). Background investigation of the render-thread UAF that
// crashes with r9=0xFFE1E1E1 (RAGE freed-heap poison).
//
// Watch surface:
//   - WATCH_ADDR (0x831C22A4): the mgr pointer that becomes 0xFFE1E1E1
//   - SRC1_ADDR  (0x831C23E4): writer 1's source slot (sub_828C01E0)
//   - SRC2_ADDR  (0x831C2294): writer 2's source slot (sub_828C0338)
//
// Hooks:
//   sub_828BF270   READ site (reads WATCH_ADDR, tail-calls sub_82A3DF50)
//   sub_828C01E0   WRITER 1 — copies SRC1_ADDR -> WATCH_ADDR
//   sub_828C0338   WRITER 2 — copies SRC2_ADDR -> WATCH_ADDR
//   sub_828C47E8   PARENT   — only function that calls the two writers
//   sub_828D4C88   GRANDPARENT — only caller of sub_828C47E8
//
// Background poller: samples WATCH_ADDR every 5ms; logs only on transition.
// =============================================================================

#include <api/Liberty.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>
#include <rex/diagnostics/policy.h>
#include <atomic>
#include <thread>
#include <chrono>

namespace {

constexpr uint32_t WATCH_ADDR = 0x831C22A4;
constexpr uint32_t SRC1_ADDR  = 0x831C23E4;  // 0x831C0000 + 9188
constexpr uint32_t SRC2_ADDR  = 0x831C2294;  // 0x831C0000 + 8852
constexpr uint32_t POISON     = 0xFFE1E1E1;

inline uint32_t ReadGuestU32(uint32_t guest_addr) {
    auto* host = static_cast<uint32_t*>(g_memory.Translate(guest_addr));
    return __builtin_bswap32(*host);
}

std::atomic<uint64_t> g_event_seq{0};
std::atomic<bool>     g_poller_started{false};
std::atomic<bool>     g_poller_stop{false};

bool WatchDiagnosticsEnabled() noexcept {
    return rex::diagnostics::IsEnabled(
               rex::diagnostics::Category::kGuestHooks) &&
           rex::diagnostics::IsEnabled(
               rex::diagnostics::Category::kLogging);
}

const char* PoisonTag(uint32_t v) {
    if (v == POISON)        return " *** POISON ***";
    if (v == 0)             return " (null)";
    if (v < 0x80000000u)    return " *** SUSPICIOUS LOW ***";
    return "";
}

void PollerLoop() {
    using namespace std::chrono_literals;
    uint32_t last = ReadGuestU32(WATCH_ADDR);
    LOGF_WARNING("[WATCH] Poller START — initial *0x{:08X} = 0x{:08X}{}",
                 WATCH_ADDR, last, PoisonTag(last));
    while (!g_poller_stop.load(std::memory_order_relaxed)) {
        std::this_thread::sleep_for(5ms);
        uint32_t now = ReadGuestU32(WATCH_ADDR);
        if (now != last) {
            uint64_t seq = g_event_seq.fetch_add(1, std::memory_order_relaxed);
            LOGF_WARNING("[WATCH] seq={} POLL TRANSITION *0x{:08X}: 0x{:08X} -> 0x{:08X}{}",
                         seq, WATCH_ADDR, last, now, PoisonTag(now));
            last = now;
        }
    }
}

void EnsurePollerStarted() {
    if (!WatchDiagnosticsEnabled()) {
        return;
    }
    bool expected = false;
    if (g_poller_started.compare_exchange_strong(expected, true)) {
        std::thread(PollerLoop).detach();
    }
}

}  // namespace

// ============================================================================
// READ SITE: sub_828BF270
//   lis r11, -31972          ; r11 = 0x831C0000
//   lwz r3, 8868(r11)        ; r3 = *0x831C22A4   <-- READ
//   b   sub_82A3DF50         ; tail call → deref of r3
// ============================================================================
PPC_FUNC_IMPL(__imp__sub_828BF270);
PPC_FUNC_HOOK(sub_828BF270) {
    if (!WatchDiagnosticsEnabled()) {
        __imp__sub_828BF270(ctx, base);
        return;
    }
    EnsurePollerStarted();
    uint32_t value = ReadGuestU32(WATCH_ADDR);
    uint64_t seq   = g_event_seq.fetch_add(1, std::memory_order_relaxed);
    LOGF_WARNING("[WATCH] seq={} READ  sub_828BF270 LR=0x{:08X} *0x{:08X}=0x{:08X}{}",
                 seq, ctx.lr, WATCH_ADDR, value, PoisonTag(value));
    __imp__sub_828BF270(ctx, base);
}

// ============================================================================
// WRITER 1: sub_828C01E0
//   ...stw r3, 8868(r10)... where r3 was loaded from r10+9188 (= 0x831C23E4)
// ============================================================================
PPC_FUNC_IMPL(__imp__sub_828C01E0);
PPC_FUNC_HOOK(sub_828C01E0) {
    if (!WatchDiagnosticsEnabled()) {
        __imp__sub_828C01E0(ctx, base);
        return;
    }
    EnsurePollerStarted();
    uint32_t before_dst = ReadGuestU32(WATCH_ADDR);
    uint32_t before_src = ReadGuestU32(SRC1_ADDR);
    uint64_t seq_in = g_event_seq.fetch_add(1, std::memory_order_relaxed);
    LOGF_WARNING("[WATCH] seq={} W1-IN sub_828C01E0 LR=0x{:08X} src(0x{:08X})=0x{:08X}{} dst(0x{:08X})=0x{:08X}{}",
                 seq_in, ctx.lr,
                 SRC1_ADDR,  before_src, PoisonTag(before_src),
                 WATCH_ADDR, before_dst, PoisonTag(before_dst));
    __imp__sub_828C01E0(ctx, base);
    uint32_t after_dst = ReadGuestU32(WATCH_ADDR);
    uint64_t seq_out = g_event_seq.fetch_add(1, std::memory_order_relaxed);
    LOGF_WARNING("[WATCH] seq={} W1-OUT sub_828C01E0 dst(0x{:08X}) {} -> 0x{:08X}{}",
                 seq_out, WATCH_ADDR,
                 (before_dst != after_dst) ? "CHANGED" : "same",
                 after_dst, PoisonTag(after_dst));
}

// ============================================================================
// WRITER 2: sub_828C0338
//   ...lwz r11, 8852(r11); stw r11, 8868(r10)... copies SRC2 -> WATCH
// ============================================================================
PPC_FUNC_IMPL(__imp__sub_828C0338);
PPC_FUNC_HOOK(sub_828C0338) {
    if (!WatchDiagnosticsEnabled()) {
        __imp__sub_828C0338(ctx, base);
        return;
    }
    EnsurePollerStarted();
    uint32_t before_dst = ReadGuestU32(WATCH_ADDR);
    uint32_t before_src = ReadGuestU32(SRC2_ADDR);
    uint64_t seq_in = g_event_seq.fetch_add(1, std::memory_order_relaxed);
    LOGF_WARNING("[WATCH] seq={} W2-IN sub_828C0338 LR=0x{:08X} src(0x{:08X})=0x{:08X}{} dst(0x{:08X})=0x{:08X}{}",
                 seq_in, ctx.lr,
                 SRC2_ADDR,  before_src, PoisonTag(before_src),
                 WATCH_ADDR, before_dst, PoisonTag(before_dst));
    __imp__sub_828C0338(ctx, base);
    uint32_t after_dst = ReadGuestU32(WATCH_ADDR);
    uint64_t seq_out = g_event_seq.fetch_add(1, std::memory_order_relaxed);
    LOGF_WARNING("[WATCH] seq={} W2-OUT sub_828C0338 dst(0x{:08X}) {} -> 0x{:08X}{}",
                 seq_out, WATCH_ADDR,
                 (before_dst != after_dst) ? "CHANGED" : "same",
                 after_dst, PoisonTag(after_dst));
}

// ============================================================================
// PARENT: sub_828C47E8 — only function that invokes both writers
// ============================================================================
PPC_FUNC_IMPL(__imp__sub_828C47E8);
PPC_FUNC_HOOK(sub_828C47E8) {
    if (!WatchDiagnosticsEnabled()) {
        __imp__sub_828C47E8(ctx, base);
        return;
    }
    EnsurePollerStarted();
    uint32_t before = ReadGuestU32(WATCH_ADDR);
    uint64_t seq_in = g_event_seq.fetch_add(1, std::memory_order_relaxed);
    LOGF_WARNING("[WATCH] seq={} PA-IN sub_828C47E8 LR=0x{:08X} *0x{:08X}=0x{:08X}{}",
                 seq_in, ctx.lr, WATCH_ADDR, before, PoisonTag(before));
    __imp__sub_828C47E8(ctx, base);
    uint32_t after = ReadGuestU32(WATCH_ADDR);
    uint64_t seq_out = g_event_seq.fetch_add(1, std::memory_order_relaxed);
    LOGF_WARNING("[WATCH] seq={} PA-OUT sub_828C47E8 *0x{:08X} {} -> 0x{:08X}{}",
                 seq_out, WATCH_ADDR,
                 (before != after) ? "CHANGED" : "same",
                 after, PoisonTag(after));
}

// ============================================================================
// DrawPrimitiveUP PAIRED CALLERS — the 4 functions that legitimately pair
// sub_82A3DAB0 (setup) with sub_82A3DF50 (commit). Logging their entry/exit
// lets us correlate setup→commit windows against the bogus 2D-primitive batcher
// path which calls sub_82A3DF50 via sub_828BF270 with no prior sub_82A3DAB0.
// Callers: sub_8231FD20, sub_8233ED88, sub_828C0688, sub_828C0848
// ============================================================================
namespace {
inline void LogCallerEntry(const char* tag, uint32_t lr) {
    if (!WatchDiagnosticsEnabled()) {
        return;
    }
    static thread_local uint64_t s_seq = 0;
    uint64_t seq = ++s_seq;
    if (seq <= 20 || (seq % 2000) == 0) {
        uint64_t gseq = g_event_seq.fetch_add(1, std::memory_order_relaxed);
        LOGF_WARNING("[WATCH] gseq={} {} ENTER LR=0x{:08X} thread_seq={}", gseq, tag, lr, seq);
    }
}
inline void LogCallerExit(const char* tag) {
    if (!WatchDiagnosticsEnabled()) {
        return;
    }
    static thread_local uint64_t s_seq = 0;
    uint64_t seq = ++s_seq;
    if (seq <= 20 || (seq % 2000) == 0) {
        uint64_t gseq = g_event_seq.fetch_add(1, std::memory_order_relaxed);
        LOGF_WARNING("[WATCH] gseq={} {} EXIT thread_seq={}", gseq, tag, seq);
    }
}
}  // namespace

PPC_FUNC_IMPL(__imp__sub_8231FD20);
PPC_FUNC_HOOK(sub_8231FD20) { LogCallerEntry("C-8231FD20", ctx.lr); __imp__sub_8231FD20(ctx, base); LogCallerExit("C-8231FD20"); }

PPC_FUNC_IMPL(__imp__sub_8233ED88);
PPC_FUNC_HOOK(sub_8233ED88) { LogCallerEntry("C-8233ED88", ctx.lr); __imp__sub_8233ED88(ctx, base); LogCallerExit("C-8233ED88"); }

PPC_FUNC_IMPL(__imp__sub_828C0688);
PPC_FUNC_HOOK(sub_828C0688) { LogCallerEntry("C-828C0688", ctx.lr); __imp__sub_828C0688(ctx, base); LogCallerExit("C-828C0688"); }

PPC_FUNC_IMPL(__imp__sub_828C0848);
PPC_FUNC_HOOK(sub_828C0848) { LogCallerEntry("C-828C0848", ctx.lr); __imp__sub_828C0848(ctx, base); LogCallerExit("C-828C0848"); }

// ============================================================================
// CONFLICT-RESOLUTION WATCHPOINT (poison-vs-color)
// ============================================================================
// Agents 5/10 say *r4/*r5 is a stack-local legitimate ARGB color word.
// Agents 9/11/13/14/15 say it's RAGE freed-heap poison from a CBaseDC UAF.
//
// This hook logs at every entry of sub_8227F5B8 / sub_8227F608:
//   - LR (which caller)
//   - the pointer value (r4 or r5)
//   - whether the pointer is in plausible-stack range vs plausible-heap range
//   - the dereferenced value (the alleged color OR poison)
//   - tagging if the value matches 0xFFE1E1E1 (the contested signature)
//
// Plus polls dword_831E49B0 / dword_831E49DC (loading-screen handle slots
// per agent 14) every 5ms to catch transitions.
// ============================================================================
namespace {

constexpr uint32_t LOAD_HANDLE_A = 0x831E49B0;
constexpr uint32_t LOAD_HANDLE_B = 0x831E49DC;

const char* ColorTag(uint32_t v) {
    if (v == 0xFFE1E1E1u) return " *** CONTESTED 0xFFE1E1E1 ***";
    if (v == 0xFFFFFFFFu) return " (white)";
    if (v == 0xFF000000u) return " (black)";
    if (v == 0xDEADBEEFu) return " (deadbeef)";
    return "";
}

// Heuristic: guest stack lives high (around 0x70?????? or so) — heap is below.
// We can't know the exact stack range, but we can flag obvious heap addrs.
const char* PtrRangeTag(uint32_t p) {
    if (p == 0)                         return " (null)";
    if (p < 0x10000)                    return " (low/sentinel)";
    if (p >= 0x20000000 && p < 0x40000000) return " (HEAP-likely)";
    if (p >= 0x70000000 && p < 0x80000000) return " (STACK-likely)";
    if (p >= 0x82000000 && p < 0x83400000) return " (.data/.rdata)";
    return "";
}

inline uint32_t SafeReadGuestU32(uint32_t guest_addr) {
    if (guest_addr == 0 || guest_addr < 0x1000) return 0xDEADBEEFu;
    auto* host = static_cast<uint32_t*>(g_memory.Translate(guest_addr));
    return __builtin_bswap32(*host);
}

}  // namespace

PPC_FUNC_IMPL(__imp__sub_8227F5B8);
PPC_FUNC_HOOK(sub_8227F5B8) {
    if (!WatchDiagnosticsEnabled()) {
        __imp__sub_8227F5B8(ctx, base);
        return;
    }
    static thread_local uint64_t s_seq = 0;
    uint64_t seq = ++s_seq;
    uint32_t color_ptr = ctx.r4.u32;
    uint32_t color_val = SafeReadGuestU32(color_ptr);
    uint32_t handle_a  = SafeReadGuestU32(LOAD_HANDLE_A);
    uint32_t handle_b  = SafeReadGuestU32(LOAD_HANDLE_B);
    if (seq <= 50 || (seq % 1000) == 0 || color_val == 0xFFE1E1E1u) {
        LOGF_WARNING("[WATCH] F5B8 seq={} LR=0x{:08X} colorPtr=0x{:08X}{} *colorPtr=0x{:08X}{} loadHA=0x{:08X} loadHB=0x{:08X}",
                     seq, ctx.lr,
                     color_ptr, PtrRangeTag(color_ptr),
                     color_val,  ColorTag(color_val),
                     handle_a, handle_b);
    }
    __imp__sub_8227F5B8(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_8227F608);
PPC_FUNC_HOOK(sub_8227F608) {
    if (!WatchDiagnosticsEnabled()) {
        __imp__sub_8227F608(ctx, base);
        return;
    }
    static thread_local uint64_t s_seq = 0;
    uint64_t seq = ++s_seq;
    uint32_t color_ptr = ctx.r5.u32;
    uint32_t color_val = SafeReadGuestU32(color_ptr);
    uint32_t handle_a  = SafeReadGuestU32(LOAD_HANDLE_A);
    uint32_t handle_b  = SafeReadGuestU32(LOAD_HANDLE_B);
    if (seq <= 50 || (seq % 1000) == 0 || color_val == 0xFFE1E1E1u) {
        LOGF_WARNING("[WATCH] F608 seq={} LR=0x{:08X} colorPtr=0x{:08X}{} *colorPtr=0x{:08X}{} loadHA=0x{:08X} loadHB=0x{:08X}",
                     seq, ctx.lr,
                     color_ptr, PtrRangeTag(color_ptr),
                     color_val,  ColorTag(color_val),
                     handle_a, handle_b);
    }
    __imp__sub_8227F608(ctx, base);
}

// ============================================================================
// GRANDPARENT: sub_828D4C88 — only caller of sub_828C47E8
// Pinpoints the outer init/setup invocation timeline.
// ============================================================================
PPC_FUNC_IMPL(__imp__sub_828D4C88);
PPC_FUNC_HOOK(sub_828D4C88) {
    if (!WatchDiagnosticsEnabled()) {
        __imp__sub_828D4C88(ctx, base);
        return;
    }
    EnsurePollerStarted();
    uint32_t before = ReadGuestU32(WATCH_ADDR);
    uint64_t seq_in = g_event_seq.fetch_add(1, std::memory_order_relaxed);
    LOGF_WARNING("[WATCH] seq={} GP-IN sub_828D4C88 LR=0x{:08X} *0x{:08X}=0x{:08X}{}",
                 seq_in, ctx.lr, WATCH_ADDR, before, PoisonTag(before));
    __imp__sub_828D4C88(ctx, base);
    uint32_t after = ReadGuestU32(WATCH_ADDR);
    uint64_t seq_out = g_event_seq.fetch_add(1, std::memory_order_relaxed);
    LOGF_WARNING("[WATCH] seq={} GP-OUT sub_828D4C88 *0x{:08X} {} -> 0x{:08X}{}",
                 seq_out, WATCH_ADDR,
                 (before != after) ? "CHANGED" : "same",
                 after, PoisonTag(after));
}
