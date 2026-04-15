#ifdef LIBERTY_RECOMP_PS4
// PS4 trophy unlock helper for LibertyRecomp
// Call LibertyPS4UnlockTrophy(id) from the achievements system.

#include <orbis/NpTrophy.h>
#include "np_conf_ps4.h"

// Defined in ps4_main.cpp
extern int32_t g_np_context;
extern int32_t g_np_handle;

// Unlock a single trophy by ID (0-65).
// Returns true if unlocked or already unlocked; false on hard error.
bool LibertyPS4UnlockTrophy(int trophy_id)
{
    if (!g_np_context || !g_np_handle)
        return false;

    int32_t platinum_id = -1;
    int32_t ret = sceNpTrophyUnlockTrophy(g_np_context, g_np_handle,
                                           trophy_id, &platinum_id);

    // 0x80551604 = SCE_NP_TROPHY_ERROR_ALREADY_UNLOCKED — not an error
    return (ret == 0 || ret == 0x80551604);
}
#endif // LIBERTY_RECOMP_PS4
