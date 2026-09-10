#pragma once
#include "modern_shader_policy.h"
namespace rex::graphics::gta4_native {
// Registry-synchronized reads; call only at a guest-frame boundary.
ModernShaderSettings ReadModernShaderSettings();
bool ModernShaderTraceEnabled();
}  // namespace rex::graphics::gta4_native
