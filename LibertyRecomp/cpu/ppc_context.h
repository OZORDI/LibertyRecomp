#pragma once
// RexGlue's PPC types are authoritative.
// LibertyRecompLib/ppc/ppc_context.h is legacy (kept but not used).
//
// ppc_config.h must be included before context.h so PPC_LOOKUP_FUNC
// and PPC_CALL_INDIRECT_FUNC resolve to the real function table macros.
#include "ppc_config.h"
#include <rex/ppc/context.h>
#include <rex/ppc/function.h>

// SDK v0.2.1: g_ppcContext renamed to rex::g_current_ppc_context (rex/ppc/function.h)

inline PPCContext* GetPPCContext()
{
    return rex::g_current_ppc_context;
}

inline void SetPPCContext(PPCContext& ctx)
{
    rex::g_current_ppc_context = &ctx;
}
