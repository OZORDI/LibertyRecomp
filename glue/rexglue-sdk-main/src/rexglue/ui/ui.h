/**
 * @file        rexglue/ui/ui.h
 * @brief       Presentation layer for the rexglue CLI
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 */

#pragma once

#include <chrono>
#include <functional>
#include <iosfwd>
#include <memory>
#include <mutex>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include <spdlog/sinks/base_sink.h>

namespace rexglue::ui {

namespace detail {
class GlobalSinkAccessor;
}

/** spdlog sink that also owns direct presentation output for the rexglue CLI. */
class PresentationSink final : public spdlog::sinks::base_sink<std::mutex> {
 public:
  PresentationSink(std::ostream& out, bool tty, bool color);

  bool tty() const { return tty_; }
  bool color() const { return color_; }

  void setActiveProgress(bool active);

 protected:
  void sink_it_(const spdlog::details::log_msg& msg) override;
  void flush_() override;

 private:
  friend class detail::GlobalSinkAccessor;

  std::mutex& sink_mutex() { return this->mutex_; }
  std::ostream& sink_out() { return out_; }

  std::ostream& out_;
  bool tty_;
  bool color_;
  bool active_progress_ = false;
};

struct InitOptions {
  bool tty;
  bool color;
};

void Init(const InitOptions& opts);
void Shutdown();

void Banner(std::string_view title);

struct KeyValueRow {
  std::string_view key;
  std::string value;
};
void KeyValueBlock(std::string_view header, std::span<const KeyValueRow> rows);

struct PlanRow {
  std::string_view action_label;
  std::string path;
  std::string reason;
};
void PlanTable(std::string_view header, std::span<const PlanRow> rows);

struct ManualReviewRow {
  std::string location;
  std::string detail;
  std::string hint;
};
void ManualReviewList(std::string_view header, std::span<const ManualReviewRow> rows);

[[nodiscard]] bool Confirm(std::string_view question);

void DoneSummary(std::chrono::milliseconds elapsed);
void FailureSummary(std::string_view reason, std::chrono::milliseconds elapsed);

namespace detail {

void SetGlobalSinkForTesting(std::unique_ptr<PresentationSink> sink);

[[nodiscard]] bool ConfirmWithStream(std::string_view question, std::istream& in,
                                     bool stdin_is_tty);

bool SinkIsTty();

void SetSinkActiveProgress(bool active);

/** RAII lock on the global sink's mutex for write access. */
class GlobalSinkAccessor {
 public:
  GlobalSinkAccessor();
  ~GlobalSinkAccessor();
  GlobalSinkAccessor(const GlobalSinkAccessor&) = delete;
  GlobalSinkAccessor& operator=(const GlobalSinkAccessor&) = delete;

  void writeLine(std::string_view s);
  void writeRaw(std::string_view s);
  void flush();

  bool color() const;

  void writeColored(std::string_view code, std::string_view text);
};

}  // namespace detail

namespace color {
inline constexpr std::string_view kReset = "\033[0m";
inline constexpr std::string_view kBold = "\033[1m";
inline constexpr std::string_view kDim = "\033[2m";
inline constexpr std::string_view kRed = "\033[31m";
inline constexpr std::string_view kGreen = "\033[32m";
inline constexpr std::string_view kCyan = "\033[36m";
inline constexpr std::string_view kBoldGreen = "\033[1;32m";
inline constexpr std::string_view kBoldRed = "\033[1;31m";
inline constexpr std::string_view kBoldYellow = "\033[1;33m";
}  // namespace color

}  // namespace rexglue::ui
