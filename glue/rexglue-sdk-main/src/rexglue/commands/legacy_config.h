/**
 * @file        rexglue/commands/legacy_config.h
 * @brief       One-shot conversion of pre-manifest single-file configs
 *
 * @copyright   Copyright (c) 2026 Tom Clay
 * @license     BSD 3-Clause License
 */

#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace rexglue::cli {

struct LegacyConversion {
  std::filesystem::path manifest_path;
  std::filesystem::path legacy_path;
  std::string project_name;
  std::string out_directory_path;
  std::string manifest_content;
  std::string stripped_legacy_content;
};

std::optional<LegacyConversion> ConvertLegacyConfig(const std::filesystem::path& legacy_path);

}  // namespace rexglue::cli
