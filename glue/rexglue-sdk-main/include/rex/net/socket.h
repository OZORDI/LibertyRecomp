/**
 * @file        net/socket.h
 * @brief       Platform-agnostic socket operations
 */
#pragma once

#include <cstdint>

namespace rex::net {

using SocketHandle = int64_t;
constexpr SocketHandle kInvalidSocket = -1;

int socket_close(SocketHandle handle);
int socket_ioctl(SocketHandle handle, uint32_t cmd, uint8_t* arg);
uint32_t socket_last_error();

}  // namespace rex::net
