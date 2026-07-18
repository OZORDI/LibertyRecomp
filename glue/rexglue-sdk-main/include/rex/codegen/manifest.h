/**
 * @file        rex/codegen/manifest.h
 * @brief       Manifest TOML parser for multi-binary projects
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 */

#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <rex/codegen/config.h>

namespace rex::codegen {

/**
 * Codegen settings for a single binary inside a manifest. The entrypoint
 * uses an empty `guestPath`; module entries set it to the canonicalized
 * guest-visible path the host runtime resolves against.
 */
struct BinaryConfig {
  RecompilerConfig recompiler;
  std::string guestPath;
};

/**
 * Canonicalize a module guest path: device-stripped, slashes/case normalized,
 * with `<project>/assets/` stripped when a matching project name is given.
 */
std::string CanonicalizeModuleGuestPath(std::string_view path, std::string_view project_name = {});

/**
 * Parsed manifest TOML. Construct via Load(); treat as read-only after.
 */
struct ManifestConfig {
  std::string projectName;
  std::optional<std::string> sdkVersion;  ///< Last SDK that ran codegen on this project
  std::optional<std::string> gameRoot;    ///< Game asset root, relative to manifestDir.
                                          ///< Set by `rexglue init` to anchor DLL guest paths.
  std::filesystem::path manifestDir;      ///< Directory containing the manifest
  BinaryConfig entrypoint;                ///< Entrypoint codegen settings (inline)
  std::vector<BinaryConfig> modules;      ///< DLL module codegen settings (inline)

  /**
   * Load a manifest TOML file. Returns nullopt on parse failure.
   */
  static std::optional<ManifestConfig> Load(const std::filesystem::path& path);

  /**
   * True when `path` parses as a manifest (i.e. has a `[project]` section).
   */
  static bool IsManifest(const std::filesystem::path& path);

  /**
   * Insert or overwrite [project].sdk_version in the manifest file at `path`.
   * Preserves the rest of the file's content. Returns false on parse or write
   * failure.
   */
  static bool WriteSdkVersionStamp(const std::filesystem::path& path, std::string_view version);
};

}  // namespace rex::codegen
