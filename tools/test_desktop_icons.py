#!/usr/bin/env python3
"""Validate desktop icon formats and exercise the Linux packaging CMake module."""
from __future__ import annotations

import argparse
import configparser
import hashlib
import json
from pathlib import Path
import plistlib
import shutil
import struct
import subprocess
import tempfile

APP_ID = 'io.github.ozordi.libertyrecomp'
SIZES = (16, 24, 32, 48, 64, 128, 256, 512, 1024)


def execute(command: list[str]) -> subprocess.CompletedProcess:
    return subprocess.run(command, check=True, capture_output=True)


def dimensions(path: Path) -> tuple[int, int]:
    data = path.read_bytes()
    assert data[:8] == b'\x89PNG\r\n\x1a\n' and data[12:16] == b'IHDR', str(path)
    return struct.unpack('>II', data[16:24])


def rgb(magick: str, image: str) -> bytes:
    return execute([magick, image, '-alpha', 'off', '-depth', '8', 'rgb:-']).stdout


def verify(repository: Path, source: Path | None, mac_app: Path | None) -> dict:
    assets = repository / 'LibertyRecomp/res/icons'
    manifest = json.loads((assets / 'manifest.json').read_text())
    for relative, digest in manifest['assets'].items():
        assert hashlib.sha256((repository / relative).read_bytes()).hexdigest() == digest, relative
    if source:
        assert source.read_bytes() == (assets / 'source.png').read_bytes()
    magick = shutil.which('magick')
    iconutil = shutil.which('iconutil')
    assert magick and iconutil, 'This verification uses ImageMagick and macOS iconutil.'
    for size in SIZES:
        assert dimensions(assets / 'png' / f'{size}.png') == (size, size)
    expected_rgb = {size: rgb(magick, str(assets / 'png' / f'{size}.png')) for size in SIZES}
    assert rgb(magick, str(assets / 'game_icon.bmp')) == expected_rgb[256]
    ico = (assets / 'game_icon.ico').read_bytes()
    reserved, kind, count = struct.unpack_from('<HHH', ico)
    assert reserved == 0 and kind == 1 and count == len(manifest['windows_icon_sizes'])
    frames = []
    for i in range(count):
        width, height, _, _, planes, depth, length, offset = struct.unpack_from('<BBBBHHII', ico, 6 + i * 16)
        width, height = width or 256, height or 256
        assert width == height and offset + length <= len(ico) and length > 0
        assert rgb(magick, f'{assets / "game_icon.ico"}[{i}]') == expected_rgb[width]
        frames.append(width)
    assert frames == manifest['windows_icon_sizes']
    with tempfile.TemporaryDirectory(prefix='liberty-icon-test-') as temporary:
        temporary = Path(temporary)
        iconset = temporary / 'roundtrip.iconset'
        execute([iconutil, '-c', 'iconset', str(repository / 'LibertyRecomp/res/macos/game_icon.icns'),
                 '-o', str(iconset)])
        representations = []
        for logical in manifest['mac_iconset_logical_sizes']:
            for scale in manifest['mac_iconset_scales']:
                suffix = '@2x' if scale == 2 else ''
                image = iconset / f'icon_{logical}x{logical}{suffix}.png'
                actual = logical * scale
                assert dimensions(image) == (actual, actual)
                assert rgb(magick, str(image)) == expected_rgb[actual], str(image)
                representations.append(image.name)
        # Compile the icon directive used by the real Windows RC template.
        compiler = shutil.which('llvm-rc')
        if not compiler:
            local = Path('/opt/homebrew/opt/llvm/bin/llvm-rc')
            compiler = str(local) if local.is_file() else None
        rc_passed = False
        if compiler:
            template = (repository / 'LibertyRecomp/res/win32/res.rc.template').read_text()
            directive = next(line for line in template.splitlines() if ' ICON ' in line)
            rc = temporary / 'icon.rc'
            rc.write_text(directive.replace('@WIN32_ICON_PATH@', str(assets / 'game_icon.ico')) + '\n')
            res = temporary / 'icon.res'
            execute([compiler, '/no-preprocess', '/FO', str(res), str(rc)])
            data = res.read_bytes()
            resource_types = []
            cursor = 0
            for _ in range(len(data)):
                if cursor == len(data):
                    break
                data_size, header_size = struct.unpack_from('<II', data, cursor)
                assert header_size >= 32 and cursor + header_size + data_size <= len(data)
                tag, resource_type = struct.unpack_from('<HH', data, cursor + 8)
                assert tag == 65535
                if data_size:
                    resource_types.append(resource_type)
                cursor = (cursor + header_size + data_size + 3) & ~3
            assert resource_types.count(3) == count and resource_types.count(14) == 1
            rc_passed = True
        # Exercise the real Linux stage/install module on a tiny native fixture;
        # this validates paths without claiming a Linux game cross-build.
        fixture = temporary / 'fixture'
        fixture.mkdir()
        (fixture / 'main.cpp').write_text('int main() { return 0; }\n')
        (fixture / 'CMakeLists.txt').write_text(
            'cmake_minimum_required(VERSION 3.20)\nproject(DesktopIconFixture LANGUAGES CXX)\n'
            'add_executable(IconFixture main.cpp)\n'
            'set_target_properties(IconFixture PROPERTIES OUTPUT_NAME LibertyRecomp)\n'
            f'include("{repository / "cmake/LinuxDesktopIntegration.cmake"}")\n'
            f'liberty_configure_linux_desktop(IconFixture "{repository / "LibertyRecomp/res"}")\n')
        build = temporary / 'build'
        prefix = temporary / 'installed'
        execute(['cmake', '-S', str(fixture), '-B', str(build), '-G', 'Ninja'])
        execute(['cmake', '--build', str(build)])
        execute(['cmake', '--install', str(build), '--prefix', str(prefix), '--component', 'LibertyRecompDesktop'])
        desktop = configparser.ConfigParser(interpolation=None)
        desktop.optionxform = str
        desktop.read(prefix / 'share/applications' / (APP_ID + '.desktop'))
        entry = desktop['Desktop Entry']
        assert entry['Name'] == 'Liberty Recompiled' and entry['Icon'] == APP_ID
        assert entry['Exec'] == entry['TryExec'] == 'LibertyRecomp'
        assert (prefix / 'bin/LibertyRecomp').is_file()
        assert (build / (APP_ID + '.desktop')).is_file()
        for size in SIZES:
            relative = Path(f'share/icons/hicolor/{size}x{size}/apps/{APP_ID}.png')
            expected = (assets / 'png' / f'{size}.png').read_bytes()
            assert (prefix / relative).read_bytes() == expected
            assert (build / relative).read_bytes() == expected
    bundle_passed = False
    if mac_app:
        with (mac_app / 'Contents/Info.plist').open('rb') as stream:
            info = plistlib.load(stream)
        assert info['CFBundleIconFile'] == 'game_icon.icns'
        assert (mac_app / 'Contents/Resources/game_icon.icns').read_bytes() == (
            repository / 'LibertyRecomp/res/macos/game_icon.icns').read_bytes()
        execute(['codesign', '--verify', '--deep', '--strict', str(mac_app)])
        bundle_passed = True
    return {'passed': True, 'source_exact_copy': source is not None,
            'windows_sizes': frames, 'windows_resource_compiled': rc_passed,
            'mac_representations_pixel_equal': representations,
            'sdl_bmp_pixel_equal': True, 'linux_stage_and_install_passed': True,
            'linux_sizes': list(SIZES), 'mac_bundle_and_signature_passed': bundle_passed}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repository', type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument('--source', type=Path)
    parser.add_argument('--mac-app', type=Path)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    report = verify(args.repository.resolve(), args.source, args.mac_app)
    if args.output:
        args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report, indent=2))
