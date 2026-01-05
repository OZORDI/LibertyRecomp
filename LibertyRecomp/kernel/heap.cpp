#include <stdafx.h>
#include "heap.h"
#include "memory.h"
#include "function.h"
#include "xdm.h"

// Virtual heap uses o1heap for general allocations
constexpr size_t RESERVED_BEGIN = 0x7FEA0000;

// Physical heap uses simple bump allocator starting at 0xA0000000
constexpr size_t PHYS_HEAP_START = 0xA0000000;
constexpr size_t PHYS_HEAP_END = 0x100000000;

// Simple bump allocator state for physical memory
static uint8_t* physBumpPtr = nullptr;
static uint8_t* physBumpEnd = nullptr;
static std::mutex physBumpMutex;

void Heap::Init()
{
    // Virtual heap with o1heap
    heap = o1heapInit(g_memory.Translate(0x20000), RESERVED_BEGIN - 0x20000);
    
    // Physical heap - simple bump allocator
    physBumpPtr = (uint8_t*)g_memory.Translate(PHYS_HEAP_START);
    physBumpEnd = (uint8_t*)g_memory.Translate(PHYS_HEAP_END);
    physicalHeap = (O1HeapInstance*)physBumpPtr; // Just for the NULL check
    
    std::fprintf(stderr, "[Heap::Init] heap=%p\n", (void*)heap);
    std::fprintf(stderr, "[Heap::Init] phys bump: %p - %p (size=%zu MB)\n", 
        physBumpPtr, physBumpEnd, (physBumpEnd - physBumpPtr) / (1024*1024));
    std::fflush(stderr);
}

void* Heap::Alloc(size_t size)
{
    std::lock_guard lock(mutex);

    void* ptr = o1heapAllocate(heap, std::max<size_t>(1, size));
    if (ptr == nullptr)
    {
        const O1HeapDiagnostics diag = o1heapGetDiagnostics(heap);
        std::fprintf(stderr,
            "[Heap] OOM (heap): request=%zu capacity=%zu allocated=%zu\n",
            size, diag.capacity, diag.allocated);
        std::fflush(stderr);
    }

    return ptr;
}

void* Heap::AllocPhysical(size_t size, size_t alignment)
{
    size = std::max<size_t>(1, size);
    alignment = alignment == 0 ? 0x1000 : std::max<size_t>(16, alignment);

    std::lock_guard lock(physBumpMutex);
    
    if (!physBumpPtr) {
        std::fprintf(stderr, "[Heap::AllocPhysical] ERROR: physBumpPtr is NULL!\n");
        std::fflush(stderr);
        return nullptr;
    }
    
    // Align the bump pointer
    uint8_t* aligned = (uint8_t*)(((size_t)physBumpPtr + alignment - 1) & ~(alignment - 1));
    
    // Check if we have enough space
    if (aligned + size > physBumpEnd) {
        std::fprintf(stderr, "[Heap::AllocPhysical] OOM: need=%zu available=%zu\n",
            size, (size_t)(physBumpEnd - aligned));
        std::fflush(stderr);
        return nullptr;
    }
    
    // Bump the pointer
    physBumpPtr = aligned + size;
    
    return aligned;
}

void Heap::Free(void* ptr)
{
    // Check if it's from the physical bump allocator region
    if (ptr >= g_memory.Translate(PHYS_HEAP_START) && ptr < g_memory.Translate(PHYS_HEAP_END)) {
        // Bump allocator doesn't free - memory is leaked until reset
        // This is fine for game memory that lives for the session
        return;
    }
    
    // Virtual heap free
    std::lock_guard lock(mutex);
    o1heapFree(heap, ptr);
}

size_t Heap::Size(void* ptr)
{
    // For bump allocator, we don't track individual sizes
    // Return 0 for physical allocations
    if (ptr >= g_memory.Translate(PHYS_HEAP_START) && ptr < g_memory.Translate(PHYS_HEAP_END)) {
        return 0;
    }
    return 0; // TODO: implement for regular heap if needed
}

uint32_t XAllocMem(uint32_t size, uint32_t flags)
{
    void* ptr = (flags & 0x80000000) != 0 ?
        g_userHeap.AllocPhysical(size, (1ull << ((flags >> 24) & 0xF))) :
        g_userHeap.Alloc(size);

    if ((flags & 0x40000000) != 0)
        memset(ptr, 0, size);

    return g_memory.MapVirtual(ptr);
}

void XFreeMem(uint32_t ptr)
{
    g_userHeap.Free(g_memory.Translate(ptr));
}
