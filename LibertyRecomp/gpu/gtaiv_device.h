#pragma once

#include <cstdint>
#include <kernel/memory.h>

// =============================================================================
// GTA IV grcDeviceDx Context Structure (22400 / 0x5780 bytes)
// Decompiled from sub_82A50890 (CreateDevice) and its 18 callees.
// All offsets are byte offsets from the device pointer at 0x831C22A4.
// =============================================================================

namespace GTAIV {

// Device context field offsets (decompiled from sub_82A50890 + callees)
namespace DeviceOffset {
    // --- Dirty Flags (5 x uint64_t at +0x00) ---
    constexpr uint32_t DirtyFlags          = 0;       // +0x00: 5 qwords, set to -1 by sub_82A4F7E0/sub_82A3BAA8

    // --- Command Buffer (sub_82A49D08) ---
    constexpr uint32_t CommandBufferPtr    = 48;      // +0x30: Current write position in PM4 buffer
    constexpr uint32_t CommandBufferEnd    = 52;      // +0x34: End of current buffer segment
    constexpr uint32_t CommandBufferLimit  = 56;      // +0x38: Soft limit (flush trigger)

    // --- Render State Function Table (sub_82A50160, ROM at 0x82B25B30) ---
    // 101 render state apply functions at dwords 16..116 (bytes 64..464)
    // Called as: func(device, value) to write GPU registers
    constexpr uint32_t RenderStateFuncs    = 64;      // +0x40: 101 x uint32_t func ptrs
    constexpr uint32_t RenderStateFuncsEnd = 468;     // +0x1D4: end of func ptr array

    // --- Sampler State Function Table (sub_82A50160, ROM at 0x82B25FF0) ---
    // 20 sampler state apply functions at dwords 117..136 (bytes 468..544)
    // Called as: func(device, stage, value)
    constexpr uint32_t SamplerStateFuncs   = 468;     // +0x1D4: 20 x uint32_t func ptrs
    constexpr uint32_t SamplerStateFuncsEnd= 548;     // +0x224: end of sampler func ptrs

    // --- Render State Values (sub_82A50160) ---
    // 101 current state values at dwords 137..237 (bytes 548..948)
    constexpr uint32_t RenderStateValues   = 548;     // +0x224: 101 x uint32_t current values
    // 20 sampler state values at dwords 238..257 (bytes 952..1028)
    constexpr uint32_t SamplerStateValues  = 952;     // +0x3B8: 20 x uint32_t current values

    // --- Sampler Stage Dirty Flags (sub_82A4F7E0) ---
    constexpr uint32_t SamplerDescs        = 1152;    // +0x480: 26 stages x 24 bytes stride

    // --- Render State Defaults (sub_82A4F7E0) ---
    constexpr uint32_t DefaultRenderState  = 10428;   // +0x28BC: 0x20002000 (PA_CL_VTE_CNTL)
    constexpr uint32_t StencilRef          = 10444;   // +0x28CC: 0xFFFFFF
    constexpr uint32_t GpuModeFlags1       = 10564;   // +0x2944: |= 0x80000
    constexpr uint32_t GpuModeFlags2       = 10568;   // +0x2948: |= 0x10000
    constexpr uint32_t FillMode            = 10580;   // +0x2954: 4 (solid)
    constexpr uint32_t DepthFunc           = 10604;   // +0x296C: 8 (less-equal)
    constexpr uint32_t SrcBlend            = 10628;   // +0x2984: 14 (D3DBLEND_SRCALPHA)
    constexpr uint32_t CullMode            = 10688;   // +0x29C0: 4 (back-face)
    constexpr uint32_t AlphaTestRef        = 10708;   // +0x29D4: 0xFF000
    constexpr uint32_t AlphaTestFunc       = 10712;   // +0x29D8: 0xFF100
    constexpr uint32_t DstBlend            = 10768;   // +0x2A10: 14 (D3DBLEND_INVSRCALPHA)
    constexpr uint32_t BlendOp             = 10772;   // +0x2A14: 16 (D3DBLENDOP_ADD)
    constexpr uint32_t StencilFunc         = 10824;   // +0x2A48: 2

    // --- GPU Control Blocks (sub_82A49D08) ---
    constexpr uint32_t GpuControlBlock     = 10896;   // +0x2A90: 96-byte GPU control block ptr
    constexpr uint32_t GpuIdentifierBlock  = 10900;   // +0x2A94: 32-byte GPU identifier block ptr
    constexpr uint32_t DrawMode            = 10908;   // +0x2A9C: swap interval / draw mode
    constexpr uint32_t MultiSampleMode     = 10916;   // +0x2AA4: 14
    constexpr uint32_t DirtyMask           = 10912;   // +0x2AA0: State dirty mask

    // --- Device Flags ---
    constexpr uint32_t CurrentVertexShader = 10932;   // +0x2AB4: Active VS handle
    constexpr uint32_t CurrentPixelShader  = 10936;   // +0x2AB8: Active PS handle
    constexpr uint32_t ShaderFlags         = 10940;   // +0x2ABC: bit2=init, bit4=no-present, bit7=deferred
    constexpr uint32_t DrawFlags           = 10941;   // +0x2ABD: Draw state
    constexpr uint32_t InitFlags           = 10942;   // +0x2ABE: bit2 set in CreateDevice
    constexpr uint32_t CommandFlags        = 10943;   // +0x2ABF: Command buffer flags
    constexpr uint32_t CmdBufFlushCount    = 10952;   // +0x2AC8: Flush counter

    // --- Dirty State (sub_82A4F7E0) ---
    constexpr uint32_t DirtyQword          = 11816;   // +0x2E28: uint64_t, set to -1
    constexpr uint32_t BlendState          = 11844;   // +0x2E44: Blend state flags

    // --- Primitive / Render State (sub_82A50160) ---
    constexpr uint32_t NoRenderFlag        = 12179;   // +0x2F93: byte, skip-render flag
    constexpr uint32_t PrimitiveType       = 12184;   // +0x2F98: 5 = triangle list
    constexpr uint32_t PrimitiveReset      = 12188;   // +0x2F9C: 1 = reset mode

    // --- Shader State ---
    constexpr uint32_t ShaderParams        = 12432;   // +0x3090: 5 RT param refs
    constexpr uint32_t DepthStencilTarget  = 12428;   // +0x308C: Depth/stencil surface

    // --- Render Targets (sub_82A50160) ---
    constexpr uint32_t ScissorState        = 12700;   // +0x319C: init -1
    constexpr uint32_t ViewportState       = 12704;   // +0x31A0: init 0
    constexpr uint32_t ShaderValidFlag     = 12708;   // +0x31A4: 0
    constexpr uint32_t ShaderSlots         = 12720;   // +0x31B0: 5 x 4-byte curRT slots

    // --- Vertex Input State ---
    constexpr uint32_t VertexFormatFlags   = 10372;   // +0x2884: Vertex format
    constexpr uint32_t VertexDeclaration   = 10456;   // +0x28D8: Vertex decl pointer
    constexpr uint32_t StreamSource0       = 12020;   // +0x2EF4: VB slot 0
    constexpr uint32_t StreamSource1       = 12024;   // +0x2EF8: VB slot 1
    constexpr uint32_t StreamSource2       = 12028;   // +0x2EFC: VB slot 2
    constexpr uint32_t StreamSource3       = 12032;   // +0x2F00: VB slot 3
    constexpr uint32_t IndexBuffer         = 13580;   // +0x350C: Index buffer / saved prim type
    constexpr uint32_t IndexBufferBase     = 13584;   // +0x3510: Index buffer base address

    // --- Render Target State ---
    constexpr uint32_t RenderTargetArray   = 1780;    // +0x06F4: Packed RT params
    constexpr uint32_t RenderTargetSlots   = 12452;   // +0x30A4: RT surface ptrs

    // --- Texture State ---
    constexpr uint32_t TextureSlots        = 12536;   // +0x30F8: 19 texture ptrs

    // --- GPU Resource Tables (sub_82A42020) ---
    constexpr uint32_t ResourceTableAlloc  = 13764;   // +0x35C4: 0x2000 byte allocation

    // --- Ring Buffer (sub_82A49D08) ---
    constexpr uint32_t PrimaryRingBufAlloc = 14820;   // +0x39E4: Primary ring buffer alloc
    constexpr uint32_t SecondaryRingBufAlloc=14824;   // +0x39E8: Secondary ring buffer alloc
    constexpr uint32_t RingBufBase         = 14880;   // +0x3A20: Ring buffer base
    constexpr uint32_t RingBufMask         = 14884;   // +0x3A24: Ring buffer mask
    constexpr uint32_t GpuContextPtr       = 14888;   // +0x3A28: Command buffer base (physical)
    constexpr uint32_t CmdBufEndPhys       = 14892;   // +0x3A2C: Command buffer end
    constexpr uint32_t CmdBufEntryCount    = 14896;   // +0x3A30: Entry count
    constexpr uint32_t BufferSegmentBase   = 14912;   // +0x3A40: Read pointer
    constexpr uint32_t CmdBufSizeDwords    = 14920;   // +0x3A48: Size in dwords

    // --- Critical Sections (sub_82A50890) ---
    constexpr uint32_t CritSection1        = 14928;   // +0x3A50: RTL_CRITICAL_SECTION (28 bytes)
    constexpr uint32_t CritSection2        = 14956;   // +0x3A6C: RTL_CRITICAL_SECTION (28 bytes)

    // --- Timer / Performance (sub_82A50890) ---
    constexpr uint32_t GpuTimerContext     = 16544;   // +0x40A0: GPU timer context
    constexpr uint32_t FrameCounter        = 16544;   // +0x40A0: alias
    constexpr uint32_t FrameSubmitted      = 16548;   // +0x40A4: GPU frame submitted counter
    constexpr uint32_t InterruptCount      = 16696;   // +0x4138: Interrupt counter
    constexpr uint32_t XConfigVideoFlags   = 16700;   // +0x413C: ExGetXConfigSetting result
    constexpr uint32_t PerfCounterAddr1    = 16704;   // +0x4140: Perf counter write addr
    constexpr uint32_t PerfCounterAddr2    = 16708;   // +0x4144: Perf counter write addr 2
    constexpr uint32_t SecondaryBufferBase = 16712;   // +0x4148: Secondary buffer base

    // --- Frame IDs (sub_82A50890) ---
    constexpr uint32_t PrevFrameId         = 21540;   // +0x5424: init -1
    constexpr uint32_t CurFrameId          = 21544;   // +0x5428: init -1
    constexpr uint32_t GpuTimerResult      = 21556;   // +0x5434: Timer result
    constexpr uint32_t PerfFrequency       = 21568;   // +0x5440: float, perf counter freq
    constexpr uint32_t PerfPeriod          = 21572;   // +0x5444: float, 1/freq

    // --- Movie Handles (sub_82A53058) ---
    constexpr uint32_t MovieHandles        = 21680;   // +0x54B0: 41 x uint32_t, init -1

    // --- Create Flags (sub_82A416B8) ---
    constexpr uint32_t CreateFlags         = 22280;   // +0x5708: Creation flags from factory

    // --- Display Info (sub_82A503C8 via VdQueryVideoMode) ---
    constexpr uint32_t DisplayWidth        = 21524;   // +0x5414: a1[5381]
    constexpr uint32_t DisplayHeight       = 21528;   // +0x5418: a1[5382]
    constexpr uint32_t FrameBufferIndex    = 19480;   // +0x4C18: Current backbuffer
}

// Device struct total size (allocated by sub_82A416B8 via sub_82A412D0)
constexpr uint32_t DeviceStructSize        = 0x5780;  // 22400 bytes

// Global addresses
constexpr uint32_t ADDR_DEVICE_PTR         = 0x831C22A4; // grcDevice* global
constexpr uint32_t ADDR_DEVICE_PTR_INDIR   = 0x8200078C; // Pointer to ADDR_DEVICE_PTR

// Render state ROM tables (sub_82A50160 data source)
constexpr uint32_t ADDR_RENDER_STATE_TABLE = 0x82B25B30; // 101 entries x 12 bytes
constexpr uint32_t ADDR_SAMPLER_STATE_TABLE= 0x82B25FF0; // 20 entries x 12 bytes
constexpr uint32_t NUM_RENDER_STATES       = 101;
constexpr uint32_t NUM_SAMPLER_STATES      = 20;
constexpr uint32_t NUM_SAMPLER_STAGES      = 26;

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
