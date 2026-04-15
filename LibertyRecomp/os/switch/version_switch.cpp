#ifdef LIBERTY_RECOMP_NX
#include <os/version.h>
#include <switch.h>

os::version::OSVersion os::version::GetOSVersion()
{
    SetSysFirmwareVersion fw{};
    // setsys must be initialized before querying firmware version,
    // otherwise setsysGetFirmwareVersion silently returns zeroes.
    setsysInitialize();
    Result rc = setsysGetFirmwareVersion(&fw);
    setsysExit();
    if (R_SUCCEEDED(rc))
        return { static_cast<uint32_t>(fw.major),
                 static_cast<uint32_t>(fw.minor),
                 static_cast<uint32_t>(fw.micro) };
    return { 0, 0, 0 };
}
#endif // LIBERTY_RECOMP_NX
