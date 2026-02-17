#include <stdafx.h>
#include "memory.h"

#include <rex/kernel/xmemory.h>
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

    fprintf(stderr, "[Memory] InsertFunction stubs done, calling PrePopulateVtables...\n");
    // Pre-populate vtables with function pointers extracted from XEX
    // This ensures vtables have valid entries before game code runs
    PrePopulateVtables(base);
    fprintf(stderr, "[Memory] PrePopulateVtables done, writing manual vtables...\n");
    
    // Additional vtables not in the pre-generated file (0x8207xxxx range)
    // These vtables are in XEX zero-fill blocks and need manual initialization
    // Extracted from default.bin - used by sub_8280D9B8 and related cleanup functions
    // off_8207C4A0 - 3 entry vtable (sub_8280D9B8 calls vtable[2] at offset +8)
    PPC_STORE_U32(0x8207C4A0, 0x8280DC38);  // vtable[0] sub_8280DC38 - destructor/cleanup
    PPC_STORE_U32(0x8207C4A4, 0x826F26B8);  // vtable[1] sub_826F26B8 - stub
    PPC_STORE_U32(0x8207C4A8, 0x826F26B8);  // vtable[2] sub_826F26B8 - called by sub_8280D9B8
    PPC_STORE_U32(0x8207C3C8, 0x8280D8A8);  // sub_8280D8A8 - cleanup method
    PPC_STORE_U32(0x8207C3B4, 0x8280D5E8);  // sub_8280D5E8
    // off_8207C3BC[2] = { sub_8280D6A8, sub_8280D648 }
    PPC_STORE_U32(0x8207C3BC, 0x8280D6A8);
    PPC_STORE_U32(0x8207C3C0, 0x8280D648);
    // off_8207C2E4[3] = { sub_8280D460, sub_826F26B8, sub_826F26B8 }
    PPC_STORE_U32(0x8207C2E4, 0x8280D460);
    PPC_STORE_U32(0x8207C2E8, 0x826F26B8);
    PPC_STORE_U32(0x8207C2EC, 0x826F26B8);
    // off_8207C2F4[2] = { sub_8280D6A8, sub_826F26B8 }
    PPC_STORE_U32(0x8207C2F4, 0x8280D6A8);
    PPC_STORE_U32(0x8207C2F8, 0x826F26B8);
    // off_8207C300[3] = { sub_8280D460, sub_8280D4A8, sub_8280D560 }
    PPC_STORE_U32(0x8207C300, 0x8280D460);
    PPC_STORE_U32(0x8207C304, 0x8280D4A8);
    PPC_STORE_U32(0x8207C308, 0x8280D560);
    // off_8207CB94[3] = { sub_8280F0F0, sub_826F26B8, sub_826F26B8 }
    PPC_STORE_U32(0x8207CB94, 0x8280F0F0);
    PPC_STORE_U32(0x8207CB98, 0x826F26B8);
    PPC_STORE_U32(0x8207CB9C, 0x826F26B8);
    // off_8207CC0C[3] = { sub_8280F700, sub_8280F340, sub_8280F418 }
    PPC_STORE_U32(0x8207CC0C, 0x8280F700);
    PPC_STORE_U32(0x8207CC10, 0x8280F340);
    PPC_STORE_U32(0x8207CC14, 0x8280F418);
    // off_8207C518 = sub_8280DDE8 (single entry used by objects)
    PPC_STORE_U32(0x8207C518, 0x8280DDE8);
    
    // Vtables used by sub_8280BAD8 resource manager initialization
    // off_8207BBAC (3 entries)
    PPC_STORE_U32(0x8207BBAC, 0x8280B7F8);
    PPC_STORE_U32(0x8207BBB0, 0x828E0AB8);
    PPC_STORE_U32(0x8207BBB4, 0x828E0AB8);
    // off_8207BB80 (2 entries)
    PPC_STORE_U32(0x8207BB80, 0x8280B710);
    PPC_STORE_U32(0x8207BB84, 0x8280B508);
    // off_8207BB74 (3 entries)
    PPC_STORE_U32(0x8207BB74, 0x8280B710);
    PPC_STORE_U32(0x8207BB78, 0x826F26B8);
    PPC_STORE_U32(0x8207BB7C, 0x820DDB00);
    // off_8207BB98 (3 entries)
    PPC_STORE_U32(0x8207BB98, 0x8280B5D0);
    PPC_STORE_U32(0x8207BB9C, 0x8280D3A8);
    PPC_STORE_U32(0x8207BBA0, 0x826F26B8);

    fprintf(stderr, "[Memory] Manual vtables done\n");
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
