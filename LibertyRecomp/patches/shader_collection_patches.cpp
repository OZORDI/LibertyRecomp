// =============================================================================
// RAGE fiCollection Shader Path System - Mount Chain Hooks
// =============================================================================

#include <api/Liberty.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>
#include <cstring>

// =============================================================================
// Address Constants
// =============================================================================

static constexpr uint32_t ADDR_SHADER_COLLECTION    = 0x82B07278;
static constexpr uint32_t OFF_PATH_COUNT             = 0xC00;
static constexpr uint32_t OFF_TOTAL_PATHS            = 0xC04;
static constexpr uint32_t ADDR_PATH_COUNT            = ADDR_SHADER_COLLECTION + OFF_PATH_COUNT;
static constexpr uint32_t ADDR_TOTAL_PATHS           = ADDR_SHADER_COLLECTION + OFF_TOTAL_PATHS;
static constexpr uint32_t ADDR_TUNE_SHADERS_LIB     = 0x82B0D348;
static constexpr uint32_t ADDR_SHADER_FACTORY_PTR   = 0x831C513C;
static constexpr uint32_t STR_COMMON_SHADERS         = 0x820009D4;
static constexpr uint32_t STR_UPDATE_SHADERS         = 0x820024F0;
static constexpr uint32_t ADDR_RPF_MODE_FLAG         = 0x831B59F8;

// =============================================================================
// sub_8285DD10 - RPF Mode Flag Check
// =============================================================================
// On Xbox 360, this returns 1 (RPF mode) because dword_831B59F8 is set during
// disc/boot detection. In the recomp, 0x831B59F8 is zero so it returns 0,
// causing directory mount instead of RPF open. Fix: return 1 unconditionally.
//
PPC_FUNC_HOOK(sub_8285DD10)
{
    uint32_t origFlag = PPC_LOAD_U32(ADDR_RPF_MODE_FLAG);
    LOGF_INFO("shader_collection: sub_8285DD10 (RPF mode check) flag=0x{:08X}, forcing return 1 (RPF mode)", origFlag);
    ctx.r3.u64 = 1;
}

// =============================================================================
// sub_828C8D78 - setShaderBasePath (passthrough)
// =============================================================================
// Copies the shader base path into aTuneShadersLib. Called once during
// grmSetup construction. We just delegate to the original.
//
PPC_FUNC_IMPL(__imp__sub_828C8D78);
PPC_FUNC_HOOK(sub_828C8D78)
{
    __imp__sub_828C8D78(ctx, base);
}

// =============================================================================
// sub_828CAA60 - Compiled Shader Loader (passthrough)
// =============================================================================
// Opens a fiStream for a shader path resolved via buildPath against the
// fiCollection at 0x82B07278. The recompiled code handles paths correctly.
//
PPC_FUNC_IMPL(__imp__sub_828CAA60);
PPC_FUNC_IMPL(__imp__sub_8284F310);  // pushPath original
PPC_FUNC_IMPL(__imp__sub_8284E830);  // popPath original
PPC_FUNC_HOOK(sub_828CAA60)
{
    __imp__sub_828CAA60(ctx, base);
}
