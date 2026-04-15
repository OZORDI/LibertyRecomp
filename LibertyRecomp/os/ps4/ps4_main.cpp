#ifdef LIBERTY_RECOMP_PS4
// PS4 entry point for LibertyRecomp (OpenOrbis)
// user_malloc_init() MUST run before any heap allocation.

#include <orbis/libkernel.h>
#include <orbis/SystemService.h>
#include <orbis/UserService.h>
#include <orbis/Sysmodule.h>
#include <orbis/NpTrophy.h>
#include <orbis/NpManager.h>
#include <orbis/Net.h>
#include <orbis/NetCtl.h>
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
    int32_t rc;

    // Trophy subsystem (regular module)
    rc = sceSysmoduleLoadModule(ORBIS_SYSMODULE_NP_TROPHY);
    if (rc != 0)
        sceKernelDebugOutText(0, "LibertyRecomp: failed to load SYSMODULE_NP_TROPHY\n");

    // Internal modules: audio + pad + net (non-fatal; SDL handles absence)
    rc = sceSysmoduleLoadModuleInternal(ORBIS_SYSMODULE_INTERNAL_AUDIOOUT);
    if (rc != 0)
        sceKernelDebugOutText(0, "LibertyRecomp: failed to load INTERNAL_AUDIOOUT\n");

    rc = sceSysmoduleLoadModuleInternal(ORBIS_SYSMODULE_INTERNAL_AUDIOIN);
    if (rc != 0)
        sceKernelDebugOutText(0, "LibertyRecomp: failed to load INTERNAL_AUDIOIN\n");

    rc = sceSysmoduleLoadModuleInternal(ORBIS_SYSMODULE_INTERNAL_NET);
    if (rc != 0)
        sceKernelDebugOutText(0, "LibertyRecomp: failed to load INTERNAL_NET\n");

    rc = sceSysmoduleLoadModuleInternal(ORBIS_SYSMODULE_INTERNAL_NETCTL);
    if (rc != 0)
        sceKernelDebugOutText(0, "LibertyRecomp: failed to load INTERNAL_NETCTL\n");

    rc = sceSysmoduleLoadModuleInternal(ORBIS_SYSMODULE_INTERNAL_PAD);
    if (rc != 0)
        sceKernelDebugOutText(0, "LibertyRecomp: failed to load INTERNAL_PAD\n");

    // Voice chat: AudioIn & Net are already loaded via internal modules above.
    // No additional sceSysmoduleLoadModule calls needed.
}

// ── NP availability check (TRC requirement) ──────────────────────────────────
// Must be called before any NP service usage.  Non-fatal — game works offline.
static void CheckNpAvailability()
{
    int32_t npRet = sceNpCheckNpAvailabilityA(0, g_ps4_user_id);
    if (npRet < 0)
        sceKernelDebugOutText(0, "LibertyRecomp: sceNpCheckNpAvailabilityA returned error (offline?)\n");
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
    {
        int32_t rc = sceNpTrophyRegisterContext(g_np_context, g_np_handle, 0);
        if (rc < 0)
            sceKernelDebugOutText(0, "LibertyRecomp: sceNpTrophyRegisterContext failed\n");
    }
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
int ps4_main(size_t argc, const void* argv)
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
    OrbisUserServiceInitializeParams userSvcParams = {};
    userSvcParams.priority = 256; // default priority (SCE_KERNEL_PRIO_FIFO_DEFAULT)
    {
        int32_t rc = sceUserServiceInitialize(&userSvcParams);
        if (rc != 0)
            sceKernelDebugOutText(0, "LibertyRecomp: sceUserServiceInitialize failed\n");
    }

    // 3. Load dynamic modules (NP Trophy, audio, pad, net)
    LoadRequiredModules();

    // 3b. Initialize sceNet + sceNetCtl (required before BSD socket layer is usable)
    {
        int32_t rc = sceNetInit();
        if (rc < 0)
            sceKernelDebugOutText(0, "LibertyRecomp: sceNetInit failed\n");

        rc = sceNetCtlInit();
        if (rc < 0)
            sceKernelDebugOutText(0, "LibertyRecomp: sceNetCtlInit failed\n");
    }

    // 4. Check NP availability (TRC-required before any NP calls; non-fatal)
    CheckNpAvailability();

    // 5. Stand up NP Trophy context (non-fatal; game runs without trophies)
    InitNpTrophy();

    // 6. Hand off to SDL/main
    const char* fake_argv[] = { "LibertyRecomp", nullptr };
    int ret = main(1, const_cast<char**>(fake_argv));

    // 7. Tear down NP, net, and heap
    if (g_np_handle)  sceNpTrophyDestroyHandle(g_np_handle);
    if (g_np_context) sceNpTrophyDestroyContext(g_np_context);
    sceNetTerm();
    user_malloc_finalize();

    return ret;
}
#endif // LIBERTY_RECOMP_PS4
