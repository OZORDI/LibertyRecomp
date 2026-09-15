#include <array>
#include <chrono>
#include <filesystem>
#include <thread>

#include <catch2/catch_test_macros.hpp>
#include <rex/cvar.h>
#include <rex/input/sony_feedback.h>

namespace {
constexpr std::array flags = {"gta4_sony_enabled", "gta4_sony_gestures",
    "gta4_sony_lighting", "gta4_sony_rumble", "gta4_sony_triggers"};
struct Reset {
  ~Reset() {
    for (const char* flag : flags) rex::cvar::ResetToDefault(flag);
    rex::cvar::ResetToDefault("gta4_sony_intensity");
  }
};
}

TEST_CASE("Sony settings persist independently through the real TOML registry", "[input][sony][config]") {
  Reset reset;
  const auto directory = std::filesystem::temp_directory_path() /
      ("liberty-sony-options-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
  REQUIRE(std::filesystem::create_directory(directory));
  struct Cleanup {
    std::filesystem::path path;
    ~Cleanup() { std::error_code ec; std::filesystem::remove_all(path, ec); }
  } cleanup{directory};
  for (const char* flag : flags) REQUIRE(rex::cvar::SetFlagByName(flag, "false"));
  REQUIRE(rex::cvar::SetFlagByName("gta4_sony_intensity", "0.25"));
  REQUIRE(rex::cvar::SaveConfig(directory / "settings.toml"));
  for (const char* flag : flags) REQUIRE(rex::cvar::SetFlagByName(flag, "true"));
  REQUIRE(rex::cvar::SetFlagByName("gta4_sony_intensity", "1"));
  rex::cvar::LoadConfig(directory / "settings.toml");
  const auto options = rex::input::sony::GetOptions();
  CHECK_FALSE(options.enabled);
  CHECK_FALSE(options.gestures);
  CHECK_FALSE(options.lighting);
  CHECK_FALSE(options.rumble);
  CHECK_FALSE(options.triggers);
  CHECK(options.intensity == 0.25f);
  REQUIRE(rex::cvar::SetFlagByName("gta4_sony_gestures", "true"));
  CHECK(rex::input::sony::GetOptions().gestures);
  CHECK_FALSE(rex::input::sony::GetOptions().rumble);
}

TEST_CASE("Sony option snapshots tolerate concurrent frontend writes", "[input][sony][config]") {
  Reset reset;
  std::thread reader([] {
    for (unsigned i = 0; i < 10000; ++i) (void)rex::input::sony::GetOptions();
  });
  for (unsigned i = 0; i < 200; ++i) {
    rex::cvar::SetFlagByName("gta4_sony_enabled", "false");
    rex::cvar::SetFlagByName("gta4_sony_enabled", "true");
  }
  reader.join();
  CHECK(rex::input::sony::GetOptions().enabled);
}

TEST_CASE("DualSense menu toggle enables the whole bundle and persists it", "[input][sony][config]") {
  Reset reset;
  const auto directory = std::filesystem::temp_directory_path() /
      ("liberty-dualsense-menu-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
  REQUIRE(std::filesystem::create_directory(directory));
  struct Cleanup {
    std::filesystem::path path;
    ~Cleanup() { std::error_code ec; std::filesystem::remove_all(path, ec); }
  } cleanup{directory};

  // Simulate an older config with the master on but individual features off.
  for (const char* flag : flags) REQUIRE(rex::cvar::SetFlagByName(flag, "false"));
  REQUIRE(rex::cvar::SetFlagByName("gta4_sony_enabled", "true"));
  REQUIRE(rex::cvar::SetFlagByName("gta4_sony_intensity", "0.25"));
  REQUIRE(rex::input::sony::SetFeaturesEnabled(true));
  const auto enabled = rex::input::sony::GetOptions();
  CHECK(enabled.enabled);
  CHECK(enabled.gestures);
  CHECK(enabled.lighting);
  CHECK(enabled.rumble);
  CHECK(enabled.triggers);
  CHECK(enabled.intensity == 0.25f);
  REQUIRE(rex::cvar::SaveConfig(directory / "enabled.toml"));

  REQUIRE(rex::input::sony::SetFeaturesEnabled(false));
  const auto disabled = rex::input::sony::GetOptions();
  CHECK_FALSE(disabled.enabled);
  CHECK_FALSE(disabled.gestures);
  CHECK_FALSE(disabled.lighting);
  CHECK_FALSE(disabled.rumble);
  CHECK_FALSE(disabled.triggers);
  REQUIRE(rex::cvar::SaveConfig(directory / "disabled.toml"));

  // Configs store non-default overrides; a fresh app starts with defaults
  // before loading them. Loading alone merges into the current registry.
  for (const char* flag : flags) rex::cvar::ResetToDefault(flag);
  rex::cvar::LoadConfig(directory / "enabled.toml");
  for (const char* flag : flags) CHECK(rex::cvar::Query<bool>(flag));
  for (const char* flag : flags) rex::cvar::ResetToDefault(flag);
  rex::cvar::LoadConfig(directory / "disabled.toml");
  for (const char* flag : flags) CHECK_FALSE(rex::cvar::Query<bool>(flag));
}
