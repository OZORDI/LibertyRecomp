#include "installer_wizard.h"

#include <nfd.h>

#include <apu/embedded_player.h>
#include <install/installer.h>
#include <install/xbox360/title_update_manager.h>
#include <install/xbox360/xex_validator.h>
#include <gpu/video.h>
#include <gpu/imgui/imgui_snapshot.h>
#include <hid/hid.h>
#include <locale/locale.h>
#include <patches/aspect_ratio_patches.h>
#include <ui/imgui_utils.h>
#include <ui/button_window.h>
#include <ui/common_menu.h>
#include <ui/message_window.h>
#include <ui/game_window.h>
#include <decompressor.h>
#include <exports.h>
#include <sdl_listener.h>
#include <user/config.h>

#ifdef LIBERTY_RECOMP_HAS_RESOURCES
#include <res/images/common/libertyrecomp.dds.h>
#endif // LIBERTY_RECOMP_HAS_RESOURCES
// Character and logo textures disabled for clean installer
// #include <res/images/installer/gta4_logo.dds.h>
// #include <res/images/installer/DLC/tlad_logo.dds.h>
// #include <res/images/installer/DLC/tbogt_logo.dds.h>
// #include <res/images/installer/install_001.dds.h>
// #include <res/images/installer/install_002.dds.h>
// #include <res/images/installer/install_003.dds.h>
// #include <res/images/installer/install_004.dds.h>
// #include <res/images/installer/install_005.dds.h>
// #include <res/images/installer/install_006.dds.h>
// #include <res/images/installer/install_007.dds.h>
// #include <res/images/installer/install_008.dds.h>
#include <res/credits.h>
#include <ui/gta4_style.h>

// Forward declarations
static bool AutoSelectTU8();
static bool ValidateDLCUpdateRequirement();
static bool InstallerParseSources(std::string &errorMessage);

// One Shot Animations Constants
static constexpr double SCANLINES_ANIMATION_TIME = 0.0;
static constexpr double SCANLINES_ANIMATION_DURATION = 15.0;

static constexpr double IMAGE_ANIMATION_TIME = SCANLINES_ANIMATION_TIME + SCANLINES_ANIMATION_DURATION;
static constexpr double IMAGE_ANIMATION_DURATION = 15.0;

static constexpr double TITLE_ANIMATION_TIME = SCANLINES_ANIMATION_DURATION;
static constexpr double TITLE_ANIMATION_DURATION = 30.0;

static constexpr double CONTAINER_LINE_ANIMATION_TIME = SCANLINES_ANIMATION_DURATION;
static constexpr double CONTAINER_LINE_ANIMATION_DURATION = 23.0;

static constexpr double CONTAINER_INNER_TIME = SCANLINES_ANIMATION_DURATION + CONTAINER_LINE_ANIMATION_DURATION + 8.0;
static constexpr double CONTAINER_INNER_DURATION = 15.0;

static constexpr double ALL_ANIMATIONS_FULL_DURATION = CONTAINER_INNER_TIME + CONTAINER_INNER_DURATION;
static constexpr double QUITTING_EXTRA_DURATION = 60.0;

// Clean installer: Centered container layout (X position computed dynamically)
constexpr float CONTAINER_Y = 120.0f;  // Below header
constexpr float CONTAINER_WIDTH = 600.0f;  // Wider for centered layout
constexpr float CONTAINER_HEIGHT = 420.0f;  // Taller for content
constexpr float CONTAINER_PADDING = 30.0f;  // Standard padding

constexpr float BOTTOM_X_GAP = 4.0f;
constexpr float BOTTOM_Y_GAP = 5.0f;
constexpr float CONTAINER_BUTTON_WIDTH = 250.0f;
constexpr float CONTAINER_BUTTON_GAP = 9.0f;
constexpr float BUTTON_HEIGHT = 22.0f;
constexpr float BUTTON_TEXT_GAP = 28.0f;

constexpr float BORDER_SIZE = 1.0f;
constexpr float BORDER_OVERSHOOT = 36.0f;

static constexpr size_t GRID_SIZE = 9;

static CommonMenu g_commonMenu;

static double g_chevronTime = 0.0;
static double g_appearTime = 0.0;
static double g_disappearTime = DBL_MAX;
static bool g_isDisappearing = false;
static bool g_isQuitting = false;

static std::filesystem::path g_installPath;
static std::filesystem::path g_gameSourcePath;
static std::array<std::filesystem::path, int(DLC::Count)> g_dlcSourcePaths;
static std::array<bool, int(DLC::Count)> g_dlcInstalled = {};
static std::unique_ptr<GuestTexture> g_upLibertyDev;
static Journal g_installerJournal;
static Installer::Sources g_installerSources;
static uint64_t g_installerAvailableSize = 0;
static std::unique_ptr<std::thread> g_installerThread;
static double g_installerStartTime = 0.0;
static double g_installerEndTime = DBL_MAX;
static float g_installerProgressRatioCurrent = 0.0f;
static std::atomic<float> g_installerProgressRatioTarget = 0.0f;
static std::atomic<bool> g_installerFinished = false;
static std::atomic<bool> g_installerHalted = false;
static std::atomic<bool> g_installerCancelled = false;
static bool g_installerFailed = false;
static std::string g_installerErrorMessage;

enum class WizardPage
{
    SelectLanguage,
    Introduction,
    SelectGame,
    SelectTitleUpdate,
    SelectDLC,
    CheckSpace,
    Installing,
    InstallSucceeded,
    InstallFailed,
};

enum class MessagePromptSource
{
    Unknown,
    Next,
    Back
};

static WizardPage g_firstPage = WizardPage::SelectLanguage;
static WizardPage g_currentPage = g_firstPage;
static std::string g_currentMessagePrompt = "";
static MessagePromptSource g_currentMessagePromptSource = MessagePromptSource::Unknown;
static bool g_currentMessagePromptConfirmation = false;
static std::list<std::filesystem::path> g_currentPickerResults;
static std::atomic<bool> g_currentPickerResultsReady = false;
static std::string g_currentPickerErrorMessage;
static std::unique_ptr<std::thread> g_currentPickerThread;
static bool g_pickerTutorialCleared[2] = {};
static bool g_pickerTutorialTriggered = false;
static bool g_pickerTutorialFolderMode = false;
static bool g_currentPickerVisible = false;
static bool g_currentPickerFolderMode = false;
static int g_currentMessageResult = -1;
static ImVec2 g_joypadAxis = {};
static int g_currentCursorIndex = -1;
static int g_currentCursorDefault = 0;
static bool g_currentCursorAccepted = false;
static bool g_currentCursorBack = false;
static std::vector<std::pair<ImVec2, ImVec2>> g_currentCursorRects;
static std::string g_creditsStr;

// DLC Selection state (declared early for use in event listener)
static int g_dlcSelectionIndex = 1; // 0=TBOGT, 1=GTA IV, 2=TLAD (center selected by default)

// Title Update selection state
static liberty::install::TitleUpdateManager g_titleUpdateManager;
static int g_selectedTitleUpdateIndex = -1;  // -1 = no update (base game)
static bool g_titleUpdatesScanned = false;

// Hold-to-skip ESC functionality
static bool g_escHeld = false;
static double g_escHoldStartTime = 0.0;
static constexpr double ESC_HOLD_DURATION = 1.5; // Hold for 1.5 seconds to skip

// Forward declaration for file picker
static void PickerShow(bool folderMode);

class SDLEventListenerForInstaller : public SDLEventListener
{
public:
    bool OnSDLEvent(SDL_Event *event) override
    {
        if (!InstallerWizard::s_isVisible)
            return false;

        bool noModals = g_currentMessagePrompt.empty() && !g_currentPickerVisible;
        if (event->type == SDL_QUIT && g_currentPage == WizardPage::Installing)
        {
            // Pretend the back button was pressed if the user tried quitting during installation.
            // This condition is above the rest of the event processing as we want to block the exit
            // button while there's confirmation message is open as well.
            if (noModals)
                g_currentCursorBack = true;

            return true;
        }

        if (!noModals || !hid::IsInputAllowed())
            return false;

        constexpr float AxisValueRange = 32767.0f;
        constexpr float AxisTapRange = 0.5f;
        int newCursorIndex = -1;
        ImVec2 tapDirection = {};

        switch (event->type)
        {
            case SDL_KEYDOWN:
            {
                switch (event->key.keysym.scancode)
                {
                    case SDL_SCANCODE_LEFT:
                    case SDL_SCANCODE_A:
                        // Special handling for DLC selection page
                        if (g_currentPage == WizardPage::SelectDLC)
                        {
                            g_dlcSelectionIndex = (g_dlcSelectionIndex + 2) % 3; // Wrap left
                            Game_PlaySound("cursor2");  // Play select sound
                        }
                        else
                        {
                            tapDirection.x = -1.0f;
                        }
                        break;
                    case SDL_SCANCODE_RIGHT:
                    case SDL_SCANCODE_D:
                        // Special handling for DLC selection page
                        if (g_currentPage == WizardPage::SelectDLC)
                        {
                            g_dlcSelectionIndex = (g_dlcSelectionIndex + 1) % 3; // Wrap right
                            Game_PlaySound("cursor2");  // Play select sound
                        }
                        else
                        {
                            tapDirection.x = 1.0f;
                        }
                        break;
                    case SDL_SCANCODE_UP:
                    case SDL_SCANCODE_DOWN:
                        tapDirection.y = (event->key.keysym.scancode == SDL_SCANCODE_DOWN) ? 1.0f : -1.0f;
                        break;
                    case SDL_SCANCODE_RETURN:
                    case SDL_SCANCODE_KP_ENTER:
                        if (g_currentPage == WizardPage::SelectDLC)
                        {
                            // For DLC selection, open file picker if a DLC is selected (not base game)
                            if (g_dlcSelectionIndex == 0 || g_dlcSelectionIndex == 2) {
                                // DLC selected - open folder picker
                                Game_PlaySound("deside");
                                PickerShow(true);
                            } else {
                                // Base game selected - proceed to next step
                                g_currentCursorAccepted = true;
                            }
                        }
                        else
                        {
                            g_currentCursorAccepted = (g_currentCursorIndex >= 0);
                        }
                        break;
                    case SDL_SCANCODE_Y:
                        // Auto-select TU8 when error modal is shown
                        if (!g_currentMessagePrompt.empty() && g_currentPage == WizardPage::SelectDLC)
                        {
                            if (AutoSelectTU8())
                            {
                                g_currentMessagePrompt.clear();
                                Game_PlaySound("deside");
                            }
                        }
                        break;
                    case SDL_SCANCODE_ESCAPE:
                        if (g_currentPage == WizardPage::SelectDLC)
                        {
                            // Start ESC hold timer for DLC skip
                            g_escHeld = true;
                            g_escHoldStartTime = ImGui::GetTime();
                        }
                        else
                        {
                            g_currentCursorBack = true;
                        }
                        break;
                }

                break;
            }
            
            case SDL_KEYUP:
            {
                if (event->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    if (g_escHeld && g_currentPage == WizardPage::SelectDLC)
                    {
                        // ESC released before hold completed - treat as back action
                        double holdTime = ImGui::GetTime() - g_escHoldStartTime;
                        if (holdTime < ESC_HOLD_DURATION)
                        {
                            g_currentCursorBack = true;
                        }
                    }
                    g_escHeld = false;
                }
                break;
            }

            case SDL_CONTROLLERBUTTONDOWN:
            {
                switch (event->cbutton.button)
                {
                    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                        // Special handling for DLC selection page
                        if (g_currentPage == WizardPage::SelectDLC)
                        {
                            g_dlcSelectionIndex = (g_dlcSelectionIndex + 2) % 3;
                            Game_PlaySound("cursor2");  // Play select sound
                        }
                        else
                        {
                            tapDirection = { -1.0f, 0.0f };
                        }
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                        if (g_currentPage == WizardPage::SelectDLC)
                        {
                            g_dlcSelectionIndex = (g_dlcSelectionIndex + 1) % 3;
                            Game_PlaySound("cursor2");  // Play select sound
                        }
                        else
                        {
                            tapDirection = { 1.0f, 0.0f };
                        }
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_UP:
                        tapDirection = { 0.0f, -1.0f };
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                        tapDirection = { 0.0f, 1.0f };
                        break;
                    case SDL_CONTROLLER_BUTTON_A:
                        if (g_currentPage == WizardPage::SelectDLC)
                        {
                            // For DLC selection, open file picker if a DLC is selected
                            if (g_dlcSelectionIndex == 0 || g_dlcSelectionIndex == 2) {
                                Game_PlaySound("deside");
                                PickerShow(true);
                            } else {
                                g_currentCursorAccepted = true;
                            }
                        }
                        else
                        {
                            g_currentCursorAccepted = (g_currentCursorIndex >= 0);
                        }
                        break;
                    case SDL_CONTROLLER_BUTTON_B:
                        g_currentCursorBack = true;
                        break;
                }

                break;
            }

            case SDL_CONTROLLERAXISMOTION:
            {
                if (event->caxis.axis < 2)
                {
                    float newAxisValue = event->caxis.value / AxisValueRange;
                    bool sameDirection = (newAxisValue * g_joypadAxis[event->caxis.axis]) > 0.0f;
                    bool wasInRange = abs(g_joypadAxis[event->caxis.axis]) > AxisTapRange;
                    bool isInRange = abs(newAxisValue) > AxisTapRange;
                    if (sameDirection && !wasInRange && isInRange)
                    {
                        tapDirection[event->caxis.axis] = newAxisValue;
                    }

                    g_joypadAxis[event->caxis.axis] = newAxisValue;
                }

                break;
            }

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEMOTION:
            {
                for (size_t i = 0; i < g_currentCursorRects.size(); i++)
                {
                    auto &currentRect = g_currentCursorRects[i];

                    if (ImGui::IsMouseHoveringRect(currentRect.first, currentRect.second, false))
                    {
                        newCursorIndex = int(i);

                        if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
                            g_currentCursorAccepted = true;

                        break;
                    }
                }

                if (newCursorIndex < 0)
                    g_currentCursorIndex = -1;

                break;
            }
        }

        if (tapDirection.x != 0.0f || tapDirection.y != 0.0f)
        {
            if (g_currentCursorIndex >= g_currentCursorRects.size() || g_currentCursorIndex < 0)
            {
                newCursorIndex = g_currentCursorDefault;
            }
            else
            {
                auto &currentRect = g_currentCursorRects[g_currentCursorIndex];
                ImVec2 currentPoint = ImVec2
                (
                    (currentRect.first.x + currentRect.second.x) / 2.0f + tapDirection.x * (currentRect.second.x - currentRect.first.x) / 2.0f,
                    (currentRect.first.y + currentRect.second.y) / 2.0f + tapDirection.y * (currentRect.second.y - currentRect.first.y) / 2.0f
                );

                float closestDistance = FLT_MAX;
                for (size_t i = 0; i < g_currentCursorRects.size(); i++)
                {
                    if (g_currentCursorIndex == i)
                    {
                        continue;
                    }

                    auto &targetRect = g_currentCursorRects[i];
                    ImVec2 targetPoint = ImVec2
                    (
                        (targetRect.first.x + targetRect.second.x) / 2.0f + tapDirection.x * (targetRect.first.x - targetRect.second.x) / 2.0f,
                        (targetRect.first.y + targetRect.second.y) / 2.0f + tapDirection.y * (targetRect.first.y - targetRect.second.y) / 2.0f
                    );

                    ImVec2 delta = ImVec2(targetPoint.x - currentPoint.x, targetPoint.y - currentPoint.y);
                    float projectedDistance = delta.x * tapDirection.x + delta.y * tapDirection.y;
                    float manhattanDistance = abs(delta.x) + abs(delta.y);
                    if (projectedDistance > 0.0f && manhattanDistance < closestDistance)
                    {
                        newCursorIndex = int(i);
                        closestDistance = manhattanDistance;
                    }
                }
            }
        }

        if (newCursorIndex >= 0)
        {
            if (g_currentCursorIndex != newCursorIndex)
                Game_PlaySound("move");

            g_currentCursorIndex = newCursorIndex;
        }

        return false;
    }
}
g_sdlEventListenerForInstaller;

static std::string& GetWizardText(WizardPage page)
{
    switch (page)
    {
        case WizardPage::SelectLanguage: return Localise("Installer_Page_SelectLanguage");
        case WizardPage::Introduction: return Localise("Installer_Page_Introduction");
        case WizardPage::SelectGame: return Localise("Installer_Page_SelectGame");
        case WizardPage::SelectTitleUpdate: return Localise("Installer_Page_SelectTitleUpdate");
        case WizardPage::SelectDLC: return Localise("Installer_Page_SelectDLC");
        case WizardPage::CheckSpace: return Localise("Installer_Page_CheckSpace");
        case WizardPage::Installing: return Localise("Installer_Page_Installing");
        case WizardPage::InstallSucceeded: return Localise("Installer_Page_InstallSucceeded");
        case WizardPage::InstallFailed: return Localise("Installer_Page_InstallFailed");
    }

    return g_localeMissing;
}

static const int WIZARD_INSTALL_TEXTURE_INDEX[] =
{
    0,
    1,
    2,
    3,  // SelectTitleUpdate
    3,
    4,
    5,
    7, // Force Elise on InstallSucceeded.
    6  // Force Eggman on InstallFailed.
};

// These are ordered from bottom to top in a 3x2 grid.
const char* LANGUAGE_TEXT[] =
{
    "FRANÇAIS", // French
    "DEUTSCH", // German
    "ENGLISH", // English
    "ESPAÑOL", // Spanish
    "ITALIANO", // Italian
    "日本語", // Japanese
};

const ELanguage LANGUAGE_ENUM[] =
{
    ELanguage::French,
    ELanguage::German,
    ELanguage::English,
    ELanguage::Spanish,
    ELanguage::Italian,
    ELanguage::Japanese,
};

// GTA IV DLC names for selection screen
const char *DLC_SOURCE_TEXT[] =
{
    "THE LOST AND DAMNED",
    "THE BALLAD OF GAY TONY",
};

static int DLCIndex(DLC dlc)
{
    assert(dlc != DLC::Unknown);
    return (int)(dlc) - 1;
}

static void SetCurrentPage(WizardPage page)
{
    g_currentPage = page;

    if (g_currentPage == WizardPage::InstallSucceeded)
    {
        ButtonWindow::Open("Button_Select");
    }
    else if (g_currentPage != WizardPage::Installing)
    {
        auto key = "Button_SelectBack";

        if (g_currentPage == g_firstPage || g_currentPage == WizardPage::InstallFailed)
            key = "Button_SelectQuit";

        ButtonWindow::Open(key);
    }
    else if (g_currentPage == WizardPage::Installing)
    {
        ButtonWindow::Open("Button_Cancel");
    }
    else
    {
        ButtonWindow::Close();
    }
}

static double ComputeMotionInstaller(double timeAppear, double timeDisappear, double offset, double total)
{
    return ComputeMotion(timeAppear, offset, total) * (1.0 - ComputeMotion(timeDisappear, ALL_ANIMATIONS_FULL_DURATION - offset - total, total));
}

static bool PushCursorRect(ImVec2 min, ImVec2 max, bool &cursorPressed, bool makeDefault = false)
{
    int currentIndex = int(g_currentCursorRects.size());
    g_currentCursorRects.emplace_back(min, max);

    if (makeDefault)
    {
        g_currentCursorDefault = currentIndex;
    }

    if (g_currentCursorIndex == currentIndex)
    {
        if (g_currentCursorAccepted)
        {
            Game_PlaySound("main_deside");
            cursorPressed = true;
            g_currentCursorAccepted = false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

static void ResetCursorRects()
{
    g_currentCursorDefault = 0;
    g_currentCursorRects.clear();
}

static void DrawBackground()
{
    auto &res = ImGui::GetIO().DisplaySize;
    auto drawList = ImGui::GetBackgroundDrawList();

    // GTA IV Style: Solid black background
    drawList->AddRectFilled({ 0.0, 0.0 }, res, GTA4Style::Colors::Background);
}

static void DrawGTA4Logo()
{
    // Logo disabled for clean installer UI
    // Just draw a minimal header accent line instead
    auto &res = ImGui::GetIO().DisplaySize;
    auto drawList = ImGui::GetBackgroundDrawList();
    
    float alpha = ComputeMotionInstaller(g_appearTime, g_disappearTime, CONTAINER_INNER_TIME, CONTAINER_INNER_DURATION);
    
    // Draw subtle orange accent line at top
    float lineHeight = Scale(3);
    drawList->AddRectFilled({ 0, 0 }, { res.x, lineHeight }, GTA4Style::WithAlpha(GTA4Style::Colors::Orange, alpha));
}

// Clean header - minimal style with page title
static void DrawGTA4Header()
{
    auto &res = ImGui::GetIO().DisplaySize;
    auto drawList = ImGui::GetBackgroundDrawList();
    
    float alpha = ComputeMotionInstaller(g_appearTime, g_disappearTime, CONTAINER_INNER_TIME, CONTAINER_INNER_DURATION);
    
    // Get current page title using GetWizardText
    const std::string &pageTitle = GetWizardText(g_currentPage);
    
    // Draw title text centered at top
    float titleFontSize = Scale(24);
    ImVec2 titleSize = g_pFntNewRodin->CalcTextSizeA(titleFontSize, FLT_MAX, 0.0f, pageTitle.c_str());
    float titleX = (res.x - titleSize.x) / 2.0f;
    float titleY = Scale(40);
    
    // Draw subtle underline
    float lineY = titleY + titleSize.y + Scale(8);
    float lineWidth = titleSize.x + Scale(40);
    float lineX = (res.x - lineWidth) / 2.0f;
    drawList->AddRectFilled({ lineX, lineY }, { lineX + lineWidth, lineY + Scale(2) }, GTA4Style::WithAlpha(GTA4Style::Colors::Orange, alpha * 0.8f));
    
    // Draw title in orange
    drawList->AddText(g_pFntNewRodin, titleFontSize, { titleX, titleY }, GTA4Style::WithAlpha(GTA4Style::Colors::Orange, alpha), pageTitle.c_str());
}

static void DrawArrows()
{
    auto& res = ImGui::GetIO().DisplaySize;

    DrawArrows({ 0, 0 }, res, g_chevronTime);
}

static void DrawLeftImage()
{
    // Character images disabled for clean installer UI
    // Keep the function but do nothing
}

static void DrawContainer(ImVec2 min, ImVec2 max, bool isTextArea)
{   
    auto &res = ImGui::GetIO().DisplaySize;
    auto drawList = ImGui::GetBackgroundDrawList();

    double gridOverlayAlpha = ComputeMotionInstaller(g_appearTime, g_disappearTime, CONTAINER_INNER_TIME, CONTAINER_INNER_DURATION);

    if (isTextArea)
    {
        auto padding = Scale(CONTAINER_PADDING);

        DrawContainerBox({ min.x - padding, min.y - padding }, { max.x + padding, max.y + padding }, gridOverlayAlpha);
    }

    float gridSize = Scale(GRID_SIZE);

    // The draw area
    drawList->PushClipRect({ min.x - gridSize * 2.0f, min.y + gridSize * 2.0f }, { max.x - gridSize * 2.0f + 1.0f, max.y - gridSize * 2.0f + 1.0f });
}

static void DrawDescriptionContainer()
{
    auto &res = ImGui::GetIO().DisplaySize;
    auto drawList = ImGui::GetBackgroundDrawList();
    auto fontSize = Scale(25.0f);

    // Center the container horizontally
    float containerWidth = Scale(CONTAINER_WIDTH);
    float containerX = (res.x - containerWidth) / 2.0f;
    
    ImVec2 descriptionMin = { round(containerX), round(g_aspectRatioOffsetY + Scale(CONTAINER_Y + 0.5f)) };
    ImVec2 descriptionMax = { round(containerX + containerWidth), round(g_aspectRatioOffsetY + Scale(CONTAINER_Y + 0.5f + CONTAINER_HEIGHT)) };
    SetProceduralOrigin(descriptionMin);
    DrawContainer(descriptionMin, descriptionMax, true);

    char descriptionText[512];
    char requiredSpaceText[128];
    char availableSpaceText[128];
    strncpy(descriptionText, GetWizardText(g_currentPage).c_str(), sizeof(descriptionText) - 1);

    if (g_currentPage == WizardPage::CheckSpace)
    {
        constexpr double DivisorGiB = (1024.0 * 1024.0 * 1024.0);
        double requiredGiB = double(g_installerSources.totalSize) / DivisorGiB;
        double availableGiB = double(g_installerAvailableSize) / DivisorGiB;
        snprintf(requiredSpaceText, sizeof(requiredSpaceText), Localise("Installer_Step_RequiredSpace").c_str(), requiredGiB);
        snprintf(availableSpaceText, sizeof(availableSpaceText), (g_installerAvailableSize > 0) ? Localise("Installer_Step_AvailableSpace").c_str() : "", availableGiB);
        snprintf(descriptionText, sizeof(descriptionText), "%s%s\n%s", GetWizardText(g_currentPage).c_str(), requiredSpaceText, availableSpaceText);
    }
    else if (g_currentPage == WizardPage::InstallFailed)
    {
        // Japanese needs text to be brought in by a normal width space
        // as it allows for text to begin further than others for
        // special characters.
        if (Config::Language == ELanguage::Japanese)
        {
            strncat(descriptionText, " ", 1);
        }

        strncat(descriptionText, g_installerErrorMessage.c_str(), sizeof(descriptionText) - 1);
    }

    double textAlpha = ComputeMotionInstaller(g_appearTime, g_disappearTime, CONTAINER_INNER_TIME, CONTAINER_INNER_DURATION);
    auto clipRectMin = drawList->GetClipRectMin();
    auto clipRectMax = drawList->GetClipRectMax();

    float textX = clipRectMin.x + fontSize;
    float textY = clipRectMin.y - Scale(1.0f);

    auto lineWidth = clipRectMax.x - (fontSize / 2.0f) - clipRectMin.x;

    clipRectMax.x += fontSize;
    clipRectMax.y += Scale(1.0f);

    float lineMargin = 5.0f;

    drawList->PushClipRect(clipRectMin, clipRectMax, false);

    DrawTextParagraph
    (
        g_pFntRodin,
        fontSize,
        lineWidth,
        { textX, textY },
        lineMargin,
        descriptionText,

        [=](const char* str, ImVec2 pos)
        {
            DrawTextBasic(g_pFntRodin, fontSize, pos, IM_COL32(255, 255, 255, 255 * textAlpha), str);
        }
    );

    drawList->PopClipRect();
    drawList->PopClipRect();

    if (g_currentPage == WizardPage::InstallSucceeded)
    {
        auto descTextSize = MeasureCentredParagraph(g_pFntRodin, fontSize, lineWidth, lineMargin, descriptionText);

        auto colWhite = IM_COL32(255, 255, 255, 255 * textAlpha);

        // Center the container coordinates
        float centeredContainerX = (res.x - Scale(CONTAINER_WIDTH)) / 2.0f;
        auto containerLeft = centeredContainerX;
        auto containerTop = g_aspectRatioOffsetY + Scale(CONTAINER_Y);
        auto containerRight = containerLeft + Scale(CONTAINER_WIDTH);
        auto containerBottom = containerTop + Scale(CONTAINER_HEIGHT);

        auto marqueeTextSize = g_pFntRodin->CalcTextSizeA(fontSize, FLT_MAX, 0, g_creditsStr.c_str());
        auto marqueeTextMarginX = Scale(5);
        auto marqueeTextMarginY = Scale(15);

        ImVec2 marqueeTextPos = { descriptionMax.x, containerBottom - marqueeTextSize.y - marqueeTextMarginY };
        ImVec2 marqueeTextMin = { containerLeft, marqueeTextPos.y };
        ImVec2 marqueeTextMax = { containerRight, containerBottom };

        auto imageWidth = Scale(524);
        auto imageHeight = Scale(45);

        ImVec2 imageRegionMin = { containerLeft, textY + descTextSize.y };
        ImVec2 imageRegionMax = { containerRight, containerBottom - (marqueeTextMax.y - marqueeTextMin.y) };

        ImVec2 imageMin = 
        {
            /* X */ imageRegionMin.x + ((imageRegionMax.x - imageRegionMin.x) / 2) - (imageWidth / 2),
            /* Y */ imageRegionMin.y + ((imageRegionMax.y - imageRegionMin.y) / 2) - (imageHeight / 2)
        };

        ImVec2 imageMax = { imageMin.x + imageWidth, imageMin.y + imageHeight };

        drawList->AddImage(g_upLibertyDev.get(), imageMin, imageMax, { 0, 0 }, { 1, 1 }, colWhite);

        // SetHorizontalMarqueeFade(marqueeTextMin, marqueeTextMax, Scale(32));
        // DrawTextWithMarquee(g_pFntRodin, fontSize, marqueeTextPos, marqueeTextMin, marqueeTextMax, colWhite, g_creditsStr.c_str(), g_installerEndTime, 0.9, Scale(200));
        // ResetMarqueeFade();
    }

    ImVec2 sideMin = { descriptionMax.x, descriptionMin.y };
    ImVec2 sideMax = { res.x, descriptionMax.y };

    DrawContainer(sideMin, sideMax, false);

    drawList->PopClipRect();

    ResetProceduralOrigin();
}

static void DrawButtonContainer(ImVec2 min, ImVec2 max, int baser, int baseg, float alpha)
{
    auto drawList = ImGui::GetBackgroundDrawList();

    // GTA IV Style: Simple dark row with subtle highlight on hover - no borders
    bool isHovered = (baser != 0 || baseg != 0);
    
    // Simple dark background row
    ImU32 bgColor = isHovered 
        ? GTA4Style::WithAlpha(IM_COL32(50, 50, 50, 255), alpha)
        : GTA4Style::WithAlpha(IM_COL32(30, 30, 30, 200), alpha);
    
    drawList->AddRectFilled(min, max, bgColor);
}

static ImVec2 ComputeTextSize(ImFont *font, const char *text, float size, float &squashRatio, float maxTextWidth = FLT_MAX)
{
    ImVec2 textSize = font->CalcTextSizeA(size, FLT_MAX, 0.0f, text);
    if (textSize.x > maxTextWidth)
    {
        squashRatio = maxTextWidth / textSize.x;
    }
    else
    {
        squashRatio = 1.0f;
    }

    return textSize;
}

static void DrawButton(ImVec2 min, ImVec2 max, const char *buttonText, bool sourceButton, bool buttonEnabled, bool &buttonPressed, float maxTextWidth = FLT_MAX, bool makeDefault = false)
{
    buttonPressed = false;

    auto drawList = ImGui::GetBackgroundDrawList();
    float alpha = ComputeMotionInstaller(g_appearTime, g_disappearTime, CONTAINER_INNER_TIME, CONTAINER_INNER_DURATION);
    if (!buttonEnabled)
    {
        alpha *= 0.5f;
    }

    int baser = 0;
    int baseg = 0;
    bool isHovered = false;
    if (g_currentMessagePrompt.empty() && !g_currentPickerVisible && !sourceButton && buttonEnabled && (alpha >= 1.0f))
    {
        bool cursorOnButton = PushCursorRect(min, max, buttonPressed, makeDefault);
        if (cursorOnButton)
        {
            baser = 48;
            baseg = 32;
            isHovered = true;
        }
    }

    DrawButtonContainer(min, max, baser, baseg, alpha);

    ImFont *font = g_pFntRodin;
    float size = Scale(18.0f);
    float squashRatio;
    ImVec2 textSize = ComputeTextSize(font, buttonText, size, squashRatio, Scale(maxTextWidth));
    min.x += ((max.x - min.x) - textSize.x) / 2.0f;
    min.y += ((max.y - min.y) - textSize.y) / 2.0f;

    SetOrigin({ min.x + textSize.x / 2.0f, min.y });
    SetScale({ squashRatio, 1.0f });

    // GTA IV Style: Orange text for selected, white/gray for normal
    ImU32 textColor = isHovered 
        ? GTA4Style::WithAlpha(GTA4Style::Colors::Orange, alpha)
        : IM_COL32(255, 255, 255, 255 * alpha);

    DrawTextBasic(font, size, min, textColor, buttonText);

    SetScale({ 1.0f, 1.0f });
    SetOrigin({ 0.0f, 0.0f });
}

enum ButtonColumn
{
    ButtonColumnLeft,
    ButtonColumnMiddle,
    ButtonColumnRight
};

static void ComputeButtonColumnCoordinates(ButtonColumn buttonColumn, float &minX, float &maxX)
{
    // Center the button coordinates to match centered container
    auto &res = ImGui::GetIO().DisplaySize;
    float containerWidth = Scale(CONTAINER_WIDTH);
    float containerX = (res.x - containerWidth) / 2.0f;
    
    switch (buttonColumn)
    {
    case ButtonColumnLeft:
        minX = containerX + Scale(CONTAINER_BUTTON_GAP);
        maxX = containerX + Scale(CONTAINER_BUTTON_GAP + CONTAINER_BUTTON_WIDTH);
        break;
    case ButtonColumnMiddle:
        minX = containerX + Scale(CONTAINER_BUTTON_GAP);
        maxX = containerX + containerWidth - Scale(CONTAINER_BUTTON_GAP);
        break;
    case ButtonColumnRight:
        minX = containerX + containerWidth - Scale(CONTAINER_BUTTON_GAP + CONTAINER_BUTTON_WIDTH);
        maxX = containerX + containerWidth - Scale(CONTAINER_BUTTON_GAP);
        break;
    }
}

static void DrawSourceButton(ButtonColumn buttonColumn, float yRatio, const char *sourceText, bool sourceSet)
{
    bool buttonPressed;
    float minX, maxX;
    ComputeButtonColumnCoordinates(buttonColumn, minX, maxX);

    float minusY = (CONTAINER_BUTTON_GAP + BUTTON_HEIGHT) * yRatio;
    ImVec2 min = { minX, g_aspectRatioOffsetY + Scale(CONTAINER_Y + CONTAINER_HEIGHT - CONTAINER_BUTTON_GAP - BUTTON_HEIGHT - minusY) };
    ImVec2 max = { maxX, g_aspectRatioOffsetY + Scale(CONTAINER_Y + CONTAINER_HEIGHT - CONTAINER_BUTTON_GAP - minusY) };

    auto alphaMotion = ComputeMotionInstaller(g_appearTime, g_disappearTime, CONTAINER_INNER_TIME, CONTAINER_INNER_DURATION);
    auto lightSize = Scale(14);

    DrawButton(min, max, sourceText, true, sourceSet, buttonPressed, ((max.x - min.x) * 0.7) / g_aspectRatioScale);
}

static void DrawProgressBar(float progressRatio)
{
    auto &res = ImGui::GetIO().DisplaySize;
    auto drawList = ImGui::GetBackgroundDrawList();
    float alpha = 1.0;
    
    // GTA IV Style: Orange progress bar
    const uint32_t innerColor0 = GTA4Style::Colors::ProgressBg;
    const uint32_t innerColor1 = GTA4Style::Colors::BackgroundDark;
    
    // Center the progress bar to match centered container
    float containerWidth = Scale(CONTAINER_WIDTH);
    float containerX = (res.x - containerWidth) / 2.0f;
    
    float xPadding = Scale(4);
    float yPadding = Scale(3);
    ImVec2 min = { containerX + BOTTOM_X_GAP + Scale(1), g_aspectRatioOffsetY + Scale(CONTAINER_Y + CONTAINER_HEIGHT + BOTTOM_Y_GAP)};
    ImVec2 max = { containerX + containerWidth - Scale(BOTTOM_X_GAP), g_aspectRatioOffsetY + Scale(CONTAINER_Y + CONTAINER_HEIGHT + BOTTOM_Y_GAP + BUTTON_HEIGHT) };

    // Background container
    drawList->AddRectFilled(min, max, GTA4Style::Colors::BackgroundPanel);
    drawList->AddRect(min, max, GTA4Style::Colors::Border);

    drawList->AddRectFilledMultiColor
    (
        { min.x + xPadding, min.y + yPadding },
        { max.x - xPadding, max.y - yPadding },
        innerColor0,
        innerColor0,
        innerColor1,
        innerColor1
    );

    // GTA IV orange progress fill
    const uint32_t sliderColor0 = GTA4Style::Colors::ProgressFill;
    const uint32_t sliderColor1 = GTA4Style::Colors::ProgressFillDark;
    xPadding += Scale(1.5f);
    yPadding += Scale(1.5f);

    ImVec2 sliderMin = { min.x + xPadding, min.y + yPadding };
    ImVec2 sliderMax = { max.x - xPadding, max.y - yPadding };
    sliderMax.x = sliderMin.x + (sliderMax.x - sliderMin.x) * progressRatio;
    drawList->AddRectFilledMultiColor(sliderMin, sliderMax, sliderColor0, sliderColor0, sliderColor1, sliderColor1);
}

static bool ConvertPathSet(const nfdpathset_t *pathSet, std::list<std::filesystem::path> &filePaths)
{
    nfdpathsetsize_t pathSetCount = 0;
    if (NFD_PathSet_GetCount(pathSet, &pathSetCount) != NFD_OKAY)
    {
        return false;
    }

    for (nfdpathsetsize_t i = 0; i < pathSetCount; i++)
    {
        nfdnchar_t *pathSetPath = nullptr;
        if (NFD_PathSet_GetPathN(pathSet, i, &pathSetPath) != NFD_OKAY)
        {
            filePaths.clear();
            return false;
        }

        filePaths.emplace_back(std::filesystem::path(pathSetPath));
        NFD_PathSet_FreePathN(pathSetPath);
    }

    return true;
}

static void PickerThreadProcess()
{
    const nfdpathset_t *pathSet;
    nfdresult_t result = NFD_ERROR;
    if (g_currentPickerFolderMode)
    {
        result = NFD_PickFolderMultipleN(&pathSet, nullptr);
    }
    else
    {
        result = NFD_OpenDialogMultipleN(&pathSet, nullptr, 0, nullptr);
    }
    
    if (result == NFD_OKAY)
    {
        bool pathsConverted = ConvertPathSet(pathSet, g_currentPickerResults);
        NFD_PathSet_Free(pathSet);
    }
    else if (result == NFD_ERROR)
    {
        g_currentPickerErrorMessage = NFD_GetError();
    }

    g_currentPickerResultsReady = true;
}

static void PickerStart(bool folderMode) {
    if (g_currentPickerThread != nullptr)
    {
        g_currentPickerThread->join();
        g_currentPickerThread.reset();
    }

    g_currentPickerResults.clear();
    g_currentPickerFolderMode = folderMode;
    g_currentPickerResultsReady = false;
    g_currentPickerVisible = true;

    // Optional single thread mode for testing on systems that do not interact well with the separate thread being used for NFD.
#ifdef __APPLE__
    constexpr bool singleThreadMode = true;
#else
    constexpr bool singleThreadMode = false;
#endif
    if (singleThreadMode)
        PickerThreadProcess();
    else
        g_currentPickerThread = std::make_unique<std::thread>(PickerThreadProcess);
}

static void PickerShow(bool folderMode)
{
    if (g_pickerTutorialCleared[folderMode])
    {
        PickerStart(folderMode);
    }
    else
    {
        g_currentMessagePrompt = Localise(folderMode ? "Installer_Message_FolderPickerTutorial" : "Installer_Message_FilePickerTutorial");
        g_currentMessagePromptConfirmation = false;
        g_pickerTutorialTriggered = true;
        g_pickerTutorialFolderMode = folderMode;
    }
}

static bool ParseSourcePaths(std::list<std::filesystem::path> &paths)
{
    assert((g_currentPage == WizardPage::SelectGame) || (g_currentPage == WizardPage::SelectDLC));

    constexpr size_t failedPathLimit = 5;
    bool isFailedPathsOverLimit = false;
    std::list<std::filesystem::path> failedPaths;
    if (g_currentPage == WizardPage::SelectGame)
    {
        for (const std::filesystem::path &path : paths)
        {
            if (Installer::parseGame(path))
            {
                g_gameSourcePath = path;
            }
            else if (failedPaths.size() < failedPathLimit)
            {
                failedPaths.push_back(path);
            }
            else
            {
                isFailedPathsOverLimit = true;
            }
        }
    }
    else if(g_currentPage == WizardPage::SelectDLC)
    {
        for (const std::filesystem::path &path : paths)
        {
            DLC dlc = Installer::parseDLC(path);
            if (dlc != DLC::Unknown)
            {
                g_dlcSourcePaths[DLCIndex(dlc)] = path;
            }
            else if (failedPaths.size() < failedPathLimit)
            {
                failedPaths.push_back(path);
            }
        }
    }

    if (!failedPaths.empty())
    {
        std::stringstream stringStream;
        stringStream << Localise("Installer_Message_InvalidFilesList") << std::endl;
        for (const std::filesystem::path &path : failedPaths)
        {
            std::u8string filenameU8 = path.filename().u8string();
            stringStream << std::endl << "- " << Truncate(std::string(filenameU8.begin(), filenameU8.end()), 32, true, true);
        }

        if (isFailedPathsOverLimit)
            stringStream << std::endl << "- [...]";

        g_currentMessagePrompt = stringStream.str();
        g_currentMessagePromptConfirmation = false;
    }

    return failedPaths.empty();
}

static void DrawLanguagePicker()
{
    bool buttonPressed = false;
    if (g_currentPage == WizardPage::SelectLanguage)
    {
        float alphaMotion = ComputeMotionInstaller(g_appearTime, g_disappearTime, CONTAINER_INNER_TIME, CONTAINER_INNER_DURATION);
        float minX, maxX;
        bool buttonPressed;

        for (int i = 0; i < 6; i++)
        {
            ComputeButtonColumnCoordinates((i < 3) ? ButtonColumnLeft : ButtonColumnRight, minX, maxX);

            float minusY = (CONTAINER_BUTTON_GAP + BUTTON_HEIGHT) * (float(i % 3));
            ImVec2 min = { minX, g_aspectRatioOffsetY + Scale(CONTAINER_Y + CONTAINER_HEIGHT - CONTAINER_BUTTON_GAP - BUTTON_HEIGHT - minusY) };
            ImVec2 max = { maxX, g_aspectRatioOffsetY + Scale(CONTAINER_Y + CONTAINER_HEIGHT - CONTAINER_BUTTON_GAP - minusY) };

            auto lightSize = Scale(14);

            DrawButton(min, max, LANGUAGE_TEXT[i], false, true, buttonPressed, FLT_MAX, LANGUAGE_ENUM[i] == ELanguage::English);

            if (buttonPressed)
            {
                Config::Language = LANGUAGE_ENUM[i];
                g_commonMenu.SetTitle(Localise("Installer_Header_Installer"), false);
            }
        }
    }
}

static void DrawSourcePickers()
{
    bool buttonPressed = false;
    std::list<std::filesystem::path> paths;
    if (g_currentPage == WizardPage::SelectGame || g_currentPage == WizardPage::SelectDLC)
    {
        // Center the picker buttons to match centered container
        auto &res = ImGui::GetIO().DisplaySize;
        float containerWidth = Scale(CONTAINER_WIDTH);
        float containerX = (res.x - containerWidth) / 2.0f;
        
        constexpr float ADD_BUTTON_MAX_TEXT_WIDTH = 168.0f;
        const std::string &addFilesText = Localise("Installer_Button_AddFiles");
        float squashRatio;
        ImVec2 textSize = ComputeTextSize(g_pFntRodin, addFilesText.c_str(), 20.0f, squashRatio, ADD_BUTTON_MAX_TEXT_WIDTH);
        ImVec2 min = { containerX + Scale(BOTTOM_X_GAP), g_aspectRatioOffsetY + Scale(CONTAINER_Y + CONTAINER_HEIGHT + BOTTOM_Y_GAP) };
        ImVec2 max = { containerX + Scale(BOTTOM_X_GAP + textSize.x * squashRatio + BUTTON_TEXT_GAP), g_aspectRatioOffsetY + Scale(CONTAINER_Y + CONTAINER_HEIGHT + BOTTOM_Y_GAP + BUTTON_HEIGHT) };
        DrawButton(min, max, addFilesText.c_str(), false, true, buttonPressed, ADD_BUTTON_MAX_TEXT_WIDTH);
        if (buttonPressed)
        {
            PickerShow(false);
        }

        min.x += Scale(BOTTOM_X_GAP + textSize.x * squashRatio + BUTTON_TEXT_GAP);

        const std::string &addFolderText = Localise("Installer_Button_AddFolder");
        textSize = ComputeTextSize(g_pFntRodin, addFolderText.c_str(), 20.0f, squashRatio, ADD_BUTTON_MAX_TEXT_WIDTH);
        max.x = min.x + Scale(textSize.x * squashRatio + BUTTON_TEXT_GAP);
        DrawButton(min, max, addFolderText.c_str(), false, true, buttonPressed, ADD_BUTTON_MAX_TEXT_WIDTH);
        if (buttonPressed)
        {
            PickerShow(true);
        }
    }
}

// Clean installer: Simplified DLC Selection with text labels only
static void DrawGTA4DLCSelection()
{
    auto &res = ImGui::GetIO().DisplaySize;
    auto drawList = ImGui::GetBackgroundDrawList();
    float alpha = ComputeMotionInstaller(g_appearTime, g_disappearTime, CONTAINER_INNER_TIME, CONTAINER_INNER_DURATION);
    
    // Draw header
    const char* headerText = "SELECT DLC TO INSTALL (OPTIONAL)";
    float headerFontSize = Scale(22);
    ImVec2 headerSize = g_pFntNewRodin->CalcTextSizeA(headerFontSize, FLT_MAX, 0.0f, headerText);
    float headerX = (res.x - headerSize.x) / 2.0f;
    float headerY = Scale(60);
    drawList->AddText(g_pFntNewRodin, headerFontSize, { headerX, headerY }, GTA4Style::WithAlpha(GTA4Style::Colors::Orange, alpha), headerText);
    
    // Draw underline
    float lineY = headerY + headerSize.y + Scale(8);
    float lineWidth = headerSize.x + Scale(40);
    float lineX = (res.x - lineWidth) / 2.0f;
    drawList->AddRectFilled({ lineX, lineY }, { lineX + lineWidth, lineY + Scale(2) }, GTA4Style::WithAlpha(GTA4Style::Colors::Orange, alpha * 0.6f));
    
    // Calculate column positions (3 columns: TBOGT | GTA IV | TLAD)
    float columnWidth = res.x / 3.0f;
    float cardY = Scale(140);
    float cardHeight = Scale(280);
    float cardWidth = columnWidth - Scale(40);
    
    // Subtitle labels for each column
    const char* subtitles[] = { "DLC", "BASE GAME", "DLC" };
    // DLC indices: 0=TBOGT, 1=GTA IV (skip), 2=TLAD
    // Map to DLC enum: TBOGT=1, TLAD=0
    
    // Draw each column
    for (int i = 0; i < 3; i++)
    {
        float columnX = columnWidth * i;
        float columnCenterX = columnX + columnWidth / 2.0f;
        
        bool isSelected = (g_dlcSelectionIndex == i);
        bool isHovered = false;
        bool isDLC = (i != 1); // 0=TBOGT, 2=TLAD are DLCs
        
        // Card bounds
        ImVec2 cardMin = { columnCenterX - cardWidth / 2.0f, cardY };
        ImVec2 cardMax = { cardMin.x + cardWidth, cardMin.y + cardHeight };
        
        // Check for mouse hover and click
        if (ImGui::IsMouseHoveringRect(cardMin, cardMax))
        {
            isHovered = true;
            if (ImGui::IsMouseClicked(0))
            {
                if (g_dlcSelectionIndex == i && isDLC)
                {
                    // Already selected DLC - open file picker
                    Game_PlaySound("deside");
                    PickerShow(true);
                }
                else
                {
                    // Select this column
                    g_dlcSelectionIndex = i;
                    Game_PlaySound("cursor2");
                }
            }
        }
        
        // Draw card background
        ImU32 cardBg = GTA4Style::WithAlpha(GTA4Style::Colors::BackgroundPanel, alpha);
        ImU32 cardBorder = isSelected ? GTA4Style::WithAlpha(GTA4Style::Colors::Orange, alpha) : 
                          (isHovered ? GTA4Style::WithAlpha(GTA4Style::Colors::Orange, alpha * 0.5f) : 
                           GTA4Style::WithAlpha(GTA4Style::Colors::Border, alpha * 0.5f));
        
        drawList->AddRectFilled(cardMin, cardMax, cardBg);
        drawList->AddRect(cardMin, cardMax, cardBorder, 0.0f, 0, isSelected ? 2.0f : 1.0f);
        
        // Draw subtitle (DLC / BASE GAME)
        float subtitleFontSize = Scale(12);
        ImVec2 subtitleSize = g_pFntRodin->CalcTextSizeA(subtitleFontSize, FLT_MAX, 0.0f, subtitles[i]);
        ImVec2 subtitlePos = { columnCenterX - subtitleSize.x / 2.0f, cardMin.y + Scale(15) };
        ImU32 subtitleColor = isDLC ? GTA4Style::WithAlpha(GTA4Style::Colors::TextOrange, alpha) : 
                                      GTA4Style::WithAlpha(GTA4Style::Colors::TextGray, alpha);
        drawList->AddText(g_pFntRodin, subtitleFontSize, subtitlePos, subtitleColor, subtitles[i]);
        
        // Draw title (multiline)
        float titleFontSize = Scale(18);
        const char* line1 = (i == 0) ? "THE BALLAD OF" : (i == 1) ? "GRAND THEFT" : "THE LOST AND";
        const char* line2 = (i == 0) ? "GAY TONY" : (i == 1) ? "AUTO IV" : "DAMNED";
        
        ImVec2 line1Size = g_pFntNewRodin->CalcTextSizeA(titleFontSize, FLT_MAX, 0.0f, line1);
        ImVec2 line2Size = g_pFntNewRodin->CalcTextSizeA(titleFontSize, FLT_MAX, 0.0f, line2);
        
        float titleY = cardMin.y + Scale(50);
        ImU32 titleColor = isSelected ? GTA4Style::WithAlpha(IM_COL32(255, 255, 255, 255), alpha) : 
                                        GTA4Style::WithAlpha(IM_COL32(180, 180, 180, 255), alpha);
        
        drawList->AddText(g_pFntNewRodin, titleFontSize, { columnCenterX - line1Size.x / 2.0f, titleY }, titleColor, line1);
        drawList->AddText(g_pFntNewRodin, titleFontSize, { columnCenterX - line2Size.x / 2.0f, titleY + line1Size.y + Scale(4) }, titleColor, line2);
        
        // Draw status for DLC columns
        if (isDLC)
        {
            int dlcIdx = (i == 0) ? 1 : 0;  // TBOGT=1, TLAD=0 in DLC enum
            bool dlcAdded = !g_dlcSourcePaths[dlcIdx].empty();
            
            const char* statusText = dlcAdded ? "ADDED" : "NOT ADDED";
            float statusFontSize = Scale(14);
            ImVec2 statusSize = g_pFntRodin->CalcTextSizeA(statusFontSize, FLT_MAX, 0.0f, statusText);
            float statusY = cardMin.y + Scale(120);
            ImU32 statusColor = dlcAdded ? GTA4Style::WithAlpha(GTA4Style::Colors::TextGreen, alpha) : 
                                           GTA4Style::WithAlpha(GTA4Style::Colors::TextGray, alpha);
            drawList->AddText(g_pFntRodin, statusFontSize, { columnCenterX - statusSize.x / 2.0f, statusY }, statusColor, statusText);
            
            // Draw "Click to add" hint if selected and not added
            if (isSelected && !dlcAdded)
            {
                const char* hintText = "CLICK TO ADD";
                float hintFontSize = Scale(12);
                ImVec2 hintSize = g_pFntRodin->CalcTextSizeA(hintFontSize, FLT_MAX, 0.0f, hintText);
                float hintY = statusY + Scale(20);
                drawList->AddText(g_pFntRodin, hintFontSize, { columnCenterX - hintSize.x / 2.0f, hintY }, 
                                  GTA4Style::WithAlpha(GTA4Style::Colors::TextOrange, alpha * 0.8f), hintText);
            }
        }
        else
        {
            // Base game - show "REQUIRED" status
            const char* statusText = !g_gameSourcePath.empty() ? "INSTALLED" : "REQUIRED";
            float statusFontSize = Scale(14);
            ImVec2 statusSize = g_pFntRodin->CalcTextSizeA(statusFontSize, FLT_MAX, 0.0f, statusText);
            float statusY = cardMin.y + Scale(120);
            ImU32 statusColor = !g_gameSourcePath.empty() ? GTA4Style::WithAlpha(GTA4Style::Colors::TextGreen, alpha) : 
                                                            GTA4Style::WithAlpha(GTA4Style::Colors::TextGray, alpha);
            drawList->AddText(g_pFntRodin, statusFontSize, { columnCenterX - statusSize.x / 2.0f, statusY }, statusColor, statusText);
        }
        
        // Draw selection indicator (orange underline for selected)
        if (isSelected)
        {
            float underlineY = cardMax.y - Scale(8);
            ImVec2 lineMin = { cardMin.x + Scale(10), underlineY };
            ImVec2 lineMax = { cardMax.x - Scale(10), underlineY + Scale(3) };
            drawList->AddRectFilled(lineMin, lineMax, GTA4Style::WithAlpha(GTA4Style::Colors::Orange, alpha));
        }
    }
    
    // Handle ENTER key to proceed (from event handler setting g_currentCursorAccepted)
    if (g_currentCursorAccepted && g_dlcSelectionIndex == 1)
    {
        // Base game column selected - trigger navigation to next page
        g_currentCursorAccepted = false;
        Game_PlaySound("main_deside");
        
        // Same validation logic as DrawNavigationButton for DLC page
        if (!ValidateDLCUpdateRequirement())
        {
            // DLC is added but TU8 not selected - show error
            std::stringstream errorMsg;
            errorMsg << "DLC Content Requires Title Update 1.06 (TU8)\n\n";
            errorMsg << "The Lost and Damned and The Ballad of Gay Tony\n";
            errorMsg << "require the latest title update to function correctly.\n\n";
            
            const auto& updates = g_titleUpdateManager.GetDetectedUpdates();
            bool tu8Available = false;
            for (const auto& update : updates)
            {
                if (update.info.version == 8)
                {
                    tu8Available = true;
                    break;
                }
            }
            
            if (tu8Available)
            {
                errorMsg << "Press Y to auto-select TU8, or\n";
                errorMsg << "Press BACK to manually select Title Update";
            }
            else
            {
                errorMsg << "TU8 not found in GAME UPDATES folder.\n";
                errorMsg << "Please add TU8 or deselect DLC.";
            }
            
            g_currentMessagePrompt = errorMsg.str();
            g_currentMessagePromptConfirmation = false;
            Game_PlaySound("error");
        }
        else
        {
            std::string sourcesErrorMessage;
            if (!InstallerParseSources(sourcesErrorMessage))
            {
                std::stringstream stringStream;
                stringStream << Localise("Installer_Message_InvalidFiles");
                if (!sourcesErrorMessage.empty())
                    stringStream << std::endl << std::endl << sourcesErrorMessage;
                g_currentMessagePrompt = stringStream.str();
                g_currentMessagePromptConfirmation = false;
            }
            else
            {
                SetCurrentPage(WizardPage::CheckSpace);
            }
        }
        return;
    }
    
    // Check ESC hold progress and skip if complete
    bool shouldSkipDLC = false;
    if (g_escHeld)
    {
        double holdTime = ImGui::GetTime() - g_escHoldStartTime;
        if (holdTime >= ESC_HOLD_DURATION)
        {
            g_escHeld = false;
            shouldSkipDLC = true;
        }
    }
    
    if (shouldSkipDLC)
    {
        Game_PlaySound("deside");
        
        // Check if any DLC is actually added - if so, need to validate
        bool anyDLCAdded = !g_dlcSourcePaths[0].empty() || !g_dlcSourcePaths[1].empty();
        if (anyDLCAdded && !ValidateDLCUpdateRequirement())
        {
            // DLC added but TU8 not selected - show error
            std::stringstream errorMsg;
            errorMsg << "DLC Content Requires Title Update 1.06 (TU8)\n\n";
            errorMsg << "Please go back and select TU8, or remove DLC.";
            g_currentMessagePrompt = errorMsg.str();
            g_currentMessagePromptConfirmation = false;
            Game_PlaySound("error");
        }
        else
        {
            std::string sourcesErrorMessage;
            if (!InstallerParseSources(sourcesErrorMessage))
            {
                std::stringstream stringStream;
                stringStream << Localise("Installer_Message_InvalidFiles");
                if (!sourcesErrorMessage.empty())
                    stringStream << std::endl << std::endl << sourcesErrorMessage;
                g_currentMessagePrompt = stringStream.str();
                g_currentMessagePromptConfirmation = false;
            }
            else
            {
                SetCurrentPage(WizardPage::CheckSpace);
            }
        }
        return;
    }
    
    // Draw navigation hints at bottom
    ImFont* hintFont = g_pFntRodin;
    float hintFontSize = Scale(13);
    float bottomY = res.y - Scale(50);
    
    // Centered navigation hint
    const char* navHint = "LEFT/RIGHT: Select  |  ENTER: Proceed  |  Hold ESC: Skip";
    ImVec2 navHintSize = hintFont->CalcTextSizeA(hintFontSize, FLT_MAX, 0.0f, navHint);
    drawList->AddText(hintFont, hintFontSize, { (res.x - navHintSize.x) / 2.0f, bottomY }, 
                      GTA4Style::WithAlpha(GTA4Style::Colors::TextGray, alpha * 0.7f), navHint);
    
    // Draw ESC hold progress bar if holding
    if (g_escHeld)
    {
        double holdTime = ImGui::GetTime() - g_escHoldStartTime;
        float progress = std::min(1.0f, (float)(holdTime / ESC_HOLD_DURATION));
        
        // Draw progress bar below hint
        float barWidth = Scale(200);
        float barHeight = Scale(4);
        float barX = (res.x - barWidth) / 2.0f;
        float barY = bottomY + Scale(20);
        ImVec2 barMin = { barX, barY };
        ImVec2 barMax = { barX + barWidth, barY + barHeight };
        
        drawList->AddRectFilled(barMin, barMax, GTA4Style::WithAlpha(IM_COL32(60, 60, 60, 255), alpha));
        drawList->AddRectFilled(barMin, { barMin.x + barWidth * progress, barMax.y }, GTA4Style::WithAlpha(GTA4Style::Colors::Orange, alpha));
    }
}

static void ScanForTitleUpdates()
{
    if (g_titleUpdatesScanned)
        return;
    
    g_titleUpdateManager.Clear();
    g_selectedTitleUpdateIndex = -1;
    
    // Scan GAME UPDATES folder in project directory
    std::filesystem::path updatesDir = std::filesystem::current_path() / "GAME UPDATES";
    if (std::filesystem::exists(updatesDir))
    {
        g_titleUpdateManager.ScanDirectory(updatesDir);
    }
    
    // Also scan install path's updates folder
    std::filesystem::path installUpdatesDir = g_installPath / "updates";
    if (std::filesystem::exists(installUpdatesDir))
    {
        g_titleUpdateManager.ScanDirectory(installUpdatesDir);
    }
    
    g_titleUpdatesScanned = true;
}

static bool ValidateDLCUpdateRequirement()
{
    // Check if DLC is selected (0=TBOGT, 1=None/Base Game, 2=TLAD)
    bool isDLCSelected = (g_dlcSelectionIndex != 1);
    
    if (!isDLCSelected)
        return true;  // No DLC selected, no requirement
    
    // Check if TU8 or higher is selected
    const auto& updates = g_titleUpdateManager.GetDetectedUpdates();
    if (g_selectedTitleUpdateIndex >= 0 && g_selectedTitleUpdateIndex < static_cast<int>(updates.size()))
    {
        uint32_t selectedVersion = updates[g_selectedTitleUpdateIndex].info.version;
        return selectedVersion >= 8;  // TU8 (v1.06) or higher required
    }
    
    return false;  // DLC selected but no update or update too old
}

static bool AutoSelectTU8()
{
    const auto& updates = g_titleUpdateManager.GetDetectedUpdates();
    for (int i = 0; i < static_cast<int>(updates.size()); i++)
    {
        if (updates[i].info.version == 8)
        {
            g_selectedTitleUpdateIndex = i;
            g_titleUpdateManager.SelectUpdate(i);
            return true;
        }
    }
    return false;  // TU8 not found
}

static void DrawTitleUpdateSelection()
{
    auto &res = ImGui::GetIO().DisplaySize;
    auto drawList = ImGui::GetBackgroundDrawList();
    float alpha = ComputeMotionInstaller(g_appearTime, g_disappearTime, CONTAINER_INNER_TIME, CONTAINER_INNER_DURATION);
    
    // Scan for updates on first entry to this page
    ScanForTitleUpdates();
    
    const auto& updates = g_titleUpdateManager.GetDetectedUpdates();
    
    // Center the title update buttons to match centered container
    float containerWidth = Scale(CONTAINER_WIDTH);
    float containerX = (res.x - containerWidth) / 2.0f;
    
    float buttonY = g_aspectRatioOffsetY + Scale(CONTAINER_Y + 20.0f);
    float buttonX = containerX + Scale(20.0f);
    float buttonWidth = containerWidth - Scale(40.0f);
    float buttonHeight = Scale(28.0f);
    float buttonSpacing = Scale(8.0f);
    
    // "No Update (Original)" option
    {
        ImVec2 min = { buttonX, buttonY };
        ImVec2 max = { buttonX + buttonWidth, buttonY + buttonHeight };
        
        bool isSelected = (g_selectedTitleUpdateIndex == -1);
        bool buttonPressed = false;
        
        std::string label = isSelected ? "[X] " : "[ ] ";
        label += Localise("Installer_TitleUpdate_None");
        
        DrawButton(min, max, label.c_str(), false, true, buttonPressed, buttonWidth - Scale(20.0f), isSelected);
        
        if (buttonPressed)
        {
            g_selectedTitleUpdateIndex = -1;
            g_titleUpdateManager.SelectUpdate(-1);
        }
        
        buttonY += buttonHeight + buttonSpacing;
    }
    
    // List detected updates
    int index = 0;
    for (const auto& update : updates)
    {
        if (buttonY > g_aspectRatioOffsetY + Scale(CONTAINER_Y + CONTAINER_HEIGHT - 40.0f))
            break;  // Don't overflow container
            
        ImVec2 min = { buttonX, buttonY };
        ImVec2 max = { buttonX + buttonWidth, buttonY + buttonHeight };
        
        bool isSelected = (g_selectedTitleUpdateIndex == index);
        bool buttonPressed = false;
        
        std::string label = isSelected ? "[X] " : "[ ] ";
        label += liberty::install::TitleUpdateManager::GetUpdateDisplayName(update.info);
        
        DrawButton(min, max, label.c_str(), false, true, buttonPressed, buttonWidth - Scale(20.0f), isSelected);
        
        if (buttonPressed)
        {
            g_selectedTitleUpdateIndex = index;
            g_titleUpdateManager.SelectUpdate(index);
        }
        
        buttonY += buttonHeight + buttonSpacing;
        index++;
    }
    
    // Show hint if no updates found
    if (updates.empty())
    {
        float hintY = buttonY + Scale(20.0f);
        const char* hintText = "Place Title Update files in 'GAME UPDATES' folder";
        float hintFontSize = Scale(14.0f);
        drawList->AddText(g_pFntRodin, hintFontSize, { buttonX, hintY }, 
            GTA4Style::WithAlpha(GTA4Style::Colors::TextGray, alpha), hintText);
    }
}

static void DrawSources()
{
    if (g_currentPage == WizardPage::SelectGame)
    {
        DrawSourceButton(ButtonColumnMiddle, 0, Localise("Installer_Step_Game").c_str(), !g_gameSourcePath.empty());
    }

    if (g_currentPage == WizardPage::SelectTitleUpdate)
    {
        DrawTitleUpdateSelection();
    }

    if (g_currentPage == WizardPage::SelectDLC)
    {
        // Use new GTA IV style DLC selection
        DrawGTA4DLCSelection();
    }
}

static void DrawInstallingProgress()
{
    if (g_currentPage == WizardPage::Installing)
    {
        constexpr float ProgressSpeed = 0.1f;
        float ratioTarget = g_installerProgressRatioTarget.load();
        g_installerProgressRatioCurrent += std::min(ratioTarget - g_installerProgressRatioCurrent, ProgressSpeed * ImGui::GetIO().DeltaTime);
        DrawProgressBar(g_installerProgressRatioCurrent);

        if (g_installerFinished)
        {
            g_installerThread->join();
            g_installerThread.reset();
            g_installerEndTime = ImGui::GetTime();
            SetCurrentPage(g_installerFailed ? WizardPage::InstallFailed : WizardPage::InstallSucceeded);
            g_commonMenu.SetTitle(Localise("Installer_Header_Installer"));
        }
    }
}

static void InstallerThread()
{
    if (!Installer::install(g_installerSources, g_installPath, false, g_installerJournal, std::chrono::seconds(1), [&]() {
        g_installerProgressRatioTarget = float(double(g_installerJournal.progressCounter) / double(g_installerJournal.progressTotal));

        // If user is being asked for confirmation on cancelling the installation, halt the installer from progressing further.
        g_installerHalted.wait(true);

        // If user has confirmed they wish to cancel the installation, return false to indicate the installer should fail and stop.
        return !g_installerCancelled.load();
    }))
    {
        g_installerFailed = true;
        g_installerErrorMessage = g_installerJournal.lastErrorMessage;

        // Delete all files that were copied.
        Installer::rollback(g_installerJournal);
    }

    // On success, persist the source directories to config so AutoInstaller
    // can find DLC / updates on future launches without user intervention.
    if (!g_installerFailed)
    {
        if (!g_dlcSourcePaths[0].empty())
            Config::DLCSourceDir = g_dlcSourcePaths[0].parent_path().string();
        else if (!g_dlcSourcePaths[1].empty())
            Config::DLCSourceDir = g_dlcSourcePaths[1].parent_path().string();

        if (g_titleUpdateManager.HasSelectedUpdate())
        {
            auto sel = g_titleUpdateManager.GetSelectedUpdate();
            if (sel.has_value())
                Config::UpdateSourceDir = sel->path.parent_path().string();
        }

        Config::Save();
    }

    g_installerFinished = true;
    g_installerCancelled = false;
}

static void InstallerStart()
{
    SetCurrentPage(WizardPage::Installing);
    g_installerStartTime = ImGui::GetTime();
    g_installerEndTime = DBL_MAX;
    g_installerProgressRatioCurrent = 0.0f;
    g_installerProgressRatioTarget = 0.0f;
    g_installerFailed = false;
    g_installerFinished = false;
    g_installerThread = std::make_unique<std::thread>(InstallerThread);
    g_commonMenu.SetTitle(Localise("Installer_Header_Installing"));
}

static bool InstallerParseSources(std::string &errorMessage)
{
    std::error_code spaceErrorCode;
    std::filesystem::space_info spaceInfo = std::filesystem::space(g_installPath, spaceErrorCode);
    if (!spaceErrorCode)
    {
        g_installerAvailableSize = spaceInfo.available;
    }

    Installer::Input installerInput;
    installerInput.gameSource = g_gameSourcePath;

    // PHASE 4: Add selected title update to installer input
    if (g_titleUpdateManager.HasSelectedUpdate())
    {
        auto selectedUpdate = g_titleUpdateManager.GetSelectedUpdate();
        if (selectedUpdate)
        {
            installerInput.updateSource = selectedUpdate->path;
        }
    }

    for (std::filesystem::path &path : g_dlcSourcePaths)
    {
        if (!path.empty())
        {
            installerInput.dlcSources.push_back(path);
        }
    }

    bool sourcesParsed = Installer::parseSources(installerInput, g_installerJournal, g_installerSources);
    errorMessage = g_installerJournal.lastErrorMessage;
    return sourcesParsed;
}

static void DrawNavigationButton()
{
    if (g_currentPage == WizardPage::Installing)
    {
        // Navigation buttons are not offered during installation at the moment.
        return;
    }

    bool nextButtonEnabled = !g_isDisappearing && (g_currentPage != WizardPage::Installing);
    if (nextButtonEnabled && g_currentPage == WizardPage::SelectGame)
    {
        nextButtonEnabled = !g_gameSourcePath.empty();
    }

    bool skipButton = false;
    if (g_currentPage == WizardPage::SelectDLC)
    {
        skipButton = std::all_of(g_dlcSourcePaths.begin(), g_dlcSourcePaths.end(), [](const std::filesystem::path &path) { return path.empty(); });
    }

    float squashRatio;
    constexpr float NAV_BUTTON_MAX_TEXT_WIDTH = 90.0f;
    std::string_view nextButtonKey = "Installer_Button_Next";
    if (skipButton)
    {
        nextButtonKey = "Installer_Button_Skip";
    }
    else if (g_currentPage == WizardPage::InstallFailed)
    {
        nextButtonKey = "Installer_Button_Retry";
    }

    const std::string &nextButtonText = Localise(nextButtonKey);
    ImVec2 nextTextSize = ComputeTextSize(g_pFntRodin, nextButtonText.c_str(), 20.0f, squashRatio, NAV_BUTTON_MAX_TEXT_WIDTH);
    
    // Center the button position to match centered container
    auto &res = ImGui::GetIO().DisplaySize;
    float containerWidth = Scale(CONTAINER_WIDTH);
    float containerX = (res.x - containerWidth) / 2.0f;
    
    ImVec2 min = { containerX + containerWidth - Scale(nextTextSize.x * squashRatio + BOTTOM_X_GAP + BUTTON_TEXT_GAP), g_aspectRatioOffsetY + Scale(CONTAINER_Y + CONTAINER_HEIGHT + BOTTOM_Y_GAP) };
    ImVec2 max = { containerX + containerWidth - Scale(BOTTOM_X_GAP), g_aspectRatioOffsetY + Scale(CONTAINER_Y + CONTAINER_HEIGHT + BOTTOM_Y_GAP + BUTTON_HEIGHT) };

    bool buttonPressed = false;
    DrawButton(min, max, nextButtonText.c_str(), false, nextButtonEnabled, buttonPressed, NAV_BUTTON_MAX_TEXT_WIDTH);

    if (buttonPressed)
    {
        if (g_currentPage == WizardPage::SelectDLC)
        {
            // PHASE 1: Validate DLC-Update requirement
            if (!ValidateDLCUpdateRequirement())
            {
                // DLC selected but TU8 not selected - show error
                std::stringstream errorMsg;
                errorMsg << "DLC Content Requires Title Update 1.06 (TU8)\n\n";
                errorMsg << "The Lost and Damned and The Ballad of Gay Tony\n";
                errorMsg << "require the latest title update to function correctly.\n\n";
                
                // Check if TU8 is available
                const auto& updates = g_titleUpdateManager.GetDetectedUpdates();
                bool tu8Available = false;
                for (const auto& update : updates)
                {
                    if (update.info.version == 8)
                    {
                        tu8Available = true;
                        break;
                    }
                }
                
                if (tu8Available)
                {
                    errorMsg << "Press Y to auto-select TU8, or\n";
                    errorMsg << "Press BACK to manually select Title Update";
                }
                else
                {
                    errorMsg << "TU8 not found in GAME UPDATES folder.\n";
                    errorMsg << "Please add TU8 or deselect DLC.";
                }
                
                g_currentMessagePrompt = errorMsg.str();
                g_currentMessagePromptConfirmation = false;
                Game_PlaySound("error");
                return;  // Block navigation
            }
            
            bool dlcInstallerMode = g_gameSourcePath.empty();
            std::string sourcesErrorMessage;
            if (!InstallerParseSources(sourcesErrorMessage))
            {
                // Some of the sources that were provided to the installer are not valid. Restart the file selection process.
                std::stringstream stringStream;
                stringStream << Localise("Installer_Message_InvalidFiles");
                if (!sourcesErrorMessage.empty()) {
                    stringStream << std::endl << std::endl << sourcesErrorMessage;
                }

                g_currentMessagePrompt = stringStream.str();
                g_currentMessagePromptConfirmation = false;
                SetCurrentPage(dlcInstallerMode ? WizardPage::SelectDLC : WizardPage::SelectGame);
            }
            else if (skipButton && dlcInstallerMode)
            {
                // Nothing was selected and the installer was in DLC mode, just close it.
                g_isDisappearing = true;
                g_disappearTime = ImGui::GetTime();
            }
            else
            {
                SetCurrentPage(WizardPage::CheckSpace);
            }
        }
        else if (g_currentPage == WizardPage::CheckSpace)
        {
            InstallerStart();
        }
        else if (g_currentPage == WizardPage::InstallSucceeded)
        {
            g_isDisappearing = true;
            g_disappearTime = ImGui::GetTime();
        }
        else if (g_currentPage == WizardPage::InstallFailed)
        {
            SetCurrentPage(g_firstPage);
        }
        else if (g_currentPage == WizardPage::SelectGame)
        {
            // STEP 1: Validate game version/region first (like Banjo:Recompiled)
            auto versionResult = Installer::validateGameVersion(g_gameSourcePath);
            if (!versionResult.isValid)
            {
                // XEX parsing failed - show error
                std::stringstream errorMsg;
                errorMsg << versionResult.errorTitle << "\n\n";
                errorMsg << versionResult.errorMessage;
                g_currentMessagePrompt = errorMsg.str();
                g_currentMessagePromptConfirmation = false;
                Game_PlaySound("error");
                return;
            }
            else if (!versionResult.isCorrectGame)
            {
                // Wrong game detected
                std::stringstream errorMsg;
                errorMsg << Localise("Installer_Message_WrongGame") << "\n\n";
                errorMsg << versionResult.errorMessage;
                g_currentMessagePrompt = errorMsg.str();
                g_currentMessagePromptConfirmation = false;
                Game_PlaySound("error");
                return;
            }
            else if (!versionResult.isCorrectRegion)
            {
                // Correct game but wrong region
                std::stringstream errorMsg;
                errorMsg << Localise("Installer_Message_WrongRegion") << "\n\n";
                errorMsg << Localise("Installer_Message_DetectedRegion") << ": " << versionResult.detectedRegion << "\n";
                errorMsg << Localise("Installer_Message_RequiredRegion") << ": " << versionResult.requiredRegion;
                g_currentMessagePrompt = errorMsg.str();
                g_currentMessagePromptConfirmation = false;
                Game_PlaySound("error");
                return;
            }
            
            // STEP 2: Validate game source files
            std::string sourcesErrorMessage;
            if (!InstallerParseSources(sourcesErrorMessage))
            {
                std::stringstream stringStream;
                stringStream << Localise("Installer_Message_InvalidFiles");
                if (!sourcesErrorMessage.empty()) {
                    stringStream << std::endl << std::endl << sourcesErrorMessage;
                }
                g_currentMessagePrompt = stringStream.str();
                g_currentMessagePromptConfirmation = false;
            }
            else
            {
                // GTA IV: Go to Title Update selection after game selection
                SetCurrentPage(WizardPage::SelectTitleUpdate);
            }
        }
        else if (g_currentPage == WizardPage::SelectTitleUpdate)
        {
            // Validate Title Update compatibility before proceeding
            if (g_titleUpdateManager.HasSelectedUpdate())
            {
                auto selectedUpdate = g_titleUpdateManager.GetSelectedUpdate();
                if (selectedUpdate)
                {
                    std::string compatError = Installer::checkGameUpdateCompatibility(
                        g_gameSourcePath, selectedUpdate->path);
                    
                    if (!compatError.empty())
                    {
                        std::stringstream errorMsg;
                        errorMsg << "Incompatible Title Update:\n\n";
                        errorMsg << compatError;
                        
                        g_currentMessagePrompt = errorMsg.str();
                        g_currentMessagePromptConfirmation = false;
                        Game_PlaySound("error");
                        return;  // Block navigation
                    }
                }
            }
            
            // Proceed to DLC selection
            SetCurrentPage(WizardPage::SelectDLC);
        }
        else
        {
            SetCurrentPage(WizardPage(int(g_currentPage) + 1));
        }
    }
}

static void CheckCancelAction()
{
    if (!g_currentCursorBack)
    {
        return;
    }
    
    g_currentCursorBack = false;

    if (g_currentPage == WizardPage::InstallSucceeded)
    {
        // Nothing to back out on this page.
        return;
    }
    if (g_currentPage == WizardPage::Installing && g_installerCancelled)
    {
        // Installer's already been cancelled, no need for more confirmations.
        return;
    }

    Game_PlaySound("cursor2");

    if (g_currentPage == g_firstPage || g_currentPage == WizardPage::InstallFailed)
    {
        // Ask for confirmation if user wants to quit the installer.
        g_currentMessagePrompt = Localise("Installer_Message_Quit");
        g_currentMessagePromptSource = MessagePromptSource::Back;
        g_currentMessagePromptConfirmation = true;
    }
    else if (g_currentPage == WizardPage::Installing)
    {
        // Ask for confirmation if the user wants to cancel the installation.
        g_currentMessagePrompt = Localise("Installer_Message_Cancel");
        g_currentMessagePromptSource = MessagePromptSource::Back;
        g_currentMessagePromptConfirmation = true;

        // Indicate to the installer that all progress should stop until the user confirms if they wish to cancel.
        g_installerHalted = true;
    }
    else if (int(g_currentPage) > 0)
    {
        // Go back to the previous page
        WizardPage prevPage = WizardPage(int(g_currentPage) - 1);
        SetCurrentPage(prevPage);
    }
}

static void DrawMessagePrompt()
{
    if (g_currentMessagePrompt.empty())
    {
        return;
    }

    bool messageWindowReturned = false;
    if (g_currentMessagePromptConfirmation)
    {
        std::array<std::string, 2> YesNoButtons = { Localise("Common_Yes"), Localise("Common_No") };
        messageWindowReturned = MessageWindow::Open(g_currentMessagePrompt, &g_currentMessageResult, YesNoButtons, 1);
    }
    else
    {
        messageWindowReturned = MessageWindow::Open(g_currentMessagePrompt, &g_currentMessageResult);
    }

    if (messageWindowReturned)
    {
        if (g_currentMessagePromptConfirmation && (g_currentMessageResult == 0))
        {
            if (g_currentMessagePromptSource == MessagePromptSource::Back)
            {
                if (g_currentPage == WizardPage::Installing)
                {
                    // If user confirms they wish to cancel the installation, notify the installation thread it must finish as soon as possible.
                    g_installerCancelled = true;
                }
                else
                {
                    // In all cases, proceed to just quit the application.
                    g_isQuitting = true;
                    g_isDisappearing = true;
                    g_disappearTime = ImGui::GetTime();
                }
            }
            else if (g_currentPage == WizardPage::SelectDLC)
            {
                // If user confirms the message prompt that they wish to skip installing the DLC, proceed to the next step.
                SetCurrentPage(WizardPage::CheckSpace);
            }
        }

        if (g_currentMessagePromptSource == MessagePromptSource::Back)
        {
            // Regardless of the confirmation, the installation thread must be resumed.
            g_installerHalted = false;
            g_installerHalted.notify_all();
        }

        g_currentMessagePrompt.clear();
        g_currentMessagePromptSource = MessagePromptSource::Unknown;
        g_currentMessageResult = -1;
    }
}

static void PickerDrawForeground()
{
    if (g_currentPickerVisible)
    {
        auto drawList = ImGui::GetBackgroundDrawList();
        drawList->AddRectFilled({ 0.0f, 0.0f }, ImGui::GetIO().DisplaySize, IM_COL32(0, 0, 0, 190));
    }
}

static void PickerCheckTutorial()
{
    if (!g_pickerTutorialTriggered || !g_currentMessagePrompt.empty())
    {
        return;
    }

    PickerStart(g_pickerTutorialFolderMode);
    g_pickerTutorialTriggered = false;
}

static void PickerCheckResults()
{
    if (!g_currentPickerResultsReady)
    {
        return;
    }

    if (!g_currentPickerErrorMessage.empty())
    {
        g_currentMessagePrompt = g_currentPickerErrorMessage;
        g_currentMessagePromptConfirmation = false;
        g_currentPickerErrorMessage.clear();
    }

    if (!g_currentPickerResults.empty() && ParseSourcePaths(g_currentPickerResults))
    {
        g_pickerTutorialCleared[g_pickerTutorialFolderMode] = true;
    }

    g_currentPickerResultsReady = false;
    g_currentPickerVisible = false;
}

static bool g_fadingOutMusic;

static void ProcessMusic()
{
    if (g_isDisappearing)
    {
        if (!g_fadingOutMusic)
        {
            EmbeddedPlayer::FadeOutMusic();
            g_fadingOutMusic = true;
        }
    }
    else
    {
        // Music disabled for clean installer
        // EmbeddedPlayer::PlayMusic();
    }
}

void InstallerWizard::Init()
{
    auto &io = ImGui::GetIO();

    g_commonMenu = CommonMenu(Localise("Installer_Header_Installer"), "", true);
    
    // Character and logo textures disabled for clean installer
    // g_installTextures[0] = LOAD_ZSTD_TEXTURE(g_install_001);
    // g_installTextures[1] = LOAD_ZSTD_TEXTURE(g_install_002);
    // g_installTextures[2] = LOAD_ZSTD_TEXTURE(g_install_003);
    // g_installTextures[3] = LOAD_ZSTD_TEXTURE(g_install_004);
    // g_installTextures[4] = LOAD_ZSTD_TEXTURE(g_install_005);
    // g_installTextures[5] = LOAD_ZSTD_TEXTURE(g_install_006);
    // g_installTextures[6] = LOAD_ZSTD_TEXTURE(g_install_007);
    // g_installTextures[7] = LOAD_ZSTD_TEXTURE(g_install_008);
    
#ifdef LIBERTY_RECOMP_HAS_RESOURCES
    g_upLibertyDev = LOAD_ZSTD_TEXTURE(g_libertyrecomp);
#endif
    
    // Logo textures disabled for clean installer
    // g_upGTA4Logo = LOAD_ZSTD_TEXTURE(g_gta4_logo);
    // g_upTLADLogo = LOAD_ZSTD_TEXTURE(g_tlad_logo);
    // g_upTBOGTLogo = LOAD_ZSTD_TEXTURE(g_tbogt_logo);

    for (int i = 0; i < g_credits.size(); i++)
    {
        g_creditsStr += g_credits[i];
        g_creditsStr += "  ";
    }
}

void InstallerWizard::Draw()
{
    if (!s_isVisible)
        return;

    ResetCursorRects();
    DrawBackground();
    DrawGTA4Logo();
    
    // For DLC selection page, use full-screen GTA IV style layout
    bool isDLCSelectionPage = (g_currentPage == WizardPage::SelectDLC);
    
    if (!isDLCSelectionPage)
    {
        // GTA IV Style: No chevron arrows - skip DrawArrows()
        DrawLeftImage();
        // GTA IV Style: Use simple header instead of common_menu with Sonic textures
        DrawGTA4Header();
        DrawDescriptionContainer();
        DrawLanguagePicker();
        DrawSourcePickers();
    }
    
    DrawSources(); // This now handles the GTA IV DLC selection screen
    
    DrawInstallingProgress();
    
    // Draw navigation button for all pages except DLC selection (which has its own skip hint)
    if (!isDLCSelectionPage)
    {
        DrawNavigationButton();
    }
    
    CheckCancelAction();
    DrawMessagePrompt();
    PickerDrawForeground();
    PickerCheckTutorial();
    PickerCheckResults();

    if (g_isDisappearing)
    {
        double disappearDuration = ALL_ANIMATIONS_FULL_DURATION / 60.0;
        if (g_isQuitting)
        {
            // Add some extra waiting time when quitting the application altogether.
            disappearDuration += QUITTING_EXTRA_DURATION / 60.0;
        }

        if (ImGui::GetTime() > (g_disappearTime + disappearDuration))
        {
            s_isVisible = false;
        }
    }
}

void InstallerWizard::Shutdown()
{
    // Wait for and erase the threads.
    if (g_installerThread != nullptr)
    {
        g_installerThread->join();
        g_installerThread.reset();
    }

    if (g_currentPickerThread != nullptr)
    {
        g_currentPickerThread->join();
        g_currentPickerThread.reset();
    }

    // Clean up any temp extraction directories
    Installer::cleanupTempFiles();

    // Erase the sources.
    g_installerSources.game.reset();
    g_installerSources.dlc.clear();
    
    // Make sure the GPU is not currently active before deleting these textures.
    Video::WaitForGPU();

    // Erase the textures - disabled since g_installTextures is not used
    // for (auto &texture : g_installTextures)
    // {
    //     texture.reset();
    // }
}

bool InstallerWizard::Run(std::filesystem::path installPath, bool skipGame)
{
    g_installPath = installPath;

    EmbeddedPlayer::Init();
    NFD_Init();

    // Guarantee one controller is initialized. We'll rely on SDL's event loop to get the controller events.
    XAMINPUT_STATE inputState;
    hid::GetState(0, &inputState);

    if (skipGame)
    {
        for (int i = 0; i < int(DLC::Count); i++)
        {
            g_dlcInstalled[i] = Installer::checkDLCInstall(g_installPath, DLC(i + 1));
        }

        g_firstPage = WizardPage::SelectDLC;
    }

    SetCurrentPage(g_firstPage);

    GameWindow::SetFullscreenCursorVisibility(true);
    s_isVisible = true;

    while (s_isVisible)
    {
        Video::WaitOnSwapChain();
        ProcessMusic();
        SDL_PumpEvents();
        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
        GameWindow::Update();
        Video::Present();
    }

    GameWindow::SetFullscreenCursorVisibility(false);
    NFD_Quit();

    InstallerWizard::Shutdown();
    EmbeddedPlayer::Shutdown();

    return !g_isQuitting;
}
