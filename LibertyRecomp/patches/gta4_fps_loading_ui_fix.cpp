#include "gta4_fps_loading_ui_fix.h"

#include "gta4_fps_police_blip_fix.h"

#include <cstdint>

#include <cpu/ppc_context.h>
#include <kernel/function.h>

namespace gta4::fps::loading_ui
{
// sub_82223CF8 calls sub_821F2FD8 here immediately before preparing the
// cd_spinner sprite. Its only raw-frame-counter read follows at 0x82224614.
constexpr std::uint32_t kCdSpinnerDrawStateReturnAddress = 0x82224544;

// CTimer's physical frame counter used by the unmodified Xbox comparison.
constexpr std::uint32_t kRawFrameCounterAddress = 0x82C6C2B8;

// Private CD-spinner state in sub_82223CF8. The routine compares this value
// with CTimer's raw frame counter before advancing its angle accumulator.
constexpr std::uint32_t kCdSpinnerLastFrameAddress = 0x82BD418C;

class CdSpinnerLogicalFrameGate final
{
public:
    [[nodiscard]] bool Observe(std::uint32_t logicalFrame) noexcept
    {
        if (logicalFrame == lastLogicalFrame_)
            return false;

        lastLogicalFrame_ = logicalFrame;
        return true;
    }

private:
    std::uint32_t lastLogicalFrame_ = 0;
};

thread_local CdSpinnerLogicalFrameGate gCdSpinnerLogicalFrameGate;
}

PPC_FUNC_IMPL(__imp__sub_821F2FD8);

PPC_FUNC_HOOK(sub_821F2FD8)
{
    const std::uint32_t returnAddress = static_cast<std::uint32_t>(ctx.lr);
    __imp__sub_821F2FD8(ctx, base);

    if (returnAddress !=
        gta4::fps::loading_ui::kCdSpinnerDrawStateReturnAddress)
    {
        return;
    }

    const std::uint32_t rawFrame =
        PPC_LOAD_U32(gta4::fps::loading_ui::kRawFrameCounterAddress);
    const bool advance =
        gta4::fps::loading_ui::gCdSpinnerLogicalFrameGate.Observe(
            gta4::fps::police_blip::GetLogicalFrameCounter());

    // sub_82223CF8 compares this private value with rawFrame, advances only on
    // inequality, then stores rawFrame back. Supplying an equal value skips a
    // physical frame; flipping one bit guarantees inequality on a 30 Hz tick.
    PPC_STORE_U32(gta4::fps::loading_ui::kCdSpinnerLastFrameAddress,
                  advance ? (rawFrame ^ 1U) : rawFrame);
}
