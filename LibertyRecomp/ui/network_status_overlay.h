#pragma once

#include <install/nebula_manager.h>

class NetworkStatusOverlay
{
public:
    static inline bool s_isVisible{true};
    static inline bool s_showDetails{false};
    static inline liberty::install::NebulaStatus s_lastStatus{};
    
    static void Init();
    static void Draw();
    static void SetVisible(bool visible) { s_isVisible = visible; }
    static bool IsVisible() { return s_isVisible; }
    
    static void RefreshStatus();
    
private:
    static void DrawMinimal();
    static void DrawDetailed();
};
