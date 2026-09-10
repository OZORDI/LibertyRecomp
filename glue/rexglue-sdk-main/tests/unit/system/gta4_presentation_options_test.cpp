#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <filesystem>
#include <limits>
#include <thread>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <rex/cvar.h>

#include "../../../gta4-recomp/src/gta4_presentation_options.h"
#include "../../../gta4-recomp/src/gta4_presentation_policy.h"

namespace p = gta4::presentation::policy;
namespace options = gta4::presentation;
namespace {
std::vector<uint8_t> Table(std::initializer_list<uint32_t> markers = {1, 2, 0, 3, 0, 4, 0}) {
  std::vector<uint8_t> data(markers.size() * p::kScreenStride, 0xA5);
  std::size_t index = 0;
  for (uint32_t marker : markers) {
    const auto offset = index++ * p::kScreenStride;
    p::StoreBe32(data, offset, 6000);
    p::StoreBe32(data, offset + 4, marker == 1 ? 0 : 1);
    p::StoreBe32(data, offset + 8, marker);
    p::StoreBe32(data, offset + 12, marker == 1 ? 0 : 1);
  }
  return data;
}
struct ResetOptions {
  ResetOptions() {
    rex::cvar::SetFlagByName("gta4_skip_intro", "false");
    rex::cvar::SetFlagByName("gta4_disable_tlad_film_grain", "false");
    rex::cvar::SetFlagByName("gta4_trace_presentation_options", "false");
    options::InitializeOptions();
  }
  ~ResetOptions() {
    rex::cvar::SetFlagByName("gta4_skip_intro", "false");
    rex::cvar::SetFlagByName("gta4_disable_tlad_film_grain", "false");
    rex::cvar::SetFlagByName("gta4_trace_presentation_options", "false");
    options::InitializeOptions();
  }
};
}  // namespace

TEST_CASE("Presentation cold-start caller and arguments are exact", "[presentation][startup]") {
  CHECK(p::IsColdStart(0x82140048, 1, 0));
  for (uint32_t caller : {0u, 0x82142140u, 0x8214C398u, 0x8214C464u})
    CHECK_FALSE(p::IsColdStart(caller, 1, 0));
  CHECK_FALSE(p::IsColdStart(0x82140048, 0, 0));
  CHECK_FALSE(p::IsColdStart(0x82140048, 1, 1));
  CHECK_FALSE(p::IsColdStart(0x82140048, 257, 0));
}

TEST_CASE("Intro transformation only removes recognized presentation fields",
          "[presentation][startup]") {
  auto data = Table();
  const auto before = data;
  const auto plan = p::CollapseIntro(data, 7, true);
  REQUIRE(plan.status == p::IntroStatus::kReady);
  CHECK(plan.prefix_count == 5);
  CHECK(plan.end_intro_index == 3);
  for (std::size_t i = 0; i < data.size(); ++i) {
    const auto row = i / p::kScreenStride;
    const auto field = i % p::kScreenStride;
    const bool changed = row < plan.prefix_count && (field < 8 || (field >= 12 && field < 16));
    if (changed)
      CHECK(data[i] == 0);
    else
      CHECK(data[i] == before[i]);
  }
  CHECK(p::LoadBe32(data, plan.end_intro_index * p::kScreenStride + 8) == 3);
  CHECK(p::LoadBe32(data, plan.prefix_count * p::kScreenStride) == 6000);
  const auto repeated = data;
  CHECK(p::CollapseIntro(data, 7, true).status == p::IntroStatus::kReady);
  CHECK(data == repeated);
}

TEST_CASE("Disabled intro option preserves every byte including unfamiliar assets",
          "[presentation][startup]") {
  auto data = Table({99, 98, 97});
  const auto before = data;
  CHECK(p::CollapseIntro(data, 3, false).status == p::IntroStatus::kDisabled);
  CHECK(data == before);
}

TEST_CASE("Intro validation rejects inconsistent metadata without partial writes",
          "[presentation][startup]") {
  for (auto markers : {std::initializer_list<uint32_t>{1, 2, 0, 3},
                       {1, 3, 2, 4},
                       {0, 2, 3, 4},
                       {1, 2, 1, 3, 4},
                       {1, 2, 2, 3, 4},
                       {1, 2, 3, 3, 4},
                       {1, 2, 99, 3, 4},
                       {1, 2, 4, 3}}) {
    auto data = Table(markers);
    const auto before = data;
    CHECK(p::CollapseIntro(data, static_cast<uint32_t>(markers.size()), true).status ==
          p::IntroStatus::kUnsupported);
    CHECK(data == before);
  }
  for (auto [field, value] :
       std::array<std::pair<uint32_t, uint32_t>, 4>{{{0, 0xFFFFFFFF}, {4, 5}, {8, 6}, {12, 4}}}) {
    auto data = Table();
    // A bad later record must reject the entire operation, not just that row.
    p::StoreBe32(data, 6 * p::kScreenStride + field, value);
    const auto before = data;
    CHECK(p::CollapseIntro(data, 7, true).status == p::IntroStatus::kUnsupported);
    CHECK(data == before);
  }
}

TEST_CASE("Intro validation bounds counts and buffer lengths before reads",
          "[presentation][startup]") {
  auto data = Table();
  for (uint32_t count : {0u, 1u, 6u, 8u, 15u, std::numeric_limits<uint32_t>::max()}) {
    const auto before = data;
    CHECK(p::CollapseIntro(data, count, true).status == p::IntroStatus::kUnsupported);
    CHECK(data == before);
  }
  CHECK(p::CollapseIntro({}, 7, true).status == p::IntroStatus::kUnsupported);
  data.pop_back();
  const auto before = data;
  CHECK(p::CollapseIntro(data, 7, true).status == p::IntroStatus::kUnsupported);
  CHECK(data == before);
}

TEST_CASE("Intro keeps audio readiness markers and completion semantics",
          "[presentation][startup]") {
  auto data = Table({1, 5, 2, 0, 3, 0, 4});
  const auto plan = p::CollapseIntro(data, 7, true);
  REQUIRE(plan.status == p::IntroStatus::kReady);
  CHECK(plan.prefix_count == 6);
  CHECK(p::LoadBe32(data, p::kScreenStride + 8) == 5);
  CHECK(p::LoadBe32(data, 4 * p::kScreenStride + 8) == 3);
  CHECK(p::LoadBe32(data, 6 * p::kScreenStride + 8) == 4);
}

TEST_CASE("TLAD remaps only four final-composite grain passes", "[presentation][grain]") {
  for (uint32_t pass = 0; pass < 64; ++pass) {
    for (uint32_t episode : {0u, 1u, 2u, 3u, 0xFFFFFFFFu}) {
      for (bool enabled : {false, true}) {
        for (bool valid : {false, true}) {
          for (uint32_t caller : {p::kCompositeCaller, 0u, 0x822CF9D0u}) {
            uint32_t expected = pass;
            if (episode == 1 && enabled && valid && caller == p::kCompositeCaller) {
              switch (pass) {
                case 24:
                  expected = 10;
                  break;
                case 25:
                  expected = 11;
                  break;
                case 26:
                  expected = 12;
                  break;
                case 27:
                  expected = 13;
                  break;
              }
            }
            CHECK(p::SelectCompositePass(pass, enabled, episode, caller, valid) == expected);
          }
        }
      }
    }
  }
  CHECK(p::SelectCompositePass(0xFFFFFFFFu, true, 1, p::kCompositeCaller, true) == 0xFFFFFFFFu);
}

TEST_CASE("Presentation flags have explicit default and lifecycle contracts",
          "[presentation][config]") {
  const ResetOptions guard;
  const auto* skip = rex::cvar::GetFlagInfo("gta4_skip_intro");
  const auto* grain = rex::cvar::GetFlagInfo("gta4_disable_tlad_film_grain");
  REQUIRE(skip);
  REQUIRE(grain);
  CHECK(skip->lifecycle == rex::cvar::Lifecycle::kRequiresRestart);
  CHECK(grain->lifecycle == rex::cvar::Lifecycle::kHotReload);
  CHECK_FALSE(options::SkipIntroAtLaunch());
  CHECK_FALSE(options::DisableTladFilmGrain());
  REQUIRE(rex::cvar::SetFlagByName("gta4_skip_intro", "true"));
  CHECK_FALSE(options::SkipIntroAtLaunch());
  options::InitializeOptions();
  CHECK(options::SkipIntroAtLaunch());
}

TEST_CASE("Grain and diagnostic option snapshots update live", "[presentation][config]") {
  const ResetOptions guard;
  for (const char* value : {"true", "1", "yes"}) {
    REQUIRE(rex::cvar::SetFlagByName("gta4_disable_tlad_film_grain", value));
    CHECK(options::DisableTladFilmGrain());
    REQUIRE(rex::cvar::SetFlagByName("gta4_disable_tlad_film_grain", "false"));
    CHECK_FALSE(options::DisableTladFilmGrain());
  }
  REQUIRE(rex::cvar::SetFlagByName("gta4_trace_presentation_options", "true"));
  CHECK(options::TraceEnabled());
  REQUIRE(rex::cvar::SetFlagByName("gta4_trace_presentation_options", "false"));
  CHECK_FALSE(options::TraceEnabled());
}

TEST_CASE("Presentation options save and reload through the normal TOML system",
          "[presentation][config]") {
  const ResetOptions guard;
  const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
  const auto directory = std::filesystem::temp_directory_path() /
                         ("liberty-presentation-test-" + std::to_string(stamp));
  REQUIRE(std::filesystem::create_directory(directory));
  struct Cleanup {
    std::filesystem::path p;
    ~Cleanup() {
      std::error_code ec;
      std::filesystem::remove_all(p, ec);
    }
  } cleanup{directory};
  const auto config = directory / "native.toml";
  REQUIRE(rex::cvar::SetFlagByName("gta4_skip_intro", "true"));
  REQUIRE(rex::cvar::SetFlagByName("gta4_disable_tlad_film_grain", "true"));
  REQUIRE(rex::cvar::SaveConfig(config));
  REQUIRE(rex::cvar::SetFlagByName("gta4_skip_intro", "false"));
  REQUIRE(rex::cvar::SetFlagByName("gta4_disable_tlad_film_grain", "false"));
  rex::cvar::LoadConfig(config);
  CHECK(rex::cvar::GetFlagByName("gta4_skip_intro") == "true");
  CHECK(rex::cvar::GetFlagByName("gta4_disable_tlad_film_grain") == "true");
  options::InitializeOptions();
  CHECK(options::SkipIntroAtLaunch());
  CHECK(options::DisableTladFilmGrain());
  CHECK_FALSE(rex::cvar::SaveConfig(directory / "missing" / "native.toml"));
}

TEST_CASE("Grain snapshot reads do not touch mutable cvar storage", "[presentation][config]") {
  const ResetOptions guard;
  std::atomic<bool> done{false};
  std::jthread reader([&] {
    for (unsigned i = 0; i < 10000; ++i)
      (void)options::DisableTladFilmGrain();
    done.store(true);
  });
  for (unsigned i = 0; i < 200; ++i)
    REQUIRE(rex::cvar::SetFlagByName("gta4_disable_tlad_film_grain", (i & 1) ? "false" : "true"));
  reader.join();
  CHECK(done.load());
  CHECK_FALSE(options::DisableTladFilmGrain());
}
