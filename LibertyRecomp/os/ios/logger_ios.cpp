// iOS logger — custom spdlog sink routing through os_log (unified logging).
#include <rex/platform.h>

#if REX_PLATFORM_IOS
#include <os/logger.h>

#include <cstdio>
#include <memory>
#include <mutex>
#include <string>

#include <os/log.h>

#include <rex/logging.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/log_msg.h>

namespace
{
    static os_log_t g_log = nullptr;

    template<typename Mutex>
    class os_log_sink : public spdlog::sinks::base_sink<Mutex>
    {
    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            spdlog::memory_buf_t formatted;
            spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

            os_log_type_t logType;
            switch (msg.level)
            {
            case spdlog::level::warn:     logType = OS_LOG_TYPE_DEFAULT; break;
            case spdlog::level::err:
            case spdlog::level::critical: logType = OS_LOG_TYPE_ERROR;   break;
            case spdlog::level::debug:
            case spdlog::level::trace:    logType = OS_LOG_TYPE_DEBUG;   break;
            default:                      logType = OS_LOG_TYPE_DEFAULT; break;
            }

            // os_log requires a null-terminated C string.
            std::string s(formatted.data(), formatted.size());
            os_log_with_type(g_log, logType, "%{public}s", s.c_str());

            // Also mirror to stderr for Xcode's debug console.
            fwrite(s.data(), 1, s.size(), stderr);
        }

        void flush_() override {}
    };

    using os_log_sink_mt = os_log_sink<std::mutex>;
}

void os::logger::Init()
{
    if (!g_log)
        g_log = os_log_create("com.libertyrecomp.app", "general");
}

void os::logger::PlatformInitSinks()
{
    Init();
    auto sink = std::make_shared<os_log_sink_mt>();
    rex::logging::AddSink(sink);
}

#endif // REX_PLATFORM_IOS
