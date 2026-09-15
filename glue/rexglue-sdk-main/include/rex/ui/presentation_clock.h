#pragma once
#include <array>
#include <cstdint>
#include <optional>
#include <rex/ui/frame_pacer.h>

namespace rex::ui {
// Bracketed correlation, not an assumption that steady_clock and WSI share an
// epoch. Unknown platform clocks remain unsupported. Reject slow clock reads.
class PresentationClockMapping {
 public:
  bool Sample(uint64_t host_before, uint64_t driver, uint64_t host_after) {
    if (!driver || host_after < host_before || host_after-host_before > 250'000) return false;
    host_ = host_before + (host_after-host_before)/2;
    driver_ = driver;
    uncertainty_ = (host_after-host_before)/2;
    valid_ = true;
    return true;
  }
  uint64_t ToDriver(uint64_t host) const {
    if (!valid_) return 0;
    if (host >= host_) return FramePacer::Add(driver_, host-host_);
    return driver_ >= host_-host ? driver_-(host_-host) : 0;
  }
  uint64_t ToHost(uint64_t driver) const {
    if (!valid_) return 0;
    if (driver >= driver_) return FramePacer::Add(host_, driver-driver_);
    return host_ >= driver_-driver ? host_-(driver_-driver) : 0;
  }
  bool valid() const { return valid_; }
  uint64_t uncertainty() const { return uncertainty_; }
 private:
  uint64_t host_ = 0, driver_ = 0, uncertainty_ = 0;
  bool valid_ = false;
};

class PresentationFeedbackHistory {
 public:
  struct Record {
    uint32_t id = 0, frame = 0, fps = 0;
    uint64_t generation = 0, queue_host_ns = 0, desired_driver_ns = 0, serial = 0;
  };
  uint32_t NewID() {
    const uint32_t id = next_id_++;
    if (!next_id_) ++next_id_;
    return id;
  }
  void Insert(Record record) { records_[record.id % records_.size()] = record; }
  void Clear() { records_ = {}; }
  std::optional<Record> Take(uint32_t id, uint64_t echoed_desired,
                             uint64_t actual_host, uint64_t now_host) {
    auto& r = records_[id % records_.size()];
    if (!id || r.id != id) return std::nullopt;
    const Record result = r;
    r = {};
    if (result.desired_driver_ns != echoed_desired || !actual_host ||
        actual_host < result.queue_host_ns || actual_host > FramePacer::Add(now_host, 1'000'000) ||
        now_host - std::min(now_host, result.queue_host_ns) > 2 * FramePacer::kSecond)
      return std::nullopt;
    return result;
  }
 private:
  std::array<Record, 128> records_{};
  uint32_t next_id_ = 1;
};
}  // namespace rex::ui
