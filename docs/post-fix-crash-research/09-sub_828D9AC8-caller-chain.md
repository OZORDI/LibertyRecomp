# Agent 9 — sub_828D9AC8 Caller Chain

## Target
- `sub_828D9AC8` @ 0x828D9AC8, 764 bytes
- **Class / slot**: `rage::grcTextureXenon::vfunc[23]` (vtable @ 0x8209612c slot 24)
- **Crash LR** 0x828D9D0C = 0x828D9AC8 + 0x244 → the `memcpy` (sub_82A00DC0) call inside this function's hot inner loop (pseudocode LABEL_22 area).

## Re-interpretation of the crash site
Per the brief's 3rd task: `LR=0x828D9D0C` means the bl that set LR is at 0x828D9D08, which is inside `sub_828D9AC8` itself. So the truly-crashing frame is `sub_82A00DC0` (memcpy, 577 callers, leaf). The crashing function with a live stack frame is `sub_828D9AC8` — the immediate caller of memcpy. sub_828D9AC8 was reached by virtual dispatch through grcTextureXenon vtable slot 24.

The memcpy arguments (from pseudocode):
```
sub_82A00DC0(
    v16,                                     // dst: v20 or v22 (pointer from 82A44168/Lock)
    *(DWORD*)(v7 + 16),                      // src: subresource data ptr
    *(u16*)(v7+14) * *(u16*)(v7+12) * *(u16*)(v7+2)); // len = h * w * bpp
```
Classic mip-level upload: `v7` walks a linked list (`v7 = *(v7+24)`) of source sub-images; len computed from (width * height * formatBytes). A bad (width*height) on a not-yet-populated/bound surface would produce an unbounded dst write. Host memcpy (rexcrt hook at 0x82A11940) would fault on the first OOB page.

## 1. Callers via find_callers MCP
`sub_828D9AC8` → **No callers found**. Expected: it is a vtable target, invoked exclusively through virtual dispatch (`PPC_CALL_INDIRECT_FUNC(ctr)` after `lwz r11, 0(r3); lwz r11, 24*4(r11); mtctr r11`). Same for every grcTextureXenon vfunc (slots 0, 5, 6, 9, 10, 15, 16, 17, 23 = 828DB6D8/9678/9680/9688/96A8/9F98/9DC8/9F08/9AC8 — all 0 static callers). This is the vtable-dispatch signature.

## 2. Vtable-writer constructors (who instantiates grcTextureXenon)
Grep `-32247`+`24876` (the `lis/addi` pair encoding 0x8209612c) across `glue/gta4-recomp/generated/` — the only files loading this exact immediate are `gta4_recomp.67.cpp` and `gta4_recomp.29.cpp`. Five writers store it at `this+0`:

|ctor|desc|first caller|
|-|-|-|
|`sub_828D9620`|no-arg ctor, zero-inits fields 16/24/32, dims 28/30|`sub_824F4730`|
|`sub_828D96C8`|full ctor (width, height, fmt, surface ptr)|`sub_828DA1A0` = **grcTextureFactoryXenon::vfunc[1]**|
|`sub_828DAC60`|ctor taking secondary init obj (v-table only)|indirect only|
|`sub_828DB760`|name-cloning ctor (CopyStr 256)|`sub_828DBB10` = **grcTextureFactoryXenon::vfunc[3]**|
|`sub_828DBBC0`|factory alloc (`sub_821B3510(60)` + ctor)|indirect only|

Also found in `gta4_recomp.29.cpp` at 0x828C29E0: a `grcTexture_rage` (parent) dtor loading the parent vtable 0x82093694 — unrelated to 0x8209612c (offset 13844 vs 24876), kept here only because grep matched on -32247.

## 3. Factory class that produces the instances
`grcTextureFactoryXenon_rage` vtable @ 0x82096084, 32 slots. The two creation vfuncs:
- `vfunc[1]` = `sub_828DA1A0` — "CreateManualTexture" equivalent: `sub_821B3510(52)` (inner 52-byte param block) → `sub_82A55738(w,h,mip=1,fmt=4,usage,0,0,-1)` (D3D descriptor init) → `sub_821B3510(60)` → `sub_828D96C8` (ctor).
- `vfunc[3]` = `sub_828DBB10` — "Create from name": `sub_826BD060(buf, name, 256)` name-copy → `sub_828C3178` lookup/dedupe → on miss `sub_821B3510(60)` + `sub_828DB760` ctor + `(*vtbl[16])(obj)` (slot 16 = `sub_828D9F98`, the bind/fill pass).

Those factory vfuncs are themselves slot targets of `grcTextureFactoryXenon` — dispatched from any caller holding the global factory pointer.

## 4. vfunc[23] semantic (what sub_828D9AC8 actually does)
Reading the pseudocode: it takes `(this, header)` where `header` is an in-memory image descriptor (`a2`). It:
- **Validates** that `this` matches the descriptor (mip count, width, height, format-count). Returns 0 on mismatch.
- Walks the mip linked list (`v7 = *(v7+24)`).
- For each mip: calls `sub_82A57088` (thunk → `sub_82A56C70`, surface Lock) to get a locked GPU-mapped buffer, then one of `sub_82A44820/38/48` (GetLevelDesc variants → `sub_82A440A0`). If the `0x100` tiled flag is not set, performs a linear `memcpy(gpuPtr, sourceMip, h*w*bpp)` (**this is the crashing call**). Then unlocks via `sub_82A42E88` → `sub_82A4A600` (surface Unlock/release).
- Finally copies 6 floats from header offsets (0x30..0x48 in `a2`) into `this+36..56` — bounding-box/UV transform data, so this is a **texture-populate-from-staging** routine. In rage naming convention this is typically `grcTextureXenon::UpdateFromSurface(...)` or `LockAndCopy(...)`.

So the vfunc is a **texture re-upload/stream path**, dispatched whenever the streaming pipeline needs to re-populate texture contents (e.g. after a streaming-in/cache-promote event, not initial creation which uses `sub_828D96C8`).

## 5. Boot-time path that leads there
`grmSetup_rage::vfunc[0]` (`sub_828C5ED8`, unique entry, no static callers — dispatched from the global `grmSetup` during `rage::grcDevice::Setup`) → `sub_828C5840` (`grcSetup::vfunc[0]`, 1 call site = 828C5ED8) → `sub_828C2A48` (unique caller = 828C5840). `sub_828C2A48` runs during renderer init; it:
- allocates 64 bytes at global 0x8205FA20+11616 via `sub_829FF840`
- dispatches vtable slot 3 (vfunc[2], offset 12) on the factory twice — slot lookup at `(*r31+12)`; these build small 1-pixel default textures
- sets dims to 32 × 1 @ `sub_828D1FC0(8, 0xFFFFFFFF, ...)` — the 8-byte argb32 default texture
- stores the two resulting grcTextureXenon pointers at globals `+11592` and `+11596`.

These global default textures live on the factory; any later streaming promotion that reuses one of these factory slots would invoke vfunc[23] on the cached instance. The log wording "right after shader cache loads" is consistent with this: the shader cache finishes, rage then runs `grcSetup::PostInit` which loops over preloaded textures and invokes the update-from-surface vfunc on each — exactly `vfunc[23]`.

### Call chain up to game-state
```
entry (rage::grcDevice::Setup)
  -> grmSetup::vfunc[0] = sub_828C5ED8
    -> grcSetup::vfunc[0] = sub_828C5840
      -> sub_828C2A48 (creates default 32x1 textures via factory vtable slots 2 & 3)
         [registers ptrs at *(0x8205FA20 + 11592 / +11596)]
  -> after shader cache warmup, per-texture update loop:
       for (tex in list) { (*tex->vtbl[23])(tex, stagingDesc); }
         -> sub_828D9AC8 (this fn)
            -> sub_82A57088 (Lock surface) [thunk to 82A56C70]
            -> sub_82A44820/38/48 (GetLevelDesc) [thunk to 82A44168]
            -> sub_82A00DC0 (memcpy h*w*bpp)  *** CRASHES HERE ***
            -> sub_82A42E88 (Unlock) [thunk to 82A4A600]
```

## 6. Why this specific memcpy faults under LibertyRecomp
- `sub_82A57088`/`sub_82A56C70`/`sub_82A44168` are Xenon GPU-memory helpers (byte_8200CF50 is the bpp-per-format lookup; functions compute tiled/linear offsets via `_cntlzw`). Under our custom host GPU init (Sonic-style, see the beta branch commit `host GPU init: Sonic-style device-ready approach`), `sub_82A56C70` likely returns a **host pointer that is not in guest space** (or zero). The subsequent `memcpy(gpuPtr=host, src=guest, len=w*h*bpp)` in sub_828D9AC8 then either fires a SIGBUS (host pointer passed to a rexcrt memcpy that uses `g_memory.Translate`) or writes to unmapped guest memory.
- `sub_82A4A600` also runs `stwcx` against `this+5` with magic values `-65536/0xF00/0x100` — these are D3D9 resource state bits (`D3DRESOURCE_LOCKED`, reference-count coalesce). On a host-backed proxy surface those bits are meaningless and the function would not release the lock cleanly.

## 7. Summary — highest "game-state boundary"
- **Phase**: renderer init, between shader-cache load and the first draw.
- **Top frame you can name**: `grmSetup::vfunc[0]` (sub_828C5ED8). There is no outer caller in static analysis; it is dispatched by `rage::grcDevice::Setup`/runtime from a global-singleton setup chain.
- **Crashing operation**: `rage::grcTextureXenon::UpdateFromSurface(header)` (vtable slot 23) running a linear Lock+memcpy+Unlock against a host-backed surface whose Lock helper (sub_82A56C70) is not returning a guest-writable pointer.

## 8. Files referenced
- Recomp scaffolds (all absolute):
  - /Users/Ozordi/Downloads/LibertyRecomp/glue/gta4-recomp/generated/gta4_recomp.67.cpp — sub_828D9620, sub_828D96C8, sub_828DAC60, sub_828DB6D8, sub_828DB760, sub_828DBB10, sub_828DBBC0 (ctors, factory vfuncs, dtor)
  - /Users/Ozordi/Downloads/LibertyRecomp/glue/gta4-recomp/generated/gta4_recomp.29.cpp — parent grcTexture dtor
- Pseudocode (IDA): /Users/Ozordi/Downloads/LibertyRecomp/gta_iv/xex_excavation_retail/pseudocode/
  - sub_828D9AC8_0x828D9AC8.c
  - sub_828DA1A0_0x828DA1A0.c (factory vfunc[1])
  - sub_828DBB10_0x828DBB10.c (factory vfunc[3])
  - sub_82A56560, sub_82A56C70, sub_82A44168, sub_82A4A600 (GPU Lock/Unlock/GetDesc)
- Authoritative vtable / class layout: grcTextureXenon_rage @ 0x8209612c (25 slots) and grcTextureFactoryXenon_rage @ 0x82096084 (32 slots), from MCP `get_class_context`.

## 9. Next-step hypothesis for patching
Hook either:
1. `sub_82A56C70` (Lock) — return a guest-owned scratch buffer big enough for the mip, so the memcpy stays in guest space. Then queue an async host upload after `sub_82A4A600` (Unlock) with the captured buffer + tiling descriptor.
2. `sub_828D9AC8` itself — bypass the whole Lock/memcpy/Unlock sequence, call directly into the host GPU texture upload using `this+24` (source surface) and `a2` (header: dims at +0x1C/+0x1E, mip count via sub_828D1C98, bpp from sub_828D9328).

Option 2 is cleaner — it skips the Xenon-tiled D3D9 emulation layer entirely, which is what every other GTA IV recomp ended up doing.
