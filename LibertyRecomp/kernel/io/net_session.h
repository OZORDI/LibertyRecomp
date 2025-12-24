#pragma once

#include <cstdint>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <os/logger.h>

/**
 * GTA IV Network Session Management
 * 
 * Following Xenia's approach, we provide simple session stubs that
 * return valid fake handles to unblock multiplayer menu access.
 * 
 * Session lifecycle:
 * 1. XamSessionCreateHandle - Create session handle
 * 2. XamSessionRefObjByHandle - Get session object by handle
 * 3. XGI operations (create, join, delete)
 * 4. CloseHandle
 */

namespace Net {

// ============================================================================
// Session Constants
// ============================================================================

// Magic values from Xenia to prevent crashes
constexpr uint32_t SESSION_HANDLE_MAGIC = 0xCAFEDEAD;
constexpr uint32_t SESSION_OBJECT_MAGIC = 0xDEADF00D;

// Session flags
enum SessionFlags : uint32_t {
    SESSION_CREATE_HOST                     = 0x00000001,
    SESSION_CREATE_USES_MATCHMAKING         = 0x00000002,
    SESSION_CREATE_USES_PEER_NETWORK        = 0x00000004,
    SESSION_CREATE_USES_STATS               = 0x00000008,
    SESSION_CREATE_INVITES_DISABLED         = 0x00000010,
    SESSION_CREATE_JOIN_VIA_PRESENCE_DISABLED = 0x00000020,
    SESSION_CREATE_JOIN_IN_PROGRESS_DISABLED = 0x00000040,
};

// Session state
enum class SessionState : uint32_t {
    None = 0,
    Creating,
    Created,
    Starting,
    InProgress,
    Ending,
    Ended,
    Deleted
};

// ============================================================================
// Session Structures
// ============================================================================

#pragma pack(push, 1)

// XSESSION_INFO - Session information (88 bytes on Xbox 360)
struct XSESSION_INFO {
    uint64_t sessionID;         // 0x00: Unique session ID
    uint32_t hostAddress;       // 0x08: Host IP address (network order)
    uint16_t hostPort;          // 0x0C: Host port (network order)
    uint8_t  reserved[6];       // 0x0E: Padding
    uint8_t  keyExchangeKey[16];// 0x14: Key exchange key
    uint8_t  sessionKey[8];     // 0x24: Session key (XNKID)
};

// XSESSION_LOCAL_DETAILS
struct XSESSION_LOCAL_DETAILS {
    uint32_t userCount;
    uint32_t userIndexMask;
    uint32_t privateSlotsFilled;
    uint32_t publicSlotsFilled;
};

#pragma pack(pop)

// ============================================================================
// Session Object
// ============================================================================

struct Session {
    uint32_t handle;
    uint32_t flags;
    uint32_t maxPublicSlots;
    uint32_t maxPrivateSlots;
    uint32_t filledPublicSlots;
    uint32_t filledPrivateSlots;
    uint64_t hostXuid;
    SessionState state;
    XSESSION_INFO info;
    
    Session() 
        : handle(0), flags(0), maxPublicSlots(0), maxPrivateSlots(0),
          filledPublicSlots(0), filledPrivateSlots(0), hostXuid(0),
          state(SessionState::None) {
        std::memset(&info, 0, sizeof(info));
    }
};

// ============================================================================
// Session Manager
// ============================================================================

class SessionManager {
public:
    static SessionManager& Instance();
    
    // Create a new session handle
    uint32_t CreateSessionHandle(uint32_t* outHandle);
    
    // Get session object by handle
    uint32_t RefSessionByHandle(uint32_t handle, uint32_t* outObj);
    
    // Close session handle
    void CloseSessionHandle(uint32_t handle);
    
    // Get session by handle (internal)
    Session* GetSession(uint32_t handle);
    
private:
    SessionManager() = default;
    
    std::mutex mutex_;
    std::unordered_map<uint32_t, Session> sessions_;
    std::atomic<uint32_t> nextHandle_{SESSION_HANDLE_MAGIC};
};

// ============================================================================
// API Functions (Called via GUEST_FUNCTION_HOOK)
// ============================================================================

// XamSessionCreateHandle - Creates a session handle
// Returns: ERROR_SUCCESS (0) on success
uint32_t XamSessionCreateHandle(uint32_t* handlePtr);

// XamSessionRefObjByHandle - Gets session object pointer by handle
// Returns: ERROR_SUCCESS (0) on success, ERROR_INVALID_HANDLE on failure
uint32_t XamSessionRefObjByHandle(uint32_t handle, uint32_t* objPtr);

} // namespace Net
