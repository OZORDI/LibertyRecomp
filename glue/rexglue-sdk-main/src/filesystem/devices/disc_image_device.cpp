/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2020 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

#include <rex/filesystem/devices/disc_image_device.h>
#include <rex/filesystem/devices/disc_image_entry.h>

#include <limits>

#include <rex/literals.h>
#include <rex/logging.h>
#include <rex/math.h>
#include <rex/memory.h>

namespace rex::filesystem {

using namespace rex::literals;

const size_t kXESectorSize = 2_KiB;
constexpr size_t kVolumeDescriptorSector = 32;
constexpr size_t kVolumeDescriptorSize = 28;
constexpr size_t kMagicSize = 20;
constexpr size_t kDirectoryEntryAlignment = 4;
constexpr size_t kDirectoryEntryHeaderSize = 14;
constexpr size_t kMinimumRootDirectorySize = 13;
constexpr size_t kMaximumRootDirectorySize = 32_MiB;
constexpr size_t kMaximumDirectoryDepth = 1024;
constexpr size_t kMaximumDirectoryEntries = 500000;

bool CheckedSectorOffset(size_t game_offset, size_t sector, size_t& result) {
  if (sector > (std::numeric_limits<size_t>::max() - game_offset) / kXESectorSize) {
    return false;
  }
  result = game_offset + sector * kXESectorSize;
  return true;
}

bool RangeWithin(size_t offset, size_t length, size_t size) {
  return offset <= size && length <= size - offset;
}

DiscImageDevice::DiscImageDevice(const std::string_view mount_path,
                                 const std::filesystem::path& host_path)
    : Device(mount_path), name_("GDFX"), host_path_(host_path) {}

DiscImageDevice::~DiscImageDevice() = default;

bool DiscImageDevice::Initialize() {
  mmap_ = memory::MappedMemory::Open(host_path_, memory::MappedMemory::Mode::kRead);
  if (!mmap_) {
    REXFS_ERROR("Disc image could not be mapped");
    return false;
  }

  ParseState state = {};
  state.ptr = mmap_->data();
  state.size = mmap_->size();
  auto result = Verify(&state);
  if (result != Error::kSuccess) {
    REXFS_ERROR("Failed to verify disc image header: {}", static_cast<int>(result));
    return false;
  }

  result = ReadAllEntries(&state, state.ptr + state.root_offset);
  if (result != Error::kSuccess) {
    REXFS_ERROR("Failed to read all GDFX entries: {}", static_cast<int>(result));
    return false;
  }

  return true;
}

void DiscImageDevice::Dump(string::StringBuffer* string_buffer) {
  auto global_lock = global_critical_region_.Acquire();
  string_buffer->AppendFormat(
      "{}: {} files, {} bytes (game_offset={:#x}, root_sector={}, root_size={}, host_size={})\n",
      mount_path(), file_count_, total_file_size_, disc_info_.game_offset, disc_info_.root_sector,
      disc_info_.root_size, disc_info_.host_size);
}

Entry* DiscImageDevice::ResolvePath(const std::string_view path) {
  // The filesystem will have stripped our prefix off already, so the path will
  // be in the form:
  // some\PATH.foo
  REXFS_DEBUG("DiscImageDevice::ResolvePath({})", path);
  return root_entry_->ResolvePath(path);
}

DiscImageDevice::Error DiscImageDevice::Verify(ParseState* state) {
  // Find sector 32 of the game partition - try at a few points.
  static const size_t likely_offsets[] = {
      0x00000000, 0x0000FB20, 0x00020600, 0x02080000, 0x0FD90000,
  };
  bool magic_found = false;
  for (size_t n = 0; n < rex::countof(likely_offsets); n++) {
    state->game_offset = likely_offsets[n];
    size_t descriptor_offset = 0;
    if (CheckedSectorOffset(state->game_offset, kVolumeDescriptorSector, descriptor_offset) &&
        VerifyMagic(state, descriptor_offset)) {
      magic_found = true;
      break;
    }
  }
  if (!magic_found) {
    // File doesn't have the magic values - likely not a real GDFX source.
    return Error::kErrorFileMismatch;
  }

  // Read sector 32 to get FS state.
  size_t descriptor_offset = 0;
  if (!CheckedSectorOffset(state->game_offset, kVolumeDescriptorSector, descriptor_offset) ||
      !RangeWithin(descriptor_offset, kVolumeDescriptorSize, state->size)) {
    return Error::kErrorReadError;
  }
  uint8_t* fs_ptr = state->ptr + descriptor_offset;
  state->root_sector = memory::load<uint32_t>(fs_ptr + 20);
  state->root_size = memory::load<uint32_t>(fs_ptr + 24);
  if (state->root_size < kMinimumRootDirectorySize ||
      state->root_size > kMaximumRootDirectorySize ||
      !CheckedSectorOffset(state->game_offset, state->root_sector, state->root_offset) ||
      !RangeWithin(state->root_offset, state->root_size, state->size)) {
    return Error::kErrorDamagedFile;
  }

  disc_info_.game_offset = state->game_offset;
  disc_info_.root_sector = state->root_sector;
  disc_info_.root_size = state->root_size;
  disc_info_.host_size = state->size;

  return Error::kSuccess;
}

bool DiscImageDevice::VerifyMagic(ParseState* state, size_t offset) {
  if (!RangeWithin(offset, kMagicSize, state->size)) {
    return false;
  }

  // Simple check to see if the given offset contains the magic value.
  return std::memcmp(state->ptr + offset, "MICROSOFT*XBOX*MEDIA", kMagicSize) == 0;
}

DiscImageDevice::Error DiscImageDevice::ReadAllEntries(ParseState* state,
                                                       const uint8_t* root_buffer) {
  auto root_entry = new DiscImageEntry(this, nullptr, "", mmap_.get());
  root_entry->attributes_ = kFileAttributeDirectory;
  root_entry_ = std::unique_ptr<Entry>(root_entry);

  std::unordered_set<uint16_t> active_ordinals;
  std::unordered_set<uint16_t> visited_ordinals;
  if (!ReadEntry(state, root_buffer, state->root_size, 0, root_entry, active_ordinals,
                 visited_ordinals, 0)) {
    return Error::kErrorDamagedFile;
  }

  return Error::kSuccess;
}

bool DiscImageDevice::ReadEntry(ParseState* state, const uint8_t* buffer, size_t buffer_size,
                                uint16_t entry_ordinal, DiscImageEntry* parent,
                                std::unordered_set<uint16_t>& active_ordinals,
                                std::unordered_set<uint16_t>& visited_ordinals, size_t depth) {
  if (depth > kMaximumDirectoryDepth || state->visited_entries >= kMaximumDirectoryEntries ||
      !active_ordinals.insert(entry_ordinal).second ||
      !visited_ordinals.insert(entry_ordinal).second) {
    return false;
  }
  ++state->visited_entries;

  const size_t ordinal = entry_ordinal;
  if (ordinal > std::numeric_limits<size_t>::max() / kDirectoryEntryAlignment) {
    return false;
  }
  const size_t entry_offset = ordinal * kDirectoryEntryAlignment;
  if (!RangeWithin(entry_offset, kDirectoryEntryHeaderSize, buffer_size)) {
    return false;
  }
  const uint8_t* p = buffer + entry_offset;

  uint16_t node_l = memory::load<uint16_t>(p + 0);
  uint16_t node_r = memory::load<uint16_t>(p + 2);
  size_t sector = memory::load<uint32_t>(p + 4);
  size_t length = memory::load<uint32_t>(p + 8);
  uint8_t attributes = memory::load<uint8_t>(p + 12);
  uint8_t name_length = memory::load<uint8_t>(p + 13);
  if (name_length == 0 ||
      !RangeWithin(entry_offset, kDirectoryEntryHeaderSize + size_t(name_length), buffer_size)) {
    return false;
  }
  auto name_buffer = reinterpret_cast<const char*>(p + 14);

  if (node_l && !ReadEntry(state, buffer, buffer_size, node_l, parent, active_ordinals,
                           visited_ordinals, depth + 1)) {
    return false;
  }

  auto name = std::string(name_buffer, name_length);

  auto entry = DiscImageEntry::Create(this, parent, name, mmap_.get());
  entry->attributes_ = attributes | kFileAttributeReadOnly;
  entry->size_ = length;
  entry->allocation_size_ = rex::round_up(length, bytes_per_sector());

  // Set to January 1, 1970 (UTC) in 100-nanosecond intervals
  entry->create_timestamp_ = 10000 * 11644473600000LL;
  entry->access_timestamp_ = 10000 * 11644473600000LL;
  entry->write_timestamp_ = 10000 * 11644473600000LL;

  if (attributes & kFileAttributeDirectory) {
    // Folder.
    entry->data_offset_ = 0;
    entry->data_size_ = 0;
    if (length) {
      size_t folder_offset = 0;
      if (!CheckedSectorOffset(state->game_offset, sector, folder_offset) ||
          !RangeWithin(folder_offset, length, state->size)) {
        return false;
      }
      uint8_t* folder_ptr = state->ptr + folder_offset;
      std::unordered_set<uint16_t> child_active_ordinals;
      std::unordered_set<uint16_t> child_visited_ordinals;
      if (!ReadEntry(state, folder_ptr, length, 0, entry.get(), child_active_ordinals,
                     child_visited_ordinals, depth + 1)) {
        return false;
      }
    }
  } else {
    // File.
    size_t file_offset = 0;
    if (!CheckedSectorOffset(state->game_offset, sector, file_offset) ||
        !RangeWithin(file_offset, length, state->size)) {
      return false;
    }
    entry->data_offset_ = file_offset;
    entry->data_size_ = length;
    ++file_count_;
    total_file_size_ += length;
  }

  // Add to parent.
  parent->children_.emplace_back(std::move(entry));

  // Read next file in the list.
  active_ordinals.erase(entry_ordinal);
  if (node_r && !ReadEntry(state, buffer, buffer_size, node_r, parent, active_ordinals,
                           visited_ordinals, depth + 1)) {
    return false;
  }

  return true;
}

}  // namespace rex::filesystem
