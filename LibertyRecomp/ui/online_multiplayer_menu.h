#pragma once

#include <ui/common_menu.h>
#include <install/nebula_manager.h>
#include <string>
#include <vector>

enum class OnlineMultiplayerMenuState
{
    Opening,
    Idle,
    Closing
};

enum class OnlineMultiplayerPage
{
    Main,           // Status overview + Create/Join/Disconnect
    CreateNetwork,  // Network creation wizard
    JoinNetwork,    // Network join wizard  
    Connecting,     // Connection progress
    Connected,      // Peer list + disconnect
    Diagnostics     // Network test + troubleshooting
};

class OnlineMultiplayerMenu
{
public:
    static inline CommonMenu s_commonMenu{};
    static inline OnlineMultiplayerMenuState s_state{};
    static inline OnlineMultiplayerPage s_currentPage{OnlineMultiplayerPage::Main};
    static inline bool s_isVisible{};
    
    // Network state
    static inline liberty::install::NebulaStatus s_nebulaStatus{};
    static inline std::string s_statusMessage{};
    static inline std::vector<std::string> s_connectedPeers{};
    
    // Create network form state
    static inline char s_networkName[64]{};
    static inline char s_publicIP[64]{};
    static inline int s_listenPort{4242};
    
    // Join network form state
    static inline char s_lighthouseAddress[128]{};
    static inline char s_virtualIP[32]{"192.168.100.2/24"};
    static inline char s_caCertificate[4096]{};
    
    // Connection progress
    static inline float s_connectionProgress{0.0f};
    static inline std::string s_connectionStatus{};
    
    static void Init();
    static void Draw();
    static void Open();
    static void Close();
    static bool IsVisible() { return s_isVisible; }
    
    // Page navigation
    static void NavigateToPage(OnlineMultiplayerPage page);
    static void GoBack();
    
private:
    // Page rendering
    static void DrawMainPage();
    static void DrawCreateNetworkPage();
    static void DrawJoinNetworkPage();
    static void DrawConnectingPage();
    static void DrawConnectedPage();
    static void DrawDiagnosticsPage();
    
    // Actions
    static void StartCreateNetwork();
    static void StartJoinNetwork();
    static void Disconnect();
    static void RunDiagnostics();
    static void RefreshStatus();
    
    // UI helpers
    static void DrawStatusIndicator();
    static void DrawButton(const char* label, bool enabled, std::function<void()> onClick);
    static void DrawInputField(const char* label, char* buffer, size_t bufferSize, const char* hint = nullptr);
    static void DrawProgressBar(float progress, const char* label);
};
