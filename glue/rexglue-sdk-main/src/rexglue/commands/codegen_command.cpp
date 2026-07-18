/**
 * @file        rexglue/commands/codegen_command.cpp
 * @brief       Code generation command implementation
 *
 * @copyright   Copyright (c) 2026 Tom Clay
 * @license     BSD 3-Clause License
 */

#include "codegen_command.h"
#include "../ui/progress.h"
#include "../ui/ui.h"
#include "legacy_config.h"
#include "migration_scan.h"
#include "template_utils.h"

#include <algorithm>
#include <filesystem>
#include <memory>
#include <span>
#include <string_view>
#include <unordered_set>
#include <vector>

#include <CLI/CLI.hpp>
#include <fmt/format.h>
#include <toml++/toml.hpp>

#include <rex/codegen/manifest.h>
#include <rex/codegen/project_recompiler.h>
#include <rex/logging.h>
#include <rex/version.h>

namespace rexglue::cli {

namespace {

namespace fs = std::filesystem;

// Compare dot-separated numeric version strings (e.g. "0.7.5"). Trailing
// pre-release/build metadata after '-' or '+' is ignored. Returns true when
// lhs orders strictly before rhs.
bool VersionLess(std::string_view lhs, std::string_view rhs) {
  auto parse = [](std::string_view s) {
    std::vector<unsigned> parts;
    unsigned value = 0;
    bool in_number = false;
    for (char c : s) {
      if (c >= '0' && c <= '9') {
        value = value * 10 + static_cast<unsigned>(c - '0');
        in_number = true;
        continue;
      }
      if (in_number)
        parts.push_back(value);
      value = 0;
      in_number = false;
      if (c == '-' || c == '+')
        break;
    }
    if (in_number)
      parts.push_back(value);
    return parts;
  };
  auto a = parse(lhs);
  auto b = parse(rhs);
  for (std::size_t i = 0; i < std::max(a.size(), b.size()); ++i) {
    unsigned va = i < a.size() ? a[i] : 0;
    unsigned vb = i < b.size() ? b[i] : 0;
    if (va != vb)
      return va < vb;
  }
  return false;
}

// The upgrade scan only runs when the installed SDK is newer than the version
// that last generated this project (or when the project was never stamped).
bool MigrationNeeded(std::string_view last_codegen_version, std::string_view installed_version) {
  return last_codegen_version.empty() || VersionLess(last_codegen_version, installed_version);
}

struct ActionStrings {
  std::string_view verb;
  std::string_view label;
};

ActionStrings ActionStringsFor(OverwriteAction action) {
  switch (action) {
    case OverwriteAction::Write:
      return {"Wrote", "write "};
    case OverwriteAction::Delete:
      return {"Deleted", "delete"};
  }
  return {"Touched", "?     "};
}

bool ApplyEntry(const OverwriteEntry& entry) {
  std::error_code ec;
  switch (entry.action) {
    case OverwriteAction::Write: {
      if (auto parent = entry.path.parent_path(); !parent.empty()) {
        fs::create_directories(parent, ec);
        if (ec) {
          REXLOG_ERROR("Failed to create directory for {}: {}", entry.path.string(), ec.message());
          return false;
        }
      }
      return write_file_atomic(entry.path, entry.rendered_content);
    }
    case OverwriteAction::Delete:
      if (!fs::exists(entry.path))
        return true;
      if (!fs::remove(entry.path, ec) || ec) {
        REXLOG_ERROR("Failed to delete {}: {}", entry.path.string(), ec.message());
        return false;
      }
      return true;
  }
  return false;
}

Result<void> ApplyPlan(const std::vector<OverwriteEntry>& plan) {
  for (const auto& entry : plan) {
    if (!ApplyEntry(entry)) {
      return Err<void>(rex::ErrorCategory::IO,
                       fmt::format("Failed to apply: {}", entry.path.string()));
    }
    REXLOG_TRACE("{}: {}", ActionStringsFor(entry.action).verb, entry.path.generic_string());
  }
  return rex::Ok();
}

Result<void> PromptConsent(const std::vector<OverwriteEntry>& plan, bool force) {
  std::vector<ui::PlanRow> rows;
  for (const auto& e : plan) {
    if (e.silent)
      continue;
    rows.push_back({ActionStringsFor(e.action).label, e.path.generic_string(), e.reason});
  }
  if (rows.empty())
    return rex::Ok();

  ui::PlanTable(
      fmt::format("Migration: {} file(s) will be rewritten before codegen runs.", rows.size()),
      rows);

  if (force)
    return rex::Ok();
  if (!ui::Confirm("Apply migration and continue?")) {
    return Err<void>(rex::ErrorCategory::UserAbort, "Upgrade declined; codegen aborted.");
  }
  return rex::Ok();
}

void EmitManualReview(std::span<const MigrationWarning> warnings, std::string_view header) {
  if (warnings.empty())
    return;
  std::vector<ui::ManualReviewRow> rows;
  rows.reserve(warnings.size());
  for (const auto& w : warnings) {
    rows.push_back(
        {fmt::format("{}:{}", w.file.generic_string(), w.line_number), w.detail, w.hint});
  }
  ui::ManualReviewList(std::string(header), rows);
}

MigrationFindings ScanProjectMigrations(const fs::path& project_dir, std::string_view project_name,
                                        std::string_view sdk_version,
                                        std::string_view entrypoint_out_dir) {
  MigrationFindings out;
  auto add_rewrites = [&](std::vector<OverwriteEntry> entries) {
    out.rewrites.insert(out.rewrites.end(), std::make_move_iterator(entries.begin()),
                        std::make_move_iterator(entries.end()));
  };
  auto add_warnings = [&](std::vector<MigrationWarning> entries) {
    out.warnings.insert(out.warnings.end(), std::make_move_iterator(entries.begin()),
                        std::make_move_iterator(entries.end()));
  };

  add_rewrites(ScanSdkTemplateDrift(project_dir, project_name, sdk_version, entrypoint_out_dir));
  add_rewrites(ScanSourceIncludeRewrites(project_dir, project_name));
  auto idents = ScanLegacyIdentifiers(project_dir);
  add_rewrites(std::move(idents.rewrites));
  add_warnings(std::move(idents.warnings));
  add_warnings(ScanCallSitePatterns(project_dir));
  return out;
}

void ReportStaleIncludes(const fs::path& manifest_path,
                         const rex::codegen::ProjectRecompiler& recompiler) {
  std::unordered_set<std::string> written(recompiler.writtenFiles().begin(),
                                          recompiler.writtenFiles().end());
  std::unordered_set<std::string> removed;
  for (const auto& f : recompiler.deletedFiles()) {
    if (!written.contains(f)) {
      removed.insert(f);
    }
  }
  if (removed.empty())
    return;

  auto matches = ScanStaleIncludes(manifest_path.parent_path() / "src", removed);
  EmitManualReview(matches,
                   fmt::format("{} source file(s) reference headers no longer emitted by codegen:",
                               matches.size()));
}

Result<void> RecompileProject(const fs::path& manifest_path, const CliContext& ctx,
                              const std::vector<std::string>& targets) {
  auto manifest = rex::codegen::ManifestConfig::Load(manifest_path);
  if (!manifest) {
    return Err<void>(rex::ErrorCategory::Config, "Failed to load manifest");
  }
  std::string title = fmt::format("Recompiling {}", manifest->projectName);
  rex::codegen::ProjectRecompiler recompiler(std::move(*manifest));
  ui::ProgressView progress(title);
  rex::codegen::ProjectRecompilerOptions opts{
      .targets = targets,
      .force = ctx.generate_despite_errors,
      .reporter = &progress,
  };
  auto result = recompiler.Run(opts);
  if (!result)
    return result;

  ReportStaleIncludes(manifest_path, recompiler);
  return rex::Ok();
}

struct ManifestSummary {
  std::string project_name;
  std::string sdk_version;
  std::string entrypoint_out_dir;
  std::size_t module_count = 0;
};

rex::Result<ManifestSummary> LoadManifestSummary(const fs::path& manifest_path) {
  auto manifest = rex::codegen::ManifestConfig::Load(manifest_path);
  if (!manifest) {
    return Err<ManifestSummary>(rex::ErrorCategory::Config, "Failed to load manifest");
  }
  return rex::Ok(ManifestSummary{
      .project_name = manifest->projectName,
      .sdk_version = manifest->sdkVersion.value_or(""),
      .entrypoint_out_dir = manifest->entrypoint.recompiler.outDirectoryPath,
      .module_count = manifest->modules.size(),
  });
}

void EmitProjectHeader(const fs::path& manifest_path, const ManifestSummary& summary) {
  std::vector<ui::KeyValueRow> rows;
  rows.push_back({"Manifest", manifest_path.generic_string()});
  if (!summary.project_name.empty()) {
    rows.push_back({"Project", summary.project_name});
  }
  if (summary.module_count > 0) {
    rows.push_back({"Modules", fmt::format("{} DLL{}", summary.module_count,
                                           summary.module_count == 1 ? "" : "s")});
  }
  if (!summary.sdk_version.empty()) {
    rows.push_back({"SDK", fmt::format("{} (project last generated by {})", REXGLUE_VERSION_STRING,
                                       summary.sdk_version)});
  } else {
    rows.push_back({"SDK", REXGLUE_VERSION_STRING});
  }
  ui::KeyValueBlock("", rows);
}

struct CodegenArgs {
  std::string config_path;
  std::vector<std::string> targets;
};

}  // namespace

Result<std::string> DiscoverManifestInCwd() {
  std::error_code ec;
  fs::path cwd = fs::current_path(ec);
  if (ec) {
    return Err<std::string>(rex::ErrorCategory::IO,
                            fmt::format("Cannot read current directory: {}", ec.message()));
  }

  std::vector<fs::path> manifests;
  std::vector<fs::path> other_tomls;
  for (const auto& entry : fs::directory_iterator(cwd, ec)) {
    if (ec)
      break;
    if (!entry.is_regular_file())
      continue;
    if (entry.path().extension() != ".toml")
      continue;
    auto stem = entry.path().stem().string();
    if (stem.size() >= 9 && std::string_view{stem}.substr(stem.size() - 9) == "_manifest") {
      manifests.push_back(entry.path());
    } else {
      other_tomls.push_back(entry.path());
    }
  }

  if (manifests.size() == 1)
    return rex::Ok(manifests.front().string());
  if (manifests.size() > 1) {
    return Err<std::string>(
        rex::ErrorCategory::Config,
        fmt::format("Multiple *_manifest.toml files in {}; pass one explicitly", cwd.string()));
  }
  if (other_tomls.size() == 1)
    return rex::Ok(other_tomls.front().string());
  if (other_tomls.size() > 1) {
    return Err<std::string>(
        rex::ErrorCategory::Config,
        fmt::format("Multiple .toml files in {}; pass the manifest explicitly", cwd.string()));
  }
  return Err<std::string>(rex::ErrorCategory::Config,
                          fmt::format("No manifest .toml found in {}", cwd.string()));
}

Result<void> CodegenFromConfig(const std::string& config_path, const CliContext& ctx,
                               const std::vector<std::string>& targets) {
  REXLOG_TRACE("Generating code with config: {}", config_path);

  toml::table parsed_tbl;
  try {
    parsed_tbl = toml::parse_file(config_path);
  } catch (const toml::parse_error& err) {
    return Err<void>(rex::ErrorCategory::Config,
                     fmt::format("Failed to parse {}: {}", config_path, err.what()));
  }

  std::vector<OverwriteEntry> pre_plan;
  std::vector<OverwriteEntry> post_plan;
  std::vector<MigrationWarning> warnings;
  bool from_legacy = false;

  fs::path manifest_path;
  ManifestSummary summary;
  const std::string current_version = REXGLUE_VERSION_FLOOR;

  auto append_findings = [&](MigrationFindings findings) {
    post_plan.insert(post_plan.end(), std::make_move_iterator(findings.rewrites.begin()),
                     std::make_move_iterator(findings.rewrites.end()));
    warnings.insert(warnings.end(), std::make_move_iterator(findings.warnings.begin()),
                    std::make_move_iterator(findings.warnings.end()));
  };

  if (parsed_tbl.contains("project")) {
    manifest_path = config_path;
    auto loaded = LoadManifestSummary(manifest_path);
    if (!loaded)
      return Err<void>(loaded.error());
    summary = std::move(*loaded);
    if (MigrationNeeded(summary.sdk_version, current_version)) {
      append_findings(ScanProjectMigrations(manifest_path.parent_path(), summary.project_name,
                                            current_version, summary.entrypoint_out_dir));
    }
  } else {
    fs::path legacy_path = config_path;
    auto converted = ConvertLegacyConfig(legacy_path);
    if (!converted) {
      return Err<void>(
          rex::ErrorCategory::Config,
          fmt::format("Cannot convert legacy config {}: missing project_name or file_path",
                      legacy_path.string()));
    }
    manifest_path = converted->manifest_path;
    summary.project_name = converted->project_name;
    summary.entrypoint_out_dir = converted->out_directory_path;

    if (fs::exists(manifest_path)) {
      REXLOG_WARN("Both {} and {} exist; using the manifest. Remove the legacy file when ready.",
                  manifest_path.filename().string(), legacy_path.filename().string());
      auto loaded = LoadManifestSummary(manifest_path);
      if (!loaded)
        return Err<void>(loaded.error());
      summary = std::move(*loaded);
      if (MigrationNeeded(summary.sdk_version, current_version)) {
        append_findings(ScanProjectMigrations(manifest_path.parent_path(), summary.project_name,
                                              current_version, summary.entrypoint_out_dir));
      }
    } else {
      from_legacy = true;
      pre_plan.push_back({manifest_path, std::move(converted->manifest_content),
                          OverwriteAction::Write, /*silent=*/false,
                          fmt::format("upgrade format to v{}", current_version)});
      if (!converted->stripped_legacy_content.empty()) {
        post_plan.push_back({legacy_path, std::move(converted->stripped_legacy_content),
                             OverwriteAction::Write, /*silent=*/false,
                             "strip absorbed fields (kept as include target)"});
      } else {
        post_plan.push_back({legacy_path, "", OverwriteAction::Delete, /*silent=*/false,
                             "absorbed into the new manifest"});
      }

      auto cmake_rewrites =
          ScanCmakeReferences(legacy_path.parent_path(), legacy_path.filename().string(),
                              manifest_path.filename().string());
      post_plan.insert(post_plan.end(), std::make_move_iterator(cmake_rewrites.begin()),
                       std::make_move_iterator(cmake_rewrites.end()));

      append_findings(ScanProjectMigrations(legacy_path.parent_path(), summary.project_name,
                                            current_version, summary.entrypoint_out_dir));
    }
  }

  EmitProjectHeader(manifest_path, summary);
  EmitManualReview(warnings,
                   fmt::format("Migration: {} site(s) need manual review:", warnings.size()));

  std::vector<OverwriteEntry> consent_view = pre_plan;
  consent_view.insert(consent_view.end(), post_plan.begin(), post_plan.end());
  if (auto consent = PromptConsent(consent_view, ctx.skip_upgrade_consent); !consent)
    return consent;

  if (auto applied = ApplyPlan(pre_plan); !applied)
    return applied;

  if (auto run_result = RecompileProject(manifest_path, ctx, targets); !run_result) {
    if (from_legacy) {
      std::error_code ec;
      fs::remove(manifest_path, ec);
      REXLOG_ERROR("Codegen failed; manifest write rolled back. Legacy config is unchanged.");
    }
    return run_result;
  }

  if (auto applied = ApplyPlan(post_plan); !applied)
    return applied;

  if (!rex::codegen::ManifestConfig::WriteSdkVersionStamp(manifest_path, current_version)) {
    REXLOG_WARN("Failed to stamp manifest sdkVersion; next run may re-prompt");
  }
  return rex::Ok();
}

void RegisterCodegen(CLI::App& parent, const CliContext& ctx, DeferredAction& pending) {
  auto args = std::make_shared<CodegenArgs>();
  auto* sub = parent.add_subcommand("codegen", "Analyze and generate C++ code")->fallthrough();
  sub->add_option("config", args->config_path,
                  "Path to manifest TOML (auto-discovered in cwd if omitted)")
      ->type_name("PATH");
  sub->add_option("--target", args->targets,
                  "DLL target to build (repeatable; entrypoint always included)")
      ->type_name("NAME")
      ->take_all();

  sub->callback([args, &ctx, &pending]() {
    pending = [args, &ctx]() -> Result<void> {
      std::string path = args->config_path;
      if (path.empty()) {
        auto discovered = DiscoverManifestInCwd();
        if (!discovered)
          return Err<void>(discovered.error());
        path = *discovered;
      }
      return CodegenFromConfig(path, ctx, args->targets);
    };
  });
}

}  // namespace rexglue::cli
