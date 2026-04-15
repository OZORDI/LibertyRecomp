// =============================================================================
// Switch Online Presence -- stub / libnx friends service
//
// The libnx friends API (friend:u) exposes friend list queries and invitation
// helpers but does NOT provide an API for setting arbitrary presence strings
// the way PS4's sceNpActivity does.  Switch presence is controlled by the
// system based on the running title; games cannot override the string.
//
// This file provides no-op stubs that satisfy the os::discord:: interface so
// that main.cpp remains platform-agnostic.  If Nintendo ever exposes a richer
// presence API (or if a homebrew extension becomes available), the TODOs below
// mark where the real implementation would go.
// =============================================================================

#include <os/discord/discord_presence.h>

#ifdef LIBERTY_RECOMP_NX

// TODO: If a future libnx or homebrew API exposes presence-string control,
//       #include <switch/services/friends.h> here and call it from PushStatus().

namespace os::discord
{

void Initialize()
{
    // TODO: friendsInitialize(FriendsServiceType_User) if presence API is added.
    //       For now the system handles presence automatically for the running title.
}

void SetState(GameState /*state*/)
{
    // No-op: Switch does not allow games to set custom presence strings.
    // The system shows "Playing Grand Theft Auto IV" automatically while
    // the title is running.
}

void RunCallbacks()
{
    // No per-frame work required on Switch.
}

void Shutdown()
{
    // TODO: friendsExit() if Initialize() called friendsInitialize().
}

} // namespace os::discord

#endif // LIBERTY_RECOMP_NX
