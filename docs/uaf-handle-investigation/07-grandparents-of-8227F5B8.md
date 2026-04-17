# UAF Handle Investigation — Agent 07: Grandparents of `sub_8227F5B8`

Walk farther up the call stack from `sub_8227F5B8(r3=rect_ptr, r4=handle)`.
Agent 5 covers immediate callers; this report covers **grandparents / great-grandparents** and the **owning subsystem** that allocates + frees the handle.

## 1. Target function summary

|-|-|
|address|`0x8227F5B8`|
|size|~0x50 bytes (tiny trampoline)|
|call count|8x|
|callee|`sub_8227F2E8` (only one)|
|r3|pointer to 4 floats (a 16-byte rect: x0,y0,x1,y1)|
|r4|the **handle stored at sp+92** — passed through to `sub_8227F2E8`|

### Recomp evidence
`sub_8227F5B8` does nothing but:
1. Load 4 floats from `r3` into `f1..f4`
2. Load two constants from `lbl_0x828008C8` / `lbl_0x82800134` into `f5..f9`
3. Spill `r4` (the handle) at `sp+92`
4. Tail-call `sub_8227F2E8`

`sub_8227F2E8` itself performs **4 × calls to `sub_828C2290`** (the RAGE 2D-line/quad draw primitive, 267x callers). It loads `r30` from `sp+300` (== `sp+208+92`, i.e. the stashed `r4`) and reads `[r30+0]` as a vtable-like function pointer — r4 is a **grcDrawHandle / immediate-mode draw context**. The outer primitive is "**draw 4 lines = a wireframe rectangle**".

## 2. Direct callers (for context; agent 5's territory)

|addr|purpose (from strings)|
|-|-|
|`sub_8214DBD0`|LOADLANG / NO_PAD / "$%d" / "%d$" — **frontend / language+pad prompts**|
|`sub_821506E8`|only caller is `sub_8214DBD0` — same subsystem|
|`sub_821B5C90`|**LOADING..., CARICAMENTO..., CHARGEMENT..., FADE_LOAD** — loading screen|
|`sub_821F1EE0`|no strings; size 0x438, called from button-prompt layout path|
|`sub_8229D8A8`|no strings; reached via street-name / mission HUD layout|

## 3. Grandparents (the ask)

### 3.1 `sub_8214DBD0` — no parent (root)
`find_callers` returns nothing. This is a **root entry point** (script/IDLE tick, or indirect-dispatched). 53 callees, 4856 bytes.
String refs: `"LOADLANG"`, `"NO_PAD"`, `"$%d"`, `"%d$"` → **Frontend / language-and-pad waiting screen** (the early boot "press start, select language" UI).

Because this is both a direct caller AND the grandparent of the `sub_821506E8` path, it owns two of the five direct call paths.

### 3.2 `sub_821B5C90` — no parent (root)
Also a root / indirect-dispatched entry. 40 callees, 4744 bytes.
String refs: `"LOADING..."`, `"BELADUNG..."`, `"CARGA..."`, `"CARICAMENTO..."`, `"CHARGEMENT..."`, `"FADE_LOAD"` → **loading screen fade-out / LOADING text render**.

### 3.3 `sub_821F1EE0` → parent `sub_821F5788` → grandparent `sub_821F6550`
`sub_821F6550` is a **hot (500x-tier) shared helper** with 28 callers across the game (front-end, loading, HUD, mission UI). It is not the owning subsystem — it is the draw-list flush / 2D-quad push primitive. Neighbors in 0x821Fxxxx are:

|addr|strings|subsystem|
|-|-|-|
|`sub_821F2360`|PAD_A/B/X/Y, LB/RB/LT/RT, DPAD_*, LSTICK_*, COL_NET_*, BLIP_*, INPUT_, ACCEPT, CANCEL|**button-prompt parser**|
|`sub_821F52C8`|A_BUTT, B_BUTT, BACK_BUTT, font1/font3/fonts, streamedfont, *_ARROW, DPAD_*|**button-prompt glyph binder**|
|`sub_821F6098`|font1/font2/font3/fonts, streamedfont, RSTICK_ROTATE|**HUD font / pad-glyph font manager**|

→ grandparent path 3 belongs to the **HUD button-prompt / on-screen pad-glyph rendering subsystem** (`CTextFile` + `CPlayerInfo::PadPrompts`).

### 3.4 `sub_8229D8A8` → parent `sub_8229F0F8` → grandparent `sub_82255CC8`
`sub_8229F0F8` (256 bytes, 1 caller) iterates a **12-entry pointer table at `0x82CC7BD0`** (stride 4) and, for entries that have a valid heap object (`[ptr]` non-null, `[[ptr]+0]`==0), calls `sub_8229D8A8(index, 0 or 1)`. It's dispatching by slot index. The gate at `lbl_0x82BFA144` is a single byte flag (enable/disable). r3 is clamped against codes `-90` and `-93` (looks like signal / event ids).

`sub_82255CC8` (56 bytes, 4 callers) is the **table-indexed trampoline**: it reads a per-slot pointer from a second array at `0x82BFA10C`, then calls `sub_8229F0F8(ptr)` then `sub_822551E0`. Its caller is `sub_8214DBD0` (the LOADLANG root again) — so this path folds back into the front-end subsystem.

Neighbor strings at 0x8229Fxxxx:

|addr|strings|
|-|-|
|`sub_8229F588`|"Streetname"|
|`sub_8229FE78`|"None", "None", "None", "None" (script null-strings)|

→ `0x8229xxxx` is the **HUD street-name / area-name rendering region**. Combined with `sub_82255CC8`'s reachability from the front-end root, grandparent path 4 is a **frontend-driven HUD text panel**.

### 3.5 `sub_821506E8` → grandparent = `sub_8214DBD0`
Trivial (same root as 3.1). Only caller of 821506E8 is 8214DBD0 itself.

## 4. Neighboring draw primitives (callees of `sub_8227F2E8`)

The 0x828Cxxxx cluster is the **RAGE grmShaderFx / grcTexture draw pipeline** (verified via `render_draw_symbols.txt` and string refs in `func_string_refs.txt`):

|callee|callers|role|
|-|-|-|
|`sub_828C19C0`|**571x** (hot)|set-up current grmShader / draw-batch|
|`sub_828C2290`|**267x** (hot)|emit one 2D quad/line vertex batch|
|`sub_828C2300`|71x|finalize the 2D batch|
|`sub_828C21D0`|75x|set blend / raster state|
|`sub_828C6568`|31x|bind grcTexture / state bundle|
|`sub_828C64C8`|29x|set texture stage|
|`sub_828C6500`|30x|unbind state|
|`sub_828C60A0`|30x|end shader pass|

String refs for `0x828Cxxxx`: "drawblit", "drawskinned", "unlit_draw", "BaseTex", "drawDepth", "drawBlur", "d3d_bias", "ps2_bias", "BlitMatrix", "DiffuseTex", "gtadrawblit", "ShadowMatrix", "blitshaft", "shadblit", "rewarp", "warpcascade", "Bucket", "opaque", "mesh" — pure RAGE render-pipeline territory.

## 5. Strings, RTTI, vtables — class context

- `sub_8227F5B8` and `sub_8227F2E8` are **not assigned to any vtable** (no RTTI). They are C-linkage helpers in the 2D draw-queue path (`rage::grc2d` / `CSprite2D` style).
- No string refs on `sub_8227F5B8`, `sub_8227F2E8`, `sub_821F1EE0`, `sub_821F5788`, `sub_821F6550`, `sub_8229D8A8`, `sub_8229F0F8`, `sub_82255CC8`. Identification had to come from **callees and address-range neighbors** (0x828Cxxxx = RAGE shader, 0x821Fxxxx = button-prompt font, 0x8229Fxxxx = street-name HUD, 0x821B5Cxx = loading screen, 0x8214DBxx = front-end).
- Candidate RTTI classes along this data path (from `render_draw_symbols.txt`):
  `CDrawRectDC` (`0x820014BC`), `CDrawCurvedWindowDC` (`0x820014D8`), `CDrawSpriteDC` (`0x82001468`), `CDrawPolyLoadingClockDC` (`0x820014F4`), `CRenderFontBufferDC` (`0x82001388`), `CDrawRadioHudTextDC` (`0x82001414`) — all 5-slot vtables, all 2D HUD draw commands. The `r4` handle is almost certainly a **pooled `CDrawCommand*` (drawlist node)** produced by the HUD/front-end code and consumed by the RAGE render thread.

## 6. Hypothesis: where the handle is allocated + freed

The r4 handle is threaded through as a **persistent `CDrawCommand*` / `grcDrawHandle*`** from the HUD/front-end code. Multiple subsystems share allocation:

|owner|path|lifetime|
|-|-|-|
|Frontend / LOADLANG screen|`sub_8214DBD0` → `sub_8227F5B8`|per-frame, torn down on state exit|
|Loading screen|`sub_821B5C90` → `sub_8227F5B8`|rebuilt on LoadingScreenBegin/End|
|Button-prompt HUD|`sub_821F6550` → `sub_821F5788` → `sub_821F1EE0` → `sub_8227F5B8`|per-frame, recycled by pad-prompt text layout|
|Street-name / HUD panel|`sub_82255CC8` → `sub_8229F0F8` → `sub_8229D8A8` → `sub_8227F5B8`|persistent, stored in 12-entry slot table at `0x82CC7BD0`|

The **`0x82CC7BD0` slot table is the prime suspect for the UAF source.** `sub_8229F0F8` dereferences `[slot_ptr][0]` as a vtable, and calls `sub_8229D8A8` which then funnels the ptr into `sub_8227F5B8`. If one subsystem frees the slot (e.g. LoadingScreen teardown tears down a CDrawCommand) while another path (`sub_8214DBD0` → `sub_82255CC8`) still owns the index, r4 arrives at `sub_8227F5B8` stale.

The front-end root `sub_8214DBD0` owns **both** the direct path AND the 82255CC8 street-name-table path, so a free on one path can poison the other. **This is the likely UAF site.**

## 7. Recommended next actions (for agents 8+)

1. Dump the 12-entry pointer array at `0x82CC7BD0` at runtime (in the host). Compare before/after LoadingScreen ↔ Frontend transitions.
2. Inspect `sub_8229D8A8`'s write path — where does it `delete` / return to pool? Are the enables at `0x82BFA144` cleared when the slot is freed?
3. Check `sub_8227F2E8` at offset +0 on `r4` — which vtable? Pull RTTI if any (likely one of the `CDraw*DC` classes).
4. Confirm whether `sub_8214DBD0` mid-call invokes `sub_821B5C90` or the street-name path such that it frees a shared slot.
5. If r4 is a `CDrawCommand*`, audit the draw-list pool allocator (likely in `0x828C*` shader module) for reuse-after-free.

## 8. Raw data (for reproducibility)

```
Direct callers of sub_8227F5B8 (5):
  sub_8214DBD0       (frontend / LOADLANG, NO_PAD — root)
  sub_821506E8       (caller=sub_8214DBD0)
  sub_821B5C90       (loading screen — root)
  sub_821F1EE0       (caller=sub_821F5788)
  sub_8229D8A8       (caller=sub_8229F0F8)

Grandparents:
  sub_8214DBD0       (frontend root — direct + grandparent)
  sub_821B5C90       (loading-screen root)
  sub_821F5788       (caller=sub_821F6550 hot-helper, 28x)
  sub_8229F0F8       (caller=sub_82255CC8)

Great-grandparents:
  sub_821F6550       (hot 2D helper, 28 callers across HUD/menu/frontend)
  sub_82255CC8       (caller=sub_8214DBD0 — loops back to frontend)
```

## 9. Summary

**Subsystem of sub_8227F5B8: RAGE 2D immediate-mode line/quad draw primitive** (neighborhood `0x828Cxxxx`, calls `sub_828C2290` 4× to outline a rect). It is NOT owning — it is the consumer.

**Owning subsystems (r4 source):**
- **Frontend UI** (sub_8214DBD0 — LOADLANG, NO_PAD; 2 of 5 call paths)
- **Loading screen** (sub_821B5C90 — LOADING/FADE_LOAD)
- **HUD button-prompts** (sub_821F6550 cluster — PAD_*, DPAD_*, fonts, streamedfont)
- **HUD street-name panel** (sub_8229xxxx — Streetname, indexed via `0x82CC7BD0` slot table)

**UAF prime suspect:** the 12-entry pointer array at `0x82CC7BD0` shared between front-end and street-name HUD, with enable-flag byte at `0x82BFA144` and per-slot vtable-pointer table at `0x82BFA10C`. Front-end teardown likely frees a slot still referenced on another path.
