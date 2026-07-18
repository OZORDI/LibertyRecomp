/**
 * @file        tests/unit/rexglue/migration_scan_test.cpp
 * @brief       Tests for project-tree migration scanners
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 * @license     BSD 3-Clause License
 */

#include "rexglue/commands/migration_scan.h"

#include <catch2/catch_test_macros.hpp>

#include <array>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_set>

namespace fs = std::filesystem;

namespace {

struct TempProject {
  fs::path root;
  explicit TempProject(const std::string& tag = "migration_scan_test")
      : root(fs::temp_directory_path() / tag) {
    fs::remove_all(root);
    fs::create_directories(root / "generated");
  }
  ~TempProject() { fs::remove_all(root); }

  void writeRexglueCmake(const std::string& content) const {
    std::ofstream f(root / "generated" / "rexglue.cmake");
    f << content;
  }

  void writeFile(const fs::path& rel, const std::string& content) const {
    fs::create_directories((root / rel).parent_path());
    std::ofstream f(root / rel);
    f << content;
  }
};

}  // namespace

// ---------------------------------------------------------------------------
// SDK template drift
// ---------------------------------------------------------------------------

TEST_CASE("MigrationScan: empty plan when rexglue.cmake matches template",
          "[rexglue][migration_scan]") {
  TempProject tp;
  std::string rendered = rexglue::cli::RenderRexglueCmake("mygame", "0.8.0", "generated/default");
  tp.writeRexglueCmake(rendered);

  auto plan = rexglue::cli::ScanSdkTemplateDrift(tp.root, "mygame", "0.8.0", "generated/default");
  CHECK(plan.empty());
}

TEST_CASE("MigrationScan: drift entry is silent (lives inside generated/)",
          "[rexglue][migration_scan]") {
  TempProject tp;
  tp.writeRexglueCmake("# obviously stale content\n");

  auto plan = rexglue::cli::ScanSdkTemplateDrift(tp.root, "mygame", "0.8.0", "generated/default");
  REQUIRE(plan.size() == 1u);
  CHECK(plan[0].path.filename() == "rexglue.cmake");
  CHECK(plan[0].silent);
  CHECK(plan[0].action == rexglue::cli::OverwriteAction::Write);
  CHECK(plan[0].rendered_content.find("rex::runtime") != std::string::npos);
  CHECK(plan[0].rendered_content.find("generated/default/sources.cmake") != std::string::npos);
}

TEST_CASE("MigrationScan: drift entry is silent when the file is missing",
          "[rexglue][migration_scan]") {
  TempProject tp;  // generated/rexglue.cmake never written

  auto plan = rexglue::cli::ScanSdkTemplateDrift(tp.root, "mygame", "0.8.0", "generated/default");
  REQUIRE(plan.size() == 1u);
  CHECK(plan[0].silent);
  CHECK(plan[0].path == tp.root / "generated" / "rexglue.cmake");
}

// ---------------------------------------------------------------------------
// CMake reference rewrites
// ---------------------------------------------------------------------------

TEST_CASE("MigrationScan: ScanCmakeReferences rewrites legacy config refs",
          "[rexglue][migration_scan]") {
  TempProject tp;
  tp.writeFile("CMakeLists.txt",
               "add_custom_target(mygame_codegen\n"
               "    COMMAND rexglue codegen ${CMAKE_CURRENT_SOURCE_DIR}/mygame_config.toml\n"
               ")\n");
  tp.writeFile("cmake/extra.cmake", "# nothing to do here\n");

  auto entries =
      rexglue::cli::ScanCmakeReferences(tp.root, "mygame_config.toml", "mygame_manifest.toml");
  REQUIRE(entries.size() == 1u);
  CHECK(entries[0].path == tp.root / "CMakeLists.txt");
  CHECK_FALSE(entries[0].silent);
  CHECK(entries[0].rendered_content.find("mygame_manifest.toml") != std::string::npos);
  CHECK(entries[0].rendered_content.find("mygame_config.toml") == std::string::npos);
}

TEST_CASE("MigrationScan: ScanCmakeReferences leaves embedded substrings alone",
          "[rexglue][migration_scan]") {
  TempProject tp;
  tp.writeFile("CMakeLists.txt",
               "# legacy file: mygame_config.toml.bak retained for reference\n"
               "add_custom_target(mygame_codegen\n"
               "    COMMAND rexglue codegen mygame_config.toml\n"
               ")\n");
  auto entries =
      rexglue::cli::ScanCmakeReferences(tp.root, "mygame_config.toml", "mygame_manifest.toml");
  REQUIRE(entries.size() == 1u);
  CHECK(entries[0].rendered_content.find("mygame_config.toml.bak") != std::string::npos);
  CHECK(entries[0].rendered_content.find("rexglue codegen mygame_manifest.toml") !=
        std::string::npos);
}

TEST_CASE("MigrationScan: ScanCmakeReferences skips files inside generated/",
          "[rexglue][migration_scan]") {
  TempProject tp;
  tp.writeFile("generated/rexglue.cmake",
               "add_custom_target(mygame_codegen\n"
               "    COMMAND rexglue codegen mygame_config.toml\n"
               ")\n");

  auto entries =
      rexglue::cli::ScanCmakeReferences(tp.root, "mygame_config.toml", "mygame_manifest.toml");
  CHECK(entries.empty());
}

TEST_CASE("MigrationScan: ScanCmakeReferences ignores irrelevant files",
          "[rexglue][migration_scan]") {
  TempProject tp;
  tp.writeFile("README.md", "mygame_config.toml is the legacy config\n");
  tp.writeFile("src/main.cpp", "// references mygame_config.toml in a comment\n");

  auto entries =
      rexglue::cli::ScanCmakeReferences(tp.root, "mygame_config.toml", "mygame_manifest.toml");
  CHECK(entries.empty());
}

// ---------------------------------------------------------------------------
// Source #include rewrites
// ---------------------------------------------------------------------------

TEST_CASE("MigrationScan: ScanSourceIncludeRewrites renames _config.h to _init.h",
          "[rexglue][migration_scan]") {
  TempProject tp;
  tp.writeFile("src/main.cpp",
               "#include \"generated/mygame_config.h\"\n"
               "int main() { return 0; }\n");

  auto entries = rexglue::cli::ScanSourceIncludeRewrites(tp.root, "mygame");
  REQUIRE(entries.size() == 1u);
  CHECK(entries[0].path == tp.root / "src" / "main.cpp");
  CHECK_FALSE(entries[0].silent);
  CHECK(entries[0].action == rexglue::cli::OverwriteAction::Write);
  CHECK(entries[0].rendered_content.find("generated/mygame_init.h") != std::string::npos);
  CHECK(entries[0].rendered_content.find("mygame_config.h") == std::string::npos);
  CHECK(entries[0].rendered_content.find("int main()") != std::string::npos);
}

TEST_CASE("MigrationScan: ScanSourceIncludeRewrites drops duplicate when _init.h already present",
          "[rexglue][migration_scan]") {
  TempProject tp;
  tp.writeFile("src/main.cpp",
               "#include \"generated/mygame_config.h\"\n"
               "#include \"generated/mygame_init.h\"\n"
               "int main() { return 0; }\n");

  auto entries = rexglue::cli::ScanSourceIncludeRewrites(tp.root, "mygame");
  REQUIRE(entries.size() == 1u);
  CHECK(entries[0].rendered_content.find("mygame_config.h") == std::string::npos);
  std::size_t init_count = 0;
  for (std::size_t pos = 0;
       (pos = entries[0].rendered_content.find("mygame_init.h", pos)) != std::string::npos;
       pos += 1) {
    ++init_count;
  }
  CHECK(init_count == 1u);
  CHECK(entries[0].rendered_content.find("int main()") != std::string::npos);
}

TEST_CASE("MigrationScan: ScanSourceIncludeRewrites no-op when no _config.h reference",
          "[rexglue][migration_scan]") {
  TempProject tp;
  tp.writeFile("src/main.cpp",
               "#include \"generated/mygame_init.h\"\n"
               "int main() { return 0; }\n");

  auto entries = rexglue::cli::ScanSourceIncludeRewrites(tp.root, "mygame");
  CHECK(entries.empty());
}

TEST_CASE("MigrationScan: ScanSourceIncludeRewrites skips files inside generated/",
          "[rexglue][migration_scan]") {
  TempProject tp;
  tp.writeFile("generated/mygame_register.cpp", "#include \"mygame_config.h\"\n");

  auto entries = rexglue::cli::ScanSourceIncludeRewrites(tp.root, "mygame");
  CHECK(entries.empty());
}

TEST_CASE("MigrationScan: ScanSourceIncludeRewrites only matches the project's own header",
          "[rexglue][migration_scan]") {
  TempProject tp;
  tp.writeFile("src/main.cpp", "#include \"otherproj_config.h\"\n");

  auto entries = rexglue::cli::ScanSourceIncludeRewrites(tp.root, "mygame");
  CHECK(entries.empty());
}

TEST_CASE("MigrationScan: ScanSourceIncludeRewrites is case-insensitive on basename",
          "[rexglue][migration_scan]") {
  TempProject tp;
  tp.writeFile("src/main.cpp", "#include \"MyGame_Config.H\"\n");

  auto entries = rexglue::cli::ScanSourceIncludeRewrites(tp.root, "mygame");
  REQUIRE(entries.size() == 1u);
  CHECK(entries[0].rendered_content.find("mygame_init.h") != std::string::npos);
}

// ---------------------------------------------------------------------------
// Stale include warnings
// ---------------------------------------------------------------------------

TEST_CASE("MigrationScan: ScanStaleIncludes matches quoted #include with stale basename",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_stale_includes");
  tp.writeFile("main.cpp",
               R"(#include "generated/foo_config.h"
int main() { return 0; }
)");
  std::unordered_set<std::string> removed{"foo_config.h"};

  auto results = rexglue::cli::ScanStaleIncludes(tp.root, removed);
  REQUIRE(results.size() == 1u);
  CHECK(results[0].file == tp.root / "main.cpp");
  CHECK(results[0].line_number == 1u);
  CHECK(results[0].detail.find("generated/foo_config.h") != std::string::npos);
  CHECK(results[0].hint.find("no longer emitted") != std::string::npos);
}

TEST_CASE("MigrationScan: ScanStaleIncludes ignores unrelated #include directives",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_stale_includes_unrelated");
  tp.writeFile("a.cpp",
               R"(#include <vector>
#include "bar.h"
)");
  std::unordered_set<std::string> removed{"foo_config.h"};

  auto results = rexglue::cli::ScanStaleIncludes(tp.root, removed);
  CHECK(results.empty());
}

TEST_CASE("MigrationScan: ScanStaleIncludes walks subdirectories and supports common extensions",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_stale_includes_walk");
  tp.writeFile("a.cpp", "#include \"foo_config.h\"\n");
  tp.writeFile("sub/b.h", "#include \"foo_config.h\"\n");
  tp.writeFile("sub/c.hpp", "#include <foo_config.h>\n");
  tp.writeFile("sub/d.inl", "#include \"foo_config.h\"\n");
  tp.writeFile("sub/e.txt", "#include \"foo_config.h\"\n");
  std::unordered_set<std::string> removed{"foo_config.h"};

  auto results = rexglue::cli::ScanStaleIncludes(tp.root, removed);
  CHECK(results.size() == 4u);
}

TEST_CASE("MigrationScan: ScanStaleIncludes returns empty when src dir does not exist",
          "[rexglue][migration_scan]") {
  fs::path nonexistent = fs::temp_directory_path() / "migration_stale_includes_missing";
  fs::remove_all(nonexistent);
  std::unordered_set<std::string> removed{"foo.h"};
  auto results = rexglue::cli::ScanStaleIncludes(nonexistent, removed);
  CHECK(results.empty());
}

TEST_CASE("MigrationScan: ScanStaleIncludes matches case-insensitively on basename",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_stale_includes_case");
  tp.writeFile("a.cpp", "#include \"Foo_Config.H\"\n");
  std::unordered_set<std::string> removed{"foo_config.h"};

  auto results = rexglue::cli::ScanStaleIncludes(tp.root, removed);
  REQUIRE(results.size() == 1u);
  CHECK(results[0].detail.find("Foo_Config.H") != std::string::npos);
}

// ---------------------------------------------------------------------------
// Legacy identifier scanner
// ---------------------------------------------------------------------------

TEST_CASE("MigrationScan: ScanLegacyIdentifiers rewrites whole-token PPC_FUNC to REX_FUNC",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_legacy_idents");
  tp.writeFile("src/foo.cpp",
               "#include <rex/ppc/context.h>\n"
               "PPC_FUNC(sub_1234) {\n"
               "  PPC_LOAD_U32(ctx.r3.u32);\n"
               "}\n");

  auto findings = rexglue::cli::ScanLegacyIdentifiers(tp.root);
  REQUIRE(findings.rewrites.size() == 1u);
  CHECK(findings.warnings.empty());
  const auto& entry = findings.rewrites[0];
  CHECK(entry.path == tp.root / "src" / "foo.cpp");
  CHECK_FALSE(entry.silent);
  CHECK(entry.rendered_content.find("REX_FUNC(sub_1234)") != std::string::npos);
  CHECK(entry.rendered_content.find("REX_LOAD_U32") != std::string::npos);
  CHECK(entry.rendered_content.find("PPC_FUNC") == std::string::npos);
  CHECK(entry.rendered_content.find("PPC_LOAD_U32") == std::string::npos);
}

TEST_CASE("MigrationScan: ScanLegacyIdentifiers leaves non-matching prefixes alone",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_legacy_idents_prefix");
  // Real binutils tokens that happen to start with PPC_ but are NOT in the
  // breaking-change rule list - they must not be rewritten.
  tp.writeFile("src/disasm.cpp",
               "#include <ppc-dis.h>\n"
               "uint32_t op = PPC_OP(insn);\n"
               "if (op == PPC_INST_BL) { /* ... */ }\n");

  auto findings = rexglue::cli::ScanLegacyIdentifiers(tp.root);
  CHECK(findings.rewrites.empty());
  CHECK(findings.warnings.empty());
}

TEST_CASE("MigrationScan: ScanLegacyIdentifiers warns on tokens with no replacement",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_legacy_idents_warn");
  tp.writeFile("src/decls.h", "PPC_EXTERN_FUNC(some_helper);\n");

  auto findings = rexglue::cli::ScanLegacyIdentifiers(tp.root);
  CHECK(findings.rewrites.empty());
  REQUIRE(findings.warnings.size() == 1u);
  CHECK(findings.warnings[0].file == tp.root / "src" / "decls.h");
  CHECK(findings.warnings[0].line_number == 1u);
  CHECK(findings.warnings[0].detail.find("PPC_EXTERN_FUNC") != std::string::npos);
  CHECK(findings.warnings[0].hint.find("extern REX_FUNC") != std::string::npos);
}

TEST_CASE("MigrationScan: ScanLegacyIdentifiers respects identifier boundaries",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_legacy_idents_boundary");
  // PPC_FUNC_PROLOGUE is a known token; PPC_FUNC_THINGAMAJIG is not. The token
  // matcher must prefer the longest run of identifier characters and only
  // rewrite when the full identifier is in the rule table.
  tp.writeFile("src/bar.cpp",
               "PPC_FUNC_PROLOGUE();\n"
               "PPC_FUNC_THINGAMAJIG();\n");

  auto findings = rexglue::cli::ScanLegacyIdentifiers(tp.root);
  REQUIRE(findings.rewrites.size() == 1u);
  CHECK(findings.rewrites[0].rendered_content.find("REX_FUNC_PROLOGUE();") != std::string::npos);
  CHECK(findings.rewrites[0].rendered_content.find("PPC_FUNC_THINGAMAJIG();") != std::string::npos);
}

TEST_CASE("MigrationScan: ScanLegacyIdentifiers skips files inside generated/",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_legacy_idents_gen");
  tp.writeFile("generated/foo.cpp", "PPC_FUNC(sub_1) {}\n");

  auto findings = rexglue::cli::ScanLegacyIdentifiers(tp.root);
  CHECK(findings.rewrites.empty());
  CHECK(findings.warnings.empty());
}

TEST_CASE("MigrationScan: ScanLegacyIdentifiers accepts a custom rule list",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_legacy_idents_custom");
  tp.writeFile("src/custom.cpp", "OLD_THING x = NEW_THING;\nUSE_ONCE();\n");

  std::array<rexglue::cli::BreakingChangeRule, 2> rules = {{
      {"OLD_THING", "NEW_THING_V2", "renamed: OLD_THING -> NEW_THING_V2"},
      {"USE_ONCE", "", "removed; manual fix required"},
  }};
  auto findings = rexglue::cli::ScanLegacyIdentifiers(tp.root, rules);
  REQUIRE(findings.rewrites.size() == 1u);
  CHECK(findings.rewrites[0].rendered_content.find("NEW_THING_V2 x = NEW_THING;") !=
        std::string::npos);
  REQUIRE(findings.warnings.size() == 1u);
  CHECK(findings.warnings[0].detail.find("USE_ONCE") != std::string::npos);
}

// ---------------------------------------------------------------------------
// Call-site pattern scanner
// ---------------------------------------------------------------------------

TEST_CASE("MigrationScan: ScanCallSitePatterns flags removed game_directory positional",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_callsite_gamedir");
  tp.writeFile("src/app.cpp",
               "#include <rex/runtime.h>\n"
               "void f() {\n"
               "  if (GetArgument(\"game_directory\").has_value()) {}\n"
               "}\n");

  auto warnings = rexglue::cli::ScanCallSitePatterns(tp.root);
  REQUIRE(warnings.size() == 1u);
  CHECK(warnings[0].file == tp.root / "src" / "app.cpp");
  CHECK(warnings[0].line_number == 3u);
  CHECK(warnings[0].detail.find("game_directory") != std::string::npos);
}

TEST_CASE("MigrationScan: ScanCallSitePatterns leaves unrelated GetArgument calls alone",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_callsite_gamedir_other");
  tp.writeFile("src/app.cpp", "  GetArgument(\"verbose\");\n");

  auto warnings = rexglue::cli::ScanCallSitePatterns(tp.root);
  CHECK(warnings.empty());
}

TEST_CASE("MigrationScan: ScanCallSitePatterns flags one-arg AllocateThunk",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_callsite_thunk_one");
  tp.writeFile("src/d.cpp",
               "auto* d = dispatcher();\n"
               "uint32_t a = d->AllocateThunk(&Helper);\n");

  auto warnings = rexglue::cli::ScanCallSitePatterns(tp.root);
  REQUIRE(warnings.size() == 1u);
  CHECK(warnings[0].line_number == 2u);
  CHECK(warnings[0].detail.find("AllocateThunk") != std::string::npos);
  CHECK(warnings[0].hint.find("caller_address") != std::string::npos);
}

TEST_CASE("MigrationScan: ScanCallSitePatterns ignores two-arg AllocateThunk",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_callsite_thunk_two");
  tp.writeFile("src/d.cpp", "uint32_t a = d->AllocateThunk(s_trampolines[i], ctx.lr);\n");

  auto warnings = rexglue::cli::ScanCallSitePatterns(tp.root);
  CHECK(warnings.empty());
}

TEST_CASE("MigrationScan: ScanCallSitePatterns handles single arg with nested parens",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_callsite_thunk_nested");
  tp.writeFile("src/d.cpp", "auto a = d->AllocateThunk(MakeHelper(env));\n");

  auto warnings = rexglue::cli::ScanCallSitePatterns(tp.root);
  REQUIRE(warnings.size() == 1u);
  CHECK(warnings[0].detail.find("AllocateThunk") != std::string::npos);
}

TEST_CASE("MigrationScan: ScanCallSitePatterns ignores zero-arg AllocateThunk",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_callsite_thunk_zero");
  tp.writeFile("src/d.cpp", "auto a = d->AllocateThunk();\n");

  auto warnings = rexglue::cli::ScanCallSitePatterns(tp.root);
  CHECK(warnings.empty());
}

TEST_CASE("MigrationScan: ScanCallSitePatterns skips files inside generated/",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_callsite_gen");
  tp.writeFile("generated/foo.cpp", "  d->AllocateThunk(&Fn);\n");

  auto warnings = rexglue::cli::ScanCallSitePatterns(tp.root);
  CHECK(warnings.empty());
}

TEST_CASE("MigrationScan: ScanCallSitePatterns accepts a custom rule list",
          "[rexglue][migration_scan]") {
  TempProject tp("migration_callsite_custom");
  tp.writeFile("src/x.cpp", "  legacy_api(42);\n");

  std::array<rexglue::cli::CallSiteRule, 1> rules = {{
      {"legacy_api(", "uses removed legacy_api()", "switch to modern_api() (drop-in)", nullptr},
  }};
  auto warnings = rexglue::cli::ScanCallSitePatterns(tp.root, rules);
  REQUIRE(warnings.size() == 1u);
  CHECK(warnings[0].detail.find("legacy_api") != std::string::npos);
  CHECK(warnings[0].hint.find("modern_api") != std::string::npos);
}

TEST_CASE("MigrationScan: DefaultCallSiteRules covers known manual fixes",
          "[rexglue][migration_scan]") {
  auto rules = rexglue::cli::DefaultCallSiteRules();
  REQUIRE(!rules.empty());
  bool saw_gamedir = false;
  bool saw_thunk = false;
  for (const auto& r : rules) {
    if (r.pattern.find("game_directory") != std::string_view::npos)
      saw_gamedir = true;
    if (r.pattern == "AllocateThunk(")
      saw_thunk = true;
  }
  CHECK(saw_gamedir);
  CHECK(saw_thunk);
}

TEST_CASE("MigrationScan: DefaultBreakingChangeRules covers PPC_ legacy macros",
          "[rexglue][migration_scan]") {
  auto rules = rexglue::cli::DefaultBreakingChangeRules();
  REQUIRE(!rules.empty());
  bool saw_ppc_func = false;
  bool saw_ppc_round_nearest = false;
  for (const auto& r : rules) {
    if (r.legacy_token == "PPC_FUNC")
      saw_ppc_func = (r.replacement == "REX_FUNC");
    if (r.legacy_token == "PPC_ROUND_NEAREST")
      saw_ppc_round_nearest = (r.replacement == "rex::ppc::kRoundNearest");
  }
  CHECK(saw_ppc_func);
  CHECK(saw_ppc_round_nearest);
}
