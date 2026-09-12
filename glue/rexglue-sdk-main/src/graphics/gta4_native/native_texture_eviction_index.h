#pragma once

#include <cstddef>
#include <cstdint>
#include <list>
#include <optional>
#include <unordered_map>

namespace rex::graphics::gta4_native {

// Round-robin cache scan containing only live, reproducible image generations.
// Supersession erases its node immediately; dead IDs must not accumulate faster
// than a bounded scan can visit them. The render worker owns this entire index.
class NativeTextureEvictionIndex {
 public:
  NativeTextureEvictionIndex() = default;
  NativeTextureEvictionIndex(const NativeTextureEvictionIndex&) = delete;
  NativeTextureEvictionIndex& operator=(const NativeTextureEvictionIndex&) = delete;
  bool Insert(uint64_t generation) {
    if (!generation || positions_.contains(generation)) return false;
    auto node = generations_.insert(generations_.end(), generation);
    positions_.emplace(generation, node);
    return true;
  }
  bool Erase(uint64_t generation) {
    auto found = positions_.find(generation);
    if (found == positions_.end()) return false;
    generations_.erase(found->second);
    positions_.erase(found);
    return true;
  }
  std::optional<uint64_t> Next() {
    if (generations_.empty()) return std::nullopt;
    const uint64_t value = generations_.front();
    generations_.splice(generations_.end(), generations_, generations_.begin());
    return value;
  }
  size_t size() const { return generations_.size(); }
  bool empty() const { return generations_.empty(); }
  void clear() { positions_.clear(); generations_.clear(); }
 private:
  std::list<uint64_t> generations_;
  std::unordered_map<uint64_t, std::list<uint64_t>::iterator> positions_;
};
}  // namespace rex::graphics::gta4_native
