// kernel_sync.h — Shared signaling helpers for guest kernel objects
//
// These functions look up guest event/semaphore handles in the kernel object
// table and signal them via the host OS. Used by streaming, audio, and
// per-frame sync code throughout the project.

#pragma once

#include <cstdint>

// Signal a guest XEvent by its guest memory address.
// Looks up the handle in kernel_state's object table and calls Set().
// No-op if the address is unmapped or the object is not an XEvent.
void SignalEventByGuestAddr(uint32_t guestAddr);

// Release a guest XSemaphore by its guest memory address.
// Increments the semaphore count by |count|.
// No-op if the address is unmapped or the object is not an XSemaphore.
void SignalSemaphoreByGuestAddr(uint32_t guestAddr, int32_t count = 1);
