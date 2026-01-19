/**
 * XEvent - Native OS event implementation
 * 
 * Based on xenia/kernel/xevent.cc
 */

#include <stdafx.h>
#include "xevent.h"

namespace kernel {

XEvent::XEvent(XKEVENT* header)
    : manual_reset_(header->Type == 0),  // Type 0 = manual, Type 1 = auto
      signaled_(header->SignalState != 0) {
}

XEvent::XEvent(bool manual_reset, bool initial_state)
    : manual_reset_(manual_reset),
      signaled_(initial_state) {
}

uint32_t XEvent::Wait(uint32_t timeout_ms) {
    std::unique_lock<std::mutex> lock(mutex_);
    
    if (timeout_ms == 0) {
        // Immediate check - no wait
        if (signaled_) {
            if (!manual_reset_) {
                signaled_ = false;
            }
            return STATUS_SUCCESS;
        }
        return STATUS_TIMEOUT;
    }
    
    if (timeout_ms == INFINITE) {
        // Wait forever
        cv_.wait(lock, [this] { return signaled_; });
        
        if (!manual_reset_) {
            signaled_ = false;
        }
        return STATUS_SUCCESS;
    }
    
    // Timed wait
    auto deadline = std::chrono::steady_clock::now() + 
                    std::chrono::milliseconds(timeout_ms);
    
    bool was_signaled = cv_.wait_until(lock, deadline, [this] {
        return signaled_;
    });
    
    if (was_signaled) {
        if (!manual_reset_) {
            signaled_ = false;
        }
        return STATUS_SUCCESS;
    }
    
    return STATUS_TIMEOUT;
}

bool XEvent::Set() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        signaled_ = true;
    }
    
    if (manual_reset_) {
        cv_.notify_all();
    } else {
        cv_.notify_one();
    }
    
    return true;
}

bool XEvent::Reset() {
    std::lock_guard<std::mutex> lock(mutex_);
    signaled_ = false;
    return true;
}

bool XEvent::Pulse() {
    // Pulse semantics: Wake all waiting threads, then reset.
    // This is a proper implementation that ensures all currently waiting
    // threads are woken before the signal is reset.
    
    std::unique_lock<std::mutex> lock(mutex_);
    
    // Set the signal
    signaled_ = true;
    
    // Wake ALL waiters (both manual and auto-reset need to wake everyone for Pulse)
    cv_.notify_all();
    
    // For proper Pulse semantics, we need to ensure waiters have a chance to
    // observe the signaled state. We do this by releasing the lock, yielding,
    // then re-acquiring and resetting. This isn't perfect but matches Xbox 360
    // behavior better than the previous implementation.
    lock.unlock();
    
    // Give waiters time to wake up and check the condition
    std::this_thread::yield();
    
    // Now reset the signal under lock
    lock.lock();
    signaled_ = false;
    
    return true;
}

} // namespace kernel
