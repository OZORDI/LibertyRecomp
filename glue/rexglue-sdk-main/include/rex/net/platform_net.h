#pragma once

/**
 * @file        net/platform_net.h
 * @brief       Cross-platform network stack init/teardown entry points.
 *
 * Every supported platform provides:
 *   bool rex::net::PlatformInit();
 *   void rex::net::PlatformShutdown();
 *
 * Desktop platforms (Windows, macOS, Linux) use a header-inline no-op because
 * BSD / Winsock sockets are available from process start (Winsock is brought
 * up separately via WSAStartup in xam_net). Console platforms (PS4, Switch)
 * and mobile platforms (iOS, Android) provide real implementations in their
 * respective net_<platform>.cpp. This lets xam_net call PlatformInit() /
 * PlatformShutdown() without any `#ifdef REX_PLATFORM_*` in the hot path.
 */

#include <rex/platform.h>

namespace rex::net {

#if REX_PLATFORM_PS4 || REX_PLATFORM_NX || REX_PLATFORM_IOS || REX_PLATFORM_ANDROID

// Out-of-line — defined in net_ps4.cpp / net_switch.cpp / net_ios.cpp / net_android.cpp.
bool PlatformInit();
void PlatformShutdown();

#else

// Desktop platforms: BSD sockets / Winsock are always available.
// Winsock is initialized separately in NetDll_WSAStartup_entry.
inline bool PlatformInit() { return true; }
inline void PlatformShutdown() {}

#endif

}  // namespace rex::net
