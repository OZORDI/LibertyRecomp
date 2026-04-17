# 04 — sub_82A44820 / sub_82A44838 / sub_82A44848 ("grcDevice Lock shim trio")

Agent 4 of 15. Research only, no code changes.

## TL;DR — These are NOT Lock/Unlock shims

The diff-analysis label "grcDevice Lock shim trio" is a misnomer. These three
functions are **Xenos surface-address calculators** — tiny leaf trampolines
that forward into `sub_82A44168` and `sub_82A441F8` (the real surface-offset
math routines, which in turn call `sub_82A440A0` → `sub_82A4A3C8`).

They do not lock anything. They do not touch grcDevice state. They compute an
offset into the texture's tiled/swizzled pixel buffer (GPU VA), based on:

- texture descriptor ptr (r3)
- a mip level / slice index
- an optional (x,y) or (x,y,z) sub-rect origin

…and write the resulting **(pitch, offset-from-base)** or
**(slice-pitch, row-pitch, offset-from-base)** tuple into a caller-supplied
output struct.

The base address itself comes from inside `sub_82A440A0` via a call to
`sub_82A4A3C8` (the actual "get GPU-VA base for this surface" routine).

## Generated recomp — full bodies

All three live in `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.82.cpp`.

### sub_82A44820 (24 bytes, leaf, 3 callers)

```cpp
// r3=texture, r4=mip, r5=out, r6=z_or_flags, r7=y, r8=x  →  forwards as
// r3=texture, r4=0, r5=mip, r6=y_or_flags, r7=z, r8=x  to sub_82A44168
PPC_FUNC_IMPL(__imp__sub_82A44820) {
    ctx.r8 = ctx.r7; ctx.r7 = ctx.r6; ctx.r6 = ctx.r5; ctx.r5 = ctx.r4;
    ctx.r4.s64 = 0;
    sub_82A44168(ctx, base);   // tail call
}
```

### sub_82A44838 (8 bytes, leaf)

```cpp
// Pure tail-jump into sub_82A44168 — no argument shuffling.
PPC_FUNC_IMPL(__imp__sub_82A44838) { sub_82A44168(ctx, base); }
```

### sub_82A44848 (8 bytes, leaf)

```cpp
// Pure tail-jump into sub_82A441F8.
PPC_FUNC_IMPL(__imp__sub_82A44848) { sub_82A441F8(ctx, base); }
```

## What the real routines return

### sub_82A44168 (called by …820 and …838)

Writes a **2-word struct** to `r29` (the pointer passed in the caller's r6
before the arg-shuffle — i.e. the output ptr):

```
out[0] = r9   // width / pitch (from stack slot 84 inside sub_82A440A0)
out[1] = offsetWithinSurface   // tiled-offset + bpp-adjusted row/col delta
```

Offset math (abbreviated from the PPC scaffold):

```
row_delta    = texel_h * pitch
col_delta    = (bpp_lookup[fmt & 0x3F] * texel_w) >> 3
final_offset = row_delta + col_delta + sub_82A440A0_output[80]
```

The final `out[1]` is the **offset from surface base** — NOT an absolute
guest pointer. It is `{ui-writable-row, sub-rect-origin-in-bytes}`.

### sub_82A441F8 (called by …848)

Writes a **3-word struct** to `r29`:

```
out[0] = slicePitch (r7, from stack slot 88)
out[1] = rowPitch   (r8, from stack slot 84)
out[2] = offsetWithinSurface     // row + col + slice delta + base offset
```

This is the volume-texture / cubemap / array variant. It adds a third term
for the Z/slice offset (`r10 = r5 * r8`  where `r5 = caller_struct[16]`, the
slice stride).

### sub_82A440A0 (the shared worker)

- Calls `sub_82A43BE0` — this populates four stack output slots
  (80, 84, 88, 92) with format/pitch/dims from the texture descriptor.
- Calls `sub_82A42F10` — tile/swizzle transform on the (x,y) pair.
- **Calls `sub_82A4A3C8(texture, 14, …)`** — this returns the **GPU base
  pointer** for the surface in `r3`, which is then added to
  `caller_struct[0]` and stored back into `r30[0]` (the caller's pitch-out
  slot). `sub_82A4A3C8` is the "GetPhysicalGpuAddress(texture, mipLevel=14)"
  oracle — and it is currently **unhooked**.

## Callers (all three share the exact same caller set)

```
sub_828D9438  grcRenderTargetXenon_rage::vfunc[16]   (MapRenderTarget?)
sub_828D9AC8  grcTextureXenon_rage::vfunc[23]        (*** crash site ***)
sub_828D9DC8  grcTextureXenon_rage::vfunc[16]        (MapTexture?)
```

Inside `sub_828D9DC8` and `sub_828D9AC8`, the three shims are selected by a
switch on the texture **dimensionality/type** byte at `texture[112]`:

| r22 value | shim called | out-struct shape |
|-|-|-|
| 1 | sub_82A44838 | 2-word (pitch, offset) |
| 3 | sub_82A44848 | 3-word (slice, row, offset) |
| other | sub_82A44820 | 2-word (pitch, offset), arg-shifted |

`sub_828D9AC8` then uses those outputs as the **src** and **dst** arguments
to downstream texture upload / memcpy paths (this is where the bogus
`dst=0xBFF8` came from).

## Host-side hook status

Search across `LibertyRecomp/gpu/video.cpp`:

- `sub_82A44820` — **NOT HOOKED**
- `sub_82A44838` — **NOT HOOKED**
- `sub_82A44848` — **NOT HOOKED**
- `sub_82A44168` — **NOT HOOKED**
- `sub_82A441F8` — **NOT HOOKED**
- `sub_82A440A0` — **NOT HOOKED**
- `sub_82A4A3C8` — **NOT HOOKED** (the GPU-address oracle — critical)

### sub_82A44850 and sub_82A44970 (per project memory)

Both are referenced in `video.cpp` at **two locations**:

1. Lines 9189-9190 (earlier, active hooks named `GTAIV_CreateVertexBuffer`
   and `GTAIV_CreateTexture`).
2. **Lines 9308-9309 inside an `#if 0 … #endif` block** with comment
   "DISABLED — parameter layout investigation needed" (lines 9303-9322
   disable several more: `sub_82A44A98 GetSurfaceDesc`,
   `sub_82A479B0 LockTextureRect`, `sub_82A47AE0 UnlockTextureRect`,
   `sub_82A47C80 LockVertexBuffer`, `sub_82A47E28 UnlockVertexBuffer`).

So the project-memory note is half-right: `sub_82A44850`/`970` are hooked as
`GTAIV_CreateTexture` / `GTAIV_CreateVertexBuffer` earlier in the file, but
the later cleaner "`CreateTexture` / `CreateVertexBuffer`" hooks (and all
Lock/Unlock hooks) are **disabled**.

## The crash chain

1. Game calls `grcTextureXenon::vfunc[23]` = `sub_828D9AC8` (probably the
   "CopySubresource" / "UpdateSubresource" vfunc) on a texture that was
   allocated via the hooked `sub_82A44850` (`GTAIV_CreateTexture`).
2. The hook allocates a host `GuestTexture*` struct, **but does not populate
   the fields the unhooked Xenos surface-address math reads** — specifically
   `texture[32]` (format word used to index the 126-byte bpp table at
   `lbl_XXXX` + 984 + 1) and `texture[48]` (another format bitfield).
3. `sub_828D9AC8` reads `texture[9]` / `texture[28]` / `texture[30]` /
   `texture[32]` (see recomp head-of-function bounds checks) and since
   they're zero/garbage, the `cmpw` comparisons take the "matched" branch.
4. It then calls one of the Lock shims (`sub_82A44838` in the commonest
   path), which forwards into `sub_82A44168` → `sub_82A440A0`, which asks
   `sub_82A4A3C8` for the GPU base address.
5. `sub_82A4A3C8` returns whatever raw recomp gives it — typically
   `texture[some_offset]` read as a 32-bit value, which for a host
   `GuestTexture*` is **a field of a C++ object, not a guest GPU VA**. The
   low 16 bits happen to be `0xBFF8`.
6. Caller adds that to a small row/column delta → `dst = 0xBFF8`, which is
   in guest low memory (not mapped) → memcpy crashes.

## Dependency chain — do Lock shims depend on prior Create shim calls?

**Yes, critically.** The Xenos surface-address math at `sub_82A440A0`
dereferences fields at `+24`, `+32`, `+48` of the texture descriptor that
are set up in the **original** `sub_82A44850` code path. The LibertyRecomp
`GTAIV_CreateTexture` hook creates a host `GuestTexture` struct with a
different memory layout, so when the unhooked `sub_82A440A0` reads those
offsets, it reads meaningless C++ object internals.

In other words: **hooking the Create side but not the Lock-calculate side
is the exact wrong combination** — the original code would have written the
guest descriptor, the unhooked Lock math would have read it back. With the
host hook in place, the descriptor writes never happen, but the reads still
do.

## Fix options (for the follow-up implementation agent)

1. **Safest**: hook `sub_82A44820`, `sub_82A44838`, `sub_82A44848` to
   short-circuit and return a sentinel that `sub_828D9AC8` treats as
   "skip this upload", matching what `LockTextureRect` in video.cpp
   already does with `texture->mappedMemory`.
2. **Correct**: hook `sub_828D9AC8` (vfunc[23]) and `sub_828D9DC8`
   (vfunc[16]) wholesale — replace with `UpdateTexture` / `MapTexture`
   that go through the existing `LockTextureRect` / `UnlockTextureRect`
   host path.
3. **Invasive**: also hook `sub_82A4A3C8` so any other Xenos surface math
   that sneaks through gets a safe base.

Option 2 is the one consistent with how `GTAIV_CreateTexture` already
redirects the allocation side — it makes the whole Xenos texture pipeline
pass through the host VFS, instead of leaving a half-hooked seam.

## File paths

- Recomp: `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.82.cpp`
  (sub_82A44168, sub_82A441F8, sub_82A440A0, sub_82A44820/38/48/50/970)
- Recomp: `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.67.cpp`
  (sub_828D9438, sub_828D9AC8, sub_828D9DC8)
- Host hooks: `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/gpu/video.cpp`
  lines 9189-9190 (active), 9303-9322 (disabled block)
