#pragma once

#include <atomic>
#include <filesystem>
#include <string>

namespace gta4::install {

struct RpfExtractionProgress {
  std::atomic<uint64_t>* completed_bytes = nullptr;
  std::atomic<uint64_t>* total_bytes = nullptr;
  const std::atomic<bool>* cancel_requested = nullptr;
};

// Parses and validates an RPF2 archive without extracting it. This runs the
// same header, encrypted-TOC, extent, directory-graph, and path-collision
// checks used by ExtractGameArchives.
bool ValidateRpfArchive(const std::filesystem::path& archive_path,
                        const std::filesystem::path& aes_key_path, std::string& error);

// Extracts the three GTA IV RPF2 roots while preserving the original archives.
// Nested RPF2 archives are expanded beside themselves into a directory named
// after the archive stem. All writes must target a private staging directory.
bool ExtractGameArchives(const std::filesystem::path& staged_game_root,
                         const std::filesystem::path& aes_key_path,
                         const RpfExtractionProgress& progress, std::string& error);

}  // namespace gta4::install
