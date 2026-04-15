#pragma once

// iOS Core Haptics + GCController vibration bridge for LibertyRecomp.
//
// Provides C++ wrappers over CHHapticEngine objects vended via
// GCController.haptics (GCDeviceHaptics). Xbox-style rumble motor
// speeds (uint16_t, 0-65535) are mapped to CHHapticEvent intensity
// parameters on separate left/right-handle haptic localities so that
// traditional dual-motor rumble feels correct on MFi/Xbox/PS controllers
// exposed through the GameController framework.
//
// Safe to call from any thread: dispatches engine work onto the main
// queue internally. All functions are no-ops on non-iOS builds or when
// the controller has no haptics support.

#include <cstdint>

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

// Opaque handle type — the actual Objective-C++ IOSHapticsEngine class
// is only visible to haptics_ios.mm. Consumers (e.g. sdl_hid.cpp) hold
// a void* and drive it through the C API below.
typedef void* LRHapticsHandle;

#if defined(__APPLE__) && (TARGET_OS_IOS || TARGET_OS_TV)

#ifdef __OBJC__
@class GCController;
#else
typedef struct objc_object GCController;
#endif

namespace lr::haptics {

// Create a haptics engine bound to the given GCController. Returns
// nullptr if the controller lacks haptics support (controller.haptics
// is nil) or if engine creation fails for all localities.
//
// Must be called on the main thread, or will internally dispatch sync
// to the main queue. Safe to call multiple times for different
// controllers — each returns an independent handle.
LRHapticsHandle Create(GCController* controller);

// Start a continuous rumble on both motors for `durationSec` seconds.
// leftMotor/rightMotor are Xbox-convention 0-65535 speeds and are
// linearly mapped to CHHapticEventParameterIDHapticIntensity 0.0-1.0.
// Passing 0/0 stops any currently-playing rumble.
//
// If `durationSec` <= 0 or > 30.0 it is clamped to (0.1, 30.0].
// Safe to call from any thread.
void SetRumble(LRHapticsHandle handle,
               uint16_t leftMotor,
               uint16_t rightMotor,
               float durationSec);

// Immediately stop any playing rumble on both motors.
void Stop(LRHapticsHandle handle);

// Tear down the engines and release the handle. Handle is invalid
// after this call.
void Destroy(LRHapticsHandle handle);

} // namespace lr::haptics

#else // not iOS/tvOS — stub out so callers compile on every platform

namespace lr::haptics {
inline LRHapticsHandle Create(void*) { return nullptr; }
inline void SetRumble(LRHapticsHandle, uint16_t, uint16_t, float) {}
inline void Stop(LRHapticsHandle) {}
inline void Destroy(LRHapticsHandle) {}
} // namespace lr::haptics

#endif
