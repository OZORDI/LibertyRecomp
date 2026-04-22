#include "main_menu.h"

#include <imgui.h>
#if !REX_PLATFORM_CONSOLE
#include <SDL3/SDL.h>
#endif

#include <gpu/video.h>
#include <gpu/imgui/imgui_snapshot.h>
#include <hid/hid.h>
#include <locale/locale.h>
#include <ui/imgui_utils.h>
#include <ui/button_window.h>
#include <ui/message_window.h>
#include <ui/game_window.h>
#include <ui/gta4_style.h>
#if !REX_PLATFORM_CONSOLE
#include <ui/installer_wizard.h>
#endif
#include <user/config.h>
#include <user/paths.h>
#if !REX_PLATFORM_CONSOLE
#include <sdl_listener.h>
#endif
#include <app.h>
#include <exports.h>
#include <version.h>

// Animation constants
static constexpr double MENU_FADE_DURATION = 0.3;
static constexpr double ITEM_STAGGER_DELAY = 0.05;

// Layout constants
static constexpr float MENU_LEFT_MARGIN = 100.0f;
static constexpr float MENU_TOP_MARGIN = 200.0f;
static constexpr float MENU_ITEM_HEIGHT = 50.0f;
static constexpr float MENU_ITEM_SPACING = 10.0f;
static constexpr float SETTINGS_PANEL_WIDTH = 800.0f;
static constexpr float SETTINGS_PANEL_HEIGHT = 500.0f;
static constexpr float SETTINGS_TAB_HEIGHT = 40.0f;
static constexpr float SETTINGS_CONTENT_PADDING = 20.0f;
static constexpr float SLIDER_HEIGHT = 30.0f;
static constexpr float TOGGLE_HEIGHT = 30.0f;

// Menu item labels
static const char* g_mainMenuItems[] = {
    "Start Game",
    "Controls",
    "Settings",
    "Mods",
#if !defined(LIBERTY_RECOMP_EMBEDDED_ASSETS)
    "Install DLCs / Updates",
#endif
    "Exit"
};

// Settings tab labels
static const char* g_settingsTabNames[] = {
    "General",
    "Graphics",
    "Controls",
    "Sound",
    "Network",
    "Mods"
};

// Input state
static bool g_upPressed = false;
static bool g_downPressed = false;
static bool g_leftPressed = false;
static bool g_rightPressed = false;
static bool g_acceptPressed = false;
static bool g_backPressed = false;
static double g_lastInputTime = 0.0;

// Settings scroll state
static float g_settingsScrollY = 0.0f;
static int g_settingsOptionIndex = 0;
static int g_settingsOptionCount = 0;

// Pending changes
static bool g_pendingChanges = false;

// SDL Event Listener for Main Menu
#if !REX_PLATFORM_CONSOLE
class MainMenuEventListener : public SDLEventListener
{
public:
    bool OnSDLEvent(SDL_Event* event) override
    {
        if (!MainMenu::s_isVisible)
            return false;

        switch (event->type)
        {
            case SDL_EVENT_KEY_DOWN:
            {
                switch (event->key.scancode)
                {
                    case SDL_SCANCODE_UP:
                    case SDL_SCANCODE_W:
                        g_upPressed = true;
                        return true;
                    case SDL_SCANCODE_DOWN:
                    case SDL_SCANCODE_S:
                        g_downPressed = true;
                        return true;
                    case SDL_SCANCODE_LEFT:
                    case SDL_SCANCODE_A:
                        g_leftPressed = true;
                        return true;
                    case SDL_SCANCODE_RIGHT:
                    case SDL_SCANCODE_D:
                        g_rightPressed = true;
                        return true;
                    case SDL_SCANCODE_RETURN:
                    case SDL_SCANCODE_KP_ENTER:
                    case SDL_SCANCODE_SPACE:
                        g_acceptPressed = true;
                        return true;
                    case SDL_SCANCODE_ESCAPE:
                    case SDL_SCANCODE_BACKSPACE:
                        g_backPressed = true;
                        return true;
                }
                break;
            }
            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            {
                switch (event->gbutton.button)
                {
                    case SDL_GAMEPAD_BUTTON_DPAD_UP:
                        g_upPressed = true;
                        return true;
                    case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
                        g_downPressed = true;
                        return true;
                    case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
                        g_leftPressed = true;
                        return true;
                    case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
                        g_rightPressed = true;
                        return true;
                    case SDL_GAMEPAD_BUTTON_SOUTH:
                        g_acceptPressed = true;
                        return true;
                    case SDL_GAMEPAD_BUTTON_EAST:
                        g_backPressed = true;
                        return true;
                }
                break;
            }
        }
        
        return false;
    }
};

static MainMenuEventListener g_eventListener;
#endif

void MainMenu::Init()
{
    // Set version string using global version variable
    s_versionString = g_versionString ? g_versionString : "v1.0.0";
    
    // Event listener auto-registers in constructor
}

void MainMenu::Shutdown()
{
    // Event listener auto-unregisters in destructor
}

void MainMenu::Open()
{
    s_state = MainMenuState::Opening;
    s_isVisible = true;
    s_currentPage = MainMenuPage::Main;
    s_selectedMainOption = 0;
    s_stateTime = ImGui::GetTime();
    s_animTime = ImGui::GetTime();
    
#if !REX_PLATFORM_CONSOLE
    GameWindow::SetFullscreenCursorVisibility(true);
#endif
}

void MainMenu::Close()
{
    s_state = MainMenuState::Closing;
    s_stateTime = ImGui::GetTime();
    
    // Save any pending config changes
    if (g_pendingChanges)
    {
        Config::Save();
        g_pendingChanges = false;
    }
}

bool MainMenu::CanClose()
{
    return s_state != MainMenuState::Closing;
}

void MainMenu::ProcessInput()
{
    double currentTime = ImGui::GetTime();
    
    // Handle navigation based on current page
    if (s_currentPage == MainMenuPage::Main)
    {
        // Main menu navigation
        if (g_upPressed)
        {
            s_selectedMainOption--;
            if (s_selectedMainOption < 0)
                s_selectedMainOption = MAIN_MENU_ITEMS - 1;
            Game_PlaySound("move");
            g_upPressed = false;
        }
        
        if (g_downPressed)
        {
            s_selectedMainOption++;
            if (s_selectedMainOption >= MAIN_MENU_ITEMS)
                s_selectedMainOption = 0;
            Game_PlaySound("move");
            g_downPressed = false;
        }
        
        if (g_acceptPressed)
        {
            OnMainMenuSelect(s_selectedMainOption);
            g_acceptPressed = false;
        }
        
        if (g_backPressed)
        {
            // On main menu, back = Exit
            s_selectedMainOption = MAIN_MENU_ITEMS - 1; // Select Exit
            g_backPressed = false;
        }
    }
    else if (s_currentPage == MainMenuPage::Settings)
    {
        // Settings navigation
        if (g_leftPressed)
        {
            // Previous tab
            int tabIndex = (int)s_currentSettingsTab;
            tabIndex--;
            if (tabIndex < 0)
                tabIndex = (int)SettingsTab::Count - 1;
            s_currentSettingsTab = (SettingsTab)tabIndex;
            g_settingsOptionIndex = 0;
            g_settingsScrollY = 0.0f;
            Game_PlaySound("move");
            g_leftPressed = false;
        }
        
        if (g_rightPressed)
        {
            // Next tab
            int tabIndex = (int)s_currentSettingsTab;
            tabIndex++;
            if (tabIndex >= (int)SettingsTab::Count)
                tabIndex = 0;
            s_currentSettingsTab = (SettingsTab)tabIndex;
            g_settingsOptionIndex = 0;
            g_settingsScrollY = 0.0f;
            Game_PlaySound("move");
            g_rightPressed = false;
        }
        
        if (g_upPressed)
        {
            g_settingsOptionIndex--;
            if (g_settingsOptionIndex < 0)
                g_settingsOptionIndex = g_settingsOptionCount > 0 ? g_settingsOptionCount - 1 : 0;
            Game_PlaySound("move");
            g_upPressed = false;
        }
        
        if (g_downPressed)
        {
            g_settingsOptionIndex++;
            if (g_settingsOptionIndex >= g_settingsOptionCount)
                g_settingsOptionIndex = 0;
            Game_PlaySound("move");
            g_downPressed = false;
        }
        
        if (g_backPressed)
        {
            // Return to main menu
            s_currentPage = MainMenuPage::Main;
            Config::Save(); // Save changes when leaving settings
            g_pendingChanges = false;
            Game_PlaySound("cursor2");
            g_backPressed = false;
        }
    }
    else
    {
        // Other pages - back returns to main
        if (g_backPressed)
        {
            s_currentPage = MainMenuPage::Main;
            Game_PlaySound("cursor2");
            g_backPressed = false;
        }
    }
    
    // Clear any remaining input flags
    g_upPressed = g_downPressed = g_leftPressed = g_rightPressed = false;
    g_acceptPressed = g_backPressed = false;
}

void MainMenu::OnMainMenuSelect(int index)
{
    Game_PlaySound("main_deside");
    
    switch (index)
    {
        case 0: // Start Game
            Close();
            break;
            
        case 1: // Controls
            s_currentPage = MainMenuPage::Controls;
            break;
            
        case 2: // Settings
            s_currentPage = MainMenuPage::Settings;
            s_currentSettingsTab = SettingsTab::General;
            g_settingsOptionIndex = 0;
            g_settingsScrollY = 0.0f;
            break;
            
        case 3: // Mods
            s_currentPage = MainMenuPage::Mods;
            break;

#if !defined(LIBERTY_RECOMP_EMBEDDED_ASSETS)
        case 4: // Install / Update
            s_currentPage = MainMenuPage::InstallDLC;
            break;

        case 5: // Exit
#else
        case 4: // Exit (no Install/Update on console builds)
#endif
            // Exit application
            s_state = MainMenuState::Closing;
            s_stateTime = ImGui::GetTime();
            break;
    }
}

void MainMenu::DrawBackground()
{
    auto& res = ImGui::GetIO().DisplaySize;
    auto drawList = ImGui::GetBackgroundDrawList();
    
    // Calculate fade alpha
    float alpha = 1.0f;
    if (s_state == MainMenuState::Opening)
    {
        double elapsed = ImGui::GetTime() - s_stateTime;
        alpha = std::min(1.0f, (float)(elapsed / MENU_FADE_DURATION));
    }
    else if (s_state == MainMenuState::Closing)
    {
        double elapsed = ImGui::GetTime() - s_stateTime;
        alpha = std::max(0.0f, 1.0f - (float)(elapsed / MENU_FADE_DURATION));
    }
    
    // GTA IV Style: Dark background with subtle gradient
    ImU32 bgTop = GTA4Style::WithAlpha(GTA4Style::Colors::BackgroundDark, alpha);
    ImU32 bgBottom = GTA4Style::WithAlpha(GTA4Style::Colors::Background, alpha);
    drawList->AddRectFilledMultiColor({0, 0}, res, bgTop, bgTop, bgBottom, bgBottom);
    
    // Orange accent line at top
    float lineHeight = Scale(3);
    drawList->AddRectFilled({0, 0}, {res.x, lineHeight}, GTA4Style::WithAlpha(GTA4Style::Colors::Orange, alpha));
    
    // Draw version in bottom-left
    float versionFontSize = Scale(14.0f);
    ImVec2 versionPos = {Scale(20.0f), res.y - Scale(30.0f)};
    drawList->AddText(g_pFntRodin, versionFontSize, versionPos, 
                      GTA4Style::WithAlpha(GTA4Style::Colors::TextGray, alpha * 0.6f), 
                      s_versionString.c_str());
}

void MainMenu::DrawHeader()
{
    auto& res = ImGui::GetIO().DisplaySize;
    auto drawList = ImGui::GetBackgroundDrawList();
    
    float alpha = 1.0f;
    if (s_state == MainMenuState::Opening || s_state == MainMenuState::Closing)
    {
        double elapsed = ImGui::GetTime() - s_stateTime;
        if (s_state == MainMenuState::Opening)
            alpha = std::min(1.0f, (float)(elapsed / MENU_FADE_DURATION));
        else
            alpha = std::max(0.0f, 1.0f - (float)(elapsed / MENU_FADE_DURATION));
    }
    
    // Draw title: "LIBERTY RECOMP"
    const char* title = "LIBERTY RECOMP";
    float titleFontSize = Scale(48.0f);
    ImVec2 titleSize = g_pFntNewRodin->CalcTextSizeA(titleFontSize, FLT_MAX, 0.0f, title);
    float titleX = Scale(MENU_LEFT_MARGIN);
    float titleY = Scale(80.0f);
    
    // Orange shadow/glow effect
    drawList->AddText(g_pFntNewRodin, titleFontSize, {titleX + 2, titleY + 2}, 
                      GTA4Style::WithAlpha(IM_COL32(100, 50, 0, 150), alpha), title);
    drawList->AddText(g_pFntNewRodin, titleFontSize, {titleX, titleY}, 
                      GTA4Style::WithAlpha(GTA4Style::Colors::Orange, alpha), title);
    
#if !REX_PLATFORM_CONSOLE
    // Debug: Show mouse position (desktop only; consoles have no mouse)
    auto& io = ImGui::GetIO();
    char mouseDebug[128];
    snprintf(mouseDebug, sizeof(mouseDebug), "Mouse: %.0f, %.0f | Display: %.0f x %.0f",
             io.MousePos.x, io.MousePos.y, io.DisplaySize.x, io.DisplaySize.y);
    drawList->AddText(g_pFntRodin, Scale(14.0f), {titleX, titleY + titleSize.y + Scale(10.0f)},
                      GTA4Style::WithAlpha(GTA4Style::Colors::TextGray, alpha), mouseDebug);
#endif
}

void MainMenu::DrawMenuButton(const char* label, bool selected, float x, float y, float width, float height, bool* pressed)
{
    auto drawList = ImGui::GetBackgroundDrawList();
    
    float alpha = 1.0f;
    if (s_state == MainMenuState::Opening || s_state == MainMenuState::Closing)
    {
        double elapsed = ImGui::GetTime() - s_stateTime;
        if (s_state == MainMenuState::Opening)
            alpha = std::min(1.0f, (float)(elapsed / MENU_FADE_DURATION));
        else
            alpha = std::max(0.0f, 1.0f - (float)(elapsed / MENU_FADE_DURATION));
    }
    
    ImVec2 min = {x, y};
    ImVec2 max = {x + width, y + height};
    
    // Background for selected item
    if (selected)
    {
        // Animated selection highlight
        float pulseAlpha = 0.8f + 0.2f * sinf((float)ImGui::GetTime() * 3.0f);
        ImU32 bgColor = GTA4Style::WithAlpha(IM_COL32(255, 150, 0, 40), alpha * pulseAlpha);
        drawList->AddRectFilled(min, max, bgColor);
        
        // Orange left border indicator
        float indicatorWidth = Scale(4.0f);
        drawList->AddRectFilled(min, {min.x + indicatorWidth, max.y}, 
                                GTA4Style::WithAlpha(GTA4Style::Colors::Orange, alpha));
    }
    
    // Text
    float fontSize = Scale(24.0f);
    ImVec2 textSize = g_pFntNewRodin->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, label);
    float textX = x + Scale(20.0f);
    float textY = y + (height - textSize.y) / 2.0f;
    
    ImU32 textColor = selected 
        ? GTA4Style::WithAlpha(GTA4Style::Colors::Orange, alpha)
        : GTA4Style::WithAlpha(IM_COL32(200, 200, 200, 255), alpha);
    
    drawList->AddText(g_pFntNewRodin, fontSize, {textX, textY}, textColor, label);
    
    // Check for mouse click
    if (pressed)
    {
        *pressed = false;
        if (ImGui::IsMouseHoveringRect(min, max))
        {
            if (ImGui::IsMouseClicked(0))
                *pressed = true;
        }
    }
}

void MainMenu::DrawMainMenuPage()
{
    auto& res = ImGui::GetIO().DisplaySize;
    
    float menuX = Scale(MENU_LEFT_MARGIN);
    float menuY = Scale(MENU_TOP_MARGIN);
    float itemWidth = Scale(300.0f);
    float itemHeight = Scale(MENU_ITEM_HEIGHT);
    float itemSpacing = Scale(MENU_ITEM_SPACING);
    
    for (int i = 0; i < MAIN_MENU_ITEMS; i++)
    {
        bool selected = (i == s_selectedMainOption);
        bool pressed = false;
        
        // Stagger animation
        float itemAlpha = 1.0f;
        if (s_state == MainMenuState::Opening)
        {
            double elapsed = ImGui::GetTime() - s_stateTime - (i * ITEM_STAGGER_DELAY);
            itemAlpha = std::max(0.0f, std::min(1.0f, (float)(elapsed / MENU_FADE_DURATION)));
        }
        
        float itemY = menuY + i * (itemHeight + itemSpacing);
        DrawMenuButton(g_mainMenuItems[i], selected, menuX, itemY, itemWidth, itemHeight, &pressed);
        
        // Handle mouse selection
        ImVec2 min = {menuX, itemY};
        ImVec2 max = {menuX + itemWidth, itemY + itemHeight};
        if (ImGui::IsMouseHoveringRect(min, max))
        {
            if (s_selectedMainOption != i)
            {
                s_selectedMainOption = i;
                Game_PlaySound("move");
            }
            if (ImGui::IsMouseClicked(0))
            {
                OnMainMenuSelect(i);
            }
        }
    }
    
    // Navigation hints at bottom
    auto drawList = ImGui::GetBackgroundDrawList();
    float hintY = res.y - Scale(60.0f);
    float hintFontSize = Scale(14.0f);
    const char* hint = "UP/DOWN: Navigate  |  ENTER: Select  |  ESC: Back";
    ImVec2 hintSize = g_pFntRodin->CalcTextSizeA(hintFontSize, FLT_MAX, 0.0f, hint);
    drawList->AddText(g_pFntRodin, hintFontSize, {(res.x - hintSize.x) / 2.0f, hintY}, 
                      GTA4Style::WithAlpha(GTA4Style::Colors::TextGray, 0.6f), hint);
}

void MainMenu::DrawSettingsTab(const char* label, SettingsTab tab, float x, float y, float width, float height)
{
    auto drawList = ImGui::GetBackgroundDrawList();
    
    bool selected = (s_currentSettingsTab == tab);
    ImVec2 min = {x, y};
    ImVec2 max = {x + width, y + height};
    
    // Tab background
    if (selected)
    {
        drawList->AddRectFilled(min, max, GTA4Style::Colors::Orange);
        // White text on orange
        float fontSize = Scale(16.0f);
        ImVec2 textSize = g_pFntRodin->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, label);
        float textX = x + (width - textSize.x) / 2.0f;
        float textY = y + (height - textSize.y) / 2.0f;
        drawList->AddText(g_pFntRodin, fontSize, {textX, textY}, IM_COL32(0, 0, 0, 255), label);
    }
    else
    {
        drawList->AddRectFilled(min, max, GTA4Style::Colors::BackgroundPanel);
        drawList->AddRect(min, max, GTA4Style::Colors::Border);
        // Gray text
        float fontSize = Scale(16.0f);
        ImVec2 textSize = g_pFntRodin->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, label);
        float textX = x + (width - textSize.x) / 2.0f;
        float textY = y + (height - textSize.y) / 2.0f;
        drawList->AddText(g_pFntRodin, fontSize, {textX, textY}, GTA4Style::Colors::TextGray, label);
    }
    
    // Handle click
    if (ImGui::IsMouseHoveringRect(min, max) && ImGui::IsMouseClicked(0))
    {
        s_currentSettingsTab = tab;
        g_settingsOptionIndex = 0;
        g_settingsScrollY = 0.0f;
        Game_PlaySound("move");
    }
}

void MainMenu::DrawSlider(const char* label, float* value, float minVal, float maxVal, float x, float y, float width)
{
    auto drawList = ImGui::GetBackgroundDrawList();
    float height = Scale(SLIDER_HEIGHT);
    
    // Label
    float fontSize = Scale(16.0f);
    drawList->AddText(g_pFntRodin, fontSize, {x, y}, IM_COL32(255, 255, 255, 255), label);
    
    // Slider track
    float trackY = y + Scale(22.0f);
    float trackHeight = Scale(8.0f);
    float trackWidth = width * 0.6f;
    float trackX = x + width - trackWidth;
    
    ImVec2 trackMin = {trackX, trackY};
    ImVec2 trackMax = {trackX + trackWidth, trackY + trackHeight};
    drawList->AddRectFilled(trackMin, trackMax, GTA4Style::Colors::ProgressBg);
    
    // Slider fill
    float normalizedValue = (*value - minVal) / (maxVal - minVal);
    float fillWidth = trackWidth * normalizedValue;
    drawList->AddRectFilled(trackMin, {trackMin.x + fillWidth, trackMax.y}, GTA4Style::Colors::Orange);
    
    // Value display
    char valueStr[32];
    snprintf(valueStr, sizeof(valueStr), "%.0f%%", normalizedValue * 100.0f);
    ImVec2 valueSize = g_pFntRodin->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, valueStr);
    drawList->AddText(g_pFntRodin, fontSize, {trackX - valueSize.x - Scale(10.0f), y}, 
                      GTA4Style::Colors::TextGray, valueStr);
}

void MainMenu::DrawToggle(const char* label, bool* value, float x, float y, float width)
{
    auto drawList = ImGui::GetBackgroundDrawList();
    
    float fontSize = Scale(16.0f);
    drawList->AddText(g_pFntRodin, fontSize, {x, y}, IM_COL32(255, 255, 255, 255), label);
    
    // Toggle indicators
    float toggleX = x + width - Scale(100.0f);
    const char* offText = "OFF";
    const char* onText = "ON";
    
    ImU32 offColor = *value ? GTA4Style::Colors::TextGray : GTA4Style::Colors::Orange;
    ImU32 onColor = *value ? GTA4Style::Colors::Orange : GTA4Style::Colors::TextGray;
    
    drawList->AddText(g_pFntRodin, fontSize, {toggleX, y}, offColor, offText);
    drawList->AddText(g_pFntRodin, fontSize, {toggleX + Scale(50.0f), y}, onColor, onText);
}

void MainMenu::DrawDropdown(const char* label, int* selectedIndex, const char** options, int optionCount, float x, float y, float width)
{
    auto drawList = ImGui::GetBackgroundDrawList();
    
    float fontSize = Scale(16.0f);
    drawList->AddText(g_pFntRodin, fontSize, {x, y}, IM_COL32(255, 255, 255, 255), label);
    
    // Display current selection with arrows
    const char* currentOption = (*selectedIndex >= 0 && *selectedIndex < optionCount) 
        ? options[*selectedIndex] 
        : "Unknown";
    
    // Position for selector
    float selectorX = x + width - Scale(200.0f);
    
    // Draw arrows and value
    drawList->AddText(g_pFntRodin, fontSize, {selectorX, y}, GTA4Style::Colors::TextGray, "<");
    
    ImVec2 optionSize = g_pFntRodin->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, currentOption);
    float optionX = selectorX + Scale(80.0f) - optionSize.x / 2.0f;
    drawList->AddText(g_pFntRodin, fontSize, {optionX, y}, GTA4Style::Colors::Orange, currentOption);
    
    drawList->AddText(g_pFntRodin, fontSize, {selectorX + Scale(160.0f), y}, GTA4Style::Colors::TextGray, ">");
}

void MainMenu::DrawGeneralSettings(float contentX, float contentY, float contentWidth, float contentHeight)
{
    auto drawList = ImGui::GetBackgroundDrawList();
    float y = contentY;
    float rowHeight = Scale(45.0f);
    g_settingsOptionCount = 0;
    
    // Rumble Strength
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        // Get rumble value from config if available (placeholder)
        float rumbleValue = 0.65f; // TODO: Link to Config::RumbleStrength
        DrawSlider("Rumble Strength", &rumbleValue, 0.0f, 1.0f, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
    
    // Joystick Deadzone
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        float deadzoneValue = 0.05f; // TODO: Link to config
        DrawSlider("Joystick Deadzone", &deadzoneValue, 0.0f, 0.5f, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
    
    // Background Input Mode
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        bool bgInput = Config::AllowBackgroundInput;
        DrawToggle("Background Input Mode", &bgInput, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
    
    // Subtitles
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        bool subtitles = Config::Subtitles;
        DrawToggle("Subtitles", &subtitles, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
    
    // Autosave
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        bool autosave = Config::Autosave;
        DrawToggle("Autosave", &autosave, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
}

void MainMenu::DrawGraphicsSettings(float contentX, float contentY, float contentWidth, float contentHeight)
{
    auto drawList = ImGui::GetBackgroundDrawList();
    float y = contentY;
    float rowHeight = Scale(45.0f);
    g_settingsOptionCount = 0;
    
    // Fullscreen
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        bool fullscreen = Config::Fullscreen;
        DrawToggle("Fullscreen", &fullscreen, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
    
    // VSync
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        bool vsync = Config::VSync;
        DrawToggle("VSync", &vsync, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
    
    // Brightness
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        float brightness = Config::Brightness;
        DrawSlider("Brightness", &brightness, 0.0f, 1.0f, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
    
    // Resolution Scale
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        float resScale = Config::ResolutionScale;
        DrawSlider("Resolution Scale", &resScale, 0.5f, 2.0f, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
    
    // Show FPS
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        bool showFps = Config::ShowFPS;
        DrawToggle("Show FPS", &showFps, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
}

void MainMenu::DrawControlsSettings(float contentX, float contentY, float contentWidth, float contentHeight)
{
    auto drawList = ImGui::GetBackgroundDrawList();
    float y = contentY;
    float rowHeight = Scale(45.0f);
    g_settingsOptionCount = 0;
    
    // Controller Icons
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        float fontSize = Scale(16.0f);
        drawList->AddText(g_pFntRodin, fontSize, {contentX + Scale(10.0f), y + Scale(8.0f)}, 
                          IM_COL32(255, 255, 255, 255), "Controller Icons");
        
        const char* iconType = "Auto"; // TODO: Get from config
        drawList->AddText(g_pFntRodin, fontSize, {contentX + contentWidth - Scale(150.0f), y + Scale(8.0f)}, 
                          GTA4Style::Colors::Orange, iconType);
        y += rowHeight;
        g_settingsOptionCount++;
    }
    
    // Mouse Sensitivity X
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        float mouseSensX = Config::MouseSensitivityX;
        DrawSlider("Mouse Sensitivity X", &mouseSensX, 0.1f, 3.0f, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
    
    // Mouse Sensitivity Y
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        float mouseSensY = Config::MouseSensitivityY;
        DrawSlider("Mouse Sensitivity Y", &mouseSensY, 0.1f, 3.0f, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
    
    // Invert Y
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        bool invertY = Config::MouseInvertY;
        DrawToggle("Invert Y Axis", &invertY, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
    
    // Motion Controls (PlayStation)
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        bool motionEnabled = Config::MotionControlsEnabled;
        DrawToggle("Motion Controls", &motionEnabled, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
}

void MainMenu::DrawSoundSettings(float contentX, float contentY, float contentWidth, float contentHeight)
{
    auto drawList = ImGui::GetBackgroundDrawList();
    float y = contentY;
    float rowHeight = Scale(45.0f);
    g_settingsOptionCount = 0;
    
    // Master Volume
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        float masterVol = Config::MasterVolume;
        DrawSlider("Master Volume", &masterVol, 0.0f, 1.0f, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
    
    // Music Volume
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        float musicVol = Config::MusicVolume;
        DrawSlider("Music Volume", &musicVol, 0.0f, 1.0f, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
    
    // Effects Volume
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        float effectsVol = Config::EffectsVolume;
        DrawSlider("Effects Volume", &effectsVol, 0.0f, 1.0f, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
    
    // Mute on Focus Lost
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
        
        bool muteOnFocus = Config::MuteOnFocusLost;
        DrawToggle("Mute on Focus Lost", &muteOnFocus, contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        y += rowHeight;
        g_settingsOptionCount++;
    }
}

// Multiplayer backend option names
static const char* g_multiplayerBackendOptions[] = {
    "Community",    // Default - uses community-hosted session tracker
    "Firebase",     // Self-hosted Firebase for private communities
    "LAN"           // LAN only - no internet matchmaking
};
static const int g_multiplayerBackendCount = 3;

// Flag to track if current setting is a dropdown (so left/right adjust value instead of changing tab)
static bool g_settingsIsDropdown = false;

void MainMenu::DrawNetworkSettings(float contentX, float contentY, float contentWidth, float contentHeight)
{
    auto drawList = ImGui::GetBackgroundDrawList();
    float y = contentY;
    float rowHeight = Scale(45.0f);
    float fontSize = Scale(16.0f);
    g_settingsOptionCount = 0;
    g_settingsIsDropdown = false;
    
    // Section header: Multiplayer
    drawList->AddText(g_pFntNewRodin, Scale(18.0f), {contentX + Scale(10.0f), y}, 
                      GTA4Style::Colors::Orange, "MULTIPLAYER");
    y += Scale(35.0f);
    
    // Multiplayer Backend
    {
        bool selected = (g_settingsOptionIndex == g_settingsOptionCount);
        if (selected)
        {
            g_settingsIsDropdown = true;
            drawList->AddRectFilled({contentX, y}, {contentX + contentWidth, y + rowHeight}, 
                                    GTA4Style::WithAlpha(GTA4Style::Colors::Orange, 0.2f));
            
            // Handle value change with Accept key or left/right while selected
            static bool acceptWasPressed = false;
            if (g_acceptPressed && !acceptWasPressed)
            {
                // Cycle forward
                int newIndex = ((int)Config::MultiplayerBackend.Value + 1) % g_multiplayerBackendCount;
                Config::MultiplayerBackend = (EMultiplayerBackend)newIndex;
                g_pendingChanges = true;
                Game_PlaySound("cursor2");
            }
            acceptWasPressed = g_acceptPressed;
        }
        
        int backendIndex = (int)Config::MultiplayerBackend.Value;
        DrawDropdown("Session Backend", &backendIndex, g_multiplayerBackendOptions, g_multiplayerBackendCount,
                     contentX + Scale(10.0f), y + Scale(8.0f), contentWidth - Scale(20.0f));
        
        // Show description based on current selection
        const char* description = "";
        switch ((EMultiplayerBackend)backendIndex)
        {
            case EMultiplayerBackend::Community:
                description = "Uses community-hosted servers for online matchmaking";
                break;
            case EMultiplayerBackend::Firebase:
                description = "Use your own Firebase backend for private communities";
                break;
            case EMultiplayerBackend::LAN:
                description = "Local network only - no internet required";
                break;
        }
        drawList->AddText(g_pFntRodin, Scale(12.0f), {contentX + Scale(20.0f), y + Scale(28.0f)}, 
                          GTA4Style::Colors::TextGray, description);
        
        y += rowHeight + Scale(10.0f);
        g_settingsOptionCount++;
    }
    
    // Note about changing backend
    y += Scale(20.0f);
    drawList->AddText(g_pFntRodin, Scale(14.0f), {contentX + Scale(10.0f), y}, 
                      GTA4Style::Colors::TextGray, "Note: Changing the backend requires a game restart.");
    y += Scale(25.0f);
    drawList->AddText(g_pFntRodin, Scale(14.0f), {contentX + Scale(10.0f), y}, 
                      GTA4Style::Colors::TextGray, "Press ENTER to cycle through options when highlighted.");
}

void MainMenu::DrawModsSettings(float contentX, float contentY, float contentWidth, float contentHeight)
{
    auto drawList = ImGui::GetBackgroundDrawList();
    float y = contentY;
    float fontSize = Scale(16.0f);
    
    // Placeholder for mod management
    drawList->AddText(g_pFntRodin, fontSize, {contentX + Scale(10.0f), y}, 
                      GTA4Style::Colors::TextGray, "Mod management coming soon...");
    y += Scale(30.0f);
    
    drawList->AddText(g_pFntRodin, fontSize, {contentX + Scale(10.0f), y}, 
                      GTA4Style::Colors::TextGray, "Place mods in the 'mods' folder");
    
    g_settingsOptionCount = 0;
}

void MainMenu::DrawSettingsPage()
{
    auto& res = ImGui::GetIO().DisplaySize;
    auto drawList = ImGui::GetBackgroundDrawList();
    
    // Center the settings panel
    float panelWidth = Scale(SETTINGS_PANEL_WIDTH);
    float panelHeight = Scale(SETTINGS_PANEL_HEIGHT);
    float panelX = (res.x - panelWidth) / 2.0f;
    float panelY = Scale(100.0f);
    
    // Panel background
    ImVec2 panelMin = {panelX, panelY};
    ImVec2 panelMax = {panelX + panelWidth, panelY + panelHeight};
    drawList->AddRectFilled(panelMin, panelMax, GTA4Style::Colors::BackgroundPanel);
    drawList->AddRect(panelMin, panelMax, GTA4Style::Colors::Border);
    
    // Draw tabs
    float tabWidth = panelWidth / (int)SettingsTab::Count;
    float tabHeight = Scale(SETTINGS_TAB_HEIGHT);
    for (int i = 0; i < (int)SettingsTab::Count; i++)
    {
        float tabX = panelX + i * tabWidth;
        DrawSettingsTab(g_settingsTabNames[i], (SettingsTab)i, tabX, panelY, tabWidth, tabHeight);
    }
    
    // Content area
    float contentX = panelX;
    float contentY = panelY + tabHeight + Scale(10.0f);
    float contentWidth = panelWidth;
    float contentHeight = panelHeight - tabHeight - Scale(20.0f);
    
    // Draw content based on current tab
    switch (s_currentSettingsTab)
    {
        case SettingsTab::General:
            DrawGeneralSettings(contentX, contentY, contentWidth, contentHeight);
            break;
        case SettingsTab::Graphics:
            DrawGraphicsSettings(contentX, contentY, contentWidth, contentHeight);
            break;
        case SettingsTab::Controls:
            DrawControlsSettings(contentX, contentY, contentWidth, contentHeight);
            break;
        case SettingsTab::Sound:
            DrawSoundSettings(contentX, contentY, contentWidth, contentHeight);
            break;
        case SettingsTab::Network:
            DrawNetworkSettings(contentX, contentY, contentWidth, contentHeight);
            break;
        case SettingsTab::Mods:
            DrawModsSettings(contentX, contentY, contentWidth, contentHeight);
            break;
        default:
            break;
    }
    
    // Close button / hint
    float hintY = panelMax.y + Scale(20.0f);
    float hintFontSize = Scale(14.0f);
    const char* hint = "LEFT/RIGHT: Change Tab  |  UP/DOWN: Navigate  |  ESC: Back";
    ImVec2 hintSize = g_pFntRodin->CalcTextSizeA(hintFontSize, FLT_MAX, 0.0f, hint);
    drawList->AddText(g_pFntRodin, hintFontSize, {(res.x - hintSize.x) / 2.0f, hintY}, 
                      GTA4Style::WithAlpha(GTA4Style::Colors::TextGray, 0.6f), hint);
}

void MainMenu::DrawControlsPage()
{
    auto& res = ImGui::GetIO().DisplaySize;
    auto drawList = ImGui::GetBackgroundDrawList();
    
    float centerX = res.x / 2.0f;
    float fontSize = Scale(20.0f);
    
    // Title
    const char* title = "CONTROLS";
    ImVec2 titleSize = g_pFntNewRodin->CalcTextSizeA(Scale(32.0f), FLT_MAX, 0.0f, title);
    drawList->AddText(g_pFntNewRodin, Scale(32.0f), {centerX - titleSize.x / 2.0f, Scale(100.0f)}, 
                      GTA4Style::Colors::Orange, title);
    
    // Placeholder content
    const char* message = "Control customization coming soon...";
    ImVec2 msgSize = g_pFntRodin->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, message);
    drawList->AddText(g_pFntRodin, fontSize, {centerX - msgSize.x / 2.0f, Scale(200.0f)}, 
                      GTA4Style::Colors::TextGray, message);
    
    // Navigation hint
    float hintY = res.y - Scale(60.0f);
    float hintFontSize = Scale(14.0f);
    const char* hint = "ESC: Back";
    ImVec2 hintSize = g_pFntRodin->CalcTextSizeA(hintFontSize, FLT_MAX, 0.0f, hint);
    drawList->AddText(g_pFntRodin, hintFontSize, {(res.x - hintSize.x) / 2.0f, hintY}, 
                      GTA4Style::WithAlpha(GTA4Style::Colors::TextGray, 0.6f), hint);
}

void MainMenu::DrawModsPage()
{
    auto& res = ImGui::GetIO().DisplaySize;
    auto drawList = ImGui::GetBackgroundDrawList();
    
    float centerX = res.x / 2.0f;
    float fontSize = Scale(20.0f);
    
    // Title
    const char* title = "MODS";
    ImVec2 titleSize = g_pFntNewRodin->CalcTextSizeA(Scale(32.0f), FLT_MAX, 0.0f, title);
    drawList->AddText(g_pFntNewRodin, Scale(32.0f), {centerX - titleSize.x / 2.0f, Scale(100.0f)}, 
                      GTA4Style::Colors::Orange, title);
    
    // Placeholder content
    const char* message = "Mod browser coming soon...";
    ImVec2 msgSize = g_pFntRodin->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, message);
    drawList->AddText(g_pFntRodin, fontSize, {centerX - msgSize.x / 2.0f, Scale(200.0f)}, 
                      GTA4Style::Colors::TextGray, message);
    
    const char* hint2 = "Place mods in the 'mods' folder in your game directory";
    ImVec2 hint2Size = g_pFntRodin->CalcTextSizeA(Scale(14.0f), FLT_MAX, 0.0f, hint2);
    drawList->AddText(g_pFntRodin, Scale(14.0f), {centerX - hint2Size.x / 2.0f, Scale(240.0f)}, 
                      GTA4Style::Colors::TextGray, hint2);
    
    // Navigation hint
    float hintY = res.y - Scale(60.0f);
    float hintFontSize = Scale(14.0f);
    const char* hint = "ESC: Back";
    ImVec2 hintSize = g_pFntRodin->CalcTextSizeA(hintFontSize, FLT_MAX, 0.0f, hint);
    drawList->AddText(g_pFntRodin, hintFontSize, {(res.x - hintSize.x) / 2.0f, hintY}, 
                      GTA4Style::WithAlpha(GTA4Style::Colors::TextGray, 0.6f), hint);
}

void MainMenu::DrawInstallPage()
{
    auto& res = ImGui::GetIO().DisplaySize;
    auto drawList = ImGui::GetBackgroundDrawList();
    
    float centerX = res.x / 2.0f;
    float fontSize = Scale(20.0f);
    float buttonWidth = Scale(300.0f);
    float buttonHeight = Scale(50.0f);
    float buttonSpacing = Scale(20.0f);
    float startY = Scale(180.0f);
    
    // Title
    const char* title = "INSTALL / UPDATE";
    ImVec2 titleSize = g_pFntNewRodin->CalcTextSizeA(Scale(32.0f), FLT_MAX, 0.0f, title);
    drawList->AddText(g_pFntNewRodin, Scale(32.0f), {centerX - titleSize.x / 2.0f, Scale(100.0f)}, 
                      GTA4Style::Colors::Orange, title);
    
    // Install DLC button
    {
        float buttonX = centerX - buttonWidth / 2.0f;
        float buttonY = startY;
        bool pressed = false;
        
        DrawMenuButton("Install DLC Content", g_settingsOptionIndex == 0, buttonX, buttonY, buttonWidth, buttonHeight, &pressed);
        
        if (ImGui::IsMouseHoveringRect({buttonX, buttonY}, {buttonX + buttonWidth, buttonY + buttonHeight}))
        {
            if (g_settingsOptionIndex != 0)
            {
                g_settingsOptionIndex = 0;
                Game_PlaySound("move");
            }
        }
    }
    
    // Reinstall ROM/ISO button
    {
        float buttonX = centerX - buttonWidth / 2.0f;
        float buttonY = startY + buttonHeight + buttonSpacing;
        bool pressed = false;
        
        DrawMenuButton("Reinstall ROM/ISO", g_settingsOptionIndex == 1, buttonX, buttonY, buttonWidth, buttonHeight, &pressed);
        
        if (ImGui::IsMouseHoveringRect({buttonX, buttonY}, {buttonX + buttonWidth, buttonY + buttonHeight}))
        {
            if (g_settingsOptionIndex != 1)
            {
                g_settingsOptionIndex = 1;
                Game_PlaySound("move");
            }
        }
    }
    
    // Check for Updates button
    {
        float buttonX = centerX - buttonWidth / 2.0f;
        float buttonY = startY + 2 * (buttonHeight + buttonSpacing);
        bool pressed = false;
        
        DrawMenuButton("Check for Updates", g_settingsOptionIndex == 2, buttonX, buttonY, buttonWidth, buttonHeight, &pressed);
        
        if (ImGui::IsMouseHoveringRect({buttonX, buttonY}, {buttonX + buttonWidth, buttonY + buttonHeight}))
        {
            if (g_settingsOptionIndex != 2)
            {
                g_settingsOptionIndex = 2;
                Game_PlaySound("move");
            }
        }
    }
    
    g_settingsOptionCount = 3;
    
    // Navigation hint
    float hintY = res.y - Scale(60.0f);
    float hintFontSize = Scale(14.0f);
    const char* hint = "UP/DOWN: Navigate  |  ENTER: Select  |  ESC: Back";
    ImVec2 hintSize = g_pFntRodin->CalcTextSizeA(hintFontSize, FLT_MAX, 0.0f, hint);
    drawList->AddText(g_pFntRodin, hintFontSize, {(res.x - hintSize.x) / 2.0f, hintY}, 
                      GTA4Style::WithAlpha(GTA4Style::Colors::TextGray, 0.6f), hint);
}

void MainMenu::Draw()
{
    if (!s_isVisible)
        return;
    
    // Update state
    if (s_state == MainMenuState::Opening)
    {
        double elapsed = ImGui::GetTime() - s_stateTime;
        if (elapsed >= MENU_FADE_DURATION)
        {
            s_state = MainMenuState::Idle;
        }
    }
    else if (s_state == MainMenuState::Closing)
    {
        double elapsed = ImGui::GetTime() - s_stateTime;
        if (elapsed >= MENU_FADE_DURATION)
        {
            s_isVisible = false;
            s_state = MainMenuState::Hidden;
            return;
        }
    }
    
    // Process input
    ProcessInput();
    
    // Draw layers
    DrawBackground();
    DrawHeader();
    
    // Draw current page
    switch (s_currentPage)
    {
        case MainMenuPage::Main:
            DrawMainMenuPage();
            break;
        case MainMenuPage::Settings:
            DrawSettingsPage();
            break;
        case MainMenuPage::Controls:
            DrawControlsPage();
            break;
        case MainMenuPage::Mods:
            DrawModsPage();
            break;
        case MainMenuPage::InstallDLC:
        case MainMenuPage::Reinstall:
            DrawInstallPage();
            break;
    }
}

bool MainMenu::Run()
{
    Open();
    
    // Guarantee one controller is initialized
    XAMINPUT_STATE inputState;
    hid::GetState(0, &inputState);
    
    // Main menu loop
    while (s_isVisible)
    {
        Video::WaitOnSwapChain();
#if !REX_PLATFORM_CONSOLE
        SDL_PumpEvents();
        SDL_FlushEvents(SDL_EVENT_FIRST, SDL_EVENT_LAST);
#endif
        GameWindow::Update();
        Video::Present();
        
        // Check if user selected Exit
        if (s_state == MainMenuState::Hidden)
        {
            break;
        }
    }
    
#if !REX_PLATFORM_CONSOLE
    GameWindow::SetFullscreenCursorVisibility(false);
#endif
    
    // Return true if game should start, false if exit was selected
    return (s_selectedMainOption == 0); // Start Game was selected
}

