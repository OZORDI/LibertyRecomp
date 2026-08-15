#include "gta4_motion_bridge.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numbers>

#include <rex/cvar.h>
#include <rex/input/input_system.h>
#include <rex/input/motion.h>
#include <rex/logging.h>
#include <rex/runtime.h>

REXCVAR_DEFINE_BOOL(gta4_motion_enabled, true, "GTA IV/Motion Sensor",
                    "Enable GTA IV motion controls when the active gamepad has a sensor");
REXCVAR_DEFINE_BOOL(gta4_motion_heli, true, "GTA IV/Motion Sensor",
                    "Enable helicopter pitch and bank motion controls");
REXCVAR_DEFINE_BOOL(gta4_motion_bike, true, "GTA IV/Motion Sensor",
                    "Enable bike steering, wheelie and stoppie motion controls");
REXCVAR_DEFINE_BOOL(gta4_motion_boat, true, "GTA IV/Motion Sensor",
                    "Enable boat steering and trim motion controls");
REXCVAR_DEFINE_BOOL(gta4_motion_aftertouch, true, "GTA IV/Motion Sensor",
                    "Enable airborne vehicle after-touch motion controls");
REXCVAR_DEFINE_BOOL(gta4_motion_reload, true, "GTA IV/Motion Sensor",
                    "Enable the ordered pitch-up then pitch-down reload gesture");
REXCVAR_DEFINE_BOOL(gta4_motion_activity, true, "GTA IV/Motion Sensor",
                    "Expose motion controls to activities such as darts and bowling");
REXCVAR_DEFINE_BOOL(gta4_motion_calibration, true, "GTA IV/Motion Sensor",
                    "Automatically capture neutral pitch when entering a vehicle");

REXCVAR_DEFINE_DOUBLE(gta4_motion_filter_cutoff_hz, 5.0, "GTA IV/Motion Sensor/Tuning",
                      "Accelerometer low-pass cutoff in Hz")
    .range(0.1, 30.0);
REXCVAR_DEFINE_DOUBLE(gta4_motion_axis_deadzone_degrees, 2.5, "GTA IV/Motion Sensor/Tuning",
                      "Angular motion deadzone in degrees")
    .range(0.0, 20.0);
REXCVAR_DEFINE_DOUBLE(gta4_motion_axis_full_scale_degrees, 25.0, "GTA IV/Motion Sensor/Tuning",
                      "Controller tilt that produces full normalized input")
    .range(5.0, 90.0);
REXCVAR_DEFINE_INT32(gta4_motion_stale_timeout_ms, 250, "GTA IV/Motion Sensor/Tuning",
                     "Reject motion after this interval without an accelerometer sample")
    .range(50, 2000);
REXCVAR_DEFINE_BOOL(gta4_motion_invert_pitch, false, "GTA IV/Motion Sensor/Tuning",
                    "Invert the pitch motion axis");
REXCVAR_DEFINE_BOOL(gta4_motion_invert_roll, false, "GTA IV/Motion Sensor/Tuning",
                    "Invert the roll motion axis");
REXCVAR_DEFINE_DOUBLE(gta4_motion_reload_up_degrees, 25.0, "GTA IV/Motion Sensor/Tuning",
                      "Positive pitch required for the first reload gesture phase")
    .range(5.0, 80.0);
REXCVAR_DEFINE_DOUBLE(gta4_motion_reload_down_degrees, -10.0, "GTA IV/Motion Sensor/Tuning",
                      "Negative pitch required to complete the reload gesture")
    .range(-80.0, -1.0);
REXCVAR_DEFINE_DOUBLE(gta4_motion_reload_neutral_degrees, 7.5, "GTA IV/Motion Sensor/Tuning",
                      "Neutral range required before accepting a new reload gesture")
    .range(1.0, 30.0);
REXCVAR_DEFINE_INT32(gta4_motion_reload_window_ms, 800, "GTA IV/Motion Sensor/Tuning",
                     "Maximum time between reload gesture phases")
    .range(100, 3000);
REXCVAR_DEFINE_INT32(gta4_motion_reload_cooldown_ms, 600, "GTA IV/Motion Sensor/Tuning",
                     "Minimum time before another reload gesture")
    .range(100, 3000);

namespace gta4 {
namespace {

constexpr double kDegreesInHalfTurn = 180.0;

double DegreesToRadians(double degrees) {
  return degrees * std::numbers::pi / kDegreesInHalfTurn;
}

std::chrono::milliseconds MillisecondsFromCVar(int32_t value) {
  return std::chrono::milliseconds(value);
}

bool IsFiniteVector(const std::array<float, 3>& value) {
  return std::ranges::all_of(value, [](float component) { return std::isfinite(component); });
}

}  // namespace

GTA4MotionBridge& GTA4MotionBridge::Get() {
  static GTA4MotionBridge bridge;
  return bridge;
}

MotionSnapshot GTA4MotionBridge::Read(uint32_t user_index) {
  std::lock_guard lock(mutex_);
  UpdateLocked(user_index);
  return snapshot_;
}

bool GTA4MotionBridge::IsPreferenceEnabled(MotionPreference preference) const {
  if (!REXCVAR_GET(gta4_motion_enabled)) {
    return false;
  }
  switch (preference) {
    case MotionPreference::kHelicopter:
      return REXCVAR_GET(gta4_motion_heli);
    case MotionPreference::kBike:
      return REXCVAR_GET(gta4_motion_bike);
    case MotionPreference::kBoat:
      return REXCVAR_GET(gta4_motion_boat);
    case MotionPreference::kAftertouch:
      return REXCVAR_GET(gta4_motion_aftertouch);
    case MotionPreference::kReload:
      return REXCVAR_GET(gta4_motion_reload);
    case MotionPreference::kActivity:
      return REXCVAR_GET(gta4_motion_activity);
    case MotionPreference::kCalibration:
      return REXCVAR_GET(gta4_motion_calibration);
    case MotionPreference::kCount:
      return false;
  }
  return false;
}

bool GTA4MotionBridge::IsPreferenceEnabled(uint32_t preference) const {
  if (preference >= static_cast<uint32_t>(MotionPreference::kCount)) {
    return false;
  }
  return IsPreferenceEnabled(static_cast<MotionPreference>(preference));
}

void GTA4MotionBridge::SetAllPreferences(bool enabled) {
  REXCVAR_SET(gta4_motion_heli, enabled);
  REXCVAR_SET(gta4_motion_bike, enabled);
  REXCVAR_SET(gta4_motion_boat, enabled);
  REXCVAR_SET(gta4_motion_aftertouch, enabled);
  REXCVAR_SET(gta4_motion_reload, enabled);
  REXCVAR_SET(gta4_motion_activity, enabled);
  REXCVAR_SET(gta4_motion_calibration, enabled);
}

void GTA4MotionBridge::Calibrate(uint32_t user_index) {
  std::lock_guard lock(mutex_);
  UpdateLocked(user_index);
  if (snapshot_.fresh) {
    neutral_pitch_radians_ += snapshot_.pitch_radians;
    neutral_roll_radians_ += snapshot_.roll_radians;
    snapshot_.pitch_radians = 0.0f;
    snapshot_.roll_radians = 0.0f;
    snapshot_.pitch_axis = 0.0f;
    snapshot_.roll_axis = 0.0f;
    calibrated_ = true;
    calibration_pending_ = false;
  } else {
    calibration_pending_ = true;
  }
}

void GTA4MotionBridge::NotifyVehicleEntry(uint32_t user_index) {
  if (IsPreferenceEnabled(MotionPreference::kCalibration)) {
    Calibrate(user_index);
  }
}

bool GTA4MotionBridge::ConsumeReloadGesture(MotionReloadConsumer consumer, uint32_t user_index) {
  std::lock_guard lock(mutex_);
  UpdateLocked(user_index);
  const size_t index = static_cast<size_t>(consumer);
  if (index >= reload_completed_.size()) {
    return false;
  }
  const bool completed = reload_completed_[index];
  reload_completed_[index] = false;
  return completed;
}

void GTA4MotionBridge::ResetDeviceLocked() {
  snapshot_ = {};
  filtered_acceleration_ = {};
  device_generation_ = 0;
  last_accelerometer_timestamp_ns_ = 0;
  last_sample_seen_ = {};
  filter_initialized_ = false;
  calibrated_ = false;
  calibration_pending_ = false;
  neutral_pitch_radians_ = 0.0f;
  neutral_roll_radians_ = 0.0f;
  reload_phase_ = ReloadPhase::kIdle;
  reload_deadline_ = {};
  reload_completed_.fill(false);
}

void GTA4MotionBridge::UpdateLocked(uint32_t user_index) {
  auto* runtime = rex::Runtime::instance();
  auto* input_system =
      runtime ? static_cast<rex::input::InputSystem*>(runtime->input_system()) : nullptr;
  rex::input::MotionState motion = {};
  const auto now = std::chrono::steady_clock::now();
  if (!input_system || !input_system->TryGetMotionState(user_index, &motion) ||
      !(motion.valid_samples & rex::input::kMotionSensorAccelerometer) ||
      !IsFiniteVector(motion.acceleration_m_s2)) {
    if (snapshot_.sensor_available) {
      ResetDeviceLocked();
    }
    return;
  }

  if (motion.device_generation != device_generation_) {
    ResetDeviceLocked();
    device_generation_ = motion.device_generation;
  }

  const auto stale_timeout = MillisecondsFromCVar(REXCVAR_GET(gta4_motion_stale_timeout_ms));
  const bool new_sample =
      motion.accelerometer_host_timestamp_ns != last_accelerometer_timestamp_ns_;
  if (!new_sample) {
    snapshot_.sequence = motion.sequence;
    if (motion.valid_samples & rex::input::kMotionSensorGyroscope) {
      snapshot_.angular_velocity_rad_s = motion.angular_velocity_rad_s;
    }
    snapshot_.fresh = last_sample_seen_ != std::chrono::steady_clock::time_point{} &&
                      now - last_sample_seen_ <= stale_timeout;
    snapshot_.controls_enabled = snapshot_.fresh && REXCVAR_GET(gta4_motion_enabled);
    if (!snapshot_.fresh) {
      snapshot_.pitch_axis = 0.0f;
      snapshot_.roll_axis = 0.0f;
      reload_phase_ = ReloadPhase::kIdle;
    }
    return;
  }

  const bool discontinuity =
      !filter_initialized_ ||
      motion.accelerometer_host_timestamp_ns <= last_accelerometer_timestamp_ns_ ||
      (last_sample_seen_ != std::chrono::steady_clock::time_point{} &&
       now - last_sample_seen_ > stale_timeout);

  const auto& acceleration = motion.acceleration_m_s2;
  const double magnitude = std::sqrt(static_cast<double>(acceleration[0]) * acceleration[0] +
                                     static_cast<double>(acceleration[1]) * acceleration[1] +
                                     static_cast<double>(acceleration[2]) * acceleration[2]);
  if (!std::isfinite(magnitude) || magnitude <= std::numeric_limits<double>::epsilon()) {
    snapshot_.fresh = false;
    snapshot_.controls_enabled = false;
    snapshot_.pitch_axis = 0.0f;
    snapshot_.roll_axis = 0.0f;
    filter_initialized_ = false;
    reload_phase_ = ReloadPhase::kIdle;
    last_accelerometer_timestamp_ns_ = motion.accelerometer_host_timestamp_ns;
    last_sample_seen_ = {};
    return;
  }

  std::array<float, 3> normalized = {};
  std::ranges::transform(acceleration, normalized.begin(),
                         [magnitude](float component) { return component / magnitude; });

  if (discontinuity) {
    filtered_acceleration_ = normalized;
    filter_initialized_ = true;
    calibrated_ = false;
    reload_phase_ = ReloadPhase::kIdle;
  } else {
    const auto elapsed_ns = std::chrono::nanoseconds(motion.accelerometer_host_timestamp_ns -
                                                     last_accelerometer_timestamp_ns_);
    const double elapsed_seconds = std::chrono::duration<double>(elapsed_ns).count();
    const double cutoff_hz = REXCVAR_GET(gta4_motion_filter_cutoff_hz);
    const double alpha =
        -std::expm1(-std::numbers::pi * static_cast<double>(2) * cutoff_hz * elapsed_seconds);
    for (size_t index = 0; index < filtered_acceleration_.size(); ++index) {
      filtered_acceleration_[index] =
          static_cast<float>(filtered_acceleration_[index] +
                             alpha * (normalized[index] - filtered_acceleration_[index]));
    }
  }

  const float absolute_pitch = std::atan2(-filtered_acceleration_[2], filtered_acceleration_[1]);
  const float absolute_roll = std::atan2(filtered_acceleration_[0], filtered_acceleration_[1]);
  if (!calibrated_ || calibration_pending_) {
    neutral_pitch_radians_ = absolute_pitch;
    neutral_roll_radians_ = absolute_roll;
    calibrated_ = true;
    calibration_pending_ = false;
  }

  snapshot_.sensor_available = true;
  snapshot_.fresh = true;
  snapshot_.controls_enabled = REXCVAR_GET(gta4_motion_enabled);
  snapshot_.sequence = motion.sequence;
  snapshot_.acceleration_m_s2 = acceleration;
  if (motion.valid_samples & rex::input::kMotionSensorGyroscope) {
    snapshot_.angular_velocity_rad_s = motion.angular_velocity_rad_s;
  } else {
    snapshot_.angular_velocity_rad_s = {};
  }
  snapshot_.pitch_radians = static_cast<float>(
      std::remainder(absolute_pitch - neutral_pitch_radians_, std::numbers::pi * 2.0));
  snapshot_.roll_radians = static_cast<float>(
      std::remainder(absolute_roll - neutral_roll_radians_, std::numbers::pi * 2.0));
  snapshot_.pitch_axis =
      ApplyAxisCurve(snapshot_.pitch_radians, REXCVAR_GET(gta4_motion_invert_pitch));
  snapshot_.roll_axis =
      ApplyAxisCurve(snapshot_.roll_radians, REXCVAR_GET(gta4_motion_invert_roll));

  last_accelerometer_timestamp_ns_ = motion.accelerometer_host_timestamp_ns;
  last_sample_seen_ = now;
  UpdateReloadLocked(now);
}

float GTA4MotionBridge::ApplyAxisCurve(float angle_radians, bool invert) const {
  const double deadzone = DegreesToRadians(REXCVAR_GET(gta4_motion_axis_deadzone_degrees));
  const double full_scale = DegreesToRadians(REXCVAR_GET(gta4_motion_axis_full_scale_degrees));
  const double magnitude = std::abs(static_cast<double>(angle_radians));
  if (magnitude <= deadzone || full_scale <= deadzone) {
    return 0.0f;
  }
  double value = std::clamp((magnitude - deadzone) / (full_scale - deadzone), 0.0, 1.0);
  value = std::copysign(value, angle_radians);
  if (invert) {
    value = -value;
  }
  return static_cast<float>(value);
}

void GTA4MotionBridge::UpdateReloadLocked(std::chrono::steady_clock::time_point now) {
  if (!snapshot_.controls_enabled || !IsPreferenceEnabled(MotionPreference::kReload)) {
    reload_phase_ = ReloadPhase::kIdle;
    reload_completed_.fill(false);
    return;
  }

  const double pitch = snapshot_.pitch_radians;
  const double up = DegreesToRadians(REXCVAR_GET(gta4_motion_reload_up_degrees));
  const double down = DegreesToRadians(REXCVAR_GET(gta4_motion_reload_down_degrees));
  const double neutral = DegreesToRadians(REXCVAR_GET(gta4_motion_reload_neutral_degrees));

  switch (reload_phase_) {
    case ReloadPhase::kIdle:
      if (pitch >= up) {
        reload_phase_ = ReloadPhase::kAwaitingDown;
        reload_deadline_ = now + MillisecondsFromCVar(REXCVAR_GET(gta4_motion_reload_window_ms));
      }
      break;
    case ReloadPhase::kAwaitingDown:
      if (now > reload_deadline_) {
        reload_phase_ = ReloadPhase::kIdle;
      } else if (pitch <= down) {
        reload_completed_.fill(true);
        reload_phase_ = ReloadPhase::kCooldown;
        reload_deadline_ = now + MillisecondsFromCVar(REXCVAR_GET(gta4_motion_reload_cooldown_ms));
      }
      break;
    case ReloadPhase::kCooldown:
      if (now >= reload_deadline_ && std::abs(pitch) <= neutral) {
        reload_phase_ = ReloadPhase::kIdle;
      }
      break;
  }
}

}  // namespace gta4
