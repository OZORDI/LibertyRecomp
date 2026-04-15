// =============================================================================
// GTA IV DualShock 4 / DualSense Patches - Implementation
// =============================================================================
// Implements DualShock 4 and DualSense specific features for GTA IV

#include <stdafx.h>
#include <cpu/ppc_context.h>
#include <kernel/function.h>
#include <hid/hid.h>
#include <os/logger.h>
#include <user/config.h>
#include <patches/gta4_ds4_patches.h>     // Our header with TouchpadGesture enum
#include <patches/gta4_motion_patches.h>  // For VehicleState
#include <cmath>
#include <cstring>
#include <algorithm>

// PS4 uses libScePad directly for pad input — SDL is not available.
// The DS4 features here (lightbar color, touchpad gestures, enhanced haptics)
// are cosmetic enhancements layered on top of SDL's gamepad API.
// On PS4, the system firmware handles lightbar and basic haptics natively,
// so these patches are disabled. On Switch (NX), SDL is available but the
// controller lacks a lightbar/touchpad, so these features are also stubbed.
#if !defined(LIBERTY_RECOMP_PS4) && !defined(LIBERTY_RECOMP_NX)
#include <SDL3/SDL.h>
#endif

namespace DS4Patches {

#if defined(LIBERTY_RECOMP_PS4) || defined(LIBERTY_RECOMP_NX)
// ============================================================================
// Stub implementations for PS4 / Switch
// ============================================================================
// On PS4, the system manages the lightbar and basic rumble via libScePad.
// On Switch, there is no lightbar or touchpad. Stub all public entry points
// so the rest of the codebase compiles without #ifdef at every call site.

void UpdateLightBar() {}
void SetLightBarOverride(uint8_t, uint8_t, uint8_t) {}
void ClearLightBarOverride() {}
void GetLightBarColor(uint8_t* r, uint8_t* g, uint8_t* b) { if (r) *r = 0; if (g) *g = 0; if (b) *b = 255; }
void OnWantedLevelChanged(int) {}
void OnHealthChanged(int, int) {}
void OnPlayerDeath() {}
void OnPlayerRespawn() {}
TouchpadGesture GetLastGesture() { return TouchpadGesture::None; }
bool WasGestureDetected(TouchpadGesture) { return false; }
void ChangeRadioStation(int) {}
void CycleWeapon(int) {}
void TriggerExplosionHaptic(float) {}
void OnExplosion(float, float, float, float, int) {}
void TriggerImpactHaptic(float) {}
void OnVehicleHealthChanged(int, int) {}
void TriggerGunfireHaptic(float) {}
void OnNearbyGunfire(float) {}
void SetWeatherHaptic(bool, float) {}
void OnWeatherChanged(int) {}
void TriggerGearChangeHaptic() {}
void OnVehicleSpeedUpdate(float) {}
void ToggleCinematicCamera() {}
bool IsCameraTogglePending() { return false; }
void ClearCameraTogglePending() {}
void SetPlayerPosition(float, float, float) {}
void Init() { LOG_INFO("GTA IV DS4 patches: stubbed on this platform (no SDL gamepad)"); }
void Update() {}

} // namespace DS4Patches

#else // Desktop platforms with SDL
// ============================================================================
// Full SDL-based DS4 implementation
// ============================================================================

// ============================================================================
// State Tracking
// ============================================================================

// Light bar state
static uint8_t s_lightBarR = 0;
static uint8_t s_lightBarG = 0;
static uint8_t s_lightBarB = 255;  // Default: blue (like PS4 player 1)
static bool s_lightBarOverride = false;
static uint8_t s_overrideR = 0, s_overrideG = 0, s_overrideB = 0;

// Game state for light bar
static int s_wantedLevel = 0;
static int s_currentHealth = 100;
static int s_maxHealth = 100;
static bool s_playerDead = false;

// Wanted level flash state
static uint64_t s_lastFlashTime = 0;
static bool s_flashState = false;
static constexpr uint64_t POLICE_FLASH_INTERVAL_MS = 150;  // Red/blue flash speed

// Low health pulse state
static float s_healthPulsePhase = 0.0f;
static constexpr float HEALTH_PULSE_SPEED = 4.0f;  // Radians per second

// Touchpad gesture detection
static TouchpadGesture s_lastGesture = TouchpadGesture::None;
static TouchpadGesture s_pendingGesture = TouchpadGesture::None;
static float s_touchStartX = 0.0f;
static float s_touchStartY = 0.0f;
static uint64_t s_touchStartTime = 0;
static uint64_t s_lastTapTime = 0;
static bool s_touchWasDown = false;

// Gesture detection thresholds
static constexpr float SWIPE_THRESHOLD = 0.3f;      // Minimum swipe distance (0-1)
static constexpr float SWIPE_MIN_SPEED = 1.5f;      // Minimum swipe velocity
static constexpr uint64_t TAP_MAX_DURATION_MS = 200;
static constexpr uint64_t DOUBLE_TAP_INTERVAL_MS = 400;
static constexpr float TAP_MAX_MOVEMENT = 0.1f;

// ============================================================================
// Phase 1-3: Enhanced Haptic State Tracking
// ============================================================================

// Explosion haptics
static uint64_t s_lastExplosionHapticTime = 0;
static constexpr uint64_t EXPLOSION_HAPTIC_COOLDOWN_MS = 100;  // Prevent rapid-fire haptics

// Collision/impact haptics
static int s_lastVehicleHealth = 1000;
static uint64_t s_lastImpactHapticTime = 0;
static constexpr uint64_t IMPACT_HAPTIC_COOLDOWN_MS = 50;
static constexpr int IMPACT_MIN_DAMAGE = 5;  // Minimum damage to trigger haptic

// Weather haptics
static bool s_weatherHapticEnabled = false;
static float s_weatherHapticIntensity = 0.15f;
static float s_weatherHapticPhase = 0.0f;
static int s_currentWeatherType = 0;

// Weather type constants (from GTA IV)
static constexpr int WEATHER_RAINING = 4;
static constexpr int WEATHER_DRIZZLE = 5;
static constexpr int WEATHER_LIGHTNING = 7;

// Gear change detection
static float s_lastVehicleSpeed = 0.0f;
static float s_speedAccumulator = 0.0f;
static int s_estimatedGear = 1;
static uint64_t s_lastGearChangeTime = 0;
static constexpr uint64_t GEAR_CHANGE_COOLDOWN_MS = 300;

// Camera toggle state
static bool s_cameraTogglePending = false;

// Player position (for distance calculations)
static float s_playerX = 0.0f;
static float s_playerY = 0.0f;
static float s_playerZ = 0.0f;

// ============================================================================
// Light Bar Control
// ============================================================================

static void CalculateLightBarColor()
{
    if (s_lightBarOverride) {
        s_lightBarR = s_overrideR;
        s_lightBarG = s_overrideG;
        s_lightBarB = s_overrideB;
        return;
    }
    
    uint64_t now = SDL_GetTicks();
    
    // Priority 1: Player is dead - solid dark red
    if (s_playerDead) {
        s_lightBarR = 50;
        s_lightBarG = 0;
        s_lightBarB = 0;
        return;
    }
    
    // Priority 2: Wanted level - red/blue police flash
    if (s_wantedLevel > 0) {
        // Flash between red and blue
        if (now - s_lastFlashTime >= POLICE_FLASH_INTERVAL_MS) {
            s_flashState = !s_flashState;
            s_lastFlashTime = now;
        }
        
        // Intensity scales with wanted level (1-6 stars)
        uint8_t intensity = static_cast<uint8_t>(100 + (s_wantedLevel * 25));
        intensity = std::min<uint8_t>(intensity, 255);
        
        if (s_flashState) {
            s_lightBarR = intensity;
            s_lightBarG = 0;
            s_lightBarB = 0;
        } else {
            s_lightBarR = 0;
            s_lightBarG = 0;
            s_lightBarB = intensity;
        }
        return;
    }
    
    // Priority 3: Low health - pulsing red
    float healthPercent = (s_maxHealth > 0) ? 
        static_cast<float>(s_currentHealth) / s_maxHealth : 1.0f;
    
    if (healthPercent < 0.25f) {
        // Pulse between dim and bright red
        s_healthPulsePhase += HEALTH_PULSE_SPEED * 0.016f;  // Assuming ~60fps
        if (s_healthPulsePhase > 6.283f) s_healthPulsePhase -= 6.283f;
        
        float pulse = (sinf(s_healthPulsePhase) + 1.0f) * 0.5f;  // 0 to 1
        uint8_t intensity = static_cast<uint8_t>(100 + pulse * 155);
        
        s_lightBarR = intensity;
        s_lightBarG = 0;
        s_lightBarB = 0;
        return;
    }
    
    // Default: Color based on health (green = full, yellow = mid, red = low)
    if (healthPercent > 0.66f) {
        // Green - healthy
        s_lightBarR = 0;
        s_lightBarG = 255;
        s_lightBarB = 0;
    } else if (healthPercent > 0.33f) {
        // Yellow/orange - caution
        s_lightBarR = 255;
        s_lightBarG = static_cast<uint8_t>(healthPercent * 3.0f * 255);
        s_lightBarB = 0;
    } else {
        // Red - danger (but not pulsing yet)
        s_lightBarR = 255;
        s_lightBarG = 0;
        s_lightBarB = 0;
    }
}

void UpdateLightBar()
{
    if (!hid::HasLightBar()) {
        return;
    }
    
    CalculateLightBarColor();
    hid::SetLightBarColor(s_lightBarR, s_lightBarG, s_lightBarB);
}

void SetLightBarOverride(uint8_t r, uint8_t g, uint8_t b)
{
    s_lightBarOverride = true;
    s_overrideR = r;
    s_overrideG = g;
    s_overrideB = b;
}

void ClearLightBarOverride()
{
    s_lightBarOverride = false;
}

void GetLightBarColor(uint8_t* r, uint8_t* g, uint8_t* b)
{
    if (r) *r = s_lightBarR;
    if (g) *g = s_lightBarG;
    if (b) *b = s_lightBarB;
}

// ============================================================================
// Game State Hooks
// ============================================================================

void OnWantedLevelChanged(int newLevel)
{
    s_wantedLevel = std::clamp(newLevel, 0, 6);
    
    // Reset flash state when wanted level changes
    s_flashState = false;
    s_lastFlashTime = SDL_GetTicks();
    
#if 0  // Debug logging
    LOGFN("[DS4] Wanted level changed to: {}", s_wantedLevel);
#endif
}

void OnHealthChanged(int currentHealth, int maxHealth)
{
    s_currentHealth = currentHealth;
    s_maxHealth = maxHealth;
    s_playerDead = (currentHealth <= 0);
}

void OnPlayerDeath()
{
    s_playerDead = true;
    s_currentHealth = 0;
}

void OnPlayerRespawn()
{
    s_playerDead = false;
    s_wantedLevel = 0;
    s_currentHealth = s_maxHealth;
}

// ============================================================================
// Touchpad Gesture Detection
// ============================================================================

static void DetectGestures()
{
    if (!hid::HasTouchpad()) {
        return;
    }
    
    const auto& touch = hid::GetTouchpadState();
    uint64_t now = SDL_GetTicks();
    
    // Track touch start
    if (touch.finger0Down && !s_touchWasDown) {
        // Touch just started
        s_touchStartX = touch.finger0X;
        s_touchStartY = touch.finger0Y;
        s_touchStartTime = now;
    }
    
    // Detect gestures on touch release
    if (!touch.finger0Down && s_touchWasDown) {
        // Touch just ended
        float deltaX = touch.finger0X - s_touchStartX;
        float deltaY = touch.finger0Y - s_touchStartY;
        float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
        uint64_t duration = now - s_touchStartTime;
        
        // Check for swipe
        if (distance >= SWIPE_THRESHOLD) {
            // Determine swipe direction
            if (fabsf(deltaX) > fabsf(deltaY)) {
                // Horizontal swipe
                s_pendingGesture = (deltaX > 0) ? TouchpadGesture::SwipeRight : TouchpadGesture::SwipeLeft;
            } else {
                // Vertical swipe
                s_pendingGesture = (deltaY > 0) ? TouchpadGesture::SwipeDown : TouchpadGesture::SwipeUp;
            }
        }
        // Check for tap
        else if (duration <= TAP_MAX_DURATION_MS && distance < TAP_MAX_MOVEMENT) {
            // Check for double tap
            if (now - s_lastTapTime <= DOUBLE_TAP_INTERVAL_MS) {
                s_pendingGesture = TouchpadGesture::DoubleTap;
                s_lastTapTime = 0;  // Reset to prevent triple-tap
            } else {
                s_pendingGesture = TouchpadGesture::Tap;
                s_lastTapTime = now;
            }
        }
    }
    
    s_touchWasDown = touch.finger0Down;
    
    // Transfer pending gesture to last gesture (for reading)
    if (s_pendingGesture != TouchpadGesture::None) {
        s_lastGesture = s_pendingGesture;
        s_pendingGesture = TouchpadGesture::None;
    }
}

TouchpadGesture GetLastGesture()
{
    TouchpadGesture result = s_lastGesture;
    s_lastGesture = TouchpadGesture::None;
    return result;
}

bool WasGestureDetected(TouchpadGesture gesture)
{
    if (s_lastGesture == gesture) {
        s_lastGesture = TouchpadGesture::None;
        return true;
    }
    return false;
}

// ============================================================================
// Touchpad Actions
// ============================================================================

// These will call into the game's script system when hooks are complete
void ChangeRadioStation(int direction)
{
    // TODO: Hook into RETUNE_RADIO_TO_STATION_NAME
    // For now, log the intent
#if 0
    LOGFN("[DS4] Radio station change requested: {}", direction > 0 ? "next" : "previous");
#endif
}

void CycleWeapon(int direction)
{
    // TODO: Hook into weapon wheel system
#if 0
    LOGFN("[DS4] Weapon cycle requested: {}", direction > 0 ? "next" : "previous");
#endif
}

// ============================================================================
// Phase 1: Explosion Haptics
// ============================================================================

void TriggerExplosionHaptic(float intensity)
{
    uint64_t now = SDL_GetTicks();
    
    // Cooldown to prevent haptic spam
    if (now - s_lastExplosionHapticTime < EXPLOSION_HAPTIC_COOLDOWN_MS) {
        return;
    }
    s_lastExplosionHapticTime = now;
    
    // Clamp intensity
    intensity = std::clamp(intensity, 0.0f, 1.0f);
    
    // Explosion = heavy low-frequency rumble with some high-freq
    // Scale: 0-255 for each motor (XAMINPUT uses 0-65535, we scale down)
    uint16_t lowFreq = static_cast<uint16_t>(intensity * 65535.0f);
    uint16_t highFreq = static_cast<uint16_t>(intensity * 32768.0f);
    
    // Apply rumble via XInput-style API
    XAMINPUT_VIBRATION vib;
    vib.wLeftMotorSpeed = lowFreq;
    vib.wRightMotorSpeed = highFreq;
    hid::SetState(0, &vib);
    
#if 0  // Debug logging
    LOGFN("[DS4] Explosion haptic: intensity={:.2f}, low={}, high={}", 
          intensity, lowFreq, highFreq);
#endif
}

void OnExplosion(float x, float y, float z, float radius, int type)
{
    // Calculate distance from player
    float dx = x - s_playerX;
    float dy = y - s_playerY;
    float dz = z - s_playerZ;
    float distance = sqrtf(dx * dx + dy * dy + dz * dz);
    
    // Effective range is 2x the explosion radius
    float effectiveRadius = radius * 2.0f;
    
    // Only trigger if within effective range
    if (distance < effectiveRadius) {
        // Intensity inversely proportional to distance
        float intensity = 1.0f - (distance / effectiveRadius);
        
        // Boost intensity for larger explosions
        float radiusBoost = std::min(radius / 10.0f, 1.5f);
        intensity *= radiusBoost;
        
        TriggerExplosionHaptic(intensity);
    }
}

// ============================================================================
// Phase 1: Collision/Impact Haptics
// ============================================================================

void TriggerImpactHaptic(float intensity)
{
    uint64_t now = SDL_GetTicks();
    
    // Cooldown to prevent haptic spam
    if (now - s_lastImpactHapticTime < IMPACT_HAPTIC_COOLDOWN_MS) {
        return;
    }
    s_lastImpactHapticTime = now;
    
    // Clamp intensity
    intensity = std::clamp(intensity, 0.0f, 1.0f);
    
    // Impact = sharp, quick rumble with emphasis on high frequency
    uint16_t lowFreq = static_cast<uint16_t>(intensity * 45000.0f);
    uint16_t highFreq = static_cast<uint16_t>(intensity * 60000.0f);
    
    XAMINPUT_VIBRATION vib;
    vib.wLeftMotorSpeed = lowFreq;
    vib.wRightMotorSpeed = highFreq;
    hid::SetState(0, &vib);
    
#if 0  // Debug logging
    LOGFN("[DS4] Impact haptic: intensity={:.2f}", intensity);
#endif
}

void OnVehicleHealthChanged(int currentHealth, int previousHealth)
{
    // Calculate damage taken
    int damage = previousHealth - currentHealth;
    
    // Only trigger for significant damage
    if (damage >= IMPACT_MIN_DAMAGE) {
        // Scale intensity based on damage (max ~500 damage = full intensity)
        float intensity = std::min(static_cast<float>(damage) / 500.0f, 1.0f);
        TriggerImpactHaptic(intensity);
    }
    
    // Update tracked health
    s_lastVehicleHealth = currentHealth;
}

// ============================================================================
// Phase 2: Nearby Gunfire Haptics
// ============================================================================

void TriggerGunfireHaptic(float intensity)
{
    // Gunfire haptic is subtle - don't overpower other haptics
    intensity = std::clamp(intensity, 0.0f, 0.4f);  // Cap at 40% max
    
    // Quick, sharp pulse - primarily high frequency
    uint16_t lowFreq = static_cast<uint16_t>(intensity * 15000.0f);
    uint16_t highFreq = static_cast<uint16_t>(intensity * 35000.0f);
    
    XAMINPUT_VIBRATION vib;
    vib.wLeftMotorSpeed = lowFreq;
    vib.wRightMotorSpeed = highFreq;
    hid::SetState(0, &vib);
}

void OnNearbyGunfire(float distance)
{
    // Gunfire only felt within 50 meters
    constexpr float MAX_GUNFIRE_DISTANCE = 50.0f;
    
    if (distance < MAX_GUNFIRE_DISTANCE) {
        float intensity = 1.0f - (distance / MAX_GUNFIRE_DISTANCE);
        intensity *= 0.3f;  // Keep it subtle
        TriggerGunfireHaptic(intensity);
    }
}

// ============================================================================
// Phase 3: Weather/Rain Haptics
// ============================================================================

void SetWeatherHaptic(bool enabled, float intensity)
{
    s_weatherHapticEnabled = enabled;
    s_weatherHapticIntensity = std::clamp(intensity, 0.0f, 0.3f);  // Cap at 30%
}

void OnWeatherChanged(int weatherType)
{
    s_currentWeatherType = weatherType;
    
    // Enable haptic for rainy weather types
    bool isRaining = (weatherType == WEATHER_RAINING || 
                      weatherType == WEATHER_DRIZZLE ||
                      weatherType == WEATHER_LIGHTNING);
    
    // Set intensity based on weather type
    float intensity = 0.0f;
    if (weatherType == WEATHER_DRIZZLE) {
        intensity = 0.08f;  // Light rain = subtle
    } else if (weatherType == WEATHER_RAINING) {
        intensity = 0.15f;  // Normal rain
    } else if (weatherType == WEATHER_LIGHTNING) {
        intensity = 0.20f;  // Storm = stronger
    }
    
    SetWeatherHaptic(isRaining, intensity);
}

static void UpdateWeatherHaptic()
{
    if (!s_weatherHapticEnabled) {
        return;
    }
    
    // Only apply weather haptic when in a vehicle (rain on windshield)
    if (!VehicleState::IsInVehicle()) {
        return;
    }
    
    // Create a varying, organic rain texture
    s_weatherHapticPhase += 0.1f;
    if (s_weatherHapticPhase > 6.283f) {
        s_weatherHapticPhase -= 6.283f;
    }
    
    // Use multiple frequencies to create rain patter effect
    float variation = 0.5f + 0.5f * sinf(s_weatherHapticPhase * 3.0f);
    float intensity = s_weatherHapticIntensity * variation;
    
    // Very low rumble for rain texture
    uint16_t lowFreq = static_cast<uint16_t>(intensity * 8000.0f);
    uint16_t highFreq = static_cast<uint16_t>(intensity * 3000.0f);
    
    XAMINPUT_VIBRATION vib;
    vib.wLeftMotorSpeed = lowFreq;
    vib.wRightMotorSpeed = highFreq;
    hid::SetState(0, &vib);
}

// ============================================================================
// Phase 3: Gear Change Haptics
// ============================================================================

void TriggerGearChangeHaptic()
{
    uint64_t now = SDL_GetTicks();
    
    // Cooldown to prevent spam
    if (now - s_lastGearChangeTime < GEAR_CHANGE_COOLDOWN_MS) {
        return;
    }
    s_lastGearChangeTime = now;
    
    // Quick, subtle bump - like a gear engaging
    uint16_t lowFreq = 15000;
    uint16_t highFreq = 8000;
    
    XAMINPUT_VIBRATION vib;
    vib.wLeftMotorSpeed = lowFreq;
    vib.wRightMotorSpeed = highFreq;
    hid::SetState(0, &vib);
    
#if 0  // Debug logging
    LOGFN("[DS4] Gear change haptic triggered");
#endif
}

void OnVehicleSpeedUpdate(float speed)
{
    if (!VehicleState::IsInVehicle()) {
        s_lastVehicleSpeed = 0.0f;
        s_estimatedGear = 1;
        return;
    }
    
    // Calculate speed delta
    float speedDelta = speed - s_lastVehicleSpeed;
    
    // Detect gear changes using speed pattern analysis
    // When accelerating, gear shifts cause brief speed dips or slowdowns
    // We detect a pattern of: accelerating -> brief slowdown -> accelerating
    
    // Accumulate acceleration
    if (speedDelta > 0.5f) {
        s_speedAccumulator += speedDelta;
    }
    
    // Detect gear shift: was accelerating, then speed growth slowed/reversed
    // while still presumably pressing gas
    if (s_speedAccumulator > 5.0f && speedDelta < -0.5f && speedDelta > -3.0f) {
        // Estimate new gear based on speed thresholds (rough GTA IV values)
        int newGear = 1;
        if (speed > 100.0f) newGear = 5;
        else if (speed > 70.0f) newGear = 4;
        else if (speed > 45.0f) newGear = 3;
        else if (speed > 20.0f) newGear = 2;
        
        if (newGear != s_estimatedGear) {
            TriggerGearChangeHaptic();
            s_estimatedGear = newGear;
        }
        
        s_speedAccumulator = 0.0f;
    }
    
    // Reset accumulator if we're slowing down significantly
    if (speedDelta < -5.0f) {
        s_speedAccumulator = 0.0f;
        // Downshift detection on hard braking
        int newGear = 1;
        if (speed > 70.0f) newGear = 4;
        else if (speed > 45.0f) newGear = 3;
        else if (speed > 20.0f) newGear = 2;
        
        if (newGear < s_estimatedGear) {
            TriggerGearChangeHaptic();
            s_estimatedGear = newGear;
        }
    }
    
    s_lastVehicleSpeed = speed;
}

// ============================================================================
// Phase 3: Camera Toggle (Touchpad Tap)
// ============================================================================

void ToggleCinematicCamera()
{
    s_cameraTogglePending = true;
    
#if 0  // Debug logging
    LOGFN("[DS4] Camera toggle requested");
#endif
}

bool IsCameraTogglePending()
{
    return s_cameraTogglePending;
}

void ClearCameraTogglePending()
{
    s_cameraTogglePending = false;
}

// ============================================================================
// Player Position Tracking (for distance calculations)
// ============================================================================

void SetPlayerPosition(float x, float y, float z)
{
    s_playerX = x;
    s_playerY = y;
    s_playerZ = z;
}

// ============================================================================
// Lifecycle
// ============================================================================

void Init()
{
    LOG_INFO("GTA IV DualShock 4/DualSense patches initialized (Phase 1-3 haptics enabled)");
    
    // Reset state
    s_wantedLevel = 0;
    s_currentHealth = 100;
    s_maxHealth = 100;
    s_playerDead = false;
    s_lightBarOverride = false;
    s_lastGesture = TouchpadGesture::None;
    s_pendingGesture = TouchpadGesture::None;
    s_touchWasDown = false;
    
    // Reset haptic state
    s_lastExplosionHapticTime = 0;
    s_lastImpactHapticTime = 0;
    s_lastVehicleHealth = 1000;
    s_weatherHapticEnabled = false;
    s_weatherHapticPhase = 0.0f;
    s_currentWeatherType = 0;
    s_lastVehicleSpeed = 0.0f;
    s_speedAccumulator = 0.0f;
    s_estimatedGear = 1;
    s_lastGearChangeTime = 0;
    s_cameraTogglePending = false;
    s_playerX = s_playerY = s_playerZ = 0.0f;
    
    // Set initial light bar color (blue = player 1)
    if (hid::HasLightBar()) {
        hid::SetLightBarColor(0, 0, 255);
    }
}

void Update()
{
    // Update touchpad state and detect gestures
    hid::UpdateTouchpadState();
    DetectGestures();
    
    // Process touchpad gestures for radio/weapon switching AND camera
    TouchpadGesture gesture = GetLastGesture();
    switch (gesture) {
        case TouchpadGesture::SwipeLeft:
            ChangeRadioStation(-1);
            break;
        case TouchpadGesture::SwipeRight:
            ChangeRadioStation(+1);
            break;
        case TouchpadGesture::SwipeUp:
            CycleWeapon(-1);
            break;
        case TouchpadGesture::SwipeDown:
            CycleWeapon(+1);
            break;
        case TouchpadGesture::Tap:
            // Phase 3: Touchpad tap toggles camera
            ToggleCinematicCamera();
            break;
        default:
            break;
    }
    
    // Update light bar based on game state
    UpdateLightBar();
    
    // Phase 3: Update weather haptic (continuous rain effect)
    UpdateWeatherHaptic();
}

} // namespace DS4Patches

#endif // !LIBERTY_RECOMP_PS4 && !LIBERTY_RECOMP_NX

// ============================================================================
// PPC Function Hooks for Game State Tracking
// ============================================================================

#if !defined(LIBERTY_RECOMP_PS4) && !defined(LIBERTY_RECOMP_NX)

// Hook for ALTER_WANTED_LEVEL (sub_82578B80)
// Called when the game changes the player's wanted level
extern "C" void __imp__sub_82578B80(PPCContext& ctx, uint8_t* base);

PPC_FUNC_HOOK(sub_82578B80)
{
    PPC_FUNC_PROLOGUE();
    
    // Call original function first
    __imp__sub_82578B80(ctx, base);
    
    // Extract new wanted level from script parameters
    // The parameter structure is at r3+8
    uint32_t paramStructAddr = PPC_LOAD_U32(ctx.r3.u32 + 8);
    if (paramStructAddr) {
        // Wanted level is typically the second parameter
        uint32_t wantedLevelPtr = PPC_LOAD_U32(paramStructAddr + 4);
        if (wantedLevelPtr) {
            int32_t newLevel = static_cast<int32_t>(PPC_LOAD_U32(wantedLevelPtr));
            DS4Patches::OnWantedLevelChanged(newLevel);
        }
    }
}

// NOTE: GET_CHAR_HEALTH (sub_8258F688) does not exist in the recompiled PPC code.
// Player health tracking will need to use a different approach if needed.

// ============================================================================
// Phase 1: ADD_EXPLOSION Hook (sub_82546D20) - Explosion Haptics
// ============================================================================
// Called when any explosion is created in the game world.
// Parameter structure (from r3+8):
// +0x00: X coordinate (float)
// +0x04: Y coordinate (float)
// +0x08: Z coordinate (float)
// +0x0C: Explosion type (int)
// +0x10: Radius (float)

extern "C" void __imp__sub_82546D20(PPCContext& ctx, uint8_t* base);

PPC_FUNC_HOOK(sub_82546D20)
{
    PPC_FUNC_PROLOGUE();
    
    // Extract explosion parameters before calling original
    uint32_t paramStructAddr = PPC_LOAD_U32(ctx.r3.u32 + 8);
    
    float explosionX = 0.0f, explosionY = 0.0f, explosionZ = 0.0f;
    float explosionRadius = 5.0f;
    int explosionType = 0;
    
    if (paramStructAddr) {
        // Read float values (need to convert from PPC format)
        uint32_t xRaw = PPC_LOAD_U32(paramStructAddr + 0);
        uint32_t yRaw = PPC_LOAD_U32(paramStructAddr + 4);
        uint32_t zRaw = PPC_LOAD_U32(paramStructAddr + 8);
        explosionType = static_cast<int>(PPC_LOAD_U32(paramStructAddr + 12));
        uint32_t radiusRaw = PPC_LOAD_U32(paramStructAddr + 16);
        
        // Convert from uint32 to float (bit reinterpret)
        std::memcpy(&explosionX, &xRaw, sizeof(float));
        std::memcpy(&explosionY, &yRaw, sizeof(float));
        std::memcpy(&explosionZ, &zRaw, sizeof(float));
        std::memcpy(&explosionRadius, &radiusRaw, sizeof(float));
    }
    
    // Call original function
    __imp__sub_82546D20(ctx, base);
    
    // Trigger explosion haptic
    DS4Patches::OnExplosion(explosionX, explosionY, explosionZ, explosionRadius, explosionType);
}

// ============================================================================
// Phase 1: GET_CAR_HEALTH Hook (sub_82563DD0) - Collision Haptics
// ============================================================================
// Called to get vehicle health. We track changes to detect collisions.

extern "C" void __imp__sub_82563DD0(PPCContext& ctx, uint8_t* base);

static int s_trackedVehicleHealth = 1000;

PPC_FUNC_HOOK(sub_82563DD0)
{
    PPC_FUNC_PROLOGUE();
    
    // Call original function first
    __imp__sub_82563DD0(ctx, base);
    
    // The function stores health directly to the output pointer from paramStruct+4.
    // PPC code: stfiwx f0,0,r8 where r8 = *(paramStruct+4)
    // The health value is stored as a converted float->int at *r8.
    uint32_t paramStructAddr = PPC_LOAD_U32(ctx.r3.u32 + 8);
    if (paramStructAddr) {
        // r8 = output pointer (stores health directly)
        uint32_t healthOutPtr = PPC_LOAD_U32(paramStructAddr + 4);
        if (healthOutPtr) {
            // The value was stored directly to healthOutPtr by the original function
            int32_t health = static_cast<int32_t>(PPC_LOAD_U32(healthOutPtr));
            
            // Check for health decrease (collision/damage)
            if (health < s_trackedVehicleHealth) {
                DS4Patches::OnVehicleHealthChanged(health, s_trackedVehicleHealth);
            }
            s_trackedVehicleHealth = health;
        }
    }
}

// ============================================================================
// NOTE: GET_CHAR_COORDINATES (sub_8258FE30) does not exist in the recompiled PPC code.
// Player position is tracked via the explosion hook using the explosion coordinates
// relative to the player's last known position. For more accurate tracking, we could
// hook a different function that the game calls frequently with player position.
// ============================================================================

// ============================================================================
// Phase 2: IS_CHAR_SHOOTING Hook (sub_82586680) - Nearby Gunfire Haptics
// ============================================================================
// Called to check if a ped is shooting. We track this for nearby NPC gunfire.

extern "C" void __imp__sub_82586680(PPCContext& ctx, uint8_t* base);

PPC_FUNC_HOOK(sub_82586680)
{
    PPC_FUNC_PROLOGUE();
    
    // Get the ped handle before calling original
    uint32_t paramStructAddr = PPC_LOAD_U32(ctx.r3.u32 + 8);
    uint32_t pedHandle = 0;
    if (paramStructAddr) {
        pedHandle = PPC_LOAD_U32(paramStructAddr + 0);
    }
    
    // Call original function
    __imp__sub_82586680(ctx, base);
    
    // Check result - if ped is shooting
    uint32_t returnPtr = PPC_LOAD_U32(ctx.r3.u32 + 0);
    if (returnPtr) {
        int32_t isShooting = static_cast<int32_t>(PPC_LOAD_U32(returnPtr));
        
        if (isShooting && pedHandle != 0) {
            // This is a non-player ped that is shooting
            // For now, trigger a subtle haptic (we'd need ped position for distance)
            // Using a fixed distance estimate of 20 meters
            DS4Patches::OnNearbyGunfire(20.0f);
        }
    }
}

// ============================================================================
// Phase 3: GET_CURRENT_WEATHER Hook (sub_82596428) - Weather Haptics
// ============================================================================
// Called to get current weather type. We use this to enable rain haptics.

extern "C" void __imp__sub_82596428(PPCContext& ctx, uint8_t* base);

PPC_FUNC_HOOK(sub_82596428)
{
    PPC_FUNC_PROLOGUE();
    
    // Call original function first
    __imp__sub_82596428(ctx, base);
    
    // Extract weather type from output
    uint32_t paramStructAddr = PPC_LOAD_U32(ctx.r3.u32 + 8);
    if (paramStructAddr) {
        uint32_t weatherOutPtr = PPC_LOAD_U32(paramStructAddr + 0);
        if (weatherOutPtr) {
            int32_t weatherType = static_cast<int32_t>(PPC_LOAD_U32(weatherOutPtr));
            DS4Patches::OnWeatherChanged(weatherType);
        }
    }
}

// ============================================================================
// Phase 3: GET_CAR_SPEED Hook (sub_82563E30) - Gear Change Detection
// ============================================================================
// Called frequently to get vehicle speed. We use this for gear change detection.

extern "C" void __imp__sub_82563E30(PPCContext& ctx, uint8_t* base);

PPC_FUNC_HOOK(sub_82563E30)
{
    PPC_FUNC_PROLOGUE();
    
    // Call original function first
    // This function calls sub_8255A700 internally which stores the speed value.
    __imp__sub_82563E30(ctx, base);
    
    // Extract speed from output.
    // PPC code: lwz r4,4(r11) gets output pointer from paramStruct+4
    // Then sub_8255A700 stores speed to *(r31) where r31=r4.
    uint32_t paramStructAddr = PPC_LOAD_U32(ctx.r3.u32 + 8);
    if (paramStructAddr) {
        uint32_t speedOutPtr = PPC_LOAD_U32(paramStructAddr + 4);
        if (speedOutPtr) {
            // Speed is stored directly to speedOutPtr by the called function
            uint32_t speedRaw = PPC_LOAD_U32(speedOutPtr);
            float speed;
            std::memcpy(&speed, &speedRaw, sizeof(float));
            
            DS4Patches::OnVehicleSpeedUpdate(speed);
        }
    }
}

// ============================================================================
// Phase 3: SET_CINEMATIC_BUTTON_ENABLED Hook (sub_825A2518) - Camera Toggle
// ============================================================================
// We hook this to inject camera toggle requests from touchpad tap.

extern "C" void __imp__sub_825A2518(PPCContext& ctx, uint8_t* base);

PPC_FUNC_HOOK(sub_825A2518)
{
    PPC_FUNC_PROLOGUE();
    
    // Check if we have a pending camera toggle from touchpad.
    // NOTE: This hook triggers whenever SET_CINEMATIC_BUTTON_ENABLED is called.
    // The input value is directly at paramStruct+0 (not a pointer to value).
    // PPC code: lwz r11,0(r10) where r10=paramStruct, so r11 = *(paramStruct+0)
    // 
    // To properly implement camera toggling from touchpad, we would need to either:
    // 1. Call this function ourselves with the desired state, OR
    // 2. Intercept and modify the direct value at paramStruct+0
    //
    // For now, we modify the input value directly when toggle is pending.
    if (DS4Patches::IsCameraTogglePending()) {
        uint32_t paramStructAddr = PPC_LOAD_U32(ctx.r3.u32 + 8);
        if (paramStructAddr) {
            // The enable flag is stored DIRECTLY at paramStruct+0 (not as a pointer)
            // Read current value and toggle it
            uint32_t currentValue = PPC_LOAD_U32(paramStructAddr + 0);
            PPC_STORE_U32(paramStructAddr + 0, currentValue ? 0 : 1);
        }
        DS4Patches::ClearCameraTogglePending();
    }
    
    // Call original function
    __imp__sub_825A2518(ctx, base);
}

#endif // !LIBERTY_RECOMP_PS4 && !LIBERTY_RECOMP_NX
