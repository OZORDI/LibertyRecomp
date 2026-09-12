#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <numeric>
#include <thread>
#include "graphics/gta4_native/native_cpu_profile.h"
#include "graphics/gta4_native/native_profile_detail.h"

namespace prof = rex::graphics::gta4_native::profile;
namespace perf = rex::graphics::gta4_native::performance;

TEST_CASE("detailed CPU scopes partition self time and retain inclusive parents",
          "[native-profiler-detail]") {
  prof::CpuRecorder r;
  REQUIRE(r.Begin(100));
  auto root = r.Enter(prof::CpuOp::kPublishFrame, 100);
  r.SetPhase(prof::CpuPhase::kRecording);
  auto record = r.Enter(prof::CpuOp::kRecordFrame, 110);
  r.SetContext({7, 4, 0xABC, 0xDEF});
  auto command = r.Enter(prof::CpuOp::kRecordCommand, 120);
  auto driver = r.Enter(prof::CpuOp::kDriverDraw, 130);
  REQUIRE(r.Leave(driver, 160));
  REQUIRE(r.Leave(command, 180));
  REQUIRE(r.Leave(record, 190));
  REQUIRE(r.Leave(root, 200));
  const auto frame = r.Finish(200, 2);
  REQUIRE(frame.enabled);
  REQUIRE(frame.measurements.size() == 4);
  uint64_t self = 0;
  for (const auto& m : frame.measurements) {
    self += m.self_ticks;
    REQUIRE(m.self_ticks <= m.inclusive_ticks);
    REQUIRE(m.calls == 1);
    if (m.op == prof::CpuOp::kRecordCommand) {
      CHECK(m.inclusive_ticks == 60);
      CHECK(m.self_ticks == 30);
      CHECK(m.worst.command == 7);
      CHECK(m.worst.pixel_shader == 0xDEF);
    }
  }
  CHECK(self == 100);
  CHECK(frame.clock_reads == 8);
  CHECK(frame.invalid_scopes == 0);
  REQUIRE(frame.shaders.size() == 1);
  CHECK(frame.shaders[0].inclusive_ticks == 60);
  CHECK(frame.shaders[0].worst_command == 7);
}

TEST_CASE("CPU scope tokens reject cross-frame, out-of-order and duplicate completion",
          "[native-profiler-detail]") {
  prof::CpuRecorder r;
  CHECK_FALSE(r.Enter(prof::CpuOp::kDriverDraw, 1));
  REQUIRE(r.Begin(10));
  auto outer = r.Enter(prof::CpuOp::kRecordFrame, 11);
  auto inner = r.Enter(prof::CpuOp::kDriverDraw, 12);
  CHECK_FALSE(r.Leave(outer, 13));
  REQUIRE(r.Leave(inner, 14));
  CHECK_FALSE(r.Leave(inner, 15));
  REQUIRE(r.Leave(outer, 16));
  auto frame = r.Finish(17);
  CHECK(frame.invalid_scopes == 2);
  REQUIRE(r.Begin(20));
  auto next = r.Enter(prof::CpuOp::kRecordFrame, 20);
  CHECK_FALSE(r.Leave(outer, 21));
  REQUIRE(r.Leave(next, 22));
  CHECK(r.Finish(22).invalid_scopes == 1);
  CHECK_FALSE(r.Finish(23).enabled);
}

TEST_CASE("CPU capture storage is bounded and omitted events remain in aggregates",
          "[native-profiler-detail]") {
  prof::CpuRecorder r;
  REQUIRE(r.Begin(0));
  auto root = r.Enter(prof::CpuOp::kPublishFrame, 0);
  uint64_t now = 1;
  for (uint32_t i = 0; i < 10000; ++i) {
    r.SetContext({i, 3, 123, 456});
    auto op = r.Enter(prof::CpuOp::kRecordCommand, now);
    now += uint64_t(i % 7 + 1);
    REQUIRE(r.Leave(op, now));
  }
  REQUIRE(r.Leave(root, now));
  const auto f = r.Finish(now);
  CHECK(f.events.size() == prof::kCpuEventLimit);
  CHECK(f.omitted_events + f.events.size() == 10001);
  CHECK(f.shaders.size() == 1);
  CHECK(f.shaders.front().commands == 10000);
  uint64_t total = 0;
  for (const auto& m : f.measurements)
    total += m.self_ticks;
  CHECK(total == now);
  CHECK(f.stack_overflows == 0);
  std::vector<prof::CpuRecorder::Token> tokens;
  REQUIRE(r.Begin(0));
  for (uint32_t i = 0; i < prof::kCpuStackLimit; ++i) {
    tokens.push_back(r.Enter(prof::CpuOp::kRecordFrame, i));
    REQUIRE(tokens.back());
  }
  CHECK_FALSE(r.Enter(prof::CpuOp::kDriverDraw, prof::kCpuStackLimit));
  for (auto i = tokens.rbegin(); i != tokens.rend(); ++i)
    REQUIRE(r.Leave(*i, ++now));
  CHECK(r.Finish(now).stack_overflows == 1);
}

TEST_CASE("CPU shader-key overflow is disclosed rather than unbounded",
          "[native-profiler-detail]") {
  prof::CpuRecorder r;
  REQUIRE(r.Begin(1));
  for (uint32_t i = 0; i < 400; ++i) {
    r.SetContext({i, 0, i + 1, i + 100});
    auto token = r.Enter(prof::CpuOp::kRecordCommand, i * 2 + 1);
    REQUIRE(r.Leave(token, i * 2 + 2));
  }
  auto f = r.Finish(1000);
  CHECK(f.shaders.size() == prof::kCpuShaderLimit);
  CHECK(f.shader_overflows == 400 - prof::kCpuShaderLimit);
  CHECK(f.measurements.front().calls == 400);
}

TEST_CASE("producer dwell is recorded as overlapping per-command data",
          "[native-profiler-detail]") {
  prof::TransportSummary s;
  s.Observe({100, 70, 10, 5, 15}, 200, 3);
  s.Observe({110, 80, 20, 10, 25}, 201, 2);
  s.Observe({}, 202, 1);
  CHECK(s.commands == 3);
  CHECK(s.measured_commands == 2);
  CHECK(s.capture_ticks == 150);
  CHECK(s.dwell_ticks == 191);
  CHECK(s.max_dwell_ticks == 100);
  CHECK(s.backpressure_ticks == 40);
  CHECK(s.queue_peak == 3);
}

TEST_CASE("detailed CPU collector generations are independent across threads",
          "[native-profiler-detail]") {
  std::array<prof::CpuFrameData, 2> frames;
  auto run = [&](size_t index) {
    prof::CpuRecorder r;
    r.Begin(0);
    auto op = r.Enter(prof::CpuOp::kDriverDraw, 1);
    r.Leave(op, uint64_t(index + 5));
    frames[index] = r.Finish(10);
  };
  std::thread first(run, 0), second(run, 1);
  first.join();
  second.join();
  CHECK(frames[0].measurements[0].self_ticks == 4);
  CHECK(frames[1].measurements[0].self_ticks == 5);
}

TEST_CASE("profile export preserves unavailable GPU observations and quoted shader names",
          "[native-profiler-detail]") {
  CHECK(prof::JsonString("a\"b\n\\c") == "\"a\\\"b\\u000a\\\\c\"");
  CHECK(prof::HexIdentity(0xFA3) == "0000000000000FA3");
  CHECK(std::string(prof::CounterSemantics(perf::Counter::kTextureImageEvictions)) ==
        "process-cumulative");
  CHECK(std::string(prof::CounterSemantics(perf::Counter::kTextureImagesLive)) ==
        "instantaneous-gauge");
  CHECK(std::string(prof::CounterSemantics(perf::Counter::kDescriptorEntriesWritten)) ==
        "per-sample");
}
