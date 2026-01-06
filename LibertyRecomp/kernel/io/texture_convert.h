#pragma once

#include <cstdint>
#include <vector>
#include <optional>
#include <filesystem>
#include "rsc_types.h"

/**
 * Texture Format Conversion for LibertyRecomp
 * 
 * Provides PC <-> Xbox 360 texture format conversion.
 * Handles Morton/Z-order swizzling required by Xbox 360 GPU.
 * 
 * Xbox 360 vs PC Texture Differences:
 *   - Endianness: Xbox 360 = Big-endian, PC = Little-endian
 *   - Layout: Xbox 360 = Morton/Z-order swizzled, PC = Linear
 *   - Resource types: Xbox 360 uses 0x07 for textures, PC uses 0x08
 *   - Extensions: Xbox 360 = .xtd/.xdr, PC = .wtd/.wdr
 * 
 * Morton Order (Z-order):
 *   Interleaves X and Y coordinate bits for better cache locality.
 *   Example for 8x8 texture:
 *     Linear: y*width + x
 *     Morton: interleave(x_bits, y_bits)
 */
namespace TextureConvert
{
    // Texture format info
    enum class TextureFormat
    {
        Unknown,
        DXT1,       // BC1 - 4 bits per pixel
        DXT3,       // BC2 - 8 bits per pixel  
        DXT5,       // BC3 - 8 bits per pixel
        DXN,        // BC5 - Normal maps
        A8R8G8B8,   // 32 bits per pixel
        X8R8G8B8,   // 32 bits per pixel (no alpha)
        R5G6B5,     // 16 bits per pixel
        A1R5G5B5,   // 16 bits per pixel
        A4R4G4B4,   // 16 bits per pixel
        L8,         // 8 bits per pixel (luminance)
        A8L8,       // 16 bits per pixel (luminance + alpha)
    };

    // Platform type
    enum class Platform
    {
        PC,
        Xbox360
    };

    // Resource header (RSC format)
#pragma pack(push, 1)
    struct RscHeader
    {
        uint32_t magic;         // 0x05435352 ("RSC\x05")
        uint32_t type;          // Resource type (0x07=Xbox texture, 0x08=PC texture)
        uint32_t flags;         // Resource flags
        uint16_t compressCodec; // Compression codec
        uint16_t padding;
    };

    // Texture header (after RSC header, inside texture data)
    struct TextureHeader
    {
        uint32_t width;
        uint32_t height;
        uint32_t format;
        uint32_t mipLevels;
        // ... more fields depending on format
    };
#pragma pack(pop)

    /**
     * Detect platform from file extension.
     */
    Platform DetectPlatform(const std::filesystem::path& path);

    /**
     * Detect platform from resource header.
     */
    Platform DetectPlatformFromHeader(const std::vector<uint8_t>& data);

    /**
     * Check if conversion is needed (PC file requested but Xbox expected).
     */
    bool NeedsConversion(const std::filesystem::path& pcPath, Platform targetPlatform);

    /**
     * Convert PC texture (.wtd) to Xbox 360 format (.xtd compatible).
     * 
     * @param pcData PC texture file data
     * @return Xbox 360 compatible texture data, or nullopt on failure
     */
    std::optional<std::vector<uint8_t>> ConvertPCToXbox360(const std::vector<uint8_t>& pcData);

    /**
     * Convert Xbox 360 texture to PC format.
     * 
     * @param xboxData Xbox 360 texture file data
     * @return PC compatible texture data, or nullopt on failure
     */
    std::optional<std::vector<uint8_t>> ConvertXbox360ToPC(const std::vector<uint8_t>& xboxData);

    /**
     * Swizzle texture data from linear to Morton order.
     * Used for PC -> Xbox 360 conversion.
     * 
     * @param linearData Linear texture data
     * @param width Texture width
     * @param height Texture height
     * @param bytesPerPixel Bytes per pixel (or bytes per block for compressed)
     * @param blockWidth Block width (1 for uncompressed, 4 for DXT)
     * @param blockHeight Block height (1 for uncompressed, 4 for DXT)
     * @return Morton-order swizzled data
     */
    std::vector<uint8_t> SwizzleLinearToMorton(
        const std::vector<uint8_t>& linearData,
        uint32_t width,
        uint32_t height,
        uint32_t bytesPerPixel,
        uint32_t blockWidth = 1,
        uint32_t blockHeight = 1);

    /**
     * Deswizzle texture data from Morton order to linear.
     * Used for Xbox 360 -> PC conversion.
     */
    std::vector<uint8_t> DeswizzleMortonToLinear(
        const std::vector<uint8_t>& mortonData,
        uint32_t width,
        uint32_t height,
        uint32_t bytesPerPixel,
        uint32_t blockWidth = 1,
        uint32_t blockHeight = 1);

    /**
     * Swap endianness of texture data.
     * Xbox 360 uses big-endian, PC uses little-endian.
     * 
     * @param data Texture data
     * @param bytesPerElement Size of each element to swap (2 or 4)
     */
    void SwapEndian(std::vector<uint8_t>& data, uint32_t bytesPerElement);

    /**
     * Get bytes per pixel/block for a texture format.
     */
    uint32_t GetBytesPerPixel(TextureFormat format);

    /**
     * Get block dimensions for compressed formats.
     */
    std::pair<uint32_t, uint32_t> GetBlockSize(TextureFormat format);

    /**
     * Compute Morton index from X,Y coordinates.
     */
    uint32_t MortonEncode(uint32_t x, uint32_t y);

    /**
     * Decode Morton index to X,Y coordinates.
     */
    std::pair<uint32_t, uint32_t> MortonDecode(uint32_t morton);
}

    /**
     * Enhanced PC to Xbox 360 conversion with full texture parsing.
     * Includes mipmap swizzling and proper format detection.
     */
    std::optional<std::vector<uint8_t>> ConvertPCToXbox360Full(const std::vector<uint8_t>& pcData);
