/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2022 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

// Disable warnings about unused parameters for kernel functions
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstring>
#include <limits>
#include <optional>

#if REX_PLATFORM_MAC
#include <sys/select.h>
#endif

#include <rex/chrono/clock.h>
#include <rex/kernel/xam/module.h>
#include <rex/kernel/xam/private.h>
#include <rex/kernel/xboxkrnl/error.h>
#include <rex/kernel/xboxkrnl/threading.h>
#include <rex/logging.h>
#include <rex/hook.h>
#include <rex/net/socket.h>
#include <rex/types.h>
#include <rex/string.h>
#include <rex/system/kernel_state.h>
#include <rex/system/xevent.h>
#include <rex/system/xsocket.h>
#include <rex/system/xthread.h>
#include <rex/system/xtypes.h>

#if REX_PLATFORM_WIN32
// NOTE: must be included last as it expects windows.h to already be included.
#define _WINSOCK_DEPRECATED_NO_WARNINGS  // inet_addr
#include <winsock2.h>                    // NOLINT(build/include_order)
#include <ws2tcpip.h>
#elif REX_PLATFORM_LINUX || REX_PLATFORM_MAC
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#endif

namespace rex {
namespace kernel {
namespace xam {
using namespace rex::system;
using namespace rex::system::xam;

// https://github.com/G91/TitanOffLine/blob/1e692d9bb9dfac386d08045ccdadf4ae3227bb5e/xkelib/xam/xamNet.h
enum {
  XNCALLER_INVALID = 0x0,
  XNCALLER_TITLE = 0x1,
  XNCALLER_SYSAPP = 0x2,
  XNCALLER_XBDM = 0x3,
  XNCALLER_TEST = 0x4,
  NUM_XNCALLER_TYPES = 0x4,
};

// https://github.com/pmrowla/hl2sdk-csgo/blob/master/common/xbox/xboxstubs.h
typedef struct {
  // FYI: IN_ADDR should be in network-byte order.
  in_addr ina;                    // IP address (zero if not static/DHCP)
  in_addr inaOnline;              // Online IP address (zero if not online)
  rex::be<uint16_t> wPortOnline;  // Online port
  uint8_t abEnet[6];              // Ethernet MAC address
  uint8_t abOnline[20];           // Online identification
} XNADDR;
static_assert_size(XNADDR, 0x24);

struct XNET_KEY_ID {
  std::array<uint8_t, 8> value{};
};
static_assert_size(XNET_KEY_ID, 0x8);

struct XNET_EXCHANGE_KEY {
  std::array<uint8_t, 16> value{};
};
static_assert_size(XNET_EXCHANGE_KEY, 0x10);

uint64_t ReadKeyId(const XNET_KEY_ID& id) {
  rex::be<uint64_t> value;
  std::memcpy(&value, id.value.data(), id.value.size());
  return value;
}

void WriteKeyId(uint64_t value, XNET_KEY_ID& id) {
  rex::be<uint64_t> encoded = value;
  std::memcpy(id.value.data(), &encoded, id.value.size());
}

typedef struct {
  rex::be<int32_t> status;
  rex::be<uint32_t> cina;
  in_addr aina[8];
} XNDNS;
static_assert_size(XNDNS, 0x28);

typedef struct {
  uint8_t flags;
  uint8_t reserved;
  rex::be<uint16_t> probes_xmit;
  rex::be<uint16_t> probes_recv;
  rex::be<uint16_t> data_len;
  rex::be<uint32_t> data_ptr;
  rex::be<uint16_t> rtt_min_in_msecs;
  rex::be<uint16_t> rtt_med_in_msecs;
  rex::be<uint32_t> up_bits_per_sec;
  rex::be<uint32_t> down_bits_per_sec;
} XNQOSINFO;
static_assert_size(XNQOSINFO, 0x18);

typedef struct {
  rex::be<uint32_t> count;
  rex::be<uint32_t> count_pending;
  XNQOSINFO info[1];
} XNQOS;
static_assert_size(XNQOS, 0x20);

enum XNetConnectStatus : uint32_t {
  XNET_CONNECT_STATUS_IDLE = 0,
  XNET_CONNECT_STATUS_PENDING = 1,
  XNET_CONNECT_STATUS_CONNECTED = 2,
  XNET_CONNECT_STATUS_LOST = 3,
};

enum XNetQosInfoFlags : uint8_t {
  XNET_QOS_INFO_COMPLETE = 0x01,
  XNET_QOS_INFO_TARGET_CONTACTED = 0x02,
};

constexpr uint32_t kMaximumQosTargets = 64;

struct Xsockaddr_t {
  rex::be<uint16_t> sa_family;
  char sa_data[14];
};

struct X_WSADATA {
  rex::be<uint16_t> version;
  rex::be<uint16_t> version_high;
  char description[256 + 1];
  char system_status[128 + 1];
  rex::be<uint16_t> max_sockets;
  rex::be<uint16_t> max_udpdg;
  rex::be<uint32_t> vendor_info_ptr;
};

struct XWSABUF {
  rex::be<uint32_t> len;
  rex::be<uint32_t> buf_ptr;
};

struct XWSAOVERLAPPED {
  rex::be<uint32_t> internal;
  rex::be<uint32_t> internal_high;
  union {
    struct {
      rex::be<uint32_t> low;
      rex::be<uint32_t> high;
    } offset;  // must be named to avoid GCC error
    rex::be<uint32_t> pointer;
  };
  rex::be<uint32_t> event_handle;
};

void LoadSockaddr(const uint8_t* ptr, sockaddr* out_addr) {
  out_addr->sa_family = memory::load_and_swap<uint16_t>(ptr + 0);
  switch (out_addr->sa_family) {
    case AF_INET: {
      auto in_addr = reinterpret_cast<sockaddr_in*>(out_addr);
      in_addr->sin_port = memory::load_and_swap<uint16_t>(ptr + 2);
      // Maybe? Depends on type.
      in_addr->sin_addr.s_addr = *(uint32_t*)(ptr + 4);
      break;
    }
    default:
      assert_unhandled_case(out_addr->sa_family);
      break;
  }
}

void StoreSockaddr(const sockaddr& addr, uint8_t* ptr) {
  switch (addr.sa_family) {
    case AF_UNSPEC:
      std::memset(ptr, 0, sizeof(addr));
      break;
    case AF_INET: {
      auto& in_addr = reinterpret_cast<const sockaddr_in&>(addr);
      memory::store_and_swap<uint16_t>(ptr + 0, in_addr.sin_family);
      memory::store_and_swap<uint16_t>(ptr + 2, in_addr.sin_port);
      // Maybe? Depends on type.
      memory::store_and_swap<uint32_t>(ptr + 4, in_addr.sin_addr.s_addr);
      break;
    }
    default:
      assert_unhandled_case(addr.sa_family);
      break;
  }
}

// https://github.com/joolswills/mameox/blob/master/MAMEoX/Sources/xbox_Network.cpp#L136
struct XNetStartupParams {
  uint8_t cfgSizeOfStruct;
  uint8_t cfgFlags;
  uint8_t cfgSockMaxDgramSockets;
  uint8_t cfgSockMaxStreamSockets;
  uint8_t cfgSockDefaultRecvBufsizeInK;
  uint8_t cfgSockDefaultSendBufsizeInK;
  uint8_t cfgKeyRegMax;
  uint8_t cfgSecRegMax;
  uint8_t cfgQosDataLimitDiv4;
  uint8_t cfgQosProbeTimeoutInSeconds;
  uint8_t cfgQosProbeRetries;
  uint8_t cfgQosSrvMaxSimultaneousResponses;
  uint8_t cfgQosPairWaitTimeInSeconds;
};

XNetStartupParams xnet_startup_params = {};

u32 XNetLogonGetMachineID_entry(mapped_u64 machine_id_ptr) {
  if (!machine_id_ptr) {
    return X_ERROR_INVALID_PARAMETER;
  }
  const auto* live = REX_KERNEL_STATE()->live_compatibility();
  if (!live || !live->available()) {
    *machine_id_ptr = 0;
    return 0x80151802;  // X_ERROR_LOGON_NOT_LOGGED_ON
  }
  *machine_id_ptr = live->identity().machine_id;
  return X_ERROR_SUCCESS;
}

u32 XNetLogonGetTitleID_entry(u32 caller, mapped_void params) {
  return REX_KERNEL_STATE()->title_id();
}

u32 NetDll_XNetStartup_entry(u32 caller, ppc_ptr_t<XNetStartupParams> params) {
  if (params) {
    assert_true(params->cfgSizeOfStruct == sizeof(XNetStartupParams));
    std::memcpy(&xnet_startup_params, params, sizeof(XNetStartupParams));
  }

  auto xam = REX_KERNEL_STATE()->GetKernelModule<XamModule>("xam.xex");

  /*
  if (!xam->xnet()) {
    auto xnet = new XNet(REX_KERNEL_STATE());
    xnet->Initialize();

    xam->set_xnet(xnet);
  }
  */

  return 0;
}

u32 NetDll_XNetCleanup_entry(u32 caller, mapped_void params) {
  auto xam = REX_KERNEL_STATE()->GetKernelModule<XamModule>("xam.xex");
  // auto xnet = xam->xnet();
  // xam->set_xnet(nullptr);

  // TODO: Shut down and delete.
  // delete xnet;

  return 0;
}

u32 NetDll_XNetGetOpt_entry(u32 one, u32 option_id, mapped_void buffer_ptr,
                            mapped_u32 buffer_size) {
  assert_true(one == 1);
  switch (option_id) {
    case 1:
      if (*buffer_size < sizeof(XNetStartupParams)) {
        *buffer_size = sizeof(XNetStartupParams);
        return 0x2738;  // WSAEMSGSIZE
      }
      std::memcpy(buffer_ptr, &xnet_startup_params, sizeof(XNetStartupParams));
      return 0;
    default:
      REXKRNL_ERROR("NetDll_XNetGetOpt: option {} unimplemented", option_id);
      return 0x2726;  // WSAEINVAL
  }
}

u32 NetDll_XNetRandom_entry(u32 caller, mapped_void buffer_ptr, u32 length) {
  if (!length || !buffer_ptr) {
    return X_ERROR_SUCCESS;
  }
  auto* live = REX_KERNEL_STATE()->live_compatibility();
  if (!live) {
    return X_ERROR_FUNCTION_FAILED;
  }
  live->FillRandomBytes(std::span<uint8_t>(buffer_ptr.as<uint8_t*>(), length));

  return X_ERROR_SUCCESS;
}

u32 XNetLogonGetNatType_entry() {
  const auto* live = REX_KERNEL_STATE()->live_compatibility();
  // Xbox 360 XONLINE_NAT_TYPE uses zero for unknown and one for open. LAN
  // routes require no NAT traversal, so only the ready LAN backend reports open.
  return live && live->available() ? 1 : 0;
}

u32 NetDll_WSAStartup_entry(u32 caller, u16 version, ppc_ptr_t<X_WSADATA> data_ptr) {
// TODO(benvanik): abstraction layer needed.
#if REX_PLATFORM_WIN32
  WSADATA wsaData;
  ZeroMemory(&wsaData, sizeof(WSADATA));
  int ret = WSAStartup(version, &wsaData);

  auto data_out = REX_KERNEL_MEMORY()->TranslateVirtual(data_ptr.guest_address());

  if (data_ptr) {
    data_ptr->version = wsaData.wVersion;
    data_ptr->version_high = wsaData.wHighVersion;
    std::memcpy(&data_ptr->description, wsaData.szDescription, 0x100);
    std::memcpy(&data_ptr->system_status, wsaData.szSystemStatus, 0x80);
    data_ptr->max_sockets = wsaData.iMaxSockets;
    data_ptr->max_udpdg = wsaData.iMaxUdpDg;

    // Some games (5841099F) want this value round-tripped - they'll compare if
    // it changes and bugcheck if it does.
    uint32_t vendor_ptr = memory::load_and_swap<uint32_t>(data_out + 0x190);
    memory::store_and_swap<uint32_t>(data_out + 0x190, vendor_ptr);
  }
#else
  int ret = 0;
  if (data_ptr) {
    // Guess these values!
    data_ptr->version = version;
    data_ptr->description[0] = '\0';
    data_ptr->system_status[0] = '\0';
    data_ptr->max_sockets = 100;
    data_ptr->max_udpdg = 1024;
  }
#endif

  // DEBUG
  /*
  auto xam = REX_KERNEL_STATE()->GetKernelModule<XamModule>("xam.xex");
  if (!xam->xnet()) {
    auto xnet = new XNet(REX_KERNEL_STATE());
    xnet->Initialize();

    xam->set_xnet(xnet);
  }
  */

  return ret;
}

u32 NetDll_WSACleanup_entry(u32 caller) {
  // This does nothing. Xenia needs WSA running.
  return 0;
}

u32 NetDll_WSAGetLastError_entry() {
  return XThread::GetLastError();
}

u32 NetDll_WSARecvFrom_entry(u32 caller, u32 socket, ppc_ptr_t<XWSABUF> buffers_ptr,
                             u32 buffer_count, mapped_u32 num_bytes_recv, mapped_u32 flags_ptr,
                             ppc_ptr_t<XSOCKADDR_IN> from_addr,
                             ppc_ptr_t<XWSAOVERLAPPED> overlapped_ptr,
                             mapped_void completion_routine_ptr) {
  if (overlapped_ptr) {
    // auto evt = REX_KERNEL_OBJECTS()->LookupObject<XEvent>(
    //    overlapped_ptr->event_handle);

    // if (evt) {
    //  //evt->Set(0, false);
    //}
  }

  // we're not going to be receiving packets any time soon
  // return error so we don't wait on that - Cancerous
  return -1;
}

// If the socket is a VDP socket, buffer 0 is the game data length, and buffer 1
// is the unencrypted game data.
u32 NetDll_WSASendTo_entry(u32 caller, u32 socket_handle, ppc_ptr_t<XWSABUF> buffers,
                           u32 num_buffers, mapped_u32 num_bytes_sent, u32 flags,
                           ppc_ptr_t<XSOCKADDR_IN> to_ptr, u32 to_len,
                           ppc_ptr_t<XWSAOVERLAPPED> overlapped, mapped_void completion_routine) {
  assert(!overlapped);
  assert(!completion_routine);

  auto socket = REX_KERNEL_OBJECTS()->LookupObject<XSocket>(socket_handle);
  if (!socket) {
    // WSAENOTSOCK
    XThread::SetLastError(0x2736);
    return -1;
  }

  // Our sockets implementation doesn't support multiple buffers, so we need
  // to combine the buffers the game has given us!
  std::vector<uint8_t> combined_buffer_mem;
  uint32_t combined_buffer_size = 0;
  uint32_t combined_buffer_offset = 0;
  for (uint32_t i = 0; i < num_buffers; i++) {
    combined_buffer_size += buffers[i].len;
    combined_buffer_mem.resize(combined_buffer_size);
    uint8_t* combined_buffer = combined_buffer_mem.data();

    std::memcpy(combined_buffer + combined_buffer_offset,
                REX_KERNEL_MEMORY()->TranslateVirtual(buffers[i].buf_ptr), buffers[i].len);
    combined_buffer_offset += buffers[i].len;
  }

  N_XSOCKADDR_IN native_to(to_ptr);
  socket->SendTo(combined_buffer_mem.data(), combined_buffer_size, flags, &native_to, to_len);

  // TODO: Instantly complete overlapped

  return 0;
}

u32 NetDll_WSAWaitForMultipleEvents_entry(u32 num_events, mapped_u32 events, u32 wait_all,
                                          u32 timeout, u32 alertable) {
  if (num_events > 64) {
    XThread::SetLastError(87);  // ERROR_INVALID_PARAMETER
    return ~0u;
  }

  uint64_t timeout_wait = (uint64_t)timeout;

  X_STATUS result = 0;
  do {
    result = xboxkrnl::xeNtWaitForMultipleObjectsEx(num_events, events, wait_all, 1, alertable,
                                                    timeout != -1 ? &timeout_wait : nullptr);
  } while (result == X_STATUS_ALERTED);

  if (XFAILED(result)) {
    uint32_t error = xboxkrnl::xeRtlNtStatusToDosError(result);
    XThread::SetLastError(error);
    return ~0u;
  }
  return 0;
}

u32 NetDll_WSACreateEvent_entry() {
  XEvent* ev = new XEvent(REX_KERNEL_STATE());
  ev->Initialize(true, false);
  return ev->handle();
}

u32 NetDll_WSACloseEvent_entry(u32 event_handle) {
  X_STATUS result = REX_KERNEL_OBJECTS()->ReleaseHandle(event_handle);
  if (XFAILED(result)) {
    uint32_t error = xboxkrnl::xeRtlNtStatusToDosError(result);
    XThread::SetLastError(error);
    return 0;
  }
  return 1;
}

u32 NetDll_WSAResetEvent_entry(u32 event_handle) {
  X_STATUS result = xboxkrnl::xeNtClearEvent(event_handle);
  if (XFAILED(result)) {
    uint32_t error = xboxkrnl::xeRtlNtStatusToDosError(result);
    XThread::SetLastError(error);
    return 0;
  }
  return 1;
}

u32 NetDll_WSASetEvent_entry(u32 event_handle) {
  X_STATUS result = xboxkrnl::xeNtSetEvent(event_handle, nullptr);
  if (XFAILED(result)) {
    uint32_t error = xboxkrnl::xeRtlNtStatusToDosError(result);
    XThread::SetLastError(error);
    return 0;
  }
  return 1;
}

struct XnAddrStatus {
  // Address acquisition is not yet complete
  static const uint32_t XNET_GET_XNADDR_PENDING = 0x00000000;
  // XNet is uninitialized or no debugger found
  static const uint32_t XNET_GET_XNADDR_NONE = 0x00000001;
  // Host has ethernet address (no IP address)
  static const uint32_t XNET_GET_XNADDR_ETHERNET = 0x00000002;
  // Host has statically assigned IP address
  static const uint32_t XNET_GET_XNADDR_STATIC = 0x00000004;
  // Host has DHCP assigned IP address
  static const uint32_t XNET_GET_XNADDR_DHCP = 0x00000008;
  // Host has PPPoE assigned IP address
  static const uint32_t XNET_GET_XNADDR_PPPOE = 0x00000010;
  // Host has one or more gateways configured
  static const uint32_t XNET_GET_XNADDR_GATEWAY = 0x00000020;
  // Host has one or more DNS servers configured
  static const uint32_t XNET_GET_XNADDR_DNS = 0x00000040;
  // Host is currently connected to online service
  static const uint32_t XNET_GET_XNADDR_ONLINE = 0x00000080;
  // Network configuration requires troubleshooting
  static const uint32_t XNET_GET_XNADDR_TROUBLESHOOT = 0x00008000;
};

u32 NetDll_XNetGetTitleXnAddr_entry(u32 caller, ppc_ptr_t<XNADDR> addr_ptr) {
  if (!addr_ptr) {
    return XnAddrStatus::XNET_GET_XNADDR_NONE;
  }
  addr_ptr.Zero();
  const auto* live = REX_KERNEL_STATE()->live_compatibility();
  if (!live || !live->available()) {
    return XnAddrStatus::XNET_GET_XNADDR_NONE;
  }

  addr_ptr->ina.s_addr = live->local_ipv4();
  addr_ptr->inaOnline.s_addr = live->local_ipv4();
  addr_ptr->wPortOnline = live->online_port();
  std::memcpy(addr_ptr->abEnet, live->identity().ethernet_address.data(),
              live->identity().ethernet_address.size());
  rex::be<uint64_t> machine_id = live->identity().machine_id;
  std::memcpy(addr_ptr->abOnline, &machine_id, sizeof(machine_id));

  return XnAddrStatus::XNET_GET_XNADDR_STATIC | XnAddrStatus::XNET_GET_XNADDR_GATEWAY |
         XnAddrStatus::XNET_GET_XNADDR_DNS | XnAddrStatus::XNET_GET_XNADDR_ONLINE;
}

u32 NetDll_XNetGetDebugXnAddr_entry(u32 caller, ppc_ptr_t<XNADDR> addr_ptr) {
  addr_ptr.Zero();

  // XNET_GET_XNADDR_NONE causes caller to gracefully return.
  return XnAddrStatus::XNET_GET_XNADDR_NONE;
}

u32 NetDll_XNetXnAddrToMachineId_entry(u32 caller, ppc_ptr_t<XNADDR> addr_ptr, mapped_u64 id_ptr) {
  if (!addr_ptr || !id_ptr) {
    return 0x2726;  // WSAEINVAL
  }
  rex::be<uint64_t> machine_id;
  std::memcpy(&machine_id, addr_ptr->abOnline, sizeof(machine_id));
  *id_ptr = machine_id;
  return X_ERROR_SUCCESS;
}

u32 NetDll_XNetInAddrToString_entry(u32 caller, u32 guest_in_addr, mapped_string string_out,
                                    u32 string_size) {
  if (!string_out || !string_size) {
    return 0x2726;  // WSAEINVAL
  }
  in_addr address{.s_addr = htonl(guest_in_addr)};
  std::array<char, INET_ADDRSTRLEN> formatted{};
  if (!inet_ntop(AF_INET, &address, formatted.data(), formatted.size())) {
    return 0x2726;  // WSAEINVAL
  }
  rex::string::copy_truncating(string_out, formatted.data(), string_size);
  return X_ERROR_SUCCESS;
}

// This converts a XNet address to an IN_ADDR. The IN_ADDR is used for
// subsequent socket calls (like a handle to a XNet address)
u32 NetDll_XNetXnAddrToInAddr_entry(u32 caller, ppc_ptr_t<XNADDR> xn_addr, mapped_void xid,
                                    mapped_void in_addr) {
  if (!xn_addr || !xid || !in_addr) {
    return 0x2726;  // WSAEINVAL
  }
  auto* live = REX_KERNEL_STATE()->live_compatibility();
  if (!live || !live->available()) {
    return 0x276D;  // WSANOTINITIALISED
  }

  const uint32_t address =
      live->config().backend == LiveBackend::kLan ? xn_addr->ina.s_addr : xn_addr->inaOnline.s_addr;
  if (!address) {
    return 0x2726;  // WSAEINVAL
  }
  *in_addr.as<uint32_t*>() = address;

  SessionRecord route;
  XNET_KEY_ID key_id;
  std::memcpy(&key_id, xid, sizeof(key_id));
  route.session_id = ReadKeyId(key_id);
  route.host_ipv4 = address;
  route.host_port = xn_addr->wPortOnline;
  std::memcpy(route.host_ethernet_address.data(), xn_addr->abEnet,
              route.host_ethernet_address.size());
  rex::be<uint64_t> machine_id;
  std::memcpy(&machine_id, xn_addr->abOnline, sizeof(machine_id));
  route.host_machine_id = machine_id;
  live->RegisterRoute(address, route);
  return X_ERROR_SUCCESS;
}

// Does the reverse of the above.
// FIXME: Arguments may not be correct.
u32 NetDll_XNetInAddrToXnAddr_entry(u32 caller, u32 guest_in_addr, ppc_ptr_t<XNADDR> xn_addr,
                                    ppc_ptr_t<XNET_KEY_ID> xid) {
  if (!xn_addr) {
    return 0x2726;  // WSAEINVAL
  }
  xn_addr.Zero();
  if (xid) {
    xid.Zero();
  }

  auto* live = REX_KERNEL_STATE()->live_compatibility();
  if (!live || !live->available()) {
    return 0x276D;  // WSANOTINITIALISED
  }
  const uint32_t address = htonl(guest_in_addr);
  const auto route = live->FindRoute(address);
  if (!route) {
    return 0x2726;  // WSAEINVAL
  }
  xn_addr->ina.s_addr = route->host_ipv4;
  xn_addr->inaOnline.s_addr = route->host_ipv4;
  xn_addr->wPortOnline = route->host_port;
  std::memcpy(xn_addr->abEnet, route->host_ethernet_address.data(),
              route->host_ethernet_address.size());
  rex::be<uint64_t> machine_id = route->host_machine_id;
  std::memcpy(xn_addr->abOnline, &machine_id, sizeof(machine_id));
  if (xid) {
    WriteKeyId(route->session_id, *xid);
  }
  return X_ERROR_SUCCESS;
}

// https://www.google.com/patents/WO2008112448A1?cl=en
// Reserves a port for use by system link
u32 NetDll_XNetSetSystemLinkPort_entry(u32 caller, u32 port) {
  auto* live = REX_KERNEL_STATE()->live_compatibility();
  if (!live || !live->available() || port > std::numeric_limits<uint16_t>::max()) {
    return 0x2726;  // WSAEINVAL
  }
  live->ObserveBoundPort(static_cast<uint16_t>(port));
  return X_ERROR_SUCCESS;
}

u32 NetDll_XNetGetSystemLinkPort_entry(u32 caller, mapped_u16 port) {
  if (!port) {
    return 0x2726;  // WSAEINVAL
  }
  const auto* live = REX_KERNEL_STATE()->live_compatibility();
  if (!live || !live->available()) {
    return 0x276D;  // WSANOTINITIALISED
  }
  *port = live->online_port();
  return X_ERROR_SUCCESS;
}

u32 NetDll_XNetCreateKey_entry(u32 caller, ppc_ptr_t<XNET_KEY_ID> session_key,
                               ppc_ptr_t<XNET_EXCHANGE_KEY> exchange_key) {
  if (!session_key || !exchange_key) {
    return 0x2726;  // WSAEINVAL
  }
  auto* live = REX_KERNEL_STATE()->live_compatibility();
  if (!live || !live->available()) {
    session_key.Zero();
    exchange_key.Zero();
    return 0x276D;  // WSANOTINITIALISED
  }

  WriteKeyId(live->GenerateSessionId(), *session_key);
  live->GenerateExchangeKey(exchange_key->value);
  return X_ERROR_SUCCESS;
}

u32 NetDll_XNetRegisterKey_entry(u32 caller, ppc_ptr_t<XNET_KEY_ID> session_key,
                                 ppc_ptr_t<XNET_EXCHANGE_KEY> exchange_key) {
  if (!session_key || !exchange_key) {
    return 0x2726;  // WSAEINVAL
  }
  auto* live = REX_KERNEL_STATE()->live_compatibility();
  if (!live || !live->available()) {
    return 0x276D;  // WSANOTINITIALISED
  }
  const uint64_t session_id = ReadKeyId(*session_key);
  if (!session_id) {
    return 0x2726;  // WSAEINVAL
  }
  live->RegisterKey(session_id, exchange_key->value);
  return X_ERROR_SUCCESS;
}

u32 NetDll_XNetUnregisterKey_entry(u32 caller, ppc_ptr_t<XNET_KEY_ID> session_key) {
  if (!session_key) {
    return 0x2726;  // WSAEINVAL
  }
  auto* live = REX_KERNEL_STATE()->live_compatibility();
  if (!live || !live->available()) {
    return 0x276D;  // WSANOTINITIALISED
  }
  live->UnregisterKey(ReadKeyId(*session_key));
  return X_ERROR_SUCCESS;
}

u32 NetDll_XNetUnregisterInAddr_entry(u32 caller, u32 guest_in_addr) {
  auto* live = REX_KERNEL_STATE()->live_compatibility();
  if (!live || !live->available()) {
    return 0x276D;  // WSANOTINITIALISED
  }
  live->UnregisterRoute(htonl(guest_in_addr));
  return X_ERROR_SUCCESS;
}

u32 NetDll_XNetConnect_entry(u32 caller, u32 guest_in_addr) {
  const auto* live = REX_KERNEL_STATE()->live_compatibility();
  if (!live || !live->available()) {
    return 0x276D;  // WSANOTINITIALISED
  }
  return live->FindRoute(htonl(guest_in_addr)) ? X_ERROR_SUCCESS : 0x2726;  // WSAEINVAL
}

u32 NetDll_XNetGetConnectStatus_entry(u32 caller, u32 guest_in_addr) {
  const auto* live = REX_KERNEL_STATE()->live_compatibility();
  if (!live || !live->available()) {
    return XNET_CONNECT_STATUS_LOST;
  }
  return live->FindRoute(htonl(guest_in_addr)) ? XNET_CONNECT_STATUS_CONNECTED
                                               : XNET_CONNECT_STATUS_IDLE;
}

u32 NetDll_XNetServerToInAddr_entry(u32 caller, u32 server_addr, u32 service_id,
                                    ppc_ptr_t<in_addr> in_addr_ptr) {
  const auto* live = REX_KERNEL_STATE()->live_compatibility();
  if (!live || !live->available()) {
    return 0x276D;  // WSANOTINITIALISED
  }
  if (!server_addr || !service_id || !in_addr_ptr) {
    return 0x2726;  // WSAEINVAL
  }
  in_addr_ptr->s_addr = htonl(server_addr);
  return X_ERROR_SUCCESS;
}

// https://github.com/ILOVEPIE/Cxbx-Reloaded/blob/master/src/CxbxKrnl/EmuXOnline.h#L39
struct XEthernetStatus {
  static const uint32_t XNET_ETHERNET_LINK_ACTIVE = 0x01;
  static const uint32_t XNET_ETHERNET_LINK_100MBPS = 0x02;
  static const uint32_t XNET_ETHERNET_LINK_10MBPS = 0x04;
  static const uint32_t XNET_ETHERNET_LINK_FULL_DUPLEX = 0x08;
  static const uint32_t XNET_ETHERNET_LINK_HALF_DUPLEX = 0x10;
};

u32 NetDll_XNetGetEthernetLinkStatus_entry(u32 caller) {
  const auto* live = REX_KERNEL_STATE()->live_compatibility();
  if (!live || !live->available()) {
    return 0;
  }
  return XEthernetStatus::XNET_ETHERNET_LINK_ACTIVE | XEthernetStatus::XNET_ETHERNET_LINK_100MBPS |
         XEthernetStatus::XNET_ETHERNET_LINK_FULL_DUPLEX;
}

u32 NetDll_XNetDnsLookup_entry(u32 caller, mapped_string host, u32 event_handle, mapped_u32 pdns) {
  if (!pdns || !host || host.value().empty()) {
    return 0x2726;  // WSAEINVAL
  }
  *pdns = 0;

  const uint32_t dns_guest = REX_KERNEL_MEMORY()->SystemHeapAlloc(sizeof(XNDNS));
  if (!dns_guest) {
    return 0x2747;  // WSAENOBUFS
  }
  auto* dns = REX_KERNEL_MEMORY()->TranslateVirtual<XNDNS*>(dns_guest);
  std::memset(dns, 0, sizeof(*dns));
  *pdns = dns_guest;

  addrinfo hints{};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  addrinfo* addresses = nullptr;
  const int lookup_result = getaddrinfo(host.value().data(), nullptr, &hints, &addresses);
  if (lookup_result != 0) {
    dns->status = 0x2AF9;  // WSAHOST_NOT_FOUND
  } else {
    uint32_t count = 0;
    for (auto* current = addresses; current && count < std::size(dns->aina);
         current = current->ai_next) {
      if (current->ai_family != AF_INET || !current->ai_addr) {
        continue;
      }
      const auto* address = reinterpret_cast<const sockaddr_in*>(current->ai_addr);
      dns->aina[count] = address->sin_addr;
      ++count;
    }
    dns->cina = count;
    dns->status = count ? X_ERROR_SUCCESS : 0x2AF9;  // WSAHOST_NOT_FOUND
    freeaddrinfo(addresses);
  }

  if (event_handle) {
    auto ev = REX_KERNEL_OBJECTS()->LookupObject<XEvent>(event_handle);
    if (ev) {
      ev->Set(0, false);
    }
  }
  return X_ERROR_SUCCESS;
}

u32 NetDll_XNetDnsRelease_entry(u32 caller, ppc_ptr_t<XNDNS> dns) {
  if (!dns) {
    return X_STATUS_INVALID_PARAMETER;
  }
  REX_KERNEL_MEMORY()->SystemHeapFree(dns.guest_address());
  return 0;
}

u32 NetDll_XNetQosServiceLookup_entry(u32 caller, u32 flags, u32 event_handle, mapped_u32 pqos) {
  // Set pqos as some games will try accessing it despite non-successful result
  if (pqos) {
    auto qos_guest = REX_KERNEL_MEMORY()->SystemHeapAlloc(sizeof(XNQOS));
    if (!qos_guest) {
      *pqos = 0;
      return 0x2747;  // WSAENOBUFS
    }
    auto qos = REX_KERNEL_MEMORY()->TranslateVirtual<XNQOS*>(qos_guest);
    qos->count = qos->count_pending = 0;
    *pqos = qos_guest;
  }
  if (event_handle) {
    auto ev = REX_KERNEL_OBJECTS()->LookupObject<XEvent>(event_handle);
    assert_not_null(ev);
    ev->Set(0, false);
  }
  return 0;
}

u32 NetDll_XNetQosRelease_entry(u32 caller, ppc_ptr_t<XNQOS> qos) {
  if (!qos) {
    return X_STATUS_INVALID_PARAMETER;
  }
  REX_KERNEL_MEMORY()->SystemHeapFree(qos.guest_address());
  return 0;
}

u32 NetDll_XNetQosListen_entry(u32 caller, mapped_void id, mapped_void data, u32 data_size, u32 r7,
                               u32 flags) {
  const auto* live = REX_KERNEL_STATE()->live_compatibility();
  return live && live->available() ? X_ERROR_SUCCESS : 0x276D;  // WSANOTINITIALISED
}

u32 NetDll_XNetQosLookup_entry(u32 caller, u32 remote_console_count,
                               mapped_void remote_address_ptrs, mapped_void session_id_ptrs,
                               mapped_void remote_key_ptrs, u32 gateway_count, mapped_void gateways,
                               mapped_void service_ids, u32 probe_count, u32 bits_per_second,
                               u32 flags, u32 event_handle, mapped_u32 qos_out) {
  if (!qos_out) {
    return 0x2722;  // WSAEACCES
  }
  *qos_out = 0;

  auto* live = REX_KERNEL_STATE()->live_compatibility();
  if (!live || !live->available()) {
    return 0x276D;  // WSANOTINITIALISED
  }

  const uint64_t total_count_wide = static_cast<uint64_t>(remote_console_count) + gateway_count;
  if (total_count_wide > kMaximumQosTargets) {
    return 0x2726;  // WSAEINVAL
  }
  const uint32_t total_count = static_cast<uint32_t>(total_count_wide);
  if (remote_console_count && (!remote_address_ptrs || !session_id_ptrs || !remote_key_ptrs)) {
    return 0x2726;  // WSAEINVAL
  }
  if (gateway_count && (!gateways || !service_ids)) {
    return 0x2726;  // WSAEINVAL
  }

  auto* remote_addresses = remote_address_ptrs.as<rex::be<uint32_t>*>();
  auto* session_ids = session_id_ptrs.as<rex::be<uint32_t>*>();
  auto* remote_keys = remote_key_ptrs.as<rex::be<uint32_t>*>();
  for (uint32_t index = 0; index < remote_console_count; ++index) {
    auto* address = REX_KERNEL_MEMORY()->TranslateVirtual<XNADDR*>(remote_addresses[index]);
    auto* session_id = REX_KERNEL_MEMORY()->TranslateVirtual<XNET_KEY_ID*>(session_ids[index]);
    auto* remote_key =
        REX_KERNEL_MEMORY()->TranslateVirtual<XNET_EXCHANGE_KEY*>(remote_keys[index]);
    if (!address || !session_id || !remote_key) {
      return 0x2726;  // WSAEINVAL
    }

    SessionRecord route;
    route.session_id = ReadKeyId(*session_id);
    route.exchange_key = remote_key->value;
    route.host_ipv4 = live->config().backend == LiveBackend::kLan ? address->ina.s_addr
                                                                  : address->inaOnline.s_addr;
    route.host_port = address->wPortOnline;
    std::memcpy(route.host_ethernet_address.data(), address->abEnet,
                route.host_ethernet_address.size());
    rex::be<uint64_t> machine_id;
    std::memcpy(&machine_id, address->abOnline, sizeof(machine_id));
    route.host_machine_id = machine_id;
    if (route.host_ipv4) {
      live->RegisterRoute(route.host_ipv4, route);
    }
    if (route.session_id) {
      live->RegisterKey(route.session_id, remote_key->value);
    }
  }

  const size_t allocation_size =
      offsetof(XNQOS, info) + sizeof(XNQOSINFO) * static_cast<size_t>(total_count);
  if (allocation_size > std::numeric_limits<uint32_t>::max()) {
    return 0x2747;  // WSAENOBUFS
  }
  const uint32_t qos_address =
      REX_KERNEL_MEMORY()->SystemHeapAlloc(static_cast<uint32_t>(allocation_size));
  if (!qos_address) {
    return 0x2747;  // WSAENOBUFS
  }
  auto* qos = REX_KERNEL_MEMORY()->TranslateVirtual<XNQOS*>(qos_address);
  std::memset(qos, 0, allocation_size);
  qos->count = total_count;
  qos->count_pending = 0;

  const uint16_t completed_probes = static_cast<uint16_t>(
      std::min(probe_count, static_cast<uint32_t>(std::numeric_limits<uint16_t>::max())));
  const uint32_t reported_bandwidth = bits_per_second ? bits_per_second : 1000000;
  for (uint32_t index = 0; index < total_count; ++index) {
    auto& info = qos->info[index];
    info.flags = XNET_QOS_INFO_COMPLETE | XNET_QOS_INFO_TARGET_CONTACTED;
    info.probes_xmit = completed_probes;
    info.probes_recv = completed_probes;
    info.rtt_min_in_msecs = 10;
    info.rtt_med_in_msecs = 10;
    info.up_bits_per_sec = reported_bandwidth;
    info.down_bits_per_sec = reported_bandwidth;
  }
  *qos_out = qos_address;

  if (event_handle) {
    auto event = REX_KERNEL_OBJECTS()->LookupObject<XEvent>(event_handle);
    if (event) {
      event->Set(0, false);
    }
  }
  return X_ERROR_SUCCESS;
}

u32 NetDll_inet_addr_entry(mapped_string addr_ptr) {
  if (!addr_ptr) {
    return -1;
  }

  uint32_t addr = inet_addr(addr_ptr);
  // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-inet_addr#return-value
  // Based on console research it seems like x360 uses old version of inet_addr
  // In case of empty string it return 0 instead of -1
  if (addr == -1 && !addr_ptr.value().length()) {
    return 0;
  }

  return rex::byte_swap(addr);
}

u32 NetDll_socket_entry(u32 caller, u32 af, u32 type, u32 protocol) {
  XSocket* socket = new XSocket(REX_KERNEL_STATE());
  X_STATUS result =
      socket->Initialize(XSocket::AddressFamily((uint32_t)af), XSocket::Type((uint32_t)type),
                         XSocket::Protocol((uint32_t)protocol));

  if (XFAILED(result)) {
    const uint32_t error = rex::net::socket_last_error();
    socket->Release();
    XThread::SetLastError(error);
    return -1;
  }

  return socket->handle();
}

u32 NetDll_closesocket_entry(u32 caller, u32 socket_handle) {
  auto socket = REX_KERNEL_OBJECTS()->LookupObject<XSocket>(socket_handle);
  if (!socket) {
    // WSAENOTSOCK
    XThread::SetLastError(0x2736);
    return -1;
  }

  const X_STATUS status = socket->Close();
  if (XFAILED(status)) {
    XThread::SetLastError(rex::net::socket_last_error());
    return -1;
  }
  socket->ReleaseHandle();
  return 0;
}

i32 NetDll_shutdown_entry(u32 caller, u32 socket_handle, i32 how) {
  auto socket = REX_KERNEL_OBJECTS()->LookupObject<XSocket>(socket_handle);
  if (!socket) {
    // WSAENOTSOCK
    XThread::SetLastError(0x2736);
    return -1;
  }

  auto ret = socket->Shutdown(how);
  if (ret == -1) {
    XThread::SetLastError(rex::net::socket_last_error());
  }
  return ret;
}

u32 NetDll_setsockopt_entry(u32 caller, u32 socket_handle, u32 level, u32 optname,
                            mapped_void optval_ptr, u32 optlen) {
  auto socket = REX_KERNEL_OBJECTS()->LookupObject<XSocket>(socket_handle);
  if (!socket) {
    // WSAENOTSOCK
    XThread::SetLastError(0x2736);
    return -1;
  }

  X_STATUS status = socket->SetOption(level, optname, optval_ptr, optlen);
  if (XFAILED(status)) {
    XThread::SetLastError(rex::net::socket_last_error());
    return -1;
  }
  return 0;
}

u32 NetDll_ioctlsocket_entry(u32 caller, u32 socket_handle, u32 cmd, mapped_void arg_ptr) {
  auto socket = REX_KERNEL_OBJECTS()->LookupObject<XSocket>(socket_handle);
  if (!socket) {
    // WSAENOTSOCK
    XThread::SetLastError(0x2736);
    return -1;
  }

  X_STATUS status = socket->IOControl(cmd, arg_ptr);
  if (XFAILED(status)) {
    XThread::SetLastError(rex::net::socket_last_error());
    return -1;
  }

  // TODO
  return 0;
}

u32 NetDll_bind_entry(u32 caller, u32 socket_handle, ppc_ptr_t<XSOCKADDR_IN> name, u32 namelen) {
  auto socket = REX_KERNEL_OBJECTS()->LookupObject<XSocket>(socket_handle);
  if (!socket) {
    // WSAENOTSOCK
    XThread::SetLastError(0x2736);
    return -1;
  }

  N_XSOCKADDR_IN native_name(name);
  X_STATUS status = socket->Bind(&native_name, namelen);
  if (XFAILED(status)) {
    XThread::SetLastError(rex::net::socket_last_error());
    return -1;
  }

  auto* live = REX_KERNEL_STATE()->live_compatibility();
  if (live && live->available() && socket->socket_type() == XSocket::X_SOCK_DGRAM &&
      (socket->protocol() == XSocket::X_IPPROTO_UDP ||
       socket->protocol() == XSocket::X_IPPROTO_VDP)) {
    live->ObserveBoundPort(socket->bound_port());
  }

  return 0;
}

u32 NetDll_connect_entry(u32 caller, u32 socket_handle, ppc_ptr_t<XSOCKADDR> name, u32 namelen) {
  auto socket = REX_KERNEL_OBJECTS()->LookupObject<XSocket>(socket_handle);
  if (!socket) {
    // WSAENOTSOCK
    XThread::SetLastError(0x2736);
    return -1;
  }

  N_XSOCKADDR native_name(name);
  X_STATUS status = socket->Connect(&native_name, namelen);
  if (XFAILED(status)) {
    XThread::SetLastError(rex::net::socket_last_error());
    return -1;
  }

  return 0;
}

u32 NetDll_listen_entry(u32 caller, u32 socket_handle, i32 backlog) {
  auto socket = REX_KERNEL_OBJECTS()->LookupObject<XSocket>(socket_handle);
  if (!socket) {
    // WSAENOTSOCK
    XThread::SetLastError(0x2736);
    return -1;
  }

  X_STATUS status = socket->Listen(backlog);
  if (XFAILED(status)) {
    XThread::SetLastError(rex::net::socket_last_error());
    return -1;
  }

  return 0;
}

u32 NetDll_accept_entry(u32 caller, u32 socket_handle, ppc_ptr_t<XSOCKADDR> addr_ptr,
                        mapped_u32 addrlen_ptr) {
  if (!addr_ptr) {
    // WSAEFAULT
    XThread::SetLastError(0x271E);
    return -1;
  }

  auto socket = REX_KERNEL_OBJECTS()->LookupObject<XSocket>(socket_handle);
  if (!socket) {
    // WSAENOTSOCK
    XThread::SetLastError(0x2736);
    return -1;
  }

  N_XSOCKADDR native_addr(addr_ptr);
  int native_len = *addrlen_ptr;
  auto new_socket = socket->Accept(&native_addr, &native_len);
  if (new_socket) {
    addr_ptr->address_family = native_addr.address_family;
    std::memcpy(addr_ptr->sa_data, native_addr.sa_data, *addrlen_ptr - 2);
    *addrlen_ptr = native_len;

    return new_socket->handle();
  } else {
    XThread::SetLastError(rex::net::socket_last_error());
    return -1;
  }
}

struct x_fd_set {
  rex::be<uint32_t> fd_count;
  rex::be<uint32_t> fd_array[64];
};

struct host_set {
  uint32_t count;
  object_ref<XSocket> sockets[64];

  void Load(const x_fd_set* guest_set) {
    assert_true(guest_set->fd_count < 64);
    this->count = guest_set->fd_count;
    for (uint32_t i = 0; i < this->count; ++i) {
      auto socket_handle = static_cast<X_HANDLE>(guest_set->fd_array[i]);
      if (socket_handle == -1) {
        this->count = i;
        break;
      }
      // Convert from Xenia -> native
      auto socket = REX_KERNEL_OBJECTS()->LookupObject<XSocket>(socket_handle);
      assert_not_null(socket);
      this->sockets[i] = socket;
    }
  }

  void Store(x_fd_set* guest_set) {
    guest_set->fd_count = 0;
    for (uint32_t i = 0; i < this->count; ++i) {
      auto socket = this->sockets[i];
      guest_set->fd_array[guest_set->fd_count++] = socket->handle();
    }
  }

  void Store(fd_set* native_set) {
    FD_ZERO(native_set);
    for (uint32_t i = 0; i < this->count; ++i) {
      FD_SET(this->sockets[i]->native_handle(), native_set);
    }
  }

  int NativeNfds() const {
#if REX_PLATFORM_WIN32
    // WinSock ignores select's nfds argument.
    return 0;
#else
    int native_nfds = 0;
    for (uint32_t i = 0; i < this->count; ++i) {
      const int native_handle = static_cast<int>(this->sockets[i]->native_handle());
      native_nfds = std::max(native_nfds, native_handle + 1);
    }
    return native_nfds;
#endif
  }

  uint32_t UpdateFrom(fd_set* native_set) {
    uint32_t new_count = 0;
    for (uint32_t i = 0; i < this->count; ++i) {
      auto socket = this->sockets[i];
      if (FD_ISSET(socket->native_handle(), native_set)) {
        this->sockets[new_count++] = socket;
      }
    }
    this->count = new_count;
    return new_count;
  }

  bool UsesPeerDatagramTransport() const {
    for (uint32_t i = 0; i < this->count; ++i) {
      if (this->sockets[i]->UsesPeerDatagramTransport()) {
        return true;
      }
    }
    return false;
  }

  bool HasPendingPeerDatagram() const {
    for (uint32_t i = 0; i < this->count; ++i) {
      if (this->sockets[i]->HasPendingPeerDatagram()) {
        return true;
      }
    }
    return false;
  }

  uint32_t UpdateReadFrom(fd_set* native_set) {
    uint32_t new_count = 0;
    for (uint32_t i = 0; i < this->count; ++i) {
      auto socket = this->sockets[i];
      if (FD_ISSET(socket->native_handle(), native_set) || socket->HasPendingPeerDatagram()) {
        this->sockets[new_count++] = socket;
      }
    }
    this->count = new_count;
    return new_count;
  }
};

i32 NetDll_select_entry(i32 caller, i32 nfds, ppc_ptr_t<x_fd_set> readfds,
                        ppc_ptr_t<x_fd_set> writefds, ppc_ptr_t<x_fd_set> exceptfds,
                        mapped_void timeout_ptr) {
  host_set host_readfds = {};
  fd_set native_readfds = {};
  if (readfds) {
    host_readfds.Load(readfds);
    host_readfds.Store(&native_readfds);
  }
  host_set host_writefds = {};
  fd_set native_writefds = {};
  if (writefds) {
    host_writefds.Load(writefds);
    host_writefds.Store(&native_writefds);
  }
  host_set host_exceptfds = {};
  fd_set native_exceptfds = {};
  if (exceptfds) {
    host_exceptfds.Load(exceptfds);
    host_exceptfds.Store(&native_exceptfds);
  }
  timeval* timeout_in = nullptr;
  timeval timeout;
  if (timeout_ptr) {
    timeout = {static_cast<int32_t>(timeout_ptr.as_array<int32_t>()[0]),
               static_cast<int32_t>(timeout_ptr.as_array<int32_t>()[1])};
    chrono::Clock::ScaleGuestDurationTimeval(reinterpret_cast<int32_t*>(&timeout.tv_sec),
                                             reinterpret_cast<int32_t*>(&timeout.tv_usec));
    timeout_in = &timeout;
  }
  constexpr auto kPeerTransportPollInterval = std::chrono::milliseconds(10);
  std::optional<std::chrono::steady_clock::time_point> deadline;
  if (timeout_in) {
    deadline = std::chrono::steady_clock::now() + std::chrono::seconds(timeout.tv_sec) +
               std::chrono::microseconds(timeout.tv_usec);
  }
  const bool poll_peer_transport = readfds && host_readfds.UsesPeerDatagramTransport();
  const int native_nfds = std::max(
      {host_readfds.NativeNfds(), host_writefds.NativeNfds(), host_exceptfds.NativeNfds()});

  int ret = 0;
  while (true) {
    if (readfds) {
      host_readfds.Store(&native_readfds);
    }
    if (writefds) {
      host_writefds.Store(&native_writefds);
    }
    if (exceptfds) {
      host_exceptfds.Store(&native_exceptfds);
    }

    const bool peer_ready = poll_peer_transport && host_readfds.HasPendingPeerDatagram();
    timeval wait_time{};
    timeval* wait_time_ptr = nullptr;
    if (peer_ready) {
      wait_time_ptr = &wait_time;
    } else if (deadline) {
      const auto now = std::chrono::steady_clock::now();
      auto remaining = now < *deadline
                           ? std::chrono::duration_cast<std::chrono::microseconds>(*deadline - now)
                           : std::chrono::microseconds::zero();
      if (poll_peer_transport) {
        remaining = std::min(remaining, std::chrono::duration_cast<std::chrono::microseconds>(
                                            kPeerTransportPollInterval));
      }
      const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(remaining);
      const auto microseconds =
          std::chrono::duration_cast<std::chrono::microseconds>(remaining - seconds);
      wait_time.tv_sec = static_cast<decltype(wait_time.tv_sec)>(seconds.count());
      wait_time.tv_usec = static_cast<decltype(wait_time.tv_usec)>(microseconds.count());
      wait_time_ptr = &wait_time;
    } else if (poll_peer_transport) {
      const auto seconds =
          std::chrono::duration_cast<std::chrono::seconds>(kPeerTransportPollInterval);
      const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(
          kPeerTransportPollInterval - seconds);
      wait_time.tv_sec = static_cast<decltype(wait_time.tv_sec)>(seconds.count());
      wait_time.tv_usec = static_cast<decltype(wait_time.tv_usec)>(microseconds.count());
      wait_time_ptr = &wait_time;
    }

    ret = select(native_nfds, readfds ? &native_readfds : nullptr,
                 writefds ? &native_writefds : nullptr, exceptfds ? &native_exceptfds : nullptr,
                 wait_time_ptr);
    if (ret != 0 || (poll_peer_transport && host_readfds.HasPendingPeerDatagram()) ||
        (deadline && std::chrono::steady_clock::now() >= *deadline)) {
      break;
    }
  }
  if (ret < 0) {
    XThread::SetLastError(rex::net::socket_last_error());
    return ret;
  }

  uint32_t ready_count = 0;
  if (readfds) {
    ready_count += host_readfds.UpdateReadFrom(&native_readfds);
    host_readfds.Store(readfds);
  }
  if (writefds) {
    ready_count += host_writefds.UpdateFrom(&native_writefds);
    host_writefds.Store(writefds);
  }
  if (exceptfds) {
    ready_count += host_exceptfds.UpdateFrom(&native_exceptfds);
    host_exceptfds.Store(exceptfds);
  }

  return static_cast<i32>(ready_count);
}

u32 NetDll_recv_entry(u32 caller, u32 socket_handle, mapped_void buf_ptr, u32 buf_len, u32 flags) {
  auto socket = REX_KERNEL_OBJECTS()->LookupObject<XSocket>(socket_handle);
  if (!socket) {
    // WSAENOTSOCK
    XThread::SetLastError(0x2736);
    return -1;
  }

  const int ret = socket->Recv(buf_ptr, buf_len, flags);
  if (ret == -1) {
    XThread::SetLastError(rex::net::socket_last_error());
  }
  return ret;
}

u32 NetDll_recvfrom_entry(u32 caller, u32 socket_handle, mapped_void buf_ptr, u32 buf_len,
                          u32 flags, ppc_ptr_t<XSOCKADDR_IN> from_ptr, mapped_u32 fromlen_ptr) {
  auto socket = REX_KERNEL_OBJECTS()->LookupObject<XSocket>(socket_handle);
  if (!socket) {
    // WSAENOTSOCK
    XThread::SetLastError(0x2736);
    return -1;
  }

  N_XSOCKADDR_IN native_from;
  if (from_ptr) {
    native_from = *from_ptr;
  }
  uint32_t native_fromlen = fromlen_ptr ? fromlen_ptr.value() : 0;
  int ret =
      socket->RecvFrom(buf_ptr, buf_len, flags, &native_from, fromlen_ptr ? &native_fromlen : 0);

  if (ret >= 0 && from_ptr) {
    from_ptr->sin_family = native_from.sin_family;
    from_ptr->sin_port = native_from.sin_port;
    from_ptr->sin_addr = native_from.sin_addr;
    std::memset(from_ptr->x_sin_zero, 0, sizeof(from_ptr->x_sin_zero));
  }
  if (ret >= 0 && fromlen_ptr) {
    *fromlen_ptr = native_fromlen;
  }

  if (ret == -1) {
    XThread::SetLastError(rex::net::socket_last_error());
  }

  return ret;
}

u32 NetDll_send_entry(u32 caller, u32 socket_handle, mapped_void buf_ptr, u32 buf_len, u32 flags) {
  auto socket = REX_KERNEL_OBJECTS()->LookupObject<XSocket>(socket_handle);
  if (!socket) {
    // WSAENOTSOCK
    XThread::SetLastError(0x2736);
    return -1;
  }

  const int ret = socket->Send(buf_ptr, buf_len, flags);
  if (ret == -1) {
    XThread::SetLastError(rex::net::socket_last_error());
  }
  return ret;
}

u32 NetDll_sendto_entry(u32 caller, u32 socket_handle, mapped_void buf_ptr, u32 buf_len, u32 flags,
                        ppc_ptr_t<XSOCKADDR_IN> to_ptr, u32 to_len) {
  auto socket = REX_KERNEL_OBJECTS()->LookupObject<XSocket>(socket_handle);
  if (!socket) {
    // WSAENOTSOCK
    XThread::SetLastError(0x2736);
    return -1;
  }

  N_XSOCKADDR_IN native_to(to_ptr);
  const auto* live = REX_KERNEL_STATE()->live_compatibility();
  const uint32_t destination_ipv4 = htonl(static_cast<uint32_t>(native_to.sin_addr));
  const bool routed_peer =
      socket->UsesPeerDatagramTransport() && live && live->FindRoute(destination_ipv4).has_value();
  const int ret = socket->SendTo(buf_ptr, buf_len, flags, &native_to, to_len);
  if (ret == -1) {
    XThread::SetLastError(routed_peer ? 10065 : rex::net::socket_last_error());
  }
  return ret;
}

u32 NetDll___WSAFDIsSet_entry(u32 socket_handle, ppc_ptr_t<x_fd_set> fd_set) {
  const uint8_t max_fd_count = std::min((uint32_t)fd_set->fd_count, uint32_t(64));
  for (uint8_t i = 0; i < max_fd_count; i++) {
    if (fd_set->fd_array[i] == socket_handle) {
      return 1;
    }
  }
  return 0;
}

void NetDll_WSASetLastError_entry(u32 error_code) {
  XThread::SetLastError(error_code);
}

}  // namespace xam
}  // namespace kernel
}  // namespace rex

REX_EXPORT(__imp__XNetLogonGetMachineID, rex::kernel::xam::XNetLogonGetMachineID_entry)
REX_EXPORT(__imp__XNetLogonGetTitleID, rex::kernel::xam::XNetLogonGetTitleID_entry)
REX_EXPORT(__imp__XNetLogonGetNatType, rex::kernel::xam::XNetLogonGetNatType_entry)
REX_EXPORT(__imp__NetDll_XNetStartup, rex::kernel::xam::NetDll_XNetStartup_entry)
REX_EXPORT(__imp__NetDll_XNetCleanup, rex::kernel::xam::NetDll_XNetCleanup_entry)
REX_EXPORT(__imp__NetDll_XNetGetOpt, rex::kernel::xam::NetDll_XNetGetOpt_entry)
REX_EXPORT(__imp__NetDll_XNetRandom, rex::kernel::xam::NetDll_XNetRandom_entry)
REX_EXPORT(__imp__NetDll_WSAStartup, rex::kernel::xam::NetDll_WSAStartup_entry)
REX_EXPORT(__imp__NetDll_WSACleanup, rex::kernel::xam::NetDll_WSACleanup_entry)
REX_EXPORT(__imp__NetDll_WSAGetLastError, rex::kernel::xam::NetDll_WSAGetLastError_entry)
REX_EXPORT(__imp__NetDll_WSARecvFrom, rex::kernel::xam::NetDll_WSARecvFrom_entry)
REX_EXPORT(__imp__NetDll_WSASendTo, rex::kernel::xam::NetDll_WSASendTo_entry)
REX_EXPORT(__imp__NetDll_WSAWaitForMultipleEvents,
           rex::kernel::xam::NetDll_WSAWaitForMultipleEvents_entry)
REX_EXPORT(__imp__NetDll_WSACreateEvent, rex::kernel::xam::NetDll_WSACreateEvent_entry)
REX_EXPORT(__imp__NetDll_WSACloseEvent, rex::kernel::xam::NetDll_WSACloseEvent_entry)
REX_EXPORT(__imp__NetDll_WSAResetEvent, rex::kernel::xam::NetDll_WSAResetEvent_entry)
REX_EXPORT(__imp__NetDll_WSASetEvent, rex::kernel::xam::NetDll_WSASetEvent_entry)
REX_EXPORT(__imp__NetDll_XNetGetTitleXnAddr, rex::kernel::xam::NetDll_XNetGetTitleXnAddr_entry)
REX_EXPORT(__imp__NetDll_XNetGetDebugXnAddr, rex::kernel::xam::NetDll_XNetGetDebugXnAddr_entry)
REX_EXPORT(__imp__NetDll_XNetXnAddrToMachineId,
           rex::kernel::xam::NetDll_XNetXnAddrToMachineId_entry)
REX_EXPORT(__imp__NetDll_XNetInAddrToString, rex::kernel::xam::NetDll_XNetInAddrToString_entry)
REX_EXPORT(__imp__NetDll_XNetXnAddrToInAddr, rex::kernel::xam::NetDll_XNetXnAddrToInAddr_entry)
REX_EXPORT(__imp__NetDll_XNetInAddrToXnAddr, rex::kernel::xam::NetDll_XNetInAddrToXnAddr_entry)
REX_EXPORT(__imp__NetDll_XNetSetSystemLinkPort,
           rex::kernel::xam::NetDll_XNetSetSystemLinkPort_entry)
REX_EXPORT(__imp__NetDll_XNetGetSystemLinkPort,
           rex::kernel::xam::NetDll_XNetGetSystemLinkPort_entry)
REX_EXPORT(__imp__NetDll_XNetCreateKey, rex::kernel::xam::NetDll_XNetCreateKey_entry)
REX_EXPORT(__imp__NetDll_XNetRegisterKey, rex::kernel::xam::NetDll_XNetRegisterKey_entry)
REX_EXPORT(__imp__NetDll_XNetUnregisterKey, rex::kernel::xam::NetDll_XNetUnregisterKey_entry)
REX_EXPORT(__imp__NetDll_XNetUnregisterInAddr, rex::kernel::xam::NetDll_XNetUnregisterInAddr_entry)
REX_EXPORT(__imp__NetDll_XNetConnect, rex::kernel::xam::NetDll_XNetConnect_entry)
REX_EXPORT(__imp__NetDll_XNetGetConnectStatus, rex::kernel::xam::NetDll_XNetGetConnectStatus_entry)
REX_EXPORT(__imp__NetDll_XNetServerToInAddr, rex::kernel::xam::NetDll_XNetServerToInAddr_entry)
REX_EXPORT(__imp__NetDll_XNetGetEthernetLinkStatus,
           rex::kernel::xam::NetDll_XNetGetEthernetLinkStatus_entry)
REX_EXPORT(__imp__NetDll_XNetDnsLookup, rex::kernel::xam::NetDll_XNetDnsLookup_entry)
REX_EXPORT(__imp__NetDll_XNetDnsRelease, rex::kernel::xam::NetDll_XNetDnsRelease_entry)
REX_EXPORT(__imp__NetDll_XNetQosServiceLookup, rex::kernel::xam::NetDll_XNetQosServiceLookup_entry)
REX_EXPORT(__imp__NetDll_XNetQosRelease, rex::kernel::xam::NetDll_XNetQosRelease_entry)
REX_EXPORT(__imp__NetDll_XNetQosListen, rex::kernel::xam::NetDll_XNetQosListen_entry)
REX_EXPORT(__imp__NetDll_XNetQosLookup, rex::kernel::xam::NetDll_XNetQosLookup_entry)
REX_EXPORT(__imp__NetDll_inet_addr, rex::kernel::xam::NetDll_inet_addr_entry)
REX_EXPORT(__imp__NetDll_socket, rex::kernel::xam::NetDll_socket_entry)
REX_EXPORT(__imp__NetDll_closesocket, rex::kernel::xam::NetDll_closesocket_entry)
REX_EXPORT(__imp__NetDll_shutdown, rex::kernel::xam::NetDll_shutdown_entry)
REX_EXPORT(__imp__NetDll_setsockopt, rex::kernel::xam::NetDll_setsockopt_entry)
REX_EXPORT(__imp__NetDll_ioctlsocket, rex::kernel::xam::NetDll_ioctlsocket_entry)
REX_EXPORT(__imp__NetDll_bind, rex::kernel::xam::NetDll_bind_entry)
REX_EXPORT(__imp__NetDll_connect, rex::kernel::xam::NetDll_connect_entry)
REX_EXPORT(__imp__NetDll_listen, rex::kernel::xam::NetDll_listen_entry)
REX_EXPORT(__imp__NetDll_accept, rex::kernel::xam::NetDll_accept_entry)
REX_EXPORT(__imp__NetDll_select, rex::kernel::xam::NetDll_select_entry)
REX_EXPORT(__imp__NetDll_recv, rex::kernel::xam::NetDll_recv_entry)
REX_EXPORT(__imp__NetDll_recvfrom, rex::kernel::xam::NetDll_recvfrom_entry)
REX_EXPORT(__imp__NetDll_send, rex::kernel::xam::NetDll_send_entry)
REX_EXPORT(__imp__NetDll_sendto, rex::kernel::xam::NetDll_sendto_entry)
REX_EXPORT(__imp__NetDll___WSAFDIsSet, rex::kernel::xam::NetDll___WSAFDIsSet_entry)
REX_EXPORT(__imp__NetDll_WSASetLastError, rex::kernel::xam::NetDll_WSASetLastError_entry)

REX_EXPORT_STUB(__imp__NetDll_UpnpActionCalculateWorkBufferSize);
REX_EXPORT_STUB(__imp__NetDll_UpnpActionCreate);
REX_EXPORT_STUB(__imp__NetDll_UpnpActionGetResults);
REX_EXPORT_STUB(__imp__NetDll_UpnpCleanup);
REX_EXPORT_STUB(__imp__NetDll_UpnpCloseHandle);
REX_EXPORT_STUB(__imp__NetDll_UpnpDescribeCreate);
REX_EXPORT_STUB(__imp__NetDll_UpnpDescribeGetResults);
REX_EXPORT_STUB(__imp__NetDll_UpnpDoWork);
REX_EXPORT_STUB(__imp__NetDll_UpnpEventCreate);
REX_EXPORT_STUB(__imp__NetDll_UpnpEventGetCurrentState);
REX_EXPORT_STUB(__imp__NetDll_UpnpEventUnsubscribe);
REX_EXPORT_STUB(__imp__NetDll_UpnpSearchCreate);
REX_EXPORT_STUB(__imp__NetDll_UpnpSearchGetDevices);
REX_EXPORT_STUB(__imp__NetDll_UpnpStartup);
REX_EXPORT_STUB(__imp__NetDll_WSACancelOverlappedIO);
REX_EXPORT_STUB(__imp__NetDll_WSAEventSelect);
REX_EXPORT_STUB(__imp__NetDll_WSAGetOverlappedResult);
REX_EXPORT_STUB(__imp__NetDll_WSARecv);
REX_EXPORT_STUB(__imp__NetDll_WSASend);
REX_EXPORT_STUB(__imp__NetDll_WSAStartupEx);
REX_EXPORT_STUB(__imp__NetDll_XHttpCloseHandle);
REX_EXPORT_STUB(__imp__NetDll_XHttpConnect);
REX_EXPORT_STUB(__imp__NetDll_XHttpCrackUrl);
REX_EXPORT_STUB(__imp__NetDll_XHttpCrackUrlW);
REX_EXPORT_STUB(__imp__NetDll_XHttpCreateUrl);
REX_EXPORT_STUB(__imp__NetDll_XHttpCreateUrlW);
REX_EXPORT_STUB(__imp__NetDll_XHttpDoWork);
REX_EXPORT_STUB(__imp__NetDll_XHttpGetPerfCounters);
REX_EXPORT_STUB(__imp__NetDll_XHttpOpen);
REX_EXPORT_STUB(__imp__NetDll_XHttpOpenRequest);
REX_EXPORT_STUB(__imp__NetDll_XHttpOpenRequestUsingMemory);
REX_EXPORT_STUB(__imp__NetDll_XHttpQueryAuthSchemes);
REX_EXPORT_STUB(__imp__NetDll_XHttpQueryHeaders);
REX_EXPORT_STUB(__imp__NetDll_XHttpQueryOption);
REX_EXPORT_STUB(__imp__NetDll_XHttpReadData);
REX_EXPORT_STUB(__imp__NetDll_XHttpReceiveResponse);
REX_EXPORT_STUB(__imp__NetDll_XHttpResetPerfCounters);
REX_EXPORT_STUB(__imp__NetDll_XHttpSendRequest);
REX_EXPORT_STUB(__imp__NetDll_XHttpSetCredentials);
REX_EXPORT_STUB(__imp__NetDll_XHttpSetOption);
REX_EXPORT_STUB(__imp__NetDll_XHttpSetStatusCallback);
REX_EXPORT_STUB(__imp__NetDll_XHttpShutdown);
REX_EXPORT_STUB(__imp__NetDll_XHttpStartup);
REX_EXPORT_STUB(__imp__NetDll_XHttpWriteData);
REX_EXPORT_STUB(__imp__NetDll_XNetDnsReverseLookup);
REX_EXPORT_STUB(__imp__NetDll_XNetDnsReverseRelease);
REX_EXPORT_STUB(__imp__NetDll_XNetGetBroadcastVersionStatus);
REX_EXPORT_STUB(__imp__NetDll_XNetGetXnAddrPlatform);
REX_EXPORT_STUB(__imp__NetDll_XNetInAddrToServer);
REX_EXPORT_STUB(__imp__NetDll_XNetQosGetListenStats);
REX_EXPORT_STUB(__imp__NetDll_XNetReplaceKey);
REX_EXPORT_STUB(__imp__NetDll_XNetSetOpt);
REX_EXPORT_STUB(__imp__NetDll_XNetStartupEx);
REX_EXPORT_STUB(__imp__NetDll_XNetTsAddrToInAddr);
REX_EXPORT_STUB(__imp__NetDll_XmlDownloadContinue);
REX_EXPORT_STUB(__imp__NetDll_XmlDownloadGetParseTime);
REX_EXPORT_STUB(__imp__NetDll_XmlDownloadGetReceivedDataSize);
REX_EXPORT_STUB(__imp__NetDll_XmlDownloadStart);
REX_EXPORT_STUB(__imp__NetDll_XmlDownloadStop);
REX_EXPORT_STUB(__imp__NetDll_XnpCapture);
REX_EXPORT_STUB(__imp__NetDll_XnpConfig);
REX_EXPORT_STUB(__imp__NetDll_XnpConfigUPnP);
REX_EXPORT_STUB(__imp__NetDll_XnpConfigUPnPPortAndExternalAddr);
REX_EXPORT_STUB(__imp__NetDll_XnpEthernetInterceptRecv);
REX_EXPORT_STUB(__imp__NetDll_XnpEthernetInterceptSetCallbacks);
REX_EXPORT_STUB(__imp__NetDll_XnpEthernetInterceptSetExtendedReceiveCallback);
REX_EXPORT_STUB(__imp__NetDll_XnpEthernetInterceptXmit);
REX_EXPORT_STUB(__imp__NetDll_XnpEthernetInterceptXmitAsIp);
REX_EXPORT_STUB(__imp__NetDll_XnpGetActiveSocketList);
REX_EXPORT_STUB(__imp__NetDll_XnpGetConfigStatus);
REX_EXPORT_STUB(__imp__NetDll_XnpGetKeyList);
REX_EXPORT_STUB(__imp__NetDll_XnpGetQosLookupList);
REX_EXPORT_STUB(__imp__NetDll_XnpGetSecAssocList);
REX_EXPORT_STUB(__imp__NetDll_XnpGetVlanXboxName);
REX_EXPORT_STUB(__imp__NetDll_XnpLoadConfigParams);
REX_EXPORT_STUB(__imp__NetDll_XnpLoadMachineAccount);
REX_EXPORT_STUB(__imp__NetDll_XnpLogonClearChallenge);
REX_EXPORT_STUB(__imp__NetDll_XnpLogonClearQEvent);
REX_EXPORT_STUB(__imp__NetDll_XnpLogonGetChallenge);
REX_EXPORT_STUB(__imp__NetDll_XnpLogonGetQFlags);
REX_EXPORT_STUB(__imp__NetDll_XnpLogonGetQVals);
REX_EXPORT_STUB(__imp__NetDll_XnpLogonGetStatus);
REX_EXPORT_STUB(__imp__NetDll_XnpLogonSetChallengeResponse);
REX_EXPORT_STUB(__imp__NetDll_XnpLogonSetPState);
REX_EXPORT_STUB(__imp__NetDll_XnpLogonSetQEvent);
REX_EXPORT_STUB(__imp__NetDll_XnpLogonSetQFlags);
REX_EXPORT_STUB(__imp__NetDll_XnpLogonSetQVals);
REX_EXPORT_STUB(__imp__NetDll_XnpNoteSystemTime);
REX_EXPORT_STUB(__imp__NetDll_XnpPersistTitleState);
REX_EXPORT_STUB(__imp__NetDll_XnpQosHistoryGetAggregateMeasurement);
REX_EXPORT_STUB(__imp__NetDll_XnpQosHistoryGetEntries);
REX_EXPORT_STUB(__imp__NetDll_XnpQosHistoryLoad);
REX_EXPORT_STUB(__imp__NetDll_XnpQosHistorySaveMeasurements);
REX_EXPORT_STUB(__imp__NetDll_XnpRegisterKeyForCallerType);
REX_EXPORT_STUB(__imp__NetDll_XnpReplaceKeyForCallerType);
REX_EXPORT_STUB(__imp__NetDll_XnpSaveConfigParams);
REX_EXPORT_STUB(__imp__NetDll_XnpSaveMachineAccount);
REX_EXPORT_STUB(__imp__NetDll_XnpSetVlanXboxName);
REX_EXPORT_STUB(__imp__NetDll_XnpToolIpProxyInject);
REX_EXPORT_STUB(__imp__NetDll_XnpToolSetCallbacks);
REX_EXPORT_STUB(__imp__NetDll_XnpUnregisterKeyForCallerType);
REX_EXPORT_STUB(__imp__NetDll_XnpUpdateConfigParams);
REX_EXPORT_STUB(__imp__NetDll_getpeername);
REX_EXPORT_STUB(__imp__NetDll_getsockname);
REX_EXPORT_STUB(__imp__NetDll_getsockopt);
