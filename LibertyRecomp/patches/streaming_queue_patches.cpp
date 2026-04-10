// streaming_queue_patches.cpp
// Iterative replacement for GTA IV's recursive streaming ProcessQueue.
//
// Original: sub_825132D0 (1008 bytes, 545 PPC instructions)
//   - Walks a pending-load linked list embedded in 24-byte streaming entries
//   - For each entry, resolves dependencies via sub_8250D328, checks their states
//   - Dispatches ready entries via sub_825120E8 (dependency resolver) or
//     sub_82512BF0 (completion callback) depending on dep status
//   - Tail-recurses once (a3 nonzero -> 0) to retry without priority filter
//   - Uses PPC idiom id*3*8 (3 instructions) instead of id*24 (1 multiply)
//
// This hook:
//   1. Replaces the tail recursion with an outer retry loop (max 2 iterations)
//   2. Uses direct id*24 pointer arithmetic instead of PPC id*3*8 pattern
//   3. Preserves exact game semantics: same state checks, same dispatch order,
//      same callback invocations, same return values

#include <kernel/function.h>
#include <kernel/memory.h>
#include <cstdint>

// =============================================================================
// Guest memory addresses (Python-verified from pseudocode globals)
// =============================================================================

// Global entry array base pointer (all entry indices are relative to this)
static constexpr uint32_t ADDR_ENTRIES_BASE = 0x83032744;  // dword_83032744

// Dependency graph dispatch table object
static constexpr uint32_t ADDR_DEP_DISPATCH = 0x82D515B8;  // dword_82D515B8

// File offset lookup table (for priority computation)
static constexpr uint32_t ADDR_FILE_OFFSET_TABLE = 0x82D476D4;  // dword_82D476D4

// =============================================================================
// Entry layout constants (24 bytes per entry)
// =============================================================================

static constexpr uint32_t ENTRY_SIZE       = 24;

// Offsets within a 24-byte entry
static constexpr uint32_t OFF_DWORD0       = 0x00;  // 4B: field_0
static constexpr uint32_t OFF_DWORD4       = 0x04;  // 4B: type_byte(high) | file_offset(low 24)
static constexpr uint32_t OFF_STATE_REF    = 0x08;  // 4B: state(bits 30-31) | refcount(bits 0-29)
static constexpr uint32_t OFF_DEP_REFC     = 0x0C;  // 2B: dependency refcount
static constexpr uint32_t OFF_FLAGS        = 0x0E;  // 2B: request flags
static constexpr uint32_t OFF_NEXT_LINK    = 0x10;  // 2B: next index in linked list (0xFFFF = end)
static constexpr uint32_t OFF_PREV_LINK    = 0x12;  // 2B: prev index in linked list
static constexpr uint32_t OFF_MODULE_IDX   = 0x17;  // 1B: module type index (0xFF = none)

// Flag bits within OFF_FLAGS (uint16)
static constexpr uint16_t FLAG_PRIORITY    = 0x0010;  // bit 4
static constexpr uint16_t FLAG_HAS_DEPS    = 0x0800;  // bit 11
static constexpr uint16_t FLAG_REQ_MASK    = 0x00CE;  // request type bits

// State values (bits 30-31 of OFF_STATE_REF dword)
static constexpr uint32_t STATE_IDLE       = 0;
static constexpr uint32_t STATE_LOADED     = 1;
static constexpr uint32_t STATE_PENDING    = 2;
static constexpr uint32_t STATE_LOADING    = 3;

// Queue object field offsets (a1 is uint32_t* so indices are dword-based)
// a1[0]  = entries_base_ptr
// a1[4]  = pending_list_sentinel (entry whose +16 = first pending link)
// a1[5]  = end_sentinel_ptr (marks end of pending list walk)
// a1[14] = total_pending_count
// a1[15] = non_has_deps_count
// a1[16] = priority_count
static constexpr uint32_t QF_BASE       = 0;   // a1[0]
static constexpr uint32_t QF_SENTINEL   = 4;   // a1[4]
static constexpr uint32_t QF_END        = 5;   // a1[5]
static constexpr uint32_t QF_COMPLETED  = 2;   // a1[2] — completed list head (used by sub_82512BF0)
static constexpr uint32_t QF_PENDING    = 14;  // a1[14]
static constexpr uint32_t QF_NONDEP     = 15;  // a1[15]
static constexpr uint32_t QF_PRIORITY   = 16;  // a1[16]

// =============================================================================
// Forward declarations for original PPC functions we call through
// =============================================================================

PPC_FUNC_IMPL(__imp__sub_8250D328);   // GetDependencies(dispatch, entry_id, out_buf) -> count
PPC_FUNC_IMPL(__imp__sub_825120E8);   // ResolveDependency(queue, entry_id, flags)
PPC_FUNC_IMPL(__imp__sub_82512BF0);   // CompletionCallback(queue, entry_id)
PPC_FUNC_IMPL(__imp__sub_825132D0);   // Original ProcessQueue (we replace this)

// =============================================================================
// Helper: compute entry pointer from index using direct multiply
// =============================================================================

static inline uint32_t EntryPtr(uint32_t base, uint16_t idx) {
    return base + static_cast<uint32_t>(idx) * ENTRY_SIZE;
}

// Helper: get state from entry's +0x08 dword (bits 30-31)
static inline uint32_t EntryState(uint32_t entry_ptr) {
    return PPC_LOAD_U32(entry_ptr + OFF_STATE_REF) >> 30;
}

// Helper: get flags from entry's +0x0E
static inline uint16_t EntryFlags(uint32_t entry_ptr) {
    return PPC_LOAD_U16(entry_ptr + OFF_FLAGS);
}

// Helper: entry has non-zero refcount OR has_deps flag set
static inline bool EntryHasWork(uint32_t entry_ptr) {
    uint32_t state_ref = PPC_LOAD_U32(entry_ptr + OFF_STATE_REF);
    uint32_t refcount = state_ref & 0x3FFFFFFF;
    if (refcount != 0) return true;
    uint16_t flags = PPC_LOAD_U16(entry_ptr + OFF_FLAGS);
    return (flags & FLAG_HAS_DEPS) != 0;
}

// Helper: compute file offset for priority comparison
// Reads type_byte from +4, looks up base in file offset table, adds low 24 bits
static inline uint32_t EntryFileOffset(uint32_t entry_ptr) {
    uint32_t dword4 = PPC_LOAD_U32(entry_ptr + OFF_DWORD4);
    uint8_t type_byte = static_cast<uint8_t>(dword4 >> 24);
    uint32_t local_offset = dword4 & 0x00FFFFFF;

    // Original PPC: dword_82D476D4[8 * type_byte + 8 * ROL4(type_byte, 2)]
    // ROL4(x, 2) for a byte = (x << 2) | (x >> 30) but x is u8 so high bits are 0
    // So it's just (x << 2) & 0xFFFFFFFF for small x.
    // Index = 8*type_byte + 8*(type_byte << 2) = 8*type_byte + 32*type_byte = 40*type_byte
    // But dword_82D476D4 is a dword array, so byte offset = 40*type_byte * 4 = 160*type_byte
    // Actually: dword_82D476D4[index] where index = 8*type_byte + 8*ROL4(type_byte,2)
    // ROL4(byte,2) = byte*4 for small values. index = 8*byte + 8*(byte*4) = 8*byte + 32*byte = 40*byte
    // As dword index: table_addr + 40*type_byte*4 = table_addr + 160*type_byte
    uint32_t table_addr = PPC_LOAD_U32(ADDR_FILE_OFFSET_TABLE + 160u * type_byte);
    return table_addr + local_offset;
}

// =============================================================================
// PPC_FUNC_HOOK: sub_825132D0 — Iterative ProcessQueue
// =============================================================================
//
// Signature: int ProcessQueue(int* a1, uint32_t min_offset, uint8_t check_priority)
// Returns: best entry index (uint16_t) or 0xFFFF if none found
//
// r3 = a1 (queue object pointer, guest address)
// r4 = a2 (minimum file offset for candidate selection)
// r5 = a3 (nonzero = check priority filter on first pass)

PPC_FUNC_HOOK(sub_825132D0) {
    uint32_t queue_addr = ctx.r3.u32;
    uint32_t min_offset = ctx.r4.u32;
    uint8_t check_priority = static_cast<uint8_t>(ctx.r5.u32);

    // Allocate guest stack frame (256 bytes, matching original)
    // dep buffer at sp+80 (176 bytes = 44 dwords for dependency IDs)
    uint32_t saved_r1 = ctx.r1.u32;
    ctx.r1.u32 -= 256;
    PPC_STORE_U32(ctx.r1.u32, saved_r1);  // back-chain pointer
    uint32_t dep_buf_ea = ctx.r1.u32 + 80;
    uint16_t final_result = 0xFFFF;

    // Retry loop replaces the single tail recursion
    // Original recurses at most once: a3=nonzero -> a3=0
    for (int retry = 0; retry < 2; ++retry) {
        uint32_t best_offset_ge = 0xFFFFFFFF;  // v5: best offset >= min_offset
        uint32_t best_offset_any = 0xFFFFFFFF;  // v6: best offset overall
        bool had_unlink = false;                 // v7: set if any entry was unlinked
        uint16_t best_id_ge = 0xFFFF;           // v10: best candidate (offset >= a2)
        uint16_t best_id_any = 0xFFFF;          // v8/v10: best candidate (any offset)

        // Read entries base and list pointers
        uint32_t entries_base = PPC_LOAD_U32(queue_addr + QF_BASE * 4);

        // Get pending list head: sentinel's +16 holds the first link index
        uint32_t sentinel_ptr = PPC_LOAD_U32(queue_addr + QF_SENTINEL * 4);
        uint16_t first_link = PPC_LOAD_U16(sentinel_ptr + OFF_NEXT_LINK);

        // Compute first entry pointer from link index
        uint32_t cur_entry;
        if (first_link == 0xFFFF) {
            cur_entry = 0;
        } else {
            cur_entry = EntryPtr(entries_base, first_link);
        }

        // End sentinel
        uint32_t end_sentinel = PPC_LOAD_U32(queue_addr + QF_END * 4);

        // Early exit: empty list
        if (cur_entry == end_sentinel) {
            // Fall through to tail-recursion check below
            best_id_any = 0xFFFF;
            best_id_ge = 0xFFFF;
            goto tail_check;
        }

        // Main loop: walk the pending linked list iteratively
        while (cur_entry != end_sentinel) {
            // Compute current entry's index using direct divide
            // entry_id = (cur_entry - entries_base) / 24
            uint32_t entry_id = (cur_entry - entries_base) / ENTRY_SIZE;

            // Read next link BEFORE we potentially unlink this entry
            uint16_t next_link = PPC_LOAD_U16(cur_entry + OFF_NEXT_LINK);
            uint32_t next_entry;
            if (next_link == 0xFFFF) {
                next_entry = 0;
            } else {
                next_entry = EntryPtr(entries_base, next_link);
            }

            // Priority filter: if check_priority AND queue has priority entries AND
            // this entry does NOT have priority bit set -> skip
            if (check_priority && PPC_LOAD_U32(queue_addr + QF_PRIORITY * 4) != 0) {
                uint16_t flags = EntryFlags(cur_entry);
                if (((flags >> 4) & 1) == 0) {
                    // Skip this entry (no priority flag)
                    cur_entry = next_entry;
                    continue;
                }
            }

            // Get dependencies for this entry
            // sub_8250D328(dep_dispatch_table, entry_id, out_buffer) -> dep_count
            ctx.r3.u32 = ADDR_DEP_DISPATCH;
            ctx.r4.u32 = entry_id;
            ctx.r5.u32 = dep_buf_ea;
            __imp__sub_8250D328(ctx, base);
            int32_t dep_count = static_cast<int32_t>(ctx.r3.s32);

            // Check if all dependencies are in state LOADED(1) or LOADING(3)
            int32_t deps_ready = 0;
            bool all_deps_satisfied = true;

            if (dep_count > 0) {
                // Re-read entries_base (it could change after calls, though unlikely)
                entries_base = PPC_LOAD_U32(queue_addr + QF_BASE * 4);

                for (int32_t i = 0; i < dep_count; ++i) {
                    uint32_t dep_id = PPC_LOAD_U32(dep_buf_ea + i * 4);
                    uint32_t dep_entry = EntryPtr(entries_base, static_cast<uint16_t>(dep_id));
                    uint32_t dep_state = EntryState(dep_entry);

                    if (dep_state == STATE_LOADED || dep_state == STATE_LOADING) {
                        deps_ready++;
                    } else {
                        // Dep not ready -> dispatch it via sub_825120E8
                        // Compute I/O flags from current entry
                        entries_base = PPC_LOAD_U32(queue_addr + QF_BASE * 4);
                        uint32_t my_entry = EntryPtr(entries_base, static_cast<uint16_t>(entry_id));
                        uint16_t my_flags = PPC_LOAD_U16(my_entry + OFF_FLAGS);

                        uint16_t io_flags = 512;  // base flag = 0x200
                        if ((my_flags & FLAG_REQ_MASK) != 0) {
                            io_flags = 520;  // 0x208
                        }
                        uint16_t priority_bit = my_flags & FLAG_PRIORITY;

                        // sub_825120E8(queue, dep_id, flags)
                        ctx.r3.u32 = queue_addr;
                        ctx.r4.u32 = dep_id;
                        ctx.r5.u32 = static_cast<uint32_t>(priority_bit | io_flags);
                        __imp__sub_825120E8(ctx, base);

                        all_deps_satisfied = false;
                        break;
                    }
                }
            }

            if (!all_deps_satisfied) {
                // Move to next entry
                cur_entry = next_entry;
                continue;
            }

            // deps_ready == dep_count: all dependencies are satisfied
            // Re-read entries base and current entry pointer
            entries_base = PPC_LOAD_U32(queue_addr + QF_BASE * 4);
            cur_entry = EntryPtr(entries_base, static_cast<uint16_t>(entry_id));

            uint16_t flags = EntryFlags(cur_entry);

            // Check has_dependencies flag (bit 11)
            if ((flags & FLAG_HAS_DEPS) != 0) {
                // All deps ready AND has_deps set -> check if ALL deps are state LOADED (not LOADING)
                had_unlink = true;

                // Save next entry and advance cur_entry now (we're about to unlink)
                // Re-read next link since cur_entry may have been modified
                next_link = PPC_LOAD_U16(cur_entry + OFF_NEXT_LINK);
                if (next_link == 0xFFFF) {
                    next_entry = 0;
                } else {
                    entries_base = PPC_LOAD_U32(queue_addr + QF_BASE * 4);
                    next_entry = EntryPtr(entries_base, next_link);
                }

                // Check if ALL deps are specifically state LOADED (1), not LOADING (3)
                int32_t all_loaded_count = 0;
                entries_base = PPC_LOAD_U32(queue_addr + QF_BASE * 4);
                for (int32_t i = 0; i < dep_count; ++i) {
                    uint32_t dep_id = PPC_LOAD_U32(dep_buf_ea + i * 4);
                    uint32_t dep_entry = EntryPtr(entries_base, static_cast<uint16_t>(dep_id));
                    uint32_t dep_state = EntryState(dep_entry);
                    if (dep_state != STATE_LOADED) break;
                    all_loaded_count++;
                }

                if (all_loaded_count == dep_count) {
                    // ALL deps fully loaded -> unlink entry from pending list and complete
                    entries_base = PPC_LOAD_U32(queue_addr + QF_BASE * 4);
                    uint32_t this_entry = EntryPtr(entries_base, static_cast<uint16_t>(entry_id));

                    // Read this entry's prev/next link indices
                    uint16_t my_next = PPC_LOAD_U16(this_entry + OFF_NEXT_LINK);
                    uint16_t my_prev = PPC_LOAD_U16(this_entry + OFF_PREV_LINK);

                    // Doubly-linked list unlink:
                    // next_entry.prev = my_prev
                    PPC_STORE_U16(EntryPtr(entries_base, my_next) + OFF_PREV_LINK, my_prev);
                    // prev_entry.next = my_next
                    PPC_STORE_U16(EntryPtr(entries_base, my_prev) + OFF_NEXT_LINK, my_next);

                    // Clear this entry's links
                    PPC_STORE_U16(this_entry + OFF_NEXT_LINK, 0xFFFF);
                    PPC_STORE_U16(this_entry + OFF_PREV_LINK, 0xFFFF);

                    // Decrement pending count
                    uint32_t pending = PPC_LOAD_U32(queue_addr + QF_PENDING * 4);
                    PPC_STORE_U32(queue_addr + QF_PENDING * 4, pending - 1);

                    // If entry does NOT have has_deps, also decrement non-dep count
                    // (But we're in the has_deps branch, so flags & FLAG_HAS_DEPS is true)
                    // Original: if ((flags >> 11) & 1) == 0 then --a1[15]
                    // Since has_deps IS set, we do NOT decrement a1[15]

                    // If entry has priority flag, decrement priority count and clear flag
                    if ((flags & FLAG_PRIORITY) != 0) {
                        uint16_t cleared = flags & ~FLAG_PRIORITY;
                        PPC_STORE_U16(this_entry + OFF_FLAGS, cleared);
                        uint32_t prio_cnt = PPC_LOAD_U32(queue_addr + QF_PRIORITY * 4);
                        PPC_STORE_U32(queue_addr + QF_PRIORITY * 4, prio_cnt - 1);
                    }

                    // Call completion callback: sub_82512BF0(queue, entry_id)
                    ctx.r3.u32 = queue_addr;
                    ctx.r4.u32 = entry_id;
                    __imp__sub_82512BF0(ctx, base);

                    // Re-read entries_base after callback (it may modify the array)
                    entries_base = PPC_LOAD_U32(queue_addr + QF_BASE * 4);
                }

                // Advance to next entry (already computed before unlink)
                cur_entry = next_entry;
                continue;
            }

            // No has_deps flag -> this is a candidate for best-entry selection
            // Compute file offset for priority ranking
            uint32_t file_offset = 0;
            if (EntryHasWork(cur_entry)) {
                file_offset = EntryFileOffset(cur_entry);
            }

            if (file_offset < best_offset_any) {
                best_offset_any = file_offset;
                best_id_any = static_cast<uint16_t>(entry_id);
            }
            if (file_offset < best_offset_ge && file_offset >= min_offset) {
                best_offset_ge = file_offset;
                best_id_ge = static_cast<uint16_t>(entry_id);
            }

            // Advance to next entry
            cur_entry = next_entry;
        }

tail_check:
        // Select best candidate: prefer one with offset >= min_offset
        // Original: v10 = v8; if v10(=v8_ge) != 0xFFFF return v10;
        // Then: if v8 != 0xFFFF || !a1[16] -> return v10
        if (best_id_ge != 0xFFFF) {
            final_result = best_id_ge;
            goto done;
        }

        // Fallback to best_any
        if (best_id_any != 0xFFFF) {
            final_result = best_id_any;
            goto done;
        }

        // No candidate found. Check if we should retry.
        {
            uint32_t prio_count = PPC_LOAD_U32(queue_addr + QF_PRIORITY * 4);
            if (prio_count == 0) {
                // No priority entries -> return 0xFFFF
                final_result = 0xFFFF;
                goto done;
            }

            // Here: no candidate AND priority_count > 0
            // Original: if (pending != 1 || !had_unlink) -> RECURSE with a3=0
            //           else (pending == 1 AND had_unlink) -> return 0xFFFF
            uint32_t pending_count = PPC_LOAD_U32(queue_addr + QF_PENDING * 4);
            if (pending_count == 1 && had_unlink) {
                // Single pending entry with a completed unlink -> give up
                final_result = 0xFFFF;
                goto done;
            }

            // Retry without priority filter (replaces tail-recursive call with a3=0)
            check_priority = 0;
            // Loop back to retry
        }
    }

    // Exhausted retry attempts (should not normally reach here)
    final_result = 0xFFFF;

done:
    // Restore guest stack frame
    ctx.r1.u32 = saved_r1;
    ctx.r3.u32 = static_cast<uint32_t>(final_result);
}
