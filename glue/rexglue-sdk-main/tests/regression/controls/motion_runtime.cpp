#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <cassert>
#include <iostream>
#define private public
#include "gta4_motion_bridge.h"
#undef private
#include "gta4_motion_bridge.cpp"
#include "motion_samples.h"

namespace {
using Bridge = gta4::GTA4MotionBridge;
using Consumer = gta4::MotionReloadConsumer;
void Feed(Bridge& bridge, std::array<float,3> value, uint64_t timestamp) {
  auto& m=rex::input::test_motion;
  m.device_generation=1;
  m.valid_samples=rex::input::kMotionSensorAccelerometer;
  m.acceleration_m_s2=value;
  m.accelerometer_host_timestamp_ns=timestamp;
  m.poll_host_timestamp_ns=timestamp;
  ++m.sequence;
  assert(bridge.Read().fresh);
}
void Complete(Bridge& bridge, uint64_t start) {
  Feed(bridge,kNeutral,start);
  Feed(bridge,kPitchUp,start+200000000);
  Feed(bridge,kPitchDown,start+400000000);
}
}
int main() {
  {
    Bridge bridge;
    Complete(bridge,1000000000);
    assert(bridge.reload_gesture_.sequence()==1);
    assert(bridge.ConsumeReloadGesture(Consumer::kGameplay));
    assert(!bridge.ConsumeReloadGesture(Consumer::kGameplay));
    bridge.Calibrate();
    assert(!bridge.ConsumeReloadGesture(Consumer::kScript));
    std::cout<<"PASS actual sensor-to-gesture path and calibration invalidation\n";
  }
  {
    Bridge bridge;
    Complete(bridge,2000000000);
    assert(bridge.reload_gesture_.sequence()==1);
    rex::input::test_motion.poll_host_timestamp_ns+=500000000;
    auto stale=bridge.Read();
    assert(!stale.fresh&&!stale.controls_enabled);
    Feed(bridge,kNeutral,3000000000);
    assert(!bridge.ConsumeReloadGesture(Consumer::kGameplay));
    assert(!bridge.ConsumeReloadGesture(Consumer::kScript));
    std::cout<<"PASS stalled sensor and recovery cannot replay old reload\n";
  }
  {
    Bridge bridge;
    Complete(bridge,4000000000);
    bridge.SetReloadContextActive(false);
    assert(!bridge.ConsumeReloadGesture(Consumer::kGameplay));
    Feed(bridge,kNeutral,4500000000);
    Feed(bridge,kPitchUp,4700000000);
    Feed(bridge,kPitchDown,4900000000);
    assert(bridge.reload_gesture_.sequence()==1);
    bridge.SetReloadContextActive(true);
    assert(!bridge.ConsumeReloadGesture(Consumer::kScript));
    std::cout<<"PASS menu and vehicle context invalidates and suppresses reload\n";
  }
  {
    Bridge bridge;
    Feed(bridge,kNeutral,6000000000);
    rex::input::test_motion.acceleration_m_s2={};
    ++rex::input::test_motion.accelerometer_host_timestamp_ns;
    ++rex::input::test_motion.poll_host_timestamp_ns;
    assert(!bridge.Read().fresh);
    assert(!bridge.Read().fresh);
    std::cout<<"PASS rereading invalid acceleration stays invalid\n";
  }
  {
    Bridge bridge;
    Complete(bridge,7000000000);
    gta4_motion_reload.value=false;
    assert(!bridge.ConsumeReloadGesture(Consumer::kGameplay));
    gta4_motion_reload.value=true;
    assert(!bridge.ConsumeReloadGesture(Consumer::kScript));
    gta4_motion_enabled.value=false;
    assert(!bridge.Read().controls_enabled);
    gta4_motion_enabled.value=true;
    assert(bridge.Read().controls_enabled);
    assert(!bridge.ConsumeReloadGesture(Consumer::kGameplay));
    std::cout<<"PASS preference and master toggles reset pending gestures\n";
  }
  {
    Bridge bridge;
    rex::input::test_motion.accelerometer_host_timestamp_ns=8000000000;
    rex::input::test_motion.poll_host_timestamp_ns=9000000000;
    auto snapshot=bridge.Read();
    assert(!snapshot.fresh);
    std::cout<<"PASS first observation of an old packet has no freshness grace period\n";
  }
}
