#pragma once

#if defined(GTA4_SONY_LEGACY_OWNER)
#include <memory>
namespace rex::input { class InputSystem; }
#endif

namespace hid
{
    enum class EInputDevice
    {
        Unknown,
        Keyboard,
        Mouse,
        Xbox,
        PlayStation
    };

    enum class EInputDeviceExplicit
    {
        Unknown,
        Xbox360,
        XboxOne,
        XboxSeriesX,
        DualShock3,
        DualShock4,
        SwitchPro,
        Virtual,
        DualSense,
        Luna,
        Stadia,
        NvShield,
        SwitchJCLeft,
        SwitchJCRight,
        SwitchJCPair,
        SteamDeck,
        SteamController
    };

    extern EInputDevice g_inputDevice;
    extern EInputDevice g_inputDeviceController;
    extern EInputDeviceExplicit g_inputDeviceExplicit;

    extern uint16_t g_prohibitedButtons;
    extern bool g_isLeftStickProhibited;
    extern bool g_isRightStickProhibited;

    void Init();

#if defined(GTA4_SONY_LEGACY_OWNER)
    // Existing SDL owner, pumped on the legacy host's main thread.
    std::unique_ptr<rex::input::InputSystem> CreateRexInputSystem(bool toolMode);
    void UpdateSonyFeedback();
    void ShutdownSonyFeedback();
#endif

    uint32_t GetState(uint32_t dwUserIndex, XAMINPUT_STATE* pState);
    uint32_t SetState(uint32_t dwUserIndex, XAMINPUT_VIBRATION* pVibration);
    uint32_t GetCapabilities(uint32_t dwUserIndex, XAMINPUT_CAPABILITIES* pCaps);

    void SetProhibitedInputs(uint16_t wButtons = 0, bool leftStick = false, bool rightStick = false);
    bool IsInputAllowed();
    bool IsInputDeviceController();
    
    // Mouse wheel support for weapon switching
    int32_t GetMouseWheelDelta();
    void ResetMouseWheelDelta();
    
    // Keystroke queue for XamInputGetKeystrokeEx
    struct KeystrokeEvent {
        uint16_t virtualKey;
        uint16_t unicode;
        uint16_t flags;
        uint8_t userIndex;
    };
    
    void EnqueueKeystroke(const KeystrokeEvent& event);
    bool DequeueKeystroke(uint8_t userIndex, KeystrokeEvent& outEvent);
    void ClearKeystrokeQueue(uint8_t userIndex);
    
    // ========================================================================
    // Motion Sensing (Gyroscope/Accelerometer) Support
    // ========================================================================
    // For PlayStation controllers (DualShock 3/4, DualSense) and Switch Pro
    // Provides gyroscope (angular velocity) and accelerometer (linear acceleration) data
    
    struct MotionState {
        // Gyroscope - angular velocity in radians/second
        // X = pitch rate (nose up/down), Y = yaw rate (turn left/right), Z = roll rate
        float gyroX;
        float gyroY;
        float gyroZ;
        
        // Accelerometer - linear acceleration in g-forces (1g = 9.81 m/s²)
        // X = left/right, Y = forward/backward, Z = up/down
        // When controller is flat and still: X≈0, Y≈0, Z≈-1 (gravity)
        float accelX;
        float accelY;
        float accelZ;
        
        // Derived orientation (calculated from gyro integration + accel correction)
        float pitch;  // Forward/backward tilt in radians
        float roll;   // Left/right tilt in radians
        float yaw;    // Heading/rotation in radians (requires magnetometer or drift)
        
        // Sensor availability flags
        bool hasGyro;
        bool hasAccel;
        bool isCalibrated;
        
        // Timestamp for sensor fusion (microseconds)
        uint64_t timestamp;
    };
    
    // Check if current controller supports motion sensing
    bool HasMotionSensor();
    
    // Enable/disable motion sensor polling (may affect battery life)
    void SetMotionSensorEnabled(bool enabled);
    bool IsMotionSensorEnabled();
    
    // Get current motion state
    const MotionState& GetMotionState();
    
    // Poll motion sensors (called from input update loop)
    void UpdateMotionState();
    
    // Reset gyro orientation (recalibrate to current position)
    void ResetMotionOrientation();
    
    // ========================================================================
    // Light Bar Support (DualShock 4 / DualSense)
    // ========================================================================
    
    // Check if current controller has a light bar
    bool HasLightBar();
    
    // Set light bar color (RGB 0-255)
    void SetLightBarColor(uint8_t r, uint8_t g, uint8_t b);
    
    // ========================================================================
    // Touchpad Support (DualShock 4 / DualSense)
    // ========================================================================
    
    struct TouchpadState {
        // Finger 0 state
        bool finger0Down;
        float finger0X;  // 0.0 (left) to 1.0 (right)
        float finger0Y;  // 0.0 (top) to 1.0 (bottom)
        
        // Finger 1 state (for multi-touch)
        bool finger1Down;
        float finger1X;
        float finger1Y;
        
        // Gesture detection
        bool isTapping;
        bool isSwiping;
        float swipeVelocityX;
        float swipeVelocityY;
    };
    
    // Check if current controller has a touchpad
    bool HasTouchpad();
    
    // Get current touchpad state
    const TouchpadState& GetTouchpadState();
    
    // Update touchpad state (called from input update loop)
    void UpdateTouchpadState();
    
    // ========================================================================
    // DualSense Adaptive Trigger Support
    // ========================================================================
    // For DualSense (PS5) controllers - provides trigger resistance effects
    // and enhanced HD haptic feedback. See hid/dualsense.h for full API.
    
    // Check if current controller is DualSense with adaptive trigger support
    bool HasAdaptiveTriggers();
    
}
