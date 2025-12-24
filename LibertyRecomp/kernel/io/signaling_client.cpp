#include "signaling_client.h"
#include <os/logger.h>
#include <random>
#include <chrono>
#include <sstream>
#include <curl/curl.h>
#include <imgui.h>

namespace Net {

// ============================================================================
// CURL Helpers
// ============================================================================

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// ============================================================================
// Singleton Instance
// ============================================================================

SignalingClient& SignalingClient::Instance() {
    static SignalingClient instance;
    return instance;
}

SignalingClient::SignalingClient() {
    localPeerId_ = GeneratePeerId();
}

SignalingClient::~SignalingClient() {
    Shutdown();
}

// ============================================================================
// Initialization
// ============================================================================

bool SignalingClient::Initialize(const std::string& projectId, const std::string& apiKey) {
    if (projectId.empty()) {
        LOG_ERROR("[Signaling] Firebase project ID is empty. Configure in settings.");
        return false;
    }
    
    firebaseProjectId_ = projectId;
    firebaseApiKey_ = apiKey;
    
    LOGF_INFO("[Signaling] Initialized with project: {}", projectId);
    LOGF_INFO("[Signaling] Local peer ID: {}", localPeerId_);
    
    initialized_ = true;
    return true;
}

void SignalingClient::Shutdown() {
    if (!initialized_.load()) {
        return;
    }
    
    // Leave any active lobby
    if (!currentLobbyId_.empty()) {
        LeaveLobby(currentLobbyId_);
    }
    
    initialized_ = false;
    LOG_INFO("[Signaling] Shutdown complete");
}

// ============================================================================
// Lobby Management
// ============================================================================

void SignalingClient::CreateLobby(const std::string& lobbyCode, const std::string& hostName,
                                   uint32_t maxPlayers, OnLobbyCreatedCallback callback) {
    if (!initialized_.load()) {
        LOG_ERROR("[Signaling] CreateLobby failed: not initialized");
        if (callback) callback(false, "");
        return;
    }
    
    LOGF_INFO("[Signaling] Creating lobby: {}", lobbyCode);
    
    // Build lobby JSON
    std::ostringstream json;
    json << "{"
         << "\"hostId\":\"" << localPeerId_ << "\","
         << "\"hostName\":\"" << hostName << "\","
         << "\"maxPlayers\":" << maxPlayers << ","
         << "\"createdAt\":{\".sv\":\"timestamp\"},"
         << "\"players\":{"
         << "\"" << localPeerId_ << "\":{"
         << "\"name\":\"" << hostName << "\","
         << "\"joinedAt\":{\".sv\":\"timestamp\"}"
         << "}"
         << "}"
         << "}";
    
    std::string url = BuildFirebaseUrl("/lobbies/" + lobbyCode + ".json");
    
    HttpPut(url, json.str(), [this, callback, lobbyCode](bool success, const std::string& response) {
        if (success) {
            currentLobbyId_ = lobbyCode;
            LOGF_INFO("[Signaling] Lobby created: {}", lobbyCode);
            if (callback) callback(true, lobbyCode);
        } else {
            LOGF_ERROR("[Signaling] Failed to create lobby: {}", response);
            if (callback) callback(false, "");
        }
    });
}

void SignalingClient::JoinLobby(const std::string& lobbyCode, const std::string& playerName,
                                 OnLobbyJoinedCallback callback) {
    if (!initialized_.load()) {
        LOG_ERROR("[Signaling] JoinLobby failed: not initialized");
        if (callback) callback(false, "Not initialized");
        return;
    }
    
    LOGF_INFO("[Signaling] Joining lobby: {}", lobbyCode);
    
    // First, check if lobby exists and get host ID
    std::string url = BuildFirebaseUrl("/lobbies/" + lobbyCode + ".json");
    
    HttpGet(url, [this, callback, lobbyCode, playerName](bool success, const std::string& response) {
        if (!success || response == "null" || response.empty()) {
            LOGF_ERROR("[Signaling] Lobby not found: {}", lobbyCode);
            if (callback) callback(false, "Lobby not found");
            return;
        }
        
        // Parse host ID from response (simple extraction)
        std::string hostId;
        size_t hostIdPos = response.find("\"hostId\":\"");
        if (hostIdPos != std::string::npos) {
            hostIdPos += 10;  // Length of "hostId":"
            size_t endPos = response.find("\"", hostIdPos);
            if (endPos != std::string::npos) {
                hostId = response.substr(hostIdPos, endPos - hostIdPos);
            }
        }
        
        if (hostId.empty()) {
            LOG_ERROR("[Signaling] Could not parse host ID from lobby");
            if (callback) callback(false, "Invalid lobby data");
            return;
        }
        
        // Add ourselves to the players list
        std::ostringstream playerJson;
        playerJson << "{"
                   << "\"name\":\"" << playerName << "\","
                   << "\"joinedAt\":{\".sv\":\"timestamp\"}"
                   << "}";
        
        std::string playerUrl = BuildFirebaseUrl("/lobbies/" + lobbyCode + "/players/" + localPeerId_ + ".json");
        
        HttpPut(playerUrl, playerJson.str(), [this, callback, lobbyCode, hostId](bool success, const std::string& response) {
            if (success) {
                currentLobbyId_ = lobbyCode;
                LOGF_INFO("[Signaling] Joined lobby: {}, host: {}", lobbyCode, hostId);
                if (callback) callback(true, hostId);
            } else {
                LOGF_ERROR("[Signaling] Failed to join lobby: {}", response);
                if (callback) callback(false, "Failed to join");
            }
        });
    });
}

void SignalingClient::LeaveLobby(const std::string& lobbyId) {
    if (lobbyId.empty()) {
        return;
    }
    
    LOGF_INFO("[Signaling] Leaving lobby: {}", lobbyId);
    
    // Remove ourselves from the players list
    std::string url = BuildFirebaseUrl("/lobbies/" + lobbyId + "/players/" + localPeerId_ + ".json");
    
    HttpDelete(url, [this, lobbyId](bool success) {
        if (success) {
            LOGF_INFO("[Signaling] Left lobby: {}", lobbyId);
        }
    });
    
    currentLobbyId_.clear();
}

void SignalingClient::GetLobbyInfo(const std::string& lobbyCode,
                                    std::function<void(bool, const FirebaseLobbyInfo&)> callback) {
    std::string url = BuildFirebaseUrl("/lobbies/" + lobbyCode + ".json");
    
    HttpGet(url, [callback](bool success, const std::string& response) {
        FirebaseLobbyInfo info;
        
        if (!success || response == "null" || response.empty()) {
            if (callback) callback(false, info);
            return;
        }
        
        // Simple JSON parsing (no external library needed)
        auto extractString = [&response](const std::string& key) -> std::string {
            std::string search = "\"" + key + "\":\"";
            size_t pos = response.find(search);
            if (pos == std::string::npos) return "";
            pos += search.length();
            size_t endPos = response.find("\"", pos);
            if (endPos == std::string::npos) return "";
            return response.substr(pos, endPos - pos);
        };
        
        info.hostId = extractString("hostId");
        info.hostName = extractString("hostName");
        
        if (callback) callback(true, info);
    });
}

// ============================================================================
// Signaling
// ============================================================================

void SignalingClient::SendSignalingMessage(const std::string& lobbyId, const SignalingMessage& msg) {
    if (!initialized_.load() || lobbyId.empty()) {
        return;
    }
    
    std::ostringstream json;
    json << "{"
         << "\"type\":" << static_cast<int>(msg.type) << ","
         << "\"from\":\"" << localPeerId_ << "\","
         << "\"to\":\"" << msg.targetId << "\","
         << "\"payload\":\"" << msg.payload << "\","
         << "\"timestamp\":{\".sv\":\"timestamp\"}"
         << "}";
    
    std::string url = BuildFirebaseUrl("/lobbies/" + lobbyId + "/signaling.json");
    
    HttpPost(url, json.str(), [](bool success, const std::string& response) {
        if (!success) {
            LOG_ERROR("[Signaling] Failed to send signaling message");
        }
    });
}

// ============================================================================
// Polling
// ============================================================================

void SignalingClient::Poll() {
    if (!initialized_.load()) {
        return;
    }
    
    // Process any pending HTTP requests
    ProcessPendingRequests();
    
    // Rate limit polling
    double currentTime = ImGui::GetTime();
    if (currentTime - lastPollTime_ < POLL_INTERVAL) {
        return;
    }
    lastPollTime_ = currentTime;
    
    // Poll for new signaling messages if in a lobby
    if (!currentLobbyId_.empty() && onSignalingMessage_) {
        std::string url = BuildFirebaseUrl("/lobbies/" + currentLobbyId_ + "/signaling.json");
        url += "?orderBy=\"timestamp\"&limitToLast=10";
        
        HttpGet(url, [this](bool success, const std::string& response) {
            if (!success || response == "null" || response.empty()) {
                return;
            }
            
            // Parse signaling messages (simplified - just detect new messages for us)
            std::string searchFor = "\"to\":\"" + localPeerId_ + "\"";
            size_t pos = 0;
            while ((pos = response.find(searchFor, pos)) != std::string::npos) {
                // Found a message for us - parse and deliver
                SignalingMessage msg;
                msg.targetId = localPeerId_;
                
                // Extract sender
                size_t fromPos = response.rfind("\"from\":\"", pos);
                if (fromPos != std::string::npos) {
                    fromPos += 8;
                    size_t endPos = response.find("\"", fromPos);
                    if (endPos != std::string::npos) {
                        msg.senderId = response.substr(fromPos, endPos - fromPos);
                    }
                }
                
                // Deliver if valid
                if (!msg.senderId.empty() && msg.senderId != localPeerId_) {
                    if (onSignalingMessage_) {
                        onSignalingMessage_(msg);
                    }
                }
                
                pos += searchFor.length();
            }
        });
    }
}

// ============================================================================
// Internal Helpers
// ============================================================================

std::string SignalingClient::GeneratePeerId() {
    static const char chars[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, sizeof(chars) - 2);
    
    std::string id = "peer_";
    for (int i = 0; i < 16; ++i) {
        id += chars[dis(gen)];
    }
    return id;
}

std::string SignalingClient::BuildFirebaseUrl(const std::string& path) {
    std::string url = "https://" + firebaseProjectId_ + "-default-rtdb.firebaseio.com" + path;
    if (!firebaseApiKey_.empty()) {
        url += (path.find('?') != std::string::npos ? "&" : "?");
        url += "auth=" + firebaseApiKey_;
    }
    return url;
}

void SignalingClient::HttpGet(const std::string& url, 
                               std::function<void(bool, const std::string&)> callback) {
    std::lock_guard<std::mutex> lock(requestsMutex_);
    pendingRequests_.push({url, "GET", "", callback});
}

void SignalingClient::HttpPut(const std::string& url, const std::string& json,
                               std::function<void(bool, const std::string&)> callback) {
    std::lock_guard<std::mutex> lock(requestsMutex_);
    pendingRequests_.push({url, "PUT", json, callback});
}

void SignalingClient::HttpPost(const std::string& url, const std::string& json,
                                std::function<void(bool, const std::string&)> callback) {
    std::lock_guard<std::mutex> lock(requestsMutex_);
    pendingRequests_.push({url, "POST", json, callback});
}

void SignalingClient::HttpDelete(const std::string& url, std::function<void(bool)> callback) {
    std::lock_guard<std::mutex> lock(requestsMutex_);
    pendingRequests_.push({url, "DELETE", "", 
        [callback](bool success, const std::string&) { if (callback) callback(success); }});
}

void SignalingClient::ProcessPendingRequests() {
    PendingRequest req;
    
    {
        std::lock_guard<std::mutex> lock(requestsMutex_);
        if (pendingRequests_.empty()) {
            return;
        }
        req = std::move(pendingRequests_.front());
        pendingRequests_.pop();
    }
    
    // Execute request using CURL
    CURL* curl = curl_easy_init();
    if (!curl) {
        if (req.callback) req.callback(false, "CURL init failed");
        return;
    }
    
    std::string response;
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    
    curl_easy_setopt(curl, CURLOPT_URL, req.url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    
    if (req.method == "PUT") {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req.body.c_str());
    } else if (req.method == "POST") {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req.body.c_str());
    } else if (req.method == "DELETE") {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    }
    
    CURLcode res = curl_easy_perform(curl);
    
    bool success = (res == CURLE_OK);
    if (!success) {
        response = curl_easy_strerror(res);
    }
    
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    
    if (req.callback) {
        req.callback(success, response);
    }
}

} // namespace Net
