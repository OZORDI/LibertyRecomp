#!/usr/bin/env python3
"""Compile production touch/motion code against controlled host dependencies.

No game launch or production source rewrite. Intended for the macOS/Linux
source-review and input-regression workflow; sparse mmap backs guest globals.
"""
from __future__ import annotations
import argparse
import hashlib
import json
import math
from pathlib import Path
import shutil
import subprocess
import sys

parser = argparse.ArgumentParser()
parser.add_argument('--output', required=True, type=Path)
parser.add_argument('--compiler', default='clang++')
parser.add_argument('--sanitize', action='store_true')
parser.add_argument('--case', action='append', choices=[
    'touch_runtime', 'coordinator_runtime', 'touch_context_runtime', 'touch_layout_runtime',
    'touch_pointer_runtime', 'touch_editor_mouse_runtime', 'touch_icon_runtime', 'motion_runtime'])
args = parser.parse_args()
sdk = Path(__file__).resolve().parents[1]
source = sdk / 'gta4-recomp/src'
out = args.output.resolve()
out.mkdir(parents=True, exist_ok=True)
stubs = out / 'stub'
headers = {
'rex/cvar.h': '''#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <initializer_list>
namespace review { template<class T> struct Setting {
 T value; template<class A,class B> Setting& range(A,B){return *this;}
 Setting& allowed(std::initializer_list<const char*>){return *this;}
}; }
#define REXCVAR_DEFINE_STRING(n,v,...) auto n=review::Setting<std::string>{v}
#define REXCVAR_DEFINE_BOOL(n,v,...) auto n=review::Setting<bool>{v}
#define REXCVAR_DEFINE_DOUBLE(n,v,...) auto n=review::Setting<double>{v}
#define REXCVAR_DEFINE_INT32(n,v,...) auto n=review::Setting<int32_t>{v}
#define REXCVAR_GET(n) (n.value)
#define REXCVAR_SET(n,v) (n.value=(v))
#define REXCVAR_DECLARE(t,n) extern review::Setting<t> n
extern review::Setting<std::string> touch_controls;
namespace rex::cvar { inline std::string GetFlagByName(std::string_view name) {
 return name == "touch_controls" ? ::touch_controls.value : std::string{};
} }
''',
'rex/logging.h': '#pragma once\n#define REXLOG_INFO(...) ((void)0)\n#define REXLOG_DEBUG(...) ((void)0)\n#define REXLOG_WARN(...) ((void)0)\n#define REXLOG_ERROR(...) ((void)0)\n',
'rex/input/input_system.h': '''#pragma once
#include <rex/input/motion.h>
namespace rex::input {
inline MotionState test_motion{};
class InputSystem { public: bool TryGetMotionState(uint32_t,MotionState* out){*out=test_motion;return true;} };
}
''',
'rex/runtime.h': '''#pragma once
#include <rex/input/input_system.h>
namespace rex { class Runtime { public:
 static Runtime* instance(){static Runtime r;return &r;}
 input::InputSystem* input_system(){static input::InputSystem i;return &i;}
}; }
''',
'rex/ui/window.h': '''#pragma once
#include <rex/ui/window_listener.h>
namespace rex::ui { class Window { public:
 int input_listeners=0,listeners=0; size_t order=0;
 void AddInputListener(WindowInputListener*,size_t value){++input_listeners;order=value;}
 void RemoveInputListener(WindowInputListener*){--input_listeners;}
 void AddListener(WindowListener*){++listeners;}
 void RemoveListener(WindowListener*){--listeners;}
}; }
''',
}
headers['gta4_init.h'] = '#pragma once\n#include <cstdint>\nunion ReviewRegister { uint64_t u64; uint32_t u32; uint8_t u8; double f64; };\nstruct PPCContext { ReviewRegister r3{},r4{},r23{},r28{},f1{},f2{}; uint64_t lr=0; };\nvoid __imp__sub_8224EEF8(PPCContext&,uint8_t*);\nvoid __imp__sub_8224EE98(PPCContext&,uint8_t*);\nvoid __imp__sub_821BF050(PPCContext&,uint8_t*);\nvoid __imp__sub_8233ABF0(PPCContext&,uint8_t*);\nvoid __imp__sub_8239C468(PPCContext&,uint8_t*);\n'
for rel, contents in headers.items():
    path = stubs / rel
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(contents)
# Sensor inputs use the same sign convention as the production atan2 path.
samples = '#pragma once\n#include <array>\n'
for name, degrees in [('kNeutral', 0), ('kPitchUp', 30), ('kPitchDown', -15)]:
    angle = math.radians(degrees)
    values = [0.0, 9.80665 * math.cos(angle), -9.80665 * math.sin(angle)]
    samples += f'inline constexpr std::array<float,3> {name} = {{' + ','.join(f'{v:.12f}f' for v in values) + '};\n'
(out/'motion_samples.h').write_text(samples)
subprocess.run([sys.executable, str(sdk/'tests/regression/controls/touch_samples.py'),
                '--output', str(out/'touch_samples.h')], check=True)
subprocess.run([sys.executable, str(sdk/'tests/regression/controls/touch_icon_samples.py'),
                '--output', str(out/'touch_icon_samples.h')], check=True)
results = {}
for name in ['touch_runtime', 'coordinator_runtime', 'touch_context_runtime', 'touch_layout_runtime', 'touch_pointer_runtime', 'touch_editor_mouse_runtime', 'touch_icon_runtime', 'motion_runtime']:
    if args.case and name not in args.case:
        continue
    cpp = sdk/'tests/regression/controls'/f'{name}.cpp'
    command = [args.compiler, '-std=c++23', '-g', '-O1', '-UNDEBUG',
               '-I'+str(stubs), '-I'+str(source), '-I'+str(sdk/'include'), '-I'+str(out), str(cpp)]
    if name in ['touch_runtime', 'coordinator_runtime']:
        command += [str(source/'input/context_touch_layout.cpp'), str(source/'input/context_touch_settings.cpp'), str(sdk/'src/input/mnk/controller_compatibility.cpp'), str(sdk/'src/input/mnk/pointer_motion.cpp')]
    if name == 'coordinator_runtime':
        command += [str(source/'gta4_touch_coordinator.cpp')]
    if name == 'touch_context_runtime':
        command += [str(source/'input/context_touch_context.cpp'), '-DGTA4_TOUCH_CONTEXT_TEST=1']
    if name == 'touch_layout_runtime':
        command += [str(source/'input/context_touch_layout.cpp'), str(source/'input/context_touch_settings.cpp')]
    if name in ['touch_pointer_runtime', 'touch_editor_mouse_runtime']:
        command += [str(sdk/'src/input/absolute_pointer.cpp')]
    if name == 'touch_icon_runtime':
        command += [str(sdk/'src/ui/image_decode.cpp'), '-I'+str(sdk/'thirdparty/stb')]
    if args.sanitize:
        command += ['-fsanitize=address,undefined', '-fno-omit-frame-pointer']
    command += ['-o',str(out/name)]
    compiled = subprocess.run(command, text=True, capture_output=True)
    (out/f'{name}.build.log').write_text(compiled.stdout+compiled.stderr)
    if compiled.returncode:
        print(compiled.stdout+compiled.stderr)
        raise SystemExit(compiled.returncode)
    executed = subprocess.run([str(out/name)] + ([str(out)] if name == 'touch_layout_runtime' else []), text=True, capture_output=True)
    (out/f'{name}.log').write_text(executed.stdout+executed.stderr)
    print(executed.stdout+executed.stderr, end='')
    results[name] = {'compile_exit': compiled.returncode, 'exit': executed.returncode,
                     'checks': sum(line.startswith('PASS ') for line in executed.stdout.splitlines())}
    if executed.returncode:
        raise SystemExit(executed.returncode)
coordinator = (source/'gta4_touch_coordinator.cpp').read_text()
start = coordinator.index('void GTA4_TouchConsumePoll(')
stop = coordinator.index('void GTA4_TouchObserveControlReplay(', start)
poll = coordinator[start:stop]
assert poll.index('extension.begin_poll(context, base, epoch, frontend_active, map_active)') < poll.index('  AbsolutePointerEvent event;') < poll.rindex('FreezeVirtualKeys(')
assert 'extension.collect_virtual_keys(epoch, snapshot.down, snapshot.pressed)' in coordinator
controls = (source/'input/context_touch_controls.cpp').read_text()
start = controls.index('void OnControlReplay(')
stop = controls.index('\n}  // namespace', start)
assert 'RefreshLayoutLocked' not in controls[start:stop]
assert 'g_runtime.epoch != epoch' in controls[start:stop]
print('PASS coordinator prepares context before pointers and key freezing; replay stays immutable')
manifest = {str(p.relative_to(sdk)): hashlib.sha256(p.read_bytes()).hexdigest() for p in [
    source/'gta4_touch_coordinator.cpp', source/'input/context_touch_controls.cpp',
    source/'input/context_touch_layout.cpp', source/'gta4_motion_bridge.cpp',
    source/'gta4_motion_reload_policy.h', source/'gta4_motion_action_policy.h',
    source/'gta4_motion_vehicle_hooks.cpp', sdk/'include/rex/input/motion_sample_cache.h']}
(out/'results.json').write_text(json.dumps({'results':results,'sanitized':args.sanitize,
    'source_checks':1,'sources':manifest},indent=2)+'\n')
