#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <system_error>

#include <rex/filesystem.h>
#include <rex/rex_app.h>
#include <rex/system/achievement_manager.h>

#include "gta4_init.h"

#ifndef GTA4_RECOMP_ASSET_XEX
#error "GTA4_RECOMP_ASSET_XEX must point to the preserved GTA IV XEX"
#endif

class GTA4App final : public rex::ReXApp {
 public:
  static std::unique_ptr<rex::ui::WindowedApp> Create(rex::ui::WindowedAppContext& context) {
    return std::unique_ptr<GTA4App>(new GTA4App(context));
  }

 private:
  explicit GTA4App(rex::ui::WindowedAppContext& context)
      : ReXApp(context, "Liberty Recompiled", PPCImageConfig) {}

  void OnPreSetup(rex::RuntimeConfig& config) override;

  std::optional<rex::PathConfig> OnFinalizePaths(
      const rex::PathConfig& defaults, std::function<void(rex::PathConfig)> resume) override;

  void OnPostSetup() override;
  void OnCreateDialogs(rex::ui::ImGuiDrawer* drawer) override;
  bool RequiresSynchronizedInitialThreadResume() const override;
  void OnShutdown() override;

  void OnConfigurePaths(rex::PathConfig& paths) override {
    std::error_code error;
    const auto default_user_root = rex::filesystem::GetUserFolder() / std::string(GetName());
    const auto liberty_root = rex::filesystem::GetUserFolder() / "LibertyRecomp";
    liberty_root_ = liberty_root;
    const bool uses_default_user_root = paths.user_data_root == default_user_root;
    const bool uses_default_cache_root = paths.cache_root == default_user_root / "cache";

    if (paths.game_data_root.empty()) {
      const auto installed_game_root = liberty_root / "game";
      paths.game_data_root = installed_game_root;
    }

    if (uses_default_user_root) {
      paths.user_data_root = liberty_root / "saves";
    }
    if (paths.saved_game_root.empty()) {
      paths.saved_game_root = liberty_root / "saves";
    }
    if (uses_default_cache_root) {
      paths.cache_root = liberty_root / "shader_cache";
    }
    native_config_path_ = liberty_root / "native.toml";
    paths.config_path = native_config_path_;
    paths.marketplace_content_root = liberty_root / "dlc";

    if (paths.update_data_root.empty() && !paths.game_data_root.empty()) {
      error.clear();
      const auto installed_update_root = paths.game_data_root / "update";
      if (std::filesystem::is_directory(installed_update_root, error)) {
        paths.update_data_root = installed_update_root;
      }
    }
  }

  rex::system::AchievementListenerHandle achievement_listener_ = 0;
  std::filesystem::path liberty_root_;
  std::filesystem::path native_config_path_;
};
