#include "achievement_overlay.h"
#include <gpu/imgui/imgui_snapshot.h>
#include <gpu/video.h>
#include <kernel/memory.h>
#include <kernel/xdbf.h>
#include <locale/locale.h>
#include <ui/imgui_utils.h>
#include <user/achievement_data.h>
#include <user/config.h>
#include <app.h>
#include <exports.h>
#include <decompressor.h>

constexpr double OVERLAY_CONTAINER_COMMON_MOTION_START = 0;
constexpr double OVERLAY_CONTAINER_COMMON_MOTION_END = 11;
constexpr double OVERLAY_CONTAINER_INTRO_FADE_START = 5;
constexpr double OVERLAY_CONTAINER_INTRO_FADE_END = 9;
constexpr double OVERLAY_CONTAINER_OUTRO_FADE_START = 0;
constexpr double OVERLAY_CONTAINER_OUTRO_FADE_END = 4;

constexpr double OVERLAY_DURATION = 5.0; // GTA IV style - slightly longer display

static bool g_isClosing = false;

static double g_appearTime = 0;

static Achievement g_achievement;

static ImFont* g_rodinFont;

// Draw a rounded rectangle with semi-transparent background (GTA IV style)
static bool DrawContainer(ImVec2 min, ImVec2 max, float cornerRadius = 12.0f)
{
    auto drawList = ImGui::GetBackgroundDrawList();

    // Expand/retract animation.
    auto containerMotion = ComputeMotion(g_appearTime, OVERLAY_CONTAINER_COMMON_MOTION_START, OVERLAY_CONTAINER_COMMON_MOTION_END);

    auto centreX = (min.x + max.x) / 2;
    auto centreY = (min.y + max.y) / 2;

    if (g_isClosing)
    {
        min.x = Hermite(min.x, centreX, containerMotion);
        max.x = Hermite(max.x, centreX, containerMotion);
        min.y = Hermite(min.y, centreY, containerMotion);
        max.y = Hermite(max.y, centreY, containerMotion);
    }
    else
    {
        min.x = Hermite(centreX, min.x, containerMotion);
        max.x = Hermite(centreX, max.x, containerMotion);
        min.y = Hermite(centreY, min.y, containerMotion);
        max.y = Hermite(centreY, max.y, containerMotion);
    }

    // Transparency fade animation.
    auto colourMotion = g_isClosing
        ? ComputeMotion(g_appearTime, OVERLAY_CONTAINER_OUTRO_FADE_START, OVERLAY_CONTAINER_OUTRO_FADE_END)
        : ComputeMotion(g_appearTime, OVERLAY_CONTAINER_INTRO_FADE_START, OVERLAY_CONTAINER_INTRO_FADE_END);

    auto alpha = g_isClosing
        ? Hermite(1.0f, 0.0f, colourMotion)
        : Hermite(0.0f, 1.0f, colourMotion);

    // Draw semi-transparent dark background (GTA IV style)
    uint8_t bgAlpha = static_cast<uint8_t>(200 * alpha);
    drawList->AddRectFilled(min, max, IM_COL32(20, 20, 20, bgAlpha), cornerRadius);
    
    // Draw subtle border
    uint8_t borderAlpha = static_cast<uint8_t>(150 * alpha);
    drawList->AddRect(min, max, IM_COL32(80, 80, 80, borderAlpha), cornerRadius, 0, 2.0f);

    if (containerMotion >= 1.0f)
    {
        drawList->PushClipRect(min, max);
        return true;
    }

    return false;
}

void AchievementOverlay::Init()
{
    auto& io = ImGui::GetIO();

    g_rodinFont = ImFontAtlasSnapshot::GetFont("FOT-RodinPro-DB.otf");
}

// Dequeue achievements only in the main thread for thread safety.
static std::thread::id g_mainThreadId = std::this_thread::get_id();

static bool CanDequeueAchievement()
{
    // Allow dequeuing if:
    // 1. We have achievements in the queue
    // 2. We're in the main thread
    if (AchievementOverlay::s_queue.empty())
        return false;
    
    if (std::this_thread::get_id() != g_mainThreadId)
        return false;
    
    return true;
}

void AchievementOverlay::Draw()
{
    if (!AchievementOverlay::s_isVisible && CanDequeueAchievement())
    {
        s_isVisible = true;
        g_isClosing = false;
        g_appearTime = ImGui::GetTime();
        g_achievement = g_xdbfWrapper.GetAchievement((EXDBFLanguage)Config::Language.Value, s_queue.front());
        s_queue.pop();
        
        if (Config::Language == ELanguage::English)
            g_achievement.Name = xdbf::FixInvalidSequences(g_achievement.Name);
    }

    if (!s_isVisible)
        return;
    
    if (ImGui::GetTime() - g_appearTime >= OVERLAY_DURATION)
        AchievementOverlay::Close();

    auto drawList = ImGui::GetBackgroundDrawList();
    auto& res = ImGui::GetIO().DisplaySize;

    auto strAchievementUnlocked = Localise("Achievements_Unlock").c_str();
    auto strAchievementName = g_achievement.Name.c_str();
    auto strGamerScore = std::to_string(g_achievement.Score) + "G";

    // Calculate text sizes.
    auto fontSize = Scale(22);
    auto smallFontSize = Scale(18);
    auto headerSize = g_rodinFont->CalcTextSizeA(fontSize, FLT_MAX, 0, strAchievementUnlocked);
    auto bodySize = g_rodinFont->CalcTextSizeA(fontSize, FLT_MAX, 0, strAchievementName);
    auto scoreSize = g_rodinFont->CalcTextSizeA(smallFontSize, FLT_MAX, 0, strGamerScore.c_str());
    auto maxTextWidth = std::max({headerSize.x, bodySize.x, scoreSize.x}) + Scale(10);

    // Calculate image margins.
    auto imageMarginX = Scale(15);
    auto imageMarginY = Scale(15);
    auto imageSize = Scale(64); // Xbox achievement icon size

    // Calculate text margins.
    auto textMarginX = imageMarginX + imageSize + Scale(15);
    auto textMarginY = imageMarginY;

    auto containerWidth = textMarginX + maxTextWidth + Scale(20);
    auto containerHeight = Scale(94);

    // Position at BOTTOM CENTER of screen
    float bottomMargin = Scale(80);
    ImVec2 min = { (res.x / 2) - (containerWidth / 2), res.y - bottomMargin - containerHeight };
    ImVec2 max = { min.x + containerWidth, min.y + containerHeight };

    if (DrawContainer(min, max))
    {
        if (!g_isClosing)
        {
            // Draw achievement icon if available
            auto iconIt = g_xdbfTextureCache.find(g_achievement.ID);
            if (iconIt != g_xdbfTextureCache.end() && iconIt->second != nullptr)
            {
                drawList->AddImage
                (
                    iconIt->second,
                    { min.x + imageMarginX, min.y + imageMarginY },
                    { min.x + imageMarginX + imageSize, min.y + imageMarginY + imageSize },
                    { 0, 0 },
                    { 1, 1 },
                    IM_COL32(255, 255, 255, 255)
                );
            }
            else
            {
                // Draw placeholder rectangle if no icon
                drawList->AddRectFilled(
                    { min.x + imageMarginX, min.y + imageMarginY },
                    { min.x + imageMarginX + imageSize, min.y + imageMarginY + imageSize },
                    IM_COL32(60, 60, 60, 255),
                    4.0f
                );
            }

            // Use low quality text.
            SetShaderModifier(IMGUI_SHADER_MODIFIER_LOW_QUALITY_TEXT);

            // Draw "Achievement Unlocked" header (green color like Xbox)
            float textY = min.y + textMarginY + Scale(5);
            DrawTextWithShadow
            (
                g_rodinFont,
                fontSize,
                { min.x + textMarginX, textY },
                IM_COL32(80, 200, 80, 255), // Xbox green
                strAchievementUnlocked,
                1,
                0.5f,
                IM_COL32(0, 0, 0, 180)
            );

            // Draw achievement name (white)
            textY += headerSize.y + Scale(6);
            DrawTextWithShadow
            (
                g_rodinFont,
                fontSize,
                { min.x + textMarginX, textY },
                IM_COL32(255, 255, 255, 255),
                strAchievementName,
                1,
                0.5f,
                IM_COL32(0, 0, 0, 180)
            );

            // Draw gamerscore (yellow/gold)
            textY += bodySize.y + Scale(4);
            DrawTextWithShadow
            (
                g_rodinFont,
                smallFontSize,
                { min.x + textMarginX, textY },
                IM_COL32(255, 215, 0, 255), // Gold
                strGamerScore.c_str(),
                1,
                0.5f,
                IM_COL32(0, 0, 0, 180)
            );

            // Reset low quality text shader modifier.
            SetShaderModifier(IMGUI_SHADER_MODIFIER_NONE);
        }
        else
        {
            s_isVisible = false;
        }

        // Pop clip rect from DrawContainer.
        drawList->PopClipRect();
    }
}

void AchievementOverlay::Open(int id)
{
    s_queue.push(id);
}

void AchievementOverlay::Close()
{
    if (!g_isClosing)
    {
        g_appearTime = ImGui::GetTime();
        g_isClosing = true;
    }

    // When closing animation is done, allow visibility to be reset
    auto containerMotion = ComputeMotion(g_appearTime, OVERLAY_CONTAINER_COMMON_MOTION_START, OVERLAY_CONTAINER_COMMON_MOTION_END);
    if (containerMotion >= 1.0f)
        s_isVisible = false;
}
