#include "gta4_fps_helicopter_fix.h"

#include <atomic>
#include <cstdint>

#include <cpu/ppc_context.h>
#include <kernel/function.h>

namespace gta4::fps::helicopter
{
namespace
{
// sub_822AB0B0 (CHeli vfunc 49 / PreRender2) derives four light phases from
// (model index + raw frame counter) modulo 32. These are the return addresses
// immediately after its component 65 through 68 light-update calls.
constexpr std::uint32_t kComponent65LightReturnAddress = 0x822AB200;
constexpr std::uint32_t kComponent66LightReturnAddress = 0x822AB21C;
constexpr std::uint32_t kComponent67LightReturnAddress = 0x822AB258;
constexpr std::uint32_t kComponent68LightReturnAddress = 0x822AB270;
constexpr std::uint32_t kVehicleModelIndexOffset = 0x2C;
constexpr std::uint32_t kBlinkPhaseMask = 0x1F;

// The phase values are the exact modulo-32 solutions for the retail PPC's
// tests of sum, sum + 12, sum + 15, and sum - 8 respectively.
constexpr std::uint32_t kComponent65Phase = 0;
constexpr std::uint32_t kComponent66Phase = 20;
constexpr std::uint32_t kComponent67Phase = 17;
constexpr std::uint32_t kComponent68Phase = 8;

std::atomic<LogicalFrameCounterProvider> gLogicalFrameCounterProvider{nullptr};

bool GetBlinkPhaseForCall(std::uint32_t returnAddress,
                          std::uint32_t& phase) noexcept
{
    switch (returnAddress)
    {
    case kComponent65LightReturnAddress:
        phase = kComponent65Phase;
        return true;
    case kComponent66LightReturnAddress:
        phase = kComponent66Phase;
        return true;
    case kComponent67LightReturnAddress:
        phase = kComponent67Phase;
        return true;
    case kComponent68LightReturnAddress:
        phase = kComponent68Phase;
        return true;
    default:
        return false;
    }
}

void CorrectBlinkPhase(PPCContext& ctx) noexcept
{
    std::uint32_t targetPhase = 0;
    if (!GetBlinkPhaseForCall(ctx.lr, targetPhase))
        return;

    const auto provider =
        gLogicalFrameCounterProvider.load(std::memory_order_acquire);
    if (provider == nullptr)
        return;

    const std::uint32_t modelIndex =
        PPC_LOAD_U16(ctx.r3.u32 + kVehicleModelIndexOffset);
    const std::uint32_t phase =
        (provider() + modelIndex) & kBlinkPhaseMask;
    ctx.r5.u64 = phase == targetPhase ? 1 : 0;
}
}

void SetLogicalFrameCounterProvider(
    LogicalFrameCounterProvider provider) noexcept
{
    gLogicalFrameCounterProvider.store(provider, std::memory_order_release);
}
}

PPC_FUNC_IMPL(__imp__sub_822AAA30);
PPC_FUNC_IMPL(__imp__sub_822AAD38);

// The two retail light helpers receive their on/off decision in r5. Override
// it only at sub_822AB0B0's four helicopter blinker callsites.
PPC_FUNC_HOOK(sub_822AAA30)
{
    gta4::fps::helicopter::CorrectBlinkPhase(ctx);
    __imp__sub_822AAA30(ctx, base);
}

PPC_FUNC_HOOK(sub_822AAD38)
{
    gta4::fps::helicopter::CorrectBlinkPhase(ctx);
    __imp__sub_822AAD38(ctx, base);
}
