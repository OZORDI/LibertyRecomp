#pragma once
// Opt-in diagnostic sidecar. Original title-command bytes and ABI stay unchanged.
#include <algorithm>
#include <array>
#include <atomic>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#ifndef REX_FIRE_TRACE_TEST
#include <rex/logging.h>
#endif
namespace rex::graphics::gta4_native {
inline constexpr uint32_t kFireTraceEnvelopeAbi = 0x46495245;
inline uint32_t FireTraceUnsigned(const char* name, uint32_t fallback, uint32_t maximum) {
  const char* value = std::getenv(name);
  if (!value || !*value || *value == '-') return fallback;
  char* end = nullptr; errno = 0;
  const auto n = std::strtoull(value, &end, 10);
  return errno || end == value || *end || n > maximum ? fallback : uint32_t(n);
}
struct FireTraceConfiguration {
  bool enabled = false;
  uint32_t interval = 120, image_interval_seconds = 20;
  uint32_t image_frame_mib = 192, disk_mib = 640;
  std::string directory;
};
inline const FireTraceConfiguration& FireTraceConfig() {
  static const auto c = [] {
    FireTraceConfiguration x;
    x.enabled = FireTraceUnsigned("REX_GTA4_FIRE_TRACE", 0, 1) != 0;
    x.interval = std::max(1u, FireTraceUnsigned("REX_GTA4_FIRE_INTERVAL", 120, 3600));
    x.image_interval_seconds = std::max(2u, FireTraceUnsigned("REX_GTA4_FIRE_IMAGE_SECONDS", 20, 120));
    x.image_frame_mib = std::max(32u, FireTraceUnsigned("REX_GTA4_FIRE_FRAME_MIB", 192, 512));
    x.disk_mib = FireTraceUnsigned("REX_GTA4_FIRE_DISK_MIB", 640, 2048);
    if (const auto* p = std::getenv("REX_GTA4_FIRE_OUTPUT")) x.directory = p;
    return x;
  }();
  return c;
}
inline bool FireTextureName(std::string_view name) {
  return name.find("sl_rustedmtl_rail01") != std::string_view::npos ||
         name.find("sl_rustedmtl_msh01") != std::string_view::npos ||
         name.find("sl_rustedmetal01_256") != std::string_view::npos ||
         name.find("sl_rustedftplate01") != std::string_view::npos ||
         name.find("fire_esc") != std::string_view::npos;
}
inline bool FireStrongTextureName(std::string_view name) {
  return name.find("sl_rustedmtl_rail01") != std::string_view::npos ||
         name.find("sl_rustedmtl_msh01") != std::string_view::npos ||
         name.find("fire_esc") != std::string_view::npos;
}
struct FireTraceContext {
  uint64_t occurrence = 0, event = 0;
  uint32_t device = 0, guest_frame = 0, caller = 0;
  uint32_t model = 0, geometry = 0, material = 0, ordinal = 0, bucket = 0;
  uint32_t technique = 0, pass = 0, texture_wrapper = 0, replay = 0;
  uint32_t command_list = 0, replay_ordinal = 0;
  std::array<char, 96> texture_name{};
};
struct alignas(8) FireTraceEnvelope {
  uint32_t version = 1, command_abi = 0, command_size = 0, reserved = 0;
  FireTraceContext context{};
};
static_assert(std::is_trivially_copyable_v<FireTraceEnvelope>);
inline std::vector<uint8_t> PackFireTraceEnvelope(const void* data, size_t size,
                                                uint32_t abi, const FireTraceContext& context) {
  if (!data || !size || size > UINT32_MAX || abi == kFireTraceEnvelopeAbi ||
      !context.occurrence || !context.event) return {};
  FireTraceEnvelope e; e.command_abi = abi; e.command_size = uint32_t(size); e.context = context;
  std::vector<uint8_t> result(sizeof(e) + size);
  std::memcpy(result.data(), &e, sizeof(e));
  std::memcpy(result.data() + sizeof(e), data, size);
  return result;
}
inline bool UnpackFireTraceEnvelope(const void*& data, size_t& size, uint32_t& abi,
                                   FireTraceContext& context) {
  if (abi != kFireTraceEnvelopeAbi || !data || size < sizeof(FireTraceEnvelope)) return false;
  FireTraceEnvelope e{}; std::memcpy(&e, data, sizeof(e));
  if (e.version != 1 || e.reserved || !e.context.occurrence || !e.context.event ||
      e.command_abi == kFireTraceEnvelopeAbi || !e.command_size ||
      e.command_size != size - sizeof(e) || e.context.texture_name.back()) return false;
  data = static_cast<const uint8_t*>(data) + sizeof(e);
  size = e.command_size; abi = e.command_abi; context = e.context; return true;
}
inline void FireTraceLog(std::string_view point, std::string_view details) {
#ifndef REX_FIRE_TRACE_TEST
  if (FireTraceConfig().enabled) REXLOG_INFO("gta4-fire: point={} {}", point, details);
#else
  (void)point; (void)details;
#endif
}
}  // namespace rex::graphics::gta4_native
