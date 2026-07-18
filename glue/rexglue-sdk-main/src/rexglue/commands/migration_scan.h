/**
 * @file        rexglue/commands/migration_scan.h
 * @brief       Project-tree scanners for SDK upgrade-driven migrations
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 */

#pragma once

#include <cstddef>
#include <filesystem>
#include <span>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

namespace rexglue::cli {

enum class OverwriteAction {
  Write,
  Delete,
};

struct OverwriteEntry {
  std::filesystem::path path;
  std::string rendered_content;
  OverwriteAction action = OverwriteAction::Write;
  bool silent = false;
  std::string reason;
};

struct MigrationWarning {
  std::filesystem::path file;
  std::size_t line_number;
  std::string raw_line;
  std::string detail;
  std::string hint;
};

struct MigrationFindings {
  std::vector<OverwriteEntry> rewrites;
  std::vector<MigrationWarning> warnings;
};

struct BreakingChangeRule {
  std::string_view legacy_token;
  std::string_view replacement;
  std::string_view reason;
};

struct CallSiteRule {
  std::string_view pattern;
  std::string_view detail;
  std::string_view hint;
  bool (*confirm)(std::string_view line) = nullptr;
};

std::span<const BreakingChangeRule> DefaultBreakingChangeRules();
std::span<const CallSiteRule> DefaultCallSiteRules();

std::string RenderRexglueCmake(std::string_view project_name, std::string_view sdk_version,
                               std::string_view entrypoint_out_dir);

std::vector<OverwriteEntry> ScanSdkTemplateDrift(const std::filesystem::path& project_root,
                                                 std::string_view project_name,
                                                 std::string_view sdk_version,
                                                 std::string_view entrypoint_out_dir);

std::vector<OverwriteEntry> ScanCmakeReferences(const std::filesystem::path& project_root,
                                                std::string_view legacy_filename,
                                                std::string_view manifest_filename);

std::vector<OverwriteEntry> ScanSourceIncludeRewrites(const std::filesystem::path& project_root,
                                                      std::string_view project_name);

MigrationFindings ScanLegacyIdentifiers(
    const std::filesystem::path& project_root,
    std::span<const BreakingChangeRule> rules = DefaultBreakingChangeRules());

std::vector<MigrationWarning> ScanCallSitePatterns(
    const std::filesystem::path& project_root,
    std::span<const CallSiteRule> rules = DefaultCallSiteRules());

std::vector<MigrationWarning> ScanStaleIncludes(
    const std::filesystem::path& src_dir, const std::unordered_set<std::string>& removed_basenames);

}  // namespace rexglue::cli
