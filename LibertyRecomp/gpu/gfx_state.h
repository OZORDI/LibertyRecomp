#pragma once

#include <cstdint>
#include <cstddef>

namespace GTAIV {

// =============================================================================
// Forward Declarations for Guest Resource Types
// =============================================================================

struct GuestShader;
struct GuestBuffer;
struct GuestTexture;
struct GuestSurface;

// =============================================================================
// D3D Primitive Types (Xbox 360 / D3D9 compatible)
// =============================================================================

enum D3DPrimitiveType : uint32_t {
    D3DPT_POINTLIST     = 1,
    D3DPT_LINELIST      = 2,
    D3DPT_LINESTRIP     = 3,
    D3DPT_TRIANGLELIST  = 4,
    D3DPT_TRIANGLESTRIP = 5,
    D3DPT_TRIANGLEFAN   = 6,
    D3DPT_QUADLIST      = 13,  // Xbox 360 extension
    D3DPT_RECTLIST      = 14,  // Xbox 360 extension
};

// =============================================================================
// Render Pass Types
// =============================================================================

enum class RenderPassType : uint32_t {
    Unknown,
    GBuffer,        // Geometry buffer pass (deferred rendering)
    Shadow,         // Shadow map generation
    Lighting,       // Lighting calculations
    PostProcess,    // Post-processing effects
    UI,             // User interface rendering
    Debug,          // Debug visualization
    Water,          // Water rendering
    Reflection,     // Reflection/cubemap passes
    Transparent,    // Transparent/alpha objects
};

// =============================================================================
// Shader Handle Translation
// GTA IV stores shader handles as guest memory addresses. We need to translate
// these to host GuestShader* objects.
// =============================================================================

void RegisterShader(uint32_t guestHandle, GuestShader* hostShader);
void UnregisterShader(uint32_t guestHandle);
GuestShader* LookupShader(uint32_t guestHandle);

// =============================================================================
// Buffer Handle Translation
// =============================================================================

void RegisterBuffer(uint32_t guestHandle, GuestBuffer* hostBuffer);
void UnregisterBuffer(uint32_t guestHandle);
GuestBuffer* LookupBuffer(uint32_t guestHandle);

// =============================================================================
// Texture Handle Translation
// =============================================================================

void RegisterTexture(uint32_t guestHandle, GuestTexture* hostTexture);
void UnregisterTexture(uint32_t guestHandle);
GuestTexture* LookupTexture(uint32_t guestHandle);

// =============================================================================
// Surface Handle Translation
// =============================================================================

void RegisterSurface(uint32_t guestHandle, GuestSurface* hostSurface);
void UnregisterSurface(uint32_t guestHandle);
GuestSurface* LookupSurface(uint32_t guestHandle);

// =============================================================================
// Resource Statistics
// =============================================================================

struct ResourceStats {
    uint32_t shaderCount;
    uint32_t bufferCount;
    uint32_t textureCount;
    uint32_t surfaceCount;
};

ResourceStats GetResourceStats();
void ClearAllResources();

// =============================================================================
// Draw Call State Management
// =============================================================================

void BeginDrawCall(D3DPrimitiveType primitiveType, bool indexed, bool instanced = false);
void SetDrawCallVertices(uint32_t startVertex, uint32_t vertexCount);
void SetDrawCallIndices(uint32_t startIndex, uint32_t indexCount, uint32_t baseVertex = 0);
void SetDrawCallInstances(uint32_t instanceCount);
void EndDrawCall();

// =============================================================================
// Draw Call Statistics
// =============================================================================

struct DrawCallStats {
    uint64_t totalDrawCalls;
    uint64_t totalTriangles;
    uint64_t frameDrawCalls;
    uint64_t frameTriangles;
};

DrawCallStats GetDrawCallStats();
void ResetFrameStats();

// =============================================================================
// Render Pass Management
// =============================================================================

void BeginRenderPass(RenderPassType passType);
void EndRenderPass();
RenderPassType GetCurrentRenderPass();
uint32_t GetCurrentPassDrawCalls();

// Get string name for render pass type (for debugging/logging)
const char* GetRenderPassTypeName(RenderPassType passType);

// =============================================================================
// Pipeline State Cache
// =============================================================================

struct PipelineStateKey {
    uint64_t vertexShaderHash;
    uint64_t pixelShaderHash;
    uint32_t vertexDeclHash;
    uint32_t blendStateHash;
    uint32_t depthStateHash;
    uint32_t rasterStateHash;
    uint32_t renderTargetFormat;
    uint32_t depthFormat;
    
    bool operator==(const PipelineStateKey& other) const;
};

void* LookupPipelineState(const PipelineStateKey& key);
void CachePipelineState(const PipelineStateKey& key, void* pipeline);

struct PipelineCacheStats {
    uint64_t hits;
    uint64_t misses;
    size_t entries;
};

PipelineCacheStats GetPipelineCacheStats();
void ClearPipelineCache();

// =============================================================================
// System Lifecycle
// =============================================================================

void InitializeGfxState();
void ShutdownGfxState();

} // namespace GTAIV
