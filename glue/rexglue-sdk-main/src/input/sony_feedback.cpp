#include <rex/input/sony_feedback.h>

#include <algorithm>
#include <cmath>

namespace rex::input::sony {
namespace {
constexpr uint64_t kLeaseMs = 250;
constexpr uint64_t kGestureExpiryMs = 500;
constexpr uint64_t kSwipeWindowMs = 700;
constexpr uint64_t kCinematicHoldMs = 300;
constexpr std::array<uint64_t, 3> kPulseMs{80, 150, 300};

float Unit(float value) {
  return std::isfinite(value) ? std::clamp(value, 0.0f, 1.0f) : 0.0f;
}
uint16_t Motor(float value) { return static_cast<uint16_t>(Unit(value) * 65535.0f); }
uint8_t Force(float value) { return static_cast<uint8_t>(Unit(value) * 110.0f); }
}  // namespace

// These basic modes and byte layouts are exercised by the pinned SDL
// test/testcontroller.c CyclePS5TriggerEffect. SDL adds transport framing/CRC.
Trigger Resistance(uint8_t position, uint8_t strength) {
  Trigger result{};
  result[0] = strength ? 0x01 : 0x05;
  if (strength) {
    result[1] = position;
    result[2] = std::min<uint8_t>(strength, 110);
  }
  return result;
}

Trigger Recoil(uint8_t strength) {
  if (!strength) return Resistance(0, 0);
  Trigger result{};
  result[0] = 0x06;
  result[1] = 15;
  result[2] = std::min<uint8_t>(strength, 63);
  result[3] = 128;
  return result;
}

EffectPacket EncodeTriggers(const Trigger& left, const Trigger& right) {
  EffectPacket result{};
  result[0] = 0x0C;
  std::copy(right.begin(), right.end(), result.begin() + 10);
  std::copy(left.begin(), left.end(), result.begin() + 21);
  return result;
}

Service& GetService() {
  static Service service;
  return service;
}

bool Service::Fresh(const Slot& slot, uint64_t now_ms) {
  return slot.device.model != Model::kNone && slot.device.generation && slot.published &&
         now_ms >= slot.published_ms && now_ms - slot.published_ms <= kLeaseMs;
}

void Service::Cancel(Slot& slot) {
  CancelGestures(slot);
  slot.pulses = {};
}

void Service::CancelGestures(Slot& slot) {
  slot.pending.clear();
  for (auto& finger : slot.fingers) finger.cancelled = finger.down;
}

Service::Slot* Service::Find(uint32_t instance_id) {
  if (!instance_id) return nullptr;
  for (auto& slot : slots_) {
    if (slot.device.instance_id == instance_id) return &slot;
  }
  return nullptr;
}

void Service::Enqueue(Slot& slot, Gesture gesture, uint64_t now_ms) {
  if (slot.pending.size() < 16) slot.pending.push_back({gesture, now_ms});
}

void Service::Connect(uint32_t user, Device device) {
  std::lock_guard lock(mutex_);
  if (user >= slots_.size()) return;
  slots_[user] = {};
  if (device.model != Model::kNone && device.instance_id && device.generation) {
    device.triggers = device.triggers && device.model == Model::kDualSense;
    slots_[user].device = device;
  }
}

void Service::Disconnect(uint32_t instance_id) {
  std::lock_guard lock(mutex_);
  if (auto* slot = Find(instance_id)) *slot = {};
}

void Service::SetFocused(bool focused) {
  std::lock_guard lock(mutex_);
  focused_ = focused;
  if (!focused) {
    for (auto& slot : slots_) {
      Cancel(slot);
      slot.published = false;
    }
  }
}

void Service::UpdateAxes(uint32_t user, uint8_t left, uint8_t right) {
  std::lock_guard lock(mutex_);
  if (user >= slots_.size()) return;
  slots_[user].device.left_trigger = left;
  slots_[user].device.right_trigger = right;
}

void Service::DisableTriggers(uint32_t user, uint64_t generation) {
  std::lock_guard lock(mutex_);
  if (user < slots_.size() && slots_[user].device.generation == generation)
    slots_[user].device.triggers = false;
}

Device Service::ReadDevice(uint32_t user) const {
  std::lock_guard lock(mutex_);
  return user < slots_.size() ? slots_[user].device : Device{};
}

void Service::ObserveTouch(uint32_t instance_id, int finger_index, ContactPhase phase,
                           float x, float y, uint64_t now_ms) {
  std::lock_guard lock(mutex_);
  auto* slot = Find(instance_id);
  if (!slot || !slot->device.touchpad || finger_index < 0 || finger_index >= 2) return;
  auto& finger = slot->fingers[static_cast<size_t>(finger_index)];
  const bool valid = std::isfinite(x) && std::isfinite(y) && x >= 0 && x <= 1 && y >= 0 && y <= 1;
  const bool owned = focused_ && Fresh(*slot, now_ms) && slot->game.active && slot->game.gestures_owned;
  if (phase == ContactPhase::kDown) {
    finger = {true, !owned || !valid, x, y, now_ms};
    if (slot->fingers[0].down && slot->fingers[1].down && !slot->multi_touch) {
      slot->multi_touch = true;
      slot->multi_started_ms = now_ms;
    }
  } else if (phase == ContactPhase::kUp) {
    if (finger.down && !finger.cancelled && !slot->multi_touch && owned && valid &&
        now_ms >= finger.started_ms && now_ms - finger.started_ms <= kSwipeWindowMs) {
      const float dx = x - finger.start_x, dy = y - finger.start_y;
      if (std::abs(dx) >= 0.20f && std::abs(dx) >= std::abs(dy) * 1.5f) {
        Enqueue(*slot, dx > 0 ? Gesture::kNextRadio : Gesture::kPreviousRadio, now_ms);
      } else if (std::abs(dy) >= 0.20f && std::abs(dy) >= std::abs(dx) * 1.5f) {
        Enqueue(*slot, dy > 0 ? Gesture::kNextWeapon : Gesture::kPreviousWeapon, now_ms);
      }
    }
    finger = {};
    if (!slot->fingers[0].down && !slot->fingers[1].down) slot->multi_touch = false;
  } else if (!owned || !valid) {
    finger.cancelled = true;
  }
}

void Service::ObserveClick(uint32_t instance_id, bool down, uint64_t now_ms) {
  std::lock_guard lock(mutex_);
  auto* slot = Find(instance_id);
  if (!slot || !slot->device.touchpad) return;
  if (down && !slot->click_down) {
    slot->click_owned = focused_ && Fresh(*slot, now_ms) && slot->game.active && slot->game.gestures_owned;
    if (slot->click_owned && !slot->multi_touch) Enqueue(*slot, Gesture::kCamera, now_ms);
  }
  slot->click_down = down;
  if (!down) slot->click_owned = false;
}

bool Service::ClaimsClick(uint32_t user, uint64_t now_ms, const Options& options) const {
  std::lock_guard lock(mutex_);
  if (user >= slots_.size()) return false;
  const auto& slot = slots_[user];
  if (slot.click_down) return slot.click_owned;
  return options.enabled && options.gestures && focused_ && Fresh(slot, now_ms) &&
         slot.game.active && slot.game.gestures_owned && slot.device.touchpad;
}

bool Service::OwnsGestures(uint32_t user, uint64_t now_ms, const Options& options) const {
  std::lock_guard lock(mutex_);
  if (user >= slots_.size()) return false;
  const auto& slot = slots_[user];
  return options.enabled && options.gestures && focused_ && Fresh(slot, now_ms) &&
         slot.game.active && slot.game.gestures_owned && slot.device.touchpad;
}

Input Service::Consume(uint32_t user, uint64_t now_ms, const Options& options) {
  std::lock_guard lock(mutex_);
  Input input{};
  if (user >= slots_.size()) return input;
  auto& slot = slots_[user];
  input.device = slot.device;
  if (!options.enabled || !focused_ || !Fresh(slot, now_ms) ||
      !slot.game.active || !slot.game.gestures_owned) {
    if (!options.enabled || !focused_ || !Fresh(slot, now_ms) || !slot.game.active)
      Cancel(slot);
    else
      CancelGestures(slot);
    return input;
  }
  if (!options.gestures) {
    CancelGestures(slot);
    return input;
  }
  input.gestures_owned = slot.device.touchpad;
  for (const auto& gesture : slot.pending) {
    if (now_ms >= gesture.timestamp && now_ms - gesture.timestamp <= kGestureExpiryMs)
      input.gestures.push_back(gesture.gesture);
  }
  slot.pending.clear();
  input.cinematic = slot.game.in_vehicle && slot.multi_touch &&
                    slot.fingers[0].down && slot.fingers[1].down &&
                    !slot.fingers[0].cancelled && !slot.fingers[1].cancelled &&
                    now_ms >= slot.multi_started_ms && now_ms - slot.multi_started_ms >= kCinematicHoldMs;
  return input;
}

void Service::Publish(uint32_t user, uint64_t generation, const GameState& state, uint64_t now_ms) {
  std::lock_guard lock(mutex_);
  if (user >= slots_.size() || !generation || slots_[user].device.generation != generation) return;
  auto& slot = slots_[user];
  const bool identity_changed = slot.game.player_identity != state.player_identity ||
                                slot.game.context_identity != state.context_identity;
  if (!focused_ || !state.active || identity_changed ||
      (slot.game.gestures_owned && !state.gestures_owned) || !Fresh(slot, now_ms)) Cancel(slot);
  if (identity_changed || (slot.game.wanted <= 0 && state.wanted > 0)) slot.wanted_started_ms = now_ms;
  slot.game = state;
  slot.published_ms = now_ms;
  slot.published = focused_;
}

void Service::Emit(uint32_t user, uint64_t generation, Event event, float strength, uint64_t now_ms) {
  std::lock_guard lock(mutex_);
  const auto index = static_cast<size_t>(event);
  if (user >= slots_.size() || index >= kPulseMs.size() || !generation) return;
  auto& slot = slots_[user];
  if (!focused_ || slot.device.generation != generation || !Fresh(slot, now_ms) || !slot.game.active) return;
  auto& pulse = slot.pulses[index];
  const float previous = now_ms < pulse.until_ms ? pulse.strength : 0.0f;
  pulse = {now_ms + kPulseMs[index], std::max(previous, Unit(strength))};
}

Output Service::Compose(uint32_t user, uint64_t now_ms, const Options& options) const {
  std::lock_guard lock(mutex_);
  Output output{};
  output.left = output.right = Resistance(0, 0);
  if (user >= slots_.size()) return output;
  const auto& slot = slots_[user];
  output.suppress_native = !focused_ || (slot.published && (!Fresh(slot, now_ms) || !slot.game.active));
  if (!options.enabled || !focused_ || !Fresh(slot, now_ms) || !slot.game.active) return output;
  output.active = true;
  const float intensity = Unit(options.intensity);
  output.lighting = options.lighting && slot.device.light;
  if (slot.game.wanted > 0 && now_ms >= slot.wanted_started_ms) {
    output.color = ((now_ms - slot.wanted_started_ms) / 250) % 2 == 0
                       ? std::array<uint8_t, 3>{255, 0, 0} : std::array<uint8_t, 3>{0, 0, 255};
  } else {
    output.color = slot.game.episode == 1 ? std::array<uint8_t, 3>{255, 128, 0}
                   : slot.game.episode == 2 ? std::array<uint8_t, 3>{128, 0, 255}
                                           : std::array<uint8_t, 3>{0, 0, 255};
  }
  auto pulse = [&](Event event) {
    const auto& value = slot.pulses[static_cast<size_t>(event)];
    return now_ms < value.until_ms ? value.strength : 0.0f;
  };
  const float shot = pulse(Event::kShot), damage = pulse(Event::kDamage), explosion = pulse(Event::kExplosion);
  if (options.rumble && slot.device.rumble) {
    output.low_motor = Motor(std::max({shot * 0.35f, damage * 0.75f, explosion}) * intensity);
    output.high_motor = Motor(std::max({shot, damage * 0.5f, explosion * 0.65f}) * intensity);
  }
  if (options.triggers && slot.device.triggers && intensity > 0) {
    if (slot.game.trigger_context == TriggerContext::kVehicle && slot.game.driver) {
      output.left = Resistance(20, Force(intensity * (0.25f + 0.5f * slot.device.left_trigger / 255.0f)));
      output.right = Resistance(20, Force(intensity * (0.25f + 0.5f * slot.device.right_trigger / 255.0f)));
    } else if (slot.game.trigger_context == TriggerContext::kWeapon) {
      if (slot.game.aiming) output.left = Resistance(30, Force(intensity * 0.5f));
      if (slot.game.can_fire) output.right = Resistance(90, Force(intensity));
      if (shot > 0) output.right = Recoil(static_cast<uint8_t>(63.0f * intensity * shot));
    }
  }
  return output;
}

}  // namespace rex::input::sony
