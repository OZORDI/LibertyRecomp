#!/usr/bin/env python3
"""Generate small valid PNGs and boundary lengths for bounded icon decoding."""
import argparse
from pathlib import Path
import struct
import zlib

parser = argparse.ArgumentParser()
parser.add_argument('--output', required=True, type=Path)
args = parser.parse_args()

def chunk(kind, value):
    payload = kind + value
    return struct.pack('>I', len(value)) + payload + struct.pack('>I', zlib.crc32(payload))

def png(width, height):
    pixel = bytes([17, 34, 51, 255])
    raw = (b'\0' + pixel * width) * height
    return (b'\x89PNG\r\n\x1a\n' +
            chunk(b'IHDR', struct.pack('>IIBBBBB', width, height, 8, 6, 0, 0, 0)) +
            chunk(b'IDAT', zlib.compress(raw)) + chunk(b'IEND', b''))

lines = ['#pragma once', '#include <cstddef>', '#include <cstdint>']
for name, image in [('kSmallIcon', png(2, 1)), ('kTooWideIcon', png(2048, 1))]:
    lines.append('inline constexpr uint8_t ' + name + '[] = {' +
                 ','.join(str(value) for value in image) + '};')
lines.append(f'inline constexpr size_t kSmallRgbaBytes = {2 * 1 * 4};')
lines.append(f'inline constexpr size_t kOversizedInputLength = {2 ** 31};')
args.output.parent.mkdir(parents=True, exist_ok=True)
args.output.write_text('\n'.join(lines) + '\n')
