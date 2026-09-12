#pragma once
#include <string_view>
#include "native_performance_samples.h"

namespace rex::graphics::gta4_native::performance {
// Diagnostic fallback only. It never changes render phases, pipelines,
// culling or shader selection. The label explicitly describes shader families
// because the title may submit them outside a tagged render-phase scope.
inline GpuRange ProfileShaderCategory(std::string_view vs, std::string_view ps) {
  const auto has = [](std::string_view name, std::string_view family) {
    return name.find(family) != std::string_view::npos;
  };
  if (has(vs,"/shadowzdir/")) return GpuRange::kDirectionalShadowShaders;
  if (has(vs,"/shadowz/") || has(vs,"/shadowzspot/")) return GpuRange::kLocalShadowShaders;
  if (has(ps,"/rage_postfx") || has(ps,"/gta_postfx")) return GpuRange::kComposite;
  if (has(ps,"/sky") || has(ps,"/clouds")) return GpuRange::kSkyShaders;
  if (has(vs,"/gta_im/") || has(ps,"/gta_im/")) return GpuRange::kImmediateShaders;
  if (has(ps,"/gta_particle") || has(ps,"/particle")) return GpuRange::kParticleShaders;
  if (has(vs,"/gta_default/") || has(vs,"/gta_normal") || has(vs,"/gta_spec/") ||
      has(vs,"/gta_vehicle") || has(vs,"/gta_ped/") || has(vs,"/gta_emissive"))
    return GpuRange::kMaterialShaders;
  return GpuRange::kUnattributed;
}
}  // namespace rex::graphics::gta4_native::performance
