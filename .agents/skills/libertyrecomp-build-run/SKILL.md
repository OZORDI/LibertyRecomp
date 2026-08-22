---
name: libertyrecomp-build-run
description: Build, package, run, and capture execution logs for LibertyRecomp, especially macOS CMake/Ninja builds. Use when configuring a build preset, compiling LibertyRecomp or LibertyRecompLib, locating the app bundle, running the current build, collecting a bounded execution log, diagnosing build failures or timeouts, or deciding whether GTA IV code generation is required.
---

# LibertyRecomp Build And Run

Use this skill for LibertyRecomp build, packaging, launch, and execution-log work. Treat the repository's current documentation and CMake files as authoritative; do not rely on remembered paths from older branches.

## Establish The Current Pipeline

1. Work from the repository root returned by `git rev-parse --show-toplevel`.
2. Read `docs/BUILDING.md` before changing build commands.
3. Inspect `CMakePresets.json` when selecting or diagnosing a preset.
4. Inspect `git status --short` and preserve all unrelated worktree changes.
5. Check for an active build before configuring or compiling:

```bash
pgrep -af 'cmake --build .*macos-|ninja.*LibertyRecomp|clang\+\+.*gta4_(recomp|register)'
```

Never start a second build because a terminal call timed out. A timeout only means the terminal stopped waiting. Poll the original process and log instead. Concurrent Ninja invocations against one build directory can damage `.ninja_log` and trigger an unnecessary broad rebuild.

## macOS Prerequisites

The documented prerequisites are the latest Xcode and these Homebrew packages:

```bash
brew install cmake ninja pkg-config
```

The repository vendors vcpkg under `thirdparty/vcpkg`. The presets also define `VCPKG_ROOT`, but exporting it explicitly is valid:

```bash
export VCPKG_ROOT="$(pwd)/thirdparty/vcpkg"
```

## Select A Preset

Use one of the documented macOS presets:

- `macos-debug`: debugging and assertions.
- `macos-relwithdebinfo`: optimized build with useful symbols.
- `macos-release`: normal release build.

For Apple Silicon, configure with:

```bash
cmake . --preset macos-release
```

For Intel macOS, add the documented architecture override:

```bash
cmake . --preset macos-release -DCMAKE_OSX_ARCHITECTURES=x86_64
```

The preset build directory is `out/build/<preset>`. Configure only when that tree is missing, the preset changed, or relevant CMake inputs changed. Do not reconfigure for an ordinary source edit.

## Build One Process At A Time

The documented targets are:

```bash
cmake --build ./out/build/macos-release --target LibertyRecompLib
cmake --build ./out/build/macos-release --target LibertyRecomp
```

Build `LibertyRecomp` for normal app iteration. Build `LibertyRecompLib` separately when the requested change or `docs/BUILDING.md` requires it. Do not assume both targets are always necessary.

Before building, run:

```bash
git diff --check
```

Prefer the supplied durable build helper so a long compile survives terminal wait limits:

```bash
.Codex/skills/libertyrecomp-build-run/scripts/macos_build.sh start macos-release LibertyRecomp
.Codex/skills/libertyrecomp-build-run/scripts/macos_build.sh status macos-release LibertyRecomp
.Codex/skills/libertyrecomp-build-run/scripts/macos_build.sh wait macos-release LibertyRecomp
```

The build can spend several minutes compiling generated GTA IV translation units, especially `gta4_register.cpp`. CPU activity and a live compiler child indicate progress. Do not kill or restart that work merely because output is quiet.

Judge success from the build process exit code and final link output. Warnings about FPCR operand width, duplicate platform macros, or duplicate `-lpthread` are not build failures if the target links successfully.

## Keep Code Generation Separate

Normal host runtime, VFS, UI, graphics backend, or application edits do not require regenerating GTA IV code.

Only run code generation when the XEX, `gta4_config.toml`, code-generation inputs, or generated outputs actually changed. Inspect `gta4-recomp/CMakeLists.txt` and the current RexGlue code-generation documentation before invoking it. Never manually edit files under `gta4-recomp/generated`.

## Locate And Run The App

For `macos-release`, the app bundle is:

```text
out/build/macos-release/LibertyRecomp/Liberty Recompiled.app
```

For interactive use:

```bash
open "./out/build/macos-release/LibertyRecomp/Liberty Recompiled.app"
```

Do not use `open` when collecting logs because it detaches. Execute the bundle binary directly:

```bash
"./out/build/macos-release/LibertyRecomp/Liberty Recompiled.app/Contents/MacOS/Liberty Recompiled"
```

Use the supplied helper for a bounded run and preserved combined log:

```bash
.Codex/skills/libertyrecomp-build-run/scripts/macos_run_log.sh macos-release 30 /tmp/liberty_run.log
```

The helper uses `timeout` or Homebrew `gtimeout`. Exit code `124` means the run reached the requested time limit; it is not a crash. Preserve the complete log before filtering it.

## Classify The Result

Inspect concise milestones and faults with:

```bash
rg -n 'CRASH DETECTED|ACCESS_VIOLATION|EXC_BAD_ACCESS|stop reason|FrameFlow|PresenterFlow' /tmp/liberty_run.log
```

Classify failures by phase:

- Configure failure: preset, toolchain, SDK, or dependency setup.
- Compile failure: report the first relevant compiler diagnostic and source location.
- Link/package failure: report the missing symbol, library, or bundle step.
- Startup environment failure: a failure to reserve the guest address space occurs before game code. Retry once from an unrestricted terminal before attributing it to a code change.
- Guest/runtime crash: preserve the full RexGlue crash block, PPC context, guest LR, and preceding game events.
- Host crash: obtain a native debugger backtrace when the RexGlue report has no useful guest context.
- Bounded success: timeout after visible game/runtime progress is a completed observation, not a failed test.

Do not assume the first visible crash is the root cause. Correlate it with the earlier initialization, VFS, GPU, and game-thread events in the same log.

## Report Back

Always report:

- Preset and target used.
- Whether configuration was run and why.
- Build exit status and app bundle path.
- Run duration and process exit status.
- Log path.
- The last confirmed runtime milestone and exact first fault, if any.
- Anything not tested.

Never use destructive Git cleanup commands to make a build pass, and never discard unrelated changes in the shared worktree.
