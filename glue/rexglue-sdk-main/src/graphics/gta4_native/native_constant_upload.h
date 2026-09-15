#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <optional>
#include <span>

namespace rex::graphics::gta4_native {

// All constant allocations are contiguous, whole float4 registers. One tracker
// belongs to one host-visible frame-slot buffer. BeginFrame is called only after
// the slot fence (or unsubmitted rollback); ResetStorage is required whenever
// that buffer is replaced. No shader ABI or per-draw allocation is changed.
class NativeConstantUploadTracker {
 public:
  static constexpr size_t kRegisterBytes = 16;

  void BeginFrame() { written_bytes_ = 0; }
  void ResetStorage() {
    initialized_bytes_ = 0;
    written_bytes_ = 0;
  }

  std::optional<size_t> Write(std::span<uint8_t> storage, size_t offset,
                              std::span<const uint8_t> source, bool guest_word_order,
                              bool compare_existing = true) {
    if (source.empty() || offset % kRegisterBytes || source.size() % kRegisterBytes ||
        offset > storage.size() || source.size() > storage.size() - offset ||
        initialized_bytes_ > storage.size() || offset > initialized_bytes_ ||
        source.size() > std::numeric_limits<size_t>::max() - written_bytes_) {
      return std::nullopt;
    }

    size_t written = 0;
    for (size_t relative = 0; relative < source.size(); relative += kRegisterBytes) {
      std::array<uint32_t, kRegisterBytes / sizeof(uint32_t)> words;
      std::memcpy(words.data(), source.data() + relative, kRegisterBytes);
      if (guest_word_order) {
        for (uint32_t& word : words) word = std::byteswap(word);
      }
      const size_t destination = offset + relative;
      // Never inspect uninitialized backing memory. Once written, comparison is
      // bit-exact, including NaN payloads, signed zero and integer fields.
      if (!compare_existing || destination >= initialized_bytes_ ||
          std::memcmp(storage.data() + destination, words.data(), kRegisterBytes) != 0) {
        std::memcpy(storage.data() + destination, words.data(), kRegisterBytes);
        written += kRegisterBytes;
      }
    }
    initialized_bytes_ = std::max(initialized_bytes_, offset + source.size());
    written_bytes_ += written;
    return written;
  }

  size_t initialized_bytes() const { return initialized_bytes_; }
  size_t written_bytes() const { return written_bytes_; }

 private:
  size_t initialized_bytes_ = 0;
  size_t written_bytes_ = 0;
};

}  // namespace rex::graphics::gta4_native
