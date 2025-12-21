#pragma once

#include <mod/mod_loader.h>

#define USER_DIRECTORY "LibertyRecomp"

#ifndef GAME_INSTALL_DIRECTORY
#define GAME_INSTALL_DIRECTORY "."
#endif

extern std::filesystem::path g_executableRoot;
inline std::unordered_map<std::string, std::filesystem::path> g_pathCache;

bool CheckPortable();
std::filesystem::path BuildUserPath();
const std::filesystem::path& GetUserPath();

inline std::filesystem::path GetGamePath()
{
    // Returns the game install directory
    // macOS: ~/Library/Application Support/LibertyRecomp/
    // Structure:
    //   game/           - extracted game files (common/, xbox360/, audio/)
    //   game/default.xex - the executable
    //   shader_cache/   - compiled shaders
    //   saves/          - save files
    return GetUserPath();
}

inline std::filesystem::path GetSavePath(bool checkForMods)
{
    if (checkForMods && !ModLoader::s_saveFilePath.empty())
        return ModLoader::s_saveFilePath.parent_path();
    else
        return GetUserPath() / "save";
}

// Returned file name may not necessarily be
// equal to SYS-DATA as mods can assign anything.
inline std::filesystem::path GetSaveFilePath(bool checkForMods)
{
    if (checkForMods && !ModLoader::s_saveFilePath.empty())
        return ModLoader::s_saveFilePath;
    else
        return GetSavePath(false) / "GTA4SaveData.bin";
}

static std::string toLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
    return str;
};

inline void BuildPathCache(const std::string& gamePath) {
    for (const auto& entry : std::filesystem::recursive_directory_iterator(gamePath)) {
        std::string fullPath = entry.path().string();
        std::string key = toLower(fullPath);
        g_pathCache[key] = entry.path();
    }
}

inline std::filesystem::path FindInPathCache(const std::string& targetPath) {
    std::string key = toLower(targetPath);
    auto it = g_pathCache.find(key);
    if (it != g_pathCache.end()) {
        return it->second;
    }
    return {};
}
