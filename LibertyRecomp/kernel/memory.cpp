#include <stdafx.h>
#include "memory.h"

#include "../cpu/ppc_context.h"

#include <rex/system/xmemory.h>
#include <rex/logging.h>

// vtable_prepopulate.h removed — PrePopulateVtables wrote 3,809 vtable
// entries BEFORE rexglue's LoadXexImage, which then overwrote them all
// when decompressing the PE. Dead stores. rexglue owns vtable loading.

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
    // RMPTFX worker thread hook - patches PPC_LOOKUP_FUNC table (used by indirect calls)
    // PatchFuncMapping only patches PPCFuncMappings[] which is NOT consulted by
    // PPC_CALL_INDIRECT_FUNC in thread trampolines (sub_827DAE40)
    InsertFunction(0x821966D0, sub_821966D0_hook);

    // REMOVED: 5 force-success PPC_FUNC stubs (0x829FBE38, 0x830F2CB8, 0x82AE5F34,
    //   0x82AE5EBC, 0x82AE5F1C). These were fake-success callbacks for 144+
    //   streaming-resource init calls. The real decompiled versions should be
    //   produced by adding these addresses to glue/gta4-recomp/gta4_config.toml
    //   [functions] and regenerating. Band-aid stubs were masking missing codegen.
    //
    // REMOVED: PrePopulateVtables(base) — 3,809 PPC_STORE_U32 writes from
    //   vtable_prepopulate.h that ran BEFORE rex::Runtime::LoadXexImage().
    //   rexglue's LoadXexImage decompresses & writes all PE sections including
    //   vtables, overwriting our pre-populated entries. Dead stores.
    //
    // REMOVED: 33 manual vtable writes to 0x8207xxxx range — they were
    //   clobbering live .rdata (RTTI strings, popcount tables).
    //
    // REMOVED: PPC_STORE_U8(0x831C501C, 1) — force-wrote XAM sign-in state
    //   for player 0. Should route through rexglue's XamUserGetSigninState.
    //
    // REMOVED: PPC_STORE_U32(0x831B59F8, 1) — force-wrote RPF mode flag
    //   (disc-detection pretend). rexglue's VFS layer should determine this.

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
