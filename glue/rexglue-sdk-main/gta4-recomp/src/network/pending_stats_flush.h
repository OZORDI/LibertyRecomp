#pragma once

#include <cstdint>
#include <optional>
#include <utility>

namespace gta4::network::detail {
// Caller holds the same mutex as Write(). A flush must settle the retained
// request using its original sequence/idempotency key, not claim success from
// the existence of a session ID. Failed attempts retain ownership for retry.
template <typename PendingWrite, typename Send>
bool FlushPendingStats(std::uint64_t session_id, bool service_ready,
                       std::optional<PendingWrite>& pending, Send&& send) {
  if (!session_id) return false;
  if (!pending || pending->session_id != session_id) return service_ready;
  // A retryable Write can mark the backend unavailable. Send must get a
  // chance to reconnect using the normal authenticated request path; refusing
  // here would retain the request forever even after the server recovered.
  bool terminal_failure = false;
  if (!std::forward<Send>(send)(*pending, terminal_failure)) return false;
  pending.reset();
  return true;
}
}  // namespace gta4::network::detail
