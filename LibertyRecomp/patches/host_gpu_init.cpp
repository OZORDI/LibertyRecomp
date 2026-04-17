// =============================================================================
// Host GPU Device-Ready Init
// =============================================================================
//
// On Xbox 360, RAGE's grmSetup singleton has a 3-state init machine:
//   0 = UNINIT  (set by ConfigureDevice during construction)
//   1 = PENDING (set by D3D device-ready callback via VdSetGraphicsInterruptCallback)
//   2 = READY   (promoted by BeginFrame on first present-enabled call)
//
// In the recompilation, Video::CreateHostDevice() creates the real GPU device
// (Vulkan/Metal) in main() BEFORE the guest thread starts — the same pattern
// used by MarathonRecomp and UnleashedRecomp. The host device is already live,
// so the Xbox 360 init state machine is unnecessary. We reflect this reality by
// writing initState=2 (READY) after the grmSetup constructor completes.
//
// This is not "forcing" a state transition — it's telling RAGE the truth: the
// host GPU you asked for is already here and operational.
//
// Additionally, we pre-seed two global flags that the render dispatch path
// checks before submitting frames:
//   0x82B0B48C — render gate (>0 = dispatch enabled). On Xbox 360 this is set
//                by VdCallGraphicsNotificationRoutines after each Present. We
//                seed it to 1 so the first render dispatch can proceed.
//   0x831CF6C0 — GPU ready flag (set during InitDevice on Xbox 360 via HSIO
//                training result). We set it to 1 since the host GPU is live.
// =============================================================================

#include <api/Liberty.h>
#include <kernel/function.h>
#include <os/logger.h>

static constexpr uint32_t ADDR_GRM_SETUP_INSTANCE = 0x82B29EEC;
static constexpr uint32_t OFF_INIT_STATE           = 0x1D0;
static constexpr uint32_t ADDR_RENDER_GATE         = 0x82B0B48C;
static constexpr uint32_t ADDR_GPU_READY_FLAG      = 0x831CF6C0;

// =============================================================================
// sub_821B3CE8 — grmSetup System Construction
// =============================================================================
// Called once during engine boot from sub_82140000 ("GTA NY"). Allocates the
// 472-byte grmSetup singleton, constructs it via grcSetup ctor, overwrites
// vtable, configures shaders/resolution, calls InitDevice, sets up scheduler.
//
// After the original completes, the host GPU is already live (created before
// guest code started), so we set initState=2 (READY) and seed the render gate.
//
PPC_FUNC_IMPL(__imp__sub_821B3CE8);
PPC_FUNC_HOOK(sub_821B3CE8)
{
    __imp__sub_821B3CE8(ctx, base);

    uint32_t inst = PPC_LOAD_U32(ADDR_GRM_SETUP_INSTANCE);
    if (inst != 0)
    {
        // Host GPU device is already live — tell RAGE the device is READY.
        PPC_STORE_U8(inst + OFF_INIT_STATE, 2);

        // Seed the render gate so the first frame dispatch can proceed.
        PPC_STORE_U32(ADDR_RENDER_GATE, 1);

        // Mark GPU as ready (HSIO training result on Xbox 360).
        PPC_STORE_U32(ADDR_GPU_READY_FLAG, 1);

        LOGF_INFO("host_gpu_init: grmSetup at 0x{:08X} — initState=2 (READY), "
                  "render gate seeded, GPU ready flag set", inst);
    }
}
