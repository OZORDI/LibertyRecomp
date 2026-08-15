#pragma once

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <mutex>

namespace gta4 {

enum class MotionPreference : uint32_t {
  kHelicopter = 0,
  kBike,
  kBoat,
  kAftertouch,
  kReload,
  kActivity,
  kCalibration,
  kCount,
};

enum class MotionReloadConsumer : uint32_t {
  kGameplay = 0,
  kScript,
  kCount,
};

struct MotionSnapshot {
  bool sensor_available = false;
  bool controls_enabled = false;
  bool fresh = false;
  uint64_t sequence = 0;

  std::array<float, 3> acceleration_m_s2 = {};
  std::array<float, 3> angular_velocity_rad_s = {};
  float pitch_radians = 0.0f;
  float roll_radians = 0.0f;
  float pitch_axis = 0.0f;
  float roll_axis = 0.0f;
};

class GTA4MotionBridge {
 public:
  static GTA4MotionBridge& Get();

  MotionSnapshot Read(uint32_t user_index = 0);
  bool IsPreferenceEnabled(MotionPreference preference) const;
  bool IsPreferenceEnabled(uint32_t preference) const;
  void SetAllPreferences(bool enabled);

  void Calibrate(uint32_t user_index = 0);
  void NotifyVehicleEntry(uint32_t user_index = 0);
  bool ConsumeReloadGesture(MotionReloadConsumer consumer, uint32_t user_index = 0);

 private:
  enum class ReloadPhase { kIdle, kAwaitingDown, kCooldown };

  GTA4MotionBridge() = default;

  void UpdateLocked(uint32_t user_index);
  void ResetDeviceLocked();
  void UpdateReloadLocked(std::chrono::steady_clock::time_point now);
  float ApplyAxisCurve(float angle_radians, bool invert) const;

  mutable std::mutex mutex_;
  MotionSnapshot snapshot_;
  std::array<float, 3> filtered_acceleration_ = {};
  uint64_t device_generation_ = 0;
  uint64_t last_accelerometer_timestamp_ns_ = 0;
  std::chrono::steady_clock::time_point last_sample_seen_ = {};

  bool filter_initialized_ = false;
  bool calibrated_ = false;
  bool calibration_pending_ = false;
  float neutral_pitch_radians_ = 0.0f;
  float neutral_roll_radians_ = 0.0f;

  ReloadPhase reload_phase_ = ReloadPhase::kIdle;
  std::chrono::steady_clock::time_point reload_deadline_ = {};
  std::array<bool, static_cast<size_t>(MotionReloadConsumer::kCount)> reload_completed_ = {};
};

}  // namespace gta4
