#include <catch2/catch_test_macros.hpp>

#include <limits>

#include <rex/diagnostics/policy.h>

namespace diagnostics = rex::diagnostics;

TEST_CASE("Diagnostics policy is completely disabled by default input",
          "[diagnostics][policy]") {
  diagnostics::Policy policy{};
  std::string error;
  REQUIRE(diagnostics::ParsePolicy(false, {}, &policy, &error));
  REQUIRE_FALSE(policy.enabled);
  for (const bool category_enabled : policy.categories) {
    REQUIRE_FALSE(category_enabled);
  }
  REQUIRE(error.empty());
}

TEST_CASE("Diagnostics master enables every category when no list is supplied",
          "[diagnostics][policy]") {
  diagnostics::Policy policy{};
  REQUIRE(diagnostics::ParsePolicy(true, {}, &policy));
  REQUIRE(policy.enabled);
  for (const bool category_enabled : policy.categories) {
    REQUIRE(category_enabled);
  }
}

TEST_CASE("Diagnostics category list enables only named categories",
          "[diagnostics][policy]") {
  diagnostics::Policy policy{};
  REQUIRE(diagnostics::ParsePolicy(
      true, " native-trace,water-invalid-name", &policy) == false);

  REQUIRE(diagnostics::ParsePolicy(
      true, " native-trace, native-probes,watchdog ", &policy));
  REQUIRE(policy.enabled);
  REQUIRE(policy.categories[static_cast<std::size_t>(
      diagnostics::Category::kNativeTrace)]);
  REQUIRE(policy.categories[static_cast<std::size_t>(
      diagnostics::Category::kNativeProbes)]);
  REQUIRE(policy.categories[static_cast<std::size_t>(
      diagnostics::Category::kWatchdog)]);
  REQUIRE_FALSE(policy.categories[static_cast<std::size_t>(
      diagnostics::Category::kLogging)]);
  REQUIRE_FALSE(policy.categories[static_cast<std::size_t>(
      diagnostics::Category::kPresenter)]);
}

TEST_CASE("Profiler artifacts do not implicitly enable general logging",
          "[diagnostics][policy]") {
  diagnostics::Policy policy{};
  REQUIRE(diagnostics::ParsePolicy(true, "native-profiler", &policy));
  REQUIRE(policy.enabled);
  REQUIRE(policy.categories[static_cast<std::size_t>(
      diagnostics::Category::kNativeProfiler)]);
  REQUIRE_FALSE(policy.categories[static_cast<std::size_t>(
      diagnostics::Category::kLogging)]);
}

TEST_CASE("Diagnostics queries reject out-of-range categories",
          "[diagnostics][policy]") {
  struct PolicyResetGuard {
    ~PolicyResetGuard() { diagnostics::testing::ResetPolicy(); }
  } reset_guard;
  diagnostics::testing::ResetPolicy();
  REQUIRE(diagnostics::Configure(true, {}));
  REQUIRE_FALSE(diagnostics::IsEnabled(diagnostics::Category::kCount));
  REQUIRE_FALSE(diagnostics::IsEnabled(
      static_cast<diagnostics::Category>(
          std::numeric_limits<std::size_t>::max())));
}

TEST_CASE("Diagnostics category list cannot bypass the master switch",
          "[diagnostics][policy]") {
  diagnostics::Policy policy{};
  std::string error;
  REQUIRE_FALSE(
      diagnostics::ParsePolicy(false, "logging", &policy, &error));
  REQUIRE(error == "--diagnostics-categories requires --diagnostics");
}

TEST_CASE("Diagnostics policy rejects malformed and unknown categories",
          "[diagnostics][policy]") {
  diagnostics::Policy policy{};
  std::string error;
  REQUIRE_FALSE(diagnostics::ParsePolicy(
      true, "native-trace,,presenter", &policy, &error));
  REQUIRE(error == "--diagnostics-categories contains an empty category");

  error.clear();
  REQUIRE_FALSE(
      diagnostics::ParsePolicy(true, "unknown", &policy, &error));
  REQUIRE(error == "unknown diagnostics category: unknown");
}

TEST_CASE("Installed diagnostics policy is immutable but identical setup is idempotent",
          "[diagnostics][policy]") {
  struct PolicyResetGuard {
    ~PolicyResetGuard() { diagnostics::testing::ResetPolicy(); }
  } reset_guard;
  diagnostics::testing::ResetPolicy();
  std::string error;
  REQUIRE(diagnostics::Configure(false, {}, &error));
  REQUIRE(diagnostics::Configure(false, {}, &error));
  REQUIRE_FALSE(diagnostics::Configure(true, {}, &error));
  REQUIRE(error == "diagnostics policy is already configured differently");
}
