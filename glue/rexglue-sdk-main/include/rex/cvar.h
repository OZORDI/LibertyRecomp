/**
 * @file        cvar.h
 * @brief       cvar (configuration variable) system
 *
 * @section cvar_defining Defining CVars
 *
 * Define cvars in a single .cpp file using REXCVAR_DEFINE_* macros:
 * @code
 * // In flags.cpp or your module's .cpp file
 * REXCVAR_DEFINE_BOOL(my_flag, false, "Category", "Description");
 * REXCVAR_DEFINE_INT32(my_int, 42, "Category", "An integer setting");
 * REXCVAR_DEFINE_STRING(my_string, "default", "Category", "A string setting");
 * @endcode
 *
 * Available types: BOOL, INT32, INT64, UINT32, UINT64, DOUBLE, STRING, COMMAND
 *
 * @section cvar_commands Defining Commands
 *
 * No-argument command:
 * @code
 * REXCVAR_DEFINE_COMMAND(my_cmd, []() { DoThing(); }, "Category", "Description");
 * @endcode
 *
 * Argument-taking command (the console passes the text after the name):
 * @code
 * REXCVAR_DEFINE_COMMAND_ARGS(echo, [](std::string_view args) {
 *   REXLOG_INFO("{}", args);
 * }, "Console", "Echo arguments");
 * @endcode
 *
 * @section cvar_declaring Declaring CVars (for use in other files)
 *
 * @code
 * // In a header or other .cpp that needs access
 * REXCVAR_DECLARE(bool, my_flag);
 * REXCVAR_DECLARE(int32_t, my_int);
 * REXCVAR_DECLARE(std::string, my_string);
 * @endcode
 *
 * @section cvar_access Getting and Setting Values
 *
 * @code
 * // Type-safe access (preferred for known cvars)
 * bool value = REXCVAR_GET(my_flag);
 * REXCVAR_SET(my_flag, true);
 *
 * // String-based access (for dynamic/runtime lookup)
 * std::string str_val = rex::cvar::GetFlagByName("my_flag");
 * rex::cvar::SetFlagByName("my_flag", "true");
 * @endcode
 *
 * @section cvar_metadata Adding Metadata
 *
 * Chain metadata methods after the DEFINE macro:
 * @code
 * REXCVAR_DEFINE_INT32(scale, 1, "GPU", "Resolution scale")
 *     .range(1, 8)
 *     .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
 *
 * REXCVAR_DEFINE_STRING(backend, "d3d12", "GPU", "Render backend")
 *     .allowed({"d3d12", "vulkan"})
 *     .lifecycle(rex::cvar::Lifecycle::kInitOnly);
 *
 * REXCVAR_DEFINE_BOOL(debug_overlay, false, "Debug", "Show overlay")
 *     .debug_only();
 * @endcode
 *
 * @section cvar_guidelines Metadata Guidelines
 *
 * - .lifecycle(kInitOnly) - Device/backend selection that cannot change after init
 * - .lifecycle(kRequiresRestart) - Settings that need restart to take effect
 * - .lifecycle(kHotReload) - Default; can change at runtime with immediate effect
 * - .range(min, max) - Numeric bounds validation
 * - .allowed({...}) - String enum validation
 * - .debug_only() - Mark as debug-only (for filtering in release UIs)
 * - .validator(fn) - Custom validation function
 *
 * @section cvar_query Querying CVars
 *
 * @code
 * // List all cvars
 * auto all = rex::cvar::ListFlags();
 *
 * // Query by category or lifecycle
 * auto gpu_flags = rex::cvar::ListFlagsByCategory("GPU");
 * auto init_only = rex::cvar::ListFlagsByLifecycle(rex::cvar::Lifecycle::kInitOnly);
 *
 * // Get metadata for a specific cvar
 * const auto* info = rex::cvar::GetFlagInfo("my_flag");
 * if (info) {
 *     // Access info->lifecycle, info->constraints, info->description, etc.
 * }
 *
 * // Check for modified values
 * auto modified = rex::cvar::ListModifiedFlags();
 * @endcode
 *
 * @copyright   Copyright (c) 2026 Tom Clay
 * @license     BSD 3-Clause License
 */

#pragma once

#include <charconv>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace rex::cvar {

//=============================================================================
// Initialization API
//=============================================================================

std::vector<std::string> Init(int argc, char** argv);
void LoadConfig(const std::filesystem::path& config_path);
void ApplyEnvironment();
void FinalizeInit();
bool IsFinalized();
void SaveConfig(const std::filesystem::path& config_path);

//=============================================================================
// Flag Registry
//=============================================================================

enum class FlagType { Boolean, Int32, Int64, Uint32, Uint64, Double, String, Command };

// Lifecycle: when can this flag be modified?
enum class Lifecycle {
  kInitOnly,        // Can only be set during initialization (before FinalizeInit)
  kHotReload,       // Can be changed at runtime with immediate effect
  kRequiresRestart  // Can be changed, but only takes effect after restart
};

// Validation constraints
struct Constraints {
  std::optional<double> min;
  std::optional<double> max;
  std::vector<std::string> allowed_values;
  std::function<bool(std::string_view)> custom_validator;

  bool HasRangeConstraint() const { return min.has_value() || max.has_value(); }
  bool HasAllowedValues() const { return !allowed_values.empty(); }
};

struct FlagEntry {
  std::string name;
  FlagType type;
  std::string category;
  std::string description;
  std::function<bool(std::string_view)> setter;
  std::function<std::string()> getter;
  std::function<void(std::string_view args)> command_callback;
  Lifecycle lifecycle = Lifecycle::kHotReload;
  Constraints constraints;
  std::string default_value;
  bool is_debug_only = false;
};

std::vector<FlagEntry>& GetRegistry();

/**
 * Returns the registered entry's index, or nullopt if the name was already
 * registered (logged at ERROR).
 */
std::optional<size_t> RegisterFlag(FlagEntry entry);

/**
 * Removes a flag from the registry. Used by `FlagRegistrar`'s destructor so
 * that DLL unload tears down the lambdas captured in each FlagEntry.
 */
void UnregisterFlag(std::string_view name);

bool SetFlagByName(std::string_view name, std::string_view value);
std::string GetFlagByName(std::string_view name);

// Invoke a registered command by name, passing the raw argument text.
// Returns false if `name` is not registered or is not a FlagType::Command.
bool InvokeCommand(std::string_view name, std::string_view args);

// Typed registry query. Cross-DLL access path that does not require linking
// the DLL where the cvar is defined. Slower than REXCVAR_GET (string parse +
// hash lookup), so prefer REXCVAR_GET when the defining DLL is already on the
// link line. Returns a value-initialized T when the cvar is missing or its
// stored string fails to parse.
template <typename T>
T Query(std::string_view name);

template <>
bool Query<bool>(std::string_view name);
template <>
int32_t Query<int32_t>(std::string_view name);
template <>
int64_t Query<int64_t>(std::string_view name);
template <>
uint32_t Query<uint32_t>(std::string_view name);
template <>
uint64_t Query<uint64_t>(std::string_view name);
template <>
double Query<double>(std::string_view name);
template <>
std::string Query<std::string>(std::string_view name);

std::vector<std::string> ListFlags();
std::vector<std::string> ListFlagsByCategory(std::string_view category);
std::vector<std::string> ListFlagsByLifecycle(Lifecycle lc);
const FlagEntry* GetFlagInfo(std::string_view name);
std::vector<std::string> GetPendingRestartFlags();
void ClearPendingRestartFlags();
void ResetToDefault(std::string_view name);
void ResetAllToDefaults();
bool HasNonDefaultValue(std::string_view name);
std::vector<std::string> ListModifiedFlags();
std::string SerializeToTOML();
std::string SerializeToTOML(std::string_view category);

/// Callback invoked when a CVAR value changes
/// @param name The CVAR name
/// @param new_value The new value as a string
using ChangeCallback = std::function<void(std::string_view name, std::string_view new_value)>;

/// Register a callback to be invoked when a specific CVAR changes
void RegisterChangeCallback(std::string_view name, ChangeCallback callback);

/// Unregister all callbacks for a specific CVAR
void UnregisterChangeCallbacks(std::string_view name);

/**
 * RAII handle for a registered flag. Destructor unregisters by name; on
 * duplicate-name registration `owned_name_` is empty so chain methods and
 * the destructor become no-ops and the original owner's entry is untouched.
 */
struct FlagRegistrar {
  std::string owned_name_;  // empty when registration was rejected

  explicit FlagRegistrar(FlagEntry e) {
    std::string name = e.name;
    if (RegisterFlag(std::move(e)).has_value()) {
      owned_name_ = std::move(name);
    }
  }

  FlagRegistrar(FlagRegistrar&& other) noexcept : owned_name_(std::move(other.owned_name_)) {
    other.owned_name_.clear();
  }

  ~FlagRegistrar() {
    if (!owned_name_.empty()) {
      UnregisterFlag(owned_name_);
    }
  }

  // Chain methods mutate the registered entry by name lookup.
  FlagRegistrar&& range(double min_val, double max_val) && {
    apply_([=](FlagEntry& entry) {
      entry.constraints.min = min_val;
      entry.constraints.max = max_val;
    });
    return std::move(*this);
  }

  FlagRegistrar&& allowed(std::initializer_list<std::string> values) && {
    std::vector<std::string> vals(values);
    apply_([vals = std::move(vals)](FlagEntry& entry) { entry.constraints.allowed_values = vals; });
    return std::move(*this);
  }

  FlagRegistrar&& lifecycle(Lifecycle lc) && {
    apply_([=](FlagEntry& entry) { entry.lifecycle = lc; });
    return std::move(*this);
  }

  FlagRegistrar&& debug_only() && {
    apply_([](FlagEntry& entry) { entry.is_debug_only = true; });
    return std::move(*this);
  }

  FlagRegistrar&& validator(std::function<bool(std::string_view)> fn) && {
    apply_([fn = std::move(fn)](FlagEntry& entry) mutable {
      entry.constraints.custom_validator = std::move(fn);
    });
    return std::move(*this);
  }

  // Non-copyable (prevent double registration)
  FlagRegistrar(const FlagRegistrar&) = delete;
  FlagRegistrar& operator=(const FlagRegistrar&) = delete;
  FlagRegistrar& operator=(FlagRegistrar&&) = delete;

 private:
  void apply_(std::function<void(FlagEntry&)> fn);
};

inline bool ParseDouble(std::string_view s, double& out) {
  if (s.empty())
    return false;
  char* end = nullptr;
  std::string str(s);
  out = std::strtod(str.c_str(), &end);
  return end != str.c_str() && *end == '\0';
}

}  // namespace rex::cvar

//=============================================================================
// CVar Macros
//=============================================================================

// Declare a cvar (use in headers and TUs that need to read it).
// The accessor function returns a reference to the cvar's storage. Storage
// lives as a static-local inside whichever DLL contains the matching
// REXCVAR_DEFINE_*. Cross-DLL access goes through the import lib.
#define REXCVAR_DECLARE(type, name) type& FLAGS_##name##_storage_()

// Get a cvar value
#define REXCVAR_GET(name) (FLAGS_##name##_storage_())

// Set a cvar value
#define REXCVAR_SET(name, value) (FLAGS_##name##_storage_() = (value))

// Cross-module typed query that goes through the cvar registry by name.
// Use this when the defining DLL is not on the consumer's link line (e.g.,
// across one-way subsystem dependencies where adding the reverse link would
// create a cycle). Slower than REXCVAR_GET; prefer REXCVAR_GET when possible.
#define REXCVAR_QUERY(type, name) (::rex::cvar::Query<type>(#name))

// Define cvars (use in one .cpp file per cvar)
// The FlagRegistrar registers the flag in its destructor, allowing method chaining.
#define REXCVAR_DEFINE_BOOL(name, default_val, category, desc)                                   \
  bool& FLAGS_##name##_storage_() {                                                              \
    static bool storage = (default_val);                                                         \
    return storage;                                                                              \
  }                                                                                              \
  static auto _cvar_reg_##name =                                                                 \
      ::rex::cvar::FlagRegistrar({#name,                                                         \
                                  ::rex::cvar::FlagType::Boolean,                                \
                                  category,                                                      \
                                  desc,                                                          \
                                  [](std::string_view v) {                                       \
                                    bool val = (v == "true" || v == "1" || v == "yes");          \
                                    FLAGS_##name##_storage_() = val;                             \
                                    return true;                                                 \
                                  },                                                             \
                                  []() { return FLAGS_##name##_storage_() ? "true" : "false"; }, \
                                  [](std::string_view) {},                                       \
                                  ::rex::cvar::Lifecycle::kHotReload,                            \
                                  {},                                                            \
                                  (default_val) ? "true" : "false",                              \
                                  false})

#define REXCVAR_DEFINE_INT32(name, default_val, category, desc)                               \
  int32_t& FLAGS_##name##_storage_() {                                                        \
    static int32_t storage = (default_val);                                                   \
    return storage;                                                                           \
  }                                                                                           \
  static auto _cvar_reg_##name =                                                              \
      ::rex::cvar::FlagRegistrar({#name,                                                      \
                                  ::rex::cvar::FlagType::Int32,                               \
                                  category,                                                   \
                                  desc,                                                       \
                                  [](std::string_view v) {                                    \
                                    int32_t val = 0;                                          \
                                    auto [ptr, ec] =                                          \
                                        std::from_chars(v.data(), v.data() + v.size(), val);  \
                                    if (ec != std::errc())                                    \
                                      return false;                                           \
                                    FLAGS_##name##_storage_() = val;                          \
                                    return true;                                              \
                                  },                                                          \
                                  []() { return std::to_string(FLAGS_##name##_storage_()); }, \
                                  [](std::string_view) {},                                    \
                                  ::rex::cvar::Lifecycle::kHotReload,                         \
                                  {},                                                         \
                                  std::to_string(default_val),                                \
                                  false})

#define REXCVAR_DEFINE_INT64(name, default_val, category, desc)                               \
  int64_t& FLAGS_##name##_storage_() {                                                        \
    static int64_t storage = (default_val);                                                   \
    return storage;                                                                           \
  }                                                                                           \
  static auto _cvar_reg_##name =                                                              \
      ::rex::cvar::FlagRegistrar({#name,                                                      \
                                  ::rex::cvar::FlagType::Int64,                               \
                                  category,                                                   \
                                  desc,                                                       \
                                  [](std::string_view v) {                                    \
                                    int64_t val = 0;                                          \
                                    auto [ptr, ec] =                                          \
                                        std::from_chars(v.data(), v.data() + v.size(), val);  \
                                    if (ec != std::errc())                                    \
                                      return false;                                           \
                                    FLAGS_##name##_storage_() = val;                          \
                                    return true;                                              \
                                  },                                                          \
                                  []() { return std::to_string(FLAGS_##name##_storage_()); }, \
                                  [](std::string_view) {},                                    \
                                  ::rex::cvar::Lifecycle::kHotReload,                         \
                                  {},                                                         \
                                  std::to_string(default_val),                                \
                                  false})

#define REXCVAR_DEFINE_UINT32(name, default_val, category, desc)                              \
  uint32_t& FLAGS_##name##_storage_() {                                                       \
    static uint32_t storage = (default_val);                                                  \
    return storage;                                                                           \
  }                                                                                           \
  static auto _cvar_reg_##name =                                                              \
      ::rex::cvar::FlagRegistrar({#name,                                                      \
                                  ::rex::cvar::FlagType::Uint32,                              \
                                  category,                                                   \
                                  desc,                                                       \
                                  [](std::string_view v) {                                    \
                                    uint32_t val = 0;                                         \
                                    auto [ptr, ec] =                                          \
                                        std::from_chars(v.data(), v.data() + v.size(), val);  \
                                    if (ec != std::errc())                                    \
                                      return false;                                           \
                                    FLAGS_##name##_storage_() = val;                          \
                                    return true;                                              \
                                  },                                                          \
                                  []() { return std::to_string(FLAGS_##name##_storage_()); }, \
                                  [](std::string_view) {},                                    \
                                  ::rex::cvar::Lifecycle::kHotReload,                         \
                                  {},                                                         \
                                  std::to_string(default_val),                                \
                                  false})

#define REXCVAR_DEFINE_UINT64(name, default_val, category, desc)                              \
  uint64_t& FLAGS_##name##_storage_() {                                                       \
    static uint64_t storage = (default_val);                                                  \
    return storage;                                                                           \
  }                                                                                           \
  static auto _cvar_reg_##name =                                                              \
      ::rex::cvar::FlagRegistrar({#name,                                                      \
                                  ::rex::cvar::FlagType::Uint64,                              \
                                  category,                                                   \
                                  desc,                                                       \
                                  [](std::string_view v) {                                    \
                                    uint64_t val = 0;                                         \
                                    auto [ptr, ec] =                                          \
                                        std::from_chars(v.data(), v.data() + v.size(), val);  \
                                    if (ec != std::errc())                                    \
                                      return false;                                           \
                                    FLAGS_##name##_storage_() = val;                          \
                                    return true;                                              \
                                  },                                                          \
                                  []() { return std::to_string(FLAGS_##name##_storage_()); }, \
                                  [](std::string_view) {},                                    \
                                  ::rex::cvar::Lifecycle::kHotReload,                         \
                                  {},                                                         \
                                  std::to_string(default_val),                                \
                                  false})

#define REXCVAR_DEFINE_DOUBLE(name, default_val, category, desc)                              \
  double& FLAGS_##name##_storage_() {                                                         \
    static double storage = (default_val);                                                    \
    return storage;                                                                           \
  }                                                                                           \
  static auto _cvar_reg_##name =                                                              \
      ::rex::cvar::FlagRegistrar({#name,                                                      \
                                  ::rex::cvar::FlagType::Double,                              \
                                  category,                                                   \
                                  desc,                                                       \
                                  [](std::string_view v) {                                    \
                                    double val = 0;                                           \
                                    if (!::rex::cvar::ParseDouble(v, val))                    \
                                      return false;                                           \
                                    FLAGS_##name##_storage_() = val;                          \
                                    return true;                                              \
                                  },                                                          \
                                  []() { return std::to_string(FLAGS_##name##_storage_()); }, \
                                  [](std::string_view) {},                                    \
                                  ::rex::cvar::Lifecycle::kHotReload,                         \
                                  {},                                                         \
                                  std::to_string(default_val),                                \
                                  false})

#define REXCVAR_DEFINE_STRING(name, default_val, category, desc)                \
  std::string& FLAGS_##name##_storage_() {                                      \
    static std::string storage = (default_val);                                 \
    return storage;                                                             \
  }                                                                             \
  static auto _cvar_reg_##name =                                                \
      ::rex::cvar::FlagRegistrar({#name,                                        \
                                  ::rex::cvar::FlagType::String,                \
                                  category,                                     \
                                  desc,                                         \
                                  [](std::string_view v) {                      \
                                    FLAGS_##name##_storage_() = std::string(v); \
                                    return true;                                \
                                  },                                            \
                                  []() { return FLAGS_##name##_storage_(); },   \
                                  [](std::string_view) {},                      \
                                  ::rex::cvar::Lifecycle::kHotReload,           \
                                  {},                                           \
                                  default_val,                                  \
                                  false})

#define REXCVAR_DEFINE_COMMAND(name, callback, category, desc)                           \
  std::function<void()>& FLAGS_##name##_storage_() {                                     \
    static std::function<void()> storage = (callback);                                   \
    return storage;                                                                      \
  }                                                                                      \
  static auto _cvar_reg_##name =                                                         \
      ::rex::cvar::FlagRegistrar({#name,                                                 \
                                  ::rex::cvar::FlagType::Command,                        \
                                  category,                                              \
                                  desc,                                                  \
                                  [](std::string_view) { return false; },                \
                                  []() { return "<command>"; },                          \
                                  [](std::string_view) { FLAGS_##name##_storage_()(); }, \
                                  ::rex::cvar::Lifecycle::kHotReload,                    \
                                  {},                                                    \
                                  "<command>",                                           \
                                  false})

// Define an argument-taking command. `callback` is convertible to
// std::function<void(std::string_view args)>; the console passes the text
// after the command name as `args`.
#define REXCVAR_DEFINE_COMMAND_ARGS(name, callback, category, desc)                         \
  std::function<void(std::string_view)>& FLAGS_##name##_storage_() {                        \
    static std::function<void(std::string_view)> storage = (callback);                      \
    return storage;                                                                         \
  }                                                                                         \
  static auto _cvar_reg_##name =                                                            \
      ::rex::cvar::FlagRegistrar({#name,                                                    \
                                  ::rex::cvar::FlagType::Command,                           \
                                  category,                                                 \
                                  desc,                                                     \
                                  [](std::string_view) { return false; },                   \
                                  []() { return "<command>"; },                             \
                                  [](std::string_view a) { FLAGS_##name##_storage_()(a); }, \
                                  ::rex::cvar::Lifecycle::kHotReload,                       \
                                  {},                                                       \
                                  "<command>",                                              \
                                  false})

namespace rex::cvar {
namespace testing {

class ScopedLifecycleOverride {
 public:
  ScopedLifecycleOverride();
  ~ScopedLifecycleOverride();

  ScopedLifecycleOverride(const ScopedLifecycleOverride&) = delete;
  ScopedLifecycleOverride& operator=(const ScopedLifecycleOverride&) = delete;
};

void ResetAllForTesting();

}  // namespace testing
}  // namespace rex::cvar
