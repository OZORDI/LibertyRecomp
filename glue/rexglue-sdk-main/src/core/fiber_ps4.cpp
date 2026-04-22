/**
 * @file        core/fiber_ps4.cpp
 * @brief       PS4 (OpenOrbis) backend for rex::thread::Fiber
 *
 * PS4 ships musl ucontext_t declarations but not their implementations.
 * The companion file ucontext_ps4.c provides getcontext/setcontext/
 * swapcontext/makecontext; this file is the standard ucontext-based
 * Fiber implementation.
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 */

// _XOPEN_SOURCE must precede ALL system includes for ucontext.h.
#if !defined(_XOPEN_SOURCE)
#define _XOPEN_SOURCE 600
#endif

#include <rex/platform.h>
#if REX_PLATFORM_PS4

#include <rex/thread/fiber.h>

#include <cassert>
#include <ucontext.h>

// Defined in ucontext_ps4.c — fxsave/fxrstor wrappers that preserve the
// callee-saved FPU control state (MXCSR + x87 CW) plus x87/XMM registers
// across a fiber swap.  The base swapcontext only saves GPRs.
extern "C" void rex_fpu_save(void* buf16);
extern "C" void rex_fpu_restore(const void* buf16);

namespace rex::thread {

thread_local Fiber* Fiber::tls_current_ = nullptr;

Fiber* Fiber::ConvertCurrentThread() {
  auto* f = new Fiber();
  if (getcontext(&f->context_) == -1) {
    delete f;
    return nullptr;
  }
  f->is_thread_fiber_ = true;
  tls_current_ = f;
  return f;
}

Fiber* Fiber::Create(size_t stack_size, void (*entry)(void*), void* arg) {
  auto* f = new Fiber();
  f->entry_ = entry;
  f->arg_ = arg;
  f->stack_.resize(stack_size);

  if (getcontext(&f->context_) == -1) {
    delete f;
    return nullptr;
  }
  f->context_.uc_stack.ss_sp = f->stack_.data();
  f->context_.uc_stack.ss_size = f->stack_.size();
  f->context_.uc_link = nullptr;
  // Trampoline reads entry_/arg_ from tls_current_ — no pointer splitting needed.
  makecontext(&f->context_, &Fiber::Trampoline, 0);
  return f;
}

/*static*/ void Fiber::Trampoline() {
  Fiber* f = tls_current_;
  f->entry_(f->arg_);
}

void Fiber::SwitchTo(Fiber* target) {
  Fiber* from = tls_current_;
  tls_current_ = target;
  // Save our FPU/SSE state, then swap.  When control eventually returns
  // here (another fiber swaps back to us), restore what we saved so
  // callee-saved MXCSR / x87 CW survive the swap.
  rex_fpu_save(from->fxsave_);
  swapcontext(&from->context_, &target->context_);
  rex_fpu_restore(from->fxsave_);
}

void Fiber::Destroy() {
  if (is_thread_fiber_) {
    tls_current_ = nullptr;
  } else {
    assert(this != tls_current_ && "Destroy called on the currently running fiber");
  }
  delete this;
}

}  // namespace rex::thread

#endif  // REX_PLATFORM_PS4
