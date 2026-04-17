# 10 — Guest memory layout around 0xC000

Agent 10 of 15. Research only.

## TL;DR

Both `0xBFF8` **and** `0xC000` sit inside rexglue's hard-protected **zero-page
guard**, which spans `0x00000000 – 0x0000FFFF` (64 KB) and is explicitly
allocated with `kMemoryProtectNoAccess`. Neither address is a real runtime
target — not EDRAM, not tile memory, not a heap. The dst value `0xBFF8` is
garbage produced by reading a host `GuestTexture*` as if it were a guest GPU
VA (see doc 04 for the full chain).

The task framing that "0xBFF8 is valid, 0xC000 is not" is **wrong under
rexglue's current layout** — on Windows and Linux/macOS both default to
`protect_zero=true`. The fault is reported at `0xC000` only because that is
where the bswap32 store happens to touch a fresh OS page the kernel decides
to trap on; the whole `[0xBFF8, 0xC0F8)` range is PROT_NONE.

## Resolve-address results

All four queries return "not in symbol table" — expected: these are inside
the zero-page guard, below the XEX image base of `0x82000000`.

| Address | Resolver |
|-|-|
| 0x0000BFF8 | not in symbol table |
| 0x0000BFFF | not in symbol table |
| 0x0000C000 | not in symbol table |
| 0x0000C100 | not in symbol table |

## Guest memory layout (rexglue v0.7.5, `xmemory.cpp:129`)

rexglue clones xenia-canary's heap topology verbatim:

| Range | Heap | Type | Page | Notes |
|-|-|-|-|-|
| 0x00000000–0x3FFFFFFF | `v00000000` | GuestVirtual | 4 KB | Low 64 KB guarded |
| 0x40000000–0x7EFFFFFF | `v40000000` | GuestVirtual | 64 KB | |
| 0x7F000000–0x7FFFFFFF | MMIO | — | — | GPU regs, dispatched via `MMIOHandler` |
| 0x80000000–0x8FFFFFFF | `v80000000` | GuestXex | 64 KB | PE image |
| 0x90000000–0x9FFFFFFF | `v90000000` | GuestXex | 4 KB | |
| 0xA0000000–0xBFFFFFFF | `vA0000000` | GuestPhysical | 64 KB | Physical alias (64k pages) |
| 0xC0000000–0xDFFFFFFF | `vC0000000` | GuestPhysical | 16 MB | Physical alias (16M pages), GPU writeback |
| 0xE0000000–0xFFCFFFFF | `vE0000000` | GuestPhysical | 4 KB | Physical alias (4k pages) |

The zero-page guard is installed explicitly by
`xmemory.cpp:191` right after heap initialization:

```cpp
// Protect the first and last 64kb of memory.
heaps_.v00000000.AllocFixed(0x00000000, 0x10000, 0x10000,
                            memory::kMemoryAllocationReserve | memory::kMemoryAllocationCommit,
                            !REXCVAR_GET(protect_zero)
                                ? memory::kMemoryProtectRead | memory::kMemoryProtectWrite
                                : memory::kMemoryProtectNoAccess);
```

And the cvar default is **true** (`xmemory.cpp:31`):

```cpp
REXCVAR_DEFINE_BOOL(protect_zero, true, "Memory",
                    "Protect the zero page from reads and writes")
```

So `0x0–0xFFFF` is `PROT_NONE`. Any access in that range traps. The
64 KB guard includes the full `[0xBFF8, 0xC0F8)` memcpy footprint.

## Math (Python)

```text
dst   = 0xBFF8
size  = 0x100
end   = 0xC0F8
guard = [0x00000000, 0x00010000)

0xBFF8 in guard?  True   (0xBFF8 < 0x10000)
0xC000  in guard?  True   (0xC000  < 0x10000)
0xC0F8  in guard?  True   (0xC0F8  < 0x10000)
```

The entire 256-byte copy lives inside the PROT_NONE region. Every byte of
the write is a fault; the OS just reports the fault address at whichever
page boundary/granule its trap happens to resolve first (on macOS/arm64 the
VM subsystem typically reports the first unmapped cache-line or 16 KB page
boundary, which is why `0xC000` shows up in the crash record even though
`0xBFF8` is equally illegal).

## Is 0xC000 "EDRAM"?

No. The task's hypothesis that "0xC000 is EDRAM" is a category confusion:

- **Xenos EDRAM is 10 MB** (`0xA00000` bytes), not 64 KB. It's a GPU-side
  SRAM addressed by tile ID, never exposed at guest address `0x0–0x10000`.
- On the real Xbox 360, guest-visible **GPU writeback** lives at the
  physical range **`0xC0000000–0xDFFFFFFF`**, set up in rexglue by
  `heaps_.vC0000000.AllocFixed(0xC0000000, 0x01000000, …)` (xmemory.cpp:201)
  — not in the low 64 KB.
- `LibertyRecomp/gpu/gtaiv_render_state.h` confirms: the "EDRAM base"
  referenced by the render state is a device-struct field (device offset
  1784), not a guest pointer into low memory.

`0xBFF8` is therefore **not a truncated EDRAM offset**. It's a 16-bit
slice of some other 32-bit value that was produced by reading fields of a
host C++ object (`GuestTexture*`) as if they were big-endian guest words.

## Where 0xBFF8 actually comes from

Per doc 04 (`04-grcdevice-lock-shims.md`), the chain is:

1. `grcTextureXenon::vfunc[23]` (`sub_828D9AC8`) receives a `GuestTexture*`
   created by `GTAIV_CreateTexture` (host hook at
   `LibertyRecomp/gpu/video.cpp:9189`).
2. The texture's Xenos descriptor fields (`+24`, `+32`, `+48`) were never
   populated — the host hook builds a different struct layout.
3. `sub_828D9AC8` calls the unhooked Xenos surface-address math
   (`sub_82A44820/38/48 → sub_82A44168 → sub_82A440A0 → sub_82A4A3C8`).
4. `sub_82A4A3C8` is also unhooked — it reads another field of the host
   C++ object as a 32-bit BE word. The low 16 bits of whatever pointer or
   handle it reads happen to be `0xBFF8`.
5. `sub_82A44168` writes `{pitch, offset = 0xBFF8}` into the caller's
   out-struct. `sub_828D9AC8` uses that `offset` directly as `dst` for a
   memcpy → guest `PPC_STORE_U32(0xBFF8, …)` → host page fault in the
   zero-page guard.

## Could 0xBFF8 ever be valid?

Only if someone disables `protect_zero`. That's a setting, not a fix —
it would replace the page-fault with a silent corruption of whatever the
OS leaves at host `base + 0xBFF8`. On rexglue's file-backed mapping that's
still whatever bytes the temp file was initialized with (zeros), i.e. the
game would stomp its own reserved low 64 KB. Not acceptable.

The real fix is the one already proposed in doc 04: either short-circuit
the three Lock shims (Option 1) or — preferred — hook `sub_828D9AC8` and
`sub_828D9DC8` wholesale so the whole Xenos texture upload path goes
through `LockTextureRect` / `UnlockTextureRect` in
`LibertyRecomp/gpu/video.cpp` (currently `#if 0`'d at lines 9303–9322).

## File paths

- `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/src/system/xmemory.cpp`
  (lines 129–219: heap init, zero-page guard)
- `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/include/rex/ppc/memory.h`
  (lines 117–122: Xbox 360 memory-map comment, MMIO range)
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/memory.cpp`
  (defers to rexglue — no custom low-memory mapping)
- `/Users/Ozordi/Downloads/LibertyRecomp/docs/post-fix-crash-research/04-grcdevice-lock-shims.md`
  (full crash-chain analysis, preferred fix)
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/gpu/video.cpp`
  (lines 9189–9190 active; 9303–9322 disabled Lock/Unlock hooks)
