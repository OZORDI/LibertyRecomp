#pragma once

#include <cstdint>
#include <kernel/memory.h>

// =============================================================================
// GTA IV Device Context Structure
// Based on RENDERING_RESEARCH.md analysis of the ~19KB device context
// All offsets are byte offsets from the device pointer
// =============================================================================

namespace GTAIV {

// Device context field offsets (from RENDERING_RESEARCH.md)
namespace DeviceOffset {
    // Command Buffer Management
    constexpr uint32_t CommandBufferPtr    = 48;      // +48: Current write position in PM4 buffer
    constexpr uint32_t CommandBufferLimit  = 56;      // +56: End of PM4 buffer (flush trigger)

    // Render State Flags
    constexpr uint32_t DirtyFlags          = 16;      // +16: 64-bit dirty state bitmask
    constexpr uint32_t RenderTargetDirty   = 24;      // +24: RT change flags
    constexpr uint32_t DeferredFlag        = 10908;   // +10908: Deferred rendering mode
    constexpr uint32_t DirtyMask           = 10912;   // +10912: State dirty mask
    constexpr uint32_t ShaderFlags         = 10940;   // +10940: Shader state (bits 3,4,5,6,7)
    constexpr uint32_t DrawFlags           = 10941;   // +10941: Draw state (bit 4 = pending)
    constexpr uint32_t CommandFlags        = 10943;   // +10943: Command buffer (bit 3 = active)

    // Shader State
    constexpr uint32_t IndexBufferBase     = 10900;   // +10900: Base address for indexed draws
    constexpr uint32_t CurrentVertexShader = 10932;   // +10932: Active VS handle (0=none, -1=invalidate)
    constexpr uint32_t CurrentPixelShader  = 10936;   // +10936: Active PS handle (0=none)
    constexpr uint32_t VertexShaderCache   = 12700;   // +12700: Cached VS handle
    constexpr uint32_t PixelShaderCache    = 12704;   // +12704: Cached PS handle
    constexpr uint32_t ShaderValidFlag     = 12708;   // +12708: 1 = valid shader bound
    constexpr uint32_t ShaderSlots         = 12720;   // +12720: Shader constant buffer slots (5 × 4 bytes)
    constexpr uint32_t ShaderParams        = 12432;   // +12432: Shader parameter references (5 × 4 bytes)

    // Vertex Input State
    constexpr uint32_t VertexFormatFlags   = 10372;   // +10372: Vertex format (bits 12-15)
    constexpr uint32_t VertexDeclaration   = 10456;   // +10456: Vertex declaration pointer
    constexpr uint32_t StreamSource0       = 12020;   // +12020: Vertex buffer slot 0
    constexpr uint32_t StreamSource1       = 12024;   // +12024: Vertex buffer slot 1
    constexpr uint32_t StreamSource2       = 12028;   // +12028: Vertex buffer slot 2
    constexpr uint32_t StreamSource3       = 12032;   // +12032: Vertex buffer slot 3
    constexpr uint32_t IndexBuffer         = 13580;   // +13580: Index buffer pointer

    // Render Target State
    constexpr uint32_t RenderTargetArray   = 1780;    // +1780: Packed RT params (4 × 8 bytes)
    constexpr uint32_t DepthStencilTarget  = 12428;   // +12428: Depth/stencil surface
    constexpr uint32_t RenderTargetSlots   = 12452;   // +12452: RT surface pointers (4 × 4 bytes)

    // Texture State
    constexpr uint32_t TextureSlots        = 12536;   // +12536: Texture pointers (19 slots × 4 bytes)
    constexpr uint32_t SamplerDescs        = 1152;    // +1152: Sampler descriptors (19 × 24 bytes)

    // Blend State
    constexpr uint32_t BlendState          = 11844;   // +11844: Blend state flags

    // Frame/Present State
    constexpr uint32_t GpuStatePtr         = 13500;   // +13500: GPU state reference
    constexpr uint32_t PresentCmdPtr       = 13508;   // +13508: Present command buffer
    constexpr uint32_t PresentCmdLimit     = 13512;   // +13512: Present buffer limit
    constexpr uint32_t DisplayWidthPacked  = 13592;   // +13592: Display width >> 17
    constexpr uint32_t DisplayHeightPacked = 13596;   // +13596: Display height info
    constexpr uint32_t GpuContextPtr       = 14888;   // +14888: GPU context reference
    constexpr uint32_t FrameCounter        = 16544;   // +16544: Frames presented
    constexpr uint32_t FrameBufferIndex    = 19480;   // +19480: Current backbuffer
}

// PM4 Command Opcodes (from RENDERING_RESEARCH.md)
namespace PM4 {
    constexpr uint32_t SET_CONTEXT_REG     = 0x05C8;
    constexpr uint32_t WAIT_FOR_IDLE       = 0x0578;
    constexpr uint32_t DRAW_INDX           = 0x057C;
    constexpr uint32_t SET_CONSTANT_VS     = 0xC0003C00;
    constexpr uint32_t SET_CONSTANT_PS     = 0xC0005400;
    constexpr uint32_t SET_SHADER          = 0xC0006000;
    constexpr uint32_t INDIRECT_BUFFER     = 0x003F;
    constexpr uint32_t ME_INIT             = 0x0048;
}

// D3D Primitive Types (Xenos)
enum class PrimitiveType : uint32_t {
    PointList      = 1,
    LineList       = 2,
    LineStrip      = 3,
    TriangleList   = 4,
    TriangleFan    = 5,
    RectList       = 6,   // Quad list (Xenos-specific)
    TriangleStrip  = 8,
    QuadList       = 13
};

// Index Format (from sub_829D4EE0 analysis)
enum class IndexFormat : uint32_t {
    Index16Bit     = 0,
    Index16Bit_Alt = 1,
    Index32BitLE   = 2,
    Index32BitBE   = 4,
    GpuDefault     = 0x80000000
};

// Helper functions to read device context fields
inline uint32_t GetDeviceU32(uint8_t* device, uint32_t offset) {
    return ByteSwap(*reinterpret_cast<uint32_t*>(device + offset));
}

inline uint64_t GetDeviceU64(uint8_t* device, uint32_t offset) {
    return ByteSwap(*reinterpret_cast<uint64_t*>(device + offset));
}

inline uint8_t GetDeviceU8(uint8_t* device, uint32_t offset) {
    return device[offset];
}

inline void SetDeviceU32(uint8_t* device, uint32_t offset, uint32_t value) {
    *reinterpret_cast<uint32_t*>(device + offset) = ByteSwap(value);
}

inline void SetDeviceU8(uint8_t* device, uint32_t offset, uint8_t value) {
    device[offset] = value;
}

// Draw call state extraction (from RENDERING_RESEARCH.md Section 6)
struct DrawCallState {
    // Shaders
    uint32_t vertexShader;      // device[10932]
    uint32_t pixelShader;       // device[10936]
    bool shaderValid;           // device[12708] != 0

    // Vertex Buffers
    uint32_t vbSlot0;           // device[12020]
    uint32_t vbSlot1;           // device[12024]
    uint32_t vbSlot2;           // device[12028]
    uint32_t vbSlot3;           // device[12032]

    // Index Buffer
    uint32_t indexBuffer;       // device[13580]

    // Blend State
    uint32_t blendState;        // device[11844]

    // Vertex Declaration
    uint32_t vertexDeclaration; // device[10456]

    static DrawCallState Extract(uint8_t* device) {
        DrawCallState state;
        state.vertexShader      = GetDeviceU32(device, DeviceOffset::CurrentVertexShader);
        state.pixelShader       = GetDeviceU32(device, DeviceOffset::CurrentPixelShader);
        state.shaderValid       = GetDeviceU32(device, DeviceOffset::ShaderValidFlag) != 0;
        state.vbSlot0           = GetDeviceU32(device, DeviceOffset::StreamSource0);
        state.vbSlot1           = GetDeviceU32(device, DeviceOffset::StreamSource1);
        state.vbSlot2           = GetDeviceU32(device, DeviceOffset::StreamSource2);
        state.vbSlot3           = GetDeviceU32(device, DeviceOffset::StreamSource3);
        state.indexBuffer       = GetDeviceU32(device, DeviceOffset::IndexBuffer);
        state.blendState        = GetDeviceU32(device, DeviceOffset::BlendState);
        state.vertexDeclaration = GetDeviceU32(device, DeviceOffset::VertexDeclaration);
        return state;
    }
};

// Index format detection (from sub_829D4EE0)
inline IndexFormat DetectIndexFormat(uint8_t* device) {
    uint32_t ibState = GetDeviceU32(device, DeviceOffset::IndexBuffer);
    switch (ibState) {
        case 0:
        case 1:  return IndexFormat::Index16Bit;
        case 2:  return IndexFormat::Index32BitLE;
        case 4:  return IndexFormat::Index32BitBE;
        default:
            if (ibState == 0x80000000) return IndexFormat::GpuDefault;
            return IndexFormat::Index16Bit;
    }
}

} // namespace GTAIV
