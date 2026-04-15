#pragma once

// LR's logging macros now forward to rexglue's spdlog-based logger.
// Call sites keep working unchanged — each LOG_* / LOGF_* macro maps to the
// appropriate REXLOG_* severity on the core category.
//
//   None     -> INFO    (default/plain info)
//   Utility  -> DEBUG   (dev-only chatter; _DEBUG build only historically)
//   Warning  -> WARN
//   Error    -> ERROR
//
// The `func` parameter that the legacy Log() API carried is now supplied
// automatically by spdlog via source_loc (__FUNCTION__) inside REX_LOG_IMPL,
// so we drop the func argument at the forwarder layer.

#include <string_view>

#include <rex/logging.h>
#include <rex/logging/macros.h>

// --- Internal forwarders --------------------------------------------------
// _DEBUG gating preserved for the Utility level to match legacy behavior:
// Utility messages always compiled in (matches original logger.h which also
// compiled them in both branches), but routed through spdlog's DEBUG channel.

// Function-specific logging (legacy emitted __func__; spdlog captures it).
#define LOG(str)               REXLOG_INFO ("{}", (str))
#define LOG_INFO(str)          REXLOG_INFO ("{}", (str))
#define LOG_DEBUG(str)         REXLOG_DEBUG("{}", (str))
#define LOG_UTILITY(str)       REXLOG_DEBUG("{}", (str))
#define LOG_WARNING(str)       REXLOG_WARN ("{}", (str))
#define LOG_ERROR(str)         REXLOG_ERROR("{}", (str))

#define LOGF(fmt_str, ...)         REXLOG_INFO (fmt_str, ##__VA_ARGS__)
#define LOGF_INFO(fmt_str, ...)    REXLOG_INFO (fmt_str, ##__VA_ARGS__)
#define LOGF_DEBUG(fmt_str, ...)   REXLOG_DEBUG(fmt_str, ##__VA_ARGS__)
#define LOGF_UTILITY(fmt_str, ...) REXLOG_DEBUG(fmt_str, ##__VA_ARGS__)
#define LOGF_WARNING(fmt_str, ...) REXLOG_WARN (fmt_str, ##__VA_ARGS__)
#define LOGF_ERROR(fmt_str, ...)   REXLOG_ERROR(fmt_str, ##__VA_ARGS__)

// Legacy LOGF_IMPL(Severity, "func_name", fmt, args...) — maps to the
// corresponding severity macro, dropping the func name (spdlog captures it).
#define LOGF_IMPL_Utility(func, fmt_str, ...) REXLOG_DEBUG(fmt_str, ##__VA_ARGS__)
#define LOGF_IMPL_Info(func, fmt_str, ...)    REXLOG_INFO (fmt_str, ##__VA_ARGS__)
#define LOGF_IMPL_Warning(func, fmt_str, ...) REXLOG_WARN (fmt_str, ##__VA_ARGS__)
#define LOGF_IMPL_Error(func, fmt_str, ...)   REXLOG_ERROR(fmt_str, ##__VA_ARGS__)
#define LOGF_IMPL_Trace(func, fmt_str, ...)   REXLOG_TRACE(fmt_str, ##__VA_ARGS__)
#define LOGF_IMPL_Debug(func, fmt_str, ...)   REXLOG_DEBUG(fmt_str, ##__VA_ARGS__)
#define LOGF_IMPL_None(func, fmt_str, ...)    REXLOG_INFO (fmt_str, ##__VA_ARGS__)
#define LOGF_IMPL(sev, func, fmt_str, ...)    LOGF_IMPL_##sev(func, fmt_str, ##__VA_ARGS__)

// Non-function-specific ("*") variants — spdlog doesn't distinguish, so they
// collapse onto the same severity macros. The visual "*" prefix from the old
// logger is no longer emitted; callers that relied on it should include a
// marker in their format string.
#define LOGN(str)               REXLOG_INFO ("{}", (str))
#define LOGN_WARNING(str)       REXLOG_WARN ("{}", (str))
#define LOGN_ERROR(str)         REXLOG_ERROR("{}", (str))
#define LOGN_UTILITY(str)       REXLOG_DEBUG("{}", (str))

#define LOGFN(fmt_str, ...)         REXLOG_INFO (fmt_str, ##__VA_ARGS__)
#define LOGFN_WARNING(fmt_str, ...) REXLOG_WARN (fmt_str, ##__VA_ARGS__)
#define LOGFN_ERROR(fmt_str, ...)   REXLOG_ERROR(fmt_str, ##__VA_ARGS__)
#define LOGFN_UTILITY(fmt_str, ...) REXLOG_DEBUG(fmt_str, ##__VA_ARGS__)

namespace os::logger
{
    // Severity enum kept for any code that references os::logger::ELogType
    // directly. Values map 1:1 to the legacy enum for ABI/source compat.
    enum class ELogType
    {
        None,     // -> info
        Utility,  // -> debug
        Warning,  // -> warn
        Error,    // -> error
        Trace,    // -> trace
        Debug = Utility,
        Info  = None,
        Warn  = Warning,
    };

    // Init/Shutdown are thin wrappers over rex::InitLogging / rex::ShutdownLogging.
    // Platform-specific logger_*.cpp implementations can register additional
    // sinks via rex::AddSink() during or after Init().
    void Init();
    void Shutdown();

    // Platform-specific sink registration hooks implemented by
    // os/<platform>/logger_<platform>.cpp (macos, win32, linux, ps4, switch).
    void PlatformInitSinks();
    void PlatformShutdownSinks();

    // Forwards a pre-formatted string to spdlog at the requested severity.
    // The `func` parameter is accepted for source compatibility with the
    // legacy signature but is ignored — spdlog captures __FUNCTION__ via
    // source_loc inside the REXLOG_* macros.
    inline void Log(std::string_view str,
                    ELogType type = ELogType::None,
                    const char* /*func*/ = nullptr)
    {
        switch (type)
        {
            case ELogType::Warning: REXLOG_WARN ("{}", str); break;
            case ELogType::Error:   REXLOG_ERROR("{}", str); break;
            case ELogType::Utility: REXLOG_DEBUG("{}", str); break;
            case ELogType::Trace:   REXLOG_TRACE("{}", str); break;
            case ELogType::None:
            default:                REXLOG_INFO ("{}", str); break;
        }
    }
}
