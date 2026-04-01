# Render Pipeline Pseudocode Trace

> Ground-truthed via LLDB exec tracing + Ghidra pseudocode dump (32,037 functions at `gta_iv/xex_excavation_retail/pseudocode/`)

## Executive Summary

The scene pointer at `0x831C2458` is null because **no game code writes to it** — zero store instructions across the entire 32K-function pseudocode dump. On Xbox 360, the D3D kernel (`d3d.xex`) populates this slot during device creation. The render gate at `0x82B0B48C` has only one writer in game code, and it writes **-1** — the positive value (viewport count) is also set by the D3D kernel at frame-start. Both must be emulated by host hooks.

## Three Blockers (Priority Order)

| # | Address | Name | Status | Root Cause |
|-|-|-|-|-|
| 1 | `0x82B0B48C` | Render gate | Stuck at `0xFFFFFFFF` (-1) | Only game writer sets -1. Positive value (viewport count) set by D3D kernel |
| 2 | `0x831C2458` | Scene pointer | `0x00000000` | Zero writers in game code. Set by D3D kernel during device init |
| 3 | `video.cpp:9401` | Diagnostic typo | Reads `0x8290B48C` not `0x82B0B48C` | Address typo in render gate diagnostic |

## Runtime State (LLDB Exec Trace)

### Before LaunchModule (XEX loaded, not running)
```
0x831C2458 (scene)    = 0x00000000  ← entire 0x831C2400-2500 region is zeros
0x82B0B48C (gate)     = 0xFFFFFFFF  ← plus 0x82B0B480-488 also 0xFFFFFFFF
0x831C2DA8 (fallback) = 0x00000000  ← not yet initialized
```

### After 12s Execution (~300+ frames)
```
0x831C2458 (scene)    = 0x00000000  ← STILL null
0x82B0B48C (gate)     = 0xFFFFFFFF  ← STILL -1, never transitions positive
0x831C2DA8 (fallback) = 0xD900B620  ← POPULATED! Valid guest heap object (grcDevice)
0x831C2400            = 0xD907B590  ← vtable pointer — render globals table IS initialized
0x831C2464            = 0x82093044  ← neighboring field populated
```

### Watchpoint Confirmation
Write watchpoints on both `0x831C2458` and `0x82B0B48C` for 5s window after reaching steady state — **neither fired**. Both are static after XEX image load.

## Render Pipeline Call Graph (from pseudocode)

```
sub_828C5BA0  — grcSetup_rage::vfunc[4] ("Present" callback)
  └─ sub_828C15C8  — frame setup
       ├─ reads dword_82B0B48C (gate) and dword_831C2458 (scene)
       ├─ sub_828C9980  — manages scene list slot at 0x831C25A8
       ├─ if gate > 0 (signed):
       │    if scene != null: vtable[16](scene)  — render into scene
       │    sub_828C1228()  — main render loop
       └─ sub_828BF420()  — finalize / present

sub_828C1228  — main render loop
  ├─ while dword_82B0B48C > 0:      ← GATE CHECK (signed compare, never enters)
  │    v6 = dword_831C2458
  │    if !v6:
  │      v6 = vtable[4](dword_831C2DA8, 0)  ← FALLBACK: get scene from device
  │    vtable[16](v6)  — render into scene
  └─ dword_82B0B48C = -1  ← the ONLY writer, resets gate
```

**Key insight**: Even if `0x831C2458` were populated, the `gate > 0` check blocks entry. The fallback path through `dword_831C2DA8` (confirmed live at `0xD900B620`) would work IF the gate were positive.

## D3D Device Creation Chain

```
sub_828C5ED8  — grcSetup_rage::vfunc[0] (device creation entry)
  ├─ if dword_831C3094: sub_828E7630(1)   ← alternate device constructor
  └─ else: sub_828DC890(1)                 ← standard device constructor
       ├─ sub_828DC4D8(device, ...)        ← rage::grcDeviceXenon constructor
       │    ├─ creates grcRenderTargetXenon objects
       │    ├─ allocs 0x13880 bytes for render target pool
       │    └─ inits GPU ring buffer, tile state, predication
       └─ dword_831C2DA8 = device          ← WRITES FALLBACK DEVICE (confirmed live)
```

Both `sub_828DC890` and `sub_828E7630` write `dword_831C2DA8`. Runtime confirms this works — fallback device IS populated.

## The Gate Problem (Deep Analysis)

### Pseudocode search: who writes `dword_82B0B48C`?
```
grep results across 32,037 pseudocode files:
  sub_828C1228: dword_82B0B48C = -1    ← THE ONLY WRITER
  sub_828C15C8: reads dword_82B0B48C   ← gate check
```

**Zero positive writes.** On Xbox 360, the D3D kernel writes the viewport count (typically 1 or 2) at the start of each frame via the GPU command buffer interrupt. This is the same mechanism as `VdSwap` / `VdCallbackNotifyContext`.

### VdSwap reference
`sub_82A467D8` calls `VdSwap[0]()` — the Xbox 360 kernel Present/Swap function. In the recomp, this is stubbed. The swap callback chain normally triggers the gate write.

## The Scene Pointer Problem

### Pseudocode search: who writes `dword_831C2458`?
```
grep "dword_831C2458 =" across 32,037 files: ZERO RESULTS
grep "831C2458" across 32,037 files: only READS in sub_828C1228 and sub_828C15C8
```

On Xbox 360, the D3D kernel registers the scene object during `IDirect3DDevice9::CreateDevice()` by writing the scene pointer into the grcDevice globals table at offset 0x58 from base 0x831C2400.

## Corrected Understanding

Previous docs claimed:
- ❌ `0x82FF5368` is a scene pointer → it's `CFileManagerHashed` (streaming manager)
- ❌ State machine stalls at state 12 → red herring for scene, states don't write `0x831C2458`
- ❌ Gate is "permanently -1" → technically correct but misleading: it's -1 because no code writes positive

Corrected model:
- ✅ The fallback device at `0x831C2DA8` IS live (`0xD900B620`)
- ✅ The render loop has a fallback: `vtable[4](device, 0)` returns a scene when `0x831C2458` is null
- ✅ The ONLY blocker is the gate at `0x82B0B48C` — set it positive and the render loop enters
- ✅ Once the gate is positive, `sub_828C1228` will use the fallback device path automatically
- ✅ `video.cpp:9401` reads wrong address (`0x8290B48C` vs `0x82B0B48C`) — diagnostic is garbage

## Minimal Fix Path

1. **Set gate positive**: Hook the frame tick to write `PPC_STORE_U32(0x82B0B48C, 1)` before `sub_828C15C8` runs
2. **Fix diagnostic**: Change `video.cpp:9401` from `0x8290B48C` to `0x82B0B48C`
3. **Scene pointer**: May be unnecessary — the fallback through `dword_831C2DA8` should provide a scene object via `vtable[4](device, 0)`

## Exec Log Analysis (20-second run, 851 lines, ~600 frames)

### Init State: COMPLETE

The game fully completes initialization. Captured via exec log with INIT-PROBE hooks:

```
Timeline:
  Line 174: sub_82140000 ENTER (RAGE init gate)
  Line 175: sub_821B3CE8 ENTER (RAGE engine init)
  Line 210: sub_82A416B8 ENTER (D3D-SETUP) — called TWICE (r4=1, r4=2 = two viewports)
  Line 583: sub_821B3CE8 RETURN = 1 (INIT SUCCESS)
  Line 584: sub_821411D8 ENTER (game systems init) → returns at 621
  Line 622: sub_82145420 ENTER (first-frame setup) → returns
  Lines 627-631: Init probes #2 (early init), #3 (engine), #4 (renderer) — ALL RETURN
  Line 636: First RENDER-GATE frame#1 — scene already null, gate already garbage
  Lines 645-814: Full subsystem init chain (phases 7-22) — ALL fire
  Line 820+: Steady-state render loop begins
  Line 851: Clean exit (SDL_QUIT after 600 frames)
```

**Conclusion**: NOT stuck in init. The game reaches steady-state frame cycling (600 VdSwap ticks in ~20s).

### Late Init Functions (#3055, #2830)

```
#2830 sub_82478AF8 (821FC1F8 call 30) — ENTERS, never logs RETURN
  └─ All child phases complete: #3001-#3054 all fire and return
  └─ #3055 sub_827C2420 (activate-streaming) — ENTERS, never logs RETURN
       └─ Calls TRACE-DD0 (sub_82852DD0) and TRACE-D18 children — ALL return
       └─ sub_82852A50 RETURN, sub_82851A10 RETURN — children complete
```

The game continues after these functions' children return. Either they return but lack RETURN instrumentation, or they spawn background work and the main thread continues (per Xbox 360 threading model).

### MISSING-FUNC Calls During D3D Init (NOT Root Cause)

Three null function pointer calls during GPU-CREATE:
```
[MISSING-FUNC] indirect call to 00000000 from 82A4DB1C
[MISSING-FUNC] indirect call to 00000000 from 82A4DB8C
[MISSING-FUNC] indirect call to 00000000 from 82A4DBC0
```

All inside `sub_82A4DAB0` — XAM notification callback registration. It calls through `MEMORY[0x10266]` and `MEMORY[0x10059]` (kernel export table entries, null in recompiled env):

| Call Site | Registration | Callback | Purpose |
|-|-|-|-|
| 0x82A4DB1C | type 28 | sub_82A4D150 | PIX debug profiling — writes "PIX!NO" |
| 0x82A4DB8C | type 47 | sub_82A4D8F0 | Display state changes — writes 0x831D0B2C, NOT scene/gate |
| 0x82A4DBC0 | type 66 | sub_82A4D0D0 | Unknown (16-byte function, not in pseudocode dump) |

GPU-CREATE returns `result=1` (success) after these — they don't crash, just fail silently. These callbacks handle display connect/disconnect and profiling, NOT scene pointer or gate setup.

### Stubbed Kernel Functions

Three kernel stubs appear right before the MISSING-FUNC calls:
```
[VdQueryVideoFlags] !!! STUB !!!
[VdCallGraphicsNotificationRoutines] !!! STUB !!!
[VdGetCurrentDisplayGamma] !!! STUB !!!
```

`VdCallGraphicsNotificationRoutines` would fire registered GPU notification callbacks on real Xbox 360. Since it's stubbed AND the registrations fail anyway, the notification system is entirely non-functional.

### Render Loop: Gate is Viewport Count

From `sub_828C1228` pseudocode, the render gate is actually a **viewport count**, not a boolean:

```c
// sub_828C1228 — main render loop
if ( dword_82B0B48C > 0 )              // SIGNED compare: -1 fails this check
{
    do {
        // ... viewport setup per-viewport ...
        v6 = dword_831C2458;             // scene pointer
        if ( !v6 )
            v6 = vtable[4](dword_831C2DA8, 0);  // fallback: get scene from device
        vtable[16](v6);                  // render into scene
        ++viewport_index;
    } while ( viewport_index < dword_82B0B48C );  // iterate viewport_count times
}

// POST-LOOP FALLBACK (runs regardless of gate)
vtable[16](dword_831C2DA8, 0, 0, -1);   // device cleanup/present
if ( !dword_831C2458 && (dword_831C23D4 & 2) == 0 )
{
    // Secondary render path using device fallback
    scene = vtable[4](dword_831C2DA8, 0);
    sub_828C2140(scene);                 // render into fallback scene
    sub_828C1080(...);                   // full render pass
}
```

On Xbox 360: viewport count = 1 (single screen) or 2 (split-screen multiplayer). Currently -1 (init sentinel). Setting it to 1 would enter the main render loop, and the fallback device path would provide a scene.

### Post-Loop Fallback Path

After the main viewport loop, there's a SECOND rendering path:
- Condition: `!dword_831C2458 && (dword_831C23D4 & 2) == 0`
- This path runs REGARDLESS of the gate value
- It uses the device fallback (`dword_831C2DA8`) to get a scene and renders
- This likely renders the loading screen / splash when no game scene exists
- Whether it works depends on `dword_831C23D4` bit 2 — needs runtime verification

### VFS Failures During Init

The exec log shows many VFS path resolution failures:
```
VFS: '' -> [no device]                     ← 12 occurrences (empty path)
VFS: 'game:\config\curves.dat12' -> [entry not found]
VFS: 'update:\shaders\rage_postfx_e2dcl\...' -> [entry not found]
```

These resolve without crashing. The empty-path failures suggest some config/shader discovery code is hitting edge cases but recovering.

## Updated Minimal Fix Path

1. **Set viewport count**: Write `PPC_STORE_U32(0x82B0B48C, 1)` in VdSwap or frame-start hook
2. **Fix diagnostic**: `video.cpp:9401` address `0x8290B48C` → `0x82B0B48C`
3. **Scene pointer**: Likely unnecessary — fallback through `dword_831C2DA8` vtable[4] should work
4. **Verify post-loop fallback**: Check `dword_831C23D4` bit 2 at runtime — if 0, the loading screen path may already be executing (but with empty Present due to no actual rendered content)

## Files Referenced

| File | Role |
|-|-|
| `pseudocode/sub_828C5BA0_*.c` | Present callback (grcSetup vfunc[4]) |
| `pseudocode/sub_828C15C8_*.c` | Frame setup, gate + scene check |
| `pseudocode/sub_828C1228_*.c` | Main render loop, gate writer (-1) |
| `pseudocode/sub_828C9980_*.c` | Scene list slot manager |
| `pseudocode/sub_828BF420_*.c` | Finalize/present |
| `pseudocode/sub_828C5ED8_*.c` | grcSetup vfunc[0] — device creation |
| `pseudocode/sub_828DC890_*.c` | Standard device constructor, writes dword_831C2DA8 |
| `pseudocode/sub_828DC4D8_*.c` | rage::grcDeviceXenon constructor |
| `pseudocode/sub_828E7630_*.c` | Alternate device constructor |
| `pseudocode/sub_82A467D8_*.c` | VdSwap caller |
| `gta4_recomp.58.cpp:115329-115388` | Generated render loop code |
| `LibertyRecomp/gpu/video.cpp:9401` | Gate diagnostic (wrong address) |
