#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <functional>
#include <memory>
#include "session_tracker.h"

namespace Net {

// Forward declarations
struct P2PConnection;
struct P2PLobby;

// Connection states
enum class P2PConnectionState {
    None,
    Connecting,
    FindingRoute,
    Connected,
    ClosedByPeer,
    ProblemDetectedLocally,
    Failed
};

// Lobby states  
enum class P2PLobbyState {
    None,
    Creating,
    Active,
    Joining,
    Joined,
    Failed
};

// Peer information
struct P2PPeer {
    uint32_t peerId;           // Unique peer ID
    uint32_t virtualIp;        // Virtual IP for game (192.168.100.x)
    std::string displayName;
    P2PConnectionState state;
    int ping;                  // Latency in ms
    bool isHost;
};

// Lobby information
struct P2PLobbyInfo {
    std::string lobbyCode;     // 6-character join code
    std::string lobbyId;       // Firebase document ID
    std::string hostName;
    uint32_t maxPlayers;
    uint32_t currentPlayers;
    bool isHost;
};

// Callbacks
using OnLobbyCreatedCallback = std::function<void(bool success, const std::string& lobbyCode)>;
using OnLobbyJoinedCallback = std::function<void(bool success, const std::string& error)>;
using OnPeerConnectedCallback = std::function<void(const P2PPeer& peer)>;
using OnPeerDisconnectedCallback = std::function<void(uint32_t peerId)>;
using OnDataReceivedCallback = std::function<void(uint32_t peerId, const void* data, size_t size)>;

/**
 * P2PManager - Seamless peer-to-peer networking for online multiplayer
 * 
 * Uses GameNetworkingSockets for reliable/unreliable messaging with
 * configurable session tracking backends (Community Server, Firebase, or LAN).
 * 
 * The game's existing XNet/socket layer routes traffic through this
 * when online multiplayer is active.
 */
class P2PManager {
public:
    static P2PManager& Instance();
    
    // =========================================================================
    // Initialization
    // =========================================================================
    
    /**
     * Initialize the P2P networking system
     * Must be called before any other P2P functions
     */
    bool Initialize();
    
    /**
     * Shutdown and cleanup
     */
    void Shutdown();
    
    /**
     * Check if P2P is initialized and ready
     */
    bool IsInitialized() const { return initialized_.load(); }
    
    /**
     * Check if currently in an active P2P session
     */
    bool IsInSession() const { return lobbyState_ == P2PLobbyState::Active || 
                                       lobbyState_ == P2PLobbyState::Joined; }
    
    // =========================================================================
    // Lobby Management
    // =========================================================================
    
    /**
     * Create a new lobby and become the host
     * @param playerName Display name for this player
     * @param gameMode Game mode (for session filtering)
     * @param mapArea Map area (for session filtering)
     * @param maxPlayers Maximum players (2-16)
     * @param isPrivate If true, generates lobby code for invite-only
     * @param callback Called when lobby is created with join code
     */
    void CreateLobby(const std::string& playerName, GameMode gameMode, MapArea mapArea,
                     uint32_t maxPlayers, bool isPrivate, OnLobbyCreatedCallback callback);
    
    /**
     * Quick Match - find any available session
     * @param gameMode Preferred game mode
     * @param callback Called when joined or failed
     */
    void QuickMatch(GameMode gameMode, OnLobbyJoinedCallback callback);
    
    /**
     * Search for available sessions
     * @param filter Search criteria
     * @param callback Called with list of sessions
     */
    void SearchSessions(const SessionSearchFilter& filter,
                        std::function<void(bool, const std::vector<SessionInfo>&)> callback);
    
    /**
     * Join an existing lobby using a 6-character code
     * @param lobbyCode The join code from the host
     * @param playerName Display name for this player
     * @param callback Called when join succeeds or fails
     */
    void JoinLobby(const std::string& lobbyCode, const std::string& playerName,
                   OnLobbyJoinedCallback callback);
    
    /**
     * Leave the current lobby
     */
    void LeaveLobby();
    
    /**
     * Get current lobby info
     */
    const P2PLobbyInfo& GetLobbyInfo() const { return currentLobby_; }
    
    /**
     * Get lobby state
     */
    P2PLobbyState GetLobbyState() const { return lobbyState_; }
    
    // =========================================================================
    // Peer Management
    // =========================================================================
    
    /**
     * Get list of connected peers
     */
    std::vector<P2PPeer> GetConnectedPeers() const;
    
    /**
     * Get peer by virtual IP
     */
    const P2PPeer* GetPeerByVirtualIp(uint32_t virtualIp) const;
    
    /**
     * Get our local virtual IP
     */
    uint32_t GetLocalVirtualIp() const { return localVirtualIp_; }
    
    /**
     * Check if a virtual IP is a P2P peer
     */
    bool IsPeerAddress(uint32_t virtualIp) const;
    
    // =========================================================================
    // Data Transmission
    // =========================================================================
    
    /**
     * Send data to a specific peer (by virtual IP)
     * @param virtualIp Destination virtual IP
     * @param data Data buffer
     * @param size Data size
     * @param reliable If true, use reliable ordered delivery
     * @return Bytes sent, or -1 on error
     */
    int SendToPeer(uint32_t virtualIp, const void* data, size_t size, bool reliable = false);
    
    /**
     * Send data to all connected peers
     */
    void Broadcast(const void* data, size_t size, bool reliable = false);
    
    /**
     * Receive data from peers (non-blocking)
     * @param outPeerId Filled with sender's virtual IP
     * @param buffer Buffer to receive data
     * @param bufferSize Size of buffer
     * @return Bytes received, 0 if no data, -1 on error
     */
    int ReceiveFromPeer(uint32_t* outPeerId, void* buffer, size_t bufferSize);
    
    // =========================================================================
    // Callbacks
    // =========================================================================
    
    void SetOnPeerConnected(OnPeerConnectedCallback callback) { onPeerConnected_ = callback; }
    void SetOnPeerDisconnected(OnPeerDisconnectedCallback callback) { onPeerDisconnected_ = callback; }
    void SetOnDataReceived(OnDataReceivedCallback callback) { onDataReceived_ = callback; }
    
    // =========================================================================
    // Polling
    // =========================================================================
    
    /**
     * Process network events - call this regularly (e.g., every frame)
     */
    void Poll();
    
    // =========================================================================
    // Configuration
    // =========================================================================
    
    /**
     * Get the session tracker instance
     */
    ISessionTracker* GetSessionTracker() { return sessionTracker_.get(); }
    
    /**
     * Get STUN/TURN server list
     */
    static std::vector<std::string> GetIceServers();
    
    /**
     * Get backend name for display
     */
    const char* GetBackendName() const;

private:
    P2PManager();
    ~P2PManager();
    
    // Prevent copying
    P2PManager(const P2PManager&) = delete;
    P2PManager& operator=(const P2PManager&) = delete;
    
    // Internal helpers
    std::string GenerateLobbyCode();
    uint32_t AssignVirtualIp();
    void ProcessSignalingMessages();
    void HandlePeerConnection(uint32_t peerId);
    void HandlePeerDisconnection(uint32_t peerId);
    
    // State
    std::atomic<bool> initialized_{false};
    std::atomic<P2PLobbyState> lobbyState_{P2PLobbyState::None};
    P2PLobbyInfo currentLobby_;
    uint32_t localVirtualIp_{0};
    std::string localPlayerName_;
    
    // Peer tracking
    mutable std::mutex peersMutex_;
    std::unordered_map<uint32_t, P2PPeer> peers_;  // Key: virtual IP
    std::unordered_map<uint32_t, uint32_t> connectionToPeer_;  // GNS connection -> virtual IP
    
    // Callbacks
    OnPeerConnectedCallback onPeerConnected_;
    OnPeerDisconnectedCallback onPeerDisconnected_;
    OnDataReceivedCallback onDataReceived_;
    
    // Session tracker (configurable backend)
    std::unique_ptr<ISessionTracker> sessionTracker_;
    
    // GNS handles (uint32_t to match HSteamListenSocket/HSteamNetPollGroup types)
    uint32_t listenSocket_{0};
    uint32_t pollGroup_{0};
    
    // Virtual IP allocation
    std::atomic<uint8_t> nextVirtualIpSuffix_{2};  // Start at 192.168.100.2
};

// Virtual IP helpers
constexpr uint32_t VIRTUAL_IP_BASE = 0xC0A86400;  // 192.168.100.0 in network order (big-endian)
constexpr uint32_t VIRTUAL_IP_HOST = 0xC0A86401;  // 192.168.100.1

inline uint32_t MakeVirtualIp(uint8_t suffix) {
    return VIRTUAL_IP_BASE | suffix;
}

inline uint8_t GetVirtualIpSuffix(uint32_t virtualIp) {
    return static_cast<uint8_t>(virtualIp & 0xFF);
}

inline bool IsVirtualIp(uint32_t addr) {
    return (addr & 0xFFFFFF00) == VIRTUAL_IP_BASE;
}

} // namespace Net
