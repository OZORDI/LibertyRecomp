#include <os/logger.h>
#include <rex/logging.h>

#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/sinks/base_sink.h>

#include <os/log.h>
#include <mutex>

namespace
{
    // Mirrors log output to Apple's unified logging system so that messages
    // appear in Console.app and `log stream` even when stdout is not a TTY
    // (e.g. when the .app is launched from Finder).
    class OsLogSink final : public spdlog::sinks::base_sink<std::mutex>
    {
    public:
        OsLogSink()
            : log_(os_log_create("com.liberty.recomp", "default"))
        {
        }

    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            spdlog::memory_buf_t formatted;
            formatter_->format(msg, formatted);

            const std::string str(formatted.data(), formatted.size());

            os_log_type_t type = OS_LOG_TYPE_DEFAULT;
            switch (msg.level)
            {
                case spdlog::level::trace:
                case spdlog::level::debug:    type = OS_LOG_TYPE_DEBUG;   break;
                case spdlog::level::info:     type = OS_LOG_TYPE_INFO;    break;
                case spdlog::level::warn:     type = OS_LOG_TYPE_DEFAULT; break;
                case spdlog::level::err:      type = OS_LOG_TYPE_ERROR;   break;
                case spdlog::level::critical: type = OS_LOG_TYPE_FAULT;   break;
                default:                      type = OS_LOG_TYPE_DEFAULT; break;
            }

            os_log_with_type(log_, type, "%{public}s", str.c_str());
        }

        void flush_() override {}

    private:
        os_log_t log_;
    };
}

void os::logger::PlatformInitSinks()
{
    // Colored stdout for Terminal.app / piped runs.
    // rexglue's rotating file sink (driven by log_file / log_max_file_size_mb /
    // log_max_files CVARs) handles persistent logs, so no file rotation here.
    auto console_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
    rex::AddSink(console_sink);

    // Apple unified logging — visible in Console.app under the
    // "com.liberty.recomp" subsystem.
    auto os_sink = std::make_shared<OsLogSink>();
    rex::AddSink(os_sink);
}

void os::logger::PlatformShutdownSinks()
{
    // spdlog handles sink cleanup on shutdown; nothing to do here.
}


// Top-level Init/Shutdown expected by main.cpp.
//
// rex::InitLoggingEarly() auto-fires on the first GetLogger*() call and
// installs a short-pattern stdout sink that is ONLY evicted when the full
// rex::InitLogging(LogConfig) runs. Without that, PlatformInitSinks() below
// would add a SECOND stdout sink on top of it, producing the duplicated
// log output we were seeing (short form + long form on every line).
//
// Call rex::InitLogging() here with log_to_console=false so rex evicts the
// early sink but does not install its own console sink — PlatformInitSinks
// then owns the single ansicolor stdout sink + Apple os_log sink.
void os::logger::Init()
{
    rex::LogConfig cfg;
    cfg.log_to_console = false;      // PlatformInitSinks adds the real console sink.
    cfg.default_level  = spdlog::level::trace;
    cfg.flush_level    = spdlog::level::trace;
    rex::InitLogging(cfg);

    PlatformInitSinks();
}

void os::logger::Shutdown()
{
    PlatformShutdownSinks();
}
