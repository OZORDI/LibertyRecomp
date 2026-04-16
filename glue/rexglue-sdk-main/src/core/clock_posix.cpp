/**
 * @file        core/clock_posix.cpp
 * @brief       POSIX platform clock implementations
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 * @license     BSD 3-Clause License
 */

#include <rex/assert.h>
#include <rex/chrono/clock.h>
#include <rex/platform.h>

static_assert(REX_PLATFORM_POSIX, "This file is POSIX-only");

#include <sys/time.h>

// CLOCK_MONOTONIC_RAW is a Linux extension (avoids NTP slew adjustments).
// macOS and other POSIX platforms only expose CLOCK_MONOTONIC.
#if REX_PLATFORM_LINUX && defined(CLOCK_MONOTONIC_RAW)
#  define REX_CLOCK_MONOTONIC CLOCK_MONOTONIC_RAW
#else
#  define REX_CLOCK_MONOTONIC CLOCK_MONOTONIC
#endif

namespace rex::chrono {

uint64_t Clock::host_tick_frequency_platform() {
  timespec res;
  int error = clock_getres(REX_CLOCK_MONOTONIC, &res);
  assert_zero(error);
  assert_zero(res.tv_sec);  // Sub second resolution is required.

  // Convert nano seconds to hertz. Resolution is 1ns on most systems.
  return 1000000000ull / res.tv_nsec;
}

uint64_t Clock::host_tick_count_platform() {
  timespec tp;
  int error = clock_gettime(REX_CLOCK_MONOTONIC, &tp);
  assert_zero(error);

  return tp.tv_nsec + tp.tv_sec * 1000000000ull;
}

uint64_t Clock::QueryHostSystemTime() {
  // https://docs.microsoft.com/en-us/windows/win32/sysinfo/converting-a-time-t-value-to-a-file-time
  constexpr uint64_t seconds_per_day = 3600 * 24;
  // Don't forget the 89 leap days.
  constexpr uint64_t seconds_1601_to_1970 = ((369 * 365 + 89) * seconds_per_day);

  timeval now;
  int error = gettimeofday(&now, nullptr);
  assert_zero(error);

  // NT systems use 100ns intervals.
  return static_cast<uint64_t>(
      (static_cast<int64_t>(now.tv_sec) + seconds_1601_to_1970) * 10000000ull + now.tv_usec * 10);
}

uint64_t Clock::QueryHostUptimeMillis() {
  return host_tick_count_platform() * 1000 / host_tick_frequency_platform();
}

}  // namespace rex::chrono
