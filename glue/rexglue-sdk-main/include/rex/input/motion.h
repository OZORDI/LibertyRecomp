#pragma once

#include <array>
#include <cstdint>

namespace rex::input {

enum MotionSensorFlags : uint32_t {
  kMotionSensorNone = 0,
  kMotionSensorAccelerometer = 1u << 0,
  kMotionSensorGyroscope = 1u << 1,
};

// Host-only controller motion state. This intentionally does not extend any
// Xbox X_INPUT structure or otherwise change the guest ABI.
struct MotionState {
  uint64_t device_generation = 0;
  uint32_t available_sensors = kMotionSensorNone;
  uint32_t valid_samples = kMotionSensorNone;

  std::array<float, 3> acceleration_m_s2 = {};
  std::array<float, 3> angular_velocity_rad_s = {};

  float accelerometer_rate_hz = 0.0f;
  float gyroscope_rate_hz = 0.0f;

  uint64_t accelerometer_host_timestamp_ns = 0;
  uint64_t accelerometer_sensor_timestamp_ns = 0;
  uint64_t gyroscope_host_timestamp_ns = 0;
  uint64_t gyroscope_sensor_timestamp_ns = 0;
  uint64_t sequence = 0;
};

}  // namespace rex::input
