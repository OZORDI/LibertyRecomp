#include <rex/platform.h>
#if REX_PLATFORM_NX
#include <os/user.h>
bool os::user::IsDarkTheme()
{
    return true; // Switch UI defaults to dark
}
#endif // REX_PLATFORM_NX
