/**
 * @file    rex_runtime.h
 * @brief   Runtime accessors - delegates to rex::Runtime
 *
 * Provides global access points to core runtime services via rex::Runtime.
 */

#pragma once

#include "../kernel/heap.h"
#include "../kernel/memory.h"

namespace rex {
namespace adapter {

inline uint8_t* GetMembase() {
    return g_memory.base;
}

inline rex::memory::Memory* GetMemory() {
    return g_memory.GetRexMemory();
}

inline rex::system::KernelState* GetKernelState() {
    return rex::Runtime::instance()->kernel_state();
}

inline rex::runtime::Processor* GetProcessor() {
    return rex::Runtime::instance()->processor();
}

template<typename T = void>
inline T* TranslateVirtual(uint32_t guest_address) {
    auto* mem = g_memory.GetRexMemory();
    if (mem) return mem->TranslateVirtual<T>(guest_address);
    return reinterpret_cast<T*>(g_memory.Translate(guest_address));
}

inline uint32_t HostToGuestVirtual(const void* host_address) {
    auto* mem = g_memory.GetRexMemory();
    if (mem) return mem->HostToGuestVirtual(host_address);
    return g_memory.MapVirtual(host_address);
}

} // namespace adapter
} // namespace rex

// =============================================================================
// Convenience Macros
// =============================================================================

#ifndef REX_MEMBASE
#define REX_MEMBASE() rex::adapter::GetMembase()
#endif

#define REX_TRANSLATE(T, addr) rex::adapter::TranslateVirtual<T>(addr)
#define REX_MAP_VIRTUAL(host) rex::adapter::HostToGuestVirtual(host)
