/**
 * @file    rex_thread_state.h
 * @brief   Bridge between RexGlue's ThreadState and LibertyRecomp's
 * GuestThreadContext
 *
 * This header provides integration points for RexGlue's ThreadState API while
 * preserving LibertyRecomp's existing Xbox 360 memory layout (PCR/TLS/TEB).
 *
 * Key design decisions:
 * - Keep LibertyRecomp's memory layout (GTA IV expects specific offsets)
 * - Adopt RexGlue's thread-local binding pattern (ThreadState::Bind/Get)
 * - Enable future migration to RexGlue's full threading model
 */

#pragma once

#include "../cpu/guest_thread.h"
#include "../cpu/ppc_context.h"

#include <memory>
#include <rex/runtime/thread_state.h>
#include <rex/runtime.h>

namespace rex {
namespace adapter {

/**
 * @brief Thread state wrapper that bridges RexGlue and LibertyRecomp
 *
 * This class provides a compatible interface with RexGlue's ThreadState
 * while internally using LibertyRecomp's GuestThreadContext for memory
 * management.
 */
class AdapterThreadState {
public:
  AdapterThreadState(GuestThreadContext &ctx)
      : ctx_(ctx), thread_id_(GuestThread::GetCurrentThreadId()) {
    // Bind this thread state as the current thread's state
    Bind(this);
    // Create and bind a RexGlue ThreadState so that kernel code
    // (e.g. XThread::GetCurrentThread() fallback, kernel exports that
    // call ThreadState::Get()) can find a valid context on this thread.
    // This is needed for the main thread and host audio threads which use
    // GuestThreadContext directly instead of XThread.
    rex_thread_state_ = std::make_unique<rex::runtime::ThreadState>(
        thread_id_,
        static_cast<uint32_t>(ctx_.ppcContext.r1.u64),   // stack_base
        static_cast<uint32_t>(ctx_.ppcContext.r13.u64),   // pcr_address
        rex::Runtime::instance() ? rex::Runtime::instance()->memory() : nullptr);
    rex::runtime::ThreadState::Bind(rex_thread_state_.get());
  }

  ~AdapterThreadState() {
    // Unbind on destruction
    if (Get() == this) {
      Bind(nullptr);
    }
    if (rex_thread_state_) {
      if (rex::runtime::ThreadState::Get() == rex_thread_state_.get()) {
        rex::runtime::ThreadState::Bind(nullptr);
      }
      rex_thread_state_.reset();
    }
  }

  // Non-copyable
  AdapterThreadState(const AdapterThreadState &) = delete;
  AdapterThreadState &operator=(const AdapterThreadState &) = delete;

  // -------------------------------------------------------------------------
  // RexGlue-compatible API
  // -------------------------------------------------------------------------

  /// Get the PPC context
  PPCContext *context() { return &ctx_.ppcContext; }
  const PPCContext *context() const { return &ctx_.ppcContext; }

  /// Get the thread ID
  uint32_t thread_id() const { return thread_id_; }

  /// Get the PCR (Processor Control Region) address
  uint32_t pcr_address() const {
    return static_cast<uint32_t>(ctx_.ppcContext.r13.u64);
  }

  /// Get the stack base address
  uint32_t stack_base() const {
    return static_cast<uint32_t>(ctx_.ppcContext.r1.u64);
  }

  // -------------------------------------------------------------------------
  // Static thread-local binding (mirroring RexGlue's pattern)
  // -------------------------------------------------------------------------

  static void Bind(AdapterThreadState *state) { tls_current_ = state; }

  static AdapterThreadState *Get() { return tls_current_; }

  static uint32_t GetThreadID() {
    auto *state = Get();
    return state ? state->thread_id() : 0;
  }

  // -------------------------------------------------------------------------
  // LibertyRecomp-specific accessors
  // -------------------------------------------------------------------------

  /// Get the underlying GuestThreadContext
  GuestThreadContext &guest_context() { return ctx_; }
  const GuestThreadContext &guest_context() const { return ctx_; }

  /// Get typed pointer to PCR
  X360_PCR *pcr() { return reinterpret_cast<X360_PCR *>(ctx_.thread); }

  /// Get typed pointer to TLS
  X360_TLS *tls() {
    return reinterpret_cast<X360_TLS *>(ctx_.thread + X360_TLS_OFFSET);
  }

  /// Get typed pointer to TEB
  X360_TEB *teb() {
    return reinterpret_cast<X360_TEB *>(ctx_.thread + X360_TEB_OFFSET);
  }

private:
  GuestThreadContext &ctx_;
  uint32_t thread_id_;
  std::unique_ptr<rex::runtime::ThreadState> rex_thread_state_;

  static thread_local AdapterThreadState *tls_current_;
};

// Thread-local storage definition
inline thread_local AdapterThreadState *AdapterThreadState::tls_current_ =
    nullptr;

} // namespace adapter
} // namespace rex

// =============================================================================
// Convenience macros for thread state access
// =============================================================================

/// Get the current thread's adapter state
#define REX_THREAD_STATE() rex::adapter::AdapterThreadState::Get()

/// Get the current thread's PPC context via adapter
#define REX_CONTEXT()                                                          \
  (REX_THREAD_STATE() ? REX_THREAD_STATE()->context() : nullptr)
