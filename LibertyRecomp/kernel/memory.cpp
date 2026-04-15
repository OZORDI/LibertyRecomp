#include <stdafx.h>
#include "memory.h"

#include <rex/system/xmemory.h>
#include <rex/logging.h>

// Include pre-generated vtable data from GTA IV XEX
// This pre-populates vtables with valid function pointers before game starts
#include "../../gta_iv/vtable_prepopulate.h"

// RMPTFX worker thread hook - suspend during Init to prevent signal_sem accumulation
// This must be registered via InsertFunction (not PatchFuncMapping) because
// the thread trampoline uses PPC_CALL_INDIRECT_FUNC which consults PPC_LOOKUP_FUNC
extern "C" void sub_821966D0_hook(PPCContext &ctx, uint8_t *base);

static constexpr size_t AlignDown(size_t value, size_t alignment) noexcept
{
    return value & ~(alignment - 1);
}

static constexpr size_t AlignUp(size_t value, size_t alignment) noexcept
{
    return (value + (alignment - 1)) & ~(alignment - 1);
}

Memory::~Memory() = default;

Memory::Memory()
{
    // Defer initialization to InitializeFromRexGlue().
    // base remains nullptr until then — KiSystemStartup() checks for this.
}

void Memory::InitializeFromRexGlue()
{
    // Create RexGlue's file-backed 4GB memory system
    rex_memory_ = std::make_unique<rex::memory::Memory>();
    if (!rex_memory_->Initialize()) {
        fprintf(stderr, "[Memory] FATAL: Failed to initialize RexGlue memory system\n");
        rex_memory_.reset();
        base = nullptr;
        return;
    }

    base = rex_memory_->virtual_membase();
    fprintf(stderr, "[Memory] RexGlue memory initialized: base=%p\n", (void*)base);

    // Initialize the function table in guest memory for PPC_LOOKUP_FUNC
    if (!rex_memory_->InitializeFunctionTable(PPC_CODE_BASE, PPC_CODE_SIZE,
                                               PPC_IMAGE_BASE, PPC_IMAGE_SIZE)) {
        fprintf(stderr, "[Memory] FATAL: Failed to initialize RexGlue function table\n");
        return;
    }

    // Register all recompiled functions from the mapping table
    // Uses RexGlue's SetFunction which writes to guest memory (PPC_LOOKUP_FUNC compatible)
    int count = 0;
    for (size_t i = 0; PPCFuncMappings[i].guest != 0; i++)
    {
        if (PPCFuncMappings[i].host != nullptr) {
            rex_memory_->SetFunction(
                static_cast<uint32_t>(PPCFuncMappings[i].guest),
                PPCFuncMappings[i].host);
            count++;
        }
    }
    fprintf(stderr, "[Memory] Registered %d recompiled functions via RexGlue\n", count);

    // Shared init: manual stubs, vtable pre-population
    PopulateFunctionTableAndVtables();

    fprintf(stderr, "[DIAG] Post-Populate GetFunction(0x82A692C8)=%p\n", (void*)rex_memory_->GetFunction(0x82A692C8));
}

void Memory::PopulateFunctionTableAndVtables()
{
    fprintf(stderr, "[Memory] PopulateFunctionTableAndVtables() ENTER\n");
    // RMPTFX worker thread hook - patches PPC_LOOKUP_FUNC table (used by indirect calls)
    // PatchFuncMapping only patches PPCFuncMappings[] which is NOT consulted by
    // PPC_CALL_INDIRECT_FUNC in thread trampolines (sub_827DAE40)
    InsertFunction(0x821966D0, sub_821966D0_hook);

    // Register stubs for missing callbacks - must be done BEFORE memory protection
    // 0x829FBE38 - called from sub_829A7960
    InsertFunction(0x829FBE38, [](PPCContext& ctx, uint8_t* base) {
        ctx.r3.u32 = 0; // Return success
    });
    
    // 0x830F2CB8 - called from sub_82A03520 (in BSS range, likely uninitialized pointer)
    InsertFunction(0x830F2CB8, [](PPCContext& ctx, uint8_t* base) {
        ctx.r3.u32 = 0; // Return success
    });

    // Streaming resource callbacks - reached via vtable dispatch from sub_8286C238
    // at offset +0x1DC (0x8286C414). Called 144 times during sub_827C2420
    // (activate-streaming) inside the subsystem init chain (sub_821412B8).
    // Without these, the init chain loops forever and the game never reaches
    // the front-end state machine or world init.
    // TODO: Add these to gta4_config.toml [functions] and re-run codegen
    //       for full recompiled implementations.
    InsertFunction(0x82AE5F34, [](PPCContext& ctx, uint8_t* base) {
        // Streaming resource load callback - return success
        ctx.r3.u32 = 0;
    });
    InsertFunction(0x82AE5EBC, [](PPCContext& ctx, uint8_t* base) {
        // Streaming resource init callback - return success
        ctx.r3.u32 = 0;
    });
    InsertFunction(0x82AE5F1C, [](PPCContext& ctx, uint8_t* base) {
        // Streaming resource callback variant - return success
        ctx.r3.u32 = 0;
    });

    fprintf(stderr, "[Memory] InsertFunction stubs done, calling PrePopulateVtables...\n");
    // Pre-populate vtables with function pointers extracted from XEX
    // This ensures vtables have valid entries before game code runs
    PrePopulateVtables(base);
    fprintf(stderr, "[Memory] PrePopulateVtables done, writing manual vtables...\n");
    
    // REMOVED: 33 manual vtable writes to 0x8207xxxx range.
    // Binary audit (Agent 4) proved these addresses contain live .rdata content:
    // - 26 writes clobbered RTTI/particle effect strings (effectrule, DispersalSettings, etc.)
    // - 7 writes destroyed popcount lookup table data
    // - 4 writes replaced existing valid vtable pointers
    // - 0x820DDB00 target was all zeros (crash if called)
    // - 0x8280F700 target was a function epilogue (crash if called)
    // The comment "These vtables are in XEX zero-fill blocks" was incorrect.
    // Unleashed does NOT manually write vtable pointers — game constructors handle it.

    fprintf(stderr, "[Memory] Manual vtables done\n");

    // ==========================================================================
    // XAM Sign-In State Emulation
    // ==========================================================================
    // On Xbox 360, the XAM kernel writes the sign-in state byte for each user
    // slot when a controller signs in. In the recomp, this never happens, so the
    // front-end state machine (sub_82142230) loops forever at state 0 because:
    //
    //   sub_822414E8 (sign-in check)
    //     → sub_821B4108 (count active player slots)
    //       → sub_822094B8(slot_N) reads byte[inner_ptr+36]
    //         → slot 0 inner_ptr = 0x831C4FF8 (set by sub_821B4660)
    //         → byte[0x831C501C] = 0 ← never written in recomp
    //     → returns 0 → state 0 loops forever → scene never created
    //
    // Fix: Set sign-in state byte for player 0 to non-zero (1 = signed in).
    // This lets sub_821B4108 return 1 active player, advancing the state machine
    // past state 0 to state 3+ where scene creation occurs.
    //
    // Addresses (verified via Python from sub_821B4660 disassembly):
    //   inner_ptr base = lis(-31972) + 20472 = 0x831C4FF8
    //   sign-in byte   = inner_ptr[0] + 36   = 0x831C501C
    // ==========================================================================
    PPC_STORE_U8(0x831C501C, 1);  // Player 0 sign-in state = signed in
    fprintf(stderr, "[Memory] XAM sign-in state: wrote 1 to byte 0x831C501C (player 0 active)\n");

    // RPF mode flag: sub_8285DD10 checks this to decide whether to open
    // common.rpf/xbox360.rpf/audio.rpf as fiPackfile archives (flag=1)
    // or mount loose directories (flag=0). On Xbox 360, disc detection
    // sets this during boot. We set it so the game loads RPF archives.
    PPC_STORE_U32(0x831B59F8, 1);
    fprintf(stderr, "[Memory] RPF mode flag set (0x831B59F8 = 1)\n");

    // ==========================================================================
    // Front-End Readiness Flag
    // ==========================================================================
    // On Xbox 360, sub_821200D0 runs the save state machine (sub_821E6508)
    // which calls XAM dialog flow (sub_8223F9F0) → sub_82254FE0 writes 1 to
    // 0x82BF9B70, signaling that sign-in/storage/content setup is complete.
    //
    // In the recomp, sub_821200D0 is at address 0x821200D0 — BELOW the
    // generated code range (0x82140000+). It was never recompiled and never
    // runs. The front-end state machine (sub_82142230) gets stuck in state 3
    // because sub_8224FA48 reads 0x82BF9B70 (initialized to -1 = "not ready")
    // and returns 0 forever.
    //
    // Fix: Set the readiness flag directly. Liberty's save_system.cpp handles
    // save enumeration, and RexGlue provides the XAM subsystem. The state
    // machine's purpose (sign-in + storage + content setup) is already
    // fulfilled by our initialization code.
    // ==========================================================================
    // NOTE: 0x82BF9B70 must stay at -1 (its natural default). This means
    // "no XAM dialog pending" which is the correct path for state 3 → state 4.
    // Writing 1 here was WRONG — it made sub_8224FA48 return 1, which branches
    // to the dialog-handling path (loc_82142504) that loops back to state 3.
    // Additionally, this write gets overwritten by XEX PE data section loading anyway.
    // NOTE: Function table protection DISABLED
    // The region 0x831F0000+ overlaps with texture buffer addresses that the game
    // legitimately writes to during texture tiling/swizzling operations (sub_829E4970).
    // On Xbox 360, this entire memory range was writable.
    // 
    // The texture system computes destination addresses by adding large offsets
    // (up to 25+ MB) to base addresses in the 0x82000000 range. This can produce
    // addresses like 0x83942000 which land in what we had protected as the function table.
    //
    // Keeping this region writable is necessary for correct texture operations.
    // The risk of game code corrupting function pointers is lower than the certainty
    // of crashes from texture operations being blocked.
    //
    // Original protection code (kept for reference):
    // constexpr size_t kPageSize = 0x1000;
    // constexpr size_t kFuncTableOffset = PPC_IMAGE_BASE + PPC_IMAGE_SIZE;
    // constexpr size_t kFuncTableSize = (PPC_CODE_SIZE * 2) + sizeof(PPCFunc*);
    // const size_t protectBegin = AlignDown(kFuncTableOffset, kPageSize);
    // const size_t protectEnd = AlignUp(kFuncTableOffset + kFuncTableSize, kPageSize);
    // mprotect(base + protectBegin, protectEnd - protectBegin, PROT_READ);
}

void* MmGetHostAddress(uint32_t ptr)
{
    return g_memory.Translate(ptr);
}
