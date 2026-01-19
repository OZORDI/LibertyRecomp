// =============================================================================
// Unified Upscaler Integration Module
// Supports: FSR 1.0, FSR 3.0, DLSS, XeSS, MetalFX
// =============================================================================
#pragma once

#include <cstdint>
#include <memory>

namespace Upscaler {

// =============================================================================
// Upscaler Backend Types
// =============================================================================
enum class Backend {
    None,
    FSR1,       // AMD FidelityFX Super Resolution 1.0 (spatial only)
    FSR3,       // AMD FidelityFX Super Resolution 3.0 (temporal + frame gen)
    DLSS,       // NVIDIA Deep Learning Super Sampling (requires RTX)
    XeSS,       // Intel Xe Super Sampling
    MetalFX     // Apple MetalFX (macOS only, spatial + temporal)
};

// =============================================================================
// Quality Presets with Render Scale Factors
// =============================================================================
enum class Quality {
    UltraQuality,     // 77% (1.3x)
    Quality,          // 67% (1.5x)
    Balanced,         // 58% (1.7x)
    Performance,      // 50% (2.0x)
    UltraPerformance  // 33% (3.0x)
};

// Get render scale factor for quality preset
float GetRenderScale(Quality quality);

// Get output scale factor (inverse of render scale)
float GetOutputScale(Quality quality);

// Quality mapping utilities are defined in upscaler.cpp
// They convert generic Quality to SDK-specific quality enums (FSR3Quality, DLSSQuality, XeSSQuality)

// =============================================================================
// Jitter Utilities (shared across upscalers)
// =============================================================================

// Halton sequence for jitter - used by TAA-based upscalers
inline float HaltonSequence(int index, int base) {
    float result = 0.0f;
    float f = 1.0f / static_cast<float>(base);
    int i = index;
    while (i > 0) {
        result += f * static_cast<float>(i % base);
        i /= base;
        f /= static_cast<float>(base);
    }
    return result;
}

// Get jitter offset using Halton sequence (standard for temporal upscalers)
inline void GetHaltonJitter(int frameIndex, int phaseCount, float& jitterX, float& jitterY) {
    int index = (frameIndex % phaseCount) + 1;
    jitterX = HaltonSequence(index, 2) - 0.5f;
    jitterY = HaltonSequence(index, 3) - 0.5f;
}

// =============================================================================
// Motion Vector Format
// =============================================================================
struct MotionVectors {
    void* texture;          // Platform-specific texture handle
    uint32_t width;
    uint32_t height;
    float scaleX;           // Pixel space to NDC scale X
    float scaleY;           // Pixel space to NDC scale Y
    bool jittered;          // Whether motion vectors include jitter
};

// =============================================================================
// Upscaler Input/Output Textures
// =============================================================================
struct UpscaleParams {
    // Input textures (render resolution)
    void* colorTexture;
    void* depthTexture;
    void* motionVectors;
    void* exposureTexture;      // Optional, for HDR
    void* reactiveMask;         // Optional, for transparency handling
    
    // Output texture (display resolution)
    void* outputTexture;
    
    // Dimensions
    uint32_t renderWidth;
    uint32_t renderHeight;
    uint32_t displayWidth;
    uint32_t displayHeight;
    
    // Camera parameters
    float cameraNear;
    float cameraFar;
    float cameraFovY;           // Vertical FOV in radians
    
    // Jitter for TAA
    float jitterX;
    float jitterY;
    
    // Timing
    float deltaTime;
    float sharpness;            // 0.0 = soft, 1.0 = sharp
    
    // Frame state
    uint32_t frameIndex;
    bool reset;                 // Reset temporal history
    bool hdrEnabled;
};

// =============================================================================
// Frame Generation Parameters
// =============================================================================
struct FrameGenParams {
    void* currentFrame;
    void* previousFrame;
    void* motionVectors;
    void* depthBuffer;
    void* outputFrame;
    
    uint32_t width;
    uint32_t height;
    float deltaTime;
    bool reset;
};

// =============================================================================
// Upscaler Context (Abstract Base)
// =============================================================================
class IUpscalerContext {
public:
    virtual ~IUpscalerContext() = default;
    
    // Initialize with display dimensions
    virtual bool Initialize(uint32_t displayWidth, uint32_t displayHeight) = 0;
    
    // Shutdown and release resources
    virtual void Shutdown() = 0;
    
    // Check if backend is available on this system
    virtual bool IsAvailable() const = 0;
    
    // Get backend type
    virtual Backend GetBackend() const = 0;
    
    // Get current quality preset
    virtual Quality GetQuality() const = 0;
    
    // Set quality preset
    virtual void SetQuality(Quality quality) = 0;
    
    // Calculate render dimensions for current quality
    virtual void GetRenderDimensions(uint32_t displayWidth, uint32_t displayHeight,
                                     uint32_t& renderWidth, uint32_t& renderHeight) const = 0;
    
    // Get jitter offset for current frame (for TAA-based upscalers)
    virtual void GetJitter(uint32_t frameIndex, float& jitterX, float& jitterY) const = 0;
    
    // Perform upscaling
    virtual bool Upscale(const UpscaleParams& params) = 0;
    
    // Check if frame generation is supported
    virtual bool SupportsFrameGeneration() const = 0;
    
    // Generate interpolated frame
    virtual bool GenerateFrame(const FrameGenParams& params) = 0;
};

// =============================================================================
// Factory Functions
// =============================================================================

// Create upscaler context for specified backend
std::unique_ptr<IUpscalerContext> CreateUpscaler(Backend backend);

// Get best available upscaler for current platform
Backend GetBestAvailableBackend();

// Check if specific backend is available
bool IsBackendAvailable(Backend backend);

// Get human-readable name for backend
const char* GetBackendName(Backend backend);

// Get status string for backend (for UI display)
const char* GetBackendStatusString(Backend backend);

// =============================================================================
// Global Upscaler State
// =============================================================================
struct UpscalerState {
    bool initialized = false;
    Backend activeBackend = Backend::None;
    Quality activeQuality = Quality::Quality;
    std::unique_ptr<IUpscalerContext> context;
    
    uint32_t displayWidth = 0;
    uint32_t displayHeight = 0;
    uint32_t renderWidth = 0;
    uint32_t renderHeight = 0;
    
    uint32_t frameIndex = 0;
    float currentJitterX = 0.0f;
    float currentJitterY = 0.0f;
    
    bool frameGenEnabled = false;
};

// Global upscaler state
extern UpscalerState g_upscalerState;

// =============================================================================
// High-Level API
// =============================================================================

// Set the graphics device for SDK upscalers (call before Initialize)
// For D3D12: pass ID3D12Device*
// For Vulkan: pass VkDevice
// For Metal: pass id<MTLDevice>
void SetGraphicsDevice(void* device);

// Get the graphics device (for SDK upscaler initialization)
void* GetGraphicsDevice();

// Initialize upscaler system with config settings
bool Initialize();

// Update upscaler for new frame
void BeginFrame();

// Perform upscaling on current frame
bool UpscaleFrame(void* colorTex, void* depthTex, void* motionTex, void* outputTex);

// Generate interpolated frame (if supported)
bool GenerateIntermediateFrame(void* outputTex);

// Shutdown upscaler system
void Shutdown();

// Get current render dimensions
void GetRenderDimensions(uint32_t& width, uint32_t& height);

// Get current jitter for projection matrix
void GetCurrentJitter(float& jitterX, float& jitterY);

// Check if frame generation is active
bool IsFrameGenerationActive();

} // namespace Upscaler
