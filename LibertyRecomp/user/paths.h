#pragma once

#include <mod/mod_loader.h>
#include <install/embedded_assets.h>

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
#if defined(LIBERTY_RECOMP_EMBEDDED_ASSETS)
    // Embedded build (PS4 /app0, Switch romfs:, iOS bundle, Android internal)
    // EmbeddedAssets::GetGameRoot() resolves the platform-specific game root.
    return EmbeddedAssets::GetGameRoot().parent_path(); // parent = package root
#else
    return GetUserPath();
#endif
}

/// On PS4, the save directory is the SCE SaveData mount point (e.g. "/savedata0").
/// This global is set by SaveSystem::Initialize() when the container is mounted.
/// Declared here so GetSavePath() can read it without pulling in save_system.h.
#ifdef LIBERTY_RECOMP_PS4
inline std::string g_ps4SaveMountPoint;
#endif

/// On Switch, the save directory is the libnx fsdev mount point (typically "save:/").
/// This global is set by SaveSystem::Initialize() after fsdevMountSaveData succeeds,
/// and consumed by GetSavePath() so stdio-based I/O resolves through the journalled
/// save-data container instead of the read-only romfs/user path.
#ifdef LIBERTY_RECOMP_NX
inline std::string g_switchSaveMountPoint;
#endif

inline std::filesystem::path GetSavePath(bool checkForMods)
{
    if (checkForMods && !ModLoader::s_saveFilePath.empty())
        return ModLoader::s_saveFilePath.parent_path();
#ifdef LIBERTY_RECOMP_PS4
    if (!g_ps4SaveMountPoint.empty())
        return std::filesystem::path(g_ps4SaveMountPoint);
#endif
#ifdef LIBERTY_RECOMP_NX
    if (!g_switchSaveMountPoint.empty())
        return std::filesystem::path(g_switchSaveMountPoint);
#endif
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
    std::error_code ec;
    constexpr size_t kMaxEntries =
#if defined(__ORBIS__) || defined(__SWITCH__)
        // Console encrypted/romfs filesystems: cap iteration to avoid
        // slow recursive enumeration on /app0 or romfs:/
        8192;
#else
        SIZE_MAX; // Desktop: no practical limit
#endif

    size_t count = 0;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(gamePath, ec)) {
        if (ec) break;
        if (count >= kMaxEntries) break;
        std::string fullPath = entry.path().string();
        std::string key = toLower(fullPath);
        g_pathCache[key] = entry.path();
        ++count;
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
