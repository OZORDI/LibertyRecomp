#pragma once
#include "gta4_aspect_policy.h"
namespace gta4::aspect::resolution {
constexpr Extent Limit(Extent bounds, uint32_t maximum) noexcept {
  return LimitExtent(bounds, maximum);
}
constexpr Extent Select(Extent bounds, std::string_view mode, Extent drawable) noexcept {
  return SelectExtent(bounds, mode, drawable);
}
}  // namespace gta4::aspect::resolution
