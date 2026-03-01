#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <random>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef SOCKET native_socket_t;
#define INVALID_NATIVE_SOCKET INVALID_SOCKET
#else
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
typedef int native_socket_t;
#define INVALID_NATIVE_SOCKET (-1)
#endif

/**
 * GTA IV Network Socket Implementation
 * 
 * Following the Xenia approach, we wrap native BSD sockets with Xbox 360
 * handle management and byte-order conversion.
 * 
 * Key differences from Xbox 360:
 * - Addresses in guest memory are big-endian
 * - Socket handles are kernel objects (we use our own handle space)
 * - XNet layer is simplified (no Xbox Live authentication)
 */

namespace Net {

// ============================================================================
// Network Logging (Xenia-style configurable logging)
// ============================================================================

enum class NetLogLevel : uint32_t {
    Off = 0,      // No network logging
    Errors = 1,   // Only errors
    Info = 2,     // Errors + important events (default)
    Verbose = 3,  // All network calls
    Debug = 4     // Everything including packet data
};

// Global log level (can be changed at runtime or via config)
extern std::atomic<NetLogLevel> g_netLogLevel;

void SetNetLogLevel(NetLogLevel level);
NetLogLevel GetNetLogLevel();

// Logging macros that respect the log level
#define NET_LOG_ERROR(fmt, ...) \
    do { if (Net::GetNetLogLevel() >= Net::NetLogLevel::Errors) LOGF_ERROR("[Net] " fmt, ##__VA_ARGS__); } while(0)
#define NET_LOG_INFO(fmt, ...) \
    do { if (Net::GetNetLogLevel() >= Net::NetLogLevel::Info) LOGF_INFO("[Net] " fmt, ##__VA_ARGS__); } while(0)
#define NET_LOG_VERBOSE(fmt, ...) \
    do { if (Net::GetNetLogLevel() >= Net::NetLogLevel::Verbose) LOGF_WARNING("[Net] " fmt, ##__VA_ARGS__); } while(0)
#define NET_LOG_DEBUG(fmt, ...) \
    do { if (Net::GetNetLogLevel() >= Net::NetLogLevel::Debug) LOGF_DEBUG("[Net] " fmt, ##__VA_ARGS__); } while(0)

// ============================================================================
// NAT Type Detection (from P2P connectivity tests)
// ============================================================================

enum class NatType : uint32_t {
    Unknown = 0,
    Open = 1,       // XONLINE_NAT_OPEN - Full connectivity
    Moderate = 2,   // XONLINE_NAT_MODERATE - Some restrictions
    Strict = 3      // XONLINE_NAT_STRICT - Restricted connectivity
};

// Get current NAT type (detected from P2P/STUN tests)
NatType GetDetectedNatType();

// ============================================================================
// Network Identity (Consistent fake addresses for offline mode)
// ============================================================================

/**
 * NetworkIdentity - Provides consistent fake network addresses
 * 
 * When not connected to Xbox Live (which is always for LibertyRecomp),
 * we need to provide valid-looking addresses that remain consistent
 * across the session for proper game behavior.
 */
struct NetworkIdentity {
    uint32_t localIp;           // Local IP (192.168.1.x style)
    uint32_t onlineIp;          // Online IP (same as local for offline)
    uint16_t onlinePort;        // Online port (3074 = Xbox Live default)
    uint8_t macAddress[6];      // Fake MAC address
    uint8_t onlineKey[20];      // Fake online identification key
    
    // Generate a consistent identity (seeded by machine-specific data)
    static NetworkIdentity Generate();
    
    // Get the singleton instance (consistent per session)
    static const NetworkIdentity& Get();

private:
    static NetworkIdentity s_instance;
    static bool s_initialized;
};

// ============================================================================
// Xbox 360 Socket Structures (Big-Endian in Guest Memory)
// ============================================================================

#pragma pack(push, 1)

struct XSOCKADDR {
    uint16_t sa_family;  // Big-endian: AF_INET = 2
    char sa_data[14];
};

struct XSOCKADDR_IN {
    uint16_t sin_family;  // Big-endian
    uint16_t sin_port;    // Big-endian (network byte order)
    uint32_t sin_addr;    // Big-endian (network byte order)
    char sin_zero[8];
};

// XNADDR - Xbox Network Address (36 bytes)
struct XNADDR {
    uint32_t ina;           // 0x00: Local IP address
    uint32_t inaOnline;     // 0x04: Online IP address
    uint16_t wPortOnline;   // 0x08: Online port
    uint8_t abEnet[6];      // 0x0A: Ethernet MAC address
    uint8_t abOnline[20];   // 0x10: Online identification key
};

// WSADATA structure for WSAStartup
struct XWSADATA {
    uint16_t wVersion;
    uint16_t wHighVersion;
    char szDescription[257];
    char szSystemStatus[129];
    uint16_t iMaxSockets;
    uint16_t iMaxUdpDg;
    uint32_t lpVendorInfo;
};

// XNetStartupParams
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

#pragma pack(pop)

// Xbox Live NAT types (for XLiveBaseGetNatType)
enum XOnlineNatType : uint32_t {
    XONLINE_NAT_OPEN = 1,     // Full connectivity
    XONLINE_NAT_MODERATE = 2, // Some restrictions
    XONLINE_NAT_STRICT = 3    // Restricted connectivity
};

// Xbox fd_set structure (big-endian)
struct x_fd_set {
    uint32_t fd_count;      // Big-endian count
    uint32_t fd_array[64];  // Big-endian socket handles
};

// Xbox timeval structure (big-endian)
struct x_timeval {
    int32_t tv_sec;   // Big-endian seconds
    int32_t tv_usec;  // Big-endian microseconds
};

// ============================================================================
// XNet Address Status Codes
// ============================================================================

enum XNetAddrStatus : uint32_t {
    XNET_GET_XNADDR_PENDING   = 0x00000000,
    XNET_GET_XNADDR_NONE      = 0x00000001,
    XNET_GET_XNADDR_ETHERNET  = 0x00000002,
    XNET_GET_XNADDR_STATIC    = 0x00000004,
    XNET_GET_XNADDR_DHCP      = 0x00000008,
    XNET_GET_XNADDR_PPPOE     = 0x00000010,
    XNET_GET_XNADDR_GATEWAY   = 0x00000020,
    XNET_GET_XNADDR_DNS       = 0x00000040,
    XNET_GET_XNADDR_ONLINE    = 0x00000080,
    XNET_GET_XNADDR_TROUBLESHOOT = 0x00008000,
};

// Ethernet link status
enum XNetEthernetStatus : uint32_t {
    XNET_ETHERNET_LINK_ACTIVE     = 0x01,
    XNET_ETHERNET_LINK_100MBPS    = 0x02,
    XNET_ETHERNET_LINK_10MBPS     = 0x04,
    XNET_ETHERNET_LINK_FULL_DUPLEX = 0x08,
    XNET_ETHERNET_LINK_HALF_DUPLEX = 0x10,
};

// XNet connect status
enum XNetConnectStatus : uint32_t {
    XNET_CONNECT_STATUS_IDLE      = 0,
    XNET_CONNECT_STATUS_PENDING   = 1,
    XNET_CONNECT_STATUS_CONNECTED = 2,
    XNET_CONNECT_STATUS_LOST      = 3,
};

// ============================================================================
// Socket Manager - Handles mapping between guest handles and native sockets
// ============================================================================

class SocketManager {
public:
    static SocketManager& Instance();
    
    // Initialize/cleanup Winsock (Windows) or no-op (POSIX)
    bool Initialize();
    void Cleanup();
    bool IsInitialized() const { return initialized_; }
    
    // Socket operations
    uint32_t CreateSocket(int af, int type, int protocol);
    bool CloseSocket(uint32_t handle);
    native_socket_t GetNativeHandle(uint32_t handle);
    
    // Socket configuration
    int SetSockOpt(uint32_t handle, int level, int optname, 
                   const void* optval, int optlen);
    int IOCtl(uint32_t handle, uint32_t cmd, uint32_t* arg);
    
    // Connection operations
    int Bind(uint32_t handle, const XSOCKADDR_IN* addr, int addrlen);
    int Connect(uint32_t handle, const XSOCKADDR_IN* addr, int addrlen);
    int Listen(uint32_t handle, int backlog);
    uint32_t Accept(uint32_t handle, XSOCKADDR_IN* addr, int* addrlen);
    int Shutdown(uint32_t handle, int how);
    
    // Data operations
    int Send(uint32_t handle, const void* buf, int len, int flags);
    int Recv(uint32_t handle, void* buf, int len, int flags);
    int SendTo(uint32_t handle, const void* buf, int len, int flags,
               const XSOCKADDR_IN* to, int tolen);
    int RecvFrom(uint32_t handle, void* buf, int len, int flags,
                 XSOCKADDR_IN* from, int* fromlen);
    
    // Non-blocking data availability check (for graceful fallback)
    bool HasPendingData(uint32_t handle);
    
    // Select
    int Select(int nfds, void* readfds, void* writefds, 
               void* exceptfds, void* timeout);
    
    // Error handling
    int GetLastError();
    void SetLastError(int error);
    
private:
    SocketManager() = default;
    ~SocketManager();
    
    std::mutex mutex_;
    std::unordered_map<uint32_t, native_socket_t> handles_;
    std::atomic<uint32_t> nextHandle_{0x10000};  // Start above kernel object range
    std::atomic<bool> initialized_{false};
    int lastError_{0};
    
    // Helper to convert Xbox sockaddr to native
    static void XboxToNativeSockaddr(const XSOCKADDR_IN* xbox, sockaddr_in* native);
    static void NativeToXboxSockaddr(const sockaddr_in* native, XSOCKADDR_IN* xbox);
};

// ============================================================================
// API Functions (Called via GUEST_FUNCTION_HOOK)
// ============================================================================

// Winsock layer
uint32_t WSAStartup(uint32_t caller, uint16_t version, XWSADATA* wsaData);
int WSACleanup(uint32_t caller);
int WSAGetLastError(uint32_t caller);

// BSD socket layer
uint32_t Socket(uint32_t caller, uint32_t af, uint32_t type, uint32_t protocol);
int CloseSocket(uint32_t caller, uint32_t socket);
int Shutdown(uint32_t caller, uint32_t socket, int how);
int IOCtlSocket(uint32_t caller, uint32_t socket, uint32_t cmd, uint32_t* arg);
int SetSockOpt(uint32_t caller, uint32_t socket, int level, int optname, 
               const void* optval, int optlen);
int GetSockName(uint32_t caller, uint32_t socket, XSOCKADDR_IN* name, int* namelen);
int Bind(uint32_t caller, uint32_t socket, const XSOCKADDR_IN* addr, int addrlen);
int Connect(uint32_t caller, uint32_t socket, const XSOCKADDR_IN* addr, int addrlen);
int Listen(uint32_t caller, uint32_t socket, int backlog);
uint32_t Accept(uint32_t caller, uint32_t socket, XSOCKADDR_IN* addr, int* addrlen);
int Select(int caller, int nfds, void* readfds, void* writefds, 
           void* exceptfds, void* timeout);
int Recv(uint32_t caller, uint32_t socket, void* buf, int len, int flags);
int Send(uint32_t caller, uint32_t socket, const void* buf, int len, int flags);
int RecvFrom(uint32_t caller, uint32_t socket, void* buf, int len, int flags,
             XSOCKADDR_IN* from, int* fromlen);
int SendTo(uint32_t caller, uint32_t socket, const void* buf, int len, int flags,
           const XSOCKADDR_IN* to, int tolen);
uint32_t InetAddr(uint32_t caller, const char* cp);

// XNet layer
uint32_t XNetStartup(uint32_t caller, XNetStartupParams* params);
int XNetCleanup(uint32_t caller);
uint32_t XNetGetTitleXnAddr(uint32_t caller, XNADDR* addr);
uint32_t XNetGetConnectStatus(uint32_t caller, void* addr);
uint32_t XNetGetEthernetLinkStatus(uint32_t caller);
int XNetXnAddrToInAddr(uint32_t caller, const XNADDR* xnaddr, void* xnkid, uint32_t* inaddr);
int XNetServerToInAddr(uint32_t caller, uint32_t inaddr, uint32_t service, uint32_t* outaddr);
int XNetUnregisterInAddr(uint32_t caller, uint32_t inaddr);
int XNetQosListen(uint32_t caller, void* xnkid, void* pb, uint32_t cb, 
                  uint32_t bitsPerSec, uint32_t flags);
int XNetQosLookup(uint32_t caller, uint32_t cxna, void* apxna, void* apxnkid,
                  void* apxnkey, uint32_t cina, void* aina, void* adwSvcId,
                  uint32_t cProbes, uint32_t dwBitsPerSec, uint32_t flags,
                  void* phEvent, void* ppxnqos);
int XNetQosRelease(uint32_t caller, void* pxnqos);

// XLive Base API (NAT type)
uint32_t XLiveBaseGetNatType(uint32_t caller);

} // namespace Net
