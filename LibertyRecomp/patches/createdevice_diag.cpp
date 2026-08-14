// =============================================================================
// CreateDevice Gate Diagnostic Instrumentation
// =============================================================================
// Pure observation, no fixes. Tracks which gate inside sub_82A50890 (the real
// rage::grcDeviceDx::CreateDevice) returns 0, preventing the singleton at
// *0x831C22A4 from being populated.
// =============================================================================

#include <api/Liberty.h>
#include <kernel/function.h>
#include <os/logger.h>
#include <atomic>

namespace {

struct GateCounter {
    std::atomic<uint32_t> enter{0};
    std::atomic<uint32_t> exit{0};
};

GateCounter g_cnt_50890;
GateCounter g_cnt_507A8;
GateCounter g_cnt_49D08;
GateCounter g_cnt_4F560;
GateCounter g_cnt_42020;
GateCounter g_cnt_503C8;
GateCounter g_cnt_416B8;

}  // namespace

PPC_FUNC_IMPL(__imp__sub_82A50890);
PPC_FUNC_HOOK(sub_82A50890) {
    uint32_t n = g_cnt_50890.enter.fetch_add(1, std::memory_order_relaxed);
    uint32_t guest_lr = (uint32_t)ctx.lr;
    uint32_t r3_in = ctx.r3.u32;
    if (n < 8) {
        LOGF_WARNING("[DIAG-CD 50890 seq={}] ENTER guest_lr=0x{:08X} r3=0x{:08X} r4=0x{:08X} r5=0x{:08X} r6=0x{:08X}",
                     n, guest_lr, r3_in, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
    }
    __imp__sub_82A50890(ctx, base);
    uint32_t m = g_cnt_50890.exit.fetch_add(1, std::memory_order_relaxed);
    if (m < 8) {
        LOGF_WARNING("[DIAG-CD 50890 seq={}] EXIT ret(r3)=0x{:08X} {}",
                     m, ctx.r3.u32, (ctx.r3.u32 == 0) ? "*** FAILURE PATH ***" : "success");
    }
}

PPC_FUNC_IMPL(__imp__sub_82A507A8);
PPC_FUNC_HOOK(sub_82A507A8) {
    uint32_t n = g_cnt_507A8.enter.fetch_add(1, std::memory_order_relaxed);
    uint32_t guest_lr = (uint32_t)ctx.lr;
    uint32_t r3_in = ctx.r3.u32;
    if (n < 8) {
        LOGF_WARNING("[DIAG-CD G1-507A8 seq={}] ENTER guest_lr=0x{:08X} r3=0x{:08X}",
                     n, guest_lr, r3_in);
    }
    __imp__sub_82A507A8(ctx, base);
    uint32_t m = g_cnt_507A8.exit.fetch_add(1, std::memory_order_relaxed);
    if (m < 8) {
        LOGF_WARNING("[DIAG-CD G1-507A8 seq={}] EXIT ret=0x{:08X} {}",
                     m, ctx.r3.u32, (ctx.r3.u32 == 0) ? "*** GATE BAIL ***" : "ok");
    }
}

PPC_FUNC_IMPL(__imp__sub_82A49D08);
PPC_FUNC_HOOK(sub_82A49D08) {
    uint32_t n = g_cnt_49D08.enter.fetch_add(1, std::memory_order_relaxed);
    uint32_t guest_lr = (uint32_t)ctx.lr;
    if (n < 8) {
        LOGF_WARNING("[DIAG-CD G2-49D08 seq={}] ENTER guest_lr=0x{:08X} r3=0x{:08X} r4=0x{:08X}",
                     n, guest_lr, ctx.r3.u32, ctx.r4.u32);
    }
    __imp__sub_82A49D08(ctx, base);
    uint32_t m = g_cnt_49D08.exit.fetch_add(1, std::memory_order_relaxed);
    if (m < 8) {
        LOGF_WARNING("[DIAG-CD G2-49D08 seq={}] EXIT ret=0x{:08X} {}",
                     m, ctx.r3.u32, (ctx.r3.u32 != 0) ? "*** GATE BAIL ***" : "ok");
    }
}

PPC_FUNC_IMPL(__imp__sub_82A4F560);
PPC_FUNC_HOOK(sub_82A4F560) {
    uint32_t n = g_cnt_4F560.enter.fetch_add(1, std::memory_order_relaxed);
    uint32_t guest_lr = (uint32_t)ctx.lr;
    if (n < 8) {
        LOGF_WARNING("[DIAG-CD G3-4F560 seq={}] ENTER guest_lr=0x{:08X} r3=0x{:08X}",
                     n, guest_lr, ctx.r3.u32);
    }
    __imp__sub_82A4F560(ctx, base);
    uint32_t m = g_cnt_4F560.exit.fetch_add(1, std::memory_order_relaxed);
    if (m < 8) {
        LOGF_WARNING("[DIAG-CD G3-4F560 seq={}] EXIT ret=0x{:08X} {}",
                     m, ctx.r3.u32, (ctx.r3.u32 == 0) ? "*** GATE BAIL ***" : "ok");
    }
}

PPC_FUNC_IMPL(__imp__sub_82A42020);
PPC_FUNC_HOOK(sub_82A42020) {
    uint32_t n = g_cnt_42020.enter.fetch_add(1, std::memory_order_relaxed);
    uint32_t guest_lr = (uint32_t)ctx.lr;
    if (n < 8) {
        LOGF_WARNING("[DIAG-CD G4-42020 seq={}] ENTER guest_lr=0x{:08X} r3=0x{:08X}",
                     n, guest_lr, ctx.r3.u32);
    }
    __imp__sub_82A42020(ctx, base);
    uint32_t m = g_cnt_42020.exit.fetch_add(1, std::memory_order_relaxed);
    if (m < 8) {
        LOGF_WARNING("[DIAG-CD G4-42020 seq={}] EXIT ret=0x{:08X} {}",
                     m, ctx.r3.u32, (ctx.r3.u32 == 0) ? "*** GATE BAIL ***" : "ok");
    }
}

PPC_FUNC_IMPL(__imp__sub_82A503C8);
PPC_FUNC_HOOK(sub_82A503C8) {
    uint32_t n = g_cnt_503C8.enter.fetch_add(1, std::memory_order_relaxed);
    uint32_t guest_lr = (uint32_t)ctx.lr;
    if (n < 8) {
        LOGF_WARNING("[DIAG-CD G5-503C8 seq={}] ENTER guest_lr=0x{:08X} r3=0x{:08X}",
                     n, guest_lr, ctx.r3.u32);
    }
    __imp__sub_82A503C8(ctx, base);
    uint32_t m = g_cnt_503C8.exit.fetch_add(1, std::memory_order_relaxed);
    if (m < 8) {
        LOGF_WARNING("[DIAG-CD G5-503C8 seq={}] EXIT ret=0x{:08X} {}",
                     m, ctx.r3.u32, (ctx.r3.u32 == 0) ? "*** GATE BAIL ***" : "ok");
    }
}

static std::atomic<uint32_t> g_free_device_hits{0};

PPC_FUNC_IMPL(__imp__sub_821B3560);
PPC_FUNC_HOOK(sub_821B3560) {
    uint32_t ptr = ctx.r4.u32;
    uint32_t device = PPC_LOAD_U32(0x831C22A4);
    if (ptr != 0 && ptr == device) {
        uint32_t n = g_free_device_hits.fetch_add(1, std::memory_order_relaxed);
        if (n < 16) {
            LOGF_WARNING("[DIAG-FREE seq={}] rage::free called with device ptr=0x{:08X} lr=0x{:08X} r3(allocator)=0x{:08X}",
                         n, ptr, (uint32_t)ctx.lr, ctx.r3.u32);
        }
    }
    __imp__sub_821B3560(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_82A416B8);
PPC_FUNC_HOOK(sub_82A416B8) {
    uint32_t n = g_cnt_416B8.enter.fetch_add(1, std::memory_order_relaxed);
    uint32_t guest_lr = (uint32_t)ctx.lr;
    if (n < 8) {
        LOGF_WARNING("[DIAG-CD P-416B8 seq={}] ENTER guest_lr=0x{:08X} r3=0x{:08X} r4=0x{:08X} r5=0x{:08X} r6=0x{:08X} r7=0x{:08X} r8=0x{:08X}",
                     n, guest_lr, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32);
    }
    __imp__sub_82A416B8(ctx, base);
    uint32_t m = g_cnt_416B8.exit.fetch_add(1, std::memory_order_relaxed);
    if (m < 8) {
        LOGF_WARNING("[DIAG-CD P-416B8 seq={}] EXIT ret=0x{:08X}", m, ctx.r3.u32);
    }
}
