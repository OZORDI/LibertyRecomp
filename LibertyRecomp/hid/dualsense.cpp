#include <stdafx.h>
#include <hid/dualsense.h>
#include <hid/hid.h>
#include <SDL3/SDL.h>
#include <os/logger.h>
#include <cstring>
#include <algorithm>
#include <array>

/**
 * DualSense Controller Implementation
 * 
 * This implementation uses SDL3's HIDAPI support to communicate with the
 * DualSense controller. SDL3 includes native DualSense support.
 * 
 * Protocol notes:
 * - USB mode: 48-byte output reports, Report ID 0x02
 * - Bluetooth mode: 78-byte output reports, Report ID 0x31 with CRC32
 * - Trigger effects: 11 bytes each at offsets 10-20 (R2) and 21-31 (L2)
 * 
 * We leverage SDL's existing controller support rather than raw HID writes
 * where possible, falling back to direct HID for features SDL doesn't expose.
 */

// ============================================================================
// Forward declarations for Controller class from sdl_hid.cpp
// ============================================================================
// This allows dualsense.cpp to access the active controller
// Note: We need to match the Controller class layout from sdl_hid.cpp

class Controller
{
public:
    SDL_Gamepad* controller{};
    SDL_Joystick* joystick{};
    SDL_JoystickID id{ 0 };
    // ... other members not needed for our purposes
};

extern std::array<Controller, 4> g_controllers;
extern Controller* g_activeController;

namespace DualSense
{
    // ========================================================================
    // INTERNAL STATE
    // ========================================================================
    
    static bool s_initialized = false;
    static bool s_connected = false;
    static bool s_bluetoothMode = false;
    
    // Current trigger effect states
    static TriggerEffect s_leftTriggerEffect;
    static TriggerEffect s_rightTriggerEffect;
    
    // Pending state (only send when changed)
    static bool s_effectsDirty = false;
    
    // Last applied weapon slot (avoid redundant updates)
    static int s_lastWeaponSlot = -1;
    
    // Recoil animation state
    static uint64_t s_recoilEndTime = 0;
    static TriggerEffect s_preRecoilEffect;
    static bool s_inRecoilAnimation = false;
    
    // Haptic state
    static uint8_t s_leftHaptic = 0;
    static uint8_t s_rightHaptic = 0;
    
    // ========================================================================
    // CRC32 CALCULATION (for Bluetooth mode)
    // ========================================================================
    // CRC32 polynomial and seed from DualSense protocol research
    
    static const uint32_t CRC32_TABLE[256] = {
        0xD202EF8D, 0xA505DF1B, 0x3C0C8EA1, 0x4B0BBE37, 0xD56F2B94, 0xA2681B02, 0x3B614AB8, 0x4C667A2E,
        0xDCD967BF, 0xABDE5729, 0x32D70693, 0x45D03605, 0xDBB4A3A6, 0xACB39330, 0x35BAC28A, 0x42BDF21C,
        0xCFB5BF09, 0xB8B28F9F, 0x21BBDE25, 0x56BCEE93, 0xC8D87B30, 0xBFDF4BA6, 0x26D61A1C, 0x51D12A8A,
        0xC16E671B, 0xB669578D, 0x2F600637, 0x586736A1, 0xC603A302, 0xB1049394, 0x280DC22E, 0x5F0AF2B8,
        0xE96CCF45, 0x9E6BFFD3, 0x0762AE69, 0x70659EFF, 0xEE010B5C, 0x99063BCA, 0x000F6A70, 0x77085AE6,
        0xE7B71777, 0x90B027E1, 0x09B9765B, 0x7EBE46CD, 0xE0DAD36E, 0x97DDE3F8, 0x0ED4B242, 0x79D382D4,
        0xF4DBCFC1, 0x83DCFF57, 0x1AD5AEED, 0x6DD29E7B, 0xF3B60BD8, 0x84B13B4E, 0x1DB86AF4, 0x6ABF5A62,
        0xFA0017F3, 0x8D072765, 0x140E76DF, 0x63094649, 0xFD6DD3EA, 0x8A6AE37C, 0x1363B2C6, 0x64648250,
        0xD4D91A39, 0xA3DE2AAF, 0x3AD77B15, 0x4DD04B83, 0xD3B4DE20, 0xA4B3EEB6, 0x3DBABF0C, 0x4ABD8F9A,
        0xDA02C20B, 0xAD05F29D, 0x3402A327, 0x430193B1, 0xDD650612, 0xAA623684, 0x336B673E, 0x446C57A8,
        0xC9641ABD, 0xBE632A2B, 0x276A7B91, 0x506D4B07, 0xCE09DEA4, 0xB90EEE32, 0x2007BF88, 0x57008F1E,
        0xC7BFC28F, 0xB0B8F219, 0x29B1A3A3, 0x5EB69335, 0xC0D20696, 0xB7D53600, 0x2EDC67BA, 0x59DB572C,
        0xEFBD6AD1, 0x98BA5A47, 0x01B30BFD, 0x76B43B6B, 0xE8D0AEC8, 0x9FD79E5E, 0x06DECFE4, 0x71D9FF72,
        0xE166B2E3, 0x96618275, 0x0F68D3CF, 0x786FE359, 0xE60B76FA, 0x910C466C, 0x080517D6, 0x7F022740,
        0xF20A6A55, 0x850D5AC3, 0x1C040B79, 0x6B033BEF, 0xF567AE4C, 0x82609EDA, 0x1B69CF60, 0x6C6EFFF6,
        0xFCD1B267, 0x8BD682F1, 0x12DFD34B, 0x65D8E3DD, 0xFBBC767E, 0x8CBB46E8, 0x15B21752, 0x62B527C4,
        0xA9B4D1B3, 0xDEB3E125, 0x47BAB09F, 0x30BD8009, 0xAED915AA, 0xD9DE253C, 0x40D77486, 0x37D04410,
        0xA76F0981, 0xD0683917, 0x496168AD, 0x3E66583B, 0xA002CD98, 0xD705FD0E, 0x4E0CACB4, 0x390B9C22,
        0xB403D137, 0xC304E1A1, 0x5A0DB01B, 0x2D0A808D, 0xB36E152E, 0xC46925B8, 0x5D607402, 0x2A674494,
        0xBAD80905, 0xCDDF3993, 0x54D66829, 0x23D158BF, 0xBDB5CD1C, 0xCAB2FD8A, 0x53BBAC30, 0x24BC9CA6,
        0x92DAA15B, 0xE5DD91CD, 0x7CD4C077, 0x0BD3F0E1, 0x95B76542, 0xE2B055D4, 0x7BB9046E, 0x0CBE34F8,
        0x9C017969, 0xEB0649FF, 0x720F1845, 0x050828D3, 0x9B6CBD70, 0xEC6B8DE6, 0x7562DC5C, 0x0265ECCA,
        0x8F6DA1DF, 0xF86A9149, 0x6163C0F3, 0x1664F065, 0x880065C6, 0xFF075550, 0x660E04EA, 0x1109347C,
        0x81B679ED, 0xF6B1497B, 0x6FB818C1, 0x18BF2857, 0x86DBBDF4, 0xF1DC8D62, 0x68D5DCD8, 0x1FD2EC4E,
        0xA66F7427, 0xD16844B1, 0x4861150B, 0x3F66259D, 0xA102B03E, 0xD60580A8, 0x4F0CD112, 0x380BE184,
        0xA8B4AC15, 0xDFB39C83, 0x46BACD39, 0x31BDFDAF, 0xAFD9680C, 0xD8DE589A, 0x41D70920, 0x36D039B6,
        0xBBD874A3, 0xCCDF4435, 0x55D6158F, 0x22D12519, 0xBCB5B0BA, 0xCBB2802C, 0x52BBD196, 0x25BCE100,
        0xB503AC91, 0xC2049C07, 0x5B0DCDBD, 0x2C0AFD2B, 0xB26E6888, 0xC569581E, 0x5C6009A4, 0x2B673932,
        0x9D0104CF, 0xEA063459, 0x730F65E3, 0x04085575, 0x9A6CC0D6, 0xED6BF040, 0x7462A1FA, 0x0365916C,
        0x93DADCFD, 0xE4DDEC6B, 0x7DD4BDD1, 0x0AD38D47, 0x94B718E4, 0xE3B02872, 0x7AB979C8, 0x0DBE495E,
        0x80B6044B, 0xF7B134DD, 0x6EB86567, 0x19BF55F1, 0x87DBC052, 0xF0DCF0C4, 0x69D5A17E, 0x1ED291E8,
        0x8E6DDC79, 0xF96AECEF, 0x6063BD55, 0x17648DC3, 0x89001860, 0xFE0728F6, 0x670E794C, 0x100949DA
    };
    
    static constexpr uint32_t CRC32_SEED = 0xEADA2D49;
    
    static uint32_t ComputeCRC32(const uint8_t* data, size_t length)
    {
        uint32_t crc = CRC32_SEED;
        for (size_t i = 0; i < length; i++)
        {
            crc = CRC32_TABLE[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
        }
        return crc;
    }
    
    // ========================================================================
    // TRIGGER EFFECT ENCODING
    // ========================================================================
    // Encode TriggerEffect struct into 11-byte hardware format
    
    static void EncodeTriggerEffect(const TriggerEffect& effect, uint8_t* buffer)
    {
        // Zero the buffer first
        memset(buffer, 0, 11);
        
        // First byte is always the mode
        buffer[0] = static_cast<uint8_t>(effect.mode);
        
        switch (effect.mode)
        {
            case TriggerMode::Off:
                // All zeros - no effect
                break;
                
            case TriggerMode::Continuous:
            // case TriggerMode::Feedback:  // Alias of Continuous - same value
                // Mode 0x01: [mode, start, strength, 0, 0, 0, 0, 0, 0, 0, 0]
                // DualSense-Windows uses 0-9 scale for position, 0-8 for strength
                // We use full 0-255 range and map internally
                buffer[1] = effect.continuous.startPosition;
                buffer[2] = effect.continuous.strength;
                break;
                
            case TriggerMode::Section:
            // case TriggerMode::Weapon:  // Alias of Section - same value
                // Mode 0x02: [mode, start, end, strength, 0, 0, 0, 0, 0, 0, 0]
                buffer[1] = effect.section.startPosition;
                buffer[2] = effect.section.endPosition;
                buffer[3] = effect.section.strength;
                break;
                
            case TriggerMode::ContinuousSlope:
                // Mode 0x05: Similar to continuous but with gradient
                buffer[1] = effect.continuous.startPosition;
                buffer[2] = effect.continuous.strength;
                break;
                
            case TriggerMode::SectionSlope:
            // case TriggerMode::Vibration:  // Alias of SectionSlope - same value
                // Mode 0x06: Vibration effect
                // [mode, pos, amplitude, freq, 0, 0, 0, 0, 0, 0, 0]
                buffer[1] = effect.vibration.startPosition;
                buffer[2] = effect.vibration.strength;
                buffer[3] = effect.vibration.frequency;
                break;
                
            case TriggerMode::MultiZone:
            case TriggerMode::MultiZoneSection:
            case TriggerMode::MultiZoneSectionSlope:
            case TriggerMode::MultiZoneContinuous:
            case TriggerMode::MultiZonePulse:
                // Extended mode: [mode, start, keep, 0, begin, middle, end, 0, 0, freq, 0]
                buffer[1] = 0xFF - effect.extended.startPosition; // Inverted per protocol
                buffer[2] = effect.extended.keepEffect;
                buffer[4] = effect.extended.beginForce;
                buffer[5] = effect.extended.middleForce;
                buffer[6] = effect.extended.endForce;
                buffer[9] = std::max<uint8_t>(1, effect.extended.frequency / 2);
                break;
                
            case TriggerMode::Calibration:
                // Mode 0xFC: Calibration mode
                buffer[0] = 0xFC;
                break;
                
            default:
                // Unknown mode - leave as zeros (disabled)
                buffer[0] = 0x00;
                break;
        }
    }
    
    // ========================================================================
    // SDL INTEGRATION
    // ========================================================================
    // Use SDL's game controller API for basic operations,
    // and raw HID for adaptive triggers (not exposed by SDL)
    
    // Helper function to get active controller from external global
    static SDL_Gamepad* GetActiveController()
    {
        if (!g_activeController)
            return nullptr;
        return g_activeController->controller;
    }
    
    // Check if connected controller is DualSense
    static bool IsDualSenseController(SDL_Gamepad* controller)
    {
        if (!controller) return false;
        
        SDL_GamepadType type = SDL_GetGamepadType(controller);
        return type == SDL_GAMEPAD_TYPE_PS5;
    }
    
    // Check connection mode (USB vs Bluetooth)
    static bool DetectBluetoothMode(SDL_Gamepad* controller)
    {
        if (!controller) return false;
        
        // SDL doesn't directly expose this, but we can check device path
        // BT devices typically have different path patterns
        SDL_Joystick* joystick = SDL_GetGamepadJoystick(controller);
        if (!joystick) return false;
        
        // Get device path if available (SDL 2.24.0+)
        // SDL3: path always available
        {
        const char* path = SDL_GetJoystickPath(joystick);
        if (path)
        {
            // Platform-specific BT detection
            #if defined(_WIN32)
            // Windows: USB paths contain "VID&", BT paths contain "HID#" without VID&
            if (strstr(path, "VID&") == nullptr && strstr(path, "HID#") != nullptr)
                return true;
            #elif defined(__APPLE__)
            // macOS: IOHIDDevice paths
            // BT devices typically have different path patterns than USB
            // USB paths usually contain "USB" or "AppleUSB"
            // BT paths typically don't contain these markers
            // Note: SDL handles mode detection internally, this is informational
            if (strstr(path, "USB") == nullptr && 
                strstr(path, "AppleUSB") == nullptr &&
                strstr(path, "usb") == nullptr)
            {
                return true;
            }
            #elif defined(__linux__)
            // Linux: Check for bluetooth vs USB in path
            // USB devices typically have "usb" in path
            // BT devices typically use hidraw without usb in path
            if (strstr(path, "bluetooth") != nullptr || 
                strstr(path, "/input/") != nullptr)
            {
                return true;
            }
            // hidraw without usb in parent path is likely Bluetooth
            if (strstr(path, "hidraw") != nullptr && strstr(path, "/usb") == nullptr)
            {
                return true;
            }
            #else
            // Other platforms: Assume USB (safer default)
            (void)path;
            #endif
        }
        }
        
        // Default to USB mode (safer - smaller packets)
        // Note: SDL_SendGamepadEffect will use the correct mode internally
        // for DualSense controllers, so this detection is primarily informational
        return false;
    }
    
    // Build and send output report for trigger effects
    static void SendEffectReport()
    {
        // Get active controller
        SDL_Gamepad* controller = GetActiveController();
        if (!controller || !IsDualSenseController(controller))
        {
            s_connected = false;
            return;
        }
        
        s_connected = true;
        s_bluetoothMode = DetectBluetoothMode(controller);
        
        // SDL3 supports DualSense but doesn't expose adaptive triggers directly
        // We use SDL_SendGamepadEffect() to send raw effect packets
        
        // SDL3: SendEffect always available
        // Build the effect state packet
        // Using DS5EffectsState_t layout from SDL's hidapi_ps5.c
        
        struct DS5EffectsPacket
        {
            uint8_t enableBits1;              // Byte 0
            uint8_t enableBits2;              // Byte 1
            uint8_t rumbleRight;              // Byte 2 (high freq)
            uint8_t rumbleLeft;               // Byte 3 (low freq)
            uint8_t headphoneVolume;          // Byte 4
            uint8_t speakerVolume;            // Byte 5
            uint8_t microphoneVolume;         // Byte 6
            uint8_t audioEnableBits;          // Byte 7
            uint8_t micLightMode;             // Byte 8
            uint8_t audioMuteBits;            // Byte 9
            uint8_t rightTriggerEffect[11];   // Bytes 10-20
            uint8_t leftTriggerEffect[11];    // Bytes 21-31
            uint8_t reserved1[6];             // Bytes 32-37
            uint8_t enableBits3;              // Byte 38
            uint8_t reserved2[2];             // Bytes 39-40
            uint8_t ledAnim;                  // Byte 41
            uint8_t ledBrightness;            // Byte 42
            uint8_t padLights;                // Byte 43
            uint8_t ledRed;                   // Byte 44
            uint8_t ledGreen;                 // Byte 45
            uint8_t ledBlue;                  // Byte 46
        };
        
        DS5EffectsPacket packet = {};
        
        // Enable trigger effects
        // Bit flags from SDL's hidapi_ps5.c:
        // k_EDS5EffectRumbleStart = 0x01
        // k_EDS5EffectRumble = 0x02
        // k_EDS5EffectRightTrigger = 0x04
        // k_EDS5EffectLeftTrigger = 0x08
        // k_EDS5EffectMicLight = 0x10
        // k_EDS5EffectAudioMute = 0x20
        // k_EDS5EffectLED = 0x40
        // k_EDS5EffectLEDAnim = 0x80
        packet.enableBits1 = 0x0C; // Right trigger (0x04) + Left trigger (0x08)
        packet.enableBits2 = 0x00;
        
        // Add rumble if haptics are active
        if (s_leftHaptic > 0 || s_rightHaptic > 0)
        {
            packet.enableBits1 |= 0x03; // Add rumble bits
            packet.rumbleLeft = s_leftHaptic;
            packet.rumbleRight = s_rightHaptic;
        }
        
        // Encode trigger effects
        EncodeTriggerEffect(s_rightTriggerEffect, packet.rightTriggerEffect);
        EncodeTriggerEffect(s_leftTriggerEffect, packet.leftTriggerEffect);
        
        // Send via SDL's effect API
        bool result = SDL_SendGamepadEffect(controller, &packet, sizeof(packet));
        
        if (!result)
        {
            // SDL_SendGamepadEffect failed
            // This can happen if the controller disconnected or doesn't support effects
            static bool s_loggedWarning = false;
            if (!s_loggedWarning)
            {
                LOGF_WARNING("DualSense: Failed to send effect packet: {}", SDL_GetError());
                s_loggedWarning = true;
            }
        }
        
        s_effectsDirty = false;
    }
    
    // ========================================================================
    // PUBLIC API IMPLEMENTATION
    // ========================================================================
    
    bool IsConnected()
    {
        // Check if we have a DualSense controller
        if (!s_initialized) return false;
        
        // Refresh connection status
        SDL_Gamepad* controller = GetActiveController();
        s_connected = IsDualSenseController(controller);
        
        return s_connected;
    }
    
    bool IsBluetoothMode()
    {
        return s_connected && s_bluetoothMode;
    }
    
    void SetTriggerEffect(Trigger trigger, const TriggerEffect& effect)
    {
        if (trigger == Trigger::Left)
        {
            if (memcmp(&s_leftTriggerEffect, &effect, sizeof(effect)) != 0)
            {
                s_leftTriggerEffect = effect;
                s_effectsDirty = true;
            }
        }
        else
        {
            if (memcmp(&s_rightTriggerEffect, &effect, sizeof(effect)) != 0)
            {
                s_rightTriggerEffect = effect;
                s_effectsDirty = true;
            }
        }
    }
    
    void ClearTriggerEffect(Trigger trigger)
    {
        SetTriggerEffect(trigger, TriggerEffect::MakeOff());
    }
    
    void ApplyWeaponProfile(uint8_t weaponSlot)
    {
        // Clamp to valid range
        if (weaponSlot >= sizeof(WEAPON_PROFILES) / sizeof(WEAPON_PROFILES[0]))
        {
            weaponSlot = 0;
        }
        
        // Skip if unchanged
        if (static_cast<int>(weaponSlot) == s_lastWeaponSlot)
        {
            return;
        }
        
        s_lastWeaponSlot = weaponSlot;
        
        const auto& profile = WEAPON_PROFILES[weaponSlot];
        
        // Build trigger effect based on profile
        TriggerEffect effect;
        
        if (profile.strength == 0)
        {
            effect = TriggerEffect::MakeOff();
        }
        else if (profile.isAutomatic)
        {
            // Automatic weapons use vibration mode
            effect = TriggerEffect::MakeVibration(
                profile.startPosition,
                profile.strength,
                profile.recoilFrequency
            );
        }
        else
        {
            // Semi-auto weapons use section resistance (crisp break)
            effect = TriggerEffect::MakeSection(
                profile.startPosition,
                255, // End at full travel
                profile.strength
            );
        }
        
        // Apply to R2 (fire trigger)
        SetTriggerEffect(Trigger::Right, effect);
        
        // L2 gets lighter resistance for ADS (aim down sights)
        if (profile.strength > 0)
        {
            TriggerEffect adsEffect = TriggerEffect::MakeContinuous(0, profile.strength / 3);
            SetTriggerEffect(Trigger::Left, adsEffect);
        }
        else
        {
            ClearTriggerEffect(Trigger::Left);
        }
    }
    
    void ApplyWeaponRecoil(uint8_t weaponSlot)
    {
        if (weaponSlot >= sizeof(WEAPON_PROFILES) / sizeof(WEAPON_PROFILES[0]))
        {
            return;
        }
        
        const auto& profile = WEAPON_PROFILES[weaponSlot];
        
        if (profile.recoilStrength == 0)
        {
            return; // No recoil for this weapon
        }
        
        // Save current effect for restoration
        if (!s_inRecoilAnimation)
        {
            s_preRecoilEffect = s_rightTriggerEffect;
        }
        
        // Apply recoil vibration
        TriggerEffect recoilEffect = TriggerEffect::MakeVibration(
            0,  // Start immediately
            profile.recoilStrength,
            profile.isAutomatic ? profile.recoilFrequency : 100 // Quick pulse for semi-auto
        );
        
        SetTriggerEffect(Trigger::Right, recoilEffect);
        
        // Set restoration timer
        // Semi-auto: 80ms recoil, Auto: sustain while firing
        s_recoilEndTime = SDL_GetTicks() + (profile.isAutomatic ? 50 : 80);
        s_inRecoilAnimation = true;
    }
    
    void ApplyVehicleProfile(uint8_t vehicleClass, uint8_t speed, 
                             uint8_t throttle, uint8_t brake)
    {
        if (vehicleClass >= sizeof(VEHICLE_PROFILES) / sizeof(VEHICLE_PROFILES[0]))
        {
            vehicleClass = 0; // Default to compact
        }
        
        // Clear weapon slot tracking (vehicle mode)
        s_lastWeaponSlot = -1;
        
        const auto& profile = VEHICLE_PROFILES[vehicleClass];
        
        // Calculate throttle resistance (R2)
        // Scales from base resistance to max based on throttle input
        uint8_t throttleResistance = profile.throttleBase;
        if (throttle > 0)
        {
            uint8_t addedResistance = (profile.throttleMax - profile.throttleBase) * throttle / 255;
            throttleResistance = profile.throttleBase + addedResistance;
        }
        
        TriggerEffect throttleEffect = TriggerEffect::MakeContinuous(
            profile.throttleStart,
            throttleResistance
        );
        SetTriggerEffect(Trigger::Right, throttleEffect);
        
        // Calculate brake resistance (L2)
        // Scales from base to max based on speed (harder braking at high speed)
        uint8_t brakeResistance = profile.brakeBase;
        if (speed > 0)
        {
            uint8_t addedResistance = (profile.brakeMax - profile.brakeBase) * speed / 255;
            brakeResistance = profile.brakeBase + addedResistance;
        }
        
        // Check for ABS simulation (high speed + hard braking)
        if (speed > profile.absThreshold && brake > 180)
        {
            // ABS pulsing effect
            TriggerEffect brakeEffect = TriggerEffect::MakeVibration(
                profile.brakeStart,
                brakeResistance,
                profile.absFrequency
            );
            SetTriggerEffect(Trigger::Left, brakeEffect);
        }
        else
        {
            // Normal brake resistance
            TriggerEffect brakeEffect = TriggerEffect::MakeContinuous(
                profile.brakeStart,
                brakeResistance
            );
            SetTriggerEffect(Trigger::Left, brakeEffect);
        }
    }
    
    void SetHapticFeedback(uint8_t leftMotor, uint8_t rightMotor)
    {
        if (s_leftHaptic != leftMotor || s_rightHaptic != rightMotor)
        {
            s_leftHaptic = leftMotor;
            s_rightHaptic = rightMotor;
            s_effectsDirty = true;
        }
    }
    
    void Initialize()
    {
        if (s_initialized) return;
        
        LOG_INFO("DualSense: Initializing adaptive trigger support");
        
        // Initialize state
        s_connected = false;
        s_bluetoothMode = false;
        s_effectsDirty = false;
        s_lastWeaponSlot = -1;
        s_recoilEndTime = 0;
        s_inRecoilAnimation = false;
        s_leftHaptic = 0;
        s_rightHaptic = 0;
        
        // Clear trigger effects
        s_leftTriggerEffect = TriggerEffect::MakeOff();
        s_rightTriggerEffect = TriggerEffect::MakeOff();
        
        s_initialized = true;
        
        // Check for DualSense
        if (IsConnected())
        {
            LOGF_INFO("DualSense: Controller detected ({})",
                  s_bluetoothMode ? "Bluetooth" : "USB");
        }
    }
    
    void Update()
    {
        if (!s_initialized) return;
        
        // Handle recoil animation end
        if (s_inRecoilAnimation)
        {
            uint64_t now = SDL_GetTicks();
            if (now >= s_recoilEndTime)
            {
                // Restore pre-recoil effect
                SetTriggerEffect(Trigger::Right, s_preRecoilEffect);
                s_inRecoilAnimation = false;
            }
        }
        
        // Send effect update if changed
        if (s_effectsDirty && IsConnected())
        {
            SendEffectReport();
        }
    }
    
    void Shutdown()
    {
        if (!s_initialized) return;
        
        LOG_INFO("DualSense: Shutting down");
        
        // Clear all effects before shutdown
        ClearTriggerEffect(Trigger::Left);
        ClearTriggerEffect(Trigger::Right);
        s_leftHaptic = 0;
        s_rightHaptic = 0;
        s_effectsDirty = true;
        
        // Send final clear
        if (IsConnected())
        {
            SendEffectReport();
        }
        
        s_initialized = false;
        s_connected = false;
    }
}
