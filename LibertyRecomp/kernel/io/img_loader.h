#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <mutex>

/**
 * IMG Archive Loader for LibertyRecomp
 * 
 * Provides support for loading and merging GTA IV IMG archives.
 * Compatible with FusionFix's IMG loading mechanism.
 * 
 * IMG v3 Format (GTA IV):
 *   - Magic: 0xA94E2A52
 *   - Block size: 2048 bytes
 *   - Header: 20 bytes (magic, version, numItems, tableSize, itemSize, unknown)
 *   - Entry: 16 bytes (size/flags, resourceType, offsetBlock, usedBlocks, flags)
 *   - Filename table: null-terminated strings
 * 
 * Features:
 *   - Extract files from IMG archives
 *   - Merge IMG with update folder (FusionFix-style)
 *   - Virtual IMG serving (no disk writes)
 *   - Case-insensitive file matching
 */
namespace ImgLoader
{
    // IMG format constants
    static constexpr size_t IMG_BLOCK_SIZE = 2048;
    static constexpr size_t MAX_FILESIZE = 0xFFFF * IMG_BLOCK_SIZE;
    static constexpr uint32_t GTAIV_MAGIC = 0xA94E2A52;

    // IMG version enum
    enum class ImgVersion
    {
        V1,  // GTA III, Vice City
        V2,  // San Andreas
        V3   // GTA IV
    };

#pragma pack(push, 1)
    // IMG v3 header (GTA IV)
    struct ImgHeaderV3
    {
        uint32_t magic;       // 0xA94E2A52
        uint32_t version;     // 3
        uint32_t numItems;
        uint32_t tableSize;
        uint16_t itemSize;    // 16
        uint16_t unknown;
    };

    // IMG v3 entry (GTA IV)
    struct ImgEntryV3
    {
        uint32_t sizeOrFlags;    // Size for normal files, RSC flags for resources
        uint32_t resourceType;   // 0 for normal, 0x08 for textures, 0x6E for models
        uint32_t offsetBlock;    // Offset in blocks
        uint16_t usedBlocks;     // Number of blocks used
        uint16_t flags;          // Flags (bit 13 = RSC file)
    };

    // RSC header for resource files
    struct RscHeader
    {
        static constexpr uint32_t MAGIC = 0x05435352; // "RSC\x05"
        uint32_t magic;
        uint32_t type;
        uint32_t flags;
        uint16_t compressCodec;
        uint16_t padding;
    };
#pragma pack(pop)

    /**
     * File entry in an IMG archive
     */
    struct FileEntry
    {
        std::string name;
        std::vector<uint8_t> data;
        uint32_t position = 0;      // Position in blocks (for writing)
        uint32_t resourceType = 0;  // Resource type (0 = normal file)
        bool isReplacement = false; // True if from update folder
    };

    /**
     * Virtual IMG archive (merged result)
     */
    struct VirtualImg
    {
        std::string originalPath;           // Path to original IMG
        std::vector<FileEntry> files;       // All files (original + replacements)
        std::shared_ptr<std::vector<uint8_t>> data; // Merged IMG data
        ImgVersion version = ImgVersion::V3;
        uint64_t lastModified = 0;
    };

    /**
     * Initialize the IMG loader system.
     */
    void Initialize();

    /**
     * Check if initialized.
     */
    bool IsInitialized();

    /**
     * Extract all files from an IMG archive.
     * 
     * @param imgPath Path to the IMG file
     * @return Vector of file entries, or nullopt on failure
     */
    std::optional<std::vector<FileEntry>> ExtractImg(const std::filesystem::path& imgPath);

    /**
     * Get replacement files from an update folder.
     * FusionFix places replacement files in folders named like the IMG file.
     * Example: update/pc/models/cdimages/vehicles.img/ contains replacement files
     * 
     * @param updateFolder Path to the update folder (with .img extension)
     * @return Vector of replacement file entries
     */
    std::vector<FileEntry> GetReplacementFiles(const std::filesystem::path& updateFolder);

    /**
     * Merge original IMG files with replacement files.
     * 
     * @param originalFiles Files from original IMG
     * @param replacementFiles Files from update folder
     * @return Merged file list (replacements override originals)
     */
    std::vector<FileEntry> MergeFiles(
        const std::vector<FileEntry>& originalFiles,
        const std::vector<FileEntry>& replacementFiles);

    /**
     * Create an IMG archive from a list of files.
     * 
     * @param files Files to include
     * @param version IMG format version
     * @return IMG data as byte vector
     */
    std::shared_ptr<std::vector<uint8_t>> CreateImg(
        std::vector<FileEntry>& files,
        ImgVersion version = ImgVersion::V3);

    /**
     * Merge an IMG with its update folder and return virtual IMG.
     * This is the main entry point for FusionFix-style IMG loading.
     * 
     * @param originalImgPath Path to original IMG
     * @param updateFolderPath Path to update folder (e.g., update/pc/models/vehicles.img/)
     * @return Virtual IMG with merged data, or nullptr on failure
     */
    std::shared_ptr<VirtualImg> MergeImgWithFolder(
        const std::filesystem::path& originalImgPath,
        const std::filesystem::path& updateFolderPath);

    /**
     * Check if an update folder exists for an IMG file.
     * Searches in registered overlay directories.
     * 
     * @param imgPath Relative path to IMG (e.g., "pc/models/cdimages/vehicles.img")
     * @return Path to update folder if found, empty path otherwise
     */
    std::filesystem::path FindUpdateFolder(const std::string& imgPath);

    /**
     * Get or create a virtual IMG for a given path.
     * Caches results for performance.
     * 
     * @param imgPath Path to IMG file (relative to game root)
     * @return Virtual IMG data, or nullptr if no merging needed
     */
    std::shared_ptr<VirtualImg> GetVirtualImg(const std::string& imgPath);

    /**
     * Clear the virtual IMG cache.
     */
    void ClearCache();

    /**
     * Get statistics about IMG loading.
     */
    struct Stats
    {
        uint64_t imgsLoaded = 0;
        uint64_t imgsMerged = 0;
        uint64_t filesReplaced = 0;
        uint64_t filesAdded = 0;
        uint64_t cacheHits = 0;
        uint64_t cacheMisses = 0;
    };
    Stats GetStats();

    /**
     * Dump IMG loader status to log.
     */
    void DumpStatus();
}
