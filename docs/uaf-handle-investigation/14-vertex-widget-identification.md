# Agent 14 — HUD Widget Identification From Live Vertex Capture

## The captured vertex (UAF crash-site input)

| field | value | meaning |
|-|-|-|
| `v0.pos`   | `[192.000, 269.625, 0.000, 0.000]` | screen-space XY; z=0 w=0 (transformed/RHW, not a 3D mesh) |
| `v0.extra` | `[0.000, -1.000]`                  | auxiliary texcoord2 (`-1` = sentinel "no alt-sprite blend") |
| `v0.color` | `0xFFE1E1E1`                       | **UAF POISON** (alpha=255, R=G=B=225) |
| `v0.uv`    | `[0.375, 0.781]`                   | sprite-atlas UV, 3/8 × 25/32 |
| prim       | `6 = nRECTLIST`                    | Xenos-specific screen-quad primitive |
| `vc, stride` | `4, 36` | 4 verts of 9 floats (xyzw + extra_uv + color32 + uv) |

## Screen-space resolution (Python-verified)

```
1280x720:  x =  15.00%  y = 37.45%   <- native Xbox 360 target
 640x480:  x =  30.00%  y = 56.17%
1024x768:  x =  18.75%  y = 35.11%
1920x1080: x =  10.00%  y = 24.97%
```

Taking the 1280x720 target (GTA IV ships at 720p internal) — **(192, 269.625) falls
in the upper-left quadrant, roughly 1/7 of the way in from the left edge and 3/8
down from the top.** Given this vertex is v0 (upper-left corner of the quad), the
widget itself extends to the right/down from that anchor.

## UV analysis (Python-verified)

```
UV       = (0.375, 0.781)
   = (3/8, ~25/32)
on 256²  -> pixel ( 96, 199)
on 512²  -> pixel (192, 399)
on 1024² -> pixel (384, 799)
```

Both axes land on clean 1/32 or 1/8 grid lines — classic **sprite atlas** placement,
not a randomly-textured 3D surface.

## Color: 0xFFE1E1E1 is poison, not a HUD tint

- A=0xFF, R=G=B=0xE1 — uniform grey at 88.2% brightness.
- GTA IV frontend tints are either `0xFFFFFFFF` (white pass-through) or a
  gameplay-driven `HUD_COLOR` (read from `HUDCOLOR` config, typically blue).
- `0xE1E1E1E1` is a recognised debug-fill / uninitialised-heap stamp and matches
  the poison value stamped by the runtime's free-pattern path. The `0xFF` alpha
  is what you'd expect if only the low 3 bytes (RGB) were stamped over a live
  texture-pointer field.
- **Conclusion**: the color lane is UAF debris. The draw is proceeding with a
  value that used to be a live handle/material pointer.

## Primitive & stride match the loading-screen renderer

Stride 36 = 9 × f32 = `(x,y,z,w) + (ex,ey) + color32 + (u,v)`. This is the
exact vertex record consumed by `sub_828C2290` (the draw-emit helper wrapped by
`sub_8227F5B8`). `nRECTLIST` + 4-vert sub-quad emission is the screen-quad path
used by the 2D HUD pipeline (*not* the 3D mesh pipeline, which uses TRIANGLELIST
or TRIANGLESTRIP with a larger stride).

## Matching call site — `sub_821B5C90` (loading-screen controller)

`sub_821B5C90` is the **loading-screen render function**. It's the only caller of
`sub_8227F5B8` that also references:

- `"LOADING..."` (English)
- `"CARGA..."` (Italian), `"CARICAMENTO..."` (It. alt), `"CHARGEMENT..."` (Fr),
  `"BELADUNG..."` (De/Nl)
- `"FADE_LOAD"` — the screen-fade key used between loading states

That's the top-level label **GTA IV draws exactly once per loading screen** —
during story-mission load, save-game load, and the initial boot → first mission
transition.

### The two call sites inside `sub_821B5C90`

Both sites bind a cached texture handle via `sub_8227EDA0(&handle_slot)` and
then emit the 4-vertex screen quad via `sub_8227F5B8`:

| call site | handle slot | what it binds |
|-|-|-|
| `0x821B652C` (near `sub_821B6520`) | `dword_831E49B0` (`.long 0`, only referenced from here) | first loading-screen sprite |
| `0x821B6EAC` (near `sub_821B6E9C`) | `dword_831E49DC` (`.long 0`, only referenced from here) | second loading-screen sprite |

Both slots are exclusively written/read by `sub_821B5C90` — they are the
**loading-screen's private texture-handle cache**. The rectangle math at the
second site uses `qword_82B3193C` (a global AABB) ± `f27`-scaled half-extents,
which builds the 4 corner floats that match the stride-36 layout.

At `0x821B6E9C` the code explicitly gates:
```
lwz   r11, (dword_831E49DC - 0x831E4958)(r18)
cmplwi cr6, r11, 0
beq   cr6, loc_821B6EAC            ; skip draw if handle is NULL
bl    sub_8227EDA0                 ; otherwise bind it
...
bl    sub_8227F5B8                 ; then emit the quad
```
So the game **does check for NULL but not for poison** — exactly the condition
an UAF exploits.

## Mapping the captured vertex back to the on-screen element

- 2D screen-space quad at upper-left on 1280x720.
- Sprite-atlas UV with tidy fractional coords.
- Rendered by `sub_821B5C90` (only function in the binary that references
  "LOADING..." and its five translations) via its private `dword_831E49B0` /
  `dword_831E49DC` texture-handle slots.
- Poisoned color = freed-handle debris leaked into the vertex color lane, which
  here doubles as the per-vertex material/packed-handle field used by
  `sub_828C2290` (per the stride-36 layout analysis in doc 02).

**The draw is one of the two loading-screen sprites emitted by `sub_821B5C90`**:

- **Strong candidate**: the *loading-screen artwork panel* that anchors at the
  upper-left of the screen — the same frame that carries the "LOADING..." label,
  the game-mode art (Liberty borough illustration / save-pickup icon / DLC TLAD /
  TBOGT cover) plus the controller-press bar along the bottom.

- **Secondary candidate**: the *loading-bar fill* sprite — "HUD_LOADING_BAR" is
  referenced from `sub_821C5C78` (HUD widget registry) but the render is
  routed through `sub_821B5C90` via the same `sub_8227F5B8` wrapper. The 15% /
  37% screen position is consistent with the bar's left anchor in GTA IV's
  actual loading layout.

**Neither call site touches `sub_82163270`** (the in-game HUD: AMMO, CASH,
AREA_NAME, STREET_NAME, WANTED_BACK, WEAPON_ICON, WEAPON_DOT) or `sub_821C5C78`
(HUD_RADAR, HUD_SUBTITLES, HUD_MP_NAME_ICON) — so the widget is **not** any of
the normal in-game HUD elements. It's the boot/loading screen UI specifically.

This aligns with the live-observed crash timing: the UAF fires while the game is
still on the loading flow (before the in-game HUD is ever ticked). The first
post-load frame that tries to re-submit a cached loading-screen sprite handle
dereferences a slot whose backing texture has already been freed (the `0xE1E1E1`
stamp is overwritten heap); the handle pointer walks into that region and the
subsequent vertex-color write pulls `0xFFE1E1E1` into the vertex record.

## Ranked suspects (narrowed from agent 1's 6 candidate sites)

1. **Loading-screen sprite panel** (`0x821B6E9C` call, handle = `dword_831E49DC`,
   AABB-driven rect corners) — **most likely**; rectangle math + upper-left
   anchor match the in-game loading-screen layout.
2. **Loading-screen auxiliary icon** (`0x821B6520` call, handle =
   `dword_831E49B0`, 4-float packet built from float-scaled int sources) —
   secondary; same function, sibling draw.
3. Not a radar blip: `[RADAR_BLIP]` is string-referenced from `sub_821F3578`
   (in-game only), unrelated to `sub_8227F5B8`.
4. Not a controller-press indicator: "controller" strings only from
   `sub_82255AF0` (input layer) and `sub_82298E70` (audio) — not from the
   wrapper call tree.
5. Not a CD/spinner icon: `cd_spinner` is a `data_82B27FE8` asset label, not
   reached through `sub_821B5C90`.

## Verification handles for agent 15 (next hop)

- `dword_831E49B0` and `dword_831E49DC` are **the two handle slots whose
  lifetime is the UAF**. They're set/torn-down exclusively by
  `sub_821B5C90`'s owner thread — the loading-screen controller.
- The freeing side (heap side) should be traceable by instrumenting writes to
  both addresses; the values living there at the moment of the poisoned draw are
  dangling pointers into regions that have been freed and restamped with the
  `0xE1` pattern.
- The 4 vertices all share the same poisoned color because
  `sub_828C2290`-style emission duplicates `r9` (the packed color word) across
  each vertex record — see doc 02 for the exact f1..f6/r9/f7/f8 layout.
