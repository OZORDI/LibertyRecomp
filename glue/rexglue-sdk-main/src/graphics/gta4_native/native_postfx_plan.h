#pragma once
#include <algorithm>
#include <array>
#include <cmath>

namespace rex::graphics::gta4_native {
// The title has an explicit zero-amplitude DOF state. Keep stipple filtering,
// but do not gather, blur and blend an image whose contribution is identically
// zero. Invalid/unbounded parameters keep the conservative full path.
inline bool NativeDofCanBeElided(const std::array<float, 4>& projection,
                                 const std::array<float, 4>& distance,
                                 const std::array<float, 4>& blur) {
  const auto finite = [](const auto& a) {
    return std::all_of(a.begin(), a.end(), [](float x) { return std::isfinite(x); });
  };
  return finite(projection) && finite(distance) && finite(blur) && blur[0] == 0.0f &&
         blur[1] == 0.0f && blur[2] == 0.0f;
}
}  // namespace rex::graphics::gta4_native
