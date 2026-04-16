/**
 * @file        core/logging_ps4.cpp
 * @brief       PS4 (Orbis) native kernel-debug log sink for rex::logging.
 *
 * Forwards formatted spdlog messages to sceKernelDebugOutText so that log
 * output appears over TTY/UART (devkit) and in system kernel logs. This
 * runs ALONGSIDE the stdout + rotating-file sinks already registered by
 * rex::InitLogging — it does not replace them.
 */

#include <rex/platform.h>

#if REX_PLATFORM_PS4

#include <memory>
#include <mutex>
#include <string>

#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/sink.h>

#include <orbis/libkernel.h>

#include <rex/logging.h>

namespace rex::logging {

namespace {

class Ps4DebugSink final : public spdlog::sinks::base_sink<std::mutex> {
 protected:
  void sink_it_(const spdlog::details::log_msg& msg) override {
    spdlog::memory_buf_t formatted;
    formatter_->format(msg, formatted);
    std::string line(formatted.data(), formatted.size());
    // Ensure a single trailing newline — sceKernelDebugOutText does not add one.
    if (line.empty() || line.back() != '\n')
      line.push_back('\n');
    sceKernelDebugOutText(0, line.c_str());
  }

  void flush_() override {}
};

}  // namespace

std::shared_ptr<spdlog::sinks::sink> CreateNativeLogSink() {
  return std::make_shared<Ps4DebugSink>();
}

}  // namespace rex::logging

#endif  // REX_PLATFORM_PS4
