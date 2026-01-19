#include <stdafx.h>
#include "memory.h"

// Include pre-generated vtable data from GTA IV XEX
// This pre-populates vtables with valid function pointers before game starts
#include "../../gta_iv/vtable_prepopulate.h"

static constexpr size_t AlignDown(size_t value, size_t alignment) noexcept
{
    return value & ~(alignment - 1);
}

static constexpr size_t AlignUp(size_t value, size_t alignment) noexcept
{
    return (value + (alignment - 1)) & ~(alignment - 1);
}

Memory::Memory()
{
#ifdef _WIN32
    base = (uint8_t*)VirtualAlloc((void*)0x100000000ull, PPC_MEMORY_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (base == nullptr)
        base = (uint8_t*)VirtualAlloc(nullptr, PPC_MEMORY_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (base == nullptr)
        return;

    // Some titles (e.g., GTA IV) legitimately touch low memory (including address 0).
    // Do not install a null-page guard in that case.
#else
    base = (uint8_t*)mmap((void*)0x100000000ull, PPC_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

    if (base == (uint8_t*)MAP_FAILED)
        base = (uint8_t*)mmap(NULL, PPC_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

    if (base == (uint8_t*)MAP_FAILED)
    {
        base = nullptr;
        return;
    }

    // Some titles (e.g., GTA IV) legitimately touch low memory (including address 0).
    // Do not install a null-page guard in that case.
#endif

    for (size_t i = 0; PPCFuncMappings[i].guest != 0; i++)
    {
        if (PPCFuncMappings[i].host != nullptr)
            InsertFunction(PPCFuncMappings[i].guest, PPCFuncMappings[i].host);
    }

    // Register stubs for missing callbacks - must be done BEFORE memory protection
    // 0x829FBE38 - called from sub_829A7960
    InsertFunction(0x829FBE38, [](PPCContext& ctx, uint8_t* base) {
        ctx.r3.u32 = 0; // Return success
    });
    
    // 0x830F2CB8 - called from sub_82A03520 (in BSS range, likely uninitialized pointer)
    InsertFunction(0x830F2CB8, [](PPCContext& ctx, uint8_t* base) {
        ctx.r3.u32 = 0; // Return success
    });

    // Pre-populate vtables with function pointers extracted from XEX
    // This ensures vtables have valid entries before game code runs
    PrePopulateVtables(base);
    
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

    // Protect the recomp function lookup table from guest writes.
    // If the game overwrites these host function pointers, it can lead to crashes that look like
    // invalid indirect branches / pointer-authentication failures on arm64e.
    constexpr size_t kPageSize = 0x1000;
    constexpr size_t kFuncTableOffset = PPC_IMAGE_BASE + PPC_IMAGE_SIZE;
    constexpr size_t kFuncTableSize = (PPC_CODE_SIZE * 2) + sizeof(PPCFunc*);
    const size_t protectBegin = AlignDown(kFuncTableOffset, kPageSize);
    const size_t protectEnd = AlignUp(kFuncTableOffset + kFuncTableSize, kPageSize);
    if (protectEnd > protectBegin)
    {
#ifdef _WIN32
        DWORD oldProtect{};
        VirtualProtect(base + protectBegin, protectEnd - protectBegin, PAGE_READONLY, &oldProtect);
#else
        mprotect(base + protectBegin, protectEnd - protectBegin, PROT_READ);
#endif
    }
}

void* MmGetHostAddress(uint32_t ptr)
{
    return g_memory.Translate(ptr);
}
