#include "app.h"
// #include <api/SWA.h>
#include <gpu/video.h>
#include <cstdio>
#include <install/installer.h>
#include <kernel/function.h>
#include <os/process.h>
#include <os/logger.h>
#include <patches/audio_patches.h>
#include <patches/inspire_patches.h>
#include <ui/game_window.h>
#include <user/config.h>
#include <user/paths.h>
#include <user/registry.h>

void App::Restart(std::vector<std::string> restartArgs)
{
    os::process::StartProcess(os::process::GetExecutablePath(), restartArgs, os::process::GetWorkingDirectory());
    Exit();
}

void App::Exit()
{
    Config::Save();

#ifdef _WIN32
    timeEndPeriod(1);
#endif

    std::_Exit(0);
}

PPC_FUNC_IMPL(__imp__sub_828B2BF8);
PPC_FUNC(sub_828B2BF8)
{
    LOGN_WARNING("sub_828B2BF8 call");

    __imp__sub_828B2BF8(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_824A7598);
PPC_FUNC(sub_824A7598)
{
    LOGN_WARNING("sub_824A7598 RUN INTRO");

    __imp__sub_824A7598(ctx, base);
}


// // SWA::CApplication::CApplication
// PPC_FUNC_IMPL(__imp__sub_824EB490);
// PPC_FUNC(sub_824EB490)
PPC_FUNC_IMPL(__imp__sub_825B2160);
PPC_FUNC(sub_825B2160)
{
    LOGN_WARNING("Asdasdadasdasd");
    printf("it should work?\n");
    App::s_isInit = true;
    App::s_isMissingDLC = true;
    App::s_language = Config::Language;

    // SWA::SGlobals::Init();
    Registry::Save();

    __imp__sub_825B2160(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_825822D0);
PPC_FUNC(sub_825822D0)
{
    LOGN_WARNING("sub_825822D0");

    __imp__sub_825822D0(ctx, base);
}

static std::thread::id g_mainThreadId = std::this_thread::get_id();

// // SWA::CApplication::Update
PPC_FUNC_IMPL(__imp__sub_825EA610);
PPC_FUNC(sub_825EA610)
{
    __imp__sub_825EA610(ctx, base);
    // LOGN_WARNING("::Update");
    Video::WaitOnSwapChain();

    // Correct small delta time errors.
    if (Config::FPS >= FPS_MIN && Config::FPS < FPS_MAX)
    {
        double targetDeltaTime = 1.0 / Config::FPS;

        if (abs(ctx.f1.f64 - targetDeltaTime) < 0.00001)
            ctx.f1.f64 = targetDeltaTime;
    }

    App::s_deltaTime = ctx.f1.f64;

    printf("s_deltaTime = %f\n", ctx.f1.f64);
    App::s_time += App::s_deltaTime;

//     // This function can also be called by the loading thread,
//     // which SDL does not like. To prevent the OS from thinking
//     // the process is unresponsive, we will flush while waiting
//     // for the pipelines to finish compiling in video.cpp.
    if (std::this_thread::get_id() == g_mainThreadId)
    {
        SDL_PumpEvents();
        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
        GameWindow::Update();
    }

//     AudioPatches::Update(App::s_deltaTime);
//     InspirePatches::Update();

//     // Apply subtitles option.
//     if (auto pApplicationDocument = SWA::CApplicationDocument::GetInstance())
//         pApplicationDocument->m_InspireSubtitles = Config::Subtitles;

//     if (Config::EnableEventCollisionDebugView)
//         *SWA::SGlobals::ms_IsTriggerRender = true;

//     if (Config::EnableGIMipLevelDebugView)
//         *SWA::SGlobals::ms_VisualizeLoadedLevel = true;

//     if (Config::EnableObjectCollisionDebugView)
//         *SWA::SGlobals::ms_IsObjectCollisionRender = true;

//     if (Config::EnableStageCollisionDebugView)
//         *SWA::SGlobals::ms_IsCollisionRender = true;

    // __imp__sub_825EA610(ctx, base);
}


void DebugZlibMidAsmHook(PPCRegister& id)
{
    // printf("DebugZlibMidAsmHook: %x\n", id.u64);
}

void DebugZlibMidAsm2Hook(PPCRegister& id)
{
    if (id.u64 == 10) {
        printf("DebugZlibMidAsm2Hook: %d\n", id.u64);
    }
}

void DebugIndirectCall(PPCRegister& id)
{
    printf("DebugIndirectCall: %x\n", id.u64);
}

