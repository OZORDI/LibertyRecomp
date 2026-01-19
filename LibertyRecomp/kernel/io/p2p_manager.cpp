#include "p2p_manager.h"
#include "session_tracker.h"
#include "voice_chat.h"
#include <os/logger.h>
#include <user/config.h>
#include <random>
#include <algorithm>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

// GameNetworkingSockets includes
#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

namespace Net {

// ============================================================================
// Singleton Instance
// ============================================================================

P2PManager& P2PManager::Instance() {
    static P2PManager instance;
    return instance;
}

P2PManager::P2PManager() {
    // Session tracker will be created on Initialize()
}

P2PManager::~P2PManager() {
    Shutdown();
}

// ============================================================================
// Initialization
// ============================================================================

bool P2PManager::Initialize() {
    if (initialized_.load()) {
        return true;
    }
    
    LOG_INFO("[P2P] Initializing GameNetworkingSockets...");
    
    // Initialize GNS
    SteamDatagramErrMsg errMsg;
    if (!GameNetworkingSockets_Init(nullptr, errMsg)) {
        LOGF_ERROR("[P2P] Failed to initialize GNS: {}", errMsg);
        return false;
    }
    
    // Configure ICE servers (STUN + free TURN)
    SteamNetworkingUtils()->SetGlobalConfigValueString(
        k_ESteamNetworkingConfig_P2P_STUN_ServerList,
        "stun:stun.l.google.com:19302,stun:stun1.l.google.com:19302,stun:stun.cloudflare.com:3478"
    );
    
    // Free public TURN servers (OpenRelay)
    // These are provided by metered.ca for testing - limited bandwidth but works
    SteamNetworkingUtils()->SetGlobalConfigValueString(
        k_ESteamNetworkingConfig_P2P_TURN_ServerList,
        "turn:openrelay.metered.ca:80?transport=udp,turn:openrelay.metered.ca:443?transport=tcp"
    );
    SteamNetworkingUtils()->SetGlobalConfigValueString(
        k_ESteamNetworkingConfig_P2P_TURN_UserList,
        "openrelayproject"
    );
    SteamNetworkingUtils()->SetGlobalConfigValueString(
        k_ESteamNetworkingConfig_P2P_TURN_PassList,
        "openrelayproject"
    );
    
    // Create session tracker based on config
    sessionTracker_ = CreateSessionTracker();
    if (!sessionTracker_) {
        LOG_ERROR("[P2P] Failed to create session tracker");
        GameNetworkingSockets_Kill();
        return false;
    }
    
    if (!sessionTracker_->Initialize()) {
        LOG_ERROR("[P2P] Failed to initialize session tracker");
        sessionTracker_.reset();
        GameNetworkingSockets_Kill();
        return false;
    }
    
    LOGF_INFO("[P2P] Using session tracker backend: {}", sessionTracker_->GetBackendName());
    
    initialized_ = true;
    LOG_INFO("[P2P] Initialization complete");
    
    return true;
}

void P2PManager::Shutdown() {
    if (!initialized_.load()) {
        return;
    }
    
    LOG_INFO("[P2P] Shutting down...");
    
    // Leave any active lobby
    if (lobbyState_ != P2PLobbyState::None) {
        LeaveLobby();
    }
    
    // Shutdown session tracker
    if (sessionTracker_) {
        sessionTracker_->Shutdown();
        sessionTracker_.reset();
    }
    
    // Shutdown GNS
    GameNetworkingSockets_Kill();
    
    initialized_ = false;
    LOG_INFO("[P2P] Shutdown complete");
}

// ============================================================================
// Lobby Management
// ============================================================================

void P2PManager::CreateLobby(const std::string& playerName, GameMode gameMode, MapArea mapArea,
                              uint32_t maxPlayers, bool isPrivate, OnLobbyCreatedCallback callback) {
    if (!initialized_.load() || !sessionTracker_) {
        LOG_ERROR("[P2P] CreateLobby failed: not initialized");
        if (callback) callback(false, "");
        return;
    }
    
    if (lobbyState_ != P2PLobbyState::None) {
        LOG_ERROR("[P2P] CreateLobby failed: already in a lobby");
        if (callback) callback(false, "");
        return;
    }
    
    lobbyState_ = P2PLobbyState::Creating;
    localPlayerName_ = playerName;
    
    // Assign ourselves the host virtual IP
    localVirtualIp_ = VIRTUAL_IP_HOST;
    
    LOGF_INFO("[P2P] Creating lobby (mode: {}, area: {})", 
              GameModeToString(gameMode), MapAreaToString(mapArea));
    
    // Create session via session tracker
    sessionTracker_->CreateSession(playerName, gameMode, mapArea, maxPlayers, isPrivate,
        [this, callback, maxPlayers](bool success, const std::string& sessionId, const std::string& lobbyCode) {
            if (success) {
                currentLobby_.lobbyCode = lobbyCode;
                currentLobby_.lobbyId = sessionId;
                currentLobby_.hostName = localPlayerName_;
                currentLobby_.maxPlayers = maxPlayers;
                currentLobby_.currentPlayers = 1;
                currentLobby_.isHost = true;
                
                lobbyState_ = P2PLobbyState::Active;
                
                // Start listening for peer connections
                auto* pInterface = SteamNetworkingSockets();
                SteamNetworkingIPAddr listenAddr;
                listenAddr.Clear();
                listenAddr.m_port = 0;  // Any port
                
                SteamNetworkingConfigValue_t opt;
                opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
                          (void*)nullptr);  // We poll instead
                
                listenSocket_ = pInterface->CreateListenSocketP2P(0, 0, nullptr);
                pollGroup_ = pInterface->CreatePollGroup();
                
                LOGF_INFO("[P2P] Lobby created successfully: {} (code: {})", sessionId, lobbyCode);
                if (callback) callback(true, lobbyCode);
            } else {
                lobbyState_ = P2PLobbyState::Failed;
                LOG_ERROR("[P2P] Failed to create lobby");
                if (callback) callback(false, "");
            }
        });
}

void P2PManager::QuickMatch(GameMode gameMode, OnLobbyJoinedCallback callback) {
    if (!initialized_.load() || !sessionTracker_) {
        if (callback) callback(false, "Not initialized");
        return;
    }
    
    if (lobbyState_ != P2PLobbyState::None) {
        if (callback) callback(false, "Already in a lobby");
        return;
    }
    
    lobbyState_ = P2PLobbyState::Joining;
    localPlayerName_ = Config::PlayerName.Value;
    
    LOGF_INFO("[P2P] Quick match for mode: {}", GameModeToString(gameMode));
    
    sessionTracker_->QuickMatch(gameMode,
        [this, callback](bool success, const std::string& hostPeerId, const std::string& error) {
            if (success) {
                localVirtualIp_ = AssignVirtualIp();
                currentLobby_.isHost = false;
                lobbyState_ = P2PLobbyState::Joined;
                
                // Create poll group and connect to host
                auto* pInterface = SteamNetworkingSockets();
                pollGroup_ = pInterface->CreatePollGroup();
                
                SteamNetworkingIdentity hostIdentity;
                hostIdentity.SetGenericString(hostPeerId.c_str());
                
                HSteamNetConnection conn = pInterface->ConnectP2P(hostIdentity, 0, 0, nullptr);
                if (conn != k_HSteamNetConnection_Invalid) {
                    pInterface->SetConnectionPollGroup(conn,
                        static_cast<HSteamNetPollGroup>(pollGroup_));
                }
                
                LOG_INFO("[P2P] Quick match successful, connecting to host...");
                if (callback) callback(true, "");
            } else {
                lobbyState_ = P2PLobbyState::Failed;
                LOGF_ERROR("[P2P] Quick match failed: {}", error);
                if (callback) callback(false, error);
            }
        });
}

void P2PManager::SearchSessions(const SessionSearchFilter& filter,
                                 std::function<void(bool, const std::vector<SessionInfo>&)> callback) {
    if (!initialized_.load() || !sessionTracker_) {
        if (callback) callback(false, {});
        return;
    }
    
    sessionTracker_->SearchSessions(filter, callback);
}

void P2PManager::JoinLobby(const std::string& lobbyCode, const std::string& playerName,
                            OnLobbyJoinedCallback callback) {
    if (!initialized_.load() || !sessionTracker_) {
        LOG_ERROR("[P2P] JoinLobby failed: not initialized");
        if (callback) callback(false, "Not initialized");
        return;
    }
    
    if (lobbyState_ != P2PLobbyState::None) {
        LOG_ERROR("[P2P] JoinLobby failed: already in a lobby");
        if (callback) callback(false, "Already in a lobby");
        return;
    }
    
    lobbyState_ = P2PLobbyState::Joining;
    localPlayerName_ = playerName;
    
    LOGF_INFO("[P2P] Joining lobby: {}", lobbyCode);
    
    // Join lobby via session tracker
    sessionTracker_->JoinByCode(lobbyCode, playerName,
        [this, callback, lobbyCode](bool success, const std::string& hostPeerId, const std::string& error) {
            if (success) {
                // Assign ourselves a virtual IP
                localVirtualIp_ = AssignVirtualIp();
                
                currentLobby_.lobbyCode = lobbyCode;
                currentLobby_.isHost = false;
                
                lobbyState_ = P2PLobbyState::Joined;
                
                // Create poll group for receiving
                auto* pInterface = SteamNetworkingSockets();
                pollGroup_ = pInterface->CreatePollGroup();
                
                // Connect to the host via signaling
                SteamNetworkingIdentity hostIdentity;
                hostIdentity.SetGenericString(hostPeerId.c_str());
                
                HSteamNetConnection conn = pInterface->ConnectP2P(
                    hostIdentity, 0, 0, nullptr);
                    
                if (conn != k_HSteamNetConnection_Invalid) {
                    pInterface->SetConnectionPollGroup(conn, 
                        static_cast<HSteamNetPollGroup>(pollGroup_));
                }
                
                LOG_INFO("[P2P] Joined lobby successfully, connecting to host...");
                if (callback) callback(true, "");
            } else {
                lobbyState_ = P2PLobbyState::Failed;
                LOGF_ERROR("[P2P] Failed to join lobby: {}", error);
                if (callback) callback(false, error);
            }
        });
}

void P2PManager::LeaveLobby() {
    if (lobbyState_ == P2PLobbyState::None) {
        return;
    }
    
    LOG_INFO("[P2P] Leaving lobby...");
    
    auto* pInterface = SteamNetworkingSockets();
    
    // Close all peer connections
    {
        std::lock_guard<std::mutex> lock(peersMutex_);
        for (auto& [connId, virtualIp] : connectionToPeer_) {
            // Notify voice chat of peer disconnection
            VoiceChatManager::Instance().OnPeerDisconnected(virtualIp);
            pInterface->CloseConnection(connId, 0, "Leaving lobby", false);
        }
        peers_.clear();
        connectionToPeer_.clear();
    }
    
    // Close listen socket if host
    if (listenSocket_) {
        pInterface->CloseListenSocket(
            static_cast<HSteamListenSocket>(listenSocket_));
        listenSocket_ = 0;
    }
    
    // Destroy poll group
    if (pollGroup_) {
        pInterface->DestroyPollGroup(
            static_cast<HSteamNetPollGroup>(pollGroup_));
        pollGroup_ = 0;
    }
    
    // Leave session via tracker
    if (sessionTracker_) {
        sessionTracker_->LeaveSession();
    }
    
    // Reset state
    currentLobby_ = P2PLobbyInfo{};
    localVirtualIp_ = 0;
    lobbyState_ = P2PLobbyState::None;
    nextVirtualIpSuffix_ = 2;
    
    LOG_INFO("[P2P] Left lobby");
}

// ============================================================================
// Peer Management
// ============================================================================

std::vector<P2PPeer> P2PManager::GetConnectedPeers() const {
    std::lock_guard<std::mutex> lock(peersMutex_);
    std::vector<P2PPeer> result;
    result.reserve(peers_.size());
    for (const auto& [ip, peer] : peers_) {
        result.push_back(peer);
    }
    return result;
}

const P2PPeer* P2PManager::GetPeerByVirtualIp(uint32_t virtualIp) const {
    std::lock_guard<std::mutex> lock(peersMutex_);
    auto it = peers_.find(virtualIp);
    if (it != peers_.end()) {
        return &it->second;
    }
    return nullptr;
}

bool P2PManager::IsPeerAddress(uint32_t virtualIp) const {
    if (!IsVirtualIp(virtualIp)) {
        return false;
    }
    if (virtualIp == localVirtualIp_) {
        return false;  // That's us
    }
    std::lock_guard<std::mutex> lock(peersMutex_);
    return peers_.find(virtualIp) != peers_.end();
}

// ============================================================================
// Data Transmission
// ============================================================================

int P2PManager::SendToPeer(uint32_t virtualIp, const void* data, size_t size, bool reliable) {
    if (!IsInSession()) {
        return -1;
    }
    
    std::lock_guard<std::mutex> lock(peersMutex_);
    
    // Find the connection for this virtual IP
    HSteamNetConnection conn = k_HSteamNetConnection_Invalid;
    for (const auto& [connId, peerIp] : connectionToPeer_) {
        if (peerIp == virtualIp) {
            conn = connId;
            break;
        }
    }
    
    if (conn == k_HSteamNetConnection_Invalid) {
        return -1;
    }
    
    auto* pInterface = SteamNetworkingSockets();
    int flags = reliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable;
    
    EResult result = pInterface->SendMessageToConnection(
        conn, data, static_cast<uint32_t>(size), flags, nullptr);
    
    return (result == k_EResultOK) ? static_cast<int>(size) : -1;
}

void P2PManager::Broadcast(const void* data, size_t size, bool reliable) {
    if (!IsInSession()) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(peersMutex_);
    
    auto* pInterface = SteamNetworkingSockets();
    int flags = reliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable;
    
    for (const auto& [connId, peerIp] : connectionToPeer_) {
        pInterface->SendMessageToConnection(
            connId, data, static_cast<uint32_t>(size), flags, nullptr);
    }
}

int P2PManager::ReceiveFromPeer(uint32_t* outPeerId, void* buffer, size_t bufferSize) {
    if (!IsInSession()) {
        return 0;
    }
    
    // Get packet from game packet queue (populated by ProcessIncomingMessages)
    std::lock_guard<std::mutex> lock(gamePacketsMutex_);
    
    if (gamePackets_.empty()) {
        return 0;
    }
    
    auto& packet = gamePackets_.front();
    
    if (outPeerId) {
        *outPeerId = packet.peerId;
    }
    
    size_t copySize = std::min(packet.data.size(), bufferSize);
    std::memcpy(buffer, packet.data.data(), copySize);
    
    gamePackets_.pop();
    
    return static_cast<int>(copySize);
}

// ============================================================================
// Polling
// ============================================================================

void P2PManager::Poll() {
    if (!initialized_.load()) {
        return;
    }
    
    // Process session tracker events
    if (sessionTracker_) {
        sessionTracker_->Poll();
    }
    
    // Process GNS callbacks
    auto* pInterface = SteamNetworkingSockets();
    if (!pInterface) {
        return;
    }
    
    pInterface->RunCallbacks();
    
    // Check connection status changes
    if (listenSocket_) {
        // Accept new connections (host mode)
        while (true) {
            SteamNetworkingIPAddr clientAddr;
            HSteamNetConnection conn = pInterface->AcceptConnection(
                static_cast<HSteamListenSocket>(listenSocket_));
            
            if (conn == k_HSteamNetConnection_Invalid) {
                break;
            }
            
            // Add to poll group
            pInterface->SetConnectionPollGroup(conn,
                static_cast<HSteamNetPollGroup>(pollGroup_));
            
            // Assign virtual IP to new peer
            uint32_t peerVirtualIp = AssignVirtualIp();
            
            {
                std::lock_guard<std::mutex> lock(peersMutex_);
                connectionToPeer_[conn] = peerVirtualIp;
                
                P2PPeer peer;
                peer.peerId = conn;
                peer.virtualIp = peerVirtualIp;
                peer.state = P2PConnectionState::Connected;
                peer.ping = 0;
                peer.isHost = false;
                peers_[peerVirtualIp] = peer;
            }
            
            LOGF_INFO("[P2P] New peer connected, assigned IP: 192.168.100.{}", 
                     GetVirtualIpSuffix(peerVirtualIp));
            
            // Notify voice chat of new peer
            VoiceChatManager::Instance().OnPeerConnected(peerVirtualIp, "Player");
            
            if (onPeerConnected_) {
                std::lock_guard<std::mutex> lock(peersMutex_);
                onPeerConnected_(peers_[peerVirtualIp]);
            }
        }
    }
    
    // Process incoming messages and route voice packets to VoiceChatManager
    ProcessIncomingMessages();
    
    // Poll voice chat
    VoiceChatManager::Instance().Poll();
}

void P2PManager::ProcessIncomingMessages() {
    if (!IsInSession() || !pollGroup_) {
        return;
    }
    
    auto* pInterface = SteamNetworkingSockets();
    
    // Process all pending messages
    ISteamNetworkingMessage* messages[32];
    int numMsgs = pInterface->ReceiveMessagesOnPollGroup(
        static_cast<HSteamNetPollGroup>(pollGroup_),
        messages, 32);
    
    for (int i = 0; i < numMsgs; ++i) {
        ISteamNetworkingMessage* pMsg = messages[i];
        if (!pMsg) continue;
        
        // Find the virtual IP for this connection
        uint32_t peerVirtualIp = 0;
        {
            std::lock_guard<std::mutex> lock(peersMutex_);
            auto it = connectionToPeer_.find(pMsg->m_conn);
            if (it != connectionToPeer_.end()) {
                peerVirtualIp = it->second;
            }
        }
        
        // Check if this is a voice packet
        if (IsVoicePacket(pMsg->m_pData, pMsg->m_cbSize)) {
            // Route to voice chat manager
            VoiceChatManager::Instance().OnVoiceDataReceived(
                peerVirtualIp, pMsg->m_pData, pMsg->m_cbSize);
        } else {
            // Queue for game's socket layer
            QueueGamePacket(peerVirtualIp, pMsg->m_pData, pMsg->m_cbSize);
        }
        
        pMsg->Release();
    }
}

void P2PManager::QueueGamePacket(uint32_t peerId, const void* data, size_t size) {
    std::lock_guard<std::mutex> lock(gamePacketsMutex_);
    
    GamePacket pkt;
    pkt.peerId = peerId;
    pkt.data.resize(size);
    std::memcpy(pkt.data.data(), data, size);
    
    gamePackets_.push(std::move(pkt));
    
    // Limit queue size to prevent memory growth
    while (gamePackets_.size() > 1000) {
        gamePackets_.pop();
    }
}

// ============================================================================
// Configuration
// ============================================================================

const char* P2PManager::GetBackendName() const {
    if (sessionTracker_) {
        return sessionTracker_->GetBackendName();
    }
    return "Not initialized";
}

std::vector<std::string> P2PManager::GetIceServers() {
    return {
        "stun:stun.l.google.com:19302",
        "stun:stun1.l.google.com:19302",
        "stun:stun.cloudflare.com:3478",
        "turn:openrelay.metered.ca:80",
        "turn:openrelay.metered.ca:443"
    };
}

// ============================================================================
// Internal Helpers
// ============================================================================

std::string P2PManager::GenerateLobbyCode() {
    static const char chars[] = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";  // No I, O, 0, 1
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, sizeof(chars) - 2);
    
    std::string code;
    code.reserve(6);
    for (int i = 0; i < 6; ++i) {
        code += chars[dis(gen)];
    }
    return code;
}

uint32_t P2PManager::AssignVirtualIp() {
    uint8_t suffix = nextVirtualIpSuffix_.fetch_add(1);
    if (suffix > 254) {
        suffix = 2;  // Wrap around (shouldn't happen with 16 player max)
        nextVirtualIpSuffix_ = 3;
    }
    return MakeVirtualIp(suffix);
}

} // namespace Net
