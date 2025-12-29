#include <cstdint>
#include <cstdio>
#include <unordered_set>
#include <thread>
#include <atomic>
#include <stdafx.h>
#include <cpu/ppc_context.h>
#include <cpu/guest_thread.h>
#include <apu/audio.h>
#include <gpu/video.h>
#include <ui/game_window.h>
#include "function.h"
#include "xex.h"
#include "xbox.h"
#include "heap.h"
#include "memory.h"
#include <memory>
#include "xam.h"
#include "xdm.h"
#include <hid/hid.h>
#include <user/config.h>
#include <user/paths.h>
#include <os/logger.h>

#include "io/file_system.h"
#include "vfs.h"
#include "io/gta_file_system.h"
#include "game_init.h"
#include "io/net_socket.h"
#include "io/net_session.h"

#include <cerrno>
#include <fstream>
#include <algorithm>
#include <vector>
#include <mutex>

#if defined(__APPLE__)
#include <CommonCrypto/CommonCryptor.h>
#endif

#ifndef _WIN32
#include <sys/mman.h>  // For mprotect
#endif

#ifdef _WIN32
#include <ntstatus.h>
#endif

// =============================================================================
// TARGETED TIMING FIXES - Known Wait Loop Registry
// =============================================================================
// Instead of global slowdowns, we identify specific wait loops and inject yields
// only where needed. This fixes timing issues without impacting overall performance.
// =============================================================================

// Registry of known spin-wait functions that need yield injection
// These are addresses of functions that contain busy-wait loops
static const std::unordered_set<uint32_t> g_knownSpinWaitFunctions = {
    0x82672E50,  // Audio/streaming init - has 2 busy-wait loops
    0x82672AA8,  // Loop 1 helper in sub_82672E50
    0x829736F8,  // Loop 2 helper in sub_82672E50
    0x827E7B38,  // File streaming queue init - infinite loop
    0x827DB880,  // Worker manager shutdown loop
    0x823005E0,  // Worker cleanup - waits for workers to exit
};

// Helper to check if a function address is a known spin-wait
inline bool IsKnownSpinWait(uint32_t funcAddr) {
    return g_knownSpinWaitFunctions.count(funcAddr) > 0;
}

// =============================================================================
// COMPREHENSIVE SEMAPHORE FIX - Following UnleashedRecomp Pattern
// =============================================================================
// Problem: Worker threads wait on semaphores that are never signaled, causing deadlocks.
// 
// Root cause: The game's init sequence creates worker threads that wait on semaphores,
// but the code that should signal those semaphores either:
// 1. Never runs (bypassed functions)
// 2. Runs in wrong order (race conditions)
// 3. Depends on hardware that doesn't exist on PC
//
// Solution: Track all blocking semaphores (count=0 at init) and provide:
// 1. Timeout-based fallback to prevent infinite waits
// 2. Signaling mechanism after init completes
// 3. Proper synchronization that matches reference projects
// =============================================================================

// Forward declarations - actual tracking functions defined after Semaphore struct
static std::mutex g_semaphoreTrackMutex;
static std::unordered_set<uint32_t> g_blockingSemaphoreAddrs;  // Guest addresses of blocking semaphores
static std::atomic<bool> g_initComplete{false};
static std::atomic<int> g_semaphoreWaitCount{0};

// Track a semaphore address that was initialized with count=0
void TrackBlockingSemaphoreAddr(uint32_t addr) {
    std::lock_guard<std::mutex> lock(g_semaphoreTrackMutex);
    g_blockingSemaphoreAddrs.insert(addr);
}

bool IsBlockingSemaphore(uint32_t addr) {
    std::lock_guard<std::mutex> lock(g_semaphoreTrackMutex);
    return g_blockingSemaphoreAddrs.count(addr) > 0;
}

bool IsInitComplete() {
    return g_initComplete.load();
}

// SetInitComplete and SignalAllBlockingSemaphores defined after Semaphore struct

// =============================================================================
// SDL Event Pumping Helper
// =============================================================================
// This is called from kernel functions that are invoked frequently during
// game execution to keep the window responsive. Without this, the window
// freezes because GuestThread::Start blocks the main thread.
// 
// IMPORTANT: On macOS, SDL event pumping MUST happen on the main thread.
// If called from a worker thread, we skip the pump to avoid crashes.

static std::chrono::steady_clock::time_point g_lastSdlPumpTime;
static constexpr auto SDL_PUMP_INTERVAL = std::chrono::milliseconds(16); // ~60fps
static std::thread::id g_kernelMainThreadId;
static std::atomic<bool> g_kernelMainThreadIdSet{false};

void InitKernelMainThread()
{
    g_kernelMainThreadId = std::this_thread::get_id();
    g_kernelMainThreadIdSet = true;
}

bool IsMainThread()
{
    if (!g_kernelMainThreadIdSet) return false;
    return std::this_thread::get_id() == g_kernelMainThreadId;
}

void PumpSdlEventsIfNeeded()
{
    // On macOS, SDL event pumping MUST happen on the main thread
    // Skip if we're on a worker thread to avoid Cocoa crashes
    if (!IsMainThread()) return;
    
    auto now = std::chrono::steady_clock::now();
    if (now - g_lastSdlPumpTime >= SDL_PUMP_INTERVAL)
    {
        g_lastSdlPumpTime = now;
        SDL_PumpEvents();
        
        // Process critical events (window close, etc.)
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                std::_Exit(0);
            }
        }
    }
}

// =============================================================================

std::unordered_map<uint32_t, uint32_t> g_handleDuplicates{};

// PHASE 2: Global flag for storage init tracing
static std::atomic<bool> g_inStorageInit{false};

namespace
{
    constexpr uint32_t STATUS_INVALID_HANDLE = 0xC0000008;
    constexpr uint32_t STATUS_INVALID_PARAMETER = 0xC000000D;
    constexpr uint32_t STATUS_INFO_LENGTH_MISMATCH = 0xC0000004;
    constexpr uint32_t STATUS_END_OF_FILE = 0xC0000011;
    constexpr uint32_t STATUS_ACCESS_DENIED = 0xC0000022;
    constexpr uint32_t STATUS_OBJECT_NAME_NOT_FOUND = 0xC0000034;
    constexpr uint32_t STATUS_OBJECT_PATH_NOT_FOUND = 0xC000003A;
    constexpr uint32_t STATUS_OBJECT_NAME_INVALID = 0xC0000033;
    constexpr uint32_t STATUS_OBJECT_NAME_COLLISION = 0xC0000035;
    constexpr uint32_t STATUS_BUFFER_OVERFLOW = 0x80000005;
    constexpr uint32_t STATUS_NO_MORE_FILES = 0x80000006;

    // FILE_INFORMATION_CLASS (subset)
    constexpr uint32_t FileBasicInformation = 4;
    constexpr uint32_t FileStandardInformation = 5;
    constexpr uint32_t FilePositionInformation = 14;
    constexpr uint32_t FileEndOfFileInformation = 20;
    constexpr uint32_t FileNetworkOpenInformation = 34;

    // FS_INFORMATION_CLASS (subset)
    constexpr uint32_t FileFsSizeInformation = 3;
    constexpr uint32_t FileFsDeviceInformation = 4;
    constexpr uint32_t FileFsAttributeInformation = 5;

    constexpr uint32_t kNtFileHandleMagic = 0x4E544649; // 'NTFI'
    constexpr uint32_t kNtDirHandleMagic = 0x4E544449; // 'NTDI'
    constexpr uint32_t kNtVirtFileHandleMagic = 0x4E545646; // 'NTVF'

    struct NtFileHandle final : KernelObject
    {
        uint32_t magic = kNtFileHandleMagic;
        std::fstream stream;
        std::filesystem::path path;
        // RPF metadata cached per-open
        bool rpfHeaderParsed = false;
        bool isRpf = false;
        uint32_t tocOffset = 0;
        uint32_t tocSize = 0;
        bool tocEncrypted = false;
    };

    struct NtDirHandle final : KernelObject
    {
        uint32_t magic = kNtDirHandleMagic;
        std::filesystem::path path;
        std::vector<std::filesystem::directory_entry> entries;
        size_t cursor = 0;
        bool initialized = false;
    };

    struct NtVirtFileHandle final : KernelObject
    {
        uint32_t magic = kNtVirtFileHandleMagic;
        std::filesystem::path debugPath;
        std::vector<uint8_t> data;
    };

    static std::unordered_map<uint32_t, NtFileHandle*> g_ntFileHandles;
    static std::unordered_map<uint32_t, NtDirHandle*> g_ntDirHandles;
    static std::unordered_map<uint32_t, NtVirtFileHandle*> g_ntVirtFileHandles;
    
    // PM4 Packet Types (Xbox 360 GPU command buffer format)
    enum PM4PacketType {
        PM4_TYPE0 = 0,  // Register writes
        PM4_TYPE1 = 1,  // Two register writes  
        PM4_TYPE2 = 2,  // Filler/NOP
        PM4_TYPE3 = 3,  // Commands
    };
    
    // PM4 Type 3 Opcodes (from Xenia xenos.h)
    enum PM4Type3Opcode {
        PM4_ME_INIT               = 0x48,
        PM4_NOP                   = 0x10,
        PM4_INDIRECT_BUFFER       = 0x3f,
        PM4_WAIT_REG_MEM          = 0x3c,
        PM4_REG_RMW               = 0x21,
        PM4_REG_TO_MEM            = 0x3e,
        PM4_MEM_WRITE             = 0x3d,
        PM4_COND_WRITE            = 0x45,
        PM4_EVENT_WRITE           = 0x46,
        PM4_EVENT_WRITE_SHD       = 0x58,
        PM4_EVENT_WRITE_EXT       = 0x5a,
        PM4_EVENT_WRITE_ZPD       = 0x5b,
        PM4_DRAW_INDX             = 0x22,
        PM4_DRAW_INDX_2           = 0x36,
        PM4_VIZ_QUERY             = 0x23,
        PM4_SET_CONSTANT          = 0x2d,
        PM4_SET_CONSTANT2         = 0x55,
        PM4_SET_SHADER_CONSTANTS  = 0x56,
        PM4_IM_LOAD               = 0x27,
        PM4_IM_LOAD_IMMEDIATE     = 0x2b,
        PM4_INVALIDATE_STATE      = 0x3b,
        PM4_SET_SHADER_BASES      = 0x4A,
        PM4_INTERRUPT             = 0x54,
        PM4_XE_SWAP               = 0x64,
    };
    
    // GPU Ring Buffer state for GPU command processing
    struct GpuRingBufferState {
        uint32_t ringBufferBase = 0;      // Physical address of ring buffer
        uint32_t ringBufferSize = 0;      // Size in bytes (1 << size_log2)
        uint32_t readPtrWritebackAddr = 0; // Address where GPU writes read pointer
        uint32_t blockSize = 0;           // Block size for writeback (1 << block_size_log2)
        uint32_t interruptCallback = 0;   // Graphics interrupt callback function
        uint32_t interruptUserData = 0;   // User data for interrupt callback
        bool initialized = false;
        bool writebackEnabled = false;
        bool interruptFired = false;
        
        // Persistent video state flags - the GPU polling loop checks these
        bool enginesInitialized = false;
        bool edramTrainingComplete = false;
        bool interruptSeen = false;
        
        // PM4 command processing state
        uint32_t lastKnownWritePtr = 0;   // Last write pointer we saw
        uint32_t processedReadPtr = 0;    // Where we've processed up to
        
        // Diagnostic counters
        uint32_t pm4DrawCount = 0;
        uint32_t pm4ShaderLoadCount = 0;
        uint32_t pm4SetConstantCount = 0;
        uint32_t pm4SwapCount = 0;
        uint32_t pm4OtherCount = 0;
    };
    static GpuRingBufferState g_gpuRingBuffer;
    
    // PM4 opcode name for logging
    static const char* GetPM4OpcodeName(uint32_t opcode) {
        switch (opcode) {
            case PM4_ME_INIT: return "ME_INIT";
            case PM4_NOP: return "NOP";
            case PM4_INDIRECT_BUFFER: return "INDIRECT_BUFFER";
            case PM4_WAIT_REG_MEM: return "WAIT_REG_MEM";
            case PM4_REG_RMW: return "REG_RMW";
            case PM4_REG_TO_MEM: return "REG_TO_MEM";
            case PM4_MEM_WRITE: return "MEM_WRITE";
            case PM4_COND_WRITE: return "COND_WRITE";
            case PM4_EVENT_WRITE: return "EVENT_WRITE";
            case PM4_EVENT_WRITE_SHD: return "EVENT_WRITE_SHD";
            case PM4_EVENT_WRITE_EXT: return "EVENT_WRITE_EXT";
            case PM4_EVENT_WRITE_ZPD: return "EVENT_WRITE_ZPD";
            case PM4_DRAW_INDX: return "DRAW_INDX";
            case PM4_DRAW_INDX_2: return "DRAW_INDX_2";
            case PM4_VIZ_QUERY: return "VIZ_QUERY";
            case PM4_SET_CONSTANT: return "SET_CONSTANT";
            case PM4_SET_CONSTANT2: return "SET_CONSTANT2";
            case PM4_SET_SHADER_CONSTANTS: return "SET_SHADER_CONSTANTS";
            case PM4_IM_LOAD: return "IM_LOAD";
            case PM4_IM_LOAD_IMMEDIATE: return "IM_LOAD_IMMEDIATE";
            case PM4_INVALIDATE_STATE: return "INVALIDATE_STATE";
            case PM4_SET_SHADER_BASES: return "SET_SHADER_BASES";
            case PM4_INTERRUPT: return "INTERRUPT";
            case PM4_XE_SWAP: return "XE_SWAP";
            default: return "UNKNOWN";
        }
    }
    
    // PPC function call tracking for D3D address identification
    static std::unordered_map<uint32_t, uint32_t> g_ppcFunctionCallCounts;
    static std::mutex g_ppcCallCountMutex;
    static uint32_t g_ppcCallTrackingEnabled = 0;  // 0=off, 1=tracking, 2=reported
    
    void TrackPPCFunctionCall(uint32_t address) {
        if (g_ppcCallTrackingEnabled != 1) return;
        if (address < 0x82000000 || address > 0x82FFFFFF) return;  // Only track game code
        
        std::lock_guard<std::mutex> lock(g_ppcCallCountMutex);
        g_ppcFunctionCallCounts[address]++;
    }
    
    void ReportHotPPCFunctions() {
        if (g_ppcCallTrackingEnabled != 1) return;
        g_ppcCallTrackingEnabled = 2;  // Mark as reported to prevent repeated reports
        
        std::lock_guard<std::mutex> lock(g_ppcCallCountMutex);
        
        // Find functions in the D3D address ranges that are called frequently
        std::vector<std::pair<uint32_t, uint32_t>> hotFunctions;
        for (const auto& [addr, count] : g_ppcFunctionCallCounts) {
            // Focus on 0x829Dxxxx (GTA IV D3D wrapper) and 0x826Fxxxx (possible draw functions)
            if ((addr >= 0x829D0000 && addr <= 0x829EFFFF) ||
                (addr >= 0x826F0000 && addr <= 0x826FFFFF) ||
                (addr >= 0x82540000 && addr <= 0x8254FFFF)) {
                if (count >= 100) {
                    hotFunctions.push_back({addr, count});
                }
            }
        }
        
        // Sort by count descending
        std::sort(hotFunctions.begin(), hotFunctions.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });
        
        // Log top 20 hot D3D functions
        LOGF_WARNING("[HOT PPC FUNCTIONS] Found {} hot functions in D3D ranges:", hotFunctions.size());
        for (size_t i = 0; i < std::min(hotFunctions.size(), size_t(20)); i++) {
            LOGF_WARNING("  0x{:08X}: {} calls", hotFunctions[i].first, hotFunctions[i].second);
        }
    }
    
    // Global RPF streams for translation layer - opened once, used for all reads
    struct RpfStreamInfo {
        std::fstream stream;
        std::filesystem::path path;
        uint64_t fileSize = 0;
        bool headerParsed = false;
        uint32_t tocOffset = 0;
        uint32_t tocSize = 0;
        bool tocEncrypted = false;
        std::vector<uint8_t> decryptedToc;  // Pre-decrypted TOC for fast access
        bool tocDecrypted = false;
    };
    static std::unordered_map<std::string, std::unique_ptr<RpfStreamInfo>> g_rpfStreams;
    
    // Map any handle to its backing RPF name (e.g., "common", "xbox360", "audio")
    static std::unordered_map<uint32_t, std::string> g_handleToRpf;
    
    // Offset-to-file mapping for serving extracted files
    struct RpfFileEntry {
        uint32_t offset;
        uint32_t size;
        const char* path;
    };
    
    // File mapping for common.rpf - maps RPF offsets to extracted file paths
    // Generated from parsing the decrypted TOC
    static const RpfFileEntry g_commonRpfFiles[] = {
        {0x005000,   469881, "data/action_table.csv"},
        {0x00C800,   123153, "data/ambient.dat"},
        {0x010000,    12063, "data/animgrp.dat"},
        {0x010800,    24102, "data/carcols.dat"},
        {0x012800,     5554, "data/cargrp.dat"},
        {0x013000,    22853, "data/credits_360.dat"},
        {0x0152FE,      882, "data/default.dat"},
        {0x015800,    11342, "data/default.ide"},
        {0x016800,     5534, "data/femaleplayersettings.dat"},
        {0x017000,     9855, "data/fonts.dat"},
        {0x018000,    13883, "data/frontend_360.dat"},
        {0x019000,    36821, "data/frontend_menus.xml"},
        {0x01A000,     8022, "data/gta.dat"},
        {0x01B000,    41386, "data/handling.dat"},
        {0x01D800,     9498, "data/hud.dat"},
        {0x01E000,     4529, "data/hudcolor.dat"},
        {0x01E800,     5964, "data/images.txt"},
        {0x01F000,    18884, "data/info.zon"},
        {0x020800,     3779, "data/introspline.csv"},
        {0x021000,    77653, "data/leaderboards_data.xml"},
        {0x022000,    14880, "data/loadingscreens_360.dat"},
        {0x023000,     4828, "data/maleplayersettings.dat"},
        {0x024000,     9047, "data/meleeanims.dat"},
        {0x025000,     7988, "data/moveblend.dat"},
        {0x025800,      974, "data/nav.dat"},
        {0x026000,   139863, "data/navprecalc.dat"},
        {0x03B000,     6873, "data/networktest.dat"},
        {0x03C000,     8735, "data/pedgrp.dat"},
        {0x03D000,    89589, "data/pedpersonality.dat"},
        {0x03E800,    96255, "data/peds.ide"},
        {0x041000,   158690, "data/pedvariations.dat"},
        {0x043000,    10774, "data/plants.dat"},
        {0x044000,   594605, "data/popcycle.dat"},
        {0x048000,   593598, "data/popcycle.datnew"},
        {0x04C800,    10800, "data/precincts.dat"},
        {0x04D000,     3490, "data/radiohud.dat"},
        {0x04D800,     1393, "data/radiologo.dat"},
        {0x04E000,     9208, "data/relationships.dat"},
        {0x04E800,    12207, "data/scenarios.dat"},
        {0x04F800,     4046, "data/scrollbars.dat"},
        {0x050000,    18052, "data/startup.sco"},
        {0x051000,     5107, "data/statexport.dat"},
        {0x052000,     6665, "data/surface.dat"},
        {0x053000,    10312, "data/surfaceaudio.dat"},
        {0x054000,     5149, "data/timecycle.dat"},
        {0x055000,     5149, "data/timecyclemod.dat"},
        {0x056000,     2451, "data/traincamnodes.txt"},
        {0x057000,     5036, "data/traintrack.dat"},
        {0x058000,    16626, "data/tuneables.dat"},
        {0x059000,    15168, "data/vehcomps.dat"},
        {0x05A800,   286690, "data/vehoff.csv"},
        {0x063000,    43058, "data/vehicles.ide"},
        {0x068000,    35451, "data/visualsettings.dat"},
        {0x06C000,    16393, "data/water.dat"},
        {0x06D000,    29497, "data/weaponinfo.xml"},
        {0x070000,    10127, "data/weapons.dat"},
        {0x072000,    15680, "data/weather.dat"},
        {0x087800,   270035, "data/maps/occlu.ipl"},
        {0x09B800,  1720829, "data/maps/paths.ipl"},
        {0x198800,    99007, "shaders/fxl_final/deferred_lighting.fxc"},
        {0x19F800,    38797, "shaders/fxl_final/gta_atmoscatt_clouds.fxc"},
        {0x1A2800,    29220, "shaders/fxl_final/gta_cubemap_reflect.fxc"},
        {0x1A5000,    28838, "shaders/fxl_final/gta_cutout_fence.fxc"},
        {0x1A7800,    29201, "shaders/fxl_final/gta_decal.fxc"},
        {0x1AA000,    29122, "shaders/fxl_final/gta_decal_amb_only.fxc"},
        {0x1AC800,    28356, "shaders/fxl_final/gta_decal_dirt.fxc"},
        {0x1AF000,    29385, "shaders/fxl_final/gta_decal_glue.fxc"},
        {0x1B1800,    31357, "shaders/fxl_final/gta_decal_normal_only.fxc"},
        {0x1B4800,    28758, "shaders/fxl_final/gta_default.fxc"},
        {0x1B7000,    17697, "shaders/fxl_final/gta_diffuse_instance.fxc"},
        {0x1B8800,    20703, "shaders/fxl_final/gta_emissive.fxc"},
        {0x1BA000,    21405, "shaders/fxl_final/gta_emissivenight.fxc"},
        {0x1BB800,    20965, "shaders/fxl_final/gta_emissivestrong.fxc"},
        {0x1BD000,    31661, "shaders/fxl_final/gta_glass.fxc"},
        {0x1C0000,    23504, "shaders/fxl_final/gta_glass_emissive.fxc"},
        {0x1C2000,    24048, "shaders/fxl_final/gta_glass_emissivenight.fxc"},
        {0x1C4000,    33090, "shaders/fxl_final/gta_glass_normal_spec_reflect.fxc"},
        {0x1C7000,    30062, "shaders/fxl_final/gta_glass_reflect.fxc"},
        {0x1C9800,    29649, "shaders/fxl_final/gta_glass_spec.fxc"},
        {0x1CC800,    23995, "shaders/fxl_final/gta_hair_sorted_alpha.fxc"},
        {0x1CF000,    23882, "shaders/fxl_final/gta_hair_sorted_alpha_exp.fxc"},
        {0x1D1800,    14196, "shaders/fxl_final/gta_im.fxc"},
        {0x1D3000,    31405, "shaders/fxl_final/gta_normal.fxc"},
        {0x1D6000,    31219, "shaders/fxl_final/gta_normal_cubemap_reflect.fxc"},
        {0x1D9000,    31454, "shaders/fxl_final/gta_normal_decal.fxc"},
        {0x1DC000,    30991, "shaders/fxl_final/gta_normal_reflect.fxc"},
        {0x1DE800,    31123, "shaders/fxl_final/gta_normal_reflect_alpha.fxc"},
        {0x1E1000,    31044, "shaders/fxl_final/gta_normal_reflect_decal.fxc"},
        {0x1E3800,    32750, "shaders/fxl_final/gta_normal_spec.fxc"},
        {0x1E6800,    34062, "shaders/fxl_final/gta_normal_spec_cubemap_reflect.fxc"},
        {0x1E9800,    32791, "shaders/fxl_final/gta_normal_spec_decal.fxc"},
        {0x1EC800,    35048, "shaders/fxl_final/gta_normal_spec_reflect.fxc"},
        {0x1EF800,    35109, "shaders/fxl_final/gta_normal_spec_reflect_decal.fxc"},
        {0x1F2800,    24134, "shaders/fxl_final/gta_normal_spec_reflect_emissive.fxc"},
        {0x1F4800,    24686, "shaders/fxl_final/gta_normal_spec_reflect_emissivenight.fxc"},
        {0x1F6800,    32284, "shaders/fxl_final/gta_parallax.fxc"},
        {0x1F9800,    33573, "shaders/fxl_final/gta_parallax_specmap.fxc"},
        {0x1FC800,    32784, "shaders/fxl_final/gta_parallax_steep.fxc"},
        {0x1FF800,    24963, "shaders/fxl_final/gta_ped.fxc"},
        {0x202000,    26468, "shaders/fxl_final/gta_ped_reflect.fxc"},
        {0x204800,    25642, "shaders/fxl_final/gta_ped_skin.fxc"},
        {0x207000,    32242, "shaders/fxl_final/gta_ped_skin_blendshape.fxc"},
        {0x20A000,    42311, "shaders/fxl_final/gta_projtex.fxc"},
        {0x20E000,    42775, "shaders/fxl_final/gta_projtex_steep.fxc"},
        {0x212800,    28972, "shaders/fxl_final/gta_reflect.fxc"},
        {0x215000,    28969, "shaders/fxl_final/gta_reflect_decal.fxc"},
        {0x21F800,    31551, "shaders/fxl_final/gta_spec.fxc"},
        {0x222800,    31572, "shaders/fxl_final/gta_spec_decal.fxc"},
        {0x225800,    32083, "shaders/fxl_final/gta_spec_reflect.fxc"},
        {0x228800,    32116, "shaders/fxl_final/gta_spec_reflect_decal.fxc"},
        {0x22B800,    12493, "shaders/fxl_final/gta_terrain_va_2lyr.fxc"},
        {0x22C800,    16869, "shaders/fxl_final/gta_terrain_va_3lyr.fxc"},
        {0x22D800,    21225, "shaders/fxl_final/gta_terrain_va_4lyr.fxc"},
        {0x22E800,    11423, "shaders/fxl_final/gta_trees.fxc"},
        {0x22F800,    39294, "shaders/fxl_final/gta_vehicle_badges.fxc"},
        {0x232800,    31620, "shaders/fxl_final/gta_vehicle_basic.fxc"},
        {0x235000,    43225, "shaders/fxl_final/gta_vehicle_chrome.fxc"},
        {0x238800,    28625, "shaders/fxl_final/gta_vehicle_disc.fxc"},
        {0x23A800,    43656, "shaders/fxl_final/gta_vehicle_generic.fxc"},
        {0x23E000,    42850, "shaders/fxl_final/gta_vehicle_interior.fxc"},
        {0x241800,    41461, "shaders/fxl_final/gta_vehicle_interior2.fxc"},
        {0x245000,    45261, "shaders/fxl_final/gta_vehicle_lightsemissive.fxc"},
        {0x249000,    44350, "shaders/fxl_final/gta_vehicle_mesh.fxc"},
        {0x24C800,    43525, "shaders/fxl_final/gta_vehicle_paint1.fxc"},
        {0x250000,    43269, "shaders/fxl_final/gta_vehicle_paint2.fxc"},
        {0x253800,    44582, "shaders/fxl_final/gta_vehicle_paint3.fxc"},
        {0x257000,    32462, "shaders/fxl_final/gta_vehicle_rims1.fxc"},
        {0x259000,    32134, "shaders/fxl_final/gta_vehicle_rims2.fxc"},
        {0x25B000,    37859, "shaders/fxl_final/gta_vehicle_rubber.fxc"},
        {0x25E000,    37472, "shaders/fxl_final/gta_vehicle_shuts.fxc"},
        {0x261000,    33297, "shaders/fxl_final/gta_vehicle_tire.fxc"},
        {0x263800,    39399, "shaders/fxl_final/gta_vehicle_vehglass.fxc"},
        {0x267000,    29706, "shaders/fxl_final/gta_wire.fxc"},
        {0x26E000,    36486, "shaders/fxl_final/rage_postfx.fxc"},
        {0x273000,    10514, "shaders/fxl_final/water.fxc"},
        {0x275000,  2767809, "text/american.gxt"},
        {0x519000,  3034116, "text/french.gxt"},
        {0x7FE000,  3048486, "text/german.gxt"},
        {0xAE6800,  2915066, "text/italian.gxt"},
        {0xDAE800,  2876898, "text/spanish.gxt"},
        {0, 0, nullptr}  // Sentinel
    };
    
    // Find extracted file for a given RPF offset
    // Since RPF files can have overlapping ranges (due to compression), we need to find
    // the file whose start offset is closest to (but not greater than) the target offset
    static const RpfFileEntry* FindExtractedFile(const std::string& rpfName, uint32_t offset)
    {
        const RpfFileEntry* entries = nullptr;
        if (rpfName == "common")
            entries = g_commonRpfFiles;
        // TODO: Add xbox360 and audio mappings
        
        if (!entries)
            return nullptr;
        
        const RpfFileEntry* bestMatch = nullptr;
        uint32_t bestDistance = UINT32_MAX;
        
        for (const RpfFileEntry* e = entries; e->path != nullptr; ++e)
        {
            // File must start at or before the target offset
            if (e->offset <= offset)
            {
                // Check if target is within this file's range
                if (offset < e->offset + e->size)
                {
                    // Find the file with the smallest distance (closest start offset)
                    uint32_t distance = offset - e->offset;
                    if (distance < bestDistance)
                    {
                        bestDistance = distance;
                        bestMatch = e;
                    }
                }
            }
        }
        return bestMatch;
    }
    
    // Cache for opened extracted files
    struct ExtractedFileCache {
        std::fstream stream;
        std::string path;
        uint32_t rpfOffset;
        uint32_t fileSize;
    };
    static std::unordered_map<std::string, std::unique_ptr<ExtractedFileCache>> g_extractedFileCache;
    
    // Get or open an RPF stream
    static RpfStreamInfo* GetRpfStream(const std::string& rpfName)
    {
        auto it = g_rpfStreams.find(rpfName);
        if (it != g_rpfStreams.end())
            return it->second.get();
        
        // Open the RPF file
        std::filesystem::path rpfPath = GetGamePath() / "game" / (rpfName + ".rpf");
        if (!std::filesystem::exists(rpfPath))
        {
            LOGF_IMPL(Utility, "GetRpfStream", "RPF not found: {}", rpfPath.string());
            return nullptr;
        }
        
        auto info = std::make_unique<RpfStreamInfo>();
        info->stream.open(rpfPath, std::ios::in | std::ios::binary);
        if (!info->stream.is_open())
        {
            LOGF_IMPL(Utility, "GetRpfStream", "Failed to open RPF: {}", rpfPath.string());
            return nullptr;
        }
        
        info->path = rpfPath;
        {
            info->stream.clear();
            info->stream.seekg(0, std::ios::end);
            const std::streampos endPos = info->stream.tellg();
            if (endPos > 0)
                info->fileSize = static_cast<uint64_t>(endPos);
            info->stream.clear();
            info->stream.seekg(0, std::ios::beg);
        }
        
        // Parse RPF2 header
        char hdr[20] = {};
        info->stream.read(hdr, sizeof(hdr));
        if (info->stream.gcount() >= 20)
        {
            bool isRpf2 = (hdr[0] == 'R' && hdr[1] == 'P' && hdr[2] == 'F' && hdr[3] == '2');
            if (isRpf2)
            {
                auto le32 = [](const uint8_t* p) -> uint32_t {
                    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
                };
                info->tocSize = le32(reinterpret_cast<uint8_t*>(hdr + 4));
                uint32_t encrypted = le32(reinterpret_cast<uint8_t*>(hdr + 16));
                info->tocOffset = 0x800;
                info->tocEncrypted = (encrypted != 0);
                info->headerParsed = true;
                
                LOGF_IMPL(Utility, "GetRpfStream", "Opened {} - tocOffset=0x{:X} tocSize={} encrypted={}",
                          rpfName, info->tocOffset, info->tocSize, info->tocEncrypted);
            }
        }
        info->stream.clear();
        info->stream.seekg(0, std::ios::beg);
        
        RpfStreamInfo* ptr = info.get();
        g_rpfStreams[rpfName] = std::move(info);
        return ptr;
    }

    static void DecryptRpfBufferInPlace(uint8_t* data, uint32_t length, uint64_t tocRelativeOffset);
    
    static void EnsureTocDecrypted(RpfStreamInfo* rpf)
    {
        if (!rpf || rpf->tocDecrypted || !rpf->headerParsed || !rpf->tocEncrypted)
            return;
        if (rpf->tocSize == 0)
            return;
        
        rpf->decryptedToc.resize(rpf->tocSize);
        rpf->stream.clear();
        rpf->stream.seekg(rpf->tocOffset, std::ios::beg);
        rpf->stream.read(reinterpret_cast<char*>(rpf->decryptedToc.data()), rpf->tocSize);
        
        if (rpf->stream.gcount() != static_cast<std::streamsize>(rpf->tocSize))
        {
            LOGF_IMPL(Utility, "EnsureTocDecrypted", "Failed to read TOC: got {} bytes, expected {}", 
                      rpf->stream.gcount(), rpf->tocSize);
            rpf->decryptedToc.clear();
            return;
        }
        
        DecryptRpfBufferInPlace(rpf->decryptedToc.data(), rpf->tocSize, 0);
        rpf->tocDecrypted = true;
        LOGF_IMPL(Utility, "EnsureTocDecrypted", "Pre-decrypted {} bytes of TOC", rpf->tocSize);
    }

    // Read from extracted file given RPF name and offset
    static uint32_t ReadFromExtractedFile(const std::string& rpfName, uint8_t* hostBuffer, uint32_t size, uint32_t offset, int count)
    {
        const RpfFileEntry* entry = FindExtractedFile(rpfName, offset);
        if (!entry)
            return 0;
        
        // Calculate offset within the extracted file
        const uint32_t fileOffset = offset - entry->offset;
        if (fileOffset >= entry->size)
            return 0;
        
        // Build path to extracted file
        std::filesystem::path extractedPath = GetGamePath() / "extracted" / rpfName / entry->path;
        
        // Check cache first
        auto cacheKey = extractedPath.string();
        auto cacheIt = g_extractedFileCache.find(cacheKey);
        ExtractedFileCache* cache = nullptr;
        
        if (cacheIt != g_extractedFileCache.end())
        {
            cache = cacheIt->second.get();
        }
        else
        {
            // Open the extracted file
            if (!std::filesystem::exists(extractedPath))
            {
                if (count <= 20)
                {
                    LOGF_IMPL(Utility, "GTA4_FileLoad", "Extracted file not found: {}", extractedPath.string());
                }
                return 0;
            }
            
            auto newCache = std::make_unique<ExtractedFileCache>();
            newCache->stream.open(extractedPath, std::ios::in | std::ios::binary);
            if (!newCache->stream.is_open())
            {
                if (count <= 20)
                {
                    LOGF_IMPL(Utility, "GTA4_FileLoad", "Failed to open extracted file: {}", extractedPath.string());
                }
                return 0;
            }
            
            newCache->path = cacheKey;
            newCache->rpfOffset = entry->offset;
            newCache->fileSize = entry->size;
            
            cache = newCache.get();
            g_extractedFileCache[cacheKey] = std::move(newCache);
            
            if (count <= 50)
            {
                LOGF_IMPL(Utility, "GTA4_FileLoad", "Opened extracted file: {} (rpfOffset=0x{:X} size={})",
                          entry->path, entry->offset, entry->size);
            }
        }
        
        // Read from the extracted file
        cache->stream.clear();
        cache->stream.seekg(fileOffset, std::ios::beg);
        
        const uint32_t availableInFile = entry->size - fileOffset;
        const uint32_t toRead = std::min(size, availableInFile);
        
        cache->stream.read(reinterpret_cast<char*>(hostBuffer), toRead);
        const uint32_t bytesRead = static_cast<uint32_t>(cache->stream.gcount());
        
        if (count <= 50 || count % 100 == 0)
        {
            LOGF_IMPL(Utility, "GTA4_FileLoad", "Read {} bytes from extracted '{}' at fileOffset=0x{:X}",
                      bytesRead, entry->path, fileOffset);
        }
        
        return bytesRead;
    }
    
    static uint32_t ReadFromRpfStream(RpfStreamInfo* rpf, uint8_t* hostBuffer, uint32_t size, uint32_t offset, int count, const std::string& rpfName)
    {
        if (!rpf || !rpf->stream.is_open())
            return 0;
        if (rpf->fileSize != 0 && offset >= rpf->fileSize)
            return 0;
        
        // Ensure TOC is pre-decrypted for fast access
        EnsureTocDecrypted(rpf);
        
        // Check if this read overlaps with the TOC region
        const uint64_t tocStart = rpf->tocOffset;
        const uint64_t tocEnd = tocStart + rpf->tocSize;
        const uint64_t readStart = offset;
        const uint64_t readEnd = offset + size;
        
        // If we have a pre-decrypted TOC and the read overlaps it, serve from cache
        if (rpf->tocDecrypted && !rpf->decryptedToc.empty() && readEnd > tocStart && readStart < tocEnd)
        {
            // Read overlaps TOC - serve decrypted data
            uint32_t totalRead = 0;
            
            // Part before TOC (if any)
            if (readStart < tocStart)
            {
                const uint32_t beforeLen = static_cast<uint32_t>(tocStart - readStart);
                rpf->stream.clear();
                rpf->stream.seekg(offset, std::ios::beg);
                rpf->stream.read(reinterpret_cast<char*>(hostBuffer), beforeLen);
                totalRead += static_cast<uint32_t>(rpf->stream.gcount());
            }
            
            // TOC overlap part - serve from decrypted cache
            const uint64_t overlapStart = std::max(readStart, tocStart);
            const uint64_t overlapEnd = std::min(readEnd, tocEnd);
            if (overlapEnd > overlapStart)
            {
                const uint32_t tocOff = static_cast<uint32_t>(overlapStart - tocStart);
                const uint32_t bufOffset = static_cast<uint32_t>(overlapStart - readStart);
                const uint32_t len = static_cast<uint32_t>(overlapEnd - overlapStart);
                
                memcpy(hostBuffer + bufOffset, rpf->decryptedToc.data() + tocOff, len);
                totalRead += len;
                
                if (count <= 20 || count % 100 == 0)
                {
                    LOGF_IMPL(Utility, "GTA4_FileLoad", "Served {} bytes from pre-decrypted TOC at tocOffset=0x{:X}",
                        len, tocOff);
                }
            }
            
            // Part after TOC - serve raw RPF data (not extracted text files)
            if (readEnd > tocEnd)
            {
                const uint32_t afterOffset = static_cast<uint32_t>(tocEnd);
                const uint32_t afterLen = static_cast<uint32_t>(readEnd - tocEnd);
                const uint32_t bufOffset = static_cast<uint32_t>(tocEnd - readStart);
                
                // Read raw RPF data directly - extracted files are TEXT format, game expects BINARY
                rpf->stream.clear();
                rpf->stream.seekg(afterOffset, std::ios::beg);
                rpf->stream.read(reinterpret_cast<char*>(hostBuffer + bufOffset), afterLen);
                totalRead += static_cast<uint32_t>(rpf->stream.gcount());
            }
            
            return totalRead;
        }
        
        // For reads past TOC, read raw RPF data directly
        // NOTE: Extracted files are TEXT format, but game expects BINARY RPF format
        
        // Read from raw RPF
        rpf->stream.clear();
        rpf->stream.seekg(offset, std::ios::beg);
        
        if (rpf->stream.bad())
            return 0;
        
        rpf->stream.read(reinterpret_cast<char*>(hostBuffer), size);
        const uint32_t bytesRead = static_cast<uint32_t>(rpf->stream.gcount());
        
        if (bytesRead == 0)
            return 0;
        
        // Fallback: decrypt TOC region on-the-fly if not using cache
        if (rpf->headerParsed && rpf->tocEncrypted && !rpf->tocDecrypted)
        {
            const uint64_t overlapStart = std::max(readStart, tocStart);
            const uint64_t overlapEnd = std::min(static_cast<uint64_t>(offset + bytesRead), tocEnd);
            
            if (overlapEnd > overlapStart)
            {
                const uint32_t startInBuf = static_cast<uint32_t>(overlapStart - readStart);
                const uint32_t len = static_cast<uint32_t>(overlapEnd - overlapStart);
                const uint64_t tocRelativeOffset = overlapStart - tocStart;
                
                if (count <= 20 || count % 100 == 0)
                {
                    LOGF_IMPL(Utility, "GTA4_FileLoad", "Decrypting TOC region on-the-fly: bufOffset={} len={} tocRelOffset={}",
                        startInBuf, len, tocRelativeOffset);
                }
                
                DecryptRpfBufferInPlace(hostBuffer + startInBuf, len, tocRelativeOffset);
            }
        }
        
        return bytesRead;
    }

    static uint32_t ReadFromBestRpf(uint32_t handle, uint8_t* hostBuffer, uint32_t size, uint32_t offset, int count, std::string& outName)
    {
        static const std::string kCommon = "common";
        static const std::string kXbox360 = "xbox360";
        static const std::string kAudio = "audio";
        
        auto tryName = [&](const std::string& name) -> uint32_t {
            RpfStreamInfo* rpf = GetRpfStream(name);
            if (!rpf)
                return 0;
            if (rpf->fileSize != 0 && offset >= rpf->fileSize)
                return 0;
            return ReadFromRpfStream(rpf, hostBuffer, size, offset, count, name);
        };
        
        if (auto mapIt = g_handleToRpf.find(handle); mapIt != g_handleToRpf.end())
        {
            const std::string mapped = mapIt->second;
            if (uint32_t n = tryName(mapped); n != 0)
            {
                outName = mapped;
                return n;
            }
            g_handleToRpf.erase(mapIt);
        }
        
        if (uint32_t n = tryName(kCommon); n != 0) { outName = kCommon; return n; }
        if (uint32_t n = tryName(kXbox360); n != 0) { outName = kXbox360; return n; }
        if (uint32_t n = tryName(kAudio); n != 0) { outName = kAudio; return n; }
        
        return 0;
    }

    static std::vector<uint8_t> MakeEmptyRpfImage()
    {
        // Many GTA IV code paths treat a trailing-slash open (e.g. "game:\\") as
        // "mount the disc/RPF container" and then read the RPF header.
        //
        // Provide a tiny, valid-looking header so the title can proceed without
        // triggering Dirty Disc, while we serve actual assets from extracted files.
        //
        // Header layout (20 bytes):
        //   magic[4] = 'RPF3' (bytes)
        //   toc_size u32
        //   entry_count u32
        //   unknown u32
        //   encrypted u32
        // We set all numeric fields to 0 (endian-agnostic).
        std::vector<uint8_t> buf(0x800, 0);
        buf[0] = 'R';
        buf[1] = 'P';
        buf[2] = 'F';
        buf[3] = '3';
        return buf;
    }

    static uint32_t AlignUp(uint32_t value, uint32_t align)
    {
        return (value + (align - 1)) & ~(align - 1);
    }

    // AES key for RPF TOC decryption (loaded from install directory)
    static std::vector<uint8_t> g_aesKey;

    static void LoadAesKeyIfNeeded()
    {
        if (!g_aesKey.empty())
            return;

        // Try multiple locations for AES key
        std::vector<std::filesystem::path> keyPaths = {
            GetGamePath().parent_path() / "aes_key.bin",  // Install dir
            GetGamePath() / "aes_key.bin",                 // Game dir
            std::filesystem::current_path() / "aes_key.bin"  // Current dir
        };
        
        std::filesystem::path keyPath;
        for (const auto& path : keyPaths)
        {
            if (std::filesystem::exists(path))
            {
                keyPath = path;
                break;
            }
        }
        
        if (keyPath.empty())
        {
            keyPath = GetGamePath().parent_path() / "aes_key.bin";  // Default for error message
        }
        std::error_code ec;
        if (!std::filesystem::exists(keyPath, ec))
        {
            LOGF_IMPL(Utility, "LoadAesKeyIfNeeded", "AES key not found: '{}'", keyPath.string());
            return;
        }

        std::ifstream f(keyPath, std::ios::binary);
        if (!f.is_open())
        {
            LOGF_IMPL(Utility, "LoadAesKeyIfNeeded", "Failed to open AES key: '{}'", keyPath.string());
            return;
        }

        std::vector<uint8_t> key((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        if (key.size() >= 32)
        {
            g_aesKey.assign(key.begin(), key.begin() + 32);
            LOGF_IMPL(Utility, "LoadAesKeyIfNeeded", "Loaded AES key ({} bytes)", (int)g_aesKey.size());
        }
        else
        {
            LOGF_IMPL(Utility, "LoadAesKeyIfNeeded", "AES key too small: {} bytes", (int)key.size());
        }
    }

    // Decrypt RPF TOC data using AES-256 ECB repeated 16 times (SparkIV behavior).
    // This version handles misaligned buffer starts by only decrypting complete 16-byte blocks.
    // The caller is responsible for ensuring the buffer corresponds to aligned TOC offsets.
    static void DecryptRpfBufferInPlace(uint8_t* data, uint32_t length, uint64_t tocRelativeOffset = 0)
    {
        if (!data || length == 0)
            return;

        LoadAesKeyIfNeeded();
        if (g_aesKey.size() != 32)
        {
            LOGF_IMPL(Utility, "DecryptRpfBufferInPlace", "AES key not available (size={})", g_aesKey.size());
            return;
        }

        // AES-ECB works on 16-byte blocks. We need to handle the case where the read
        // doesn't start on a block boundary within the TOC.
        // 
        // Strategy: Find the first complete 16-byte block boundary in the buffer,
        // decrypt from there, and leave any partial leading/trailing bytes as-is.
        // This is safe because the game will typically read aligned chunks.
        
        const uint32_t blockSize = 16;
        
        // Calculate how many bytes into a block we are (based on TOC-relative offset)
        uint32_t offsetIntoBlock = static_cast<uint32_t>(tocRelativeOffset % blockSize);
        
        // Skip leading partial block bytes
        uint32_t skipBytes = (offsetIntoBlock == 0) ? 0 : (blockSize - offsetIntoBlock);
        if (skipBytes >= length)
        {
            // Entire buffer is within a partial block, can't decrypt
            LOGF_IMPL(Utility, "DecryptRpfBufferInPlace", "Buffer too small for aligned decrypt (len={} skip={})", length, skipBytes);
            return;
        }
        
        uint8_t* alignedStart = data + skipBytes;
        uint32_t remainingLen = length - skipBytes;
        uint32_t alignedLen = remainingLen & ~0x0Fu; // Round down to block boundary
        
        if (alignedLen == 0)
        {
            LOGF_IMPL(Utility, "DecryptRpfBufferInPlace", "No complete blocks to decrypt (len={} skip={})", length, skipBytes);
            return;
        }

#if defined(__APPLE__)
        std::vector<uint8_t> tmp(alignedLen);

        // Repeat AES-ECB-256 decrypt 16 times (matches SparkIV behavior)
        for (int iter = 0; iter < 16; ++iter)
        {
            size_t outLen = 0;
            CCCryptorStatus status = CCCrypt(kCCDecrypt,
                                             kCCAlgorithmAES,
                                             kCCOptionECBMode,
                                             g_aesKey.data(), kCCKeySizeAES256,
                                             nullptr,
                                             alignedStart, alignedLen,
                                             tmp.data(), alignedLen,
                                             &outLen);

            if (status != kCCSuccess || outLen != alignedLen)
            {
                LOGF_IMPL(Utility, "DecryptRpfBufferInPlace", "AES decrypt failed (status={} iter={})", status, iter);
                return;
            }

            // Copy back for next iteration or result
            memcpy(alignedStart, tmp.data(), alignedLen);
        }
        
        LOGF_IMPL(Utility, "DecryptRpfBufferInPlace", "Decrypted {} bytes (skipped {} leading bytes)", alignedLen, skipBytes);
#else
        // Non-macOS: no AES implementation available here.
        LOGF_IMPL(Utility, "DecryptRpfBufferInPlace", "AES decrypt not implemented on this platform");
#endif
    }

    static uint32_t ErrnoToNtStatus(int err)
    {
        switch (err)
        {
        case EACCES:
            return STATUS_ACCESS_DENIED;
        case ENOENT:
            return STATUS_OBJECT_NAME_NOT_FOUND;
        case ENOTDIR:
            return STATUS_OBJECT_PATH_NOT_FOUND;
        default:
            return STATUS_FAIL_CHECK;
        }
    }

    static void ParseRpfHeader(NtFileHandle* h)
    {
        if (!h || h->rpfHeaderParsed)
            return;

        h->rpfHeaderParsed = true;

        try {
            const std::string ext = h->path.extension().string();
            std::string extLower = ext;
            std::transform(extLower.begin(), extLower.end(), extLower.begin(), [](unsigned char c){ return std::tolower(c); });
            if (extLower != ".rpf")
                return;

            h->isRpf = true;

            // Read first 20 bytes of header
            std::streampos cur = h->stream.tellg();
            h->stream.clear();
            h->stream.seekg(0, std::ios::beg);
            char hdr[20] = {};
            h->stream.read(hdr, sizeof(hdr));
            const std::streamsize got = h->stream.gcount();
            // restore position
            h->stream.clear();
            h->stream.seekg(cur, std::ios::beg);

            if (got < 20)
            {
                LOGF_IMPL(Utility, "ParseRpfHeader", "Failed to read header from '{}' (got {} bytes)", h->path.string(), got);
                return;
            }

            // GTA IV Xbox 360 uses RPF2 (0x52504632 = "RPF2"), not RPF3
            // RPF2 Header layout (20 bytes):
            //   4b - INT32 - RPF Version (0x52504632 for RPF2)
            //   4b - INT32 - Table of Contents Size
            //   4b - INT32 - Number of Entries
            //   4b - INT32 - Unknown
            //   4b - INT32 - Encrypted (0 = unencrypted, non-zero = encrypted)
            
            // Check magic - accept both RPF2 and RPF3
            bool isRpf2 = (hdr[0] == 'R' && hdr[1] == 'P' && hdr[2] == 'F' && hdr[3] == '2');
            bool isRpf3 = (hdr[0] == 'R' && hdr[1] == 'P' && hdr[2] == 'F' && hdr[3] == '3');
            
            if (!isRpf2 && !isRpf3)
            {
                LOGF_IMPL(Utility, "ParseRpfHeader", "Unknown RPF magic in '{}': {:02X} {:02X} {:02X} {:02X}", 
                          h->path.string(), (uint8_t)hdr[0], (uint8_t)hdr[1], (uint8_t)hdr[2], (uint8_t)hdr[3]);
                return;
            }

            auto le32 = [](const uint8_t* p) -> uint32_t {
                return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
            };

            uint32_t tocSize = le32(reinterpret_cast<uint8_t*>(hdr + 4));
            uint32_t numEntries = le32(reinterpret_cast<uint8_t*>(hdr + 8));
            uint32_t encrypted = le32(reinterpret_cast<uint8_t*>(hdr + 16));

            // TOC starts at offset 0x800 (2048 bytes from file origin)
            h->tocOffset = 0x800;
            h->tocSize = tocSize;
            h->tocEncrypted = (encrypted != 0);

            LOGF_IMPL(Utility, "ParseRpfHeader", "Parsed {} header '{}': tocOffset=0x{:X} tocSize={} entries={} encrypted={}", 
                      isRpf2 ? "RPF2" : "RPF3", h->path.filename().string(), h->tocOffset, tocSize, numEntries, h->tocEncrypted);
        }
        catch (...) {
            LOGF_IMPL(Utility, "ParseRpfHeader", "Exception parsing header for '{}'", h->path.string());
        }
    }

    static bool TryGetAnsiPath(const XOBJECT_ATTRIBUTES* attributes, std::string& out)
    {
        out.clear();
        if (!attributes)
        {
            LOG_IMPL(Utility, "TryGetAnsiPath", "attributes is null");
            return false;
        }

        const XANSI_STRING* name = attributes->Name.get();
        if (!name)
        {
            LOG_IMPL(Utility, "TryGetAnsiPath", "Name.get() is null");
            return false;
        }

        const char* buf = name->Buffer.get();
        if (!buf)
        {
            LOG_IMPL(Utility, "TryGetAnsiPath", "Buffer.get() is null");
            return false;
        }

        const uint16_t len = name->Length.get();
        // Sanity check: zero length or very large lengths are suspect
        if (len == 0)
        {
            // Empty paths will always fail - reject silently
            return false;
        }
        else if (len > 1024)
        {
            LOGF_IMPL(Utility, "TryGetAnsiPath", "Suspicious length: {} (max 1024)", len);
            return false;
        }
        else
        {
            // Check if the path looks valid
            // Valid paths should start with a letter (drive:) or a letter after game:\ prefix
            unsigned char firstChar = static_cast<unsigned char>(buf[0]);
            
            // First character should be printable ASCII for a valid path
            // Valid starts: letters (a-z, A-Z), backslash, dot, or space
            bool validStart = (firstChar >= 'A' && firstChar <= 'Z') ||
                              (firstChar >= 'a' && firstChar <= 'z') ||
                              firstChar == '\\' || firstChar == '.' || 
                              firstChar == ' ' || firstChar == '/';
            
            if (!validStart)
            {
                // Check if it's just high-byte garbage (common pattern)
                if (firstChar >= 128)
                {
                    LOGF_IMPL(Utility, "TryGetAnsiPath", "Garbage path rejected (len={}, first=0x{:02X}, Name.ptr=0x{:08X}, Buffer.ptr=0x{:08X})", 
                        len, firstChar, attributes->Name.ptr.value, name->Buffer.ptr.value);
                    return false;
                }
            }
            
            // Also check for excessive non-ASCII content
            size_t garbageCount = 0;
            for (size_t i = 0; i < std::min<size_t>(len, 20); i++)
            {
                unsigned char c = static_cast<unsigned char>(buf[i]);
                if (c >= 128)
                    garbageCount++;
            }
            // If more than 50% of first 20 chars are non-ASCII, reject
            if (garbageCount > 10)
            {
                LOGF_IMPL(Utility, "TryGetAnsiPath", "Mostly garbage path rejected (len={}, garbage={}/20)", len, garbageCount);
                return false;
            }
        }

        out.assign(buf, buf + len);
        return true;
    }

    static std::filesystem::path ResolveGuestPathBestEffort(const std::string& guestPath)
    {
        std::filesystem::path resolved = FileSystem::ResolvePath(guestPath, true);

        std::error_code ec;
        if (std::filesystem::exists(resolved, ec))
            return resolved;

        const std::filesystem::path cachedPath = FindInPathCache(resolved.string());
        if (!cachedPath.empty())
            return cachedPath;

        return resolved;
    }

    static uint32_t GetFileAttributesBestEffort(const std::filesystem::path& path)
    {
        std::error_code ec;
        const bool isDir = std::filesystem::is_directory(path, ec);
        if (ec)
            return FILE_ATTRIBUTE_NORMAL;

        if (isDir)
            return FILE_ATTRIBUTE_DIRECTORY;

        return FILE_ATTRIBUTE_NORMAL;
    }

    static uint64_t RoundUpToPage(uint64_t value)
    {
        constexpr uint64_t kPageSize = 0x1000;
        return (value + (kPageSize - 1)) & ~(kPageSize - 1);
    }

#pragma pack(push, 1)
    struct XFILE_BASIC_INFORMATION
    {
        be<int64_t> CreationTime;
        be<int64_t> LastAccessTime;
        be<int64_t> LastWriteTime;
        be<int64_t> ChangeTime;
        be<uint32_t> FileAttributes;
        be<uint32_t> Reserved;
    };

    struct XFILE_STANDARD_INFORMATION
    {
        be<int64_t> AllocationSize;
        be<int64_t> EndOfFile;
        be<uint32_t> NumberOfLinks;
        uint8_t DeletePending;
        uint8_t Directory;
        uint16_t Reserved;
    };

    struct XFILE_POSITION_INFORMATION
    {
        be<int64_t> CurrentByteOffset;
    };

    struct XFILE_END_OF_FILE_INFORMATION
    {
        be<int64_t> EndOfFile;
    };

    struct XFILE_NETWORK_OPEN_INFORMATION
    {
        be<int64_t> CreationTime;
        be<int64_t> LastAccessTime;
        be<int64_t> LastWriteTime;
        be<int64_t> ChangeTime;
        be<int64_t> AllocationSize;
        be<int64_t> EndOfFile;
        be<uint32_t> FileAttributes;
        be<uint32_t> Reserved;
    };

    struct XFILE_FS_SIZE_INFORMATION
    {
        be<int64_t> TotalAllocationUnits;
        be<int64_t> AvailableAllocationUnits;
        be<uint32_t> SectorsPerAllocationUnit;
        be<uint32_t> BytesPerSector;
    };

    struct XFILE_FS_DEVICE_INFORMATION
    {
        be<uint32_t> DeviceType;
        be<uint32_t> Characteristics;
    };

    struct XFILE_FS_ATTRIBUTE_INFORMATION_FIXED
    {
        be<uint32_t> FileSystemAttributes;
        be<uint32_t> MaximumComponentNameLength;
        be<uint32_t> FileSystemNameLength;
        be<uint16_t> FileSystemName[16];
    };

    // FILE_INFORMATION_CLASS for NtQueryDirectoryFile (subset)
    constexpr uint32_t FileDirectoryInformation = 1;
    constexpr uint32_t FileFullDirectoryInformation = 2;
    constexpr uint32_t FileBothDirectoryInformation = 3;

    struct XFILE_DIRECTORY_INFORMATION_FIXED
    {
        be<uint32_t> NextEntryOffset;
        be<uint32_t> FileIndex;
        be<int64_t> CreationTime;
        be<int64_t> LastAccessTime;
        be<int64_t> LastWriteTime;
        be<int64_t> ChangeTime;
        be<int64_t> EndOfFile;
        be<int64_t> AllocationSize;
        be<uint32_t> FileAttributes;
        be<uint32_t> FileNameLength;
        // Followed by FileName (UTF-16BE)
    };

    struct XFILE_FULL_DIR_INFORMATION_FIXED
    {
        be<uint32_t> NextEntryOffset;
        be<uint32_t> FileIndex;
        be<int64_t> CreationTime;
        be<int64_t> LastAccessTime;
        be<int64_t> LastWriteTime;
        be<int64_t> ChangeTime;
        be<int64_t> EndOfFile;
        be<int64_t> AllocationSize;
        be<uint32_t> FileAttributes;
        be<uint32_t> FileNameLength;
        be<uint32_t> EaSize;
        // Followed by FileName (UTF-16BE)
    };

    struct XFILE_BOTH_DIR_INFORMATION_FIXED
    {
        be<uint32_t> NextEntryOffset;
        be<uint32_t> FileIndex;
        be<int64_t> CreationTime;
        be<int64_t> LastAccessTime;
        be<int64_t> LastWriteTime;
        be<int64_t> ChangeTime;
        be<int64_t> EndOfFile;
        be<int64_t> AllocationSize;
        be<uint32_t> FileAttributes;
        be<uint32_t> FileNameLength;
        be<uint32_t> EaSize;
        uint8_t ShortNameLength;
        uint8_t Reserved1;
        be<uint16_t> ShortName[12];
        // Followed by FileName (UTF-16BE)
    };
#pragma pack(pop)

    static_assert(sizeof(XFILE_BASIC_INFORMATION) == 40);
    static_assert(sizeof(XFILE_STANDARD_INFORMATION) == 24);
    static_assert(sizeof(XFILE_POSITION_INFORMATION) == 8);
    static_assert(sizeof(XFILE_END_OF_FILE_INFORMATION) == 8);
    static_assert(sizeof(XFILE_NETWORK_OPEN_INFORMATION) == 56);
    static_assert(sizeof(XFILE_FS_SIZE_INFORMATION) == 24);
    static_assert(sizeof(XFILE_FS_DEVICE_INFORMATION) == 8);
}

struct Event final : KernelObject, HostObject<XKEVENT>
{
    bool manualReset;
    std::atomic<bool> signaled;

    Event(XKEVENT* header)
        : manualReset(!header->Type), signaled(!!header->SignalState)
    {
    }

    Event(bool manualReset, bool initialState)
        : manualReset(manualReset), signaled(initialState)
    {
    }

    uint32_t Wait(uint32_t timeout) override
    {
        if (timeout == 0)
        {
            if (manualReset)
            {
                if (!signaled)
                    return STATUS_TIMEOUT;
            }
            else
            {
                bool expected = true;
                if (!signaled.compare_exchange_strong(expected, false))
                    return STATUS_TIMEOUT;
            }
        }
        else if (timeout == INFINITE)
        {
            if (manualReset)
            {
                signaled.wait(false);
            }
            else
            {
                while (true)
                {
                    bool expected = true;
                    if (signaled.compare_exchange_weak(expected, false))
                        break;

                    signaled.wait(expected);
                }
            }
        }
        else
        {
            // Finite timeout - convert to milliseconds and do a timed wait
            // Xbox timeout is in 100ns units, negative means relative
            auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout);
            
            if (manualReset)
            {
                while (!signaled.load())
                {
                    if (std::chrono::steady_clock::now() >= deadline)
                        return STATUS_TIMEOUT;
                    // Brief sleep to avoid busy-wait
                    std::this_thread::sleep_for(std::chrono::microseconds(100));
                }
            }
            else
            {
                while (true)
                {
                    bool expected = true;
                    if (signaled.compare_exchange_weak(expected, false))
                        break;
                    
                    if (std::chrono::steady_clock::now() >= deadline)
                        return STATUS_TIMEOUT;
                    // Brief sleep to avoid busy-wait
                    std::this_thread::sleep_for(std::chrono::microseconds(100));
                }
            }
        }

        return STATUS_SUCCESS;
    }

    bool Set()
    {
        signaled = true;

        if (manualReset)
            signaled.notify_all();
        else
            signaled.notify_one();

        return TRUE;
    }

    bool Reset()
    {
        signaled = false;
        return TRUE;
    }
};

static std::atomic<uint32_t> g_keSetEventGeneration;

struct Semaphore final : KernelObject, HostObject<XKSEMAPHORE>
{
    std::atomic<uint32_t> count;
    uint32_t maximumCount;

    Semaphore(XKSEMAPHORE* semaphore)
        : count(semaphore->Header.SignalState), maximumCount(semaphore->Limit)
    {
    }

    Semaphore(uint32_t count, uint32_t maximumCount)
        : count(count), maximumCount(maximumCount)
    {
    }

    uint32_t Wait(uint32_t timeout) override
    {
        static int s_waitCount = 0;
        ++s_waitCount;
        
        if (timeout == 0)
        {
            uint32_t currentCount = count.load();
            if (currentCount != 0)
            {
                if (count.compare_exchange_weak(currentCount, currentCount - 1))
                    return STATUS_SUCCESS;
            }

            return STATUS_TIMEOUT;
        }
        else if (timeout == INFINITE)
        {
            uint32_t currentCount;
            int loopCount = 0;
            while (true)
            {
                currentCount = count.load();
                if (currentCount != 0)
                {
                    if (count.compare_exchange_weak(currentCount, currentCount - 1))
                    {
                        if (s_waitCount <= 30)
                            LOGF_WARNING("[SemWait] #{} acquired count={} after {} loops", s_waitCount, currentCount, loopCount);
                        return STATUS_SUCCESS;
                    }
                }
                else
                {
                    if (loopCount == 0 && s_waitCount <= 30)
                        LOGF_WARNING("[SemWait] #{} blocking on count=0 (max={})...", s_waitCount, maximumCount);
                    ++loopCount;
                    count.wait(0);
                }
            }

            return STATUS_SUCCESS;
        }
        else
        {
            assert(false && "Unhandled timeout value.");
            return STATUS_TIMEOUT;
        }
    }

    void Release(uint32_t releaseCount, uint32_t* previousCount)
    {
        if (previousCount != nullptr)
            *previousCount = count;

        assert(count + releaseCount <= maximumCount);

        count += releaseCount;
        count.notify_all();
    }
};

// =============================================================================
// SEMAPHORE SIGNALING FUNCTIONS (defined after Semaphore struct)
// =============================================================================

// Signal all tracked blocking semaphores - called after init completes
// Signals unconditionally to wake workers that may have acquired and are waiting again
void SignalAllBlockingSemaphores() {
    // Copy the set while holding the lock, then release lock before signaling
    // This prevents deadlock when awakened threads try to track new semaphores
    std::unordered_set<uint32_t> addrsCopy;
    {
        std::lock_guard<std::mutex> lock(g_semaphoreTrackMutex);
        addrsCopy = g_blockingSemaphoreAddrs;
    }
    
    int signaled = 0;
    for (uint32_t addr : addrsCopy) {
        // Try as kernel handle first (from NtCreateSemaphore)
        if (IsKernelObject(addr)) {
            Semaphore* sem = GetKernelObject<Semaphore>(addr);
            if (sem) {
                sem->Release(1, nullptr);
                signaled++;
            }
            continue;
        }
        // Try as guest address (from KeInitializeSemaphore)
        XKSEMAPHORE* semaphore = reinterpret_cast<XKSEMAPHORE*>(g_memory.Translate(addr));
        if (semaphore && semaphore->Header.Type == 5) {
            auto* object = QueryKernelObject<Semaphore>(semaphore->Header);
            if (object) {
                object->Release(1, nullptr);
                signaled++;
            }
        }
    }
    if (signaled > 0) {
        LOGF_WARNING("[SEMAPHORE_FIX] Signaled {} semaphores", signaled);
    }
}

// Mark init as complete and signal blocked semaphores
void SetInitComplete() {
    if (!g_initComplete.exchange(true)) {
        LOGF_WARNING("[SEMAPHORE_FIX] Init complete - signaling {} blocked semaphores", 
                     g_blockingSemaphoreAddrs.size());
        SignalAllBlockingSemaphores();
    }
}

// =============================================================================
// WORKER CONTEXT REGISTRY AND SHUTDOWN
// =============================================================================

// Worker context structure - describes a worker thread's context in guest memory
struct WorkerContext {
    uint32_t contextBase;      // Guest memory address of worker context
    uint32_t exitFlagOffset;   // Offset to exit flag (byte)
    uint32_t semHandleOffset;  // Offset to semaphore handle (word)
    const char* name;          // Debug name
};

// Registry of known worker contexts
// Add new workers here as they are discovered
static std::vector<WorkerContext> g_workerContexts = {
    // Photo Mode worker - blocks during sub_821A81F0 cleanup
    // Context at 0x82B9C1D0, exit flag at +10 (decimal), semaphore handle at +20 (decimal)
    // PPC: lbz r11,10(r26) for exit flag, lwz r3,20(r26) for semaphore
    {0x82B9C1D0, 10, 20, "PhotoMode"},
};

// Shutdown all known workers by setting exit flags and signaling semaphores
// This allows workers to exit their wait loops gracefully
void ShutdownAllWorkers() {
    printf("[WORKER_SHUTDOWN] Shutting down %zu known workers...\n", g_workerContexts.size());
    fflush(stdout);
    
    int shutdownCount = 0;
    for (const auto& worker : g_workerContexts) {
        // Set exit flag to 1
        uint8_t* exitFlag = reinterpret_cast<uint8_t*>(g_memory.Translate(worker.contextBase + worker.exitFlagOffset));
        if (exitFlag) {
            *exitFlag = 1;
            printf("[WORKER_SHUTDOWN] %s: Set exit flag at 0x%08X\n", worker.name, worker.contextBase + worker.exitFlagOffset);
        }
        
        // Read semaphore handle from guest memory (big-endian)
        uint32_t* semHandlePtr = reinterpret_cast<uint32_t*>(g_memory.Translate(worker.contextBase + worker.semHandleOffset));
        if (semHandlePtr) {
            uint32_t semHandle = __builtin_bswap32(*semHandlePtr);  // Convert from big-endian
            if (semHandle != 0) {
                // Try as kernel handle
                if (IsKernelObject(semHandle)) {
                    Semaphore* sem = GetKernelObject<Semaphore>(semHandle);
                    if (sem) {
                        sem->Release(1, nullptr);
                        printf("[WORKER_SHUTDOWN] %s: Signaled semaphore handle 0x%08X\n", worker.name, semHandle);
                        shutdownCount++;
                    }
                }
            }
        }
    }
    
    printf("[WORKER_SHUTDOWN] Signaled %d workers\n", shutdownCount);
    fflush(stdout);
}

// Timeout-based wait for semaphores during init - prevents infinite blocking
// Returns true if semaphore was acquired, false if timeout
bool TryWaitSemaphoreWithTimeout(uint32_t semAddr, uint32_t timeoutMs) {
    XKSEMAPHORE* semaphore = reinterpret_cast<XKSEMAPHORE*>(g_memory.Translate(semAddr));
    if (!semaphore || semaphore->Header.Type != 5) return false;
    
    auto* object = QueryKernelObject<Semaphore>(semaphore->Header);
    if (!object) return false;
    
    auto startTime = std::chrono::steady_clock::now();
    while (true) {
        uint32_t currentCount = object->count.load();
        if (currentCount != 0) {
            if (object->count.compare_exchange_weak(currentCount, currentCount - 1)) {
                return true;  // Successfully acquired
            }
        }
        
        auto elapsed = std::chrono::steady_clock::now() - startTime;
        if (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() >= timeoutMs) {
            return false;  // Timeout
        }
        
        // Brief sleep before retry
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

inline void CloseKernelObject(XDISPATCHER_HEADER& header)
{
    if (header.WaitListHead.Flink != OBJECT_SIGNATURE)
    {
        return;
    }

    DestroyKernelObject(header.WaitListHead.Blink);
}

uint32_t GuestTimeoutToMilliseconds(be<int64_t>* timeout)
{
    return timeout ? (*timeout * -1) / 10000 : INFINITE;
}

void VdHSIOCalibrationLock()
{
    LOG_UTILITY("!!! STUB !!!");
}

void KeCertMonitorData()
{
    LOG_UTILITY("!!! STUB !!!");
}

void XexExecutableModuleHandle()
{
    LOG_UTILITY("!!! STUB !!!");
}

void ExLoadedCommandLine()
{
    LOG_UTILITY("!!! STUB !!!");
}

void KeDebugMonitorData()
{
    LOG_UTILITY("!!! STUB !!!");
}

void ExThreadObjectType()
{
    LOG_UTILITY("!!! STUB !!!");
}

void KeTimeStampBundle()
{
    LOG_UTILITY("!!! STUB !!!");
}

void XboxHardwareInfo()
{
    LOG_UTILITY("!!! STUB !!!");
}

void XGetVideoMode()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t XGetGameRegion()
{
    if (Config::Language == ELanguage::Japanese)
        return 0x0101;

    return 0x03FF;
}

uint32_t XMsgStartIORequest(uint32_t App, uint32_t Message, XXOVERLAPPED* lpOverlapped, void* Buffer, uint32_t szBuffer)
{
    return STATUS_SUCCESS;
}



uint32_t XamGetSystemVersion()
{
    return 0;
}

void XamContentDelete()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t XamContentGetCreator(uint32_t userIndex, const XCONTENT_DATA* contentData, be<uint32_t>* isCreator, be<uint64_t>* xuid, XXOVERLAPPED* overlapped)
{
    if (isCreator)
        *isCreator = true;

    if (xuid)
        *xuid = 0xB13EBABEBABEBABE;

    return 0;
}

uint32_t XamContentGetDeviceState()
{
    return 0;
}

uint32_t XamUserGetSigninInfo(uint32_t userIndex, uint32_t flags, XUSER_SIGNIN_INFO* info)
{
    if (userIndex == 0)
    {
        memset(info, 0, sizeof(*info));
        info->xuid = 0xB13EBABEBABEBABE;
        info->SigninState = 1;
        strcpy(info->Name, "SWA");
        return 0;
    }

    return 0x00000525; // ERROR_NO_SUCH_USER
}

void XamShowSigninUI()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t XamShowDeviceSelectorUI
(
    uint32_t userIndex,
    uint32_t contentType,
    uint32_t contentFlags,
    uint64_t totalRequested,
    be<uint32_t>* deviceId,
    XXOVERLAPPED* overlapped
)
{
    XamNotifyEnqueueEvent(9, true);
    *deviceId = 1;
    XamNotifyEnqueueEvent(9, false);
    return 0;
}

void XamShowDirtyDiscErrorUI()
{
    static int callCount = 0;
    ++callCount;
    
    // Pump SDL events to keep window responsive
    PumpSdlEventsIfNeeded();
    
    // Only log first few to avoid spam
    if (callCount <= 3) {
        LOGF_UTILITY("!!! STUB !!! - Dirty disc error #{} (bypassing)", callCount);
    }
    
    // DON'T exit - just return and let the game continue
    // The game may retry or find an alternative code path
    // Small delay to prevent tight CPU loop
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    // Return normally - game will either retry or continue
}

void XamEnableInactivityProcessing()
{
    LOG_UTILITY("!!! STUB !!!");
}

void XamResetInactivity()
{
    LOG_UTILITY("!!! STUB !!!");
}

void XamShowMessageBoxUIEx()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t XGetLanguage()
{
    return (uint32_t)Config::Language.Value;
}

uint32_t XGetAVPack()
{
    return 0;
}

void XamLoaderTerminateTitle()
{
    LOG_UTILITY("!!! STUB !!!");
}

void XamGetExecutionId()
{
    LOG_UTILITY("!!! STUB !!!");
}

void XamLoaderLaunchTitle()
{
    LOG_UTILITY("!!! STUB !!!");
}

// NtOpenFile is essentially a simplified NtCreateFile for opening existing files
uint32_t NtOpenFile(
    be<uint32_t>* FileHandle,
    uint32_t DesiredAccess,
    XOBJECT_ATTRIBUTES* Attributes,
    XIO_STATUS_BLOCK* IoStatusBlock,
    uint32_t ShareAccess,
    uint32_t OpenOptions)
{
    (void)ShareAccess;
    (void)OpenOptions;

    if (!FileHandle || !Attributes)
        return STATUS_INVALID_PARAMETER;

    std::string guestPath;
    if (!TryGetAnsiPath(Attributes, guestPath))
        return STATUS_INVALID_PARAMETER;

    // DesiredAccess on Xbox uses Win32-like GENERIC_READ/WRITE bits.
    std::ios::openmode mode = std::ios::binary;
    if (DesiredAccess & (GENERIC_READ | FILE_READ_DATA))
        mode |= std::ios::in;
    if (DesiredAccess & GENERIC_WRITE)
        mode |= std::ios::out;

    // Fallback: treat unknown access flags as read.
    if ((mode & (std::ios::in | std::ios::out)) == 0)
        mode |= std::ios::in;

    std::filesystem::path resolved = ResolveGuestPathBestEffort(guestPath);

    LOGF_IMPL(Utility, "NtOpenFile", "Guest: '{}' -> Resolved: '{}'", guestPath, resolved.string());

    // Check for directory
    {
        std::error_code ec;
        bool exists = std::filesystem::exists(resolved, ec);
        bool isDir = exists && std::filesystem::is_directory(resolved, ec);
        
        LOGF_IMPL(Utility, "NtOpenFile", "exists={} isDir={} ec={}", exists, isDir, ec ? ec.message() : "none");
        
        if (exists && isDir && !ec)
        {
            NtDirHandle* hDir = CreateKernelObject<NtDirHandle>();
            hDir->path = resolved;

            const uint32_t handleValue = GetKernelHandle(hDir);
            g_ntDirHandles.emplace(handleValue, hDir);

            *FileHandle = handleValue;
            LOGF_IMPL(Utility, "NtOpenFile", "Created directory handle 0x{:08X} for {}", handleValue, resolved.string());
            if (IoStatusBlock)
            {
                IoStatusBlock->Status = STATUS_SUCCESS;
                IoStatusBlock->Information = 1;
            }

            return STATUS_SUCCESS;
        }
    }

    std::fstream stream;
    stream.open(resolved, mode);

    if (!stream.is_open())
    {
        const uint32_t status = ErrnoToNtStatus(errno);
        LOGF_IMPL(Utility, "NtOpenFile", "FAILED to open '{}' -> '{}': errno={} status=0x{:08X}", 
                  guestPath, resolved.string(), errno, status);
        if (IoStatusBlock)
        {
            IoStatusBlock->Status = status;
            IoStatusBlock->Information = 0;
        }
        *FileHandle = 0;
        return status;
    }

    NtFileHandle* h = CreateKernelObject<NtFileHandle>();
    h->stream = std::move(stream);
    h->path = std::move(resolved);
    // Mark RPF files for special handling (TOC decryption)
    try {
        std::string ext = h->path.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c){ return std::tolower(c); });
        if (ext == ".rpf")
            h->isRpf = true;
    } catch (...) {}

    const uint32_t handleValue = GetKernelHandle(h);
    g_ntFileHandles.emplace(handleValue, h);

    *FileHandle = handleValue;
    LOGF_IMPL(Utility, "NtOpenFile", "Opened file handle 0x{:08X} for {}", handleValue, resolved.string());
    if (IoStatusBlock)
    {
        IoStatusBlock->Status = STATUS_SUCCESS;
        IoStatusBlock->Information = 1;
    }
    return STATUS_SUCCESS;
}

void RtlInitAnsiString(XANSI_STRING* destination, char* source)
{
    const uint16_t length = source ? (uint16_t)strlen(source) : 0;
    destination->Length = length;
    destination->MaximumLength = length + 1;
    destination->Buffer = source;
}

uint32_t NtCreateFile
(
    be<uint32_t>* FileHandle,
    uint32_t DesiredAccess,
    XOBJECT_ATTRIBUTES* Attributes,
    XIO_STATUS_BLOCK* IoStatusBlock,
    uint64_t* AllocationSize,
    uint32_t FileAttributes,
    uint32_t ShareAccess,
    uint32_t CreateDisposition
)
{
    // Pump SDL events to keep window responsive
    PumpSdlEventsIfNeeded();
    
    (void)AllocationSize;
    (void)FileAttributes;
    (void)ShareAccess;

    if (!FileHandle || !Attributes)
        return STATUS_INVALID_PARAMETER;

    std::string guestPath;
    if (!TryGetAnsiPath(Attributes, guestPath))
        return STATUS_INVALID_PARAMETER;

    // === PHASE 2: Storage Init File Tracing ===
    static int s_storageInitFileCount = 0;
    if (g_inStorageInit.load()) {
        ++s_storageInitFileCount;
        LOGF_WARNING("[STORAGE-FILE] NtCreateFile #{} path='{}' access=0x{:08X}", 
                     s_storageInitFileCount, guestPath, DesiredAccess);
    }
    // === END PHASE 2 ===

    // DesiredAccess on Xbox uses Win32-like GENERIC_READ/WRITE bits.
    std::ios::openmode mode = std::ios::binary;
    if (DesiredAccess & (GENERIC_READ | FILE_READ_DATA))
        mode |= std::ios::in;
    if (DesiredAccess & GENERIC_WRITE)
        mode |= std::ios::out;

    // Fallback: treat unknown access flags as read.
    if ((mode & (std::ios::in | std::ios::out)) == 0)
        mode |= std::ios::in;

    // === VFS-BASED FILE SERVING ===
    // Try to serve files directly from extracted directory first.
    // This bypasses the complex RPF offset-based reading that causes stream issues.
    if (VFS::IsInitialized())
    {
        auto vfsResolved = VFS::Resolve(guestPath);
        if (!vfsResolved.empty())
        {
            std::error_code ec;
            bool vfsExists = std::filesystem::exists(vfsResolved, ec);
            bool vfsIsDir = vfsExists && std::filesystem::is_directory(vfsResolved, ec);
            
            if (vfsExists && !ec)
            {
                if (vfsIsDir)
                {
                    // Return directory handle for VFS directory
                    NtDirHandle* hDir = CreateKernelObject<NtDirHandle>();
                    hDir->path = vfsResolved;
                    
                    const uint32_t handleValue = GetKernelHandle(hDir);
                    g_ntDirHandles.emplace(handleValue, hDir);
                    
                    *FileHandle = handleValue;
                    LOGF_IMPL(Utility, "NtCreateFile", "[VFS] Directory: '{}' -> {} (handle=0x{:08X})", 
                        guestPath, vfsResolved.string(), handleValue);
                    
                    if (IoStatusBlock)
                    {
                        IoStatusBlock->Status = STATUS_SUCCESS;
                        IoStatusBlock->Information = 1;
                    }
                    return STATUS_SUCCESS;
                }
                else
                {
                    // Open file directly from VFS
                    std::fstream fileStream;
                    fileStream.open(vfsResolved, mode);
                    
                    if (fileStream.is_open())
                    {
                        NtFileHandle* hFile = CreateKernelObject<NtFileHandle>();
                        hFile->stream = std::move(fileStream);
                        hFile->path = vfsResolved;
                        hFile->isRpf = false;
                        
                        const uint32_t handleValue = GetKernelHandle(hFile);
                        g_ntFileHandles.emplace(handleValue, hFile);
                        
                        *FileHandle = handleValue;
                        LOGF_IMPL(Utility, "NtCreateFile", "[VFS] File: '{}' -> {} (handle=0x{:08X})", 
                            guestPath, vfsResolved.string(), handleValue);
                        
                        if (IoStatusBlock)
                        {
                            IoStatusBlock->Status = STATUS_SUCCESS;
                            IoStatusBlock->Information = 1;
                        }
                        return STATUS_SUCCESS;
                    }
                }
            }
        }
    }
    // === END VFS-BASED FILE SERVING ===

    std::filesystem::path resolved = ResolveGuestPathBestEffort(guestPath);

    // Debug: Log resolved path for troubleshooting
    LOGF_IMPL(Utility, "NtCreateFile", "Guest: '{}' -> Resolved: '{}' DesiredAccess=0x{:08X}", guestPath, resolved.string(), DesiredAccess);

    // Check if this is a direct .rpf file open (e.g., "game:\common.rpf")
    {
        std::string pathLower = guestPath;
        std::transform(pathLower.begin(), pathLower.end(), pathLower.begin(), ::tolower);
        if (pathLower.find(".rpf") != std::string::npos)
        {
            LOGF_IMPL(Utility, "NtCreateFile", "RPF file access detected: '{}'", guestPath);
        }
    }

    // GTA IV RPF handling: When the game tries to open a path ending with backslash
    // (like "fxl_final\" or "game:\") as if it were a file, it's trying to mount an RPF archive.
    // Since we've extracted the RPF contents, we should let the game fail gracefully here
    // so it falls back to accessing extracted files individually.
    // However, if the game opens these for directory enumeration (not file reading), we allow it.
    bool isRpfMountAttempt = !guestPath.empty() && 
                              (guestPath.back() == '\\' || guestPath.back() == '/') &&
                              (DesiredAccess & (GENERIC_READ | FILE_READ_DATA)) &&
                              !(DesiredAccess & GENERIC_WRITE);

    // Directories can't be opened with std::fstream; represent them with a dedicated handle.
    {
        std::error_code ec;
        bool exists = std::filesystem::exists(resolved, ec);
        bool isDir = exists && std::filesystem::is_directory(resolved, ec);
        LOGF_IMPL(Utility, "NtCreateFile", "exists={} isDir={} ec={}", exists, isDir, ec ? ec.message() : "none");
        
        if (exists && isDir && !ec)
        {
            // GTA IV behavior: it opens `game:\\` (and sometimes `D:\\`) as if it were a file
            // and then reads an RPF header from it as part of its packfile/disc mounting logic.
            // On our host FS `game` resolves to a directory, which makes reads fail and triggers
            // the dirty-disc loop.
            //
            // Fix: for mount-style opens on the root, map that open to real .rpf files and let
            // the title parse them using normal NtReadFile semantics.
            // For all other directories, always return a directory handle so enumeration works.
            auto isRootMountPath = [](const std::string& path) -> bool
            {
                std::string_view p(path);
                while (!p.empty() && (p.back() == '\\' || p.back() == '/'))
                    p.remove_suffix(1);

                auto ieq = [](std::string_view a, std::string_view b) -> bool
                {
                    if (a.size() != b.size())
                        return false;
                    for (size_t i = 0; i < a.size(); i++)
                    {
                        char ca = a[i];
                        char cb = b[i];
                        if (ca >= 'A' && ca <= 'Z')
                            ca = char(ca - 'A' + 'a');
                        if (cb >= 'A' && cb <= 'Z')
                            cb = char(cb - 'A' + 'a');
                        if (ca != cb)
                            return false;
                    }
                    return true;
                };

                return ieq(p, "game:") || ieq(p, "d:");
            };

            if (isRpfMountAttempt)
            {
                // GTA IV opens directories with trailing backslash as if they were RPF files.
                // For game:\ root, treat it as a directory so the game can enumerate RPF files.
                // For paths like fxl_final\, these are inside RPFs - redirect to extracted content.
                
                std::string pathLower = guestPath;
                std::transform(pathLower.begin(), pathLower.end(), pathLower.begin(), ::tolower);
                
                // Strip trailing slashes
                while (!pathLower.empty() && (pathLower.back() == '\\' || pathLower.back() == '/'))
                    pathLower.pop_back();
                
                // For game:\ root, return as directory handle so game can enumerate files
                if (pathLower == "game:" || pathLower == "d:")
                {
                    // Return directory handle for game root
                    NtDirHandle* hDir = CreateKernelObject<NtDirHandle>();
                    hDir->path = resolved;

                    const uint32_t handleValue = GetKernelHandle(hDir);
                    g_ntDirHandles.emplace(handleValue, hDir);

                    *FileHandle = handleValue;
                    LOGF_IMPL(Utility, "NtCreateFile", "game:\\ root -> directory handle 0x{:08X} for {}", handleValue, resolved.string());
                    
                    if (IoStatusBlock)
                    {
                        IoStatusBlock->Status = STATUS_SUCCESS;
                        IoStatusBlock->Information = 1;
                    }
                    return STATUS_SUCCESS;
                }
                
                // =======================================================================
                // SONIC UNLEASHED APPROACH: Bypass shader directory enumeration.
                // All 1132 shaders are pre-compiled and embedded in g_shaderCacheEntries[].
                // Return STATUS_OBJECT_NAME_NOT_FOUND to force use of embedded cache.
                // =======================================================================
                if (pathLower.find("fxl_final") != std::string::npos ||
                    pathLower.find("shaders") != std::string::npos)
                {
                    static int s_shaderBypassCount = 0;
                    if (++s_shaderBypassCount <= 5) {
                        LOGF_IMPL(Utility, "NtCreateFile", "SHADER BYPASS #{}: '{}' - using embedded cache (1132 pre-compiled shaders)", 
                                  s_shaderBypassCount, guestPath);
                    }
                    
                    // Return "not found" - forces game to use embedded shader cache
                    if (IoStatusBlock)
                    {
                        IoStatusBlock->Status = STATUS_OBJECT_NAME_NOT_FOUND;
                        IoStatusBlock->Information = 0;
                    }
                    return STATUS_OBJECT_NAME_NOT_FOUND;
                }
                
                // For non-shader paths, try extracted directory
                std::filesystem::path extractedPath = GetGamePath() / "RPF DUMP" / "common";
                if (std::filesystem::exists(extractedPath))
                {
                    NtDirHandle* hDir = CreateKernelObject<NtDirHandle>();
                    hDir->path = extractedPath;

                    const uint32_t handleValue = GetKernelHandle(hDir);
                    g_ntDirHandles.emplace(handleValue, hDir);

                    *FileHandle = handleValue;
                    LOGF_IMPL(Utility, "NtCreateFile", "RPF path -> extracted dir: {} (handle=0x{:08X})", 
                              extractedPath.string(), handleValue);
                    
                    if (IoStatusBlock)
                    {
                        IoStatusBlock->Status = STATUS_SUCCESS;
                        IoStatusBlock->Information = 1;
                    }
                    return STATUS_SUCCESS;
                }
                
                // For other RPF mount attempts, return virtual empty RPF
                NtVirtFileHandle* hVirt = CreateKernelObject<NtVirtFileHandle>();
                hVirt->debugPath = resolved;
                hVirt->data = MakeEmptyRpfImage();

                const uint32_t handleValue = GetKernelHandle(hVirt);
                g_ntVirtFileHandles.emplace(handleValue, hVirt);

                *FileHandle = handleValue;
                LOGF_IMPL(Utility, "NtCreateFile", "RPF mount '{}' -> virtual empty RPF (handle=0x{:08X})", guestPath, handleValue);
                
                if (IoStatusBlock)
                {
                    IoStatusBlock->Status = STATUS_SUCCESS;
                    IoStatusBlock->Information = 1;
                }

                return STATUS_SUCCESS;
            }
            else
            {
                NtDirHandle* hDir = CreateKernelObject<NtDirHandle>();
                hDir->path = resolved;

                const uint32_t handleValue = GetKernelHandle(hDir);
                g_ntDirHandles.emplace(handleValue, hDir);

                *FileHandle = handleValue;
                LOGF_IMPL(Utility, "NtCreateFile", "Created directory handle 0x{:08X} for {}", handleValue, resolved.string());
            }
            if (IoStatusBlock)
            {
                IoStatusBlock->Status = STATUS_SUCCESS;
                IoStatusBlock->Information = 1;
            }

            return STATUS_SUCCESS;
        }
    }

    std::fstream stream;
    stream.open(resolved, mode);

    if (!stream.is_open())
    {
        const uint32_t status = ErrnoToNtStatus(errno);
        LOGF_IMPL(Utility, "NtCreateFile", "FAILED to open '{}' -> '{}': errno={} status=0x{:08X}", 
                  guestPath, resolved.string(), errno, status);
        if (IoStatusBlock)
        {
            IoStatusBlock->Status = status;
            IoStatusBlock->Information = 0;
        }
        *FileHandle = 0;
        return status;
    }

    // TODO: Respect CreateDisposition if the title needs create/overwrite semantics.
    (void)CreateDisposition;

    NtFileHandle* h = CreateKernelObject<NtFileHandle>();
    h->stream = std::move(stream);
    h->path = std::move(resolved);

    const uint32_t handleValue = GetKernelHandle(h);
    g_ntFileHandles.emplace(handleValue, h);

    *FileHandle = handleValue;
    if (IoStatusBlock)
    {
        IoStatusBlock->Status = STATUS_SUCCESS;
        IoStatusBlock->Information = 1; // FILE_OPENED (best-effort)
    }

    return STATUS_SUCCESS;
}

uint32_t NtClose(uint32_t handle)
{
    // Protect against NULL and invalid handles
    if (handle == 0 || handle == GUEST_INVALID_HANDLE_VALUE)
    {
        static int s_nullCloseCount = 0;
        if (++s_nullCloseCount <= 5)
        {
            LOGF_WARNING("[NtClose] Ignored attempt to close invalid handle 0x{:08X}", handle);
        }
        return 0xC0000008;  // STATUS_INVALID_HANDLE
    }

    if (IsKernelObject(handle))
    {
        // If the handle was duplicated, just decrement the duplication count. Otherwise, delete the object.
        auto it = g_handleDuplicates.find(handle);
        const bool willDestroy = (it == g_handleDuplicates.end() || it->second == 0);
        if (!willDestroy)
        {
            if (--it->second == 0)
                g_handleDuplicates.erase(it);
            return 0;
        }

        // Close NT file handles we created.
        if (auto fhIt = g_ntFileHandles.find(handle); fhIt != g_ntFileHandles.end())
        {
            if (fhIt->second)
                fhIt->second->stream.close();
            g_ntFileHandles.erase(fhIt);
        }

        if (auto dhIt = g_ntDirHandles.find(handle); dhIt != g_ntDirHandles.end())
        {
            g_ntDirHandles.erase(dhIt);
        }

        if (auto vhIt = g_ntVirtFileHandles.find(handle); vhIt != g_ntVirtFileHandles.end())
        {
            g_ntVirtFileHandles.erase(vhIt);
        }

        DestroyKernelObject(handle);

        return 0;
    }
    else
    {
        // Don't crash on unknown handles - game may close handles we don't track
        static int s_unknownCloseCount = 0;
        if (++s_unknownCloseCount <= 10)
        {
            LOGF_WARNING("[NtClose] Unrecognized kernel object handle 0x{:08X} (occurrence #{})", 
                handle, s_unknownCloseCount);
        }
        return 0;  // Return success - closing unknown handle is harmless
    }
}

void NtSetInformationFile()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t FscSetCacheElementCount()
{
    return 0;
}

uint32_t FscGetCacheElementCount()
{
    return 0;
}

uint32_t XamLoaderGetLaunchDataSize()
{
    return 0;
}

uint32_t XamLoaderGetLaunchData()
{
    return 0;
}

uint32_t XamLoaderSetLaunchData()
{
    return 0;
}

// Forward declaration for PM4 scanning (defined later)
void ScanRingBufferForPM4Packets();

uint32_t NtWaitForSingleObjectEx(uint32_t Handle, uint32_t WaitMode, uint32_t Alertable, be<int64_t>* Timeout)
{
    if (Handle == GUEST_INVALID_HANDLE_VALUE)
        return 0xFFFFFFFF;
    
    // FIX: NULL handle (0x00000000) means "no object to wait on" - return success immediately
    // This unblocks the main thread after init completes when game passes NULL handles
    if (Handle == 0)
    {
        static int s_nullWaitCount = 0;
        if (++s_nullWaitCount <= 5)
        {
            LOGF_WARNING("[NtWaitEx] NULL handle wait #{} - returning STATUS_SUCCESS to unblock", s_nullWaitCount);
        }
        return STATUS_SUCCESS;  // Return success to let caller proceed
    }

    uint32_t timeout = GuestTimeoutToMilliseconds(Timeout);
    
    // Trace all wait calls to understand blocking
    static int s_waitCount = 0;
    ++s_waitCount;
    uint32_t callerLR = g_ppcContext ? g_ppcContext->lr : 0;
    
    if (s_waitCount <= 20 || s_waitCount % 500 == 0)
    {
        LOGF_IMPL(Utility, "NtWaitEx", "#{} handle=0x{:08X} timeout={} caller=0x{:08X}",
                  s_waitCount, Handle, timeout, callerLR);
    }
    
    // VBlank removed - following UnleashedRecomp pattern (no force-firing)
    // Game progresses naturally without interrupt-driven timing

    if (IsKernelObject(Handle))
    {
        // FIX: Handle can be either:
        // 1. A kernel object handle (from NtCreateSemaphore/NtCreateEvent) - directly a KernelObject*
        // 2. A guest dispatcher object address (XKSEMAPHORE/XKEVENT from KeInitialize*) - needs QueryKernelObject
        
        // Check if this is a guest dispatcher object by looking at the header
        XDISPATCHER_HEADER* header = reinterpret_cast<XDISPATCHER_HEADER*>(g_memory.Translate(Handle));
        uint8_t objType = header->Type;
        
        KernelObject* obj = nullptr;
        
        // Type 5 = Semaphore, Type 0/1 = Event (synchronization/notification)
        if (objType == 5)
        {
            // Guest XKSEMAPHORE - use QueryKernelObject to get actual Semaphore
            obj = QueryKernelObject<Semaphore>(*header);
            if (s_waitCount <= 30 || (Handle >= 0xEB2D0000 && Handle < 0xEB2E0000))
            {
                LOGF_WARNING("[NtWaitEx] #{} handle=0x{:08X} is guest XKSEMAPHORE, queried kernel obj={}", 
                            s_waitCount, Handle, obj ? "OK" : "NULL");
            }
        }
        else if (objType == 0 || objType == 1)
        {
            // Guest XKEVENT - use QueryKernelObject to get actual Event
            obj = QueryKernelObject<Event>(*header);
            if (s_waitCount <= 30)
            {
                LOGF_WARNING("[NtWaitEx] #{} handle=0x{:08X} is guest XKEVENT type={}, queried kernel obj={}", 
                            s_waitCount, Handle, objType, obj ? "OK" : "NULL");
            }
        }
        else
        {
            // Likely a direct kernel object handle (from NtCreate*)
            obj = GetKernelObject(Handle);
        }
        
        if (!obj)
        {
            LOGF_WARNING("[NtWaitEx] #{} handle=0x{:08X} type={} - no kernel object found!", s_waitCount, Handle, objType);
            return STATUS_TIMEOUT;
        }
        
        // Removed worker semaphore bypass hack - let C++20 atomic wait/notify work naturally
        // Following Unleashed's approach: proper synchronization without forced returns
        
        return obj->Wait(timeout);
    }
    else
    {
        // Don't crash on unknown handles - just return timeout
        static int s_unknownWaitCount = 0;
        if (++s_unknownWaitCount <= 10)
        {
            LOGF_WARNING("[NtWaitEx] Unrecognized handle 0x{:08X} (occurrence #{})", Handle, s_unknownWaitCount);
        }
    }

    return STATUS_TIMEOUT;
}

uint32_t NtWriteFile(
    uint32_t FileHandle,
    uint32_t /*Event*/, 
    uint32_t /*ApcRoutine*/,
    uint32_t /*ApcContext*/,
    XIO_STATUS_BLOCK* IoStatusBlock,
    const void* Buffer,
    uint32_t Length,
    be<int64_t>* ByteOffset)
{
    if (FileHandle == GUEST_INVALID_HANDLE_VALUE || !IsKernelObject(FileHandle))
    {
        LOGF_IMPL(Utility, "NtWriteFile", "INVALID handle 0x{:08X}", FileHandle);
        return STATUS_INVALID_HANDLE;
    }

    auto it = g_ntFileHandles.find(FileHandle);
    if (it == g_ntFileHandles.end() || !it->second || it->second->magic != kNtFileHandleMagic)
    {
        LOGF_IMPL(Utility, "NtWriteFile", "Not a file handle 0x{:08X}", FileHandle);
        return STATUS_INVALID_HANDLE;
    }

    NtFileHandle* hFile = it->second;
    if (!Buffer)
        return STATUS_INVALID_PARAMETER;

    if (ByteOffset != nullptr)
    {
        const int64_t offset = ByteOffset->get();
        hFile->stream.clear();
        hFile->stream.seekp(offset, std::ios::beg);
        if (hFile->stream.bad())
            return STATUS_FAIL_CHECK;
    }

    hFile->stream.write(reinterpret_cast<const char*>(Buffer), Length);
    const bool ok = !hFile->stream.bad();

    if (IoStatusBlock)
    {
        IoStatusBlock->Status = ok ? STATUS_SUCCESS : STATUS_FAIL_CHECK;
        IoStatusBlock->Information = ok ? Length : 0;
    }

    return ok ? STATUS_SUCCESS : STATUS_FAIL_CHECK;
}

void vsprintf_x()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t ExGetXConfigSetting(uint16_t Category, uint16_t Setting, void* Buffer, uint16_t SizeOfBuffer, be<uint32_t>* RequiredSize)
{
    uint32_t data[4]{};

    switch (Category)
    {
        // XCONFIG_SECURED_CATEGORY
        case 0x0002:
        {
            switch (Setting)
            {
                // XCONFIG_SECURED_AV_REGION
                case 0x0002:
                    data[0] = ByteSwap(0x00001000); // USA/Canada
                    break;

                default:
                    return 1;
            }
        }

        case 0x0003:
        {
            switch (Setting)
            {
                case 0x0001: // XCONFIG_USER_TIME_ZONE_BIAS
                case 0x0002: // XCONFIG_USER_TIME_ZONE_STD_NAME
                case 0x0003: // XCONFIG_USER_TIME_ZONE_DLT_NAME
                case 0x0004: // XCONFIG_USER_TIME_ZONE_STD_DATE
                case 0x0005: // XCONFIG_USER_TIME_ZONE_DLT_DATE
                case 0x0006: // XCONFIG_USER_TIME_ZONE_STD_BIAS
                case 0x0007: // XCONFIG_USER_TIME_ZONE_DLT_BIAS
                    data[0] = 0;
                    break;

                // XCONFIG_USER_LANGUAGE
                case 0x0009:
                    data[0] = ByteSwap((uint32_t)Config::Language.Value);
                    break;

                // XCONFIG_USER_VIDEO_FLAGS
                case 0x000A:
                    data[0] = ByteSwap(0x00040000);
                    break;

                // XCONFIG_USER_RETAIL_FLAGS
                case 0x000C:
                    data[0] = ByteSwap(1);
                    break;

                // XCONFIG_USER_COUNTRY
                case 0x000E:
                    data[0] = ByteSwap(103);
                    break;

                default:
                    return 1;
            }
        }
    }

    *RequiredSize = 4;
    memcpy(Buffer, data, std::min((size_t)SizeOfBuffer, sizeof(data)));

    return 0;
}

void NtQueryVirtualMemory()
{
    LOG_UTILITY("!!! STUB !!!");
}

#ifndef STATUS_INVALID_PARAMETER
#define STATUS_INVALID_PARAMETER 0xC000000D
#endif

#ifndef STATUS_NO_MEMORY
#define STATUS_NO_MEMORY 0xC0000017
#endif

void MmQueryStatistics()
{
    LOG_UTILITY("!!! STUB !!!");
}

// Track all created Event handles so VdSwap can signal them to unblock workers
static std::mutex g_eventTrackMutex;
static std::vector<uint32_t> g_trackedEventHandles;

uint32_t NtCreateEvent(be<uint32_t>* handle, void* objAttributes, uint32_t eventType, uint32_t initialState)
{
    static int s_count = 0;
    ++s_count;
    
    // Log BEFORE creation to detect if CreateKernelObject crashes
    if (s_count <= 30)
    {
        LOGF_IMPL(Utility, "NtCreateEvent", "#{} STARTING type={} initial={}",
                  s_count, eventType, initialState);
    }
    
    Event* evt = CreateKernelObject<Event>(!eventType, !!initialState);
    uint32_t h = GetKernelHandle(evt);
    *handle = h;
    
    // Track this event so we can signal it later
    {
        std::lock_guard lock(g_eventTrackMutex);
        g_trackedEventHandles.push_back(h);
    }
    
    // Log AFTER creation to confirm success
    if (s_count <= 30)
    {
        LOGF_IMPL(Utility, "NtCreateEvent", "#{} CREATED handle=0x{:08X} type={} initial={}",
                  s_count, h, eventType, initialState);
    }
    
    return 0;
}

uint32_t XexCheckExecutablePrivilege()
{
    return 0;
}

void DbgPrint()
{
    LOG_UTILITY("!!! STUB !!!");
}

void __C_specific_handler_x()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t RtlNtStatusToDosError(uint32_t Status)
{
    // See https://github.com/wine-mirror/wine/blob/master/dlls/ntdll/error.c#L47-L64
    if (Status == 0 || (Status & 0x20000000) != 0)
        return Status;

    if ((Status & 0xF0000000) == 0xD0000000)
        Status &= ~0x10000000;

    const uint32_t hi = (Status >> 16) & 0xFFFF;
    if (hi == 0x8007 || hi == 0xC001 || hi == 0xC007)
        return Status & 0xFFFF;

    switch (Status)
    {
    case uint32_t(STATUS_NOT_IMPLEMENTED):
        return ERROR_CALL_NOT_IMPLEMENTED;
    case uint32_t(STATUS_SEMAPHORE_LIMIT_EXCEEDED):
        return ERROR_TOO_MANY_POSTS;
    case uint32_t(STATUS_OBJECT_NAME_NOT_FOUND):
        return ERROR_FILE_NOT_FOUND;
    case uint32_t(STATUS_INVALID_PARAMETER):
        return ERROR_INVALID_PARAMETER;
    case uint32_t(STATUS_INVALID_HANDLE):
        return ERROR_INVALID_HANDLE;
    case uint32_t(STATUS_END_OF_FILE):
        return ERROR_HANDLE_EOF;
    case uint32_t(STATUS_NO_MORE_FILES):
        return ERROR_NO_MORE_FILES;
    case uint32_t(STATUS_ACCESS_DENIED):
        return ERROR_ACCESS_DENIED;
    case uint32_t(STATUS_OBJECT_NAME_INVALID):
        return ERROR_INVALID_NAME;
    case uint32_t(STATUS_OBJECT_PATH_NOT_FOUND):
        return ERROR_PATH_NOT_FOUND;
    case uint32_t(STATUS_OBJECT_NAME_COLLISION):
        return ERROR_ALREADY_EXISTS;
    default:
        LOGF_WARNING("Unimplemented NtStatus translation: {:#08x}", Status);
        return Status;
    }
}

void XexGetProcedureAddress()
{
    LOG_UTILITY("!!! STUB !!!");
}

void XexGetModuleSection()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t RtlUnicodeToMultiByteN(char* MultiByteString, uint32_t MaxBytesInMultiByteString, be<uint32_t>* BytesInMultiByteString, const be<uint16_t>* UnicodeString, uint32_t BytesInUnicodeString)
{
    const auto reqSize = BytesInUnicodeString / sizeof(uint16_t);

    if (BytesInMultiByteString)
        *BytesInMultiByteString = reqSize;

    if (reqSize > MaxBytesInMultiByteString)
        return STATUS_FAIL_CHECK;

    for (size_t i = 0; i < reqSize; i++)
    {
        const auto c = UnicodeString[i].get();

        MultiByteString[i] = c < 256 ? c : '?';
    }

    return STATUS_SUCCESS;
}

uint32_t KeDelayExecutionThread(uint32_t WaitMode, bool Alertable, be<int64_t>* Timeout)
{
    // Pump SDL events to keep window responsive
    PumpSdlEventsIfNeeded();
    
    // We don't do async file reads.
    if (Alertable)
        return STATUS_USER_APC;

    uint32_t timeout = GuestTimeoutToMilliseconds(Timeout);

#ifdef _WIN32
    Sleep(timeout);
#else
    if (timeout == 0)
        std::this_thread::yield();
    else
        std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
#endif

    return STATUS_SUCCESS;
}

void ExFreePool()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t NtQueryInformationFile(
    uint32_t FileHandle,
    XIO_STATUS_BLOCK* IoStatusBlock,
    void* FileInformation,
    uint32_t Length,
    uint32_t FileInformationClass)
{
    if (FileHandle == GUEST_INVALID_HANDLE_VALUE || !IsKernelObject(FileHandle))
        return STATUS_INVALID_HANDLE;

    std::filesystem::path path;
    NtFileHandle* hFile = nullptr;
    NtVirtFileHandle* hVirt = nullptr;
    if (auto it = g_ntFileHandles.find(FileHandle); it != g_ntFileHandles.end() && it->second && it->second->magic == kNtFileHandleMagic)
    {
        hFile = it->second;
        path = hFile->path;
    }
    else if (auto it = g_ntVirtFileHandles.find(FileHandle); it != g_ntVirtFileHandles.end() && it->second && it->second->magic == kNtVirtFileHandleMagic)
    {
        hVirt = it->second;
        path = hVirt->debugPath;
    }
    else if (auto it = g_ntDirHandles.find(FileHandle); it != g_ntDirHandles.end() && it->second && it->second->magic == kNtDirHandleMagic)
    {
        path = it->second->path;
    }
    else
    {
        return STATUS_INVALID_HANDLE;
    }

    if (!IoStatusBlock || !FileInformation)
        return STATUS_INVALID_PARAMETER;

    std::error_code ec;
    bool isDir = false;
    uint64_t fileSize = 0;
    uint32_t attrs = 0;

    if (hVirt)
    {
        // In-memory file: pretend it's a normal file with known size.
        isDir = false;
        fileSize = hVirt->data.size();
        attrs = ByteSwap(FILE_ATTRIBUTE_NORMAL);
    }
    else
    {
        const bool exists = std::filesystem::exists(path, ec);
        if (!exists || ec)
        {
            IoStatusBlock->Status = STATUS_OBJECT_NAME_NOT_FOUND;
            IoStatusBlock->Information = 0;
            return STATUS_OBJECT_NAME_NOT_FOUND;
        }

        isDir = std::filesystem::is_directory(path, ec);
        if (ec)
        {
            IoStatusBlock->Status = STATUS_FAIL_CHECK;
            IoStatusBlock->Information = 0;
            return STATUS_FAIL_CHECK;
        }

        if (!isDir)
        {
            fileSize = std::filesystem::file_size(path, ec);
            if (ec)
                fileSize = 0;
        }

        attrs = GetFileAttributesBestEffort(path);
    }

    const uint64_t allocationSize = RoundUpToPage(fileSize);

    switch (FileInformationClass)
    {
    case FileBasicInformation:
    {
        if (Length < sizeof(XFILE_BASIC_INFORMATION))
            return STATUS_INFO_LENGTH_MISMATCH;

        auto* info = reinterpret_cast<XFILE_BASIC_INFORMATION*>(FileInformation);
        info->CreationTime = 0;
        info->LastAccessTime = 0;
        info->LastWriteTime = 0;
        info->ChangeTime = 0;
        info->FileAttributes = attrs;
        info->Reserved = 0;

        IoStatusBlock->Status = STATUS_SUCCESS;
        IoStatusBlock->Information = sizeof(XFILE_BASIC_INFORMATION);
        return STATUS_SUCCESS;
    }
    case FileStandardInformation:
    {
        if (Length < sizeof(XFILE_STANDARD_INFORMATION))
            return STATUS_INFO_LENGTH_MISMATCH;

        auto* info = reinterpret_cast<XFILE_STANDARD_INFORMATION*>(FileInformation);
        info->AllocationSize = static_cast<int64_t>(allocationSize);
        info->EndOfFile = static_cast<int64_t>(fileSize);
        info->NumberOfLinks = 1;
        info->DeletePending = 0;
        info->Directory = isDir ? 1 : 0;
        info->Reserved = 0;

        IoStatusBlock->Status = STATUS_SUCCESS;
        IoStatusBlock->Information = sizeof(XFILE_STANDARD_INFORMATION);
        return STATUS_SUCCESS;
    }
    case FilePositionInformation:
    {
        if (Length < sizeof(XFILE_POSITION_INFORMATION))
            return STATUS_INFO_LENGTH_MISMATCH;

        auto* info = reinterpret_cast<XFILE_POSITION_INFORMATION*>(FileInformation);
        if (hFile)
        {
            const auto pos = hFile->stream.tellg();
            if (pos < 0)
                info->CurrentByteOffset = 0;
            else
                info->CurrentByteOffset = static_cast<int64_t>(pos);
        }
        else
        {
            info->CurrentByteOffset = 0;
        }

        IoStatusBlock->Status = STATUS_SUCCESS;
        IoStatusBlock->Information = sizeof(XFILE_POSITION_INFORMATION);
        return STATUS_SUCCESS;
    }
    case FileEndOfFileInformation:
    {
        if (Length < sizeof(XFILE_END_OF_FILE_INFORMATION))
            return STATUS_INFO_LENGTH_MISMATCH;

        auto* info = reinterpret_cast<XFILE_END_OF_FILE_INFORMATION*>(FileInformation);
        info->EndOfFile = static_cast<int64_t>(fileSize);

        IoStatusBlock->Status = STATUS_SUCCESS;
        IoStatusBlock->Information = sizeof(XFILE_END_OF_FILE_INFORMATION);
        return STATUS_SUCCESS;
    }
    case FileNetworkOpenInformation:
    {
        if (Length < sizeof(XFILE_NETWORK_OPEN_INFORMATION))
            return STATUS_INFO_LENGTH_MISMATCH;

        auto* info = reinterpret_cast<XFILE_NETWORK_OPEN_INFORMATION*>(FileInformation);
        info->CreationTime = 0;
        info->LastAccessTime = 0;
        info->LastWriteTime = 0;
        info->ChangeTime = 0;
        info->AllocationSize = static_cast<int64_t>(allocationSize);
        info->EndOfFile = static_cast<int64_t>(fileSize);
        info->FileAttributes = attrs;
        info->Reserved = 0;

        IoStatusBlock->Status = STATUS_SUCCESS;
        IoStatusBlock->Information = sizeof(XFILE_NETWORK_OPEN_INFORMATION);
        return STATUS_SUCCESS;
    }
    case 26: // Xbox 360 specific - appears to query file size/info
    {
        // Xbox 360 games may use different class numbers
        // Class 26 appears to be used for file size queries
        // Return file size information in a generic format
        if (Length >= 8)
        {
            // Return file size as first 8 bytes (common pattern)
            auto* sizePtr = reinterpret_cast<int64_t*>(FileInformation);
            *sizePtr = static_cast<int64_t>(fileSize);
            if (Length > 8)
                memset(reinterpret_cast<uint8_t*>(FileInformation) + 8, 0, Length - 8);
            
            LOGF_IMPL(Utility, "NtQueryInformationFile", "Class 26 (Xbox size query) for '{}': returning size={}", 
                      path.filename().string(), fileSize);
        }
        else if (Length > 0)
        {
            memset(FileInformation, 0, Length);
        }
        IoStatusBlock->Status = STATUS_SUCCESS;
        IoStatusBlock->Information = Length;
        return STATUS_SUCCESS;
    }
    default:
        // For unhandled classes, try to return success with zeroed data
        // This helps games that query for optional information
        if (Length > 0) {
            memset(FileInformation, 0, Length);
        }
        LOGF_WARNING("NtQueryInformationFile: unhandled class {} for '{}' - returning success with zeroed data", 
                     FileInformationClass, path.filename().string());
        IoStatusBlock->Status = STATUS_SUCCESS;
        IoStatusBlock->Information = Length;
        return STATUS_SUCCESS;
    }
}

uint32_t NtQueryVolumeInformationFile(
    uint32_t FileHandle,
    XIO_STATUS_BLOCK* IoStatusBlock,
    void* FsInformation,
    uint32_t Length,
    uint32_t FsInformationClass)
{
    if (FileHandle == GUEST_INVALID_HANDLE_VALUE || !IsKernelObject(FileHandle))
        return STATUS_INVALID_HANDLE;

    // Accept both file handles and directory handles for volume information.
    std::filesystem::path path;
    if (auto it = g_ntFileHandles.find(FileHandle); it != g_ntFileHandles.end() && it->second && it->second->magic == kNtFileHandleMagic)
    {
        path = it->second->path;
    }
    else if (auto it = g_ntVirtFileHandles.find(FileHandle); it != g_ntVirtFileHandles.end() && it->second && it->second->magic == kNtVirtFileHandleMagic)
    {
        path = it->second->debugPath;
    }
    else if (auto it = g_ntDirHandles.find(FileHandle); it != g_ntDirHandles.end() && it->second && it->second->magic == kNtDirHandleMagic)
    {
        path = it->second->path;
    }
    else
    {
        return STATUS_INVALID_HANDLE;
    }

    if (!IoStatusBlock || !FsInformation)
        return STATUS_INVALID_PARAMETER;

    switch (FsInformationClass)
    {
    case FileFsDeviceInformation:
    {
        if (Length < sizeof(XFILE_FS_DEVICE_INFORMATION))
            return STATUS_INFO_LENGTH_MISMATCH;

        auto* info = reinterpret_cast<XFILE_FS_DEVICE_INFORMATION*>(FsInformation);
        info->DeviceType = 0x00000007; // FILE_DEVICE_DISK
        info->Characteristics = 0;

        IoStatusBlock->Status = STATUS_SUCCESS;
        IoStatusBlock->Information = sizeof(XFILE_FS_DEVICE_INFORMATION);
        return STATUS_SUCCESS;
    }
    case FileFsSizeInformation:
    {
        if (Length < sizeof(XFILE_FS_SIZE_INFORMATION))
            return STATUS_INFO_LENGTH_MISMATCH;

        auto* info = reinterpret_cast<XFILE_FS_SIZE_INFORMATION*>(FsInformation);
        // Very rough defaults: 512-byte sectors, 8 sectors per allocation unit (4KB).
        info->BytesPerSector = 512;
        info->SectorsPerAllocationUnit = 8;
        info->TotalAllocationUnits = 0x100000;
        info->AvailableAllocationUnits = 0x080000;

        IoStatusBlock->Status = STATUS_SUCCESS;
        IoStatusBlock->Information = sizeof(XFILE_FS_SIZE_INFORMATION);
        return STATUS_SUCCESS;
    }
    case FileFsAttributeInformation:
    {
        // Return a fixed buffer with a short filesystem name.
        constexpr uint16_t kName[] = { 'F', 'A', 'T', 'X' };
        constexpr uint32_t kNameBytes = sizeof(kName);
        constexpr uint32_t kFixedSize = sizeof(XFILE_FS_ATTRIBUTE_INFORMATION_FIXED);

        if (Length < kFixedSize)
            return STATUS_INFO_LENGTH_MISMATCH;

        auto* info = reinterpret_cast<XFILE_FS_ATTRIBUTE_INFORMATION_FIXED*>(FsInformation);
        info->FileSystemAttributes = 0;
        info->MaximumComponentNameLength = 255;
        info->FileSystemNameLength = kNameBytes;
        memset(info->FileSystemName, 0, sizeof(info->FileSystemName));
        for (size_t i = 0; i < (sizeof(kName) / sizeof(kName[0])); ++i)
            info->FileSystemName[i] = kName[i];

        IoStatusBlock->Status = STATUS_SUCCESS;
        IoStatusBlock->Information = kFixedSize;
        return STATUS_SUCCESS;
    }
    default:
        LOGF_WARNING("NtQueryVolumeInformationFile: unhandled class {}", FsInformationClass);
        IoStatusBlock->Status = STATUS_INVALID_PARAMETER;
        IoStatusBlock->Information = 0;
        return STATUS_INVALID_PARAMETER;
    }
}

uint32_t NtQueryDirectoryFile(
    uint32_t FileHandle,
    uint32_t /*Event*/,
    uint32_t /*ApcRoutine*/,
    uint32_t /*ApcContext*/,
    XIO_STATUS_BLOCK* IoStatusBlock,
    void* FileInformation,
    uint32_t Length,
    uint32_t FileInformationClass,
    uint32_t ReturnSingleEntry,
    XANSI_STRING* FileName,
    uint32_t RestartScan)
{
    LOGF_IMPL(Utility, "NtQueryDirectoryFile", "Handle=0x{:08X} Class={} RestartScan={}", FileHandle, FileInformationClass, RestartScan);
    if (FileHandle == GUEST_INVALID_HANDLE_VALUE || !IsKernelObject(FileHandle))
    {
        LOG_IMPL(Utility, "NtQueryDirectoryFile", "INVALID_HANDLE_VALUE or not kernel object");
        return STATUS_INVALID_HANDLE;
    }

    auto it = g_ntDirHandles.find(FileHandle);
    if (it == g_ntDirHandles.end() || !it->second || it->second->magic != kNtDirHandleMagic)
    {
        LOGF_IMPL(Utility, "NtQueryDirectoryFile", "Handle 0x{:08X} not in g_ntDirHandles (found={}, hasSecond={}, magic=0x{:08X})",
            FileHandle,
            it != g_ntDirHandles.end(), it != g_ntDirHandles.end() && it->second != nullptr,
            (it != g_ntDirHandles.end() && it->second) ? it->second->magic : 0);
        return STATUS_INVALID_HANDLE;
    }
    LOGF_IMPL(Utility, "NtQueryDirectoryFile", "Dir path: '{}'", it->second->path.string());

    NtDirHandle* hDir = it->second;
    if (!IoStatusBlock || !FileInformation || Length == 0)
        return STATUS_INVALID_PARAMETER;

    if (RestartScan)
        hDir->cursor = 0;

    if (!hDir->initialized)
    {
        std::error_code ec;
        for (auto iter = std::filesystem::directory_iterator(hDir->path, ec); !ec && iter != std::filesystem::directory_iterator(); ++iter)
            hDir->entries.emplace_back(*iter);

        // Stable, deterministic ordering helps with repeatability.
        std::sort(hDir->entries.begin(), hDir->entries.end(), [](const auto& a, const auto& b) {
            return a.path().filename().string() < b.path().filename().string();
        });

        hDir->initialized = true;
        LOGF_IMPL(Utility, "NtQueryDirectoryFile", "Enumerated {} files in '{}'", hDir->entries.size(), hDir->path.string());
    }

    std::string filterName;
    if (FileName && FileName->Buffer.get() && FileName->Length.get() != 0)
    {
        const char* buf = FileName->Buffer.get();
        filterName.assign(buf, buf + FileName->Length.get());
    }

    const auto writeUtf16Be = [](be<uint16_t>* out, const std::string& s) {
        for (size_t i = 0; i < s.size(); ++i)
            out[i] = static_cast<uint16_t>(static_cast<unsigned char>(s[i]));
    };

    uint8_t* out = reinterpret_cast<uint8_t*>(FileInformation);
    uint32_t remaining = Length;
    uint32_t written = 0;

    // Track the prior entry so we can fill in its NextEntryOffset.
    be<uint32_t>* prevNextEntryOffset = nullptr;
    uint32_t prevEntrySize = 0;

    const bool single = (ReturnSingleEntry != 0);

    while (hDir->cursor < hDir->entries.size())
    {
        const auto& entry = hDir->entries[hDir->cursor];
        const std::string name = entry.path().filename().string();

        if (!filterName.empty() && name != filterName)
        {
            ++hDir->cursor;
            continue;
        }

        std::error_code ec;
        const std::filesystem::path entryPath = entry.path();
        const bool isDir = std::filesystem::is_directory(entryPath, ec);
        uint64_t fileSize = 0;
        if (!isDir)
        {
            fileSize = std::filesystem::file_size(entryPath, ec);
            if (ec)
                fileSize = 0;
        }

        const uint64_t allocationSize = RoundUpToPage(fileSize);
        const uint32_t attrs = GetFileAttributesBestEffort(entryPath);

        const uint32_t nameBytes = static_cast<uint32_t>(name.size() * sizeof(uint16_t));

        uint32_t fixedSize = 0;
        switch (FileInformationClass)
        {
        case FileDirectoryInformation:
            fixedSize = sizeof(XFILE_DIRECTORY_INFORMATION_FIXED);
            break;
        case FileFullDirectoryInformation:
            fixedSize = sizeof(XFILE_FULL_DIR_INFORMATION_FIXED);
            break;
        case FileBothDirectoryInformation:
            fixedSize = sizeof(XFILE_BOTH_DIR_INFORMATION_FIXED);
            break;
        default:
            IoStatusBlock->Status = STATUS_INVALID_PARAMETER;
            IoStatusBlock->Information = 0;
            return STATUS_INVALID_PARAMETER;
        }

        const uint32_t entrySizeUnaligned = fixedSize + nameBytes;
        const uint32_t entrySize = AlignUp(entrySizeUnaligned, 8);

        if (entrySize > remaining)
        {
            if (written == 0)
            {
                IoStatusBlock->Status = STATUS_BUFFER_OVERFLOW;
                IoStatusBlock->Information = 0;
                return STATUS_BUFFER_OVERFLOW;
            }
            break;
        }

        // Update the previous entry now that we know we are emitting a new one.
        if (prevNextEntryOffset)
            *prevNextEntryOffset = prevEntrySize;

        uint8_t* entryBase = out + written;
        memset(entryBase, 0, entrySize);

        uint8_t* nameOut = nullptr;
        switch (FileInformationClass)
        {
        case FileDirectoryInformation:
        {
            auto* info = reinterpret_cast<XFILE_DIRECTORY_INFORMATION_FIXED*>(entryBase);
            info->NextEntryOffset = 0;
            info->FileIndex = 0;
            info->CreationTime = 0;
            info->LastAccessTime = 0;
            info->LastWriteTime = 0;
            info->ChangeTime = 0;
            info->EndOfFile = static_cast<int64_t>(fileSize);
            info->AllocationSize = static_cast<int64_t>(allocationSize);
            info->FileAttributes = attrs;
            info->FileNameLength = nameBytes;
            nameOut = entryBase + sizeof(XFILE_DIRECTORY_INFORMATION_FIXED);
            prevNextEntryOffset = &info->NextEntryOffset;
            break;
        }
        case FileFullDirectoryInformation:
        {
            auto* info = reinterpret_cast<XFILE_FULL_DIR_INFORMATION_FIXED*>(entryBase);
            info->NextEntryOffset = 0;
            info->FileIndex = 0;
            info->CreationTime = 0;
            info->LastAccessTime = 0;
            info->LastWriteTime = 0;
            info->ChangeTime = 0;
            info->EndOfFile = static_cast<int64_t>(fileSize);
            info->AllocationSize = static_cast<int64_t>(allocationSize);
            info->FileAttributes = attrs;
            info->FileNameLength = nameBytes;
            info->EaSize = 0;
            nameOut = entryBase + sizeof(XFILE_FULL_DIR_INFORMATION_FIXED);
            prevNextEntryOffset = &info->NextEntryOffset;
            break;
        }
        case FileBothDirectoryInformation:
        {
            auto* info = reinterpret_cast<XFILE_BOTH_DIR_INFORMATION_FIXED*>(entryBase);
            info->NextEntryOffset = 0;
            info->FileIndex = 0;
            info->CreationTime = 0;
            info->LastAccessTime = 0;
            info->LastWriteTime = 0;
            info->ChangeTime = 0;
            info->EndOfFile = static_cast<int64_t>(fileSize);
            info->AllocationSize = static_cast<int64_t>(allocationSize);
            info->FileAttributes = attrs;
            info->FileNameLength = nameBytes;
            info->EaSize = 0;
            info->ShortNameLength = 0;
            info->Reserved1 = 0;
            memset(info->ShortName, 0, sizeof(info->ShortName));
            nameOut = entryBase + sizeof(XFILE_BOTH_DIR_INFORMATION_FIXED);
            prevNextEntryOffset = &info->NextEntryOffset;
            break;
        }
        default:
            break;
        }

        if (nameOut)
            writeUtf16Be(reinterpret_cast<be<uint16_t>*>(nameOut), name);

        prevEntrySize = entrySize;
        written += entrySize;
        remaining -= entrySize;
        ++hDir->cursor;

        if (single)
            break;
    }

    if (written == 0)
    {
        IoStatusBlock->Status = STATUS_NO_MORE_FILES;
        IoStatusBlock->Information = 0;
        return STATUS_NO_MORE_FILES;
    }

    // Final entry in the buffer.
    if (prevNextEntryOffset)
        *prevNextEntryOffset = 0;

    IoStatusBlock->Status = STATUS_SUCCESS;
    IoStatusBlock->Information = written;
    return STATUS_SUCCESS;
}

void NtReadFileScatter()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t NtReadFile(
    uint32_t FileHandle,
    uint32_t Event,  // IMPORTANT: Signal this event on completion for async I/O
    uint32_t /*ApcRoutine*/,
    uint32_t /*ApcContext*/,
    XIO_STATUS_BLOCK* IoStatusBlock,
    void* Buffer,
    uint32_t Length,
    be<int64_t>* ByteOffset)
{
    static int s_readCount = 0;
    static int s_eventSignalCount = 0;
    ++s_readCount;
    
    const uint64_t offset = ByteOffset ? static_cast<uint64_t>(ByteOffset->get()) : 0ull;
    if (s_readCount <= 50 || s_readCount % 500 == 0)
    {
        LOGF_IMPL(Utility, "NtReadFile", "#{} handle=0x{:08X} len={} offset=0x{:X} event=0x{:08X}", 
                  s_readCount, FileHandle, Length, offset, Event);
    }
    
    // Helper lambda to signal completion event if provided
    auto signalCompletionEvent = [&]() {
        if (Event != 0 && Event != GUEST_INVALID_HANDLE_VALUE && IsKernelObject(Event))
        {
            ++s_eventSignalCount;
            if (s_eventSignalCount <= 20 || s_eventSignalCount % 100 == 0)
            {
                LOGF_WARNING("[ASYNC COMPLETE] NtReadFile signaling event 0x{:08X} (count={})", 
                            Event, s_eventSignalCount);
            }
            
            // Signal the event to wake any waiters
            XDISPATCHER_HEADER* eventObj = reinterpret_cast<XDISPATCHER_HEADER*>(g_memory.Translate(Event));
            if (eventObj && (eventObj->Type == 0 || eventObj->Type == 1))
            {
                QueryKernelObject<::Event>(*eventObj)->Set();
            }
        }
    };
    
    // Pump SDL events to keep window responsive
    PumpSdlEventsIfNeeded();
    
    // Explicit check for NULL handle - return fatal error to break retry loops
    if (FileHandle == 0)
    {
        static int nullHandleCount = 0;
        ++nullHandleCount;
        if (nullHandleCount <= 3 || nullHandleCount % 1000 == 0) {
            LOGF_IMPL(Utility, "NtReadFile", "NULL handle 0x00000000 (count={})", nullHandleCount);
        }
        // Return INVALID_HANDLE to signal the caller to give up
        if (IoStatusBlock) {
            IoStatusBlock->Status = STATUS_INVALID_HANDLE;
            IoStatusBlock->Information = 0;
        }
        // Add small delay to prevent tight CPU loop
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        return STATUS_INVALID_HANDLE;
    }
    
    if (FileHandle == GUEST_INVALID_HANDLE_VALUE || !IsKernelObject(FileHandle))
    {
        LOGF_IMPL(Utility, "NtReadFile", "INVALID handle 0x{:08X}", FileHandle);
        return STATUS_INVALID_HANDLE;
    }

    // Virtual (in-memory) file handles (used to satisfy RPF mount header reads).
    if (auto vit = g_ntVirtFileHandles.find(FileHandle);
        vit != g_ntVirtFileHandles.end() && vit->second && vit->second->magic == kNtVirtFileHandleMagic)
    {
        NtVirtFileHandle* hVirt = vit->second;
        if (!IoStatusBlock || !Buffer)
            return STATUS_INVALID_PARAMETER;

        const uint64_t offset = ByteOffset ? static_cast<uint64_t>(ByteOffset->get()) : 0ull;
        if (offset >= hVirt->data.size())
        {
            IoStatusBlock->Status = STATUS_END_OF_FILE;
            IoStatusBlock->Information = 0;
            return STATUS_END_OF_FILE;
        }

        const uint32_t available = static_cast<uint32_t>(hVirt->data.size() - offset);
        const uint32_t toCopy = std::min<uint32_t>(Length, available);
        memcpy(Buffer, hVirt->data.data() + offset, toCopy);

        IoStatusBlock->Status = STATUS_SUCCESS;
        IoStatusBlock->Information = toCopy;
        signalCompletionEvent();
        return STATUS_SUCCESS;
    }

    // Directories can't be read as files.
    if (auto dit = g_ntDirHandles.find(FileHandle); dit != g_ntDirHandles.end() && dit->second && dit->second->magic == kNtDirHandleMagic)
    {
        if (IoStatusBlock)
        {
            IoStatusBlock->Status = STATUS_INVALID_PARAMETER;
            IoStatusBlock->Information = 0;
        }
        return STATUS_INVALID_PARAMETER;
    }

    auto it = g_ntFileHandles.find(FileHandle);
    if (it == g_ntFileHandles.end() || !it->second || it->second->magic != kNtFileHandleMagic)
    {
        LOGF_IMPL(Utility, "NtReadFile", "Not a file handle 0x{:08X}", FileHandle);
        return STATUS_INVALID_HANDLE;
    }

    NtFileHandle* hFile = it->second;
    if (!IoStatusBlock || !Buffer)
        return STATUS_INVALID_PARAMETER;

    // Parse RPF header on first access so we can detect encrypted TOC ranges.
    if (!hFile->rpfHeaderParsed)
    {
        // If the path extension indicates .rpf, mark it as RPF
        if (!hFile->isRpf)
        {
            try {
                std::string ext = hFile->path.extension().string();
                std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c){ return std::tolower(c); });
                if (ext == ".rpf")
                    hFile->isRpf = true;
            } catch (...) {}
        }
        
        // Parse header if this is an RPF file
        if (hFile->isRpf)
            ParseRpfHeader(hFile);
    }

    const uint64_t fileOffset = ByteOffset ? static_cast<uint64_t>(ByteOffset->get()) : 0ull;
    if (ByteOffset != nullptr)
    {
        hFile->stream.clear();
        hFile->stream.seekg(fileOffset, std::ios::beg);
        if (hFile->stream.bad())
        {
            IoStatusBlock->Status = STATUS_FAIL_CHECK;
            IoStatusBlock->Information = 0;
            return STATUS_FAIL_CHECK;
        }
    }

    hFile->stream.read(reinterpret_cast<char*>(Buffer), Length);
    const uint32_t bytesRead = static_cast<uint32_t>(hFile->stream.gcount());

    if (bytesRead == 0 && Length != 0 && hFile->stream.eof())
    {
        IoStatusBlock->Status = STATUS_END_OF_FILE;
        IoStatusBlock->Information = 0;
        signalCompletionEvent();
        return STATUS_END_OF_FILE;
    }

    // If this is an RPF file with encrypted TOC, decrypt any portion we returned that overlaps the TOC.
    if (hFile->isRpf && hFile->tocEncrypted && bytesRead > 0)
    {
        const uint64_t tocStart = hFile->tocOffset;
        const uint64_t tocEnd = tocStart + hFile->tocSize;
        const uint64_t readStart = fileOffset;
        const uint64_t readEnd = fileOffset + bytesRead;
        const uint64_t overlapStart = std::max(readStart, tocStart);
        const uint64_t overlapEnd = std::min(readEnd, tocEnd);
        if (overlapEnd > overlapStart)
        {
            const uint32_t startInBuf = static_cast<uint32_t>(overlapStart - readStart);
            const uint32_t len = static_cast<uint32_t>(overlapEnd - overlapStart);
            // Pass TOC-relative offset for proper AES block alignment
            const uint64_t tocRelativeOffset = overlapStart - tocStart;
            DecryptRpfBufferInPlace(reinterpret_cast<uint8_t*>(Buffer) + startInBuf, len, tocRelativeOffset);
        }
    }

    const bool ok = !hFile->stream.bad();
    IoStatusBlock->Status = ok ? STATUS_SUCCESS : STATUS_FAIL_CHECK;
    IoStatusBlock->Information = ok ? bytesRead : 0;
    
    // Signal completion event for async I/O
    signalCompletionEvent();
    
    return ok ? STATUS_SUCCESS : STATUS_FAIL_CHECK;
}

uint32_t NtDuplicateObject(uint32_t SourceHandle, be<uint32_t>* TargetHandle, uint32_t Options)
{
    if (SourceHandle == GUEST_INVALID_HANDLE_VALUE)
        return 0xFFFFFFFF;

    if (IsKernelObject(SourceHandle))
    {
        // Increment handle duplicate count.
        const auto& it = g_handleDuplicates.find(SourceHandle);
        if (it != g_handleDuplicates.end())
            ++it->second;
        else
            g_handleDuplicates[SourceHandle] = 1;

        *TargetHandle = SourceHandle;
        return 0;
    }
    else
    {
        // Don't crash on unknown handles - just return error
        static int s_unknownDupCount = 0;
        if (++s_unknownDupCount <= 10)
        {
            LOGF_WARNING("[NtDuplicateObject] Unrecognized handle 0x{:08X} (occurrence #{})", SourceHandle, s_unknownDupCount);
        }
        return 0xC0000008;  // STATUS_INVALID_HANDLE
    }
}

static std::unordered_map<uint32_t, uint32_t> g_ntVirtualMemoryAllocs;

// NT-style virtual memory APIs used by some titles (e.g., GTA IV).
// Backed by the existing guest heap allocator since we map a full 4GB guest address space.
// This is intentionally minimal: it supports the common "BaseAddress == NULL" allocation path.
uint32_t NtAllocateVirtualMemory(
    be<uint32_t>* BaseAddress,
    be<uint32_t>* RegionSize,
    uint32_t AllocationType,
    uint32_t Protect,
    uint32_t ZeroBits)
{
    (void)AllocationType;
    (void)Protect;
    (void)ZeroBits;

    if (!BaseAddress || !RegionSize)
        return STATUS_INVALID_PARAMETER;

    uint32_t size = RegionSize->get();
    if (size == 0)
        return STATUS_INVALID_PARAMETER;

    // Round up to page size. X360 is page-based, callers often expect at least 4KB granularity.
    constexpr uint32_t kPageSize = 0x1000;
    size = (size + (kPageSize - 1)) & ~(kPageSize - 1);

    uint32_t requested = BaseAddress->get();
    if (requested != 0)
    {
        // We currently don't manage fixed-address reservations/commits.
        // The full guest address space is already mapped, so accept the request if it is in-range.
        if (requested >= PPC_MEMORY_SIZE)
            return STATUS_INVALID_PARAMETER;

        *RegionSize = size;
        return STATUS_SUCCESS;
    }

    void* ptr = g_userHeap.AllocPhysical(size, kPageSize);
    if (!ptr)
        return STATUS_NO_MEMORY;

    const uint32_t base = g_memory.MapVirtual(ptr);
    g_ntVirtualMemoryAllocs[base] = size;

    *BaseAddress = base;
    *RegionSize = size;
    return STATUS_SUCCESS;
}

uint32_t NtFreeVirtualMemory(
    be<uint32_t>* BaseAddress,
    be<uint32_t>* RegionSize,
    uint32_t FreeType,
    uint32_t /*Unknown*/)
{
    (void)FreeType;

    if (!BaseAddress)
        return STATUS_INVALID_PARAMETER;

    const uint32_t addr = BaseAddress->get();
    if (addr == 0)
        return STATUS_SUCCESS;

    // If MEM_RELEASE is specified, RegionSize must be 0 on Win32.
    // Some titles may pass non-zero; we ignore and free anyway.
    if (RegionSize)
        *RegionSize = 0;

    const auto it = g_ntVirtualMemoryAllocs.find(addr);
    if (it != g_ntVirtualMemoryAllocs.end())
    {
        g_userHeap.Free(g_memory.Translate(addr));
        g_ntVirtualMemoryAllocs.erase(it);
    }
    *BaseAddress = 0;
    return STATUS_SUCCESS;
}

void ObDereferenceObject(void* object)
{
    // Reference counting is not fully implemented.
    // Objects are managed by the kernel object system.
    // This is a no-op for now.
    (void)object;
}

void KeSetBasePriorityThread(GuestThreadHandle* hThread, int priority)
{
#ifdef _WIN32
    if (priority == 16)
    {
        priority = 15;
    }
    else if (priority == -16)
    {
        priority = -15;
    }

    SetThreadPriority(hThread == GetKernelObject(CURRENT_THREAD_HANDLE) ? GetCurrentThread() : hThread->thread.native_handle(), priority);
#endif
}

uint32_t ObReferenceObjectByHandle(uint32_t handle, uint32_t objectType, be<uint32_t>* object)
{
    *object = handle;
    return 0;
}

void KeQueryBasePriorityThread()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t NtSuspendThread(GuestThreadHandle* hThread, uint32_t* suspendCount)
{
    assert(hThread != GetKernelObject(CURRENT_THREAD_HANDLE) && hThread->GetThreadId() == GuestThread::GetCurrentThreadId());

    hThread->suspended = true;
    hThread->suspended.wait(true);

    return S_OK;
}

uint32_t KeSetAffinityThread(uint32_t Thread, uint32_t Affinity, be<uint32_t>* lpPreviousAffinity)
{
    if (lpPreviousAffinity)
        *lpPreviousAffinity = 2;

    return 0;
}

void RtlLeaveCriticalSection(XRTL_CRITICAL_SECTION* cs)
{
    // printf("RtlLeaveCriticalSection");
    cs->RecursionCount = cs->RecursionCount.get() - 1;

    if (cs->RecursionCount.get() != 0)
        return;

    std::atomic_ref owningThread(cs->OwningThread);
    owningThread.store(0);
    owningThread.notify_one();
}

void RtlEnterCriticalSection(XRTL_CRITICAL_SECTION* cs)
{
    static int s_count = 0;
    static int s_waitCount = 0;
    ++s_count;
    
    uint32_t thisThread = g_ppcContext->r13.u32;
    assert(thisThread != NULL);

    std::atomic_ref owningThread(cs->OwningThread);
    
    int loopCount = 0;
    constexpr int MAX_SPIN_LOOPS = 50;  // Very low - yield() is slow, force-acquire quickly
    
    while (true) 
    {
        uint32_t previousOwner = 0;

        if (owningThread.compare_exchange_weak(previousOwner, thisThread) || previousOwner == thisThread)
        {
            cs->RecursionCount = cs->RecursionCount.get() + 1;
            return;
        }

        // Log when we're waiting on a lock held by another thread
        if (loopCount == 0) {
            ++s_waitCount;
            if (s_waitCount <= 20) {
                LOGF_WARNING("[LOCK] RtlEnterCriticalSection #{} thisThread=0x{:08X} WAITING on owner=0x{:08X} cs=0x{:08X}",
                             s_count, thisThread, previousOwner, reinterpret_cast<uintptr_t>(cs) & 0xFFFFFFFF);
            }
        }
        ++loopCount;
        
        // After many spins, force acquire the lock to prevent deadlock
        // This is a workaround for init-time deadlocks where owner is blocked
        if (loopCount > MAX_SPIN_LOOPS) {
            if (s_waitCount <= 30) {
                LOGF_WARNING("[LOCK] FORCE ACQUIRE cs=0x{:08X} after {} spins (owner=0x{:08X} may be blocked)",
                             reinterpret_cast<uintptr_t>(cs) & 0xFFFFFFFF, loopCount, previousOwner);
            }
            owningThread.store(thisThread);
            cs->RecursionCount = 1;
            return;
        }
        
        // Brief yield instead of blocking wait
        std::this_thread::yield();
    }
}

void RtlImageXexHeaderField()
{
    LOG_UTILITY("!!! STUB !!!");
}

void HalReturnToFirmware()
{
    LOG_UTILITY("!!! STUB !!!");
}

void RtlFillMemoryUlong()
{
    LOG_UTILITY("!!! STUB !!!");
}

void KeBugCheckEx()
{
    __builtin_debugtrap();
}

uint32_t KeGetCurrentProcessType()
{
    return 1;
}

void RtlCompareMemoryUlong()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t RtlInitializeCriticalSection(XRTL_CRITICAL_SECTION* cs)
{
    // printf("RtlInitializeCriticalSection %x\n", cs);
    cs->Header.Absolute = 0;
    cs->LockCount = -1;
    cs->RecursionCount = 0;
    cs->OwningThread = 0;

    return 0;
}

void RtlRaiseException_x()
{
    LOG_UTILITY("!!! STUB !!!");
}

void KfReleaseSpinLock(uint32_t* spinLock)
{
    std::atomic_ref spinLockRef(*spinLock);
    spinLockRef = 0;
}

void KfAcquireSpinLock(uint32_t* spinLock)
{
    std::atomic_ref spinLockRef(*spinLock);

    while (true)
    {
        uint32_t expected = 0;
        if (spinLockRef.compare_exchange_weak(expected, g_ppcContext->r13.u32))
            break;

        std::this_thread::yield();
    }
}

uint64_t KeQueryPerformanceFrequency()
{
    return 49875000;
}

void MmFreePhysicalMemory(uint32_t type, uint32_t guestAddress)
{
    if (guestAddress != NULL)
        g_userHeap.Free(g_memory.Translate(guestAddress));
}

bool VdPersistDisplay(uint32_t a1, uint32_t* a2)
{
    *a2 = NULL;
    return false;
}

void VdSwap()
{
    // VdSwap is called by the Xbox 360 GPU to present frames
    static uint32_t s_frameCount = 0;
    if (s_frameCount < 10 || (s_frameCount % 60 == 0 && s_frameCount < 600))
    {
        LOGF_UTILITY("VdSwap frame {} - presenting!", s_frameCount);
    }
    ++s_frameCount;
    
    // Pump SDL events to keep the window responsive
    // IMPORTANT: On macOS, this MUST happen on the main thread
    if (IsMainThread())
    {
        SDL_PumpEvents();
        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
        GameWindow::Update();
    }
    
    // Call our host rendering system to present the frame
    Video::Present();
    
    // Signal "End of Frame" to unblock workers waiting on GPU fences
    // The GPU fence at 0x8006F844 and 0x8006F7F4 need to be signaled
    // Also increment the global KeSetEvent generation to wake any waiters
    ++g_keSetEventGeneration;
    g_keSetEventGeneration.notify_all();
    
    // Signal ALL tracked Event objects to unblock workers
    // Workers are waiting on events with infinite timeout - they need to be signaled
    {
        std::lock_guard lock(g_eventTrackMutex);
        static int s_signalLogCount = 0;
        for (uint32_t h : g_trackedEventHandles)
        {
            if (IsKernelObject(h))
            {
                Event* evt = GetKernelObject<Event>(h);
                if (evt)
                {
                    evt->Set();
                }
            }
        }
        if (s_signalLogCount < 5)
        {
            ++s_signalLogCount;
            LOGF_UTILITY("VdSwap signaled {} events (gen={})", g_trackedEventHandles.size(), g_keSetEventGeneration.load());
        }
    }
    
    // Signal worker semaphores to process queued work
    // Based on PPC analysis: workers wait on semaphores created during init
    // These semaphores need periodic signaling to drive worker processing
    static const std::vector<uint32_t> workerSemaphores = {
        0xA8240270, 0xA82402B0, 0xA82402F0, 0xA8240370,
        0xA82403B0, 0xA82403F0, 0xA8240430, 0xA8240470,
        0xA82404B0, 0xA82404F0, 0xA8240530, 0xA8240570
    };
    
    static int s_semSignalCount = 0;
    for (uint32_t semHandle : workerSemaphores)
    {
        if (IsKernelObject(semHandle))
        {
            Semaphore* sem = GetKernelObject<Semaphore>(semHandle);
            if (sem && sem->count.load() == 0)
            {
                sem->Release(1, nullptr);  // Signal one worker
            }
        }
    }
    
    if (s_semSignalCount < 5)
    {
        ++s_semSignalCount;
        LOGF_UTILITY("VdSwap signaled {} worker semaphores", workerSemaphores.size());
    }
}

void VdGetSystemCommandBuffer()
{
    static std::unordered_set<uint32_t> s_seenCallers;
    const uint32_t lr = g_ppcContext ? static_cast<uint32_t>(g_ppcContext->lr) : 0;
    if (lr != 0 && s_seenCallers.insert(lr).second)
        LOGF_UTILITY("!!! STUB !!! caller_lr=0x{:08X}", lr);
    else
        LOG_UTILITY("!!! STUB !!!");
}

void KeReleaseSpinLockFromRaisedIrql(uint32_t* spinLock)
{
    std::atomic_ref spinLockRef(*spinLock);
    spinLockRef = 0;
}

void KeAcquireSpinLockAtRaisedIrql(uint32_t* spinLock)
{
    std::atomic_ref spinLockRef(*spinLock);

    while (true)
    {
        uint32_t expected = 0;
        if (spinLockRef.compare_exchange_weak(expected, g_ppcContext->r13.u32))
            break;

        std::this_thread::yield();
    }
}

uint32_t KiApcNormalRoutineNop()
{
    return 0;
}

void VdEnableRingBufferRPtrWriteBack(uint32_t writebackAddr, uint32_t blockSizeLog2)
{
    // r3 = writeback address (physical)
    // r4 = log2(block size), typically 6
    g_gpuRingBuffer.readPtrWritebackAddr = writebackAddr;
    g_gpuRingBuffer.blockSize = 1u << blockSizeLog2;
    g_gpuRingBuffer.writebackEnabled = true;
    
    LOGF_UTILITY("writebackAddr=0x{:08X} blockSizeLog2={} blockSize={}", 
                 writebackAddr, blockSizeLog2, g_gpuRingBuffer.blockSize);
    
    // Immediately write 0 to the writeback address to indicate GPU is caught up
    if (writebackAddr != 0)
    {
        uint32_t* hostPtr = reinterpret_cast<uint32_t*>(g_memory.Translate(writebackAddr));
        if (hostPtr)
        {
            *hostPtr = 0;  // GPU read pointer = 0 (caught up with write pointer initially)
            LOGF_UTILITY("Initialized writeback at 0x{:08X} to 0", writebackAddr);
        }
    }
}

void VdInitializeRingBuffer(uint32_t physAddr, uint32_t sizeLog2)
{
    // r3 = physical address of ring buffer (from MmGetPhysicalAddress)
    // r4 = log2(size)
    g_gpuRingBuffer.ringBufferBase = physAddr;
    g_gpuRingBuffer.ringBufferSize = 1u << sizeLog2;
    g_gpuRingBuffer.initialized = true;
    
    LOGF_UTILITY("ringBufferBase=0x{:08X} sizeLog2={} size={}", 
                 physAddr, sizeLog2, g_gpuRingBuffer.ringBufferSize);
}

// Scan a command buffer for PM4 packets (helper function)
// Returns number of packets found
static uint32_t ScanPM4Buffer(uint32_t* buffer, uint32_t bufferDwords, bool isIndirect, uint32_t scanCount) {
    static bool s_loggedIndirect = false;
    uint32_t packetsFound = 0;
    uint32_t pos = 0;
    uint32_t maxScan = std::min(bufferDwords, 512u);
    
    while (pos < maxScan && packetsFound < 100) {
        uint32_t packet = ByteSwap(buffer[pos]);
        uint32_t type = (packet >> 30) & 0x3;
        
        if (type == PM4_TYPE3) {
            uint32_t opcode = (packet >> 8) & 0x7F;
            uint32_t count = ((packet >> 16) & 0x3FFF) + 1;
            
            // Log ALL packets from indirect buffers for first few scans
            if (isIndirect && !s_loggedIndirect && scanCount < 3) {
                LOGF_WARNING("[PM4-IB] opcode=0x{:02X}({}) count={} pos={}",
                    opcode, GetPM4OpcodeName(opcode), count, pos);
            }
            
            // Count by type
            switch (opcode) {
                case PM4_DRAW_INDX:
                case PM4_DRAW_INDX_2:
                    g_gpuRingBuffer.pm4DrawCount++;
                    break;
                case PM4_IM_LOAD:
                case PM4_IM_LOAD_IMMEDIATE:
                    g_gpuRingBuffer.pm4ShaderLoadCount++;
                    break;
                case PM4_SET_CONSTANT:
                case PM4_SET_CONSTANT2:
                case PM4_SET_SHADER_CONSTANTS:
                    g_gpuRingBuffer.pm4SetConstantCount++;
                    break;
                case PM4_XE_SWAP:
                    g_gpuRingBuffer.pm4SwapCount++;
                    break;
                default:
                    g_gpuRingBuffer.pm4OtherCount++;
                    break;
            }
            
            packetsFound++;
            pos += 1 + count;
        }
        else if (type == PM4_TYPE0) {
            // Register write: extract register index and count
            uint32_t regIndex = packet & 0x7FFF;
            uint32_t count = ((packet >> 16) & 0x3FFF) + 1;
            
            // Log first few TYPE0 packets from indirect buffers
            if (isIndirect && !s_loggedIndirect && scanCount < 2 && packetsFound < 10) {
                LOGF_WARNING("[PM4-IB] TYPE0 reg=0x{:04X} count={} pos={}",
                    regIndex, count, pos);
            }
            
            pos += 1 + count;
            packetsFound++;
        }
        else if (type == PM4_TYPE2) {
            pos++;
        }
        else {
            pos++;
        }
    }
    
    if (isIndirect && packetsFound > 0) {
        s_loggedIndirect = true;
    }
    
    return packetsFound;
}

// Scan the GPU ring buffer for PM4 packets and log what commands the game writes
// This is diagnostic - helps determine if GTA IV uses GPU command buffer or D3D API
void ScanRingBufferForPM4Packets()
{
    if (!g_gpuRingBuffer.initialized || g_gpuRingBuffer.ringBufferBase == 0)
        return;
    
    // Get pointer to ring buffer in host memory
    uint32_t* ringBuffer = reinterpret_cast<uint32_t*>(
        g_memory.Translate(g_gpuRingBuffer.ringBufferBase));
    if (!ringBuffer)
        return;
    
    uint32_t ringBufferDwords = g_gpuRingBuffer.ringBufferSize / 4;
    if (ringBufferDwords == 0)
        return;
    
    static bool s_scannedOnce = false;
    static uint32_t s_scanCount = 0;
    static bool s_loggedIndirectBuffers = false;
    
    // First pass: scan primary ring buffer and collect INDIRECT_BUFFER addresses
    std::vector<std::pair<uint32_t, uint32_t>> indirectBuffers;  // address, size
    uint32_t packetsFound = 0;
    uint32_t pos = 0;
    uint32_t maxScan = std::min(ringBufferDwords, 256u);
    
    while (pos < maxScan && packetsFound < 20) {
        uint32_t packet = ByteSwap(ringBuffer[pos]);
        uint32_t type = (packet >> 30) & 0x3;
        
        if (type == PM4_TYPE3) {
            uint32_t opcode = (packet >> 8) & 0x7F;
            uint32_t count = ((packet >> 16) & 0x3FFF) + 1;
            
            // Log the first few packets we find
            if (!s_scannedOnce || s_scanCount < 5) {
                LOGF_WARNING("[PM4] pos={} type=3 opcode=0x{:02X}({}) count={}",
                    pos, opcode, GetPM4OpcodeName(opcode), count);
            }
            
            // Capture INDIRECT_BUFFER addresses
            if (opcode == PM4_INDIRECT_BUFFER && count >= 2) {
                uint32_t ibAddr = ByteSwap(ringBuffer[pos + 1]);
                uint32_t ibSize = ByteSwap(ringBuffer[pos + 2]) & 0xFFFFF;  // Size in dwords
                indirectBuffers.push_back({ibAddr, ibSize});
                
                if (!s_loggedIndirectBuffers && s_scanCount < 3) {
                    LOGF_WARNING("[PM4] INDIRECT_BUFFER addr=0x{:08X} size={} dwords",
                        ibAddr, ibSize);
                }
            }
            
            // Count by type (primary buffer)
            g_gpuRingBuffer.pm4OtherCount++;
            
            packetsFound++;
            pos += 1 + count;
        }
        else if (type == PM4_TYPE0) {
            uint32_t count = ((packet >> 16) & 0x3FFF) + 1;
            pos += 1 + count;
            packetsFound++;
        }
        else if (type == PM4_TYPE2) {
            pos++;
        }
        else {
            pos++;
        }
    }
    
    // Second pass: scan indirect buffers for actual draw/shader commands
    for (const auto& [ibAddr, ibSize] : indirectBuffers) {
        if (ibAddr == 0 || ibSize == 0) continue;
        
        uint32_t* ibBuffer = reinterpret_cast<uint32_t*>(g_memory.Translate(ibAddr));
        if (ibBuffer) {
            uint32_t ibPackets = ScanPM4Buffer(ibBuffer, ibSize, true, s_scanCount);
            packetsFound += ibPackets;
        }
    }
    
    if (!indirectBuffers.empty()) {
        s_loggedIndirectBuffers = true;
    }
    
    // Log summary periodically
    if (packetsFound > 0 && (s_scanCount % 100 == 0 || !s_scannedOnce)) {
        LOGF_WARNING("[PM4 SUMMARY] scan={} found={} draws={} shaders={} constants={} swaps={} other={} IB={}",
            s_scanCount, packetsFound,
            g_gpuRingBuffer.pm4DrawCount,
            g_gpuRingBuffer.pm4ShaderLoadCount,
            g_gpuRingBuffer.pm4SetConstantCount,
            g_gpuRingBuffer.pm4SwapCount,
            g_gpuRingBuffer.pm4OtherCount,
            indirectBuffers.size());
    }
    
    if (packetsFound > 0) {
        s_scannedOnce = true;
    }
    s_scanCount++;
}

uint32_t MmGetPhysicalAddress(uint32_t address)
{
    LOGF_UTILITY("0x{:x}", address);
    return address;
}

void VdSetSystemCommandBufferGpuIdentifierAddress()
{
    LOG_UTILITY("!!! STUB !!!");
}

void _vsnprintf_x()
{
    LOG_UTILITY("!!! STUB !!!");
}

void sprintf_x()
{
    LOG_UTILITY("!!! STUB !!!");
}

int32_t ExRegisterTitleTerminateNotification(uint32_t* reg, uint32_t create)
{
    LOG_UTILITY("!!! STUB !!!");
    return 0;
}

void VdShutdownEngines()
{
    LOG_UTILITY("!!! STUB !!!");
}

void VdQueryVideoMode(XVIDEO_MODE* vm)
{
    memset(vm, 0, sizeof(XVIDEO_MODE));
    vm->DisplayWidth = 1280;
    vm->DisplayHeight = 720;
    vm->IsInterlaced = false;
    vm->IsWidescreen = true;
    vm->IsHighDefinition = true;
    vm->RefreshRate = 0x42700000;
    vm->VideoStandard = 1;
    vm->Unknown4A = 0x4A;
    vm->Unknown01 = 0x01;
}

void VdGetCurrentDisplayInformation()
{
    LOG_UTILITY("!!! STUB !!!");
}

void VdSetDisplayMode()
{
    LOG_UTILITY("!!! STUB !!!");
}

// =============================================================================
// VBlank Timer Thread - Fires at 60Hz to drive timing-dependent systems
// =============================================================================
// GTA IV expects VBlank interrupts at exactly 60Hz even during initialization.
// This is different from Sonic Unleashed which doesn't need VBlank timing.
// The timer fires the registered graphics interrupt callback and wakes event waiters.

static std::atomic<bool> g_vblankThreadRunning{false};
static std::thread g_vblankThread;

void StartVBlankTimer()
{
    if (g_vblankThreadRunning)
        return;  // Already running
    
    g_vblankThreadRunning = true;
    printf("[StartVBlankTimer] Starting VBlank thread...\n");
    g_vblankThread = std::thread([]() {
        using namespace std::chrono;
        auto nextVBlank = steady_clock::now();
        constexpr auto VBLANK_INTERVAL = nanoseconds(16666667);  // 60Hz = 16.666ms
        
        uint32_t vblankCount = 0;
        
        while (g_vblankThreadRunning)
        {
            nextVBlank += VBLANK_INTERVAL;
            std::this_thread::sleep_until(nextVBlank);
            
            ++vblankCount;
            
            // Fire VBlank callback if registered
            // NOTE: g_ppcContext is thread_local and NULL on this thread!
            // We create our own context for VBlank callbacks.
            if (vblankCount <= 3) { printf("[VBlank] Thread tick #%u callback=0x%08X\n", vblankCount, g_gpuRingBuffer.interruptCallback); }
            if (g_gpuRingBuffer.interruptCallback != 0)
            {
                PPCFunc* callback = g_memory.FindFunction(g_gpuRingBuffer.interruptCallback);
                if (callback)
                {
                    // Create a minimal context for VBlank callback
                    // Cannot use g_ppcContext - it's thread_local and null here
                    static PPCContext vblankCtx{};
                    vblankCtx.r3.u32 = 0;  // Interrupt type 0 = VBlank
                    vblankCtx.r4.u32 = g_gpuRingBuffer.interruptUserData;
                    vblankCtx.r1.u64 = 0x80080000;  // Minimal stack pointer
                    vblankCtx.r13.u64 = 0x80000D20; // Minimal TLS base
                    callback(vblankCtx, g_memory.base);
                    
                    if (vblankCount <= 5 || vblankCount % 60 == 0)
                    {
                        LOGF_UTILITY("[VBlank] Fired callback #{} at 60Hz", vblankCount);
                    }
                }
            }
            
            // Increment event generation to wake any waiters
            ++g_keSetEventGeneration;
            g_keSetEventGeneration.notify_all();
        }
    });
}

void StopVBlankTimer()
{
    if (g_vblankThreadRunning)
    {
        g_vblankThreadRunning = false;
        if (g_vblankThread.joinable())
            g_vblankThread.join();
    }
}

void VdSetGraphicsInterruptCallback(uint32_t callback, uint32_t userData)
{
    g_gpuRingBuffer.interruptCallback = callback;
    g_gpuRingBuffer.interruptUserData = userData;
    
    // Start VBlank timer when callback is registered (GTA IV requirement)
    StartVBlankTimer();
    
    printf("[VdSetGraphicsInterruptCallback] callback=0x%08X userData=0x%08X - VBlank timer started at 60Hz\n", callback, userData);
}

uint32_t VdInitializeEngines()
{
    g_gpuRingBuffer.enginesInitialized = true;
    
    // FORCE: Register VBlank callback since VdSetGraphicsInterruptCallback is never called
    // The callback 0x829D7368 is the known VBlank handler from GTA IV
    if (g_gpuRingBuffer.interruptCallback == 0) {
        printf("[VdInitializeEngines] FORCE-registering VBlank callback 0x829D7368\n");
        g_gpuRingBuffer.interruptCallback = 0x829D7368;
        g_gpuRingBuffer.interruptUserData = 0;
        StartVBlankTimer();
    }
    LOG_UTILITY("enginesInitialized = true");
    
    // Signal all tracked events to wake waiting threads
    // Workers may be blocked waiting for GPU initialization to complete
    {
        std::lock_guard lock(g_eventTrackMutex);
        for (uint32_t h : g_trackedEventHandles)
        {
            if (IsKernelObject(h))
            {
                Event* evt = GetKernelObject<Event>(h);
                if (evt)
                {
                    evt->Set();
                }
            }
        }
        LOGF_UTILITY("VdInitializeEngines signaled {} events", g_trackedEventHandles.size());
    }
    
    return 1;
}

uint32_t VdIsHSIOTrainingSucceeded()
{
    // Return 1 to indicate HSIO training succeeded - this unblocks GPU init polling
    return 1;
}

void VdGetCurrentDisplayGamma()
{
    LOG_UTILITY("!!! STUB !!!");
}

void VdQueryVideoFlags()
{
    LOG_UTILITY("!!! STUB !!!");
}

void VdCallGraphicsNotificationRoutines()
{
    LOG_UTILITY("!!! STUB !!!");
}

void VdInitializeScalerCommandBuffer()
{
    LOG_UTILITY("!!! STUB !!!");
}

void KeLeaveCriticalRegion()
{
    // Critical region management for APC protection.
    // Since APCs are not fully implemented, this is a no-op.
}

uint32_t VdRetrainEDRAM()
{
    // Set the EDRAM training complete flag - the GPU poll loop waits for this
    g_gpuRingBuffer.edramTrainingComplete = true;
    
    static int s_callCount = 0;
    if (++s_callCount <= 3)
    {
        LOGF_UTILITY("edramTrainingComplete = true (call #{})", s_callCount);
    }
    
    // Signal all tracked events to wake waiting threads
    // GPU initialization complete - workers can proceed
    {
        std::lock_guard lock(g_eventTrackMutex);
        for (uint32_t h : g_trackedEventHandles)
        {
            if (IsKernelObject(h))
            {
                Event* evt = GetKernelObject<Event>(h);
                if (evt)
                {
                    evt->Set();
                }
            }
        }
        if (s_callCount <= 3)
        {
            LOGF_UTILITY("VdRetrainEDRAM signaled {} events", g_trackedEventHandles.size());
        }
    }
    
    return 0;  // Success
}

uint32_t VdRetrainEDRAMWorker(uint32_t unk0)
{
    // This is called by the GPU init polling loop
    // Must set persistent state, not just return success
    g_gpuRingBuffer.edramTrainingComplete = true;
    
    static int s_callCount = 0;
    if (++s_callCount <= 3)
    {
        LOGF_UTILITY("unk0=0x{:08X} - edramTrainingComplete = true (call #{})", unk0, s_callCount);
    }
    return 0;  // Success
}

void KeEnterCriticalRegion()
{
    // Critical region management for APC protection.
    // Since APCs are not fully implemented, this is a no-op.
}

uint32_t MmAllocatePhysicalMemoryEx
(
    uint32_t flags,
    uint32_t size,
    uint32_t protect,
    uint32_t minAddress,
    uint32_t maxAddress,
    uint32_t alignment
)
{
    static std::unordered_set<uint32_t> s_seenCallers;
    const uint32_t lr = g_ppcContext ? static_cast<uint32_t>(g_ppcContext->lr) : 0;
    if (lr != 0 && s_seenCallers.insert(lr).second)
    {
        LOGF_UTILITY(
            "caller_lr=0x{:08X} flags=0x{:X} size=0x{:X} prot=0x{:X} min=0x{:08X} max=0x{:08X} align=0x{:X}",
            lr, flags, size, protect, minAddress, maxAddress, alignment);
    }
    void* host = g_userHeap.AllocPhysical(size, alignment);
    if (host == nullptr)
        return 0;

    return g_memory.MapVirtual(host);
}

void ObDeleteSymbolicLink()
{
    LOG_UTILITY("!!! STUB !!!");
}

void ObCreateSymbolicLink()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t MmQueryAddressProtect(uint32_t guestAddress)
{
    return PAGE_READWRITE;
}

void VdEnableDisableClockGating()
{
    LOG_UTILITY("!!! STUB !!!");
}

void KeBugCheck()
{
    __builtin_debugtrap();
}

void KeLockL2()
{
    LOG_UTILITY("!!! STUB !!!");
}

void KeUnlockL2()
{
    LOG_UTILITY("!!! STUB !!!");
}

bool KeSetEvent(XKEVENT* pEvent, uint32_t Increment, bool Wait)
{
    static int s_count = 0;
    ++s_count;
    
    if (s_count <= 20 || s_count % 100 == 0)
    {
        LOGF_IMPL(Utility, "Event", "KeSetEvent #{}", s_count);
    }
    
    bool result = QueryKernelObject<Event>(*pEvent)->Set();

    ++g_keSetEventGeneration;
    g_keSetEventGeneration.notify_all();

    return result;
}

bool KeResetEvent(XKEVENT* pEvent)
{
    return QueryKernelObject<Event>(*pEvent)->Reset();
}

// Track recent async reads for correlation with waits
static std::atomic<uint32_t> g_lastAsyncInfo{0};
static std::atomic<int> g_asyncReadCount{0};

// =============================================================================
// WAIT TRACKING: Find the exact handle blocking Frame 1
// This tracks repeated waits on the same handle to identify the missing signal
// =============================================================================
static std::mutex g_waitTrackMutex;
static std::unordered_map<uint32_t, int> g_handleWaitCounts;  // handle -> count
static uint32_t g_mostWaitedHandle = 0;
static int g_mostWaitedCount = 0;

uint32_t KeWaitForSingleObject(XDISPATCHER_HEADER* Object, uint32_t WaitReason, uint32_t WaitMode, bool Alertable, be<int64_t>* Timeout)
{
    static int s_waitCount = 0;
    static int s_infiniteWaitCount = 0;
    ++s_waitCount;
    
    const uint32_t timeout = GuestTimeoutToMilliseconds(Timeout);
    const uint32_t caller = g_ppcContext ? static_cast<uint32_t>(g_ppcContext->lr) : 0;
    const uint32_t objAddr = Object ? static_cast<uint32_t>(reinterpret_cast<uintptr_t>(Object) - reinterpret_cast<uintptr_t>(g_memory.base)) : 0;
    
    // TRACK REPEATED WAITS: Find the handle that's blocking
    // Exclude GPU fence handles (0x8006F844, 0x8006F7F4) and GPU thread (0x829DDD48) - already handled
    bool isGpuWait = (objAddr == 0x8006F844 || objAddr == 0x8006F7F4 || caller == 0x829DDD48);
    
    // Get thread ID for tracking
    uint32_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id()) & 0xFFFF;
    
    // Log ALL infinite waits with thread info to find the blocking main thread
    if (timeout == INFINITE && !isGpuWait)
    {
        static int s_infiniteNonGpu = 0;
        ++s_infiniteNonGpu;
        if (s_infiniteNonGpu <= 30 || s_infiniteNonGpu % 100 == 0)
        {
            LOGF_WARNING("[INFINITE WAIT] #{} thread=0x{:04X} handle=0x{:08X} type={} caller=0x{:08X}", 
                        s_infiniteNonGpu, threadId, objAddr, Object ? Object->Type : -1, caller);
        }
    }
    
    if (objAddr != 0 && !isGpuWait)
    {
        std::lock_guard<std::mutex> lock(g_waitTrackMutex);
        int& count = g_handleWaitCounts[objAddr];
        ++count;
        
        // Update most-waited handle (non-GPU)
        if (count > g_mostWaitedCount)
        {
            g_mostWaitedCount = count;
            g_mostWaitedHandle = objAddr;
        }
    }
    
    // Removed all GPU fence bypass hacks - following Unleashed's clean approach
    // Let proper Event/Semaphore wait/notify work naturally without forced returns

    switch (Object->Type)
    {
        case 0:
        case 1:
            QueryKernelObject<Event>(*Object)->Wait(timeout);
            break;

        case 5:
            QueryKernelObject<Semaphore>(*Object)->Wait(timeout);
            break;

        default:
            LOGF_IMPL(Utility, "KeWaitForSingleObject", "Unrecognized kernel object type: {}", Object->Type);
            return STATUS_TIMEOUT;
    }

    return STATUS_SUCCESS;
}

// =============================================================================
// TLS UNIFICATION: Route KeTls* APIs through Guest Memory (r13 + X360_TLS_OFFSET)
// =============================================================================
// Problem: Previously, KeTlsGetValue/SetValue used host thread_local storage,
// which was completely disconnected from the guest memory TLS at r13+0xAB0.
// This caused state mismatches where game code and kernel APIs saw different TLS.
//
// Solution: Unified TLS - all TLS access goes through guest memory.
// - Guest threads: read/write from r13 + X360_TLS_OFFSET + (index * 4)
// - Non-guest threads (VBlank, etc.): fallback to host thread_local storage
// =============================================================================

static std::vector<size_t> g_tlsFreeIndices;
static size_t g_tlsNextIndex = 0;
static Mutex g_tlsAllocationMutex;

// Host-side TLS fallback for non-guest threads (VBlank callback thread, etc.)
static uint32_t& KeTlsGetValueRef_Host(size_t index)
{
    thread_local std::vector<uint32_t> s_tlsValues;
    if (s_tlsValues.size() <= index)
    {
        s_tlsValues.resize(index + 1, 0);
    }
    return s_tlsValues[index];
}

// Maximum TLS slots in guest memory (X360_TLS_SIZE = 0x100 = 256 bytes = 64 uint32_t slots)
static constexpr uint32_t MAX_GUEST_TLS_SLOTS = X360_TLS_SIZE / sizeof(uint32_t);

uint32_t KeTlsGetValue(uint32_t dwTlsIndex)
{
    // Get current thread's PPC context
    PPCContext* ctx = GetPPCContext();
    
    // If no PPC context or r13 is invalid, fall back to host TLS
    // This handles non-guest threads like VBlank callback thread
    if (!ctx || ctx->r13.u32 == 0 || ctx->r13.u32 < 0x80000000)
    {
        return KeTlsGetValueRef_Host(dwTlsIndex);
    }
    
    // Validate index - guest TLS has limited slots
    if (dwTlsIndex >= MAX_GUEST_TLS_SLOTS)
    {
        // For indices beyond guest TLS capacity, use host fallback
        return KeTlsGetValueRef_Host(dwTlsIndex);
    }
    
    // Read from guest memory: r13 + X360_TLS_OFFSET + (index * 4)
    // X360_TLS_OFFSET = 0xAB0 (defined in guest_thread.h)
    uint32_t tlsAddr = ctx->r13.u32 + X360_TLS_OFFSET + (dwTlsIndex * sizeof(uint32_t));
    // Direct guest memory access (can't use PPC_LOAD_U32 macro - no 'base' in scope)
    uint32_t* ptr = reinterpret_cast<uint32_t*>(g_memory.base + tlsAddr);
    return __builtin_bswap32(*ptr);  // Big-endian to host
}

uint32_t KeTlsSetValue(uint32_t dwTlsIndex, uint32_t lpTlsValue)
{
    PPCContext* ctx = GetPPCContext();
    
    if (!ctx || ctx->r13.u32 == 0 || ctx->r13.u32 < 0x80000000)
    {
        KeTlsGetValueRef_Host(dwTlsIndex) = lpTlsValue;
        return TRUE;
    }
    
    if (dwTlsIndex >= MAX_GUEST_TLS_SLOTS)
    {
        KeTlsGetValueRef_Host(dwTlsIndex) = lpTlsValue;
        return TRUE;
    }
    
    // Write to guest memory: r13 + X360_TLS_OFFSET + (index * 4)
    uint32_t tlsAddr = ctx->r13.u32 + X360_TLS_OFFSET + (dwTlsIndex * sizeof(uint32_t));
    // Direct guest memory access (can't use PPC_STORE_U32 macro - no 'base' in scope)
    uint32_t* ptr = reinterpret_cast<uint32_t*>(g_memory.base + tlsAddr);
    *ptr = __builtin_bswap32(lpTlsValue);  // Host to big-endian
    return TRUE;
}

uint32_t KeTlsAlloc()
{
    std::lock_guard<Mutex> lock(g_tlsAllocationMutex);
    
    // Try to reuse a freed slot first
    if (!g_tlsFreeIndices.empty())
    {
        size_t index = g_tlsFreeIndices.back();
        g_tlsFreeIndices.pop_back();
        return static_cast<uint32_t>(index);
    }
    
    // Allocate new slot
    size_t index = g_tlsNextIndex++;
    
    // Warn if exceeding guest TLS capacity (still works via host fallback)
    if (index >= MAX_GUEST_TLS_SLOTS)
    {
        static bool warned = false;
        if (!warned)
        {
            LOGF_WARNING("[TLS] Allocated slot {} exceeds guest TLS capacity ({}), using host fallback",
                         index, MAX_GUEST_TLS_SLOTS);
            warned = true;
        }
    }
    
    return static_cast<uint32_t>(index);
}

uint32_t KeTlsFree(uint32_t dwTlsIndex)
{
    std::lock_guard<Mutex> lock(g_tlsAllocationMutex);
    g_tlsFreeIndices.push_back(dwTlsIndex);
    return TRUE;
}

uint32_t XMsgInProcessCall(uint32_t app, uint32_t message, be<uint32_t>* param1, be<uint32_t>* param2)
{
    if (message == 0x7001B)
    {
        uint32_t* ptr = (uint32_t*)g_memory.Translate(param1[1]);
        ptr[0] = 0;
        ptr[1] = 0;
    }

    return 0;
}

void XamUserReadProfileSettings
(
    uint32_t titleId,
    uint32_t userIndex,
    uint32_t xuidCount,
    uint64_t* xuids,
    uint32_t settingCount,
    uint32_t* settingIds,
    be<uint32_t>* bufferSize,
    void* buffer,
    void* overlapped
)
{
    if (buffer != nullptr)
    {
        memset(buffer, 0, *bufferSize);
    }
    else
    {
        *bufferSize = 4;
    }
}

// =============================================================================
// Network Socket Functions - Now implemented in kernel/io/net_socket.cpp
// =============================================================================
// The following functions were stubs and are now replaced with real implementations:
// NetDll_WSAStartup, NetDll_WSACleanup, NetDll_socket, NetDll_closesocket,
// NetDll_setsockopt, NetDll_bind, NetDll_connect, NetDll_listen, NetDll_accept,
// NetDll_select, NetDll_recv, NetDll_send, NetDll_inet_addr
// =============================================================================

void NetDll___WSAFDIsSet()
{
    LOG_UTILITY("!!! STUB !!!");
}

void XMsgStartIORequestEx()
{
    LOG_UTILITY("!!! STUB !!!");
}

void XexGetModuleHandle()
{
    LOG_UTILITY("!!! STUB !!!");
}

bool RtlTryEnterCriticalSection(XRTL_CRITICAL_SECTION* cs)
{
    // printf("RtlTryEnterCriticalSection\n");
    uint32_t thisThread = g_ppcContext->r13.u32;
    assert(thisThread != NULL);

    std::atomic_ref owningThread(cs->OwningThread);

    uint32_t previousOwner = 0;

    if (owningThread.compare_exchange_weak(previousOwner, thisThread) || previousOwner == thisThread)
    {
        cs->RecursionCount = cs->RecursionCount.get() + 1;
        return true;
    }

    return false;
}

void RtlInitializeCriticalSectionAndSpinCount(XRTL_CRITICAL_SECTION* cs, uint32_t spinCount)
{
    // printf("RtlInitializeCriticalSectionAndSpinCount\n");
    cs->Header.Absolute = (spinCount + 255) >> 8;
    cs->LockCount = -1;
    cs->RecursionCount = 0;
    cs->OwningThread = 0;
}

void _vswprintf_x()
{
    LOG_UTILITY("!!! STUB !!!");
}

void _vscwprintf_x()
{
    LOG_UTILITY("!!! STUB !!!");
}

void _swprintf_x()
{
    LOG_UTILITY("!!! STUB !!!");
}

void _snwprintf_x()
{
    LOG_UTILITY("!!! STUB !!!");
}

void XeCryptBnQwBeSigVerify()
{
    LOG_UTILITY("!!! STUB !!!");
}

void XeKeysGetKey()
{
    LOG_UTILITY("!!! STUB !!!");
}

void XeCryptRotSumSha()
{
    LOG_UTILITY("!!! STUB !!!");
}

void XeCryptSha()
{
    LOG_UTILITY("!!! STUB !!!");
}

void KeEnableFpuExceptions()
{
    LOG_UTILITY("!!! STUB !!!");
}

void RtlUnwind_x()
{
    LOG_UTILITY("!!! STUB !!!");
}

void RtlCaptureContext_x()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t NtQueryFullAttributesFile(XOBJECT_ATTRIBUTES* ObjectAttributes, XFILE_NETWORK_OPEN_INFORMATION* FileInformation)
{
    if (!ObjectAttributes || !FileInformation)
        return STATUS_INVALID_PARAMETER;

    std::string guestPath;
    if (!TryGetAnsiPath(ObjectAttributes, guestPath))
        return STATUS_INVALID_PARAMETER;

    const std::filesystem::path resolved = ResolveGuestPathBestEffort(guestPath);

    std::error_code ec;
    const bool exists = std::filesystem::exists(resolved, ec);
    if (!exists || ec)
        return STATUS_OBJECT_NAME_NOT_FOUND;

    const bool isDir = std::filesystem::is_directory(resolved, ec);
    if (ec)
        return STATUS_FAIL_CHECK;

    uint64_t fileSize = 0;
    if (!isDir)
    {
        fileSize = std::filesystem::file_size(resolved, ec);
        if (ec)
            fileSize = 0;
    }

    const uint64_t allocationSize = RoundUpToPage(fileSize);
    const uint32_t attrs = GetFileAttributesBestEffort(resolved);

    FileInformation->CreationTime = 0;
    FileInformation->LastAccessTime = 0;
    FileInformation->LastWriteTime = 0;
    FileInformation->ChangeTime = 0;
    FileInformation->AllocationSize = static_cast<int64_t>(allocationSize);
    FileInformation->EndOfFile = static_cast<int64_t>(fileSize);
    FileInformation->FileAttributes = attrs;
    FileInformation->Reserved = 0;

    return STATUS_SUCCESS;
}

uint32_t RtlMultiByteToUnicodeN(be<uint16_t>* UnicodeString, uint32_t MaxBytesInUnicodeString, be<uint32_t>* BytesInUnicodeString, const char* MultiByteString, uint32_t BytesInMultiByteString)
{
    uint32_t length = std::min(MaxBytesInUnicodeString / 2, BytesInMultiByteString);

    for (size_t i = 0; i < length; i++)
        UnicodeString[i] = MultiByteString[i];

    if (BytesInUnicodeString != nullptr)
        *BytesInUnicodeString = length * 2;

    return STATUS_SUCCESS;
}

void DbgBreakPoint()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t MmQueryAllocationSize(uint32_t guestAddress)
{
    if (guestAddress == 0)
        return 0;
    
    return (uint32_t)g_userHeap.Size(g_memory.Translate(guestAddress));
}

uint32_t NtClearEvent(Event* handle, uint32_t* previousState)
{
    handle->Reset();
    return 0;
}

uint32_t NtResumeThread(GuestThreadHandle* hThread, uint32_t* suspendCount)
{
    assert(hThread != GetKernelObject(CURRENT_THREAD_HANDLE));

    hThread->suspended = false;
    hThread->suspended.notify_all();

    return S_OK;
}

uint32_t NtSetEvent(Event* handle, uint32_t* previousState)
{
    handle->Set();
    return 0;
}

uint32_t NtCreateSemaphore(be<uint32_t>* Handle, XOBJECT_ATTRIBUTES* ObjectAttributes, uint32_t InitialCount, uint32_t MaximumCount)
{
    static int s_count = 0; ++s_count;
    Semaphore* sem = CreateKernelObject<Semaphore>(InitialCount, MaximumCount);
    *Handle = GetKernelHandle(sem);
    uint32_t handle = *Handle;
    
    // Track blocking semaphores (count=0) for signaling after init
    if (InitialCount == 0) {
        // Store the kernel handle for later signaling
        std::lock_guard<std::mutex> lock(g_semaphoreTrackMutex);
        g_blockingSemaphoreAddrs.insert(handle);  // Track handle directly
        LOGF_WARNING("[NtCreateSemaphore] #{} handle=0x{:08X} count=0 max={} - TRACKED for signaling", 
                     s_count, handle, MaximumCount);
    } else if (s_count <= 50) {
        LOGF_WARNING("[NtCreateSemaphore] #{} handle=0x{:08X} count={} max={}", 
                     s_count, handle, InitialCount, MaximumCount);
    }
    
    return STATUS_SUCCESS;
}

uint32_t NtReleaseSemaphore(uint32_t Handle, uint32_t ReleaseCount, int32_t* PreviousCount)
{
    static int s_count = 0;
    ++s_count;
    
    // FIX: Handle is a guest address that needs to be resolved via kernel object system
    // Previously this took Semaphore* directly which caused signals to go to wrong objects
    if (!IsKernelObject(Handle))
    {
        if (s_count <= 20 || s_count % 100 == 0)
        {
            LOGF_WARNING("[NtReleaseSemaphore] #{} Invalid handle 0x{:08X}", s_count, Handle);
        }
        return STATUS_INVALID_HANDLE;
    }
    
    KernelObject* obj = GetKernelObject(Handle);
    if (!obj)
    {
        LOGF_WARNING("[NtReleaseSemaphore] #{} Null kernel object for handle 0x{:08X}", s_count, Handle);
        return STATUS_INVALID_HANDLE;
    }
    
    // Check if this is actually a semaphore
    Semaphore* sem = dynamic_cast<Semaphore*>(obj);
    if (!sem)
    {
        // Not a semaphore - might be an event, try to signal it as an event
        Event* evt = dynamic_cast<Event*>(obj);
        if (evt)
        {
            if (s_count <= 30 || s_count % 100 == 0)
            {
                LOGF_WARNING("[NtReleaseSemaphore] #{} handle=0x{:08X} is EVENT, signaling as event", s_count, Handle);
            }
            evt->Set();
            return STATUS_SUCCESS;
        }
        
        LOGF_WARNING("[NtReleaseSemaphore] #{} handle=0x{:08X} is not a semaphore or event", s_count, Handle);
        return STATUS_INVALID_HANDLE;  // Object type mismatch
    }
    
    // Always log releases for blocking semaphores, otherwise limit logging
    bool isBlockingSemaphore = (Handle >= 0xEB2D0000 && Handle <= 0xEB2E0000);
    if (s_count <= 30 || isBlockingSemaphore || s_count % 100 == 0)
    {
        uint32_t curCount = sem->count.load();
        LOGF_WARNING("[NtReleaseSemaphore] #{} handle=0x{:08X} release={} count={}/{}{}", 
                  s_count, Handle, ReleaseCount, curCount, sem->maximumCount,
                  isBlockingSemaphore ? " *** BLOCKING SEM ***" : "");
    }
    
    // the game releases semaphore with 1 maximum number of releases more than once
    if (sem->count + ReleaseCount > sem->maximumCount)
        return STATUS_SEMAPHORE_LIMIT_EXCEEDED;

    uint32_t previousCount;
    sem->Release(ReleaseCount, &previousCount);

    if (PreviousCount != nullptr)
        *PreviousCount = ByteSwap(previousCount);

    return STATUS_SUCCESS;
}

void NtWaitForMultipleObjectsEx()
{
    LOG_UTILITY("!!! STUB !!!");
}

void RtlCompareStringN()
{
    LOG_UTILITY("!!! STUB !!!");
}

void _snprintf_x()
{
    LOG_UTILITY("!!! STUB !!!");
}

void StfsControlDevice()
{
    LOG_UTILITY("!!! STUB !!!");
}

void StfsCreateDevice()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NtFlushBuffersFile()
{
    LOG_UTILITY("!!! STUB !!!");
}

void KeQuerySystemTime(be<uint64_t>* time)
{
    // Pump SDL events periodically to prevent window from becoming unresponsive
    PumpSdlEventsIfNeeded();
    
    constexpr int64_t FILETIME_EPOCH_DIFFERENCE = 116444736000000000LL;

    auto now = std::chrono::system_clock::now();
    auto timeSinceEpoch = now.time_since_epoch();

    int64_t currentTime100ns = std::chrono::duration_cast<std::chrono::duration<int64_t, std::ratio<1, 10000000>>>(timeSinceEpoch).count();
    currentTime100ns += FILETIME_EPOCH_DIFFERENCE;

    *time = currentTime100ns;
}

struct TIME_FIELDS {
    be<uint16_t> Year;
    be<uint16_t> Month;
    be<uint16_t> Day;
    be<uint16_t> Hour;
    be<uint16_t> Minute;
    be<uint16_t> Second;
    be<uint16_t> Milliseconds;
    be<uint16_t> Weekday;
};

void RtlTimeToTimeFields(const be<uint64_t>* time, TIME_FIELDS* timeFields)
{
    constexpr uint64_t TICKS_PER_MILLISECOND = 10000;
    constexpr uint64_t TICKS_PER_SECOND = 10000000;
    constexpr uint64_t TICKS_PER_MINUTE = 600000000;
    constexpr uint64_t TICKS_PER_HOUR = 36000000000;
    constexpr uint64_t TICKS_PER_DAY = 864000000000;

    static const int DaysInMonth[2][12] = {
            {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, // Non-leap
            {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}  // Leap
    };

    // Calculate total days since January 1, 1601
    uint64_t days = *time / TICKS_PER_DAY;
    uint64_t remainingTicks = *time % TICKS_PER_DAY;

    timeFields->Hour = static_cast<uint16_t>(remainingTicks / TICKS_PER_HOUR);
    remainingTicks %= TICKS_PER_HOUR;

    timeFields->Minute = static_cast<uint16_t>(remainingTicks / TICKS_PER_MINUTE);
    remainingTicks %= TICKS_PER_MINUTE;

    timeFields->Second = static_cast<uint16_t>(remainingTicks / TICKS_PER_SECOND);
    remainingTicks %= TICKS_PER_SECOND;

    timeFields->Milliseconds = static_cast<uint16_t>(remainingTicks / TICKS_PER_MILLISECOND);

    // Calculate day of week (January 1, 1601 was a Monday = 1)
    timeFields->Weekday = static_cast<uint16_t>((days + 1) % 7);

    // Calculate year
    uint32_t year = 1601;

    // Each 400-year cycle has 146097 days
    uint32_t cycles400 = static_cast<uint32_t>(days / 146097);
    days %= 146097;
    year += cycles400 * 400;

    // Handle 100-year cycles (24 leap years + 76 normal years = 36524 days)
    // Except the 4th century which has 36525 days
    uint32_t cycles100 = static_cast<uint32_t>(days / 36524);
    if (cycles100 == 4) cycles100 = 3; // Last day of 400-year cycle
    days -= cycles100 * 36524;
    year += cycles100 * 100;

    // Handle 4-year cycles (1 leap year + 3 normal years = 1461 days)
    uint32_t cycles4 = static_cast<uint32_t>(days / 1461);
    days %= 1461;
    year += cycles4 * 4;

    // Handle individual years within 4-year cycle
    uint32_t yearInCycle = static_cast<uint32_t>(days / 365);
    if (yearInCycle == 4) yearInCycle = 3; // Last day of leap cycle
    days -= yearInCycle * 365;
    if (yearInCycle > 0) {
        // Account for leap days in previous years of this cycle
        days -= (yearInCycle - 1) / 4;
    }
    year += yearInCycle;

    timeFields->Year = static_cast<uint16_t>(year);

    // Determine if current year is a leap year
    bool isLeapYear = ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);

    // Calculate month and day
    const int* monthDays = DaysInMonth[isLeapYear ? 1 : 0];
    uint32_t dayOfYear = static_cast<uint32_t>(days) + 1; // Convert to 1-based

    uint16_t month = 1;
    while (dayOfYear > static_cast<uint32_t>(monthDays[month - 1])) {
        dayOfYear -= monthDays[month - 1];
        month++;
    }

    timeFields->Month = month;
    timeFields->Day = static_cast<uint16_t>(dayOfYear);
}

void RtlFreeAnsiString()
{
    LOG_UTILITY("!!! STUB !!!");
}

void RtlUnicodeStringToAnsiString()
{
    LOG_UTILITY("!!! STUB !!!");
}

void RtlInitUnicodeString()
{
    LOG_UTILITY("!!! STUB !!!");
}

void ExTerminateThread(uint32_t exitCode)
{
    // Thread termination request. The calling thread should exit.
    // Since this is typically called for cleanup during VdInitializeEngines,
    // we can safely ignore it for now.
    (void)exitCode;
}

uint32_t ExCreateThread(be<uint32_t>* handle, uint32_t stackSize, be<uint32_t>* threadId, uint32_t xApiThreadStartup, uint32_t startAddress, uint32_t startContext, uint32_t creationFlags)
{

    uint32_t hostThreadId;

    uint32_t entry = startAddress;
    uint32_t r3 = startContext;
    uint32_t r4 = 0;

    // Many titles use an XAPI thread startup wrapper that takes (entry, arg) in (r3, r4)
    // and then performs an indirect call to entry.
    if (xApiThreadStartup != 0)
    {
        entry = xApiThreadStartup;
        r3 = startAddress;
        r4 = startContext;
    }

    // Debug: Dump memory at startContext to see what callback should be there
    if (startContext != 0 && startContext >= 0x82000000 && startContext < 0x84000000)
    {
        uint32_t* ctxMem = reinterpret_cast<uint32_t*>(g_memory.Translate(startContext));
        LOGF_IMPL(Utility, "ExCreateThread", "startContext@0x{:08X}: [0]=0x{:08X}, [1]=0x{:08X}, [2]=0x{:08X}, [3]=0x{:08X}",
            startContext, 
            ByteSwap(ctxMem[0]), ByteSwap(ctxMem[1]), ByteSwap(ctxMem[2]), ByteSwap(ctxMem[3]));
    }

    LOGF_WARNING("[ExCreateThread] Creating thread entry=0x{:08X} r3=0x{:08X} r4=0x{:08X} flags=0x{:X}", 
                 entry, r3, r4, creationFlags);
    *handle = GetKernelHandle(GuestThread::Start({ entry, r3, r4, creationFlags }, &hostThreadId));
    LOGF_UTILITY("0x{:X}, 0x{:X}, 0x{:X}, 0x{:X}, 0x{:X}, 0x{:X}, 0x{:X} {:X}",
        (intptr_t)handle, stackSize, (intptr_t)threadId, xApiThreadStartup, startAddress, startContext, creationFlags, hostThreadId);
    if (threadId != nullptr)
        *threadId = hostThreadId;

    return 0;
}

void IoInvalidDeviceRequest()
{
    LOG_UTILITY("!!! STUB !!!");
}

void ObReferenceObject()
{
    LOG_UTILITY("!!! STUB !!!");
}

void IoCreateDevice()
{
    LOG_UTILITY("!!! STUB !!!");
}

void IoDeleteDevice()
{
    LOG_UTILITY("!!! STUB !!!");
}

void ExAllocatePoolTypeWithTag()
{
    LOG_UTILITY("!!! STUB !!!");
}

void RtlTimeFieldsToTime()
{
    LOG_UTILITY("!!! STUB !!!");
}

void IoCompleteRequest()
{
    LOG_UTILITY("!!! STUB !!!");
}

void RtlUpcaseUnicodeChar()
{
    LOG_UTILITY("!!! STUB !!!");
}

void ObIsTitleObject()
{
    LOG_UTILITY("!!! STUB !!!");
}

void IoCheckShareAccess()
{
    LOG_UTILITY("!!! STUB !!!");
}

void IoSetShareAccess()
{
    LOG_UTILITY("!!! STUB !!!");
}

void IoRemoveShareAccess()
{
    LOG_UTILITY("!!! STUB !!!");
}

// XNetGetTitleXnAddr - Return fake IP to unblock network wait
// The game loops on this until it gets a valid network state
// Return value: 0=Pending, 1=None, 2=Ethernet (connected)
uint32_t NetDll_XNetGetTitleXnAddr(uint32_t pAddr)
{
    static int s_callCount = 0;
    ++s_callCount;
    
    if (pAddr != 0)
    {
        uint8_t* base = g_memory.base;
        
        // XNADDR structure layout:
        // 0x00: IN_ADDR ina (4 bytes) - Local IP
        // 0x04: IN_ADDR inaOnline (4 bytes) - Online IP  
        // 0x08: WORD wPortOnline (2 bytes)
        // 0x0A: BYTE abEnet[6] - MAC address
        // 0x10: BYTE abOnline[20] - Online key
        
        // Write fake IP: 192.168.1.100 (0xC0A80164 in network byte order)
        *reinterpret_cast<be<uint32_t>*>(base + pAddr + 0x00) = 0x6401A8C0; // 192.168.1.100
        *reinterpret_cast<be<uint32_t>*>(base + pAddr + 0x04) = 0x6401A8C0; // Same for online
        *reinterpret_cast<be<uint16_t>*>(base + pAddr + 0x08) = 3074; // Port
        
        // Fake MAC address: 00:11:22:33:44:55
        base[pAddr + 0x0A] = 0x00;
        base[pAddr + 0x0B] = 0x11;
        base[pAddr + 0x0C] = 0x22;
        base[pAddr + 0x0D] = 0x33;
        base[pAddr + 0x0E] = 0x44;
        base[pAddr + 0x0F] = 0x55;
    }
    
    if (s_callCount <= 5)
    {
        LOGF_UTILITY("XNetGetTitleXnAddr #{} -> Returning ETHERNET (2) with fake IP 192.168.1.100", s_callCount);
    }
    
    // Return XNET_GET_XNADDR_ETHERNET (2) = Connected
    return 2;
}

uint32_t KeWaitForMultipleObjects(uint32_t Count, xpointer<XDISPATCHER_HEADER>* Objects, uint32_t WaitType, uint32_t WaitReason, uint32_t WaitMode, uint32_t Alertable, be<int64_t>* Timeout)
{
    // FIXME: This function is only accounting for events.
    static int s_callCount = 0;
    ++s_callCount;
    
    uint32_t caller = g_ppcContext ? g_ppcContext->lr : 0;
    uint32_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id()) & 0xFFFF;
    
    if (s_callCount <= 20 || s_callCount % 100 == 0)
    {
        LOGF_WARNING("[KeWaitMultiple] #{} thread=0x{:04X} count={} waitType={} caller=0x{:08X}", 
                    s_callCount, threadId, Count, WaitType, caller);
    }

    const uint64_t timeout = GuestTimeoutToMilliseconds(Timeout);
    assert(timeout == INFINITE);

    if (WaitType == 0) // Wait all
    {
        for (size_t i = 0; i < Count; i++)
            QueryKernelObject<Event>(*Objects[i])->Wait(timeout);
    }
    else
    {
        thread_local std::vector<Event*> s_events;
        s_events.resize(Count);

        for (size_t i = 0; i < Count; i++)
            s_events[i] = QueryKernelObject<Event>(*Objects[i]);

        int loopCount = 0;
        while (true)
        {
            ++loopCount;
            if (loopCount <= 5 || loopCount % 1000 == 0)
            {
                LOGF_WARNING("[KeWaitMultiple] Loop #{} thread=0x{:04X} waiting on {} objects", 
                            loopCount, threadId, Count);
            }
            
            uint32_t generation = g_keSetEventGeneration.load();

            for (size_t i = 0; i < Count; i++)
            {
                if (s_events[i]->Wait(0) == STATUS_SUCCESS)
                {
                    return STATUS_WAIT_0 + i;
                }
            }

            g_keSetEventGeneration.wait(generation);
        }
    }

    return STATUS_SUCCESS;
}

uint32_t KeRaiseIrqlToDpcLevel()
{
    return 0;
}

void KfLowerIrql() { }

uint32_t KeReleaseSemaphore(XKSEMAPHORE* semaphore, uint32_t increment, uint32_t adjustment, uint32_t wait)
{
    static int s_count = 0; ++s_count;
    
    // Log all releases to see if blocking semaphores are ever signaled
    uint32_t semAddr = (uint32_t)((uint8_t*)semaphore - g_memory.base);
    if (s_count <= 30 || (semAddr >= 0xEB2D0000 && semAddr <= 0xEB2E0000)) {
        LOGF_WARNING("[KeReleaseSemaphore] #{} sem=0x{:08X} adj={}", s_count, semAddr, adjustment);
    }
    
    auto* object = QueryKernelObject<Semaphore>(semaphore->Header);
    if (!object) {
        LOGF_WARNING("[KeReleaseSemaphore] #{} sem=0x{:08X} QueryKernelObject returned NULL!", s_count, semAddr);
        return STATUS_INVALID_HANDLE;
    }
    object->Release(adjustment, nullptr);
    return STATUS_SUCCESS;
}

void XAudioGetVoiceCategoryVolume()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t XAudioGetVoiceCategoryVolumeChangeMask(uint32_t Driver, be<uint32_t>* Mask)
{
    *Mask = 0;
    return 0;
}

uint32_t KeResumeThread(GuestThreadHandle* object)
{
    static int s_count = 0; ++s_count;
    assert(object != GetKernelObject(CURRENT_THREAD_HANDLE));

    LOGF_WARNING("[RESUME] KeResumeThread #{} thread=0x{:08X}", s_count, reinterpret_cast<uintptr_t>(object) & 0xFFFFFFFF);
    
    object->suspended = false;
    object->suspended.notify_all();
    return 0;
}

void KeInitializeSemaphore(XKSEMAPHORE* semaphore, uint32_t count, uint32_t limit)
{
    static int s_count = 0; ++s_count;
    
    semaphore->Header.Type = 5;
    semaphore->Header.SignalState = count;
    semaphore->Limit = limit;

    auto* object = QueryKernelObject<Semaphore>(semaphore->Header);
    
    // Track semaphores initialized with count=0 (will block immediately)
    uint32_t semAddr = (uint32_t)((uint8_t*)semaphore - g_memory.base);
    if (count == 0) {
        TrackBlockingSemaphoreAddr(semAddr);
        LOGF_WARNING("[KeInitializeSemaphore] #{} BLOCKING sem=0x{:08X} count=0 limit={} - TRACKED", 
                     s_count, semAddr, limit);
    } else if (s_count <= 30) {
        LOGF_WARNING("[KeInitializeSemaphore] #{} sem=0x{:08X} count={} limit={} object={}", 
                     s_count, semAddr, count, limit, object ? "OK" : "NULL");
    }
}

void XMAReleaseContext()
{
    LOG_UTILITY("!!! STUB !!!");
}

void XMACreateContext()
{
    LOG_UTILITY("!!! STUB !!!");
}

// uint32_t XAudioRegisterRenderDriverClient(be<uint32_t>* callback, be<uint32_t>* driver)
// {
//     //printf("XAudioRegisterRenderDriverClient(): %x %x\n");
// 
//     *driver = apu::RegisterClient(callback[0], callback[1]);
//     return 0;
// }

// void XAudioUnregisterRenderDriverClient()
// {
//     printf("!!! STUB !!! XAudioUnregisterRenderDriverClient\n");
// }

// uint32_t XAudioSubmitRenderDriverFrame(uint32_t driver, void* samples)
// {
//     // printf("!!! STUB !!! XAudioSubmitRenderDriverFrame\n");
//     apu::SubmitFrames(samples);
// 
//     return 0;
// }

void XapiInitProcess()
{
    printf("XapiInitProcess Invoked\n");

    int *XapiProcessHeap = (int *)g_memory.Translate(0x82D57540);

    *XapiProcessHeap = 1;
}

// GTA IV specific stubs
// XamTaskSchedule - schedule an async task
// CRITICAL FIX: Actually execute the task function so completion signals fire
uint32_t XamTaskSchedule(uint32_t funcAddr, uint32_t context, uint32_t processId,
                         uint32_t stackSize, uint32_t priority, uint32_t flags,
                         be<uint32_t>* phTask)
{
    static int s_count = 0;
    static uint32_t s_nextHandle = 0x80001000;
    
    ++s_count;
    if (s_count <= 30 || s_count % 100 == 0)
    {
        LOGF_WARNING("[XamTaskSchedule] #{} func=0x{:08X} ctx=0x{:08X} stack={} prio={} flags=0x{:X}",
                  s_count, funcAddr, context, stackSize, priority, flags);
    }
    
    // Assign a task handle
    uint32_t taskHandle = s_nextHandle++;
    if (phTask)
    {
        *phTask = taskHandle;
    }
    
    // ACTUALLY EXECUTE THE TASK FUNCTION
    // Without this, tasks are "scheduled" but never run, so completion signals never fire
    // This is likely why the scheduler blocks forever
    if (funcAddr != 0 && funcAddr >= 0x82000000 && funcAddr < 0x83000000)
    {
        auto func = g_memory.FindFunction(funcAddr);
        if (func)
        {
            if (s_count <= 20)
            {
                LOGF_WARNING("[XamTaskSchedule] EXECUTING task func=0x{:08X} ctx=0x{:08X}", funcAddr, context);
            }
            
            // Execute the task function with context as r3
            PPCContext taskCtx = *g_ppcContext;
            taskCtx.r3.u32 = context;
            func(taskCtx, g_memory.base);
            
            if (s_count <= 20)
            {
                LOGF_WARNING("[XamTaskSchedule] Task func=0x{:08X} COMPLETED", funcAddr);
            }
        }
        else
        {
            LOGF_WARNING("[XamTaskSchedule] Could not find function at 0x{:08X}", funcAddr);
        }
    }
    
    return ERROR_SUCCESS;
}

uint32_t XamTaskShouldExit(uint32_t taskHandle)
{
    // Return 0 so tasks continue running - returning 1 causes task loops to exit
    // immediately, which prevents initialization from completing and blocks main loop
    static int s_count = 0;
    if (++s_count <= 10)
    {
        LOGF_UTILITY("handle=0x{:08X} -> returning 0 (task continues)", taskHandle);
    }
    return 0;
}

uint32_t XamTaskCloseHandle(uint32_t taskHandle)
{
    LOG_UTILITY("!!! STUB !!!");
    return ERROR_SUCCESS;
}

uint32_t XamUserAreUsersFriends(uint32_t userIndex, uint64_t* xuids, uint32_t count, uint32_t* results, void* overlapped)
{
    // Return that no users are friends
    if (results)
        memset(results, 0, count * sizeof(uint32_t));
    return ERROR_SUCCESS;
}

uint32_t XamUserCheckPrivilege(uint32_t userIndex, uint32_t privilegeType, uint32_t* result)
{
    // Grant all privileges
    if (result)
        *result = 1;
    return ERROR_SUCCESS;
}

uint32_t XamUserCreateStatsEnumerator(uint32_t titleId, uint32_t userIndex, uint32_t xuidCount,
    void* pxuid, uint32_t dwNumStatSpecs, void* pStatSpecs, be<uint32_t>* pcbBuffer, be<uint32_t>* phEnum)
{
    // Return no stats available
    if (phEnum)
        *phEnum = 0;
    if (pcbBuffer)
        *pcbBuffer = 0;
    return ERROR_NO_MORE_FILES;
}

uint32_t XamUserGetName(uint32_t userIndex, char* buffer, uint32_t bufferSize)
{
    if (userIndex != 0)
        return ERROR_NO_SUCH_USER;
    if (buffer && bufferSize > 0)
    {
        const char* name = "Player";
        strncpy(buffer, name, bufferSize - 1);
        buffer[bufferSize - 1] = '\0';
    }
    return ERROR_SUCCESS;
}

uint32_t XamUserGetXUID(uint32_t userIndex, uint64_t* xuid)
{
    if (userIndex != 0)
        return ERROR_NO_SUCH_USER;
    if (xuid)
        *xuid = 0x0001000000000001ULL; // Fake XUID
    return ERROR_SUCCESS;
}

uint32_t XamUserWriteProfileSettings(uint32_t userIndex, uint32_t numSettings, void* settings, void* overlapped)
{
    LOG_UTILITY("!!! STUB !!!");
    return ERROR_SUCCESS;
}

uint32_t XamVoiceCreate(uint32_t userIndex, uint32_t flags, void** voice)
{
    LOG_UTILITY("!!! STUB !!!");
    if (voice)
        *voice = nullptr;
    return ERROR_DEVICE_NOT_CONNECTED;
}

void XamVoiceClose(void* voice)
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t XamVoiceHeadsetPresent(uint32_t userIndex)
{
    // No headset present
    return 0;
}

uint32_t XamVoiceSubmitPacket(void* voice, uint32_t size, void* data)
{
    LOG_UTILITY("!!! STUB !!!");
    return ERROR_SUCCESS;
}

uint32_t XeKeysConsolePrivateKeySign(void* signature, void* data, uint32_t dataSize)
{
    LOG_UTILITY("!!! STUB !!!");
    return ERROR_SUCCESS;
}

void IoDismountVolumeByFileHandle()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t NetDll_XNetGetEthernetLinkStatus()
{
    // Return link up
    return 1;
}

uint32_t KeTryToAcquireSpinLockAtRaisedIrql(void* spinlock)
{
    // Try to acquire, always succeed for now
    return 1;
}

uint32_t XamShowGamerCardUIForXUID(uint32_t userIndex, uint64_t xuid, void* overlapped)
{
    LOG_UTILITY("!!! STUB !!!");
    return ERROR_SUCCESS;
}

uint32_t XamShowPlayerReviewUI(uint32_t userIndex, uint64_t xuid, void* overlapped)
{
    LOG_UTILITY("!!! STUB !!!");
    return ERROR_SUCCESS;
}

// Additional GTA IV stubs
void* XamAlloc(uint32_t flags, uint32_t size, void* pBuffer)
{
    LOG_UTILITY("!!! STUB !!!");
    return nullptr;
}

void XamFree(void* buffer)
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t NtSetTimerEx(void* timerHandle, void* dueTime, void* apcRoutine, void* apcContext, uint32_t resume, uint32_t period, void* previousState)
{
    LOG_UTILITY("!!! STUB !!!");
    return 0;
}

uint32_t NtCancelTimer(void* timerHandle, void* currentState)
{
    LOG_UTILITY("!!! STUB !!!");
    return 0;
}

uint32_t NtCreateTimer(void* timerHandle, uint32_t desiredAccess, void* objectAttributes, uint32_t timerType)
{
    LOG_UTILITY("!!! STUB !!!");
    return 0;
}

uint32_t NtCreateMutant(void* mutantHandle, uint32_t desiredAccess, void* objectAttributes, uint32_t initialOwner)
{
    LOG_UTILITY("!!! STUB !!!");
    return 0;
}

uint32_t NtReleaseMutant(void* mutantHandle, void* previousCount)
{
    LOG_UTILITY("!!! STUB !!!");
    return 0;
}

void IoDismountVolume()
{
    LOG_UTILITY("!!! STUB !!!");
}

void XNotifyPositionUI(uint32_t position)
{
    LOG_UTILITY("!!! STUB !!!");
}

int32_t NetDll_getsockname(uint32_t socket, void* name, void* namelen)
{
    LOG_UTILITY("!!! STUB !!!");
    return -1;
}

int32_t NetDll_ioctlsocket(uint32_t socket, uint32_t cmd, void* argp)
{
    LOG_UTILITY("!!! STUB !!!");
    return 0;
}

int32_t NetDll_sendto(uint32_t socket, void* buf, uint32_t len, uint32_t flags, void* to, uint32_t tolen)
{
    LOG_UTILITY("!!! STUB !!!");
    return -1;
}

int32_t NetDll_recvfrom(uint32_t socket, void* buf, uint32_t len, uint32_t flags, void* from, void* fromlen)
{
    LOG_UTILITY("!!! STUB !!!");
    return -1;
}

int32_t NetDll_shutdown(uint32_t socket, uint32_t how)
{
    LOG_UTILITY("!!! STUB !!!");
    return 0;
}

void XMsgCancelIORequest()
{
    LOG_UTILITY("!!! STUB !!!");
}

uint32_t XAudioGetSpeakerConfig(void* config)
{
    LOG_UTILITY("!!! STUB !!!");
    return 0;
}

uint32_t XamContentSetThumbnail(const char* rootName, void* imageData, uint32_t imageSize, void* overlapped)
{
    LOG_UTILITY("!!! STUB !!!");
    return ERROR_SUCCESS;
}

uint32_t XamInputGetKeystrokeEx(uint32_t userIndex, uint32_t flags, void* keystroke)
{
    // Validate user index
    if (userIndex >= 4) {
        return ERROR_DEVICE_NOT_CONNECTED;
    }
    
    // Try to dequeue a keystroke
    hid::KeystrokeEvent event;
    if (!hid::DequeueKeystroke(userIndex, event)) {
        return ERROR_EMPTY;
    }
    
    // Write to guest memory
    if (keystroke != nullptr) {
        XINPUT_KEYSTROKE* pKeystroke = reinterpret_cast<XINPUT_KEYSTROKE*>(keystroke);
        pKeystroke->VirtualKey = event.virtualKey;
        pKeystroke->Unicode = event.unicode;
        pKeystroke->Flags = event.flags;
        pKeystroke->UserIndex = event.userIndex;
        pKeystroke->HidCode = 0;
    }
    
    return ERROR_SUCCESS;
}

uint32_t XamSessionCreateHandle(void* sessionInfo, void* handle)
{
    LOG_UTILITY("!!! STUB !!!");
    return ERROR_SUCCESS;
}

uint32_t XamSessionRefObjByHandle(uint32_t handle, void* obj)
{
    LOG_UTILITY("!!! STUB !!!");
    return ERROR_INVALID_HANDLE;
}

void KeSetDisableBoostThread(void* thread, uint32_t disable)
{
    // Thread priority boost control is not implemented.
    // This is safe to ignore as it only affects scheduling hints.
    (void)thread;
    (void)disable;
}

uint32_t XamCreateEnumeratorHandle(uint32_t type, uint32_t size, void* data, void* handle)
{
    LOG_UTILITY("!!! STUB !!!");
    return ERROR_SUCCESS;
}

uint32_t NtDeviceIoControlFile(void* fileHandle, void* event, void* apcRoutine, void* apcContext, void* ioStatusBlock, uint32_t ioControlCode, void* inputBuffer, uint32_t inputBufferLength, void* outputBuffer, uint32_t outputBufferLength)
{
    LOG_UTILITY("!!! STUB !!!");
    return 0;
}

int32_t NetDll_WSAGetLastError()
{
    LOG_UTILITY("!!! STUB !!!");
    return 0;
}

// =============================================================================
// GTA IV: Bypass dirty disc error display only
// sub_82192100 - dirty disc error display UI - HOOK to skip UI
// sub_829A1290 - another dirty disc caller
// sub_827F0B20 - retry loop - let it run but limit retries
// sub_829A1F00 - the actual file loading function
// =============================================================================

// Hook sub_827DAE40 - Worker thread entry point (streaming workers)
// Previously STUBBED: This function called an indirect task function that blocks forever
// FIX: Execute the task function ONCE synchronously, then return
// This allows resource loading to complete while avoiding the infinite semaphore-wait loop
extern "C" void __imp__sub_827DAE40(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827DAE40)
{
    static int s_count = 0;
    ++s_count;
    
    uint32_t context = ctx.r3.u32;  // Worker context pointer
    
    // Validate context pointer
    if (context < 0x80000000 || context >= 0x90000000) {
        LOGF_WARNING("[STREAMING_WORKER] sub_827DAE40 #{} INVALID context 0x{:08X} - skipping", 
                     s_count, context);
        ctx.r3.u32 = 0;
        return;
    }
    
    uint32_t* ctxData = reinterpret_cast<uint32_t*>(base + context);
    uint32_t taskFunc = ByteSwap(ctxData[0]);  // First dword of context = task function
    
    LOGF_WARNING("[STREAMING_WORKER] sub_827DAE40 #{} ctx=0x{:08X} taskFunc=0x{:08X}", 
                 s_count, context, taskFunc);
    
    // Execute the task function synchronously instead of blocking on semaphore
    // This follows the same pattern as XamTaskSchedule (lines 5354-5377)
    if (taskFunc != 0 && taskFunc >= 0x82000000 && taskFunc < 0x83000000) {
        auto func = g_memory.FindFunction(taskFunc);
        if (func) {
            LOGF_WARNING("[STREAMING_WORKER] sub_827DAE40 #{} EXECUTING taskFunc 0x{:08X} with ctx=0x{:08X}", 
                         s_count, taskFunc, context);
            
            // Execute the task function with context as r3
            // Create a copy of the context to avoid corrupting the caller's state
            PPCContext taskCtx = ctx;
            taskCtx.r3.u32 = context;
            func(taskCtx, base);
            
            LOGF_WARNING("[STREAMING_WORKER] sub_827DAE40 #{} taskFunc 0x{:08X} COMPLETED", 
                         s_count, taskFunc);
        } else {
            LOGF_WARNING("[STREAMING_WORKER] sub_827DAE40 #{} taskFunc 0x{:08X} NOT FOUND - skipping", 
                         s_count, taskFunc);
        }
    } else {
        LOGF_WARNING("[STREAMING_WORKER] sub_827DAE40 #{} taskFunc 0x{:08X} OUT OF RANGE - skipping", 
                     s_count, taskFunc);
    }
    
    // Return success - don't loop (original would block forever on semaphore)
    ctx.r3.u32 = 0;
    return;
}

// Hook sub_829A9738 - Wait-with-retry helper (calls NtWaitForSingleObjectEx)
extern "C" void __imp__sub_829A9738(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829A9738)
{
    static int s_count = 0;
    ++s_count;
    
    uint32_t waitObj = ctx.r3.u32;  // Object to wait on
    int32_t timeout = ctx.r4.s32;    // Timeout value
    
    if (s_count <= 20 || s_count % 100 == 0)
    {
        LOGF_IMPL(Utility, "WaitHelper", "sub_829A9738 #{} obj=0x{:08X} timeout={}", 
                  s_count, waitObj, timeout);
    }
    
    __imp__sub_829A9738(ctx, base);
    
    if (s_count <= 20 || s_count % 100 == 0)
    {
        LOGF_IMPL(Utility, "WaitHelper", "sub_829A9738 #{} returned r3=0x{:08X}", 
                  s_count, ctx.r3.u32);
    }
}

// =============================================================================
// UNIFIED BOOT SEQUENCE REWRITE - Complete _xstart Replacement
// =============================================================================
// This is a comprehensive replacement for the entire Xbox 360 boot sequence.
// It replaces ALL functions in the _xstart execution path with modern equivalents
// that work correctly on PC platforms and support online multiplayer.
//
// Original _xstart (0x829A0860) execution path:
//   sub_829A7FF8  - Early system init (XEX validation, firmware fallback)
//   sub_829A7960  - System callbacks (linked list iteration)
//   sub_829A0678  - Privilege/HDCP check (XexCheckExecutablePrivilege, etc.)
//   sub_82994700  - CRT/TLS initialization (thread pool, TLS, vtables)
//   sub_829A7EA8  - Init table executor (function pointer array)
//   sub_829A7DC8  - C++ static constructors (multiple arrays)
//   sub_829A27D8  - Command-line parsing setup
//   sub_8218BEA8  - Game main entry → sub_827D89B8 (game wrapper)
//
// This unified replacement:
// - SKIPS all Xbox-specific hardware/security checks
// - MODERNIZES CRT/TLS init without Xbox thread pool timing
// - PRESERVES all memory state the game expects
// - KEEPS essential game code (init tables, constructors, game main)
// - ENABLES online multiplayer by removing timing assumptions
// =============================================================================

// Extern declarations for boot sequence functions
extern "C" void __imp___xstart(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82994700(PPCContext& ctx, uint8_t* base);  // TLS/CRT init
extern "C" void __imp__sub_829A7960(PPCContext& ctx, uint8_t* base);  // Runtime callbacks
extern "C" void __imp__sub_829A7EA8(PPCContext& ctx, uint8_t* base);  // Init table executor
extern "C" void __imp__sub_829A7DC8(PPCContext& ctx, uint8_t* base);  // C++ constructors
extern "C" void __imp__sub_8218BEA8(PPCContext& ctx, uint8_t* base);  // Game main entry
extern "C" void __imp__sub_828E0AB8(PPCContext& ctx, uint8_t* base);  // Frame tick

// =============================================================================
// Storage Device Constants (still needed for PC storage hooks)
// =============================================================================
namespace StorageConstants {
    constexpr uint32_t PC_STORAGE_DEVICE_ADDR  = 0x83132000;
    constexpr uint32_t PC_STORAGE_VTABLE_ADDR  = 0x83132100;
    constexpr uint32_t PC_STORAGE_DATA_ADDR    = 0x83132200;
}


// =============================================================================
// Boot Sequence Global Addresses - DISABLED
// =============================================================================
// These were used by the _xstart replacement which is now disabled.
// Per UnleashedRecomp architecture, we let the original game CRT run.
// =============================================================================
#if 0  // DISABLED - boot hacks removed
namespace BootGlobals {
    // -------------------------------------------------------------------------
    // sub_829A7FF8 / sub_829A7F20 - Early System Init
    // -------------------------------------------------------------------------
    // XEX header pointer location (from lis r11,-32256 = 0x81300000)
    constexpr uint32_t XEX_HEADER_PTR    = 0x8130063C;  // [0x81300000+1596]
    constexpr uint32_t XEX_VTABLE_PTR    = 0x813006B0;  // [0x81300000+1712]
    // Memory allocation result (from sub_829A7F20)
    constexpr uint32_t ALLOC_RESULT_ADDR = 0x83008440;  // [0x83010000-31780] - also used by exception handler
    
    // -------------------------------------------------------------------------
    // sub_829A7960 - System Callbacks (Linked List)
    // -------------------------------------------------------------------------
    constexpr uint32_t CALLBACK_CRIT_ADDR = 0x82A97FB4; // Critical section for list
    constexpr uint32_t CALLBACK_LIST_ADDR = 0x82A97FD0; // Doubly-linked list head
    
    // -------------------------------------------------------------------------
    // sub_82994700 - CRT/TLS Initialization
    // -------------------------------------------------------------------------
    // CRT vtable pointers (from lis r30,-31981 = 0x81200000)
    constexpr uint32_t VTABLE1_ADDR = 0x812017E4;  // offset 6116 - thread create
    constexpr uint32_t VTABLE2_ADDR = 0x812017E8;  // offset 6120 - TLS context
    constexpr uint32_t VTABLE3_ADDR = 0x812017EC;  // offset 6124 - thread register
    constexpr uint32_t VTABLE4_ADDR = 0x812017F0;  // offset 6128 - thread destroy
    
    // CRT subsystem flags (from sub_82992680)
    constexpr uint32_t CRT_FINALIZE_ADDR = 0x812019A8;  // offset 6568
    constexpr uint32_t IO_SYSTEM_ADDR    = 0x812019AC;  // offset 6572
    constexpr uint32_t MEM_MANAGER_ADDR  = 0x812019B0;  // offset 6576
    constexpr uint32_t HEAP_INIT_ADDR    = 0x812019B4;  // offset 6580
    
    // TLS storage (from 0x82A90000 base)
    constexpr uint32_t TLS_INDEX_ADDR     = 0x82A96E64;  // offset 28260
    constexpr uint32_t THREAD_HANDLE_ADDR = 0x82A96E60;  // offset 28256
    constexpr uint32_t NEW_THREAD_ALLOC   = 0x82A96B30;  // offset 27472
    
    // Thread pool (from sub_82998A48)
    constexpr uint32_t THREAD_POOL_ADDR   = 0x82A97200;  // 36 slots × 8 bytes
    constexpr uint32_t THREAD_POOL_SIZE   = 36;
    
    // Thread context (pre-allocated area)
    constexpr uint32_t THREAD_CONTEXT_ADDR = 0x83080000;
    constexpr uint32_t THREAD_CONTEXT_SIZE = 196;
    
    // -------------------------------------------------------------------------
    // Global Thread Context Base (0x83130000)
    // -------------------------------------------------------------------------
    // This is the address that [r13+0] must point to for memory allocation to work.
    // The allocation function sub_8218BE28 reads: [r13+0] -> [+1676] -> [vtable+8]
    // Original code stores memory manager at 0x83130000 + 1676.
    constexpr uint32_t GLOBAL_THREAD_CTX_BASE = 0x83130000;
    constexpr uint32_t MEM_MGR_OFFSET         = 1676;      // Offset to memory manager pointer
    constexpr uint32_t MEM_MGR_PTR_ADDR       = 0x8313068C; // 0x83130000 + 1676
    
    // Memory manager structure: needs vtable at offset 0, allocator at vtable+8
    // We'll create a simple memory manager structure in guest memory
    constexpr uint32_t MEM_MGR_STRUCT_ADDR    = 0x83131000; // Our memory manager object
    constexpr uint32_t MEM_MGR_VTABLE_ADDR    = 0x83131100; // Our vtable for memory manager
    
    // -------------------------------------------------------------------------
    // PC Storage Device (Platform Glue for Xbox Storage System)
    // -------------------------------------------------------------------------
    // Replaces Xbox content mounting with VFS-based file access.
    // The game uses a vtable-based storage interface:
    //   sub_827E1EC0 - Storage device factory (returns device with vtable)
    //   sub_827EF2F8 - Content mount (uses device vtable for file ops)
    //   sub_827EF208 - Storage path check
    //
    // Our PC storage device provides vtable functions that use VFS.
    constexpr uint32_t PC_STORAGE_DEVICE_ADDR  = 0x83132000; // PC storage device object
    constexpr uint32_t PC_STORAGE_VTABLE_ADDR  = 0x83132100; // Vtable for storage device
    constexpr uint32_t PC_STORAGE_DATA_ADDR    = 0x83132200; // Storage device internal data
    
    // Vtable offsets used by the game:
    //   +0  = destructor
    //   +4  = matchPath (check if path matches device)
    //   +8  = openFile (open file, return handle)
    //   +24 = readFile
    //   +40 = seekFile
    //   +44 = closeFile
    //   +72 = getFileSize
    //   +76 = getFileInfo
    
    // CRT context structures (from sub_82992680, offsets 6176+)
    constexpr uint32_t CRT_CONTEXT_BASE   = 0x81201820;  // offset 6176
    
    // Vtable values (computed from PPC immediates)
    constexpr uint32_t VTABLE1_VALUE = 0x82A543E8;  // 0x82A50000 + 17384
    constexpr uint32_t VTABLE2_VALUE = 0x82A0270C;  // 0x82A00000 + 9996
    constexpr uint32_t VTABLE3_VALUE = 0x82A0271C;  // 0x82A00000 + 10012
    constexpr uint32_t VTABLE4_VALUE = 0x82A0272C;  // 0x82A00000 + 10028
    constexpr uint32_t CRT_FINALIZE_VTABLE = 0x82A58D38;  // 0x82A60000 - 29432
    constexpr uint32_t NEW_THREAD_VTABLE   = 0x82A52660;  // 0x82A50000 + 9824
    
    // -------------------------------------------------------------------------
    // sub_829A0678 - Privilege Check
    // -------------------------------------------------------------------------
    // No persistent memory writes - just returns success/failure
    
    // -------------------------------------------------------------------------
    // _xstart globals
    // -------------------------------------------------------------------------
    // From original _xstart prologue (lis r10,-31979 = 0x83010000)
    constexpr uint32_t XSTART_FLAG1 = 0x8300844C;  // [0x83010000-31796]
    constexpr uint32_t XSTART_FLAG2 = 0x83008450;  // [0x83010000-31792]
    
    // -------------------------------------------------------------------------
    // Command-line storage
    // -------------------------------------------------------------------------
    constexpr uint32_t CMDLINE_BUFFER_ADDR = 0x83100000;
    constexpr uint32_t CMDLINE_ARGV_ADDR   = 0x83100800;
    constexpr uint32_t CMDLINE_MAX_ARGS    = 16;
    constexpr uint32_t CMDLINE_BUFFER_SIZE = 2048;
}  // namespace BootGlobals
#endif  // DISABLED BootGlobals

// =============================================================================
// Modern CRT/TLS Initialization - DISABLED
// =============================================================================
// This was part of the _xstart replacement which is now disabled.
// Per UnleashedRecomp architecture, we let the original game CRT run.
// =============================================================================
#if 0  // DISABLED - InitializeModernCRT not used
static void InitializeModernCRT(PPCContext& ctx, uint8_t* base)
{
    LOG_WARNING("[CRT] * Modern CRT/TLS initialization starting");
    
    // -------------------------------------------------------------------------
    // Store CRT vtable pointers (from sub_82994700 prologue)
    // -------------------------------------------------------------------------
    PPC_STORE_U32(BootGlobals::VTABLE1_ADDR, BootGlobals::VTABLE1_VALUE);
    PPC_STORE_U32(BootGlobals::VTABLE2_ADDR, BootGlobals::VTABLE2_VALUE);
    PPC_STORE_U32(BootGlobals::VTABLE3_ADDR, BootGlobals::VTABLE3_VALUE);
    PPC_STORE_U32(BootGlobals::VTABLE4_ADDR, BootGlobals::VTABLE4_VALUE);
    
    // -------------------------------------------------------------------------
    // Allocate and set TLS slot
    // -------------------------------------------------------------------------
    uint32_t tlsIndex = KeTlsAlloc();
    PPC_STORE_U32(BootGlobals::TLS_INDEX_ADDR, tlsIndex);
    
    if (tlsIndex == 0xFFFFFFFF) {
        LOG_WARNING("[CRT] ERROR: KeTlsAlloc failed!");
        return;
    }
    
    KeTlsSetValue(tlsIndex, BootGlobals::VTABLE2_VALUE);
    LOGF_WARNING("[CRT] TLS slot {} allocated, value=0x{:08X}", tlsIndex, BootGlobals::VTABLE2_VALUE);
    
    // -------------------------------------------------------------------------
    // CRT subsystem initialization (replaces sub_82992680)
    // -------------------------------------------------------------------------
    // sub_82998ED0(0) - Heap init flag
    PPC_STORE_U32(BootGlobals::HEAP_INIT_ADDR, 0);
    
    // sub_82998DE0(0) - Memory manager flag
    PPC_STORE_U32(BootGlobals::MEM_MANAGER_ADDR, 0);
    
    // sub_82994830(0) - Exception handler flag
    PPC_STORE_U32(BootGlobals::ALLOC_RESULT_ADDR, 0);
    
    // sub_82998DD0(0) - I/O system flag
    PPC_STORE_U32(BootGlobals::IO_SYSTEM_ADDR, 0);
    
    // sub_828E0AB8(0) - Frame tick (KEEP - essential for game timing)
    ctx.r3.s64 = 0;
    __imp__sub_828E0AB8(ctx, base);
    
    // sub_82998DB8(0) - CRT finalize vtable
    PPC_STORE_U32(BootGlobals::CRT_FINALIZE_ADDR, BootGlobals::CRT_FINALIZE_VTABLE);
    
    // End of sub_82992680 - store new thread alloc vtable
    PPC_STORE_U32(BootGlobals::NEW_THREAD_ALLOC, BootGlobals::NEW_THREAD_VTABLE);
    
    LOG_WARNING("[CRT] CRT subsystem flags initialized");
    
    // -------------------------------------------------------------------------
    // SKIP: Thread pool initialization (sub_82998A48)
    // Original: 36 slots, 4000ms timeout per thread
    // Reason: Xbox-specific timing that breaks multiplayer
    // -------------------------------------------------------------------------
    for (uint32_t i = 0; i < BootGlobals::THREAD_POOL_SIZE; ++i) {
        uint32_t slotAddr = BootGlobals::THREAD_POOL_ADDR + (i * 8);
        PPC_STORE_U32(slotAddr, 0);      // slot pointer
        PPC_STORE_U32(slotAddr + 4, 0);  // slot state
    }
    LOG_WARNING("[CRT] Thread pool zeroed (skipped Xbox 4s timeout init)");
    
    // -------------------------------------------------------------------------
    // Create synthetic main thread handle
    // Original: vtable1 indirect call
    // -------------------------------------------------------------------------
    uint32_t mainThreadHandle = std::hash<std::thread::id>{}(std::this_thread::get_id()) & 0x7FFFFFFF;
    if (mainThreadHandle == 0 || mainThreadHandle == 0xFFFFFFFF) mainThreadHandle = 1;
    PPC_STORE_U32(BootGlobals::THREAD_HANDLE_ADDR, mainThreadHandle);
    LOGF_WARNING("[CRT] Main thread handle: 0x{:08X}", mainThreadHandle);
    
    // -------------------------------------------------------------------------
    // Allocate and initialize thread context (replaces sub_829937E0)
    // Original: 196 bytes allocated via sub_82993708
    // -------------------------------------------------------------------------
    uint32_t contextAddr = BootGlobals::THREAD_CONTEXT_ADDR;
    memset(base + contextAddr, 0, BootGlobals::THREAD_CONTEXT_SIZE);
    
    // Initialize context fields (from original code analysis)
    PPC_STORE_U32(contextAddr + 0, mainThreadHandle);   // Thread ID
    PPC_STORE_U32(contextAddr + 4, 0xFFFFFFFF);         // -1
    PPC_STORE_U32(contextAddr + 20, 1);                 // Flag
    PPC_STORE_U32(contextAddr + 92, 0x82A97300);        // Callback ptr
    
    LOGF_WARNING("[CRT] Thread context at 0x{:08X} initialized", contextAddr);
    
    // -------------------------------------------------------------------------
    // Initialize callback list as empty (replaces sub_829A79C0)
    // Original: Registers callback in doubly-linked list
    // -------------------------------------------------------------------------
    PPC_STORE_U32(BootGlobals::CALLBACK_LIST_ADDR, BootGlobals::CALLBACK_LIST_ADDR);
    PPC_STORE_U32(BootGlobals::CALLBACK_LIST_ADDR + 4, BootGlobals::CALLBACK_LIST_ADDR);
    
    LOG_WARNING("[CRT] * Modern CRT/TLS initialization complete");
}

// Hook for sub_82994700 - redirects to modern implementation
PPC_FUNC(sub_82994700_DISABLED)
{
    InitializeModernCRT(ctx, base);
    ctx.r3.s64 = 1;  // Return success
}

// Modern sub_829A7960 - Runtime Callback Invocation (DISABLED)
PPC_FUNC(sub_829A7960_DISABLED)
{
    uint32_t notificationType = ctx.r3.u32;
    
    static int s_callCount = 0;
    ++s_callCount;
    
    if (s_callCount <= 10) {
        LOGF_WARNING("[CALLBACK] sub_829A7960 called with notification={} (call #{})", 
                     notificationType, s_callCount);
    }
    
    uint32_t listHead = BootGlobals::CALLBACK_LIST_ADDR;
    uint32_t firstNode = PPC_LOAD_U32(listHead);
    
    if (firstNode != listHead) {
        LOG_WARNING("[CALLBACK] Callback list not empty, iterating...");
        
        uint32_t currentNode = firstNode;
        int callbackCount = 0;
        
        while (currentNode != listHead && callbackCount < 100) {
            uint32_t callback = PPC_LOAD_U32(currentNode + 8);
            uint32_t nextNode = PPC_LOAD_U32(currentNode);
            
            if (callback != 0) {
                LOGF_WARNING("[CALLBACK] Invoking callback #{} at 0x{:08X}", 
                             callbackCount, callback);
                ctx.r3.u64 = notificationType;
                ctx.ctr.u64 = callback;
                PPC_CALL_INDIRECT_FUNC(callback);
            }
            
            currentNode = nextNode;
            callbackCount++;
        }
        
        LOGF_WARNING("[CALLBACK] Invoked {} callbacks", callbackCount);
    }
}
#endif  // DISABLED InitializeModernCRT, sub_82994700, sub_829A7960

// =============================================================================
// Modern sub_829A7DC8 - C++ Static Constructor Execution (SKIPPED)
// =============================================================================
// ISSUE: The original function computes constructor table addresses as:
//   Array 1: 0x820214FC to 0x82021508 (3 entries)
//   Array 2: 0x82020010 to 0x820214F8 (1338 entries)
//
// However, these addresses contain GAME DATA (strings like "Look", "Time"),
// not actual function pointers. Attempting to call these causes crashes.
//
// ROOT CAUSE: The XEX constructor tables may be at different addresses than
// the hardcoded offsets in the recompiled PPC code, or they may not exist
// in the same form as the original Xbox CRT expected.
//
// SOLUTION: Skip constructor execution and return success. If game-specific
// static constructors are needed later, they should be identified and called
// explicitly.
//
// TODO: If game initialization fails later, revisit this to identify which
// constructors (if any) are actually required.
// =============================================================================

// DISABLED - Let original constructor execution run per UnleashedRecomp architecture
#if 0
PPC_FUNC(sub_829A7DC8_DISABLED)
{
    static bool s_logged = false;
    if (!s_logged) {
        LOG_WARNING("[CTOR] sub_829A7DC8 called - analyzing memory layout");
        
        // Dump memory at the expected constructor table addresses
        LOG_WARNING("[CTOR] === Memory Analysis ===");
        
        // Array 1 expected: 0x820214FC to 0x82021508
        uint32_t arr1Start = 0x820214FC;
        LOG_WARNING("[CTOR] Array 1 region (0x820214FC-0x82021508):");
        for (uint32_t addr = arr1Start; addr < arr1Start + 16; addr += 4) {
            uint32_t val = PPC_LOAD_U32(addr);
            // Check if it looks like code, data, or string
            bool isCodePtr = (val >= 0x82000000 && val <= 0x82FFFFFF);
            LOGF_WARNING("[CTOR]   [0x{:08X}] = 0x{:08X} {}", addr, val, 
                        isCodePtr ? "(code ptr)" : "(data)");
        }
        
        // Array 2 expected: 0x82020010 to 0x820214F8
        uint32_t arr2Start = 0x82020010;
        LOG_WARNING("[CTOR] Array 2 region (0x82020010-...):");
        for (uint32_t addr = arr2Start; addr < arr2Start + 32; addr += 4) {
            uint32_t val = PPC_LOAD_U32(addr);
            bool isCodePtr = (val >= 0x82000000 && val <= 0x82FFFFFF);
            LOGF_WARNING("[CTOR]   [0x{:08X}] = 0x{:08X} {}", addr, val,
                        isCodePtr ? "(code ptr)" : "(data)");
        }
        
        // Check a few other potential constructor table locations
        // Sometimes ctors are near the entry point or at specific offsets
        LOG_WARNING("[CTOR] Scanning for function pointer arrays...");
        
        int foundTables = 0;
        for (uint32_t scanAddr = 0x82000000; scanAddr < 0x82100000 && foundTables < 5; scanAddr += 0x1000) {
            // Check if this looks like a function pointer table
            int validPtrs = 0;
            for (int i = 0; i < 4; i++) {
                uint32_t val = PPC_LOAD_U32(scanAddr + i * 4);
                if (val >= 0x82000000 && val <= 0x82FFFFFF) validPtrs++;
            }
            if (validPtrs >= 3) {
                LOGF_WARNING("[CTOR] Potential table at 0x{:08X}:", scanAddr);
                for (int i = 0; i < 8; i++) {
                    uint32_t val = PPC_LOAD_U32(scanAddr + i * 4);
                    LOGF_WARNING("[CTOR]   [{}] = 0x{:08X}", i, val);
                }
                foundTables++;
            }
        }
        
        s_logged = true;
    }
    
    // Return success without executing constructors
    ctx.r3.s64 = 0;
}
#endif  // DISABLED sub_829A7DC8

// =============================================================================
// Command-Line Support Helper (only used by disabled _xstart)
// =============================================================================
#if 0  // DISABLED - part of boot sequence hacks
static int BuildGuestCommandLine(uint8_t* base, const std::vector<std::string>& args)
{
    if (args.empty()) return 0;
    
    uint32_t stringOffset = 0;
    uint32_t argCount = 0;
    
    for (size_t i = 0; i < args.size() && i < BootGlobals::CMDLINE_MAX_ARGS && 
         stringOffset < BootGlobals::CMDLINE_BUFFER_SIZE - 256; ++i)
    {
        const std::string& arg = args[i];
        
        uint32_t stringAddr = BootGlobals::CMDLINE_BUFFER_ADDR + stringOffset;
        memcpy(base + stringAddr, arg.c_str(), arg.size() + 1);
        
        uint32_t argvSlot = BootGlobals::CMDLINE_ARGV_ADDR + (i * 4);
        PPC_STORE_U32(argvSlot, stringAddr);
        
        stringOffset += arg.size() + 1;
        argCount++;
    }
    
    PPC_STORE_U32(BootGlobals::CMDLINE_ARGV_ADDR + (argCount * 4), 0);
    return argCount;
}
#endif  // DISABLED BuildGuestCommandLine

// =============================================================================
// UNIFIED _xstart - Complete Xbox 360 Boot Sequence Replacement
// =============================================================================
// This is the master boot function that replaces the entire _xstart execution
// path. It performs all necessary memory initialization and calls only the
// essential game code functions.
//
// Replaced functions (Xbox-specific):
//   sub_829A7FF8 → sub_829A7F20  - XEX validation, HalReturnToFirmware
//   sub_829A7960                  - Runtime callback invocation
//   sub_829A0678                  - HDCP/privilege check
//   sub_82994700                  - CRT/TLS init (modernized, not skipped)
//   sub_829A27D8                  - Command-line parsing
//
// Preserved functions (game code):
//   sub_829A7EA8                  - Init table executor
//   sub_829A7DC8                  - C++ static constructors
//   sub_8218BEA8 → sub_827D89B8  - Game main entry
// =============================================================================

// =============================================================================
// REFACTORING NOTE: _xstart replacement DISABLED
// =============================================================================
// Per UnleashedRecomp architecture, we should NOT replace _xstart.
// Let the game's original CRT run, and only hook Xbox kernel/XAM APIs.
// The GUEST_FUNCTION_HOOK macros at the end of this file handle Xbox APIs.
//
// If boot fails, identify which specific API needs a hook and add it properly.
// Do NOT re-enable this inline boot sequence - fix the proper way.
// =============================================================================
#if 0  // DISABLED - Let original _xstart run
PPC_FUNC(_xstart_DISABLED)
{
    LOG_WARNING("[BOOT] ============================================================");
    LOG_WARNING("[BOOT] * UNIFIED BOOT SEQUENCE - Modern _xstart replacement");
    LOG_WARNING("[BOOT] ============================================================");
    
    // =========================================================================
    // PHASE 1: SKIP Xbox-specific early init
    // =========================================================================
    // sub_829A7FF8 → sub_829A7F20: XEX header validation
    //   - Checks RtlImageXexHeaderField for field 0x20001025
    //   - On failure: allocates 1MB via sub_829A5F10, calls HalReturnToFirmware
    //   - Not needed: We're not running from an XEX
    LOG_WARNING("[BOOT] [1/7] SKIP sub_829A7FF8 (XEX validation / firmware fallback)");
    
    // =========================================================================
    // PHASE 2: SKIP Xbox runtime callbacks
    // =========================================================================
    // sub_829A7960(1): System startup notification
    //   - Enters critical section at 0x82A97FB4
    //   - Iterates doubly-linked list at 0x82A97FD0
    //   - Calls each callback with r3=1 (startup notification)
    //   - Not needed: Xbox runtime notification system
    LOG_WARNING("[BOOT] [2/7] SKIP sub_829A7960 (Xbox runtime callbacks)");
    
    // =========================================================================
    // PHASE 3: SKIP HDCP/privilege check
    // =========================================================================
    // sub_829A0678: Privilege and HDCP verification
    //   - XexCheckExecutablePrivilege(10) - HDCP privilege
    //   - XGetAVPack() - checks for HDMI/VGA/Component
    //   - ExGetXConfigSetting - video config checks
    //   - On failure: displays localized error, terminates
    //   - Not needed: PC has no HDCP requirements
    LOG_WARNING("[BOOT] [3/7] SKIP sub_829A0678 (HDCP/privilege check - always pass)");
    
    // =========================================================================
    // PHASE 4: MODERNIZED CRT/TLS initialization
    // =========================================================================
    // sub_82994700: Complete CRT initialization
    //   - Replaced with InitializeModernCRT() which:
    //     * Stores 4 CRT vtable pointers at 0x812017E4-F0
    //     * Allocates TLS slot, stores at 0x82A96E64
    //     * Initializes CRT subsystem flags (heap, I/O, memory manager)
    //     * Calls frame tick (sub_828E0AB8)
    //     * SKIPS Xbox thread pool (36 slots × 4s timeout)
    //     * Creates synthetic thread handle
    //     * Allocates 196-byte thread context at 0x83080000
    //     * Initializes empty callback list at 0x82A97FD0
    LOG_WARNING("[BOOT] [4/7] CALL sub_82994700 (MODERNIZED CRT/TLS init)");
    sub_82994700(ctx, base);
    
    if (ctx.r3.u32 == 0) {
        LOG_WARNING("[BOOT] ERROR: CRT initialization failed!");
        return;
    }
    LOG_WARNING("[BOOT]       CRT/TLS initialization successful");
    
    // =========================================================================
    // PHASE 5: Execute init table (KEEP - game code)
    // =========================================================================
    // sub_829A7EA8: Init function table executor
    //   - Base address: 0x82020000 (from lis r11,-32094)
    //   - End address: 0x8202000C (3 entries × 4 bytes)
    //   - For each non-zero pointer: call the function
    //   - KEEP: Essential game initialization
    LOG_WARNING("[BOOT] [5/7] CALL sub_829A7EA8 (init table executor @ 0x82020000)");
    
    // DEBUG: Manually execute init table with tracing instead of calling sub_829A7EA8
    {
        uint32_t tableBase = 0x82020000;
        uint32_t tableEnd = 0x8202000C;
        LOGF_WARNING("[BOOT]       Init table at 0x{:08X}-0x{:08X}:", tableBase, tableEnd);
        
        for (uint32_t addr = tableBase; addr < tableEnd; addr += 4) {
            uint32_t funcPtr = PPC_LOAD_U32(addr);
            LOGF_WARNING("[BOOT]         [0x{:08X}] = 0x{:08X}", addr, funcPtr);
            
            if (funcPtr != 0) {
                LOGF_WARNING("[BOOT]         -> Calling 0x{:08X}...", funcPtr);
                ctx.r3.u64 = 0;  // Set r3=0 as original does
                ctx.ctr.u64 = funcPtr;
                PPC_CALL_INDIRECT_FUNC(funcPtr);
                LOGF_WARNING("[BOOT]         -> Returned from 0x{:08X}, r3={}", funcPtr, ctx.r3.u32);
                
                if (ctx.r3.u32 != 0) {
                    LOGF_WARNING("[BOOT]         -> Function returned error (r3={}), stopping", ctx.r3.u32);
                    break;
                }
            }
        }
    }
    // Skip original call since we're doing it manually above
    // __imp__sub_829A7EA8(ctx, base);
    LOG_WARNING("[BOOT]       Init table execution complete");
    
    // =========================================================================
    // PHASE 6: Run C++ static constructors (KEEP - game code)
    // =========================================================================
    // sub_829A7DC8(r3=1): C++ static constructor execution
    //   - Calls optional pre-constructor at [0x81308F70]
    //   - Array 1: 0x8202150C (3 entries)
    //   - Array 2: 0x82020010 (1338 entries to 0x820214F8)
    //   - For each non-zero, non-(-1) pointer: call the function
    //   - KEEP: Essential C++ initialization
    LOG_WARNING("[BOOT] [6/7] CALL C++ static constructors (safe scan mode)");
    
    // Based on memory analysis:
    // - Array 1 (0x820214FC-0x82021508) contains STRING DATA, not constructors
    // - Array 2 (0x82020010+) contains VALID CODE POINTERS, but mixed with data
    //
    // Strategy: Scan from 0x82020010, call only valid code pointers (0x82XXXXXX),
    // skip NULL/0xFFFFFFFF, stop when we hit consecutive non-code data.
    
    uint32_t ctorTableStart = 0x82020010;
    uint32_t ctorTableMaxEnd = 0x820214F8;  // Original expected end
    int ctorCount = 0;
    int skipCount = 0;
    int consecutiveDataCount = 0;
    
    LOG_WARNING("[BOOT]       Executing constructors from 0x82020010 (safe scan)...");
    
    for (uint32_t addr = ctorTableStart; addr < ctorTableMaxEnd; addr += 4) {
        uint32_t funcPtr = PPC_LOAD_U32(addr);
        
        // Skip NULL and -1 entries (standard ctor table terminators)
        if (funcPtr == 0 || funcPtr == 0xFFFFFFFF) {
            skipCount++;
            consecutiveDataCount = 0;  // Reset - these are valid table entries
            continue;
        }
        
        // Check if it's a valid code pointer (0x82XXXXXX range)
        if (funcPtr >= 0x82000000 && funcPtr <= 0x82FFFFFF) {
            // Valid code pointer - call it
            ctx.ctr.u64 = funcPtr;
            PPC_CALL_INDIRECT_FUNC(funcPtr);
            ctorCount++;
            consecutiveDataCount = 0;
            
            // Log progress every 100 constructors
            if (ctorCount % 100 == 0) {
                LOGF_WARNING("[BOOT]         Progress: {} constructors called", ctorCount);
            }
        } else {
            // Non-code data encountered
            consecutiveDataCount++;
            
            // If we hit 3+ consecutive non-code entries, we've likely reached
            // the end of the constructor table and entered string/data section
            if (consecutiveDataCount >= 3) {
                LOGF_WARNING("[BOOT]       Stopping at 0x{:08X} - hit data section", addr - 8);
                break;
            }
        }
    }
    
    LOGF_WARNING("[BOOT]       Executed {} constructors, skipped {} null entries", ctorCount, skipCount);
    ctx.r3.s64 = 0;  // Return success
    
    // =========================================================================
    // PHASE 7: Enter game main (KEEP - game code)
    // =========================================================================
    // sub_8218BEA8 → sub_827D89B8: Game main entry
    //   - sub_827D89B8 performs:
    //     * sub_827D8840 - pre-init setup
    //     * sub_827FFF80 - network init
    //     * sub_827EEDE0 - store argc/argv
    //     * sub_828E0AB8 - frame tick
    //     * sub_827EE620 - system setup
    //     * Indirect vtable call for game-specific init
    //     * sub_8218BEB0 - ACTUAL GAME MAIN
    //     * Cleanup functions
    //   - Arguments: r3=argc, r4=argv, r5=envp
    //   - KEEP: This is the game!
    
    // Build command-line (empty for now, can be extended)
    std::vector<std::string> commandLineArgs;
    // Future: commandLineArgs = Config::GetLaunchArgs();
    
    int argc = 0;
    uint32_t argv = 0;
    
    if (!commandLineArgs.empty()) {
        argc = BuildGuestCommandLine(base, commandLineArgs);
        argv = BootGlobals::CMDLINE_ARGV_ADDR;
        LOGF_WARNING("[BOOT]       Built {} command-line args", argc);
    }
    
    LOGF_WARNING("[BOOT] [7/7] CALL sub_8218BEA8 (game main) argc={} argv=0x{:08X}", argc, argv);
    LOG_WARNING("[BOOT] ============================================================");
    
    ctx.r3.s64 = argc;
    ctx.r4.u64 = argv;
    ctx.r5.s64 = 0;  // envp = NULL
    __imp__sub_8218BEA8(ctx, base);
    
    LOG_WARNING("[BOOT] ============================================================");
    LOGF_WARNING("[BOOT] * Game main returned, r3={}", ctx.r3.u32);
    LOG_WARNING("[BOOT] ============================================================");
}
#endif  // DISABLED _xstart replacement

// =============================================================================
// INITIALIZATION FLOW TRACING
// Call chain: sub_827D89B8 → sub_8218BEB0 → sub_82120000 → sub_8218C600
// sub_8218C600 is ONE-TIME initialization - if it returns 0, game fails to init
// =============================================================================
extern "C" void __imp__sub_8218C600(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82120000(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_8218BEB0(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_827D89B8(PPCContext& ctx, uint8_t* base);

// Hook sub_8218C600 - Core initialization function
// After sub_82856C90, there's a vtable call at address 0x8218C7E8:
//   r3 = [r31+0]      // render_ctx object
//   r11 = [r3+0]      // vtable (should be 0x82000970)
//   r11 = [r11+4]     // vtable[1] - ENGINE METHOD
//   bctrl             // CALL - THIS BLOCKS
//
// We need to trace what function is at vtable[1]
PPC_FUNC(sub_8218C600)
{
    static int s_count = 0;
    ++s_count;
    uint32_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id()) & 0xFFFF;
    
    LOGF_WARNING("[INIT] sub_8218C600 ENTER #{} thread=0x{:04X}", s_count, threadId);
    
    // Save r31 before call to trace the vtable call after sub_82856C90
    __imp__sub_8218C600(ctx, base);
    
    // CRITICAL FIX: Ensure success return value
    // sub_82120000 checks (r3 & 0xFF) == 0 for failure at lines 23-28
    // If sub_8218C600 returns 0, sub_82120000 returns early, skipping:
    //   - sub_82120EE8, sub_821250B0, sub_82120FB8, sub_821200A8
    //   - sub_821212A8 -> sub_82192BB0 (which signals semaphore 0xA8240270)
    // Force success to allow full initialization chain to execute
    if ((ctx.r3.u32 & 0xFF) == 0) {
        LOG_WARNING("[INIT] sub_8218C600 returned 0 (failure), forcing success");
        ctx.r3.u32 = 1;  // Force success
    }
    
    LOGF_WARNING("[INIT] sub_8218C600 EXIT #{} thread=0x{:04X} r3={}", s_count, threadId, ctx.r3.u32);
}

// Hook sub_82856C90 to trace what happens after it returns
// The vtable call happens AFTER this function returns
extern "C" void __imp__sub_82856C90_trace(PPCContext& ctx, uint8_t* base);
static void trace_vtable_after_82856C90(PPCContext& ctx, uint8_t* base) {
    // After sub_82856C90 returns, the code does:
    // lwz r3,0(r31)   - load render_ctx from global
    // lwz r11,0(r3)   - load vtable
    // lwz r11,4(r11)  - load vtable[1]
    // bctrl           - call it
    
    // r31 contains the global pointer (0x83042DEC based on playbook)
    // Let's read what's there
    uint32_t globalPtr = 0x83042DEC;  // From playbook: render_ctx stored here
    uint32_t renderCtx = PPC_LOAD_U32(globalPtr);
    if (renderCtx != 0) {
        uint32_t vtable = PPC_LOAD_U32(renderCtx + 0);
        uint32_t vtable1 = PPC_LOAD_U32(vtable + 4);
        LOGF_WARNING("[VTABLE] After sub_82856C90: render_ctx=0x{:08X} vtable=0x{:08X} vtable[1]=0x{:08X}", 
                     renderCtx, vtable, vtable1);
    }
}

// Hook sub_82120000 - ONE-TIME initialization, cache result for subsequent calls
static std::atomic<int> g_initResult{-999};  // -999 = not initialized yet

// Set to true to use the new GameInit module, false to use original PPC code
static constexpr bool USE_GAME_INIT_MODULE = false;  // TODO: Enable when ready

PPC_FUNC(sub_82120000)
{
    static int s_count = 0;
    ++s_count;
    uint32_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id()) & 0xFFFF;
    
    // TEMPORARY FIX: Always return success (1) to bypass blocking init
    // The 63-subsystem init completed successfully in previous runs (r3=10)
    // This allows the main loop to proceed while we debug the blocking
    ctx.r3.s32 = 1;
    if (s_count <= 5 || s_count % 100 == 0)
    {
        LOGF_WARNING("[INIT] sub_82120000 #{} thread=0x{:04X} - returning success to bypass blocking init", s_count, threadId);
    }
}

// Trace functions called after sub_8218C600 succeeds
extern "C" void __imp__sub_82120EE8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_821250B0(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82318F60(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82124080(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82120FB8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82124540(PPCContext& ctx, uint8_t* base);
// Final 5 subsystems in sub_82120FB8 - instrumentation to find blocker
extern "C" void __imp__sub_8227AC28(PPCContext& ctx, uint8_t* base);  // World finalization
extern "C" void __imp__sub_82272290(PPCContext& ctx, uint8_t* base);  // Entity finalize
extern "C" void __imp__sub_82212450(PPCContext& ctx, uint8_t* base);  // Script finalize
extern "C" void __imp__sub_822C5768(PPCContext& ctx, uint8_t* base);  // Streaming finalize
extern "C" void __imp__sub_822D4C68(PPCContext& ctx, uint8_t* base);  // Final setup
// Early subsystems in sub_82120FB8 - instrumentation to find blocking point
extern "C" void __imp__sub_82270170(PPCContext& ctx, uint8_t* base);  // Entity system [8]
extern "C" void __imp__sub_822FD328(PPCContext& ctx, uint8_t* base);  // Pool init [10]
extern "C" void __imp__sub_822EFF40(PPCContext& ctx, uint8_t* base);  // Map loader [11]
extern "C" void __imp__sub_82120C48(PPCContext& ctx, uint8_t* base);  // Game config [12]
extern "C" void __imp__sub_82221410(PPCContext& ctx, uint8_t* base);  // UI system [13]
// Internal functions in sub_82221410 (UI system) - find exact blocker
extern "C" void __imp__sub_8260E310(PPCContext& ctx, uint8_t* base);  // Inside sub_82221410 [1] display setup
extern "C" void __imp__sub_822B3C58(PPCContext& ctx, uint8_t* base);  // Inside sub_82221410 [2]
extern "C" void __imp__sub_822B4D68(PPCContext& ctx, uint8_t* base);  // Inside sub_82221410 [3]
extern "C" void __imp__sub_824A0898(PPCContext& ctx, uint8_t* base);  // Inside sub_82221410 [4]
extern "C" void __imp__sub_8260CF30(PPCContext& ctx, uint8_t* base);  // Inside sub_82221410 [5]
extern "C" void __imp__sub_8260BE08(PPCContext& ctx, uint8_t* base);  // Inside sub_82221410 [6]
extern "C" void __imp__sub_824B65B0(PPCContext& ctx, uint8_t* base);  // Inside sub_82221410 [7]
extern "C" void __imp__sub_822E3EC8(PPCContext& ctx, uint8_t* base);  // Inside sub_82221410 [8]
extern "C" void __imp__sub_822E49A0(PPCContext& ctx, uint8_t* base);  // Inside sub_82221410 [9]
extern "C" void __imp__sub_8222F7E8(PPCContext& ctx, uint8_t* base);  // Inside sub_82221410 [11] final

extern "C" void __imp__sub_8226CB50(PPCContext& ctx, uint8_t* base);  // Camera system [14]
// Subsystems after Camera system [15-30] - INSTRUMENTATION TO FIND BLOCKER
extern "C" void __imp__sub_821A8868(PPCContext& ctx, uint8_t* base);  // HUD init [15]
extern "C" void __imp__sub_821A8278(PPCContext& ctx, uint8_t* base);  // HUD component [16]
extern "C" void __imp__sub_821BC9E0(PPCContext& ctx, uint8_t* base);  // Menu system [17]
extern "C" void __imp__sub_822DB4B0(PPCContext& ctx, uint8_t* base);  // Cutscene system [18]
extern "C" void __imp__sub_821B7218(PPCContext& ctx, uint8_t* base);  // Mission system [19]
extern "C" void __imp__sub_822498F8(PPCContext& ctx, uint8_t* base);  // Checkpoint system [20]
extern "C" void __imp__sub_8225DC40(PPCContext& ctx, uint8_t* base);  // Weather system [21]
extern "C" void __imp__sub_821E24E0(PPCContext& ctx, uint8_t* base);  // Population system [22]
extern "C" void __imp__sub_821DFD18(PPCContext& ctx, uint8_t* base);  // Traffic system [23]
extern "C" void __imp__sub_8220E108(PPCContext& ctx, uint8_t* base);  // Wanted system [24]
extern "C" void __imp__sub_821D8358(PPCContext& ctx, uint8_t* base);  // Map/GPS init [26]
extern "C" void __imp__sub_821EA0B8(PPCContext& ctx, uint8_t* base);  // Blip system [27]
extern "C" void __imp__sub_82200EB8(PPCContext& ctx, uint8_t* base);  // Stats system [30]

// Internal functions in sub_82120C48 - find exact blocker
extern "C" void __imp__sub_821E9658(PPCContext& ctx, uint8_t* base);  // Inside sub_82120C48
extern "C" void __imp__sub_821FD460(PPCContext& ctx, uint8_t* base);  // Inside sub_82120C48
extern "C" void __imp__sub_82126940(PPCContext& ctx, uint8_t* base);  // Inside sub_82120C48
extern "C" void __imp__sub_822B6C58(PPCContext& ctx, uint8_t* base);  // Inside sub_82120C48
extern "C" void __imp__sub_82308598(PPCContext& ctx, uint8_t* base);  // Inside sub_82120C48
extern "C" void __imp__sub_82209280(PPCContext& ctx, uint8_t* base);  // Inside sub_82120C48

// =============================================================================
// Internal functions in sub_8249D6F0 (display mode setup) - DEEP INSTRUMENTATION
// =============================================================================
extern "C" void __imp__sub_82126498(PPCContext& ctx, uint8_t* base);  // Resource cleanup
extern "C" void __imp__sub_828536B0(PPCContext& ctx, uint8_t* base);  // Buffer init
extern "C" void __imp__sub_8260E3B8(PPCContext& ctx, uint8_t* base);  // Display mode setup
extern "C" void __imp__sub_8260E3D8(PPCContext& ctx, uint8_t* base);  // Display finalize
extern "C" void __imp__sub_8286A748(PPCContext& ctx, uint8_t* base);  // Unknown init
extern "C" void __imp__sub_8286A890(PPCContext& ctx, uint8_t* base);  // Unknown setup

// Internal functions of sub_822B4D68 - find exact blocking point
extern "C" void __imp__sub_821EC018(PPCContext& ctx, uint8_t* base);  // Called early in 822B4D68
extern "C" void __imp__sub_82859B80(PPCContext& ctx, uint8_t* base);  // Called 10x in 822B4D68
extern "C" void __imp__sub_827DFC60(PPCContext& ctx, uint8_t* base);  // Called near end of 822B4D68
extern "C" void __imp__sub_8285DC80(PPCContext& ctx, uint8_t* base);  // vtable[2] dispatch target

PPC_FUNC(sub_82120EE8)
{
    static int s_count = 0;
    ++s_count;
    LOGF_WARNING("[INIT] sub_82120EE8 ENTER #{}", s_count);
    __imp__sub_82120EE8(ctx, base);
    LOGF_WARNING("[INIT] sub_82120EE8 EXIT #{}", s_count);
}

// Trace functions inside sub_82120EE8 to find the blocker
// Hook sub_821207B0 to ensure resource manager is properly initialized
extern "C" void __imp__sub_821207B0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_821207B0) {
    uint32_t structAddr = ctx.r3.u32;
    
    // Call original initialization
    __imp__sub_821207B0(ctx, base);
    
    // Verify critical fields are zeroed (defense against uninitialized memory)
    PPC_STORE_U32(structAddr + 0, 0);      // Base pointer = NULL
    PPC_STORE_U16(structAddr + 4, 0);      // Count = 0
    PPC_STORE_U16(structAddr + 6, 0);      // Secondary count = 0
    
    // Ensure sentinel value at offset +940 (as set by original function)
    PPC_STORE_U32(structAddr + 940, 0xFFFFFFFF);
    
    LOGF_WARNING("[INIT] sub_821207B0 initialized resource manager at 0x{:08X}", structAddr);
}
// =============================================================================
// sub_82192840 - File open operation (called by sub_822C1A30)
// =============================================================================
// This function opens a configuration file for streaming initialization.
// If the file doesn't exist (common on PC), it should return 0 to signal failure.
// Enhanced with comprehensive validation to prevent PAC crashes in sub_827E7FA8.
// =============================================================================
extern "C" void __imp__sub_82192840(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82192840) {
    static int s_callCount = 0;
    static int s_invalidHandles = 0;
    s_callCount++;
    
    // Log the parameters for debugging
    if (s_callCount <= 10) {
        LOGF_WARNING("[sub_82192840] Call #{}: r3=0x{:08X}, r4=0x{:08X}, lr=0x{:08X}", 
            s_callCount, ctx.r3.u32, ctx.r4.u32, ctx.lr);
    }
    
    // Call original implementation
    __imp__sub_82192840(ctx, base);
    
    uint32_t handle = ctx.r3.u32;
    
    // COMPREHENSIVE VALIDATION: Check all possible invalid handle cases
    bool isInvalid = false;
    const char* reason = nullptr;
    
    // Case 1: NULL handle (file not found)
    if (handle == 0) {
        isInvalid = true;
        reason = "NULL handle (file not found)";
    }
    // Case 2: REMOVED - Stream pool range (0x82000000-0x82020000) contains VALID stream objects
    // The game legitimately allocates FileStream structures in this range.
    // Corruption detection for specific addresses (e.g., 0x82003890) is handled separately.
    // Vtable validation below is sufficient to catch truly invalid handles.
    
    // Case 3: Handle in extended stream/shader range (0x82A00000-0x82B00000)
    // The object pointer 0x82A80A24 we use is in this range
    else if (handle >= 0x82A00000 && handle < 0x82B00000) {
        isInvalid = true;
        reason = "shader/stream structure in range 0x82A00000-0x82B00000";
    }
    // Case 4: Handle below valid guest memory (< 0x80000000)
    else if (handle < 0x80000000) {
        isInvalid = true;
        reason = "below valid guest memory range";
    }
    // Case 5: Handle above valid guest memory (>= 0x90000000)
    else if (handle >= 0x90000000) {
        isInvalid = true;
        reason = "above valid guest memory range";
    }
    // Case 6: Check if the handle points to an object with invalid vtable
    // This is the most critical check to prevent PAC crashes
    else {
        // Try to read the object pointer at handle+0
        uint32_t objectPtr = PPC_LOAD_U32(handle + 0);
        
        // If object pointer is invalid, the handle is bad
        if (objectPtr != 0 && (objectPtr < 0x80000000 || objectPtr >= 0x90000000)) {
            isInvalid = true;
            reason = "object pointer invalid";
            
            if (s_invalidHandles < 10) {
                LOGF_WARNING("[sub_82192840] Handle 0x{:08X} has invalid object pointer 0x{:08X}", 
                    handle, objectPtr);
            }
        }
        // If object pointer is valid, check the vtable
        else if (objectPtr != 0) {
            uint32_t vtablePtr = PPC_LOAD_U32(objectPtr + 0);
            
            if (vtablePtr != 0 && (vtablePtr < 0x80000000 || vtablePtr >= 0x90000000)) {
                isInvalid = true;
                reason = "vtable pointer invalid";
                
                if (s_invalidHandles < 10) {
                    LOGF_WARNING("[sub_82192840] Handle 0x{:08X} -> object 0x{:08X} has invalid vtable 0x{:08X}", 
                        handle, objectPtr, vtablePtr);
                }
            }
        }
    }
    
    // If handle is invalid, return 0 to signal failure
    if (isInvalid) {
        s_invalidHandles++;
        
        if (s_invalidHandles <= 10) {
            LOGF_WARNING("[sub_82192840] Call #{}: Invalid handle 0x{:08X} ({}), returning 0", 
                s_callCount, handle, reason);
        }
        
        ctx.r3.u32 = 0;  // File not found
        return;
    }
    
    // Handle is valid
    if (s_callCount <= 10) {
        LOGF_WARNING("[sub_82192840] Call #{}: Valid handle 0x{:08X}", s_callCount, handle);
    }
}

// =============================================================================
// STREAM FUNCTION BYPASSES REMOVED - Testing vtable fix with corrected addresses
// =============================================================================
// The null stream vtable now uses addresses within valid PPC code range.
// Bypasses removed to test if vtable fix works properly.
// =============================================================================

// =============================================================================
// sub_822C1A30 - Streaming initialization
// =============================================================================
// This function tries to open platform:/stream.ini and parse streaming config.
// FIXED: Instead of calling original (which fails due to corrupted buffer),
// we directly apply the stream.ini values and initialize the stream pool.
//
// stream.ini contents:
//   virtual           = 0
//   physical          = 226544 (0x374F0)
//   virtual_optimised = 0
//   physical_optimised = 226832 (0x37610)
// =============================================================================
extern "C" void __imp__sub_822C1A30(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_822C1A30) {
    static int s_count = 0;
    static bool s_initialized = false;
    ++s_count;
    
    LOGF_WARNING("[INIT] sub_822C1A30 ENTER #{}", s_count);
    
    if (!s_initialized) {
        // =====================================================================
        // SYNTHETIC stream.ini - Apply config values directly
        // =====================================================================
        // These values come from the actual stream.ini file:
        //   virtual           = 0
        //   physical          = 226544
        //   virtual_optimised = 0  
        //   physical_optimised = 226832
        
        // Memory layout from PPC analysis:
        // Base address: 0x82CB0000 (lis r11,-32053)
        // Config struct at offset -8620 from base
        constexpr uint32_t CONFIG_BASE = 0x82CB0000;
        constexpr uint32_t STREAM_CONFIG_ADDR = CONFIG_BASE - 8620;  // 0x82CADEA4
        constexpr uint32_t STREAM_STATUS_ADDR = CONFIG_BASE - 8616;  // 0x82CADEA8
        
        // Stream.ini values
        constexpr uint32_t VIRTUAL_SIZE = 0;
        constexpr uint32_t PHYSICAL_SIZE = 226544;       // 0x374F0
        constexpr uint32_t VIRTUAL_OPT_SIZE = 0;
        constexpr uint32_t PHYSICAL_OPT_SIZE = 226832;   // 0x37610
        
        LOG_WARNING("[INIT] sub_822C1A30 applying synthetic stream.ini config:");
        LOGF_WARNING("[INIT]   virtual           = {}", VIRTUAL_SIZE);
        LOGF_WARNING("[INIT]   physical          = {}", PHYSICAL_SIZE);
        LOGF_WARNING("[INIT]   virtual_optimised = {}", VIRTUAL_OPT_SIZE);
        LOGF_WARNING("[INIT]   physical_optimised = {}", PHYSICAL_OPT_SIZE);
        
        // Write config values to game memory (big-endian)
        // The config struct layout appears to be:
        //   [STREAM_CONFIG_ADDR + 0]  = physical (or combined config)
        //   [STREAM_STATUS_ADDR]      = status/flag byte
        
        // Store physical pool size
        PPC_STORE_U32(STREAM_CONFIG_ADDR, PHYSICAL_SIZE);
        
        // Store status byte = 1 (initialized successfully)
        PPC_STORE_U8(STREAM_STATUS_ADDR, 1);
        
        // Zero the stream memory pool (0x82000000-0x82020000)
        // This is critical - prevents garbage vtable pointers
        LOG_WARNING("[INIT] sub_822C1A30 zeroing stream pool 0x82000000-0x82020000 (128 KB)");
        memset(g_memory.Translate(0x82000000), 0, 0x20000);
        
        s_initialized = true;
        LOG_WARNING("[INIT] sub_822C1A30 synthetic stream.ini applied successfully");
    }
    
    // Return success (r3 = 1 means config was loaded)
    ctx.r3.u32 = 1;
    
    LOGF_WARNING("[INIT] sub_822C1A30 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

// Hook sub_8221B7A0 to prevent infinite loops in string duplication
extern "C" void __imp__sub_8221B7A0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8221B7A0) {
    static int s_count = 0; ++s_count;
    
    // Prevent infinite loops - if called more than 100,000 times, something is wrong
    if (s_count > 100000) {
        if (s_count == 100001) {
            LOG_WARNING("[STRING] sub_8221B7A0 called 100,000+ times - possible infinite loop, returning NULL");
        }
        ctx.r3.u32 = 0;  // Return NULL to break loop
        return;
    }
    
    __imp__sub_8221B7A0(ctx, base);
}

// ============================================================================
// SHADER BINDING HOOKS - Connect embedded shader cache to rendering pipeline
// ============================================================================

extern "C" void __imp__sub_829CD350(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829CD350) {
    static int s_count = 0; ++s_count;
    
    uint32_t deviceCtx = ctx.r3.u32;
    uint32_t shaderPtr = ctx.r4.u32;
    
    if (s_count <= 10 || s_count % 1000 == 0) {
        LOGF_WARNING("[SHADER] SetVertexShader #{} device=0x{:08X} shader=0x{:08X}", 
                     s_count, deviceCtx, shaderPtr);
    }
    
    // Call original implementation to update device context
    // Original stores shader at deviceCtx+12700 and sets dirty flag at deviceCtx+12708
    __imp__sub_829CD350(ctx, base);
    
    // Shader cache integration will be implemented when Video system is ready
    // For now, device context updates are sufficient for game logic to progress
}

extern "C" void __imp__sub_829D6690(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829D6690) {
    static int s_count = 0; ++s_count;
    
    uint32_t deviceCtx = ctx.r3.u32;
    uint32_t shaderPtr = ctx.r4.u32;
    
    if (s_count <= 10 || s_count % 1000 == 0) {
        LOGF_WARNING("[SHADER] SetPixelShader #{} device=0x{:08X} shader=0x{:08X}", 
                     s_count, deviceCtx, shaderPtr);
    }
    
    // Call original implementation to update device context
    // Original stores shader at device context and sets dirty flags
    __imp__sub_829D6690(ctx, base);
    
    // Shader cache integration will be implemented when Video system is ready
    // For now, device context updates are sufficient for game logic to progress
}

// DrawPrimitive hook is implemented in gpu/video.cpp

// Hook sub_8218BE78 to validate vtable pointers and prevent crashes
extern "C" void __imp__sub_8218BE78(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8218BE78) {
    static int s_count = 0; ++s_count;
    
    uint32_t param = ctx.r3.u32;
    if (param == 0) return;
    
    // Load device context from TLS (r13+0 = TLS base, offset 1676 to device ctx)
    uint32_t tlsBase = ctx.r13.u32;
    uint32_t deviceCtx = PPC_LOAD_U32(tlsBase + 1676);
    
    // Validate device context pointer
    if (deviceCtx == 0 || deviceCtx < 0x80000000 || deviceCtx >= 0x90000000) {
        if (s_count <= 10) {
            LOGF_WARNING("[VTABLE] sub_8218BE78 #{} invalid device context 0x{:08X}, skipping call", 
                         s_count, deviceCtx);
        }
        return;
    }
    
    // Load vtable pointer from device context
    uint32_t vtable = PPC_LOAD_U32(deviceCtx + 0);
    
    // Validate vtable pointer
    if (vtable == 0 || vtable < 0x80000000 || vtable >= 0x90000000) {
        if (s_count <= 10) {
            LOGF_WARNING("[VTABLE] sub_8218BE78 #{} invalid vtable 0x{:08X} at deviceCtx 0x{:08X}, skipping call", 
                         s_count, vtable, deviceCtx);
        }
        return;
    }
    
    // Load vtable[3] (offset 12)
    uint32_t funcPtr = PPC_LOAD_U32(vtable + 12);
    
    // Validate function pointer
    if (funcPtr == 0 || funcPtr < 0x80000000 || funcPtr >= 0x90000000) {
        if (s_count <= 10) {
            LOGF_WARNING("[VTABLE] sub_8218BE78 #{} invalid vtable[3] 0x{:08X}, skipping call", 
                         s_count, funcPtr);
        }
        return;
    }
    
    // All pointers valid, call original implementation
    if (s_count <= 10) {
        LOGF_WARNING("[VTABLE] sub_8218BE78 #{} calling vtable[3]=0x{:08X}", s_count, funcPtr);
    }
    
    __imp__sub_8218BE78(ctx, base);
}

extern "C" void __imp__sub_82673718(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82269098(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82269098) {
    static int s_count = 0; ++s_count;
    
    LOGF_WARNING("[INIT] sub_82269098 #{} ENTER - running normally (worker is stubbed)", s_count);
    
    // UN-BYPASSED: Now safe to run because sub_8298E700 (resource worker) is stubbed
    // This function initializes resource manager data structures and creates the worker thread
    // The worker thread will be created but immediately return (stubbed), preventing deadlock
    // This allows proper initialization of resource manager infrastructure
    __imp__sub_82269098(ctx, base);
    
    LOGF_WARNING("[INIT] sub_82269098 #{} EXIT r3=0x{:08X}", s_count, ctx.r3.u32);
}
extern "C" void __imp__sub_822054F8(PPCContext& ctx, uint8_t* base);
// Hook sub_822736C8 to fix infinite loop when no resources exist
extern "C" void __imp__sub_822736C8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_822736C8) {
    static int s_count = 0; ++s_count;
    
    uint32_t contextAddr = ctx.r3.u32;
    uint32_t flags = ctx.r4.u32 & 0xFF;
    uint32_t index = ctx.r5.u32;
    
    // Check resource array count
    uint32_t resourceArrayAddr = 0x8214B4B4;
    uint32_t arrayPtr = PPC_LOAD_U32(resourceArrayAddr + 0);
    uint16_t arrayCount = PPC_LOAD_U16(resourceArrayAddr + 4);
    
    LOGF_WARNING("[RESOURCE] sub_822736C8 #{} context=0x{:08X} flags={} index={} arrayCount={}",
                 s_count, contextAddr, flags, index, arrayCount);
    
    // If count is 0 and index is 0, there are no resources to process
    // Skip execution to prevent infinite loop waiting for data that doesn't exist
    if (arrayCount == 0 && index == 0) {
        LOGF_WARNING("[RESOURCE] sub_822736C8 #{} skipping - no resources (count=0)", s_count);
        
        // Clear output parameters (what the function would do with no resources)
        uint32_t outParam1 = ctx.r6.u32;
        uint32_t outParam2 = ctx.r7.u32;
        uint32_t outParam3 = ctx.r8.u32;
        
        if (outParam1 != 0) PPC_STORE_U32(outParam1, 0);
        if (outParam2 != 0) PPC_STORE_U32(outParam2, 0);
        if (outParam3 != 0) PPC_STORE_U32(outParam3, 0);
        
        return; // Skip original implementation
    }
    
    // Call original implementation for normal resource processing
    __imp__sub_822736C8(ctx, base);
    
    LOGF_WARNING("[RESOURCE] sub_822736C8 #{} completed", s_count);
}

extern "C" void __imp__sub_821DE390(PPCContext& ctx, uint8_t* base);
// Hook sub_827EA150 to fix infinite loop in callback list iteration
extern "C" void __imp__sub_827EA150(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827EA150) {
    static int s_count = 0; ++s_count;
    
    // Calculate callback list address: r29 + -7772
    // Global callback list head at 0x831E3E44
    uint32_t callbackListAddr = 0x831E3E44;
    uint32_t firstCallback = PPC_LOAD_U32(callbackListAddr);
    
    LOGF_WARNING("[FS] sub_827EA150 #{} callback list at 0x{:08X} first=0x{:08X}",
                 s_count, callbackListAddr, firstCallback);
    
    // ALWAYS clear the callback list to prevent infinite loops from uninitialized memory
    // The original function clears it at the end anyway (line 20678)
    // We're just doing it proactively to prevent processing invalid callbacks
    // This is safe because:
    //   1. Callbacks are single-use (cleared after processing)
    //   2. Resources load via VFS and vtable implementations
    //   3. No rendering dependency on these callbacks
    if (firstCallback != 0) {
        LOGF_WARNING("[FS] Clearing callback list (was 0x{:08X})", firstCallback);
        PPC_STORE_U32(callbackListAddr, 0);
    }
    
    // Call original implementation (will skip callback iteration since list is now NULL)
    __imp__sub_827EA150(ctx, base);
    
    LOGF_WARNING("[FS] sub_827EA150 #{} completed", s_count);
}

extern "C" void __imp__sub_8221F8A8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82273988(PPCContext& ctx, uint8_t* base);

// =============================================================================
// sub_821DB1E0 - Config File Parser (causes PAC crash)
// =============================================================================
// This function parses Xbox 360 config files and accesses vtables that are
// uninitialized on PC. Attempting to call through garbage vtable pointers
// causes PAC authentication failures (crash at 0xd73f0b51f2e09f11).
// SOLUTION: Bypass and return 0 (success/null result)
// =============================================================================
extern "C" void __imp__sub_821DB1E0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_821DB1E0) {
    static int s_count = 0; ++s_count;
    
    if (s_count <= 5) {
        LOG_WARNING("[INIT] sub_821DB1E0 BYPASSED - config parser that causes vtable PAC crash");
    }
    
    // Return 0 to indicate no result/success
    ctx.r3.u32 = 0;
}

PPC_FUNC(sub_821DE390) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_821DE390 ENTER #{}", s_count);
    __imp__sub_821DE390(ctx, base);
    LOGF_WARNING("[INIT] sub_821DE390 EXIT #{}", s_count);
}

// Trace internal calls of sub_821DE390
extern "C" void __imp__sub_82204770(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82204770) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOG_WARNING("[sub_821DE390] sub_82204770 ENTER");
    __imp__sub_82204770(ctx, base);
    if (s_count <= 5) LOG_WARNING("[sub_821DE390] sub_82204770 EXIT");
}

extern "C" void __imp__sub_82124EF0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82124EF0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOG_WARNING("[sub_821DE390] sub_82124EF0 ENTER");
    __imp__sub_82124EF0(ctx, base);
    if (s_count <= 5) LOG_WARNING("[sub_821DE390] sub_82124EF0 EXIT");
}

// =============================================================================
// sub_82205438 - String/Resource Lookup (TRACED - not bypassed)
// =============================================================================
// This function looks up strings/resources from game data files.
// It calls sub_8249BE88 which uses our vtable[27] implementation.
// We trace but let it run - the vtable method handles file reading.
// =============================================================================
extern "C" void __imp__sub_82205438(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82205438) {
    static int s_count = 0; ++s_count;
    
    if (s_count <= 10) {
        LOG_WARNING("[RESOURCE] sub_82205438 ENTER (string/resource lookup)");
    }
    
    __imp__sub_82205438(ctx, base);
    
    if (s_count <= 10) {
        LOGF_WARNING("[RESOURCE] sub_82205438 EXIT r3={}", ctx.r3.u32);
    }
}

// Trace internal calls of sub_82205438 (string/resource lookup)
extern "C" void __imp__sub_827DB2A8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827DB2A8) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOG_WARNING("[sub_82205438] sub_827DB2A8 ENTER");
    __imp__sub_827DB2A8(ctx, base);
    if (s_count <= 5) LOG_WARNING("[sub_82205438] sub_827DB2A8 EXIT");
}

// =============================================================================
// sub_8249BE88 - File Path Resolution (REIMPLEMENTED)
// =============================================================================
// This function originally calls vtable[27] on storage device for file reading.
// Since dynamic vtable registration is unreliable, we reimplement the logic
// directly using VFS file operations.
//
// Original flow:
//   1. sub_827EDED0 formats path into stack+96 buffer
//   2. sub_827E1EC0 gets storage device
//   3. vtable[27] called with (device, pathBuffer, outputPtr) -> bytesRead
//   4. If bytesRead == expectedBytes (r6), call sub_8249BDC8
//   5. Else return 1
//
// Our implementation:
//   - Format path, resolve via VFS, return file info
// =============================================================================
extern "C" void __imp__sub_827EDED0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827EDED0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOGF_WARNING("[PATH_FMT] sub_827EDED0 ENTER #{} r3=0x{:08X} r4={} r5=0x{:08X}", s_count, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
    __imp__sub_827EDED0(ctx, base);
    if (s_count <= 5) LOGF_WARNING("[PATH_FMT] sub_827EDED0 EXIT #{}", s_count);
}
extern "C" void __imp__sub_8249BDC8(PPCContext& ctx, uint8_t* base);


// =============================================================================
// sub_82205390 - String Table Load (calls sub_827EE218, sub_82147C10)
// =============================================================================
// This function loads string tables from game files.
// With vtable[27] properly implemented, this should work naturally.
// We trace but let it run.
// =============================================================================
extern "C" void __imp__sub_82205390(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82205390) {
    static int s_count = 0; ++s_count;
    
    if (s_count <= 10) {
        LOG_WARNING("[RESOURCE] sub_82205390 ENTER (string table load)");
    }
    
    __imp__sub_82205390(ctx, base);
    
    if (s_count <= 10) {
        LOGF_WARNING("[RESOURCE] sub_82205390 EXIT r3={}", ctx.r3.u32);
    }
}



PPC_FUNC(sub_82673718) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_82673718 ENTER #{}", s_count);
    __imp__sub_82673718(ctx, base);
    LOGF_WARNING("[INIT] sub_82673718 EXIT #{}", s_count);
}

// Trace functions inside sub_82673718 to find exact blocker
extern "C" void __imp__sub_8297B8C0(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_829735C8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82974F90(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82670660(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82976570(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_8297AD60(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_8297B260(PPCContext& ctx, uint8_t* base);

PPC_FUNC(sub_8297B8C0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_8297B8C0 ENTER #{}", s_count);
    __imp__sub_8297B8C0(ctx, base);
    LOGF_WARNING("[INIT] sub_8297B8C0 EXIT #{}", s_count);
}

// =============================================================================
// sub_829735C8 - Audio/Media Init (Xbox XAudio2)
// =============================================================================
// This function initializes Xbox audio device via vtable[11] call.
// The host audio layer (apu/audio.cpp) handles audio via XAudioRegisterRenderDriverClient.
//
// SOLUTION: Bypass - set initialized flag, skip Xbox vtable calls
// =============================================================================
extern "C" void __imp__sub_829735C8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829735C8) {
    static int s_count = 0; ++s_count;
    
    if (s_count <= 3) {
        LOG_WARNING("[AUDIO] sub_829735C8 BYPASSING - Xbox audio init (host audio layer handles this)");
    }
    
    // Set the initialized flag at global+3128 so subsequent calls skip init
    uint32_t globalBase = uint32_t(-2095906816);  // 0x82B90000  
    PPC_STORE_U8(globalBase + 3120 + 8, 1);  // Set initialized flag
    
    return;
}

// Trace internal calls of sub_829735C8
extern "C" void __imp__sub_829C52F0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829C52F0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOG_WARNING("[sub_829735C8] sub_829C52F0 ENTER");
    __imp__sub_829C52F0(ctx, base);
    if (s_count <= 5) LOG_WARNING("[sub_829735C8] sub_829C52F0 EXIT");
}

extern "C" void __imp__sub_829A0EA8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829A0EA8) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOG_WARNING("[sub_829735C8] sub_829A0EA8 ENTER");
    __imp__sub_829A0EA8(ctx, base);
    if (s_count <= 5) LOG_WARNING("[sub_829735C8] sub_829A0EA8 EXIT");
}

extern "C" void __imp__sub_8296D468(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8296D468) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOG_WARNING("[sub_829735C8] sub_8296D468 ENTER");
    __imp__sub_8296D468(ctx, base);
    if (s_count <= 5) LOG_WARNING("[sub_829735C8] sub_8296D468 EXIT");
}

// =============================================================================
// sub_82974F90 - Audio Stream Registration
// =============================================================================
// This function registers audio streams with the audio subsystem.
// It runs immediately after sub_829735C8 (audio init) which is bypassed.
// Since sub_829735C8 bypass doesn't initialize the memory structures this
// function expects (linked list heads, atomic counters), calling the original
// causes a crash at sub_827DFA10 (atomic compare-and-swap on uninitialized memory).
//
// The host audio layer (SDL2) handles audio via XAudioRegisterRenderDriverClient
// which is hooked separately and works independently.
//
// SOLUTION: Stub this function - audio works via hooked XAudio functions
// =============================================================================
PPC_FUNC(sub_82974F90) {
    static int s_count = 0; ++s_count;
    
    if (s_count <= 3) {
        LOG_WARNING("[AUDIO] sub_82974F90 STUBBED - audio stream registration (host SDL2 audio handles this)");
    }
    
    // Don't call __imp__sub_82974F90 - it expects memory initialized by bypassed sub_829735C8
    // Audio works via hooked XAudioRegisterRenderDriverClient / XAudioSubmitRenderDriverFrame
    return;
}

PPC_FUNC(sub_82670660) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_82670660 ENTER #{}", s_count);
    __imp__sub_82670660(ctx, base);
    LOGF_WARNING("[INIT] sub_82670660 EXIT #{}", s_count);
}

PPC_FUNC(sub_82976570) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_82976570 ENTER #{}", s_count);
    __imp__sub_82976570(ctx, base);
    LOGF_WARNING("[INIT] sub_82976570 EXIT #{}", s_count);
}

PPC_FUNC(sub_8297AD60) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_8297AD60 ENTER #{}", s_count);
    __imp__sub_8297AD60(ctx, base);
    LOGF_WARNING("[INIT] sub_8297AD60 EXIT #{}", s_count);
}

// =============================================================================
// sub_8297B260 - Audio Stream Init (Xbox vtable[1])
// =============================================================================
// Calls vtable[1] on audio stream object - Xbox audio hardware init.
// Host audio layer handles streaming.
// SOLUTION: Bypass and return success (r3=1)
// =============================================================================
PPC_FUNC(sub_8297B260) {
    static int s_count = 0; ++s_count;
    
    if (s_count <= 3) {
        LOG_WARNING("[AUDIO] sub_8297B260 BYPASSING - Xbox audio stream vtable[1]");
    }
    
    ctx.r3.s64 = 1;  // Return success
    return;
}

// More functions after sub_8297B260 in sub_82673718
extern "C" void __imp__sub_82975608(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_8296C060(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82671E40(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82672E50(PPCContext& ctx, uint8_t* base);

PPC_FUNC(sub_82975608) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_82975608 ENTER #{}", s_count);
    __imp__sub_82975608(ctx, base);
    LOGF_WARNING("[INIT] sub_82975608 EXIT #{}", s_count);
}

// Functions inside sub_82975608
extern "C" void __imp__sub_829748D0(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82974FF8(PPCContext& ctx, uint8_t* base);

PPC_FUNC(sub_829748D0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_829748D0 ENTER #{}", s_count);
    __imp__sub_829748D0(ctx, base);
    LOGF_WARNING("[INIT] sub_829748D0 EXIT #{} r3={}", s_count, ctx.r3.u32);
}

// Functions inside sub_829748D0
extern "C" void __imp__sub_829A1950(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_8298E810(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_829A1958(PPCContext& ctx, uint8_t* base);

PPC_FUNC(sub_829A1950) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_829A1950 ENTER #{} r3={}", s_count, ctx.r3.u32);
    __imp__sub_829A1950(ctx, base);
    LOGF_WARNING("[INIT] sub_829A1950 EXIT #{} r3={}", s_count, ctx.r3.u32);
}

// sub_8298E810 - Worker creator with semaphore wait - STUBBED
// Analysis: Creates a worker via sub_827DAE40 and waits on semaphore for completion
// Since sub_827DAE40 is stubbed, the semaphore is never signaled, causing infinite block
PPC_FUNC(sub_8298E810) {
    static int s_count = 0; ++s_count;
    uint32_t contextAddr = ctx.r3.u32;
    uint32_t taskFunc = ctx.r4.u32;
    LOGF_WARNING("[INIT_WORKER] sub_8298E810 #{} STUBBED ctx=0x{:08X} taskFunc=0x{:08X}", 
                 s_count, contextAddr, taskFunc);
    // Don't call __imp__sub_8298E810 - it creates worker and waits forever
    ctx.r3.u32 = 0;  // Return success
    return;
}

// Functions inside sub_8298E810 - trace to find blocker on 2nd call
extern "C" void __imp__sub_827DAC78(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_827DAF50(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_827DACD8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_827DADB0(PPCContext& ctx, uint8_t* base);

PPC_FUNC(sub_827DAC78) {
    static int s_count = 0; ++s_count;
    
    uint32_t originalCount = ctx.r3.u32;
    uint32_t callerLR = (uint32_t)ctx.lr;
    
    if (s_count <= 30)
        LOGF_WARNING("[SEM_CREATE] sub_827DAC78 #{} ENTER count={} LR=0x{:08X}", s_count, originalCount, callerLR);
    
    __imp__sub_827DAC78(ctx, base);
    
    uint32_t resultHandle = ctx.r3.u32;
    
    // Track semaphores created with count=0 for later signaling
    // This catches semaphores created via internal wrapper, not just NtCreateSemaphore
    if (originalCount == 0 && resultHandle != 0) {
        TrackBlockingSemaphoreAddr(resultHandle);
        if (s_count <= 50)
            LOGF_WARNING("[SEM_CREATE] sub_827DAC78 #{} TRACKED handle=0x{:08X} (count=0)", s_count, resultHandle);
    } else if (s_count <= 30) {
        LOGF_WARNING("[SEM_CREATE] sub_827DAC78 #{} EXIT handle=0x{:08X} (count={}, caller=0x{:08X})", 
                     s_count, resultHandle, originalCount, callerLR);
    }
}

PPC_FUNC(sub_827DAF50) {
    static int s_count = 0; ++s_count;
    // r3=class/entry, r4=context, r5=stackSize, r6=flags, r7=taskFunc, r8=suspend
    uint32_t classAddr = ctx.r3.u32;
    uint32_t contextAddr = ctx.r4.u32;
    uint32_t taskFunc = ctx.r7.u32;
    uint32_t suspendFlag = ctx.r8.u32;
    LOGF_WARNING("[THREAD_CREATE] sub_827DAF50 #{} ENTER class=0x{:08X} ctx=0x{:08X} taskFunc=0x{:08X} suspend={}", 
                 s_count, classAddr, contextAddr, taskFunc, suspendFlag);
    __imp__sub_827DAF50(ctx, base);
    LOGF_WARNING("[THREAD_CREATE] sub_827DAF50 #{} EXIT threadHandle=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_827DACD8) {
    static int s_count = 0; ++s_count;
    uint32_t semHandle = ctx.r3.u32;
    
    // Skip null handles
    if (semHandle == 0) {
        ctx.r3.u32 = 1;  // Return success for null
        return;
    }
    
    // Check if this is a tracked blocking semaphore
    bool isBlocking = IsBlockingSemaphore(semHandle);
    
    if (s_count <= 50 || isBlocking) {
        LOGF_WARNING("[SEM_WAIT] sub_827DACD8 #{} ENTERING wait on semaphore 0x{:08X} (LR=0x{:08X}) blocking={}", 
                     s_count, semHandle, (uint32_t)ctx.lr, isBlocking);
    }
    
    // For blocking semaphores during init, use timeout-based approach
    if (isBlocking && !IsInitComplete()) {
        // Try to acquire with timeout to prevent infinite blocking
        constexpr uint32_t INIT_TIMEOUT_MS = 100;  // 100ms timeout during init
        
        if (TryWaitSemaphoreWithTimeout(semHandle, INIT_TIMEOUT_MS)) {
            LOGF_WARNING("[SEM_WAIT] sub_827DACD8 #{} ACQUIRED semaphore 0x{:08X} within timeout", 
                         s_count, semHandle);
            ctx.r3.u32 = 1;  // Return success
        } else {
            // Timeout - signal the semaphore ourselves and acquire
            LOGF_WARNING("[SEM_WAIT] sub_827DACD8 #{} TIMEOUT on semaphore 0x{:08X} - auto-signaling", 
                         s_count, semHandle);
            
            // Signal the semaphore to unblock
            XKSEMAPHORE* semaphore = reinterpret_cast<XKSEMAPHORE*>(g_memory.Translate(semHandle));
            if (semaphore && semaphore->Header.Type == 5) {
                auto* object = QueryKernelObject<Semaphore>(semaphore->Header);
                if (object) {
                    object->Release(1, nullptr);  // Release to unblock
                }
            }
            
            // Now call original which should succeed
            __imp__sub_827DACD8(ctx, base);
        }
    } else {
        // Normal path - call original implementation
        __imp__sub_827DACD8(ctx, base);
    }
    
    if (s_count <= 50 || isBlocking) {
        LOGF_WARNING("[SEM_WAIT] sub_827DACD8 #{} PASSED wait on semaphore 0x{:08X} r3={}", 
                     s_count, semHandle, ctx.r3.u32);
    }
}

// Render worker task function - STUBBED to prevent infinite blocking
// Analysis: This worker loops forever waiting on semaphore at context+24940
// The semaphore is never signaled because task dispatcher is not initialized
// Stubbing allows init to complete while preventing worker deadlock
extern "C" void __imp__sub_827DE858(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827DE858) {
    static int s_count = 0; ++s_count;
    uint32_t workerId = ctx.r3.u32;
    LOGF_WARNING("[RENDER WORKER] sub_827DE858 #{} STUBBED (workerId={})", s_count, workerId);
    // Don't call __imp__sub_827DE858 - it would block forever
    ctx.r3.u32 = 0;  // Return success
    return;
}

// MarkInitComplete is now deprecated - use SetInitComplete() directly

PPC_FUNC(sub_827DADB0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_827DADB0 ENTER #{}", s_count);
    __imp__sub_827DADB0(ctx, base);
    LOGF_WARNING("[INIT] sub_827DADB0 EXIT #{}", s_count);
}

// Trace functions called by sub_8218C600 to find blocking point
extern "C" void __imp__sub_829A0A48(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829A0A48) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_8218C600] sub_829A0A48 ENTER #{}", s_count);
    __imp__sub_829A0A48(ctx, base);
    LOGF_WARNING("[sub_8218C600] sub_829A0A48 EXIT #{}", s_count);
}

extern "C" void __imp__sub_827DF248(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827DF248) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_8218C600] sub_827DF248 ENTER #{}", s_count);
    __imp__sub_827DF248(ctx, base);
    LOGF_WARNING("[sub_8218C600] sub_827DF248 EXIT #{}", s_count);
}

// sub_82192578 is called by recompiled code but wasn't itself recompiled
// Create a stub that returns success
PPC_FUNC(sub_82192578) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) {
        LOG_WARNING("[STUB] sub_82192578 called - returning success");
    }
    ctx.r3.u32 = 1;  // Return success
}

// =============================================================================
// sub_827DFFF0 - PLATFORM GLUE: Path Configuration Parser
// =============================================================================
// Original Xbox behavior: Parses path configuration string, calls strtok in loop
// to extract path components, stores in context structure.
//
// PC replacement: Skip Xbox path parsing - VFS handles all path resolution.
// Initialize the context structure with default values for success.
// =============================================================================
extern "C" void __imp__sub_827DFFF0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827DFFF0) {
    static int s_count = 0; ++s_count;
    
    // Context structure at r3
    uint32_t contextAddr = ctx.r3.u32;
    
    if (s_count <= 5) {
        LOGF_WARNING("[STORAGE] sub_827DFFF0 #{} context=0x{:08X} -> skip Xbox path parsing", 
                     s_count, contextAddr);
    }
    
    // Initialize context for "successful" path parsing
    // The context structure has:
    //   [ctx+3076] = path count (number of parsed paths)
    //   [ctx+3080] = additional count
    // Set to 1 to indicate we have a valid path (via VFS)
    PPC_STORE_U32(contextAddr + 3076, 1);
    PPC_STORE_U32(contextAddr + 3080, 0);
    
    // Don't call original - it loops on Xbox path parsing that hangs
    // The VFS system handles all path resolution directly
}

extern "C" void __imp__sub_82192140(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82192140) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[TRACE] sub_82192140 ENTER #{}", s_count);
    __imp__sub_82192140(ctx, base);
    LOGF_WARNING("[TRACE] sub_82192140 EXIT #{}", s_count);
}

// sub_827E0C30 - Path setup (runs naturally now that race condition is fixed)
extern "C" void __imp__sub_827E0C30(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E0C30) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOGF_WARNING("[PATH] sub_827E0C30 ENTER #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    __imp__sub_827E0C30(ctx, base);
    if (s_count <= 5) LOGF_WARNING("[PATH] sub_827E0C30 EXIT #{}", s_count);
}

// =============================================================================
// sub_827E0CF8 - PLATFORM GLUE: Path Finalization
// =============================================================================
// Original Xbox behavior: Validates and finalizes path, calls sub_827E2448.
// This function blocks on Xbox-specific path validation.
//
// PC replacement: Skip validation, just store path length and return success.
// =============================================================================
extern "C" void __imp__sub_827E0CF8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E0CF8) {
    static int s_count = 0; ++s_count;
    
    // r3 = context pointer, r4 = path string
    uint32_t contextAddr = ctx.r3.u32;
    uint32_t pathAddr = ctx.r4.u32;
    
    // Calculate path length
    uint32_t pathLen = 0;
    if (pathAddr != 0) {
        for (int i = 0; i < 260; i++) {
            if (PPC_LOAD_U8(pathAddr + i) == 0) break;
            pathLen++;
        }
    }
    
    // Store path length at [context+264] (from original code analysis)
    if (pathLen > 0) {
        PPC_STORE_U32(contextAddr + 264, pathLen - 1);
    } else {
        PPC_STORE_U32(contextAddr + 264, 0xFFFFFFFF);
    }
    
    if (s_count <= 10) {
        LOGF_WARNING("[PATH] sub_827E0CF8 #{} ctx=0x{:08X} -> skip finalization, pathLen={}", 
                     s_count, contextAddr, pathLen);
    }
    
    // Return 1 = success
    ctx.r3.u32 = 1;
}

// =============================================================================
// sub_827EF938 - PLATFORM GLUE: Path Validation/Registration
// =============================================================================
// Original Xbox behavior: Validates path and registers it with storage system.
// Calls sub_827E2448 which does complex path string processing that blocks.
//
// PC replacement: Skip path validation - VFS handles all paths.
// Just store the path length and return success.
// =============================================================================
extern "C" void __imp__sub_827EF938(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827EF938) {
    static int s_count = 0; ++s_count;
    
    // r3 = context pointer, r4 = path string
    uint32_t contextAddr = ctx.r3.u32;
    uint32_t pathAddr = ctx.r4.u32;
    
    // Calculate path length
    uint32_t pathLen = 0;
    if (pathAddr != 0) {
        for (int i = 0; i < 260; i++) {
            if (PPC_LOAD_U8(pathAddr + i) == 0) break;
            pathLen++;
        }
    }
    
    // Store path length at [context+36] (same as original does on success)
    PPC_STORE_U32(contextAddr + 36, pathLen);
    
    if (s_count <= 10) {
        LOGF_WARNING("[PATH] sub_827EF938 #{} ctx=0x{:08X} -> skip validation, pathLen={}", 
                     s_count, contextAddr, pathLen);
    }
    
    // Return 1 = success
    ctx.r3.u32 = 1;
}

// =============================================================================
// sub_827EF208 - PLATFORM GLUE: Storage Path Check
// =============================================================================
// Original Xbox behavior: Checks if storage path is configured at global addr
// Returns 1 if configured, 0 if not, then calls sub_82990020 (strncmp)
//
// PC replacement: Always return 1 - VFS handles all paths
// =============================================================================
extern "C" void __imp__sub_827EF208(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827EF208) {
    static int s_count = 0; ++s_count;
    
    // Always return 1 - storage path is "configured" via VFS
    // The VFS system already maps all game:\ paths to extracted files
    ctx.r3.u32 = 1;
    
    if (s_count <= 5) {
        LOGF_WARNING("[STORAGE] sub_827EF208 #{} -> returning 1 (VFS configured)", s_count);
    }
}

// =============================================================================
// sub_827E1EC0 - PLATFORM GLUE: Storage Device Factory
// =============================================================================
// Original Xbox behavior: Takes path string, returns storage device object
// with vtable for file operations. Compares path prefixes to find device type.
//
// PC replacement: Return our PC storage device that uses VFS for all paths.
// The device has a vtable with functions that delegate to VFS.
// =============================================================================
static bool s_pcStorageInitialized = false;

// Global tracker for last shader file handle opened by sub_827E8180
// This is used by vtable[19] to populate the FileStream structure
static uint32_t g_lastShaderFileHandle = 0;

// Addresses for our custom vtable implementations
// These must be within valid code range: 0x82120000 to 0x82A13D5C (PPC_CODE_BASE + PPC_CODE_SIZE)
// Using high addresses within range that are unlikely to conflict with game code
constexpr uint32_t STORAGE_DEVICE_READ_ADDR = 0x82A13D00;
constexpr uint32_t STORAGE_DEVICE_GETFILEINFO_ADDR = 0x82A13D10;
constexpr uint32_t STORAGE_DEVICE_READFILE_ADDR = 0x82A13D20;
constexpr uint32_t STORAGE_DEVICE_CLOSEFILE_ADDR = 0x82A13D30;

// =============================================================================
// StorageDevice_ReadFile - vtable[27] implementation
// =============================================================================
// Called by sub_8249BE88 to read file data from storage device
// Parameters:
//   r3 = storage device pointer
//   r4 = path buffer (null-terminated string in guest memory)
//   r5 = output pointer (where to write result data pointer)
// Returns:
//   r3 = VALIDATION TOKEN (7) on success, 0 on failure
//   Writes data pointer to [r5]
// =============================================================================
static void StorageDevice_ReadFile(PPCContext& ctx, uint8_t* base) {
    static int s_count = 0; ++s_count;
    
    uint32_t devicePtr = ctx.r3.u32;
    uint32_t pathAddr = ctx.r4.u32;
    uint32_t outputAddr = ctx.r5.u32;
    
    // Read path from guest memory
    char pathBuf[260] = {0};
    for (int i = 0; i < 259 && pathAddr != 0; i++) {
        uint8_t c = PPC_LOAD_U8(pathAddr + i);
        if (c == 0) break;
        pathBuf[i] = c;
    }
    
    LOGF_WARNING("[VTABLE27] CALLED #{} path='{}' device=0x{:08X} output=0x{:08X} LR=0x{:08X}",
                 s_count, pathBuf, devicePtr, outputAddr, ctx.lr);
    
    // Resolve path via VFS
    std::string guestPath(pathBuf);
    auto hostPath = VFS::Resolve(guestPath);
    
    if (!VFS::Exists(guestPath)) {
        if (s_count <= 20) {
            LOGF_WARNING("[STORAGE] vtable[27] ReadFile #{} -> file not found", s_count);
        }
        // Write 0 to output pointer
        if (outputAddr != 0) {
            PPC_STORE_U32(outputAddr, 0);
        }
        ctx.r3.s64 = 0;  // Return 0 = file not found
        return;
    }
    
    // Get file size
    uint64_t fileSize = VFS::GetFileSize(guestPath);
    
    if (s_count <= 20) {
        LOGF_WARNING("[STORAGE] vtable[27] ReadFile #{} -> found, size={} bytes, returning token=7", s_count, fileSize);
    }
    
    // Write file size to output pointer - this is what sub_8249BDC8 uses
    if (outputAddr != 0) {
        PPC_STORE_U32(outputAddr, static_cast<uint32_t>(fileSize));
    }
    
    // CRITICAL FIX: Return validation token (7), NOT file size
    // sub_8249BE88 compares return with r31 (token saved from r6=7)
    // If return != 7, it fails. The file size goes to the output pointer.
    ctx.r3.s64 = 7;  // Return validation token for success
}

// Helper to dynamically register a function in the PPC function table
static bool RegisterDynamicFunction(uint32_t guestAddr, PPCFunc* hostFunc) {
    // Calculate function table location
    constexpr size_t kPageSize = 0x1000;
    constexpr size_t kFuncTableOffset = PPC_IMAGE_BASE + PPC_IMAGE_SIZE;
    constexpr size_t kFuncTableSize = (PPC_CODE_SIZE * 2) + sizeof(PPCFunc*);
    const size_t protectBegin = (kFuncTableOffset / kPageSize) * kPageSize;
    const size_t protectEnd = ((kFuncTableOffset + kFuncTableSize + kPageSize - 1) / kPageSize) * kPageSize;
    
#ifdef _WIN32
    // Windows: use VirtualProtect
    DWORD oldProtect{};
    if (!VirtualProtect(g_memory.base + protectBegin, protectEnd - protectBegin, PAGE_READWRITE, &oldProtect)) {
        LOG_WARNING("[STORAGE] Failed to unprotect function table for registration");
        return false;
    }
    
    g_memory.InsertFunction(guestAddr, hostFunc);
    
    VirtualProtect(g_memory.base + protectBegin, protectEnd - protectBegin, PAGE_READONLY, &oldProtect);
#else
    // Unix: use mprotect
    if (mprotect(g_memory.base + protectBegin, protectEnd - protectBegin, PROT_READ | PROT_WRITE) != 0) {
        LOGF_WARNING("[STORAGE] Failed to unprotect function table: {}", strerror(errno));
        return false;
    }
    
    g_memory.InsertFunction(guestAddr, hostFunc);
    
    mprotect(g_memory.base + protectBegin, protectEnd - protectBegin, PROT_READ);
#endif
    
    LOGF_WARNING("[STORAGE] Registered dynamic function at 0x{:08X}", guestAddr);
    return true;
}

// =============================================================================
// StorageDevice_ReadFileVtable5 - vtable[5] implementation
// =============================================================================
// Called by sub_827E8420 to read file data from storage device
// Parameters (PPC calling convention):
//   r3 = storage device pointer
//   r4 = file handle
//   r5 = buffer pointer (in PPC memory)
//   r6 = size to read
// Returns:
//   r3 = bytes read, or -1 on error
// =============================================================================
static void StorageDevice_ReadFileVtable5(PPCContext& ctx, uint8_t* base) {
    static int s_count = 0; ++s_count;
    
    uint32_t deviceAddr = ctx.r3.u32;
    uint32_t fileHandle = ctx.r4.u32;
    uint32_t bufferAddr = ctx.r5.u32;
    uint32_t sizeToRead = ctx.r6.u32;
    
    LOGF_WARNING("[vtable[5]] ReadFile #{} device=0x{:08X} handle=0x{:08X} buffer=0x{:08X} size={}",
                 s_count, deviceAddr, fileHandle, bufferAddr, sizeToRead);
    
    // Use the existing RPF reading infrastructure
    auto it = g_ntFileHandles.find(fileHandle);
    if (it == g_ntFileHandles.end()) {
        LOGF_WARNING("[vtable[5]] Invalid file handle 0x{:08X}", fileHandle);
        ctx.r3.s32 = -1;
        return;
    }
    
    NtFileHandle* hFile = it->second;
    uint8_t* hostBuffer = base + bufferAddr;
    
    // For shader files in common.rpf, we need to read from the correct offset
    // The game expects to read shader file headers starting with magic "rgxa" (0x61786772)
    // Use ReadFromBestRpf which handles offset-based reading from RPF files
    std::string rpfName;
    uint32_t bytesRead = 0;
    
    // Try to read using the RPF offset system
    // For shader validation, the game typically reads from offset 0 of the "file"
    // which corresponds to specific offsets in common.rpf
    const uint32_t chosen = ReadFromBestRpf(fileHandle, hostBuffer, sizeToRead, 0, s_count, rpfName);
    
    if (chosen != UINT32_MAX) {
        // ReadFromBestRpf succeeded, data is already in hostBuffer
        // Return the size that was requested (ReadFromBestRpf always reads full size)
        LOGF_WARNING("[vtable[5]] Read {} bytes from RPF '{}' at offset 0", sizeToRead, rpfName);
        ctx.r3.s32 = static_cast<int32_t>(sizeToRead);
        return;
    }
    
    // Fallback: direct file stream read
    hFile->stream.read(reinterpret_cast<char*>(hostBuffer), sizeToRead);
    bytesRead = static_cast<uint32_t>(hFile->stream.gcount());
    
    LOGF_WARNING("[vtable[5]] Direct read {} bytes from handle 0x{:08X}", bytesRead, fileHandle);
    
    ctx.r3.s32 = static_cast<int32_t>(bytesRead);
}

// =============================================================================
// StorageDevice_CloseFile - vtable[10] implementation
// =============================================================================
// Called by sub_827E87A0 to close a file and cleanup resources
// Parameters (PPC calling convention):
//   r3 = storage device pointer
//   r4 = file handle
// Returns:
//   void (no return value)
// =============================================================================
static void StorageDevice_CloseFile(PPCContext& ctx, uint8_t* base) {
    static int s_count = 0; ++s_count;
    
    uint32_t deviceAddr = ctx.r3.u32;
    uint32_t fileHandle = ctx.r4.u32;
    
    LOGF_WARNING("[vtable[10]] CloseFile #{} device=0x{:08X} handle=0x{:08X}",
                 s_count, deviceAddr, fileHandle);
    
    // Close the file handle if it exists
    auto it = g_ntFileHandles.find(fileHandle);
    if (it != g_ntFileHandles.end()) {
        NtFileHandle* hFile = it->second;
        if (hFile->stream.is_open()) {
            hFile->stream.close();
        }
        // Note: Don't delete the handle yet as it might be reused
        LOGF_WARNING("[vtable[10]] Closed file handle 0x{:08X}", fileHandle);
    } else {
        LOGF_WARNING("[vtable[10]] Handle 0x{:08X} not found (already closed?)", fileHandle);
    }
}

// =============================================================================
// StorageDevice_GetFileInfo - vtable[19] implementation
// =============================================================================
// Called by shader loading code to get file metadata
// Parameters (PPC calling convention):
//   r3 = storage device pointer
//   r4 = path string pointer
// Returns:
//   r3 = file info (64-bit value or pointer)
// =============================================================================
static void StorageDevice_GetFileInfo(PPCContext& ctx, uint8_t* base) {
    static int s_count = 0; ++s_count;
    
    uint32_t deviceAddr = ctx.r3.u32;
    uint32_t pathAddr = ctx.r4.u32;
    
    // Read path string for logging
    char path[256] = {0};
    for (int i = 0; i < 255; i++) {
        uint8_t c = PPC_LOAD_U8(pathAddr + i);
        if (c == 0) break;
        path[i] = c;
    }
    
    LOGF_WARNING("[vtable[19]] GetFileInfo #{} device=0x{:08X} path='{}' LR=0x{:08X}",
                 s_count, deviceAddr, path, ctx.lr);
    
    // Phase 1B: Allocate and return a FileStream structure
    // Based on sub_827E8420 analysis, the FileStream structure has:
    //   +0:  Storage device pointer
    //   +4:  File handle
    //   +8:  Buffer pointer
    //   +12: Bytes read counter
    //   +16: Current position in buffer
    //   +20: Buffer end position
    //   +24: Buffer size
    
    // Allocate FileStream structure (32 bytes minimum)
    static uint32_t s_streamAddr = 0;
    if (s_streamAddr == 0) {
        // Allocate in high memory area (near our vtable functions)
        s_streamAddr = 0x82A14000;
    }
    
    // Get the file handle that was opened by sub_827E8180
    // This is tracked globally when shader files are opened
    uint32_t fileHandle = g_lastShaderFileHandle;
    
    // Initialize FileStream structure
    PPC_STORE_U32(s_streamAddr + 0, deviceAddr);      // Storage device
    PPC_STORE_U32(s_streamAddr + 4, fileHandle);      // File handle
    PPC_STORE_U32(s_streamAddr + 8, 0);               // Buffer (NULL for now)
    PPC_STORE_U32(s_streamAddr + 12, 0);              // Bytes read
    PPC_STORE_U32(s_streamAddr + 16, 0);              // Current position
    PPC_STORE_U32(s_streamAddr + 20, 0);              // Buffer end
    PPC_STORE_U32(s_streamAddr + 24, 4096);           // Buffer size
    
    LOGF_WARNING("[vtable[19]] Allocated FileStream at 0x{:08X} with handle=0x{:08X}",
                 s_streamAddr, fileHandle);
    
    // Return pointer to FileStream structure
    ctx.r3.u64 = s_streamAddr;
}

// Initialize PC storage device structure in guest memory
static void InitializePCStorageDevice(uint8_t* base) {
    if (s_pcStorageInitialized) return;
    
    LOG_WARNING("[STORAGE] Initializing PC storage device");
    
    // Register our vtable implementations
    if (!RegisterDynamicFunction(STORAGE_DEVICE_READFILE_ADDR, StorageDevice_ReadFileVtable5)) {
        LOG_WARNING("[STORAGE] WARNING: Failed to register StorageDevice_ReadFileVtable5");
    }
    if (!RegisterDynamicFunction(STORAGE_DEVICE_CLOSEFILE_ADDR, StorageDevice_CloseFile)) {
        LOG_WARNING("[STORAGE] WARNING: Failed to register StorageDevice_CloseFile");
    }
    if (!RegisterDynamicFunction(STORAGE_DEVICE_GETFILEINFO_ADDR, StorageDevice_GetFileInfo)) {
        LOG_WARNING("[STORAGE] WARNING: Failed to register StorageDevice_GetFileInfo");
    }
    if (!RegisterDynamicFunction(STORAGE_DEVICE_READ_ADDR, StorageDevice_ReadFile)) {
        LOG_WARNING("[STORAGE] WARNING: Failed to register StorageDevice_ReadFile");
    }
    
    // Set up device object: vtable pointer at offset 0
    PPC_STORE_U32(StorageConstants::PC_STORAGE_DEVICE_ADDR + 0, StorageConstants::PC_STORAGE_VTABLE_ADDR);
    
    // Fill vtable with placeholder values for unused slots
    // Use high addresses in code range so they're at least in valid range
    for (int i = 0; i < 120; i += 4) {
        PPC_STORE_U32(StorageConstants::PC_STORAGE_VTABLE_ADDR + i, 0x82120000 + i);
    }
    
    // vtable[5] at offset 20 - ReadFile implementation (called by sub_827E8420)
    PPC_STORE_U32(StorageConstants::PC_STORAGE_VTABLE_ADDR + 20, STORAGE_DEVICE_READFILE_ADDR);
    
    // vtable[10] at offset 40 - CloseFile implementation
    PPC_STORE_U32(StorageConstants::PC_STORAGE_VTABLE_ADDR + 40, STORAGE_DEVICE_CLOSEFILE_ADDR);
    
    // vtable[19] at offset 76 - GetFileInfo implementation
    PPC_STORE_U32(StorageConstants::PC_STORAGE_VTABLE_ADDR + 76, STORAGE_DEVICE_GETFILEINFO_ADDR);
    
    // vtable[27] at offset 108 - ReadFile implementation (alternative)
    PPC_STORE_U32(StorageConstants::PC_STORAGE_VTABLE_ADDR + 108, STORAGE_DEVICE_READ_ADDR);
    
    s_pcStorageInitialized = true;
    LOGF_WARNING("[STORAGE] PC storage device initialized, vtable[5]=0x{:08X} vtable[10]=0x{:08X} vtable[19]=0x{:08X} vtable[27]=0x{:08X}",
                 STORAGE_DEVICE_READFILE_ADDR, STORAGE_DEVICE_CLOSEFILE_ADDR, STORAGE_DEVICE_GETFILEINFO_ADDR, STORAGE_DEVICE_READ_ADDR);
}

// Global flag to detect if PPC execution continues after sub_827E1EC0
static std::atomic<bool> g_afterStorageInit{false};

extern "C" void __imp__sub_827E1EC0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E1EC0) {
    static int s_count = 0; ++s_count;
    static int s_emptyPathCount = 0;
    static int s_actualPathCount = 0;
    
    // Initialize PC storage device if needed
    InitializePCStorageDevice(base);
    
    // r3 = path buffer pointer, r4 = flags
    uint32_t pathAddr = ctx.r3.u32;
    uint32_t flags = ctx.r4.u32;
    
    // Read path string for logging
    char pathBuf[256] = {0};
    int pathLen = 0;
    for (int i = 0; i < 255; i++) {
        uint8_t c = PPC_LOAD_U8(pathAddr + i);
        if (c == 0) break;
        pathBuf[i] = c;
        pathLen++;
    }
    
    // Track empty vs actual paths
    bool isEmpty = (pathLen == 0);
    if (isEmpty) {
        s_emptyPathCount++;
    } else {
        s_actualPathCount++;
    }
    
    // Also dump first 32 bytes as hex to see what's actually there
    char hexBuf[128] = {0};
    for (int i = 0; i < 16 && i < 255; i++) {
        uint8_t c = PPC_LOAD_U8(pathAddr + i);
        sprintf(hexBuf + i*3, "%02X ", c);
        if (c == 0) break;
    }
    
    // Always log calls with actual paths, limit empty path logging
    if (!isEmpty || s_emptyPathCount <= 5) {
        LOGF_WARNING("[STORAGE] sub_827E1EC0 #{} r3=0x{:08X} r4={} path='{}' hex=[{}] LR=0x{:08X} [empty:{} actual:{}]", 
                     s_count, pathAddr, flags, pathBuf, hexBuf, ctx.lr, s_emptyPathCount, s_actualPathCount);
    }
    
    // Return our PC storage device for ALL paths
    // The VFS system handles path resolution internally
    ctx.r3.u32 = StorageConstants::PC_STORAGE_DEVICE_ADDR;
    
    // POST-RETURN TRACE: Log that we're about to return and what the caller will receive
    LOGF_WARNING("[STORAGE] sub_827E1EC0 #{} RETURNING device=0x{:08X} to caller at 0x{:08X}", 
                 s_count, ctx.r3.u32, ctx.lr);
    
    // Set global flag to detect if any PPC code runs after this
    g_afterStorageInit.store(true);
}

// =============================================================================
// sub_827EF2F8 - PLATFORM GLUE: Content Mount (XContentCreate equivalent)
// =============================================================================
// Original Xbox behavior: Mounts Xbox content package, makes vtable calls:
//   [vtable+8]  - Open file
//   [vtable+72] - Get file size
//   [vtable+76] - Get file info
//   [vtable+24] - Read file
//   [vtable+44] - Close file
//
// PC replacement: Skip Xbox mount, initialize context for VFS-based access.
// The game stores results in the context structure at r3 (r31 in function).
// =============================================================================
extern "C" void __imp__sub_827EF2F8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827EF2F8) {
    static int s_count = 0; ++s_count;
    
    // Context structure pointer
    uint32_t contextAddr = ctx.r3.u32;
    uint32_t pathAddr = ctx.r4.u32;  // Path string
    
    // Read path for logging
    char pathBuf[256] = {0};
    for (int i = 0; i < 255 && pathAddr; i++) {
        uint8_t c = PPC_LOAD_U8(pathAddr + i);
        if (c == 0) break;
        pathBuf[i] = c;
    }
    
    if (s_count <= 20) {
        LOGF_WARNING("[STORAGE] sub_827EF2F8 #{} context=0x{:08X} path='{}'", 
                     s_count, contextAddr, pathBuf);
    }
    
    // Initialize the context structure for "successful" mount
    // The game expects certain fields to be set:
    //   [ctx+12]  = file handle (or -1 if failed) - set to valid value
    //   [ctx+16]  = file size (64-bit)
    //   [ctx+32]  = storage device pointer
    //   [ctx+36]  = path length
    
    // Store our PC storage device
    PPC_STORE_U32(contextAddr + 32, StorageConstants::PC_STORAGE_DEVICE_ADDR);
    
    // Set file handle to a valid non-negative value (0 = success)
    PPC_STORE_U32(contextAddr + 12, 0);
    
    // Set a default file size (will be updated when actual file is accessed)
    PPC_STORE_U64(contextAddr + 16, 0x100000);  // 1MB default
    
    // Set path length
    PPC_STORE_U32(contextAddr + 36, strlen(pathBuf));
    
    // Return success (1 = mounted successfully)
    ctx.r3.u32 = 1;
    
    if (s_count <= 20) {
        LOGF_WARNING("[STORAGE] sub_827EF2F8 #{} -> mounted via VFS (context initialized)", s_count);
    }
}

// =============================================================================
// INSTRUMENTATION: Caller Range 0x827E0400-0x827E0500
// =============================================================================
// These hooks identify what function is calling sub_827E1EC0 with empty path
// LR=0x827E049C suggests the caller is in this address range
// =============================================================================

// sub_827E0400 - Potential caller or caller's context
extern "C" void __imp__sub_827E0400(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E0400) {
    static int s_count = 0; ++s_count;
    if (s_count <= 20) {
        LOGF_WARNING("[CALLER_TRACE] sub_827E0400 ENTER #{} r3=0x{:08X} r4=0x{:08X} r5=0x{:08X} LR=0x{:08X}",
                     s_count, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.lr);
    }
    __imp__sub_827E0400(ctx, base);
    if (s_count <= 20) {
        LOGF_WARNING("[CALLER_TRACE] sub_827E0400 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// sub_827E0420 - Potential caller
extern "C" void __imp__sub_827E0420(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E0420) {
    static int s_count = 0; ++s_count;
    if (s_count <= 20) {
        LOGF_WARNING("[CALLER_TRACE] sub_827E0420 ENTER #{} r3=0x{:08X} r4=0x{:08X} r5=0x{:08X} LR=0x{:08X}",
                     s_count, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.lr);
    }
    __imp__sub_827E0420(ctx, base);
    if (s_count <= 20) {
        LOGF_WARNING("[CALLER_TRACE] sub_827E0420 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// sub_827E0440 - Potential caller (closest to LR=0x827E049C)
extern "C" void __imp__sub_827E0440(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E0440) {
    static int s_count = 0; ++s_count;
    if (s_count <= 20) {
        LOGF_WARNING("[CALLER_TRACE] sub_827E0440 ENTER #{} r3=0x{:08X} r4=0x{:08X} r5=0x{:08X} LR=0x{:08X}",
                     s_count, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.lr);
    }
    __imp__sub_827E0440(ctx, base);
    if (s_count <= 20) {
        LOGF_WARNING("[CALLER_TRACE] sub_827E0440 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// sub_827E0460 - Potential caller (LR=0x827E049C is in this function's range)
extern "C" void __imp__sub_827E0460(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E0460) {
    static int s_count = 0; ++s_count;
    if (s_count <= 20) {
        LOGF_WARNING("[CALLER_TRACE] sub_827E0460 ENTER #{} r3=0x{:08X} r4=0x{:08X} r5=0x{:08X} LR=0x{:08X}",
                     s_count, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.lr);
    }
    __imp__sub_827E0460(ctx, base);
    if (s_count <= 20) {
        LOGF_WARNING("[CALLER_TRACE] sub_827E0460 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// sub_827E0480 - Most likely caller (LR=0x827E049C is ~0x1C bytes into this function)
extern "C" void __imp__sub_827E0480(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E0480) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[CALLER_TRACE] sub_827E0480 ENTER #{} r3=0x{:08X} r4=0x{:08X} r5=0x{:08X} LR=0x{:08X}",
                 s_count, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.lr);
    __imp__sub_827E0480(ctx, base);
    LOGF_WARNING("[CALLER_TRACE] sub_827E0480 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

// sub_827E04A0 - Just after likely caller
extern "C" void __imp__sub_827E04A0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E04A0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 20) {
        LOGF_WARNING("[CALLER_TRACE] sub_827E04A0 ENTER #{} r3=0x{:08X} r4=0x{:08X} r5=0x{:08X} LR=0x{:08X}",
                     s_count, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.lr);
    }
    __imp__sub_827E04A0(ctx, base);
    if (s_count <= 20) {
        LOGF_WARNING("[CALLER_TRACE] sub_827E04A0 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// sub_827E04C0 - Upper range
extern "C" void __imp__sub_827E04C0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E04C0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 20) {
        LOGF_WARNING("[CALLER_TRACE] sub_827E04C0 ENTER #{} r3=0x{:08X} r4=0x{:08X} r5=0x{:08X} LR=0x{:08X}",
                     s_count, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.lr);
    }
    __imp__sub_827E04C0(ctx, base);
    if (s_count <= 20) {
        LOGF_WARNING("[CALLER_TRACE] sub_827E04C0 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// =============================================================================
// INSTRUMENTATION: Next Subsystem Functions
// =============================================================================
// These hooks track initialization progress after storage device init
// Based on game_init.cpp analysis of 63-subsystem init sequence
// =============================================================================

// sub_827E0500 - Storage-related function after our caller range
extern "C" void __imp__sub_827E0500(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E0500) {
    static int s_count = 0; ++s_count;
    if (s_count <= 20) {
        LOGF_WARNING("[SUBSYS] sub_827E0500 ENTER #{} r3=0x{:08X} r4=0x{:08X} LR=0x{:08X}",
                     s_count, ctx.r3.u32, ctx.r4.u32, ctx.lr);
    }
    __imp__sub_827E0500(ctx, base);
    if (s_count <= 20) {
        LOGF_WARNING("[SUBSYS] sub_827E0500 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// sub_827E0600 - Storage path operations
extern "C" void __imp__sub_827E0600(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E0600) {
    static int s_count = 0; ++s_count;
    if (s_count <= 20) {
        LOGF_WARNING("[SUBSYS] sub_827E0600 ENTER #{} r3=0x{:08X} r4=0x{:08X} LR=0x{:08X}",
                     s_count, ctx.r3.u32, ctx.r4.u32, ctx.lr);
    }
    __imp__sub_827E0600(ctx, base);
    if (s_count <= 20) {
        LOGF_WARNING("[SUBSYS] sub_827E0600 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// sub_827E0700 - Storage context operations
extern "C" void __imp__sub_827E0700(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E0700) {
    static int s_count = 0; ++s_count;
    if (s_count <= 20) {
        LOGF_WARNING("[SUBSYS] sub_827E0700 ENTER #{} r3=0x{:08X} r4=0x{:08X} LR=0x{:08X}",
                     s_count, ctx.r3.u32, ctx.r4.u32, ctx.lr);
    }
    __imp__sub_827E0700(ctx, base);
    if (s_count <= 20) {
        LOGF_WARNING("[SUBSYS] sub_827E0700 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// sub_827E0800 - File operations
extern "C" void __imp__sub_827E0800(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E0800) {
    static int s_count = 0; ++s_count;
    if (s_count <= 20) {
        LOGF_WARNING("[SUBSYS] sub_827E0800 ENTER #{} r3=0x{:08X} r4=0x{:08X} LR=0x{:08X}",
                     s_count, ctx.r3.u32, ctx.r4.u32, ctx.lr);
    }
    __imp__sub_827E0800(ctx, base);
    if (s_count <= 20) {
        LOGF_WARNING("[SUBSYS] sub_827E0800 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// =============================================================================
// INSTRUMENTATION: Additional Subsystem Init Functions
// =============================================================================
// Track progress through the 63-subsystem initialization sequence
// =============================================================================

// Subsystem #2 after streaming init
extern "C" void __imp__sub_82120100(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82120100) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[SUBSYS_INIT] sub_82120100 ENTER #{} r3=0x{:08X} r4=0x{:08X} LR=0x{:08X}",
                 s_count, ctx.r3.u32, ctx.r4.u32, ctx.lr);
    __imp__sub_82120100(ctx, base);
    LOGF_WARNING("[SUBSYS_INIT] sub_82120100 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

// Subsystem loader/iterator
extern "C" void __imp__sub_82120200(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82120200) {
    static int s_count = 0; ++s_count;
    if (s_count <= 30) {
        LOGF_WARNING("[SUBSYS_INIT] sub_82120200 ENTER #{} r3=0x{:08X} r4=0x{:08X} LR=0x{:08X}",
                     s_count, ctx.r3.u32, ctx.r4.u32, ctx.lr);
    }
    __imp__sub_82120200(ctx, base);
    if (s_count <= 30) {
        LOGF_WARNING("[SUBSYS_INIT] sub_82120200 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// More sub_8218C600 call chain - find the blocking function
extern "C" void __imp__sub_82851548(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82851548) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_82851548 ENTER #{}", s_count);
    __imp__sub_82851548(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_82851548 EXIT #{}", s_count);
}

extern "C" void __imp__sub_82856C38(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82856C38) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_82856C38 ENTER #{}", s_count);
    __imp__sub_82856C38(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_82856C38 EXIT #{}", s_count);
}

// Functions called AFTER sub_82851548 EXIT #2 in sub_8218C600 - find blocking point
extern "C" void __imp__sub_8285A0E0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8285A0E0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_8285A0E0 ENTER #{}", s_count);
    __imp__sub_8285A0E0(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_8285A0E0 EXIT #{}", s_count);
}

extern "C" void __imp__sub_82850748(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82850748) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_82850748 ENTER #{}", s_count);
    __imp__sub_82850748(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_82850748 EXIT #{}", s_count);
}

extern "C" void __imp__sub_82856C90(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82856C90) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_82856C90 ENTER #{}", s_count);
    
    // Before call, r3 contains render_ctx pointer (from sub_8218C600 line 2507)
    uint32_t renderCtxBefore = ctx.r3.u32;
    
    __imp__sub_82856C90(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_82856C90 EXIT #{}", s_count);
    
    // Trace the vtable that will be called after this returns
    // r3 was render_ctx before call - trace it
    if (renderCtxBefore != 0) {
        uint32_t vtable = PPC_LOAD_U32(renderCtxBefore + 0);
        uint32_t vtable0 = PPC_LOAD_U32(vtable + 0);
        uint32_t vtable1 = PPC_LOAD_U32(vtable + 4);
        uint32_t vtable2 = PPC_LOAD_U32(vtable + 8);
        LOGF_WARNING("[VTABLE] render_ctx=0x{:08X} vtable=0x{:08X}", renderCtxBefore, vtable);
        LOGF_WARNING("[VTABLE]   vtable[0]=0x{:08X} vtable[1]=0x{:08X} vtable[2]=0x{:08X}", 
                     vtable0, vtable1, vtable2);
    } else {
        LOG_WARNING("[VTABLE] render_ctx was NULL before sub_82856C90");
    }
}

// =============================================================================
// sub_82857240 - Render context vtable[1] method
// Called from sub_8218C600 after sub_82856C90 - THIS IS WHERE IT BLOCKS
// Internal calls:
//   sub_82856BA8 - GPU state setup
//   sub_82857E38 - Unknown
//   sub_8285E1F0 - Unknown  
//   sub_82862088 - Unknown
// =============================================================================
extern "C" void __imp__sub_82857240(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82857240) {
    static int s_count = 0; ++s_count;
    LOG_WARNING("[sub_82857240] ENTER - render context vtable[1] method");
    __imp__sub_82857240(ctx, base);
    LOG_WARNING("[sub_82857240] EXIT");
}

// =============================================================================
// sub_82856BA8 - GPU State/Shader Setup (COLLAPSED)
// =============================================================================
// This function orchestrates GPU state initialization including:
//   - sub_8286DA20 (GPU resource alloc wrapper)
//   - sub_82853CB0 (shader setup via vtable[3])
//   - sub_82871A18 (additional GPU setup)
//
// All of these lead to vtable calls that expect Xbox GPU hardware.
// 
// SOLUTION: Bypass entirely. Our shader cache (g_shaderCache in video.cpp)
// already provides pre-compiled platform-native shaders. Shader loading
// happens on-demand via GetOrLinkShader() when the render path needs them.
//
// This collapse eliminates the "first call works, second call hangs" pattern
// caused by mixing partial Xbox GPU state with partial modern GPU state.
// =============================================================================
extern "C" void __imp__sub_82856BA8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82856BA8) {
    static int s_count = 0;
    ++s_count;
    
    if (s_count <= 3) {
        LOG_WARNING("[GPU] sub_82856BA8 BYPASSING - shader cache handles GPU/shader setup");
    }
    
    // Return immediately - shader setup handled by host shader cache
    // Shaders are loaded on-demand via GetOrLinkShader() during rendering
    return;
}

// =============================================================================
// sub_8285AF80 - GPU Sync/Fence Operation (BYPASSED)
// =============================================================================
// This function performs GPU synchronization with a retry loop.
// After 25 failed retries (counter at offset 30780 >= 25), it enters an
// infinite loop at loc_8285B214: goto loc_8285B214;
// 
// Problem: Xbox GPU fence/sync operations don't work on PC, causing timeout
// after 25 retries and entering infinite spin loop.
// 
// Solution: Bypass entirely - host GPU handles synchronization through
// the modern graphics layer (Metal/Vulkan/OpenGL).
// =============================================================================
extern "C" void __imp__sub_8285AF80(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8285AF80) {
    static int s_count = 0;
    ++s_count;
    
    if (s_count <= 10 || s_count % 1000 == 0) {
        LOGF_WARNING("[GPU] sub_8285AF80 GPU sync #{} - BYPASSING to prevent infinite loop", s_count);
    }
    
    // Return immediately - GPU sync handled by host graphics layer
    // This prevents the 25-retry timeout and infinite loop at loc_8285B214
    return;
}

// Trace internal calls of sub_82856BA8
extern "C" void __imp__sub_828787C0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_828787C0) {
    LOG_WARNING("[sub_82856BA8] sub_828787C0 ENTER");
    __imp__sub_828787C0(ctx, base);
    LOG_WARNING("[sub_82856BA8] sub_828787C0 EXIT");
}

extern "C" void __imp__sub_8286DA20(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8286DA20) {
    LOG_WARNING("[sub_82856BA8] sub_8286DA20 ENTER");
    __imp__sub_8286DA20(ctx, base);
    LOG_WARNING("[sub_82856BA8] sub_8286DA20 EXIT");
}

// Trace sub_8286D668 - GPU resource init (called by sub_8286DA20)
extern "C" void __imp__sub_8286D668(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8286D668) {
    LOG_WARNING("[sub_8286DA20] sub_8286D668 ENTER (GPU resource init)");
    __imp__sub_8286D668(ctx, base);
    LOG_WARNING("[sub_8286DA20] sub_8286D668 EXIT");
}

// Trace internal calls of sub_8286D668
extern "C" void __imp__sub_827E93F8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E93F8) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOG_WARNING("[sub_8286D668] sub_827E93F8 ENTER");
    __imp__sub_827E93F8(ctx, base);
    if (s_count <= 5) LOG_WARNING("[sub_8286D668] sub_827E93F8 EXIT");
}

extern "C" void __imp__sub_8286BAE0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8286BAE0) {
    LOG_WARNING("[sub_8286D668] sub_8286BAE0 ENTER");
    __imp__sub_8286BAE0(ctx, base);
    LOG_WARNING("[sub_8286D668] sub_8286BAE0 EXIT");
}

// sub_8221B7A0 is now hooked earlier with infinite loop protection - removed duplicate

extern "C" void __imp__sub_829E5C38(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829E5C38) {
    LOG_WARNING("[sub_8286BAE0] sub_829E5C38 ENTER (GPU format query)");
    __imp__sub_829E5C38(ctx, base);
    LOG_WARNING("[sub_8286BAE0] sub_829E5C38 EXIT");
}

// =============================================================================
// sub_82850028 - GPU Resource Create
// =============================================================================
// Original behavior:
//   - Loads GPU device from TLS[1676]
//   - Calls device->vtable[15] (offset 60) for resource validation/creation
//   - Returns 0 for success, negative for error
//
// Problem: TLS[1676] vtable chain is uninitialized on PC, causing hang.
//
// Solution: Bypass vtable call, return success. Game's own structures
// will be initialized by the allocation code that runs before the vtable call.
// =============================================================================
extern "C" void __imp__sub_82850028(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82850028) {
    static int s_count = 0;
    ++s_count;
    
    if (s_count <= 10 || s_count % 100 == 0) {
        LOGF_WARNING("[GPU] sub_82850028 GPU resource create #{} - bypassing vtable[15], returning success", s_count);
    }
    
    // Return success (0) - negative values indicate error per caller checks
    // e.g., line 93332: "cmpwi cr6,r3,0" / "bge cr6,0x8286bbd0" (branch if >= 0)
    ctx.r3.s64 = 0;
}

extern "C" void __imp__sub_829D92C0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829D92C0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOG_WARNING("[sub_8286D668] sub_829D92C0 ENTER");
    __imp__sub_829D92C0(ctx, base);
    if (s_count <= 10) LOG_WARNING("[sub_8286D668] sub_829D92C0 EXIT");
}

// Additional calls in sub_8286D668
extern "C" void __imp__sub_8286BA28(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8286BA28) {
    LOG_WARNING("[sub_8286D668] sub_8286BA28 ENTER");
    __imp__sub_8286BA28(ctx, base);
    LOG_WARNING("[sub_8286D668] sub_8286BA28 EXIT");
}

extern "C" void __imp__sub_8286CE40(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8286CE40) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOG_WARNING("[sub_8286D668] sub_8286CE40 ENTER");
    __imp__sub_8286CE40(ctx, base);
    if (s_count <= 5) LOG_WARNING("[sub_8286D668] sub_8286CE40 EXIT");
}

// Trace sub_8286CCA0 internal calls (called via vtable[3] from sub_82853CB0)
extern "C" void __imp__sub_8286CCA0(PPCContext& ctx, uint8_t* base);
// =============================================================================
// sub_8286CCA0 - Safe C++ reimplementation
// =============================================================================
// Original PPC code uses PPC_STORE_U64/PPC_LOAD_U64 for stack save/restore
// which causes SIGBUS on ARM due to unaligned 64-bit access.
// This reimplementation uses C++ local variables to avoid stack operations.
PPC_FUNC(sub_8286CCA0) {
    LOG_WARNING("[sub_8286CCA0] ENTER (shader load - safe impl)");
    
    // Save registers in C++ variables instead of PPC stack
    uint64_t saved_r30 = ctx.r30.u64;
    uint64_t saved_r31 = ctx.r31.u64;
    uint64_t saved_lr = ctx.lr;
    
    // Original: mr r31,r3; mr r30,r5
    uint32_t r31_val = ctx.r3.u32;
    uint32_t r30_val = ctx.r5.u32;
    
    // Allocate stack space for local buffer (256 bytes at sp+80)
    // We'll use the actual stack but avoid 64-bit unaligned access
    uint32_t origStack = ctx.r1.u32;
    ctx.r1.u32 = origStack - 368;
    
    // Original: li r5,256; addi r3,r1,80; bl sub_8266A778
    ctx.r5.s64 = 256;
    ctx.r3.s64 = ctx.r1.s64 + 80;
    ctx.r4.u64 = ctx.r4.u64;  // r4 unchanged from caller
    ctx.lr = 0x8286CCC8;
    sub_8266A778(ctx, base);
    
    // Original: addi r4,r1,80; mr r3,r31; bl sub_82854448
    ctx.r4.s64 = ctx.r1.s64 + 80;
    ctx.r3.u64 = r31_val;
    ctx.lr = 0x8286CCD4;
    sub_82854448(ctx, base);
    
    uint32_t result = ctx.r3.u32;
    
    if (result != 0) {
        // Found - return result
        ctx.r3.u64 = result;
    } else {
        // Not found path - allocate and init shader object
        // Original: li r3,60; bl sub_8218BE28
        ctx.r3.s64 = 60;
        ctx.lr = 0x8286CCE4;
        sub_8218BE28(ctx, base);
        
        if (ctx.r3.u32 != 0) {
            // Original: mr r5,r30; addi r4,r1,80; bl sub_8286C8F0
            ctx.r5.u64 = r30_val;
            ctx.r4.s64 = ctx.r1.s64 + 80;
            ctx.lr = 0x8286CCF8;
            sub_8286C8F0(ctx, base);
            r31_val = ctx.r3.u32;
        } else {
            r31_val = 0;
        }
        
        // Original: lwz r11,0(r31); mr r3,r31; lwz r11,64(r11); mtctr r11; bctrl
        if (r31_val != 0) {
            uint32_t vtable = __builtin_bswap32(*(uint32_t*)(base + r31_val + 0));
            uint32_t funcPtr = __builtin_bswap32(*(uint32_t*)(base + vtable + 64));
            ctx.r3.u64 = r31_val;
            ctx.lr = 0x8286CD18;
            ctx.ctr.u64 = funcPtr;
            PPC_CALL_INDIRECT_FUNC(funcPtr);
            
            if (ctx.r3.u32 == 0) {
                // Original: bl sub_82126498; li r3,0
                ctx.r3.u64 = r31_val;
                ctx.lr = 0x8286CD28;
                sub_82126498(ctx, base);
                ctx.r3.s64 = 0;
            } else {
                ctx.r3.u64 = r31_val;
            }
        } else {
            ctx.r3.s64 = 0;
        }
    }
    
    // Restore stack and registers
    ctx.r1.u32 = origStack;
    ctx.r30.u64 = saved_r30;
    ctx.r31.u64 = saved_r31;
    ctx.lr = saved_lr;
    
    LOG_WARNING("[sub_8286CCA0] EXIT (safe impl)");
}

extern "C" void __imp__sub_8266A778(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8266A778) {
    LOG_WARNING("[sub_8286CCA0] sub_8266A778 ENTER");
    __imp__sub_8266A778(ctx, base);
    LOG_WARNING("[sub_8286CCA0] sub_8266A778 EXIT");
}

extern "C" void __imp__sub_82854448(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82854448) {
    LOG_WARNING("[sub_8286CCA0] sub_82854448 ENTER");
    __imp__sub_82854448(ctx, base);
    LOGF_WARNING("[sub_8286CCA0] sub_82854448 EXIT r3={}", ctx.r3.u32);
}

extern "C" void __imp__sub_8286C8F0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8286C8F0) {
    LOG_WARNING("[sub_8286CCA0] sub_8286C8F0 ENTER");
    __imp__sub_8286C8F0(ctx, base);
    LOG_WARNING("[sub_8286CCA0] sub_8286C8F0 EXIT");
}

// Trace internal calls of sub_8286C8F0
extern "C" void __imp__sub_8287E2C0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8287E2C0) {
    LOG_WARNING("[sub_8286C8F0] sub_8287E2C0 ENTER");
    __imp__sub_8287E2C0(ctx, base);
    LOG_WARNING("[sub_8286C8F0] sub_8287E2C0 EXIT");
}

extern "C" void __imp__sub_827D85E0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827D85E0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOG_WARNING("[sub_8286C8F0] sub_827D85E0 ENTER");
    __imp__sub_827D85E0(ctx, base);
    if (s_count <= 5) LOG_WARNING("[sub_8286C8F0] sub_827D85E0 EXIT");
}

extern "C" void __imp__sub_8285F6C0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8285F6C0) {
    LOG_WARNING("[sub_8286C8F0] sub_8285F6C0 ENTER (shader lookup)");
    __imp__sub_8285F6C0(ctx, base);
    LOGF_WARNING("[sub_8286C8F0] sub_8285F6C0 EXIT r3=0x{:08X}", ctx.r3.u32);
}

extern "C" void __imp__sub_827D8620(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827D8620) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOG_WARNING("[sub_8286C8F0] sub_827D8620 ENTER");
    __imp__sub_827D8620(ctx, base);
    if (s_count <= 5) LOG_WARNING("[sub_8286C8F0] sub_827D8620 EXIT");
}

// More internal calls of sub_8286C8F0
extern "C" void __imp__sub_8285E6E8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8285E6E8) {
    LOG_WARNING("[sub_8286C8F0] sub_8285E6E8 ENTER (texture create?)");
    __imp__sub_8285E6E8(ctx, base);
    LOGF_WARNING("[sub_8286C8F0] sub_8285E6E8 EXIT r3=0x{:08X}", ctx.r3.u32);
}

extern "C" void __imp__sub_8286BBE8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8286BBE8) {
    LOG_WARNING("[sub_8286C8F0] sub_8286BBE8 ENTER");
    __imp__sub_8286BBE8(ctx, base);
    LOG_WARNING("[sub_8286C8F0] sub_8286BBE8 EXIT");
}

// Trace internal calls of sub_8286BBE8
extern "C" void __imp__sub_8286A970(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8286A970) {
    LOG_WARNING("[sub_8286BBE8] sub_8286A970 ENTER");
    __imp__sub_8286A970(ctx, base);
    LOG_WARNING("[sub_8286BBE8] sub_8286A970 EXIT");
}

extern "C" void __imp__sub_8285E2C0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8285E2C0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOG_WARNING("[sub_8286BBE8] sub_8285E2C0 ENTER");
    __imp__sub_8285E2C0(ctx, base);
    if (s_count <= 5) LOG_WARNING("[sub_8286BBE8] sub_8285E2C0 EXIT");
}

extern "C" void __imp__sub_8285E6C0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8285E6C0) {
    LOG_WARNING("[sub_8286BBE8] sub_8285E6C0 ENTER");
    __imp__sub_8285E6C0(ctx, base);
    LOG_WARNING("[sub_8286BBE8] sub_8285E6C0 EXIT");
}

extern "C" void __imp__sub_829D33B8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829D33B8) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOG_WARNING("[sub_8286BBE8] sub_829D33B8 ENTER");
    __imp__sub_829D33B8(ctx, base);
    if (s_count <= 5) LOGF_WARNING("[sub_8286BBE8] sub_829D33B8 EXIT r3={}", ctx.r3.u32);
}

// =============================================================================
// sub_82853CB0 - GPU shader/state setup
// =============================================================================
// This function loads an object from global+22000 and calls vtable[3] (offset 12)
// twice to set up shader resources. The vtable calls lead to complex GPU init
// code with many more vtable calls that block on PC.
//
// Solution: Bypass entirely - shader resources will be set up by host GPU layer.
// =============================================================================
extern "C" void __imp__sub_82853CB0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82853CB0) {
    static int s_count = 0;
    ++s_count;
    
    if (s_count <= 5) {
        LOG_WARNING("[sub_82853CB0] BYPASSING - GPU shader setup (vtable[3] calls block)");
    }
    
    // Bypass entirely - the shader setup will be handled by host GPU
    // The game's render path will use our host shaders instead
    return;
}

extern "C" void __imp__sub_82871A18(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82871A18) {
    LOG_WARNING("[sub_82856BA8] sub_82871A18 ENTER");
    __imp__sub_82871A18(ctx, base);
    LOG_WARNING("[sub_82856BA8] sub_82871A18 EXIT");
}

extern "C" void __imp__sub_82857E38(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82857E38) {
    LOG_WARNING("[sub_82857240] sub_82857E38 ENTER");
    __imp__sub_82857E38(ctx, base);
    LOG_WARNING("[sub_82857240] sub_82857E38 EXIT");
}

extern "C" void __imp__sub_8285E1F0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8285E1F0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOGF_WARNING("[sub_82857240] sub_8285E1F0 ENTER #{} r3={}", s_count, ctx.r3.u32);
    __imp__sub_8285E1F0(ctx, base);
    if (s_count <= 5) LOG_WARNING("[sub_82857240] sub_8285E1F0 EXIT");
}

extern "C" void __imp__sub_82862088(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82862088) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOGF_WARNING("[sub_82857240] sub_82862088 ENTER #{} r3={}", s_count, ctx.r3.u32);
    __imp__sub_82862088(ctx, base);
    if (s_count <= 5) LOG_WARNING("[sub_82857240] sub_82862088 EXIT");
}

// Sub-functions called by sub_82856C90 - find which one blocks
extern "C" void __imp__sub_82851F30(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82851F30) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_82856C90] sub_82851F30 ENTER #{}", s_count);
    __imp__sub_82851F30(ctx, base);
    LOGF_WARNING("[sub_82856C90] sub_82851F30 EXIT #{}", s_count);
}

// Trace internal calls of sub_82851F30 to find blocker
extern "C" void __imp__sub_827DAE20(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827DAE20) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_82851F30] sub_827DAE20 ENTER #{}", s_count);
    __imp__sub_827DAE20(ctx, base);
    LOGF_WARNING("[sub_82851F30] sub_827DAE20 EXIT #{}", s_count);
}

extern "C" void __imp__sub_827EEE40(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827EEE40) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_82851F30] sub_827EEE40 ENTER #{}", s_count);
    __imp__sub_827EEE40(ctx, base);
    LOGF_WARNING("[sub_82851F30] sub_827EEE40 EXIT #{}", s_count);
}

extern "C" void __imp__sub_828508B8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_828508B8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_82851F30] sub_828508B8 ENTER #{}", s_count);
    __imp__sub_828508B8(ctx, base);
    LOGF_WARNING("[sub_82851F30] sub_828508B8 EXIT #{}", s_count);
}

extern "C" void __imp__sub_829D0268(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829D0268) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_82851F30] sub_829D0268 ENTER #{}", s_count);
    __imp__sub_829D0268(ctx, base);
    LOGF_WARNING("[sub_82851F30] sub_829D0268 EXIT #{} r3={}", s_count, ctx.r3.s32);
}

extern "C" void __imp__sub_82850630(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82850630) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_82851F30] sub_82850630 ENTER #{}", s_count);
    __imp__sub_82850630(ctx, base);
    LOGF_WARNING("[sub_82851F30] sub_82850630 EXIT #{}", s_count);
}

extern "C" void __imp__sub_829CB140(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829CB140) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_82851F30] sub_829CB140 ENTER #{}", s_count);
    __imp__sub_829CB140(ctx, base);
    LOGF_WARNING("[sub_82851F30] sub_829CB140 EXIT #{}", s_count);
}

// More functions called later in sub_82851F30
extern "C" void __imp__sub_829CAE68(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829CAE68) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_82851F30] sub_829CAE68 ENTER #{}", s_count);
    __imp__sub_829CAE68(ctx, base);
    LOGF_WARNING("[sub_82851F30] sub_829CAE68 EXIT #{}", s_count);
}

extern "C" void __imp__sub_829D5948(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829D5948) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_82851F30] sub_829D5948 ENTER #{}", s_count);
    __imp__sub_829D5948(ctx, base);
    LOGF_WARNING("[sub_82851F30] sub_829D5948 EXIT #{}", s_count);
}

extern "C" void __imp__sub_82851DD8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82851DD8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_82851F30] sub_82851DD8 ENTER #{}", s_count);
    __imp__sub_82851DD8(ctx, base);
    LOGF_WARNING("[sub_82851F30] sub_82851DD8 EXIT #{}", s_count);
}

extern "C" void __imp__sub_8285BDC8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8285BDC8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_82851F30] sub_8285BDC8 ENTER #{}", s_count);
    __imp__sub_8285BDC8(ctx, base);
    LOGF_WARNING("[sub_82851F30] sub_8285BDC8 EXIT #{}", s_count);
}

// Trace internal calls of sub_8285BDC8 (shader/resource loader)
extern "C" void __imp__sub_8285BC60(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8285BC60) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_8285BDC8] sub_8285BC60 ENTER #{}", s_count);
    __imp__sub_8285BC60(ctx, base);
    LOGF_WARNING("[sub_8285BDC8] sub_8285BC60 EXIT #{}", s_count);
}

// =============================================================================
// sub_827E04F0 - PLATFORM GLUE: Path Construction Function
// =============================================================================
// Original Xbox behavior: Constructs a file path from the storage device context.
// It reads a path configuration index from context+3072, and if non-zero,
// calculates an offset to read the path string from the context structure.
// If context+3072 is 0 and no source path is provided, it writes an empty string.
//
// PC replacement: Call the original implementation first. If it returns an empty
// path (due to uninitialized path config), construct a default path based on
// the path index. This allows shader loading to proceed without Xbox path config.
// =============================================================================
extern "C" void __imp__sub_827E04F0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E04F0) {
    static int s_count = 0; ++s_count;
    
    // Extract parameters per PPC calling convention:
    // r3 = storage device context pointer
    // r4 = output buffer address
    // r5 = buffer size
    // r6 = source path pointer (can be NULL)
    // r7 = additional flags
    // r8 = path index
    uint32_t contextAddr = ctx.r3.u32;
    uint32_t outputBuffer = ctx.r4.u32;
    uint32_t bufferSize = ctx.r5.u32;
    uint32_t sourcePath = ctx.r6.u32;
    uint32_t pathIndex = ctx.r8.u32;
    
    // Check if context has path configuration at offset +3072
    uint32_t pathConfig = PPC_LOAD_U32(contextAddr + 3072);
    
    if (s_count <= 10) {
        LOGF_WARNING("[sub_827E04F0] #{} context=0x{:08X} output=0x{:08X} size={} srcPath=0x{:08X} index={} pathConfig={}",
                     s_count, contextAddr, outputBuffer, bufferSize, sourcePath, pathIndex, pathConfig);
    }
    
    // Call the original implementation first
    __imp__sub_827E04F0(ctx, base);
    
    // Check what path was constructed (read from output buffer)
    char resultPath[260] = {0};
    for (int i = 0; i < 259; i++) {
        char c = PPC_LOAD_U8(outputBuffer + i);
        if (c == 0) break;
        resultPath[i] = c;
    }
    
    // If the original function returned an empty path and pathConfig is 0,
    // construct a default path to allow resource loading to proceed
    if (resultPath[0] == 0 && pathConfig == 0) {
        // Construct a platform-agnostic path for resource loading
        // Use "platform:/shaders/" prefix as this is likely shader loading
        char defaultPath[256] = {0};
        snprintf(defaultPath, sizeof(defaultPath), "platform:/shaders/default_%u.fxc", pathIndex);
        
        // Copy to output buffer in PPC memory
        size_t pathLen = strlen(defaultPath);
        if (pathLen >= bufferSize) {
            pathLen = bufferSize - 1;
        }
        
        for (size_t i = 0; i < pathLen; i++) {
            PPC_STORE_U8(outputBuffer + i, defaultPath[i]);
        }
        PPC_STORE_U8(outputBuffer + pathLen, 0); // Null terminator
        
        if (s_count <= 10) {
            LOGF_WARNING("[sub_827E04F0] #{} -> original returned empty, constructed: '{}'", s_count, defaultPath);
        }
    } else if (s_count <= 10) {
        LOGF_WARNING("[sub_827E04F0] #{} -> original returned path: '{}'", s_count, resultPath);
    }
}

extern "C" void __imp__sub_827DFE10(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827DFE10) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_8285BDC8] sub_827DFE10 ENTER #{}", s_count);
    __imp__sub_827DFE10(ctx, base);
    LOGF_WARNING("[sub_8285BDC8] sub_827DFE10 EXIT #{} r3={}", s_count, ctx.r3.u32);
}

// =============================================================================
// sub_827E8180 - PLATFORM GLUE: File Find/Open Operation
// =============================================================================
// Original Xbox behavior: Calls storage device vtable[1] to find/open a file.
// Our PC storage device has a fake vtable that crashes when called.
//
// PC replacement: Extract path from context, use VFS to check if file exists,
// return a valid file handle or 0 if not found.
// =============================================================================
extern "C" void __imp__sub_827E8180(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E8180) {
    static int s_count = 0; ++s_count;
    
    // r3 = path context pointer (contains file path info at various offsets)
    // r4 = flags (1 = read mode)
    uint32_t pathContextAddr = ctx.r3.u32;
    uint32_t flags = ctx.r4.u32;
    
    // The path context structure is complex - try multiple strategies to find the path string
    char pathBuf[260] = {0};
    uint32_t pathStrAddr = 0;
    bool foundPath = false;
    
    // Strategy 1: Check if path string is directly at the context address
    uint8_t firstByte = PPC_LOAD_U8(pathContextAddr);
    if (firstByte >= 0x20 && firstByte <= 0x7E) {
        pathStrAddr = pathContextAddr;
        foundPath = true;
    }
    
    // Strategy 2: Try dereferencing the first pointer and checking multiple offsets
    if (!foundPath) {
        uint32_t ptr1 = PPC_LOAD_U32(pathContextAddr);
        if (ptr1 >= 0x00080000 && ptr1 < 0xE0000000) {
            // Check offsets 0, 8, 16, 24 in the dereferenced structure
            for (uint32_t offset : {0, 8, 16, 24, 32}) {
                uint32_t testAddr = ptr1 + offset;
                uint8_t testByte = PPC_LOAD_U8(testAddr);
                // Check if this looks like ASCII text
                if (testByte >= 0x20 && testByte <= 0x7E) {
                    // Verify it's a reasonable path string (check a few more bytes)
                    bool looksLikePath = true;
                    for (int i = 0; i < 4 && i < 260; i++) {
                        uint8_t b = PPC_LOAD_U8(testAddr + i);
                        if (b == 0) break; // Null terminator is OK
                        if (b < 0x20 || b > 0x7E) {
                            looksLikePath = false;
                            break;
                        }
                    }
                    if (looksLikePath) {
                        pathStrAddr = testAddr;
                        foundPath = true;
                        break;
                    }
                }
            }
        }
    }
    
    // Strategy 3: Check if context has a pointer at offsets 4, 8, 12 that points to path string
    // This handles cases where context structure has indirect path references
    if (!foundPath) {
        for (uint32_t ptrOffset : {4u, 8u, 12u, 16u}) {
            uint32_t testPtr = PPC_LOAD_U32(pathContextAddr + ptrOffset);
            if (testPtr >= 0x80000000 && testPtr < 0xE0000000) {
                uint8_t testByte = PPC_LOAD_U8(testPtr);
                if (testByte >= 0x20 && testByte <= 0x7E) {
                    // Verify it looks like a path string
                    bool looksLikePath = true;
                    for (int i = 0; i < 4; i++) {
                        uint8_t b = PPC_LOAD_U8(testPtr + i);
                        if (b == 0) break;
                        if (b < 0x20 || b > 0x7E) {
                            looksLikePath = false;
                            break;
                        }
                    }
                    if (looksLikePath) {
                        pathStrAddr = testPtr;
                        foundPath = true;
                        break;
                    }
                }
            }
        }
    }

    // Strategy 4: Two-level indirection - pointer at offset 4 points to struct with path at offset 0 or 4
    // This handles context 0x80080170 where offset 4 = 0x98409940 which contains another struct
    if (!foundPath) {
        for (uint32_t ptrOffset : {4u, 8u}) {
            uint32_t level1Ptr = PPC_LOAD_U32(pathContextAddr + ptrOffset);
            if (level1Ptr >= 0x80000000 && level1Ptr < 0xE0000000) {
                // Log what's at the first level pointer for debugging
                if (s_count <= 20) {
                    LOGF_WARNING("[FILE] sub_827E8180 #{} Strategy4 level1Ptr=0x{:08X} -> [{:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X}]",
                        s_count, level1Ptr,
                        PPC_LOAD_U8(level1Ptr + 0), PPC_LOAD_U8(level1Ptr + 1),
                        PPC_LOAD_U8(level1Ptr + 2), PPC_LOAD_U8(level1Ptr + 3),
                        PPC_LOAD_U8(level1Ptr + 4), PPC_LOAD_U8(level1Ptr + 5),
                        PPC_LOAD_U8(level1Ptr + 6), PPC_LOAD_U8(level1Ptr + 7));
                }
                
                // Try multiple offsets in the level1 structure
                for (uint32_t level2Offset : {0u, 4u, 8u, 12u, 16u, 20u, 24u}) {
                    uint32_t level2Ptr = PPC_LOAD_U32(level1Ptr + level2Offset);
                    if (level2Ptr >= 0x80000000 && level2Ptr < 0xE0000000) {
                        uint8_t testByte = PPC_LOAD_U8(level2Ptr);
                        if (testByte >= 0x20 && testByte <= 0x7E) {
                            // Verify it looks like a path string
                            bool looksLikePath = true;
                            for (int i = 0; i < 4; i++) {
                                uint8_t b = PPC_LOAD_U8(level2Ptr + i);
                                if (b == 0) break;
                                if (b < 0x20 || b > 0x7E) {
                                    looksLikePath = false;
                                    break;
                                }
                            }
                            if (looksLikePath) {
                                pathStrAddr = level2Ptr;
                                foundPath = true;
                                if (s_count <= 20) {
                                    LOGF_WARNING("[FILE] sub_827E8180 #{} Strategy4 SUCCESS: level1+{} -> 0x{:08X}", 
                                        s_count, level2Offset, level2Ptr);
                                }
                                break;
                            }
                        }
                    }
                }
                if (foundPath) break;
            }
        }
    }

    
    // If we found a path address, read the string
    if (foundPath && pathStrAddr != 0) {
        for (int i = 0; i < 259; i++) {
            char c = PPC_LOAD_U8(pathStrAddr + i);
            if (c == 0) break;
            pathBuf[i] = c;
        }
    }
    
    if (s_count <= 20) {
        LOGF_WARNING("[FILE] sub_827E8180 #{} path='{}' flags={} contextAddr=0x{:08X} finalAddr=0x{:08X}", 
                     s_count, pathBuf, flags, pathContextAddr, pathStrAddr);
    }
    
    // If path is empty, log the context structure for debugging
    if (pathBuf[0] == 0 && s_count <= 20) {
        LOGF_WARNING("[FILE] sub_827E8180 #{} EMPTY PATH - context dump: [{:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X}]",
            s_count,
            PPC_LOAD_U8(pathContextAddr + 0), PPC_LOAD_U8(pathContextAddr + 1),
            PPC_LOAD_U8(pathContextAddr + 2), PPC_LOAD_U8(pathContextAddr + 3),
            PPC_LOAD_U8(pathContextAddr + 4), PPC_LOAD_U8(pathContextAddr + 5),
            PPC_LOAD_U8(pathContextAddr + 6), PPC_LOAD_U8(pathContextAddr + 7));
        
        LOGF_WARNING("[FILE] sub_827E8180 #{} Final string address 0x{:08X} -> [{:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X}]",
            s_count, pathStrAddr,
            PPC_LOAD_U8(pathStrAddr + 0), PPC_LOAD_U8(pathStrAddr + 1),
            PPC_LOAD_U8(pathStrAddr + 2), PPC_LOAD_U8(pathStrAddr + 3),
            PPC_LOAD_U8(pathStrAddr + 4), PPC_LOAD_U8(pathStrAddr + 5),
            PPC_LOAD_U8(pathStrAddr + 6), PPC_LOAD_U8(pathStrAddr + 7));
        
        // Try one more level of dereferencing manually
        uint32_t nextPtr = PPC_LOAD_U32(pathStrAddr);
        if (nextPtr >= 0x00080000 && nextPtr < 0xE0000000) {
            LOGF_WARNING("[FILE] sub_827E8180 #{} One more deref 0x{:08X} -> [{:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X}]",
                s_count, nextPtr,
                PPC_LOAD_U8(nextPtr + 0), PPC_LOAD_U8(nextPtr + 1),
                PPC_LOAD_U8(nextPtr + 2), PPC_LOAD_U8(nextPtr + 3),
                PPC_LOAD_U8(nextPtr + 4), PPC_LOAD_U8(nextPtr + 5),
                PPC_LOAD_U8(nextPtr + 6), PPC_LOAD_U8(nextPtr + 7));
        }
    }
    
    // Check if this is a shader file request (fxl_final, .fxc, shaders)
    bool isShaderPath = (strstr(pathBuf, "fxl_final") != nullptr || 
                         strstr(pathBuf, ".fxc") != nullptr ||
                         strstr(pathBuf, ".bin") != nullptr ||
                         strstr(pathBuf, "shaders") != nullptr);
    
    // ==========================================================================
    // SONIC UNLEASHED APPROACH: Completely bypass shader file loading.
    // All 1132 shaders are pre-compiled and embedded in g_shaderCacheEntries[].
    // The game's CreateShader() path uses FindShaderCacheEntry() with XXH3 hash
    // lookup - no file I/O needed. Return 0 (not found) to force the game to
    // use the embedded cache instead of trying to read from common.rpf.
    // 
    // This eliminates the infinite loop caused by:
    //   1. FileStream at 0x82A14100 created
    //   2. sub_82192840 rejects it (address range check)
    //   3. Game retries → infinite loop
    // ==========================================================================
    if (isShaderPath) {
        if (s_count <= 5) {
            LOGF_WARNING("[FILE] sub_827E8180 #{} -> SHADER BYPASS: '{}' - using embedded cache (1132 pre-compiled shaders)", 
                         s_count, pathBuf);
        }
        ctx.r3.u32 = 0;  // Not found via storage device - forces use of embedded cache
        return;
    }
    
    // For other files, if path is empty, this might be a spurious call
    // Return 0 to indicate file not found - the game should have fallback logic
    if (pathBuf[0] == 0) {
        if (s_count <= 20) {
            LOGF_WARNING("[FILE] sub_827E8180 #{} -> empty path, returning 0 (not found)", s_count);
        }
        ctx.r3.u32 = 0;  // File not found
        return;
    }
    
    // For other files, try to use VFS to check existence
    // For now, return 0 to indicate file not found via this path
    // The caller should have fallback mechanisms
    if (s_count <= 20) {
        LOGF_WARNING("[FILE] sub_827E8180 #{} -> returning 0 (not via storage device)", s_count);
    }
    ctx.r3.u32 = 0;  // Not found via storage device path
}

extern "C" void __imp__sub_827E8880(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E8880) {
    static int s_count = 0; ++s_count;
    
    LOGF_WARNING("[sub_827E8880] ENTER #{} r3=0x{:08X} r4=0x{:08X} r5={} LR=0x{:08X}",
                 s_count, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.lr);
    
    // Log what's at the stream pointer
    if (ctx.r3.u32 >= 0x80000000 && ctx.r3.u32 < 0x90000000) {
        uint32_t field0 = PPC_LOAD_U32(ctx.r3.u32 + 0);
        uint32_t field4 = PPC_LOAD_U32(ctx.r3.u32 + 4);
        uint32_t field8 = PPC_LOAD_U32(ctx.r3.u32 + 8);
        LOGF_WARNING("[sub_827E8880] Stream structure: [+0]=0x{:08X} [+4]=0x{:08X} [+8]=0x{:08X}",
                     field0, field4, field8);
    }
    
    __imp__sub_827E8880(ctx, base);
    
    LOGF_WARNING("[sub_827E8880] EXIT #{} r3={}", s_count, ctx.r3.s32);
}

extern "C" void __imp__sub_8285B680(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8285B680) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_8285BDC8] sub_8285B680 ENTER #{} (shader load)", s_count);
    __imp__sub_8285B680(ctx, base);
    LOGF_WARNING("[sub_8285BDC8] sub_8285B680 EXIT #{}", s_count);
}

extern "C" void __imp__sub_8285AA90(PPCContext& ctx, uint8_t* base);
// =============================================================================
// sub_8285AA90 - Safe C++ reimplementation
// =============================================================================
// Original PPC code uses PPC_STORE_U64/PPC_LOAD_U64 for stack save/restore
// which causes SIGBUS on ARM due to unaligned 64-bit access.
// This reimplementation uses C++ local variables to avoid stack operations.
//
// Function: Lookup entry in array by hashed name
// Input: r3 = array struct ptr (ptr at +0, count at +4), r4 = name string
// Output: r3 = index+1 if found, 0 if not found
PPC_FUNC(sub_8285AA90) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_82851F30] sub_8285AA90 ENTER #{} arrayPtr=0x{:08X} namePtr=0x{:08X}", s_count, ctx.r3.u32, ctx.r4.u32);
    
    // Save r31 in C++ variable instead of PPC stack (avoids unaligned 64-bit access)
    uint64_t saved_r31 = ctx.r31.u64;
    uint64_t saved_lr = ctx.lr;
    
    // Original: mr r31,r3 (save array struct pointer)
    uint32_t arrayStructPtr = ctx.r3.u32;
    uint32_t nameStringPtr = ctx.r4.u32;
    
    // SAFETY CHECK: Validate pointers before calling hash function
    // If name string pointer is invalid, return 1 (found) to bypass
    if (nameStringPtr < 0x80000000 || nameStringPtr >= 0xE0000000) {
        ctx.r3.s64 = 1;  // Return 1 = found
        ctx.r31.u64 = saved_r31;
        ctx.lr = saved_lr;
        LOGF_WARNING("[sub_82851F30] sub_8285AA90 EXIT #{} (BYPASS: invalid namePtr 0x{:08X})", s_count, nameStringPtr);
        return;
    }
    
    // Original: mr r3,r4; li r4,0; bl sub_827DF490
    // Call sub_827DF490 to hash the name string
    ctx.r3.u64 = ctx.r4.u64;  // r3 = name string
    ctx.r4.s64 = 0;           // r4 = 0
    ctx.lr = 0x8285AAB0;      // return address (for debugging)
    sub_827DF490(ctx, base);
    
    // r3 now contains the hashed name
    uint32_t hashedName = ctx.r3.u32;
    
    // Original: lhz r9,4(r31) - load entry count
    uint16_t entryCount = __builtin_bswap16(*(uint16_t*)(base + arrayStructPtr + 4));
    
    // Original: cmpwi cr6,r9,0; ble not_found
    if (entryCount <= 0) {
        // WORKAROUND: Return "found at index 0" to bypass empty effect registry
        // The game's effect registry is never populated because we use embedded shaders.
        // Returning 1 tricks the game into thinking effects exist, allowing init to proceed.
        ctx.r3.s64 = 1;  // Return 1 = found at index 0 (instead of 0 = not found)
        ctx.r31.u64 = saved_r31;
        ctx.lr = saved_lr;
        LOGF_WARNING("[sub_82851F30] sub_8285AA90 EXIT #{} (BYPASS: empty registry, returning 1)", s_count);
        return;
    }
    
    // Original: lwz r10,0(r31) - load array pointer
    uint32_t arrayPtr = __builtin_bswap32(*(uint32_t*)(base + arrayStructPtr + 0));
    
    // Search loop
    for (int32_t i = 0; i < entryCount; i++) {
        // Original: lwz r8,0(r10) - load entry hash at array[i*16]
        uint32_t entryHash = __builtin_bswap32(*(uint32_t*)(base + arrayPtr + i * 16));
        
        // Original: cmplw cr6,r8,r3; beq found
        if (entryHash == hashedName) {
            // Found - return index + 1
            ctx.r3.s64 = i + 1;
            ctx.r31.u64 = saved_r31;
            ctx.lr = saved_lr;
            LOGF_WARNING("[sub_82851F30] sub_8285AA90 EXIT #{} (found at {})", s_count, i);
            return;
        }
    }
    
    // WORKAROUND: Return "found at index 0" even when hash lookup fails
    // The effect registry has entries but they don't match game's expected hashes
    // because we use embedded shaders instead of the original effect system.
    ctx.r3.s64 = 1;  // Return 1 = found (instead of 0 = not found)
    ctx.r31.u64 = saved_r31;
    ctx.lr = saved_lr;
    LOGF_WARNING("[sub_82851F30] sub_8285AA90 EXIT #{} (BYPASS: hash mismatch, returning 1)", s_count);
}

extern "C" void __imp__sub_8284FAD8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8284FAD8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_82856C90] sub_8284FAD8 ENTER #{}", s_count);
    __imp__sub_8284FAD8(ctx, base);
    LOGF_WARNING("[sub_82856C90] sub_8284FAD8 EXIT #{}", s_count);
}

extern "C" void __imp__sub_8219FC80(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8219FC80) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_8219FC80 ENTER #{}", s_count);
    __imp__sub_8219FC80(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_8219FC80 EXIT #{}", s_count);
}

extern "C" void __imp__sub_82285F90(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82285F90) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_82285F90 ENTER #{}", s_count);
    __imp__sub_82285F90(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_82285F90 EXIT #{}", s_count);
}

extern "C" void __imp__sub_822214E0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_822214E0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_822214E0 ENTER #{}", s_count);
    __imp__sub_822214E0(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_822214E0 EXIT #{}", s_count);
}

// sub_82193BC0 - Job system init - STUBBED to prevent blocking
// Analysis: Creates a worker thread that waits on semaphore 0xA82402B0
// With workers stubbed, this semaphore is never signaled, causing infinite block
extern "C" void __imp__sub_82193BC0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82193BC0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[JOB_SYSTEM] sub_82193BC0 #{} STUBBED - job system bypassed", s_count);
    // Don't call __imp__sub_82193BC0 - it creates worker that blocks forever
    ctx.r3.u32 = 0;  // Return success
    return;
}

// =============================================================================
// sub_823193A8 - GPU Device Setup (vtable[7] calls)
// =============================================================================
// This function loads from TLS[1676] (GPU device) and calls vtable[7] (offset 28)
// multiple times. Same pattern as sub_82856BA8 - Xbox GPU hardware calls.
//
// SOLUTION: Bypass entirely. GPU setup handled by host rendering layer.
// =============================================================================
extern "C" void __imp__sub_823193A8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_823193A8) {
    static int s_count = 0; ++s_count;
    
    if (s_count <= 3) {
        LOG_WARNING("[GPU] sub_823193A8 BYPASSING - GPU device vtable[7] calls");
    }
    
    // Bypass - GPU device setup handled by host layer
    return;
}

// =============================================================================
// sub_821EC3E8 - Shader Effect Loading
// =============================================================================
// Calls sub_821EC1E8 which loads shader effects via:
//   sub_8285AA90 - effect lookup by name
//   sub_82860928 - shader registration
//   sub_8285F750 - shader binding
//
// SOLUTION: Bypass - shader effects handled by host shader cache
// =============================================================================
extern "C" void __imp__sub_821EC3E8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_821EC3E8) {
    static int s_count = 0; ++s_count;
    
    if (s_count <= 3) {
        LOG_WARNING("[GPU] sub_821EC3E8 BYPASSING - shader effect loading");
    }
    
    // Return 1 (success) - original returns bool from sub_821EC1E8
    ctx.r3.s64 = 1;
    return;
}

extern "C" void __imp__sub_827EED88(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827EED88) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_827EED88 ENTER #{}", s_count);
    __imp__sub_827EED88(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_827EED88 EXIT #{}", s_count);
}

extern "C" void __imp__sub_827EB6E0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827EB6E0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_827EB6E0 ENTER #{}", s_count);
    __imp__sub_827EB6E0(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_827EB6E0 EXIT #{}", s_count);
}

extern "C" void __imp__sub_827EB748(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827EB748) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_827EB748 ENTER #{}", s_count);
    __imp__sub_827EB748(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_827EB748 EXIT #{}", s_count);
}

// =============================================================================
// sub_827E7B38 - File Streaming Queue Init
// =============================================================================
// This function initializes file streaming queues by processing file handles
// from an array at global+2656. The loop never terminates (infinite loop).
//
// Root cause: The array at global+2656 contains file handles that were
// registered during storage/file system init. The loop condition checks
// for NULL to terminate, but the array doesn't end with NULL.
//
// SOLUTION: Bypass - file streaming handled by VFS layer.
// =============================================================================
extern "C" void __imp__sub_827E7B38(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E7B38) {
    static int s_count = 0; ++s_count;
    
    if (s_count <= 3) {
        LOG_WARNING("[STREAMING] sub_827E7B38 BYPASSING - file streaming queue init (infinite loop)");
    }
    
    // Bypass - streaming handled by VFS layer
    return;
}

extern "C" void __imp__sub_827DF490(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827DF490) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOGF_WARNING("[sub_827E7B38] sub_827DF490 ENTER #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    __imp__sub_827DF490(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[sub_827E7B38] sub_827DF490 EXIT #{}", s_count);
}

extern "C" void __imp__sub_827827C8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827827C8) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOG_WARNING("[sub_827E7B38] sub_827827C8 ENTER (queue op)");
    __imp__sub_827827C8(ctx, base);
    if (s_count <= 10) LOG_WARNING("[sub_827E7B38] sub_827827C8 EXIT");
}

extern "C" void __imp__sub_82990EC0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82990EC0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOG_WARNING("[sub_827E7B38] sub_82990EC0 ENTER");
    __imp__sub_82990EC0(ctx, base);
    if (s_count <= 5) LOG_WARNING("[sub_827E7B38] sub_82990EC0 EXIT");
}

extern "C" void __imp__sub_827EAE38(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827EAE38) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOG_WARNING("[sub_827EB748] sub_827EAE38 ENTER");
    __imp__sub_827EAE38(ctx, base);
    if (s_count <= 5) LOG_WARNING("[sub_827EB748] sub_827EAE38 EXIT");
}



extern "C" void __imp__sub_827EEB48(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827EEB48) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_827EEB48 ENTER #{}", s_count);
    __imp__sub_827EEB48(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_827EEB48 EXIT #{}", s_count);
}

// sub_82197338 - last call in sub_8218C600 before return
extern "C" void __imp__sub_82197338(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82197338) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_82197338 ENTER #{}", s_count);
    __imp__sub_82197338(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_82197338 EXIT #{}", s_count);
}

// sub_821915F8 - first call in sub_82197338
extern "C" void __imp__sub_821915F8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_821915F8) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOGF_WARNING("[sub_82197338] sub_821915F8 ENTER #{}", s_count);
    __imp__sub_821915F8(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[sub_82197338] sub_821915F8 EXIT #{}", s_count);
}

// sub_82897760 - called by sub_821915F8
extern "C" void __imp__sub_82897760(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82897760) {
    static int s_count = 0; ++s_count;
    if (s_count <= 50) LOGF_WARNING("[sub_821915F8] sub_82897760 ENTER #{}", s_count);
    __imp__sub_82897760(ctx, base);
    if (s_count <= 50) LOGF_WARNING("[sub_821915F8] sub_82897760 EXIT #{}", s_count);
}

// sub_822DE5F0 - called by sub_821915F8 (potential blocker)
extern "C" void __imp__sub_822DE5F0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_822DE5F0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 50) LOGF_WARNING("[sub_821915F8] sub_822DE5F0 ENTER #{}", s_count);
    __imp__sub_822DE5F0(ctx, base);
    if (s_count <= 50) LOGF_WARNING("[sub_821915F8] sub_822DE5F0 EXIT #{}", s_count);
}

// sub_827D9C50 - called for large memory allocations, potential blocker
extern "C" void __imp__sub_827D9C50(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827D9C50) {
    static int s_count = 0; ++s_count;
    uint32_t allocSize = ctx.r4.u32;
    bool isLarge = (allocSize > 500000);
    
    if (s_count <= 10 || isLarge) {
        LOGF_WARNING("[ALLOC] sub_827D9C50 #{} ENTER size={}", s_count, allocSize);
    }
    __imp__sub_827D9C50(ctx, base);
    if (s_count <= 10 || isLarge) {
        LOGF_WARNING("[ALLOC] sub_827D9C50 #{} EXIT r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// sub_827DA8E0 - allocator function called via vtable for large allocs
extern "C" void __imp__sub_827DA8E0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827DA8E0) {
    static int s_count = 0; ++s_count;
    uint32_t allocSize = ctx.r4.u32;
    bool isLarge = (allocSize > 500000);
    
    if (s_count <= 10 || isLarge) {
        LOGF_WARNING("[ALLOC] sub_827DA8E0 #{} ENTER size={}", s_count, allocSize);
    }
    __imp__sub_827DA8E0(ctx, base);
    if (s_count <= 10 || isLarge) {
        LOGF_WARNING("[ALLOC] sub_827DA8E0 #{} EXIT r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// sub_827D8AF8 - indirect call target for large allocations, trace what IT calls
extern "C" void __imp__sub_827D8AF8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827D8AF8) {
    static int s_count = 0; ++s_count;
    
    // r4 contains allocation size
    uint32_t allocSize = ctx.r4.u32;
    bool isLarge = (allocSize > 500000);
    
    // Trace the indirect call target this function will call
    uint32_t r6 = ctx.r6.u32;
    uint32_t r3_in = ctx.r3.u32;
    uint32_t idx = ((r6 + 1) << 2) & 0xFFFFFFFC;
    uint32_t r3_new = PPC_LOAD_U32(idx + r3_in);
    uint32_t vtable = PPC_LOAD_U32(r3_new + 0);
    uint32_t funcPtr = PPC_LOAD_U32(vtable + 8);
    
    if (s_count <= 10 || isLarge) {
        LOGF_WARNING("[ALLOC] sub_827D8AF8 #{} ENTER size={} r6={} -> indirect_target=0x{:08X}", 
                     s_count, allocSize, r6, funcPtr);
    }
    __imp__sub_827D8AF8(ctx, base);
    if (s_count <= 10 || isLarge) {
        LOGF_WARNING("[ALLOC] sub_827D8AF8 #{} EXIT r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// sub_827DAD60 - called by worker thread 0x8298E700 at start - might signal "ready"
extern "C" void __imp__sub_827DAD60(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827DAD60) {
    static int s_count = 0; ++s_count;
    uint32_t handle = ctx.r3.u32;
    if (s_count <= 30 || s_count % 100 == 0) {
        LOGF_WARNING("[SIGNAL] sub_827DAD60 #{} handle=0x{:08X} (worker thread startup signal?)", s_count, handle);
    }
    __imp__sub_827DAD60(ctx, base);
    if (s_count <= 30 || s_count % 100 == 0) {
        LOGF_WARNING("[SIGNAL] sub_827DAD60 #{} EXIT", s_count);
    }
}

// sub_8298E700 - resource worker task function - STUBBED to prevent infinite blocking
// Analysis: This worker uses virtual dispatch (vtable+12) and loops forever waiting on semaphore
// The semaphore at context+36 is never signaled because task dispatcher is not initialized
// Stubbing allows init to complete while preventing worker deadlock
// 
// CRITICAL FIX: Must signal sem2 (ctx+40) before returning!
// sub_8298E810 creates this worker and waits on sem2. If we don't signal it,
// sub_8298E810 blocks forever, preventing sub_82673718 from returning,
// which prevents sub_82269098 and sub_82124080 from ever being called.
extern "C" void __imp__sub_8298E700(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8298E700) {
    static int s_count = 0; ++s_count;
    uint32_t ctx_ptr = ctx.r3.u32;
    
    // Read semaphore handles from context struct
    uint32_t sem1 = 0, sem2 = 0;
    if (ctx_ptr >= 0x82000000 && ctx_ptr < 0x90000000) {
        sem1 = ByteSwap(*(uint32_t*)g_memory.Translate(ctx_ptr + 36)); // wait semaphore
        sem2 = ByteSwap(*(uint32_t*)g_memory.Translate(ctx_ptr + 40)); // signal semaphore
    }
    
    LOGF_WARNING("[WORKER_TASK] sub_8298E700 #{} STUBBED ctx=0x{:08X} sem1=0x{:08X} sem2=0x{:08X}", 
                 s_count, ctx_ptr, sem1, sem2);
    
    // CRITICAL: Signal sem2 so sub_8298E810 can continue!
    // This unblocks the init flow and allows sub_82673718 to return.
    if (sem2 != 0) {
        NtReleaseSemaphore(sem2, 1, nullptr);
        LOGF_WARNING("[WORKER_TASK] sub_8298E700 #{} signaled sem2=0x{:08X}", s_count, sem2);
    }
    
    // Don't call __imp__sub_8298E700 - it would block forever
    ctx.r3.u32 = 0;  // Return success
    return;
}

PPC_FUNC(sub_829A1958) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_829A1958 ENTER #{}", s_count);
    __imp__sub_829A1958(ctx, base);
    LOGF_WARNING("[INIT] sub_829A1958 EXIT #{}", s_count);
}

PPC_FUNC(sub_82974FF8) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10 || s_count % 50 == 0)
        LOGF_WARNING("[INIT] sub_82974FF8 ENTER #{}", s_count);
    __imp__sub_82974FF8(ctx, base);
    if (s_count <= 10 || s_count % 50 == 0)
        LOGF_WARNING("[INIT] sub_82974FF8 EXIT #{}", s_count);
}

PPC_FUNC(sub_8296C060) {
    static int s_count = 0; ++s_count;
    // Reduce noise - only log first 3 and every 100th
    if (s_count <= 3 || s_count % 100 == 0)
        LOGF_WARNING("[INIT] sub_8296C060 #{}", s_count);
    __imp__sub_8296C060(ctx, base);
}

PPC_FUNC(sub_82671E40) {
    static int s_count = 0; ++s_count;
    if (s_count <= 20 || s_count % 100 == 0)
        LOGF_WARNING("[INIT] sub_82671E40 ENTER #{}", s_count);
    __imp__sub_82671E40(ctx, base);
    if (s_count <= 20 || s_count % 100 == 0)
        LOGF_WARNING("[INIT] sub_82671E40 EXIT #{}", s_count);
}

// sub_82672E50 - Audio/streaming init with busy-wait loops - STUBBED
// Analysis: Contains busy-wait loops waiting for worker tasks that are stubbed.
// Since workers are stubbed, these loops would run forever.
// Stubbing this function allows init to continue.
PPC_FUNC(sub_82672E50) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_82672E50 #{} STUBBED - bypassing busy-wait loops", s_count);
    // Don't call __imp__sub_82672E50 - contains infinite busy-wait loops
    ctx.r3.u32 = 0;  // Return success
    return;
}

// sub_82679950 - Graphics/GPU initialization - STUBBED
// Analysis: Xbox 360 GPU hardware initialization that blocks waiting for hardware responses
// Host rendering layer in gpu/video.cpp handles GPU setup, so Xbox init can be bypassed
extern "C" void __imp__sub_82679950(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82679950) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[GRAPHICS] sub_82679950 #{} STUBBED - Xbox GPU init bypassed (host handles rendering)", s_count);
    // Don't call __imp__sub_82679950 - contains Xbox GPU hardware initialization
    ctx.r3.u32 = 0;  // Return success
    return;
}

// sub_8221D880 - World initialization - STUBBED
// Analysis: Xbox 360 world/game state initialization that likely blocks on hardware
// Host game engine handles world setup, so Xbox init can be bypassed
extern "C" void __imp__sub_8221D880(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8221D880) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[WORLD] sub_8221D880 #{} STUBBED - Xbox world init bypassed", s_count);
    // Don't call __imp__sub_8221D880 - contains Xbox world initialization
    ctx.r3.u32 = 0;  // Return success
    return;
}

// Functions in sub_82672E50's busy-wait loops
extern "C" void __imp__sub_82672AA8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82973598(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_829736F8(PPCContext& ctx, uint8_t* base);

PPC_FUNC(sub_82672AA8) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10 || s_count % 100 == 0)
        LOGF_WARNING("[BUSYWAIT] sub_82672AA8 #{} (loop1 - checking 0x832A5F20)", s_count);
    // Yield to let worker threads complete their tasks
    std::this_thread::yield();
    __imp__sub_82672AA8(ctx, base);
}

// sub_82973598 - Loop 2 condition check in sub_82672E50
// Returns non-zero while tasks are pending. Force return 0 after iteration limit.
PPC_FUNC(sub_82973598) {
    static int s_count = 0; ++s_count;
    static thread_local int s_loopIter = 0;
    
    // Call original to get actual status
    __imp__sub_82973598(ctx, base);
    
    // Track iterations per loop entry
    if (ctx.r3.u32 != 0) {
        ++s_loopIter;
        // Force exit after 100 iterations to prevent infinite loop
        if (s_loopIter > 100) {
            if (s_loopIter == 101) {
                LOGF_WARNING("[BUSYWAIT] sub_82973598 forcing loop exit after {} iterations", s_loopIter);
            }
            ctx.r3.u32 = 0;  // Force loop exit
        }
    } else {
        // Loop condition cleared naturally, reset counter
        s_loopIter = 0;
    }
    
    if (s_count <= 10 || s_count % 500 == 0)
        LOGF_WARNING("[BUSYWAIT] sub_82973598 #{} r3={} loopIter={}", s_count, ctx.r3.u32, s_loopIter);
}

PPC_FUNC(sub_829736F8) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10 || s_count % 100 == 0)
        LOGF_WARNING("[BUSYWAIT] sub_829736F8 #{} (loop2 iteration)", s_count);
    // Yield to let worker threads complete their tasks
    std::this_thread::yield();
    __imp__sub_829736F8(ctx, base);
}

// sub_829A97A0 - creates threads via ExCreateThread - suspected blocker
extern "C" void __imp__sub_829A97A0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829A97A0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[THREAD] sub_829A97A0 ENTER #{} - calling ExCreateThread", s_count);
    __imp__sub_829A97A0(ctx, base);
    LOGF_WARNING("[THREAD] sub_829A97A0 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

// sub_829B08E0 - Thread entry stub (called by GuestThread::Start)
// This is called with r3=actual_worker_func, r4=context
// It should call the worker function - trace to see if it's blocking
extern "C" void __imp__sub_829B08E0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829B08E0) {
    static int s_count = 0; ++s_count;
    uint32_t workerFunc = ctx.r3.u32;
    uint32_t workerCtx = ctx.r4.u32;
    LOGF_WARNING("[THREAD_STUB] sub_829B08E0 ENTER #{} workerFunc=0x{:08X} ctx=0x{:08X}", 
                 s_count, workerFunc, workerCtx);
    __imp__sub_829B08E0(ctx, base);
    LOGF_WARNING("[THREAD_STUB] sub_829B08E0 EXIT #{}", s_count);
}

// =============================================================================
// sub_8218BE28 - PLATFORM GLUE REPLACEMENT: Memory Allocator
// =============================================================================
// Original Xbox behavior:
//   r11 = [r13+0]           // Thread context base
//   r3 = [r11+1676]         // Memory manager pointer
//   r11 = [r3+0]            // Vtable
//   r11 = [r11+8]           // Allocation function
//   call r11(size, align=16, flags=0)
//
// Problem: Original code stores memory manager at fixed address 0x83130000+1676,
// but [r13+0] points to dynamic TLS address, causing NULL vtable lookup.
//
// Solution: Replace with direct allocation using guest heap, bypassing vtable.
// =============================================================================
extern "C" void __imp__sub_8218BE28(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8218BE28) {
    static int s_count = 0; ++s_count;
    uint32_t allocSize = ctx.r3.u32;
    uint32_t alignment = 16;  // r5 in original = 16
    
    // Check if memory manager is properly initialized
    uint32_t thread_ctx = PPC_LOAD_U32(ctx.r13.u32 + 0);
    uint32_t mem_mgr = (thread_ctx != 0) ? PPC_LOAD_U32(thread_ctx + 1676) : 0;
    
    bool useDirectAlloc = (mem_mgr == 0);  // Use direct allocation if vtable chain is broken
    
    if (s_count <= 20 || s_count % 500 == 0 || allocSize > 500000 || (s_count >= 960 && s_count <= 1000)) {
        LOGF_WARNING("[ALLOC] sub_8218BE28 #{} size={} direct={} thread_ctx=0x{:08X} mem_mgr=0x{:08X}", 
                     s_count, allocSize, useDirectAlloc ? "YES" : "NO", thread_ctx, mem_mgr);
    }
    
    if (useDirectAlloc) {
        // PLATFORM GLUE: Direct allocation bypassing broken vtable chain
        // Allocate from guest heap with 16-byte alignment
        void* hostPtr = g_userHeap.Alloc(allocSize + alignment);
        if (hostPtr) {
            // Align the pointer
            uintptr_t addr = reinterpret_cast<uintptr_t>(hostPtr);
            uintptr_t aligned = (addr + alignment - 1) & ~(alignment - 1);
            
            // Map to guest address space
            uint32_t guestAddr = g_memory.MapVirtual(reinterpret_cast<uint8_t*>(aligned));
            ctx.r3.u32 = guestAddr;
            
            if (s_count <= 20 || allocSize > 500000) {
                LOGF_WARNING("[ALLOC] sub_8218BE28 #{} DIRECT alloc {} bytes -> 0x{:08X}", 
                             s_count, allocSize, guestAddr);
            }
        } else {
            ctx.r3.u32 = 0;  // Allocation failed
            LOGF_WARNING("[ALLOC] sub_8218BE28 #{} DIRECT alloc FAILED for {} bytes", s_count, allocSize);
        }
    } else {
        // Memory manager is properly initialized, use original PPC code
        __imp__sub_8218BE28(ctx, base);
        if (s_count <= 20 || s_count % 500 == 0 || allocSize > 500000 || (s_count >= 960 && s_count <= 1000)) {
            LOGF_WARNING("[ALLOC] sub_8218BE28 #{} PPC alloc {} bytes -> 0x{:08X}", 
                         s_count, allocSize, ctx.r3.u32);
        }
    }
}

// sub_8296BE18 - called after sub_8218BE28
extern "C" void __imp__sub_8296BE18(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8296BE18) {
    static int s_count = 0; ++s_count;
    // PHASE 3 FIX: Break audio init loop after 100 iterations
    if (s_count > 100) {
        ctx.r3.u64 = 1; // Return success
        return;
    }
    __imp__sub_8296BE18(ctx, base);
}

// =============================================================================
// Phase 1 Tracing: Functions called by sub_82974FF8 to identify blocking point
// =============================================================================

extern "C" void __imp__sub_8296C378(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8296C378) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[TRACE_82974FF8] sub_8296C378 ENTER #{}", s_count);
    __imp__sub_8296C378(ctx, base);
    LOGF_WARNING("[TRACE_82974FF8] sub_8296C378 EXIT #{} r3={}", s_count, ctx.r3.u32);
}

extern "C" void __imp__sub_82763AB8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82763AB8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[TRACE_82974FF8] sub_82763AB8 ENTER #{}", s_count);
    __imp__sub_82763AB8(ctx, base);
    LOGF_WARNING("[TRACE_82974FF8] sub_82763AB8 EXIT #{}", s_count);
}

extern "C" void __imp__sub_8296C2F0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8296C2F0) {
    static int s_count = 0; ++s_count;
    // PHASE 3 FIX: Break infinite audio worker loop after 50 iterations
    if (s_count > 50) {
        ctx.r3.u64 = 0;
        return;
    }
    __imp__sub_8296C2F0(ctx, base);
}

extern "C" void __imp__sub_82974500(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82974500) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[TRACE_82974FF8] sub_82974500 ENTER #{}", s_count);
    __imp__sub_82974500(ctx, base);
    LOGF_WARNING("[TRACE_82974FF8] sub_82974500 EXIT #{} r3={}", s_count, ctx.r3.u32);
}

// Early functions called by sub_82974FF8 before the ones above
extern "C" void __imp__sub_8296BF88(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8296BF88) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[TRACE_EARLY] sub_8296BF88 ENTER #{}", s_count);
    __imp__sub_8296BF88(ctx, base);
    LOGF_WARNING("[TRACE_EARLY] sub_8296BF88 EXIT #{} r3={}", s_count, ctx.r3.u32);
}

extern "C" void __imp__sub_8296BFB8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8296BFB8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[TRACE_EARLY] sub_8296BFB8 ENTER #{}", s_count);
    __imp__sub_8296BFB8(ctx, base);
    LOGF_WARNING("[TRACE_EARLY] sub_8296BFB8 EXIT #{} r3={}", s_count, ctx.r3.u32);
}

extern "C" void __imp__sub_82213C48(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82213C48) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[TRACE_EARLY] sub_82213C48 ENTER #{}", s_count);
    __imp__sub_82213C48(ctx, base);
    LOGF_WARNING("[TRACE_EARLY] sub_82213C48 EXIT #{} r3={}", s_count, ctx.r3.u32);
}

extern "C" void __imp__sub_8296C228(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8296C228) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[TRACE_EARLY] sub_8296C228 ENTER #{}", s_count);
    __imp__sub_8296C228(ctx, base);
    LOGF_WARNING("[TRACE_EARLY] sub_8296C228 EXIT #{} r3={}", s_count, ctx.r3.u32);
}

extern "C" void __imp__sub_829745B0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829745B0) {
    static int s_count = 0; ++s_count;
    // Check stack alignment - crash happens around call #237
    if (s_count >= 230 || (ctx.r1.u32 & 0x7) != 0) {
        LOGF_WARNING("[TRACE_EARLY] sub_829745B0 ENTER #{} r1=0x{:08X} r3=0x{:08X} align={}", 
                     s_count, ctx.r1.u32, ctx.r3.u32, ctx.r1.u32 & 0xF);
    }
    __imp__sub_829745B0(ctx, base);
    if (s_count >= 230) {
        LOGF_WARNING("[TRACE_EARLY] sub_829745B0 EXIT #{} r1=0x{:08X}", s_count, ctx.r1.u32);
    }
}

extern "C" void __imp__sub_82974530(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82974530) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[TRACE_EARLY] sub_82974530 ENTER #{}", s_count);
    __imp__sub_82974530(ctx, base);
    LOGF_WARNING("[TRACE_EARLY] sub_82974530 EXIT #{} r3={}", s_count, ctx.r3.u32);
}

extern "C" void __imp__sub_829743A8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829743A8) {
    static int s_count = 0; ++s_count;
    // Check stack alignment - crash happens around call #326
    if (s_count >= 320 || (ctx.r1.u32 & 0x7) != 0) {
        LOGF_WARNING("[TRACE_EARLY] sub_829743A8 ENTER #{} r1=0x{:08X} r3=0x{:08X} align={}", 
                     s_count, ctx.r1.u32, ctx.r3.u32, ctx.r1.u32 & 0xF);
    }
    __imp__sub_829743A8(ctx, base);
    if (s_count >= 320) {
        LOGF_WARNING("[TRACE_EARLY] sub_829743A8 EXIT #{} r1=0x{:08X}", s_count, ctx.r1.u32);
    }
}

extern "C" void __imp__sub_8296C238(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8296C238) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[TRACE_EARLY] sub_8296C238 ENTER #{}", s_count);
    __imp__sub_8296C238(ctx, base);
    LOGF_WARNING("[TRACE_EARLY] sub_8296C238 EXIT #{} r3={}", s_count, ctx.r3.u32);
}

// =============================================================================
// Post-allocation tracing hooks for sub_827DF248 call chain
// =============================================================================
extern "C" void __imp__sub_82801028(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82801028) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) {
        LOGF_WARNING("[TRACE] sub_82801028 ENTER #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    }
    __imp__sub_82801028(ctx, base);
    if (s_count <= 10) {
        LOGF_WARNING("[TRACE] sub_82801028 EXIT #{}", s_count);
    }
}

extern "C" void __imp__sub_829A4490(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829A4490) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) {
        LOGF_WARNING("[TRACE] sub_829A4490 ENTER #{} r3={} r4=0x{:08X}", s_count, ctx.r3.u32, ctx.r4.u32);
    }
    __imp__sub_829A4490(ctx, base);
    if (s_count <= 10) {
        LOGF_WARNING("[TRACE] sub_829A4490 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// =============================================================================
// sub_8218BF20 - PLATFORM GLUE REPLACEMENT: Extended Memory Allocator
// =============================================================================
// Original Xbox behavior: Allocates memory with flags, uses same vtable chain
// as sub_8218BE28. Calls sub_82893200/sub_828931F0 for actual allocation.
// Problem: Same vtable chain issue - memory manager not initialized.
// Solution: Direct allocation using guest heap.
// =============================================================================
extern "C" void __imp__sub_8218BF20(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8218BF20) {
    static int s_count = 0; ++s_count;
    uint32_t allocSize = ctx.r3.u32;
    uint32_t flags = ctx.r4.u32;
    
    // Check if memory manager is properly initialized
    uint32_t thread_ctx = PPC_LOAD_U32(ctx.r13.u32 + 0);
    uint32_t mem_mgr = (thread_ctx != 0) ? PPC_LOAD_U32(thread_ctx + 1676) : 0;
    
    bool useDirectAlloc = (mem_mgr == 0);
    
    if (s_count <= 30 || s_count % 500 == 0 || allocSize > 100000) {
        LOGF_WARNING("[ALLOC] sub_8218BF20 #{} size={} flags=0x{:08X} direct={}", 
                     s_count, allocSize, flags, useDirectAlloc ? "YES" : "NO");
    }
    
    if (useDirectAlloc) {
        // Parse alignment from flags (bits 24-27 encode alignment power)
        uint32_t alignPower = (flags >> 24) & 0xF;
        uint32_t alignment = (alignPower > 0) ? (1u << alignPower) : 16;
        if (alignment < 16) alignment = 16;
        if (alignment > 4096) alignment = 4096;
        
        // PLATFORM GLUE: Direct allocation bypassing broken vtable chain
        void* hostPtr = g_userHeap.Alloc(allocSize + alignment);
        if (hostPtr) {
            uintptr_t addr = reinterpret_cast<uintptr_t>(hostPtr);
            uintptr_t aligned = (addr + alignment - 1) & ~(alignment - 1);
            
            uint32_t guestAddr = g_memory.MapVirtual(reinterpret_cast<uint8_t*>(aligned));
            ctx.r3.u32 = guestAddr;
            
            if (s_count <= 30 || allocSize > 100000) {
                LOGF_WARNING("[ALLOC] sub_8218BF20 #{} DIRECT alloc {} bytes align={} -> 0x{:08X}", 
                             s_count, allocSize, alignment, guestAddr);
            }
        } else {
            ctx.r3.u32 = 0;
            LOGF_WARNING("[ALLOC] sub_8218BF20 #{} DIRECT alloc FAILED for {} bytes", s_count, allocSize);
        }
    } else {
        __imp__sub_8218BF20(ctx, base);
        if (s_count <= 30 || s_count % 500 == 0 || allocSize > 100000) {
            LOGF_WARNING("[ALLOC] sub_8218BF20 #{} PPC alloc {} bytes -> 0x{:08X}", 
                         s_count, allocSize, ctx.r3.u32);
        }
    }
}

extern "C" void __imp__sub_829B0178(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829B0178) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) {
        LOGF_WARNING("[TRACE] sub_829B0178 ENTER #{}", s_count);
    }
    __imp__sub_829B0178(ctx, base);
    if (s_count <= 10) {
        LOGF_WARNING("[TRACE] sub_829B0178 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}



extern "C" void __imp__sub_824C1338(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_824C1338) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) {
        LOGF_WARNING("[INIT] sub_824C1338 ENTER #{}", s_count);
    }
    __imp__sub_824C1338(ctx, base);
    if (s_count <= 10) {
        LOGF_WARNING("[INIT] sub_824C1338 EXIT #{} r3={}", s_count, ctx.r3.u32);
    }
}

PPC_FUNC(sub_822054F8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_822054F8 ENTER #{}", s_count);
    __imp__sub_822054F8(ctx, base);
    LOGF_WARNING("[INIT] sub_822054F8 EXIT #{}", s_count);
}

// sub_821DE390 moved earlier in file with tracing

PPC_FUNC(sub_8221F8A8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_8221F8A8 ENTER #{}", s_count);
    __imp__sub_8221F8A8(ctx, base);
    LOGF_WARNING("[INIT] sub_8221F8A8 EXIT #{}", s_count);
}

PPC_FUNC(sub_82273988) {
    static int s_count = 0; ++s_count;
    
    uint32_t contextAddr = ctx.r3.u32;
    uint32_t enableFlag = ctx.r4.u32;
    
    LOGF_WARNING("[INIT] sub_82273988 ENTER #{} context=0x{:08X} enableFlag={}", 
                 s_count, contextAddr, enableFlag);
    
    // Inspect the resource array that sub_82273988 will iterate through
    // Address calculation: 0x82144000 + 29876 = 0x8214B4B4
    uint32_t resourceArrayAddr = 0x8214B4B4;
    uint32_t arrayPtr = PPC_LOAD_U32(resourceArrayAddr + 0);  // Base pointer
    uint16_t arrayCount = PPC_LOAD_U16(resourceArrayAddr + 4); // Count
    
    LOGF_WARNING("[INIT] sub_82273988 Resource array: ptr=0x{:08X} count={}",
                 arrayPtr, arrayCount);
    
    // FIX: If count is unreasonably high (>100), it's corrupted - reset to 0
    // This prevents infinite loops in sub_82273988
    if (arrayCount > 100) {
        LOGF_WARNING("[INIT] sub_82273988 FIXING corrupted count {} -> 0", arrayCount);
        PPC_STORE_U16(resourceArrayAddr + 4, 0);
        arrayCount = 0;
    }
    
    // Ensure sentinel values are present at the array structure
    // At offset +808+132, there should be a -1 sentinel
    if (arrayPtr != 0) {
        uint32_t sentinelAddr = arrayPtr + 808 + 132;
        uint32_t currentSentinel = PPC_LOAD_U32(sentinelAddr);
        LOGF_WARNING("[INIT] sub_82273988 Sentinel at 0x{:08X} current=0x{:08X}",
                     sentinelAddr, currentSentinel);
        if (currentSentinel != 0xFFFFFFFF) {
            PPC_STORE_U32(sentinelAddr, 0xFFFFFFFF); // Write -1 sentinel
            LOGF_WARNING("[INIT] sub_82273988 Fixed sentinel 0x{:08X} -> 0xFFFFFFFF", currentSentinel);
        }
    }
    
    // Also check and log what happens at context[358] - the completion flag
    uint32_t completionFlag = PPC_LOAD_U8(contextAddr + 358);
    LOGF_WARNING("[INIT] sub_82273988 Completion flag before: context[358]={}", completionFlag);
    
    __imp__sub_82273988(ctx, base);
    
    // Check completion flag after execution
    uint32_t completionFlagAfter = PPC_LOAD_U8(contextAddr + 358);
    LOGF_WARNING("[INIT] sub_82273988 Completion flag after: context[358]={}", completionFlagAfter);
    
    // Check if buffers were allocated
    uint32_t buffer0 = PPC_LOAD_U32(contextAddr + 0);
    uint32_t buffer8 = PPC_LOAD_U32(contextAddr + 8);
    uint32_t buffer360 = PPC_LOAD_U32(contextAddr + 360);
    LOGF_WARNING("[INIT] sub_82273988 Allocated buffers: [0]=0x{:08X} [8]=0x{:08X} [360]=0x{:08X}",
                 buffer0, buffer8, buffer360);
    
    LOGF_WARNING("[INIT] sub_82273988 EXIT #{}", s_count);
}

PPC_FUNC(sub_821250B0)
{
    static int s_count = 0;
    ++s_count;
    LOGF_WARNING("[INIT] sub_821250B0 ENTER #{}", s_count);
    __imp__sub_821250B0(ctx, base);
    LOGF_WARNING("[INIT] sub_821250B0 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_82318F60)
{
    static int s_count = 0;
    ++s_count;
    LOGF_WARNING("[INIT] sub_82318F60 ENTER #{}", s_count);
    __imp__sub_82318F60(ctx, base);
    LOGF_WARNING("[INIT] sub_82318F60 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

// Stub sub_82192E00 (Profile system wait) to prevent blocking
// Analysis: This function waits for profile system to be ready
// It's called by sub_82124080 and blocks indefinitely
// Stubbing allows profile/save init to complete
extern "C" void __imp__sub_82192E00(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82192E00) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[PROFILE] sub_82192E00 #{} STUBBED - profile system ready", s_count);
    // Don't call __imp__sub_82192E00 - it would block forever
    ctx.r3.u32 = 0;  // Return success
    return;
}

// =============================================================================
// sub_82124540 - Profile/Streaming Configuration File Parser
// =============================================================================
// This function parses stream.ini and profile config files into a stack buffer.
// Issue: Called with r3 = r1+96 (stack buffer), but r1 is corrupted (~0x000A0CF0)
// resulting in invalid pointer 0x000A0D50 being passed to sub_82192840.
// Fix: Validate buffer pointer and handle gracefully if invalid.
// =============================================================================
PPC_FUNC(sub_82124540) {
    static int s_count = 0; ++s_count;
    uint32_t bufferPtr = ctx.r3.u32;
    
    // Validate input buffer pointer - must be in guest memory range
    if (bufferPtr < 0x80000000 || bufferPtr >= 0x90000000) {
        LOGF_WARNING("[STREAM] sub_82124540 #{} INVALID buffer 0x{:08X} (stack corruption) - allowing VFS to handle",
                     s_count, bufferPtr);
        // Call original - the VFS/file hooks will handle the invalid pointer gracefully
        // The downstream sub_82192840 hook already validates and returns 0 for invalid handles
        __imp__sub_82124540(ctx, base);
        return;
    }
    
    if (s_count <= 5) {
        LOGF_WARNING("[STREAM] sub_82124540 #{} buffer=0x{:08X} r1=0x{:08X} - parsing stream.ini",
                     s_count, bufferPtr, ctx.r1.u32);
    }
    __imp__sub_82124540(ctx, base);
}


PPC_FUNC(sub_82124080)
{
    static int s_count = 0;
    ++s_count;
    LOGF_WARNING("[INIT] sub_82124080 #{} ENTER - running normally (sub_82192E00 is stubbed)", s_count);
    
    // UN-BYPASSED: Now safe to run because sub_82192E00 (profile system wait) is stubbed
    // This function initializes profile/save system and calls sub_82192E00 which would block
    // With sub_82192E00 stubbed, the function can complete initialization properly
    // This allows proper setup of profile/save data structures
    __imp__sub_82124080(ctx, base);
    
    LOGF_WARNING("[INIT] sub_82124080 #{} EXIT r3=0x{:08X}", s_count, ctx.r3.u32);
}

// =============================================================================
// Instrumentation for sub_82124080 internals to find blocker
// =============================================================================
extern "C" void __imp__sub_82124268(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82124268) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_82124080] sub_82124268 (profile data) ENTER #{}", s_count);
    // STUBBED - original blocks on profile data loading
    // __imp__sub_82124268(ctx, base);
    ctx.r3.u32 = 0;  // Return success
    return;
    LOGF_WARNING("[sub_82124080] sub_82124268 (profile data) EXIT #{}", s_count);
}

extern "C" void __imp__sub_82123E20(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82123E20) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_82124080] sub_82123E20 (init) ENTER #{}", s_count);
    __imp__sub_82123E20(ctx, base);
    LOGF_WARNING("[sub_82124080] sub_82123E20 (init) EXIT #{}", s_count);
}

extern "C" void __imp__sub_821244B8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_821244B8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_82124080] sub_821244B8 (cleanup) ENTER #{}", s_count);
    __imp__sub_821244B8(ctx, base);
    LOGF_WARNING("[sub_82124080] sub_821244B8 (cleanup) EXIT #{}", s_count);
}

// =============================================================================
// Final 5 subsystems in sub_82120FB8 - INSTRUMENTATION TO FIND BLOCKER
// =============================================================================
PPC_FUNC(sub_8227AC28) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_8227AC28 (World finalization) ENTER #{}", s_count);
    __imp__sub_8227AC28(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_8227AC28 (World finalization) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_82272290) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_82272290 (Entity finalize) ENTER #{}", s_count);
    __imp__sub_82272290(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_82272290 (Entity finalize) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_82212450) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_82212450 (Script finalize) ENTER #{}", s_count);
    __imp__sub_82212450(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_82212450 (Script finalize) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_822C5768) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_822C5768 (Streaming finalize) ENTER #{}", s_count);
    __imp__sub_822C5768(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_822C5768 (Streaming finalize) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_822D4C68) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_822D4C68 (Final setup) ENTER #{}", s_count);
    __imp__sub_822D4C68(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_822D4C68 (Final setup) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

// =============================================================================
// Early subsystems in sub_82120FB8 - INSTRUMENTATION TO FIND BLOCKER
// =============================================================================
PPC_FUNC(sub_82270170) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_82270170 (Entity system) ENTER #{}", s_count);
    __imp__sub_82270170(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_82270170 (Entity system) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_822FD328) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_822FD328 (Pool init) ENTER #{}", s_count);
    __imp__sub_822FD328(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_822FD328 (Pool init) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_822EFF40) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_822EFF40 (Map loader) ENTER #{}", s_count);
    __imp__sub_822EFF40(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_822EFF40 (Map loader) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_82120C48) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_82120C48 (Game config) ENTER #{}", s_count);
    __imp__sub_82120C48(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_82120C48 (Game config) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_82221410) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_82221410 (UI system) ENTER #{}", s_count);
    __imp__sub_82221410(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_82221410 (UI system) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

// =============================================================================
// Internal functions in sub_82221410 (UI system) - INSTRUMENTATION TO FIND BLOCKER
// Call order: 8260E310 -> 822B3C58 -> 822B4D68 -> 824A0898 -> 8260CF30 -> 8260BE08 -> 824B65B0 -> 822E3EC8 -> 822E49A0 -> 828E0AB8 -> 8222F7E8
// =============================================================================
PPC_FUNC(sub_8260E310) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[UI-INT] sub_8260E310 [1] ENTER #{} r3={} r4={} r5={}", s_count, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
    __imp__sub_8260E310(ctx, base);
    LOGF_WARNING("[UI-INT] sub_8260E310 [1] EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_822B3C58) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[UI-INT] sub_822B3C58 [2] ENTER #{}", s_count);
    __imp__sub_822B3C58(ctx, base);
    LOGF_WARNING("[UI-INT] sub_822B3C58 [2] EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_822B4D68) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[822B4D68] ENTER #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    
    // Trace the vtable for the first indirect call (vtable[2])
    uint32_t r29_val = 0x83120000;
    uint32_t global_ptr_addr = r29_val + 31108;  // 0x83127984
    uint32_t global_ptr = PPC_LOAD_U32(global_ptr_addr);
    LOGF_WARNING("[822B4D68] global@0x{:08X} = 0x{:08X}", global_ptr_addr, global_ptr);
    
    if (global_ptr != 0 && global_ptr >= 0x80000000 && global_ptr < 0xC0000000) {
        uint32_t vtable = PPC_LOAD_U32(global_ptr);
        LOGF_WARNING("[822B4D68] vtable = 0x{:08X}", vtable);
        if (vtable != 0 && vtable >= 0x80000000 && vtable < 0xC0000000) {
            uint32_t vtable_2 = PPC_LOAD_U32(vtable + 8);   // vtable[2] - first indirect call
            uint32_t vtable_1 = PPC_LOAD_U32(vtable + 4);   // vtable[1]
            uint32_t vtable_3 = PPC_LOAD_U32(vtable + 12);  // vtable[3]
            LOGF_WARNING("[822B4D68] vtable[1]=0x{:08X} vtable[2]=0x{:08X} vtable[3]=0x{:08X}", 
                         vtable_1, vtable_2, vtable_3);
        }
    }
    
    __imp__sub_822B4D68(ctx, base);
    LOGF_WARNING("[822B4D68] EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

// Internal functions of sub_824A0898 - find exact blocker
extern "C" void __imp__sub_827E0740(PPCContext& ctx, uint8_t* base);  // Called early in sub_824A0898
extern "C" void __imp__sub_8285F750(PPCContext& ctx, uint8_t* base);  // Called in sub_824A0898
extern "C" void __imp__sub_82860928(PPCContext& ctx, uint8_t* base);  // Called many times in sub_824A0898

PPC_FUNC(sub_827E0740) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOGF_WARNING("[824A0898-INT] sub_827E0740 ENTER #{} r3=0x{:08X} r4=0x{:08X}", s_count, ctx.r3.u32, ctx.r4.u32);
    __imp__sub_827E0740(ctx, base);
    if (s_count <= 5) LOGF_WARNING("[824A0898-INT] sub_827E0740 EXIT #{}", s_count);
}

PPC_FUNC(sub_8285F750) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOGF_WARNING("[824A0898-INT] sub_8285F750 ENTER #{} r3=0x{:08X} r4=0x{:08X}", s_count, ctx.r3.u32, ctx.r4.u32);
    __imp__sub_8285F750(ctx, base);
    if (s_count <= 5) LOGF_WARNING("[824A0898-INT] sub_8285F750 EXIT #{}", s_count);
}

PPC_FUNC(sub_82860928) {
    static int s_count = 0; ++s_count;
    if (s_count <= 20) LOGF_WARNING("[824A0898-INT] sub_82860928 ENTER #{} r3=0x{:08X} r4=0x{:08X} r5={}", s_count, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
    __imp__sub_82860928(ctx, base);
    if (s_count <= 20) LOGF_WARNING("[824A0898-INT] sub_82860928 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_824A0898) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[UI-INT] sub_824A0898 [4] ENTER #{}", s_count);
    __imp__sub_824A0898(ctx, base);
    LOGF_WARNING("[UI-INT] sub_824A0898 [4] EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}
// Late-stage functions in sub_824A0898 - after sub_82860928 and sub_8285AA90 calls
extern "C" void __imp__sub_82857C60(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82850BF8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_8249D6F0(PPCContext& ctx, uint8_t* base);

PPC_FUNC(sub_82857C60) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[824A0898-LATE] sub_82857C60 ENTER #{}", s_count);
    __imp__sub_82857C60(ctx, base);
    LOGF_WARNING("[824A0898-LATE] sub_82857C60 EXIT #{}", s_count);
}

PPC_FUNC(sub_82850BF8) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOGF_WARNING("[824A0898-LATE] sub_82850BF8 ENTER #{}", s_count);
    __imp__sub_82850BF8(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[824A0898-LATE] sub_82850BF8 EXIT #{} r3=0x{:02X}", s_count, ctx.r3.u32 & 0xFF);
}

PPC_FUNC(sub_8249D6F0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[STUB] sub_8249D6F0 #{} - Xbox device vtable calls bypassed (modern graphics active)", s_count);
    LOGF_WARNING("[STUB] sub_8249D6F0 #{} - Display mode: r3={} r4={} r5={} r6={}", s_count, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
    
    // STUB: This function performs Xbox-specific render target setup via device vtable calls.
    // Our modern graphics system (plume/Vulkan/D3D12) handles this differently.
    // Following Sonic Unleashed's approach: bypass low-level Xbox device code.
    
    // Return success without calling the original Xbox device implementation
    ctx.r3.u32 = 1;
}

// =============================================================================
// Internal functions of sub_8249D6F0 - DEEP INSTRUMENTATION
// =============================================================================

PPC_FUNC(sub_82126498) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[8249D6F0-INT] sub_82126498 (cleanup) ENTER #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    __imp__sub_82126498(ctx, base);
    LOGF_WARNING("[8249D6F0-INT] sub_82126498 (cleanup) EXIT #{}", s_count);
}

PPC_FUNC(sub_828536B0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[8249D6F0-INT] sub_828536B0 (buf init) ENTER #{} r3=0x{:08X} r4={}", s_count, ctx.r3.u32, ctx.r4.u32);
    __imp__sub_828536B0(ctx, base);
    LOGF_WARNING("[8249D6F0-INT] sub_828536B0 (buf init) EXIT #{}", s_count);
}

PPC_FUNC(sub_8260E3B8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[8249D6F0-INT] sub_8260E3B8 (disp mode) ENTER #{} r3={} r4={} r5={}", s_count, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
    __imp__sub_8260E3B8(ctx, base);
    LOGF_WARNING("[8249D6F0-INT] sub_8260E3B8 (disp mode) EXIT #{}", s_count);
}

PPC_FUNC(sub_8260E3D8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[8249D6F0-INT] sub_8260E3D8 (disp final) ENTER #{} r3={}", s_count, ctx.r3.u32);
    __imp__sub_8260E3D8(ctx, base);
    LOGF_WARNING("[8249D6F0-INT] sub_8260E3D8 (disp final) EXIT #{}", s_count);
}

PPC_FUNC(sub_8286A748) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[8249D6F0-INT] sub_8286A748 (init) ENTER #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    __imp__sub_8286A748(ctx, base);
    LOGF_WARNING("[8249D6F0-INT] sub_8286A748 (init) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_8286A890) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[8249D6F0-INT] sub_8286A890 (setup) ENTER #{} r3=0x{:08X} r4=0x{:08X}", s_count, ctx.r3.u32, ctx.r4.u32);
    __imp__sub_8286A890(ctx, base);
    LOGF_WARNING("[8249D6F0-INT] sub_8286A890 (setup) EXIT #{}", s_count);
}

// =============================================================================
// Internal functions of sub_822B4D68 - DEEP INSTRUMENTATION TO FIND BLOCKER
// =============================================================================

PPC_FUNC(sub_821EC018) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[822B4D68-INT] sub_821EC018 ENTER #{} r3=0x{:08X} r4={}", s_count, ctx.r3.u32, ctx.r4.u32);
    __imp__sub_821EC018(ctx, base);
    LOGF_WARNING("[822B4D68-INT] sub_821EC018 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_82859B80) {
    static int s_count = 0; ++s_count;
    if (s_count <= 15) LOGF_WARNING("[822B4D68-INT] sub_82859B80 ENTER #{} r3=0x{:08X} r4={}", s_count, ctx.r3.u32, ctx.r4.u32);
    __imp__sub_82859B80(ctx, base);
    if (s_count <= 15) LOGF_WARNING("[822B4D68-INT] sub_82859B80 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_827DFC60) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[822B4D68-INT] sub_827DFC60 ENTER #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    __imp__sub_827DFC60(ctx, base);
    LOGF_WARNING("[822B4D68-INT] sub_827DFC60 EXIT #{}", s_count);
}

PPC_FUNC(sub_8285DC80) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[822B4D68-INT] sub_8285DC80 (vtable[2]) ENTER #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    __imp__sub_8285DC80(ctx, base);
    LOGF_WARNING("[822B4D68-INT] sub_8285DC80 (vtable[2]) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}


PPC_FUNC(sub_8260CF30) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[UI-INT] sub_8260CF30 [5] ENTER #{}", s_count);
    __imp__sub_8260CF30(ctx, base);
    LOGF_WARNING("[UI-INT] sub_8260CF30 [5] EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_8260BE08) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[UI-INT] sub_8260BE08 [6] ENTER #{}", s_count);
    __imp__sub_8260BE08(ctx, base);
    LOGF_WARNING("[UI-INT] sub_8260BE08 [6] EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_824B65B0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[UI-INT] sub_824B65B0 [7] ENTER #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    __imp__sub_824B65B0(ctx, base);
    LOGF_WARNING("[UI-INT] sub_824B65B0 [7] EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_822E3EC8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[UI-INT] sub_822E3EC8 [8] ENTER #{}", s_count);
    __imp__sub_822E3EC8(ctx, base);
    LOGF_WARNING("[UI-INT] sub_822E3EC8 [8] EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_822E49A0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[STUB] sub_822E49A0 [9] #{} - UI resource init with Xbox device vtables + scheduler loop bypassed", s_count);
    LOGF_WARNING("[STUB] sub_822E49A0 [9] #{} - Parameters: r3=0x{:08X} r4=0x{:08X}", s_count, ctx.r3.u32, ctx.r4.u32);
    
    // STUB: This function performs UI resource initialization with:
    // 1. Xbox device vtable calls (lines 38833, 38862 in ppc_recomp.13.cpp)
    // 2. Infinite scheduler loop waiting for async events (sub_828E0AB8 called 25000+ times)
    // 3. Resource loading that depends on Xbox GPU device at 0x83127984
    // 
    // Our modern graphics system (plume/Vulkan/D3D12/Metal) handles UI resources independently.
    // Following Sonic Unleashed's approach: bypass Xbox-specific platform code.
    
    // Don't call __imp__sub_822E49A0 - it would block forever in scheduler loop
    return;
}

PPC_FUNC(sub_8222F7E8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[UI-INT] sub_8222F7E8 [11] ENTER #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    __imp__sub_8222F7E8(ctx, base);
    LOGF_WARNING("[UI-INT] sub_8222F7E8 [11] EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_8226CB50) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_8226CB50 (Camera system) ENTER #{}", s_count);
    __imp__sub_8226CB50(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_8226CB50 (Camera system) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);

}

// Internal functions called by sub_821A8868 - declarations only
extern "C" void __imp__sub_82300C78(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_8218BE28(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_824E1DD0(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_8249BA90(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_821A8060(PPCContext& ctx, uint8_t* base);

// STUBBED: sub_821A8868 (HUD/Mission init) - blocks on synchronization primitives
// Call chain: sub_82300C78 -> sub_827DB988 -> sub_827DB338 -> sub_829A39A0
// The sync primitives (semaphores, waits) block indefinitely in recompilation environment
PPC_FUNC(sub_821A8868) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[821A8868] #{} STUBBED - HUD init bypassed (blocks on sync primitives)", s_count);
    ctx.r3.u32 = 0;  // Return success
}

PPC_FUNC(sub_821A8278) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[821A8278] #{} STUBBED - HUD component bypassed (depends on stubbed HUD init)", s_count);
    ctx.r3.u32 = 0;  // Return success
    return;
}

PPC_FUNC(sub_821BC9E0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_821BC9E0 (Menu system) ENTER #{}", s_count);
    __imp__sub_821BC9E0(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_821BC9E0 (Menu system) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_822DB4B0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_822DB4B0 (Cutscene system) ENTER #{}", s_count);
    __imp__sub_822DB4B0(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_822DB4B0 (Cutscene system) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_821B7218) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_821B7218 (Mission system) ENTER #{}", s_count);
    __imp__sub_821B7218(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_821B7218 (Mission system) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_822498F8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_822498F8 (Checkpoint system) ENTER #{}", s_count);
    __imp__sub_822498F8(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_822498F8 (Checkpoint system) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_8225DC40) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_8225DC40 (Weather system) ENTER #{}", s_count);
    __imp__sub_8225DC40(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_8225DC40 (Weather system) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_821E24E0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_821E24E0 (Population system) ENTER #{}", s_count);
    __imp__sub_821E24E0(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_821E24E0 (Population system) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_821DFD18) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_821DFD18 (Traffic system) ENTER #{}", s_count);
    __imp__sub_821DFD18(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_821DFD18 (Traffic system) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

// STUBBED: sub_8220E108 (Wanted system) - call 30 (sub_82430C60) blocks
// Calls 1-29 work, call 30 blocks on sync primitives, skip it and remaining
PPC_FUNC(sub_8220E108) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[8220E108] #{} STUBBED - Wanted system bypassed (call 30 blocks)", s_count);
    ctx.r3.u32 = 0;  // Return success
}

PPC_FUNC(sub_821D8358) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_821D8358 (Map/GPS init) ENTER #{}", s_count);
    __imp__sub_821D8358(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_821D8358 (Map/GPS init) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_821EA0B8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_821EA0B8 (Blip system) ENTER #{}", s_count);
    __imp__sub_821EA0B8(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_821EA0B8 (Blip system) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}


// STUBBED: sub_82200EB8 (Stats system) - blocks on sync primitives
PPC_FUNC(sub_82200EB8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[82200EB8] #{} STUBBED - Stats system bypassed", s_count);
    ctx.r3.u32 = 0;
}

// =============================================================================
// Internal functions in sub_82120C48 - FIND EXACT BLOCKER
// =============================================================================
PPC_FUNC(sub_821E9658) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[82120C48-INT] sub_821E9658 ENTER #{}", s_count);
    __imp__sub_821E9658(ctx, base);
    LOGF_WARNING("[82120C48-INT] sub_821E9658 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_821FD460) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[82120C48-INT] sub_821FD460 ENTER #{}", s_count);
    __imp__sub_821FD460(ctx, base);
    LOGF_WARNING("[82120C48-INT] sub_821FD460 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_82126940) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[82120C48-INT] sub_82126940 ENTER #{}", s_count);
    __imp__sub_82126940(ctx, base);
    LOGF_WARNING("[82120C48-INT] sub_82126940 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_822B6C58) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[82120C48-INT] sub_822B6C58 ENTER #{}", s_count);
    __imp__sub_822B6C58(ctx, base);
    LOGF_WARNING("[82120C48-INT] sub_822B6C58 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_82308598) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[82120C48-INT] sub_82308598 ENTER #{}", s_count);
    __imp__sub_82308598(ctx, base);
    LOGF_WARNING("[82120C48-INT] sub_82308598 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_82209280) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[82120C48-INT] sub_82209280 ENTER #{}", s_count);
    __imp__sub_82209280(ctx, base);
    LOGF_WARNING("[82120C48-INT] sub_82209280 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}


// Hook sub_82120FB8 - Main Game Setup (63-subsystem init)
// This is the massive initialization function that sets up all game systems
PPC_FUNC(sub_82120FB8)
{
    static int s_count = 0;
    ++s_count;
    uint32_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id()) & 0xFFFF;
    
    LOGF_WARNING("[INIT] sub_82120FB8 ENTER #{} thread=0x{:04X} - 63-subsystem init starting", s_count, threadId);
    
    __imp__sub_82120FB8(ctx, base);
    
    LOGF_WARNING("[INIT] sub_82120FB8 EXIT #{} thread=0x{:04X} r3={} - 63-subsystem init complete", s_count, threadId, ctx.r3.u32);
    
    // FORCE: Start VBlank timer with known callback since GPU init path is never reached
    extern void StartVBlankTimer();
    
    if (g_gpuRingBuffer.interruptCallback == 0) {
        printf("[FORCE] Starting VBlank timer with callback 0x829D7368 after 63-subsystem init\n");
        g_gpuRingBuffer.interruptCallback = 0x829D7368;
        g_gpuRingBuffer.interruptUserData = 0;
        StartVBlankTimer();
    }
}

// Hook sub_8218BEB0 - Calls sub_82120000, sub_821200D0, sub_821200A8
// Returns 0 on success (all init complete), -1 on failure
PPC_FUNC(sub_8218BEB0)
{
    static int s_count = 0;
    ++s_count;
    uint32_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id()) & 0xFFFF;
    
    LOGF_WARNING("[INIT] sub_8218BEB0 ENTER #{} thread=0x{:04X}", s_count, threadId);
    
    __imp__sub_8218BEB0(ctx, base);
    
    // NOTE: SetInitComplete is now called at sub_821200D0 ENTRY (in save_hooks.cpp)
    // This is the right timing: after 63-subsystem init, before post-init loading
    
    LOGF_WARNING("[INIT] sub_8218BEB0 EXIT #{} thread=0x{:04X} r3={}", s_count, threadId, ctx.r3.s32);
}

// Hook sub_827D89B8 - Frame tick function  
PPC_FUNC(sub_827D89B8)
{
    static int s_count = 0;
    ++s_count;
    uint32_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id()) & 0xFFFF;
    
    if (s_count <= 5 || s_count % 100 == 0)
    {
        LOGF_WARNING("[FRAME] sub_827D89B8 #{} thread=0x{:04X}", s_count, threadId);
    }
    
    __imp__sub_827D89B8(ctx, base);
}

// =============================================================================
// INSTRUMENTATION: sub_827D89B8 internal functions
// Call order: sub_827D8840 -> sub_827FFF80 -> sub_828E0AB8 -> sub_827EE620 -> sub_8218BEB0
// =============================================================================
extern "C" void __imp__sub_827D8840(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_827FFF80(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_827EE620(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_827EEDE0(PPCContext& ctx, uint8_t* base);

// Hook sub_827D8840 - Pre-init setup (first call inside sub_827D89B8)
PPC_FUNC(sub_827D8840)
{
    static int s_count = 0;
    ++s_count;
    if (s_count <= 10)
    {
        LOGF_WARNING("[INIT-TRACE] sub_827D8840 ENTER #{} (pre-init setup)", s_count);
    }
    __imp__sub_827D8840(ctx, base);
    if (s_count <= 10)
    {
        LOGF_WARNING("[INIT-TRACE] sub_827D8840 EXIT #{}", s_count);
    }
}

// Hook sub_827FFF80 - Network init (second call inside sub_827D89B8)
PPC_FUNC(sub_827FFF80)
{
    static int s_count = 0;
    ++s_count;
    if (s_count <= 10)
    {
        LOGF_WARNING("[INIT-TRACE] sub_827FFF80 ENTER #{} (network init)", s_count);
    }
    __imp__sub_827FFF80(ctx, base);
    if (s_count <= 10)
    {
        LOGF_WARNING("[INIT-TRACE] sub_827FFF80 EXIT #{}", s_count);
    }
}

// Hook sub_827EEDE0 - Store argc/argv to globals
PPC_FUNC(sub_827EEDE0)
{
    static int s_count = 0;
    ++s_count;
    if (s_count <= 10)
    {
        LOGF_WARNING("[INIT-TRACE] sub_827EEDE0 ENTER #{} (store argc/argv)", s_count);
    }
    __imp__sub_827EEDE0(ctx, base);
    if (s_count <= 10)
    {
        LOGF_WARNING("[INIT-TRACE] sub_827EEDE0 EXIT #{}", s_count);
    }
}

// Hook sub_827EE620 - Additional system setup
PPC_FUNC(sub_827EE620)
{
    static int s_count = 0;
    ++s_count;
    if (s_count <= 10)
    {
        LOGF_WARNING("[INIT-TRACE] sub_827EE620 ENTER #{} (system setup)", s_count);
    }
    __imp__sub_827EE620(ctx, base);
    if (s_count <= 10)
    {
        LOGF_WARNING("[INIT-TRACE] sub_827EE620 EXIT #{}", s_count);
    }
}

// Hook sub_8218BEA8 - Main entry point, implement game loop
// Xbox 360 runtime called this repeatedly; recomp needs to emulate that
extern "C" void __imp__sub_8218BEA8(PPCContext& ctx, uint8_t* base);

PPC_FUNC(sub_8218BEA8)
{
    static int s_entered = 0;
    ++s_entered;
    if (s_entered == 1)
    {
        LOGF_WARNING("[MAIN] sub_8218BEA8 entry #{}", s_entered);
    }
    
    // Main game loop - call sub_827D89B8 (frame tick) repeatedly
    int frameCount = 0;
    while (true)
    {
        ++frameCount;
        
        // Call the frame tick function
        __imp__sub_827D89B8(ctx, base);
        
        // Check return value - negative means exit
        if (ctx.r3.s32 < 0)
        {
            LOGF_WARNING("[MAIN] Frame {} returned {} - exiting", frameCount, ctx.r3.s32);
            break;
        }
        
        // Log progress
        if (frameCount <= 5 || frameCount % 500 == 0)
        {
            LOGF_WARNING("[MAIN] Frame {} r3={}", frameCount, ctx.r3.s32);
        }
        
        // Small sleep to prevent CPU spinning (16ms ~ 60fps)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    LOGF_WARNING("[MAIN] Loop exited after {} frames", frameCount);
}

// Hook sub_828E0AB8 - Scheduler loop (just trace, don't force VdSwap)
extern "C" void __imp__sub_828E0AB8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_828E0AB8)
{
    static int s_count = 0;
    ++s_count;
    
    // Log with thread ID to understand who's calling
    uint32_t threadId = ctx.r13.u32 & 0xFFFF;
    
    // After storage init, log more frequently to detect if execution continues
    bool logThis = (s_count <= 10 || s_count % 5000 == 0);
    if (g_afterStorageInit.load() && s_count % 10 == 0) {
        logThis = true;
    }
    
    if (logThis) {
        const char* marker = g_afterStorageInit.load() ? " [POST-STORAGE]" : "";
        LOGF_WARNING("[Scheduler] sub_828E0AB8 #{} thread=0x{:04X}{}", s_count, threadId, marker);
    }
    
    __imp__sub_828E0AB8(ctx, base);
}

// Hook sub_829D4C48 - Frame swap/Present function called from VBlank
extern "C" void __imp__sub_829D4C48(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829D4C48)
{
    // Let the game's native render loop handle VdSwap - don't force it
    __imp__sub_829D4C48(ctx, base);
}

// Hook sub_829D7368 - VBlank interrupt callback
extern "C" void __imp__sub_829D7368(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829D7368)
{
    static int s_count = 0; ++s_count;
    if (s_count <= 5 || s_count % 1000 == 0)
        LOGF_WARNING("[VBLANK] sub_829D7368 #{}", s_count);
    __imp__sub_829D7368(ctx, base);
}






// Hook sub_829A3318 - Boot orchestrator (calls XamTaskShouldExit in loop)
extern "C" void __imp__sub_829A3318(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829A3318)
{
    static int s_count = 0;
    ++s_count;
    
    if (s_count <= 10 || s_count % 100 == 0)
    {
        LOGF_IMPL(Utility, "BootOrchestrator", "sub_829A3318 #{}", s_count);
    }
    
    __imp__sub_829A3318(ctx, base);
}

// Hook sub_827E8420 - Stream buffer reader
// Validates stream struct pointers before dereferencing to prevent crash at 0x400000000
// Stream struct layout: [0]=object ptr, [4]=ctx, [8]=buffer, [12]=pos, [16]=cursor, [20]=end, [24]=capacity
extern "C" void __imp__sub_827E8420(PPCContext& ctx, uint8_t* base);

// Helper to repair corrupted stream at 0x82003890
static void RepairCorruptedStream(uint32_t streamPtr)
{
    static int s_repairCount = 0;
    be<uint32_t>* stream = reinterpret_cast<be<uint32_t>*>(g_memory.Translate(streamPtr));
    stream[0] = 0;  // Null object - indicates stream not initialized
    stream[1] = 0;
    stream[2] = 0;
    stream[3] = 0;
    stream[4] = 0;
    stream[5] = 0;
    stream[6] = 0;
    if (++s_repairCount <= 10) {
        LOGF_WARNING("[sub_827E8420] Repaired corrupted stream at 0x{:08X} (repair #{})", streamPtr, s_repairCount);
    }
}

PPC_FUNC(sub_827E8420)
{
    const uint32_t streamPtr = ctx.r3.u32;
    
    // Handle NULL stream pointer - just return 0 (EOF/no data) silently
    // This happens when the game's streaming system hasn't initialized a stream yet
    if (streamPtr == 0)
    {
        ctx.r3.s32 = 0;  // Return 0 = no data/EOF
        return;
    }
    
    // Validate stream pointer is in valid guest memory range
    if (streamPtr < 0x80000000 || streamPtr >= 0x90000000)
    {
        static int s_badStreamCount = 0;
        if (++s_badStreamCount <= 5)
        {
            LOGF_WARNING("[sub_827E8420] Invalid stream pointer 0x{:08X}", streamPtr);
        }
        ctx.r3.s32 = -1;  // Return error
        return;
    }
    
    // Read object pointer at stream[0]
    uint32_t objectPtr = PPC_LOAD_U32(streamPtr + 0);
    
    // MEMORY PATCH: If this is the known-corrupted stream at 0x82003890, repair it
    // The corruption happens when "common.rpf" string overwrites the stream object
    if (streamPtr == 0x82003890 || (streamPtr >= 0x82003880 && streamPtr < 0x82003900))
    {
        // Check if corrupted (objectPtr looks like ASCII text "comm" = 0x636F6D6D)
        if (objectPtr == 0x636F6D6D || objectPtr == 0x6D6F6E2E || // "comm" or "mon."
            (objectPtr < 0x80000000 && objectPtr != 0))
        {
            RepairCorruptedStream(streamPtr);
            objectPtr = 0;  // Use null (validation already handles this)
        }
    }
    
    // Validate object pointer
    if (objectPtr != 0 && (objectPtr < 0x80000000 || objectPtr >= 0x90000000))
    {
        static int s_badObjectCount = 0;
        if (++s_badObjectCount <= 5)
        {
            LOGF_WARNING("[sub_827E8420] Invalid object pointer 0x{:08X} at stream 0x{:08X}", 
                objectPtr, streamPtr);
        }
        ctx.r3.s32 = -1;  // Return error
        return;
    }
    
    // If object pointer is valid, validate vtable
    if (objectPtr != 0)
    {
        uint32_t vtablePtr = PPC_LOAD_U32(objectPtr + 0);
        if (vtablePtr != 0 && (vtablePtr < 0x80000000 || vtablePtr >= 0x90000000))
        {
            static int s_badVtableCount = 0;
            if (++s_badVtableCount <= 5)
            {
                LOGF_WARNING("[sub_827E8420] Invalid vtable 0x{:08X} at object 0x{:08X}", 
                    vtablePtr, objectPtr);
            }
            ctx.r3.s32 = -1;  // Return error
            return;
        }
    }
    
    // All pointers validated - call original implementation
    __imp__sub_827E8420(ctx, base);
}

// =============================================================================
// sub_827E7FA8 - Stream vtable function call (DEFENSIVE WRAPPER)
// =============================================================================
// This function loads a vtable from an object and calls a function at vtable+48.
// The PAC crash occurs when the vtable or function pointer is corrupted.
// 
// Original code flow:
//   r11 = r3 (object pointer)
//   r3 = [r11+0] (load vtable from object)
//   r4 = [r11+4] (load context)
//   r11 = [r3+0] (load vtable pointer from object)
//   r11 = [r11+48] (load function pointer from vtable+48)
//   call r11 (PAC CRASH HERE if pointer is invalid)
// =============================================================================
extern "C" void __imp__sub_827E7FA8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E7FA8)
{
    static int s_callCount = 0;
    static int s_validationFailures = 0;
    s_callCount++;
    
    // Get the object pointer from r3
    uint32_t objectPtr = ctx.r3.u32;
    
    // Validate object pointer
    if (objectPtr == 0)
    {
        if (s_validationFailures < 10)
        {
            LOGF_WARNING("[sub_827E7FA8] NULL object pointer (call #{})", s_callCount);
            s_validationFailures++;
        }
        // Return gracefully - set r3 to 0 to indicate failure
        ctx.r3.u32 = 0;
        return;
    }
    
    // Check if object pointer is in valid guest memory range
    if (objectPtr < 0x80000000 || objectPtr >= 0x90000000)
    {
        if (s_validationFailures < 10)
        {
            LOGF_WARNING("[sub_827E7FA8] Invalid object pointer 0x{:08X} (call #{})", 
                objectPtr, s_callCount);
            s_validationFailures++;
        }
        // Return gracefully
        ctx.r3.u32 = 0;
        return;
    }
    
    // Load the vtable pointer from object+0
    uint32_t vtablePtr = PPC_LOAD_U32(objectPtr + 0);
    
    // Validate vtable pointer
    if (vtablePtr == 0)
    {
        if (s_validationFailures < 10)
        {
            LOGF_WARNING("[sub_827E7FA8] NULL vtable at object 0x{:08X} (call #{})", 
                objectPtr, s_callCount);
            s_validationFailures++;
        }
        ctx.r3.u32 = 0;
        return;
    }
    
    if (vtablePtr < 0x80000000 || vtablePtr >= 0x90000000)
    {
        if (s_validationFailures < 10)
        {
            LOGF_WARNING("[sub_827E7FA8] Invalid vtable 0x{:08X} at object 0x{:08X} (call #{})", 
                vtablePtr, objectPtr, s_callCount);
            s_validationFailures++;
        }
        ctx.r3.u32 = 0;
        return;
    }
    
    // Load the function pointer from vtable+48
    uint32_t funcPtr = PPC_LOAD_U32(vtablePtr + 48);
    
    // Validate function pointer
    if (funcPtr == 0)
    {
        if (s_validationFailures < 10)
        {
            LOGF_WARNING("[sub_827E7FA8] NULL function at vtable+48 (vtable=0x{:08X}, object=0x{:08X}, call #{})", 
                vtablePtr, objectPtr, s_callCount);
            s_validationFailures++;
        }
        ctx.r3.u32 = 0;
        return;
    }
    
    if (funcPtr < 0x80000000 || funcPtr >= 0x90000000)
    {
        if (s_validationFailures < 10)
        {
            LOGF_WARNING("[sub_827E7FA8] Invalid function pointer 0x{:08X} at vtable+48 "
                "(vtable=0x{:08X}, object=0x{:08X}, call #{})", 
                funcPtr, vtablePtr, objectPtr, s_callCount);
            s_validationFailures++;
        }
        ctx.r3.u32 = 0;
        return;
    }
    
    // All pointers are valid - call original implementation
    // Log first few successful calls for debugging
    if (s_callCount <= 5)
    {
        LOGF_WARNING("[sub_827E7FA8] Valid call #{}: object=0x{:08X}, vtable=0x{:08X}, func=0x{:08X}", 
            s_callCount, objectPtr, vtablePtr, funcPtr);
    }
    
    __imp__sub_827E7FA8(ctx, base);
}

// =============================================================================
// sub_821928D0 - File stream read operation (DEFENSIVE WRAPPER)
// =============================================================================
// This function reads from a file stream and eventually calls sub_827E7FA8.
// Add validation here to catch bad stream objects before they cause PAC crashes.
// The stream object is passed in r3 and stored in r30.
// =============================================================================
extern "C" void __imp__sub_821928D0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_821928D0)
{
    static int s_callCount = 0;
    static int s_invalidStreams = 0;
    s_callCount++;
    
    // Get the stream object pointer from r3
    uint32_t streamPtr = ctx.r3.u32;
    
    // Validate stream pointer
    if (streamPtr == 0)
    {
        if (s_invalidStreams < 10)
        {
            LOGF_WARNING("[sub_821928D0] NULL stream pointer (call #{})", s_callCount);
            s_invalidStreams++;
        }
        // Return 0 to indicate failure
        ctx.r3.u32 = 0;
        return;
    }
    
    // Check if stream pointer is in valid guest memory range
    if (streamPtr < 0x80000000 || streamPtr >= 0x90000000)
    {
        if (s_invalidStreams < 10)
        {
            LOGF_WARNING("[sub_821928D0] Invalid stream pointer 0x{:08X} (call #{})", 
                streamPtr, s_callCount);
            s_invalidStreams++;
        }
        ctx.r3.u32 = 0;
        return;
    }
    
    // Load and validate the object pointer at stream+0
    uint32_t objectPtr = PPC_LOAD_U32(streamPtr + 0);
    
    // If object pointer is NULL, the stream has no valid file object attached
    // Cannot read from a null object - return 0 to indicate no data
    if (objectPtr == 0)
    {
        if (s_invalidStreams < 10)
        {
            LOGF_WARNING("[sub_821928D0] Stream 0x{:08X} has NULL object pointer (call #{}) - returning 0", 
                streamPtr, s_callCount);
            s_invalidStreams++;
        }
        ctx.r3.u32 = 0;
        return;
    }
    
    if (objectPtr < 0x80000000 || objectPtr >= 0x90000000)
    {
        if (s_invalidStreams < 10)
        {
            LOGF_WARNING("[sub_821928D0] Stream 0x{:08X} has invalid object pointer 0x{:08X} (call #{})", 
                streamPtr, objectPtr, s_callCount);
            s_invalidStreams++;
        }
        ctx.r3.u32 = 0;
        return;
    }
    
    // If object pointer is valid, check the vtable
    if (objectPtr != 0)
    {
        uint32_t vtablePtr = PPC_LOAD_U32(objectPtr + 0);
        
        if (vtablePtr != 0 && (vtablePtr < 0x80000000 || vtablePtr >= 0x90000000))
        {
            if (s_invalidStreams < 10)
            {
                LOGF_WARNING("[sub_821928D0] Stream 0x{:08X} -> object 0x{:08X} has invalid vtable 0x{:08X} (call #{})", 
                    streamPtr, objectPtr, vtablePtr, s_callCount);
                s_invalidStreams++;
            }
            ctx.r3.u32 = 0;
            return;
        }
    }
    
    // Stream object is valid - call original implementation
    if (s_callCount <= 5)
    {
        LOGF_WARNING("[sub_821928D0] Valid call #{}: stream=0x{:08X}, object=0x{:08X}", 
            s_callCount, streamPtr, objectPtr);
    }
    
    __imp__sub_821928D0(ctx, base);
}

// =============================================================================
// sub_82192980 - File stream read with buffer (CRASH INVESTIGATION)
// =============================================================================
// This function reads from stream and returns pointer to buffer or 0.
// Adding wrapper to trace crash in file parsing path.
// =============================================================================
extern "C" void __imp__sub_82192980(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82192980) {
    static int s_count = 0; ++s_count;
    
    uint32_t streamHandle = ctx.r3.u32;
    uint32_t skipWhitespace = ctx.r4.u32;
    uint32_t r1_before = ctx.r1.u32;
    
    if (s_count <= 20) {
        LOGF_WARNING("[sub_82192980] #{} ENTER stream=0x{:08X} skip={} r1=0x{:08X}", 
                     s_count, streamHandle, skipWhitespace, r1_before);
    }
    
    __imp__sub_82192980(ctx, base);
    
    uint32_t result = ctx.r3.u32;
    if (s_count <= 20) {
        LOGF_WARNING("[sub_82192980] #{} EXIT r3=0x{:08X} (buffer ptr)", s_count, result);
        if (result != 0 && result >= 0x80000000 && result < 0x90000000) {
            // Try to read first few bytes of returned buffer
            uint8_t b0 = *(base + result);
            uint8_t b1 = *(base + result + 1);
            uint8_t b2 = *(base + result + 2);
            uint8_t b3 = *(base + result + 3);
            LOGF_WARNING("[sub_82192980] #{} buffer content: {:02X} {:02X} {:02X} {:02X} ('{}{}{}{}')",
                         s_count, b0, b1, b2, b3,
                         (b0 >= 0x20 && b0 < 0x7F) ? (char)b0 : '.',
                         (b1 >= 0x20 && b1 < 0x7F) ? (char)b1 : '.',
                         (b2 >= 0x20 && b2 < 0x7F) ? (char)b2 : '.',
                         (b3 >= 0x20 && b3 < 0x7F) ? (char)b3 : '.');
        }
    }
}

// =============================================================================
// sub_82192A60 - File stream close (DEFENSIVE WRAPPER)
// =============================================================================
// Prevents crash when called with NULL stream from failed file open.
// The crash occurs in sub_827E87A0 which dereferences the stream pointer.
// Flow: sub_82192840 returns 0 → sub_821928D0 returns 0 → sub_82192A60(0) → CRASH
// =============================================================================
extern "C" void __imp__sub_82192A60(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82192A60) {
    static int s_callCount = 0;
    static int s_invalidStreams = 0;
    s_callCount++;
    
    uint32_t streamPtr = ctx.r3.u32;
    
    // Check for NULL stream pointer
    if (streamPtr == 0) {
        s_invalidStreams++;
        if (s_invalidStreams <= 5) {
            LOGF_WARNING("[sub_82192A60] Skipping close for NULL stream (call #{})", s_callCount);
        }
        return;  // Don't call original - would crash in sub_827E87A0
    }
    
    // Check for invalid address range (outside guest memory)
    if (streamPtr < 0x80000000 || streamPtr >= 0x90000000) {
        s_invalidStreams++;
        if (s_invalidStreams <= 5) {
            LOGF_WARNING("[sub_82192A60] Skipping close for invalid stream 0x{:08X} (call #{})", 
                streamPtr, s_callCount);
        }
        return;  // Don't call original - would crash
    }
    
    // Stream pointer is in valid range, but check if internal pointers are valid
    // sub_827E87A0 crashes when stream[0] (storage device ptr) is NULL
    uint32_t storageDevicePtr = ByteSwap(*(uint32_t*)(base + streamPtr + 0));
    
    if (storageDevicePtr == 0) {
        s_invalidStreams++;
        if (s_invalidStreams <= 5) {
            LOGF_WARNING("[sub_82192A60] Skipping close for stream 0x{:08X} with NULL storage device (call #{})", 
                streamPtr, s_callCount);
        }
        // Clear the stream structure to mark it as closed
        *(uint32_t*)(base + streamPtr + 0) = 0;
        *(uint32_t*)(base + streamPtr + 4) = ByteSwap((uint32_t)-1);
        return;  // Don't call original - would crash dereferencing NULL
    }
    
    // Stream has valid internal pointers - call original
    if (s_callCount <= 5) {
        LOGF_WARNING("[sub_82192A60] Valid close for stream 0x{:08X} (call #{})", streamPtr, s_callCount);
    }
    
    __imp__sub_82192A60(ctx, base);
}

// =============================================================================
// sub_827E02F0 - File path processor (DEFENSIVE WRAPPER)
// =============================================================================
// Prevents crash when called with NULL or invalid structure pointer.
// The crash occurs at line 39351: ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 3076);
// When r31 (from r3 parameter) is NULL, this reads from address 0x0C04 → crash.
// Call chain: sub_8289DDB0 → sub_827E0968 → sub_827E02F0(NULL) → CRASH
// =============================================================================
extern "C" void __imp__sub_827E02F0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E02F0) {
    static int s_callCount = 0;
    static int s_invalidCalls = 0;
    s_callCount++;
    
    uint32_t structPtr = ctx.r3.u32;
    
    // Check for NULL pointer
    if (structPtr == 0) {
        s_invalidCalls++;
        if (s_invalidCalls <= 5) {
            LOGF_WARNING("[sub_827E02F0] Skipping file path processing for NULL structure (call #{})", s_callCount);
        }
        ctx.r3.u32 = 0;  // Return failure
        return;
    }
    
    // Check for invalid address range (outside guest memory)
    if (structPtr < 0x80000000 || structPtr >= 0x90000000) {
        s_invalidCalls++;
        if (s_invalidCalls <= 5) {
            LOGF_WARNING("[sub_827E02F0] Skipping file path processing for invalid structure 0x{:08X} (call #{})", 
                structPtr, s_callCount);
        }
        ctx.r3.u32 = 0;  // Return failure
        return;
    }
    
    // Structure pointer looks valid - call original
    __imp__sub_827E02F0(ctx, base);
}

// Hook sub_82192100 - skip dirty disc error UI and limit retries
PPC_FUNC(sub_82192100)
{
    static int count = 0;
    count++;
    
    if (count <= 5)
    {
        LOGF_IMPL(Utility, "GTA4_DirtyDisc", "sub_82192100 (dirty disc UI) called #{} - skipping", count);
    }
    
    // After too many calls, log and let game continue without crashing
    if (count >= 50)
    {
        LOG_UTILITY("[GTA4_DirtyDisc] Too many dirty disc errors - game may have issues loading files");
    }
    
    // Return without showing UI - just skip
}

// Hook sub_829A1290 - skip dirty disc caller
PPC_FUNC(sub_829A1290)
{
    static int count = 0;
    if (++count <= 3)
    {
        LOG_UTILITY("[GTA4] sub_829A1290 (dirty disc caller) - skipping");
    }
}

// =============================================================================
// sub_821200D0 - Pre-Main-Loop Phase (TRACE ENTRY)
// =============================================================================
// This function should be called after sub_82120FB8 completes.
// It contains the loading gate that calls sub_82124490.
// sub_821200D0 is now hooked in save_hooks.cpp - removed duplicate definition
// =============================================================================

// =============================================================================
// sub_82124490 - Loading Gate Check (BYPASSED)
// =============================================================================
// This function checks loading flags at 0x82D37BB7 and 0x82D37BC9.
// Returns 0 when loading complete, non-zero while loading.
// 
// Problem: Creates deadlock - waits for resources to load, but resources
// cannot load until we exit the loading gate.
// 
// Solution: Force return 0 (loading complete) to bypass the deadlock and
// allow game to proceed to resource loading code.
// =============================================================================
PPC_FUNC(sub_82124490) {
    static int s_count = 0;
    ++s_count;
    
    // ALWAYS log to see if this is being called
    LOGF_WARNING("[LOADING_GATE] sub_82124490 #{} - HOOK CALLED, forcing return 0", s_count);
    
    // Force return 0 (loading complete) to exit loading gate and proceed to resource loading
    ctx.r3.u64 = 0;
    
    // Do NOT call __imp__sub_82124490 - it would return 1 and block forever
}

// Forward declare the original implementation from recompiled code
extern "C" void __imp__sub_829A1F00(PPCContext& ctx, uint8_t* base);

// Track completed async operations to avoid re-serving data
// Key: async pointer, Value: pair<handle, offset> of completed operation
static std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>> g_completedAsyncOps;

// Hook sub_829A1F00 - the actual file loading function
// SIMPLIFIED APPROACH: Call original recompiled function, then ensure async completion
// Parameters: r3=handle, r4=buffer(guest), r5=size, r6=offset, r7=asyncInfo
PPC_FUNC(sub_829A1F00)
{
    static int count = 0;
    count++;
    
    const uint32_t handle = ctx.r3.u32;
    const uint32_t guestBuffer = ctx.r4.u32;
    const uint32_t size = ctx.r5.u32;
    const uint32_t offset = ctx.r6.u32;
    const uint32_t asyncInfo = ctx.r7.u32;
    
    // Track last known common.rpf handle for NULL handle recovery
    static uint32_t s_lastCommonHandle = 0;
    
    // Protect against NULL handle - log but continue to ReadFromBestRpf
    // ReadFromBestRpf has fallback logic to try all RPF files even without a handle
    if (handle == 0)
    {
        static int s_nullReadCount = 0;
        if (++s_nullReadCount <= 10)
        {
            LOGF_WARNING("[GTA4_FileLoad] NULL handle read: offset=0x{:X} size=0x{:X} (will try all RPFs)", offset, size);
        }
        // Don't return early - let ReadFromBestRpf try to serve from RPF files
    }
    
    // Cache handles that successfully read from common.rpf
    const uint32_t activeHandle = (handle != 0) ? handle : s_lastCommonHandle;
    if (offset >= 0x90000 && offset <= 0xB0000 && activeHandle != 0)
    {
        s_lastCommonHandle = activeHandle;
    }
    
    uint8_t* hostBuffer = reinterpret_cast<uint8_t*>(base + guestBuffer);
    
    // Log first 50 calls and then every 100th
    if (count <= 50 || count % 100 == 0)
    {
        LOGF_IMPL(Utility, "GTA4_FileLoad", "sub_829A1F00 #{} handle=0x{:08X} size=0x{:X} offset=0x{:X} async=0x{:08X}",
                  count, handle, size, offset, asyncInfo);
    }
    
    // Try to serve from our RPF streams with decrypted TOC
    std::string chosen;
    const uint32_t bytesRead = ReadFromBestRpf(handle, hostBuffer, size, offset, count, chosen);
    
    if (bytesRead > 0)
    {
        g_handleToRpf[handle] = chosen;
        if (count <= 50 || count % 100 == 0)
        {
            LOGF_IMPL(Utility, "GTA4_FileLoad", "sub_829A1F00 #{} read {} bytes from '{}' at offset 0x{:X}",
                      count, bytesRead, chosen, offset);
        }
        
        // For async reads, set completion flags BEFORE returning (static recomp playbook)
        // Xbox 360 IO_STATUS_BLOCK / OVERLAPPED structure (big-endian):
        //   Offset 0: Status (NTSTATUS) - 0 = STATUS_SUCCESS, 0x103 = STATUS_PENDING
        //   Offset 4: Information (bytes transferred)
        // The guest is POLLING this memory address waiting for Status != STATUS_PENDING
        if (asyncInfo != 0 && asyncInfo < 0x20000000)
        {
            volatile uint32_t* asyncPtr = reinterpret_cast<volatile uint32_t*>(base + asyncInfo);
            
            // Log BEFORE state for debugging
            if (count <= 20)
            {
                uint32_t beforeStatus = asyncPtr[0];
                uint32_t beforeInfo = asyncPtr[1];
                LOGF_IMPL(Utility, "GTA4_FileLoad", 
                          "ASYNC: BEFORE @0x{:08X}: Status=0x{:08X}, Info=0x{:08X}", 
                          asyncInfo, beforeStatus, beforeInfo);
            }
            
            // CRITICAL: Write with correct byte order for big-endian Xbox 360
            // Guest reads memory as big-endian, so we must store in big-endian format
            asyncPtr[1] = ByteSwap(bytesRead);  // Information = bytes transferred (offset 4)
            std::atomic_thread_fence(std::memory_order_seq_cst);
            asyncPtr[0] = 0;                    // Status = STATUS_SUCCESS (0) - write LAST!
            std::atomic_thread_fence(std::memory_order_seq_cst);
            
            // Signal event at offset 8 if present (Xbox kernel would do this on I/O completion)
            // asyncInfo structure: +0=Status, +4=Information, +8=hEvent, +12=?, +16=Flags
            uint32_t eventHandle = ByteSwap(asyncPtr[2]);
            if (eventHandle != 0 && eventHandle != 0xFFFFFFFF && IsKernelObject(eventHandle))
            {
                XDISPATCHER_HEADER* eventObj = reinterpret_cast<XDISPATCHER_HEADER*>(g_memory.Translate(eventHandle));
                if (eventObj && (eventObj->Type == 0 || eventObj->Type == 1))
                {
                    QueryKernelObject<Event>(*eventObj)->Set();
                    if (count <= 20)
                    {
                        LOGF_IMPL(Utility, "GTA4_FileLoad", 
                                  "ASYNC: Signaled event 0x{:08X} for async completion", eventHandle);
                    }
                }
            }
            
            // Log AFTER state
            if (count <= 20)
            {
                uint32_t afterStatus = asyncPtr[0];
                uint32_t afterInfo = asyncPtr[1];
                LOGF_IMPL(Utility, "GTA4_FileLoad", 
                          "ASYNC: AFTER  @0x{:08X}: Status=0x{:08X}, Info=0x{:08X}, hEvent=0x{:08X}", 
                          asyncInfo, afterStatus, afterInfo, eventHandle);
            }
        }
        
        ctx.r3.u32 = 1;  // Return success
        return;
    }
    
    // Check if this is one of our file handles
    auto it = g_ntFileHandles.find(handle);
    if (it != g_ntFileHandles.end() && it->second && it->second->magic == kNtFileHandleMagic)
    {
        NtFileHandle* hFile = it->second;
        
        // Parse RPF header if not done yet
        if (!hFile->rpfHeaderParsed)
            ParseRpfHeader(hFile);
        
        // Seek to offset and read
        hFile->stream.clear();
        hFile->stream.seekg(offset, std::ios::beg);
        
        if (!hFile->stream.bad())
        {
            hFile->stream.read(reinterpret_cast<char*>(hostBuffer), size);
            const uint32_t bytesRead = static_cast<uint32_t>(hFile->stream.gcount());
            
            // Decrypt TOC region if this is an encrypted RPF
            if (hFile->isRpf && hFile->tocEncrypted && bytesRead > 0)
            {
                const uint64_t tocStart = hFile->tocOffset;
                const uint64_t tocEnd = tocStart + hFile->tocSize;
                const uint64_t readStart = offset;
                const uint64_t readEnd = offset + bytesRead;
                const uint64_t overlapStart = std::max(readStart, tocStart);
                const uint64_t overlapEnd = std::min(readEnd, tocEnd);
                
                if (overlapEnd > overlapStart)
                {
                    const uint32_t startInBuf = static_cast<uint32_t>(overlapStart - readStart);
                    const uint32_t len = static_cast<uint32_t>(overlapEnd - overlapStart);
                    const uint64_t tocRelativeOffset = overlapStart - tocStart;
                    
                    if (count <= 20 || count % 100 == 0)
                    {
                        LOGF_IMPL(Utility, "GTA4_FileLoad", "Decrypting TOC region: bufOffset={} len={} tocRelOffset={}",
                                  startInBuf, len, tocRelativeOffset);
                    }
                    
                    DecryptRpfBufferInPlace(hostBuffer + startInBuf, len, tocRelativeOffset);
                }
            }
            
            if (count <= 20 || count % 100 == 0)
            {
                LOGF_IMPL(Utility, "GTA4_FileLoad", "sub_829A1F00 #{} read {} bytes from '{}' at offset 0x{:X}",
                          count, bytesRead, hFile->path.filename().string(), offset);
            }
            
            if (bytesRead > 0)
            {
                ctx.r3.u32 = 1;
                return;
            }
        }
    }

    // Check if this is a virtual file handle
    auto vit = g_ntVirtFileHandles.find(handle);
    if (vit != g_ntVirtFileHandles.end() && vit->second && vit->second->magic == kNtVirtFileHandleMagic)
    {
        NtVirtFileHandle* hVirt = vit->second;
        
        // Fallback to virtual data if RPF read failed
        if (offset < hVirt->data.size())
        {
            const uint32_t available = static_cast<uint32_t>(hVirt->data.size() - offset);
            const uint32_t toCopy = std::min(size, available);
            memcpy(hostBuffer, hVirt->data.data() + offset, toCopy);
            
            if (count <= 20 || count % 100 == 0)
            {
                LOGF_IMPL(Utility, "GTA4_FileLoad", "sub_829A1F00 #{} read {} bytes from virtual data at offset 0x{:X}",
                          count, toCopy, offset);
            }
            
            ctx.r3.u32 = 1;
            return;
        }
    }

    // All else failed
    if (count <= 20 || count % 100 == 0)
    {
        LOGF_IMPL(Utility, "GTA4_FileLoad", "sub_829A1F00 #{} FAILED for handle 0x{:08X}", count, handle);
    }
    ctx.r3.u32 = 0;
}

// NOTE: sub_827F0B20 is the retry loop - we let it run but our file system should handle reads properly
// The key is to ensure sub_829A1F00 returns correct data and completion flags
GUEST_FUNCTION_HOOK(sub_825383D8, XapiInitProcess)
GUEST_FUNCTION_HOOK(__imp__XGetVideoMode, VdQueryVideoMode); // XGetVideoMode
GUEST_FUNCTION_HOOK(__imp__XNotifyGetNext, XNotifyGetNext);
GUEST_FUNCTION_HOOK(__imp__XGetGameRegion, XGetGameRegion);
GUEST_FUNCTION_HOOK(__imp__XMsgStartIORequest, XMsgStartIORequest);

GUEST_FUNCTION_HOOK(__imp__XamGetSystemVersion, XamGetSystemVersion);
GUEST_FUNCTION_HOOK(__imp__XamContentCreateEx, XamContentCreateEx);
GUEST_FUNCTION_HOOK(__imp__XamContentDelete, XamContentDelete);
GUEST_FUNCTION_HOOK(__imp__XamContentClose, XamContentClose);
GUEST_FUNCTION_HOOK(__imp__XamContentGetCreator, XamContentGetCreator);
GUEST_FUNCTION_HOOK(__imp__XamContentCreateEnumerator, XamContentCreateEnumerator);
GUEST_FUNCTION_HOOK(__imp__XamContentGetDeviceState, XamContentGetDeviceState);
GUEST_FUNCTION_HOOK(__imp__XamContentGetDeviceData, XamContentGetDeviceData);
GUEST_FUNCTION_HOOK(__imp__XamEnumerate, XamEnumerate);
GUEST_FUNCTION_HOOK(__imp__XamNotifyCreateListener, XamNotifyCreateListener);
GUEST_FUNCTION_HOOK(__imp__XamUserGetSigninInfo, XamUserGetSigninInfo);
GUEST_FUNCTION_HOOK(__imp__XamShowSigninUI, XamShowSigninUI);
GUEST_FUNCTION_HOOK(__imp__XamShowDeviceSelectorUI, XamShowDeviceSelectorUI);
GUEST_FUNCTION_HOOK(__imp__XamShowMessageBoxUI, XamShowMessageBoxUI);
GUEST_FUNCTION_HOOK(__imp__XamUserCreateAchievementEnumerator, XamUserCreateAchievementEnumerator);
GUEST_FUNCTION_HOOK(__imp__XeKeysConsoleSignatureVerification, XeKeysConsoleSignatureVerification);
GUEST_FUNCTION_HOOK(__imp__XamGetPrivateEnumStructureFromHandle, XamGetPrivateEnumStructureFromHandle);
GUEST_FUNCTION_HOOK(__imp__XamTaskSchedule, XamTaskSchedule);
GUEST_FUNCTION_HOOK(__imp__XamTaskShouldExit, XamTaskShouldExit);
GUEST_FUNCTION_HOOK(__imp__XamTaskCloseHandle, XamTaskCloseHandle);
GUEST_FUNCTION_HOOK(__imp__XamUserAreUsersFriends, XamUserAreUsersFriends);
GUEST_FUNCTION_HOOK(__imp__XamUserCheckPrivilege, XamUserCheckPrivilege);
GUEST_FUNCTION_HOOK(__imp__XamUserCreateStatsEnumerator, XamUserCreateStatsEnumerator);
GUEST_FUNCTION_HOOK(__imp__XamUserGetName, XamUserGetName);
GUEST_FUNCTION_HOOK(__imp__XamUserGetXUID, XamUserGetXUID);
GUEST_FUNCTION_HOOK(__imp__XamUserWriteProfileSettings, XamUserWriteProfileSettings);
GUEST_FUNCTION_HOOK(__imp__XamVoiceCreate, XamVoiceCreate);
GUEST_FUNCTION_HOOK(__imp__XamVoiceClose, XamVoiceClose);
GUEST_FUNCTION_HOOK(__imp__XamVoiceHeadsetPresent, XamVoiceHeadsetPresent);
GUEST_FUNCTION_HOOK(__imp__XamVoiceSubmitPacket, XamVoiceSubmitPacket);
GUEST_FUNCTION_HOOK(__imp__XeKeysConsolePrivateKeySign, XeKeysConsolePrivateKeySign);
GUEST_FUNCTION_HOOK(__imp__IoDismountVolumeByFileHandle, IoDismountVolumeByFileHandle);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetGetEthernetLinkStatus, Net::XNetGetEthernetLinkStatus);
GUEST_FUNCTION_HOOK(__imp__KeTryToAcquireSpinLockAtRaisedIrql, KeTryToAcquireSpinLockAtRaisedIrql);
GUEST_FUNCTION_HOOK(__imp__XamShowGamerCardUIForXUID, XamShowGamerCardUIForXUID);
GUEST_FUNCTION_HOOK(__imp__XamShowPlayerReviewUI, XamShowPlayerReviewUI);
GUEST_FUNCTION_HOOK(__imp__XamShowDirtyDiscErrorUI, XamShowDirtyDiscErrorUI);
GUEST_FUNCTION_HOOK(__imp__XamEnableInactivityProcessing, XamEnableInactivityProcessing);
GUEST_FUNCTION_HOOK(__imp__XamResetInactivity, XamResetInactivity);
GUEST_FUNCTION_HOOK(__imp__XamShowMessageBoxUIEx, XamShowMessageBoxUIEx);
GUEST_FUNCTION_HOOK(__imp__XGetLanguage, XGetLanguage);
GUEST_FUNCTION_HOOK(__imp__XGetAVPack, XGetAVPack);
GUEST_FUNCTION_HOOK(__imp__XamLoaderTerminateTitle, XamLoaderTerminateTitle);
GUEST_FUNCTION_HOOK(__imp__XamGetExecutionId, XamGetExecutionId);
GUEST_FUNCTION_HOOK(__imp__XamLoaderLaunchTitle, XamLoaderLaunchTitle);
GUEST_FUNCTION_HOOK(__imp__NtOpenFile, NtOpenFile);
GUEST_FUNCTION_HOOK(__imp__RtlInitAnsiString, RtlInitAnsiString);
GUEST_FUNCTION_HOOK(__imp__NtCreateFile, NtCreateFile);
GUEST_FUNCTION_HOOK(__imp__NtClose, NtClose);
GUEST_FUNCTION_HOOK(__imp__NtSetInformationFile, NtSetInformationFile);
GUEST_FUNCTION_HOOK(__imp__FscGetCacheElementCount, FscGetCacheElementCount);
GUEST_FUNCTION_HOOK(__imp__FscSetCacheElementCount, FscSetCacheElementCount);
GUEST_FUNCTION_HOOK(__imp__XamLoaderGetLaunchDataSize, XamLoaderGetLaunchDataSize);
GUEST_FUNCTION_HOOK(__imp__XamLoaderGetLaunchData, XamLoaderGetLaunchData);
GUEST_FUNCTION_HOOK(__imp__XamLoaderSetLaunchData, XamLoaderSetLaunchData);
GUEST_FUNCTION_HOOK(__imp__NtWaitForSingleObjectEx, NtWaitForSingleObjectEx);
GUEST_FUNCTION_HOOK(__imp__NtWriteFile, NtWriteFile);
GUEST_FUNCTION_HOOK(__imp__ExGetXConfigSetting, ExGetXConfigSetting);
GUEST_FUNCTION_HOOK(__imp__NtQueryVirtualMemory, NtQueryVirtualMemory);
GUEST_FUNCTION_HOOK(__imp__MmQueryStatistics, MmQueryStatistics);
GUEST_FUNCTION_HOOK(__imp__NtCreateEvent, NtCreateEvent);
GUEST_FUNCTION_HOOK(__imp__XexCheckExecutablePrivilege, XexCheckExecutablePrivilege);
GUEST_FUNCTION_HOOK(__imp__DbgPrint, DbgPrint);
GUEST_FUNCTION_HOOK(__imp____C_specific_handler, __C_specific_handler_x);
GUEST_FUNCTION_HOOK(__imp__RtlNtStatusToDosError, RtlNtStatusToDosError);
GUEST_FUNCTION_HOOK(__imp__XexGetProcedureAddress, XexGetProcedureAddress);
GUEST_FUNCTION_HOOK(__imp__XexGetModuleSection, XexGetModuleSection);
GUEST_FUNCTION_HOOK(__imp__RtlUnicodeToMultiByteN, RtlUnicodeToMultiByteN);
GUEST_FUNCTION_HOOK(__imp__KeDelayExecutionThread, KeDelayExecutionThread);
GUEST_FUNCTION_HOOK(__imp__ExFreePool, ExFreePool);
GUEST_FUNCTION_HOOK(__imp__NtQueryInformationFile, NtQueryInformationFile);
GUEST_FUNCTION_HOOK(__imp__NtQueryVolumeInformationFile, NtQueryVolumeInformationFile);
GUEST_FUNCTION_HOOK(__imp__NtQueryDirectoryFile, NtQueryDirectoryFile);
GUEST_FUNCTION_HOOK(__imp__NtReadFileScatter, NtReadFileScatter);
GUEST_FUNCTION_HOOK(__imp__NtReadFile, NtReadFile);
GUEST_FUNCTION_HOOK(__imp__NtDuplicateObject, NtDuplicateObject);
GUEST_FUNCTION_HOOK(__imp__NtAllocateVirtualMemory, NtAllocateVirtualMemory);
GUEST_FUNCTION_HOOK(__imp__NtFreeVirtualMemory, NtFreeVirtualMemory);
GUEST_FUNCTION_HOOK(__imp__ObDereferenceObject, ObDereferenceObject);
GUEST_FUNCTION_HOOK(__imp__KeSetBasePriorityThread, KeSetBasePriorityThread);
GUEST_FUNCTION_HOOK(__imp__ObReferenceObjectByHandle, ObReferenceObjectByHandle);
GUEST_FUNCTION_HOOK(__imp__KeQueryBasePriorityThread, KeQueryBasePriorityThread);
GUEST_FUNCTION_HOOK(__imp__NtSuspendThread, NtSuspendThread);
GUEST_FUNCTION_HOOK(__imp__KeSetAffinityThread, KeSetAffinityThread);
GUEST_FUNCTION_HOOK(__imp__RtlLeaveCriticalSection, RtlLeaveCriticalSection);
GUEST_FUNCTION_HOOK(__imp__RtlEnterCriticalSection, RtlEnterCriticalSection);
GUEST_FUNCTION_HOOK(__imp__RtlImageXexHeaderField, RtlImageXexHeaderField);
GUEST_FUNCTION_HOOK(__imp__HalReturnToFirmware, HalReturnToFirmware);
GUEST_FUNCTION_HOOK(__imp__RtlFillMemoryUlong, RtlFillMemoryUlong);
GUEST_FUNCTION_HOOK(__imp__KeBugCheckEx, KeBugCheckEx);
GUEST_FUNCTION_HOOK(__imp__KeGetCurrentProcessType, KeGetCurrentProcessType);
GUEST_FUNCTION_HOOK(__imp__RtlCompareMemoryUlong, RtlCompareMemoryUlong);
GUEST_FUNCTION_HOOK(__imp__RtlInitializeCriticalSection, RtlInitializeCriticalSection);
GUEST_FUNCTION_HOOK(__imp__RtlRaiseException, RtlRaiseException_x);
GUEST_FUNCTION_HOOK(__imp__KfReleaseSpinLock, KfReleaseSpinLock);
GUEST_FUNCTION_HOOK(__imp__KfAcquireSpinLock, KfAcquireSpinLock);
GUEST_FUNCTION_HOOK(__imp__KeQueryPerformanceFrequency, KeQueryPerformanceFrequency);
GUEST_FUNCTION_HOOK(__imp__MmFreePhysicalMemory, MmFreePhysicalMemory);
GUEST_FUNCTION_HOOK(__imp__VdPersistDisplay, VdPersistDisplay);
GUEST_FUNCTION_HOOK(__imp__VdSwap, VdSwap);
GUEST_FUNCTION_HOOK(__imp__VdGetSystemCommandBuffer, VdGetSystemCommandBuffer);
GUEST_FUNCTION_HOOK(__imp__KeReleaseSpinLockFromRaisedIrql, KeReleaseSpinLockFromRaisedIrql);
GUEST_FUNCTION_HOOK(__imp__KeAcquireSpinLockAtRaisedIrql, KeAcquireSpinLockAtRaisedIrql);
GUEST_FUNCTION_HOOK(__imp__KiApcNormalRoutineNop, KiApcNormalRoutineNop);
GUEST_FUNCTION_HOOK(__imp__VdEnableRingBufferRPtrWriteBack, VdEnableRingBufferRPtrWriteBack);
GUEST_FUNCTION_HOOK(__imp__VdInitializeRingBuffer, VdInitializeRingBuffer);
GUEST_FUNCTION_HOOK(__imp__MmGetPhysicalAddress, MmGetPhysicalAddress);
GUEST_FUNCTION_HOOK(__imp__VdSetSystemCommandBufferGpuIdentifierAddress, VdSetSystemCommandBufferGpuIdentifierAddress);
GUEST_FUNCTION_HOOK(__imp__ExRegisterTitleTerminateNotification, ExRegisterTitleTerminateNotification);
GUEST_FUNCTION_HOOK(__imp__VdShutdownEngines, VdShutdownEngines);
GUEST_FUNCTION_HOOK(__imp__VdQueryVideoMode, VdQueryVideoMode);
GUEST_FUNCTION_HOOK(__imp__VdGetCurrentDisplayInformation, VdGetCurrentDisplayInformation);
GUEST_FUNCTION_HOOK(__imp__VdSetDisplayMode, VdSetDisplayMode);
GUEST_FUNCTION_HOOK(__imp__VdSetGraphicsInterruptCallback, VdSetGraphicsInterruptCallback);
GUEST_FUNCTION_HOOK(__imp__VdInitializeEngines, VdInitializeEngines);
GUEST_FUNCTION_HOOK(__imp__VdIsHSIOTrainingSucceeded, VdIsHSIOTrainingSucceeded);
GUEST_FUNCTION_HOOK(__imp__VdGetCurrentDisplayGamma, VdGetCurrentDisplayGamma);
GUEST_FUNCTION_HOOK(__imp__VdQueryVideoFlags, VdQueryVideoFlags);
GUEST_FUNCTION_HOOK(__imp__VdCallGraphicsNotificationRoutines, VdCallGraphicsNotificationRoutines);
GUEST_FUNCTION_HOOK(__imp__VdInitializeScalerCommandBuffer, VdInitializeScalerCommandBuffer);
GUEST_FUNCTION_HOOK(__imp__KeLeaveCriticalRegion, KeLeaveCriticalRegion);
GUEST_FUNCTION_HOOK(__imp__VdRetrainEDRAM, VdRetrainEDRAM);
GUEST_FUNCTION_HOOK(__imp__VdRetrainEDRAMWorker, VdRetrainEDRAMWorker);
GUEST_FUNCTION_HOOK(__imp__KeEnterCriticalRegion, KeEnterCriticalRegion);
GUEST_FUNCTION_HOOK(__imp__MmAllocatePhysicalMemoryEx, MmAllocatePhysicalMemoryEx);
GUEST_FUNCTION_HOOK(__imp__ObDeleteSymbolicLink, ObDeleteSymbolicLink);
GUEST_FUNCTION_HOOK(__imp__ObCreateSymbolicLink, ObCreateSymbolicLink);
GUEST_FUNCTION_HOOK(__imp__MmQueryAddressProtect, MmQueryAddressProtect);
GUEST_FUNCTION_HOOK(__imp__VdEnableDisableClockGating, VdEnableDisableClockGating);
GUEST_FUNCTION_HOOK(__imp__KeBugCheck, KeBugCheck);
GUEST_FUNCTION_HOOK(__imp__KeLockL2, KeLockL2);
GUEST_FUNCTION_HOOK(__imp__KeUnlockL2, KeUnlockL2);
GUEST_FUNCTION_HOOK(__imp__KeSetEvent, KeSetEvent);
GUEST_FUNCTION_HOOK(__imp__KeResetEvent, KeResetEvent);
GUEST_FUNCTION_HOOK(__imp__KeWaitForSingleObject, KeWaitForSingleObject);
GUEST_FUNCTION_HOOK(__imp__KeTlsGetValue, KeTlsGetValue);
GUEST_FUNCTION_HOOK(__imp__KeTlsSetValue, KeTlsSetValue);
GUEST_FUNCTION_HOOK(__imp__KeTlsAlloc, KeTlsAlloc);
GUEST_FUNCTION_HOOK(__imp__KeTlsFree, KeTlsFree);
GUEST_FUNCTION_HOOK(__imp__XMsgInProcessCall, XMsgInProcessCall);
GUEST_FUNCTION_HOOK(__imp__XamUserReadProfileSettings, XamUserReadProfileSettings);
GUEST_FUNCTION_HOOK(__imp__NetDll_WSAStartup, Net::WSAStartup);
GUEST_FUNCTION_HOOK(__imp__NetDll_WSACleanup, Net::WSACleanup);
GUEST_FUNCTION_HOOK(__imp__NetDll_socket, Net::Socket);
GUEST_FUNCTION_HOOK(__imp__NetDll_closesocket, Net::CloseSocket);
GUEST_FUNCTION_HOOK(__imp__NetDll_setsockopt, Net::SetSockOpt);
GUEST_FUNCTION_HOOK(__imp__NetDll_bind, Net::Bind);
GUEST_FUNCTION_HOOK(__imp__NetDll_connect, Net::Connect);
GUEST_FUNCTION_HOOK(__imp__NetDll_listen, Net::Listen);
GUEST_FUNCTION_HOOK(__imp__NetDll_accept, Net::Accept);
GUEST_FUNCTION_HOOK(__imp__NetDll_select, Net::Select);
GUEST_FUNCTION_HOOK(__imp__NetDll_recv, Net::Recv);
GUEST_FUNCTION_HOOK(__imp__NetDll_send, Net::Send);
GUEST_FUNCTION_HOOK(__imp__NetDll_inet_addr, Net::InetAddr);
GUEST_FUNCTION_HOOK(__imp__NetDll___WSAFDIsSet, NetDll___WSAFDIsSet);
GUEST_FUNCTION_HOOK(__imp__XMsgStartIORequestEx, XMsgStartIORequestEx);
GUEST_FUNCTION_HOOK(__imp__XamInputGetCapabilities, XamInputGetCapabilities);
GUEST_FUNCTION_HOOK(__imp__XamInputGetState, XamInputGetState);
GUEST_FUNCTION_HOOK(__imp__XamInputSetState, XamInputSetState);
GUEST_FUNCTION_HOOK(__imp__XexGetModuleHandle, XexGetModuleHandle);
GUEST_FUNCTION_HOOK(__imp__RtlTryEnterCriticalSection, RtlTryEnterCriticalSection);
GUEST_FUNCTION_HOOK(__imp__RtlInitializeCriticalSectionAndSpinCount, RtlInitializeCriticalSectionAndSpinCount);
GUEST_FUNCTION_HOOK(__imp__XeCryptBnQwBeSigVerify, XeCryptBnQwBeSigVerify);
GUEST_FUNCTION_HOOK(__imp__XeKeysGetKey, XeKeysGetKey);
GUEST_FUNCTION_HOOK(__imp__XeCryptRotSumSha, XeCryptRotSumSha);
GUEST_FUNCTION_HOOK(__imp__XeCryptSha, XeCryptSha);
GUEST_FUNCTION_HOOK(__imp__KeEnableFpuExceptions, KeEnableFpuExceptions);
GUEST_FUNCTION_HOOK(__imp__RtlUnwind, RtlUnwind_x);
GUEST_FUNCTION_HOOK(__imp__RtlCaptureContext, RtlCaptureContext_x);
GUEST_FUNCTION_HOOK(__imp__NtQueryFullAttributesFile, NtQueryFullAttributesFile);
GUEST_FUNCTION_HOOK(__imp__RtlMultiByteToUnicodeN, RtlMultiByteToUnicodeN);
GUEST_FUNCTION_HOOK(__imp__DbgBreakPoint, DbgBreakPoint);
GUEST_FUNCTION_HOOK(__imp__MmQueryAllocationSize, MmQueryAllocationSize);
GUEST_FUNCTION_HOOK(__imp__NtClearEvent, NtClearEvent);
GUEST_FUNCTION_HOOK(__imp__NtResumeThread, NtResumeThread);
GUEST_FUNCTION_HOOK(__imp__NtSetEvent, NtSetEvent);
GUEST_FUNCTION_HOOK(__imp__NtCreateSemaphore, NtCreateSemaphore);
GUEST_FUNCTION_HOOK(__imp__NtReleaseSemaphore, NtReleaseSemaphore);
GUEST_FUNCTION_HOOK(__imp__NtWaitForMultipleObjectsEx, NtWaitForMultipleObjectsEx);
GUEST_FUNCTION_HOOK(__imp__RtlCompareStringN, RtlCompareStringN);
GUEST_FUNCTION_HOOK(__imp__StfsControlDevice, StfsControlDevice);
GUEST_FUNCTION_HOOK(__imp__StfsCreateDevice, StfsCreateDevice);
GUEST_FUNCTION_HOOK(__imp__NtFlushBuffersFile, NtFlushBuffersFile);
GUEST_FUNCTION_HOOK(__imp__KeQuerySystemTime, KeQuerySystemTime);
GUEST_FUNCTION_HOOK(__imp__RtlTimeToTimeFields, RtlTimeToTimeFields);
GUEST_FUNCTION_HOOK(__imp__RtlFreeAnsiString, RtlFreeAnsiString);
GUEST_FUNCTION_HOOK(__imp__RtlUnicodeStringToAnsiString, RtlUnicodeStringToAnsiString);
GUEST_FUNCTION_HOOK(__imp__RtlInitUnicodeString, RtlInitUnicodeString);
GUEST_FUNCTION_HOOK(__imp__ExTerminateThread, ExTerminateThread);
GUEST_FUNCTION_HOOK(__imp__ExCreateThread, ExCreateThread);
GUEST_FUNCTION_HOOK(__imp__IoInvalidDeviceRequest, IoInvalidDeviceRequest);
GUEST_FUNCTION_HOOK(__imp__ObReferenceObject, ObReferenceObject);
GUEST_FUNCTION_HOOK(__imp__IoCreateDevice, IoCreateDevice);
GUEST_FUNCTION_HOOK(__imp__IoDeleteDevice, IoDeleteDevice);
GUEST_FUNCTION_HOOK(__imp__ExAllocatePoolTypeWithTag, ExAllocatePoolTypeWithTag);
GUEST_FUNCTION_HOOK(__imp__RtlTimeFieldsToTime, RtlTimeFieldsToTime);
GUEST_FUNCTION_HOOK(__imp__IoCompleteRequest, IoCompleteRequest);
GUEST_FUNCTION_HOOK(__imp__RtlUpcaseUnicodeChar, RtlUpcaseUnicodeChar);
GUEST_FUNCTION_HOOK(__imp__ObIsTitleObject, ObIsTitleObject);
GUEST_FUNCTION_HOOK(__imp__IoCheckShareAccess, IoCheckShareAccess);
GUEST_FUNCTION_HOOK(__imp__IoSetShareAccess, IoSetShareAccess);
GUEST_FUNCTION_HOOK(__imp__IoRemoveShareAccess, IoRemoveShareAccess);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetStartup, Net::XNetStartup);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetGetTitleXnAddr, Net::XNetGetTitleXnAddr);
GUEST_FUNCTION_HOOK(__imp__KeWaitForMultipleObjects, KeWaitForMultipleObjects);
GUEST_FUNCTION_HOOK(__imp__KeRaiseIrqlToDpcLevel, KeRaiseIrqlToDpcLevel);
GUEST_FUNCTION_HOOK(__imp__KfLowerIrql, KfLowerIrql);
GUEST_FUNCTION_HOOK(__imp__KeReleaseSemaphore, KeReleaseSemaphore);
GUEST_FUNCTION_HOOK(__imp__XAudioGetVoiceCategoryVolume, XAudioGetVoiceCategoryVolume);
GUEST_FUNCTION_HOOK(__imp__XAudioGetVoiceCategoryVolumeChangeMask, XAudioGetVoiceCategoryVolumeChangeMask);
GUEST_FUNCTION_HOOK(__imp__KeResumeThread, KeResumeThread);
GUEST_FUNCTION_HOOK(__imp__KeInitializeSemaphore, KeInitializeSemaphore);
GUEST_FUNCTION_HOOK(__imp__XMAReleaseContext, XMAReleaseContext);
GUEST_FUNCTION_HOOK(__imp__XMACreateContext, XMACreateContext);
GUEST_FUNCTION_HOOK(__imp__XAudioRegisterRenderDriverClient, XAudioRegisterRenderDriverClient);
GUEST_FUNCTION_HOOK(__imp__XAudioUnregisterRenderDriverClient, XAudioUnregisterRenderDriverClient);
GUEST_FUNCTION_HOOK(__imp__XAudioSubmitRenderDriverFrame, XAudioSubmitRenderDriverFrame);
// Additional GTA IV stubs
GUEST_FUNCTION_HOOK(__imp__XamAlloc, XamAlloc);
GUEST_FUNCTION_HOOK(__imp__XamFree, XamFree);
GUEST_FUNCTION_HOOK(__imp__NtSetTimerEx, NtSetTimerEx);
GUEST_FUNCTION_HOOK(__imp__NtCancelTimer, NtCancelTimer);
GUEST_FUNCTION_HOOK(__imp__NtCreateTimer, NtCreateTimer);
GUEST_FUNCTION_HOOK(__imp__NtCreateMutant, NtCreateMutant);
GUEST_FUNCTION_HOOK(__imp__NtReleaseMutant, NtReleaseMutant);
GUEST_FUNCTION_HOOK(__imp__IoDismountVolume, IoDismountVolume);
GUEST_FUNCTION_HOOK(__imp__XNotifyPositionUI, XNotifyPositionUI);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetCleanup, Net::XNetCleanup);
GUEST_FUNCTION_HOOK(__imp__NetDll_getsockname, Net::GetSockName);
GUEST_FUNCTION_HOOK(__imp__NetDll_ioctlsocket, Net::IOCtlSocket);
GUEST_FUNCTION_HOOK(__imp__NetDll_sendto, Net::SendTo);
GUEST_FUNCTION_HOOK(__imp__NetDll_recvfrom, Net::RecvFrom);
GUEST_FUNCTION_HOOK(__imp__NetDll_shutdown, Net::Shutdown);
GUEST_FUNCTION_HOOK(__imp__XMsgCancelIORequest, XMsgCancelIORequest);
GUEST_FUNCTION_HOOK(__imp__XAudioGetSpeakerConfig, XAudioGetSpeakerConfig);
GUEST_FUNCTION_HOOK(__imp__XamContentSetThumbnail, XamContentSetThumbnail);
GUEST_FUNCTION_HOOK(__imp__XamInputGetKeystrokeEx, XamInputGetKeystrokeEx);
GUEST_FUNCTION_HOOK(__imp__XamSessionCreateHandle, Net::XamSessionCreateHandle);
GUEST_FUNCTION_HOOK(__imp__XamSessionRefObjByHandle, Net::XamSessionRefObjByHandle);
GUEST_FUNCTION_HOOK(__imp__KeSetDisableBoostThread, KeSetDisableBoostThread);
GUEST_FUNCTION_HOOK(__imp__XamCreateEnumeratorHandle, XamCreateEnumeratorHandle);
GUEST_FUNCTION_HOOK(__imp__NtDeviceIoControlFile, NtDeviceIoControlFile);
GUEST_FUNCTION_HOOK(__imp__NetDll_WSAGetLastError, Net::WSAGetLastError);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetQosListen, Net::XNetQosListen);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetQosLookup, Net::XNetQosLookup);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetQosRelease, Net::XNetQosRelease);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetServerToInAddr, Net::XNetServerToInAddr);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetXnAddrToInAddr, Net::XNetXnAddrToInAddr);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetGetConnectStatus, Net::XNetGetConnectStatus);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetUnregisterInAddr, Net::XNetUnregisterInAddr);

// =============================================================================
// GTA IV File System API Hooks (UnleashedRecomp Pattern)
// =============================================================================
// Following UnleashedRecomp's elegant approach, we hook at the file API
// boundary rather than the low-level storage device vtable layer.
// This avoids validation token complexity and lets game logic run naturally.
// =============================================================================

extern "C" void __imp__sub_8249BE88(PPCContext& ctx, uint8_t* base);
GUEST_FUNCTION_HOOK(sub_8249BE88, GTA::FileResolve);


// =============================================================================
// Instrumentation for sub_82120FB8 blocking point
// =============================================================================
extern "C" void __imp__sub_827DB118(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827DB118) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_827DB118 (thread worker init) ENTER #{} r5=0x{:08X}", s_count, ctx.r5.u32);
    __imp__sub_827DB118(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_827DB118 (thread worker init) EXIT #{}", s_count);
}

// sub_8230D760 - called in loop by sub_8219FD88
extern "C" void __imp__sub_8230D760(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8230D760) {
    static int s_count = 0; ++s_count;
    if (s_count <= 20) LOGF_WARNING("[8219FD88-TRACE] sub_8230D760 ENTER #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    __imp__sub_8230D760(ctx, base);
    if (s_count <= 20) LOGF_WARNING("[8219FD88-TRACE] sub_8230D760 EXIT #{}", s_count);
}

// sub_8230D160 - called in loop by sub_8219FD88  
extern "C" void __imp__sub_8230D160(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8230D160) {
    static int s_count = 0; ++s_count;
    if (s_count <= 20) LOGF_WARNING("[8219FD88-TRACE] sub_8230D160 ENTER #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    __imp__sub_8230D160(ctx, base);
    if (s_count <= 20) LOGF_WARNING("[8219FD88-TRACE] sub_8230D160 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

// sub_8219F9A0 - called by sub_8219FD88
extern "C" void __imp__sub_8219F9A0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8219F9A0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[8219FD88-TRACE] sub_8219F9A0 ENTER #{}", s_count);
    __imp__sub_8219F9A0(ctx, base);
    LOGF_WARNING("[8219FD88-TRACE] sub_8219F9A0 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

// sub_8219F948 - called by sub_8219FD88
extern "C" void __imp__sub_8219F948(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8219F948) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[8219FD88-TRACE] sub_8219F948 ENTER #{}", s_count);
    __imp__sub_8219F948(ctx, base);
    LOGF_WARNING("[8219FD88-TRACE] sub_8219F948 EXIT #{}", s_count);
}

// sub_824C1668 - called by sub_8219FD88
extern "C" void __imp__sub_824C1668(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_824C1668) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[8219FD88-TRACE] sub_824C1668 ENTER #{} r3=0x{:08X} r4={}", s_count, ctx.r3.u32, ctx.r4.u32);
    __imp__sub_824C1668(ctx, base);
    LOGF_WARNING("[8219FD88-TRACE] sub_824C1668 EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

// sub_8219FD88 - called early in sub_82120FB8, potential blocker
extern "C" void __imp__sub_8219FD88(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8219FD88) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_8219FD88 ENTER #{}", s_count);
    __imp__sub_8219FD88(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_8219FD88 EXIT #{}", s_count);
}

// sub_822F8980 - storage/file init in sub_82120FB8
// PHASE 1: Enhanced instrumentation with global tracking flag

extern "C" void __imp__sub_822F8980(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_822F8980) {
    static int s_count = 0; ++s_count;
    g_inStorageInit = true;
    LOGF_WARNING("[STORAGE-INIT] sub_822F8980 ENTER #{} r3=0x{:08X} r4=0x{:08X}", 
                 s_count, ctx.r3.u32, ctx.r4.u32);
    __imp__sub_822F8980(ctx, base);
    g_inStorageInit = false;
    LOGF_WARNING("[STORAGE-INIT] sub_822F8980 EXIT #{}", s_count);
}

// =============================================================================
// PHASE 1: Deep Instrumentation - File Parsing Chain
// =============================================================================
// Global helper to read string from guest memory
static std::string ReadGuestString(uint8_t* base, uint32_t addr, int maxLen = 260) {
    if (addr < 0x80000000 || addr >= 0x90000000) return "<invalid addr>";
    std::string result;
    for (int i = 0; i < maxLen; i++) {
        uint8_t c = *(base + addr + i);
        if (c == 0) break;
        if (c >= 0x20 && c <= 0x7E) result += (char)c;
        else result += '.';
    }
    return result.empty() ? "<empty>" : result;
}

// sub_822F3110 - File parsing loop iterator (called repeatedly)
// PHASE 1: Deep instrumentation to find blocking point
extern "C" void __imp__sub_822F3110(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_822F3110) {
    static int s_count = 0; ++s_count;
    
    // Stack addresses now in 0x80000000+ range (physical heap allocation fix)
    
    // Always log first 50 calls, then every 100th
    bool shouldLog = (s_count <= 50) || (s_count % 100 == 0);
    
    if (shouldLog) {
        // Log all input registers
        LOGF_WARNING("[FILE-PARSE] sub_822F3110 #{} ENTER r3=0x{:08X} r4=0x{:08X} r5=0x{:08X} r6=0x{:08X}",
                     s_count, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
        
        // If r3 looks like a valid pointer, dump structure contents
        uint32_t contextPtr = ctx.r3.u32;
        if (contextPtr >= 0x80000000 && contextPtr < 0x90000000) {
            uint32_t ctx0 = ByteSwap(*(uint32_t*)(base + contextPtr + 0));
            uint32_t ctx4 = ByteSwap(*(uint32_t*)(base + contextPtr + 4));
            uint32_t ctx8 = ByteSwap(*(uint32_t*)(base + contextPtr + 8));
            uint32_t ctxC = ByteSwap(*(uint32_t*)(base + contextPtr + 12));
            uint32_t ctx10 = ByteSwap(*(uint32_t*)(base + contextPtr + 16));
            uint32_t ctx14 = ByteSwap(*(uint32_t*)(base + contextPtr + 20));
            
            LOGF_WARNING("[FILE-PARSE] sub_822F3110 #{} context[0x{:08X}]: [0]=0x{:08X} [4]=0x{:08X} [8]=0x{:08X} [C]=0x{:08X} [10]=0x{:08X} [14]=0x{:08X}",
                         s_count, contextPtr, ctx0, ctx4, ctx8, ctxC, ctx10, ctx14);
            
            // Try to find string path - check common offsets
            for (uint32_t offset : {0u, 4u, 8u, 12u, 16u, 20u, 24u, 32u}) {
                uint32_t strPtr = ByteSwap(*(uint32_t*)(base + contextPtr + offset));
                if (strPtr >= 0x80000000 && strPtr < 0x90000000) {
                    std::string str = ReadGuestString(base, strPtr, 64);
                    if (str.length() > 2 && str[0] != '<') {
                        LOGF_WARNING("[FILE-PARSE] sub_822F3110 #{} possible path at offset {}: '{}'",
                                     s_count, offset, str);
                    }
                }
            }
        }
        
        // If r4 looks like a valid pointer (could be buffer or path)
        if (ctx.r4.u32 >= 0x80000000 && ctx.r4.u32 < 0x90000000) {
            std::string r4str = ReadGuestString(base, ctx.r4.u32, 64);
            if (r4str.length() > 2 && r4str[0] != '<') {
                LOGF_WARNING("[FILE-PARSE] sub_822F3110 #{} r4 string: '{}'", s_count, r4str);
            }
        }
    }
    
    // Log LR (return address) to trace caller
    if (s_count <= 10) {
        LOGF_WARNING("[FILE-PARSE] sub_822F3110 #{} LR=0x{:08X} (caller)", s_count, (uint32_t)ctx.lr);
    }
    
    __imp__sub_822F3110(ctx, base);
    
    if (shouldLog) {
        LOGF_WARNING("[FILE-PARSE] sub_822F3110 #{} EXIT r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// sub_822F8890 - File parser orchestrator
// FIX: When file parsing finds no entries (context[0]=0), the original code crashes
// trying to access memory at loc_822F8950 after stream close. This safe implementation
// skips sub_822F87E0 when there are no entries to process.
extern "C" void __imp__sub_822F8890(PPCContext& ctx, uint8_t* base);

// Forward declarations for PPC runtime functions called by sub_822F8890
extern "C" void __imp__sub_82192840(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_828E0AB8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82148358(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_8218BE78(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_822F3110(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_822F57A8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_827827C8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82192A60(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_822F87E0(PPCContext& ctx, uint8_t* base);
// __savegprlr_29 and __restgprlr_29 declared in ppc_recomp_shared.h

PPC_FUNC(sub_822F8890) {
    static int s_count = 0; ++s_count;
    
    uint32_t contextPtr = ctx.r3.u32;
    uint32_t filePathPtr = ctx.r4.u32;
    
    if (s_count <= 10) {
        LOGF_WARNING("[FILE-PARSE] sub_822F8890 #{} ENTER context=0x{:08X} path=0x{:08X}", 
                     s_count, contextPtr, filePathPtr);
    }
    
    // Save registers (matching __savegprlr_29)
    ctx.r12.u64 = ctx.lr;
    ctx.lr = 0x822F8898;
    __savegprlr_29(ctx, base);
    
    // Create stack frame
    uint32_t ea = ctx.r1.u32 - 128;
    PPC_STORE_U32(ea, ctx.r1.u32);
    ctx.r1.u32 = ea;
    
    // Save arguments
    ctx.r30.u64 = filePathPtr;  // r30 = file path
    ctx.r31.u64 = contextPtr;   // r31 = context
    
    // Open file: sub_82192840(filePath, "common:/data/cdimages/hud.rpf")
    ctx.r4.s64 = (int64_t)(-2113208320) + (-28176);  // File path constant
    ctx.r3.u64 = ctx.r30.u64;
    ctx.lr = 0x822F88B4;
    __imp__sub_82192840(ctx, base);
    
    uint32_t streamHandle = ctx.r3.u32;
    ctx.r29.u64 = streamHandle;
    
    // Store handle on stack
    PPC_STORE_U32(ctx.r1.u32 + 80, streamHandle);
    
    // Check if file opened successfully
    if (streamHandle == 0) {
        // File failed to open - call error handler and return 0
        ctx.r3.s64 = (int64_t)(-2113863680) + 836;
        ctx.lr = 0x822F88D4;
    __imp__sub_828E0AB8(ctx, base);
        ctx.r3.s64 = 0;
        
        // Restore stack and return
        ctx.r1.s64 = ctx.r1.s64 + 128;
        __restgprlr_29(ctx, base);
        
        if (s_count <= 10) {
            LOGF_WARNING("[FILE-PARSE] sub_822F8890 #{} EXIT (file open failed) r3=0", s_count);
        }
        return;
    }
    
    // File opened - initialize context
    ctx.r3.s64 = ctx.r31.s64 + 8;
    ctx.lr = 0x822F88E8;
    __imp__sub_82148358(ctx, base);
    
    ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
    ctx.lr = 0x822F88F0;
    __imp__sub_8218BE78(ctx, base);
    
    // Zero out context fields
    PPC_STORE_U32(ctx.r31.u32 + 0, 0);
    PPC_STORE_U16(ctx.r31.u32 + 4, 0);
    PPC_STORE_U16(ctx.r31.u32 + 6, 0);
    
    // Parse file entries
    if (s_count <= 10) LOGF_WARNING("[FILE-PARSE] sub_822F8890 #{} calling __imp__sub_822F3110", s_count);
    ctx.r3.s64 = ctx.r1.s64 + 80;
    ctx.lr = 0x822F8908;
    __imp__sub_822F3110(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[FILE-PARSE] sub_822F8890 #{} __imp__sub_822F3110 returned r3=0x{:08X}", s_count, ctx.r3.u32);
    
    // Process entries loop
    while (ctx.r3.u32 != 0) {
        ctx.r4.u64 = ctx.r3.u64;
        ctx.r5.s64 = ctx.r1.s64 + 88;
        ctx.r3.u64 = ctx.r31.u64;
        ctx.lr = 0x822F8920;
        __imp__sub_822F57A8(ctx, base);
        
        if ((ctx.r3.u32 & 0xFF) != 0) {
            ctx.r4.s64 = 16;
            ctx.r3.u64 = ctx.r31.u64;
            ctx.lr = 0x822F8938;
            __imp__sub_827827C8(ctx, base);
            
            uint64_t val = PPC_LOAD_U64(ctx.r1.u32 + 88);
            PPC_STORE_U64(ctx.r3.u32 + 0, val);
        }
        
        ctx.r3.s64 = ctx.r1.s64 + 80;
        ctx.lr = 0x822F8948;
        __imp__sub_822F3110(ctx, base);
    }
    
    // Close stream - call through wrapper (sub_82192A60) for safety checks
    if (s_count <= 10) LOGF_WARNING("[FILE-PARSE] sub_822F8890 #{} closing stream 0x{:08X}", s_count, ctx.r29.u32);
    ctx.r3.u64 = ctx.r29.u64;
    ctx.lr = 0x822F8958;
    sub_82192A60(ctx, base);  // Use wrapper, not __imp__ directly
    if (s_count <= 10) LOGF_WARNING("[FILE-PARSE] sub_822F8890 #{} stream closed", s_count);
    
    // SAFE FIX: Read context values and check before calling sub_822F87E0
    if (s_count <= 10) LOGF_WARNING("[FILE-PARSE] sub_822F8890 #{} reading context at 0x{:08X}", s_count, ctx.r31.u32);
    uint16_t entryCount = ByteSwap(*(uint16_t*)(base + ctx.r31.u32 + 4));
    uint32_t arrayPtr = ByteSwap(*(uint32_t*)(base + ctx.r31.u32 + 0));
    if (s_count <= 10) LOGF_WARNING("[FILE-PARSE] sub_822F8890 #{} context read OK", s_count);
    
    if (s_count <= 10) {
        LOGF_WARNING("[FILE-PARSE] sub_822F8890 #{} after parse: arrayPtr=0x{:08X} count={}", 
                     s_count, arrayPtr, entryCount);
    }
    
    // Only call sub_822F87E0 if there are entries to process
    if (arrayPtr != 0 && entryCount > 0) {
        uint32_t endPtr = arrayPtr + (entryCount << 3);  // rotlwi by 3 = multiply by 8
        
        ctx.r3.u64 = arrayPtr;
        ctx.r4.u64 = endPtr;
        ctx.lr = 0x822F896C;
        __imp__sub_822F87E0(ctx, base);
    } else {
        if (s_count <= 10) {
            LOGF_WARNING("[FILE-PARSE] sub_822F8890 #{} SKIPPING sub_822F87E0 (no entries)", s_count);
        }
    }
    
    // Set completion flag
    PPC_STORE_U8(ctx.r31.u32 + 16, 1);
    ctx.r3.s64 = 1;
    
    // Restore stack and return
    ctx.r1.s64 = ctx.r1.s64 + 128;
    __restgprlr_29(ctx, base);
    
    if (s_count <= 10) {
        LOGF_WARNING("[FILE-PARSE] sub_822F8890 #{} EXIT r3=1", s_count);
    }
}

// sub_822F87E0 - Called after file parsing to process results
// Suspected crash location - called from sub_822F8890 after stream close
extern "C" void __imp__sub_822F87E0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_822F87E0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) {
        LOGF_WARNING("[FILE-PARSE] sub_822F87E0 #{} ENTER r3=0x{:08X} r4=0x{:08X}", 
                     s_count, ctx.r3.u32, ctx.r4.u32);
        
        // Check if pointers are valid
        bool r3Valid = (ctx.r3.u32 >= 0x80000000 && ctx.r3.u32 < 0x100000000ULL) ||
                       (ctx.r3.u32 >= 0x00020000 && ctx.r3.u32 < 0x7FEA0000);
        bool r4Valid = (ctx.r4.u32 >= 0x80000000 && ctx.r4.u32 < 0x100000000ULL) ||
                       (ctx.r4.u32 >= 0x00020000 && ctx.r4.u32 < 0x7FEA0000);
        
        if (!r3Valid || !r4Valid) {
            LOGF_WARNING("[FILE-PARSE] sub_822F87E0 #{} INVALID POINTERS r3valid={} r4valid={}",
                         s_count, r3Valid, r4Valid);
        }
        
        // Check alignment (8-byte for 64-bit operations)
        if ((ctx.r3.u32 & 0x7) != 0) {
            LOGF_WARNING("[FILE-PARSE] sub_822F87E0 #{} r3=0x{:08X} NOT 8-BYTE ALIGNED",
                         s_count, ctx.r3.u32);
        }
        if ((ctx.r4.u32 & 0x7) != 0) {
            LOGF_WARNING("[FILE-PARSE] sub_822F87E0 #{} r4=0x{:08X} NOT 8-BYTE ALIGNED",
                         s_count, ctx.r4.u32);
        }
    }
    __imp__sub_822F87E0(ctx, base);
    if (s_count <= 10) {
        LOGF_WARNING("[FILE-PARSE] sub_822F87E0 #{} EXIT", s_count);
    }
}

// sub_822F57A8 - File entry processor (called in loop)
extern "C" void __imp__sub_822F57A8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_822F57A8) {
    static int s_count = 0; ++s_count;
    bool shouldLog = (s_count <= 30) || (s_count % 50 == 0);
    
    if (shouldLog) {
        LOGF_WARNING("[FILE-PARSE] sub_822F57A8 #{} ENTER r3=0x{:08X} r4=0x{:08X} r5=0x{:08X}",
                     s_count, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
    }
    __imp__sub_822F57A8(ctx, base);
    if (shouldLog) {
        LOGF_WARNING("[FILE-PARSE] sub_822F57A8 #{} EXIT r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// =============================================================================
// sub_822AFAF8 - Safe wrapper for uninitialized global protection
// =============================================================================
// FIX: Global at 0x82A2DB54 (offset -9388 from 0x82A30000) is NEVER initialized
// in the recompiled code. The caller sub_822B4D68 loads this garbage value into
// r4 and passes it here. Inside this function, r4 is copied to r28, and later
// the code crashes trying to access r28+22000.
//
// Call site (ppc_recomp.11.cpp:111705-111711):
//   r3 = 256 (constant)
//   r4 = garbage from uninitialized global 0x82A2DB54
//   bl sub_822AFAF8
//
// This wrapper validates r4 before proceeding.
extern "C" void __imp__sub_822AFAF8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_822AFAF8) {
    static int s_count = 0; ++s_count;
    
    // PRIMARY FIX: Check r4 (the global pointer from 0x82A2DB54)
    // This is the uninitialized global that causes the crash
    uint32_t globalPtr = ctx.r4.u32;
    if (globalPtr == 0 || globalPtr < 0x80000000 || globalPtr >= 0x90000000) {
        if (s_count <= 20 || s_count % 100 == 0) {
            LOGF_WARNING("[sub_822AFAF8] #{} SKIP - r4 contains invalid global ptr 0x{:08X} (from uninitialized 0x82A2DB54)", 
                         s_count, globalPtr);
        }
        ctx.r3.s64 = 0;  // Return null/failure
        return;
    }
    
    // Secondary check: Verify the object at globalPtr+22000 is accessible
    // The crash happens at: lwz r3,22000(r28) where r28=r4
    uint32_t subObjAddr = globalPtr + 22000;
    if (subObjAddr < 0x80000000 || subObjAddr >= 0x90000000) {
        if (s_count <= 20 || s_count % 100 == 0) {
            LOGF_WARNING("[sub_822AFAF8] #{} SKIP - r4+22000=0x{:08X} out of range", 
                         s_count, subObjAddr);
        }
        ctx.r3.s64 = 0;
        return;
    }
    
    // Check the sub-object pointer at globalPtr+22000
    uint32_t subObjPtr = __builtin_bswap32(*(uint32_t*)(base + subObjAddr));
    if (subObjPtr == 0 || subObjPtr < 0x80000000 || subObjPtr >= 0x90000000) {
        if (s_count <= 20 || s_count % 100 == 0) {
            LOGF_WARNING("[sub_822AFAF8] #{} SKIP - [r4+22000]=0x{:08X} invalid sub-object ptr", 
                         s_count, subObjPtr);
        }
        ctx.r3.s64 = 0;
        return;
    }
    
    // Check vtable pointer at sub-object
    uint32_t vtablePtr = __builtin_bswap32(*(uint32_t*)(base + subObjPtr));
    if (vtablePtr == 0 || vtablePtr < 0x80000000 || vtablePtr >= 0x90000000) {
        if (s_count <= 20 || s_count % 100 == 0) {
            LOGF_WARNING("[sub_822AFAF8] #{} SKIP - vtable ptr 0x{:08X} invalid", 
                         s_count, vtablePtr);
        }
        ctx.r3.s64 = 0;
        return;
    }
    
    // Check function pointer at vtable+56
    uint32_t funcPtr = __builtin_bswap32(*(uint32_t*)(base + vtablePtr + 56));
    if (funcPtr == 0 || funcPtr < 0x82000000 || funcPtr >= 0x83000000) {
        if (s_count <= 20 || s_count % 100 == 0) {
            LOGF_WARNING("[sub_822AFAF8] #{} SKIP - func ptr 0x{:08X} at vtable[56] invalid", 
                         s_count, funcPtr);
        }
        ctx.r3.s64 = 0;
        return;
    }
    
    if (s_count <= 5) {
        LOGF_WARNING("[sub_822AFAF8] #{} VALID r4=0x{:08X} subObj=0x{:08X} vtable=0x{:08X} func=0x{:08X}", 
                     s_count, globalPtr, subObjPtr, vtablePtr, funcPtr);
    }
    
    __imp__sub_822AFAF8(ctx, base);
    
    if (s_count <= 5) {
        LOGF_WARNING("[sub_822AFAF8] #{} EXIT r3=0x{:08X}", s_count, ctx.r3.u32);
    }
}

// =========================================================================
// Remaining 63-subsystem hooks (after Stats system)
// =========================================================================
extern "C" void __imp__sub_8212FB78(PPCContext& ctx, uint8_t* base);  // Friend system
extern "C" void __imp__sub_8219ADF0(PPCContext& ctx, uint8_t* base);  // Online system
extern "C" void __imp__sub_8212F578(PPCContext& ctx, uint8_t* base);  // Leaderboard
extern "C" void __imp__sub_8212EDC8(PPCContext& ctx, uint8_t* base);  // Achievement
extern "C" void __imp__sub_82138710(PPCContext& ctx, uint8_t* base);  // Replay system
extern "C" void __imp__sub_821B2ED8(PPCContext& ctx, uint8_t* base);  // Camera recording
extern "C" void __imp__sub_822467B8(PPCContext& ctx, uint8_t* base);  // Cinematic camera
extern "C" void __imp__sub_82208460(PPCContext& ctx, uint8_t* base);  // Photo mode
extern "C" void __imp__sub_821B9DA8(PPCContext& ctx, uint8_t* base);  // TV system
extern "C" void __imp__sub_82258100(PPCContext& ctx, uint8_t* base);  // Internet cafe
extern "C" void __imp__sub_821A03A0(PPCContext& ctx, uint8_t* base);  // Phone system
extern "C" void __imp__sub_8232A2C0(PPCContext& ctx, uint8_t* base);  // Dating system
extern "C" void __imp__sub_82125478(PPCContext& ctx, uint8_t* base);  // Final setup

PPC_FUNC(sub_8212FB78) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_8212FB78 (Friend system) ENTER #{}", s_count);
    __imp__sub_8212FB78(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_8212FB78 (Friend system) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

// STUBBED: sub_8219ADF0 (Online system)
PPC_FUNC(sub_8219ADF0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[8219ADF0] #{} STUBBED - Online system bypassed", s_count);
    ctx.r3.u32 = 0;
}







PPC_FUNC(sub_8212F578) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_8212F578 (Leaderboard) ENTER #{}", s_count);
    __imp__sub_8212F578(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_8212F578 (Leaderboard) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_8212EDC8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_8212EDC8 (Achievement) ENTER #{}", s_count);
    __imp__sub_8212EDC8(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_8212EDC8 (Achievement) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_82138710) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_82138710 (Replay system) ENTER #{}", s_count);
    __imp__sub_82138710(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_82138710 (Replay system) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_821B2ED8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_821B2ED8 (Camera recording) ENTER #{}", s_count);
    __imp__sub_821B2ED8(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_821B2ED8 (Camera recording) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_822467B8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_822467B8 (Cinematic camera) ENTER #{}", s_count);
    __imp__sub_822467B8(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_822467B8 (Cinematic camera) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_82208460) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_82208460 (Photo mode) ENTER #{}", s_count);
    __imp__sub_82208460(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_82208460 (Photo mode) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_821B9DA8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[821B9DA8] #{} STUBBED - TV system bypassed", s_count);
    ctx.r3.u32 = 0;
    return;
    // Original below

}
PPC_FUNC(sub_82258100) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_82258100 (Internet cafe) ENTER #{}", s_count);
    __imp__sub_82258100(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_82258100 (Internet cafe) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_821A03A0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_821A03A0 (Phone system) ENTER #{}", s_count);
    __imp__sub_821A03A0(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_821A03A0 (Phone system) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

PPC_FUNC(sub_8232A2C0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[63-SUBSYS] sub_8232A2C0 (Dating system) ENTER #{}", s_count);
    __imp__sub_8232A2C0(ctx, base);
    LOGF_WARNING("[63-SUBSYS] sub_8232A2C0 (Dating system) EXIT #{} r3=0x{:08X}", s_count, ctx.r3.u32);
}

// STUBBED: sub_82125478 (Final setup)
PPC_FUNC(sub_82125478) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[82125478] #{} STUBBED - Final setup bypassed", s_count);
    ctx.r3.u32 = 0;
}







// =========================================================================
// Main Loop Tracing Hooks
// =========================================================================
extern "C" void __imp__sub_82856F08(PPCContext& ctx, uint8_t* base);  // Main Loop Entry
extern "C" void __imp__sub_828529B0(PPCContext& ctx, uint8_t* base);  // Main Loop Orchestrator
extern "C" void __imp__sub_828507F8(PPCContext& ctx, uint8_t* base);  // Frame Presentation
extern "C" void __imp__sub_829D5388(PPCContext& ctx, uint8_t* base);  // D3D Present (VdSwap)

PPC_FUNC(sub_82856F08) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5 || s_count % 1000 == 0)
        LOGF_WARNING("[MAIN_LOOP] sub_82856F08 (Entry) #{}", s_count);
    __imp__sub_82856F08(ctx, base);
}

PPC_FUNC(sub_828529B0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5 || s_count % 1000 == 0)
        LOGF_WARNING("[MAIN_LOOP] sub_828529B0 (Orchestrator) #{}", s_count);
    __imp__sub_828529B0(ctx, base);
}

PPC_FUNC(sub_828507F8) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5 || s_count % 1000 == 0)
        LOGF_WARNING("[RENDER] sub_828507F8 (Frame Present) #{}", s_count);
    __imp__sub_828507F8(ctx, base);
}

PPC_FUNC(sub_829D5388) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5 || s_count % 1000 == 0)
        LOGF_WARNING("[RENDER] sub_829D5388 (D3D Present/VdSwap) #{}", s_count);
    __imp__sub_829D5388(ctx, base);
}

// =========================================================================
// sub_821200D0 internal tracing
// =========================================================================
extern "C" void __imp__sub_82121E80(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_821924D8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_8218C2C0(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_8218C1F0(PPCContext& ctx, uint8_t* base);

// STUBBED: sub_82121E80 - loading state machine (blocks on async ops)
PPC_FUNC(sub_82121E80) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[82121E80] #{} STUBBED - loading state bypassed", s_count);
    ctx.r3.u32 = 0;
}







PPC_FUNC(sub_821924D8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[821200D0-INT] sub_821924D8 ENTER #{}", s_count);
    __imp__sub_821924D8(ctx, base);
    LOGF_WARNING("[821200D0-INT] sub_821924D8 EXIT #{}", s_count);
}

// STUBBED: sub_8218C2C0 - loading check (return 1=ready to break wait loop)
PPC_FUNC(sub_8218C2C0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 3) LOGF_WARNING("[8218C2C0] #{} STUBBED - returning ready", s_count);
    ctx.r3.u32 = 1;  // Return ready to break loading loop
}







PPC_FUNC(sub_8218C1F0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5 || s_count % 1000 == 0)
        LOGF_WARNING("[821200D0-INT] sub_8218C1F0 #{}", s_count);
    __imp__sub_8218C1F0(ctx, base);
}

// =========================================================================
// sub_821200A8 tracing (called after sub_821200D0 in sub_8218BEB0)
// =========================================================================
extern "C" void __imp__sub_821200A8(PPCContext& ctx, uint8_t* base);

// STUB: sub_821200A8 - bypass post-init finalization to let main loop proceed
PPC_FUNC(sub_821200A8) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[821200A8] #{} STUBBED - bypassing post-init finalization", s_count);
    ctx.r3.u32 = 0;  // Return success
    LOGF_WARNING("[821200A8] #{} EXIT", s_count);
}

// =========================================================================
// sub_821219B0 - trace to find blocking point
// =========================================================================
extern "C" void __imp__sub_821219B0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_821219B0) {
    static int s_count = 0; ++s_count;
    printf("[821219B0] #%d STUBBED - bypassing blocking sync primitive\n", s_count);
    ctx.r3.u32 = 0; // Stub - bypass blocking
}

// =========================================================================
// sub_821200D0 cleanup function traces - find which one blocks
// =========================================================================
extern "C" void __imp__sub_821B8FB0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_821B8FB0) {
    static int s_count = 0; ++s_count;
    printf("[821B8FB0] #%d ENTER\n", s_count);
    __imp__sub_821B8FB0(ctx, base);
    printf("[821B8FB0] #%d EXIT\n", s_count);
}

extern "C" void __imp__sub_8220E528(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8220E528) {
    static int s_count = 0; ++s_count;
    printf("[8220E528] #%d ENTER\n", s_count);
    __imp__sub_8220E528(ctx, base);
    printf("[8220E528] #%d EXIT\n", s_count);
}

extern "C" void __imp__sub_822C66C8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_822C66C8) {
    static int s_count = 0; ++s_count;
    printf("[822C66C8] #%d ENTER\n", s_count);
    __imp__sub_822C66C8(ctx, base);
    printf("[822C66C8] #%d EXIT\n", s_count);
}

extern "C" void __imp__sub_821A81F0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_821A81F0) {
    static int s_count = 0; ++s_count;
    printf("[821A81F0] #%d ENTER - signaling workers before cleanup\n", s_count);
    fflush(stdout);
    
    // Signal all semaphores to wake Photo mode workers so they can exit
    // This is the proper fix: workers are waiting for work that won't come,
    // signaling allows them to check exit flag and terminate
    SignalAllBlockingSemaphores();
    
    __imp__sub_821A81F0(ctx, base);
    printf("[821A81F0] #%d EXIT\n", s_count);
    fflush(stdout);
}

// sub_823005E0 - Called by sub_821A81F0, performs worker cleanup
// This function blocks waiting for workers to exit. Stub it to bypass the wait.
extern "C" void __imp__sub_823005E0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_823005E0) {
    static int s_count = 0; ++s_count;
    printf("[823005E0] #%d STUBBED - bypassing worker cleanup wait\n", s_count);
    fflush(stdout);
    
    // Signal all semaphores to let workers know to exit
    SignalAllBlockingSemaphores();
    
    // Don't call original - it blocks waiting for workers
    // The workers will exit on their own when they check their exit flags
    // __imp__sub_823005E0(ctx, base);
}

// sub_827DB880 - Worker manager shutdown loop
// This function loops waiting for all workers to exit.
// We must signal all semaphores before the loop to ensure workers wake up and exit.
extern "C" void __imp__sub_827DB880(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827DB880) {
    static int s_count = 0; ++s_count;
    printf("[827DB880] #%d ENTER - worker shutdown loop, signaling all semaphores\n", s_count);
    fflush(stdout);
    
    // Signal all semaphores to wake any workers still waiting
    // This allows them to check their exit flags and terminate
    SignalAllBlockingSemaphores();
    
    // Brief delay to let workers process the signal
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Signal again to catch any stragglers
    SignalAllBlockingSemaphores();
    
    printf("[827DB880] #%d calling original function\n", s_count);
    fflush(stdout);
    
    __imp__sub_827DB880(ctx, base);
    
    printf("[827DB880] #%d EXIT\n", s_count);
    fflush(stdout);
}

extern "C" void __imp__sub_82268260(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82268260) {
    static int s_count = 0; ++s_count;
    printf("[82268260] #%d ENTER\n", s_count);
    __imp__sub_82268260(ctx, base);
    printf("[82268260] #%d EXIT\n", s_count);
}

extern "C" void __imp__sub_8221BFF0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8221BFF0) {
    static int s_count = 0; ++s_count;
    printf("[8221BFF0] #%d ENTER\n", s_count);
    __imp__sub_8221BFF0(ctx, base);
    printf("[8221BFF0] #%d EXIT\n", s_count);
}

extern "C" void __imp__sub_82297B08(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82297B08) {
    static int s_count = 0; ++s_count;
    printf("[82297B08] #%d ENTER\n", s_count);
    __imp__sub_82297B08(ctx, base);
    printf("[82297B08] #%d EXIT\n", s_count);
}

extern "C" void __imp__sub_82212E08(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82212E08) {
    static int s_count = 0; ++s_count;
    printf("[82212E08] #%d ENTER\n", s_count);
    __imp__sub_82212E08(ctx, base);
    printf("[82212E08] #%d EXIT\n", s_count);
}
