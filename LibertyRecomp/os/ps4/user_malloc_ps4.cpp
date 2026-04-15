#ifdef LIBERTY_RECOMP_PS4
// PS4 custom memory allocator for LibertyRecomp (OpenOrbis)
// Modeled on Minecraft PS4's user_malloc.cpp (Sony SDK reference).
// Must be called before ANY other allocation — do it first in ps4_main.cpp.
//
// Memory layout (total: 4.6 GB dmem)
//   Heap (mspace)   ~3.954 GB   — everything below
//   GPU VRAM        512 MB      — reserved by renderer at init
//   CPU gfx buffers 112 MB      — reserved by renderer at init
//   Save file       24 MB       — reserved via virtual allocs
//   TLS             4 MB
//   Razor/debug     2 MB        (aligned up from ~64 KB)

#include <orbis/libkernel.h>
#include <orbis/libc/stdlib.h>
#include <orbis/UserService.h>

#include <stddef.h>
#include <stdint.h>

// ── Heap state ───────────────────────────────────────────────────────────────

static SceLibcMspace s_mspace    = nullptr;
static void*         s_mappedAddr = nullptr;
static off_t         s_memStart  = 0;
static size_t        s_memLength = 0;

static constexpr size_t MB            = 1024ULL * 1024ULL;
static constexpr size_t MEM_ALIGN     = 2 * MB;   // 2 MB — match dmem granularity
static constexpr size_t HEAP_TOTAL    = 4608 * MB; // PS4 main dmem ceiling

static constexpr size_t RESERVE_TLS   = 4   * MB;
static constexpr size_t RESERVE_RAZOR = 2   * MB;
static constexpr size_t RESERVE_SAVES = 24  * MB;
static constexpr size_t RESERVE_GPU   = 512 * MB;
static constexpr size_t RESERVE_CPU   = 112 * MB;

// ── Public API ───────────────────────────────────────────────────────────────

extern "C" int user_malloc_init(void)
{
    s_memLength = HEAP_TOTAL
                  - RESERVE_TLS
                  - RESERVE_RAZOR
                  - RESERVE_SAVES
                  - RESERVE_GPU
                  - RESERVE_CPU;

    // Allocate direct (physical) memory from the kernel
    int res = sceKernelAllocateDirectMemory(
        /*searchStart*/ 0,
        /*searchEnd*/   SCE_KERNEL_MAIN_DMEM_SIZE,
        /*len*/         s_memLength,
        /*alignment*/   MEM_ALIGN,
        /*memoryType*/  SCE_KERNEL_WB_ONION,
        /*physAddrOut*/ &s_memStart);

    if (res < 0)
        return 1;

    // Map it into the process virtual address space
    void* addr = nullptr;
    res = sceKernelMapDirectMemory(
        &addr,
        s_memLength,
        SCE_KERNEL_PROT_CPU_READ | SCE_KERNEL_PROT_CPU_WRITE,
        0,
        s_memStart,
        MEM_ALIGN);

    if (res < 0)
        return 1;

    s_mappedAddr = addr;

    // Create an mspace over the mapped region
    s_mspace = sceLibcMspaceCreate("LibertyHeap", addr, s_memLength, 0);
    if (!s_mspace)
        return 1;

    return 0;
}

extern "C" int user_malloc_finalize(void)
{
    if (s_mspace)
    {
        int res = sceLibcMspaceDestroy(s_mspace);
        if (res != 0)
            return 1;
        s_mspace = nullptr;
    }

    if (s_mappedAddr)
    {
        int res = sceKernelMunmap(s_mappedAddr, s_memLength);
        if (res < 0)
            return 1;
        s_mappedAddr = nullptr;
    }

    if (s_memStart)
    {
        int res = sceKernelReleaseDirectMemory(s_memStart, s_memLength);
        if (res < 0)
            return 1;
        s_memStart  = 0;
        s_memLength = 0;
    }

    return 0;
}

// ── Malloc overrides (called by libc via user_malloc_init linkage) ───────────

extern "C" void* user_malloc(size_t size)
{
    return sceLibcMspaceMalloc(s_mspace, size);
}

extern "C" void user_free(void* ptr)
{
    sceLibcMspaceFree(s_mspace, ptr);
}

extern "C" void* user_calloc(size_t nelem, size_t size)
{
    return sceLibcMspaceCalloc(s_mspace, nelem, size);
}

extern "C" void* user_realloc(void* ptr, size_t size)
{
    return sceLibcMspaceRealloc(s_mspace, ptr, size);
}

extern "C" void* user_memalign(size_t boundary, size_t size)
{
    return sceLibcMspaceMemalign(s_mspace, boundary, size);
}

extern "C" int user_posix_memalign(void** ptr, size_t boundary, size_t size)
{
    return sceLibcMspacePosixMemalign(s_mspace, ptr, boundary, size);
}

extern "C" void* user_reallocalign(void* ptr, size_t size, size_t boundary)
{
    return sceLibcMspaceReallocalign(s_mspace, ptr, boundary, size);
}

extern "C" int user_malloc_stats(SceLibcMallocManagedSize* mmsize)
{
    return sceLibcMspaceMallocStats(s_mspace, mmsize);
}

extern "C" int user_malloc_stats_fast(SceLibcMallocManagedSize* mmsize)
{
    return sceLibcMspaceMallocStatsFast(s_mspace, mmsize);
}

extern "C" size_t user_malloc_usable_size(void* ptr)
{
    return sceLibcMspaceMallocUsableSize(ptr);
}
#endif // LIBERTY_RECOMP_PS4
