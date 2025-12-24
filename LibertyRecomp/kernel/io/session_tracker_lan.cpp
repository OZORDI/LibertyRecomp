#include "session_tracker.h"
#include <user/config.h>
#include <os/logger.h>
#include <random>
#include <chrono>
#include <sstream>
#include <mutex>
#include <thread>
#include <atomic>
#include <imgui.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

namespace Net {

// ============================================================================
// LAN Protocol Constants
// ============================================================================

static constexpr uint32_t LAN_MAGIC = 0x4C524D50; // "LRMP" - Liberty Recomp Multiplayer
static constexpr uint16_t LAN_VERSION = 1;

enum class LANMessageType : uint8_t {
    Announce = 1,      // Host announces session
    Query = 2,         // Client queries for sessions
    Response = 3,      // Host responds to query
    Join = 4,          // Client requests to join
    JoinAccept = 5,    // Host accepts join
    JoinReject = 6,    // Host rejects join
    Leave = 7,         // Player leaves
    Heartbeat = 8      // Keep-alive
};

#pragma pack(push, 1)
struct LANHeader {
    uint32_t magic;
    uint16_t version;
    LANMessageType type;
    uint8_t reserved;
};

struct LANAnnounce {
    LANHeader header;
    char sessionId[32];
    char hostPeerId[32];
    char hostName[32];
    char lobbyCode[8];
    uint8_t gameMode;
    uint8_t mapArea;
    uint8_t maxPlayers;
    uint8_t currentPlayers;
    uint8_t isPrivate;
    uint8_t padding[3];
};

struct LANQuery {
    LANHeader header;
    char requesterPeerId[32];
};

struct LANJoinRequest {
    LANHeader header;
    char sessionId[32];
    char peerId[32];
    char playerName[32];
};

struct LANJoinResponse {
    LANHeader header;
    char sessionId[32];
    char hostPeerId[32];
    uint8_t accepted;
    char reason[63];
};
#pragma pack(pop)

// ============================================================================
// LANSessionTracker Implementation
// ============================================================================

class LANSessionTracker : public ISessionTracker {
public:
    LANSessionTracker() {
        localPeerId_ = GeneratePeerId();
    }
    
    ~LANSessionTracker() override {
        Shutdown();
    }
    
    // =========================================================================
    // Lifecycle
    // =========================================================================
    
    bool Initialize() override {
        if (initialized_) return true;
        
#ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            LOG_ERROR("[LANSession] WSAStartup failed");
            return false;
        }
#endif
        
        broadcastPort_ = Config::LANBroadcastPort.Value;
        if (broadcastPort_ <= 0 || broadcastPort_ > 65535) {
            broadcastPort_ = 3074;
        }
        
        // Create UDP socket for broadcast
        socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (socket_ == INVALID_SOCKET) {
            LOG_ERROR("[LANSession] Failed to create socket");
            return false;
        }
        
        // Enable broadcast
        int broadcast = 1;
        if (setsockopt(socket_, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof(broadcast)) < 0) {
            LOG_ERROR("[LANSession] Failed to enable broadcast");
            closesocket(socket_);
            return false;
        }
        
        // Enable address reuse
        int reuse = 1;
        setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));
        
        // Bind to port
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(broadcastPort_);
        
        if (bind(socket_, (sockaddr*)&addr, sizeof(addr)) < 0) {
            LOG_ERROR("[LANSession] Failed to bind socket");
            closesocket(socket_);
            return false;
        }
        
        // Set non-blocking
#ifdef _WIN32
        u_long mode = 1;
        ioctlsocket(socket_, FIONBIO, &mode);
#else
        int flags = fcntl(socket_, F_GETFL, 0);
        fcntl(socket_, F_SETFL, flags | O_NONBLOCK);
#endif
        
        LOGF_INFO("[LANSession] Initialized on port {}", broadcastPort_);
        LOGF_INFO("[LANSession] Local peer ID: {}", localPeerId_);
        
        initialized_ = true;
        return true;
    }
    
    void Shutdown() override {
        if (!initialized_) return;
        
        if (IsInSession()) {
            LeaveSession();
        }
        
        if (socket_ != INVALID_SOCKET) {
            closesocket(socket_);
            socket_ = INVALID_SOCKET;
        }
        
#ifdef _WIN32
        WSACleanup();
#endif
        
        initialized_ = false;
        LOG_INFO("[LANSession] Shutdown complete");
    }
    
    bool IsInitialized() const override { return initialized_; }
    
    const char* GetBackendName() const override { return "LAN (Local Network)"; }
    
    // =========================================================================
    // Session Hosting
    // =========================================================================
    
    void CreateSession(
        const std::string& playerName,
        GameMode gameMode,
        MapArea mapArea,
        uint32_t maxPlayers,
        bool isPrivate,
        OnSessionCreatedCallback callback) override 
    {
        if (!initialized_) {
            if (callback) callback(false, "", "");
            return;
        }
        
        if (IsInSession()) {
            if (callback) callback(false, "", "");
            return;
        }
        
        std::string sessionId = GenerateSessionId();
        std::string lobbyCode = isPrivate ? GenerateLobbyCode() : "";
        
        currentSession_.sessionId = sessionId;
        currentSession_.hostPeerId = localPeerId_;
        currentSession_.hostName = playerName;
        currentSession_.gameMode = gameMode;
        currentSession_.mapArea = mapArea;
        currentSession_.maxPlayers = maxPlayers;
        currentSession_.currentPlayers = 1;
        currentSession_.isPrivate = isPrivate;
        currentSession_.lobbyCode = lobbyCode;
        currentSession_.createdAt = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        
        isInSession_ = true;
        isHost_ = true;
        
        // Broadcast announcement
        BroadcastAnnounce();
        
        LOGF_INFO("[LANSession] Session created: {} (code: {})", sessionId, lobbyCode);
        if (callback) callback(true, sessionId, lobbyCode);
    }
    
    void UpdateSession(const SessionInfo& info) override {
        if (!isHost_ || !isInSession_) return;
        currentSession_.currentPlayers = info.currentPlayers;
        BroadcastAnnounce();
    }
    
    void CloseSession() override {
        if (!isHost_ || !isInSession_) return;
        
        isInSession_ = false;
        isHost_ = false;
        currentSession_ = SessionInfo{};
        discoveredSessions_.clear();
        
        LOG_INFO("[LANSession] Session closed");
    }
    
    void SendHeartbeat() override {
        if (!isHost_ || !isInSession_) return;
        BroadcastAnnounce();
    }
    
    // =========================================================================
    // Session Discovery
    // =========================================================================
    
    void QuickMatch(GameMode gameMode, OnSessionJoinedCallback callback) override {
        if (!initialized_) {
            if (callback) callback(false, "", "Not initialized");
            return;
        }
        
        // Send query and wait for responses
        BroadcastQuery();
        
        // Store callback for when we receive responses
        pendingQuickMatch_ = callback;
        pendingQuickMatchMode_ = gameMode;
        quickMatchTimeout_ = ImGui::GetTime() + 2.0; // 2 second timeout
    }
    
    void SearchSessions(const SessionSearchFilter& filter, OnSessionListCallback callback) override {
        if (!initialized_) {
            if (callback) callback(false, {});
            return;
        }
        
        // Send query
        BroadcastQuery();
        
        // Store callback
        pendingSearch_ = callback;
        pendingSearchFilter_ = filter;
        searchTimeout_ = ImGui::GetTime() + 2.0;
    }
    
    void JoinSession(const std::string& sessionId, const std::string& playerName, 
                     OnSessionJoinedCallback callback) override 
    {
        if (!initialized_) {
            if (callback) callback(false, "", "Not initialized");
            return;
        }
        
        if (IsInSession()) {
            if (callback) callback(false, "", "Already in session");
            return;
        }
        
        // Find session in discovered list
        auto it = discoveredSessions_.find(sessionId);
        if (it == discoveredSessions_.end()) {
            if (callback) callback(false, "", "Session not found");
            return;
        }
        
        // Send join request
        SendJoinRequest(it->second.hostAddr, sessionId, playerName);
        
        pendingJoin_ = callback;
        pendingJoinSession_ = sessionId;
        joinTimeout_ = ImGui::GetTime() + 5.0;
    }
    
    void JoinByCode(const std::string& lobbyCode, const std::string& playerName,
                    OnSessionJoinedCallback callback) override 
    {
        // Search for session with this code
        for (const auto& [id, info] : discoveredSessions_) {
            if (info.session.lobbyCode == lobbyCode) {
                JoinSession(id, playerName, callback);
                return;
            }
        }
        
        // Not found - send query and wait
        BroadcastQuery();
        pendingJoinByCode_ = callback;
        pendingJoinCode_ = lobbyCode;
        pendingJoinName_ = playerName;
        joinByCodeTimeout_ = ImGui::GetTime() + 2.0;
    }
    
    void LeaveSession() override {
        if (!isInSession_) return;
        
        if (isHost_) {
            CloseSession();
        } else {
            // Send leave message
            // (simplified - just clear local state)
        }
        
        isInSession_ = false;
        isHost_ = false;
        currentSession_ = SessionInfo{};
        
        LOG_INFO("[LANSession] Left session");
    }
    
    // =========================================================================
    // Session State
    // =========================================================================
    
    const SessionInfo* GetCurrentSession() const override {
        return isInSession_ ? &currentSession_ : nullptr;
    }
    
    bool IsInSession() const override { return isInSession_; }
    bool IsHost() const override { return isHost_; }
    const std::string& GetLocalPeerId() const override { return localPeerId_; }
    
    // =========================================================================
    // Callbacks
    // =========================================================================
    
    void SetOnPlayerJoined(OnPlayerJoinedCallback callback) override {
        onPlayerJoined_ = callback;
    }
    
    void SetOnPlayerLeft(OnPlayerLeftCallback callback) override {
        onPlayerLeft_ = callback;
    }
    
    void SetOnSessionUpdated(OnSessionUpdatedCallback callback) override {
        onSessionUpdated_ = callback;
    }
    
    // =========================================================================
    // Polling
    // =========================================================================
    
    void Poll() override {
        if (!initialized_) return;
        
        // Receive incoming packets
        ReceivePackets();
        
        double currentTime = ImGui::GetTime();
        
        // Handle pending quick match
        if (pendingQuickMatch_ && currentTime >= quickMatchTimeout_) {
            // Find best matching session
            for (const auto& [id, info] : discoveredSessions_) {
                if (info.session.gameMode == pendingQuickMatchMode_ &&
                    info.session.currentPlayers < info.session.maxPlayers &&
                    !info.session.isPrivate) {
                    JoinSession(id, Config::PlayerName.Value, pendingQuickMatch_);
                    pendingQuickMatch_ = nullptr;
                    return;
                }
            }
            pendingQuickMatch_(false, "", "No sessions found");
            pendingQuickMatch_ = nullptr;
        }
        
        // Handle pending search
        if (pendingSearch_ && currentTime >= searchTimeout_) {
            std::vector<SessionInfo> results;
            for (const auto& [id, info] : discoveredSessions_) {
                bool include = true;
                if (pendingSearchFilter_.filterByGameMode && 
                    info.session.gameMode != pendingSearchFilter_.gameMode) include = false;
                if (pendingSearchFilter_.filterByMapArea && 
                    info.session.mapArea != pendingSearchFilter_.mapArea) include = false;
                if (!pendingSearchFilter_.includePrivate && info.session.isPrivate) include = false;
                if (!pendingSearchFilter_.includeFull && 
                    info.session.currentPlayers >= info.session.maxPlayers) include = false;
                
                if (include) results.push_back(info.session);
            }
            pendingSearch_(true, results);
            pendingSearch_ = nullptr;
        }
        
        // Handle pending join by code
        if (pendingJoinByCode_ && currentTime >= joinByCodeTimeout_) {
            for (const auto& [id, info] : discoveredSessions_) {
                if (info.session.lobbyCode == pendingJoinCode_) {
                    JoinSession(id, pendingJoinName_, pendingJoinByCode_);
                    pendingJoinByCode_ = nullptr;
                    return;
                }
            }
            pendingJoinByCode_(false, "", "Lobby not found");
            pendingJoinByCode_ = nullptr;
        }
        
        // Handle pending join timeout
        if (pendingJoin_ && currentTime >= joinTimeout_) {
            pendingJoin_(false, "", "Join timeout");
            pendingJoin_ = nullptr;
        }
        
        // Periodic announcements for hosts
        if (isHost_ && isInSession_) {
            if (currentTime - lastAnnounceTime_ >= ANNOUNCE_INTERVAL) {
                BroadcastAnnounce();
                lastAnnounceTime_ = currentTime;
            }
        }
        
        // Clean up stale sessions
        CleanupStaleSessions();
    }

private:
    // =========================================================================
    // Internal Helpers
    // =========================================================================
    
    std::string GeneratePeerId() {
        static const char chars[] = "abcdefghijklmnopqrstuvwxyz0123456789";
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, sizeof(chars) - 2);
        
        std::string id = "lan_";
        for (int i = 0; i < 12; ++i) {
            id += chars[dis(gen)];
        }
        return id;
    }
    
    std::string GenerateSessionId() {
        return GeneratePeerId();
    }
    
    std::string GenerateLobbyCode() {
        static const char chars[] = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, sizeof(chars) - 2);
        
        std::string code;
        for (int i = 0; i < 6; ++i) {
            code += chars[dis(gen)];
        }
        return code;
    }
    
    void BroadcastAnnounce() {
        if (!isHost_ || !isInSession_) return;
        
        LANAnnounce msg{};
        msg.header.magic = LAN_MAGIC;
        msg.header.version = LAN_VERSION;
        msg.header.type = LANMessageType::Announce;
        
        strncpy(msg.sessionId, currentSession_.sessionId.c_str(), sizeof(msg.sessionId) - 1);
        strncpy(msg.hostPeerId, localPeerId_.c_str(), sizeof(msg.hostPeerId) - 1);
        strncpy(msg.hostName, currentSession_.hostName.c_str(), sizeof(msg.hostName) - 1);
        strncpy(msg.lobbyCode, currentSession_.lobbyCode.c_str(), sizeof(msg.lobbyCode) - 1);
        msg.gameMode = static_cast<uint8_t>(currentSession_.gameMode);
        msg.mapArea = static_cast<uint8_t>(currentSession_.mapArea);
        msg.maxPlayers = currentSession_.maxPlayers;
        msg.currentPlayers = currentSession_.currentPlayers;
        msg.isPrivate = currentSession_.isPrivate ? 1 : 0;
        
        sockaddr_in broadcastAddr{};
        broadcastAddr.sin_family = AF_INET;
        broadcastAddr.sin_addr.s_addr = INADDR_BROADCAST;
        broadcastAddr.sin_port = htons(broadcastPort_);
        
        sendto(socket_, (char*)&msg, sizeof(msg), 0, 
               (sockaddr*)&broadcastAddr, sizeof(broadcastAddr));
    }
    
    void BroadcastQuery() {
        LANQuery msg{};
        msg.header.magic = LAN_MAGIC;
        msg.header.version = LAN_VERSION;
        msg.header.type = LANMessageType::Query;
        strncpy(msg.requesterPeerId, localPeerId_.c_str(), sizeof(msg.requesterPeerId) - 1);
        
        sockaddr_in broadcastAddr{};
        broadcastAddr.sin_family = AF_INET;
        broadcastAddr.sin_addr.s_addr = INADDR_BROADCAST;
        broadcastAddr.sin_port = htons(broadcastPort_);
        
        sendto(socket_, (char*)&msg, sizeof(msg), 0,
               (sockaddr*)&broadcastAddr, sizeof(broadcastAddr));
    }
    
    void SendJoinRequest(const sockaddr_in& hostAddr, const std::string& sessionId, 
                         const std::string& playerName) {
        LANJoinRequest msg{};
        msg.header.magic = LAN_MAGIC;
        msg.header.version = LAN_VERSION;
        msg.header.type = LANMessageType::Join;
        strncpy(msg.sessionId, sessionId.c_str(), sizeof(msg.sessionId) - 1);
        strncpy(msg.peerId, localPeerId_.c_str(), sizeof(msg.peerId) - 1);
        strncpy(msg.playerName, playerName.c_str(), sizeof(msg.playerName) - 1);
        
        sendto(socket_, (char*)&msg, sizeof(msg), 0,
               (sockaddr*)&hostAddr, sizeof(hostAddr));
    }
    
    void ReceivePackets() {
        char buffer[1024];
        sockaddr_in senderAddr{};
        socklen_t senderLen = sizeof(senderAddr);
        
        while (true) {
            int received = recvfrom(socket_, buffer, sizeof(buffer), 0,
                                    (sockaddr*)&senderAddr, &senderLen);
            if (received <= 0) break;
            
            if (received < sizeof(LANHeader)) continue;
            
            LANHeader* header = (LANHeader*)buffer;
            if (header->magic != LAN_MAGIC || header->version != LAN_VERSION) continue;
            
            switch (header->type) {
                case LANMessageType::Announce:
                    if (received >= sizeof(LANAnnounce)) {
                        HandleAnnounce((LANAnnounce*)buffer, senderAddr);
                    }
                    break;
                    
                case LANMessageType::Query:
                    if (received >= sizeof(LANQuery) && isHost_) {
                        // Respond with our session info
                        BroadcastAnnounce();
                    }
                    break;
                    
                case LANMessageType::Join:
                    if (received >= sizeof(LANJoinRequest) && isHost_) {
                        HandleJoinRequest((LANJoinRequest*)buffer, senderAddr);
                    }
                    break;
                    
                case LANMessageType::JoinAccept:
                case LANMessageType::JoinReject:
                    if (received >= sizeof(LANJoinResponse)) {
                        HandleJoinResponse((LANJoinResponse*)buffer);
                    }
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    void HandleAnnounce(LANAnnounce* msg, const sockaddr_in& senderAddr) {
        std::string sessionId(msg->sessionId);
        
        // Don't add our own session
        if (sessionId == currentSession_.sessionId) return;
        
        DiscoveredSession& discovered = discoveredSessions_[sessionId];
        discovered.session.sessionId = sessionId;
        discovered.session.hostPeerId = msg->hostPeerId;
        discovered.session.hostName = msg->hostName;
        discovered.session.lobbyCode = msg->lobbyCode;
        discovered.session.gameMode = static_cast<GameMode>(msg->gameMode);
        discovered.session.mapArea = static_cast<MapArea>(msg->mapArea);
        discovered.session.maxPlayers = msg->maxPlayers;
        discovered.session.currentPlayers = msg->currentPlayers;
        discovered.session.isPrivate = msg->isPrivate != 0;
        discovered.hostAddr = senderAddr;
        discovered.lastSeen = ImGui::GetTime();
    }
    
    void HandleJoinRequest(LANJoinRequest* msg, const sockaddr_in& senderAddr) {
        std::string sessionId(msg->sessionId);
        std::string peerId(msg->peerId);
        std::string playerName(msg->playerName);
        
        if (sessionId != currentSession_.sessionId) return;
        
        LANJoinResponse response{};
        response.header.magic = LAN_MAGIC;
        response.header.version = LAN_VERSION;
        strncpy(response.sessionId, sessionId.c_str(), sizeof(response.sessionId) - 1);
        strncpy(response.hostPeerId, localPeerId_.c_str(), sizeof(response.hostPeerId) - 1);
        
        if (currentSession_.currentPlayers >= currentSession_.maxPlayers) {
            response.header.type = LANMessageType::JoinReject;
            response.accepted = 0;
            strncpy(response.reason, "Session full", sizeof(response.reason) - 1);
        } else {
            response.header.type = LANMessageType::JoinAccept;
            response.accepted = 1;
            currentSession_.currentPlayers++;
            
            if (onPlayerJoined_) {
                onPlayerJoined_(peerId, playerName);
            }
        }
        
        sendto(socket_, (char*)&response, sizeof(response), 0,
               (sockaddr*)&senderAddr, sizeof(senderAddr));
    }
    
    void HandleJoinResponse(LANJoinResponse* msg) {
        std::string sessionId(msg->sessionId);
        
        if (sessionId != pendingJoinSession_) return;
        
        if (msg->accepted && pendingJoin_) {
            currentSession_.sessionId = sessionId;
            currentSession_.hostPeerId = msg->hostPeerId;
            isInSession_ = true;
            isHost_ = false;
            
            pendingJoin_(true, msg->hostPeerId, "");
        } else if (pendingJoin_) {
            pendingJoin_(false, "", msg->reason);
        }
        
        pendingJoin_ = nullptr;
    }
    
    void CleanupStaleSessions() {
        double currentTime = ImGui::GetTime();
        
        for (auto it = discoveredSessions_.begin(); it != discoveredSessions_.end();) {
            if (currentTime - it->second.lastSeen > SESSION_TIMEOUT) {
                it = discoveredSessions_.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    // =========================================================================
    // State
    // =========================================================================
    
    struct DiscoveredSession {
        SessionInfo session;
        sockaddr_in hostAddr;
        double lastSeen;
    };
    
    bool initialized_ = false;
    bool isInSession_ = false;
    bool isHost_ = false;
    SOCKET socket_ = INVALID_SOCKET;
    int broadcastPort_ = 3074;
    std::string localPeerId_;
    SessionInfo currentSession_;
    
    std::unordered_map<std::string, DiscoveredSession> discoveredSessions_;
    
    // Callbacks
    OnPlayerJoinedCallback onPlayerJoined_;
    OnPlayerLeftCallback onPlayerLeft_;
    OnSessionUpdatedCallback onSessionUpdated_;
    
    // Pending operations
    OnSessionJoinedCallback pendingQuickMatch_;
    GameMode pendingQuickMatchMode_;
    double quickMatchTimeout_ = 0;
    
    OnSessionListCallback pendingSearch_;
    SessionSearchFilter pendingSearchFilter_;
    double searchTimeout_ = 0;
    
    OnSessionJoinedCallback pendingJoin_;
    std::string pendingJoinSession_;
    double joinTimeout_ = 0;
    
    OnSessionJoinedCallback pendingJoinByCode_;
    std::string pendingJoinCode_;
    std::string pendingJoinName_;
    double joinByCodeTimeout_ = 0;
    
    double lastAnnounceTime_ = 0;
    
    static constexpr double ANNOUNCE_INTERVAL = 5.0;
    static constexpr double SESSION_TIMEOUT = 15.0;
};

// ============================================================================
// Factory Function
// ============================================================================

std::unique_ptr<ISessionTracker> CreateLANSessionTracker() {
    return std::make_unique<LANSessionTracker>();
}

} // namespace Net
