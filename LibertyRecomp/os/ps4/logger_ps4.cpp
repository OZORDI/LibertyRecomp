// PS4 logger — custom spdlog sink routing through sceKernelDebugOutText
// (OpenOrbis / Mira jailbreak debug sink).
#ifdef LIBERTY_RECOMP_PS4
#include <os/logger.h>

#include <cstdio>
#include <memory>
#include <mutex>
#include <string>

#include <orbis/libkernel.h>

#include <rex/logging.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/log_msg.h>

namespace
{
    template<typename Mutex>
    class ps4_debug_sink : public spdlog::sinks::base_sink<Mutex>
    {
    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            spdlog::memory_buf_t formatted;
            spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

            // sceKernelDebugOutText wants a null-terminated C string.
            std::string s(formatted.data(), formatted.size());
            sceKernelDebugOutText(0, s.c_str());

            // Also go to stdout (OpenOrbis forwards fd 1 to its debug sink).
            fwrite(s.data(), 1, s.size(), stdout);
        }

        void flush_() override
        {
            fflush(stdout);
        }
    };

    using ps4_debug_sink_mt = ps4_debug_sink<std::mutex>;
}

void os::logger::Init()
{
    sceKernelDebugOutText(0, "LibertyRecomp: logger initialized\n");
}

void os::logger::PlatformInitSinks()
{
    Init();
    auto sink = std::make_shared<ps4_debug_sink_mt>();
    rex::AddSink(sink);
}

#endif // LIBERTY_RECOMP_PS4
