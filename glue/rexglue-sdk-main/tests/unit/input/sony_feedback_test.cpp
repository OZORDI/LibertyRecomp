// Standalone behavioral tests; tools/run_sony_feedback_tests.py generates the
// independent Python oracle and compiles this against the production service.
#include <rex/input/sony_feedback.h>

#include <array>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

#include "sony_feedback_oracle.h"

namespace {
using namespace rex::input::sony;
namespace oracle = sony_oracle;

void Require(bool condition, const char* message) {
  if (!condition) throw std::runtime_error(message);
}

Device Pad(Model model = Model::kDualSense, uint32_t id = 42,
           uint64_t generation = 7) {
  Device device{};
  device.instance_id = id;
  device.generation = generation;
  device.model = model;
  device.touchpad = device.light = device.rumble = device.triggers = true;
  return device;
}

GameState Playing() {
  GameState game{};
  game.active = game.gestures_owned = true;
  game.player_identity = 19;
  game.context_identity = 23;
  return game;
}

struct Fixture {
  Service service;
  Device device = Pad();
  GameState game = Playing();
  Options options{};

  Fixture() {
    service.Connect(0, device);
    Publish(oracle::kBase);
  }
  void Publish(uint64_t time) { service.Publish(0, device.generation, game, time); }
  Input Consume(uint64_t time) { return service.Consume(0, time, options); }
  Output Compose(uint64_t time) { return service.Compose(0, time, options); }
  void Touch(ContactPhase phase, float x, float y, uint64_t time, int finger = 0) {
    service.ObserveTouch(device.instance_id, finger, phase, x, y, time);
  }
  void Click(bool down, uint64_t time) {
    service.ObserveClick(device.instance_id, down, time);
  }
};

void RequireNeutral(const Output& output) {
  Require(!output.active && !output.lighting, "inactive output still claims lighting");
  Require(output.low_motor == 0 && output.high_motor == 0, "inactive output still rumbles");
  Require(output.left == oracle::kNeutral && output.right == oracle::kNeutral,
          "inactive output must explicitly release both triggers");
}

void RequireGesture(const Input& input, Gesture expected) {
  Require(input.gestures.size() == 1, "gesture must be delivered exactly once");
  Require(input.gestures.front() == expected, "gesture routed to incorrect action");
}

void EncoderGoldenVectors() {
  Require(Resistance(99, 0) == oracle::kNeutral, "zero resistance must encode neutral mode 0x05");
  Require(Resistance(45, 110) == oracle::kResistance, "continuous mode wire layout differs from SDL");
  Require(Resistance(45, 255) == oracle::kResistance, "resistance exceeds bounded force");
  Require(Recoil(0) == oracle::kNeutral, "zero recoil must release trigger");
  Require(Recoil(63) == oracle::kRecoil, "recoil wire order must be frequency, force, position");
  Require(Recoil(255) == oracle::kRecoil, "recoil exceeds bounded force");
  Require(EncodeTriggers(Resistance(45, 110), Recoil(63)) == oracle::kPacket,
          "effect state must be 47 bytes with right at 10, left at 21, mask 0x0c and zero padding");
  Require(EncodeTriggers(Resistance(0, 0), Recoil(0)) == oracle::kNeutralPacket,
          "neutral packet must release both triggers without a USB/BT envelope");
}

void DeviceAndCapabilityGates() {
  Service service;
  Options options{};
  auto game = Playing();
  service.Connect(0, Pad(Model::kNone));
  service.Publish(0, 7, game, oracle::kBase);
  service.ObserveClick(42, true, oracle::kT10);
  RequireNeutral(service.Compose(0, oracle::kT10, options));
  Require(service.Consume(0, oracle::kT10, options).gestures.empty(), "non-Sony accepted gesture");
  Require(!service.ClaimsClick(0, oracle::kT10, options), "non-Sony swallowed Back");
  service.Connect(0, Pad(Model::kDualSense, 42, 0));
  Require(service.ReadDevice(0).model == Model::kNone, "zero generation connected");
  service.Connect(0, Pad(Model::kDualSense, 0, 7));
  Require(service.ReadDevice(0).model == Model::kNone, "zero instance connected");
  service.Connect(0, Pad(Model::kDualShock4));
  Require(!service.ReadDevice(0).triggers, "DS4 advertised adaptive triggers");
  game.trigger_context = TriggerContext::kWeapon;
  game.aiming = game.can_fire = true;
  service.Publish(0, 7, game, oracle::kBase);
  const auto ds4 = service.Compose(0, oracle::kT10, options);
  Require(ds4.active && ds4.lighting, "DS4 lost supported feedback");
  Require(ds4.left == oracle::kNeutral && ds4.right == oracle::kNeutral,
          "DS4 received adaptive triggers");
  auto limited = Pad();
  limited.touchpad = limited.light = limited.rumble = limited.triggers = false;
  service.Connect(0, limited);
  service.Publish(0, 7, game, oracle::kBase);
  service.Emit(0, 7, Event::kExplosion, 1, oracle::kT10);
  service.ObserveClick(42, true, oracle::kT10);
  const auto output = service.Compose(0, oracle::kT10, options);
  Require(!output.lighting && output.low_motor == 0 && output.high_motor == 0,
          "missing capabilities did not suppress outputs");
  Require(output.left == oracle::kNeutral && output.right == oracle::kNeutral,
          "missing trigger capability did not suppress effects");
  Require(service.Consume(0, oracle::kT10, options).gestures.empty(), "missing touchpad accepted click");
  RequireNeutral(service.Compose(4, oracle::kT10, options));
}

void LeaseBoundariesAndClockRegression() {
  Fixture f;
  f.game.trigger_context = TriggerContext::kWeapon;
  f.game.can_fire = true;
  f.Publish(oracle::kBase);
  Require(f.Compose(oracle::kT249).active, "lease expired early");
  Require(f.Compose(oracle::kT250).active, "inclusive 250ms lease boundary lost");
  RequireNeutral(f.Compose(oracle::kT251));
  RequireNeutral(f.Compose(oracle::kBeforeBase));
  f.Touch(ContactPhase::kDown, 0, 0, oracle::kT10);
  f.Publish(oracle::kT251);
  f.Touch(ContactPhase::kUp, 1, 0, oracle::kT299);
  Require(f.Consume(oracle::kT299).gestures.empty(), "expired lease revived old gesture");
}

void GenerationIsolationAndReconnect() {
  Fixture f;
  f.Click(true, oracle::kT10);
  f.service.Emit(0, 7, Event::kExplosion, 1, oracle::kT10);
  f.service.Connect(0, Pad(Model::kDualSense, 43, 8));
  f.service.Publish(0, 7, f.game, oracle::kT20);
  f.service.Emit(0, 7, Event::kShot, 1, oracle::kT20);
  f.service.ObserveClick(42, true, oracle::kT20);
  RequireNeutral(f.Compose(oracle::kT20));
  f.service.Publish(0, 8, f.game, oracle::kT20);
  const auto fresh = f.Compose(oracle::kT20);
  Require(fresh.active && fresh.low_motor == 0 && fresh.high_motor == 0,
          "reconnect inherited stale pulse");
  Require(f.Consume(oracle::kT20).gestures.empty(), "reconnect inherited stale gesture");
  f.service.DisableTriggers(0, 7);
  Require(f.service.ReadDevice(0).triggers, "stale generation disabled new trigger backend");
  f.service.DisableTriggers(0, 8);
  Require(!f.service.ReadDevice(0).triggers, "backend failure did not disable triggers");
  f.service.Disconnect(42);
  Require(f.service.ReadDevice(0).instance_id == 43, "old disconnect removed new controller");
  f.service.Disconnect(43);
  RequireNeutral(f.Compose(oracle::kT20));
  Require(f.Consume(oracle::kT20).device.instance_id == 0, "disconnected identity survived");
}

void MultipleUsersStayIndependent() {
  Fixture f;
  f.service.Connect(1, Pad(Model::kDualSense, 73, 14));
  auto second = f.game;
  second.episode = 2;
  f.service.Publish(1, 14, second, oracle::kBase);
  f.service.ObserveClick(73, true, oracle::kT10);
  f.service.Emit(1, 14, Event::kDamage, 1, oracle::kT10);
  Require(f.Consume(oracle::kT10).gestures.empty(), "second pad gesture reached first user");
  Require(f.Compose(oracle::kT10).low_motor == 0, "second pad rumble reached first user");
  RequireGesture(f.service.Consume(1, oracle::kT10, f.options), Gesture::kCamera);
  Require(f.service.Compose(1, oracle::kT10, f.options).color == oracle::kPurple,
          "second player lighting lost identity");
  f.service.Disconnect(42);
  Require(f.service.Compose(1, oracle::kT10, f.options).active,
          "disconnect cancelled unrelated controller");
}

void GestureDirectionAndThresholds() {
  for (const auto& sample : oracle::kSwipes) {
    Fixture f;
    f.Touch(ContactPhase::kDown, sample.sx, sample.sy, oracle::kBase);
    f.Touch(ContactPhase::kMove, sample.ex, sample.ey, oracle::kT50);
    Require(f.Consume(oracle::kT50).gestures.empty(), "moving finger fired swipe early");
    f.Touch(ContactPhase::kUp, sample.ex, sample.ey, oracle::kT100);
    const auto input = f.Consume(oracle::kT100);
    const auto action = sample.horizontal
                            ? (sample.positive ? Gesture::kNextRadio : Gesture::kPreviousRadio)
                            : (sample.positive ? Gesture::kNextWeapon : Gesture::kPreviousWeapon);
    if (sample.accepted) RequireGesture(input, action);
    else Require(input.gestures.empty(), "short or ambiguous swipe was accepted");
    Require(f.Consume(oracle::kT110).gestures.empty(), "swipe replayed after consumption");
  }
}

void OwnershipFlagsAndBackFallback() {
  Fixture f;
  Require(f.service.OwnsGestures(0, oracle::kT10, f.options), "active controller lacks gesture lease");
  Require(f.Consume(oracle::kT10).gestures_owned, "input snapshot omitted gesture lease");
  f.options.gestures = false;
  Require(!f.service.OwnsGestures(0, oracle::kT20, f.options), "disabled gestures retained ownership");
  Require(!f.Consume(oracle::kT20).gestures_owned, "disabled gesture snapshot retained lease");
  // The title publishes the disabled preference; SDL callbacks have no cvar
  // reads and use this publication to preserve the existing Back binding.
  f.game.gestures_owned = false;
  f.Publish(oracle::kT20);
  f.Click(true, oracle::kT40);
  Require(!f.service.ClaimsClick(0, oracle::kT40, f.options), "disabled publication swallowed Back");
  f.options.gestures = true;
  f.game.gestures_owned = true;
  f.Publish(oracle::kT50);
  Require(!f.service.ClaimsClick(0, oracle::kT50, f.options), "enabling gestures stole a held Back press");
  f.Click(false, oracle::kT79);
  f.Click(true, oracle::kT80);
  Require(f.service.ClaimsClick(0, oracle::kT80, f.options), "new owned press was not claimed");
  RequireGesture(f.Consume(oracle::kT80), Gesture::kCamera);
  f.service.SetFocused(false);
  Require(!f.service.OwnsGestures(0, oracle::kT81, f.options), "focus loss retained gesture lease");
  Require(!f.Consume(oracle::kT81).gestures_owned, "focus loss did not invalidate title replay lease");
  Require(f.service.ClaimsClick(0, oracle::kT81, f.options),
          "owned held press leaked a new Back edge when focus changed");
  f.Click(false, oracle::kT100);
  Require(!f.service.ClaimsClick(0, oracle::kT100, f.options), "released press retained ownership");
  f.service.SetFocused(true);
  f.Publish(oracle::kBase);
  Require(f.service.OwnsGestures(0, oracle::kT250, f.options), "lease boundary omitted ownership");
  Require(!f.service.OwnsGestures(0, oracle::kT251, f.options), "expired lease retained ownership");
  Require(!f.Consume(oracle::kT251).gestures_owned, "stale title replay lease was not cancelled");
}

void SwipeDurationBoundary() {
  for (const auto end : {oracle::kT699, oracle::kT700, oracle::kT701}) {
    Fixture f;
    f.Touch(ContactPhase::kDown, 0, 0, oracle::kBase);
    for (const auto time : {oracle::kT200, oracle::kT400, oracle::kT600}) f.Publish(time);
    f.Touch(ContactPhase::kUp, 1, 0, end);
    const auto result = f.Consume(end);
    if (end == oracle::kT701) Require(result.gestures.empty(), "swipe exceeded 700ms window");
    else RequireGesture(result, Gesture::kNextRadio);
  }
}

void GestureEntryOwnership() {
  Fixture f;
  f.game.gestures_owned = false;
  f.Publish(oracle::kBase);
  f.Touch(ContactPhase::kDown, 0, 0, oracle::kT10);
  f.Click(true, oracle::kT10);
  f.game.gestures_owned = true;
  f.Publish(oracle::kT20);
  Require(!f.service.ClaimsClick(0, oracle::kT20, f.options), "unowned held click was stolen");
  f.Touch(ContactPhase::kUp, 1, 0, oracle::kT40);
  Require(f.Consume(oracle::kT40).gestures.empty(), "unowned contact was stolen mid-gesture");
  f.Click(false, oracle::kT40);
  f.Click(true, oracle::kT50);
  RequireGesture(f.Consume(oracle::kT50), Gesture::kCamera);
  f.Click(true, oracle::kT79);
  Require(f.Consume(oracle::kT79).gestures.empty(), "held click repeated camera action");
}

void GestureEpochCancellation() {
  for (const int reason : {0, 1, 2, 3}) {
    Fixture f;
    f.Touch(ContactPhase::kDown, 0, 0, oracle::kBase);
    f.Click(true, oracle::kT10);
    if (reason == 0) f.game.player_identity = 99;
    if (reason == 1) f.game.context_identity = 99;
    if (reason == 2) f.game.gestures_owned = false;
    if (reason == 3) f.game.active = false;
    f.Publish(oracle::kT20);
    f.game.active = f.game.gestures_owned = true;
    f.Publish(oracle::kT40);
    f.Touch(ContactPhase::kUp, 1, 0, oracle::kT50);
    Require(f.Consume(oracle::kT50).gestures.empty(), "context transition revived pending/contact input");
    f.Touch(ContactPhase::kDown, 0, 0, oracle::kT79);
    f.Touch(ContactPhase::kUp, 0, 1, oracle::kT100);
    RequireGesture(f.Consume(oracle::kT100), Gesture::kNextWeapon);
  }
}

void PendingGestureExpiry() {
  for (const auto end : {oracle::kT499, oracle::kT500, oracle::kT501}) {
    Fixture f;
    f.Click(true, oracle::kBase);
    f.Click(false, oracle::kT10);
    f.Publish(oracle::kT200);
    f.Publish(oracle::kT400);
    const auto input = f.Consume(end);
    if (end == oracle::kT501) Require(input.gestures.empty(), "expired gesture was replayed");
    else RequireGesture(input, Gesture::kCamera);
  }
}

void MultitouchHoldAndRelease() {
  Fixture f;
  f.game.in_vehicle = true;
  f.Publish(oracle::kBase);
  f.Touch(ContactPhase::kDown, 0, 0, oracle::kBase);
  f.Touch(ContactPhase::kDown, 1, 0, oracle::kT10, 1);
  f.Publish(oracle::kT200);
  Require(!f.Consume(oracle::kT309).cinematic, "two-finger hold activated early");
  Require(f.Consume(oracle::kT310).cinematic, "two-finger hold missing at 300ms");
  f.Click(true, oracle::kT310);
  Require(f.Consume(oracle::kT310).gestures.empty(), "multitouch click leaked camera action");
  f.Touch(ContactPhase::kUp, 1, 0, oracle::kT311);
  auto input = f.Consume(oracle::kT311);
  Require(!input.cinematic && input.gestures.empty(), "finger release failed to stop cinematic/swipe");
  f.Touch(ContactPhase::kUp, 0, 1, oracle::kT399, 1);
  Require(f.Consume(oracle::kT399).gestures.empty(), "second finger leaked a swipe");
  f.game.in_vehicle = false;
  f.Publish(oracle::kT400);
  f.Touch(ContactPhase::kDown, 0, 0, oracle::kT400);
  f.Touch(ContactPhase::kDown, 1, 0, oracle::kT400, 1);
  f.Publish(oracle::kT600);
  Require(!f.Consume(oracle::kT700).cinematic, "on-foot multitouch claimed cinematic camera");
}

void InvalidTouchAndFocus() {
  Fixture f;
  const auto nan = std::numeric_limits<float>::quiet_NaN();
  for (const auto invalid : {nan, -0.1f, 1.1f}) {
    f.Touch(ContactPhase::kDown, 0, 0, oracle::kBase);
    f.Touch(ContactPhase::kMove, invalid, 0, oracle::kT10);
    f.Touch(ContactPhase::kUp, 1, 0, oracle::kT20);
    Require(f.Consume(oracle::kT20).gestures.empty(), "invalid coordinate did not cancel gesture");
  }
  f.Touch(ContactPhase::kDown, 0, 0, oracle::kT40, -1);
  f.Touch(ContactPhase::kUp, 1, 0, oracle::kT50, -1);
  f.Touch(ContactPhase::kDown, 0, 0, oracle::kT40, 2);
  f.Touch(ContactPhase::kUp, 1, 0, oracle::kT50, 2);
  Require(f.Consume(oracle::kT50).gestures.empty(), "invalid finger index accepted");
  f.Touch(ContactPhase::kDown, 0, 0, oracle::kT50);
  f.service.Emit(0, 7, Event::kExplosion, 1, oracle::kT50);
  f.service.SetFocused(false);
  RequireNeutral(f.Compose(oracle::kT79));
  f.service.SetFocused(true);
  RequireNeutral(f.Compose(oracle::kT80));
  f.Publish(oracle::kT81);
  f.Touch(ContactPhase::kUp, 1, 0, oracle::kT100);
  Require(f.Consume(oracle::kT100).gestures.empty(), "focus return revived held contact");
  Require(f.Compose(oracle::kT100).low_motor == 0, "focus return revived explosion");
}

void PulseExpiration() {
  struct Boundary { Event event; uint64_t before, at, after; };
  const Boundary boundaries[] = {
      {Event::kShot, oracle::kT79, oracle::kT80, oracle::kT81},
      {Event::kDamage, oracle::kT149, oracle::kT150, oracle::kT151},
      {Event::kExplosion, oracle::kT299, oracle::kT300, oracle::kT301},
  };
  for (const auto& boundary : boundaries) {
    Fixture f;
    f.service.Emit(0, 7, boundary.event, 1, oracle::kBase);
    if (boundary.event == Event::kExplosion) f.Publish(oracle::kT200);
    const auto before = f.Compose(boundary.before);
    Require(before.low_motor > 0 && before.high_motor > 0, "pulse expired early");
    const auto at = f.Compose(boundary.at);
    Require(at.low_motor == 0 && at.high_motor == 0, "pulse active at exclusive expiry");
    const auto after = f.Compose(boundary.after);
    Require(after.low_motor == 0 && after.high_motor == 0, "expired pulse revived");
  }
}

void OptionsRemainIndependent() {
  Fixture f;
  f.service.Emit(0, 7, Event::kDamage, 1, oracle::kBase);
  f.Click(true, oracle::kT10);
  f.options.gestures = false;
  Require(f.Consume(oracle::kT20).gestures.empty(), "disabled gestures were delivered");
  Require(f.Compose(oracle::kT20).low_motor > 0,
          "disabling gestures erased independent damage feedback");
  f.options.rumble = false;
  Require(f.Compose(oracle::kT20).low_motor == 0, "rumble toggle ignored");
  Require(f.Compose(oracle::kT20).lighting, "rumble toggle disabled independent lighting");
  f.options.lighting = false;
  Require(!f.Compose(oracle::kT20).lighting, "lighting toggle ignored");
  f.options.enabled = false;
  RequireNeutral(f.Compose(oracle::kT20));
}

void IntensityAndInvalidEventBounds() {
  for (const auto intensity : {0.0f, -1.0f, std::numeric_limits<float>::infinity(),
                               std::numeric_limits<float>::quiet_NaN()}) {
    Fixture f;
    f.game.trigger_context = TriggerContext::kWeapon;
    f.game.aiming = f.game.can_fire = true;
    f.Publish(oracle::kBase);
    f.service.Emit(0, 7, Event::kExplosion, 1, oracle::kBase);
    f.options.intensity = intensity;
    const auto output = f.Compose(oracle::kT10);
    Require(output.low_motor == 0 && output.high_motor == 0,
            "invalid or zero intensity generated rumble");
    Require(output.left == oracle::kNeutral && output.right == oracle::kNeutral,
            "invalid or zero intensity generated resistance");
  }
  Fixture f;
  f.options.intensity = 1;
  f.service.Emit(0, 7, Event::kExplosion, 1, oracle::kBase);
  const auto maximum = f.Compose(oracle::kT10);
  f.options.intensity = 100;
  const auto clamped = f.Compose(oracle::kT10);
  Require(clamped.low_motor == maximum.low_motor && clamped.high_motor == maximum.high_motor,
          "intensity above one did not clamp");
  Fixture invalid;
  invalid.service.Emit(0, 7, static_cast<Event>(99), 1, oracle::kBase);
  invalid.service.Emit(0, 7, Event::kShot, std::numeric_limits<float>::quiet_NaN(), oracle::kBase);
  Require(invalid.Compose(oracle::kT10).low_motor == 0, "invalid event generated feedback");
}

void TriggerContextAndRecoilRelease() {
  Fixture f;
  f.game.trigger_context = TriggerContext::kWeapon;
  f.game.aiming = f.game.can_fire = true;
  f.Publish(oracle::kBase);
  const auto ready = f.Compose(oracle::kBase);
  Require(ready.left.front() == 0x01 && ready.right.front() == 0x01,
          "weapon readiness missing resistance");
  f.service.Emit(0, 7, Event::kShot, 1, oracle::kBase);
  Require(f.Compose(oracle::kT79).right.front() == 0x06, "shot did not select recoil effect");
  Require(f.Compose(oracle::kT80).right == ready.right, "shot expiry did not restore current resistance");
  f.game.can_fire = f.game.aiming = false;
  f.Publish(oracle::kT81);
  Require(f.Compose(oracle::kT100).right == oracle::kNeutral, "unarmed state retained trigger effect");
  f.game.trigger_context = TriggerContext::kVehicle;
  f.game.driver = false;
  f.Publish(oracle::kT100);
  Require(f.Compose(oracle::kT100).right == oracle::kNeutral, "passenger received driving resistance");
  f.game.driver = true;
  f.Publish(oracle::kT110);
  f.service.UpdateAxes(0, 0, 255);
  const auto driving = f.Compose(oracle::kT110);
  Require(driving.left.front() == 0x01 && driving.right.front() == 0x01,
          "driver did not receive vehicle resistance");
  Require(driving.right.at(2) > driving.left.at(2), "vehicle trigger axes were swapped or ignored");
  f.options.triggers = false;
  const auto disabled = f.Compose(oracle::kT110);
  Require(disabled.left == oracle::kNeutral && disabled.right == oracle::kNeutral,
          "trigger toggle failed to release both sides");
}

void EpisodeAndWantedLighting() {
  Fixture f;
  for (const auto& sample : {std::pair{0, oracle::kBlue}, std::pair{1, oracle::kOrange},
                             std::pair{2, oracle::kPurple}, std::pair{99, oracle::kBlue}}) {
    f.game.episode = sample.first;
    f.Publish(oracle::kBase);
    Require(f.Compose(oracle::kBase).color == sample.second, "episode color mismatch");
  }
  f.game.wanted = 1;
  f.Publish(oracle::kBase);
  Require(f.Compose(oracle::kT249).color == oracle::kRed, "wanted red phase ended early");
  f.Publish(oracle::kT250);
  Require(f.Compose(oracle::kT250).color == oracle::kBlue, "wanted phase did not change at 250ms");
  f.game.wanted = 4;
  f.Publish(oracle::kT400);
  Require(f.Compose(oracle::kT499).color == oracle::kBlue, "wanted-level change reset animation");
  Require(f.Compose(oracle::kT500).color == oracle::kRed, "wanted phase did not cycle at 500ms");
  f.game.wanted = 0;
  f.game.episode = 2;
  f.Publish(oracle::kT501);
  Require(f.Compose(oracle::kT501).color == oracle::kPurple, "wanted exit did not restore episode color");
  f.game.wanted = 1;
  f.Publish(oracle::kT600);
  Require(f.Compose(oracle::kT600).color == oracle::kRed, "new wanted session did not restart red");
}
}  // namespace

int main() {
  const std::pair<const char*, void (*)()> tests[] = {
      {"independent SDL trigger packet vectors", EncoderGoldenVectors},
      {"device and capability gates", DeviceAndCapabilityGates},
      {"lease boundaries and clock regression", LeaseBoundariesAndClockRegression},
      {"generation isolation and reconnect", GenerationIsolationAndReconnect},
      {"multiple user association", MultipleUsersStayIndependent},
      {"gesture direction, distance and dominance", GestureDirectionAndThresholds},
      {"ownership flags and Back fallback", OwnershipFlagsAndBackFallback},
      {"swipe duration boundary", SwipeDurationBoundary},
      {"gesture ownership at entry", GestureEntryOwnership},
      {"gesture context epochs", GestureEpochCancellation},
      {"pending gesture expiry", PendingGestureExpiry},
      {"two-finger hold and release", MultitouchHoldAndRelease},
      {"invalid touch and focus cancellation", InvalidTouchAndFocus},
      {"shot, damage and explosion expiry", PulseExpiration},
      {"independent options", OptionsRemainIndependent},
      {"intensity and invalid event bounds", IntensityAndInvalidEventBounds},
      {"trigger contexts and recoil release", TriggerContextAndRecoilRelease},
      {"episode and wanted lighting", EpisodeAndWantedLighting},
  };
  bool failed = false;
  for (const auto& [name, test] : tests) {
    try {
      test();
      std::cout << "PASS " << name << '\n';
    } catch (const std::exception& error) {
      failed = true;
      std::cerr << "FAIL " << name << ": " << error.what() << '\n';
    }
  }
  return failed ? 1 : 0;
}
