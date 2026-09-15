#pragma once

#include <array>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include <utility>

namespace rex::graphics::gta4_native {

#if defined(__APPLE__)
// Metal may create autoreleased objects even during Vulkan resource teardown.
void WithNativeAutoreleasePool(const std::function<void()>& work);
#endif

// Accepts only resources whose GPU and descriptor lifetimes are already over.
// The bounds include the item currently being destroyed, not just the queue.
// A rejected enqueue leaves ownership with the caller for synchronous release.
template <typename Resource>
class NativeResourceReclaimer {
 public:
  using Destroy = std::function<void(Resource&)>;
  NativeResourceReclaimer(uint64_t byte_limit, size_t count_limit, Destroy destroy)
      : byte_limit_(byte_limit), count_limit_(count_limit), destroy_(std::move(destroy)),
        worker_([this] { Run(); }) {}
  ~NativeResourceReclaimer() {
    { std::lock_guard lock(mutex_); stopping_ = true; }
    condition_.notify_all();
    worker_.join();
  }
  NativeResourceReclaimer(const NativeResourceReclaimer&) = delete;
  NativeResourceReclaimer& operator=(const NativeResourceReclaimer&) = delete;

  bool TryEnqueue(Resource& resource, uint64_t bytes, uint32_t heap,
                  uint64_t last_submission, uint64_t completed_submission) {
    std::lock_guard lock(mutex_);
    if (stopping_ || last_submission > completed_submission || heap >= heap_bytes_.size() ||
        count_ >= count_limit_ || bytes > byte_limit_ || bytes_ > byte_limit_ - bytes)
      return false;
    queue_.push_back({std::move(resource), bytes, heap});
    bytes_ += bytes;
    heap_bytes_[heap] += bytes;
    ++count_;
    condition_.notify_all();
    return true;
  }
  // Exceptional allocation recovery may wait for already-detached memory.
  void Drain() {
    std::unique_lock lock(mutex_);
    condition_.wait(lock, [this] { return count_ == 0; });
  }
  struct Usage {
    uint64_t bytes = 0;
    size_t count = 0;
    std::array<uint64_t, 32> heaps{};
  };
  Usage usage() const {
    std::lock_guard lock(mutex_);
    return {bytes_, count_, heap_bytes_};
  }

 private:
  struct Entry { Resource resource; uint64_t bytes; uint32_t heap; };
  void Run() {
    for (;;) {
      std::unique_lock lock(mutex_);
      condition_.wait(lock, [this] { return stopping_ || !queue_.empty(); });
      if (queue_.empty()) return;
      Entry entry = std::move(queue_.front());
      queue_.pop_front();
      lock.unlock();
      destroy_(entry.resource);
      lock.lock();
      bytes_ -= entry.bytes;
      heap_bytes_[entry.heap] -= entry.bytes;
      --count_;
      if (!count_) condition_.notify_all();
    }
  }
  const uint64_t byte_limit_;
  const size_t count_limit_;
  Destroy destroy_;
  mutable std::mutex mutex_;
  std::condition_variable condition_;
  std::deque<Entry> queue_;
  uint64_t bytes_ = 0;
  size_t count_ = 0;
  std::array<uint64_t, 32> heap_bytes_{};
  bool stopping_ = false;
  std::thread worker_;
};
}  // namespace rex::graphics::gta4_native
