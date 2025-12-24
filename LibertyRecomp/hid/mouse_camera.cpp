#include <stdafx.h>
#include "mouse_camera.h"
#include <user/config.h>
#include <algorithm>
#include <cmath>
#include <chrono>

namespace MouseCamera
{
    // Mouse camera state
    static float s_velocityX = 0.0f;
    static float s_velocityY = 0.0f;
    static float s_sensitivityX = 1.0f;
    static float s_sensitivityY = 1.0f;
    static float s_smoothing = 0.5f;
    static bool s_invertY = false;
    static bool s_isActive = false;
    
    // Timing for activity detection
    static std::chrono::steady_clock::time_point s_lastMovementTime;
    static constexpr auto ACTIVITY_TIMEOUT = std::chrono::milliseconds(100);
    
    // Sensitivity scaling factors
    static constexpr float MOUSE_TO_ANALOG_SCALE = 100.0f;  // Pixels to analog range
    static constexpr float MAX_VELOCITY = 32767.0f;         // Max analog stick value
    static constexpr float DAMPING_FACTOR = 0.85f;          // Velocity decay when no input
    
    void Initialize()
    {
        s_velocityX = 0.0f;
        s_velocityY = 0.0f;
        s_isActive = false;
        s_lastMovementTime = std::chrono::steady_clock::now();
    }
    
    void Update(int32_t deltaX, int32_t deltaY, float deltaTime)
    {
        if (deltaX == 0 && deltaY == 0)
            return;
        
        // Mark as active
        s_isActive = true;
        s_lastMovementTime = std::chrono::steady_clock::now();
        
        // Apply config sensitivity and smoothing
        s_sensitivityX = Config::MouseSensitivityX;
        s_sensitivityY = Config::MouseSensitivityY;
        s_smoothing = Config::MouseSmoothing;
        s_invertY = Config::MouseInvertY;
        
        // Convert pixel delta to velocity with sensitivity
        float inputX = static_cast<float>(deltaX) * s_sensitivityX * MOUSE_TO_ANALOG_SCALE;
        float inputY = static_cast<float>(deltaY) * s_sensitivityY * MOUSE_TO_ANALOG_SCALE;
        
        // Apply Y-axis inversion
        if (s_invertY)
            inputY = -inputY;
        
        // Apply exponential smoothing (exponential moving average)
        // Higher smoothing = more lag but smoother motion
        float alpha = 1.0f - s_smoothing;  // Convert to blend factor
        s_velocityX = s_velocityX * (1.0f - alpha) + inputX * alpha;
        s_velocityY = s_velocityY * (1.0f - alpha) + inputY * alpha;
        
        // Clamp to analog stick range
        s_velocityX = std::clamp(s_velocityX, -MAX_VELOCITY, MAX_VELOCITY);
        s_velocityY = std::clamp(s_velocityY, -MAX_VELOCITY, MAX_VELOCITY);
    }
    
    void GetAnalogValues(int16_t& outX, int16_t& outY)
    {
        // Check if we've been inactive for too long
        auto now = std::chrono::steady_clock::now();
        if (now - s_lastMovementTime > ACTIVITY_TIMEOUT)
        {
            s_isActive = false;
            
            // Apply damping to gradually reduce velocity
            s_velocityX *= DAMPING_FACTOR;
            s_velocityY *= DAMPING_FACTOR;
            
            // Zero out if very small
            if (std::abs(s_velocityX) < 1.0f) s_velocityX = 0.0f;
            if (std::abs(s_velocityY) < 1.0f) s_velocityY = 0.0f;
        }
        
        // Convert to int16_t analog values
        outX = static_cast<int16_t>(std::clamp(s_velocityX, -MAX_VELOCITY, MAX_VELOCITY));
        outY = static_cast<int16_t>(std::clamp(s_velocityY, -MAX_VELOCITY, MAX_VELOCITY));
    }
    
    void Reset()
    {
        s_velocityX = 0.0f;
        s_velocityY = 0.0f;
        s_isActive = false;
    }
    
    bool IsActive()
    {
        return s_isActive;
    }
    
    void SetSensitivity(float sensitivityX, float sensitivityY)
    {
        s_sensitivityX = std::clamp(sensitivityX, 0.1f, 10.0f);
        s_sensitivityY = std::clamp(sensitivityY, 0.1f, 10.0f);
    }
    
    void SetInvertY(bool invert)
    {
        s_invertY = invert;
    }
    
    void SetSmoothing(float smoothing)
    {
        s_smoothing = std::clamp(smoothing, 0.0f, 0.95f);
    }
}
