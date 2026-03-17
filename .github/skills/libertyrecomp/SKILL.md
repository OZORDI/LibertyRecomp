---
name: libertyrecomp
description: Provides deep context for working on LibertyRecomp — a static PPC recompilation of GTA IV Xbox 360 targeting macOS/Windows/Linux/PS4/Switch/iOS/Android. Use when working with sub_8XXXXXXX generated functions, writing PPC_FUNC_HOOK or GUEST_FUNCTION_HOOK overrides, reading or writing guest memory, debugging crashes, adding platform-specific code (PS4/Switch/iOS/Android/embedded), or working with the RexGlue runtime, VFS, audio integration, multiplayer, or the gta4-recomp generated code.
---
# LibertyRecomp

GTA IV Xbox 360 (PPC) → native ARM64/x64 via static recompilation. The XEX is recompiled to C++ using RexGlue. Game logic runs on a guest thread; host code intercepts it via hooks.

## Key facts

- **XEX v8 base**: `0x82000000`. Address offset from v1→v8: `+0x71450`.
- **Entry point**: `xstart` at `0x82A11290` → calls `sub_82A18BE0` → `sub_82A11290`.
- **Guest memory**: `g_memory.base` (host ptr) + uint32 guest addr = host pointer. Range `0x82000000–0x83200000`.
- **Big-endian loads**: `PPC_LOAD_U32(ea)` byte-swaps automatically. Raw host reads need `__builtin_bswap32`.
- **Generated files**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.{0-81}.cpp` (82 files).

## Repo layout

```
LibertyRecomp/
├── LibertyRecomp/          ← Liberty host code (hooks, OS layers, GPU, HID)
│   ├── main.cpp            ← Entry; RexGlue setup, VFS, LaunchModule
│   ├── gpu/video.cpp       ← GPU hooks (PPC_FUNC_HOOK, GUEST_FUNCTION_HOOK)
│   ├── kernel/             ← XAM, memory, IO, net, save
│   ├── os/{ps4,switch,ios,android,macos,win32,linux}/
│   ├── apu/driver/         ← SDL2 + PS4 sceAudio backends
│   └── install/            ← Installer wizard + EmbeddedAssets
├── glue/rexglue-sdk-main/
│   ├── gta4-recomp/generated/  ← All recompiled PPC functions
│   └── src/                    ← RexGlue runtime (VFS, kernel, audio)
└── gta_iv/                 ← gta4_functions.txt, gta4_vtables.txt (Ghidra exports)
```

## Finding a function

```bash
# Find generated body of a guest function
grep -n "PPC_FUNC_IMPL.*8285E710" glue/rexglue-sdk-main/gta4-recomp/generated/*.cpp

# Find which file owns an address range (files are sequential by address)
grep -l "sub_828" glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.5*.cpp
```

## Guest memory helpers

```cpp
// Read 32-bit big-endian word from guest address
uint32_t val = PPC_LOAD_U32(ctx.r3.u32 + 28);

// Write
PPC_STORE_U32(ctx.r3.u32 + 28, new_val);

// Translate guest addr → host ptr
void* host = g_memory.base + guest_addr;

// Translate host ptr → guest addr
uint32_t guest = g_memory.MapVirtual(host_ptr);
```

## Core hook macros (quick ref)

```cpp
// Override a generated PPC function — must be extern "C" to win the weak symbol
PPC_FUNC_HOOK(sub_82A49C38) {
    // ctx.r3 = arg0, ctx.r4 = arg1 ...  ctx.r3 = return
}

// Typed C++ shim — auto-translates args/return via ArgTranslator
GUEST_FUNCTION_HOOK(sub_82A44850, GTAIV_CreateTexture);
// Signature: uint32_t GTAIV_CreateTexture(uint32_t device, uint32_t w, uint32_t h, ...);

// No-op stub
GUEST_FUNCTION_STUB(sub_82A44B00);
```

See [HOOKS.md](HOOKS.md) for register ABI, full macro details, and calling guests from host.

## RexGlue runtime (key entry points)

```cpp
rex::Runtime::instance()                         // singleton after Setup()
rex::Runtime::Setup(code_base, code_size, ...)   // initialises kernel, VFS, audio
rex::chrono::Clock::set_guest_tick_frequency(50000000ULL); // MUST be 50 MHz
runtime->file_system()->RegisterSymbolicLink("common:", "\\Device\\Harddisk0\\Partition1\\common");
runtime->LaunchModule()                          // spawns main XThread → xstart
```

## VFS symlinks (registered in main.cpp after Setup)

| Symlink | Maps to |
|---------|---------|
| `game:` / `d:` | `\Device\Harddisk0\Partition1` (game dir) |
| `common:` | `…\common` |
| `platform:` / `xbox360:` | `…\xbox360` |
| `audio:` | `…\audio` |
| `update:` | `…\update` |
| `cache:` / `cache1:` | writable cache device |

## Build & run (macOS)

```bash
export VCPKG_ROOT=$(pwd)/thirdparty/vcpkg
cmake --build ./out/build/macos-release --target LibertyRecomp

"./out/build/macos-release/LibertyRecomp/Liberty Recompiled.app/Contents/MacOS/Liberty Recompiled" \
  > /tmp/liberty_run.log 2>&1
```

## Supporting files

- [HOOKS.md](HOOKS.md) — full hook patterns, ArgTranslator, calling guests from host
- [PLATFORMS.md](PLATFORMS.md) — PS4/Switch/iOS/Android guards, embedded build, sceAudio
- [DEBUGGING.md](DEBUGGING.md) — crash reading, guest addr from fault, log workflow
