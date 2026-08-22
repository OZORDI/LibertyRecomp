#include "gta4_fps_water_force_fix.h"

#include <bit>
#include <cstdint>

#include <kernel/function.h>

namespace gta4::fps::water_force
{
// CTimer::fTimeStep, the gameplay timestep used by FusionFix's water-force
// correction. Retail sub_8227FE40 updates this separately from fCamTimeStep.
constexpr std::uint32_t kGameplayTimeStepAddress = 0x82C6C2AC;

// Exact binary32 result of 1.0f / 30.0f, calculated with Python.
constexpr float kOriginalFrameSeconds = 0x1.111112p-5f;

float ScalePerFrameAmount(float amount, float timeStepSeconds) noexcept
{
    return amount * (timeStepSeconds / kOriginalFrameSeconds);
}

float LoadGameplayTimeStep(std::uint8_t* base) noexcept
{
    return std::bit_cast<float>(PPC_LOAD_U32(kGameplayTimeStepAddress));
}
}

PPC_FUNC_IMPL(__imp__sub_82290FA0);
PPC_FUNC_IMPL(__imp__sub_82291048);

// CWater::AddToDynamicWaterSpeed. Retail PPC uses r3/r4 as dynamic-water
// grid coordinates and adds f1 to that cell. Its only caller is the
// rotor/angle/distance helicopter-downwash routine sub_82547BC0.
PPC_FUNC_HOOK(sub_82290FA0)
{
    ctx.f1.f64 = static_cast<double>(
        gta4::fps::water_force::ScalePerFrameAmount(
            static_cast<float>(ctx.f1.f64),
            gta4::fps::water_force::LoadGameplayTimeStep(base)));
    __imp__sub_82290FA0(ctx, base);
}

// CWater::ModifyDynamicWaterSpeed. Retail PPC uses r3/r4 as grid
// coordinates, f1 as the target speed, and f2 as the blend amount in
// target*f2 + current*(1-f2). Its only caller is the common buoyancy routine
// sub_826811D0, so only the per-frame blend amount is timestep-scaled.
PPC_FUNC_HOOK(sub_82291048)
{
    ctx.f2.f64 = static_cast<double>(
        gta4::fps::water_force::ScalePerFrameAmount(
            static_cast<float>(ctx.f2.f64),
            gta4::fps::water_force::LoadGameplayTimeStep(base)));
    __imp__sub_82291048(ctx, base);
}
