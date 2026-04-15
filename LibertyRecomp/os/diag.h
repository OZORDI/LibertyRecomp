// =============================================================================
// Diagnostic logging gate
// =============================================================================
// Centralised on/off switch for the pre-spdlog diagnostic prints that used to
// fire unconditionally on every boot — `[SHADER-DIAG]`, `[VFS-DIAG]`,
// `[SEMA-SEED]`-success, `[DIAG]`, per-byte `[HeaderDiff]`, etc.
//
// By default these are silent in release builds. Set the environment variable
// LIBERTY_VERBOSE_DIAG=1 (or pass -DLIBERTY_RECOMP_VERBOSE_DIAG at CMake time)
// to re-enable them without a rebuild. Error-severity prints in the same code
// paths remain unconditional — this gate only covers the "informational
// trace" branches.
//
// Use via DIAG_EMIT(...) for raw `fprintf(stderr, ...)` content — no added
// formatting, no spdlog dependency, safe to call from signal-unsafe contexts
// that already use fprintf today.

#pragma once

#include <cstdio>

namespace os::diag
{
    // Returns true if diagnostic traces should be emitted. Caches the result
    // on first call so the env lookup only happens once per process.
    bool ShouldEmit() noexcept;
}

#ifdef LIBERTY_RECOMP_VERBOSE_DIAG
    // Compile-time forced ON.
    #define LIBERTY_DIAG_ENABLED 1
#else
    #define LIBERTY_DIAG_ENABLED ::os::diag::ShouldEmit()
#endif

#define DIAG_EMIT(fmt_str, ...) \
    do { \
        if (LIBERTY_DIAG_ENABLED) { \
            fprintf(stderr, fmt_str, ##__VA_ARGS__); \
            fflush(stderr); \
        } \
    } while (0)
