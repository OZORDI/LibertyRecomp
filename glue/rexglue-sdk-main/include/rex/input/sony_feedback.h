#pragma once

#include <array>
#include <cstdint>
#include <deque>
#include <mutex>
#include <vector>

namespace rex::input::sony {

enum class Model { kNone, kDualShock4, kDualSense };
enum class Gesture { kCamera, kNextWeapon, kPreviousWeapon, kNextRadio, kPreviousRadio };
enum class ContactPhase { kDown, kMove, kUp };
enum class Event { kShot, kDamage, kExplosion };
enum class TriggerContext { kNone, kWeapon, kVehicle };

struct Options {
  bool enabled = true;
  bool gestures = true;
  bool lighting = true;
  bool rumble = true;
  bool triggers = true;
  float intensity = 0.5f;
};
Options GetOptions();
// Frontend master toggle also resets former per-feature switches so enabling
// the bundle cannot leave a feature hidden behind an old saved Off value.
bool SetFeaturesEnabled(bool enabled);

struct Device {
  uint32_t instance_id = 0;
  uint64_t generation = 0;
  Model model = Model::kNone;
  bool touchpad = false;
  bool light = false;
  bool rumble = false;
  bool triggers = false;
  uint8_t left_trigger = 0;
  uint8_t right_trigger = 0;
};

struct Input {
  Device device{};
  std::vector<Gesture> gestures;
  bool gestures_owned = false;
  bool cinematic = false;
};

// Values copied on a guest thread. No guest pointer or SDL handle crosses this interface.
struct GameState {
  bool active = false;
  bool gestures_owned = false;
  bool in_vehicle = false;
  bool driver = false;
  bool aiming = false;
  bool can_fire = false;
  int episode = 0;
  int wanted = 0;
  TriggerContext trigger_context = TriggerContext::kNone;
  uint32_t player_identity = 0;
  uint32_t context_identity = 0;
};

using Trigger = std::array<uint8_t, 11>;
using EffectPacket = std::array<uint8_t, 47>;
struct Output {
  bool active = false;
  bool suppress_native = false;
  bool lighting = false;
  std::array<uint8_t, 3> color{};
  uint16_t low_motor = 0;
  uint16_t high_motor = 0;
  Trigger left{};
  Trigger right{};
};

Trigger Resistance(uint8_t position, uint8_t strength);
Trigger Recoil(uint8_t strength);
EffectPacket EncodeTriggers(const Trigger& left, const Trigger& right);

class Service {
 public:
  void Connect(uint32_t user, Device device);
  void Disconnect(uint32_t instance_id);
  void SetFocused(bool focused);
  void UpdateAxes(uint32_t user, uint8_t left, uint8_t right);
  void DisableTriggers(uint32_t user, uint64_t generation);
  Device ReadDevice(uint32_t user) const;
  void ObserveTouch(uint32_t instance_id, int finger, ContactPhase phase,
                    float x, float y, uint64_t now_ms);
  void ObserveClick(uint32_t instance_id, bool down, uint64_t now_ms);
  bool ClaimsClick(uint32_t user, uint64_t now_ms, const Options& options) const;
  bool OwnsGestures(uint32_t user, uint64_t now_ms, const Options& options) const;
  Input Consume(uint32_t user, uint64_t now_ms, const Options& options);
  void Publish(uint32_t user, uint64_t generation, const GameState& state, uint64_t now_ms);
  void Emit(uint32_t user, uint64_t generation, Event event, float strength, uint64_t now_ms);
  Output Compose(uint32_t user, uint64_t now_ms, const Options& options) const;

 private:
  struct Finger {
    bool down = false;
    bool cancelled = false;
    float start_x = 0, start_y = 0;
    uint64_t started_ms = 0;
  };
  struct PendingGesture { Gesture gesture; uint64_t timestamp; };
  struct Pulse { uint64_t until_ms = 0; float strength = 0; };
  struct Slot {
    Device device{};
    GameState game{};
    uint64_t published_ms = 0;
    bool published = false;
    uint64_t wanted_started_ms = 0;
    std::array<Finger, 2> fingers{};
    std::deque<PendingGesture> pending;
    bool click_down = false;
    bool click_owned = false;
    bool multi_touch = false;
    uint64_t multi_started_ms = 0;
    std::array<Pulse, 3> pulses{};
  };
  static bool Fresh(const Slot& slot, uint64_t now_ms);
  static void Cancel(Slot& slot);
  static void CancelGestures(Slot& slot);
  Slot* Find(uint32_t instance_id);
  static void Enqueue(Slot& slot, Gesture gesture, uint64_t now_ms);
  mutable std::mutex mutex_;
  std::array<Slot, 4> slots_{};
  bool focused_ = true;
};

Service& GetService();

}  // namespace rex::input::sony
