#pragma once

// Umbrella header — most patches removed during non-A/I/R cleanup.
// Remaining patches self-register via static PPC_FUNC_HOOK linker symbols.

inline void InitPatches()
{
    // no-op: all active patches auto-register via extern "C" PPC_FUNC_HOOK symbols
}
