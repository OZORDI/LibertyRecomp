#include "session_tracker.h"
#include <user/config.h>
#include <os/logger.h>
#include <curl/curl.h>
#include <random>
#include <chrono>
#include <sstream>
#include <mutex>
#include <queue>
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
// CommunitySessionTracker Implementation
// ============================================================================

class CommunitySessionTracker : public ISessionTracker {
public:
    CommunitySessionTracker() {
        localPeerId_ = GeneratePeerId();
    }
    
    ~CommunitySessionTracker() override {
        Shutdown();
    }
    
    // =========================================================================
    // Lifecycle
    // =========================================================================
    
    bool Initialize() override {
        if (initialized_) return true;
        
        serverUrl_ = Config::CommunityServerURL.Value;
        if (serverUrl_.empty()) {
            serverUrl_ = "https://liberty-sessions.libertyrecomp.com";
        }
        
        // Remove trailing slash if present
        if (!serverUrl_.empty() && serverUrl_.back() == '/') {
            serverUrl_.pop_back();
        }
        
        LOGF_INFO("[CommunitySession] Initialized with server: {}", serverUrl_);
        LOGF_INFO("[CommunitySession] Local peer ID: {}", localPeerId_);
        
        initialized_ = true;
        return true;
    }
    
    void Shutdown() override {
        if (!initialized_) return;
        
        if (IsInSession()) {
            LeaveSession();
        }
        
        initialized_ = false;
        LOG_INFO("[CommunitySession] Shutdown complete");
    }
    
    bool IsInitialized() const override { return initialized_; }
    
    const char* GetBackendName() const override { return "Community Server"; }
    
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
            LOG_ERROR("[CommunitySession] CreateSession failed: not initialized");
            if (callback) callback(false, "", "");
            return;
        }
        
        if (IsInSession()) {
            LOG_ERROR("[CommunitySession] CreateSession failed: already in session");
            if (callback) callback(false, "", "");
            return;
        }
        
        std::string lobbyCode = isPrivate ? GenerateLobbyCode() : "";
        
        // Build JSON payload
        std::ostringstream json;
        json << "{"
             << "\"hostPeerId\":\"" << localPeerId_ << "\","
             << "\"hostName\":\"" << EscapeJson(playerName) << "\","
             << "\"gameMode\":" << static_cast<int>(gameMode) << ","
             << "\"mapArea\":" << static_cast<int>(mapArea) << ","
             << "\"maxPlayers\":" << maxPlayers << ","
             << "\"currentPlayers\":1,"
             << "\"isPrivate\":" << (isPrivate ? "true" : "false") << ","
             << "\"lobbyCode\":\"" << lobbyCode << "\""
             << "}";
        
        std::string url = serverUrl_ + "/api/sessions";
        
        HttpPost(url, json.str(), [this, callback, playerName, gameMode, mapArea, maxPlayers, isPrivate, lobbyCode]
            (bool success, const std::string& response) 
        {
            if (success) {
                // Parse session ID from response
                std::string sessionId = ExtractJsonString(response, "sessionId");
                if (sessionId.empty()) {
                    sessionId = ExtractJsonString(response, "id");
                }
                
                if (!sessionId.empty()) {
                    // Store current session
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
                    
                    LOGF_INFO("[CommunitySession] Session created: {} (code: {})", sessionId, lobbyCode);
                    if (callback) callback(true, sessionId, lobbyCode);
                } else {
                    LOG_ERROR("[CommunitySession] Failed to parse session ID from response");
                    if (callback) callback(false, "", "");
                }
            } else {
                LOGF_ERROR("[CommunitySession] Failed to create session: {}", response);
                if (callback) callback(false, "", "");
            }
        });
    }
    
    void UpdateSession(const SessionInfo& info) override {
        if (!isHost_ || !isInSession_) return;
        
        std::ostringstream json;
        json << "{"
             << "\"currentPlayers\":" << info.currentPlayers
             << "}";
        
        std::string url = serverUrl_ + "/api/sessions/" + currentSession_.sessionId;
        
        HttpPut(url, json.str(), [](bool success, const std::string& response) {
            if (!success) {
                LOG_WARNING("[CommunitySession] Failed to update session");
            }
        });
        
        currentSession_.currentPlayers = info.currentPlayers;
    }
    
    void CloseSession() override {
        if (!isHost_ || !isInSession_) return;
        
        std::string url = serverUrl_ + "/api/sessions/" + currentSession_.sessionId;
        
        HttpDelete(url, [this](bool success) {
            if (success) {
                LOG_INFO("[CommunitySession] Session closed");
            }
        });
        
        isInSession_ = false;
        isHost_ = false;
        currentSession_ = SessionInfo{};
    }
    
    void SendHeartbeat() override {
        if (!isHost_ || !isInSession_) return;
        
        std::string url = serverUrl_ + "/api/sessions/" + currentSession_.sessionId + "/heartbeat";
        
        HttpPost(url, "{}", [](bool success, const std::string&) {
            if (!success) {
                LOG_WARNING("[CommunitySession] Heartbeat failed");
            }
        });
    }
    
    // =========================================================================
    // Session Discovery
    // =========================================================================
    
    void QuickMatch(GameMode gameMode, OnSessionJoinedCallback callback) override {
        if (!initialized_) {
            if (callback) callback(false, "", "Not initialized");
            return;
        }
        
        // Search for any joinable session with this game mode
        SessionSearchFilter filter;
        filter.gameMode = gameMode;
        filter.filterByGameMode = true;
        filter.includeFull = false;
        filter.includePrivate = false;
        filter.maxResults = 1;
        
        SearchSessions(filter, [this, callback](bool success, const std::vector<SessionInfo>& sessions) {
            if (success && !sessions.empty()) {
                // Join the first available session
                const auto& session = sessions[0];
                JoinSession(session.sessionId, Config::PlayerName.Value, callback);
            } else {
                if (callback) callback(false, "", "No sessions found");
            }
        });
    }
    
    void SearchSessions(const SessionSearchFilter& filter, OnSessionListCallback callback) override {
        if (!initialized_) {
            if (callback) callback(false, {});
            return;
        }
        
        // Build query string
        std::ostringstream url;
        url << serverUrl_ << "/api/sessions?";
        url << "limit=" << filter.maxResults;
        
        if (filter.filterByGameMode) {
            url << "&gameMode=" << static_cast<int>(filter.gameMode);
        }
        if (filter.filterByMapArea) {
            url << "&mapArea=" << static_cast<int>(filter.mapArea);
        }
        if (!filter.includeFull) {
            url << "&notFull=true";
        }
        if (!filter.includePrivate) {
            url << "&public=true";
        }
        
        HttpGet(url.str(), [callback](bool success, const std::string& response) {
            std::vector<SessionInfo> sessions;
            
            if (success) {
                // Parse JSON array of sessions
                // Simple parsing - look for session objects
                size_t pos = 0;
                while ((pos = response.find("\"sessionId\"", pos)) != std::string::npos) {
                    SessionInfo session;
                    
                    // Find the start of this object
                    size_t objStart = response.rfind('{', pos);
                    size_t objEnd = response.find('}', pos);
                    
                    if (objStart != std::string::npos && objEnd != std::string::npos) {
                        std::string obj = response.substr(objStart, objEnd - objStart + 1);
                        
                        session.sessionId = ExtractJsonString(obj, "sessionId");
                        if (session.sessionId.empty()) {
                            session.sessionId = ExtractJsonString(obj, "id");
                        }
                        session.hostPeerId = ExtractJsonString(obj, "hostPeerId");
                        session.hostName = ExtractJsonString(obj, "hostName");
                        session.lobbyCode = ExtractJsonString(obj, "lobbyCode");
                        session.gameMode = static_cast<GameMode>(ExtractJsonInt(obj, "gameMode"));
                        session.mapArea = static_cast<MapArea>(ExtractJsonInt(obj, "mapArea"));
                        session.maxPlayers = ExtractJsonInt(obj, "maxPlayers");
                        session.currentPlayers = ExtractJsonInt(obj, "currentPlayers");
                        session.isPrivate = ExtractJsonBool(obj, "isPrivate");
                        
                        if (!session.sessionId.empty()) {
                            sessions.push_back(session);
                        }
                    }
                    
                    pos = objEnd + 1;
                }
            }
            
            if (callback) callback(success, sessions);
        });
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
        
        std::ostringstream json;
        json << "{"
             << "\"peerId\":\"" << localPeerId_ << "\","
             << "\"playerName\":\"" << EscapeJson(playerName) << "\""
             << "}";
        
        std::string url = serverUrl_ + "/api/sessions/" + sessionId + "/join";
        
        HttpPost(url, json.str(), [this, callback, sessionId, playerName]
            (bool success, const std::string& response) 
        {
            if (success) {
                std::string hostPeerId = ExtractJsonString(response, "hostPeerId");
                
                if (!hostPeerId.empty()) {
                    currentSession_.sessionId = sessionId;
                    currentSession_.hostPeerId = hostPeerId;
                    isInSession_ = true;
                    isHost_ = false;
                    
                    LOGF_INFO("[CommunitySession] Joined session: {}", sessionId);
                    if (callback) callback(true, hostPeerId, "");
                } else {
                    if (callback) callback(false, "", "Failed to get host info");
                }
            } else {
                LOGF_ERROR("[CommunitySession] Failed to join session: {}", response);
                if (callback) callback(false, "", response);
            }
        });
    }
    
    void JoinByCode(const std::string& lobbyCode, const std::string& playerName,
                    OnSessionJoinedCallback callback) override 
    {
        if (!initialized_) {
            if (callback) callback(false, "", "Not initialized");
            return;
        }
        
        // First, find session by code
        std::string url = serverUrl_ + "/api/sessions?lobbyCode=" + lobbyCode;
        
        HttpGet(url, [this, callback, playerName, lobbyCode](bool success, const std::string& response) {
            if (success) {
                std::string sessionId = ExtractJsonString(response, "sessionId");
                if (sessionId.empty()) {
                    sessionId = ExtractJsonString(response, "id");
                }
                
                if (!sessionId.empty()) {
                    JoinSession(sessionId, playerName, callback);
                } else {
                    LOGF_ERROR("[CommunitySession] Lobby code not found: {}", lobbyCode);
                    if (callback) callback(false, "", "Lobby not found");
                }
            } else {
                if (callback) callback(false, "", "Failed to find lobby");
            }
        });
    }
    
    void LeaveSession() override {
        if (!isInSession_) return;
        
        if (isHost_) {
            CloseSession();
        } else {
            std::string url = serverUrl_ + "/api/sessions/" + currentSession_.sessionId + "/leave";
            
            std::ostringstream json;
            json << "{\"peerId\":\"" << localPeerId_ << "\"}";
            
            HttpPost(url, json.str(), [](bool success, const std::string&) {
                // Don't care about result
            });
        }
        
        isInSession_ = false;
        isHost_ = false;
        currentSession_ = SessionInfo{};
        
        LOG_INFO("[CommunitySession] Left session");
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
        
        ProcessPendingRequests();
        
        // Rate limit polling
        double currentTime = ImGui::GetTime();
        if (currentTime - lastPollTime_ < POLL_INTERVAL) {
            return;
        }
        lastPollTime_ = currentTime;
        
        // Heartbeat for hosts
        if (isHost_ && isInSession_) {
            if (currentTime - lastHeartbeatTime_ >= HEARTBEAT_INTERVAL) {
                SendHeartbeat();
                lastHeartbeatTime_ = currentTime;
            }
        }
        
        // Poll for session updates if in session
        if (isInSession_ && !isHost_) {
            PollSessionUpdates();
        }
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
        
        std::string id = "peer_";
        for (int i = 0; i < 16; ++i) {
            id += chars[dis(gen)];
        }
        return id;
    }
    
    std::string GenerateLobbyCode() {
        static const char chars[] = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789"; // No I, O, 0, 1
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, sizeof(chars) - 2);
        
        std::string code;
        for (int i = 0; i < 6; ++i) {
            code += chars[dis(gen)];
        }
        return code;
    }
    
    static std::string EscapeJson(const std::string& str) {
        std::string result;
        for (char c : str) {
            switch (c) {
                case '"':  result += "\\\""; break;
                case '\\': result += "\\\\"; break;
                case '\n': result += "\\n"; break;
                case '\r': result += "\\r"; break;
                case '\t': result += "\\t"; break;
                default:   result += c; break;
            }
        }
        return result;
    }
    
    static std::string ExtractJsonString(const std::string& json, const std::string& key) {
        std::string search = "\"" + key + "\":\"";
        size_t pos = json.find(search);
        if (pos == std::string::npos) return "";
        pos += search.length();
        size_t endPos = json.find("\"", pos);
        if (endPos == std::string::npos) return "";
        return json.substr(pos, endPos - pos);
    }
    
    static int ExtractJsonInt(const std::string& json, const std::string& key) {
        std::string search = "\"" + key + "\":";
        size_t pos = json.find(search);
        if (pos == std::string::npos) return 0;
        pos += search.length();
        // Skip whitespace
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        size_t endPos = pos;
        while (endPos < json.size() && (isdigit(json[endPos]) || json[endPos] == '-')) endPos++;
        if (endPos == pos) return 0;
        return std::stoi(json.substr(pos, endPos - pos));
    }
    
    static bool ExtractJsonBool(const std::string& json, const std::string& key) {
        std::string search = "\"" + key + "\":";
        size_t pos = json.find(search);
        if (pos == std::string::npos) return false;
        pos += search.length();
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        return json.substr(pos, 4) == "true";
    }
    
    void PollSessionUpdates() {
        // Poll for player list changes
        std::string url = serverUrl_ + "/api/sessions/" + currentSession_.sessionId;
        
        HttpGet(url, [this](bool success, const std::string& response) {
            if (success) {
                int newPlayerCount = ExtractJsonInt(response, "currentPlayers");
                if (newPlayerCount != currentSession_.currentPlayers) {
                    currentSession_.currentPlayers = newPlayerCount;
                    if (onSessionUpdated_) {
                        onSessionUpdated_(currentSession_);
                    }
                }
            }
        });
    }
    
    // =========================================================================
    // HTTP Request Queue
    // =========================================================================
    
    struct PendingRequest {
        std::string url;
        std::string method;
        std::string body;
        std::function<void(bool, const std::string&)> callback;
    };
    
    void HttpGet(const std::string& url, std::function<void(bool, const std::string&)> callback) {
        std::lock_guard<std::mutex> lock(requestsMutex_);
        pendingRequests_.push({url, "GET", "", callback});
    }
    
    void HttpPost(const std::string& url, const std::string& json,
                  std::function<void(bool, const std::string&)> callback) {
        std::lock_guard<std::mutex> lock(requestsMutex_);
        pendingRequests_.push({url, "POST", json, callback});
    }
    
    void HttpPut(const std::string& url, const std::string& json,
                 std::function<void(bool, const std::string&)> callback) {
        std::lock_guard<std::mutex> lock(requestsMutex_);
        pendingRequests_.push({url, "PUT", json, callback});
    }
    
    void HttpDelete(const std::string& url, std::function<void(bool)> callback) {
        std::lock_guard<std::mutex> lock(requestsMutex_);
        pendingRequests_.push({url, "DELETE", "", 
            [callback](bool success, const std::string&) { if (callback) callback(success); }});
    }
    
    void ProcessPendingRequests() {
        PendingRequest req;
        
        {
            std::lock_guard<std::mutex> lock(requestsMutex_);
            if (pendingRequests_.empty()) return;
            req = std::move(pendingRequests_.front());
            pendingRequests_.pop();
        }
        
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
        
        if (req.method == "POST") {
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req.body.c_str());
        } else if (req.method == "PUT") {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
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
    
    // =========================================================================
    // State
    // =========================================================================
    
    bool initialized_ = false;
    bool isInSession_ = false;
    bool isHost_ = false;
    std::string serverUrl_;
    std::string localPeerId_;
    SessionInfo currentSession_;
    
    // Callbacks
    OnPlayerJoinedCallback onPlayerJoined_;
    OnPlayerLeftCallback onPlayerLeft_;
    OnSessionUpdatedCallback onSessionUpdated_;
    
    // HTTP request queue
    std::mutex requestsMutex_;
    std::queue<PendingRequest> pendingRequests_;
    
    // Polling
    double lastPollTime_ = 0;
    double lastHeartbeatTime_ = 0;
    static constexpr double POLL_INTERVAL = 1.0;      // 1 second
    static constexpr double HEARTBEAT_INTERVAL = 30.0; // 30 seconds
};

// ============================================================================
// Factory Function
// ============================================================================

std::unique_ptr<ISessionTracker> CreateCommunitySessionTracker() {
    return std::make_unique<CommunitySessionTracker>();
}

} // namespace Net
