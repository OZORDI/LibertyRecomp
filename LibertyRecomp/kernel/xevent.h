/**
 * XEvent - Native OS event wrapper for Xbox 360 kernel events
 * 
 * Based on xenia/kernel/xevent.h
 * Uses native OS primitives for proper synchronization.
 */

#pragma once

#include <stdafx.h>
#include <cstdint>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include "xdm.h"

namespace kernel {

/**
 * XEvent wraps native OS synchronization primitives.
 * 
 * Supports both manual-reset and auto-reset events.
 * Uses condition variables for efficient waiting.
 */
class XEvent final : public KernelObject {
public:
    using guest_type = XKEVENT;

    // Create from guest structure
    explicit XEvent(XKEVENT* header);
    
    // Create directly with parameters
    XEvent(bool manual_reset, bool initial_state);
    
    ~XEvent() override = default;

    // KernelObject interface
    uint32_t Wait(uint32_t timeout_ms) override;

    // Event operations
    bool Set();
    bool Reset();
    bool Pulse();
    
    // State query (requires lock for thread safety)
    bool IsSignaled() const { 
        std::lock_guard<std::mutex> lock(mutex_);
        return signaled_; 
    }
    bool IsManualReset() const { return manual_reset_; }

private:
    bool manual_reset_;
    // Use plain bool under mutex protection - no atomics needed
    bool signaled_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
};

} // namespace kernel
