/**
 * @file native_frame_scheduling_test.cpp
 * @brief Texture eviction policy tests for the GTA IV native renderer.
 */

#include <catch2/catch_test_macros.hpp>

#include "graphics/gta4_native/native_frame_scheduling.h"

namespace gta4 = rex::graphics::gta4_native;

TEST_CASE("GTA IV native texture eviction waits out the full grace window") {
  constexpr uint32_t kGrace = gta4::kNativeTextureEvictionGraceFrames;
  const uint32_t last_used = 1000;
  // Referenced this frame or within the grace window: kept.
  REQUIRE_FALSE(gta4::ShouldEvictNativeTexture(last_used, last_used, kGrace));
  REQUIRE_FALSE(
      gta4::ShouldEvictNativeTexture(last_used + kGrace, last_used, kGrace));
  // One frame past the grace window: evicted.
  REQUIRE(
      gta4::ShouldEvictNativeTexture(last_used + kGrace + 1, last_used, kGrace));
  // A frame counter that moved backwards (title reset) must never evict, so a
  // stale stamp can only delay eviction, never destroy a live image.
  REQUIRE_FALSE(gta4::ShouldEvictNativeTexture(last_used - 1, last_used, kGrace));
  REQUIRE_FALSE(gta4::ShouldEvictNativeTexture(0, UINT32_MAX, kGrace));
}
