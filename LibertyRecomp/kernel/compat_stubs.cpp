// Minimal compat stubs for symbols that used to live in the deleted
// kernel/imports.cpp. Each is a no-op targeted enough to let Liberty
// link; the real work now happens inside rexglue (xam, xboxkrnl, etc.)
// or is intentionally dropped (runtime-phase logging helpers).

#include <stdafx.h>
#include <cstdint>
#include <cpu/ppc_context.h>
#include <kernel/xam.h>

// -----------------------------------------------------------------------------
// main.cpp: InitKernelMainThread
//
// Used to capture main-thread id for SDL event dispatch safety. rexglue
// owns the main event pump now; leave as a marker no-op.
// -----------------------------------------------------------------------------
void InitKernelMainThread()
{
    // No-op — rexglue runs the main event loop.
}

// -----------------------------------------------------------------------------
// gpu/video.cpp: KernelPhase_EnterRuntime
//
// Old imports.cpp flipped verbose boot logging off and marked the kernel
// phase as Runtime on first Present(). Drop — rexglue has its own phase
// tracking via rex::system::kernel_state.
// -----------------------------------------------------------------------------
void KernelPhase_EnterRuntime()
{
    // No-op.
}

// -----------------------------------------------------------------------------
// kernel/memory.cpp: sub_821966D0_hook
//
// Was a guest-function dispatch-table registration for a RAGE early-boot
// stub. With imports.cpp gone the recompiled guest body runs natively;
// we emit an empty hook so Memory::PopulateFunctionTableAndVtables links.
// -----------------------------------------------------------------------------
extern "C" void sub_821966D0_hook(PPCContext& /*ctx*/, uint8_t* /*base*/)
{
    // No-op — let the recompiled function run as-is.
}

// -----------------------------------------------------------------------------
// hid/driver/sdl_hid.cpp: XamNotifyEnqueueEvent
//
// Old stub posted controller-connect/disconnect events into a Liberty-local
// notify queue. rexglue's XNotifyListener path is the real home for this;
// wiring it up is a separate task. For now swallow the event.
// -----------------------------------------------------------------------------
void XamNotifyEnqueueEvent(uint32_t /*dwId*/, uint32_t /*dwParam*/)
{
    // No-op — TODO: route through rex::system::XNotifyListener.
}
