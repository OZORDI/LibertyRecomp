#include "postprocess_aa.h"
#include "postprocess_renderer.h"
#include "user/config.h"
#include "os/logger.h"
#include <shader/postprocess_cache.h>
#include <cmath>
#include <algorithm>

namespace PostProcess {

AAState g_aaState;

// Lookup a post-process shader by hash
static PostProcessShaderEntry* FindPostProcessShader(uint64_t hash)
{
    for (size_t i = 0; i < g_postprocessShaderCount; ++i)
    {
        if (g_postprocessShaderEntries[i].hash == hash)
            return &g_postprocessShaderEntries[i];
    }
    return nullptr;
}

// Check if post-process shaders are available (accessible from IsModeAvailable)
bool g_postprocessShadersAvailable = false;

static void ValidatePostProcessShaders()
{
    if (g_postprocessShaderCount == 0)
    {
        LOGF_WARNING("[PostProcess] No post-process shaders in cache - AA disabled (count={})", g_postprocessShaderCount);
        g_postprocessShadersAvailable = false;
        return;
    }
    
    // Verify all required shaders are present
    bool hasFullscreenVS = FindPostProcessShader(ShaderHash::FullscreenVS) != nullptr;
    bool hasTAA = FindPostProcessShader(ShaderHash::TAAPS) != nullptr;
    bool hasSMAAEdge = FindPostProcessShader(ShaderHash::SMAAEdgeDetectPS) != nullptr;
    bool hasSMAABlend = FindPostProcessShader(ShaderHash::SMAABlendPS) != nullptr;
    bool hasFSR1EASU = FindPostProcessShader(ShaderHash::FSR1EASUPS) != nullptr;
    bool hasFSR1RCAS = FindPostProcessShader(ShaderHash::FSR1RCASPS) != nullptr;
    
    g_postprocessShadersAvailable = hasFullscreenVS && hasTAA && hasSMAAEdge && 
                                     hasSMAABlend && hasFSR1EASU && hasFSR1RCAS;
    
    LOGF_INFO("[PostProcess] Shader validation: FullscreenVS={} TAA={} SMAA={}/{} FSR1={}/{}",
              hasFullscreenVS, hasTAA, hasSMAAEdge, hasSMAABlend, hasFSR1EASU, hasFSR1RCAS);
    LOGF_INFO("[PostProcess] Post-process shaders available: {}", g_postprocessShadersAvailable);
}

// Halton sequence for TAA jitter
static float HaltonSequence(int index, int base)
{
    float result = 0.0f;
    float f = 1.0f / base;
    int i = index;
    while (i > 0)
    {
        result += f * (i % base);
        i = i / base;
        f /= base;
    }
    return result;
}

void InitializeAA()
{
    g_aaState = AAState();
    g_aaState.currentMode = Config::ModernAA;
    g_aaState.currentResolutionScale = Config::ResolutionScale;
    g_aaState.targetFrameTime = Config::TargetFrameTime;
    g_aaState.initialized = true;
    
    // Validate post-process shaders are available
    ValidatePostProcessShaders();
    
    // Check if selected mode is actually available
    if (g_aaState.currentMode != EModernAA::Off && !IsModeAvailable(g_aaState.currentMode))
    {
        LOGF_WARNING("[PostProcess] AA mode {} is not available: {}",
                     static_cast<int>(g_aaState.currentMode),
                     GetModeStatusString(g_aaState.currentMode));
    }
    
    LOGF_INFO("[PostProcess] AA initialized: mode={} available={} resScale={:.2f} targetFrameTime={:.2f}ms",
              static_cast<int>(g_aaState.currentMode),
              IsModeAvailable(g_aaState.currentMode),
              g_aaState.currentResolutionScale,
              g_aaState.targetFrameTime);
}

void UpdateAA(float deltaTime)
{
    if (!g_aaState.initialized)
        return;
    
    // Update mode if config changed
    if (g_aaState.currentMode != Config::ModernAA)
    {
        g_aaState.currentMode = Config::ModernAA;
        LOGF_INFO("[PostProcess] AA mode changed to {}", static_cast<int>(g_aaState.currentMode));
    }
    
    // TAA jitter update
    if (g_aaState.currentMode == EModernAA::TAA)
    {
        g_aaState.prevJitterX = g_aaState.jitterX;
        g_aaState.prevJitterY = g_aaState.jitterY;
        
        // 8-sample Halton sequence for temporal jitter
        int sampleIndex = g_aaState.frameIndex % 8;
        g_aaState.jitterX = HaltonSequence(sampleIndex + 1, 2) - 0.5f;
        g_aaState.jitterY = HaltonSequence(sampleIndex + 1, 3) - 0.5f;
        
        g_aaState.frameIndex++;
    }
    else
    {
        g_aaState.jitterX = 0.0f;
        g_aaState.jitterY = 0.0f;
        g_aaState.prevJitterX = 0.0f;
        g_aaState.prevJitterY = 0.0f;
    }
    
    // Dynamic resolution scaling update
    if (Config::DynamicResolution != EDynamicResolution::Off)
    {
        g_aaState.lastFrameTime = deltaTime * 1000.0f; // Convert to ms
        
        float targetUtilization = 0.75f; // Default balanced
        switch (Config::DynamicResolution.Value)
        {
            case EDynamicResolution::Quality:     targetUtilization = 0.90f; break;
            case EDynamicResolution::Balanced:    targetUtilization = 0.75f; break;
            case EDynamicResolution::Performance: targetUtilization = 0.60f; break;
            default: break;
        }
        
        float currentUtilization = g_aaState.lastFrameTime / g_aaState.targetFrameTime;
        
        // Smooth adjustment
        float scaleDelta = 0.0f;
        if (currentUtilization > targetUtilization + 0.05f)
        {
            // Running slow, decrease resolution
            scaleDelta = -0.02f;
        }
        else if (currentUtilization < targetUtilization - 0.05f)
        {
            // Running fast, increase resolution
            scaleDelta = 0.01f;
        }
        
        g_aaState.currentResolutionScale = std::clamp(
            g_aaState.currentResolutionScale + scaleDelta,
            Config::MinResolutionScale.Value,
            1.0f
        );
    }
    else
    {
        g_aaState.currentResolutionScale = Config::ResolutionScale;
    }
}

// Apply AA using the PostProcessRenderer
// This function is called from Video::Present with the current frame's resources
bool ApplyAA(RenderCommandList* commandList, 
             RenderTexture* colorTexture,
             RenderTexture* depthTexture,
             RenderTexture* motionTexture,
             RenderTexture* outputTexture,
             uint32_t inputWidth, uint32_t inputHeight,
             uint32_t outputWidth, uint32_t outputHeight)
{
    if (!g_aaState.initialized || g_aaState.currentMode == EModernAA::Off)
        return false;
    
    if (!g_postProcessRenderer.IsInitialized())
    {
        static bool s_loggedOnce = false;
        if (!s_loggedOnce)
        {
            LOG_WARNING("[PostProcess] PostProcessRenderer not initialized - AA disabled");
            s_loggedOnce = true;
        }
        return false;
    }
    
    // Check if the selected mode is actually available
    if (!IsModeAvailable(g_aaState.currentMode))
    {
        return false;
    }
    
    bool result = false;
    
    switch (g_aaState.currentMode)
    {
        case EModernAA::TAA:
        {
            // TAA requires motion vectors
            if (!motionTexture)
            {
                static bool s_loggedOnce = false;
                if (!s_loggedOnce)
                {
                    LOG_WARNING("[PostProcess] TAA requires motion vectors - falling back to passthrough");
                    s_loggedOnce = true;
                }
                return false;
            }
            
            // Determine if we need to reset history (on first frame or after resolution change)
            bool resetHistory = (g_aaState.frameIndex == 0);
            
            result = g_postProcessRenderer.ApplyTAA(
                commandList,
                colorTexture,
                depthTexture,
                motionTexture,
                outputTexture,
                g_aaState.jitterX,
                g_aaState.jitterY,
                g_aaState.prevJitterX,
                g_aaState.prevJitterY,
                resetHistory
            );
            
            if (result)
            {
                // Swap history buffers for next frame
                g_postProcessRenderer.SwapTAAHistory();
            }
            break;
        }
            
        case EModernAA::SMAA:
        {
            result = g_postProcessRenderer.ApplySMAA(
                commandList,
                colorTexture,
                outputTexture
            );
            break;
        }
            
        case EModernAA::FSR1:
        {
            // FSR 1.0 is used as an AA technique when input == output resolution
            // It provides edge-adaptive sharpening which reduces aliasing
            float sharpness = Config::UpscaleSharpness;
            
            result = g_postProcessRenderer.ApplyFSR1(
                commandList,
                colorTexture,
                outputTexture,
                inputWidth,
                inputHeight,
                outputWidth,
                outputHeight,
                sharpness
            );
            break;
        }
            
        default:
            break;
    }
    
    return result;
}

// Legacy overload for backwards compatibility - returns false (requires full parameters)
bool ApplyAA()
{
    // This overload cannot work without the required parameters
    // It exists for backwards compatibility and will always return false
    static bool s_loggedOnce = false;
    if (!s_loggedOnce && g_aaState.currentMode != EModernAA::Off)
    {
        LOG_WARNING("[PostProcess] ApplyAA() called without required parameters - use the full overload");
        s_loggedOnce = true;
    }
    return false;
}

void GetTAAJitter(float& outJitterX, float& outJitterY)
{
    outJitterX = g_aaState.jitterX;
    outJitterY = g_aaState.jitterY;
}

float GetResolutionScale()
{
    return g_aaState.currentResolutionScale;
}

float GetSSAAMultiplier()
{
    switch (Config::SSAA)
    {
        case ESSAA::x1_5: return 1.5f;
        case ESSAA::x2:   return 2.0f;
        case ESSAA::x4:   return 4.0f;
        default:          return 1.0f;
    }
}

bool IsModeAvailable(EModernAA mode)
{
    switch (mode)
    {
        case EModernAA::Off:
            return true;  // Always available
            
        case EModernAA::TAA:
            // TAA available if PostProcessRenderer is initialized with shaders
            return g_postProcessRenderer.IsInitialized() && g_postprocessShadersAvailable;
            
        case EModernAA::SMAA:
            // SMAA available if PostProcessRenderer is initialized with LUT textures
            return g_postProcessRenderer.IsInitialized() && g_postprocessShadersAvailable;
            
        case EModernAA::FSR1:
            // FSR 1.0 available if PostProcessRenderer is initialized
            return g_postProcessRenderer.IsInitialized() && g_postprocessShadersAvailable;
            
        default:
            return false;
    }
}

const char* GetModeStatusString(EModernAA mode)
{
    bool rendererReady = g_postProcessRenderer.IsInitialized();
    bool shadersReady = g_postprocessShadersAvailable;
    
    switch (mode)
    {
        case EModernAA::Off:
            return "Available";
            
        case EModernAA::TAA:
            if (rendererReady && shadersReady)
                return "Available (temporal anti-aliasing with motion vectors)";
            else if (!rendererReady)
                return "Pending (PostProcessRenderer not initialized)";
            else
                return "Unavailable (shaders not loaded)";
            
        case EModernAA::SMAA:
            if (rendererReady && shadersReady)
                return "Available (subpixel morphological AA)";
            else if (!rendererReady)
                return "Pending (PostProcessRenderer not initialized)";
            else
                return "Unavailable (shaders not loaded)";
            
        case EModernAA::FSR1:
            if (rendererReady && shadersReady)
                return "Available (FSR 1.0 EASU + RCAS)";
            else if (!rendererReady)
                return "Pending (PostProcessRenderer not initialized)";
            else
                return "Unavailable (shaders not loaded)";
            
        default:
            return "Unknown";
    }
}

void ShutdownAA()
{
    g_aaState.initialized = false;
    LOGF_INFO("[PostProcess] AA shutdown (mode={})", static_cast<int>(g_aaState.currentMode));
}

} // namespace PostProcess
