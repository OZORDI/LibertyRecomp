#include "sync_table.h"
#include "xdm.h"

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

static std::mutex g_syncTableMutex;
static std::unordered_map<uint32_t, SyncObject*> g_syncObjectTable;

uint32_t SyncObject::Wait(uint32_t timeoutMs) {
    waitCount++;
    
    
    std::unique_lock<std::mutex> lock(mtx);
    
    if (type == SyncType::Event) {
        if (timeoutMs == INFINITE) {
            cv.wait(lock, [this]{ return signalState.load() != 0; });
        } else if (timeoutMs == 0) {
            if (signalState.load() == 0) return STATUS_TIMEOUT;
        } else {
            if (!cv.wait_for(lock, std::chrono::milliseconds(timeoutMs), 
                             [this]{ return signalState.load() != 0; })) {
                return STATUS_TIMEOUT;
            }
        }
        if (!manualReset) signalState = 0;
    }
    else if (type == SyncType::Semaphore) {
        if (timeoutMs == INFINITE) {
            cv.wait(lock, [this]{ return signalState.load() > 0; });
            signalState--;
        } else if (timeoutMs == 0) {
            if (signalState.load() <= 0) return STATUS_TIMEOUT;
            signalState--;
        } else {
            if (!cv.wait_for(lock, std::chrono::milliseconds(timeoutMs),
                             [this]{ return signalState.load() > 0; })) {
                return STATUS_TIMEOUT;
            }
            signalState--;
        }
    }
    return STATUS_SUCCESS;
}

bool SyncObject::TryWait() {
    std::lock_guard<std::mutex> lock(mtx);
    
    if (type == SyncType::Event) {
        if (signalState.load() != 0) {
            if (!manualReset) signalState = 0;
            return true;
        }
        return false;
    }
    else if (type == SyncType::Semaphore) {
        if (signalState.load() > 0) {
            signalState--;
            return true;
        }
        return false;
    }
    return false;
}

void SyncObject::Signal(int32_t count) {
    signalCount++;
    // FIX: Clamp count to at least 1 - r4 often contains garbage (pointer values)
    if (count <= 0) count = 1;
    
    std::lock_guard<std::mutex> lock(mtx);
    if (type == SyncType::Event) {
        signalState = 1;
        if (manualReset) cv.notify_all(); else cv.notify_one();
    } else if (type == SyncType::Semaphore) {
        signalState += count;
        if (signalState > maxCount) signalState = maxCount;
        cv.notify_all();
    }
}

void SyncObject::Reset() {
    std::lock_guard<std::mutex> lock(mtx);
    signalState = 0;
}

SyncObject* SyncTable_GetOrCreate(uint32_t addr, SyncType type, uint32_t callerLR) {
    std::lock_guard<std::mutex> lock(g_syncTableMutex);
    auto it = g_syncObjectTable.find(addr);
    if (it != g_syncObjectTable.end()) return it->second;
    
    SyncObject* obj = new SyncObject(type, addr);
    obj->creatorLR = callerLR;
    g_syncObjectTable[addr] = obj;
    
    static int s_count = 0;
    if (++s_count <= 100) {
        printf("[SYNC-TABLE] CREATE %s @ 0x%08X (caller=0x%08X) total=%zu\n",
               type == SyncType::Event ? "event" : "semaphore",
               addr, callerLR, g_syncObjectTable.size());
        fflush(stdout);
    }
    return obj;
}

SyncObject* SyncTable_Get(uint32_t addr) {
    std::lock_guard<std::mutex> lock(g_syncTableMutex);
    auto it = g_syncObjectTable.find(addr);
    return (it != g_syncObjectTable.end()) ? it->second : nullptr;
}

void SyncTable_InitSemaphore(uint32_t addr, int32_t count, int32_t max, uint32_t callerLR) {
    SyncObject* obj = SyncTable_GetOrCreate(addr, SyncType::Semaphore, callerLR);
    obj->signalState = count;
    obj->maxCount = max;
}

void SyncTable_InitEvent(uint32_t addr, bool manual, bool initial, uint32_t callerLR) {
    SyncObject* obj = SyncTable_GetOrCreate(addr, SyncType::Event, callerLR);
    obj->manualReset = manual;
    obj->signalState = initial ? 1 : 0;
}

uint32_t SyncTable_Wait(uint32_t addr, uint32_t timeoutMs, uint32_t callerLR) {
    SyncObject* obj = SyncTable_Get(addr);
    if (!obj) return STATUS_SUCCESS; // Unknown object - don't block
    return obj->Wait(timeoutMs);
}

void SyncTable_Signal(uint32_t addr, int32_t count, uint32_t callerLR) {
    SyncObject* obj = SyncTable_Get(addr);
    if (obj) obj->Signal(count);
}

void SyncTable_DumpBroken() {
    std::lock_guard<std::mutex> lock(g_syncTableMutex);
    printf("\n=== SYNC TABLE DUMP ===\nTotal: %zu\n", g_syncObjectTable.size());
    int broken = 0;
    for (const auto& [addr, obj] : g_syncObjectTable) {
        if (obj->waitCount > 0 && obj->signalCount == 0) {
            printf("[BROKEN] %s @ 0x%08X: waits=%d signals=0 creator=0x%08X\n",
                   obj->type == SyncType::Event ? "event" : "sem",
                   addr, obj->waitCount.load(), obj->creatorLR);
            broken++;
        }
    }
    printf(broken ? "[SUMMARY] %d broken\n" : "[OK] All healthy\n", broken);
    printf("=======================\n\n");
    fflush(stdout);
}

uint32_t SyncTable_WaitMultiple(uint32_t count, uint32_t* addresses, uint32_t waitType,
                                 uint32_t timeoutMs, uint32_t callerLR) {
    // waitType: 0 = WaitAll, 1 = WaitAny
    // Returns: STATUS_WAIT_0 + index for WaitAny, STATUS_SUCCESS for WaitAll, STATUS_TIMEOUT on timeout
    
    static int s_callCount = 0;
    ++s_callCount;
    
    if (s_callCount <= 20 || s_callCount % 100 == 0) {
        printf("[SyncTable_WaitMultiple] #%d count=%u waitType=%u timeout=%u caller=0x%08X\n",
               s_callCount, count, waitType, timeoutMs, callerLR);
        for (uint32_t i = 0; i < count; i++) {
            printf("  [%u] addr=0x%08X\n", i, addresses[i]);
        }
        fflush(stdout);
    }
    
    // Collect/create all sync objects
    std::vector<SyncObject*> syncObjs(count);
    for (uint32_t i = 0; i < count; i++) {
        SyncObject* obj = SyncTable_Get(addresses[i]);
        if (!obj) {
            // Create on-the-fly - assume Event (most common for multi-wait)
            obj = SyncTable_GetOrCreate(addresses[i], SyncType::Event, callerLR);
        }
        syncObjs[i] = obj;
    }
    
    auto startTime = std::chrono::steady_clock::now();
    
    if (waitType == 0) {
        // WaitAll - wait on each object sequentially
        for (uint32_t i = 0; i < count; i++) {
            uint32_t remainingMs = timeoutMs;
            if (timeoutMs != INFINITE) {
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - startTime).count();
                if (elapsed >= timeoutMs) return STATUS_TIMEOUT;
                remainingMs = timeoutMs - static_cast<uint32_t>(elapsed);
            }
            
            uint32_t result = syncObjs[i]->Wait(remainingMs);
            if (result == STATUS_TIMEOUT) return STATUS_TIMEOUT;
        }
        return STATUS_SUCCESS;
    }
    else {
        // WaitAny - poll objects until one is signaled
        int pollCount = 0;
        while (true) {
            pollCount++;
            
            // Try each object with non-blocking check
            for (uint32_t i = 0; i < count; i++) {
                if (syncObjs[i]->TryWait()) {
                    if (s_callCount <= 20) {
                        printf("[SyncTable_WaitMultiple] WaitAny satisfied by index %u after %d polls\n", 
                               i, pollCount);
                        fflush(stdout);
                    }
                    return STATUS_WAIT_0 + i;
                }
            }
            
            // Check timeout
            if (timeoutMs != INFINITE) {
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - startTime).count();
                if (elapsed >= timeoutMs) {
                    printf("[SyncTable_WaitMultiple] WaitAny TIMEOUT after %d polls\n", pollCount);
                    fflush(stdout);
                    return STATUS_TIMEOUT;
                }
            }
            
            // Sleep briefly before polling again (1ms)
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}
