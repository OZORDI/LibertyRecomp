// rpf_optimization_patches.cpp
// Performance optimizations for RAGE fiPackfile (RPF) subsystem
//
// Two high-impact optimizations:
//   1. sub_8284E060 (Jenkins hash) - 1287 callers, hottest function in the binary.
//      Replace recompiled PPC with native C++ for ~4x speedup.
//   2. sub_8285E500 (RPF entry binary search) - repeated lookups for same paths.
//      Add a hash map cache to skip binary search on cache hit.
//
// NOT hooked (future work):
//   sub_8285E710 (compressed RPF read) - requires reimplementing full inflate
//   state machine with larger buffers. Only 2 callers, lower priority.

#include <cpu/ppc_context.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>
#include <cstring>
#include <unordered_map>
#include <shared_mutex>

// =============================================================================
// 1. sub_8284E060 - Native Jenkins One-at-a-Time Hash
// =============================================================================
//
// The hottest function in the GTA IV binary with 1287 callers. Every RPF path
// lookup, shader name resolution, and string table search goes through this.
//
// Algorithm (from IDA pseudocode + PPC recomp verification):
//   - If first char is '"', skip it and stop at closing '"'
//   - For each char: lowercase A-Z -> a-z, backslash -> '/'
//   - Jenkins one-at-a-time: hash += c; hash += hash<<10; hash ^= hash>>6
//   - Finalizer: hash += hash<<3; hash ^= hash>>11; hash += hash<<15
//
// ABI: r3 = string pointer (guest), r4 = seed (uint32_t)
//       Returns hash in r3
//
// Verified against Python reference with known test vectors:
//   hash("common:/shaders", 0) = 0xCDD08BAF
//   hash("COMMON:/SHADERS", 0) = 0xCDD08BAF (case insensitive)
//   hash("a\\b\\c", 0) == hash("a/b/c", 0)  (backslash normalization)
//   hash("a", 0) = 0xCA2E9442
//

PPC_FUNC_HOOK(sub_8284E060)
{
    const uint8_t* str = base + ctx.r3.u32;
    uint32_t hash = ctx.r4.u32;

    // Check for quoted string: if first char is '"', skip it and terminate at
    // closing '"'. This matches the PPC code's extsb + cntlzw + rlwinm logic.
    bool quoted = false;
    if (*str == '"')
    {
        quoted = true;
        ++str;
    }

    while (true)
    {
        uint8_t raw = *str;
        // Sign-extend byte to match PPC extsb instruction.
        // For standard ASCII (0-127) this is a no-op.
        // For bytes > 127, sign extension gives 0xFFFFFFxx when added to hash.
        int8_t c = static_cast<int8_t>(raw);

        if (c == 0)
            break;
        if (quoted && c == '"')
            break;

        ++str;

        // Case fold: A-Z -> a-z (PPC: cmpwi 65, cmpwi 90, addi 32)
        if (c >= 'A' && c <= 'Z')
        {
            c += 32;
        }
        // Backslash -> forward slash (PPC: cmpwi 92, li 47)
        else if (c == '\\')
        {
            c = '/';
        }

        // Jenkins one-at-a-time body
        // PPC: add r11,r11,r8 / rlwinm r8,r11,10,... / add r11,r8,r11 / rlwinm / xor
        uint32_t uc = static_cast<uint32_t>(static_cast<int32_t>(c));
        hash += uc;
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    // Jenkins finalizer
    // PPC: rlwinm r11,r8,3,... / add / rlwinm r10,r11,21,... / xor / rlwinm r10,r11,15,... / add
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    ctx.r3.u32 = hash;
}

// =============================================================================
// 2. sub_8285E500 - Cached RPF Entry Lookup
// =============================================================================
//
// Called by 8 fiPackfile vtable methods (open, exists, getSize, etc.).
// Each call does a per-component binary search through the RPF directory tree:
//   1. Split path at '/' or '\\'
//   2. Hash each component with sub_8284E060
//   3. Binary search 16-byte entries comparing hash or string
//   4. Descend into subdirectory if entry has the directory bit set
//   5. Return entry pointer or 0
//
// Since RPF contents are immutable at runtime (mounted once, never modified),
// we can cache the full-path -> entry-address mapping in a native hash map.
// The cache key is (fiPackfile address, full path hash) packed into uint64.
//
// ABI: r3 = fiPackfile* this (guest addr), r4 = path string (guest addr)
//       Returns entry pointer in r3 (guest addr, or 0)
//

// Native Jenkins hash for cache key generation — same algorithm as the hook
// above, but operates on host-side char* (already copied from guest memory).
static uint32_t nativeJenkinsHash(const char* str)
{
    uint32_t hash = 0;
    bool quoted = false;
    if (*str == '"')
    {
        quoted = true;
        ++str;
    }

    while (*str)
    {
        int8_t c = static_cast<int8_t>(*str);
        if (quoted && c == '"')
            break;
        ++str;

        if (c >= 'A' && c <= 'Z')
            c += 32;
        else if (c == '\\')
            c = '/';

        hash += static_cast<uint32_t>(static_cast<int32_t>(c));
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

// Mount generation counter from vfs_cache_patches.cpp
// When this changes, the mount table was modified and cached entries may be stale.
extern uint32_t vfs_cache_get_mount_generation();

// Entry cache: maps (fiPackfile_addr << 32 | path_hash) -> entry_guest_addr
// Thread-safe via shared_mutex (reads can be concurrent, writes are exclusive).
// Automatically invalidated when mount generation changes.
// Bounded to 4096 entries to prevent unbounded growth during streaming.
struct RPFEntryCache
{
    std::shared_mutex mutex;
    std::unordered_map<uint64_t, uint32_t> map;
    uint32_t lastMountGen = 0;

    static constexpr size_t MAX_ENTRIES = 4096;

    static uint64_t makeKey(uint32_t packfileAddr, uint32_t pathHash)
    {
        return (static_cast<uint64_t>(packfileAddr) << 32) | pathHash;
    }

    bool lookup(uint32_t packfileAddr, uint32_t pathHash, uint32_t& outEntry)
    {
        // Check mount generation before acquiring lock (cheap atomic read)
        uint32_t currentGen = vfs_cache_get_mount_generation();

        std::shared_lock lock(mutex);
        if (currentGen != lastMountGen)
        {
            // Generation changed — need to invalidate under exclusive lock
            lock.unlock();
            {
                std::unique_lock wlock(mutex);
                // Double-check after acquiring write lock
                if (currentGen != lastMountGen)
                {
                    map.clear();
                    lastMountGen = currentGen;
                }
            }
            return false;
        }

        auto it = map.find(makeKey(packfileAddr, pathHash));
        if (it != map.end())
        {
            outEntry = it->second;
            return true;
        }
        return false;
    }

    void insert(uint32_t packfileAddr, uint32_t pathHash, uint32_t entry)
    {
        std::unique_lock lock(mutex);
        // Evict entire cache if it grows too large (simple but effective —
        // streaming converges to a working set well under 4096 paths)
        if (map.size() >= MAX_ENTRIES)
        {
            map.clear();
        }
        map.emplace(makeKey(packfileAddr, pathHash), entry);
    }
};

static RPFEntryCache g_rpfEntryCache;

PPC_FUNC_IMPL(__imp__sub_8285E500);

PPC_FUNC_HOOK(sub_8285E500)
{
    uint32_t packfileAddr = ctx.r3.u32;
    uint32_t pathGuestAddr = ctx.r4.u32;

    // Read path string from guest memory (RPF paths are max ~256 chars)
    char pathBuf[260];
    pathBuf[0] = '\0';
    if (pathGuestAddr != 0)
    {
        for (int i = 0; i < 259; i++)
        {
            char c = static_cast<char>(PPC_LOAD_U8(pathGuestAddr + i));
            pathBuf[i] = c;
            if (c == 0) break;
        }
        pathBuf[259] = '\0';
    }

    // Compute full-path hash for cache key (normalizes case + slashes)
    uint32_t pathHash = nativeJenkinsHash(pathBuf);

    // Cache lookup (shared lock — fast concurrent reads)
    uint32_t cachedEntry = 0;
    if (g_rpfEntryCache.lookup(packfileAddr, pathHash, cachedEntry))
    {
        // Validate: ensure the RPF entry table is still mapped.
        // fiPackfile+8 holds the entry table base pointer.
        uint32_t entryTableBase = PPC_LOAD_U32(packfileAddr + 8);
        if (entryTableBase != 0)
        {
            ctx.r3.u32 = cachedEntry;
            return;
        }
        // Entry table is gone (RPF was unmounted) — fall through to re-lookup.
        // This is extremely rare in practice since RPFs stay mounted for the
        // entire game session.
    }

    // Cache miss — call original binary search implementation
    __imp__sub_8285E500(ctx, base);

    uint32_t result = ctx.r3.u32;

    // Store in cache. We cache both hits (entry addr) and misses (0) to
    // avoid repeated binary searches for paths that don't exist.
    g_rpfEntryCache.insert(packfileAddr, pathHash, result);
}
