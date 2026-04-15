#pragma once

#include <api/Liberty.h>
#include <ui/common_menu.h>

// Main Menu for LibertyRecomp - GTA IV Style launcher
// Inspired by Banjo-Recompiled main menu but using GTA IV theming

enum class MainMenuState
{
    Opening,    // Menu is opening with animation
    Idle,       // Menu is visible and interactive
    Closing,    // Menu is closing with animation
    Hidden      // Menu is not visible
};

enum class MainMenuPage
{
    Main,           // Start Game, Controls, Settings, Mods, Exit
    Settings,       // Settings panel with tabs
    Controls,       // Controls configuration
    Mods,           // Mod browser/manager
    InstallDLC,     // Install DLC content
    Reinstall       // Reinstall ROM/ISO
};

enum class SettingsTab
{
    General,
    Graphics,
    Controls,
    Sound,
    Network,
    Mods,
    Count
};

class MainMenu
{
public:
    // State
    static inline MainMenuState s_state = MainMenuState::Hidden;
    static inline MainMenuPage s_currentPage = MainMenuPage::Main;
    static inline SettingsTab s_currentSettingsTab = SettingsTab::General;
    
    // UI state
    static inline bool s_isVisible = false;
    static inline int s_selectedMainOption = 0;
    static inline int s_selectedSettingsOption = 0;
    static inline double s_stateTime = 0.0;
    static inline double s_animTime = 0.0;
    
    // Menu item counts
#if !defined(LIBERTY_RECOMP_EMBEDDED_ASSETS)
    static constexpr int MAIN_MENU_ITEMS = 6; // Start Game, Controls, Settings, Mods, Install/Update, Exit
#else
    static constexpr int MAIN_MENU_ITEMS = 5; // Start Game, Controls, Settings, Mods, Exit (no installer on consoles)
#endif
    
    // Version string
    static inline std::string s_versionString = "v1.0.0";
    
    // Initialize resources
    static void Init();
    
    // Cleanup resources
    static void Shutdown();
    
    // Draw the main menu
    static void Draw();
    
    // Open the main menu
    static void Open();
    
    // Close the main menu and start game
    static void Close();
    
    // Process input for menu navigation
    static void ProcessInput();
    
    // Handle main menu item selection
    static void OnMainMenuSelect(int index);
    
    // Check if menu can be closed
    static bool CanClose();
    
    // Run the menu loop (blocking until game starts or exit)
    static bool Run();
    
private:
    // Drawing helpers
    static void DrawBackground();
    static void DrawHeader();
    static void DrawMainMenuPage();
    static void DrawSettingsPage();
    static void DrawControlsPage();
    static void DrawModsPage();
    static void DrawInstallPage();
    
    // Settings tab drawing
    static void DrawGeneralSettings(float contentX, float contentY, float contentWidth, float contentHeight);
    static void DrawGraphicsSettings(float contentX, float contentY, float contentWidth, float contentHeight);
    static void DrawControlsSettings(float contentX, float contentY, float contentWidth, float contentHeight);
    static void DrawSoundSettings(float contentX, float contentY, float contentWidth, float contentHeight);
    static void DrawNetworkSettings(float contentX, float contentY, float contentWidth, float contentHeight);
    static void DrawModsSettings(float contentX, float contentY, float contentWidth, float contentHeight);
    
    // UI helpers
    static void DrawMenuButton(const char* label, bool selected, float x, float y, float width, float height, bool* pressed);
    static void DrawSlider(const char* label, float* value, float minVal, float maxVal, float x, float y, float width);
    static void DrawToggle(const char* label, bool* value, float x, float y, float width);
    static void DrawDropdown(const char* label, int* selectedIndex, const char** options, int optionCount, float x, float y, float width);
    static void DrawSettingsTab(const char* label, SettingsTab tab, float x, float y, float width, float height);
};

