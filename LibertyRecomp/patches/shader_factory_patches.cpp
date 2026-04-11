// =============================================================================
// Shader Factory Chain - Device Guard Bypass
// =============================================================================

#include <api/Liberty.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>

// =============================================================================
// Guest Memory Addresses
// =============================================================================

namespace ShaderFactory {

constexpr uint32_t DEVICE_PTR              = 0x831C22A4;
constexpr uint32_t DEVICE_PTR_COPY         = 0x831C2294;
constexpr uint32_t TECHNIQUE_TABLE_BASE    = 0x831C3DF0;
constexpr uint32_t TECHNIQUE_TABLE_COUNT   = 0x831C4FF0;
constexpr uint32_t SHADER_TABLE            = 0x831C30B8;
constexpr uint32_t SHADER_TABLE_END        = 0x831C32B8;
constexpr uint32_t BUILTIN_SHADER_FX       = 0x831C25A8;
constexpr uint32_t TECH_DRAW               = 0x831C23E0;
constexpr uint32_t TECH_UNLIT_DRAW         = 0x831C23BC;
constexpr uint32_t TECH_DRAWSKINNED        = 0x831C23FC;
constexpr uint32_t TECH_UNLIT_DRAWSKINNED  = 0x831C23C4;
constexpr uint32_t TECH_DRAWBLIT           = 0x831C2404;
constexpr uint32_t PARAM_DIFFUSETEX        = 0x831C2290;
constexpr uint32_t VS_CREATE_CALLBACK      = 0x831CF6B0;
constexpr uint32_t PS_CREATE_CALLBACK      = 0x831CF6B4;
constexpr uint32_t VFS_ROOT                = 0x82B07278;
constexpr uint32_t DEVICE_SENTINEL         = 0xDEAD0001;
constexpr uint32_t FXC_MAGIC_AXGR          = 0x61786772;

} // namespace ShaderFactory

// =============================================================================
// sub_828CA318 - Parse Compiled Effect (DEVICE GUARD BYPASS)
// =============================================================================
// Sets a sentinel device pointer so shader objects are created by
// sub_82A42BA8 and sub_82A42CB8 during FXC parsing.
//
PPC_FUNC_IMPL(__imp__sub_828CA318);
PPC_FUNC_HOOK(sub_828CA318)
{
    using namespace ShaderFactory;

    uint32_t savedDevicePtr = PPC_LOAD_U32(DEVICE_PTR);

    if (savedDevicePtr == 0)
    {
        PPC_STORE_U32(DEVICE_PTR, DEVICE_SENTINEL);
        LOG_INFO("shader_factory: Set device guard sentinel for FXC parsing");
    }

    __imp__sub_828CA318(ctx, base);

    if (savedDevicePtr == 0)
    {
        PPC_STORE_U32(DEVICE_PTR, savedDevicePtr);
    }
}

// =============================================================================
// sub_828C8108 - Read Pixel Shader Entry (DEVICE GUARD BYPASS)
// =============================================================================
// Safety net in case sub_828C8108 is called outside sub_828CA318's scope.
//
PPC_FUNC_IMPL(__imp__sub_828C8108);
PPC_FUNC_HOOK(sub_828C8108)
{
    using namespace ShaderFactory;

    uint32_t savedDevicePtr = PPC_LOAD_U32(DEVICE_PTR);

    if (savedDevicePtr == 0)
    {
        PPC_STORE_U32(DEVICE_PTR, DEVICE_SENTINEL);
    }

    __imp__sub_828C8108(ctx, base);

    if (savedDevicePtr == 0)
    {
        PPC_STORE_U32(DEVICE_PTR, savedDevicePtr);
    }
}
