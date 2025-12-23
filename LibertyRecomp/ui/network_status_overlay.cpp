#include "network_status_overlay.h"
#include "online_multiplayer_menu.h"
#include <ui/imgui_utils.h>
#include <ui/gta4_style.h>
#include <hid/hid.h>

#include <imgui.h>

// Position constants
static constexpr float OVERLAY_MARGIN = 15.0f;
static constexpr float INDICATOR_SIZE = 12.0f;
static constexpr float DETAILED_WIDTH = 200.0f;
static constexpr float DETAILED_HEIGHT = 80.0f;

// Animation
static double g_pulseTime{};
static double g_lastRefreshTime{};
static constexpr double REFRESH_INTERVAL = 5.0; // seconds

void NetworkStatusOverlay::Init()
{
    RefreshStatus();
}

void NetworkStatusOverlay::Draw()
{
    if (!s_isVisible)
        return;
    
    // Don't show overlay if online multiplayer menu is open
    if (OnlineMultiplayerMenu::IsVisible())
        return;
    
    auto time = ImGui::GetTime();
    
    // Periodic status refresh
    if (time - g_lastRefreshTime > REFRESH_INTERVAL)
    {
        RefreshStatus();
        g_lastRefreshTime = time;
    }
    
    // Only show if Nebula is configured or running
    if (s_lastStatus == liberty::install::NebulaStatus::NotInstalled ||
        s_lastStatus == liberty::install::NebulaStatus::NotConfigured)
    {
        return;
    }
    
    if (s_showDetails)
    {
        DrawDetailed();
    }
    else
    {
        DrawMinimal();
    }
}

void NetworkStatusOverlay::DrawMinimal()
{
    auto& io = ImGui::GetIO();
    auto time = ImGui::GetTime();
    
    // Position in top-right corner
    float x = io.DisplaySize.x - Scale(OVERLAY_MARGIN) - Scale(INDICATOR_SIZE);
    float y = Scale(OVERLAY_MARGIN);
    
    ImVec2 center(x, y + Scale(INDICATOR_SIZE) / 2.0f);
    float radius = Scale(INDICATOR_SIZE) / 2.0f;
    
    // Determine color based on status
    ImU32 color = IM_COL32(100, 100, 100, 255);
    bool pulse = false;
    
    switch (s_lastStatus)
    {
        case liberty::install::NebulaStatus::Running:
            color = IM_COL32(50, 200, 50, 255);
            break;
        case liberty::install::NebulaStatus::Starting:
            color = IM_COL32(255, 200, 50, 255);
            pulse = true;
            break;
        case liberty::install::NebulaStatus::Stopped:
            color = IM_COL32(150, 150, 150, 255);
            break;
        case liberty::install::NebulaStatus::Error:
            color = IM_COL32(200, 50, 50, 255);
            pulse = true;
            break;
        default:
            break;
    }
    
    // Apply pulse animation
    if (pulse)
    {
        float alpha = (sin(time * 4.0) + 1.0f) / 2.0f * 0.5f + 0.5f;
        uint8_t a = static_cast<uint8_t>(255 * alpha);
        color = (color & 0x00FFFFFF) | (a << 24);
    }
    
    // Draw indicator
    auto* drawList = ImGui::GetForegroundDrawList();
    drawList->AddCircleFilled(center, radius, color);
    drawList->AddCircle(center, radius, IM_COL32(255, 255, 255, 80), 0, Scale(1.0f));
    
    // Check for click to show details
    ImVec2 clickMin(x - radius * 2, y - radius);
    ImVec2 clickMax(x + radius * 2, y + radius * 3);
    
    if (ImGui::IsMouseHoveringRect(clickMin, clickMax))
    {
        // Show tooltip
        ImGui::BeginTooltip();
        
        const char* statusText = "Unknown";
        switch (s_lastStatus)
        {
            case liberty::install::NebulaStatus::Running:
                statusText = "Online - Connected";
                break;
            case liberty::install::NebulaStatus::Starting:
                statusText = "Connecting...";
                break;
            case liberty::install::NebulaStatus::Stopped:
                statusText = "Offline";
                break;
            case liberty::install::NebulaStatus::Error:
                statusText = "Connection Error";
                break;
            default:
                break;
        }
        
        ImGui::Text("%s", statusText);
        ImGui::Text("Click to open Online Multiplayer menu");
        ImGui::EndTooltip();
        
        // Open menu on click
        if (ImGui::IsMouseClicked(0))
        {
            OnlineMultiplayerMenu::Open();
        }
    }
}

void NetworkStatusOverlay::DrawDetailed()
{
    auto& io = ImGui::GetIO();
    
    // Position in top-right corner
    float x = io.DisplaySize.x - Scale(OVERLAY_MARGIN) - Scale(DETAILED_WIDTH);
    float y = Scale(OVERLAY_MARGIN);
    
    ImVec2 min(x, y);
    ImVec2 max(x + Scale(DETAILED_WIDTH), y + Scale(DETAILED_HEIGHT));
    
    auto* drawList = ImGui::GetForegroundDrawList();
    
    // Background
    drawList->AddRectFilled(min, max, GTA4Style::Colors::BackgroundPanel, Scale(6.0f));
    drawList->AddRect(min, max, GTA4Style::Colors::Border, Scale(6.0f), 0, Scale(1.0f));
    
    // Status indicator
    ImVec2 indicatorCenter(min.x + Scale(15.0f), min.y + Scale(15.0f));
    float indicatorRadius = Scale(6.0f);
    
    ImU32 indicatorColor = IM_COL32(100, 100, 100, 255);
    const char* statusText = "Unknown";
    
    switch (s_lastStatus)
    {
        case liberty::install::NebulaStatus::Running:
            indicatorColor = IM_COL32(50, 200, 50, 255);
            statusText = "Connected";
            break;
        case liberty::install::NebulaStatus::Starting:
            indicatorColor = IM_COL32(255, 200, 50, 255);
            statusText = "Connecting...";
            break;
        case liberty::install::NebulaStatus::Stopped:
            indicatorColor = IM_COL32(150, 150, 150, 255);
            statusText = "Disconnected";
            break;
        case liberty::install::NebulaStatus::Error:
            indicatorColor = IM_COL32(200, 50, 50, 255);
            statusText = "Error";
            break;
        default:
            break;
    }
    
    drawList->AddCircleFilled(indicatorCenter, indicatorRadius, indicatorColor);
    
    // Status text
    ImVec2 textPos(min.x + Scale(28.0f), min.y + Scale(8.0f));
    DrawTextWithShadow(g_pFntNewRodin, Scale(12.0f), textPos, 
        GTA4Style::Colors::TextOrange, "ONLINE MULTIPLAYER");
    
    textPos.y += Scale(16.0f);
    DrawTextWithShadow(g_pFntNewRodin, Scale(11.0f), textPos,
        indicatorColor, statusText);
    
    // Peer count if connected
    if (s_lastStatus == liberty::install::NebulaStatus::Running)
    {
        auto& nebula = liberty::install::NebulaManager::Instance();
        auto peers = nebula.GetConnectedPeers();
        
        textPos.y += Scale(16.0f);
        char peerText[64];
        snprintf(peerText, sizeof(peerText), "Peers: %zu", peers.size());
        DrawTextWithShadow(g_pFntNewRodin, Scale(10.0f), textPos,
            GTA4Style::Colors::TextGray, peerText);
    }
    
    // Close button hint
    textPos = ImVec2(min.x + Scale(5.0f), max.y - Scale(15.0f));
    DrawTextWithShadow(g_pFntNewRodin, Scale(9.0f), textPos,
        GTA4Style::Colors::TextGray, "Click indicator to toggle");
    
    // Check for click on indicator to toggle
    ImVec2 clickMin(min.x, min.y);
    ImVec2 clickMax(min.x + Scale(30.0f), min.y + Scale(30.0f));
    
    if (ImGui::IsMouseHoveringRect(clickMin, clickMax) && ImGui::IsMouseClicked(0))
    {
        s_showDetails = false;
    }
}

void NetworkStatusOverlay::RefreshStatus()
{
    auto& nebula = liberty::install::NebulaManager::Instance();
    s_lastStatus = nebula.GetStatus();
}
