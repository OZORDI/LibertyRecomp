#pragma once

namespace gta4::fps::water_force
{
[[nodiscard]] float ScalePerFrameAmount(float amount,
                                        float timeStepSeconds) noexcept;
}
