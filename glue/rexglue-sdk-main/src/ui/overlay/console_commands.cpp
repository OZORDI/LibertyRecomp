/**
 * @file        ui/overlay/console_commands.cpp
 *
 * @brief       Sample argument-taking console commands (echo, find) that
 *              exercise the REXCVAR_DEFINE_COMMAND_ARGS path end to end.
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 */
#include <rex/cvar.h>
#include <rex/logging.h>

#include <string>
#include <string_view>

namespace {

void ConsoleEcho(std::string_view args) {
  REXLOG_INFO("{}", args);
}

void ConsoleFind(std::string_view args) {
  while (!args.empty() && args.front() == ' ')
    args.remove_prefix(1);
  while (!args.empty() && args.back() == ' ')
    args.remove_suffix(1);
  if (args.empty()) {
    REXLOG_INFO("find: usage: find <substr>");
    return;
  }
  std::string needle(args);
  int matches = 0;
  for (const auto& n : rex::cvar::ListFlags()) {
    if (n.find(needle) != std::string::npos) {
      REXLOG_INFO("  {}", n);
      ++matches;
    }
  }
  REXLOG_INFO("find: {} match(es) for '{}'", matches, needle);
}

}  // namespace

REXCVAR_DEFINE_COMMAND_ARGS(echo, ConsoleEcho, "Console", "Echo arguments to the console");
REXCVAR_DEFINE_COMMAND_ARGS(find, ConsoleFind, "Console",
                            "List cvar/command names containing a substring");
