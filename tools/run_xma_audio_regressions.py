#!/usr/bin/env python3
"""Run XMA looping, decoder-lifetime and CoreAudio-buffer regressions on macOS.

Build the normal macos-release LibertyRecomp target first. This script links to
that exact runtime, reads the installed FAST_2 bank, and writes all test data to
--output. It neither starts the game nor alters game assets or audio routing.
"""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shlex
import subprocess
import sys
import time


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--build', type=Path, default=Path('out/build/macos-release'))
    parser.add_argument('--archive', type=Path, default=Path.home() / 'Library/Application Support/LibertyRecomp/game/xbox360/audio/sfx/streamed_vehicles.rpf')
    args = parser.parse_args()
    root = Path(__file__).resolve().parent.parent
    os.chdir(root)
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    fixture_dir = output / 'fixtures'
    sdk = root / 'glue/rexglue-sdk-main'
    libraries = sdk / 'out/mac-arm64'
    runtime = libraries / 'librexruntime.dylib'
    runtime_hash = hashlib.sha256(runtime.read_bytes()).hexdigest()
    cache = root / 'LibertyRecompLib/shader/shader_cache.cpp'
    cache_hash = hashlib.sha256(cache.read_bytes()).hexdigest()
    ninja = (args.build / 'build.ninja').read_text()
    start = ninja.index('build glue/rexglue-sdk/src/audio/CMakeFiles/rexaudio.dir/xma_context.cpp.o:')
    end = ninja.index('\nbuild ', start + 6)
    includes = shlex.split(re.search(r'  INCLUDES = (.*)', ninja[start:end])[1])
    common = ['clang++', '-std=c++23', '-O2', '-DSPDLOG_COMPILED_LIB', '-DSPDLOG_FMT_EXTERNAL', *includes]
    env = {k:v for k,v in os.environ.items() if not k.startswith('REX_AUDIO_HANDOFF')}
    records = []

    def run(name: str, command: list[str], timeout: int = 90) -> str:
        began = time.monotonic()
        result = subprocess.run(command, cwd=root, env=env, capture_output=True, text=True, timeout=timeout)
        (output / (name + '.log')).write_text('COMMAND ' + repr(command) + '\n' + result.stdout + '\n' + result.stderr)
        row = {'name':name, 'command':command, 'exit':result.returncode, 'seconds':time.monotonic()-began}
        records.append(row)
        if result.returncode:
            raise RuntimeError(f'{name} failed; see {output / (name + ".log")}')
        print(name + ': passed', flush=True)
        return result.stdout

    try:
        loop = output / 'test_xma_loop_playback'
        core = output / 'test_coreaudio_playback_contract'
        capacity = output / 'test_xma_output_capacity'
        for filename, binary in [('test_xma_loop_playback.cpp',loop), ('test_coreaudio_playback_contract.cpp',core), ('test_xma_output_capacity.cpp',capacity)]:
            run('compile-' + binary.name, common + ['-fno-access-control',str(root/'tools'/filename),str(runtime),'-Wl,-rpath,'+str(libraries),'-o',str(binary)])
        fixture = output / 'xma_fixture.o'
        run('compile-reset-fixture', ['clang','-std=c11','-O2','-DHAVE_AV_CONFIG_H','-I'+str(sdk/'thirdparty/FFmpeg'),'-c',str(root/'tools/xma_reset_fixture.c'),'-o',str(fixture)])
        for source in ['test_xma_decoder_reset','generate_xma_silent_fixture']:
            run('compile-'+source, ['clang++','-std=c++20','-O2','-I'+str(sdk/'thirdparty/FFmpeg'),'-I'+str(sdk/'src/audio'),str(root/'tools'/(source+'.cpp')),str(fixture),str(libraries/'liblibavcodec.a'),str(libraries/'liblibavutil.a'),'-pthread','-framework','CoreFoundation','-framework','Security','-o',str(output/source)])
        run('decoder-reset', [str(output/'test_xma_decoder_reset')])
        run('extract-fast2', [sys.executable,str(root/'tools/extract_xma_loop_fixtures.py'),str(args.archive),str(fixture_dir)])
        run('synthetic-frames', [str(output/'generate_xma_silent_fixture'),str(fixture_dir)])
        run('synthetic-layouts', [sys.executable,str(root/'tools/generate_xma_loop_cases.py'),str(fixture_dir)])
        actual = run('retail-playback', [str(loop),str(fixture_dir/'manifest.tsv')])
        extended = run('extended-playback', [str(loop),str(fixture_dir/'extended.tsv')])
        callback = run('coreaudio-callback', [str(core)])
        capacity_result = run('pending-output-capacity', [str(capacity)])
        lowlevel = output / 'test_audio_lowlevel'
        run('compile-audio-lowlevel', common + ['-I'+str(sdk/'thirdparty/catch2/src'),'-I'+str(args.build.resolve()/'glue/rexglue-sdk/thirdparty/catch2/generated-includes'),*[str(sdk/'tests/unit/audio'/name) for name in ['audio_conversion_test.cpp','audio_clock_test.cpp','audio_ring_test.cpp']],str(libraries/'libCatch2Main.a'),str(libraries/'libCatch2.a'),str(runtime),'-Wl,-rpath,'+str(libraries),'-o',str(lowlevel)])
        low_result = run('audio-lowlevel', [str(lowlevel)])
        counts = []
        for log in [actual,extended]:
            row = next(line for line in log.splitlines() if line.startswith('RESULT '))
            counts.append({k:int(v) for k,v in re.findall(r'(\w+)=(\d+)',row)})
        summary = {'status':'passed', 'runtime':str(runtime), 'runtime_sha256':runtime_hash,
                   'runtime_unchanged_during_tests':hashlib.sha256(runtime.read_bytes()).hexdigest()==runtime_hash,
                   'shader_cache_unchanged':hashlib.sha256(cache.read_bytes()).hexdigest()==cache_hash,
                   'retail':counts[0], 'extended':counts[1],
                   'total_playback_cases':sum(r['cases'] for r in counts),
                   'total_decoded_frames':sum(r['decoded_frames'] for r in counts),
                   'total_compared_sample_frames':sum(r['compared_samples'] for r in counts),
                   'unique_consume_cases':counts[0]['consume_cases'],
                   'work_cases':sum(r['work_cases'] for r in counts),
                   'callback_result':callback.strip(), 'lowlevel_result':low_result.strip(),
                   'pending_output_capacity_result':capacity_result.strip(),
                   'live_game_reproduction':False, 'live_audio_device_opened':False,
                   'records':records}
        if not summary['runtime_unchanged_during_tests'] or not summary['shader_cache_unchanged']:
            raise RuntimeError('Runtime or shader cache changed during the tests; repeat with stable artifacts')
        (output/'results.json').write_text(json.dumps(summary,indent=2)+'\n')
        print(json.dumps({k:v for k,v in summary.items() if k!='records'},indent=2))
    except Exception as error:
        (output/'failure.json').write_text(json.dumps({'error':str(error),'records':records},indent=2)+'\n')
        raise

if __name__ == '__main__':
    main()
