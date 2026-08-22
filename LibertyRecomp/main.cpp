#include <cstdio>
#include <algorithm>
#include <limits>
#include <stdafx.h>
#ifdef __x86_64__
#include <cpuid.h>
#endif
#ifdef __APPLE__
#include <sys/mman.h>
#include <signal.h>
#include <execinfo.h>
#include <unistd.h>
#include <dlfcn.h>
#endif
#ifdef __ANDROID__
#include <dlfcn.h>
#endif
#if REX_PLATFORM_CONSOLE
#include <unistd.h>
#endif
#ifndef _WIN32
#include <signal.h>
#endif
#include <cpu/guest_thread.h>
#include <gpu/video.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <kernel/heap.h>
#include <file.h>
#include <vector>
#include <image.h>
#include <hid/hid.h>
#include <user/config.h>
#include <user/paths.h>
#include <user/registry.h>
#include <kernel/xdbf.h>
#if !REX_PLATFORM_CONSOLE
#include <install/auto_installer.h>
#include <install/installer.h>
#endif
#ifndef LIBERTY_RECOMP_NO_CURL
#include <install/update_checker.h>
#endif
#include <os/logger.h>
#include <os/diag.h>
#include <os/process.h>
#include <os/registry.h>
#include <CLI/CLI.hpp>
#if defined(LIBERTY_RECOMP_DISCORD_RPC)
#include <os/discord/discord_presence.h>
#endif
#include <install/embedded_assets.h>
#include <ui/game_window.h>
#if !REX_PLATFORM_CONSOLE
#include <ui/installer_wizard.h>
#endif
#include <ui/main_menu.h>
#include <mod/mod_loader.h>
#include <network/community_multiplayer.h>
#include <iostream>
#include <app.h>
#include <debugger.h>

#include <rex/exception_handler.h>
#include <rex/diagnostics/policy.h>
#include <rex/platform/exceptions.h>  // rex::SehException
#include <rex/logging.h>
#include <atomic>
#include <chrono>
#include <thread>
#include <rex/runtime.h>
// #include <rex/runtime/guest/exceptions.h>  // Not present in graine SDK; unused
// #include <rex/runtime/processor.h>         // Not present in graine SDK; unused
#include <rex/system/kernel_state.h>
#include <rex/system/xthread.h>
#include <rex/system/xobject.h>
#include <rex/system/user_module.h>
#include <rex/kernel/xam/apps/xgi_app.h>
#include <rex/filesystem/vfs.h>
#include <rex/filesystem/devices/host_path_device.h>
#include <rex/kernel/crt/heap.h>
#include <rex/chrono/clock.h>
#include <rex/input/input_system.h>
#if defined(LIBERTY_RECOMP_PS4)
#include "../glue/rexglue-sdk-main/src/audio/orbis/orbis_audio_system.h"
#elif REX_PLATFORM_NX
#include <rex/audio/switch/switch_audio_system.h>
#elif REX_PLATFORM_MAC && !REX_PLATFORM_IOS
#include <rex/audio/coreaudio/coreaudio_audio_system.h>
#else
#include <rex/audio/sdl/sdl_audio_system.h>
#endif
#if defined(LIBERTY_RECOMP_PS4)
#include <os/ps4/achievement_bridge_ps4.h>
#endif
#ifdef LIBERTY_RECOMP_GAMECENTER
#include <os/gamecenter/achievement_bridge_gc.h>
#endif
#if REX_PLATFORM_ANDROID
#include <os/android/achievement_bridge_android.h>
#endif

#ifdef _WIN32
#include <timeapi.h>
#endif

#if defined(_WIN32) && defined(LIBERTY_RECOMP_D3D12)
static std::array<std::string_view, 3> g_D3D12RequiredModules =
{
    "D3D12/D3D12Core.dll",
    "dxcompiler.dll",
    "dxil.dll"
};
#endif

// On Android, SDL3 runs the native entry point through SDLActivity.nativeRunMain,
// which invokes SDL_main() rather than main(). Including <SDL3/SDL_main.h> in the
// translation unit that defines main() aliases `main` → `SDL_main` at preprocess
// time, so the existing main() body below gets picked up correctly. On desktop
// this header is also a no-op unless SDL_MAIN_HANDLED is NOT defined; we leave
// it Android-only to avoid perturbing the other platforms.
#if defined(__ANDROID__)
#include <SDL3/SDL_main.h>
#endif

const size_t XMAIOBegin = 0x7FEA0000;
const size_t XMAIOEnd = XMAIOBegin + 0x0000FFFF;

Memory g_memory;
XDBFWrapper g_xdbfWrapper;
std::unordered_map<uint16_t, GuestTexture*> g_xdbfTextureCache;

// ============================================================================
// RexGlue Exception Handler
// ============================================================================

// Fallback exception handler: logs crash diagnostics when no other handler
// (e.g., MMIOHandler) handles the fault. Installed last in the chain so it
// only fires for truly unrecoverable faults.
static bool RexFallbackCrashHandler(rex::arch::Exception* ex, void* /*data*/) {
    if (!ex) return false;

    // Only handle access violations and illegal instructions that
    // weren't handled by prior handlers (MMIOHandler, etc.)
    // We log diagnostics but return false to let the default crash path run.
    const char* type_str = "UNKNOWN";
    switch (ex->code()) {
        case rex::arch::Exception::Code::kAccessViolation:
            type_str = "ACCESS_VIOLATION";
            break;
        case rex::arch::Exception::Code::kIllegalInstruction:
            type_str = "ILLEGAL_INSTRUCTION";
            break;
        default:
            break;
    }

    fprintf(stderr, "\n");
    fprintf(stderr, "============================================================\n");
    fprintf(stderr, "CRASH DETECTED (RexGlue): %s\n", type_str);
    fprintf(stderr, "============================================================\n");
    fprintf(stderr, "Fault address: 0x%016llX\n",
            (unsigned long long)ex->fault_address());
    fprintf(stderr, "PC:            0x%016llX\n",
            (unsigned long long)ex->pc());
#if !defined(_WIN32) && !REX_PLATFORM_CONSOLE
    {
        Dl_info info;
        if (dladdr((void*)ex->pc(), &info)) {
            fprintf(stderr, "Symbol:        %s + 0x%lX\n",
                    info.dli_sname ? info.dli_sname : "(unknown)",
                    (unsigned long)((uintptr_t)ex->pc() - (uintptr_t)info.dli_saddr));
            fprintf(stderr, "Image:         %s\n", info.dli_fname ? info.dli_fname : "(unknown)");
        }
    }
#endif
    fprintf(stderr, "\nHeap state at crash:\n");
    fprintf(stderr, "  Memory base:   %p\n", (void*)g_memory.base);

#if defined(__aarch64__) || defined(__arm64__)
    // Dump ARM64 register state from HostThreadContext
    auto* tc = ex->thread_context();
    if (tc) {
        fprintf(stderr, "\nARM64 Registers:\n");
        for (int i = 0; i < 31; i++) {
            fprintf(stderr, "  x%-2d = 0x%016llX", i, (unsigned long long)tc->x[i]);
            if (i % 3 == 2 || i == 30) fprintf(stderr, "\n");
        }
        fprintf(stderr, "  sp  = 0x%016llX  pc  = 0x%016llX\n",
                (unsigned long long)tc->sp, (unsigned long long)tc->pc);

        // x0 and x1 in generated code are (PPCContext& ctx, uint8_t* base)
        // Try to dump PPCContext guest registers
        auto* ctx_ptr = reinterpret_cast<PPCContext*>(tc->x[0]);
        uint8_t* base = reinterpret_cast<uint8_t*>(tc->x[1]);
        if (ctx_ptr && base == g_memory.base) {
            fprintf(stderr, "\nPPCContext (guest registers):\n");
            fprintf(stderr, "  r0 =0x%08X  r1 =0x%08X  r2 =0x%08X  r3 =0x%08X\n",
                    ctx_ptr->r0.u32, ctx_ptr->r1.u32, ctx_ptr->r2.u32, ctx_ptr->r3.u32);
            fprintf(stderr, "  r4 =0x%08X  r5 =0x%08X  r6 =0x%08X  r7 =0x%08X\n",
                    ctx_ptr->r4.u32, ctx_ptr->r5.u32, ctx_ptr->r6.u32, ctx_ptr->r7.u32);
            fprintf(stderr, "  r8 =0x%08X  r9 =0x%08X  r10=0x%08X  r11=0x%08X\n",
                    ctx_ptr->r8.u32, ctx_ptr->r9.u32, ctx_ptr->r10.u32, ctx_ptr->r11.u32);
            fprintf(stderr, "  r12=0x%08X  r13=0x%08X  r14=0x%08X  lr =0x%08llX\n",
                    ctx_ptr->r12.u32, ctx_ptr->r13.u32, ctx_ptr->r14.u32, (unsigned long long)ctx_ptr->lr);

            // Check if fault address is base + something
            uintptr_t fa = ex->fault_address();
            uintptr_t b = (uintptr_t)base;
            if (fa >= b && fa < b + 0x100000000ULL) {
                fprintf(stderr, "\n  Fault is at guest addr: 0x%08X\n", (uint32_t)(fa - b));
            } else {
                fprintf(stderr, "\n  Fault address 0x%llX is OUTSIDE guest memory [0x%llX - 0x%llX]\n",
                        (unsigned long long)fa, (unsigned long long)b,
                        (unsigned long long)(b + 0x100000000ULL));
            }
        }
    }
#endif

    fprintf(stderr, "============================================================\n");
    fflush(stderr);

    // Return false: we didn't fix anything. The RexGlue exception system
    // will restore the original signal handler and re-raise for a core dump.
    return false;
}

// Phase 2a: Install the fallback crash handler early (before memory init).
// This sets up RexGlue's signal handlers (SIGILL, SIGSEGV, SIGBUS) via
// ExceptionHandler::Install() so any faults during startup are caught.
// The fallback handler only logs diagnostics and returns false.
static void InstallRexGlueExceptionHandlers() {
    // Install the fallback crash handler. ExceptionHandler::Install() also
    // registers SIGILL/SIGSEGV/SIGBUS signal handlers on first call, so
    // any faults during early startup are caught and diagnosed.
    // Memory::Initialize() installs its own MMIOHandler internally, so
    // no separate MMIO handler installation is needed.
    //
    // IMPORTANT: Do NOT install a separate raw SIGBUS handler here.
    // RexGlue's ExceptionHandlerCallback already handles SIGBUS (macOS
    // delivers SIGBUS for PROT_NONE guard page writes). A raw handler
    // would override ExceptionHandlerCallback, preventing the MMIOHandler
    // → AccessViolationCallback → stack guard expansion chain from working.
    rex::arch::ExceptionHandler::Install(RexFallbackCrashHandler, nullptr);
    DIAG_EMIT("[RexGlue] Fallback exception handler installed (signal handlers active)\n");
}

// ============================================================================

static void ShowVideoBackendErrorAndExit()
{
#if !REX_PLATFORM_CONSOLE
    if (GameWindow::s_pWindow)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, GameWindow::GetTitle(), Localise("Video_BackendError").c_str(), GameWindow::s_pWindow);
    }
    else
#endif
    {
        fprintf(stderr, "[Main] Video backend initialization failed (no window available for message box).\n");
        fflush(stderr);
    }
    DIAG_EMIT("[EXIT-TRACE] main.cpp:203 calling _Exit\n");
    std::_Exit(1);
}

void HostStartup()
{
#ifdef _WIN32
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#endif

    hid::Init();
}

// ----------------------------------------------------------------------------
// Liberty watchdog: periodically dumps every XThread's lr/r1 so we can tell
// where each guest thread is stuck when the game goes silent. Race-tolerant by
// design — snapshots are approximate, but a stable repeated lr across ticks
// unambiguously identifies a hung thread.
// ----------------------------------------------------------------------------
static std::atomic<bool> g_libertyWatchdogRunning{false};
static std::thread g_libertyWatchdogThread;

static void LibertyWatchdogLoop()
{
    using namespace std::chrono;
    uint64_t tick = 0;
    auto started = steady_clock::now();
    while (g_libertyWatchdogRunning.load(std::memory_order_relaxed)) {
        std::this_thread::sleep_for(milliseconds(500));
        auto* ks = rex::runtime::current_kernel_state();
        if (!ks) continue;
        auto threads = ks->object_table()->GetObjectsByType<rex::system::XThread>();
        double elapsed_s = duration<double>(steady_clock::now() - started).count();
        fprintf(stderr, "[WATCHDOG] tick=%llu t=%.1fs threads=%zu\n",
                (unsigned long long)tick, elapsed_s, threads.size());
        uint8_t* mem_base = g_memory.base;
        auto read_guest_u32 = [mem_base](uint32_t ea) -> uint32_t {
            if (!mem_base) return 0;
            if (ea < 0x10000u || ea >= PPC_MEMORY_SIZE - 4) return 0;
            uint32_t raw;
            std::memcpy(&raw, mem_base + ea, 4);
            return __builtin_bswap32(raw);
        };
        for (auto& t : threads) {
            if (!t) continue;
            auto* ts = t->thread_state();
            uint32_t lr_lo = 0, lr_hi = 0, r1 = 0, r3 = 0;
            bool have_ctx = false;
            if (ts && ts->context()) {
                auto* ctx = ts->context();
                uint64_t lr = ctx->lr;
                lr_lo = static_cast<uint32_t>(lr);
                lr_hi = static_cast<uint32_t>(lr >> 32);
                r1 = ctx->r1.u32;
                r3 = ctx->r3.u32;
                have_ctx = true;
            }
            fprintf(stderr,
                    "  tid=%u name='%s' run=%d guest=%d main=%d lr=0x%08X%08X r1=0x%08X r3=0x%08X ctx=%d\n",
                    t->thread_id(), t->name().c_str(),
                    (int)t->is_running(), (int)t->is_guest_thread(),
                    (int)t->main_thread(), lr_hi, lr_lo, r1, r3, (int)have_ctx);
            // Stack-walk disabled: it served its purpose (identifying
            // sub_82847D48/buddy-allocator as the suspected hang site) but
            // faults when it reaches unmapped pages near the top of a guest
            // thread's stack. Re-enable with mincore-based page probing if
            // deeper stack visibility is needed again.
        }
        fflush(stderr);
        ++tick;
    }
}

static void StartLibertyWatchdog()
{
    if (!rex::diagnostics::IsEnabled(
            rex::diagnostics::Category::kWatchdog)) return;
    if (g_libertyWatchdogRunning.exchange(true)) return;
    g_libertyWatchdogThread = std::thread(LibertyWatchdogLoop);
}

static void StopLibertyWatchdog()
{
    if (!g_libertyWatchdogRunning.exchange(false)) return;
    if (g_libertyWatchdogThread.joinable()) g_libertyWatchdogThread.join();
}

// Forward declarations from imports.cpp
void InitKernelMainThread();

// Xenon memory initialization handled by rexglue LoadXexImage.

// Name inspired from nt's entry point
void KiSystemStartup()
{
    // Initialize main thread ID for SDL event safety
    InitKernelMainThread();

    if (g_memory.base == nullptr)
    {
#if !REX_PLATFORM_CONSOLE
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, GameWindow::GetTitle(), Localise("System_MemoryAllocationFailed").c_str(), GameWindow::s_pWindow);
#endif
        fprintf(stderr, "[Main] Memory allocation failed\n"); fflush(stderr);
        DIAG_EMIT("[EXIT-TRACE] main.cpp:230 calling _Exit\n");
        std::_Exit(1);
    }

    // Initialize debugger before anything else
    Debugger::SetMemoryBase(g_memory.base, PPC_MEMORY_SIZE);
    Debugger::Initialize();
    Debugger::StartCLIThread();

    // NOTE: InitializeXenonMemoryRegions removed — RexGlue's LoadXexImage
    // handles PE section initialization (writes .data, zeroes BSS).
    // The old call copied xex_header_data + zeroed BSS, but LoadXexImage
    // overwrites ALL of that.  Removing eliminates redundant work.

    g_userHeap.Init();

    // rexcrt heap is now initialized in main() after Runtime::Setup(),
    // where runtime->memory() is available (matching rex_app.cpp pattern).

    // SaveSystem removed — rexglue handles save/content via XAM.

    // ---------------------------------------------------------------
    // REMOVED: Liberty's VFS, content registration, path cache, root
    // mappings.  RexGlue's VFS handles ALL file I/O now.
    // RexGlue mounts game: → \Device\Harddisk0\Partition1 and the
    // GTA IV symlinks (common:, platform:, audio:, etc.) are
    // registered in main() after rex::Runtime::Setup().
    // ---------------------------------------------------------------

    // XAudioInitializeSystem(); // Removed: RexGlue SDL audio handles this
}

// Static buffer keeps the XEX bytes alive for the lifetime of XDBFWrapper
// so we don't hand out pointers into a stack-local std::vector.
static std::vector<uint8_t> g_xexFileBuffer;

uint32_t LdrLoadModule(const std::filesystem::path &path)
{
    auto loadResult = LoadFile(path);
    if (loadResult.empty())
    {
        assert("Failed to load module" && false);
        return 0;
    }

    const auto image = Image::ParseImage(loadResult.data(), loadResult.size());

    // Feed XDBFWrapper from the RAW FILE BYTES, not guest memory. Previously
    // Liberty memcpy'd the whole decompressed PE into guest memory just so
    // XDBFWrapper could point at image.resource_offset inside g_memory.base —
    // but rexglue's LoadXexImage then overwrote that same region immediately
    // afterwards, making the memcpy a pure waste (plus it was running before
    // rexglue finished setting up its own PE layout).
    //
    // image.data is a unique_ptr<uint8_t[]> into a stack-local parsed view;
    // the ParseImage helper in xenon_image.cpp copies the decompressed PE
    // into image.data with image.resource_offset as the byte offset of XDBF
    // within that buffer. Hand XDBFWrapper a pointer into g_xexFileBuffer
    // instead.
    g_xexFileBuffer.assign(image.data.get(), image.data.get() + image.size);
    const uint32_t xdbfOffset = image.resource_offset - image.base;
    if (xdbfOffset + image.resource_size <= g_xexFileBuffer.size())
    {
        g_xdbfWrapper = XDBFWrapper(g_xexFileBuffer.data() + xdbfOffset,
                                    image.resource_size);
    }

    return image.entry_point;
}

#ifdef __x86_64__
__attribute__((constructor(101), target("no-avx,no-avx2"), noinline))
void init()
{
    uint32_t eax, ebx, ecx, edx;

    // Execute CPUID for processor info and feature bits.
    __get_cpuid(1, &eax, &ebx, &ecx, &edx);

    // Check for AVX support.
    if ((ecx & (1 << 28)) == 0)
    {
        printf("[*] CPU does not support the AVX instruction set.\n");

#ifdef _WIN32
        MessageBoxA(nullptr, "Your CPU does not meet the minimum system requirements.", "Liberty Recompiled", MB_ICONERROR);
#endif

        DIAG_EMIT("[EXIT-TRACE] main.cpp:318 calling _Exit\n");
        std::_Exit(1);
    }
}
#endif

static void LibertyOnXboxAchievementUnlocked(uint32_t xbox_id)
{
#if defined(LIBERTY_RECOMP_PS4)
    LibertyPS4OnXboxAchievementUnlocked(xbox_id);
#endif
#ifdef LIBERTY_RECOMP_GAMECENTER
    LibertyGCOnXboxAchievementUnlocked(xbox_id);
#endif
#if REX_PLATFORM_ANDROID
    LibertyAndroidOnXboxAchievementUnlocked(xbox_id);
#endif
}

int main(int argc, char *argv[])
{
    bool forceInstaller = false;
    bool forceDLCInstaller = false;
    bool useDefaultWorkingDirectory = false;
    bool forceInstallationCheck = false;
    bool graphicsApiRetry = false;
    bool diagnostics = false;
    const char *sdlVideoDriver = nullptr;
    std::string sdlVideoDriverStr;
    std::string diagnosticsCategories;

    // Parse the command-line diagnostics policy before installing log sinks,
    // starting diagnostic threads, or creating the graphics plugin. The
    // process policy is immutable after this point.
    {
        CLI::App cli{"Liberty Recompiled \u2014 GTA IV PPC recompilation"};
        cli.allow_extras();
        bool skipLogos = false;
        cli.add_flag("--install",              forceInstaller,             "Force the installer wizard to run");
        cli.add_flag("--install-dlc",          forceDLCInstaller,          "Install DLC content from staged sources");
        cli.add_flag("--use-cwd",              useDefaultWorkingDirectory, "Use current working directory instead of executable path");
        cli.add_flag("--install-check",        forceInstallationCheck,     "Force re-validation of the installed game files");
        cli.add_flag("--graphics-api-retry",   graphicsApiRetry,           "Retry with an alternative graphics API on failure");
        cli.add_flag("--skip-logos",           skipLogos,                  "Skip publisher/developer logos at startup");
        cli.add_option("--sdl-video-driver",   sdlVideoDriverStr,          "Override the SDL video driver");
        cli.add_flag("--diagnostics", diagnostics,
                     "Enable diagnostic logging and instrumentation");
        cli.add_option("--diagnostics-categories", diagnosticsCategories,
                       "Comma-separated diagnostic categories (default: all; logging controls "
                       "general application logs independently of diagnostic artifacts)");

        try { cli.parse(argc, argv); }
        catch (const CLI::ParseError &e) { return cli.exit(e); }

        std::string diagnosticsError;
        if (!rex::diagnostics::Configure(diagnostics, diagnosticsCategories,
                                         &diagnosticsError)) {
            std::fprintf(stderr, "Liberty Recompiled: %s\n", diagnosticsError.c_str());
            return CLI::ExitCodes::ValidationError;
        }

        App::s_isSkipLogos = App::s_isSkipLogos || skipLogos;
        if (!sdlVideoDriverStr.empty())
            sdlVideoDriver = sdlVideoDriverStr.c_str();
    }

    // Install RexGlue's signal handlers + fallback crash handler early.
    // MMIOHandler is installed later after g_memory.base is valid.
    InstallRexGlueExceptionHandlers();

#ifdef _WIN32
    timeBeginPeriod(1);
#endif

    os::process::CheckConsole();

    if (!os::registry::Init())
        LOGN("OS does not support registry.");

    if (rex::diagnostics::IsEnabled(rex::diagnostics::Category::kLogging))
        os::logger::Init();

    // preload_executable.cpp removed — was Windows-only, NOP elsewhere.

    if (!useDefaultWorkingDirectory)
    {
        // Set the current working directory to the executable's path.
        std::error_code ec;
        std::filesystem::current_path(os::process::GetExecutablePath().parent_path(), ec);
    }

    Config::Load();

    // RexGlue logging init happens inside os::logger::Init() (see C4 refactor):
    // os::logger::Init() now calls rex::InitLogging() + registers platform sinks.
    // Calling it again here would be redundant.

    // SDK v0.2.1: Memory is now created and owned internally by rex::Runtime.
    // g_memory.base is set from runtime->virtual_membase() after Setup() succeeds.
    // No separate InitializeFromRexGlue() call needed.

    // Create rex::Runtime with pre-existing Memory.
    // Runtime creates: ExportResolver, Processor, KernelState.
    // KernelState constructor sets shared_kernel_state_ singleton, enabling
    // kernel_state() to work in RexGlue's xboxkrnl export implementations.
    // Under REX_HEADLESS: GPU, audio, input drivers are skipped.
    // content_root = game directory so RexGlue's VFS handles all file I/O.
    // RexGlue mounts this as \Device\Harddisk0\Partition1 with game: and d: symlinks.
    // GTA IV-specific symlinks (common:, platform:, audio:) are added after Setup().
    DIAG_EMIT("[Main] Getting game path...\n");
    const auto gamePath = GetGamePath();
    DIAG_EMIT("[Main] Game path: %s\n", gamePath.string().c_str());
    const auto rexContentRoot = gamePath / "game";
    DIAG_EMIT("[Main] Content root: %s\n", rexContentRoot.string().c_str());
    DIAG_EMIT("[Main] Content root exists: %d\n", (int)std::filesystem::exists(rexContentRoot));

    // Extract title update files BEFORE VFS init — HostPathDevice snapshots the
    // directory at mount time, so default.xexp must exist before rex::Runtime::Setup().
#if !REX_PLATFORM_CONSOLE
    {
        AutoInstallResult aiResult = AutoInstaller::Run(gamePath);
        if (!aiResult.errorMessage.empty())
            DIAG_EMIT("[AutoInstall] Warning: %s\n", aiResult.errorMessage.c_str());
    }
#endif

    // Bridge extracted audio.rpf content to where the RAGE relative device expects it.
    // The packfile device at audio:/ has an AES-encrypted TOC that fails to decrypt in
    // the recompiled env. The fallback relative device uses root game:/xbox360/audio/,
    // but extracted config files live at audio/config/. Copy the directory tree.
    //
    // On console builds (PS4 /app0 is read-only), the PKG must pre-lay this out:
    // make_gp4.py packages the staging tree as-is, so the payload has to already
    // contain xbox360/audio/config/* at build time.
#if !REX_PLATFORM_CONSOLE
    {
        auto destPath   = rexContentRoot / "xbox360" / "audio" / "config";
        auto srcPath    = gamePath / "game" / "audio" / "config";
        // Also try gamePath directly (audio/ may be beside game/)
        if (!std::filesystem::is_directory(srcPath)) {
            srcPath = gamePath / "audio" / "config";
        }
        std::error_code ec;
        if (std::filesystem::is_directory(srcPath, ec) && !std::filesystem::exists(destPath, ec)) {
            std::filesystem::create_directories(destPath.parent_path(), ec);
            std::filesystem::copy(srcPath, destPath,
                std::filesystem::copy_options::recursive |
                std::filesystem::copy_options::skip_existing, ec);
            if (!ec) {
                DIAG_EMIT("[Main] Copied audio config: %s -> %s\n",
                          srcPath.string().c_str(), destPath.string().c_str());
            } else {
                DIAG_EMIT("[Main] WARN: Could not copy audio config: %s\n", ec.message().c_str());
            }
        }
    }
#endif  // !REX_PLATFORM_CONSOLE (audio config bridge)

    // Bridge DLC directories into game root for direct file probing.
    // GTA IV's DLC discovery probes hardcoded paths: game:\DLC1\setup2.xml
    // and game:\DLC1\DLC.rpf (TLAD = DLC1, TBOGT = DLC2). These bypass the
    // XAM content system and open files directly via the game: VFS mount.
    // The game: mount points to rexContentRoot, so we copy DLC content there.
#if !REX_PLATFORM_CONSOLE
    {
        struct DlcGameMapping {
            const char* gameDirName;  // What the game probes (game:\DLC1\)
            const char* dlcDirName;   // Where the content lives (dlc/TLAD/)
        };
        const DlcGameMapping dlcMappings[] = {
            { "DLC1", "TLAD" },
            { "DLC2", "TBOGT" },
        };
        std::error_code ec;
        for (const auto& mapping : dlcMappings) {
            auto actualDlc = gamePath / "dlc" / mapping.dlcDirName;
            auto linkPath  = rexContentRoot / mapping.gameDirName;
            bool srcExists = std::filesystem::is_directory(actualDlc, ec);
            bool dstExists = std::filesystem::exists(linkPath, ec);
            DIAG_EMIT("[Main] DLC %s: src=%s exists=%d, dst=%s exists=%d\n",
                      mapping.gameDirName, actualDlc.string().c_str(), (int)srcExists,
                      linkPath.string().c_str(), (int)dstExists);
            if (srcExists && !dstExists) {
                std::filesystem::copy(actualDlc, linkPath,
                    std::filesystem::copy_options::recursive |
                    std::filesystem::copy_options::skip_existing, ec);
                if (!ec) {
                    DIAG_EMIT("[Main] DLC game bridge copy: %s -> %s\n",
                              actualDlc.string().c_str(), linkPath.string().c_str());
                } else {
                    DIAG_EMIT("[Main] WARN: DLC game bridge copy failed for %s: %s\n",
                              mapping.gameDirName, ec.message().c_str());
                }
            }
        }
    }
#endif  // !REX_PLATFORM_CONSOLE (DLC1/DLC2 direct-probe bridge)

    {
        static std::unique_ptr<rex::Runtime> s_rexRuntime;
        const auto rexSaveRoot = GetSavePath(false);
        const auto rexMarketplaceRoot = gamePath / "dlc";
        std::error_code pathError;
        std::filesystem::create_directories(rexSaveRoot, pathError);
        if (pathError) {
            fprintf(stderr, "[Main] FATAL: Could not create save directory %s: %s\n",
                    rexSaveRoot.string().c_str(), pathError.message().c_str());
            return 1;
        }
        DIAG_EMIT("[Main] Save root: %s\n", rexSaveRoot.string().c_str());
        DIAG_EMIT("[Main] Marketplace content root: %s\n",
                  rexMarketplaceRoot.string().c_str());
        DIAG_EMIT("[Main] Constructing rex::Runtime...\n");
        s_rexRuntime = std::make_unique<rex::Runtime>(
            rexContentRoot,                    // game_data_root
            rexSaveRoot,                       // user_data_root
            rexContentRoot / "update",        // update_data_root
            std::filesystem::path{},           // cache_root
            std::filesystem::path{},           // metadata_root
            rexMarketplaceRoot,                // marketplace_content_root
            rexSaveRoot);                      // saved_game_root
        DIAG_EMIT("[Main] Runtime constructed, calling Setup() with func mappings...\n");

        // Let guest::initialize() (called inside Setup) install its SEH
        // signal handler normally.  After Setup(), we re-install
        // ExceptionHandler on top so the chain becomes:
        //   ExceptionHandler (NULL-PC, MMIO) → SEH (data faults in __try)
        // This matches standalone: ExceptionHandler saves SEH as its
        // "previous handler" and falls back to it for unhandled faults.
        // Give RexGlue full audio control via native CoreAudio on macOS and
        // its SDL AudioSystem on the other desktop platforms.
        // Liberty's audio hooks in imports.cpp have been removed so RexGlue's
        // XBOXKRNL exports (XMACreateContext, XAudioRegisterRenderDriverClient,
        // etc.) handle all game audio calls with full Xenia-based implementations.
        rex::RuntimeConfig rexConfig;
#if defined(LIBERTY_RECOMP_PS4)
        rexConfig.audio_factory = REX_AUDIO_BACKEND(rex::audio::orbis::OrbisAudioSystem);
#elif REX_PLATFORM_NX
        rexConfig.audio_factory = REX_AUDIO_BACKEND(rex::audio::nx::SwitchAudioSystem);
#elif REX_PLATFORM_MAC && !REX_PLATFORM_IOS
        rexConfig.audio_factory = REX_AUDIO_BACKEND(rex::audio::coreaudio::CoreAudioAudioSystem);
#else
        rexConfig.audio_factory = REX_AUDIO_BACKEND(rex::audio::sdl::SDLAudioSystem);
#endif
        rexConfig.input_factory = REX_INPUT_BACKEND(rex::input::CreateDefaultInputSystem);
        switch (Config::MultiplayerBackend.Value)
        {
            case EMultiplayerBackend::LAN:
                rexConfig.live.backend = rex::system::xam::LiveBackend::kLan;
                break;
            case EMultiplayerBackend::Community:
                rexConfig.live.backend = rex::system::xam::LiveBackend::kCommunity;
                break;
            case EMultiplayerBackend::Firebase:
                // Firebase is deliberately unavailable until the LAN and community
                // protocols share the same verified session ABI.
                rexConfig.live.backend = rex::system::xam::LiveBackend::kOffline;
                break;
            case EMultiplayerBackend::Offline:
                rexConfig.live.backend = rex::system::xam::LiveBackend::kOffline;
                break;
        }
        rexConfig.live.community_url = Config::CommunityServerURL.Value;
        rexConfig.live.player_name = Config::PlayerName.Value;
        switch (Config::MultiplayerRelayPolicy.Value)
        {
            case EMultiplayerRelayPolicy::Auto:
                rexConfig.live.relay_policy = rex::system::xam::RelayPolicy::kAuto;
                break;
            case EMultiplayerRelayPolicy::DirectOnly:
                rexConfig.live.relay_policy = rex::system::xam::RelayPolicy::kDirectOnly;
                break;
            case EMultiplayerRelayPolicy::RelayOnly:
                rexConfig.live.relay_policy = rex::system::xam::RelayPolicy::kRelayOnly;
                break;
        }
        rexConfig.live.community_backend_factory =
            &LibertyRecomp::Network::CreateCommunityMultiplayerBackend;
        rexConfig.live.lan_discovery_port = static_cast<uint16_t>(std::clamp<int32_t>(
            Config::LANBroadcastPort.Value, 1, std::numeric_limits<uint16_t>::max()));

        uint32_t rt_status = s_rexRuntime->Setup(
            static_cast<uint32_t>(PPC_CODE_BASE),
            static_cast<uint32_t>(PPC_CODE_SIZE),
            static_cast<uint32_t>(PPC_IMAGE_BASE),
            static_cast<uint32_t>(PPC_IMAGE_SIZE),
            PPCFuncMappings,
            std::move(rexConfig));
        DIAG_EMIT("[Main] Setup() returned 0x%08X\n", rt_status);

        // Xbox 360 timebase = exactly 50 MHz. Must be explicit because Liberty
        // drives rex::Runtime directly (not via rex::ReXApp), so ReXApp's
        // defaults don't fire and the Clock would otherwise default to host CPU
        // freq, breaking time-gated loading tasks.
        rex::chrono::Clock::set_guest_tick_frequency(50000000ULL);

        if (rt_status != 0 /* X_STATUS_SUCCESS */) {
            fprintf(stderr, "[Main] FATAL: rex::Runtime::Setup() failed with 0x%08X\n", rt_status);
            DIAG_EMIT("[EXIT-TRACE] main.cpp:478 calling _Exit\n");
            std::_Exit(1);
        }

        rex::kernel::xam::apps::SetAchievementUnlockCallback(&LibertyOnXboxAchievementUnlocked);
#ifdef LIBERTY_RECOMP_GAMECENTER
        LibertyGCInit();
#endif

        // rexcrt heap init. Same reason as above — rex_app.cpp normally handles
        // this for ReXApp-derived apps, but Liberty owns its own main() so we
        // have to call it ourselves. Without this rexcrt_RtlAllocateHeap fails
        // with "kernel memory is null" on every guest alloc → SIGBUS.
        if (!rex::kernel::crt::InitHeap(FLAGS_rexcrt_heap_size_mb, s_rexRuntime->memory())) {
            fprintf(stderr, "[Main] FATAL: rexcrt heap init failed\n");
            std::_Exit(1);
        }
        // DIAG: verify xstart is registered after Setup(). This is subordinate
        // to the immutable command-line logging category.
        if (::os::diag::ShouldEmit()) {
            auto* p = s_rexRuntime->function_dispatcher();
            PPCFunc* xsf = p->GetFunction(0x82A11290);
            fprintf(stderr, "[DIAG] After Setup(): GetFunction(0x82A11290)=%p  HasFT=%d\n",
                    (void*)xsf, (int)p->HasFunctionTable());
            int total = 0, nullHost = 0;
            bool foundEntry = false;
            for (int i = 0; PPCFuncMappings[i].guest != 0; ++i) {
                total++;
                if (PPCFuncMappings[i].host == nullptr) nullHost++;
                if (PPCFuncMappings[i].guest == 0x82A11290) {
                    fprintf(stderr, "[DIAG] PPCFuncMappings[%d] = { 0x%zX, %p } (xstart)\n",
                            i, PPCFuncMappings[i].guest, (void*)PPCFuncMappings[i].host);
                    foundEntry = true;
                }
            }
            fprintf(stderr, "[DIAG] PPCFuncMappings: %d total, %d null-host, found_82A11290=%d\n",
                    total, nullHost, (int)foundEntry);
        }
        // SDK v0.2.1: set_instance() is automatic after Setup().
        // Get memory base from the Runtime's internally-managed Memory object.
        g_memory.base = s_rexRuntime->virtual_membase();
        DIAG_EMIT("[Main] RexGlue memory: base=%p\n", (void*)g_memory.base);
        // Set base pointer for rexcrt memset watchpoint diagnostic
        // Populate vtables and install function stubs into guest memory.
        g_memory.PopulateFunctionTableAndVtables();
        DIAG_EMIT("[Main] rex::Runtime created (kernel_state=%p)\n",
                  (void*)s_rexRuntime->kernel_state());

        // Register GTA IV-specific symbolic links in RexGlue's VFS.
        // RexGlue already registered game: and d: -> \Device\Harddisk0\Partition1
        // GTA IV uses common:, platform:, audio: prefixes for file access.
        // These map to subdirectories of the content_root (game/).
        auto* fs = s_rexRuntime->file_system();
        if (fs) {
            // common:\path -> \Device\Harddisk0\Partition1\common\path
            fs->RegisterSymbolicLink("common:", "\\Device\\Harddisk0\\Partition1\\common");
            // platform:\path -> \Device\Harddisk0\Partition1\xbox360\path
            fs->RegisterSymbolicLink("platform:", "\\Device\\Harddisk0\\Partition1\\xbox360");
            // audio:\path -> \Device\Harddisk0\Partition1\audio\path
            fs->RegisterSymbolicLink("audio:", "\\Device\\Harddisk0\\Partition1\\audio");
            // Some code uses xbox360: instead of platform:
            fs->RegisterSymbolicLink("xbox360:", "\\Device\\Harddisk0\\Partition1\\xbox360");
            // X: drive prefix used by some game code
            fs->RegisterSymbolicLink("x:", "\\Device\\Harddisk0\\Partition1");
            // update: symlink removed — rex::Runtime::SetupVfs()
            // (runtime.cpp:272) already registers it. Liberty's override was
            // racing with rex's via std::multimap::insert (order-dependent
            // first-match in FindSymbolicLink).
            //
            // cache:/cache1: devices removed — rexglue's runtime.cpp:292-294
            // explicitly documents: "Do NOT register a device for cache:
            // paths... Games handle 'device not found' gracefully but don't
            // handle NAME_COLLISION well. Let cache: fail cleanly."

            // Register writable save device for PS4 (SCE savedata mount at /savedata0/)
#ifdef LIBERTY_RECOMP_PS4
            if (!g_ps4SaveMountPoint.empty()) {
                auto saveDevice = std::make_unique<rex::filesystem::HostPathDevice>(
                    "\\Device\\SavePartition", std::filesystem::path(g_ps4SaveMountPoint),
                    /*read_only=*/false);
                if (saveDevice->Initialize()) {
                    fs->RegisterDevice(std::move(saveDevice));
                    fs->RegisterSymbolicLink("save:", "\\Device\\SavePartition");
                    DIAG_EMIT("[Main] Registered save: device at %s\n",
                              g_ps4SaveMountPoint.c_str());
                } else {
                    DIAG_EMIT("[Main] WARNING: Failed to initialize save device at %s\n",
                              g_ps4SaveMountPoint.c_str());
                }
            }
#endif

            // DLC path resolution is handled by the XAM content system at runtime.
            // When the game calls XamContentCreateEx for DLC, XamRootCreate maps the
            // content root name directly to GetGamePath()/dlc/ on the host filesystem.
            // No VFS HostPathDevice mounts are needed.

            DIAG_EMIT("[Main] Registered GTA IV VFS symlinks: common:, platform:, audio:, xbox360:, x:, update:, cache:, cache1:\n");
        }

    }

    if (forceInstallationCheck)
    {
#if !REX_PLATFORM_CONSOLE
        // Create the console to show progress to the user, otherwise it will seem as if the game didn't boot at all.
        os::process::ShowConsole();

        Journal journal;
        double lastProgressMiB = 0.0;
        double lastTotalMib = 0.0;
        Installer::checkInstallIntegrity(GAME_INSTALL_DIRECTORY, journal, [&]()
        {
            constexpr double MiBDivisor = 1024.0 * 1024.0;
            constexpr double MiBProgressThreshold = 128.0;
            double progressMiB = double(journal.progressCounter) / MiBDivisor;
            double totalMiB = double(journal.progressTotal) / MiBDivisor;
            if (journal.progressCounter > 0)
            {
                if ((progressMiB - lastProgressMiB) > MiBProgressThreshold)
                {
                    fprintf(stdout, "Checking files: %0.2f MiB / %0.2f MiB\n", progressMiB, totalMiB);
                    lastProgressMiB = progressMiB;
                }
            }
            else
            {
                if ((totalMiB - lastTotalMib) > MiBProgressThreshold)
                {
                    fprintf(stdout, "Scanning files: %0.2f MiB\n", totalMiB);
                    lastTotalMib = totalMiB;
                }
            }

            return true;
        });

        char resultText[512];
        if (journal.lastResult == Journal::Result::Success)
        {
            snprintf(resultText, sizeof(resultText), "%s", Localise("IntegrityCheck_Success").c_str());
            fprintf(stdout, "%s\n", resultText);
        }
        else
        {
            snprintf(resultText, sizeof(resultText), Localise("IntegrityCheck_Failed").c_str(), journal.lastErrorMessage.c_str());
            fprintf(stderr, "%s\n", resultText);
        }

#if !REX_PLATFORM_CONSOLE
        uint32_t messageBoxStyle = (journal.lastResult == Journal::Result::Success)
            ? SDL_MESSAGEBOX_INFORMATION : SDL_MESSAGEBOX_ERROR;
        SDL_ShowSimpleMessageBox(messageBoxStyle, GameWindow::GetTitle(), resultText, GameWindow::s_pWindow);
#endif
        DIAG_EMIT("[EXIT-TRACE] main.cpp:637 calling _Exit\n");
        std::_Exit(int(journal.lastResult));
#endif // !PS4 && !NX
    }

#if defined(_WIN32) && defined(LIBERTY_RECOMP_D3D12)
    for (auto& dll : g_D3D12RequiredModules)
    {
        if (!std::filesystem::exists(g_executableRoot / dll))
        {
            char text[512];
            snprintf(text, sizeof(text), Localise("System_Win32_MissingDLLs").c_str(), dll.data());
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, GameWindow::GetTitle(), text, GameWindow::s_pWindow);
            DIAG_EMIT("[EXIT-TRACE] main.cpp:648 calling _Exit\n");
            std::_Exit(1);
        }
    }
#endif

    if (Config::ShowConsole)
        os::process::ShowConsole();
    LOGN("Host Startup");
    HostStartup();
#if defined(LIBERTY_RECOMP_DISCORD_RPC)
    os::discord::Initialize();
#endif

    std::filesystem::path modulePath;

#if defined(LIBERTY_RECOMP_EMBEDDED_ASSETS)
    // ── Embedded build (PS4 / Switch / iOS / Android) ────────────────────────
    // Game files are already present in the package; skip installer wizard and
    // main menu entirely.  modulePath points directly at the embedded XEX.
    //
    // On Android we support two delivery modes:
    //   (a) Folder picker — LibertySDLActivity pushes an already-extracted
    //       game directory via nativeSetGameRoot(). g_androidGameRoot is set,
    //       GetGameRoot() returns it verbatim, and there is no OBB to unpack.
    //   (b) OBB expansion file — the APK ships alongside a .obb zip under
    //       /sdcard/Android/obb/<pkg>/main.<versionCode>.<pkg>.obb. We locate
    //       the zip, unzip it into internal storage, and update the cached
    //       game root. Only runs when a .obb file actually exists; the env
    //       var that names the OBB directory (g_androidObbPath) always points
    //       at the sandboxed OBB directory even when empty, so we probe for
    //       the specific .obb file before attempting extraction.
#if defined(__ANDROID__)
    {
        extern const char* g_androidObbPath;
        extern const char* g_androidGameRoot;
        const bool folderPickerMode = g_androidGameRoot && g_androidGameRoot[0] != '\0';
        if (!folderPickerMode && g_androidObbPath) {
            std::error_code ec;
            std::filesystem::path obbDir(g_androidObbPath);
            std::filesystem::path obbFile;
            if (std::filesystem::is_directory(obbDir, ec)) {
                for (const auto& e : std::filesystem::directory_iterator(obbDir, ec)) {
                    if (!ec && e.is_regular_file(ec) && e.path().extension() == ".obb") {
                        obbFile = e.path();
                        break;
                    }
                }
            }
            if (!obbFile.empty()) {
                auto destPath = EmbeddedAssets::GetGameRoot().parent_path();
                if (!EmbeddedAssets::ExtractObbIfNeeded(obbFile, destPath)) {
                    printf("[Main] FATAL: Failed to extract OBB payload\n");
                    fflush(stdout);
                    std::_Exit(1);
                }
            }
        }
    }
#endif
    modulePath = EmbeddedAssets::GetGameRoot() / "default.xex";
    if (!std::filesystem::exists(modulePath)) {
        printf("[Main] FATAL: Embedded game XEX not found at %s\n",
               modulePath.string().c_str());
        fflush(stdout);
        std::_Exit(1);
    }
    DIAG_EMIT("[Main] Embedded build — game root: %s\n",
              EmbeddedAssets::GetGameRoot().string().c_str());

    // Video device is still needed for rendering.
    if (!Video::CreateHostDevice(sdlVideoDriver, graphicsApiRetry))
        ShowVideoBackendErrorAndExit();

    const bool runInstallerWizard = false;
    const bool showMainMenu       = false;

#else
    // ── Desktop build ─────────────────────────────────────────────────────────
    bool isGameInstalled = Installer::checkGameInstall(GetGamePath(), modulePath);
    bool runInstallerWizard = forceInstaller || forceDLCInstaller || !isGameInstalled;

    if (runInstallerWizard)
    {
        if (!Video::CreateHostDevice(sdlVideoDriver, graphicsApiRetry))
            ShowVideoBackendErrorAndExit();

        if (!InstallerWizard::Run(GetGamePath(), isGameInstalled && forceDLCInstaller))
        {
            DIAG_EMIT("[EXIT-TRACE] main.cpp:675 calling _Exit\n");
            std::_Exit(0);
        }
    }

    bool showMainMenu = false;
    for (uint32_t i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--skip-menu") == 0) showMainMenu = false;
        if (strcmp(argv[i], "--show-menu") == 0) showMainMenu = true;
    }

    if (showMainMenu)
    {
        if (!runInstallerWizard)
        {
            if (!Video::CreateHostDevice(sdlVideoDriver, graphicsApiRetry))
                ShowVideoBackendErrorAndExit();
        }
        MainMenu::Init();
        if (!MainMenu::Run())
        {
            MainMenu::Shutdown();
            DIAG_EMIT("[EXIT-TRACE] main.cpp:707 calling _Exit\n");
            std::_Exit(0);
        }
        MainMenu::Shutdown();
    }

    if (!runInstallerWizard && !showMainMenu)
    {
        DIAG_EMIT("[Main] Creating video device...\n");
        if (!Video::CreateHostDevice(sdlVideoDriver, graphicsApiRetry))
            ShowVideoBackendErrorAndExit();
        DIAG_EMIT("[Main] Video device created\n");
    }
#endif // LIBERTY_RECOMP_EMBEDDED_ASSETS

    // ------------------------------------------------------------------
    // Step 1: Normal Liberty initialization.
    // KiSystemStartup copies xex_header_data (function pointers, delegate
    // table) and zeroes BSS.  This sets up Liberty's expected layout.
    // ------------------------------------------------------------------
    DIAG_EMIT("[Main] Calling KiSystemStartup...\n");
    KiSystemStartup();
    DIAG_EMIT("[Main] KiSystemStartup done\n");

    DIAG_EMIT("[Main] Loading module (Liberty): %s\n", modulePath.string().c_str());
    LdrLoadModule(modulePath);
    DIAG_EMIT("[Main] Liberty module prep done\n");

    // ------------------------------------------------------------------
    // Step 2: Load the XEX through RexGlue.
    // ReadImage decompresses the FULL PE into guest memory, overwriting
    // Liberty's header + BSS.  This gives us correct .data/.rdata section
    // initial values that the CRT and game code require.
    // ------------------------------------------------------------------
    {
        auto* rt = rex::Runtime::instance();

        // VFS setup removed — rex::Runtime::SetupVfs() (runtime.cpp:253-262)
        // already registers the HostPathDevice at \Device\Harddisk0\Partition1
        // plus symlinks for game: and d: during Runtime construction. Liberty's
        // duplicate block was re-inserting a second HostPathDevice + re-binding
        // the symlinks every boot.

        rex::X_STATUS xst = rt->LoadXexImage("game:\\default.xex");
        if (xst != 0) {
            printf("[Main] FATAL: LoadXexImage failed: 0x%08X\n", xst);
            fflush(stdout);
            DIAG_EMIT("[EXIT-TRACE] main.cpp:766 calling _Exit\n");
            std::_Exit(1);
        }

        // Unprotect so we can re-apply Liberty's header patches.
        auto* heap = rt->memory()->LookupHeap(PPC_IMAGE_BASE);
        if (heap) {
            heap->Protect(PPC_IMAGE_BASE, PPC_IMAGE_SIZE,
                          rex::memory::kMemoryProtectRead | rex::memory::kMemoryProtectWrite);
        }
        DIAG_EMIT("[Main] XEX loaded — PE data sections now authoritative\n");
    }

    // xex_header_data diff removed — was a diagnostic for an overlay
    // we stopped applying. rexglue's LoadXexImage is authoritative.
    {
        // GPU context GOT entry — keep this patch.
        // RexGlue may not resolve this specific import (ordinal 446).
        constexpr uint32_t GOT_GPU_CONTEXT = 0x82000768;
        constexpr uint32_t GPU_CONTEXT_GLOBAL = 0x83124900;
        uint32_t currentGOT = __builtin_bswap32(*reinterpret_cast<uint32_t*>(g_memory.base + GOT_GPU_CONTEXT));
        if (currentGOT != GPU_CONTEXT_GLOBAL) {
            uint32_t* gotEntry = reinterpret_cast<uint32_t*>(g_memory.base + GOT_GPU_CONTEXT);
            *gotEntry = __builtin_bswap32(GPU_CONTEXT_GLOBAL);
            DIAG_EMIT("[Main] GPU GOT 0x%08X -> 0x%08X (was 0x%08X)\n",
                      GOT_GPU_CONTEXT, GPU_CONTEXT_GLOBAL, currentGOT);
        } else {
            DIAG_EMIT("[Main] GPU GOT already correct (0x%08X)\n", GPU_CONTEXT_GLOBAL);
        }
    }

#ifndef LIBERTY_RECOMP_NO_CURL
    // Check the time since the last time an update was checked.
    constexpr double TimeBetweenUpdateChecksInSeconds = 6 * 60 * 60;
    time_t timeNow = std::time(nullptr);
    double timeDifferenceSeconds = difftime(timeNow, Config::LastChecked);
    if (timeDifferenceSeconds > TimeBetweenUpdateChecksInSeconds)
    {
        UpdateChecker::initialize();
        UpdateChecker::start();
        Config::LastChecked = timeNow;
        Config::Save();
    }
#endif

    // Install a terminate handler to diagnose uncaught exceptions.
    std::set_terminate([]() {
        fprintf(stderr, "[TERMINATE] std::terminate() called\n");
        auto eptr = std::current_exception();
        if (eptr) {
            try { std::rethrow_exception(eptr); }
            catch (const rex::SehException& e) {
                fprintf(stderr, "[TERMINATE] SEH: %s  code=0x%08X addr=0x%016llX\n",
                        e.what(), (unsigned)e.code(),
                        (unsigned long long)e.address());
            }
            catch (const std::exception& e) {
                fprintf(stderr, "[TERMINATE] exception: %s\n", e.what());
            }
            catch (...) {
                fprintf(stderr, "[TERMINATE] unknown exception\n");
            }
        } else {
            fprintf(stderr, "[TERMINATE] no current exception\n");
        }
        fflush(stderr);
        // This is the emergency terminate path. Keep the exception report
        // above unconditional, but suppress the redundant healthy-path trace
        // marker unless launch diagnostics were explicitly enabled.
        DIAG_EMIT("[EXIT-TRACE] main.cpp:860 calling _Exit\n");
        std::_Exit(99);
    });

    // ------------------------------------------------------------------
    // Launch the game via RexGlue's LaunchModule — creates the main
    // XThread with entry point, stack size, and TLS from the XEX header.
    // This is exactly how standalone RexGlue works.
    // ------------------------------------------------------------------
    LOGN("Start Guest Thread");
    LOGN(modulePath.string());

    auto* rt = rex::Runtime::instance();
    // DIAG: verify xstart is still registered right before LaunchModule
    if (::os::diag::ShouldEmit()) {
        auto* p = rt->function_dispatcher();
        PPCFunc* xsf = p->GetFunction(0x82A11290);
        fprintf(stderr, "[DIAG] Before LaunchModule(): GetFunction(0x82A11290)=%p  HasFT=%d  instance=%p\n",
                (void*)xsf, (int)p->HasFunctionTable(), (void*)rt);
        fflush(stderr);
    }
    auto main_xthread = rt->LaunchModule();
    if (!main_xthread) {
        printf("[Main] FATAL: LaunchModule() returned null\n");
        fflush(stdout);
        DIAG_EMIT("[EXIT-TRACE] main.cpp:884 calling _Exit\n");
        std::_Exit(1);
    }
    StartLibertyWatchdog();
    // rexglue's own XThread::Execute log already covers this; suppress the
    // duplicate printf (was polluting stdout with an untagged line).
#if defined(LIBERTY_RECOMP_DISCORD_RPC)
    os::discord::SetState(os::discord::GameState::InGame);
#endif

    // Wait for the XThread to actually begin executing.
    for (int i = 0; i < 5000 && !main_xthread->is_running(); ++i) {
#if !REX_PLATFORM_CONSOLE
        SDL_Delay(1);
#else
        usleep(1000);
#endif
        if (i == 100 || i == 1000 || i == 3000) {
            DIAG_EMIT("[Main] Still waiting for XThread (i=%d, running=%d)\n",
                      i, (int)main_xthread->is_running());
        }
    }
    if (!main_xthread->is_running()) {
        DIAG_EMIT("[Main] WARNING: XThread did not start within 5 seconds\n");
    }

    // Main thread: pump SDL events while game runs on XThread.
    // SDL requires event pumping on the main thread (macOS Cocoa requirement).
#if REX_PLATFORM_NX
    extern bool SwitchAppletIsRunning();
#endif
#if defined(LIBERTY_RECOMP_DISCORD_RPC)
    static int s_discordCallbackTick = 0;
#endif
    while (main_xthread->is_running()
#if REX_PLATFORM_NX
           && SwitchAppletIsRunning()
#endif
    ) {
#if !REX_PLATFORM_CONSOLE
        SDL_PumpEvents();
#endif
#if defined(LIBERTY_RECOMP_DISCORD_RPC)
        // Pump Discord callbacks ~once per second (every 1000 ms of 1ms sleeps)
        if (++s_discordCallbackTick >= 1000) {
            os::discord::RunCallbacks();
            s_discordCallbackTick = 0;
        }
#endif
#if !REX_PLATFORM_CONSOLE
        SDL_Delay(1);
#else
        usleep(1000);
#endif
    }

    StopLibertyWatchdog();
    DIAG_EMIT("[Main] Main XThread finished\n");
#if defined(LIBERTY_RECOMP_DISCORD_RPC)
    os::discord::Shutdown();
#endif
    return 0;
}

// String function stubs removed — RexGlue (xboxkrnl_strings) handles these.
