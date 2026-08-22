// =============================================================================
// RAGE fiCollection Shader Path System - Mount Chain Hooks
// =============================================================================

#include <api/Liberty.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>
#include <os/diag.h>
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

// sub_8285DD10 — removed. Was forcing RPF mode by overriding return value.
// Instead, set the RPF flag (0x831B59F8) in guest memory during init so the
// recompiled code naturally takes the RPF path. See memory.cpp.

// =============================================================================
// sub_828C8D78 - setShaderBasePath (passthrough)
// =============================================================================
// Copies the shader base path into aTuneShadersLib. Called once during
// grmSetup construction. We just delegate to the original.
//
PPC_FUNC_IMPL(__imp__sub_828C8D78);
PPC_FUNC_HOOK(sub_828C8D78)
{
    if (::os::diag::ShouldEmit()) {
        const uint32_t pathAddr = ctx.r3.u32;
        char pathStr[64] = {};
        if (pathAddr) {
            for (int i = 0; i < 63; i++) {
                char c = static_cast<char>(PPC_LOAD_U8(pathAddr + i));
                if (c == 0) break;
                pathStr[i] = c;
            }
        }
        fprintf(stderr, "[SHADER-DIAG] setShaderBasePath('%s') addr=0x%08X\n", pathStr, pathAddr);
        fflush(stderr);
    }
    __imp__sub_828C8D78(ctx, base);
}

// =============================================================================
// sub_828CAA60 - Compiled Shader Loader (with diagnostics)
// =============================================================================
static int s_shaderLoadCount = 0;
PPC_FUNC_IMPL(__imp__sub_828CAA60);
PPC_FUNC_IMPL(__imp__sub_8284F310);
PPC_FUNC_IMPL(__imp__sub_8284E830);
PPC_FUNC_HOOK(sub_828CAA60)
{
    uint32_t objAddr = ctx.r3.u32;
    uint32_t nameAddr = ctx.r4.u32;
    uint32_t flags = ctx.r5.u32;
    const bool diagnostics = ::os::diag::ShouldEmit();
    const int n = diagnostics ? s_shaderLoadCount++ : 0;
    char name[64] = {};
    uint32_t pathBefore = 0;
    if (diagnostics) {
        if (nameAddr) {
            for (int i = 0; i < 63; i++) {
                char c = static_cast<char>(PPC_LOAD_U8(nameAddr + i));
                if (c == 0) break;
                name[i] = c;
            }
        }
        pathBefore = objAddr != 0 ? PPC_LOAD_U32(objAddr + 32) : 0;
    }

    if (diagnostics && n < 20) {
        fprintf(stderr, "[SHADER-DIAG] sub_828CAA60 #%d obj=0x%08X name='%s'(0x%08X) flags=%u path_before=0x%08X\n",
                n, objAddr, name, nameAddr, flags, pathBefore);
        fflush(stderr);
    }

    // ---------------------------------------------------------------------
    // Fix: ensure setShaderBasePath has run before the loader dereferences
    // the shader collection.
    //
    // sub_821B3CE8 (grmSetup ctor) calls sub_828C58D0 (which invokes this
    // loader) BEFORE calling sub_828C8D78 (setShaderBasePath). On the Xbox
    // 360 the stale base-path buffer (`aTuneShadersLib` @ 0x82B0D348) held
    // whatever the BSS/image zero-init plus earlier static ctors left
    // there, but on the recomp it is pure-zero, so sub_828CAA60's path-
    // build branch constructs `"/<name>.fxc"` (empty base) and the
    // downstream fiDevice lookup returns a null/invalid device pointer
    // → vtable deref at guest 0 → SIGBUS.
    //
    // Detect the empty base-path and pre-populate it with the canonical
    // "common:/shaders" value (same string the game writes on the real
    // hardware). This is a pre-call fix-up — the real sub_828C8D78 still
    // runs later under sub_821B3CE8 and overwrites with the identical
    // value, so no ordering assumption is broken.
    if (PPC_LOAD_U8(ADDR_TUNE_SHADERS_LIB) == 0)
    {
        PPCContext save_ctx = ctx;
        ctx.r3.u64 = STR_COMMON_SHADERS;
        __imp__sub_828C8D78(ctx, base);
        ctx = save_ctx;
        if (diagnostics && n < 20) {
            fprintf(stderr, "[SHADER-FIX ] sub_828CAA60 #%d primed aTuneShadersLib with 'common:/shaders' "
                            "before first loader call\n", n);
            fflush(stderr);
        }
    }

    __imp__sub_828CAA60(ctx, base);

    if (diagnostics && n < 20) {
        const uint32_t pathAfter = objAddr != 0 ? PPC_LOAD_U32(objAddr + 32) : 0;
        const uint32_t result = ctx.r3.u32;
        fprintf(stderr, "[SHADER-DIAG] sub_828CAA60 #%d result=%u path_after=0x%08X\n",
                n, result, pathAfter);
        fflush(stderr);
    }
}
