// =============================================================================
// GTA IV Render State - Extended State Extraction
// Based on RENDERING_RESEARCH.md - extends gtaiv_device.h with HDR/EDRAM support
// =============================================================================
#pragma once

#include "gtaiv_device.h"

namespace GTAIV {

// =============================================================================
// Extended Device Offsets (EDRAM / HDR not in gtaiv_device.h)
// =============================================================================
namespace DeviceOffsetExt {
    // Xbox 360 EDRAM / 10-bit HDR framebuffer
    constexpr uint32_t EdramBase            = 1784;    // EDRAM tile base
    constexpr uint32_t EdramPitch           = 1788;    // EDRAM pitch
    constexpr uint32_t SurfaceFormat        = 1792;    // Surface format (10-bit = 0x1A)
}

// =============================================================================
// Dirty Flag Bits (from device[16] 64-bit bitmask)
// =============================================================================
namespace DirtyBit {
    constexpr uint64_t VertexShader         = (1ULL << 0);
    constexpr uint64_t PixelShader          = (1ULL << 1);
    constexpr uint64_t RenderTarget         = (1ULL << 2);
    constexpr uint64_t DepthStencil         = (1ULL << 3);
    constexpr uint64_t Viewport             = (1ULL << 4);
    constexpr uint64_t ScissorRect          = (1ULL << 5);
    constexpr uint64_t BlendState           = (1ULL << 6);
    constexpr uint64_t DepthState           = (1ULL << 7);
    constexpr uint64_t RasterState          = (1ULL << 8);
    constexpr uint64_t VertexBuffers        = (1ULL << 9);
    constexpr uint64_t IndexBuffer          = (1ULL << 10);
    constexpr uint64_t Textures             = (1ULL << 11);
    constexpr uint64_t Samplers             = (1ULL << 12);
    constexpr uint64_t VertexDecl           = (1ULL << 13);
    constexpr uint64_t Constants            = (1ULL << 56);  // From rldicr instruction
}

// =============================================================================
// Xbox 360 Surface Formats (EDRAM / 10-bit HDR)
// =============================================================================
enum class XenosSurfaceFormat : uint32_t {
    R8G8B8A8        = 0x06,  // Standard 8-bit RGBA
    R10G10B10A2     = 0x1A,  // 10-bit HDR (Xbox 360 internal)
    R16G16B16A16_F  = 0x1F,  // 16-bit float HDR
    D24S8           = 0x28,  // Depth 24 + Stencil 8
    D24FS8          = 0x29,  // Depth 24 float + Stencil 8
};

// PrimitiveType and IndexFormat are defined in gtaiv_device.h
// Memory access helpers (GetDeviceU32, etc.) are defined in gtaiv_device.h

// =============================================================================
// Extracted Render State Structure
// Complete state extracted from device context for a draw call
// =============================================================================
struct RenderState {
    // Shaders
    uint32_t vertexShaderHandle;
    uint32_t pixelShaderHandle;
    bool shaderValid;

    // Vertex Buffers (4 stream sources)
    uint32_t streamSource[4];
    uint32_t vertexDeclaration;
    uint32_t vertexFormatFlags;

    // Index Buffer
    uint32_t indexBuffer;
    uint32_t indexBufferBase;
    IndexFormat indexFormat;

    // Render Targets
    uint32_t renderTarget[4];
    uint32_t depthStencil;

    // Textures (19 slots)
    uint32_t textureSlots[19];

    // Blend/Depth/Raster State
    uint32_t blendState;

    // Dirty Flags
    uint64_t dirtyFlags;
    uint8_t shaderFlags;
    uint8_t drawFlags;
    uint8_t commandFlags;

    // EDRAM / HDR state
    uint32_t edramBase;
    uint32_t edramPitch;
    XenosSurfaceFormat surfaceFormat;

    // Frame state
    uint32_t frameCounter;
    uint32_t frameBufferIndex;

    // Extract complete state from device context
    static RenderState Extract(uint8_t* device);
    
    // Check if specific state is dirty
    bool IsVertexShaderDirty() const { return dirtyFlags & DirtyBit::VertexShader; }
    bool IsPixelShaderDirty() const { return dirtyFlags & DirtyBit::PixelShader; }
    bool IsRenderTargetDirty() const { return dirtyFlags & DirtyBit::RenderTarget; }
    bool IsVertexBuffersDirty() const { return dirtyFlags & DirtyBit::VertexBuffers; }
    bool IsTexturesDirty() const { return dirtyFlags & DirtyBit::Textures; }
    
    // Check if using 10-bit HDR framebuffer
    bool IsHDR10Bit() const { return surfaceFormat == XenosSurfaceFormat::R10G10B10A2; }
    bool IsHDR16Bit() const { return surfaceFormat == XenosSurfaceFormat::R16G16B16A16_F; }
};

// =============================================================================
// Sampler State Extraction
// =============================================================================
struct SamplerState {
    uint32_t addressU;      // Wrap mode U
    uint32_t addressV;      // Wrap mode V
    uint32_t addressW;      // Wrap mode W
    uint32_t minFilter;     // Minification filter
    uint32_t magFilter;     // Magnification filter
    uint32_t mipFilter;     // Mipmap filter
    uint32_t maxAnisotropy; // Anisotropic filtering level
    float mipLodBias;
    float minLod;
    float maxLod;
    uint32_t borderColor;

    static SamplerState Extract(uint8_t* device, uint32_t slot);
};

} // namespace GTAIV
