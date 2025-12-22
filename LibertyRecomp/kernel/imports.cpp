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
#include <user/config.h>
#include <user/paths.h>
#include <os/logger.h>

#include "io/file_system.h"
#include "vfs.h"
#include "game_init.h"

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

uint32_t XamUserGetSigninState(uint32_t userIndex)
{
    return true;
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
                
                // For paths like fxl_final\, these are inside common.rpf
                // Open common.rpf as a file handle so sub_829A1F00 can read from it
                if (pathLower.find("fxl_final") != std::string::npos)
                {
                    // Open common.rpf for reading
                    std::filesystem::path rpfPath = GetGamePath() / "game" / "common.rpf";
                    if (std::filesystem::exists(rpfPath))
                    {
                        std::fstream rpfStream;
                        rpfStream.open(rpfPath, std::ios::in | std::ios::binary);
                        
                        if (rpfStream.is_open())
                        {
                            NtFileHandle* hFile = CreateKernelObject<NtFileHandle>();
                            hFile->stream = std::move(rpfStream);
                            hFile->path = rpfPath;
                            hFile->isRpf = true;
                            
                            const uint32_t handleValue = GetKernelHandle(hFile);
                            g_ntFileHandles.emplace(handleValue, hFile);
                            
                            // Parse RPF header immediately
                            ParseRpfHeader(hFile);

                            *FileHandle = handleValue;
                            LOGF_IMPL(Utility, "NtCreateFile", "fxl_final -> opened common.rpf for reading (handle=0x{:08X})", 
                                      handleValue);
                            
                            if (IoStatusBlock)
                            {
                                IoStatusBlock->Status = STATUS_SUCCESS;
                                IoStatusBlock->Information = 1;
                            }
                            return STATUS_SUCCESS;
                        }
                    }
                    
                    // Fallback to extracted directory if RPF not available
                    std::filesystem::path extractedPath = GetGamePath() / "RPF DUMP" / "common" / "shaders" / "fxl_final";
                    if (std::filesystem::exists(extractedPath))
                    {
                        NtDirHandle* hDir = CreateKernelObject<NtDirHandle>();
                        hDir->path = extractedPath;

                        const uint32_t handleValue = GetKernelHandle(hDir);
                        g_ntDirHandles.emplace(handleValue, hDir);

                        *FileHandle = handleValue;
                        LOGF_IMPL(Utility, "NtCreateFile", "fxl_final -> fallback to extracted dir: {} (handle=0x{:08X})", 
                                  extractedPath.string(), handleValue);
                        
                        if (IoStatusBlock)
                        {
                            IoStatusBlock->Status = STATUS_SUCCESS;
                            IoStatusBlock->Information = 1;
                        }
                        return STATUS_SUCCESS;
                    }
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
    
    // VBlank Heartbeat Injection - Fire at 60Hz from guest thread context
    // This is safe here because NtWaitEx is called by valid Guest Threads with TLS setup
    uint32_t vblankCallback = g_gpuRingBuffer.interruptCallback;
    
    if (vblankCallback != 0)
    {
        static int s_vblankCount = 0;
        static int s_callsSinceVBlank = 0;
        ++s_callsSinceVBlank;
        
        // Fire VBlank every 3 NtWaitEx calls to ensure steady heartbeat
        // Game has blocking waits so need aggressive firing
        if (s_callsSinceVBlank >= 3)
        {
            s_callsSinceVBlank = 0;
            ++s_vblankCount;
            
            // Scan ring buffer for PM4 packets (diagnostic)
            ScanRingBufferForPM4Packets();
            
            // Enable PPC call tracking after VBlank 10, report at VBlank 300
            if (s_vblankCount == 10) {
                g_ppcCallTrackingEnabled = 1;
                LOGF_WARNING("[DIAG] Enabled PPC function call tracking at VBlank #{}", s_vblankCount);
            }
            if (s_vblankCount == 300) {
                ReportHotPPCFunctions();
            }
            
            if (s_vblankCount <= 20 || s_vblankCount % 60 == 0)
            {
                LOGF_IMPL(Utility, "VBlank", "Firing #{} from NtWaitEx (call-based)", s_vblankCount);
            }
            
            // Execute the game's interrupt handler
            auto func = g_memory.FindFunction(vblankCallback);
            if (func)
            {
                PPCContext tempCtx = *g_ppcContext;
                tempCtx.r3.u32 = 0;  // Interrupt Type 0 = VBlank
                tempCtx.r4.u32 = g_gpuRingBuffer.interruptUserData;
                func(tempCtx, g_memory.base);
            }
        }
    }

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
        
        // Force-succeed worker semaphore waits to allow workers to do their tasks
        // Workers are waiting for work signals - force them to run so they can 
        // potentially unblock the main thread which is stuck in sub_82273988
        // Main thread is NOT in these waits - it's busy-waiting on memory inside sub_82273988
        bool isWorkerSem = (Handle >= 0xEB2C0000 && Handle <= 0xEB2DFFFF) ||
                           (Handle == 0xA82487B0 || Handle == 0xA82487F0 ||
                            Handle == 0xA8240270 || Handle == 0xA82402B0);
        if (isWorkerSem)
        {
            static int s_forceCount = 0;
            if (++s_forceCount <= 30 || s_forceCount % 500 == 0)
            {
                LOGF_WARNING("[NtWaitEx] FORCE WORKER #{} sem 0x{:08X} timeout={}", 
                            s_forceCount, Handle, timeout);
            }
            return STATUS_SUCCESS;
        }
        
        // Log all other timeout=0 waits to find blocking handles
        if (timeout == 0)
        {
            static int s_poll = 0;
            if (++s_poll <= 50 || s_poll % 500 == 0)
            {
                LOGF_WARNING("[NtWaitEx] POLL #{} handle=0x{:08X} (not in force list)", s_poll, Handle);
            }
        }
        
        // Log ALL waits that actually reach Wait() - these are potential blocking points
        static int s_actualWait = 0;
        ++s_actualWait;
        if (s_actualWait <= 50 || timeout == INFINITE)
        {
            LOGF_WARNING("[NtWaitEx] ACTUAL WAIT #{} handle=0x{:08X} timeout={} - WILL BLOCK!", 
                        s_actualWait, Handle, timeout);
        }
        
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
    Event* evt = CreateKernelObject<Event>(!eventType, !!initialState);
    uint32_t h = GetKernelHandle(evt);
    *handle = h;
    
    // Track this event so we can signal it later
    {
        std::lock_guard lock(g_eventTrackMutex);
        g_trackedEventHandles.push_back(h);
    }
    
    static int s_count = 0;
    ++s_count;
    if (s_count <= 20)
    {
        LOGF_IMPL(Utility, "NtCreateEvent", "#{} handle=0x{:08X} type={} initial={}", 
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

void VdSetGraphicsInterruptCallback(uint32_t callback, uint32_t userData)
{
    // r3 = callback function pointer
    // r4 = user data passed to callback
    g_gpuRingBuffer.interruptCallback = callback;
    g_gpuRingBuffer.interruptUserData = userData;
    
    LOGF_UTILITY("callback=0x{:08X} userData=0x{:08X}", callback, userData);
    LOG_WARNING("[VBlank] Interrupt callback registered - VBlank will fire from NtWaitEx");
}

uint32_t VdInitializeEngines()
{
    g_gpuRingBuffer.enginesInitialized = true;
    LOG_UTILITY("enginesInitialized = true");
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
    
    // =========================================================================
    // GPU FENCE BYPASS: Auto-signal the GPU fence to unblock the boot sequence
    // The GPU thread (caller 0x829DDD48) waits on fence 0x8006F844.
    // The caller is in a loop that processes GPU commands and waits for completion.
    // Since we don't have a real GPU, we block this thread to let others run.
    // =========================================================================
    if (objAddr == 0x8006F844 || caller == 0x829DDD48)
    {
        static int s_fenceBypassCount = 0;
        static bool s_gpuThreadParked = false;
        ++s_fenceBypassCount;
        
        if (s_fenceBypassCount <= 10)
        {
            LOGF_IMPL(Utility, "FenceBypass", "GPU thread wait #{} obj=0x{:08X} caller=0x{:08X}", 
                      s_fenceBypassCount, objAddr, caller);
        }
        
        // Force-signal any fence object
        if (Object)
        {
            Object->SignalState = 1;
        }
        
        // Update ring buffer read pointer to indicate all commands consumed
        if (g_gpuRingBuffer.writebackEnabled && g_gpuRingBuffer.readPtrWritebackAddr != 0)
        {
            uint32_t* writebackPtr = reinterpret_cast<uint32_t*>(
                g_memory.Translate(g_gpuRingBuffer.readPtrWritebackAddr));
            *writebackPtr = ByteSwap(0xFFFFFFFF);
        }
        
        // Force-set all GPU init flags after a few iterations to unblock boot
        if (s_fenceBypassCount >= 5)
        {
            g_gpuRingBuffer.enginesInitialized = true;
            g_gpuRingBuffer.edramTrainingComplete = true;
            g_gpuRingBuffer.interruptSeen = true;
        }
        
        // Park the GPU thread: once GPU init is done, fire VBlank interrupt periodically
        if (g_gpuRingBuffer.enginesInitialized && 
            g_gpuRingBuffer.edramTrainingComplete && 
            g_gpuRingBuffer.interruptSeen)
        {
            if (!s_gpuThreadParked)
            {
                s_gpuThreadParked = true;
                LOGF_IMPL(Utility, "FenceBypass", "GPU init complete - starting VBlank heartbeat (after {} waits)", s_fenceBypassCount);
            }
            
            // Fire VBlank interrupt periodically (~60 FPS = 16ms)
            static auto s_lastVBlank = std::chrono::steady_clock::now();
            static int s_vblankCount = 0;
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - s_lastVBlank).count();
            
            if (elapsed >= 16 && g_gpuRingBuffer.interruptCallback != 0)
            {
                s_lastVBlank = now;
                ++s_vblankCount;
                
                // Scan ring buffer for PM4 packets (diagnostic)
                ScanRingBufferForPM4Packets();
                
                // Enable PPC call tracking after VBlank 10, report at VBlank 300
                if (s_vblankCount == 10) {
                    g_ppcCallTrackingEnabled = 1;
                    LOGF_WARNING("[DIAG] Enabled PPC function call tracking at VBlank #{}", s_vblankCount);
                }
                if (s_vblankCount == 300) {
                    ReportHotPPCFunctions();
                }
                
                if (s_vblankCount <= 10 || s_vblankCount % 60 == 0)
                {
                    LOGF_IMPL(Utility, "VBlank", "Firing VBlank #{} callback=0x{:08X}", 
                              s_vblankCount, g_gpuRingBuffer.interruptCallback);
                }
                
                // Call the guest interrupt callback function
                // r3 = interrupt type (0 = VBlank, 1 = other)
                // r4 = user data pointer
                auto func = g_memory.FindFunction(g_gpuRingBuffer.interruptCallback);
                if (func)
                {
                    PPCContext tempCtx = *g_ppcContext;
                    tempCtx.r3.u32 = 0;  // Interrupt type: 0 = VBlank
                    tempCtx.r4.u32 = g_gpuRingBuffer.interruptUserData;
                    func(tempCtx, g_memory.base);
                }
            }
            
            // Short sleep to prevent tight spinning
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        else
        {
            // During init, short sleep to allow progress
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
        
        return STATUS_SUCCESS;
    }
    
    // Fire VBlank from ANY wait once GPU is initialized (not just GPU thread waits)
    // This ensures the game's render loop progresses
    if (g_gpuRingBuffer.enginesInitialized && 
        g_gpuRingBuffer.edramTrainingComplete && 
        g_gpuRingBuffer.interruptCallback != 0)
    {
        static auto s_lastVBlankAny = std::chrono::steady_clock::now();
        static int s_vblankAnyCount = 0;
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - s_lastVBlankAny).count();
        
        if (elapsed >= 16)
        {
            s_lastVBlankAny = now;
            ++s_vblankAnyCount;
            
            if (s_vblankAnyCount <= 10 || s_vblankAnyCount % 60 == 0)
            {
                LOGF_IMPL(Utility, "VBlankAny", "Firing VBlank #{} from wait caller=0x{:08X}", 
                          s_vblankAnyCount, caller);
            }
            
            // Call the guest interrupt callback
            auto func = g_memory.FindFunction(g_gpuRingBuffer.interruptCallback);
            if (func)
            {
                PPCContext tempCtx = *g_ppcContext;
                tempCtx.r3.u32 = 0;  // VBlank interrupt type
                tempCtx.r4.u32 = g_gpuRingBuffer.interruptUserData;
                func(tempCtx, g_memory.base);
            }
        }
    }
    
    // Log ALL waits for async IO debugging - exclude GPU poll thread (0x829DDD48)
    static int s_asyncWaitLogCount = 0;
    if (g_asyncReadCount > 0 && caller != 0x829DDD48 && s_asyncWaitLogCount < 200)
    {
        ++s_asyncWaitLogCount;
        LOGF_IMPL(Utility, "KeWaitForSingleObject", 
                  "ASYNC_WAIT #{} type={} obj=0x{:08X} caller=0x{:08X} timeout={} lastAsync=0x{:08X}",
                  s_asyncWaitLogCount, Object ? Object->Type : -1, objAddr, caller, timeout, g_lastAsyncInfo.load());
    }
    
    // Log INFINITE waits more aggressively as they can block forever
    if (timeout == INFINITE)
    {
        ++s_infiniteWaitCount;
        if (s_infiniteWaitCount <= 20 || s_infiniteWaitCount % 100 == 0)
        {
            LOGF_IMPL(Utility, "KeWaitForSingleObject", "INFINITE WAIT #{} type={} obj=0x{:08X} caller=0x{:08X}", 
                      s_infiniteWaitCount, Object ? Object->Type : -1, objAddr, caller);
        }
        
        // GPU init deadlock prevention: known GPU bootstrap callers that wait forever
        // These callers are waiting for GPU readiness signals that never come from stubs.
        // We fake GPU consumption by updating the ring buffer read pointer writeback.
        static const std::unordered_set<uint32_t> s_gpuInitCallers = {
            0x829DDD48,  // VdRetrainEDRAM / GPU init wait
            0x829D8AA8,  // VdInitializeRingBuffer caller
        };
        
        if (s_gpuInitCallers.count(caller) != 0)
        {
            static int s_gpuSignalCount = 0;
            ++s_gpuSignalCount;
            
            // Log video state flags periodically for debugging
            if (s_gpuSignalCount <= 5 || s_gpuSignalCount % 1000 == 0)
            {
                LOGF_IMPL(Utility, "KeWaitForSingleObject", 
                          "GPU poll #{}: engines={} edram={} interrupt={}", 
                          s_gpuSignalCount,
                          g_gpuRingBuffer.enginesInitialized ? 1 : 0,
                          g_gpuRingBuffer.edramTrainingComplete ? 1 : 0,
                          g_gpuRingBuffer.interruptSeen ? 1 : 0);
            }
            
            // Short-circuit: if all video state flags are true, GPU init is complete
            // The game's polling loop checks memory-backed flags that we can't easily update.
            // Instead, we block this thread with a real wait to stop the spinning.
            // This allows other threads to progress while the GPU poll thread sleeps.
            if (g_gpuRingBuffer.enginesInitialized &&
                g_gpuRingBuffer.edramTrainingComplete &&
                g_gpuRingBuffer.interruptSeen)
            {
                static bool s_loggedOnce = false;
                if (!s_loggedOnce)
                {
                    s_loggedOnce = true;
                    LOGF_IMPL(Utility, "KeWaitForSingleObject", 
                              "GPU init complete - all flags true, parking poll thread (count={})", s_gpuSignalCount);
                }
                // Park this thread - sleep for a long time to stop spinning
                // The game's GPU poll thread will be effectively disabled
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                return STATUS_SUCCESS;
            }
            
            // Fake GPU consumption: set RP to ring buffer size
            if (g_gpuRingBuffer.writebackEnabled && g_gpuRingBuffer.readPtrWritebackAddr != 0)
            {
                uint32_t* writebackPtr = reinterpret_cast<uint32_t*>(
                    g_memory.Translate(g_gpuRingBuffer.readPtrWritebackAddr));
                if (writebackPtr)
                {
                    uint32_t fakeRP = g_gpuRingBuffer.ringBufferSize;
                    *writebackPtr = ByteSwap(fakeRP);
                }
            }
            
            // Signal the event
            if (Object->Type == 0 || Object->Type == 1)
            {
                QueryKernelObject<Event>(*Object)->Set();
            }
            
            // Fire graphics interrupt callback once and set interruptSeen flag
            if (!g_gpuRingBuffer.interruptFired && g_gpuRingBuffer.interruptCallback != 0)
            {
                g_gpuRingBuffer.interruptFired = true;
                g_gpuRingBuffer.interruptSeen = true;
                LOGF_IMPL(Utility, "KeWaitForSingleObject", 
                          "Firing graphics interrupt callback 0x{:08X} - interruptSeen = true", 
                          g_gpuRingBuffer.interruptCallback);
            }
            
            // After many iterations, add a small delay to prevent CPU spinning
            if (s_gpuSignalCount > 1000)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            
            return STATUS_SUCCESS;
        }
        
    }

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

static std::vector<size_t> g_tlsFreeIndices;
static size_t g_tlsNextIndex = 0;
static Mutex g_tlsAllocationMutex;

static uint32_t& KeTlsGetValueRef(size_t index)
{
    // Having this a global thread_local variable
    // for some reason crashes on boot in debug builds.
    thread_local std::vector<uint32_t> s_tlsValues;

    if (s_tlsValues.size() <= index)
    {
        s_tlsValues.resize(index + 1, 0);
    }

    return s_tlsValues[index];
}

uint32_t KeTlsGetValue(uint32_t dwTlsIndex)
{
    return KeTlsGetValueRef(dwTlsIndex);
}

uint32_t KeTlsSetValue(uint32_t dwTlsIndex, uint32_t lpTlsValue)
{
    KeTlsGetValueRef(dwTlsIndex) = lpTlsValue;
    return TRUE;
}

uint32_t KeTlsAlloc()
{
    std::lock_guard<Mutex> lock(g_tlsAllocationMutex);
    if (!g_tlsFreeIndices.empty())
    {
        size_t index = g_tlsFreeIndices.back();
        g_tlsFreeIndices.pop_back();
        return index;
    }

    return g_tlsNextIndex++;
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

void NetDll_WSAStartup()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_WSACleanup()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_socket()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_closesocket()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_setsockopt()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_bind()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_connect()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_listen()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_accept()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_select()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_recv()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_send()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_inet_addr()
{
    LOG_UTILITY("!!! STUB !!!");
}

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
    
    // Log semaphore creation - EXPERIMENT ROLLED BACK (force signal only helped once)
    if (s_count <= 50 || (handle >= 0xEB2D0000 && handle <= 0xEB2E0000)) {
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

void NetDll_XNetStartup()
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
    
    // Log semaphore initialization to trace blocking semaphores
    uint32_t semAddr = (uint32_t)((uint8_t*)semaphore - g_memory.base);
    if (s_count <= 30 || (semAddr >= 0xEB2D0000 && semAddr <= 0xEB2E0000)) {
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

void NetDll_XNetCleanup()
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
    LOG_UTILITY("!!! STUB !!!");
    return ERROR_DEVICE_NOT_CONNECTED;
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

void NetDll_XNetQosListen()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_XNetQosLookup()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_XNetQosRelease()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_XNetServerToInAddr()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_XNetXnAddrToInAddr()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_XNetGetConnectStatus()
{
    LOG_UTILITY("!!! STUB !!!");
}

void NetDll_XNetUnregisterInAddr()
{
    LOG_UTILITY("!!! STUB !!!");
}

// =============================================================================
// GTA IV: Bypass dirty disc error display only
// sub_82192100 - dirty disc error display UI - HOOK to skip UI
// sub_829A1290 - another dirty disc caller
// sub_827F0B20 - retry loop - let it run but limit retries
// sub_829A1F00 - the actual file loading function
// =============================================================================

// Hook sub_827DAE40 - Worker thread entry point (streaming workers)
// This function calls an indirect task function that may be stuck
extern "C" void __imp__sub_827DAE40(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827DAE40)
{
    static int s_count = 0;
    ++s_count;
    
    uint32_t context = ctx.r3.u32;  // Worker context pointer
    
    // Read the task function pointer from the context structure
    // Based on the disassembly, the function ptr is at context+80 (after 44-byte copy)
    uint32_t* ctxData = reinterpret_cast<uint32_t*>(base + context);
    uint32_t taskFunc = ByteSwap(ctxData[0]);  // First dword of context
    
    if (s_count <= 20 || s_count % 100 == 0)
    {
        LOGF_IMPL(Utility, "WorkerThread", 
                  "sub_827DAE40 ENTER #{} ctx=0x{:08X} taskFunc=0x{:08X}", 
                  s_count, context, taskFunc);
    }
    
    __imp__sub_827DAE40(ctx, base);
    
    if (s_count <= 20 || s_count % 100 == 0)
    {
        LOGF_IMPL(Utility, "WorkerThread", "sub_827DAE40 EXIT #{}", s_count);
    }
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
    
    // Return cached result if already initialized
    if (g_initResult.load() != -999)
    {
        ctx.r3.s32 = g_initResult.load();
        if (s_count <= 10 || s_count % 100 == 0)
        {
            LOGF_WARNING("[INIT] sub_82120000 #{} CACHED r3={}", s_count, ctx.r3.s32);
        }
        return;
    }
    
    LOGF_WARNING("[INIT] sub_82120000 #{} thread=0x{:04X} (first call)", s_count, threadId);
    
    if constexpr (USE_GAME_INIT_MODULE)
    {
        // Use the new GameInit module which replaces Xbox 360-specific behavior
        ctx.r3.u32 = GameInit::Initialize(ctx, base);
    }
    else
    {
        // Use original PPC code (current behavior for development/testing)
        __imp__sub_82120000(ctx, base);
    }
    
    // Cache the result
    g_initResult.store(ctx.r3.s32);
    
    LOGF_WARNING("[INIT] sub_82120000 #{} EXIT r3={} (cached)", s_count, ctx.r3.s32);
}

// Trace functions called after sub_8218C600 succeeds
extern "C" void __imp__sub_82120EE8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_821250B0(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82318F60(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82124080(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82120FB8(PPCContext& ctx, uint8_t* base);

PPC_FUNC(sub_82120EE8)
{
    static int s_count = 0;
    ++s_count;
    LOGF_WARNING("[INIT] sub_82120EE8 ENTER #{}", s_count);
    __imp__sub_82120EE8(ctx, base);
    LOGF_WARNING("[INIT] sub_82120EE8 EXIT #{}", s_count);
}

// Trace functions inside sub_82120EE8 to find the blocker
extern "C" void __imp__sub_821207B0(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82673718(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82269098(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_822054F8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_821DE390(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_8221F8A8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82273988(PPCContext& ctx, uint8_t* base);

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
extern "C" void __imp__sub_8249BE88(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_827EDED0(PPCContext& ctx, uint8_t* base);
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

PPC_FUNC(sub_821207B0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_821207B0 ENTER #{}", s_count);
    __imp__sub_821207B0(ctx, base);
    LOGF_WARNING("[INIT] sub_821207B0 EXIT #{}", s_count);
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

PPC_FUNC(sub_82974F90) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_82974F90 ENTER #{}", s_count);
    __imp__sub_82974F90(ctx, base);
    LOGF_WARNING("[INIT] sub_82974F90 EXIT #{}", s_count);
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

PPC_FUNC(sub_8298E810) {
    static int s_count = 0; ++s_count;
    // r3=context struct, r4=task function, r5=stack size, r6=flags
    uint32_t contextAddr = ctx.r3.u32;
    uint32_t taskFunc = ctx.r4.u32;
    LOGF_WARNING("[INIT_WORKER] sub_8298E810 #{} ENTER ctx=0x{:08X} taskFunc=0x{:08X}", 
                 s_count, contextAddr, taskFunc);
    __imp__sub_8298E810(ctx, base);
    LOGF_WARNING("[INIT_WORKER] sub_8298E810 #{} EXIT result={}", s_count, ctx.r3.u32);
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
    
    // Log ALL semaphore creations during init to trace the flow
    if (s_count <= 30) {
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
    
    // Log ALL semaphore waits for first 50 calls to trace init flow
    if (s_count <= 50) {
        LOGF_WARNING("[SEM_WAIT] sub_827DACD8 #{} ENTERING wait on semaphore 0x{:08X} (LR=0x{:08X})", 
                     s_count, semHandle, (uint32_t)ctx.lr);
    }
    
    __imp__sub_827DACD8(ctx, base);
    
    if (s_count <= 50) {
        LOGF_WARNING("[SEM_WAIT] sub_827DACD8 #{} PASSED wait on semaphore 0x{:08X}", s_count, semHandle);
    }
}

// Render worker task function - trace entry and progress
extern "C" void __imp__sub_827DE858(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827DE858) {
    static int s_count = 0; ++s_count;
    uint32_t workerId = ctx.r3.u32;
    LOGF_WARNING("[RENDER WORKER] sub_827DE858 ENTER #{} workerId={}", s_count, workerId);
    __imp__sub_827DE858(ctx, base);
    LOGF_WARNING("[RENDER WORKER] sub_827DE858 EXIT #{}", s_count);
}

// Helper to mark init complete - called when sub_82673718 exits
void MarkInitComplete() {
    // This will be called from sub_82673718 hook when it exits
    extern void SetInitComplete();
}

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

// Deeper tracing for sub_82192140 internals
extern "C" void __imp__sub_827E0C30(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E0C30) {
    static int s_count = 0; ++s_count;
    if (s_count <= 20) LOGF_WARNING("[PATH] sub_827E0C30 ENTER #{} r3=0x{:08X}", s_count, ctx.r3.u32);
    __imp__sub_827E0C30(ctx, base);
    if (s_count <= 20) LOGF_WARNING("[PATH] sub_827E0C30 EXIT #{}", s_count);
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

// Address for our custom vtable[27] implementation
// This must be within valid code range: 0x82120000 to 0x82A13D5C (PPC_CODE_BASE + PPC_CODE_SIZE)
// Using a high address within range that's unlikely to conflict with game code
constexpr uint32_t STORAGE_DEVICE_READ_ADDR = 0x82A13D00;

// =============================================================================
// StorageDevice_ReadFile - vtable[27] implementation
// =============================================================================
// Called by sub_8249BE88 to read file data from storage device
// Parameters:
//   r3 = storage device pointer
//   r4 = path buffer (null-terminated string in guest memory)
//   r5 = output pointer (where to write result data pointer)
// Returns:
//   r3 = bytes read (or 0 if file not found)
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
    
    if (s_count <= 20) {
        LOGF_WARNING("[STORAGE] vtable[27] ReadFile #{} path='{}' device=0x{:08X} output=0x{:08X}",
                     s_count, pathBuf, devicePtr, outputAddr);
    }
    
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
        LOGF_WARNING("[STORAGE] vtable[27] ReadFile #{} -> found, size={} bytes", s_count, fileSize);
    }
    
    // For now, we return the file size as "bytes read"
    // The output pointer gets the path length or a handle value
    // Based on sub_8249BE88 usage, it loads [output] into r4 for sub_8249BDC8
    if (outputAddr != 0) {
        // Write path length or handle value
        PPC_STORE_U32(outputAddr, static_cast<uint32_t>(fileSize));
    }
    
    ctx.r3.s64 = static_cast<int64_t>(fileSize);  // Return bytes read
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

// Initialize PC storage device structure in guest memory
static void InitializePCStorageDevice(uint8_t* base) {
    if (s_pcStorageInitialized) return;
    
    LOG_WARNING("[STORAGE] Initializing PC storage device");
    
    // Register our vtable[27] implementation
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
    
    // vtable[27] at offset 108 - point to our ReadFile implementation
    PPC_STORE_U32(StorageConstants::PC_STORAGE_VTABLE_ADDR + 108, STORAGE_DEVICE_READ_ADDR);
    
    s_pcStorageInitialized = true;
    LOGF_WARNING("[STORAGE] PC storage device initialized, vtable[27]=0x{:08X}", STORAGE_DEVICE_READ_ADDR);
}

extern "C" void __imp__sub_827E1EC0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E1EC0) {
    static int s_count = 0; ++s_count;
    
    // Initialize PC storage device if needed
    InitializePCStorageDevice(base);
    
    // r3 = path string, r4 = flags
    uint32_t pathAddr = ctx.r3.u32;
    
    // Read path string for logging
    char pathBuf[256] = {0};
    for (int i = 0; i < 255; i++) {
        uint8_t c = PPC_LOAD_U8(pathAddr + i);
        if (c == 0) break;
        pathBuf[i] = c;
    }
    
    if (s_count <= 20) {
        LOGF_WARNING("[STORAGE] sub_827E1EC0 #{} path='{}' -> returning PC device 0x{:08X}", 
                     s_count, pathBuf, StorageConstants::PC_STORAGE_DEVICE_ADDR);
    }
    
    // Return our PC storage device for ALL paths
    // The VFS system handles path resolution internally
    ctx.r3.u32 = StorageConstants::PC_STORAGE_DEVICE_ADDR;
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

// Trace internal calls of sub_8286BAE0
extern "C" void __imp__sub_8221B7A0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8221B7A0) {
    LOG_WARNING("[sub_8286BAE0] sub_8221B7A0 ENTER");
    __imp__sub_8221B7A0(ctx, base);
    LOG_WARNING("[sub_8286BAE0] sub_8221B7A0 EXIT");
}

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
PPC_FUNC(sub_8286CCA0) {
    LOG_WARNING("[sub_8286CCA0] ENTER (shader load vtable method)");
    __imp__sub_8286CCA0(ctx, base);
    LOG_WARNING("[sub_8286CCA0] EXIT");
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

extern "C" void __imp__sub_827E04F0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827E04F0) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_8285BDC8] sub_827E04F0 ENTER #{}", s_count);
    __imp__sub_827E04F0(ctx, base);
    LOGF_WARNING("[sub_8285BDC8] sub_827E04F0 EXIT #{}", s_count);
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
    
    // The path context structure has the path string at offset 0
    // Read the path string
    char pathBuf[260] = {0};
    for (int i = 0; i < 259; i++) {
        char c = PPC_LOAD_U8(pathContextAddr + i);
        if (c == 0) break;
        pathBuf[i] = c;
    }
    
    if (s_count <= 20) {
        LOGF_WARNING("[FILE] sub_827E8180 #{} path='{}' flags={}", s_count, pathBuf, flags);
    }
    
    // Check if this is a shader file request (fxl_final)
    bool isShaderPath = (strstr(pathBuf, "fxl_final") != nullptr || 
                         strstr(pathBuf, ".fxc") != nullptr ||
                         strstr(pathBuf, ".bin") != nullptr);
    
    // For shader files, we return 0 (not found) since they're loaded from embedded cache
    // The game has fallback paths for shader loading
    if (isShaderPath) {
        if (s_count <= 20) {
            LOGF_WARNING("[FILE] sub_827E8180 #{} -> shader path, returning 0 (use cache)", s_count);
        }
        ctx.r3.u32 = 0;  // File not found - use embedded shader cache
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
    LOGF_WARNING("[sub_8285BDC8] sub_827E8880 ENTER #{}", s_count);
    __imp__sub_827E8880(ctx, base);
    LOGF_WARNING("[sub_8285BDC8] sub_827E8880 EXIT #{}", s_count);
}

extern "C" void __imp__sub_8285B680(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8285B680) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_8285BDC8] sub_8285B680 ENTER #{} (shader load)", s_count);
    __imp__sub_8285B680(ctx, base);
    LOGF_WARNING("[sub_8285BDC8] sub_8285B680 EXIT #{}", s_count);
}

extern "C" void __imp__sub_8285AA90(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8285AA90) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[sub_82851F30] sub_8285AA90 ENTER #{}", s_count);
    __imp__sub_8285AA90(ctx, base);
    LOGF_WARNING("[sub_82851F30] sub_8285AA90 EXIT #{}", s_count);
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

extern "C" void __imp__sub_82193BC0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82193BC0) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_82193BC0 ENTER #{}", s_count);
    __imp__sub_82193BC0(ctx, base);
    if (s_count <= 10) LOGF_WARNING("[sub_8218C600] sub_82193BC0 EXIT #{}", s_count);
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

extern "C" void __imp__sub_827EA150(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_827EA150) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) LOG_WARNING("[sub_827EB748] sub_827EA150 ENTER");
    __imp__sub_827EA150(ctx, base);
    if (s_count <= 5) LOG_WARNING("[sub_827EB748] sub_827EA150 EXIT");
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

// sub_8298E700 - worker thread task function that should call sub_827DAD60 at start
extern "C" void __imp__sub_8298E700(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8298E700) {
    static int s_count = 0; ++s_count;
    uint32_t ctx_ptr = ctx.r3.u32;
    
    // Read semaphore handles from context struct (offsets +36 and +40)
    uint32_t sem1 = 0, sem2 = 0;
    if (ctx_ptr >= 0x82000000 && ctx_ptr < 0x90000000) {
        sem1 = ByteSwap(*(uint32_t*)g_memory.Translate(ctx_ptr + 36)); // wait semaphore
        sem2 = ByteSwap(*(uint32_t*)g_memory.Translate(ctx_ptr + 40)); // signal semaphore
    }
    
    LOGF_WARNING("[WORKER_TASK] sub_8298E700 #{} ENTER ctx=0x{:08X} sem1(wait)=0x{:08X} sem2(signal)=0x{:08X}", 
                 s_count, ctx_ptr, sem1, sem2);
    __imp__sub_8298E700(ctx, base);
    LOGF_WARNING("[WORKER_TASK] sub_8298E700 #{} EXIT", s_count);
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

PPC_FUNC(sub_82672E50) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_82672E50 ENTER #{}", s_count);
    __imp__sub_82672E50(ctx, base);
    LOGF_WARNING("[INIT] sub_82672E50 EXIT #{}", s_count);
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

PPC_FUNC(sub_82973598) {
    static int s_count = 0; ++s_count;
    if (s_count <= 10 || s_count % 100 == 0)
        LOGF_WARNING("[BUSYWAIT] sub_82973598 #{}", s_count);
    __imp__sub_82973598(ctx, base);
    if (s_count <= 10 || s_count % 100 == 0)
        LOGF_WARNING("[BUSYWAIT] sub_82973598 #{} r3={}", s_count, ctx.r3.u32);
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
    
    if (s_count <= 20 || s_count % 500 == 0 || allocSize > 500000) {
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
        if (s_count <= 20 || s_count % 500 == 0 || allocSize > 500000) {
            LOGF_WARNING("[ALLOC] sub_8218BE28 #{} PPC alloc {} bytes -> 0x{:08X}", 
                         s_count, allocSize, ctx.r3.u32);
        }
    }
}

// sub_8296BE18 - called after sub_8218BE28
extern "C" void __imp__sub_8296BE18(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8296BE18) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_8296BE18 ENTER #{}", s_count);
    __imp__sub_8296BE18(ctx, base);
    LOGF_WARNING("[INIT] sub_8296BE18 EXIT #{}", s_count);
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

PPC_FUNC(sub_82269098) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[INIT] sub_82269098 ENTER #{}", s_count);
    __imp__sub_82269098(ctx, base);
    LOGF_WARNING("[INIT] sub_82269098 EXIT #{}", s_count);
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
    LOGF_WARNING("[INIT] sub_82273988 ENTER #{}", s_count);
    __imp__sub_82273988(ctx, base);
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

PPC_FUNC(sub_82124080)
{
    static int s_count = 0;
    ++s_count;
    LOGF_WARNING("[INIT] sub_82124080 ENTER #{} r3={} r4={}", s_count, ctx.r3.u32, ctx.r4.u32);
    __imp__sub_82124080(ctx, base);
    LOGF_WARNING("[INIT] sub_82124080 EXIT #{}", s_count);
}

// Hook sub_8218BEB0 - Calls sub_82120000, returns -1 on failure
PPC_FUNC(sub_8218BEB0)
{
    static int s_count = 0;
    ++s_count;
    uint32_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id()) & 0xFFFF;
    
    LOGF_WARNING("[INIT] sub_8218BEB0 ENTER #{} thread=0x{:04X}", s_count, threadId);
    
    __imp__sub_8218BEB0(ctx, base);
    
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
    if (s_count <= 10 || s_count % 5000 == 0)
    {
        LOGF_WARNING("[Scheduler] sub_828E0AB8 #{} thread=0x{:04X}", s_count, threadId);
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
    // Let the game control its own VBlank flags - don't force them
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
    stream[0] = 0x82A80A24;  // Valid object pointer
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
            objectPtr = 0x82A80A24;  // Use the repaired value
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
    
    // Protect against NULL handle - try to recover using last known handle
    if (handle == 0)
    {
        // If offset is in common.rpf range (0x9xxxx-0xAxxxx) and we have a cached handle, use it
        if (offset >= 0x90000 && offset <= 0xB0000 && s_lastCommonHandle != 0)
        {
            static int s_recoveryCount = 0;
            if (++s_recoveryCount <= 5)
            {
                LOGF_WARNING("[GTA4_FileLoad] NULL handle recovery: using cached handle 0x{:08X} for offset 0x{:X}",
                    s_lastCommonHandle, offset);
            }
            // Use the cached handle instead
            ctx.r3.u32 = s_lastCommonHandle;
            // Fall through to normal processing with recovered handle
        }
        else
        {
            static int s_nullReadCount = 0;
            if (++s_nullReadCount <= 10)
            {
                LOGF_WARNING("[GTA4_FileLoad] Error: Attempted read on NULL handle! Offset=0x{:X} Size=0x{:X}", offset, size);
            }
            ctx.r3.u32 = 0;  // Return 0 bytes read
            return;
        }
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
            
            // Log AFTER state
            if (count <= 20)
            {
                uint32_t afterStatus = asyncPtr[0];
                uint32_t afterInfo = asyncPtr[1];
                uint32_t eventHandle = ByteSwap(asyncPtr[2]);
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
GUEST_FUNCTION_HOOK(__imp__XamUserGetSigninState, XamUserGetSigninState);
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
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetGetEthernetLinkStatus, NetDll_XNetGetEthernetLinkStatus);
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
GUEST_FUNCTION_HOOK(__imp__NetDll_WSAStartup, NetDll_WSAStartup);
GUEST_FUNCTION_HOOK(__imp__NetDll_WSACleanup, NetDll_WSACleanup);
GUEST_FUNCTION_HOOK(__imp__NetDll_socket, NetDll_socket);
GUEST_FUNCTION_HOOK(__imp__NetDll_closesocket, NetDll_closesocket);
GUEST_FUNCTION_HOOK(__imp__NetDll_setsockopt, NetDll_setsockopt);
GUEST_FUNCTION_HOOK(__imp__NetDll_bind, NetDll_bind);
GUEST_FUNCTION_HOOK(__imp__NetDll_connect, NetDll_connect);
GUEST_FUNCTION_HOOK(__imp__NetDll_listen, NetDll_listen);
GUEST_FUNCTION_HOOK(__imp__NetDll_accept, NetDll_accept);
GUEST_FUNCTION_HOOK(__imp__NetDll_select, NetDll_select);
GUEST_FUNCTION_HOOK(__imp__NetDll_recv, NetDll_recv);
GUEST_FUNCTION_HOOK(__imp__NetDll_send, NetDll_send);
GUEST_FUNCTION_HOOK(__imp__NetDll_inet_addr, NetDll_inet_addr);
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
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetStartup, NetDll_XNetStartup);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetGetTitleXnAddr, NetDll_XNetGetTitleXnAddr);
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
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetCleanup, NetDll_XNetCleanup);
GUEST_FUNCTION_HOOK(__imp__NetDll_getsockname, NetDll_getsockname);
GUEST_FUNCTION_HOOK(__imp__NetDll_ioctlsocket, NetDll_ioctlsocket);
GUEST_FUNCTION_HOOK(__imp__NetDll_sendto, NetDll_sendto);
GUEST_FUNCTION_HOOK(__imp__NetDll_recvfrom, NetDll_recvfrom);
GUEST_FUNCTION_HOOK(__imp__NetDll_shutdown, NetDll_shutdown);
GUEST_FUNCTION_HOOK(__imp__XMsgCancelIORequest, XMsgCancelIORequest);
GUEST_FUNCTION_HOOK(__imp__XAudioGetSpeakerConfig, XAudioGetSpeakerConfig);
GUEST_FUNCTION_HOOK(__imp__XamContentSetThumbnail, XamContentSetThumbnail);
GUEST_FUNCTION_HOOK(__imp__XamInputGetKeystrokeEx, XamInputGetKeystrokeEx);
GUEST_FUNCTION_HOOK(__imp__XamSessionCreateHandle, XamSessionCreateHandle);
GUEST_FUNCTION_HOOK(__imp__XamSessionRefObjByHandle, XamSessionRefObjByHandle);
GUEST_FUNCTION_HOOK(__imp__KeSetDisableBoostThread, KeSetDisableBoostThread);
GUEST_FUNCTION_HOOK(__imp__XamCreateEnumeratorHandle, XamCreateEnumeratorHandle);
GUEST_FUNCTION_HOOK(__imp__NtDeviceIoControlFile, NtDeviceIoControlFile);
GUEST_FUNCTION_HOOK(__imp__NetDll_WSAGetLastError, NetDll_WSAGetLastError);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetQosListen, NetDll_XNetQosListen);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetQosLookup, NetDll_XNetQosLookup);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetQosRelease, NetDll_XNetQosRelease);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetServerToInAddr, NetDll_XNetServerToInAddr);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetXnAddrToInAddr, NetDll_XNetXnAddrToInAddr);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetGetConnectStatus, NetDll_XNetGetConnectStatus);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetUnregisterInAddr, NetDll_XNetUnregisterInAddr);
