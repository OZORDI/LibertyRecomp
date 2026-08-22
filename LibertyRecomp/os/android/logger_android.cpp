// Android logger — uses spdlog's built-in android_sink (logcat via __android_log_write).
#include <rex/platform.h>
#if REX_PLATFORM_ANDROID
#include <os/logger.h>

#include <memory>

#include <android/log.h>

#include <rex/logging.h>
#include <rex/diagnostics/policy.h>
#include <spdlog/sinks/android_sink.h>

#define LIBERTY_LOG_TAG "LibertyRecomp"

void os::logger::Init()
{
    if (!rex::diagnostics::IsEnabled(
            rex::diagnostics::Category::kLogging)) return;
    __android_log_print(ANDROID_LOG_INFO, LIBERTY_LOG_TAG,
        "LibertyRecomp logger initialized (logcat)");
}

void os::logger::PlatformInitSinks()
{
    if (!rex::diagnostics::IsEnabled(
            rex::diagnostics::Category::kLogging)) return;
    Init();
    auto sink = std::make_shared<spdlog::sinks::android_sink_mt>(LIBERTY_LOG_TAG);
    rex::AddSink(sink);
}

#endif // REX_PLATFORM_ANDROID
