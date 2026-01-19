// =============================================================================
// GTA IV Renderer - Resource Handle Translation
// Maps guest memory addresses to host resource objects
// Based on RENDERING_RESEARCH.md analysis
// =============================================================================

#include "gfx_state.h"
#include "video.h"
#include <unordered_map>
#include <shared_mutex>
#include <mutex>
#include <cstdio>

namespace GTAIV {

// =============================================================================
// Shader Handle Translation
// GTA IV stores shader handles as guest memory addresses. We need to translate
// these to host GuestShader* objects.
// =============================================================================

static std::shared_mutex s_shaderMapMutex;
static std::unordered_map<uint32_t, GuestShader*> s_shaderHandleMap;

void RegisterShader(uint32_t guestHandle, GuestShader* hostShader) {
    if (guestHandle == 0 || hostShader == nullptr) return;
    std::unique_lock<std::shared_mutex> lock(s_shaderMapMutex);
    s_shaderHandleMap[guestHandle] = hostShader;
}

GuestShader* LookupShader(uint32_t guestHandle) {
    if (guestHandle == 0 || guestHandle == 0xFFFFFFFF) return nullptr;
    std::shared_lock<std::shared_mutex> lock(s_shaderMapMutex);  // Read lock - allows concurrent reads
    auto it = s_shaderHandleMap.find(guestHandle);
    return (it != s_shaderHandleMap.end()) ? it->second : nullptr;
}

void UnregisterShader(uint32_t guestHandle) {
    if (guestHandle == 0) return;
    std::unique_lock<std::shared_mutex> lock(s_shaderMapMutex);
    s_shaderHandleMap.erase(guestHandle);
}

// =============================================================================
// Buffer Handle Translation
// =============================================================================

static std::shared_mutex s_bufferMapMutex;
static std::unordered_map<uint32_t, GuestBuffer*> s_bufferHandleMap;

void RegisterBuffer(uint32_t guestHandle, GuestBuffer* hostBuffer) {
    if (guestHandle == 0 || hostBuffer == nullptr) return;
    std::unique_lock<std::shared_mutex> lock(s_bufferMapMutex);
    s_bufferHandleMap[guestHandle] = hostBuffer;
}

GuestBuffer* LookupBuffer(uint32_t guestHandle) {
    if (guestHandle == 0) return nullptr;
    std::shared_lock<std::shared_mutex> lock(s_bufferMapMutex);  // Read lock
    auto it = s_bufferHandleMap.find(guestHandle);
    return (it != s_bufferHandleMap.end()) ? it->second : nullptr;
}

void UnregisterBuffer(uint32_t guestHandle) {
    if (guestHandle == 0) return;
    std::unique_lock<std::shared_mutex> lock(s_bufferMapMutex);
    s_bufferHandleMap.erase(guestHandle);
}

// =============================================================================
// Texture Handle Translation
// =============================================================================

static std::shared_mutex s_textureMapMutex;
static std::unordered_map<uint32_t, GuestTexture*> s_textureHandleMap;

void RegisterTexture(uint32_t guestHandle, GuestTexture* hostTexture) {
    if (guestHandle == 0 || hostTexture == nullptr) return;
    std::unique_lock<std::shared_mutex> lock(s_textureMapMutex);
    s_textureHandleMap[guestHandle] = hostTexture;
}

GuestTexture* LookupTexture(uint32_t guestHandle) {
    if (guestHandle == 0) return nullptr;
    std::shared_lock<std::shared_mutex> lock(s_textureMapMutex);  // Read lock
    auto it = s_textureHandleMap.find(guestHandle);
    return (it != s_textureHandleMap.end()) ? it->second : nullptr;
}

void UnregisterTexture(uint32_t guestHandle) {
    if (guestHandle == 0) return;
    std::unique_lock<std::shared_mutex> lock(s_textureMapMutex);
    s_textureHandleMap.erase(guestHandle);
}

// =============================================================================
// Surface Handle Translation
// =============================================================================

static std::shared_mutex s_surfaceMapMutex;
static std::unordered_map<uint32_t, GuestSurface*> s_surfaceHandleMap;

void RegisterSurface(uint32_t guestHandle, GuestSurface* hostSurface) {
    if (guestHandle == 0 || hostSurface == nullptr) return;
    std::unique_lock<std::shared_mutex> lock(s_surfaceMapMutex);
    s_surfaceHandleMap[guestHandle] = hostSurface;
}

GuestSurface* LookupSurface(uint32_t guestHandle) {
    if (guestHandle == 0) return nullptr;
    std::shared_lock<std::shared_mutex> lock(s_surfaceMapMutex);  // Read lock
    auto it = s_surfaceHandleMap.find(guestHandle);
    return (it != s_surfaceHandleMap.end()) ? it->second : nullptr;
}

void UnregisterSurface(uint32_t guestHandle) {
    if (guestHandle == 0) return;
    std::unique_lock<std::shared_mutex> lock(s_surfaceMapMutex);
    s_surfaceHandleMap.erase(guestHandle);
}

// =============================================================================
// Resource Statistics - Using std::scoped_lock to prevent deadlock
// =============================================================================

ResourceStats GetResourceStats() {
    // Use scoped_lock to acquire all mutexes atomically - prevents deadlock
    std::scoped_lock lock(s_shaderMapMutex, s_bufferMapMutex, s_textureMapMutex, s_surfaceMapMutex);
    
    ResourceStats stats;
    stats.shaderCount = static_cast<uint32_t>(s_shaderHandleMap.size());
    stats.bufferCount = static_cast<uint32_t>(s_bufferHandleMap.size());
    stats.textureCount = static_cast<uint32_t>(s_textureHandleMap.size());
    stats.surfaceCount = static_cast<uint32_t>(s_surfaceHandleMap.size());
    return stats;
}

// =============================================================================
// Resource Cleanup
// =============================================================================

void ClearAllResources() {
    // Use scoped_lock to acquire all mutexes atomically - prevents deadlock
    std::scoped_lock lock(s_shaderMapMutex, s_bufferMapMutex, s_textureMapMutex, s_surfaceMapMutex);
    
    s_shaderHandleMap.clear();
    s_bufferHandleMap.clear();
    s_textureHandleMap.clear();
    s_surfaceHandleMap.clear();
}

// =============================================================================
// Draw Call State Management
// =============================================================================

struct DrawCallState {
    D3DPrimitiveType primitiveType;
    uint32_t startVertex;
    uint32_t vertexCount;
    uint32_t startIndex;
    uint32_t indexCount;
    uint32_t baseVertex;
    uint32_t instanceCount;
    bool indexed;
    bool instanced;
};

static DrawCallState s_currentDrawCall = {};
static uint64_t s_totalDrawCalls = 0;
static uint64_t s_totalTriangles = 0;
static uint64_t s_frameDrawCalls = 0;
static uint64_t s_frameTriangles = 0;

void BeginDrawCall(D3DPrimitiveType primitiveType, bool indexed, bool instanced) {
    s_currentDrawCall = {};
    s_currentDrawCall.primitiveType = primitiveType;
    s_currentDrawCall.indexed = indexed;
    s_currentDrawCall.instanced = instanced;
    s_currentDrawCall.instanceCount = 1;
}

void SetDrawCallVertices(uint32_t startVertex, uint32_t vertexCount) {
    s_currentDrawCall.startVertex = startVertex;
    s_currentDrawCall.vertexCount = vertexCount;
}

void SetDrawCallIndices(uint32_t startIndex, uint32_t indexCount, uint32_t baseVertex) {
    s_currentDrawCall.startIndex = startIndex;
    s_currentDrawCall.indexCount = indexCount;
    s_currentDrawCall.baseVertex = baseVertex;
}

void SetDrawCallInstances(uint32_t instanceCount) {
    s_currentDrawCall.instanceCount = instanceCount;
    s_currentDrawCall.instanced = instanceCount > 1;
}

// Calculate triangle count from primitive type and vertex/index count
static uint32_t CalculateTriangleCount(D3DPrimitiveType primType, uint32_t count) {
    switch (primType) {
        case D3DPT_TRIANGLELIST:
            return count / 3;
        case D3DPT_TRIANGLESTRIP:
        case D3DPT_TRIANGLEFAN:
            return (count > 2) ? count - 2 : 0;
        case D3DPT_QUADLIST:
            return (count / 4) * 2;  // Each quad = 2 triangles
        case D3DPT_RECTLIST:
            return (count / 3) * 2;  // Xbox 360 rect = 2 triangles
        default:
            return 0;
    }
}

void EndDrawCall() {
    s_totalDrawCalls++;
    s_frameDrawCalls++;
    
    // Track draw calls per render pass
    s_frameDrawCalls++;
    
    // Estimate triangle count based on primitive type
    uint32_t count = s_currentDrawCall.indexed ? 
                     s_currentDrawCall.indexCount : 
                     s_currentDrawCall.vertexCount;
    
    uint32_t triangles = CalculateTriangleCount(s_currentDrawCall.primitiveType, count);
    triangles *= s_currentDrawCall.instanceCount;
    
    s_totalTriangles += triangles;
    s_frameTriangles += triangles;
}

void ResetFrameStats() {
    s_frameDrawCalls = 0;
    s_frameTriangles = 0;
}

DrawCallStats GetDrawCallStats() {
    DrawCallStats stats;
    stats.totalDrawCalls = s_totalDrawCalls;
    stats.totalTriangles = s_totalTriangles;
    stats.frameDrawCalls = s_frameDrawCalls;
    stats.frameTriangles = s_frameTriangles;
    return stats;
}

// =============================================================================
// Render Pass Management
// =============================================================================

static RenderPassType s_currentPass = RenderPassType::Unknown;
static uint32_t s_passDrawCalls = 0;

void BeginRenderPass(RenderPassType passType) {
    s_currentPass = passType;
    s_passDrawCalls = 0;
}

void EndRenderPass() {
    s_currentPass = RenderPassType::Unknown;
}

RenderPassType GetCurrentRenderPass() {
    return s_currentPass;
}

uint32_t GetCurrentPassDrawCalls() {
    return s_passDrawCalls;
}

const char* GetRenderPassTypeName(RenderPassType passType) {
    switch (passType) {
        case RenderPassType::Unknown:     return "Unknown";
        case RenderPassType::GBuffer:     return "GBuffer";
        case RenderPassType::Shadow:      return "Shadow";
        case RenderPassType::Lighting:    return "Lighting";
        case RenderPassType::PostProcess: return "PostProcess";
        case RenderPassType::UI:          return "UI";
        case RenderPassType::Debug:       return "Debug";
        case RenderPassType::Water:       return "Water";
        case RenderPassType::Reflection:  return "Reflection";
        case RenderPassType::Transparent: return "Transparent";
        default:                          return "Invalid";
    }
}

// =============================================================================
// Pipeline State Cache
// =============================================================================

bool PipelineStateKey::operator==(const PipelineStateKey& other) const {
    return vertexShaderHash == other.vertexShaderHash &&
           pixelShaderHash == other.pixelShaderHash &&
           vertexDeclHash == other.vertexDeclHash &&
           blendStateHash == other.blendStateHash &&
           depthStateHash == other.depthStateHash &&
           rasterStateHash == other.rasterStateHash &&
           renderTargetFormat == other.renderTargetFormat &&
           depthFormat == other.depthFormat;
}

struct PipelineStateKeyHash {
    size_t operator()(const PipelineStateKey& k) const {
        // Use boost::hash_combine-style mixing with golden ratio constant
        size_t h = 0;
        h ^= std::hash<uint64_t>{}(k.vertexShaderHash) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<uint64_t>{}(k.pixelShaderHash) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<uint32_t>{}(k.vertexDeclHash) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<uint32_t>{}(k.blendStateHash) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<uint32_t>{}(k.depthStateHash) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<uint32_t>{}(k.rasterStateHash) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<uint32_t>{}(k.renderTargetFormat) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<uint32_t>{}(k.depthFormat) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

static std::mutex s_pipelineCacheMutex;
static std::unordered_map<PipelineStateKey, void*, PipelineStateKeyHash> s_pipelineCache;
static uint64_t s_pipelineCacheHits = 0;
static uint64_t s_pipelineCacheMisses = 0;

void* LookupPipelineState(const PipelineStateKey& key) {
    std::lock_guard<std::mutex> lock(s_pipelineCacheMutex);
    auto it = s_pipelineCache.find(key);
    if (it != s_pipelineCache.end()) {
        s_pipelineCacheHits++;
        return it->second;
    }
    s_pipelineCacheMisses++;
    return nullptr;
}

void CachePipelineState(const PipelineStateKey& key, void* pipeline) {
    std::lock_guard<std::mutex> lock(s_pipelineCacheMutex);
    s_pipelineCache[key] = pipeline;
}

PipelineCacheStats GetPipelineCacheStats() {
    std::lock_guard<std::mutex> lock(s_pipelineCacheMutex);
    PipelineCacheStats stats;
    stats.hits = s_pipelineCacheHits;
    stats.misses = s_pipelineCacheMisses;
    stats.entries = s_pipelineCache.size();
    return stats;
}

void RecordPipelineCacheHit() {
    s_pipelineCacheHits++;
}

void RecordPipelineCacheMiss() {
    s_pipelineCacheMisses++;
}

void ClearPipelineCache() {
    std::lock_guard<std::mutex> lock(s_pipelineCacheMutex);
    s_pipelineCache.clear();
    s_pipelineCacheHits = 0;
    s_pipelineCacheMisses = 0;
}

// =============================================================================
// System Lifecycle
// =============================================================================

void InitializeGfxState() {
    // Clear all resource maps
    ClearAllResources();
    
    // Reset draw call stats
    s_totalDrawCalls = 0;
    s_totalTriangles = 0;
    s_frameDrawCalls = 0;
    s_frameTriangles = 0;
    
    // Reset render pass state
    s_currentPass = RenderPassType::Unknown;
    s_passDrawCalls = 0;
    
    // Clear pipeline cache
    ClearPipelineCache();
}

void ShutdownGfxState() {
    ClearAllResources();
    ClearPipelineCache();
}

} // namespace GTAIV
