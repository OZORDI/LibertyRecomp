#include <app.h>
#include <install/installer.h>
#include <kernel/function.h>
#include <ui/window.h>
#include <patches/audio_patches.h>

bool g_isAppInit = false;
bool g_isMissingDLC = false;

double g_deltaTime;

// CApplication::Ctor
PPC_FUNC_IMPL(__imp__sub_824EB490);
PPC_FUNC(sub_824EB490)
{
    g_isAppInit = true;
    g_isMissingDLC = !Installer::checkAllDLC(GetGamePath());

    __imp__sub_824EB490(ctx, base);
}

// CApplication::Update
PPC_FUNC_IMPL(__imp__sub_822C1130);
PPC_FUNC(sub_822C1130)
{
    g_isGameLoaded = true;
    g_deltaTime = ctx.f1.f64;

    SDL_PumpEvents();
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

    Window::Update();
    AudioPatches::Update(g_deltaTime);

    __imp__sub_822C1130(ctx, base);
}
