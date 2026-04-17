# Research 05 — rage::grcDevice "default vertex declaration" hypothesis

## Hypothesis restated
> On Xbox 360, `rage::grcDevice` initialises a default/nullary vertex declaration
> during boot that is bound until the game explicitly sets one. LibertyRecomp's
> host-side init is missing this step, so `g_pipelineState.vertexDeclaration`
> starts `nullptr` and the first HUD draw crashes at `video.cpp:5115`
> (`pipelineState.vertexDeclaration->vertexStreams[i]`).

## Verdict
**HYPOTHESIS REJECTED.** RAGE does not bind a "default" decl at boot.
But the investigation exposed the real defect: **LibertyRecomp never hooks
GTA IV's guest `SetVertexDeclaration` / `CreateVertexDeclaration` functions**,
so no decl ever reaches `g_pipelineState`. Every draw on this build dereferences
a null pointer — the HUD text path is simply the first one to hit the dirty flush.

---

## Evidence

### 1. RAGE boot strings: no "default decl"
Searched the 20 000-entry string index via `get_string_refs` for every plausible
marker. Hits:

| query | hit |
|-|-|
| `grcRenderTarget` | 1 (`grcRenderTargetMemPool: %dk …` @ 0x82096190) |
| `grcTextureFactory` | 1 (PlaceTexture error @ 0x820963d8) |
| `grcDevice` / `grcSetup` / `grcInit` / `grcEffect` / `VertexDecl` / `SetVertexDeclaration` / `CreateVertexDeclaration` / `StandardVertex` / `FVF` / `D3DVERTEXELEMENT` | 0 |

No debug string ever mentions a default / built-in / standard decl being
created or bound at device init. RAGE treats decl binding as a per-draw
contract the caller is expected to satisfy; the hardware doesn't punish a null
decl because the real Xenos only reads PM4 `REG_SHADER_CONSTANT` writes, which
the guest emits directly from its own decl pointer — it never dereferences a
host-side metadata struct.

### 2. UnleashedRecomp (same engine family) — proves the contract
`/Users/Ozordi/Downloads/LibertyRecomp/Reference Projects/UnleashedRecomp-main/UnleashedRecomp/gpu/video.cpp`
lines 7815–7846:

```
GUEST_FUNCTION_HOOK(sub_82BE04B0, GetVertexDeclaration);
GUEST_FUNCTION_HOOK(sub_82BE0428, CreateVertexDeclaration);
GUEST_FUNCTION_HOOK(sub_82BE02E0, SetVertexDeclaration);
```

Unleashed uses the **same un-guarded deref pattern** at its `SanitizePipelineState`
(`video.cpp:4043` — `if (!pipelineState.vertexDeclaration->vertexStreams[i])`)
and at `CheckInstancing` (line 4585 —
`g_pipelineState.vertexDeclaration->indexVertexStream != 0`). It does not crash
because the guest's `SetVertexDeclaration` fires well before the first draw, so
the host pointer is always non-null by the time `SanitizePipelineState` runs.
Same pattern in `Reference Projects/MarathonRecomp-main/MarathonRecomp/gpu/video.cpp:4498`.

### 3. LibertyRecomp — the hook is missing
`/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/gpu/video.cpp`:

- Line 286: `static PipelineState g_pipelineState;` → `vertexDeclaration` is
  default-initialised to `nullptr` (line 210).
- Lines 6286-6324: `CreateVertexDeclaration`, `SetVertexDeclaration`,
  `ProcSetVertexDeclaration` all defined as host helpers.
- Active GTA IV hook block (lines 9466-…): hooks for `CreateTexture`,
  `DrawPrimitive`, `DrawPrimitiveUP`, etc. — **no hook for SetVertexDeclaration,
  none for CreateVertexDeclaration**.
- Sonic-06-era hooks that reference these hosts live inside `#if 0` (line 9415)
  and `#if 0 // DISABLED - parameter layout investigation needed` (line 9306).
- `tools/ppc-mcp-server/src/tools/rendering-tools.ts:29` already identifies the
  GTA IV guest address: `sub_829C9440` = `SetVertexDeclaration` (device+10456).
  That address is not a discovered top-level function in the recomp DB
  (`get_function_info` returns unknown; `search_symbols` returns empty) — it
  sits inside the D3D thunk region the splitter did not expose, which is why
  it has zero callers in the index and has gone un-hooked.

### 4. How the crash reaches line 5115
`video.cpp:5074` `SanitizePipelineState` is called unconditionally from the
pipeline-cache miss path in `CreateGraphicsPipeline` (line 5129). The HUD text
path funnels through `DrawPrimitiveUP` (hooked at `sub_82A3DF50`, line 9981)
which calls `FlushRenderStateForMainThread` → enqueues a `DrawPrimitive*` cmd →
`ProcDrawPrimitive*` → `FindPipeline` → `SanitizePipelineState`. The loop
`for (size_t i = 0; i < 16; i++) { if (!pipelineState.vertexDeclaration->vertexStreams[i]) …}`
dereferences the null directly. Same `nullptr` would bite any earlier draw; HUD
text just happens to be the first real `DrawPrimitiveUP` commit after PM4
bypass is installed (see the two-phase `s_pendingDrawUP` commit at line 9940).

### 5. Why not just null-guard line 5115?
Null-guarding only pushes the crash one frame deeper — `CheckInstancing`-style
paths do not exist in Liberty's current draw emitters, but `ProcSetVertexDeclaration`
(line 6303) gates on `!= nullptr`, and pipeline hashing /
`desc.inputElements = pipelineState.vertexDeclaration->inputElements.get()`
(line 5178) still explode. Patching each deref is a band-aid that masks the
real missing-hook issue.

---

## What the correct fix looks like (no implementation)

1. **Hook the guest decl pipeline.** GTA IV exposes these at (per
   `tools/ppc-mcp-server/src/tools/rendering-tools.ts`):
   - SetVertexDeclaration: `sub_829C9440`
   - (CreateVertexDeclaration entry point has to be resolved in IDA; the
     recomp DB does not expose it as a top-level symbol — it is inlined into
     a D3D wrapper. Candidate range: neighbourhood of `sub_829CD350`
     / `sub_829C9070`, which are other bind thunks also missing from the
     function index.)
   The hook shims must call Liberty's existing host helpers
   (`CreateVertexDeclaration`, `SetVertexDeclaration`) after translating guest
   vertex-element arrays / decl handles.

2. **Seat a canonical decl at device-create time, NOT at boot.** If the exact
   addresses above cannot be resolved cleanly, the minimum-risk proper fix is
   to install a one-element pass-through `GuestVertexDeclaration` into
   `g_pipelineState.vertexDeclaration` during `Video::CreateDevice` (or
   `Video::Init`), matching what Unleashed's guest `SetVertexDeclaration` would
   have produced on its first call. This is NOT "replicate RAGE's default" —
   RAGE has none — it's "keep the host invariant that `g_pipelineState` is
   usable once the device exists".

3. **Resolve the two-phase `DrawPrimitiveUP` decl question.** The hook at
   `sub_82A3DAB0` / `sub_82A3DF50` (lines 9949, 9981) does not pass a decl to
   `DrawPrimitiveUP`. Whatever decl the guest sets via `sub_829C9440` must be
   captured and live on `g_pipelineState` before `ProcDrawPrimitiveUP` runs,
   otherwise the same crash reappears on every UP draw.

## Artefacts
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/gpu/video.cpp:210,286,5113-5117,6286-6324,9306,9415,9466,9940-10042`
- `/Users/Ozordi/Downloads/LibertyRecomp/Reference Projects/UnleashedRecomp-main/UnleashedRecomp/gpu/video.cpp:4043,4585,7815-7846`
- `/Users/Ozordi/Downloads/LibertyRecomp/Reference Projects/MarathonRecomp-main/MarathonRecomp/gpu/video.cpp:4498,5437`
- `/Users/Ozordi/Downloads/LibertyRecomp/tools/ppc-mcp-server/src/tools/rendering-tools.ts:29`
