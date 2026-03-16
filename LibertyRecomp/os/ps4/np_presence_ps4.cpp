// =============================================================================
// PS4 Online Presence — sceNpActivity implementation
// Sets a visible activity string in the XMB / Friends list.
// Called through the same os::discord:: namespace so main.cpp is platform-agnostic.
// =============================================================================

#include <os/discord/discord_presence.h>

#ifdef LIBERTY_RECOMP_PS4

#include <orbis/NpActivity.h>
#include <orbis/NpCommon.h>
#include <orbis/libkernel.h>

// g_ps4_user_id is set during NP init in os/ps4/ps4_main.cpp
extern int32_t g_ps4_user_id;

namespace os::discord
{
    // ── sceNpActivity presence strings ────────────────────────────────────────
    // Sony recommends short, localised strings; we keep them EN-US for now.
    static constexpr const char* STATUS_IN_GAME = "Playing Grand Theft Auto IV";
    static constexpr const char* STATUS_LOADING = "Loading Grand Theft Auto IV";
    static constexpr const char* STATUS_IDLE    = "Grand Theft Auto IV";

    static bool s_initialized = false;

    // ── Helpers ───────────────────────────────────────────────────────────────
    static void PushStatus(const char* status)
    {
        if (!s_initialized || g_ps4_user_id <= 0) return;

        SceNpActivityPresenceStatus presence{};
        // Copy status string safely into the fixed-size buffer
        sceNpActivityInitPresenceStatus(&presence);
        sceNpActivitySetPresenceStatusString(&presence,
            "inGamePresence", const_cast<char*>(status));
        sceNpActivitySetPresence(g_ps4_user_id, &presence);
    }

    // ── Public API ─────────────────────────────────────────────────────────────
    void Initialize()
    {
        // sceNpActivity is initialised as part of the NP context in ps4_main.cpp.
        // We just record that we're ready and push idle state.
        s_initialized = true;
        SetState(GameState::Idle);
    }

    void SetState(GameState state)
    {
        switch (state)
        {
        case GameState::InGame:
            PushStatus(STATUS_IN_GAME);
            break;
        case GameState::Loading:
            PushStatus(STATUS_LOADING);
            break;
        case GameState::Idle:
        default:
            PushStatus(STATUS_IDLE);
            break;
        }
    }

    void RunCallbacks()
    {
        // sceNpActivity is event-driven; no per-frame polling required.
    }

    void Shutdown()
    {
        s_initialized = false;
        // NP context teardown is handled in ps4_main.cpp.
    }

} // namespace os::discord

#endif // LIBERTY_RECOMP_PS4
