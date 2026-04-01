# Scene Slot Runtime Watchpoints

## Summary

This note corrects several earlier scene-pointer docs that overclaimed the write path for
`0x831C2458` and the origin of the render gate at `0x82B0B48C`.

What is now verified in the current build:

1. `sub_828C15C8` reads `0x82B0B48C` and `0x831C2458` exactly as expected, and skips the
   scene path when the signed gate is `<= 0`.
2. In a post-load watchpoint run, neither `0x831C2458` nor `0x82B0B48C` was written after
   `rex::Runtime::LaunchModule()` and through roughly 1200 presented frames.
3. A control watchpoint on a neighboring render-global slot did fire during live runtime,
   so the watchpoint method is valid.
4. In a fresh breakpoint run, `rex::Runtime::LaunchModule()` was reached without any hit on
   `sub_828C1228`, but the live gate was already `0xFFFFFFFF` and the scene slot was already
   `0x00000000`.
5. `0x82FF5368` is `g_pManager2 (VoiceBlock)`, not a scene pointer.

The strongest supported conclusion is therefore narrower than the older docs:

- No post-load runtime writer to `0x831C2458` was observed in this build.
- No post-load runtime writer to `0x82B0B48C` was observed in this build.
- The gate is already negative before guest execution begins.
- The old `0x82FF5368 -> 0x831C2458` copy theory should be treated as incorrect.

## Code Anchors

### Render dispatch

In `sub_828C15C8`, the render dispatch computes the scene slot, reads the gate, branches
away if the signed value is `<= 0`, and only then reads the scene pointer:

- `gta4_recomp.58.cpp:115329-115354`
- `gta4_recomp.58.cpp:115386-115388`

Relevant lines:

```cpp
// addi r11,r11,9304
ctx.r11.s64 = ctx.r11.s64 + 9304;     // 0x831C2458
// stw r10,-432(r11)
PPC_STORE_U32(ctx.r11.u32 + -432, ctx.r10.u32); // 0x831C22A8, not 0x831C2458
// lwz r10,-19316(r10)
ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -19316); // 0x82B0B48C
// ble cr6,0x828c1764
if (!ctx.cr6.gt) goto loc_828C1764;
// lwz r3,0(r11)
ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0); // 0x831C2458
...
// bl 0x828c1228
sub_828C1228(ctx, base);
```

### Host diagnostic bug

The host-side render-gate diagnostic still reads the wrong address:

- `LibertyRecomp/gpu/video.cpp:9397-9402`

```cpp
uint32_t gateVal = PPC_LOAD_U32(0x8290B48C); // wrong
uint32_t sceneListPtr = PPC_LOAD_U32(0x831C2458); // right
```

This means the logged `gate=2145647480` values are garbage for the gate, even though the
scene-slot part of the log remains meaningful.

### XEX-loaded image is authoritative

`main.cpp` explicitly states that `LoadXexImage()` overwrites Liberty's data/BSS with the
authoritative PE image from `default.xex`:

- `LibertyRecomp/main.cpp:848-852`
- `LibertyRecomp/main.cpp:891`
- `LibertyRecomp/main.cpp:896-901`

This matters because `gta_iv/default.bin` is not authoritative once the XEX has been loaded.

### `0x82FF5368` is not a scene pointer

The current architecture doc already maps `0x82FF5368` as `g_pManager2 (VoiceBlock)`:

- `docs/rewrite/REWRITE_ARCHITECTURE.md:483-486`

That address must not be treated as a render-scene object.

## Experiment A: Post-Load Watchpoints

### Goal

Test whether either address is written after the XEX image has been loaded and the module is
about to launch.

### Method

1. Use the fresh macOS build:
   `out/build/macos-release/LibertyRecomp/Liberty Recompiled.app/Contents/MacOS/Liberty Recompiled`
2. Break at `rex::Runtime::LaunchModule()`.
3. Read the live guest-mapped values immediately before guest launch.
4. Set write watchpoints on the host-mapped addresses for:
   - guest `0x831C2458`
   - guest `0x82B0B48C`
5. Continue through startup and steady-state rendering.

The host watchpoint address is always:

```python
host_address = g_memory.base + guest_address
```

Example run A used:

```python
base = 0x400000000
scene_slot = base + 0x831C2458  # 0x04831C2458
render_gate = base + 0x82B0B48C # 0x0482B0B48C
control_slot = base + 0x831C22A8 # 0x04831C22A8
```

### Observed values before launch

Immediately before `LaunchModule()` in the watchpoint run:

- `0x831C2458 = 0x00000000`
- `0x82B0B48C = 0xFFFFFFFF`

### Result

After the watchpoints were armed post-load and the game was allowed to run to roughly 1200
presented frames:

- no write watchpoint fired for `0x831C2458`
- no write watchpoint fired for `0x82B0B48C`

### Control validation

A separate control watchpoint on neighboring render-global slot `0x831C22A8` did fire during
live runtime, stopping in:

- `sub_828BF898 + 136`

with backtrace:

```text
sub_828BF898 + 136
sub_828C5B08 + 152
sub_82144188 + 2172
sub_821B5B20 + 1528
sub_821B6F18 + 164
sub_82849940 + 556
sub_82A21600 + 216
rex::system::XThread::Execute() + 1156
```

That control result matters because it shows live writes in the same global region are
visible to LLDB watchpoints. The absence of hits on `0x831C2458` and `0x82B0B48C` is not a
debugger artifact.

## Experiment B: Fresh Breakpoint Run Before Guest Launch

### Goal

Determine whether `sub_828C1228` is responsible for the negative gate before guest execution
begins.

### Method

1. Restart the app under LLDB.
2. Set direct address breakpoints on:
   - `sub_828C1228`
   - `sub_828C15C8`
   - `rex::Runtime::LaunchModule()`
3. Run from process start.

Example run B used:

```python
base = 0x8000000000
scene_slot = base + 0x831C2458  # 0x0080831C2458
render_gate = base + 0x82B0B48C # 0x008082B0B48C
```

### Result

The process reached `rex::Runtime::LaunchModule()` without any breakpoint hit on
`sub_828C1228`.

At `LaunchModule()` in that fresh run:

- `0x82B0B48C = 0xFFFFFFFF`
- `0x831C2458 = 0x00000000`

This rules out the earlier claim that `sub_828C1228` is what sets the gate negative before
guest launch, at least in the observed boot path.

## `default.bin` Versus Live Memory

Reading `gta_iv/default.bin` directly gives:

```python
base = 0x82000000
off_scene = 0x831C2458 - base  # 0x11C2458
off_gate = 0x82B0B48C - base   # 0x0B0B48C
```

Observed file contents:

- `default.bin[0x11C2458:0x11C245C] = 0xD3741CA7`
- `default.bin[0x0B0B48C:0x0B0B490] = 0x00000000`

That does **not** match live memory after `LoadXexImage()`, where the gate is already
`0xFFFFFFFF`. This mismatch is expected given `main.cpp`'s explicit statement that the XEX
loaded PE data becomes authoritative and that Liberty's static data image should not be
trusted after load.

## Corrected Conclusions

### Verified facts

1. `sub_828C15C8` reads the real render gate from `0x82B0B48C` and the scene slot from
   `0x831C2458`.
2. The host diagnostic in `video.cpp` reads the wrong gate address (`0x8290B48C`).
3. In the observed runtime windows, `0x831C2458` remained zero post-load.
4. In the observed runtime windows, `0x82B0B48C` remained `0xFFFFFFFF` post-load.
5. No post-load write to either address was observed through steady-state execution.
6. A fresh run reached `LaunchModule()` with the gate already negative and without any
   `sub_828C1228` breakpoint hit.
7. `0x82FF5368` is `g_pManager2 (VoiceBlock)`, not a scene object.

### What is still inference

1. The negative gate likely comes from XEX image materialization or another pre-launch memory
   write, not from `sub_828C1228` on the observed path.
2. The writer for `0x831C2458` has still not been identified. The strongest supported claim is
   only that no post-load runtime writer was observed in the current build.

## Practical Impact

The older docs that recommend copying `*(0x82FF5368)` into `*(0x831C2458)` should not be used
as a fix plan. That copy path is unsupported by the current evidence and uses the wrong source
object.

## Next Research Step

The next tight step is not another broad grep. It is targeted instrumentation around the image
load boundary:

1. instrument `LoadXexImage()` / `XexModule::ReadImage()` to dump the authoritative live words
   at `0x82B0B48C` and `0x831C2458` immediately after the XEX image is materialized
2. identify the exact transition point where the gate becomes `0xFFFFFFFF`
3. keep `sub_828C1228` out of the suspected pre-launch path unless a later run actually hits
   that breakpoint before steady-state rendering
