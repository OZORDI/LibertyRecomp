/**
 * @file        core/logging_ios.mm
 * @brief       iOS Apple unified-logging sink for rex::logging.
 *
 * Mirrors log output to os_log so messages appear in Console.app,
 * `log stream`, and system sysdiagnose captures. Subsystem is
 * "com.liberty.recomp" / category "runtime". Runs ALONGSIDE the
 * rex stdout + rotating-file sinks.
 */

#include <rex/platform.h>

#if REX_PLATFORM_IOS

#include <memory>
#include <mutex>
#include <string>

#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/sink.h>

#include <os/log.h>

#include <rex/logging.h>

namespace rex::logging {

namespace {

class IosOsLogSink final : public spdlog::sinks::base_sink<std::mutex> {
 public:
  IosOsLogSink() : log_(os_log_create("com.liberty.recomp", "runtime")) {}

 protected:
  void sink_it_(const spdlog::details::log_msg& msg) override {
    spdlog::memory_buf_t formatted;
    formatter_->format(msg, formatted);
    const std::string str(formatted.data(), formatted.size());

    os_log_type_t type = OS_LOG_TYPE_DEFAULT;
    switch (msg.level) {
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

}  // namespace

std::shared_ptr<spdlog::sinks::sink> CreateNativeLogSink() {
  return std::make_shared<IosOsLogSink>();
}

}  // namespace rex::logging

#endif  // REX_PLATFORM_IOS
