#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <optional>
#include <mutex>

/**
 * Virtual File System for GTA IV Recompilation
 * 
 * Maps Xbox 360 game paths to extracted host file paths.
 * This eliminates the need for complex RPF offset-based reading
 * by serving files directly from the extracted directory.
 * 
 * Path Mapping:
 *   game:\                    → <extracted_root>/
 *   game:\common.rpf          → <extracted_root>/common/
 *   game:\common\shaders\*    → <extracted_root>/common/shaders/*
 *   game:\xbox360.rpf         → <extracted_root>/xbox360/
 */
namespace VFS
{
    /**
     * Initialize the virtual file system with the extracted root directory.
     * Should be called once at startup after game installation.
     */
    void Initialize(const std::filesystem::path& extractedRoot);
    
    /**
     * Check if the VFS is initialized.
     */
    bool IsInitialized();
    
    /**
     * Get the extracted root directory.
     */
    std::filesystem::path GetExtractedRoot();
    
    /**
     * Resolve a guest (Xbox 360) path to a host file path.
     * 
     * @param guestPath The Xbox 360 path (e.g., "game:\common\shaders\fxl_final\gta_default.fxc")
     * @return The resolved host path, or empty path if not found
     * 
     * Examples:
     *   "game:\" → "/path/to/extracted/"
     *   "game:\common\shaders\fxl_final\gta_default.fxc" → "/path/to/extracted/common/shaders/fxl_final/gta_default.fxc"
     *   "fxl_final\" → "/path/to/extracted/common/shaders/fxl_final/"
     */
    std::filesystem::path Resolve(const std::string& guestPath);
    
    /**
     * Check if a guest path exists in the extracted directory.
     */
    bool Exists(const std::string& guestPath);
    
    /**
     * Check if a guest path is a directory.
     */
    bool IsDirectory(const std::string& guestPath);
    
    /**
     * Get the file size for a guest path (0 if not found or is directory).
     */
    uint64_t GetFileSize(const std::string& guestPath);
    
    /**
     * Normalize a guest path (lowercase, convert backslashes to forward slashes).
     */
    std::string NormalizePath(const std::string& guestPath);
    
    /**
     * Strip the drive prefix from a guest path.
     * "game:\foo\bar" → "foo/bar"
     * "d:\foo\bar" → "foo/bar"
     */
    std::string StripDrivePrefix(const std::string& guestPath);
    
    /**
     * Special path mappings for GTA IV.
     * Some paths need special handling (e.g., fxl_final → common/shaders/fxl_final)
     */
    struct PathMapping
    {
        std::string guestPrefix;   // Guest path prefix to match
        std::string hostPrefix;    // Host path prefix to replace with
    };
    
    /**
     * Add a special path mapping.
     */
    void AddPathMapping(const std::string& guestPrefix, const std::string& hostPrefix);
    
    /**
     * Clear all path mappings and reset to defaults.
     */
    void ResetPathMappings();
    
    /**
     * Scan the extracted directory and build a file index for fast lookups.
     * Called automatically during Initialize(), but can be called again
     * if files are added/removed.
     */
    void RebuildIndex();
    
    /**
     * Get statistics about the VFS.
     */
    struct Stats
    {
        uint64_t totalFiles = 0;
        uint64_t totalDirectories = 0;
        uint64_t totalBytes = 0;
        uint64_t cacheHits = 0;
        uint64_t cacheMisses = 0;
    };
    Stats GetStats();
}
