/**
 * @file diagnostics/policy.h
 * @brief Immutable, command-line-owned diagnostics policy.
 */

#pragma once

#include <array>
#include <cstddef>
#include <string>
#include <string_view>

namespace rex::diagnostics {

enum class Category : std::size_t {
  kLogging,
  kNativeTrace,
  kNativeProbes,
  kNativeTranslucency,
  kNativeProfiler,
  kPresenter,
  kGuestHooks,
  kPhysics,
  kLegal,
  kAudio,
  kVulkan,
  kWatchdog,
  kTransition,
  kCount,
};

struct Policy {
  bool enabled = false;
  std::array<bool, static_cast<std::size_t>(Category::kCount)> categories{};
};

// Parses a policy without changing process state. This is also the test seam.
bool ParsePolicy(bool enabled, std::string_view category_list, Policy* policy,
                 std::string* error = nullptr);

// Installs the process policy before any worker threads or logger sinks are
// created. Repeating the identical policy is idempotent; a conflicting second
// call is rejected so config files, CVARs and environment variables cannot
// mutate the command-line decision later.
bool Configure(bool enabled, std::string_view category_list,
               std::string* error = nullptr);

bool IsConfigured() noexcept;
bool IsEnabled() noexcept;
bool IsEnabled(Category category) noexcept;
std::string_view CategoryName(Category category) noexcept;

#if defined(REX_DIAGNOSTICS_TESTING)
namespace testing {
// Unit-test-only seam. Production builds do not declare or compile this API.
void ResetPolicy() noexcept;
}  // namespace testing
#endif

}  // namespace rex::diagnostics
