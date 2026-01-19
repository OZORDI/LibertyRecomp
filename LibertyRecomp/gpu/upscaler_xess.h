// =============================================================================
// XeSS Integration - Intel Xe Super Sampling
// Provides high-quality temporal upscaling using Intel XeSS SDK
// 
// Platform Support:
//   - Windows D3D12: Fully implemented
//   - Windows/Linux Vulkan: Requires XeSS 2.0+ SDK (not yet implemented)
//   - macOS: Not supported
// =============================================================================

#pragma once

#include <cstdint>
#include <memory>

// Forward declarations for D3D12
struct ID3D12Device;
struct ID3D12GraphicsCommandList;
struct ID3D12Resource;

// Forward declarations for Vulkan
typedef struct VkDevice_T* VkDevice;
typedef struct VkCommandBuffer_T* VkCommandBuffer;
typedef struct VkImage_T* VkImage;

namespace Upscaler {

// XeSS Quality Modes
enum class XeSSQuality {
    UltraPerformance = 0,  // 3.0x
    Performance = 1,       // 2.0x
    Balanced = 2,          // 1.7x
    Quality = 3,           // 1.5x
    UltraQuality = 4,      // 1.3x
    NativeAA = 5           // 1.0x (anti-aliasing only)
};

// XeSS Init Mode
enum class XeSSInitMode {
    Default = 0,           // Use DP4a if available, otherwise fallback
    HighQuality = 1,       // Force XMX (Intel Arc GPUs only)
    HighPerformance = 2    // Force DP4a (works on all SM 6.4 GPUs)
};

// XeSS Context Creation Parameters (D3D12)
struct XeSSInitParamsD3D12 {
    ID3D12Device* device;
    uint32_t maxRenderWidth;
    uint32_t maxRenderHeight;
    uint32_t displayWidth;
    uint32_t displayHeight;
    XeSSInitMode initMode;
    bool enableHDR;
    bool enableAutoExposure;
    bool enableResponsiveMask;
};

// XeSS Context Creation Parameters (Vulkan)
struct XeSSInitParamsVulkan {
    VkDevice device;
    uint32_t maxRenderWidth;
    uint32_t maxRenderHeight;
    uint32_t displayWidth;
    uint32_t displayHeight;
    XeSSInitMode initMode;
    bool enableHDR;
    bool enableAutoExposure;
};

// XeSS Dispatch Parameters (D3D12)
struct XeSSDispatchParamsD3D12 {
    ID3D12GraphicsCommandList* commandList;
    ID3D12Resource* colorInput;
    ID3D12Resource* depthInput;
    ID3D12Resource* motionVectors;
    ID3D12Resource* exposure;         // Optional
    ID3D12Resource* responsiveMask;   // Optional
    ID3D12Resource* output;
    uint32_t renderWidth;
    uint32_t renderHeight;
    float jitterX;
    float jitterY;
    float motionVectorScaleX;
    float motionVectorScaleY;
    bool reset;
    bool isHDR;
};

// XeSS Dispatch Parameters (Vulkan)
struct XeSSDispatchParamsVulkan {
    VkCommandBuffer commandBuffer;
    VkImage colorInput;
    VkImage depthInput;
    VkImage motionVectors;
    VkImage exposure;         // Optional (VK_NULL_HANDLE if not used)
    VkImage output;
    uint32_t renderWidth;
    uint32_t renderHeight;
    float jitterX;
    float jitterY;
    float motionVectorScaleX;
    float motionVectorScaleY;
    bool reset;
    bool isHDR;
};

class XeSSContext {
public:
    XeSSContext();
    ~XeSSContext();
    
    // Initialize XeSS context (D3D12)
    bool InitializeD3D12(const XeSSInitParamsD3D12& params);
    
    // Initialize XeSS context (Vulkan)
    bool InitializeVulkan(const XeSSInitParamsVulkan& params);
    
    // Shutdown and release resources
    void Shutdown();
    
    // Check if XeSS is available
    static bool IsAvailable();
    
    // Check if XMX (Intel Arc) acceleration is available
    static bool IsXMXAvailable();
    
    // Get render resolution for quality mode
    static void GetRenderResolution(XeSSQuality quality,
                                     uint32_t displayWidth, uint32_t displayHeight,
                                     uint32_t& renderWidth, uint32_t& renderHeight);
    
    // Get jitter offset for current frame
    void GetJitterOffset(uint32_t frameIndex, float& jitterX, float& jitterY) const;
    
    // Execute XeSS upscaling pass (D3D12)
    bool UpscaleD3D12(const XeSSDispatchParamsD3D12& params);
    
    // Execute XeSS upscaling pass (Vulkan)
    bool UpscaleVulkan(const XeSSDispatchParamsVulkan& params);
    
    // Set quality mode
    void SetQuality(XeSSQuality quality);
    XeSSQuality GetQuality() const { return m_quality; }
    
    // Get version info
    static const char* GetVersionString();
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    XeSSQuality m_quality = XeSSQuality::Quality;
    bool m_initialized = false;
    bool m_isD3D12 = false;
    uint32_t m_displayWidth = 0;
    uint32_t m_displayHeight = 0;
    int32_t m_jitterPhaseCount = 0;
};

} // namespace Upscaler
