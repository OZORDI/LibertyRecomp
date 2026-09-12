#include "gta4_quicksave_hooks.h"
#include "gta4_aspect_hooks.h"
#include "gta4_frontend_hooks.h"

#include <array>
#include <bit>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <limits>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

#include <rex/cvar.h>
#include <rex/diagnostics/policy.h>
#include <rex/graphics/gta4_native/anti_aliasing_policy.h>
#include <rex/graphics/gta4_native/hdr_policy.h>
#include <rex/input/input_trace.h>
#include <rex/logging.h>
#include <rex/runtime.h>
#include <rex/system/kernel_state.h>

#include "gta4_frontend_menu_policy.h"
#include "gta4_init.h"
#include "gta4_touch_coordinator.h"

REXCVAR_DECLARE(std::string, gta4_episode_startup_prompt);
REXCVAR_DEFINE_BOOL(gta4_frontend_advanced_graphics_menu, true, "GTA IV/Frontend",
                    "Add an Advanced Graphics page without exceeding the retail 20-row list widget")
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);

void GTA4_RunWithPrimaryPlayerInfoAlias(PPCContext& ctx, uint8_t* base,
                                        void (*function)(PPCContext&, uint8_t*));

namespace {

constexpr uint32_t kDisplayScreen = 8;
constexpr uint32_t kAudioScreen = 7;
constexpr uint32_t kGameScreen = 9;
constexpr uint32_t kScreenDescriptorsAddress = 0x831DAE28;
constexpr uint32_t kScreenDescriptorSize = 24;
constexpr uint32_t kScreenOptionsOffset = 16;
constexpr uint32_t kDisplayOptionsPointer = 0x831DAEF8;
constexpr uint32_t kDisplayOptionsCount = 0x831DAEFC;
constexpr uint32_t kDisplayOptionsCapacity = 0x831DAEFE;
constexpr uint32_t kCurrentScreenAddress = 0x82BFA124;
constexpr uint32_t kOptionRecordSize = 22;
constexpr uint32_t kOptionActionOffset = 0;
constexpr uint32_t kOptionLabelOffset = 1;
constexpr uint32_t kOptionLabelCapacity = 16;
constexpr uint32_t kOptionValueOffset = 18;
constexpr uint32_t kOptionScalerOffset = 20;
constexpr uint32_t kOptionDisplayValueOffset = 21;
constexpr uint8_t kMenuOptionAdjust = 1;
constexpr uint8_t kMenuOptionJump = 10;
constexpr uint8_t kEndOfMenuOptions = 36;
constexpr uint16_t kSafeStockPreference = 0;
constexpr uint8_t kSafeStockDisplayValue = 0;
constexpr uint32_t kAdjustmentDeltaReturnAddress = 0x8225903C;
constexpr uint32_t kMenuTraceStringCapacity = 128;
constexpr uint32_t kMenuTraceRowLimit = 64;
constexpr uint32_t kMenuTraceCallLimit = 256;
constexpr uint32_t kUiRowRecordSize = 68;
constexpr uint32_t kUiRowLabelCapacity = 60;
constexpr uint32_t kLeftListBuildReturnAddress = 0x82255E38;
constexpr uint32_t kFrontendWidgetPointerTable = 0x82CC7BD0;
constexpr uint32_t kFrontendWidgetActiveCountOffset = 3020;
constexpr uint32_t kFrontendWidgetSelectedOffset = 3196;
constexpr uint32_t kFrontendWidgetTransitionProgressOffset = 3208;
constexpr uint32_t kFrontendWidgetTransitionAnchorOffset = 3212;
constexpr uint32_t kFrontendWidgetStateSize = 3216;
// sub_8229CD40 fills and clears exactly 20 fixed list slots in the retail UI
// object. Keep this diagnostic constant tied to that generated-code bound.
constexpr int32_t kRetailListSlotCapacity = gta4::frontend_menu::policy::kRetailListSlotCapacity;

enum class TextId : uint8_t {
  kResolutionLabel,
  kAspectLabel,
  kFullscreenLabel,
  kPresentationLabel,
  kFrameLimitLabel,
  kHdrLabel,
  kPaperWhiteLabel,
  kPeakBrightnessLabel,
  kPostAaLabel,
  kUpscalingLabel,
  kFsrLabel,
  kTextureFilteringLabel,
  kAnisotropicLabel,
  kReflectionResolutionLabel,
  kReflectionAaLabel,
  kShadowLabel,
  kDitherLabel,
  kModernShadersLabel,
  kMotionControlsLabel,
  kSkipIntroLabel,
  kFilmGrainLabel,
  kAdvancedLabel,
  kSaveLabel,
  kBackLabel,
  kPreviousPageLabel,
  kNextPageLabel,
  kAutomatic,
  kResolution720p,
  kResolution1080p,
  kResolution1440p,
  kResolution4k,
  kDisplay,
  kOriginalAspect,
  kAspect16x10,
  kAspect3x2,
  kAspect4x3,
  kAspect5x4,
  kAspect21x9,
  kAspect32x9,
  kAspect43x18,
  kAspect32x10,
  kWindowed,
  kFullscreen,
  kOff,
  kOn,
  kScRgb,
  kAutoHdr,
  k80Nits,
  k100Nits,
  k160Nits,
  k203Nits,
  k250Nits,
  k300Nits,
  k400Nits,
  k500Nits,
  k600Nits,
  k800Nits,
  k1000Nits,
  k1500Nits,
  k2000Nits,
  kMailbox,
  kFxaa,
  kSmaa,
  kMsaa2x,
  kMsaa4x,
  kSsaa2x,
  kSsaa4x,
  kSsaa6x,
  kSsaa8x,
  kSsaa10x,
  kSsaa12x,
  kSsaa14x,
  kSsaa16x,
  kLow,
  kMedium,
  kHigh,
  kUltra,
  kNative,
  kFsr1,
  kUltraQuality,
  kQuality,
  kBalanced,
  kPerformance,
  kOriginal,
  kBilinear,
  kTrilinear,
  k1x,
  k2x,
  k4x,
  k8x,
  k16x,
  k1080p,
  kFull,
  k30Fps,
  k60Fps,
  k120Fps,
  kUnlocked,
  kCount,
};

struct Choice {
  std::string_view value;
  TextId text;
};

enum class SettingBinding : uint8_t {
  kCvar,
  kAntiAliasing,
  kHdr,
  kUpscaler,
};

struct Setting {
  std::string_view key;
  TextId label;
  std::string_view cvar;
  const Choice* choices;
  uint8_t choice_count;
  bool restart_required = false;
  SettingBinding binding = SettingBinding::kCvar;
};

constexpr std::array kResolutionChoices = {
    Choice{"", TextId::kAutomatic},
    Choice{"720p", TextId::kResolution720p},
    Choice{"1080p", TextId::kResolution1080p},
    Choice{"1440p", TextId::kResolution1440p},
    Choice{"4k", TextId::kResolution4k},
};
constexpr std::array kAspectChoices = {
    Choice{"auto", TextId::kDisplay},
    Choice{"16:9", TextId::kOriginalAspect},
    Choice{"16:10", TextId::kAspect16x10},
    Choice{"3:2", TextId::kAspect3x2},
    Choice{"4:3", TextId::kAspect4x3},
    Choice{"5:4", TextId::kAspect5x4},
    Choice{"21:9", TextId::kAspect21x9},
    Choice{"32:9", TextId::kAspect32x9},
    Choice{"43:18", TextId::kAspect43x18},
    Choice{"32:10", TextId::kAspect32x10},
};
constexpr std::array kToggleChoices = {
    Choice{"false", TextId::kOff},
    Choice{"true", TextId::kOn},
};
constexpr std::array kFilmGrainChoices = {
    Choice{"true", TextId::kOff},
    Choice{"false", TextId::kOn},
};
constexpr std::array kHdrChoices = {
    Choice{"off", TextId::kOff},
    Choice{"scrgb", TextId::kScRgb},
    Choice{"auto_hdr", TextId::kAutoHdr},
};
constexpr std::array kPaperWhiteChoices = {
    Choice{"80", TextId::k80Nits},
    Choice{"100", TextId::k100Nits},
    Choice{"160", TextId::k160Nits},
    Choice{"203", TextId::k203Nits},
    Choice{"250", TextId::k250Nits},
    Choice{"300", TextId::k300Nits},
    Choice{"400", TextId::k400Nits},
    Choice{"500", TextId::k500Nits},
};
constexpr std::array kPeakBrightnessChoices = {
    Choice{"400", TextId::k400Nits},
    Choice{"600", TextId::k600Nits},
    Choice{"800", TextId::k800Nits},
    Choice{"1000", TextId::k1000Nits},
    Choice{"1500", TextId::k1500Nits},
    Choice{"2000", TextId::k2000Nits},
};
constexpr std::array kDisplayModeChoices = {
    Choice{"false", TextId::kWindowed},
    Choice{"true", TextId::kFullscreen},
};
constexpr std::array kPresentChoices = {
    Choice{"auto", TextId::kAutomatic},
    Choice{"vsync", TextId::kOn},
    Choice{"mailbox", TextId::kMailbox},
    Choice{"immediate", TextId::kOff},
};
constexpr std::array kFrameLimitChoices = {
    Choice{"30", TextId::k30Fps},
    Choice{"60", TextId::k60Fps},
    Choice{"120", TextId::k120Fps},
    Choice{"0", TextId::kUnlocked},
};
constexpr std::array kAntiAliasingChoices = {
    Choice{"off", TextId::kOff},
    Choice{"fxaa", TextId::kFxaa},
    Choice{"smaa", TextId::kSmaa},
    Choice{"msaa2x", TextId::kMsaa2x},
    Choice{"msaa4x", TextId::kMsaa4x},
    Choice{"ssaa2x", TextId::kSsaa2x},
    Choice{"ssaa4x", TextId::kSsaa4x},
    Choice{"ssaa6x", TextId::kSsaa6x},
    Choice{"ssaa8x", TextId::kSsaa8x},
    Choice{"ssaa10x", TextId::kSsaa10x},
    Choice{"ssaa12x", TextId::kSsaa12x},
    Choice{"ssaa14x", TextId::kSsaa14x},
    Choice{"ssaa16x", TextId::kSsaa16x},
};
constexpr std::array kUpscalerChoices = {
    Choice{"native", TextId::kNative},
    Choice{"fsr1", TextId::kFsr1},
};
constexpr std::array kFsrQualityChoices = {
    Choice{"ultra_quality", TextId::kUltraQuality},
    Choice{"quality", TextId::kQuality},
    Choice{"balanced", TextId::kBalanced},
    Choice{"performance", TextId::kPerformance},
};
constexpr std::array kTextureFilteringChoices = {
    Choice{"bilinear", TextId::kBilinear},
    Choice{"trilinear", TextId::kTrilinear},
};
constexpr std::array kAnisotropicChoices = {
    Choice{"1x", TextId::k1x},
    Choice{"2x", TextId::k2x},
    Choice{"4x", TextId::k4x},
    Choice{"8x", TextId::k8x},
    Choice{"16x", TextId::k16x},
};
constexpr std::array kReflectionChoices = {
    Choice{"original", TextId::kOriginal},
    Choice{"1080p", TextId::k1080p},
    Choice{"full", TextId::kFull},
};
constexpr std::array kReflectionAaChoices = {
    Choice{"original", TextId::kOriginal},
    Choice{"off", TextId::kOff},
    Choice{"2x", TextId::k2x},
    Choice{"4x", TextId::k4x},
};
constexpr std::array kShadowChoices = {
    Choice{"256", TextId::kLow},
    Choice{"512", TextId::kHigh},
    Choice{"1024", TextId::kUltra},
};
constexpr std::array kSettings = {
    Setting{"LR_FULLSCR", TextId::kFullscreenLabel, "fullscreen", kDisplayModeChoices.data(),
            kDisplayModeChoices.size(), true},
    Setting{"LR_RES", TextId::kResolutionLabel, "resolution", kResolutionChoices.data(),
            kResolutionChoices.size(), true},
    Setting{"LR_ASPECT", TextId::kAspectLabel, "gta4_aspect_ratio", kAspectChoices.data(),
            kAspectChoices.size(), true},
    Setting{"LR_HDR", TextId::kHdrLabel, "gta4_native_hdr_mode", kHdrChoices.data(),
            kHdrChoices.size(), false, SettingBinding::kHdr},
    Setting{"LR_HDR_WHITE", TextId::kPaperWhiteLabel,
            "gta4_native_hdr_paper_white_nits", kPaperWhiteChoices.data(),
            kPaperWhiteChoices.size()},
    Setting{"LR_HDR_PEAK", TextId::kPeakBrightnessLabel,
            "gta4_native_hdr_peak_nits", kPeakBrightnessChoices.data(),
            kPeakBrightnessChoices.size()},
    Setting{"LR_PRESENT", TextId::kPresentationLabel, "gta4_present_mode", kPresentChoices.data(),
            kPresentChoices.size(), true},
    Setting{"LR_FPS", TextId::kFrameLimitLabel, "gta4_frame_limit", kFrameLimitChoices.data(),
            kFrameLimitChoices.size()},
    Setting{"LR_UPSCALE", TextId::kUpscalingLabel, "gta4_native_upscaler", kUpscalerChoices.data(),
            kUpscalerChoices.size(), true, SettingBinding::kUpscaler},
    Setting{"LR_FSR", TextId::kFsrLabel, "gta4_fsr1_quality", kFsrQualityChoices.data(),
            kFsrQualityChoices.size(), true},
    Setting{"LR_AA", TextId::kPostAaLabel, "gta4_native_anti_aliasing", kAntiAliasingChoices.data(),
            kAntiAliasingChoices.size(), false, SettingBinding::kAntiAliasing},
    Setting{"LR_TEXFILTER", TextId::kTextureFilteringLabel, "gta4_texture_filtering",
            kTextureFilteringChoices.data(), kTextureFilteringChoices.size()},
    Setting{"LR_ANISO", TextId::kAnisotropicLabel, "gta4_anisotropic_filtering",
            kAnisotropicChoices.data(), kAnisotropicChoices.size()},
    Setting{"LR_SHADOW", TextId::kShadowLabel, "gta4_shadow_map_base_size", kShadowChoices.data(),
            kShadowChoices.size(), true},
    Setting{"LR_REFL_RES", TextId::kReflectionResolutionLabel, "gta4_reflection_resolution",
            kReflectionChoices.data(), kReflectionChoices.size(), true},
    Setting{"LR_REFL_AA", TextId::kReflectionAaLabel, "gta4_reflection_aa",
            kReflectionAaChoices.data(), kReflectionAaChoices.size(), true},
    Setting{"LR_DITHER", TextId::kDitherLabel, "gta4_native_output_dither", kToggleChoices.data(),
            kToggleChoices.size()},
    Setting{"LR_MODSHADER", TextId::kModernShadersLabel, "gta4_modern_shaders",
            kToggleChoices.data(), kToggleChoices.size()},
    Setting{"LR_MOTION", TextId::kMotionControlsLabel, "gta4_motion_enabled",
            kToggleChoices.data(), kToggleChoices.size()},
    Setting{"LR_SKIPINTRO", TextId::kSkipIntroLabel, "gta4_skip_intro",
            kToggleChoices.data(), kToggleChoices.size(), true},
    Setting{"LR_TLAD_GRAIN", TextId::kFilmGrainLabel, "gta4_disable_tlad_film_grain",
            kFilmGrainChoices.data(), kFilmGrainChoices.size()},
};

constexpr std::string_view kAdvancedKey = "LR_ADVANCED";
constexpr std::string_view kSaveKey = "LR_SAVE";
constexpr std::string_view kBackKey = "LR_BACK";
constexpr std::string_view kPreviousPageKey = "LR_PREV";
constexpr std::string_view kNextPageKey = "LR_NEXT";

constexpr std::array<std::string_view, static_cast<size_t>(TextId::kCount)> kStringPool = {
    "Resolution",
    "Aspect Ratio",
    "Display Mode",
    "Vertical Sync",
    "Frame Rate Limit",
    "HDR",
    "Paper White",
    "Peak Brightness",
    "Anti-Aliasing",
    "Upscaler",
    "Upscaling Quality",
    "Texture Filtering",
    "Anisotropic Filtering",
    "Reflection Resolution",
    "Reflection MSAA",
    "Shadow Resolution",
    "Output Dithering",
    "Modern shaders",
    "Motion Controls",
    "Skip Intro",
    "Film Grain",
    "Advanced",
    "Save",
    "Back",
    "Previous Page",
    "Next Page",
    "Automatic",
    "1280 x 720",
    "1920 x 1080",
    "2560 x 1440",
    "3840 x 2160",
    "Display",
    "16:9",
    "16:10",
    "3:2",
    "4:3",
    "5:4",
    "21:9",
    "32:9",
    "43:18 (3440 x 1440)",
    "32:10",
    "Windowed",
    "Fullscreen",
    "Off",
    "On",
    "scRGB",
    "Auto HDR",
    "80 nits",
    "100 nits",
    "160 nits",
    "203 nits",
    "250 nits",
    "300 nits",
    "400 nits",
    "500 nits",
    "600 nits",
    "800 nits",
    "1000 nits",
    "1500 nits",
    "2000 nits",
    "Mailbox (Low Latency)",
    "FXAA",
    "SMAA",
    "MSAA 2x (Deferred)",
    "MSAA 4x (Deferred)",
    "SSAA 2x",
    "SSAA 4x",
    "SSAA 6x",
    "SSAA 8x",
    "SSAA 10x",
    "SSAA 12x",
    "SSAA 14x",
    "SSAA 16x",
    "Low",
    "Medium",
    "High",
    "Ultra",
    "Native",
    "FSR 1",
    "Ultra Quality",
    "Quality",
    "Balanced",
    "Performance",
    "Original",
    "Bilinear",
    "Trilinear",
    "1x",
    "2x",
    "4x",
    "8x",
    "16x",
    "1080p",
    "Full",
    "30 FPS",
    "60 FPS",
    "120 FPS",
    "Unlocked",
};

struct NativePageState {
  uint32_t rows = 0;
  uint16_t count = 0;
  std::size_t first_setting = 0;
  std::size_t setting_count = 0;
  bool has_previous = false;
  bool has_next = false;
};

struct DisplayMenuState {
  gta4::frontend_menu::policy::Page page = gta4::frontend_menu::policy::Page::kDisabled;
  uint32_t stock_rows = 0;
  uint32_t primary_rows = 0;
  uint32_t string_pool = 0;
  uint32_t allocation = 0;
  uint32_t last_frontend_channel = 0;
  uint16_t stock_count = 0;
  uint16_t stock_capacity = 0;
  uint16_t primary_count = 0;
  uint16_t sentinel_index = 0;
  uint16_t advanced_entry_index = 0;
  std::size_t current_native_page = 0;
  std::vector<NativePageState> native_pages;
  std::array<uint32_t, static_cast<size_t>(TextId::kCount)> text_addresses{};
  bool rebuild_pending = false;
};

DisplayMenuState g_display_menu;
uint32_t g_last_frontend_channel = 0;
std::filesystem::path g_config_path;
uint64_t g_menu_trace_sequence = 0;
uint32_t g_menu_trace_label_calls = 0;
uint32_t g_menu_trace_value_calls = 0;
thread_local uint32_t g_menu_trace_page_screen = std::numeric_limits<uint32_t>::max();

bool FrontendDiagnosticsEnabled() noexcept {
  return rex::diagnostics::IsEnabled(rex::diagnostics::Category::kGuestHooks) &&
         rex::diagnostics::IsEnabled(rex::diagnostics::Category::kLogging);
}

bool IsGuestSpanValid(uint32_t address, std::size_t size) {
  auto* kernel_state = REX_KERNEL_STATE();
  auto* memory = kernel_state ? kernel_state->memory() : nullptr;
  if (!memory || address == 0 || size == 0 || size > std::numeric_limits<uint32_t>::max()) {
    return false;
  }
  const uint64_t end = static_cast<uint64_t>(address) + size - 1;
  if (end > std::numeric_limits<uint32_t>::max()) {
    return false;
  }
  const auto* first_heap = memory->LookupHeap(address);
  return first_heap != nullptr && first_heap == memory->LookupHeap(static_cast<uint32_t>(end));
}

struct FrontendWidgetState {
  uint32_t widget = 0;
  int32_t active_count = 0;
  int32_t list_count = 0;
  int32_t selected = 0;
  int32_t navigation_delta = 0;
  uint32_t transition_progress_bits = 0;
  int32_t transition_anchor = 0;
  int32_t first_enabled_row = -1;
  int32_t last_enabled_row = -1;
  int32_t selected_marker_row = -1;
  bool valid = false;
};

FrontendWidgetState g_owned_scroll_trace_state;
uint32_t g_owned_scroll_trace_channel = std::numeric_limits<uint32_t>::max();

struct FrontendLayoutBoundsTrace {
  uint32_t channel = std::numeric_limits<uint32_t>::max();
  uint32_t caller = 0;
  int32_t first_row = -1;
  int32_t end_row = -1;
  bool valid = false;
};

FrontendLayoutBoundsTrace g_owned_layout_bounds_trace;
bool g_owned_navigation_route_logged = false;
bool g_owned_layout_route_logged = false;
std::size_t g_owned_scroll_window_first = 0;

FrontendWidgetState ReadFrontendWidgetState(uint8_t* base, uint32_t channel) {
  FrontendWidgetState state;
  const uint64_t pointer_address =
      static_cast<uint64_t>(kFrontendWidgetPointerTable) +
      static_cast<uint64_t>(channel) * sizeof(uint32_t);
  if (pointer_address > std::numeric_limits<uint32_t>::max() ||
      !IsGuestSpanValid(static_cast<uint32_t>(pointer_address), sizeof(uint32_t))) {
    return state;
  }
  state.widget = REX_LOAD_U32(static_cast<uint32_t>(pointer_address));
  if (!IsGuestSpanValid(state.widget, kFrontendWidgetStateSize)) {
    return state;
  }
  state.active_count =
      static_cast<int32_t>(REX_LOAD_U32(state.widget + kFrontendWidgetActiveCountOffset));
  state.list_count = static_cast<int32_t>(REX_LOAD_U32(state.widget + 3024));
  state.selected =
      static_cast<int32_t>(REX_LOAD_U32(state.widget + kFrontendWidgetSelectedOffset));
  state.navigation_delta = static_cast<int32_t>(REX_LOAD_U32(state.widget + 3204));
  state.transition_progress_bits =
      REX_LOAD_U32(state.widget + kFrontendWidgetTransitionProgressOffset);
  state.transition_anchor =
      static_cast<int32_t>(REX_LOAD_U32(state.widget + kFrontendWidgetTransitionAnchorOffset));
  for (int32_t row = 0; row < kRetailListSlotCapacity; ++row) {
    if (REX_LOAD_U8(state.widget + 2964 + static_cast<uint32_t>(row)) != 0) {
      if (state.first_enabled_row < 0) {
        state.first_enabled_row = row;
      }
      state.last_enabled_row = row;
    }
    if (REX_LOAD_U8(state.widget + 2984 + static_cast<uint32_t>(row)) != 0) {
      state.selected_marker_row = row;
    }
  }
  state.valid = true;
  return state;
}

const char* ScrollDirection(const FrontendWidgetState& before,
                            const FrontendWidgetState& after) noexcept {
  if (!before.valid || !after.valid || before.selected == after.selected) {
    return "none";
  }
  if (before.active_count == after.active_count && before.active_count > 1) {
    if (before.selected == 0 && after.selected == before.active_count - 1) {
      return "up-wrap";
    }
    if (before.selected == before.active_count - 1 && after.selected == 0) {
      return "down-wrap";
    }
  }
  return after.selected < before.selected ? "up" : "down";
}

void ResetOwnedScrollTrace() noexcept {
  g_owned_scroll_trace_state = {};
  g_owned_scroll_trace_channel = std::numeric_limits<uint32_t>::max();
  g_owned_layout_bounds_trace = {};
  g_owned_scroll_window_first = 0;
}

void TraceOwnedLayoutBounds(uint32_t channel, uint32_t caller, int32_t first_row,
                            int32_t end_row, const FrontendWidgetState& widget) {
  if (!FrontendDiagnosticsEnabled() || !widget.valid) {
    return;
  }
  const bool changed = !g_owned_layout_bounds_trace.valid ||
                       g_owned_layout_bounds_trace.channel != channel ||
                       g_owned_layout_bounds_trace.caller != caller ||
                       g_owned_layout_bounds_trace.first_row != first_row ||
                       g_owned_layout_bounds_trace.end_row != end_row;
  if (!changed) {
    return;
  }
  REXLOG_INFO(
      "GTA4MenuTrace seq={} point=owned-layout-bounds channel={} caller={:08X} ui={:08X} "
      "first-row={} end-row={} viewport-capacity={} active-count={} selected={} "
      "transition-progress={} transition-anchor={} draws-entire-active-range={}",
      ++g_menu_trace_sequence, channel, caller, widget.widget, first_row, end_row,
      g_display_menu.primary_count > 1 ? g_display_menu.primary_count - 1 : 0,
      widget.active_count, widget.selected,
      std::bit_cast<float>(widget.transition_progress_bits), widget.transition_anchor,
      first_row == 0 && end_row == widget.active_count);
  g_owned_layout_bounds_trace = {
      .channel = channel,
      .caller = caller,
      .first_row = first_row,
      .end_row = end_row,
      .valid = true,
  };
}

void TraceOwnedScrollState(std::string_view point, uint32_t channel,
                           const FrontendWidgetState& before,
                           const FrontendWidgetState& after) {
  if (!FrontendDiagnosticsEnabled() || !after.valid) {
    return;
  }
  const bool changed =
      !g_owned_scroll_trace_state.valid || g_owned_scroll_trace_channel != channel ||
      g_owned_scroll_trace_state.widget != after.widget ||
      g_owned_scroll_trace_state.active_count != after.active_count ||
      g_owned_scroll_trace_state.list_count != after.list_count ||
      g_owned_scroll_trace_state.selected != after.selected ||
      g_owned_scroll_trace_state.navigation_delta != after.navigation_delta ||
      g_owned_scroll_trace_state.transition_progress_bits != after.transition_progress_bits ||
      g_owned_scroll_trace_state.transition_anchor != after.transition_anchor ||
      g_owned_scroll_trace_state.first_enabled_row != after.first_enabled_row ||
      g_owned_scroll_trace_state.last_enabled_row != after.last_enabled_row ||
      g_owned_scroll_trace_state.selected_marker_row != after.selected_marker_row;
  if (!changed) {
    return;
  }
  const float transition_progress = std::bit_cast<float>(after.transition_progress_bits);
  REXLOG_INFO(
      "GTA4MenuTrace seq={} point={} channel={} ui={:08X} active-count={} list-count={} "
      "selected-before={} selected-after={} navigation-delta={} transition-progress={} "
      "transition-progress-bits={:08X} transition-anchor-before={} "
      "transition-anchor-after={} first-enabled={} last-enabled={} selected-marker={} "
      "direction={}",
      ++g_menu_trace_sequence, point, channel, after.widget, after.active_count, after.list_count,
      before.valid ? before.selected : -1, after.selected, after.navigation_delta,
      transition_progress,
      after.transition_progress_bits, before.valid ? before.transition_anchor : -1,
      after.transition_anchor, after.first_enabled_row, after.last_enabled_row,
      after.selected_marker_row, ScrollDirection(before, after));
  g_owned_scroll_trace_state = after;
  g_owned_scroll_trace_channel = channel;
}

struct AdjustmentContext {
  bool active = false;
  uint32_t frontend_channel = 0;
  int32_t delta = 0;
};

thread_local AdjustmentContext g_adjustment;

PPCContext InvokeGuest(PPCContext& parent, uint8_t* base, PPCFunc function, uint64_t r3 = 0) {
  PPCContext nested = parent;
  nested.r3.u64 = r3;
  function(nested, base);
  return nested;
}

PPCContext InvokeGuest(PPCContext& parent, uint8_t* base, PPCFunc function, uint64_t r3,
                       uint64_t r4, uint64_t r5) {
  PPCContext nested = parent;
  nested.r3.u64 = r3;
  nested.r4.u64 = r4;
  nested.r5.u64 = r5;
  function(nested, base);
  return nested;
}

std::string ReadGuestString(uint8_t* base, uint32_t address, uint32_t capacity) {
  std::string result;
  if (address == 0) {
    return result;
  }
  result.reserve(capacity);
  for (uint32_t index = 0; index < capacity; ++index) {
    const uint8_t value = REX_LOAD_U8(address + index);
    if (value == 0) {
      break;
    }
    result.push_back(static_cast<char>(value));
  }
  return result;
}

uint32_t ScreenOptionsAddress(uint32_t screen) {
  return kScreenDescriptorsAddress + screen * kScreenDescriptorSize + kScreenOptionsOffset;
}

const char* TraceScreenName(uint32_t screen) {
  if (screen == kAudioScreen) {
    return "audio";
  }
  if (screen == kDisplayScreen) {
    return "display";
  }
  if (screen == kGameScreen) {
    return "game";
  }
  return "other";
}

bool IsBodyTraceScreen(uint32_t screen) {
  return screen == kAudioScreen || screen == kDisplayScreen || screen == kGameScreen;
}

void TraceScreenRows(uint8_t* base, std::string_view point, uint32_t screen) {
  if (!FrontendDiagnosticsEnabled()) {
    return;
  }
  const uint32_t vector = ScreenOptionsAddress(screen);
  const uint32_t rows = REX_LOAD_U32(vector);
  const uint16_t count = REX_LOAD_U16(vector + 4);
  const uint16_t capacity = REX_LOAD_U16(vector + 6);
  const uint64_t sequence = ++g_menu_trace_sequence;
  REXLOG_INFO(
      "GTA4MenuTrace seq={} point={} screen={}({}) vector={:08X} rows={:08X} count={} "
      "capacity={} string-pool={:08X}",
      sequence, point, screen, TraceScreenName(screen), vector, rows, count, capacity,
      g_display_menu.string_pool);
  if (rows == 0 || count > kMenuTraceRowLimit) {
    REXLOG_INFO("GTA4MenuTrace seq={} point={}-rows-skipped reason={} count={}", sequence, point,
                rows == 0 ? "null-vector" : "count-limit", count);
    return;
  }
  for (uint16_t index = 0; index < count; ++index) {
    const uint32_t row = rows + static_cast<uint32_t>(index) * kOptionRecordSize;
    REXLOG_INFO(
        "GTA4MenuTrace seq={} point={}-row screen={} row={} address={:08X} action={} "
        "key='{}' value={} scaler={} display={}",
        sequence, point, screen, index, row, REX_LOAD_U8(row + kOptionActionOffset),
        ReadGuestString(base, row + kOptionLabelOffset, kOptionLabelCapacity),
        REX_LOAD_U16(row + kOptionValueOffset), REX_LOAD_U8(row + kOptionScalerOffset),
        REX_LOAD_U8(row + kOptionDisplayValueOffset));
  }
}

bool GuestStringEquals(uint8_t* base, uint32_t address, std::string_view expected,
                       uint32_t capacity) {
  if (address == 0 || expected.size() >= capacity) {
    return false;
  }
  for (uint32_t index = 0; index < expected.size(); ++index) {
    if (REX_LOAD_U8(address + index) != static_cast<uint8_t>(expected[index])) {
      return false;
    }
  }
  return REX_LOAD_U8(address + expected.size()) == 0;
}

const Setting* FindSettingByKey(uint8_t* base, uint32_t key_address) {
  for (const Setting& setting : kSettings) {
    if (GuestStringEquals(base, key_address, setting.key, kOptionLabelCapacity)) {
      return &setting;
    }
  }
  return nullptr;
}

bool IsDisplayScreen(uint8_t* base) {
  return REX_LOAD_U32(kCurrentScreenAddress) == kDisplayScreen;
}

uint32_t PublishedDisplayRows(uint8_t* base) {
  return REX_LOAD_U32(kDisplayOptionsPointer);
}

const NativePageState* CurrentNativePage() {
  if (g_display_menu.current_native_page >= g_display_menu.native_pages.size()) {
    return nullptr;
  }
  return &g_display_menu.native_pages[g_display_menu.current_native_page];
}

bool OwnsPublishedDisplayDescriptor(uint8_t* base) {
  const uint32_t published_rows = PublishedDisplayRows(base);
  if (g_display_menu.page == gta4::frontend_menu::policy::Page::kPrimary) {
    return published_rows == g_display_menu.primary_rows;
  }
  if (g_display_menu.page == gta4::frontend_menu::policy::Page::kAdvanced) {
    const NativePageState* page = CurrentNativePage();
    return page != nullptr && published_rows == page->rows;
  }
  return false;
}

bool IsOwnedPrimaryPage(uint8_t* base) {
  return g_display_menu.page == gta4::frontend_menu::policy::Page::kPrimary &&
         OwnsPublishedDisplayDescriptor(base);
}

bool IsOwnedAdvancedPage(uint8_t* base) {
  return g_display_menu.page == gta4::frontend_menu::policy::Page::kAdvanced &&
         OwnsPublishedDisplayDescriptor(base);
}

uint32_t OwnedDisplayRowAddress(uint8_t* base, int32_t row_index) {
  if (!OwnsPublishedDisplayDescriptor(base)) {
    return 0;
  }
  if (row_index < 0) {
    return 0;
  }
  const uint16_t published_count = REX_LOAD_U16(kDisplayOptionsCount);
  const NativePageState* native_page = CurrentNativePage();
  const uint16_t expected_count =
      g_display_menu.page == gta4::frontend_menu::policy::Page::kPrimary
          ? g_display_menu.primary_count
          : (native_page ? native_page->count : 0);
  if (published_count != expected_count || static_cast<uint32_t>(row_index) >= expected_count) {
    return 0;
  }
  const uint32_t rows = REX_LOAD_U32(kDisplayOptionsPointer);
  const std::size_t span_bytes = static_cast<std::size_t>(expected_count) * kOptionRecordSize;
  if (!IsGuestSpanValid(rows, span_bytes)) {
    return 0;
  }
  const uint64_t row = static_cast<uint64_t>(rows) +
                       static_cast<uint32_t>(row_index) * kOptionRecordSize;
  if (row > std::numeric_limits<uint32_t>::max()) {
    return 0;
  }
  return static_cast<uint32_t>(row);
}

const Setting* FindSettingByRow(uint8_t* base, int32_t row_index) {
  if (!IsOwnedAdvancedPage(base)) {
    return nullptr;
  }
  const uint32_t row = OwnedDisplayRowAddress(base, row_index);
  if (row == 0 || REX_LOAD_U8(row + kOptionActionOffset) != kMenuOptionAdjust) {
    return nullptr;
  }
  return FindSettingByKey(base, row + kOptionLabelOffset);
}

bool IsOwnedJumpRow(uint8_t* base, int32_t row_index, std::string_view key) {
  const uint32_t row = OwnedDisplayRowAddress(base, row_index);
  return row != 0 && REX_LOAD_U8(row + kOptionActionOffset) == kMenuOptionJump &&
         GuestStringEquals(base, row + kOptionLabelOffset, key, kOptionLabelCapacity);
}

uint32_t TextAddress(TextId text) {
  return g_display_menu.text_addresses[static_cast<size_t>(text)];
}

std::string_view TextString(TextId text) {
  const std::size_t index = static_cast<std::size_t>(text);
  return index < kStringPool.size() ? kStringPool[index] : std::string_view{};
}

bool WriteUiRowLabel(uint8_t* base, uint32_t destination, std::string_view label) {
  if (label.empty() || label.size() >= kUiRowLabelCapacity ||
      !IsGuestSpanValid(destination, kUiRowLabelCapacity)) {
    return false;
  }
  std::memset(base + destination, 0, kUiRowLabelCapacity);
  std::memcpy(base + destination, label.data(), label.size());
  return true;
}

bool FindOwnedLabelText(uint8_t* base, uint32_t key_address, TextId& text) {
  if (!OwnsPublishedDisplayDescriptor(base)) {
    return false;
  }
  if (IsOwnedPrimaryPage(base) &&
      GuestStringEquals(base, key_address, kAdvancedKey, kOptionLabelCapacity)) {
    text = TextId::kAdvancedLabel;
    return true;
  }
  if (!IsOwnedAdvancedPage(base)) {
    return false;
  }
  if (const Setting* setting = FindSettingByKey(base, key_address)) {
    text = setting->label;
    return true;
  }
  if (GuestStringEquals(base, key_address, kSaveKey, kOptionLabelCapacity)) {
    text = TextId::kSaveLabel;
    return true;
  }
  if (GuestStringEquals(base, key_address, kBackKey, kOptionLabelCapacity)) {
    text = TextId::kBackLabel;
    return true;
  }
  if (GuestStringEquals(base, key_address, kPreviousPageKey, kOptionLabelCapacity)) {
    text = TextId::kPreviousPageLabel;
    return true;
  }
  if (GuestStringEquals(base, key_address, kNextPageKey, kOptionLabelCapacity)) {
    text = TextId::kNextPageLabel;
    return true;
  }
  return false;
}

std::string CurrentSettingValue(const Setting& setting) {
  if (setting.binding == SettingBinding::kAntiAliasing) {
    return std::string(
        rex::graphics::gta4_native::GetConfiguredAntiAliasingModeName());
  }
  if (setting.binding == SettingBinding::kHdr) {
    return std::string(rex::graphics::gta4_native::GetConfiguredHdrModeName());
  }
  std::string value = rex::cvar::GetFlagByName(setting.cvar);
  if (std::string_view(setting.cvar) == "gta4_aspect_ratio" && value == "original")
    value = "16:9";
  return value;
}

const Choice& CurrentChoice(const Setting& setting) {
  const std::string current = CurrentSettingValue(setting);
  for (uint8_t index = 0; index < setting.choice_count; ++index) {
    if (current == setting.choices[index].value) {
      return setting.choices[index];
    }
  }
  return setting.choices[0];
}

uint8_t CurrentChoiceIndex(const Setting& setting) {
  const std::string current = CurrentSettingValue(setting);
  for (uint8_t index = 0; index < setting.choice_count; ++index) {
    if (current == setting.choices[index].value) {
      return index;
    }
  }
  return 0;
}

bool WriteInlineKey(uint8_t* base, uint32_t destination, std::string_view key) {
  if (!gta4::frontend_menu::policy::FitsInlineKey(key.size())) {
    REXLOG_ERROR("GTA IV native menu: key '{}' exceeds the {}-byte retail field", key,
                 kOptionLabelCapacity);
    return false;
  }
  for (uint32_t index = 0; index < kOptionLabelCapacity; ++index) {
    REX_STORE_U8(destination + index, 0);
  }
  for (uint32_t index = 0; index < key.size(); ++index) {
    REX_STORE_U8(destination + index, static_cast<uint8_t>(key[index]));
  }
  return true;
}

bool ValidateNativeMenuKeys() {
  for (const Setting& setting : kSettings) {
    if (!gta4::frontend_menu::policy::FitsInlineKey(setting.key.size())) {
      REXLOG_ERROR("GTA IV native menu disabled: key '{}' exceeds the {}-byte retail field",
                   setting.key, kOptionLabelCapacity);
      return false;
    }
  }
  for (const std::string_view key :
       {kAdvancedKey, kSaveKey, kBackKey, kPreviousPageKey, kNextPageKey}) {
    if (!gta4::frontend_menu::policy::FitsInlineKey(key.size())) {
      REXLOG_ERROR("GTA IV native menu disabled: key '{}' exceeds the {}-byte retail field", key,
                   kOptionLabelCapacity);
      return false;
    }
  }
  return true;
}

bool WriteSettingRow(uint8_t* base, uint32_t destination, const Setting& setting) {
  std::memset(base + destination, 0, kOptionRecordSize);
  REX_STORE_U8(destination + kOptionActionOffset, kMenuOptionAdjust);
  if (!WriteInlineKey(base, destination + kOptionLabelOffset, setting.key)) {
    return false;
  }
  REX_STORE_U16(destination + kOptionValueOffset, kSafeStockPreference);
  REX_STORE_U8(destination + kOptionScalerOffset, setting.choice_count);
  REX_STORE_U8(destination + kOptionDisplayValueOffset, kSafeStockDisplayValue);
  return true;
}

bool WriteJumpRow(uint8_t* base, uint32_t destination, std::string_view key) {
  std::memset(base + destination, 0, kOptionRecordSize);
  REX_STORE_U8(destination + kOptionActionOffset, kMenuOptionJump);
  if (!WriteInlineKey(base, destination + kOptionLabelOffset, key)) {
    return false;
  }
  REX_STORE_U16(destination + kOptionValueOffset, kDisplayScreen);
  REX_STORE_U8(destination + kOptionScalerOffset, 0);
  REX_STORE_U8(destination + kOptionDisplayValueOffset, kSafeStockDisplayValue);
  return true;
}

void WriteSentinelRow(uint8_t* base, uint32_t destination) {
  std::memset(base + destination, 0, kOptionRecordSize);
  REX_STORE_U8(destination + kOptionActionOffset, kEndOfMenuOptions);
}

size_t StringPoolBytes() {
  size_t bytes = 0;
  for (std::string_view value : kStringPool) {
    bytes += value.size() + 1;
  }
  return bytes;
}

void WriteStringPool(uint8_t* base, DisplayMenuState& state, uint32_t destination) {
  uint32_t offset = 0;
  for (size_t index = 0; index < kStringPool.size(); ++index) {
    const std::string_view value = kStringPool[index];
    state.text_addresses[index] = destination + offset;
    std::memcpy(base + destination + offset, value.data(), value.size());
    REX_STORE_U8(destination + offset + value.size(), 0);
    offset += static_cast<uint32_t>(value.size()) + 1;
  }
}

void PublishDisplayDescriptor(uint8_t* base, uint32_t rows, uint16_t count, uint16_t capacity) {
  REX_STORE_U32(kDisplayOptionsPointer, rows);
  REX_STORE_U16(kDisplayOptionsCount, count);
  REX_STORE_U16(kDisplayOptionsCapacity, capacity);
}

void TraceDescriptorPublish(std::string_view cause, std::string_view page, uint32_t rows,
                            uint16_t count, int64_t page_index) {
  if (!FrontendDiagnosticsEnabled()) {
    return;
  }
  REXLOG_INFO(
      "GTA4MenuTrace seq={} point=descriptor-publish cause={} page={} page-index={} "
      "channel={} rows={:08X} count={} capacity={}",
      ++g_menu_trace_sequence, cause, page, page_index, g_display_menu.last_frontend_channel, rows,
      count, count);
}

void PublishPrimaryPage(uint8_t* base, std::string_view cause = "primary-request") {
  if (g_display_menu.primary_rows == 0) {
    return;
  }
  PublishDisplayDescriptor(base, g_display_menu.primary_rows, g_display_menu.primary_count,
                           g_display_menu.primary_count);
  g_display_menu.page = gta4::frontend_menu::policy::Page::kPrimary;
  ResetOwnedScrollTrace();
  TraceDescriptorPublish(cause, "primary", g_display_menu.primary_rows,
                         g_display_menu.primary_count, -1);
}

bool PublishNativePage(uint8_t* base, std::size_t page_index) {
  if (page_index >= g_display_menu.native_pages.size()) {
    REXLOG_ERROR("GTA IV native menu: rejected invalid page index {}", page_index);
    return false;
  }
  const NativePageState& page = g_display_menu.native_pages[page_index];
  PublishDisplayDescriptor(base, page.rows, page.count, page.count);
  g_display_menu.current_native_page = page_index;
  g_display_menu.page = gta4::frontend_menu::policy::Page::kAdvanced;
  ResetOwnedScrollTrace();
  TraceDescriptorPublish("native-request", "native", page.rows, page.count,
                         static_cast<int64_t>(page_index));
  return true;
}

void ReleaseDisplayExtension(PPCContext& ctx, uint8_t* base) {
  if (g_display_menu.page == gta4::frontend_menu::policy::Page::kDisabled) {
    return;
  }
  const bool owns_published_descriptor = OwnsPublishedDisplayDescriptor(base);
  if (owns_published_descriptor) {
    PublishDisplayDescriptor(base, g_display_menu.stock_rows, g_display_menu.stock_count,
                             g_display_menu.stock_capacity);
    TraceDescriptorPublish("release", "stock", g_display_menu.stock_rows,
                           g_display_menu.stock_count, -1);
  } else {
    REXLOG_WARN(
        "GTA IV native menu: descriptor ownership changed before release; retail descriptor was "
        "not republished");
  }
  const uint32_t allocation = g_display_menu.allocation;
  g_display_menu = {};
  if (gta4::frontend_menu::policy::ShouldFreeGuestAllocation(owns_published_descriptor,
                                                              allocation)) {
    InvokeGuest(ctx, base, sub_821B3560, allocation);
  } else if (allocation != 0) {
    // Host-side menu state survives an episode/title reload, but its guest
    // allocation does not. A changed descriptor means the guest no longer
    // proves ownership of this address, so calling the guest allocator with it
    // can free recycled memory.
    REXLOG_WARN(
        "GTA IV native menu: discarded stale allocation metadata {:08X}; guest free skipped",
        allocation);
  }
  REXLOG_INFO("GTA IV native menu: release complete restored={}", owns_published_descriptor);
}

void InstallDisplayExtension(PPCContext& ctx, uint8_t* base) {
  if (!REXCVAR_GET(gta4_frontend_advanced_graphics_menu)) {
    return;
  }
  if (!ValidateNativeMenuKeys()) {
    return;
  }

  const uint32_t stock_rows = REX_LOAD_U32(kDisplayOptionsPointer);
  const uint16_t stock_count = REX_LOAD_U16(kDisplayOptionsCount);
  const uint16_t stock_capacity = REX_LOAD_U16(kDisplayOptionsCapacity);
  const std::size_t stock_span_bytes = static_cast<std::size_t>(stock_count) * kOptionRecordSize;
  if (stock_rows == 0 || stock_count == 0 ||
      stock_count > gta4::frontend_menu::policy::kRetailListSlotCapacity ||
      stock_count > stock_capacity || !IsGuestSpanValid(stock_rows, stock_span_bytes)) {
    REXLOG_ERROR(
        "GTA IV Advanced Graphics disabled: invalid Display vector rows={:08X} count={} "
        "capacity={} span-bytes={}",
        stock_rows, stock_count, stock_capacity, stock_span_bytes);
    return;
  }
  uint16_t sentinel_index = stock_count;
  for (uint16_t index = 0; index < stock_count; ++index) {
    const uint32_t row = stock_rows + static_cast<uint32_t>(index) * kOptionRecordSize;
    if (REX_LOAD_U8(row + kOptionActionOffset) == kEndOfMenuOptions) {
      sentinel_index = index;
      break;
    }
  }

  const gta4::frontend_menu::policy::StockLayout stock_layout = {
      .rows = stock_rows,
      .count = stock_count,
      .capacity = stock_capacity,
      .sentinel_index = sentinel_index,
  };
  if (!gta4::frontend_menu::policy::CanInstallPrimary(stock_layout)) {
    REXLOG_ERROR(
        "GTA IV Advanced Graphics disabled: incompatible Display vector rows={:08X} "
        "count={} capacity={} sentinel={}",
        stock_rows, stock_count, stock_capacity, sentinel_index);
    return;
  }

  std::vector<gta4::frontend_menu::policy::NativePageSlice> page_slices;
  std::size_t first_setting = 0;
  std::size_t total_row_count = 0;
  while (first_setting < kSettings.size()) {
    const auto slice =
        gta4::frontend_menu::policy::PlanNativePage(kSettings.size(), first_setting);
    if (slice.item_count == 0 ||
        !gta4::frontend_menu::policy::CanBuildAdvanced(slice.RowCount()) ||
        slice.RowCount() > std::numeric_limits<uint16_t>::max()) {
      REXLOG_ERROR("GTA IV native menu disabled: invalid page layout at item {}", first_setting);
      return;
    }
    page_slices.push_back(slice);
    total_row_count += slice.RowCount();
    first_setting += slice.item_count;
  }
  if (page_slices.empty()) {
    REXLOG_ERROR("GTA IV native menu disabled: no settings were registered");
    return;
  }

  const std::size_t primary_row_count = static_cast<std::size_t>(stock_count) + 1;
  const size_t primary_rows_bytes = primary_row_count * kOptionRecordSize;
  const size_t native_rows_bytes = total_row_count * kOptionRecordSize;
  const size_t rows_bytes = primary_rows_bytes + native_rows_bytes;
  const size_t pool_bytes = StringPoolBytes();
  const size_t allocation_bytes = rows_bytes + pool_bytes;
  if (allocation_bytes > std::numeric_limits<uint32_t>::max()) {
    REXLOG_ERROR("GTA IV Advanced Graphics disabled: allocation overflow {}", allocation_bytes);
    return;
  }

  const uint32_t allocation =
      InvokeGuest(ctx, base, sub_821B3510, static_cast<uint32_t>(allocation_bytes)).r3.u32;
  if (allocation == 0) {
    REXLOG_ERROR("GTA IV Advanced Graphics disabled: allocation of {} bytes failed",
                 allocation_bytes);
    return;
  }
  if (!IsGuestSpanValid(allocation, allocation_bytes)) {
    REXLOG_ERROR("GTA IV Advanced Graphics disabled: allocator returned invalid span {:08X}+{}",
                 allocation, allocation_bytes);
    InvokeGuest(ctx, base, sub_821B3560, allocation);
    return;
  }

  DisplayMenuState next;
  next.page = gta4::frontend_menu::policy::Page::kPrimary;
  next.stock_rows = stock_rows;
  next.primary_rows = allocation;
  next.string_pool = allocation + static_cast<uint32_t>(rows_bytes);
  next.allocation = allocation;
  next.stock_count = stock_count;
  next.stock_capacity = stock_capacity;
  next.primary_count = static_cast<uint16_t>(primary_row_count);
  next.sentinel_index = sentinel_index;
  next.advanced_entry_index = sentinel_index;

  std::memcpy(base + next.primary_rows, base + stock_rows, stock_span_bytes);
  const uint32_t primary_extension =
      next.primary_rows + static_cast<uint32_t>(sentinel_index) * kOptionRecordSize;
  if (!WriteJumpRow(base, primary_extension, kAdvancedKey)) {
    InvokeGuest(ctx, base, sub_821B3560, allocation);
    return;
  }
  WriteSentinelRow(base, primary_extension + kOptionRecordSize);

  uint32_t destination = allocation + static_cast<uint32_t>(primary_rows_bytes);
  for (const auto& slice : page_slices) {
    NativePageState page = {
        .rows = destination,
        .count = static_cast<uint16_t>(slice.RowCount()),
        .first_setting = slice.first_item,
        .setting_count = slice.item_count,
        .has_previous = slice.has_previous,
        .has_next = slice.has_next,
    };
    for (std::size_t index = 0; index < slice.item_count; ++index) {
      const Setting& setting = kSettings[slice.first_item + index];
      if (!WriteSettingRow(base, destination, setting)) {
        InvokeGuest(ctx, base, sub_821B3560, allocation);
        return;
      }
      destination += kOptionRecordSize;
    }
    if (slice.has_previous) {
      if (!WriteJumpRow(base, destination, kPreviousPageKey)) {
        InvokeGuest(ctx, base, sub_821B3560, allocation);
        return;
      }
      destination += kOptionRecordSize;
    }
    if (slice.has_next) {
      if (!WriteJumpRow(base, destination, kNextPageKey)) {
        InvokeGuest(ctx, base, sub_821B3560, allocation);
        return;
      }
      destination += kOptionRecordSize;
    }
    if (!WriteJumpRow(base, destination, kSaveKey)) {
      InvokeGuest(ctx, base, sub_821B3560, allocation);
      return;
    }
    destination += kOptionRecordSize;
    if (!WriteJumpRow(base, destination, kBackKey)) {
      InvokeGuest(ctx, base, sub_821B3560, allocation);
      return;
    }
    destination += kOptionRecordSize;
    WriteSentinelRow(base, destination);
    destination += kOptionRecordSize;
    next.native_pages.push_back(page);
  }
  if (destination != next.string_pool) {
    REXLOG_ERROR(
        "GTA IV native menu disabled: row layout ended at {:08X}, expected string pool {:08X}",
        destination, next.string_pool);
    InvokeGuest(ctx, base, sub_821B3560, allocation);
    return;
  }
  WriteStringPool(base, next, next.string_pool);

  g_display_menu = next;
  PublishPrimaryPage(base, "install");

  REXLOG_INFO(
      "GTA IV native menu installed: primary-rows={} native-pages={} native-settings={} "
      "stock-vector={:08X} allocation={:08X} allocation-bytes={}",
      g_display_menu.primary_count, g_display_menu.native_pages.size(), kSettings.size(),
      g_display_menu.stock_rows, g_display_menu.allocation, allocation_bytes);
}

void SwitchDisplayPage(PPCContext& ctx, uint8_t* base, gta4::frontend_menu::policy::Page page,
                       uint16_t selected_row, std::size_t native_page = 0) {
  if (page == gta4::frontend_menu::policy::Page::kAdvanced) {
    if (!PublishNativePage(base, native_page)) {
      return;
    }
  } else {
    PublishPrimaryPage(base);
  }
  InvokeGuest(ctx, base, __imp__sub_82256D08, g_display_menu.last_frontend_channel, kDisplayScreen,
              selected_row);
}

void ChangeSetting(const Setting& setting, int32_t delta) {
  if (delta == 0) {
    return;
  }
  const std::string old_value = CurrentSettingValue(setting);
  uint8_t index = CurrentChoiceIndex(setting);
  if (delta > 0) {
    index = index + 1 == setting.choice_count ? 0 : static_cast<uint8_t>(index + 1);
  } else {
    index = index == 0 ? static_cast<uint8_t>(setting.choice_count - 1)
                       : static_cast<uint8_t>(index - 1);
  }

  const Choice& choice = setting.choices[index];
  bool restart_required = setting.restart_required;
  if (setting.binding == SettingBinding::kAntiAliasing) {
    using rex::graphics::gta4_native::AntiAliasingApplyResult;
    const AntiAliasingApplyResult result =
        rex::graphics::gta4_native::SetConfiguredAntiAliasingMode(choice.value);
    if (result == AntiAliasingApplyResult::kRejected) {
      REXLOG_ERROR("GTA IV Advanced Graphics: rejected {}={}", setting.cvar, choice.value);
      return;
    }
    restart_required = result == AntiAliasingApplyResult::kRestartRequired;
  } else if (setting.binding == SettingBinding::kHdr) {
    if (!rex::graphics::gta4_native::SetConfiguredHdrMode(choice.value)) {
      REXLOG_ERROR("GTA IV Advanced Graphics: rejected {}={}", setting.cvar, choice.value);
      return;
    }
  } else if (setting.binding == SettingBinding::kUpscaler && choice.value != "native" &&
             rex::graphics::gta4_native::UsesSceneSupersampling(
                 rex::graphics::gta4_native::GetConfiguredAntiAliasingMode())) {
    REXLOG_ERROR(
        "GTA IV Advanced Graphics: rejected {}={} because SSAA is selected; "
        "select a non-SSAA anti-aliasing mode first",
        setting.cvar, choice.value);
    return;
  } else if (!rex::cvar::SetFlagByName(setting.cvar, choice.value)) {
    REXLOG_ERROR("GTA IV Advanced Graphics: rejected {}={}", setting.cvar, choice.value);
    return;
  }
  const std::string applied_value = CurrentSettingValue(setting);
  if (applied_value != choice.value) {
    REXLOG_ERROR(
        "GTA4MenuTrace seq={} point=setting-change-rejected key='{}' cvar={} old='{}' "
        "requested='{}' observed='{}'",
        ++g_menu_trace_sequence, setting.key, setting.cvar, old_value, choice.value,
        applied_value);
    return;
  }
  g_display_menu.rebuild_pending = true;
  REXLOG_INFO(
      "GTA4MenuTrace seq={} point=setting-change key='{}' cvar={} old='{}' new='{}' "
      "direction={} apply={}",
      ++g_menu_trace_sequence, setting.key, setting.cvar, old_value, applied_value, delta,
      restart_required ? "restart" : "live");
}

void SaveGraphicsSettings() {
  if (g_config_path.empty()) {
    REXLOG_ERROR("GTA IV Display extension: native config path is unavailable");
    return;
  }
  std::error_code error;
  std::filesystem::create_directories(g_config_path.parent_path(), error);
  if (error) {
    REXLOG_ERROR("GTA IV Display extension: failed to create config directory: {}",
                 error.message());
    return;
  }
  if (!rex::cvar::SaveConfig(g_config_path)) {
    REXLOG_ERROR("GTA IV Display extension: graphics settings were not saved to {}",
                 g_config_path.string());
    return;
  }
  REXLOG_INFO("GTA IV Display extension: saved graphics settings to {}", g_config_path.string());
}

class ScopedAdjustment final {
 public:
  ScopedAdjustment(uint32_t frontend_channel, bool is_display) : previous_(g_adjustment) {
    g_adjustment = {.active = is_display, .frontend_channel = frontend_channel, .delta = 0};
  }

  ~ScopedAdjustment() { g_adjustment = previous_; }

 private:
  AdjustmentContext previous_;
};

}  // namespace

namespace gta4::frontend_menu {

void SetConfigPath(std::filesystem::path path) {
  g_config_path = std::move(path);
}

bool SwitchPauseTab(PPCContext& parent, uint8_t* base, policy::PauseTabDirection direction,
                    uint32_t* previous_screen, uint32_t* target_screen) {
  const uint32_t current = REX_LOAD_U32(kCurrentScreenAddress);
  uint32_t target = 0;
  if (previous_screen) {
    *previous_screen = current;
  }
  if (!policy::ResolvePauseTab(current, direction, target)) {
    return false;
  }
  if (target_screen) {
    *target_screen = target;
  }
  if (policy::ShouldRestorePrimaryBeforeSwitch(g_display_menu.page, target, kDisplayScreen)) {
    PublishPrimaryPage(base);
  }
  InvokeGuest(parent, base, __imp__sub_82256D08, g_last_frontend_channel, target, 0);
  if (rex::input::IsInputTraceEnabled()) {
    REXLOG_INFO(
        "input-e2e: seq={} stage=pause-tab result=switched direction={} channel={} "
        "screen={}->{}",
        rex::input::NextInputTraceSequence(), static_cast<int32_t>(direction),
        g_last_frontend_channel, current, target);
  }
  return true;
}

}  // namespace gta4::frontend_menu

extern "C" void sub_82241428(PPCContext& ctx, uint8_t* base) {
  __imp__sub_82241428(ctx, base);
  const int32_t retail_episode = ctx.r3.s32;
  const std::string policy = REXCVAR_GET(gta4_episode_startup_prompt);

  if (policy == "off") {
    ctx.r3.s64 = -1;
    REXLOG_INFO("GTA IV episode startup: policy=off retail-result={}", retail_episode);
    return;
  }
  if (policy == "always") {
    REXLOG_WARN(
        "GTA IV episode startup: legacy policy=always now preserves retail behavior; "
        "use Game -> New to choose any installed episode");
  }
  REXLOG_INFO("GTA IV episode startup: policy={} retail-result={} preserved", policy,
              retail_episode);
}

extern "C" void sub_82157F90(PPCContext& ctx, uint8_t* base) {
  ReleaseDisplayExtension(ctx, base);
  __imp__sub_82157F90(ctx, base);
  TraceScreenRows(base, "stock-loaded", kAudioScreen);
  TraceScreenRows(base, "stock-loaded", kDisplayScreen);
  InstallDisplayExtension(ctx, base);
  TraceScreenRows(base, "extension-published", kAudioScreen);
  TraceScreenRows(base, "extension-published", kDisplayScreen);
}

extern "C" void sub_8221FD88(PPCContext& ctx, uint8_t* base) {
  if (gta4::quicksave::ResolveText(ctx, base))
    return;
  const bool diagnostics = FrontendDiagnosticsEnabled();
  const uint32_t destination = ctx.r3.u32;
  const uint32_t key_address = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  const uint32_t screen = diagnostics ? REX_LOAD_U32(kCurrentScreenAddress) : 0;
  TextId custom_text = TextId::kCount;
  if (IsDisplayScreen(base) && FindOwnedLabelText(base, key_address, custom_text)) {
    const uint32_t text_address = TextAddress(custom_text);
    if (text_address != 0) {
      ctx.r3.u64 = text_address;
      if (diagnostics) {
        REXLOG_INFO(
            "GTA4MenuTrace seq={} point=label-resolve-custom screen={}({}) caller={:08X} "
            "destination={:08X} key-address={:08X} key='{}' result={:08X} text='{}'",
            ++g_menu_trace_sequence, screen, TraceScreenName(screen), caller, destination,
            key_address, ReadGuestString(base, key_address, kMenuTraceStringCapacity), ctx.r3.u32,
            ReadGuestString(base, ctx.r3.u32, kMenuTraceStringCapacity));
      }
      return;
    }
  }
  __imp__sub_8221FD88(ctx, base);
  if (diagnostics && IsBodyTraceScreen(screen) && g_menu_trace_label_calls < kMenuTraceCallLimit) {
    ++g_menu_trace_label_calls;
    REXLOG_INFO(
        "GTA4MenuTrace seq={} point=label-resolve-stock screen={}({}) caller={:08X} "
        "destination={:08X} key-address={:08X} key='{}' result={:08X} text='{}'",
        ++g_menu_trace_sequence, screen, TraceScreenName(screen), caller, destination, key_address,
        ReadGuestString(base, key_address, kMenuTraceStringCapacity), ctx.r3.u32,
        ReadGuestString(base, ctx.r3.u32, kMenuTraceStringCapacity));
  }
}

extern "C" void sub_82252A98(PPCContext& ctx, uint8_t* base) {
  const bool diagnostics = FrontendDiagnosticsEnabled();
  const uint32_t frontend_channel = ctx.r3.u32;
  const uint32_t screen = ctx.r4.u32;
  const uint32_t selected_value = ctx.r5.u32;
  const int32_t row_index = ctx.r6.s32;
  if (screen == kDisplayScreen && IsOwnedAdvancedPage(base)) {
    if (const Setting* setting = FindSettingByRow(base, ctx.r6.s32)) {
      ctx.r3.u64 = TextAddress(CurrentChoice(*setting).text);
      if (diagnostics) {
        REXLOG_INFO(
            "GTA4MenuTrace seq={} point=value-resolve-custom channel={} screen={} row={} "
            "selected-value={} key='{}' result={:08X} text='{}'",
            ++g_menu_trace_sequence, frontend_channel, screen, row_index, selected_value,
            setting->key, ctx.r3.u32, ReadGuestString(base, ctx.r3.u32, kMenuTraceStringCapacity));
      }
      return;
    }
  }
  __imp__sub_82252A98(ctx, base);
  if (diagnostics && IsBodyTraceScreen(screen) && g_menu_trace_value_calls < kMenuTraceCallLimit) {
    ++g_menu_trace_value_calls;
    REXLOG_INFO(
        "GTA4MenuTrace seq={} point=value-resolve-stock channel={} screen={} row={} "
        "selected-value={} result={:08X} text='{}'",
        ++g_menu_trace_sequence, frontend_channel, screen, row_index, selected_value, ctx.r3.u32,
        ReadGuestString(base, ctx.r3.u32, kMenuTraceStringCapacity));
  }
}

extern "C" void sub_82255D00(PPCContext& ctx, uint8_t* base) {
  const uint32_t frontend_channel = ctx.r3.u32;
  g_last_frontend_channel = frontend_channel;
  const uint32_t screen = REX_LOAD_U32(kCurrentScreenAddress);
  if (g_display_menu.page == gta4::frontend_menu::policy::Page::kAdvanced &&
      screen != kDisplayScreen) {
    PublishPrimaryPage(base);
  }
  if (screen == kDisplayScreen && OwnsPublishedDisplayDescriptor(base)) {
    g_display_menu.last_frontend_channel = frontend_channel;
  }
  const bool diagnostics = FrontendDiagnosticsEnabled();
  const uint32_t vector = ScreenOptionsAddress(screen);
  const uint32_t rows = REX_LOAD_U32(vector);
  const uint16_t count = REX_LOAD_U16(vector + 4);
  const uint16_t capacity = REX_LOAD_U16(vector + 6);
  g_menu_trace_label_calls = 0;
  g_menu_trace_value_calls = 0;
  if (diagnostics) {
    REXLOG_INFO(
        "GTA4MenuTrace seq={} point=page-build-enter channel={} screen={}({}) vector={:08X} "
        "rows={:08X} count={} capacity={} retail-list-slots={} over-capacity={}",
        ++g_menu_trace_sequence, frontend_channel, screen, TraceScreenName(screen), vector, rows,
        count, capacity, kRetailListSlotCapacity, count > kRetailListSlotCapacity);
  }
  const uint32_t previous_page_screen = g_menu_trace_page_screen;
  g_menu_trace_page_screen = screen;
  __imp__sub_82255D00(ctx, base);
  g_menu_trace_page_screen = previous_page_screen;
  if (screen == kDisplayScreen && IsOwnedAdvancedPage(base)) {
    const FrontendWidgetState widget = ReadFrontendWidgetState(base, frontend_channel);
    TraceOwnedScrollState("owned-scroll-build", frontend_channel, {}, widget);
  }
  if (diagnostics) {
    REXLOG_INFO(
        "GTA4MenuTrace seq={} point=page-build-exit channel={} screen={}({}) "
        "return={:08X} label-calls={} value-calls={}",
        ++g_menu_trace_sequence, frontend_channel, screen, TraceScreenName(screen), ctx.r3.u32,
        g_menu_trace_label_calls, g_menu_trace_value_calls);
  }
}

extern "C" void sub_8229D360(PPCContext& ctx, uint8_t* base) {
  const uint32_t channel = ctx.r3.u32;
  const bool owns_advanced = IsDisplayScreen(base) && IsOwnedAdvancedPage(base);
  const FrontendWidgetState before =
      owns_advanced ? ReadFrontendWidgetState(base, channel) : FrontendWidgetState{};
  if (owns_advanced && FrontendDiagnosticsEnabled() && !g_owned_navigation_route_logged) {
    g_owned_navigation_route_logged = true;
    REXLOG_INFO(
        "GTA4MenuTrace seq={} point=owned-navigation-hook-route guest=8229D360 "
        "channel={} caller={:08X} ui={:08X}",
        ++g_menu_trace_sequence, channel, ctx.lr, before.widget);
  }
  __imp__sub_8229D360(ctx, base);
  if (owns_advanced && IsDisplayScreen(base) && IsOwnedAdvancedPage(base)) {
    const FrontendWidgetState after = ReadFrontendWidgetState(base, channel);
    TraceOwnedScrollState("owned-scroll-input", channel, before, after);
  }
}

extern "C" void sub_8229D258(PPCContext& ctx, uint8_t* base) {
  const uint32_t channel = ctx.r3.u32;
  const uint32_t first_row_address = ctx.r4.u32;
  const uint32_t end_row_address = ctx.r5.u32;
  const uint32_t caller = ctx.lr;
  const bool owns_advanced = IsDisplayScreen(base) && IsOwnedAdvancedPage(base);
  __imp__sub_8229D258(ctx, base);
  if (!owns_advanced || !IsDisplayScreen(base) || !IsOwnedAdvancedPage(base) ||
      !IsGuestSpanValid(first_row_address, sizeof(uint32_t)) ||
      !IsGuestSpanValid(end_row_address, sizeof(uint32_t))) {
    return;
  }
  const FrontendWidgetState widget = ReadFrontendWidgetState(base, channel);
  int32_t first_row = static_cast<int32_t>(REX_LOAD_U32(first_row_address));
  int32_t end_row = static_cast<int32_t>(REX_LOAD_U32(end_row_address));
  const std::size_t visible_capacity =
      g_display_menu.primary_count > 1
          ? static_cast<std::size_t>(g_display_menu.primary_count - 1)
          : 0;
  if (widget.valid && widget.active_count > 0 && first_row == 0 &&
      end_row == widget.active_count && visible_capacity != 0 &&
      static_cast<std::size_t>(widget.active_count) > visible_capacity) {
    const std::size_t selected =
        widget.selected >= 0 ? static_cast<std::size_t>(widget.selected) : 0;
    const auto viewport = gta4::frontend_menu::policy::FollowSelectionViewport(
        static_cast<std::size_t>(widget.active_count), visible_capacity, selected,
        g_owned_scroll_window_first);
    g_owned_scroll_window_first = viewport.first;
    first_row = static_cast<int32_t>(viewport.first);
    end_row = static_cast<int32_t>(viewport.end);
    REX_STORE_U32(first_row_address, static_cast<uint32_t>(viewport.first));
    REX_STORE_U32(end_row_address, static_cast<uint32_t>(viewport.end));
  }
  TraceOwnedLayoutBounds(channel, caller, first_row, end_row, widget);
}

extern "C" void sub_8229D8A8(PPCContext& ctx, uint8_t* base) {
  const uint32_t channel = ctx.r3.u32;
  const uint32_t list_index = ctx.r4.u32;
  const bool owns_advanced = IsDisplayScreen(base) && IsOwnedAdvancedPage(base);
  const FrontendWidgetState before =
      owns_advanced ? ReadFrontendWidgetState(base, channel) : FrontendWidgetState{};
  if (owns_advanced && FrontendDiagnosticsEnabled() && !g_owned_layout_route_logged) {
    g_owned_layout_route_logged = true;
    REXLOG_INFO(
        "GTA4MenuTrace seq={} point=owned-layout-hook-route guest=8229D8A8 channel={} "
        "caller={:08X} list={} ui={:08X}",
        ++g_menu_trace_sequence, channel, ctx.lr, list_index, before.widget);
  }
  GTA4_TouchCaptureFrontendDraw(ctx, base, __imp__sub_8229D8A8);
  if (owns_advanced && IsDisplayScreen(base) && IsOwnedAdvancedPage(base)) {
    const FrontendWidgetState after = ReadFrontendWidgetState(base, channel);
    TraceOwnedScrollState("owned-layout-draw", channel, before, after);
  }
}

extern "C" void sub_8229CD40(PPCContext& ctx, uint8_t* base) {
  const bool trace_page = FrontendDiagnosticsEnabled() &&
                          g_menu_trace_page_screen != std::numeric_limits<uint32_t>::max();
  const uint32_t caller = ctx.lr;
  const uint32_t frontend_ui = ctx.r3.u32;
  const int32_t list_index = ctx.r4.s32;
  const uint32_t title = ctx.r5.u32;
  const uint32_t row_payload = ctx.r6.u32;
  const int32_t count = ctx.r7.s32;
  const bool is_owned_display_list =
      g_menu_trace_page_screen == kDisplayScreen && OwnsPublishedDisplayDescriptor(base) &&
      count > 0 && count <= kRetailListSlotCapacity;
  const bool owns_display_label_payload =
      is_owned_display_list && list_index == 0 && caller == kLeftListBuildReturnAddress;
  const std::size_t payload_bytes =
      owns_display_label_payload ? static_cast<std::size_t>(count) * kUiRowRecordSize : 0;
  if (owns_display_label_payload && IsGuestSpanValid(row_payload, payload_bytes)) {
    for (int32_t index = 0; index < count; ++index) {
      const uint32_t option = OwnedDisplayRowAddress(base, index);
      TextId text = TextId::kCount;
      if (option == 0 ||
          !FindOwnedLabelText(base, option + kOptionLabelOffset, text)) {
        continue;
      }
      const uint32_t payload_row =
          row_payload + static_cast<uint32_t>(index) * kUiRowRecordSize;
      const std::string_view label = TextString(text);
      const std::string raw_label =
          trace_page ? ReadGuestString(base, payload_row, kUiRowLabelCapacity) : std::string{};
      if (WriteUiRowLabel(base, payload_row, label) && trace_page) {
        REXLOG_INFO(
            "GTA4MenuTrace seq={} point=ui-label-materialized screen={} caller={:08X} list={} "
            "row={} option={:08X} payload={:08X} raw='{}' mapped='{}' post='{}'",
            ++g_menu_trace_sequence, g_menu_trace_page_screen, caller, list_index, index, option,
            payload_row, raw_label, label,
            ReadGuestString(base, payload_row, kUiRowLabelCapacity));
      }
    }
  } else if (owns_display_label_payload) {
    REXLOG_ERROR(
        "GTA IV native menu: rejected invalid UI row payload {:08X}+{} count={}", row_payload,
        payload_bytes, count);
  }
  if (trace_page) {
    REXLOG_INFO(
        "GTA4MenuTrace seq={} point=retail-list-build-enter screen={}({}) caller={:08X} "
        "frontend-ui={:08X} list={} title={:08X} payload={:08X} count={} fixed-slots={} "
        "over-capacity={}",
        ++g_menu_trace_sequence, g_menu_trace_page_screen,
        TraceScreenName(g_menu_trace_page_screen), caller, frontend_ui, list_index, title,
        row_payload, count, kRetailListSlotCapacity, count > kRetailListSlotCapacity);
  }
  __imp__sub_8229CD40(ctx, base);
  if (is_owned_display_list && IsOwnedAdvancedPage(base)) {
    const FrontendWidgetState widget = ReadFrontendWidgetState(base, frontend_ui);
    TraceOwnedScrollState("owned-row-materialization", frontend_ui, {}, widget);
  }
  if (trace_page) {
    REXLOG_INFO(
        "GTA4MenuTrace seq={} point=retail-list-build-exit screen={}({}) caller={:08X} "
        "frontend-ui={:08X} list={} count={} return={:08X}",
        ++g_menu_trace_sequence, g_menu_trace_page_screen,
        TraceScreenName(g_menu_trace_page_screen), caller, frontend_ui, list_index, count,
        ctx.r3.u32);
  }
}

extern "C" void sub_82258FB0(PPCContext& ctx, uint8_t* base) {
  const uint32_t frontend_channel = ctx.r3.u32;
  {
    ScopedAdjustment adjustment(frontend_channel,
                                IsDisplayScreen(base) && IsOwnedAdvancedPage(base));
    __imp__sub_82258FB0(ctx, base);
  }
  if (g_display_menu.rebuild_pending) {
    g_display_menu.rebuild_pending = false;
    if (IsDisplayScreen(base) && IsOwnedAdvancedPage(base)) {
      InvokeGuest(ctx, base, sub_82255D00, frontend_channel);
      if (FrontendDiagnosticsEnabled()) {
        REXLOG_INFO(
            "GTA4MenuTrace seq={} point=setting-rebuild channel={} page={} page-index={}",
            ++g_menu_trace_sequence, frontend_channel, "native",
            g_display_menu.current_native_page);
      }
    }
  }
}

extern "C" void sub_8229C4F8(PPCContext& ctx, uint8_t* base) {
  __imp__sub_8229C4F8(ctx, base);
  if (g_adjustment.active && ctx.lr == kAdjustmentDeltaReturnAddress) {
    g_adjustment.delta = ctx.r3.s32;
    if (FrontendDiagnosticsEnabled()) {
      REXLOG_INFO("GTA4MenuTrace seq={} point=adjust-delta channel={} caller={:08X} delta={}",
                  ++g_menu_trace_sequence, g_adjustment.frontend_channel, ctx.lr,
                  g_adjustment.delta);
    }
  }
}

extern "C" void sub_82253370(PPCContext& ctx, uint8_t* base) {
  if (g_adjustment.active) {
    const int32_t row_index = ctx.r3.s32;
    if (const Setting* setting = FindSettingByRow(base, row_index)) {
      if (FrontendDiagnosticsEnabled()) {
        REXLOG_INFO(
            "GTA4MenuTrace seq={} point=adjust-dispatch channel={} row={} key='{}' cvar={} "
            "delta={}",
            ++g_menu_trace_sequence, g_adjustment.frontend_channel, row_index, setting->key,
            setting->cvar, g_adjustment.delta);
      }
      ChangeSetting(*setting, g_adjustment.delta);
      ctx.r3.u64 = 0;
      return;
    }
  }
  __imp__sub_82253370(ctx, base);
}

extern "C" void sub_82258388(PPCContext& ctx, uint8_t* base) {
  const uint32_t frontend_channel = ctx.r3.u32;
  const int32_t selected_row = ctx.r4.s32;
  if (IsDisplayScreen(base) && IsOwnedPrimaryPage(base) &&
      IsOwnedJumpRow(base, selected_row, kAdvancedKey)) {
    if (FrontendDiagnosticsEnabled()) {
      REXLOG_INFO(
          "GTA4MenuTrace seq={} point=activate action=open-native channel={} row={}",
          ++g_menu_trace_sequence, frontend_channel, selected_row);
    }
    g_display_menu.last_frontend_channel = frontend_channel;
    SwitchDisplayPage(ctx, base, gta4::frontend_menu::policy::Page::kAdvanced, 0);
    ctx.r3.u64 = 1;
    return;
  }
  if (IsDisplayScreen(base) && IsOwnedAdvancedPage(base) &&
      FindSettingByRow(base, selected_row) != nullptr) {
    if (FrontendDiagnosticsEnabled()) {
      REXLOG_INFO(
          "GTA4MenuTrace seq={} point=activate action=setting-consumed channel={} row={}",
          ++g_menu_trace_sequence, frontend_channel, selected_row);
    }
    ctx.r3.u64 = 0;
    return;
  }
  if (IsDisplayScreen(base) && IsOwnedAdvancedPage(base) &&
      IsOwnedJumpRow(base, selected_row, kPreviousPageKey)) {
    if (g_display_menu.current_native_page != 0) {
      SwitchDisplayPage(ctx, base, gta4::frontend_menu::policy::Page::kAdvanced, 0,
                        g_display_menu.current_native_page - 1);
    }
    ctx.r3.u64 = 1;
    return;
  }
  if (IsDisplayScreen(base) && IsOwnedAdvancedPage(base) &&
      IsOwnedJumpRow(base, selected_row, kNextPageKey)) {
    const std::size_t next_page = g_display_menu.current_native_page + 1;
    if (next_page < g_display_menu.native_pages.size()) {
      SwitchDisplayPage(ctx, base, gta4::frontend_menu::policy::Page::kAdvanced, 0, next_page);
    }
    ctx.r3.u64 = 1;
    return;
  }
  if (IsDisplayScreen(base) && IsOwnedAdvancedPage(base) &&
      IsOwnedJumpRow(base, selected_row, kSaveKey)) {
    if (FrontendDiagnosticsEnabled()) {
      REXLOG_INFO("GTA4MenuTrace seq={} point=activate action=save channel={} row={}",
                  ++g_menu_trace_sequence, frontend_channel, selected_row);
    }
    SaveGraphicsSettings();
    ctx.r3.u64 = 1;
    return;
  }
  if (IsDisplayScreen(base) && IsOwnedAdvancedPage(base) &&
      IsOwnedJumpRow(base, selected_row, kBackKey)) {
    if (FrontendDiagnosticsEnabled()) {
      REXLOG_INFO("GTA4MenuTrace seq={} point=activate action=back channel={} row={}",
                  ++g_menu_trace_sequence, frontend_channel, selected_row);
    }
    SwitchDisplayPage(ctx, base, gta4::frontend_menu::policy::Page::kPrimary,
                      g_display_menu.advanced_entry_index);
    ctx.r3.u64 = 1;
    return;
  }
  GTA4_RunWithPrimaryPlayerInfoAlias(ctx, base, __imp__sub_82258388);
}

extern "C" void sub_82256D08(PPCContext& ctx, uint8_t* base) {
  if (gta4::frontend_menu::policy::ShouldRestorePrimaryBeforeSwitch(g_display_menu.page, ctx.r4.u32,
                                                                    kDisplayScreen)) {
    PublishPrimaryPage(base);
  }
  __imp__sub_82256D08(ctx, base);
}

extern "C" void sub_82257450(PPCContext& ctx, uint8_t* base) {
  if (IsDisplayScreen(base) && IsOwnedAdvancedPage(base)) {
    if (FrontendDiagnosticsEnabled()) {
      REXLOG_INFO("GTA4MenuTrace seq={} point=cancel action=back page-index={}",
                  ++g_menu_trace_sequence, g_display_menu.current_native_page);
    }
    SwitchDisplayPage(ctx, base, gta4::frontend_menu::policy::Page::kPrimary,
                      g_display_menu.advanced_entry_index);
    return;
  }
  __imp__sub_82257450(ctx, base);
}

extern "C" void sub_8224F4D0(PPCContext& ctx, uint8_t* base) {
  __imp__sub_8224F4D0(ctx, base);
}

extern "C" void sub_8224FEA8(PPCContext& ctx, uint8_t* base) {
  if (g_display_menu.page == gta4::frontend_menu::policy::Page::kAdvanced) {
    PublishPrimaryPage(base);
  }
  __imp__sub_8224FEA8(ctx, base);
}

extern "C" void sub_82258EC0(PPCContext& ctx, uint8_t* base) {
  if (g_display_menu.page == gta4::frontend_menu::policy::Page::kAdvanced) {
    PublishPrimaryPage(base);
  }
  __imp__sub_82258EC0(ctx, base);
}
