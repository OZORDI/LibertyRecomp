#include "modern_shader_options.h"
#include <rex/cvar.h>

REXCVAR_DEFINE_BOOL(gta4_modern_shaders, false, "GTA IV/Graphics/Post-Processing",
                    "Use modern visual shader replacements; applies on the next complete frame")
    .lifecycle(rex::cvar::Lifecycle::kHotReload);
REXCVAR_DEFINE_BOOL(gta4_trace_modern_shaders, false, "GTA IV/Diagnostics",
                    "Bounded frame-boundary and bound-pipeline evidence for Modern shaders");

namespace rex::graphics::gta4_native {
ModernShaderSettings ReadModernShaderSettings() {
  return {rex::cvar::Query<bool>("gta4_modern_shaders"),
          rex::cvar::Query<bool>("gta4_disable_tlad_film_grain")};
}
bool ModernShaderTraceEnabled() {
  return rex::cvar::Query<bool>("gta4_trace_modern_shaders");
}
}  // namespace rex::graphics::gta4_native
