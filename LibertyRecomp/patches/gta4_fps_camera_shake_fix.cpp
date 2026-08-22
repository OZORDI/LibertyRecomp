#include "gta4_fps_camera_shake_fix.h"

#include <kernel/function.h>

#include <bit>
#include <cmath>
#include <cstdint>

namespace
{
// FusionFix gates noise and impulses with CTimer::fTimeStep, but preserves the
// retail CHandShaker integration multiplier sourced from CTimer::fCamTimeStep.
constexpr std::uint32_t kGameplayTimeStepAddress = 0x82C6C2AC;
constexpr std::uint32_t kCameraTimeStepAddress = 0x82C6C26C;

// Exact binary32 values calculated with Python and rounded once up front.
constexpr float kFixedTickSeconds = 0x1.111112p-5f;
constexpr float kTicksPerSecond = 30.0f;
constexpr float kRandUnitScale = 0x1.0002p-15f;

// The first 0x40 bytes are the output Matrix34. The remaining offsets are
// proven by both retail PPC assembly and the generated recompilation.
constexpr std::uint32_t kAngleX = 0x40;
constexpr std::uint32_t kAngleY = 0x44;
constexpr std::uint32_t kAngleZ = 0x48;
constexpr std::uint32_t kAngleLimitX = 0x50;
constexpr std::uint32_t kAngleLimitY = 0x54;
constexpr std::uint32_t kAngleLimitZ = 0x58;
constexpr std::uint32_t kNoiseScaleX = 0x60;
constexpr std::uint32_t kNoiseScaleY = 0x64;
constexpr std::uint32_t kNoiseScaleZ = 0x68;
constexpr std::uint32_t kVelocityX = 0x70;
constexpr std::uint32_t kVelocityY = 0x74;
constexpr std::uint32_t kVelocityZ = 0x78;
constexpr std::uint32_t kSameDirectionScaleX = 0x80;
constexpr std::uint32_t kSameDirectionScaleY = 0x84;
constexpr std::uint32_t kSameDirectionScaleZ = 0x88;
constexpr std::uint32_t kResponseMin = 0x90;
constexpr std::uint32_t kResponseMax = 0x94;
constexpr std::uint32_t kImpulsePeriod = 0x98;
constexpr std::uint32_t kImpulseMagnitude = 0x9C;

// The FusionFix correction intentionally uses one accumulator shared by every
// CHandShaker instance, matching its replacement function's static local.
float g_fixedTickAccumulator = 0.0f;

float LoadGuestFloat(std::uint8_t* base, std::uint32_t address)
{
    return std::bit_cast<float>(PPC_LOAD_U32(address));
}

void StoreGuestFloat(std::uint8_t* base, std::uint32_t address, float value)
{
    PPC_STORE_U32(address, std::bit_cast<std::uint32_t>(value));
}

float ClampAngle(float angle, float limit)
{
    if (angle <= -limit)
        angle = -limit;

    if (limit <= angle)
        angle = limit;

    return angle;
}
}

PPC_FUNC_IMPL(sub_829FFB80);
PPC_FUNC_IMPL(sub_82143C00);
PPC_FUNC_IMPL(sub_82225208);

PPC_FUNC_HOOK(sub_8236B4D8)
{
    const std::uint32_t shaker = ctx.r3.u32;
    const float outputScale = static_cast<float>(ctx.f1.f64);

    // FusionFix captures the angles once per Process call. Catch-up ticks use
    // the same captured values, exactly like its 30 Hz while-loop.
    const float initialAngleX = LoadGuestFloat(base, shaker + kAngleX);
    const float initialAngleY = LoadGuestFloat(base, shaker + kAngleY);
    const float initialAngleZ = LoadGuestFloat(base, shaker + kAngleZ);
    const float responseMin = LoadGuestFloat(base, shaker + kResponseMin);

    const float gameplayTimeStep =
        LoadGuestFloat(base, kGameplayTimeStepAddress);
    const float integrationScale =
        LoadGuestFloat(base, kCameraTimeStepAddress) * kTicksPerSecond;

    g_fixedTickAccumulator += gameplayTimeStep;

    while (g_fixedTickAccumulator >= kFixedTickSeconds)
    {
        g_fixedTickAccumulator -= kFixedTickSeconds;

        const float responseRange =
            LoadGuestFloat(base, shaker + kResponseMax) - responseMin;

        const float baseResponseX =
            std::fabs(initialAngleX / LoadGuestFloat(base, shaker + kAngleLimitX)) *
                responseRange +
            responseMin;
        const float baseResponseY =
            std::fabs(initialAngleY / LoadGuestFloat(base, shaker + kAngleLimitY)) *
                responseRange +
            responseMin;
        const float baseResponseZ =
            std::fabs(initialAngleZ / LoadGuestFloat(base, shaker + kAngleLimitZ)) *
                responseRange +
            responseMin;

        float responseX = baseResponseX;
        float responseY = baseResponseY;
        float responseZ = baseResponseZ;

        float velocityX = LoadGuestFloat(base, shaker + kVelocityX);
        float velocityY = LoadGuestFloat(base, shaker + kVelocityY);
        float velocityZ = LoadGuestFloat(base, shaker + kVelocityZ);

        if ((initialAngleX > 0.0f && velocityX > 0.0f) ||
            (initialAngleX < 0.0f && velocityX < 0.0f))
        {
            responseX *= LoadGuestFloat(base, shaker + kSameDirectionScaleX);
        }

        if ((initialAngleY > 0.0f && velocityY > 0.0f) ||
            (initialAngleY < 0.0f && velocityY < 0.0f))
        {
            responseY *= LoadGuestFloat(base, shaker + kSameDirectionScaleY);
        }

        if ((initialAngleZ > 0.0f && velocityZ > 0.0f) ||
            (initialAngleZ < 0.0f && velocityZ < 0.0f))
        {
            responseZ *= LoadGuestFloat(base, shaker + kSameDirectionScaleZ);
        }

        // Preserve the retail RNG order: Z, Y, X.
        sub_829FFB80(ctx, base);
        const float randomZ = static_cast<float>(ctx.r3.u32) * kRandUnitScale;
        sub_829FFB80(ctx, base);
        const float randomY = static_cast<float>(ctx.r3.u32) * kRandUnitScale;
        sub_829FFB80(ctx, base);
        const float randomX = static_cast<float>(ctx.r3.u32) * kRandUnitScale;

        float noiseX =
            randomX * LoadGuestFloat(base, shaker + kNoiseScaleX) * responseX;
        float noiseY =
            randomY * LoadGuestFloat(base, shaker + kNoiseScaleY) * responseY;
        float noiseZ =
            randomZ * LoadGuestFloat(base, shaker + kNoiseScaleZ) * responseZ;

        if (initialAngleX > 0.0f)
            noiseX = -noiseX;
        if (initialAngleY > 0.0f)
            noiseY = -noiseY;
        if (initialAngleZ > 0.0f)
            noiseZ = -noiseZ;

        velocityX += noiseX;
        velocityY += noiseY;
        velocityZ += noiseZ;

        // At 30 Hz the timestep multiplier is one. Removing it from this
        // probability term avoids its integer truncation to zero above 30 FPS.
        const std::int32_t impulsePeriod = static_cast<std::int32_t>(
            LoadGuestFloat(base, shaker + kImpulsePeriod));
        ctx.r3.s64 = 1;
        ctx.r4.s64 = impulsePeriod;
        sub_82143C00(ctx, base);

        if (ctx.r3.s32 == 2)
        {
            const float magnitude =
                LoadGuestFloat(base, shaker + kImpulseMagnitude);

            // Preserve the retail RNG order: Z, Y, X.
            sub_829FFB80(ctx, base);
            const float impulseZ =
                (static_cast<float>(ctx.r3.u32) * kRandUnitScale) *
                    (magnitude - -magnitude) -
                magnitude;
            sub_829FFB80(ctx, base);
            const float impulseY =
                (static_cast<float>(ctx.r3.u32) * kRandUnitScale) *
                    (magnitude - -magnitude) -
                magnitude;
            sub_829FFB80(ctx, base);
            const float impulseX =
                (static_cast<float>(ctx.r3.u32) * kRandUnitScale) *
                    (magnitude - -magnitude) -
                magnitude;

            velocityX += impulseX;
            velocityY += impulseY;
            velocityZ += impulseZ;
        }

        StoreGuestFloat(base, shaker + kVelocityX, velocityX);
        StoreGuestFloat(base, shaker + kVelocityY, velocityY);
        StoreGuestFloat(base, shaker + kVelocityZ, velocityZ);
    }

    // Velocity integration remains per rendered frame. Scaling by dt*30 keeps
    // the original 30 FPS response while making the angular motion framerate
    // independent and smooth between fixed-cadence noise/impulse updates.
    float angleX = initialAngleX +
        LoadGuestFloat(base, shaker + kVelocityX) * integrationScale;
    float angleY = initialAngleY +
        LoadGuestFloat(base, shaker + kVelocityY) * integrationScale;
    float angleZ = initialAngleZ +
        LoadGuestFloat(base, shaker + kVelocityZ) * integrationScale;

    angleX = ClampAngle(angleX, LoadGuestFloat(base, shaker + kAngleLimitX));
    angleY = ClampAngle(angleY, LoadGuestFloat(base, shaker + kAngleLimitY));
    angleZ = ClampAngle(angleZ, LoadGuestFloat(base, shaker + kAngleLimitZ));

    StoreGuestFloat(base, shaker + kAngleX, angleX);
    StoreGuestFloat(base, shaker + kAngleY, angleY);
    StoreGuestFloat(base, shaker + kAngleZ, angleZ);

    // Supply the original Matrix34 routine with a short, aligned guest-stack
    // vector. Its fourth lane is padding and is intentionally zeroed.
    constexpr std::uint32_t kScratchFrameSize = 0x20;
    constexpr std::uint32_t kEulerVectorOffset = 0x10;

    const std::uint32_t oldStackPointer = ctx.r1.u32;
    const std::uint32_t scratchStackPointer =
        oldStackPointer - kScratchFrameSize;
    const std::uint32_t eulerVector =
        scratchStackPointer + kEulerVectorOffset;

    PPC_STORE_U32(scratchStackPointer, oldStackPointer);
    StoreGuestFloat(base, eulerVector + 0x0, angleX * outputScale);
    StoreGuestFloat(base, eulerVector + 0x4, angleY * outputScale);
    StoreGuestFloat(base, eulerVector + 0x8, angleZ * outputScale);
    StoreGuestFloat(base, eulerVector + 0xC, 0.0f);

    ctx.r1.u64 = scratchStackPointer;
    ctx.r3.u64 = shaker;
    ctx.r4.u64 = eulerVector;
    sub_82225208(ctx, base);
    ctx.r1.u64 = oldStackPointer;
}
