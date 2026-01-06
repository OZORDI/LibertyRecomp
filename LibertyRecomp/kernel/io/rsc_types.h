#pragma once

#include <cstdint>

/**
 * GTA IV Resource (RSC) Format Types and Constants
 * 
 * Based on gtamods.com documentation:
 * https://gtamods.com/wiki/WDR
 * https://gtamods.com/wiki/WTD
 * 
 * RSC files have a header followed by CPU and GPU sections.
 * Offsets in CPU section use 0x50 prefix, GPU uses 0x60 prefix.
 */
namespace RscTypes
{
    // =========================================================================
    // RSC Header Magic
    // =========================================================================
    
    static constexpr uint32_t RSC_MAGIC = 0x05435352;  // "RSC\x05"

    // =========================================================================
    // Resource Types (from IMG archive resource type field)
    // =========================================================================
    
    namespace ResourceType
    {
        static constexpr uint32_t Generic = 0x01;
        static constexpr uint32_t TextureArchive = 0x08;  // .wtd/.xtd
        static constexpr uint32_t Bounds = 0x20;          // .wbd/.xbd
        static constexpr uint32_t Model = 0x6E;           // .wdr/.xdr
        static constexpr uint32_t Xpfl = 0x24;
        
        // Platform-specific texture types in RSC header
        static constexpr uint32_t XboxTexture = 0x07;
        static constexpr uint32_t PCTexture = 0x08;
    }

    // =========================================================================
    // VTable Addresses (PC - from gtamods wiki)
    // These differ between PC and Xbox 360!
    // =========================================================================
    
    namespace VTablePC
    {
        // WDR (Drawable) VTables
        static constexpr uint32_t DrawableHeader = 0x00695254;
        static constexpr uint32_t Model = 0x34026B00;
        static constexpr uint32_t Geometry = 0xF4486B00;
        static constexpr uint32_t VertexBuffer = 0xD8BA6B00;
        static constexpr uint32_t IndexBuffer = 0x70B86B00;
        static constexpr uint32_t ShaderGroup = 0x44166B00;
        static constexpr uint32_t ShaderFX = 0x3C226B00;
        static constexpr uint32_t TextureDef = 0x005C676B;
        
        // Skeleton
        static constexpr uint32_t Skeleton = 0x006B2778;
    }

    // Xbox 360 VTable addresses (need to be determined from Xbox binary)
    namespace VTableXbox
    {
        // These would need to be mapped from the Xbox 360 executable
        // For now, document that they differ
        static constexpr uint32_t Unknown = 0x00000000;
    }

    // =========================================================================
    // RSC Section Markers (offset prefixes)
    // =========================================================================
    
    static constexpr uint8_t CPU_SECTION_MARKER = 0x50;
    static constexpr uint8_t GPU_SECTION_MARKER = 0x60;

    // =========================================================================
    // Structure Sizes (from gtamods wiki)
    // =========================================================================
    
    namespace BlockSize
    {
        static constexpr uint32_t WdrHeader = 0x2A0;
        static constexpr uint32_t ModelCollection = 0x08;
        static constexpr uint32_t Model = 0x20;
        static constexpr uint32_t Geometry = 0x50;
        static constexpr uint32_t VertexBuffer = 0x64;
        static constexpr uint32_t IndexBuffer = 0x40;
        static constexpr uint32_t ShaderGroup = 0x50;
        static constexpr uint32_t ShaderFX = 0x50;
        static constexpr uint32_t SkeletonData = 0x40;
        static constexpr uint32_t TextureDef = 0x1C;
        static constexpr uint32_t PointerCollection = 0x08;
        static constexpr uint32_t SimpleCollection = 0x08;
    }

    // =========================================================================
    // Vertex Stride Values
    // =========================================================================
    
    static constexpr uint32_t VERTEX_STRIDE_NO_NORMALMAP = 36;
    static constexpr uint32_t VERTEX_STRIDE_WITH_NORMALMAP = 52;

    // =========================================================================
    // RSC Header Structure
    // =========================================================================
    
    #pragma pack(push, 1)
    struct RscHeader
    {
        uint32_t magic;         // RSC_MAGIC
        uint32_t type;          // ResourceType
        uint32_t flags;         // Contains CPU/GPU section sizes
        uint16_t compressCodec; // Compression codec (0 = none)
        uint16_t padding;
        
        // Extract CPU section size from flags
        uint32_t GetCpuSize() const
        {
            return (flags & 0x7FF) << ((flags >> 11) & 0xF);
        }
        
        // Extract GPU section size from flags  
        uint32_t GetGpuSize() const
        {
            return ((flags >> 15) & 0x7FF) << ((flags >> 26) & 0xF);
        }
    };

    // WDR Header (Drawable)
    struct WdrHeader
    {
        uint32_t vtable;            // VTablePC::DrawableHeader
        uint32_t headerLength;      // Usually 0x90
        uint32_t shaderGroup;       // Offset to ShaderGroup (0 = none)
        uint32_t skeletonData;      // Offset to Skeleton (0 = none)
        float centerX, centerY, centerZ, centerW;
        float boundsMinX, boundsMinY, boundsMinZ, boundsMinW;
        float boundsMaxX, boundsMaxY, boundsMaxZ, boundsMaxW;
        uint32_t modelCollection;   // Pointer to ModelCollection
        uint32_t lodModels[3];      // LOD model pointers
        float maxVector[4];         // Usually 9999.0
        uint32_t objectCount;
        uint32_t unknown[3];        // Usually 0xFFFFFFFF
        float unknownFloat;
        uint32_t unknownZeros[3];
        uint32_t effect2dfxOffset;  // 2DFX array offset
        uint16_t effect2dfxCount;
        uint16_t effect2dfxSize;
        uint64_t unassigned;        // 0xCDCDCDCDCDCDCDCD
        uint32_t endOfHeader;       // Usually 0
    };
    #pragma pack(pop)

    // =========================================================================
    // Helper Functions
    // =========================================================================
    
    /**
     * Check if an offset is in CPU section (0x50 prefix).
     */
    inline bool IsCpuOffset(uint32_t offset)
    {
        return (offset & 0xFF000000) == (CPU_SECTION_MARKER << 24);
    }

    /**
     * Check if an offset is in GPU section (0x60 prefix).
     */
    inline bool IsGpuOffset(uint32_t offset)
    {
        return (offset & 0xFF000000) == (GPU_SECTION_MARKER << 24);
    }

    /**
     * Strip section marker from offset.
     */
    inline uint32_t StripSectionMarker(uint32_t offset)
    {
        return offset & 0x00FFFFFF;
    }

    /**
     * Check if resource type is a texture.
     */
    inline bool IsTextureResource(uint32_t type)
    {
        return type == ResourceType::TextureArchive ||
               type == ResourceType::XboxTexture ||
               type == ResourceType::PCTexture;
    }

    /**
     * Check if resource type is a model.
     */
    inline bool IsModelResource(uint32_t type)
    {
        return type == ResourceType::Model;
    }
}
