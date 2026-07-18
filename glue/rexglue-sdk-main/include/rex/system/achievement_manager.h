/**
 * @file        rex/system/achievement_manager.h
 * @brief       Host-side achievement catalog, persistence, and event API.
 *
 * @copyright   Copyright (c) 2026 Rien Gupta <rgupta9@scu.edu>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 */
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <mutex>
#include <optional>
#include <span>
#include <unordered_map>
#include <vector>

#include <rex/system/achievement_store.h>

namespace rex::system {

struct AchievementEvent {
  AchievementInfo achievement;
  uint64_t unlocked_at_filetime = 0;
};

enum class AchievementNotification {
  kShow,
  kSuppress,
};

enum class AchievementUnlockResult {
  kUnlocked,
  kAlreadyUnlocked,
  kUnknownAchievement,
};

using AchievementListenerHandle = uint64_t;
using AchievementCallback = std::function<void(const AchievementEvent&)>;

class AchievementManager {
 public:
  // Adds a new achievement or overrides an existing achievement with the same
  // ID. Intended for startup hooks so recomp-specific metadata can layer over
  // extracted title metadata.
  bool RegisterAchievement(AchievementInfo info);
  void RegisterAchievements(std::span<const AchievementInfo> achievements);
  void ReplaceAchievements(std::vector<AchievementInfo> achievements);

  // Loads and merges [[achievements]] entries from an editable TOML file.
  bool LoadMetadataFile(const std::filesystem::path& path);

  std::vector<AchievementInfo> ListAchievements() const;
  std::optional<AchievementInfo> FindAchievement(uint32_t id) const;

  // Unlock persistence and presentation are separate by default. Pass kShow
  // for a convenience notification, or call ShowAchievementNotification()
  // explicitly when title-specific timing is required.
  AchievementUnlockResult UnlockAchievement(
      uint32_t id, AchievementNotification notification = AchievementNotification::kSuppress);
  bool ShowAchievementNotification(uint32_t id);
  bool IsUnlocked(uint32_t id) const;
  uint64_t GetUnlockTime(uint32_t id) const;

  AchievementListenerHandle RegisterUnlockCallback(AchievementCallback callback);
  AchievementListenerHandle RegisterNotificationCallback(AchievementCallback callback);
  void UnregisterCallback(AchievementListenerHandle handle);

  void SetUnlockSavePath(std::filesystem::path path);
  void LoadUnlockState();
  void SaveUnlockState() const;

 private:
  static uint64_t CurrentFileTime();

  void DispatchCallbacks(
      const std::unordered_map<AchievementListenerHandle, AchievementCallback>& callbacks,
      const AchievementEvent& event) const;

  mutable std::mutex mutex_;
  mutable std::mutex save_mutex_;
  std::vector<AchievementInfo> achievements_;
  std::unordered_map<uint32_t, uint64_t> unlocked_achievements_;
  std::filesystem::path unlock_save_path_;
  std::unordered_map<AchievementListenerHandle, AchievementCallback> unlock_callbacks_;
  std::unordered_map<AchievementListenerHandle, AchievementCallback> notification_callbacks_;
  AchievementListenerHandle next_listener_handle_ = 1;
};

}  // namespace rex::system
