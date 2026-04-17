# Agent 02 — Missing API Hook Hypothesis

## Hypothesis

> GTA IV's Xbox-360 D3D binding path uses an internal `IDirect3DDevice9` method
> that LibertyRecomp hooks **incompletely**. An API call (analogous to
> `SetStreamSource` / `SetVertexShader` / a private RAGE binding) that on real
> Xbox implicitly establishes the vertex format/declaration needs an explicit
> host hook — and is currently missing.

**VERDICT: CONFIRMED.** There is exactly one missing hook: `sub_82A42930`
(GTA IV's in-binary `IDirect3DDevice9::SetVertexDeclaration` equivalent). It is
called by every RAGE draw path — including `sub_828C21D0`, the HUD/text
`DrawPrimitiveUP` wrapper that triggers the crash — but video.cpp has no
`PPC_FUNC_HOOK`/`GUEST_FUNCTION_HOOK` for it. The guest-side device context
field `device[11812]` is updated correctly, but no
`RenderCommandType::SetVertexDeclaration` command is ever enqueued, so
`g_pipelineState.vertexDeclaration` stays `nullptr` and crashes at
`video.cpp:5115`.

---

## Hooked-API Inventory (video.cpp — only ACTIVE hooks)

Lines 8820–10133 contain every ACTIVE hook. Everything at 9417–9461
(`#if 0 // Disabled Sonic 06 hooks`) is dead code — those Sonic-06 addresses
don't exist in GTA IV.

### PPC_FUNC_HOOK (let recomp run, then post-hook)
|Address|Role|
|-|-|
|`sub_828E02E8`|Render thread bootstrap|
|`sub_82A42BA8`|`CreateShader` (allocates `GuestShader`)|
|`sub_82A42CB8`|`CreateShader` (variant)|
|`sub_828BEC78`|RAGE `CreateRenderTarget`|
|`sub_82A467D8`|`D3DDevice::Present` wrapper|
|`sub_82A55DC0`|Texture / RT surface allocation|
|`sub_82A492A8`|PM4 packet builder (stub)|
|`sub_82A499B8`|PM4 flush (stub)|
|`sub_82A46330` / `sub_82A46578`|PM4 helpers (stubs)|
|`sub_82A49CB0`|PM4 resolve draw (no-op)|
|**`sub_82A42760`**|**`SetVertexShader`** — translates handle, calls host `SetVertexShader`|
|**`sub_82A424A8`**|**`SetPixelShader`** — translates handle, calls host `SetPixelShader`|
|`sub_82A3B690`|`SetRenderTarget` register writer|
|`sub_82A3B7B0`|`SetDepthStencil` register writer|
|`sub_82A44B78`|`SetTexture` fetch const|
|**`sub_82A3DAB0`**|**`DrawPrimitiveUP_Begin`** (captures primType/vertCount/stride)|
|**`sub_82A3DF50`**|**`DrawPrimitiveUP_Commit`** (fires `DrawPrimitiveUP`)|
|`sub_82A3CC68`|`DrawPrimitivesInternal` (indexed / VB draws)|
|`sub_82A3E348`|`DrawIndexedVertices`|

### GUEST_FUNCTION_HOOK (full replace)
|Address|Role|
|-|-|
|`sub_82A50F28`|GPU-mem alloc stub|
|`sub_82A44970`|`CreateVertexBuffer` (GTA IV variant)|
|`sub_82A44850`|`CreateTexture` (GTA IV variant)|

### State commands wired through the render thread
`RenderCommandType::SetVertexDeclaration` exists (video.cpp:1100, 5559+,
6293, 6303, 6697). The host implementation is correct —
`ProcSetVertexDeclaration` assigns `g_pipelineState.vertexDeclaration`. The
**command is just never enqueued from any guest hook.**

---

## Missing-API Analysis

### Root cause: `sub_82A42930` has no hook

```
sub_82A42930(device, vertexDeclHandle):
    *(_DWORD *)(device + 11812) = vertexDeclHandle      ← guest state
    *(_QWORD *)(device + 16)   |= 0x80000               ← dirty flag bit 19
```

This is `IDirect3DDevice9::SetVertexDeclaration` equivalent. All 6 callers are
the canonical D3D-reset / draw-setup sites:
|Caller|Purpose|
|-|-|
|`sub_828BF1F8`|Apply vertex-decl state block (immediate-mode path)|
|`sub_828BFC00`|Indexed-draw dispatch (applies decl, then `sub_82A3E348`)|
|`sub_828C0688`|**HUD/text 3-vertex quad** (applies decl, then `sub_82A3DAB0`/`sub_82A3DF50`)|
|`sub_828C0848`|HUD/text variant|
|`sub_828C15C8`|Frame render-body reset (clears VS/PS/decl)|
|`sub_82A415A8`|`D3DDevice::Reset` — full state wipe|

### Why the crash: tracing `LR=0x828C2258` (`sub_828C21D0+0x88`)

`sub_828C21D0` pseudocode:
```
if (!gated)             → sub_828C20B0(1) + sub_828C00B0(...)
sub_828BF1F8(dword_831C2298)      ← supposed to set vertex decl
sub_828BF248(a1, a2, 36)          ← BeginVertices(vc=a2, stride=36)
...
```
The crash is at `+0x88` which is the `bl sub_828BF248` call site — the
`DrawPrimitiveUP_Begin`. Matches the instrumentation data exactly:
`vc=4, str=36, prim=6 (RECTLIST)`. 46 callers include
`CDrawRadarMapSectionDC`, **`CDrawRadioHudTextDC`**, `CDrawTriShapeDC`.

`sub_828BF1F8` calls `sub_82A42930(device, *a1)` where `a1` is a pre-built
state block at `dword_831C2298`. Because `sub_82A42930` is **not hooked**,
only the guest recompiled code runs — it writes `device[11812]` and sets the
dirty bit, but **no `RenderCommandType::SetVertexDeclaration` ever reaches the
render thread**. When the subsequent `DrawPrimitiveUP` commit arrives,
`g_pipelineState.vertexDeclaration` is still `nullptr` → deref crash at
`video.cpp:5115`:

```
for (size_t i = 0; i < 16; i++)
    if (!pipelineState.vertexDeclaration->vertexStreams[i])   // NULL deref
```

### Why VS/PS work but VD doesn't

The shader hooks (`sub_82A42760` / `sub_82A424A8`) DO translate the guest
handle and call `SetVertexShader`/`SetPixelShader`, enqueueing the host render
command. The symmetric hook for `sub_82A42930` → `SetVertexDeclaration` is
simply absent. Three host-side paths exist for creating a `GuestVertexDeclaration`:

* `CreateVertexDeclaration(elements)` (video.cpp:6286)
* `CreateVertexDeclarationWithoutAddRef(elements)` (video.cpp:6054)
* Cached lookup by hash in `g_vertexDeclarations`

None are ever invoked by guest code — there is no GTA IV-side factory hook
like `CreateVertexDeclaration` either. `sub_82A42930` receives a **pre-built
handle** from pooled state blocks (`dword_831C2298`, `dword_831C23CC`, etc.),
so the handle interpretation is game-specific and needs translation.

### Other draw-path hooks also missing

In addition to `sub_82A42930`, these are also unhooked but related:

|Address|Role|Status|
|-|-|-|
|`sub_82A42930`|`SetVertexDeclaration`|**MISSING — root cause**|
|`sub_82A4xxx`|`SetStreamSource` (unlocated)|Not found in binary (immediate-mode path uses DrawUP staging)|
|`sub_828BF1F8`|Vertex-decl state-block applier|Not hooked; wraps `sub_82A42930`|
|`sub_828BF248`|`BeginVertices` thunk|Not hooked; calls `sub_82A3DAB0` which IS hooked|

The `DrawPrimitiveUP` path flow is:
```
sub_828C21D0  (HUD/text entry)
  sub_828BF1F8(state block)          ← unhooked, but only wraps sub_82A42930
    sub_82A42930(device, declHandle) ← MISSING HOOK — vertex decl never applied
  sub_828BF248(count, start, 36)
    sub_82A3DAB0(device, 6, 4, 36)   ← HOOKED — captures prim/vc/str
  (game writes vertex data)
  sub_82A3DF50(device)                ← HOOKED — fires DrawPrimitiveUP
                                        → pipelineState.vertexDeclaration == nullptr → CRASH
```

---

## Fix Design

### Required hook: `sub_82A42930` → host `SetVertexDeclaration`

Model after the existing `PPC_FUNC_HOOK(sub_82A42760)` pattern:

1. Capture `deviceAddr = ctx.r3.u32` and `declHandle = ctx.r4.u32`.
2. Let `__imp__sub_82A42930` run (updates `device[11812]` and dirty flag).
3. Translate `declHandle`:
   * Handle == 0 → decl = nullptr (bind default / skip).
   * Handle != 0 → check a `GTAIV::LookupVertexDeclaration(handle)` registry;
     fallback to `g_memory.Translate(handle)` as a direct `GuestVertexDeclaration*`
     pointer.
4. Call host `SetVertexDeclaration(device, decl)` to enqueue
   `RenderCommandType::SetVertexDeclaration`.

### Where the declaration must be CREATED (separate concern, still MISSING)

GTA IV does not call a `D3DDevice::CreateVertexDeclaration` at runtime — the
state blocks at `dword_831C2298`, `dword_831C23CC`, `dword_82B0B530[]` are
built at game init from `D3DVERTEXELEMENT9` arrays baked into the xex. The
hook for `sub_82A42930` must therefore either:

* **(a)** Resolve the handle to an element array in guest memory, call
  `CreateVertexDeclarationWithoutAddRef(elements)` on first use (caches by
  xxHash), and pass the resulting `GuestVertexDeclaration*` to the render
  command; OR
* **(b)** Hook the RAGE init-time builder that creates these state blocks and
  register them in a `GTAIV::LookupVertexDeclaration` table (symmetric to
  `GTAIV::RegisterBuffer` / `RegisterTexture`).

Option (a) is the self-contained minimum fix — the handle layout needs to be
decoded: the state block at `*a1` typically begins with a pointer or inline
array of `D3DVERTEXELEMENT9` (8 bytes each, terminated by `D3DDECL_END()` =
`0xFF,0,0x11,0,0,0`). Inspection of `dword_831C2298` and the caller-provided
`a1 = dword_831C23CC` is needed to pin down the exact offset.

### Hook body sketch (NOT for implementation — design only)

```
PPC_FUNC_IMPL(__imp__sub_82A42930);
PPC_FUNC_HOOK(sub_82A42930) {
    uint32_t deviceAddr = ctx.r3.u32;
    uint32_t declHandle = ctx.r4.u32;
    __imp__sub_82A42930(ctx, base);    // update device[11812] + dirty bit

    GuestVertexDeclaration* decl = nullptr;
    if (declHandle != 0) {
        decl = GTAIV::LookupVertexDeclaration(declHandle);
        if (!decl) {
            GuestVertexElement* elems = ResolveDeclElements(declHandle);
            if (elems) decl = CreateVertexDeclarationWithoutAddRef(elems);
        }
    }
    auto* device = (GuestDevice*)g_memory.Translate(deviceAddr);
    SetVertexDeclaration(device, decl);
}
```

Where `ResolveDeclElements(handle)` parses the state-block layout to extract
the `D3DVERTEXELEMENT9` array. That helper is the genuinely hard part of the
fix — it requires disassembling one of the state-block constructors (called
before `dword_831C2298` / `dword_831C23CC` are first written).

### Why this is root-cause-correct, not a band-aid

* It closes the exact gap the hypothesis names: the missing symmetric hook
  in a family where VS/PS are hooked but VD is not.
* It routes through the existing, correct host path
  (`ProcSetVertexDeclaration` → `g_pipelineState.vertexDeclaration`) — no
  duplication, no default/fallback decl needed.
* It does NOT synthesize a decl at draw time (that would be a band-aid that
  breaks as soon as any path uses a non-HUD vertex layout).
* It fixes the symptom for **ALL 46 callers** of `sub_828C21D0` and all other
  paths that reach `sub_82A42930`, including `CDrawRadarMapSectionDC`,
  `CDrawRadioHudTextDC`, `CDrawTriShapeDC`, and every indexed-draw path via
  `sub_828BFC00`/`sub_82A3E348`.

---

## Files referenced

* `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/gpu/video.cpp` lines
  `5113-5117` (crash), `6286-6323` (host SetVertexDeclaration),
  `9088-10133` (active hook region), `9415-9463` (DEAD `#if 0` block).
* `/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/xex_excavation_retail/pseudocode/`:
  `sub_828C21D0`, `sub_828BF1F8`, `sub_828BF248`, `sub_828BFC00`,
  `sub_828C0688`, `sub_828C15C8`, `sub_82A415A8`, `sub_82A42930`,
  `sub_82A3DAB0`.
