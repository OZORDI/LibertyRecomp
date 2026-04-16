/**
 * @file        core/net_android.cpp
 * @brief       Android network stack entry points.
 *
 * Android (bionic libc) exposes standard BSD sockets from process launch, so
 * no explicit init is required. These functions exist purely so that xam_net
 * can call a uniform PlatformInit() / PlatformShutdown() pair on every
 * platform without any `#ifdef REX_PLATFORM_*` in its hot path.
 *
 * Note: the android.permission.INTERNET permission must be declared in the
 * app manifest or socket() will fail with EACCES. That is an app/packaging
 * concern, not something this file can enforce.
 */

#include <rex/platform.h>

#if REX_PLATFORM_ANDROID

#include <rex/logging.h>
#include <rex/net/platform_net.h>

namespace rex::net {

static bool s_net_initialized = false;

bool PlatformInit() {
  if (s_net_initialized) {
    return true;
  }
  s_net_initialized = true;
  REXKRNL_INFO("Android network stack ready (BSD sockets)");
  return true;
}

void PlatformShutdown() {
  if (!s_net_initialized) {
    return;
  }
  s_net_initialized = false;
  REXKRNL_INFO("Android network stack shut down");
}

}  // namespace rex::net

#endif  // REX_PLATFORM_ANDROID
