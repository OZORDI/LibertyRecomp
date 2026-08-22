#!/usr/bin/env python3
"""Compute renderer-layout XXH3 identities for extracted stock font DDS data."""

from __future__ import annotations

import argparse
import struct
import subprocess
import tempfile
from pathlib import Path


DDS_HEADER_SIZE = 128


def build_hasher(source_root: Path, destination: Path) -> None:
    source = destination.with_suffix(".cpp")
    source.write_text(
        """#include <cstdio>\n
#include <fstream>\n
#include <iterator>\n
#include <vector>\n
#include <xxhash.h>\n
int main(int argc, char** argv) {\n
  for (int i = 1; i < argc; ++i) {\n
    std::ifstream stream(argv[i], std::ios::binary);\n
    std::vector<unsigned char> data((std::istreambuf_iterator<char>(stream)), {});\n
    if (data.size() < 128) return 2;\n
    const auto stock_size = data.size() - 128;\n
    std::vector<unsigned char> host_payload(16, 0);\n
    host_payload.insert(host_payload.end(), data.begin() + 128, data.end());\n
    const auto identity_hash = XXH3_64bits(host_payload.data(), stock_size);\n
    const auto full_hash = XXH3_64bits(host_payload.data(), host_payload.size());\n
    std::printf("%s stock=%zu host=%zu identity=0x%016llX full=0x%016llX\\n",\n
                argv[i], stock_size, host_payload.size(),\n
                static_cast<unsigned long long>(identity_hash),\n
                static_cast<unsigned long long>(full_hash));\n
  }\n
}\n
""",
        encoding="utf-8",
    )
    subprocess.run(
        [
            "c++",
            "-std=c++17",
            "-I",
            str(source_root / "glue/rexglue-sdk-main/thirdparty/xxHash"),
            str(source),
            str(source_root / "glue/rexglue-sdk-main/out/mac-arm64/libxxhash.a"),
            "-o",
            str(destination),
        ],
        check=True,
    )


def read_payload(path: Path) -> bytes:
    data = path.read_bytes()
    if len(data) < DDS_HEADER_SIZE or data[:4] != b"DDS ":
        raise RuntimeError(f"{path} is not a DDS file")
    width = struct.unpack_from("<I", data, 16)[0]
    height = struct.unpack_from("<I", data, 12)[0]
    fourcc = data[84:88]
    if (width, height, fourcc) != (512, 512, b"DXT5"):
        raise RuntimeError(
            f"{path} is {width}x{height} {fourcc!r}, expected 512x512 DXT5"
        )
    return data[DDS_HEADER_SIZE:]


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--source-root", type=Path, required=True)
    parser.add_argument("dds", type=Path, nargs="+")
    arguments = parser.parse_args()
    for path in arguments.dds:
        read_payload(path)
    with tempfile.TemporaryDirectory(prefix="gta4-font-hash-") as directory:
        hasher = Path(directory) / "font_hash"
        build_hasher(arguments.source_root, hasher)
        subprocess.run([str(hasher), *(str(path) for path in arguments.dds)], check=True)


if __name__ == "__main__":
    main()
