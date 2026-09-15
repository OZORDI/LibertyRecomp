// SDL transport integration for the production Sony feedback service/encoder.
// Default mode opens only virtual devices. Physical mode requires an exact ID.
#include <SDL3/SDL.h>
#include <rex/input/sony_feedback.h>

#include <algorithm>
#include <array>
#include <charconv>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "sony_feedback_oracle.h"

namespace {
using namespace rex::input::sony;
namespace oracle = sony_oracle;

void Require(bool condition, const char* message) {
  if (!condition) throw std::runtime_error(std::string(message) + ": " + SDL_GetError());
}

struct Capture {
  std::vector<EffectPacket> effects;
  std::vector<std::array<uint8_t, 3>> lights;
  std::vector<std::pair<uint16_t, uint16_t>> motors;
  bool fail_effects = false;

  static bool SDLCALL Effect(void* userdata, const void* data, int size) {
    auto& capture = *static_cast<Capture*>(userdata);
    if (capture.fail_effects) return SDL_SetError("injected virtual effect failure");
    if (size != oracle::kPacketSize) return SDL_SetError("unexpected Sony effect payload size");
    EffectPacket packet{};
    std::memcpy(packet.data(), data, packet.size());
    capture.effects.push_back(packet);
    return true;
  }
  static bool SDLCALL Light(void* userdata, uint8_t r, uint8_t g, uint8_t b) {
    static_cast<Capture*>(userdata)->lights.push_back({r, g, b});
    return true;
  }
  static bool SDLCALL Rumble(void* userdata, uint16_t low, uint16_t high) {
    static_cast<Capture*>(userdata)->motors.emplace_back(low, high);
    return true;
  }
};

struct VirtualPad {
  Capture capture;
  SDL_JoystickID id = 0;
  SDL_Gamepad* pad = nullptr;

  explicit VirtualPad(uint16_t product = oracle::kDualSenseProduct, bool effects = true) {
    SDL_VirtualJoystickTouchpadDesc touchpad{};
    touchpad.nfingers = 2;
    SDL_VirtualJoystickDesc descriptor{};
    SDL_INIT_INTERFACE(&descriptor);
    descriptor.type = SDL_JOYSTICK_TYPE_GAMEPAD;
    descriptor.vendor_id = oracle::kSonyVendor;
    descriptor.product_id = product;
    descriptor.naxes = SDL_GAMEPAD_AXIS_COUNT;
    descriptor.nbuttons = SDL_GAMEPAD_BUTTON_COUNT;
    descriptor.ntouchpads = 1;
    descriptor.touchpads = &touchpad;
    descriptor.name = "Liberty automated Sony virtual controller";
    descriptor.userdata = &capture;
    descriptor.Rumble = &Capture::Rumble;
    descriptor.SetLED = &Capture::Light;
    descriptor.SendEffect = effects ? &Capture::Effect : nullptr;
    id = SDL_AttachVirtualJoystick(&descriptor);
    Require(id != 0, "attach virtual controller");
    pad = SDL_OpenGamepad(id);
    if (!pad) {
      SDL_DetachVirtualJoystick(id);
      id = 0;
      Require(false, "open virtual controller");
    }
  }
  ~VirtualPad() {
    if (pad) SDL_CloseGamepad(pad);
    if (id) SDL_DetachVirtualJoystick(id);
  }
  VirtualPad(const VirtualPad&) = delete;
  VirtualPad& operator=(const VirtualPad&) = delete;
};

Device Describe(SDL_Gamepad* pad, uint64_t generation) {
  Device device{};
  device.instance_id = SDL_GetGamepadID(pad);
  device.generation = generation;
  const auto type = SDL_GetRealGamepadType(pad);
  device.model = type == SDL_GAMEPAD_TYPE_PS5 ? Model::kDualSense
                 : type == SDL_GAMEPAD_TYPE_PS4 ? Model::kDualShock4 : Model::kNone;
  const auto properties = SDL_GetGamepadProperties(pad);
  device.touchpad = SDL_GetNumGamepadTouchpads(pad) > 0;
  device.light = SDL_GetBooleanProperty(properties, SDL_PROP_GAMEPAD_CAP_RGB_LED_BOOLEAN, false);
  device.rumble = SDL_GetBooleanProperty(properties, SDL_PROP_GAMEPAD_CAP_RUMBLE_BOOLEAN, false);
  device.triggers = device.model == Model::kDualSense;
  return device;
}

struct TouchWatch {
  Service& service;
  SDL_JoystickID id;
  uint64_t now_ms = oracle::kBase;
  bool installed = false;

  TouchWatch(Service& value, SDL_JoystickID instance) : service(value), id(instance) {
    installed = SDL_AddEventWatch(&Watch, this);
    Require(installed, "install touch event watch");
  }
  ~TouchWatch() {
    if (installed) SDL_RemoveEventWatch(&Watch, this);
  }
  static bool SDLCALL Watch(void* userdata, SDL_Event* event) {
    auto& watch = *static_cast<TouchWatch*>(userdata);
    if ((event->type == SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN ||
         event->type == SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION ||
         event->type == SDL_EVENT_GAMEPAD_TOUCHPAD_UP) && event->gtouchpad.which == watch.id) {
      const auto phase = event->type == SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN ? ContactPhase::kDown
                         : event->type == SDL_EVENT_GAMEPAD_TOUCHPAD_UP ? ContactPhase::kUp
                                                                     : ContactPhase::kMove;
      watch.service.ObserveTouch(event->gtouchpad.which, event->gtouchpad.finger, phase,
                                 event->gtouchpad.x, event->gtouchpad.y, watch.now_ms);
    }
    return true;
  }
  void Finger(SDL_Gamepad* pad, bool down, float x, float y, uint64_t time, int finger = 0) {
    now_ms = time;
    Require(SDL_SetJoystickVirtualTouchpad(SDL_GetGamepadJoystick(pad), 0, finger,
                                           down, x, y, down ? 1.0f : 0.0f),
            "set virtual touchpad sample");
    SDL_UpdateGamepads();
  }
};

GameState Playing() {
  GameState game{};
  game.active = game.gestures_owned = true;
  game.player_identity = 19;
  game.context_identity = 23;
  return game;
}

void VirtualRuntime() {
  VirtualPad first;
  VirtualPad second;
  VirtualPad ds4(oracle::kDualShockProduct, false);
  Require(SDL_IsJoystickVirtual(first.id), "default harness must only open virtual devices");
  Require(SDL_GetRealGamepadType(first.pad) == SDL_GAMEPAD_TYPE_PS5, "virtual Sony identity recognition");
  Require(SDL_GetRealGamepadType(ds4.pad) == SDL_GAMEPAD_TYPE_PS4, "virtual DS4 identity recognition");
  Service service;
  const auto device = Describe(first.pad, 7);
  Require(device.touchpad && device.light && device.rumble, "virtual SDL capability detection");
  service.Connect(0, device);
  service.Connect(1, Describe(second.pad, 8));
  service.Connect(2, Describe(ds4.pad, 9));
  Require(!service.ReadDevice(2).triggers, "DS4 adaptive trigger gate");
  auto game = Playing();
  Options options{};
  service.Publish(0, 7, game, oracle::kBase);
  service.Publish(1, 8, game, oracle::kBase);
  TouchWatch watch(service, first.id);
  watch.Finger(first.pad, true, 0, 0, oracle::kT10);
  watch.Finger(first.pad, true, 0.5f, 0, oracle::kT50);
  Require(service.Consume(0, oracle::kT50, options).gestures.empty(), "SDL movement fired early swipe");
  watch.Finger(first.pad, false, 0.5f, 0, oracle::kT100);
  const auto input = service.Consume(0, oracle::kT100, options);
  Require(input.gestures.size() == 1 && input.gestures.front() == Gesture::kNextRadio,
          "SDL down/move/up did not reach production gesture service");
  Require(service.Consume(1, oracle::kT100, options).gestures.empty(), "SDL touch crossed player association");
  std::cout << "PASS real SDL virtual identity, capabilities and touch event delivery\n";

  const auto packet = EncodeTriggers(Resistance(45, 110), Recoil(63));
  Require(SDL_SendGamepadEffect(first.pad, packet.data(), static_cast<int>(packet.size())),
          "send production trigger packet through SDL");
  Require(!first.capture.effects.empty() && first.capture.effects.back() == oracle::kPacket,
          "SDL callback payload differs from independent Python oracle");
  Require(second.capture.effects.empty(), "trigger output reached the wrong controller");
  Require(SDL_RumbleGamepad(first.pad, 32768, 65535, oracle::kProbeDelayMs), "SDL native rumble");
  Require(!first.capture.motors.empty() && first.capture.motors.back() == std::pair<uint16_t, uint16_t>{32768, 65535},
          "16-bit rumble values wrapped or changed");
  service.Emit(0, 7, Event::kDamage, 1, oracle::kT100);
  const auto output = service.Compose(0, oracle::kT110, options);
  Require(SDL_RumbleGamepad(first.pad, output.low_motor, output.high_motor, oracle::kProbeDelayMs),
          "service pulse rumble output");
  Require(first.capture.motors.back() == std::pair{output.low_motor, output.high_motor},
          "service rumble changed at SDL callback");
  Require(SDL_SetGamepadLED(first.pad, output.color[0], output.color[1], output.color[2]),
          "service light output");
  Require(!first.capture.lights.empty() && first.capture.lights.back() == oracle::kBlue,
          "episode color changed at SDL callback");
  std::cout << "PASS real SDL output callbacks preserve production packets, lighting and rumble\n";

  first.capture.fail_effects = true;
  Require(!SDL_SendGamepadEffect(first.pad, packet.data(), static_cast<int>(packet.size())),
          "SDL hid output failure incorrectly reported success");
  service.DisableTriggers(0, 7);
  Require(!service.ReadDevice(0).triggers && service.ReadDevice(1).triggers,
          "failed trigger backend disabled an unrelated controller");
  Require(!SDL_SendGamepadEffect(ds4.pad, packet.data(), static_cast<int>(packet.size())),
          "unsupported virtual effects incorrectly reported success");
  first.capture.fail_effects = false;
  SDL_ClearError();
  service.SetFocused(false);
  const auto stopped = service.Compose(0, oracle::kT149, options);
  const auto neutral = EncodeTriggers(stopped.left, stopped.right);
  Require(neutral == oracle::kNeutralPacket, "focus reset did not generate neutral effects");
  Require(SDL_SendGamepadEffect(first.pad, neutral.data(), static_cast<int>(neutral.size())),
          "neutral trigger output");
  Require(SDL_RumbleGamepad(first.pad, stopped.low_motor, stopped.high_motor, 0), "neutral rumble output");
  Require(first.capture.effects.back() == oracle::kNeutralPacket,
          "SDL reset packet retained trigger resistance");
  Require(first.capture.motors.back() == std::pair<uint16_t, uint16_t>{0, 0},
          "SDL reset retained rumble");
  service.Disconnect(first.id);
  Require(service.ReadDevice(0).instance_id == 0 && service.ReadDevice(1).instance_id == second.id,
          "disconnect ownership cleanup");
  std::cout << "PASS real SDL failure results, capability downgrade and neutral reset callbacks\n";
}

struct PhysicalPad {
  SDL_Gamepad* pad = nullptr;
  bool owns_output = false;
  ~PhysicalPad() {
    if (!pad) return;
    if (owns_output) {
      const auto neutral = EncodeTriggers(Resistance(0, 0), Resistance(0, 0));
      SDL_SendGamepadEffect(pad, neutral.data(), static_cast<int>(neutral.size()));
      SDL_RumbleGamepad(pad, 0, 0, 0);
      SDL_SetGamepadLED(pad, 0, 0, 0);
    }
    SDL_CloseGamepad(pad);
  }
};

SDL_JoystickID UniqueSonyInstance() {
  int count = 0;
  auto* ids = SDL_GetGamepads(&count);
  Require(ids != nullptr, "enumerate physical candidate IDs");
  std::cout << "INFO SDL physical inventory count=" << count << '\n';
  std::vector<SDL_JoystickID> candidates;
  for (int index = 0; index < count; ++index) {
    const auto candidate = ids[index];
    std::cout << "INFO instance=" << candidate
              << " type=" << static_cast<int>(SDL_GetRealGamepadTypeForID(candidate))
              << " vendor=" << SDL_GetGamepadVendorForID(candidate)
              << " product=" << SDL_GetGamepadProductForID(candidate)
              << " virtual=" << SDL_IsJoystickVirtual(candidate) << '\n';
    if (!SDL_IsJoystickVirtual(candidate) &&
        SDL_GetRealGamepadTypeForID(candidate) == SDL_GAMEPAD_TYPE_PS5 &&
        SDL_GetGamepadVendorForID(candidate) == oracle::kSonyVendor) {
      candidates.push_back(candidate);
    }
  }
  SDL_free(ids);
  Require(candidates.size() == 1, "automatic probe requires exactly one physical Sony DualSense");
  return candidates.front();
}

void PhysicalRuntime(SDL_JoystickID id) {
  Require(!SDL_IsJoystickVirtual(id), "physical mode received virtual instance");
  Require(SDL_GetRealGamepadTypeForID(id) == SDL_GAMEPAD_TYPE_PS5,
          "physical mode requires a real DualSense-class controller");
  Require(SDL_GetGamepadVendorForID(id) == oracle::kSonyVendor, "physical mode requires Sony vendor");
  PhysicalPad device;
  device.pad = SDL_OpenGamepad(id);
  Require(device.pad != nullptr, "open explicitly selected physical controller");
  Require(SDL_GetGamepadConnectionState(device.pad) == SDL_JOYSTICK_CONNECTION_WIRED,
          "physical probe currently requires a wired controller");
  device.owns_output = true;
  Service service;
  service.Connect(0, Describe(device.pad, 7));
  auto game = Playing();
  game.trigger_context = TriggerContext::kWeapon;
  game.can_fire = true;
  Options options{};
  options.intensity = 0.15f;
  service.Publish(0, 7, game, oracle::kBase);
  service.Emit(0, 7, Event::kDamage, 0.15f, oracle::kBase);
  const auto output = service.Compose(0, oracle::kBase, options);
  const auto packet = EncodeTriggers(output.left, output.right);
  Require(SDL_SendGamepadEffect(device.pad, packet.data(), static_cast<int>(packet.size())),
          "physical SDL accepted trigger state");
  Require(SDL_SetGamepadLED(device.pad, output.color[0], output.color[1], output.color[2]),
          "physical SDL accepted episode light");
  Require(SDL_RumbleGamepad(device.pad, output.low_motor, output.high_motor, oracle::kProbeDelayMs),
          "physical SDL accepted short feedback pulse");
  SDL_Delay(oracle::kProbeDelayMs);
  service.SetFocused(false);
  const auto stopped = service.Compose(0, oracle::kT100, options);
  const auto neutral = EncodeTriggers(stopped.left, stopped.right);
  Require(SDL_SendGamepadEffect(device.pad, neutral.data(), static_cast<int>(neutral.size())),
          "physical SDL accepted neutral trigger reset");
  Require(SDL_RumbleGamepad(device.pad, 0, 0, 0), "physical SDL accepted rumble stop");
  std::cout << "PASS physical wired DualSense instance " << id
            << " accepted production trigger/light/rumble/reset output calls\n"
            << "LIMIT automated acknowledgements do not measure tactile strength, actuator motion, "
               "speaker audio or Bluetooth support\n";
}
}  // namespace

int main(int argc, char** argv) {
  bool physical = false;
  bool unique_physical = false;
  SDL_JoystickID id = 0;
  if (argc == 2 && std::string(argv[1]) == "--physical-unique-usb") {
    physical = unique_physical = true;
  } else if (argc != 1) {
    if (argc != 3 || std::string(argv[1]) != "--physical-instance") {
      std::cerr << "Usage: sony_feedback_runtime [--physical-instance ID | --physical-unique-usb]\n";
      return 2;
    }
    const std::string argument = argv[2];
    const auto result = std::from_chars(argument.data(), argument.data() + argument.size(), id);
    if (result.ec != std::errc{} || result.ptr != argument.data() + argument.size() || !id) {
      std::cerr << "FAIL invalid physical instance ID\n";
      return 2;
    }
    physical = true;
  }
  SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
  if (!physical) {
    // Do not let real HID or Apple MFI devices become test output targets.
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI, "0");
    SDL_SetHint(SDL_HINT_JOYSTICK_MFI, "0");
  }
  if (!SDL_Init(SDL_INIT_GAMEPAD)) {
    std::cerr << "FAIL SDL initialization: " << SDL_GetError() << '\n';
    return 1;
  }
  bool failed = false;
  try {
    if (physical) PhysicalRuntime(unique_physical ? UniqueSonyInstance() : id);
    else VirtualRuntime();
  } catch (const std::exception& error) {
    std::cerr << "FAIL " << error.what() << '\n';
    failed = true;
  }
  SDL_Quit();
  return failed ? 1 : 0;
}
