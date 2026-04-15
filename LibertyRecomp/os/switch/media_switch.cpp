#ifdef LIBERTY_RECOMP_NX
#include <os/media.h>
bool os::media::IsExternalMediaPlaying()
{
    return false;
}
#endif // LIBERTY_RECOMP_NX
