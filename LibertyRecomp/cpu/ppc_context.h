#pragma once
// RexGlue's PPC types are authoritative.
// LibertyRecompLib/ppc/ppc_context.h is legacy (kept but not used).
//
// ppc_config.h must be included before context.h so PPC_LOOKUP_FUNC
// and PPC_CALL_INDIRECT_FUNC resolve to the real function table macros.
#include "ppc_config.h"
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
