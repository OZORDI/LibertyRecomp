#include "fader.h"
#include "imgui_utils.h"
#include <user/config.h>

static bool g_isFadeIn;

static float g_startTime;

static float g_duration;
static ImU32 g_colour = IM_COL32(0, 0, 0, 255);
static std::function<void()> g_endCallback;
static float g_endCallbackDelay;

void Fader::Draw()
{
    if (!s_isVisible)
        return;

    auto time = (ImGui::GetTime() - g_startTime) / g_duration;
    auto alpha = 1.0f;

    if (time >= g_duration)
    {
        if (g_endCallback && time >= g_duration + g_endCallbackDelay)
            g_endCallback();
    }
    else
    {
        alpha = g_isFadeIn
            ? Lerp(1, 0, time)
            : Lerp(0, 1, time);
    }

    auto colour = IM_COL32(g_colour & 0xFF, (g_colour >> 8) & 0xFF, (g_colour >> 16) & 0xFF, 255 * alpha);

    ImGui::GetForegroundDrawList()->AddRectFilled({ 0, 0 }, ImGui::GetIO().DisplaySize, colour);
}

static void DoFade(float duration, std::function<void()> endCallback, float endCallbackDelay)
{
    Fader::s_isVisible = true;
    g_startTime = ImGui::GetTime();
    g_duration = duration;
    g_endCallback = endCallback;
    g_endCallbackDelay = endCallbackDelay;
}

void Fader::SetFadeColour(ImU32 colour)
{
    g_colour = colour;
}

void Fader::FadeIn(float duration, std::function<void()> endCallback, float endCallbackDelay)
{
    g_isFadeIn = true;
    DoFade(duration, endCallback, endCallbackDelay);
}

void Fader::FadeOut(float duration, std::function<void()> endCallback, float endCallbackDelay)
{
    g_isFadeIn = false;
    DoFade(duration, endCallback, endCallbackDelay);
}
