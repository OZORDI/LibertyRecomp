// streaming_async_patches.cpp
// Streaming I/O completion signaling for pgStreamer.
//
// COMPLETION SIGNALING:
//   After each ProcessRead (sub_8284BF50) completes, we signal the streaming
//   I/O event (0x83131E10) to immediately wake any thread polling for streaming
//   completion. This replaces the Xbox 360 hardware interrupt and eliminates
//   the up-to-16ms latency of waiting for the next VdSwap frame signal.

#include <cpu/ppc_context.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <kernel/kernel_sync.h>

// Streaming I/O completion event — signaled after each read to wake polling code
static constexpr uint32_t STREAMING_IO_EVENT = 0x83131E10;

// Forward-declare the recompiled ProcessRead
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
