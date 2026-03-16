#pragma once
// =============================================================================
// Discord Rich Presence — cross-platform abstraction
//
// Desktop (Windows / Linux / macOS): discord-rpc library
// PS4:                               sceNpActivity API
// PS5 / Switch / iOS / Android:      no-op stubs
//
// Discord application: "Grand Theft Auto IV"
// Application ID registered at https://discord.com/developers/applications
// Cover art: LibertyRecomp/res/ps4/sce_sys/icon0.png (512×512 square)
//   → uploaded as asset key "cover_art" in the Discord app's Rich Presence tab
// =============================================================================

#include <cstdint>

namespace os::discord
{
    enum class GameState : uint8_t
    {
        Idle,       // presence visible, not yet in-game (main menu / loading)
        InGame,     // actively playing
        Loading,    // loading screen
    };

    // Call once from main() after the window is created.
    void Initialize();

    // Call whenever the game state changes (loading → in game, etc.)
    void SetState(GameState state);

    // Call from the main loop to flush pending callbacks (~once per second is fine).
    void RunCallbacks();

    // Call on clean shutdown.
    void Shutdown();
}
