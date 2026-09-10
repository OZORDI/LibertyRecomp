#include "deferred_responses.h"
#include <utility>

namespace libserver::http {

DeferredResponses::DeferredResponses(std::size_t capacity, std::size_t per_owner,
                                     std::size_t writers)
    : capacity_(capacity), per_owner_(per_owner), writer_count_(writers) {}
DeferredResponses::~DeferredResponses() { Shutdown(); }

bool DeferredResponses::Start() {
  std::unique_lock lock(mutex_);
  if (started_ || stopping_ || !capacity_ || !per_owner_ || !writer_count_) return false;
  started_ = true;
  try {
    scheduler_ = std::thread(&DeferredResponses::SchedulerMain, this);
    for (std::size_t i = 0; i < writer_count_; ++i) {
      writers_.emplace_back(&DeferredResponses::WriterMain, this);
    }
  } catch (...) {
    lock.unlock();
    Shutdown();
    return false;
  }
  return true;
}

DeferredResponses::Admission DeferredResponses::Submit(
    Connection& connection, std::string owner, std::string key,
    std::chrono::milliseconds wait, Poll poll) {
  std::lock_guard lock(mutex_);
  if (!started_ || stopping_) return Admission::kStopping;
  if (jobs_.contains(key)) return Admission::kDuplicate;
  const auto count = owner_counts_.find(owner);
  if (jobs_.size() >= capacity_ ||
      (count != owner_counts_.end() && count->second >= per_owner_) ||
      wait.count() <= 0 || !poll) return Admission::kFull;
  auto job = std::make_shared<Job>();
  job->owner = std::move(owner);
  job->key = std::move(key);
  job->expires = std::chrono::steady_clock::now() + wait;
  job->poll = std::move(poll);
  // Complete all allocations before transferring the worker's connection.
  jobs_.emplace(job->key, job);
  try {
    ++owner_counts_[job->owner];
    job->connection = connection.Detach();
  } catch (...) {
    jobs_.erase(job->key);
    auto c = owner_counts_.find(job->owner);
    if (c != owner_counts_.end() && c->second && --c->second == 0) owner_counts_.erase(c);
    throw;
  }
  condition_.notify_all();
  return Admission::kAccepted;
}

std::size_t DeferredResponses::pending() const {
  std::lock_guard lock(mutex_);
  return jobs_.size();
}

void DeferredResponses::SchedulerMain() {
  for (;;) {
    std::vector<std::shared_ptr<Job>> polling;
    {
      std::unique_lock lock(mutex_);
      if (jobs_.empty()) condition_.wait(lock, [&] { return stopping_ || !jobs_.empty(); });
      if (stopping_) return;
      for (const auto& [key, job] : jobs_) {
        (void)key;
        if (job->phase == Job::Phase::kPolling) polling.push_back(job);
      }
    }
    for (const auto& job : polling) {
      {
        std::lock_guard lock(mutex_);
        if (stopping_) return;
      }
      std::optional<std::string> response;
      try {
        response = job->poll(std::chrono::steady_clock::now() >= job->expires);
      } catch (...) {
        response = "HTTP/1.1 500 Internal Server Error\r\nConnection: close\r\nContent-Length: 0\r\n\r\n";
      }
      if (!response) continue;
      std::lock_guard lock(mutex_);
      if (stopping_) return;
      job->response = std::move(*response);
      job->poll = {};
      job->phase = Job::Phase::kQueued;
      writes_.push_back(job);
      condition_.notify_all();
    }
    std::unique_lock lock(mutex_);
    condition_.wait_for(lock, std::chrono::milliseconds(5), [&] { return stopping_; });
    if (stopping_) return;
  }
}

void DeferredResponses::WriterMain() {
  for (;;) {
    std::shared_ptr<Job> job;
    {
      std::unique_lock lock(mutex_);
      condition_.wait(lock, [&] { return stopping_ || !writes_.empty(); });
      if (stopping_) return;
      job = std::move(writes_.front());
      writes_.pop_front();
      job->phase = Job::Phase::kWriting;
    }
    try {
      job->connection->WriteAll(job->response, std::chrono::milliseconds(1000));
      job->connection->Shutdown(std::chrono::milliseconds::zero());
    } catch (...) {
      ShutdownSocket(job->connection->socket());
    }
    {
      std::lock_guard lock(mutex_);
      jobs_.erase(job->key);
      auto count = owner_counts_.find(job->owner);
      if (count != owner_counts_.end() && --count->second == 0) owner_counts_.erase(count);
    }
    // Connection destruction closes its owned socket only after the job is no
    // longer in the shutdown registry, preventing descriptor reuse races.
  }
}

void DeferredResponses::Shutdown() {
  {
    std::lock_guard lock(mutex_);
    if (!started_) return;
    stopping_ = true;
    for (const auto& [key, job] : jobs_) {
      (void)key;
      if (job->connection) ShutdownSocket(job->connection->socket());
    }
  }
  condition_.notify_all();
  if (scheduler_.joinable()) scheduler_.join();
  for (auto& writer : writers_) if (writer.joinable()) writer.join();
  std::lock_guard lock(mutex_);
  writes_.clear();
  jobs_.clear();
  owner_counts_.clear();
  writers_.clear();
  started_ = false;
}
}  // namespace libserver::http
