#include <catch2/catch_test_macros.hpp>
#include <optional>
#include <string>
#include "network/pending_stats_flush.h"
#include "network/community_qos_policy.h"

namespace {
struct Pending { uint64_t session_id; std::string sequence; std::string idempotency; };
using gta4::network::detail::FlushPendingStats;
using gta4::network::detail::RelayHostRttMilliseconds;

TEST_CASE("community flush preserves and retries the actual pending transaction", "[community-reliability]") {
  std::optional<Pending> pending = Pending{42, "7", "original-key"};
  unsigned calls = 0;
  auto unavailable = [&](const Pending& value, bool& terminal) {
    ++calls; CHECK(value.sequence == "7"); CHECK(value.idempotency == "original-key");
    terminal = false; return false;
  };
  CHECK_FALSE(FlushPendingStats(42, true, pending, unavailable));
  REQUIRE(pending); CHECK(calls == 1);
  // Write failure marked the backend unready; Flush must still try the normal
  // authenticated request path so server recovery can settle the transaction.
  auto recovered = [&](const Pending& value, bool&) {
    ++calls; CHECK(value.session_id == 42); CHECK(value.idempotency == "original-key"); return true;
  };
  CHECK(FlushPendingStats(42, false, pending, recovered));
  CHECK_FALSE(pending); CHECK(calls == 2);
}
TEST_CASE("community flush does not drop terminal errors or another session's work", "[community-reliability]") {
  std::optional<Pending> pending = Pending{42, "8", "same-sequence"};
  unsigned calls = 0;
  auto terminal_error = [&](const Pending&, bool& terminal) { ++calls; terminal = true; return false; };
  CHECK_FALSE(FlushPendingStats(42, true, pending, terminal_error));
  REQUIRE(pending);
  CHECK_FALSE(FlushPendingStats(42, true, pending, terminal_error));
  REQUIRE(pending); CHECK(calls == 2);
  CHECK(FlushPendingStats(43, true, pending, terminal_error));
  CHECK_FALSE(FlushPendingStats(43, false, pending, terminal_error));
  CHECK_FALSE(FlushPendingStats(0, true, pending, terminal_error));
  CHECK(calls == 2); REQUIRE(pending); CHECK(pending->session_id == 42);
}
TEST_CASE("community QoS reports separate authenticated relay host timings", "[community-reliability]") {
  CHECK(RelayHostRttMilliseconds(UINT64_C(80000), UINT64_C(60000), UINT64_C(10000)) == 30);
  CHECK(RelayHostRttMilliseconds(UINT64_C(80000), UINT64_C(60000), UINT64_C(40000)) == 60);
  CHECK(RelayHostRttMilliseconds(UINT64_C(0), UINT64_C(0), UINT64_C(0)) == 0);
  CHECK(RelayHostRttMilliseconds(UINT64_C(18446744073709551615), UINT64_C(18446744073709551615), UINT64_C(18446744073709551615)) == 65535);
  CHECK_FALSE(RelayHostRttMilliseconds(1000, 2000, 1000));
  CHECK_FALSE(RelayHostRttMilliseconds(3000, 1000, 2000));
}
}  // namespace
