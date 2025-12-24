#include "net_socket.h"
#include <kernel/memory.h>
#include <os/logger.h>
#include <cstring>
#include <vector>

namespace Net {

// ============================================================================
// SocketManager Implementation
// ============================================================================

SocketManager& SocketManager::Instance() {
    static SocketManager instance;
    return instance;
}

SocketManager::~SocketManager() {
    Cleanup();
}

bool SocketManager::Initialize() {
    if (initialized_) return true;
    
#ifdef _WIN32
    WSADATA wsaData;
    int result = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        LOGF_ERROR("[Net] WSAStartup failed: {}", result);
        return false;
    }
#endif
    
    initialized_ = true;
    LOG_WARNING("[Net] Socket system initialized");
    return true;
}

void SocketManager::Cleanup() {
    if (!initialized_) return;
    
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Close all open sockets
    for (auto& pair : handles_) {
#ifdef _WIN32
        ::closesocket(pair.second);
#else
        ::close(pair.second);
#endif
    }
    handles_.clear();
    
#ifdef _WIN32
    ::WSACleanup();
#endif
    
    initialized_ = false;
    LOG_WARNING("[Net] Socket system cleaned up");
}

void SocketManager::XboxToNativeSockaddr(const XSOCKADDR_IN* xbox, sockaddr_in* native) {
    std::memset(native, 0, sizeof(*native));
    native->sin_family = AF_INET;
    // Xbox addresses are already in network byte order (big-endian)
    native->sin_port = xbox->sin_port;
    native->sin_addr.s_addr = xbox->sin_addr;
}

void SocketManager::NativeToXboxSockaddr(const sockaddr_in* native, XSOCKADDR_IN* xbox) {
    std::memset(xbox, 0, sizeof(*xbox));
    xbox->sin_family = ByteSwap(static_cast<uint16_t>(native->sin_family));
    xbox->sin_port = native->sin_port;
    xbox->sin_addr = native->sin_addr.s_addr;
}

uint32_t SocketManager::CreateSocket(int af, int type, int protocol) {
    if (!initialized_) {
        LOG_ERROR("[Net] CreateSocket called but not initialized");
        return static_cast<uint32_t>(-1);
    }
    
    native_socket_t sock = ::socket(af, type, protocol);
    if (sock == INVALID_NATIVE_SOCKET) {
#ifdef _WIN32
        lastError_ = ::WSAGetLastError();
#else
        lastError_ = errno;
#endif
        LOGF_ERROR("[Net] socket() failed: {}", lastError_);
        return static_cast<uint32_t>(-1);
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    uint32_t handle = nextHandle_++;
    handles_[handle] = sock;
    
    LOGF_WARNING("[Net] Created socket handle=0x{:08X} native={} af={} type={} proto={}",
                handle, static_cast<int64_t>(sock), af, type, protocol);
    
    return handle;
}

bool SocketManager::CloseSocket(uint32_t handle) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = handles_.find(handle);
    if (it == handles_.end()) {
        LOGF_ERROR("[Net] CloseSocket: invalid handle 0x{:08X}", handle);
        return false;
    }
    
#ifdef _WIN32
    ::closesocket(it->second);
#else
    ::close(it->second);
#endif
    
    handles_.erase(it);
    LOGF_WARNING("[Net] Closed socket handle=0x{:08X}", handle);
    return true;
}

native_socket_t SocketManager::GetNativeHandle(uint32_t handle) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = handles_.find(handle);
    if (it == handles_.end()) {
        return INVALID_NATIVE_SOCKET;
    }
    return it->second;
}

int SocketManager::SetSockOpt(uint32_t handle, int level, int optname,
                               const void* optval, int optlen) {
    native_socket_t sock = GetNativeHandle(handle);
    if (sock == INVALID_NATIVE_SOCKET) {
        lastError_ = 0x2736;  // WSAENOTSOCK
        return -1;
    }
    
    int result = ::setsockopt(sock, level, optname, 
                              static_cast<const char*>(optval), optlen);
    if (result < 0) {
#ifdef _WIN32
        lastError_ = ::WSAGetLastError();
#else
        lastError_ = errno;
#endif
    }
    return result;
}

int SocketManager::IOCtl(uint32_t handle, uint32_t cmd, uint32_t* arg) {
    native_socket_t sock = GetNativeHandle(handle);
    if (sock == INVALID_NATIVE_SOCKET) {
        lastError_ = 0x2736;  // WSAENOTSOCK
        return -1;
    }
    
    int result = 0;
#ifdef _WIN32
    u_long mode = *arg;
    result = ::ioctlsocket(sock, cmd, &mode);
    *arg = static_cast<uint32_t>(mode);
    if (result < 0) {
        lastError_ = ::WSAGetLastError();
    }
#else
    int flags = fcntl(sock, F_GETFL, 0);
    if (cmd == 0x8004667E) {  // FIONBIO
        if (*arg) {
            flags |= O_NONBLOCK;
        } else {
            flags &= ~O_NONBLOCK;
        }
        result = fcntl(sock, F_SETFL, flags);
        if (result < 0) {
            lastError_ = errno;
        }
    }
#endif
    return result;
}

int SocketManager::Bind(uint32_t handle, const XSOCKADDR_IN* addr, int addrlen) {
    native_socket_t sock = GetNativeHandle(handle);
    if (sock == INVALID_NATIVE_SOCKET) {
        lastError_ = 0x2736;  // WSAENOTSOCK
        return -1;
    }
    
    sockaddr_in native;
    XboxToNativeSockaddr(addr, &native);
    
    LOGF_WARNING("[Net] Bind socket=0x{:08X} port={} addr=0x{:08X}",
                handle, ntohs(native.sin_port), ntohl(native.sin_addr.s_addr));
    
    int result = ::bind(sock, reinterpret_cast<sockaddr*>(&native), sizeof(native));
    if (result < 0) {
#ifdef _WIN32
        lastError_ = ::WSAGetLastError();
#else
        lastError_ = errno;
#endif
        LOGF_ERROR("[Net] bind() failed: {}", lastError_);
    }
    return result;
}

int SocketManager::Connect(uint32_t handle, const XSOCKADDR_IN* addr, int addrlen) {
    native_socket_t sock = GetNativeHandle(handle);
    if (sock == INVALID_NATIVE_SOCKET) {
        lastError_ = 0x2736;  // WSAENOTSOCK
        return -1;
    }
    
    sockaddr_in native;
    XboxToNativeSockaddr(addr, &native);
    
    LOGF_WARNING("[Net] Connect socket=0x{:08X} to {}:{}", 
                handle, inet_ntoa(native.sin_addr), ntohs(native.sin_port));
    
    int result = ::connect(sock, reinterpret_cast<sockaddr*>(&native), sizeof(native));
    if (result < 0) {
#ifdef _WIN32
        lastError_ = ::WSAGetLastError();
#else
        lastError_ = errno;
#endif
        LOGF_ERROR("[Net] connect() failed: {}", lastError_);
    }
    return result;
}

int SocketManager::Listen(uint32_t handle, int backlog) {
    native_socket_t sock = GetNativeHandle(handle);
    if (sock == INVALID_NATIVE_SOCKET) {
        lastError_ = 0x2736;  // WSAENOTSOCK
        return -1;
    }
    
    LOGF_WARNING("[Net] Listen socket=0x{:08X} backlog={}", handle, backlog);
    
    int result = ::listen(sock, backlog);
    if (result < 0) {
#ifdef _WIN32
        lastError_ = ::WSAGetLastError();
#else
        lastError_ = errno;
#endif
    }
    return result;
}

uint32_t SocketManager::Accept(uint32_t handle, XSOCKADDR_IN* addr, int* addrlen) {
    native_socket_t sock = GetNativeHandle(handle);
    if (sock == INVALID_NATIVE_SOCKET) {
        lastError_ = 0x2736;  // WSAENOTSOCK
        return static_cast<uint32_t>(-1);
    }
    
    sockaddr_in native;
    socklen_t len = sizeof(native);
    
    native_socket_t newSock = ::accept(sock, reinterpret_cast<sockaddr*>(&native), &len);
    if (newSock == INVALID_NATIVE_SOCKET) {
#ifdef _WIN32
        lastError_ = ::WSAGetLastError();
#else
        lastError_ = errno;
#endif
        return static_cast<uint32_t>(-1);
    }
    
    if (addr && addrlen) {
        NativeToXboxSockaddr(&native, addr);
        *addrlen = sizeof(XSOCKADDR_IN);
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    uint32_t newHandle = nextHandle_++;
    handles_[newHandle] = newSock;
    
    LOGF_WARNING("[Net] Accept on socket=0x{:08X} new=0x{:08X} from {}:{}",
                handle, newHandle, inet_ntoa(native.sin_addr), ntohs(native.sin_port));
    
    return newHandle;
}

int SocketManager::Shutdown(uint32_t handle, int how) {
    native_socket_t sock = GetNativeHandle(handle);
    if (sock == INVALID_NATIVE_SOCKET) {
        lastError_ = 0x2736;  // WSAENOTSOCK
        return -1;
    }
    
    int result = ::shutdown(sock, how);
    if (result < 0) {
#ifdef _WIN32
        lastError_ = ::WSAGetLastError();
#else
        lastError_ = errno;
#endif
    }
    return result;
}

int SocketManager::Send(uint32_t handle, const void* buf, int len, int flags) {
    native_socket_t sock = GetNativeHandle(handle);
    if (sock == INVALID_NATIVE_SOCKET) {
        lastError_ = 0x2736;  // WSAENOTSOCK
        return -1;
    }
    
    int result = ::send(sock, static_cast<const char*>(buf), len, flags);
    if (result < 0) {
#ifdef _WIN32
        lastError_ = ::WSAGetLastError();
#else
        lastError_ = errno;
#endif
    }
    return result;
}

int SocketManager::Recv(uint32_t handle, void* buf, int len, int flags) {
    native_socket_t sock = GetNativeHandle(handle);
    if (sock == INVALID_NATIVE_SOCKET) {
        lastError_ = 0x2736;  // WSAENOTSOCK
        return -1;
    }
    
    int result = ::recv(sock, static_cast<char*>(buf), len, flags);
    if (result < 0) {
#ifdef _WIN32
        lastError_ = ::WSAGetLastError();
#else
        lastError_ = errno;
#endif
    }
    return result;
}

int SocketManager::SendTo(uint32_t handle, const void* buf, int len, int flags,
                           const XSOCKADDR_IN* to, int tolen) {
    native_socket_t sock = GetNativeHandle(handle);
    if (sock == INVALID_NATIVE_SOCKET) {
        lastError_ = 0x2736;  // WSAENOTSOCK
        return -1;
    }
    
    sockaddr_in native;
    XboxToNativeSockaddr(to, &native);
    
    int result = ::sendto(sock, static_cast<const char*>(buf), len, flags,
                          reinterpret_cast<sockaddr*>(&native), sizeof(native));
    if (result < 0) {
#ifdef _WIN32
        lastError_ = ::WSAGetLastError();
#else
        lastError_ = errno;
#endif
    }
    return result;
}

int SocketManager::RecvFrom(uint32_t handle, void* buf, int len, int flags,
                             XSOCKADDR_IN* from, int* fromlen) {
    native_socket_t sock = GetNativeHandle(handle);
    if (sock == INVALID_NATIVE_SOCKET) {
        lastError_ = 0x2736;  // WSAENOTSOCK
        return -1;
    }
    
    sockaddr_in native;
    socklen_t nativeLen = sizeof(native);
    
    int result = ::recvfrom(sock, static_cast<char*>(buf), len, flags,
                            reinterpret_cast<sockaddr*>(&native), &nativeLen);
    if (result < 0) {
#ifdef _WIN32
        lastError_ = ::WSAGetLastError();
#else
        lastError_ = errno;
#endif
    } else if (from && fromlen) {
        NativeToXboxSockaddr(&native, from);
        *fromlen = sizeof(XSOCKADDR_IN);
    }
    
    return result;
}

int SocketManager::Select(int nfds, void* readfds, void* writefds,
                          void* exceptfds, void* timeout) {
    static int s_count = 0; ++s_count;
    
    x_fd_set* xReadfds = static_cast<x_fd_set*>(readfds);
    x_fd_set* xWritefds = static_cast<x_fd_set*>(writefds);
    x_fd_set* xExceptfds = static_cast<x_fd_set*>(exceptfds);
    x_timeval* xTimeout = static_cast<x_timeval*>(timeout);
    
    // Build native fd_sets from Xbox fd_sets
    fd_set nativeReadfds, nativeWritefds, nativeExceptfds;
    FD_ZERO(&nativeReadfds);
    FD_ZERO(&nativeWritefds);
    FD_ZERO(&nativeExceptfds);
    
    int maxFd = 0;
    
    // Temporary arrays to track which sockets are in each set
    std::vector<std::pair<uint32_t, native_socket_t>> readSockets;
    std::vector<std::pair<uint32_t, native_socket_t>> writeSockets;
    std::vector<std::pair<uint32_t, native_socket_t>> exceptSockets;
    
    // Load read fd_set
    if (xReadfds) {
        uint32_t count = ByteSwap(xReadfds->fd_count);
        for (uint32_t i = 0; i < count && i < 64; ++i) {
            uint32_t handle = ByteSwap(xReadfds->fd_array[i]);
            if (handle == 0xFFFFFFFF) break;
            
            native_socket_t sock = GetNativeHandle(handle);
            if (sock != INVALID_NATIVE_SOCKET) {
                FD_SET(sock, &nativeReadfds);
                readSockets.push_back({handle, sock});
                if (static_cast<int>(sock) > maxFd) maxFd = static_cast<int>(sock);
            }
        }
    }
    
    // Load write fd_set
    if (xWritefds) {
        uint32_t count = ByteSwap(xWritefds->fd_count);
        for (uint32_t i = 0; i < count && i < 64; ++i) {
            uint32_t handle = ByteSwap(xWritefds->fd_array[i]);
            if (handle == 0xFFFFFFFF) break;
            
            native_socket_t sock = GetNativeHandle(handle);
            if (sock != INVALID_NATIVE_SOCKET) {
                FD_SET(sock, &nativeWritefds);
                writeSockets.push_back({handle, sock});
                if (static_cast<int>(sock) > maxFd) maxFd = static_cast<int>(sock);
            }
        }
    }
    
    // Load except fd_set
    if (xExceptfds) {
        uint32_t count = ByteSwap(xExceptfds->fd_count);
        for (uint32_t i = 0; i < count && i < 64; ++i) {
            uint32_t handle = ByteSwap(xExceptfds->fd_array[i]);
            if (handle == 0xFFFFFFFF) break;
            
            native_socket_t sock = GetNativeHandle(handle);
            if (sock != INVALID_NATIVE_SOCKET) {
                FD_SET(sock, &nativeExceptfds);
                exceptSockets.push_back({handle, sock});
                if (static_cast<int>(sock) > maxFd) maxFd = static_cast<int>(sock);
            }
        }
    }
    
    // Convert timeout
    timeval nativeTimeout;
    timeval* timeoutPtr = nullptr;
    if (xTimeout) {
        nativeTimeout.tv_sec = ByteSwap(xTimeout->tv_sec);
        nativeTimeout.tv_usec = ByteSwap(xTimeout->tv_usec);
        timeoutPtr = &nativeTimeout;
    }
    
    if (s_count <= 10) {
        LOGF_WARNING("[Net] Select #{} read={} write={} except={} timeout={}",
                    s_count, readSockets.size(), writeSockets.size(), exceptSockets.size(),
                    timeoutPtr ? "yes" : "infinite");
    }
    
    // Call native select
    int result = ::select(maxFd + 1,
                          xReadfds ? &nativeReadfds : nullptr,
                          xWritefds ? &nativeWritefds : nullptr,
                          xExceptfds ? &nativeExceptfds : nullptr,
                          timeoutPtr);
    
    if (result < 0) {
#ifdef _WIN32
        lastError_ = ::WSAGetLastError();
#else
        lastError_ = errno;
#endif
        return result;
    }
    
    // Update Xbox fd_sets with results
    if (xReadfds) {
        uint32_t newCount = 0;
        for (auto& [handle, sock] : readSockets) {
            if (FD_ISSET(sock, &nativeReadfds)) {
                xReadfds->fd_array[newCount++] = ByteSwap(handle);
            }
        }
        xReadfds->fd_count = ByteSwap(newCount);
    }
    
    if (xWritefds) {
        uint32_t newCount = 0;
        for (auto& [handle, sock] : writeSockets) {
            if (FD_ISSET(sock, &nativeWritefds)) {
                xWritefds->fd_array[newCount++] = ByteSwap(handle);
            }
        }
        xWritefds->fd_count = ByteSwap(newCount);
    }
    
    if (xExceptfds) {
        uint32_t newCount = 0;
        for (auto& [handle, sock] : exceptSockets) {
            if (FD_ISSET(sock, &nativeExceptfds)) {
                xExceptfds->fd_array[newCount++] = ByteSwap(handle);
            }
        }
        xExceptfds->fd_count = ByteSwap(newCount);
    }
    
    return result;
}

int SocketManager::GetLastError() {
    return lastError_;
}

void SocketManager::SetLastError(int error) {
    lastError_ = error;
}

// ============================================================================
// API Function Implementations
// ============================================================================

uint32_t WSAStartup(uint16_t version, XWSADATA* wsaData) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[Net] WSAStartup #{} version=0x{:04X}", s_count, version);
    
    if (!SocketManager::Instance().Initialize()) {
        return 10093;  // WSANOTINITIALISED
    }
    
    if (wsaData) {
        std::memset(wsaData, 0, sizeof(*wsaData));
        wsaData->wVersion = ByteSwap(static_cast<uint16_t>(0x0202));
        wsaData->wHighVersion = ByteSwap(static_cast<uint16_t>(0x0202));
        wsaData->iMaxSockets = ByteSwap(static_cast<uint16_t>(64));
        wsaData->iMaxUdpDg = ByteSwap(static_cast<uint16_t>(65507));
    }
    
    return 0;
}

int WSACleanup() {
    LOG_WARNING("[Net] WSACleanup");
    SocketManager::Instance().Cleanup();
    return 0;
}

int WSAGetLastError() {
    return SocketManager::Instance().GetLastError();
}

uint32_t Socket(uint32_t caller, uint32_t af, uint32_t type, uint32_t protocol) {
    return SocketManager::Instance().CreateSocket(af, type, protocol);
}

int CloseSocket(uint32_t caller, uint32_t socket) {
    return SocketManager::Instance().CloseSocket(socket) ? 0 : -1;
}

int Shutdown(uint32_t caller, uint32_t socket, int how) {
    return SocketManager::Instance().Shutdown(socket, how);
}

int IOCtlSocket(uint32_t caller, uint32_t socket, uint32_t cmd, uint32_t* arg) {
    return SocketManager::Instance().IOCtl(socket, cmd, arg);
}

int SetSockOpt(uint32_t caller, uint32_t socket, int level, int optname,
               const void* optval, int optlen) {
    return SocketManager::Instance().SetSockOpt(socket, level, optname, optval, optlen);
}

int GetSockName(uint32_t caller, uint32_t socket, XSOCKADDR_IN* name, int* namelen) {
    LOG_WARNING("[Net] GetSockName - stub");
    return 0;
}

int Bind(uint32_t caller, uint32_t socket, const XSOCKADDR_IN* addr, int addrlen) {
    return SocketManager::Instance().Bind(socket, addr, addrlen);
}

int Connect(uint32_t caller, uint32_t socket, const XSOCKADDR_IN* addr, int addrlen) {
    return SocketManager::Instance().Connect(socket, addr, addrlen);
}

int Listen(uint32_t caller, uint32_t socket, int backlog) {
    return SocketManager::Instance().Listen(socket, backlog);
}

uint32_t Accept(uint32_t caller, uint32_t socket, XSOCKADDR_IN* addr, int* addrlen) {
    return SocketManager::Instance().Accept(socket, addr, addrlen);
}

int Select(int caller, int nfds, void* readfds, void* writefds,
           void* exceptfds, void* timeout) {
    return SocketManager::Instance().Select(nfds, readfds, writefds, exceptfds, timeout);
}

int Recv(uint32_t caller, uint32_t socket, void* buf, int len, int flags) {
    return SocketManager::Instance().Recv(socket, buf, len, flags);
}

int Send(uint32_t caller, uint32_t socket, const void* buf, int len, int flags) {
    return SocketManager::Instance().Send(socket, buf, len, flags);
}

int RecvFrom(uint32_t caller, uint32_t socket, void* buf, int len, int flags,
             XSOCKADDR_IN* from, int* fromlen) {
    return SocketManager::Instance().RecvFrom(socket, buf, len, flags, from, fromlen);
}

int SendTo(uint32_t caller, uint32_t socket, const void* buf, int len, int flags,
           const XSOCKADDR_IN* to, int tolen) {
    return SocketManager::Instance().SendTo(socket, buf, len, flags, to, tolen);
}

uint32_t InetAddr(uint32_t caller, const char* cp) {
    if (!cp) return 0xFFFFFFFF;  // INADDR_NONE
    
    uint32_t addr = inet_addr(cp);
    // inet_addr returns in network byte order, which is what Xbox expects
    return addr;
}

// ============================================================================
// XNet Layer Implementation
// ============================================================================

static XNetStartupParams g_xnetParams = {0};
static bool g_xnetInitialized = false;

uint32_t XNetStartup(uint32_t caller, XNetStartupParams* params) {
    static int s_count = 0; ++s_count;
    LOGF_WARNING("[Net] XNetStartup #{}", s_count);
    
    if (params) {
        std::memcpy(&g_xnetParams, params, sizeof(g_xnetParams));
    }
    
    // Ensure socket layer is initialized
    if (!SocketManager::Instance().Initialize()) {
        return 1;  // Error
    }
    
    g_xnetInitialized = true;
    return 0;  // Success
}

int XNetCleanup(uint32_t caller) {
    LOG_WARNING("[Net] XNetCleanup");
    g_xnetInitialized = false;
    return 0;
}

uint32_t XNetGetTitleXnAddr(uint32_t caller, XNADDR* addr) {
    static int s_count = 0; ++s_count;
    
    if (addr) {
        std::memset(addr, 0, sizeof(*addr));
        
        // Return a fake local IP: 192.168.1.100
        // Network byte order (big-endian)
        addr->ina = 0x6401A8C0;  // 192.168.1.100
        addr->inaOnline = 0x6401A8C0;
        addr->wPortOnline = ByteSwap(static_cast<uint16_t>(3074));
        
        // Fake MAC address
        addr->abEnet[0] = 0x00;
        addr->abEnet[1] = 0x11;
        addr->abEnet[2] = 0x22;
        addr->abEnet[3] = 0x33;
        addr->abEnet[4] = 0x44;
        addr->abEnet[5] = 0x55;
    }
    
    if (s_count <= 5) {
        LOGF_WARNING("[Net] XNetGetTitleXnAddr #{} -> STATIC (fake 192.168.1.100)", s_count);
    }
    
    // Return XNET_GET_XNADDR_STATIC - we have a static IP
    return XNET_GET_XNADDR_STATIC | XNET_GET_XNADDR_ETHERNET;
}

uint32_t XNetGetConnectStatus(uint32_t caller, void* addr) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) {
        LOGF_WARNING("[Net] XNetGetConnectStatus #{} -> CONNECTED", s_count);
    }
    return XNET_CONNECT_STATUS_CONNECTED;
}

uint32_t XNetGetEthernetLinkStatus(uint32_t caller) {
    static int s_count = 0; ++s_count;
    if (s_count <= 5) {
        LOGF_WARNING("[Net] XNetGetEthernetLinkStatus #{} -> ACTIVE", s_count);
    }
    return XNET_ETHERNET_LINK_ACTIVE | XNET_ETHERNET_LINK_100MBPS | XNET_ETHERNET_LINK_FULL_DUPLEX;
}

int XNetXnAddrToInAddr(uint32_t caller, const XNADDR* xnaddr, void* xnkid, uint32_t* inaddr) {
    // Convert XNADDR to IN_ADDR - for LAN play, just use the ina field
    if (xnaddr && inaddr) {
        *inaddr = xnaddr->ina;
    }
    LOG_WARNING("[Net] XNetXnAddrToInAddr - returning ina directly");
    return 0;
}

int XNetServerToInAddr(uint32_t caller, uint32_t inaddr, uint32_t service, uint32_t* outaddr) {
    // Stub - for online services we don't support
    LOG_WARNING("[Net] XNetServerToInAddr - stub");
    if (outaddr) *outaddr = inaddr;
    return 0;
}

int XNetUnregisterInAddr(uint32_t caller, uint32_t inaddr) {
    LOG_WARNING("[Net] XNetUnregisterInAddr - stub");
    return 0;
}

int XNetQosListen(uint32_t caller, void* xnkid, void* pb, uint32_t cb,
                  uint32_t bitsPerSec, uint32_t flags) {
    LOG_WARNING("[Net] XNetQosListen - stub");
    return 0;
}

int XNetQosLookup(uint32_t caller, uint32_t cxna, void* apxna, void* apxnkid,
                  void* apxnkey, uint32_t cina, void* aina, void* adwSvcId,
                  uint32_t cProbes, uint32_t dwBitsPerSec, uint32_t flags,
                  void* phEvent, void* ppxnqos) {
    LOG_WARNING("[Net] XNetQosLookup - stub");
    return 0;
}

int XNetQosRelease(uint32_t caller, void* pxnqos) {
    LOG_WARNING("[Net] XNetQosRelease - stub");
    return 0;
}

} // namespace Net
