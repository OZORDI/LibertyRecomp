#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>

#include <rex/input/mnk/encoded_action.h>

namespace gta4 {

enum class VehicleMotionKind : uint32_t { kHelicopter, kBike, kBoat, kAutomobile };

struct VehicleMotionActions {
  int32_t lateral = 0;
  int32_t pitch = 0;
  int32_t pedals = 0;
};

inline int32_t QuantizeMotionAxis(float axis) noexcept {
  return std::isfinite(axis)
             ? static_cast<int32_t>(std::lround(std::clamp(axis, -1.0f, 1.0f) * 255.0f)) : 0;
}

inline VehicleMotionActions BuildVehicleMotionActions(VehicleMotionKind kind, float roll,
                                                       float pitch, bool airborne) noexcept {
  if (kind == VehicleMotionKind::kAutomobile && !airborne) {
    return {};
  }
  // Generated .12 sub_822ABEE0 and .44/.45 vehicle control consumers negate
  // the lateral stick. 30/31 drives bank/steering; 57/58 is separate heli yaw.
  const int32_t lateral = -QuantizeMotionAxis(roll);
  const int32_t longitudinal = QuantizeMotionAxis(pitch);
  return kind == VehicleMotionKind::kAutomobile
             ? VehicleMotionActions{lateral, 0, longitudinal}
             : VehicleMotionActions{lateral, longitudinal, 0};
}

// Feed the original game routine its motion-augmented actions BEFORE it applies
// gates, smoothing, limits and derives dependent steering fields. Restore only
// the bytes we changed, and only if the routine has not replaced them itself.
// Read/write callbacks preserve the generated guest-memory translation rules.
template <typename ReadByte, typename WriteByte>
class ScopedMotionActions {
 public:
  ScopedMotionActions(uint32_t control, VehicleMotionActions actions,
                      ReadByte read, WriteByte write)
      : control_(control), read_(read), write_(write) {
    if (!control_) {
      return;
    }
    MergePair(30, 31, actions.lateral);
    MergePair(32, 33, actions.pitch);
    MergePedals(actions.pedals);
  }
  ScopedMotionActions(const ScopedMotionActions&) = delete;
  ScopedMotionActions& operator=(const ScopedMotionActions&) = delete;

  ~ScopedMotionActions() {
    for (size_t index = count_; index > 0; --index) {
      const auto& saved = saved_[index - 1];
      if (read_(saved.address) == saved.applied &&
          read_(saved.address - 2) == saved.polarity) {
        write_(saved.address, saved.before);
      }
    }
  }
  bool changed() const noexcept { return count_ != 0; }

 private:
  uint32_t Record(uint32_t action) const noexcept { return control_ + 2328 + action * 12; }
  void Store(uint32_t record, uint8_t encoded) {
    const uint8_t before = read_(record + 2);
    if (before == encoded) {
      return;
    }
    saved_[count_++] = {record + 2, before, encoded, read_(record)};
    write_(record + 2, encoded);
  }
  void MergePair(uint32_t negative, uint32_t positive, int32_t requested) {
    if (!requested) {
      return;
    }
    const uint32_t n = Record(negative), p = Record(positive);
    const auto merge = rex::input::mnk::MergeSignedActionPair(
        read_(n), read_(n + 2), read_(p), read_(p + 2), requested);
    if (merge.changed) {
      Store(n, merge.negative_encoded);
      Store(p, merge.positive_encoded);
    }
  }
  void MergePedals(int32_t requested) {
    if (!requested) {
      return;
    }
    const uint32_t gas = Record(40), brake = Record(41);
    const auto current_gas = rex::input::mnk::DecodeActionMagnitude(read_(gas), read_(gas + 2));
    const auto current_brake = rex::input::mnk::DecodeActionMagnitude(read_(brake), read_(brake + 2));
    const uint8_t magnitude = static_cast<uint8_t>(std::min(std::abs(requested), 255));
    if (magnitude <= std::max(current_gas, current_brake)) {
      return;
    }
    Store(gas, rex::input::mnk::EncodeActionMagnitude(read_(gas), requested > 0 ? magnitude : 0));
    Store(brake, rex::input::mnk::EncodeActionMagnitude(read_(brake), requested < 0 ? magnitude : 0));
  }
  struct SavedByte {
    uint32_t address;
    uint8_t before, applied, polarity;
  };
  uint32_t control_;
  ReadByte read_;
  WriteByte write_;
  std::array<SavedByte, 6> saved_{};
  size_t count_ = 0;
};

}  // namespace gta4
