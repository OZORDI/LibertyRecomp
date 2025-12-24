#pragma once

#include <cstdint>

// =============================================================================
// GTA IV Player Limit Patches
// =============================================================================
// Increases the multiplayer player limit from 16 to 64 players.
// 
// Original Xbox 360 limit: 16 players
// New fixed limit: 64 players
//
// Key patched function: sub_826A7858 (player object accessor)
// =============================================================================

namespace PlayerLimitPatches
{
    // Player limit constants - FIXED at 64 for matchmaking compatibility
    static constexpr int32_t ORIGINAL_MAX_PLAYERS = 16;
    static constexpr int32_t MAX_PLAYERS = 64;  // Fixed, non-configurable
    
    // Player manager structure offsets (from reverse engineering)
    static constexpr uint32_t OFFSET_MULTIPLAYER_FLAGS = 7864;  // Byte with bit 6 = MP active
    static constexpr uint32_t OFFSET_PLAYER_ARRAY_BASE = 1316;  // 329 * 4 = player pointer array
    static constexpr uint32_t PLAYER_ARRAY_INDEX_OFFSET = 329;  // Added to player index for array access
    
    // Player struct sizes for allocation expansion
    static constexpr size_t PLAYER_STRUCT_SIZE_56 = 56;    // Network state
    static constexpr size_t PLAYER_STRUCT_SIZE_44 = 44;    // Compact state
    static constexpr size_t PLAYER_STRUCT_SIZE_136 = 136;  // Extended data
    static constexpr size_t PLAYER_STRUCT_SIZE_1032 = 1032; // Full entity
    
    // Expanded array sizes (original * 4)
    static constexpr size_t EXPANDED_ARRAY_56 = MAX_PLAYERS * PLAYER_STRUCT_SIZE_56;    // 3584 bytes
    static constexpr size_t EXPANDED_ARRAY_44 = MAX_PLAYERS * PLAYER_STRUCT_SIZE_44;    // 2816 bytes
    static constexpr size_t EXPANDED_ARRAY_136 = MAX_PLAYERS * PLAYER_STRUCT_SIZE_136;  // 8704 bytes
    static constexpr size_t EXPANDED_ARRAY_1032 = MAX_PLAYERS * PLAYER_STRUCT_SIZE_1032; // 66048 bytes
    
    // Player pointer array expansion
    static constexpr size_t ORIGINAL_PLAYER_ARRAY_SIZE = ORIGINAL_MAX_PLAYERS * sizeof(void*); // 64 bytes
    static constexpr size_t EXPANDED_PLAYER_ARRAY_SIZE = MAX_PLAYERS * sizeof(void*);          // 256 bytes
    static constexpr size_t PLAYER_ARRAY_EXPANSION = EXPANDED_PLAYER_ARRAY_SIZE - ORIGINAL_PLAYER_ARRAY_SIZE; // 192 bytes
    
    // Initialize player limit patches
    void Init();
    
    // Get the fixed maximum players (always 64)
    constexpr int32_t GetMaxPlayers() { return MAX_PLAYERS; }
    
    // Validate a player index against the limit
    inline bool IsValidPlayerIndex(int8_t index) { return index >= 0 && index < MAX_PLAYERS; }
    
    // Runtime statistics
    struct Stats {
        uint64_t totalPlayerLookups;
        uint64_t extendedIndexLookups;  // Lookups for index >= 16
        uint64_t rejectedLookups;       // Lookups rejected due to invalid index
    };
    
    const Stats& GetStats();
    
    // Memory expansion hooks
    namespace Memory {
        // Intercept and expand player manager allocation
        void* ExpandPlayerManagerAlloc(size_t originalSize);
        
        // Intercept and expand player struct array allocations
        void* ExpandPlayerStructArray(size_t structSize, size_t originalCount);
        
        // Zero-initialize expanded player slots
        void InitializeExpandedSlots(void* playerManager);
    }
}

// =============================================================================
// Player Loop Patch Locations
// =============================================================================
// All confirmed locations of player iteration loops that are hooked.
// =============================================================================

namespace PlayerLoopPatches
{
    struct LoopPatchInfo {
        uint32_t functionAddress;
        const char* functionName;
        const char* description;
    };
    
    // High-priority player loops from ppc_recomp.44.cpp (15 functions)
    static constexpr LoopPatchInfo PLAYER_LOOPS_44[] = {
        {0x8267DB90, "sub_8267DB90", "Distance/proximity check loop"},
        {0x8267D798, "sub_8267D798", "Player iteration with offset 288"},
        {0x8267D910, "sub_8267D910", "Player iteration loop"},
        {0x8267DE28, "sub_8267DE28", "Player bounds check"},
        {0x8267E198, "sub_8267E198", "Player iteration loop"},
        {0x8267EBA8, "sub_8267EBA8", "Player iteration loop"},
        {0x8267ECF0, "sub_8267ECF0", "Player iteration loop"},
        {0x8267F318, "sub_8267F318", "Player bounds check"},
        {0x8267F838, "sub_8267F838", "Player iteration loop"},
        {0x82683998, "sub_82683998", "Player iteration loop"},
        {0x82685D20, "sub_82685D20", "Player bounds check"},
        {0x82685EB0, "sub_82685EB0", "Player bounds check"},
        {0x826861D0, "sub_826861D0", "Player bounds check"},
        {0x82689828, "sub_82689828", "Player iteration loop"},
        {0x82689998, "sub_82689998", "Dual player loops (r29/r30)"},
    };
    
    // High-priority player loops from ppc_recomp.46.cpp (5 functions)
    static constexpr LoopPatchInfo PLAYER_LOOPS_46[] = {
        {0x826AE960, "sub_826AE960", "Player vtable update loop"},
        {0x826AF5B8, "sub_826AF5B8", "Player iteration loop"},
        {0x826AF748, "sub_826AF748", "Player iteration loop"},
        {0x826B0720, "sub_826B0720", "Player iteration loop"},
        {0x826B0830, "sub_826B0830", "Player iteration loop"},
    };
    
    // Player loops from other files (2 functions)
    static constexpr LoopPatchInfo PLAYER_LOOPS_OTHER[] = {
        {0x827FC738, "sub_827FC738", "ppc_recomp.64: vtable call loop"},
        {0x8238F700, "sub_8238F700", "ppc_recomp.18: distance check loop"},
    };
    
    // Total count for validation
    static constexpr size_t TOTAL_LOOP_PATCHES = 
        sizeof(PLAYER_LOOPS_44) / sizeof(LoopPatchInfo) +
        sizeof(PLAYER_LOOPS_46) / sizeof(LoopPatchInfo) +
        sizeof(PLAYER_LOOPS_OTHER) / sizeof(LoopPatchInfo);
}
