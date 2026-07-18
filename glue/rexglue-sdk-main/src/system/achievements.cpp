/**
 * @file        system/achievements.cpp
 * @brief       Convenience facade over the live AchievementManager.
 *
 * @copyright   Copyright (c) 2026 Rien Gupta <rgupta9@scu.edu>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 */

#include <rex/system/achievements.h>

#include <utility>

#include <rex/system/achievement_manager.h>
#include <rex/system/kernel_state.h>

namespace rex::system {

bool RegisterAchievement(AchievementInfo info) {
  auto* ks = kernel_state();
  if (!ks) {
    return false;
  }
  return ks->achievements().RegisterAchievement(std::move(info));
}

bool UnlockAchievement(uint32_t id, bool show_toast) {
  auto* ks = kernel_state();
  if (!ks) {
    return false;
  }
  auto result = ks->achievements().UnlockAchievement(
      id, show_toast ? AchievementNotification::kShow : AchievementNotification::kSuppress);
  return result == AchievementUnlockResult::kUnlocked;
}

bool IsAchievementUnlocked(uint32_t id) {
  auto* ks = kernel_state();
  if (!ks) {
    return false;
  }
  return ks->achievements().IsUnlocked(id);
}

}  // namespace rex::system
