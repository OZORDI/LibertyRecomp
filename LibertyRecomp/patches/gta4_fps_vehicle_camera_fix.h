#pragma once

namespace gta4::fps::vehicle_camera
{
[[nodiscard]] float ScalePerFrameForce(float force,
                                       float timeStepSeconds) noexcept;
}
