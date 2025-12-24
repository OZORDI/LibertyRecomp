#include "network_status_overlay.h"
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
static constexpr double REFRESH_INTERVAL = 2.0; // seconds

void NetworkStatusOverlay::Init()
{
    RefreshStatus();
}

void NetworkStatusOverlay::Draw()
{
    if (!s_isVisible)
        return;
    
    auto time = ImGui::GetTime();
    
    // Periodic status refresh
    if (time - g_lastRefreshTime > REFRESH_INTERVAL)
    {
        RefreshStatus();
        g_lastRefreshTime = time;
    }
    
    // Only show if in a P2P session
    if (s_lastStatus == NetworkOverlayStatus::Offline)
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
        case NetworkOverlayStatus::Connected:
            color = IM_COL32(50, 200, 50, 255);
            break;
        case NetworkOverlayStatus::Connecting:
            color = IM_COL32(255, 200, 50, 255);
            pulse = true;
            break;
        case NetworkOverlayStatus::Offline:
            color = IM_COL32(150, 150, 150, 255);
            break;
        case NetworkOverlayStatus::Error:
            color = IM_COL32(200, 50, 50, 255);
            pulse = true;
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
    
    // Check for hover to show tooltip
    ImVec2 clickMin(x - radius * 2, y - radius);
    ImVec2 clickMax(x + radius * 2, y + radius * 3);
    
    if (ImGui::IsMouseHoveringRect(clickMin, clickMax))
    {
        // Show tooltip
        ImGui::BeginTooltip();
        
        const char* statusText = "Unknown";
        switch (s_lastStatus)
        {
            case NetworkOverlayStatus::Connected:
                statusText = "Online - Connected";
                break;
            case NetworkOverlayStatus::Connecting:
                statusText = "Connecting...";
                break;
            case NetworkOverlayStatus::Offline:
                statusText = "Offline";
                break;
            case NetworkOverlayStatus::Error:
                statusText = "Connection Error";
                break;
        }
        
        ImGui::Text("%s", statusText);
        if (s_peerCount > 0)
        {
            ImGui::Text("Peers: %d", s_peerCount);
        }
        ImGui::Text("Click to toggle details");
        ImGui::EndTooltip();
        
        // Toggle details on click
        if (ImGui::IsMouseClicked(0))
        {
            s_showDetails = !s_showDetails;
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
        case NetworkOverlayStatus::Connected:
            indicatorColor = IM_COL32(50, 200, 50, 255);
            statusText = "Connected";
            break;
        case NetworkOverlayStatus::Connecting:
            indicatorColor = IM_COL32(255, 200, 50, 255);
            statusText = "Connecting...";
            break;
        case NetworkOverlayStatus::Offline:
            indicatorColor = IM_COL32(150, 150, 150, 255);
            statusText = "Disconnected";
            break;
        case NetworkOverlayStatus::Error:
            indicatorColor = IM_COL32(200, 50, 50, 255);
            statusText = "Error";
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
    if (s_lastStatus == NetworkOverlayStatus::Connected && s_peerCount > 0)
    {
        textPos.y += Scale(16.0f);
        char peerText[64];
        snprintf(peerText, sizeof(peerText), "Peers: %d", s_peerCount);
        DrawTextWithShadow(g_pFntNewRodin, Scale(10.0f), textPos,
            GTA4Style::Colors::TextGray, peerText);
    }
    
    // Close button hint
    textPos = ImVec2(min.x + Scale(5.0f), max.y - Scale(15.0f));
    DrawTextWithShadow(g_pFntNewRodin, Scale(9.0f), textPos,
        GTA4Style::Colors::TextGray, "Click indicator to close");
    
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
    auto& p2p = Net::P2PManager::Instance();
    
    if (!p2p.IsInitialized())
    {
        s_lastStatus = NetworkOverlayStatus::Offline;
        s_peerCount = 0;
        return;
    }
    
    auto lobbyState = p2p.GetLobbyState();
    
    switch (lobbyState)
    {
        case Net::P2PLobbyState::Active:
        case Net::P2PLobbyState::Joined:
            s_lastStatus = NetworkOverlayStatus::Connected;
            s_peerCount = static_cast<int>(p2p.GetConnectedPeers().size());
            break;
        case Net::P2PLobbyState::Creating:
        case Net::P2PLobbyState::Joining:
            s_lastStatus = NetworkOverlayStatus::Connecting;
            s_peerCount = 0;
            break;
        case Net::P2PLobbyState::Failed:
            s_lastStatus = NetworkOverlayStatus::Error;
            s_peerCount = 0;
            break;
        default:
            s_lastStatus = NetworkOverlayStatus::Offline;
            s_peerCount = 0;
            break;
    }
}
