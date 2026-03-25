/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2022 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

#include <rex/exception_handler.h>

#if REX_PLATFORM_LINUX || REX_PLATFORM_MAC

#include <signal.h>

#include <cstdint>
#include <cstring>

#include <rex/assert.h>
#include <rex/logging.h>
#include <rex/math.h>
#include <rex/platform.h>

#ifdef __APPLE__
// macOS requires _XOPEN_SOURCE for ucontext_t.
// We define it locally so we don't pollute every translation unit.
#  ifndef _XOPEN_SOURCE
#    define _XOPEN_SOURCE 700
#    include <ucontext.h>
#    undef _XOPEN_SOURCE
#  else
#    include <ucontext.h>
#  endif
#else
#include <ucontext.h>
#endif

namespace rex::arch {

bool signal_handlers_installed_ = false;
struct sigaction original_sigill_handler_;
struct sigaction original_sigsegv_handler_;
struct sigaction original_sigbus_handler_;

// This can be as large as needed, but isn't often needed.
// As we will be sometimes firing many exceptions we want to avoid having to
// scan the table too much or invoke many custom handlers.
constexpr size_t kMaxHandlerCount = 8;

// All custom handlers, left-aligned and null terminated.
// Executed in order.
std::pair<ExceptionHandler::Handler, void*> handlers_[kMaxHandlerCount];

static void ExceptionHandlerCallback(int signal_number, siginfo_t* signal_info,
                                     void* signal_context) {
#ifdef __APPLE__
  // On macOS, uc_mcontext is already a pointer (mcontext_t = __darwin_mcontext64*).
  mcontext_t mcontext = reinterpret_cast<ucontext_t*>(signal_context)->uc_mcontext;
#else
  mcontext_t& mcontext = reinterpret_cast<ucontext_t*>(signal_context)->uc_mcontext;
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
#  ifdef __APPLE__
  // macOS ARM64: mcontext is a pointer; register state lives in ss/ns sub-structs.
  // __darwin_arm_thread_state64::__x holds x0–x28 (29 regs); __fp=x29, __lr=x30.
  std::memcpy(thread_context.x, mcontext->__ss.__x, sizeof(mcontext->__ss.__x));
  thread_context.x[29] = reinterpret_cast<uint64_t>(__darwin_arm_thread_state64_get_fp(mcontext->__ss));   // x29 / fp
  thread_context.x[30] = reinterpret_cast<uint64_t>(__darwin_arm_thread_state64_get_lr_fptr(mcontext->__ss)); // x30 / lr
  thread_context.sp     = reinterpret_cast<uint64_t>(__darwin_arm_thread_state64_get_sp(mcontext->__ss));
  thread_context.pc     = reinterpret_cast<uint64_t>(__darwin_arm_thread_state64_get_pc_fptr(mcontext->__ss));
  thread_context.pstate = mcontext->__ss.__cpsr;
  // NEON/FP state is directly accessible — no extension scanning needed.
  thread_context.fpsr = mcontext->__ns.__fpsr;
  thread_context.fpcr = mcontext->__ns.__fpcr;
  std::memcpy(thread_context.v, mcontext->__ns.__v, sizeof(thread_context.v));
  // ESR is not directly exposed in macOS ucontext; mark unavailable.
  void* mcontext_esr = nullptr;  // always null on macOS
#  else
  // Linux ARM64: mcontext is a value; registers in flat arrays + extensions.
  std::memcpy(thread_context.x, mcontext.regs, sizeof(thread_context.x));
  thread_context.sp     = mcontext.sp;
  thread_context.pc     = mcontext.pc;
  thread_context.pstate = mcontext.pstate;
  struct fpsimd_context* mcontext_fpsimd = nullptr;
  struct esr_context* mcontext_esr = nullptr;
  for (struct _aarch64_ctx* mcontext_extension =
           reinterpret_cast<struct _aarch64_ctx*>(mcontext.__reserved);
       mcontext_extension->magic;
       mcontext_extension = reinterpret_cast<struct _aarch64_ctx*>(
           reinterpret_cast<uint8_t*>(mcontext_extension) + mcontext_extension->size)) {
    switch (mcontext_extension->magic) {
      case FPSIMD_MAGIC:
        mcontext_fpsimd = reinterpret_cast<struct fpsimd_context*>(mcontext_extension);
        break;
      case ESR_MAGIC:
        mcontext_esr = reinterpret_cast<struct esr_context*>(mcontext_extension);
        break;
      default:
        break;
    }
  }
  assert_not_null(mcontext_fpsimd);
  if (mcontext_fpsimd) {
    thread_context.fpsr = mcontext_fpsimd->fpsr;
    thread_context.fpcr = mcontext_fpsimd->fpcr;
    std::memcpy(thread_context.v, mcontext_fpsimd->vregs, sizeof(thread_context.v));
  }
#  endif  // __APPLE__
#endif  // REX_ARCH

  Exception ex;
  switch (signal_number) {
    case SIGILL:
      ex.InitializeIllegalInstruction(&thread_context);
      break;
    case SIGBUS:
      // macOS delivers SIGBUS (not SIGSEGV) for writes to PROT_NONE shared
      // memory regions — including guest stack guard pages mapped via mmap.
#if defined(__APPLE__) && REX_ARCH_ARM64
    {
      // On macOS ARM64, SIGBUS is exclusively delivered for writes to
      // PROT_NONE pages (guard pages, unmapped shared memory). ESR is
      // unavailable, and IsArm64LoadPrefetchStore may fail for certain
      // compiler-generated store variants, silently setting kUnknown in
      // release builds (NDEBUG makes assert_always a no-op). Hardcode
      // kWrite to ensure the MMIOHandler chain processes these faults.
      Exception::AccessViolationOperation access_violation_operation =
          Exception::AccessViolationOperation::kWrite;
      ex.InitializeAccessViolation(&thread_context,
                                   reinterpret_cast<uint64_t>(signal_info->si_addr),
                                   access_violation_operation);
    } break;
#else
      // Non-Apple or non-ARM64: fall through to SIGSEGV instruction decoding.
#endif
    case SIGSEGV: {
      Exception::AccessViolationOperation access_violation_operation;
#if REX_ARCH_AMD64
      // x86_pf_error_code::X86_PF_WRITE
      constexpr uint64_t kX86PageFaultErrorCodeWrite = UINT64_C(1) << 1;
      access_violation_operation = (uint64_t(mcontext.gregs[REG_ERR]) & kX86PageFaultErrorCodeWrite)
                                       ? Exception::AccessViolationOperation::kWrite
                                       : Exception::AccessViolationOperation::kRead;
#elif REX_ARCH_ARM64
      // For a Data Abort (EC - ESR_EL1 bits 31:26 - 0b100100 from a lower
      // Exception Level, 0b100101 without a change in the Exception Level),
      // bit 6 is 0 for reading from a memory location, 1 for writing to a
      // memory location.
#  ifndef __APPLE__  // esr_context is Linux-only; on macOS mcontext_esr is always nullptr
      if (mcontext_esr && ((mcontext_esr->esr >> 26) & 0b111110) == 0b100100) {
        access_violation_operation = (mcontext_esr->esr & (UINT64_C(1) << 6))
                                         ? Exception::AccessViolationOperation::kWrite
                                         : Exception::AccessViolationOperation::kRead;
      } else {
#  else
      {
#  endif
        // Determine the memory access direction based on which instruction has
        // requested it.
        // esr_context may be unavailable on certain hosts (for instance, on
        // Android, it was added only in NDK r16 - which is the first NDK
        // version to support the Android API level 27, while NDK r15 doesn't
        // have esr_context in its API 26 sigcontext.h).
        // On AArch64 (unlike on AArch32), the program counter is the address of
        // the currently executing instruction.
        bool instruction_is_store;
#  ifdef __APPLE__
        const uint64_t _fault_pc = reinterpret_cast<uint64_t>(__darwin_arm_thread_state64_get_pc(mcontext->__ss));
#  else
        const uint64_t _fault_pc = mcontext.pc;
#  endif
        if (IsArm64LoadPrefetchStore(*reinterpret_cast<const uint32_t*>(_fault_pc),
                                     instruction_is_store)) {
          access_violation_operation = instruction_is_store
                                           ? Exception::AccessViolationOperation::kWrite
                                           : Exception::AccessViolationOperation::kRead;
        } else {
          assert_always(
              "No ESR in the exception thread context, or it's not a Data "
              "Abort, and the faulting instruction is not a known load, "
              "prefetch or store instruction");
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
      uint32_t modified_register_index;
#  ifdef __APPLE__
      uint32_t modified_x_registers_remaining = ex.modified_x_registers();
      while (rex::bit_scan_forward(modified_x_registers_remaining, &modified_register_index)) {
        modified_x_registers_remaining &= ~(UINT32_C(1) << modified_register_index);
        if (modified_register_index < 29) {
          mcontext->__ss.__x[modified_register_index] = thread_context.x[modified_register_index];
        }
      }
      __darwin_arm_thread_state64_set_sp(mcontext->__ss, reinterpret_cast<void*>(thread_context.sp));
      __darwin_arm_thread_state64_set_pc_fptr(mcontext->__ss, reinterpret_cast<void*>(thread_context.pc));
      mcontext->__ss.__cpsr = thread_context.pstate;
      mcontext->__ns.__fpsr = thread_context.fpsr;
      mcontext->__ns.__fpcr = thread_context.fpcr;
      {
        uint32_t modified_v_registers_remaining = ex.modified_v_registers();
        while (rex::bit_scan_forward(modified_v_registers_remaining, &modified_register_index)) {
          modified_v_registers_remaining &= ~(UINT32_C(1) << modified_register_index);
          std::memcpy(&mcontext->__ns.__v[modified_register_index],
                      &thread_context.v[modified_register_index], sizeof(vec128_t));
        }
      }
#  else
      uint32_t modified_x_registers_remaining = ex.modified_x_registers();
      while (rex::bit_scan_forward(modified_x_registers_remaining, &modified_register_index)) {
        modified_x_registers_remaining &= ~(UINT32_C(1) << modified_register_index);
        mcontext.regs[modified_register_index] = thread_context.x[modified_register_index];
      }
      mcontext.sp     = thread_context.sp;
      mcontext.pc     = thread_context.pc;
      mcontext.pstate = thread_context.pstate;
      if (mcontext_fpsimd) {
        mcontext_fpsimd->fpsr = thread_context.fpsr;
        mcontext_fpsimd->fpcr = thread_context.fpcr;
        uint32_t modified_v_registers_remaining = ex.modified_v_registers();
        while (rex::bit_scan_forward(modified_v_registers_remaining, &modified_register_index)) {
          modified_v_registers_remaining &= ~(UINT32_C(1) << modified_register_index);
          std::memcpy(&mcontext_fpsimd->vregs[modified_register_index],
                      &thread_context.v[modified_register_index], sizeof(vec128_t));
          mcontext.regs[modified_register_index] = thread_context.x[modified_register_index];
        }
      }
#  endif  // __APPLE__
#endif  // REX_ARCH
      return;
    }
  }
}

void ExceptionHandler::Install(Handler fn, void* data) {
  // Always (re)install signal handlers to ensure ExceptionHandlerCallback
  // is the active handler. seh_initialize() runs before MMIOHandler::Install()
  // and overwrites signal handlers with its own SEH handler. Re-installing
  // here guarantees ExceptionHandlerCallback takes priority over SEH.
  {
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
    if (sigaction(SIGBUS, &signal_handler, &original_sigbus_handler_) != 0) {
      assert_always("Failed to install new SIGBUS handler");
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
      if (sigaction(SIGBUS, &original_sigbus_handler_, NULL) != 0) {
        assert_always("Failed to restore original SIGBUS handler");
      }
      signal_handlers_installed_ = false;
    }
  }
}

}  // namespace rex::arch

#endif  // REX_PLATFORM_LINUX || REX_PLATFORM_MAC
