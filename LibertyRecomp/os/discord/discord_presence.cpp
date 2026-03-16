// =============================================================================
// Discord Rich Presence — desktop implementation (Windows / Linux / macOS)
// Uses the lightweight discord-rpc library (not the Game SDK).
//
// To activate:
//  1. Create an application at https://discord.com/developers/applications
//     Name it "Grand Theft Auto IV" (matches the official title exactly).
//  2. Under Rich Presence → Art Assets, upload
//     LibertyRecomp/res/ps4/sce_sys/icon0.png as key "cover_art".
//  3. Replace DISCORD_CLIENT_ID below with your Application ID.
// =============================================================================

#include "discord_presence.h"

// Only compile on non-PS4 desktop targets.
#if !defined(LIBERTY_RECOMP_PS4)  && \
    !defined(LIBERTY_RECOMP_PS5)  && \
    !defined(LIBERTY_RECOMP_NX)   && \
    !defined(__ANDROID__)         && \
    !defined(TARGET_OS_IOS)

#include <discord_rpc.h>
#include <cstring>
#include <ctime>

namespace os::discord
{
    // ── Configuration ─────────────────────────────────────────────────────────
    // Replace with your Discord application's client ID.
    // Registered under app name "Grand Theft Auto IV".
    static constexpr const char* DISCORD_CLIENT_ID = "1234567890123456789"; // TODO: replace

    // Asset key uploaded to the Discord app's Rich Presence → Art Assets panel.
    // Source: LibertyRecomp/res/ps4/sce_sys/icon0.png (512×512 RGBA)
    static constexpr const char* LARGE_IMAGE_KEY  = "cover_art";
    static constexpr const char* LARGE_IMAGE_TEXT = "Grand Theft Auto IV";

    // ── Internal state ─────────────────────────────────────────────────────────
    static bool   s_initialized  = false;
    static time_t s_startTime    = 0;

    // ── Handlers (required by discord-rpc but unused) ──────────────────────────
    static void OnReady(const DiscordUser*)  {}
    static void OnError(int, const char*)    {}
    static void OnDisconnect(int, const char*) {}

    // ── Public API ─────────────────────────────────────────────────────────────
    void Initialize()
    {
        DiscordEventHandlers handlers{};
        handlers.ready      = OnReady;
        handlers.errored    = OnError;
        handlers.disconnected = OnDisconnect;

        Discord_Initialize(DISCORD_CLIENT_ID, &handlers, /*autoRegister=*/0, /*steamId=*/"");
        s_startTime   = std::time(nullptr);
        s_initialized = true;

        // Start with idle state (main menu / pre-game).
        SetState(GameState::Idle);
    }

    void SetState(GameState state)
    {
        if (!s_initialized) return;

        DiscordRichPresence p{};
        p.largeImageKey  = LARGE_IMAGE_KEY;
        p.largeImageText = LARGE_IMAGE_TEXT;
        p.startTimestamp = s_startTime;
        p.instance       = 0;

        switch (state)
        {
        case GameState::InGame:
            p.state   = "In Game";
            p.details = "Grand Theft Auto IV";
            break;
        case GameState::Loading:
            p.state   = "Loading";
            p.details = "Grand Theft Auto IV";
            break;
        case GameState::Idle:
        default:
            p.state   = "Main Menu";
            p.details = "Grand Theft Auto IV";
            break;
        }

        Discord_UpdatePresence(&p);
    }

    void RunCallbacks()
    {
        if (s_initialized)
            Discord_RunCallbacks();
    }

    void Shutdown()
    {
        if (s_initialized)
        {
            Discord_ClearPresence();
            Discord_Shutdown();
            s_initialized = false;
        }
    }

} // namespace os::discord

#else  // ── Stub for unsupported platforms ──────────────────────────────────

namespace os::discord
{
    void Initialize()   {}
    void SetState(GameState) {}
    void RunCallbacks() {}
    void Shutdown()     {}
}

#endif
