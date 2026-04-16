/**
 * @file        core/logging_switch.cpp
 * @brief       Nintendo Switch (libnx) native debug log sink for rex::logging.
 *
 * Forwards formatted spdlog messages to svcOutputDebugString so that log
 * output appears in the devkit/IS-Nitro console and — when NXLINK_DEBUG
 * is defined (or the NXLINK_DEBUG env variable is set) — is also mirrored
 * to stdout, which nxlink redirects to the host.
 *
 * Runs ALONGSIDE the rex stdout + rotating-file sinks.
 */

#include <rex/platform.h>

#if REX_PLATFORM_NX || REX_PLATFORM_SWITCH

#include <cstdio>
#include <cstdlib>
#include <memory>
#include <mutex>
#include <string>

#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/sink.h>

#include <switch.h>

#include <rex/logging.h>

namespace rex::logging {

namespace {

class SwitchDebugSink final : public spdlog::sinks::base_sink<std::mutex> {
 public:
  SwitchDebugSink() {
#ifdef NXLINK_DEBUG
    nxlink_mirror_ = true;
#else
    const char* env = std::getenv("NXLINK_DEBUG");
    nxlink_mirror_ = (env != nullptr && env[0] != '\0' && env[0] != '0');
#endif
  }

 protected:
  void sink_it_(const spdlog::details::log_msg& msg) override {
    spdlog::memory_buf_t formatted;
    formatter_->format(msg, formatted);
    std::string line(formatted.data(), formatted.size());
    if (line.empty() || line.back() != '\n')
      line.push_back('\n');

    // Kernel/devkit debug channel — visible under svcOutputDebugString.
    svcOutputDebugString(line.c_str(), line.size());

    // nxlink mirror — stdout is redirected to the host when nxlinkStdio()
    // was called during startup. Guarded so we don't double-print when the
    // rex console sink is also active.
    if (nxlink_mirror_) {
      std::fwrite(line.data(), 1, line.size(), stdout);
      std::fflush(stdout);
    }
  }

  void flush_() override {
    if (nxlink_mirror_)
      std::fflush(stdout);
  }

 private:
  bool nxlink_mirror_ = false;
};

}  // namespace

std::shared_ptr<spdlog::sinks::sink> CreateNativeLogSink() {
  return std::make_shared<SwitchDebugSink>();
}

}  // namespace rex::logging

#endif  // REX_PLATFORM_NX || REX_PLATFORM_SWITCH
