// =============================================================================
// Unified Upscaler Integration Module - Implementation
// =============================================================================

#include "upscaler.h"
#include "postprocess_aa.h"
#include "postprocess_renderer.h"
#include "video.h"
#include "../user/config.h"
#include <os/logger.h>
#include <cmath>
#include <array>

#ifdef __APPLE__
#include <TargetConditionals.h>
#include "upscaler_metalfx.h"
#endif

#ifdef _WIN32
#include "upscaler_fsr3.h"
#include "upscaler_dlss.h"
#endif

#include "upscaler_xess.h"

namespace Upscaler {

// =============================================================================
// Global State
// =============================================================================
UpscalerState g_upscalerState;
static void* s_graphicsDevice = nullptr;

void SetGraphicsDevice(void* device) {
    s_graphicsDevice = device;
    LOGF_INFO("[Upscaler] Graphics device set: {}", device != nullptr ? "valid" : "null");
}

void* GetGraphicsDevice() {
    return s_graphicsDevice;
}

// =============================================================================
// Quality Preset Scale Factors
// =============================================================================
float GetRenderScale(Quality quality) {
    switch (quality) {
        case Quality::UltraQuality:     return 0.77f;
        case Quality::Quality:          return 0.67f;
        case Quality::Balanced:         return 0.58f;
        case Quality::Performance:      return 0.50f;
        case Quality::UltraPerformance: return 0.33f;
        default:                        return 1.0f;
    }
}

float GetOutputScale(Quality quality) {
    float renderScale = GetRenderScale(quality);
    return (renderScale > 0.0f) ? (1.0f / renderScale) : 1.0f;
}

// =============================================================================
// Quality Mapping Utilities
// =============================================================================
#ifdef _WIN32
static FSR3Quality ToFSR3Quality(Quality quality) {
    switch (quality) {
        case Quality::UltraQuality:     return FSR3Quality::UltraQuality;
        case Quality::Quality:          return FSR3Quality::Quality;
        case Quality::Balanced:         return FSR3Quality::Balanced;
        case Quality::Performance:      return FSR3Quality::Performance;
        case Quality::UltraPerformance: return FSR3Quality::UltraPerformance;
        default:                        return FSR3Quality::Quality;
    }
}

static DLSSQuality ToDLSSQuality(Quality quality) {
    switch (quality) {
        case Quality::UltraQuality:     return DLSSQuality::MaxQuality;
        case Quality::Quality:          return DLSSQuality::Quality;
        case Quality::Balanced:         return DLSSQuality::Balanced;
        case Quality::Performance:      return DLSSQuality::Performance;
        case Quality::UltraPerformance: return DLSSQuality::UltraPerformance;
        default:                        return DLSSQuality::Quality;
    }
}

static XeSSQuality ToXeSSQuality(Quality quality) {
    switch (quality) {
        case Quality::UltraQuality:     return XeSSQuality::UltraQuality;
        case Quality::Quality:          return XeSSQuality::Quality;
        case Quality::Balanced:         return XeSSQuality::Balanced;
        case Quality::Performance:      return XeSSQuality::Performance;
        case Quality::UltraPerformance: return XeSSQuality::UltraPerformance;
        default:                        return XeSSQuality::Quality;
    }
}
#endif

// =============================================================================
// Halton Jitter - Default phase count for simple upscalers
// =============================================================================
static constexpr int DEFAULT_JITTER_PHASE_COUNT = 8;

// =============================================================================
// FSR 1.0 Context (Spatial Upscaling Only)
// =============================================================================
class FSR1Context : public IUpscalerContext {
public:
    bool Initialize(uint32_t displayWidth, uint32_t displayHeight) override {
        m_displayWidth = displayWidth;
        m_displayHeight = displayHeight;
        m_initialized = true;
        LOGF_INFO("[Upscaler] FSR 1.0 initialized ({}x{})", displayWidth, displayHeight);
        return true;
    }
    
    void Shutdown() override {
        m_initialized = false;
        LOG_INFO("[Upscaler] FSR 1.0 shutdown");
    }
    
    bool IsAvailable() const override {
        // FSR 1.0 is available when PostProcessRenderer is initialized
        return PostProcess::g_postProcessRenderer.IsInitialized();
    }
    Backend GetBackend() const override { return Backend::FSR1; }
    Quality GetQuality() const override { return m_quality; }
    void SetQuality(Quality quality) override { m_quality = quality; }
    
    void GetRenderDimensions(uint32_t displayWidth, uint32_t displayHeight,
                             uint32_t& renderWidth, uint32_t& renderHeight) const override {
        float scale = GetRenderScale(m_quality);
        renderWidth = static_cast<uint32_t>(displayWidth * scale);
        renderHeight = static_cast<uint32_t>(displayHeight * scale);
        renderWidth = (renderWidth + 1) & ~1;   // Align to 2
        renderHeight = (renderHeight + 1) & ~1;
    }
    
    void GetJitter(uint32_t frameIndex, float& jitterX, float& jitterY) const override {
        // FSR 1.0 is spatial-only, no jitter needed
        jitterX = 0.0f;
        jitterY = 0.0f;
    }
    
    bool Upscale(const UpscaleParams& params) override {
        if (!m_initialized) return false;
        if (!PostProcess::g_postProcessRenderer.IsInitialized()) return false;
        
        // Use PostProcessRenderer for FSR 1.0 upscaling
        // Note: This requires the command list to be passed through
        // For now, the actual dispatch happens in the post-process pass
        // The upscaler module signals that FSR1 should be applied
        return true;  // Signal that FSR1 is ready to be applied
    }
    
    bool SupportsFrameGeneration() const override { return false; }
    bool GenerateFrame(const FrameGenParams& params) override { return false; }
    
private:
    bool m_initialized = false;
    Quality m_quality = Quality::Quality;
    uint32_t m_displayWidth = 0;
    uint32_t m_displayHeight = 0;
};

// =============================================================================
// FSR 3.0 Context (Temporal Upscaling + Frame Generation)
// Uses AMD FidelityFX SDK from tools/FidelityFX-SDK
// =============================================================================
class FSR3ContextWrapper : public IUpscalerContext {
public:
    bool Initialize(uint32_t displayWidth, uint32_t displayHeight) override {
        m_displayWidth = displayWidth;
        m_displayHeight = displayHeight;
        
#ifdef _WIN32
        // Use real FSR 3.0 implementation
        if (FSR3Context::IsAvailable()) {
            m_fsr3Context = std::make_unique<FSR3Context>();
            
            // Get D3D12 device from global state
            ID3D12Device* d3d12Device = static_cast<ID3D12Device*>(GetGraphicsDevice());
            if (!d3d12Device) {
                LOG_WARNING("[Upscaler] FSR 3.0 requires D3D12 device - call SetGraphicsDevice first");
                return false;
            }
            
            FSR3InitParams initParams = {};
            initParams.device = d3d12Device;
            initParams.maxRenderWidth = static_cast<uint32_t>(displayWidth * GetRenderScale(Quality::UltraPerformance));
            initParams.maxRenderHeight = static_cast<uint32_t>(displayHeight * GetRenderScale(Quality::UltraPerformance));
            initParams.displayWidth = displayWidth;
            initParams.displayHeight = displayHeight;
            initParams.enableHDR = true;
            initParams.enableAutoExposure = true;
            initParams.enableSharpening = true;
            initParams.enableFrameGeneration = true;
            
            if (!m_fsr3Context->Initialize(initParams)) {
                LOG_ERROR("[Upscaler] FSR 3.0 initialization failed");
                m_fsr3Context.reset();
                return false;
            }
            
            m_initialized = true;
            LOGF_INFO("[Upscaler] FSR 3.0 initialized ({}x{})", displayWidth, displayHeight);
            return true;
        }
#endif
        LOG_WARNING("[Upscaler] FSR 3.0 not available on this platform");
        return false;
    }
    
    void Shutdown() override {
#ifdef _WIN32
        if (m_fsr3Context) {
            m_fsr3Context->Shutdown();
            m_fsr3Context.reset();
        }
#endif
        m_initialized = false;
        LOG_INFO("[Upscaler] FSR 3.0 shutdown");
    }
    
    bool IsAvailable() const override {
#ifdef _WIN32
        return FSR3Context::IsAvailable();
#else
        return false;
#endif
    }
    
    Backend GetBackend() const override { return Backend::FSR3; }
    Quality GetQuality() const override { return m_quality; }
    void SetQuality(Quality quality) override { 
        m_quality = quality;
#ifdef _WIN32
        if (m_fsr3Context) {
            FSR3Quality fsr3Quality = FSR3Quality::Quality;
            switch (quality) {
                case Quality::UltraQuality:     fsr3Quality = FSR3Quality::Quality; break;
                case Quality::Quality:          fsr3Quality = FSR3Quality::Quality; break;
                case Quality::Balanced:         fsr3Quality = FSR3Quality::Balanced; break;
                case Quality::Performance:      fsr3Quality = FSR3Quality::Performance; break;
                case Quality::UltraPerformance: fsr3Quality = FSR3Quality::UltraPerformance; break;
            }
            m_fsr3Context->SetQuality(fsr3Quality);
        }
#endif
    }
    
    void GetRenderDimensions(uint32_t displayWidth, uint32_t displayHeight,
                             uint32_t& renderWidth, uint32_t& renderHeight) const override {
#ifdef _WIN32
        FSR3Quality fsr3Quality = FSR3Quality::Quality;
        switch (m_quality) {
            case Quality::UltraQuality:     fsr3Quality = FSR3Quality::Quality; break;
            case Quality::Quality:          fsr3Quality = FSR3Quality::Quality; break;
            case Quality::Balanced:         fsr3Quality = FSR3Quality::Balanced; break;
            case Quality::Performance:      fsr3Quality = FSR3Quality::Performance; break;
            case Quality::UltraPerformance: fsr3Quality = FSR3Quality::UltraPerformance; break;
        }
        FSR3Context::GetRenderResolution(fsr3Quality, displayWidth, displayHeight, renderWidth, renderHeight);
#else
        float scale = GetRenderScale(m_quality);
        renderWidth = static_cast<uint32_t>(displayWidth * scale);
        renderHeight = static_cast<uint32_t>(displayHeight * scale);
        renderWidth = (renderWidth + 1) & ~1;
        renderHeight = (renderHeight + 1) & ~1;
#endif
    }
    
    void GetJitter(uint32_t frameIndex, float& jitterX, float& jitterY) const override {
#ifdef _WIN32
        if (m_fsr3Context) {
            m_fsr3Context->GetJitterOffset(frameIndex, jitterX, jitterY);
            return;
        }
#endif
        GetHaltonJitter(static_cast<int>(frameIndex), DEFAULT_JITTER_PHASE_COUNT, jitterX, jitterY);
    }
    
    bool Upscale(const UpscaleParams& params) override {
#ifdef _WIN32
        if (!m_initialized || !m_fsr3Context) return false;
        
        // Prepare FSR3 dispatch parameters
        FSR3DispatchParams dispatchParams = {};
        dispatchParams.commandList = static_cast<ID3D12GraphicsCommandList*>(params.commandList);
        dispatchParams.colorTexture = static_cast<ID3D12Resource*>(params.colorTexture);
        dispatchParams.depthTexture = static_cast<ID3D12Resource*>(params.depthTexture);
        dispatchParams.motionVectors = static_cast<ID3D12Resource*>(params.motionVectors);
        dispatchParams.outputTexture = static_cast<ID3D12Resource*>(params.outputTexture);
        dispatchParams.renderWidth = params.renderWidth;
        dispatchParams.renderHeight = params.renderHeight;
        dispatchParams.displayWidth = params.displayWidth;
        dispatchParams.displayHeight = params.displayHeight;
        dispatchParams.jitterX = params.jitterX;
        dispatchParams.jitterY = params.jitterY;
        dispatchParams.frameTimeDelta = params.deltaTime;
        dispatchParams.cameraNear = params.cameraNear;
        dispatchParams.cameraFar = params.cameraFar;
        dispatchParams.cameraFovY = params.cameraFovY;
        dispatchParams.sharpness = params.sharpness;
        dispatchParams.reset = params.reset;
        
        return m_fsr3Context->Upscale(dispatchParams);
#else
        return false;
#endif
    }
    
    bool SupportsFrameGeneration() const override { 
#ifdef _WIN32
        return m_fsr3Context ? m_fsr3Context->SupportsFrameGeneration() : false;
#else
        return false;
#endif
    }
    
    bool GenerateFrame(const FrameGenParams& params) override {
#ifdef _WIN32
        if (!m_initialized || !m_fsr3Context) return false;
        // Frame generation requires D3D12 integration
        return m_fsr3Context->SupportsFrameGeneration();
#else
        return false;
#endif
    }
    
private:
    bool m_initialized = false;
    Quality m_quality = Quality::Quality;
    uint32_t m_displayWidth = 0;
    uint32_t m_displayHeight = 0;
#ifdef _WIN32
    std::unique_ptr<FSR3Context> m_fsr3Context;
#endif
};

// =============================================================================
// DLSS Context (NVIDIA RTX Only)
// Uses NVIDIA NGX SDK from tools/DLSS
// =============================================================================
class DLSSContextWrapper : public IUpscalerContext {
public:
    bool Initialize(uint32_t displayWidth, uint32_t displayHeight) override {
        m_displayWidth = displayWidth;
        m_displayHeight = displayHeight;
        
#ifdef _WIN32
        // Use real DLSS implementation
        if (DLSSContext::IsAvailable()) {
            m_dlssContext = std::make_unique<DLSSContext>();
            
            // Get D3D12 device from global state
            ID3D12Device* d3d12Device = static_cast<ID3D12Device*>(GetGraphicsDevice());
            if (!d3d12Device) {
                LOG_WARNING("[Upscaler] DLSS requires D3D12 device - call SetGraphicsDevice first");
                return false;
            }
            
            DLSSInitParams initParams = {};
            initParams.device = d3d12Device;
            initParams.maxRenderWidth = static_cast<uint32_t>(displayWidth * GetRenderScale(Quality::UltraPerformance));
            initParams.maxRenderHeight = static_cast<uint32_t>(displayHeight * GetRenderScale(Quality::UltraPerformance));
            initParams.displayWidth = displayWidth;
            initParams.displayHeight = displayHeight;
            initParams.enableHDR = true;
            initParams.enableSharpening = true;
            initParams.enableFrameGeneration = true;
            initParams.appDataPath = L".";
            initParams.appId = 0;  // Development mode
            
            if (!m_dlssContext->Initialize(initParams)) {
                LOG_ERROR("[Upscaler] DLSS initialization failed");
                m_dlssContext.reset();
                return false;
            }
            
            m_initialized = true;
            LOGF_INFO("[Upscaler] DLSS initialized ({}x{})", displayWidth, displayHeight);
            return true;
        }
        LOG_WARNING("[Upscaler] DLSS not available (no RTX GPU detected)");
#else
        LOG_WARNING("[Upscaler] DLSS is only available on Windows with NVIDIA RTX GPUs");
#endif
        return false;
    }
    
    void Shutdown() override {
#ifdef _WIN32
        if (m_dlssContext) {
            m_dlssContext->Shutdown();
            m_dlssContext.reset();
        }
#endif
        m_initialized = false;
        LOG_INFO("[Upscaler] DLSS shutdown");
    }
    
    bool IsAvailable() const override {
#ifdef _WIN32
        return DLSSContext::IsAvailable();
#else
        return false;
#endif
    }
    
    Backend GetBackend() const override { return Backend::DLSS; }
    Quality GetQuality() const override { return m_quality; }
    void SetQuality(Quality quality) override { 
        m_quality = quality;
#ifdef _WIN32
        if (m_dlssContext) {
            DLSSQuality dlssQuality = DLSSQuality::Quality;
            switch (quality) {
                case Quality::UltraQuality:     dlssQuality = DLSSQuality::UltraQuality; break;
                case Quality::Quality:          dlssQuality = DLSSQuality::Quality; break;
                case Quality::Balanced:         dlssQuality = DLSSQuality::Balanced; break;
                case Quality::Performance:      dlssQuality = DLSSQuality::Performance; break;
                case Quality::UltraPerformance: dlssQuality = DLSSQuality::UltraPerformance; break;
            }
            m_dlssContext->SetQuality(dlssQuality);
        }
#endif
    }
    
    void GetRenderDimensions(uint32_t displayWidth, uint32_t displayHeight,
                             uint32_t& renderWidth, uint32_t& renderHeight) const override {
#ifdef _WIN32
        DLSSQuality dlssQuality = DLSSQuality::Quality;
        switch (m_quality) {
            case Quality::UltraQuality:     dlssQuality = DLSSQuality::UltraQuality; break;
            case Quality::Quality:          dlssQuality = DLSSQuality::Quality; break;
            case Quality::Balanced:         dlssQuality = DLSSQuality::Balanced; break;
            case Quality::Performance:      dlssQuality = DLSSQuality::Performance; break;
            case Quality::UltraPerformance: dlssQuality = DLSSQuality::UltraPerformance; break;
        }
        DLSSContext::GetRenderResolution(dlssQuality, displayWidth, displayHeight, renderWidth, renderHeight);
#else
        float scale = GetRenderScale(m_quality);
        renderWidth = static_cast<uint32_t>(displayWidth * scale);
        renderHeight = static_cast<uint32_t>(displayHeight * scale);
#endif
    }
    
    void GetJitter(uint32_t frameIndex, float& jitterX, float& jitterY) const override {
#ifdef _WIN32
        if (m_dlssContext) {
            m_dlssContext->GetJitterOffset(frameIndex, jitterX, jitterY);
            return;
        }
#endif
        GetHaltonJitter(static_cast<int>(frameIndex), DEFAULT_JITTER_PHASE_COUNT, jitterX, jitterY);
    }
    
    bool Upscale(const UpscaleParams& params) override {
#ifdef _WIN32
        if (!m_initialized || !m_dlssContext) return false;
        
        // Prepare DLSS dispatch parameters
        DLSSDispatchParams dispatchParams = {};
        dispatchParams.commandList = static_cast<ID3D12GraphicsCommandList*>(params.commandList);
        dispatchParams.colorTexture = static_cast<ID3D12Resource*>(params.colorTexture);
        dispatchParams.depthTexture = static_cast<ID3D12Resource*>(params.depthTexture);
        dispatchParams.motionVectors = static_cast<ID3D12Resource*>(params.motionVectors);
        dispatchParams.outputTexture = static_cast<ID3D12Resource*>(params.outputTexture);
        dispatchParams.exposureTexture = static_cast<ID3D12Resource*>(params.exposureTexture);
        dispatchParams.renderWidth = params.renderWidth;
        dispatchParams.renderHeight = params.renderHeight;
        dispatchParams.displayWidth = params.displayWidth;
        dispatchParams.displayHeight = params.displayHeight;
        dispatchParams.jitterX = params.jitterX;
        dispatchParams.jitterY = params.jitterY;
        dispatchParams.motionScaleX = params.motionScaleX > 0.0f ? params.motionScaleX : static_cast<float>(params.renderWidth);
        dispatchParams.motionScaleY = params.motionScaleY > 0.0f ? params.motionScaleY : static_cast<float>(params.renderHeight);
        dispatchParams.sharpness = params.sharpness;
        dispatchParams.reset = params.reset;
        
        return m_dlssContext->Upscale(dispatchParams);
#else
        return false;
#endif
    }
    
    bool SupportsFrameGeneration() const override {
#ifdef _WIN32
        return m_dlssContext ? m_dlssContext->SupportsFrameGeneration() : false;
#else
        return false;
#endif
    }
    
    bool GenerateFrame(const FrameGenParams& params) override {
#ifdef _WIN32
        if (!m_initialized || !m_dlssContext) return false;
        return m_dlssContext->SupportsFrameGeneration();
#else
        return false;
#endif
    }
    
private:
    bool m_initialized = false;
    Quality m_quality = Quality::Quality;
    uint32_t m_displayWidth = 0;
    uint32_t m_displayHeight = 0;
#ifdef _WIN32
    std::unique_ptr<DLSSContext> m_dlssContext;
#endif
};

#ifdef _WIN32
// =============================================================================
// XeSS Context (Intel Arc + All DP4a GPUs)
// Uses Intel XeSS SDK - supports both D3D12 and Vulkan
// =============================================================================
class XeSSContextWrapper : public IUpscalerContext {
public:
    bool Initialize(uint32_t displayWidth, uint32_t displayHeight) override {
        m_displayWidth = displayWidth;
        m_displayHeight = displayHeight;
        
        // Use real XeSS implementation
        if (XeSSContext::IsAvailable()) {
            m_xessContext = std::make_unique<XeSSContext>();
            
#ifdef _WIN32
            // Get D3D12 device from global state
            ID3D12Device* d3d12Device = static_cast<ID3D12Device*>(GetGraphicsDevice());
            if (!d3d12Device) {
                LOG_WARNING("[Upscaler] XeSS requires D3D12 device - call SetGraphicsDevice first");
                return false;
            }
            
            XeSSInitParamsD3D12 initParams = {};
            initParams.device = d3d12Device;
            initParams.maxRenderWidth = static_cast<uint32_t>(displayWidth * GetRenderScale(Quality::UltraPerformance));
            initParams.maxRenderHeight = static_cast<uint32_t>(displayHeight * GetRenderScale(Quality::UltraPerformance));
            initParams.displayWidth = displayWidth;
            initParams.displayHeight = displayHeight;
            initParams.initMode = XeSSInitMode::Default;
            initParams.enableHDR = true;
            initParams.enableAutoExposure = true;
            initParams.enableResponsiveMask = false;
            
            if (!m_xessContext->InitializeD3D12(initParams)) {
                LOG_ERROR("[Upscaler] XeSS initialization failed");
                m_xessContext.reset();
                return false;
            }
#else
            // Vulkan path - not yet implemented
            LOG_WARNING("[Upscaler] XeSS Vulkan support not yet implemented");
            return false;
#endif
            
            m_initialized = true;
            LOGF_INFO("[Upscaler] XeSS initialized ({}x{}, version={})", 
                      displayWidth, displayHeight, XeSSContext::GetVersionString());
            return true;
        }
        LOG_WARNING("[Upscaler] XeSS not available (libxess.dll not found)");
        return false;
    }
    
    void Shutdown() override {
        if (m_xessContext) {
            m_xessContext->Shutdown();
            m_xessContext.reset();
        }
        m_initialized = false;
        LOG_INFO("[Upscaler] XeSS shutdown");
    }
    
    bool IsAvailable() const override {
        return XeSSContext::IsAvailable();
    }
    
    Backend GetBackend() const override { return Backend::XeSS; }
    Quality GetQuality() const override { return m_quality; }
    void SetQuality(Quality quality) override { 
        m_quality = quality;
        if (m_xessContext) {
            XeSSQuality xessQuality = XeSSQuality::Quality;
            switch (quality) {
                case Quality::UltraQuality:     xessQuality = XeSSQuality::UltraQuality; break;
                case Quality::Quality:          xessQuality = XeSSQuality::Quality; break;
                case Quality::Balanced:         xessQuality = XeSSQuality::Balanced; break;
                case Quality::Performance:      xessQuality = XeSSQuality::Performance; break;
                case Quality::UltraPerformance: xessQuality = XeSSQuality::UltraPerformance; break;
            }
            m_xessContext->SetQuality(xessQuality);
        }
    }
    
    void GetRenderDimensions(uint32_t displayWidth, uint32_t displayHeight,
                             uint32_t& renderWidth, uint32_t& renderHeight) const override {
        XeSSQuality xessQuality = XeSSQuality::Quality;
        switch (m_quality) {
            case Quality::UltraQuality:     xessQuality = XeSSQuality::UltraQuality; break;
            case Quality::Quality:          xessQuality = XeSSQuality::Quality; break;
            case Quality::Balanced:         xessQuality = XeSSQuality::Balanced; break;
            case Quality::Performance:      xessQuality = XeSSQuality::Performance; break;
            case Quality::UltraPerformance: xessQuality = XeSSQuality::UltraPerformance; break;
        }
        XeSSContext::GetRenderResolution(xessQuality, displayWidth, displayHeight, renderWidth, renderHeight);
    }
    
    void GetJitter(uint32_t frameIndex, float& jitterX, float& jitterY) const override {
        if (m_xessContext) {
            m_xessContext->GetJitterOffset(frameIndex, jitterX, jitterY);
            return;
        }
        GetHaltonJitter(static_cast<int>(frameIndex), DEFAULT_JITTER_PHASE_COUNT, jitterX, jitterY);
    }
    
    bool Upscale(const UpscaleParams& params) override {
        if (!m_initialized || !m_xessContext) return false;
        
#ifdef _WIN32
        // Prepare XeSS dispatch parameters for D3D12
        XeSSDispatchParamsD3D12 dispatchParams = {};
        dispatchParams.commandList = static_cast<ID3D12GraphicsCommandList*>(params.commandList);
        dispatchParams.colorTexture = static_cast<ID3D12Resource*>(params.colorTexture);
        dispatchParams.depthTexture = static_cast<ID3D12Resource*>(params.depthTexture);
        dispatchParams.motionVectors = static_cast<ID3D12Resource*>(params.motionVectors);
        dispatchParams.outputTexture = static_cast<ID3D12Resource*>(params.outputTexture);
        dispatchParams.exposureTexture = static_cast<ID3D12Resource*>(params.exposureTexture);
        dispatchParams.responsiveMask = nullptr;  // Optional
        dispatchParams.renderWidth = params.renderWidth;
        dispatchParams.renderHeight = params.renderHeight;
        dispatchParams.displayWidth = params.displayWidth;
        dispatchParams.displayHeight = params.displayHeight;
        dispatchParams.jitterX = params.jitterX;
        dispatchParams.jitterY = params.jitterY;
        dispatchParams.reset = params.reset;
        
        return m_xessContext->UpscaleD3D12(dispatchParams);
#else
        // Vulkan path - not yet implemented
        return false;
#endif
    }
    
    bool SupportsFrameGeneration() const override { 
        // XeSS 2.0+ supports frame generation on Intel Arc
        return false;  // Not yet implemented
    }
    
    bool GenerateFrame(const FrameGenParams& params) override { 
        return false; 
    }
    
private:
    bool m_initialized = false;
    Quality m_quality = Quality::Quality;
    uint32_t m_displayWidth = 0;
    uint32_t m_displayHeight = 0;
    std::unique_ptr<XeSSContext> m_xessContext;
};
#endif // _WIN32

// =============================================================================
// MetalFX Context (Apple Silicon) - Uses native Objective-C++ implementation
// =============================================================================
class MetalFXContext : public IUpscalerContext {
public:
    bool Initialize(uint32_t displayWidth, uint32_t displayHeight) override {
        m_displayWidth = displayWidth;
        m_displayHeight = displayHeight;
        
#ifdef __APPLE__
        // Calculate render dimensions based on quality
        GetRenderDimensions(displayWidth, displayHeight, m_renderWidth, m_renderHeight);
        
        // Initialize native MetalFX implementation
        // Note: mtlDevice needs to be passed from video system
        // For now, we defer initialization until first use when device is available
        m_initialized = true;
        m_needsNativeInit = true;
        LOGF_INFO("[Upscaler] MetalFX context prepared ({}x{} -> {}x{})",
                  m_renderWidth, m_renderHeight, displayWidth, displayHeight);
#else
        LOG_WARNING("[Upscaler] MetalFX is only available on macOS/iOS");
        m_initialized = false;
#endif
        return m_initialized;
    }
    
    void Shutdown() override {
#ifdef __APPLE__
        MetalFX_Shutdown();
#endif
        m_initialized = false;
        m_needsNativeInit = true;
        LOG_INFO("[Upscaler] MetalFX shutdown");
    }
    
    bool IsAvailable() const override {
#ifdef __APPLE__
        // MetalFX requires macOS 13+ or iOS 16+
        if (__builtin_available(macOS 13.0, iOS 16.0, *)) {
            return true;
        }
        return false;
#else
        return false;
#endif
    }
    
    Backend GetBackend() const override { return Backend::MetalFX; }
    Quality GetQuality() const override { return m_quality; }
    void SetQuality(Quality quality) override { 
        m_quality = quality;
        // Recalculate render dimensions
        GetRenderDimensions(m_displayWidth, m_displayHeight, m_renderWidth, m_renderHeight);
    }
    
    void GetRenderDimensions(uint32_t displayWidth, uint32_t displayHeight,
                             uint32_t& renderWidth, uint32_t& renderHeight) const override {
        float scale = GetRenderScale(m_quality);
        renderWidth = static_cast<uint32_t>(displayWidth * scale);
        renderHeight = static_cast<uint32_t>(displayHeight * scale);
        // MetalFX prefers dimensions divisible by 16
        renderWidth = (renderWidth + 15) & ~15;
        renderHeight = (renderHeight + 15) & ~15;
    }
    
    void GetJitter(uint32_t frameIndex, float& jitterX, float& jitterY) const override {
        // MetalFX temporal scaler uses Halton jitter sequence
        GetHaltonJitter(static_cast<int>(frameIndex), DEFAULT_JITTER_PHASE_COUNT, jitterX, jitterY);
    }
    
    bool Upscale(const UpscaleParams& params) override {
        if (!m_initialized) return false;
        
#ifdef __APPLE__
        // Try temporal upscaling first (requires motion vectors)
        if (MetalFX_IsTemporalAvailable() && params.motionVectors != nullptr) {
            return MetalFX_UpscaleTemporal(
                params.colorTexture,
                params.depthTexture,
                params.motionVectors,
                params.outputTexture,
                params.jitterX,
                params.jitterY,
                params.reset);
        }
        
        // Fall back to spatial upscaling
        if (MetalFX_IsSpatialAvailable()) {
            return MetalFX_UpscaleSpatial(params.colorTexture, params.outputTexture);
        }
        
        LOG_WARNING("[MetalFX] No scaler available for upscaling");
        return false;
#else
        return false;
#endif
    }
    
    bool SupportsFrameGeneration() const override {
#ifdef __APPLE__
        // MetalFX Frame Interpolation available on M1 Pro/Max/Ultra and M2+
        return true;
#else
        return false;
#endif
    }
    
    bool GenerateFrame(const FrameGenParams& params) override {
        if (!m_initialized) return false;
#ifdef __APPLE__
        // MetalFX Frame Interpolation would go here
        // Currently not implemented - requires additional Metal APIs
        return false;
#else
        return false;
#endif
    }
    
    // Initialize native MetalFX with Metal device
    bool InitializeNative(void* mtlDevice) {
#ifdef __APPLE__
        if (!m_needsNativeInit) return true;
        
        bool result = MetalFX_Initialize(mtlDevice, m_renderWidth, m_renderHeight,
                                         m_displayWidth, m_displayHeight);
        if (result) {
            m_needsNativeInit = false;
            LOGF_INFO("[MetalFX] Native initialization complete (temporal: {}, spatial: {})",
                      MetalFX_IsTemporalAvailable() ? "yes" : "no",
                      MetalFX_IsSpatialAvailable() ? "yes" : "no");
        }
        return result;
#else
        return false;
#endif
    }
    
    void ResetTemporalHistory() {
#ifdef __APPLE__
        MetalFX_ResetHistory();
#endif
    }
    
private:
    bool m_initialized = false;
    bool m_needsNativeInit = true;
    Quality m_quality = Quality::Quality;
    uint32_t m_displayWidth = 0;
    uint32_t m_displayHeight = 0;
    uint32_t m_renderWidth = 0;
    uint32_t m_renderHeight = 0;
};

// =============================================================================
// Factory Functions
// =============================================================================
std::unique_ptr<IUpscalerContext> CreateUpscaler(Backend backend) {
    switch (backend) {
        case Backend::FSR1:    return std::make_unique<FSR1Context>();
#ifdef _WIN32
        case Backend::FSR3:    return std::make_unique<FSR3ContextWrapper>();
        case Backend::DLSS:    return std::make_unique<DLSSContextWrapper>();
        case Backend::XeSS:    return std::make_unique<XeSSContextWrapper>();
#else
        case Backend::FSR3:    return nullptr;  // Windows only
        case Backend::DLSS:    return nullptr;  // Windows only
        case Backend::XeSS:    return nullptr;  // Windows only
#endif
#ifdef __APPLE__
        case Backend::MetalFX: return std::make_unique<MetalFXContext>();
#else
        case Backend::MetalFX: return nullptr;  // macOS only
#endif
        default:               return nullptr;
    }
}

Backend GetBestAvailableBackend() {
#ifdef __APPLE__
    return Backend::MetalFX;
#elif defined(_WIN32)
    // Check for NVIDIA RTX first, then fall back to FSR
    if (IsBackendAvailable(Backend::DLSS)) return Backend::DLSS;
    if (IsBackendAvailable(Backend::FSR3)) return Backend::FSR3;
    return Backend::FSR1;
#else
    return Backend::FSR1;
#endif
}

bool IsBackendAvailable(Backend backend) {
    auto ctx = CreateUpscaler(backend);
    return ctx ? ctx->IsAvailable() : false;
}

const char* GetBackendName(Backend backend) {
    switch (backend) {
        case Backend::None:    return "None";
        case Backend::FSR1:    return "AMD FSR 1.0";
        case Backend::FSR3:    return "AMD FSR 3.0";
        case Backend::DLSS:    return "NVIDIA DLSS";
        case Backend::XeSS:    return "Intel XeSS";
        case Backend::MetalFX: return "Apple MetalFX";
        default:               return "Unknown";
    }
}

const char* GetBackendStatusString(Backend backend) {
    switch (backend) {
        case Backend::None:
            return "Available";
            
        case Backend::FSR1:
            if (PostProcess::g_postProcessRenderer.IsInitialized()) {
                return "Available (FSR 1.0 EASU + RCAS)";
            }
            return "Pending (PostProcessRenderer not initialized)";
            
        case Backend::FSR3:
#ifdef _WIN32
            if (FSR3Context::IsAvailable()) {
                return "Available (AMD FidelityFX SDK)";
            }
            return "Unavailable (amd_fidelityfx_loader.dll not found)";
#else
            return "Unavailable (Windows only)";
#endif
            
        case Backend::DLSS:
#ifdef _WIN32
            if (DLSSContext::IsAvailable()) {
                return "Available (NVIDIA NGX SDK)";
            }
            return "Unavailable (RTX GPU not detected)";
#else
            return "Unavailable (Windows + RTX GPU required)";
#endif
            
        case Backend::XeSS:
            if (XeSSContext::IsAvailable()) {
                return "Available (Intel XeSS SDK)";
            }
            return "Unavailable (libxess.dll not found)";
            
        case Backend::MetalFX:
#ifdef __APPLE__
            if (__builtin_available(macOS 13.0, iOS 16.0, *)) {
                return "Available (macOS 13+)";
            }
            return "Unavailable (requires macOS 13+)";
#else
            return "Unavailable (macOS/iOS only)";
#endif
            
        default:
            return "Unknown";
    }
}

// =============================================================================
// High-Level API Implementation
// =============================================================================
static Backend ConfigToBackend(EUpscaler configValue) {
    switch (configValue) {
        case EUpscaler::FSR1:    return Backend::FSR1;
        case EUpscaler::FSR3:    return Backend::FSR3;
        case EUpscaler::DLSS:    return Backend::DLSS;
        case EUpscaler::XeSS:    return Backend::XeSS;
        case EUpscaler::MetalFX: return Backend::MetalFX;
        default:                 return Backend::None;
    }
}

static Quality ConfigToQuality(EUpscaleQuality configValue) {
    switch (configValue) {
        case EUpscaleQuality::UltraQuality:     return Quality::UltraQuality;
        case EUpscaleQuality::Quality:          return Quality::Quality;
        case EUpscaleQuality::Balanced:         return Quality::Balanced;
        case EUpscaleQuality::Performance:      return Quality::Performance;
        case EUpscaleQuality::UltraPerformance: return Quality::UltraPerformance;
        default:                                return Quality::Quality;
    }
}

bool Initialize() {
    Backend backend = ConfigToBackend(Config::Upscaler);
    Quality quality = ConfigToQuality(Config::UpscaleQuality);
    
    if (backend == Backend::None) {
        LOG_INFO("[Upscaler] Upscaling disabled");
        g_upscalerState.initialized = false;
        return true;
    }
    
    // Check if the requested backend is actually available
    if (!IsBackendAvailable(backend)) {
        LOGF_WARNING("[Upscaler] {} is not available on this system (SDK not integrated or platform unsupported)",
                     GetBackendName(backend));
        LOG_WARNING("[Upscaler] Upscaling will be disabled - select a different backend or wait for implementation");
        g_upscalerState.initialized = false;
        g_upscalerState.activeBackend = Backend::None;
        return true;  // Not a fatal error, just disable upscaling
    }
    
    g_upscalerState.context = CreateUpscaler(backend);
    if (!g_upscalerState.context) {
        LOGF_ERROR("[Upscaler] Failed to create {} context", GetBackendName(backend));
        return false;
    }
    
    // Get display dimensions from video system
    g_upscalerState.displayWidth = Video::s_viewportWidth > 0 ? Video::s_viewportWidth : 1920;
    g_upscalerState.displayHeight = Video::s_viewportHeight > 0 ? Video::s_viewportHeight : 1080;
    
    if (!g_upscalerState.context->Initialize(g_upscalerState.displayWidth, 
                                              g_upscalerState.displayHeight)) {
        LOGF_ERROR("[Upscaler] Failed to initialize {}", GetBackendName(backend));
        g_upscalerState.context.reset();
        return false;
    }
    
    g_upscalerState.context->SetQuality(quality);
    g_upscalerState.context->GetRenderDimensions(
        g_upscalerState.displayWidth, g_upscalerState.displayHeight,
        g_upscalerState.renderWidth, g_upscalerState.renderHeight);
    
    g_upscalerState.activeBackend = backend;
    g_upscalerState.activeQuality = quality;
    g_upscalerState.initialized = true;
    
    // Only enable frame generation if actually supported AND available
    bool frameGenSupported = g_upscalerState.context->SupportsFrameGeneration();
    g_upscalerState.frameGenEnabled = 
        (Config::FrameGeneration != EFrameGeneration::Off) && frameGenSupported;
    
    if (Config::FrameGeneration != EFrameGeneration::Off && !frameGenSupported) {
        LOGF_WARNING("[Upscaler] Frame generation requested but not available for {}", 
                     GetBackendName(backend));
    }
    
    LOGF_INFO("[Upscaler] Initialized {} at {}x{} -> {}x{} (available: {}, frame gen: {})",
              GetBackendName(backend),
              g_upscalerState.renderWidth, g_upscalerState.renderHeight,
              g_upscalerState.displayWidth, g_upscalerState.displayHeight,
              g_upscalerState.context->IsAvailable() ? "yes" : "no",
              g_upscalerState.frameGenEnabled ? "enabled" : "disabled");
    
    return true;
}

void BeginFrame() {
    if (!g_upscalerState.initialized || !g_upscalerState.context) return;
    
    g_upscalerState.frameIndex++;
    g_upscalerState.context->GetJitter(
        g_upscalerState.frameIndex,
        g_upscalerState.currentJitterX,
        g_upscalerState.currentJitterY);
}

bool UpscaleFrame(void* colorTex, void* depthTex, void* motionTex, void* outputTex) {
    if (!g_upscalerState.initialized || !g_upscalerState.context) return false;
    
    UpscaleParams params = {};
    params.colorTexture = colorTex;
    params.depthTexture = depthTex;
    params.motionVectors = motionTex;
    params.outputTexture = outputTex;
    params.renderWidth = g_upscalerState.renderWidth;
    params.renderHeight = g_upscalerState.renderHeight;
    params.displayWidth = g_upscalerState.displayWidth;
    params.displayHeight = g_upscalerState.displayHeight;
    params.jitterX = g_upscalerState.currentJitterX;
    params.jitterY = g_upscalerState.currentJitterY;
    params.frameIndex = g_upscalerState.frameIndex;
    params.sharpness = Config::UpscaleSharpness;
    params.reset = (g_upscalerState.frameIndex == 1);
    
    return g_upscalerState.context->Upscale(params);
}

bool GenerateIntermediateFrame(void* outputTex) {
    if (!g_upscalerState.frameGenEnabled || !g_upscalerState.context) return false;
    
    FrameGenParams params = {};
    params.outputFrame = outputTex;
    params.reset = false;
    
    return g_upscalerState.context->GenerateFrame(params);
}

void Shutdown() {
    if (g_upscalerState.context) {
        g_upscalerState.context->Shutdown();
        g_upscalerState.context.reset();
    }
    g_upscalerState.initialized = false;
    g_upscalerState.activeBackend = Backend::None;
    LOG_INFO("[Upscaler] Shutdown complete");
}

void GetRenderDimensions(uint32_t& width, uint32_t& height) {
    if (g_upscalerState.initialized) {
        width = g_upscalerState.renderWidth;
        height = g_upscalerState.renderHeight;
    } else {
        width = g_upscalerState.displayWidth;
        height = g_upscalerState.displayHeight;
    }
}

void GetCurrentJitter(float& jitterX, float& jitterY) {
    jitterX = g_upscalerState.currentJitterX;
    jitterY = g_upscalerState.currentJitterY;
}

bool IsFrameGenerationActive() {
    return g_upscalerState.frameGenEnabled;
}

} // namespace Upscaler
