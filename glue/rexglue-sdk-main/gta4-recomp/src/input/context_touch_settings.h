#pragma once

#include <cstdint>
#include <filesystem>

#include "input/context_touch_layout.h"

namespace gta4::input {

struct ContextTouchPreferences {
  float button_scale = 1.0f;
  float opacity = 0.65f;
  float camera_sensitivity = 1.0f;
  float aim_sensitivity = 0.35f;
  float vehicle_sensitivity = 1.0f;
  float flight_sensitivity = 1.0f;
  bool left_handed = false;
  bool floating_stick = true;
  bool invert_camera_y = false;
  bool haptics = true;
};

// Preferences and placements are copied across the input/render boundary.
void ConfigureContextTouchSettings(const std::filesystem::path& path) noexcept;
ContextTouchPreferences GetContextTouchPreferences() noexcept;
void SetContextTouchPreferences(ContextTouchPreferences preferences) noexcept;
bool FlushContextTouchSettings() noexcept;
bool ContextTouchEditorOpen() noexcept;
void SetContextTouchEditorOpen(bool open) noexcept;
void ApplyContextTouchSavedLayout(ContextTouchLayout& layout) noexcept;
bool SetContextTouchPlacement(const ContextTouchLayout& layout, TouchAction action,
                             float x, float y, float radius) noexcept;
void RestoreContextTouchLayout(const ContextTouchLayout& layout) noexcept;
void HandleContextTouchEditorAction(TouchAction action, const ContextTouchLayout& layout) noexcept;

}  // namespace gta4::input
