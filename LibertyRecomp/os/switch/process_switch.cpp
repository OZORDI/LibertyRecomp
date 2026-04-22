#include <rex/platform.h>
#if REX_PLATFORM_NX
// Switch process helpers (libnx)
#include <os/process.h>
#include <switch.h>
#include <unistd.h>

std::filesystem::path os::process::GetExecutablePath()
{
    // On Switch homebrew, envGetLoaderInfo() returns the path of the current NRO.
    // envGetNextLoadPath() is wrong here — it returns the chainload target, not us.
    const char* path = envGetLoaderInfo();
    if (path && path[0] != '\0')
        return std::filesystem::path(path);
    return std::filesystem::path("romfs:/");
}

std::filesystem::path os::process::GetExecutableRoot()
{
    // Assets are mounted as RomFS
    return std::filesystem::path("romfs:/");
}

std::filesystem::path os::process::GetWorkingDirectory()
{
    char buf[FS_MAX_PATH] = {};
    if (getcwd(buf, sizeof(buf)))
        return std::filesystem::path(buf);
    return std::filesystem::path("sdmc:/LibertyRecomp/");
}

bool os::process::SetWorkingDirectory(const std::filesystem::path& path)
{
    return chdir(path.c_str()) == 0;
}

bool os::process::StartProcess(const std::filesystem::path&, const std::vector<std::string>&, std::filesystem::path)
{
    // Homebrew cannot freely spawn processes; could use envSetNextLoad() for relaunch
    return false;
}

void os::process::CheckConsole()
{
    g_consoleVisible = false;
}

void os::process::ShowConsole()
{
    // No-op on Switch
}
#endif // REX_PLATFORM_NX
