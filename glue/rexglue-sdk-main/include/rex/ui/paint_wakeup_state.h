#pragma once
#include <atomic>
#include <cstdint>
#include <mutex>

namespace rex::ui {
// A superseded timer/event cannot consume a newer paint request. Timer callbacks
// retain this state, never a Window pointer. The earlier request always wins.
class PaintWakeupState {
 public:
  uint32_t Request(uint64_t due_ns) {
    std::lock_guard lock(mutex_);
    if (stopped_ || (ticket_ && due_ns_ <= due_ns)) return 0;
    uint32_t next = next_ticket_.fetch_add(1, std::memory_order_relaxed);
    if (!next) next = next_ticket_.fetch_add(1, std::memory_order_relaxed);
    ticket_ = next;
    due_ns_ = due_ns;
    return ticket_;
  }
  bool IsCurrent(uint32_t ticket) const {
    std::lock_guard lock(mutex_);
    return !stopped_ && ticket && ticket_ == ticket;
  }
  bool Complete(uint32_t ticket) {
    std::lock_guard lock(mutex_);
    if (stopped_ || !ticket || ticket != ticket_) return false;
    ticket_ = 0;
    return true;
  }
  void Stop() {
    std::lock_guard lock(mutex_);
    stopped_ = true;
    ticket_ = 0;
  }
 private:
  inline static std::atomic<uint32_t> next_ticket_{1};
  mutable std::mutex mutex_;
  uint64_t due_ns_ = 0;
  uint32_t ticket_ = 0;
  bool stopped_ = false;
};
}  // namespace rex::ui
