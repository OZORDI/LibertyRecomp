#pragma once

#include <array>
#include <cmath>
#include <cstdint>
#include <mutex>
#include <unordered_map>

#include <rex/input/motion.h>

namespace rex::input {

// Sensor deliveries, not getter calls, advance sample identity. This lock is
// independent of the driver/controller lock: SDL may deliver an event from
// inside SDL_UpdateGamepads with its joystick lock already held.
class MotionSampleCache {
 public:
  void BeginDevice(uint64_t id, uint64_t generation, uint64_t started_ns) {
    std::lock_guard lock(mutex_);
    auto& entry = devices_[id];
    entry = {};
    entry.state.device_generation = generation;
    entry.started_ns = started_ns;
  }

  void EndDevice(uint64_t id) {
    std::lock_guard lock(mutex_);
    devices_.erase(id);
  }

  bool Observe(uint64_t id, uint32_t sensor, const std::array<float, 3>& values,
               uint64_t host_ns, uint64_t sensor_ns) {
    if (!host_ns || (sensor != kMotionSensorAccelerometer && sensor != kMotionSensorGyroscope)) {
      return false;
    }
    for (float value : values) {
      if (!std::isfinite(value)) {
        return false;
      }
    }
    std::lock_guard lock(mutex_);
    auto found = devices_.find(id);
    if (found == devices_.end() || host_ns < found->second.started_ns) {
      return false;
    }
    auto& state = found->second.state;
    const bool accelerometer = sensor == kMotionSensorAccelerometer;
    auto& last_host = accelerometer ? state.accelerometer_host_timestamp_ns
                                   : state.gyroscope_host_timestamp_ns;
    auto& last_sensor = accelerometer ? state.accelerometer_sensor_timestamp_ns
                                     : state.gyroscope_sensor_timestamp_ns;
    if (host_ns <= last_host || (sensor_ns && last_sensor && sensor_ns == last_sensor)) {
      return false;
    }
    (accelerometer ? state.acceleration_m_s2 : state.angular_velocity_rad_s) = values;
    last_host = host_ns;
    last_sensor = sensor_ns;
    state.valid_samples |= sensor;
    ++state.sequence;
    return true;
  }

  bool Read(uint64_t id, uint64_t poll_ns, MotionState& out) const {
    std::lock_guard lock(mutex_);
    const auto found = devices_.find(id);
    if (found == devices_.end()) {
      out = {};
      return false;
    }
    out = found->second.state;
    out.poll_host_timestamp_ns = poll_ns;
    return true;
  }

 private:
  struct Entry {
    MotionState state{};
    uint64_t started_ns = 0;
  };
  mutable std::mutex mutex_;
  std::unordered_map<uint64_t, Entry> devices_;
};

// Both timestamps must belong to the same host clock. Sensor-native clocks are
// retained for identity, never compared to the host's polling clock.
constexpr bool MotionSampleIsFresh(uint64_t sample_ns, uint64_t poll_ns,
                                   uint64_t timeout_ns) noexcept {
  return sample_ns && poll_ns >= sample_ns && poll_ns - sample_ns <= timeout_ns;
}

}  // namespace rex::input
