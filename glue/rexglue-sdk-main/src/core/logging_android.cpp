/**
 * @file        core/logging_android.cpp
 * @brief       Android logcat sink for rex::logging.
 *
 * Forwards formatted spdlog messages to __android_log_print under the
 * "LibertyRecomp" tag so output appears via `adb logcat`. Runs ALONGSIDE
 * the rex stdout + rotating-file sinks.
 */

#include <rex/platform.h>

#if REX_PLATFORM_ANDROID

#include <memory>
#include <mutex>
#include <string>

#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/sink.h>

#include <android/log.h>

#include <rex/logging.h>

namespace rex::logging {

namespace {

class AndroidLogSink final : public spdlog::sinks::base_sink<std::mutex> {
 protected:
  void sink_it_(const spdlog::details::log_msg& msg) override {
    spdlog::memory_buf_t formatted;
    formatter_->format(msg, formatted);
    const std::string str(formatted.data(), formatted.size());

    int priority = ANDROID_LOG_INFO;
    switch (msg.level) {
      case spdlog::level::trace:    priority = ANDROID_LOG_VERBOSE; break;
      case spdlog::level::debug:    priority = ANDROID_LOG_DEBUG;   break;
      case spdlog::level::info:     priority = ANDROID_LOG_INFO;    break;
      case spdlog::level::warn:     priority = ANDROID_LOG_WARN;    break;
      case spdlog::level::err:      priority = ANDROID_LOG_ERROR;   break;
      case spdlog::level::critical: priority = ANDROID_LOG_FATAL;   break;
      default:                      priority = ANDROID_LOG_INFO;    break;
    }

    __android_log_print(priority, "LibertyRecomp", "%s", str.c_str());
  }

  void flush_() override {}
};

}  // namespace

std::shared_ptr<spdlog::sinks::sink> CreateNativeLogSink() {
  return std::make_shared<AndroidLogSink>();
}

}  // namespace rex::logging

#endif  // REX_PLATFORM_ANDROID
