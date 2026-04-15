#ifdef LIBERTY_RECOMP_PS4
#include <os/version.h>
#include <orbis/libkernel.h>

os::version::OSVersion os::version::GetOSVersion()
{
    OrbisKernelSwVersion ver{};
    int32_t rc = sceKernelGetSystemSwVersion(&ver);
    if (rc < 0)
    {
        // Failed to query firmware version — return 0.0.0 as unknown
        return { 0, 0, 0 };
    }
    // Version string like "09.00"
    uint32_t major = 0, minor = 0;
    sscanf(ver.VersionString, "%u.%u", &major, &minor);
    return { major, minor, 0 };
}
#endif // LIBERTY_RECOMP_PS4
