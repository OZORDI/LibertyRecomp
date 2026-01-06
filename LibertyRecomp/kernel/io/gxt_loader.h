#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <cstdint>

/**
 * GXT Loader for LibertyRecomp
 * 
 * Loads custom GXT text files for localization mods.
 * GTA IV uses GXT2 format for text strings.
 * 
 * File structure:
 *   - GXT2 files contain Jenkins hash -> string mappings
 *   - FusionFix supports custom GXT files with suffixes (e.g., american_FF.gxt)
 *   - Custom entries override base game strings
 */
namespace GxtLoader
{
    /**
     * GXT entry - hash to string mapping.
     */
    struct GxtEntry
    {
        uint32_t hash;
        std::wstring text;
    };

    /**
     * Initialize the GXT loader.
     */
    void Initialize();

    /**
     * Check if initialized.
     */
    bool IsInitialized();

    /**
     * Load a GXT file and merge entries into the custom table.
     * 
     * @param path Path to GXT file
     * @return Number of entries loaded
     */
    size_t LoadGxtFile(const std::filesystem::path& path);

    /**
     * Scan mod overlays for custom GXT files.
     * Looks for files matching: common/text/*_FF.gxt, *_RR.gxt, etc.
     */
    void ScanForCustomGxt(const std::filesystem::path& overlayRoot);

    /**
     * Look up a custom text entry by hash.
     * 
     * @param hash Jenkins hash of the text key
     * @return Custom text if found, nullopt if not overridden
     */
    std::optional<std::wstring> GetCustomText(uint32_t hash);

    /**
     * Look up a custom text entry by key string.
     * Computes Jenkins hash internally.
     */
    std::optional<std::wstring> GetCustomText(const std::string& key);

    /**
     * Get all custom text entries.
     */
    const std::unordered_map<uint32_t, std::wstring>& GetAllCustomEntries();

    /**
     * Clear all custom entries.
     */
    void ClearCustomEntries();

    /**
     * Compute Jenkins hash for a string.
     * Used by GTA IV for text key lookups.
     */
    uint32_t JenkinsHash(const char* str);

    /**
     * Get statistics.
     */
    struct Stats
    {
        uint64_t filesLoaded = 0;
        uint64_t entriesLoaded = 0;
        uint64_t lookups = 0;
        uint64_t hits = 0;
    };
    Stats GetStats();
}
