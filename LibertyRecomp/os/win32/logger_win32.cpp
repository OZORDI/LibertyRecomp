#include <os/logger.h>
#include <rex/logging.h>
#include <rex/diagnostics/policy.h>

#include <spdlog/sinks/wincolor_sink.h>
#include <spdlog/sinks/msvc_sink.h>

void os::logger::PlatformInitSinks()
{
    if (!rex::diagnostics::IsEnabled(
            rex::diagnostics::Category::kLogging)) return;
    // Colored console output for attached terminals.
    auto console_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
    rex::AddSink(console_sink);

    // Mirror log output to the Visual Studio / WinDbg output window.
    auto debug_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
    rex::AddSink(debug_sink);
}

void os::logger::PlatformShutdownSinks()
{
    // spdlog handles sink cleanup on shutdown; nothing to do here.
}
