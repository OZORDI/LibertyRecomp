/**
 * @file        core/net_switch.cpp
 * @brief       Nintendo Switch network stack initialization using libnx nifm / bsd
 *
 * Switch (Horizon OS) uses a two-layer networking model:
 *   1. nifm  -- Network Interface Module: manages WiFi/LAN connectivity state.
 *   2. bsd   -- BSD socket driver: provides POSIX socket(), bind(), connect(), etc.
 *
 * libnx's socketInitializeDefault() initializes the bsd service and also
 * enables DNS resolution (getaddrinfo / gethostbyname) automatically via
 * the sfdnsres service.  No additional DNS setup is required.
 *
 * After initialization, standard POSIX socket calls work identically to Linux.
 */

#include <rex/platform.h>

#if REX_PLATFORM_NX

#include <rex/logging.h>
#include <rex/net/net_switch.h>
#include <rex/net/platform_net.h>

#include <switch.h>

namespace rex::net {

static bool s_net_initialized = false;

bool switch_net_initialize() {
  if (s_net_initialized) {
    return true;
  }

  // Initialize the Network Interface Module to manage connectivity.
  Result rc = nifmInitialize(NifmServiceType_User);
  if (R_FAILED(rc)) {
    REXKRNL_ERROR("nifmInitialize() failed: 0x{:08X}", rc);
    return false;
  }

  // Check if we actually have an internet connection.
  NifmInternetConnectionStatus conn_status;
  rc = nifmGetInternetConnectionStatus(nullptr, nullptr, &conn_status);
  if (R_FAILED(rc)) {
    REXKRNL_WARN("nifmGetInternetConnectionStatus() failed: 0x{:08X} -- proceeding anyway", rc);
  } else if (conn_status != NifmInternetConnectionStatus_Connected) {
    REXKRNL_WARN("Switch network: not connected (status={})", static_cast<int>(conn_status));
    // Non-fatal: game may work offline or connect later.
  }

  // Initialize the BSD socket driver + DNS resolver (sfdnsres).
  // socketInitializeDefault() allocates a default-sized transfer memory buffer
  // and calls bsdInitialize + resolverInitialize internally.
  rc = socketInitializeDefault();
  if (R_FAILED(rc)) {
    REXKRNL_ERROR("socketInitializeDefault() failed: 0x{:08X}", rc);
    nifmExit();
    return false;
  }

  s_net_initialized = true;
  REXKRNL_INFO("Switch network stack initialized");
  return true;
}

void switch_net_shutdown() {
  if (!s_net_initialized) {
    return;
  }

  socketExit();
  nifmExit();

  s_net_initialized = false;
}

// Cross-platform entry points consumed by xam_net.
bool PlatformInit() { return switch_net_initialize(); }
void PlatformShutdown() { switch_net_shutdown(); }

bool switch_net_is_connected() {
  NifmInternetConnectionStatus conn_status;
  Result rc = nifmGetInternetConnectionStatus(nullptr, nullptr, &conn_status);
  if (R_FAILED(rc)) {
    return false;
  }
  return conn_status == NifmInternetConnectionStatus_Connected;
}

}  // namespace rex::net

#endif  // REX_PLATFORM_NX
