#include "platform_paths.h"

#include <cstdlib>

#if defined(_WIN32)
#include <shlobj.h>
#include <windows.h>
#elif defined(__APPLE__)
#include <pwd.h>
#include <unistd.h>
#include <mach-o/dyld.h>
#else // Linux
#include <pwd.h>
#include <unistd.h>
#endif

namespace PlatformPaths
{
    static std::filesystem::path s_installDir;
    static bool s_initialized = false;
    
    static std::filesystem::path GetHomeDirectory()
    {
#if defined(_WIN32)
        char path[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path)))
        {
            return std::filesystem::path(path);
        }
        // Fallback
        const char* appdata = std::getenv("LOCALAPPDATA");
        if (appdata) return std::filesystem::path(appdata);
        return std::filesystem::path(".");
#elif defined(__APPLE__)
        const char* home = std::getenv("HOME");
        if (home) return std::filesystem::path(home);
        struct passwd* pw = getpwuid(getuid());
        if (pw) return std::filesystem::path(pw->pw_dir);
        return std::filesystem::path(".");
#else // Linux
        // Check XDG_DATA_HOME first (XDG Base Directory Specification)
        const char* xdgData = std::getenv("XDG_DATA_HOME");
        if (xdgData && xdgData[0] != '\0')
        {
            return std::filesystem::path(xdgData);
        }
        // Fall back to ~/.local/share
        const char* home = std::getenv("HOME");
        if (home) return std::filesystem::path(home) / ".local" / "share";
        struct passwd* pw = getpwuid(getuid());
        if (pw) return std::filesystem::path(pw->pw_dir) / ".local" / "share";
        return std::filesystem::path(".");
#endif
    }
    
    std::filesystem::path GetInstallDirectory()
    {
        if (!s_initialized)
        {
#if defined(_WIN32)
            // Windows: %LOCALAPPDATA%\LibertyRecomp\
            s_installDir = GetHomeDirectory() / "LibertyRecomp";
#elif defined(__APPLE__)
            // macOS: ~/Library/Application Support/LibertyRecomp/
            s_installDir = GetHomeDirectory() / "Library" / "Application Support" / "LibertyRecomp";
#else // Linux
            // Linux: ~/.local/share/LibertyRecomp/ (XDG compliant)
            s_installDir = GetHomeDirectory() / "LibertyRecomp";
#endif
            s_initialized = true;
        }
        return s_installDir;
    }
    
    std::filesystem::path GetGameDirectory()
    {
        return GetInstallDirectory() / "game";
    }
    
    std::filesystem::path GetShaderCacheDirectory()
    {
        return GetInstallDirectory() / "shader_cache";
    }
    
    std::filesystem::path GetTempDirectory()
    {
        return GetInstallDirectory() / "temp";
    }
    
    std::filesystem::path GetExtractedRpfDirectory()
    {
        return GetGameDirectory() / "extracted";
    }
    
    std::filesystem::path GetAesKeyPath()
    {
        return GetInstallDirectory() / "aes_key.bin";
    }
    
    std::filesystem::path GetBundledAesKeyPath()
    {
        // The AES key is bundled in LibertyRecompLib/private/aes_key.bin
        // At runtime, we need to find it relative to the executable
#if defined(__APPLE__)
        // On macOS, it's in the app bundle: .app/Contents/Resources/aes_key.bin
        // or relative to executable for development builds
        std::error_code ec;
        
        // Try app bundle Resources first
        char path[1024];
        uint32_t size = sizeof(path);
        if (_NSGetExecutablePath(path, &size) == 0)
        {
            std::filesystem::path execPath(path);
            // .app/Contents/MacOS/LibertyRecomp -> .app/Contents/Resources/
            auto resourcesPath = execPath.parent_path().parent_path() / "Resources" / "aes_key.bin";
            if (std::filesystem::exists(resourcesPath, ec))
            {
                return resourcesPath;
            }
        }
        
        // Development build: relative to project root
        // Try common development paths
        std::vector<std::filesystem::path> devPaths = {
            "LibertyRecompLib/private/aes_key.bin",
            "../LibertyRecompLib/private/aes_key.bin",
            "../../LibertyRecompLib/private/aes_key.bin",
            "../../../LibertyRecompLib/private/aes_key.bin",
        };
        
        for (const auto& p : devPaths)
        {
            if (std::filesystem::exists(p, ec))
            {
                return std::filesystem::absolute(p);
            }
        }
        
        // Last resort: check install directory
        return GetAesKeyPath();
#elif defined(_WIN32)
        // On Windows, bundled next to executable or in install
        std::error_code ec;
        wchar_t path[MAX_PATH];
        GetModuleFileNameW(nullptr, path, MAX_PATH);
        std::filesystem::path execPath(path);
        
        auto bundledPath = execPath.parent_path() / "aes_key.bin";
        if (std::filesystem::exists(bundledPath, ec))
        {
            return bundledPath;
        }
        
        // Development paths
        std::vector<std::filesystem::path> devPaths = {
            "LibertyRecompLib/private/aes_key.bin",
            "../LibertyRecompLib/private/aes_key.bin",
        };
        
        for (const auto& p : devPaths)
        {
            if (std::filesystem::exists(p, ec))
            {
                return std::filesystem::absolute(p);
            }
        }
        
        return GetAesKeyPath();
#else
        // Linux: next to executable or development path
        std::error_code ec;
        auto execPath = std::filesystem::read_symlink("/proc/self/exe", ec);
        if (!ec)
        {
            auto bundledPath = execPath.parent_path() / "aes_key.bin";
            if (std::filesystem::exists(bundledPath, ec))
            {
                return bundledPath;
            }
        }
        
        std::vector<std::filesystem::path> devPaths = {
            "LibertyRecompLib/private/aes_key.bin",
            "../LibertyRecompLib/private/aes_key.bin",
        };
        
        for (const auto& p : devPaths)
        {
            if (std::filesystem::exists(p, ec))
            {
                return std::filesystem::absolute(p);
            }
        }
        
        return GetAesKeyPath();
#endif
    }
    
    void EnsureDirectoriesExist()
    {
        std::error_code ec;
        std::filesystem::create_directories(GetInstallDirectory(), ec);
        std::filesystem::create_directories(GetGameDirectory(), ec);
        std::filesystem::create_directories(GetShaderCacheDirectory(), ec);
        std::filesystem::create_directories(GetTempDirectory(), ec);
        std::filesystem::create_directories(GetExtractedRpfDirectory(), ec);
    }
    
    void CleanupTemp()
    {
        std::error_code ec;
        std::filesystem::remove_all(GetTempDirectory(), ec);
        std::filesystem::create_directories(GetTempDirectory(), ec);
    }
    
    std::string GetPlatformName()
    {
#if defined(_WIN32)
        return "Windows";
#elif defined(__APPLE__)
        return "macOS";
#else
        return "Linux";
#endif
    }
}
