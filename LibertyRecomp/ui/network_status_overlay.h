#pragma once

#include <kernel/io/p2p_manager.h>

// Network connection status for overlay
enum class NetworkOverlayStatus {
    Offline,
    Connecting,
    Connected,
    Error
};

class NetworkStatusOverlay
{
public:
    static inline bool s_isVisible{true};
    static inline bool s_showDetails{false};
    static inline NetworkOverlayStatus s_lastStatus{NetworkOverlayStatus::Offline};
    static inline int s_peerCount{0};
    
    static void Init();
    static void Draw();
    static void SetVisible(bool visible) { s_isVisible = visible; }
    static bool IsVisible() { return s_isVisible; }
    
    static void RefreshStatus();
    
private:
    static void DrawMinimal();
    static void DrawDetailed();
};
