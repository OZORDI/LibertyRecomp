/**
 * auto_installer.cpp
 *
 * Silently installs both DLC packs and applies the latest title update at
 * startup without the wizard UI.
 *
 * DLC installation (large ~1.8GB zips each) runs on a detached background
 * thread so it never blocks game startup.
 *
 * Title update application is fast (~3.5MB STFS, native xexp patcher) and
 * runs synchronously before the game boots so the patched XEX is always
 * in place before execution begins.
 *
 * Config keys (set automatically by the wizard, or manually):
 *   [Install]
 *   DLCSourceDir    = "/path/to/folder/with/dlc/zips"
 *   UpdateSourceDir = "/path/to/folder/with/update/zips"
 */

#include "auto_installer.h"
#include "installer.h"
#include "xbox360/title_update_manager.h"

#include <user/config.h>
#include <os/diag.h>

#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>

// --------------------------------------------------------------------------
// Helpers
// --------------------------------------------------------------------------

static std::vector<std::filesystem::path> FindZips(const std::filesystem::path& dir)
{
    std::vector<std::filesystem::path> result;
    if (!std::filesystem::is_directory(dir))
        return result;
    for (const auto& entry : std::filesystem::directory_iterator(dir))
    {
        if (!entry.is_regular_file()) continue;
        std::string ext = entry.path().extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (ext == ".zip")
            result.push_back(entry.path());
    }
    return result;
}

static std::string ReadMarker(const std::filesystem::path& p)
{
    if (!std::filesystem::exists(p)) return {};
    std::ifstream f(p);
    std::string s;
    std::getline(f, s);
    return s;
}

static void WriteMarker(const std::filesystem::path& p, const std::string& v)
{
    std::error_code ec;
    std::filesystem::create_directories(p.parent_path(), ec);
    std::ofstream f(p);
    f << v;
}

// --------------------------------------------------------------------------
// DLC — runs on a background thread (large zips, slow extraction)
// --------------------------------------------------------------------------

static void DLCWorker(std::filesystem::path dlcSourceDir, std::filesystem::path gamePath)
{
    bool tladMissing  = !Installer::checkDLCInstall(gamePath, DLC::TheLostAndDamned);
    bool tbogtMissing = !Installer::checkDLCInstall(gamePath, DLC::TheBalladOfGayTony);

    if (!tladMissing && !tbogtMissing)
    {
        DIAG_EMIT("[AutoInstall] Both DLC packs already installed.\n");
        return;
    }

    std::list<std::filesystem::path> dlcSources;
    for (const auto& zip : FindZips(dlcSourceDir))
    {
        DLC dlc = Installer::parseDLC(zip);
        if      (dlc == DLC::TheLostAndDamned   && tladMissing)  dlcSources.push_back(zip);
        else if (dlc == DLC::TheBalladOfGayTony && tbogtMissing) dlcSources.push_back(zip);
    }

    if (dlcSources.empty())
    {
        DIAG_EMIT("[AutoInstall] No matching DLC source zips found in: %s\n",
                  dlcSourceDir.string().c_str());
        return;
    }

    DIAG_EMIT("[AutoInstall] Background DLC install starting (%zu pack(s))...\n",
              dlcSources.size());

    Installer::Input input;
    input.dlcSources = dlcSources;

    Journal journal;
    Installer::Sources sources;
    if (!Installer::parseSources(input, journal, sources))
    {
        DIAG_EMIT("[AutoInstall] DLC parse failed: %s\n",
                  journal.lastErrorMessage.c_str());
        return;
    }

    if (!Installer::install(sources, gamePath, /*skipHashChecks=*/true, journal,
                            std::chrono::seconds(0), []() { return true; }))
    {
        DIAG_EMIT("[AutoInstall] DLC install failed: %s\n",
                  journal.lastErrorMessage.c_str());
        return;
    }

    bool tladOk  = Installer::checkDLCInstall(gamePath, DLC::TheLostAndDamned);
    bool tbogtOk = Installer::checkDLCInstall(gamePath, DLC::TheBalladOfGayTony);
    DIAG_EMIT("[AutoInstall] Background DLC install done — TLAD=%d TBOGT=%d\n",
              (int)tladOk, (int)tbogtOk);
}

// --------------------------------------------------------------------------
// Title update — extract STFS content to game dir.
// RexGlue handles delta patching at runtime (looks for default.xexp next
// to default.xex, just like Xenia).  We only need to get the files on disk.
// --------------------------------------------------------------------------

static void ApplyLatestUpdate(
    const std::filesystem::path& updateSourceDir,
    const std::filesystem::path& gamePath)
{
    if (updateSourceDir.empty() || !std::filesystem::is_directory(updateSourceDir))
        return;

    std::filesystem::path gameDir = gamePath / "game";
    if (!std::filesystem::exists(gameDir / "default.xex"))
        return;

    // Skip if already extracted
    std::filesystem::path marker = gamePath / "game" / ".update_applied";
    if (std::filesystem::exists(marker))
    {
        DIAG_EMIT("[AutoInstall] Title update already extracted, skipping.\n");
        return;
    }

    // Unzip all update zips, parse STFS, pick highest version
    liberty::install::TitleUpdateManager mgr;
    for (const auto& zip : FindZips(updateSourceDir))
    {
        std::filesystem::path tempDir =
            std::filesystem::temp_directory_path() / "LibertyRecomp_TU" / zip.stem();
        std::error_code ec;
        std::filesystem::remove_all(tempDir, ec);
        std::filesystem::create_directories(tempDir, ec);

        std::string cmd = "unzip -q -o \"" + zip.string() + "\" -d \"" + tempDir.string() + "\"";
        if (std::system(cmd.c_str()) != 0) continue;

        for (const auto& entry : std::filesystem::recursive_directory_iterator(tempDir, ec))
        {
            if (entry.is_regular_file())
                mgr.AddTitleUpdate(entry.path());
        }
    }

    // Select highest version
    const auto& updates = mgr.GetDetectedUpdates();
    if (updates.empty()) return;

    int bestIdx = 0;
    for (int i = 1; i < static_cast<int>(updates.size()); i++)
    {
        if (updates[i].info.version > updates[bestIdx].info.version)
            bestIdx = i;
    }
    mgr.SelectUpdate(bestIdx);

    auto sel = mgr.GetSelectedUpdate();
    if (!sel) return;

    DIAG_EMIT("[AutoInstall] Extracting title update v%u from STFS...\n",
              sel->info.version);

    // Open the STFS package and extract all files to the game directory.
    // RexGlue will find default.xexp at game/default.xexp and apply the
    // delta patch at XEX load time (same as Xenia).
    liberty::install::StfsParser parser;
    if (!parser.Open(sel->path))
    {
        DIAG_EMIT("[AutoInstall] Failed to open STFS package.\n");
        return;
    }

    int extracted = 0;
    for (const auto& file : parser.GetFileList())
    {
        if (file.isDirectory) continue;

        std::filesystem::path outPath = gameDir / file.name;
        std::error_code ec;
        std::filesystem::create_directories(outPath.parent_path(), ec);

        if (parser.ExtractFileToDisk(file.name, outPath))
            extracted++;
        else
            DIAG_EMIT("[AutoInstall]   WARN: failed to extract %s\n", file.name.c_str());
    }

    DIAG_EMIT("[AutoInstall] Extracted %d files from title update.\n", extracted);
    WriteMarker(marker, std::to_string(sel->info.version));
}

// --------------------------------------------------------------------------
// Entry point
// --------------------------------------------------------------------------

AutoInstallResult AutoInstaller::Run(const std::filesystem::path& gamePath)
{
    AutoInstallResult result;

    std::filesystem::path modulePath;
    if (!Installer::checkGameInstall(gamePath, modulePath))
    {
        DIAG_EMIT("[AutoInstall] Game not installed, skipping.\n");
        return result;
    }

    std::filesystem::path dlcDir    = Config::DLCSourceDir.Value;
    std::filesystem::path updateDir = Config::UpdateSourceDir.Value;

    if (dlcDir.empty() && updateDir.empty())
        return result;

    // Title update: fast, apply now so the patched XEX is ready before boot
    if (!updateDir.empty())
        ApplyLatestUpdate(updateDir, gamePath);

    // DLC: slow (1.8GB zips), run in background — game continues loading
    if (!dlcDir.empty() &&
        (!Installer::checkDLCInstall(gamePath, DLC::TheLostAndDamned) ||
         !Installer::checkDLCInstall(gamePath, DLC::TheBalladOfGayTony)))
    {
        DIAG_EMIT("[AutoInstall] Spawning background DLC install thread.\n");
        std::thread(DLCWorker, dlcDir, gamePath).detach();
        result.tladInstalled  = false; // not yet — happens in background
        result.tbogtInstalled = false;
    }

    return result;
}
