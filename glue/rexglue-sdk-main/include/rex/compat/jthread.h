/**
 * @file        rex/compat/jthread.h
 * @brief       Minimal std::jthread / std::stop_token polyfill for platforms
 *              where libc++ lacks C++20 jthread support (e.g. Android NDK).
 *
 * This is NOT a full implementation — only enough for the TimerQueue usage:
 *   - stop_token with stop_requested()
 *   - jthread with request_stop(), get_id(), auto-join on destruction
 *   - Callable invoked as fn(stop_token) on construction
 */

#pragma once

#include <atomic>
#include <functional>
#include <thread>
#include <utility>

#if defined(__ANDROID__) && !defined(__cpp_lib_jthread)

namespace std {

class stop_token {
 public:
  stop_token() noexcept : stopped_(nullptr) {}
  explicit stop_token(std::atomic<bool>* flag) noexcept : stopped_(flag) {}

  [[nodiscard]] bool stop_requested() const noexcept {
    return stopped_ && stopped_->load(std::memory_order_acquire);
  }
  [[nodiscard]] bool stop_possible() const noexcept { return stopped_ != nullptr; }

 private:
  std::atomic<bool>* stopped_;
};

class jthread {
 public:
  using id = std::thread::id;

  jthread() noexcept = default;

  template <typename Fn, typename... Args>
  explicit jthread(Fn&& fn, Args&&... args)
      : stopped_(new std::atomic<bool>(false)) {
    auto* flag = stopped_;
    thread_ = std::thread(
        [flag, f = std::forward<Fn>(fn)](auto&&... a) mutable {
          f(stop_token(flag), std::forward<decltype(a)>(a)...);
        },
        std::forward<Args>(args)...);
  }

  ~jthread() {
    if (thread_.joinable()) {
      request_stop();
      thread_.join();
    }
    delete stopped_;
  }

  jthread(jthread&& other) noexcept
      : thread_(std::move(other.thread_)), stopped_(other.stopped_) {
    other.stopped_ = nullptr;
  }

  jthread& operator=(jthread&& other) noexcept {
    if (this != &other) {
      if (thread_.joinable()) {
        request_stop();
        thread_.join();
      }
      delete stopped_;
      thread_ = std::move(other.thread_);
      stopped_ = other.stopped_;
      other.stopped_ = nullptr;
    }
    return *this;
  }

  jthread(const jthread&) = delete;
  jthread& operator=(const jthread&) = delete;

  void request_stop() noexcept {
    if (stopped_) stopped_->store(true, std::memory_order_release);
  }

  [[nodiscard]] id get_id() const noexcept { return thread_.get_id(); }
  [[nodiscard]] bool joinable() const noexcept { return thread_.joinable(); }

 private:
  std::thread thread_;
  std::atomic<bool>* stopped_ = nullptr;
};

}  // namespace std

#endif  // __ANDROID__ && !__cpp_lib_jthread
