# Agent 10 — RAGE Freed-Heap Poison Writer (0xE1E1E1E1)

## Summary

The RAGE "fill freed block with poison" helper is `sub_82847160` (not a vtable
entry — a *generic fill primitive* shared across allocators). It reads the
4-byte fill pattern from a global and stamps it over the freed block. When the
configured byte is `0xE1`, freed memory ends up covered in `0xE1E1E1E1` — the
canonical RAGE UAF marker.

## Why the generated recomp never contains a literal `0xE1E1E1E1`

PPC has no 32-bit immediate encoding for a non-small constant used by a store.
The fill byte is held in a **data-section byte table**, not an immediate. The
recomp for `sub_82847160` reads it with `lbz`, then compares all 4 bytes; if
they are equal it tail-calls `rexcrt_memset` for the fast path. The byte value
(e.g. `0xE1`) is never baked into any instruction, so grepping for `0xE1E1E1E1`
/ `57825` / `225` / `-505290271` / `-2021161081` across
`glue/rexglue-sdk-main/gta4-recomp/generated/` legitimately returns zero hits.

(The `lis r3,-31; ori r3,r3,57825` hits in gta4_recomp.0/6/12/38.cpp build
`0xFFE1E1E1`, but those are ARGB color constants fed to UI draw call
`sub_821F2E30`, not free-time fill.)

## The poison-writer: `sub_82847160` @ `0x82847160`

```
File: glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.62.cpp
Size: ~0xE8 (~232 bytes), leaf-ish (no outgoing recomp calls; tail-calls rexcrt_memset)
```

Recomp-level behavior (paraphrased from the scaffold):

```
void sub_82847160(u8* dst /*r3*/, u32 pattern_byte /*r4, only .u8 used*/,
                  u32 len /*r5*/) {
    u8 kill_flag = PPC_LOAD_U8(0x82085FD1);      // "fill freed blocks?" global
    if (kill_flag == 0) {                         // OFF → degenerate fill
        // overwrites r11(=r3) with itself — effectively does nothing useful,
        // used as a no-op branch target
        while (len--) *dst++ = (u8)(u32)dst;      // (irrelevant path)
        return;
    }

    u8* pat_tab = (u8*)0x82795E38;                // 4-byte pattern table
    u8 b3 = pat_tab[3], b2 = pat_tab[2],
       b1 = pat_tab[1], b0 = pat_tab[0];

    if (b3 == b2 && b3 == b1 && b3 == b0) {       // all four bytes identical
        // TAIL-CALL rexcrt_memset(dst, r4, len)  ← fast path for 0xE1E1E1E1 fill
        rexcrt_memset(ctx, base);                 // b 0x82a11d08
        return;
    }

    // else: 4-byte word fill (pattern not uniform)
    u32 word = *(u32*)pat_tab;                    // e.g. 0xDEADC0DE
    ... stw r9, 0(r10); addi r10, r10, 4 ...      // word loop
    ... rotate + stb remainder for tail bytes ...
}
```

So when `0x82795E38..0x82795E3B = { 0xE1, 0xE1, 0xE1, 0xE1 }` (the retail
build's configured RAGE free fill), this function becomes a straight
`memset(dst, 0xE1, len)` — and that is what stamps `0xE1E1E1E1` into every
released block.

### Key addresses

| item | address |
|-|-|
| poison writer (PPC fn) | `0x82847160` |
| tail-call target (memset fast path) | `rexcrt_memset` @ `0x82A11D08` |
| fill-enable flag (u8) | `0x82085FD1` |
| fill-pattern table (4×u8) | `0x82795E38` (a.k.a. `0x82795D58` + `0xE0`) |

(Addresses computed in Python from the recomp `lis/addi` pairs:
`(-2095513600 & 0xFFFFFFFF) + 10193 = 0x82085FD1`,
`(-2102394880 & 0xFFFFFFFF) + 28728 = 0x82795E38`.)

## Callers (`find_callers sub_82847160`, 5 hits)

Every caller is a `Free`-side heap-exit path.

| caller | role |
|-|-|
| `sub_827FEA38` | `rage::fragHeapAllocator::Free` (vtable `fragHeapAllocator_rage` slot 2 @ `0x82080664`) — **definitive RAGE free that poisons** |
| `sub_827F5B40` | helper invoked by `fragHeapAllocator::Free` and `sub_82173C38`/`sub_82173CF0`-aware sibling; fill-before-coalesce branch |
| `sub_82847C08` | sub-block "clear bucket" helper, called exclusively from `sub_821B3770` (fragment system drain) |
| `sub_82848750` | `sysMemSimpleAllocator` main carve/free slow path (size ≈ 0x418, called from `sub_828493E0` allocate and from the free wrapper `sub_828494D8` when the small-bucket fast path fails). This is where the simple allocator fills **both** freshly carved and just-released blocks. |
| `sub_82A581B0` | `sysMemExternalAllocator` internal dispose path (paired with `sub_82A59080` coalesce) |

## Related allocator Free entry points (not direct callers, but the vtable dispatchers above them)

These are the public `Free(void*)` entry points players hit; each routes down
to `sub_82847160` one or two calls deep.

| class | vtable addr | slot | Free fn |
|-|-|-|-|
| `rage::sysMemSimpleAllocator` | `0x82084ae4` | [3] | `sub_828494D8` (calls `sub_82848B68` fill-with-0xDD path + `sub_82848750` path that reaches `sub_82847160`) |
| `rage::sysMemBuddyAllocator` | `0x820848cc` | [3] | `sub_82847D48` |
| `rage::sysMemExternalAllocator` | (RTTI `0x821024c8`) | [2] | `sub_82A58090` → `sub_82A59648` / `sub_82A581B0` |
| `rage::fragHeapAllocator` | `0x82080664` | [2] | `sub_827FEA38` (directly calls `sub_82847160`) |

## Secondary note on `0xDD` fill in `sub_82848B68`

The `sysMemSimpleAllocator` free core at `sub_82848B68` *also* calls
`sub_829FF840(ptr, 221, size)` — that's a different optimised memset filling
with `0xDD` (MSVC `_bFreeFill`-style). `0xDD` blocks are distinct from the
`0xE1` poison written by `sub_82847160` via simple-allocator's `sub_82848750`
path. Seeing `DD DD DD DD` vs. `E1 E1 E1 E1` in a UAF dump tells you which
allocator released the block.

## For the UAF investigation

Any use-after-free that finds a handle word or object pointer equal to
`0xE1E1E1E1` (or, tellingly, a 32-bit value derived from it — e.g. an index of
`0xE1E1` or a partial `0x??E1E1E1` from a misaligned read) went through one of
the five callers above. The two highest-probability suspects for gameplay
handle UAFs are:

1. `rage::fragHeapAllocator::Free` (`sub_827FEA38`) — fragment/skeleton heap.
2. `rage::sysMemSimpleAllocator` slow-path free (`sub_82848750` from
   `sub_828494D8`) — main-heap medium/large allocations.

Hook `sub_82847160` (prologue) and log `r3` (dst) + `r5` (len) + caller-LR to
prove which Free caused a given poisoned-block report at runtime.
