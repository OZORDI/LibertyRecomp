/**
 * @file    rex_xobject.h
 * @brief   XObject bridge - utilities between Liberty's KernelObject and RexGlue's XObject
 *
 * rex::kernel::XObject, object_ref, XEvent, XSemaphore, XMutant are
 * available via rex_adapter.h includes. This file provides bridge
 * utilities between Liberty's KernelObject and RexGlue's XObject.
 */

#pragma once

#include "../kernel/xdm.h"

namespace rex::adapter {

/// Get a Liberty KernelObject from guest dispatch header
template <typename T>
inline T* GetLibertyKernelObject(void *native_ptr) {
    if (!native_ptr) return nullptr;
    auto *header = reinterpret_cast<XDISPATCHER_HEADER *>(native_ptr);
    return QueryKernelObject<T>(*header);
}

/// Get Liberty KernelObject by guest address
template <typename T>
inline T* GetLibertyObjectByAddress(uint32_t guest_addr) {
    KernelObject *obj = LookupKernelObject(guest_addr);
    return static_cast<T*>(obj);
}

} // namespace rex::adapter

// =============================================================================
// Convenience Macros
// =============================================================================

#define REX_GET_OBJECT(type, ptr) rex::adapter::GetLibertyKernelObject<type>(ptr)
#define REX_QUERY_OBJECT(type, header) QueryKernelObject<type>(header)
