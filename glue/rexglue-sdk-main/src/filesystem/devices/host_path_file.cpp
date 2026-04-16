/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2013 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

#include <rex/filesystem.h>
#include <rex/filesystem/devices/host_path_device.h>
#include <rex/filesystem/devices/host_path_entry.h>
#include <rex/filesystem/devices/host_path_file.h>

namespace rex::filesystem {

namespace {
// Mirrors the helper in host_path_device.cpp — extracts the libnx fsdev mount
// name (everything before the first ':') if the host path is a save mount.
std::string ExtractSaveMountNameForFile(const std::filesystem::path& host_path) {
  const std::string s = host_path.string();
  const auto colon = s.find(':');
  if (colon == std::string::npos || colon == 0) {
    return {};
  }
  std::string name = s.substr(0, colon);
  if (name.compare(0, 4, "save") == 0) {
    return name;
  }
  return {};
}
}  // namespace

HostPathFile::HostPathFile(uint32_t file_access, HostPathEntry* entry,
                           std::unique_ptr<rex::filesystem::FileHandle> file_handle)
    : File(file_access, entry), file_handle_(std::move(file_handle)) {}

HostPathFile::~HostPathFile() {
  // Persist Switch save data on close. Commit only when the file was actually
  // written to in this session — fsdevCommitDevice is expensive (writes to
  // flash) so we never call it on read-only opens.
  if (dirty_ && entry_) {
    auto* host_entry = static_cast<HostPathEntry*>(entry_);
    auto* host_device = static_cast<HostPathDevice*>(host_entry->device());
    if (host_device) {
      auto save_name = ExtractSaveMountNameForFile(host_device->host_path());
      if (!save_name.empty()) {
        CommitSaveMount(save_name);
      }
    }
  }
}

void HostPathFile::Destroy() {
  delete this;
}

X_STATUS HostPathFile::ReadSync(std::span<uint8_t> buffer, size_t byte_offset,
                                size_t* out_bytes_read) {
  if (!(file_access_ & (FileAccess::kGenericRead | FileAccess::kFileReadData))) {
    return X_STATUS_ACCESS_DENIED;
  }

  if (file_handle_->Read(byte_offset, buffer.data(), buffer.size(), out_bytes_read)) {
    return X_STATUS_SUCCESS;
  } else {
    return X_STATUS_END_OF_FILE;
  }
}

X_STATUS HostPathFile::WriteSync(std::span<const uint8_t> buffer, size_t byte_offset,
                                 size_t* out_bytes_written) {
  if (!(file_access_ &
        (FileAccess::kGenericWrite | FileAccess::kFileWriteData | FileAccess::kFileAppendData))) {
    return X_STATUS_ACCESS_DENIED;
  }

  if (file_handle_->Write(byte_offset, buffer.data(), buffer.size(), out_bytes_written)) {
    dirty_ = true;
    return X_STATUS_SUCCESS;
  } else {
    return X_STATUS_END_OF_FILE;
  }
}

X_STATUS HostPathFile::SetLength(size_t length) {
  if (!(file_access_ & (FileAccess::kGenericWrite | FileAccess::kFileWriteData))) {
    return X_STATUS_ACCESS_DENIED;
  }

  if (file_handle_->SetLength(length)) {
    dirty_ = true;
    return X_STATUS_SUCCESS;
  } else {
    return X_STATUS_END_OF_FILE;
  }
}

}  // namespace rex::filesystem
