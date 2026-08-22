#include "gta4_frontend_hooks.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <limits>
#include <string>
#include <string_view>
#include <system_error>

#include <rex/cvar.h>
#include <rex/diagnostics/policy.h>
#include <rex/logging.h>

#include "gta4_init.h"

REXCVAR_DECLARE(std::string, gta4_episode_startup_prompt);

namespace {

constexpr uint32_t kDisplayScreen = 8;
constexpr uint32_t kAudioScreen = 7;
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
constexpr uint32_t kStringPoolBytes = 506;
constexpr uint32_t kAdjustmentDeltaReturnAddress = 0x8225903C;
constexpr uint32_t kMenuTraceStringCapacity = 128;
constexpr uint32_t kMenuTraceRowLimit = 64;
constexpr uint32_t kMenuTraceCallLimit = 256;

struct Choice {
  std::string_view value;
  uint32_t text_offset;
};

struct Setting {
  std::string_view key;
  uint32_t label_offset;
  std::string_view cvar;
  const Choice* choices;
  uint8_t choice_count;
};

constexpr std::array kResolutionChoices = {
    Choice{"", 241}, Choice{"720p", 251}, Choice{"1080p", 262},
    Choice{"1440p", 274}, Choice{"4k", 286},
};
constexpr std::array kAspectChoices = {
    Choice{"auto", 298}, Choice{"original", 306},
};
constexpr std::array kToggleChoices = {
    Choice{"false", 320}, Choice{"true", 324},
};
constexpr std::array kPresentChoices = {
    Choice{"auto", 241}, Choice{"vsync", 327}, Choice{"mailbox", 340},
    Choice{"immediate", 348},
};
constexpr std::array kAntiAliasingChoices = {
    Choice{"off", 320}, Choice{"spatial", 358}, Choice{"fxaa", 366},
    Choice{"smaa", 371},
};
constexpr std::array kSmaaQualityChoices = {
    Choice{"low", 376}, Choice{"medium", 380}, Choice{"high", 387},
    Choice{"ultra", 392},
};
constexpr std::array kUpscalerChoices = {
    Choice{"native", 398}, Choice{"fsr1", 405},
};
constexpr std::array kFsrQualityChoices = {
    Choice{"ultra_quality", 411}, Choice{"quality", 425}, Choice{"balanced", 433},
    Choice{"performance", 442},
};
constexpr std::array kTextureFilteringChoices = {
    Choice{"original", 454}, Choice{"bilinear", 463}, Choice{"trilinear", 472},
};
constexpr std::array kAnisotropicChoices = {
    Choice{"original", 454}, Choice{"off", 320}, Choice{"2x", 482},
    Choice{"4x", 485}, Choice{"8x", 488}, Choice{"16x", 491},
};
constexpr std::array kReflectionChoices = {
    Choice{"original", 454}, Choice{"1080p", 495}, Choice{"full", 501},
};
constexpr std::array kReflectionAaChoices = {
    Choice{"original", 454}, Choice{"off", 320}, Choice{"2x", 482}, Choice{"4x", 485},
};
constexpr std::array kShadowChoices = {
    Choice{"256", 376}, Choice{"512", 387}, Choice{"1024", 392},
};

constexpr std::array kSettings = {
    Setting{"LR_RES", 0, "resolution", kResolutionChoices.data(), kResolutionChoices.size()},
    Setting{"LR_ASPECT", 18, "gta4_aspect_ratio", kAspectChoices.data(),
            kAspectChoices.size()},
    Setting{"LR_FULLSCR", 31, "fullscreen", kToggleChoices.data(), kToggleChoices.size()},
    Setting{"LR_PRESENT", 42, "gta4_present_mode", kPresentChoices.data(),
            kPresentChoices.size()},
    Setting{"LR_HDR", 55, "vulkan_hdr", kToggleChoices.data(), kToggleChoices.size()},
    Setting{"LR_AA", 59, "gta4_native_anti_aliasing", kAntiAliasingChoices.data(),
            kAntiAliasingChoices.size()},
    Setting{"LR_SMAA", 73, "gta4_native_smaa_quality", kSmaaQualityChoices.data(),
            kSmaaQualityChoices.size()},
    Setting{"LR_UPSCALE", 86, "gta4_native_upscaler", kUpscalerChoices.data(),
            kUpscalerChoices.size()},
    Setting{"LR_FSR", 96, "gta4_fsr1_quality", kFsrQualityChoices.data(),
            kFsrQualityChoices.size()},
    Setting{"LR_TEXFILTER", 110, "gta4_texture_filtering", kTextureFilteringChoices.data(),
            kTextureFilteringChoices.size()},
    Setting{"LR_ANISO", 128, "gta4_anisotropic_filtering", kAnisotropicChoices.data(),
            kAnisotropicChoices.size()},
    Setting{"LR_REFL_RES", 150, "gta4_reflection_resolution", kReflectionChoices.data(),
            kReflectionChoices.size()},
    Setting{"LR_REFL_AA", 172, "gta4_reflection_aa", kReflectionAaChoices.data(),
            kReflectionAaChoices.size()},
    Setting{"LR_SHADOW", 186, "gta4_shadow_map_base_size", kShadowChoices.data(),
            kShadowChoices.size()},
    Setting{"LR_DITHER", 201, "gta4_native_output_dither", kToggleChoices.data(),
            kToggleChoices.size()},
};

constexpr std::string_view kSaveKey = "LR_SAVE";
constexpr uint32_t kSaveLabelOffset = 218;

constexpr std::array<std::string_view, 50> kStringPool = {
    "Render Resolution",      "Aspect Ratio",          "Fullscreen",
    "Presentation",           "HDR",                   "Anti-Aliasing",
    "SMAA Quality",           "Upscaling",             "FSR 1 Quality",
    "Texture Filtering",      "Anisotropic Filtering", "Reflection Resolution",
    "Reflection AA",          "Shadow Quality",        "Output Dithering",
    "Save Graphics Settings", "Automatic",             "1280 x 720",
    "1920 x 1080",            "2560 x 1440",           "3840 x 2160",
    "Display",                "Original 16:9",         "Off",
    "On",                     "VSync (FIFO)",           "Mailbox",
    "Immediate",              "Spatial",               "FXAA",
    "SMAA",                   "Low",                   "Medium",
    "High",                   "Ultra",                 "Native",
    "FSR 1",                  "Ultra Quality",         "Quality",
    "Balanced",               "Performance",           "Original",
    "Bilinear",               "Trilinear",             "2x",
    "4x",                     "8x",                    "16x",
    "1080p",                  "Full",
};

uint32_t g_string_pool_address = 0;
std::filesystem::path g_config_path;
uint64_t g_menu_trace_sequence = 0;
uint32_t g_menu_trace_label_calls = 0;
uint32_t g_menu_trace_value_calls = 0;

bool FrontendDiagnosticsEnabled() noexcept {
  return rex::diagnostics::IsEnabled(
             rex::diagnostics::Category::kGuestHooks) &&
         rex::diagnostics::IsEnabled(
             rex::diagnostics::Category::kLogging);
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
  return "other";
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
      g_string_pool_address);
  if (rows == 0 || count > kMenuTraceRowLimit) {
    REXLOG_INFO("GTA4MenuTrace seq={} point={}-rows-skipped reason={} count={}", sequence,
                point, rows == 0 ? "null-vector" : "count-limit", count);
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

uint32_t DisplayRowAddress(uint8_t* base, int32_t row_index) {
  if (row_index < 0) {
    return 0;
  }
  const uint16_t count = REX_LOAD_U16(kDisplayOptionsCount);
  if (static_cast<uint32_t>(row_index) >= count) {
    return 0;
  }
  const uint32_t rows = REX_LOAD_U32(kDisplayOptionsPointer);
  if (rows == 0) {
    return 0;
  }
  return rows + static_cast<uint32_t>(row_index) * kOptionRecordSize;
}

const Setting* FindSettingByRow(uint8_t* base, int32_t row_index) {
  const uint32_t row = DisplayRowAddress(base, row_index);
  if (row == 0 || REX_LOAD_U8(row + kOptionActionOffset) != kMenuOptionAdjust) {
    return nullptr;
  }
  return FindSettingByKey(base, row + kOptionLabelOffset);
}

bool IsSaveRow(uint8_t* base, int32_t row_index) {
  const uint32_t row = DisplayRowAddress(base, row_index);
  return row != 0 && REX_LOAD_U8(row + kOptionActionOffset) == kMenuOptionJump &&
         GuestStringEquals(base, row + kOptionLabelOffset, kSaveKey, kOptionLabelCapacity);
}

const Choice& CurrentChoice(const Setting& setting) {
  const std::string current = rex::cvar::GetFlagByName(setting.cvar);
  for (uint8_t index = 0; index < setting.choice_count; ++index) {
    if (current == setting.choices[index].value) {
      return setting.choices[index];
    }
  }
  return setting.choices[0];
}

uint8_t CurrentChoiceIndex(const Setting& setting) {
  const std::string current = rex::cvar::GetFlagByName(setting.cvar);
  for (uint8_t index = 0; index < setting.choice_count; ++index) {
    if (current == setting.choices[index].value) {
      return index;
    }
  }
  return 0;
}

void WriteInlineKey(uint8_t* base, uint32_t destination, std::string_view key) {
  for (uint32_t index = 0; index < kOptionLabelCapacity; ++index) {
    REX_STORE_U8(destination + index, 0);
  }
  for (uint32_t index = 0; index < key.size(); ++index) {
    REX_STORE_U8(destination + index, static_cast<uint8_t>(key[index]));
  }
}

void WriteSettingRow(uint8_t* base, uint32_t destination, const Setting& setting) {
  REX_STORE_U8(destination + kOptionActionOffset, kMenuOptionAdjust);
  WriteInlineKey(base, destination + kOptionLabelOffset, setting.key);
  REX_STORE_U16(destination + kOptionValueOffset, kSafeStockPreference);
  REX_STORE_U8(destination + kOptionScalerOffset, setting.choice_count);
  REX_STORE_U8(destination + kOptionDisplayValueOffset, kSafeStockDisplayValue);
}

void WriteSaveRow(uint8_t* base, uint32_t destination) {
  REX_STORE_U8(destination + kOptionActionOffset, kMenuOptionJump);
  WriteInlineKey(base, destination + kOptionLabelOffset, kSaveKey);
  REX_STORE_U16(destination + kOptionValueOffset, 0);
  REX_STORE_U8(destination + kOptionScalerOffset, 0);
  REX_STORE_U8(destination + kOptionDisplayValueOffset, kSafeStockDisplayValue);
}

void WriteSentinelRow(uint8_t* base, uint32_t destination) {
  std::memset(base + destination, 0, kOptionRecordSize);
  REX_STORE_U8(destination + kOptionActionOffset, kEndOfMenuOptions);
}

void WriteStringPool(uint8_t* base, uint32_t destination) {
  uint32_t offset = 0;
  for (std::string_view value : kStringPool) {
    std::memcpy(base + destination + offset, value.data(), value.size());
    REX_STORE_U8(destination + offset + value.size(), 0);
    offset += static_cast<uint32_t>(value.size()) + 1;
  }
  if (offset != kStringPoolBytes) {
    REXLOG_ERROR("GTA IV Display extension: string pool mismatch expected={} actual={}",
                 kStringPoolBytes, offset);
  }
}

void ExtendDisplayOptions(PPCContext& ctx, uint8_t* base) {
  const uint32_t old_rows = REX_LOAD_U32(kDisplayOptionsPointer);
  const uint16_t old_count = REX_LOAD_U16(kDisplayOptionsCount);
  if (old_rows == 0 || old_count == 0) {
    REXLOG_WARN("GTA IV Display extension: stock option vector is empty");
    return;
  }

  uint16_t sentinel_index = old_count;
  for (uint16_t index = 0; index < old_count; ++index) {
    const uint32_t row = old_rows + static_cast<uint32_t>(index) * kOptionRecordSize;
    if (REX_LOAD_U8(row + kOptionActionOffset) == kEndOfMenuOptions) {
      sentinel_index = index;
      break;
    }
  }
  if (sentinel_index == old_count) {
    REXLOG_ERROR("GTA IV Display extension: no stock sentinel in {} rows", old_count);
    return;
  }

  uint16_t stock_count = 0;
  for (uint16_t index = 0; index < sentinel_index; ++index) {
    const uint32_t row = old_rows + static_cast<uint32_t>(index) * kOptionRecordSize;
    const bool is_custom = FindSettingByKey(base, row + kOptionLabelOffset) != nullptr ||
                           GuestStringEquals(base, row + kOptionLabelOffset, kSaveKey,
                                             kOptionLabelCapacity);
    if (!is_custom) {
      ++stock_count;
    }
  }

  const size_t new_count_size = static_cast<size_t>(stock_count) + kSettings.size() + 2;
  if (new_count_size > std::numeric_limits<uint16_t>::max()) {
    REXLOG_ERROR("GTA IV Display extension: option count overflow ({})", new_count_size);
    return;
  }
  const uint16_t new_count = static_cast<uint16_t>(new_count_size);
  const size_t rows_bytes = new_count_size * kOptionRecordSize;
  const size_t allocation_bytes = rows_bytes + kStringPoolBytes;
  if (allocation_bytes > std::numeric_limits<uint32_t>::max()) {
    REXLOG_ERROR("GTA IV Display extension: allocation overflow ({})", allocation_bytes);
    return;
  }

  const uint32_t new_rows =
      InvokeGuest(ctx, base, sub_821B3510, static_cast<uint32_t>(allocation_bytes)).r3.u32;
  if (new_rows == 0) {
    REXLOG_ERROR("GTA IV Display extension: failed to allocate {} bytes", allocation_bytes);
    return;
  }

  uint32_t destination = new_rows;
  for (uint16_t index = 0; index < sentinel_index; ++index) {
    const uint32_t source = old_rows + static_cast<uint32_t>(index) * kOptionRecordSize;
    const bool is_custom = FindSettingByKey(base, source + kOptionLabelOffset) != nullptr ||
                           GuestStringEquals(base, source + kOptionLabelOffset, kSaveKey,
                                             kOptionLabelCapacity);
    if (!is_custom) {
      std::memcpy(base + destination, base + source, kOptionRecordSize);
      destination += kOptionRecordSize;
    }
  }
  for (const Setting& setting : kSettings) {
    WriteSettingRow(base, destination, setting);
    destination += kOptionRecordSize;
  }
  WriteSaveRow(base, destination);
  destination += kOptionRecordSize;
  WriteSentinelRow(base, destination);

  g_string_pool_address = new_rows + static_cast<uint32_t>(rows_bytes);
  WriteStringPool(base, g_string_pool_address);

  REX_STORE_U32(kDisplayOptionsPointer, new_rows);
  REX_STORE_U16(kDisplayOptionsCount, new_count);
  REX_STORE_U16(kDisplayOptionsCapacity, new_count);
  InvokeGuest(ctx, base, sub_821B3560, old_rows);

  REXLOG_INFO(
      "GTA IV Display extension: stock-rows={} custom-settings={} total-rows={} vector={:08X}",
      stock_count, kSettings.size(), new_count, new_rows);
}

void ChangeSetting(PPCContext& ctx, uint8_t* base, const Setting& setting, int32_t delta) {
  if (delta == 0) {
    return;
  }
  uint8_t index = CurrentChoiceIndex(setting);
  if (delta > 0) {
    index = index + 1 == setting.choice_count ? 0 : static_cast<uint8_t>(index + 1);
  } else {
    index = index == 0 ? static_cast<uint8_t>(setting.choice_count - 1)
                       : static_cast<uint8_t>(index - 1);
  }

  const Choice& choice = setting.choices[index];
  if (!rex::cvar::SetFlagByName(setting.cvar, choice.value)) {
    REXLOG_ERROR("GTA IV Display extension: rejected {}={}", setting.cvar, choice.value);
    return;
  }

  PPCContext rebuild = ctx;
  rebuild.r3.u64 = g_adjustment.frontend_channel;
  sub_82255D00(rebuild, base);
  REXLOG_INFO("GTA IV Display extension: changed {}={} direction={}", setting.cvar,
              choice.value, delta);
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
  rex::cvar::SaveConfig(g_config_path);
  REXLOG_INFO("GTA IV Display extension: saved graphics settings to {}",
              g_config_path.string());
}

class ScopedAdjustment final {
 public:
  ScopedAdjustment(uint32_t frontend_channel, bool is_display) : previous_(g_adjustment) {
    g_adjustment = {
        .active = is_display, .frontend_channel = frontend_channel, .delta = 0};
  }

  ~ScopedAdjustment() { g_adjustment = previous_; }

 private:
  AdjustmentContext previous_;
};

}  // namespace

namespace gta4::frontend_menu {

void SetConfigPath(std::filesystem::path path) { g_config_path = std::move(path); }

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
  __imp__sub_82157F90(ctx, base);
  TraceScreenRows(base, "stock-loaded", kAudioScreen);
  TraceScreenRows(base, "stock-loaded", kDisplayScreen);
  ExtendDisplayOptions(ctx, base);
  TraceScreenRows(base, "extension-published", kAudioScreen);
  TraceScreenRows(base, "extension-published", kDisplayScreen);
}

extern "C" void sub_8221FD88(PPCContext& ctx, uint8_t* base) {
  const bool diagnostics = FrontendDiagnosticsEnabled();
  const uint32_t destination = ctx.r3.u32;
  const uint32_t key_address = ctx.r4.u32;
  const uint32_t caller = ctx.lr;
  const uint32_t screen = diagnostics ? REX_LOAD_U32(kCurrentScreenAddress) : 0;
  if (g_string_pool_address != 0) {
    if (const Setting* setting = FindSettingByKey(base, ctx.r4.u32)) {
      ctx.r3.u64 = g_string_pool_address + setting->label_offset;
      if (diagnostics) {
        REXLOG_INFO(
            "GTA4MenuTrace seq={} point=label-resolve-custom screen={}({}) caller={:08X} "
            "destination={:08X} key-address={:08X} key='{}' result={:08X} text='{}'",
            ++g_menu_trace_sequence, screen, TraceScreenName(screen), caller, destination,
            key_address, ReadGuestString(base, key_address, kMenuTraceStringCapacity),
            ctx.r3.u32,
            ReadGuestString(base, ctx.r3.u32, kMenuTraceStringCapacity));
      }
      return;
    }
    if (GuestStringEquals(base, ctx.r4.u32, kSaveKey, kOptionLabelCapacity)) {
      ctx.r3.u64 = g_string_pool_address + kSaveLabelOffset;
      if (diagnostics) {
        REXLOG_INFO(
            "GTA4MenuTrace seq={} point=label-resolve-custom screen={}({}) caller={:08X} "
            "destination={:08X} key-address={:08X} key='{}' result={:08X} text='{}'",
            ++g_menu_trace_sequence, screen, TraceScreenName(screen), caller, destination,
            key_address, ReadGuestString(base, key_address, kMenuTraceStringCapacity),
            ctx.r3.u32,
            ReadGuestString(base, ctx.r3.u32, kMenuTraceStringCapacity));
      }
      return;
    }
  }
  __imp__sub_8221FD88(ctx, base);
  if (diagnostics && (screen == kAudioScreen || screen == kDisplayScreen) &&
      g_menu_trace_label_calls < kMenuTraceCallLimit) {
    ++g_menu_trace_label_calls;
    REXLOG_INFO(
        "GTA4MenuTrace seq={} point=label-resolve-stock screen={}({}) caller={:08X} "
        "destination={:08X} key-address={:08X} key='{}' result={:08X} text='{}'",
        ++g_menu_trace_sequence, screen, TraceScreenName(screen), caller, destination,
        key_address, ReadGuestString(base, key_address, kMenuTraceStringCapacity), ctx.r3.u32,
        ReadGuestString(base, ctx.r3.u32, kMenuTraceStringCapacity));
  }
}

extern "C" void sub_82252A98(PPCContext& ctx, uint8_t* base) {
  const bool diagnostics = FrontendDiagnosticsEnabled();
  const uint32_t frontend_channel = ctx.r3.u32;
  const uint32_t screen = ctx.r4.u32;
  const uint32_t selected_value = ctx.r5.u32;
  const int32_t row_index = ctx.r6.s32;
  if (ctx.r4.u32 == kDisplayScreen && g_string_pool_address != 0) {
    if (const Setting* setting = FindSettingByRow(base, ctx.r6.s32)) {
      ctx.r3.u64 = g_string_pool_address + CurrentChoice(*setting).text_offset;
      if (diagnostics) {
        REXLOG_INFO(
            "GTA4MenuTrace seq={} point=value-resolve-custom channel={} screen={} row={} "
            "selected-value={} key='{}' result={:08X} text='{}'",
            ++g_menu_trace_sequence, frontend_channel, screen, row_index, selected_value,
            setting->key, ctx.r3.u32,
            ReadGuestString(base, ctx.r3.u32, kMenuTraceStringCapacity));
      }
      return;
    }
  }
  __imp__sub_82252A98(ctx, base);
  if (diagnostics && (screen == kAudioScreen || screen == kDisplayScreen) &&
      g_menu_trace_value_calls < kMenuTraceCallLimit) {
    ++g_menu_trace_value_calls;
    REXLOG_INFO(
        "GTA4MenuTrace seq={} point=value-resolve-stock channel={} screen={} row={} "
        "selected-value={} result={:08X} text='{}'",
        ++g_menu_trace_sequence, frontend_channel, screen, row_index, selected_value,
        ctx.r3.u32, ReadGuestString(base, ctx.r3.u32, kMenuTraceStringCapacity));
  }
}

extern "C" void sub_82255D00(PPCContext& ctx, uint8_t* base) {
  if (!FrontendDiagnosticsEnabled()) {
    __imp__sub_82255D00(ctx, base);
    return;
  }
  const uint32_t frontend_channel = ctx.r3.u32;
  const uint32_t screen = REX_LOAD_U32(kCurrentScreenAddress);
  const uint32_t vector = ScreenOptionsAddress(screen);
  const uint32_t rows = REX_LOAD_U32(vector);
  const uint16_t count = REX_LOAD_U16(vector + 4);
  const uint16_t capacity = REX_LOAD_U16(vector + 6);
  g_menu_trace_label_calls = 0;
  g_menu_trace_value_calls = 0;
  REXLOG_INFO(
      "GTA4MenuTrace seq={} point=page-build-enter channel={} screen={}({}) vector={:08X} "
      "rows={:08X} count={} capacity={}",
      ++g_menu_trace_sequence, frontend_channel, screen, TraceScreenName(screen), vector,
      rows, count, capacity);
  __imp__sub_82255D00(ctx, base);
  REXLOG_INFO(
      "GTA4MenuTrace seq={} point=page-build-exit channel={} screen={}({}) "
      "return={:08X} label-calls={} value-calls={}",
      ++g_menu_trace_sequence, frontend_channel, screen, TraceScreenName(screen), ctx.r3.u32,
      g_menu_trace_label_calls, g_menu_trace_value_calls);
}

extern "C" void sub_82258FB0(PPCContext& ctx, uint8_t* base) {
  ScopedAdjustment adjustment(ctx.r3.u32, IsDisplayScreen(base));
  __imp__sub_82258FB0(ctx, base);
}

extern "C" void sub_8229C4F8(PPCContext& ctx, uint8_t* base) {
  __imp__sub_8229C4F8(ctx, base);
  if (g_adjustment.active && ctx.lr == kAdjustmentDeltaReturnAddress) {
    g_adjustment.delta = ctx.r3.s32;
  }
}

extern "C" void sub_82253370(PPCContext& ctx, uint8_t* base) {
  if (g_adjustment.active) {
    if (const Setting* setting = FindSettingByRow(base, ctx.r3.s32)) {
      ChangeSetting(ctx, base, *setting, g_adjustment.delta);
      ctx.r3.u64 = 0;
      return;
    }
  }
  __imp__sub_82253370(ctx, base);
}

extern "C" void sub_82258388(PPCContext& ctx, uint8_t* base) {
  const int32_t selected_row = ctx.r4.s32;
  if (IsDisplayScreen(base) && FindSettingByRow(base, selected_row) != nullptr) {
    ctx.r3.u64 = 0;
    return;
  }
  if (IsDisplayScreen(base) && IsSaveRow(base, selected_row)) {
    SaveGraphicsSettings();
    ctx.r3.u64 = 1;
    return;
  }
  __imp__sub_82258388(ctx, base);
}
