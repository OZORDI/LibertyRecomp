#pragma once
// Opt-in observation only. No audio samples, guest state or scheduling decisions
// are modified. Producers publish to bounded nonblocking queues; a separate
// thread performs formatting and file writes.
#include <array>
#include <atomic>
#include <bit>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <type_traits>

namespace rex::audio::handoff {
extern std::atomic<bool> enabled;
inline bool Enabled() noexcept { return enabled.load(std::memory_order_acquire); }
inline uint64_t Clock() noexcept {
  return uint64_t(std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::steady_clock::now().time_since_epoch()).count());
}
// Bounded MPSC queue. Reservation retries are capped, including on the audio
// callback. Contention/full queues drop diagnostics, never wait for the writer.
template<class T, size_t Capacity> class Queue {
  static_assert(Capacity && !(Capacity & (Capacity - 1)));
  static_assert(std::is_trivially_copyable_v<T>);
  struct Cell { std::atomic<uint64_t> sequence; T data; };
  std::array<Cell, Capacity> cells_;
  alignas(64) std::atomic<uint64_t> enqueue_{0};
  alignas(64) uint64_t dequeue_ = 0;
 public:
  std::atomic<uint64_t> dropped{0};
  Queue() { for (size_t i = 0; i < Capacity; ++i) cells_[i].sequence.store(i); }
  bool Push(const T& value) noexcept {
    auto pos = enqueue_.load(std::memory_order_relaxed);
    for (unsigned retry = 0; retry < 8; ++retry) {
      auto& cell = cells_[pos & (Capacity - 1)];
      const auto seq = cell.sequence.load(std::memory_order_acquire);
      const auto difference = int64_t(seq - pos);
      if (!difference) {
        if (enqueue_.compare_exchange_weak(pos,pos+1,std::memory_order_relaxed)) {
          cell.data = value;
          cell.sequence.store(pos+1,std::memory_order_release);
          return true;
        }
      } else if (difference < 0) break;
      else pos = enqueue_.load(std::memory_order_relaxed);
    }
    dropped.fetch_add(1,std::memory_order_relaxed);
    return false;
  }
  bool Pop(T& value) noexcept {
    auto& cell=cells_[dequeue_ & (Capacity-1)];
    if (cell.sequence.load(std::memory_order_acquire) != dequeue_+1) return false;
    value=cell.data;
    cell.sequence.store(dequeue_+Capacity,std::memory_order_release);
    ++dequeue_; return true;
  }
};
struct Signal {
  float peak=0, rms=0, max_step=0, first=0, last=0;
  uint32_t clipped=0, nonfinite=0;
};
Signal Inspect(const float* const* planar, const float* interleaved,
               uint32_t frames,uint32_t channels) noexcept;
uint64_t Record(const char* point,uint64_t id=0,
                std::initializer_list<uint64_t> values={},const char* text=nullptr,
                const Signal* signal=nullptr) noexcept;
void Initialize();
void Shutdown();
void Loading(uint32_t pc,uint32_t lr,bool active,uint32_t state,uint64_t flags) noexcept;
enum class Stage : uint32_t { Guest=0, Converted=1, Mix=2, Output=3, Decoded=4 };
void Capture(Stage stage,const float* interleaved,uint32_t frames,uint32_t channels,
             uint32_t rate,uint64_t id,uint64_t serial,uint64_t position,
             uint64_t auxiliary=0,bool planar_big_endian=false,
             const float* const* planar=nullptr) noexcept;
struct Span {
  const char* label;
  uint64_t id=0,begin=0,sequence=0,arg0=0,arg1=0;
  Span(const char* name,uint64_t key=0,uint64_t a=0,uint64_t b=0) noexcept
      :label(name),id(key),arg0(a),arg1(b) {
    if(Enabled()) {begin=Clock();sequence=Record("span-begin",id,{arg0,arg1},label);}
  }
  ~Span() {if(begin)Record("span-end",id,{sequence,Clock()-begin,arg0,arg1},label);}
};
} // namespace rex::audio::handoff
