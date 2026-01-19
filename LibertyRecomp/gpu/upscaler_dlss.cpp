// =============================================================================
// DLSS Integration - NVIDIA Deep Learning Super Sampling
// Full implementation using NVIDIA NGX SDK
// =============================================================================

#ifdef _WIN32

#include "upscaler_dlss.h"
#include "upscaler.h"
#include "os/logger.h"

#include <Windows.h>
#include <d3d12.h>
#include <atomic>
#include <mutex>

// NVIDIA NGX SDK includes from tools/DLSS/include
#include <nvsdk_ngx.h>
#include <nvsdk_ngx_defs.h>
#include <nvsdk_ngx_params.h>
#include <nvsdk_ngx_helpers.h>

namespace Upscaler {

// Static state with thread safety
static std::atomic<bool> s_ngxInitialized{false};
static NVSDK_NGX_Parameter* s_ngxParams = nullptr;
static std::mutex s_ngxMutex;
static ID3D12Device* s_ngxDevice = nullptr;

struct DLSSContext::Impl {
    ID3D12Device* device = nullptr;
    NVSDK_NGX_Handle* dlssFeature = nullptr;
    NVSDK_NGX_Handle* frameGenFeature = nullptr;
    uint32_t optimalRenderWidth = 0;
    uint32_t optimalRenderHeight = 0;
    float optimalSharpness = 0.0f;
    
    ~Impl() {
        if (dlssFeature) {
            // Feature will be released in Shutdown()
        }
    }
};

static bool InitializeNGX(ID3D12Device* device, const wchar_t* appDataPath, uint64_t appId) {
    if (s_ngxInitialized.load()) return true;
    
    std::lock_guard<std::mutex> lock(s_ngxMutex);
    if (s_ngxInitialized.load()) return true;  // Double-check after acquiring lock
    
    NVSDK_NGX_FeatureCommonInfo featureInfo = {};
    featureInfo.PathListInfo.Length = 0;
    featureInfo.PathListInfo.Path = nullptr;
    
    NVSDK_NGX_Result result = NVSDK_NGX_D3D12_Init(
        appId,                          // Application ID (0 for dev)
        appDataPath,                    // App data path for temp files
        device,                         // D3D12 device
        &featureInfo,                   // Feature info
        NVSDK_NGX_Version_API           // API version
    );
    
    if (NVSDK_NGX_FAILED(result)) {
        LOGF_ERROR("[DLSS] NGX initialization failed: 0x{:X}", static_cast<uint32_t>(result));
        return false;
    }
    
    // Get capability parameters
    result = NVSDK_NGX_D3D12_GetCapabilityParameters(&s_ngxParams);
    if (NVSDK_NGX_FAILED(result)) {
        LOG_ERROR("[DLSS] Failed to get NGX capability parameters");
        NVSDK_NGX_D3D12_Shutdown1(device);
        return false;
    }
    
    // Check if DLSS is supported
    int dlssSupported = 0;
    NVSDK_NGX_Result dlssCheckResult = s_ngxParams->Get(NVSDK_NGX_Parameter_SuperSampling_Available, &dlssSupported);
    if (NVSDK_NGX_FAILED(dlssCheckResult) || !dlssSupported) {
        LOG_WARNING("[DLSS] DLSS not supported on this GPU (RTX required)");
        NVSDK_NGX_D3D12_Shutdown1(device);
        return false;
    }
    
    s_ngxDevice = device;
    s_ngxInitialized.store(true);
    LOG_INFO("[DLSS] NVIDIA NGX SDK initialized successfully");
    return true;
}

// Shutdown NGX globally - call at application exit
static void ShutdownNGX() {
    std::lock_guard<std::mutex> lock(s_ngxMutex);
    if (!s_ngxInitialized.load()) return;
    
    if (s_ngxDevice) {
        NVSDK_NGX_D3D12_Shutdown1(s_ngxDevice);
        s_ngxDevice = nullptr;
    }
    s_ngxParams = nullptr;
    s_ngxInitialized.store(false);
    LOG_INFO("[DLSS] NVIDIA NGX SDK shutdown");
}

DLSSContext::DLSSContext() : m_impl(std::make_unique<Impl>()) {}

DLSSContext::~DLSSContext() {
    Shutdown();
}

// Cached DLSS availability check
static std::atomic<bool> s_dlssAvailabilityChecked{false};
static std::atomic<bool> s_dlssAvailable{false};

bool DLSSContext::IsAvailable() {
    // Fast path - already checked
    if (s_dlssAvailabilityChecked.load()) {
        return s_dlssAvailable.load();
    }
    
    std::lock_guard<std::mutex> lock(s_ngxMutex);
    
    // Double-check after acquiring lock
    if (s_dlssAvailabilityChecked.load()) {
        return s_dlssAvailable.load();
    }
    
    bool available = false;
    
    // Check for NVIDIA driver and RTX support
    HMODULE nvapi = LoadLibraryW(L"nvapi64.dll");
    if (nvapi) {
        FreeLibrary(nvapi);
        
        // Check for NGX DLL
        HMODULE ngxCore = LoadLibraryW(L"_nvngx.dll");
        if (!ngxCore) {
            ngxCore = LoadLibraryW(L"nvngx_dlss.dll");
        }
        if (ngxCore) {
            FreeLibrary(ngxCore);
            available = true;
        }
    }
    
    s_dlssAvailable.store(available);
    s_dlssAvailabilityChecked.store(true);
    
    if (available) {
        LOG_INFO("[DLSS] NVIDIA NGX SDK available");
    } else {
        LOG_INFO("[DLSS] NVIDIA NGX SDK not available (no RTX GPU or drivers)");
    }
    
    return available;
}

bool DLSSContext::Initialize(const DLSSInitParams& params) {
    if (m_initialized) {
        Shutdown();
    }
    
    m_impl->device = params.device;
    m_displayWidth = params.displayWidth;
    m_displayHeight = params.displayHeight;
    
    if (!InitializeNGX(params.device, params.appDataPath, params.appId)) {
        return false;
    }
    
    // Get optimal settings for the quality mode
    NVSDK_NGX_PerfQuality_Value perfQuality = NVSDK_NGX_PerfQuality_Value_MaxQuality;
    switch (m_quality) {
        case DLSSQuality::UltraPerformance: perfQuality = NVSDK_NGX_PerfQuality_Value_UltraPerformance; break;
        case DLSSQuality::Performance:      perfQuality = NVSDK_NGX_PerfQuality_Value_MaxPerf; break;
        case DLSSQuality::Balanced:         perfQuality = NVSDK_NGX_PerfQuality_Value_Balanced; break;
        case DLSSQuality::Quality:          perfQuality = NVSDK_NGX_PerfQuality_Value_MaxQuality; break;
        case DLSSQuality::UltraQuality:     perfQuality = NVSDK_NGX_PerfQuality_Value_UltraQuality; break;
        case DLSSQuality::DLAA:             perfQuality = NVSDK_NGX_PerfQuality_Value_DLAA; break;
    }
    
    // Query optimal render resolution
    NVSDK_NGX_Result result = NGX_DLSS_GET_OPTIMAL_SETTINGS(
        s_ngxParams,
        params.displayWidth,
        params.displayHeight,
        perfQuality,
        &m_impl->optimalRenderWidth,
        &m_impl->optimalRenderHeight,
        &m_impl->optimalRenderWidth,  // maxRenderWidth
        &m_impl->optimalRenderHeight, // maxRenderHeight
        &m_impl->optimalRenderWidth,  // minRenderWidth
        &m_impl->optimalRenderHeight, // minRenderHeight
        &m_impl->optimalSharpness
    );
    
    if (NVSDK_NGX_FAILED(result)) {
        LOGF_ERROR("[DLSS] Failed to get optimal settings: 0x{:X}", static_cast<uint32_t>(result));
        return false;
    }
    
    // Create DLSS feature
    NVSDK_NGX_DLSS_Create_Params dlssParams = {};
    dlssParams.Feature.InWidth = m_impl->optimalRenderWidth;
    dlssParams.Feature.InHeight = m_impl->optimalRenderHeight;
    dlssParams.Feature.InTargetWidth = params.displayWidth;
    dlssParams.Feature.InTargetHeight = params.displayHeight;
    dlssParams.Feature.InPerfQualityValue = perfQuality;
    dlssParams.InFeatureCreateFlags = NVSDK_NGX_DLSS_Feature_Flags_None;
    
    if (params.enableHDR) {
        dlssParams.InFeatureCreateFlags |= NVSDK_NGX_DLSS_Feature_Flags_IsHDR;
    }
    if (params.enableSharpening) {
        dlssParams.InFeatureCreateFlags |= NVSDK_NGX_DLSS_Feature_Flags_DoSharpening;
    }
    dlssParams.InFeatureCreateFlags |= NVSDK_NGX_DLSS_Feature_Flags_MVLowRes;
    dlssParams.InFeatureCreateFlags |= NVSDK_NGX_DLSS_Feature_Flags_DepthInverted;
    
    // We need a command list to create the feature - defer creation to first Upscale call
    
    m_initialized = true;
    
    // Check for frame generation support (RTX 40 series)
    if (params.enableFrameGeneration) {
        int frameGenSupported = 0;
        s_ngxParams->Get(NVSDK_NGX_Parameter_FrameGeneration_Available, &frameGenSupported);
        m_frameGenSupported = (frameGenSupported != 0);
        if (m_frameGenSupported) {
            LOG_INFO("[DLSS] Frame Generation (DLSS 3) supported");
        }
    }
    
    LOGF_INFO("[DLSS] Initialized (render={}x{}, display={}x{}, sharpness={:.2f})",
              m_impl->optimalRenderWidth, m_impl->optimalRenderHeight,
              params.displayWidth, params.displayHeight, m_impl->optimalSharpness);
    return true;
}

void DLSSContext::Shutdown() {
    if (!m_initialized) return;
    
    if (m_impl->dlssFeature) {
        NVSDK_NGX_D3D12_ReleaseFeature(m_impl->dlssFeature);
        m_impl->dlssFeature = nullptr;
    }
    
    if (m_impl->frameGenFeature) {
        NVSDK_NGX_D3D12_ReleaseFeature(m_impl->frameGenFeature);
        m_impl->frameGenFeature = nullptr;
    }
    
    m_initialized = false;
    m_frameGenSupported = false;
    LOG_INFO("[DLSS] Shutdown");
}

bool DLSSContext::SupportsFrameGeneration() const {
    return m_frameGenSupported;
}

void DLSSContext::GetRenderResolution(DLSSQuality quality,
                                       uint32_t displayWidth, uint32_t displayHeight,
                                       uint32_t& renderWidth, uint32_t& renderHeight) {
    float ratio = 1.0f;
    switch (quality) {
        case DLSSQuality::UltraPerformance: ratio = 3.0f; break;
        case DLSSQuality::Performance:      ratio = 2.0f; break;
        case DLSSQuality::Balanced:         ratio = 1.7f; break;
        case DLSSQuality::Quality:          ratio = 1.5f; break;
        case DLSSQuality::UltraQuality:     ratio = 1.3f; break;
        case DLSSQuality::DLAA:             ratio = 1.0f; break;
    }
    
    renderWidth = static_cast<uint32_t>(displayWidth / ratio);
    renderHeight = static_cast<uint32_t>(displayHeight / ratio);
    
    // Align to 2 pixels
    renderWidth = (renderWidth + 1) & ~1;
    renderHeight = (renderHeight + 1) & ~1;
}

bool DLSSContext::GetOptimalSettings(DLSSQuality quality,
                                      uint32_t& renderWidth, uint32_t& renderHeight,
                                      float& sharpness) const {
    if (!m_initialized) return false;
    
    renderWidth = m_impl->optimalRenderWidth;
    renderHeight = m_impl->optimalRenderHeight;
    sharpness = m_impl->optimalSharpness;
    return true;
}

void DLSSContext::GetJitterOffset(uint32_t frameIndex, float& jitterX, float& jitterY) const {
    // DLSS uses Halton sequence for jittering - use shared utility
    const int phaseCount = 8;  // Standard for DLSS
    GetHaltonJitter(static_cast<int>(frameIndex), phaseCount, jitterX, jitterY);
}

bool DLSSContext::Upscale(const DLSSDispatchParams& params) {
    if (!m_initialized) return false;
    
    // Create feature on first use (requires command list)
    if (!m_impl->dlssFeature) {
        NVSDK_NGX_PerfQuality_Value perfQuality = NVSDK_NGX_PerfQuality_Value_MaxQuality;
        switch (m_quality) {
            case DLSSQuality::UltraPerformance: perfQuality = NVSDK_NGX_PerfQuality_Value_UltraPerformance; break;
            case DLSSQuality::Performance:      perfQuality = NVSDK_NGX_PerfQuality_Value_MaxPerf; break;
            case DLSSQuality::Balanced:         perfQuality = NVSDK_NGX_PerfQuality_Value_Balanced; break;
            case DLSSQuality::Quality:          perfQuality = NVSDK_NGX_PerfQuality_Value_MaxQuality; break;
            case DLSSQuality::UltraQuality:     perfQuality = NVSDK_NGX_PerfQuality_Value_UltraQuality; break;
            case DLSSQuality::DLAA:             perfQuality = NVSDK_NGX_PerfQuality_Value_DLAA; break;
        }
        
        int createFlags = NVSDK_NGX_DLSS_Feature_Flags_MVLowRes | 
                          NVSDK_NGX_DLSS_Feature_Flags_DepthInverted;
        if (params.isHDR) {
            createFlags |= NVSDK_NGX_DLSS_Feature_Flags_IsHDR;
        }
        if (params.sharpness > 0.0f) {
            createFlags |= NVSDK_NGX_DLSS_Feature_Flags_DoSharpening;
        }
        
        NVSDK_NGX_Result result = NGX_D3D12_CREATE_DLSS_EXT(
            params.commandList,
            1, 1,  // Creation node mask
            &m_impl->dlssFeature,
            s_ngxParams,
            createFlags,
            params.renderWidth,
            params.renderHeight,
            params.displayWidth,
            params.displayHeight,
            perfQuality
        );
        
        if (NVSDK_NGX_FAILED(result)) {
            LOGF_ERROR("[DLSS] Failed to create DLSS feature: 0x{:X}", static_cast<uint32_t>(result));
            return false;
        }
    }
    
    // Execute DLSS
    NVSDK_NGX_D3D12_DLSS_Eval_Params evalParams = {};
    evalParams.Feature.pInColor = params.colorInput;
    evalParams.Feature.pInOutput = params.output;
    evalParams.pInDepth = params.depthInput;
    evalParams.pInMotionVectors = params.motionVectors;
    evalParams.pInExposureTexture = params.exposure;
    evalParams.InJitterOffsetX = params.jitterX;
    evalParams.InJitterOffsetY = params.jitterY;
    evalParams.Feature.InSharpness = params.sharpness;
    evalParams.InReset = params.reset ? 1 : 0;
    evalParams.InMVScaleX = params.motionVectorScaleX;
    evalParams.InMVScaleY = params.motionVectorScaleY;
    evalParams.InRenderSubrectDimensions.Width = params.renderWidth;
    evalParams.InRenderSubrectDimensions.Height = params.renderHeight;
    
    NVSDK_NGX_Result result = NGX_D3D12_EVALUATE_DLSS_EXT(
        params.commandList,
        m_impl->dlssFeature,
        s_ngxParams,
        &evalParams
    );
    
    if (NVSDK_NGX_FAILED(result)) {
        LOGF_ERROR("[DLSS] Evaluation failed: 0x{:X}", static_cast<uint32_t>(result));
        return false;
    }
    
    return true;
}

bool DLSSContext::GenerateFrame(const DLSSFrameGenParams& params) {
    if (!m_initialized || !m_frameGenSupported) {
        return false;
    }
    
    // DLSS 3 Frame Generation implementation
    // This requires RTX 40 series GPU and additional setup
    
    if (!m_impl->frameGenFeature) {
        // Create frame generation feature on first use
        NVSDK_NGX_Result result = NGX_D3D12_CREATE_DLSSG(
            params.commandList,
            1,
            &m_impl->frameGenFeature,
            s_ngxParams,
            m_displayWidth,
            m_displayHeight
        );
        
        if (NVSDK_NGX_FAILED(result)) {
            LOGF_ERROR("[DLSS] Failed to create frame generation feature: 0x{:X}", 
                       static_cast<uint32_t>(result));
            m_frameGenSupported = false;
            return false;
        }
    }
    
    // Execute frame generation
    NVSDK_NGX_D3D12_DLSSG_Eval_Params evalParams = {};
    evalParams.pInOutput = params.backbufferOutput;
    evalParams.pInDepth = params.depthInput;
    evalParams.pInMotionVectors = params.motionVectors;
    evalParams.pInHUDLessColor = params.hudLessColor;
    evalParams.InReset = params.reset ? 1 : 0;
    
    NVSDK_NGX_Result result = NGX_D3D12_EVALUATE_DLSSG(
        params.commandList,
        m_impl->frameGenFeature,
        s_ngxParams,
        &evalParams
    );
    
    if (NVSDK_NGX_FAILED(result)) {
        LOGF_ERROR("[DLSS] Frame generation failed: 0x{:X}", static_cast<uint32_t>(result));
        return false;
    }
    
    return true;
}

void DLSSContext::SetQuality(DLSSQuality quality) {
    if (quality != m_quality) {
        m_quality = quality;
        // Need to recreate feature with new quality
        if (m_impl->dlssFeature) {
            NVSDK_NGX_D3D12_ReleaseFeature(m_impl->dlssFeature);
            m_impl->dlssFeature = nullptr;
        }
    }
}

} // namespace Upscaler

#endif // _WIN32
