#include "gta4_fps_ladder_fix.h"

#include <kernel/function.h>

PPC_FUNC_IMPL(__imp__sub_823FE5F0);

// sub_823FE5F0 constructs CTaskSimpleMoveAchieveHeading and stores f3 as its
// completion-angle threshold. Override only the call from
// CTaskComplexClimbLadder::CreateSubTask; all other tasks retain their retail
// threshold inputs.
PPC_FUNC_HOOK(sub_823FE5F0)
{
    if (ctx.lr == gta4::fps::ladder::kMoveAchieveHeadingReturnAddress)
    {
        ctx.f3.f64 = static_cast<double>(
            gta4::fps::ladder::kHeadingAngleThreshold);
    }

    __imp__sub_823FE5F0(ctx, base);
}
