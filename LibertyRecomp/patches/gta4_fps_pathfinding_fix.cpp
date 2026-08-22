#include "gta4_fps_pathfinding_fix.h"

#include <bit>
#include <cstdint>
#include <mutex>
#include <unordered_map>

#include <cpu/ppc_context.h>
#include <kernel/function.h>

namespace gta4::fps::pathfinding
{
// CTimer's gameplay timestep in seconds. Retail sub_8227FE40 derives this
// scaled timestep separately from the camera timestep at 0x82C6C26C.
constexpr std::uint32_t kGameplayTimeStepAddress = 0x82C6C2AC;

// Exact binary32 result of 1.0f / 30.0f, calculated with Python.
constexpr float kStaticCounterPeriodSeconds = 0x1.111112p-5f;

static std::mutex gAccumulatorMutex;
static std::unordered_map<std::uint32_t, StaticCounterAccumulator30Hz>
    gAccumulators;

bool StaticCounterAccumulator30Hz::Advance(float timeStepSeconds) noexcept
{
    remainderSeconds_ += timeStepSeconds;

    // FusionFix's loop returns after the first iteration, so a long frame runs
    // the original counter at most once and preserves the excess remainder.
    if (remainderSeconds_ < kStaticCounterPeriodSeconds)
        return false;

    remainderSeconds_ -= kStaticCounterPeriodSeconds;
    return true;
}

void AddAccumulator(std::uint32_t intelligence)
{
    const std::scoped_lock lock(gAccumulatorMutex);
    gAccumulators.insert_or_assign(intelligence, StaticCounterAccumulator30Hz{});
}

void RemoveAccumulator(std::uint32_t intelligence)
{
    const std::scoped_lock lock(gAccumulatorMutex);
    gAccumulators.erase(intelligence);
}

enum class ProcessDecision
{
    MissingAccumulator,
    Skip,
    RunOriginal,
};

ProcessDecision AdvanceAccumulator(std::uint32_t intelligence,
                                   float timeStepSeconds)
{
    const std::scoped_lock lock(gAccumulatorMutex);
    const auto it = gAccumulators.find(intelligence);
    if (it == gAccumulators.end())
        return ProcessDecision::MissingAccumulator;

    return it->second.Advance(timeStepSeconds)
        ? ProcessDecision::RunOriginal
        : ProcessDecision::Skip;
}
}

PPC_FUNC_IMPL(__imp__sub_823C17E8);
PPC_FUNC_IMPL(__imp__sub_823C2E60);
PPC_FUNC_IMPL(__imp__sub_823C3068);

// CPedIntelligence constructor. Keep the host-side accumulator on precisely
// the same lifetime as the Xbox object, matching FusionFix's class extension.
PPC_FUNC_HOOK(sub_823C2E60)
{
    const std::uint32_t intelligence = ctx.r3.u32;
    __imp__sub_823C2E60(ctx, base);
    gta4::fps::pathfinding::AddAccumulator(intelligence);
}

// CPedIntelligence destructor. Remove state before guest teardown, matching
// FusionFix and preventing pooled addresses from inheriting old remainder.
PPC_FUNC_HOOK(sub_823C3068)
{
    const std::uint32_t intelligence = ctx.r3.u32;
    gta4::fps::pathfinding::RemoveAccumulator(intelligence);
    __imp__sub_823C3068(ctx, base);
}

// CPedIntelligence::ProcessStaticCounter. Retail sub_823C35C0 calls this once
// at the start of CPedIntelligence::ProcessFirst; its counter updates are at
// +0x264/+0x268 and its stored-position vector starts at +0x270.
PPC_FUNC_HOOK(sub_823C17E8)
{
    const std::uint32_t intelligence = ctx.r3.u32;
    const float timeStepSeconds = std::bit_cast<float>(
        PPC_LOAD_U32(gta4::fps::pathfinding::kGameplayTimeStepAddress));
    const auto decision = gta4::fps::pathfinding::AdvanceAccumulator(
        intelligence, timeStepSeconds);

    // FusionFix falls through to the original routine if no extension exists.
    if (decision != gta4::fps::pathfinding::ProcessDecision::Skip)
        __imp__sub_823C17E8(ctx, base);
}
