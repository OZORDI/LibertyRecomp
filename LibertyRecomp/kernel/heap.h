#pragma once

// =============================================================================
// Guest-memory allocator — thin wrapper around RexGlue's Memory system.
// Liberty uses this to place data structures in guest-addressable memory
// (GPU resources, audio buffers, mod data, stdx containers, etc.).
// Game code's own malloc (sub_8218BF20) runs natively through RexGlue's
// NtAllocateVirtualMemory — it is NOT hooked here.
// =============================================================================

struct Heap
{
    void Init();

    void* Alloc(size_t size);
    void* AllocPhysical(size_t size, size_t alignment);
    void Free(void* ptr);

    size_t Size(void* ptr);

    template<typename T, typename... Args>
    T* Alloc(Args&&... args)
    {
        T* obj = (T*)Alloc(sizeof(T));
        new (obj) T(std::forward<Args>(args)...);
        return obj;
    }

    template<typename T, typename... Args>
    T* AllocPhysical(Args&&... args)
    {
        T* obj = (T*)AllocPhysical(sizeof(T), alignof(T));
        new (obj) T(std::forward<Args>(args)...);
        return obj;
    }
};

extern Heap g_userHeap;
