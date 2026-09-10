#!/usr/bin/env python3
"""Regenerate the checked-in desktop icons from the supplied square PNG.

Run on macOS with ImageMagick and Apple's iconutil. Normal application builds
consume the generated files directly and do not require these conversion tools.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
import tempfile

PNG_SIZES = (16, 24, 32, 48, 64, 128, 256, 512, 1024)
ICO_SIZES = (16, 24, 32, 48, 64, 128, 256)


def run(command: list[str]) -> None:
    subprocess.run(command, check=True, capture_output=True, text=True)


def png_dimensions(path: Path) -> tuple[int, int]:
    data = path.read_bytes()
    if data[:8] != b'\x89PNG\r\n\x1a\n' or data[12:16] != b'IHDR':
        raise ValueError(f'Not a PNG image: {path}')
    return struct.unpack('>II', data[16:24])


def generate(source: Path, repository: Path) -> dict:
    source = source.resolve(strict=True)
    width, height = png_dimensions(source)
    if width != height or width < 1024:
        raise ValueError('Supply a square PNG at least 1024 pixels wide.')
    magick = shutil.which('magick')
    iconutil = shutil.which('iconutil')
    if not magick or not iconutil:
        raise RuntimeError('Regeneration requires ImageMagick and macOS iconutil.')
    assets = repository / 'LibertyRecomp/res/icons'
    mac_icon = repository / 'LibertyRecomp/res/macos/game_icon.icns'
    with tempfile.TemporaryDirectory(prefix='liberty-desktop-icons-') as temporary:
        stage = Path(temporary)
        pngs = stage / 'png'
        pngs.mkdir()
        for size in PNG_SIZES:
            output = pngs / f'{size}.png'
            run([magick, str(source), '-colorspace', 'sRGB', '-filter', 'Lanczos',
                 '-resize', f'{size}x{size}', '-depth', '8', '-strip',
                 '-define', 'png:exclude-chunks=date,time', str(output)])
            if png_dimensions(output) != (size, size):
                raise RuntimeError(f'Incorrect generated size: {output}')
        iconset = stage / 'game_icon.iconset'
        iconset.mkdir()
        for logical in (16, 32, 128, 256, 512):
            for scale in (1, 2):
                suffix = '@2x' if scale == 2 else ''
                shutil.copyfile(pngs / f'{logical * scale}.png',
                                iconset / f'icon_{logical}x{logical}{suffix}.png')
        run([iconutil, '-c', 'icns', str(iconset), '-o', str(stage / 'game_icon.icns')])
        run([magick, *[str(pngs / f'{size}.png') for size in ICO_SIZES],
             '-alpha', 'on', '-depth', '8', str(stage / 'game_icon.ico')])
        run([magick, str(pngs / '256.png'), '-alpha', 'off', '-type', 'TrueColor',
             '-depth', '8', 'BMP3:' + str(stage / 'game_icon.bmp')])
        # Install only after every conversion succeeded. Preserve the original
        # PNG byte-for-byte as the reproducible source asset.
        (assets / 'png').mkdir(parents=True, exist_ok=True)
        if source != (assets / 'source.png').resolve():
            shutil.copyfile(source, assets / 'source.png')
        for size in PNG_SIZES:
            shutil.copyfile(pngs / f'{size}.png', assets / 'png' / f'{size}.png')
        for name in ('game_icon.ico', 'game_icon.bmp'):
            shutil.copyfile(stage / name, assets / name)
        mac_icon.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(stage / 'game_icon.icns', mac_icon)
    outputs = [assets / 'source.png', assets / 'game_icon.ico', assets / 'game_icon.bmp',
               mac_icon, *[assets / 'png' / f'{size}.png' for size in PNG_SIZES]]
    report = {
        'source_dimensions': [width, height],
        'png_sizes': list(PNG_SIZES),
        'windows_icon_sizes': list(ICO_SIZES),
        'mac_iconset_logical_sizes': [16, 32, 128, 256, 512],
        'mac_iconset_scales': [1, 2],
        'assets': {str(p.relative_to(repository)): hashlib.sha256(p.read_bytes()).hexdigest()
                   for p in outputs},
    }
    (assets / 'manifest.json').write_text(json.dumps(report, indent=2) + '\n')
    return report


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('source', type=Path)
    parser.add_argument('--repository', type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    print(json.dumps(generate(args.source, args.repository.resolve()), indent=2))
