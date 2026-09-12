#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <unordered_set>
#include "graphics/gta4_native/native_texture_eviction_index.h"

using rex::graphics::gta4_native::NativeTextureEvictionIndex;

TEST_CASE("texture eviction index removes dead generations immediately", "[native-memory-fix]") {
  NativeTextureEvictionIndex index;
  REQUIRE_FALSE(index.Insert(0));
  REQUIRE_FALSE(index.Next());
  REQUIRE_FALSE(index.Erase(20));
  for (uint64_t i = 1; i <= 5000; ++i) REQUIRE(index.Insert(i));
  REQUIRE_FALSE(index.Insert(1));
  for (uint64_t i = 1; i <= 5000; ++i) REQUIRE(index.Next() == i);
  for (uint64_t i = 1; i <= 5000; i += 2) REQUIRE(index.Erase(i));
  REQUIRE(index.size() == 2500);
  for (uint64_t i = 2; i <= 5000; i += 2) REQUIRE(index.Next() == i);
  for (uint64_t i = 2; i <= 5000; i += 2) REQUIRE(index.Erase(i));
  REQUIRE(index.empty());
  REQUIRE_FALSE(index.Next());
}

TEST_CASE("streaming generations do not accumulate tombstones in the eviction scan", "[native-memory-fix]") {
  NativeTextureEvictionIndex index;
  uint64_t sequence = 1;
  for (uint32_t frame = 0; frame < 1000; ++frame) {
    std::unordered_set<uint64_t> frame_generations;
    for (uint32_t i = 0; i < 320; ++i) {
      REQUIRE(index.Insert(sequence));
      frame_generations.insert(sequence++);
    }
    // Visit less than the production rate, reproducing the old deque backlog.
    for (uint32_t visit = 0; visit < 128; ++visit) {
      const auto generation = index.Next();
      REQUIRE(generation);
      REQUIRE(frame_generations.contains(*generation));
    }
    for (uint64_t generation : frame_generations) REQUIRE(index.Erase(generation));
    REQUIRE(index.empty());
  }
  REQUIRE(index.Insert(5));
  index.clear();
  REQUIRE(index.empty());
  REQUIRE(index.Insert(5));
  REQUIRE(index.Next() == 5);
}
