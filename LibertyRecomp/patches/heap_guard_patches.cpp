// heap_guard_patches.cpp
// Double-free guard for the RAGE physical heap allocator system.
//
// RAGE MEMORY ALLOCATOR HIERARCHY (decompiled from recomp analysis):
//
//   rage::sysMemAllocator (base class, vtable @ 0x82000FA4)
//     vtable[2] = Allocate(size, align, sub_allocator_idx)
//     vtable[3] = Free(ptr)
//     vtable[18] = Contains(ptr) -> bool
//     Field +0x00: vtable ptr
//     Field +0x08: unknown (10 reads)
//
//   rage::sysMemMultiAllocator : sysMemAllocator (vtable @ 0x8208480C)
//     Dispatches to N child allocators. Iterates children calling Contains
//     to find the owner, then dispatches Free to that child.
//     Field +0x00: vtable ptr
//     Field +0x04: subAllocators[8] (array of sysMemAllocator*)
//     Field +0x24: subAllocatorCount (int32)
//     vtable[3]  = Free (sub_82847618) - iterates sub-allocators
//     vtable[18] = Contains (sub_828478B0) - checks sub-allocators
//
//   rage::sysMemSimpleAllocator : sysMemAllocator (vtable @ 0x82084AE4)
//     Arena allocator with 16-byte block headers and free-list bucketing.
//     Uses a bitmap at +0xF0 for large-block tracking (16KB pages).
//     Field +0x00: vtable ptr
//     Field +0x04: heapStart
//     Field +0x4C: heapEnd
//     Field +0x54: totalSize (usedBytes at +0x54)
//     Field +0x98: unknown
//     Field +0xBC: unknown (read+write)
//     Field +0xC8: unknown
//     Field +0xEC: heapBase (base address for block index computation)
//     Field +0xF0: bitmap[N] (large-block presence bits, 1 bit per 16KB page)
//     vtable[3]  = Free (sub_828494D8) - lock, bitmap check, dispatch
//     vtable[5]  = sub_82848E70
//     vtable[11] = sub_82848508 (leak dump: "%d leak(s) total")
//     vtable[16] = sub_82849580
//     vtable[18] = Contains (sub_82848F48)
//
//   Block header (16 bytes before each user pointer in sysMemSimpleAllocator):
//     +0x00: guardWord (self-pointer to header; trashed = buffer overrun)
//     +0x04: blockSize (payload bytes, excludes 16-byte header)
//     +0x08: nextPhysical (ptr to next block header in arena)
//     +0x0C: flags (bit4=allocated, bits[3:0]=sizeClass for free-list bucket)
//     When free, two additional fields used for doubly-linked free-list:
//     +0x10: freeList prev ptr (0 = head of bucket)
//     +0x14: freeList next ptr (0 = tail of bucket)
//     Free-list buckets: allocator+0x0C..allocator+0x48 (15 buckets by size>>4)
//
// CALL CHAIN:
//   rage::free (sub_821B3560) -- 2283 callers, leaf, 48 bytes
//     r4 = r3 (ptr); if null return;
//     allocator = TLS[r13+0][1676]
//     tail-call allocator->vtable[3](allocator, ptr)
//       -> sysMemMultiAllocator::Free (sub_82847618) -- 224 bytes
//         for i in 0..subAllocatorCount:
//           if subAllocators[i]->Contains(ptr):  // vtable[18], offset 72
//             check TLS flags at [r13+0]+1688 and [r13+0]+1704
//             dispatch to subAllocators[i+1]->Free(ptr)  // vtable[3], offset 12
//             return
//         nullsub_1("sysMemMultiAllocator::Free(%p) - Not owned by any known heap?")
//           -> sysMemSimpleAllocator::Free (sub_828494D8) -- 168 bytes
//             EnterCriticalSection(critSec at stack+80 via sub_8285FF50)
//             blockIdx = (ptr - this->heapBase) >> 14
//             bitmapWord = this->bitmap[blockIdx >> 5]
//             if (bitmapWord & (1 << (blockIdx & 0x1F))) && (ptr & 0xFFFFC000):
//               sub_82871758(pageBase, ptr, this)  // large-block free (208 bytes)
//                 memset(ptr, 0xDD, pageSize)
//                 push ptr onto page free-list
//                 if page fully free: unlink from partial list, clear bitmap bit
//                 then fall through to sub_82848B68(this, pageBase)
//             else:
//               sub_82848B68(this, ptr)  // normal block free (776 bytes)
//                 assert !TLS[r13+0][1684] ("free from locked heap")
//                 assert this->Contains(ptr)
//                 header = ptr - 16
//                 assert header->flags & 0x10 ("already marked free")
//                 assert header->guardWord == header ("guard word trashed")
//                 header->flags &= ~0x10  // clear allocated bit
//                 memset(ptr, 0xDD, header->blockSize)
//                 update allocator stats (+0x98 freeBytes, +0x54 usedBytes)
//                 update per-sizeClass stats (allocator + (sizeClass+22)*4)
//                 coalesce with next physical block if free
//                 coalesce with prev physical block if free
//                 insert into free-list bucket by size
//                 ** CORRUPTION HAPPENS HERE on double-free:
//                    block header already modified, free-list linkage corrupted **
//             LeaveCriticalSection (sub_8285FFA0)
//
// DOUBLE-FREE SOURCES:
//   sub_828D1828  - generic resource destructor
//   sub_828DB760  - "nonresident" destructor (double refcount decrement)
//   sub_828C73F0  - shader .dcl loader (failure path + destructor both free)
//   sub_828C8108  - shader resource fixup (temp buffer freed twice)
//
// BUG IN PREVIOUS IMPLEMENTATION:
//   The rage::free hook recorded the address into the ring buffer BEFORE calling
//   __imp__sub_821B3560. The __imp__ then dispatched to sysMemMultiAllocator::Free
//   via vtable[3]. The sysMemMultiAllocator::Free hook checked the ring buffer and
//   found the address already recorded — blocking the free as a "double-free" when
//   it was actually the FIRST legitimate free propagating down the call chain.
//   This caused 51 physical heap blocks to leak per session.
//
// FIX:
//   - rage::free: check ring BEFORE dispatch, record AFTER dispatch returns
//   - sysMemMultiAllocator::Free: check-only guard (no recording), catches
//     direct vtable calls that bypass rage::free
//   - sysMemSimpleAllocator::Free: block-header flag check as innermost safety net

#include <cpu/ppc_context.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>
#include <fmt/format.h>
#include <cstring>
#include <mutex>

// =============================================================================
// Recently-freed address ring buffer
// =============================================================================
// Fixed-size, O(1) insert, O(N) lookup. N=4096 covers typical burst patterns
// without host heap allocation. Protected by mutex since multiple game threads
// call rage::free concurrently.

// These are non-static so allocator_fast_path_patches.cpp can access them.
constexpr size_t FREED_RING_SIZE = 4096;
constexpr size_t FREED_RING_MASK = FREED_RING_SIZE - 1;

uint32_t g_freedRing[FREED_RING_SIZE] = {};
size_t   g_freedHead = 0;
std::mutex g_freedMutex;

// Stats for diagnostics
uint64_t g_doubleFreeCount = 0;
uint64_t g_totalFreeCount  = 0;

bool HeapGuard_WasRecentlyFreed(uint32_t guestAddr)
{
    for (size_t i = 0; i < FREED_RING_SIZE; ++i) {
        size_t idx = (g_freedHead - 1 - i) & FREED_RING_MASK;
        uint32_t entry = g_freedRing[idx];
        if (entry == guestAddr)
            return true;
        if (entry == 0)
            break; // Hit empty slot = end of valid entries
    }
    return false;
}

void HeapGuard_RecordFree(uint32_t guestAddr)
{
    g_freedRing[g_freedHead & FREED_RING_MASK] = guestAddr;
    g_freedHead++;
}

void HeapGuard_RemoveEntry(uint32_t guestAddr)
{
    // Called when a pointer is re-allocated (rage::malloc returns it).
    // Remove from ring so legitimate alloc-free-alloc-free cycles work.
    for (size_t i = 0; i < FREED_RING_SIZE; ++i) {
        size_t idx = (g_freedHead - 1 - i) & FREED_RING_MASK;
        if (g_freedRing[idx] == guestAddr) {
            g_freedRing[idx] = 0; // Invalidate
            return;
        }
        if (g_freedRing[idx] == 0)
            break;
    }
}

// =============================================================================
// sub_821B3510 (rage::malloc), sub_821B3560 (rage::free), sub_82847618
// (sysMemMultiAllocator::Free) hooks have been moved to
// allocator_fast_path_patches.cpp which integrates HeapGuard double-free
// protection with fast-path vtable dispatch (cached function pointers).
// =============================================================================

// =============================================================================
// sub_828494D8 - sysMemSimpleAllocator::Free(this, ptr)
// =============================================================================
// Decompiled original (168 bytes):
//   void sysMemSimpleAllocator::Free(void* ptr) {
//       ScopedCriticalSection lock(this->critSec);  // sub_8285FF50 / sub_8285FFA0
//       int blockIdx = ((int)ptr - this->heapBase) >> 14;    // +0xEC
//       uint32_t bitmapWord = this->bitmap[blockIdx >> 5];    // +0xF0 array
//       uint32_t bit = 1 << (blockIdx & 0x1F);
//       if ((bitmapWord & bit) && (ptr & 0xFFFFC000)) {
//           // Large-block free: ptr belongs to a 16KB page tracked in bitmap
//           sub_82871758(ptr & 0xFFFFC000, ptr, this);
//       } else {
//           // Normal block free: standard free-list manipulation
//           sub_82848B68(this, ptr);
//       }
//   }
//
// sub_82848B68 (inner free, 776 bytes) performs:
//   1. Assert heap not locked (TLS[r13+0][1684])
//   2. Assert allocator->Contains(ptr) via vtable[18]
//   3. Read block header at ptr-16:
//      - Assert flags & 0x10 ("already marked free")
//      - Assert guardWord == &header ("guard word trashed")
//   4. Clear allocated bit: flags &= ~0x10
//   5. memset(ptr, 0xDD, blockSize) via sub_829FF840
//   6. Update allocator stats: freeBytes(+0x98), usedBytes(+0x54), per-sizeClass
//   7. Coalesce with next physical block if free (unlink from free-list, merge)
//   8. Coalesce with prev physical block if free (unlink from free-list, merge)
//   9. Insert merged block into free-list bucket (15 buckets by size>>4, capped at 15)
//
// sub_82871758 (large-block free, 208 bytes) performs:
//   1. memset(ptr, 0xDD, pageSize) via sub_829FF840
//   2. Push ptr onto page's free-slot list (page+12 = head, page+8 = count)
//   3. If page fully freed (count == maxSlots from page descriptor+6):
//      a. Unlink page from partial-page doubly-linked list
//      b. Clear bitmap bit for this page
//      c. Fall through to sub_82848B68(allocator, pageBase) to free the page itself
//
// Hook: innermost safety net using block-header allocated flag. This catches
// any double-free that somehow passes through both outer guards.

extern "C" PPC_FUNC(__imp__sub_828494D8);

PPC_FUNC_HOOK(sub_828494D8)
{
    uint32_t allocator = ctx.r3.u32;
    uint32_t ptr = ctx.r4.u32;

    if (ptr == 0)
        return;

    // Check the block header's allocated flag (bit4 of flags at header+12).
    // header = ptr - 16; if bit4 is NOT set, the block is already free.
    uint32_t headerAddr = ptr - 16;
    uint32_t flags = PPC_LOAD_U32(headerAddr + 12);

    if ((flags & 0x10) == 0) {
        // Block is already free. This is a definitive double-free — the
        // sub_82848B68 inner free clears this bit on first free.
        std::lock_guard<std::mutex> lock(g_freedMutex);
        g_doubleFreeCount++;

        if (g_doubleFreeCount <= 64 ||
            (g_doubleFreeCount & 0xFF) == 0)
        {
            uint32_t guardWord = PPC_LOAD_U32(headerAddr + 0);
            uint32_t blockSize = PPC_LOAD_U32(headerAddr + 4);
            LOGF_WARNING("HeapGuard: blocked double-free at "
                "sysMemSimpleAllocator::Free for 0x{:08X} "
                "(header=0x{:08X}, size={}, flags=0x{:08X}, guard=0x{:08X})",
                ptr, headerAddr, blockSize, flags, guardWord);
        }
        return;
    }

    ctx.r3.u32 = allocator;
    ctx.r4.u32 = ptr;
    __imp__sub_828494D8(ctx, base);
}
