/**
 * XSemaphore - Native OS semaphore wrapper for Xbox 360 kernel semaphores
 * 
 * Based on xenia/kernel/xsemaphore.h
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
 * XSemaphore wraps native OS counting semaphore.
 * 
 * Properly handles acquire/release semantics with counting.
 * Uses condition variables for efficient waiting.
 */
class XSemaphore final : public KernelObject {
public:
    using guest_type = XKSEMAPHORE;

    // Create from guest structure
    explicit XSemaphore(XKSEMAPHORE* header);
    
    // Create directly with parameters
    XSemaphore(int32_t initial_count, int32_t maximum_count);
    
    ~XSemaphore() override = default;

    // KernelObject interface
    uint32_t Wait(uint32_t timeout_ms) override;

    // Semaphore operations
    int32_t Release(int32_t release_count, int32_t* previous_count = nullptr);
    
    // State query (requires lock for thread safety)
    int32_t GetCount() const { 
        std::lock_guard<std::mutex> lock(mutex_);
        return count_; 
    }
    int32_t GetMaximumCount() const { return maximum_count_; }

private:
    // Use plain int32_t under mutex protection - no atomics needed
    // This avoids the race condition from mixing mutex + atomics
    int32_t count_;
    int32_t maximum_count_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
};

} // namespace kernel
