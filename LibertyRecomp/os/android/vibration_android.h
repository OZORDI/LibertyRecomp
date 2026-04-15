// Android VibratorManager JNI bridge for dual-motor controller rumble.
// Targets API 31+ (VibratorManager per-motor) with fallback to legacy
// InputDevice.getVibrator() on older devices.
#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

// Motor speeds are the XInput-style uint16 range (0..65535).
// They are right-shifted by 8 to produce an Android amplitude in 0..255.
// low_freq  -> "left"  / strong motor (typically vibrator index 0)
// high_freq -> "right" / weak motor   (typically vibrator index 1)
// duration_ms of 0 is treated as a stop.
void android_vibration_set_rumble(int32_t device_id,
                                  uint16_t low_freq,
                                  uint16_t high_freq,
                                  uint32_t duration_ms);

// Stop any in-flight vibration on both motors for the given input device.
void android_vibration_stop(int32_t device_id);

#ifdef __cplusplus
} // extern "C"
#endif
