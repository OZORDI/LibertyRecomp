/**
 * @file        core/net_ios.cpp
 * @brief       iOS network stack entry points.
 *
 * iOS exposes the standard BSD socket API from process launch — no explicit
 * bring-up is required (unlike PS4's sceNet or Switch's nifm/bsd services).
 * These functions exist purely so that xam_net can call a uniform
 * PlatformInit() / PlatformShutdown() pair on every platform without any
 * `#ifdef REX_PLATFORM_*` in its hot path.
 *
 * If the app ever needs NWPathMonitor or reachability wiring, plumb it in
 * here so xam_net stays platform-agnostic.
 */

#include <rex/platform.h>

#if REX_PLATFORM_IOS

#include <rex/logging.h>
#include <rex/net/platform_net.h>

namespace rex::net {

static bool s_net_initialized = false;

bool PlatformInit() {
  if (s_net_initialized) {
    return true;
  }
  s_net_initialized = true;
  REXKRNL_INFO("iOS network stack ready (BSD sockets)");
  return true;
}

void PlatformShutdown() {
  if (!s_net_initialized) {
    return;
  }
  s_net_initialized = false;
  REXKRNL_INFO("iOS network stack shut down");
}

}  // namespace rex::net

#endif  // REX_PLATFORM_IOS
