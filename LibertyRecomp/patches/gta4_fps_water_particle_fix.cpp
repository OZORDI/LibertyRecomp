#include "gta4_fps_water_particle_fix.h"

#include <bit>
#include <cstdint>
#include <limits>

#include <cpu/ppc_context.h>
#include <kernel/function.h>

namespace gta4::fps::water_particles
{
namespace
{
// CTimer::fTimeStep and CTimer::m_FrameCounter. Retail sub_8227FE40 updates
// the gameplay timestep separately from the camera timestep.
constexpr std::uint32_t kGameplayTimeStepAddress = 0x82C6C2AC;
constexpr std::uint32_t kRawFrameCounterAddress = 0x82C6C2B8;

// Exact binary32 result of 1.0f / 30.0f, calculated with Python.
constexpr float kReferenceFrameSeconds = 0x1.111112p-5f;

// Retail data used by the sole modulus in CBuoyancy::ProcessSplashVfx.
// PPC at 0x8267F6D4..0x8267F740 selects 10 at <= 1.5 speed, 5 at
// >= 2.5 speed, and truncates a linear interpolation between them.
constexpr float kLowRippleSpeed = 1.5f;
constexpr float kRippleSpeedSpan = 1.0f;
constexpr float kHighRippleSpeed = 2.5f;
constexpr std::int32_t kLowSpeedDivisor = 10;
constexpr std::int32_t kHighSpeedDivisor = 5;

// Return immediately after the wading/splash helper on the only path that
// reaches the retail divisor calculation and modulus at 0x8267F740.
constexpr std::uint32_t kSplashModuloPathReturnAddress = 0x8267F6B8;

// Return immediately after the generic ripple emitter selected by that gate.
constexpr std::uint32_t kSplashRippleEmitterReturnAddress = 0x8267F7A8;

struct SplashFrameOverride
{
    bool active = false;
    bool frameAdjusted = false;
    std::uint32_t originalFrame = 0;
};

thread_local SplashFrameOverride gSplashFrameOverride;

float LoadGameplayTimeStep() noexcept
{
    return std::bit_cast<float>(PPC_LOAD_U32(kGameplayTimeStepAddress));
}

void RestoreRawFrameCounter() noexcept
{
    if (!gSplashFrameOverride.active ||
        !gSplashFrameOverride.frameAdjusted)
    {
        return;
    }

    PPC_STORE_U32(kRawFrameCounterAddress,
                  gSplashFrameOverride.originalFrame);
    gSplashFrameOverride.frameAdjusted = false;
}

void CorrectSplashRippleGate(PPCContext& ctx) noexcept
{
    const float timeStepSeconds = LoadGameplayTimeStep();
    if (!(timeStepSeconds > 0.0f) ||
        timeStepSeconds >= kReferenceFrameSeconds)
    {
        return;
    }

    // f29 is the horizontal entity speed retained across sub_8234AB70;
    // r31 is the physical entity and r29 is the matched water-FX table row.
    const std::uint32_t retailDivisor = ComputeSplashRippleDivisor(
        static_cast<float>(ctx.f29.f64));
    const std::uint32_t correctedDivisor = ScaleParticleDivisor(
        retailDivisor, timeStepSeconds);

    const std::uint32_t frame = gSplashFrameOverride.originalFrame;
    const std::uint32_t entityIndex = PPC_LOAD_U16(ctx.r31.u32 + 0x2C);
    const std::uint32_t dividend = frame + entityIndex + ctx.r29.u32;
    const std::uint32_t retailRemainder = dividend % retailDivisor;
    const bool correctedGatePasses =
        (dividend % correctedDivisor) == 0;

    std::uint32_t adjustedFrame = frame;
    if (correctedGatePasses)
    {
        // Make the unchanged retail modulus pass. This also supplies corrected
        // spawns whose scaled divisor is not a multiple of the retail divisor.
        adjustedFrame -= retailRemainder;
    }
    else if (retailRemainder == 0)
    {
        // Make an otherwise-passing retail modulus fail. Splash divisors are
        // proven to be in [5, 10], so advancing by one cannot remain divisible.
        adjustedFrame += 1U;
    }

    if (adjustedFrame == frame)
        return;

    PPC_STORE_U32(kRawFrameCounterAddress, adjustedFrame);
    gSplashFrameOverride.frameAdjusted = true;
}
}

std::uint32_t ComputeSplashRippleDivisor(float horizontalSpeed) noexcept
{
    if (horizontalSpeed <= kLowRippleSpeed)
        return static_cast<std::uint32_t>(kLowSpeedDivisor);

    if (horizontalSpeed >= kHighRippleSpeed)
        return static_cast<std::uint32_t>(kHighSpeedDivisor);

    const float interpolation =
        (horizontalSpeed - kLowRippleSpeed) / kRippleSpeedSpan;
    const float interpolatedDelta = static_cast<float>(
        kHighSpeedDivisor - kLowSpeedDivisor) * interpolation;
    return static_cast<std::uint32_t>(
        kLowSpeedDivisor + static_cast<std::int32_t>(interpolatedDelta));
}

std::uint32_t ScaleParticleDivisor(std::uint32_t divisor,
                                   float timeStepSeconds) noexcept
{
    if (divisor == 0 || !(timeStepSeconds > 0.0f) ||
        timeStepSeconds >= kReferenceFrameSeconds)
    {
        return divisor;
    }

    const float frameScale = timeStepSeconds / kReferenceFrameSeconds;
    const float scaled = static_cast<float>(divisor) / frameScale;
    if (scaled >= static_cast<float>(
            std::numeric_limits<std::int32_t>::max()))
    {
        return static_cast<std::uint32_t>(
            std::numeric_limits<std::int32_t>::max());
    }

    const auto truncated = static_cast<std::int32_t>(scaled);
    return static_cast<std::uint32_t>(truncated > 1 ? truncated : 1);
}
}

PPC_FUNC_IMPL(__imp__sub_8267F318);
PPC_FUNC_IMPL(__imp__sub_8234AB70);
PPC_FUNC_IMPL(__imp__sub_82347B08);

// CBuoyancy::ProcessSplashVfx. Keep a private copy of the real frame counter
// around the exact retail gate; every other effect path sees retail state.
PPC_FUNC_HOOK(sub_8267F318)
{
    const auto previous =
        gta4::fps::water_particles::gSplashFrameOverride;
    gta4::fps::water_particles::gSplashFrameOverride = {
        true,
        false,
        PPC_LOAD_U32(
            gta4::fps::water_particles::kRawFrameCounterAddress),
    };

    __imp__sub_8267F318(ctx, base);
    gta4::fps::water_particles::RestoreRawFrameCounter();
    gta4::fps::water_particles::gSplashFrameOverride = previous;
}

// This helper's only caller is ProcessSplashVfx. Its return at 0x8267F6B8 is
// the last call before the speed-derived divisor and modulus, so adjusting the
// frame counter here changes only that one gate.
PPC_FUNC_HOOK(sub_8234AB70)
{
    const bool isSplashModuloPath =
        gta4::fps::water_particles::gSplashFrameOverride.active &&
        static_cast<std::uint32_t>(ctx.lr) ==
            gta4::fps::water_particles::kSplashModuloPathReturnAddress;

    __imp__sub_8234AB70(ctx, base);

    if (isSplashModuloPath)
        gta4::fps::water_particles::CorrectSplashRippleGate(ctx);
}

// sub_82347B08 is the leaf generic water-particle emitter. Restore the real
// frame immediately after the gate and before the corrected ripple is emitted.
PPC_FUNC_HOOK(sub_82347B08)
{
    if (gta4::fps::water_particles::gSplashFrameOverride.active &&
        static_cast<std::uint32_t>(ctx.lr) ==
            gta4::fps::water_particles::kSplashRippleEmitterReturnAddress)
    {
        gta4::fps::water_particles::RestoreRawFrameCounter();
    }

    __imp__sub_82347B08(ctx, base);
}
