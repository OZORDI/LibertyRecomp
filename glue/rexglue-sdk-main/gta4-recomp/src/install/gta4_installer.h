#pragma once

#include <atomic>
#include <filesystem>
#include <string>
#include <vector>

namespace gta4::install {

enum class Episode {
  kTlad,
  kTbogt,
};

struct DlcSelection {
  Episode episode;
  std::filesystem::path source;
};

struct Selection {
  std::filesystem::path game_source;
  std::filesystem::path update_source;
  std::vector<DlcSelection> dlc_sources;
};

struct Progress {
  std::atomic<uint64_t> copied_bytes{0};
  std::atomic<uint64_t> total_bytes{0};
  std::atomic<bool> cancel_requested{false};
};

struct Result {
  bool success = false;
  std::string error;
};

bool IsInstallReady(const std::filesystem::path& game_root, std::string* reason = nullptr);
bool IsEpisodeReady(const std::filesystem::path& marketplace_root, Episode episode);
Result VerifyInstall(const std::filesystem::path& game_root,
                     const std::filesystem::path& marketplace_root);
Result Install(const Selection& selection, const std::filesystem::path& install_root,
               Progress& progress);

const char* EpisodeDirectory(Episode episode);

}  // namespace gta4::install
