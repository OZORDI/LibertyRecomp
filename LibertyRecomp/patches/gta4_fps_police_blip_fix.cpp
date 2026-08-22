#include "gta4_fps_police_blip_fix.h"

#include "gta4_fps_helicopter_fix.h"

#include <bit>

#include <cpu/ppc_context.h>
#include <kernel/function.h>

namespace gta4::fps::police_blip
{
// sub_8227FE40 stores its unscaled elapsed seconds here once per timer update.
constexpr std::uint32_t kTimerUnscaledElapsedSecondsAddress = 0x82C6C29C;

// Exact binary64 values used by FusionFix's millisecond accumulator, derived
// with Python's Fraction(1000, 1) and Fraction(1000, 30).
constexpr double kMillisecondsPerSecond = 0x1.f400000000000p+9;
constexpr double kLogicalFramePeriodMilliseconds = 0x1.0aaaaaaaaaaabp+5;

// sub_8215B4C0 calls the blip-rotation setter here after calculating its
// chase-vehicle angle from the raw frame counter.
constexpr std::uint32_t kChaseBlipRotationReturnAddress = 0x8215B73C;
constexpr std::uint32_t kRadiansPerDegreeAddress = 0x820BEEF4;
constexpr std::uint32_t kDegreesPerLogicalFrame = 21;
constexpr std::uint32_t kDegreesPerCircle = 360;

static LogicalFrameCounter30Hz gLogicalFrameCounter;

void LogicalFrameCounter30Hz::Reset() noexcept
{
    elapsedRemainderMilliseconds_ = 0.0;
    value_.store(0, std::memory_order_release);
}

void LogicalFrameCounter30Hz::Advance(float elapsedSeconds) noexcept
{
    elapsedRemainderMilliseconds_ +=
        static_cast<double>(elapsedSeconds) * kMillisecondsPerSecond;

    while (elapsedRemainderMilliseconds_ >= kLogicalFramePeriodMilliseconds)
    {
        elapsedRemainderMilliseconds_ -= kLogicalFramePeriodMilliseconds;
        value_.fetch_add(1, std::memory_order_release);
    }
}

std::uint32_t LogicalFrameCounter30Hz::Value() const noexcept
{
    return value_.load(std::memory_order_acquire);
}

std::uint32_t GetLogicalFrameCounter() noexcept
{
    return gLogicalFrameCounter.Value();
}
}

PPC_FUNC_IMPL(__imp__sub_8227F978);
PPC_FUNC_IMPL(__imp__sub_8227FE40);
PPC_FUNC_IMPL(__imp__sub_82334990);

// CTimer initialization. The Xbox function initializes the raw frame counter
// at 0x82C6C2B8; keep the 30 Hz sidecar counter on the same lifetime.
PPC_FUNC_HOOK(sub_8227F978)
{
    __imp__sub_8227F978(ctx, base);
    gta4::fps::police_blip::gLogicalFrameCounter.Reset();
    gta4::fps::helicopter::SetLogicalFrameCounterProvider(
        gta4::fps::police_blip::GetLogicalFrameCounter);
}

// CTimer update. Advance the sidecar from the exact unscaled elapsed-seconds
// value produced by the original Xbox timer update.
PPC_FUNC_HOOK(sub_8227FE40)
{
    __imp__sub_8227FE40(ctx, base);

    const float elapsedSeconds = std::bit_cast<float>(
        PPC_LOAD_U32(gta4::fps::police_blip::kTimerUnscaledElapsedSecondsAddress));
    gta4::fps::police_blip::gLogicalFrameCounter.Advance(elapsedSeconds);
}

// Blip rotation setter. Override f1 only at sub_8215B4C0's chase-vehicle
// rotation callsite; all other blip rotations retain their original inputs.
PPC_FUNC_HOOK(sub_82334990)
{
    if (ctx.lr == gta4::fps::police_blip::kChaseBlipRotationReturnAddress)
    {
        const std::uint32_t phaseDegrees =
            (gta4::fps::police_blip::GetLogicalFrameCounter() *
             gta4::fps::police_blip::kDegreesPerLogicalFrame) %
            gta4::fps::police_blip::kDegreesPerCircle;
        const float radiansPerDegree = std::bit_cast<float>(
            PPC_LOAD_U32(gta4::fps::police_blip::kRadiansPerDegreeAddress));

        ctx.f1.f64 = static_cast<double>(
            static_cast<float>(phaseDegrees) * radiansPerDegree);
    }

    __imp__sub_82334990(ctx, base);
}
