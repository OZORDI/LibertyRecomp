// vfs_cache_patches.cpp
// VFS mount-point resolution cache with proper invalidation
//
// Bottleneck analysis:
//   sub_82855460 (fiDevice::GetDevice) is called for every file open.
//   It performs strncmp against 8 known prefixes, then a linear scan
//   through the mount table (276-byte entries) comparing each mount name.
//   With ~20 mounts and hundreds of opens per frame during streaming,
//   this is pure overhead for paths that resolve identically every time.
//
//   The mount table lives at global 0x831AB940:
//     +0: uint32_t entries_ptr  (heap-allocated array of 276-byte slots)
//     +4: uint16_t count        (active mount count)
//     +6: uint16_t capacity
//
//   Each 276-byte mount slot:
//     +0..263:   mount name string
//     +264:      name_len (uint16)
//     +268:      device pointer array
//     +272:      device_count (uint16)
//
//   sub_82855460 returns a fiDevice* for a given path string.
//   The mapping only changes when mounts are added (sub_82855A18)
//   or removed (sub_82855D48), which happens rarely (engine init,
//   DLC load, title update overlay).
//
// Strategy:
//   1. Hook sub_82855460: cache path prefix -> device result.
//      Use a small hash map keyed on path hash. On mount table change,
//      invalidate the entire cache (cheap, <20 entries at any time).
//   2. Hook sub_82855A18 (Mount) and sub_82855D48 (Unmount) to
//      invalidate both the device cache AND the RPF entry cache
//      (in rpf_optimization_patches.cpp).
//
// Thread safety:
//   Mount/Unmount are called from the main thread during init.
//   GetDevice is called from streaming threads. Use a shared_mutex
//   for concurrent reads, exclusive writes on invalidation.

#include <cpu/ppc_context.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>
#include <cstring>
#include <unordered_map>
#include <shared_mutex>
#include <atomic>

// =============================================================================
// Mount table addresses (from recomp analysis of sub_82855460)
// =============================================================================

static constexpr uint32_t ADDR_MOUNT_TABLE       = 0x831AB940;  // dword_831AB940
static constexpr uint32_t ADDR_MOUNT_COUNT       = 0x831AB944;  // word_831AB944
static constexpr uint32_t ADDR_DEFAULT_DEVICE    = 0x82B07EE8;  // off_82B07EE8
static constexpr uint32_t ADDR_CACHE_DEVICE      = 0x82B08004;  // cache:/embedded: device
static constexpr uint32_t ADDR_GAME_DEVICE       = 0x82B07EF0;  // game:/dvd:/cd: device

// =============================================================================
// Device Resolution Cache
// =============================================================================
//
// Caches the full result of sub_82855460(path, flag) -> fiDevice*.
// Key: Jenkins hash of the path prefix up to and including the ':' separator.
// Value: guest address of the fiDevice* returned by the original function.
//
// The prefix extraction ensures that "game:/maps/foo.rpf" and
// "game:/audio/bar.rpf" both map to the same device via key "game:".
// This matches how GetDevice works: it finds the longest mount-name
// prefix match in the table.
//
// However, some paths resolve differently even with the same prefix
// when multiple devices are stacked on the same mount point (the
// mount table supports a device array per slot with fallback chain).
// For correctness, we cache on the FULL path hash, not just the prefix.
// The cache is small enough (~100-200 unique paths during streaming)
// that this is fine.

struct DeviceCache
{
    std::shared_mutex mutex;
    std::unordered_map<uint64_t, uint32_t> map;  // (pathHash, flags) -> device addr
    std::atomic<uint64_t> hits{0};
    std::atomic<uint64_t> misses{0};

    // Combine path hash + flags byte into cache key
    static uint64_t makeKey(uint32_t pathHash, uint8_t flags)
    {
        return (static_cast<uint64_t>(pathHash) << 8) | flags;
    }

    bool lookup(uint32_t pathHash, uint8_t flags, uint32_t& outDevice)
    {
        std::shared_lock lock(mutex);
        auto it = map.find(makeKey(pathHash, flags));
        if (it != map.end())
        {
            hits.fetch_add(1, std::memory_order_relaxed);
            outDevice = it->second;
            return true;
        }
        misses.fetch_add(1, std::memory_order_relaxed);
        return false;
    }

    void insert(uint32_t pathHash, uint8_t flags, uint32_t device)
    {
        std::unique_lock lock(mutex);
        // Limit cache size to prevent unbounded growth
        // Mount-point device lookups converge to a small set (<200 unique paths)
        if (map.size() >= 4096)
        {
            map.clear();  // Full reset is fine — repopulates in <1 frame
        }
        map[makeKey(pathHash, flags)] = device;
    }

    void invalidate()
    {
        std::unique_lock lock(mutex);
        map.clear();
    }
};

static DeviceCache g_deviceCache;

// =============================================================================
// Native Jenkins hash (same as sub_8284E060 / rpf_optimization_patches.cpp)
// Used to hash path strings on the host side without guest memory roundtrip.
// =============================================================================

static uint32_t jenkinsHash(const char* str)
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

// =============================================================================
// sub_82855460 - fiDevice::GetDevice (cached)
// =============================================================================
//
// Original ABI: r3 = path string (guest), r4 = flags byte
//               Returns fiDevice* in r3 (guest addr)
//
// The original function does:
//   1. Fast-path strncmp for "memory:", "embedded:/", "tcpip:", "cache:",
//      "cache1:", "game:", "dvd:", "dlc" — returns static device pointers
//   2. Linear scan of mount table for longest prefix match
//   3. Walks device stack (array of devices per mount point)
//   4. Falls back to default device at 0x82B07EE8
//
// Our cache stores the FINAL device pointer for each (path, flags) pair.
// Cache is invalidated on every Mount/Unmount operation.

PPC_FUNC_IMPL(__imp__sub_82855460);

PPC_FUNC_HOOK(sub_82855460)
{
    uint32_t pathAddr = ctx.r3.u32;
    uint8_t flags = static_cast<uint8_t>(ctx.r4.u32);

    // Read path from guest memory (only need enough for the cache key)
    char pathBuf[272] = {};
    if (pathAddr != 0)
    {
        for (int i = 0; i < 271; i++)
        {
            char c = static_cast<char>(PPC_LOAD_U8(pathAddr + i));
            if (c == 0) break;
            pathBuf[i] = c;
        }
    }

    uint32_t pathHash = jenkinsHash(pathBuf);

    // Cache lookup
    uint32_t cachedDevice = 0;
    if (g_deviceCache.lookup(pathHash, flags, cachedDevice))
    {
        ctx.r3.u32 = cachedDevice;
        return;
    }

    // Cache miss — call original
    __imp__sub_82855460(ctx, base);

    uint32_t result = ctx.r3.u32;

    // Cache the result
    g_deviceCache.insert(pathHash, flags, result);
}

// =============================================================================
// External entry cache invalidation
// =============================================================================
//
// The RPF entry cache in rpf_optimization_patches.cpp stores
// (fiPackfile_addr, path_hash) -> entry_addr mappings. When an RPF is
// unmounted, those entries become dangling pointers. We need to clear
// that cache on unmount.
//
// We declare the external cache's clear function here. The entry cache
// in rpf_optimization_patches.cpp exposes g_entryCache as a static
// with file-internal linkage, so we can't call it directly. Instead,
// we provide a global invalidation counter that both caches check.

static std::atomic<uint32_t> g_mountGeneration{0};

// Exposed for rpf_optimization_patches.cpp to poll
uint32_t vfs_cache_get_mount_generation()
{
    return g_mountGeneration.load(std::memory_order_acquire);
}

// =============================================================================
// sub_82855A18 - fiDevice::Mount (cache invalidation)
// =============================================================================
//
// Original ABI: r3 = mount name (guest), r4 = fiDevice** (guest), r5 = flags byte
//               Returns 1 on success, 0 on failure
//
// We call the original, then invalidate all caches if the mount succeeded.

PPC_FUNC_IMPL(__imp__sub_82855A18);

PPC_FUNC_HOOK(sub_82855A18)
{
    // Read mount name for logging
    uint32_t nameAddr = ctx.r3.u32;
    char mountName[64] = {};
    if (nameAddr != 0)
    {
        for (int i = 0; i < 63; i++)
        {
            char c = static_cast<char>(PPC_LOAD_U8(nameAddr + i));
            if (c == 0) break;
            mountName[i] = c;
        }
    }

    // Call original Mount
    __imp__sub_82855A18(ctx, base);

    uint32_t result = ctx.r3.u32;
    if (result != 0)
    {
        // Mount succeeded — invalidate device cache
        g_deviceCache.invalidate();
        g_mountGeneration.fetch_add(1, std::memory_order_release);
        LOGF_INFO("vfs_cache: Mount('{}') succeeded, caches invalidated (gen={})",
                  mountName, g_mountGeneration.load());
    }
}

// =============================================================================
// sub_82855D48 - fiDevice::Unmount (cache invalidation)
// =============================================================================
//
// Original ABI: r3 = mount name (guest)
//               Returns 1 on success, 0 on failure
//
// Unmount is more critical than mount for cache correctness: cached device
// pointers become dangling, and cached RPF entries point into freed memory.

PPC_FUNC_IMPL(__imp__sub_82855D48);

PPC_FUNC_HOOK(sub_82855D48)
{
    // Read mount name for logging
    uint32_t nameAddr = ctx.r3.u32;
    char mountName[64] = {};
    if (nameAddr != 0)
    {
        for (int i = 0; i < 63; i++)
        {
            char c = static_cast<char>(PPC_LOAD_U8(nameAddr + i));
            if (c == 0) break;
            mountName[i] = c;
        }
    }

    // Invalidate caches BEFORE unmount to prevent racing readers
    // from using a device that's about to be freed
    g_deviceCache.invalidate();
    g_mountGeneration.fetch_add(1, std::memory_order_release);

    // Call original Unmount
    __imp__sub_82855D48(ctx, base);

    uint32_t result = ctx.r3.u32;
    LOGF_INFO("vfs_cache: Unmount('{}') result={}, caches invalidated (gen={})",
              mountName, result, g_mountGeneration.load());
}
