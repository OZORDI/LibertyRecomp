#!/usr/bin/env python3
"""Build, inspect, verify, and optionally install GTA IV button-prompt XTDs."""

from __future__ import annotations

import argparse
import hashlib
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable


SCRIPT_DIR = Path(__file__).resolve().parent
REPO_ROOT = SCRIPT_DIR.parents[1]
FUSIONFIX_SOURCE = (
    REPO_ROOT
    / "tools/GTAIV.EFLC.FusionFix-master/textures/pc/textures/buttons_360.wtd"
)
PRODUCTION_ROOT = REPO_ROOT / "LibertyRecompLib/private/button_prompts"
TEMPLATE_XTD = PRODUCTION_ROOT / "xbox360/buttons_360.xtd"
EMBEDDED_ROOT = REPO_ROOT / "LibertyRecomp/res/button_prompts"
FILE_TO_C_SCRIPT = REPO_ROOT / "scripts/file_to_c.py"
DOWNLOADS_MIRROR = Path.home() / "Downloads/button_prompts"
MACOS_BUILD_DIR = REPO_ROOT / "out/build/macos-release"
STALE_BUTTON_PROMPT_PATHS = (
    Path.home()
    / "Library/Application Support/LibertyRecomp/temp/button_prompts",
    Path.home()
    / "Library/Application Support/LibertyRecomp/shader_cache/button_prompts",
    REPO_ROOT
    / "out/build/macos-release/LibertyRecomp/CMakeFiles/LibertyRecomp.dir/res/button_prompts",
    REPO_ROOT
    / (
        "os/android/app/.cxx/RelWithDebInfo/493e2z53/arm64-v8a/"
        "LibertyRecomp/CMakeFiles/LibertyRecomp.dir/res/button_prompts"
    ),
)

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

TEXTURE_NAMES = (
    "a_butt",
    "b_butt",
    "x_butt",
    "y_butt",
    "lb_butt",
    "rb_butt",
    "lt_butt",
    "rt_butt",
    "back_butt",
    "start_butt",
    "dpad_all",
    "dpad_up",
    "dpad_down",
    "dpad_left",
    "dpad_right",
    "dpad_updown",
    "dpad_leftright",
    "dpad_none",
    "lstick_all",
    "lstick_up",
    "lstick_down",
    "lstick_left",
    "lstick_right",
    "lstick_updown",
    "lstick_leftright",
    "lstick_none",
    "rstick_all",
    "rstick_up",
    "rstick_down",
    "rstick_left",
    "rstick_right",
    "rstick_updown",
    "rstick_leftright",
    "rstick_none",
    "up_arrow",
    "down_arrow",
    "left_arrow",
    "right_arrow",
)

# Xbox Series X has controller overview images, but no individual prompt DDS files.
# Xbox One is the intended complete fallback for the individual Series X glyphs.
VARIANTS = (
    ("ps3", "ps3_"),
    ("ps4", "ps4_"),
    ("ps5", "ps5_"),
    ("switch", "switch_"),
    ("steam_controller", "sc_"),
    ("steam_deck", "sd_"),
    ("xbox_one", "xbone_"),
    ("xbox_series_x", "xbone_"),
    ("xbox360", ""),
)
VARIANT_NAMES = tuple(name for name, _ in VARIANTS)

DDSD_PITCH = 0x8
DDSD_MIPMAPCOUNT = 0x20000
DDSD_LINEARSIZE = 0x80000
DDPF_ALPHAPIXELS = 0x1
DDPF_FOURCC = 0x4
DDPF_RGB = 0x40
DDSCAPS_COMPLEX = 0x8
DDSCAPS_TEXTURE = 0x1000
DDSCAPS_MIPMAP = 0x400000


class RoundTripError(RuntimeError):
    """Raised when a conversion or verification invariant fails."""


@dataclass(frozen=True)
class DDSInfo:
    width: int
    height: int
    header_flags: int
    pixel_flags: int
    fourcc: bytes
    rgb_bit_count: int
    red_mask: int
    green_mask: int
    blue_mask: int
    alpha_mask: int
    raw_mip_count: int
    payload_size: int

    @property
    def is_dxt5(self) -> bool:
        return bool(self.pixel_flags & DDPF_FOURCC) and self.fourcc == b"DXT5"

    @property
    def is_a8r8g8b8(self) -> bool:
        return (
            bool(self.pixel_flags & DDPF_RGB)
            and bool(self.pixel_flags & DDPF_ALPHAPIXELS)
            and self.rgb_bit_count == 32
            and self.red_mask == 0x00FF0000
            and self.green_mask == 0x0000FF00
            and self.blue_mask == 0x000000FF
            and self.alpha_mask == 0xFF000000
        )


@dataclass(frozen=True)
class PlatformResult:
    name: str
    root: Path
    staged_xtd: Path
    prepared_dds: Path
    prepared_png: Path
    contact_sheet: Path


@dataclass(frozen=True)
class TextureRecord:
    name: str
    width: int
    height: int
    texture_type: int
    gpu_offset: int


@dataclass(frozen=True)
class PendingReplacement:
    target: Path
    source: Path
    backup: Path
    pending: Path


def find_required_program(name: str, fallback: str | None = None) -> str:
    path = shutil.which(name)
    if path:
        return path
    if fallback and Path(fallback).is_file():
        return fallback
    raise RoundTripError(f"Required program not found: {name}")


def run_command(
    command: Iterable[str | Path],
    *,
    cwd: Path | None = None,
    timeout: int = 300,
) -> subprocess.CompletedProcess[str]:
    args = [str(part) for part in command]
    environment = os.environ.copy()
    environment.setdefault("WINEDEBUG", "-all")
    result = subprocess.run(
        args,
        cwd=str(cwd) if cwd else None,
        env=environment,
        capture_output=True,
        text=True,
        timeout=timeout,
    )
    if result.returncode != 0:
        details = "\n".join(part for part in (result.stdout, result.stderr) if part)
        raise RoundTripError(
            f"Command failed ({result.returncode}): {' '.join(args)}\n{details}"
        )
    return result


def inspect_dds(path: Path) -> DDSInfo:
    data = path.read_bytes()
    if len(data) < 128 or data[:4] != b"DDS ":
        raise RoundTripError(f"Invalid legacy DDS file: {path}")
    if struct.unpack_from("<I", data, 4)[0] != 124:
        raise RoundTripError(f"Invalid DDS header size: {path}")
    if struct.unpack_from("<I", data, 76)[0] != 32:
        raise RoundTripError(f"Invalid DDS pixel-format header size: {path}")

    info = DDSInfo(
        width=struct.unpack_from("<I", data, 16)[0],
        height=struct.unpack_from("<I", data, 12)[0],
        header_flags=struct.unpack_from("<I", data, 8)[0],
        pixel_flags=struct.unpack_from("<I", data, 80)[0],
        fourcc=data[84:88],
        rgb_bit_count=struct.unpack_from("<I", data, 88)[0],
        red_mask=struct.unpack_from("<I", data, 92)[0],
        green_mask=struct.unpack_from("<I", data, 96)[0],
        blue_mask=struct.unpack_from("<I", data, 100)[0],
        alpha_mask=struct.unpack_from("<I", data, 104)[0],
        raw_mip_count=struct.unpack_from("<I", data, 28)[0],
        payload_size=len(data) - 128,
    )
    if info.fourcc == b"DX10":
        raise RoundTripError(f"DX10 DDS headers are not accepted: {path}")
    return info


def bc3_top_level_size(width: int, height: int) -> int:
    block_columns = max(1, (width + 3) // 4)
    block_rows = max(1, (height + 3) // 4)
    return block_columns * block_rows * 16


def validate_prepared_dds(path: Path) -> DDSInfo:
    info = inspect_dds(path)
    if (info.width, info.height) != (64, 64):
        raise RoundTripError(
            f"Prepared DDS must be 64x64, got {info.width}x{info.height}: {path}"
        )
    if not info.is_dxt5:
        raise RoundTripError(f"Prepared DDS is not legacy DXT5: {path}")
    expected_payload = bc3_top_level_size(info.width, info.height)
    if info.payload_size != expected_payload:
        raise RoundTripError(
            f"Prepared DDS payload is {info.payload_size}, expected {expected_payload}: {path}"
        )
    if info.raw_mip_count != 0 or info.header_flags & DDSD_MIPMAPCOUNT:
        raise RoundTripError(f"Prepared DDS contains mipmap metadata: {path}")
    return info


def normalize_dxt5_file(source: Path, destination: Path) -> None:
    source_info = inspect_dds(source)
    if not source_info.is_dxt5:
        raise RoundTripError(f"Cannot normalize a non-DXT5 DDS: {source}")
    if (source_info.width, source_info.height) != (64, 64):
        raise RoundTripError(
            f"DXT5 input must be 64x64, got {source_info.width}x{source_info.height}: {source}"
        )

    data = bytearray(source.read_bytes())
    top_level_size = bc3_top_level_size(source_info.width, source_info.height)
    if len(data) < 128 + top_level_size:
        raise RoundTripError(f"Truncated DXT5 payload: {source}")

    header = bytearray(data[:128])
    flags = struct.unpack_from("<I", header, 8)[0]
    flags |= DDSD_LINEARSIZE
    flags &= ~DDSD_PITCH
    flags &= ~DDSD_MIPMAPCOUNT
    struct.pack_into("<I", header, 8, flags)
    struct.pack_into("<I", header, 20, top_level_size)
    struct.pack_into("<I", header, 24, 0)
    struct.pack_into("<I", header, 28, 0)
    struct.pack_into("<I", header, 80, DDPF_FOURCC)
    header[84:88] = b"DXT5"
    for offset in (88, 92, 96, 100, 104):
        struct.pack_into("<I", header, offset, 0)
    caps = struct.unpack_from("<I", header, 108)[0]
    caps |= DDSCAPS_TEXTURE
    caps &= ~DDSCAPS_COMPLEX
    caps &= ~DDSCAPS_MIPMAP
    struct.pack_into("<I", header, 108, caps)
    for offset in (112, 116, 120, 124):
        struct.pack_into("<I", header, offset, 0)

    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.write_bytes(bytes(header) + bytes(data[128 : 128 + top_level_size]))
    validate_prepared_dds(destination)


def prepare_dds_file(source: Path, destination: Path, magick: str) -> None:
    info = inspect_dds(source)
    if (info.width, info.height) != (64, 64):
        raise RoundTripError(
            f"Source DDS must be 64x64, got {info.width}x{info.height}: {source}"
        )

    if info.is_dxt5:
        normalize_dxt5_file(source, destination)
        return

    if not info.is_a8r8g8b8:
        raise RoundTripError(
            f"Unsupported DDS source format; expected DXT5 or A8R8G8B8: {source}"
        )

    destination.parent.mkdir(parents=True, exist_ok=True)
    encoded = destination.with_name(f"{destination.stem}.encoded.dds")
    run_command(
        (
            magick,
            source,
            "-define",
            "dds:compression=dxt5",
            encoded,
        )
    )
    try:
        normalize_dxt5_file(encoded, destination)
    finally:
        encoded.unlink(missing_ok=True)


def dds_to_png(source: Path, destination: Path, magick: str) -> None:
    destination.parent.mkdir(parents=True, exist_ok=True)
    run_command((magick, source, "-define", "png:color-type=6", destination))
    if not destination.is_file():
        raise RoundTripError(f"ImageMagick did not create PNG: {destination}")


def exact_png_match(first: Path, second: Path, magick: str) -> None:
    result = subprocess.run(
        (magick, "compare", "-metric", "AE", str(first), str(second), "null:"),
        capture_output=True,
        text=True,
    )
    if result.returncode != 0:
        metric = (result.stderr or result.stdout).strip()
        raise RoundTripError(
            f"PNG pixels differ ({metric or 'unknown error'}): {first} vs {second}"
        )


def dds_payload(path: Path) -> bytes:
    info = validate_prepared_dds(path)
    expected_size = bc3_top_level_size(info.width, info.height)
    data = path.read_bytes()
    return data[128 : 128 + expected_size]


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for chunk in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def git_extract_toolchain(destination: Path, git: str) -> None:
    destination.mkdir(parents=True, exist_ok=True)
    for output_name, repository_path in TOOLCHAIN_FILES.items():
        result = subprocess.run(
            (git, "show", f"{TOOLCHAIN_COMMIT}:{repository_path}"),
            cwd=REPO_ROOT,
            capture_output=True,
        )
        if result.returncode != 0 or not result.stdout:
            details = result.stderr.decode(errors="replace")
            raise RoundTripError(
                f"Unable to recover {repository_path} from {TOOLCHAIN_COMMIT}: {details}"
            )
        (destination / output_name).write_bytes(result.stdout)


def compile_xtd_cli(destination: Path, compiler: str) -> Path:
    executable = destination / "xtd_cli.exe"
    run_command(
        (
            compiler,
            "-Wall",
            "-Wextra",
            "-O2",
            "-o",
            executable,
            SCRIPT_DIR / "xtd_cli.c",
        ),
        cwd=REPO_ROOT,
    )
    if not executable.is_file():
        raise RoundTripError("MinGW did not produce xtd_cli.exe")
    return executable


def wine_path(path: Path) -> str:
    return "Z:" + str(path.resolve()).replace("/", "\\")


def run_xtd_cli(
    wine: str,
    executable: Path,
    toolchain_dir: Path,
    arguments: Iterable[str],
) -> subprocess.CompletedProcess[str]:
    return run_command(
        (wine, executable, *arguments),
        cwd=toolchain_dir,
        timeout=600,
    )


def read_be_u16(data: bytes | bytearray, offset: int) -> int:
    return struct.unpack_from(">H", data, offset)[0]


def read_be_u32(data: bytes | bytearray, offset: int) -> int:
    return struct.unpack_from(">I", data, offset)[0]


def resource_offset(value: int) -> int:
    if value == 0:
        return 0
    section = value >> 28
    if section not in (5, 6):
        raise RoundTripError(f"Invalid RSC virtual offset: 0x{value:08X}")
    return value & 0x0FFFFFFF


def parse_texture_records(data: bytes | bytearray) -> dict[str, TextureRecord]:
    if len(data) < 32:
        raise RoundTripError("Decompressed XTD is too small")
    texture_count = read_be_u16(data, 20)
    texture_list_offset = resource_offset(read_be_u32(data, 24))
    if texture_count == 0 or texture_list_offset >= len(data):
        raise RoundTripError(
            f"Invalid XTD dictionary header: count={texture_count}, "
            f"list_offset=0x{texture_list_offset:X}"
        )

    records: dict[str, TextureRecord] = {}
    for index in range(texture_count):
        list_entry = texture_list_offset + index * 4
        if list_entry + 4 > len(data):
            raise RoundTripError("XTD texture list extends beyond decompressed data")
        texture_offset = resource_offset(read_be_u32(data, list_entry))
        if texture_offset + 32 > len(data):
            raise RoundTripError("XTD texture record extends beyond decompressed data")

        name_offset = resource_offset(read_be_u32(data, texture_offset + 20))
        d3d_offset = resource_offset(read_be_u32(data, texture_offset + 24))
        width = read_be_u16(data, texture_offset + 28)
        height = read_be_u16(data, texture_offset + 30)
        if name_offset >= len(data) or d3d_offset + 36 > len(data):
            raise RoundTripError("XTD texture record contains an invalid pointer")

        name_end = data.find(b"\0", name_offset)
        if name_end < 0:
            raise RoundTripError("Unterminated texture name in XTD")
        name = bytes(data[name_offset:name_end]).decode("ascii")
        if name.startswith("pack:/"):
            name = name[6:]
        if name.endswith(".dds"):
            name = name[:-4]

        dword9 = read_be_u32(data, d3d_offset + 32)
        texture_type = dword9 & 0x3F
        gpu_offset = dword9 & 0x00FFFF00
        if name in records:
            raise RoundTripError(f"Duplicate texture name in XTD: {name}")
        records[name] = TextureRecord(
            name=name,
            width=width,
            height=height,
            texture_type=texture_type,
            gpu_offset=gpu_offset,
        )

    expected = set(TEXTURE_NAMES)
    actual = set(records)
    if actual != expected:
        raise RoundTripError(
            "XTD template inventory mismatch; "
            f"missing={sorted(expected - actual)}, unexpected={sorted(actual - expected)}"
        )
    return records


def rsc_segment_sizes(header: bytes) -> tuple[int, int]:
    if len(header) < 16 or header[:4] != b"RSC\x05":
        raise RoundTripError("Invalid RSC5 header")
    flags = struct.unpack_from("<I", header, 8)[0]
    cpu_size = (flags & 0x7FF) << (((flags >> 11) & 0xF) + 8)
    gpu_size = ((flags >> 15) & 0x7FF) << (((flags >> 26) & 0xF) + 8)
    return cpu_size, gpu_size


def xg_address_2d_tiled_offset(
    x: int, y: int, width_in_blocks: int, texel_pitch: int
) -> int:
    aligned_width = (width_in_blocks + 31) & ~31
    log_bpp = (texel_pitch >> 2) + ((texel_pitch >> 1) >> (texel_pitch >> 2))
    macro = (
        (x >> 5) + (y >> 5) * (aligned_width >> 5)
    ) << (log_bpp + 7)
    micro = ((x & 7) + ((y & 6) << 2)) << log_bpp
    offset = (
        macro
        + ((micro & ~15) << 1)
        + (micro & 15)
        + ((y & 8) << (3 + log_bpp))
        + ((y & 1) << 4)
    )
    return (
        ((offset & ~511) << 3)
        + ((offset & 448) << 2)
        + (offset & 63)
        + ((y & 16) << 7)
        + (((((y & 8) >> 2) + (x >> 3)) & 3) << 6)
    ) >> log_bpp


def swizzle_dxt5_payload(payload: bytes, width: int, height: int) -> bytes:
    texel_pitch = 16
    block_width = width // 4
    block_height = height // 4
    aligned_width = (width + 127) & ~127
    aligned_height = (height + 127) & ~127
    padded_size = aligned_width * aligned_height

    endian_swapped = bytearray(payload)
    for index in range(0, len(endian_swapped), 2):
        endian_swapped[index], endian_swapped[index + 1] = (
            endian_swapped[index + 1],
            endian_swapped[index],
        )

    swizzled = bytearray(padded_size)
    for y in range(block_height):
        for x in range(block_width):
            source_offset = (x + y * block_width) * texel_pitch
            destination_offset = (
                xg_address_2d_tiled_offset(x, y, block_width, texel_pitch)
                * texel_pitch
            )
            source_end = source_offset + texel_pitch
            destination_end = destination_offset + texel_pitch
            if source_end > len(endian_swapped) or destination_end > len(swizzled):
                raise RoundTripError("DXT5 swizzle offset is outside its buffer")
            swizzled[destination_offset:destination_end] = endian_swapped[
                source_offset:source_end
            ]
    return bytes(swizzled)


def build_xtd(
    prepared_dir: Path,
    output_path: Path,
    workspace: Path,
    wine: str,
    xtd_executable: Path,
    toolchain_dir: Path,
) -> None:
    workspace.mkdir(parents=True, exist_ok=True)
    template_data = TEMPLATE_XTD.read_bytes()
    if len(template_data) < 20:
        raise RoundTripError(f"XTD template is truncated: {TEMPLATE_XTD}")
    decompressed_path = workspace / "decompressed.bin"
    patched_path = workspace / "patched.bin"

    run_xtd_cli(
        wine,
        xtd_executable,
        toolchain_dir,
        (
            "unpack",
            wine_path(TEMPLATE_XTD),
            wine_path(decompressed_path),
        ),
    )
    decompressed = bytearray(decompressed_path.read_bytes())
    cpu_size, gpu_size = rsc_segment_sizes(template_data[:16])
    if len(decompressed) < cpu_size + gpu_size:
        raise RoundTripError(
            f"Decompressed XTD size is {len(decompressed)}, "
            f"smaller than the declared segments ({cpu_size + gpu_size})"
        )
    records = parse_texture_records(decompressed)

    for texture_name in TEXTURE_NAMES:
        record = records[texture_name]
        if record.texture_type != 20:
            raise RoundTripError(
                f"Unsupported XTD texture type {record.texture_type}: {texture_name}"
            )
        prepared = prepared_dir / f"{texture_name}.dds"
        validate_prepared_dds(prepared)
        payload = dds_payload(prepared)
        swizzled = swizzle_dxt5_payload(payload, record.width, record.height)
        write_offset = cpu_size + record.gpu_offset
        write_end = write_offset + len(swizzled)
        if write_end > len(decompressed):
            raise RoundTripError(
                f"XTD GPU write is out of bounds for {texture_name}: "
                f"0x{write_offset:X}-0x{write_end:X}"
            )
        decompressed[write_offset:write_end] = swizzled

    patched_path.write_bytes(decompressed)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    run_xtd_cli(
        wine,
        xtd_executable,
        toolchain_dir,
        (
            "pack",
            wine_path(TEMPLATE_XTD),
            wine_path(patched_path),
            wine_path(output_path),
        ),
    )
    if not output_path.is_file():
        raise RoundTripError("XTD packing produced no output")


def export_xtd(
    xtd_path: Path,
    output_dir: Path,
    wine: str,
    executable: Path,
    toolchain_dir: Path,
) -> None:
    output_dir.parent.mkdir(parents=True, exist_ok=True)
    run_xtd_cli(
        wine,
        executable,
        toolchain_dir,
        ("export", wine_path(xtd_path), wine_path(output_dir)),
    )
    exported_names = {path.stem for path in output_dir.glob("*.dds")}
    expected_names = set(TEXTURE_NAMES)
    if exported_names != expected_names:
        missing = sorted(expected_names - exported_names)
        unexpected = sorted(exported_names - expected_names)
        raise RoundTripError(
            f"XTD export inventory mismatch; missing={missing}, unexpected={unexpected}"
        )


def verify_exported_textures(
    prepared_dds: Path,
    prepared_png: Path,
    extracted_dds: Path,
    final_png: Path,
    magick: str,
) -> None:
    final_png.mkdir(parents=True, exist_ok=True)
    for texture_name in TEXTURE_NAMES:
        prepared_path = prepared_dds / f"{texture_name}.dds"
        extracted_path = extracted_dds / f"{texture_name}.dds"
        if dds_payload(prepared_path) != dds_payload(extracted_path):
            raise RoundTripError(
                f"BC3 payload changed during XTD round trip: {texture_name}"
            )
        final_path = final_png / f"{texture_name}.png"
        dds_to_png(extracted_path, final_path, magick)
        exact_png_match(prepared_png / f"{texture_name}.png", final_path, magick)


def make_contact_sheet(
    platform_root: Path,
    source_png: Path,
    final_png: Path,
    magick: str,
) -> Path:
    contact_work = platform_root / "contact_work"
    contact_work.mkdir(parents=True, exist_ok=True)
    panels: list[Path] = []

    for texture_name in TEXTURE_NAMES:
        source_render = contact_work / f"{texture_name}.source-render.png"
        final_render = contact_work / f"{texture_name}.final-render.png"
        source_preview = contact_work / f"{texture_name}.source-preview.png"
        final_preview = contact_work / f"{texture_name}.final-preview.png"
        difference_preview = contact_work / f"{texture_name}.difference-preview.png"
        difference = contact_work / f"{texture_name}.difference.png"
        panel = contact_work / f"{texture_name}.panel.png"

        for input_path, render_path in (
            (source_png / f"{texture_name}.png", source_render),
            (final_png / f"{texture_name}.png", final_render),
        ):
            run_command(
                (
                    magick,
                    "-size",
                    "64x64",
                    "pattern:checkerboard",
                    input_path,
                    "-alpha",
                    "on",
                    "-compose",
                    "over",
                    "-composite",
                    render_path,
                )
            )

        for render_path, label, preview_path in (
            (source_render, "SOURCE", source_preview),
            (final_render, "FINAL", final_preview),
        ):
            run_command(
                (
                    magick,
                    render_path,
                    "-filter",
                    "point",
                    "-resize",
                    "192x192",
                    "-gravity",
                    "south",
                    "-background",
                    "#202020",
                    "-fill",
                    "white",
                    "-splice",
                    "0x24",
                    "-pointsize",
                    "15",
                    "-annotate",
                    "+0+4",
                    label,
                    preview_path,
                )
            )

        run_command(
            (
                magick,
                source_render,
                final_render,
                "-compose",
                "difference",
                "-composite",
                "-channel",
                "RGBA",
                "-evaluate",
                "multiply",
                "8",
                "+channel",
                difference,
            )
        )
        run_command(
            (
                magick,
                difference,
                "-filter",
                "point",
                "-resize",
                "192x192",
                "-gravity",
                "south",
                "-background",
                "#202020",
                "-fill",
                "white",
                "-splice",
                "0x24",
                "-pointsize",
                "15",
                "-annotate",
                "+0+4",
                "DIFF x8",
                difference_preview,
            )
        )
        run_command(
            (
                magick,
                source_preview,
                final_preview,
                difference_preview,
                "+append",
                "-gravity",
                "north",
                "-background",
                "#101010",
                "-fill",
                "white",
                "-splice",
                "0x28",
                "-pointsize",
                "17",
                "-annotate",
                "+0+6",
                texture_name,
                panel,
            )
        )
        panels.append(panel)

    contact_sheet = platform_root / "contact_sheet.png"
    run_command(
        (
            magick,
            "montage",
            *panels,
            "-tile",
            "3x",
            "-geometry",
            "+10+10",
            "-background",
            "#181818",
            contact_sheet,
        ),
        timeout=600,
    )
    return contact_sheet


def run_platform(
    name: str,
    prefix: str,
    qa_root: Path,
    magick: str,
    wine: str,
    xtd_executable: Path,
    toolchain_dir: Path,
) -> PlatformResult:
    platform_root = qa_root / name
    source_png = platform_root / "source_png"
    prepared_dds = platform_root / "prepared_dds"
    prepared_png = platform_root / "prepared_png"
    extracted_dds = platform_root / "extracted_dds"
    final_png = platform_root / "final_png"
    staged_xtd = platform_root / "buttons_360.xtd"

    print(f"[{name}] Preparing and rendering source DDS files", flush=True)
    for texture_name in TEXTURE_NAMES:
        source = FUSIONFIX_SOURCE / f"{prefix}{texture_name}.dds"
        if not source.is_file():
            raise RoundTripError(f"Missing source DDS for {name}: {source}")
        dds_to_png(source, source_png / f"{texture_name}.png", magick)
        prepared = prepared_dds / f"{texture_name}.dds"
        prepare_dds_file(source, prepared, magick)
        dds_to_png(prepared, prepared_png / f"{texture_name}.png", magick)

    print(f"[{name}] Replacing all textures and building XTD", flush=True)
    build_xtd(
        prepared_dds,
        staged_xtd,
        platform_root / "xtd_work",
        wine,
        xtd_executable,
        toolchain_dir,
    )

    print(f"[{name}] Exporting and verifying XTD round trip", flush=True)
    export_xtd(staged_xtd, extracted_dds, wine, xtd_executable, toolchain_dir)
    verify_exported_textures(
        prepared_dds,
        prepared_png,
        extracted_dds,
        final_png,
        magick,
    )
    contact_sheet = make_contact_sheet(platform_root, source_png, final_png, magick)
    print(f"[{name}] PASS — {contact_sheet}", flush=True)

    return PlatformResult(
        name=name,
        root=platform_root,
        staged_xtd=staged_xtd,
        prepared_dds=prepared_dds,
        prepared_png=prepared_png,
        contact_sheet=contact_sheet,
    )


def restore_production(backups: dict[str, Path]) -> None:
    for name, backup in backups.items():
        target = PRODUCTION_ROOT / name / "buttons_360.xtd"
        rollback = target.parent / "buttons_360.xtd.roundtrip-rollback"
        shutil.copy2(backup, rollback)
        os.replace(rollback, target)


def install_and_verify(
    results: dict[str, PlatformResult],
    qa_root: Path,
    magick: str,
    wine: str,
    executable: Path,
    toolchain_dir: Path,
) -> None:
    backup_root = qa_root / "production_backups"
    backup_root.mkdir(parents=True, exist_ok=True)
    backups: dict[str, Path] = {}
    pending: dict[str, Path] = {}

    for name, result in results.items():
        target = PRODUCTION_ROOT / name / "buttons_360.xtd"
        backup = backup_root / name / "buttons_360.xtd"
        backup.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(target, backup)
        backups[name] = backup

        replacement = target.parent / "buttons_360.xtd.roundtrip-new"
        shutil.copy2(result.staged_xtd, replacement)
        pending[name] = replacement

    replaced_names: list[str] = []
    try:
        for name, replacement in pending.items():
            target = PRODUCTION_ROOT / name / "buttons_360.xtd"
            os.replace(replacement, target)
            replaced_names.append(name)

        post_install_root = qa_root / "post_install"
        for name, result in results.items():
            target = PRODUCTION_ROOT / name / "buttons_360.xtd"
            if sha256(target) != sha256(result.staged_xtd):
                raise RoundTripError(f"Installed XTD hash mismatch: {name}")

            extracted = post_install_root / name / "extracted_dds"
            final_png = post_install_root / name / "final_png"
            export_xtd(target, extracted, wine, executable, toolchain_dir)
            verify_exported_textures(
                result.prepared_dds,
                result.prepared_png,
                extracted,
                final_png,
                magick,
            )
            print(f"[{name}] Installed XTD verified", flush=True)
    except Exception:
        if replaced_names:
            restore_production(backups)
        raise
    finally:
        for replacement in pending.values():
            replacement.unlink(missing_ok=True)


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Build and verify all LibertyRecomp button-prompt XTD variants."
    )
    parser.add_argument(
        "--install",
        action="store_true",
        help="Install all nine XTDs only after the complete round-trip test passes.",
    )
    return parser.parse_args()


def main() -> int:
    arguments = parse_arguments()
    qa_root = Path(tempfile.mkdtemp(prefix="libertyrecomp_button_prompts_roundtrip_"))
    print(f"QA workspace: {qa_root}", flush=True)

    try:
        magick = find_required_program("magick", "/opt/homebrew/bin/magick")
        wine = find_required_program("wine", "/opt/homebrew/bin/wine")
        git = find_required_program("git")
        compiler = find_required_program("i686-w64-mingw32-gcc")
        if not FUSIONFIX_SOURCE.is_dir():
            raise RoundTripError(f"FusionFix DDS source not found: {FUSIONFIX_SOURCE}")
        if not TEMPLATE_XTD.is_file():
            raise RoundTripError(f"XTD template not found: {TEMPLATE_XTD}")

        toolchain_dir = qa_root / "toolchain"
        git_extract_toolchain(toolchain_dir, git)
        xtd_executable = compile_xtd_cli(toolchain_dir, compiler)

        results: dict[str, PlatformResult] = {}
        for name, prefix in VARIANTS:
            results[name] = run_platform(
                name,
                prefix,
                qa_root,
                magick,
                wine,
                xtd_executable,
                toolchain_dir,
            )

        if arguments.install:
            print("All staging checks passed; installing production XTDs", flush=True)
            install_and_verify(
                results,
                qa_root,
                magick,
                wine,
                xtd_executable,
                toolchain_dir,
            )

        print("All button-prompt round-trip checks passed.", flush=True)
        print("Contact sheets:", flush=True)
        for name, result in results.items():
            print(f"  {name}: {result.contact_sheet}", flush=True)
        if arguments.install:
            print("All nine verified XTDs are installed.", flush=True)
        else:
            print("Production XTDs were not changed; rerun with --install to install.", flush=True)
        return 0
    except Exception as error:
        print(f"FAILED: {error}", file=sys.stderr, flush=True)
        print(f"QA workspace retained: {qa_root}", file=sys.stderr, flush=True)
        return 1


if __name__ == "__main__":
    sys.exit(main())
