#pragma once
// Forward to the generated ppc_context.h in LibertyRecompLib
#include "../../LibertyRecompLib/ppc/ppc_context.h"

// Thread-local PPC context pointer
inline thread_local PPCContext* g_ppcContext = nullptr;

inline PPCContext* GetPPCContext()
{
    return g_ppcContext;
}

inline void SetPPCContext(PPCContext& ctx)
{
    g_ppcContext = &ctx;
}
