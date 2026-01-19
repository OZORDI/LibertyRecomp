// =============================================================================
// FSR 3.0 Integration - AMD FidelityFX Super Resolution 3
// Full implementation using AMD FidelityFX SDK
// =============================================================================

#ifdef _WIN32

#include "upscaler_fsr3.h"
#include "os/logger.h"

#include <Windows.h>
#include <d3d12.h>
#include <atomic>
#include <mutex>

// FidelityFX SDK includes from tools/FidelityFX-SDK/Kits/FidelityFX
#include <api/include/ffx_api.h>
#include <api/include/ffx_api_types.h>
#include <api/include/dx12/ffx_api_dx12.h>
#include <upscalers/include/ffx_upscale.h>
#include <framegeneration/include/ffx_framegeneration.h>

namespace Upscaler {

// FSR 3.0 DLL loader with thread safety
static std::atomic<bool> s_fsr3Initialized{false};
static std::atomic<bool> s_fsr3Available{false};
static std::atomic<bool> s_fsr3AvailabilityChecked{false};
static std::mutex s_fsr3Mutex;
static HMODULE s_ffxLoaderDll = nullptr;
static PfnFfxCreateContext s_pfnCreateContext = nullptr;
static PfnFfxDestroyContext s_pfnDestroyContext = nullptr;
static PfnFfxConfigure s_pfnConfigure = nullptr;
static PfnFfxQuery s_pfnQuery = nullptr;
static PfnFfxDispatch s_pfnDispatch = nullptr;

static bool LoadFSR3Library() {
    // Fast path - already loaded
    if (s_fsr3Initialized.load()) return s_fsr3Available.load();
    
    std::lock_guard<std::mutex> lock(s_fsr3Mutex);
    
    // Double-check after acquiring lock
    if (s_fsr3Initialized.load()) return s_fsr3Available.load();
    
    // Try to load the FidelityFX loader DLL
    s_ffxLoaderDll = LoadLibraryW(L"amd_fidelityfx_loader.dll");
    if (!s_ffxLoaderDll) {
        LOG_WARNING("[FSR3] Failed to load amd_fidelityfx_loader.dll");
        s_fsr3Available.store(false);
        s_fsr3Initialized.store(true);
        return false;
    }
    
    // Get function pointers
    s_pfnCreateContext = (PfnFfxCreateContext)GetProcAddress(s_ffxLoaderDll, "ffxCreateContext");
    s_pfnDestroyContext = (PfnFfxDestroyContext)GetProcAddress(s_ffxLoaderDll, "ffxDestroyContext");
    s_pfnConfigure = (PfnFfxConfigure)GetProcAddress(s_ffxLoaderDll, "ffxConfigure");
    s_pfnQuery = (PfnFfxQuery)GetProcAddress(s_ffxLoaderDll, "ffxQuery");
    s_pfnDispatch = (PfnFfxDispatch)GetProcAddress(s_ffxLoaderDll, "ffxDispatch");
    
    if (!s_pfnCreateContext || !s_pfnDestroyContext || !s_pfnDispatch) {
        LOG_ERROR("[FSR3] Failed to get FFX function pointers");
        FreeLibrary(s_ffxLoaderDll);
        s_ffxLoaderDll = nullptr;
        s_fsr3Available.store(false);
        s_fsr3Initialized.store(true);
        return false;
    }
    
    LOG_INFO("[FSR3] AMD FidelityFX SDK loaded successfully");
    s_fsr3Available.store(true);
    s_fsr3Initialized.store(true);
    return true;
}

// Unload FSR3 library - call at application exit
static void UnloadFSR3Library() {
    if (s_ffxLoaderDll) {
        FreeLibrary(s_ffxLoaderDll);
        s_ffxLoaderDll = nullptr;
        s_pfnCreateContext = nullptr;
        s_pfnDestroyContext = nullptr;
        s_pfnConfigure = nullptr;
        s_pfnQuery = nullptr;
        s_pfnDispatch = nullptr;
        LOG_INFO("[FSR3] AMD FidelityFX SDK unloaded");
    }
}

struct FSR3Context::Impl {
    ffxContext upscaleContext = nullptr;
    ffxContext frameGenContext = nullptr;
    ID3D12Device* device = nullptr;
    
    ~Impl() {
        if (upscaleContext && s_pfnDestroyContext) {
            s_pfnDestroyContext(&upscaleContext, nullptr);
        }
        if (frameGenContext && s_pfnDestroyContext) {
            s_pfnDestroyContext(&frameGenContext, nullptr);
        }
    }
};

FSR3Context::FSR3Context() : m_impl(std::make_unique<Impl>()) {}

FSR3Context::~FSR3Context() {
    Shutdown();
}

bool FSR3Context::IsAvailable() {
    return LoadFSR3Library();
}

bool FSR3Context::Initialize(const FSR3InitParams& params) {
    if (m_initialized) {
        Shutdown();
    }
    
    if (!LoadFSR3Library()) {
        return false;
    }
    
    m_impl->device = params.device;
    m_displayWidth = params.displayWidth;
    m_displayHeight = params.displayHeight;
    
    // Create upscale context
    ffxCreateContextDescUpscale upscaleDesc = {};
    upscaleDesc.header.type = FFX_API_CREATE_CONTEXT_DESC_TYPE_UPSCALE;
    upscaleDesc.maxRenderSize.width = params.maxRenderWidth;
    upscaleDesc.maxRenderSize.height = params.maxRenderHeight;
    upscaleDesc.maxUpscaleSize.width = params.displayWidth;
    upscaleDesc.maxUpscaleSize.height = params.displayHeight;
    
    if (params.enableHDR) {
        upscaleDesc.flags |= FFX_UPSCALE_ENABLE_HIGH_DYNAMIC_RANGE;
    }
    if (params.enableAutoExposure) {
        upscaleDesc.flags |= FFX_UPSCALE_ENABLE_AUTO_EXPOSURE;
    }
    upscaleDesc.flags |= FFX_UPSCALE_ENABLE_DEPTH_INVERTED;
    
    // Create DX12 backend description
    ffxCreateBackendDX12Desc backendDesc = {};
    backendDesc.header.type = FFX_API_CREATE_CONTEXT_DESC_TYPE_BACKEND_DX12;
    backendDesc.device = params.device;
    upscaleDesc.header.pNext = &backendDesc.header;
    
    ffxReturnCode_t result = s_pfnCreateContext(&m_impl->upscaleContext, 
                                                 &upscaleDesc.header, nullptr);
    if (result != FFX_API_RETURN_OK) {
        LOGF_ERROR("[FSR3] Failed to create upscale context: {}", result);
        return false;
    }
    
    // Query jitter phase count
    ffxQueryDescUpscaleGetJitterPhaseCount jitterQuery = {};
    jitterQuery.header.type = FFX_API_QUERY_DESC_TYPE_UPSCALE_GETJITTERPHASECOUNT;
    jitterQuery.renderWidth = params.maxRenderWidth;
    jitterQuery.displayWidth = params.displayWidth;
    jitterQuery.pOutPhaseCount = &m_jitterPhaseCount;
    s_pfnQuery(&m_impl->upscaleContext, &jitterQuery.header);
    
    // Create frame generation context if requested
    if (params.enableFrameGeneration) {
        ffxCreateContextDescFrameGeneration frameGenDesc = {};
        frameGenDesc.header.type = FFX_API_CREATE_CONTEXT_DESC_TYPE_FRAMEGENERATION;
        frameGenDesc.displaySize.width = params.displayWidth;
        frameGenDesc.displaySize.height = params.displayHeight;
        frameGenDesc.header.pNext = &backendDesc.header;
        
        result = s_pfnCreateContext(&m_impl->frameGenContext, 
                                     &frameGenDesc.header, nullptr);
        if (result == FFX_API_RETURN_OK) {
            m_frameGenSupported = true;
            LOG_INFO("[FSR3] Frame generation enabled");
        } else {
            LOG_WARNING("[FSR3] Frame generation not available on this hardware");
        }
    }
    
    m_initialized = true;
    LOGF_INFO("[FSR3] Initialized (render={}x{}, display={}x{}, jitterPhases={})",
              params.maxRenderWidth, params.maxRenderHeight,
              params.displayWidth, params.displayHeight, m_jitterPhaseCount);
    return true;
}

void FSR3Context::Shutdown() {
    if (!m_initialized) return;
    
    m_impl = std::make_unique<Impl>();
    m_initialized = false;
    m_frameGenSupported = false;
    LOG_INFO("[FSR3] Shutdown");
}

bool FSR3Context::SupportsFrameGeneration() const {
    return m_frameGenSupported && m_impl->frameGenContext != nullptr;
}

void FSR3Context::GetRenderResolution(FSR3Quality quality,
                                       uint32_t displayWidth, uint32_t displayHeight,
                                       uint32_t& renderWidth, uint32_t& renderHeight) {
    float ratio = 1.0f;
    switch (quality) {
        case FSR3Quality::NativeAA:        ratio = 1.0f; break;
        case FSR3Quality::Quality:         ratio = 1.5f; break;
        case FSR3Quality::Balanced:        ratio = 1.7f; break;
        case FSR3Quality::Performance:     ratio = 2.0f; break;
        case FSR3Quality::UltraPerformance: ratio = 3.0f; break;
    }
    
    renderWidth = static_cast<uint32_t>(displayWidth / ratio);
    renderHeight = static_cast<uint32_t>(displayHeight / ratio);
    
    // Align to 2 pixels
    renderWidth = (renderWidth + 1) & ~1;
    renderHeight = (renderHeight + 1) & ~1;
}

void FSR3Context::GetJitterOffset(uint32_t frameIndex, float& jitterX, float& jitterY) const {
    if (!m_initialized || !s_pfnQuery) {
        jitterX = 0.0f;
        jitterY = 0.0f;
        return;
    }
    
    ffxQueryDescUpscaleGetJitterOffset jitterQuery = {};
    jitterQuery.header.type = FFX_API_QUERY_DESC_TYPE_UPSCALE_GETJITTEROFFSET;
    jitterQuery.index = frameIndex % m_jitterPhaseCount;
    jitterQuery.phaseCount = m_jitterPhaseCount;
    jitterQuery.pOutX = &jitterX;
    jitterQuery.pOutY = &jitterY;
    
    s_pfnQuery(&m_impl->upscaleContext, &jitterQuery.header);
}

bool FSR3Context::Upscale(const FSR3DispatchParams& params) {
    if (!m_initialized || !m_impl->upscaleContext) {
        return false;
    }
    
    ffxDispatchDescUpscale dispatchDesc = {};
    dispatchDesc.header.type = FFX_API_DISPATCH_DESC_TYPE_UPSCALE;
    dispatchDesc.commandList = params.commandList;
    
    // Set up resources using SDK helper
    dispatchDesc.color = ffxApiGetResourceDX12(params.colorInput, FFX_API_RESOURCE_STATE_COMPUTE_READ);
    dispatchDesc.depth = ffxApiGetResourceDX12(params.depthInput, FFX_API_RESOURCE_STATE_COMPUTE_READ);
    dispatchDesc.motionVectors = ffxApiGetResourceDX12(params.motionVectors, FFX_API_RESOURCE_STATE_COMPUTE_READ);
    
    if (params.exposure) {
        dispatchDesc.exposure = ffxApiGetResourceDX12(params.exposure, FFX_API_RESOURCE_STATE_COMPUTE_READ);
    }
    
    if (params.reactive) {
        dispatchDesc.reactive = ffxApiGetResourceDX12(params.reactive, FFX_API_RESOURCE_STATE_COMPUTE_READ);
    }
    
    dispatchDesc.output = ffxApiGetResourceDX12(params.output, FFX_API_RESOURCE_STATE_UNORDERED_ACCESS, FFX_API_RESOURCE_USAGE_UAV);
    
    dispatchDesc.jitterOffset.x = params.jitterX;
    dispatchDesc.jitterOffset.y = params.jitterY;
    dispatchDesc.motionVectorScale.x = params.motionVectorScaleX;
    dispatchDesc.motionVectorScale.y = params.motionVectorScaleY;
    dispatchDesc.renderSize.width = params.renderWidth;
    dispatchDesc.renderSize.height = params.renderHeight;
    dispatchDesc.upscaleSize.width = params.displayWidth;
    dispatchDesc.upscaleSize.height = params.displayHeight;
    dispatchDesc.enableSharpening = params.sharpness > 0.0f;
    dispatchDesc.sharpness = params.sharpness;
    dispatchDesc.frameTimeDelta = params.frameTimeDelta;
    dispatchDesc.preExposure = 1.0f;
    dispatchDesc.reset = params.reset;
    dispatchDesc.cameraNear = params.cameraNear;
    dispatchDesc.cameraFar = params.cameraFar;
    dispatchDesc.cameraFovAngleVertical = params.cameraFovVertical;
    dispatchDesc.viewSpaceToMetersFactor = 1.0f;
    
    ffxReturnCode_t result = s_pfnDispatch(&m_impl->upscaleContext, &dispatchDesc.header);
    if (result != FFX_API_RETURN_OK) {
        LOGF_ERROR("[FSR3] Upscale dispatch failed: {}", result);
        return false;
    }
    
    return true;
}

bool FSR3Context::GenerateFrame(const FSR3FrameGenParams& params) {
    if (!m_initialized || !m_impl->frameGenContext || !m_frameGenSupported) {
        return false;
    }
    
    ffxDispatchDescFrameGeneration dispatchDesc = {};
    dispatchDesc.header.type = FFX_API_DISPATCH_DESC_TYPE_FRAMEGENERATION;
    dispatchDesc.commandList = params.commandList;
    
    dispatchDesc.presentColor = ffxApiGetResourceDX12(params.backbufferInput, FFX_API_RESOURCE_STATE_COMPUTE_READ);
    dispatchDesc.outputs[0] = ffxApiGetResourceDX12(params.backbufferOutput, FFX_API_RESOURCE_STATE_UNORDERED_ACCESS, FFX_API_RESOURCE_USAGE_UAV);
    
    dispatchDesc.reset = params.reset;
    dispatchDesc.numInterpolatedFrames = 1;
    
    ffxReturnCode_t result = s_pfnDispatch(&m_impl->frameGenContext, &dispatchDesc.header);
    if (result != FFX_API_RETURN_OK) {
        LOGF_ERROR("[FSR3] Frame generation dispatch failed: {}", result);
        return false;
    }
    
    return true;
}

void FSR3Context::SetQuality(FSR3Quality quality) {
    m_quality = quality;
}

} // namespace Upscaler

#endif // _WIN32
