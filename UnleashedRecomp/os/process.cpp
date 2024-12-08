#include <os/process.h>
#include <os/process_detail.h>

std::filesystem::path os::process::GetExecutablePath()
{
    return detail::GetExecutablePath();
}

std::filesystem::path os::process::GetWorkingDirectory()
{
    return detail::GetWorkingDirectory();
}

bool os::process::StartProcess(const std::filesystem::path path, const std::vector<std::string> args, std::filesystem::path work)
{
    return detail::StartProcess(path, args, work);
}
