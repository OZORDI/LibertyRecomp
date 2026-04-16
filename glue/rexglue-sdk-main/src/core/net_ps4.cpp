/**
 * @file        core/net_ps4.cpp
 * @brief       PS4 network stack initialization using sceNet / sceNetCtl APIs
 *
 * PS4 (FreeBSD-based) supports standard BSD sockets, but the SCE network
 * subsystem must be initialized first via sceNetInit() and sceNetCtlInit().
 * A memory pool must also be created for internal libSceNet allocations.
 *
 * After initialization, standard socket(), bind(), connect(), send(), recv()
 * calls work identically to any other POSIX platform.
 */

#include <rex/platform.h>

#if REX_PLATFORM_PS4

#include <rex/assert.h>
#include <rex/logging.h>
#include <rex/net/platform_net.h>
#include <rex/net/socket.h>

#include <orbis/Net.h>
#include <orbis/NetCtl.h>
#include <orbis/Sysmodule.h>

namespace rex::net {

static bool s_net_initialized = false;
static int32_t s_net_pool_id = -1;

bool ps4_net_initialize() {
  if (s_net_initialized) {
    return true;
  }

  // Load the network sysmodule (required before any sceNet* call on PS4 apps
  // that aren't statically linked against libSceNet).
  int32_t ret = sceSysmoduleLoadModule(ORBIS_SYSMODULE_NET);
  if (ret < 0) {
    REXKRNL_ERROR("sceSysmoduleLoadModule(NET) failed: 0x{:08X}", static_cast<uint32_t>(ret));
    return false;
  }

  // Initialize the SCE network library.
  ret = sceNetInit();
  if (ret < 0) {
    REXKRNL_ERROR("sceNetInit() failed: 0x{:08X}", static_cast<uint32_t>(ret));
    return false;
  }

  // Create a memory pool for internal network allocations.
  // 16 KB is sufficient for typical game networking.
  constexpr int32_t kNetPoolSize = 16 * 1024;
  s_net_pool_id = sceNetPoolCreate("LibertyRecomp", kNetPoolSize, 0);
  if (s_net_pool_id < 0) {
    REXKRNL_ERROR("sceNetPoolCreate() failed: 0x{:08X}", static_cast<uint32_t>(s_net_pool_id));
    s_net_pool_id = -1;
    // Non-fatal: BSD sockets may still work without a pool.
  }

  // Initialize the network control library (needed to query IP, link status, etc.)
  ret = sceNetCtlInit();
  if (ret < 0) {
    REXKRNL_ERROR("sceNetCtlInit() failed: 0x{:08X}", static_cast<uint32_t>(ret));
    // Non-fatal: networking will work, but we can't query adapter info.
  }

  s_net_initialized = true;
  REXKRNL_INFO("PS4 network stack initialized (pool_id={})", s_net_pool_id);
  return true;
}

void ps4_net_shutdown() {
  if (!s_net_initialized) {
    return;
  }

  sceNetCtlTerm();

  if (s_net_pool_id >= 0) {
    sceNetPoolDestroy(s_net_pool_id);
    s_net_pool_id = -1;
  }

  // Note: sceNetTerm() is not exposed in OpenOrbis headers.
  // The OS cleans up on process exit.

  sceSysmoduleUnloadModule(ORBIS_SYSMODULE_NET);

  s_net_initialized = false;
}

// Cross-platform entry points consumed by xam_net.
bool PlatformInit() { return ps4_net_initialize(); }
void PlatformShutdown() { ps4_net_shutdown(); }

int32_t ps4_net_pool_id() {
  return s_net_pool_id;
}

bool ps4_net_query_ip_address(char* out_ip, size_t out_ip_size) {
  OrbisNetCtlInfo info;
  int32_t ret = sceNetCtlGetInfo(ORBIS_NET_CTL_INFO_IP_ADDRESS, &info);
  if (ret < 0) {
    return false;
  }
  // info.ip_address is a null-terminated string like "192.168.1.42"
  size_t len = strlen(info.ip_address);
  if (len >= out_ip_size) {
    return false;
  }
  memcpy(out_ip, info.ip_address, len + 1);
  return true;
}

bool ps4_net_query_mac_address(uint8_t out_mac[6]) {
  OrbisNetEtherAddr mac;
  int32_t ret = sceNetGetMacAddress(&mac, sizeof(mac));
  if (ret < 0) {
    // Fallback: try NetCtl
    OrbisNetCtlInfo info;
    ret = sceNetCtlGetInfo(ORBIS_NET_CTL_INFO_ETHER_ADDR, &info);
    if (ret < 0) {
      return false;
    }
    memcpy(out_mac, info.ether_addr, 6);
    return true;
  }
  memcpy(out_mac, mac.data, 6);
  return true;
}

bool ps4_net_query_link_status(uint32_t* out_link) {
  OrbisNetCtlInfo info;
  int32_t ret = sceNetCtlGetInfo(ORBIS_NET_CTL_INFO_LINK, &info);
  if (ret < 0) {
    *out_link = 0;
    return false;
  }
  *out_link = info.link;
  return true;
}

bool ps4_net_query_dns(char* out_primary, size_t primary_size,
                       char* out_secondary, size_t secondary_size) {
  OrbisNetCtlInfo info;
  int32_t ret = sceNetCtlGetInfo(ORBIS_NET_CTL_INFO_PRIMARY_DNS, &info);
  if (ret < 0) {
    return false;
  }
  if (out_primary && primary_size > 0) {
    size_t len = strlen(info.primary_dns);
    if (len < primary_size) {
      memcpy(out_primary, info.primary_dns, len + 1);
    }
  }

  ret = sceNetCtlGetInfo(ORBIS_NET_CTL_INFO_SECONDARY_DNS, &info);
  if (ret == 0 && out_secondary && secondary_size > 0) {
    size_t len = strlen(info.secondary_dns);
    if (len < secondary_size) {
      memcpy(out_secondary, info.secondary_dns, len + 1);
    }
  }

  return true;
}

int32_t ps4_net_resolve_hostname(const char* hostname, uint32_t* out_addr) {
  if (s_net_pool_id < 0) {
    return -1;
  }

  OrbisNetId resolver = sceNetResolverCreate("resolver", s_net_pool_id, 0);
  if (resolver < 0) {
    return resolver;
  }

  OrbisNetInAddr addr;
  int32_t ret = sceNetResolverStartNtoa(resolver, hostname, &addr, 5 /* timeout_sec */,
                                        3 /* retries */, 0 /* flags */);
  sceNetResolverDestroy(resolver);

  if (ret < 0) {
    return ret;
  }

  *out_addr = addr.s_addr;
  return 0;
}

}  // namespace rex::net

#endif  // REX_PLATFORM_PS4
