#include "http_runtime.h"

#include <algorithm>
#include <array>
#include <charconv>
#include <cctype>
#include <cerrno>
#include <chrono>
#include <limits>
#include <utility>

#if !defined(_WIN32)
#include <arpa/inet.h>
#include <poll.h>
#include <sys/time.h>
#include <unistd.h>
#endif

namespace libserver::http {
namespace {

using Clock = std::chrono::steady_clock;

ReadResult Error(int status, std::string code, std::string message) {
  return {.error_status = status,
          .error_code = std::move(code),
          .error_message = std::move(message)};
}

bool IsTokenCharacter(unsigned char value) {
  if (std::isalnum(value)) return true;
  constexpr std::string_view kTokenPunctuation = "!#$%&'*+-.^_`|~";
  return kTokenPunctuation.find(static_cast<char>(value)) != std::string_view::npos;
}

bool IsValidFieldValue(std::string_view value) {
  return std::all_of(value.begin(), value.end(), [](unsigned char character) {
    return character == '\t' || (character >= 0x20 && character != 0x7f);
  });
}

std::string_view TrimOptionalWhitespace(std::string_view value) {
  while (!value.empty() && (value.front() == ' ' || value.front() == '\t')) {
    value.remove_prefix(1);
  }
  while (!value.empty() && (value.back() == ' ' || value.back() == '\t')) {
    value.remove_suffix(1);
  }
  return value;
}

std::string Lower(std::string_view value) {
  std::string result(value);
  std::transform(result.begin(), result.end(), result.begin(), [](unsigned char character) {
    return static_cast<char>(std::tolower(character));
  });
  return result;
}

std::unordered_map<std::string, std::string> ParseQuery(std::string_view query) {
  std::unordered_map<std::string, std::string> result;
  while (!query.empty()) {
    const std::size_t separator = query.find('&');
    const std::string_view field = query.substr(0, separator);
    const std::size_t equals = field.find('=');
    result.emplace(std::string(field.substr(0, equals)),
                   equals == std::string_view::npos
                       ? std::string{}
                       : std::string(field.substr(equals + 1)));
    if (separator == std::string_view::npos) break;
    query.remove_prefix(separator + 1);
  }
  return result;
}

bool IsValidHost(std::string_view host) {
  if (host.empty()) return false;
  std::string_view name = host;
  std::string_view port;
  if (host.front() == '[') {
    const std::size_t bracket = host.find(']');
    if (bracket == std::string_view::npos || bracket == 1) return false;
    name = host.substr(1, bracket - 1);
    const std::string_view suffix = host.substr(bracket + 1);
    if (!suffix.empty()) {
      if (suffix.front() != ':' || suffix.size() == 1) return false;
      port = suffix.substr(1);
    }
    std::array<unsigned char, 16> binary{};
    std::string address(name);
    if (inet_pton(AF_INET6, address.c_str(), binary.data()) != 1) return false;
  } else {
    const std::size_t colon = host.find(':');
    if (colon != std::string_view::npos) {
      if (host.find(':', colon + 1) != std::string_view::npos || colon == 0 ||
          colon + 1 == host.size()) {
        return false;
      }
      name = host.substr(0, colon);
      port = host.substr(colon + 1);
    }
    for (std::size_t index = 0; index < name.size(); ++index) {
      const unsigned char character = static_cast<unsigned char>(name[index]);
      constexpr std::string_view kRegNamePunctuation = "-._~!$&'()*+;=";
      if (std::isalnum(character) ||
          kRegNamePunctuation.find(static_cast<char>(character)) != std::string_view::npos) {
        continue;
      }
      if (character == '%' && index + 2 < name.size() &&
          std::isxdigit(static_cast<unsigned char>(name[index + 1])) &&
          std::isxdigit(static_cast<unsigned char>(name[index + 2]))) {
        index += 2;
        continue;
      }
      return false;
    }
  }
  if (name.empty()) return false;
  if (!port.empty()) {
    unsigned value = 0;
    const auto parsed = std::from_chars(port.data(), port.data() + port.size(), value);
    if (parsed.ec != std::errc{} || parsed.ptr != port.data() + port.size() ||
        value > std::numeric_limits<std::uint16_t>::max()) {
      return false;
    }
  }
  return true;
}

bool IsValidRequestTarget(std::string_view target) {
  for (std::size_t index = 0; index < target.size(); ++index) {
    const unsigned char character = static_cast<unsigned char>(target[index]);
    if (character <= 0x20 || character == 0x7f || character == '#') return false;
    if (character != '%') continue;
    if (index + 2 >= target.size() ||
        !std::isxdigit(static_cast<unsigned char>(target[index + 1])) ||
        !std::isxdigit(static_cast<unsigned char>(target[index + 2]))) {
      return false;
    }
    index += 2;
  }
  return true;
}

bool SocketCallInterrupted() {
#if defined(_WIN32)
  return WSAGetLastError() == WSAEINTR;
#else
  return errno == EINTR;
#endif
}

bool SocketCallTimedOut() {
#if defined(_WIN32)
  const int error = WSAGetLastError();
  return error == WSAETIMEDOUT || error == WSAEWOULDBLOCK;
#else
  return errno == EAGAIN || errno == EWOULDBLOCK || errno == ETIMEDOUT;
#endif
}

WaitResult WaitForSocket(Socket socket, std::chrono::milliseconds timeout, bool writable) {
  if (timeout.count() < 0) return WaitResult::kError;
#if defined(_WIN32)
  fd_set descriptors;
  FD_ZERO(&descriptors);
  FD_SET(socket, &descriptors);
  const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(timeout);
  const auto microseconds =
      std::chrono::duration_cast<std::chrono::microseconds>(timeout - seconds);
  timeval value{};
  value.tv_sec = static_cast<decltype(value.tv_sec)>(seconds.count());
  value.tv_usec = static_cast<decltype(value.tv_usec)>(microseconds.count());
  const int selected = writable
                           ? select(0, nullptr, &descriptors, nullptr, &value)
                           : select(0, &descriptors, nullptr, nullptr, &value);
#else
  if (timeout.count() > std::numeric_limits<int>::max()) return WaitResult::kError;
  pollfd descriptor{.fd = socket,
                    .events = static_cast<short>(writable ? POLLOUT : POLLIN),
                    .revents = 0};
  const int selected = poll(&descriptor, 1, static_cast<int>(timeout.count()));
#endif
  if (selected > 0) return WaitResult::kReady;
  if (selected == 0) return WaitResult::kTimeout;
  return SocketCallInterrupted() ? WaitResult::kInterrupted : WaitResult::kError;
}

std::optional<std::chrono::milliseconds> RemainingUntil(Clock::time_point expires) {
  const auto now = Clock::now();
  if (now >= expires) return std::nullopt;
  auto remaining = std::chrono::duration_cast<std::chrono::milliseconds>(expires - now);
  if (remaining.count() == 0) remaining = std::chrono::milliseconds(1);
  return remaining;
}

}  // namespace

Connection::Connection(Socket socket, SSL_CTX* tls_context)
    : socket_(socket), tls_requested_(tls_context != nullptr) {
  if (!tls_context || socket == kInvalidSocket) return;
  ssl_ = SSL_new(tls_context);
  if (!ssl_ || SSL_set_fd(ssl_, static_cast<int>(socket_)) != 1) {
    if (ssl_) SSL_free(ssl_);
    ssl_ = nullptr;
    return;
  }
  SSL_set_accept_state(ssl_);
}

Connection::Connection(Connection&& other) noexcept
    : on_detach_(std::move(other.on_detach_)),
      owns_socket_(std::exchange(other.owns_socket_, false)),
      socket_(std::exchange(other.socket_, kInvalidSocket)),
      ssl_(std::exchange(other.ssl_, nullptr)),
      tls_requested_(other.tls_requested_),
      handshake_complete_(other.handshake_complete_),
      shutdown_started_(other.shutdown_started_) {}

Connection::~Connection() {
  if (ssl_) SSL_free(ssl_);
  if (owns_socket_) CloseSocket(socket_);
}

std::shared_ptr<Connection> Connection::Detach() {
  auto detached = std::make_shared<Connection>(std::move(*this));
  detached->owns_socket_ = true;
  if (detached->on_detach_) detached->on_detach_();
  detached->on_detach_ = {};
  return detached;
}

bool Connection::valid() const {
  return socket_ != kInvalidSocket && (!tls_requested_ || ssl_ != nullptr);
}

bool Connection::encrypted() const { return ssl_ != nullptr; }

Socket Connection::socket() const { return socket_; }

bool Connection::Handshake(std::chrono::milliseconds deadline) {
  if (!valid() || deadline.count() <= 0) return false;
  if (!ssl_) {
    handshake_complete_ = true;
    return true;
  }
  if (handshake_complete_) return true;

  const auto expires = Clock::now() + deadline;
  while (true) {
    const auto remaining = RemainingUntil(expires);
    if (!remaining || !SetSocketDeadlines(socket_, *remaining)) return false;
    const int accepted = SSL_accept(ssl_);
    if (accepted == 1) {
      handshake_complete_ = true;
      return true;
    }
    const int error = SSL_get_error(ssl_, accepted);
    if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE) {
      const WaitResult readiness =
          WaitForSocket(socket_, *remaining, error == SSL_ERROR_WANT_WRITE);
      if (readiness == WaitResult::kInterrupted) continue;
      if (readiness == WaitResult::kReady) continue;
      return false;
    }
    if (error == SSL_ERROR_SYSCALL && SocketCallInterrupted()) continue;
    return false;
  }
}

StreamReadResult Connection::ReadSome(char* buffer, std::size_t capacity,
                                      std::chrono::milliseconds deadline) {
  if (!valid() || !handshake_complete_ || !buffer || capacity == 0 ||
      deadline.count() <= 0) {
    return {};
  }
  const auto expires = Clock::now() + deadline;
  bool wait_writable = false;
  while (true) {
    const auto remaining = RemainingUntil(expires);
    if (!remaining || !SetSocketDeadlines(socket_, *remaining)) {
      return {.status = StreamStatus::kTimeout};
    }
    if (!ssl_ || wait_writable || SSL_pending(ssl_) == 0) {
      const WaitResult readiness = WaitForSocket(socket_, *remaining, wait_writable);
      if (readiness == WaitResult::kInterrupted) continue;
      if (readiness == WaitResult::kTimeout) return {.status = StreamStatus::kTimeout};
      if (readiness != WaitResult::kReady) return {};
    }

    if (ssl_) {
      std::size_t received = 0;
      const int result = SSL_read_ex(ssl_, buffer, capacity, &received);
      if (result == 1) return {.status = StreamStatus::kData, .bytes = received};
      const int error = SSL_get_error(ssl_, result);
      if (error == SSL_ERROR_ZERO_RETURN ||
          (error == SSL_ERROR_SYSCALL && result == 0)) {
        return {.status = StreamStatus::kClosed};
      }
      if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE) {
        wait_writable = error == SSL_ERROR_WANT_WRITE;
        continue;
      }
      if (error == SSL_ERROR_SYSCALL && SocketCallInterrupted()) continue;
      if (error == SSL_ERROR_SYSCALL && SocketCallTimedOut()) {
        return {.status = StreamStatus::kTimeout};
      }
      return {};
    }

#if defined(_WIN32)
    const int length = static_cast<int>(
        std::min(capacity, static_cast<std::size_t>(std::numeric_limits<int>::max())));
    const int received = recv(socket_, buffer, length, 0);
#else
    const ssize_t received = recv(socket_, buffer, capacity, 0);
#endif
    if (received > 0) {
      return {.status = StreamStatus::kData,
              .bytes = static_cast<std::size_t>(received)};
    }
    if (received == 0) return {.status = StreamStatus::kClosed};
    if (SocketCallInterrupted()) continue;
    return {.status = SocketCallTimedOut() ? StreamStatus::kTimeout
                                           : StreamStatus::kError};
  }
}

bool Connection::WriteAll(std::string_view wire, std::chrono::milliseconds deadline) {
  if (!valid() || !handshake_complete_ || deadline.count() <= 0) return false;
  const auto expires = Clock::now() + deadline;
  std::size_t sent = 0;
  bool wait_writable = true;
  while (sent < wire.size()) {
    const auto remaining = RemainingUntil(expires);
    if (!remaining || !SetSocketDeadlines(socket_, *remaining)) return false;
    const WaitResult readiness = WaitForSocket(socket_, *remaining, wait_writable);
    if (readiness == WaitResult::kInterrupted) continue;
    if (readiness != WaitResult::kReady) return false;

    if (ssl_) {
      std::size_t written = 0;
      const int result = SSL_write_ex(ssl_, wire.data() + sent, wire.size() - sent,
                                      &written);
      if (result == 1) {
        sent += written;
        wait_writable = true;
        continue;
      }
      const int error = SSL_get_error(ssl_, result);
      if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE) {
        wait_writable = error == SSL_ERROR_WANT_WRITE;
        continue;
      }
      if (error == SSL_ERROR_SYSCALL && SocketCallInterrupted()) continue;
      return false;
    }

    const std::size_t remaining_bytes = wire.size() - sent;
#if defined(_WIN32)
    const int length = static_cast<int>(
        std::min(remaining_bytes,
                 static_cast<std::size_t>(std::numeric_limits<int>::max())));
    const int written = send(socket_, wire.data() + sent, length, 0);
#else
    int flags = 0;
#if defined(MSG_NOSIGNAL)
    flags |= MSG_NOSIGNAL;
#endif
    const ssize_t written = send(socket_, wire.data() + sent, remaining_bytes, flags);
#endif
    if (written <= 0) {
      if (written < 0 && SocketCallInterrupted()) continue;
      return false;
    }
    sent += static_cast<std::size_t>(written);
  }
  return true;
}

void Connection::Shutdown(std::chrono::milliseconds deadline) {
  if (shutdown_started_) return;
  shutdown_started_ = true;
  if (ssl_ && handshake_complete_ && deadline.count() > 0) {
    const auto expires = Clock::now() + deadline;
    while (true) {
      const auto remaining = RemainingUntil(expires);
      if (!remaining || !SetSocketDeadlines(socket_, *remaining)) break;
      const int result = SSL_shutdown(ssl_);
      if (result == 1) break;
      if (result == 0) {
        const WaitResult readiness = WaitForSocket(socket_, *remaining, false);
        if (readiness == WaitResult::kInterrupted || readiness == WaitResult::kReady) continue;
        break;
      }
      const int error = SSL_get_error(ssl_, result);
      if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE) {
        const WaitResult readiness =
            WaitForSocket(socket_, *remaining, error == SSL_ERROR_WANT_WRITE);
        if (readiness == WaitResult::kInterrupted || readiness == WaitResult::kReady) continue;
      }
      break;
    }
  }
  ShutdownSocket(socket_);
}

void CloseSocket(Socket socket) {
  if (socket == kInvalidSocket) return;
#if defined(_WIN32)
  closesocket(socket);
#else
  close(socket);
#endif
}

void ShutdownSocket(Socket socket) {
  if (socket == kInvalidSocket) return;
#if defined(_WIN32)
  shutdown(socket, SD_BOTH);
#else
  shutdown(socket, SHUT_RDWR);
#endif
}

bool SetSocketDeadlines(Socket socket, std::chrono::milliseconds deadline) {
  if (deadline.count() <= 0) return false;
#if defined(_WIN32)
  if (deadline.count() > std::numeric_limits<DWORD>::max()) return false;
  const DWORD milliseconds = static_cast<DWORD>(deadline.count());
  return setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO,
                    reinterpret_cast<const char*>(&milliseconds), sizeof(milliseconds)) == 0 &&
         setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO,
                    reinterpret_cast<const char*>(&milliseconds), sizeof(milliseconds)) == 0;
#else
  const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(deadline);
  const auto microseconds =
      std::chrono::duration_cast<std::chrono::microseconds>(deadline - seconds);
  timeval value{};
  value.tv_sec = static_cast<decltype(value.tv_sec)>(seconds.count());
  value.tv_usec = static_cast<decltype(value.tv_usec)>(microseconds.count());
  if (setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &value, sizeof(value)) != 0 ||
      setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, &value, sizeof(value)) != 0) {
    return false;
  }
#if defined(SO_NOSIGPIPE)
  int enabled = 1;
  if (setsockopt(socket, SOL_SOCKET, SO_NOSIGPIPE, &enabled, sizeof(enabled)) != 0) {
    return false;
  }
#endif
  return true;
#endif
}

WaitResult WaitReadable(Socket socket, std::chrono::milliseconds timeout) {
  return WaitForSocket(socket, timeout, false);
}

ReadResult ParseRequestHead(std::string_view head, const Limits& limits) {
  if (head.size() > limits.maximum_header_bytes ||
      limits.maximum_header_bytes - head.size() < 4) {
    return Error(431, "headers_too_large", "request headers exceed the configured limit");
  }
  if (head.find('\n') != std::string_view::npos) {
    for (std::size_t index = 0; index < head.size(); ++index) {
      if (head[index] == '\n' && (index == 0 || head[index - 1] != '\r')) {
        return Error(400, "malformed_headers", "header lines must use CRLF");
      }
      if (head[index] == '\r' &&
          (index + 1 >= head.size() || head[index + 1] != '\n')) {
        return Error(400, "malformed_headers", "header lines must use CRLF");
      }
    }
  } else if (head.find('\r') != std::string_view::npos) {
    return Error(400, "malformed_headers", "header lines must use CRLF");
  }

  const std::size_t request_line_end = head.find("\r\n");
  const std::string_view request_line = head.substr(0, request_line_end);
  if (request_line.empty() || request_line.size() > limits.maximum_header_line_bytes) {
    return Error(400, "malformed_request_line", "request line is malformed");
  }
  const std::size_t first_space = request_line.find(' ');
  const std::size_t second_space =
      first_space == std::string_view::npos
          ? std::string_view::npos
          : request_line.find(' ', first_space + 1);
  if (first_space == std::string_view::npos || second_space == std::string_view::npos ||
      first_space == 0 || second_space == first_space + 1 ||
      request_line.find(' ', second_space + 1) != std::string_view::npos) {
    return Error(400, "malformed_request_line", "request line is malformed");
  }

  const std::string_view method = request_line.substr(0, first_space);
  const std::string_view target =
      request_line.substr(first_space + 1, second_space - first_space - 1);
  const std::string_view version = request_line.substr(second_space + 1);
  if (!std::all_of(method.begin(), method.end(), IsTokenCharacter)) {
    return Error(400, "invalid_method", "HTTP method is invalid");
  }
  if (version != "HTTP/1.1") {
    return Error(505, "http_version_not_supported", "only HTTP/1.1 is supported");
  }
  if (target.size() > limits.maximum_request_target_bytes) {
    return Error(414, "request_target_too_long", "request target exceeds the configured limit");
  }
  if (target.empty() || target.front() != '/' || !IsValidRequestTarget(target)) {
    return Error(400, "invalid_request_target", "request target must use origin form");
  }

  Request request;
  request.method = std::string(method);
  const std::size_t question = target.find('?');
  request.path = std::string(target.substr(0, question));
  if (question != std::string_view::npos) {
    request.query = ParseQuery(target.substr(question + 1));
  }

  bool saw_host = false;
  bool saw_content_length = false;
  bool saw_transfer_encoding = false;
  std::size_t content_length = 0;
  std::size_t field_count = 0;
  std::size_t cursor = request_line_end == std::string_view::npos
                           ? head.size()
                           : request_line_end + 2;
  while (cursor < head.size()) {
    const std::size_t line_end = head.find("\r\n", cursor);
    const std::size_t end = line_end == std::string_view::npos ? head.size() : line_end;
    const std::string_view line = head.substr(cursor, end - cursor);
    if (line.empty() || line.size() > limits.maximum_header_line_bytes ||
        line.front() == ' ' || line.front() == '\t' || ++field_count > limits.maximum_header_fields) {
      return Error(400, "malformed_headers", "request header field is malformed");
    }
    const std::size_t colon = line.find(':');
    if (colon == std::string_view::npos || colon == 0 ||
        !std::all_of(line.begin(), line.begin() + static_cast<std::ptrdiff_t>(colon),
                     IsTokenCharacter)) {
      return Error(400, "malformed_headers", "request header field is malformed");
    }
    const std::string name = Lower(line.substr(0, colon));
    const std::string_view value = TrimOptionalWhitespace(line.substr(colon + 1));
    if (!IsValidFieldValue(value)) {
      return Error(400, "malformed_headers", "request header value is malformed");
    }

    if (name == "host") {
      if (saw_host || !IsValidHost(value)) {
        return Error(400, "invalid_host", "exactly one valid Host header is required");
      }
      saw_host = true;
    } else if (name == "content-length") {
      if (saw_content_length || value.empty() ||
          !std::all_of(value.begin(), value.end(), [](unsigned char character) {
            return std::isdigit(character);
          })) {
        return Error(400, "invalid_content_length",
                     "Content-Length must be a single decimal value");
      }
      saw_content_length = true;
      const auto parsed = std::from_chars(value.data(), value.data() + value.size(), content_length);
      if (parsed.ec == std::errc::result_out_of_range ||
          parsed.ptr != value.data() + value.size()) {
        return Error(413, "payload_too_large", "request body exceeds the configured limit");
      }
      if (content_length > limits.maximum_body_bytes) {
        return Error(413, "payload_too_large", "request body exceeds the configured limit");
      }
    } else if (name == "transfer-encoding") {
      saw_transfer_encoding = true;
    } else if (name == "authorization" && request.headers.contains(name)) {
      return Error(400, "duplicate_authorization", "Authorization must not be repeated");
    }

    const auto existing = request.headers.find(name);
    if (existing == request.headers.end()) {
      request.headers.emplace(name, std::string(value));
    } else if (name != "host" && name != "content-length" && name != "authorization") {
      existing->second.append(", ");
      existing->second.append(value);
    }
    cursor = line_end == std::string_view::npos ? head.size() : line_end + 2;
  }
  if (!saw_host) {
    return Error(400, "missing_host", "HTTP/1.1 requests require a Host header");
  }
  if (saw_transfer_encoding) {
    return Error(saw_content_length ? 400 : 501,
                 saw_content_length ? "ambiguous_framing" : "transfer_encoding_not_supported",
                 saw_content_length
                     ? "Transfer-Encoding and Content-Length cannot be combined"
                     : "Transfer-Encoding is not supported");
  }
  request.body.reserve(content_length);
  return {.request = std::move(request)};
}

ReadResult ReadRequest(Connection& connection, const Limits& limits,
                       std::chrono::milliseconds deadline) {
  if (!connection.valid() || deadline.count() <= 0) {
    return Error(0, "socket_configuration_failed", "connection deadline is invalid");
  }
  const auto expires = Clock::now() + deadline;
  std::string wire;
  wire.reserve(4096);
  std::array<char, 8192> buffer{};
  std::size_t header_end = std::string::npos;
  while ((header_end = wire.find("\r\n\r\n")) == std::string::npos) {
    const auto remaining = RemainingUntil(expires);
    if (!remaining) {
      return Error(408, "request_timeout",
                   "request headers were not received before the deadline");
    }
    const StreamReadResult read = connection.ReadSome(buffer.data(), buffer.size(), *remaining);
    if (read.status == StreamStatus::kClosed) {
      return Error(0, "connection_closed", "connection closed before the request completed");
    }
    if (read.status == StreamStatus::kTimeout) {
      return Error(408, "request_timeout",
                   "request headers were not received before the deadline");
    }
    if (read.status != StreamStatus::kData) {
      return Error(0, "socket_read_failed", "request connection read failed");
    }
    wire.append(buffer.data(), read.bytes);
    const std::size_t complete_head = wire.find("\r\n\r\n");
    const std::size_t scan_size =
        complete_head == std::string::npos ? wire.size() : complete_head;
    for (std::size_t index = 0; index < scan_size; ++index) {
      if (wire[index] == '\n' && (index == 0 || wire[index - 1] != '\r')) {
        return Error(400, "malformed_headers", "header lines must use CRLF");
      }
      if (wire[index] == '\r' && index + 1 < scan_size && wire[index + 1] != '\n') {
        return Error(400, "malformed_headers", "header lines must use CRLF");
      }
    }
    if (wire.find("\r\n\r\n") == std::string::npos &&
        wire.size() > limits.maximum_header_bytes) {
      return Error(431, "headers_too_large", "request headers exceed the configured limit");
    }
  }
  if (header_end > limits.maximum_header_bytes ||
      limits.maximum_header_bytes - header_end < 4) {
    return Error(431, "headers_too_large", "request headers exceed the configured limit");
  }

  ReadResult parsed = ParseRequestHead(std::string_view(wire).substr(0, header_end), limits);
  if (!parsed) return parsed;
  Request& request = *parsed.request;
  std::size_t content_length = 0;
  if (const auto found = request.headers.find("content-length");
      found != request.headers.end()) {
    const auto converted = std::from_chars(found->second.data(),
                                           found->second.data() + found->second.size(),
                                           content_length);
    if (converted.ec != std::errc{} ||
        converted.ptr != found->second.data() + found->second.size()) {
      return Error(400, "invalid_content_length", "Content-Length is invalid");
    }
  }
  const std::size_t body_offset = header_end + 4;
  const std::size_t available = wire.size() - body_offset;
  request.body.assign(wire.data() + body_offset, std::min(available, content_length));
  while (request.body.size() < content_length) {
    const auto remaining_deadline = RemainingUntil(expires);
    if (!remaining_deadline) {
      return Error(408, "request_timeout",
                   "request body bytes were not received before the deadline");
    }
    const std::size_t remaining = content_length - request.body.size();
    const std::size_t capacity = std::min(remaining, buffer.size());
    const StreamReadResult read =
        connection.ReadSome(buffer.data(), capacity, *remaining_deadline);
    if (read.status == StreamStatus::kClosed) {
      return Error(400, "incomplete_body", "connection closed before the body completed");
    }
    if (read.status == StreamStatus::kTimeout) {
      return Error(408, "request_timeout",
                   "request body was not received before the deadline");
    }
    if (read.status != StreamStatus::kData) {
      return Error(0, "socket_read_failed", "request connection read failed");
    }
    request.body.append(buffer.data(), read.bytes);
  }
  return parsed;
}

bool SendAll(Connection& connection, std::string_view wire,
             std::chrono::milliseconds deadline) {
  return connection.WriteAll(wire, deadline);
}

WorkerPool::WorkerPool(std::size_t worker_count, std::size_t queue_capacity,
                       Handler handler, SSL_CTX* tls_context)
    : worker_count_(worker_count),
      queue_capacity_(queue_capacity),
      handler_(std::move(handler)),
      tls_context_(tls_context) {}

WorkerPool::~WorkerPool() { Shutdown(); }

bool WorkerPool::Start() {
  std::unique_lock lock(mutex_);
  if (started_ || stopping_ || worker_count_ == 0 || queue_capacity_ == 0 || !handler_) {
    return false;
  }
  started_ = true;
  try {
    workers_.reserve(worker_count_);
    for (std::size_t index = 0; index < worker_count_; ++index) {
      workers_.emplace_back(&WorkerPool::WorkerMain, this);
    }
    return true;
  } catch (...) {
    stopping_ = true;
    lock.unlock();
    condition_.notify_all();
    for (auto& worker : workers_) {
      if (worker.joinable()) worker.join();
    }
    return false;
  }
}

bool WorkerPool::Enqueue(Socket socket) {
  if (socket == kInvalidSocket) return false;
  std::lock_guard lock(mutex_);
  if (!started_ || stopping_ || queue_.size() >= queue_capacity_) return false;
  queue_.push_back(socket);
  condition_.notify_one();
  return true;
}

void WorkerPool::Shutdown() {
  std::deque<Socket> abandoned;
  std::vector<Socket> active;
  {
    std::lock_guard lock(mutex_);
    if (!started_ && workers_.empty()) return;
    stopping_ = true;
    abandoned.swap(queue_);
    active.assign(active_.begin(), active_.end());
    for (Socket socket : active) ShutdownSocket(socket);
  }
  for (Socket socket : abandoned) CloseSocket(socket);
  condition_.notify_all();
  for (auto& worker : workers_) {
    if (worker.joinable()) worker.join();
  }
  workers_.clear();
  std::lock_guard lock(mutex_);
  started_ = false;
}

std::size_t WorkerPool::queued() const {
  std::lock_guard lock(mutex_);
  return queue_.size();
}

std::size_t WorkerPool::active() const {
  std::lock_guard lock(mutex_);
  return active_.size();
}

void WorkerPool::WorkerMain() {
  while (true) {
    Socket socket = kInvalidSocket;
    {
      std::unique_lock lock(mutex_);
      condition_.wait(lock, [&] { return stopping_ || !queue_.empty(); });
      if (stopping_ && queue_.empty()) return;
      socket = queue_.front();
      queue_.pop_front();
      active_.insert(socket);
    }
    bool close_socket = true;
    {
      Connection connection(socket, tls_context_);
      connection.on_detach_ = [this, socket] {
        std::lock_guard lock(mutex_);
        active_.erase(socket);
      };
      try {
        if (connection.valid()) handler_(connection);
      } catch (...) {
        // A malformed request must not terminate the worker.
      }
      close_socket = connection.socket() != kInvalidSocket;
    }
    if (close_socket) {
      {
        std::lock_guard lock(mutex_);
        active_.erase(socket);
      }
      CloseSocket(socket);
    }
  }
}

}  // namespace libserver::http
