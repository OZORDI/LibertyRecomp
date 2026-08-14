#!/usr/bin/env python3
"""Safely prepare FusionFix button-prompt DDS files for Xbox 360 XTD packing.

The historical version inferred texture format from payload length and could
relabel raw A8R8G8B8 bytes as DXT5. This wrapper delegates to the canonical,
header-aware conversion used by the end-to-end round-trip test.
"""

from __future__ import annotations

import shutil
import sys
import tempfile
from pathlib import Path

from button_prompt_roundtrip import (
    FUSIONFIX_SOURCE,
    RoundTripError,
    SCRIPT_DIR,
    find_required_program,
    inspect_dds,
    prepare_dds_file,
)


PROCESSED_DIR = SCRIPT_DIR / "processed_dds"


def included_source_files() -> list[Path]:
    return [
        path
        for path in sorted(FUSIONFIX_SOURCE.glob("*.dds"))
        if "controller" not in path.name.lower()
        and "key_" not in path.name.lower()
    ]


def main() -> int:
    try:
        magick = find_required_program("magick", "/opt/homebrew/bin/magick")
        if not FUSIONFIX_SOURCE.is_dir():
            raise RoundTripError(f"DDS source directory not found: {FUSIONFIX_SOURCE}")

        staging = Path(
            tempfile.mkdtemp(prefix="processed_dds_", dir=str(SCRIPT_DIR))
        )
        converted = 0
        preserved = 0

        for source in included_source_files():
            source_info = inspect_dds(source)
            destination = staging / source.name
            prepare_dds_file(source, destination, magick)
            if source_info.is_dxt5:
                preserved += 1
            else:
                converted += 1

        backup = SCRIPT_DIR / "processed_dds.previous"
        if backup.exists():
            shutil.rmtree(backup)
        if PROCESSED_DIR.exists():
            PROCESSED_DIR.rename(backup)
        staging.rename(PROCESSED_DIR)

        print(f"Prepared DDS files: {len(included_source_files())}")
        print(f"Preserved DXT5: {preserved}")
        print(f"Converted A8R8G8B8 to DXT5: {converted}")
        print(f"Output: {PROCESSED_DIR}")
        return 0
    except Exception as error:
        print(f"FAILED: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
