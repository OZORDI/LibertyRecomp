#!/usr/bin/env python3

from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import re
import struct
import subprocess
import sys
import tempfile
from typing import Any, Iterable, Sequence


SPIRV_MAGIC = 0x07230203
SPIRV_HEADER_WORDS = 5
SPIRV_OP_ENTRY_POINT = 15
SPIRV_OP_EXECUTION_MODE = 16
SPIRV_EXECUTION_MODE_EARLY_FRAGMENT_TESTS = 9
SPIRV_EXECUTION_MODELS = {"vertex": 0, "pixel": 4}
SHADER_OVERRIDE_STAGES = {"pixel": 0, "vertex": 1}
SPEC_CONSTANT_ALPHA_TEST = 0x00000002
HASH_PATTERN = re.compile(r"0x[0-9A-Fa-f]{16}\Z")
MASK_PATTERN = re.compile(r"0x[0-9A-Fa-f]{8}\Z")
ENTRY_POINT_PATTERN = re.compile(r"[A-Za-z_][A-Za-z0-9_]*\Z")
DEFINE_PATTERN = re.compile(r"[A-Za-z_][A-Za-z0-9_]*(?:=[A-Za-z0-9_+.,-]+)?\Z")
TOP_LEVEL_KEYS = {"version", "overrides"}
ENTRY_KEYS = {
    "stage",
    "hash",
    "language",
    "source",
    "entry_point",
    "specialization_constants_mask",
    "defines",
}


class ManifestError(ValueError):
    pass


def _require_exact_keys(value: dict[str, Any], allowed: set[str], context: str) -> None:
    unknown = sorted(set(value) - allowed)
    if unknown:
        raise ManifestError(f"{context} contains unknown keys: {', '.join(unknown)}")


def _parse_hex(value: Any, pattern: re.Pattern[str], context: str) -> int:
    if not isinstance(value, str) or not pattern.fullmatch(value):
        raise ManifestError(f"{context} must match {pattern.pattern!r}")
    return int(value, 16)


def _resolve_source(source_root: Path, source_value: Any, context: str) -> tuple[Path, str]:
    if not isinstance(source_value, str) or not source_value:
        raise ManifestError(f"{context}.source must be a non-empty relative path")
    relative = Path(source_value)
    if relative.is_absolute():
        raise ManifestError(f"{context}.source must be relative to the source root")
    root = source_root.resolve(strict=True)
    source = (root / relative).resolve(strict=False)
    try:
        canonical_relative = source.relative_to(root).as_posix()
    except ValueError as error:
        raise ManifestError(f"{context}.source escapes the source root") from error
    if not source.is_file():
        raise ManifestError(f"{context}.source does not exist: {canonical_relative}")
    return source, canonical_relative


def load_manifest(manifest_path: Path, source_root: Path) -> list[dict[str, Any]]:
    try:
        document = json.loads(manifest_path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as error:
        raise ManifestError(f"failed to read manifest {manifest_path}: {error}") from error
    if not isinstance(document, dict):
        raise ManifestError("manifest root must be an object")
    _require_exact_keys(document, TOP_LEVEL_KEYS, "manifest")
    if document.get("version") != 1:
        raise ManifestError("manifest.version must be 1")
    raw_entries = document.get("overrides")
    if not isinstance(raw_entries, list):
        raise ManifestError("manifest.overrides must be an array")

    entries: list[dict[str, Any]] = []
    identities: set[tuple[int, int]] = set()
    for index, raw_entry in enumerate(raw_entries):
        context = f"manifest.overrides[{index}]"
        if not isinstance(raw_entry, dict):
            raise ManifestError(f"{context} must be an object")
        _require_exact_keys(raw_entry, ENTRY_KEYS, context)

        stage_name = raw_entry.get("stage")
        if stage_name not in SHADER_OVERRIDE_STAGES:
            raise ManifestError(f"{context}.stage must be 'pixel' or 'vertex'")
        language = raw_entry.get("language")
        if language not in {"hlsl", "glsl"}:
            raise ManifestError(f"{context}.language must be 'hlsl' or 'glsl'")
        source, source_name = _resolve_source(source_root, raw_entry.get("source"), context)
        expected_suffix = f".{language}"
        if source.suffix.lower() != expected_suffix:
            raise ManifestError(f"{context}.source must use the {expected_suffix} extension")

        entry_point = raw_entry.get("entry_point")
        if not isinstance(entry_point, str) or not ENTRY_POINT_PATTERN.fullmatch(entry_point):
            raise ManifestError(f"{context}.entry_point is not a valid shader entry point")
        if language == "glsl" and entry_point != "main":
            raise ManifestError(f"{context}.entry_point must be 'main' for GLSL")
        shader_hash = _parse_hex(raw_entry.get("hash"), HASH_PATTERN, f"{context}.hash")
        mask = _parse_hex(
            raw_entry.get("specialization_constants_mask"),
            MASK_PATTERN,
            f"{context}.specialization_constants_mask",
        )

        raw_defines = raw_entry.get("defines", [])
        if not isinstance(raw_defines, list) or any(
            not isinstance(define, str) or not DEFINE_PATTERN.fullmatch(define)
            for define in raw_defines
        ):
            raise ManifestError(f"{context}.defines must be an array of NAME or NAME=value strings")
        if len(set(raw_defines)) != len(raw_defines):
            raise ManifestError(f"{context}.defines contains a duplicate")

        identity = (SHADER_OVERRIDE_STAGES[stage_name], shader_hash)
        if identity in identities:
            raise ManifestError(
                f"{context} duplicates the {stage_name} shader identity {raw_entry['hash']}"
            )
        identities.add(identity)
        entries.append(
            {
                "stage_name": stage_name,
                "stage": SHADER_OVERRIDE_STAGES[stage_name],
                "execution_model": SPIRV_EXECUTION_MODELS[stage_name],
                "hash": shader_hash,
                "language": language,
                "source": source,
                "source_name": source_name,
                "entry_point": entry_point,
                "specialization_constants_mask": mask,
                "defines": sorted(raw_defines),
            }
        )

    return sorted(entries, key=lambda entry: (entry["stage"], entry["hash"]))


def _run_tool(command: Sequence[str], environment: dict[str, str] | None = None) -> None:
    result = subprocess.run(
        command,
        check=False,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        env=environment,
    )
    if result.returncode:
        rendered = " ".join(command)
        raise ManifestError(f"shader tool failed ({rendered}):\n{result.stdout}")


def _compiler_environment(library_path: str | None) -> dict[str, str]:
    environment = dict(os.environ)
    if not library_path:
        return environment
    variable = "DYLD_LIBRARY_PATH" if sys.platform == "darwin" else "LD_LIBRARY_PATH"
    existing = environment.get(variable)
    environment[variable] = (
        library_path if not existing else os.pathsep.join((library_path, existing))
    )
    return environment


def requires_late_fragment_tests(entry: dict[str, Any]) -> bool:
    return (
        entry["stage_name"] == "pixel"
        and entry["specialization_constants_mask"] & SPEC_CONSTANT_ALPHA_TEST
    ) != 0


def compile_shader(
    entry: dict[str, Any],
    output_path: Path,
    source_root: Path,
    dxc: str | None,
    dxc_library_path: str | None,
    glslang_validator: str | None,
    additional_defines: Sequence[str] = (),
) -> None:
    source = entry["source"]
    if entry["language"] == "hlsl":
        if not dxc:
            raise ManifestError(f"DXC is required by {entry['source_name']}")
        target = "vs_6_0" if entry["stage_name"] == "vertex" else "ps_6_0"
        command = [
            dxc,
            "-spirv",
            "-fspv-target-env=vulkan1.0",
            "-HV",
            "2021",
            "-T",
            target,
            "-E",
            entry["entry_point"],
            "-all-resources-bound",
            "-fvk-use-dx-layout",
            "-O3",
            "-Qstrip_debug",
            "-WX",
            "-I",
            str(source_root),
            "-I",
            str(source.parent),
        ]
        if entry["stage_name"] == "vertex":
            command.append("-fvk-invert-y")
        else:
            # Match XenosRecomp's stock pixel-shader compilation. Its shared
            # sampling helpers select implicit derivatives only with this
            # stage define; without it, overrides silently force mip level 0.
            command.append("-DXENOS_RECOMP_PIXEL_SHADER")
        command.extend(f"-D{define}" for define in entry["defines"])
        command.extend(f"-D{define}" for define in additional_defines)
        command.extend(("-Fo", str(output_path), str(source)))
        _run_tool(command, _compiler_environment(dxc_library_path))
        return

    if not glslang_validator:
        raise ManifestError(f"glslangValidator is required by {entry['source_name']}")
    shader_stage = "vert" if entry["stage_name"] == "vertex" else "frag"
    command = [
        glslang_validator,
        "-V",
        "--target-env",
        "vulkan1.0",
        "-S",
        shader_stage,
        "-e",
        entry["entry_point"],
        "-g0",
        "-Os",
        "-I" + str(source_root),
        "-I" + str(source.parent),
    ]
    for define in entry["defines"]:
        command.extend(("--define-macro", define))
    for define in additional_defines:
        command.extend(("--define-macro", define))
    command.extend(("-o", str(output_path), str(source)))
    _run_tool(command)


def _decode_spirv_string(words: Sequence[int]) -> str:
    encoded = b"".join(struct.pack("<I", word) for word in words)
    terminator = encoded.find(b"\0")
    if terminator < 0:
        raise ManifestError("SPIR-V OpEntryPoint name is not null-terminated")
    try:
        return encoded[:terminator].decode("utf-8")
    except UnicodeDecodeError as error:
        raise ManifestError("SPIR-V OpEntryPoint name is not UTF-8") from error


def validate_spirv(
    blob: bytes,
    expected_execution_model: int,
    expected_entry_point: str,
    forbid_early_fragment_tests: bool = False,
) -> tuple[int, ...]:
    if len(blob) < SPIRV_HEADER_WORDS * struct.calcsize("<I"):
        raise ManifestError("SPIR-V module is shorter than its header")
    if len(blob) % struct.calcsize("<I"):
        raise ManifestError("SPIR-V byte size is not a multiple of four")
    word_count = len(blob) // struct.calcsize("<I")
    words = struct.unpack(f"<{word_count}I", blob)
    if words[0] != SPIRV_MAGIC:
        raise ManifestError("SPIR-V module has an invalid magic number")

    matching_entry_point = False
    index = SPIRV_HEADER_WORDS
    while index < len(words):
        instruction = words[index]
        instruction_word_count = instruction >> 16
        opcode = instruction & 0xFFFF
        if not instruction_word_count:
            raise ManifestError("SPIR-V instruction has a zero word count")
        instruction_end = index + instruction_word_count
        if instruction_end > len(words):
            raise ManifestError("SPIR-V instruction extends past the module")
        if opcode == SPIRV_OP_ENTRY_POINT:
            if instruction_word_count < 4:
                raise ManifestError("SPIR-V OpEntryPoint is truncated")
            execution_model = words[index + 1]
            entry_point = _decode_spirv_string(words[index + 3 : instruction_end])
            matching_entry_point |= (
                execution_model == expected_execution_model and entry_point == expected_entry_point
            )
        elif (
            forbid_early_fragment_tests
            and opcode == SPIRV_OP_EXECUTION_MODE
            and instruction_word_count >= 3
            and words[index + 2] == SPIRV_EXECUTION_MODE_EARLY_FRAGMENT_TESTS
        ):
            raise ManifestError("late SPIR-V unexpectedly enables EarlyFragmentTests")
        index = instruction_end
    if not matching_entry_point:
        raise ManifestError(
            f"SPIR-V does not contain the requested stage/entry point {expected_entry_point!r}"
        )
    return words


def _format_words(words: Iterable[int]) -> str:
    return "\n".join(f"    0x{word:08X}u," for word in words)


def generate_cpp(compiled_entries: Sequence[dict[str, Any]]) -> str:
    output = [
        '#include <shader_overrides/shader_override_cache.h>',
        "",
        "namespace {",
    ]
    for index, entry in enumerate(compiled_entries):
        output.extend(
            (
                f"constexpr uint32_t kShaderOverrideSpirv{index}[] = {{",
                _format_words(entry["words"]),
                "};",
                "",
            )
        )
        if entry.get("late_words") is not None:
            output.extend(
                (
                    f"constexpr uint32_t kShaderOverrideLateSpirv{index}[] = {{",
                    _format_words(entry["late_words"]),
                    "};",
                    "",
                )
            )
    output.append("}  // namespace")
    output.append("")
    if compiled_entries:
        output.append("const ShaderOverrideCacheEntry g_shaderOverrideEntries[] = {")
        for index, entry in enumerate(compiled_entries):
            stage_constant = (
                "kShaderOverrideStageVertex"
                if entry["stage_name"] == "vertex"
                else "kShaderOverrideStagePixel"
            )
            filename = json.dumps(entry["source_name"], ensure_ascii=True)
            if entry.get("late_words") is None:
                late_spirv = "nullptr"
                late_spirv_size = "0"
            else:
                late_spirv = f"kShaderOverrideLateSpirv{index}"
                late_spirv_size = f"sizeof(kShaderOverrideLateSpirv{index})"
            output.append(
                "    {"
                f"0x{entry['hash']:016X}ull, {stage_constant}, "
                f"0x{entry['specialization_constants_mask']:08X}u, "
                f"kShaderOverrideSpirv{index}, sizeof(kShaderOverrideSpirv{index}), "
                f"{late_spirv}, {late_spirv_size}, {filename}"
                "},"
            )
        output.append("};")
        output.append(
            "const size_t g_shaderOverrideEntryCount = "
            "sizeof(g_shaderOverrideEntries) / sizeof(g_shaderOverrideEntries[0]);"
        )
    else:
        output.extend(
            (
                "const ShaderOverrideCacheEntry g_shaderOverrideEntries[1] = {};",
                "const size_t g_shaderOverrideEntryCount = 0;",
            )
        )
    output.append("")
    return "\n".join(output)


def _write_if_changed(path: Path, content: str) -> None:
    encoded = content.encode("utf-8")
    if path.is_file() and path.read_bytes() == encoded:
        return
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(path.name + ".tmp")
    temporary.write_bytes(encoded)
    temporary.replace(path)


def _escape_depfile_path(path: Path) -> str:
    return str(path).replace("$", "$$").replace("#", "\\#").replace(" ", "\\ ")


def generate_depfile(output: Path, dependencies: Sequence[Path]) -> str:
    target = _escape_depfile_path(output.resolve())
    dependency_list = " ".join(
        _escape_depfile_path(dependency.resolve()) for dependency in dependencies
    )
    return f"{target}: {dependency_list}\n"


def parse_arguments(arguments: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Compile hash-specific GTA IV native shader overrides into a C++ SPIR-V table."
    )
    parser.add_argument("--manifest", required=True, type=Path)
    parser.add_argument("--source-root", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--depfile", type=Path)
    parser.add_argument("--dxc")
    parser.add_argument("--dxc-library-path")
    parser.add_argument("--glslang-validator")
    parser.add_argument("--spirv-val")
    return parser.parse_args(arguments)


def main(arguments: Sequence[str] | None = None) -> int:
    options = parse_arguments(arguments)
    try:
        entries = load_manifest(options.manifest, options.source_root)
        if entries and not options.spirv_val:
            raise ManifestError("spirv-val is required when the override manifest is not empty")

        compiled_entries: list[dict[str, Any]] = []
        with tempfile.TemporaryDirectory(prefix="gta4-shader-overrides-") as temporary_directory:
            temporary_root = Path(temporary_directory)
            for index, entry in enumerate(entries):
                spirv_path = temporary_root / f"override-{index}.spv"
                compile_shader(
                    entry,
                    spirv_path,
                    options.source_root.resolve(strict=True),
                    options.dxc,
                    options.dxc_library_path,
                    options.glslang_validator,
                )
                _run_tool(
                    (
                        options.spirv_val,
                        "--target-env",
                        "vulkan1.0",
                        str(spirv_path),
                    )
                )
                compiled_entry = dict(entry)
                compiled_entry["words"] = validate_spirv(
                    spirv_path.read_bytes(),
                    entry["execution_model"],
                    entry["entry_point"],
                )
                compiled_entry["late_words"] = None
                if requires_late_fragment_tests(entry):
                    late_spirv_path = temporary_root / f"override-{index}-late.spv"
                    compile_shader(
                        entry,
                        late_spirv_path,
                        options.source_root.resolve(strict=True),
                        options.dxc,
                        options.dxc_library_path,
                        options.glslang_validator,
                        ("XENOS_RECOMP_LATE_FRAGMENT_TESTS",),
                    )
                    _run_tool(
                        (
                            options.spirv_val,
                            "--target-env",
                            "vulkan1.0",
                            str(late_spirv_path),
                        )
                    )
                    compiled_entry["late_words"] = validate_spirv(
                        late_spirv_path.read_bytes(),
                        entry["execution_model"],
                        entry["entry_point"],
                        forbid_early_fragment_tests=True,
                    )
                compiled_entries.append(compiled_entry)

        _write_if_changed(options.output, generate_cpp(compiled_entries))
        if options.depfile:
            dependencies = [options.manifest, Path(__file__)]
            dependencies.extend(entry["source"] for entry in entries)
            _write_if_changed(options.depfile, generate_depfile(options.output, dependencies))
    except ManifestError as error:
        print(f"shader override compiler: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
