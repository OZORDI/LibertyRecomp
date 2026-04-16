#pragma once

/**
 * @file        net/net_ps4.h
 * @brief       PS4 sceNet / sceNetCtl initialization and query helpers
 *
 * These are only available when building for PS4 (REX_PLATFORM_PS4).
 * Call ps4_net_initialize() before any BSD socket or sceNet calls.
 */

#include <rex/platform.h>

#if REX_PLATFORM_PS4

#include <cstddef>
#include <cstdint>

namespace rex::net {

/// Initialize the PS4 networking stack (sceNetInit, sceNetPoolCreate, sceNetCtlInit).
/// Safe to call multiple times — subsequent calls are no-ops.
bool ps4_net_initialize();

/// Shut down the PS4 networking stack.
void ps4_net_shutdown();

/// Get the internal network pool ID created during initialization.
int32_t ps4_net_pool_id();

/// Query the console's current IPv4 address as a string (e.g. "192.168.1.42").
/// Returns false if the network is not connected.
bool ps4_net_query_ip_address(char* out_ip, size_t out_ip_size);

/// Query the console's MAC address (6 bytes).
bool ps4_net_query_mac_address(uint8_t out_mac[6]);

/// Query the ethernet/WiFi link status.  *out_link is non-zero if connected.
bool ps4_net_query_link_status(uint32_t* out_link);

/// Query primary and secondary DNS server addresses.
bool ps4_net_query_dns(char* out_primary, size_t primary_size,
                       char* out_secondary, size_t secondary_size);

/// Resolve a hostname to an IPv4 address (network byte order).
/// Returns 0 on success, negative SCE error code on failure.
int32_t ps4_net_resolve_hostname(const char* hostname, uint32_t* out_addr);

}  // namespace rex::net

#endif  // REX_PLATFORM_PS4
