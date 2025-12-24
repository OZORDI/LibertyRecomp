#include "net_session.h"
#include <kernel/memory.h>
#include <cstring>

namespace Net {

// ============================================================================
// SessionManager Implementation
// ============================================================================

SessionManager& SessionManager::Instance() {
    static SessionManager instance;
    return instance;
}

uint32_t SessionManager::CreateSessionHandle(uint32_t* outHandle) {
    if (!outHandle) {
        return 0x80070057;  // E_INVALIDARG
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    
    uint32_t handle = nextHandle_++;
    
    Session session;
    session.handle = handle;
    session.state = SessionState::Creating;
    
    // Generate fake session info
    session.info.sessionID = static_cast<uint64_t>(handle) << 32 | 0x12345678;
    session.info.hostAddress = 0x6401A8C0;  // 192.168.1.100 (network order)
    session.info.hostPort = ByteSwap(static_cast<uint16_t>(3074));
    
    sessions_[handle] = session;
    *outHandle = handle;
    
    LOGF_WARNING("[Session] CreateSessionHandle -> handle=0x{:08X}", handle);
    
    return 0;  // ERROR_SUCCESS
}

uint32_t SessionManager::RefSessionByHandle(uint32_t handle, uint32_t* outObj) {
    if (!outObj) {
        return 0x80070057;  // E_INVALIDARG
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = sessions_.find(handle);
    if (it == sessions_.end()) {
        // Xenia returns a fake object pointer to prevent crashes
        *outObj = SESSION_OBJECT_MAGIC;
        LOGF_WARNING("[Session] RefSessionByHandle handle=0x{:08X} not found, returning fake obj=0x{:08X}", 
                    handle, SESSION_OBJECT_MAGIC);
        return 0;  // Return success with fake object (Xenia approach)
    }
    
    // Return pointer to session info in guest memory
    // For now, return fake object pointer like Xenia
    *outObj = SESSION_OBJECT_MAGIC;
    
    LOGF_WARNING("[Session] RefSessionByHandle handle=0x{:08X} -> obj=0x{:08X}", handle, *outObj);
    
    return 0;  // ERROR_SUCCESS
}

void SessionManager::CloseSessionHandle(uint32_t handle) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = sessions_.find(handle);
    if (it != sessions_.end()) {
        LOGF_WARNING("[Session] CloseSessionHandle handle=0x{:08X}", handle);
        sessions_.erase(it);
    }
}

Session* SessionManager::GetSession(uint32_t handle) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = sessions_.find(handle);
    if (it != sessions_.end()) {
        return &it->second;
    }
    return nullptr;
}

// ============================================================================
// API Function Implementations
// ============================================================================

uint32_t XamSessionCreateHandle(uint32_t* handlePtr) {
    static int s_count = 0; ++s_count;
    
    if (s_count <= 10) {
        LOGF_WARNING("[Net] XamSessionCreateHandle #{}", s_count);
    }
    
    return SessionManager::Instance().CreateSessionHandle(handlePtr);
}

uint32_t XamSessionRefObjByHandle(uint32_t handle, uint32_t* objPtr) {
    static int s_count = 0; ++s_count;
    
    if (s_count <= 10) {
        LOGF_WARNING("[Net] XamSessionRefObjByHandle #{} handle=0x{:08X}", s_count, handle);
    }
    
    return SessionManager::Instance().RefSessionByHandle(handle, objPtr);
}

} // namespace Net
