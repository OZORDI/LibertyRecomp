/**
 * @file        core/seh_posix.cpp
 * @brief       POSIX platform SEH implementations
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 * @license     BSD 3-Clause License
 */

#include <rex/platform.h>
#include <rex/platform/seh.h>

static_assert(REX_PLATFORM_POSIX, "This file is POSIX-only");

#include <signal.h>
#include <cstdlib>  // std::abort()

#include <rex/platform/exceptions.h>

namespace rex::platform {

static thread_local SehThreadState tls_seh_state;
static thread_local bool tls_seh_active = false;

// Saved previously-installed handlers — when a fault fires outside SEH code
// we must chain to these (e.g. rex::arch::ExceptionHandler's SIGSEGV/SIGBUS
// handler that services guest memory access violations) rather than
// restoring SIG_DFL and terminating the process.
static struct sigaction g_prev_sigsegv{};
static struct sigaction g_prev_sigbus{};
static struct sigaction g_prev_sigfpe{};
static struct sigaction g_prev_sigill{};

static const struct sigaction* previous_handler_for(int sig) {
  switch (sig) {
    case SIGSEGV: return &g_prev_sigsegv;
    case SIGBUS:  return &g_prev_sigbus;
    case SIGFPE:  return &g_prev_sigfpe;
    case SIGILL:  return &g_prev_sigill;
    default:      return nullptr;
  }
}

SehThreadState& seh_thread_state() {
  return tls_seh_state;
}

int seh_filter(uint32_t /*code*/, void* /*ep*/) {
  // Not used on POSIX - signal handlers throw directly
  return 0;
}

/// Signal handler for SIGSEGV/SIGBUS/SIGFPE/SIGILL
static void signal_handler(int sig, siginfo_t* info, void* ucontext) {
  // Only handle if we're in SEH-protected code
  if (!tls_seh_active) {
    // Not in SEH region — forward to whoever was registered before us
    // (typically rex::arch::ExceptionHandler's guest-memory access-violation
    // handler). If nobody was registered, restore SIG_DFL and re-raise so the
    // process still dies on a real fault. Previously this code unconditionally
    // restored SIG_DFL even when a real handler existed, which killed the
    // process on any SIGBUS/SIGSEGV that originated outside a __try block.
    const struct sigaction* prev = previous_handler_for(sig);
    if (prev) {
      if ((prev->sa_flags & SA_SIGINFO) && prev->sa_sigaction) {
        prev->sa_sigaction(sig, info, ucontext);
        return;
      }
      if (prev->sa_handler == SIG_IGN) {
        return;
      }
      if (prev->sa_handler && prev->sa_handler != SIG_DFL) {
        prev->sa_handler(sig);
        return;
      }
    }
    signal(sig, SIG_DFL);
    raise(sig);
    return;
  }

  // Determine exception code based on signal
  SehException::Code code;
  switch (sig) {
    case SIGSEGV:
      code = SehException::ACCESS_VIOLATION;
      break;
    case SIGBUS:
      code = SehException::IN_PAGE_ERROR;
      break;
    case SIGFPE:
      code = SehException::FLOAT_DIVIDE_BY_ZERO;
      break;
    case SIGILL:
      code = SehException::ILLEGAL_INSTRUCTION;
      break;
    default:
      code = SehException::UNKNOWN;
      break;
  }

  // Get fault address
  uintptr_t address = info ? reinterpret_cast<uintptr_t>(info->si_addr) : 0;

  // Store in thread state for potential rethrow
  tls_seh_state.code = static_cast<uint32_t>(code);
  tls_seh_state.info[0] = 0;
  tls_seh_state.info[1] = address;

  // Use libunwind to throw from signal context
  // This works because libunwind can unwind through signal frames
  throw SehException(code, address);
}

[[noreturn]] void seh_rethrow() {
  // Map stored code back to signal and re-raise
  uint32_t code = tls_seh_state.code;
  int sig;
  switch (code) {
    case SehException::ACCESS_VIOLATION:
      sig = SIGSEGV;
      break;
    case SehException::IN_PAGE_ERROR:
      sig = SIGBUS;
      break;
    case SehException::FLOAT_DIVIDE_BY_ZERO:
    case SehException::INTEGER_DIVIDE_BY_ZERO:
      sig = SIGFPE;
      break;
    case SehException::ILLEGAL_INSTRUCTION:
      sig = SIGILL;
      break;
    default:
      abort();
  }
  // Restore default handler and re-raise
  signal(sig, SIG_DFL);
  raise(sig);
  abort();  // Should not reach here
}

void seh_initialize() {
  if (g_seh_initialized.exchange(true)) {
    return;  // Already initialized
  }

  struct sigaction sa;
  sa.sa_sigaction = signal_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO | SA_NODEFER;  // SA_NODEFER allows re-entry for nested exceptions

  // Save prior handlers so signal_handler() can chain to them when a fault
  // fires outside of SEH-protected code.
  sigaction(SIGSEGV, &sa, &g_prev_sigsegv);
  sigaction(SIGBUS, &sa, &g_prev_sigbus);
  sigaction(SIGFPE, &sa, &g_prev_sigfpe);
  sigaction(SIGILL, &sa, &g_prev_sigill);
}

bool& seh_active() {
  return tls_seh_active;
}

}  // namespace rex::platform
