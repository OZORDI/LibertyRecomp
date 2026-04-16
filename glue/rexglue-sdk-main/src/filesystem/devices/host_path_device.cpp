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

#include <rex/filesystem/devices/host_path_entry.h>

#include <algorithm>

#include <rex/filesystem.h>
#include <rex/filesystem/devices/host_path_device.h>
#include <rex/logging.h>
#include <rex/math.h>
#include <rex/platform.h>
#include <rex/string/utf8.h>

#if REX_PLATFORM_NX
#include <switch.h>
#endif

namespace rex::filesystem {

namespace {
// Returns the libnx fsdev mount name (everything before the first ':') if
// |host_path| is a libnx-mounted save device path (e.g. "save:/profile/0").
// Returns an empty string otherwise.
std::string ExtractSaveMountName(const std::filesystem::path& host_path) {
  const std::string s = host_path.string();
  const auto colon = s.find(':');
  if (colon == std::string::npos || colon == 0) {
    return {};
  }
  // libnx save mounts are conventionally named "save" or "save<N>". Be
  // permissive: any mount whose name starts with "save" is treated as a save
  // device that needs explicit commit-to-flash.
  std::string name = s.substr(0, colon);
  if (name.compare(0, 4, "save") == 0) {
    return name;
  }
  return {};
}
}  // namespace

void CommitSaveMount(const std::string& mount_name) {
#if REX_PLATFORM_NX
  if (mount_name.empty()) {
    return;
  }
  Result rc = fsdevCommitDevice(mount_name.c_str());
  if (R_FAILED(rc)) {
    REXFS_ERROR("fsdevCommitDevice(\"{}\") failed: 0x{:08X}", mount_name,
                static_cast<uint32_t>(rc));
  }
#else
  (void)mount_name;
#endif
}

HostPathDevice::HostPathDevice(const std::string_view mount_path,
                               const std::filesystem::path& host_path, bool read_only)
    : Device(mount_path), name_("STFS"), host_path_(host_path), read_only_(read_only) {}

HostPathDevice::~HostPathDevice() {
  // Flush any buffered writes for Switch save mounts so data survives
  // reboot / sudden power loss.
  if (!read_only_) {
    auto save_name = ExtractSaveMountName(host_path_);
    if (!save_name.empty()) {
      CommitSaveMount(save_name);
    }
  }
}

bool HostPathDevice::Initialize() {
  if (!std::filesystem::exists(host_path_)) {
    if (!read_only_) {
      // Create the path.
      std::filesystem::create_directories(host_path_);
    } else {
      REXFS_ERROR("Host path does not exist");
      return false;
    }
  }

  auto root_entry = new HostPathEntry(this, nullptr, "", host_path_);
  root_entry->attributes_ = kFileAttributeDirectory;
  root_entry_ = std::unique_ptr<Entry>(root_entry);
  PopulateEntry(root_entry);

  return true;
}

void HostPathDevice::Dump(string::StringBuffer* string_buffer) {
  auto global_lock = global_critical_region_.Acquire();
  root_entry_->Dump(string_buffer, 0);
}

Entry* HostPathDevice::ResolvePath(const std::string_view path) {
  // The filesystem will have stripped our prefix off already, so the path will
  // be in the form:
  // some\PATH.foo
  auto* resolved = root_entry_->ResolvePath(path);
  if (resolved) {
    return resolved;
  }

  // Fallback to a lazy case-insensitive host lookup when an entry is missing
  // from the in-memory tree (for example because casing differs on Linux).
  auto* current_entry = static_cast<HostPathEntry*>(root_entry_.get());
  for (const auto& part : rex::string::utf8_split_path(path)) {
    if (part.empty()) {
      continue;
    }

    auto* child = current_entry->GetChild(part);
    if (!child) {
      auto child_infos = rex::filesystem::ListFiles(current_entry->host_path());
      auto match = std::find_if(child_infos.begin(), child_infos.end(), [&](const auto& info) {
        return rex::string::utf8_equal_case(rex::path_to_utf8(info.name), part);
      });
      if (match == child_infos.end()) {
        return nullptr;
      }

      auto new_child = HostPathEntry::Create(this, current_entry,
                                             current_entry->host_path() / match->name, *match);
      if (!new_child) {
        return nullptr;
      }
      child = new_child;
      current_entry->children_.push_back(std::unique_ptr<Entry>(new_child));
    }

    current_entry = static_cast<HostPathEntry*>(child);
  }

  return current_entry;
}

void HostPathDevice::PopulateEntry(HostPathEntry* parent_entry) {
  auto child_infos = rex::filesystem::ListFiles(parent_entry->host_path());
  for (auto& child_info : child_infos) {
    auto child = HostPathEntry::Create(this, parent_entry,
                                       parent_entry->host_path() / child_info.name, child_info);
    parent_entry->children_.push_back(std::unique_ptr<Entry>(child));

    if (child_info.type == rex::filesystem::FileInfo::Type::kDirectory) {
      PopulateEntry(child);
    }
  }
}

}  // namespace rex::filesystem
