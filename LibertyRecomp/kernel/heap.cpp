#include "heap.h"
#include "memory.h"
#include <cstdio>
#include <cstring>
#include <algorithm>

#include <rex/kernel/kernel_state.h>
#include <rex/kernel/xmemory.h>

// =============================================================================
// Guest-memory allocator backed by RexGlue's Memory system.
//
// Liberty's host-side code (video, audio, patches, mod loader, stdx
// containers) needs to allocate memory visible to guest code.  We route
// every allocation through RexGlue's SystemHeapAlloc / SystemHeapFree so
// there is ONE memory manager for the entire address space.
//
// The game's own malloc (sub_8218BF20) is NOT hooked — it runs as
// recompiled PPC code and calls NtAllocateVirtualMemory via RexGlue.
// =============================================================================

Heap g_userHeap;

// Helper: get RexGlue Memory* — returns nullptr before RexGlue is up.
static rex::memory::Memory* rexmem() {
  auto* ks = rex::kernel::kernel_state();
  return ks ? ks->memory() : nullptr;
}

void Heap::Init() {
  std::fprintf(stderr, "[Heap::Init] Using RexGlue memory system (no o1heap)\n");
  std::fflush(stderr);
}

void *Heap::Alloc(size_t size) {
  size = std::max<size_t>(1, size);
  auto* mem = rexmem();
  if (!mem) {
    std::fprintf(stderr, "[Heap::Alloc] FAILED: RexGlue memory not ready\n");
    return nullptr;
  }
  uint32_t guest = mem->SystemHeapAlloc(static_cast<uint32_t>(size));
  if (!guest) {
    std::fprintf(stderr, "[Heap::Alloc] FAILED: size=%zu\n", size);
    return nullptr;
  }
  // SystemHeapAlloc already zeroes the memory.
  return g_memory.Translate(guest);
}

void *Heap::AllocPhysical(size_t size, size_t alignment) {
  size = std::max<size_t>(1, size);
  alignment = alignment == 0 ? 0x1000 : std::max<size_t>(16, alignment);

  auto* mem = rexmem();
  if (!mem) {
    std::fprintf(stderr, "[Heap::AllocPhysical] FAILED: RexGlue memory not ready\n");
    return nullptr;
  }
  uint32_t guest = mem->SystemHeapAlloc(
      static_cast<uint32_t>(size),
      static_cast<uint32_t>(alignment),
      rex::memory::kSystemHeapPhysical);
  if (!guest) {
    std::fprintf(stderr,
                 "[Heap::AllocPhysical] FAILED: size=%zu align=%zu\n",
                 size, alignment);
    return nullptr;
  }
  return g_memory.Translate(guest);
}

void Heap::Free(void *ptr) {
  if (!ptr) return;
  auto* mem = rexmem();
  if (!mem) return;
  uint32_t guest = g_memory.MapVirtual(ptr);
  if (guest) mem->SystemHeapFree(guest);
}

size_t Heap::Size(void *ptr) {
  if (!ptr) return 0;
  auto* mem = rexmem();
  if (!mem) return 0;
  uint32_t guest = g_memory.MapVirtual(ptr);
  auto* heap = mem->LookupHeap(guest);
  if (!heap) return 0;
  uint32_t sz = 0;
  heap->QuerySize(guest, &sz);
  return sz;
}
