#pragma once

#include <imgui.h>
#include <cstdint>

namespace GTA4Style
{
    namespace Colors
    {
        // Primary colors
        constexpr ImU32 Orange = IM_COL32(255, 150, 0, 255);
        constexpr ImU32 Background = IM_COL32(10, 10, 10, 255);
        constexpr ImU32 BackgroundDark = IM_COL32(5, 5, 5, 255);
        constexpr ImU32 BackgroundPanel = IM_COL32(20, 20, 20, 230);
        constexpr ImU32 Border = IM_COL32(60, 60, 60, 255);
        
        // Progress bar colors
        constexpr ImU32 ProgressBg = IM_COL32(40, 40, 40, 255);
        constexpr ImU32 ProgressFill = IM_COL32(255, 150, 0, 255);
        constexpr ImU32 ProgressFillDark = IM_COL32(200, 100, 0, 255);
        
        // Text colors
        constexpr ImU32 TextOrange = IM_COL32(255, 150, 0, 255);
        constexpr ImU32 TextGreen = IM_COL32(100, 200, 100, 255);
        constexpr ImU32 TextGray = IM_COL32(150, 150, 150, 255);
    }

    inline ImU32 WithAlpha(ImU32 color, float alpha)
    {
        uint8_t r = (color >> IM_COL32_R_SHIFT) & 0xFF;
        uint8_t g = (color >> IM_COL32_G_SHIFT) & 0xFF;
        uint8_t b = (color >> IM_COL32_B_SHIFT) & 0xFF;
        uint8_t a = static_cast<uint8_t>(((color >> IM_COL32_A_SHIFT) & 0xFF) * alpha);
        return IM_COL32(r, g, b, a);
    }
}
