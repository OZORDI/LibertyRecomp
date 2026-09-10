#pragma once

#include "http_runtime.h"
#include <atomic>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace libserver::http {

// Waiting subscriptions own connections, not request workers. A scheduler
// checks bounded callbacks; a separate bounded writer set handles socket I/O.
// None of these callbacks may block waiting for application events.
class DeferredResponses final {
 public:
  using Poll = std::function<std::optional<std::string>(bool expired)>;
  enum class Admission { kAccepted, kDuplicate, kFull, kStopping };
  DeferredResponses(std::size_t capacity = 512, std::size_t per_owner = 8,
                    std::size_t writers = 4);
  ~DeferredResponses();
  DeferredResponses(const DeferredResponses&) = delete;
  DeferredResponses& operator=(const DeferredResponses&) = delete;
  bool Start();
  Admission Submit(Connection& connection, std::string owner, std::string key,
                   std::chrono::milliseconds wait, Poll poll);
  void Shutdown();
  std::size_t pending() const;

 private:
  struct Job {
    std::shared_ptr<Connection> connection;
    std::string owner;
    std::string key;
    std::chrono::steady_clock::time_point expires;
    Poll poll;
    std::string response;
    enum class Phase { kPolling, kQueued, kWriting } phase = Phase::kPolling;
  };
  void SchedulerMain();
  void WriterMain();
  const std::size_t capacity_;
  const std::size_t per_owner_;
  const std::size_t writer_count_;
  mutable std::mutex mutex_;
  std::condition_variable condition_;
  bool started_ = false;
  bool stopping_ = false;
  std::unordered_map<std::string, std::shared_ptr<Job>> jobs_;
  std::unordered_map<std::string, std::size_t> owner_counts_;
  std::deque<std::shared_ptr<Job>> writes_;
  std::thread scheduler_;
  std::vector<std::thread> writers_;
};
}  // namespace libserver::http
