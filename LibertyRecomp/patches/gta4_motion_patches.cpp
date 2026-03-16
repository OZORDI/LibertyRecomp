// =============================================================================
// GTA IV Motion Control Patches
// =============================================================================
// Implements PS3 Sixaxis motion control script commands for PlayStation controllers
// 
// These hooks intercept the stubbed motion control functions from the Xbox 360
// version and provide actual motion data from DualShock 3/4 and DualSense controllers.
//
// Script Commands Hooked:
// - GET_MOTION_SENSOR_VALUES: Returns accelerometer values (x, y, z)
// - GET_MOTION_CONTROLS_ENABLED: Returns whether motion controls are available
// - HAS_RELOADED_WITH_MOTION_CONTROL: Returns if player reloaded using motion
// - GET_MOTION_CONTROL_PREFERENCE: Returns user's motion control preference
// - SET_ALL_MOTION_CONTROL_PREFERENCES_ON_OFF: Enables/disables motion prefs
// - GET_PAD_ORIENTATION: Returns controller orientation
// - GET_PAD_PITCH_ROLL: Returns pitch and roll angles
// - GET_POSITION_OF_ANALOGUE_STICKS: Hooked for motion steering blend
//
// Original function addresses (from PPC recompiled code):
// - sub_8259EF90: Generic stub returning 0 (used by multiple commands)
// - sub_82593700: GET_MOTION_CONTROLS_ENABLED stub (stores to two output ptrs)
// - sub_82593668: GET_POSITION_OF_ANALOGUE_STICKS (hooked for steering blend)
// - nullsub_1: SET_ALL_MOTION_CONTROL_PREFERENCES_ON_OFF (no-op)
//
// Script Context Structure (r3):
// - r3 + 0  → Pointer to return value location (for simple returns)
// - r3 + 8  → Pointer to parameter/output array structure
//
// Parameter Array Structure (at *(r3+8)):
// - +0x00 → First parameter or output pointer
// - +0x04 → Second parameter or output pointer
// - +0x08 → Third parameter or output pointer
// - +0x0C → Fourth parameter or output pointer
// - +0x10 → Fifth parameter or output pointer
// =============================================================================

#include <stdafx.h>
#include <cpu/ppc_context.h>
#include <kernel/function.h>
#include <hid/hid.h>
#include <os/logger.h>
#include <user/config.h>
#include <SDL.h>
#include <cmath>
#include <algorithm>  // for std::clamp

// Debug logging control - set to 1 to enable verbose logging
#define MOTION_PATCHES_DEBUG_LOGGING 0

// Vehicle state detection (set by game hooks)
namespace VehicleState {
    static bool s_isInVehicle = false;
    static bool s_isInHelicopter = false;
    static bool s_isInBoat = false;
    static bool s_isAiming = false;
    static bool s_isOnMotorcycle = false;  // Phase 2: Motorcycle tracking
    
    void SetInVehicle(bool inVehicle) { s_isInVehicle = inVehicle; }
    void SetInHelicopter(bool inHeli) { s_isInHelicopter = inHeli; }
    void SetInBoat(bool inBoat) { s_isInBoat = inBoat; }
    void SetAiming(bool aiming) { s_isAiming = aiming; }
    void SetOnMotorcycle(bool onBike) { s_isOnMotorcycle = onBike; }
    
    bool IsInVehicle() { return s_isInVehicle; }
    bool IsInHelicopter() { return s_isInHelicopter; }
    bool IsInBoat() { return s_isInBoat; }
    bool IsAiming() { return s_isAiming; }
    bool IsOnMotorcycle() { return s_isOnMotorcycle; }
}

namespace MotionPatches {

// ============================================================================
// State Tracking
// ============================================================================

static bool s_motionReloadTriggered = false;
static bool s_motionPreferencesEnabled = true;  // User preference for motion controls

// Shake detection state
static float s_lastAccelMagnitude = 1.0f;
static int s_shakeCount = 0;
static uint64_t s_lastShakeTime = 0;

// Motion data smoothing
static float s_smoothedAccelX = 0.0f;
static float s_smoothedAccelY = 0.0f;
static float s_smoothedAccelZ = -1.0f;  // Gravity at rest

// Phase 2: Wheelie balance state
static bool s_performingWheelie = false;
static float s_wheelieBalanceInput = 0.0f;

// ============================================================================
// Helper Functions
// ============================================================================

// Convert SDL motion data to PS3 SIXAXIS format
// PS3 SIXAXIS uses a different coordinate system and scale
// X = left(-)/right(+), Y = back(-)/forward(+), Z = down(-)/up(+)
// Range: approximately -1.0 to +1.0 for gravity

inline void ConvertToSixaxisFormat(float& x, float& y, float& z)
{
    // SDL: X right, Y forward, Z up (m/s²)
    // PS3: X right, Y back, Z down (g-forces, already converted by SDL driver)
    // Note: SDL driver already converts to g-forces in our implementation
    
    // Swap Y direction (SDL forward+ → PS3 forward+, but PS3 Y is back+)
    y = -y;
    // No change needed for X
    // Z is already correct (gravity pulls down)
}

// Apply exponential smoothing to reduce sensor noise
inline float SmoothValue(float current, float target, float alpha)
{
    return alpha * target + (1.0f - alpha) * current;
}

// ============================================================================
// sub_8259EF90 - Generic Stub Override
// ============================================================================
// This stub is used for:
// - GET_MOTION_SENSOR_VALUES
// - GET_PAD_ORIENTATION
// - GET_PAD_PITCH_ROLL
// - HAS_RELOADED_WITH_MOTION_CONTROL
// - GET_MOTION_CONTROL_PREFERENCE
//
// Since this stub is shared, we check if motion sensing is available and
// return appropriate values. For more specific behavior, we may need to
// hook the individual script command callers.

extern "C" void __imp__sub_8259EF90(PPCContext& ctx, uint8_t* base);

PPC_FUNC_HOOK(sub_8259EF90)
{
    PPC_FUNC_PROLOGUE();
    
    // The original stub just stores 0 to *(*(r3+0))
    // Get return pointer (following the exact pattern from PPC code)
    uint32_t ptrToReturnLoc = PPC_LOAD_U32(ctx.r3.u32 + 0);
    
    // Default: return 0 (motion not available / false / no reload)
    int32_t result = 0;
    
    // Check if we have motion sensing available and enabled
    if (hid::HasMotionSensor() && hid::IsMotionSensorEnabled() && 
        Config::MotionControlsEnabled && s_motionPreferencesEnabled) {
        
        // For HAS_RELOADED_WITH_MOTION_CONTROL, check reload state
        if (s_motionReloadTriggered && Config::MotionReload) {
            result = 1;
            s_motionReloadTriggered = false;  // Clear after reading
        } else {
            // For GET_MOTION_CONTROL_PREFERENCE, return enabled state
            result = 1;
        }
    }
    
    // Store result (big-endian u32)
    PPC_STORE_U32(ptrToReturnLoc, static_cast<uint32_t>(result));
    
#if MOTION_PATCHES_DEBUG_LOGGING
    static int s_callCount = 0;
    if (++s_callCount <= 10) {
        LOGFN("[MotionPatch] sub_8259EF90: returning {}", result);
    }
#endif
}

// ============================================================================
// sub_82593700 - GET_MOTION_CONTROLS_ENABLED
// ============================================================================
// Returns two boolean values:
// - Output 1 (at paramStruct+0): Motion controls enabled (bool)
// - Output 2 (at paramStruct+4): Motion controls available/hardware present (bool)
//
// Original PPC assembly (from ppc_recomp.63.cpp line 21644):
//   lwz r11, 8(r3)      ; Get param struct pointer
//   li r9, 0            ; Default = false
//   lwz r10, 4(r11)     ; Get second output pointer (available)
//   lwz r11, 0(r11)     ; Get first output pointer (enabled)
//   stb r9, 0(r11)      ; Store false to first output
//   stb r9, 0(r10)      ; Store false to second output

extern "C" void __imp__sub_82593700(PPCContext& ctx, uint8_t* base);

PPC_FUNC_HOOK(sub_82593700)
{
    PPC_FUNC_PROLOGUE();
    
    // Follow the exact PPC memory access pattern
    // Get the parameter structure pointer from r3+8
    uint32_t paramStructAddr = PPC_LOAD_U32(ctx.r3.u32 + 8);
    
    // Get output pointers (order matches PPC: r10=+4, r11=+0)
    uint32_t output2Addr = PPC_LOAD_U32(paramStructAddr + 4);  // Second output (available)
    uint32_t output1Addr = PPC_LOAD_U32(paramStructAddr + 0);  // First output (enabled)
    
    // Check if motion controls are available and enabled
    bool hasMotionHardware = hid::HasMotionSensor();
    bool configEnabled = Config::MotionControlsEnabled;
    bool sensorActive = hid::IsMotionSensorEnabled();
    bool userPrefEnabled = s_motionPreferencesEnabled;
    
    // Motion is "enabled" only if all conditions are met
    bool motionEnabled = hasMotionHardware && configEnabled && sensorActive && userPrefEnabled;
    
    // Store results (as bytes, matching stb instruction)
    PPC_STORE_U8(output1Addr, motionEnabled ? 1 : 0);     // Motion controls enabled
    PPC_STORE_U8(output2Addr, hasMotionHardware ? 1 : 0); // Motion hardware available
    
#if MOTION_PATCHES_DEBUG_LOGGING
    static int s_callCount = 0;
    if (++s_callCount <= 10) {
        LOGFN("[MotionPatch] GET_MOTION_CONTROLS_ENABLED: enabled={}, hardware={}, config={}, sensor={}, pref={}",
              motionEnabled, hasMotionHardware, configEnabled, sensorActive, userPrefEnabled);
    }
#endif
}

// ============================================================================
// Motion Sensor Value API
// ============================================================================
// These functions provide motion data to the game in the expected format.
// They can be called from script command hooks or vehicle control patches.

// Get smoothed accelerometer values in SIXAXIS format
void GetMotionSensorValues(float* outX, float* outY, float* outZ)
{
    if (!hid::HasMotionSensor() || !hid::IsMotionSensorEnabled() || !Config::MotionControlsEnabled) {
        *outX = 0.0f;
        *outY = 0.0f;
        *outZ = -1.0f;  // Gravity at rest
        return;
    }
    
    const auto& motion = hid::GetMotionState();
    
    // Apply smoothing to reduce sensor noise (exponential moving average)
    constexpr float SMOOTH_ALPHA = 0.3f;  // Lower = more smoothing
    s_smoothedAccelX = SmoothValue(s_smoothedAccelX, motion.accelX, SMOOTH_ALPHA);
    s_smoothedAccelY = SmoothValue(s_smoothedAccelY, motion.accelY, SMOOTH_ALPHA);
    s_smoothedAccelZ = SmoothValue(s_smoothedAccelZ, motion.accelZ, SMOOTH_ALPHA);
    
    // Apply sensitivity scaling
    float sensitivity = Config::MotionSensitivity;
    
    // Return smoothed accelerometer values (in g-forces, matching PS3 format)
    // PS3 SIXAXIS coordinate system: X = left/right tilt, Y = forward/back tilt, Z = up/down
    *outX = s_smoothedAccelX * sensitivity;
    *outY = s_smoothedAccelY * sensitivity;
    *outZ = s_smoothedAccelZ;  // Don't scale Z (gravity reference)
    
    // Convert to SIXAXIS format if needed
    ConvertToSixaxisFormat(*outX, *outY, *outZ);
    
#if MOTION_PATCHES_DEBUG_LOGGING
    static int s_callCount = 0;
    if (++s_callCount % 60 == 0) {  // Log every ~1 second at 60fps
        LOGFN("[MotionPatch] Motion values: X={:.3f} Y={:.3f} Z={:.3f} (sens={:.2f})", 
              *outX, *outY, *outZ, sensitivity);
    }
#endif
}

// Get controller orientation (pitch/roll) for script commands
void GetPadOrientation(float* outPitch, float* outRoll)
{
    if (!hid::HasMotionSensor() || !hid::IsMotionSensorEnabled() || !Config::MotionControlsEnabled) {
        *outPitch = 0.0f;
        *outRoll = 0.0f;
        return;
    }
    
    const auto& motion = hid::GetMotionState();
    
    // Apply sensitivity to orientation
    float sensitivity = Config::MotionSensitivity;
    
    // Return pitch and roll in radians, scaled by sensitivity
    *outPitch = motion.pitch * sensitivity;
    *outRoll = motion.roll * sensitivity;
    
#if MOTION_PATCHES_DEBUG_LOGGING
    static int s_callCount = 0;
    if (++s_callCount % 60 == 0) {
        LOGFN("[MotionPatch] Orientation: pitch={:.3f} roll={:.3f}", *outPitch, *outRoll);
    }
#endif
}

// Get pitch and roll in the format expected by GET_PAD_PITCH_ROLL
void GetPadPitchRoll(float* outPitch, float* outRoll)
{
    // This is effectively the same as GetPadOrientation, but may use different
    // normalization in the future if needed
    GetPadOrientation(outPitch, outRoll);
}

// ============================================================================
// SET_ALL_MOTION_CONTROL_PREFERENCES_ON_OFF Hook
// ============================================================================
// Original: nullsub_1 (does nothing)
// We use this to toggle motion control preferences globally.
// Note: nullsub_1 is used by many commands, so we provide an API instead.

void SetMotionControlPreference(bool enabled)
{
    s_motionPreferencesEnabled = enabled;
    
    // Enable/disable the actual sensor when preference changes
    if (hid::HasMotionSensor()) {
        hid::SetMotionSensorEnabled(enabled && Config::MotionControlsEnabled);
    }
    
    LOGFN("Motion control preferences set to: {}", enabled ? "ON" : "OFF");
}

bool GetMotionControlPreference()
{
    return s_motionPreferencesEnabled && Config::MotionControlsEnabled;
}

// ============================================================================
// Motion Reload Detection (Shake-to-Reload)
// ============================================================================
// For HAS_RELOADED_WITH_MOTION_CONTROL - detect controller shake gesture
// Similar to PS3 GTA IV which allowed reloading by shaking the controller

void TriggerMotionReload()
{
    s_motionReloadTriggered = true;
    s_lastShakeTime = SDL_GetTicks64();
    
#if MOTION_PATCHES_DEBUG_LOGGING
    LOGFN("[MotionPatch] Motion reload triggered!");
#endif
}

bool HasReloadedWithMotionControl()
{
    bool result = s_motionReloadTriggered;
    s_motionReloadTriggered = false;  // Clear after reading
    return result;
}

// Improved shake detection with temporal filtering
// Uses a state machine approach for more reliable gesture detection
void UpdateMotionReloadDetection()
{
    // Check all prerequisites
    if (!hid::HasMotionSensor() || !hid::IsMotionSensorEnabled() || 
        !s_motionPreferencesEnabled || !Config::MotionReload) {
        s_shakeCount = 0;
        return;
    }
    
    const auto& motion = hid::GetMotionState();
    
    // Calculate total acceleration magnitude
    float accelMagnitude = sqrtf(motion.accelX * motion.accelX + 
                                  motion.accelY * motion.accelY + 
                                  motion.accelZ * motion.accelZ);
    
    // Detect shake gesture: high acceleration that oscillates rapidly
    // A shake typically produces acceleration peaks alternating above and below 1g
    constexpr float SHAKE_THRESHOLD_HIGH = 2.0f;   // Peak acceleration (2g)
    constexpr float SHAKE_THRESHOLD_LOW = 0.3f;    // Minimum change to count
    constexpr int SHAKE_COUNT_REQUIRED = 3;        // Number of oscillations needed
    constexpr uint64_t SHAKE_TIMEOUT_MS = 500;     // Time window for shake detection
    constexpr uint64_t SHAKE_COOLDOWN_MS = 200;    // Cooldown after triggering
    
    uint64_t now = SDL_GetTicks64();
    
    // Cooldown after reload to prevent retriggering
    if (s_lastShakeTime > 0 && (now - s_lastShakeTime) < SHAKE_COOLDOWN_MS) {
        return;
    }
    
    // Detect rapid changes in acceleration (shake oscillation)
    float delta = fabsf(accelMagnitude - s_lastAccelMagnitude);
    
    if (delta > SHAKE_THRESHOLD_LOW && accelMagnitude > SHAKE_THRESHOLD_HIGH) {
        s_shakeCount++;
        
#if MOTION_PATCHES_DEBUG_LOGGING
        LOGFN("[MotionPatch] Shake detected: count={}, magnitude={:.2f}, delta={:.2f}", 
              s_shakeCount, accelMagnitude, delta);
#endif
        
        if (s_shakeCount >= SHAKE_COUNT_REQUIRED) {
            TriggerMotionReload();
            s_shakeCount = 0;
        }
    } else if (delta < 0.05f) {
        // Controller is stable - reset shake count if too much time passed
        if (s_shakeCount > 0) {
            // Allow some time for oscillations
            static uint64_t s_firstShakeTime = 0;
            if (s_firstShakeTime == 0) {
                s_firstShakeTime = now;
            } else if ((now - s_firstShakeTime) > SHAKE_TIMEOUT_MS) {
                s_shakeCount = 0;
                s_firstShakeTime = 0;
            }
        }
    }
    
    s_lastAccelMagnitude = accelMagnitude;
}

// ============================================================================
// Vehicle Motion Steering (for Config::MotionSteering)
// ============================================================================
// Applies motion tilt to vehicle steering when enabled

float GetMotionSteeringInput()
{
    if (!Config::MotionSteering || !hid::HasMotionSensor() || !hid::IsMotionSensorEnabled()) {
        return 0.0f;
    }
    
    const auto& motion = hid::GetMotionState();
    
    // Use roll (left/right tilt) for steering
    // Clamp to reasonable range (-45 to +45 degrees = -0.78 to +0.78 radians)
    float steeringInput = motion.roll * Config::MotionSensitivity;
    steeringInput = std::clamp(steeringInput, -0.78f, 0.78f);
    
    // Normalize to -1 to +1 range
    return steeringInput / 0.78f;
}

// ============================================================================
// Helicopter Motion Control (for Config::MotionHelicopter)
// ============================================================================
// Applies motion tilt to helicopter pitch/roll when enabled

void GetMotionHelicopterInput(float* outPitch, float* outRoll)
{
    if (!Config::MotionHelicopter || !hid::HasMotionSensor() || !hid::IsMotionSensorEnabled()) {
        *outPitch = 0.0f;
        *outRoll = 0.0f;
        return;
    }
    
    const auto& motion = hid::GetMotionState();
    
    // Use pitch/roll from motion sensor with sensitivity scaling
    float sensitivity = Config::MotionSensitivity * 0.5f;  // Helicopters need gentler input
    
    *outPitch = motion.pitch * sensitivity;
    *outRoll = motion.roll * sensitivity;
    
    // Clamp to prevent extreme inputs
    *outPitch = std::clamp(*outPitch, -1.0f, 1.0f);
    *outRoll = std::clamp(*outRoll, -1.0f, 1.0f);
}

// ============================================================================
// Gyro Aiming (for Config::MotionAiming)
// ============================================================================
// Provides fine aim adjustment using gyroscope

void GetMotionAimOffset(float* outX, float* outY)
{
    if (!Config::MotionAiming || !hid::HasMotionSensor() || !hid::IsMotionSensorEnabled()) {
        *outX = 0.0f;
        *outY = 0.0f;
        return;
    }
    
    const auto& motion = hid::GetMotionState();
    
    // Use gyro rates for aim offset (angular velocity)
    // This provides responsive, relative aim adjustment
    float sensitivity = Config::MotionSensitivity * 0.02f;  // Fine adjustment
    
    // Yaw (Y rotation) controls horizontal aim, Pitch (X rotation) controls vertical
    *outX = motion.gyroY * sensitivity;  // Horizontal
    *outY = -motion.gyroX * sensitivity; // Vertical (inverted for natural feel)
}

// ============================================================================
// Phase 2: Motorcycle Wheelie Balance
// ============================================================================
// Uses controller pitch to help balance during wheelies

void SetWheelieState(bool performingWheelie)
{
    s_performingWheelie = performingWheelie;
    
    // Reset balance input when wheelie ends
    if (!performingWheelie) {
        s_wheelieBalanceInput = 0.0f;
    }
    
#if MOTION_PATCHES_DEBUG_LOGGING
    if (performingWheelie) {
        LOGFN("[MotionPatch] Wheelie started - motion balance active");
    }
#endif
}

bool IsPerformingWheelie()
{
    return s_performingWheelie;
}

float GetMotionWheelieBalance()
{
    // Only provide balance input when on a motorcycle doing a wheelie
    if (!s_performingWheelie || !VehicleState::IsOnMotorcycle()) {
        return 0.0f;
    }
    
    if (!hid::HasMotionSensor() || !hid::IsMotionSensorEnabled() || !Config::MotionControlsEnabled) {
        return 0.0f;
    }
    
    const auto& motion = hid::GetMotionState();
    
    // Use pitch for forward/back balance
    // Tilting controller forward = lean forward (prevent flip back)
    // Tilting controller back = lean back (prevent fall forward)
    float sensitivity = Config::MotionSensitivity * 1.5f;  // Wheelies need responsive input
    
    // Map pitch to balance input (-1 = full back, +1 = full forward)
    float balanceInput = -motion.pitch * sensitivity;  // Invert: pitch forward = positive input
    
    // Apply smoothing to prevent jerky balance corrections
    constexpr float BALANCE_SMOOTH_ALPHA = 0.4f;
    s_wheelieBalanceInput = BALANCE_SMOOTH_ALPHA * balanceInput + 
                            (1.0f - BALANCE_SMOOTH_ALPHA) * s_wheelieBalanceInput;
    
    // Clamp to valid range
    s_wheelieBalanceInput = std::clamp(s_wheelieBalanceInput, -1.0f, 1.0f);
    
#if MOTION_PATCHES_DEBUG_LOGGING
    static int s_callCount = 0;
    if (++s_callCount % 30 == 0) {  // Log every half second at 60fps
        LOGFN("[MotionPatch] Wheelie balance: pitch={:.3f}, input={:.3f}", 
              motion.pitch, s_wheelieBalanceInput);
    }
#endif
    
    return s_wheelieBalanceInput;
}

// ============================================================================
// Initialization
// ============================================================================

void Init()
{
    LOG_INFO("GTA IV Motion Control Patches initialized (Phase 2 wheelie balance enabled)");
    
    // Reset state
    s_motionReloadTriggered = false;
    s_motionPreferencesEnabled = true;
    s_shakeCount = 0;
    s_lastShakeTime = 0;
    s_lastAccelMagnitude = 1.0f;
    s_smoothedAccelX = 0.0f;
    s_smoothedAccelY = 0.0f;
    s_smoothedAccelZ = -1.0f;
    
    // Phase 2: Reset wheelie state
    s_performingWheelie = false;
    s_wheelieBalanceInput = 0.0f;
    
    // Check if we have a motion-capable controller
    if (hid::HasMotionSensor()) {
        LOG_INFO("Motion sensor detected - enabling motion controls");
        hid::SetMotionSensorEnabled(Config::MotionControlsEnabled);
    } else {
        LOG_INFO("No motion sensor detected - motion controls disabled");
    }
}

// Called each frame to update motion state
void Update()
{
    if (hid::HasMotionSensor() && hid::IsMotionSensorEnabled()) {
        hid::UpdateMotionState();
        UpdateMotionReloadDetection();
    }
}

} // namespace MotionPatches

// ============================================================================
// PPC Function Hooks for Vehicle Motion Integration
// ============================================================================

// Hook for GET_POSITION_OF_ANALOGUE_STICKS (sub_82593668)
// Blends motion steering input with analog stick values when in a vehicle
extern "C" void __imp__sub_82593668(PPCContext& ctx, uint8_t* base);

PPC_FUNC_HOOK(sub_82593668)
{
    PPC_FUNC_PROLOGUE();
    
    // Call original function to get actual stick values
    __imp__sub_82593668(ctx, base);
    
    // If motion steering is enabled and we're in a vehicle, blend motion input
    if (Config::MotionSteering && VehicleState::IsInVehicle() && 
        hid::HasMotionSensor() && hid::IsMotionSensorEnabled()) {
        
        // Get parameter structure to access output values
        uint32_t paramStructAddr = PPC_LOAD_U32(ctx.r3.u32 + 8);
        if (paramStructAddr) {
            // Output structure:
            // +0x04 (r6) → Left stick X output pointer
            // +0x08 (r5) → Left stick Y output pointer
            uint32_t leftStickXPtr = PPC_LOAD_U32(paramStructAddr + 4);
            
            if (leftStickXPtr) {
                // Get current stick value
                int32_t currentX = static_cast<int32_t>(PPC_LOAD_U32(leftStickXPtr));
                
                // Get motion steering input (-1 to +1)
                float motionInput = MotionPatches::GetMotionSteeringInput();
                
                // Convert motion to stick range (-128 to 127 or similar)
                // The exact range depends on game's expected values
                int32_t motionSteerValue = static_cast<int32_t>(motionInput * 127.0f);
                
                // Blend: add motion input to current stick value, clamped
                int32_t blendedX = std::clamp(currentX + motionSteerValue, -128, 127);
                
                // Store blended value back
                PPC_STORE_U32(leftStickXPtr, static_cast<uint32_t>(blendedX));
                
#if MOTION_PATCHES_DEBUG_LOGGING
                static int s_callCount = 0;
                if (++s_callCount % 60 == 0) {
                    LOGFN("[MotionSteer] original={}, motion={:.2f}, blended={}", 
                          currentX, motionInput, blendedX);
                }
#endif
            }
        }
    }
    
    // If helicopter motion is enabled, blend with helicopter controls
    if (Config::MotionHelicopter && VehicleState::IsInHelicopter() &&
        hid::HasMotionSensor() && hid::IsMotionSensorEnabled()) {
        
        uint32_t paramStructAddr = PPC_LOAD_U32(ctx.r3.u32 + 8);
        if (paramStructAddr) {
            uint32_t leftStickXPtr = PPC_LOAD_U32(paramStructAddr + 4);
            uint32_t leftStickYPtr = PPC_LOAD_U32(paramStructAddr + 8);
            
            float heliPitch, heliRoll;
            MotionPatches::GetMotionHelicopterInput(&heliPitch, &heliRoll);
            
            if (leftStickXPtr) {
                int32_t currentX = static_cast<int32_t>(PPC_LOAD_U32(leftStickXPtr));
                int32_t motionRollValue = static_cast<int32_t>(heliRoll * 127.0f);
                int32_t blendedX = std::clamp(currentX + motionRollValue, -128, 127);
                PPC_STORE_U32(leftStickXPtr, static_cast<uint32_t>(blendedX));
            }
            
            if (leftStickYPtr) {
                int32_t currentY = static_cast<int32_t>(PPC_LOAD_U32(leftStickYPtr));
                int32_t motionPitchValue = static_cast<int32_t>(heliPitch * 127.0f);
                int32_t blendedY = std::clamp(currentY + motionPitchValue, -128, 127);
                PPC_STORE_U32(leftStickYPtr, static_cast<uint32_t>(blendedY));
            }
        }
    }
    
    // Phase 2: If on motorcycle doing wheelie, blend balance input with Y axis
    if (VehicleState::IsOnMotorcycle() && MotionPatches::IsPerformingWheelie() &&
        hid::HasMotionSensor() && hid::IsMotionSensorEnabled() && Config::MotionControlsEnabled) {
        
        uint32_t paramStructAddr = PPC_LOAD_U32(ctx.r3.u32 + 8);
        if (paramStructAddr) {
            uint32_t leftStickYPtr = PPC_LOAD_U32(paramStructAddr + 8);
            
            if (leftStickYPtr) {
                int32_t currentY = static_cast<int32_t>(PPC_LOAD_U32(leftStickYPtr));
                
                // Get wheelie balance input from motion sensor
                float balanceInput = MotionPatches::GetMotionWheelieBalance();
                
                // Convert to stick range and blend
                int32_t motionBalanceValue = static_cast<int32_t>(balanceInput * 80.0f);  // Gentler blend
                int32_t blendedY = std::clamp(currentY + motionBalanceValue, -128, 127);
                
                PPC_STORE_U32(leftStickYPtr, static_cast<uint32_t>(blendedY));
                
#if MOTION_PATCHES_DEBUG_LOGGING
                static int s_wheelieCount = 0;
                if (++s_wheelieCount % 30 == 0) {
                    LOGFN("[WheelieBalance] original={}, balance={:.2f}, blended={}", 
                          currentY, balanceInput, blendedY);
                }
#endif
            }
        }
    }
}

// Hook for IS_CHAR_IN_ANY_CAR (sub_82590628) - to detect vehicle state
// This is called frequently and lets us track if player is in a vehicle
extern "C" void __imp__sub_82590628(PPCContext& ctx, uint8_t* base);

PPC_FUNC_HOOK(sub_82590628)
{
    PPC_FUNC_PROLOGUE();
    
    // Call original function
    __imp__sub_82590628(ctx, base);
    
    // The result is stored at the return value pointer
    // Update our vehicle state tracking
    uint32_t returnPtr = PPC_LOAD_U32(ctx.r3.u32 + 0);
    if (returnPtr) {
        int32_t isInCar = static_cast<int32_t>(PPC_LOAD_U32(returnPtr));
        VehicleState::SetInVehicle(isInCar != 0);
    }
}

// Hook for IS_CHAR_IN_ANY_HELI (sub_82590918) - to detect helicopter state
extern "C" void __imp__sub_82590918(PPCContext& ctx, uint8_t* base);

PPC_FUNC_HOOK(sub_82590918)
{
    PPC_FUNC_PROLOGUE();
    
    // Call original function
    __imp__sub_82590918(ctx, base);
    
    // Update helicopter state
    uint32_t returnPtr = PPC_LOAD_U32(ctx.r3.u32 + 0);
    if (returnPtr) {
        int32_t isInHeli = static_cast<int32_t>(PPC_LOAD_U32(returnPtr));
        VehicleState::SetInHelicopter(isInHeli != 0);
    }
}

// ============================================================================
// Phase 2: IS_CHAR_ON_ANY_BIKE Hook (sub_82587310) - Motorcycle Detection
// ============================================================================
// Called to check if player is on a motorcycle. We use this for wheelie balance.

extern "C" void __imp__sub_82587310(PPCContext& ctx, uint8_t* base);

PPC_FUNC_HOOK(sub_82587310)
{
    PPC_FUNC_PROLOGUE();
    
    // Call original function
    __imp__sub_82587310(ctx, base);
    
    // Update motorcycle state
    uint32_t returnPtr = PPC_LOAD_U32(ctx.r3.u32 + 0);
    if (returnPtr) {
        int32_t isOnBike = static_cast<int32_t>(PPC_LOAD_U32(returnPtr));
        VehicleState::SetOnMotorcycle(isOnBike != 0);
        
        // If not on bike, clear wheelie state
        if (!isOnBike) {
            MotionPatches::SetWheelieState(false);
        }
    }
}

// ============================================================================
// Phase 2: IS_PLAYER_PERFORMING_WHEELIE Hook (sub_82579960) - Wheelie Detection
// ============================================================================
// Called to check if player is doing a wheelie. Enables motion balance assist.

extern "C" void __imp__sub_82579960(PPCContext& ctx, uint8_t* base);

PPC_FUNC_HOOK(sub_82579960)
{
    PPC_FUNC_PROLOGUE();
    
    // Call original function
    __imp__sub_82579960(ctx, base);
    
    // Check if performing wheelie
    uint32_t returnPtr = PPC_LOAD_U32(ctx.r3.u32 + 0);
    if (returnPtr) {
        int32_t isWheelie = static_cast<int32_t>(PPC_LOAD_U32(returnPtr));
        MotionPatches::SetWheelieState(isWheelie != 0);
    }
}

// ============================================================================
// Phase 2: GET_POSITION_OF_ANALOGUE_STICKS Extension - Wheelie Balance Blend
// ============================================================================
// The main GET_POSITION_OF_ANALOGUE_STICKS hook is extended to blend wheelie
// balance input when on a motorcycle doing a wheelie. This modifies the
// left stick Y axis to help maintain balance.
