#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <optional>

/**
 * Mod Overlay System for LibertyRecomp
 * 
 * Provides FusionFix-compatible mod loading support.
 * Mods can override game files by placing them in overlay directories
 * that mirror the game's file structure.
 * 
 * FusionFix Compatibility:
 *   - Supports `update/` folder structure from FusionFix
 *   - Supports `common/`, `pc/`, `TLAD/`, `TBoGT/` subfolders
 *   - Case-insensitive file matching
 *   - Loose file overrides (no repacking required)
 * 
 * Directory Structure:
 *   <game_root>/
 *   ├── mods/                        # Primary mod overlay folder
 *   │   └── update/                  # FusionFix-style update folder
 *   │       ├── common/              # Overrides for common.rpf contents
 *   │       ├── pc/                  # PC-specific overrides (ignored on Xbox)
 *   │       ├── TLAD/                # The Lost and Damned overrides
 *   │       └── TBoGT/               # Ballad of Gay Tony overrides
 *   └── update/                      # Alternative location (FusionFix default)
 * 
 * Priority (highest to lowest):
 *   1. mods/update/
 *   2. update/
 *   3. Base game files (from VFS extracted root)
 */
namespace ModOverlay
{
    /**
     * Overlay directory entry with priority
     */
    struct OverlayEntry
    {
        std::filesystem::path path;
        int priority;  // Higher = checked first
        bool enabled;
        std::string name;
    };

    /**
     * File override entry
     */
    struct FileOverride
    {
        std::filesystem::path hostPath;     // Actual file path on disk
        std::string normalizedPath;          // Normalized game path
        int overlayPriority;                 // Which overlay it came from
    };

    /**
     * Initialize the mod overlay system.
     * Scans for overlay directories relative to the game root.
     * 
     * @param gameRoot The game installation directory
     */
    void Initialize(const std::filesystem::path& gameRoot);

    /**
     * Check if the mod overlay system is initialized.
     */
    bool IsInitialized();

    /**
     * Add a custom overlay directory.
     * 
     * @param path Path to the overlay directory
     * @param priority Priority (higher = checked first)
     * @param name Human-readable name for logging
     */
    void AddOverlay(const std::filesystem::path& path, int priority = 0, const std::string& name = "");

    /**
     * Remove an overlay directory.
     */
    void RemoveOverlay(const std::filesystem::path& path);

    /**
     * Clear all overlay directories.
     */
    void ClearOverlays();

    /**
     * Get all registered overlay directories.
     */
    std::vector<OverlayEntry> GetOverlays();

    /**
     * Enable or disable an overlay.
     */
    void SetOverlayEnabled(const std::filesystem::path& path, bool enabled);

    /**
     * Rebuild the file index for all overlays.
     * Call this after adding/removing overlays or if mod files change.
     */
    void RebuildIndex();

    /**
     * Resolve a game path through the overlay system.
     * Returns the override file path if one exists, or empty path if not.
     * 
     * @param normalizedPath The normalized game path (lowercase, forward slashes)
     * @return Override file path, or empty if no override exists
     */
    std::filesystem::path Resolve(const std::string& normalizedPath);

    /**
     * Check if an override exists for a path.
     */
    bool HasOverride(const std::string& normalizedPath);

    /**
     * Get all file overrides (for debugging/UI).
     */
    std::vector<FileOverride> GetAllOverrides();

    /**
     * Statistics about the overlay system
     */
    struct Stats
    {
        uint64_t totalOverlays = 0;
        uint64_t enabledOverlays = 0;
        uint64_t totalOverrideFiles = 0;
        uint64_t overrideHits = 0;
        uint64_t overrideMisses = 0;
    };
    Stats GetStats();

    /**
     * Dump overlay status to log (for debugging).
     */
    void DumpStatus();

    /**
     * FusionFix-specific: Scan for FusionFix update folder structure.
     * Automatically detects and registers:
     *   - <gameRoot>/update/
     *   - <gameRoot>/mods/update/
     *   - <gameRoot>/GTAIV.EFLC.FusionFix/update/
     */
    void ScanForFusionFix(const std::filesystem::path& gameRoot);

    /**
     * Map FusionFix subfolder names to VFS paths.
     * FusionFix uses specific folder names that need mapping:
     *   - common/  -> common/
     *   - pc/      -> (skipped for Xbox 360 recomp, but we can support it)
     *   - TLAD/    -> dlc/TLAD/
     *   - TBoGT/   -> dlc/TBoGT/
     */
    std::string MapFusionFixPath(const std::string& fusionFixPath);
}
