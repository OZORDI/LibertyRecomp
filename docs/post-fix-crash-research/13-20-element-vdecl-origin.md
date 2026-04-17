# 20-Element VDECL Origin — Post-Fix Crash Research (Agent 13)

## Direct answer

The 20-element D3DVERTEXELEMENT9 array at guest stack `0x7010F7F0`
is built by **`sub_828D0A40`** — the RAGE `grmGeometryQB`
material-driven vertex declaration builder. It is not a 2D HUD quad.
It is a skinned / multi-UV / tangent-space mesh decl.

## Call chain captured by VD-FIX trace

```
sub_828D0A40  (material-driven VDECL builder, 64-slot stack scratch)
  -> sub_828C09F0 @ +0x140 == 0x828C0B30   [LR in capture]
       -> sub_82A42A38  (allocates decl, copies into runtime object)
            -> sub_82A42948 (fills header, NtDevice.CreateVertexDeclaration analog)
```

`LR = 0x828C0B30` is exactly `sub_828C09F0 + 0x140` (verified via
`resolve_address`). `sub_828C09F0` is called from five sites; only
`sub_828D0A40` iterates up to four channels and produces the
large element counts we saw.

## sub_828C09F0 — the "element array -> D3DVERTEXELEMENT9[]" transcoder

Signature: `sub_828C09F0(int *descArray, int descCount)`

Per descriptor (7 DWORDs on the stack = 28 bytes):
- `*(v9-4)` = stream index  (triggers offset reset on stream change)
- `*(v9-3)` = usage (POSITION/NORMAL/TEXCOORD/...)
- `*(v9-2)` = usage index
- `*(v9-1)` = size-in-bytes (advances per-stream offset `v6`)
-  `*v9`    = type code index -> `dword_82B0B498[type]` -> D3D type+method
- `*(v9+4)` = stream dword (written once per new stream)
- `byte_82B0B4D8[method]` = D3DDECLMETHOD

For each descriptor the builder writes a 12-byte
`D3DVERTEXELEMENT9`: `{u16 stream, u16 offset, u8 type, u8 method, u8 usage, u8 usageIndex}`.
After the loop it appends the standard 12-byte `D3DDECL_END()`:
```
v19[0] = 0x00FF0000     // end marker (usage=0xFF)
v19[1] = -1
v19[2] = v20            // tail byte
```
then calls `sub_82A42A38` with the terminated array. That tail
sentinel (`0xFF`) is exactly what `sub_82A42A38` scans for to
compute element count.

## sub_828D0A40 — why 20?

Signature: `sub_828D0A40(ch0, ch1, ch2, ch3, cached)` — four
channel descriptors + "allow cache hit" flag.

Allocates **64 slots x 28 bytes** (`v53[]`/`v54[]`) on stack,
iterates the four channel pointers, and for each non-null channel
dispatches to:
- `sub_828CFEB8` if `*(ch+6) != 0` (extended / skinned variant)
- `sub_828D0170` otherwise

`sub_828D0170` is the per-channel emitter. It walks 9 bitflag
channels on `channelDesc[0]` and appends up to ~14 elements:

| Bit | Usage emitted | Notes |
|-|-|-|
| 0x0001 | POSITION (usage 0) | plus per-usage counter via `*a4` |
| 0x0002 | BLENDINDICES (usage 6) | `*a28` counter (skinning) |
| 0x0004 | BLENDWEIGHT (usage 7) | `*a30` counter (skinning) |
| 0x0008 | NORMAL (usage 2) | `*a5` counter |
| 0x0010 | TANGENT (usage 8) | tangent-space |
| 0x0020 | BINORMAL (usage ?) | tangent-space |
| 0x0040..0x2000 | TEXCOORD (usage 5) x 8 | loop `v49` 0..7 via `sub_828D0D90` |
| 0x4000 | COLOR (usage 4) | via `sub_828D0DB8` |
| 0x8000 | PSIZE/specular (usage 3) | via `sub_828D0DE0` |

Type codes come from `dword_82095300[]` (16-entry table) indexed
by 4-bit type nibbles packed in `*(ch+8)`/`*(ch+12)`.

Given `inputCount=20` at the runtime boundary, the most likely
composition is a **full skinned ped/vehicle mesh with tangent
space**:

```
POSITION + BLENDINDICES + BLENDWEIGHT + NORMAL + TANGENT + BINORMAL
  + 8x TEXCOORD + COLOR + PSIZE          = 15 per channel
```

Twenty is reached when one primary channel contributes ~13 and a
second channel (e.g. instance / morph / secondary UV stream) adds
~7, or when a single channel emits all bits (~14) and the builder
also writes a stream-change header for a second non-null channel.
`sub_828D0A40`'s 64-slot buffer is intentionally sized to cover
the worst-case 4 x 14 = 56 elements plus the END sentinel — the
trace shows the 20-element prefix plus END (`0xFF, 0xFF...`).

## Is this the `gta_im` shader pipeline?

Probably yes. `sub_828D0A40` is called by multiple
`grmGeometryQB_rage::vfunc[]` slots (cross-checked via
`find_callers`):
- vfunc[0] = `sub_828E6A98`  -> builds geometry; ends by calling
  `sub_828D0A40` and storing result in `grmGeometryQB+4`
- vfunc[6] = `sub_828E70C0`
- vfunc[8] = `sub_828E5B98`

That means the 20-element decl is produced during
**`grmGeometry`/`grmModel` construction**, which is the same
pipeline that resolves `grmShaderFx` techniques. The `gta_im`
shaders loaded just before the VD-FIX events are the material
programs that consume this layout — they are wired together by
`grmShaderGroup::Lookup` (`sub_828C9728`, visible in
`sub_828C0B48` loading techniques named `draw`, `unlit_draw`,
`drawskinned`, `unlit_drawskinned`, `drawblit`). Those technique
names line up 1:1 with the skinned + tangent-space layout above.

## Why the stack address 0x7010F7F0

`sub_828D0A40` reserves a large stack frame
(`[sp+50h..sp+E8h]` = 0x798 bytes) containing `v53[64 * 28 bytes]`.
The array passed to `sub_828C09F0` is `v53`, i.e. the
function's own on-stack scratch — matching the observed guest
stack address `0x7010F7F0` exactly. The array is **not** copied
from a static template; it is materialised fresh each call from
the channel bitmasks and the `dword_82095300` type table.

## Two Create events at the same LR

Both captured `CreateVertexDeclaration(inputCount=20)` events had
`LR = 0x828C0B30`. That is consistent with two different geometry
objects going through `grmGeometryQB::SetVertexFormat` in
quick succession during initial streaming. The shared LR is the
single call site inside `sub_828C09F0`, not two distinct callers.
Deduplication happens upstream inside `sub_828D0A40` via
`sub_82912948(dword_831C51D4, &key)` — a hash lookup keyed on the
**decl name string** built from `v51` ("no%d" fallback for null
channels). If the key hashes to an entry where `a5==0`, the
cached decl is returned and `sub_828C09F0` is not called at all.
Two hits past that cache means two distinct channel
configurations — likely one skinned ped/vehicle decl and one
non-skinned environment decl emitted by the same frame.

## Relevance to crash

Not suspicious in isolation. 20 elements is legitimate for RAGE
skinned meshes. The failure mode to watch for is whether the
host-side VDECL handler:
1. Interprets `dword_82095300`-derived type codes correctly
   (Xenos extended types for `SHORT2N`, `UBYTE4N`, `DEC3N` etc.)
2. Preserves the stream-change semantics when `stream` field
   changes mid-array (four-channel input -> multi-stream output)
3. Does not truncate at the 16- or 18-element limit some early
   D3D9 translators assume

## Files / symbols

All absolute paths:
- `/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/xex_excavation_retail/pseudocode/sub_828C09F0_0x828C09F0.c`
- `/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/xex_excavation_retail/pseudocode/sub_828D0A40_0x828D0A40.c`
- `/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/xex_excavation_retail/pseudocode/sub_828D0170_0x828D0170.c`
- `/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/xex_excavation_retail/pseudocode/sub_828E6A98_0x828E6A98.c`
- `/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/xex_excavation_retail/pseudocode/sub_82A42A38_0x82A42A38.c`
- `/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/xex_excavation_retail/pseudocode/sub_82A42948_0x82A42948.c`

Key addresses:
- 0x828C09F0 = descriptor -> D3DVERTEXELEMENT9 transcoder
- 0x828C0B30 = call site of `sub_82A42A38` inside the transcoder (LR value)
- 0x828D0A40 = grmGeometryQB material-driven VDECL builder (4 channels, 64-slot stack scratch)
- 0x828D0170 = per-channel element emitter (bitfield-driven)
- 0x82A42A38 = CreateVertexDeclaration allocate + install
- 0x82A42948 = CreateVertexDeclaration header fill (12*count+56 bytes)
- 0x82095300 = 16-entry D3D type/method lookup table (not in symbols)
- 0x82B0B498 = `dword_82B0B498[]` type code translation table used by `sub_828C09F0`
- 0x82B0B4D8 = `byte_82B0B4D8[]` method translation table used by `sub_828C09F0`
- 0x831C51D4 = decl-name hashtable used by `sub_828D0A40` (dedup cache)
