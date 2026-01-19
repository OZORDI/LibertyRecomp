// =============================================================================
// DLSS Integration - NVIDIA Deep Learning Super Sampling
// Uses NVIDIA NGX SDK from tools/DLSS
// =============================================================================

#pragma once

#ifdef _WIN32  // DLSS only supported on Windows with NVIDIA RTX GPUs

#include <cstdint>
#include <memory>

// Forward declarations
struct ID3D12Device;
struct ID3D12GraphicsCommandList;
struct ID3D12Resource;

namespace Upscaler {

// DLSS Quality Modes
enum class DLSSQuality {
    UltraPerformance = 0,  // 3.0x
    Performance = 1,       // 2.0x
    Balanced = 2,          // 1.7x
    Quality = 3,           // 1.5x
    UltraQuality = 4,      // 1.3x (DLAA when render=display)
    DLAA = 5               // Native resolution AA
};

// DLSS Context Creation Parameters
struct DLSSInitParams {
    ID3D12Device* device;
    uint32_t maxRenderWidth;
    uint32_t maxRenderHeight;
    uint32_t displayWidth;
    uint32_t displayHeight;
    bool enableHDR;
    bool enableSharpening;
    bool enableFrameGeneration;  // DLSS 3 Frame Generation
    const wchar_t* appDataPath;  // Path for NGX temp files
    uint64_t appId;              // NVIDIA App ID (0 for development)
};

// DLSS Dispatch Parameters
struct DLSSDispatchParams {
    ID3D12GraphicsCommandList* commandList;
    ID3D12Resource* colorInput;
    ID3D12Resource* depthInput;
    ID3D12Resource* motionVectors;
    ID3D12Resource* exposure;         // Optional
    ID3D12Resource* output;
    uint32_t renderWidth;
    uint32_t renderHeight;
    uint32_t displayWidth;
    uint32_t displayHeight;
    float jitterX;
    float jitterY;
    float motionVectorScaleX;
    float motionVectorScaleY;
    float sharpness;            // 0.0 - 1.0
    bool reset;                 // Camera cut/teleport
    bool isHDR;
};

// DLSS Frame Generation Parameters (DLSS 3)
struct DLSSFrameGenParams {
    ID3D12GraphicsCommandList* commandList;
    ID3D12Resource* backbufferInput;
    ID3D12Resource* backbufferOutput;
    ID3D12Resource* depthInput;
    ID3D12Resource* motionVectors;
    ID3D12Resource* hudLessColor;     // Optional
    bool reset;
};

class DLSSContext {
public:
    DLSSContext();
    ~DLSSContext();
    
    // Initialize DLSS context
    bool Initialize(const DLSSInitParams& params);
    
    // Shutdown and release resources
    void Shutdown();
    
    // Check if DLSS is available (RTX GPU required)
    static bool IsAvailable();
    
    // Check if frame generation is supported (RTX 40 series required)
    bool SupportsFrameGeneration() const;
    
    // Get render resolution for quality mode
    static void GetRenderResolution(DLSSQuality quality,
                                     uint32_t displayWidth, uint32_t displayHeight,
                                     uint32_t& renderWidth, uint32_t& renderHeight);
    
    // Get optimal settings for quality mode
    bool GetOptimalSettings(DLSSQuality quality,
                            uint32_t& renderWidth, uint32_t& renderHeight,
                            float& sharpness) const;
    
    // Get jitter offset for current frame
    void GetJitterOffset(uint32_t frameIndex, float& jitterX, float& jitterY) const;
    
    // Execute DLSS upscaling pass
    bool Upscale(const DLSSDispatchParams& params);
    
    // Execute DLSS Frame Generation pass
    bool GenerateFrame(const DLSSFrameGenParams& params);
    
    // Set quality mode
    void SetQuality(DLSSQuality quality);
    DLSSQuality GetQuality() const { return m_quality; }
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    DLSSQuality m_quality = DLSSQuality::Quality;
    bool m_initialized = false;
    bool m_frameGenSupported = false;
    uint32_t m_displayWidth = 0;
    uint32_t m_displayHeight = 0;
};

} // namespace Upscaler

#endif // _WIN32
