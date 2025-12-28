// =============================================================================
// GTA IV Save System PPC Function Hooks
// =============================================================================
// These hooks intercept GTA IV's save wrapper functions to provide visibility
// into save operations and ensure proper integration with host filesystem.
//
// Based on REWRITE_PLAYBOOK.md section 14.1:
// - sub_829A1C38: Content creation wrapper (calls XamContentCreateEx)
// - sub_829A1CA0: Content close wrapper (calls XamContentClose)
// - sub_829A1CB8: Enumeration wrapper (calls XamContentCreateEnumerator)
// - sub_8297A930: Save manager (orchestrates save operations)
// =============================================================================

#include <stdafx.h>
#include <cpu/ppc_context.h>
#include "function.h"
#include "memory.h"
#include <cstdio>

// Debug logging control
#define SAVE_HOOKS_DEBUG_LOGGING 1

// =============================================================================
// sub_829A1C38 - Content Creation Wrapper
// Location: ppc_recomp.79.cpp:37654
// Calls XamContentCreateEx to create/open save containers
// =============================================================================
extern "C" void __imp__sub_829A1C38(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829A1C38)
{
    static int s_count = 0;
    ++s_count;
    
#if SAVE_HOOKS_DEBUG_LOGGING
    printf("[SaveHook] ========================================\n");
    printf("[SaveHook] sub_829A1C38 (Content Creation) #%d\n", s_count);
    printf("[SaveHook] r3=0x%08X r4=0x%08X r5=0x%08X\n", ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
#endif
    
    // Call original implementation
    __imp__sub_829A1C38(ctx, base);
    
#if SAVE_HOOKS_DEBUG_LOGGING
    printf("[SaveHook] sub_829A1C38 returned r3=0x%08X\n", ctx.r3.u32);
    printf("[SaveHook] ========================================\n");
#endif
}

// =============================================================================
// sub_829A1CA0 - Content Close Wrapper
// Location: ppc_recomp.79.cpp:37723
// Direct jump to XamContentClose
// =============================================================================
extern "C" void __imp__sub_829A1CA0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829A1CA0)
{
    static int s_count = 0;
    ++s_count;
    
#if SAVE_HOOKS_DEBUG_LOGGING
    printf("[SaveHook] sub_829A1CA0 (Content Close) #%d r3=0x%08X\n", s_count, ctx.r3.u32);
#endif
    
    // Call original implementation
    __imp__sub_829A1CA0(ctx, base);
    
#if SAVE_HOOKS_DEBUG_LOGGING
    printf("[SaveHook] sub_829A1CA0 returned r3=0x%08X\n", ctx.r3.u32);
#endif
}

// =============================================================================
// sub_829A1CB8 - Enumeration Wrapper
// Location: ppc_recomp.79.cpp:37756
// Calls XamContentCreateEnumerator to list save files
// =============================================================================
extern "C" void __imp__sub_829A1CB8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_829A1CB8)
{
    static int s_count = 0;
    ++s_count;
    
#if SAVE_HOOKS_DEBUG_LOGGING
    printf("[SaveHook] ========================================\n");
    printf("[SaveHook] sub_829A1CB8 (Content Enumeration) #%d\n", s_count);
    printf("[SaveHook] r3=0x%08X r4=0x%08X r5=0x%08X r6=0x%08X\n", 
           ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
#endif
    
    // Call original implementation
    __imp__sub_829A1CB8(ctx, base);
    
#if SAVE_HOOKS_DEBUG_LOGGING
    printf("[SaveHook] sub_829A1CB8 returned r3=0x%08X\n", ctx.r3.u32);
    printf("[SaveHook] ========================================\n");
#endif
}

// =============================================================================
// sub_8297A930 - Save Manager
// Location: ppc_recomp.77.cpp:7308
// Orchestrates save operations, calls sub_829A1878
// =============================================================================
extern "C" void __imp__sub_8297A930(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8297A930)
{
    static int s_count = 0;
    ++s_count;
    
#if SAVE_HOOKS_DEBUG_LOGGING
    if (s_count <= 10) {
        printf("[SaveHook] ========================================\n");
        printf("[SaveHook] sub_8297A930 (Save Manager) #%d\n", s_count);
        printf("[SaveHook] r3=0x%08X (context+208)\n", ctx.r3.u32);
        printf("[SaveHook] ========================================\n");
    }
#endif
    
    // Call original implementation
    __imp__sub_8297A930(ctx, base);
    
#if SAVE_HOOKS_DEBUG_LOGGING
    if (s_count <= 10) {
        printf("[SaveHook] sub_8297A930 returned\n");
    }
#endif
}

// =============================================================================
// sub_82122CA0 - Save System Init
// Location: ppc_recomp.0.cpp:7117
// Initializes save system with 3 save slot contexts (Profile, Game, Autosave)
// =============================================================================
extern "C" void __imp__sub_82122CA0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_82122CA0)
{
    static int s_count = 0;
    ++s_count;
    
    printf("[SaveHook] ========================================\n");
    printf("[SaveHook] sub_82122CA0 (Save System Init) #%d\n", s_count);
    printf("[SaveHook] Initializing 3 save slot contexts:\n");
    printf("[SaveHook]   - Profile Save (vtable: 0x81209104)\n");
    printf("[SaveHook]   - Game Save (vtable: 0x81209064)\n");
    printf("[SaveHook]   - Autosave (vtable: 0x81209028)\n");
    printf("[SaveHook] ========================================\n");
    
    // Call original implementation
    __imp__sub_82122CA0(ctx, base);
    
    printf("[SaveHook] sub_82122CA0 completed\n");
}

// =============================================================================
// sub_821200D0 - Post-Init (Profiles/Saves)
// Location: ppc_recomp.0.cpp:134
// Loads profile and enumerates/opens save files
// =============================================================================
extern "C" void __imp__sub_821200D0(PPCContext& ctx, uint8_t* base);
// STUBBED: sub_821200D0 - Post-Init (blocks on loading loop)
PPC_FUNC(sub_821200D0)
{
    static int s_count = 0;
    ++s_count;
    printf("[821200D0] #%d STUBBED - Post-init loading bypassed", s_count);
    ctx.r3.u32 = 0;
}



















