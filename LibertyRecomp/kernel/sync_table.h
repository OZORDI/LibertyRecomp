#pragma once
// =============================================================================
// SYNC OBJECT TABLE - Xenia-style centralized sync primitive tracking
// =============================================================================

#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <chrono>
#include <thread>

// Platform-independent constants for sync primitives
#ifndef INFINITE
#define INFINITE 0xFFFFFFFF
#endif

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS 0x00000000
#endif

#ifndef STATUS_TIMEOUT
#define STATUS_TIMEOUT 0x00000102
#endif

#ifndef STATUS_WAIT_0
#define STATUS_WAIT_0 0x00000000
#endif

enum class SyncType : uint8_t { Event = 0, Semaphore = 1, Unknown = 255 };

struct SyncObject {
    SyncType type;
    uint32_t guestAddr;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<int32_t> signalState{0};
    int32_t maxCount{1};
    bool manualReset{false};
    std::atomic<int> waitCount{0};
    std::atomic<int> signalCount{0};
    uint32_t creatorLR{0};
    
    SyncObject(SyncType t, uint32_t addr) : type(t), guestAddr(addr) {}
    
    uint32_t Wait(uint32_t timeoutMs);
    bool TryWait(); // Non-blocking check - returns true if acquired
    void Signal(int32_t count = 1);
    void Reset();
};

// Global table access
SyncObject* SyncTable_GetOrCreate(uint32_t addr, SyncType type, uint32_t callerLR = 0);
SyncObject* SyncTable_Get(uint32_t addr);
void SyncTable_InitSemaphore(uint32_t addr, int32_t count, int32_t max, uint32_t callerLR);
void SyncTable_InitEvent(uint32_t addr, bool manual, bool initial, uint32_t callerLR);
uint32_t SyncTable_Wait(uint32_t addr, uint32_t timeoutMs, uint32_t callerLR);
void SyncTable_Signal(uint32_t addr, int32_t count, uint32_t callerLR);
void SyncTable_DumpBroken();

// WaitMultiple - waits on multiple sync objects
// waitType: 0 = WaitAll, 1 = WaitAny
// Returns: STATUS_WAIT_0 + index for WaitAny, STATUS_SUCCESS for WaitAll, STATUS_TIMEOUT on timeout
uint32_t SyncTable_WaitMultiple(uint32_t count, uint32_t* addresses, uint32_t waitType, 
                                 uint32_t timeoutMs, uint32_t callerLR);
