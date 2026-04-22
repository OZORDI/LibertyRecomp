#pragma once

#include <rex/platform.h>

#if REX_PLATFORM_IOS

#ifdef __cplusplus
extern "C" {
#endif

// Side selector
//   0 = left  trigger (L2)
//   1 = right trigger (R2)

// Returns true if the supplied GCController* is a GCDualSenseGamepad on iOS 14.5+.
// `controller` is an opaque (__bridge) GCController* pointer.
bool ios_adaptive_trigger_is_dualsense(void* controller);

// Mode: Feedback — uniform resistance from `start_pos` to full pull.
//   start_pos : 0.0 .. 1.0   (trigger travel where resistance begins)
//   strength  : 0.0 .. 1.0   (resistive strength)
void ios_adaptive_trigger_set_feedback(void* controller, int side,
                                       float start_pos, float strength);

// Mode: Weapon — gun-trigger snap between `start` and `end`.
//   start, end : 0.0 .. 1.0  (start < end)
//   strength   : 0.0 .. 1.0
void ios_adaptive_trigger_set_weapon(void* controller, int side,
                                     float start, float end, float strength);

// Mode: Vibration — rapid pulses past `start`.
//   start     : 0.0 .. 1.0
//   amplitude : 0.0 .. 1.0
//   frequency : Hz (typical range 1..50)
void ios_adaptive_trigger_set_vibration(void* controller, int side,
                                        float start, float amp, float freq);

// Reset the specified trigger to its default (no effect) state.
void ios_adaptive_trigger_reset(void* controller, int side);

// Reset both triggers.
void ios_adaptive_trigger_reset_all(void* controller);

// --------------------------------------------------------------------------
// High-level profile API (mirrors hid/dualsense.cpp ApplyWeaponProfile /
// ApplyVehicleProfile semantics, but targets a specific iOS controller).
// --------------------------------------------------------------------------

// Apply a weapon profile to R2 (fire) and a light ADS resistance to L2.
//   weaponSlot indexes hid/dualsense.h WEAPON_PROFILES[].
void ios_adaptive_trigger_apply_weapon_profile(void* controller, int weaponSlot);

// Apply a vehicle profile (throttle on R2, brake on L2 with ABS pulse).
void ios_adaptive_trigger_apply_vehicle_profile(void* controller,
                                                int vehicleClass,
                                                int speed,      // 0..255
                                                int throttle,   // 0..255
                                                int brake);     // 0..255

#ifdef __cplusplus
}
#endif

#endif // REX_PLATFORM_IOS
