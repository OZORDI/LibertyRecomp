#include "gta4_installer.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <chrono>
#include <cstring>
#include <fstream>
#include <limits>
#include <memory>
#include <optional>
#include <span>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

#include <rex/crypto/sha256.h>
#include <rex/filesystem.h>
#include <rex/filesystem/device.h>
#include <rex/filesystem/devices/disc_image_device.h>
#include <rex/filesystem/devices/host_path_device.h>
#include <rex/filesystem/devices/stfs_container_device.h>
#include <rex/filesystem/entry.h>
#include <rex/filesystem/file.h>
#include <rex/literals.h>
#include <rex/logging.h>
#include <rex/system/util/xex2_info.h>
#include <rex/system/xcontent.h>
#include <rex/system/xtypes.h>

#include "TinySHA1.hpp"
#include "gta4_rpf_extractor.h"

#ifndef GTA4_RPF_AES_KEY_SOURCE
#error "GTA4_RPF_AES_KEY_SOURCE must point to the GTA IV RPF AES key"
#endif

namespace gta4::install {
namespace {

using namespace rex::literals;

constexpr uint32_t kGta4TitleId = 0x545407F2;
constexpr uint32_t kRequiredTargetVersion = 0x00000805;
constexpr std::string_view kRequiredPatchSha256 =
    "480aee5e2b42707791e7571bb8407c5bb3f6c7534f07f9beb426db4cfc648fd3";
constexpr std::string_view kEmbeddedTargetXexSha256 =
    "8268fdc91f83c4288e1db8319a109e622865497d1405d9f4ca98f46830d1ff21";
constexpr size_t kMaximumMetadataFileSize = 64_MiB;
constexpr size_t kCopyBufferSize = 1_MiB;
constexpr size_t kMaximumTreeEntries = 500000;

struct FileDestroyer {
  void operator()(rex::filesystem::File* file) const {
    if (file) {
      file->Destroy();
    }
  }
};
using FilePtr = std::unique_ptr<rex::filesystem::File, FileDestroyer>;

struct MountedSource {
  std::unique_ptr<rex::filesystem::Device> device;
  rex::filesystem::Entry* root = nullptr;
  std::optional<uint32_t> package_title_id;
};

struct FoundEntry {
  rex::filesystem::Entry* entry = nullptr;
  std::filesystem::path relative_path;
};

struct XexInfo {
  uint32_t module_flags = 0;
  uint32_t version = 0;
  uint32_t title_id = 0;
  std::array<uint8_t, 0x100> rsa_signature{};
  bool has_security_info = false;
  bool has_execution_info = false;
  bool has_delta_descriptor = false;
  uint32_t delta_source_version = 0;
  uint32_t delta_target_version = 0;
  std::array<uint8_t, 0x14> delta_source_digest{};
};

struct PreparedSource {
  MountedSource mounted;
  rex::filesystem::Entry* payload_root = nullptr;
  FoundEntry xex;
  std::vector<uint8_t> xex_bytes;
  XexInfo xex_info;
};

struct PreparedUpdate {
  std::optional<MountedSource> mounted;
  rex::filesystem::Entry* payload_root = nullptr;
  FoundEntry patch;
  std::vector<uint8_t> patch_bytes;
  XexInfo patch_info;
  bool raw_patch = false;
};

struct PreparedDlc {
  Episode episode;
  MountedSource mounted;
  rex::filesystem::Entry* payload_root = nullptr;
};

struct PublishItem {
  std::filesystem::path staged;
  std::filesystem::path destination;
  std::filesystem::path backup;
  bool had_destination = false;
  bool published = false;
};

std::string Lowercase(std::string_view value) {
  std::string result(value);
  std::transform(result.begin(), result.end(), result.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return result;
}

bool IsDirectory(const rex::filesystem::Entry* entry) {
  return entry && (entry->attributes() & rex::filesystem::kFileAttributeDirectory) != 0;
}

std::optional<std::filesystem::path> SafeComponent(std::string_view name) {
  if (name.empty() || name == "." || name == ".." || name.find('/') != std::string_view::npos ||
      name.find('\\') != std::string_view::npos || name.find(':') != std::string_view::npos) {
    return std::nullopt;
  }
  auto component = rex::to_path(name);
  if (component.empty() || component.is_absolute() || component.has_root_path()) {
    return std::nullopt;
  }
  return component;
}

rex::filesystem::Entry* FindDirectChild(rex::filesystem::Entry* parent,
                                        std::string_view wanted_name) {
  if (!parent) {
    return nullptr;
  }
  const std::string wanted = Lowercase(wanted_name);
  for (const auto& child : parent->children()) {
    if (Lowercase(child->name()) == wanted) {
      return child.get();
    }
  }
  return nullptr;
}

std::vector<FoundEntry> FindEntries(rex::filesystem::Entry* root, std::string_view wanted_name) {
  struct Pending {
    rex::filesystem::Entry* entry;
    std::filesystem::path relative_path;
  };

  std::vector<FoundEntry> matches;
  std::vector<Pending> pending;
  pending.push_back({root, {}});
  const std::string wanted = Lowercase(wanted_name);
  size_t visited = 0;
  while (!pending.empty()) {
    Pending current = std::move(pending.back());
    pending.pop_back();
    if (++visited > kMaximumTreeEntries) {
      return {};
    }

    if (current.entry != root && Lowercase(current.entry->name()) == wanted) {
      matches.push_back({current.entry, current.relative_path});
    }
    if (!IsDirectory(current.entry)) {
      continue;
    }
    for (const auto& child : current.entry->children()) {
      auto component = SafeComponent(child->name());
      if (!component) {
        return {};
      }
      pending.push_back({child.get(), current.relative_path / *component});
    }
  }
  return matches;
}

bool ReadEntryBytes(rex::filesystem::Entry* entry, std::vector<uint8_t>& bytes,
                    std::string& error) {
  if (!entry || IsDirectory(entry)) {
    error = "The selected source entry is not a file.";
    return false;
  }
  if (entry->size() > kMaximumMetadataFileSize) {
    error = "The selected metadata file is unexpectedly large.";
    return false;
  }

  rex::filesystem::File* raw_file = nullptr;
  const rex::X_STATUS open_status =
      entry->Open(rex::filesystem::FileAccess::kGenericRead, &raw_file);
  FilePtr file(raw_file);
  if (open_status != 0 || !file) {
    error = "The selected source file could not be opened.";
    return false;
  }

  bytes.resize(entry->size());
  size_t offset = 0;
  while (offset < bytes.size()) {
    size_t bytes_read = 0;
    const rex::X_STATUS read_status =
        file->ReadSync(std::span<uint8_t>(bytes).subspan(offset), offset, &bytes_read);
    if (read_status != 0 || bytes_read == 0 || bytes_read > bytes.size() - offset) {
      error = "The selected source file could not be read completely.";
      return false;
    }
    offset += bytes_read;
  }
  return true;
}

bool ReadHostFile(const std::filesystem::path& path, std::vector<uint8_t>& bytes,
                  std::string& error) {
  std::error_code fs_error;
  const uintmax_t size = std::filesystem::file_size(path, fs_error);
  if (fs_error || size > kMaximumMetadataFileSize ||
      size > static_cast<uintmax_t>(std::numeric_limits<size_t>::max())) {
    error = "The selected file is missing or unexpectedly large.";
    return false;
  }
  bytes.resize(static_cast<size_t>(size));
  std::ifstream stream(path, std::ios::binary);
  if (!stream ||
      (!bytes.empty() && !stream.read(reinterpret_cast<char*>(bytes.data()), bytes.size()))) {
    error = "The selected file could not be read.";
    return false;
  }
  return true;
}

std::string HashBytes(std::span<const uint8_t> bytes) {
  return rex::crypto::sha256(
      std::string_view(reinterpret_cast<const char*>(bytes.data()), bytes.size()));
}

bool ParseXex(std::span<const uint8_t> bytes, XexInfo& info, std::string& error) {
  if (bytes.size() < sizeof(rex::xex2_header) || std::memcmp(bytes.data(), "XEX2", 4) != 0) {
    error = "The selected executable is not an XEX2 image.";
    return false;
  }

  const auto* header = reinterpret_cast<const rex::xex2_header*>(bytes.data());
  const size_t header_size = header->header_size;
  const size_t header_count = header->header_count;
  if (header_size > bytes.size() || header_size < sizeof(rex::xex2_header)) {
    error = "The XEX header size is invalid.";
    return false;
  }
  if (header_count >
      (header_size - offsetof(rex::xex2_header, headers)) / sizeof(rex::xex2_opt_header)) {
    error = "The XEX optional-header table is invalid.";
    return false;
  }

  info.module_flags = static_cast<uint32_t>(header->module_flags);
  const size_t security_offset = header->security_offset;
  if (security_offset <= header_size &&
      sizeof(rex::xex2_security_info) <= header_size - security_offset) {
    const auto* security =
        reinterpret_cast<const rex::xex2_security_info*>(bytes.data() + security_offset);
    std::memcpy(info.rsa_signature.data(), security->rsa_signature, info.rsa_signature.size());
    info.has_security_info = true;
  }

  for (size_t index = 0; index < header_count; ++index) {
    const auto& optional = header->headers[index];
    const uint32_t key = optional.key;
    const size_t offset = optional.offset;
    if (key == rex::XEX_HEADER_EXECUTION_INFO) {
      if (offset > header_size || sizeof(rex::xex2_opt_execution_info) > header_size - offset) {
        error = "The XEX execution-information header is out of bounds.";
        return false;
      }
      const auto* execution =
          reinterpret_cast<const rex::xex2_opt_execution_info*>(bytes.data() + offset);
      info.version = execution->version_value;
      info.title_id = execution->title_id;
      info.has_execution_info = true;
    } else if (key == rex::XEX_HEADER_DELTA_PATCH_DESCRIPTOR) {
      if (offset > header_size ||
          offsetof(rex::xex2_opt_delta_patch_descriptor, info) > header_size - offset) {
        error = "The XEXP delta descriptor is out of bounds.";
        return false;
      }
      const auto* delta =
          reinterpret_cast<const rex::xex2_opt_delta_patch_descriptor*>(bytes.data() + offset);
      info.delta_target_version = delta->target_version_value;
      info.delta_source_version = delta->source_version_value;
      std::memcpy(info.delta_source_digest.data(), delta->digest_source,
                  info.delta_source_digest.size());
      info.has_delta_descriptor = true;
    }
  }

  return true;
}

bool ValidateBaseXex(const XexInfo& info, std::string& error) {
  if (!info.has_execution_info || !info.has_security_info) {
    error = "The base XEX is missing required execution or security information.";
    return false;
  }
  if (info.title_id != kGta4TitleId) {
    error = "The selected source is not Grand Theft Auto IV for Xbox 360.";
    return false;
  }
  if ((info.module_flags & rex::XEX_MODULE_TITLE) == 0) {
    error = "The selected XEX is not a title executable.";
    return false;
  }
  return true;
}

bool ValidatePatch(const XexInfo& base, const XexInfo& patch, std::span<const uint8_t> patch_bytes,
                   std::string& error) {
  const uint32_t patch_flags = rex::XEX_MODULE_MODULE_PATCH | rex::XEX_MODULE_PATCH_DELTA;
  if ((patch.module_flags & patch_flags) != patch_flags || !patch.has_delta_descriptor) {
    error = "The selected update does not contain a delta XEXP patch.";
    return false;
  }
  if (patch.delta_target_version != kRequiredTargetVersion) {
    error = "This build requires the GTA IV v8 (0.0.8.5) title update.";
    return false;
  }
  if (patch.delta_source_version != base.version) {
    error = "The selected title update does not target this base-game revision.";
    return false;
  }
  if (HashBytes(patch_bytes) != kRequiredPatchSha256) {
    error = "The selected XEXP is not the supported GTA IV v8 patch.";
    return false;
  }

  std::array<uint8_t, 0x14> digest{};
  sha1::SHA1 sha;
  sha.processBytes(base.rsa_signature.data(), base.rsa_signature.size());
  sha.finalize(digest.data());
  if (digest != patch.delta_source_digest) {
    error = "The title update signature does not match the selected base XEX.";
    return false;
  }
  return true;
}

std::optional<MountedSource> MountSource(const std::filesystem::path& path, std::string& error) {
  std::error_code fs_error;
  if (std::filesystem::is_directory(path, fs_error)) {
    MountedSource source;
    source.device = std::make_unique<rex::filesystem::HostPathDevice>("install:", path, true);
    if (!source.device->Initialize()) {
      error = "The selected folder could not be opened.";
      return std::nullopt;
    }
    source.root = source.device->ResolvePath("");
    return source;
  }

  fs_error.clear();
  if (!std::filesystem::is_regular_file(path, fs_error)) {
    error = "The selected source does not exist.";
    return std::nullopt;
  }

  if (Lowercase(path.extension().string()) == ".iso") {
    MountedSource source;
    source.device = std::make_unique<rex::filesystem::DiscImageDevice>("install:", path);
    if (!source.device->Initialize()) {
      error = "The selected file is not a valid Xbox 360 disc image.";
      return std::nullopt;
    }
    source.root = source.device->ResolvePath("");
    return source;
  }

  auto header = rex::filesystem::StfsContainerDevice::ReadPackageHeader(path);
  if (!header) {
    error = "The selected file is neither an Xbox 360 ISO nor an STFS/SVOD package.";
    return std::nullopt;
  }

  MountedSource source;
  source.package_title_id = header->metadata.execution_info.title_id;
  source.device = std::make_unique<rex::filesystem::StfsContainerDevice>("install:", path);
  if (!source.device->Initialize()) {
    error = "The Xbox content package is corrupt or unsupported.";
    return std::nullopt;
  }
  source.root = source.device->ResolvePath("");
  return source;
}

bool CheckedAdd(uint64_t value, uint64_t& total) {
  if (value > std::numeric_limits<uint64_t>::max() - total) {
    return false;
  }
  total += value;
  return true;
}

std::optional<std::filesystem::path> FindRpfAesKey() {
  const std::array candidates = {
      rex::filesystem::GetExecutableFolder().parent_path() / "Resources" / "aes_key.bin",
      std::filesystem::path(GTA4_RPF_AES_KEY_SOURCE),
  };
  for (const auto& candidate : candidates) {
    std::error_code fs_error;
    if (std::filesystem::is_regular_file(candidate, fs_error) &&
        std::filesystem::file_size(candidate, fs_error) == 32 && !fs_error) {
      return candidate;
    }
  }
  return std::nullopt;
}

bool TreeSize(rex::filesystem::Entry* root, uint64_t& total, std::string& error) {
  std::vector<rex::filesystem::Entry*> pending;
  pending.push_back(root);
  size_t visited = 0;
  while (!pending.empty()) {
    auto* entry = pending.back();
    pending.pop_back();
    if (++visited > kMaximumTreeEntries) {
      error = "The source contains an unexpectedly large directory tree.";
      return false;
    }
    if (IsDirectory(entry)) {
      for (const auto& child : entry->children()) {
        pending.push_back(child.get());
      }
    } else if (!CheckedAdd(entry->size(), total)) {
      error = "The source size exceeds the supported range.";
      return false;
    }
  }
  return true;
}

bool CopyFileEntry(rex::filesystem::Entry* entry, const std::filesystem::path& destination,
                   Progress& progress, std::string& error) {
  std::error_code fs_error;
  std::filesystem::create_directories(destination.parent_path(), fs_error);
  if (fs_error) {
    error = "Could not create an installation directory: " + fs_error.message();
    return false;
  }

  rex::filesystem::File* raw_file = nullptr;
  const rex::X_STATUS open_status =
      entry->Open(rex::filesystem::FileAccess::kGenericRead, &raw_file);
  FilePtr file(raw_file);
  if (open_status != 0 || !file) {
    error = "Could not open " + entry->path() + ".";
    return false;
  }

  std::ofstream output(destination, std::ios::binary | std::ios::trunc);
  if (!output) {
    error = "Could not create " + destination.string() + ".";
    return false;
  }

  std::vector<uint8_t> buffer(kCopyBufferSize);
  size_t offset = 0;
  while (offset < entry->size()) {
    if (progress.cancel_requested.load(std::memory_order_relaxed)) {
      error = "Installation was cancelled.";
      return false;
    }
    const size_t remaining = entry->size() - offset;
    const size_t request = std::min(remaining, buffer.size());
    size_t bytes_read = 0;
    const rex::X_STATUS read_status =
        file->ReadSync(std::span<uint8_t>(buffer).first(request), offset, &bytes_read);
    if (read_status != 0 || bytes_read == 0 || bytes_read > request) {
      error = "Failed while reading " + entry->path() + ".";
      return false;
    }
    output.write(reinterpret_cast<const char*>(buffer.data()), bytes_read);
    if (!output) {
      error = "Failed while writing " + destination.string() + ".";
      return false;
    }
    offset += bytes_read;
    progress.copied_bytes.fetch_add(bytes_read, std::memory_order_relaxed);
  }
  output.flush();
  if (!output) {
    error = "Failed while flushing " + destination.string() + ".";
    return false;
  }
  return true;
}

bool CopyTree(rex::filesystem::Entry* root, const std::filesystem::path& destination,
              Progress& progress, std::string& error) {
  struct Pending {
    rex::filesystem::Entry* entry;
    std::filesystem::path relative_path;
  };
  std::vector<Pending> pending;
  for (const auto& child : root->children()) {
    auto component = SafeComponent(child->name());
    if (!component) {
      error = "The source contains an unsafe path component.";
      return false;
    }
    pending.push_back({child.get(), *component});
  }

  std::unordered_set<std::string> destinations;
  size_t visited = 0;
  while (!pending.empty()) {
    Pending current = std::move(pending.back());
    pending.pop_back();
    if (++visited > kMaximumTreeEntries) {
      error = "The source contains an unexpectedly large directory tree.";
      return false;
    }

    const std::string collision_key = Lowercase(current.relative_path.generic_string());
    if (!destinations.insert(collision_key).second) {
      error = "The source contains colliding case-insensitive paths.";
      return false;
    }

    const auto output_path = destination / current.relative_path;
    if (IsDirectory(current.entry)) {
      std::error_code fs_error;
      std::filesystem::create_directories(output_path, fs_error);
      if (fs_error) {
        error = "Could not create an installation directory: " + fs_error.message();
        return false;
      }
      for (const auto& child : current.entry->children()) {
        auto component = SafeComponent(child->name());
        if (!component) {
          error = "The source contains an unsafe path component.";
          return false;
        }
        pending.push_back({child.get(), current.relative_path / *component});
      }
    } else if (!CopyFileEntry(current.entry, output_path, progress, error)) {
      return false;
    }
  }
  return true;
}

bool CopyHostFile(const std::filesystem::path& source, const std::filesystem::path& destination,
                  Progress& progress, std::string& error) {
  std::error_code fs_error;
  std::filesystem::create_directories(destination.parent_path(), fs_error);
  if (fs_error) {
    error = "Could not create an installation directory: " + fs_error.message();
    return false;
  }

  std::ifstream input(source, std::ios::binary);
  std::ofstream output(destination, std::ios::binary | std::ios::trunc);
  if (!input || !output) {
    error = "Could not copy " + source.string() + ".";
    return false;
  }
  std::vector<char> buffer(kCopyBufferSize);
  while (input) {
    if (progress.cancel_requested.load(std::memory_order_relaxed)) {
      error = "Installation was cancelled.";
      return false;
    }
    input.read(buffer.data(), buffer.size());
    const std::streamsize count = input.gcount();
    if (count > 0) {
      output.write(buffer.data(), count);
      if (!output) {
        error = "Failed while writing " + destination.string() + ".";
        return false;
      }
      progress.copied_bytes.fetch_add(static_cast<uint64_t>(count), std::memory_order_relaxed);
    }
  }
  if (!input.eof()) {
    error = "Failed while reading " + source.string() + ".";
    return false;
  }
  output.flush();
  return static_cast<bool>(output);
}

std::optional<PreparedSource> PrepareGameSource(const std::filesystem::path& path,
                                                std::string& error) {
  auto mounted = MountSource(path, error);
  if (!mounted) {
    return std::nullopt;
  }
  auto matches = FindEntries(mounted->root, "default.xex");
  if (matches.size() != 1) {
    error = "The game source must contain exactly one default.xex.";
    return std::nullopt;
  }

  PreparedSource source;
  source.mounted = std::move(*mounted);
  source.xex = std::move(matches.front());
  source.payload_root = source.xex.entry->parent();
  if (!source.payload_root) {
    error = "The game source has an invalid root layout.";
    return std::nullopt;
  }
  if (!ReadEntryBytes(source.xex.entry, source.xex_bytes, error) ||
      !ParseXex(source.xex_bytes, source.xex_info, error) ||
      !ValidateBaseXex(source.xex_info, error)) {
    return std::nullopt;
  }
  if (source.mounted.package_title_id && *source.mounted.package_title_id != kGta4TitleId) {
    error = "The selected Xbox content package belongs to a different title.";
    return std::nullopt;
  }
  return source;
}

std::optional<PreparedUpdate> PrepareUpdateSource(const std::filesystem::path& path,
                                                  const XexInfo& base, std::string& error) {
  PreparedUpdate update;
  if (Lowercase(path.extension().string()) == ".xexp" && std::filesystem::is_regular_file(path)) {
    update.raw_patch = true;
    if (!ReadHostFile(path, update.patch_bytes, error)) {
      return std::nullopt;
    }
  } else {
    auto mounted = MountSource(path, error);
    if (!mounted) {
      return std::nullopt;
    }
    if (mounted->package_title_id && *mounted->package_title_id != kGta4TitleId) {
      error = "The selected title update belongs to a different game.";
      return std::nullopt;
    }
    auto matches = FindEntries(mounted->root, "default.xexp");
    if (matches.size() != 1) {
      error = "The title update must contain exactly one default.xexp.";
      return std::nullopt;
    }
    update.patch = std::move(matches.front());
    update.payload_root = update.patch.entry->parent();
    if (!update.payload_root) {
      error = "The title update has an invalid payload layout.";
      return std::nullopt;
    }
    if (!ReadEntryBytes(update.patch.entry, update.patch_bytes, error)) {
      return std::nullopt;
    }
    update.mounted = std::move(*mounted);
  }

  if (!ParseXex(update.patch_bytes, update.patch_info, error) ||
      !ValidatePatch(base, update.patch_info, update.patch_bytes, error)) {
    return std::nullopt;
  }
  return update;
}

rex::filesystem::Entry* FindDlcPayloadRoot(rex::filesystem::Entry* root, Episode episode) {
  std::vector<rex::filesystem::Entry*> pending;
  pending.push_back(root);
  size_t visited = 0;
  while (!pending.empty()) {
    auto* directory = pending.back();
    pending.pop_back();
    if (++visited > kMaximumTreeEntries) {
      return nullptr;
    }

    auto* setup = FindDirectChild(directory, "setup2.xml");
    auto* content = FindDirectChild(directory, "content.dat");
    auto* episode_marker =
        FindDirectChild(directory, episode == Episode::kTlad ? "e1_audio.xml" : "e2_audio.xml");
    auto* wrong_marker =
        FindDirectChild(directory, episode == Episode::kTlad ? "e2_audio.xml" : "e1_audio.xml");
    if (setup && content && episode_marker && !wrong_marker) {
      return directory;
    }

    for (const auto& child : directory->children()) {
      if (IsDirectory(child.get())) {
        pending.push_back(child.get());
      }
    }
  }
  return nullptr;
}

std::optional<PreparedDlc> PrepareDlcSource(const DlcSelection& selection, std::string& error) {
  auto mounted = MountSource(selection.source, error);
  if (!mounted) {
    return std::nullopt;
  }
  if (mounted->package_title_id && *mounted->package_title_id != kGta4TitleId) {
    error = "The selected DLC package belongs to a different game.";
    return std::nullopt;
  }
  auto* payload_root = FindDlcPayloadRoot(mounted->root, selection.episode);
  if (!payload_root) {
    error = std::string("The selected source is not a complete ") +
            (selection.episode == Episode::kTlad ? "TLAD" : "TBOGT") + " package.";
    return std::nullopt;
  }
  return PreparedDlc{selection.episode, std::move(*mounted), payload_root};
}

bool WriteManifest(const std::filesystem::path& game_root, const XexInfo& base,
                   std::span<const uint8_t> base_bytes, std::span<const uint8_t> patch_bytes,
                   std::string& error) {
  const auto temporary = game_root / ".install-manifest.tmp";
  const auto destination = game_root / ".install-manifest";
  std::ofstream stream(temporary, std::ios::binary | std::ios::trunc);
  if (!stream) {
    error = "Could not create the installation manifest.";
    return false;
  }
  stream << "format=1\n";
  stream << "title_id=545407F2\n";
  stream << "base_version=" << base.version << "\n";
  stream << "target_version=" << kRequiredTargetVersion << "\n";
  stream << "rpf_layout=preserved-and-extracted\n";
  stream << "base_sha256=" << HashBytes(base_bytes) << "\n";
  if (!patch_bytes.empty()) {
    stream << "patch_sha256=" << HashBytes(patch_bytes) << "\n";
  }
  stream.flush();
  if (!stream) {
    error = "Could not finish the installation manifest.";
    return false;
  }
  stream.close();

  std::error_code fs_error;
  std::filesystem::remove(destination, fs_error);
  fs_error.clear();
  std::filesystem::rename(temporary, destination, fs_error);
  if (fs_error) {
    error = "Could not publish the installation manifest: " + fs_error.message();
    return false;
  }
  return true;
}

bool PublishDirectories(std::vector<PublishItem>& items, std::string& error) {
  std::error_code fs_error;
  auto rollback = [&]() {
    for (auto item = items.rbegin(); item != items.rend(); ++item) {
      std::error_code rollback_error;
      if (item->published) {
        std::filesystem::remove_all(item->destination, rollback_error);
        rollback_error.clear();
      }
      if (item->had_destination && std::filesystem::exists(item->backup, rollback_error)) {
        rollback_error.clear();
        std::filesystem::rename(item->backup, item->destination, rollback_error);
      }
    }
  };

  for (auto& item : items) {
    std::filesystem::create_directories(item.destination.parent_path(), fs_error);
    if (fs_error) {
      error = "Could not create the installation root: " + fs_error.message();
      rollback();
      return false;
    }
    std::filesystem::remove_all(item.backup, fs_error);
    if (fs_error) {
      error = "Could not clear a stale installation backup: " + fs_error.message();
      rollback();
      return false;
    }
    item.had_destination = std::filesystem::exists(item.destination, fs_error);
    if (fs_error) {
      error = "Could not inspect the previous installation: " + fs_error.message();
      rollback();
      return false;
    }
    if (item.had_destination) {
      std::filesystem::rename(item.destination, item.backup, fs_error);
      if (fs_error) {
        error = "Could not preserve the previous installation: " + fs_error.message();
        rollback();
        return false;
      }
    }
  }

  for (auto& item : items) {
    std::filesystem::rename(item.staged, item.destination, fs_error);
    if (fs_error) {
      error = "Could not publish the new installation: " + fs_error.message();
      rollback();
      return false;
    }
    item.published = true;
  }

  for (auto& item : items) {
    std::filesystem::remove_all(item.backup, fs_error);
    if (fs_error) {
      REXLOG_WARN("Installer: could not remove backup '{}': {}", item.backup.string(),
                  fs_error.message());
    }
    fs_error.clear();
  }
  return true;
}

bool ValidateStagedEpisode(const std::filesystem::path& path, Episode episode) {
  std::error_code error;
  if (!std::filesystem::is_regular_file(path / "setup2.xml", error)) {
    return false;
  }
  error.clear();
  if (!std::filesystem::is_regular_file(path / "content.dat", error)) {
    return false;
  }
  error.clear();
  const auto required_marker = episode == Episode::kTlad ? "e1_audio.xml" : "e2_audio.xml";
  const auto wrong_marker = episode == Episode::kTlad ? "e2_audio.xml" : "e1_audio.xml";
  if (!std::filesystem::is_regular_file(path / required_marker, error)) {
    return false;
  }
  error.clear();
  return !std::filesystem::exists(path / wrong_marker, error) && !error;
}

bool ValidateInstalledPair(const std::filesystem::path& game_root, std::string& reason) {
  const auto base_path = game_root / "default.xex";
  std::vector<uint8_t> base_bytes;
  if (!ReadHostFile(base_path, base_bytes, reason)) {
    reason = "default.xex is missing or unreadable.";
    return false;
  }
  XexInfo base;
  if (!ParseXex(base_bytes, base, reason) || !ValidateBaseXex(base, reason)) {
    return false;
  }

  if (base.version == kRequiredTargetVersion && HashBytes(base_bytes) == kEmbeddedTargetXexSha256) {
    return true;
  }

  const auto patch_path = game_root / "default.xexp";
  std::vector<uint8_t> patch_bytes;
  if (!ReadHostFile(patch_path, patch_bytes, reason)) {
    reason = "The required sibling default.xexp is missing.";
    return false;
  }
  XexInfo patch;
  return ParseXex(patch_bytes, patch, reason) && ValidatePatch(base, patch, patch_bytes, reason);
}

}  // namespace

const char* EpisodeDirectory(Episode episode) {
  return episode == Episode::kTlad ? "TLAD" : "TBOGT";
}

bool IsInstallReady(const std::filesystem::path& game_root, std::string* reason) {
  std::string local_reason;
  const bool ready = ValidateInstalledPair(game_root, local_reason);
  if (reason) {
    *reason = std::move(local_reason);
  }
  return ready;
}

bool IsEpisodeReady(const std::filesystem::path& marketplace_root, Episode episode) {
  return ValidateStagedEpisode(marketplace_root / EpisodeDirectory(episode), episode);
}

Result VerifyInstall(const std::filesystem::path& game_root,
                     const std::filesystem::path& marketplace_root) {
  Result result;
  if (!IsInstallReady(game_root, &result.error)) {
    return result;
  }
  for (Episode episode : {Episode::kTlad, Episode::kTbogt}) {
    const auto episode_root = marketplace_root / EpisodeDirectory(episode);
    std::error_code fs_error;
    if (std::filesystem::exists(episode_root, fs_error) &&
        !IsEpisodeReady(marketplace_root, episode)) {
      result.error = std::string(EpisodeDirectory(episode)) + " is installed incompletely.";
      return result;
    }
  }
  result.success = true;
  return result;
}

Result Install(const Selection& selection, const std::filesystem::path& install_root,
               Progress& progress) {
  Result result;
  progress.copied_bytes = 0;
  progress.total_bytes = 0;

  std::optional<PreparedSource> game;
  if (!selection.game_source.empty()) {
    game = PrepareGameSource(selection.game_source, result.error);
    if (!game) {
      return result;
    }
  } else {
    if (!selection.update_source.empty()) {
      result.error = "A base-game source is required when installing a title update.";
      return result;
    }
    if (!IsInstallReady(install_root / "game", &result.error)) {
      result.error =
          "DLC-only installation requires a valid existing GTA IV installation: " + result.error;
      return result;
    }
    if (selection.dlc_sources.empty()) {
      result.error = "No DLC source was selected.";
      return result;
    }
  }

  std::optional<PreparedUpdate> update;
  if (game && !selection.update_source.empty()) {
    update = PrepareUpdateSource(selection.update_source, game->xex_info, result.error);
    if (!update) {
      return result;
    }
  } else if (game && (game->xex_info.version != kRequiredTargetVersion ||
                      HashBytes(game->xex_bytes) != kEmbeddedTargetXexSha256)) {
    result.error = "The retail GTA IV source requires the v8 title update package.";
    return result;
  }

  std::vector<PreparedDlc> dlc;
  dlc.reserve(selection.dlc_sources.size());
  std::unordered_set<std::string> selected_episodes;
  for (const auto& selected : selection.dlc_sources) {
    if (!selected_episodes.insert(EpisodeDirectory(selected.episode)).second) {
      result.error = "The same episode was selected more than once.";
      return result;
    }
    auto prepared = PrepareDlcSource(selected, result.error);
    if (!prepared) {
      return result;
    }
    dlc.push_back(std::move(*prepared));
  }

  uint64_t total = 0;
  if (game && !TreeSize(game->payload_root, total, result.error)) {
    return result;
  }
  if (update) {
    if (update->raw_patch) {
      if (!CheckedAdd(update->patch_bytes.size(), total)) {
        result.error = "The installation size exceeds the supported range.";
        return result;
      }
    } else if (!TreeSize(update->payload_root, total, result.error) ||
               !CheckedAdd(update->patch_bytes.size(), total)) {
      result.error = "The installation size exceeds the supported range.";
      return result;
    }
  }
  for (auto& episode : dlc) {
    if (!TreeSize(episode.payload_root, total, result.error)) {
      return result;
    }
  }
  const auto aes_key_path = game ? FindRpfAesKey() : std::nullopt;
  if (game && !aes_key_path) {
    result.error = "The bundled GTA IV RPF AES key is missing.";
    return result;
  }
  if (aes_key_path) {
    std::error_code key_error;
    const uint64_t key_size = std::filesystem::file_size(*aes_key_path, key_error);
    if (key_error || !CheckedAdd(key_size, total)) {
      result.error = "The bundled GTA IV RPF AES key could not be measured.";
      return result;
    }
  }
  progress.total_bytes = total;

  const auto staging_root = install_root / ".install-staging";
  const auto staged_game = staging_root / "game";
  std::error_code fs_error;
  std::filesystem::remove_all(staging_root, fs_error);
  fs_error.clear();
  std::filesystem::create_directories(game ? staged_game : staging_root, fs_error);
  if (fs_error) {
    result.error = "Could not create the staging directory: " + fs_error.message();
    return result;
  }
  const auto available_space = std::filesystem::space(staging_root, fs_error);
  if (fs_error || available_space.available < total) {
    result.error = fs_error
                       ? "Could not determine available installation space: " + fs_error.message()
                       : "There is not enough free space for the GTA IV installation.";
    std::filesystem::remove_all(staging_root, fs_error);
    return result;
  }

  auto fail = [&](std::string error) {
    std::error_code cleanup_error;
    std::filesystem::remove_all(staging_root, cleanup_error);
    return Result{false, std::move(error)};
  };

  if (game && !CopyTree(game->payload_root, staged_game, progress, result.error)) {
    return fail(std::move(result.error));
  }

  if (update) {
    if (!update->raw_patch) {
      const auto staged_update = staged_game / "update";
      std::filesystem::remove_all(staged_update, fs_error);
      if (fs_error || !CopyTree(update->payload_root, staged_update, progress, result.error)) {
        if (result.error.empty()) {
          result.error =
              "Could not prepare the staged title-update directory: " + fs_error.message();
        }
        return fail(std::move(result.error));
      }
    }
    const auto sibling_patch = staged_game / "default.xexp";
    if (update->raw_patch) {
      if (!CopyHostFile(selection.update_source, sibling_patch, progress, result.error)) {
        return fail(std::move(result.error));
      }
    } else if (!CopyFileEntry(update->patch.entry, sibling_patch, progress, result.error)) {
      return fail(std::move(result.error));
    }
  }

  if (game) {
    if (!CopyHostFile(*aes_key_path, staged_game / "aes_key.bin", progress, result.error)) {
      return fail(std::move(result.error));
    }
    const RpfExtractionProgress extraction_progress = {
        &progress.copied_bytes,
        &progress.total_bytes,
        &progress.cancel_requested,
    };
    if (!ExtractGameArchives(staged_game, *aes_key_path, extraction_progress, result.error)) {
      return fail(std::move(result.error));
    }
  }

  std::vector<PublishItem> publish;
  if (game) {
    publish.push_back({staged_game, install_root / "game", install_root / ".game-backup"});
  }
  for (auto& episode : dlc) {
    const auto staged_episode = staging_root / "dlc" / EpisodeDirectory(episode.episode);
    if (!CopyTree(episode.payload_root, staged_episode, progress, result.error)) {
      return fail(std::move(result.error));
    }
    if (!ValidateStagedEpisode(staged_episode, episode.episode)) {
      return fail(std::string(EpisodeDirectory(episode.episode)) +
                  " did not produce a complete DLC layout.");
    }
    publish.push_back(
        {staged_episode, install_root / "dlc" / EpisodeDirectory(episode.episode),
         install_root / (std::string(".") + EpisodeDirectory(episode.episode) + "-backup")});
  }

  if (game) {
    std::string staged_reason;
    if (!ValidateInstalledPair(staged_game, staged_reason)) {
      return fail("The staged game failed final validation: " + staged_reason);
    }
    if (!WriteManifest(
            staged_game, game->xex_info, game->xex_bytes,
            update ? std::span<const uint8_t>(update->patch_bytes) : std::span<const uint8_t>(),
            result.error)) {
      return fail(std::move(result.error));
    }
  }
  if (!PublishDirectories(publish, result.error)) {
    return fail(std::move(result.error));
  }

  std::filesystem::remove_all(staging_root, fs_error);
  result.success = true;
  return result;
}

}  // namespace gta4::install
