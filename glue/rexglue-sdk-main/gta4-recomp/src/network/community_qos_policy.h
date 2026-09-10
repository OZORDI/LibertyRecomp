#pragma once

#include <algorithm>
#include <cstdint>
#include <limits>
#include <optional>

namespace gta4::network::detail {

// Client -> relay RPC duration includes the server's bounded host-probe wait.
// Remove that shared wait, then add the individual host's authenticated probe
// duration. This measures the relay path, not a direct peer UDP path. Reject
// inconsistent timing and never substitute one batch RTT for all targets.
inline std::optional<std::uint16_t> RelayHostRttMilliseconds(
    std::uint64_t rpc_microseconds, std::uint64_t service_microseconds,
    std::uint64_t host_microseconds) noexcept {
  if (host_microseconds > service_microseconds ||
      service_microseconds > rpc_microseconds) return std::nullopt;
  const std::uint64_t outer = rpc_microseconds - service_microseconds;
  // host <= service proves the sum is bounded by rpc, including UINT64_MAX.
  const std::uint64_t result = (outer + host_microseconds) / 1000;
  return static_cast<std::uint16_t>(std::min<std::uint64_t>(
      result, std::numeric_limits<std::uint16_t>::max()));
}

}  // namespace gta4::network::detail
