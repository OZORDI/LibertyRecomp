/**
 ******************************************************************************
 * ReXGlue — iOS GameController input driver (Objective-C++)
 ******************************************************************************
 *
 * Bridges Apple's GameController.framework (MFi / Xbox / PS / Switch
 * controllers over BT + Lightning) into the rex InputDriver contract.
 *
 * Connect/disconnect is wired via NSNotificationCenter; polling reads
 * extendedGamepad values directly from the live GCController object each
 * time GetState is called. Haptics uses CHHapticEngine obtained via
 * controller.haptics. This mirrors haptics_ios.mm's pattern but is
 * self-contained so the SDK does not depend on the app-side bridge.
 */

#include "ios_input_driver.h"

#if REX_PLATFORM_IOS

#import <Foundation/Foundation.h>
#import <GameController/GameController.h>
#import <CoreHaptics/CoreHaptics.h>

#include <algorithm>
#include <cmath>
#include <cstring>

#include <rex/logging.h>

// ---------------------------------------------------------------------------
// Per-controller Obj-C state
// ---------------------------------------------------------------------------

API_AVAILABLE(ios(14.0), tvos(14.0))
@interface LRIOSControllerBridge : NSObject
@property(nonatomic, strong) GCController* controller;
@property(nonatomic, strong) CHHapticEngine* hapticEngine;
@property(nonatomic, strong) id<CHHapticPatternPlayer> rumblePlayer;
- (instancetype)initWithController:(GCController*)controller;
- (void)rumbleLeft:(float)left right:(float)right;
- (void)teardown;
@end

@implementation LRIOSControllerBridge

- (instancetype)initWithController:(GCController*)controller {
  self = [super init];
  if (!self) return nil;
  _controller = controller;

  // Lazily build a haptic engine for the controller if it exposes one.
  if (@available(iOS 14.0, tvOS 14.0, *)) {
    GCDeviceHaptics* haptics = controller.haptics;
    if (haptics) {
      _hapticEngine = [haptics createEngineWithLocality:GCHapticsLocalityDefault];
      if (_hapticEngine) {
        __weak LRIOSControllerBridge* weakSelf = self;
        _hapticEngine.resetHandler = ^{
          LRIOSControllerBridge* s = weakSelf;
          if (!s) return;
          NSError* err = nil;
          [s.hapticEngine startAndReturnError:&err];
          (void)err;
        };
        NSError* err = nil;
        [_hapticEngine startAndReturnError:&err];
        (void)err;
      }
    }
    // Enable motion sampling if supported (gyro/accel).
    if (controller.motion) {
      controller.motion.sensorsActive = YES;
    }
  }
  return self;
}

- (void)rumbleLeft:(float)left right:(float)right {
  if (@available(iOS 14.0, tvOS 14.0, *)) {
    if (!_hapticEngine) return;
    dispatch_block_t block = ^{
      NSError* err = nil;
      if (self.rumblePlayer) {
        [self.rumblePlayer stopAtTime:0 error:&err];
        self.rumblePlayer = nil;
      }
      float mixed = std::min(1.0f, std::max(left, right) * 0.7f +
                                       std::min(left, right) * 0.3f);
      if (mixed <= 0.0f) return;
      [self.hapticEngine startAndReturnError:&err];

      CHHapticEventParameter* intensity =
          [[CHHapticEventParameter alloc] initWithParameterID:CHHapticEventParameterIDHapticIntensity
                                                        value:mixed];
      CHHapticEventParameter* sharpness =
          [[CHHapticEventParameter alloc] initWithParameterID:CHHapticEventParameterIDHapticSharpness
                                                        value:0.5f];
      CHHapticEvent* event =
          [[CHHapticEvent alloc] initWithEventType:CHHapticEventTypeHapticContinuous
                                        parameters:@[intensity, sharpness]
                                      relativeTime:0
                                          duration:0.5];
      CHHapticPattern* pattern =
          [[CHHapticPattern alloc] initWithEvents:@[event] parameters:@[] error:&err];
      if (!pattern) return;
      self.rumblePlayer = [self.hapticEngine createPlayerWithPattern:pattern error:&err];
      if (self.rumblePlayer) {
        [self.rumblePlayer startAtTime:0 error:&err];
      }
      (void)err;
    };
    if ([NSThread isMainThread]) block();
    else dispatch_async(dispatch_get_main_queue(), block);
  }
}

- (void)teardown {
  if (@available(iOS 14.0, tvOS 14.0, *)) {
    NSError* err = nil;
    if (_rumblePlayer) { [_rumblePlayer stopAtTime:0 error:&err]; _rumblePlayer = nil; }
    if (_hapticEngine) {
      [_hapticEngine stopWithCompletionHandler:^(NSError* _Nullable e) { (void)e; }];
      _hapticEngine = nil;
    }
    if (_controller && _controller.motion) {
      _controller.motion.sensorsActive = NO;
    }
    (void)err;
  }
  _controller = nil;
}

@end

namespace rex::input::ios {

// Opaque wrapper so the header doesn't leak Obj-C types.
struct IOSController {
  void* bridge_retained = nullptr;  // LRIOSControllerBridge* (__bridge_retained)
};

}  // namespace rex::input::ios

// ---------------------------------------------------------------------------
// NSNotificationCenter observer
// ---------------------------------------------------------------------------

@interface LRIOSInputObserver : NSObject {
@public
  rex::input::ios::IOSInputDriver* _driver;
}
@end

@implementation LRIOSInputObserver
- (void)onConnect:(NSNotification*)note {
  GCController* c = (GCController*)note.object;
  if (_driver && c) _driver->HandleConnect((__bridge void*)c);
}
- (void)onDisconnect:(NSNotification*)note {
  GCController* c = (GCController*)note.object;
  if (_driver && c) _driver->HandleDisconnect((__bridge void*)c);
}
@end

// ---------------------------------------------------------------------------
// Driver implementation
// ---------------------------------------------------------------------------

namespace rex::input::ios {

IOSInputDriver::IOSInputDriver(rex::ui::Window* window, size_t window_z_order)
    : InputDriver(window, window_z_order) {}

IOSInputDriver::~IOSInputDriver() {
  std::lock_guard<std::mutex> g(mutex_);
  if (observer_) {
    LRIOSInputObserver* obs = (__bridge_transfer LRIOSInputObserver*)observer_;
    [[NSNotificationCenter defaultCenter] removeObserver:obs];
    observer_ = nullptr;
  }
  for (auto& slot : slots_) {
    if (slot.ios) {
      if (slot.ios->bridge_retained) {
        LRIOSControllerBridge* b =
            (__bridge_transfer LRIOSControllerBridge*)slot.ios->bridge_retained;
        [b teardown];
        (void)b;
        slot.ios->bridge_retained = nullptr;
      }
      delete slot.ios;
      slot.ios = nullptr;
    }
  }
}

X_STATUS IOSInputDriver::Setup() {
  LRIOSInputObserver* obs = [[LRIOSInputObserver alloc] init];
  obs->_driver = this;
  observer_ = (__bridge_retained void*)obs;

  NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
  [nc addObserver:obs
         selector:@selector(onConnect:)
             name:GCControllerDidConnectNotification
           object:nil];
  [nc addObserver:obs
         selector:@selector(onDisconnect:)
             name:GCControllerDidDisconnectNotification
           object:nil];

  // Adopt any controllers already present (app launched with a pad paired).
  for (GCController* c in [GCController controllers]) {
    HandleConnect((__bridge void*)c);
  }

  initialized_ = true;
  REXLOG_INFO("iOS GameController input driver initialized");
  return X_STATUS_SUCCESS;
}

void IOSInputDriver::OnWindowAvailable(rex::ui::Window* /*window*/) {
  // Nothing window-specific; GameController is process-global.
}

void IOSInputDriver::HandleConnect(void* gc_raw) {
  GCController* controller = (__bridge GCController*)gc_raw;
  if (!controller || !controller.extendedGamepad) return;

  std::lock_guard<std::mutex> g(mutex_);
  // Find a free slot.
  for (size_t i = 0; i < slots_.size(); ++i) {
    if (!slots_[i].ios) {
      auto* ios = new IOSController();
      if (@available(iOS 14.0, tvOS 14.0, *)) {
        LRIOSControllerBridge* bridge =
            [[LRIOSControllerBridge alloc] initWithController:controller];
        ios->bridge_retained = (__bridge_retained void*)bridge;
      } else {
        // Pre-iOS 14: no haptics, but we still want polling.
        ios->bridge_retained = nullptr;
      }
      controller.playerIndex = static_cast<GCControllerPlayerIndex>(i);
      slots_[i].ios = ios;
      slots_[i].state = {};
      slots_[i].state_changed = true;
      REXLOG_INFO("iOS GameController connected at user index {}", i);
      return;
    }
  }
  REXLOG_WARN("iOS GameController connected but no free slot.");
}

void IOSInputDriver::HandleDisconnect(void* gc_raw) {
  GCController* controller = (__bridge GCController*)gc_raw;
  std::lock_guard<std::mutex> g(mutex_);
  for (size_t i = 0; i < slots_.size(); ++i) {
    if (!slots_[i].ios || !slots_[i].ios->bridge_retained) continue;
    LRIOSControllerBridge* b =
        (__bridge LRIOSControllerBridge*)slots_[i].ios->bridge_retained;
    if (b.controller == controller) {
      LRIOSControllerBridge* consumed =
          (__bridge_transfer LRIOSControllerBridge*)slots_[i].ios->bridge_retained;
      [consumed teardown];
      (void)consumed;
      delete slots_[i].ios;
      slots_[i] = {};
      REXLOG_INFO("iOS GameController disconnected at user index {}", i);
      return;
    }
  }
}

IOSInputDriver::Slot* IOSInputDriver::FindSlot(uint32_t user_index) {
  if (user_index >= slots_.size()) return nullptr;
  auto& s = slots_[user_index];
  return s.ios ? &s : nullptr;
}

void IOSInputDriver::SampleLocked(Slot& slot) {
  if (!slot.ios || !slot.ios->bridge_retained) return;
  LRIOSControllerBridge* bridge =
      (__bridge LRIOSControllerBridge*)slot.ios->bridge_retained;
  GCController* c = bridge.controller;
  if (!c) return;
  GCExtendedGamepad* gp = c.extendedGamepad;
  if (!gp) return;

  uint16_t buttons = 0;
  if (gp.buttonA.pressed)        buttons |= X_INPUT_GAMEPAD_A;
  if (gp.buttonB.pressed)        buttons |= X_INPUT_GAMEPAD_B;
  if (gp.buttonX.pressed)        buttons |= X_INPUT_GAMEPAD_X;
  if (gp.buttonY.pressed)        buttons |= X_INPUT_GAMEPAD_Y;
  if (gp.leftShoulder.pressed)   buttons |= X_INPUT_GAMEPAD_LEFT_SHOULDER;
  if (gp.rightShoulder.pressed)  buttons |= X_INPUT_GAMEPAD_RIGHT_SHOULDER;
  if (gp.dpad.up.pressed)        buttons |= X_INPUT_GAMEPAD_DPAD_UP;
  if (gp.dpad.down.pressed)      buttons |= X_INPUT_GAMEPAD_DPAD_DOWN;
  if (gp.dpad.left.pressed)      buttons |= X_INPUT_GAMEPAD_DPAD_LEFT;
  if (gp.dpad.right.pressed)     buttons |= X_INPUT_GAMEPAD_DPAD_RIGHT;
  if (gp.leftThumbstickButton.pressed)  buttons |= X_INPUT_GAMEPAD_LEFT_THUMB;
  if (gp.rightThumbstickButton.pressed) buttons |= X_INPUT_GAMEPAD_RIGHT_THUMB;
  if (@available(iOS 13.0, tvOS 13.0, *)) {
    if (gp.buttonMenu.pressed)    buttons |= X_INPUT_GAMEPAD_START;
    if (gp.buttonOptions.pressed) buttons |= X_INPUT_GAMEPAD_BACK;
  }
  if (@available(iOS 14.0, tvOS 14.0, *)) {
    if (gp.buttonHome && gp.buttonHome.pressed) buttons |= X_INPUT_GAMEPAD_GUIDE;
  }

  auto to_axis = [](float v) -> int16_t {
    v = std::max(-1.0f, std::min(1.0f, v));
    float scaled = v * 32767.0f;
    if (scaled > 32767.0f) scaled = 32767.0f;
    if (scaled < -32768.0f) scaled = -32768.0f;
    return static_cast<int16_t>(std::lround(scaled));
  };
  auto to_trigger = [](float v) -> uint8_t {
    v = std::max(0.0f, std::min(1.0f, v));
    return static_cast<uint8_t>(std::lround(v * 255.0f));
  };

  auto& pad = slot.state.gamepad;
  uint16_t prev_buttons = static_cast<uint16_t>(pad.buttons);
  pad.buttons = buttons;
  pad.left_trigger  = to_trigger(gp.leftTrigger.value);
  pad.right_trigger = to_trigger(gp.rightTrigger.value);
  pad.thumb_lx = to_axis(gp.leftThumbstick.xAxis.value);
  pad.thumb_ly = to_axis(gp.leftThumbstick.yAxis.value);
  pad.thumb_rx = to_axis(gp.rightThumbstick.xAxis.value);
  pad.thumb_ry = to_axis(gp.rightThumbstick.yAxis.value);

  if (prev_buttons != buttons || slot.state_changed) {
    slot.state.packet_number++;
    slot.state_changed = false;
  }
  slot.last_buttons = buttons;
}

X_RESULT IOSInputDriver::GetCapabilities(uint32_t user_index, uint32_t /*flags*/,
                                         X_INPUT_CAPABILITIES* out_caps) {
  if (user_index >= HID_IOS_USER_COUNT || !out_caps) return X_ERROR_BAD_ARGUMENTS;
  std::lock_guard<std::mutex> g(mutex_);
  auto* slot = FindSlot(user_index);
  if (!slot) return X_ERROR_DEVICE_NOT_CONNECTED;

  std::memset(out_caps, 0, sizeof(*out_caps));
  out_caps->type = 0x01;
  out_caps->sub_type = 0x01;
  out_caps->flags = 0;
  out_caps->gamepad.buttons = 0xF3FF;
  out_caps->gamepad.left_trigger = 0xFF;
  out_caps->gamepad.right_trigger = 0xFF;
  out_caps->gamepad.thumb_lx = static_cast<int16_t>(0xFFFFu);
  out_caps->gamepad.thumb_ly = static_cast<int16_t>(0xFFFFu);
  out_caps->gamepad.thumb_rx = static_cast<int16_t>(0xFFFFu);
  out_caps->gamepad.thumb_ry = static_cast<int16_t>(0xFFFFu);
  out_caps->vibration.left_motor_speed = 0xFFFFu;
  out_caps->vibration.right_motor_speed = 0xFFFFu;
  return X_ERROR_SUCCESS;
}

X_RESULT IOSInputDriver::GetState(uint32_t user_index, X_INPUT_STATE* out_state) {
  if (user_index >= HID_IOS_USER_COUNT) return X_ERROR_BAD_ARGUMENTS;
  std::lock_guard<std::mutex> g(mutex_);
  auto* slot = FindSlot(user_index);
  if (!slot) return X_ERROR_DEVICE_NOT_CONNECTED;

  SampleLocked(*slot);

  if (out_state) {
    if (is_active()) {
      std::memcpy(out_state, &slot->state, sizeof(*out_state));
    } else {
      std::memcpy(out_state, &slot->state, sizeof(*out_state));
      std::memset(&out_state->gamepad, 0, sizeof(out_state->gamepad));
    }
  }
  return X_ERROR_SUCCESS;
}

X_RESULT IOSInputDriver::SetState(uint32_t user_index, X_INPUT_VIBRATION* vibration) {
  if (user_index >= HID_IOS_USER_COUNT || !vibration) return X_ERROR_BAD_ARGUMENTS;
  std::lock_guard<std::mutex> g(mutex_);
  auto* slot = FindSlot(user_index);
  if (!slot) return X_ERROR_DEVICE_NOT_CONNECTED;
  if (!slot->ios || !slot->ios->bridge_retained) return X_ERROR_SUCCESS;

  if (@available(iOS 14.0, tvOS 14.0, *)) {
    LRIOSControllerBridge* bridge =
        (__bridge LRIOSControllerBridge*)slot->ios->bridge_retained;
    float left  = vibration->left_motor_speed  / 65535.0f;
    float right = vibration->right_motor_speed / 65535.0f;
    [bridge rumbleLeft:left right:right];
  }
  return X_ERROR_SUCCESS;
}

X_RESULT IOSInputDriver::GetKeystroke(uint32_t user_index, uint32_t /*flags*/,
                                      X_INPUT_KEYSTROKE* out_keystroke) {
  // The iOS driver doesn't translate analog-to-keystroke events; games
  // that need menu navigation fall back to the higher-level input system
  // or the MnK driver. Report empty so the system moves on.
  if (user_index != 0xFF && user_index >= HID_IOS_USER_COUNT) {
    return X_ERROR_BAD_ARGUMENTS;
  }
  if (out_keystroke) std::memset(out_keystroke, 0, sizeof(*out_keystroke));
  return X_ERROR_EMPTY;
}

}  // namespace rex::input::ios

#endif  // REX_PLATFORM_IOS
