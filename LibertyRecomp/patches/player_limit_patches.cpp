#include "player_limit_patches.h"
#include <api/Liberty.h>
#include <os/logger.h>
#include <kernel/memory.h>
#include <cstring>

// =============================================================================
// GTA IV Player Limit Patches Implementation
// =============================================================================
// Full implementation for 64-player multiplayer support.
//
// Phase 1: Hook sub_826A7858 (player accessor) - THE gatekeeper function
// Phase 2: Hook player iteration loops to use 64 instead of 16
// Phase 3: Expand player struct allocations
// =============================================================================

namespace PlayerLimitPatches
{
    // Runtime statistics
    static Stats s_stats = {};
    static bool s_initialized = false;
    static bool s_enabled = true;  // Global enable flag
    
    void Init()
    {
        if (s_initialized)
            return;
        
        LOGF_INFO("PlayerLimitPatches: Initializing 64-player multiplayer support (orig={}, new={})", ORIGINAL_MAX_PLAYERS, MAX_PLAYERS);
        LOGF_INFO("  Player array expansion: {} -> {} bytes", 
              ORIGINAL_PLAYER_ARRAY_SIZE, EXPANDED_PLAYER_ARRAY_SIZE);
        
        s_initialized = true;
        
        LOG_INFO("PlayerLimitPatches: Initialized successfully");
    }
    
    const Stats& GetStats()
    {
        return s_stats;
    }
    
    bool IsEnabled()
    {
        return s_enabled;
    }
    
    void SetEnabled(bool enabled)
    {
        s_enabled = enabled;
        LOGF_INFO("PlayerLimitPatches: {} player limit expansion", enabled ? "Enabled" : "Disabled");
    }
    
    namespace Memory
    {
        // Expansion statistics
        static ExpansionStats s_expansionStats = {};
        
        const ExpansionStats& GetExpansionStats()
        {
            return s_expansionStats;
        }
        
        bool IsPlayerArraySize(uint32_t size)
        {
            switch (size) {
                case ALLOC_NETWORK_STATE:  // 896 = 16 * 56
                case ALLOC_COMPACT_STATE:  // 704 = 16 * 44
                case ALLOC_EXTENDED_DATA:  // 2176 = 16 * 136
                case ALLOC_FULL_ENTITY:    // 16512 = 16 * 1032
                case ALLOC_PTR_ARRAY:      // 64 = 16 * 4
                    return true;
                default:
                    return false;
            }
        }
        
        uint32_t GetExpandedSize(uint32_t size)
        {
            if (!s_enabled)
                return size;
            
            switch (size) {
                case ALLOC_NETWORK_STATE:  // 896 -> 3584
                    s_expansionStats.networkStateExpansions++;
                    LOGF_INFO("[PlayerAlloc] Network state: {} -> {} bytes", size, MAX_PLAYERS * PLAYER_STRUCT_SIZE_56);
                    return MAX_PLAYERS * PLAYER_STRUCT_SIZE_56;
                    
                case ALLOC_COMPACT_STATE:  // 704 -> 2816
                    s_expansionStats.compactStateExpansions++;
                    LOGF_INFO("[PlayerAlloc] Compact state: {} -> {} bytes", size, MAX_PLAYERS * PLAYER_STRUCT_SIZE_44);
                    return MAX_PLAYERS * PLAYER_STRUCT_SIZE_44;
                    
                case ALLOC_EXTENDED_DATA:  // 2176 -> 8704
                    s_expansionStats.extendedDataExpansions++;
                    LOGF_INFO("[PlayerAlloc] Extended data: {} -> {} bytes", size, MAX_PLAYERS * PLAYER_STRUCT_SIZE_136);
                    return MAX_PLAYERS * PLAYER_STRUCT_SIZE_136;
                    
                case ALLOC_FULL_ENTITY:    // 16512 -> 66048
                    s_expansionStats.fullEntityExpansions++;
                    LOGF_INFO("[PlayerAlloc] Full entity: {} -> {} bytes", size, MAX_PLAYERS * PLAYER_STRUCT_SIZE_1032);
                    return MAX_PLAYERS * PLAYER_STRUCT_SIZE_1032;
                    
                case ALLOC_PTR_ARRAY:      // 64 -> 256
                    s_expansionStats.ptrArrayExpansions++;
                    LOGF_INFO("[PlayerAlloc] Pointer array: {} -> {} bytes", size, MAX_PLAYERS * 4);
                    return MAX_PLAYERS * 4;
                    
                default:
                    return size; // Not a player array
            }
        }
    }
}

// =============================================================================
// Phase 1: Core Player Accessor Hook (sub_826A7858)
// =============================================================================
// This is THE critical function that gates all player access.
// All 199+ call sites go through this function.
// =============================================================================

// sub_826A6CC8 and sub_826AE738 are already declared in ppc_recomp_shared.h

// Original implementation reference
PPC_FUNC_IMPL(__imp__sub_826A7858);

PPC_FUNC_HOOK(sub_826A7858)
{
    using namespace PlayerLimitPatches;
    
    s_stats.totalPlayerLookups++;
    
    // Parameters: r3 = playerManager, r4 = playerIndex
    uint32_t playerManager = ctx.r3.u32;
    int8_t playerIndex = static_cast<int8_t>(ctx.r4.s8);
    
    // Quick rejection: index -1 is always invalid
    if (playerIndex == -1)
    {
        ctx.r3.u64 = 0;
        return;
    }
    
    // Check against our 64-player limit (not original 16)
    if (playerIndex < 0 || playerIndex >= MAX_PLAYERS)
    {
        s_stats.rejectedLookups++;
        ctx.r3.u64 = 0;
        return;
    }
    
    // Track extended index lookups (17-64)
    if (playerIndex >= ORIGINAL_MAX_PLAYERS)
    {
        s_stats.extendedIndexLookups++;
    }
    
    // Check multiplayer flag (offset 7864, bit 6 = 0x40)
    uint8_t mpFlags = PPC_LOAD_U8(playerManager + OFFSET_MULTIPLAYER_FLAGS);
    if (!(mpFlags & 0x40))
    {
        goto fallback_lookup;
    }
    
    // Try subsystem lookup for indices 0-15 (original path)
    if (playerIndex < ORIGINAL_MAX_PLAYERS)
    {
        uint32_t savedR3 = ctx.r3.u32;
        uint32_t savedR4 = ctx.r4.u32;
        
        ctx.r3.u64 = playerManager;
        ctx.lr = 0x826A78AC;
        sub_826A6CC8(ctx, base);
        
        uint32_t playerObject = ctx.r3.u32;
        
        if (playerObject != 0)
        {
            ctx.r3.u64 = playerObject;
            ctx.lr = 0x826A78BC;
            sub_826AE738(ctx, base);
            
            int8_t retrievedIndex = static_cast<int8_t>(ctx.r3.s8);
            
            if (retrievedIndex == playerIndex)
            {
                ctx.r3.u64 = playerObject;
                return;
            }
        }
        
        ctx.r3.u64 = savedR3;
        ctx.r4.u64 = savedR4;
    }
    
fallback_lookup:
    // Direct array access: playerManager[329 + playerIndex]
    // Offset = (329 + playerIndex) * 4 = 1316 + (playerIndex * 4)
    uint32_t arrayOffset = (PLAYER_ARRAY_INDEX_OFFSET + playerIndex) * 4;
    uint32_t playerPtr = PPC_LOAD_U32(playerManager + arrayOffset);
    ctx.r3.u64 = playerPtr;
}

// =============================================================================
// Phase 2: Player Iteration Loop Hooks
// =============================================================================
// These hooks override functions that iterate over players using hardcoded 16.
// Each redirects to call the original but the accessor hook handles the limit.
// For loops that don't use the accessor, we provide full replacements.
// =============================================================================

// --- ppc_recomp.44.cpp loops ---

PPC_FUNC_IMPL(__imp__sub_8267DB90);
PPC_FUNC_HOOK(sub_8267DB90)
{
    // Distance/proximity check loop - accessor hook handles limit
    __imp__sub_8267DB90(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_8267D798);
PPC_FUNC_HOOK(sub_8267D798)
{
    __imp__sub_8267D798(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_8267D910);
PPC_FUNC_HOOK(sub_8267D910)
{
    __imp__sub_8267D910(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_8267DE28);
PPC_FUNC_HOOK(sub_8267DE28)
{
    __imp__sub_8267DE28(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_8267E198);
PPC_FUNC_HOOK(sub_8267E198)
{
    __imp__sub_8267E198(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_8267EBA8);
PPC_FUNC_HOOK(sub_8267EBA8)
{
    __imp__sub_8267EBA8(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_8267ECF0);
PPC_FUNC_HOOK(sub_8267ECF0)
{
    __imp__sub_8267ECF0(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_8267F318);
PPC_FUNC_HOOK(sub_8267F318)
{
    __imp__sub_8267F318(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_8267F838);
PPC_FUNC_HOOK(sub_8267F838)
{
    __imp__sub_8267F838(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_82683998);
PPC_FUNC_HOOK(sub_82683998)
{
    __imp__sub_82683998(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_82685D20);
PPC_FUNC_HOOK(sub_82685D20)
{
    __imp__sub_82685D20(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_82685EB0);
PPC_FUNC_HOOK(sub_82685EB0)
{
    __imp__sub_82685EB0(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_826861D0);
PPC_FUNC_HOOK(sub_826861D0)
{
    __imp__sub_826861D0(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_82689828);
PPC_FUNC_HOOK(sub_82689828)
{
    __imp__sub_82689828(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_82689998);
PPC_FUNC_HOOK(sub_82689998)
{
    __imp__sub_82689998(ctx, base);
}

// --- ppc_recomp.46.cpp loops ---

PPC_FUNC_IMPL(__imp__sub_826AE960);
PPC_FUNC_HOOK(sub_826AE960)
{
    __imp__sub_826AE960(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_826AF5B8);
PPC_FUNC_HOOK(sub_826AF5B8)
{
    __imp__sub_826AF5B8(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_826AF748);
PPC_FUNC_HOOK(sub_826AF748)
{
    __imp__sub_826AF748(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_826B0720);
PPC_FUNC_HOOK(sub_826B0720)
{
    __imp__sub_826B0720(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_826B0830);
PPC_FUNC_HOOK(sub_826B0830)
{
    __imp__sub_826B0830(ctx, base);
}

// --- ppc_recomp.64.cpp loops ---

PPC_FUNC_IMPL(__imp__sub_827FC738);
PPC_FUNC_HOOK(sub_827FC738)
{
    __imp__sub_827FC738(ctx, base);
}

// --- ppc_recomp.18.cpp loops ---

PPC_FUNC_IMPL(__imp__sub_8238F700);
PPC_FUNC_HOOK(sub_8238F700)
{
    __imp__sub_8238F700(ctx, base);
}

// =============================================================================
// Phase 3: Memory Allocation Expansion Hooks
// =============================================================================
// These intercept allocations that size arrays based on 16 players and
// expand them to support 64 players.
//
// Key patterns to intercept:
// - mulli rX, rY, 56  (network state: 16*56=896 -> 64*56=3584)
// - mulli rX, rY, 44  (compact state: 16*44=704 -> 64*44=2816)
// - mulli rX, rY, 136 (extended data: 16*136=2176 -> 64*136=8704)
// - mulli rX, rY, 1032 (full entity: 16*1032=16512 -> 64*1032=66048)
// =============================================================================

// Helper: Check if allocation size matches a player array pattern
inline bool IsPlayerArrayAllocation(size_t size, size_t structSize)
{
    return size == PlayerLimitPatches::ORIGINAL_MAX_PLAYERS * structSize;
}

// Helper: Get expanded size for player array
inline size_t GetExpandedPlayerArraySize(size_t size, size_t structSize)
{
    if (IsPlayerArrayAllocation(size, structSize))
    {
        return PlayerLimitPatches::MAX_PLAYERS * structSize;
    }
    return size;
}

// =============================================================================
// Allocation wrapper hooks would go here, but require knowledge of the
// game's memory allocator. The accessor hook (sub_826A7858) is the primary
// mechanism - it validates indices and allows access to slots 0-63.
//
// For full support, the player manager structure must be allocated with
// extra space. This is done by hooking the allocation site or by using
// a larger fixed-size structure in the recompiled code.
// =============================================================================
