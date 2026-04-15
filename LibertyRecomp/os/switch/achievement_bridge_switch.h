// achievement_bridge_switch.h -- Xbox 360 achievement -> local tracker bridge (Switch)
//
// The Nintendo Switch has no native trophy/achievement system.
// This module provides a local file-backed tracker that mirrors the PS4
// bridge's public interface.  Achievement state is persisted to
// save:/achievements.dat and auto-saved on every unlock.
//
// Xbox achievement IDs are 1-indexed (1..65, base + DLC).
// A "completion" flag (equivalent to PS4 Platinum) is set automatically
// once all 65 achievements are unlocked.
//
// Call LibertyNXOnXboxAchievementUnlocked() from the XAM message handler
// whenever the game fires an "AWARD_ACHIEVEMENT" write (XMsg 0xFB/0xB0008).

#pragma once

#ifdef LIBERTY_RECOMP_NX

#include <cstdint>

namespace os::achievements
{
    // Initialise the achievement subsystem.
    // Loads any previously saved state from save:/achievements.dat.
    // Safe to call more than once (subsequent calls are no-ops).
    bool InitAchievements();

    // Record an achievement unlock.  xbox_id is 1-indexed (1..65).
    // Automatically persists to disk and checks the completion flag.
    void UnlockAchievement(uint32_t xbox_id);

    // Query whether a specific achievement has been unlocked.
    // xbox_id is 1-indexed (1..65).
    bool IsAchievementUnlocked(uint32_t xbox_id);

    // Returns the number of non-platinum achievements currently unlocked (0..65).
    int GetAchievementProgress();

    // Returns true if the completion flag (all 65 unlocked) is set.
    bool IsCompleted();

    // Flush state and release resources.
    void Shutdown();
}

// Convenience wrapper matching the PS4 bridge's call-site convention.
// Delegates to os::achievements::UnlockAchievement().
void LibertyNXOnXboxAchievementUnlocked(uint32_t xbox_id);

#endif // LIBERTY_RECOMP_NX
