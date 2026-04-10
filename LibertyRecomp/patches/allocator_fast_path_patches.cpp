// allocator_fast_path_patches.cpp
// Fast-path hooks for the hottest allocation functions in GTA IV, with
// integrated HeapGuard double-free protection (moved from heap_guard_patches.cpp).
//
// rage::malloc (sub_821B3510, 3611 callers) and rage::free (sub_821B3560,
// 2283 callers) are the two most-called functions in the entire binary.
// sysMemMultiAllocator::Free (sub_82847618) is the second-level dispatcher
// called by rage::free for every deallocation.
//
// ORIGINAL PPC PATH PER CALL (eliminated by this patch):
//   1. Load r13 (TLS base)
//   2. Deref *r13 to get TLS block pointer
//   3. Load allocator pointer from TLS[1676]
//   4. Load vtable pointer from allocator+0
//   5. Load vfunc guest address from vtable+offset
//   6. PPC_CALL_INDIRECT_FUNC: range check, function table lookup, call
//
// FAST PATH:
//   Steps 1-5 remain (must read guest memory), but step 6 is replaced with
//   a cached host function pointer. PPC_LOOKUP_FUNC is called once per
//   allocator/vtable combination; subsequent calls use the cached pointer
//   directly. This eliminates the per-call range check and table lookup
//   that PPC_CALL_INDIRECT_FUNC performs.
//
// HEAPGUARD INTEGRATION:
//   - rage::free: check ring BEFORE dispatch, record AFTER dispatch returns
//   - rage::malloc: remove returned pointer from ring (prevents false positives
//     on alloc-free-alloc-free cycles)
//   - sysMemMultiAllocator::Free: check-only guard for direct vtable callers
//   The sub_828494D8 (sysMemSimpleAllocator::Free) block-header guard remains
//   in heap_guard_patches.cpp as the innermost safety net.

#include <cpu/ppc_context.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>
#include <cstdint>
#include <cstdio>
#include <mutex>

// =============================================================================
// HeapGuard ring buffer (defined in heap_guard_patches.cpp)
// =============================================================================

extern uint32_t g_freedRing[];
extern size_t   g_freedHead;
extern std::mutex g_freedMutex;
extern uint64_t g_doubleFreeCount;
extern uint64_t g_totalFreeCount;

extern bool HeapGuard_WasRecentlyFreed(uint32_t guestAddr);
extern void HeapGuard_RecordFree(uint32_t guestAddr);
extern void HeapGuard_RemoveEntry(uint32_t guestAddr);

// =============================================================================
// Helper: resolve a guest vtable slot to a host function pointer
// =============================================================================

static inline PPCFunc* ResolveVtableFunc(uint64_t base, uint32_t objGuestAddr, uint32_t vtableByteOffset)
{
    uint32_t vtablePtr = PPC_LOAD_U32(objGuestAddr + 0);
    uint32_t vfuncAddr = PPC_LOAD_U32(vtablePtr + vtableByteOffset);
    return PPC_LOOKUP_FUNC(base, vfuncAddr);
}

// =============================================================================
// TLS allocator accessor
// =============================================================================

static inline uint32_t GetTlsAllocator(PPCContext& ctx)
{
    uint32_t tlsBlock = PPC_LOAD_U32(ctx.r13.u32 + 0);
    return PPC_LOAD_U32(tlsBlock + 1676);
}

// =============================================================================
// Per-thread cache for the primary allocator's Allocate/Free function pointers
// =============================================================================
// The game's primary allocator (sysMemMultiAllocator) is set once during init
// and rarely changes. We cache resolved host function pointers keyed by the
// allocator address and vtable pointer. If either changes (thread switch, swap),
// we re-resolve. thread_local avoids any locking on the hot path.

struct AllocatorCache {
    uint32_t allocatorAddr;
    uint32_t vtablePtr;
    PPCFunc* allocateFunc; // vtable offset 8 (slot 2)
    PPCFunc* freeFunc;     // vtable offset 12 (slot 3)
};

static thread_local AllocatorCache tl_cache = {};

static inline void EnsureCacheValid(uint64_t base, uint32_t allocatorAddr)
{
    uint32_t vtablePtr = PPC_LOAD_U32(allocatorAddr + 0);
    if (tl_cache.allocatorAddr == allocatorAddr &&
        tl_cache.vtablePtr == vtablePtr) {
        return;
    }
    tl_cache.allocatorAddr = allocatorAddr;
    tl_cache.vtablePtr = vtablePtr;
    tl_cache.allocateFunc = ResolveVtableFunc(base, allocatorAddr, 8);
    tl_cache.freeFunc = ResolveVtableFunc(base, allocatorAddr, 12);
}

// =============================================================================
// sub_821B3510 - rage::malloc(size) — FAST PATH + HeapGuard
// =============================================================================
// Original: allocator = TLS[*r13+1676]; return allocator->Allocate(size, 16, 0)
// r3 = size on entry, r3 = allocated pointer on exit.

PPC_FUNC_HOOK(sub_821B3510)
{
    uint32_t size = ctx.r3.u32;
    uint32_t allocatorAddr = GetTlsAllocator(ctx);

    EnsureCacheValid(base, allocatorAddr);

    // Fast path: direct call via cached function pointer
    if (tl_cache.allocateFunc) {
        ctx.r3.u64 = allocatorAddr;
        ctx.r4.u64 = size;
        ctx.r5.s64 = 16;
        ctx.r6.s64 = 0;
        tl_cache.allocateFunc(ctx, base);
    } else {
        // Fallback: full PPC indirect dispatch
        uint32_t vtablePtr = PPC_LOAD_U32(allocatorAddr + 0);
        uint32_t vfuncAddr = PPC_LOAD_U32(vtablePtr + 8);
        ctx.r3.u64 = allocatorAddr;
        ctx.r4.u64 = size;
        ctx.r5.s64 = 16;
        ctx.r6.s64 = 0;
        PPC_CALL_INDIRECT_FUNC(vfuncAddr);
    }

    // HeapGuard: remove the returned pointer from the freed ring so that
    // alloc-free-alloc-free cycles don't trigger false-positive double-free.
    uint32_t result = ctx.r3.u32;
    if (result != 0) {
        std::lock_guard<std::mutex> lock(g_freedMutex);
        HeapGuard_RemoveEntry(result);
    }
}

// =============================================================================
// sub_821B3560 - rage::free(ptr) — FAST PATH + HeapGuard
// =============================================================================
// Original: if (!ptr) return; allocator = TLS[*r13+1676]; allocator->Free(ptr)
// r3 = ptr on entry.

PPC_FUNC_HOOK(sub_821B3560)
{
    uint32_t ptr = ctx.r3.u32;
    if (ptr == 0)
        return;

    // HeapGuard Phase 1: check for double-free BEFORE dispatching.
    {
        std::lock_guard<std::mutex> lock(g_freedMutex);
        g_totalFreeCount++;

        if (HeapGuard_WasRecentlyFreed(ptr)) {
            g_doubleFreeCount++;
            if (g_doubleFreeCount <= 64 ||
                (g_doubleFreeCount & 0xFF) == 0) {
                LOGF_WARNING("HeapGuard: blocked double-free of 0x{:08X} "
                    "(count: {} / {} total frees)",
                    ptr, g_doubleFreeCount, g_totalFreeCount);
            }
            return;
        }
    }

    // Fast path Phase 2: dispatch via cached function pointer.
    uint32_t allocatorAddr = GetTlsAllocator(ctx);
    EnsureCacheValid(base, allocatorAddr);

    if (tl_cache.freeFunc) {
        ctx.r3.u64 = allocatorAddr;
        ctx.r4.u64 = ptr;
        tl_cache.freeFunc(ctx, base);
    } else {
        uint32_t vtablePtr = PPC_LOAD_U32(allocatorAddr + 0);
        uint32_t vfuncAddr = PPC_LOAD_U32(vtablePtr + 12);
        ctx.r3.u64 = allocatorAddr;
        ctx.r4.u64 = ptr;
        PPC_CALL_INDIRECT_FUNC(vfuncAddr);
    }

    // HeapGuard Phase 3: record AFTER successful free.
    {
        std::lock_guard<std::mutex> lock(g_freedMutex);
        HeapGuard_RecordFree(ptr);
    }
}

// =============================================================================
// sub_82847618 - sysMemMultiAllocator::Free(this, ptr) — FAST PATH + HeapGuard
// =============================================================================
// Original (224 bytes):
//   if (!ptr) return;
//   for (i = 0; i < this->subAllocatorCount; i++) {
//       sub = this->subAllocators[i];           // this+4 array, 4 bytes each
//       if (sub->Contains(ptr)) {               // vtable offset 72
//           // Dispatch logic (from PPC control flow):
//           //   if (tls[1688] != 0 && tls[1704] == 0): FORCE free
//           //   elif (i == 1 || i == 3): SKIP free (return)
//           //   else: DO free
//           subAllocators[i+1]->Free(ptr);      // vtable offset 12
//           return;
//       }
//   }
//   nullsub_1("sysMemMultiAllocator::Free(%p) - Not owned by any known heap?")

// Cache for sub-allocator function pointers (max 8 sub-allocators per layout).
struct MultiAllocSubCache {
    uint32_t subAllocAddr;
    uint32_t vtablePtr;
    PPCFunc* containsFunc; // vtable offset 72
    PPCFunc* freeFunc;     // vtable offset 12
};

struct MultiAllocCache {
    uint32_t multiAllocAddr;
    uint32_t subAllocCount;
    MultiAllocSubCache subs[8];
};

static thread_local MultiAllocCache tl_multiCache = {};

static void EnsureMultiCacheValid(uint64_t base, uint32_t multiAllocAddr)
{
    uint32_t count = PPC_LOAD_U32(multiAllocAddr + 36);

    bool cacheHit = (tl_multiCache.multiAllocAddr == multiAllocAddr &&
                     tl_multiCache.subAllocCount == count);

    if (cacheHit && count > 0) {
        // Quick validation: check first sub-allocator vtable hasn't changed
        uint32_t firstSub = PPC_LOAD_U32(multiAllocAddr + 4);
        uint32_t firstVt = PPC_LOAD_U32(firstSub + 0);
        if (tl_multiCache.subs[0].subAllocAddr == firstSub &&
            tl_multiCache.subs[0].vtablePtr == firstVt) {
            return;
        }
    } else if (cacheHit) {
        return;
    }

    // Rebuild cache
    tl_multiCache.multiAllocAddr = multiAllocAddr;
    tl_multiCache.subAllocCount = count;

    uint32_t cap = count < 8 ? count : 8;
    for (uint32_t i = 0; i < cap; i++) {
        uint32_t subAddr = PPC_LOAD_U32(multiAllocAddr + 4 + i * 4);
        uint32_t vtPtr = PPC_LOAD_U32(subAddr + 0);
        tl_multiCache.subs[i].subAllocAddr = subAddr;
        tl_multiCache.subs[i].vtablePtr = vtPtr;
        tl_multiCache.subs[i].containsFunc = ResolveVtableFunc(base, subAddr, 72);
        tl_multiCache.subs[i].freeFunc = ResolveVtableFunc(base, subAddr, 12);
    }
}

// Forward-declare nullsub_1 (warning printer, does nothing in release)
extern "C" PPC_FUNC(sub_822BCA90);

PPC_FUNC_HOOK(sub_82847618)
{
    uint32_t multiAllocAddr = ctx.r3.u32;
    uint32_t ptr = ctx.r4.u32;

    if (ptr == 0)
        return;

    // HeapGuard: check-only guard for direct vtable callers that bypass rage::free.
    {
        std::lock_guard<std::mutex> lock(g_freedMutex);
        if (HeapGuard_WasRecentlyFreed(ptr)) {
            g_doubleFreeCount++;
            if (g_doubleFreeCount <= 64 ||
                (g_doubleFreeCount & 0xFF) == 0) {
                LOGF_WARNING("HeapGuard: blocked double-free at "
                    "sysMemMultiAllocator::Free for 0x{:08X} "
                    "(count: {})", ptr, g_doubleFreeCount);
            }
            return;
        }
    }

    uint32_t count = PPC_LOAD_U32(multiAllocAddr + 36);
    if (count <= 0) {
        // No sub-allocators — "not owned" warning
        ctx.r3.u64 = 0x82084884u; // "sysMemMultiAllocator::Free(%p) - Not owned by any known heap?"
        ctx.r4.u64 = ptr;
        sub_822BCA90(ctx, base);
        return;
    }

    EnsureMultiCacheValid(base, multiAllocAddr);

    uint32_t cap = count < 8 ? count : 8;

    for (uint32_t i = 0; i < cap; i++) {
        // Call Contains(sub, ptr) — returns bool in r3 (lowest byte)
        PPCFunc* containsFn = tl_multiCache.subs[i].containsFunc;
        if (containsFn) {
            ctx.r3.u64 = tl_multiCache.subs[i].subAllocAddr;
            ctx.r4.u64 = ptr;
            containsFn(ctx, base);
        } else {
            // Fallback: full PPC indirect dispatch
            uint32_t subAddr = PPC_LOAD_U32(multiAllocAddr + 4 + i * 4);
            uint32_t vt = PPC_LOAD_U32(subAddr + 0);
            uint32_t containsAddr = PPC_LOAD_U32(vt + 72);
            ctx.r3.u64 = subAddr;
            ctx.r4.u64 = ptr;
            PPC_CALL_INDIRECT_FUNC(containsAddr);
        }

        if ((ctx.r3.u32 & 0xFF) == 0)
            continue; // Not contained in this sub-allocator

        // Found owning sub-allocator at index i.
        // Replicate TLS-based skip/force logic from original PPC:
        //   if (tls[1688] != 0 && tls[1704] == 0): FORCE free (skip index check)
        //   elif (i == 1 || i == 3): SKIP free (return without freeing)
        //   else: DO free
        uint32_t tlsBlock = PPC_LOAD_U32(ctx.r13.u32 + 0);
        uint32_t tls1688 = PPC_LOAD_U32(tlsBlock + 1688);
        bool forceFree = false;
        if (tls1688 != 0) {
            uint32_t tls1704 = PPC_LOAD_U32(tlsBlock + 1704);
            if (tls1704 == 0) {
                forceFree = true; // TLS override: free unconditionally
            }
        }

        if (!forceFree && (i == 1 || i == 3)) {
            return; // Skip free for these sub-allocator indices
        }

        // Dispatch Free to subAllocators[i+1] (per original logic)
        uint32_t nextIdx = i + 1;
        if (nextIdx < cap && tl_multiCache.subs[nextIdx].freeFunc) {
            ctx.r3.u64 = tl_multiCache.subs[nextIdx].subAllocAddr;
            ctx.r4.u64 = ptr;
            tl_multiCache.subs[nextIdx].freeFunc(ctx, base);
        } else {
            // Fallback
            uint32_t nextSubAddr = PPC_LOAD_U32(multiAllocAddr + 4 + nextIdx * 4);
            uint32_t vt = PPC_LOAD_U32(nextSubAddr + 0);
            uint32_t freeAddr = PPC_LOAD_U32(vt + 12);
            ctx.r3.u64 = nextSubAddr;
            ctx.r4.u64 = ptr;
            PPC_CALL_INDIRECT_FUNC(freeAddr);
        }
        return;
    }

    // No sub-allocator contains the pointer
    ctx.r3.u64 = 0x82084884u; // "sysMemMultiAllocator::Free(%p) - Not owned by any known heap?"
    ctx.r4.u64 = ptr;
    sub_822BCA90(ctx, base);
}
