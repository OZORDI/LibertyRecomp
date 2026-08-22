#pragma once

#include <cstdint>

namespace gta4::fps::water_particles
{
// Retail CBuoyancy::ProcessSplashVfx selects a ripple divisor from the
// object's horizontal speed before applying its frame-counter modulus gate.
[[nodiscard]] std::uint32_t ComputeSplashRippleDivisor(
    float horizontalSpeed) noexcept;

// FusionFix scales a per-frame particle divisor by the gameplay timestep,
// capped at the retail 30 Hz timestep.
[[nodiscard]] std::uint32_t ScaleParticleDivisor(
    std::uint32_t divisor, float timeStepSeconds) noexcept;
}
