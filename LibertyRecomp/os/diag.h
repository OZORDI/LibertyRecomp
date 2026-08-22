// =============================================================================
// Diagnostic logging gate
// =============================================================================
// Centralised on/off switch for the pre-spdlog diagnostic prints that used to
// fire unconditionally on every boot — `[SHADER-DIAG]`, `[VFS-DIAG]`,
// `[SEMA-SEED]`-success, `[DIAG]`, per-byte `[HeaderDiff]`, etc.
//
// These are silent unless the process was launched with --diagnostics and the
// logging category is selected. Environment variables, config files and CVARs
// cannot override the launch decision.
//
// Use via DIAG_EMIT(...) for ordinary raw `fprintf(stderr, ...)` diagnostics.
// Crash/signal handlers retain a separate emergency path and must not call this
// policy helper or any other potentially locking/allocating logging machinery.

#pragma once

#include <cstdio>

namespace os::diag
{
    // Returns the immutable command-line diagnostics decision.
    bool ShouldEmit() noexcept;
}

#define LIBERTY_DIAG_ENABLED ::os::diag::ShouldEmit()

#define DIAG_EMIT(fmt_str, ...) \
    do { \
        if (LIBERTY_DIAG_ENABLED) { \
            fprintf(stderr, fmt_str, ##__VA_ARGS__); \
            fflush(stderr); \
        } \
    } while (0)
