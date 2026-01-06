#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <unordered_map>
#include <mutex>
#include <functional>

/**
 * Runtime RPF Loader for LibertyRecomp
 * 
 * Provides on-demand extraction of RPF archives found in mod directories.
 * Unlike install-time extraction, this lazily extracts files when accessed.
 * 
 * Features:
 *   - Detect .rpf files in mod overlay directories
 *   - Extract individual files on first access (lazy extraction)
 *   - Cache extracted files in memory or temp directory
 *   - Integrate with VFS for transparent file serving
 * 
 * RPF2 Format (GTA IV):
 *   - Magic: 0x52504632 ("RPF2")
 *   - TOC at offset 0x800
 *   - AES-256-ECB encryption (16 rounds)
 *   - Raw deflate compression
 */
namespace RpfLoader
{
    /**
     * File entry from an RPF archive
     */
    struct RpfFileEntry
    {
        std::string name;           // Filename
        std::string fullPath;       // Full path within archive
        uint32_t offset;            // Offset in archive
        uint32_t size;              // Uncompressed size
        uint32_t compressedSize;    // Compressed size (0 if not compressed)
        bool isCompressed;
        bool isDirectory;
        uint8_t resourceType;       // 0 for normal files
    };

    /**
     * Cached RPF archive info
     */
    struct RpfCache
    {
        std::filesystem::path rpfPath;          // Path to .rpf file
        std::vector<RpfFileEntry> entries;      // All entries in archive
        std::unordered_map<std::string, size_t> fileIndex;  // Normalized path -> entry index
        std::vector<uint8_t> aesKey;            // AES key (if encrypted)
        bool isEncrypted;
        uint64_t lastModified;
    };

    /**
     * Extracted file data
     */
    struct ExtractedFile
    {
        std::vector<uint8_t> data;
        std::string originalPath;
        bool fromCache;
    };

    /**
     * Initialize the RPF loader.
     * Loads AES key from standard locations.
     */
    void Initialize();

    /**
     * Check if initialized.
     */
    bool IsInitialized();

    /**
     * Set the AES key for encrypted RPF archives.
     */
    void SetAesKey(const std::vector<uint8_t>& key);

    /**
     * Scan mod overlay directories for .rpf files.
     * Caches TOC entries for quick lookup.
     */
    void ScanForRpfFiles(const std::filesystem::path& overlayRoot);

    /**
     * Check if a file path exists in any loaded RPF archive.
     * 
     * @param normalizedPath Normalized file path (lowercase, forward slashes)
     * @return True if file exists in an RPF
     */
    bool HasFile(const std::string& normalizedPath);

    /**
     * Get the RPF path that contains a file.
     * 
     * @param normalizedPath Normalized file path
     * @return Path to containing RPF, or empty if not found
     */
    std::filesystem::path GetContainingRpf(const std::string& normalizedPath);

    /**
     * Extract a single file from an RPF archive.
     * Caches the result for subsequent access.
     * 
     * @param normalizedPath Normalized file path within RPF
     * @return Extracted file data, or nullopt on failure
     */
    std::optional<ExtractedFile> ExtractFile(const std::string& normalizedPath);

    /**
     * Extract a file to a temporary location on disk.
     * Useful for files that need to be memory-mapped.
     * 
     * @param normalizedPath Normalized file path
     * @return Path to extracted temp file, or empty on failure
     */
    std::filesystem::path ExtractToTemp(const std::string& normalizedPath);

    /**
     * List all files in a specific RPF archive.
     */
    std::vector<RpfFileEntry> ListFiles(const std::filesystem::path& rpfPath);

    /**
     * List all loaded RPF archives.
     */
    std::vector<std::filesystem::path> GetLoadedRpfs();

    /**
     * Clear all cached data.
     */
    void ClearCache();

    /**
     * Get statistics.
     */
    struct Stats
    {
        uint64_t rpfsLoaded = 0;
        uint64_t filesExtracted = 0;
        uint64_t bytesExtracted = 0;
        uint64_t cacheHits = 0;
        uint64_t cacheMisses = 0;
    };
    Stats GetStats();

    /**
     * Dump status to log.
     */
    void DumpStatus();
}
