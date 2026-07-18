#pragma once

#include <filesystem>
#include <memory>
#include <system_error>

#include <rex/filesystem.h>
#include <rex/rex_app.h>

#include "dlc_layout.h"
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

  void OnPreSetup(rex::RuntimeConfig& config) override {
    config.graphics.reset();
    config.gpu_plugin = "xenos";
  }

  void OnConfigurePaths(rex::PathConfig& paths) override {
    std::error_code error;

    if (paths.game_data_root.empty()) {
      const auto installed_game_root = rex::filesystem::GetUserFolder() / "LibertyRecomp" / "game";
      if (std::filesystem::is_directory(installed_game_root, error)) {
        paths.game_data_root = installed_game_root;
      } else {
        error.clear();
        const auto preserved_xex =
            std::filesystem::canonical(std::filesystem::path(GTA4_RECOMP_ASSET_XEX), error);
        if (!error) {
          paths.game_data_root = preserved_xex.parent_path();
        }
      }
    }

    if (paths.update_data_root.empty() && !paths.game_data_root.empty()) {
      error.clear();
      const auto installed_update_root = paths.game_data_root / "update";
      if (std::filesystem::is_directory(installed_update_root, error)) {
        paths.update_data_root = installed_update_root;
      }
    }

    gta4::PrepareDlcLayout(paths.game_data_root);
  }
};
