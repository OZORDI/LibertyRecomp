#include "gta4_app.h"
#include "gta4_present_mode_policy.h"

#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <unordered_set>
#include <vector>

#include <rex/graphics/gta4_native/title_commands.h>
#include <rex/graphics/gta4_native/anti_aliasing_policy.h>
#include <rex/graphics/gta4_native/hdr_policy.h>
#include <rex/graphics/gta4_native/supersampling_policy.h>
#include <rex/graphics/video_mode_util.h>
#include <rex/cvar.h>
#include <rex/logging.h>
#include <rex/runtime.h>
#include <rex/system/kernel_state.h>
#include <rex/system/xam/content_manager.h>
#include <rex/system/xam/live_compatibility.h>
#include <rex/system/xam/user_profile.h>
#include <rex/ui/flags.h>
#include <rex/ui/keybinds.h>
#include <rex/ui/window.h>

#include "achievement_bridge_gc.h"
#include "gta4_frontend_hooks.h"
#include "gta4_presentation_options.h"
#include "gta4_keyboard_controller.h"
#include "gta4_touch_coordinator.h"
#include "install/gta4_install_dialog.h"
#include "install/gta4_installer.h"
#include "input/user_music_player.h"
#include "input/text_chat_dialog.h"
#include "input/context_touch_controls.h"
#include "input/context_touch_overlay.h"
#include "rpf_button_prompts.h"
#include <network/community_multiplayer.h>
#include <network/gta4_voice_audio.h>

REXCVAR_DEFINE_STRING(gta4_multiplayer_backend, "community", "GTA IV/Multiplayer",
                      "Compatibility service: offline, lan, or community")
    .allowed({"offline", "lan", "community"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_STRING(gta4_community_url, "http://127.0.0.1:8080", "GTA IV/Multiplayer",
                      "Community session service base URL")
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
REXCVAR_DEFINE_BOOL(gta4_diagnostics_skip_user_music, false, "GTA IV/Diagnostics",
                    "Skip the host user-music player during isolated diagnostics")
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);

GTA4App::GTA4App(rex::ui::WindowedAppContext& context)
    : ReXApp(context, "Liberty Recompiled", PPCImageConfig) {}

GTA4App::~GTA4App() = default;

REXCVAR_DEFINE_STRING(gta4_aspect_ratio, "auto", "GTA IV/Graphics/Display",
                      "Render aspect: auto follows the display; fixed ratios fit without stretching")
    .allowed({"auto", "original", "16:9", "16:10", "3:2", "4:3", "5:4", "21:9", "43:18", "32:9", "32:10"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_STRING(gta4_present_mode, "auto", "GTA IV/Graphics/Display",
                      "Presentation mode: auto, vsync, mailbox, or immediate")
    .allowed({"auto", "vsync", "mailbox", "immediate"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_UINT32(gta4_frame_limit, 60, "GTA IV/Graphics/Display",
                      "Maximum guest frame rate: 0 is unlocked; supported caps are 30, 60, and "
                      "120 FPS")
    .allowed({"0", "30", "60", "120"});
REXCVAR_DEFINE_STRING(gta4_native_hdr_mode, "off", "GTA IV/Graphics/HDR",
                      "HDR output: off, scRGB, or Auto HDR")
    .allowed({"off", "scrgb", "auto_hdr"});
REXCVAR_DEFINE_BOOL(gta4_native_hdr_mode_unified, false,
                    "GTA IV/Graphics/HDR/Compatibility",
                    "Canonical HDR mode has replaced the legacy Vulkan HDR toggle");
REXCVAR_DEFINE_DOUBLE(gta4_native_hdr_paper_white_nits, 203.0,
                      "GTA IV/Graphics/HDR",
                      "SDR reference white brightness used by scRGB and Auto HDR")
    .range(80.0, 500.0);
REXCVAR_DEFINE_DOUBLE(gta4_native_hdr_peak_nits, 400.0, "GTA IV/Graphics/HDR",
                      "Auto HDR highlight target brightness")
    .range(80.0, 2000.0);
REXCVAR_DEFINE_DOUBLE(gta4_native_auto_hdr_shoulder_start, 0.0,
                      "GTA IV/Graphics/HDR/Advanced",
                      "Normalized luminance where Auto HDR highlight expansion begins")
    .range(0.0, 1.0);
REXCVAR_DEFINE_DOUBLE(gta4_native_auto_hdr_shoulder_power, 2.5,
                      "GTA IV/Graphics/HDR/Advanced",
                      "Auto HDR highlight shoulder exponent")
    .range(1.0, 10.0);

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
                      "Anti-aliasing: off, fxaa, smaa, deferred MSAA, or even-factor SSAA")
    // `spatial` remains loadable as a deprecated compatibility alias. It is
    // never exposed by the frontend or accepted by the controller setter.
    .allowed({"off", "fxaa", "smaa", "msaa2x", "msaa4x", "ssaa2x", "ssaa4x",
              "ssaa6x", "ssaa8x", "ssaa10x", "ssaa12x", "ssaa14x", "ssaa16x",
              "spatial"});
REXCVAR_DEFINE_BOOL(gta4_native_anti_aliasing_unified, false,
                    "GTA IV/Graphics/Anti-Aliasing/Compatibility",
                    "Canonical unified anti-aliasing selection has replaced legacy split values");
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
                      "Multiplier applied through GTA IV's built-in world-distance input")
    .range(1.0, 4.0);
REXCVAR_DEFINE_UINT32(gta4_drawable_reference_limit, 20000, "GTA IV/Graphics/LOD",
                      "Drawable-reference capacity used by extended draw distances")
    .range(13000, 40000)
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);

namespace {

std::mutex g_anti_aliasing_controller_mutex;
std::atomic<bool> g_anti_aliasing_controller_initialized{false};
std::atomic<rex::graphics::gta4_native::AntiAliasingMode>
    g_configured_anti_aliasing_mode{rex::graphics::gta4_native::AntiAliasingMode::kSmaa};
std::atomic<rex::graphics::gta4_native::AntiAliasingMode>
    g_active_anti_aliasing_mode{rex::graphics::gta4_native::AntiAliasingMode::kSmaa};
std::mutex g_hdr_controller_mutex;
std::atomic<bool> g_hdr_controller_initialized{false};
std::atomic<rex::graphics::gta4_native::HdrMode> g_configured_hdr_mode{
    rex::graphics::gta4_native::HdrMode::kOff};

const char* AntiAliasingCompatibilityName(
    rex::graphics::gta4_native::AntiAliasingCompatibility compatibility) {
  using Compatibility = rex::graphics::gta4_native::AntiAliasingCompatibility;
  switch (compatibility) {
    case Compatibility::kCanonical:
      return "canonical";
    case Compatibility::kSpatialAlias:
      return "spatial-alias";
    case Compatibility::kLegacySceneMsaa:
      return "legacy-scene-msaa";
    case Compatibility::kLegacySpatialToggle:
      return "legacy-spatial-toggle";
    case Compatibility::kFallbackOff:
      return "fallback-off";
  }
  return "fallback-off";
}

void InitializeFrontendAntiAliasingControllerLocked() {
  using namespace rex::graphics::gta4_native;
  if (g_anti_aliasing_controller_initialized.load(std::memory_order_acquire)) {
    return;
  }
  const std::string configured = REXCVAR_GET(gta4_native_anti_aliasing);
  const std::string legacy_scene_msaa = rex::cvar::GetFlagByName("gta4_native_msaa");
  const auto resolved = ResolveAntiAliasingConfiguration(
      configured, legacy_scene_msaa,
      rex::cvar::Query<bool>("gta4_native_spatial_aa"),
      REXCVAR_GET(gta4_native_anti_aliasing_unified));
  g_configured_anti_aliasing_mode.store(resolved.mode, std::memory_order_release);
  g_active_anti_aliasing_mode.store(resolved.mode, std::memory_order_release);
  g_anti_aliasing_controller_initialized.store(true, std::memory_order_release);
  const auto route = GetAntiAliasingRoute(resolved.mode);
  REXLOG_INFO(
      "GTA4AAPolicy owner=frontend event=initialize configured-raw={} legacy-msaa={} "
      "legacy-spatial={} unified={} configured={} active={} compatibility={} "
      "ignored-legacy-msaa={} route-fxaa={} route-smaa={} scene-samples={} ssaa-factor={}",
      configured, legacy_scene_msaa, rex::cvar::Query<bool>("gta4_native_spatial_aa"),
      REXCVAR_GET(gta4_native_anti_aliasing_unified), AntiAliasingModeName(resolved.mode),
      AntiAliasingModeName(resolved.mode), AntiAliasingCompatibilityName(resolved.compatibility),
      resolved.ignored_legacy_scene_msaa, route.presentation_fxaa, route.presentation_smaa,
      route.scene_sample_count, route.supersampling_pixel_factor);
}

const char* HdrCompatibilityName(
    rex::graphics::gta4_native::HdrCompatibility compatibility) {
  using Compatibility = rex::graphics::gta4_native::HdrCompatibility;
  switch (compatibility) {
    case Compatibility::kCanonical:
      return "canonical";
    case Compatibility::kLegacyVulkanHdr:
      return "legacy-vulkan-hdr";
    case Compatibility::kFallbackOff:
      return "fallback-off";
  }
  return "fallback-off";
}

void InitializeFrontendHdrControllerLocked() {
  using namespace rex::graphics::gta4_native;
  if (g_hdr_controller_initialized.load(std::memory_order_acquire)) {
    return;
  }
  const std::string configured = REXCVAR_GET(gta4_native_hdr_mode);
  const bool legacy_vulkan_hdr = rex::cvar::Query<bool>("vulkan_hdr");
  const auto resolved = ResolveHdrConfiguration(
      configured, legacy_vulkan_hdr, REXCVAR_GET(gta4_native_hdr_mode_unified));
  const std::string_view resolved_name = HdrModeName(resolved.mode);
  const bool transport_enabled = HdrModeNeedsExtendedOutput(resolved.mode);
  const bool canonical_written =
      rex::cvar::SetFlagByName("gta4_native_hdr_mode", resolved_name);
  const bool marker_written =
      rex::cvar::SetFlagByName("gta4_native_hdr_mode_unified", "true");
  const bool transport_written = rex::cvar::SetFlagByName(
      "vulkan_hdr", transport_enabled ? "true" : "false");
  if (!canonical_written || !marker_written || !transport_written) {
    REXLOG_ERROR(
        "GTA4HDRPolicy event=initialize-failed configured-raw={} legacy-vulkan-hdr={} "
        "resolved={} canonical-written={} marker-written={} transport-written={}",
        configured, legacy_vulkan_hdr, resolved_name, canonical_written, marker_written,
        transport_written);
  }
  g_configured_hdr_mode.store(resolved.mode, std::memory_order_release);
  g_hdr_controller_initialized.store(true, std::memory_order_release);
  REXLOG_INFO(
      "GTA4HDRPolicy event=initialize configured-raw={} legacy-vulkan-hdr={} unified={} "
      "configured={} compatibility={} transport={}",
      configured, legacy_vulkan_hdr, REXCVAR_GET(gta4_native_hdr_mode_unified), resolved_name,
      HdrCompatibilityName(resolved.compatibility), transport_enabled);
}

bool ValidateSupersamplingSelection(
    rex::graphics::gta4_native::AntiAliasingMode mode) {
  using namespace rex::graphics::gta4_native;
  const uint32_t pixel_factor = GetAntiAliasingRoute(mode).supersampling_pixel_factor;
  if (pixel_factor == 1u) {
    return true;
  }

  auto* runtime = rex::Runtime::instance();
  auto* graphics = runtime ? runtime->graphics_system() : nullptr;
  auto* window = runtime ? runtime->display_window() : nullptr;
  if (!graphics || graphics->GetTitleCommandAbi(kTitleId) != kTitleCommandAbi || !window) {
    REXLOG_ERROR(
        "GTA4SSAA event=reject-selection factor={} reason=device-capabilities-unavailable",
        pixel_factor);
    return false;
  }

  uint32_t output_width = window->GetActualPhysicalWidth();
  uint32_t output_height = window->GetActualPhysicalHeight();
  int32_t preset_width = 0;
  int32_t preset_height = 0;
  if (rex::graphics::video_mode_util::TryGetResolutionPresetFromCVar(preset_width,
                                                                     preset_height) &&
      preset_width > 0 && preset_height > 0) {
    output_width = uint32_t(preset_width);
    output_height = uint32_t(preset_height);
  }

  QueryDeviceCapabilitiesCommand command{};
  DeviceCapabilitiesResult capabilities{};
  if (!output_width || !output_height ||
      !graphics->ExecuteTitleCommand(kTitleId, kTitleCommandAbi, &command, sizeof(command),
                                     &capabilities, sizeof(capabilities))) {
    REXLOG_ERROR(
        "GTA4SSAA event=reject-selection factor={} output={}x{} "
        "reason=device-query-failed",
        pixel_factor, output_width, output_height);
    return false;
  }

  const auto physical = CalculateSupersampledExtent(
      output_width, output_height, pixel_factor, capabilities.max_image_dimension_2d);
  if (!physical) {
    REXLOG_ERROR(
        "GTA4SSAA event=reject-selection factor={} output={}x{} max-dimension={} "
        "reason=physical-extent-unsupported",
        pixel_factor, output_width, output_height, capabilities.max_image_dimension_2d);
    return false;
  }
  REXLOG_INFO(
      "GTA4SSAA event=validate-selection factor={} logical={}x{} physical={}x{} "
      "max-dimension={}",
      pixel_factor, output_width, output_height, physical->width, physical->height,
      capabilities.max_image_dimension_2d);
  return true;
}

}  // namespace

namespace rex::graphics::gta4_native {

void InitializeHdrController() {
  std::lock_guard lock(g_hdr_controller_mutex);
  InitializeFrontendHdrControllerLocked();
}

HdrMode GetConfiguredHdrMode() {
  InitializeHdrController();
  return g_configured_hdr_mode.load(std::memory_order_acquire);
}

std::string_view GetConfiguredHdrModeName() {
  return HdrModeName(GetConfiguredHdrMode());
}

bool SetConfiguredHdrMode(std::string_view value) {
  const auto requested = ParseHdrMode(value);
  if (!requested) {
    REXLOG_ERROR("GTA4HDRPolicy event=reject requested={} reason=invalid-mode", value);
    return false;
  }
  std::lock_guard lock(g_hdr_controller_mutex);
  InitializeFrontendHdrControllerLocked();
  const HdrMode previous =
      g_configured_hdr_mode.load(std::memory_order_acquire);
  const bool transport_enabled = HdrModeNeedsExtendedOutput(*requested);
  if (!rex::cvar::SetFlagByName("gta4_native_hdr_mode_unified", "true") ||
      !rex::cvar::SetFlagByName("gta4_native_hdr_mode", HdrModeName(*requested)) ||
      !rex::cvar::SetFlagByName("vulkan_hdr",
                                transport_enabled ? "true" : "false")) {
    REXLOG_ERROR("GTA4HDRPolicy event=reject requested={} reason=cvar-write", value);
    return false;
  }
  g_configured_hdr_mode.store(*requested, std::memory_order_release);
  REXLOG_INFO(
      "GTA4HDRPolicy event=set previous={} configured={} transport={} apply=live",
      HdrModeName(previous), HdrModeName(*requested), transport_enabled);
  return true;
}

void InitializeAntiAliasingController() {
  std::lock_guard lock(g_anti_aliasing_controller_mutex);
  InitializeFrontendAntiAliasingControllerLocked();
}

AntiAliasingMode GetConfiguredAntiAliasingMode() {
  InitializeAntiAliasingController();
  return g_configured_anti_aliasing_mode.load(std::memory_order_acquire);
}

AntiAliasingMode GetActiveAntiAliasingMode() {
  InitializeAntiAliasingController();
  return g_active_anti_aliasing_mode.load(std::memory_order_acquire);
}

std::string_view GetConfiguredAntiAliasingModeName() {
  return AntiAliasingModeName(GetConfiguredAntiAliasingMode());
}

std::string_view GetActiveAntiAliasingModeName() {
  return AntiAliasingModeName(GetActiveAntiAliasingMode());
}

AntiAliasingApplyResult SetConfiguredAntiAliasingMode(std::string_view value) {
  const auto requested = ParseAntiAliasingMode(value);
  if (!requested) {
    REXLOG_ERROR("GTA4AAPolicy owner=frontend event=reject requested={}", value);
    return AntiAliasingApplyResult::kRejected;
  }

  std::lock_guard lock(g_anti_aliasing_controller_mutex);
  InitializeFrontendAntiAliasingControllerLocked();
  const AntiAliasingMode active =
      g_active_anti_aliasing_mode.load(std::memory_order_acquire);
  const AntiAliasingMode previous_configured =
      g_configured_anti_aliasing_mode.load(std::memory_order_acquire);
  if (UsesSceneSupersampling(*requested) &&
      rex::cvar::GetFlagByName("gta4_native_upscaler") != "native") {
    REXLOG_ERROR(
        "GTA4AAPolicy owner=frontend event=reject requested={} reason=upscaler-conflict "
        "upscaler={}",
        value, rex::cvar::GetFlagByName("gta4_native_upscaler"));
    return AntiAliasingApplyResult::kRejected;
  }
  if (!ValidateSupersamplingSelection(*requested)) {
    return AntiAliasingApplyResult::kRejected;
  }
  const bool apply_live = CanApplyAntiAliasingLive(active, *requested);

  if (!rex::cvar::SetFlagByName("gta4_native_anti_aliasing_unified", "true")) {
    REXLOG_ERROR("GTA4AAPolicy owner=frontend event=reject requested={} reason=unified-marker",
                 value);
    return AntiAliasingApplyResult::kRejected;
  }
  if (!apply_live) {
    std::string_view legacy_scene_msaa = "original";
    if (*requested == AntiAliasingMode::kMsaa2x) {
      legacy_scene_msaa = "2x";
    } else if (*requested == AntiAliasingMode::kMsaa4x) {
      legacy_scene_msaa = "4x";
    }
    // SetFlagByName intentionally records a pending restart even if the value
    // already matches. The active renderer route remains latched separately.
    if (!rex::cvar::SetFlagByName("gta4_native_msaa", legacy_scene_msaa)) {
      REXLOG_ERROR(
          "GTA4AAPolicy owner=frontend event=reject requested={} reason=restart-tracking",
          value);
      return AntiAliasingApplyResult::kRejected;
    }
  }
  if (!rex::cvar::SetFlagByName("gta4_native_anti_aliasing",
                                AntiAliasingModeName(*requested))) {
    REXLOG_ERROR("GTA4AAPolicy owner=frontend event=reject requested={} reason=canonical-cvar",
                 value);
    return AntiAliasingApplyResult::kRejected;
  }

  g_configured_anti_aliasing_mode.store(*requested, std::memory_order_release);
  if (apply_live) {
    g_active_anti_aliasing_mode.store(*requested, std::memory_order_release);
  }
  const AntiAliasingMode resulting_active =
      g_active_anti_aliasing_mode.load(std::memory_order_acquire);
  const auto route = GetAntiAliasingRoute(resulting_active);
  REXLOG_INFO(
      "GTA4AAPolicy owner=frontend event=set previous-configured={} requested={} active-before={} "
      "active-after={} apply={} route-fxaa={} route-smaa={} scene-samples={} ssaa-factor={}",
      AntiAliasingModeName(previous_configured), AntiAliasingModeName(*requested),
      AntiAliasingModeName(active), AntiAliasingModeName(resulting_active),
      apply_live ? "live" : "restart", route.presentation_fxaa, route.presentation_smaa,
      route.scene_sample_count, route.supersampling_pixel_factor);
  return apply_live ? AntiAliasingApplyResult::kAppliedLive
                    : AntiAliasingApplyResult::kRestartRequired;
}

}  // namespace rex::graphics::gta4_native

namespace {

bool IsEpisodePackageReady(const std::filesystem::path& marketplace_root,
                           std::string_view package_name) {
  const auto package_path = marketplace_root / package_name;
  std::error_code error;
  const bool has_setup = std::filesystem::is_regular_file(package_path / "setup2.xml", error);
  error.clear();
  const bool has_content = std::filesystem::is_regular_file(package_path / "content.dat", error);
  return has_setup && has_content;
}

void SetStartupFlag(std::string_view name, std::string_view value) {
  if (!rex::cvar::SetFlagByName(name, value)) {
    REXLOG_WARN("GTA IV graphics configuration could not set {}={}", name, value);
  }
}

void ApplyPresentationMode() {
  const auto policy = gta4::presentation::ResolveMode(REXCVAR_GET(gta4_present_mode));
  if (!policy.explicit_mode) return;
  SetStartupFlag("vsync", policy.vsync ? "true" : "false");
  SetStartupFlag("vulkan_prefer_present_mode_fifo", policy.prefer_fifo ? "true" : "false");
  SetStartupFlag("vulkan_allow_present_mode_immediate", policy.immediate ? "true" : "false");
  SetStartupFlag("vulkan_allow_present_mode_mailbox", policy.mailbox ? "true" : "false");
  SetStartupFlag("vulkan_allow_present_mode_fifo_relaxed", "false");
}

void LogEpisodeInstallState(const std::filesystem::path& marketplace_root) {
  constexpr std::array<std::string_view, 2> kEpisodePackages = {"TLAD", "TBOGT"};
  for (const auto package_name : kEpisodePackages) {
    const auto package_path = marketplace_root / package_name;
    if (IsEpisodePackageReady(marketplace_root, package_name)) {
      REXLOG_INFO("GTA IV episode package '{}' is installed and ready at '{}'", package_name,
                  package_path.string());
    } else {
      REXLOG_WARN("GTA IV episode package '{}' is incomplete or missing at '{}'", package_name,
                  package_path.string());
    }
  }
}

}  // namespace

struct GTA4App::AchievementSyncState {
  std::mutex mutex;
  std::condition_variable condition;
  std::unordered_set<uint32_t> pending_uploads;
  bool stopping = false;
};

struct GTA4App::TitleProfileSyncState {
  std::mutex mutex;
  std::condition_variable condition;
  std::optional<std::vector<uint8_t>> pending_blob;
  uint64_t startup_generation = 0;
  uint64_t pending_generation = 0;
  bool stopping = false;
};

std::optional<rex::PathConfig> GTA4App::OnFinalizePaths(
    const rex::PathConfig& defaults, std::function<void(rex::PathConfig)> resume) {
  auto paths = defaults;
  paths.game_data_root = liberty_root_ / "game";
  paths.marketplace_content_root = liberty_root_ / "dlc";
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
  rex::input::mnk::SetNativeControllerCompatibilityBindings(
      gta4::input::KeyboardControllerBindings());
  if (!config.graphics && config.gpu_plugin.empty()) {
    config.gpu_plugin = "gta4-native";
  }

  ApplyPresentationMode();
  rex::graphics::gta4_native::InitializeHdrController();

  const bool ssaa_active =
      rex::graphics::gta4_native::UsesSceneSupersampling(
          rex::graphics::gta4_native::GetActiveAntiAliasingMode());
  const bool fsr1_requested = REXCVAR_GET(gta4_native_upscaler) == "fsr1";
  const bool use_fsr1 = fsr1_requested && !ssaa_active;
  if (fsr1_requested && ssaa_active) {
    REXLOG_ERROR(
        "GTA4AAPolicy owner=startup event=reject-upscaler upscaler=fsr1 aa={} "
        "reason=ssaa-renders-above-output-resolution",
        rex::graphics::gta4_native::GetActiveAntiAliasingModeName());
  }
  REXCVAR_SET(present_effect, use_fsr1 ? "fsr" : "bilinear");
  REXCVAR_SET(present_fsr_sharpness_reduction, REXCVAR_GET(gta4_fsr1_sharpness_reduction));
  REXLOG_INFO(
      "GTA IV native image quality: aa={} upscaler={} fsr1-quality={} "
      "fsr1-sharpness-reduction={} aspect={} present-mode={} hdr-mode={} hdr-transport={} "
      "paper-white-nits={} peak-nits={}",
      REXCVAR_GET(gta4_native_anti_aliasing), REXCVAR_GET(gta4_native_upscaler),
      REXCVAR_GET(gta4_fsr1_quality), REXCVAR_GET(gta4_fsr1_sharpness_reduction),
      REXCVAR_GET(gta4_aspect_ratio), REXCVAR_GET(gta4_present_mode),
      rex::graphics::gta4_native::GetConfiguredHdrModeName(),
      rex::cvar::Query<bool>("vulkan_hdr"), REXCVAR_GET(gta4_native_hdr_paper_white_nits),
      REXCVAR_GET(gta4_native_hdr_peak_nits));

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
  config.live.voice_audio_device = gta4::voice::CreateAudioDevice();
  config.live.voice_sample_codec = gta4::voice::CreateSampleCodec();
  const std::weak_ptr<rex::system::xam::IVoiceAudioDevice> voice_device =
      config.live.voice_audio_device;
  config.live.voice_capture_available = [voice_device] {
    const auto device = voice_device.lock();
    return device && device->capture_available();
  };
}

void GTA4App::OnPostSetup() {
  gta4::presentation::InitializeOptions();
  rex::graphics::gta4_native::InitializeAntiAliasingController();
  gta4::input::InitializeContextTouchControls();
  if (REXCVAR_GET(gta4_diagnostics_skip_user_music)) {
    gta4::input::PublishUserMusicPlayer(nullptr);
    REXLOG_INFO("gta4-user-music: skipped for isolated diagnostics");
  } else {
    user_music_player_ =
        std::make_unique<gta4::input::UserMusicPlayer>(liberty_root_ / "User Music");
    gta4::input::PublishUserMusicPlayer(user_music_player_.get());
  }
  if (text_chat_dialog_) {
    text_chat_dialog_->AttachLive(
        runtime()->kernel_state()->live_compatibility());
  }
  gta4::frontend_menu::SetConfigPath(native_config_path_);
  auto* kernel_state = runtime()->kernel_state();
  auto* live = kernel_state->live_compatibility();
  kernel_state->content_manager()->SetMarketplacePackageAllowlist(std::nullopt);
  REXLOG_INFO("GTA IV episode visibility policy is install-based; community entitlements do not "
              "filter local marketplace packages");
  if (live && live->config().backend == rex::system::xam::LiveBackend::kCommunity) {
    entitlement_service_ = live->entitlement_service();
    auto refresh_entitlements = [entitlement_service = entitlement_service_] {
      const auto packages = entitlement_service
                                ? entitlement_service->FetchEpisodePackages()
                                : std::optional<std::vector<std::string>>{};
      if (packages) {
        REXLOG_INFO(
            "GTA IV community reported {} episode entitlement(s); local episode enumeration "
            "remains install-based",
            packages->size());
      } else {
        REXLOG_WARN(
            "GTA IV community entitlement lookup failed; local installed episodes remain "
            "available");
      }
    };
    if (entitlement_service_) {
      entitlement_service_->SetConnectionRestoredHandler(refresh_entitlements);
      refresh_entitlements();
    } else {
      REXLOG_WARN(
          "GTA IV community entitlement service is unavailable; local installed episodes remain "
          "available");
    }
  }
  LogEpisodeInstallState(runtime()->marketplace_content_root());
  gta4::button_prompts::PrepareAndMount(*runtime(), game_data_root(), cache_root());

  achievement_service_ = live && live->config().backend == rex::system::xam::LiveBackend::kCommunity
                             ? live->achievement_service()
                             : nullptr;
  if (achievement_service_)
    achievement_sync_state_ = std::make_shared<AchievementSyncState>();
  const std::weak_ptr<AchievementSyncState> weak_sync_state = achievement_sync_state_;
  achievement_listener_ = kernel_state->RegisterAchievementUnlockCallback(
      [weak_sync_state](const rex::system::AchievementInfo& achievement) {
        gta4::game_center::SubmitAchievement(achievement.id);
        if (const auto sync_state = weak_sync_state.lock()) {
          {
            std::lock_guard lock(sync_state->mutex);
            if (sync_state->stopping)
              return;
            sync_state->pending_uploads.insert(achievement.id);
          }
          sync_state->condition.notify_all();
        }
      });

  for (const auto& achievement : kernel_state->loaded_achievements()) {
    if (kernel_state->IsAchievementUnlocked(achievement.id)) {
      gta4::game_center::SubmitAchievement(achievement.id);
      QueueAchievementUpload(achievement.id);
    }
  }

  if (achievement_service_) {
    try {
      achievement_sync_worker_ = std::thread(&GTA4App::AchievementSyncWorkerMain, this);
    } catch (...) {
      REXLOG_ERROR("GTA IV community achievement synchronization could not start");
      achievement_service_ = nullptr;
      achievement_sync_state_.reset();
    }
  }

  title_profile_service_ =
      live && live->config().backend == rex::system::xam::LiveBackend::kCommunity
          ? live->title_profile_service()
          : nullptr;
  if (title_profile_service_) {
    title_profile_sync_state_ = std::make_shared<TitleProfileSyncState>();
    title_profile_sync_state_->startup_generation =
        kernel_state->user_profile()->gta4_title_profile_generation();
    const std::weak_ptr<TitleProfileSyncState> weak_profile_state = title_profile_sync_state_;
    kernel_state->user_profile()->SetGta4TitleProfileWriteCallback(
        [weak_profile_state](std::vector<uint8_t> blob, uint64_t generation) {
          const auto state = weak_profile_state.lock();
          if (!state)
            return;
          {
            std::lock_guard lock(state->mutex);
            if (state->stopping)
              return;
            state->pending_blob = std::move(blob);
            state->pending_generation = generation;
          }
          state->condition.notify_all();
        });
    try {
      title_profile_sync_worker_ = std::thread(&GTA4App::TitleProfileSyncWorkerMain, this);
    } catch (...) {
      REXLOG_ERROR("GTA IV community title-profile synchronization could not start");
      kernel_state->user_profile()->SetGta4TitleProfileWriteCallback({});
      title_profile_sync_state_.reset();
      title_profile_service_ = nullptr;
    }
  }
}

void GTA4App::QueueAchievementUpload(uint32_t achievement_id) {
  const auto sync_state = achievement_sync_state_;
  if (!sync_state)
    return;
  {
    std::lock_guard lock(sync_state->mutex);
    if (!achievement_service_ || sync_state->stopping)
      return;
    sync_state->pending_uploads.insert(achievement_id);
  }
  sync_state->condition.notify_all();
}

void GTA4App::AchievementSyncWorkerMain() {
  constexpr auto kRetryDelay = std::chrono::seconds(5);
  const auto sync_state = achievement_sync_state_;
  if (!sync_state)
    return;
  bool fetched_remote_unlocks = false;
  for (;;) {
    if (!fetched_remote_unlocks) {
      if (auto remote = achievement_service_->FetchUnlockedAchievements()) {
        std::size_t imported = 0;
        for (const uint32_t achievement_id : *remote) {
          if (runtime()->kernel_state()->achievements().ImportUnlockedAchievement(achievement_id) ==
              rex::system::AchievementUnlockResult::kUnlocked) {
            ++imported;
          }
        }
        REXLOG_INFO("GTA IV community achievements merged: remote={} imported={}", remote->size(),
                    imported);
        fetched_remote_unlocks = true;
      }
    }

    std::vector<uint32_t> pending;
    {
      std::unique_lock lock(sync_state->mutex);
      if (sync_state->pending_uploads.empty()) {
        sync_state->condition.wait_for(lock, kRetryDelay, [&] {
          return sync_state->stopping || !sync_state->pending_uploads.empty();
        });
      }
      if (sync_state->stopping)
        return;
      pending.assign(sync_state->pending_uploads.begin(), sync_state->pending_uploads.end());
      sync_state->pending_uploads.clear();
    }

    if (pending.empty())
      continue;
    if (achievement_service_->MergeUnlockedAchievements(pending)) {
      REXLOG_INFO("GTA IV community achievements uploaded: count={}", pending.size());
      continue;
    }

    std::unique_lock lock(sync_state->mutex);
    sync_state->pending_uploads.insert(pending.begin(), pending.end());
    sync_state->condition.wait_for(lock, kRetryDelay, [&] { return sync_state->stopping; });
    if (sync_state->stopping)
      return;
  }
}

void GTA4App::TitleProfileSyncWorkerMain() {
  using rex::system::xam::TitleProfileFetchStatus;
  using rex::system::xam::TitleProfileStoreStatus;
  constexpr auto kRetryDelay = std::chrono::seconds(5);
  constexpr uint32_t kTitleId = rex::graphics::gta4_native::kTitleId;

  const auto state = title_profile_sync_state_;
  auto* user_profile = runtime()->kernel_state()->user_profile();
  auto* live = runtime()->kernel_state()->live_compatibility();
  if (!state || !user_profile || !live)
    return;

  auto wait_for_retry = [&] {
    std::unique_lock lock(state->mutex);
    state->condition.wait_for(lock, kRetryDelay, [&] { return state->stopping; });
    return state->stopping;
  };

  const uint64_t startup_generation = state->startup_generation;
  std::optional<int64_t> known_revision;
  while (!known_revision) {
    {
      std::lock_guard lock(state->mutex);
      if (state->stopping)
        return;
    }

    const auto fetched = title_profile_service_->Fetch(kTitleId);
    if (fetched.status == TitleProfileFetchStatus::kFound && fetched.record &&
        fetched.record->title_id == kTitleId && fetched.record->xuid == live->identity().xuid &&
        fetched.record->revision >= 0 &&
        rex::system::xam::UserProfile::ValidateGta4TitleProfileBlob(fetched.record->blob)) {
      known_revision = fetched.record->revision;
      if (user_profile->ImportGta4TitleProfileBlobIfGeneration(fetched.record->blob,
                                                               startup_generation)) {
        REXLOG_INFO("GTA IV community title profile imported: revision={} bytes={}",
                    fetched.record->revision, fetched.record->blob.size());
      } else {
        REXLOG_INFO(
            "GTA IV community title profile kept a newer local guest write over revision {}",
            fetched.record->revision);
      }
      break;
    }

    if (fetched.status == TitleProfileFetchStatus::kNotFound && !fetched.record) {
      known_revision = 0;
      if (user_profile->gta4_title_profile_generation() == startup_generation) {
        if (auto local_blob = user_profile->SnapshotGta4TitleProfileBlob()) {
          std::lock_guard lock(state->mutex);
          if (!state->pending_blob) {
            state->pending_blob = std::move(*local_blob);
            state->pending_generation = startup_generation;
          }
        }
      }
      break;
    }

    REXLOG_WARN("GTA IV community title-profile fetch failed; retrying");
    if (wait_for_retry())
      return;
  }

  for (;;) {
    std::vector<uint8_t> blob;
    uint64_t generation = 0;
    {
      std::unique_lock lock(state->mutex);
      state->condition.wait(lock,
                            [&] { return state->stopping || state->pending_blob.has_value(); });
      if (state->stopping)
        return;
      blob = *state->pending_blob;
      generation = state->pending_generation;
    }

    const auto stored = title_profile_service_->Store(kTitleId, *known_revision, blob);
    if (stored.status == TitleProfileStoreStatus::kUpdated && stored.revision >= 0 &&
        !stored.conflict_record) {
      known_revision = stored.revision;
      {
        std::lock_guard lock(state->mutex);
        if (state->pending_blob && state->pending_generation == generation) {
          state->pending_blob.reset();
        }
      }
      REXLOG_INFO("GTA IV community title profile uploaded: revision={} bytes={}",
                  stored.revision, blob.size());
      continue;
    }

    if (stored.status == TitleProfileStoreStatus::kConflict && stored.conflict_record &&
        stored.conflict_record->title_id == kTitleId &&
        stored.conflict_record->xuid == live->identity().xuid &&
        stored.conflict_record->revision >= 0 &&
        rex::system::xam::UserProfile::ValidateGta4TitleProfileBlob(
            stored.conflict_record->blob)) {
      // A local guest write is authoritative once startup reconciliation has
      // completed. Retry the same whole blob against the server's new CAS
      // revision; a still-newer callback will replace it before the retry.
      known_revision = stored.conflict_record->revision;
      REXLOG_INFO("GTA IV community title-profile CAS conflict at revision {}; retrying",
                  *known_revision);
      continue;
    }

    REXLOG_WARN("GTA IV community title-profile upload failed; retrying");
    {
      std::unique_lock lock(state->mutex);
      state->condition.wait_for(lock, kRetryDelay, [&] {
        return state->stopping || state->pending_generation != generation;
      });
      if (state->stopping)
        return;
    }
  }
}

void GTA4App::OnCreateDialogs(rex::ui::ImGuiDrawer* drawer) {
  gta4::game_center::Initialize();
  context_touch_overlay_ =
      std::make_unique<gta4::input::ContextTouchOverlay>(drawer);
  text_chat_dialog_ = std::make_unique<gta4::input::TextChatDialog>(
      drawer, [this](bool captured) {
        SetTitleInputCaptured(captured);
        GTA4_SetTouchTitleInputOwned(captured);
      });
  rex::ui::RegisterBind("bind_gta4_team_chat", "U", "Open GTA IV team text chat",
                        [this] {
                          if (text_chat_dialog_) {
                            text_chat_dialog_->RequestOpen(
                                rex::system::xam::TextChatChannel::kTeam);
                            REXLOG_INFO("gta4-pc-input: action=text-chat-team key=U");
                          }
                        });
  rex::ui::RegisterBind("bind_gta4_all_chat", "Y", "Open GTA IV all text chat",
                        [this] {
                          if (text_chat_dialog_) {
                            text_chat_dialog_->RequestOpen(
                                rex::system::xam::TextChatChannel::kAll);
                            REXLOG_INFO("gta4-pc-input: action=text-chat-all key=Y");
                          }
                        });
}

bool GTA4App::RequiresSynchronizedInitialThreadResume() const {
  auto* graphics_system = runtime() ? runtime()->graphics_system() : nullptr;
  return graphics_system &&
         graphics_system->GetTitleCommandAbi(rex::graphics::gta4_native::kTitleId) ==
             rex::graphics::gta4_native::kTitleCommandAbi;
}

void GTA4App::OnShutdown() {
  if (entitlement_service_) {
    entitlement_service_->SetConnectionRestoredHandler({});
    entitlement_service_ = nullptr;
  }
  gta4::input::ShutdownContextTouchControls();
  context_touch_overlay_.reset();
  rex::ui::UnregisterBind("bind_gta4_all_chat");
  rex::ui::UnregisterBind("bind_gta4_team_chat");
  SetTitleInputCaptured(false);
  GTA4_SetTouchTitleInputOwned(false);
  if (text_chat_dialog_) {
    text_chat_dialog_->Stop();
    text_chat_dialog_.reset();
  }
  gta4::input::PublishUserMusicPlayer(nullptr);
  if (user_music_player_) {
    user_music_player_->Stop();
    user_music_player_.reset();
  }
  if (runtime() != nullptr) {
    runtime()->kernel_state()->user_profile()->SetGta4TitleProfileWriteCallback({});
  }
  const auto profile_state = title_profile_sync_state_;
  if (profile_state) {
    {
      std::lock_guard lock(profile_state->mutex);
      profile_state->stopping = true;
    }
    profile_state->condition.notify_all();
  }
  if (title_profile_sync_worker_.joinable())
    title_profile_sync_worker_.join();
  title_profile_sync_state_.reset();
  title_profile_service_ = nullptr;

  if (runtime() != nullptr && achievement_listener_ != 0) {
    runtime()->kernel_state()->achievements().UnregisterCallback(achievement_listener_);
    achievement_listener_ = 0;
  }

  const auto sync_state = achievement_sync_state_;
  if (sync_state) {
    {
      std::lock_guard lock(sync_state->mutex);
      sync_state->stopping = true;
    }
    sync_state->condition.notify_all();
  }
  if (achievement_sync_worker_.joinable())
    achievement_sync_worker_.join();
  achievement_sync_state_.reset();
  achievement_service_ = nullptr;
}
