#include "paths.h"
#include <os/process.h>

std::filesystem::path g_executableRoot = os::process::GetExecutableRoot();
std::filesystem::path g_userPath = BuildUserPath();

bool CheckPortable()
{
    return std::filesystem::exists(g_executableRoot / "portable.txt");
}

std::filesystem::path BuildUserPath()
{
    // ── Embedded platforms: fixed OS-defined paths ────────────────────────────
#if defined(__ORBIS__)
    // PS4: read-only game data at /app0, writable user data at /user/data/<TITLEID>
    return std::filesystem::path("/user/data/LBTY00001");

#elif defined(__SWITCH__)
    // Switch: save-data device mounted as save:/ by switch_main.cpp __appInit
    // SD card general storage lives at sdmc:/LibertyRecomp/
    return std::filesystem::path("sdmc:/LibertyRecomp");

#elif defined(__ANDROID__)
    // Android: internal storage path injected by JNI before SDL starts
    extern const char* g_androidAppInternalPath;
    if (g_androidAppInternalPath)
        return std::filesystem::path(g_androidAppInternalPath) / "LibertyRecomp";
    return std::filesystem::path("/data/user/0/com.libertyrecomp/files/LibertyRecomp");

#elif defined(TARGET_OS_IOS) && TARGET_OS_IOS
    // iOS sandbox: Documents directory
    extern "C" const char* LIBERTY_IOS_GetDocumentsPath();
    const char* docs = LIBERTY_IOS_GetDocumentsPath();
    if (docs)
        return std::filesystem::path(docs) / "LibertyRecomp";
    return std::filesystem::path("LibertyRecomp");

    // ── Desktop platforms ─────────────────────────────────────────────────────
#elif defined(_WIN32)
    if (CheckPortable())
        return g_executableRoot;
    PWSTR knownPath = NULL;
    std::filesystem::path result;
    if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &knownPath) == S_OK)
        result = std::filesystem::path{ knownPath } / USER_DIRECTORY;
    CoTaskMemFree(knownPath);
    return result;

#elif defined(__linux__) || defined(__APPLE__)
    if (CheckPortable())
        return g_executableRoot;

    const char* homeDir = getenv("HOME");
#if defined(__linux__)
    if (!homeDir)
        homeDir = getpwuid(getuid())->pw_dir;
#endif
    if (homeDir) {
        std::filesystem::path homePath = homeDir;
#if defined(__linux__)
        std::filesystem::path configPath = homePath / ".config";
#else
        std::filesystem::path configPath = homePath / "Library" / "Application Support";
#endif
        if (std::filesystem::exists(configPath))
            return configPath / USER_DIRECTORY;
        return homePath / ("." USER_DIRECTORY);
    }
    return std::filesystem::path(USER_DIRECTORY);

#else
#  error "BuildUserPath() not implemented for this platform."
#endif
}

const std::filesystem::path& GetUserPath()
{
    return g_userPath;
}
