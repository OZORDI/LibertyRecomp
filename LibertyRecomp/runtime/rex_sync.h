/**
 * @file    rex_sync.h
 * @brief   VBlank semaphore tracking for VdSwap signaling
 *
 * Tracks semaphore guest addresses for VdSwap signaling.
 * This is a LibertyRecomp-specific feature with no RexGlue equivalent.
 *
 * Sync primitives (events, semaphores, mutants) are managed directly
 * by RexGlue's rex::kernel types via KernelState.
 */

#pragma once

#include <mutex>
#include <vector>
#include <cstdint>

namespace rex {
namespace adapter {

// =============================================================================
// VBlank Tracker
// =============================================================================
// Tracks semaphore guest addresses for VdSwap signaling.

class VBlankTracker {
public:
    static VBlankTracker& instance() {
        static VBlankTracker inst;
        return inst;
    }

    void RegisterSemaphore(uint32_t guest_addr) {
        std::lock_guard<std::mutex> lock(mutex_);
        semaphore_addrs_.push_back(guest_addr);
    }

    std::vector<uint32_t> GetAllSemaphoreAddresses() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return semaphore_addrs_;
    }

private:
    VBlankTracker() = default;
    mutable std::mutex mutex_;
    std::vector<uint32_t> semaphore_addrs_;
};

} // namespace adapter
} // namespace rex
