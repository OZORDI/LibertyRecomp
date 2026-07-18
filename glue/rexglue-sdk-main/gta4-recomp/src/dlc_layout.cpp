#include "dlc_layout.h"

#include <string_view>
#include <system_error>

#include <rex/logging.h>

namespace gta4 {
namespace {

struct DlcMapping {
  std::string_view package_name;
  std::string_view legacy_game_directory;
};

bool IsExtractedPackageReady(const std::filesystem::path& path) {
  std::error_code error;
  return std::filesystem::is_directory(path, error) &&
         std::filesystem::is_regular_file(path / "setup2.xml", error) &&
         std::filesystem::is_regular_file(path / "content.dat", error);
}

bool LooksLikeExtractedPackage(const std::filesystem::path& path) {
  std::error_code error;
  if (!std::filesystem::is_directory(path, error) || error) {
    return false;
  }

  error.clear();
  return std::filesystem::is_regular_file(path / "setup2.xml", error) && !error;
}

bool PublishPackage(const std::filesystem::path& source_path,
                    const std::filesystem::path& legacy_package_path,
                    const std::filesystem::path& package_path) {
  if (IsExtractedPackageReady(package_path)) {
    return true;
  }

  std::error_code error;
  if (std::filesystem::exists(package_path, error)) {
    std::filesystem::remove_all(package_path, error);
    if (error) {
      REXLOG_WARN("Unable to replace incomplete DLC package {}: {}", package_path.string(),
                  error.message());
      return false;
    }
  }

  if (IsExtractedPackageReady(legacy_package_path)) {
    error.clear();
    std::filesystem::rename(legacy_package_path, package_path, error);
    if (!error && IsExtractedPackageReady(package_path)) {
      REXLOG_INFO("Migrated DLC marketplace package: {} -> {}", legacy_package_path.string(),
                  package_path.string());
      return true;
    }

    if (!error) {
      std::filesystem::remove_all(package_path, error);
      if (error) {
        REXLOG_WARN("Unable to remove incomplete migrated DLC package {}: {}",
                    package_path.string(), error.message());
        return false;
      }
    }
  }

  auto staging_path = package_path;
  staging_path += ".libertyrecomp-staging";

  error.clear();
  std::filesystem::remove_all(staging_path, error);
  error.clear();
  std::filesystem::copy(
      source_path, staging_path,
      std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing,
      error);

  if (!error && IsExtractedPackageReady(staging_path)) {
    std::filesystem::rename(staging_path, package_path, error);
  }

  if (!error && IsExtractedPackageReady(package_path)) {
    REXLOG_INFO("Staged DLC marketplace package: {} -> {}", source_path.string(),
                package_path.string());
    return true;
  }

  if (error) {
    REXLOG_WARN("Unable to stage DLC marketplace package {}: {}", source_path.string(),
                error.message());
  } else {
    REXLOG_WARN("Staged DLC marketplace package is incomplete: {}", staging_path.string());
  }

  std::error_code cleanup_error;
  std::filesystem::remove_all(staging_path, cleanup_error);
  cleanup_error.clear();
  std::filesystem::remove_all(package_path, cleanup_error);
  return false;
}

void RemoveLegacyExtractedCopy(const std::filesystem::path& path) {
  if (!LooksLikeExtractedPackage(path)) {
    return;
  }

  std::error_code error;
  const bool has_archive = std::filesystem::is_regular_file(path / "DLC.rpf", error);
  if (error || has_archive) {
    return;
  }

  error.clear();
  std::filesystem::remove_all(path, error);
  if (error) {
    REXLOG_WARN("Unable to remove obsolete extracted disc-DLC bridge {}: {}", path.string(),
                error.message());
  } else {
    REXLOG_INFO("Removed obsolete extracted disc-DLC bridge: {}", path.string());
  }
}

}  // namespace

void PrepareDlcLayout(const std::filesystem::path& game_data_root) {
  if (game_data_root.empty()) {
    return;
  }

  const auto source_root = game_data_root.parent_path() / "dlc";
  const auto marketplace_root = game_data_root / "0000000000000000" / "545407F2" / "00000002";
  const auto legacy_marketplace_root = game_data_root / "545407F2" / "00000002";
  const DlcMapping mappings[] = {
      {"TLAD", "DLC1"},
      {"TBOGT", "DLC2"},
  };

  std::error_code error;
  std::filesystem::create_directories(marketplace_root, error);
  if (error) {
    REXLOG_WARN("Unable to create DLC marketplace root {}: {}", marketplace_root.string(),
                error.message());
    return;
  }

  for (const auto& mapping : mappings) {
    const auto source_path = source_root / mapping.package_name;
    if (!IsExtractedPackageReady(source_path)) {
      continue;
    }

    const auto package_path = marketplace_root / mapping.package_name;
    const auto legacy_package_path = legacy_marketplace_root / mapping.package_name;
    if (!PublishPackage(source_path, legacy_package_path, package_path)) {
      continue;
    }

    if (LooksLikeExtractedPackage(legacy_package_path)) {
      error.clear();
      std::filesystem::remove_all(legacy_package_path, error);
      if (error) {
        REXLOG_WARN("Unable to remove obsolete DLC marketplace copy {}: {}",
                    legacy_package_path.string(), error.message());
      } else {
        REXLOG_INFO("Removed obsolete DLC marketplace copy: {}", legacy_package_path.string());
      }
    }

    RemoveLegacyExtractedCopy(game_data_root / mapping.legacy_game_directory);
  }

  // These calls remove empty directories only, preserving any unrelated content.
  error.clear();
  std::filesystem::remove(legacy_marketplace_root, error);
  error.clear();
  std::filesystem::remove(legacy_marketplace_root.parent_path(), error);
}

}  // namespace gta4
