#pragma once

namespace gta4::fps::drunk_steering
{
[[nodiscard]] float ScaleBiasForTimeStep(float bias,
                                         float timeStepSeconds) noexcept;
}
