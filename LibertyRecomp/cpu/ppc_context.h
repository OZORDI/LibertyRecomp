#pragma once
// RexGlue's PPC types are authoritative. Previously Liberty had its own
// ppc_config.h shim that forwarded to gta4-recomp's generated
// gta4_config.h — consumers now include that directly.
#include "../../glue/gta4-recomp/generated/gta4_config.h"
#include <rex/ppc/context.h>
#include <rex/ppc/function.h>

// SDK v0.7.4: Thread-local PPC context accessed via rex::runtime::current_ppc_context()
#include <rex/system/thread_state.h>

inline PPCContext* GetPPCContext()
{
    return rex::runtime::current_ppc_context();
}

inline void SetPPCContext(PPCContext& /* ctx */)
{
    // In 0.7.4, the context is per-thread via ThreadState::Bind().
    // SetPPCContext is a no-op — the context is managed by the runtime.
}
