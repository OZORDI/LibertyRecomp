// Switch logger — custom spdlog sink routing through svcOutputDebugString
// (visible to Atmosphere's sysmodule log, Goldleaf, and nxlink).
#include <rex/platform.h>
#if REX_PLATFORM_NX
#include <os/logger.h>

#include <cstdio>
#include <memory>
#include <mutex>

#include <switch.h>

#include <rex/logging.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/log_msg.h>

namespace
{
    template<typename Mutex>
    class switch_debug_sink : public spdlog::sinks::base_sink<Mutex>
    {
    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            spdlog::memory_buf_t formatted;
            spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

            // svcOutputDebugString takes (ptr, byte length) — no NUL required.
            svcOutputDebugString(formatted.data(), static_cast<u64>(formatted.size()));

            // Mirror to stdout so nxlink / console sinks see it too.
            fwrite(formatted.data(), 1, formatted.size(), stdout);
        }

        void flush_() override
        {
            fflush(stdout);
        }
    };

    using switch_debug_sink_mt = switch_debug_sink<std::mutex>;
}

void os::logger::Init()
{
    const char kInitMsg[] = "LibertyRecomp: logger init\n";
    svcOutputDebugString(kInitMsg, sizeof(kInitMsg) - 1);
}

void os::logger::PlatformInitSinks()
{
    Init();
    auto sink = std::make_shared<switch_debug_sink_mt>();
    rex::logging::AddSink(sink);
}

#endif // REX_PLATFORM_NX
