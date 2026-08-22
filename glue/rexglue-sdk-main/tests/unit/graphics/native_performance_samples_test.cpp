#include <catch2/catch_test_macros.hpp>

#include <string_view>

#include "graphics/gta4_native/native_performance_samples.h"

namespace rex::graphics::gta4_native::performance {
namespace {

TEST_CASE("GTA IV native performance ranges reject stale and duplicate tokens") {
  FrameBuilder builder;
  builder.Begin(7);
  const GpuSpanToken token =
      builder.BeginGpuRange(GpuRange::kDepthCopy, 11, 0, 1);
  REQUIRE(token);
  CHECK(builder.EndGpuRange(token));
  CHECK_FALSE(builder.EndGpuRange(token));

  builder.Begin(8);
  CHECK_FALSE(builder.EndGpuRange(token));
  CHECK(builder.sample().frame == 8);
}

TEST_CASE("GTA IV native performance ranges enforce query and nesting order") {
  FrameBuilder builder;
  builder.Begin(17);
  const GpuSpanToken parent =
      builder.BeginGpuRange(GpuRange::kFrame, 0, 0, 1);
  const GpuSpanToken child =
      builder.BeginGpuRange(GpuRange::kDepthCopy, 1, 2, 3);
  REQUIRE(parent);
  REQUIRE(child);
  CHECK_FALSE(builder.EndGpuRange(parent));
  CHECK(builder.EndGpuRange(child));
  CHECK(builder.EndGpuRange(parent));
  CHECK_FALSE(builder.BeginGpuRange(GpuRange::kComposite, 2, 2, 3));
  CHECK_FALSE(builder.BeginGpuRange(GpuRange::kComposite, 2, 4, 4));
  CHECK_FALSE(builder.BeginGpuRange(GpuRange::kComposite, 2, 4,
                                    kMaximumGpuQueriesPerFrame));
}

TEST_CASE("GTA IV native performance ranges aggregate only available ended spans") {
  FrameBuilder builder;
  builder.Begin(23);
  const GpuSpanToken available =
      builder.BeginGpuRange(GpuRange::kDeferredLightVolumes, 41, 0, 1);
  REQUIRE(builder.EndGpuRange(available));
  const GpuSpanToken unavailable =
      builder.BeginGpuRange(GpuRange::kDeferredLightVolumes, 47, 2, 3);
  REQUIRE(builder.EndGpuRange(unavailable));
  const GpuSpanToken incomplete =
      builder.BeginGpuRange(GpuRange::kTranslucentWaterSurface, 53, 4, 5);
  CHECK(builder.ResolveGpuRange(available, 100, true));
  CHECK(builder.ResolveGpuRange(unavailable, 200, false));
  CHECK_FALSE(builder.ResolveGpuRange(unavailable, 200, false));
  CHECK_FALSE(builder.ResolveGpuRange(incomplete, 300, true));

  const FrameSample& sample = builder.sample();
  CHECK(sample.gpu_ticks[size_t(GpuRange::kDeferredLightVolumes)] == 100);
  CHECK(sample.gpu_range_counts[size_t(GpuRange::kDeferredLightVolumes)] == 1);
  CHECK(sample.gpu_ticks[size_t(GpuRange::kTranslucentWaterSurface)] == 0);
  CHECK(sample.counters[size_t(Counter::kUnavailableGpuRanges)] == 1);
}

TEST_CASE("GTA IV native performance frame zero remains inactive") {
  FrameBuilder builder;
  builder.Begin(0);
  CHECK_FALSE(builder.active());
  CHECK_FALSE(builder.BeginGpuRange(GpuRange::kFrame, 0, 0, 1));
  CHECK(builder.sample().frame == 0);
}

TEST_CASE("GTA IV native performance timestamp deltas mask and wrap") {
  uint64_t delta = 0;
  REQUIRE(CalculateTimestampDelta(0xFFFFFFF8u, 0x5u, 32, &delta));
  CHECK(delta == 13);
  REQUIRE(CalculateTimestampDelta(UINT64_MAX - 7, 5, 64, &delta));
  CHECK(delta == 13);
  REQUIRE(CalculateTimestampDelta(1, 0, 1, &delta));
  CHECK(delta == 1);
  REQUIRE(CalculateTimestampDelta((uint64_t(1) << 63) - 3, 1, 63,
                                  &delta));
  CHECK(delta == 4);
  REQUIRE(CalculateTimestampDelta(0x100000005ull, 0x200000009ull, 32,
                                  &delta));
  CHECK(delta == 4);
  REQUIRE(CalculateTimestampDelta(9, 9, 32, &delta));
  CHECK(delta == 0);
  CHECK_FALSE(CalculateTimestampDelta(0, 1, 0, &delta));
  CHECK_FALSE(CalculateTimestampDelta(0, 1, 65, &delta));
  CHECK_FALSE(CalculateTimestampDelta(0, 1, 32, nullptr));
}

TEST_CASE("GTA IV native performance finalize counts unresolved spans once") {
  FrameBuilder builder;
  builder.Begin(55);
  const GpuSpanToken incomplete =
      builder.BeginGpuRange(GpuRange::kComposite, 77, 0, 1);
  REQUIRE(incomplete);
  const FrameSample& sample = builder.Finish();
  CHECK(sample.counters[size_t(Counter::kDroppedGpuRanges)] == 1);
  CHECK_FALSE(builder.ResolveGpuRange(incomplete, 500, true));
  CHECK(builder.Finish().counters[size_t(Counter::kDroppedGpuRanges)] == 1);
}

TEST_CASE("GTA IV native performance range names cover reflection families") {
  CHECK(std::string_view(GpuRangeName(GpuRange::kMirrorReflections)) ==
        "mirror-reflections");
  CHECK(std::string_view(GpuRangeName(GpuRange::kWaterReflections)) ==
        "water-reflections");
  CHECK(std::string_view(GpuRangeName(GpuRange::kEnvironmentReflections)) ==
        "environment-reflections");
}

TEST_CASE("GTA IV native performance keeps water surface and texture separate") {
  CHECK(std::string_view(
            GpuRangeName(GpuRange::kTranslucentWaterSurface)) ==
        "translucent-water-surface");
  CHECK(std::string_view(
            GpuRangeName(GpuRange::kTranslucentWaterTexture)) ==
        "translucent-water-texture");
}

TEST_CASE("GTA IV native performance ring overwrites oldest samples deterministically") {
  FrameSampleRing ring;
  FrameSample sample{};
  for (size_t index = 0; index < ring.capacity(); ++index) {
    sample.frame = uint32_t(index + 1);
    ring.Push(sample);
  }
  REQUIRE(ring.size() == ring.capacity());
  FrameSample copied{};
  REQUIRE(ring.CopyOldest(0, &copied));
  CHECK(copied.frame == 1);
  REQUIRE(ring.CopyNewest(0, &copied));
  CHECK(copied.frame == ring.capacity());

  sample.frame = 9001;
  ring.Push(sample);
  CHECK(ring.size() == ring.capacity());
  REQUIRE(ring.CopyOldest(0, &copied));
  CHECK(copied.frame == 2);
  REQUIRE(ring.CopyNewest(0, &copied));
  CHECK(copied.frame == 9001);
  CHECK_FALSE(ring.CopyNewest(0, nullptr));
  CHECK_FALSE(ring.CopyOldest(ring.size(), &copied));
}

TEST_CASE("GTA IV native performance CPU ranges and counters aggregate without allocation") {
  FrameBuilder builder;
  builder.Begin(99);
  builder.AddCpuRange(CpuRange::kCommandRecording, 17);
  builder.AddCpuRange(CpuRange::kCommandRecording, 19);
  builder.AddCounter(Counter::kUploadBytes, 130);
  builder.AddCounter(Counter::kUploadBytes, 26);

  const FrameSample& sample = builder.sample();
  CHECK(sample.cpu_ticks[size_t(CpuRange::kCommandRecording)] == 36);
  CHECK(sample.cpu_range_counts[size_t(CpuRange::kCommandRecording)] == 2);
  CHECK(sample.counters[size_t(Counter::kUploadBytes)] == 156);
}

}  // namespace
}  // namespace rex::graphics::gta4_native::performance
