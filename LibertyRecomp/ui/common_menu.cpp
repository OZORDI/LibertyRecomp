#include "common_menu.h"
#include <patches/aspect_ratio_patches.h>
#include <ui/imgui_utils.h>
#include <ui/gta4_style.h>
#include <app.h>

static constexpr double ANIMATION_DURATION = 10.0;

void CommonMenu::Draw()
{
    auto* drawList = ImGui::GetBackgroundDrawList();
    auto& res = ImGui::GetIO().DisplaySize;

    if (App::s_isInit)
        SetShaderModifier(IMGUI_SHADER_MODIFIER_LOW_QUALITY_TEXT);

    ImVec2 min = { g_horzCentre, g_vertCentre };
    ImVec2 max = { res.x - min.x, res.y - min.y };

    auto borderMotionTime = PlayTransitions ? ComputeLinearMotion(m_time, 0, 10, m_isClosing) : 1.0;

    auto topPlateCornerUVs = PIXELS_TO_UV_COORDS(1024, 1024, 0, 154, 250, 144);
    auto topPlateLeftStretchUVs = PIXELS_TO_UV_COORDS(1024, 1024, 2, 154, 150, 144);
    auto topPlateRightStretchUVs = PIXELS_TO_UV_COORDS(1024, 1024, 250, 154, 750, 144);
    auto topPlateHeight = Scale(145, true);
    auto topPlateMotion = Lerp(min.y - topPlateHeight, min.y - Scale(0.2, true), borderMotionTime);

    ImVec2 topPlateCornerMin = { min.x - Scale(46, true), topPlateMotion };
    ImVec2 topPlateCornerMax = { topPlateCornerMin.x + Scale(250, true), topPlateMotion + topPlateHeight };
    ImVec2 topPlateStretchMin = { topPlateCornerMax.x, topPlateCornerMin.y };
    ImVec2 topPlateStretchMax = { res.x, topPlateCornerMax.y };

    auto redStripCornerUVs = PIXELS_TO_UV_COORDS(1024, 1024, 0, 301, 300, 50);
    auto redStripLeftStretchUVs = PIXELS_TO_UV_COORDS(1024, 1024, 0, 301, 150, 50);
    auto redStripRightStretchUVs = PIXELS_TO_UV_COORDS(1024, 1024, 300, 301, 300, 50);
    // GTA IV Style: Orange strip instead of red
    auto redStripColour = GTA4Style::Colors::Orange;
    auto redStripOffsetY = Scale(81.5, true);
    auto redStripHeight = Scale(50, true);
    auto redStripMotion = topPlateMotion + redStripOffsetY;

    // TODO: account for metal plate height in motion.
    ImVec2 redStripCornerMin = { min.x - Scale(43.5, true), redStripMotion };
    ImVec2 redStripCornerMax = { redStripCornerMin.x + Scale(300, true), redStripMotion + redStripHeight };

    // GTA IV Style: Dark background with subtle gradient
    auto gradientTop = GTA4Style::Colors::BackgroundDark;
    auto gradientBottom = GTA4Style::Colors::Background;
    auto gradientHeight = Scale(120, true);

    // Draw gradient to fill gap between red strip and top metal plates.
    // GTA IV Style: Solid black background - no textures
    if (!ReduceDraw)
    {
        // Draw solid black header area
        drawList->AddRectFilled({ 0.0f, 0.0f }, { res.x, topPlateMotion + gradientHeight }, GTA4Style::Colors::Background);
        
        // Draw orange accent strip (simple solid rectangle)
        drawList->AddRectFilled({ 0.0f, redStripCornerMin.y }, { res.x, redStripCornerMax.y }, redStripColour);

    }

    auto titleText = Title.empty() ? "DUMMY" : Title.data();
    auto titleFontSize = Scale(33, true);
    auto titleSize = g_pFntNewRodin->CalcTextSizeA(titleFontSize, FLT_MAX, 0.0f, titleText);
    auto titleOffsetX = redStripCornerMax.x - Scale(105, true);
    auto titleOffsetY = redStripMotion + Scale(3.25, true);
    auto titleMotionTime = ComputeLinearMotion(m_titleTime, PlayTransitions && !m_isClosing ? 10 : 0, 10, m_isClosing);
    auto titleOffsetXMotion = Lerp(max.x + titleSize.x, titleOffsetX, titleMotionTime);

    if (!ReduceDraw && !m_previousTitle.empty())
    {
        auto prevTitleAlphaMotionTime = ComputeLinearMotion(m_titleTime, PlayTransitions ? 10 : 0, 3, m_isClosing);

        // Draw previous title fading out.
        drawList->AddText(g_pFntNewRodin, titleFontSize, { titleOffsetX, titleOffsetY }, IM_COL32(255, 255, 255, Lerp(255, 0, prevTitleAlphaMotionTime)), m_previousTitle.data());
    }

    // Draw title.
    drawList->AddText(g_pFntNewRodin, titleFontSize, { titleOffsetXMotion, titleOffsetY }, IM_COL32(255, 255, 255, 255 * titleMotionTime), titleText);

    // GTA IV Style: No metal plates - just solid black background (already drawn above)

    auto textCoverCornerUVs = PIXELS_TO_UV_COORDS(1024, 1024, 801, 400, 150, 150);
    auto textCoverCornerExtendUVs = PIXELS_TO_UV_COORDS(1024, 1024, 801, 400, 125, 150);
    auto textCoverCentreUVs = PIXELS_TO_UV_COORDS(1024, 1024, 950, 400, 50, 150);
    auto textCoverCornerUVCompensation = Scale(2, true);
    auto textCoverOffsetY = Scale(16.4, true);
    auto textCoverWidth = Scale(149.5, true);
    auto textCoverHeight = Scale(150, true);
    auto textCoverMotion = Lerp(max.y + textCoverHeight, max.y - textCoverOffsetY - textCoverHeight, borderMotionTime);
    // GTA IV Style: Dark panel color
    auto textCoverColour = GTA4Style::Colors::BackgroundPanel;

    ImVec2 textCoverCornerLeftMin = { min.x, textCoverMotion };
    ImVec2 textCoverCornerLeftMax = { textCoverCornerLeftMin.x + textCoverWidth, textCoverCornerLeftMin.y + textCoverHeight };
    ImVec2 textCoverCentreMin = { textCoverCornerLeftMax.x, textCoverCornerLeftMin.y };
    ImVec2 textCoverCentreMax = { max.x - textCoverWidth, textCoverCornerLeftMax.y };
    ImVec2 textCoverCornerRightMin = { max.x - textCoverWidth, textCoverCornerLeftMin.y };
    ImVec2 textCoverCornerRightMax = { max.x, textCoverCornerLeftMax.y };

    if (ReduceDraw)
    {
        auto horzMargin = Scale(128, true);

        ImVec2 textCoverClipMin = { textCoverCornerLeftMin.x + horzMargin, textCoverCornerLeftMin.y + Scale(14, true) };
        ImVec2 textCoverClipMax = { textCoverCornerRightMax.x - horzMargin, textCoverCornerRightMax.y - Scale(90, true) };

        drawList->PushClipRect(textCoverClipMin, textCoverClipMax);
    }

    if (!Description.empty())
    {
        auto descFadeScale = Scale(20, true);
        auto descFontSize = Scale(Config::Language == ELanguage::Japanese ? 28 : 27, true);
        auto descSize = g_pFntRodin->CalcTextSizeA(descFontSize, FLT_MAX, 0.0f, Description.data());

        ImVec2 descBoundsMin = { textCoverCentreMin.x - Scale(18, true), textCoverCentreMin.y + Scale(20, true) };
        ImVec2 descBoundsMax = { textCoverCentreMax.x + Scale(18, true), textCoverCentreMax.y - Scale(90, true) };
        auto descBoundsWidth = descBoundsMax.x - descBoundsMin.x;

        m_descPos =
        {
            descBoundsMin.x + ((descBoundsMax.x - descBoundsMin.x) / 2) - (descSize.x / 2),
            descBoundsMin.y + ((descBoundsMax.y - descBoundsMin.y) / 2) - (descSize.y / 2)
        };

        if (descSize.x > descBoundsWidth)
        {
            auto descScrollMax = descSize.x - (descBoundsWidth - descFadeScale * 2);
            auto descScrollSpeed = Scale(150, true);
            auto descScrollDelay = 1.2f;

            if (descScrollMax > 0.0f)
            {
                auto horz = -m_inputListener.RightStickX;

                if (fabs(horz) > 0.25f)
                {
                    m_isDescManualScrolling = true;
                    m_descScrollOffset += horz * descScrollSpeed * App::s_deltaTime;
                }
                else if (m_isDescManualScrolling && fabs(horz) <= 0.25f)
                {
                    m_isDescScrolling = false;
                    m_isDescManualScrolling = false;
                    m_descScrollTimer = 0.0f;
                    m_descScrollDirection = horz > 0.0f ? 1.0f : -1.0f;
                }

                if (!m_isDescManualScrolling)
                {
                    if (!m_isDescScrolling)
                    {
                        m_descScrollTimer += App::s_deltaTime;

                        if (m_descScrollTimer >= descScrollDelay)
                            m_isDescScrolling = true;
                    }

                    if (m_isDescScrolling)
                    {
                        m_descScrollOffset += descScrollSpeed * m_descScrollDirection * App::s_deltaTime;

                        if (m_descScrollOffset >= descScrollMax)
                        {
                            m_isDescScrolling = false;
                            m_descScrollOffset = descScrollMax;
                            m_descScrollTimer = 0.0f;
                            m_descScrollDirection = -1.0f;
                        }
                        else if (m_descScrollOffset <= 0.0f)
                        {
                            m_isDescScrolling = false;
                            m_descScrollOffset = 0;
                            m_descScrollTimer = 0.0f;
                            m_descScrollDirection = 1.0f;
                        }
                    }
                }

                m_descScrollOffset = std::clamp(m_descScrollOffset, 0.0f, descScrollMax);
            }
            else
            {
                m_isDescScrolling = false;
                m_descScrollOffset = 0.0f;
                m_descScrollTimer = 0.0f;
                m_descScrollDirection = 1.0f;
            }

            m_descPos.x = (descBoundsMin.x + descFadeScale) - m_descScrollOffset;
        }

        auto descAlphaMotionTime = ComputeLinearMotion(m_descTime, PlayTransitions ? 10 : 0, 15, m_isClosing);

        // Draw text cover backdrop.
        drawList->AddRectFilled({ 0.0f, textCoverMotion }, { res.x, textCoverMotion + textCoverHeight }, IM_COL32(0, 0, 0, 65));

        // Draw previous description fading out.
        if (!m_previousDesc.empty())
            drawList->AddText(g_pFntRodin, descFontSize, m_previousDescPos, IM_COL32(255, 255, 255, Lerp(255, 0, descAlphaMotionTime)), m_previousDesc.data());

        // Draw description.
        drawList->AddText(g_pFntRodin, descFontSize, m_descPos, IM_COL32(255, 255, 255, Lerp(0, 255, descAlphaMotionTime)), Description.data());

        // GTA IV Style: Simple dark text cover (solid rectangle, no texture)
        drawList->AddRectFilled({ 0.0f, textCoverCornerLeftMin.y }, { res.x, textCoverCornerLeftMax.y }, textCoverColour);
    }
    else
    {
        // Draw blank text cover.
        drawList->AddRectFilled({ 0.0f, textCoverCornerLeftMin.y }, { res.x, textCoverCornerLeftMax.y }, textCoverColour);
    }

    if (ReduceDraw)
    {
        drawList->PopClipRect();
    }
    else
    {
        // GTA IV Style: Solid black footer area - no metal plates
        auto bottomPlateHeight = Scale(145, true);
        auto bottomPlateMotion = Lerp(max.y + bottomPlateHeight, max.y - bottomPlateHeight + Scale(12, true), borderMotionTime);
        drawList->AddRectFilled({ 0.0f, bottomPlateMotion }, { res.x, res.y }, GTA4Style::Colors::Background);
    }

    if (App::s_isInit)
        SetShaderModifier(IMGUI_SHADER_MODIFIER_NONE);

    if (ShowVersionString)
    {
        auto verAlphaMotionTime = PlayTransitions ? ComputeLinearMotion(m_time, 0, m_isClosing ? 3 : 10, m_isClosing) : 1.0;

        DrawVersionString(IM_COL32(0, 0, 0, 70 * verAlphaMotionTime));
    }
}

void CommonMenu::Open()
{
    m_isClosing = false;
    m_time = ImGui::GetTime();
    m_titleTime = m_time;
    m_descTime = m_time;
}

double CommonMenu::Close(bool isAnimated)
{
    if (!m_isClosing)
    {
        m_isClosing = true;
        m_time = ImGui::GetTime();
        m_titleTime = m_time;
        m_descTime = m_time;
    }

    return isAnimated ? ComputeLinearMotion(m_time, 0, ANIMATION_DURATION) : 1.0;
}

bool CommonMenu::IsOpen() const
{
    return !m_isClosing && ComputeLinearMotion(m_time, 0, ANIMATION_DURATION) >= 1.0;
}

void CommonMenu::SetTitle(std::string title, bool isAnimated)
{
    if (Title == title)
        return;

    m_previousTitle = Title;
    Title = title;

    if (!isAnimated)
        return;

    m_titleTime = ImGui::GetTime();
}

void CommonMenu::SetDescription(std::string desc, bool isAnimated)
{
    if (Description == desc)
        return;

    m_previousDesc = Description;
    m_previousDescPos = m_descPos;
    Description = desc;

    resetDescScroll();

    if (!isAnimated)
        return;

    m_descTime = ImGui::GetTime();
}
