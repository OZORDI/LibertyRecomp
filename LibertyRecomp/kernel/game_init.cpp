// =============================================================================
// Game Initialization Module
// =============================================================================
// This module replaces sub_82120000 (0x82120000) and its nested functions.
// 
// Original call tree:
//   sub_82120000 (Game Init)
//   ├── sub_8218C600  → InitCoreEngine()
//   ├── sub_82120EE8  → InitGameManager()
//   ├── sub_821250B0  → AllocateFromPool() [kept as-is, pure logic]
//   ├── sub_82318F60  → [kept as-is, string table lookup]
//   ├── sub_82124080  → InitProfileSystem()
//   └── sub_82120FB8  → InitSubsystems()
//
// Strategy:
// - Replace Xbox 360-specific behavior with modern equivalents
// - Preserve all memory writes the game expects
// - Keep pure game logic functions (call original PPC)
// - Skip timing-dependent stalls and thread pool waits
// =============================================================================

#include <cstdint>
#include <cstdio>
#include <atomic>
#include <thread>

#include <stdafx.h>
#include <cpu/ppc_context.h>
#include <os/logger.h>

#include "game_init.h"

// =============================================================================
// External PPC Function Declarations
// =============================================================================
// Only the functions we actually call in the current implementation.
// Additional declarations can be added when we implement direct replacement.

// Main initialization functions (currently called directly)
extern "C" void __imp__sub_8218C600(PPCContext& ctx, uint8_t* base);  // Core engine init
extern "C" void __imp__sub_82120EE8(PPCContext& ctx, uint8_t* base);  // Game manager init
extern "C" void __imp__sub_821250B0(PPCContext& ctx, uint8_t* base);  // Memory pool alloc
extern "C" void __imp__sub_82318F60(PPCContext& ctx, uint8_t* base);  // String table lookup
extern "C" void __imp__sub_82124080(PPCContext& ctx, uint8_t* base);  // Profile/save init
extern "C" void __imp__sub_82120FB8(PPCContext& ctx, uint8_t* base);  // Subsystem init (63)

// =============================================================================
// Helper Macros
// =============================================================================
// ByteSwap is provided by XenonUtils/byteswap.h via stdafx.h

#define PPC_STORE_U8(addr, val) \
    *(uint8_t*)(base + (addr)) = (uint8_t)(val)

#define PPC_STORE_U16(addr, val) \
    *(uint16_t*)(base + (addr)) = ByteSwap((uint16_t)(val))

#define PPC_STORE_U32(addr, val) \
    *(uint32_t*)(base + (addr)) = ByteSwap((uint32_t)(val))

#define PPC_LOAD_U32(addr) \
    ByteSwap(*(uint32_t*)(base + (addr)))

// =============================================================================
// Phase 1: InitCoreEngine (replaces sub_8218C600)
// =============================================================================
// 
// Original sub_8218C600 does:
// 1. Thread priority setup (sub_829A0A48)
// 2. Global flag writes
// 3. Thread pool setup (sub_827DF248)
// 4. Thread initialization (sub_82192578)
// 5. GPU availability check (sub_82850AF0)
// 6. GPU init mode (sub_82850B60)
// 7. Allocate render context (sub_8218BE28, 472 bytes)
// 8. Render buffer init (sub_82857028)
// 9. GPU mode setup (sub_82851548)
// 10. GPU context setup (sub_82856C38)
// 11. GPU resource setup (sub_8285A0E0)
// 12. GPU buffer creation (sub_82850748)
// 13. GPU state init (sub_82856C90)
// 14. Vtable call
// 15. World init (sub_82285F90)
// 16. Time system (sub_8219FC80)
// 17. Streaming init (sub_822214E0)
// 18. String table init (sub_823193A8)
// 19. Config init (sub_821EC3E8)
// 20. Allocate file system (sub_8218BE28, 48 bytes)
// 21. File system construct (sub_827EB6E0)
// 22. Archive scan (sub_827EED88)
// 23. Archive mount (sub_827EB748)
// 24. Resource manager (sub_827EEB48)
// 25. Job system (sub_82193BC0)
// 26. Worker startup (sub_82197338)
// 27. Returns 1 on success
//
// Our replacement:
// - Writes all global memory state
// - Calls pure game logic functions
// - Skips Xbox-specific thread pool timing
// - Returns 1 (success)
// =============================================================================

namespace GameInit {

bool InitCoreEngine(PPCContext& ctx, uint8_t* base)
{
    LOG_WARNING("[GameInit] Phase 1: InitCoreEngine starting...");
    
    // -------------------------------------------------------------------------
    // Call original sub_8218C600
    // -------------------------------------------------------------------------
    // The PPC function handles all global writes internally:
    //   - CORE_INIT_FLAG (0x8312579A) = 1
    //   - GPU_STATE_1/2/3 = -1
    //   - GPU_BUFFER_SIZE = 64
    //   - Engine vtable pointers
    //   - Thread pool, GPU, file system, job system init
    //
    // We pass the correct init context address computed from:
    //   lis r11,-32246 = 0x82120000, addi r3,r11,24980 = 0x82126194
    // -------------------------------------------------------------------------
    ctx.r3.u64 = GameInitGlobals::INIT_CONTEXT_ADDR;
    __imp__sub_8218C600(ctx, base);
    
    bool success = (ctx.r3.u32 & 0xFF) != 0;
    LOGF_WARNING("[GameInit] Phase 1: InitCoreEngine {} (r3={})", 
                 success ? "SUCCEEDED" : "FAILED", ctx.r3.u32);
    
    return success;
}

// =============================================================================
// Phase 2: InitGameManager (replaces sub_82120EE8)
// =============================================================================

void InitGameManager(PPCContext& ctx, uint8_t* base)
{
    LOG_WARNING("[GameInit] Phase 2: InitGameManager starting...");
    
    // For now, call the original function
    // TODO: Replace with direct implementation to avoid audio blocking
    __imp__sub_82120EE8(ctx, base);
    
    LOG_WARNING("[GameInit] Phase 2: InitGameManager completed");
}

// =============================================================================
// Phase 3: AllocateFromPool (wraps sub_821250B0)
// =============================================================================

uint32_t AllocateFromPool(PPCContext& ctx, uint8_t* base, uint32_t poolPtr)
{
    ctx.r3.u64 = poolPtr;
    __imp__sub_821250B0(ctx, base);
    return ctx.r3.u32;
}

// =============================================================================
// Phase 4: InitProfileSystem (replaces sub_82124080)
// =============================================================================

void InitProfileSystem(PPCContext& ctx, uint8_t* base)
{
    LOG_WARNING("[GameInit] Phase 4: InitProfileSystem starting...");
    
    // For now, call the original function
    // TODO: Replace XContent APIs with VFS-based save system
    ctx.r3.s64 = 1;  // r3 = 1
    ctx.r4.s64 = 0;  // r4 = 0
    __imp__sub_82124080(ctx, base);
    
    LOG_WARNING("[GameInit] Phase 4: InitProfileSystem completed");
}

// =============================================================================
// Phase 5: InitSubsystems (replaces sub_82120FB8)
// =============================================================================

void InitSubsystems(PPCContext& ctx, uint8_t* base)
{
    LOG_WARNING("[GameInit] Phase 5: InitSubsystems starting...");
    
    // -------------------------------------------------------------------------
    // Set subsystem flags
    // -------------------------------------------------------------------------
    PPC_STORE_U32(GameInitGlobals::SUBSYS_STATE, 0);
    PPC_STORE_U8(GameInitGlobals::SUBSYS_FLAG_1, 0);
    PPC_STORE_U8(GameInitGlobals::SUBSYS_FLAG_2, 0);
    
    // -------------------------------------------------------------------------
    // Call original sub_82120FB8 which initializes 63 subsystems
    // Most are pure game logic and should work as-is
    // -------------------------------------------------------------------------
    __imp__sub_82120FB8(ctx, base);
    
    LOG_WARNING("[GameInit] Phase 5: InitSubsystems completed");
}

// =============================================================================
// Main Entry Point: Initialize (replaces sub_82120000)
// =============================================================================

uint32_t Initialize(PPCContext& ctx, uint8_t* base)
{
    LOG_WARNING("[GameInit] ============================================");
    LOG_WARNING("[GameInit] Game Initialization Starting");
    LOG_WARNING("[GameInit] ============================================");
    
    // -------------------------------------------------------------------------
    // Phase 1: Core Engine Init
    // -------------------------------------------------------------------------
    if (!InitCoreEngine(ctx, base)) {
        LOG_WARNING("[GameInit] FATAL: InitCoreEngine failed!");
        return 0;
    }
    
    // -------------------------------------------------------------------------
    // Phase 2: Game Manager Init
    // -------------------------------------------------------------------------
    InitGameManager(ctx, base);
    
    // -------------------------------------------------------------------------
    // Phase 3: Memory Pool Allocation
    // -------------------------------------------------------------------------
    LOG_WARNING("[GameInit] Phase 3: Memory pool allocation...");
    uint32_t poolPtr = PPC_LOAD_U32(GameInitGlobals::POOL_PTR_ADDR);
    uint32_t allocResult = AllocateFromPool(ctx, base, poolPtr);
    LOGF_WARNING("[GameInit] Allocated from pool: 0x{:08X}", allocResult);
    
    // Set up the allocated structure
    // offset 0: field_0 = 0
    // offset 4: field_4 = 0
    // offset 8: field_8 = string table result
    // offset 12: field_C = -1
    PPC_STORE_U32(allocResult + 0, 0);
    PPC_STORE_U32(allocResult + 4, 0);
    
    // Get string table pointer
    ctx.r3.u64 = GameInitGlobals::STRING_TABLE_ADDR;
    __imp__sub_82318F60(ctx, base);
    uint32_t stringTable = ctx.r3.u32;
    
    PPC_STORE_U32(allocResult + 8, stringTable);
    PPC_STORE_U32(allocResult + 12, 0xFFFFFFFF);
    
    // -------------------------------------------------------------------------
    // Phase 4: Profile/Save Init
    // -------------------------------------------------------------------------
    InitProfileSystem(ctx, base);
    
    // -------------------------------------------------------------------------
    // Phase 5: Subsystem Init
    // -------------------------------------------------------------------------
    InitSubsystems(ctx, base);
    
    LOG_WARNING("[GameInit] ============================================");
    LOG_WARNING("[GameInit] Game Initialization Complete - SUCCESS");
    LOG_WARNING("[GameInit] ============================================");
    
    return 1;  // Success
}

} // namespace GameInit

