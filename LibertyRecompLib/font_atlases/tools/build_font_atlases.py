#!/usr/bin/env python3
"""Build metric-aware high-resolution GTA IV font atlases.

The Xbox renderer addresses a 16-column atlas with 32x40 source-pixel cells,
but FONTS.DAT stores the amount trimmed from each proportional glyph cell.
Licensed faces are rasterized against the exact sampled rectangle. Each face
uses one FreeType size and baseline, retaining vector advances and bearings.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import math
import shutil
import statistics
import tempfile
import zipfile
from dataclasses import dataclass
from pathlib import Path

from PIL import Image, ImageChops, ImageDraw, ImageFont


ATLAS_NAMES = ("font1", "font2", "font3")
ATLAS_FONT_IDS = {"font1": 0, "font2": 2, "font3": 1}
SOURCE_ATLAS_EXTENT = 512
SOURCE_LAYOUT_RESOLUTION = (512, 488)
SOURCE_CELL_WIDTH = 32
SOURCE_ROW_PITCH = 40
OUTPUT_SIZE = 2048
OUTPUT_SCALE = OUTPUT_SIZE // SOURCE_ATLAS_EXTENT
COLUMN_COUNT = SOURCE_ATLAS_EXTENT // SOURCE_CELL_WIDTH
CELL_WIDTH = SOURCE_CELL_WIDTH * OUTPUT_SCALE
ROW_PITCH = SOURCE_ROW_PITCH * OUTPUT_SCALE
DEFAULT_SAFE_GUTTER_X = OUTPUT_SCALE
MIN_SAFE_GUTTER_X = 1
SAFE_GUTTER_Y = OUTPUT_SCALE

# sub_821F5028 initializes 512.0, 40.0, 4.0, and 39.5. sub_821F1EE0
# applies the remaining normalized offsets when constructing glyph UVs.
UV_ROW_PHASE = 0.045000002
UV_BOTTOM_SUBTRACT = 0.001
UV_BOTTOM_ADD = 0.0048000002
UV_TOP_SOURCE = 4.0
UV_BOTTOM_SOURCE = 39.5

REFERENCE_CHARACTERS = frozenset("HIOEFTXZ0123456789")
BASE_FACE_CHARACTERS = frozenset(
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
)
REVIEW_CHARACTERS = frozenset(
    "WwiljtgpqymR0123456789.,:;!?%&@-+_ÀÁÂÄÇÈÉÊËÌÍÎÏÑÒÓÔÖÙÚÛÜ"
)
PARSED_FONT_SECTIONS = frozenset(
    {
        "MAP",
        "PROP",
        "UNPROP",
        "MAINFONT",
        "SUBFONT_1",
        "SUBFONT_2",
        "COMMON_FONT",
        "SPACE_BETWEEN_CHARS",
        "WHITESPACE",
    }
)


@dataclass(frozen=True)
class FontDefinition:
    font_id: int
    mapping: tuple[int, ...]
    proportional_trim: tuple[int, ...]
    source_proportional_count: int
    unproportional_width: int
    mainfont: tuple[int, int]
    subfont_1: tuple[int, int]
    subfont_2: tuple[int, int]
    common_font: tuple[int, int]
    space_between_chars: tuple[int, ...]
    whitespace: int

    def range_for(self, section: str) -> tuple[int, int]:
        return {
            "MAINFONT": self.mainfont,
            "SUBFONT_1": self.subfont_1,
            "SUBFONT_2": self.subfont_2,
            "COMMON_FONT": self.common_font,
        }[section]

    def proportional_width(self, index: int) -> int:
        # sub_821F3578 executes `subfic value,value,32` before storing PROP.
        return SOURCE_CELL_WIDTH - self.proportional_trim[index]


@dataclass(frozen=True)
class PixelBox:
    left: int
    top: int
    right: int
    bottom: int

    @property
    def width(self) -> int:
        return self.right - self.left

    @property
    def height(self) -> int:
        return self.bottom - self.top

    def relative_to(self, origin_x: int, origin_y: int) -> "PixelBox":
        return PixelBox(
            self.left - origin_x,
            self.top - origin_y,
            self.right - origin_x,
            self.bottom - origin_y,
        )

    def inset(self, amount: int) -> "PixelBox":
        return PixelBox(
            self.left + amount,
            self.top + amount,
            self.right - amount,
            self.bottom - amount,
        )

    def inset_xy(self, horizontal: int, vertical: int) -> "PixelBox":
        return PixelBox(
            self.left + horizontal,
            self.top + vertical,
            self.right - horizontal,
            self.bottom - vertical,
        )

    def contains(self, bounds: tuple[int, int, int, int]) -> bool:
        return (
            bounds[0] >= self.left
            and bounds[1] >= self.top
            and bounds[2] <= self.right
            and bounds[3] <= self.bottom
        )


@dataclass(frozen=True)
class GlyphRecord:
    index: int
    character: str
    sample_box: PixelBox
    safe_box: PixelBox


@dataclass(frozen=True)
class FaceTransform:
    point_size: int
    baseline: int
    horizontal_scale: float
    reference_height: float
    reference_count: int


@dataclass(frozen=True)
class FaceRange:
    atlas: str
    first: int
    last_exclusive: int
    role: str
    font_key: str
    range_section: str
    horizontal_gutter: int = DEFAULT_SAFE_GUTTER_X
    calibrate_sampled_width: bool = False
    fit_base_characters: bool = False
    allow_traced_fallback: bool = False
    fixed_transform: FaceTransform | None = None


# Licensed ranges are resolved against each title's parsed FONTS.DAT. Ambiguous
# gaps and COMMON_FONT intentionally stay traced stock.
PROFILE_FACE_RANGES = {
    "gta4": (
    FaceRange(
        "font1",
        0,
        134,
        "HELECOND",
        "helvetica_condensed_medium",
        "MAINFONT",
        horizontal_gutter=MIN_SAFE_GUTTER_X,
        calibrate_sampled_width=True,
        fit_base_characters=True,
        allow_traced_fallback=True,
    ),
    FaceRange("font1", 134, 150, "PRICEDOWN", "pricedown", "SUBFONT_1"),
    FaceRange("font2", 0, 134, "HELE BLACK", "helvetica_heavy", "MAINFONT"),
    FaceRange(
        "font2", 135, 195, "HELE ROMAN", "helvetica_roman", "SUBFONT_1"
    ),
    ),
    "tlad": (
        FaceRange(
            "font1",
            0,
            134,
            "HELECOND",
            "helvetica_condensed_medium",
            "MAINFONT",
            horizontal_gutter=MIN_SAFE_GUTTER_X,
            calibrate_sampled_width=True,
            fit_base_characters=True,
            allow_traced_fallback=True,
            fixed_transform=FaceTransform(152, 126, 0.9333333333333333, 110.0, 18),
        ),
        FaceRange(
            "font1", 134, 150, "PRICEDOWN", "pricedown", "SUBFONT_1",
            fixed_transform=FaceTransform(161, 143, 1.0, 122.0, 11),
        ),
        FaceRange(
            "font2", 0, 134, "HELVETICA HEAVY", "helvetica_heavy", "MAINFONT",
            fixed_transform=FaceTransform(112, 127, 1.0, 98.0, 17),
        ),
        FaceRange(
            "font2", 135, 195, "HELVETICA ROMAN", "helvetica_roman", "SUBFONT_1",
            fixed_transform=FaceTransform(102, 131, 1.0, 101.0, 16),
        ),
        FaceRange(
            "font3",
            93,
            206,
            "MESQUITE STD",
            "mesquite",
            "SUBFONT_2",
            calibrate_sampled_width=True,
            fit_base_characters=True,
            allow_traced_fallback=True,
        ),
    ),
    "tbogt": (
        FaceRange(
            "font1",
            0,
            134,
            "HELECOND",
            "helvetica_condensed_medium",
            "MAINFONT",
            horizontal_gutter=MIN_SAFE_GUTTER_X,
            calibrate_sampled_width=True,
            fit_base_characters=True,
            allow_traced_fallback=True,
            fixed_transform=FaceTransform(152, 126, 0.9333333333333333, 110.0, 18),
        ),
        FaceRange(
            "font1", 134, 150, "PRICEDOWN", "pricedown", "SUBFONT_1",
            fixed_transform=FaceTransform(161, 143, 1.0, 122.0, 11),
        ),
        FaceRange(
            "font2", 0, 134, "HELVETICA HEAVY", "helvetica_heavy", "MAINFONT",
            fixed_transform=FaceTransform(112, 127, 1.0, 98.0, 17),
        ),
        FaceRange(
            "font2", 135, 195, "HELVETICA ROMAN", "helvetica_roman", "SUBFONT_1",
            fixed_transform=FaceTransform(102, 131, 1.0, 101.0, 16),
        ),
        FaceRange(
            "font3",
            92,
            179,
            "DIN BOLD",
            "din_bold",
            "SUBFONT_2",
            calibrate_sampled_width=True,
            fit_base_characters=True,
            allow_traced_fallback=True,
        ),
    ),
}

PROFILE_SHARED_ATLASES = {
    "gta4": (),
    "tlad": ("font1", "font2"),
    "tbogt": ("font1",),
}


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Rebuild GTA IV's font1/font2/font3 atlases at 2048x2048"
    )
    parser.add_argument("--profile", choices=tuple(PROFILE_FACE_RANGES), required=True)
    parser.add_argument("--fonts-dat", type=Path, required=True)
    parser.add_argument("--font-pack", type=Path, action="append", required=True)
    parser.add_argument("--traced-root", type=Path, required=True)
    parser.add_argument("--shared-root", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--archive-traced", type=Path)
    return parser.parse_args()


def extract_font_pack(font_pack: Path, destination: Path) -> None:
    for source in sorted(font_pack.rglob("*")):
        if source.is_file() and source.suffix.lower() in {".otf", ".ttf"}:
            relative = source.relative_to(font_pack)
            output = destination / relative
            output.parent.mkdir(parents=True, exist_ok=True)
            output.write_bytes(source.read_bytes())
        elif source.is_file() and source.suffix.lower() == ".zip":
            with zipfile.ZipFile(source) as archive:
                archive.extractall(destination / source.relative_to(font_pack).with_suffix(""))


def find_unique(root: Path, filename: str) -> Path:
    matches = sorted(path for path in root.rglob(filename) if path.is_file())
    if len(matches) != 1:
        raise RuntimeError(
            f"expected exactly one {filename!r} beneath {root}, found {len(matches)}"
        )
    return matches[0]


def find_preferred(root: Path, filename: str) -> Path:
    matches = sorted(path for path in root.rglob(filename) if path.is_file())
    if not matches:
        raise RuntimeError(f"could not find {filename!r} beneath {root}")
    return matches[-1]


def locate_fonts(extracted: Path, required_keys: set[str]) -> dict[str, Path]:
    filenames = {
        "helvetica_condensed_medium": "Neue Helvetica 67 Condensed Medium.otf",
        "helvetica_heavy": "HelveticaNeue-Heavy.otf",
        "helvetica_roman": "Neue Helvetica 55 Roman.otf",
        "pricedown": "Pricedown Bl.otf",
        "mesquite": "MesquiteStd.otf",
        "din_bold": "DIN Bold.otf",
        "din_mittelschrift": "din1451alt.ttf",
    }
    return {key: find_preferred(extracted, filenames[key]) for key in required_keys}


def strip_fonts_dat_line(raw_line: str) -> str:
    return raw_line.split("#", 1)[0].split("//", 1)[0].strip()


def require_single_value(font_id: int, section: str, values: list[int]) -> int:
    if len(values) != 1:
        raise RuntimeError(
            f"font ID {font_id} [{section}] has {len(values)} values, expected one"
        )
    return values[0]


def require_range(
    font_id: int, section: str, values: list[int]
) -> tuple[int, int]:
    if len(values) != 2:
        raise RuntimeError(
            f"font ID {font_id} [{section}] has {len(values)} values, expected two"
        )
    first, last_exclusive = values
    if first < 0 or last_exclusive < first:
        raise RuntimeError(
            f"font ID {font_id} [{section}] has invalid range {values}"
        )
    return first, last_exclusive


def parse_fonts_dat(
    fonts_dat: Path,
) -> tuple[tuple[int, int], dict[str, FontDefinition]]:
    sections_by_id: dict[int, dict[str, list[int]]] = {}
    resolution_values: list[int] = []
    active_font_id: int | None = None
    active_section: str | None = None

    for raw_line in fonts_dat.read_text(encoding="cp1252").splitlines():
        line = strip_fonts_dat_line(raw_line)
        if not line:
            continue
        if line.startswith("[") and line.endswith("]"):
            section = line[1:-1]
            active_section = None if section.startswith("/") else section
            continue
        if active_section == "FONT_ID":
            active_font_id = int(line, 10)
            sections_by_id.setdefault(active_font_id, {})
            continue
        if active_section == "RESOLUTION":
            resolution_values.extend(
                int(component.strip(), 10) for component in line.split(",")
            )
            continue
        if active_font_id is None or active_section not in PARSED_FONT_SECTIONS:
            continue
        sections_by_id[active_font_id].setdefault(active_section, []).extend(
            int(value, 10) for value in line.replace(",", " ").split()
        )

    if len(resolution_values) != 2:
        raise RuntimeError(
            f"fonts.dat resolution has {len(resolution_values)} values, expected two"
        )
    resolution = resolution_values[0], resolution_values[1]
    expected_ids = set(ATLAS_FONT_IDS.values())
    if set(sections_by_id) != expected_ids:
        raise RuntimeError(
            f"fonts.dat IDs are {sorted(sections_by_id)}, expected {sorted(expected_ids)}"
        )

    definitions_by_id: dict[int, FontDefinition] = {}
    for font_id, sections in sections_by_id.items():
        missing = PARSED_FONT_SECTIONS.difference(sections)
        if missing:
            raise RuntimeError(f"font ID {font_id} is missing {sorted(missing)}")
        mapping = tuple(sections["MAP"])
        proportional_values = tuple(sections["PROP"])
        if not mapping:
            raise RuntimeError(f"font ID {font_id} has an empty [MAP]")
        if len(proportional_values) < len(mapping):
            raise RuntimeError(
                f"font ID {font_id} has {len(mapping)} map entries but "
                f"only {len(proportional_values)} PROP entries"
            )
        # TLAD's retail FONT3 table declares and supplies nine more PROP values
        # than it maps. The game stores both tables independently and only a
        # mapped cell can be addressed, so retain the mapped prefix and record
        # the ignored tail for the manifest.
        proportional_trim = proportional_values[: len(mapping)]
        definitions_by_id[font_id] = FontDefinition(
            font_id=font_id,
            mapping=mapping,
            proportional_trim=proportional_trim,
            source_proportional_count=len(proportional_values),
            unproportional_width=require_single_value(
                font_id, "UNPROP", sections["UNPROP"]
            ),
            mainfont=require_range(font_id, "MAINFONT", sections["MAINFONT"]),
            subfont_1=require_range(font_id, "SUBFONT_1", sections["SUBFONT_1"]),
            subfont_2=require_range(font_id, "SUBFONT_2", sections["SUBFONT_2"]),
            common_font=require_range(
                font_id, "COMMON_FONT", sections["COMMON_FONT"]
            ),
            space_between_chars=tuple(sections["SPACE_BETWEEN_CHARS"]),
            whitespace=require_single_value(
                font_id, "WHITESPACE", sections["WHITESPACE"]
            ),
        )
    return resolution, {
        atlas_name: definitions_by_id[font_id]
        for atlas_name, font_id in ATLAS_FONT_IDS.items()
    }


def decode_character(code: int) -> str | None:
    try:
        character = bytes((code,)).decode("cp1252")
    except UnicodeDecodeError:
        return None
    return character if character.isprintable() else None


def validate_layout() -> None:
    if OUTPUT_SIZE % SOURCE_ATLAS_EXTENT != 0:
        raise RuntimeError("output extent is not an integer source-atlas multiple")
    if SOURCE_ATLAS_EXTENT % SOURCE_CELL_WIDTH != 0:
        raise RuntimeError("source atlas is not divisible by the cell width")


def cell_box(index: int) -> PixelBox:
    x = (index % COLUMN_COUNT) * CELL_WIDTH
    y = (index // COLUMN_COUNT) * ROW_PITCH
    if y >= OUTPUT_SIZE:
        raise RuntimeError(
            f"cell {index} begins at y={y}, outside the {OUTPUT_SIZE}px atlas"
        )
    return PixelBox(
        x,
        y,
        min(x + CELL_WIDTH, OUTPUT_SIZE),
        min(y + ROW_PITCH, OUTPUT_SIZE),
    )


def renderer_sample_box(index: int, proportional_width: int) -> PixelBox:
    if proportional_width <= 0 or proportional_width > SOURCE_CELL_WIDTH:
        raise RuntimeError(
            f"cell {index} has invalid runtime proportional width {proportional_width}"
        )
    cell = cell_box(index)
    row = index // COLUMN_COUNT
    row_offset_source = (row - UV_ROW_PHASE) * SOURCE_ROW_PITCH
    top_uv = (UV_TOP_SOURCE + row_offset_source) / SOURCE_ATLAS_EXTENT
    bottom_uv = (
        (UV_BOTTOM_SOURCE + row_offset_source) / SOURCE_ATLAS_EXTENT
        - UV_BOTTOM_SUBTRACT
        + UV_BOTTOM_ADD
    )
    sample = PixelBox(
        cell.left,
        math.ceil(top_uv * OUTPUT_SIZE),
        cell.left + proportional_width * OUTPUT_SCALE,
        math.floor(bottom_uv * OUTPUT_SIZE),
    )
    if (
        sample.left < cell.left
        or sample.top < cell.top
        or sample.right > cell.right
        or sample.width <= MIN_SAFE_GUTTER_X * 2
        or sample.height <= SAFE_GUTTER_Y * 2
    ):
        raise RuntimeError(
            f"cell {index} runtime sample box {sample} is invalid for cell {cell}"
        )
    return sample


def resolve_face_ranges(
    profile: str, definitions: dict[str, FontDefinition]
) -> tuple[FaceRange, ...]:
    resolved: list[FaceRange] = []
    for configured in PROFILE_FACE_RANGES[profile]:
        definition = definitions[configured.atlas]
        parsed_first, parsed_last = definition.range_for(configured.range_section)
        if parsed_first != configured.first or parsed_last != configured.last_exclusive:
            raise RuntimeError(
                f"{configured.atlas} {configured.role} range is "
                f"{(parsed_first, parsed_last)}, expected "
                f"{(configured.first, configured.last_exclusive)}"
            )
        resolved.append(
            FaceRange(
                atlas=configured.atlas,
                first=parsed_first,
                last_exclusive=min(parsed_last, len(definition.mapping)),
                role=configured.role,
                font_key=configured.font_key,
                range_section=configured.range_section,
                horizontal_gutter=configured.horizontal_gutter,
                calibrate_sampled_width=configured.calibrate_sampled_width,
                fit_base_characters=configured.fit_base_characters,
                allow_traced_fallback=configured.allow_traced_fallback,
                fixed_transform=configured.fixed_transform,
            )
        )
    return tuple(resolved)


def validate_font_definitions(
    definitions: dict[str, FontDefinition], face_ranges: tuple[FaceRange, ...]
) -> None:
    for definition in definitions.values():
        cell_box(len(definition.mapping) - 1)
        for index in range(len(definition.mapping)):
            renderer_sample_box(index, definition.proportional_width(index))
    for face_range in face_ranges:
        definition = definitions[face_range.atlas]
        if face_range.last_exclusive > len(definition.mapping):
            raise RuntimeError(
                f"{face_range.atlas} mapping cannot cover "
                f"{(face_range.first, face_range.last_exclusive)}"
            )


def face_records(
    face_range: FaceRange, definition: FontDefinition
) -> tuple[list[GlyphRecord], list[int]]:
    records: list[GlyphRecord] = []
    preserved_edge_cells: list[int] = []
    for index in range(face_range.first, face_range.last_exclusive):
        character = decode_character(definition.mapping[index])
        if character is None:
            raise RuntimeError(
                f"{face_range.atlas} cell {index} has no printable CP1252 character"
            )
        sample = renderer_sample_box(index, definition.proportional_width(index))
        # The stock font2 SUBFONT_1 declaration includes three characters whose
        # compiled V coordinates extend past the physical texture. The native
        # sampler clamps them to the bottom edge, so replacing their cells cannot
        # produce an unclipped result. Preserve those traced cells exactly.
        if sample.bottom > OUTPUT_SIZE:
            preserved_edge_cells.append(index)
            continue
        safe = sample.inset_xy(face_range.horizontal_gutter, SAFE_GUTTER_Y)
        if safe.width <= 0 or safe.height <= 0:
            raise RuntimeError(
                f"{face_range.atlas} cell {index} has empty safe box {safe}"
            )
        records.append(GlyphRecord(index, character, sample, safe))
    return records, preserved_edge_cells


def traced_reference_metrics(
    traced: Image.Image, records: list[GlyphRecord]
) -> tuple[float, int, int]:
    alpha = traced.getchannel("A")
    heights: list[int] = []
    bottoms: list[int] = []
    for record in records:
        if record.character not in REFERENCE_CHARACTERS:
            continue
        cell = cell_box(record.index)
        bounds = alpha.crop(
            (cell.left, cell.top, cell.right, cell.bottom)
        ).getbbox()
        if bounds is None:
            continue
        if (
            bounds[0] == 0
            or bounds[1] == 0
            or bounds[2] == cell.width
            or bounds[3] == cell.height
        ):
            continue
        heights.append(bounds[3] - bounds[1])
        bottoms.append(bounds[3])
    if not heights:
        raise RuntimeError("face has no uncontaminated traced reference glyphs")
    return (
        float(statistics.median(heights)),
        round(statistics.median(bottoms)),
        len(heights),
    )


def vector_reference_height(
    font: ImageFont.FreeTypeFont, records: list[GlyphRecord]
) -> float:
    heights: list[float] = []
    seen: set[str] = set()
    for record in records:
        if record.character not in REFERENCE_CHARACTERS or record.character in seen:
            continue
        seen.add(record.character)
        bounds = glyph_ink_bounds(font, record.character)
        if bounds is not None:
            heights.append(float(bounds[3] - bounds[1]))
    if not heights:
        raise RuntimeError("licensed face has no vector reference glyphs")
    return float(statistics.median(heights))


def local_safe_box(record: GlyphRecord) -> PixelBox:
    cell = cell_box(record.index)
    return record.safe_box.relative_to(cell.left, cell.top)


def glyph_ink_bounds(
    font: ImageFont.FreeTypeFont, character: str
) -> tuple[int, int, int, int] | None:
    mask, offset = font.getmask2(character, mode="L", anchor="ls")
    mask_bounds = mask.getbbox()
    if mask_bounds is None:
        return None
    return (
        offset[0] + mask_bounds[0],
        offset[1] + mask_bounds[1],
        offset[0] + mask_bounds[2],
        offset[1] + mask_bounds[3],
    )


def glyph_origin_x(
    font: ImageFont.FreeTypeFont,
    record: GlyphRecord,
    horizontal_scale: float,
) -> float:
    safe = local_safe_box(record)
    advance = float(font.getlength(record.character)) * horizontal_scale
    bounds = glyph_ink_bounds(font, record.character)
    if bounds is None:
        raise RuntimeError(f"glyph {record.character!r} has no vector bounds")
    ideal_origin = safe.left + (safe.width - advance) / 2.0
    minimum_origin = safe.left - bounds[0] * horizontal_scale
    maximum_origin = safe.right - bounds[2] * horizontal_scale
    if minimum_origin > maximum_origin:
        return ideal_origin
    # Keep the font's natural advance-box placement whenever possible. If the
    # replacement face has different bearings than Rockstar's source face,
    # translate only far enough to keep the outline inside the sampled region.
    return min(max(ideal_origin, minimum_origin), maximum_origin)


def face_fits(
    font: ImageFont.FreeTypeFont,
    baseline: int,
    horizontal_scale: float,
    records: list[GlyphRecord],
) -> bool:
    for record in records:
        safe = local_safe_box(record)
        advance = float(font.getlength(record.character)) * horizontal_scale
        # The game's PROP table is the authoritative character advance. The
        # FreeType advance is used only to center the face's advance box while
        # retaining its bearings; it is not constrained to the sampled ink box.
        if advance <= 0.0:
            return False
        bounds = glyph_ink_bounds(font, record.character)
        if bounds is None:
            return False
        origin_x = glyph_origin_x(font, record, horizontal_scale)
        placed = (
            math.floor(origin_x + bounds[0] * horizontal_scale),
            math.floor(baseline + bounds[1]),
            math.ceil(origin_x + bounds[2] * horizontal_scale),
            math.ceil(baseline + bounds[3]),
        )
        if not safe.contains(placed):
            return False
    return True


def first_face_fit_failure(
    font: ImageFont.FreeTypeFont,
    baseline: int,
    horizontal_scale: float,
    records: list[GlyphRecord],
) -> str | None:
    for record in records:
        safe = local_safe_box(record)
        advance = float(font.getlength(record.character)) * horizontal_scale
        bounds = glyph_ink_bounds(font, record.character)
        if bounds is None or advance <= 0.0:
            return f"cell {record.index} {record.character!r}: missing metrics"
        origin_x = glyph_origin_x(font, record, horizontal_scale)
        placed = (
            math.floor(origin_x + bounds[0] * horizontal_scale),
            math.floor(baseline + bounds[1]),
            math.ceil(origin_x + bounds[2] * horizontal_scale),
            math.ceil(baseline + bounds[3]),
        )
        if not safe.contains(placed):
            return (
                f"cell {record.index} {record.character!r}: alpha={placed} "
                f"safe={safe} advance={advance:.3f} bounds={bounds}"
            )
    return None


def fitted_face_baseline(
    font: ImageFont.FreeTypeFont,
    preferred_baseline: int,
    records: list[GlyphRecord],
) -> int | None:
    minimum_baseline = 0
    maximum_baseline = ROW_PITCH
    for record in records:
        safe = local_safe_box(record)
        bounds = glyph_ink_bounds(font, record.character)
        if bounds is None:
            return None
        minimum_baseline = max(minimum_baseline, safe.top - bounds[1])
        maximum_baseline = min(maximum_baseline, safe.bottom - bounds[3])
    if minimum_baseline > maximum_baseline:
        return None
    return min(max(preferred_baseline, minimum_baseline), maximum_baseline)


def traced_sample_width_scale(
    traced: Image.Image,
    font: ImageFont.FreeTypeFont,
    records: list[GlyphRecord],
) -> float:
    alpha = traced.getchannel("A")
    factors: list[float] = []
    for record in records:
        sample = record.sample_box
        traced_bounds = alpha.crop(
            (sample.left, sample.top, sample.right, sample.bottom)
        ).getbbox()
        vector_bounds = glyph_ink_bounds(font, record.character)
        if traced_bounds is None or vector_bounds is None:
            continue
        traced_width = traced_bounds[2] - traced_bounds[0]
        vector_width = vector_bounds[2] - vector_bounds[0]
        if traced_width > 0 and vector_width > 0:
            factors.append(traced_width / vector_width)
    if not factors:
        raise RuntimeError("face has no sampled width calibration glyphs")
    return float(statistics.median(factors))


def maximum_horizontal_scale(
    font: ImageFont.FreeTypeFont, records: list[GlyphRecord]
) -> float:
    limits: list[float] = []
    for record in records:
        bounds = glyph_ink_bounds(font, record.character)
        if bounds is None:
            continue
        ink_width = bounds[2] - bounds[0]
        if ink_width > 0:
            limits.append(local_safe_box(record).width / ink_width)
    if not limits:
        raise RuntimeError("face has no horizontal fit limits")
    return min(limits)


def derive_face_transform(
    face_range: FaceRange,
    font_path: Path,
    traced: Image.Image,
    records: list[GlyphRecord],
) -> FaceTransform:
    target_height, baseline, reference_count = traced_reference_metrics(
        traced, records
    )
    candidates: list[tuple[float, int]] = []
    for point_size in range(1, ROW_PITCH * 2 + 1):
        font = ImageFont.truetype(str(font_path), point_size)
        height = vector_reference_height(font, records)
        candidates.append((abs(height - target_height), point_size))
    nominal_size = min(candidates)[1]
    fit_records = (
        [
            record
            for record in records
            if record.character in BASE_FACE_CHARACTERS
        ]
        if face_range.fit_base_characters
        else records
    )
    if not fit_records:
        raise RuntimeError(f"{face_range.role} has no transform-fit glyphs")
    nominal_font = ImageFont.truetype(str(font_path), nominal_size)
    nominal_scale = (
        min(
            traced_sample_width_scale(traced, nominal_font, fit_records),
            maximum_horizontal_scale(nominal_font, fit_records),
        )
        if face_range.calibrate_sampled_width
        else 1.0
    )
    nominal_baseline = fitted_face_baseline(
        nominal_font, baseline, fit_records
    )
    nominal_failure = (
        "no shared baseline fits every glyph"
        if nominal_baseline is None
        else first_face_fit_failure(
            nominal_font, nominal_baseline, nominal_scale, fit_records
        )
    )
    if nominal_failure is not None:
        print(
            f"{font_path.name}: nominal size {nominal_size} rejected: "
            f"{nominal_failure}"
        )
    for point_size in range(nominal_size, 0, -1):
        font = ImageFont.truetype(str(font_path), point_size)
        horizontal_scale = (
            min(
                traced_sample_width_scale(traced, font, fit_records),
                maximum_horizontal_scale(font, fit_records),
            )
            if face_range.calibrate_sampled_width
            else 1.0
        )
        fitted_baseline = fitted_face_baseline(font, baseline, fit_records)
        if fitted_baseline is not None and face_fits(
            font, fitted_baseline, horizontal_scale, fit_records
        ):
            if point_size < nominal_size:
                limiting_font = ImageFont.truetype(
                    str(font_path), point_size + 1
                )
                limiting_baseline = fitted_face_baseline(
                    limiting_font, baseline, fit_records
                )
                limiting_scale = (
                    min(
                        traced_sample_width_scale(
                            traced, limiting_font, fit_records
                        ),
                        maximum_horizontal_scale(
                            limiting_font, fit_records
                        ),
                    )
                    if face_range.calibrate_sampled_width
                    else 1.0
                )
                limiting_failure = (
                    "no shared baseline fits every glyph"
                    if limiting_baseline is None
                    else first_face_fit_failure(
                        limiting_font,
                        limiting_baseline,
                        limiting_scale,
                        fit_records,
                    )
                )
                print(
                    f"{font_path.name}: limiting failure above selected size: "
                    f"{limiting_failure}"
                )
            print(
                f"{font_path.name}: selected size {point_size}, baseline "
                f"{fitted_baseline}, horizontal scale {horizontal_scale:.6f}, "
                f"target reference height {target_height}"
            )
            return FaceTransform(
                point_size=point_size,
                baseline=fitted_baseline,
                horizontal_scale=horizontal_scale,
                reference_height=target_height,
                reference_count=reference_count,
            )
    raise RuntimeError(f"could not fit licensed face {font_path.name}")


def render_face(
    atlas: Image.Image,
    font_path: Path,
    transform: FaceTransform,
    records: list[GlyphRecord],
    allow_traced_fallback: bool,
) -> tuple[list[GlyphRecord], list[GlyphRecord]]:
    font = ImageFont.truetype(str(font_path), transform.point_size)
    redrawn: list[GlyphRecord] = []
    fallback: list[GlyphRecord] = []
    for record in records:
        cell = cell_box(record.index)
        alpha = Image.new("L", (cell.width, cell.height), 0)
        mask, _ = font.getmask2(record.character, mode="L", anchor="ls")
        mask_image = Image.frombytes("L", mask.size, bytes(mask))
        mask_bounds = mask_image.getbbox()
        ink_bounds = glyph_ink_bounds(font, record.character)
        if mask_bounds is None or ink_bounds is None:
            raise RuntimeError(
                f"{font_path.name} produced an empty glyph for {record.character!r}"
            )
        ink = mask_image.crop(mask_bounds)
        scaled_width = max(
            1, round(ink.width * transform.horizontal_scale)
        )
        if scaled_width != ink.width:
            ink = ink.resize((scaled_width, ink.height), Image.Resampling.LANCZOS)
        origin_x = glyph_origin_x(
            font, record, transform.horizontal_scale
        )
        paste_x = round(
            origin_x + ink_bounds[0] * transform.horizontal_scale
        )
        paste_y = transform.baseline + ink_bounds[1]
        alpha.paste(ink, (paste_x, paste_y))
        bounds = alpha.getbbox()
        if bounds is None:
            raise RuntimeError(
                f"{font_path.name} produced an empty glyph for {record.character!r}"
            )
        safe = local_safe_box(record)
        if not safe.contains(bounds):
            if allow_traced_fallback:
                fallback.append(record)
                continue
            raise RuntimeError(
                f"{font_path.name} glyph {record.character!r} cell {record.index} "
                f"has alpha {bounds} outside {safe}"
            )
        clean_cell = Image.new("RGBA", alpha.size, (255, 255, 255, 0))
        clean_cell.putalpha(alpha)
        atlas.paste(clean_cell, (cell.left, cell.top, cell.right, cell.bottom))
        redrawn.append(record)
    return redrawn, fallback


def verify_redrawn_geometry(
    atlas_name: str,
    rebuilt: Image.Image,
    face_outputs: list[tuple[FaceRange, list[GlyphRecord], FaceTransform]],
) -> None:
    alpha = rebuilt.getchannel("A")
    for face_range, records, transform in face_outputs:
        if (
            transform.point_size <= 0
            or transform.baseline <= 0
            or transform.horizontal_scale <= 0.0
        ):
            raise RuntimeError(f"{face_range.role} has invalid transform {transform}")
        for record in records:
            cell = cell_box(record.index)
            bounds = alpha.crop(
                (cell.left, cell.top, cell.right, cell.bottom)
            ).getbbox()
            if bounds is None:
                raise RuntimeError(
                    f"{atlas_name} cell {record.index} has no redrawn alpha"
                )
            safe = local_safe_box(record)
            if not safe.contains(bounds):
                raise RuntimeError(
                    f"{atlas_name} cell {record.index} alpha {bounds} escapes {safe}"
                )
            if (
                bounds[0] == 0
                or bounds[1] == 0
                or bounds[2] == cell.width
                or bounds[3] == cell.height
            ):
                raise RuntimeError(
                    f"{atlas_name} cell {record.index} touches a nominal edge"
                )


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as file:
        for block in iter(lambda: file.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def build_review(traced: Image.Image, rebuilt: Image.Image, output: Path) -> None:
    preview_size = OUTPUT_SIZE // 4
    left = traced.resize((preview_size, preview_size), Image.Resampling.LANCZOS)
    right = rebuilt.resize((preview_size, preview_size), Image.Resampling.LANCZOS)
    review = Image.new("RGBA", (preview_size * 2, preview_size), (0, 0, 0, 255))
    review.alpha_composite(left, (0, 0))
    review.alpha_composite(right, (preview_size, 0))
    review.save(output, compress_level=9)


def build_sampling_review(
    traced: Image.Image,
    rebuilt: Image.Image,
    face_outputs: list[tuple[FaceRange, list[GlyphRecord], FaceTransform]],
    output: Path,
) -> None:
    selected = [
        record
        for _, records, _ in face_outputs
        for record in records
        if record.character in REVIEW_CHARACTERS
    ]
    if not selected:
        return
    tile_width = 224
    tile_height = 192
    label_height = 24
    columns = 6
    rows = math.ceil(len(selected) / columns)
    sheet = Image.new(
        "RGBA", (columns * tile_width, rows * tile_height), (32, 32, 32, 255)
    )
    labels = ImageDraw.Draw(sheet)
    for output_index, record in enumerate(selected):
        sample = record.sample_box
        traced_crop = traced.crop(
            (sample.left, sample.top, sample.right, sample.bottom)
        )
        rebuilt_crop = rebuilt.crop(
            (sample.left, sample.top, sample.right, sample.bottom)
        )
        preview_width = tile_width // 2
        preview_height = tile_height - label_height
        traced_crop.thumbnail((preview_width, preview_height), Image.Resampling.NEAREST)
        rebuilt_crop.thumbnail((preview_width, preview_height), Image.Resampling.NEAREST)
        x = (output_index % columns) * tile_width
        y = (output_index // columns) * tile_height
        labels.text(
            (x, y),
            f"{record.index}: {record.character!r} traced | rebuilt",
            fill="white",
        )
        sheet.alpha_composite(traced_crop, (x, y + label_height))
        sheet.alpha_composite(rebuilt_crop, (x + preview_width, y + label_height))
    sheet.save(output, compress_level=9)


def verify_unmodified_cells(
    atlas_name: str,
    traced: Image.Image,
    rebuilt: Image.Image,
    face_outputs: list[tuple[FaceRange, list[GlyphRecord], FaceTransform]],
) -> None:
    difference = ImageChops.difference(traced, rebuilt)
    for _, records, _ in face_outputs:
        for record in records:
            box = cell_box(record.index)
            difference.paste((0, 0, 0, 0), (box.left, box.top, box.right, box.bottom))
    if difference.getbbox() is not None:
        raise RuntimeError(f"{atlas_name} changed pixels outside redrawn cells")


def main() -> None:
    arguments = parse_arguments()
    validate_layout()
    arguments.output.mkdir(parents=True, exist_ok=True)
    review_root = arguments.output / "review"
    review_root.mkdir(parents=True, exist_ok=True)
    resolution, definitions = parse_fonts_dat(arguments.fonts_dat)
    if resolution != SOURCE_LAYOUT_RESOLUTION:
        raise RuntimeError(
            f"fonts.dat resolution is {resolution}, expected {SOURCE_LAYOUT_RESOLUTION}"
        )
    face_ranges = resolve_face_ranges(arguments.profile, definitions)
    validate_font_definitions(definitions, face_ranges)
    shared_atlases = PROFILE_SHARED_ATLASES[arguments.profile]
    shared_manifest: dict[str, object] | None = None
    if shared_atlases:
        if arguments.shared_root is None:
            raise RuntimeError(
                f"profile {arguments.profile} requires --shared-root for {shared_atlases}"
            )
        shared_manifest = json.loads(
            (arguments.shared_root / "manifest.json").read_text(encoding="utf-8")
        )

    with tempfile.TemporaryDirectory(prefix="gta4-licensed-fonts-") as directory:
        extracted = Path(directory)
        for pack_index, font_pack in enumerate(arguments.font_pack):
            extract_font_pack(font_pack, extracted / f"pack{pack_index}")
        required_font_keys = {face_range.font_key for face_range in face_ranges}
        fonts = locate_fonts(extracted, required_font_keys)
        manifest: dict[str, object] = {
            "format": 4,
            "profile": arguments.profile,
            "atlas_extent": OUTPUT_SIZE,
            "grid_extent": COLUMN_COUNT,
            "fit_model": {
                "name": "runtime-sample-safe-uniform-face",
                "safe_gutter_output_pixels": {
                    "horizontal_default": DEFAULT_SAFE_GUTTER_X,
                    "vertical": SAFE_GUTTER_Y,
                    "font1_HELECOND_horizontal": MIN_SAFE_GUTTER_X,
                },
                "horizontal_metrics": "FONTS.DAT PROP trim with vector bearings",
                "vertical_metrics": "sub_821F5028/sub_821F1EE0 sample window",
            },
            "layout": {
                "source_atlas_extent": SOURCE_ATLAS_EXTENT,
                "source_layout_resolution": list(SOURCE_LAYOUT_RESOLUTION),
                "source_cell_width": SOURCE_CELL_WIDTH,
                "source_row_pitch": SOURCE_ROW_PITCH,
                "columns": COLUMN_COUNT,
                "output_cell_width": CELL_WIDTH,
                "output_row_pitch": ROW_PITCH,
            },
            "source": "licensed vector faces fitted to compiled GTA IV sampling metrics",
            "faces": {key: path.name for key, path in fonts.items()},
            "atlases": {},
        }

        for atlas_name in ATLAS_NAMES:
            traced_path = arguments.traced_root / f"{atlas_name}.png"
            traced = Image.open(traced_path).convert("RGBA")
            if traced.size != (OUTPUT_SIZE, OUTPUT_SIZE):
                raise RuntimeError(
                    f"{traced_path} is {traced.size}, expected {OUTPUT_SIZE}x{OUTPUT_SIZE}"
                )
            if arguments.archive_traced is not None:
                arguments.archive_traced.mkdir(parents=True, exist_ok=True)
                traced.save(
                    arguments.archive_traced / f"{atlas_name}.png", compress_level=9
                )
            definition = definitions[atlas_name]
            if atlas_name in shared_atlases:
                source_path = arguments.shared_root / f"{atlas_name}.png"
                base_atlas_manifest = shared_manifest["atlases"][atlas_name]
                if sha256_file(source_path) != base_atlas_manifest["sha256"]:
                    raise RuntimeError(
                        f"{source_path} does not match the approved GTA IV manifest"
                    )
                output_path = arguments.output / f"{atlas_name}.png"
                # Do not preserve the base file's timestamp. CMake's bundle-resource
                # copier is timestamp driven, so copy2() could leave a newly generated
                # shared atlas older than a stale file already in the app bundle.
                shutil.copyfile(source_path, output_path)
                for review_suffix in ("licensed_review", "sampling_review"):
                    source_review = (
                        arguments.shared_root
                        / "review"
                        / f"{atlas_name}_{review_suffix}.png"
                    )
                    if source_review.is_file():
                        shutil.copyfile(
                            source_review,
                            review_root / source_review.name,
                        )
                manifest["atlases"][atlas_name] = {
                    "file": output_path.name,
                    "sha256": sha256_file(output_path),
                    "font_id": definition.font_id,
                    "map_entries": len(definition.mapping),
                    "prop_entries": len(definition.proportional_trim),
                    "source_prop_entries": definition.source_proportional_count,
                    "ignored_prop_entries": (
                        definition.source_proportional_count
                        - len(definition.proportional_trim)
                    ),
                    "unproportional_width": definition.unproportional_width,
                    "whitespace": definition.whitespace,
                    "space_between_chars": list(definition.space_between_chars),
                    "vector_ranges": base_atlas_manifest["vector_ranges"],
                    "other_cells": "GTA IV shared atlas; stock alpha verified",
                    "shared_from_profile": "gta4",
                }
                print(f"{atlas_name}: {output_path} (shared GTA IV atlas)")
                continue
            rebuilt = traced.copy()
            replaced: list[dict[str, object]] = []
            face_outputs: list[
                tuple[FaceRange, list[GlyphRecord], FaceTransform]
            ] = []
            for face_range in face_ranges:
                if face_range.atlas != atlas_name:
                    continue
                records, preserved_edge_cells = face_records(face_range, definition)
                transform = face_range.fixed_transform or derive_face_transform(
                    face_range,
                    fonts[face_range.font_key],
                    traced,
                    records,
                )
                redrawn_records, transform_fallback_records = render_face(
                    rebuilt,
                    fonts[face_range.font_key],
                    transform,
                    records,
                    face_range.allow_traced_fallback,
                )
                fallback_cells = sorted(
                    preserved_edge_cells
                    + [record.index for record in transform_fallback_records]
                )
                face_outputs.append((face_range, redrawn_records, transform))
                replaced.append(
                    {
                        "role": face_range.role,
                        "font": fonts[face_range.font_key].name,
                        "first_cell": face_range.first,
                        "last_cell_exclusive": face_range.last_exclusive,
                        "redrawn_cells": len(redrawn_records),
                        "traced_fallback_cells": len(fallback_cells),
                        "preserved_edge_cells": fallback_cells,
                        "point_size": transform.point_size,
                        "baseline": transform.baseline,
                        "horizontal_scale": transform.horizontal_scale,
                        "reference_height": transform.reference_height,
                        "reference_glyphs": transform.reference_count,
                    }
                )
            verify_unmodified_cells(atlas_name, traced, rebuilt, face_outputs)
            verify_redrawn_geometry(atlas_name, rebuilt, face_outputs)
            output_path = arguments.output / f"{atlas_name}.png"
            rebuilt.save(output_path, compress_level=9)
            build_review(
                traced,
                rebuilt,
                review_root / f"{atlas_name}_licensed_review.png",
            )
            build_sampling_review(
                traced,
                rebuilt,
                face_outputs,
                review_root / f"{atlas_name}_sampling_review.png",
            )
            manifest["atlases"][atlas_name] = {
                "file": output_path.name,
                "sha256": sha256_file(output_path),
                "font_id": definition.font_id,
                "map_entries": len(definition.mapping),
                "prop_entries": len(definition.proportional_trim),
                "source_prop_entries": definition.source_proportional_count,
                "ignored_prop_entries": (
                    definition.source_proportional_count
                    - len(definition.proportional_trim)
                ),
                "unproportional_width": definition.unproportional_width,
                "whitespace": definition.whitespace,
                "space_between_chars": list(definition.space_between_chars),
                "vector_ranges": replaced,
                "other_cells": "traced stock contours",
            }
            print(
                f"{atlas_name}: {output_path} ({len(replaced)} licensed face ranges)"
            )
        manifest_path = arguments.output / "manifest.json"
        manifest_path.write_text(
            json.dumps(manifest, indent=2, sort_keys=True) + "\n", encoding="utf-8"
        )
        print(f"manifest: {manifest_path}")


if __name__ == "__main__":
    main()
