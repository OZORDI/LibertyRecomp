#include <api/Liberty.h>
#include <kernel/memory.h>
#include <os/logger.h>
#include <patches/aspect_ratio_patches.h>
#include <user/config.h>

const char* g_pBlockName{};

void SetMSAALevel(PPCRegister& val)
{
    val.u32 = 0;
}

void BeginBlockGetName(PPCRegister& r3)
{
    g_pBlockName = (const char*)g_memory.Translate(r3.u32);

#if _DEBUG
    if (g_pBlockName)
        LOGFN_UTILITY("Block Begin: {}", g_pBlockName);
#endif
}

// EndBlock
PPC_FUNC_IMPL(__imp__sub_826078D8);
PPC_FUNC_HOOK(sub_826078D8)
{
#if _DEBUG
    if (g_pBlockName)
        LOGFN_UTILITY("Block End: {}", g_pBlockName);
#endif

    g_pBlockName = nullptr;

    __imp__sub_826078D8(ctx, base);
}

float ReflectionScaleFactor(EReflectionResolution ref) {
    switch (ref) {
        case EReflectionResolution::Eighth:
            return 0.5f;
        case EReflectionResolution::Quarter:
            return 1.0f;
        case EReflectionResolution::Half:
            return 2.0f;
        case EReflectionResolution::Full:
            return 4.0f;
        default:
            return 1.0f;
    }
}

// CreateTexture
PPC_FUNC_IMPL(__imp__sub_82619D00);
PPC_FUNC_HOOK(sub_82619D00)
{
    auto pName = (stdx::string*)g_memory.Translate(ctx.r4.u32);

    if (*pName == "radermap")
    {
        ctx.r5.u32 = g_radarMapScale;
        ctx.r6.u32 = g_radarMapScale;
    }

    if (*pName == "reflection0")
    {
        ctx.r5.u32 = static_cast<int>(static_cast<float>(ctx.r5.u32) *
            ReflectionScaleFactor(Config::ReflectionResolution));
        ctx.r6.u32 = static_cast<int>(static_cast<float>(ctx.r6.u32) *
            ReflectionScaleFactor(Config::ReflectionResolution));
    }

    // RenderMefiress
    if (*pName == "user0")
    {
        ctx.r5.u32 = static_cast<int>(Config::ShadowResolution.Value);
        ctx.r6.u32 = static_cast<int>(Config::ShadowResolution.Value);
    }

#if _DEBUG
    auto width = ctx.r5.u32;
    auto height = ctx.r6.u32;
#endif

    __imp__sub_82619D00(ctx, base);

#if _DEBUG
    LOGFN_UTILITY("Created texture: {} ({}x{})", pName->c_str(), width, height);
#endif
}

// CreateDepthStencilSurface
PPC_FUNC_IMPL(__imp__sub_82619B88);
PPC_FUNC_HOOK(sub_82619B88)
{
    auto pName = (stdx::string*)g_memory.Translate(ctx.r4.u32);

    if (g_pBlockName)
    {
        if (strcmp(g_pBlockName, "radermap0") == 0)
        {
            ctx.r5.u32 = g_radarMapScale;
            ctx.r6.u32 = g_radarMapScale;
        }

        // RenderMefiress
        if (strcmp(g_pBlockName, "user0") == 0 && *pName == "depthstencil_256")
        {
            ctx.r5.u32 = static_cast<int>(Config::ShadowResolution.Value);
            ctx.r6.u32 = static_cast<int>(Config::ShadowResolution.Value);
        }
    }

    if (*pName == "depthstencil_1_4")
    {
        ctx.r5.u32 = static_cast<int>(static_cast<float>(ctx.r5.u32) *
            ReflectionScaleFactor(Config::ReflectionResolution));
        ctx.r6.u32 = static_cast<int>(static_cast<float>(ctx.r6.u32) *
            ReflectionScaleFactor(Config::ReflectionResolution));

        // Bad hack to stop EDRAM cache from messing up
        if (Config::ReflectionResolution == EReflectionResolution::Full)
            ctx.r5.u32++;
    }
    
#if _DEBUG
    auto width = ctx.r5.u32;
    auto height = ctx.r6.u32;
#endif

    __imp__sub_82619B88(ctx, base);

#if _DEBUG
    if (g_pBlockName)
    {
        LOGFN_UTILITY("Created texture for {}: {} ({}x{})", g_pBlockName, pName->c_str(), width, height);
    }
    else
    {
        LOGFN_UTILITY("Created texture: {} ({}x{})", pName->c_str(), width, height);
    }
#endif
}

float ShadowScaleFactor(EShadowResolution ref) {
    // Returns scale factor relative to the original Xbox 360 shadow base of 256 pixels.
    // user0 hook sets the shadow RT to Config::ShadowResolution.Value pixels directly,
    // so this multiplier should match that ratio (value/256) to keep CSM tiles consistent.
    //   x512  → 512/256 = 2×  original
    //   x1024 → 4×  original
    //   x2048 → 8×  original
    //   x4096 → 16× original (default)
    //   x8192 → 32× original
    constexpr float kOriginalBase = 256.0f;
    int32_t resValue = static_cast<int32_t>(ref);
    if (resValue > 0) {
        return static_cast<float>(resValue) / kOriginalBase;
    }
    return 1.0f;  // EShadowResolution::Original — keep native size
}

// CreateArrayTexture
PPC_FUNC_IMPL(__imp__sub_82619FF0);
PPC_FUNC_HOOK(sub_82619FF0)
{
    auto pName = (stdx::string*)g_memory.Translate(ctx.r4.u32);

    if (*pName == "csm")
    {
        ctx.r5.u32 = static_cast<int>(static_cast<float>(ctx.r5.u32) *
            ShadowScaleFactor(Config::ShadowResolution));
        ctx.r6.u32 = static_cast<int>(static_cast<float>(ctx.r6.u32) *
            ShadowScaleFactor(Config::ShadowResolution));
    }

    __imp__sub_82619FF0(ctx, base);
}

std::string g_renderWorldFBO;

void GetRenderWorldFBO(PPCRegister& name)
{
    auto pName = xpointer(reinterpret_cast<char*>(name.u32));
    g_renderWorldFBO = std::string(pName.get());
}

void FurtherObjectShadows(PPCRegister& scope)
{
    if (g_renderWorldFBO != "shadowmap")
        return;

    scope.u32 = 1;
}

bool DisableRadialBlur()
{
    return Config::RadialBlur == ERadialBlur::Off;
}

bool DisableKingdomValleyMist()
{
    return Config::DisableKingdomValleyMist;
}
