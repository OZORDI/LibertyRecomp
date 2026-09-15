#!/usr/bin/env python3
"""Compile the actual replay scope and run CPU-only state-restoration regressions.

The production scope, dirty-word consumer, replay-entry mask block, and renderer
constant layout are extracted verbatim. Only guest address translation is a
fixture adapter. No game, GPU, save data, or production build is launched.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import shutil
import subprocess
import tempfile


def definition(source: str, signature: str) -> str:
    start = source.index(signature)
    end = source.index("\n}", start) + 2
    if source[end:end + 1] == ";":
        end += 1
    return source[start:end]


TESTS = r"""
namespace Test {
using n::NativeDirtyWords;
constexpr uint32_t kDevice = 0x10000, kList = 0x40000;
constexpr size_t kVS = 0x780, kPS = 0x1780, kVSBytes = 0x1000, kPSBytes = 0xE00;
size_t assertions = 0, failures = 0, cases = 0;
void Check(bool value, const char* reason) {
  ++assertions;
  if (!value) throw std::runtime_error(reason);
}
uint32_t Read32(const uint8_t* bytes) {
  uint32_t value; std::memcpy(&value, bytes, sizeof(value));
  return __builtin_bswap32(value);
}
void Write32(std::vector<uint8_t>& bytes, size_t offset, uint32_t value) {
  value = __builtin_bswap32(value); std::memcpy(bytes.data() + offset, &value, sizeof(value));
}
NativeDirtyWords ReadWords(std::vector<uint8_t>& bytes, uint32_t device = kDevice) {
  NativeDirtyWords result{};
  for (size_t word = 0; word < result.size(); ++word)
    result[word] = Hook::LoadU64(bytes.data(), device + uint32_t(word * 8));
  return result;
}
void WriteWords(std::vector<uint8_t>& bytes, const NativeDirtyWords& words,
                uint32_t address = kDevice) {
  for (size_t word = 0; word < words.size(); ++word)
    Hook::StoreU64(bytes.data(), address + uint32_t(word * 8), words[word]);
}
Hook::CapturedDrawSnapshot Capture(std::vector<uint8_t>& bytes, uint32_t device = kDevice) {
  Hook::CapturedDrawSnapshot snapshot;
  Hook::CaptureDrawState(bytes.data(), device, snapshot);
  return snapshot;
}
struct Renderer {
  n::AuthoritativeConstantState vs{kVSBytes}, ps{kPSBytes};
  n::AuthoritativeScalarState<std::array<uint32_t, 2>> booleans;
  void Apply(const std::vector<uint8_t>& bytes, const NativeDirtyWords& dirty, bool full = false) {
    n::DirtyStateDelta decoded; n::DirtyDeltaScratch scratch;
    Check(n::BuildDirtyStateDelta(dirty, Layout::NativeConstantDirtyLayout(), decoded, scratch).valid(),
          "production dirty layout rejected fixture");
    const std::span<const uint8_t> vertex(bytes.data() + kDevice + kVS, kVSBytes);
    const std::span<const uint8_t> pixel(bytes.data() + kDevice + kPS, kPSBytes);
    n::ConstantPayloadDelta vd, pd;
    Check(full ? n::CaptureCompleteConstantSnapshot(vertex, vd)
               : n::CaptureConstantPayloadDelta(vertex, decoded.vertex_constant_ranges, 64, vd),
          "vertex delta capture failed");
    Check(full ? n::CaptureCompleteConstantSnapshot(pixel, pd)
               : n::CaptureConstantPayloadDelta(pixel, decoded.pixel_constant_ranges, 64, pd),
          "pixel delta capture failed");
    const auto hash = [](std::span<const uint8_t> block) {
      uint64_t value = 1469598103934665603ull;
      for (uint8_t byte : block) value = (value ^ byte) * 1099511628211ull;
      return value;
    };
    Check(bool(vs.Apply(vd, hash)), "vertex apply failed");
    Check(bool(ps.Apply(pd, hash)), "pixel apply failed");
    if (full || decoded.boolean_constant_mask.Any()) {
      const auto result = booleans.Apply({Read32(bytes.data() + kDevice + 0x2780),
                                        Read32(bytes.data() + kDevice + 0x2790)});
      Check(result.status == n::ConstantApplyStatus::kApplied, "boolean apply failed");
    }
  }
  void Ordinary(std::vector<uint8_t>& bytes) {
    Apply(bytes, Hook::ConsumeNativeDrawDirtyState(bytes.data(), kDevice).words);
  }
  void Match(const std::vector<uint8_t>& bytes) {
    Check(std::memcmp(vs.canonical().data(), bytes.data() + kDevice + kVS, kVSBytes) == 0,
          "ordinary draw retained replay vertex constants");
    Check(std::memcmp(ps.canonical().data(), bytes.data() + kDevice + kPS, kPSBytes) == 0,
          "ordinary draw retained replay pixel constants");
    Check(booleans.value() == std::array<uint32_t, 2>{Read32(bytes.data() + kDevice + 0x2780),
                                                   Read32(bytes.data() + kDevice + 0x2790)},
          "ordinary draw retained replay booleans");
  }
};
NativeDirtyWords All() { NativeDirtyWords result{}; result.fill(UINT64_MAX); return result; }
void ExhaustiveGroups() {
  for (size_t bank = 0; bank < 2; ++bank) {
    const size_t count = bank ? 56 : 64, start = bank ? kPS : kVS;
    for (size_t group = 0; group < count; ++group) {
      for (size_t byte : {size_t{0}, size_t{63}}) {
        std::vector<uint8_t> bytes(0x80000);
        const auto snapshot = Capture(bytes);
        bytes[kDevice + start + group * 64 + byte] = 0xA7;
        NativeDirtyWords pending{}; pending[2] = 0x20; pending[3] = 2;
        WriteWords(bytes, pending);
        const auto before = bytes;
        Renderer renderer; renderer.Apply(bytes, All(), true);
        std::shared_ptr<const n::ConstantStateVersion> queued;
        {
          Hook::ScopedReplayDrawState replay(bytes.data(), kDevice, snapshot);
          renderer.Apply(bytes, All());
          queued = renderer.vs.SnapshotCurrentVersion();
        }
        auto expected = pending; expected[bank] |= (uint64_t{1} << 63) >> group;
        Check(ReadWords(bytes) == expected, "restored constant group was not marked with exact guest bit");
        Check(std::equal(bytes.begin() + kDevice + 40, bytes.end(), before.begin() + kDevice + 40),
              "scope changed bytes outside its dirty-word publication");
        const auto retained = *n::AuthoritativeConstantState::MaterializeView(queued);
        renderer.Ordinary(bytes); renderer.Match(bytes);
        Check(ReadWords(bytes) == NativeDirtyWords{}, "ordinary consumer did not clear pending words");
        Check(*n::AuthoritativeConstantState::MaterializeView(queued) == retained,
              "restoration mutated an already queued immutable version");
      }
    }
  }
}
void BooleanBanks() {
  for (size_t offset : {size_t{0x2780}, size_t{0x2790}}) {
    std::vector<uint8_t> bytes(0x80000);
    const auto snapshot = Capture(bytes);
    Write32(bytes, kDevice + offset, 0x80000001);
    Renderer renderer; renderer.Apply(bytes, All(), true);
    { Hook::ScopedReplayDrawState replay(bytes.data(), kDevice, snapshot); renderer.Apply(bytes, All()); }
    NativeDirtyWords expected{}; expected[4] = uint64_t{1} << 56;
    Check(ReadWords(bytes) == expected, "boolean restoration did not mark the shared boolean bit");
    renderer.Ordinary(bytes); renderer.Match(bytes);
  }
}
void NoOpAndInvalidScopes() {
  std::vector<uint8_t> bytes(0x80000, 0x3C);
  const auto snapshot = Capture(bytes);
  const auto before = bytes;
  { Hook::ScopedReplayDrawState replay(bytes.data(), kDevice, snapshot); }
  Check(bytes == before, "unchanged replay added dirty bits or altered memory");
  auto invalid = snapshot; invalid.valid = false;
  { Hook::ScopedReplayDrawState replay(bytes.data(), kDevice, invalid); }
  { Hook::ScopedReplayDrawState replay(nullptr, kDevice, snapshot); }
  { Hook::ScopedReplayDrawState replay(bytes.data(), 0, snapshot); }
  Check(bytes == before, "inactive replay scope changed memory");
}
void PreserveReplayEntry() {
  std::vector<uint8_t> bytes(0x80000);
  const NativeDirtyWords pending{0x2000000000000000ull, 0x0010000000000000ull, 7, 9,
                                 0x0100000000000001ull};
  const NativeDirtyWords preserved{UINT64_MAX, UINT64_MAX, UINT64_MAX - 16,
                                   UINT64_MAX - 32, UINT64_MAX - 64};
  WriteWords(bytes, pending); WriteWords(bytes, preserved, kList + 64);
  Hook::PrepareReplay(bytes.data(), kDevice, kList);
  auto expected = pending;
  for (size_t word = 0; word < expected.size(); ++word) expected[word] |= ~preserved[word];
  Check(ReadWords(bytes) == expected, "replay entry erased pending CPU/restoration dirty work");
  Hook::PrepareReplay(bytes.data(), kDevice, kList);
  Check(ReadWords(bytes) == expected, "empty second replay erased pending dirty work");
}
void FollowupPartialAndEmptyReplay() {
  std::vector<uint8_t> bytes(0x80000);
  Write32(bytes, kDevice + kVS + 8 * 16, 0x3E800000);
  Write32(bytes, kDevice + kPS + 46 * 16, 0x40000000);
  const auto snapshot = Capture(bytes);
  Write32(bytes, kDevice + kVS + 8 * 16, 0x3F800000);
  Write32(bytes, kDevice + kPS + 46 * 16, 0x3F800000);
  WriteWords(bytes, All(), kList + 64);
  Renderer renderer; renderer.Apply(bytes, All(), true);
  { Hook::ScopedReplayDrawState replay(bytes.data(), kDevice, snapshot); renderer.Apply(bytes, All()); }
  // No selected commands: only the next replay's real entry block runs.
  Hook::PrepareReplay(bytes.data(), kDevice, kList);
  Write32(bytes, kDevice + kPS + 100 * 16, 0x40400000);
  auto dirty = ReadWords(bytes); dirty[1] |= (uint64_t{1} << 63) >> (100 / 4);
  WriteWords(bytes, dirty);
  renderer.Ordinary(bytes); renderer.Match(bytes);
  Check(Read32(renderer.vs.canonical().data() + 8 * 16) == 0x3F800000,
        "the reproduced vertex value was not restored");
  Check(Read32(renderer.ps.canonical().data() + 46 * 16) == 0x3F800000,
        "the reproduced pixel value was not restored");
}
void ExactFloatBits() {
  for (auto pair : {std::pair<uint32_t, uint32_t>{0, 0x80000000},
                   {0x7FC00001, 0x7FC00002}, {0x7F800000, 0xFF800000},
                   {0x7FC00001, 0x7FC00001}}) {
    std::vector<uint8_t> bytes(0x80000);
    Write32(bytes, kDevice + kVS, pair.first); const auto snapshot = Capture(bytes);
    Write32(bytes, kDevice + kVS, pair.second);
    { Hook::ScopedReplayDrawState replay(bytes.data(), kDevice, snapshot); }
    Check(ReadWords(bytes)[0] == (pair.first == pair.second ? 0 : uint64_t{1} << 63),
          "float restoration was not bit-exact for signed zero or exceptional payloads");
    Check(Read32(bytes.data() + kDevice + kVS) == pair.second, "float bits were not restored");
  }
}
void RepeatedAndNestedScopes() {
  std::vector<uint8_t> bytes(0x80000);
  auto first = Capture(bytes), second = first;
  second.shader_constants[0x1000 + 64] = 5;
  bytes[kDevice + kVS] = 7;
  Renderer renderer; renderer.Apply(bytes, All(), true);
  for (size_t repeat = 0; repeat < 8; ++repeat) {
    { Hook::ScopedReplayDrawState outer(bytes.data(), kDevice, first);
      renderer.Apply(bytes, All());
      { Hook::ScopedReplayDrawState inner(bytes.data(), kDevice, second); renderer.Apply(bytes, All()); }
      renderer.Ordinary(bytes); renderer.Match(bytes);
    }
    renderer.Ordinary(bytes); renderer.Match(bytes);
  }
}
void RestoreOtherFieldsAndUnwind() {
  std::vector<uint8_t> bytes(0x80000);
  const auto snapshot = Capture(bytes);
  bytes[kDevice + 0x480 + 0x270 - 1] = 6;
  Write32(bytes, kDevice + 12648, 0x44A00000);
  Write32(bytes, kDevice + 12432, 0x40076EB0);
  bytes[kDevice + kVS + kVSBytes - 1] = 9;
  const auto before = bytes;
  try { Hook::ScopedReplayDrawState replay(bytes.data(), kDevice, snapshot); throw 3; }
  catch (int) {}
  Check(std::equal(bytes.begin() + kDevice + 40, bytes.end(), before.begin() + kDevice + 40),
        "exception exit failed to restore complete CPU state");
  Check(ReadWords(bytes)[0] == 1, "exception exit did not publish last VS group");
}
void DeviceIsolation() {
  std::vector<uint8_t> bytes(0x80000);
  const auto snapshot = Capture(bytes);
  constexpr uint32_t other = 0x20000;
  bytes[other + kPS] = 9;
  { Hook::ScopedReplayDrawState replay(bytes.data(), other, snapshot); }
  Check(ReadWords(bytes) == NativeDirtyWords{}, "dirty invalidation escaped its device");
  Check(ReadWords(bytes, other)[1] == uint64_t{1} << 63, "second device was not invalidated");
}
void Run(const char* name, void (*test)()) {
  ++cases;
  try { test(); std::cout << "PASS " << name << '\n'; }
  catch (const std::exception& error) { ++failures; std::cerr << "FAIL " << name << ": " << error.what() << '\n'; }
}
}
int main() {
  Test::Run("all float groups, both byte boundaries, ownership and clean followup", Test::ExhaustiveGroups);
  Test::Run("both boolean banks", Test::BooleanBanks);
  Test::Run("no-op and inactive scopes", Test::NoOpAndInvalidScopes);
  Test::Run("existing dirty flags survive empty replay entry", Test::PreserveReplayEntry);
  Test::Run("original followup plus unrelated write and empty list", Test::FollowupPartialAndEmptyReplay);
  Test::Run("bit-exact signed zero and exceptional values", Test::ExactFloatBits);
  Test::Run("repeated and nested scopes", Test::RepeatedAndNestedScopes);
  Test::Run("fetch, viewport, target and exception restoration", Test::RestoreOtherFieldsAndUnwind);
  Test::Run("per-device isolation", Test::DeviceIsolation);
  std::cout << "RESULT cases=" << Test::cases << " assertions=" << Test::assertions
            << " failures=" << Test::failures << '\n';
  return Test::failures ? 1 : 0;
}
"""


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--hooks-source", type=Path)
    parser.add_argument("--output-directory", type=Path)
    parser.add_argument("--cxx", default=os.environ.get("CXX", "clang++"))
    args = parser.parse_args()
    sdk = Path(__file__).resolve().parents[2]
    source_path = args.hooks_source or sdk / "gta4-recomp/src/gta4_native_hooks.cpp"
    hooks = source_path.read_text()
    renderer = (sdk / "src/graphics/gta4_native/graphics_system.cpp").read_text()
    abi = (sdk / "include/rex/graphics/gta4_native/title_commands.h").read_text()
    output = args.output_directory or Path(tempfile.mkdtemp(prefix="liberty-replay-restore-"))
    output.mkdir(parents=True, exist_ok=True)
    extracted = {
        "snapshot": definition(hooks, "struct CapturedDrawSnapshot {"),
        "capture": definition(hooks, "void CaptureDrawState("),
        "scope": definition(hooks, "class ScopedReplayDrawState {"),
        "consume": definition(hooks, "NativeDirtyState ConsumeNativeDrawDirtyState("),
        "load": definition(hooks, "uint64_t LoadU64("),
        "store": definition(hooks, "void StoreU64("),
        "dirty_type": definition(abi, "struct NativeDirtyState {"),
        "layout": definition(renderer, "constexpr DirtyStateLayout NativeConstantDirtyLayout()"),
    }
    replay = definition(hooks, 'extern "C" void sub_82A47E28(')
    start = replay.index("\n  }", replay.index("  if (selector_tree) {")) + len("\n  }")
    extracted["entry"] = replay[start:replay.index("  uint64_t selected_count = 0;", start)]
    constants = re.findall(r"^constexpr uint32_t kReplay\w+ = [^;]+;", hooks, re.MULTILINE)
    spans = [line for line in renderer.splitlines() if line.startswith("constexpr std::array<DirtyBitSpan,")
             and any(name in line for name in ("kVertexConstantDirtySpans", "kPixelConstantDirtySpans", "kBooleanConstantDirtySpans"))]
    if len(spans) != 3 or len(constants) < 9:
        raise ValueError("production replay/layout extraction is incomplete")
    body = """#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <span>
#include <stdexcept>
#include <utility>
#include <vector>
#include "graphics/gta4_native/stateful_constant_state.h"
namespace n = rex::graphics::gta4_native;
namespace Hook {
uint8_t* GuestPointer(uint8_t* base, uint32_t address) { return base + address; }
"""
    body += "\n".join(constants) + "\n"
    body += "\n".join(extracted[key] for key in ("dirty_type", "load", "store", "snapshot", "capture", "scope", "consume"))
    body += "\nvoid PrepareReplay(uint8_t* base, uint32_t device, uint32_t command_list) {\n" + extracted["entry"] + "\n}\n}\n"
    body += "namespace Layout { using namespace n;\n" + "\n".join(spans) + "\n" + extracted["layout"] + "\n}\n"
    body += TESTS
    cpp = output / "replay_restore.cpp"
    cpp.write_text(body)
    compiler = shutil.which(args.cxx)
    if not compiler:
        raise FileNotFoundError(f"C++ compiler not found: {args.cxx}")
    command = [compiler, "-std=c++23", "-O1", "-g", "-fsanitize=address,undefined",
               "-fno-omit-frame-pointer", f"-I{sdk / 'src'}", str(cpp), "-o", str(output / "replay_restore")]
    if platform.system() == "Darwin":
        sysroot = subprocess.check_output(["xcrun", "--sdk", "macosx", "--show-sdk-path"], text=True).strip()
        command[1:1] = ["-isysroot", sysroot, "-mmacosx-version-min=26.0"]
        runtime = Path(compiler).resolve().parents[1] / "lib/c++"
        if runtime.is_dir():
            command.extend([f"-L{runtime}", f"-Wl,-rpath,{runtime}"])
    receipt = {"source": str(source_path), "source_sha256": hashlib.sha256(hooks.encode()).hexdigest(),
               "extracted_sha256": {name: hashlib.sha256(text.encode()).hexdigest() for name, text in extracted.items()},
               "command": command, "scope": "CPU-only exact production scope, consumer, entry mask block, and layout"}
    with (output / "compile.log").open("w") as log:
        build = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, timeout=120)
    receipt["compile_exit"] = build.returncode
    if build.returncode == 0:
        env = dict(os.environ, ASAN_OPTIONS="detect_leaks=0:halt_on_error=1", UBSAN_OPTIONS="halt_on_error=1")
        with (output / "tests.log").open("w") as log:
            result = subprocess.run([str(output / "replay_restore")], stdout=log, stderr=subprocess.STDOUT,
                                    env=env, timeout=60)
        receipt["test_exit"] = result.returncode
        print((output / "tests.log").read_text())
    else:
        print((output / "compile.log").read_text())
    (output / "results.json").write_text(json.dumps(receipt, indent=2) + "\n")
    print(f"Evidence: {output}")
    return receipt.get("test_exit", build.returncode)


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, subprocess.SubprocessError) as error:
        raise SystemExit(f"replay restoration test failed: {error}")
