#!/usr/bin/env python3
"""Compile the production Sony service against independent Python fixtures.

No full game build, generated PPC edits, downloads, or physical device opens are
needed. The default runtime check opens only virtual SDL devices. Physical output
requires an explicit instance ID and is never included in the default suite.
"""

from __future__ import annotations

import argparse
from decimal import Decimal
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import sys
import tempfile


def oracle_header() -> str:
    # Byte layout comes from pinned SDL test/testcontroller.c, not production
    # sony_feedback.cpp. All fixture arithmetic is performed here in Python.
    trigger_size = 11
    right_offset = 10
    left_offset = right_offset + trigger_size
    packet_size = 47
    neutral = [0x05] + [0] * (trigger_size - 1)
    resistance = [0x01, 45, 110] + [0] * (trigger_size - 3)
    recoil = [0x06, 15, 63, 128] + [0] * (trigger_size - 4)
    packet = [0] * packet_size
    packet[0] = (1 << 2) | (1 << 3)
    packet[right_offset : right_offset + trigger_size] = recoil
    packet[left_offset : left_offset + trigger_size] = resistance
    neutral_packet = [0] * packet_size
    neutral_packet[0] = packet[0]
    neutral_packet[right_offset : right_offset + trigger_size] = neutral
    neutral_packet[left_offset : left_offset + trigger_size] = neutral
    assert left_offset == 21 and len(packet) == 47
    assert packet[1:10] == [0] * 9 and packet[32:] == [0] * 15

    base = 1000
    times = {"Base": base, "BeforeBase": base - 1}
    for elapsed in (1, 10, 20, 40, 50, 79, 80, 81, 100, 110, 149, 150,
                    151, 199, 200, 249, 250, 251, 299, 300, 301, 309,
                    310, 311, 399, 400, 499, 500, 501, 510, 511, 599,
                    600, 699, 700, 701, 749, 750, 751, 800, 1000):
        times[f"T{elapsed}"] = base + elapsed

    def array(name: str, values: list[int]) -> str:
        return (f"inline constexpr std::array<uint8_t, {len(values)}> {name}"
                "{" + ",".join(str(value) for value in values) + "};\n")

    result = "#pragma once\n#include <array>\n#include <cstdint>\nnamespace sony_oracle {\n"
    for name, value in times.items():
        result += f"inline constexpr uint64_t k{name} = {value};\n"
    for name, values in (("kNeutral", neutral), ("kResistance", resistance),
                         ("kRecoil", recoil), ("kPacket", packet),
                         ("kNeutralPacket", neutral_packet),
                         ("kBlue", [0, 0, 255]), ("kOrange", [255, 128, 0]),
                         ("kPurple", [128, 0, 255]), ("kRed", [255, 0, 0])):
        result += array(name, values)
    result += f"inline constexpr int kPacketSize = {packet_size};\n"
    result += f"inline constexpr int kRightOffset = {right_offset};\n"
    result += f"inline constexpr int kLeftOffset = {left_offset};\n"
    result += f"inline constexpr uint16_t kSonyVendor = {int('054c', 16)};\n"
    result += f"inline constexpr uint16_t kDualSenseProduct = {int('0ce6', 16)};\n"
    result += f"inline constexpr uint16_t kDualShockProduct = {int('09cc', 16)};\n"
    # Deterministic physical probe: a brief low-intensity state, then neutral.
    result += "inline constexpr uint32_t kProbeDelayMs = 100;\n"
    result += "struct Swipe { float sx, sy, ex, ey; bool accepted, horizontal, positive; };\n"
    result += "inline constexpr Swipe kSwipes[] = {\n"
    for coordinates in (("0", "0", "0.20", "0"), ("0", "0", "0.199", "0"),
                        ("0.75", "0", "0.25", "0"), ("0", "0", "0", "0.20"),
                        ("0", "0.75", "0", "0.25"), ("0", "0", "0.375", "0.25"),
                        ("0", "0", "0.374", "0.25"), ("0", "0", "0.25", "0.375"),
                        ("0", "0", "0.5", "0.5")):
        sx, sy, ex, ey = map(Decimal, coordinates)
        dx, dy = ex - sx, ey - sy
        horizontal = abs(dx) >= Decimal("0.20") and abs(dx) >= abs(dy) * Decimal("1.5")
        vertical = abs(dy) >= Decimal("0.20") and abs(dy) >= abs(dx) * Decimal("1.5")
        positive = dx > 0 if horizontal else dy > 0
        floats = [f"{float(value):.9f}f" for value in coordinates]
        values = floats + [str(horizontal or vertical).lower(), str(horizontal).lower(), str(positive).lower()]
        result += "{" + ",".join(values) + "},\n"
    result += "};\n"
    result += "}\n"
    return result


def function_text(source: str, signature: str) -> str:
    """Extract an existing definition verbatim; never synthesize its policy."""
    start = source.index(signature)
    opening = source.index("{", start)
    depth = 0
    for offset in range(opening, len(source)):
        if source[offset] == "{":
            depth += 1
        elif source[offset] == "}":
            depth -= 1
            if depth == 0:
                return source[start : offset + 1]
    raise RuntimeError(f"Unclosed production function: {signature}")


def owner_routing_test(sdk: Path) -> str:
    source = (sdk / "src/input/sdl/sdl_input_driver.cpp").read_text()
    watch = function_text(source, "bool SDLCALL SDLInputDriver::EventWatch(")
    handle = function_text(source, "void SDLInputDriver::HandleEvent(")
    # Compile both actual event functions against the real service. Only the
    # unrelated sensor/keyboard sinks and clock are test doubles. This detects
    # the previously missing outer filter, unlike grepping HandleEvent alone.
    return r'''
#include <SDL3/SDL.h>
#include <rex/input/sony_feedback.h>
#include <rex/input/sdl/physical_device_inventory.h>
#include <array>
#include <iostream>
#include <limits>
#include <stdexcept>
#include "sony_feedback_oracle.h"
namespace fixture { inline uint64_t now = sony_oracle::kBase; }
#define SDL_GetTicks() fixture::now
#define assert_always() throw std::runtime_error("invalid event-watch context")
namespace rex::input::sdl {
inline constexpr uint32_t kMotionSensorNone = 0;
inline constexpr uint32_t kMotionSensorAccelerometer = 1;
inline constexpr uint32_t kMotionSensorGyroscope = 2;
struct MotionSink {
  void Observe(uint32_t, uint32_t, const std::array<float, 3>&, uint64_t, uint64_t) {}
};
struct InventorySink {
  uint64_t last_physical_input = 0;
  bool focused = true;
  void SetFocused(bool value, uint64_t) { focused = value; }
  void NotifyPhysicalInput(uint64_t timestamp) { last_physical_input = timestamp; }
  void AddPhysicalKeyboard(uint64_t, uint64_t) {}
  void RemovePhysicalKeyboard(uint64_t, uint64_t) {}
  void AddGameController(uint64_t, uint64_t) {}
  void RemoveGameController(uint64_t, uint64_t) {}
};
InventorySink& GetAbsolutePointerService() { static InventorySink sink; return sink; }
class SDLInputDriver {
 public:
  static bool SDLCALL EventWatch(void*, SDL_Event*);
  void HandleEvent(const SDL_Event&);
  MotionSink motion_samples_;
  PhysicalDeviceInventory physical_device_inventory_;
  std::atomic<bool> pointer_foreground_refresh_pending_{false};
};
''' + watch + "\n" + handle + r'''
}
void Check(bool valid, const char* message) {
  if (!valid) throw std::runtime_error(message);
}
int main() {
  try {
    using namespace rex::input::sony;
    namespace oracle = sony_oracle;
    Service& service = GetService();
    Device pad{};
    pad.instance_id = 42; pad.generation = 7; pad.model = Model::kDualSense; pad.touchpad = true;
    service.Connect(0, pad);
    GameState game{};
    game.active = game.gestures_owned = true;
    service.Publish(0, 7, game, oracle::kBase);
    Options options{};
    rex::input::sdl::SDLInputDriver driver;
    SDL_Event event{};
    auto dispatch = [&](uint32_t type, uint64_t time) {
      event.type = type;
      fixture::now = time;
      driver.EventWatch(&driver, &event);
    };
    event.gtouchpad.which = 42;
    dispatch(SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN, oracle::kT10);
    event.gtouchpad.x = 0.5f;
    dispatch(SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION, oracle::kT50);
    dispatch(SDL_EVENT_GAMEPAD_TOUCHPAD_UP, oracle::kT100);
    auto input = service.Consume(0, oracle::kT100, options);
    Check(input.gestures.size() == 1 && input.gestures.front() == Gesture::kNextRadio,
          "production EventWatch/HandleEvent dropped or misrouted touch down/up");
    event = {};
    event.gtouchpad.which = 42;
    dispatch(SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN, oracle::kT110);
    event.gtouchpad.x = std::numeric_limits<float>::quiet_NaN();
    dispatch(SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION, oracle::kT149);
    event.gtouchpad.x = 0.5f;
    dispatch(SDL_EVENT_GAMEPAD_TOUCHPAD_UP, oracle::kT150);
    Check(service.Consume(0, oracle::kT150, options).gestures.empty(),
          "production EventWatch dropped invalid motion that must cancel swipe");
    event = {};
    event.gbutton.which = 42;
    event.gbutton.button = SDL_GAMEPAD_BUTTON_TOUCHPAD;
    dispatch(SDL_EVENT_GAMEPAD_BUTTON_DOWN, oracle::kT151);
    input = service.Consume(0, oracle::kT151, options);
    Check(input.gestures.size() == 1 && input.gestures.front() == Gesture::kCamera,
          "production EventWatch/HandleEvent dropped touchpad click");
    dispatch(SDL_EVENT_GAMEPAD_BUTTON_UP, oracle::kT199);
    game.gestures_owned = false;
    service.Publish(0, 7, game, oracle::kT200);
    dispatch(SDL_EVENT_GAMEPAD_BUTTON_DOWN, oracle::kT200);
    Check(!service.ClaimsClick(0, oracle::kT200, options),
          "production click release failed to return subsequent press to Back");
    Check(service.Consume(0, oracle::kT200, options).gestures.empty(),
          "disabled publication leaked camera action");
    auto& inventory = rex::input::sdl::GetAbsolutePointerService();
    inventory.last_physical_input = 0;
    event = {};
    event.motion.which = SDL_TOUCH_MOUSEID;
    event.motion.xrel = 1.0f;
    event.common.timestamp = oracle::kT250;
    dispatch(SDL_EVENT_MOUSE_MOTION, oracle::kT250);
    Check(inventory.last_physical_input == 0, "touch-generated mouse must not claim physical ownership");
    event.motion.which = SDL_PEN_MOUSEID;
    dispatch(SDL_EVENT_MOUSE_MOTION, oracle::kT250);
    Check(inventory.last_physical_input == 0, "pen-generated mouse must not claim physical ownership");
    event.motion.which = 42;
    dispatch(SDL_EVENT_MOUSE_MOTION, oracle::kT250);
    Check(inventory.last_physical_input == oracle::kT250, "physical mouse activity was lost");
    event = {};
    dispatch(SDL_EVENT_DID_ENTER_BACKGROUND, oracle::kT300);
    Check(!inventory.focused && !driver.pointer_foreground_refresh_pending_.load(),
          "background transition must cancel ownership and pending foreground refresh");
    dispatch(SDL_EVENT_DID_ENTER_FOREGROUND, oracle::kT310);
    Check(driver.pointer_foreground_refresh_pending_.load(), "foreground refresh request was lost");
    std::cout << "PASS physical-input filtering and background/foreground ownership routing\n";
    std::cout << "PASS production EventWatch and HandleEvent bodies route touch/click and release ownership\n";
    return 0;
  } catch (const std::exception& error) {
    std::cerr << "FAIL production SDL owner routing: " << error.what() << '\n';
    return 1;
  }
}
'''


def sdl_link_flags(sdk: Path, build: Path, explicit: Path | None) -> list[str]:
    candidates = ([explicit] if explicit else []) + list(sdk.glob("out/*/libSDL3.a"))
    candidates += list(sdk.glob("out/*/SDL3-static.lib"))
    candidates += list(build.glob("**/libSDL3.a"))
    candidates += list(build.glob("**/SDL3-static.lib"))
    library = next((path for path in candidates if path and path.is_file()), None)
    if not library:
        raise RuntimeError("No existing SDL3 library found; provide --sdl-library or use --unit-only")
    flags = [str(library)]
    pc_files = list(build.glob("**/sdl3.pc"))
    if pc_files:
        for line in pc_files[0].read_text().splitlines():
            if line.startswith(("Libs:", "Libs.private:")):
                for flag in shlex.split(line.partition(":")[2]):
                    if not flag.startswith("-L") and flag != "-lSDL3":
                        flags.append(flag)
    elif sys.platform == "darwin":
        for framework in ("CoreMedia", "CoreVideo", "Cocoa", "UniformTypeIdentifiers",
                          "IOKit", "ForceFeedback", "Carbon", "CoreAudio", "AudioToolbox",
                          "AVFoundation", "Foundation", "GameController", "Metal",
                          "UserNotifications", "QuartzCore", "Security", "CoreHaptics"):
            flags.extend(["-framework", framework])
    elif sys.platform.startswith("linux"):
        flags.extend(["-ldl", "-lm", "-pthread"])
    elif os.name == "nt":
        raise RuntimeError("Provide an existing SDL sdl3.pc in --build-dir for Windows static dependencies")
    return flags


def main() -> int:
    sdk = Path(__file__).resolve().parent.parent
    repo = next(path for path in sdk.parents if (path / "docs/BUILDING.md").is_file())
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-dir", type=Path, default=repo / "out/build/macos-release")
    parser.add_argument("--output-dir", type=Path)
    parser.add_argument("--sdl-library", type=Path)
    parser.add_argument("--cxx", default=os.environ.get("CXX", "clang++"))
    parser.add_argument("--unit-only", action="store_true")
    parser.add_argument("--compile-only", action="store_true")
    parser.add_argument("--sanitize", action="store_true")
    parser.add_argument("--physical-instance", type=int,
                        help="explicit opt-in: briefly exercise one wired physical DualSense ID")
    parser.add_argument("--physical-unique-usb", action="store_true",
                        help="explicit opt-in: probe only if exactly one Sony DualSense is present and wired")
    args = parser.parse_args()
    if (args.physical_instance is not None or args.physical_unique_usb) and args.unit_only:
        parser.error("physical output requires the SDL runtime harness")
    if args.physical_instance is not None and args.physical_unique_usb:
        parser.error("select only one physical probe mode")
    output = args.output_dir or Path(tempfile.mkdtemp(prefix="liberty-sony-tests-"))
    output.mkdir(parents=True, exist_ok=True)
    (output / "sony_feedback_oracle.h").write_text(oracle_header())
    owner_source = output / "sony_feedback_owner_routing.cpp"
    owner_source.write_text(owner_routing_test(sdk))
    compiler = [args.cxx] if Path(args.cxx).is_file() else shlex.split(args.cxx)
    if not compiler or not shutil.which(compiler[0]):
        raise RuntimeError(f"Compiler unavailable: {args.cxx}")
    common = compiler + ["-std=c++20", "-O1", "-g", "-Wall", "-Wextra", "-Werror",
                         "-pthread", "-I" + str(sdk / "include"), "-I" + str(output)]
    if args.sanitize:
        common += ["-fsanitize=address,undefined", "-fno-omit-frame-pointer"]
    service = sdk / "src/input/sony_feedback.cpp"
    suffix = ".exe" if os.name == "nt" else ""
    unit = output / ("sony_feedback_test" + suffix)
    commands = [common + [str(sdk / "tests/unit/input/sony_feedback_test.cpp"),
                          str(service), "-o", str(unit)]]
    owner = output / ("sony_feedback_owner_routing" + suffix)
    commands.append(common + ["-I" + str(sdk / "thirdparty/sdl3/include"),
                              str(owner_source), str(service), "-o", str(owner)])
    runtime = output / ("sony_feedback_runtime" + suffix)
    if not args.unit_only:
        commands.append(common + ["-I" + str(sdk / "thirdparty/sdl3/include"),
                                  str(sdk / "tests/regression/controls/sony_feedback_runtime.cpp"),
                                  str(service), "-o", str(runtime)] +
                        sdl_link_flags(sdk, args.build_dir, args.sdl_library))
    manifest = {"commands": commands, "physical_output": args.physical_instance,
                "physical_unique_usb": args.physical_unique_usb,
                "oracle": "Python fixture generation from pinned SDL packet documentation"}
    (output / "commands.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print(f"Sony feedback test artifacts: {output}", flush=True)
    for command in commands:
        result = subprocess.run(command, text=True, capture_output=True, timeout=120)
        (output / (Path(command[command.index("-o") + 1]).stem + ".build.log")).write_text(
            result.stdout + result.stderr)
        if result.returncode:
            print(result.stdout + result.stderr, file=sys.stderr)
            return result.returncode
    if args.compile_only:
        print("PASS compilation; no tests or device output executed")
        return 0
    runs = [[str(unit)], [str(owner)]]
    if not args.unit_only:
        runs.append([str(runtime)])
    if args.physical_instance is not None:
        runs.append([str(runtime), "--physical-instance", str(args.physical_instance)])
    if args.physical_unique_usb:
        runs.append([str(runtime), "--physical-unique-usb"])
    failed = False
    for command in runs:
        result = subprocess.run(command, text=True, capture_output=True, timeout=30)
        label = "physical" if any(value.startswith("--physical-") for value in command) else Path(command[0]).stem
        (output / (label + ".log")).write_text(result.stdout + result.stderr)
        print(result.stdout, end="")
        if result.stderr:
            print(result.stderr, file=sys.stderr, end="")
        failed = failed or result.returncode != 0
    return 1 if failed else 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, RuntimeError, subprocess.TimeoutExpired) as error:
        print(f"FAIL: {error}", file=sys.stderr)
        raise SystemExit(1)
