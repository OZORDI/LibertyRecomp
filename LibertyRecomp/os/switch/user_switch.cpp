#ifdef LIBERTY_RECOMP_NX
#include <os/user.h>
bool os::user::IsDarkTheme()
{
    return true; // Switch UI defaults to dark
}
#endif // LIBERTY_RECOMP_NX
