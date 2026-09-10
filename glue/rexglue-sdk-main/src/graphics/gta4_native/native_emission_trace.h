#pragma once
// Opt-in observation only. None of these selectors changes a production draw.
#include <algorithm>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <string_view>
#include <rex/diagnostics/policy.h>

namespace rex::graphics::gta4_native {
inline uint32_t EmissionTraceUnsigned(const char* name, uint32_t fallback, uint32_t limit) {
  const char* text = std::getenv(name);
  if (!text || !*text || *text == '-') return fallback;
  char* end = nullptr;
  errno = 0;
  const auto value = std::strtoull(text, &end, 10);
  return errno || end == text || *end || value > limit ? fallback : uint32_t(value);
}
struct EmissionTraceConfiguration {
  bool enabled = false;
  bool pipeline = false;
  bool pipeline_full_readback = false;
  bool corona_trace = false;
  bool require_full_region = false;
  uint32_t pipeline_frames = 12;
  uint32_t pipeline_interval = 120;
  uint32_t pipeline_writers = 160;
  bool probes = false;
  bool variants = false;
  bool color_readback = false;
  uint64_t vertex_hash = 0;
  uint32_t indices = 0;
  uint32_t minimum_frame = 0;
  std::filesystem::path directory;
  std::filesystem::path arm_file;
};
inline const EmissionTraceConfiguration& EmissionTraceConfig() {
  static const auto config = [] {
    EmissionTraceConfiguration c;
    c.pipeline = EmissionTraceUnsigned("REX_GTA4_BULB_PIPELINE_TRACE", 0, 1) != 0;
    c.require_full_region = EmissionTraceUnsigned("REX_GTA4_BULB_REQUIRE_FULL_REGION", 0, 1) != 0;
    c.pipeline_full_readback = c.pipeline && EmissionTraceUnsigned("REX_GTA4_BULB_FULL_READBACK", 0, 1) != 0;
    c.corona_trace = c.pipeline_full_readback && EmissionTraceUnsigned("REX_GTA4_BULB_CORONA_TRACE", 0, 1) != 0;
    c.pipeline_frames = std::max(1u, EmissionTraceUnsigned("REX_GTA4_BULB_PIPELINE_FRAMES", 12, 64));
    c.pipeline_interval = std::max(1u, EmissionTraceUnsigned("REX_GTA4_BULB_PIPELINE_INTERVAL", 120, 3600));
    c.pipeline_writers = std::max(16u, EmissionTraceUnsigned("REX_GTA4_BULB_PIPELINE_WRITERS", 160, 192));
    c.enabled = c.pipeline || EmissionTraceUnsigned("REX_GTA4_EMISSION_TRACE", 0, 1) != 0;
    c.probes = EmissionTraceUnsigned("REX_GTA4_EMISSION_PROBES", 0, 1) != 0;
    c.variants = EmissionTraceUnsigned("REX_GTA4_EMISSION_VARIANTS", 0, 1) != 0;
    c.color_readback = EmissionTraceUnsigned("REX_GTA4_EMISSION_COLOR_READBACK", 0, 1) != 0;
    if (const char* text = std::getenv("REX_GTA4_EMISSION_VERTEX_HASH")) {
      char* end = nullptr;
      errno = 0;
      const auto value = std::strtoull(text, &end, 16);
      if (!errno && end != text && !*end && *text != '-') c.vertex_hash = value;
    }
    c.indices = EmissionTraceUnsigned("REX_GTA4_EMISSION_INDICES", 0, 65536);
    c.minimum_frame = EmissionTraceUnsigned("REX_GTA4_EMISSION_MIN_FRAME", 0, UINT32_MAX);
    if (const auto* p = std::getenv("REX_GTA4_EMISSION_OUTPUT")) c.directory = p;
    if (const auto* p = std::getenv("REX_GTA4_EMISSION_ARM_FILE")) c.arm_file = p;
    return c;
  }();
  return config;
}
inline bool EmissionTraceActive(uint32_t frame) {
  const auto& c = EmissionTraceConfig();
  if (!c.enabled || frame < c.minimum_frame ||
      !rex::diagnostics::IsEnabled(rex::diagnostics::Category::kLogging)) return false;
  if (c.arm_file.empty()) return true;
  std::error_code error;
  return std::filesystem::is_regular_file(c.arm_file, error) && !error;
}
inline bool IsEmissionShader(std::string_view filename) {
  return filename.find("/gta_emissive") != std::string_view::npos ||
         filename.find("/gta_glass_emissive") != std::string_view::npos ||
         filename.find("/gta_normal_spec_reflect_emissive") != std::string_view::npos;
}
inline bool IsFixtureEmissionProbe(std::string_view filename, uint32_t indices) {
  if (!EmissionTraceConfig().probes || !IsEmissionShader(filename)) return false;
  const auto selected = EmissionTraceConfig().indices;
  return selected ? indices == selected : (indices == 72 || indices == 150 || indices == 1080);
}
} // namespace rex::graphics::gta4_native
