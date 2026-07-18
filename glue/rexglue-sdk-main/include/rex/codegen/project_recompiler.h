/**
 * @file        rex/codegen/project_recompiler.h
 * @brief       Project-level recompiler driving manifest-based multi-binary codegen
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 */

#pragma once

#include <string>
#include <vector>

#include <rex/codegen/manifest.h>
#include <rex/codegen/progress_reporter.h>
#include <rex/result.h>

namespace rex::codegen {

struct ProjectRecompilerOptions {
  std::vector<std::string> targets;  // empty = all
  bool force = false;
  bool enableExceptionHandlers = false;
  ProgressReporter* reporter = nullptr;
};

class ProjectRecompiler {
 public:
  explicit ProjectRecompiler(ManifestConfig manifest);
  Result<void> Run(const ProjectRecompilerOptions& opts);

  /**
   * Aggregated basenames of files removed across all modules during the most
   * recent Run() call. Empty until Run() completes successfully.
   */
  const std::vector<std::string>& deletedFiles() const { return deletedFiles_; }

  /**
   * Aggregated basenames of files written across all modules during the most
   * recent Run() call. Empty until Run() completes successfully.
   */
  const std::vector<std::string>& writtenFiles() const { return writtenFiles_; }

 private:
  ManifestConfig manifest_;
  std::vector<std::string> deletedFiles_;
  std::vector<std::string> writtenFiles_;
};

}  // namespace rex::codegen
