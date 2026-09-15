#!/usr/bin/env python3
"""Generate independent arithmetic fixtures for shared touch runtime checks."""
import argparse
import math
from pathlib import Path
import struct

parser = argparse.ArgumentParser()
parser.add_argument("--output", required=True, type=Path)
args = parser.parse_args()

def f32(value):
    return struct.unpack("f", struct.pack("f", value))[0]

step = 3.0
half_axis = math.floor(255 * 0.5 + 0.5)
values = {
    "kGuestBytes": ("size_t", 1 << 32),
    "kCameraStep": ("float", f"{step}f"),
    "kCameraUnits": ("int32_t", int(step * 12)),
    "kCameraHalfRateUnits": ("int32_t", int(step * 12 * (1 / 60) / f32(1 / 30))),
    "kAimCameraUnits": ("int32_t", int(step * 12 * f32(0.35))),
    "kHalfAxis": ("int32_t", half_axis),
    "kHalfNativeAxis": ("int16_t", math.floor(half_axis * 32767 / 255 + 0.5)),
    "kPinchSpread": ("float", "12.0f"),
    "kPinchZoom": ("int32_t", -12 * 12),
    "kHoldTimestamp": ("uint64_t", int(0.35 * 1_000_000_000)),
    "kControlA": ("uint32_t", "0x300000"),
    "kControlB": ("uint32_t", "0x304000"),
    "kControlOther": ("uint32_t", "0x308000"),
    "kScriptContext": ("uint32_t", "0x4000"),
    "kScriptArguments": ("uint32_t", "0x5000"),
    "kFrontendObject": ("uint32_t", "0x70000"),
    "kFrontendRowX": ("float", f"{0.2 * 1280}f"),
    "kFrontendRowY": ("float", f"{0.3 * 720}f"),
    "kFrontendColumnBits": ("uint32_t", hex(struct.unpack("I", struct.pack("f", 0.2))[0])),
    "kFrontendHeightBits": ("uint32_t", hex(struct.unpack("I", struct.pack("f", 0.05))[0])),
    "kFrontendAcceptCurrent": ("uint32_t", 2328 + 77 * 12 + 2),
    "kMapXCurrent": ("uint32_t", 2328 + 72 * 12 + 2),
    "kMapCenterX": ("float", f"{1280 / 2}f"),
    "kMapCenterY": ("float", f"{720 / 2}f"),
    "kMapDragPixels": ("float", f"{1280 * 0.05}f"),
    "kLookRightCurrent": ("uint32_t", 2328 + 17 * 12 + 2),
    "kFadeSecond": ("uint64_t", 1_000_000_000),
    "kFadeQuarterDuration": ("uint64_t", int(0.25 * 1_000_000_000)),
    "kFadeHalfDuration": ("uint64_t", int(0.5 * 1_000_000_000)),
    "kEditorFadeOutQuarterAlpha": ("float", f"{1 - 0.25}f"),
    "kFadeStart": ("uint64_t", 10 * 1_000_000_000),
    "kFadeQuarter": ("uint64_t", int(10.25 * 1_000_000_000)),
    "kFadeFull": ("uint64_t", 11 * 1_000_000_000),
    "kFadeHalfOut": ("uint64_t", int(11.5 * 1_000_000_000)),
    "kFadeReverseQuarter": ("uint64_t", int(11.75 * 1_000_000_000)),
    "kFadeReverseFull": ("uint64_t", int(12.5 * 1_000_000_000)),
    "kFadeHidden": ("uint64_t", int(13.5 * 1_000_000_000)),
    "kFadeQuarterAlpha": ("float", f"{0.25}f"),
    "kFadeHalfOutAlpha": ("float", f"{1 - 0.5}f"),
    "kFadeReversedAlpha": ("float", f"{0.5 + (1 - 0.5) * 0.25}f"),
}
lines = ["#pragma once", "#include <cstddef>", "#include <cstdint>", "namespace touch_samples {"]
for name, (kind, value) in values.items():
    lines.append(f"inline constexpr {kind} {name} = {value};")
outputs = [0x6100 + index * 4 for index in range(4)]
lines.append("inline constexpr uint32_t kStickOutputs[] = {" + ", ".join(hex(v) for v in outputs) + "};")
# Installed common/data/hud.dat: HUD_RADAR in HD and CRT profiles. Retail
# sub_821C31A8 may additionally scale only X and width by 0.75.
radars = []
for x, y, width, height in [(0.064, 0.745, 0.161, 0.211), (0.089, 0.720, 0.150, 0.197)]:
    for horizontal in [1.0, 0.75]:
        x, y, width, height = map(f32, (x, y, width, height))
        left, top = f32(x * horizontal), y
        right = left + f32(width * horizontal)
        bottom = top + height
        radars.append([left, top, right, bottom])
lines.append("inline constexpr double kRadarStyleBounds[][4] = {")
for radar in radars:
    lines.append("  {" + ", ".join(value.hex() for value in radar) + "},")
lines.append("};")
lines.append("}")
args.output.parent.mkdir(parents=True, exist_ok=True)
args.output.write_text("\n".join(lines) + "\n")
