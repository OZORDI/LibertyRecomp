/**
 * @file        core/platform/console_posix.cpp
 * @brief       POSIX implementation of rex::platform::console.
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 */

#include <rex/platform.h>
#include <rex/platform/console.h>

static_assert(REX_PLATFORM_LINUX || REX_PLATFORM_MAC, "This file is POSIX-only");

#include <unistd.h>

namespace rex::platform::console {

bool is_tty(FILE* stream) {
  if (!stream) {
    return false;
  }
  return ::isatty(::fileno(stream)) != 0;
}

bool enable_ansi_escapes(FILE* stream) {
  return stream != nullptr;
}

void set_utf8_output_codepage() {}

}  // namespace rex::platform::console
