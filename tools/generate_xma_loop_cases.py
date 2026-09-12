#!/usr/bin/env python3
"""Generate controlled loop layouts and packet-edge cases from local test data."""
from pathlib import Path
import argparse
import json

PAYLOAD_BITS = (2048 - 4) * 8

def unpack(raw):
    start, end = raw[0] >> 5, (raw[0] >> 2) & 7
    bits = [(b >> j) & 1 for b in raw[1:] for j in range(7, -1, -1)]
    return bits[start:len(bits) - end if end else len(bits)]

def pack(raw):
    padding = (-len(raw)) % 8
    bits = raw + [0] * padding
    return bytes([padding << 2]) + bytes(sum(bits[i+j] << (7-j) for j in range(8)) for i in range(0, len(bits), 8))

def physical_bit(payload_bit):
    return payload_bit // PAYLOAD_BITS * 16384 + 32 + payload_bit % PAYLOAD_BITS

def stream(directory, name, template, prefix, count=7):
    bits = [0] * prefix
    positions = []
    raw_frames = []
    for i in range(count):
        frame = unpack(template)
        frame[-1] = int(i + 1 < count)
        positions.append(len(bits))
        raw_frames.append(frame)
        bits.extend(frame)
    packets = (len(bits) + PAYLOAD_BITS - 1) // PAYLOAD_BITS
    bits.extend([0] * (packets * PAYLOAD_BITS - len(bits)))
    binary = bytearray()
    for packet in range(packets):
        starts = [p % PAYLOAD_BITS for p in positions if p // PAYLOAD_BITS == packet]
        first = starts[0] if starts else 32767
        header = bytes([((packet & 15) << 4) | 8 | ((first >> 13) & 3), (first >> 5) & 255, (first & 31) << 3, 0])
        chunk = bits[packet * PAYLOAD_BITS:(packet + 1) * PAYLOAD_BITS]
        data = bytes(sum(chunk[i+j] << (7-j) for j in range(8)) for i in range(0, len(chunk), 8))
        binary.extend(header + data)
    filename = name + '.xma'
    (directory / filename).write_bytes(binary)
    lines = []
    for i, frame in enumerate(raw_frames):
        part = name + f'.f{i}.raw'
        (directory / part).write_bytes(pack(frame))
        lines.append(f'{physical_bit(positions[i])} {len(frame)} {part}')
    (directory / (filename + '.frames')).write_text('\n'.join(lines) + '\n')
    return filename, [physical_bit(p) for p in positions]

def interleave(directory, source, name, stride):
    """Insert unrelated packets between one stream's packets.

    Packet skip counts, compressed-frame locations and loop bit offsets must
    follow the selected stream. Encoded frame payloads are left unchanged.
    """
    original = (directory / source).read_bytes()
    if len(original) % 2048 or stride not in (2, 3):
        raise ValueError('invalid interleave fixture')
    result = bytearray()
    decoy = bytes([0x0b, 0xff, 0xf8, 0xff]) + bytes(2044)
    for packet in range(len(original) // 2048):
        selected = bytearray(original[packet * 2048:(packet + 1) * 2048])
        selected[3] = stride - 1
        result.extend(selected)
        for unused in range(stride - 1):
            result.extend(decoy)
    filename = name + '.xma'
    (directory / filename).write_bytes(result)
    positions = []
    entries = []
    for line in (directory / (source + '.frames')).read_text().splitlines():
        bit, length, raw = line.split()
        bit = int(bit)
        relocated = bit // 16384 * stride * 16384 + bit % 16384
        positions.append(relocated)
        entries.append(f'{relocated} {length} {raw}')
    (directory / (filename + '.frames')).write_text('\n'.join(entries) + '\n')
    return filename, positions

def generate(directory):
    lines = []
    # Vary real audio loop boundaries, including same-frame loops and a skipped
    # full priming frame, without changing any encoded sample content.
    source = 'FAST_2_START_DIST.xma'
    positions = [int(l.split()[0]) for l in (directory / (source + '.frames')).read_text().splitlines()]
    for skip in range(5):
        for end in range(4):
            for rate in [44100]:
                lines.append(f'REAL_LAYOUT_s{skip}_e{end}_r{rate} {rate} 1 {positions[1]} {positions[6]} {skip} {end} 0 {source}')
    for skip in range(4):
        for end in range(skip, 4):
            lines.append(f'REAL_SINGLE_FRAME_s{skip}_e{end} 44100 1 {positions[3]} {positions[3]} {skip} {end} 0 {source}')
    for channels in [1, 2]:
        template = (directory / f'silent-{channels}.raw').read_bytes()
        filename, positions = stream(directory, f'SYNTHETIC_{channels}', template, 0)
        for rate in [24000, 32000, 44100, 48000]:
            for skip in range(5):
                for end in range(4):
                    lines.append(f'SYNTHETIC_c{channels}_r{rate}_s{skip}_e{end} {rate} {channels} {positions[1]} {positions[5]} {skip} {end} 0 {filename}')
        # Every possible split of a 15-bit XMA frame header. Start the previous
        # frame earlier so sequential packet traversal must discover the split.
        frame_length = len(unpack(template))
        for remaining in range(1, 15):
            prefix = PAYLOAD_BITS - remaining - frame_length
            filename, positions = stream(directory, f'SPLIT_c{channels}_b{remaining}', template, prefix)
            lines.append(f'SPLIT_c{channels}_b{remaining} 48000 {channels} {positions[0]} {positions[5]} 1 2 0 {filename}')
    # Nonzero retail audio and split-header stereo streams with unrelated
    # packets between stream packets. This isolates packet selection from
    # sample reconstruction and prevents an all-silence fixture masking a skip.
    for row in (directory / 'manifest.tsv').read_text().splitlines():
        source_name, rate, channels, start, end, skip, end_sub, samples, source = row.split()
        for stride in (2, 3):
            name = f'INTERLEAVED_{source_name}_x{stride}'
            filename, positions = interleave(directory, source, name, stride)
            remap = lambda bit: int(bit) // 16384 * stride * 16384 + int(bit) % 16384
            lines.append(f'{name} {rate} {channels} {remap(start)} {remap(end)} {skip} {end_sub} {samples} {filename}')
    for channels in (1, 2):
        for remainder in (1, 7, 14):
            source = f'SPLIT_c{channels}_b{remainder}.xma'
            for stride in (2, 3):
                name = f'INTERLEAVED_SPLIT_c{channels}_b{remainder}_x{stride}'
                filename, positions = interleave(directory, source, name, stride)
                lines.append(f'{name} 48000 {channels} {positions[0]} {positions[5]} 1 2 0 {filename}')
    (directory / 'extended.tsv').write_text('\n'.join(lines) + '\n')
    report = {'wave_layouts':len(lines), 'real_boundary_layouts':sum(line.startswith('REAL_') for line in lines), 'synthetic_rates':[24000,32000,44100,48000], 'channels':[1,2], 'split_header_positions':list(range(1,15)), 'interleaved_layouts':sum(line.startswith('INTERLEAVED_') for line in lines), 'source':'synthetic mono/stereo frames and all eight local FAST_2 waves; packet/loop-layout variants, not a gameplay recording'}
    (directory / 'extended.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report, indent=2))

if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('directory', type=Path)
    generate(p.parse_args().directory)
