#include <array>
#include <chrono>
#include <limits>
#include <thread>

#include <catch2/catch_test_macros.hpp>

#include <rex/input/motion_sample_cache.h>
#include "gta4_motion_action_policy.h"
#include "gta4_motion_reload_policy.h"
#include "input/context_touch_layout.h"

using namespace std::chrono_literals;

TEST_CASE("motion sample reads cannot renew a cached packet", "[controls_fix][motion]") {
  rex::input::MotionSampleCache cache;
  cache.BeginDevice(1, 7, 100);
  const std::array<float, 3> gravity{0, 9.8f, 0};
  REQUIRE(cache.Observe(1, rex::input::kMotionSensorAccelerometer, gravity, 110, 900));
  rex::input::MotionState first{}, repeated{};
  REQUIRE(cache.Read(1, 120, first));
  REQUIRE(cache.Read(1, 1000, repeated));
  CHECK(first.sequence == repeated.sequence);
  CHECK(first.accelerometer_host_timestamp_ns == repeated.accelerometer_host_timestamp_ns);
  CHECK(rex::input::MotionSampleIsFresh(first.accelerometer_host_timestamp_ns,
                                       first.poll_host_timestamp_ns, 100));
  CHECK_FALSE(rex::input::MotionSampleIsFresh(repeated.accelerometer_host_timestamp_ns,
                                             repeated.poll_host_timestamp_ns, 100));
  CHECK_FALSE(cache.Observe(1, rex::input::kMotionSensorAccelerometer, gravity, 120, 900));
  CHECK(cache.Observe(1, rex::input::kMotionSensorAccelerometer, gravity, 130, 901));
  CHECK(cache.Read(1, 131, repeated));
  CHECK(repeated.sequence == first.sequence + 1);
  CHECK(repeated.acceleration_m_s2 == gravity);
}

TEST_CASE("motion cache separates sensors and device lifetimes", "[controls_fix][motion]") {
  rex::input::MotionSampleCache cache;
  const std::array<float, 3> value{0, 1, 0};
  CHECK_FALSE(cache.Observe(1, rex::input::kMotionSensorAccelerometer, value, 100, 1));
  cache.BeginDevice(1, 2, 100);
  CHECK_FALSE(cache.Observe(1, rex::input::kMotionSensorAccelerometer, value, 99, 1));
  REQUIRE(cache.Observe(1, rex::input::kMotionSensorAccelerometer, value, 101, 0));
  REQUIRE(cache.Observe(1, rex::input::kMotionSensorGyroscope, value, 200, 0));
  rex::input::MotionState state{};
  REQUIRE(cache.Read(1, 201, state));
  CHECK(state.accelerometer_host_timestamp_ns == 101);
  CHECK(state.gyroscope_host_timestamp_ns == 200);
  CHECK_FALSE(cache.Observe(1, rex::input::kMotionSensorAccelerometer, value, 100, 2));
  const auto nan = std::numeric_limits<float>::quiet_NaN();
  CHECK_FALSE(cache.Observe(1, rex::input::kMotionSensorAccelerometer, {nan, 0, 0}, 300, 3));
  cache.EndDevice(1);
  CHECK_FALSE(cache.Read(1, 400, state));
  cache.BeginDevice(1, 3, 500);
  REQUIRE(cache.Read(1, 501, state));
  CHECK(state.device_generation == 3);
  CHECK(state.valid_samples == 0);
  CHECK(state.sequence == 0);
  CHECK_FALSE(rex::input::MotionSampleIsFresh(600, 599, 100));
}

TEST_CASE("motion delivery and reads share no controller lock", "[controls_fix][motion]") {
  rex::input::MotionSampleCache cache;
  cache.BeginDevice(1, 1, 1);
  std::jthread producer([&] {
    for (uint64_t i = 2; i <= 1000; ++i) {
      cache.Observe(1, rex::input::kMotionSensorAccelerometer, {0, 1, 0}, i, i);
    }
  });
  rex::input::MotionState state{};
  for (uint64_t i = 2; i <= 1000; ++i) {
    REQUIRE(cache.Read(1, i, state));
    CHECK(state.accelerometer_host_timestamp_ns == state.accelerometer_sensor_timestamp_ns);
  }
  producer.join();
  REQUIRE(cache.Read(1, 1001, state));
  CHECK(state.sequence == 999);
}

namespace {
using Reload = gta4::MotionReloadGesture<2>;
gta4::MotionReloadConfig ReloadConfig() {
  return {.up_radians = 0.5, .down_radians = -0.2, .neutral_radians = 0.1,
          .window = 800ms, .cooldown = 600ms, .completion_lifetime = 250ms};
}
void Complete(Reload& gesture, Reload::TimePoint start) {
  auto config = ReloadConfig();
  gesture.Observe(0, start, config);
  gesture.Observe(0.6, start + 10ms, config);
  gesture.Observe(-0.3, start + 20ms, config);
}
}

TEST_CASE("reload completion has bounded per-consumer delivery", "[controls_fix][motion]") {
  Reload gesture;
  const auto now = Reload::TimePoint{} + 1s;
  Complete(gesture, now);
  CHECK(gesture.sequence() == 1);
  CHECK(gesture.Consume(0, now + 21ms));
  CHECK_FALSE(gesture.Consume(0, now + 22ms));
  CHECK(gesture.Consume(1, now + 23ms));
  CHECK_FALSE(gesture.Consume(2, now + 24ms));
  gesture.Observe(0, now + 621ms, ReloadConfig());
  gesture.Observe(0.6, now + 622ms, ReloadConfig());
  gesture.Observe(-0.3, now + 623ms, ReloadConfig());
  CHECK(gesture.sequence() == 2);
  CHECK_FALSE(gesture.Consume(0, now + 873ms));
  CHECK_FALSE(gesture.Consume(1, now + 874ms));
}

TEST_CASE("reload invalidation requires neutral and discards both pending consumers", "[controls_fix][motion]") {
  Reload gesture;
  const auto now = Reload::TimePoint{} + 1s;
  Complete(gesture, now);
  gesture.Reset();
  CHECK_FALSE(gesture.Consume(0, now + 21ms));
  CHECK_FALSE(gesture.Consume(1, now + 21ms));
  gesture.Observe(0.6, now + 30ms, ReloadConfig());
  gesture.Observe(-0.3, now + 40ms, ReloadConfig());
  CHECK(gesture.sequence() == 1);
  Complete(gesture, now + 50ms);
  CHECK(gesture.sequence() == 2);
  CHECK(gesture.Consume(0, now + 71ms));
}

TEST_CASE("reload ignores reversed and expired gestures", "[controls_fix][motion]") {
  Reload gesture;
  const auto now = Reload::TimePoint{} + 1s;
  gesture.Observe(0, now, ReloadConfig());
  gesture.Observe(-0.3, now + 1ms, ReloadConfig());
  CHECK_FALSE(gesture.Consume(0, now + 2ms));
  gesture.Observe(0.6, now + 3ms, ReloadConfig());
  gesture.Observe(-0.3, now + 804ms, ReloadConfig());
  CHECK(gesture.sequence() == 0);
  CHECK_FALSE(gesture.Consume(0, now + 805ms));
}
namespace {
struct TestPad {
  std::array<uint8_t, 8192> bytes{};
  static constexpr uint32_t control = 64;
  uint32_t record(uint32_t action) const { return control + 2328 + action * 12; }
  TestPad() {
    for (uint32_t i : {30u, 31u, 32u, 33u}) {
      bytes[record(i)] = i % 2 ? 0 : 255;
      bytes[record(i) + 2] = rex::input::mnk::EncodeActionMagnitude(bytes[record(i)], 127);
    }
  }
  uint8_t raw(uint32_t action) const {
    return rex::input::mnk::DecodeActionMagnitude(bytes[record(action)], bytes[record(action)+2]);
  }
};
}

TEST_CASE("helicopter motion feeds bank rather than yaw and restores action bytes", "[controls_fix][motion]") {
  TestPad pad;
  const auto before = pad.bytes;
  const auto plan = gta4::BuildVehicleMotionActions(gta4::VehicleMotionKind::kHelicopter, 1, 1, false);
  CHECK(plan.lateral == -255);
  CHECK(plan.pitch == 255);
  {
    gta4::ScopedMotionActions scope(TestPad::control, plan,
        [&](uint32_t p) { return pad.bytes.at(p); },
        [&](uint32_t p, uint8_t v) { pad.bytes.at(p) = v; });
    CHECK(scope.changed());
    CHECK(pad.raw(30) == 0);
    CHECK(pad.raw(31) == 0);
    CHECK(pad.raw(32) == 255);
    CHECK(pad.raw(33) == 255);
    CHECK(pad.raw(57) == 0);
    CHECK(pad.raw(58) == 0);
    // The retail consumer can compute dependent steering from the new input.
    const float bank = -float(rex::input::mnk::DecodeSignedActionRaw(pad.raw(30))) / 255.0f;
    CHECK(bank == 1.0f);
  }
  CHECK(pad.bytes == before);
}

TEST_CASE("motion scope keeps stronger controller input and game replacements", "[controls_fix][motion]") {
  TestPad pad;
  pad.bytes[pad.record(30)+2] = rex::input::mnk::EncodeActionMagnitude(pad.bytes[pad.record(30)], 255);
  pad.bytes[pad.record(31)+2] = 255;
  const auto before = pad.bytes;
  {
    gta4::ScopedMotionActions scope(TestPad::control, gta4::VehicleMotionActions{-128, 0, 0},
        [&](uint32_t p) { return pad.bytes.at(p); },
        [&](uint32_t p, uint8_t v) { pad.bytes.at(p) = v; });
    CHECK_FALSE(scope.changed());
  }
  CHECK(pad.bytes == before);
  {
    gta4::ScopedMotionActions scope(TestPad::control, gta4::VehicleMotionActions{0, 255, 0},
        [&](uint32_t p) { return pad.bytes.at(p); },
        [&](uint32_t p, uint8_t v) { pad.bytes.at(p) = v; });
    CHECK(scope.changed());
    pad.bytes[pad.record(32)+2] = 77;
  }
  CHECK(pad.bytes[pad.record(32)+2] == 77);
  CHECK(pad.bytes[pad.record(33)+2] == before[pad.record(33)+2]);
}

TEST_CASE("motion aftertouch only augments airborne car actions", "[controls_fix][motion]") {
  const auto ground = gta4::BuildVehicleMotionActions(gta4::VehicleMotionKind::kAutomobile, 1, 1, false);
  CHECK(ground.lateral == 0);
  CHECK(ground.pedals == 0);
  TestPad pad;
  const auto before = pad.bytes;
  {
    auto plan = gta4::BuildVehicleMotionActions(gta4::VehicleMotionKind::kAutomobile, 0, -1, true);
    gta4::ScopedMotionActions scope(TestPad::control, plan,
        [&](uint32_t p) { return pad.bytes.at(p); },
        [&](uint32_t p, uint8_t v) { pad.bytes.at(p) = v; });
    CHECK(pad.raw(40) == 0);
    CHECK(pad.raw(41) == 255);
  }
  CHECK(pad.bytes == before);
  CHECK(gta4::QuantizeMotionAxis(std::numeric_limits<float>::quiet_NaN()) == 0);
}

TEST_CASE("touch short taps last exactly one epoch and cancellation emits no tap", "[controls_fix][touch]") {
  using namespace gta4::input;
  ContextTouchKeyLatch latch;
  auto key = rex::ui::VirtualKey::kSpace;
  const auto index = static_cast<uint16_t>(key);
  std::array<uint8_t, 256> down{}, pressed{};
  latch.Press(key, 41);
  latch.Release(key);
  latch.Collect(41, down, pressed);
  CHECK(down[index] == 1);
  CHECK(pressed[index] == 1);
  down={}; pressed={}; latch.Collect(42, down, pressed);
  CHECK(down[index] == 0);
  CHECK(pressed[index] == 0);
  latch.Press(key, 43); latch.Release(key, true);
  down={}; pressed={}; latch.Collect(43, down, pressed);
  CHECK(down[index] == 0);
  CHECK(pressed[index] == 0);
  latch.Press(key, 44); latch.Press(key, 45);
  down={}; pressed={}; latch.Collect(45, down, pressed);
  CHECK(down[index] == 1);
  CHECK(pressed[index] == 0);
}

TEST_CASE("touch minigame query kinds share a button but raw IDs remain distinct", "[controls_fix][touch]") {
  using namespace gta4::input;
  ContextTouchViewport viewport{};
  viewport.valid=viewport.focused=true; viewport.safe_width=1280; viewport.safe_height=720;
  const std::array queries = {
      TouchScriptControl{TouchScriptQueryKind::kControlPressed,17},
      TouchScriptControl{TouchScriptQueryKind::kControlHeld,17},
      TouchScriptControl{TouchScriptQueryKind::kControlAnalog,17},
      TouchScriptControl{TouchScriptQueryKind::kRawButton,17}};
  const auto layout = BuildContextTouchLayout(ContextTouchMode::kMinigame,viewport,queries);
  REQUIRE(layout.control_count == 2);
  CHECK(layout.controls[0].script.kind == TouchScriptQueryKind::kControlHeld);
  CHECK(layout.controls[1].script.kind == TouchScriptQueryKind::kRawButton);
  auto helicopter = BuildContextTouchLayout(ContextTouchMode::kVehicleHelicopter,viewport,{});
  bool yaw_left=false, yaw_right=false;
  for (size_t i=0;i<helicopter.control_count;++i) {
    yaw_left |= helicopter.controls[i].key == rex::ui::VirtualKey::kNumpad4;
    yaw_right |= helicopter.controls[i].key == rex::ui::VirtualKey::kNumpad6;
  }
  CHECK(yaw_left);
  CHECK(yaw_right);
}

#include <rex/input/mnk/controller_compatibility.h>
#include "gta4_keyboard_controller.h"
#include "gta4_gyro_aim_policy.h"

TEST_CASE("virtual touch controller keys use retail phone and flight bindings", "[controls_fix][touch]") {
  using namespace rex::input; using namespace rex::input::mnk;
  using rex::ui::VirtualKey;
  const auto before = GetNativeControllerCompatibilityBindings();
  SetNativeControllerCompatibilityBindings(gta4::input::KeyboardControllerBindings());
  std::array<uint8_t,256> keys{};
  keys[static_cast<uint16_t>(VirtualKey::kUp)] = 1;
  keys[static_cast<uint16_t>(VirtualKey::kReturn)] = 1;
  keys[static_cast<uint16_t>(VirtualKey::kBack)] = 1;
  PublishVirtualControllerCompatibilityKeys(0,keys,true);
  X_INPUT_GAMEPAD first{},again{};
  CHECK(ReadVirtualControllerCompatibilityGamepad(0,first));
  CHECK(first.buttons == (X_INPUT_GAMEPAD_DPAD_UP | X_INPUT_GAMEPAD_A | X_INPUT_GAMEPAD_B));
  CHECK(ReadVirtualControllerCompatibilityGamepad(0,again));
  CHECK(first.buttons == again.buttons);
  CHECK_FALSE(ReadVirtualControllerCompatibilityGamepad(1,again));
  SetNativeControllerCompatibilityBindings(gta4::input::KeyboardControllerBindings(true));
  keys={};keys[static_cast<uint16_t>(VirtualKey::kNumpad4)] = 1;
  keys[static_cast<uint16_t>(VirtualKey::kW)] = 1;
  PublishVirtualControllerCompatibilityKeys(0,keys,true);
  CHECK(ReadVirtualControllerCompatibilityGamepad(0,first));
  CHECK(first.buttons == X_INPUT_GAMEPAD_LEFT_SHOULDER);
  CHECK(first.right_trigger == 255);
  PublishVirtualControllerCompatibilityKeys(0,{},false);
  CHECK_FALSE(ReadVirtualControllerCompatibilityGamepad(0,first));
  SetNativeControllerCompatibilityBindings(before);
}

TEST_CASE("touch frontend and map expose an accessible back button", "[controls_fix][touch]") {
  using namespace gta4::input;
  ContextTouchViewport v{};v.valid=v.focused=true;v.safe_width=1280;v.safe_height=720;
  for (auto mode : {ContextTouchMode::kFrontend,ContextTouchMode::kMap}) {
    const auto layout=BuildContextTouchLayout(mode,v,{});
    REQUIRE(layout.control_count==1);
    CHECK(layout.controls[0].key==rex::ui::VirtualKey::kBack);
    CHECK(layout.controls[0].minimum_x>=v.safe_x);
    CHECK(layout.controls[0].maximum_y<=v.safe_y+v.safe_height);
  }
}

TEST_CASE("gyro aiming is bounded fresh rate input and never a repeated displacement", "[controls_fix][motion]") {
  using gta4::input::BuildGyroAimActions;
  const auto input=BuildGyroAimActions(true,{1,2,0},2,false,false);
  CHECK(input.horizontal==255);
  CHECK(input.vertical==-128);
  CHECK(BuildGyroAimActions(false,{1,2,0},2,false,false).horizontal==0);
  CHECK(BuildGyroAimActions(true,{1,2,0},0,false,false).horizontal==0);
  CHECK(BuildGyroAimActions(true,{0,0.01f,0},2,false,false).horizontal==0);
  CHECK(BuildGyroAimActions(true,{1,2,0},2,true,true).horizontal==-255);
  CHECK(BuildGyroAimActions(true,{1,2,0},2,true,true).vertical==128);
  CHECK(BuildGyroAimActions(true,{0,std::numeric_limits<float>::quiet_NaN(),0},2,false,false).horizontal==0);
  for (int i=0;i<4;++i) CHECK(BuildGyroAimActions(true,{1,2,0},2,false,false).horizontal==input.horizontal);
}

TEST_CASE("controller-only gyro does not require native keyboard input", "[controls_fix][motion]") {
  using gta4::input::NeedsNativeActionReplay;
  CHECK(NeedsNativeActionReplay(false,false,true,true));
  CHECK_FALSE(NeedsNativeActionReplay(false,false,false,true));
  CHECK_FALSE(NeedsNativeActionReplay(false,false,true,false));
  CHECK(NeedsNativeActionReplay(true,false,false,false));
  CHECK(NeedsNativeActionReplay(false,true,false,false));
}
