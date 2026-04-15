#include <cstdio>
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
#if defined(LIBERTY_RECOMP_PS4) || defined(LIBERTY_RECOMP_NX)
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
#if !LIBERTY_RECOMP_PS4 && !LIBERTY_RECOMP_NX
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
#if !defined(LIBERTY_RECOMP_PS4) && !defined(LIBERTY_RECOMP_NX)
#include <ui/installer_wizard.h>
#endif
#include <ui/main_menu.h>
#include <mod/mod_loader.h>
#include <preload_executable.h>
#include <iostream>
#include <app.h>
#include <debugger.h>

#include <rex/exception_handler.h>
#include <rex/platform/exceptions.h>  // rex::SehException
#include <rex/logging.h>
#include <rex/runtime.h>
// #include <rex/runtime/guest/exceptions.h>  // Not present in graine SDK; unused
// #include <rex/runtime/processor.h>         // Not present in graine SDK; unused
#include <rex/system/kernel_state.h>
#include <rex/system/xthread.h>
#include <rex/system/xobject.h>
#include <rex/system/user_module.h>
#include <rex/filesystem/vfs.h>
#include <rex/filesystem/devices/host_path_device.h>
#include <rex/kernel/crt/heap.h>
#include <rex/chrono/clock.h>
#if defined(LIBERTY_RECOMP_PS4)
#include "../glue/rexglue-sdk-main/src/audio/orbis/orbis_audio_system.h"
#elif defined(LIBERTY_RECOMP_NX)
#include <rex/audio/switch/switch_audio_system.h>
#else
#include <rex/audio/sdl/sdl_audio_system.h>
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
#if !defined(_WIN32) && !defined(LIBERTY_RECOMP_PS4) && !defined(LIBERTY_RECOMP_NX)
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
    fprintf(stderr, "[RexGlue] Fallback exception handler installed (signal handlers active)\n");
    fflush(stderr);
}

// ============================================================================

static void ShowVideoBackendErrorAndExit()
{
#if !defined(LIBERTY_RECOMP_PS4) && !defined(LIBERTY_RECOMP_NX)
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
    printf("[EXIT-TRACE] main.cpp:203 calling _Exit\n"); fflush(stdout);
    std::_Exit(1);
}

void HostStartup()
{
#ifdef _WIN32
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#endif

    hid::Init();
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
#if !defined(LIBERTY_RECOMP_PS4) && !defined(LIBERTY_RECOMP_NX)
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, GameWindow::GetTitle(), Localise("System_MemoryAllocationFailed").c_str(), GameWindow::s_pWindow);
#endif
        fprintf(stderr, "[Main] Memory allocation failed\n"); fflush(stderr);
        printf("[EXIT-TRACE] main.cpp:230 calling _Exit\n"); fflush(stdout);
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

uint32_t LdrLoadModule(const std::filesystem::path &path)
{
    const auto loadResult = LoadFile(path);
    if (loadResult.empty())
    {
        assert("Failed to load module" && false);
        return 0;
    }

    const auto image = Image::ParseImage(loadResult.data(), loadResult.size());

    // Write PE to guest memory so we can extract XDBF from guest addresses.
    // LoadXexImage will overwrite this with its own PE decompression later,
    // but we need the image in memory now for the XDBF wrapper.
    memcpy(g_memory.Translate(image.base), image.data.get(), image.size);
    g_xdbfWrapper = XDBFWrapper(static_cast<uint8_t*>(g_memory.Translate(image.resource_offset)), image.resource_size);

    // NOTE: Collision fix, worker globals zeroing, .rdata protection all
    // removed — RexGlue's LoadXexImage overwrites guest memory with the
    // correct PE data and handles BSS zeroing.  Those patches were
    // overwritten anyway and are no longer needed.

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

        printf("[EXIT-TRACE] main.cpp:318 calling _Exit\n"); fflush(stdout);
        std::_Exit(1);
    }
}
#endif

int main(int argc, char *argv[])
{
    // Install RexGlue's signal handlers + fallback crash handler early.
    // MMIOHandler is installed later after g_memory.base is valid.
    InstallRexGlueExceptionHandlers();
    
#ifdef _WIN32
    timeBeginPeriod(1);
#endif

    os::process::CheckConsole();

    if (!os::registry::Init())
        LOGN("OS does not support registry.");

    os::logger::Init();

    PreloadContext preloadContext;
    preloadContext.PreloadExecutable();

    bool forceInstaller = false;
    bool forceDLCInstaller = false;
    bool useDefaultWorkingDirectory = false;
    bool forceInstallationCheck = false;
    bool graphicsApiRetry = false;
    const char *sdlVideoDriver = nullptr;
    std::string sdlVideoDriverStr;

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

        try { cli.parse(argc, argv); }
        catch (const CLI::ParseError &e) { return cli.exit(e); }

        App::s_isSkipLogos = App::s_isSkipLogos || skipLogos;
        if (!sdlVideoDriverStr.empty())
            sdlVideoDriver = sdlVideoDriverStr.c_str();
    }

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
    fprintf(stderr, "[Main] Getting game path...\n");
    const auto gamePath = GetGamePath();
    fprintf(stderr, "[Main] Game path: %s\n", gamePath.string().c_str());
    const auto rexContentRoot = gamePath / "game";
    fprintf(stderr, "[Main] Content root: %s\n", rexContentRoot.string().c_str());
    fprintf(stderr, "[Main] Content root exists: %d\n", (int)std::filesystem::exists(rexContentRoot));

    // Extract title update files BEFORE VFS init — HostPathDevice snapshots the
    // directory at mount time, so default.xexp must exist before rex::Runtime::Setup().
#if !LIBERTY_RECOMP_PS4 && !LIBERTY_RECOMP_NX
    {
        AutoInstallResult aiResult = AutoInstaller::Run(gamePath);
        if (!aiResult.errorMessage.empty())
            fprintf(stderr, "[AutoInstall] Warning: %s\n", aiResult.errorMessage.c_str());
    }
#endif

    // Bridge extracted audio.rpf content to where the RAGE relative device expects it.
    // The packfile device at audio:/ has an AES-encrypted TOC that fails to decrypt in
    // the recompiled env. The fallback relative device uses root game:/xbox360/audio/,
    // but extracted config files live at audio/config/. Copy the directory tree.
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
                fprintf(stderr, "[Main] Copied audio config: %s -> %s\n",
                        srcPath.string().c_str(), destPath.string().c_str());
            } else {
                fprintf(stderr, "[Main] WARN: Could not copy audio config: %s\n", ec.message().c_str());
            }
        }
    }

    // Bridge DLC directories to where RexGlue's ContentManager expects them.
    // ContentManager::ListContent() scans: content_root / title_id / content_type / *
    // GTA IV title_id = 545407F2, DLC content_type = 00000002 (kMarketplaceContent).
    // Our DLC is at gamePath/dlc/TLAD and gamePath/dlc/TBOGT.
    // Copy instead of symlink so there are no filesystem portability issues.
    {
        const auto dlcContentDir = rexContentRoot / "545407F2" / "00000002";
        std::error_code ec;
        std::filesystem::create_directories(dlcContentDir, ec);

        const char* dlcNames[] = { "TLAD", "TBOGT" };
        for (const char* name : dlcNames) {
            auto actualDlc = gamePath / "dlc" / name;
            auto linkPath  = dlcContentDir / name;
            if (std::filesystem::is_directory(actualDlc, ec) &&
                !std::filesystem::exists(linkPath, ec)) {
                std::filesystem::copy(actualDlc, linkPath,
                    std::filesystem::copy_options::recursive |
                    std::filesystem::copy_options::skip_existing, ec);
                if (!ec) {
                    fprintf(stderr, "[Main] DLC bridge copy: %s -> %s\n",
                            actualDlc.string().c_str(), linkPath.string().c_str());
                } else {
                    fprintf(stderr, "[Main] WARN: DLC bridge copy failed for %s: %s\n",
                            name, ec.message().c_str());
                }
            }
        }
    }

    // Bridge DLC directories into game root for direct file probing.
    // GTA IV's DLC discovery probes hardcoded paths: game:\DLC1\setup2.xml
    // and game:\DLC1\DLC.rpf (TLAD = DLC1, TBOGT = DLC2). These bypass the
    // XAM content system and open files directly via the game: VFS mount.
    // The game: mount points to rexContentRoot, so we copy DLC content there.
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
            fprintf(stderr, "[Main] DLC %s: src=%s exists=%d, dst=%s exists=%d\n",
                    mapping.gameDirName, actualDlc.string().c_str(), (int)srcExists,
                    linkPath.string().c_str(), (int)dstExists);
            if (srcExists && !dstExists) {
                std::filesystem::copy(actualDlc, linkPath,
                    std::filesystem::copy_options::recursive |
                    std::filesystem::copy_options::skip_existing, ec);
                if (!ec) {
                    fprintf(stderr, "[Main] DLC game bridge copy: %s -> %s\n",
                            actualDlc.string().c_str(), linkPath.string().c_str());
                } else {
                    fprintf(stderr, "[Main] WARN: DLC game bridge copy failed for %s: %s\n",
                            mapping.gameDirName, ec.message().c_str());
                }
            }
        }
    }

    {
        static std::unique_ptr<rex::Runtime> s_rexRuntime;
        fprintf(stderr, "[Main] Constructing rex::Runtime...\n");
        s_rexRuntime = std::make_unique<rex::Runtime>(
            rexContentRoot /* game_data_root - SDK v0.2.1 first arg */);
        fprintf(stderr, "[Main] Runtime constructed, calling Setup() with func mappings...\n");

        // Let guest::initialize() (called inside Setup) install its SEH
        // signal handler normally.  After Setup(), we re-install
        // ExceptionHandler on top so the chain becomes:
        //   ExceptionHandler (NULL-PC, MMIO) → SEH (data faults in __try)
        // This matches standalone: ExceptionHandler saves SEH as its
        // "previous handler" and falls back to it for unhandled faults.
        // Give RexGlue full audio control via its SDL AudioSystem.
        // Liberty's audio hooks in imports.cpp have been removed so RexGlue's
        // XBOXKRNL exports (XMACreateContext, XAudioRegisterRenderDriverClient,
        // etc.) handle all game audio calls with full Xenia-based implementations.
        rex::RuntimeConfig rexConfig;
#if defined(LIBERTY_RECOMP_PS4)
        rexConfig.audio_factory = REX_AUDIO_BACKEND(rex::audio::orbis::OrbisAudioSystem);
#elif defined(LIBERTY_RECOMP_NX)
        rexConfig.audio_factory = REX_AUDIO_BACKEND(rex::audio::nx::SwitchAudioSystem);
#else
        rexConfig.audio_factory = REX_AUDIO_BACKEND(rex::audio::sdl::SDLAudioSystem);
#endif

        uint32_t rt_status = s_rexRuntime->Setup(
            static_cast<uint32_t>(PPC_CODE_BASE),
            static_cast<uint32_t>(PPC_CODE_SIZE),
            static_cast<uint32_t>(PPC_IMAGE_BASE),
            static_cast<uint32_t>(PPC_IMAGE_SIZE),
            PPCFuncMappings,
            std::move(rexConfig));
        fprintf(stderr, "[Main] Setup() returned 0x%08X\n", rt_status);

        // Xbox 360 timebase runs at exactly 50MHz.
        // Must be set before any PPC_QUERY_TIMEBASE() call fires, so the game's
        // timing math (loading-screen task advancement in sub_82143DC8) gets the
        // correct elapsed-time values.  Without this, guest_tick_frequency_ defaults
        // to the host CPU frequency (~1-4GHz), causing the 1/50M scaling constant
        // baked into the game to return effectively-zero milliseconds and every
        // time-gated loading task spins forever at state=0.
        rex::chrono::Clock::set_guest_tick_frequency(50000000ULL);
        fprintf(stderr, "[Main] Guest tick frequency set to 50MHz (Xbox 360 timebase)\n");

        if (rt_status != 0 /* X_STATUS_SUCCESS */) {
            fprintf(stderr, "[Main] FATAL: rex::Runtime::Setup() failed with 0x%08X\n", rt_status);
            printf("[EXIT-TRACE] main.cpp:478 calling _Exit\n"); fflush(stdout);
            std::_Exit(1);
        }

        // Initialize rexcrt heap with the Runtime's Memory (matches rex_app.cpp pattern).
        // Must be after Setup() so runtime_->memory() is valid.
        if (!rex::kernel::crt::InitHeap(FLAGS_rexcrt_heap_size_mb, s_rexRuntime->memory())) {
            fprintf(stderr, "[Main] FATAL: rexcrt heap init failed\n");
            std::_Exit(1);
        }
        fprintf(stderr, "[Main] rexcrt heap initialized (%u MB)\n", FLAGS_rexcrt_heap_size_mb);
        // DIAG: verify xstart is registered after Setup().
        // Gated by LIBERTY_VERBOSE_DIAG env var — silent in normal runs.
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
        fprintf(stderr, "[Main] RexGlue memory: base=%p\n", (void*)g_memory.base);
        // Set base pointer for rexcrt memset watchpoint diagnostic
        // Populate vtables and install function stubs into guest memory.
        g_memory.PopulateFunctionTableAndVtables();
        printf("[Main] rex::Runtime created (kernel_state=%p)\n",
               (void*)s_rexRuntime->kernel_state()); fflush(stdout);

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
            // update:\path -> \Device\Harddisk0\Partition1\update\path
            // GTA IV v8 title update content (effects, text, shaders)
            fs->RegisterSymbolicLink("update:", "\\Device\\Harddisk0\\Partition1\\update");
            // Register writable cache device for Xbox 360 temp/scratch storage
            auto cachePath = GetUserPath() / "cache";
            std::filesystem::create_directories(cachePath);
            auto cacheDevice = std::make_unique<rex::filesystem::HostPathDevice>(
                "\\Device\\CachePartition", cachePath, /*read_only=*/false);
            if (cacheDevice->Initialize()) {
                fs->RegisterDevice(std::move(cacheDevice));
                fs->RegisterSymbolicLink("cache:", "\\Device\\CachePartition");
                fs->RegisterSymbolicLink("cache1:", "\\Device\\CachePartition");
                printf("[Main] Registered cache: device at %s\n", cachePath.string().c_str());
            } else {
                printf("[Main] WARNING: Failed to initialize cache device\n");
            }

            // Register writable save device for PS4 (SCE savedata mount at /savedata0/)
#ifdef LIBERTY_RECOMP_PS4
            if (!g_ps4SaveMountPoint.empty()) {
                auto saveDevice = std::make_unique<rex::filesystem::HostPathDevice>(
                    "\\Device\\SavePartition", std::filesystem::path(g_ps4SaveMountPoint),
                    /*read_only=*/false);
                if (saveDevice->Initialize()) {
                    fs->RegisterDevice(std::move(saveDevice));
                    fs->RegisterSymbolicLink("save:", "\\Device\\SavePartition");
                    printf("[Main] Registered save: device at %s\n",
                           g_ps4SaveMountPoint.c_str());
                } else {
                    printf("[Main] WARNING: Failed to initialize save device at %s\n",
                           g_ps4SaveMountPoint.c_str());
                }
            }
#endif

            // DLC path resolution is handled by the XAM content system at runtime.
            // When the game calls XamContentCreateEx for DLC, XamRootCreate maps the
            // content root name directly to GetGamePath()/dlc/ on the host filesystem.
            // No VFS HostPathDevice mounts are needed.

                        printf("[Main] Registered GTA IV VFS symlinks: common:, platform:, audio:, xbox360:, x:, update:, cache:, cache1:\n");
            fflush(stdout);
        }

    }

    if (forceInstallationCheck)
    {
#if !LIBERTY_RECOMP_PS4 && !LIBERTY_RECOMP_NX
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

#if !defined(LIBERTY_RECOMP_PS4) && !defined(LIBERTY_RECOMP_NX)
        uint32_t messageBoxStyle = (journal.lastResult == Journal::Result::Success)
            ? SDL_MESSAGEBOX_INFORMATION : SDL_MESSAGEBOX_ERROR;
        SDL_ShowSimpleMessageBox(messageBoxStyle, GameWindow::GetTitle(), resultText, GameWindow::s_pWindow);
#endif
        printf("[EXIT-TRACE] main.cpp:637 calling _Exit\n"); fflush(stdout);
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
            printf("[EXIT-TRACE] main.cpp:648 calling _Exit\n"); fflush(stdout);
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
    // On Android, extract the OBB payload to internal storage on first boot.
#if defined(__ANDROID__)
    {
        extern const char* g_androidObbPath;
        if (g_androidObbPath) {
            auto obbPath  = std::filesystem::path(g_androidObbPath);
            auto destPath = EmbeddedAssets::GetGameRoot().parent_path();
            if (!EmbeddedAssets::ExtractObbIfNeeded(obbPath, destPath)) {
                printf("[Main] FATAL: Failed to extract OBB payload\n");
                fflush(stdout);
                std::_Exit(1);
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
    printf("[Main] Embedded build — game root: %s\n",
           EmbeddedAssets::GetGameRoot().string().c_str()); fflush(stdout);

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
            printf("[EXIT-TRACE] main.cpp:675 calling _Exit\n"); fflush(stdout);
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
            printf("[EXIT-TRACE] main.cpp:707 calling _Exit\n"); fflush(stdout);
            std::_Exit(0);
        }
        MainMenu::Shutdown();
    }

    if (!runInstallerWizard && !showMainMenu)
    {
        printf("[Main] Creating video device...\n"); fflush(stdout);
        if (!Video::CreateHostDevice(sdlVideoDriver, graphicsApiRetry))
            ShowVideoBackendErrorAndExit();
        printf("[Main] Video device created\n"); fflush(stdout);
    }
#endif // LIBERTY_RECOMP_EMBEDDED_ASSETS

    // ------------------------------------------------------------------
    // Step 1: Normal Liberty initialization.
    // KiSystemStartup copies xex_header_data (function pointers, delegate
    // table) and zeroes BSS.  This sets up Liberty's expected layout.
    // ------------------------------------------------------------------
    printf("[Main] Calling KiSystemStartup...\n"); fflush(stdout);
    KiSystemStartup();
    printf("[Main] KiSystemStartup done\n"); fflush(stdout);

    printf("[Main] Loading module (Liberty): %s\n", modulePath.string().c_str()); fflush(stdout);
    LdrLoadModule(modulePath);
    printf("[Main] Liberty module prep done\n"); fflush(stdout);

    // ------------------------------------------------------------------
    // Step 2: Load the XEX through RexGlue.
    // ReadImage decompresses the FULL PE into guest memory, overwriting
    // Liberty's header + BSS.  This gives us correct .data/.rdata section
    // initial values that the CRT and game code require.
    // ------------------------------------------------------------------
    {
        auto* rt = rex::Runtime::instance();

#if defined(LIBERTY_RECOMP_EMBEDDED_ASSETS)
        auto gameDir = EmbeddedAssets::GetGameRoot();
#else
        auto gameDir = modulePath.parent_path();
#endif
        printf("[Main] Setting up RexGlue VFS: %s\n", gameDir.string().c_str()); fflush(stdout);

        auto device = std::make_unique<rex::filesystem::HostPathDevice>(
            "\\Device\\Harddisk0\\Partition1", gameDir, /*read_only=*/true);
        if (!device->Initialize()) {
            printf("[Main] FATAL: Failed to initialize VFS host device\n");
            fflush(stdout);
            printf("[EXIT-TRACE] main.cpp:755 calling _Exit\n"); fflush(stdout);
            std::_Exit(1);
        }
        rt->file_system()->RegisterDevice(std::move(device));
        rt->file_system()->RegisterSymbolicLink("game:", "\\Device\\Harddisk0\\Partition1");
        rt->file_system()->RegisterSymbolicLink("d:", "\\Device\\Harddisk0\\Partition1");
        printf("[Main] RexGlue VFS ready\n"); fflush(stdout);

        rex::X_STATUS xst = rt->LoadXexImage("game:\\default.xex");
        if (xst != 0) {
            printf("[Main] FATAL: LoadXexImage failed: 0x%08X\n", xst);
            fflush(stdout);
            printf("[EXIT-TRACE] main.cpp:766 calling _Exit\n"); fflush(stdout);
            std::_Exit(1);
        }

        // Unprotect so we can re-apply Liberty's header patches.
        auto* heap = rt->memory()->LookupHeap(PPC_IMAGE_BASE);
        if (heap) {
            heap->Protect(PPC_IMAGE_BASE, PPC_IMAGE_SIZE,
                          rex::memory::kMemoryProtectRead | rex::memory::kMemoryProtectWrite);
        }
        printf("[Main] XEX loaded — PE data sections now authoritative\n");
        fflush(stdout);
    }

    // ------------------------------------------------------------------
    // Step 3: Diagnostic — compare xex_header_data against RexGlue's PE.
    // The function dispatch table lives at base+0x831F0000+ (PPC_LOOKUP_FUNC),
    // completely independent of this region.  RexGlue's LoadXexImage already
    // wrote the correct PE data here AND resolved variable imports.
    // We no longer overwrite with xex_header_data — that was destroying
    // RexGlue's import resolution and potentially corrupting .data values.
    // ------------------------------------------------------------------
    {
        #include <kernel/xex_header_data.h>
        const uint8_t* rexPE = reinterpret_cast<const uint8_t*>(g_memory.base + 0x82000000);
        int diffCount = 0;
        int importDiffs = 0;   // diffs in 0x700-0x950 (import slot range)
        int dataDiffs = 0;     // diffs elsewhere
        for (size_t i = 0; i < sizeof(xex_header_data); i++) {
            if (rexPE[i] != xex_header_data[i]) {
                diffCount++;
                bool inImportRange = (i >= 0x700 && i < 0x950);
                if (inImportRange) importDiffs++;
                else dataDiffs++;
                if (diffCount <= 20 && ::os::diag::ShouldEmit()) {
                    printf("[HeaderDiff] offset=0x%05zX rex=0x%02X hdr=0x%02X %s\n",
                           i, rexPE[i], xex_header_data[i],
                           inImportRange ? "(IMPORT)" : "(DATA)");
                }
            }
        }
        printf("[Main] Header comparison: %d diffs (%d import, %d data) out of %zu bytes\n",
               diffCount, importDiffs, dataDiffs, sizeof(xex_header_data));

        // xex_header_data contains XEX header metadata, NOT .rdata section data.
        // Applying it as overlay CORRUPTS vtables with ASCII string fragments
        // (e.g. "DIAL", "- SA", "age:", "Stor"). DO NOT apply.
        // RexGlue's PE decompression provides the correct .rdata content.
        printf("[Main] RexGlue PE is authoritative — NOT overwriting with xex_header_data\n");
        fflush(stdout);

        // GPU context GOT entry — keep this patch.
        // RexGlue may not resolve this specific import (ordinal 446).
        constexpr uint32_t GOT_GPU_CONTEXT = 0x82000768;
        constexpr uint32_t GPU_CONTEXT_GLOBAL = 0x83124900;
        uint32_t currentGOT = __builtin_bswap32(*reinterpret_cast<uint32_t*>(g_memory.base + GOT_GPU_CONTEXT));
        if (currentGOT != GPU_CONTEXT_GLOBAL) {
            uint32_t* gotEntry = reinterpret_cast<uint32_t*>(g_memory.base + GOT_GPU_CONTEXT);
            *gotEntry = __builtin_bswap32(GPU_CONTEXT_GLOBAL);
            printf("[Main] GPU GOT 0x%08X -> 0x%08X (was 0x%08X)\n",
                   GOT_GPU_CONTEXT, GPU_CONTEXT_GLOBAL, currentGOT);
        } else {
            printf("[Main] GPU GOT already correct (0x%08X)\n", GPU_CONTEXT_GLOBAL);
        }
        fflush(stdout);
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
        printf("[EXIT-TRACE] main.cpp:860 calling _Exit\n"); fflush(stdout);
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
        printf("[EXIT-TRACE] main.cpp:884 calling _Exit\n"); fflush(stdout);
        std::_Exit(1);
    }
    printf("[Main] Main XThread launched via RexGlue (handle=0x%08X)\n",
           main_xthread->handle()); fflush(stdout);
#if defined(LIBERTY_RECOMP_DISCORD_RPC)
    os::discord::SetState(os::discord::GameState::InGame);
#endif

    // Wait for the XThread to actually begin executing.
    for (int i = 0; i < 5000 && !main_xthread->is_running(); ++i) {
#if !defined(LIBERTY_RECOMP_PS4) && !defined(LIBERTY_RECOMP_NX)
        SDL_Delay(1);
#else
        usleep(1000);
#endif
        if (i == 100 || i == 1000 || i == 3000) {
            printf("[Main] Still waiting for XThread (i=%d, running=%d)\n",
                   i, (int)main_xthread->is_running()); fflush(stdout);
        }
    }
    if (!main_xthread->is_running()) {
        printf("[Main] WARNING: XThread did not start within 5 seconds\n");
        fflush(stdout);
    }

    // Main thread: pump SDL events while game runs on XThread.
    // SDL requires event pumping on the main thread (macOS Cocoa requirement).
#if defined(LIBERTY_RECOMP_NX)
    extern bool SwitchAppletIsRunning();
#endif
#if defined(LIBERTY_RECOMP_DISCORD_RPC)
    static int s_discordCallbackTick = 0;
#endif
    while (main_xthread->is_running()
#if defined(LIBERTY_RECOMP_NX)
           && SwitchAppletIsRunning()
#endif
    ) {
#if !defined(LIBERTY_RECOMP_PS4) && !defined(LIBERTY_RECOMP_NX)
        SDL_PumpEvents();
#endif
#if defined(LIBERTY_RECOMP_DISCORD_RPC)
        // Pump Discord callbacks ~once per second (every 1000 ms of 1ms sleeps)
        if (++s_discordCallbackTick >= 1000) {
            os::discord::RunCallbacks();
            s_discordCallbackTick = 0;
        }
#endif
#if !defined(LIBERTY_RECOMP_PS4) && !defined(LIBERTY_RECOMP_NX)
        SDL_Delay(1);
#else
        usleep(1000);
#endif
    }

    printf("[Main] Main XThread finished\n");
    fflush(stdout);
#if defined(LIBERTY_RECOMP_DISCORD_RPC)
    os::discord::Shutdown();
#endif
    return 0;
}

// String function stubs removed — RexGlue (xboxkrnl_strings) handles these.
