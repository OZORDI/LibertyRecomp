# Signal Handler Fallthrough Bug

## The Bug

In `glue/rexglue-sdk-main/src/core/exception_handler_posix.cpp`, the
`ExceptionHandlerCallback` function (line 56) has a silent fallthrough bug.
The handler loop at lines 224-295 iterates all registered handlers. If a
handler returns `true`, the function writes back the modified register state
and returns (line 293). But if **no handler returns true**, execution falls
off the end of `ExceptionHandlerCallback` at line 296 — a bare `}` with no
fallback action.

```
224:  for (size_t i = 0; i < rex::countof(handlers_) && handlers_[i].first; ++i) {
225:    if (handlers_[i].first(&ex, handlers_[i].second)) {
          // ... write back registers ...
293:      return;
294:    }
295:  }
296:}   // <-- BUG: falls off here when no handler claims the fault
```

## Why This Is an Infinite Loop on POSIX

On Windows, `EXCEPTION_CONTINUE_SEARCH` tells the OS to walk the SEH chain
further. There is always a final "unhandled exception filter" that terminates
the process. The Windows version of this code correctly returns
`EXCEPTION_CONTINUE_SEARCH` (line 101 of `exception_handler_win.cpp`).

On POSIX, the signal delivery model is fundamentally different:

1. The kernel delivers SIGSEGV/SIGBUS/SIGILL to the signal handler.
2. When the handler returns, the kernel **restores the register state from
   the ucontext** and resumes execution at the faulting instruction.
3. Since no handler fixed the fault (no register writeback happened), the
   same instruction faults again.
4. The kernel delivers the signal again. Goto step 1.

This is an **infinite loop** — the process spins at 100% CPU re-delivering
the same signal forever without making progress. There is no crash, no core
dump, no log output beyond what the fallback handler already printed. The
process appears hung.

### macOS ARM64 Specifics

On macOS ARM64 (the target platform for LibertyRecomp):

- `SIGBUS` is delivered for writes to `PROT_NONE` pages (guard pages, unmapped
  shared memory). This is different from Linux, which delivers `SIGSEGV`.
- The kernel restores the full `__darwin_arm_thread_state64` from the signal
  frame's `ucontext_t` when the handler returns. The `pc` register points to
  the faulting instruction, so the same store/load re-executes.
- `SA_RESTART` is not relevant here — that flag affects interrupted syscalls,
  not synchronous faults. The instruction restart is unconditional.
- The signal handler was installed **without** `SA_RESETHAND`, so the handler
  is NOT automatically reset to `SIG_DFL` after first invocation. The custom
  handler stays installed and keeps being called.

## Xenia's Identical Bug

The upstream Xenia code (`tools/xenia-master-1/src/xenia/base/exception_handler_posix.cc`)
has the exact same bug. Line 225 is the end of `ExceptionHandlerCallback` with
no fallback after the handler loop (lines 163-224). Xenia was originally a
Windows-only project, and the POSIX handler was adapted without accounting for
the difference in signal delivery semantics. Xenia typically runs on Linux
x86-64 where this bug would manifest identically (SIGSEGV redelivery loop).

## The SEH Handler Interaction

There is a second signal handler layer: `seh_posix.cpp` installs its own
`signal_handler` for SIGSEGV/SIGBUS/SIGFPE/SIGILL with `SA_NODEFER`. This
handler **does** have correct fallthrough behavior:

```cpp
if (!tls_seh_active) {
    signal(sig, SIG_DFL);   // reset to default
    raise(sig);             // re-raise -> core dump
    return;
}
```

However, `ExceptionHandler::Install()` (line 298) reinstalls
`ExceptionHandlerCallback` over the SEH handler every time a new handler is
registered. The comment in the code explicitly acknowledges this:
"seh_initialize() runs before MMIOHandler::Install() and overwrites signal
handlers with its own SEH handler. Re-installing here guarantees
ExceptionHandlerCallback takes priority over SEH."

So in practice, `ExceptionHandlerCallback` is always the active handler, and
the SEH handler's correct fallthrough logic is unreachable for the signals
both handlers register for.

## The Fallback Handler (RexFallbackCrashHandler)

LibertyRecomp installs a `RexFallbackCrashHandler` in `main.cpp` (line 90)
as the last handler in the chain. This handler:

1. Logs detailed crash diagnostics (fault address, PC, symbol info, ARM64
   registers, PPC guest registers).
2. **Returns `false`** (line 176).

The comment on line 174 says: "Return false: we didn't fix anything. The
RexGlue exception system will restore the original signal handler and
re-raise for a core dump."

**This comment is wrong.** The RexGlue exception system does NOT restore the
original signal handler and re-raise. It just falls off the end of the
function. So the fallback handler's diagnostics are printed, and then the
infinite loop begins — the same fault is delivered again, the fallback handler
prints the same diagnostics again, forever.

## The Original Signal Handlers

`ExceptionHandlerCallback` saves the original handlers at install time:

- `original_sigill_handler_`
- `original_sigsegv_handler_`
- `original_sigbus_handler_`

These are only used in `Uninstall()` (line 332) when the last handler is
removed. They are never consulted in the fallthrough path.

## Fix Options

### Option A: Restore original handler and re-raise (recommended)

After the handler loop, forward to the original handler. This is the correct
POSIX pattern — it preserves the pre-existing handler chain (e.g., crash
reporters, debugger trap handlers).

```cpp
  // No handler claimed the fault. Forward to original handler.
  struct sigaction* original = nullptr;
  switch (signal_number) {
    case SIGILL:  original = &original_sigill_handler_; break;
    case SIGSEGV: original = &original_sigsegv_handler_; break;
    case SIGBUS:  original = &original_sigbus_handler_; break;
  }
  if (original) {
    if (original->sa_flags & SA_SIGINFO) {
      original->sa_sigaction(signal_number, signal_info, signal_context);
    } else if (original->sa_handler == SIG_DFL) {
      signal(signal_number, SIG_DFL);
      raise(signal_number);
    } else if (original->sa_handler != SIG_IGN) {
      original->sa_handler(signal_number);
    } else {
      // Original was SIG_IGN — still need to terminate, otherwise infinite loop
      _exit(128 + signal_number);
    }
  }
```

### Option B: abort()

Simplest fix. Produces a core dump on all POSIX systems.

```cpp
  abort();
```

Downside: does not invoke any previously-installed crash reporter or handler.

### Option C: Reset to SIG_DFL and re-raise

```cpp
  signal(signal_number, SIG_DFL);
  raise(signal_number);
```

This is what `seh_posix.cpp` does. It produces a core dump via the default
handler. Downside: discards any original handler that was saved.

### Option D: _exit(128 + signal_number)

No core dump, but clean termination. Only appropriate when core dumps are
unwanted (e.g., release builds).

## Recommendation

**Option A** is the most correct. It matches what the Windows code does
conceptually (returning `EXCEPTION_CONTINUE_SEARCH` lets the OS walk to the
next handler), and it preserves the original handler chain. It also makes the
`RexFallbackCrashHandler` comment in `main.cpp` line 174 actually true.

As a minimum viable fix, **Option C** (reset + re-raise) is the simplest
change that breaks the infinite loop and produces a core dump for diagnosis.

## Files

- Bug location: `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/src/core/exception_handler_posix.cpp` line 296
- Windows counterpart (correct): `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/src/core/exception_handler_win.cpp` line 101
- Upstream Xenia (same bug): `/Users/Ozordi/Downloads/LibertyRecomp/tools/xenia-master-1/src/xenia/base/exception_handler_posix.cc` line 225
- SEH handler (correct fallthrough): `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/src/core/seh_posix.cpp` lines 35-39
- Fallback crash handler: `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/main.cpp` lines 90-177
- Misleading comment: `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/main.cpp` lines 174-175
