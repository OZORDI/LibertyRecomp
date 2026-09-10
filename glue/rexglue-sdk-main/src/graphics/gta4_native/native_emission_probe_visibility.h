#pragma once
// CPU selection aid for the audited static emissive VS2 geometry only.
// This never culls a game draw and never replaces a GPU coverage result.
#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <span>

namespace rex::graphics::gta4_native {
struct EmissionProbeBounds {
  bool valid = false;
  bool intersects_view = false;
  std::array<float, 3> minimum{};
  std::array<float, 3> maximum{};
};
inline EmissionProbeBounds InspectEmissionProbeBounds(
    std::span<const uint8_t> vertices, size_t offset, size_t stride,
    std::span<const uint8_t> vertex_constants) {
  EmissionProbeBounds result;
  constexpr size_t kMatrixOffset = 8 * 16;
  constexpr size_t kMatrixBytes = 4 * 16;
  if (stride < 12 || offset > vertices.size() || vertices.size() - offset < stride ||
      (vertices.size() - offset) % stride ||
      vertex_constants.size() < kMatrixOffset + kMatrixBytes) return result;
  auto read = [](std::span<const uint8_t> bytes, size_t at) {
    const uint32_t bits = (uint32_t(bytes[at]) << 24) | (uint32_t(bytes[at + 1]) << 16) |
                          (uint32_t(bytes[at + 2]) << 8) | uint32_t(bytes[at + 3]);
    return std::bit_cast<float>(bits);
  };
  std::array<std::array<float, 4>, 4> matrix{};
  for (size_t row = 0; row < 4; ++row)
    for (size_t col = 0; col < 4; ++col) {
      matrix[row][col] = read(vertex_constants, kMatrixOffset + row * 16 + col * 4);
      if (!std::isfinite(matrix[row][col])) return result;
    }
  result.minimum.fill(std::numeric_limits<float>::infinity());
  result.maximum.fill(-std::numeric_limits<float>::infinity());
  bool positive_w = false;
  for (size_t at = offset; vertices.size() - at >= stride; at += stride) {
    const std::array<float, 3> p = {read(vertices, at), read(vertices, at + 4), read(vertices, at + 8)};
    if (!std::isfinite(p[0]) || !std::isfinite(p[1]) || !std::isfinite(p[2])) return {};
    std::array<float, 4> clip{};
    for (size_t col = 0; col < 4; ++col) {
      clip[col] = p[2] * matrix[2][col] + matrix[3][col];
      clip[col] = p[1] * matrix[1][col] + clip[col];
      clip[col] = p[0] * matrix[0][col] + clip[col];
    }
    // Only a conservative selection filter. Near-plane intersections and
    // unknown matrices must not be described as authoritative raster results.
    if (!std::isfinite(clip[3]) || clip[3] <= 0) continue;
    positive_w = true;
    for (size_t col = 0; col < 3; ++col) {
      const float value = clip[col] / clip[3];
      if (!std::isfinite(value)) return {};
      result.minimum[col] = std::min(result.minimum[col], value);
      result.maximum[col] = std::max(result.maximum[col], value);
    }
  }
  result.valid = positive_w;
  result.intersects_view = positive_w && result.maximum[0] > -1 && result.minimum[0] < 1 &&
      result.maximum[1] > -1 && result.minimum[1] < 1 &&
      result.maximum[2] >= 0 && result.minimum[2] <= 1;
  return result;
}
} // namespace rex::graphics::gta4_native
