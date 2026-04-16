/**
 * @file        core/fiber_ios.cpp
 * @brief       iOS backend for rex::thread::Fiber
 *
 * makecontext / swapcontext / getcontext are deprecated on macOS since 10.6
 * and completely unavailable on iOS (the ucontext.h symbols resolve at build
 * time but every call returns an error at runtime, and on arm64 the saved
 * context does not include the FPU/SIMD state needed for guest code). We
 * therefore take the same approach as the Switch backend (fiber_switch.cpp):
 * save/restore callee-saved integer + SIMD registers via setjmp/longjmp and
 * perform an explicit AArch64 stack pivot for the first trampoline call.
 *
 * This works on both arm64 and arm64e devices. On arm64e the stack pivot is
 * a plain `mov sp, x*` which does not interact with pointer authentication;
 * the subsequent `br` to the trampoline uses an unsigned (raw) function
 * pointer so no PAC discriminator is needed.
 *
 * @license     BSD 3-Clause License
 */

#include <rex/platform.h>

#if REX_PLATFORM_IOS

#include <rex/thread/fiber.h>

#include <cassert>
#include <csetjmp>
#include <cstdlib>
#include <cstring>

// The fiber struct reserves 512 bytes for the jmp_buf. Darwin's jmp_buf on
// arm64 is (14 * 8) + (8 * 8) + 8 + 8 = 192 bytes, but _JBLEN is platform
// dependent — check at compile time.
static_assert(sizeof(jmp_buf) <= 512, "jmp_buf exceeds Fiber::jmpbuf_ storage");

namespace rex::thread {

thread_local Fiber* Fiber::tls_current_ = nullptr;

#define FIBER_JMP_BUF(f) (*reinterpret_cast<jmp_buf*>((f)->jmpbuf_))

Fiber* Fiber::ConvertCurrentThread() {
  auto* f = new Fiber();
  std::memset(f->jmpbuf_, 0, sizeof(f->jmpbuf_));
  f->is_thread_fiber_ = true;
  f->started_ = true;  // already running on this OS thread
  tls_current_ = f;
  return f;
}

Fiber* Fiber::Create(size_t stack_size, void (*entry)(void*), void* arg) {
  // Minimum 64 KiB, aligned to 16 bytes for the AArch64 ABI.
  if (stack_size < 65536) stack_size = 65536;

  void* stack = nullptr;
  if (posix_memalign(&stack, 16, stack_size) != 0) stack = nullptr;
  if (!stack) return nullptr;

  auto* f = new Fiber();
  std::memset(f->jmpbuf_, 0, sizeof(f->jmpbuf_));
  f->stack_ = stack;
  f->stack_size_ = stack_size;
  f->entry_ = entry;
  f->arg_ = arg;
  f->started_ = false;
  return f;
}

// Trampoline runs on the fiber's own stack. Must not return.
__attribute__((noinline, noreturn))
/*static*/ void Fiber::Trampoline() {
  Fiber* f = tls_current_;
  f->entry_(f->arg_);
  // Entry is a coroutine body — if it ever returns without SwitchTo'ing
  // somewhere else, we have no valid context to resume. Trap loudly.
  __builtin_trap();
}

void Fiber::SwitchTo(Fiber* target) {
  Fiber* from = tls_current_;
  assert(from && "Fiber::SwitchTo called without ConvertCurrentThread");
  assert(target && "Fiber::SwitchTo target is null");

  tls_current_ = target;

  // setjmp returns 0 on the direct call, non-zero when longjmp'd back.
  if (setjmp(FIBER_JMP_BUF(from)) == 0) {
    if (target->started_) {
      longjmp(FIBER_JMP_BUF(target), 1);
    } else {
      // First switch — pivot SP to the fiber's private stack and tail-call
      // the trampoline. AArch64 stack grows downward, SP must be 16-byte
      // aligned, and we leave 16 bytes of red-zone headroom.
      target->started_ = true;
      uintptr_t sp =
          (reinterpret_cast<uintptr_t>(target->stack_) + target->stack_size_) & ~uintptr_t(15);
      void (*trampoline)() = &Fiber::Trampoline;
#if defined(__aarch64__)
      __asm__ volatile(
          "mov sp, %[newsp]\n\t"  // pivot to the fiber's stack
          "br  %[func]\n\t"       // tail-call trampoline (never returns)
          :
          : [newsp] "r"(sp), [func] "r"(trampoline)
          : "memory");
#else
      // iOS simulator on x86_64 (development only) — fall back to a direct
      // call. This will grow the thread's own stack, but the simulator does
      // not actually run guest code, it's for UI/build smoke tests.
      (void)sp;
      trampoline();
#endif
      __builtin_unreachable();
    }
  }
  // Non-zero return — we were longjmp'd back in. Resume.
}

void Fiber::Destroy() {
  if (is_thread_fiber_) {
    tls_current_ = nullptr;
  } else {
    assert(this != tls_current_ && "Fiber::Destroy called on the running fiber");
    std::free(stack_);
  }
  delete this;
}

}  // namespace rex::thread

#undef FIBER_JMP_BUF

#endif  // REX_PLATFORM_IOS
