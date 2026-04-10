// streaming_memory_patches.cpp
// PC streaming memory budget overrides for the RAGE streaming subsystem.
//
// BACKGROUND:
//   Xbox 360 had 512MB total RAM shared between GPU and CPU. The streaming
//   system parsed platform:/stream.ini to get budget values (in KB, shifted
//   left by 10 to get bytes). On PC we have gigabytes available, so we
//   override the budget setter functions to use much larger pools.
//
// STREAMING MANAGER LAYOUT (global at 0x82A9AA7C):
//   +0x20 (32): virtual heap budget  (GPU-visible: textures, VB/IB)
//   +0x2C (44): physical heap budget (CPU data: collision, skeletons, anims)
//
// KEY FUNCTIONS:
//   sub_821CFD10 - Budget parser: reads platform:/stream.ini, parses
//                  "virtual", "physical", "virtual_optimised", "physical_optimised"
//                  keys, converts KB values to bytes (<<10), calls setters.
//   sub_82511C50 - SetVirtualBudget(mgr, budget): reads TLS allocator via
//                  [*r13+1676], gets heap index 1 (virtual), queries
//                  free+used space via vtable[7]+vtable[8], clamps budget
//                  to available space, writes to mgr+32.
//   sub_82511CD8 - SetPhysicalBudget(mgr, budget): same pattern but heap
//                  index 3 (physical), writes to mgr+44.
//   sub_821C6890 - Defrag entry (virtual): sets TLS+1704 dispatch context,
//                  increments global counter at 0x831AB9A8, calls sub_8247E660.
//   sub_821C69B0 - Defrag entry (physical): same pattern, calls sub_825045C8.
//   sub_8247E338 - Inner defrag logic: relocates allocations within a heap
//                  region, iterating 108-byte entries.
//
// STRATEGY:
//   - Hook sub_82511C50/sub_82511CD8 to multiply the parsed budget by 4x
//     (capped at reasonable maximums). The TLS allocator mechanism stays
//     intact -- we just tell it "you have more room to work with".
//   - Hook sub_821CFD10 (budget parser) to inject our own budgets directly,
//     bypassing stream.ini entirely for PC.
//   - Hook defrag entries to reduce frequency -- with 4x memory, fragmentation
//     pressure is much lower.
//   - Add periodic diagnostics logging streaming memory utilization.

#include <cpu/ppc_context.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>
#include <cstring>
#include <atomic>

// =============================================================================
// Configuration constants
// =============================================================================

// PC budget targets (in bytes). Xbox 360 stream.ini typically specifies
// ~110MB virtual and ~60MB physical. We scale up significantly for PC.
//
// These are the values written to the streaming manager struct. The actual
// heap sizes are determined by the allocator -- these budgets tell the
// streaming scheduler how much it's allowed to have in-flight at once.

static constexpr uint32_t PC_VIRTUAL_BUDGET  = 512u * 1024u * 1024u;  // 512 MB
static constexpr uint32_t PC_PHYSICAL_BUDGET = 256u * 1024u * 1024u;  // 256 MB

// Defrag throttle: skip N out of every (N+1) defrag calls.
// With more memory headroom, defrag is rarely needed. Skipping most calls
// reduces CPU overhead from the relocate-and-fixup loop in sub_8247E338.
static constexpr uint32_t DEFRAG_SKIP_COUNT = 3;  // run 1 in 4 defrag calls

// Streaming manager global address
static constexpr uint32_t STREAMING_MGR_ADDR = 0x82A9AA7C;

// Diagnostics: log every N frames (0 = disabled)
static constexpr uint32_t DIAG_LOG_INTERVAL = 1800;  // ~30 seconds at 60fps

// =============================================================================
// Diagnostics state
// =============================================================================

static std::atomic<uint32_t> g_diagFrameCounter{0};
static std::atomic<uint32_t> g_defragVirtualCalls{0};
static std::atomic<uint32_t> g_defragVirtualSkips{0};
static std::atomic<uint32_t> g_defragPhysicalCalls{0};
static std::atomic<uint32_t> g_defragPhysicalSkips{0};
static std::atomic<uint32_t> g_virtualBudgetSet{0};
static std::atomic<uint32_t> g_physicalBudgetSet{0};

// =============================================================================
// sub_821CFD10 - Budget parser (reads platform:/stream.ini)
// =============================================================================
// Original logic:
//   Opens "platform:/stream.ini", reads lines, tokenizes on ',' separator,
//   matches keys "virtual", "physical", "virtual_optimised", "physical_optimised",
//   converts values from KB to bytes (atoi << 10).
//   Prefers *_optimised values when present, falls back to base values.
//   Calls sub_82511C50(streaming_mgr, virtual_budget) and
//         sub_82511CD8(streaming_mgr, physical_budget).
//
// Hook: skip the file parse entirely and inject PC budgets directly.
// We still call the original setters so the TLS allocator queries happen
// correctly -- we just pass much larger budget values.

extern "C" PPC_FUNC(__imp__sub_82511C50);
extern "C" PPC_FUNC(__imp__sub_82511CD8);

PPC_FUNC_HOOK(sub_821CFD10)
{
    // Call the original budget setters with our PC-scaled values.
    // sub_82511C50(streaming_mgr, virtual_budget)
    ctx.r3.u32 = STREAMING_MGR_ADDR;
    ctx.r4.u32 = PC_VIRTUAL_BUDGET;
    __imp__sub_82511C50(ctx, base);

    // sub_82511CD8(streaming_mgr, physical_budget)
    ctx.r3.u32 = STREAMING_MGR_ADDR;
    ctx.r4.u32 = PC_PHYSICAL_BUDGET;
    __imp__sub_82511CD8(ctx, base);

    LOG_INFO("StreamingMemory: PC budgets applied - "
             "virtual=512MB, physical=256MB (bypassed stream.ini)");
}

// =============================================================================
// sub_82511C50 - SetVirtualBudget(streaming_mgr, budget)
// =============================================================================
// Original logic:
//   1. Gets TLS allocator: alloc = TLS[*r13 + 1676]
//   2. Gets virtual heap (index 1): heap = alloc->vtable[4](alloc, 1)
//   3. Queries total free: free = heap->vtable[7](heap, -1)
//   4. Queries total used: used = heap->vtable[8](heap)
//   5. available = free + used
//   6. if (available >= budget) mgr+32 = budget; else mgr+32 = available
//
// Hook: multiply the requested budget by 4x (up to our PC cap), then let
// the original clamping logic ensure we don't exceed actual heap capacity.

PPC_FUNC_HOOK(sub_82511C50)
{
    uint32_t mgr    = ctx.r3.u32;
    uint32_t budget = ctx.r4.u32;

    // Scale up the budget for PC, capped at our target
    uint32_t scaled = budget;
    if (budget < PC_VIRTUAL_BUDGET) {
        // 4x multiplier, capped
        uint64_t wide = static_cast<uint64_t>(budget) * 4;
        scaled = (wide > PC_VIRTUAL_BUDGET) ? PC_VIRTUAL_BUDGET
                                            : static_cast<uint32_t>(wide);
    }

    g_virtualBudgetSet.store(scaled, std::memory_order_relaxed);

    ctx.r3.u32 = mgr;
    ctx.r4.u32 = scaled;
    __imp__sub_82511C50(ctx, base);

    // Read back what was actually written (may have been clamped by heap capacity)
    uint32_t actual = PPC_LOAD_U32(mgr + 32);

    if (actual != scaled) {
        LOGF_INFO("StreamingMemory: virtual budget clamped by heap: "
                  "requested={}MB, actual={}MB",
                  scaled / (1024 * 1024), actual / (1024 * 1024));
    }
}

// =============================================================================
// sub_82511CD8 - SetPhysicalBudget(streaming_mgr, budget)
// =============================================================================
// Same pattern as virtual but uses heap index 3 and writes to mgr+44.

PPC_FUNC_HOOK(sub_82511CD8)
{
    uint32_t mgr    = ctx.r3.u32;
    uint32_t budget = ctx.r4.u32;

    uint32_t scaled = budget;
    if (budget < PC_PHYSICAL_BUDGET) {
        uint64_t wide = static_cast<uint64_t>(budget) * 4;
        scaled = (wide > PC_PHYSICAL_BUDGET) ? PC_PHYSICAL_BUDGET
                                             : static_cast<uint32_t>(wide);
    }

    g_physicalBudgetSet.store(scaled, std::memory_order_relaxed);

    ctx.r3.u32 = mgr;
    ctx.r4.u32 = scaled;
    __imp__sub_82511CD8(ctx, base);

    uint32_t actual = PPC_LOAD_U32(mgr + 44);

    if (actual != scaled) {
        LOGF_INFO("StreamingMemory: physical budget clamped by heap: "
                  "requested={}MB, actual={}MB",
                  scaled / (1024 * 1024), actual / (1024 * 1024));
    }
}

// =============================================================================
// sub_821C6890 - Defrag entry (virtual heap)
// =============================================================================
// Original logic:
//   1. Saves current TLS+1704 dispatch context, sets new one tagged "defrag"
//   2. Increments global counter at 0x831AB9A8
//   3. If a2+8 != 0, calls sub_8247E660(a2+8, dispatch_ctx) to do actual defrag
//   4. Decrements counter, restores TLS+1704
//
// Hook: skip most defrag calls. With 4x memory budgets, fragmentation builds
// up much more slowly. We still run defrag periodically to prevent long-term
// drift. The TLS+1704 mechanism is preserved for the calls we do allow through.

extern "C" PPC_FUNC(__imp__sub_821C6890);

static uint32_t g_defragVirtualCounter = 0;

PPC_FUNC_HOOK(sub_821C6890)
{
    g_defragVirtualCalls.fetch_add(1, std::memory_order_relaxed);

    if (g_defragVirtualCounter < DEFRAG_SKIP_COUNT) {
        g_defragVirtualCounter++;
        g_defragVirtualSkips.fetch_add(1, std::memory_order_relaxed);
        // Return the same value the original would: r28 = *(a2+8)
        // The caller (indirect dispatch) doesn't use the return value
        // in a meaningful way, but we mirror the original for safety.
        ctx.r3.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
        return;
    }

    g_defragVirtualCounter = 0;
    __imp__sub_821C6890(ctx, base);
}

// =============================================================================
// sub_821C69B0 - Defrag entry (physical heap)
// =============================================================================
// Same pattern as virtual defrag but dispatches to sub_825045C8.

extern "C" PPC_FUNC(__imp__sub_821C69B0);

static uint32_t g_defragPhysicalCounter = 0;

PPC_FUNC_HOOK(sub_821C69B0)
{
    g_defragPhysicalCalls.fetch_add(1, std::memory_order_relaxed);

    if (g_defragPhysicalCounter < DEFRAG_SKIP_COUNT) {
        g_defragPhysicalCounter++;
        g_defragPhysicalSkips.fetch_add(1, std::memory_order_relaxed);
        ctx.r3.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
        return;
    }

    g_defragPhysicalCounter = 0;
    __imp__sub_821C69B0(ctx, base);
}

// =============================================================================
// Diagnostics: call from a frame-tick hook to periodically log stats
// =============================================================================
// This is exposed for other patches (e.g. scene_tick_patches) to call once
// per frame. It logs streaming memory utilization every DIAG_LOG_INTERVAL
// frames so developers can monitor memory pressure without spamming logs.

void StreamingMemory_FrameTick()
{
    if (DIAG_LOG_INTERVAL == 0)
        return;

    uint32_t frame = g_diagFrameCounter.fetch_add(1, std::memory_order_relaxed);
    if ((frame % DIAG_LOG_INTERVAL) != 0)
        return;

    // Read current budget values from the streaming manager struct
    uint32_t virtualBudget  = PPC_LOAD_U32(STREAMING_MGR_ADDR + 32);
    uint32_t physicalBudget = PPC_LOAD_U32(STREAMING_MGR_ADDR + 44);

    // Read defrag stats
    uint32_t dvCalls = g_defragVirtualCalls.load(std::memory_order_relaxed);
    uint32_t dvSkips = g_defragVirtualSkips.load(std::memory_order_relaxed);
    uint32_t dpCalls = g_defragPhysicalCalls.load(std::memory_order_relaxed);
    uint32_t dpSkips = g_defragPhysicalSkips.load(std::memory_order_relaxed);

    LOGF_INFO("StreamingMemory [frame {}]: "
              "vBudget={}MB, pBudget={}MB | "
              "defrag virt={}/{} phys={}/{} (run/total)",
              frame,
              virtualBudget / (1024u * 1024u),
              physicalBudget / (1024u * 1024u),
              dvCalls - dvSkips, dvCalls,
              dpCalls - dpSkips, dpCalls);
}
