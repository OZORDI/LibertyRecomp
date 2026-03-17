#include "embedded_assets.h"
#include "platform_paths.h"

#include <fstream>
#include <filesystem>

// ── Platform-specific game root ─────────────────────────────────────────────
static std::filesystem::path ResolveEmbeddedGameRoot()
{
#if defined(__ORBIS__)
    return std::filesystem::path("/app0/game");
#elif defined(__SWITCH__)
    return std::filesystem::path("romfs:/game");
#elif defined(TARGET_OS_IOS) && TARGET_OS_IOS
    // Bundle resources are at <Bundle>/game/
    extern "C" const char* LIBERTY_IOS_GetBundlePath();
    const char* bp = LIBERTY_IOS_GetBundlePath();
    if (bp)
        return std::filesystem::path(bp) / "game";
    return std::filesystem::path("game");
#elif defined(__ANDROID__)
    // After first-boot extraction, files live in internal storage
    return PlatformPaths::GetGameDirectory();
#else
    // Desktop: embedded path is unused, fall back to normal install dir
    return PlatformPaths::GetGameDirectory();
#endif
}

// ────────────────────────────────────────────────────────────────────────────

bool EmbeddedAssets::IsAvailable()
{
#if defined(LIBERTY_RECOMP_EMBEDDED_ASSETS)
    return true;
#else
    return false;
#endif
}

std::filesystem::path EmbeddedAssets::GetGameRoot()
{
    static std::filesystem::path root = ResolveEmbeddedGameRoot();
    return root;
}

std::filesystem::path EmbeddedAssets::GetDLCRoot()
{
#if defined(LIBERTY_RECOMP_EMBEDDED_ASSETS)
    return GetGameRoot().parent_path() / "dlc";
#else
    return PlatformPaths::GetInstallDirectory() / "dlc";
#endif
}

EmbeddedAssets::Manifest EmbeddedAssets::ReadManifest()
{
    Manifest m;

    // The manifest JSON is written by CMake's configure_file into the package.
    // On embedded platforms it lives next to the game root as manifest.json.
    std::filesystem::path manifestPath = GetGameRoot().parent_path() / "manifest.json";

    std::ifstream f(manifestPath);
    if (!f.is_open())
        return m;

    // Minimal hand-rolled JSON parse to avoid pulling in a JSON library dependency
    std::string line;
    while (std::getline(f, line))
    {
        auto extract = [&](const std::string& key) -> std::string
        {
            auto pos = line.find("\"" + key + "\"");
            if (pos == std::string::npos) return {};
            auto colon = line.find(':', pos);
            if (colon == std::string::npos) return {};
            auto q1 = line.find('"', colon + 1);
            if (q1 == std::string::npos) return {};
            auto q2 = line.find('"', q1 + 1);
            if (q2 == std::string::npos) return {};
            return line.substr(q1 + 1, q2 - q1 - 1);
        };

        if (!extract("gameVersion").empty())     m.gameVersion     = extract("gameVersion");
        if (!extract("titleUpdate").empty())     m.titleUpdateVersion = extract("titleUpdate");
        if (line.find("\"hasTLAD\": true") != std::string::npos)  m.hasTLAD  = true;
        if (line.find("\"hasTBOGT\": true") != std::string::npos) m.hasTBOGT = true;

        {
            auto pos = line.find("\"buildTimestamp\"");
            if (pos != std::string::npos)
            {
                auto colon = line.find(':', pos);
                if (colon != std::string::npos)
                    m.buildTimestamp = static_cast<uint32_t>(std::stoul(line.substr(colon + 1)));
            }
        }
    }

    return m;
}

bool EmbeddedAssets::ExtractObbIfNeeded(const std::filesystem::path& obbPath,
                                         const std::filesystem::path& destPath)
{
#if defined(__ANDROID__)
    std::error_code ec;
    auto marker = destPath / ".obb_extracted";
    if (std::filesystem::exists(marker, ec))
        return true; // Already extracted on a previous boot

    if (!std::filesystem::exists(obbPath, ec)) {
        // OBB not found — this may be a dev build with files pre-staged
        return std::filesystem::exists(destPath / "game" / "default.xex", ec);
    }

    // ── miniz ZIP extraction ──────────────────────────────────────────────────
    // The Android OBB is a ZIP archive with the game payload at its root.
    // We extract it to destPath preserving directory structure.
    #include <cstdio>
    // Use miniz (already in thirdparty via stb or a direct copy).
    // If miniz_zip is not available we fall back to reporting the error.
#ifdef MINIZ_NO_ARCHIVE_APIS
    (void)obbPath; (void)destPath;
    return false; // miniz archive support disabled
#else
    #include "../../thirdparty/stb/miniz.h"
    mz_zip_archive zip{};
    if (!mz_zip_reader_init_file(&zip, obbPath.string().c_str(), 0))
        return false;

    mz_uint numFiles = mz_zip_reader_get_num_files(&zip);
    std::filesystem::create_directories(destPath, ec);

    for (mz_uint i = 0; i < numFiles; ++i) {
        mz_zip_archive_file_stat stat{};
        if (!mz_zip_reader_file_stat(&zip, i, &stat)) continue;
        if (mz_zip_reader_is_file_a_directory(&zip, i)) continue;

        auto outPath = destPath / stat.m_filename;
        std::filesystem::create_directories(outPath.parent_path(), ec);
        if (!mz_zip_reader_extract_to_file(&zip, i, outPath.string().c_str(), 0)) {
            mz_zip_reader_end(&zip);
            return false;
        }
    }

    mz_zip_reader_end(&zip);

    // Write extraction marker
    std::ofstream(marker.string()).put('1');
    return true;
#endif // MINIZ_NO_ARCHIVE_APIS

#else
    (void)obbPath;
    (void)destPath;
    return true; // Not needed on non-Android platforms
#endif
}
