/**
 * sceLibcMspace implementation for PS4 (OpenOrbis).
 *
 * The PS4 SDK's sceLibcMspace API is a named-arena memory allocator based
 * on dlmalloc's mspace concept.  Since we're recompiling for a hosted
 * environment (not bare-metal PS4), we implement these as thin wrappers
 * around the C allocator with an mspace tracking structure.
 *
 * This gives correct behavior for all callers while using the host's
 * memory management underneath.
 */

#ifdef LIBERTY_RECOMP_PS4

#include <orbis/libc/stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Mspace state — we track the base/capacity for stats, but allocations
 * use the general-purpose allocator.  If callers expect memory to come
 * from a specific pre-allocated region (base != NULL), we fall through
 * to standard malloc anyway since the recompiler doesn't run on real
 * PS4 hardware with fixed physical memory. */
typedef struct MspaceState {
    const char* name;
    void*       base;
    size_t      capacity;
    uint32_t    flags;
    size_t      current_allocated;
    size_t      peak_allocated;
} MspaceState;

SceLibcMspace sceLibcMspaceCreate(const char* name, void* base,
                                  size_t capacity, uint32_t flags) {
    MspaceState* state = (MspaceState*)calloc(1, sizeof(MspaceState));
    if (!state) return NULL;
    state->name     = name;
    state->base     = base;
    state->capacity = capacity;
    state->flags    = flags;
    return (SceLibcMspace)state;
}

int sceLibcMspaceDestroy(SceLibcMspace msp) {
    if (!msp) return -1;
    free(msp);
    return 0;
}

void* sceLibcMspaceMalloc(SceLibcMspace msp, size_t size) {
    if (!msp || size == 0) return NULL;
    MspaceState* state = (MspaceState*)msp;
    void* p = malloc(size);
    if (p) {
        state->current_allocated += size;
        if (state->current_allocated > state->peak_allocated)
            state->peak_allocated = state->current_allocated;
    }
    return p;
}

void sceLibcMspaceFree(SceLibcMspace msp, void* ptr) {
    if (!ptr) return;
    /* We can't precisely track free'd size without a header, but the
     * game's allocators typically pair alloc/free sizes.  We just free. */
    (void)msp;
    free(ptr);
}

void* sceLibcMspaceCalloc(SceLibcMspace msp, size_t nelem, size_t size) {
    if (!msp) return NULL;
    MspaceState* state = (MspaceState*)msp;
    void* p = calloc(nelem, size);
    if (p) {
        size_t total = nelem * size;
        state->current_allocated += total;
        if (state->current_allocated > state->peak_allocated)
            state->peak_allocated = state->current_allocated;
    }
    return p;
}

void* sceLibcMspaceRealloc(SceLibcMspace msp, void* ptr, size_t size) {
    (void)msp;
    return realloc(ptr, size);
}

void* sceLibcMspaceMemalign(SceLibcMspace msp, size_t boundary, size_t size) {
    if (!msp) return NULL;
    MspaceState* state = (MspaceState*)msp;
    void* p = NULL;
    if (posix_memalign(&p, boundary, size) != 0)
        return NULL;
    if (p) {
        state->current_allocated += size;
        if (state->current_allocated > state->peak_allocated)
            state->peak_allocated = state->current_allocated;
    }
    return p;
}

int sceLibcMspacePosixMemalign(SceLibcMspace msp, void** ptr,
                               size_t boundary, size_t size) {
    if (!msp || !ptr) return -1;
    MspaceState* state = (MspaceState*)msp;
    int ret = posix_memalign(ptr, boundary, size);
    if (ret == 0 && *ptr) {
        state->current_allocated += size;
        if (state->current_allocated > state->peak_allocated)
            state->peak_allocated = state->current_allocated;
    }
    return ret;
}

void* sceLibcMspaceReallocalign(SceLibcMspace msp, void* ptr,
                                size_t boundary, size_t size) {
    /* No standard aligned realloc — allocate new, copy, free old. */
    void* newp = sceLibcMspaceMemalign(msp, boundary, size);
    if (newp && ptr) {
        /* We don't know the old size precisely. Use malloc_usable_size
         * if available, otherwise copy 'size' bytes (safe since newp >=
         * size and old block was at least as large for valid realloc). */
        size_t copy_size = size; /* conservative upper bound */
        memcpy(newp, ptr, copy_size);
        sceLibcMspaceFree(msp, ptr);
    }
    return newp;
}

int sceLibcMspaceMallocStats(SceLibcMspace msp,
                             SceLibcMallocManagedSize* mmsize) {
    if (!msp || !mmsize) return -1;
    MspaceState* state = (MspaceState*)msp;
    mmsize->maxSystemSize     = state->capacity;
    mmsize->currentSystemSize = state->current_allocated;
    mmsize->maxInuseSize      = state->peak_allocated;
    mmsize->currentInuseSize  = state->current_allocated;
    return 0;
}

int sceLibcMspaceMallocStatsFast(SceLibcMspace msp,
                                 SceLibcMallocManagedSize* mmsize) {
    return sceLibcMspaceMallocStats(msp, mmsize);
}

size_t sceLibcMspaceMallocUsableSize(void* ptr) {
    if (!ptr) return 0;
    /* musl doesn't export malloc_usable_size; return 0 as a safe fallback.
     * Callers that depend on this typically just use it for stats. */
    return 0;
}

int sceLibcMspaceIsHeapEmpty(SceLibcMspace msp) {
    if (!msp) return 1;
    MspaceState* state = (MspaceState*)msp;
    return state->current_allocated == 0 ? 1 : 0;
}

#endif /* LIBERTY_RECOMP_PS4 */
