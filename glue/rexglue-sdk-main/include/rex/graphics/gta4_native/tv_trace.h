#pragma once
// Opt-in diagnostic metadata. Title commands retain their normal ABI and bytes.
#include <algorithm>
#include <array>
#include <atomic>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#ifndef REX_TV_TRACE_TEST
#include <rex/logging.h>
#endif

namespace rex::graphics::gta4_native {
inline constexpr uint32_t kTvTraceEnvelopeAbi = 0x54565452;
inline constexpr uint32_t kTvScriptName = 0x82001950;
inline bool IsTvBinkShader(uint64_t hash) {
  return hash == 0x9E76B68B60127349ull || hash == 0xA6C9E2B8B2A59D7Aull;
}
inline uint32_t TvTraceUnsigned(const char* name, uint32_t fallback, uint32_t maximum) {
  const char* value = std::getenv(name);
  if (!value || !*value || *value == '-') return fallback;
  char* end = nullptr;
  errno = 0;
  const auto result = std::strtoull(value, &end, 10);
  return errno || end == value || *end || result > maximum ? fallback : uint32_t(result);
}
struct TvTraceConfiguration {
  bool enabled = false, readbacks = false;
  uint32_t frames = 180;
  std::string arm_file, output_directory;
};
inline const TvTraceConfiguration& TvTraceConfig() {
  static const auto config = [] {
    TvTraceConfiguration c;
    c.enabled = TvTraceUnsigned("REX_GTA4_TV_TRACE", 0, 1) != 0;
    c.readbacks = c.enabled && TvTraceUnsigned("REX_GTA4_TV_READBACKS", 0, 1);
    c.frames = std::max(2u, TvTraceUnsigned("REX_GTA4_TV_FRAMES", 180, 600));
    if (auto p = std::getenv("REX_GTA4_TV_ARM_FILE")) c.arm_file = p;
    if (auto p = std::getenv("REX_GTA4_TV_OUTPUT")) c.output_directory = p;
    return c;
  }();
  return config;
}
struct TvTraceContext {
  uint64_t run = 0, event = 0, binding_event = 0;
  uint32_t device = 0, guest_frame = 0;
  uint32_t script_wrapper = 0, script_surface = 0, script_texture = 0;
  uint32_t player = 0, decoder = 0, frame_table = 0, frame_index = 0;
  uint32_t ready = 0, watching = 0;
  std::array<uint32_t, 4> plane_wrappers{}, plane_handles{};
};
struct alignas(8) TvTraceEnvelope {
  uint32_t version = 1, command_abi = 0, command_size = 0, reserved = 0;
  TvTraceContext context{};
};
static_assert(std::is_trivially_copyable_v<TvTraceEnvelope>);
static_assert(sizeof(TvTraceEnvelope) % alignof(uint64_t) == 0);
inline std::vector<uint8_t> PackTvTraceEnvelope(const void* command, size_t size,
    uint32_t abi, const TvTraceContext& context) {
  if (!command || !size || size > UINT32_MAX || abi == kTvTraceEnvelopeAbi ||
      !context.run || !context.event) throw std::invalid_argument("invalid TV diagnostic envelope");
  TvTraceEnvelope envelope;
  envelope.command_abi = abi; envelope.command_size = uint32_t(size); envelope.context = context;
  std::vector<uint8_t> bytes(sizeof(envelope) + size);
  std::memcpy(bytes.data(), &envelope, sizeof(envelope));
  std::memcpy(bytes.data() + sizeof(envelope), command, size);
  return bytes;
}
inline bool UnpackTvTraceEnvelope(const void*& command, size_t& size, uint32_t& abi,
    TvTraceContext& context) {
  if (abi != kTvTraceEnvelopeAbi || !command || size < sizeof(TvTraceEnvelope)) return false;
  TvTraceEnvelope envelope{}; std::memcpy(&envelope, command, sizeof(envelope));
  if (envelope.version != 1 || envelope.reserved || !envelope.context.run ||
      !envelope.context.event || envelope.command_abi == kTvTraceEnvelopeAbi ||
      !envelope.command_size || envelope.command_size != size - sizeof(envelope)) return false;
  command = static_cast<const uint8_t*>(command) + sizeof(envelope);
  size = envelope.command_size; abi = envelope.command_abi; context = envelope.context;
  return true;
}
inline void TvTraceLog(std::string_view point, std::string_view details) {
#ifndef REX_TV_TRACE_TEST
  if (!TvTraceConfig().enabled) return;
  static std::atomic<uint64_t> count{0};
  const auto record = count.fetch_add(1, std::memory_order_relaxed);
  if (record < 524288) REXLOG_INFO("gta4-tv-detail: point={} {}", point, details);
  else if (record == 524288) REXLOG_ERROR("gta4-tv-detail: point=incomplete reason=record-budget");
#else
  (void)point; (void)details;
#endif
}
inline std::filesystem::path TvTraceDirectory(uint64_t run) {
  std::error_code error;
  auto parent = TvTraceConfig().output_directory.empty()
      ? std::filesystem::temp_directory_path(error)
      : std::filesystem::path(TvTraceConfig().output_directory);
  return error ? std::filesystem::path{} : parent / ("tv-" + std::to_string(run));
}
struct TvProbeIdentity {
  TvTraceContext context{};
  uint64_t seq = 0, submission = 0, image = 0, lifetime = 0, generation = 0;
  uint64_t writer = 0, ancestor = 0;
  uint32_t epoch = 0, frame = 0, command_index = 0, checkpoint = 0;
  uint32_t attachment = 0, bytes_per_texel = 0;
  std::string role;
};
} // namespace rex::graphics::gta4_native
