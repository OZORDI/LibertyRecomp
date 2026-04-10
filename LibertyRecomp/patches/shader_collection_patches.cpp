// =============================================================================
// RAGE fiCollection Shader Path System - Decompiled Mount Chain
// =============================================================================
//
// This file decompiles the entire fiCollection path subsystem that underpins
// RAGE's shader virtual filesystem resolution. The fiCollection object at
// 0x82B07278 is a static .data structure that manages a PATH STACK for
// resolving relative shader filenames into full "common:/shaders/" VFS paths.
//
// ARCHITECTURE:
//
//   fiCollection at 0x82B07278 (3076+ bytes):
//     +0x000..0x0FF  path[0]  (256 bytes) - base path slot 0
//     +0x100..0x1FF  path[1]  (256 bytes) - base path slot 1
//     +0x200..0x2FF  path[2]  (256 bytes) - base path slot 2
//     +0x300..0x3FF  path[3]  (256 bytes) - dynamic push slot (pathCount=0)
//     +0x400..0x4FF  path[4]  (256 bytes) - dynamic push slot (pathCount=1)
//     ...
//     +0xC00         pathCount (uint32)   - current dynamic path stack depth
//     +0xC04         totalPaths (uint32)  - total path count for findFile iteration
//
//   Path encoding: slot N is at base + (N * 256). The first 3 slots (0-2)
//   are pre-configured base paths. Dynamic paths pushed via pushPath() go
//   into slots (pathCount + 3) and above.
//
// MOUNT CHAIN (engine init):
//
//   sub_821B3CE8 (grmSetup construction)
//     -> sub_828C8D78("common:/shaders")  // copies path to aTuneShadersLib buffer
//     -> sub_828C5928()                    // device init, loads base shaders
//     -> sub_821D6140()                    // RAGE shader+postfx init
//         -> sub_82275E38("common:/shaders")  // shadow shader setup
//             -> sub_8284F310(0x82B07278, "common:/shaders")  // PUSH path
//             -> ... shader factory loads shaders via sub_828CAA60 ...
//                 -> sub_828CAA60 calls sub_8284F0C0(0x82B07278, ...)
//                    to resolve each shader name against the path stack
//             -> sub_8284E830(0x82B07278)                      // POP path
//
// KEY FUNCTIONS:
//
//   sub_8284F310  pushPath(base, path)
//     Stores the resolved path at slot (pathCount+3), increments pathCount.
//     For absolute paths (containing ':'), stores directly.
//     For relative paths, prepends the current dynamic base path.
//
//   sub_8284E830  popPath(base)
//     Decrements pathCount. Undoes a pushPath.
//
//   sub_8284F0C0  buildPath(base, outBuf, bufSize, filename, ext, pathIndex)
//     Resolves a relative filename against a specific path slot.
//     If filename is relative (no '/', '\\', or ':'), copies path[pathIndex]
//     as prefix, then checks dynamic path at (pathCount+3) for overlay.
//     Appends filename and extension via sub_8284ED70.
//
//   sub_8284F468  findFile(base, name, ext, flags, openMode)
//     Iterates path slots 0..totalPaths, calling buildPath for each,
//     then tries to open the resulting path via sub_8285AA68 (fiStream::Open).
//     Returns the first successful stream handle.
//
//   sub_828CAA60  loadCompiledShader(factory, name, flags)
//     Calls buildPath to resolve "name.fxc" against the path collection,
//     swaps extension to "fxl_final/", opens stream, reads "axgr" magic,
//     and parses the compiled shader archive.
//
//   sub_8284E690  isRelativePath(base, path)
//     Returns 1 if path has no leading '/', '\\', or embedded ':'.
//     Returns 0 for absolute/device paths like "common:/shaders".
//
//   sub_8284E700  collapseDotDot(path)
//     Resolves "/../" sequences in a built path string. Called in a loop
//     by buildPath until no more ".." segments remain.
//
//   sub_8284ED70  appendFilenameExt(base, outBuf, bufSize, filename, ext)
//     Appends filename to outBuf, normalizes backslashes to forward slashes,
//     removes duplicate slashes, and appends ".ext" if needed.
//
//   sub_8284EAF0  copyPathToSlot(dest, maxLen, src)
//     Copies a path string into a 256-byte slot, normalizes backslashes
//     to forward slashes, and ensures a trailing '/'.
//
//   sub_8284E060  hashPath(str, seed)
//     Jenkins-style hash of a path string with case folding (A-Z -> a-z)
//     and backslash normalization ('\\' -> '/'). Used for shader name lookup.
//
//   sub_8284E830  popPath(base) -- trivially decrements pathCount at +0xC00.
//
//   sub_82211840  strcat_safe(dest, src, maxLen)
//     Safe string concatenation with length limit. Used throughout the
//     path building pipeline.
//
//   sub_828C8D78  setShaderBasePath(path)
//     Copies the given path string (e.g. "common:/shaders") into the static
//     128-byte buffer at aTuneShadersLib (0x82B0D348). Called once during
//     grmSetup construction.
//
// WHY 0x82B07278 pathCount IS ZERO AFTER INIT:
//
//   The pushPath/popPath mechanism is a SCOPED path stack. sub_82275E38
//   pushes "common:/shaders" before loading shaders, then pops it after.
//   This is by design: the path is only needed during the batch load window.
//
//   However, sub_828CAA60 (compiled shader loader) also uses the path
//   collection. If any shader is loaded AFTER sub_82275E38 returns (e.g.
//   lazy load, reload, or DLC shaders), pathCount=0 means the "common:/shaders"
//   prefix is absent, and buildPath produces bare filenames that fail VFS
//   lookup. On Xbox 360 this never happens because all shaders are loaded
//   during init. On the recomp, deferred loading or re-entry can trigger it.
//
// HOOK STRATEGY:
//
//   We hook sub_82275E38 (the shadow shader init function) to ensure the
//   path push succeeds, and we hook sub_828CAA60 (shader loader) with a
//   safety net that re-pushes "common:/shaders" if pathCount is 0 when a
//   shader load is attempted outside the init window.
//
// =============================================================================

#include <api/Liberty.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>
#include <cstring>

// =============================================================================
// Address Constants
// =============================================================================

// Static fiCollection object for shader paths (in .data section)
static constexpr uint32_t ADDR_SHADER_COLLECTION    = 0x82B07278;

// Offset of pathCount within the fiCollection object
static constexpr uint32_t OFF_PATH_COUNT             = 0xC00;  // 3072
// Offset of totalPaths (used by findFile iteration)
static constexpr uint32_t OFF_TOTAL_PATHS            = 0xC04;  // 3076

// Computed: pathCount field absolute address
static constexpr uint32_t ADDR_PATH_COUNT            = ADDR_SHADER_COLLECTION + OFF_PATH_COUNT;   // 0x82B07E78
static constexpr uint32_t ADDR_TOTAL_PATHS           = ADDR_SHADER_COLLECTION + OFF_TOTAL_PATHS;  // 0x82B07E7C

// aTuneShadersLib: 128-byte static buffer storing the base shader path
// Written by sub_828C8D78 during grmSetup construction with "common:/shaders"
static constexpr uint32_t ADDR_TUNE_SHADERS_LIB     = 0x82B0D348;

// grmShaderFactory singleton pointer
// lis r11,-31972; lwz r3,20796(r11) => 0x831C0000 + 20796 = 0x831C513C
static constexpr uint32_t ADDR_SHADER_FACTORY_PTR   = 0x831C513C;

// Shadow shader globals written by sub_82275E38
static constexpr uint32_t ADDR_SHADOW_EFFECT_GROUP   = 0x82C58D3C;
static constexpr uint32_t ADDR_SHADOW_Z_POINT        = 0x82C58D34;
static constexpr uint32_t ADDR_SHADOW_Z_DIR          = 0x82C58D38;
static constexpr uint32_t ADDR_SHADOW_SMARTBLIT      = 0x82C58D30;

// String constants in .rdata
static constexpr uint32_t STR_COMMON_SHADERS         = 0x820009D4;  // "common:/shaders"
static constexpr uint32_t STR_UPDATE_SHADERS         = 0x820024F0;  // "update:/shaders"

// Mount table globals
static constexpr uint32_t ADDR_MOUNT_TABLE_PTR       = 0x831AB940;  // ptr to 276-byte entry array
static constexpr uint32_t ADDR_MOUNT_TABLE_COUNT     = 0x831AB944;  // u16 active count
static constexpr uint32_t ADDR_MOUNT_TABLE_CAPACITY  = 0x831AB946;  // u16 capacity

// RPF mode flag — must be 1 for common.rpf to be opened as an RPF archive
static constexpr uint32_t ADDR_RPF_MODE_FLAG         = 0x831B59F8;

// =============================================================================
// sub_8285DD10 - RPF Mode Flag Check
// =============================================================================
// On Xbox 360, this returns 1 (RPF mode) because dword_831B59F8 is set during
// disc/boot detection. In the recomp, 0x831B59F8 is zero so it returns 0,
// causing Path B (directory mount) instead of Path A (RPF open). This means
// common.rpf is never opened as a fiPackfile, 'common:' is never registered
// in the mount table, and ALL shader loads fail.
//
// Fix: return 1 unconditionally — we always want RPF mode in the recomp.
//
PPC_FUNC_HOOK(sub_8285DD10)
{
    uint32_t origFlag = PPC_LOAD_U32(ADDR_RPF_MODE_FLAG);
    LOGF_INFO("shader_collection: sub_8285DD10 (RPF mode check) flag=0x{:08X}, forcing return 1 (RPF mode)", origFlag);

    // Force RPF mode — common.rpf/xbox360.rpf/audio.rpf are RPF archives
    ctx.r3.u64 = 1;
}

// =============================================================================
// Diagnostic: Dump mount table state
// =============================================================================
static void DumpMountTable(uint8_t* base)
{
    uint32_t tablePtr = PPC_LOAD_U32(ADDR_MOUNT_TABLE_PTR);
    uint16_t count    = PPC_LOAD_U16(ADDR_MOUNT_TABLE_COUNT);
    uint16_t capacity = PPC_LOAD_U16(ADDR_MOUNT_TABLE_CAPACITY);

    LOGF_INFO("shader_collection: Mount table @ 0x{:08X}: ptr=0x{:08X} count={} capacity={}",
              ADDR_MOUNT_TABLE_PTR, tablePtr, count, capacity);

    if (tablePtr == 0 || count == 0)
    {
        LOG_WARNING("shader_collection: Mount table is EMPTY — no dynamic devices registered");
        return;
    }

    for (uint16_t i = 0; i < count && i < 16; i++)
    {
        uint32_t entryAddr = tablePtr + (i * 276);
        char name[32] = {};
        for (int j = 0; j < 31; j++)
        {
            char c = static_cast<char>(PPC_LOAD_U8(entryAddr + j));
            if (c == 0) break;
            name[j] = c;
        }
        uint16_t nameLen = PPC_LOAD_U16(entryAddr + 264);
        LOGF_INFO("shader_collection: Mount[{}]: '{}' (len={})", i, name, nameLen);
    }
}

// =============================================================================
// sub_82275E38 - Shadow Shader Init (fiCollection Path Push + Shader Setup)
// =============================================================================
// Called once from sub_821D6140 with arg "common:/shaders".
//
// Decompiled flow:
//   1. pushPath(SHADER_COLLECTION, "common:/shaders")  -> pathCount 0->1
//   2. Create effect group via shader factory singleton
//   3. Load 3 shadow effects: "_shadow_Z_Point", "_shadow_Z_Dir", "_shadow_smartblit_"
//   4. Register shader constants: ShadowMatrix, ShadowZTextureDir, etc.
//   5. Register effect params: HemiCubeTexture, ClearfColour, SmartBlitTexture, etc.
//   6. Register technique passes: rewarp, clearf, shadblit, etc.
//   7. popPath(SHADER_COLLECTION)  -> pathCount 1->0
//
// We hook this with diagnostics to verify the path push/pop lifecycle and
// to log the shader factory operations for debugging.
//
PPC_FUNC_IMPL(__imp__sub_82275E38);
PPC_FUNC_HOOK(sub_82275E38)
{
    // Read the shader name being registered (r3 on entry = path string)
    uint32_t pathAddr = ctx.r3.u32;
    char pathStr[64] = {};
    if (pathAddr != 0)
    {
        for (int i = 0; i < 63; i++)
        {
            char c = static_cast<char>(PPC_LOAD_U8(pathAddr + i));
            if (c == 0) break;
            pathStr[i] = c;
        }
    }

    uint32_t pathCountBefore = PPC_LOAD_U32(ADDR_PATH_COUNT);
    LOGF_INFO("shader_collection: sub_82275E38 ENTER path='{}' pathCount={}", pathStr, pathCountBefore);

    __imp__sub_82275E38(ctx, base);

    uint32_t pathCountAfter = PPC_LOAD_U32(ADDR_PATH_COUNT);
    LOGF_INFO("shader_collection: sub_82275E38 EXIT pathCount={}", pathCountAfter);
}

// =============================================================================
// sub_821D6140 - RAGE Shader + PostFX Init
// =============================================================================
// Top-level init function called from sub_821412B8.
// Sequence:
//   1. sub_82523218(800, 1056, 0)    - allocate render targets
//   2. sub_82274BE8()                 - init shadow rendering params
//   3. sub_82275E38("common:/shaders") - shadow shader setup (push/load/pop)
//   4. sub_824F78D8()                 - post-process setup
//   5. sub_82521E38()                 - water shader setup
//   6. sub_82520D10()                 - clouds/sky setup
//   7. sub_824F10E8(dword_82FFDFB0)  - additional postfx
//   8. sub_822D0C78()                 - render pipeline finalize
//   9. sub_822D1788(dword_82D475D0, "update:/shaders", "rage_postfx_e2")
//  10. nullsub_1(debug string)
//  11. sub_821EE680(dword_82B98468)   - scene setup finalize
//
PPC_FUNC_IMPL(__imp__sub_821D6140);
PPC_FUNC_HOOK(sub_821D6140)
{
    LOG_INFO("shader_collection: sub_821D6140 (RAGE shader init) ENTER");

    uint32_t pathCount = PPC_LOAD_U32(ADDR_PATH_COUNT);
    uint32_t totalPaths = PPC_LOAD_U32(ADDR_TOTAL_PATHS);
    LOGF_INFO("shader_collection: fiCollection state: pathCount={} totalPaths={}", pathCount, totalPaths);

    // Dump mount table to see if common:/ is registered
    DumpMountTable(base);

    __imp__sub_821D6140(ctx, base);

    pathCount = PPC_LOAD_U32(ADDR_PATH_COUNT);
    totalPaths = PPC_LOAD_U32(ADDR_TOTAL_PATHS);
    LOGF_INFO("shader_collection: sub_821D6140 EXIT pathCount={} totalPaths={}", pathCount, totalPaths);
}

// =============================================================================
// sub_828CAA60 - Compiled Shader Loader (PATH SAFETY NET)
// =============================================================================
// Opens a fiStream for a shader path resolved via buildPath against the
// fiCollection at 0x82B07278. If called outside the sub_82275E38 push/pop
// window (pathCount=0), the "common:/shaders/" prefix is missing and the
// resolved path is just "shadername.fxc" which fails VFS lookup.
//
// Our hook checks pathCount before calling the original. If pathCount is 0,
// we temporarily push "common:/shaders" to restore the path context, call
// the original, then pop it back. This makes late/deferred shader loads work.
//
// Arguments:
//   r3 = shader factory object (this)
//   r4 = shader name string (e.g. "drawblit")
//   r5 = flags/path index
//
PPC_FUNC_IMPL(__imp__sub_828CAA60);
// Forward-declare the __imp__ originals so we can call pushPath/popPath
// directly without going through our diagnostic hooks (avoids recursion).
PPC_FUNC_IMPL(__imp__sub_8284F310);  // pushPath original
PPC_FUNC_IMPL(__imp__sub_8284E830);  // popPath original
PPC_FUNC_HOOK(sub_828CAA60)
{
    uint32_t pathCount = PPC_LOAD_U32(ADDR_PATH_COUNT);
    bool needsPush = (pathCount == 0);

    if (needsPush)
    {
        // Path stack is empty - push "common:/shaders" so buildPath
        // can resolve relative shader names correctly.
        // Read the base shader path from aTuneShadersLib (set during init).
        uint32_t tuneAddr = ADDR_TUNE_SHADERS_LIB;
        char tunePath[64] = {};
        for (int i = 0; i < 63; i++)
        {
            char c = static_cast<char>(PPC_LOAD_U8(tuneAddr + i));
            if (c == 0) break;
            tunePath[i] = c;
        }

        // Only push if the buffer has content
        if (tunePath[0] != '\0')
        {
            LOGF_INFO("shader_collection: Late shader load with pathCount=0, pushing '{}'", tunePath);

            // Call pushPath(SHADER_COLLECTION, shaderBasePath)
            PPCContext pushCtx{};
            pushCtx.r3.u32 = ADDR_SHADER_COLLECTION;
            pushCtx.r4.u32 = tuneAddr;
            __imp__sub_8284F310(pushCtx, base);
        }
        else
        {
            // aTuneShadersLib not initialized - use hardcoded fallback
            LOG_WARNING("shader_collection: Late shader load but aTuneShadersLib is empty, using hardcoded path");

            PPCContext pushCtx{};
            pushCtx.r3.u32 = ADDR_SHADER_COLLECTION;
            pushCtx.r4.u32 = STR_COMMON_SHADERS;  // "common:/shaders" in .rdata
            __imp__sub_8284F310(pushCtx, base);
        }
    }

    // Log the shader name being loaded
    uint32_t nameAddr = ctx.r4.u32;
    if (nameAddr != 0)
    {
        char name[64] = {};
        for (int i = 0; i < 63; i++)
        {
            char c = static_cast<char>(PPC_LOAD_U8(nameAddr + i));
            if (c == 0) break;
            name[i] = c;
        }
        LOGF_INFO("shader_collection: Loading shader '{}' (pathCount={})",
                   name, PPC_LOAD_U32(ADDR_PATH_COUNT));
    }

    __imp__sub_828CAA60(ctx, base);

    uint32_t result = ctx.r3.u32;

    if (needsPush)
    {
        // Pop the path we pushed
        PPCContext popCtx{};
        popCtx.r3.u32 = ADDR_SHADER_COLLECTION;
        __imp__sub_8284E830(popCtx, base);
    }

    if (result == 1)
    {
        LOG_INFO("shader_collection: Shader load SUCCESS");
    }
    else
    {
        LOG_WARNING("shader_collection: Shader load FAILED");
    }
}


// =============================================================================
// sub_828C8D78 - setShaderBasePath (DIAGNOSTIC HOOK)
// =============================================================================
// Copies the shader base path (e.g. "common:/shaders") into the 128-byte
// static buffer at aTuneShadersLib (0x82B0D348). Called once during
// grmSetup construction (sub_821B3CE8).
//
// We hook this to log the path being set and verify the copy succeeds.
//
PPC_FUNC_IMPL(__imp__sub_828C8D78);
PPC_FUNC_HOOK(sub_828C8D78)
{
    uint32_t pathAddr = ctx.r3.u32;
    char pathStr[64] = {};
    if (pathAddr != 0)
    {
        for (int i = 0; i < 63; i++)
        {
            char c = static_cast<char>(PPC_LOAD_U8(pathAddr + i));
            if (c == 0) break;
            pathStr[i] = c;
        }
    }

    LOGF_INFO("shader_collection: setShaderBasePath('{}') -> aTuneShadersLib @ 0x{:08X}",
              pathStr, ADDR_TUNE_SHADERS_LIB);

    __imp__sub_828C8D78(ctx, base);

    // Also write the path into the fiCollection's prefix slot 3 (offset 0x300).
    // When pathCount=0, sub_8284F0C0 (the path builder) reads from
    // (pathCount+3)<<8 = slot 3 = fiCollection + 0x300.
    // On Xbox 360 this slot was pre-populated by RAGE init. In the recomp,
    // the push/pop cycle in sub_82275E38 writes to slot 4 (offset 0x400)
    // but after popPath, pathCount goes back to 0 and the path builder
    // reads from slot 3 which is empty. This fixes it.
    if (pathAddr != 0)
    {
        constexpr uint32_t FICOLLECTION_BASE = 0x82B07278;

        // Write to BOTH slot 3 (offset 0x300) and slot 4 (offset 0x400).
        // Path builder reads (pathCount+3)<<8:
        //   pathCount=0 → reads slot 3 at 0x300
        //   pathCount=1 → reads slot 4 at 0x400
        // Both must contain the base path for shader loading to work
        // regardless of whether the safety-net push has fired.
        constexpr uint32_t SLOT_OFFSETS[] = { 0x300, 0x400 };
        for (uint32_t slotOff : SLOT_OFFSETS)
        {
            uint32_t dstAddr = FICOLLECTION_BASE + slotOff;
            for (int i = 0; i < 255; i++)
            {
                uint8_t c = PPC_LOAD_U8(pathAddr + i);
                PPC_STORE_U8(dstAddr + i, c);
                if (c == 0) break;
            }
            PPC_STORE_U8(dstAddr + 255, 0);
        }

        LOGF_INFO("shader_collection: Wrote path to fiCollection slots 3+4 (0x{:08X}, 0x{:08X})",
                   FICOLLECTION_BASE + 0x300, FICOLLECTION_BASE + 0x400);
    }

    // Verify the copy at aTuneShadersLib
    char verify[64] = {};
    for (int i = 0; i < 63; i++)
    {
        char c = static_cast<char>(PPC_LOAD_U8(ADDR_TUNE_SHADERS_LIB + i));
        if (c == 0) break;
        verify[i] = c;
    }
    LOGF_INFO("shader_collection: aTuneShadersLib now contains '{}'", verify);
}
