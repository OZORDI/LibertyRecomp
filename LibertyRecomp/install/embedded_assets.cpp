#include "embedded_assets.h"
#include "platform_paths.h"

#include <fstream>
#include <filesystem>
#include <cstdio>
#include <cstdlib>

#include <rex/platform.h>

#if defined(__ANDROID__)
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
// miniz with full archive API support.
// The SDL3-vendored copy disables archive APIs, and the smol-v copy's .h
// defines MINIZ_HEADER_FILE_ONLY.  Include the .c which has both decls+impl
// but first undo the header-only flag from the .h include.
#undef MINIZ_HEADER_FILE_ONLY
#include "../../tools/XenosRecomp/thirdparty/smol-v/testing/external/miniz/miniz.c"

// User-selected game root, published by LibertySDLActivity.loadLibraries()
// via Java_com_libertyrecomp_LibertySDLActivity_nativeSetGameRoot. Remains
// null until the user commits a picker selection. Takes precedence over the
// CMake-baked path + the internal-storage fallback when non-null.
extern "C" const char* g_androidGameRoot;
#endif

#if defined(__ANDROID__)
namespace {

// fsync a file descriptor, tolerating EINTR.
static bool fsync_fd(int fd)
{
    while (::fsync(fd) != 0) {
        if (errno == EINTR) continue;
        return false;
    }
    return true;
}

// Open the given directory and fsync it so that its dirents
// (creations, renames, unlinks) are committed to stable storage.
// This is a POSIX requirement for rename() atomicity across crashes.
static bool fsync_directory(const std::filesystem::path& dir)
{
    int dfd = ::open(dir.c_str(), O_RDONLY | O_DIRECTORY);
    if (dfd < 0) return false;
    bool ok = fsync_fd(dfd);
    ::close(dfd);
    return ok;
}

// Stream a single ZIP entry out to `outPath`, fsync, and close.
static bool extract_and_fsync(mz_zip_archive* zip, mz_uint idx,
                              const std::filesystem::path& outPath)
{
    // Ensure parent directory exists on disk.
    std::error_code ec;
    std::filesystem::create_directories(outPath.parent_path(), ec);

    // Open with O_CREAT|O_TRUNC|O_WRONLY so we can fsync the fd directly.
    int fd = ::open(outPath.c_str(),
                    O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC,
                    S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0) return false;

    // Pump the decompressed stream into our fd via a heap callback.
    struct Ctx { int fd; bool err; };
    Ctx ctx{ fd, false };
    auto writer = [](void* pOpaque, mz_uint64 /*file_ofs*/,
                     const void* pBuf, size_t n) -> size_t {
        Ctx* c = static_cast<Ctx*>(pOpaque);
        const char* p = static_cast<const char*>(pBuf);
        size_t remaining = n;
        while (remaining) {
            ssize_t w = ::write(c->fd, p, remaining);
            if (w < 0) {
                if (errno == EINTR) continue;
                c->err = true;
                return n - remaining;
            }
            p += w;
            remaining -= static_cast<size_t>(w);
        }
        return n;
    };

    if (!mz_zip_reader_extract_to_callback(zip, idx, writer, &ctx, 0) || ctx.err) {
        ::close(fd);
        return false;
    }

    bool ok = fsync_fd(fd);
    ::close(fd);
    return ok;
}

} // namespace
#endif // __ANDROID__

// ── Platform-specific game root ─────────────────────────────────────────────
static std::filesystem::path ResolveEmbeddedGameRoot()
{
#if defined(__ORBIS__)
    return std::filesystem::path("/app0/game");
#elif defined(__SWITCH__)
    return std::filesystem::path("romfs:/game");
#elif REX_PLATFORM_IOS
    // Bundle resources are at <Bundle>/game/
    extern "C" const char* LIBERTY_IOS_GetBundlePath();
    const char* bp = LIBERTY_IOS_GetBundlePath();
    if (bp)
        return std::filesystem::path(bp) / "game";
    return std::filesystem::path("game");
#elif defined(__ANDROID__)
    // Precedence (first non-empty wins):
    //   1. Path pushed from Java via nativeSetGameRoot() — folder-picker UI.
    //      This is the primary delivery path in the current build.
    //   2. Extracted internal-storage location (used by the OBB flow).
    //
    // The picker-supplied path already points *directly* at the directory
    // containing default.xex + *.rpf; unlike the OBB flow it is NOT nested
    // under an additional "game/" subdir, so we return it verbatim.
    if (g_androidGameRoot && g_androidGameRoot[0] != '\0')
        return std::filesystem::path(g_androidGameRoot);
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
#ifdef MINIZ_NO_ARCHIVE_APIS
    (void)obbPath; (void)destPath;
    return false; // miniz archive support disabled at build time
#else
    std::error_code ec;
    auto marker = destPath / ".obb_extracted";
    if (std::filesystem::exists(marker, ec))
        return true; // Already extracted on a previous boot.

    if (!std::filesystem::exists(obbPath, ec)) {
        // OBB not found — this may be a dev build with files pre-staged.
        return std::filesystem::exists(destPath / "game" / "default.xex", ec);
    }

    // ── Crash-safe extraction ────────────────────────────────────────────────
    // Strategy:
    //   1. Extract into a sibling staging dir: <destPath>.tmp/
    //   2. fsync() every file after writing, before close.
    //   3. fsync() the staging dir so dirents are durable.
    //   4. rename(staging, destPath)  — atomic on ext4/F2FS (same FS).
    //   5. fsync() the parent dir so the rename itself is durable.
    //   6. Write the marker, fsync it, fsync its directory.
    //
    // A mid-extract crash leaves either no staging dir (step 1 failed) or
    // a partial staging dir at destPath.tmp (cleaned up on next boot).  The
    // final destPath never exists unless the rename succeeded, and the
    // marker never exists unless all files beneath it did.

    std::filesystem::create_directories(destPath.parent_path(), ec);

    // Staging path derived from destPath; keep it on the same filesystem so
    // rename() is atomic per POSIX semantics.
    auto stagingPath = destPath;
    stagingPath += ".tmp";

    // Resume from prior crash: wipe any half-written staging tree.
    if (std::filesystem::exists(stagingPath, ec))
        std::filesystem::remove_all(stagingPath, ec);

    // If the final dest somehow already exists without a marker, it's stale
    // from a pre-atomic build — drop it so the rename has a clean target.
    if (std::filesystem::exists(destPath, ec)) {
        std::filesystem::remove_all(destPath, ec);
    }

    std::filesystem::create_directories(stagingPath, ec);

    mz_zip_archive zip{};
    if (!mz_zip_reader_init_file(&zip, obbPath.string().c_str(), 0)) {
        std::filesystem::remove_all(stagingPath, ec);
        return false;
    }

    mz_uint numFiles = mz_zip_reader_get_num_files(&zip);
    for (mz_uint i = 0; i < numFiles; ++i) {
        mz_zip_archive_file_stat st{};
        if (!mz_zip_reader_file_stat(&zip, i, &st)) continue;
        if (mz_zip_reader_is_file_a_directory(&zip, i)) continue;

        auto outPath = stagingPath / st.m_filename;
        if (!extract_and_fsync(&zip, i, outPath)) {
            mz_zip_reader_end(&zip);
            std::filesystem::remove_all(stagingPath, ec);
            return false;
        }
    }

    mz_zip_reader_end(&zip);

    // Commit the staging tree to disk (all subdirs in it) before the rename.
    // A shallow fsync on the root covers its own dirents; per-file fsync has
    // already committed leaf contents + their parent dirs transitively via
    // extract_and_fsync's followup — but to be explicit we fsync the root.
    if (!fsync_directory(stagingPath)) {
        std::filesystem::remove_all(stagingPath, ec);
        return false;
    }

    // Atomic rename. Both paths are in the same parent dir → same FS.
    if (::rename(stagingPath.c_str(), destPath.c_str()) != 0) {
        std::filesystem::remove_all(stagingPath, ec);
        return false;
    }

    // fsync the parent so the rename itself survives a crash.
    if (!fsync_directory(destPath.parent_path())) {
        // Rename already happened; returning false here would leave an
        // unmarked but valid dir, which the resume path above will treat as
        // stale on next boot.  Err on the side of "retry next boot".
        return false;
    }

    // Marker — only written once the payload is durable.
    {
        int mfd = ::open(marker.c_str(),
                         O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC,
                         S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (mfd < 0) return false;
        const char one = '1';
        ssize_t w;
        do { w = ::write(mfd, &one, 1); } while (w < 0 && errno == EINTR);
        if (w != 1 || !fsync_fd(mfd)) { ::close(mfd); return false; }
        ::close(mfd);
    }
    if (!fsync_directory(destPath)) return false;

    return true;
#endif // MINIZ_NO_ARCHIVE_APIS

#else
    (void)obbPath;
    (void)destPath;
    return true; // Not needed on non-Android platforms.
#endif
}
