// =============================================================================
// XeSS Integration - Intel Xe Super Sampling
// Full implementation using Intel XeSS SDK
// SDK downloaded from: https://github.com/intel/xess/releases
// =============================================================================

#include "upscaler_xess.h"
#include "upscaler.h"
#include "os/logger.h"

#include <cmath>
#include <atomic>
#include <mutex>

#ifdef _WIN32
#include <Windows.h>
#endif

// Intel XeSS SDK headers from tools/xess/inc
#include <xess/xess.h>
#ifdef _WIN32
#include <xess/xess_d3d12.h>
#endif

namespace Upscaler {

// Function pointer typedefs for dynamic loading (Windows only - D3D12)
#ifdef _WIN32
typedef xess_result_t (*PFN_xessD3D12CreateContext)(ID3D12Device*, xess_context_handle_t*);
typedef xess_result_t (*PFN_xessDestroyContext)(xess_context_handle_t);
typedef xess_result_t (*PFN_xessD3D12Init)(xess_context_handle_t, const xess_d3d12_init_params_t*);
typedef xess_result_t (*PFN_xessD3D12Execute)(xess_context_handle_t, ID3D12GraphicsCommandList*, const xess_d3d12_execute_params_t*);
typedef xess_result_t (*PFN_xessGetInputResolution)(xess_context_handle_t, const xess_2d_t*, xess_quality_settings_t, xess_2d_t*);
typedef xess_result_t (*PFN_xessGetJitterScale)(xess_context_handle_t, float*, float*);
typedef xess_result_t (*PFN_xessGetOptimalInputResolution)(xess_context_handle_t, const xess_2d_t*, xess_quality_settings_t, xess_2d_t*, xess_2d_t*, xess_2d_t*);
typedef xess_result_t (*PFN_xessGetVersion)(xess_version_t*);
typedef xess_result_t (*PFN_xessIsOptimalDriver)(xess_context_handle_t);
typedef xess_result_t (*PFN_xessSetVelocityScale)(xess_context_handle_t, float, float);
#endif

// Static XeSS library state with thread safety
static std::atomic<bool> s_xessInitialized{false};
static std::atomic<bool> s_xessAvailable{false};
static std::mutex s_xessMutex;

#ifdef _WIN32
static void* s_xessLibrary = nullptr;
static PFN_xessD3D12CreateContext s_pfnCreateContext = nullptr;
static PFN_xessDestroyContext s_pfnDestroyContext = nullptr;
static PFN_xessD3D12Init s_pfnInit = nullptr;
static PFN_xessD3D12Execute s_pfnExecute = nullptr;
static PFN_xessGetInputResolution s_pfnGetInputResolution = nullptr;
static PFN_xessGetJitterScale s_pfnGetJitterScale = nullptr;
static PFN_xessGetOptimalInputResolution s_pfnGetOptimalInputResolution = nullptr;
static PFN_xessGetVersion s_pfnGetVersion = nullptr;
static PFN_xessIsOptimalDriver s_pfnIsOptimalDriver = nullptr;
static PFN_xessSetVelocityScale s_pfnSetVelocityScale = nullptr;
static char s_versionString[64] = "Unknown";
#endif

static bool LoadXeSSLibrary() {
    // Fast path - already checked
    if (s_xessInitialized.load()) return s_xessAvailable.load();
    
    std::lock_guard<std::mutex> lock(s_xessMutex);
    
    // Double-check after acquiring lock
    if (s_xessInitialized.load()) return s_xessAvailable.load();
    
#ifdef _WIN32
    // Try to load the XeSS DLL
    s_xessLibrary = LoadLibraryW(L"libxess.dll");
    if (!s_xessLibrary) {
        LOG_WARNING("[XeSS] Failed to load libxess.dll - XeSS not available");
        s_xessAvailable.store(false);
        s_xessInitialized.store(true);
        return false;
    }
    
    // Get function pointers
    s_pfnCreateContext = (PFN_xessD3D12CreateContext)GetProcAddress((HMODULE)s_xessLibrary, "xessD3D12CreateContext");
    s_pfnDestroyContext = (PFN_xessDestroyContext)GetProcAddress((HMODULE)s_xessLibrary, "xessDestroyContext");
    s_pfnInit = (PFN_xessD3D12Init)GetProcAddress((HMODULE)s_xessLibrary, "xessD3D12Init");
    s_pfnExecute = (PFN_xessD3D12Execute)GetProcAddress((HMODULE)s_xessLibrary, "xessD3D12Execute");
    s_pfnGetInputResolution = (PFN_xessGetInputResolution)GetProcAddress((HMODULE)s_xessLibrary, "xessGetInputResolution");
    s_pfnGetJitterScale = (PFN_xessGetJitterScale)GetProcAddress((HMODULE)s_xessLibrary, "xessGetJitterScale");
    s_pfnGetOptimalInputResolution = (PFN_xessGetOptimalInputResolution)GetProcAddress((HMODULE)s_xessLibrary, "xessGetOptimalInputResolution");
    s_pfnGetVersion = (PFN_xessGetVersion)GetProcAddress((HMODULE)s_xessLibrary, "xessGetVersion");
    s_pfnIsOptimalDriver = (PFN_xessIsOptimalDriver)GetProcAddress((HMODULE)s_xessLibrary, "xessIsOptimalDriver");
    s_pfnSetVelocityScale = (PFN_xessSetVelocityScale)GetProcAddress((HMODULE)s_xessLibrary, "xessSetVelocityScale");
    
    if (!s_pfnCreateContext || !s_pfnDestroyContext || !s_pfnInit || !s_pfnExecute) {
        LOG_ERROR("[XeSS] Failed to get XeSS function pointers");
        FreeLibrary((HMODULE)s_xessLibrary);
        s_xessLibrary = nullptr;
        s_xessAvailable.store(false);
        s_xessInitialized.store(true);
        return false;
    }
    
    // Get version
    if (s_pfnGetVersion) {
        xess_version_t version = {};
        if (s_pfnGetVersion(&version) == XESS_RESULT_SUCCESS) {
            snprintf(s_versionString, sizeof(s_versionString), "%u.%u.%u", 
                     version.major, version.minor, version.patch);
        }
    }
    
    LOGF_INFO("[XeSS] Intel XeSS SDK loaded (version {})", s_versionString);
    s_xessAvailable.store(true);
    s_xessInitialized.store(true);
    return true;
#else
    // Linux/macOS - XeSS requires libxess shared library
    // XeSS 2.0+ has Vulkan support but requires separate SDK integration
    LOG_WARNING("[XeSS] XeSS not available on this platform (Windows D3D12 only in current implementation)");
    s_xessAvailable.store(false);
    s_xessInitialized.store(true);
    return false;
#endif
}

// Unload XeSS library - call at application exit
static void UnloadXeSSLibrary() {
#ifdef _WIN32
    if (s_xessLibrary) {
        FreeLibrary((HMODULE)s_xessLibrary);
        s_xessLibrary = nullptr;
        s_pfnCreateContext = nullptr;
        s_pfnDestroyContext = nullptr;
        s_pfnInit = nullptr;
        s_pfnExecute = nullptr;
        s_pfnGetInputResolution = nullptr;
        s_pfnGetJitterScale = nullptr;
        s_pfnGetOptimalInputResolution = nullptr;
        s_pfnGetVersion = nullptr;
        s_pfnIsOptimalDriver = nullptr;
        s_pfnSetVelocityScale = nullptr;
        LOG_INFO("[XeSS] Intel XeSS SDK unloaded");
    }
#endif
}

#ifdef _WIN32

struct XeSSContext::Impl {
    xess_context_handle_t context = nullptr;
    ID3D12Device* device = nullptr;
    uint32_t optimalRenderWidth = 0;
    uint32_t optimalRenderHeight = 0;
    bool initialized = false;
    xess_quality_settings_t currentQuality = XESS_QUALITY_SETTING_QUALITY;
    
    ~Impl() {
        if (context && s_pfnDestroyContext) {
            s_pfnDestroyContext(context);
        }
    }
};

XeSSContext::XeSSContext() : m_impl(std::make_unique<Impl>()) {}

XeSSContext::~XeSSContext() {
    Shutdown();
}

bool XeSSContext::IsAvailable() {
    return LoadXeSSLibrary();
}

bool XeSSContext::IsXMXAvailable() {
    // XMX is available on Intel Arc GPUs
    // This requires checking the GPU vendor and model
    // For now, return false as a safe default
    return false;
}

const char* XeSSContext::GetVersionString() {
    if (LoadXeSSLibrary()) {
        return s_versionString;
    }
    return "Not Available";
}

bool XeSSContext::InitializeD3D12(const XeSSInitParamsD3D12& params) {
    if (m_initialized) {
        Shutdown();
    }
    
    if (!LoadXeSSLibrary()) {
        return false;
    }
    
    m_impl->device = params.device;
    m_displayWidth = params.displayWidth;
    m_displayHeight = params.displayHeight;
    m_isD3D12 = true;
    
    // Create XeSS context
    xess_result_t result = s_pfnCreateContext(params.device, &m_impl->context);
    if (result != XESS_RESULT_SUCCESS) {
        LOGF_ERROR("[XeSS] Failed to create context: {}", static_cast<int>(result));
        return false;
    }
    
    // Map quality setting
    xess_quality_settings_t xessQuality = XESS_QUALITY_SETTING_QUALITY;
    switch (m_quality) {
        case XeSSQuality::UltraPerformance: xessQuality = XESS_QUALITY_SETTING_ULTRA_PERFORMANCE; break;
        case XeSSQuality::Performance:      xessQuality = XESS_QUALITY_SETTING_PERFORMANCE; break;
        case XeSSQuality::Balanced:         xessQuality = XESS_QUALITY_SETTING_BALANCED; break;
        case XeSSQuality::Quality:          xessQuality = XESS_QUALITY_SETTING_QUALITY; break;
        case XeSSQuality::UltraQuality:     xessQuality = XESS_QUALITY_SETTING_ULTRA_QUALITY; break;
        case XeSSQuality::NativeAA:         xessQuality = XESS_QUALITY_SETTING_AA; break;
    }
    m_impl->currentQuality = xessQuality;
    
    // Get optimal render resolution
    if (s_pfnGetOptimalInputResolution) {
        xess_2d_t outputRes = { params.displayWidth, params.displayHeight };
        xess_2d_t optimalRes = {}, minRes = {}, maxRes = {};
        result = s_pfnGetOptimalInputResolution(m_impl->context, 
                                                  &outputRes,
                                                  xessQuality,
                                                  &optimalRes, &minRes, &maxRes);
        if (result == XESS_RESULT_SUCCESS) {
            m_impl->optimalRenderWidth = optimalRes.x;
            m_impl->optimalRenderHeight = optimalRes.y;
        } else {
            // Fall back to basic calculation
            GetRenderResolution(m_quality, params.displayWidth, params.displayHeight,
                               m_impl->optimalRenderWidth, m_impl->optimalRenderHeight);
        }
    } else {
        GetRenderResolution(m_quality, params.displayWidth, params.displayHeight,
                           m_impl->optimalRenderWidth, m_impl->optimalRenderHeight);
    }
    
    // Initialize XeSS context with parameters
    xess_d3d12_init_params_t initParams = {};
    initParams.outputResolution.x = params.displayWidth;
    initParams.outputResolution.y = params.displayHeight;
    initParams.qualitySetting = xessQuality;
    initParams.initFlags = XESS_INIT_FLAG_INVERTED_DEPTH;
    if (params.enableAutoExposure) {
        initParams.initFlags |= XESS_INIT_FLAG_ENABLE_AUTOEXPOSURE;
    }
    if (params.enableResponsiveMask) {
        initParams.initFlags |= XESS_INIT_FLAG_RESPONSIVE_PIXEL_MASK;
    }
    initParams.creationNodeMask = 1;
    initParams.visibleNodeMask = 1;
    
    result = s_pfnInit(m_impl->context, &initParams);
    if (result != XESS_RESULT_SUCCESS) {
        LOGF_ERROR("[XeSS] Failed to initialize context: {}", static_cast<int>(result));
        s_pfnDestroyContext(m_impl->context);
        m_impl->context = nullptr;
        return false;
    }
    m_impl->initialized = true;
    
    // Calculate jitter phase count based on upscaling ratio
    float ratio = static_cast<float>(params.displayWidth) / static_cast<float>(m_impl->optimalRenderWidth);
    m_jitterPhaseCount = static_cast<int32_t>(8.0f * ratio * ratio);
    if (m_jitterPhaseCount < 1) m_jitterPhaseCount = 1;
    
    m_initialized = true;
    LOGF_INFO("[XeSS] Initialized (render={}x{}, display={}x{}, jitterPhases={})",
              m_impl->optimalRenderWidth, m_impl->optimalRenderHeight,
              params.displayWidth, params.displayHeight, m_jitterPhaseCount);
    return true;
}

bool XeSSContext::InitializeVulkan(const XeSSInitParamsVulkan& params) {
    // XeSS Vulkan support requires XeSS 2.0+ SDK which provides xessVulkanCreateContext
    // and related Vulkan-specific APIs. The current implementation uses XeSS 1.x which
    // only supports D3D12. To enable Vulkan support:
    // 1. Update to XeSS 2.0+ SDK from https://github.com/intel/xess/releases
    // 2. Add xess_vulkan.h include
    // 3. Implement xessVulkanCreateContext and xessVulkanExecute calls
    LOG_WARNING("[XeSS] Vulkan support requires XeSS 2.0+ SDK - not yet implemented");
    return false;
}

void XeSSContext::Shutdown() {
    if (!m_initialized) return;
    
    m_impl = std::make_unique<Impl>();
    m_initialized = false;
    LOG_INFO("[XeSS] Shutdown");
}

void XeSSContext::GetRenderResolution(XeSSQuality quality,
                                       uint32_t displayWidth, uint32_t displayHeight,
                                       uint32_t& renderWidth, uint32_t& renderHeight) {
    float ratio = 1.0f;
    switch (quality) {
        case XeSSQuality::UltraPerformance: ratio = 3.0f; break;
        case XeSSQuality::Performance:      ratio = 2.0f; break;
        case XeSSQuality::Balanced:         ratio = 1.7f; break;
        case XeSSQuality::Quality:          ratio = 1.5f; break;
        case XeSSQuality::UltraQuality:     ratio = 1.3f; break;
        case XeSSQuality::NativeAA:         ratio = 1.0f; break;
    }
    
    renderWidth = static_cast<uint32_t>(displayWidth / ratio);
    renderHeight = static_cast<uint32_t>(displayHeight / ratio);
    
    // Align to 2 pixels
    renderWidth = (renderWidth + 1) & ~1;
    renderHeight = (renderHeight + 1) & ~1;
}

void XeSSContext::GetJitterOffset(uint32_t frameIndex, float& jitterX, float& jitterY) const {
    if (!m_initialized) {
        jitterX = 0.0f;
        jitterY = 0.0f;
        return;
    }
    
    // Use shared Halton sequence utility
    GetHaltonJitter(static_cast<int>(frameIndex), m_jitterPhaseCount, jitterX, jitterY);
    
    // Apply jitter scale if available from SDK
    if (s_pfnGetJitterScale && m_impl->context) {
        float scaleX = 1.0f, scaleY = 1.0f;
        if (s_pfnGetJitterScale(m_impl->context, &scaleX, &scaleY) == XESS_RESULT_SUCCESS) {
            jitterX *= scaleX;
            jitterY *= scaleY;
        }
    }
}

bool XeSSContext::UpscaleD3D12(const XeSSDispatchParamsD3D12& params) {
    if (!m_initialized || !m_impl->context || !m_isD3D12 || !m_impl->initialized) {
        return false;
    }
    
    // Set velocity scale for motion vectors
    if (s_pfnSetVelocityScale) {
        s_pfnSetVelocityScale(m_impl->context, params.motionVectorScaleX, params.motionVectorScaleY);
    }
    
    // Build execution parameters using SDK struct
    xess_d3d12_execute_params_t execParams = {};
    execParams.pColorTexture = params.colorInput;
    execParams.pDepthTexture = params.depthInput;
    execParams.pVelocityTexture = params.motionVectors;
    execParams.pExposureScaleTexture = params.exposure;
    execParams.pResponsivePixelMaskTexture = params.responsiveMask;
    execParams.pOutputTexture = params.output;
    execParams.inputWidth = params.renderWidth;
    execParams.inputHeight = params.renderHeight;
    execParams.jitterOffsetX = params.jitterX;
    execParams.jitterOffsetY = params.jitterY;
    execParams.exposureScale = 1.0f;
    execParams.resetHistory = params.reset ? 1 : 0;
    
    xess_result_t result = s_pfnExecute(m_impl->context, params.commandList, &execParams);
    if (result != XESS_RESULT_SUCCESS) {
        LOGF_ERROR("[XeSS] Execute failed: {}", static_cast<int>(result));
        return false;
    }
    
    return true;
}

bool XeSSContext::UpscaleVulkan(const XeSSDispatchParamsVulkan& params) {
    // Vulkan upscaling - requires XeSS 2.0 SDK
    LOG_WARNING("[XeSS] Vulkan upscaling not implemented");
    return false;
}

void XeSSContext::SetQuality(XeSSQuality quality) {
    m_quality = quality;
}

#else // !_WIN32

// Empty Impl struct for non-Windows platforms
struct XeSSContext::Impl {};

// Stub implementations for non-Windows platforms
XeSSContext::XeSSContext() : m_impl(nullptr) {}
XeSSContext::~XeSSContext() {}
bool XeSSContext::IsAvailable() { return false; }
bool XeSSContext::IsXMXAvailable() { return false; }
const char* XeSSContext::GetVersionString() { return "Not Available"; }
bool XeSSContext::InitializeD3D12(const XeSSInitParamsD3D12&) { return false; }
bool XeSSContext::InitializeVulkan(const XeSSInitParamsVulkan&) { return false; }
void XeSSContext::Shutdown() {}
void XeSSContext::GetRenderResolution(XeSSQuality, uint32_t, uint32_t, uint32_t&, uint32_t&) {}
void XeSSContext::GetJitterOffset(uint32_t, float&, float&) const {}
bool XeSSContext::UpscaleD3D12(const XeSSDispatchParamsD3D12&) { return false; }
bool XeSSContext::UpscaleVulkan(const XeSSDispatchParamsVulkan&) { return false; }
void XeSSContext::SetQuality(XeSSQuality) {}

#endif // _WIN32

} // namespace Upscaler
