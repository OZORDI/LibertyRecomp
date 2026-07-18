/**
 * @file        core/platform/env_posix.cpp
 * @brief       POSIX implementation of rex::platform::env.
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 */

#include <rex/platform.h>
#include <rex/platform/env.h>

static_assert(REX_PLATFORM_LINUX || REX_PLATFORM_MAC, "This file is POSIX-only");

#include <cstdlib>
#include <string>

namespace rex::platform::env {

std::optional<std::string> get(std::string_view name) {
  std::string c_name(name);
  const char* value = std::getenv(c_name.c_str());
  if (!value) {
    return std::nullopt;
  }
  return std::string(value);
}

bool set(std::string_view name, std::string_view value) {
  std::string c_name(name);
  std::string c_value(value);
  return ::setenv(c_name.c_str(), c_value.c_str(), 1) == 0;
}

bool unset(std::string_view name) {
  std::string c_name(name);
  return ::unsetenv(c_name.c_str()) == 0;
}

}  // namespace rex::platform::env
