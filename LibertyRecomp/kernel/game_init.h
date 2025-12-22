#pragma once

#include <cstdint>

// =============================================================================
// Game Initialization Module
// =============================================================================
// This module replaces sub_82120000 (0x82120000) and all its nested functions:
//   - sub_8218C600: Core engine init (D3D, GPU, thread pools, job system)
//   - sub_82120EE8: Game manager init (944-byte manager, 352-byte world)
//   - sub_821250B0: Memory pool allocator
//   - sub_82318F60: RAGE string table lookup
//   - sub_82124080: Profile/save subsystem init
//   - sub_82120FB8: 63 subsystem initializations
//
// The goal is to replace Xbox 360-specific behavior with modern equivalents
// while preserving all memory state the game expects.
// =============================================================================

namespace GameInitGlobals {
    
    // =========================================================================
    // sub_82120000 - Game Init Entry Point
    // =========================================================================
    // Input parameter to sub_8218C600 (computed from lis/addi)
    // lis r11,-32246 = 0x82120000, addi r3,r11,24980 = +0x6194
    constexpr uint32_t INIT_CONTEXT_ADDR   = 0x82126194;  // r3 passed to sub_8218C600
    
    // Memory pool pointer location (loaded before sub_821250B0)
    // lis r11, -32070 (0x83060000) + lwz offset -15952 (-0x3E50)
    constexpr uint32_t POOL_PTR_ADDR       = 0x8305C1B0;
    
    // String table address passed to sub_82318F60
    // lis r11, -32245 (0x82130000) + addi -28352 (-0x6EC0)
    constexpr uint32_t STRING_TABLE_ADDR   = 0x82129140;
    
    // =========================================================================
    // sub_8218C600 - Core Engine Initialization
    // =========================================================================
    // Global flag writes during core engine init
    
    // Init flag (stb 1) - lis r9, -31982 (0x83120000) + 22426
    constexpr uint32_t CORE_INIT_FLAG      = 0x8312579A;
    
    // GPU state registers (stw -1)
    // lis r9, -32088 (0x83080000) + various offsets
    constexpr uint32_t GPU_STATE_1         = 0x83084044;  // +16452
    constexpr uint32_t GPU_STATE_2         = 0x83085784;  // +22404
    
    // lis r9, -32087 (0x83090000) + 14180
    constexpr uint32_t GPU_STATE_3         = 0x83093764;
    
    // GPU buffer size (stw 64)
    // lis r8, -32088 (0x83080000) + 2592
    constexpr uint32_t GPU_BUFFER_SIZE     = 0x83080A20;
    
    // Vtable pointer storage locations
    // lis r10, -31985 (0x830F0000) + 20492
    constexpr uint32_t ENGINE_VTABLE_PTR_1 = 0x830F500C;
    constexpr uint32_t ENGINE_VTABLE_VAL_1 = 0x830350D4;  // Value stored there
    
    // lis r10, -31982 (0x83120000) + 19968 + 4
    constexpr uint32_t ENGINE_VTABLE_PTR_2 = 0x83124E04;
    constexpr uint32_t ENGINE_VTABLE_VAL_2 = 0x82128F40;  // String table vtable
    
    // +19988 + 4
    constexpr uint32_t ENGINE_VTABLE_PTR_3 = 0x83124E18;
    
    // +19684 + 4 (conditionally written if null)
    constexpr uint32_t ENGINE_VTABLE_PTR_4 = 0x83124CE8;
    constexpr uint32_t ENGINE_VTABLE_VAL_4 = 0x820009E0;  // Default if null
    
    // +19864 + 4 (conditionally written if null)
    constexpr uint32_t ENGINE_VTABLE_PTR_5 = 0x83124D9C;
    constexpr uint32_t ENGINE_VTABLE_VAL_5 = 0x820009DC;  // Default if null
    
    // =========================================================================
    // sub_82120EE8 - Game Manager Initialization
    // =========================================================================
    // Pointer to 944-byte game manager structure
    // lis r31, -31980 (0x83140000) + 29876 (0x74B4)
    constexpr uint32_t GAME_MANAGER_PTR    = 0x831474B4;
    constexpr uint32_t GAME_MANAGER_SIZE   = 944;
    
    // Pointer to 352-byte world context structure
    // lis r30, -31980 (0x83140000) + 29880 (0x74B8)
    constexpr uint32_t WORLD_CONTEXT_PTR   = 0x831474B8;
    constexpr uint32_t WORLD_CONTEXT_SIZE  = 352;
    
    // =========================================================================
    // sub_82124080 - Profile/Save Subsystem
    // =========================================================================
    // Profile context base address
    // lis r11, -31981 (0x83130000) + 31672 (0x7BB8)
    constexpr uint32_t PROFILE_CONTEXT     = 0x83137BB8;
    
    // Init flag checked at offset -1
    constexpr uint32_t PROFILE_INIT_FLAG   = 0x83137BB7;
    
    // Additional profile flags written during init
    // +31680, +31684, +31688, +31689, +31690, +31691, +31692
    constexpr uint32_t PROFILE_FLAG_1      = 0x83137BC0;  // stw 0
    constexpr uint32_t PROFILE_FLAG_2      = 0x83137BC4;  // stw 0
    constexpr uint32_t PROFILE_FLAG_3      = 0x83137BC8;  // stb 0
    constexpr uint32_t PROFILE_FLAG_4      = 0x83137BC9;  // stb (from r3)
    constexpr uint32_t PROFILE_FLAG_5      = 0x83137BCA;  // stb (from r3)
    constexpr uint32_t PROFILE_FLAG_6      = 0x83137BCB;  // stb 1
    constexpr uint32_t PROFILE_FLAG_7      = 0x83137BCC;  // stb (from r4)
    
    // =========================================================================
    // sub_82120FB8 - Subsystem Initialization
    // =========================================================================
    // Flags written at start of subsystem init
    // lis r10, -31981 (0x83130000) + various offsets
    constexpr uint32_t SUBSYS_STATE        = 0x83137654;  // +30292: stw 0
    constexpr uint32_t SUBSYS_FLAG_1       = 0x83137BB4;  // +31668: stb 0
    constexpr uint32_t SUBSYS_FLAG_2       = 0x83137BB6;  // +31670: stb 0
    
    // =========================================================================
    // Allocation Structure Offsets
    // =========================================================================
    // Memory pool returned by sub_821250B0 has this layout:
    // offset 0:  uint32_t field_0  (set to 0)
    // offset 4:  uint32_t field_4  (set to 0)
    // offset 8:  uint32_t field_8  (set to string table result)
    // offset 12: uint32_t field_C  (set to -1)
    
    // =========================================================================
    // Subsystem Function Addresses (for reference)
    // =========================================================================
    // These are the 63 subsystems initialized by sub_82120FB8
    // They are called in order; most are pure game logic
    constexpr uint32_t SUBSYS_COUNT = 63;
    
} // namespace GameInitGlobals

// =============================================================================
// Function Declarations
// =============================================================================

struct PPCContext;

namespace GameInit {
    
    // -------------------------------------------------------------------------
    // Phase 1: Core Engine (replaces sub_8218C600)
    // -------------------------------------------------------------------------
    // Sets up D3D/GPU, thread pools, job system, render buffers
    // Returns: true on success
    bool InitCoreEngine(PPCContext& ctx, uint8_t* base);
    
    // -------------------------------------------------------------------------
    // Phase 2: Game Manager (replaces sub_82120EE8)
    // -------------------------------------------------------------------------
    // Allocates 944-byte game manager and 352-byte world context
    // Initializes audio, streaming, entity, physics, AI, script systems
    void InitGameManager(PPCContext& ctx, uint8_t* base);
    
    // -------------------------------------------------------------------------
    // Phase 3: Memory Pool (wraps sub_821250B0)
    // -------------------------------------------------------------------------
    // Allocates from the game's memory pool
    // Returns: pointer to allocated slot
    uint32_t AllocateFromPool(PPCContext& ctx, uint8_t* base, uint32_t poolPtr);
    
    // -------------------------------------------------------------------------
    // Phase 4: Profile/Save (replaces sub_82124080)
    // -------------------------------------------------------------------------
    // Initializes profile and save system using VFS instead of XContent
    void InitProfileSystem(PPCContext& ctx, uint8_t* base);
    
    // -------------------------------------------------------------------------
    // Phase 5: Subsystems (replaces sub_82120FB8)
    // -------------------------------------------------------------------------
    // Initializes all 63 game subsystems
    void InitSubsystems(PPCContext& ctx, uint8_t* base);
    
    // -------------------------------------------------------------------------
    // Main Entry Point (replaces sub_82120000)
    // -------------------------------------------------------------------------
    // Orchestrates all initialization phases
    // Returns: 1 on success, 0 on failure
    uint32_t Initialize(PPCContext& ctx, uint8_t* base);
    
} // namespace GameInit

