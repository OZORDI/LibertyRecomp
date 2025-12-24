#pragma once

#include <filesystem>
#include <string>

/**
 * GTA IV Save System Implementation
 * 
 * GTA IV uses Xbox 360 content system with save files named SGTA4XX (XX = slot number 00-15)
 * This module bridges Xbox 360 save APIs to host filesystem operations.
 * 
 * Save Structure:
 * - Profile Save (vtable: 0x81209104) - Player settings, stats
 * - Game Save (vtable: 0x81209064) - Mission progress, world state
 * - Autosave (vtable: 0x81209028) - Automatic checkpoint saves
 * 
 * Platform-specific save paths:
 * - macOS: ~/Library/Application Support/LibertyRecomp/saves/
 * - Windows: %LOCALAPPDATA%\LibertyRecomp\saves\
 * - Linux: ~/.local/share/LibertyRecomp/saves/
 */

namespace SaveSystem
{
    /**
     * Initialize the save system at startup.
     * Creates save directories and registers default save content.
     * Should be called from KiSystemStartup() before game initialization.
     */
    void Initialize();
    
    /**
     * Enumerate available save files in the save directory.
     * Returns list of save file names (SGTA400, SGTA401, etc.)
     */
    std::vector<std::string> EnumerateSaveFiles();
    
    /**
     * Check if a specific save slot exists.
     * @param slotNumber Save slot number (0-15)
     * @return true if save file exists
     */
    bool SaveSlotExists(uint32_t slotNumber);
    
    /**
     * Get the full path to a save file.
     * @param slotNumber Save slot number (0-15)
     * @return Full filesystem path to save file
     */
    std::filesystem::path GetSaveFilePath(uint32_t slotNumber);
    
    /**
     * Get the save directory path.
     * @return Path to saves directory
     */
    std::filesystem::path GetSaveDirectory();
    
    /**
     * Register a save file with the content system.
     * @param slotNumber Save slot number (0-15)
     */
    void RegisterSaveSlot(uint32_t slotNumber);
    
    /**
     * Copy a save file from external source (for testing/importing).
     * @param sourcePath Path to source save file
     * @param slotNumber Destination slot number (0-15)
     * @return true if copy succeeded
     */
    bool ImportSaveFile(const std::filesystem::path& sourcePath, uint32_t slotNumber);
}
