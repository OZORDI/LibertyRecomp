#ifdef LIBERTY_RECOMP_PS4
#include <os/media.h>
bool os::media::IsExternalMediaPlaying()
{
    // TODO: query SCE audio routing
    return false;
}
#endif // LIBERTY_RECOMP_PS4
