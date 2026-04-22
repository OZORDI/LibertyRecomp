/**
 ******************************************************************************
 * ReXGlue runtime - Nintendo Switch (libnx) exception handler.
 ******************************************************************************
 *
 * Switch does not deliver POSIX signals for CPU faults: sigaction(SIGSEGV) in
 * libnx's newlib port is a stub that returns -1, so the signal-based
 * exception_handler_posix.cpp install path is a silent no-op on NX. That means
 * mmio_handler.cpp's fault handler would never fire on writes to the guest
 * MMIO range.
 *
 * Instead, libnx routes unhandled user exceptions through a weak symbol,
 * __libnx_exception_handler(ThreadExceptionDump*), that the application may
 * override. We translate the libnx ThreadExceptionDump into a rex::arch
 * Exception and dispatch it through the same handler list used on other
 * platforms (MMIOHandler::ExceptionCallback etc.).
 */

#include <rex/platform.h>

#if REX_PLATFORM_NX

#include <rex/exception_handler.h>

#include <unistd.h>  // write(), STDERR_FILENO — used from the fault-handler path.

#include <cstdint>
#include <cstring>

#include <rex/assert.h>
#include <rex/logging.h>
#include <rex/math.h>
#include <rex/system/mmio_handler.h>

extern "C" {
#include <switch/arm/thread_context.h>
#include <switch/kernel/svc.h>
#include <switch/types.h>
}

namespace rex::arch {

namespace {

constexpr size_t kMaxHandlerCount = 8;
std::pair<ExceptionHandler::Handler, void*> g_handlers[kMaxHandlerCount]{};

inline bool IsDataAbortWrite(uint32_t esr) {
  // EC (bits 31:26): 0b100100 (lower EL data abort) / 0b100101 (same EL).
  const uint32_t ec = (esr >> 26) & 0x3Fu;
  if ((ec & 0x3Eu) != 0x24u) {
    return false;
  }
  // ISS bit 6 = WnR (write-not-read).
  return (esr & (1u << 6)) != 0u;
}

inline bool IsAccessViolation(uint32_t error_desc) {
  // Treat data / instruction aborts and misaligned accesses as AV.
  switch (error_desc) {
    case ThreadExceptionDesc_InstructionAbort:
    case ThreadExceptionDesc_MisalignedPC:
    case ThreadExceptionDesc_MisalignedSP:
    case ThreadExceptionDesc_Other:  // EC <= 0x34, includes data abort
      return true;
    default:
      return false;
  }
}

void FillThreadContext(HostThreadContext& tc, const ThreadExceptionDump* ctx) {
#if REX_ARCH_ARM64
  for (uint32_t i = 0; i < 29; ++i) {
    tc.x[i] = ctx->cpu_gprs[i].x;
  }
  tc.x[29] = ctx->fp.x;
  tc.x[30] = ctx->lr.x;
  tc.sp    = ctx->sp.x;
  tc.pc    = ctx->pc.x;
  tc.pstate = ctx->pstate;
  tc.fpsr = ctx->fpsr;
  tc.fpcr = ctx->fpcr;
  for (uint32_t i = 0; i < 32; ++i) {
    std::memcpy(&tc.v[i], &ctx->fpu_gprs[i].v, sizeof(tc.v[i]));
  }
#else
  (void)tc;
  (void)ctx;
#endif
}

[[noreturn]] void PanicBreak() {
  svcBreak(BreakReason_Panic, 0, 0);
  for (;;) {
  }
}

// --------------------------------------------------------------------------
// Async-signal-safe / fault-handler-safe helpers.
//
// __libnx_exception_handler runs in the same restricted context as a POSIX
// signal handler: spdlog / fmt::format / std::string / malloc are all
// unsafe (spdlog may be mid-write on the faulting thread's mutex). Route
// diagnostic output through write(2) with stack buffers only before the
// final svcBreak.
// --------------------------------------------------------------------------

inline void SigSafeWriteCStr(const char* s) {
  if (!s) return;
  size_t n = 0;
  while (s[n]) ++n;
  (void)::write(STDERR_FILENO, s, n);
}

inline size_t SigSafeFormatHex(char* buf, size_t cap, uint64_t value) {
  static const char kHex[] = "0123456789abcdef";
  if (cap < 4) return 0;
  size_t w = 0;
  buf[w++] = '0';
  buf[w++] = 'x';
  int shift = 60;
  while (shift > 0 && ((value >> shift) & 0xF) == 0) shift -= 4;
  while (shift >= 0 && w < cap) {
    buf[w++] = kHex[(value >> shift) & 0xF];
    shift -= 4;
  }
  return w;
}

inline void SigSafeWriteLabelHex(const char* label, uint64_t value) {
  char buf[96];
  size_t w = 0;
  for (const char* p = label; *p && w < sizeof(buf) - 1; ++p) buf[w++] = *p;
  if (w < sizeof(buf) - 1) buf[w++] = '=';
  w += SigSafeFormatHex(buf + w, sizeof(buf) - w - 1, value);
  if (w < sizeof(buf)) buf[w++] = '\n';
  (void)::write(STDERR_FILENO, buf, w);
}

}  // namespace

void ExceptionHandler::Install(Handler fn, void* data) {
  for (size_t i = 0; i < kMaxHandlerCount; ++i) {
    if (!g_handlers[i].first) {
      g_handlers[i].first = fn;
      g_handlers[i].second = data;
      return;
    }
  }
  assert_always("Too many exception handlers installed");
}

void ExceptionHandler::Uninstall(Handler fn, void* data) {
  for (size_t i = 0; i < kMaxHandlerCount; ++i) {
    if (g_handlers[i].first == fn && g_handlers[i].second == data) {
      for (; i < kMaxHandlerCount - 1; ++i) {
        g_handlers[i] = g_handlers[i + 1];
      }
      g_handlers[kMaxHandlerCount - 1] = {nullptr, nullptr};
      return;
    }
  }
}

}  // namespace rex::arch

extern "C" void __libnx_exception_handler(ThreadExceptionDump* ctx) {
  using namespace rex::arch;

  const uint64_t fault_pc   = ctx->pc.x;
  const uint64_t fault_addr = ctx->far.x;
  const uint32_t esr        = ctx->esr;
  const bool is_write       = IsDataAbortWrite(esr);

  if (!IsAccessViolation(ctx->error_desc)) {
    // Signal-safe only: spdlog / fmt::format are not async-safe.
    SigSafeWriteCStr("[rex] Switch exception (non-AV)\n");
    SigSafeWriteLabelHex("error_desc", static_cast<uint64_t>(ctx->error_desc));
    SigSafeWriteLabelHex("pc", fault_pc);
    SigSafeWriteLabelHex("far", fault_addr);
    SigSafeWriteLabelHex("esr", static_cast<uint64_t>(esr));
    PanicBreak();
  }

  HostThreadContext thread_context{};
  FillThreadContext(thread_context, ctx);

  Exception ex;
  ex.InitializeAccessViolation(
      &thread_context, fault_addr,
      is_write ? Exception::AccessViolationOperation::kWrite
               : Exception::AccessViolationOperation::kRead);

  for (size_t i = 0; i < kMaxHandlerCount && g_handlers[i].first; ++i) {
    if (g_handlers[i].first(&ex, g_handlers[i].second)) {
#if REX_ARCH_ARM64
      // Write back modified x registers.
      uint32_t modified = ex.modified_x_registers();
      uint32_t idx;
      while (rex::bit_scan_forward(modified, &idx)) {
        modified &= ~(UINT32_C(1) << idx);
        if (idx < 29) {
          ctx->cpu_gprs[idx].x = thread_context.x[idx];
        } else if (idx == 29) {
          ctx->fp.x = thread_context.x[29];
        } else if (idx == 30) {
          ctx->lr.x = thread_context.x[30];
        }
      }
      uint32_t modified_v = ex.modified_v_registers();
      while (rex::bit_scan_forward(modified_v, &idx)) {
        modified_v &= ~(UINT32_C(1) << idx);
        std::memcpy(&ctx->fpu_gprs[idx].v, &thread_context.v[idx], sizeof(ctx->fpu_gprs[idx].v));
      }
      ctx->sp.x = thread_context.sp;
      ctx->pc.x = thread_context.pc;  // resume PC (handler advanced past insn).
#endif
      // Returning from __libnx_exception_handler with a modified ctx causes
      // libnx to svcReturnFromException and resume at ctx->pc.
      return;
    }
  }

  // Signal-safe only: spdlog / fmt::format are not async-safe.
  SigSafeWriteCStr("[rex] Unhandled Switch access violation\n");
  SigSafeWriteLabelHex("pc", fault_pc);
  SigSafeWriteLabelHex("far", fault_addr);
  SigSafeWriteLabelHex("esr", static_cast<uint64_t>(esr));
  SigSafeWriteLabelHex("is_write", is_write ? 1ULL : 0ULL);
  PanicBreak();
}

#endif  // REX_PLATFORM_NX
