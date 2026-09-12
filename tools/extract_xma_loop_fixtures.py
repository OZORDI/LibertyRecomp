#!/usr/bin/env python3
"""Read the installed FAST_2 bank into local regression fixtures; never edits game data."""
from pathlib import Path
import argparse
import hashlib
import json
import struct

NAMES = {0x3470113f:'REVS_OFF', 0x35218201:'START_DIST', 0x3ac956e6:'ENG_IDLE_LOOP', 0x68d5649a:'EX_IDLE_LOOP', 0x7240b2aa:'EXHAUST_LOW', 0xc0ff4fd6:'ENGINE_HIGH', 0xdb46437d:'EXHAUST_HIGH', 0xe7a567b3:'ENGINE_LOW'}

def frames(data: bytes):
    packet_bits = 2048 * 8
    def read(at, n):
        bits = []
        for _ in range(n):
            if at % packet_bits == 0:
                at += 32
            if at >= len(data) * 8:
                raise ValueError('frame extends beyond source')
            bits.append((data[at // 8] >> (7 - at % 8)) & 1)
            at += 1
        return bits, at
    result = []
    for packet in range(len(data) // 2048):
        header = data[packet * 2048:packet * 2048 + 4]
        if header[3] == 255:
            continue
        if header[3] != 0:
            raise ValueError('interleaved bank requires stream-aware extraction')
        first = ((header[0] & 3) << 13) | (header[1] << 5) | (header[2] >> 3)
        at = packet * packet_bits + 32 + first
        end = (packet + 1) * packet_bits
        if at >= end:
            continue
        for _ in range(packet_bits // 15):
            if at >= end:
                break
            length_bits, _ = read(at, 15)
            n = int(''.join(map(str, length_bits)), 2)
            if n in (0, 32767):
                break
            bits, following = read(at, n)
            padding = (-len(bits)) % 8
            raw = bytes([padding << 2]) + int(''.join(map(str, bits)) + '0' * padding, 2).to_bytes((n + padding) // 8, 'big')
            result.append((at, n, raw))
            at = following
            if not bits[-1]:
                break
    if len({a for a, _, _ in result}) != len(result):
        raise ValueError('duplicate encoded frame')
    return result

def extract(archive: Path, output: Path):
    with archive.open('rb') as f:
        f.seek(2021376)
        bank = f.read(100352)
    if hashlib.sha256(bank).hexdigest() != '095ce074d81651663e1e4a13d635e558d5c71dcac9415a370241bb6f8c3309a5':
        raise ValueError('FAST_2 bank differs from verified source; locate the correct entry first')
    output.mkdir(parents=True, exist_ok=True)
    manifest, records = [], []
    for i in range(8):
        off, h, size = struct.unpack_from('>QII', bank, 28 + i * 16)
        meta = bank[156 + off:156 + off + size]
        pos, hash_, length, samples = struct.unpack_from('>QIII', meta)
        start, end, packed = struct.unpack_from('>III', meta, 40)
        if hash_ != h or h not in NAMES:
            raise ValueError('invalid waveform record')
        payload = bank[2048 + pos:2048 + pos + length]
        if len(payload) != length:
            raise ValueError('out-of-bank waveform')
        decoded = frames(payload)
        endidx = next(j for j, x in enumerate(decoded) if x[0] == end)
        skip, end_sub = packed & 15, packed >> 4
        measured = len(decoded) * 512 - skip * 128 - (3 - end_sub) * 128
        if samples != measured:
            raise ValueError(('sample-boundary mismatch', NAMES[h], samples, measured))
        name = 'FAST_2_' + NAMES[h]
        filename = name + '.xma'
        (output / filename).write_bytes(payload)
        lines = []
        for j, (at, n, raw) in enumerate(decoded):
            part = f'{name}.f{j}.raw'
            (output / part).write_bytes(raw)
            lines.append(f'{at} {n} {part}')
        (output / (filename + '.frames')).write_text('\n'.join(lines) + '\n')
        rate = 24000 if h == 0x68d5649a else 44100
        manifest.append(f'{name} {rate} 1 {start} {end} {skip} {end_sub} {samples} {filename}')
        records.append(dict(name=name, sha256=hashlib.sha256(payload).hexdigest(), frames=len(decoded), end_frame=endidx, loop_start=start, loop_end=end, skip=skip, end_subframe=end_sub, playable_samples=samples, rate=rate))
    (output / 'manifest.tsv').write_text('\n'.join(manifest) + '\n')
    (output / 'provenance.json').write_text(json.dumps({'archive':str(archive), 'bank_offset':2021376, 'bank_size':100352, 'bank_sha256':hashlib.sha256(bank).hexdigest(), 'waves':records}, indent=2) + '\n')
    print(json.dumps(records, indent=2))

if __name__ == '__main__':
    a = argparse.ArgumentParser(description=__doc__)
    a.add_argument('archive', type=Path)
    a.add_argument('output', type=Path)
    v = a.parse_args()
    extract(v.archive, v.output)
