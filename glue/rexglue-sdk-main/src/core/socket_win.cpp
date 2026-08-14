#include <rex/net/socket.h>
#include <rex/platform.h>

static_assert(REX_PLATFORM_WIN32, "This file is Windows-only");

#include "platform_win.h"

#include <WinSock2.h>

namespace rex::net {

int socket_close(SocketHandle handle) {
  return closesocket(static_cast<SOCKET>(handle));
}

int socket_ioctl(SocketHandle handle, uint32_t cmd, uint8_t* arg) {
  return ioctlsocket(static_cast<SOCKET>(handle), cmd, reinterpret_cast<u_long*>(arg));
}

uint32_t socket_last_error() {
  return static_cast<uint32_t>(WSAGetLastError());
}

}  // namespace rex::net
