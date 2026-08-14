#pragma once

#include <filesystem>
#include <string>

/**
 * Cross-platform path resolution for LibertyRecomp installation directories.
 * 
 * Platform-specific install directories:
 * - Windows: %LOCALAPPDATA%\LibertyRecomp\
 * - Linux:   ~/.local/share/LibertyRecomp/ (XDG compliant)
 * - macOS:   ~/Library/Application Support/LibertyRecomp/
 */
namespace PlatformPaths
{
    /**
     * Get the base installation directory for the current platform.
     * Creates the directory if it doesn't exist.
     */
    std::filesystem::path GetInstallDirectory();
    
    /**
     * Get the game files directory (where extracted game content goes).
     * Returns: <install_dir>/game/
     * On PS4 this returns /app0/game (read-only PKG mount); use
     * GetWritableCacheDirectory() for any runtime writes.
     */
    std::filesystem::path GetGameDirectory();

    /**
     * Get a writable directory for runtime-generated files that would
     * otherwise go under the game tree (e.g. button-prompt XTD cache).
     * On PS4 this lives under /user/data/<title> which is writable, while
     * GetGameDirectory() points at /app0 which is read-only.
     * On desktop platforms this sits next to the game install.
     */
    std::filesystem::path GetWritableCacheDirectory();

    /**
     * Get the shader cache directory.
     * Returns: <install_dir>/shader_cache/
     */
    std::filesystem::path GetShaderCacheDirectory();

    /**
     * Get the saved-game data directory.
     * Returns: <install_dir>/saves/
     */
    std::filesystem::path GetSavesDirectory();
    
    /**
     * Get temporary directory for extraction operations.
     * Returns: <install_dir>/temp/
     */
    std::filesystem::path GetTempDirectory();
    
    /**
     * Get the extracted RPF content directory.
     * Returns: <install_dir>/game/extracted/
     */
    std::filesystem::path GetExtractedRpfDirectory();
    
    /**
     * Get the path where the AES key should be stored/found.
     * Returns: <install_dir>/aes_key.bin
     */
    std::filesystem::path GetAesKeyPath();
    
    /**
     * Get the path to the bundled AES key shipped with the application.
     * This is used at install time to decrypt RPF archives.
     * Returns: <app_bundle>/LibertyRecompLib/private/aes_key.bin or equivalent
     */
    std::filesystem::path GetBundledAesKeyPath();
    
    /**
     * Ensure all required directories exist.
     * Creates them if they don't.
     */
    void EnsureDirectoriesExist();
    
    /**
     * Clean up temporary files.
     */
    void CleanupTemp();
    
    /**
     * Get platform name as string ("Windows", "Linux", "macOS").
     */
    std::string GetPlatformName();
}
