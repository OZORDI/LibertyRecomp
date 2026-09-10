#pragma once

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace libserver {
struct QosProbeAttempt {
  std::string id;
  std::string session_id;
  std::string host_xuid;
  std::string exchange_key;
  std::string challenge;
  std::uint64_t listener_generation = 0;
  std::int64_t host_epoch = 0;
  bool eligible = false;
  bool delivered = false;
  bool acknowledged = false;
  std::uint64_t round_trip_microseconds = 0;
  std::optional<std::string> title_data;
};
struct QosProbeBatch {
  std::string owner_xuid;
  bool completed = false;
  std::chrono::steady_clock::time_point started;
  std::chrono::steady_clock::time_point deadline;
  std::chrono::steady_clock::time_point expires;
  std::vector<QosProbeAttempt> attempts;
};
inline constexpr auto kQosProbeDeadline = std::chrono::milliseconds(2000);
inline constexpr auto kQosProbeReceiptLifetime = std::chrono::seconds(15);
inline constexpr std::size_t kMaximumQosProbeBatches = 512;
inline constexpr std::size_t kMaximumQosBatchesPerOwner = 4;
}  // namespace libserver
