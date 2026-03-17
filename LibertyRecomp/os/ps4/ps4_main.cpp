// PS4 entry point for LibertyRecomp (OpenOrbis)
// user_malloc_init() MUST run before any heap allocation.

#include <orbis/libkernel.h>
#include <orbis/SystemService.h>
#include <orbis/UserService.h>
#include <orbis/Sysmodule.h>
#include <orbis/NpTrophy.h>
#include "np_conf_ps4.h"

// Forward declarations
extern "C" int  user_malloc_init(void);
extern "C" int  user_malloc_finalize(void);
extern     int  main(int argc, char* argv[]);

// Global NP state — used by os/ps4/trophy_ps4.cpp
int32_t g_ps4_user_id   = 0;
int32_t g_np_context    = 0;
int32_t g_np_handle     = 0;

// ── Sysmodule loading ─────────────────────────────────────────────────────────
// Internal modules needed for audio, pad, video, networking.
// ORBIS_SYSMODULE_INTERNAL_* variants accept uint32_t; cast is intentional.
static void LoadRequiredModules()
{
    // Trophy subsystem (regular module)
    sceSysmoduleLoadModule(ORBIS_SYSMODULE_NP_TROPHY);

    // Internal modules: audio + pad + net (non-fatal; SDL handles absence)
    sceSysmoduleLoadModuleInternal(ORBIS_SYSMODULE_INTERNAL_AUDIOOUT);
    sceSysmoduleLoadModuleInternal(ORBIS_SYSMODULE_INTERNAL_AUDIOIN);
    sceSysmoduleLoadModuleInternal(ORBIS_SYSMODULE_INTERNAL_NET);
    sceSysmoduleLoadModuleInternal(ORBIS_SYSMODULE_INTERNAL_NETCTL);
    sceSysmoduleLoadModuleInternal(ORBIS_SYSMODULE_INTERNAL_PAD);

    // Voice chat: sceAudioIn (microphone capture) + sceNet (for GNS/raw sockets)
    // ORBIS_SYSMODULE_AUDIO_IN is needed for sceAudioInOpen
    sceSysmoduleLoadModule(ORBIS_SYSMODULE_AUDIO_IN);
    // Network module for BSD socket layer (GameNetworkingSockets fallback)
    sceSysmoduleLoadModule(ORBIS_SYSMODULE_NET);
}

// ── NP Trophy context setup ───────────────────────────────────────────────────
static void InitNpTrophy()
{
    // Get initial user — required for NP context creation on PS4
    if (sceUserServiceGetInitialUser(&g_ps4_user_id) != 0)
        return;

    // Context ties this process to the LBTY00001_00 trophy set
    if (sceNpTrophyCreateContext(&g_np_context, g_ps4_user_id, 0, 0) < 0)
        return;

    if (sceNpTrophyCreateHandle(&g_np_handle) < 0)
    {
        sceNpTrophyDestroyContext(g_np_context);
        g_np_context = 0;
        return;
    }

    // RegisterContext installs the trophy pack on first run.
    // Passing 0 for options uses default (show trophy install dialog if needed).
    sceNpTrophyRegisterContext(g_np_context, g_np_handle, 0);
}

// ── Debugger hook ─────────────────────────────────────────────────────────────
#if defined(LIBERTY_RECOMP_PS4_NETDEBUG)
#ifndef LIBERTY_PS4_DEBUG_IP
#define LIBERTY_PS4_DEBUG_IP "192.168.1.100"
#endif
static void InitNetDebug()
{
    // OpenOrbis redirects sceDbg* to UART TTY automatically.
    // For net printf, the developer would configure a custom hook here.
    (void)LIBERTY_PS4_DEBUG_IP;
}
#endif

// ── PS4 application entry point ───────────────────────────────────────────────
int ps4_main(SceSize argc, const void* argv)
{
    // 1. Heap FIRST — must precede any C++ static constructors or SDL init
    if (user_malloc_init() != 0)
    {
        for (;;) sceKernelSleep(1); // hang for debugger
    }

#if defined(LIBERTY_RECOMP_PS4_NETDEBUG)
    InitNetDebug();
#endif

    // 2. UserService — required before most SCE services
    SceUserServiceInitializeParams userSvcParams = {};
    userSvcParams.priority = SCE_KERNEL_PRIO_FIFO_DEFAULT;
    sceUserServiceInitialize(&userSvcParams);

    // 3. Load dynamic modules (NP Trophy, audio, pad, net)
    LoadRequiredModules();

    // 4. Stand up NP Trophy context (non-fatal; game runs without trophies)
    InitNpTrophy();

    // 5. Hand off to SDL/main
    const char* fake_argv[] = { "LibertyRecomp", nullptr };
    int ret = main(1, const_cast<char**>(fake_argv));

    // 6. Tear down NP and heap
    if (g_np_handle)  sceNpTrophyDestroyHandle(g_np_handle);
    if (g_np_context) sceNpTrophyDestroyContext(g_np_context);
    user_malloc_finalize();

    return ret;
}
