#pragma once

/**
 * @file        net/net_switch.h
 * @brief       Nintendo Switch (libnx) network initialization helpers
 *
 * These are only available when building for Switch (REX_PLATFORM_NX).
 * Call switch_net_initialize() before any BSD socket or DNS calls.
 *
 * libnx's socketInitializeDefault() sets up the BSD socket driver and
 * enables standard POSIX getaddrinfo() / gethostbyname() automatically.
 * nifm (Network Interface Module) is used to check connectivity status.
 */

#include <rex/platform.h>

#if REX_PLATFORM_NX

namespace rex::net {

/// Initialize the Switch networking stack (nifmInitialize + socketInitializeDefault).
/// Safe to call multiple times -- subsequent calls are no-ops.
bool switch_net_initialize();

/// Shut down the Switch networking stack.
void switch_net_shutdown();

/// Returns true if the console currently has an active internet connection.
bool switch_net_is_connected();

}  // namespace rex::net

#endif  // REX_PLATFORM_NX
