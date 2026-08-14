#include "gta4_app.h"

#include <array>

#include <rex/graphics/gta4_native/title_commands.h>
#include <rex/cvar.h>
#include <rex/logging.h>
#include <rex/runtime.h>
#include <rex/system/kernel_state.h>
#include <rex/ui/flags.h>

#include "achievement_bridge_gc.h"
#include "gta4_frontend_hooks.h"
#include "install/gta4_install_dialog.h"
#include "install/gta4_installer.h"
#include "rpf_button_prompts.h"
#include <network/community_multiplayer.h>

REXCVAR_DEFINE_STRING(gta4_multiplayer_backend, "lan", "GTA IV/Multiplayer",
                      "Compatibility service: offline, lan, or community")
    .allowed({"offline", "lan", "community"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_STRING(gta4_community_url, "https://liberty-sessions.libertyrecomp.com",
                      "GTA IV/Multiplayer", "Community session service base URL")
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_STRING(gta4_player_name, "Player", "GTA IV/Multiplayer",
                      "Anonymous LibertyRecomp player name")
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_STRING(gta4_relay_policy, "auto", "GTA IV/Multiplayer",
                      "Peer route policy: auto, direct_only, or relay_only")
    .allowed({"auto", "direct_only", "relay_only"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_UINT32(gta4_lan_discovery_port, 36002, "GTA IV/Multiplayer",
                      "LAN discovery UDP port")
    .range(1, 65535)
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_STRING(gta4_episode_startup_prompt, "retail", "GTA IV/Episodes",
                      "New-episode notification: retail or off")
    // Keep the old spellings loadable so existing configuration files migrate
    // without failing. Both aliases now preserve the retail one-time prompt.
    .allowed({"retail", "new_only", "always", "off"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);

REXCVAR_DEFINE_BOOL(install, false, "GTA IV/Installation", "Run the full game installation wizard")
    .lifecycle(rex::cvar::Lifecycle::kInitOnly);
REXCVAR_DEFINE_BOOL(install_dlc, false, "GTA IV/Installation",
                    "Run the episode installation wizard")
    .lifecycle(rex::cvar::Lifecycle::kInitOnly);
REXCVAR_DEFINE_BOOL(install_check, false, "GTA IV/Installation",
                    "Verify the installed game and episode layouts before launch")
    .lifecycle(rex::cvar::Lifecycle::kInitOnly);

REXCVAR_DEFINE_STRING(gta4_aspect_ratio, "auto", "GTA IV/Graphics/Display",
                      "Render aspect ratio: auto uses the display, original preserves 16:9")
    .allowed({"auto", "original"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_STRING(gta4_present_mode, "auto", "GTA IV/Graphics/Display",
                      "Presentation mode: auto, vsync, mailbox, or immediate")
    .allowed({"auto", "vsync", "mailbox", "immediate"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);

REXCVAR_DEFINE_UINT32(gta4_shadow_map_base_size, 512, "GTA IV/Graphics/Shadows",
                      "Base shadow-map size (512 creates a 4096x4096 point-shadow cache)")
    .range(256, 1024)
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_DOUBLE(gta4_shadow_distance_scale, 2.0, "GTA IV/Graphics/Shadows",
                      "Multiplier applied to GTA IV's directional shadow range")
    .range(1.0, 4.0)
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_STRING(gta4_reflection_resolution, "1080p", "GTA IV/Graphics/Reflections",
                      "Reflection resolution preset: original, 1080p, or full")
    .allowed({"original", "1080p", "full"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_STRING(gta4_reflection_resolution_cap, "1440p", "GTA IV/Graphics/Reflections",
                      "Maximum Full reflection resolution: 1080p, 1440p, or display")
    .allowed({"1080p", "1440p", "display"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_STRING(gta4_mirror_reflection_resolution, "inherit",
                      "GTA IV/Graphics/Reflections/Advanced",
                      "Mirror reflection override: inherit, original, 1080p, or full")
    .allowed({"inherit", "original", "1080p", "full"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_STRING(gta4_water_reflection_resolution, "inherit",
                      "GTA IV/Graphics/Reflections/Advanced",
                      "Water reflection override: inherit, original, 1080p, or full")
    .allowed({"inherit", "original", "1080p", "full"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_STRING(gta4_environment_reflection_resolution, "inherit",
                      "GTA IV/Graphics/Reflections/Advanced",
                      "Vehicle/world environment reflection override: inherit, original, 1080p, "
                      "or full")
    .allowed({"inherit", "original", "1080p", "full"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_STRING(gta4_reflection_aa, "original", "GTA IV/Graphics/Reflections",
                      "Native reflection capture anti-aliasing: original, off, 2x, or 4x")
    .allowed({"original", "off", "2x", "4x"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_STRING(gta4_reflection_capture_distance, "original",
                      "GTA IV/Graphics/Reflections/Advanced",
                      "Exterior environment capture distance: original (40), extended (60), or "
                      "far (80)")
    .allowed({"original", "extended", "far"});
REXCVAR_DEFINE_STRING(gta4_native_anti_aliasing, "smaa", "GTA IV/Graphics/Anti-Aliasing",
                      "Final-image anti-aliasing: off, spatial, fxaa, or three-pass smaa")
    .allowed({"off", "spatial", "fxaa", "smaa"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_STRING(gta4_native_smaa_quality, "high", "GTA IV/Graphics/Anti-Aliasing",
                      "SMAA 1x preset: low, medium, high, or ultra")
    .allowed({"low", "medium", "high", "ultra"});
REXCVAR_DEFINE_STRING(gta4_native_upscaler, "native", "GTA IV/Graphics/Upscaling",
                      "Output upscaler: native or fsr1")
    .allowed({"native", "fsr1"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_STRING(gta4_fsr1_quality, "quality", "GTA IV/Graphics/Upscaling",
                      "FSR 1 preset: ultra_quality, quality, balanced, or performance")
    .allowed({"ultra_quality", "quality", "balanced", "performance"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_DOUBLE(gta4_fsr1_sharpness_reduction, 0.2, "GTA IV/Graphics/Upscaling",
                      "FSR 1 RCAS sharpness reduction in stops")
    .range(0.0, 2.0)
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_BOOL(gta4_force_highest_lod, true, "GTA IV/Graphics/LOD",
                    "Prefer the highest resident model LOD regardless of distance");
REXCVAR_DEFINE_DOUBLE(gta4_draw_distance_scale, 3.0, "GTA IV/Graphics/LOD",
                      "Multiplier applied to GTA IV's world draw-distance scale")
    .range(1.0, 4.0);
REXCVAR_DEFINE_BOOL(gta4_disable_timecycle_far_clip, true, "GTA IV/Graphics/LOD",
                    "Prevent timecycle weather data from shortening the camera far clip");
REXCVAR_DEFINE_UINT32(gta4_drawable_reference_limit, 20000, "GTA IV/Graphics/LOD",
                      "Drawable-reference capacity used by extended draw distances")
    .range(13000, 40000)
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_BOOL(gta4_disable_model_budget_reduction, true, "GTA IV/Graphics/LOD",
                    "Disable pedestrian and vehicle model-budget reduction");

namespace {

void SetStartupFlag(std::string_view name, std::string_view value) {
  if (!rex::cvar::SetFlagByName(name, value)) {
    REXLOG_WARN("GTA IV graphics configuration could not set {}={}", name, value);
  }
}

void ApplyPresentationMode() {
  const std::string mode = REXCVAR_GET(gta4_present_mode);
  if (mode == "auto") {
    return;
  }

  const bool immediate = mode == "immediate";
  const bool mailbox = mode == "mailbox";
  SetStartupFlag("vsync", immediate ? "false" : "true");
  SetStartupFlag("vulkan_allow_present_mode_immediate", immediate ? "true" : "false");
  SetStartupFlag("vulkan_allow_present_mode_mailbox", mailbox ? "true" : "false");
  SetStartupFlag("vulkan_allow_present_mode_fifo_relaxed", "false");
}

void LogEpisodeInstallState(const std::filesystem::path& marketplace_root) {
  constexpr std::array<std::string_view, 2> kEpisodePackages = {"TLAD", "TBOGT"};
  for (const auto package_name : kEpisodePackages) {
    const auto package_path = marketplace_root / package_name;
    std::error_code error;
    const bool has_setup = std::filesystem::is_regular_file(package_path / "setup2.xml", error);
    error.clear();
    const bool has_content = std::filesystem::is_regular_file(package_path / "content.dat", error);
    if (has_setup && has_content) {
      REXLOG_INFO("GTA IV episode package '{}' is installed and ready at '{}'", package_name,
                  package_path.string());
    } else {
      REXLOG_WARN("GTA IV episode package '{}' is incomplete or missing at '{}'", package_name,
                  package_path.string());
    }
  }
}

}  // namespace

std::optional<rex::PathConfig> GTA4App::OnFinalizePaths(
    const rex::PathConfig& defaults, std::function<void(rex::PathConfig)> resume) {
  auto paths = defaults;
  paths.game_data_root = liberty_root_ / "game";
  paths.marketplace_content_root = liberty_root_ / "dlc";
  paths.saved_game_root = liberty_root_ / "saves";
  paths.user_data_root = liberty_root_ / "saves";
  paths.cache_root = liberty_root_ / "shader_cache";
  paths.config_path = native_config_path_;

  const bool force_install = REXCVAR_GET(install);
  const bool force_dlc = REXCVAR_GET(install_dlc);
  const bool run_check = REXCVAR_GET(install_check);
  REXCVAR_SET(install, false);
  REXCVAR_SET(install_dlc, false);
  REXCVAR_SET(install_check, false);

  std::string readiness_error;
  bool ready = gta4::install::IsInstallReady(paths.game_data_root, &readiness_error);
  if (run_check) {
    const auto verification =
        gta4::install::VerifyInstall(paths.game_data_root, paths.marketplace_content_root);
    if (verification.success) {
      REXLOG_INFO("GTA IV installation integrity check passed");
    } else {
      REXLOG_ERROR("GTA IV installation integrity check failed: {}", verification.error);
      readiness_error = verification.error;
      ready = false;
    }
  }

  if (ready && !force_install && !force_dlc) {
    std::error_code error;
    const auto update_root = paths.game_data_root / "update";
    if (std::filesystem::is_directory(update_root, error)) {
      paths.update_data_root = update_root;
    }
    return paths;
  }

  if (!ready) {
    REXLOG_WARN("GTA IV installation is not launch-ready: {}", readiness_error);
  }
  const bool dlc_only = ready && force_dlc && !force_install;
  new gta4::install::InstallDialog(
      imgui_drawer(), liberty_root_, dlc_only,
      [paths = std::move(paths), resume = std::move(resume)]() mutable {
        std::error_code error;
        const auto update_root = paths.game_data_root / "update";
        if (std::filesystem::is_directory(update_root, error)) {
          paths.update_data_root = update_root;
        } else {
          paths.update_data_root.clear();
        }
        resume(std::move(paths));
      },
      [this]() { app_context().QuitFromUIThread(); });
  return std::nullopt;
}

void GTA4App::OnPreSetup(rex::RuntimeConfig& config) {
  if (!config.graphics && config.gpu_plugin.empty()) {
    config.gpu_plugin = "gta4-native";
  }

  ApplyPresentationMode();

  const bool use_fsr1 = REXCVAR_GET(gta4_native_upscaler) == "fsr1";
  REXCVAR_SET(present_effect, use_fsr1 ? "fsr" : "bilinear");
  REXCVAR_SET(present_fsr_sharpness_reduction, REXCVAR_GET(gta4_fsr1_sharpness_reduction));
  REXLOG_INFO(
      "GTA IV native image quality: aa={} upscaler={} fsr1-quality={} "
      "fsr1-sharpness-reduction={} aspect={} present-mode={} hdr={}",
      REXCVAR_GET(gta4_native_anti_aliasing), REXCVAR_GET(gta4_native_upscaler),
      REXCVAR_GET(gta4_fsr1_quality), REXCVAR_GET(gta4_fsr1_sharpness_reduction),
      REXCVAR_GET(gta4_aspect_ratio), REXCVAR_GET(gta4_present_mode),
      rex::cvar::Query<bool>("vulkan_hdr"));

  const std::string backend = REXCVAR_GET(gta4_multiplayer_backend);
  config.live.session_protocol_version = 2;
  if (backend == "lan") {
    config.live.backend = rex::system::xam::LiveBackend::kLan;
  } else if (backend == "community") {
    config.live.backend = rex::system::xam::LiveBackend::kCommunity;
  } else {
    config.live.backend = rex::system::xam::LiveBackend::kOffline;
  }
  const std::string relay_policy = REXCVAR_GET(gta4_relay_policy);
  if (relay_policy == "direct_only") {
    config.live.relay_policy = rex::system::xam::RelayPolicy::kDirectOnly;
  } else if (relay_policy == "relay_only") {
    config.live.relay_policy = rex::system::xam::RelayPolicy::kRelayOnly;
  } else {
    config.live.relay_policy = rex::system::xam::RelayPolicy::kAuto;
  }
  config.live.community_url = REXCVAR_GET(gta4_community_url);
  config.live.player_name = REXCVAR_GET(gta4_player_name);
  config.live.lan_discovery_port = static_cast<uint16_t>(REXCVAR_GET(gta4_lan_discovery_port));
  config.live.community_backend_factory =
      &LibertyRecomp::Network::CreateCommunityMultiplayerBackend;
}

void GTA4App::OnPostSetup() {
  gta4::frontend_menu::SetConfigPath(native_config_path_);
  LogEpisodeInstallState(runtime()->marketplace_content_root());
  gta4::button_prompts::PrepareAndMount(*runtime(), game_data_root(), cache_root());

  auto* kernel_state = runtime()->kernel_state();
  achievement_listener_ = kernel_state->RegisterAchievementUnlockCallback(
      [](const rex::system::AchievementInfo& achievement) {
        gta4::game_center::SubmitAchievement(achievement.id);
      });

  for (const auto& achievement : kernel_state->loaded_achievements()) {
    if (kernel_state->IsAchievementUnlocked(achievement.id)) {
      gta4::game_center::SubmitAchievement(achievement.id);
    }
  }
}

void GTA4App::OnCreateDialogs(rex::ui::ImGuiDrawer* drawer) {
  gta4::game_center::Initialize();
}

bool GTA4App::RequiresSynchronizedInitialThreadResume() const {
  auto* graphics_system = runtime() ? runtime()->graphics_system() : nullptr;
  return graphics_system &&
         graphics_system->GetTitleCommandAbi(rex::graphics::gta4_native::kTitleId) ==
             rex::graphics::gta4_native::kTitleCommandAbi;
}

void GTA4App::OnShutdown() {
  if (achievement_listener_ == 0 || runtime() == nullptr) {
    return;
  }

  runtime()->kernel_state()->achievements().UnregisterCallback(achievement_listener_);
  achievement_listener_ = 0;
}
