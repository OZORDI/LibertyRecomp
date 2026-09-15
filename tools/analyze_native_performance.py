#!/usr/bin/env python3
"""Analyze LibertyRecomp's deferred native-renderer performance CSV."""

from __future__ import annotations

import argparse
import csv
import html
import math
import statistics
from pathlib import Path
from typing import Iterable


DEFAULT_INPUT = (
    Path.home()
    / "Library"
    / "Application Support"
    / "LibertyRecomp"
    / "Diagnostics"
    / "native-performance-latest.csv"
)


def numeric_rows(path: Path) -> list[dict[str, float]]:
    with path.open(newline="", encoding="utf-8") as stream:
        reader = csv.DictReader(stream)
        if not reader.fieldnames:
            raise ValueError(f"{path} has no CSV header")
        rows: list[dict[str, float]] = []
        for raw in reader:
            converted: dict[str, float] = {}
            for key, value in raw.items():
                if key is None:
                    continue
                try:
                    converted[key] = float(value or 0)
                except ValueError as error:
                    raise ValueError(f"non-numeric value in {key}: {value!r}") from error
            rows.append(converted)
    if not rows:
        raise ValueError(f"{path} has no sample rows")
    return rows


def percentile(values: Iterable[float], quantile: float) -> float:
    ordered = sorted(values)
    if not ordered:
        return 0.0
    if len(ordered) == 1:
        return ordered[0]
    position = (len(ordered) - 1) * quantile
    lower = math.floor(position)
    upper = math.ceil(position)
    if lower == upper:
        return ordered[lower]
    fraction = position - lower
    return ordered[lower] * (1.0 - fraction) + ordered[upper] * fraction


def robust_spike_threshold(values: list[float]) -> tuple[float, float, float]:
    median = statistics.median(values)
    mad = statistics.median(abs(value - median) for value in values)
    return median + max(3.0, 6.0 * mad), median, mad


def metric_columns(rows: list[dict[str, float]], prefix: str) -> list[str]:
    if prefix == "gpu_" and any(row.get("counter_coarse_gpu_timing", 0) for row in rows):
        return []  # Only the complete GPU envelope was measured in coarse mode.
    excluded = {"gpu_frame_ms", "gpu_accounted_ms", "gpu_residual_ms"}
    return [
        name
        for name in rows[0]
        if name.startswith(prefix) and name.endswith("_ms") and name not in excluded
    ]


def local_baseline(
    rows: list[dict[str, float]], index: int, spike_indices: set[int], radius: int = 30
) -> dict[str, float]:
    begin = max(0, index - radius)
    end = min(len(rows), index + radius + 1)
    neighbors = [rows[i] for i in range(begin, end) if i not in spike_indices and i != index]
    if not neighbors:
        neighbors = [row for i, row in enumerate(rows) if i not in spike_indices and i != index]
    if not neighbors:
        return {key: 0.0 for key in rows[index]}
    return {
        key: statistics.median(row.get(key, 0.0) for row in neighbors)
        for key in rows[index]
    }


def classify_spike(row: dict[str, float], baseline: dict[str, float]) -> str:
    groups = {
        "GPU workload": row.get("gpu_frame_ms", 0.0) - baseline.get("gpu_frame_ms", 0.0),
        "presentation": row.get("cpu_presenter_total_ms", 0.0)
        - baseline.get("cpu_presenter_total_ms", 0.0),
        "renderer CPU": row.get("cpu_publish_ms", 0.0) - baseline.get("cpu_publish_ms", 0.0),
        "GPU/fence wait": row.get("cpu_fence_wait_ms", 0.0)
        - baseline.get("cpu_fence_wait_ms", 0.0),
        "slot cleanup": row.get("cpu_slot_cleanup_ms", 0.0)
        - baseline.get("cpu_slot_cleanup_ms", 0.0),
        "outside renderer": row.get("cpu_outside_renderer_ms", 0.0)
        - baseline.get("cpu_outside_renderer_ms", 0.0),
    }
    name, delta = max(groups.items(), key=lambda item: item[1])
    return name if delta > 0.25 else "mixed/unclear"


def spike_records(rows: list[dict[str, float]]) -> tuple[list[dict[str, object]], float, float, float]:
    intervals = [row.get("cpu_frame_interval_ms", 0.0) for row in rows]
    threshold, median, mad = robust_spike_threshold(intervals)
    indices = {index for index, value in enumerate(intervals) if value > threshold}
    coarse = any(row.get("counter_coarse_gpu_timing", 0) for row in rows)
    gpu_columns = ["gpu_frame_ms"] if coarse else metric_columns(rows, "gpu_") + ["gpu_residual_ms"]
    cpu_columns = [
        name
        for name in metric_columns(rows, "cpu_")
        if name not in {"cpu_frame_interval_ms", "cpu_outside_renderer_ms"}
    ]
    records: list[dict[str, object]] = []
    for index in sorted(indices, key=lambda i: intervals[i], reverse=True):
        row = rows[index]
        baseline = local_baseline(rows, index, indices)
        deltas = {
            name: row.get(name, 0.0) - baseline.get(name, 0.0)
            for name in gpu_columns + cpu_columns
        }
        positive = sorted(deltas.items(), key=lambda item: item[1], reverse=True)
        contributor, contributor_delta = positive[0] if positive else ("none", 0.0)
        records.append(
            {
                "rank": len(records) + 1,
                "capture_index": index,
                "frame": int(row.get("frame", 0.0)),
                "frame_interval_ms": intervals[index],
                "local_baseline_ms": baseline.get("cpu_frame_interval_ms", median),
                "excess_ms": intervals[index]
                - baseline.get("cpu_frame_interval_ms", median),
                "gpu_frame_ms": row.get("gpu_frame_ms", 0.0),
                "cpu_publish_ms": row.get("cpu_publish_ms", 0.0),
                "cpu_fence_wait_ms": row.get("cpu_fence_wait_ms", 0.0),
                "cpu_housekeeping_ms": row.get("cpu_housekeeping_ms", 0.0),
                "cpu_slot_cleanup_ms": row.get("cpu_slot_cleanup_ms", 0.0),
                "cpu_profile_readback_ms": row.get("cpu_profile_readback_ms", 0.0),
                "cpu_presenter_acquire_ms": row.get("cpu_presenter_acquire_ms", 0.0),
                "cpu_presenter_submit_ms": row.get("cpu_presenter_submit_ms", 0.0),
                "cpu_presenter_present_ms": row.get("cpu_presenter_present_ms", 0.0),
                "cpu_presenter_total_ms": row.get("cpu_presenter_total_ms", 0.0),
                "cpu_outside_renderer_ms": row.get("cpu_outside_renderer_ms", 0.0),
                "classification": classify_spike(row, baseline),
                "largest_delta": contributor,
                "largest_delta_ms": contributor_delta,
            }
        )
    return records, threshold, median, mad


def write_spike_csv(path: Path, records: list[dict[str, object]]) -> None:
    fields = [
        "rank",
        "capture_index",
        "frame",
        "frame_interval_ms",
        "local_baseline_ms",
        "excess_ms",
        "gpu_frame_ms",
        "cpu_publish_ms",
        "cpu_fence_wait_ms",
        "cpu_housekeeping_ms",
        "cpu_slot_cleanup_ms",
        "cpu_profile_readback_ms",
        "cpu_presenter_acquire_ms",
        "cpu_presenter_submit_ms",
        "cpu_presenter_present_ms",
        "cpu_presenter_total_ms",
        "cpu_outside_renderer_ms",
        "classification",
        "largest_delta",
        "largest_delta_ms",
    ]
    with path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields)
        writer.writeheader()
        writer.writerows(records)


def svg_polyline(values: list[float], width: float, height: float, maximum: float) -> str:
    if not values:
        return ""
    denominator = max(1, len(values) - 1)
    points = []
    for index, value in enumerate(values):
        x = index * width / denominator
        y = height - min(value, maximum) * height / maximum
        points.append(f"{x:.2f},{y:.2f}")
    return " ".join(points)


def write_timeline_svg(
    path: Path,
    rows: list[dict[str, float]],
    spikes: list[dict[str, object]],
    threshold: float,
) -> None:
    width, height = 1200.0, 520.0
    left, top, plot_width, plot_height = 70.0, 55.0, 1100.0, 390.0
    interval = [row.get("cpu_frame_interval_ms", 0.0) for row in rows]
    gpu = [row.get("gpu_frame_ms", 0.0) for row in rows]
    publish = [row.get("cpu_publish_ms", 0.0) for row in rows]
    presenter = [row.get("cpu_presenter_total_ms", 0.0) for row in rows]
    maximum = max(1.0, percentile(interval + gpu + publish + presenter, 0.99), threshold * 1.1)
    interval_points = svg_polyline(interval, plot_width, plot_height, maximum)
    gpu_points = svg_polyline(gpu, plot_width, plot_height, maximum)
    publish_points = svg_polyline(publish, plot_width, plot_height, maximum)
    presenter_points = svg_polyline(presenter, plot_width, plot_height, maximum)
    spike_indices = {int(record["capture_index"]) for record in spikes}
    markers = []
    denominator = max(1, len(rows) - 1)
    for index in spike_indices:
        x = left + index * plot_width / denominator
        markers.append(
            f'<line x1="{x:.2f}" y1="{top:.2f}" x2="{x:.2f}" '
            f'y2="{top + plot_height:.2f}" stroke="#ff4d4d" stroke-width="1" opacity="0.45"/>'
        )
    grid = []
    for step in range(6):
        y = top + plot_height * step / 5
        value = maximum * (5 - step) / 5
        grid.append(
            f'<line x1="{left}" y1="{y:.2f}" x2="{left + plot_width}" y2="{y:.2f}" '
            f'stroke="#2e3440"/><text x="{left - 8}" y="{y + 4:.2f}" text-anchor="end" '
            f'fill="#aeb7c4" font-size="12">{value:.1f}</text>'
        )
    document = f"""<svg xmlns="http://www.w3.org/2000/svg" width="{width:.0f}" height="{height:.0f}" viewBox="0 0 {width:.0f} {height:.0f}">
<rect width="100%" height="100%" fill="#10151d"/>
<text x="{left}" y="28" fill="#f2f4f8" font-family="sans-serif" font-size="20">Native renderer frame timeline</text>
<text x="{left}" y="47" fill="#aeb7c4" font-family="sans-serif" font-size="12">milliseconds; red vertical markers are robust frame-interval spikes</text>
<g font-family="sans-serif">{''.join(grid)}{''.join(markers)}</g>
<g transform="translate({left},{top})" fill="none" stroke-linejoin="round" stroke-linecap="round">
<polyline points="{interval_points}" stroke="#ff6b57" stroke-width="2"/>
<polyline points="{gpu_points}" stroke="#45aaf2" stroke-width="1.5"/>
<polyline points="{publish_points}" stroke="#f7b731" stroke-width="1.5"/>
<polyline points="{presenter_points}" stroke="#a55eea" stroke-width="1.5"/>
</g>
<g font-family="sans-serif" font-size="13"><text x="{left}" y="480" fill="#ff6b57">frame interval</text><text x="{left + 130}" y="480" fill="#45aaf2">GPU envelope</text><text x="{left + 250}" y="480" fill="#f7b731">renderer publish CPU</text><text x="{left + 425}" y="480" fill="#a55eea">presenter total CPU</text></g>
<text x="{left + plot_width / 2}" y="510" text-anchor="middle" fill="#aeb7c4" font-family="sans-serif" font-size="12">capture index (0–{len(rows) - 1})</text>
</svg>"""
    path.write_text(document, encoding="utf-8")


def write_breakdown_svg(path: Path, rows: list[dict[str, float]]) -> None:
    columns = metric_columns(rows, "gpu_")
    coarse = any(row.get("counter_coarse_gpu_timing", 0) for row in rows)
    if coarse:
        columns = ["gpu_frame_ms"]
    chart_title = "GPU envelope — pass timing unavailable" if coarse else "Approximate GPU range breakdown"
    totals = {name: sum(row.get(name, 0.0) for row in rows) for name in columns}
    selected = [name for name, _ in sorted(totals.items(), key=lambda item: item[1], reverse=True)[:8]]
    width, height = 1200.0, 520.0
    left, top, plot_width, plot_height = 70.0, 55.0, 1100.0, 390.0
    per_frame_totals = [sum(row.get(name, 0.0) for name in columns) for row in rows]
    maximum = max(1.0, percentile(per_frame_totals, 0.99))
    colors = ["#45aaf2", "#26de81", "#fed330", "#fc5c65", "#a55eea", "#2bcbba", "#fd9644", "#778ca3", "#4b6584"]
    bar_width = plot_width / max(1, len(rows))
    rectangles: list[str] = []
    for index, row in enumerate(rows):
        y = top + plot_height
        selected_sum = 0.0
        for color, name in zip(colors, selected):
            value = row.get(name, 0.0)
            selected_sum += value
            bar_height = min(value, maximum) * plot_height / maximum
            y -= bar_height
            rectangles.append(
                f'<rect x="{left + index * bar_width:.2f}" y="{y:.2f}" width="{max(0.5, bar_width):.2f}" height="{bar_height:.2f}" fill="{color}"/>'
            )
        other = max(0.0, per_frame_totals[index] - selected_sum)
        other_height = min(other, maximum) * plot_height / maximum
        y -= other_height
        rectangles.append(
            f'<rect x="{left + index * bar_width:.2f}" y="{y:.2f}" width="{max(0.5, bar_width):.2f}" height="{other_height:.2f}" fill="{colors[-1]}"/>'
        )
    legend = []
    for index, name in enumerate(selected + ["other"]):
        x = left + (index % 5) * 215
        y = 475 + (index // 5) * 20
        color = colors[index] if index < len(selected) else colors[-1]
        label = html.escape(name.removeprefix("gpu_").removesuffix("_ms").replace("_", " "))
        legend.append(f'<rect x="{x}" y="{y - 10}" width="10" height="10" fill="{color}"/><text x="{x + 15}" y="{y}" fill="#d8dee9" font-size="11">{label}</text>')
    document = f"""<svg xmlns="http://www.w3.org/2000/svg" width="{width:.0f}" height="{height:.0f}" viewBox="0 0 {width:.0f} {height:.0f}">
<rect width="100%" height="100%" fill="#10151d"/>
<text x="{left}" y="28" fill="#f2f4f8" font-family="sans-serif" font-size="20">{chart_title}</text>
<text x="{left}" y="47" fill="#aeb7c4" font-family="sans-serif" font-size="12">stacked milliseconds per captured frame; clipped at p99={maximum:.2f} ms</text>
<line x1="{left}" y1="{top + plot_height}" x2="{left + plot_width}" y2="{top + plot_height}" stroke="#4b5563"/>
{''.join(rectangles)}<g font-family="sans-serif">{''.join(legend)}</g>
</svg>"""
    path.write_text(document, encoding="utf-8")


def summary_table(rows: list[dict[str, float]], columns: list[str], limit: int = 12) -> str:
    ranked = sorted(columns, key=lambda name: sum(row.get(name, 0.0) for row in rows), reverse=True)
    lines = ["| Metric | Median | p95 | p99 | Max |", "|---|---:|---:|---:|---:|"]
    for name in ranked[:limit]:
        values = [row.get(name, 0.0) for row in rows]
        lines.append(
            f"| `{name}` | {statistics.median(values):.3f} | {percentile(values, 0.95):.3f} | "
            f"{percentile(values, 0.99):.3f} | {max(values):.3f} |"
        )
    return "\n".join(lines)


def linear_slope(xs: list[float], ys: list[float]) -> float:
    if len(xs) < 2 or len(xs) != len(ys):
        return 0.0
    x_mean = statistics.mean(xs)
    y_mean = statistics.mean(ys)
    denominator = sum((value - x_mean) ** 2 for value in xs)
    if denominator == 0.0:
        return 0.0
    return sum((x - x_mean) * (y - y_mean) for x, y in zip(xs, ys)) / denominator


def memory_table(rows: list[dict[str, float]]) -> str:
    byte_columns = [
        "counter_process_physical_footprint_bytes",
        "counter_process_resident_bytes",
        "counter_surface_image_bytes",
        "counter_texture_image_bytes",
        "counter_upload_buffer_capacity_bytes",
        "counter_upload_buffer_allocation_bytes",
    ]
    frames = [row.get("frame", float(index)) for index, row in enumerate(rows)]
    lines = [
        "| Metric | First MiB | Last MiB | Peak MiB | Delta MiB | Trend MiB / 1000 frames |",
        "|---|---:|---:|---:|---:|---:|",
    ]
    bytes_per_mib = 1024.0 * 1024.0
    for name in byte_columns:
        if name not in rows[0]:
            continue
        values = [row.get(name, 0.0) for row in rows]
        slope = linear_slope(frames, values)
        lines.append(
            f"| `{name}` | {values[0] / bytes_per_mib:.3f} | "
            f"{values[-1] / bytes_per_mib:.3f} | {max(values) / bytes_per_mib:.3f} | "
            f"{(values[-1] - values[0]) / bytes_per_mib:.3f} | "
            f"{slope * 1000.0 / bytes_per_mib:.3f} |"
        )
    return "\n".join(lines)


def residency_table(rows: list[dict[str, float]]) -> str:
    columns = [
        "counter_surface_images_live",
        "counter_texture_images_live",
        "counter_pipelines_live",
        "counter_pending_texture_releases",
        "counter_texture_image_evictions",
        "counter_texture_allocation_failures",
    ]
    lines = ["| Counter | First | Last | Peak | Delta |", "|---|---:|---:|---:|---:|"]
    for name in columns:
        if name not in rows[0]:
            continue
        values = [row.get(name, 0.0) for row in rows]
        lines.append(
            f"| `{name}` | {values[0]:.0f} | {values[-1]:.0f} | {max(values):.0f} | "
            f"{values[-1] - values[0]:.0f} |"
        )
    return "\n".join(lines)


def write_markdown(
    path: Path,
    source: Path,
    rows: list[dict[str, float]],
    spikes: list[dict[str, object]],
    threshold: float,
    median: float,
    mad: float,
) -> None:
    intervals = [row.get("cpu_frame_interval_ms", 0.0) for row in rows]
    spike_gaps = [
        int(spikes[index - 1]["frame"]) - int(spikes[index]["frame"])
        for index in range(1, len(spikes))
    ]
    # Ranking is by severity, so periodicity uses chronological frames instead.
    chronological = sorted(int(record["frame"]) for record in spikes)
    spike_gaps = [chronological[index] - chronological[index - 1] for index in range(1, len(chronological))]
    periodicity = (
        f"median spike gap {statistics.median(spike_gaps):.1f} frames "
        f"(min {min(spike_gaps)}, max {max(spike_gaps)})"
        if spike_gaps
        else "not enough spikes to estimate periodicity"
    )
    top_spikes = [
        f"| {record['rank']} | {record['frame']} | {record['frame_interval_ms']:.3f} | "
        f"{record['excess_ms']:.3f} | {record['classification']} | "
        f"`{record['largest_delta']}` (+{record['largest_delta_ms']:.3f}) |"
        for record in spikes[:20]
    ]
    if not top_spikes:
        top_spikes = ["| — | — | — | — | No robust spikes detected | — |"]
    gpu_columns = metric_columns(rows, "gpu_") + ["gpu_residual_ms"]
    cpu_columns = metric_columns(rows, "cpu_")
    coarse = any(row.get("counter_coarse_gpu_timing", 0) for row in rows)
    gpu_explanation = (
        "Coarse mode measures the whole GPU command-buffer envelope. Per-pass costs are unavailable; "
        "the unattributed field must not be interpreted as an isolated workload."
        if coarse else
        "Ranges are approximate: a backend may defer timestamps to encoder boundaries. "
        "Extra boundaries may perturb the workload; these values do not prove isolated pass costs."
    )
    if coarse:
        gpu_columns = ["gpu_frame_ms"]
    text = f"""# Native renderer performance analysis

Source: `{source}`  
Samples: **{len(rows)}** consecutive frames ({int(rows[0].get('frame', 0))}–{int(rows[-1].get('frame', 0))})

## Frame pacing

- Frame interval: median **{median:.3f} ms**, p95 **{percentile(intervals, 0.95):.3f} ms**, p99 **{percentile(intervals, 0.99):.3f} ms**, max **{max(intervals):.3f} ms**.
- Robust spike threshold: **{threshold:.3f} ms** (`median + max(3 ms, 6×MAD)`, MAD **{mad:.3f} ms**).
- Detected **{len(spikes)}** spikes; {periodicity}.
- Classification is evidence-based attribution from local deltas, not proof of causality.

## Largest spikes

| Rank | Frame | Interval ms | Excess vs local baseline | Dominant domain | Largest measured delta ms |
|---:|---:|---:|---:|---|---|
{chr(10).join(top_spikes)}

## GPU timing

{gpu_explanation}

{summary_table(rows, gpu_columns)}

## CPU and frame timing

New captures measure `cpu_fence_wait_ms` only inside submission completion waiting; slot cleanup and profiler readback are separate. Older captures without those columns included cleanup in fence wait.

`cpu_pipeline_compile_jobs_ms` is compiler-job wall time, reported when results are consumed; it can overlap render-owner work and is not an exclusive CPU range. `cpu_pipeline_wait_ms` measures the render owner's exact-pipeline wait. These ranges must not be added to publish time or each other as separate frame costs.

{summary_table(rows, cpu_columns)}

## Memory and native residency

The trend is an ordinary least-squares slope across this bounded capture. A positive slope is a
screening signal, not proof of a leak; repeat the same scene after warmup and compare whether the
resource counts and byte totals plateau.

{memory_table(rows)}

{residency_table(rows)}

## Artifacts

- `native-performance-timeline.svg`: frame interval, full GPU envelope, renderer publish CPU, and spike markers.
- `native-performance-breakdown.svg`: approximate GPU range stacks, or the whole GPU envelope when coarse timing disables per-pass attribution.
- `native-performance-spikes.csv`: ranked spike details for further analysis.
"""
    path.write_text(text, encoding="utf-8")


def analyze(source: Path, output_directory: Path | None = None, *, markdown: bool = False,
            include_first: bool = False, compare: Path | None = None) -> dict[str, Path]:
    with source.open(newline="", encoding="utf-8") as stream:
        schema = next(csv.DictReader(stream), {}).get("schema_version", "0")
    if schema == "2":
        from analyze_native_profile_detail import analyze_detail
        return analyze_detail(source, output_directory, include_first=include_first, compare=compare)
    rows = numeric_rows(source)
    spikes, threshold, median, mad = spike_records(rows)
    destination = output_directory or source.parent
    destination.mkdir(parents=True, exist_ok=True)
    outputs = {
        "markdown": destination / "native-performance-analysis.md",
        "timeline": destination / "native-performance-timeline.svg",
        "breakdown": destination / "native-performance-breakdown.svg",
        "spikes": destination / "native-performance-spikes.csv",
    }
    write_spike_csv(outputs["spikes"], spikes)
    write_timeline_svg(outputs["timeline"], rows, spikes, threshold)
    write_breakdown_svg(outputs["breakdown"], rows)
    if markdown:
        write_markdown(outputs["markdown"], source, rows, spikes, threshold, median, mad)
    else:
        outputs.pop("markdown")
    return outputs


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("csv", nargs="?", type=Path, default=DEFAULT_INPUT)
    parser.add_argument("--output-directory", type=Path)
    parser.add_argument("--markdown", action="store_true", help="Explicitly request a legacy Markdown report")
    parser.add_argument("--include-first", action="store_true", help="Include the schema-2 capture-start sample")
    parser.add_argument("--compare", type=Path, help="Prior detailed analysis JSON")
    arguments = parser.parse_args()
    outputs = analyze(arguments.csv.expanduser(), arguments.output_directory, markdown=arguments.markdown, include_first=arguments.include_first, compare=arguments.compare)
    for label, path in outputs.items():
        print(f"{label}: {path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
