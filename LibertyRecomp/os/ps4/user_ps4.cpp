#ifdef LIBERTY_RECOMP_PS4
#include <os/user.h>
bool os::user::IsDarkTheme()
{
    return true; // PS4 UI is always dark
}
#endif // LIBERTY_RECOMP_PS4
