#!/usr/bin/env python3
"""Schema-2 native profiler analysis with explicit timing domains and validity.

Consumes raw captures without replacing missing observations with zero. The
output is JSON/CSV/HTML. Render-owner self time, inclusive spans, producer sums,
GPU queue intervals and presenter observations are separate domains.
"""
from __future__ import annotations
import argparse
import collections
import csv
import html
import json
import math
import statistics
from pathlib import Path
from typing import Iterable


def read_csv(path: Path) -> list[dict[str, str]]:
    with path.open(newline='', encoding='utf-8') as stream:
        reader = csv.DictReader(stream)
        if not reader.fieldnames:
            raise ValueError(f'No CSV header: {path}')
        rows = list(reader)
    if any(None in row or any(value is None for value in row.values()) for row in rows):
        raise ValueError(f'Incomplete or malformed CSV: {path}')
    return rows


def number(value: object) -> float | None:
    if value in ('', None):
        return None
    try:
        result = float(value)
    except (TypeError, ValueError) as error:
        raise ValueError(f'Invalid numeric observation: {value!r}') from error
    return result if math.isfinite(result) else None


def quantile(values: list[float], q: float) -> float | None:
    if not values:
        return None
    ordered = sorted(values)
    position = (len(ordered) - 1) * q
    low, high = math.floor(position), math.ceil(position)
    return ordered[low] + (ordered[high] - ordered[low]) * (position - low)


def stats(values: Iterable[float | None]) -> dict:
    inputs = list(values)
    valid = [value for value in inputs if value is not None and math.isfinite(value)]
    return dict(observed=len(valid), unavailable=len(inputs)-len(valid),
                mean=statistics.mean(valid) if valid else None,
                median=statistics.median(valid) if valid else None,
                p95=quantile(valid, .95), p99=quantile(valid, .99),
                maximum=max(valid) if valid else None)


def write_csv(path: Path, rows: list[dict], fields: list[str] | None = None) -> None:
    names = fields or list(dict.fromkeys(key for row in rows for key in row)) or ['no_observations']
    with path.open('w', newline='', encoding='utf-8') as stream:
        writer = csv.DictWriter(stream, fieldnames=names)
        writer.writeheader()
        writer.writerows(rows)


def table(rows: list[dict], limit: int = 20) -> str:
    if not rows:
        return '<p>No valid observations in this category.</p>'
    keys = list(rows[0])
    def cell(value):
        if value is None:
            return 'not measured'
        if isinstance(value, float):
            return f'{value:.5f}'
        return str(value)
    return '<table><thead><tr>' + ''.join('<th>'+html.escape(key)+'</th>' for key in keys) + '</tr></thead><tbody>' + ''.join('<tr>'+''.join('<td>'+html.escape(cell(row.get(key)))+'</td>' for key in keys)+'</tr>' for row in rows[:limit]) + '</tbody></table>'


def analyze_detail(source: Path, destination: Path | None = None, *, include_first: bool = False,
                   compare: Path | None = None) -> dict[str, Path]:
    source = source.resolve()
    destination = (destination or source.parent).resolve()
    destination.mkdir(parents=True, exist_ok=True)
    raw = read_csv(source)
    if not raw or any(int(row.get('schema_version', '0')) != 2 for row in raw):
        raise ValueError('The detailed analyzer requires a complete schema-2 capture')
    metadata_path = source.parent / 'native-performance-meta.json'
    meta = json.loads(metadata_path.read_text(encoding='utf-8'))
    ids = {int(row['capture_id']) for row in raw}
    if ids != {int(meta.get('capture_id', -1))} or not meta.get('complete') or meta.get('samples') != len(raw):
        raise ValueError('Capture manifest and flat CSV do not match; copy all capture files together')
    by_sequence = {int(row['capture_sequence']): row for row in raw}
    if len(by_sequence) != len(raw):
        raise ValueError('Duplicate capture sequence')
    details = read_csv(source.parent/'native-performance-frames.csv')
    if {(int(row['sequence']), int(row['frame'])) for row in details} != {(seq, int(row['frame'])) for seq, row in by_sequence.items()}:
        raise ValueError('Detailed frame identities do not match the raw capture')
    detail_by_sequence = {int(row['sequence']): row for row in details}
    sequences = sorted(by_sequence)
    excluded = [] if include_first else [sequences[0]]
    sequences = [seq for seq in sequences if seq not in excluded]
    if not sequences:
        raise ValueError('No samples remain after excluding the capture-start sample')
    selected = set(sequences)
    rows = [by_sequence[seq] for seq in sequences]
    observations = [detail_by_sequence[seq] for seq in sequences]
    warnings = []
    names = meta.get('shader_names', {})
    def shader_name(identity):
        return names.get(identity.upper().zfill(16), identity)
    def selected_rows(name):
        result = read_csv(source.parent/name)
        if any(int(row['sequence']) not in by_sequence or int(row['frame']) != int(by_sequence[int(row['sequence'])]['frame']) for row in result):
            raise ValueError(f'Mismatched frame identity in {name}')
        return [row for row in result if int(row['sequence']) in selected]
    cpu = selected_rows('native-performance-cpu.csv')
    shaders = selected_rows('native-performance-cpu-shaders.csv')
    gpu = selected_rows('native-performance-gpu-passes.csv')
    slices = selected_rows('native-performance-gpu-slices.csv')
    groups = collections.defaultdict(list)
    by_frame_cpu = collections.defaultdict(list)
    for row in cpu:
        groups[(row['phase'], row['operation'])].append(row)
        by_frame_cpu[int(row['sequence'])].append(row)
    cpu_rank = []
    for (phase, operation), group in groups.items():
        measured = {int(row['sequence']): row for row in group}
        values = [number(measured[seq]['self_ms']) if seq in measured else 0.0 for seq in sequences]
        valid_cpu = [int(detail_by_sequence[seq]['cpu_scope_clock_reads']) > 0 for seq in sequences]
        self_stats = stats(value if valid else None for value, valid in zip(values, valid_cpu))
        inclusive = stats(number(measured[seq]['inclusive_ms']) if seq in measured else (0.0 if valid_cpu[i] else None) for i, seq in enumerate(sequences))
        worst = max(group, key=lambda row: number(row['max_call_ms']) or 0)
        cpu_rank.append(dict(phase=phase, operation=operation,
            self_median_ms=self_stats['median'], self_p95_ms=self_stats['p95'], self_p99_ms=self_stats['p99'],
            inclusive_median_ms=inclusive['median'], max_call_ms=number(worst['max_call_ms']),
            calls=sum(int(row['calls']) for row in group), worst_frame=int(worst['frame']),
            worst_command=int(worst['worst_command']), vertex_shader=shader_name(worst['vertex_shader']),
            pixel_shader=shader_name(worst['pixel_shader'])))
    cpu_rank.sort(key=lambda row: row['self_median_ms'] or 0, reverse=True)
    shader_groups = collections.defaultdict(list)
    for row in shaders:
        shader_groups[(row['render_phase'], row['vertex_shader'], row['pixel_shader'])].append(row)
    shader_rank = []
    for (phase, vs, ps), group in shader_groups.items():
        mapped = {int(row['sequence']): row for row in group}
        timing = stats(number(mapped[seq]['inclusive_ms']) if seq in mapped else 0.0 for seq in sequences)
        worst = max(group, key=lambda row: number(row['max_command_ms']) or 0)
        shader_rank.append(dict(render_phase=phase, vertex_shader=shader_name(vs), pixel_shader=shader_name(ps),
            command_cpu_median_ms=timing['median'], command_cpu_p95_ms=timing['p95'],
            max_command_cpu_ms=number(worst['max_command_ms']), worst_frame=int(worst['frame']),
            worst_command=int(worst['worst_command']), commands=sum(int(row['commands']) for row in group)))
    shader_rank.sort(key=lambda row: row['command_cpu_median_ms'] or 0, reverse=True)
    # GPU slices and detailed regions overlap conceptually. Rank them separately.
    gpu_groups = collections.defaultdict(list)
    for row in slices:
        gpu_groups[row['range']].append(row)
    gpu_rank = []
    for label, group in gpu_groups.items():
        per_frame = collections.defaultdict(list)
        for row in group:
            if row['status'] != 'unavailable':
                per_frame[int(row['sequence'])].append(number(row['duration_ms']))
        sums = [sum(value for value in values if value is not None) for values in per_frame.values()]
        t = stats(sums)
        gpu_rank.append(dict(range=label, observed_frames=len(sums),
            unavailable_slices=sum(row['status']=='unavailable' for row in group),
            collapsed_slices=sum(row['status']=='collapsed-or-zero' for row in group),
            median_per_observed_frame_ms=t['median'], p95_per_observed_frame_ms=t['p95'],
            max_observed_frame_ms=t['maximum']))
    gpu_rank.sort(key=lambda row: row['median_per_observed_frame_ms'] or 0, reverse=True)
    pass_groups = collections.defaultdict(list)
    for row in gpu:
        mixed = int(row['pass_keys']) > 1
        key = (row['range'], row['classification'], row['detail'], row['render_phase'], row['reflection_family'],
               'mixed' if mixed else row['vertex_shader'], 'mixed' if mixed else row['pixel_shader'])
        pass_groups[key].append(row)
    pass_rank = []
    for key, group in pass_groups.items():
        per_frame = collections.defaultdict(list)
        for row in group:
            if row['status'] != 'unavailable':
                per_frame[int(row['sequence'])].append(number(row['gpu_ms']))
        t = stats(sum(v for v in values if v is not None) for values in per_frame.values())
        pass_rank.append(dict(range=key[0], classification=key[1], detail=key[2], render_phase=key[3],
            reflection_family=key[4], vertex_shader=shader_name(key[5]), pixel_shader=shader_name(key[6]),
            observed_frames=len(per_frame), unavailable_regions=sum(r['status']=='unavailable' for r in group),
            collapsed_regions=sum(r['status']=='collapsed-or-zero' for r in group),
            median_per_observed_frame_ms=t['median'], p95_per_observed_frame_ms=t['p95']))
    pass_rank.sort(key=lambda row: row['median_per_observed_frame_ms'] or 0, reverse=True)
    # Check exclusive accounting against each measured root span.
    accounting_errors = []
    for seq in sequences:
        group = by_frame_cpu[seq]
        root = sum(number(row['inclusive_ms']) or 0 for row in group if row['operation']=='publish-frame')
        self_sum = sum(number(row['self_ms']) or 0 for row in group)
        if group and not math.isclose(root, self_sum, rel_tol=1e-6, abs_tol=1e-6):
            accounting_errors.append(dict(frame=int(by_sequence[seq]['frame']), root_ms=root, self_sum_ms=self_sum))
    if accounting_errors:
        warnings.append('Some CPU scope trees fail self-time reconciliation; inspect accounting_errors before attributing them.')
    totals = {}
    for key in ('cpu_invalid_scopes','cpu_stack_overflow','shader_keys_overflow','cpu_events_omitted',
                'gpu_dropped_boundaries','gpu_dropped_regions','paint_observations_overwritten'):
        totals[key] = sum(int(row[key]) for row in observations)
    for key in ('cpu_invalid_scopes','cpu_stack_overflow','shader_keys_overflow','gpu_dropped_boundaries','gpu_dropped_regions'):
        if totals[key]: warnings.append(f'{key}={totals[key]}: detail coverage is incomplete.')
    if any(int(row['gpu_queries']) > int(row['gpu_query_budget']) for row in observations):
        warnings.append('A frame exceeded its declared query budget.')
    if any(row['status']=='collapsed-or-zero' for row in slices):
        warnings.append('GPU timestamps collapsed or measured zero in some intervals; do not interpret every zero as a free pass.')
    if not cpu:
        warnings.append('Detailed CPU capture was disabled or unavailable.')
    if not any(int(row['gpu_detailed']) for row in observations):
        warnings.append('GPU mode is coarse; per-pass timings are unavailable.')
    metrics = {key: stats(number(row.get(key)) for row in rows) for key in rows[0] if key.endswith('_ms')}
    transport_fields = ['producer_capture_sum_ms','capture_lock_sum_ms','queue_lock_sum_ms','backpressure_sum_ms',
        'queue_dwell_max_ms','worker_assembly_ms','worker_idle_ms','internal_flush_ms',
        'clock_pair_floor_estimate_ms','query_readback_ms','profiler_snapshot_ms','paint_total_ms']
    transport = {key: stats(number(row[key]) for row in observations) for key in transport_fields}
    memory = []
    for key in rows[0]:
        if not key.startswith('counter_'):
            continue
        values = [number(row[key]) for row in rows]
        if any(value is None for value in values):
            continue
        semantics = meta.get('counter_semantics', {}).get(key.removeprefix('counter_').replace('_','-'), 'per-sample')
        delta = values[-1]-values[0]
        item = dict(counter=key, semantics=semantics, first=values[0], last=values[-1], peak=max(values), change=delta)
        if semantics == 'process-cumulative':
            diffs = [b-a for a,b in zip(values, values[1:])]
            item['resets'] = sum(value < 0 for value in diffs)
            item['positive_increment_total'] = sum(value for value in diffs if value >= 0)
        memory.append(item)
    frame_rank = []
    for seq in sequences:
        row, detail = by_sequence[seq], detail_by_sequence[seq]
        frame_ops = by_frame_cpu[seq]
        worst = max(frame_ops, key=lambda item: number(item['self_ms']) or 0, default=None)
        frame_rank.append(dict(frame=int(row['frame']), sequence=seq,
            native_interval_ms=number(row.get('cpu_frame_interval_ms')), gpu_ms=number(row.get('gpu_frame_ms')),
            publish_cpu_ms=number(row.get('cpu_publish_ms')), largest_self_operation=worst['operation'] if worst else None,
            largest_self_phase=worst['phase'] if worst else None, largest_self_ms=number(worst['self_ms']) if worst else None,
            worst_command=int(worst['worst_command']) if worst else None,
            queue_dwell_max_ms=number(detail['queue_dwell_max_ms']),
            dropped_gpu_boundaries=int(detail['gpu_dropped_boundaries']),
            paint_guest_frame=int(detail['paint_guest_frame'])))
    frame_rank.sort(key=lambda row: row['native_interval_ms'] or 0, reverse=True)
    summary = dict(schema_version=2, source=str(source), metadata=meta, raw_samples=len(raw),
        analyzed_samples=len(rows), excluded_sequences=excluded, warnings=warnings, coverage=totals,
        accounting_errors=accounting_errors, broad_metrics=metrics, cpu_self_ranking=cpu_rank,
        cpu_shader_command_ranking=shader_rank, gpu_slice_ranking=gpu_rank, gpu_pass_ranking=pass_rank,
        transport_metrics=transport, counters=memory, slow_frames=frame_rank,
        accounting={
          'CPU': 'Rank self time across one render-owner scope tree. Inclusive parents are not additional costs.',
          'CPU shader groups': 'Inclusive recording cost grouped by title shader identity; not GPU shader execution.',
          'GPU': 'Approximate queue intervals. Flat slices and detailed regions are alternative views, not additive.',
          'Producer': 'Per-command capture/dwell sums overlap. Worker assembly can include internal flush time.',
          'Presenter': 'Correlated CPU paint observations; no physical scanout or calibrated CPU/GPU clock alignment.',
          'Counter': 'Process-cumulative values require differences. Resident gauges are not allocation traffic.'})
    outputs = {name: destination/filename for name,filename in {
        'summary':'native-performance-analysis.json', 'cpu':'native-performance-cpu-ranking.csv',
        'cpu_shaders':'native-performance-cpu-shader-ranking.csv', 'gpu':'native-performance-gpu-ranking.csv',
        'gpu_passes':'native-performance-gpu-pass-ranking.csv', 'slow_frames':'native-performance-slow-frames.csv',
        'report':'native-performance-report.html'}.items()}
    for label,records in [('cpu',cpu_rank),('cpu_shaders',shader_rank),('gpu',gpu_rank),('gpu_passes',pass_rank),('slow_frames',frame_rank)]:
        write_csv(outputs[label],records)
    if compare:
        baseline = json.loads(compare.read_text(encoding='utf-8'))
        old = {(item['phase'],item['operation']):item for item in baseline.get('cpu_self_ranking', [])}
        comparison=[]
        for row in cpu_rank:
            prior=old.get((row['phase'],row['operation']))
            if prior and prior['self_median_ms'] is not None and row['self_median_ms'] is not None:
                comparison.append(dict(phase=row['phase'],operation=row['operation'],before_ms=prior['self_median_ms'],after_ms=row['self_median_ms'],delta_ms=row['self_median_ms']-prior['self_median_ms']))
        summary['comparison']={'baseline':str(compare),'note':'Only compare matched scenes, settings and profiling modes. This table does not establish causation.','cpu_self':comparison}
    outputs['summary'].write_text(json.dumps(summary,indent=2,allow_nan=False)+'\n',encoding='utf-8')
    sections = [('CPU self time: descending median',cpu_rank),('CPU command cost by shader',shader_rank),
        ('GPU queue slices: approximate',gpu_rank),('GPU pass drill-down: separate view',pass_rank),('Slowest native-frame intervals',frame_rank)]
    report='<!doctype html><html><head><meta charset="utf-8"><title>Native renderer profile</title><style>body{font:15px system-ui;max-width:1600px;margin:32px auto;padding:0 20px}table{border-collapse:collapse;display:block;overflow-x:auto}td,th{text-align:left;padding:8px;border-bottom:1px solid #999}th{white-space:nowrap}section{margin-top:32px}code{overflow-wrap:anywhere}</style></head><body><h1>Native renderer profile</h1><p><code>'+html.escape(str(source))+'</code></p><p>'+str(len(rows))+' analyzed frames; '+str(len(excluded))+' capture-start sample excluded.</p>'
    report+='<h2>Accounting</h2>'+''.join('<p><strong>'+html.escape(k)+': </strong>'+html.escape(v)+'</p>' for k,v in summary['accounting'].items())
    report+='<h2>Coverage</h2>'+''.join('<p>'+html.escape(w)+'</p>' for w in warnings)
    report+=''.join('<section><h2>'+html.escape(title)+'</h2>'+table(records)+'</section>' for title,records in sections)
    report+='</body></html>'
    outputs['report'].write_text(report,encoding='utf-8')
    return outputs


def main() -> int:
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('csv',type=Path)
    parser.add_argument('--output-directory',type=Path)
    parser.add_argument('--include-first',action='store_true')
    parser.add_argument('--compare',type=Path,help='Prior schema-2 analysis JSON from a matched scene')
    args=parser.parse_args()
    for label,path in analyze_detail(args.csv,args.output_directory,include_first=args.include_first,compare=args.compare).items():
        print(f'{label}: {path}')
    return 0

if __name__=='__main__':
    raise SystemExit(main())
