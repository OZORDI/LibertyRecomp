#include "input/context_touch_draw.h"

#include <algorithm>
#include <cfloat>
#include <cmath>

namespace gta4::input {

void DrawContextTouchOverlay(ImDrawList* draw, ImFont* font, float font_size,
                             const ContextTouchOverlaySnapshot& snapshot,
                             float logical_width, float logical_height,
                             ContextTouchIconResolver resolver, void* icon_context) {
  if (!draw || !font || !snapshot.visible || !std::isfinite(font_size) || font_size <= 0.0f ||
      snapshot.layout.mode == ContextTouchMode::kFrontend ||
      snapshot.layout.mode == ContextTouchMode::kMap) return;
  const auto transform = BuildContextTouchOverlayTransform(snapshot.layout.viewport,
                                                          logical_width, logical_height);
  if (!transform.valid) return;
  const float radius_scale = std::min(transform.scale_x, transform.scale_y);
  const float opacity = std::clamp(snapshot.layout.opacity, 0.2f, 1.0f) *
                        std::clamp(snapshot.fade_alpha, 0.0f, 1.0f);
  if (!std::isfinite(opacity) || opacity <= 0.0f) return;
  const auto color = [opacity](int r, int g, int b, float alpha) {
    return IM_COL32(r, g, b, static_cast<int>(std::clamp(alpha * opacity, 0.0f, 255.0f)));
  };
  const auto point = [&](float x, float y) {
    return ImVec2(transform.offset_x + x * transform.scale_x,
                  transform.offset_y + y * transform.scale_y);
  };
  for (size_t i = 0; i < snapshot.layout.control_count; ++i) {
    const auto& control = snapshot.layout.controls[i];
    if (!control.visible || control.kind == ContextTouchControlKind::kLookSurface) continue;
    const ImVec2 center = control.kind == ContextTouchControlKind::kMovementStick && snapshot.movement_owned
        ? point(snapshot.movement_origin_x, snapshot.movement_origin_y)
        : point(control.center_x, control.center_y);
    const float radius = control.radius * radius_scale;
    if (!std::isfinite(radius) || radius <= 0.0f) continue;
    const bool active = snapshot.active[i] != 0;
    draw->AddCircleFilled(center, radius, active ? color(236, 135, 42, 255.0f)
                                               : color(12, 18, 27, 210.0f), 48);
    draw->AddCircle(center, radius, color(255, 255, 255, 255.0f), 48,
                    std::max(1.0f, radius * 0.025f));
    if (control.kind == ContextTouchControlKind::kMovementStick || control.kind == ContextTouchControlKind::kRightStick) {
      const bool right = control.kind == ContextTouchControlKind::kRightStick;
      const float x = std::clamp(right ? snapshot.right_x : snapshot.movement_x, -255.0f, 255.0f) / 255.0f;
      const float y = std::clamp(right ? snapshot.right_y : snapshot.movement_y, -255.0f, 255.0f) / 255.0f;
      draw->AddCircleFilled(ImVec2(center.x + x * radius * 0.55f,
                                  center.y + y * radius * 0.55f),
                            radius * 0.34f, color(255, 255, 255, 210.0f), 32);
    }
    ContextTouchIcon icon;
    bool has_icon = false;
    if (resolver && control.icon_id[0]) {
      has_icon = resolver(icon_context, control.icon_id.data(), &icon) && icon.texture &&
                 std::isfinite(icon.aspect) && icon.aspect > 0.0f;
    }
    if (has_icon) {
      const float bound = radius * 0.62f;
      const float half_width = icon.aspect >= 1.0f ? bound : bound * icon.aspect;
      const float half_height = icon.aspect >= 1.0f ? bound / icon.aspect : bound;
      draw->AddImage(icon.texture, ImVec2(center.x - half_width, center.y - half_height),
                     ImVec2(center.x + half_width, center.y + half_height), icon.uv_min,
                     icon.uv_max, color(255, 255, 255, 255.0f));
    } else if (control.label[0]) {
      const ImVec2 natural = font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, control.label.data());
      const float fitted_size = natural.x > 0.0f
          ? std::min(font_size, font_size * radius * 1.55f / natural.x) : font_size;
      const ImVec2 size = font->CalcTextSizeA(fitted_size, FLT_MAX, 0.0f, control.label.data());
      draw->AddText(font, fitted_size, ImVec2(center.x - size.x * 0.5f, center.y - size.y * 0.5f),
                     color(255, 255, 255, 255.0f), control.label.data());
    }
  }
}

}  // namespace gta4::input
