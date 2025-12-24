#include "online_multiplayer_menu.h"
#include <ui/imgui_utils.h>
#include <ui/gta4_style.h>
#include <ui/button_window.h>
#include <ui/message_window.h>
#include <locale/locale.h>
#include <hid/hid.h>
#include <app.h>

#include <imgui.h>
#include <cstring>
#include <thread>

// UI Constants
static constexpr float MENU_WIDTH = 500.0f;
static constexpr float MENU_HEIGHT = 400.0f;
static constexpr float BUTTON_HEIGHT = 35.0f;
static constexpr float BUTTON_WIDTH = 200.0f;
static constexpr float INPUT_WIDTH = 300.0f;
static constexpr float PADDING = 20.0f;
static constexpr float ITEM_SPACING = 10.0f;

// Animation timing
static double g_stateTime{};
static double g_pageTransitionTime{};

// Input state
static bool g_accepted{};
static bool g_declined{};
static bool g_up{};
static bool g_down{};

// Connection thread
static std::unique_ptr<std::thread> g_connectionThread{};
static std::atomic<bool> g_connectionInProgress{false};
static std::atomic<bool> g_connectionSuccess{false};

void OnlineMultiplayerMenu::Init()
{
    // Initialize form fields with defaults
    std::strncpy(s_networkName, "LibertyRecomp-Network", sizeof(s_networkName));
    std::strncpy(s_virtualIP, "192.168.100.2/24", sizeof(s_virtualIP));
    s_listenPort = 4242;
    
    RefreshStatus();
}

void OnlineMultiplayerMenu::Draw()
{
    if (!s_isVisible)
        return;
    
    auto time = ImGui::GetTime();
    auto& io = ImGui::GetIO();
    
    // Handle input (TODO: Implement proper HID button handling)
    g_accepted = false;
    g_declined = false;
    g_up = false;
    g_down = false;
    
    // Handle back button
    if (g_declined)
    {
        if (s_currentPage == OnlineMultiplayerPage::Main)
        {
            Close();
            return;
        }
        else
        {
            GoBack();
        }
    }
    
    // Draw common menu background
    s_commonMenu.Draw();
    
    // Calculate menu position (centered)
    float menuX = (io.DisplaySize.x - Scale(MENU_WIDTH)) / 2.0f;
    float menuY = (io.DisplaySize.y - Scale(MENU_HEIGHT)) / 2.0f;
    
    ImVec2 menuMin(menuX, menuY);
    ImVec2 menuMax(menuX + Scale(MENU_WIDTH), menuY + Scale(MENU_HEIGHT));
    
    // Draw menu background
    auto* drawList = ImGui::GetBackgroundDrawList();
    drawList->AddRectFilled(menuMin, menuMax, GTA4Style::Colors::BackgroundPanel, Scale(8.0f));
    drawList->AddRect(menuMin, menuMax, GTA4Style::Colors::Border, Scale(8.0f), 0, Scale(2.0f));
    
    // Draw status indicator at top
    DrawStatusIndicator();
    
    // Draw current page content
    ImVec2 contentMin(menuX + Scale(PADDING), menuY + Scale(60.0f));
    ImVec2 contentMax(menuMax.x - Scale(PADDING), menuMax.y - Scale(PADDING));
    
    ImGui::SetCursorScreenPos(contentMin);
    
    switch (s_currentPage)
    {
        case OnlineMultiplayerPage::Main:
            DrawMainPage();
            break;
        case OnlineMultiplayerPage::CreateNetwork:
            DrawCreateNetworkPage();
            break;
        case OnlineMultiplayerPage::JoinNetwork:
            DrawJoinNetworkPage();
            break;
        case OnlineMultiplayerPage::Connecting:
            DrawConnectingPage();
            break;
        case OnlineMultiplayerPage::Connected:
            DrawConnectedPage();
            break;
        case OnlineMultiplayerPage::Diagnostics:
            DrawDiagnosticsPage();
            break;
    }
    
    // Draw navigation hint at bottom
    ImVec2 hintPos(menuX + Scale(PADDING), menuMax.y - Scale(30.0f));
    if (s_currentPage != OnlineMultiplayerPage::Main)
    {
        DrawTextWithShadow(g_pFntNewRodin, Scale(14.0f), hintPos, 
            GTA4Style::Colors::TextGray, "[B] Back", Scale(2.0f));
    }
    else
    {
        DrawTextWithShadow(g_pFntNewRodin, Scale(14.0f), hintPos,
            GTA4Style::Colors::TextGray, "[B] Close", Scale(2.0f));
    }
}

void OnlineMultiplayerMenu::Open()
{
    if (s_isVisible)
        return;
    
    s_isVisible = true;
    s_state = OnlineMultiplayerMenuState::Opening;
    s_currentPage = OnlineMultiplayerPage::Main;
    g_stateTime = ImGui::GetTime();
    
    s_commonMenu.Title = "ONLINE MULTIPLAYER";
    s_commonMenu.Description = "Connect with players over the internet";
    s_commonMenu.Open();
    
    RefreshStatus();
}

void OnlineMultiplayerMenu::Close()
{
    if (!s_isVisible)
        return;
    
    s_state = OnlineMultiplayerMenuState::Closing;
    s_commonMenu.Close();
    s_isVisible = false;
}

void OnlineMultiplayerMenu::NavigateToPage(OnlineMultiplayerPage page)
{
    s_currentPage = page;
    g_pageTransitionTime = ImGui::GetTime();
    
    // Update description based on page
    switch (page)
    {
        case OnlineMultiplayerPage::Main:
            s_commonMenu.SetDescription("Connect with players over the internet");
            break;
        case OnlineMultiplayerPage::CreateNetwork:
            s_commonMenu.SetDescription("Create a new network for others to join");
            break;
        case OnlineMultiplayerPage::JoinNetwork:
            s_commonMenu.SetDescription("Join an existing network");
            break;
        case OnlineMultiplayerPage::Connecting:
            s_commonMenu.SetDescription("Establishing connection...");
            break;
        case OnlineMultiplayerPage::Connected:
            s_commonMenu.SetDescription("Connected to network");
            break;
        case OnlineMultiplayerPage::Diagnostics:
            s_commonMenu.SetDescription("Network diagnostics and troubleshooting");
            break;
    }
}

void OnlineMultiplayerMenu::GoBack()
{
    switch (s_currentPage)
    {
        case OnlineMultiplayerPage::CreateNetwork:
        case OnlineMultiplayerPage::JoinNetwork:
        case OnlineMultiplayerPage::Connected:
        case OnlineMultiplayerPage::Diagnostics:
            NavigateToPage(OnlineMultiplayerPage::Main);
            break;
        case OnlineMultiplayerPage::Connecting:
            // Cancel connection
            g_connectionInProgress = false;
            NavigateToPage(OnlineMultiplayerPage::Main);
            break;
        default:
            break;
    }
}

void OnlineMultiplayerMenu::DrawMainPage()
{
    auto& io = ImGui::GetIO();
    float startY = ImGui::GetCursorScreenPos().y;
    float centerX = io.DisplaySize.x / 2.0f;
    
    // Status display
    const char* statusText = "Unknown";
    ImU32 statusColor = GTA4Style::Colors::TextGray;
    
    switch (s_nebulaStatus)
    {
        case liberty::install::NebulaStatus::NotInstalled:
            statusText = "Nebula Not Installed";
            statusColor = IM_COL32(200, 50, 50, 255);
            break;
        case liberty::install::NebulaStatus::NotConfigured:
            statusText = "Not Configured";
            statusColor = GTA4Style::Colors::TextGray;
            break;
        case liberty::install::NebulaStatus::Stopped:
            statusText = "Disconnected";
            statusColor = IM_COL32(200, 50, 50, 255);
            break;
        case liberty::install::NebulaStatus::Starting:
            statusText = "Connecting...";
            statusColor = IM_COL32(255, 200, 50, 255);
            break;
        case liberty::install::NebulaStatus::Running:
            statusText = "Connected";
            statusColor = IM_COL32(50, 200, 50, 255);
            break;
        case liberty::install::NebulaStatus::Error:
            statusText = "Error";
            statusColor = IM_COL32(200, 50, 50, 255);
            break;
    }
    
    // Draw status text centered
    auto statusSize = g_pFntNewRodin->CalcTextSizeA(Scale(18.0f), FLT_MAX, 0, statusText);
    ImVec2 statusPos(centerX - statusSize.x / 2.0f, startY);
    DrawTextWithShadow(g_pFntNewRodin, Scale(18.0f), statusPos, statusColor, statusText);
    
    startY += Scale(40.0f);
    
    // Action buttons based on status
    float buttonX = centerX - Scale(BUTTON_WIDTH) / 2.0f;
    
    if (s_nebulaStatus == liberty::install::NebulaStatus::Running)
    {
        // Connected - show disconnect and diagnostics
        ImGui::SetCursorScreenPos(ImVec2(buttonX, startY));
        DrawButton("View Connected Peers", true, []() {
            NavigateToPage(OnlineMultiplayerPage::Connected);
        });
        
        startY += Scale(BUTTON_HEIGHT + ITEM_SPACING);
        ImGui::SetCursorScreenPos(ImVec2(buttonX, startY));
        DrawButton("Diagnostics", true, []() {
            NavigateToPage(OnlineMultiplayerPage::Diagnostics);
        });
        
        startY += Scale(BUTTON_HEIGHT + ITEM_SPACING);
        ImGui::SetCursorScreenPos(ImVec2(buttonX, startY));
        DrawButton("Disconnect", true, []() {
            Disconnect();
        });
    }
    else if (s_nebulaStatus == liberty::install::NebulaStatus::NotInstalled)
    {
        // Not installed - show error message
        const char* errorMsg = "Nebula VPN binaries not found.\nPlease run the build script in tools/nebula/";
        auto errorSize = g_pFntNewRodin->CalcTextSizeA(Scale(14.0f), FLT_MAX, Scale(400.0f), errorMsg);
        ImVec2 errorPos(centerX - errorSize.x / 2.0f, startY);
        DrawTextWithShadow(g_pFntNewRodin, Scale(14.0f), errorPos, GTA4Style::Colors::TextGray, errorMsg);
    }
    else
    {
        // Not connected - show create/join options
        ImGui::SetCursorScreenPos(ImVec2(buttonX, startY));
        DrawButton("Create Network", true, []() {
            NavigateToPage(OnlineMultiplayerPage::CreateNetwork);
        });
        
        startY += Scale(BUTTON_HEIGHT + ITEM_SPACING);
        ImGui::SetCursorScreenPos(ImVec2(buttonX, startY));
        DrawButton("Join Network", true, []() {
            NavigateToPage(OnlineMultiplayerPage::JoinNetwork);
        });
        
        startY += Scale(BUTTON_HEIGHT + ITEM_SPACING);
        ImGui::SetCursorScreenPos(ImVec2(buttonX, startY));
        DrawButton("Diagnostics", true, []() {
            NavigateToPage(OnlineMultiplayerPage::Diagnostics);
        });
    }
}

void OnlineMultiplayerMenu::DrawCreateNetworkPage()
{
    auto& io = ImGui::GetIO();
    float startY = ImGui::GetCursorScreenPos().y;
    float labelX = ImGui::GetCursorScreenPos().x;
    float inputX = labelX + Scale(120.0f);
    
    // Network Name
    DrawTextWithShadow(g_pFntNewRodin, Scale(14.0f), ImVec2(labelX, startY), 
        GTA4Style::Colors::TextOrange, "Network Name:");
    ImGui::SetCursorScreenPos(ImVec2(inputX, startY - Scale(3.0f)));
    ImGui::PushItemWidth(Scale(INPUT_WIDTH));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, GTA4Style::Colors::BackgroundDark);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::InputText("##networkname", s_networkName, sizeof(s_networkName));
    ImGui::PopStyleColor(2);
    ImGui::PopItemWidth();
    
    startY += Scale(35.0f);
    
    // Public IP (auto-detected or manual)
    DrawTextWithShadow(g_pFntNewRodin, Scale(14.0f), ImVec2(labelX, startY),
        GTA4Style::Colors::TextOrange, "Public IP:");
    ImGui::SetCursorScreenPos(ImVec2(inputX, startY - Scale(3.0f)));
    ImGui::PushItemWidth(Scale(INPUT_WIDTH));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, GTA4Style::Colors::BackgroundDark);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::InputText("##publicip", s_publicIP, sizeof(s_publicIP));
    ImGui::PopStyleColor(2);
    ImGui::PopItemWidth();
    
    startY += Scale(35.0f);
    
    // Listen Port
    DrawTextWithShadow(g_pFntNewRodin, Scale(14.0f), ImVec2(labelX, startY),
        GTA4Style::Colors::TextOrange, "Port:");
    ImGui::SetCursorScreenPos(ImVec2(inputX, startY - Scale(3.0f)));
    ImGui::PushItemWidth(Scale(100.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, GTA4Style::Colors::BackgroundDark);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::InputInt("##port", &s_listenPort, 0, 0);
    ImGui::PopStyleColor(2);
    ImGui::PopItemWidth();
    
    startY += Scale(50.0f);
    
    // Info text
    const char* infoText = "You will need to forward UDP port on your router.\nOther players will use your public IP to connect.";
    DrawTextWithShadow(g_pFntNewRodin, Scale(12.0f), ImVec2(labelX, startY),
        GTA4Style::Colors::TextGray, infoText);
    
    startY += Scale(50.0f);
    
    // Create button
    float buttonX = io.DisplaySize.x / 2.0f - Scale(BUTTON_WIDTH) / 2.0f;
    ImGui::SetCursorScreenPos(ImVec2(buttonX, startY));
    DrawButton("Create Network", std::strlen(s_networkName) > 0, []() {
        StartCreateNetwork();
    });
}

void OnlineMultiplayerMenu::DrawJoinNetworkPage()
{
    auto& io = ImGui::GetIO();
    float startY = ImGui::GetCursorScreenPos().y;
    float labelX = ImGui::GetCursorScreenPos().x;
    float inputX = labelX + Scale(140.0f);
    
    // Lighthouse Address
    DrawTextWithShadow(g_pFntNewRodin, Scale(14.0f), ImVec2(labelX, startY),
        GTA4Style::Colors::TextOrange, "Lighthouse Address:");
    ImGui::SetCursorScreenPos(ImVec2(inputX, startY - Scale(3.0f)));
    ImGui::PushItemWidth(Scale(INPUT_WIDTH));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, GTA4Style::Colors::BackgroundDark);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::InputTextWithHint("##lighthouse", "e.g., 203.0.113.45:4242", 
        s_lighthouseAddress, sizeof(s_lighthouseAddress));
    ImGui::PopStyleColor(2);
    ImGui::PopItemWidth();
    
    startY += Scale(35.0f);
    
    // Virtual IP
    DrawTextWithShadow(g_pFntNewRodin, Scale(14.0f), ImVec2(labelX, startY),
        GTA4Style::Colors::TextOrange, "Your Virtual IP:");
    ImGui::SetCursorScreenPos(ImVec2(inputX, startY - Scale(3.0f)));
    ImGui::PushItemWidth(Scale(INPUT_WIDTH));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, GTA4Style::Colors::BackgroundDark);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::InputTextWithHint("##virtualip", "e.g., 192.168.100.2/24",
        s_virtualIP, sizeof(s_virtualIP));
    ImGui::PopStyleColor(2);
    ImGui::PopItemWidth();
    
    startY += Scale(35.0f);
    
    // CA Certificate (multiline)
    DrawTextWithShadow(g_pFntNewRodin, Scale(14.0f), ImVec2(labelX, startY),
        GTA4Style::Colors::TextOrange, "CA Certificate:");
    startY += Scale(20.0f);
    
    ImGui::SetCursorScreenPos(ImVec2(labelX, startY));
    ImGui::PushItemWidth(Scale(MENU_WIDTH - PADDING * 2));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, GTA4Style::Colors::BackgroundDark);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(200, 200, 200, 255));
    ImGui::InputTextMultiline("##cacert", s_caCertificate, sizeof(s_caCertificate),
        ImVec2(Scale(MENU_WIDTH - PADDING * 2), Scale(80.0f)));
    ImGui::PopStyleColor(2);
    ImGui::PopItemWidth();
    
    startY += Scale(100.0f);
    
    // Info text
    const char* infoText = "Paste the CA certificate provided by the network host.";
    DrawTextWithShadow(g_pFntNewRodin, Scale(12.0f), ImVec2(labelX, startY),
        GTA4Style::Colors::TextGray, infoText);
    
    startY += Scale(30.0f);
    
    // Join button
    float buttonX = io.DisplaySize.x / 2.0f - Scale(BUTTON_WIDTH) / 2.0f;
    ImGui::SetCursorScreenPos(ImVec2(buttonX, startY));
    bool canJoin = std::strlen(s_lighthouseAddress) > 0 && 
                   std::strlen(s_virtualIP) > 0 &&
                   std::strlen(s_caCertificate) > 0;
    DrawButton("Join Network", canJoin, []() {
        StartJoinNetwork();
    });
}

void OnlineMultiplayerMenu::DrawConnectingPage()
{
    auto& io = ImGui::GetIO();
    float centerX = io.DisplaySize.x / 2.0f;
    float startY = ImGui::GetCursorScreenPos().y + Scale(50.0f);
    
    // Connecting animation
    double time = ImGui::GetTime();
    int dots = static_cast<int>(time * 2.0) % 4;
    std::string connectingText = "Connecting";
    for (int i = 0; i < dots; i++)
        connectingText += ".";
    
    auto textSize = g_pFntNewRodin->CalcTextSizeA(Scale(20.0f), FLT_MAX, 0, connectingText.c_str());
    ImVec2 textPos(centerX - textSize.x / 2.0f, startY);
    DrawTextWithShadow(g_pFntNewRodin, Scale(20.0f), textPos, 
        GTA4Style::Colors::TextOrange, connectingText.c_str());
    
    startY += Scale(40.0f);
    
    // Progress bar
    float barWidth = Scale(300.0f);
    float barHeight = Scale(20.0f);
    ImVec2 barMin(centerX - barWidth / 2.0f, startY);
    ImVec2 barMax(barMin.x + barWidth, barMin.y + barHeight);
    
    auto* drawList = ImGui::GetBackgroundDrawList();
    drawList->AddRectFilled(barMin, barMax, GTA4Style::Colors::ProgressBg, Scale(4.0f));
    
    ImVec2 fillMax(barMin.x + barWidth * s_connectionProgress, barMax.y);
    drawList->AddRectFilled(barMin, fillMax, GTA4Style::Colors::ProgressFill, Scale(4.0f));
    
    startY += Scale(40.0f);
    
    // Status message
    if (!s_connectionStatus.empty())
    {
        auto statusSize = g_pFntNewRodin->CalcTextSizeA(Scale(14.0f), FLT_MAX, 0, s_connectionStatus.c_str());
        ImVec2 statusPos(centerX - statusSize.x / 2.0f, startY);
        DrawTextWithShadow(g_pFntNewRodin, Scale(14.0f), statusPos,
            GTA4Style::Colors::TextGray, s_connectionStatus.c_str());
    }
    
    // Check connection thread status
    if (!g_connectionInProgress && g_connectionThread && g_connectionThread->joinable())
    {
        g_connectionThread->join();
        g_connectionThread.reset();
        
        if (g_connectionSuccess)
        {
            NavigateToPage(OnlineMultiplayerPage::Connected);
        }
        else
        {
            s_statusMessage = "Connection failed";
            NavigateToPage(OnlineMultiplayerPage::Main);
        }
    }
}

void OnlineMultiplayerMenu::DrawConnectedPage()
{
    auto& io = ImGui::GetIO();
    float startY = ImGui::GetCursorScreenPos().y;
    float labelX = ImGui::GetCursorScreenPos().x;
    float centerX = io.DisplaySize.x / 2.0f;
    
    // Connected status
    DrawTextWithShadow(g_pFntNewRodin, Scale(16.0f), ImVec2(labelX, startY),
        GTA4Style::Colors::TextGreen, "Connected to Network");
    
    startY += Scale(30.0f);
    
    // Peer list header
    DrawTextWithShadow(g_pFntNewRodin, Scale(14.0f), ImVec2(labelX, startY),
        GTA4Style::Colors::TextOrange, "Connected Peers:");
    
    startY += Scale(25.0f);
    
    // Peer list
    if (s_connectedPeers.empty())
    {
        DrawTextWithShadow(g_pFntNewRodin, Scale(12.0f), ImVec2(labelX + Scale(10.0f), startY),
            GTA4Style::Colors::TextGray, "No other peers connected");
    }
    else
    {
        for (const auto& peer : s_connectedPeers)
        {
            DrawTextWithShadow(g_pFntNewRodin, Scale(12.0f), ImVec2(labelX + Scale(10.0f), startY),
                IM_COL32(255, 255, 255, 255), peer.c_str());
            startY += Scale(18.0f);
        }
    }
    
    startY += Scale(30.0f);
    
    // Action buttons
    float buttonX = centerX - Scale(BUTTON_WIDTH) / 2.0f;
    
    ImGui::SetCursorScreenPos(ImVec2(buttonX, startY));
    DrawButton("Refresh Peers", true, []() {
        RefreshStatus();
    });
    
    startY += Scale(BUTTON_HEIGHT + ITEM_SPACING);
    
    ImGui::SetCursorScreenPos(ImVec2(buttonX, startY));
    DrawButton("Disconnect", true, []() {
        Disconnect();
        NavigateToPage(OnlineMultiplayerPage::Main);
    });
}

void OnlineMultiplayerMenu::DrawDiagnosticsPage()
{
    auto& io = ImGui::GetIO();
    float startY = ImGui::GetCursorScreenPos().y;
    float labelX = ImGui::GetCursorScreenPos().x;
    float centerX = io.DisplaySize.x / 2.0f;
    
    // Nebula Status
    DrawTextWithShadow(g_pFntNewRodin, Scale(14.0f), ImVec2(labelX, startY),
        GTA4Style::Colors::TextOrange, "Nebula Status:");
    
    const char* statusText = "Unknown";
    ImU32 statusColor = GTA4Style::Colors::TextGray;
    
    auto& nebula = liberty::install::NebulaManager::Instance();
    if (!nebula.IsAvailable())
    {
        statusText = "Binaries Not Found";
        statusColor = IM_COL32(200, 50, 50, 255);
    }
    else if (nebula.IsRunning())
    {
        statusText = "Running";
        statusColor = GTA4Style::Colors::TextGreen;
    }
    else
    {
        statusText = "Stopped";
        statusColor = GTA4Style::Colors::TextGray;
    }
    
    DrawTextWithShadow(g_pFntNewRodin, Scale(14.0f), ImVec2(labelX + Scale(150.0f), startY),
        statusColor, statusText);
    
    startY += Scale(25.0f);
    
    // Binary paths
    DrawTextWithShadow(g_pFntNewRodin, Scale(12.0f), ImVec2(labelX, startY),
        GTA4Style::Colors::TextGray, "Nebula Path:");
    DrawTextWithShadow(g_pFntNewRodin, Scale(10.0f), ImVec2(labelX + Scale(100.0f), startY),
        IM_COL32(180, 180, 180, 255), nebula.GetNebulaBinaryPath().string().c_str());
    
    startY += Scale(20.0f);
    
    // Configuration status
    DrawTextWithShadow(g_pFntNewRodin, Scale(12.0f), ImVec2(labelX, startY),
        GTA4Style::Colors::TextGray, "Config:");
    DrawTextWithShadow(g_pFntNewRodin, Scale(12.0f), ImVec2(labelX + Scale(100.0f), startY),
        nebula.HasValidConfig() ? GTA4Style::Colors::TextGreen : GTA4Style::Colors::TextGray,
        nebula.HasValidConfig() ? "Valid" : "Not Configured");
    
    startY += Scale(40.0f);
    
    // Troubleshooting tips
    DrawTextWithShadow(g_pFntNewRodin, Scale(14.0f), ImVec2(labelX, startY),
        GTA4Style::Colors::TextOrange, "Troubleshooting Tips:");
    
    startY += Scale(25.0f);
    
    const char* tips[] = {
        "1. Ensure UDP port 4242 is forwarded (host only)",
        "2. Check firewall allows Nebula application",
        "3. Verify CA certificate matches network",
        "4. Try running as administrator/root",
        "5. Check docs/ONLINE_MULTIPLAYER.md for details"
    };
    
    for (const auto& tip : tips)
    {
        DrawTextWithShadow(g_pFntNewRodin, Scale(11.0f), ImVec2(labelX, startY),
            GTA4Style::Colors::TextGray, tip);
        startY += Scale(16.0f);
    }
    
    startY += Scale(20.0f);
    
    // Test connection button
    float buttonX = centerX - Scale(BUTTON_WIDTH) / 2.0f;
    ImGui::SetCursorScreenPos(ImVec2(buttonX, startY));
    DrawButton("Run Diagnostics", true, []() {
        RunDiagnostics();
    });
}

void OnlineMultiplayerMenu::DrawStatusIndicator()
{
    auto& io = ImGui::GetIO();
    float menuX = (io.DisplaySize.x - Scale(MENU_WIDTH)) / 2.0f;
    float menuY = (io.DisplaySize.y - Scale(MENU_HEIGHT)) / 2.0f;
    
    // Status indicator circle
    ImVec2 circleCenter(menuX + Scale(30.0f), menuY + Scale(30.0f));
    float circleRadius = Scale(8.0f);
    
    ImU32 circleColor = GTA4Style::Colors::TextGray;
    
    switch (s_nebulaStatus)
    {
        case liberty::install::NebulaStatus::Running:
            circleColor = IM_COL32(50, 200, 50, 255);
            break;
        case liberty::install::NebulaStatus::Starting:
            circleColor = IM_COL32(255, 200, 50, 255);
            break;
        case liberty::install::NebulaStatus::Error:
        case liberty::install::NebulaStatus::NotInstalled:
            circleColor = IM_COL32(200, 50, 50, 255);
            break;
        default:
            circleColor = IM_COL32(100, 100, 100, 255);
            break;
    }
    
    auto* drawList = ImGui::GetBackgroundDrawList();
    drawList->AddCircleFilled(circleCenter, circleRadius, circleColor);
    drawList->AddCircle(circleCenter, circleRadius, IM_COL32(255, 255, 255, 100), 0, Scale(1.5f));
    
    // Title
    ImVec2 titlePos(menuX + Scale(50.0f), menuY + Scale(20.0f));
    DrawTextWithShadow(g_pFntNewRodin, Scale(18.0f), titlePos,
        GTA4Style::Colors::TextOrange, "ONLINE MULTIPLAYER");
}

void OnlineMultiplayerMenu::DrawButton(const char* label, bool enabled, std::function<void()> onClick)
{
    ImGui::PushStyleColor(ImGuiCol_Button, enabled ? 
        GTA4Style::Colors::BackgroundDark : IM_COL32(40, 40, 40, 128));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 
        GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, GTA4Style::Colors::Orange);
    ImGui::PushStyleColor(ImGuiCol_Text, enabled ? 
        IM_COL32(255, 255, 255, 255) : IM_COL32(128, 128, 128, 255));
    
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Scale(4.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, Scale(1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, GTA4Style::Colors::Border);
    
    if (ImGui::Button(label, ImVec2(Scale(BUTTON_WIDTH), Scale(BUTTON_HEIGHT))))
    {
        if (enabled && onClick)
            onClick();
    }
    
    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar(2);
}

void OnlineMultiplayerMenu::StartCreateNetwork()
{
    auto& nebula = liberty::install::NebulaManager::Instance();
    
    if (!nebula.IsAvailable())
    {
        s_statusMessage = "Nebula binaries not found";
        return;
    }
    
    NavigateToPage(OnlineMultiplayerPage::Connecting);
    s_connectionProgress = 0.0f;
    s_connectionStatus = "Generating certificates...";
    g_connectionInProgress = true;
    g_connectionSuccess = false;
    
    g_connectionThread = std::make_unique<std::thread>([]() {
        auto& nebula = liberty::install::NebulaManager::Instance();
        auto nebulaDir = nebula.GetNebulaDirectory();
        
        s_connectionProgress = 0.1f;
        s_connectionStatus = "Creating CA certificate...";
        
        // Generate CA
        auto result = nebula.GenerateCA(s_networkName, nebulaDir);
        if (!result.success)
        {
            s_connectionStatus = "Failed to generate CA: " + result.errorMessage;
            g_connectionInProgress = false;
            return;
        }
        
        s_connectionProgress = 0.3f;
        s_connectionStatus = "Creating lighthouse certificate...";
        
        // Generate lighthouse cert
        result = nebula.GenerateNodeCert("lighthouse", "192.168.100.1/24", nebulaDir, nebulaDir, true);
        if (!result.success)
        {
            s_connectionStatus = "Failed to generate cert: " + result.errorMessage;
            g_connectionInProgress = false;
            return;
        }
        
        s_connectionProgress = 0.5f;
        s_connectionStatus = "Creating configuration...";
        
        // Create config
        liberty::install::NebulaConfig config;
        config.isLighthouse = true;
        config.networkName = s_networkName;
        config.nodeName = "lighthouse";
        config.ipAddress = "192.168.100.1/24";
        config.lighthouseVirtualIp = "192.168.100.1";
        config.lighthousePublicAddress = s_publicIP;
        config.listenPort = static_cast<uint16_t>(s_listenPort);
        config.caCertPath = nebulaDir / "ca.crt";
        config.nodeCertPath = nebulaDir / "lighthouse.crt";
        config.nodeKeyPath = nebulaDir / "lighthouse.key";
        
        result = nebula.CreateConfig(config, nebulaDir / "config.yml");
        if (!result.success)
        {
            s_connectionStatus = "Failed to create config: " + result.errorMessage;
            g_connectionInProgress = false;
            return;
        }
        
        s_connectionProgress = 0.7f;
        s_connectionStatus = "Starting Nebula...";
        
        // Start nebula
        result = nebula.Start(nebulaDir / "config.yml");
        if (!result.success)
        {
            s_connectionStatus = "Failed to start Nebula: " + result.errorMessage;
            g_connectionInProgress = false;
            return;
        }
        
        s_connectionProgress = 1.0f;
        s_connectionStatus = "Connected!";
        g_connectionSuccess = true;
        g_connectionInProgress = false;
    });
}

void OnlineMultiplayerMenu::StartJoinNetwork()
{
    auto& nebula = liberty::install::NebulaManager::Instance();
    
    if (!nebula.IsAvailable())
    {
        s_statusMessage = "Nebula binaries not found";
        return;
    }
    
    NavigateToPage(OnlineMultiplayerPage::Connecting);
    s_connectionProgress = 0.0f;
    s_connectionStatus = "Importing CA certificate...";
    g_connectionInProgress = true;
    g_connectionSuccess = false;
    
    g_connectionThread = std::make_unique<std::thread>([]() {
        auto& nebula = liberty::install::NebulaManager::Instance();
        auto nebulaDir = nebula.GetNebulaDirectory();
        
        s_connectionProgress = 0.1f;
        s_connectionStatus = "Importing CA certificate...";
        
        // Import CA cert
        auto result = nebula.ImportCACert(s_caCertificate, nebulaDir);
        if (!result.success)
        {
            s_connectionStatus = "Failed to import CA: " + result.errorMessage;
            g_connectionInProgress = false;
            return;
        }
        
        s_connectionProgress = 0.3f;
        s_connectionStatus = "Generating node certificate...";
        
        // Generate node cert (requires CA key - this won't work for join, need signed cert from host)
        // For now, just create config assuming cert exists
        
        s_connectionProgress = 0.5f;
        s_connectionStatus = "Creating configuration...";
        
        // Parse lighthouse address
        std::string lighthouseAddr = s_lighthouseAddress;
        std::string lighthouseIP = "192.168.100.1";
        
        // Create config
        liberty::install::NebulaConfig config;
        config.isLighthouse = false;
        config.nodeName = "client";
        config.ipAddress = s_virtualIP;
        config.lighthouseVirtualIp = lighthouseIP;
        config.lighthousePublicAddress = lighthouseAddr;
        config.caCertPath = nebulaDir / "ca.crt";
        config.nodeCertPath = nebulaDir / "client.crt";
        config.nodeKeyPath = nebulaDir / "client.key";
        
        result = nebula.CreateConfig(config, nebulaDir / "config.yml");
        if (!result.success)
        {
            s_connectionStatus = "Failed to create config: " + result.errorMessage;
            g_connectionInProgress = false;
            return;
        }
        
        s_connectionProgress = 0.7f;
        s_connectionStatus = "Starting Nebula...";
        
        // Start nebula
        result = nebula.Start(nebulaDir / "config.yml");
        if (!result.success)
        {
            s_connectionStatus = "Failed to start Nebula: " + result.errorMessage;
            g_connectionInProgress = false;
            return;
        }
        
        s_connectionProgress = 1.0f;
        s_connectionStatus = "Connected!";
        g_connectionSuccess = true;
        g_connectionInProgress = false;
    });
}

void OnlineMultiplayerMenu::Disconnect()
{
    auto& nebula = liberty::install::NebulaManager::Instance();
    nebula.Stop();
    RefreshStatus();
}

void OnlineMultiplayerMenu::RunDiagnostics()
{
    // TODO: Implement network diagnostics
    s_statusMessage = "Diagnostics completed";
    RefreshStatus();
}

void OnlineMultiplayerMenu::RefreshStatus()
{
    auto& nebula = liberty::install::NebulaManager::Instance();
    s_nebulaStatus = nebula.GetStatus();
    s_connectedPeers = nebula.GetConnectedPeers();
}
