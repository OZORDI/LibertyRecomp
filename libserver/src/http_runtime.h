#pragma once

#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <mutex>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <openssl/ssl.h>

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#endif

namespace libserver::http {

#if defined(_WIN32)
using Socket = SOCKET;
inline constexpr Socket kInvalidSocket = INVALID_SOCKET;
#else
using Socket = int;
inline constexpr Socket kInvalidSocket = -1;
#endif

// These policy values are verified by tests/runtime_limit_math.py.
inline constexpr std::size_t kMaximumBodyBytes = 2097152;
inline constexpr std::size_t kMaximumHeaderBytes = 32768;
inline constexpr std::size_t kMaximumRequestTargetBytes = 8192;
inline constexpr std::size_t kMaximumHeaderLineBytes = 8192;
inline constexpr std::size_t kMaximumHeaderFields = 100;
inline constexpr std::size_t kDefaultWorkerThreads = 16;
inline constexpr std::size_t kDefaultQueuedConnections = 128;
inline constexpr int kDefaultListenBacklog = 128;
inline constexpr std::chrono::milliseconds kDefaultSocketDeadline{10000};
inline constexpr std::chrono::milliseconds kAcceptPollInterval{250};

struct Request {
  std::string method;
  std::string path;
  std::unordered_map<std::string, std::string> query;
  std::unordered_map<std::string, std::string> headers;
  std::string body;
};

struct Limits {
  std::size_t maximum_body_bytes = kMaximumBodyBytes;
  std::size_t maximum_header_bytes = kMaximumHeaderBytes;
  std::size_t maximum_request_target_bytes = kMaximumRequestTargetBytes;
  std::size_t maximum_header_line_bytes = kMaximumHeaderLineBytes;
  std::size_t maximum_header_fields = kMaximumHeaderFields;
};

struct ReadResult {
  std::optional<Request> request;
  int error_status = 0;
  std::string error_code;
  std::string error_message;

  explicit operator bool() const { return request.has_value(); }
};

enum class WaitResult {
  kReady,
  kTimeout,
  kInterrupted,
  kError,
};

enum class StreamStatus {
  kData,
  kClosed,
  kTimeout,
  kError,
};

struct StreamReadResult {
  StreamStatus status = StreamStatus::kError;
  std::size_t bytes = 0;
};

class Connection {
 public:
  explicit Connection(Socket socket, SSL_CTX* tls_context = nullptr);
  Connection(Connection&& other) noexcept;
  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;
  ~Connection();
  std::shared_ptr<Connection> Detach();

  bool valid() const;
  bool encrypted() const;
  Socket socket() const;
  bool Handshake(std::chrono::milliseconds deadline = kDefaultSocketDeadline);
  StreamReadResult ReadSome(char* buffer, std::size_t capacity,
                            std::chrono::milliseconds deadline);
  bool WriteAll(std::string_view wire,
                std::chrono::milliseconds deadline = kDefaultSocketDeadline);
  void Shutdown(std::chrono::milliseconds deadline = kDefaultSocketDeadline);

 private:
  friend class WorkerPool;
  std::function<void()> on_detach_;
  bool owns_socket_ = false;
  Socket socket_ = kInvalidSocket;
  SSL* ssl_ = nullptr;
  bool tls_requested_ = false;
  bool handshake_complete_ = false;
  bool shutdown_started_ = false;
};

void CloseSocket(Socket socket);
void ShutdownSocket(Socket socket);
bool SetSocketDeadlines(Socket socket, std::chrono::milliseconds deadline);
WaitResult WaitReadable(Socket socket, std::chrono::milliseconds timeout);
ReadResult ParseRequestHead(std::string_view head, const Limits& limits = {});
ReadResult ReadRequest(Connection& connection, const Limits& limits = {},
                       std::chrono::milliseconds deadline = kDefaultSocketDeadline);
bool SendAll(Connection& connection, std::string_view wire,
             std::chrono::milliseconds deadline = kDefaultSocketDeadline);

class WorkerPool {
 public:
  using Handler = std::function<void(Connection&)>;

  WorkerPool(std::size_t worker_count, std::size_t queue_capacity, Handler handler,
             SSL_CTX* tls_context = nullptr);
  WorkerPool(const WorkerPool&) = delete;
  WorkerPool& operator=(const WorkerPool&) = delete;
  ~WorkerPool();

  bool Start();
  bool Enqueue(Socket socket);
  void Shutdown();

  std::size_t queued() const;
  std::size_t active() const;

 private:
  void WorkerMain();

  const std::size_t worker_count_;
  const std::size_t queue_capacity_;
  Handler handler_;
  SSL_CTX* tls_context_ = nullptr;
  mutable std::mutex mutex_;
  std::condition_variable condition_;
  std::deque<Socket> queue_;
  std::unordered_set<Socket> active_;
  std::vector<std::thread> workers_;
  bool started_ = false;
  bool stopping_ = false;
};

}  // namespace libserver::http
