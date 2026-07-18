#pragma once
// RexGlue's PPC types are authoritative. graine SDK 0.7.5 renamed the generated
// project header from `gta4_config.h` → `gta4_init.h` and the image/code-size
// defines from `PPC_*` → `REX_*`. Keep the old PPC_* aliases so Liberty's
// kernel/memory.cpp + main.cpp don't need wholesale renames.
#include "../../glue/rexglue-sdk-main/gta4-recomp/generated/gta4_init.h"
#ifndef PPC_IMAGE_BASE
  #define PPC_IMAGE_BASE REX_IMAGE_BASE
  #define PPC_IMAGE_SIZE REX_IMAGE_SIZE
  #define PPC_CODE_BASE  REX_CODE_BASE
  #define PPC_CODE_SIZE  REX_CODE_SIZE
#endif
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
