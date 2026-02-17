#pragma once
// RexGlue's PPC types are authoritative.
// LibertyRecompLib/ppc/ppc_context.h is legacy (kept but not used).
//
// ppc_config.h must be included before context.h so PPC_LOOKUP_FUNC
// and PPC_CALL_INDIRECT_FUNC resolve to the real function table macros.
#include "ppc_config.h"
#include <rex/runtime/guest/context.h>

// g_ppcContext is now defined in rex/runtime/guest/context.h
// so both Liberty and the SDK share the same thread-local variable.

inline PPCContext* GetPPCContext()
{
    return g_ppcContext;
}

inline void SetPPCContext(PPCContext& ctx)
{
    g_ppcContext = &ctx;
}
