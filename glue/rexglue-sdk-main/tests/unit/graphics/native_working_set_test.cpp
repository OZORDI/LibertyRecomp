#include <catch2/catch_test_macros.hpp>
#include <array>
#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <unordered_set>
#include <vector>
#include "graphics/gta4_native/native_working_set.h"
#include "graphics/gta4_native/native_image_reuse.h"
#include "graphics/gta4_native/native_postfx_plan.h"
#include "graphics/gta4_native/native_owner_retirement.h"
#include "../../../gta4-recomp/src/gta4_present_mode_policy.h"

namespace n = rex::graphics::gta4_native;
TEST_CASE("indexed working sets grow across pages without lifetime-history exhaustion",
          "[renderer-performance]") {
  n::NativeDescriptorWorkingSet<21> frames[2];
  for (uint64_t frame = 0; frame < 1200; ++frame) {
    auto& planner = frames[frame % 2];
    REQUIRE(planner.Begin(1211, 64, 32, frame));
    for (uint64_t draw = 0; draw < 320; ++draw) {
      std::array<uint64_t, 21> images{}, samplers{};
      for (size_t stage = 0; stage < images.size(); ++stage) {
        images[stage] = stage < 9 ? 1 + frame * 10000 + ((draw * 7 + stage) % 2600) : 0;
        samplers[stage] = stage < 9 ? 1 + stage % 5 : 0;
      }
      auto a = planner.Assign(images, samplers);
      REQUIRE(a);
      const auto& page = planner.page(a->page);
      for (size_t stage = 0; stage < images.size(); ++stage) {
        REQUIRE(page.images.at(a->images[stage]) == images[stage]);
        REQUIRE(page.samplers.at(a->samplers[stage]) == samplers[stage]);
      }
      REQUIRE(page.images.size() <= 1211);
      REQUIRE(page.samplers.size() <= 64);
    }
    REQUIRE(planner.page_count() <= 3);
    REQUIRE(planner.MarkSubmitted(frame + 1));
    REQUIRE_FALSE(planner.Begin(1211, 64, 32, frame));
  }
}
TEST_CASE("descriptor assignments are atomic with respect to capacity and completion",
          "[renderer-performance]") {
  n::NativeDescriptorWorkingSet<4> p;
  REQUIRE_FALSE(p.MarkSubmitted(1));
  REQUIRE_FALSE(p.Begin(0, 8, 2));
  REQUIRE_FALSE(p.Begin(8, 0, 2));
  REQUIRE_FALSE(p.Begin(8, 8, 0));
  REQUIRE(p.Begin(3, 3, 2));
  auto a = p.Assign({7, 7, 0, 8}, {9, 0, 9, 0});
  REQUIRE(a);
  REQUIRE(a->images[0] == a->images[1]);
  REQUIRE(a->images[2] == 0);
  auto b = p.Assign({10, 11, 0, 0}, {9, 0, 0, 0});
  REQUIRE(b);
  REQUIRE(b->page == 1);
  REQUIRE_FALSE(p.Assign({12, 13, 0, 0}, {9, 0, 0, 0}));
  REQUIRE(p.page(0).images.at(a->images[0]) == 7);
  REQUIRE(p.page(1).images.at(b->images[0]) == 10);
  REQUIRE(p.MarkSubmitted(7));
  REQUIRE_FALSE(p.MarkSubmitted(8));
  REQUIRE_FALSE(p.Begin(3, 3, 2, 6));
  REQUIRE(p.Begin(3, 3, 2, 7));
  REQUIRE_FALSE(p.MarkSubmitted(7));
  REQUIRE(p.MarkSubmitted(8));
}
TEST_CASE("retirement budgets bound batches and permit progress on large allocations",
          "[renderer-performance]") {
  n::NativeWorkBudget no_items(0, 100);
  REQUIRE_FALSE(no_items.Consume(1));
  n::NativeWorkBudget b(64, 33554432);
  for (unsigned i = 0; i < 64; ++i)
    REQUIRE(b.Consume(1024));
  REQUIRE_FALSE(b.Consume(1));
  REQUIRE(b.items() == 64);
  n::NativeWorkBudget large(64, 1024);
  REQUIRE(large.Consume(65536));
  REQUIRE_FALSE(large.Consume(1));
  n::NativeWorkBudget exact(8, 16);
  REQUIRE(exact.Consume(8));
  REQUIRE(exact.Consume(8));
  REQUIRE_FALSE(exact.Consume(1));
  n::NativeWorkBudget overflow(8, UINT64_MAX);
  REQUIRE(overflow.Consume(UINT64_MAX));
  REQUIRE_FALSE(overflow.Consume(1));
}
TEST_CASE("reusable allocations require exact shape ownership and bounded byte accounting",
          "[renderer-performance]") {
  n::NativeImageReusePool<uint64_t> pool(1024, 4);
  n::NativeImageAllocationKey key{{1, 2, 3, 640, 360, 1, 1, 1, 1, 1, 4}};
  REQUIRE_FALSE(pool.Retain(key, 100, 512, 8, 7, 10));
  REQUIRE_FALSE(pool.Retain(key, 100, 2048, 8, 8, 10));
  REQUIRE(pool.Retain(key, 100, 512, 8, 8, 10));
  REQUIRE(pool.bytes() == 512);
  for (size_t field = 0; field < key.fields.size(); ++field) {
    auto changed = key;
    ++changed.fields[field];
    REQUIRE_FALSE(pool.Take(changed));
  }
  auto got = pool.Take(key);
  REQUIRE(got);
  REQUIRE(got->allocation == 100);
  REQUIRE(pool.bytes() == 0);
  REQUIRE_FALSE(pool.Take(key));
  for (uint64_t i = 0; i < 4; ++i)
    REQUIRE(pool.Retain(key, i, 128, 8, 8, 10));
  REQUIRE_FALSE(pool.Retain(key, 5, 128, 8, 8, 10));
  std::vector<uint64_t> released;
  pool.Trim(11, 2, 4, [&](auto x) { released.push_back(x); });
  REQUIRE(released.empty());
  pool.Trim(12, 2, 2, [&](auto x) { released.push_back(x); });
  REQUIRE(released.size() == 2);
  REQUIRE(pool.bytes() == 256);
  pool.Trim(0, 0, SIZE_MAX, [&](auto x) { released.push_back(x); });
  REQUIRE(released.size() == 4);
  REQUIRE(pool.bytes() == 0);
}
TEST_CASE("bounded owner retirement releases each expired resource once after its GPU serial",
          "[renderer-performance]") {
  struct Entry {
    uint64_t id, submission;
  };
  using Q = n::NativeOwnerRetirementQueue<Entry>;
  std::vector<Entry> entries;
  entries.reserve(10000);
  auto queue = std::make_shared<Q>();
  {
    n::NativeOwnerRetirementWatch<Entry> owner;
    for (uint64_t i = 0; i < 10000; ++i) {
      entries.push_back({i, i % 9});
      owner.Track(queue, entries.back());
    }
  }
  std::unordered_set<uint64_t> released;
  unsigned inspected = 0;
  auto inspect = [&](const Entry& e) {
    ++inspected;
    return e.submission;
  };
  auto release = [&](const Entry& e) { REQUIRE(released.insert(e.id).second); };
  for (unsigned step = 0; step < 200; ++step) {
    auto before = inspected;
    auto old = released.size();
    queue->DrainCompleted(3, inspect, release, 64);
    REQUIRE(inspected - before <= 64);
    REQUIRE(released.size() - old <= 64);
  }
  REQUIRE(released.size() == 4445);
  for (unsigned step = 0; step < 200; ++step) {
    auto old = released.size();
    queue->DrainCompleted(10, inspect, release, 64);
    REQUIRE(released.size() - old <= 64);
  }
  REQUIRE(inspected == 10000);
  REQUIRE(released.size() == 10000);
  queue->Close();
  entries.clear();
  queue->DrainCompleted(UINT64_MAX, inspect, release, 64);
}
TEST_CASE("pipeline snapshot reuse retains all packed color output and surface fields",
          "[renderer-performance]") {
  struct Surface {
    uint32_t handle, flags, base, address, packed_dimensions, format, width, height, sample_type;
  };
  Surface a{1, 2, 3, 4, 5, 6, 7, 8, 9};
  REQUIRE(n::NativeSurfaceStateEqual(a, a));
  uint32_t Surface::* fields[] = {&Surface::handle,
                                  &Surface::flags,
                                  &Surface::base,
                                  &Surface::address,
                                  &Surface::packed_dimensions,
                                  &Surface::format,
                                  &Surface::width,
                                  &Surface::height,
                                  &Surface::sample_type};
  for (auto f : fields) {
    auto b = a;
    b.*f ^= 0x80000000u;
    REQUIRE_FALSE(n::NativeSurfaceStateEqual(a, b));
  }
}
TEST_CASE("zero amplitude DOF plan preserves active and invalid parameter variants",
          "[renderer-performance]") {
  std::array<float, 4> projection{1, 1, 2, 1}, distance{1, 1, 1, 1}, zero{};
  REQUIRE(n::NativeDofCanBeElided(projection, distance, zero));
  for (size_t i = 0; i < 3; ++i)
    for (float amplitude : {-1.0f, 0.000001f, 1.0f}) {
      auto blur = zero;
      blur[i] = amplitude;
      REQUIRE_FALSE(n::NativeDofCanBeElided(projection, distance, blur));
    }
  for (size_t i = 0; i < 4; ++i) {
    auto p = projection;
    p[i] = std::numeric_limits<float>::infinity();
    REQUIRE_FALSE(n::NativeDofCanBeElided(p, distance, zero));
    auto d = distance;
    d[i] = std::numeric_limits<float>::quiet_NaN();
    REQUIRE_FALSE(n::NativeDofCanBeElided(projection, d, zero));
  }
  std::mt19937 random(731);
  std::uniform_real_distribution<float> value(0.01f, 10.0f);
  for (unsigned i = 0; i < 10000; ++i) {
    projection = {0, 0, value(random), value(random)};
    distance = {value(random), value(random), value(random), value(random)};
    REQUIRE(n::NativeDofCanBeElided(projection, distance, zero));
    const float encoded = float(i % 101) / 100.0f;
    const float depth = std::pow(projection[2], encoded) * projection[3];
    const float near = std::max(distance[3] - depth - distance[1] * 0.5f, 0.f) / distance[0];
    const float far = std::max(depth - distance[3] - distance[1] * 0.5f, 0.f) / distance[2];
    const float coc = std::clamp(
        std::max(std::min(0.f + (0.f - 0.f) * near, 0.f), std::min(0.f + (0.f - 0.f) * far, 0.f)),
        0.f, 1.f);
    REQUIRE(coc == 0.f);
  }
}
TEST_CASE("explicit present modes cannot be overridden by the macOS FIFO preference",
          "[renderer-performance]") {
  using gta4::presentation::ResolveMode;
  for (auto mode : {"auto", "unknown", ""})
    REQUIRE_FALSE(ResolveMode(mode).explicit_mode);
  auto immediate = ResolveMode("immediate");
  REQUIRE(immediate.explicit_mode);
  REQUIRE(immediate.immediate);
  REQUIRE_FALSE(immediate.vsync);
  REQUIRE_FALSE(immediate.prefer_fifo);
  auto mailbox = ResolveMode("mailbox");
  REQUIRE(mailbox.explicit_mode);
  REQUIRE(mailbox.mailbox);
  REQUIRE(mailbox.vsync);
  REQUIRE_FALSE(mailbox.prefer_fifo);
  auto fifo = ResolveMode("fifo");
  REQUIRE(fifo.explicit_mode);
  REQUIRE(fifo.prefer_fifo);
  REQUIRE(fifo.vsync);
  REQUIRE_FALSE(fifo.immediate);
  REQUIRE_FALSE(fifo.mailbox);
}
