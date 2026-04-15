/* Minimal libc++ atomic-wait implementation for PS4.
 *
 * LLVM 21 libc++ uses a global hash-table–based parking lot for
 * std::atomic::wait/notify.  The runtime functions live in libc++.so
 * which doesn't exist on PS4.  We provide a thin polling fallback.
 */

#include <__atomic/atomic_sync.h>

namespace std { inline namespace __1 {

__cxx_contention_t __atomic_monitor_global(const void *) noexcept {
    return 0;
}

void __atomic_wait_global_table(const void *, __cxx_contention_t) noexcept {
    __asm__ volatile ("pause" ::: "memory");
}

void __atomic_notify_one_global_table(const void *) noexcept { }
void __atomic_notify_all_global_table(const void *) noexcept { }

}} // namespace std::__1

/* ---- Symbols missing from OpenOrbis PS4 stubs ---- */

/* libc++ thread_local destructor registration.
 * On real PS4 this is in libkernel; OpenOrbis stubs don't export it.
 * Provide a minimal implementation: register the dtor with atexit(). */
#include <cstdlib>
extern "C" int __cxa_thread_atexit_impl(void (*dtor)(void *), void *obj, void * /*dso_symbol*/) {
    /* Good-enough fallback: run at process exit instead of thread exit. */
    std::atexit(reinterpret_cast<void(*)()>(reinterpret_cast<void*>(dtor)));
    (void)obj;
    return 0;
}

/* ZSTD tracing callbacks — optional instrumentation, safe to no-op. */
extern "C" {
unsigned long long ZSTD_trace_decompress_begin(void) { return 0; }
void ZSTD_trace_decompress_end(unsigned long long) {}
}
