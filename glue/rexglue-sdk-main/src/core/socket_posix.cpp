#include <rex/net/socket.h>
#include <rex/platform.h>

static_assert(REX_PLATFORM_LINUX || REX_PLATFORM_MAC, "This file is POSIX-only");

#include <cerrno>
#include <sys/ioctl.h>
#include <unistd.h>

namespace rex::net {

int socket_close(SocketHandle handle) {
  return close(static_cast<int>(handle));
}

int socket_ioctl(SocketHandle handle, uint32_t cmd, uint8_t* arg) {
  return ioctl(static_cast<int>(handle), cmd, arg);
}

uint32_t socket_last_error() {
  // Xbox socket APIs expose the WinSock error namespace even when RexGlue is
  // hosted by a POSIX platform. Keep this translation next to the operation
  // that captures errno so title code observes the same retry/error contract.
  switch (errno) {
    case EINTR:
      return 10004;  // WSAEINTR
    case EACCES:
      return 10013;  // WSAEACCES
    case EFAULT:
      return 10014;  // WSAEFAULT
    case EINVAL:
      return 10022;  // WSAEINVAL
    case EMFILE:
      return 10024;  // WSAEMFILE
    case EAGAIN:
      return 10035;  // WSAEWOULDBLOCK
#if EWOULDBLOCK != EAGAIN
    case EWOULDBLOCK:
      return 10035;  // WSAEWOULDBLOCK
#endif
    case EINPROGRESS:
      return 10036;  // WSAEINPROGRESS
    case EALREADY:
      return 10037;  // WSAEALREADY
    case ENOTSOCK:
      return 10038;  // WSAENOTSOCK
    case EDESTADDRREQ:
      return 10039;  // WSAEDESTADDRREQ
    case EMSGSIZE:
      return 10040;  // WSAEMSGSIZE
    case EPROTOTYPE:
      return 10041;  // WSAEPROTOTYPE
    case ENOPROTOOPT:
      return 10042;  // WSAENOPROTOOPT
    case EPROTONOSUPPORT:
      return 10043;  // WSAEPROTONOSUPPORT
#ifdef ESOCKTNOSUPPORT
    case ESOCKTNOSUPPORT:
      return 10044;  // WSAESOCKTNOSUPPORT
#endif
    case EOPNOTSUPP:
      return 10045;  // WSAEOPNOTSUPP
#if defined(ENOTSUP) && ENOTSUP != EOPNOTSUPP
    case ENOTSUP:
      return 10045;  // WSAEOPNOTSUPP
#endif
#ifdef EPFNOSUPPORT
    case EPFNOSUPPORT:
      return 10046;  // WSAEPFNOSUPPORT
#endif
    case EAFNOSUPPORT:
      return 10047;  // WSAEAFNOSUPPORT
    case EADDRINUSE:
      return 10048;  // WSAEADDRINUSE
    case EADDRNOTAVAIL:
      return 10049;  // WSAEADDRNOTAVAIL
    case ENETDOWN:
      return 10050;  // WSAENETDOWN
    case ENETUNREACH:
      return 10051;  // WSAENETUNREACH
    case ENETRESET:
      return 10052;  // WSAENETRESET
    case ECONNABORTED:
      return 10053;  // WSAECONNABORTED
    case ECONNRESET:
      return 10054;  // WSAECONNRESET
    case ENOBUFS:
      return 10055;  // WSAENOBUFS
    case EISCONN:
      return 10056;  // WSAEISCONN
    case ENOTCONN:
      return 10057;  // WSAENOTCONN
    case ESHUTDOWN:
      return 10058;  // WSAESHUTDOWN
#ifdef ETOOMANYREFS
    case ETOOMANYREFS:
      return 10059;  // WSAETOOMANYREFS
#endif
    case ETIMEDOUT:
      return 10060;  // WSAETIMEDOUT
    case ECONNREFUSED:
      return 10061;  // WSAECONNREFUSED
    case ELOOP:
      return 10062;  // WSAELOOP
    case ENAMETOOLONG:
      return 10063;  // WSAENAMETOOLONG
#ifdef EHOSTDOWN
    case EHOSTDOWN:
      return 10064;  // WSAEHOSTDOWN
#endif
    case EHOSTUNREACH:
      return 10065;  // WSAEHOSTUNREACH
    case ENOTEMPTY:
      return 10066;  // WSAENOTEMPTY
#ifdef EUSERS
    case EUSERS:
      return 10068;  // WSAEUSERS
#endif
#ifdef EDQUOT
    case EDQUOT:
      return 10069;  // WSAEDQUOT
#endif
#ifdef ESTALE
    case ESTALE:
      return 10070;  // WSAESTALE
#endif
#ifdef EREMOTE
    case EREMOTE:
      return 10071;  // WSAEREMOTE
#endif
    default:
      return 10022;  // WSAEINVAL is the safest title-visible fallback.
  }
}

}  // namespace rex::net
