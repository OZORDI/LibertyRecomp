#include "button_window.h"
#include <patches/aspect_ratio_patches.h>
#include <ui/imgui_utils.h>
#include <app.h>

static std::string g_buttonKey{};
static double g_time{};
static bool g_isAnimated{};

void ButtonWindow::Draw()
{
    // GTA IV Style: Disabled Sonic-style keycap button window
    // The button prompts are now handled differently in GTA IV style UI
    return;
}

void ButtonWindow::Open(std::string key, bool isAnimated)
{
    s_isVisible = true;

    if (g_buttonKey != key)
    {
        g_buttonKey = key;
        g_time = ImGui::GetTime();
        g_isAnimated = isAnimated;
    }
}

void ButtonWindow::Close()
{
    s_isVisible = false;
    g_buttonKey.clear();
}
