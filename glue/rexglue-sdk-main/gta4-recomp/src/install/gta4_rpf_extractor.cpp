#include "gta4_rpf_extractor.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <limits>
#include <optional>
#include <span>
#include <string_view>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

#include <zlib.h>

extern "C" {
#include <rijndael-alg-fst.h>
}

namespace gta4::install {
namespace {

constexpr uint32_t kRpf2Magic = 0x32465052;
constexpr uint64_t kTocOffset = 0x800;
constexpr uint32_t kDirectoryBit = 0x80000000;
constexpr uint32_t kResourceMask = 0xC0000000;
constexpr uint32_t kResourceOffsetMask = 0x7FFFFF00;
constexpr uint32_t kCompressedBit = 0x40000000;
constexpr uint32_t kArchivedSizeMask = 0xBFFFFFFF;
constexpr size_t kHeaderSize = 20;
constexpr size_t kEntrySize = 16;
constexpr size_t kAesKeySize = 32;
constexpr size_t kMaximumEntryCount = 1000000;
constexpr size_t kMaximumArchiveDepth = 32;
constexpr size_t kMaximumNestedArchiveCount = 100000;
constexpr uint64_t kMaximumExtractedFileSize = 2147483648;
constexpr size_t kIoBufferSize = 1048576;

struct TocEntry {
  std::string name;
  bool is_directory = false;
  bool is_resource = false;
  bool is_compressed = false;
  uint32_t content_index = 0;
  uint32_t content_count = 0;
  uint32_t size = 0;
  uint32_t archived_size = 0;
  uint32_t offset = 0;
};

struct Archive {
  std::vector<TocEntry> entries;
};

std::string Lowercase(std::string_view value) {
  std::string result(value);
  std::transform(result.begin(), result.end(), result.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return result;
}

uint32_t ReadU32(std::span<const uint8_t> data, size_t offset) {
  return static_cast<uint32_t>(data[offset]) | (static_cast<uint32_t>(data[offset + 1]) << 8) |
         (static_cast<uint32_t>(data[offset + 2]) << 16) |
         (static_cast<uint32_t>(data[offset + 3]) << 24);
}

bool CheckedAdd(uint64_t left, uint64_t right, uint64_t& result) {
  if (right > std::numeric_limits<uint64_t>::max() - left) {
    return false;
  }
  result = left + right;
  return true;
}

bool CheckedMultiply(uint64_t left, uint64_t right, uint64_t& result) {
  if (left != 0 && right > std::numeric_limits<uint64_t>::max() / left) {
    return false;
  }
  result = left * right;
  return true;
}

bool RangeWithin(uint64_t offset, uint64_t length, uint64_t size) {
  return offset <= size && length <= size - offset;
}

bool IsCancelled(const RpfExtractionProgress& progress) {
  return progress.cancel_requested && progress.cancel_requested->load(std::memory_order_relaxed);
}

std::optional<std::vector<uint8_t>> ReadExactFile(const std::filesystem::path& path,
                                                  size_t expected_size) {
  std::error_code fs_error;
  if (std::filesystem::file_size(path, fs_error) != expected_size || fs_error) {
    return std::nullopt;
  }
  std::vector<uint8_t> bytes(expected_size);
  std::ifstream stream(path, std::ios::binary);
  if (!stream ||
      (!bytes.empty() && !stream.read(reinterpret_cast<char*>(bytes.data()), bytes.size()))) {
    return std::nullopt;
  }
  return bytes;
}

bool CryptToc(std::span<uint8_t> toc, std::span<const uint8_t> key) {
  if (key.size() != kAesKeySize || toc.size() % 16 != 0) {
    return false;
  }
  std::array<u32, 4 * (MAXNR + 1)> round_keys{};
  const int rounds = rijndaelKeySetupDec(round_keys.data(), key.data(), 256);
  if (rounds <= 0) {
    return false;
  }
  std::array<uint8_t, 16> output{};
  for (int pass = 0; pass < 16; ++pass) {
    for (size_t offset = 0; offset < toc.size(); offset += output.size()) {
      rijndaelDecrypt(round_keys.data(), rounds, toc.data() + offset, output.data());
      std::memcpy(toc.data() + offset, output.data(), output.size());
    }
  }
  return true;
}

std::optional<std::string> ReadName(std::span<const uint8_t> toc, size_t string_table_offset,
                                    uint32_t name_offset) {
  uint64_t offset = 0;
  if (!CheckedAdd(string_table_offset, name_offset, offset) || offset >= toc.size()) {
    return std::nullopt;
  }
  const auto* begin = reinterpret_cast<const char*>(toc.data() + offset);
  const size_t available = toc.size() - static_cast<size_t>(offset);
  const auto* end = static_cast<const char*>(std::memchr(begin, '\0', available));
  if (!end) {
    return std::nullopt;
  }
  std::string name(begin, static_cast<size_t>(end - begin));
  if (name.empty() || name == "." || name == ".." ||
      name.find_first_of("/\\:") != std::string::npos) {
    return std::nullopt;
  }
  return name;
}

std::optional<Archive> ParseArchive(const std::filesystem::path& path, std::span<const uint8_t> key,
                                    std::string& error) {
  std::error_code fs_error;
  const uint64_t file_size = std::filesystem::file_size(path, fs_error);
  if (fs_error || file_size < kTocOffset) {
    error = "RPF archive is missing or shorter than its header: " + path.string();
    return std::nullopt;
  }

  std::ifstream stream(path, std::ios::binary);
  std::array<uint8_t, kHeaderSize> header{};
  if (!stream.read(reinterpret_cast<char*>(header.data()), header.size()) ||
      ReadU32(header, 0) != kRpf2Magic) {
    error = "The selected game contains an invalid RPF2 archive: " + path.string();
    return std::nullopt;
  }
  const uint32_t toc_size = ReadU32(header, 4);
  const uint32_t entry_count = ReadU32(header, 8);
  const bool encrypted = ReadU32(header, 16) != 0;
  uint64_t entry_bytes = 0;
  uint64_t toc_end = 0;
  if (entry_count == 0 || entry_count > kMaximumEntryCount ||
      !CheckedMultiply(entry_count, kEntrySize, entry_bytes) || entry_bytes > toc_size ||
      !CheckedAdd(kTocOffset, toc_size, toc_end) || toc_end > file_size) {
    error = "The RPF2 table of contents is malformed: " + path.string();
    return std::nullopt;
  }
  if (encrypted && key.size() != kAesKeySize) {
    error = "The bundled GTA IV RPF AES key is missing or invalid.";
    return std::nullopt;
  }

  std::vector<uint8_t> toc(toc_size);
  stream.seekg(static_cast<std::streamoff>(kTocOffset), std::ios::beg);
  if (!stream.read(reinterpret_cast<char*>(toc.data()), toc.size())) {
    error = "Could not read the RPF2 table of contents: " + path.string();
    return std::nullopt;
  }
  if (encrypted && !CryptToc(toc, key)) {
    error = "Could not decrypt the RPF2 table of contents: " + path.string();
    return std::nullopt;
  }

  Archive archive;
  archive.entries.reserve(entry_count);
  const size_t string_table_offset = static_cast<size_t>(entry_bytes);
  for (uint32_t index = 0; index < entry_count; ++index) {
    const size_t offset = static_cast<size_t>(index) * kEntrySize;
    TocEntry entry;
    auto name = ReadName(toc, string_table_offset, ReadU32(toc, offset));
    if (!name && index != 0) {
      error = "The RPF2 archive contains an invalid filename: " + path.string();
      return std::nullopt;
    }
    entry.name = name.value_or("/");
    const uint32_t word1 = ReadU32(toc, offset + 4);
    const uint32_t word2 = ReadU32(toc, offset + 8);
    const uint32_t word3 = ReadU32(toc, offset + 12);
    entry.is_directory = (word2 & kDirectoryBit) != 0;
    if (entry.is_directory) {
      entry.content_index = word2 & ~kDirectoryBit;
      entry.content_count = word3 & 0x0FFFFFFF;
      uint64_t child_end = 0;
      if (!CheckedAdd(entry.content_index, entry.content_count, child_end) ||
          child_end > entry_count) {
        error = "The RPF2 archive contains an invalid directory range: " + path.string();
        return std::nullopt;
      }
    } else {
      entry.size = word1;
      entry.is_resource = (word3 & kResourceMask) == kResourceMask;
      if (entry.is_resource) {
        entry.offset = word2 & kResourceOffsetMask;
        entry.archived_size = entry.size;
      } else {
        entry.offset = word2;
        entry.archived_size = word3 & kArchivedSizeMask;
        entry.is_compressed = (word3 & kCompressedBit) != 0;
      }
      if (entry.size > kMaximumExtractedFileSize ||
          !RangeWithin(entry.offset, entry.archived_size, file_size)) {
        error = "The RPF2 archive contains an out-of-range file entry: " + path.string();
        return std::nullopt;
      }
    }
    archive.entries.push_back(std::move(entry));
  }
  if (!archive.entries.front().is_directory) {
    error = "The RPF2 root entry is not a directory: " + path.string();
    return std::nullopt;
  }
  return archive;
}

bool ValidateDirectoryGraph(const Archive& archive, size_t index,
                            const std::filesystem::path& archive_path, std::vector<bool>& active,
                            std::vector<bool>& visited, std::string& error) {
  if (index >= archive.entries.size() || active[index] || visited[index]) {
    error =
        "The RPF2 directory graph contains a cycle or duplicate entry: " + archive_path.string();
    return false;
  }
  const auto& directory = archive.entries[index];
  if (!directory.is_directory) {
    error = "The RPF2 directory graph references a file as a directory: " + archive_path.string();
    return false;
  }

  active[index] = true;
  visited[index] = true;
  std::unordered_set<std::string> child_names;
  const uint64_t end = uint64_t(directory.content_index) + directory.content_count;
  for (uint64_t child_index = directory.content_index; child_index < end; ++child_index) {
    const size_t child = static_cast<size_t>(child_index);
    const auto& entry = archive.entries[child];
    if (!child_names.insert(Lowercase(entry.name)).second) {
      error =
          "The RPF2 archive contains colliding case-insensitive paths: " + archive_path.string();
      return false;
    }
    if (visited[child]) {
      error =
          "The RPF2 directory graph references an entry more than once: " + archive_path.string();
      return false;
    }
    if (entry.is_directory) {
      if (!ValidateDirectoryGraph(archive, child, archive_path, active, visited, error)) {
        return false;
      }
    } else {
      visited[child] = true;
    }
  }
  active[index] = false;
  return true;
}

bool ValidateArchiveGraph(const Archive& archive, const std::filesystem::path& archive_path,
                          std::string& error) {
  std::vector<bool> active(archive.entries.size());
  std::vector<bool> visited(archive.entries.size());
  if (!ValidateDirectoryGraph(archive, 0, archive_path, active, visited, error)) {
    return false;
  }
  if (!std::all_of(visited.begin(), visited.end(), [](bool value) { return value; })) {
    error = "The RPF2 archive contains unreachable table entries: " + archive_path.string();
    return false;
  }
  return true;
}

bool EnsureFreeSpace(const std::filesystem::path& destination, uint64_t required,
                     std::string& error) {
  std::error_code fs_error;
  const auto space = std::filesystem::space(destination, fs_error);
  if (fs_error) {
    error = "Could not determine free space for RPF extraction: " + fs_error.message();
    return false;
  }
  if (space.available < required) {
    error = "There is not enough free space to extract the GTA IV archives.";
    return false;
  }
  return true;
}

bool WriteEntry(const std::filesystem::path& archive_path, const TocEntry& entry,
                const std::filesystem::path& output_path, const RpfExtractionProgress& progress,
                std::string& error) {
  if (IsCancelled(progress)) {
    error = "Installation was cancelled.";
    return false;
  }
  std::ifstream input(archive_path, std::ios::binary);
  input.seekg(entry.offset, std::ios::beg);
  if (!input) {
    error = "Could not read an RPF2 file entry from " + archive_path.string() + ".";
    return false;
  }
  if (!entry.is_compressed && entry.archived_size != entry.size) {
    error =
        "An uncompressed RPF2 file entry has inconsistent sizes in " + archive_path.string() + ".";
    return false;
  }

  std::error_code fs_error;
  std::filesystem::create_directories(output_path.parent_path(), fs_error);
  if (fs_error) {
    error = "Could not create an RPF extraction directory: " + fs_error.message();
    return false;
  }
  std::ofstream destination(output_path, std::ios::binary | std::ios::trunc);
  if (!destination) {
    error = "Could not write an extracted RPF2 file: " + output_path.string();
    return false;
  }

  std::vector<uint8_t> input_buffer(kIoBufferSize);
  std::vector<uint8_t> output_buffer(kIoBufferSize);
  uint64_t remaining = entry.archived_size;
  if (!entry.is_compressed) {
    while (remaining != 0) {
      if (IsCancelled(progress)) {
        error = "Installation was cancelled.";
        return false;
      }
      const size_t chunk = static_cast<size_t>(std::min<uint64_t>(remaining, input_buffer.size()));
      if (!input.read(reinterpret_cast<char*>(input_buffer.data()), chunk) ||
          !destination.write(reinterpret_cast<const char*>(input_buffer.data()), chunk)) {
        error = "Could not copy an RPF2 file entry from " + archive_path.string() + ".";
        return false;
      }
      remaining -= chunk;
      if (progress.completed_bytes) {
        progress.completed_bytes->fetch_add(chunk, std::memory_order_relaxed);
      }
    }
  } else {
    uint64_t written = 0;
    z_stream stream{};
    if (inflateInit2(&stream, -MAX_WBITS) != Z_OK) {
      error = "Could not initialize RPF2 decompression.";
      return false;
    }
    bool complete = false;
    while (!complete) {
      if (IsCancelled(progress)) {
        inflateEnd(&stream);
        error = "Installation was cancelled.";
        return false;
      }
      if (stream.avail_in == 0 && remaining != 0) {
        const size_t chunk =
            static_cast<size_t>(std::min<uint64_t>(remaining, input_buffer.size()));
        if (!input.read(reinterpret_cast<char*>(input_buffer.data()), chunk)) {
          inflateEnd(&stream);
          error = "Could not read a compressed RPF2 file entry from " + archive_path.string() + ".";
          return false;
        }
        remaining -= chunk;
        stream.next_in = input_buffer.data();
        stream.avail_in = static_cast<uInt>(chunk);
      }

      stream.next_out = output_buffer.data();
      stream.avail_out = static_cast<uInt>(output_buffer.size());
      const int flush = remaining == 0 && stream.avail_in == 0 ? Z_FINISH : Z_NO_FLUSH;
      const int status = inflate(&stream, flush);
      const size_t produced = output_buffer.size() - stream.avail_out;
      if (produced != 0) {
        if (!destination.write(reinterpret_cast<const char*>(output_buffer.data()), produced)) {
          inflateEnd(&stream);
          error = "Could not write a decompressed RPF2 file entry: " + output_path.string();
          return false;
        }
        written += produced;
        if (written > entry.size) {
          inflateEnd(&stream);
          error = "A compressed RPF2 entry expands beyond its declared size.";
          return false;
        }
        if (progress.completed_bytes) {
          progress.completed_bytes->fetch_add(produced, std::memory_order_relaxed);
        }
      }
      if (status == Z_STREAM_END) {
        complete = true;
      } else if (status != Z_OK || (produced == 0 && stream.avail_in == 0 && remaining == 0)) {
        inflateEnd(&stream);
        error = "Could not decompress an RPF2 file entry from " + archive_path.string() + ".";
        return false;
      }
    }
    const bool exact = remaining == 0 && stream.avail_in == 0 && written == entry.size;
    inflateEnd(&stream);
    if (!exact) {
      error = "A compressed RPF2 entry does not match its declared size.";
      return false;
    }
  }
  destination.flush();
  if (!destination) {
    error = "Could not finish an extracted RPF2 file: " + output_path.string();
    return false;
  }
  return true;
}

bool ExtractDirectory(const std::filesystem::path& archive_path, const Archive& archive,
                      size_t index, const std::filesystem::path& output_root,
                      std::vector<bool>& active, std::vector<bool>& visited,
                      std::vector<std::filesystem::path>& nested_archives,
                      const RpfExtractionProgress& progress, std::string& error) {
  if (index >= archive.entries.size() || active[index] || visited[index]) {
    error =
        "The RPF2 directory graph contains a cycle or duplicate entry: " + archive_path.string();
    return false;
  }
  const auto& directory = archive.entries[index];
  if (!directory.is_directory) {
    error = "The RPF2 directory graph references a file as a directory: " + archive_path.string();
    return false;
  }
  active[index] = true;
  visited[index] = true;
  const uint64_t end = uint64_t(directory.content_index) + directory.content_count;
  for (uint64_t child_index = directory.content_index; child_index < end; ++child_index) {
    const size_t child = static_cast<size_t>(child_index);
    if (visited[child]) {
      error =
          "The RPF2 directory graph references an entry more than once: " + archive_path.string();
      return false;
    }
    const auto& entry = archive.entries[child];
    const auto output_path = output_root / entry.name;
    if (entry.is_directory) {
      std::error_code fs_error;
      std::filesystem::create_directories(output_path, fs_error);
      if (fs_error || !ExtractDirectory(archive_path, archive, child, output_path, active, visited,
                                        nested_archives, progress, error)) {
        if (error.empty()) {
          error = "Could not create an RPF extraction directory: " + fs_error.message();
        }
        return false;
      }
    } else {
      visited[child] = true;
      if (!WriteEntry(archive_path, entry, output_path, progress, error)) {
        return false;
      }
      std::string extension = output_path.extension().string();
      std::transform(extension.begin(), extension.end(), extension.begin(),
                     [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
      if (extension == ".rpf") {
        nested_archives.push_back(output_path);
      }
    }
  }
  active[index] = false;
  return true;
}

bool ExtractOne(const std::filesystem::path& archive_path, const std::filesystem::path& output_root,
                std::span<const uint8_t> key, const RpfExtractionProgress& progress,
                std::vector<std::filesystem::path>& nested_archives, std::string& error) {
  auto archive = ParseArchive(archive_path, key, error);
  if (!archive) {
    return false;
  }
  if (!ValidateArchiveGraph(*archive, archive_path, error)) {
    return false;
  }
  uint64_t extracted_size = 0;
  for (const auto& entry : archive->entries) {
    if (!entry.is_directory && !CheckedAdd(extracted_size, entry.size, extracted_size)) {
      error = "The RPF2 extracted size exceeds the supported range.";
      return false;
    }
  }
  if (!EnsureFreeSpace(output_root.parent_path(), extracted_size, error)) {
    return false;
  }
  if (progress.total_bytes) {
    progress.total_bytes->fetch_add(extracted_size, std::memory_order_relaxed);
  }
  std::error_code fs_error;
  std::filesystem::create_directories(output_root, fs_error);
  if (fs_error) {
    error = "Could not create an RPF extraction directory: " + fs_error.message();
    return false;
  }
  std::vector<bool> active(archive->entries.size());
  std::vector<bool> visited(archive->entries.size());
  if (!ExtractDirectory(archive_path, *archive, 0, output_root, active, visited, nested_archives,
                        progress, error)) {
    return false;
  }
  return true;
}

}  // namespace

bool ValidateRpfArchive(const std::filesystem::path& archive_path,
                        const std::filesystem::path& aes_key_path, std::string& error) {
  auto key = ReadExactFile(aes_key_path, kAesKeySize);
  if (!key) {
    error = "The bundled GTA IV RPF AES key could not be read.";
    return false;
  }
  auto archive = ParseArchive(archive_path, *key, error);
  return archive && ValidateArchiveGraph(*archive, archive_path, error);
}

bool ExtractGameArchives(const std::filesystem::path& staged_game_root,
                         const std::filesystem::path& aes_key_path,
                         const RpfExtractionProgress& progress, std::string& error) {
  auto key = ReadExactFile(aes_key_path, kAesKeySize);
  if (!key) {
    error = "The bundled GTA IV RPF AES key could not be read.";
    return false;
  }

  const std::array<std::pair<std::string_view, std::string_view>, 3> roots = {{
      {"common.rpf", "common"},
      {"xbox360.rpf", "xbox360"},
      {"audio.rpf", "audio"},
  }};
  std::vector<std::tuple<std::filesystem::path, std::filesystem::path, size_t>> pending;
  for (const auto& [archive_name, directory_name] : roots) {
    const auto archive_path = staged_game_root / archive_name;
    std::error_code fs_error;
    if (!std::filesystem::is_regular_file(archive_path, fs_error)) {
      error = "The GTA IV source is missing " + std::string(archive_name) + ".";
      return false;
    }
    pending.emplace_back(archive_path, staged_game_root / directory_name, 0);
  }

  std::unordered_set<std::string> extracted;
  size_t archive_count = 0;
  while (!pending.empty()) {
    if (++archive_count > kMaximumNestedArchiveCount) {
      error = "The GTA IV source contains too many nested RPF archives.";
      return false;
    }
    auto [archive_path, output_root, depth] = std::move(pending.back());
    pending.pop_back();
    if (depth > kMaximumArchiveDepth) {
      error = "The GTA IV source contains an excessively deep nested RPF archive tree.";
      return false;
    }
    const auto key_path = archive_path.lexically_normal().generic_string();
    if (!extracted.insert(key_path).second) {
      error = "The GTA IV source contains a repeated nested RPF archive path.";
      return false;
    }
    std::vector<std::filesystem::path> nested;
    if (!ExtractOne(archive_path, output_root, *key, progress, nested, error)) {
      return false;
    }
    for (auto& child : nested) {
      pending.emplace_back(child, child.parent_path() / child.stem(), depth + 1);
    }
  }
  return true;
}

}  // namespace gta4::install
