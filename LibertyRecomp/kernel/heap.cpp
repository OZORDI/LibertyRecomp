#include "heap.h"
#include "function.h"
#include "memory.h"
#include "xdm.h"
#include <stdafx.h>

// Memory layout from MarathonRecomp:
// | Region        | Start      | End        | Purpose              |
// |---------------|------------|------------|----------------------|
// | Null guard    | 0x00000000 | 0x00001000 | Catch null ptr access|
// | Normal heap   | 0x00020000 | 0x7FEA0000 | General allocations  |
// | Reserved      | 0x7FEA0000 | 0xA0000000 | XMA I/O, system      |
// | Physical heap | 0xA0000000 | 0xFFFFFFFF | GPU/DMA memory       |

constexpr size_t RESERVED_BEGIN = 0x7FEA0000;
constexpr size_t RESERVED_END = 0xA0000000;

void Heap::Init() {
  // Normal heap: 0x20000 to 0x7FEA0000 (using o1heap)
  heap = o1heapInit(g_memory.Translate(0x20000), RESERVED_BEGIN - 0x20000);

  // Physical heap: 0xA0000000 to 0x100000000 (using o1heap for proper free
  // support)
  physicalHeap =
      o1heapInit(g_memory.Translate(RESERVED_END), 0x100000000 - RESERVED_END);

  std::fprintf(stderr, "[Heap::Init] heap=%p physicalHeap=%p\n", (void *)heap,
               (void *)physicalHeap);
  std::fflush(stderr);
}

void *Heap::Alloc(size_t size) {
  std::lock_guard lock(mutex);

  return o1heapAllocate(heap, std::max<size_t>(1, size));
}

void *Heap::AllocPhysical(size_t size, size_t alignment) {
  size = std::max<size_t>(1, size);
  alignment = alignment == 0 ? 0x1000 : std::max<size_t>(16, alignment);

  std::lock_guard lock(physicalMutex);

  void *ptr = o1heapAllocate(physicalHeap, size + alignment);
  size_t aligned = ((size_t)ptr + alignment) & ~(alignment - 1);

  // Store original pointer and size for Free() to work correctly
  *((void **)aligned - 1) = ptr;
  *((size_t *)aligned - 2) = size + O1HEAP_ALIGNMENT;

  return (void *)aligned;
}

void Heap::Free(void *ptr) {
  if (ptr >= physicalHeap) {
    std::lock_guard lock(physicalMutex);
    o1heapFree(physicalHeap, *((void **)ptr - 1));
  } else {
    std::lock_guard lock(mutex);
    o1heapFree(heap, ptr);
  }
}

size_t Heap::Size(void *ptr) {
  if (ptr)
    return *((size_t *)ptr - 2) -
           O1HEAP_ALIGNMENT; // relies on fragment header in o1heap.c

  return 0;
}

// =============================================================================
// Rtl Heap Functions (from MarathonRecomp)
// =============================================================================

uint32_t RtlAllocateHeap(uint32_t heapHandle, uint32_t flags, uint32_t size) {
  void *ptr = g_userHeap.Alloc(size);
  if ((flags & 0x8) != 0) // HEAP_ZERO_MEMORY
    memset(ptr, 0, size);

  assert(ptr);
  return g_memory.MapVirtual(ptr);
}

uint32_t RtlReAllocateHeap(uint32_t heapHandle, uint32_t flags,
                           uint32_t memoryPointer, uint32_t size) {
  void *ptr = g_userHeap.Alloc(size);
  if ((flags & 0x8) != 0) // HEAP_ZERO_MEMORY
    memset(ptr, 0, size);

  if (memoryPointer != 0) {
    void *oldPtr = g_memory.Translate(memoryPointer);
    memcpy(ptr, oldPtr, std::min<size_t>(size, g_userHeap.Size(oldPtr)));
    g_userHeap.Free(oldPtr);
  }

  assert(ptr);
  return g_memory.MapVirtual(ptr);
}

uint32_t RtlFreeHeap(uint32_t heapHandle, uint32_t flags,
                     uint32_t memoryPointer) {
  if (memoryPointer != NULL)
    g_userHeap.Free(g_memory.Translate(memoryPointer));

  return true;
}

uint32_t RtlSizeHeap(uint32_t heapHandle, uint32_t flags,
                     uint32_t memoryPointer) {
  if (memoryPointer != NULL)
    return (uint32_t)g_userHeap.Size(g_memory.Translate(memoryPointer));

  return 0;
}

// =============================================================================
// X Memory Functions
// =============================================================================

uint32_t XAllocMem(uint32_t size, uint32_t flags) {
  void *ptr =
      (flags & 0x80000000) != 0
          ? g_userHeap.AllocPhysical(size, (1ull << ((flags >> 24) & 0xF)))
          : g_userHeap.Alloc(size);

  if ((flags & 0x40000000) != 0)
    memset(ptr, 0, size);

  assert(ptr);
  return g_memory.MapVirtual(ptr);
}

void XFreeMem(uint32_t baseAddress, uint32_t flags) {
  if (baseAddress != NULL)
    g_userHeap.Free(g_memory.Translate(baseAddress));
}

uint32_t XVirtualAlloc(void *lpAddress, unsigned int dwSize,
                       unsigned int flAllocationType, unsigned int flProtect) {
  assert(!lpAddress);
  return g_memory.MapVirtual(g_userHeap.Alloc(dwSize));
}

uint32_t XVirtualFree(uint32_t lpAddress, unsigned int dwSize,
                      unsigned int dwFreeType) {
  if ((dwFreeType & 0x8000) != 0 && dwSize)
    return FALSE;

  if (lpAddress)
    g_userHeap.Free(g_memory.Translate(lpAddress));

  return TRUE;
}

// =============================================================================
// GTA IV CRT Heap Hooks
// =============================================================================
// NOTE: The actual heap hooks are in imports.cpp using PPC_FUNC macro.
// GUEST_FUNCTION_HOOK doesn't work for game functions (only kernel imports).
// The hooks are: sub_82993298 (malloc), sub_82993360 (free), sub_82993848
// (realloc), sub_829A64C0 (low-level alloc), sub_829A7090 (low-level realloc)
// =============================================================================
// sub_8218BF20 (malloc-like) -> XAllocMem
GUEST_FUNCTION_HOOK(sub_8218BF20, XAllocMem);
