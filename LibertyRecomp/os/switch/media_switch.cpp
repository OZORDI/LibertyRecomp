#include <rex/platform.h>
#if REX_PLATFORM_NX
#include <os/media.h>
bool os::media::IsExternalMediaPlaying()
{
    return false;
}
#endif // REX_PLATFORM_NX
