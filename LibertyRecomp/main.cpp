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
#ifndef _WIN32
#include <signal.h>
#endif
#include <cpu/guest_thread.h>
#include <gpu/video.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <kernel/heap.h>
#include <kernel/save_system.h>
#include <file.h>
#include <vector>
#include <image.h>
#include <apu/audio.h>
#include <hid/hid.h>
#include <user/config.h>
#include <user/paths.h>
#include <user/registry.h>
#include <kernel/xdbf.h>
#include <install/installer.h>
#include <install/update_checker.h>
#include <os/logger.h>
#include <os/process.h>
#include <os/registry.h>
#include <ui/game_window.h>
#include <ui/installer_wizard.h>
#include <ui/main_menu.h>
#include <mod/mod_loader.h>
#include <preload_executable.h>
#include <iostream>
#include <app.h>
#include <debugger.h>

#include <rex/exception_handler.h>
#include <rex/logging.h>
#include <rex/runtime.h>
#include <rex/runtime/guest/exceptions.h>
#include <rex/runtime/processor.h>
#include <rex/kernel/kernel_state.h>
#include <rex/kernel/xthread.h>
#include <rex/kernel/xobject.h>
#include <rex/kernel/user_module.h>
#include <rex/filesystem/vfs.h>
#include <rex/filesystem/devices/host_path_device.h>

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
#ifndef _WIN32
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
    rex::arch::ExceptionHandler::Install(RexFallbackCrashHandler, nullptr);
    fprintf(stderr, "[RexGlue] Fallback exception handler installed (signal handlers active)\n");
    fflush(stderr);
}

// ============================================================================

static void ShowVideoBackendErrorAndExit()
{
    if (GameWindow::s_pWindow)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, GameWindow::GetTitle(), Localise("Video_BackendError").c_str(), GameWindow::s_pWindow);
    }
    else
    {
        fprintf(stderr, "[Main] Video backend initialization failed (no window available for message box).\n");
        fflush(stderr);
    }
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

// Xenon memory initialization
#include <kernel/xenon_memory.h>

// Name inspired from nt's entry point
void KiSystemStartup()
{
    // Initialize main thread ID for SDL event safety
    InitKernelMainThread();
    
    if (g_memory.base == nullptr)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, GameWindow::GetTitle(), Localise("System_MemoryAllocationFailed").c_str(), GameWindow::s_pWindow);
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

    // Initialize save system early - creates directories and registers save content
    SaveSystem::Initialize();

    // Save file backwards compat — copy base save to modded save if missing
    const auto saveFilePath = GetSaveFilePath(true);
    if (!std::filesystem::exists(saveFilePath))
    {
        std::error_code ec;
        std::filesystem::create_directories(saveFilePath.parent_path(), ec);
        if (!ec)
            std::filesystem::copy_file(GetSaveFilePath(false), saveFilePath, ec);
    }

    // ---------------------------------------------------------------
    // REMOVED: Liberty's VFS, content registration, path cache, root
    // mappings.  RexGlue's VFS handles ALL file I/O now.
    // RexGlue mounts game: → \Device\Harddisk0\Partition1 and the
    // GTA IV symlinks (common:, platform:, audio:, etc.) are
    // registered in main() after rex::Runtime::Setup().
    // ---------------------------------------------------------------

    XAudioInitializeSystem();
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
        LOGN_WARNING("OS does not support registry.");

    os::logger::Init();

    PreloadContext preloadContext;
    preloadContext.PreloadExecutable();

    bool forceInstaller = false;
    bool forceDLCInstaller = false;
    bool useDefaultWorkingDirectory = false;
    bool forceInstallationCheck = false;
    bool graphicsApiRetry = false;
    const char *sdlVideoDriver = nullptr;

    for (uint32_t i = 1; i < argc; i++)
    {
        forceInstaller = forceInstaller || (strcmp(argv[i], "--install") == 0);
        forceDLCInstaller = forceDLCInstaller || (strcmp(argv[i], "--install-dlc") == 0);
        useDefaultWorkingDirectory = useDefaultWorkingDirectory || (strcmp(argv[i], "--use-cwd") == 0);
        forceInstallationCheck = forceInstallationCheck || (strcmp(argv[i], "--install-check") == 0);
        graphicsApiRetry = graphicsApiRetry || (strcmp(argv[i], "--graphics-api-retry") == 0);
        App::s_isSkipLogos = App::s_isSkipLogos || (strcmp(argv[i], "--skip-logos") == 0);

        if (strcmp(argv[i], "--sdl-video-driver") == 0)
        {
            if ((i + 1) < argc)
                sdlVideoDriver = argv[++i];
            else
                LOGN_WARNING("No argument was specified for --sdl-video-driver. Option will be ignored.");
        }
    }

    if (!useDefaultWorkingDirectory)
    {
        // Set the current working directory to the executable's path.
        std::error_code ec;
        std::filesystem::current_path(os::process::GetExecutablePath().parent_path(), ec);
    }

    Config::Load();

    // Initialize RexGlue logging before anything else that might use it.
    // Guest threads call kernel exports (RtlNtStatusToDosError etc.) that log
    // via REXKRNL_IMPORT_INFO; the lazy auto-init in GetLogger() has a
    // thread-safety issue on ARM64 (plain bool without memory barrier).
    rex::InitLogging();

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

    // Bridge extracted audio.rpf content to where the RAGE relative device expects it.
    // The packfile device at audio:/ has an AES-encrypted TOC that fails to decrypt in
    // the recompiled env. The fallback relative device uses root game:/xbox360/audio/,
    // but extracted config files live at audio/config/. Symlink bridges this gap.
    {
        auto linkPath   = rexContentRoot / "xbox360" / "audio" / "config";
        auto targetPath = std::filesystem::path("../../audio/config");
        std::error_code ec;
        if (!std::filesystem::exists(linkPath, ec)) {
            std::filesystem::create_directories(linkPath.parent_path(), ec);
            std::filesystem::create_directory_symlink(targetPath, linkPath, ec);
            if (!ec) {
                fprintf(stderr, "[Main] Created symlink: xbox360/audio/config -> ../../audio/config\n");
            } else {
                fprintf(stderr, "[Main] WARN: Could not create audio config symlink: %s\n", ec.message().c_str());
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
        uint32_t rt_status = s_rexRuntime->Setup(
            static_cast<uint32_t>(PPC_CODE_BASE),
            static_cast<uint32_t>(PPC_CODE_SIZE),
            static_cast<uint32_t>(PPC_IMAGE_BASE),
            static_cast<uint32_t>(PPC_IMAGE_SIZE),
            PPCFuncMappings);
        fprintf(stderr, "[Main] Setup() returned 0x%08X\n", rt_status);
        if (rt_status != 0 /* X_STATUS_SUCCESS */) {
            fprintf(stderr, "[Main] FATAL: rex::Runtime::Setup() failed with 0x%08X\n", rt_status);
            std::_Exit(1);
        }
        // SDK v0.2.1: set_instance() is automatic after Setup().
        // Get memory base from the Runtime's internally-managed Memory object.
        g_memory.base = s_rexRuntime->virtual_membase();
        fprintf(stderr, "[Main] RexGlue memory: base=%p\n", (void*)g_memory.base);
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

            printf("[Main] Registered GTA IV VFS symlinks: common:, platform:, audio:, xbox360:, x:, cache:, cache1:\n");
            fflush(stdout);
        }

        // Populate Processor function table for XThread::Execute().
        // Memory's function table was populated during InitializeFromRexGlue(),
        // but the Processor has its own hash map used by processor->GetFunction().
        // InitializeFunctionTable adopts the existing memory table (no double-init).
        auto* proc = s_rexRuntime->processor();
        proc->InitializeFunctionTable(
            static_cast<uint32_t>(PPC_CODE_BASE),
            static_cast<uint32_t>(PPC_CODE_SIZE),
            static_cast<uint32_t>(PPC_IMAGE_BASE),
            static_cast<uint32_t>(PPC_IMAGE_SIZE));
        {
            int funcCount = 0;
            for (size_t i = 0; PPCFuncMappings[i].guest != 0; i++) {
                if (PPCFuncMappings[i].host != nullptr) {
                    proc->SetFunction(
                        static_cast<uint32_t>(PPCFuncMappings[i].guest),
                        PPCFuncMappings[i].host);
                    funcCount++;
                }
            }
            fprintf(stderr, "[Main] Processor function table: %d functions registered\n",
                   funcCount);
        }
    }

    // Memory::Initialize() installed its own MMIOHandler internally.
    // Move fallback crash handler to end of chain so MMIOHandler runs first.
    rex::arch::ExceptionHandler::Uninstall(RexFallbackCrashHandler, nullptr);
    rex::arch::ExceptionHandler::Install(RexFallbackCrashHandler, nullptr);

    // Runtime::Setup() → guest::initialize() installed SEH's signal handler
    // on top of ExceptionHandler.  Re-install ExceptionHandler so it runs
    // first.  The chain becomes:
    //   ExceptionHandler (MMIO, NULL-PC, fallback) → SEH (SehException)
    // SDK v0.2.1: ReinstallSignalHandlers() removed; SDK manages handler chain internally.
    fprintf(stderr, "[RexGlue] Exception handlers active (SDK v0.2.1 automatic chain)\n");
    fflush(stderr);

    if (forceInstallationCheck)
    {
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
        uint32_t messageBoxStyle;
        if (journal.lastResult == Journal::Result::Success)
        {
            snprintf(resultText, sizeof(resultText), "%s", Localise("IntegrityCheck_Success").c_str());
            fprintf(stdout, "%s\n", resultText);
            messageBoxStyle = SDL_MESSAGEBOX_INFORMATION;
        }
        else
        {
            snprintf(resultText, sizeof(resultText), Localise("IntegrityCheck_Failed").c_str(), journal.lastErrorMessage.c_str());
            fprintf(stderr, "%s\n", resultText);
            messageBoxStyle = SDL_MESSAGEBOX_ERROR;
        }

        SDL_ShowSimpleMessageBox(messageBoxStyle, GameWindow::GetTitle(), resultText, GameWindow::s_pWindow);
        std::_Exit(int(journal.lastResult));
    }

#if defined(_WIN32) && defined(LIBERTY_RECOMP_D3D12)
    for (auto& dll : g_D3D12RequiredModules)
    {
        if (!std::filesystem::exists(g_executableRoot / dll))
        {
            char text[512];
            snprintf(text, sizeof(text), Localise("System_Win32_MissingDLLs").c_str(), dll.data());
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, GameWindow::GetTitle(), text, GameWindow::s_pWindow);
            std::_Exit(1);
        }
    }
#endif

    if (Config::ShowConsole)
        os::process::ShowConsole();
    LOGN_WARNING("Host Startup");
    HostStartup();

    std::filesystem::path modulePath;
    bool isGameInstalled = Installer::checkGameInstall(GetGamePath(), modulePath);
    bool runInstallerWizard = forceInstaller || forceDLCInstaller || !isGameInstalled;
    
    // TEMPORARY: Force installer UI to always show for preview
    // TODO: Remove this line after UI preview is done
    // runInstallerWizard = true;  // DISABLED - respect actual install check
    
     if (runInstallerWizard)
     {
         if (!Video::CreateHostDevice(sdlVideoDriver, graphicsApiRetry))
         {
             ShowVideoBackendErrorAndExit();
         }

         if (!InstallerWizard::Run(GetGamePath(), isGameInstalled && forceDLCInstaller))
         {
             std::_Exit(0);
         }
     }

    // Show main menu before starting the game
    // This provides a launcher-style experience like Banjo-Recompiled
    // TEMPORARY: Disabled main menu for debugging - set to false to skip
    bool showMainMenu = false; // TODO: Re-enable main menu after debugging
    for (uint32_t i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--skip-menu") == 0)
            showMainMenu = false;
        if (strcmp(argv[i], "--show-menu") == 0)
            showMainMenu = true;
    }
    
    if (showMainMenu)
    {
        // Create video device if not already created by installer
        if (!runInstallerWizard)
        {
            if (!Video::CreateHostDevice(sdlVideoDriver, graphicsApiRetry))
            {
                ShowVideoBackendErrorAndExit();
            }
        }
        
        MainMenu::Init();
        if (!MainMenu::Run())
        {
            // User selected Exit from main menu
            MainMenu::Shutdown();
            std::_Exit(0);
        }
        MainMenu::Shutdown();
    }

    // ModLoader::Init();

    // Create video device if not already created by installer or main menu
    if (!runInstallerWizard && !showMainMenu)
    {
        printf("[Main] Creating video device...\n"); fflush(stdout);
        if (!Video::CreateHostDevice(sdlVideoDriver, graphicsApiRetry))
        {
            ShowVideoBackendErrorAndExit();
        }
        printf("[Main] Video device created\n"); fflush(stdout);
    }

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

        auto gameDir = modulePath.parent_path();
        printf("[Main] Setting up RexGlue VFS: %s\n", gameDir.string().c_str()); fflush(stdout);

        auto device = std::make_unique<rex::filesystem::HostPathDevice>(
            "\\Device\\Harddisk0\\Partition1", gameDir, /*read_only=*/true);
        if (!device->Initialize()) {
            printf("[Main] FATAL: Failed to initialize VFS host device\n");
            fflush(stdout);
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
                if (diffCount <= 20) {
                    printf("[HeaderDiff] offset=0x%05zX rex=0x%02X hdr=0x%02X %s\n",
                           i, rexPE[i], xex_header_data[i],
                           inImportRange ? "(IMPORT)" : "(DATA)");
                }
            }
        }
        printf("[Main] Header comparison: %d diffs (%d import, %d data) out of %zu bytes\n",
               diffCount, importDiffs, dataDiffs, sizeof(xex_header_data));
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
        std::_Exit(99);
    });

    // ------------------------------------------------------------------
    // Launch the game via RexGlue's LaunchModule — creates the main
    // XThread with entry point, stack size, and TLS from the XEX header.
    // This is exactly how standalone RexGlue works.
    // ------------------------------------------------------------------
    LOGN_WARNING("Start Guest Thread");
    LOGN_WARNING(modulePath.string());

    auto* rt = rex::Runtime::instance();
    auto main_xthread = rt->LaunchModule();
    if (!main_xthread) {
        printf("[Main] FATAL: LaunchModule() returned null\n");
        fflush(stdout);
        std::_Exit(1);
    }
    printf("[Main] Main XThread launched via RexGlue (handle=0x%08X)\n",
           main_xthread->handle()); fflush(stdout);

    // Wait for the XThread to actually begin executing.
    for (int i = 0; i < 5000 && !main_xthread->is_running(); ++i) {
        SDL_Delay(1);
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
    while (main_xthread->is_running()) {
        SDL_PumpEvents();
        SDL_Delay(1);
    }

    printf("[Main] Main XThread finished\n");
    fflush(stdout);
    return 0;
}

// String function stubs removed — RexGlue (xboxkrnl_strings) handles these.
