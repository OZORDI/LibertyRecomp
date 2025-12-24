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
// FirebaseSessionTracker Implementation
// ============================================================================

class FirebaseSessionTracker : public ISessionTracker {
public:
    FirebaseSessionTracker() {
        localPeerId_ = GeneratePeerId();
    }
    
    ~FirebaseSessionTracker() override {
        Shutdown();
    }
    
    // =========================================================================
    // Lifecycle
    // =========================================================================
    
    bool Initialize() override {
        if (initialized_) return true;
        
        projectId_ = Config::FirebaseProjectId.Value;
        apiKey_ = Config::FirebaseApiKey.Value;
        
        if (projectId_.empty()) {
            LOG_ERROR("[FirebaseSession] Firebase project ID is empty. Configure in settings.");
            return false;
        }
        
        LOGF_INFO("[FirebaseSession] Initialized with project: {}", projectId_);
        LOGF_INFO("[FirebaseSession] Local peer ID: {}", localPeerId_);
        
        initialized_ = true;
        return true;
    }
    
    void Shutdown() override {
        if (!initialized_) return;
        
        if (IsInSession()) {
            LeaveSession();
        }
        
        initialized_ = false;
        LOG_INFO("[FirebaseSession] Shutdown complete");
    }
    
    bool IsInitialized() const override { return initialized_; }
    
    const char* GetBackendName() const override { return "Firebase (Self-Hosted)"; }
    
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
            LOG_ERROR("[FirebaseSession] CreateSession failed: not initialized");
            if (callback) callback(false, "", "");
            return;
        }
        
        if (IsInSession()) {
            LOG_ERROR("[FirebaseSession] CreateSession failed: already in session");
            if (callback) callback(false, "", "");
            return;
        }
        
        std::string lobbyCode = isPrivate ? GenerateLobbyCode() : "";
        std::string sessionId = lobbyCode.empty() ? GenerateSessionId() : lobbyCode;
        
        // Build JSON payload for Firebase
        std::ostringstream json;
        json << "{"
             << "\"hostPeerId\":\"" << localPeerId_ << "\","
             << "\"hostName\":\"" << EscapeJson(playerName) << "\","
             << "\"gameMode\":" << static_cast<int>(gameMode) << ","
             << "\"mapArea\":" << static_cast<int>(mapArea) << ","
             << "\"maxPlayers\":" << maxPlayers << ","
             << "\"currentPlayers\":1,"
             << "\"isPrivate\":" << (isPrivate ? "true" : "false") << ","
             << "\"lobbyCode\":\"" << lobbyCode << "\","
             << "\"createdAt\":{\".sv\":\"timestamp\"},"
             << "\"lastHeartbeat\":{\".sv\":\"timestamp\"},"
             << "\"players\":{"
             << "\"" << localPeerId_ << "\":{"
             << "\"name\":\"" << EscapeJson(playerName) << "\","
             << "\"joinedAt\":{\".sv\":\"timestamp\"}"
             << "}"
             << "}"
             << "}";
        
        std::string url = BuildFirebaseUrl("/sessions/" + sessionId + ".json");
        
        HttpPut(url, json.str(), [this, callback, sessionId, lobbyCode, playerName, gameMode, mapArea, maxPlayers, isPrivate]
            (bool success, const std::string& response) 
        {
            if (success && response != "null") {
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
                
                LOGF_INFO("[FirebaseSession] Session created: {} (code: {})", sessionId, lobbyCode);
                if (callback) callback(true, sessionId, lobbyCode);
            } else {
                LOGF_ERROR("[FirebaseSession] Failed to create session: {}", response);
                if (callback) callback(false, "", "");
            }
        });
    }
    
    void UpdateSession(const SessionInfo& info) override {
        if (!isHost_ || !isInSession_) return;
        
        std::ostringstream json;
        json << "{"
             << "\"currentPlayers\":" << info.currentPlayers << ","
             << "\"lastHeartbeat\":{\".sv\":\"timestamp\"}"
             << "}";
        
        std::string url = BuildFirebaseUrl("/sessions/" + currentSession_.sessionId + ".json");
        
        HttpPatch(url, json.str(), [](bool success, const std::string& response) {
            if (!success) {
                LOG_WARNING("[FirebaseSession] Failed to update session");
            }
        });
        
        currentSession_.currentPlayers = info.currentPlayers;
    }
    
    void CloseSession() override {
        if (!isHost_ || !isInSession_) return;
        
        std::string url = BuildFirebaseUrl("/sessions/" + currentSession_.sessionId + ".json");
        
        HttpDelete(url, [this](bool success) {
            if (success) {
                LOG_INFO("[FirebaseSession] Session closed");
            }
        });
        
        isInSession_ = false;
        isHost_ = false;
        currentSession_ = SessionInfo{};
    }
    
    void SendHeartbeat() override {
        if (!isHost_ || !isInSession_) return;
        
        std::ostringstream json;
        json << "{\"lastHeartbeat\":{\".sv\":\"timestamp\"}}";
        
        std::string url = BuildFirebaseUrl("/sessions/" + currentSession_.sessionId + ".json");
        
        HttpPatch(url, json.str(), [](bool success, const std::string&) {
            if (!success) {
                LOG_WARNING("[FirebaseSession] Heartbeat failed");
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
        
        SessionSearchFilter filter;
        filter.gameMode = gameMode;
        filter.filterByGameMode = true;
        filter.includeFull = false;
        filter.includePrivate = false;
        filter.maxResults = 1;
        
        SearchSessions(filter, [this, callback](bool success, const std::vector<SessionInfo>& sessions) {
            if (success && !sessions.empty()) {
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
        
        // Firebase query - get all sessions, filter client-side
        // (Firebase Realtime DB has limited query capabilities)
        std::string url = BuildFirebaseUrl("/sessions.json");
        url += "&orderBy=\"createdAt\"&limitToLast=" + std::to_string(filter.maxResults * 2);
        
        HttpGet(url, [this, filter, callback](bool success, const std::string& response) {
            std::vector<SessionInfo> sessions;
            
            if (success && response != "null" && !response.empty()) {
                // Parse Firebase response (object with session IDs as keys)
                size_t pos = 0;
                while ((pos = response.find("\"hostPeerId\"", pos)) != std::string::npos) {
                    size_t objStart = response.rfind('"', pos - 3);
                    if (objStart == std::string::npos) { pos++; continue; }
                    
                    // Find session ID (the key before this object)
                    size_t keyStart = response.rfind('"', objStart - 2);
                    if (keyStart == std::string::npos) { pos++; continue; }
                    
                    std::string sessionId = response.substr(keyStart + 1, objStart - keyStart - 2);
                    
                    // Find end of this object
                    int braceCount = 0;
                    size_t objEnd = pos;
                    for (size_t i = objStart; i < response.size(); i++) {
                        if (response[i] == '{') braceCount++;
                        else if (response[i] == '}') {
                            braceCount--;
                            if (braceCount == 0) { objEnd = i; break; }
                        }
                    }
                    
                    std::string obj = response.substr(objStart, objEnd - objStart + 1);
                    
                    SessionInfo session;
                    session.sessionId = sessionId;
                    session.hostPeerId = ExtractJsonString(obj, "hostPeerId");
                    session.hostName = ExtractJsonString(obj, "hostName");
                    session.lobbyCode = ExtractJsonString(obj, "lobbyCode");
                    session.gameMode = static_cast<GameMode>(ExtractJsonInt(obj, "gameMode"));
                    session.mapArea = static_cast<MapArea>(ExtractJsonInt(obj, "mapArea"));
                    session.maxPlayers = ExtractJsonInt(obj, "maxPlayers");
                    session.currentPlayers = ExtractJsonInt(obj, "currentPlayers");
                    session.isPrivate = ExtractJsonBool(obj, "isPrivate");
                    
                    // Apply filters
                    bool include = true;
                    if (filter.filterByGameMode && session.gameMode != filter.gameMode) include = false;
                    if (filter.filterByMapArea && session.mapArea != filter.mapArea) include = false;
                    if (!filter.includePrivate && session.isPrivate) include = false;
                    if (!filter.includeFull && session.currentPlayers >= session.maxPlayers) include = false;
                    
                    if (include && !session.hostPeerId.empty()) {
                        sessions.push_back(session);
                    }
                    
                    pos = objEnd + 1;
                }
                
                // Limit results
                if (sessions.size() > filter.maxResults) {
                    sessions.resize(filter.maxResults);
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
        
        // First get session info
        std::string url = BuildFirebaseUrl("/sessions/" + sessionId + ".json");
        
        HttpGet(url, [this, callback, sessionId, playerName](bool success, const std::string& response) {
            if (!success || response == "null" || response.empty()) {
                if (callback) callback(false, "", "Session not found");
                return;
            }
            
            std::string hostPeerId = ExtractJsonString(response, "hostPeerId");
            if (hostPeerId.empty()) {
                if (callback) callback(false, "", "Invalid session data");
                return;
            }
            
            // Add ourselves to players list
            std::ostringstream playerJson;
            playerJson << "{"
                       << "\"name\":\"" << EscapeJson(playerName) << "\","
                       << "\"joinedAt\":{\".sv\":\"timestamp\"}"
                       << "}";
            
            std::string playerUrl = BuildFirebaseUrl("/sessions/" + sessionId + "/players/" + localPeerId_ + ".json");
            
            HttpPut(playerUrl, playerJson.str(), [this, callback, sessionId, hostPeerId](bool success, const std::string& response) {
                if (success) {
                    currentSession_.sessionId = sessionId;
                    currentSession_.hostPeerId = hostPeerId;
                    isInSession_ = true;
                    isHost_ = false;
                    
                    LOGF_INFO("[FirebaseSession] Joined session: {}", sessionId);
                    if (callback) callback(true, hostPeerId, "");
                } else {
                    if (callback) callback(false, "", "Failed to join session");
                }
            });
        });
    }
    
    void JoinByCode(const std::string& lobbyCode, const std::string& playerName,
                    OnSessionJoinedCallback callback) override 
    {
        // In Firebase, lobby code IS the session ID for private sessions
        // Or we need to search for it
        std::string url = BuildFirebaseUrl("/sessions.json");
        url += "&orderBy=\"lobbyCode\"&equalTo=\"" + lobbyCode + "\"";
        
        HttpGet(url, [this, callback, playerName, lobbyCode](bool success, const std::string& response) {
            if (success && response != "null" && response != "{}") {
                // Extract session ID from response
                size_t pos = response.find("\"hostPeerId\"");
                if (pos != std::string::npos) {
                    size_t keyStart = response.rfind('"', pos - 3);
                    size_t keyEnd = response.rfind('"', keyStart - 1);
                    if (keyStart != std::string::npos && keyEnd != std::string::npos) {
                        std::string sessionId = response.substr(keyEnd + 1, keyStart - keyEnd - 2);
                        JoinSession(sessionId, playerName, callback);
                        return;
                    }
                }
            }
            
            // Try using lobby code directly as session ID
            JoinSession(lobbyCode, playerName, callback);
        });
    }
    
    void LeaveSession() override {
        if (!isInSession_) return;
        
        if (isHost_) {
            CloseSession();
        } else {
            std::string url = BuildFirebaseUrl("/sessions/" + currentSession_.sessionId + "/players/" + localPeerId_ + ".json");
            HttpDelete(url, [](bool) {});
        }
        
        isInSession_ = false;
        isHost_ = false;
        currentSession_ = SessionInfo{};
        
        LOG_INFO("[FirebaseSession] Left session");
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
        
        double currentTime = ImGui::GetTime();
        if (currentTime - lastPollTime_ < POLL_INTERVAL) {
            return;
        }
        lastPollTime_ = currentTime;
        
        if (isHost_ && isInSession_) {
            if (currentTime - lastHeartbeatTime_ >= HEARTBEAT_INTERVAL) {
                SendHeartbeat();
                lastHeartbeatTime_ = currentTime;
            }
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
    
    std::string GenerateSessionId() {
        static const char chars[] = "abcdefghijklmnopqrstuvwxyz0123456789";
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, sizeof(chars) - 2);
        
        std::string id;
        for (int i = 0; i < 20; ++i) {
            id += chars[dis(gen)];
        }
        return id;
    }
    
    std::string BuildFirebaseUrl(const std::string& path) {
        std::string url = "https://" + projectId_ + "-default-rtdb.firebaseio.com" + path;
        if (!apiKey_.empty()) {
            url += (path.find('?') != std::string::npos ? "&" : "?");
            url += "auth=" + apiKey_;
        }
        return url;
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
    
    void HttpPut(const std::string& url, const std::string& json,
                 std::function<void(bool, const std::string&)> callback) {
        std::lock_guard<std::mutex> lock(requestsMutex_);
        pendingRequests_.push({url, "PUT", json, callback});
    }
    
    void HttpPatch(const std::string& url, const std::string& json,
                   std::function<void(bool, const std::string&)> callback) {
        std::lock_guard<std::mutex> lock(requestsMutex_);
        pendingRequests_.push({url, "PATCH", json, callback});
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
        
        if (req.method == "PUT") {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req.body.c_str());
        } else if (req.method == "PATCH") {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
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
    std::string projectId_;
    std::string apiKey_;
    std::string localPeerId_;
    SessionInfo currentSession_;
    
    OnPlayerJoinedCallback onPlayerJoined_;
    OnPlayerLeftCallback onPlayerLeft_;
    OnSessionUpdatedCallback onSessionUpdated_;
    
    std::mutex requestsMutex_;
    std::queue<PendingRequest> pendingRequests_;
    
    double lastPollTime_ = 0;
    double lastHeartbeatTime_ = 0;
    static constexpr double POLL_INTERVAL = 0.5;
    static constexpr double HEARTBEAT_INTERVAL = 30.0;
};

// ============================================================================
// Factory Function
// ============================================================================

std::unique_ptr<ISessionTracker> CreateFirebaseSessionTracker() {
    return std::make_unique<FirebaseSessionTracker>();
}

} // namespace Net
