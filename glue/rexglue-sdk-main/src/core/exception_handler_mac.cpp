/**
 * @file        core/exception_handler_mac.cpp
 * @brief       macOS exception handling implementation derived from the POSIX backend
 *
 * @license     BSD 3-Clause License
 */

#include <rex/exception_handler.h>

#if REX_PLATFORM_MAC

#include <signal.h>

#include <cstdint>
#include <cstring>

#include <rex/assert.h>
#include <rex/logging.h>
#include <rex/math.h>
#include <rex/platform.h>

// Darwin hides parts of ucontext behind X/Open feature gating.
#if !defined(_XOPEN_SOURCE)
#define _XOPEN_SOURCE 700
#endif
#include <ucontext.h>

namespace rex::arch {

bool signal_handlers_installed_ = false;
struct sigaction original_sigill_handler_;
struct sigaction original_sigsegv_handler_;

// This can be as large as needed, but isn't often needed.
// As we will be sometimes firing many exceptions we want to avoid having to
// scan the table too much or invoke many custom handlers.
constexpr size_t kMaxHandlerCount = 8;

// All custom handlers, left-aligned and null terminated.
// Executed in order.
std::pair<ExceptionHandler::Handler, void*> handlers_[kMaxHandlerCount];

static void ExceptionHandlerCallback(int signal_number, siginfo_t* signal_info,
                                     void* signal_context) {
// On macOS arm64, uc_mcontext points at a Mach-flavored structure split into
// general-purpose, exception, and NEON register banks.
#if REX_ARCH_ARM64
  auto* mcontext = reinterpret_cast<ucontext_t*>(signal_context)->uc_mcontext;
  auto& thread_state = mcontext->__ss;
  auto& exception_state = mcontext->__es;
  auto& neon_state = mcontext->__ns;
#endif

  HostThreadContext thread_context;

#if REX_ARCH_AMD64
  thread_context.rip = uint64_t(mcontext.gregs[REG_RIP]);
  thread_context.eflags = uint32_t(mcontext.gregs[REG_EFL]);
  // The REG_ order may be different than the register indices in the
  // instruction encoding.
  thread_context.rax = uint64_t(mcontext.gregs[REG_RAX]);
  thread_context.rcx = uint64_t(mcontext.gregs[REG_RCX]);
  thread_context.rdx = uint64_t(mcontext.gregs[REG_RDX]);
  thread_context.rbx = uint64_t(mcontext.gregs[REG_RBX]);
  thread_context.rsp = uint64_t(mcontext.gregs[REG_RSP]);
  thread_context.rbp = uint64_t(mcontext.gregs[REG_RBP]);
  thread_context.rsi = uint64_t(mcontext.gregs[REG_RSI]);
  thread_context.rdi = uint64_t(mcontext.gregs[REG_RDI]);
  thread_context.r8 = uint64_t(mcontext.gregs[REG_R8]);
  thread_context.r9 = uint64_t(mcontext.gregs[REG_R9]);
  thread_context.r10 = uint64_t(mcontext.gregs[REG_R10]);
  thread_context.r11 = uint64_t(mcontext.gregs[REG_R11]);
  thread_context.r12 = uint64_t(mcontext.gregs[REG_R12]);
  thread_context.r13 = uint64_t(mcontext.gregs[REG_R13]);
  thread_context.r14 = uint64_t(mcontext.gregs[REG_R14]);
  thread_context.r15 = uint64_t(mcontext.gregs[REG_R15]);
  std::memcpy(thread_context.xmm_registers, mcontext.fpregs->_xmm,
              sizeof(thread_context.xmm_registers));
#elif REX_ARCH_ARM64
  // Darwin stores arm64 GPR/FPU state in the split __ss / __ns records rather
  // than the Linux sigcontext layout used by the shared POSIX backend.
  std::memcpy(thread_context.x, thread_state.__x, sizeof(thread_state.__x));
  thread_context.x[29] = thread_state.__fp;
  thread_context.x[30] = thread_state.__lr;
  thread_context.sp = thread_state.__sp;
  thread_context.pc = thread_state.__pc;
  thread_context.pstate = thread_state.__cpsr;
  thread_context.fpsr = neon_state.__fpsr;
  thread_context.fpcr = neon_state.__fpcr;
  std::memcpy(thread_context.v, neon_state.__v, sizeof(thread_context.v));
#endif  // REX_ARCH

  Exception ex;
  switch (signal_number) {
    case SIGILL:
      ex.InitializeIllegalInstruction(&thread_context);
      break;
    case SIGSEGV: {
      Exception::AccessViolationOperation access_violation_operation;
#if REX_ARCH_AMD64
      // x86_pf_error_code::X86_PF_WRITE
      constexpr uint64_t kX86PageFaultErrorCodeWrite = UINT64_C(1) << 1;
      access_violation_operation = (uint64_t(mcontext.gregs[REG_ERR]) & kX86PageFaultErrorCodeWrite)
                                       ? Exception::AccessViolationOperation::kWrite
                                       : Exception::AccessViolationOperation::kRead;
#elif REX_ARCH_ARM64
      // Darwin exposes the decoded ESR directly in the exception state.
      constexpr uint64_t kArm64DataAbortMask = 0b111110;
      constexpr uint64_t kArm64DataAbortEc = 0b100100;
      const uint64_t esr = exception_state.__esr;
      if (((esr >> 26) & kArm64DataAbortMask) == kArm64DataAbortEc) {
        access_violation_operation = (esr & (UINT64_C(1) << 6))
                                         ? Exception::AccessViolationOperation::kWrite
                                         : Exception::AccessViolationOperation::kRead;
      } else {
        bool instruction_is_store;
        if (IsArm64LoadPrefetchStore(*reinterpret_cast<const uint32_t*>(thread_state.__pc),
                                     instruction_is_store)) {
          access_violation_operation = instruction_is_store
                                           ? Exception::AccessViolationOperation::kWrite
                                           : Exception::AccessViolationOperation::kRead;
        } else {
          assert_always(
              "No usable ESR in the macOS exception thread context and the "
              "faulting instruction is not a known load, prefetch or store instruction");
          access_violation_operation = Exception::AccessViolationOperation::kUnknown;
        }
      }
#else
      access_violation_operation = Exception::AccessViolationOperation::kUnknown;
#endif  // REX_ARCH
      ex.InitializeAccessViolation(&thread_context,
                                   reinterpret_cast<uint64_t>(signal_info->si_addr),
                                   access_violation_operation);
    } break;
    default:
      assert_unhandled_case(signal_number);
  }

  for (size_t i = 0; i < rex::countof(handlers_) && handlers_[i].first; ++i) {
    if (handlers_[i].first(&ex, handlers_[i].second)) {
      // Exception handled.
#if REX_ARCH_AMD64
      mcontext.gregs[REG_RIP] = greg_t(thread_context.rip);
      mcontext.gregs[REG_EFL] = greg_t(thread_context.eflags);
      uint32_t modified_register_index;
      // The order must match the order in X64Register.
      static const size_t kIntRegisterMap[] = {
          REG_RAX, REG_RCX, REG_RDX, REG_RBX, REG_RSP, REG_RBP, REG_RSI, REG_RDI,
          REG_R8,  REG_R9,  REG_R10, REG_R11, REG_R12, REG_R13, REG_R14, REG_R15,
      };
      uint16_t modified_int_registers_remaining = ex.modified_int_registers();
      while (rex::bit_scan_forward(modified_int_registers_remaining, &modified_register_index)) {
        modified_int_registers_remaining &= ~(UINT16_C(1) << modified_register_index);
        mcontext.gregs[kIntRegisterMap[modified_register_index]] =
            thread_context.int_registers[modified_register_index];
      }
      uint16_t modified_xmm_registers_remaining = ex.modified_xmm_registers();
      while (rex::bit_scan_forward(modified_xmm_registers_remaining, &modified_register_index)) {
        modified_xmm_registers_remaining &= ~(UINT16_C(1) << modified_register_index);
        std::memcpy(&mcontext.fpregs->_xmm[modified_register_index],
                    &thread_context.xmm_registers[modified_register_index], sizeof(vec128_t));
      }
#elif REX_ARCH_ARM64
      // Write any handler modifications back through the Darwin thread-state
      // records instead of the Linux sigcontext fields.
      uint32_t modified_register_index;
      uint32_t modified_x_registers_remaining = ex.modified_x_registers();
      while (rex::bit_scan_forward(modified_x_registers_remaining, &modified_register_index)) {
        modified_x_registers_remaining &= ~(UINT32_C(1) << modified_register_index);
        if (modified_register_index < 29) {
          thread_state.__x[modified_register_index] = thread_context.x[modified_register_index];
        }
      }
      thread_state.__fp = thread_context.x[29];
      thread_state.__lr = thread_context.x[30];
      thread_state.__sp = thread_context.sp;
      thread_state.__pc = thread_context.pc;
      thread_state.__cpsr = static_cast<uint32_t>(thread_context.pstate);
      neon_state.__fpsr = thread_context.fpsr;
      neon_state.__fpcr = thread_context.fpcr;
      uint32_t modified_v_registers_remaining = ex.modified_v_registers();
      while (rex::bit_scan_forward(modified_v_registers_remaining, &modified_register_index)) {
        modified_v_registers_remaining &= ~(UINT32_C(1) << modified_register_index);
        std::memcpy(&neon_state.__v[modified_register_index],
                    &thread_context.v[modified_register_index], sizeof(vec128_t));
      }
#endif  // REX_ARCH
      return;
    }
  }
}

void ExceptionHandler::Install(Handler fn, void* data) {
  if (!signal_handlers_installed_) {
    struct sigaction signal_handler;

    std::memset(&signal_handler, 0, sizeof(signal_handler));
    signal_handler.sa_sigaction = ExceptionHandlerCallback;
    signal_handler.sa_flags = SA_SIGINFO;

    if (sigaction(SIGILL, &signal_handler, &original_sigill_handler_) != 0) {
      assert_always("Failed to install new SIGILL handler");
    }
    if (sigaction(SIGSEGV, &signal_handler, &original_sigsegv_handler_) != 0) {
      assert_always("Failed to install new SIGSEGV handler");
    }
    signal_handlers_installed_ = true;
  }

  for (size_t i = 0; i < rex::countof(handlers_); ++i) {
    if (!handlers_[i].first) {
      handlers_[i].first = fn;
      handlers_[i].second = data;
      return;
    }
  }
  assert_always("Too many exception handlers installed");
}

void ExceptionHandler::Uninstall(Handler fn, void* data) {
  for (size_t i = 0; i < rex::countof(handlers_); ++i) {
    if (handlers_[i].first == fn && handlers_[i].second == data) {
      for (; i < rex::countof(handlers_) - 1; ++i) {
        handlers_[i] = handlers_[i + 1];
      }
      handlers_[i].first = nullptr;
      handlers_[i].second = nullptr;
      break;
    }
  }

  bool has_any = false;
  for (size_t i = 0; i < rex::countof(handlers_); ++i) {
    if (handlers_[i].first) {
      has_any = true;
      break;
    }
  }
  if (!has_any) {
    if (signal_handlers_installed_) {
      if (sigaction(SIGILL, &original_sigill_handler_, NULL) != 0) {
        assert_always("Failed to restore original SIGILL handler");
      }
      if (sigaction(SIGSEGV, &original_sigsegv_handler_, NULL) != 0) {
        assert_always("Failed to restore original SIGSEGV handler");
      }
      signal_handlers_installed_ = false;
    }
  }
}

}  // namespace rex::arch

#endif  // REX_PLATFORM_MAC
