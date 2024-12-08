#include <os/version.h>
#include <os/version_detail.h>

os::version::detail::OSVersion os::version::GetOSVersion()
{
    return detail::GetOSVersion();
}
