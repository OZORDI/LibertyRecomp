#include "input/context_touch_settings.h"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <vector>

namespace gta4::input {
namespace {

struct Placement {
  ContextTouchMode mode{};
  TouchAction action{};
  bool portrait = false;
  bool left_handed = false;
  bool large_screen = false;
  float x = 0.0f;
  float y = 0.0f;
  float radius = 0.0f;
};

std::mutex g_mutex;
ContextTouchPreferences g_preferences;
std::vector<Placement> g_placements;
std::filesystem::path g_path;
std::atomic<bool> g_editor{false};
bool g_dirty = false;

float Bounded(float value, float low, float high, float fallback) {
  return std::isfinite(value) ? std::clamp(value, low, high) : fallback;
}

ContextTouchPreferences Sanitize(ContextTouchPreferences p) {
  p.button_scale = Bounded(p.button_scale, 0.65f, 1.8f, 1.0f);
  p.opacity = Bounded(p.opacity, 0.2f, 1.0f, 0.65f);
  p.camera_sensitivity = Bounded(p.camera_sensitivity, 0.1f, 3.0f, 1.0f);
  p.aim_sensitivity = Bounded(p.aim_sensitivity, 0.05f, 1.5f, 0.35f);
  p.vehicle_sensitivity = Bounded(p.vehicle_sensitivity, 0.1f, 3.0f, 1.0f);
  p.flight_sensitivity = Bounded(p.flight_sensitivity, 0.1f, 3.0f, 1.0f);
  return p;
}

bool SameProfile(const Placement& p, const ContextTouchLayout& layout) {
  const bool portrait = layout.viewport.safe_height > layout.viewport.safe_width;
  const bool large = std::min(layout.viewport.safe_width, layout.viewport.safe_height) >= 600.0f;
  return p.mode == layout.mode && p.portrait == portrait && p.large_screen == large &&
         p.left_handed == g_preferences.left_handed;
}

bool ValidPlacement(const Placement& p) {
  return p.mode > ContextTouchMode::kDisabled && p.mode <= ContextTouchMode::kMap &&
         p.action > TouchAction::kNone && p.action < TouchAction::kCount &&
         p.action != TouchAction::kScript && std::isfinite(p.x) && std::isfinite(p.y) &&
         std::isfinite(p.radius) && p.x >= 0.0f && p.x <= 1.0f && p.y >= 0.0f && p.y <= 1.0f &&
         p.radius > 0.0f && p.radius <= 0.3f;
}

void Place(ContextTouchControl& c, const ContextTouchViewport& v, const Placement& p) {
  const float edge = std::min(v.safe_width, v.safe_height);
  const float maximum = edge * 0.25f;
  const float minimum = std::min(maximum, v.host_space ? 24.0f : edge * 0.04f);
  c.radius = std::clamp(p.radius * edge, minimum, maximum);
  c.center_x = std::clamp(v.safe_x + p.x * v.safe_width,
                         v.safe_x + c.radius, v.safe_x + v.safe_width - c.radius);
  c.center_y = std::clamp(v.safe_y + p.y * v.safe_height,
                         v.safe_y + c.radius, v.safe_y + v.safe_height - c.radius);
  c.minimum_x = c.center_x - c.radius;
  c.maximum_x = c.center_x + c.radius;
  c.minimum_y = c.center_y - c.radius;
  c.maximum_y = c.center_y + c.radius;
}

bool Overlaps(const ContextTouchControl& a, const ContextTouchControl& b) {
  if (!a.visible || !b.visible || a.kind == ContextTouchControlKind::kLookSurface ||
      b.kind == ContextTouchControlKind::kLookSurface) return false;
  const float dx = a.center_x - b.center_x;
  const float dy = a.center_y - b.center_y;
  const float sum = a.radius + b.radius;
  return dx * dx + dy * dy < sum * sum;
}

}  // namespace

void ConfigureContextTouchSettings(const std::filesystem::path& path) noexcept {
  try {
    std::lock_guard lock(g_mutex);
    if (g_path == path) return;
    g_path = path;
    g_preferences = {};
    g_placements.clear();
    g_dirty = false;
    std::ifstream file(path);
    std::string magic;
    unsigned version = 0;
    if (!(file >> magic >> version) || magic != "liberty-touch" || version != 1) return;
    std::string line;
    size_t lines = 0;
    while (std::getline(file, line) && ++lines <= 1024) {
      std::istringstream row(line);
      std::string kind;
      row >> kind;
      if (kind == "preferences") {
        ContextTouchPreferences p;
        if (row >> p.button_scale >> p.opacity >> p.camera_sensitivity >> p.aim_sensitivity >>
            p.vehicle_sensitivity >> p.flight_sensitivity >> p.left_handed >> p.floating_stick >>
            p.invert_camera_y >> p.haptics) g_preferences = Sanitize(p);
      } else if (kind == "placement") {
        Placement p;
        unsigned mode = 0, action = 0;
        if (!(row >> mode >> action >> p.portrait >> p.left_handed >> p.large_screen >>
              p.x >> p.y >> p.radius)) continue;
        if (mode > static_cast<unsigned>(ContextTouchMode::kMap) ||
            action >= static_cast<unsigned>(TouchAction::kCount)) continue;
        p.mode = static_cast<ContextTouchMode>(mode);
        p.action = static_cast<TouchAction>(action);
        if (ValidPlacement(p)) g_placements.push_back(p);
      }
    }
  } catch (...) {
    // An unreadable user configuration leaves the complete built-in layout available.
  }
}

ContextTouchPreferences GetContextTouchPreferences() noexcept {
  std::lock_guard lock(g_mutex);
  return g_preferences;
}

void SetContextTouchPreferences(ContextTouchPreferences p) noexcept {
  std::lock_guard lock(g_mutex);
  g_preferences = Sanitize(p);
  g_dirty = true;
}

bool FlushContextTouchSettings() noexcept {
  try {
    std::lock_guard lock(g_mutex);
    if (!g_dirty) return true;
    if (g_path.empty()) return false;
    std::error_code error;
    if (!g_path.parent_path().empty()) std::filesystem::create_directories(g_path.parent_path(), error);
    if (error) return false;
    auto temporary = g_path;
    temporary += ".tmp";
    std::ofstream file(temporary, std::ios::trunc);
    if (!file) return false;
    const auto& p = g_preferences;
    file << std::setprecision(9) << "liberty-touch 1\npreferences " << p.button_scale << ' '
         << p.opacity << ' ' << p.camera_sensitivity << ' ' << p.aim_sensitivity << ' '
         << p.vehicle_sensitivity << ' ' << p.flight_sensitivity << ' ' << p.left_handed << ' '
         << p.floating_stick << ' ' << p.invert_camera_y << ' ' << p.haptics << '\n';
    for (const auto& item : g_placements) {
      file << "placement " << static_cast<unsigned>(item.mode) << ' '
           << static_cast<unsigned>(item.action) << ' ' << item.portrait << ' '
           << item.left_handed << ' ' << item.large_screen << ' ' << item.x << ' ' << item.y
           << ' ' << item.radius << '\n';
    }
    file.close();
    if (!file) return false;
    std::filesystem::rename(temporary, g_path, error);
    if (error) {
      // Windows does not replace an existing destination with filesystem::rename.
      error.clear();
      std::filesystem::copy_file(temporary, g_path, std::filesystem::copy_options::overwrite_existing, error);
      if (error) return false;
      std::filesystem::remove(temporary, error);
    }
    g_dirty = false;
    return true;
  } catch (...) {
    return false;
  }
}

bool ContextTouchEditorOpen() noexcept { return g_editor.load(std::memory_order_acquire); }
void SetContextTouchEditorOpen(bool open) noexcept { g_editor.store(open, std::memory_order_release); }

void ApplyContextTouchSavedLayout(ContextTouchLayout& layout) noexcept {
  std::lock_guard lock(g_mutex);
  if (layout.viewport.safe_width <= 0.0f || layout.viewport.safe_height <= 0.0f) return;
  for (size_t index = 0; index < layout.control_count; ++index) {
    auto& control = layout.controls[index];
    if (control.kind == ContextTouchControlKind::kLookSurface ||
        control.kind == ContextTouchControlKind::kUtility) continue;
    for (const auto& placement : g_placements) {
      if (placement.action != control.action || !SameProfile(placement, layout)) continue;
      auto candidate = control;
      Place(candidate, layout.viewport, placement);
      bool overlaps = false;
      for (size_t other = 0; other < layout.control_count; ++other) {
        if (index != other && Overlaps(candidate, layout.controls[other])) overlaps = true;
      }
      if (!overlaps) control = candidate;
      break;
    }
  }
}

bool SetContextTouchPlacement(const ContextTouchLayout& layout, TouchAction action,
                             float x, float y, float radius) noexcept {
  try {
    const auto& v = layout.viewport;
    if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(radius) ||
        v.safe_width <= 0.0f || v.safe_height <= 0.0f) return false;
    std::lock_guard lock(g_mutex);
    Placement p{.mode = layout.mode, .action = action,
                .portrait = v.safe_height > v.safe_width,
                .left_handed = g_preferences.left_handed,
                .large_screen = std::min(v.safe_width, v.safe_height) >= 600.0f,
                .x = (x - v.safe_x) / v.safe_width, .y = (y - v.safe_y) / v.safe_height,
                .radius = radius / std::min(v.safe_width, v.safe_height)};
    if (!ValidPlacement(p)) return false;
    const ContextTouchControl* original = nullptr;
    for (size_t i = 0; i < layout.control_count; ++i) {
      if (layout.controls[i].action == action) original = &layout.controls[i];
    }
    if (!original || original->kind == ContextTouchControlKind::kLookSurface ||
        original->kind == ContextTouchControlKind::kUtility) return false;
    auto candidate = *original;
    Place(candidate, v, p);
    for (size_t i = 0; i < layout.control_count; ++i) {
      if (&layout.controls[i] != original && Overlaps(candidate, layout.controls[i])) return false;
    }
    for (auto& existing : g_placements) {
      if (existing.action == action && SameProfile(existing, layout)) {
        existing = p;
        g_dirty = true;
        return true;
      }
    }
    if (g_placements.size() >= 1024) return false;
    g_placements.push_back(p);
    g_dirty = true;
    return true;
  } catch (...) {
    return false;
  }
}

void RestoreContextTouchLayout(const ContextTouchLayout& layout) noexcept {
  std::lock_guard lock(g_mutex);
  std::erase_if(g_placements, [&](const Placement& p) { return SameProfile(p, layout); });
  g_dirty = true;
}

void HandleContextTouchEditorAction(TouchAction action, const ContextTouchLayout& layout) noexcept {
  if (action == TouchAction::kSettings) { SetContextTouchEditorOpen(true); return; }
  if (action == TouchAction::kEditDone) { SetContextTouchEditorOpen(false); return; }
  if (action == TouchAction::kEditReset) { RestoreContextTouchLayout(layout); return; }
  auto p = GetContextTouchPreferences();
  switch (action) {
    case TouchAction::kEditSmaller: p.button_scale -= 0.1f; break;
    case TouchAction::kEditLarger: p.button_scale += 0.1f; break;
    case TouchAction::kEditOpacity: p.opacity = p.opacity >= 0.95f ? 0.3f : p.opacity + 0.15f; break;
    case TouchAction::kEditHandedness: p.left_handed = !p.left_handed; break;
    case TouchAction::kEditFloating: p.floating_stick = !p.floating_stick; break;
    case TouchAction::kEditCameraSpeed:
      p.camera_sensitivity = p.camera_sensitivity >= 2.0f ? 0.5f : p.camera_sensitivity + 0.25f;
      break;
    case TouchAction::kEditAimSpeed: p.aim_sensitivity = p.aim_sensitivity >= 1.0f ? 0.15f : p.aim_sensitivity + 0.1f; break;
    case TouchAction::kEditVehicleSpeed:
      p.vehicle_sensitivity = p.vehicle_sensitivity >= 2.0f ? 0.5f : p.vehicle_sensitivity + 0.25f;
      break;
    case TouchAction::kEditFlightSpeed:
      p.flight_sensitivity = p.flight_sensitivity >= 2.0f ? 0.5f : p.flight_sensitivity + 0.25f;
      break;
    case TouchAction::kEditInvertY: p.invert_camera_y = !p.invert_camera_y; break;
    default: return;
  }
  SetContextTouchPreferences(p);
}

}  // namespace gta4::input
