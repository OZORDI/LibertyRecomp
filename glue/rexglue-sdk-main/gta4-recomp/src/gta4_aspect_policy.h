#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <optional>
#include <string_view>

namespace gta4::aspect {
inline constexpr double kReferenceAspect = 16.0 / 9.0;
struct Ratio {
  uint32_t x = 0, y = 0;
};
struct Extent {
  uint32_t width = 0, height = 0;
  constexpr bool valid() const noexcept { return width && height; }
  constexpr double aspect() const noexcept {
    return valid() ? double(width) / height : kReferenceAspect;
  }
  bool operator==(const Extent&) const = default;
};
struct Preset {
  std::string_view value, label;
  Ratio ratio;
};
inline constexpr std::array kPresets = {
    Preset{"auto", "Display", {0, 0}},  Preset{"16:9", "Original 16:9", {16, 9}},
    Preset{"16:10", "16:10", {16, 10}}, Preset{"3:2", "3:2", {3, 2}},
    Preset{"4:3", "4:3", {4, 3}},       Preset{"5:4", "5:4", {5, 4}},
    Preset{"21:9", "21:9", {21, 9}},    Preset{"43:18", "43:18 (3440 x 1440)", {43, 18}},
    Preset{"32:9", "32:9", {32, 9}},    Preset{"32:10", "32:10", {32, 10}},
};
constexpr std::string_view CanonicalPreset(std::string_view value) noexcept {
  return value == "original" ? "16:9" : value;
}
constexpr std::optional<Ratio> ParseRatio(std::string_view value) noexcept {
  value = CanonicalPreset(value);
  for (const auto& preset : kPresets)
    if (preset.value == value)
      return preset.ratio;
  return std::nullopt;
}
// Cross-products retain the selected shape without floating-point fit decisions.
constexpr Extent Fit(Extent bounds, Ratio ratio) noexcept {
  if (!bounds.valid() || !ratio.x || !ratio.y)
    return bounds;
  if (uint64_t(bounds.width) * ratio.y > uint64_t(bounds.height) * ratio.x)
    return {std::max(1u, uint32_t(uint64_t(bounds.height) * ratio.x / ratio.y)), bounds.height};
  return {bounds.width, std::max(1u, uint32_t(uint64_t(bounds.width) * ratio.y / ratio.x))};
}
constexpr Extent LimitExtent(Extent e, uint32_t maximum) noexcept {
  if (!e.valid() || !maximum)
    return {};
  return e.width <= maximum && e.height <= maximum ? e
                                                   : Fit({maximum, maximum}, {e.width, e.height});
}
constexpr Extent SelectExtent(Extent budget, std::string_view mode, Extent drawable) noexcept {
  const auto ratio = ParseRatio(mode);
  if (!ratio)
    return budget;
  return Fit(budget, ratio->x           ? *ratio
                     : drawable.valid() ? Ratio{drawable.width, drawable.height}
                                        : Ratio{budget.width, budget.height});
}
struct Point {
  double x = 0, y = 0;
};
struct Rect {
  double left = 0, top = 0, right = 0, bottom = 0;
};
struct Transform {
  double sx = 1, sy = 1, ox = 0, oy = 0;
  constexpr bool identity() const noexcept { return sx == 1 && sy == 1 && ox == 0 && oy == 0; }
  constexpr Point Map(Point p) const noexcept { return {p.x * sx + ox, p.y * sy + oy}; }
  constexpr Point Unmap(Point p) const noexcept { return {(p.x - ox) / sx, (p.y - oy) / sy}; }
  constexpr Rect Map(Rect r) const noexcept {
    return {r.left * sx + ox, r.top * sy + oy, r.right * sx + ox, r.bottom * sy + oy};
  }
  constexpr Transform Pixels(Extent e) const noexcept {
    return {sx, sy, ox * e.width, oy * e.height};
  }
};
inline Transform Layout(Extent output, Point anchor = {0.5, 0.5}) noexcept {
  if (!output.valid())
    return {};
  const double aspect = output.aspect();
  if (std::abs(aspect - kReferenceAspect) < 1e-7)
    return {};
  const double sx = std::min(1.0, kReferenceAspect / aspect);
  const double sy = std::min(1.0, aspect / kReferenceAspect);
  return {sx, sy, (1 - sx) * anchor.x, (1 - sy) * anchor.y};
}
// frontend_360.dat: TOP_position_of_top_line. The row origin is authored
// relative to this fixed menu boundary, not relative to the top of the display.
inline constexpr double kDefaultMenuDividerY = 0.192;
inline Transform MenuBodyLayout(Extent output, double divider_y = kDefaultMenuDividerY) noexcept {
  if (!std::isfinite(divider_y) || divider_y <= 0 || divider_y >= 1)
    divider_y = kDefaultMenuDividerY;
  return Layout(output, {0.5, divider_y});
}
inline double ExpandVerticalFov(double authored_degrees, double aspect) noexcept {
  if (!std::isfinite(authored_degrees) || !std::isfinite(aspect) || authored_degrees <= 0 ||
      authored_degrees >= 179 || aspect <= 0)
    return authored_degrees;
  if (aspect >= kReferenceAspect - 1e-7)
    return authored_degrees;
  constexpr double radians = 3.14159265358979323846 / 180.0;
  return 2.0 * std::atan(std::tan(authored_degrees * radians * 0.5) * kReferenceAspect / aspect) /
         radians;
}
// Verified CViewport owners. Its grcViewport member begins at owner + 16.
// Deliberately exclude the generic 3D scene, radar, HTML, reflection and shadow views.
constexpr bool ScreenCameraOwner(uint32_t vtable) noexcept {
  return vtable == 0x820B9284 || vtable == 0x820212F4 || vtable == 0x820BCB40;
}
constexpr bool PhoneCameraOwner(uint32_t vtable) noexcept {
  return vtable == 0x820B95F0;
}
constexpr bool PrimaryUiOwner(uint32_t vtable) noexcept {
  return vtable == 0x820B92C4;
}
inline double EdgeAnchor(double position) noexcept {
  return position < 1.0 / 3.0 ? 0.0 : position > 2.0 / 3.0 ? 1.0 : 0.5;
}
// Selected once from a component's authored origin, never per vertex or per glyph.
inline Point ComponentAnchor(Point position) noexcept {
  return {EdgeAnchor(position.x), EdgeAnchor(position.y)};
}
inline Transform CoveringBackground(Transform t, Rect r) noexcept {
  if (r.left <= 0 && r.right >= 1) {
    t.sx = 1;
    t.ox = 0;
  }
  if (r.top <= 0 && r.bottom >= 1) {
    t.sy = 1;
    t.oy = 0;
  }
  return t;
}
constexpr bool IsLoadingArt(uint32_t caller) noexcept {
  return caller == 0x821440C0;
}
// Post-projection UI mapping for the independently rendered phone. All four rows
// include the homogeneous term; the retail rebuild updates inverse/frustum data next.
inline void TransformProjection(std::array<float, 16>& matrix, Transform t) noexcept {
  const double bx = 2 * t.ox + t.sx - 1;
  const double by = 1 - t.sy - 2 * t.oy;
  for (unsigned row = 0; row < 4; ++row) {
    const auto i = row * 4;
    matrix[i] = float(matrix[i] * t.sx + matrix[i + 3] * bx);
    matrix[i + 1] = float(matrix[i + 1] * t.sy + matrix[i + 3] * by);
  }
}
// Font records own these values, so delayed playback needs no ambient layout flag.
// Kerning at +56 is multiplied by the width later; do not scale it a second time.
inline constexpr std::array<uint32_t, 4> kFontScaledOffsets = {4, 8, 12, 64};
inline double FontScale(uint32_t offset, Transform t) noexcept {
  return offset == 8 ? t.sy : t.sx;
}
// Constructors publish an instance sequence in the high bits. Both the common
// Append helper and the frontend's inline Append subsequently change only the
// command-size bits 0x0003FF80 (generated sub_8229D8A8 / sub_82146790).
constexpr uint32_t StableDcToken(uint32_t token) noexcept {
  return token & 0xFFFC007Fu;
}
struct DcIdentity {
  uint32_t address = 0, token = 0, vtable = 0;
  bool operator==(const DcIdentity&) const = default;
};
constexpr bool HasUiDrawExecutor(uint32_t vtable) noexcept {
  switch (vtable) {
    case 0x820013A8:
    case 0x820013C4:
    case 0x820013E0:
    case 0x820013FC:
    case 0x82001434:
    case 0x82001450:
    case 0x8200146C:
    case 0x820014A4:
    case 0x820014C0:
    case 0x820014DC:
      return true;
    default:
      return false;
  }
}
}  // namespace gta4::aspect
