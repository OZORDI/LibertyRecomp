/**
 * @file        core/fiber_switch.cpp
 * @brief       Switch (libnx/newlib) backend for rex::thread::Fiber
 *
 * libnx newlib has no ucontext.h, but setjmp/longjmp are available.
 * We use setjmp to save/restore registers and inline asm to pivot the
 * stack pointer for the initial trampoline call.
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 */

#include <rex/platform.h>
#if REX_PLATFORM_NX

#include <rex/thread/fiber.h>

#include <cassert>
#include <csetjmp>
#include <cstdlib>
#include <cstring>

// Verify our jmpbuf_ storage is large enough for the platform jmp_buf.
static_assert(sizeof(jmp_buf) <= 256, "jmp_buf exceeds Fiber::jmpbuf_ storage");

namespace rex::thread {

thread_local Fiber* Fiber::tls_current_ = nullptr;

// Reinterpret the raw char[] storage as a jmp_buf. Called only from Fiber
// member functions, so private-member access is fine.
#define FIBER_JMP_BUF(f) (*reinterpret_cast<jmp_buf*>((f)->jmpbuf_))

Fiber* Fiber::ConvertCurrentThread() {
  auto* f = new Fiber();
  std::memset(f->jmpbuf_, 0, sizeof(f->jmpbuf_));
  f->is_thread_fiber_ = true;
  f->started_ = true;  // thread fiber is already running
  tls_current_ = f;
  return f;
}

Fiber* Fiber::Create(size_t stack_size, void (*entry)(void*), void* arg) {
  // Minimum 64 KiB, aligned to 16 bytes for AArch64 calling convention.
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

// Standalone trampoline called on the fiber's own stack. Never returns.
// Marked noinline so the compiler cannot fold it into SwitchTo (which would
// break the stack-pivot assumption).
__attribute__((noinline, noreturn))
/*static*/ void Fiber::Trampoline() {
  Fiber* f = tls_current_;
  f->entry_(f->arg_);
  // Entry function must not return — it should SwitchTo another fiber.
  // If it does, trap immediately.
  __builtin_trap();
}

void Fiber::SwitchTo(Fiber* target) {
  Fiber* from = tls_current_;
  assert(from && "SwitchTo called without ConvertCurrentThread");
  assert(target && "SwitchTo target is null");

  tls_current_ = target;

  // Save current context. setjmp returns 0 on direct call, non-zero on longjmp.
  if (setjmp(FIBER_JMP_BUF(from)) == 0) {
    if (target->started_) {
      // Target has been running before — resume it.
      longjmp(FIBER_JMP_BUF(target), 1);
    } else {
      // First switch into this fiber — pivot SP to the fiber's stack, then
      // call the trampoline. The trampoline never returns.
      target->started_ = true;
      // AArch64 stack grows downward. SP must be 16-byte aligned.
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
      (void)sp;
      trampoline();
#endif
      __builtin_unreachable();
    }
  }
  // setjmp returned non-zero — we were longjmp'd back. Resume here.
}

void Fiber::Destroy() {
  if (is_thread_fiber_) {
    tls_current_ = nullptr;
  } else {
    assert(this != tls_current_ && "Destroy called on the currently running fiber");
    std::free(stack_);
  }
  delete this;
}

}  // namespace rex::thread

#undef FIBER_JMP_BUF

#endif  // REX_PLATFORM_NX
