#include <rex/platform.h>
#if REX_PLATFORM_NX
// Nintendo Switch entry point / runtime init for LibertyRecomp (libnx/devkitPro)
// Runs before main() via __appInit(), which libnx calls from its own _start.
//
// Key responsibilities:
//   1. Mount RomFS so romfs:/ paths work before any asset access
//   2. Mount SD card as sdmc:/ (user config, screenshots, logs)
//   3. Mount account save data as save:/ (persisted game saves, settings)
//   4. Set up nxlink stdio for debug builds
//   5. Initialize applet framework
//   6. Provide __appExit cleanup

#include <switch.h>
#include <cstdio>

// Forward declaration from main.cpp
extern int main(int argc, char* argv[]);

// ── Stack size override (default 1 MB → 8 MB for recomp workloads) ──────────
extern "C" u32 __nx_applet_type     = AppletType_Application;
extern "C" u64 __nx_heap_size       = 0xC0000000ULL; // 3 GB — libnx maps from system memory

// ── Title application ID ─────────────────────────────────────────────────────
// This must match the ApplicationId field in the NSP/NCA metadata (NACP).
// Homebrew builds that have not been assigned a real Nintendo ID should keep
// the placeholder below; the save data will still work on hacked consoles.
// Replace with your assigned ID before submission to Nintendo eShop.
#ifndef LIBERTY_RECOMP_NX_APPLICATION_ID
#  define LIBERTY_RECOMP_NX_APPLICATION_ID UINT64_C(0x0100000000000001)
#endif

// Module-level UID filled in during __appInit; used by __appExit to unmount.
static AccountUid g_nx_uid = {};
static bool       g_save_mounted  = false;
static bool       g_sdmc_mounted  = false;

// ── Called by libnx CRT before main() ────────────────────────────────────────
extern "C" void __appInit(void)
{
    Result rc;

    // Initialize SM (service manager) — required by almost everything
    rc = smInitialize();
    if (R_FAILED(rc))
        fatalThrow(rc);

    // Filesystem services (required for romfsInit and fsdev)
    rc = fsInitialize();
    if (R_FAILED(rc))
        fatalThrow(rc);

    // Mount RomFS — game assets are packed into the NRO romfs section.
    // All read-only asset paths are under romfs:/game/…
    rc = romfsInit();
    if (R_FAILED(rc))
        fatalThrow(rc);

    // ── SD card: sdmc:/ ──────────────────────────────────────────────────────
    // Used for: per-session logs, screenshots, user config that doesn't need
    // the journalled save-data container.  Non-fatal; some kiosks have no SD.
    if (R_SUCCEEDED(fsdevMountSdmc()))
        g_sdmc_mounted = true;

    // ── Account service + save data: save:/ ──────────────────────────────────
    // Switch save data is per-user. We resolve the preselected (or first active)
    // user at startup and mount their save container as the POSIX device "save".
    // After this call, save:/ behaves like a normal directory for open/stat/readdir.
    rc = accountInitialize(AccountServiceType_Application);
    if (R_SUCCEEDED(rc)) {
        // accountGetPreselectedUser works when launched from a user-aware title.
        // accountListAllUsers is the fallback for homebrew without a user context.
        if (R_FAILED(accountGetPreselectedUser(&g_nx_uid))) {
            s32 total = 0;
            AccountUid uid_list[8] = {};
            if (R_SUCCEEDED(accountListAllUsers(uid_list, 8, &total)) && total > 0)
                g_nx_uid = uid_list[0];
        }
        // NOTE: Do NOT call accountExit() here — account services must remain
        // available throughout the app lifecycle.  Cleanup is in __appExit().
    }

    // Mount save data only if we resolved a valid user.
    // accountUidIsValid() checks that the UID is non-zero.
    if (accountUidIsValid(&g_nx_uid)) {
        rc = fsdevMountSaveData("save",
                                LIBERTY_RECOMP_NX_APPLICATION_ID,
                                g_nx_uid);
        if (R_SUCCEEDED(rc))
            g_save_mounted = true;
    }

    // Time service (used by logging / profiling)
    timeInitialize();

    // HID (controller input)
    hidInitialize();

    // Audio output
    audoutInitialize();

    // Network interface management (needed for IP queries via nifm)
    nifmInitialize(NifmServiceType_User);

    // Network (optional; non-fatal for offline play)
    socketInitializeDefault();

#ifdef DEBUG
    // Route printf() over the nxlink TCP connection (nxlink tool on PC listens on :2991)
    nxlinkStdio();
#endif
}

// ── Called by libnx CRT after main() returns ─────────────────────────────────
extern "C" void __appExit(void)
{
    socketExit();
    nifmExit();
    audoutExit();
    hidExit();
    timeExit();
    accountExit();

    // Commit and unmount save data — MUST happen before fsExit()
    if (g_save_mounted) {
        fsdevCommitDevice("save");   // flush journal
        fsdevUnmountDevice("save");
    }
    if (g_sdmc_mounted)
        fsdevUnmountDevice("sdmc");

    romfsExit();
    fsExit();
    smExit();
}

// ── Applet main-loop helper (call from app main loop) ────────────────────────
// Returns false when the system requests the app to close (Home button, etc.)
extern "C" bool SwitchAppletIsRunning(void)
{
    if (!appletMainLoop())
        return false;

    // Process applet focus change events (suspend/resume)
    AppletFocusState state = appletGetFocusState();
    if (state == AppletFocusState_InFocus)
        appletSetFocusHandlingMode(AppletFocusHandlingMode_NoSuspend);

    return true;
}

#endif // REX_PLATFORM_NX
