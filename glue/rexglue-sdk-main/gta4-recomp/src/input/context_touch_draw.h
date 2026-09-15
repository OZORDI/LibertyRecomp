#pragma once

#include <string_view>
#include <imgui.h>

#include "input/context_touch_controls.h"

namespace gta4::input {

struct ContextTouchIcon {
  ImTextureID texture{};
  ImVec2 uv_min{0.0f, 0.0f};
  ImVec2 uv_max{1.0f, 1.0f};
  float aspect = 1.0f;
};

using ContextTouchIconResolver = bool (*)(void* context, std::string_view id,
                                        ContextTouchIcon* icon);

// No global ImGui state, frame ownership, guest memory, or input access.
void DrawContextTouchOverlay(ImDrawList* draw, ImFont* font, float font_size,
                             const ContextTouchOverlaySnapshot& snapshot,
                             float logical_width, float logical_height,
                             ContextTouchIconResolver resolver = nullptr,
                             void* icon_context = nullptr);

}  // namespace gta4::input
