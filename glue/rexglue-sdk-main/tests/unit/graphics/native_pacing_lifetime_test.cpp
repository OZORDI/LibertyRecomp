#include <catch2/catch_test_macros.hpp>
#include <array>
#include <atomic>
#include <future>
#include <memory>
#include <thread>
#include <vector>
#include "graphics/gta4_native/native_resource_reclaimer.h"
#include "graphics/gta4_native/native_attachment_policy.h"
#include "graphics/gta4_native/stateful_constant_state.h"
namespace n = rex::graphics::gta4_native;

TEST_CASE("resource reclaimer bounds include active destruction and preserve rejected ownership", "[pacing]") {
  std::promise<void> started, release;
  auto gate = release.get_future().share();
  std::vector<int> destroyed;
  bool first_ok, second_ok, overflow_ok, inflight_ok;
  n::NativeResourceReclaimer<std::unique_ptr<int>>::Usage usage;
  auto first = std::make_unique<int>(1), second = std::make_unique<int>(2);
  auto overflow = std::make_unique<int>(3), inflight = std::make_unique<int>(4);
  {
    n::NativeResourceReclaimer<std::unique_ptr<int>> queue(128, 2, [&](auto& item) {
      if (*item == 1) { started.set_value(); gate.wait(); }
      destroyed.push_back(*item);
      item.reset();
    });
    first_ok = queue.TryEnqueue(first, 64, 0, 7, 7);
    started.get_future().wait();
    inflight_ok = queue.TryEnqueue(inflight, 1, 0, 8, 7);
    second_ok = queue.TryEnqueue(second, 64, 1, 7, 7);
    overflow_ok = queue.TryEnqueue(overflow, 1, 0, 7, 7);
    usage = queue.usage();
    release.set_value();
  }
  REQUIRE(first_ok); REQUIRE(second_ok);
  REQUIRE_FALSE(overflow_ok); REQUIRE_FALSE(inflight_ok);
  REQUIRE_FALSE(first); REQUIRE_FALSE(second);
  REQUIRE(*overflow == 3); REQUIRE(*inflight == 4);
  REQUIRE(usage.bytes == 128); REQUIRE(usage.count == 2);
  REQUIRE(usage.heaps[0] == 64); REQUIRE(usage.heaps[1] == 64);
  REQUIRE(destroyed == std::vector<int>{1, 2});
}
TEST_CASE("resource reclaimer drains every accepted resource once under sustained reuse", "[pacing]") {
  std::vector<unsigned> destroyed;
  {
    n::NativeResourceReclaimer<std::unique_ptr<unsigned>> queue(64, 8, [&](auto& item) {
      destroyed.push_back(*item); item.reset();
    });
    for (unsigned i = 0; i < 10000; ++i) {
      auto item = std::make_unique<unsigned>(i);
      while (!queue.TryEnqueue(item, 8, i % 2, i, i)) std::this_thread::yield();
    }
    queue.Drain();
    REQUIRE(queue.usage().bytes == 0);
    REQUIRE(queue.usage().count == 0);
  }
  REQUIRE(destroyed.size() == 10000);
  for (unsigned i = 0; i < destroyed.size(); ++i) REQUIRE(destroyed[i] == i);
}
TEST_CASE("draw capture snapshots remain immutable across later constant updates", "[pacing]") {
  n::AuthoritativeConstantState state(16);
  n::ConstantPayloadDelta initial;
  std::vector<uint8_t> expected(16, 1);
  REQUIRE(n::CaptureCompleteConstantSnapshot(expected, initial));
  auto hash = [](std::span<const uint8_t> bytes) {
    uint64_t result = 0; for (auto byte : bytes) result = result * 31 + byte; return result;
  };
  REQUIRE(state.Apply(initial, hash));
  auto first = state.SnapshotCurrentVersion();
  REQUIRE(first->materialized);
  REQUIRE(state.SnapshotCurrentVersion()->materialized == first->materialized);
  n::ConstantPayloadDelta changed;
  changed.ranges.push_back({4, 0, 1}); changed.payload.push_back(9);
  REQUIRE(state.Apply(changed, hash));
  auto next = state.SnapshotCurrentVersion();
  REQUIRE(*n::AuthoritativeConstantState::Materialize(first) == expected);
  expected[4] = 9;
  REQUIRE(*n::AuthoritativeConstantState::Materialize(next) == expected);
  REQUIRE_FALSE(next->parent);
}
TEST_CASE("rendering scopes ignore component masks but respect every attachment boundary", "[pacing]") {
  struct Target {
    std::array<unsigned, 4> color_views{}, color_formats{};
    unsigned depth_view = 0, depth_format = 0, samples = 1, width = 640, height = 360;
    unsigned color_attachment_mask = 1, color_write_mask = 15;
    bool depth_stencil_attachment_active = false, uses_presenter = false;
  };
  Target a, b;
  b.color_write_mask = 7;
  REQUIRE(n::NativeRenderingAttachmentsEqual(a, b));
#define BOUNDARY(field) { b = a; ++b.field; REQUIRE_FALSE(n::NativeRenderingAttachmentsEqual(a, b)); }
  BOUNDARY(color_views[0]); BOUNDARY(color_formats[0]); BOUNDARY(depth_view);
  BOUNDARY(depth_format); BOUNDARY(samples); BOUNDARY(width); BOUNDARY(height);
  BOUNDARY(color_attachment_mask);
#undef BOUNDARY
  b = a; b.depth_stencil_attachment_active = true;
  REQUIRE_FALSE(n::NativeRenderingAttachmentsEqual(a, b));
  b = a; b.uses_presenter = true;
  REQUIRE_FALSE(n::NativeRenderingAttachmentsEqual(a, b));
}
