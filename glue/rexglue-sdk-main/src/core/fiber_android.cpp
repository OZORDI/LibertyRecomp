/**
 * @file        core/fiber_android.cpp
 * @brief       Android (Bionic) backend for rex::thread::Fiber
 *
 * Bionic removed ucontext/makecontext/swapcontext. We use the same
 * setjmp/longjmp + aarch64 stack pivot pattern as the Switch backend.
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 */

#include <rex/platform.h>
#if REX_PLATFORM_ANDROID

#include <rex/thread/fiber.h>

#include <cassert>
#include <csetjmp>
#include <cstdlib>
#include <cstring>

static_assert(sizeof(jmp_buf) <= 256, "jmp_buf exceeds Fiber::jmpbuf_ storage");

namespace rex::thread {

thread_local Fiber* Fiber::tls_current_ = nullptr;

#define FIBER_JMP_BUF(f) (*reinterpret_cast<jmp_buf*>((f)->jmpbuf_))

Fiber* Fiber::ConvertCurrentThread() {
  auto* f = new Fiber();
  std::memset(f->jmpbuf_, 0, sizeof(f->jmpbuf_));
  f->is_thread_fiber_ = true;
  f->started_ = true;
  tls_current_ = f;
  return f;
}

Fiber* Fiber::Create(size_t stack_size, void (*entry)(void*), void* arg) {
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

__attribute__((noinline, noreturn))
/*static*/ void Fiber::Trampoline() {
  Fiber* f = tls_current_;
  f->entry_(f->arg_);
  __builtin_trap();
}

void Fiber::SwitchTo(Fiber* target) {
  Fiber* from = tls_current_;
  assert(from && "SwitchTo called without ConvertCurrentThread");
  assert(target && "SwitchTo target is null");

  tls_current_ = target;

  if (setjmp(FIBER_JMP_BUF(from)) == 0) {
    if (target->started_) {
      longjmp(FIBER_JMP_BUF(target), 1);
    } else {
      target->started_ = true;
      uintptr_t sp =
          (reinterpret_cast<uintptr_t>(target->stack_) + target->stack_size_) & ~uintptr_t(15);
      void (*trampoline)() = &Fiber::Trampoline;
#if defined(__aarch64__)
      __asm__ volatile(
          "mov sp, %[newsp]\n\t"
          "br  %[func]\n\t"
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

#endif  // REX_PLATFORM_ANDROID
