#include <rex/diagnostics/policy.h>

#include <algorithm>
#include <atomic>
#include <cctype>
#include <mutex>
#include <sstream>
#include <string>

namespace rex::diagnostics {
namespace {

Policy g_policy;
std::atomic<bool> g_configured = false;
std::mutex g_configure_mutex;

struct CategoryEntry {
  std::string_view name;
  Category category;
};

constexpr std::array kCategories = {
    CategoryEntry{"logging", Category::kLogging},
    CategoryEntry{"native-trace", Category::kNativeTrace},
    CategoryEntry{"native-probes", Category::kNativeProbes},
    CategoryEntry{"native-translucency", Category::kNativeTranslucency},
    CategoryEntry{"native-profiler", Category::kNativeProfiler},
    CategoryEntry{"presenter", Category::kPresenter},
    CategoryEntry{"guest-hooks", Category::kGuestHooks},
    CategoryEntry{"physics", Category::kPhysics},
    CategoryEntry{"legal", Category::kLegal},
    CategoryEntry{"audio", Category::kAudio},
    CategoryEntry{"vulkan", Category::kVulkan},
    CategoryEntry{"watchdog", Category::kWatchdog},
    CategoryEntry{"transition", Category::kTransition},
};

std::string Normalize(std::string value) {
  value.erase(std::remove_if(value.begin(), value.end(),
                             [](unsigned char character) {
                               return std::isspace(character) != 0;
                             }),
              value.end());
  std::transform(value.begin(), value.end(), value.begin(),
                 [](unsigned char character) {
                   return static_cast<char>(std::tolower(character));
                 });
  return value;
}

void EnableAll(Policy* policy) noexcept {
  policy->categories.fill(true);
}

}  // namespace

bool ParsePolicy(bool enabled, std::string_view category_list, Policy* policy,
                 std::string* error) {
  if (!policy) {
    if (error) {
      *error = "diagnostics policy output is null";
    }
    return false;
  }

  Policy parsed{};
  parsed.enabled = enabled;
  const std::string normalized_list = Normalize(std::string(category_list));
  if (!enabled) {
    if (!normalized_list.empty()) {
      if (error) {
        *error = "--diagnostics-categories requires --diagnostics";
      }
      return false;
    }
    *policy = parsed;
    return true;
  }

  if (normalized_list.empty() || normalized_list == "all") {
    EnableAll(&parsed);
    *policy = parsed;
    return true;
  }

  std::istringstream stream(normalized_list);
  std::string token;
  while (std::getline(stream, token, ',')) {
    if (token.empty()) {
      if (error) {
        *error = "--diagnostics-categories contains an empty category";
      }
      return false;
    }
    const auto entry = std::find_if(
        kCategories.begin(), kCategories.end(),
        [&token](const CategoryEntry& candidate) { return candidate.name == token; });
    if (entry == kCategories.end()) {
      if (error) {
        *error = "unknown diagnostics category: " + token;
      }
      return false;
    }
    parsed.categories[static_cast<std::size_t>(entry->category)] = true;
  }

  *policy = parsed;
  return true;
}

bool Configure(bool enabled, std::string_view category_list, std::string* error) {
  std::lock_guard lock(g_configure_mutex);
  Policy parsed{};
  if (!ParsePolicy(enabled, category_list, &parsed, error)) {
    return false;
  }
  if (g_configured.load(std::memory_order_relaxed)) {
    if (g_policy.enabled == parsed.enabled &&
        g_policy.categories == parsed.categories) {
      return true;
    }
    if (error) {
      *error = "diagnostics policy is already configured differently";
    }
    return false;
  }
  g_policy = parsed;
  g_configured.store(true, std::memory_order_release);
  return true;
}

bool IsConfigured() noexcept {
  return g_configured.load(std::memory_order_acquire);
}

bool IsEnabled() noexcept {
  return IsConfigured() && g_policy.enabled;
}

bool IsEnabled(Category category) noexcept {
  if (!IsEnabled() ||
      static_cast<std::size_t>(category) >=
          static_cast<std::size_t>(Category::kCount)) {
    return false;
  }
  return g_policy.categories[static_cast<std::size_t>(category)];
}

std::string_view CategoryName(Category category) noexcept {
  const auto entry = std::find_if(
      kCategories.begin(), kCategories.end(),
      [category](const CategoryEntry& candidate) { return candidate.category == category; });
  return entry == kCategories.end() ? std::string_view{} : entry->name;
}

#if defined(REX_DIAGNOSTICS_TESTING)
namespace testing {
void ResetPolicy() noexcept {
  std::lock_guard lock(g_configure_mutex);
  g_policy = {};
  g_configured.store(false, std::memory_order_release);
}
}  // namespace testing
#endif

}  // namespace rex::diagnostics
