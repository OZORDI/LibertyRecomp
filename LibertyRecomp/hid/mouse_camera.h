#pragma once

#include <cstdint>

/**
 * Mouse Camera Controller for GTA IV
 * 
 * Converts mouse delta movement into analog stick values for camera control.
 * Features:
 * - Configurable sensitivity (X and Y independent)
 * - Exponential smoothing to reduce jitter
 * - Y-axis inversion support
 * - Velocity accumulation with damping
 */

namespace MouseCamera
{
    /**
     * Initialize the mouse camera system.
     * Should be called once at startup.
     */
    void Initialize();
    
    /**
     * Update mouse camera state with new mouse delta.
     * Called from SDL event handler on mouse motion.
     * 
     * @param deltaX Mouse X movement in pixels (relative)
     * @param deltaY Mouse Y movement in pixels (relative)
     * @param deltaTime Frame delta time in seconds
     */
    void Update(int32_t deltaX, int32_t deltaY, float deltaTime);
    
    /**
     * Get current analog stick values for camera control.
     * Maps to right stick (camera) input.
     * 
     * @param outX Output X axis value (-32768 to 32767)
     * @param outY Output Y axis value (-32768 to 32767)
     */
    void GetAnalogValues(int16_t& outX, int16_t& outY);
    
    /**
     * Reset camera velocity to zero.
     * Called when mouse becomes inactive or focus is lost.
     */
    void Reset();
    
    /**
     * Check if mouse camera is currently active.
     * @return true if mouse has moved recently
     */
    bool IsActive();
    
    /**
     * Set mouse sensitivity multipliers.
     * @param sensitivityX Horizontal sensitivity (default: 1.0)
     * @param sensitivityY Vertical sensitivity (default: 1.0)
     */
    void SetSensitivity(float sensitivityX, float sensitivityY);
    
    /**
     * Set Y-axis inversion.
     * @param invert true to invert Y-axis (flight controls style)
     */
    void SetInvertY(bool invert);
    
    /**
     * Set smoothing factor.
     * @param smoothing Smoothing amount 0.0-1.0 (0=no smoothing, 1=max smoothing)
     */
    void SetSmoothing(float smoothing);
}
