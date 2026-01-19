// =============================================================================
// GTA IV Render State Implementation
// Extracts render state from device context based on RENDERING_RESEARCH.md
// =============================================================================

#include "gtaiv_render_state.h"
#include "video.h"
#include <kernel/memory.h>
#include <cstdio>

extern Memory g_memory;

namespace GTAIV {

// =============================================================================
// RenderState::Extract - Extract complete state from device context
// =============================================================================
RenderState RenderState::Extract(uint8_t* device) {
    RenderState state = {};
    
    if (device == nullptr) return state;

    // Dirty Flags (Section 2.2)
    state.dirtyFlags = GetDeviceU64(device, DeviceOffset::DirtyFlags);
    state.shaderFlags = GetDeviceU8(device, DeviceOffset::ShaderFlags);
    state.drawFlags = GetDeviceU8(device, DeviceOffset::DrawFlags);
    state.commandFlags = GetDeviceU8(device, DeviceOffset::CommandFlags);

    // Shader State (Section 2.3)
    state.vertexShaderHandle = GetDeviceU32(device, DeviceOffset::CurrentVertexShader);
    state.pixelShaderHandle = GetDeviceU32(device, DeviceOffset::CurrentPixelShader);
    state.shaderValid = GetDeviceU32(device, DeviceOffset::ShaderValidFlag) != 0;

    // Vertex Input State (Section 2.4)
    state.streamSource[0] = GetDeviceU32(device, DeviceOffset::StreamSource0);
    state.streamSource[1] = GetDeviceU32(device, DeviceOffset::StreamSource1);
    state.streamSource[2] = GetDeviceU32(device, DeviceOffset::StreamSource2);
    state.streamSource[3] = GetDeviceU32(device, DeviceOffset::StreamSource3);
    state.vertexDeclaration = GetDeviceU32(device, DeviceOffset::VertexDeclaration);
    state.vertexFormatFlags = GetDeviceU32(device, DeviceOffset::VertexFormatFlags);
    state.indexBuffer = GetDeviceU32(device, DeviceOffset::IndexBuffer);
    state.indexBufferBase = GetDeviceU32(device, DeviceOffset::IndexBufferBase);

    // Detect index format (from sub_829D4EE0)
    uint32_t ibState = state.indexBuffer;
    switch (ibState & 0xFF) {
        case 0:
        case 1:  state.indexFormat = IndexFormat::Index16Bit; break;
        case 2:  state.indexFormat = IndexFormat::Index32BitLE; break;
        case 4:  state.indexFormat = IndexFormat::Index32BitBE; break;
        default:
            if (ibState == 0x80000000) 
                state.indexFormat = IndexFormat::GpuDefault;
            else 
                state.indexFormat = IndexFormat::Index16Bit;
            break;
    }

    // Render Target State (Section 2.5)
    for (int i = 0; i < 4; i++) {
        state.renderTarget[i] = GetDeviceU32(device, DeviceOffset::RenderTargetSlots + i * 4);
    }
    state.depthStencil = GetDeviceU32(device, DeviceOffset::DepthStencilTarget);

    // Texture State (Section 2.6) - 19 texture slots
    for (int i = 0; i < 19; i++) {
        state.textureSlots[i] = GetDeviceU32(device, DeviceOffset::TextureSlots + i * 4);
    }

    // Blend State
    state.blendState = GetDeviceU32(device, DeviceOffset::BlendState);

    // EDRAM / HDR state - Xbox 360 10-bit internal framebuffer
    state.edramBase = GetDeviceU32(device, DeviceOffsetExt::EdramBase);
    state.edramPitch = GetDeviceU32(device, DeviceOffsetExt::EdramPitch);
    state.surfaceFormat = static_cast<XenosSurfaceFormat>(GetDeviceU32(device, DeviceOffsetExt::SurfaceFormat));

    // Frame state
    state.frameCounter = GetDeviceU32(device, DeviceOffset::FrameCounter);
    state.frameBufferIndex = GetDeviceU32(device, DeviceOffset::FrameBufferIndex);

    return state;
}

// =============================================================================
// SamplerState::Extract - Extract sampler state for a texture slot
// Each sampler descriptor is 24 bytes at device+1152 + slot*24
// =============================================================================
SamplerState SamplerState::Extract(uint8_t* device, uint32_t slot) {
    SamplerState state = {};
    
    if (device == nullptr || slot >= 19) return state;

    uint32_t base = DeviceOffset::SamplerDescs + slot * 24;

    // Sampler descriptor layout (24 bytes per sampler):
    // +0:  Address modes (U, V, W packed)
    // +4:  Filter modes (min, mag, mip packed)  
    // +8:  LOD bias
    // +12: Min LOD
    // +16: Max LOD
    // +20: Border color / anisotropy

    uint32_t addressModes = GetDeviceU32(device, base + 0);
    uint32_t filterModes = GetDeviceU32(device, base + 4);
    uint32_t lodBias = GetDeviceU32(device, base + 8);
    uint32_t minLod = GetDeviceU32(device, base + 12);
    uint32_t maxLod = GetDeviceU32(device, base + 16);
    uint32_t misc = GetDeviceU32(device, base + 20);

    // Decode address modes (bits 10-12 = U, 13-15 = V, 16-18 = W)
    state.addressU = (addressModes >> 10) & 0x7;
    state.addressV = (addressModes >> 13) & 0x7;
    state.addressW = (addressModes >> 16) & 0x7;

    // Decode filter modes (bits 0-2 = min, 3-5 = mag, 23-24 = mip)
    state.minFilter = filterModes & 0x7;
    state.magFilter = (filterModes >> 3) & 0x7;
    state.mipFilter = (filterModes >> 23) & 0x3;

    // LOD values (convert from fixed point)
    state.mipLodBias = *reinterpret_cast<float*>(&lodBias);
    state.minLod = *reinterpret_cast<float*>(&minLod);
    state.maxLod = *reinterpret_cast<float*>(&maxLod);

    // Anisotropy (bits 0-3) and border color (bits 4-5)
    state.maxAnisotropy = 1 << (misc & 0xF);  // 2^n
    state.borderColor = (misc >> 4) & 0x3;

    return state;
}

// =============================================================================
// Global State Tracking
// =============================================================================
static RenderState g_lastState = {};
static uint32_t g_drawCallCount = 0;
static uint32_t g_stateChangeCount = 0;
static bool g_hdrPassthroughEnabled = false;

// =============================================================================
// HDR 10-bit Framebuffer Passthrough
// Xbox 360 uses EDRAM with 10-bit (R10G10B10A2) internal format
// =============================================================================
void EnableHDRPassthrough(bool enable) {
    g_hdrPassthroughEnabled = enable;
}

bool IsHDRPassthroughEnabled() {
    return g_hdrPassthroughEnabled;
}

// Determine if current render target uses 10-bit HDR
bool IsCurrentRenderTargetHDR(uint8_t* device) {
    if (device == nullptr) return false;
    
    XenosSurfaceFormat fmt = static_cast<XenosSurfaceFormat>(
        GetDeviceU32(device, DeviceOffsetExt::SurfaceFormat));
    
    return fmt == XenosSurfaceFormat::R10G10B10A2 || 
           fmt == XenosSurfaceFormat::R16G16B16A16_F;
}

// Get the internal EDRAM format for HDR passthrough
XenosSurfaceFormat GetEDRAMFormat(uint8_t* device) {
    if (device == nullptr) return XenosSurfaceFormat::R8G8B8A8;
    return static_cast<XenosSurfaceFormat>(GetDeviceU32(device, DeviceOffsetExt::SurfaceFormat));
}

// =============================================================================
// State Change Detection
// Returns bitmask of what changed since last draw
// =============================================================================
uint64_t DetectStateChanges(const RenderState& current) {
    uint64_t changes = 0;

    if (current.vertexShaderHandle != g_lastState.vertexShaderHandle)
        changes |= DirtyBit::VertexShader;
    
    if (current.pixelShaderHandle != g_lastState.pixelShaderHandle)
        changes |= DirtyBit::PixelShader;

    for (int i = 0; i < 4; i++) {
        if (current.renderTarget[i] != g_lastState.renderTarget[i]) {
            changes |= DirtyBit::RenderTarget;
            break;
        }
    }

    if (current.depthStencil != g_lastState.depthStencil)
        changes |= DirtyBit::DepthStencil;

    for (int i = 0; i < 4; i++) {
        if (current.streamSource[i] != g_lastState.streamSource[i]) {
            changes |= DirtyBit::VertexBuffers;
            break;
        }
    }

    if (current.indexBuffer != g_lastState.indexBuffer)
        changes |= DirtyBit::IndexBuffer;

    for (int i = 0; i < 19; i++) {
        if (current.textureSlots[i] != g_lastState.textureSlots[i]) {
            changes |= DirtyBit::Textures;
            break;
        }
    }

    if (current.blendState != g_lastState.blendState)
        changes |= DirtyBit::BlendState;

    if (current.vertexDeclaration != g_lastState.vertexDeclaration)
        changes |= DirtyBit::VertexDecl;

    return changes;
}

// =============================================================================
// Update tracked state after draw
// =============================================================================
void CommitState(const RenderState& state) {
    g_lastState = state;
    g_drawCallCount++;
}

// =============================================================================
// Statistics
// =============================================================================
void GetRenderStats(uint32_t& drawCalls, uint32_t& stateChanges) {
    drawCalls = g_drawCallCount;
    stateChanges = g_stateChangeCount;
}

void ResetRenderStats() {
    g_drawCallCount = 0;
    g_stateChangeCount = 0;
}

// =============================================================================
// Debug Logging
// =============================================================================
void LogRenderState(const RenderState& state, const char* context) {
    static uint32_t logCount = 0;
    if (++logCount > 100) return;  // Limit logging

    printf("[GTAIV::%s] VS=0x%08X PS=0x%08X Valid=%d\n",
           context, state.vertexShaderHandle, state.pixelShaderHandle, state.shaderValid);
    printf("  VB[0-3]={0x%08X, 0x%08X, 0x%08X, 0x%08X}\n",
           state.streamSource[0], state.streamSource[1], 
           state.streamSource[2], state.streamSource[3]);
    printf("  IB=0x%08X RT[0]=0x%08X DS=0x%08X\n",
           state.indexBuffer, state.renderTarget[0], state.depthStencil);
    printf("  EDRAM: base=0x%08X pitch=%u fmt=0x%02X (HDR=%s)\n",
           state.edramBase, state.edramPitch, 
           static_cast<uint32_t>(state.surfaceFormat),
           state.IsHDR10Bit() ? "10-bit" : (state.IsHDR16Bit() ? "16-bit" : "8-bit"));
}

} // namespace GTAIV
