#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace Net {

// ============================================================================
// Session Data Structures
// ============================================================================

// GTA IV Game Modes (matching original Xbox Live)
enum class GameMode : uint32_t {
    FreeMode = 0,
    Deathmatch,
    TeamDeathmatch,
    MafiyaWork,
    TeamMafiyaWork,
    CarJackCity,
    TeamCarJackCity,
    Race,
    GTARace,
    CopsNCrooks,
    TurfWar,
    DealBreaker,
    HangmansNOOSE,
    BombDaBaseII,
    PartyMode
};

// Map areas for filtering
enum class MapArea : uint32_t {
    AllOfLibertyCity = 0,
    Broker,
    Dukes,
    Bohan,
    Algonquin,
    Alderney
};

// Session information (what Xbox Live would return)
struct SessionInfo {
    std::string sessionId;          // Unique session identifier
    std::string hostPeerId;         // Host's peer ID for P2P connection
    std::string hostName;           // Host's display name
    GameMode gameMode;              // Game mode
    MapArea mapArea;                // Map area
    uint32_t maxPlayers;            // Maximum players (2-16)
    uint32_t currentPlayers;        // Current player count
    bool isPrivate;                 // Private (invite/code only) vs public
    std::string lobbyCode;          // 6-char code for private lobbies
    uint64_t createdAt;             // Unix timestamp
    uint64_t lastHeartbeat;         // Last update timestamp
};

// Session search filters (for Custom Match)
struct SessionSearchFilter {
    GameMode gameMode = GameMode::FreeMode;
    MapArea mapArea = MapArea::AllOfLibertyCity;
    bool filterByGameMode = false;
    bool filterByMapArea = false;
    bool includePrivate = false;    // Include private sessions (need code)
    bool includeFull = false;       // Include full sessions
    uint32_t maxResults = 20;
};

// ============================================================================
// Callbacks
// ============================================================================

using OnSessionCreatedCallback = std::function<void(bool success, const std::string& sessionId, const std::string& lobbyCode)>;
using OnSessionJoinedCallback = std::function<void(bool success, const std::string& hostPeerId, const std::string& error)>;
using OnSessionListCallback = std::function<void(bool success, const std::vector<SessionInfo>& sessions)>;
using OnSessionUpdatedCallback = std::function<void(const SessionInfo& session)>;
using OnPlayerJoinedCallback = std::function<void(const std::string& peerId, const std::string& playerName)>;
using OnPlayerLeftCallback = std::function<void(const std::string& peerId)>;

// ============================================================================
// ISessionTracker Interface
// ============================================================================

/**
 * ISessionTracker - Abstract interface for session tracking backends
 * 
 * Implementations:
 * - CommunitySessionTracker: Uses community-hosted REST API (default)
 * - FirebaseSessionTracker: Uses Firebase Realtime Database (self-hosted)
 * - LANSessionTracker: Uses UDP broadcast for LAN discovery
 * 
 * This interface provides Xbox Live-style session management:
 * - Create/host sessions with game mode, map, player limits
 * - Search/browse sessions with filters (Quick Match, Custom Match)
 * - Join sessions by ID or lobby code
 * - Real-time player join/leave notifications
 */
class ISessionTracker {
public:
    virtual ~ISessionTracker() = default;
    
    // =========================================================================
    // Lifecycle
    // =========================================================================
    
    /**
     * Initialize the session tracker
     * @return true on success
     */
    virtual bool Initialize() = 0;
    
    /**
     * Shutdown and cleanup
     */
    virtual void Shutdown() = 0;
    
    /**
     * Check if initialized
     */
    virtual bool IsInitialized() const = 0;
    
    /**
     * Get backend name for display
     */
    virtual const char* GetBackendName() const = 0;
    
    // =========================================================================
    // Session Hosting
    // =========================================================================
    
    /**
     * Create a new session (become host)
     * @param playerName Host's display name
     * @param gameMode Game mode
     * @param mapArea Map area
     * @param maxPlayers Maximum players (2-16)
     * @param isPrivate If true, generates lobby code for invite-only
     * @param callback Called with result
     */
    virtual void CreateSession(
        const std::string& playerName,
        GameMode gameMode,
        MapArea mapArea,
        uint32_t maxPlayers,
        bool isPrivate,
        OnSessionCreatedCallback callback) = 0;
    
    /**
     * Update session info (player count, etc.)
     * Only valid for host
     */
    virtual void UpdateSession(const SessionInfo& info) = 0;
    
    /**
     * Close/delete the session
     * Only valid for host
     */
    virtual void CloseSession() = 0;
    
    /**
     * Send heartbeat to keep session alive
     * Should be called periodically (every 30s)
     */
    virtual void SendHeartbeat() = 0;
    
    // =========================================================================
    // Session Discovery (Xbox Live-style)
    // =========================================================================
    
    /**
     * Quick Match - Find any available session
     * Returns first joinable session matching loose criteria
     * @param gameMode Preferred game mode (optional filter)
     * @param callback Called with result
     */
    virtual void QuickMatch(
        GameMode gameMode,
        OnSessionJoinedCallback callback) = 0;
    
    /**
     * Search sessions with filters (Custom Match)
     * @param filter Search criteria
     * @param callback Called with list of matching sessions
     */
    virtual void SearchSessions(
        const SessionSearchFilter& filter,
        OnSessionListCallback callback) = 0;
    
    /**
     * Join session by ID (from search results)
     * @param sessionId Session to join
     * @param playerName Player's display name
     * @param callback Called with result
     */
    virtual void JoinSession(
        const std::string& sessionId,
        const std::string& playerName,
        OnSessionJoinedCallback callback) = 0;
    
    /**
     * Join session by lobby code (for private sessions)
     * @param lobbyCode 6-character lobby code
     * @param playerName Player's display name
     * @param callback Called with result
     */
    virtual void JoinByCode(
        const std::string& lobbyCode,
        const std::string& playerName,
        OnSessionJoinedCallback callback) = 0;
    
    /**
     * Leave current session
     */
    virtual void LeaveSession() = 0;
    
    // =========================================================================
    // Session State
    // =========================================================================
    
    /**
     * Get current session info (if in a session)
     */
    virtual const SessionInfo* GetCurrentSession() const = 0;
    
    /**
     * Check if currently in a session
     */
    virtual bool IsInSession() const = 0;
    
    /**
     * Check if we are the host
     */
    virtual bool IsHost() const = 0;
    
    /**
     * Get our local peer ID
     */
    virtual const std::string& GetLocalPeerId() const = 0;
    
    // =========================================================================
    // Callbacks
    // =========================================================================
    
    virtual void SetOnPlayerJoined(OnPlayerJoinedCallback callback) = 0;
    virtual void SetOnPlayerLeft(OnPlayerLeftCallback callback) = 0;
    virtual void SetOnSessionUpdated(OnSessionUpdatedCallback callback) = 0;
    
    // =========================================================================
    // Polling
    // =========================================================================
    
    /**
     * Process network events - call regularly (every frame)
     */
    virtual void Poll() = 0;
};

// ============================================================================
// Factory
// ============================================================================

/**
 * Create session tracker based on current config
 * Reads Config::MultiplayerBackend to determine which implementation
 */
std::unique_ptr<ISessionTracker> CreateSessionTracker();

// ============================================================================
// Utility Functions
// ============================================================================

const char* GameModeToString(GameMode mode);
const char* MapAreaToString(MapArea area);
GameMode StringToGameMode(const std::string& str);
MapArea StringToMapArea(const std::string& str);

} // namespace Net
