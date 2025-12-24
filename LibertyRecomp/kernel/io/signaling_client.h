#pragma once

#include <string>
#include <functional>
#include <vector>
#include <memory>
#include <atomic>
#include <mutex>
#include <queue>

namespace Net {

// Signaling message types
enum class SignalingMessageType {
    Offer,          // ICE offer from initiator
    Answer,         // ICE answer from responder
    IceCandidate,   // ICE candidate exchange
    PeerJoined,     // New peer joined lobby
    PeerLeft,       // Peer left lobby
    LobbyClose      // Host closed lobby
};

// Signaling message
struct SignalingMessage {
    SignalingMessageType type;
    std::string senderId;
    std::string targetId;      // Empty for broadcast
    std::string payload;       // JSON data
};

// Lobby info from Firebase
struct FirebaseLobbyInfo {
    std::string lobbyId;
    std::string hostId;
    std::string hostName;
    uint32_t maxPlayers;
    uint32_t currentPlayers;
    std::vector<std::string> playerIds;
};

// Callbacks
using OnLobbyCreatedCallback = std::function<void(bool success, const std::string& lobbyId)>;
using OnLobbyJoinedCallback = std::function<void(bool success, const std::string& errorOrHostId)>;
using OnSignalingMessageCallback = std::function<void(const SignalingMessage& msg)>;
using OnPeerListChangedCallback = std::function<void(const std::vector<std::string>& peerIds)>;

/**
 * SignalingClient - Firebase Realtime Database integration for WebRTC signaling
 * 
 * Uses Firebase REST API to exchange ICE candidates and manage lobbies.
 * No SDK required - just HTTP requests to the Firebase REST endpoints.
 * 
 * Firebase structure:
 *   /lobbies/{lobbyCode}/
 *     - hostId: string
 *     - hostName: string
 *     - maxPlayers: number
 *     - createdAt: timestamp
 *     - players/{peerId}/
 *         - name: string
 *         - joinedAt: timestamp
 *     - signaling/{messageId}/
 *         - type: string
 *         - from: string
 *         - to: string (optional)
 *         - payload: string
 */
class SignalingClient {
public:
    static SignalingClient& Instance();
    
    // =========================================================================
    // Initialization
    // =========================================================================
    
    /**
     * Initialize with Firebase project credentials
     * @param projectId Firebase project ID (from Firebase Console)
     * @param apiKey Firebase Web API Key (from Project Settings)
     */
    bool Initialize(const std::string& projectId, const std::string& apiKey);
    
    /**
     * Shutdown and cleanup
     */
    void Shutdown();
    
    /**
     * Check if initialized
     */
    bool IsInitialized() const { return initialized_.load(); }
    
    // =========================================================================
    // Lobby Management
    // =========================================================================
    
    /**
     * Create a new lobby
     * @param lobbyCode 6-character lobby code
     * @param hostName Host's display name
     * @param maxPlayers Maximum players (2-16)
     * @param callback Called with result
     */
    void CreateLobby(const std::string& lobbyCode, const std::string& hostName,
                     uint32_t maxPlayers, OnLobbyCreatedCallback callback);
    
    /**
     * Join an existing lobby
     * @param lobbyCode Lobby code to join
     * @param playerName Player's display name
     * @param callback Called with result (success + host ID or error message)
     */
    void JoinLobby(const std::string& lobbyCode, const std::string& playerName,
                   OnLobbyJoinedCallback callback);
    
    /**
     * Leave current lobby
     */
    void LeaveLobby(const std::string& lobbyId);
    
    /**
     * Get lobby info
     */
    void GetLobbyInfo(const std::string& lobbyCode,
                      std::function<void(bool, const FirebaseLobbyInfo&)> callback);
    
    // =========================================================================
    // Signaling
    // =========================================================================
    
    /**
     * Send a signaling message (ICE offer/answer/candidate)
     * @param lobbyId Lobby to send in
     * @param msg Message to send
     */
    void SendSignalingMessage(const std::string& lobbyId, const SignalingMessage& msg);
    
    /**
     * Set callback for incoming signaling messages
     */
    void SetOnSignalingMessage(OnSignalingMessageCallback callback) {
        onSignalingMessage_ = callback;
    }
    
    /**
     * Set callback for peer list changes
     */
    void SetOnPeerListChanged(OnPeerListChangedCallback callback) {
        onPeerListChanged_ = callback;
    }
    
    // =========================================================================
    // Polling
    // =========================================================================
    
    /**
     * Poll for new signaling messages - call regularly
     */
    void Poll();
    
    // =========================================================================
    // Identity
    // =========================================================================
    
    /**
     * Get our unique peer ID
     */
    const std::string& GetLocalPeerId() const { return localPeerId_; }

private:
    SignalingClient();
    ~SignalingClient();
    
    // Prevent copying
    SignalingClient(const SignalingClient&) = delete;
    SignalingClient& operator=(const SignalingClient&) = delete;
    
    // Internal helpers
    std::string GeneratePeerId();
    std::string BuildFirebaseUrl(const std::string& path);
    void HttpGet(const std::string& url, std::function<void(bool, const std::string&)> callback);
    void HttpPut(const std::string& url, const std::string& json,
                 std::function<void(bool, const std::string&)> callback);
    void HttpPost(const std::string& url, const std::string& json,
                  std::function<void(bool, const std::string&)> callback);
    void HttpDelete(const std::string& url, std::function<void(bool)> callback);
    void ProcessPendingRequests();
    
    // State
    std::atomic<bool> initialized_{false};
    std::string firebaseProjectId_;
    std::string firebaseApiKey_;
    std::string localPeerId_;
    std::string currentLobbyId_;
    std::string lastMessageId_;  // For polling new messages
    
    // Callbacks
    OnSignalingMessageCallback onSignalingMessage_;
    OnPeerListChangedCallback onPeerListChanged_;
    
    // Pending HTTP requests (processed in Poll)
    struct PendingRequest {
        std::string url;
        std::string method;  // GET, PUT, POST, DELETE
        std::string body;
        std::function<void(bool, const std::string&)> callback;
    };
    std::mutex requestsMutex_;
    std::queue<PendingRequest> pendingRequests_;
    
    // Polling state
    double lastPollTime_{0};
    static constexpr double POLL_INTERVAL = 0.5;  // 500ms
};

} // namespace Net
