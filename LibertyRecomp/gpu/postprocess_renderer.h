// =============================================================================
// Post-Process Renderer - Actual Implementation
// TAA, SMAA, FSR 1.0 rendering pipelines with proper resource management
// =============================================================================
#pragma once

#include <plume_render_interface.h>
#include <memory>
#include <cstdint>

using namespace plume;

namespace PostProcess {

// Forward declarations
struct PostProcessResources;

// =============================================================================
// TAA Constants - matches taa_ps.hlsl cbuffer
// =============================================================================
struct TAAConstants {
    float resolutionX;      // 1/width
    float resolutionY;      // 1/height  
    float width;
    float height;
    float jitterX;
    float jitterY;
    float prevJitterX;
    float prevJitterY;
    float blendFactor;      // 0.05 - 0.1 typical
    float motionScale;
    float padding[2];
};

// =============================================================================
// SMAA Constants - matches smaa_edge_detect_ps.hlsl cbuffer
// =============================================================================
struct SMAAConstants {
    float resolutionX;      // 1/width
    float resolutionY;      // 1/height
    float width;
    float height;
};

// =============================================================================
// FSR 1.0 Constants - matches fsr1_easu_ps.hlsl cbuffer
// =============================================================================
struct FSR1Constants {
    float inputWidth;
    float inputHeight;
    float inputRcpWidth;
    float inputRcpHeight;
    float outputWidth;
    float outputHeight;
    float outputRcpWidth;
    float outputRcpHeight;
    float scaleX;           // inputWidth/outputWidth
    float scaleY;           // inputHeight/outputHeight
    float halfScaleX;
    float halfScaleY;
    float reserved[4];
};

// =============================================================================
// Vignette Constants - matches vignette_ps.hlsl cbuffer
// =============================================================================
struct VignetteConstants {
    float intensity;      // Overall vignette strength (0 = off, 1 = full)
    float radius;         // Inner radius where vignette starts (0-1)
    float softness;       // Falloff softness (higher = softer edge)
    float roundness;      // Aspect ratio correction (1 = circular)
    float centerX;        // Vignette center X (usually 0.5)
    float centerY;        // Vignette center Y (usually 0.5)
    float resolutionX;    // Screen width
    float resolutionY;    // Screen height
    uint32_t textureIndex;// Input texture descriptor index
    uint32_t padding[3];
};

// =============================================================================
// SSAO Constants - matches ssao_gtao_ps.hlsl cbuffer
// =============================================================================
struct SSAOConstants {
    float resolutionX;        // 1/width
    float resolutionY;        // 1/height
    float width;
    float height;
    float cameraNear;         // Camera near plane
    float cameraFar;          // Camera far plane
    float cameraFovY;         // Vertical FOV in radians
    float aspectRatio;        // Width/Height
    float projMatrix[16];     // Projection matrix (row-major)
    float invProjMatrix[16];  // Inverse projection matrix
    float radius;             // World-space AO radius
    float intensity;          // AO intensity multiplier
    float bias;               // Depth comparison bias
    float falloffDistance;    // Distance at which AO fades out
    int sampleCount;          // Number of direction samples
    int stepsPerSample;       // Steps per direction
    float frameIndex;         // For temporal jitter
    float padding;
};

// =============================================================================
// SSAO Blur Constants - matches ssao_blur_ps.hlsl cbuffer
// =============================================================================
struct SSAOBlurConstants {
    float resolutionX;        // 1/width
    float resolutionY;        // 1/height
    float width;
    float height;
    float blurDirectionX;     // 1 for horizontal, 0 for vertical
    float blurDirectionY;     // 0 for horizontal, 1 for vertical
    float sharpness;          // Edge sharpness
    float depthThreshold;     // Depth threshold for edge detection
    float nearPlane;
    float farPlane;
    float padding[2];
};

// =============================================================================
// DoF Constants - matches dof_ps.hlsl cbuffer
// =============================================================================
struct DOFConstants {
    float resolutionX;        // 1/width
    float resolutionY;        // 1/height
    float width;
    float height;
    float focusDistance;      // Distance to focus plane
    float apertureSize;       // Aperture size (affects blur amount)
    float nearPlane;          // Camera near plane
    float farPlane;           // Camera far plane
    int kernelSize;           // Quality level (3, 5, 7, or 9)
    float maxBlur;            // Maximum blur radius in pixels
    float padding[2];
};

// =============================================================================
// SSR Constants - matches ssr_raytrace_ps.hlsl cbuffer
// =============================================================================
struct SSRConstants {
    float resolutionX;        // 1/width
    float resolutionY;        // 1/height
    float width;
    float height;
    float cameraNear;
    float cameraFar;
    float cameraFovY;
    float aspectRatio;
    float viewMatrix[16];     // View matrix (row-major)
    float projMatrix[16];     // Projection matrix
    float invViewMatrix[16];  // Inverse view matrix
    float invProjMatrix[16];  // Inverse projection matrix
    float maxDistance;        // Maximum ray distance
    float stride;             // Ray march stride
    float strideZCutoff;      // Z cutoff for stride
    float maxSteps;           // Maximum ray steps
    float thickness;          // Surface thickness
    float fadeStart;          // Fade start distance
    float fadeEnd;            // Fade end distance
    float roughnessCutoff;    // Roughness threshold
    float edgeFade;           // Screen edge fade
    float frameIndex;         // Temporal jitter
    float padding[2];
};

// =============================================================================
// SSR Composite Constants - matches ssr_composite_ps.hlsl cbuffer
// =============================================================================
struct SSRCompositeConstants {
    float resolutionX;
    float resolutionY;
    float width;
    float height;
    float intensity;          // Overall SSR intensity
    float maxRoughness;       // Max roughness for reflections
    float padding[2];
};

// =============================================================================
// Post-Process Renderer
// =============================================================================
class PostProcessRenderer {
public:
    PostProcessRenderer() = default;
    ~PostProcessRenderer();

    // Initialize renderer with device and pipeline layout
    bool Initialize(RenderDevice* device, RenderPipelineLayout* pipelineLayout,
                    RenderDescriptorSet* textureDescriptorSet,
                    uint32_t displayWidth, uint32_t displayHeight);
    
    // Shutdown and release resources
    void Shutdown();
    
    // Check if initialized
    bool IsInitialized() const { return m_initialized; }
    
    // Resize buffers when display size changes
    void Resize(uint32_t displayWidth, uint32_t displayHeight);
    
    // =======================================================================
    // TAA Implementation
    // =======================================================================
    
    // Apply TAA to color buffer
    // colorTexture: current frame color (input)
    // depthTexture: current frame depth
    // motionTexture: motion vectors (RG = velocity)
    // outputTexture: TAA result (output)
    // Returns true if TAA was applied
    bool ApplyTAA(RenderCommandList* commandList,
                  RenderTexture* colorTexture,
                  RenderTexture* depthTexture,
                  RenderTexture* motionTexture,
                  RenderTexture* outputTexture,
                  float jitterX, float jitterY,
                  float prevJitterX, float prevJitterY,
                  bool resetHistory);
    
    // =======================================================================
    // SMAA Implementation
    // =======================================================================
    
    // Apply SMAA to color buffer
    // colorTexture: input color
    // outputTexture: SMAA result
    // Returns true if SMAA was applied
    bool ApplySMAA(RenderCommandList* commandList,
                   RenderTexture* colorTexture,
                   RenderTexture* outputTexture);
    
    // =======================================================================
    // FSR 1.0 Implementation
    // =======================================================================
    
    // Apply FSR 1.0 upscaling
    // inputTexture: low-res input
    // outputTexture: high-res output
    // inputWidth/Height: input dimensions
    // outputWidth/Height: output dimensions
    // sharpness: 0.0 = soft, 1.0 = sharp
    bool ApplyFSR1(RenderCommandList* commandList,
                   RenderTexture* inputTexture,
                   RenderTexture* outputTexture,
                   uint32_t inputWidth, uint32_t inputHeight,
                   uint32_t outputWidth, uint32_t outputHeight,
                   float sharpness);
    
    // Get TAA history buffer for external use
    RenderTexture* GetTAAHistoryBuffer() const { return m_taaHistoryBuffer.get(); }
    
    // Swap TAA history buffers (call after TAA pass)
    void SwapTAAHistory();
    
    // =======================================================================
    // Vignette Implementation
    // =======================================================================
    
    // Apply vignette effect to color buffer
    // colorTexture: input color
    // outputTexture: vignette result
    // Uses config values for intensity, radius, softness, roundness
    // Returns true if vignette was applied
    bool ApplyVignette(RenderCommandList* commandList,
                       RenderTexture* colorTexture,
                       RenderTexture* outputTexture,
                       uint32_t textureDescriptorIndex);
    
    // =======================================================================
    // SSAO (Screen-Space Ambient Occlusion) Implementation
    // =======================================================================
    
    // Apply SSAO to the scene
    // depthTexture: depth buffer
    // colorTexture: scene color (for composite)
    // outputTexture: SSAO result
    // cameraNear/cameraFar: camera clipping planes
    // cameraFovY: vertical field of view in radians
    // Returns true if SSAO was applied
    bool ApplySSAO(RenderCommandList* commandList,
                   RenderTexture* depthTexture,
                   RenderTexture* colorTexture,
                   RenderTexture* outputTexture,
                   float cameraNear, float cameraFar, float cameraFovY);
    
    // =======================================================================
    // Depth of Field Implementation
    // =======================================================================
    
    // Apply Depth of Field effect
    // colorTexture: input scene color
    // depthTexture: depth buffer
    // outputTexture: DoF result
    // focusDistance: distance to focus plane
    // apertureSize: lens aperture (affects blur strength)
    // cameraNear/cameraFar: camera clipping planes
    // Returns true if DoF was applied
    bool ApplyDoF(RenderCommandList* commandList,
                  RenderTexture* colorTexture,
                  RenderTexture* depthTexture,
                  RenderTexture* outputTexture,
                  float focusDistance, float apertureSize,
                  float cameraNear, float cameraFar);
    
    // =======================================================================
    // SSR (Screen-Space Reflections) Implementation
    // =======================================================================
    
    // Apply Screen-Space Reflections
    // colorTexture: input scene color
    // depthTexture: depth buffer
    // outputTexture: SSR composite result
    // cameraNear/cameraFar/cameraFovY: camera parameters
    // viewMatrix/projMatrix: camera matrices (row-major, 16 floats each)
    // Returns true if SSR was applied
    bool ApplySSR(RenderCommandList* commandList,
                  RenderTexture* colorTexture,
                  RenderTexture* depthTexture,
                  RenderTexture* outputTexture,
                  float cameraNear, float cameraFar, float cameraFovY,
                  const float* viewMatrix, const float* projMatrix);

private:
    bool CreateShaders();
    bool CreatePipelines();
    bool CreateRenderTargets(uint32_t width, uint32_t height);
    bool CreateSMAATables();
    
    void DrawFullscreenTriangle(RenderCommandList* commandList);
    
    bool m_initialized = false;
    RenderDevice* m_device = nullptr;
    RenderPipelineLayout* m_pipelineLayout = nullptr;
    RenderDescriptorSet* m_textureDescriptorSet = nullptr;
    
    uint32_t m_displayWidth = 0;
    uint32_t m_displayHeight = 0;
    
    // Shaders
    std::unique_ptr<RenderShader> m_fullscreenVS;
    std::unique_ptr<RenderShader> m_taaPS;
    std::unique_ptr<RenderShader> m_smaaEdgePS;
    std::unique_ptr<RenderShader> m_smaaBlendPS;
    std::unique_ptr<RenderShader> m_smaaNeighborhoodBlendPS;
    std::unique_ptr<RenderShader> m_fsr1EasuPS;
    std::unique_ptr<RenderShader> m_fsr1RcasPS;
    std::unique_ptr<RenderShader> m_vignettePS;
    std::unique_ptr<RenderShader> m_ssaoPS;
    std::unique_ptr<RenderShader> m_ssaoBlurPS;
    std::unique_ptr<RenderShader> m_ssaoCompositePS;
    std::unique_ptr<RenderShader> m_dofPS;
    std::unique_ptr<RenderShader> m_ssrRaytracePS;
    std::unique_ptr<RenderShader> m_ssrCompositePS;
    
    // Pipelines
    std::unique_ptr<RenderPipeline> m_taaPipeline;
    std::unique_ptr<RenderPipeline> m_smaaEdgePipeline;
    std::unique_ptr<RenderPipeline> m_smaaBlendPipeline;
    std::unique_ptr<RenderPipeline> m_smaaNeighborhoodBlendPipeline;
    std::unique_ptr<RenderPipeline> m_fsr1EasuPipeline;
    std::unique_ptr<RenderPipeline> m_fsr1RcasPipeline;
    std::unique_ptr<RenderPipeline> m_vignettePipeline;
    std::unique_ptr<RenderPipeline> m_ssaoPipeline;
    std::unique_ptr<RenderPipeline> m_ssaoBlurPipeline;
    std::unique_ptr<RenderPipeline> m_ssaoCompositePipeline;
    std::unique_ptr<RenderPipeline> m_dofPipeline;
    std::unique_ptr<RenderPipeline> m_ssrRaytracePipeline;
    std::unique_ptr<RenderPipeline> m_ssrCompositePipeline;
    
    // TAA history buffers (double-buffered)
    std::unique_ptr<RenderTexture> m_taaHistoryBuffer;
    std::unique_ptr<RenderTexture> m_taaHistoryBufferPrev;
    std::unique_ptr<RenderFramebuffer> m_taaFramebuffer;
    uint32_t m_taaHistoryDescriptorIndex = 0;
    uint32_t m_taaHistoryPrevDescriptorIndex = 0;
    
    // SMAA intermediate buffers
    std::unique_ptr<RenderTexture> m_smaaEdgeBuffer;
    std::unique_ptr<RenderTexture> m_smaaBlendBuffer;
    std::unique_ptr<RenderFramebuffer> m_smaaEdgeFramebuffer;
    std::unique_ptr<RenderFramebuffer> m_smaaBlendFramebuffer;
    uint32_t m_smaaEdgeDescriptorIndex = 0;
    uint32_t m_smaaBlendDescriptorIndex = 0;
    
    // SMAA lookup tables (area and search textures)
    std::unique_ptr<RenderTexture> m_smaaAreaTex;
    std::unique_ptr<RenderTexture> m_smaaSearchTex;
    uint32_t m_smaaAreaDescriptorIndex = 0;
    uint32_t m_smaaSearchDescriptorIndex = 0;
    
    // FSR 1.0 intermediate buffer (for EASU output before RCAS)
    std::unique_ptr<RenderTexture> m_fsr1IntermediateBuffer;
    std::unique_ptr<RenderFramebuffer> m_fsr1IntermediateFramebuffer;
    uint32_t m_fsr1IntermediateDescriptorIndex = 0;
    
    // Samplers
    std::unique_ptr<RenderSampler> m_linearSampler;
    std::unique_ptr<RenderSampler> m_pointSampler;
    
    // Constant buffers for shader parameters
    std::unique_ptr<RenderBuffer> m_taaConstantBuffer;
    std::unique_ptr<RenderBuffer> m_smaaConstantBuffer;
    std::unique_ptr<RenderBuffer> m_fsr1ConstantBuffer;
    std::unique_ptr<RenderBuffer> m_ssaoConstantBuffer;
    std::unique_ptr<RenderBuffer> m_ssaoBlurConstantBuffer;
    std::unique_ptr<RenderBuffer> m_dofConstantBuffer;
    
    // SSAO intermediate buffers
    std::unique_ptr<RenderTexture> m_ssaoBuffer;           // Raw AO output
    std::unique_ptr<RenderTexture> m_ssaoBlurBuffer;       // Horizontally blurred AO
    std::unique_ptr<RenderFramebuffer> m_ssaoFramebuffer;
    std::unique_ptr<RenderFramebuffer> m_ssaoBlurFramebuffer;
    
    // DoF intermediate buffer
    std::unique_ptr<RenderTexture> m_dofBuffer;
    std::unique_ptr<RenderFramebuffer> m_dofFramebuffer;
    
    // SSR intermediate buffers
    std::unique_ptr<RenderTexture> m_ssrBuffer;           // Reflection result
    std::unique_ptr<RenderFramebuffer> m_ssrFramebuffer;
    std::unique_ptr<RenderBuffer> m_ssrConstantBuffer;
    std::unique_ptr<RenderBuffer> m_ssrCompositeConstantBuffer;
    
    // Blue noise texture for SSAO temporal jitter
    std::unique_ptr<RenderTexture> m_blueNoiseTex;
    
    // Frame counter for history management
    uint32_t m_frameIndex = 0;
};

// Global post-process renderer instance
extern PostProcessRenderer g_postProcessRenderer;

// Initialize the global renderer (called from Video::CreateHostDevice)
bool InitializePostProcessRenderer(RenderDevice* device, RenderPipelineLayout* pipelineLayout,
                                    RenderDescriptorSet* textureDescriptorSet,
                                    uint32_t displayWidth, uint32_t displayHeight);

// Shutdown the global renderer
void ShutdownPostProcessRenderer();

} // namespace PostProcess
