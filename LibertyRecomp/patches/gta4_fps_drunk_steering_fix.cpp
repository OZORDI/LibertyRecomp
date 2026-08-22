#include "gta4_fps_drunk_steering_fix.h"

#include <bit>
#include <cstddef>
#include <cstdint>

#include <kernel/function.h>

namespace gta4::fps::drunk_steering
{
// CTimer::fTimeStep. Retail CAutomobile::ProcessControlInputs and
// CBike::ProcessControlInputs both read this gameplay timestep.
constexpr std::uint32_t kGameplayTimeStepAddress = 0x82C6C2AC;

// SET_VEHICLE_STEER_BIAS stores its argument here. Both vehicle-control
// functions read the same field for their otherwise per-frame bias addition.
constexpr std::uint32_t kSteeringBiasOffset = 0x1074;

// Exact binary32 result of 1.0f / 30.0f, calculated with Python.
constexpr float kOriginalFrameSeconds = 0x1.111112p-5f;

float ScaleBiasForTimeStep(float bias, float timeStepSeconds) noexcept
{
    return bias * (timeStepSeconds / kOriginalFrameSeconds);
}

namespace
{
constexpr std::uint32_t kAutomobileProcessControlInputs = 0x82643870;
constexpr std::uint32_t kBikeProcessControlInputs = 0x82647DD0;

PPCFunc* gAutomobileProcessControlInputs = nullptr;
PPCFunc* gBikeProcessControlInputs = nullptr;

class ScopedScaledSteeringBias
{
public:
    ScopedScaledSteeringBias(std::uint32_t vehicle, float timeStepSeconds) noexcept
        : address_(vehicle + kSteeringBiasOffset),
          originalBits_(PPC_LOAD_U32(address_))
    {
        const float originalBias = std::bit_cast<float>(originalBits_);
        PPC_STORE_U32(address_, std::bit_cast<std::uint32_t>(
            ScaleBiasForTimeStep(originalBias, timeStepSeconds)));
    }

    ~ScopedScaledSteeringBias()
    {
        PPC_STORE_U32(address_, originalBits_);
    }

    ScopedScaledSteeringBias(const ScopedScaledSteeringBias&) = delete;
    ScopedScaledSteeringBias& operator=(const ScopedScaledSteeringBias&) = delete;

private:
    std::uint32_t address_;
    std::uint32_t originalBits_;
};

void InvokeWithScaledSteeringBias(PPCContext& ctx, std::uint8_t* base,
                                  PPCFunc* original)
{
    const float timeStepSeconds = std::bit_cast<float>(
        PPC_LOAD_U32(kGameplayTimeStepAddress));
    const ScopedScaledSteeringBias scaledBias(ctx.r3.u32, timeStepSeconds);
    original(ctx, base);
}

void AutomobileProcessControlInputs(PPCContext& ctx, std::uint8_t* base)
{
    InvokeWithScaledSteeringBias(
        ctx, base, gAutomobileProcessControlInputs);
}

void BikeProcessControlInputs(PPCContext& ctx, std::uint8_t* base)
{
    InvokeWithScaledSteeringBias(ctx, base, gBikeProcessControlInputs);
}

void ReplaceMapping(std::uint32_t guestAddress, PPCFunc* replacement,
                    PPCFunc*& original) noexcept
{
    for (std::size_t index = 0; PPCFuncMappings[index].guest != 0; ++index)
    {
        if (PPCFuncMappings[index].guest != guestAddress)
            continue;

        original = PPCFuncMappings[index].host;
        PPCFuncMappings[index].host = replacement;
        return;
    }
}

// These two guest functions already have strong motion-input wrappers in
// gta4_motion_vehicle_hooks.cpp. Replacing their mapping entries chains this
// correction around those wrappers without defining duplicate hook symbols;
// the bias is restored after the original/motion chain returns.
struct MappingInstaller
{
    MappingInstaller() noexcept
    {
        ReplaceMapping(kAutomobileProcessControlInputs,
                       AutomobileProcessControlInputs,
                       gAutomobileProcessControlInputs);
        ReplaceMapping(kBikeProcessControlInputs,
                       BikeProcessControlInputs,
                       gBikeProcessControlInputs);
    }
};

[[maybe_unused]] MappingInstaller gMappingInstaller;
}
}
