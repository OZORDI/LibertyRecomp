// Adaptive trigger support for DualSense on iOS via GameController.framework.
//
// GameController.framework exposes DualSense adaptive triggers starting in
// iOS 14.5 via GCDualSenseGamepad / GCDualSenseAdaptiveTrigger.
//
// Docs:
//   developer.apple.com/documentation/gamecontroller/gcdualsenseadaptivetrigger
//   - setModeFeedbackWithStartPosition:resistiveStrength:
//   - setModeWeaponWithStartPosition:endPosition:resistiveStrength:
//   - setModeVibrationWithStartPosition:amplitude:frequency:
//
// This file mirrors the profile-based API in hid/dualsense.cpp but targets a
// specific GCController* supplied by the iOS HID bridge rather than the
// SDL-tracked "active" controller used on desktop.

#include "adaptive_triggers_ios.h"

#if TARGET_OS_IOS

#import <Foundation/Foundation.h>
#import <GameController/GameController.h>

#include <hid/dualsense.h>   // WEAPON_PROFILES / VEHICLE_PROFILES tables
#include <algorithm>
#include <cmath>

// --------------------------------------------------------------------------
// Helpers
// --------------------------------------------------------------------------

static inline float clamp01(float v)
{
    if (v < 0.0f) return 0.0f;
    if (v > 1.0f) return 1.0f;
    return v;
}

// Convert a GCController* opaque pointer to a GCDualSenseGamepad* if possible.
// Returns nil when:
//   - controller is null
//   - running on iOS < 14.5
//   - attached controller is not a DualSense
static GCDualSenseGamepad* GetDualSenseGamepad(void* controller) API_AVAILABLE(ios(14.5))
{
    if (controller == nullptr) return nil;

    GCController* gc = (__bridge GCController*)controller;
    if (gc == nil) return nil;

    // extendedGamepad returns the most-derived profile; cast checks the
    // concrete DualSense profile class.
    GCExtendedGamepad* ext = gc.extendedGamepad;
    if (![ext isKindOfClass:[GCDualSenseGamepad class]]) {
        return nil;
    }
    return (GCDualSenseGamepad*)ext;
}

static GCDualSenseAdaptiveTrigger* PickTrigger(GCDualSenseGamepad* ds, int side) API_AVAILABLE(ios(14.5))
{
    if (ds == nil) return nil;
    return (side == 0) ? ds.leftTrigger : ds.rightTrigger;
}

// --------------------------------------------------------------------------
// C API
// --------------------------------------------------------------------------

extern "C" bool ios_adaptive_trigger_is_dualsense(void* controller)
{
    if (@available(iOS 14.5, *)) {
        return GetDualSenseGamepad(controller) != nil;
    }
    return false;
}

extern "C" void ios_adaptive_trigger_set_feedback(void* controller, int side,
                                                  float start_pos, float strength)
{
    if (@available(iOS 14.5, *)) {
        GCDualSenseGamepad* ds = GetDualSenseGamepad(controller);
        GCDualSenseAdaptiveTrigger* t = PickTrigger(ds, side);
        if (t == nil) return;
        [t setModeFeedbackWithStartPosition:clamp01(start_pos)
                          resistiveStrength:clamp01(strength)];
    }
}

extern "C" void ios_adaptive_trigger_set_weapon(void* controller, int side,
                                                float start, float end, float strength)
{
    if (@available(iOS 14.5, *)) {
        GCDualSenseGamepad* ds = GetDualSenseGamepad(controller);
        GCDualSenseAdaptiveTrigger* t = PickTrigger(ds, side);
        if (t == nil) return;

        float s = clamp01(start);
        float e = clamp01(end);
        // Ensure ordering; weapon mode requires start < end.
        if (e <= s) e = std::min(1.0f, s + 0.01f);

        [t setModeWeaponWithStartPosition:s
                              endPosition:e
                        resistiveStrength:clamp01(strength)];
    }
}

extern "C" void ios_adaptive_trigger_set_vibration(void* controller, int side,
                                                   float start, float amp, float freq)
{
    if (@available(iOS 14.5, *)) {
        GCDualSenseGamepad* ds = GetDualSenseGamepad(controller);
        GCDualSenseAdaptiveTrigger* t = PickTrigger(ds, side);
        if (t == nil) return;

        // Apple's API takes frequency in Hz directly as a float.
        float f = freq < 0.0f ? 0.0f : freq;
        [t setModeVibrationWithStartPosition:clamp01(start)
                                   amplitude:clamp01(amp)
                                   frequency:f];
    }
}

extern "C" void ios_adaptive_trigger_reset(void* controller, int side)
{
    if (@available(iOS 14.5, *)) {
        GCDualSenseGamepad* ds = GetDualSenseGamepad(controller);
        GCDualSenseAdaptiveTrigger* t = PickTrigger(ds, side);
        if (t == nil) return;
        [t setModeOff];
    }
}

extern "C" void ios_adaptive_trigger_reset_all(void* controller)
{
    if (@available(iOS 14.5, *)) {
        GCDualSenseGamepad* ds = GetDualSenseGamepad(controller);
        if (ds == nil) return;
        [ds.leftTrigger  setModeOff];
        [ds.rightTrigger setModeOff];
    }
}

// --------------------------------------------------------------------------
// High-level profile bridges — mirror hid/dualsense.cpp behaviour.
// We translate the table's 0..255 byte scales into Apple's 0..1 normalised
// floats so gameplay code can use the same profile indices on every platform.
// --------------------------------------------------------------------------

static inline float byteToUnit(uint8_t v)
{
    return static_cast<float>(v) / 255.0f;
}

extern "C" void ios_adaptive_trigger_apply_weapon_profile(void* controller, int weaponSlot)
{
    if (!(@available(iOS 14.5, *))) return;

    GCDualSenseGamepad* ds = GetDualSenseGamepad(controller);
    if (ds == nil) return;

    constexpr int kNumProfiles =
        static_cast<int>(sizeof(DualSense::WEAPON_PROFILES) /
                         sizeof(DualSense::WEAPON_PROFILES[0]));
    if (weaponSlot < 0 || weaponSlot >= kNumProfiles) weaponSlot = 0;

    const auto& p = DualSense::WEAPON_PROFILES[weaponSlot];

    // R2: fire trigger
    if (p.strength == 0) {
        [ds.rightTrigger setModeOff];
    } else if (p.isAutomatic) {
        float freq = static_cast<float>(p.recoilFrequency); // already in Hz
        [ds.rightTrigger setModeVibrationWithStartPosition:byteToUnit(p.startPosition)
                                                 amplitude:byteToUnit(p.strength)
                                                 frequency:freq];
    } else {
        [ds.rightTrigger setModeWeaponWithStartPosition:byteToUnit(p.startPosition)
                                            endPosition:1.0f
                                      resistiveStrength:byteToUnit(p.strength)];
    }

    // L2: light ADS resistance at 1/3 strength
    if (p.strength > 0) {
        [ds.leftTrigger setModeFeedbackWithStartPosition:0.0f
                                        resistiveStrength:byteToUnit(p.strength) / 3.0f];
    } else {
        [ds.leftTrigger setModeOff];
    }
}

extern "C" void ios_adaptive_trigger_apply_vehicle_profile(void* controller,
                                                           int vehicleClass,
                                                           int speed,
                                                           int throttle,
                                                           int brake)
{
    if (!(@available(iOS 14.5, *))) return;

    GCDualSenseGamepad* ds = GetDualSenseGamepad(controller);
    if (ds == nil) return;

    constexpr int kNumProfiles =
        static_cast<int>(sizeof(DualSense::VEHICLE_PROFILES) /
                         sizeof(DualSense::VEHICLE_PROFILES[0]));
    if (vehicleClass < 0 || vehicleClass >= kNumProfiles) vehicleClass = 0;

    speed    = std::clamp(speed,    0, 255);
    throttle = std::clamp(throttle, 0, 255);
    brake    = std::clamp(brake,    0, 255);

    const auto& p = DualSense::VEHICLE_PROFILES[vehicleClass];

    // Throttle (R2) — feedback scaled by throttle input
    uint8_t throttleResistance = p.throttleBase;
    if (throttle > 0) {
        uint8_t extra = static_cast<uint8_t>(
            (p.throttleMax - p.throttleBase) * throttle / 255);
        throttleResistance = static_cast<uint8_t>(p.throttleBase + extra);
    }
    [ds.rightTrigger setModeFeedbackWithStartPosition:byteToUnit(p.throttleStart)
                                    resistiveStrength:byteToUnit(throttleResistance)];

    // Brake (L2) — feedback scaled by speed, with ABS vibration at high speed
    uint8_t brakeResistance = p.brakeBase;
    if (speed > 0) {
        uint8_t extra = static_cast<uint8_t>(
            (p.brakeMax - p.brakeBase) * speed / 255);
        brakeResistance = static_cast<uint8_t>(p.brakeBase + extra);
    }

    if (speed > p.absThreshold && brake > 180) {
        [ds.leftTrigger setModeVibrationWithStartPosition:byteToUnit(p.brakeStart)
                                                amplitude:byteToUnit(brakeResistance)
                                                frequency:static_cast<float>(p.absFrequency)];
    } else {
        [ds.leftTrigger setModeFeedbackWithStartPosition:byteToUnit(p.brakeStart)
                                        resistiveStrength:byteToUnit(brakeResistance)];
    }
}

#endif // TARGET_OS_IOS
