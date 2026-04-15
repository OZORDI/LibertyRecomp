#ifdef LIBERTY_RECOMP_PS4
// PS4 process helpers (OpenOrbis)
#include <os/process.h>
#include <orbis/libkernel.h>
#include <unistd.h>
#include <string.h>

std::filesystem::path os::process::GetExecutablePath()
{
    // Under OpenOrbis the application unpacks to /app0/
    return std::filesystem::path("/app0/eboot.bin");
}

std::filesystem::path os::process::GetExecutableRoot()
{
    return std::filesystem::path("/app0");
}

std::filesystem::path os::process::GetWorkingDirectory()
{
    char buf[1024] = {};
    if (getcwd(buf, sizeof(buf)))
        return std::filesystem::path(buf);
    return std::filesystem::path("/app0");
}

bool os::process::SetWorkingDirectory(const std::filesystem::path& path)
{
    return chdir(path.c_str()) == 0;
}

bool os::process::StartProcess(const std::filesystem::path&, const std::vector<std::string>&, std::filesystem::path)
{
    // PS4 sandbox does not support arbitrary process spawning from homebrew
    return false;
}

void os::process::CheckConsole()
{
    g_consoleVisible = false;
}

void os::process::ShowConsole()
{
    // No console window on PS4 homebrew
}
#endif // LIBERTY_RECOMP_PS4
