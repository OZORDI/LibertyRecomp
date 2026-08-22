#!/usr/bin/env python3
"""Extract the three stock GTA IV font atlases from an Xbox 360 XTD."""

from __future__ import annotations

import argparse
import shutil
import subprocess
import tempfile
from pathlib import Path

from PIL import Image


REPO_ROOT = Path(__file__).resolve().parents[3]
XTD_TOOLS = REPO_ROOT / "tools/xtd_tools"
XTD_SOURCE = XTD_TOOLS / "xtd_cli.c"
TOOLCHAIN_COMMIT = "c2645c659bd6e766f29e0cba5f13598e49d01042"
TOOLCHAIN_FILES = {
    "xcompress.dll": (
        "tools/xtd_tools/full/RAGE-Console-Texture-Editor-master/xcompress.dll"
    ),
    "xcompress_cpp.dll": (
        "tools/xtd_tools/full/RAGE-Console-Texture-Editor-master/xcompress_cpp.dll"
    ),
    "xcompress_open.dll": (
        "tools/xtd_tools/full/RAGE-Console-Texture-Editor-master/xcompress_open.dll"
    ),
}
FONT_NAMES = ("font1", "font2", "font3")
SOURCE_EXTENT = 512
OUTPUT_EXTENT = 2048


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--dds-output", type=Path)
    parser.add_argument(
        "--names",
        nargs="+",
        choices=FONT_NAMES,
        default=list(FONT_NAMES),
        help="atlas names expected in this dictionary",
    )
    return parser.parse_args()


def run(command: list[str | Path], *, cwd: Path | None = None) -> None:
    result = subprocess.run(
        [str(part) for part in command],
        cwd=cwd,
        capture_output=True,
        text=True,
        timeout=600,
    )
    if result.returncode != 0:
        details = "\n".join(part for part in (result.stdout, result.stderr) if part)
        raise RuntimeError(f"command failed: {' '.join(map(str, command))}\n{details}")


def recover_toolchain(destination: Path) -> None:
    destination.mkdir(parents=True, exist_ok=True)
    for output_name, repository_path in TOOLCHAIN_FILES.items():
        result = subprocess.run(
            ("git", "show", f"{TOOLCHAIN_COMMIT}:{repository_path}"),
            cwd=REPO_ROOT,
            capture_output=True,
        )
        if result.returncode != 0 or not result.stdout:
            details = result.stderr.decode(errors="replace")
            raise RuntimeError(f"could not recover {repository_path}: {details}")
        (destination / output_name).write_bytes(result.stdout)


def wine_path(path: Path) -> str:
    return "Z:" + str(path.resolve()).replace("/", "\\")


def main() -> None:
    arguments = parse_arguments()
    wine = shutil.which("wine")
    compiler = shutil.which("i686-w64-mingw32-gcc")
    if wine is None or compiler is None:
        raise RuntimeError("wine and i686-w64-mingw32-gcc are required")
    if not arguments.input.is_file():
        raise RuntimeError(f"missing input XTD: {arguments.input}")

    arguments.output.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="liberty-font-xtd-") as temporary:
        workspace = Path(temporary)
        toolchain = workspace / "toolchain"
        exported = workspace / "exported"
        recover_toolchain(toolchain)
        executable = toolchain / "xtd_cli.exe"
        run(
            [compiler, "-Wall", "-Wextra", "-O2", "-o", executable, XTD_SOURCE],
            cwd=REPO_ROOT,
        )
        run(
            [wine, executable, "export", wine_path(arguments.input), wine_path(exported)],
            cwd=toolchain,
        )

        inventory = {path.stem: path for path in exported.glob("*.dds")}
        missing = set(arguments.names).difference(inventory)
        if missing:
            raise RuntimeError(
                f"XTD inventory is {sorted(inventory)}; missing {sorted(missing)}"
            )
        for name in arguments.names:
            if arguments.dds_output is not None:
                arguments.dds_output.mkdir(parents=True, exist_ok=True)
                shutil.copy2(inventory[name], arguments.dds_output / f"{name}.dds")
            with Image.open(inventory[name]) as source:
                rgba = source.convert("RGBA")
            if rgba.size != (SOURCE_EXTENT, SOURCE_EXTENT):
                raise RuntimeError(
                    f"{name} is {rgba.size}, expected {SOURCE_EXTENT}x{SOURCE_EXTENT}"
                )
            traced = rgba.resize(
                (OUTPUT_EXTENT, OUTPUT_EXTENT), Image.Resampling.NEAREST
            )
            destination = arguments.output / f"{name}.png"
            traced.save(destination, compress_level=9)
            print(f"{name}: {destination}")


if __name__ == "__main__":
    main()
