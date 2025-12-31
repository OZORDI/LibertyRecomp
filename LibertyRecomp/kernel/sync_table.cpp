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
