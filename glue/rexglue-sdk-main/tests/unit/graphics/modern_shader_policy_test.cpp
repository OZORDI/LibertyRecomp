#include <array>
#include <chrono>
#include <filesystem>
#include <thread>
#include <atomic>
#include <catch2/catch_test_macros.hpp>
#include <rex/cvar.h>
#include "graphics/gta4_native/modern_shader_policy.h"
#include "graphics/gta4_native/modern_shader_options.h"
#include "graphics/gta4_native/native_pipeline_lookup_memo.h"
#include "gta4_presentation_options.h"
#include "gta4_presentation_policy.h"

namespace m = rex::graphics::gta4_native;
namespace p = gta4::presentation::policy;
namespace {
constexpr std::array<uint64_t, 18> hashes{
    0xEE75C9F6AA1AB16Aull, 0xE51D9DD95A333D92ull, 0xD3B2B2125BC24911ull, 0x94CC5AF0E2FF5B08ull,
    0x79044EA1461439CAull, 0xC5D3E7806E478A16ull, 0x67C1FB770BB55E69ull, 0xE4AF188ACBDA7362ull,
    0xE74BFC50127AFED3ull, 0x86CB9F8D0850ABE3ull, 0xC3256D6D7C2E426Dull, 0xDF64C22EC010C136ull,
    0x458818340E2283DEull, 0xEFAACEED3DBD802Dull, 0x1A6669BBAFDC43E7ull, 0x156BAD4A9EE62726ull,
    0x9E76B68B60127349ull, 0xA6C9E2B8B2A59D7Aull};
constexpr std::array<uint64_t, 1> vertex_counterpart{0x458818340E2283DEull};
constexpr std::array<uint64_t, 1> pixel_counterpart{0xDF64C22EC010C136ull};
m::ShaderOverrideCandidate Candidate(uint64_t hash) {
  m::ShaderOverrideCandidate c{true, m::ShaderOverrideActivation::kStage, 0, hash, 0, 0, {}};
  if (m::ClassifyModernShader(hash) == m::ModernShaderFamily::kLightVolume) {
    c.activation = m::ShaderOverrideActivation::kPipelinePair;
    c.pipeline_pair_id = 1;
    c.support_radius_bits = 0x3F59999A;
    c.supported_sample_count_mask = 7;
    c.counterpart_hashes = hash == 0xDF64C22EC010C136ull ? vertex_counterpart : pixel_counterpart;
  }
  return c;
}
struct ResetOptions {
  ResetOptions() { gta4::presentation::InitializeOptions(); }
  ~ResetOptions() {
    rex::cvar::SetFlagByName("gta4_modern_shaders", "false");
    rex::cvar::SetFlagByName("gta4_disable_tlad_film_grain", "false");
    rex::cvar::SetFlagByName("gta4_trace_modern_shaders", "false");
  }
};
struct Fixed {
  uint32_t value = 0;
  bool operator==(const Fixed&) const = default;
};
}  // namespace
TEST_CASE("Modern shaders classify every shipped override and preserve video", "[modern-shaders]") {
  for (auto hash : hashes) {
    const auto family = m::ClassifyModernShader(hash);
    REQUIRE(family != m::ModernShaderFamily::kOther);
    for (bool enabled : {false, true})
      for (bool disabled : {false, true}) {
        const bool expected =
            family == m::ModernShaderFamily::kVideo ||
            (enabled && !(disabled && family == m::ModernShaderFamily::kTladGrain));
        CHECK(m::AllowModernShader(hash, {enabled, disabled}) == expected);
      }
  }
  CHECK(m::AllowModernShader(0x12345678, {false, true}));
}
TEST_CASE("Film-grain disable vetoes every modern noise variant in every selection mode",
          "[modern-shaders]") {
  for (auto mode :
       {m::ShaderOverrideMode::kStock, m::ShaderOverrideMode::kPair, m::ShaderOverrideMode::kStage})
    for (auto hash : hashes)
      for (uint32_t samples : {1u, 2u, 4u, 8u})
        for (bool enabled : {false, true}) {
          if (m::ClassifyModernShader(hash) != m::ModernShaderFamily::kTladGrain)
            continue;
          const auto selected =
              m::ResolveModernShaderSelection(mode, {enabled, true}, {}, Candidate(hash), samples);
          CHECK_FALSE(selected.pixel_override);
          CHECK(selected.variant_key == 0);
          const auto allowed =
              m::ResolveModernShaderSelection(mode, {enabled, false}, {}, Candidate(hash), samples);
          CHECK(allowed.pixel_override == (enabled && mode != m::ShaderOverrideMode::kStock));
        }
}
TEST_CASE("Modern light volumes and fog always retain compatible pairs", "[modern-shaders]") {
  for (auto mode : {m::ShaderOverrideMode::kPair, m::ShaderOverrideMode::kStage})
    for (uint32_t samples : {1u, 2u, 4u, 8u})
      for (bool enabled : {false, true}) {
        auto vs = Candidate(0xDF64C22EC010C136ull), ps = Candidate(0x458818340E2283DEull);
        auto selected = m::ResolveModernShaderSelection(mode, {enabled, false}, vs, ps, samples);
        CHECK(selected.vertex_override == (enabled && samples != 8));
        CHECK(selected.pixel_override == selected.vertex_override);
        ps.present = false;
        selected = m::ResolveModernShaderSelection(mode, {enabled, false}, vs, ps, samples);
        CHECK_FALSE(selected.vertex_override);
        CHECK_FALSE(selected.pixel_override);
        ps = Candidate(0x458818340E2283DEull);
        vs.support_radius_bits = 0x3F000000;
        selected = m::ResolveModernShaderSelection(mode, {enabled, false}, vs, ps, samples);
        CHECK_FALSE(selected.vertex_override);
        CHECK_FALSE(selected.pixel_override);
      }
  for (bool enabled : {false, true})
    for (bool vertex_ready : {false, true})
      for (bool pixel_ready : {false, true}) {
        auto vs = Candidate(0xEFAACEED3DBD802Dull), ps = Candidate(0x1A6669BBAFDC43E7ull);
        vs.present = vertex_ready;
        ps.present = pixel_ready;
        const auto selected = m::ResolveModernShaderSelection(m::ShaderOverrideMode::kPair,
                                                              {enabled, false}, vs, ps, 1);
        CHECK(selected.vertex_override == (enabled && vertex_ready && pixel_ready));
        CHECK(selected.pixel_override == selected.vertex_override);
      }
  auto selected = m::ResolveModernShaderSelection(m::ShaderOverrideMode::kPair, {true, false},
                                                  Candidate(0xEFAACEED3DBD802Dull),
                                                  Candidate(0xE74BFC50127AFED3ull), 1);
  CHECK_FALSE(selected.vertex_override);
  CHECK(selected.pixel_override);
}
TEST_CASE("Frame policy never changes across prewarm draw or readback flush", "[modern-shaders]") {
  m::ModernShaderFramePolicy frame;
  for (unsigned repeat = 0; repeat < 1000; ++repeat)
    for (bool enabled : {false, true})
      for (bool grain : {false, true}) {
        const m::ModernShaderSettings requested{enabled, grain};
        frame.Begin(requested);
        CHECK(frame.settings() == requested);
        for (unsigned partial = 0; partial < 3; ++partial) {
          CHECK_FALSE(frame.Begin({!enabled, !grain}));
          CHECK(frame.settings() == requested);
          auto selected =
              m::ResolveModernShaderSelection(m::ShaderOverrideMode::kPair, frame.settings(), {},
                                              Candidate(0x79044EA1461439CAull), 1);
          CHECK(selected.pixel_override == (enabled && !grain));
        }
        frame.EndGuestFrame();
        CHECK_FALSE(frame.active());
      }
  frame.Begin({true, true});
  frame.Reset();
  CHECK(frame.Begin({false, false}));
  CHECK(frame.settings().key() == 0);
}
TEST_CASE(
    "Modern and grain changes invalidate cached prewarm receipts without destroying pipelines",
    "[modern-shaders]") {
  m::NativePipelineLookupMemo<Fixed, 4, uint64_t> memo;
  Fixed fixed;
  int owner = 0;
  m::NativePipelineLookupContext<4> context;
  context.lifetime = 1;
  for (uint32_t state = 0; state < 4; ++state) {
    context.modern_shader_settings = state;
    memo.Store(&owner, fixed, context, 100 + state);
    for (uint32_t next = 0; next < 4; ++next) {
      auto changed = context;
      changed.modern_shader_settings = next;
      CHECK(memo.Find(&owner, fixed, changed) == (state == next ? 100 + state : 0));
    }
  }
}
TEST_CASE("Real frontend CVar writes control guest grain remapping and modern selection together",
          "[modern-shaders][cvar]") {
  ResetOptions reset;
  REQUIRE(rex::cvar::GetFlagInfo("gta4_modern_shaders"));
  CHECK(rex::cvar::GetFlagInfo("gta4_modern_shaders")->lifecycle ==
        rex::cvar::Lifecycle::kHotReload);
  for (bool modern : {false, true})
    for (bool disabled : {false, true}) {
      REQUIRE(rex::cvar::SetFlagByName("gta4_modern_shaders", modern ? "true" : "false"));
      REQUIRE(
          rex::cvar::SetFlagByName("gta4_disable_tlad_film_grain", disabled ? "true" : "false"));
      const auto state = m::ReadModernShaderSettings();
      CHECK(state.enabled == modern);
      CHECK(state.disable_tlad_grain == disabled);
      CHECK(gta4::presentation::DisableTladFilmGrain() == disabled);
      for (uint32_t episode : {0u, 1u, 2u})
        for (uint32_t pass = 0; pass < 32; ++pass) {
          auto selected = p::SelectCompositePass(pass, gta4::presentation::DisableTladFilmGrain(),
                                                 episode, p::kCompositeCaller, true);
          CHECK(selected ==
                ((episode == 1 && disabled && p::IsNoisePass(pass)) ? pass - 14 : pass));
        }
      const auto motion = m::ResolveModernShaderSelection(m::ShaderOverrideMode::kPair, state, {},
                                                          Candidate(0xEE75C9F6AA1AB16Aull), 1);
      CHECK(motion.pixel_override == modern);  // Grain-free motion blur remains available.
      for (auto hash : hashes)
        if (m::ClassifyModernShader(hash) == m::ModernShaderFamily::kTladGrain) {
          const auto grain = m::ResolveModernShaderSelection(m::ShaderOverrideMode::kPair, state,
                                                             {}, Candidate(hash), 1);
          CHECK(grain.pixel_override == (modern && !disabled));
        }
      const auto serialized = rex::cvar::SerializeToTOML();
      // The real serializer omits defaults; a new process restores Off by default.
      CHECK((serialized.find("gta4_modern_shaders = true") != std::string::npos) == modern);
    }
}
TEST_CASE("Registry changes can race frame snapshots without raw Boolean access",
          "[modern-shaders][cvar]") {
  ResetOptions reset;
  std::atomic<bool> done = false;
  std::jthread writer([&] {
    for (unsigned i = 0; i < 3000; ++i) {
      rex::cvar::SetFlagByName("gta4_modern_shaders", i % 2 ? "true" : "false");
      rex::cvar::SetFlagByName("gta4_disable_tlad_film_grain", i % 3 ? "true" : "false");
    }
    done = true;
  });
  for (unsigned i = 0; i < 3000 || !done; ++i) {
    auto state = m::ReadModernShaderSettings();
    const auto selected = m::ResolveModernShaderSelection(m::ShaderOverrideMode::kPair, state, {},
                                                          Candidate(0x79044EA1461439CAull), 1);
    REQUIRE(selected.pixel_override == (state.enabled && !state.disable_tlad_grain));
  }
  writer.join();
}

TEST_CASE("Modern and TLAD settings persist through the actual config serializer",
          "[modern-shaders][cvar]") {
  ResetOptions reset;
  const auto directory =
      std::filesystem::temp_directory_path() /
      ("liberty-modern-cvar-" +
       std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
  REQUIRE(std::filesystem::create_directory(directory));
  struct Cleanup {
    std::filesystem::path path;
    ~Cleanup() {
      std::error_code ec;
      std::filesystem::remove_all(path, ec);
    }
  } cleanup{directory};
  for (bool modern : {false, true})
    for (bool grain : {false, true}) {
      REQUIRE(rex::cvar::SetFlagByName("gta4_modern_shaders", modern ? "true" : "false"));
      REQUIRE(rex::cvar::SetFlagByName("gta4_disable_tlad_film_grain", grain ? "true" : "false"));
      REQUIRE(rex::cvar::SaveConfig(directory / "native.toml"));
      rex::cvar::ResetToDefault("gta4_modern_shaders");
      rex::cvar::ResetToDefault("gta4_disable_tlad_film_grain");
      rex::cvar::LoadConfig(directory / "native.toml");
      const auto loaded = m::ReadModernShaderSettings();
      CHECK(loaded.enabled == modern);
      CHECK(loaded.disable_tlad_grain == grain);
      gta4::presentation::InitializeOptions();
      CHECK(gta4::presentation::DisableTladFilmGrain() == grain);
    }
}
