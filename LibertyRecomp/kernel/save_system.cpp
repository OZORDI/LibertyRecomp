#include <stdafx.h>
#include "save_system.h"
#include "xam.h"
#include <user/paths.h>
#include <fmt/format.h>
#include <fstream>

namespace SaveSystem
{
    // GTA IV save file naming convention: SGTA4XX where XX is slot number (00-15)
    constexpr const char* SAVE_FILE_PREFIX = "SGTA4";
    constexpr uint32_t MAX_SAVE_SLOTS = 16;
    
    std::filesystem::path GetSaveDirectory()
    {
        return GetSavePath(true);
    }
    
    std::filesystem::path GetSaveFilePath(uint32_t slotNumber)
    {
        if (slotNumber >= MAX_SAVE_SLOTS)
        {
            printf("[SaveSystem] ERROR: Invalid save slot number %u (max %u)\n", slotNumber, MAX_SAVE_SLOTS - 1);
            return {};
        }
        
        std::string filename = fmt::format("{}{:02d}", SAVE_FILE_PREFIX, slotNumber);
        return GetSaveDirectory() / filename;
    }
    
    bool SaveSlotExists(uint32_t slotNumber)
    {
        auto path = GetSaveFilePath(slotNumber);
        return !path.empty() && std::filesystem::exists(path);
    }
    
    std::vector<std::string> EnumerateSaveFiles()
    {
        std::vector<std::string> saveFiles;
        auto saveDir = GetSaveDirectory();
        
        if (!std::filesystem::exists(saveDir))
        {
            printf("[SaveSystem] Save directory does not exist: %s\n", saveDir.string().c_str());
            return saveFiles;
        }
        
        std::error_code ec;
        for (const auto& entry : std::filesystem::directory_iterator(saveDir, ec))
        {
            if (entry.is_regular_file())
            {
                std::string filename = entry.path().filename().string();
                // Check if filename starts with SGTA4
                if (filename.find(SAVE_FILE_PREFIX) == 0)
                {
                    saveFiles.push_back(filename);
                    printf("[SaveSystem] Found save file: %s\n", filename.c_str());
                }
            }
        }
        
        printf("[SaveSystem] Enumerated %zu save files\n", saveFiles.size());
        return saveFiles;
    }
    
    void RegisterSaveSlot(uint32_t slotNumber)
    {
        auto savePath = GetSaveFilePath(slotNumber);
        if (savePath.empty())
            return;
            
        std::string filename = savePath.filename().string();
        std::string saveDir = savePath.parent_path().string();
        
        // Register with content system
        XamRegisterContent(XamMakeContent(XCONTENTTYPE_SAVEDATA, filename), saveDir);
        
        printf("[SaveSystem] Registered save slot %u: %s -> %s\n", 
               slotNumber, filename.c_str(), saveDir.c_str());
    }
    
    bool ImportSaveFile(const std::filesystem::path& sourcePath, uint32_t slotNumber)
    {
        if (!std::filesystem::exists(sourcePath))
        {
            printf("[SaveSystem] ERROR: Source save file does not exist: %s\n", sourcePath.string().c_str());
            return false;
        }
        
        auto destPath = GetSaveFilePath(slotNumber);
        if (destPath.empty())
            return false;
        
        std::error_code ec;
        std::filesystem::copy_file(sourcePath, destPath, 
                                   std::filesystem::copy_options::overwrite_existing, ec);
        
        if (ec)
        {
            printf("[SaveSystem] ERROR: Failed to import save file: %s\n", ec.message().c_str());
            return false;
        }
        
        printf("[SaveSystem] Imported save file: %s -> slot %u\n", 
               sourcePath.filename().string().c_str(), slotNumber);
        
        // Register the newly imported save
        RegisterSaveSlot(slotNumber);
        
        return true;
    }
    
    void Initialize()
    {
        printf("[SaveSystem] ========================================\n");
        printf("[SaveSystem] Initializing GTA IV Save System\n");
        printf("[SaveSystem] ========================================\n");
        
        // Get save directory path
        auto saveDir = GetSaveDirectory();
        printf("[SaveSystem] Save directory: %s\n", saveDir.string().c_str());
        
        // Create save directory if it doesn't exist
        std::error_code ec;
        std::filesystem::create_directories(saveDir, ec);
        
        if (ec)
        {
            printf("[SaveSystem] ERROR: Failed to create save directory: %s\n", ec.message().c_str());
            return;
        }
        
        printf("[SaveSystem] Save directory created/verified\n");
        
        // Enumerate existing save files
        auto saveFiles = EnumerateSaveFiles();
        
        // Register all existing save files with content system
        for (uint32_t slot = 0; slot < MAX_SAVE_SLOTS; slot++)
        {
            if (SaveSlotExists(slot))
            {
                RegisterSaveSlot(slot);
            }
        }
        
        // Create root mapping for "SaveData" path used by game
        std::string saveDirStr = saveDir.string();
        XamRootCreate("SaveData", saveDirStr);
        printf("[SaveSystem] Registered root: SaveData -> %s\n", saveDirStr.c_str());
        
        // Also register alternate names the game might use
        XamRootCreate("save", saveDirStr);
        XamRootCreate("saves", saveDirStr);
        
        printf("[SaveSystem] ========================================\n");
        printf("[SaveSystem] Save system initialized successfully\n");
        printf("[SaveSystem] Found %zu existing save files\n", saveFiles.size());
        printf("[SaveSystem] ========================================\n");
    }
}
