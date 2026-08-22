#include "gta4_fps_vehicle_camera_fix.h"

#include <bit>
#include <cstdint>

#include <kernel/function.h>

namespace gta4::fps::vehicle_camera
{
namespace
{
// CTimer's gameplay timestep in seconds.
constexpr std::uint32_t kGameplayTimeStepAddress = 0x82C6C2AC;

// Exact binary32 result of 1.0f / 30.0f, calculated with Python.
constexpr float kReferenceFrameSeconds = 0x1.111112p-5f;

float LoadGuestFloat(std::uint8_t* base, std::uint32_t address) noexcept
{
    return std::bit_cast<float>(PPC_LOAD_U32(address));
}
}

float ScalePerFrameForce(float force, float timeStepSeconds) noexcept
{
    return force * (timeStepSeconds / kReferenceFrameSeconds);
}
}

PPC_FUNC_IMPL(__imp__sub_825381C0);

// CCamFollowVehicle handbrake swing. Its sole retail caller adds this return
// value to the yaw force once per rendered frame, so normalize that per-frame
// contribution to the original 30 Hz reference rate.
PPC_FUNC_HOOK(sub_825381C0)
{
    __imp__sub_825381C0(ctx, base);

    const float timeStep =
        gta4::fps::vehicle_camera::LoadGuestFloat(
            base,
            gta4::fps::vehicle_camera::kGameplayTimeStepAddress);
    ctx.f1.f64 = static_cast<double>(
        gta4::fps::vehicle_camera::ScalePerFrameForce(
            static_cast<float>(ctx.f1.f64), timeStep));
}
