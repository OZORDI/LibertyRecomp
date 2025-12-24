#include "p2p_manager.h"
#include "signaling_client.h"
#include <os/logger.h>
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
    // Default Firebase config - users should set their own
    firebaseProjectId_ = "";
    firebaseApiKey_ = "";
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
    
    // Initialize Firebase signaling client
    if (!firebaseProjectId_.empty()) {
        SignalingClient::Instance().Initialize(firebaseProjectId_, firebaseApiKey_);
    }
    
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
    
    // Shutdown signaling
    SignalingClient::Instance().Shutdown();
    
    // Shutdown GNS
    GameNetworkingSockets_Kill();
    
    initialized_ = false;
    LOG_INFO("[P2P] Shutdown complete");
}

// ============================================================================
// Lobby Management
// ============================================================================

void P2PManager::CreateLobby(const std::string& playerName, uint32_t maxPlayers,
                              OnLobbyCreatedCallback callback) {
    if (!initialized_.load()) {
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
    
    // Generate unique lobby code
    std::string lobbyCode = GenerateLobbyCode();
    
    // Assign ourselves the host virtual IP
    localVirtualIp_ = VIRTUAL_IP_HOST;
    
    LOGF_INFO("[P2P] Creating lobby with code: {}", lobbyCode);
    
    // Create lobby in Firebase
    SignalingClient::Instance().CreateLobby(lobbyCode, playerName, maxPlayers,
        [this, callback, lobbyCode](bool success, const std::string& lobbyId) {
            if (success) {
                currentLobby_.lobbyCode = lobbyCode;
                currentLobby_.lobbyId = lobbyId;
                currentLobby_.hostName = localPlayerName_;
                currentLobby_.maxPlayers = 16;
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
                
                LOGF_INFO("[P2P] Lobby created successfully: {} ({})", lobbyCode, lobbyId);
                if (callback) callback(true, lobbyCode);
            } else {
                lobbyState_ = P2PLobbyState::Failed;
                LOG_ERROR("[P2P] Failed to create lobby in Firebase");
                if (callback) callback(false, "");
            }
        });
}

void P2PManager::JoinLobby(const std::string& lobbyCode, const std::string& playerName,
                            OnLobbyJoinedCallback callback) {
    if (!initialized_.load()) {
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
    
    // Join lobby via Firebase
    SignalingClient::Instance().JoinLobby(lobbyCode, playerName,
        [this, callback, lobbyCode](bool success, const std::string& errorOrHostId) {
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
                hostIdentity.SetGenericString(errorOrHostId.c_str());
                
                HSteamNetConnection conn = pInterface->ConnectP2P(
                    hostIdentity, 0, 0, nullptr);
                    
                if (conn != k_HSteamNetConnection_Invalid) {
                    pInterface->SetConnectionPollGroup(conn, 
                        static_cast<HSteamNetPollGroup>(reinterpret_cast<intptr_t>(pollGroup_)));
                }
                
                LOGF_INFO("[P2P] Joined lobby successfully, connecting to host...");
                if (callback) callback(true, "");
            } else {
                lobbyState_ = P2PLobbyState::Failed;
                LOGF_ERROR("[P2P] Failed to join lobby: {}", errorOrHostId);
                if (callback) callback(false, errorOrHostId);
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
            pInterface->CloseConnection(connId, 0, "Leaving lobby", false);
        }
        peers_.clear();
        connectionToPeer_.clear();
    }
    
    // Close listen socket if host
    if (listenSocket_) {
        pInterface->CloseListenSocket(
            static_cast<HSteamListenSocket>(reinterpret_cast<intptr_t>(listenSocket_)));
        listenSocket_ = nullptr;
    }
    
    // Destroy poll group
    if (pollGroup_) {
        pInterface->DestroyPollGroup(
            static_cast<HSteamNetPollGroup>(reinterpret_cast<intptr_t>(pollGroup_)));
        pollGroup_ = nullptr;
    }
    
    // Leave Firebase lobby
    SignalingClient::Instance().LeaveLobby(currentLobby_.lobbyId);
    
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
    if (!IsInSession() || !pollGroup_) {
        return 0;
    }
    
    auto* pInterface = SteamNetworkingSockets();
    
    ISteamNetworkingMessage* pMsg = nullptr;
    int numMsgs = pInterface->ReceiveMessagesOnPollGroup(
        static_cast<HSteamNetPollGroup>(reinterpret_cast<intptr_t>(pollGroup_)),
        &pMsg, 1);
    
    if (numMsgs <= 0 || !pMsg) {
        return 0;
    }
    
    // Find the virtual IP for this connection
    {
        std::lock_guard<std::mutex> lock(peersMutex_);
        auto it = connectionToPeer_.find(pMsg->m_conn);
        if (it != connectionToPeer_.end() && outPeerId) {
            *outPeerId = it->second;
        }
    }
    
    // Copy data to buffer
    size_t copySize = std::min(static_cast<size_t>(pMsg->m_cbSize), bufferSize);
    std::memcpy(buffer, pMsg->m_pData, copySize);
    
    pMsg->Release();
    
    return static_cast<int>(copySize);
}

// ============================================================================
// Polling
// ============================================================================

void P2PManager::Poll() {
    if (!initialized_.load()) {
        return;
    }
    
    // Process signaling messages
    SignalingClient::Instance().Poll();
    
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
                static_cast<HSteamListenSocket>(reinterpret_cast<intptr_t>(listenSocket_)));
            
            if (conn == k_HSteamNetConnection_Invalid) {
                break;
            }
            
            // Add to poll group
            pInterface->SetConnectionPollGroup(conn,
                static_cast<HSteamNetPollGroup>(reinterpret_cast<intptr_t>(pollGroup_)));
            
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
            
            if (onPeerConnected_) {
                std::lock_guard<std::mutex> lock(peersMutex_);
                onPeerConnected_(peers_[peerVirtualIp]);
            }
        }
    }
}

// ============================================================================
// Configuration
// ============================================================================

void P2PManager::SetFirebaseConfig(const std::string& projectId, const std::string& apiKey) {
    firebaseProjectId_ = projectId;
    firebaseApiKey_ = apiKey;
    
    if (initialized_.load()) {
        SignalingClient::Instance().Initialize(projectId, apiKey);
    }
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
