// =============================================================================
// FSR 3.0 Integration - AMD FidelityFX Super Resolution 3
// Uses AMD FidelityFX SDK from tools/FidelityFX-SDK
// =============================================================================

#pragma once

#ifdef _WIN32  // FSR 3.0 only supported on Windows (D3D12)

#include <cstdint>
#include <memory>

// Forward declarations
struct ID3D12Device;
struct ID3D12GraphicsCommandList;
struct ID3D12Resource;

namespace Upscaler {

// FSR 3.0 Quality Modes
enum class FSR3Quality {
    NativeAA = 0,        // 1.0x
    Quality = 1,         // 1.5x
    Balanced = 2,        // 1.7x
    Performance = 3,     // 2.0x
    UltraPerformance = 4 // 3.0x
};

// FSR 3.0 Context Creation Parameters
struct FSR3InitParams {
    ID3D12Device* device;
    uint32_t maxRenderWidth;
    uint32_t maxRenderHeight;
    uint32_t displayWidth;
    uint32_t displayHeight;
    bool enableHDR;
    bool enableAutoExposure;
    bool enableSharpening;
    bool enableFrameGeneration;
};

// FSR 3.0 Dispatch Parameters
struct FSR3DispatchParams {
    ID3D12GraphicsCommandList* commandList;
    ID3D12Resource* colorInput;
    ID3D12Resource* depthInput;
    ID3D12Resource* motionVectors;
    ID3D12Resource* exposure;         // Optional, can be nullptr
    ID3D12Resource* reactive;         // Optional, can be nullptr
    ID3D12Resource* output;
    uint32_t renderWidth;
    uint32_t renderHeight;
    uint32_t displayWidth;
    uint32_t displayHeight;
    float jitterX;
    float jitterY;
    float motionVectorScaleX;
    float motionVectorScaleY;
    float frameTimeDelta;       // milliseconds
    float sharpness;            // 0.0 - 1.0
    float cameraNear;
    float cameraFar;
    float cameraFovVertical;    // radians
    bool reset;                 // Camera cut/teleport
};

// FSR 3.0 Frame Generation Parameters
struct FSR3FrameGenParams {
    ID3D12GraphicsCommandList* commandList;
    ID3D12Resource* backbufferInput;
    ID3D12Resource* backbufferOutput;
    ID3D12Resource* depthInput;
    ID3D12Resource* motionVectors;
    bool reset;
};

class FSR3Context {
public:
    FSR3Context();
    ~FSR3Context();
    
    // Initialize FSR 3.0 context
    bool Initialize(const FSR3InitParams& params);
    
    // Shutdown and release resources
    void Shutdown();
    
    // Check if FSR 3.0 is available on this system
    static bool IsAvailable();
    
    // Check if frame generation is supported
    bool SupportsFrameGeneration() const;
    
    // Get render resolution for quality mode
    static void GetRenderResolution(FSR3Quality quality, 
                                     uint32_t displayWidth, uint32_t displayHeight,
                                     uint32_t& renderWidth, uint32_t& renderHeight);
    
    // Get jitter offset for current frame
    void GetJitterOffset(uint32_t frameIndex, float& jitterX, float& jitterY) const;
    
    // Execute upscaling pass
    bool Upscale(const FSR3DispatchParams& params);
    
    // Execute frame generation pass
    bool GenerateFrame(const FSR3FrameGenParams& params);
    
    // Set quality mode
    void SetQuality(FSR3Quality quality);
    FSR3Quality GetQuality() const { return m_quality; }
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    FSR3Quality m_quality = FSR3Quality::Quality;
    bool m_initialized = false;
    bool m_frameGenSupported = false;
    uint32_t m_displayWidth = 0;
    uint32_t m_displayHeight = 0;
    int32_t m_jitterPhaseCount = 0;
};

} // namespace Upscaler

#endif // _WIN32
