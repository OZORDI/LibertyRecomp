/**
 * @file        tests/unit/codegen/manifest_test.cpp
 * @brief       Unit tests for manifest TOML parser
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 */

#include <catch2/catch_test_macros.hpp>
#include <rex/codegen/manifest.h>

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace {

struct TempDir {
  fs::path path;
  TempDir() : path(fs::temp_directory_path() / "manifest_test") {
    fs::remove_all(path);
    fs::create_directories(path);
  }
  ~TempDir() { fs::remove_all(path); }
  void writeFile(const std::string& name, const std::string& content) const {
    std::ofstream f(path / name);
    f << content;
  }
};

}  // namespace

TEST_CASE("Manifest: parse manifest with inline entrypoint", "[codegen][manifest]") {
  TempDir tmp;

  tmp.writeFile("manifest.toml", R"(
[project]
name = "mygame"

[entrypoint]
file_path = "assets/default.xex"
out_directory_path = "generated/default"
includes = []
  )");

  auto result = rex::codegen::ManifestConfig::Load(tmp.path / "manifest.toml");
  REQUIRE(result.has_value());
  CHECK(result->projectName == "mygame");
  CHECK(result->entrypoint.recompiler.filePath == "assets/default.xex");
  CHECK(result->entrypoint.recompiler.outDirectoryPath == "generated/default");
  CHECK(result->entrypoint.guestPath.empty());
  CHECK(result->modules.empty());
}

TEST_CASE("Manifest: IsManifest detection", "[codegen][manifest]") {
  TempDir tmp;
  tmp.writeFile("manifest.toml",
                "[project]\nname = \"test\"\n[entrypoint]\nfile_path = \"x.xex\"\n");
  tmp.writeFile("config.toml", "project_name = \"test\"\nfile_path = \"test.xex\"");

  CHECK(rex::codegen::ManifestConfig::IsManifest(tmp.path / "manifest.toml"));
  CHECK_FALSE(rex::codegen::ManifestConfig::IsManifest(tmp.path / "config.toml"));
}

TEST_CASE("Manifest: parse manifest with inline modules", "[codegen][manifest]") {
  TempDir tmp;
  tmp.writeFile("manifest.toml", R"(
[project]
name = "mygame"

[entrypoint]
file_path = "assets/default.xex"
out_directory_path = "generated/default"

[[modules]]
guest_path = "bin/lib_a.dll"
file_path = "assets/lib_a.dll"
out_directory_path = "generated/lib_a"

[[modules]]
guest_path = "bin/lib_b.dll"
file_path = "assets/lib_b.dll"
out_directory_path = "generated/lib_b"
  )");

  auto result = rex::codegen::ManifestConfig::Load(tmp.path / "manifest.toml");
  REQUIRE(result.has_value());
  REQUIRE(result->modules.size() == 2u);
  CHECK(result->modules[0].guestPath == "bin/lib_a.dll");
  CHECK(result->modules[0].recompiler.filePath == "assets/lib_a.dll");
  CHECK(result->modules[1].guestPath == "bin/lib_b.dll");
  CHECK(result->modules[1].recompiler.filePath == "assets/lib_b.dll");
}

TEST_CASE("Manifest: missing project section fails", "[codegen][manifest]") {
  TempDir tmp;
  tmp.writeFile("manifest.toml", "[entrypoint]\nfile_path = \"x.xex\"\n");
  auto result = rex::codegen::ManifestConfig::Load(tmp.path / "manifest.toml");
  CHECK_FALSE(result.has_value());
}

TEST_CASE("Manifest: missing entrypoint section fails", "[codegen][manifest]") {
  TempDir tmp;
  tmp.writeFile("manifest.toml", "[project]\nname = \"mygame\"\n");
  auto result = rex::codegen::ManifestConfig::Load(tmp.path / "manifest.toml");
  CHECK_FALSE(result.has_value());
}

TEST_CASE("Manifest: missing project name fails", "[codegen][manifest]") {
  TempDir tmp;
  tmp.writeFile("manifest.toml",
                "[project]\n[entrypoint]\nfile_path = \"x.xex\"\nout_directory_path = \"o\"\n");
  auto result = rex::codegen::ManifestConfig::Load(tmp.path / "manifest.toml");
  CHECK_FALSE(result.has_value());
}

TEST_CASE("Manifest: parses sdk_version when present", "[codegen][manifest]") {
  TempDir tmp;
  tmp.writeFile("manifest.toml", R"(
[project]
name = "mygame"
sdk_version = "0.7.8.48"

[entrypoint]
file_path = "assets/default.xex"
out_directory_path = "generated/default"
  )");
  auto result = rex::codegen::ManifestConfig::Load(tmp.path / "manifest.toml");
  REQUIRE(result.has_value());
  REQUIRE(result->sdkVersion.has_value());
  CHECK(*result->sdkVersion == "0.7.8.48");
}

TEST_CASE("Manifest: sdk_version is nullopt when absent", "[codegen][manifest]") {
  TempDir tmp;
  tmp.writeFile("manifest.toml", R"(
[project]
name = "mygame"

[entrypoint]
file_path = "assets/default.xex"
out_directory_path = "generated/default"
  )");
  auto result = rex::codegen::ManifestConfig::Load(tmp.path / "manifest.toml");
  REQUIRE(result.has_value());
  CHECK_FALSE(result->sdkVersion.has_value());
}

TEST_CASE("Manifest: WriteSdkVersionStamp inserts when missing", "[codegen][manifest]") {
  TempDir tmp;
  tmp.writeFile("manifest.toml", R"([project]
name = "mygame"

[entrypoint]
file_path = "assets/default.xex"
out_directory_path = "generated/default"
)");

  auto path = tmp.path / "manifest.toml";
  CHECK(rex::codegen::ManifestConfig::WriteSdkVersionStamp(path, "0.8.0"));

  auto result = rex::codegen::ManifestConfig::Load(path);
  REQUIRE(result.has_value());
  REQUIRE(result->sdkVersion.has_value());
  CHECK(*result->sdkVersion == "0.8.0");
  CHECK(result->projectName == "mygame");
}

TEST_CASE("Manifest: WriteSdkVersionStamp overwrites existing", "[codegen][manifest]") {
  TempDir tmp;
  tmp.writeFile("manifest.toml", R"([project]
name = "mygame"
sdk_version = "0.7.0"

[entrypoint]
file_path = "assets/default.xex"
out_directory_path = "generated/default"
)");

  auto path = tmp.path / "manifest.toml";
  CHECK(rex::codegen::ManifestConfig::WriteSdkVersionStamp(path, "0.8.0"));

  auto result = rex::codegen::ManifestConfig::Load(path);
  REQUIRE(result.has_value());
  REQUIRE(result->sdkVersion.has_value());
  CHECK(*result->sdkVersion == "0.8.0");
}

TEST_CASE("CanonicalizeModuleGuestPath: device prefix", "[codegen][manifest][canonicalize]") {
  using rex::codegen::CanonicalizeModuleGuestPath;
  CHECK(CanonicalizeModuleGuestPath("game:\\bin\\foo.dll") == "bin/foo.dll");
  CHECK(CanonicalizeModuleGuestPath("game:/bin/foo.dll") == "bin/foo.dll");
  CHECK(CanonicalizeModuleGuestPath("d:\\bin\\foo.dll") == "bin/foo.dll");
}

TEST_CASE("CanonicalizeModuleGuestPath: case + slashes", "[codegen][canonicalize]") {
  using rex::codegen::CanonicalizeModuleGuestPath;
  CHECK(CanonicalizeModuleGuestPath("BIN\\Foo.DLL") == "bin/foo.dll");
  CHECK(CanonicalizeModuleGuestPath("bin\\sub\\Foo.dll") == "bin/sub/foo.dll");
}

TEST_CASE("CanonicalizeModuleGuestPath: leading slashes", "[codegen][canonicalize]") {
  using rex::codegen::CanonicalizeModuleGuestPath;
  CHECK(CanonicalizeModuleGuestPath("/bin/foo.dll") == "bin/foo.dll");
  CHECK(CanonicalizeModuleGuestPath("//bin/foo.dll") == "bin/foo.dll");
}

TEST_CASE("CanonicalizeModuleGuestPath: bare assets prefix preserved without project",
          "[codegen][canonicalize]") {
  using rex::codegen::CanonicalizeModuleGuestPath;
  CHECK(CanonicalizeModuleGuestPath("assets/bin/foo.dll") == "assets/bin/foo.dll");
  CHECK(CanonicalizeModuleGuestPath("Assets/Bin/Foo.DLL") == "assets/bin/foo.dll");
}

TEST_CASE("CanonicalizeModuleGuestPath: project assets prefix stripped",
          "[codegen][canonicalize]") {
  using rex::codegen::CanonicalizeModuleGuestPath;
  CHECK(CanonicalizeModuleGuestPath("mygame/assets/bin/foo.dll", "mygame") == "bin/foo.dll");
  CHECK(CanonicalizeModuleGuestPath("MyGame/Assets/Bin/Foo.DLL", "mygame") == "bin/foo.dll");
}

TEST_CASE("CanonicalizeModuleGuestPath: project mismatch keeps prefix", "[codegen][canonicalize]") {
  using rex::codegen::CanonicalizeModuleGuestPath;
  CHECK(CanonicalizeModuleGuestPath("othergame/assets/bin/foo.dll", "mygame") ==
        "othergame/assets/bin/foo.dll");
  CHECK(CanonicalizeModuleGuestPath("assets/foo.dll", "mygame") == "assets/foo.dll");
}
