/**
 * @file        tests/unit/rexglue/ui_test.cpp
 * @brief       Unit tests for the rexglue presentation layer
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 * @license     BSD 3-Clause License
 */

#include "rexglue/ui/progress.h"
#include "rexglue/ui/ui.h"

#include <array>
#include <chrono>
#include <memory>
#include <sstream>
#include <string>

#include <catch2/catch_test_macros.hpp>
#include <spdlog/spdlog.h>

namespace {

// Test helper: build a logger that writes through the sink under test.
struct LoggerHarness {
  std::ostringstream out;
  std::shared_ptr<rexglue::ui::PresentationSink> sink;
  std::shared_ptr<spdlog::logger> logger;

  explicit LoggerHarness(bool tty = false, bool color = false)
      : sink(std::make_shared<rexglue::ui::PresentationSink>(out, tty, color)),
        logger(std::make_shared<spdlog::logger>("test", sink)) {
    logger->set_level(spdlog::level::trace);
  }
};

// For block-writer tests we need ui:: free functions to find a global sink.
struct UiBlockHarness {
  std::ostringstream out;
  UiBlockHarness() {
    rexglue::ui::Shutdown();
    rexglue::ui::detail::SetGlobalSinkForTesting(
        std::make_unique<rexglue::ui::PresentationSink>(out, /*tty=*/false,
                                                        /*color=*/false));
  }
  ~UiBlockHarness() { rexglue::ui::Shutdown(); }
};

}  // namespace

TEST_CASE("PresentationSink routes spdlog messages to the injected stream", "[ui][sink]") {
  LoggerHarness h;
  h.logger->info("hello world");
  h.logger->flush();
  REQUIRE(h.out.str().find("hello world") != std::string::npos);
}

TEST_CASE("ui::Banner writes title followed by blank line", "[ui][block]") {
  UiBlockHarness h;
  rexglue::ui::Banner("ReXGlue 0.8.0.26");
  REQUIRE(h.out.str() == "ReXGlue 0.8.0.26\n\n");
}

TEST_CASE("ui::KeyValueBlock aligns keys and emits header", "[ui][block]") {
  UiBlockHarness h;
  std::array<rexglue::ui::KeyValueRow, 2> rows = {{
      {"Manifest", "/abs/manifest.toml"},
      {"Project", "reblue (entrypoint + 2 DLLs)"},
  }};
  rexglue::ui::KeyValueBlock("", rows);
  auto s = h.out.str();
  REQUIRE(s.find("  Manifest: /abs/manifest.toml\n") != std::string::npos);
  REQUIRE(s.find("  Project:  reblue (entrypoint + 2 DLLs)\n") != std::string::npos);
}

TEST_CASE("ui::PlanTable formats rows with action label and reason", "[ui][block]") {
  UiBlockHarness h;
  std::array<rexglue::ui::PlanRow, 2> rows = {{
      {"write ", "/a/b.toml", "upgrade format"},
      {"delete", "/a/c.toml", "absorbed"},
  }};
  rexglue::ui::PlanTable("Migration: 2 file(s) will be rewritten.", rows);
  auto s = h.out.str();
  REQUIRE(s.find("Migration: 2 file(s) will be rewritten.\n") != std::string::npos);
  REQUIRE(s.find("  [write ] /a/b.toml") != std::string::npos);
  REQUIRE(s.find("upgrade format") != std::string::npos);
  REQUIRE(s.find("  [delete] /a/c.toml") != std::string::npos);
  REQUIRE(s.find("absorbed") != std::string::npos);
}

TEST_CASE("ui::ManualReviewList nests detail and optional hint", "[ui][block]") {
  UiBlockHarness h;
  std::array<rexglue::ui::ManualReviewRow, 2> rows = {{
      {"f.cpp:329", "single-arg call", "pass ctx.lr instead"},
      {"g.cpp:43", "removed positional argument", ""},
  }};
  rexglue::ui::ManualReviewList("Migration: 2 site(s) need manual review:", rows);
  auto s = h.out.str();
  REQUIRE(s.find("Migration: 2 site(s) need manual review:\n") != std::string::npos);
  REQUIRE(s.find("  f.cpp:329  single-arg call\n") != std::string::npos);
  REQUIRE(s.find("    pass ctx.lr instead\n") != std::string::npos);
  REQUIRE(s.find("  g.cpp:43  removed positional argument\n") != std::string::npos);
  REQUIRE(s.find("    \n") == std::string::npos);
}

TEST_CASE("ui::Confirm returns false on non-TTY without consuming input", "[ui][confirm]") {
  UiBlockHarness h;
  std::istringstream input("y\n");
  bool result = rexglue::ui::detail::ConfirmWithStream("Apply?", input, /*tty=*/false);
  REQUIRE(result == false);
  REQUIRE(input.tellg() == 0);
  REQUIRE(h.out.str().find("Apply? [y/N]: ") != std::string::npos);
}

TEST_CASE("ui::Confirm returns true for 'y' on TTY", "[ui][confirm]") {
  UiBlockHarness h;
  std::istringstream input("y\n");
  REQUIRE(rexglue::ui::detail::ConfirmWithStream("Apply?", input,
                                                 /*tty=*/true) == true);
}

TEST_CASE("ui::Confirm returns true for 'YES' (case-insensitive)", "[ui][confirm]") {
  UiBlockHarness h;
  std::istringstream input("YES\n");
  REQUIRE(rexglue::ui::detail::ConfirmWithStream("Apply?", input,
                                                 /*tty=*/true) == true);
}

TEST_CASE("ui::Confirm returns false for 'n', empty, and EOF", "[ui][confirm]") {
  for (const std::string& answer : {std::string{"n\n"}, std::string{"\n"}, std::string{}}) {
    UiBlockHarness h;
    std::istringstream input(answer);
    REQUIRE(rexglue::ui::detail::ConfirmWithStream("Apply?", input,
                                                   /*tty=*/true) == false);
  }
}

TEST_CASE("ProgressView non-TTY emits one line per event in order", "[ui][progress]") {
  UiBlockHarness h;
  {
    rexglue::ui::ProgressView pv("Recompiling reblue (entrypoint + 1 DLL)");
    pv.moduleStarted("reblue", 0, 2);
    pv.phaseChanged("Register");
    pv.phaseChanged("Discover");
    pv.moduleFinished(std::chrono::milliseconds{1234});
    pv.moduleStarted("bdengine", 1, 2);
    pv.phaseChanged("Register");
    pv.moduleFinished(std::chrono::milliseconds{2345});
  }
  auto s = h.out.str();
  REQUIRE(s.find("Recompiling reblue (entrypoint + 1 DLL)\n") == 0);
  auto pos1 = s.find("  start  reblue");
  auto pos2 = s.find("  phase  reblue: Register");
  auto pos3 = s.find("  phase  reblue: Discover");
  auto pos4 = s.find("  done   reblue (1.2s)");
  auto pos5 = s.find("  start  bdengine");
  auto pos6 = s.find("  done   bdengine (2.3s)");
  REQUIRE(pos1 != std::string::npos);
  REQUIRE(pos1 < pos2);
  REQUIRE(pos2 < pos3);
  REQUIRE(pos3 < pos4);
  REQUIRE(pos4 < pos5);
  REQUIRE(pos5 < pos6);
}

TEST_CASE("ui::DoneSummary writes single Done line", "[ui][summary]") {
  UiBlockHarness h;
  rexglue::ui::DoneSummary(std::chrono::milliseconds{8400});
  REQUIRE(h.out.str() == "Done in 8.4s.\n");
}

TEST_CASE("ui::FailureSummary writes Failed line with reason and duration", "[ui][summary]") {
  UiBlockHarness h;
  rexglue::ui::FailureSummary("manifest not found", std::chrono::milliseconds{2500});
  REQUIRE(h.out.str() == "Failed: manifest not found (after 2.5s)\n");
}
