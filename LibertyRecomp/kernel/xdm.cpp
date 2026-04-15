#include <stdafx.h>
#include "xdm.h"
#include "freelist.h"
#include <unordered_map>

Mutex g_kernelLock;

// Thread-safe object table: maps guest dispatcher header address → host KernelObject*
// This replaces storing host pointers in guest memory, which was vulnerable to corruption
static std::unordered_map<uint32_t, KernelObject*> g_kernelObjectTable;

KernelObject* LookupKernelObject(uint32_t guestAddr)
{
    auto it = g_kernelObjectTable.find(guestAddr);
    if (it != g_kernelObjectTable.end())
        return it->second;
    return nullptr;
}

void RegisterKernelObject(uint32_t guestAddr, KernelObject* obj)
{
    g_kernelObjectTable[guestAddr] = obj;
}

void UnregisterKernelObject(KernelObject* obj)
{
    // Remove by value (reverse lookup)
    for (auto it = g_kernelObjectTable.begin(); it != g_kernelObjectTable.end(); ++it) {
        if (it->second == obj) {
            g_kernelObjectTable.erase(it);
            return;
        }
    }
}

void DestroyKernelObject(KernelObject* obj)
{
    std::lock_guard guard{ g_kernelLock };
    UnregisterKernelObject(obj);
    obj->~KernelObject();
    g_userHeap.Free(obj);
}

uint32_t GetKernelHandle(KernelObject* obj)
{
    assert(obj != GetInvalidKernelObject());
    return g_memory.MapVirtual(obj);
}

void DestroyKernelObject(uint32_t handle)
{
    DestroyKernelObject(GetKernelObject(handle));
}

bool IsKernelObject(uint32_t handle)
{
    return (handle & 0x80000000) != 0;
}

bool IsKernelObject(void* obj)
{
    return IsKernelObject(g_memory.MapVirtual(obj));
}

bool IsInvalidKernelObject(void* obj)
{
    return obj == GetInvalidKernelObject();
}
